#include "MinuitSolver.hh"
#include <vector>

minuitSolver::minuitSolver(fcn_t fcn, TVectorD pars, std::vector<TString> parnames, TVectorD parerrors, int ndf, int maxpars)
{
  if(pars.GetNoElements() > maxpars)
  {
    std::cerr << "More than " << maxpars << " parameters, increase maxpars" << std::endl;
  }
  _minuit = new TMinuit(maxpars);
  _pars = pars;
  _ndf = ndf;
  _parerrors = parerrors;
  _parnames = parnames;
  _minuit->SetFCN(fcn);
  std::vector<TString>::iterator it_names = _parnames.begin();
  Int_t iPar = 0;
  Int_t error;
  while(it_names != _parnames.end()){
    error = _minuit->DefineParameter(iPar, (*it_names).Data(), _pars(iPar), _parerrors(iPar),0.0, 0.0);
    if (error != 0){
      std::cerr << "Minuit define parameter error:" << error << std::endl;
    }
    it_names++;
    iPar++;
  }
}

minuitSolver::~minuitSolver()
{
  delete _minuit;
}

  //getter
  stat_t minuitSolver::getStat(){
    stat_t hstat;
    
    _minuit->mnstat(hstat.min, hstat.edm, hstat.errdef, 
		    hstat.npari, hstat.nparx, hstat.status);
    return hstat;
  }

  std::pair<TVectorD, TVectorD> minuitSolver::getPars(){
    int nPars = _pars.GetNoElements();
    TVectorD pars(nPars);
    TVectorD parerrors(nPars);
    for (int iPar = 0; iPar < nPars; ++iPar){
      int ier = _minuit->GetParameter(iPar, pars(iPar), parerrors(iPar));
      if (ier < 0) {
	std::cerr << "Parameter " << iPar << " not defined!" << std::endl;
      }
    }
    return std::pair<TVectorD, TVectorD>(pars, parerrors);
  }
  
  TMatrixD minuitSolver::getCovarianceMatrix(){
    int nPars = _pars.GetNoElements();
    TMatrixD covmat(nPars, nPars);
    _minuit->mnemat(covmat.GetMatrixArray(), nPars);
    return covmat;
  }

//   TMatrixD minuitSolver::getCorrelationMatrix(){
//   }
// 
// //   print
//   void minuitSolver::printResult(TString option, bool cov, bool cor){
//   }
// 
//   void minuitSolver::printCovariances(){
//   }
// 
//   void minuitSolver::printCorrelations(){
//   }


void minuitSolver::solve(bool Blobel)
{
  TString command("MIGRAD");
  Int_t error = _minuit->Command(command.Data());
  if(error != 0)
  {
    std::cerr << "Minuit command " << command.Data() << " failed: " << error << std::endl;
  }
}
 
 