

#include "AverageDataParser.hh"
#include "INIReader.hh"
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <math.h>
#include <iomanip>

// Commonly used identifiers
using std::string;
using std::vector;
using std::map;
using std::endl;
using namespace INIParser;

// Ctors:
AverageDataParser::AverageDataParser( const string& fname ) 
  : m_filename( fname ) {
  INIParser::INIReader reader( fname );
  makeNames( reader );
  makeValues( reader );
  makeGroups( reader );
  makeErrorsAndOptions( reader );
  makeCorrelations( reader );
  initialise();
}

AverageDataParser::AverageDataParser( const vector<string>& names,
				      const TVectorD& values,
				      const VectorMap& errors,
				      const StringMap& covopts,
				      StringMap correlations,
				      vector<string> groups ) :
  m_filename( "NONE" ), m_names( names ), m_values( values ), 
  m_errors( errors ), m_covopts( covopts ), m_correlations( correlations ), 
  m_groups( groups ) {
  initialise();
}

void AverageDataParser::initialise() {
  checkRelativeErrors();
  makeCovariances();
  makeTotalErrors();
  makeGroupMatrix();  
}

// Return data values:
TVectorD AverageDataParser::getValues() const {
  return m_values;
}
void AverageDataParser::makeValues( const INIParser::INIReader& reader ) {
  string valuestring= reader.get( "Data", "values", "" );
  vector<string> valuetokens= INIParser::getTokens( valuestring );
  size_t ntok= valuetokens.size();
  m_values.ResizeTo( ntok );
  for( size_t itok= 0; itok != ntok; itok++ ) {
    double value= INIParser::stringToType( valuetokens[itok], 0.0 );
    m_values[itok]= value;
  }
  return;
}

// Return all variable names:
vector<string> AverageDataParser::getNames() const {
  return m_names;
}
void AverageDataParser::makeNames( const INIParser::INIReader& reader ) {
  string namestring= reader.get( "Data", "names", "" );
  m_names= INIParser::getTokens( namestring );
  return;
}

// Return groups information:
vector<string> AverageDataParser::getGroups() const {
  return m_groups;
}
vector<string> AverageDataParser::getUniqueGroups() const {
  return m_uniquegroups;
}
TMatrixD AverageDataParser::getGroupMatrix() const {
  return m_groupmatrix;
}
void AverageDataParser::makeGroups( const INIParser::INIReader& reader ) {
  string groupsstring= reader.get( "Data", "groups", "" );
  if( !( groupsstring == "" ) ) {
    vector<string> grouptokens= INIParser::getTokens( groupsstring );
    for( size_t itok= 0; itok != grouptokens.size(); itok++ ) {
      m_groups.push_back( grouptokens[itok] );
    }
  }
  else {
    for( size_t iname= 0; iname < m_names.size(); iname++ ) {
      m_groups.push_back( "a" );
    }
  }
}
void AverageDataParser::makeGroupMatrix() {
  m_uniquegroups= m_groups;
  std::sort( m_uniquegroups.begin(), m_uniquegroups.end() );
  vector<string>::iterator uniqueend= std::unique( m_uniquegroups.begin(), 
						   m_uniquegroups.end() );
  m_uniquegroups.resize( uniqueend - m_uniquegroups.begin() );
  m_groupmatrix.ResizeTo( m_groups.size(), m_uniquegroups.size() );
  for( size_t igroup= 0; igroup < m_groups.size(); igroup++ ) {
    vector<string>::iterator itr= std::find( m_uniquegroups.begin(), 
					     m_uniquegroups.end(), 
					     m_groups[igroup] );
    if( itr != m_uniquegroups.end() ) {
      size_t groupindex= itr - m_uniquegroups.begin();
      m_groupmatrix( igroup, groupindex )= 1.0;
    }
  }
  return;
}


// Return map of error values for each error category:
VectorMap AverageDataParser::getErrors() const {
  return m_errors;
}

// Return map of covariance options
map<string, string> AverageDataParser::getCovoption() const {
  return m_covopts;
}

// Read errors and covariance options from Data section:
// Predicate for remove_if below:
class Match {
public:
  Match( const string& s ) : reference( s ) {}
  bool operator()( const string& teststr ) const {
    return reference == teststr;
  }
private:
  string reference;
};
void AverageDataParser::checkRelativeErrors() {
  for( VectorMap::iterator mapitr= m_errors.begin();
       mapitr != m_errors.end(); mapitr++ ) {
    string errorkey= mapitr->first;
    if( m_covopts[errorkey].find( "%" ) != string::npos ) {
      TVectorD& errors= mapitr->second;
      for( Int_t ierr= 0; ierr != errors.GetNoElements(); ierr++ ) {
        errors[ierr]*= m_values[ierr] / 100.0;
      }
    }
  }
}
void AverageDataParser::makeErrorsAndOptions( const INIParser::INIReader& 
					      reader ) {
  vector<string> keys= reader.getNames( "Data" );
  vector<string> removekeys;
  removekeys.push_back( "names" );
  removekeys.push_back( "values" );
  removekeys.push_back( "groups" );
  for( size_t ikey= 0; ikey != removekeys.size(); ikey++ ) {
    keys.erase( std::remove_if( keys.begin(), keys.end(),
				Match( removekeys[ikey] ) ),
		keys.end() );
  }
  for( size_t ikey= 0; ikey != keys.size(); ikey++ ) {
    string key= keys[ikey];
    string elementstring= reader.get( "Data", key, "" );
    vector<string> elementtokens= INIParser::getTokens( elementstring );
    string covopt= elementtokens.back();
    m_covopts[key]= covopt;
    elementtokens.pop_back();
    //    vector<double> elements2;
    size_t ntok= elementtokens.size();
    TVectorD elements( ntok );
    for( size_t itok= 0; itok != ntok; itok++ ) {
      double element= INIParser::stringToType( elementtokens[itok], 0.0 );      
      //      elements2.push_back( element );
      elements[itok]= element;
    }
    m_errors.insert( VectorMap::value_type( key, elements ) );
  }
  return;
}

// Return total errors for each variable:
TVectorD AverageDataParser::getTotalErrors() const {
  return m_totalerrors;
}
void AverageDataParser::makeTotalErrors() {
  m_totalerrors.ResizeTo( m_values.GetNoElements() );
  for( VectorMap::const_iterator itr= m_errors.begin(); 
       itr != m_errors.end(); itr++ ) {
    const TVectorD& errors= itr->second;
    for( Int_t ierr= 0; ierr < errors.GetNoElements(); ierr++ ) {
      m_totalerrors[ierr]+= errors[ierr]*errors[ierr];
    }
  }
  for( Int_t ierr= 0; ierr < m_totalerrors.GetNoElements(); ierr++ ) {
    m_totalerrors[ierr]= sqrt( m_totalerrors[ierr] );
  }
  return;
}

// Return sums of covariance matrices:
TMatrixDSym AverageDataParser::getTotalReducedCovariances() const {
  return sumOverMatrixMap( m_reducedCovariances );
}
TMatrixDSym AverageDataParser::getTotalCovariances() const {
  return sumOverMatrixMap( m_covariances );
}
TMatrixDSym AverageDataParser::sumOverMatrixMap( const MatrixMap& matrixmap ) const {
  Int_t ndim= m_values.GetNoElements();
  TMatrixDSym total( ndim );
  for( MatrixMap::const_iterator mapitr= matrixmap.begin();
       mapitr != matrixmap.end(); mapitr++ ) {
    total+= mapitr->second;
  }
  return total;
}

// Read detailed correlation information as a string from extra section
// "Covariances" if indicated by option in "Data" section.
// On failure the map contains empty strings.
StringMap AverageDataParser::getCorrelations() const {
  return m_correlations;
}
void AverageDataParser::makeCorrelations( const INIParser::INIReader& reader ) {
  for( map<string, string>::const_iterator itr= m_covopts.begin();
       itr != m_covopts.end(); itr++ ) {
    string covopt= itr->second;
    if( covopt.find( "c" ) != string::npos or 
	covopt.find( "m" ) != string::npos ) {
      string key= itr->first;
      string covariancesstring= reader.get( "Covariances", key, "" );
      vector<string> covariancestokens= 
	INIParser::getTokens( covariancesstring );
      string str;
      size_t ntok= covariancestokens.size();
      for( size_t itok= 0; itok < ntok; itok++ ) {
	str+= covariancestokens[itok];
	if( itok < ntok-1 ) str+= " ";
      }
      m_correlations[key]= str;
    }
  }
  return;
}

// Getters for covariances:
MatrixMap AverageDataParser::getCovariances() const {
  return m_covariances;
}
MatrixMap AverageDataParser::getReducedCovariances() const {
  return m_reducedCovariances;
}
map<int,TVectorD> AverageDataParser::getSysterrorMatrix() const {
  return m_systerrmatrix;
}
// Helper to calculate covariances from errors and options u, p, f, a:
Double_t AverageDataParser::calcCovariance( const string& covopt, 
					    const TVectorD& errors, 
					    size_t ierr, size_t jerr ) const {
  Double_t cov= 0.0;
  if( covopt.find( "u" ) != string::npos ) {
    if( ierr == jerr ) cov= errors[ierr]*errors[ierr];
  }
  else if( covopt.find( "p" ) != string::npos ) {
    cov= pow( std::min( errors[ierr], errors[jerr] ), 2 );
  }
  else if( covopt.find( "f" ) != string::npos ) {
    cov= errors[ierr]*errors[jerr];
  }
  else if( covopt.find( "a" ) != string::npos ) {
    if( ierr == jerr ) cov= errors[ierr]*errors[ierr];
    else cov= - errors[ierr]*errors[jerr];
  }
  return cov;
}
// Calculate covariances:
void AverageDataParser::makeCovariances() {
  int nsysterr;
  VectorMap::const_iterator mapitr;
  for( mapitr= m_errors.begin(), nsysterr= 0; 
       mapitr != m_errors.end(); mapitr++, nsysterr++ ) {
    string errorkey= mapitr->first;
    TVectorD errors= mapitr->second;
    Int_t nerr= errors.GetNoElements();
    string covopt= m_covopts.find( errorkey )->second;
    TMatrixDSym covm( nerr );
    TMatrixDSym reducedcovm( nerr );
    if( covopt.find( "gpr" ) != string::npos ) {
      TVectorD ratios( nerr );
      TVectorD systerrs( nerr );
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
      	ratios[ierr]= errors[ierr]/m_values[ierr];
      }
      Double_t minrelerr= ratios.Min();
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	  if( ierr == jerr ) {
	    covm(ierr,ierr)= errors[ierr]*errors[ierr];
	    reducedcovm(ierr,ierr)= 
	      std::max( pow( errors[ierr], 2 ) -
			pow( minrelerr*m_values[ierr], 2 ), 0.0 );
	  }
	  else {
	    covm(ierr,jerr)= minrelerr*minrelerr*m_values[ierr]*m_values[jerr];
	  }
	}
	systerrs[ierr]= minrelerr*m_values[ierr];
      }
      m_systerrmatrix.insert( map<int,TVectorD>::value_type( nsysterr, 
							     systerrs ) );
    }
    else if( covopt.find( "gp" ) != string::npos ) {
      Double_t minerr= errors.Min();
      TVectorD systerrs( nerr );
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	  if( ierr == jerr ) {
	    covm(ierr,ierr)= errors[ierr]*errors[ierr];
	    reducedcovm(ierr,ierr)= errors[ierr]*errors[ierr]-minerr*minerr;
	  }
	  else {
	    covm(ierr,jerr)= minerr*minerr;
	  }
	}
	systerrs[ierr]= minerr;
      }
      m_systerrmatrix.insert( map<int,TVectorD>::value_type( nsysterr, 
							     systerrs ) );
    }
    else if( covopt.find( "u" ) != string::npos or
	     covopt.find( "p" ) != string::npos or
	     covopt.find( "f" ) != string::npos or
	     covopt.find( "a" ) != string::npos ) {
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	  covm(ierr,jerr)= calcCovariance( covopt, errors, ierr, jerr );
	}
      }
      if( covopt.find( "f" ) != string::npos ) {
	m_systerrmatrix.insert( map<int,TVectorD>::value_type( nsysterr, 
							       errors ) );
      }
      else {
	reducedcovm= covm;
      }
    }
    else if( covopt.find( "c" ) != string::npos ) {
      string corrstr= m_correlations[errorkey];
      vector<string> corrtokens= INIParser::getTokens( corrstr );
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	  Double_t corr= 
	    INIParser::stringToType( corrtokens.at( ierr*nerr+jerr ), 0.0 );
	  covm(ierr,jerr)= corr*errors[ierr]*errors[jerr];
	}
      }
      reducedcovm= covm;
    }
    else if( covopt.find( "m" ) != string::npos ) {
      string corrstr= m_correlations[errorkey];
      vector<string> corrtokens= INIParser::getTokens( corrstr );
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	  covm(ierr,jerr)= calcCovariance( corrtokens.at( ierr*nerr+jerr ), 
					   errors, ierr, jerr );
	}
      }
      if( corrstr.find( "f" ) != string::npos and 
	  corrstr.find( "p" ) == string::npos ) {
	m_systerrmatrix.insert( map<int,TVectorD>::value_type( nsysterr, 
							       errors ) );
      }
      else {
	reducedcovm= covm;
      }
    }
    else {
      std::cerr << "Covoption " << covopt << " not recognised" << std::endl;
    }
    m_covariances.insert( MatrixMap::value_type( errorkey, covm ) );
    m_reducedCovariances.insert( MatrixMap::value_type( errorkey, reducedcovm ) );
  }
  return;
}


string AverageDataParser::stripLeadingDigits( const string& word ) const {
  size_t iposalpha= 0;
  for( size_t ipos= 0; ipos < word.size(); ipos++ ) {
    if( isalpha( word[ipos] ) ) {
      iposalpha= ipos;
      break;
    }
  }
  return word.substr( iposalpha );
}  


void AverageDataParser::printFilename( std::ostream& ost ) const {
  ost << "AverageDataParser: input from " << m_filename << endl;
}
void AverageDataParser::printNames( std::ostream& ost ) const {
  ost << std::setw(11) << "Variables:";
  for( size_t iname= 0; iname < m_names.size(); iname++ ) {
    ost << " " << std::setw(10) << m_names[iname];
  }
  ost << endl;
}
void AverageDataParser::printUniqueGroups( std::ostream& ost ) const {
  printvectorstring( m_uniquegroups, "Groups:", ost );
}
void AverageDataParser::printGroups( std::ostream& ost ) const {
  printvectorstring( m_groups, "Groups:", ost );
}
void AverageDataParser::printvectorstring( const vector<string>& vec,
					   const string& txt,
					   std::ostream& ost ) const {
  ost << std::setw(11) << txt;
  for( size_t i= 0; i < vec.size(); i++ ) {
    ost << " " << std::setw(10) << vec[i];
  }
  ost << endl;
}
void AverageDataParser::printValues( std::ostream& ost ) const {
  ost << std::setw(11) << "Values:";
  ost.precision( 4 );
  ost.setf( std::ios::fixed, std::ios::floatfield );
  for( Int_t ival= 0; ival < m_values.GetNoElements(); ival++ ) {
    ost << " " << std::setw(10) << m_values[ival];
  }
  ost << endl;
}
void AverageDataParser::printErrors( std::ostream& ost ) const {
  ost.precision( 4 );
  ost.setf( std::ios::fixed, std::ios::floatfield );
  for( VectorMap::const_iterator mapitr= m_errors.begin();
       mapitr != m_errors.end(); mapitr++ ) {
    string key= mapitr->first;
    ost << std::setw(11) << stripLeadingDigits( key )+":";
    TVectorD errors= mapitr->second;
    for( Int_t ierr= 0; ierr < errors.GetNoElements(); ierr++ ) {
      ost << " " << std::setw(10) << errors[ierr];
    }
    ost << " " << (m_covopts.find( key ))->second << endl;
  }
}
void AverageDataParser::printTotalErrors( std::ostream& ost ) const {
  ost.precision( 4 );
  ost.setf( std::ios::fixed, std::ios::floatfield );
  ost << std::setw(11) << "total:";
  for( Int_t ierr= 0; ierr < m_totalerrors.GetNoElements(); ierr++ ) {
    ost << " " << std::setw(10) << m_totalerrors[ierr];
  }
  ost << endl;
}
void AverageDataParser::printCorrelations( std::ostream& ost ) const {
  if( m_correlations.size() > 1 ) ost << "Correlations:" << endl;
  for( StringMap::const_iterator mapitr= m_correlations.begin();
       mapitr != m_correlations.end(); mapitr++ ) {
    string key= mapitr->first;
    ost << "\n " << stripLeadingDigits( key )+":" << endl;
    string correlations= mapitr->second;
    vector<string> corrtokens= INIParser::getTokens( correlations );
    size_t nerr= m_names.size();
    for( size_t ierr= 0; ierr < nerr; ierr++ ) {
      for( size_t jerr= 0; jerr < nerr; jerr++ ) {
	ost << " " << corrtokens.at( ierr*nerr+jerr );
      }
      ost << endl;
    }
  }
}
void AverageDataParser::printCovariances( std::ostream& ost,
					  std::ios_base::fmtflags flag,
					  size_t prec ) const {
  ost << "Covariances:" << endl;
  ost.setf( flag, std::ios::floatfield );
  ost.precision( prec );
  size_t width= 5;
  if( flag == std::ios_base::fixed ) {
    width= prec+4;
  }
  else if( flag == std::ios_base::scientific ) {
    width= prec+7;
  }
  for( MatrixMap::const_iterator mapitr= m_covariances.begin();
       mapitr != m_covariances.end(); mapitr++ ) {
    string key= mapitr->first;
    ost << "\n " << stripLeadingDigits( key )+":" << endl;
    TMatrixDSym covm= mapitr->second;
    size_t nerr= covm.GetNrows();
    for( size_t ierr= 0; ierr < nerr; ierr++ ) {
      for( size_t jerr= 0; jerr < nerr; jerr++ ) {
	ost << " " << std::setw( width ) << covm( ierr, jerr );
      }
      ost << endl;
    }
  }
}
void AverageDataParser::printCorrelationMatrices( std::ostream& ost )const {
  ost << "Correlation matrices:" << endl;
  ost.setf( std::ios::fixed, std::ios::floatfield );
  ost.precision( 2 );
  for( MatrixMap::const_iterator mapitr= m_covariances.begin();
       mapitr != m_covariances.end(); mapitr++ ) {
    string key= mapitr->first;
    ost << "\n " << stripLeadingDigits( key )+":" << endl;
    TMatrixDSym covm= mapitr->second;
    size_t nerr= covm.GetNrows();
    for( size_t ierr= 0; ierr < nerr; ierr++ ) {
      for( size_t jerr= 0; jerr < nerr; jerr++ ) {
	Double_t corr= covm( ierr, jerr )/sqrt( covm( ierr, ierr )*covm( jerr, jerr ) );
	ost << " " << std::setw( 5 ) << corr;
      }
      ost << endl;
    }
  }
}



void AverageDataParser::printInputs( std::ostream& ost ) const {
  printFilename( ost );
  printNames( ost );
  if( m_uniquegroups.size() > 0 ) {
    printGroups( ost );
  }
  printValues( ost );
  printErrors( ost );
  printTotalErrors( ost );
  if( m_correlations.size() > 0 ) {
    printCorrelations( ost );
  }
  printCovariances( ost );
  printCorrelationMatrices( ost );
  return;
}
