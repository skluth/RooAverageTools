
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "blue.hh"

#include "stdio.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>



//// ROOT includes

#include <TMatrixD.h>
#include <TMatrixDSym.h>


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
  blueTestFixture() : blueObject( "filename" ) {
    BOOST_MESSAGE( "Create blueTestFixture" );

    /*
     *
     * AS LONG AS WE DO NOT HAVE A WORKING AVERAGE PARSER...
     *
     *
     */

    double invdata[]={ 0.34659934,-0.19506397, 0.000588,
                        -0.19506397 ,0.47026712,-0.29356603,
                        0.000588,-0.29356603,  0.27282462};
    blueObject.inv.ResizeTo(3,3);
    blueObject.inv.SetMatrixArray(invdata);

    double groupdata[]={1,1,1};
    blueObject.groupmatrix.ResizeTo(3,1);
    blueObject.groupmatrix.SetMatrixArray(groupdata);

    double datadata[]={171.5,  173.1,  174.5};
    blueObject.data.ResizeTo(3,1);
    blueObject.data.SetMatrixArray(datadata);
    /*
     *
     *
     * END OF WORKAROUND
     *
     *
     */

  }
  virtual ~blueTestFixture() {
    BOOST_MESSAGE( "Tear down blueTestFixture" );
  }
  blue blueObject;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( bluesuite, blueTestFixture )

// Test cases:


BOOST_AUTO_TEST_CASE( testCalcWeightsMatrix ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcWeightsMatrix()");

	double expected[]={1.3390306603614366, -0.16163492961906992, -0.17739573074236697};

	TMatrixD result=blueObject.calcWeightsMatrix();
	for(int i=0; i<3; i++){

		BOOST_CHECK_CLOSE_FRACTION(expected[i], result(0,i), 1e-6);
	}


}

BOOST_AUTO_TEST_CASE( testCalcAverage ) {
	BOOST_MESSAGE("Testing >>>>>>>>>> blue::calcAverage()");

	double expected = 170.70919692;

	double result = blueObject.calcAverage();


	BOOST_CHECK_CLOSE_FRACTION(expected, result, 1e-9);



}


BOOST_AUTO_TEST_SUITE_END()

