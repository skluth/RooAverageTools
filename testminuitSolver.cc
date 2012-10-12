// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE minuitSolverTestFixture
#include <boost/test/unit_test.hpp>

#include "minuitSolver.hh"


// Test cases:

// Self test for boost:
// BOOST_AUTO_TEST_CASE(testFails)
// {
// 	BOOST_FAIL("testFails self test failed");
// }

void testFCN(int &npar, double *gin, double &f, double *u, int flag);

class minuitSolverTestFixture {
	
public:
	minuitSolverTestFixture():
		testParArray(NULL),
		testParErrArray(NULL),
		strVec(NULL),
		testexpNDdof(0),
		testMaxParam(0)
	{
    BOOST_MESSAGE( "Create minuitSolverTestFixture" );
		
		minuitSolver solverForCtorTest(testFCN,testParArray,testParErrArray,strVec,testexpNDdof,testMaxParam);
	}
	
	~minuitSolverTestFixture()
	{
    BOOST_MESSAGE( "Tear down minuitSolverTestFixture" );
	}
	
private:
	double *testParArray;
	double *testParErrArray;
	char** strVec;
	int testexpNDdof;
	int testMaxParam;
};

void testFCN(int &npar, double *gin, double &f, double *u, int flag)
{
	npar = 2;
	f = u[1] + u[0] * u[2];
}

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( minuitsolvertestsuite, minuitSolverTestFixture )

BOOST_AUTO_TEST_CASE(testPasses)
{
	BOOST_CHECK_EQUAL( 1, 1 );
}

// BOOST_AUTO_TEST_CASE(testFails)
// {
// 	
// 	BOOST_
// }

BOOST_AUTO_TEST_SUITE_END()