#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include <string>
#include <vector>
#include <map>

#include "TVectorD.h"
#include "TMatrixD.h"

namespace INIParser {
  class INIReader;
}

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );
  AverageDataParser( const std::vector<std::string>& names,
		     const TVectorD& values,
		     const std::map<std::string,TVectorD>& errors,
		     const std::map<std::string,std::string>& covopts,
		     std::map<std::string,std::string> correlations=
		     std::map<std::string,std::string>(),
		     std::vector<std::string> groups= 
		     std::vector<std::string>() );

  std::vector<std::string> getNames() const;
  TVectorD getValues() const;
  std::map<std::string,TVectorD> getErrors() const;
  std::map<std::string, std::string> getCovoption() const;
  std::map<std::string, std::string> getCorrelations() const;
  TVectorD getTotalErrors() const;
  std::map<std::string, TMatrixD> getCovariances() const;
  std::map<std::string, TMatrixD> getReducedCovariances() const;
  TMatrixD getTotalCovariances() const;
  TMatrixD getTotalReducedCovariances() const;
  std::map<int,TVectorD> getSysterrorMatrix() const;
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
			   const TVectorD& errors, 
			   size_t ierr, size_t jerr ) const;
  TMatrixD sumOverMatrixMap( const std::map<std::string,TMatrixD>& ) const;

  std::vector<std::string> m_names;
  TVectorD m_values;
  std::map<std::string,TVectorD> m_errors;
  std::map<std::string,std::string> m_covopts;
  std::map<std::string,std::string> m_correlations;
  std::vector<std::string> m_groups;
  std::map<std::string,TMatrixD> m_covariances;
  std::map<std::string,TMatrixD> m_reducedCovariances;
  std::map<int,TVectorD> m_systerrmatrix;
  TMatrixD m_groupmatrix;

};

#endif
