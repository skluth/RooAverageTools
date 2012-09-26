
// Unit tests for ClsqAverage
// S. Kluth, 9/2012

#include "ClsqAverage.hh"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE clsqaveragetests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;

// Test fixture for all tests:
class ClsqAverageTestFixture {
public:
  ClsqAverageTestFixture() : clsqavg( "test.txt" ) {
    BOOST_MESSAGE( "Create ClsqAverageTestFixture" );
  }
  virtual ~ClsqAverageTestFixture() {
    BOOST_MESSAGE( "Tear down ClsqAverageTestFixture" );
  }
  ClsqAverage clsqavg;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( clsqaveragesuite, ClsqAverageTestFixture )

// Test cases:

// Test returning (dummy) average:
BOOST_AUTO_TEST_CASE( testgetAverage ) {
  Double_t avg= clsqavg.getAverage();
  Double_t expected= 99.0;
  BOOST_CHECK_CLOSE( avg, expected, 0.0001 );
}



BOOST_AUTO_TEST_SUITE_END()

