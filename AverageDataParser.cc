

#include "AverageDataParser.hh"


using std::string;
using namespace INIParser;


AverageDataParser::AverageDataParser( const string& fname ) :
  filename( fname ), reader( fname ) {}


string AverageDataParser::getFilename() const {
  return filename;
}

