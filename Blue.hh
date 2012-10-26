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
  TMatrixD calcWeightsMatrix() const;
  TVectorD calcAverage() const;
  Double_t calcChisq() const;
  TVectorD calcPulls() const;
  MatrixMap errorAnalysis() const;
  void printInputs( std::ostream& ost= std::cout ) const;
  void printResults( std::ostream& ost= std::cout ) const;
  void printChisq( std::ostream& ost= std::cout ) const;
  void printWeights( std::ostream& ost= std::cout ) const;
  void printPulls( std::ostream& ost= std::cout ) const;
  void printAverages( std::ostream& ost= std::cout ) const;
  void printErrors( std::ostream& ost= std::cout ) const;
  void printCorrelations( std::ostream& ost= std::cout ) const;


private:

  void printVector( const TVectorD& vec, const std::string& txt,
		    std::ostream& ost= std::cout ) const;
  AverageDataParser m_parser;
  TMatrixDSym m_invm;

};

#endif /* BLUE_HH */
