#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include "INIReader.hh"

#include <string>
#include <vector>

#include <TObject.h>
#include <TMatrixD.h>

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );

  std::string getFilename() const;
  std::vector<float> getValues() const;
  std::vector<std::string> getNames() const;
  std::map<std::string, std::vector<float> > getErrors() const;
  std::vector<float> getTotalErrors() const;
  std::map<std::string, std::string> getCovoption() const;
  std::map<std::string, std::string> getCorrelations() const;
  std::map<std::string, TMatrixD> getCovariances() const;
  std::map<unsigned int, std::vector<float> > getSysterrorMatrix() const;

  void makeNames( const INIParser::INIReader& );
  void makeValues( const INIParser::INIReader& );
  void makeErrorsAndOptions( const INIParser::INIReader& );
  void makeCorrelations( const INIParser::INIReader& );
  void makeCovariances();
  Double_t calcCovariance( const std::string& covopt, 
			   const std::vector<float>& errors, 
			   size_t ierr, size_t jerr ) const;

private:
  std::map<std::string, std::vector<float> > m_errors;
  std::map<std::string, std::string> m_covopts;
  std::map<std::string, TMatrixD> m_covariances;
  std::map<std::string, std::string> m_correlations;
  std::vector<float> m_values;
  std::vector<std::string> m_names;
  std::string filename;

};





#endif
