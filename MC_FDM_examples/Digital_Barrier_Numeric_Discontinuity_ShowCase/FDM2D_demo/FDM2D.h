// FDM solver for 2D PDE grid for barrerier
// inputs should be iniital conditions
// FDM setups a grid, with boundry conditions
// FDM solve the 2D PDE grid using CN and CN+Rannacher
// this FDM also generates greeks


#pragma once

#include<iostream>
#include<vector>
#include<array>
#include"conditions.h"

using Matrix = std::vector<std::vector<double>>;



class FDM2D
{
    public:

    FDM2D(Initial_Condition &init_cond,const int&num_r,const int&num_c );



    private:
    Matrix SetUpMat(double&S0,double&K,double&B,double&dS,double&dt,std::array<int,2>& mat_size,const std::vector<double>&S_values);

    std::vector<double> ColValueExtract(Matrix&mat,int col_index);
    void ColValueUpdate(Matrix&mat,int col_index, std::vector<double>& new_values);

    void CNSolver(Matrix&mat,std::vector<double>S_values,                        
                    const double&dt, const double&sigma,const double&dS);

    void ImpSolver(Matrix&mat,std::vector<double>S_values,                        
                    const double&dt, const double&sigma,const double&dS);

    void CNRannacherSolver(Matrix&mat,std::vector<double>S_values, 
                        const double&dt, const double&sigma,const double&dS);


    std::vector<double> ThomasSolver(const Matrix&m, const std::vector<double>&rhs);


    void Greeks();
    void printGreeks();
    void printValue();
    void printMatrix(Matrix&mat);
    std::vector<double> GenSValues(int num_r,const double&dS);
};
