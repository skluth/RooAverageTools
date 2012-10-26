#ifndef AVERAGEDATAPARSER_HH
#define AVERAGEDATAPARSER_HH

#include <string>
#include <vector>
#include <map>
#include <iostream>

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
		     StringMap correlations= StringMap(),
		     std::vector<std::string> groups= 
		     std::vector<std::string>() );

  std::vector<std::string> getNames() const;
  TVectorD getValues() const;
  VectorMap getErrors() const;
  StringMap getCovoption() const;
  StringMap getCorrelations() const;
  TVectorD getTotalErrors() const;
  MatrixMap getCovariances() const;
  MatrixMap getReducedCovariances() const;
  TMatrixDSym getTotalCovariances() const;
  TMatrixDSym getTotalReducedCovariances() const;
  std::map<int,TVectorD> getSysterrorMatrix() const;
  std::vector<std::string> getGroups() const;
  std::vector<std::string> getUniqueGroups() const;
  TMatrixD getGroupMatrix() const;
  void printInputs( std::ostream& ost=std::cout ) const;
  void printFilename( std::ostream& ost=std::cout ) const;
  void printNames( std::ostream& ost=std::cout ) const;
  void printGroups( std::ostream& ost=std::cout ) const;
  void printUniqueGroups( std::ostream& ost=std::cout ) const;
  void printValues( std::ostream& ost=std::cout ) const;
  void printErrors( std::ostream& ost=std::cout ) const;
  void printTotalErrors( std::ostream& ost=std::cout ) const;
  void printCorrelations( std::ostream& ost=std::cout ) const;
  void printCovariances( std::ostream& ost=std::cout,
			 std::ios_base::fmtflags flag=std::ios_base::fixed,
			 size_t prec=4 ) const;
  void printCorrelationMatrices( std::ostream& ost=std::cout ) const;
  std::string stripLeadingDigits( const std::string& word ) const;

private:

  void makeNames( const INIParser::INIReader& );
  void makeValues( const INIParser::INIReader& );
  void makeGroups( const INIParser::INIReader& );
  void makeGroupMatrix();
  void makeErrorsAndOptions( const INIParser::INIReader& );
  void checkRelativeErrors();
  void makeCorrelations( const INIParser::INIReader& );
  void makeCovariances();
  void makeTotalErrors();
  void initialise();
  Double_t calcCovariance( const std::string& covopt, 
			   const TVectorD& errors, 
			   size_t ierr, size_t jerr ) const;
  TMatrixDSym sumOverMatrixMap( const MatrixMap& ) const;
  void printvectorstring( const std::vector<std::string>& vec,
			  const std::string& txt,
			  std::ostream& ost=std::cout ) const;

  std::string m_filename;
  std::vector<std::string> m_names;
  TVectorD m_values;
  VectorMap m_errors;
  StringMap m_covopts;
  StringMap m_correlations;
  std::vector<std::string> m_groups;
  std::vector<std::string> m_uniquegroups;
  MatrixMap m_covariances;
  MatrixMap m_reducedCovariances;
  std::map<int,TVectorD> m_systerrmatrix;
  TMatrixD m_groupmatrix;
  TVectorD m_totalerrors;

};

#endif
