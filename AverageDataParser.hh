#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include <string>
#include <vector>
#include <map>

#include "TMatrixD.h"

namespace INIParser {
  class INIReader;
}

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );
  AverageDataParser( const std::vector<std::string>& names,
		     const std::vector<double>& values,
		     const std::map<std::string,std::vector<double> >& errors,
		     const std::map<std::string,std::string>& covopts,
		     std::map<std::string,std::string> correlations=
		     std::map<std::string,std::string>(),
		     std::vector<std::string> groups= 
		     std::vector<std::string>() );

  std::vector<std::string> getNames() const;
  std::vector<double> getValues() const;
  std::map<std::string, std::vector<double> > getErrors() const;
  std::map<std::string, std::string> getCovoption() const;
  std::map<std::string, std::string> getCorrelations() const;
  std::vector<double> getTotalErrors() const;
  std::map<std::string, TMatrixD> getCovariances() const;
  std::map<std::string, TMatrixD> getReducedCovariances() const;
  std::map<int,std::vector<double> > getSysterrorMatrix() const;
  std::vector<std::string> getGroups() const;
  TMatrixD getGroupMatrix() const;

private:

  void makeNames( const INIParser::INIReader& );
  void makeValues( const INIParser::INIReader& );
  void makeGroups( const INIParser::INIReader& );
  void makeErrorsAndOptions( const INIParser::INIReader& );
  void checkRelativeErrors();
  void makeCorrelations( const INIParser::INIReader& );
  void makeCovariances();
  Double_t calcCovariance( const std::string& covopt, 
			   const std::vector<double>& errors, 
			   size_t ierr, size_t jerr ) const;

  std::vector<std::string> m_names;
  std::vector<double> m_values;
  std::map<std::string,std::vector<double> > m_errors;
  std::map<std::string,std::string> m_covopts;
  std::map<std::string,std::string> m_correlations;
  std::vector<std::string> m_groups;
  std::map<std::string,TMatrixD> m_covariances;
  std::map<std::string,TMatrixD> m_reducedCovariances;
  std::map<int,std::vector<double> > m_systerrmatrix;
  TMatrixD m_groupmatrix;

};

#endif
