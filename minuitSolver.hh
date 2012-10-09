#ifndef MINUIT_SOLVER_HH
#define MINUIT_SOLVER_HH

#include <vector>
#include <iostream>

// #include "TMinuit.h"
// #include "TMath.h"


class minuitSolver
{
	typedef  void (* MinuitFCN_t )(int &npar, double *gin, double &f, double *u, int flag);
	
public:
		minuitSolver(MinuitFCN_t fcn, double* parameters, double* parerrors, 
		std::vector<std::string> const& parnames, unsigned int const expectedNdof, unsigned int const maxParam);
		
	~minuitSolver();
	
private:
	MinuitFCN_t  _fcn;
	unsigned int _expectedNdof;
	unsigned int _maxParam;

};



#endif
