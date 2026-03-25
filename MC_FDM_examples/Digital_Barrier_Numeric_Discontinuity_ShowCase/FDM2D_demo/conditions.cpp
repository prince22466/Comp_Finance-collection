//setup initial conditions for barrier and digital
// the initial conditions will be consumed by FDM solver and MC simulator
#include<iostream>
#include "conditions.h"

Initial_Condition::Initial_Condition(double S0_, double K_, double B_, double T_, double sigma_, double r_, double dS_, double dt_)
{//for barrier
    S0 = S0_;
    K = K_;
    B = B_;
    T = T_;
    sigma = sigma_;
    r = r_;
    dS = dS_;
    dt = dt_;

};


Initial_Condition::Initial_Condition(double S0_, double K_, double T_, double sigma_, double r_, double dS_, double dt_)
{// for digital
    S0 = S0_;
    K = K_;
    T = T_;
    sigma = sigma_;
    r = r_;
    dS = dS_;
    dt = dt_;

};

void Initial_Condition::print()
{
    std::cout << "S0: " << S0 << std::endl;
    std::cout << "K: " << K << std::endl;
    if (B > 0)
        std::cout << "B: " << B << std::endl;
    else
        std::cout << "B: No Barrier" << std::endl;

    std::cout << "T: " << T << std::endl;
    std::cout << "sigma: " << sigma << std::endl;
    std::cout << "r: " << r << std::endl;
    std::cout << "dS: " << dS << std::endl;
    std::cout << "dt: " << dt << std::endl;

}
