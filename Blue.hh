#ifndef BLUE_HH
#define BLUE_HH
/*
 * blue.hh
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "AverageDataParser.hh"

// C++ includes
#include <vector>
#include <map>
#include <iostream>

// ROOT includes
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
//#include "TString.h"

class Blue {

public:

  blue( std::string filename );
  virtual ~blue();

  //  TMatrixD _columnVector(std::vector<double> stdvec);
  
  TMatrixD calcWeightsMatrix();
  Double_t calcAverage();
  Double_t calcChisq();
  TMatrixD calcPulls();
  void printInputs();

private:

  std::map<std::string,TVectorD> errors;
  std::vector<std::string> names;
  std::map<std::string, std::string > covopts;
  std::map<std::string, std::vector<double> > correlations;
  std::map<std::string, TMatrixD > hcov;
  TMatrixD cov;
  TMatrixDSym inv;
  TMatrixD groupmatrix;
  TVectorD data;
  TVectorD totalerrors;
  

  int dim;  // max number of rows/columns


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

#endif /* BLUE_HH */
