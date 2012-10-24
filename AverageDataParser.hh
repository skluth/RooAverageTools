#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include <string>
#include <vector>
#include <map>

#include "TVectorD.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"

namespace INIParser {
  class INIReader;
}

typedef std::map<std::string,TMatrixDSym> MatrixMap;
typedef std::map<std::string,TVectorD> VectorMap;
typedef std::map<std::string,std::string> StringMap;

class AverageDataParser {

public:

  AverageDataParser( const std::string& fname );
  AverageDataParser( const std::vector<std::string>& names,
		     const TVectorD& values,
		     const VectorMap& errors,
		     const StringMap& covopts,
		     StringMap correlations=
		     StringMap(),
		     std::vector<std::string> groups= 
		     std::vector<std::string>() );

  std::vector<std::string> getNames() const;
  TVectorD getValues() const;
  VectorMap getErrors() const;
  std::map<std::string, std::string> getCovoption() const;
  std::map<std::string, std::string> getCorrelations() const;
  TVectorD getTotalErrors() const;
  MatrixMap getCovariances() const;
  MatrixMap getReducedCovariances() const;
  TMatrixDSym getTotalCovariances() const;
  TMatrixDSym getTotalReducedCovariances() const;
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
  TMatrixDSym sumOverMatrixMap( const MatrixMap& ) const;

  std::vector<std::string> m_names;
  TVectorD m_values;
  VectorMap m_errors;
  StringMap m_covopts;
  StringMap m_correlations;
  std::vector<std::string> m_groups;
  MatrixMap m_covariances;
  MatrixMap m_reducedCovariances;
  std::map<int,TVectorD> m_systerrmatrix;
  TMatrixD m_groupmatrix;

};

#endif
