#include "MT.h"
#include <iostream>

MT::MT() : generator(std::random_device{}()) {// on  initialise le générateur avec une graine aléatoire
}

double MT::rand_unif() {       // génère un nombre uniforme
    MT& instance = get_instance();
    return instance.uniform_dist(instance.generator);
}

double MT::rand_norm() { // génère un nombre normal
    MT& instance = get_instance();
    return instance.normal_dist(instance.generator);
}

MT& MT::get_instance() {   // on retourne l’instance unique du singleton
    static MT instance;        // création la première fois, réutilisé ensuite
    return instance;
}

std::uniform_real_distribution<double> MT::uniform_dist(0.0, 1.0); // loi uniforme U(0,1)
std::normal_distribution<double> MT::normal_dist(0.0, 1.0);  // loi normale N(0,1)


