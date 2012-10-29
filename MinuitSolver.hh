#ifndef MINUITSOLVER_HH
#define MINUITSOLVER_HH

#include <iostream>
#include <string>

#include "TVectorD.h"
#include "TMinuit.h"
#include "TString.h"

// Minuit fcn:
typedef void (* fcn_t)( Int_t&, Double_t*, Double_t&, Double_t*, Int_t );

struct stat_t {
  Double_t min;
  Double_t edm;
  Double_t errdef;
  Int_t npari;
  Int_t nparx;
  Int_t status;
};	

class MinuitSolverFunction {
public:
  virtual Double_t calculate( const TVectorD& pars ) const = 0;
};

class MinuitSolver {

private:

  MinuitSolver() {}
  MinuitSolver( const MinuitSolver& ) {}
  ~MinuitSolver();

public:

  static MinuitSolver& getInstance();

  void configure( fcn_t fcn, 
		  const std::vector<std::string>& parnames, 
		  const TVectorD& pars, 
		  const TVectorD& parerrors, 
		  int ndf, bool quiet=true, int maxpars=50 );

  void configure( const MinuitSolverFunction& msf, 
		  const std::vector<std::string>& parnames, 
		  const TVectorD& pars, 
		  const TVectorD& parerrors, 
		  int ndf, bool quiet=true, int maxpars=50 );

  //getter
  int getNdof() const { return m_ndof; }
  TVectorD getUpar() const { return getPars().first; }
  TVectorD getUparErrors() const { return getPars().second; }
  std::vector<std::string> getUparNames() const { return m_parnames; }
  TMatrixD getCovarianceMatrix() const;
  TMatrixD getCorrelationMatrix() const;
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

  static void setMinuitSolverFunction( const MinuitSolverFunction& );
  
private:

  std::pair<TVectorD,TVectorD> getPars() const;
  stat_t getStat() const;
  void printPars( TString option= ".4f" ) const;
  void checkMaxpars( const TVectorD& pars, Int_t maxpars );
  void setupParameters();

  std::vector<std::string> m_parnames;
  TVectorD m_pars;
  TVectorD m_parerrors;
  int m_ndof;
  // Must be pointer due to non-constness of TMinuit
  TMinuit* m_minuit;

  static void myfcn( Int_t& npar, Double_t* grad, Double_t& fval, 
  		     Double_t* par, Int_t iflag );
  static const MinuitSolverFunction* m_msf;


};

const MinuitSolverFunction* MinuitSolver::m_msf= 0;


#endif
