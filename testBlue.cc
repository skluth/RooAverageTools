
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "blue.hh"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE bluetests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;

// Test fixture for all tests:
class blueTestFixture {
public:
  blueTestFixture() : parser( "test.txt" ) {
    BOOST_MESSAGE( "Create blueTestFixture" );
  }
  virtual ~blueTestFixture() {
    BOOST_MESSAGE( "Tear down blueTestFixture" );
  }
  blue parser;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( bluesuite, blueTestFixture )

// Test cases:

// Test returning filename:
BOOST_AUTO_TEST_CASE( testgetFilename ) {
  string filename= parser.getFilename();
  string expected= "test.txt";
  BOOST_CHECK_EQUAL( filename, expected );
}



BOOST_AUTO_TEST_SUITE_END()

