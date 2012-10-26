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
}

Blue::~Blue() {}

TMatrixD Blue::calcWeightsMatrix() const {
  TMatrixD gm( m_parser.getGroupMatrix() );
  TMatrixD gmT( gm );
  gmT.T();
  TMatrixDSym invm( m_invm );
  TMatrixDSym& utvinvuinv= invm.SimilarityT( gm );
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
  TMatrixDSym invm( m_invm );
  Double_t chisq= invm.Similarity( delta );
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

MatrixMap Blue::errorAnalysis() const {
  MatrixMap covariances= m_parser.getCovariances();
  TMatrixD wm= calcWeightsMatrix();
  Int_t navg= wm.GetNrows();
  TMatrixDSym avgsystcov( navg );
  TMatrixDSym avgtotcov( navg );
  MatrixMap avgCovariancesMap;
  for( MatrixMap::iterator mapitr= covariances.begin();
       mapitr != covariances.end(); mapitr++ ) {
    const string& errorkey= mapitr->first;
    TMatrixDSym cov= mapitr->second;
    cov.Similarity( wm );
    avgtotcov+= cov;
    if( errorkey.find( "stat" ) == string::npos ) avgsystcov+= cov;
    avgCovariancesMap.insert( MatrixMap::value_type( errorkey, cov ) );
  }
  avgCovariancesMap.insert( MatrixMap::value_type( "syst", avgsystcov ) );
  avgCovariancesMap.insert( MatrixMap::value_type( "total", avgtotcov ) );
  return avgCovariancesMap;
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
  ost << std::endl;
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
  printCorrelations( ost );
  ost << std::endl;
}

void Blue::printChisq( std::ostream& ost ) const {
  TMatrixD wm= calcWeightsMatrix();
  Int_t navg= wm.GetNrows();
  Int_t nvar= wm.GetNcols();
  Int_t ndof= nvar-navg;
  Double_t chisq= calcChisq();
  Double_t chisqdof= chisq/Double_t(ndof);
  Double_t pvalue= TMath::Prob( chisq, ndof );
  ost << "Results:" << std::endl;
  ost.precision( 2 );
  ost.setf( std::ios_base::fixed );
  ost << "\nChi^2= " << chisq << " for " << ndof << " d.o.f,";
  ost << " chi^2/d.o.f= " << chisqdof;
  ost.precision( 4 );
  ost << ", P(chi^2)= " << pvalue << std::endl;
}

void Blue::printWeights( std::ostream& ost ) const {
  TMatrixD wm= calcWeightsMatrix();
  Int_t navg= wm.GetNrows();
  Int_t nvar= wm.GetNcols();
  vector<string> uniquegroups= m_parser.getUniqueGroups();
  ost.precision( 4 );
  ost.setf( std::ios_base::fixed );
  for( Int_t iavg= 0; iavg < navg; iavg++ ) {
    string txt= "Weights";
    if( uniquegroups.size() > 1 ) {
      txt+= " " + uniquegroups.at( iavg );
    }
    ost << std::setw( 11 ) << txt+":";
    for( Int_t ivar= 0; ivar < nvar; ivar++ ) {
      ost << " " << std::setw(10) << wm(iavg,ivar);
    }
    ost << std::endl;
  }
}

void Blue::printPulls( std::ostream& ost ) const {
  printVector( calcPulls(), "Pulls:", ost );
}
void Blue::printAverages( std::ostream& ost ) const {
  printVector( calcAverage(), "Average:", ost );
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
}

void Blue::printErrors( std::ostream& ost ) const {
  MatrixMap errorsmap= errorAnalysis();
  for( MatrixMap::const_iterator mapitr= errorsmap.begin();
       mapitr != errorsmap.end(); mapitr++ ) {
    string errorkey= mapitr->first;
    TMatrixD covm= mapitr->second;
    ost << std::setw(11) << m_parser.stripLeadingDigits( errorkey )+":";
    Int_t navg= covm.GetNrows();
    for( Int_t iavg= 0; iavg < navg; iavg++ ) {
      ost << " " << std::setw(10) << sqrt( covm(iavg,iavg) );
    }
    ost << std::endl;
  }
}

void Blue::printCorrelations( std::ostream& ost ) const {
  vector<string> uniquegroups= m_parser.getUniqueGroups();
  size_t navg= uniquegroups.size();
  if( navg > 1 ) {
    MatrixMap errorsmap= errorAnalysis();
    TMatrixD totcov= errorsmap["total"];
    Int_t nerr= totcov.GetNrows();
    ost << "\nTotal correlations:\n  ";
    for( size_t iavg= 0; iavg < navg; iavg++ ) {
      ost << " " << std::setw( 5 ) << uniquegroups[iavg];
    }
    ost << std::endl;
    ost.precision( 2 );
    ost.setf( std::ios_base::fixed );    
    for( Int_t iavg= 0; iavg < navg; iavg++ ) {
      ost << std::setw( 2 ) << uniquegroups[iavg];
      for( Int_t javg= 0; javg < nerr; javg++ ) {
	ost << " " << std::setw( 5 ) 
	    << totcov(iavg,javg)/sqrt(totcov(iavg,iavg)*totcov(javg,javg));
      }
      ost << std::endl;
    }
  }
}
