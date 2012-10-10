
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

// Namespaces:
using std::string;
using std::vector;

  
void fcn(Int_t & n, Double_t *grad, Double_t &fval, Double_t *par, Int_t ipar){
   
    Double_t mtop[3]={171.5, 173.1, 174.5};
    Double_t stat[3]={0.3, 0.33, 0.4};
    Double_t erra[3]={1.1, 1.3, 1.5};
    Double_t errb[3]={0.9, 1.5, 1.9};
    Double_t errc[3]={2.4, 3.1, 3.5};
    Double_t ave= par[0];
    Double_t pa = par[1];
    Double_t pb = par[2];
    Double_t pc = par[3];
    Double_t term0= ( mtop[0] - ave + erra[0]*pa + errb[0]*pb + errc[0]*pc)/stat[0];
    Double_t term1= ( mtop[1] - ave + erra[1]*pa + errb[1]*pb + errc[1]*pc )/stat[1];
    Double_t term2= ( mtop[2] - ave + erra[2]*pa + errb[2]*pb + errc[2]*pc )/stat[2];
    fval= term0*term0 + term1*term1 + term2*term2 + pa*pa + pb*pb + pc*pc;

  }


// Test fixture for all tests:
class MinuitSolverTestFixture {
public:
  minuitSolver *minsol;

  MinuitSolverTestFixture(){
    Double_t tmppar[4]={172.,0.,0.,0.};
    TVectorD pars(4,tmppar);
    Double_t tmpparerr[4] = {2.,1.,1.,1.};
    TVectorD parerrors(4,tmpparerr);
    std::vector<TString> parnames;
    parnames.push_back(TString("average"));
    parnames.push_back(TString("pa"));
    parnames.push_back(TString("pb"));
    parnames.push_back(TString("pc"));
    int ndof= 2;
    minsol = new minuitSolver(fcn,pars,parnames,parerrors,ndof);

    BOOST_MESSAGE( "Create MinuitSolverTestFixture" );
  }
  virtual ~MinuitSolverTestFixture() {
    BOOST_MESSAGE( "Tear down MinuitSolverTestFixture" );
  }
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( minuitsolversuite, MinuitSolverTestFixture )


// Test cases:

BOOST_AUTO_TEST_CASE( test_solve ) {
  minsol->solve(true);
  int hstat = minsol->getStatus();
  BOOST_CHECK_MESSAGE( hstat== 3, "Something wrong with solve() status= " <<hstat);
}
/*

BOOST_AUTO_TEST_CASE( test_getChisq ) {
  minsol->solve(true);
  double chisq = minsol->getChisq();
  double expchisq = 3.58037721;
  BOOST_CHECK_MESSAGE( abs(chisq-expchisq)/expchisq< 0.01, "Something wrong with getChisq() ");
}
*/
BOOST_AUTO_TEST_CASE( test_getNdof ) {
  int tmp_ndof = minsol->getNdof();
  int exp_ndof = 2;
  BOOST_CHECK_MESSAGE( tmp_ndof== exp_ndof, "Something wrong with getNdof() ");
}

BOOST_AUTO_TEST_CASE( test_getUpar ) {
  minsol->solve(true);
  TVectorD pars = minsol->getUpar();
  Double_t expectedpars[4]= { 167.1022776, -0.48923998, -1.13417736, -1.21202615 };
  TVectorD exp_pars(4,expectedpars);
  /* 
      for par, expectedpar in zip( pars, expectedpars ):
            self.assertAlmostEqual( par, expectedpar, places=6 )
  */     
  BOOST_CHECK_MESSAGE( false, "Something wrong with solve() ");
}
// Test returning (dummy) average:
/*BOOST_AUTO_TEST_CASE( testgetAverage ) {
  Double_t avg= minsol.getAverage();
  Double_t expected= 99.0;
  BOOST_CHECK_CLOSE( avg, expected, 0.0001 );
  }*/



BOOST_AUTO_TEST_SUITE_END()

