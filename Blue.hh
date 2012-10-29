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
#include <string>
#include <iostream>

// ROOT includes
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"

class Blue {

public:

  Blue( const std::string& filename );
  virtual ~Blue();  
  TMatrixD getWeightsMatrix() const;
  TVectorD getAverage() const;
  Double_t getChisq() const;
  TVectorD getPulls() const;
  MatrixMap getErrors() const;
  void printInputs( std::ostream& ost= std::cout ) const;
  void printResults( std::ostream& ost= std::cout ) const;
  void printChisq( std::ostream& ost= std::cout ) const;
  void printWeights( std::ostream& ost= std::cout ) const;
  void printPulls( std::ostream& ost= std::cout ) const;
  void printAverages( std::ostream& ost= std::cout ) const;
  void printErrors( std::ostream& ost= std::cout ) const;
  void printCorrelations( std::ostream& ost= std::cout ) const;

private:

  void calcWeightsMatrix();
  void calcAverage();
  void calcChisq();
  void calcPulls();
  void errorAnalysis();
  void printVector( const TVectorD& vec, const std::string& txt,
		    std::ostream& ost= std::cout ) const;
  AverageDataParser m_parser;
  TMatrixDSym m_invm;
  TMatrixD m_weightsmatrix;
  TVectorD m_average;
  Double_t m_chisq;
  TVectorD m_pulls;
  MatrixMap m_errorsmap;

};

#endif /* BLUE_HH */
