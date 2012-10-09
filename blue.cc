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

TMatrixD blue::calcWeightsMatrix() {

	TMatrixD gmTrans;
	gmTrans.ResizeTo(1, 3);
	gmTrans.Transpose(groupmatrix);

	TMatrixD utvinvu = gmTrans * inv * groupmatrix;
	TMatrixD utvinvuinv = utvinvu.Invert();
	TMatrixD wm = utvinvuinv * gmTrans * inv;


	return wm;

}



double blue::calcAverage(){
	TMatrixD wm=calcWeightsMatrix();
	double avg= (wm*data)(0,0);
	return avg;

}

