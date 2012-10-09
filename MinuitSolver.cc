#include "MinuitSolver.hh"
#include <vector>

//minuitSolver(void (*)(Int_t&, Double_t*, Double_t&f, Double_t*, Int_t) fcn, TVectorD pars, TVectorD parerrors, int ndf, int maxpars){
minuitSolver::minuitSolver(void *fcn, TVectorD pars, std::vector<TString> parnames, TVectorD parerrors, int ndf, int maxpars):   _minuit(maxpars)
{

  _pars = pars;
  _ndf = ndf;
  _parerrors = parerrors;
  _parnames = parnames;
  _minuit.SetFCN(fcn);
  std::vector<TString>::iterator it_names = _parnames.begin();
  Int_t nPar = 0;
  Int_t error;
  while(it_names != _parnames.end()){
    error = _minuit.DefineParameter(nPar, (*it_names).Data(), _pars(nPar), _parerrors(nPar),0.0, 0.0);
    if (error != 0){
      std::cerr << "Minuit define parameter error:" << error << std::endl;
    }
    nPar++;
  }
}
