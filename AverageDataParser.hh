#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH


#include "INIReader.hh"

#include <string>
#include <vector>

#include "TObject.h"
#include "TMatrixD.h"

using std::string;

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );

  std::string getFilename() const;

  std::vector<float> getValues() const;

  std::vector<string> getNames() const;

  std::map<string, std::vector<float> > getErrors();

  void getErrorsAndOptions();

  std::vector<float> getTotalErrors() const;

  std::map<string, string> getCovoption();

  std::map<string, string> getCorrelations() const;

  std::map<string, TMatrixD> getCovariances() const;

  std::map<unsigned int, std::vector<float> > getSysterrorMatrix() const;

private:
  std::map<string, std::vector<float> > * m_errors;
  std::map<string, string> * m_covopts;

  std::string filename;
  INIParser::INIReader reader;

};





#endif
