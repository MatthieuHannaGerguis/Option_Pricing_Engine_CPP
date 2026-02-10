#include "CRRPricer.h"


double power(double x, int n) {
    double result = 1.0;
    while (n > 0) {
        if (n % 2 == 1) result *= x;
        x *= x;
        n /= 2;
    }
    return result; // fonction de puissance 
}

CRRPricer::CRRPricer(Option* option, int depth, double asset_price,
    double up, double down, double interest_rate)
{
    _option = option;
    _depth = depth;
    _up = up;
    _down = down;
    S0 = asset_price;
    _interest_rate = interest_rate;
    _computed = false;


    // we reject Asian options
    if (_option && _option->isAsianOption()) { // -> is a pointer to an object
        throw std::invalid_argument("CRRPricer: Asian options cannot be priced with the CRR model.");
    }

    if (!_option) { //vérification du pointeur
        throw std::invalid_argument("Le pointeur vers l'option est nul (null pointer)");
    }

    if (S0 <= 0.0) { // on verifie les parametres
        throw std::invalid_argument("Initial asset price must be positive.");
    }
    if (_up <= _down) {
        throw std::invalid_argument("Up factor must be greater than down factor.");
    }
    if (_down <= -1.0 || _interest_rate <= -1.0) {
        throw std::invalid_argument("Down factor and interest rate must be > -1.");
    }
    if (_depth <= 0) {
        throw std::invalid_argument("Tree depth must be positive.");
    }

    if (!(_down < _interest_rate && _interest_rate < _up)) {
        // on verifie la condition d’absence d’arbitrage : D < R < U
        throw std::invalid_argument("Arbitrage détecté : il faut avoir D < R < U.");
    }

    // on calcule la probabilité neutre au risque
    _q = (_interest_rate - _down) / (_up - _down);

    _assetTree.setDepth(_depth);
    _valueTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);


    // on cree l'arbre pour le prix de l'actif
    for (int n = 0; n <= _depth; n++) {
        for (int i = 0; i <= n; i++) {
            double S = S0 * pow(1 + _up, i) * pow(1 + _down, n - i);
            // on calcule a partir de la formule de l'enonce
            _assetTree.setNode(n, i, S); // on utilise le setter pour integrer la valeur
        }
    }
}

// Constructeur Black-Scholes approximation
CRRPricer::CRRPricer(Option* option, int depth, double asset_price, double r, double sigma)
    : _option(option), _depth(depth), S0(asset_price)
{
    _computed = false; // on fait en sorte que notre bollean soit bien negatif
    if (!_option) {
        throw std::invalid_argument("Null option pointer.");
    }
    if (_depth <= 0) {
        throw std::invalid_argument("Depth must be positive.");
    }
    if (S0 <= 0.0) {
        throw std::invalid_argument("Asset price must be positive.");
    }
    if (sigma <= 0.0) {
        throw std::invalid_argument("Volatility must be positive.");
    }
    // on divise la période en N intervalles
    double T = _option->getExpiry();
    double h = T / _depth;

    // formules du modèle binomial comme approximation de Black–Scholes
    _up = std::exp((r + 0.5 * sigma * sigma) * h + sigma * std::sqrt(h)) - 1.0;
    _down = std::exp((r + 0.5 * sigma * sigma) * h - sigma * std::sqrt(h)) - 1.0;
    _interest_rate = std::exp(r * h) - 1.0;

    if (!(_down < _interest_rate && _interest_rate < _up)) {
        throw std::invalid_argument("Arbitrage detected in Black-Scholes parameters.");
    }
    if (_up <= _down) {
        throw std::invalid_argument("Inconsistent model: up must be greater than down.");
    } // on verifie la condition d'arbitrage
    _q = (_interest_rate - _down) / (_up - _down);
    _valueTree.setDepth(_depth);
    _exerciseTree.setDepth(_depth);
}

void CRRPricer::compute() {

    double u = 1 + _up;
    double d = 1 + _down;
    std::vector<double> S_line(_depth + 1);
    S_line[0] = S0;
    for (int k = 0; k < _depth; ++k)
        S_line[0] *= d;   // S(N,0) = S0 * d^N
    for (int i = 1; i <= _depth; ++i)
        S_line[i] = S_line[i - 1] * (u / d);
    for (int i = 0; i <= _depth; ++i) {
        double payoff = _option->payoff(S_line[i]);
        _valueTree.setNode(_depth, i, payoff);
        _exerciseTree.setNode(_depth, i, false);
    }
    // on fait de la backward induction
    for (int n = _depth - 1; n >= 0; --n) {
        S_line.resize(n + 1);
        S_line[0] = S0;
        for (int k = 0; k < n; ++k)
            S_line[0] *= d;
        for (int i = 1; i <= n; ++i)
            S_line[i] = S_line[i - 1] * (u / d);
        for (int i = 0; i <= n; ++i) {
            double continuation =
                (_q * _valueTree.getNode(n + 1, i + 1)
                    + (1 - _q) * _valueTree.getNode(n + 1, i))
                / (1 + _interest_rate);
            double nodeValue = continuation;
            bool exercise = false;

            if (_option->isAmericanOption()) {
                double intrinsic = _option->payoff(S_line[i]);
                if (intrinsic >= continuation) {
                    nodeValue = intrinsic;
                    exercise = true;
                }
            }
            _valueTree.setNode(n, i, nodeValue);
            _exerciseTree.setNode(n, i, exercise);
        }
    }
    _computed = true;
}

// Operator() qui permet d'avoir le prix de l'option
double CRRPricer::operator()(bool closed_form) {
    double u = 1 + _up;
    double d = 1 + _down;

    if (closed_form) { // formule fermée vrai lorsqu'on price une option europeene
        // formule fermée du modèle binomial
        double price = 0.0;

        // on pré-calcule u^i et d^(N-i) pour éviter pow dans la boucle
        std::vector<double> uPow(_depth + 1), dPow(_depth + 1);
        uPow[0] = dPow[0] = 1.0;
        for (int k = 1; k <= _depth; ++k) {
            uPow[k] = uPow[k - 1] * u;
            dPow[k] = dPow[k - 1] * d;
        }
        for (int i = 0; i <= _depth; ++i) {
            double S = S0 * uPow[i] * dPow[_depth - i]; // S(N, i)
            double payoff = _option->payoff(S);         // h(S(N, i))

            // coefficient binomial C(N, i) = N! / (i! * (N - i)!)
            double comb = 1.0;
            for (int k = 1; k <= i; ++k)
                comb *= double(_depth - (i - k)) / k; // calcul de factoriel plus stable
            // on ajoute les autres termes de la formule
            price += comb * std::pow(_q, i) * std::pow(1 - _q, _depth - i) * payoff;
        }
        price /= std::pow(1 + _interest_rate, _depth);
        return price;
    }
    // sinon on utilise le backward induction (CRR tree)
    if (!_computed)
        compute();
    return _valueTree.getNode(0, 0);
}

bool CRRPricer::getExercise(int n, int i) const { // retourn true if the option should be exercised early at that node.
    if (i < 0 || n < 0 || i > n || n > _depth) {
        throw std::out_of_range("Invalid CRR tree indices");
    }
    return _exerciseTree.getNode(n, i); //retourne false if it’s better to continue holding.
}

// notre affichage
void CRRPricer::display() const { // ne pas mettre cette fonction publique
    std::cout << "Asset Price Tree:\n";
    _assetTree.display();

    std::cout << "Option Value Tree:\n";
    _valueTree.display();

    if (_option->isAmericanOption()) {
        std::cout << "Early Exercise Tree (1 = exercise):\n";
        for (int n = 0; n <= _depth; ++n) {
            for (int i = 0; i <= n; ++i) {
                std::cout << (_exerciseTree.getNode(n, i) ? "1 " : "0 ");
            }
            std::cout << "\n";
        }
    }
}