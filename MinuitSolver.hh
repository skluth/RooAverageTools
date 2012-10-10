#include <iostream>

#include "TVectorD.h"
#include "TMinuit.h"
#include "TString.h"
class minuitSolver {

private:
  TMinuit* _minuit;
  int _ndf;
  TVectorD _pars;
  TVectorD _parerrors;
  std::vector<TString> _parnames;

  void getPars(TVectorD &pars, TVectorD &parerrors);
  double getStat();
  void printPars(TVectorD pars, TVectorD parerrors, TVectorD parnames, TString option = ".4f");

public:
  //  minuitSolver();
  minuitSolver(void *fcn, TVectorD pars, std::vector<TString> parnames, TVectorD parerrors, int ndf, int maxpars = 50);
  //minuitSolver(void (*)(Int_t&, Double_t*, Double_t&f, Double_t*, Int_t) fcn, TVectorD pars, TVectorD parerrors, int ndf, int maxpars = 50);
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
  void minuitCommand(TString command);
  void solve(bool Blobel);
  
};
