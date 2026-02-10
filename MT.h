#pragma once
#include <random>

class MT {
public:
    
    static double rand_norm();

    //rand_unif retourne un nombre entre 0 and 1
    static double rand_unif();


private:
    MT();  //constructeur

    static MT& get_instance();     //fonction statique


    //copy constructor ainsi que l''operator prive pour avoir un singleton
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;



    // Déclaration des variables privées : 
        // - le générateur aléatoire (generator)
        // - la distribution uniforme (uniform_dist)
        // - la distribution normale (normal_dist)
    std::mt19937 generator;
    static std::uniform_real_distribution<double> uniform_dist;
    static std::normal_distribution<double> normal_dist;
};

