/*
 * blue.cc
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "blue.hh"
#include <iostream>

using std::string;

Blue::Blue(string filename) {
	AverageDataParser dataparser(filename);

	errors=dataparser.getErrors();
	names=dataparser.getNames();
	covopts=dataparser.getCovoption();

	// lovely ROOT...
	TVectorD tmp1( dataparser.getTotalErrors() );
	totalerrors.ResizeTo(tmp1);
	totalerrors=tmp1;

	//TMatrixD tmp2(_columnVector(dataparser.getValues()));
	TVectorD tmp2( dataparser.getValues() );
	data.ResizeTo(tmp2);
	data=tmp2;

	dim=data.GetNrows();

}


Blue::~Blue() {}


TMatrixD Blue::_columnVector(std::vector<double> stdvec){
	TMatrixD result;
	int size=stdvec.size();
	result.ResizeTo(size,1);
	for(int i=0; i<size; i++){
		result(i,0)=stdvec[i];
	}

	return result;
}


TMatrixD Blue::calcWeightsMatrix() {

	TMatrixD gmTrans;

	gmTrans.ResizeTo(1, dim);
	gmTrans.Transpose(groupmatrix);

	TMatrixD utvinvu = gmTrans * inv * groupmatrix;
	TMatrixD utvinvuinv = utvinvu.Invert();
	TMatrixD wm = utvinvuinv * gmTrans * inv;


	return wm;

}



double Blue::calcAverage(){
	TMatrixD wm=calcWeightsMatrix();
	double avg= (wm*data)(0);
	return avg;

}

double Blue::calcChisq(){
  //TVectorD delta= data - groupmatrix * calcAverage();
	// TMatrixD deltaT;
	// deltaT.ResizeTo(1,dim);
	// deltaT.Transpose(delta);
	// double chisq= (deltaT*inv*delta)(0,0);
	// return chisq;
	return -99;
}




TMatrixD Blue::calcPulls(){
	// TMatrixD delta=data - groupmatrix * calcAverage();

	// TMatrixD pulls=delta;
	// pulls/= totalerrors;   // weird: TMatrix knows 'A/=B' but not 'A=C/B'
	// return pulls;
}

