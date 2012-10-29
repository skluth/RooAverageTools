// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "Blue.hh"

//#include "stdio.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>

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
using std::map;

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

BOOST_AUTO_TEST_CASE( testerrorAnalysis ) {
  BOOST_MESSAGE( "testerrorAnalysis" );
  MatrixMap obtained= blue.errorAnalysis();
  map<string,Double_t> expected;
  expected["00stat"]= 0.4114006127938109;
  expected["01err1"]= 1.132605331048375;
  expected["02err2"]= 0.62562331148619099;
  expected["03err3"]= 2.5071108260136228;
  expected["04err4"]= 0.82049571716089753;
  expected["syst"]= 2.9381996663923697;
  expected["total"]= 2.9668615983552984;
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  MatrixMap::const_iterator obtitr;
  map<string,Double_t>::const_iterator expitr;
  for( obtitr= obtained.begin(), expitr= expected.begin();
       expitr != expected.end(); obtitr++, expitr++ ) {
    BOOST_CHECK_EQUAL( obtitr->first, expitr->first );
    Double_t obtainederror= sqrt( (obtitr->second)(0,0) );
    Double_t expectederror= expitr->second;
    BOOST_CHECK_CLOSE( obtainederror, expectederror, 1.0e-4 );
  }
}

BOOST_AUTO_TEST_SUITE_END()


class BluePrintTestFixture {
public:
  BluePrintTestFixture() : blue( "valassi5.txt" ) {}
  Blue blue;
  std::ostringstream osst;
};

BOOST_FIXTURE_TEST_SUITE( blueprintsuite, BluePrintTestFixture )

BOOST_AUTO_TEST_CASE( testprintChisq ) {
  BOOST_MESSAGE( "testprintChisq" );
  blue.printChisq( osst );
  string obtained= osst.str();
  string expected= "Chi^2= 1.23 for 2 d.o.f, chi^2/d.o.f= 0.62, P(chi^2)= 0.5404\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintWeights ) {
  BOOST_MESSAGE( "testprintWeights" );
  blue.printWeights( osst );
  string obtained= osst.str();
  string expected= 
    " Weights a:     0.8195     0.1805     0.0897    -0.0897\n"
    " Weights b:     0.8076    -0.8076     0.0981     0.9019\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintPulls ) {
  BOOST_MESSAGE( "testprintPulls" );
  blue.printPulls( osst );
  string obtained= osst.str();
  string expected= "     Pulls:    -0.1377     0.9549    -0.5453     0.9556\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintAverages ) {
  BOOST_MESSAGE( "testprintAverages" );
  blue.printAverages( osst );
  string obtained= osst.str();
  string expected= "   Average:    10.6377    11.1358\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintErrors ) {
  BOOST_MESSAGE( "testprintErrors" );
  blue.printErrors( osst );
  string obtained= osst.str();
  string expected= 
    "      stat:     0.8636     0.8963\n"
    "       exp:     0.2714     0.2820\n"
    "      syst:     0.2714     0.2820\n"
    "     total:     0.9053     0.9397\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintCorrelations ) {
  BOOST_MESSAGE( "testprintCorrelations" );
  blue.printCorrelations( osst );
  string obtained= osst.str();
  string expected= "Total correlations:\n"
    "       a     b\n"
    " a  1.00  0.95\n"
    " b  0.95  1.00\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}


BOOST_AUTO_TEST_SUITE_END()

