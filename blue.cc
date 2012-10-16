/*
 * blue.cc
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "blue.hh"
#include <iostream>

using std::string;

blue::blue(string filename) {
	AverageDataParser dataparser(filename);

	errors=dataparser.getErrors();
	names=dataparser.getNames();
	covopts=dataparser.getCovoption();

	// lovely ROOT...
	TMatrixD tmp1(_columnVector(dataparser.getTotalErrors()));
	totalerrors.ResizeTo(tmp1);
	totalerrors=tmp1;

	TMatrixD tmp2(_columnVector(dataparser.getValues()));
	data.ResizeTo(tmp2);
	data=tmp2;

	dim=data.GetNrows();



//	TODO: To be implemented in AverageDataParser and here...
//	correlations;
//	hcov;
//	groupmatrix;
//	cov;
//	inv;


//----------------------------------
//  python code of constructor...
//	self.dataparser= AverageDataParser( filename )
//	        self.errors= self.dataparser.getErrors()
//	        self.names= self.dataparser.getNames()
//	        self.covopts= self.dataparser.getCovoption()
//	        self.correlations= self.dataparser.getCorrelations()
//	        self.hcov= self.dataparser.getCovariances()
//	        self.cov= self.dataparser.getTotalCovariance()
//	        self.inv= self.cov.getI()
//	        self.groupmatrix= numpy.matrix( self.dataparser.getGroupMatrix() )
//	        self.data= self.__columnVector( self.dataparser.getValues() )
//	        self.totalerrors= self.__columnVector( self.dataparser.getTotalErrors() )

}

blue::~blue() {
	// TODO Auto-generated destructor stub



}

TMatrixD blue::_columnVector(std::vector<float> stdvec){
	TMatrixD result;
	int size=stdvec.size();
	result.ResizeTo(size,1);
	for(int i=0; i<size; i++){
		result(i,0)=stdvec[i];
	}

	return result;
}


TMatrixD blue::calcWeightsMatrix() {

	TMatrixD gmTrans;

	gmTrans.ResizeTo(1, dim);
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

double blue::calcChisq(){
	TMatrixD delta=data - groupmatrix * calcAverage();
	TMatrixD deltaT;
	deltaT.ResizeTo(1,dim);
	deltaT.Transpose(delta);
	double chisq= (deltaT*inv*delta)(0,0);
	return chisq;
}




TMatrixD blue::calcPulls(){
	TMatrixD delta=data - groupmatrix * calcAverage();

	TMatrixD pulls=delta;
	pulls/= totalerrors;   // weird: TMatrix knows 'A/=B' but not 'A=C/B'
	return pulls;
}
