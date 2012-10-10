#include <iostream>

#include "TVectorD.h"
#include "TMinuit.h"
#include "TString.h"

typedef void (* fcn_t)(Int_t&, Double_t*, Double_t&f, Double_t*, Int_t);

struct stat_t {
  double min;
  double edm;
  double errdef;
  int npari;
  int nparx;
  int status;
};	

class minuitSolver {

private:
  TMinuit* _minuit;
  int _ndf;
  TVectorD _pars;
  TVectorD _parerrors;
  std::vector<TString> _parnames;

  void getPars(TVectorD &pars, TVectorD &parerrors);
  stat_t getStat();
  void printPars(TVectorD pars, TVectorD parerrors, TVectorD parnames, TString option = ".4f");

public:
  minuitSolver(fcn_t fcn, TVectorD pars,  std::vector<TString> parnames, TVectorD parerrors, int ndf, int maxpars = 50);
  ~minuitSolver();
  //getter
  TVectorD getUpar();
  TVectorD getUparErrors();
  TVectorD getUparNames();
  TMatrixD getCovarianceMatrix();
  TMatrixD getCorrelationMatrix();
  double getChisq();

  //print
  void printResult(TString option = ".4f", bool cov = false, bool cor = false);
  void printCovariances();
  void printCorrelations();

  //other
  void solve(bool Blobel);
  
};
