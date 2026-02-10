#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>

// constructeur
BlackScholesMCPricer::BlackScholesMCPricer(Option* option, double initial_price,
    double interest_rate, double volatility)
    : _nb_paths(0), _option(option),_interest_rate(interest_rate),
    _initial_price(initial_price),_volatility(volatility),
    _sumDiscountedPayoffs(0.0),_sumDiscountedPayoffsSquare(0.0)
{
}

int BlackScholesMCPricer::getNbPaths() const {
    return _nb_paths; // on retourne le nombre de trajectoires simulées
}

void BlackScholesMCPricer::generate(int nb_paths) {

    const double rate = _interest_rate;
    const double vol = _volatility;

    const double maturity = _option->getExpiry();
    bool isAsian = _option->isAsianOption(); // on teste si l option est asiatique
    const double discount = std::exp(-rate * maturity); // facteur d actualisation e^{-rT}
    AsianOption* asianOpt = nullptr;
    const std::vector<double>* timeSteps = nullptr;
    std::vector<double> pricePath;

    if (isAsian) {
        asianOpt = dynamic_cast<AsianOption*>(_option);
        if (!asianOpt) {
            throw std::runtime_error("Asian option but dynamic_cast failed");
        }
        timeSteps = &asianOpt->getTimeSteps(); // dates d observation
        pricePath.resize(timeSteps->size()); // allocation du vecteur du chemin
    }
    const double drift_Europeen = (rate - 0.5 * vol * vol) * maturity; // drift pour une option européenne

    const double volTerm_Europeen = vol * std::sqrt(maturity);       // terme de volatilité

    for (int pathIndex = 0; pathIndex < nb_paths; ++pathIndex) {
        double payoff = 0.0;

        if (isAsian && timeSteps && !timeSteps->empty()) {
            // cas d une option asiatique
            double spot = _initial_price;
            double lastTime = 0.0;
            const int nbSteps = static_cast<int>(timeSteps->size());

            for (int step = 0; step < nbSteps; ++step) {
                double currentTime = (*timeSteps)[step];
                double deltaTime = currentTime - lastTime; // pas de temps
                double gaussian = MT::rand_norm(); // notre tirage gaussien N(0,1)
                double driftTerm = (rate - 0.5 * vol * vol) * deltaTime;
                double volatilityTerm = vol * std::sqrt(deltaTime);
                double nextSpot = spot * std::exp(driftTerm + volatilityTerm * gaussian); // dynamique de black scholes
                pricePath[step] = nextSpot; // stockage du point du chemin
                spot = nextSpot;
                lastTime = currentTime;
            }
            payoff = _option->payoffPath(pricePath); // on calcul du payoff à partir du chemin
        }
        else {
            // cas d une option européenne
            double gaussian = MT::rand_norm(); // tirage gaussien
            double terminalSpot = _initial_price * std::exp(drift_Europeen + volTerm_Europeen * gaussian);
            payoff = _option->payoff(terminalSpot); // le payoff à maturité
        }

        double discountedPayoff = discount * payoff; // actualisation du payoff
        _sumDiscountedPayoffs += discountedPayoff; // somme des payoffs actualisés
        _sumDiscountedPayoffsSquare += discountedPayoff * discountedPayoff; // on fait la somme des carrés
    }

    _nb_paths += nb_paths; // mise à jour du nombre total de trajectoires
}

std::vector<double> BlackScholesMCPricer::confidenceInterval() const {

    if (_nb_paths == 0) {
        throw std::runtime_error("No paths generated yet"); // intervalle de confiance à 95 %
    }
    double avg = _sumDiscountedPayoffs / _nb_paths; // notre moyenne empirique
    double var = (_sumDiscountedPayoffsSquare / _nb_paths) - avg * avg; // estimation de la variance
    double stdErr = std::sqrt(var / _nb_paths); // l'erreur standard
    double z95 = 1.96; // quantile pour un intervalle de confiance à 95 %
    return { avg - z95 * stdErr, avg + z95 * stdErr };
}

double BlackScholesMCPricer::operator()() const {
    if (_nb_paths == 0) {
        throw std::runtime_error("No paths generated yet ");
    } // surcharge de l'operateur qui retourne l estimateur du prix
    return _sumDiscountedPayoffs / _nb_paths;
}


