/*
 * blue.cc
 *
 *  Created on: 09.10.2012
 *      Author: sweber
 */

#include "Blue.hh"

#include <iostream>
#include <iomanip>

#include "TMath.h"

using std::string;
using std::vector;

Blue::Blue( const string& filename ) :
  m_parser( filename ), 
  m_invm( m_parser.getTotalCovariances() ) {
  m_invm.Invert();
  calcWeightsMatrix();
  calcAverage();
  calcChisq();
  calcPulls();
  errorAnalysis();
}

Blue::~Blue() {}

TMatrixD Blue::getWeightsMatrix() const {
  return m_weightsmatrix;
}
void Blue::calcWeightsMatrix() {
  TMatrixD gm( m_parser.getGroupMatrix() );
  TMatrixD gmT( gm );
  gmT.T();
  TMatrixDSym invm( m_invm );
  TMatrixDSym& utvinvuinv= invm.SimilarityT( gm );
  utvinvuinv.Invert();
  m_weightsmatrix.ResizeTo( gm.GetNcols(), gm.GetNrows() );
  m_weightsmatrix= utvinvuinv*gmT*m_invm;
  return;
}

TVectorD Blue::getAverage() const {
  return m_average;
}
void Blue::calcAverage() {
  TVectorD data= m_parser.getValues();
  m_average.ResizeTo( m_weightsmatrix.GetNrows() );
  m_average= m_weightsmatrix*data;
  return;
}

Double_t Blue::getChisq() const {
  return m_chisq;
}
void Blue::calcChisq() {
  TVectorD data= m_parser.getValues();
  TMatrixD gm= m_parser.getGroupMatrix();
  TVectorD delta= data - gm*m_average;
  TMatrixDSym invm( m_invm );
  m_chisq= invm.Similarity( delta );
  return;
}

TVectorD Blue::getPulls() const {
  return m_pulls;
}
void Blue::calcPulls() {
  TVectorD data= m_parser.getValues();
  TMatrixD gm= m_parser.getGroupMatrix();
  TVectorD totalerrors= m_parser.getTotalErrors();
  TVectorD delta= data - gm*m_average;
  Int_t nerr= data.GetNoElements();
  m_pulls.ResizeTo( nerr );
  for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
    m_pulls[ierr]= delta[ierr]/totalerrors[ierr];
  }
  return;
}

MatrixMap Blue::getErrors() const {
  return m_errorsmap;
}
void Blue::errorAnalysis() {
  MatrixMap covariances= m_parser.getCovariances();
  Int_t navg= m_weightsmatrix.GetNrows();
  TMatrixDSym avgsystcov( navg );
  TMatrixDSym avgtotcov( navg );
  for( MatrixMap::iterator mapitr= covariances.begin();
       mapitr != covariances.end(); mapitr++ ) {
    const string& errorkey= mapitr->first;
    TMatrixDSym cov= mapitr->second;
    cov.Similarity( m_weightsmatrix );
    avgtotcov+= cov;
    if( errorkey.find( "stat" ) == string::npos ) avgsystcov+= cov;
    m_errorsmap.insert( MatrixMap::value_type( errorkey, cov ) );
  }
  m_errorsmap.insert( MatrixMap::value_type( "syst", avgsystcov ) );
  m_errorsmap.insert( MatrixMap::value_type( "total", avgtotcov ) );
  return;
}

void Blue::printInputs( std::ostream& ost ) const {
  ost << "\nBest Linear Unbiased Estimator average\n" << std::endl;
  m_parser.printFilename( ost );
  ost << std::endl;
  m_parser.printNames( ost );
  if( m_parser.getUniqueGroups().size() > 1 ) {
    m_parser.printGroups( ost );
  }
  m_parser.printValues( ost );
  m_parser.printErrors( ost );
  ost << std::endl;
  m_parser.printCorrelations( ost );
  m_parser.printCovariances( ost );
  return;
}

void Blue::printResults( std::ostream& ost ) const {
  ost << "\nResults:" << std::endl;
  printChisq();
  ost << std::endl;
  m_parser.printNames( ost );
  size_t nuniquegroups= m_parser.getUniqueGroups().size();
  if( nuniquegroups > 1 ) m_parser.printGroups( ost );
  printWeights( ost );
  printPulls( ost );
  ost << std::endl;
  if( nuniquegroups > 1 ) m_parser.printUniqueGroups( ost );
  printAverages( ost );
  printErrors( ost );
  if( nuniquegroups > 1 ) {
    ost << std::endl;
    printCorrelations( ost );
  }
  ost << std::endl;
  return;
}

void Blue::printChisq( std::ostream& ost ) const {
  Int_t ndof= m_weightsmatrix.GetNcols() - m_weightsmatrix.GetNrows();
  Double_t chisqdof= m_chisq/Double_t(ndof);
  Double_t pvalue= TMath::Prob( m_chisq, ndof );
  ost.precision( 2 );
  ost.setf( std::ios_base::fixed );
  ost << "Chi^2= " << m_chisq << " for " << ndof << " d.o.f,"
      << " chi^2/d.o.f= " << chisqdof;
  ost.precision( 4 );
  ost << ", P(chi^2)= " << pvalue << std::endl;
  return;
}

void Blue::printWeights( std::ostream& ost ) const {
  Int_t navg= m_weightsmatrix.GetNrows();
  Int_t nvar= m_weightsmatrix.GetNcols();
  vector<string> uniquegroups= m_parser.getUniqueGroups();
  ost.precision( 4 );
  ost.setf( std::ios_base::fixed );
  for( Int_t iavg= 0; iavg < navg; iavg++ ) {
    string txt= "Weights";
    if( uniquegroups.size() > 1 ) txt+= " " + uniquegroups.at( iavg );
    ost << std::setw( 11 ) << txt+":";
    for( Int_t ivar= 0; ivar < nvar; ivar++ ) {
      ost << " " << std::setw(10) << m_weightsmatrix(iavg,ivar);
    }
    ost << std::endl;
  }
  return;
}

void Blue::printPulls( std::ostream& ost ) const {
  printVector( m_pulls, "Pulls:", ost );
  return;
}
void Blue::printAverages( std::ostream& ost ) const {
  printVector( m_average, "Average:", ost );
  return;
}
void Blue::printVector( const TVectorD& vec,
			const string& txt,
			std::ostream& ost ) const {
  ost << std::setw(11) << txt;
  ost.precision( 4 );
  ost.setf( std::ios_base::fixed );  
  for( Int_t i= 0; i < vec.GetNoElements(); i++ ) {
    ost << " " << std::setw(10) << vec[i];
  }
  ost << std::endl;
  return;
}

void Blue::printErrors( std::ostream& ost ) const {
  ost.precision( 4 );
  ost.setf( std::ios_base::fixed );  
  for( MatrixMap::const_iterator mapitr= m_errorsmap.begin();
       mapitr != m_errorsmap.end(); mapitr++ ) {
    string errorkey= mapitr->first;
    TMatrixD covm= mapitr->second;
    ost << std::setw(11) << m_parser.stripLeadingDigits( errorkey )+":";
    Int_t navg= covm.GetNrows();
    for( Int_t iavg= 0; iavg < navg; iavg++ ) {
      ost << " " << std::setw(10) << sqrt( covm(iavg,iavg) );
    }
    ost << std::endl;
  }
  return;
}

void Blue::printCorrelations( std::ostream& ost ) const {
  vector<string> uniquegroups= m_parser.getUniqueGroups();
  size_t navg= uniquegroups.size();
  if( navg > 1 ) {
    MatrixMap::const_iterator totitr= m_errorsmap.find( "total" );
    if( totitr == m_errorsmap.end() ) {
      ost << "Total covariance matrix not found" << std::endl;
      return;
    }
    TMatrixD totcov= totitr->second;
    ost << "Total correlations:\n  ";
    for( size_t iavg= 0; iavg < navg; iavg++ ) {
      ost << " " << std::setw( 5 ) << uniquegroups[iavg];
    }
    ost << std::endl;
    ost.precision( 2 );
    ost.setf( std::ios_base::fixed );    
    for( size_t iavg= 0; iavg < navg; iavg++ ) {
      ost << std::setw( 2 ) << uniquegroups[iavg];
      for( size_t javg= 0; javg < navg; javg++ ) {
	ost << " " << std::setw( 5 ) 
	    << totcov(iavg,javg)/sqrt(totcov(iavg,iavg)*totcov(javg,javg));
      }
      ost << std::endl;
    }
  }
  return;
}
