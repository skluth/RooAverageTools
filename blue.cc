/*
 * blue.cc
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "blue.hh"





blue::blue(TString filename) {

}

blue::~blue() {
	// TODO Auto-generated destructor stub
}

TMatrixTSym<double> blue::calcWeightsMatrix(){




		TMatrixTSym<double> utvinvu = inv.Similarity(groupmatrix);  // gives groupmatrix^T * inv * groupmatrix
		TMatrixTSym<double> utvinvuinv =utvinvu.Invert();

		TMatrixTSym<double> gmTrans;
		gmTrans.Transpose(groupmatrix);
		TMatrixTSym<double> wm = utvinvuinv*gmTrans*inv;



		return wm;

}

