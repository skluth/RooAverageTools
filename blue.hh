/*
 * blue.hh
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#ifndef BLUE_HH_
#define BLUE_HH_



#include "AverageDataParser.hh"
////////// C++ includes

#include <iostream>
////////// ROOT includes
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixT.h>
#include <TMatrixTSym.h>
#include <TMatrixDSym.h>
#include <TString.h>





class blue {
public:

	TMatrixDSym inv;
	TMatrixD groupmatrix;
	TMatrixD data;
	TMatrixD totalerrors;
public:
	blue(TString filename);
	virtual ~blue();

	//__columnVector( self, inlist ):
	TMatrixD  calcWeightsMatrix();
	double calcAverage();
	double calcChisq();
	TMatrixD calcPulls();

	    //# Print the input data:
	    //printInputs( self, cov=False  ):

	    // Print results:
	    //printResults( self ):

	    //# Error analysis from weights and input covariance matrices:
	    // __makeZeroMatrix( self, ndim ):

	    //def errorAnalysis( self ):
	      // Scan correlations between p and f:
	    //scanCorr( self, step ):


};

#endif /* BLUE_HH_ */
