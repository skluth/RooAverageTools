#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH


#include "INIReader.hh"

#include <string>

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );

  std::string getFilename() const;

private:

  std::string filename;
  INIParser::INIReader reader;

};





#endif
