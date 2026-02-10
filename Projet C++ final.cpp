
#include <iomanip> 
#include <iostream>
#include "Option.h"
#include "EuropeanVanillaOption.h"
#include "CallOption.h"
#include "PutOption.h"
#include "EuropeanDigitalCallOption.h"
#include "EuropeanDigitalPutOption.h"
#include "BlackScholesPricer.h"
#include "CRRPricer.h"
#include "BinaryTree.h"
#include "AsianCallOption.h"
#include "AsianPutOption.h"
#include "BlackScholesMCPricer.h"
#include "AmericanPutOption.h"
#include "AmericanCallOption.h"
#include "AmericanOption.h"

int main()
{
    
    ////////////////////// Partie 1 //////////////////////////////////////

    double S0(100), K(101), T(5), r(0.01), sigma(0.1);
    CallOption opt1(T, K);
    PutOption opt2(T, K);

    std::cout << "European options 1" << std::endl << std::endl;

    {
        BlackScholesPricer pricer1(&opt1, S0, r, sigma);
        std::cout << "CALL OPTION EUROPEAN,, BlackScholesPricer price=" << pricer1()
            << ", delta=" << pricer1.delta() << std::endl;

        BlackScholesPricer pricer2(&opt2, S0, r, sigma);
        std::cout << "PUT OPTION EUROPEAN ,BlackScholesPricer price=" << pricer2()
            << ", delta=" << pricer2.delta() << std::endl;
        std::cout << std::endl;

        int N(5);
        double U = 0.05;
        double D = -0.045;
        double R = 0.01;

        CRRPricer crr_pricer1(&opt1, N, S0, U, D, R);
        std::cout << "CALL OPTION EUROPEAN, Calling CRR pricer with depth=" << N << std::endl;
        std::cout << std::endl;
        std::cout << "CRR pricer computed price=" << crr_pricer1() << std::endl;
        std::cout << "CRR pricer explicit formula price=" << crr_pricer1(true) << std::endl;
        std::cout << std::endl;

        CRRPricer crr_pricer2(&opt2, N, S0, U, D, R);
        std::cout << "PUT OPTION EUROPEAN, Calling CRR pricer with depth=" << N << std::endl;
        std::cout << std::endl;
        std::cout << "CRR pricer computed price=" << crr_pricer2() << std::endl;
        std::cout << "CRR pricer explicit formula price=" << crr_pricer2(true) << std::endl;
    }

    std::cout << std::endl << "*********************************************************" << std::endl;

    std::cout << std::endl << "************ AMERICAN OPTIONS ******************" << std::endl << std::endl;

    {
        int N(5);
        double U = 0.05;
        double D = -0.045;
        double R = 0.01;

        AmericanCallOption opt3(T, K);
        AmericanPutOption  opt4(T, K);

        // --- American CALL ---
        {
            CRRPricer crr_pricer3(&opt3, N, S0, U, D, R);
            std::cout << "CALL OPTION AMERICAN, depth=" << N << std::endl;
            std::cout << "CRR pricer price (with early exercise)=" << crr_pricer3() << std::endl;
            std::cout << std::endl;
        }

        // --- American PUT ---
        {
            CRRPricer crr_pricer4(&opt4, N, S0, U, D, R);
            std::cout << "PUT OPTION AMERICAN, depth=" << N << std::endl;
            std::cout << "CRR pricer price (with early exercise)=" << crr_pricer4() << std::endl;
            std::cout << std::endl;
        }
    }

    ////////////////////// Binary Tree Tests /////////////////////////////

    {
        std::cout << "Binary Tree" << std::endl << std::endl;

        BinaryTree<bool> t1;
        t1.setDepth(3);
        t1.setNode(1, 1, true);
        t1.display();
        t1.setDepth(5);
        t1.display();
        t1.setDepth(3);
        t1.display();

        BinaryTree<double> t2;
        t2.setDepth(2);
        t2.setNode(2, 1, 3.14);
        t2.display();
        t2.setDepth(4);
        t2.display();
        t2.setDepth(3);
        t2.display();

        BinaryTree<int> t3;
        t3.setDepth(4);
        t3.setNode(3, 0, 8);
        t3.display();
        t3.setDepth(2);
        t3.display();
        t3.setDepth(4);
        t3.display();

        std::cout << std::endl << "*********************************************************" << std::endl;
    }
    
    ////////////////////// Partie 2 : Digital options //////////////////////
     
    {
        double S0(100), K(101), T(5), r(0.01), sigma(0.1);

        EuropeanDigitalCallOption opt1(T, K);
        EuropeanDigitalPutOption opt2(T, K);

        std::cout << "European options 2" << std::endl << std::endl;

        {
            std::cout << std::fixed << std::setprecision(6);

            BlackScholesPricer pricer1(&opt1, S0, r, sigma);
            std::cout << "CALL OPTION EUROPEAN, BlackScholesPricer price=" << pricer1()
                << ", delta=" << pricer1.delta() << std::endl;

            BlackScholesPricer pricer2(&opt2, S0, r, sigma);
            std::cout << "PUT OPTION EUROPEAN, BlackScholesPricer price=" << pricer2()
                << ", delta=" << pricer2.delta() << std::endl;

            std::cout << std::endl;

            int N(5);
            double U = 0.05;
            double D = -0.045;
            double R = 0.01;

            CRRPricer crr_pricer1(&opt1, N, S0, U, D, R);
            std::cout << "Calling CRR pricer with depth=" << N << std::endl;
            std::cout << "CRR pricer computed price=" << crr_pricer1() << std::endl;
            std::cout << "CRR pricer explicit formula price=" << crr_pricer1(true) << std::endl;

            std::cout << std::endl;

            CRRPricer crr_pricer2(&opt2, N, S0, U, D, R);
            std::cout << "Calling CRR pricer with depth=" << N << std::endl;
            std::cout << "CRR pricer computed price=" << crr_pricer2() << std::endl;
            std::cout << "CRR pricer explicit formula price=" << crr_pricer2(true) << std::endl;
        }

        std::cout << std::endl << "*********************************************************" << std::endl;
    }
    
     //////////////////////////////////////////////// Test 2 /////////////////////////////////////////////////////////////////////////////////////////////////////////
    {

        double S0(100), K(101), T(5), r(0.01), sigma(0.1);
        std::vector<Option*> opt_ptrs;
        opt_ptrs.push_back(new CallOption(T, K));
        opt_ptrs.push_back(new PutOption(T, K));
        opt_ptrs.push_back(new EuropeanDigitalCallOption(T, K));
        opt_ptrs.push_back(new EuropeanDigitalPutOption(T, K));

        std::vector<double> fixing_dates;
        for (int i = 1; i <= 5; i++) {
            fixing_dates.push_back(0.1 * i);
        }
        opt_ptrs.push_back(new AsianCallOption(fixing_dates, K));
        opt_ptrs.push_back(new AsianPutOption(fixing_dates, K));

        std::vector<double> ci;
        BlackScholesMCPricer* pricer;

        for (auto& opt_ptr : opt_ptrs) {
            pricer = new BlackScholesMCPricer(opt_ptr, S0, r, sigma);
            do {
                pricer->generate(10);
                ci = pricer->confidenceInterval();
            } while (ci[1] - ci[0] > 1e-2);
            std::cout << "nb samples: " << pricer->getNbPaths() << std::endl;
            std::cout << "price: " << (*pricer)() << std::endl << std::endl;
            delete pricer;
            delete opt_ptr;
        }
    }
    ////////////////////////////////////     Test 3     ///////////////////////////////////////////////////////
    {
        double S0(100), K(101), T(5), r(0.01), sigma(0.1);
        std::vector<Option*> opt_ptrs;
        opt_ptrs.push_back(new CallOption(T, K));
        opt_ptrs.push_back(new PutOption(T, K));
        opt_ptrs.push_back(new EuropeanDigitalCallOption(T, K));
        opt_ptrs.push_back(new EuropeanDigitalPutOption(T, K));
        opt_ptrs.push_back(new AmericanCallOption(T, K));
        opt_ptrs.push_back(new AmericanPutOption(T, K));

        CRRPricer* pricer;

        for (auto& opt_ptr : opt_ptrs) {
            pricer = new CRRPricer(opt_ptr, 5, S0, r, sigma);

            pricer->compute();

            std::cout << "price: " << (*pricer)() << std::endl << std::endl;
            delete pricer;
            delete opt_ptr;

        }
    }
   
    ///////////////////////////////   test prix option asiatique    ///////////////////////////////////////////////////////////
    {
    double S0 = 100.0;
    double K = 101.0;
    double T = 5.0;
    double r = 0.01;
    double sigma = 0.1;

    // === Fixings === 
    // 5 fixings equally spaced: 1,2,3,4,5
    std::vector<double> fixing_dates;
    for (int i = 1; i <= 5; i++)
        fixing_dates.push_back(T * i / 5.0);
    std::cout << "Fixing dates: ";
    for (double t : fixing_dates)
        std::cout << t << " ";
    std::cout << std::endl;

    // === Create Asian options ===
    AsianCallOption call(fixing_dates, K);
    AsianPutOption  put(fixing_dates, K);

    // === Monte Carlo pricers ===
    BlackScholesMCPricer pricer_call(&call, S0, r, sigma);
    BlackScholesMCPricer pricer_put(&put, S0, r, sigma);

    // Generate Monte Carlo paths
    pricer_call.generate(200000);   // 200k paths = enough
    pricer_put.generate(200000);

    std::cout << "Asian Call Price = " << pricer_call() << std::endl;
    std::cout << "Asian Put Price  = " << pricer_put() << std::endl;
    }
    return 0;

}




