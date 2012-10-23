

#include "AverageDataParser.hh"
#include "INIReader.hh"
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <iostream>
#include <math.h>

// Commonly used identifiers
using std::string;
using std::vector;
using std::map;
using namespace INIParser;

// Ctors:
AverageDataParser::AverageDataParser( const string& fname ) {
  INIParser::INIReader reader( fname );
  makeNames( reader );
  makeValues( reader );
  makeGroups( reader );
  makeErrorsAndOptions( reader );
  makeCorrelations( reader );
  checkRelativeErrors();
  makeCovariances();
}

AverageDataParser::AverageDataParser( const vector<string>& names,
				      const TVectorD& values,
				      const map<string,TVectorD>& errors,
				      const map<string,string>& covopts,
				      map<string,string> correlations,
				      vector<string> groups ) :
  m_names( names ), m_values( values ), m_errors( errors ),
  m_covopts( covopts ), m_correlations( correlations ), m_groups( groups ) {
  checkRelativeErrors();
  makeCovariances();
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
  vector<string> uniquegroups( m_groups );
  std::sort( uniquegroups.begin(), uniquegroups.end() );
  vector<string>::iterator uniqueend= std::unique( uniquegroups.begin(), 
						   uniquegroups.end() );
  uniquegroups.resize( uniqueend - uniquegroups.begin() );
  m_groupmatrix.ResizeTo( m_groups.size(), uniquegroups.size() );
  for( size_t igroup= 0; igroup < m_groups.size(); igroup++ ) {
    vector<string>::iterator itr= std::find( uniquegroups.begin(), 
					     uniquegroups.end(), 
					     m_groups[igroup] );
    if( itr != uniquegroups.end() ) {
      size_t groupindex= itr - uniquegroups.begin();
      m_groupmatrix( igroup, groupindex )= 1.0;
    }
  }
  return;
}


// Return map of error values for each error category:
// map<string, vector<double> > AverageDataParser::getErrors2() const {
//   return m_errors2;
// }
map<string,TVectorD> AverageDataParser::getErrors() const {
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
  for( map<string,TVectorD>::iterator mapitr= m_errors.begin();
       mapitr != m_errors.end(); mapitr++ ) {
    string errorkey= mapitr->first;
    if( m_covopts[errorkey].find( "%" ) != string::npos ) {
      //      vector<double>& errors= mapitr->second;
      TVectorD& errors= mapitr->second;
      for( Int_t ierr= 0; ierr != errors.GetNoElements(); ierr++ ) {
        errors[ierr]*= m_values[ierr] / 100.0;
      }
    }
  }
}
void AverageDataParser::makeErrorsAndOptions( const INIParser::INIReader& reader ) {
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
    vector<double> elements2;
    size_t ntok= elementtokens.size();
    TVectorD elements( ntok );
    for( size_t itok= 0; itok != ntok; itok++ ) {
      double element= INIParser::stringToType( elementtokens[itok], 0.0 );      
      elements2.push_back( element );
      elements[itok]= element;
    }
    // m_errors2[key]= elements2;
    m_errors.insert( map<string,TVectorD>::value_type( key, elements ) );
  }
  return;
}

// Return total errors for each variable:
TVectorD AverageDataParser::getTotalErrors() const {
  TVectorD totalerrors( m_values.GetNoElements() );
  for( map<string,TVectorD>::const_iterator itr= m_errors.begin(); 
       itr != m_errors.end(); itr++ ) {
    const TVectorD& errors= itr->second;
    for( Int_t ierr= 0; ierr < errors.GetNoElements(); ierr++ ) {
      totalerrors[ierr]+= errors[ierr]*errors[ierr];
    }
  }
  for( Int_t ierr= 0; ierr < totalerrors.GetNoElements(); ierr++ ) {
    totalerrors[ierr]= sqrt( totalerrors[ierr] );
  }
  return totalerrors;
}

// Read detailed correlation information as a string from extra section
// "Covariances" if indicated by option in "Data" section.
// On failure the map containes empty strings.
map<string,string> AverageDataParser::getCorrelations() const {
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
map<string, TMatrixD> AverageDataParser::getCovariances() const {
  return m_covariances;
}
map<string, TMatrixD> AverageDataParser::getReducedCovariances() const {
  return m_reducedCovariances;
}
// map<int,vector<double> > AverageDataParser::getSysterrorMatrix2() const {
//   return m_systerrmatrix2;
// }
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
  map<string,TVectorD>::const_iterator mapitr;
  for( mapitr= m_errors.begin(), nsysterr= 0; 
       mapitr != m_errors.end(); mapitr++, nsysterr++ ) {
    string errorkey= mapitr->first;
    TVectorD errors= mapitr->second;
    Int_t nerr= errors.GetNoElements();
    string covopt= m_covopts.find( errorkey )->second;
    TMatrixD covm( nerr, nerr );
    TMatrixD reducedcovm( nerr, nerr );
    if( covopt.find( "gpr" ) != string::npos ) {
      vector<double> ratios;
      //vector<double> systerrs2;
      TVectorD systerrs( nerr );
      for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
	ratios.push_back( errors[ierr]/m_values[ierr] );
      }
      Double_t minrelerr= *std::min_element( ratios.begin(), ratios.end() );
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
	//systerrs2.push_back( minrelerr*m_values[ierr] );
	systerrs[ierr]= minrelerr*m_values[ierr];
      }
      //      m_systerrmatrix2[nsysterr]= systerrs2;
      m_systerrmatrix.insert( map<int,TVectorD>::value_type( nsysterr, 
							     systerrs ) );
    }
    else if( covopt.find( "gp" ) != string::npos ) {
      Double_t minerr= errors.Min();
      //vector<double> systerrs2;
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
	//systerrs2.push_back( minerr );
	systerrs[ierr]= minerr;
      }
      //m_systerrmatrix2[nsysterr]= systerrs2;
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
	//m_systerrmatrix[nsysterr]= errors2;
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
	  Double_t corr= INIParser::stringToType( corrtokens[ierr*nerr+jerr], 
						  0.0 );
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
	  covm(ierr,jerr)= calcCovariance( corrtokens[ierr*nerr+jerr],
					   errors, ierr, jerr );
	}
      }
      if( corrstr.find( "f" ) != string::npos and 
	  corrstr.find( "p" ) == string::npos ) {
	//m_systerrmatrix[nsysterr]= errors;
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
    m_covariances.insert( map<string, TMatrixD>::value_type( errorkey, covm ) );
    m_reducedCovariances.insert( map<string, TMatrixD>::value_type( errorkey, reducedcovm ) );
  }
  return;
}
  

