//setup initial conditions for barrier and digital
// the initial conditions will be consumed by FDM solver and MC simulator
#pragma once

class Initial_Condition
{
public:
    double S0{};
    double K{};
    double B{};
    double T{};
    double sigma{};
    double r{};
    double dS{};
    double dt{};

    Initial_Condition(double S0_, double K_, double B_, double T_, double sigma_, double r_, double dS_, double dt_);// for barrier

    Initial_Condition(double S0_, double K_, double T_, double sigma_, double r_, double dS_, double dt_);//for digital

    void print();
};
