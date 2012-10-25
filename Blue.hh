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
  // void printInputs();

private:

  AverageDataParser m_parser;
  TMatrixDSym m_invm;

};

#endif /* BLUE_HH */
