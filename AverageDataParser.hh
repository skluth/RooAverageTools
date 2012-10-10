#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH


#include "INIReader.hh"

#include <string>
#include <vector>

using std::string;

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );

  std::string getFilename() const;

  std::vector<float> getValues() const;

  std::vector<string> getNames() const;

  std::map<string, std::vector<float> > getErrors() const;

  std::vector<float> getTotalErrors() const;

  std::map<string, string> getCovoption() const;

  std::map<string, string> getCorrelations() const;

  std::map<unsigned int, std::vector<float> > getSysterrorMatrix() const;

private:

  std::string filename;
  INIParser::INIReader reader;

};





#endif
