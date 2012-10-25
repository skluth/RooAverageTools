// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "Blue.hh"

//#include "stdio.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

//// ROOT includes

#include "TMatrixD.h"
#include "TMatrixDSym.h"

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE bluetests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;

// Test fixture for all tests:
class BlueTestFixture {
public:
  BlueTestFixture() : blue( "test.txt" ) {}
  Blue blue;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( bluesuite, BlueTestFixture )

// Test cases:

BOOST_AUTO_TEST_CASE( testcalcWeightsMatrix ) {
  BOOST_MESSAGE( "testcalcWeightsMatrix" );
  double expected[]= { 1.33903066, -0.16163493, -0.17739573 };
  TMatrixD result= blue.calcWeightsMatrix();
  for( int i = 0; i < 3; i++) {
    BOOST_CHECK_CLOSE( expected[i], result(0,i), 1.0e-4 );
  }  
}

BOOST_AUTO_TEST_CASE( testcalcAverage ) {
  BOOST_MESSAGE( "testcalcAverage" );
  Double_t expected= 170.709197;
  TVectorD result= blue.calcAverage();
  BOOST_CHECK_CLOSE( expected, result[0], 1.0e-4 );
}

BOOST_AUTO_TEST_CASE( testcalcChisq ) {
  BOOST_MESSAGE( "testcalcChisq" );
  double obtained= blue.calcChisq();
  double expected= 0.770025;
  BOOST_CHECK_CLOSE( obtained, expected, 1.0e-4 );
}

BOOST_AUTO_TEST_CASE( testcalcPulls ) {
  BOOST_MESSAGE( "testcalcPulls" );
  TVectorD obtained= blue.calcPulls();
  Double_t expected[]= { 0.25222701, 0.5089246205, 0.70200057 };
  for( int i= 0; i < 3; i++ ) {
    BOOST_CHECK_CLOSE( obtained[i], expected[i], 1.0e-4 );
  }
}

BOOST_AUTO_TEST_SUITE_END()

