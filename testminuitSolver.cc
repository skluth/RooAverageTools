#include "minuitSolver.hh"

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE_testminuitSolver
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testFails)
{
	BOOST_FAIL("This test failed");
}


class testminuitSolver {
public:
	testminuitSolver(){}
	~testminuitSolver(){}
};
