/*
 * blue.cc
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "Blue.hh"

#include <iostream>

using std::string;

Blue::Blue( const string& filename ) :
  m_parser( filename ) {
  TMatrixDSym cov( m_parser.getTotalCovariances() );
  Int_t ndim= cov.GetNrows();
  m_invm.ResizeTo( ndim, ndim );
  m_invm= cov.Invert();
}

Blue::~Blue() {}

TMatrixD Blue::calcWeightsMatrix() const {
  TMatrixD gm( m_parser.getGroupMatrix() );
  TMatrixD gmT( gm );
  gmT.T();
  TMatrixD utvinvuinv= gmT*m_invm*gm;
  utvinvuinv.Invert();
  TMatrixD wm= utvinvuinv*gmT*m_invm;
  return wm;
}

TVectorD Blue::calcAverage() const {
  TMatrixD wm= calcWeightsMatrix();
  TVectorD data= m_parser.getValues();
  TVectorD avg= wm*data;
  return avg;
}

Double_t Blue::calcChisq() const {
  TVectorD avg= calcAverage();
  TVectorD data= m_parser.getValues();
  TMatrixD gm= m_parser.getGroupMatrix();
  TVectorD delta= data - gm*avg;
  Double_t chisq= m_invm.Similarity( delta );
  return chisq;
}

TVectorD Blue::calcPulls() const {
  TVectorD avg= calcAverage();
  TVectorD data= m_parser.getValues();
  TMatrixD gm= m_parser.getGroupMatrix();
  TVectorD totalerrors= m_parser.getTotalErrors();
  TVectorD delta= data - gm*avg;
  Int_t nerr= data.GetNoElements();
  TVectorD pulls( nerr ); 
  for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
    pulls[ierr]= delta[ierr]/totalerrors[ierr];
  }
  return pulls;
}

