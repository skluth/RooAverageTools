
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "AverageDataParser.hh"

#include "stdio.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

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

