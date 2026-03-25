// FDM solver for 2D PDE grid for barrerier
// inputs should be iniital conditions
// FDM setups a grid, with boundry conditions
// FDM solve the 2D PDE grid using CN and CN+Rannacher
// this FDM also generates greeks


//#pragma once

#include<iostream>
#include<vector>
#include"conditions.h"
#include "FDM2D.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ranges>
#include <numeric>


FDM2D::FDM2D(Initial_Condition &init_cond, const int&num_r,const int&num_c ) 
{
    //num of rows, and cols, 
    //number of rows is count of number of S,
    //number of cols is count of number of t,

    std::array<int,2> mat_size{num_r,num_c}; // num_row * num_col
    auto S_values = GenSValues(num_r,init_cond.dS);//from max to min, vector of S values
    
    auto mat1 = SetUpMat(init_cond.S0,init_cond.K,init_cond.B,init_cond.dS,init_cond.dt,mat_size,S_values);
    CNSolver(mat1,S_values,init_cond.dt,init_cond.sigma,init_cond.dS);
    std::cout<<"the CN solution is: "<<std::endl;
    printMatrix(mat1);

    auto mat2 = SetUpMat(init_cond.S0,init_cond.K,init_cond.B,init_cond.dS,init_cond.dt,mat_size,S_values);
    ImpSolver(mat2,S_values,init_cond.dt,init_cond.sigma,init_cond.dS);
    std::cout<<"the Implicit solution is: "<<std::endl;
    printMatrix(mat2);

    auto mat3 = SetUpMat(init_cond.S0,init_cond.K,init_cond.B,init_cond.dS,init_cond.dt,mat_size,S_values);
    CNRannacherSolver(mat3,S_values,init_cond.dt,init_cond.sigma,init_cond.dS);
    std::cout<<"the CNRannacher solution is: "<<std::endl;
    printMatrix(mat3);
}

Matrix FDM2D::SetUpMat(double&S0,double&K,double&B,double&dS,double&dt,std::array<int,2>& mat_size,const std::vector<double>&S_values)
{
    //num of rows and cols
    int num_r = mat_size[0];
    int num_c = mat_size[1];
    // boundry values at end time(last col), values when S very high and 0(first and last row)  
    //double Smin = (B > 0) ? B : 0.0; // Start grid at barrier if it exists
    //double Smin = 0.0; 

    // Initialize a 2D matrix with zeros
    auto mat = Matrix(num_r, std::vector<double>(num_c, 0.0));

    // update boundry values 
    //for final time at T
    //make a new vector<double> based on Svalues - K
    std::vector<double> payoffs(S_values.size());
    std::transform(S_values.cbegin(), S_values.cend(), payoffs.begin(),
                            [K](double S) { return std::max(S - K, 0.0); });
    ColValueUpdate(mat,num_c-1,payoffs); 

    //first row for Smax-K(no discounting)
    double Smax = S_values.front();
    std::transform(mat[0].begin(), mat[0].end(), 
                            mat[0].begin(),[Smax,K](double ) { return std::max(Smax - K,0.0); });


    //last row for max(Smin-K,0)(no discounting), it is already all zeros
    return mat;
}

std::vector<double> FDM2D::ColValueExtract(Matrix&mat, int col_index)
{// extract the col values into a vector, and return the vector
        std::vector<double> col_values(mat.size());
        std::transform(mat.begin(),mat.end(),col_values.begin(),
                        [col_index](const std::vector<double>& row) {return row[col_index]; });
        return col_values;
}

void FDM2D::ColValueUpdate(Matrix&mat,int col_index, std::vector<double>& new_values)
{// update the col values in the matrix with the new values from the vector
    if (mat.size() != new_values.size())
        throw std::runtime_error("Matrix size doesnt match with new col's size");
    if (col_index <0 || col_index>mat.at(0).size() -1)
        throw std::runtime_error("col_index is less than 0 or larger than matrix columns count");

    for (std::size_t i = 0; i < mat.size() && i < new_values.size(); ++i) {
        if (col_index < mat[i].size()) {
            mat[i][col_index] = new_values[i];
        }
    }

}

void FDM2D::CNSolver(Matrix&mat,std::vector<double>S_values, 
                        const double&dt, const double&sigma,const double&dS)
{
    int num_r = mat.size();
    int num_c = mat[0].size();

    //going through each col from end to start, 
    //and each loop solve the matrix for that col, and update the col, then move to next col
    for (int c = num_c - 2; c >= 0; --c) {
        std::vector<double> V_t_1 = ColValueExtract(mat, c+1);
        double upper_bound = ColValueExtract(mat, c).front(); //uppder bound value time t
        double lower_bound = ColValueExtract(mat, c).back();//lower bound value time t

        //setup the matrices to solve
        // M Vt = rhs, Vt is the wanted one, rhs is function of V(t+1)
        // upper, lower boundry are not used in M but in rhs,
        // thus for example a 5*5 grid is reduced to 3*3 matrix M 

        double scaler= (dt*sigma*sigma)/(dS*dS);
        // build up M and rhs given each row of S value
        auto M = Matrix(num_r - 2, std::vector<double>(num_r - 2, 0.0));
        auto rhs = std::vector<double>(num_r - 2, 0.0);
        

        for (int r = 0; r < num_r - 2; ++r) {
            double S_value_r_squre = S_values[r+1]*S_values[r+1];
            if(r==0)//for the first row of reduced matrix
            {   
                M[r][r] = 1 - 0.5*scaler*S_value_r_squre;
                M[r][r+1] = 0.25*scaler*S_value_r_squre;

                rhs[r] = (1 + 0.5*scaler*S_value_r_squre)*V_t_1[r+1]; 
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*V_t_1[r+2];
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*V_t_1[r];
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*upper_bound;//this is the special treatment
            }
            else if(r==num_r-3)// last row
            {
                M[r][r-1] = 0.25*scaler*S_value_r_squre;
                M[r][r] = 1 - 0.5*scaler*S_value_r_squre;

                rhs[r] = - 0.25*scaler*S_value_r_squre*V_t_1[r]; 
                rhs[r] = rhs[r] + (1 + 0.5*scaler*S_value_r_squre)*V_t_1[r+1];
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*V_t_1[r+2];
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*lower_bound;//this is the special treatment
            }
            else
            {
                M[r][r-1] = 0.25*scaler*S_value_r_squre;
                M[r][r] = 1 - 0.5*scaler*S_value_r_squre;
                M[r][r+1] = 0.25*scaler*S_value_r_squre;

                rhs[r] = - 0.25*scaler*S_value_r_squre*V_t_1[r]; 
                rhs[r] = rhs[r] + (1 + 0.5*scaler*S_value_r_squre)*V_t_1[r+1];
                rhs[r] = rhs[r] - 0.25*scaler*S_value_r_squre*V_t_1[r+2];
            }
        }

        // solve reduced Vt given M and rhs using Thomas algo
        std::vector<double> Vt_redued = ThomasSolver(M,rhs);

        // incoprate reduced Vt to mat
        Vt_redued.push_back(lower_bound);
        Vt_redued.insert(Vt_redued.begin(), upper_bound);
        ColValueUpdate(mat,c,Vt_redued);
    }
}


void FDM2D::ImpSolver(Matrix&mat,std::vector<double>S_values, 
                        const double&dt, const double&sigma,const double&dS)
{// this is the implicit solver, which is unconditionally stable, but it is more computationally expensive than CN
    int num_r = mat.size();
    int num_c = mat[0].size();

    //going through each col from end to start, 
    //and each loop solve the matrix for that col, and update the col, then move to next col
    for (int c = num_c - 2; c >= 0; --c) {
        std::vector<double> V_t_1 = ColValueExtract(mat, c+1);
        double upper_bound = ColValueExtract(mat, c).front(); //uppder bound value time t
        double lower_bound = ColValueExtract(mat, c).back();//lower bound value time t

        //setup the matrices to solve
        // M Vt = rhs, Vt is the wanted one, rhs is V(t+1)
        // upper, lower boundry are not used in M but in rhs,
        // thus for example a 5*5 grid is reduced to 3*3 matrix M 

        double scaler= (dt*sigma*sigma)/(2*dS*dS);
        // build up M and rhs given each row of S value
        auto M = Matrix(num_r - 2, std::vector<double>(num_r - 2, 0.0));
        auto rhs = std::vector<double>(num_r - 2, 0.0);
        

        for (int r = 0; r < num_r - 2; ++r) {
            double S_value_r_squre = S_values[r+1]*S_values[r+1];
            if(r==0)//for the first row of reduced matrix
            {   
                M[r][r] = 1 + 2*scaler*S_value_r_squre;
                M[r][r+1] = - scaler*S_value_r_squre;

                rhs[r] = V_t_1[r+1]; 
                rhs[r] = rhs[r] + scaler*S_value_r_squre*upper_bound;//this is the special treatment
            }
            else if(r==num_r-3)// last row
            {
                M[r][r-1] = - scaler*S_value_r_squre;
                M[r][r] = 1 + 2*scaler*S_value_r_squre;

                rhs[r] = V_t_1[r+1]; 
                rhs[r] = rhs[r] + scaler*S_value_r_squre*lower_bound;//this is the special treatment
            }
            else
            {
                M[r][r-1] = - scaler*S_value_r_squre;
                M[r][r] = 1 + 2*scaler*S_value_r_squre;
                M[r][r+1] = - scaler*S_value_r_squre;

                rhs[r] = V_t_1[r+1];
            }
        }

        // solve reduced Vt given M and rhs using Thomas algo
        std::vector<double> Vt_redued = ThomasSolver(M,rhs);

        // incoprate reduced Vt to mat
        Vt_redued.push_back(lower_bound);
        Vt_redued.insert(Vt_redued.begin(), upper_bound);
        ColValueUpdate(mat,c,Vt_redued);
        }
}

void FDM2D::CNRannacherSolver(Matrix&mat,std::vector<double>S_values, 
                        const double&dt, const double&sigma,const double&dS)
{// this is the CNRannacher solver, 
// which combines Implicit method and CN
    int num_c = mat[0].size();

    //first few loops are the Implicit method on last 2 cols in reduced matrix
    Matrix mat_final2col ;
    for(auto&row : mat)
    {
        std::vector<double> last2items(row.end() - 5, row.end());
        mat_final2col.push_back(last2items);
    };
    ImpSolver(mat_final2col,S_values,dt,sigma,dS);
    std::vector<double> last_col = ColValueExtract(mat_final2col, 4);
    ColValueUpdate(mat, num_c - 1, last_col);
    std::vector<double> last2nd_col = ColValueExtract(mat_final2col, 3);
    ColValueUpdate(mat, num_c - 2, last2nd_col);
    std::vector<double> last3rd_col = ColValueExtract(mat_final2col, 2);
    ColValueUpdate(mat, num_c - 3, last3rd_col);
    std::vector<double> last4th_col = ColValueExtract(mat_final2col, 1);
    ColValueUpdate(mat, num_c - 4, last4th_col);
    std::vector<double> last5th_col = ColValueExtract(mat_final2col, 0);
    ColValueUpdate(mat, num_c - 5, last5th_col);

    //rest are the CN method, to get the rest cols
    Matrix mat_rest ;
    for(auto&row : mat)
    {
        std::vector<double> restitems(row.begin(), row.end()-4);
        mat_rest.push_back(restitems);
    };    
    CNSolver(mat_rest,S_values,dt,sigma,dS);
    int cols_index =mat_rest[0].size();
    for(int c =0; c<cols_index; ++c)
    {   
        std::vector<double> col = ColValueExtract(mat_rest, c);
        ColValueUpdate(mat, c, col);
    }
}



void FDM2D::printGreeks()

{

}

void FDM2D::printValue()
{}

void FDM2D::printMatrix(Matrix&mat)
{//print the matrix, for testing purpose
    for (const auto& row : mat) {
        for (const auto& val : row) {
            std::cout << std::setw(10) << std::fixed << std::setprecision(3) << val << " ";
        }
        std::cout << "\n";
    }

}

std::vector<double> FDM2D::GenSValues(int num_r, const double&dS)
{//generate vector S values
    std::vector<double> S_values(num_r);
    std::iota(S_values.begin(), S_values.end(), 0.0);
    std::transform(S_values.begin(), S_values.end(), 
                            S_values.begin(),[dS](int i) { return i * dS; });
    std::reverse(S_values.begin(), S_values.end());//from max to min
    return S_values;
}

std::vector<double> FDM2D::ThomasSolver(const Matrix&m, const std::vector<double>&rhs)
{//Thomas algo is Gaussian elimination specialized to tridiagonal matrices.
// Thomas doesn work with near-singluar matrix, which has near-zero pivots


/*
    test case
    Matrix m = {
        {2.0, 4.0, 0.0},
        {1.0, 3.0, 5.0},
        {0.0, 2.0, 2.0}
    };
    std::vector<double> rhs = {1.0, 2.0, 3.0};
    results x = {-2.5, 1.5 ,0}
*/
    if (m.size() != m.at(0).size())
        throw std::invalid_argument("The input Matrix m must be square.");
    
    int n = m.size();//
    std::vector<double> a(n,.0);// subdiag (a[0]=0)
    std::vector<double> b(n,.0); // diag
    std::vector<double> c(n,.0);// superdiag (c[n-1]=0)
    std::vector<double> d = rhs;

    for(int r =0; r<n;++r)
    {
        b[r] = m[r][r];
        if (r != 0) a[r] = m[r][r - 1];
        if (r != n - 1) c[r] = m[r][r + 1];
    }

    auto near_zero = [](double x) { return std::abs(x) < 1e-14; };

    std::vector<double> cp(n, 0.0);
    std::vector<double> dp(n, 0.0);

    // Forward sweep to create a up triangular matrix
    double denom = b[0];
    if (near_zero(denom)) throw std::runtime_error("Zero pivot at row 0.");
    cp[0] = c[0] / denom;
    dp[0] = d[0] / denom;

    for (int i = 1; i < n; ++i) {
        denom = b[i] - a[i] * cp[i - 1];
        if (near_zero(denom)) throw std::runtime_error("Zero pivot at row " + std::to_string(i));
        cp[i] = (i == n - 1) ? 0.0 : (c[i] / denom);
        dp[i] = (d[i] - a[i] * dp[i - 1]) / denom;
    }

    // Back substitution, to solve the matrix
    std::vector<double> x(n, 0.0);
    x[n - 1] = dp[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        x[i] = dp[i] - cp[i] * x[i + 1];
    }
    return x;
}


int main()
{   
    Initial_Condition init_cond{8.0, // S0
                            2.0, // K
                            3.0,  // B
                            1.0,   // T
                            0.05,    // sigma
                            0.0,  // r
                            2.0,  // dS
                            1.0,  // dt
                            };

    //init_cond.print();

    int rows =12;
    int cols =200;
    FDM2D fdm2d{init_cond, rows, cols}; // num of rows and cols

    return 0;
}
