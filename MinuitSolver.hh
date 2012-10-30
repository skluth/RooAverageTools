#ifndef MINUITSOLVER_HH
#define MINUITSOLVER_HH

#include <iostream>
#include <string>

#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TMinuit.h"
#include "TString.h"

// Minuit fcn:
typedef void (*fcn_t)( Int_t&, Double_t*, Double_t&, Double_t*, Int_t );

struct stat_t {
  Double_t min;
  Double_t edm;
  Double_t errdef;
  Int_t npari;
  Int_t nparx;
  Int_t status;
};	

// Interface for myTMinuit function objects:
class MinuitSolverFunction {
public:
  virtual void operator()( Int_t&, Double_t*, Double_t&, Double_t*, Int_t )=0;
};


// Class to handle Minuit fits:
class MinuitSolver {

public:

  // Use plain TMinuit and a standard fcn:
  MinuitSolver( fcn_t fcn, 
		const std::vector<std::string>& parnames, 
		const TVectorD& pars, 
		const TVectorD& parerrors, 
		int ndf, bool quiet=true, int maxpars=50 );

  // Use with function objects:
  MinuitSolver( MinuitSolverFunction& msf, 
		const std::vector<std::string>& parnames, 
		const TVectorD& pars, 
		const TVectorD& parerrors, 
		int ndf, bool quiet=true, int maxpars=50 );

  ~MinuitSolver();

  //getter
  int getNdof() const { return m_ndof; }
  TVectorD getUpar() const { return getPars().first; }
  TVectorD getUparErrors() const { return getPars().second; }
  std::vector<std::string> getUparNames() const { return m_parnames; }
  TMatrixDSym getCovarianceMatrix() const;
  TMatrixDSym getCorrelationMatrix() const;
  int getStatus() const { return getStat().status; }
  double getChisq() const { return getStat().min; }

  //print
  void printResult( bool cov= false, bool cor= false, 
		    TString option = ".4f" ) const;
  void printCovariances() const;
  void printCorrelations() const;

  //other
  void solve() const;
  void minuitCommand( std::string cmd ) const;
  
private:

  std::pair<TVectorD,TVectorD> getPars() const;
  stat_t getStat() const;
  void printPars( TString option= ".4f" ) const;
  void checkMaxpars( Int_t maxpars );
  void setupParameters();
  void initialise( Int_t maxpars, bool quiet );

  std::vector<std::string> m_parnames;
  TVectorD m_pars;
  TVectorD m_parerrors;
  int m_ndof;
  // Must be pointer due to non-constness of TMinuit:
  TMinuit* m_minuit;

};

#endif
