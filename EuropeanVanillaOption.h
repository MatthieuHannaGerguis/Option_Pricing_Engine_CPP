#ifndef PROJECT_EUROPEANVANILLAOPTION_H
#define PROJECT_EUROPEANVANILLAOPTION_H
#include "Option.h"


// Déclaration anticipée pour l'ami
class BlackScholesPricer;


class EuropeanVanillaOption : public Option {
public:
    // Type d’option (enum de "classe"  portée limitée pas de collisions)
    enum class optionType { call, put };

    // Doit être redéfini par CallOption / PutOption
    virtual optionType GetOptionType() const = 0;
    virtual ~EuropeanVanillaOption() = default; // destructeur virtuel


protected:
    
    explicit EuropeanVanillaOption(double expiry, double strike); // Constructeur : initialise T et K, et vérifie non-négativité

    // Accès K pour les classes dérivées (pas public)
    double getStrike() const {
        return _strike;
    }

private:
    double _strike;  // K
    friend class BlackScholesPricer; // accès direct pour le pricer
};
#endif 
