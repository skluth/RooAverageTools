
#include "MinuitSolver.hh"
#include <vector>
#include <cmath>
#include "TMath.h"

using std::string;
using std::vector;

// Ctor for use with standard Minuit fcn:
MinuitSolver::MinuitSolver( fcn_t fcn, 
			    const vector<string>& parnames, 
			    const TVectorD& pars, 
			    const TVectorD& parerrors, 
			    int ndof, bool quiet, int maxpars ):
  m_parnames( parnames),
  m_pars( pars ),
  m_parerrors( parerrors ),
  m_ndof( ndof ),
  m_minuit( new TMinuit( maxpars ) ) {
  m_minuit->SetFCN( fcn );
  checkMaxpars( pars, maxpars );
  if( quiet ) minuitCommand( "SET PRI -1" );
  setupParameters();
}

// Ctor for use with MinuitSolverFunction, the address of the
// MinuitSolverFunction object needs to be set explicitly since
// myfcn must be static for Minuit:
MinuitSolver::MinuitSolver( const vector<string>& parnames, 
			    const TVectorD& pars, 
			    const TVectorD& parerrors, 
			    int ndof, bool quiet, int maxpars ):
  m_parnames( parnames ),
  m_pars( pars ),
  m_parerrors( parerrors ),
  m_ndof( ndof ),
  m_minuit( new TMinuit( maxpars ) ) {
  m_minuit->SetFCN( myfcn );
  checkMaxpars( pars, maxpars );
  if( quiet ) minuitCommand( "SET PRI -1" );
  setupParameters();
}
void MinuitSolver::myfcn( Int_t& npar, Double_t* grad, Double_t& fval, 
			  Double_t* par, Int_t iflag ) {
  TVectorD pars( npar, par );
  fval= m_msf->calculate( pars );
  return;
}
void MinuitSolver::setMinuitSolverFunction( const MinuitSolverFunction& msf ) {
  m_msf= &msf;
  return;
}


void MinuitSolver::checkMaxpars( const TVectorD& pars, Int_t maxpars ) {
  if( pars.GetNoElements() > maxpars ) {
    std::cerr << "More than " << maxpars << " parameters, increase maxpars" 
	      << std::endl;
  }
  return;
}

void MinuitSolver::setupParameters() {
  Int_t nPars= m_pars.GetNoElements();
  for( int iPar= 0; iPar < nPars; ++iPar ) {
    int error= m_minuit->DefineParameter( iPar, m_parnames[iPar].c_str(), 
					  m_pars(iPar), m_parerrors(iPar), 
					  0.0, 0.0);
    if( error != 0 ) {
      std::cerr << "Minuit define parameter error: " << error << std::endl;
    }
  }
  return;
}

MinuitSolver::~MinuitSolver() {
  delete m_minuit;
}


//getter
stat_t MinuitSolver::getStat() const {
  stat_t hstat;
  m_minuit->mnstat( hstat.min, hstat.edm, hstat.errdef, 
		    hstat.npari, hstat.nparx, hstat.status );
  return hstat;
}

std::pair<TVectorD,TVectorD> MinuitSolver::getPars() const {
  int nPars= m_pars.GetNoElements();
  TVectorD pars( nPars );
  TVectorD parerrors( nPars );
  for( int iPar= 0; iPar < nPars; ++iPar ) {
    int ierr= m_minuit->GetParameter( iPar, pars(iPar), parerrors(iPar) );
    if( ierr < 0 ) {
      std::cerr << "Parameter " << iPar << " not defined!" << std::endl;
    }
  }
  return std::pair<TVectorD,TVectorD>( pars, parerrors );
}

TMatrixD MinuitSolver::getCovarianceMatrix() const {
  int nPars= m_pars.GetNoElements();
  TMatrixD covmat( nPars, nPars );
  m_minuit->mnemat( covmat.GetMatrixArray(), nPars );
  return covmat;
}

TMatrixD MinuitSolver::getCorrelationMatrix() const {
  int nPars= m_pars.GetNoElements();
  TMatrixD corrmat( nPars, nPars );
  TMatrixD covmat( getCovarianceMatrix() );
  for( int i= 0; i < nPars; ++i ) {
    for( int j= 0; j < nPars; ++j ) {
      corrmat(i,j)= covmat(i,j)/sqrt(covmat(i,i)*covmat(j,j));
    }
  }
  return corrmat;
}

//   print
void MinuitSolver::printResult( bool cov, bool cor, TString option ) const {
  double chi2= getChisq();
  std::cout << "Minuit least squares\nResult after minuit fit:"  
	    << " Chi^2= " << chi2 
	    << " for " << m_ndof << " d.o.f., "
	    << " Chi^2/d.o.f= " << chi2/Double_t( m_ndof )
	    << " Estimated dist. to min: " <<  TMath::Prob( chi2, m_ndof )
	    << std::endl;
  std::cout << "\nFitted parameters and errors:\nName\tValue\tError" 
	    << std::endl;
  printPars();
  if( cov ) printCovariances();
  if( cor ) printCovariances();
}

void MinuitSolver::printPars( TString option ) const {
  vector<string>::const_iterator it_names= m_parnames.begin();
  int iparameter = 0;
  while( it_names != m_parnames.end() ) {
    std::cout << *it_names << "\t" << 
      getPars().first( iparameter ) << "\t" << 
      getPars().second( iparameter ) << std::endl;
    it_names++;
    iparameter++;
  }
}

void MinuitSolver::printCovariances() const {
  TMatrixD matrix= getCovarianceMatrix();
  matrix.Print();
}

void MinuitSolver::printCorrelations() const {
  TMatrixD matrix= getCorrelationMatrix();
  matrix.Print();
}

void MinuitSolver::solve() const {
  minuitCommand( "MIGRAD" );
}

void MinuitSolver::minuitCommand( string command ) const {
  Int_t error= m_minuit->Command( command.c_str() );
  if( error != 0 ) {
    std::cerr << "Minuit command " << command << " failed: " 
	      << error << std::endl;
  }
}

