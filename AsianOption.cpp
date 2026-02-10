#include "AsianOption.h"

AsianOption::AsianOption(double expiry,
    double strikeValue,
    const std::vector<double>& timestamps)
    : Option(timestamps.back()),  // expiry = last fixing date
    _strike(strikeValue),
    _timeSteps(timestamps)
{
    if (_timeSteps.empty()) {
        throw std::invalid_argument(" timeSteps cannot be empty");
    }
    for (size_t i = 1; i < _timeSteps.size(); ++i) {
        if (_timeSteps[i] <= _timeSteps[i - 1]) {
            throw std::invalid_argument("timestamps must be increased");
        }
    } // on verifie que les timeSteps sont strictement croissant
    if (_strike < 0.0) {
        throw std::invalid_argument(" strike wrong");
    }
}

const std::vector<double>& AsianOption::getTimeSteps() const {
    return _timeSteps;
}

double AsianOption::getStrike() const {
    return _strike;
}

bool AsianOption::isAsianOption() const {
    return true;
}

double AsianOption::payoffPath(const std::vector<double>& path) const {
    if (path.empty())
        throw std::invalid_argument(" payoffPath is a empty path");

    double avg = std::accumulate(path.begin(), path.end(), 0.0) / path.size(); // on calcule la moyenne des prix du sous-jacent le long du chemin simule
    return payoff(avg); // on applique le payoff sur la moyenne
}
