
#include "MinuitSolver.hh"
#include <vector>
#include <cmath>
#include <sstream>
#include "TMath.h"

using std::string;
using std::stringstream;
using std::vector;

// Subclass of TMinuit for use with MinuitSolverFunction function objects:
class myTMinuit: public TMinuit {
public:
  myTMinuit( MinuitSolverFunction& msf, Int_t maxpar ) :
    TMinuit( maxpar ), m_msf( msf ) {}
  virtual Int_t Eval( Int_t npar, Double_t *grad, Double_t &fval, 
		      Double_t *par, Int_t iflag ) {
    m_msf( npar, grad, fval, par, iflag );
    return 0;
  }
private:
  MinuitSolverFunction& m_msf;
};

// Errors from Minuit:
class MinuitError: public std::exception {
public:
  MinuitError( int ierr, const char* txt ) : 
    errorcode( ierr ), message( txt ) {}
  virtual const char* what() const throw() {
    stringstream strstr;
    strstr << "Minuit error: " << errorcode << " "
	   << message;
    string txt= strstr.str();
    return txt.c_str();
  }
private:
  int errorcode;
  const char* message;
};

// Configuration for use with standard Minuit fcn:
MinuitSolver::MinuitSolver( fcn_t fcn, 
			    const vector<string>& parnames, 
			    const TVectorD& pars, 
			    const TVectorD& parerrors, 
			    int ndof, bool quiet, int maxpars ) :
  m_parnames( parnames ),
  m_pars( pars ),
  m_parerrors( parerrors ),
  m_ndof( ndof ),
  m_minuit( new TMinuit( maxpars ) ) {
  m_minuit->SetFCN( fcn );
  initialise( maxpars, quiet );
  return;
}

// Configuration for use with MinuitSolverFunction:
MinuitSolver::MinuitSolver( MinuitSolverFunction& msf,
			    const vector<string>& parnames, 
			    const TVectorD& pars, 
			    const TVectorD& parerrors, 
			    int ndof, bool quiet, int maxpars ) :
  m_parnames( parnames ),
  m_pars( pars ),
  m_parerrors( parerrors ),
  m_ndof( ndof ),
  m_minuit( new myTMinuit( msf, maxpars ) ) {
  initialise( maxpars, quiet );
  return;
}

// Ctor helpers:
void MinuitSolver::initialise( Int_t maxpars, bool quiet ) {
  checkMaxpars( maxpars );
  if( quiet ) minuitCommand( "SET PRI -1" );
  setupParameters();
  return;
}

void MinuitSolver::checkMaxpars( Int_t maxpars ) {
  if( m_pars.GetNoElements() > maxpars ) {
    stringstream strstr;
    strstr << "More than " << maxpars << " parameters, increase maxpars";
    string str= strstr.str();
    throw MinuitError( 0, str.c_str() );
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
      //      std::cerr << "Minuit define parameter error: " << error << std::endl;
      throw MinuitError( error, "in DefineParameter" );
    }
  }
  return;
}

// Dtor:
MinuitSolver::~MinuitSolver() {
  delete m_minuit;
}

// Getters:
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
      //      std::cerr << "Parameter " << iPar << " not defined!" << std::endl;
      throw MinuitError( ierr, "in GetParameter" );
    }
  }
  return std::pair<TVectorD,TVectorD>( pars, parerrors );
}

TMatrixDSym MinuitSolver::getCovarianceMatrix() const {
  int nPars= m_pars.GetNoElements();
  TMatrixDSym covmat( nPars );
  m_minuit->mnemat( covmat.GetMatrixArray(), nPars );
  return covmat;
}

TMatrixDSym MinuitSolver::getCorrelationMatrix() const {
  int nPars= m_pars.GetNoElements();
  TMatrixDSym covmat( getCovarianceMatrix() );
  TMatrixDSym corrmat( nPars );
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
  return;
}

void MinuitSolver::minuitCommand( string command ) const {
  Int_t error= m_minuit->Command( command.c_str() );
  if( error != 0 ) {
    // std::cerr << "Minuit command " << command << " failed: " 
    // 	      << error << std::endl;
    string txt= "in "+command;
    throw MinuitError( error, txt.c_str() ); 
  }
  return;
}

