#include <iostream>

#include "TVectorD.h"
#include "TMinuit.h"
#include "TString.h"
class minuitSolver {

private:
  TMinuit _minuit;
  int ndf;
  TVectorD pars;
  TVectorD parerrors;
  std::vector<TString> parnames;

  void getPars(TVectorD &pars, TVectorD &parerrors);
  double getStat();
  void printPars(TVectorD pars, TVectorD parerrors, TVectorD parnames, TString option = ".4f");

public:
  minuitSolver();
  minuitSolver(void* fnc, TVectorD pars, TVectorD parerrors, int ndf);
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
