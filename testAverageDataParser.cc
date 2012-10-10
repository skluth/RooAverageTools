
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "AverageDataParser.hh"

#include "stdio.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include "TObject.h"
#include "TMatrixD.h"

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE averagedataparsertests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;

// Test fixture for all tests:
class AverageDataParserTestFixture {
public:
  AverageDataParserTestFixture() : parser( "test.txt" ) {
    BOOST_MESSAGE( "Create AverageDataParserTestFixture" );
  }
  virtual ~AverageDataParserTestFixture() {
    BOOST_MESSAGE( "Tear down AverageDataParserTestFixture" );
  }
  AverageDataParser parser;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( averagedataparsersuite, AverageDataParserTestFixture )

// Test cases:

// Test returning filename:
BOOST_AUTO_TEST_CASE( testgetFilename ) {
  string filename= parser.getFilename();
  string expected= "test.txt";
  BOOST_CHECK_EQUAL( filename, expected );
}

BOOST_AUTO_TEST_CASE( testgetValues ) {
  std::vector<float> values = parser.getValues();
  std::vector<float> expectedvalues;
  expectedvalues.push_back(171.5);
  expectedvalues.push_back(173.1);
  expectedvalues.push_back(174.5);
  for(unsigned int i=0; i<expectedvalues.size(); i++) {
    BOOST_CHECK_EQUAL( values.at(i), expectedvalues.at(i) );
  }
}

BOOST_AUTO_TEST_CASE( testgetNames ) {
  std::vector<string> names = parser.getNames();
  std::vector<string> expectednames;
  expectednames.push_back("Val1");
  expectednames.push_back("Val2");
  expectednames.push_back("Val3");
  BOOST_CHECK_EQUAL_COLLECTIONS( names.begin(), names.end(),
                                 expectednames.begin(), expectednames.end() );
}

BOOST_AUTO_TEST_CASE( testgetErrors ) {
  std::map<string, std::vector<float> > errors = parser.getErrors();
  std::map<string, std::vector<float> > expectederrors;
  std::vector<float> tmp;
  tmp.push_back(0.3);
  tmp.push_back(0.33);
  tmp.push_back(0.4);
  expectederrors["00stat"] = tmp;
  tmp.clear();
  tmp.push_back(1.1);
  tmp.push_back(1.3);
  tmp.push_back(1.5);
  expectederrors["01err1"] = tmp;
  tmp.clear();
  tmp.push_back(0.9);
  tmp.push_back(1.5);
  tmp.push_back(1.9);
  expectederrors["02err2"] = tmp;
  tmp.clear();
  tmp.push_back(2.4);
  tmp.push_back(3.1);
  tmp.push_back(3.5);
  expectederrors["03err3"] = tmp;
  tmp.clear();
  tmp.push_back(1.4);
  tmp.push_back(2.9);
  tmp.push_back(3.3);
  expectederrors["04err4"] = tmp;
  tmp.clear();
  std::map<string, std::vector<float> >::const_iterator expecteditr = expectederrors.begin();
  for(std::map<string, std::vector<float> >::const_iterator itr = errors.begin(); itr != errors.end(); ++itr) {
    BOOST_CHECK_EQUAL(itr->first, expecteditr->first);
    BOOST_CHECK_EQUAL_COLLECTIONS(itr->second.begin(), itr->second.end(),
        expecteditr->second.begin(), expecteditr->second.end() );
    ++expecteditr;
  }
  BOOST_CHECK_EQUAL(errors.size(), expectederrors.size() );
}

BOOST_AUTO_TEST_CASE( testgetTotalErrors ) {
  vector<float> totalerrors = parser.getTotalErrors();
  vector<float> expectedtotalerrors;
  expectedtotalerrors.push_back(3.1352830813181765);
  expectedtotalerrors.push_back(4.6977547828723454);
  expectedtotalerrors.push_back(5.3999999999999995);
  BOOST_CHECK_EQUAL_COLLECTIONS(totalerrors.begin(), totalerrors.end(),
      expectedtotalerrors.begin(),expectedtotalerrors.end() );
}

BOOST_AUTO_TEST_CASE( testgetCovoption ) {
  std::map<string, string> covopts = parser.getCovoption();
  std::map<string, string> expectedcovopts;
  expectedcovopts["00stat"] = string("c");
  expectedcovopts["01err1"] = string("m");
  expectedcovopts["02err2"] = string("m");
  expectedcovopts["03err3"] = string("p");
  expectedcovopts["04err4"] = string("f");
  std::map<string, string >::const_iterator expecteditr = expectedcovopts.begin();
  for(std::map<string, string >::const_iterator itr = covopts.begin(); itr != covopts.end(); ++itr) {
    BOOST_CHECK_EQUAL(itr->first, expecteditr->first);
    BOOST_CHECK_EQUAL(itr->second, expecteditr->second);
    ++expecteditr;
  }
  BOOST_CHECK_EQUAL(covopts.size(), expectedcovopts.size() );
}


BOOST_AUTO_TEST_CASE( testgetCorrelations ) {
  std::map<string, string > correlations = parser.getCorrelations();
  std::map<string, string > expectedcorrelations;
  string tmp = "1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0";
  expectedcorrelations["00stat"] = tmp;
  tmp = "p, p, p, p, p, p, p, p, p";
  expectedcorrelations["01err1"] = tmp;
  tmp = "f, f, f, f, f, f, f, f, f";
  expectedcorrelations["02err2"] = tmp;
  std::map<string, string >::const_iterator expecteditr = expectedcorrelations.begin();
  for(std::map<string, string >::const_iterator itr = correlations.begin(); itr != correlations.end(); ++itr) {
    BOOST_CHECK_EQUAL(itr->first, expecteditr->first);
    BOOST_CHECK_EQUAL(itr->second, expecteditr->second);
    ++expecteditr;  
  }
  BOOST_CHECK_EQUAL(correlations.size(), expectedcorrelations.size() );
}

BOOST_AUTO_TEST_CASE( testgetCovariances ) {
  std::map<string, TMatrixD> covariances;
  std::map<string, TMatrixD> expectedcovariances;
  double data[] = {0.09, 0.0, 0.0, 0.0, 0.1089, 0.0, 0.0, 0.0, 0.16};
  TMatrixD tmp(3,3,data);
    /* Python
        covariances= self.__parser.getCovariances()
        expectedCovariances= { '00stat': matrix( [ [ 0.09, 0.0,  0.0 ],
                                                   [ 0.0, 0.1089, 0.0 ],
                                                   [ 0.0, 0.0, 0.16 ] ] ), 
                               '01err1': matrix( [ [ 1.21, 1.21, 1.21 ],
                                                   [ 1.21, 1.69, 1.69 ],
                                                   [ 1.21, 1.69, 2.25 ] ] ), 
                               '03err3': matrix( [ [ 5.76, 5.76, 5.76 ],
                                                   [ 5.76, 9.61, 9.61 ],
                                                   [ 5.76, 9.61, 12.25 ] ] ),
                               '02err2': matrix( [ [ 0.81, 1.35, 1.71 ],
                                                   [ 1.35, 2.25, 2.85 ],
                                                   [ 1.71, 2.85, 3.61 ] ] ), 
                               '04err4': matrix( [ [ 1.96, 4.06, 4.62 ],
                                                   [ 4.06, 8.41, 9.57 ],
                                                   [ 4.62, 9.57, 10.89 ] ] ) }
        self.assertEqual( sorted( covariances.keys() ), 
                          sorted( expectedCovariances.keys() ) )
                          */
 }


BOOST_AUTO_TEST_CASE( testCovariances ) {
  std::map<string,TMatrixD*> covariances, expectedCovariances;
  //covariances = parser.getCovariances()
  double matrixData1 [] = {  0.117649, 0.0, 0.0,  0.0, 0.14502387, 0.0, 0.0,  0.0,  0.27405225 };
  TMatrixD myMatrix1(3,3,matrixData1);
  expectedCovariances["00stat"] = &myMatrix1;
  double matrixData2 [] = { 3.55888225, 3.55888225, 3.55888225, 3.55888225,  5.06385009,  3.55888225, 3.55888225,  3.55888225,  6.85130625};
  TMatrixD myMatrix2(3,3,matrixData2);
  expectedCovariances["01erra"] = &myMatrix2;
  double matrixData3 [] = {  0.81,  0.81,  0.81,  0.81,  2.25,  0.81, 0.81,  0.81,  3.61};
  TMatrixD myMatrix3(3,3,matrixData3);
  expectedCovariances["02errb"] = &myMatrix3;
  double matrixData4 [] = { 5.76, 5.81373761, 5.86075802,  5.81373761, 9.61, 5.91543564, 5.86075802, 5.91543564, 12.25 };
  TMatrixD myMatrix4(3,3,matrixData4);
  expectedCovariances["03errc"] = &myMatrix4;
  
  std::map<string, TMatrixD* >::const_iterator expecteditr = expectedCovariances.begin();
  for(std::map<string, TMatrixD* >::const_iterator itr = covariances.begin(); itr != covariances.end(); ++itr) {
    BOOST_CHECK_EQUAL(itr->first, expecteditr->first);
    //BOOST_CHECK_EQUAL(*(itr->second), *(expecteditr->second));
    ++expecteditr;
  }  
  BOOST_CHECK_EQUAL(covariances.size(), expectedCovariances.size() );
}

BOOST_AUTO_TEST_CASE( testSysterrormatrix ) {
  std::map<unsigned int, std::vector<float> > systerrmatrix;
  std::map<unsigned int, std::vector<float> > expectedsysterrmatrix;
  systerrmatrix = parser.getSysterrorMatrix();
  std::vector<float> tmp;
  tmp.push_back(1.8865);
  tmp.push_back(1.8865);
  tmp.push_back(1.8865);
  expectedsysterrmatrix[1] = tmp;
  tmp.clear();
  tmp.push_back(0.9);
  tmp.push_back(0.9);
  tmp.push_back(0.9);
  expectedsysterrmatrix[2] = tmp;
  tmp.clear();
  tmp.push_back(2.4);
  tmp.push_back(2.42239067);
  tmp.push_back(2.4419825);
  expectedsysterrmatrix[3] = tmp;

  std::map<unsigned int, std::vector<float> >::const_iterator expecteditr = expectedsysterrmatrix.begin();
  for(std::map<unsigned int, std::vector<float> >::const_iterator itr = systerrmatrix.begin(); itr != systerrmatrix.end(); ++itr) {
    BOOST_CHECK_EQUAL(itr->first, expecteditr->first);
    BOOST_CHECK_EQUAL_COLLECTIONS(itr->second.begin(), itr->second.end(), expecteditr->second.begin(), expecteditr->second.end());
    ++expecteditr;
  }
  BOOST_CHECK_EQUAL(systerrmatrix.size(), expectedsysterrmatrix.size() );
  
}

BOOST_AUTO_TEST_SUITE_END()

