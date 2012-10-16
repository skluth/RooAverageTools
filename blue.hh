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
#include "vector"
#include "map"
#include <map>
#include <iostream>
////////// ROOT includes
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixT.h>
#include <TMatrixTSym.h>
#include <TMatrixDSym.h>
#include <TString.h>



// objects as given by first python test - so you can see values and types
//errors:
//{'00stat': [0.3, 0.33, 0.4], '01err1': [1.1, 1.3, 1.5], '03err3': [2.4, 3.1, 3.5], '02err2': [0.9, 1.5, 1.9], '04err4': [1.4, 2.9, 3.3]}
//names:
//['Val1', 'Val2', 'Val3']
//covopts:
//{'00stat': 'c', '01err1': 'm', '03err3': 'p', '02err2': 'm', '04err4': 'f'}
//correlations:
//{'00stat': [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0], '01err1': ['p', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'], '02err2': ['f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f']}
//hcov:
//{'00stat': matrix([[ 0.09  ,  0.    ,  0.    ],
//        [ 0.    ,  0.1089,  0.    ],
//        [ 0.    ,  0.    ,  0.16  ]]), '01err1': matrix([[ 1.21,  1.21,  1.21],
//        [ 1.21,  1.69,  1.69],
//        [ 1.21,  1.69,  2.25]]), '03err3': matrix([[  5.76,   5.76,   5.76],
//        [  5.76,   9.61,   9.61],
//        [  5.76,   9.61,  12.25]]), '02err2': matrix([[ 0.81,  1.35,  1.71],
//        [ 1.35,  2.25,  2.85],
//        [ 1.71,  2.85,  3.61]]), '04err4': matrix([[  1.96,   4.06,   4.62],
//        [  4.06,   8.41,   9.57],
//        [  4.62,   9.57,  10.89]])}
//cov:
//[[  9.83    12.38    13.3   ]
// [ 12.38    22.0689  23.72  ]
// [ 13.3     23.72    29.16  ]]
//inv:
//[[ 0.34659934 -0.19506397  0.000588  ]
// [-0.19506397  0.47026712 -0.29356603]
// [ 0.000588   -0.29356603  0.27282462]]
//groupmatrix:
//[[1]
// [1]
// [1]]
//data:
//[[ 171.5]
// [ 173.1]
// [ 174.5]]
//totalerrors:
//[[ 3.13528308]
// [ 4.69775478]
// [ 5.4       ]]

class blue {
public:




  std::map<std::string, std::vector<float> > errors;
  std::vector<std::string> names;
  std::map<std::string, std::string > covopts;
  std::map<std::string, std::vector<float> > correlations;
  std::map<std::string, TMatrixD > hcov;
  TMatrixD cov;
  TMatrixDSym inv;
  TMatrixD groupmatrix;
  TMatrixD data;
  TMatrixD totalerrors;
  

  int dim;  // max number of rows/columns
public:
  blue(std::string filename);
  virtual ~blue();
  TMatrixD _columnVector(std::vector<float> stdvec);
  
  TMatrixD  calcWeightsMatrix();
  double calcAverage();
  double calcChisq();
  TMatrixD calcPulls();
  void printInputs();

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
