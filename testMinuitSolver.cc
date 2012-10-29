
// Unit tests for MinuitSolver
// S. Kluth, 9/2012

#include "MinuitSolver.hh"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE minuitsolvertests
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

// Namespaces:
using std::string;
using std::vector;

// Minuit style function for minimisation:  
void fcn( Int_t& npar, Double_t* grad, Double_t& fval, Double_t* par, 
	  Int_t iflag ) {
  Double_t mtop[3]= { 171.5, 173.1, 174.5 };
  Double_t stat[3]= { 0.3, 0.33, 0.4 };
  Double_t erra[3]= { 1.1, 1.3, 1.5 };
  Double_t errb[3]= { 0.9, 1.5, 1.9 };
  Double_t errc[3]= { 2.4, 3.1, 3.5 };
  Double_t ave= par[0];
  Double_t pa= par[1];
  Double_t pb= par[2];
  Double_t pc= par[3];
  Double_t term0= ( mtop[0] - ave + erra[0]*pa + errb[0]*pb + errc[0]*pc)/stat[0];
  Double_t term1= ( mtop[1] - ave + erra[1]*pa + errb[1]*pb + errc[1]*pc )/stat[1];
  Double_t term2= ( mtop[2] - ave + erra[2]*pa + errb[2]*pb + errc[2]*pc )/stat[2];
  fval= term0*term0 + term1*term1 + term2*term2 + pa*pa + pb*pb + pc*pc;
  return;
}

class testmsf: public MinuitSolverFunction {
public:
  testmsf( const TVectorD& mtop, const TVectorD& stat,
	   const TVectorD& erra, const TVectorD& errb, const TVectorD& errc ) : 
    m_mtop( mtop ), m_stat( stat ), m_erra( erra ), m_errb( errb ), 
    m_errc( errc ) {}
  Double_t calculate( const TVectorD& pars ) const {
    Double_t ave= pars[0];
    Double_t pa= pars[1];
    Double_t pb= pars[2];
    Double_t pc= pars[3];
    Double_t fval= 0.0;
    for( Int_t ival= 0; ival < m_mtop.GetNoElements(); ival++ ) {
      Double_t term= ( m_mtop[ival] - ave + m_erra[ival]*pa + 
		       m_errb[ival]*pb + m_errc[ival]*pc )/m_stat[ival];
      fval+= term*term;
    }
    return fval + pa*pa + pb*pb + pc*pc;
  }
private:
  TVectorD m_mtop, m_stat, m_erra, m_errb, m_errc;
};

// Test fixture for all tests:
class MinuitSolverTestFixture {
public:
  MinuitSolverTestFixture() {
    Double_t tmppar[4]= { 172., 0., 0., 0. };
    TVectorD pars( 4, tmppar );
    Double_t tmpparerr[4]= { 2., 1., 1., 1. };
    TVectorD parerrors( 4, tmpparerr );
    vector<string> parnames;
    parnames.push_back( "average" );
    parnames.push_back( "pa" );
    parnames.push_back( "pb" );
    parnames.push_back( "pc" );
    int ndof= 2;
    // minsol2= new MinuitSolver( fcn, parnames, pars, parerrors, ndof );
    // minsol2->solve( true );
    minsol= new MinuitSolver( parnames, pars, parerrors, ndof );
    Double_t mtopdata[3]= { 171.5, 173.1, 174.5 };
    Double_t statdata[3]= { 0.3, 0.33, 0.4 };
    Double_t erradata[3]= { 1.1, 1.3, 1.5 };
    Double_t errbdata[3]= { 0.9, 1.5, 1.9 };
    Double_t errcdata[3]= { 2.4, 3.1, 3.5 };
    TVectorD mtop( 3, mtopdata );
    TVectorD stat( 3, statdata );
    TVectorD erra( 3, erradata );
    TVectorD errb( 3, errbdata );
    TVectorD errc( 3, errcdata );
    testmsf tmsf( mtop, stat, erra, errb, errc );
    MinuitSolver::setMinuitSolverFunction( tmsf );
    minsol->solve();
  }
  virtual ~MinuitSolverTestFixture() {
    delete minsol;
  }
  MinuitSolver* minsol;
  MinuitSolver* minsol2;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( minuitsolversuite, MinuitSolverTestFixture )

// Test cases:

BOOST_AUTO_TEST_CASE( testgetStatus ) {
  BOOST_MESSAGE( "testgetStatus" );
  int hstat= minsol->getStatus();
  BOOST_CHECK_MESSAGE( hstat == 3, 
		       "Something wrong with solve() status= " << hstat );
}

BOOST_AUTO_TEST_CASE( testgetChisq ) {
  BOOST_MESSAGE( "testgetChisq" );
  Double_t chisq= minsol->getChisq();
  Double_t expchisq= 3.58037721;
  BOOST_CHECK_CLOSE( chisq , expchisq , 1e-4 );
}

BOOST_AUTO_TEST_CASE( test_getNdof ) {
  BOOST_MESSAGE( "testgetNdof" );
  int tmp_ndof= minsol->getNdof();
  int exp_ndof= 2;
  BOOST_CHECK_MESSAGE( tmp_ndof== exp_ndof, 
		       "Something wrong with getNdof()" );
}

BOOST_AUTO_TEST_CASE( test_getUpar ) {
  BOOST_MESSAGE( "testgetUpar" );
  TVectorD pars= minsol->getUpar();
  Double_t expectedpars[4]= { 167.1022776, -0.48923998, 
			      -1.13417736, -1.21202615 };
  TVectorD exppars( 4, expectedpars );
  for( int i= 0; i < exppars.GetNoElements(); i++ ) {
    BOOST_CHECK_CLOSE( pars[i], exppars[i], 1e-4 );
  }
}

BOOST_AUTO_TEST_CASE( test_getUparErrors ) {
  BOOST_MESSAGE( "testgetUparErrors" );
  TVectorD parerrors= minsol->getUparErrors();
  Double_t expectedparerrors[4]= { 1.4395944, 0.96551507, 
				   0.78581713, 0.72292831 };
  TVectorD expparerrors( 4, expectedparerrors );
  for(int i= 0; i < expparerrors.GetNoElements(); i++ ) {
    BOOST_CHECK_CLOSE( parerrors[i], expparerrors[i], 1e-4 );
  }
}

BOOST_AUTO_TEST_CASE( test_getCovarianceMatrix ) {
  BOOST_MESSAGE( "testgetCovarianceMatrix" );
  TMatrixD cov_matrix= minsol->getCovarianceMatrix();
  double exp_cov_matrix[4][4]= {
    { 2.0724326179335506, 0.51824661355716117, 
      -0.5889555125183219, 0.74832061921642412},
    { 0.51824661355716117, 0.93221958901081603, 
      -0.15033723965202533, -0.15772527200359948},
    { -0.5889555125183219, -0.15033723965202533, 
      0.61750870113079592, -0.42340001172919944},
    { 0.74832061921642412, -0.15772527200359948, 
      -0.42340001172919944, 0.52262549079771636}
  };
  for( int i= 0; i < 4; i++ ) {
    for( int j= 0; j < 4; j++ ) {
      BOOST_CHECK_CLOSE( cov_matrix[i][j], exp_cov_matrix[i][j], 1e-4 ); 
    }
  }
}

BOOST_AUTO_TEST_CASE( test_getCorrelationMatrix ) {
  BOOST_MESSAGE( "testgetCorrelationMatrix" );
  TMatrixD cor_matrix= minsol->getCorrelationMatrix();
  double exp_cor_matrix[4][4]= {
    { 1.0000, 0.37285257870057764, -0.52061987169579238, 0.71903860509038131 },
    { 0.37285257870057764, 1.0000, -0.19814627795415776, -0.22596796708443162 },
    { -0.52061987169579238, -0.19814627795415776, 1.0000, -0.74530499312239862 },
    { 0.71903860509038131, -0.22596796708443162, -0.74530499312239862, 1.0000 }
  };
  for( int i= 0; i < 4; i++ ) {
    for( int j= 0; j < 4; j++ ) {
      BOOST_CHECK_CLOSE( cor_matrix[i][j], exp_cor_matrix[i][j], 1e-4 );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()

