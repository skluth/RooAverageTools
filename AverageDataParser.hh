#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include "INIReader.hh"

#include <string>
#include <vector>

#include "TObject.h"
#include "TMatrixD.h"

//using std::string;

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );

  std::string getFilename() const;

  std::vector<float> getValues() const;

  std::vector<std::string> getNames() const;

  std::map<std::string, std::vector<float> > getErrors();

  void getErrorsAndOptions();

  std::vector<float> getTotalErrors();

  std::map<std::string, std::string> getCovoption();

  std::map<std::string, std::string> getCorrelations() const;

  std::map<std::string, TMatrixD> getCovariances() const;

  std::map<unsigned int, std::vector<float> > getSysterrorMatrix() const;

private:
  std::map<std::string, std::vector<float> > m_errors;
  std::map<std::string, std::string> m_covopts;

  std::string filename;
  INIParser::INIReader reader;

};





#endif
