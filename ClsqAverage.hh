#ifndef CLSQAVERAGE_HH
#define CLSQAVERAGE_HH

#include "AverageDataParser.hh"

#include "Rtypes.h"

#include <string>

class ClsqAverage {

public:

  ClsqAverage( const std::string& filename );

  Double_t getAverage( bool lBlobel=false );

private:

  AverageDataParser dataparser;

};




#endif
