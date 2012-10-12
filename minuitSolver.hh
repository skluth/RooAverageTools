#ifndef MINUIT_SOLVER_HH
#define MINUIT_SOLVER_HH

#include <vector>
#include <iostream>
#include <string>

// #include "TMinuit.h"
// #include "TMath.h"

typedef  void (* MinuitFCN_t )(int &npar, double *gin, double &f, double *u, int flag);

class minuitSolver
{
	
public:
	minuitSolver(MinuitFCN_t fcn, double* parameters, double* parerrors, 
	char** parnames, unsigned int const expectedNdof, unsigned int const maxParam);
	
	~minuitSolver();
	
	void solve(char* option);
	
private:
	minuitSolver(){}
	
	MinuitFCN_t  _fcn;
	unsigned int _expectedNdof;
	unsigned int _maxParam;
};



#endif
