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
  blueTestFixture() : blueObject( "filename" ) {
    BOOST_MESSAGE( "Create blueTestFixture" );
  }
  virtual ~blueTestFixture() {
    BOOST_MESSAGE( "Tear down blueTestFixture");
  }
  blue blueObject;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( bluesuite, blueTestFixture )

// Test cases:

BOOST_AUTO_TEST_CASE( testCalcWeightsMatrix ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcWeightsMatrix()");

	double expected[] = { 1.3390306603614366, -0.16163492961906992, -0.17739573074236697 };

	TMatrixD result = blueObject.calcWeightsMatrix();
	for (int i = 0; i < 3; i++) {

		BOOST_CHECK_CLOSE_FRACTION(expected[i], result(0,i), 1e-6);
	}

}

BOOST_AUTO_TEST_CASE( testCalcAverage ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcAverage()");

	double expected = 170.70919692;

	double result = blueObject.calcAverage();

	BOOST_CHECK_CLOSE_FRACTION(expected, result, 1e-9);

}

BOOST_AUTO_TEST_CASE( testCalcChisq ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcChisq()");

	double expected = 0.770025093468;

	double result = blueObject.calcChisq();

	BOOST_CHECK_CLOSE_FRACTION(expected, result, 1e-8);

}

BOOST_AUTO_TEST_CASE( testCalcPulls ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcPulls()");

	double totalErrdata1[] = { 1, 1, 1 };
	blueObject.totalerrors.ResizeTo(3, 1);
	blueObject.totalerrors.SetMatrixArray(totalErrdata1);
	TMatrixD result1 = blueObject.calcPulls();

	double totalErrdata2[] = { 3, 3, 3 };
	blueObject.totalerrors.SetMatrixArray(totalErrdata2);
	TMatrixD result2 = blueObject.calcPulls();

	for (int i = 0; i < 3; i++) {
		BOOST_CHECK_CLOSE_FRACTION(result1(1,0), 3.0*result2(1,0), 1e-300);
	}

}
BOOST_AUTO_TEST_SUITE_END()

