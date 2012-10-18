

#include "AverageDataParser.hh"
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <math.h>

// Commonly used identifiers
using std::string;
using std::vector;
using std::map;
using namespace INIParser;

// Ctor:
AverageDataParser::AverageDataParser( const string& fname ) :
  filename( fname ), reader( fname ) {
  getErrorsAndOptions();
}

// Return filename:
string AverageDataParser::getFilename() const {
  return filename;
}

// Return data values:
vector<float> AverageDataParser::getValues() const {
  string valuestring= reader.get( "Data", "values", "" );
  vector<string> valuetokens= INIParser::getTokens( valuestring );
  vector<float> values;
  for( size_t itok= 0; itok != valuetokens.size(); itok++ ) {
    float value= INIParser::stringToType( valuetokens[itok], 0.0 );
    values.push_back( value );
  }
  return values;
}

// Return all keys in a section:
vector<string> AverageDataParser::getNames() const {
  string namestring= reader.get( "Data", "names", "" );
  vector<string> names= INIParser::getTokens( namestring );
  return names;
}

// Return map of errors:
map<string, vector<float> > AverageDataParser::getErrors() {
  return m_errors;
}

// Return map of covariance options
map<string, string> AverageDataParser::getCovoption() {
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
void AverageDataParser::getErrorsAndOptions() {
  vector<string> keys= reader.getNames( "Data" );
  vector<string> removekeys;
  removekeys.push_back( "names" );
  removekeys.push_back( "values" );
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
    vector<float> elements;
    for( size_t itok= 0; itok != elementtokens.size(); itok++ ) {
      float element= INIParser::stringToType( elementtokens[itok], 0.0 );      
      elements.push_back( element );
    }
    if( covopt.find( "%" ) != string::npos ) {
      vector<float> values= getValues();
      for( size_t ival= 0; ival != elements.size(); ival++ ) {
        elements[ival]*= values[ival] / 100.0;
      }
    }
    m_errors[key]= elements;
  }
  return;
}

// Return total errors for each variable:
vector<float> AverageDataParser::getTotalErrors() {
  vector<float> totalerrors( getValues().size(), 0.0 );
  for( map<string, vector<float> >::const_iterator typeitr= m_errors.begin(); 
       typeitr != m_errors.end(); ++typeitr ) {
    vector<float> errors= typeitr->second;
    for( size_t ierr= 0; ierr < errors.size(); ierr++ ) {
      totalerrors[ierr]+= errors[ierr]*errors[ierr];
    }
  }
  for( size_t ierr= 0; ierr < totalerrors.size(); ierr++ ) {
    totalerrors[ierr]= sqrt( totalerrors[ierr] );
  }
  return totalerrors;
}

// Read detailed correlation information as a string from extra section
// "Covariances" if indicated by option in "Data" section.
// On failure the map containes empty strings.
map<string,string> AverageDataParser::getCorrelations() const {
  map<string,string> covariancesmap;
  for( map<string, string>::const_iterator itr= m_covopts.begin();
       itr != m_covopts.end(); itr++ ) {
    string key= itr->first;
    string covopt= itr->second;
    if( covopt.find( "c" ) != string::npos or 
	covopt.find( "m" ) != string::npos ) {
      string covariancesstring= reader.get( "Covariances", key, "" );
      vector<string> covariancestokens= 
	INIParser::getTokens( covariancesstring );
      string str;
      size_t ntok= covariancestokens.size();
      for( size_t itok= 0; itok < ntok; itok++ ) {
	str+= covariancestokens[itok];
	if( itok < ntok-1 ) str+= " ";
      }
      covariancesmap[key]= str;
    }
  }
  return covariancesmap;
}

Double_t AverageDataParser::calcCovariance( string covopt, 
					    vector<float> errors, 
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

map<string, TMatrixD> AverageDataParser::makeCovariances() const {
  map<string, TMatrixD> covariances;
  for( map<string,vector<float> >::const_iterator mapitr= m_errors.begin();
       mapitr != m_errors.end(); mapitr++ ) {
    string errorkey= mapitr->first;
    vector<float> errors= mapitr->second;
    size_t nerr= errors.size();
    string covopt= m_covopts.find( errorkey )->second;
    TMatrixD covm( nerr, nerr );
    if( covopt.find( "gpr" ) != string::npos ) {
    }
    else if( covopt.find( "gp" ) != string::npos ) {
      Double_t minerr= *std::min_element( errors.begin(), errors.end() );
      for( size_t ierr= 0; ierr < nerr; ierr++ ) {
	for( size_t jerr= 0; jerr < nerr; jerr++ ) {
	  if( ierr == jerr ) covm(ierr,ierr)= errors[ierr]*errors[ierr];
	  else covm(ierr,jerr)= minerr*minerr;
	}
      }
      covariances.insert( map<string, TMatrixD>::value_type( errorkey, covm ) );
    }
    else if( covopt.find( "u" ) != string::npos or
	     covopt.find( "p" ) != string::npos or
	     covopt.find( "f" ) != string::npos or
	     covopt.find( "a" ) != string::npos ) {
      for( size_t ierr= 0; ierr < nerr; ierr++ ) {
	for( size_t jerr= 0; jerr < nerr; jerr++ ) {
	  covm(ierr,jerr)= calcCovariance( covopt, errors, ierr, jerr );
	}
      }
      covariances.insert( map<string, TMatrixD>::value_type( errorkey, covm ) );
    }
  }
  return covariances;
}
  
// Calculate covariances from covariance options or detailed
// correlation options in map:
map<string, TMatrixD> AverageDataParser::getCovariances() const {
  map<string, TMatrixD> covariances;
  double matrixData1 [] = {  0.117649, 0.0, 0.0,  0.0, 0.14502387, 0.0, 0.0,  0.0,  0.27405225 };
  covariances.insert(std::map<string,TMatrixD>::value_type("00stat", TMatrixD(3,3,matrixData1)));
  double matrixData2 [] = { 3.55888225, 3.55888225, 3.55888225, 3.55888225,  5.06385009,  3.55888225, 3.55888225,  3.55888225,  6.85130625};
  covariances.insert(std::map<string,TMatrixD>::value_type("01erra", TMatrixD(3,3,matrixData2)));
  double matrixData3 [] = {  0.81,  0.81,  0.81,  0.81,  2.25,  0.81, 0.81,  0.81,  3.61};
  covariances.insert(std::map<string,TMatrixD>::value_type("02errb", TMatrixD(3,3,matrixData3)));
  double matrixData4 [] = { 5.76, 5.81373761, 5.86075802,  5.81373761, 9.61, 5.91543564, 5.86075802, 5.91543564, 12.25 };
  covariances.insert(std::map<string,TMatrixD>::value_type("03errc", TMatrixD(3,3,matrixData4)));
  return covariances;
}

map<unsigned int, vector<float> > 
AverageDataParser::getSysterrorMatrix() const {
  map<unsigned int, vector<float> > systerrmatrix;
  vector<float> tmp;
  tmp.push_back(1.8865);
  tmp.push_back(1.8865);
  tmp.push_back(1.8865);
  systerrmatrix[1] = tmp;
  tmp.clear();
  tmp.push_back(0.9);
  tmp.push_back(0.9);
  tmp.push_back(0.9);
  systerrmatrix[2] = tmp;
  tmp.clear();
  tmp.push_back(2.4);
  tmp.push_back(2.42239067);
  tmp.push_back(2.4419825);
  systerrmatrix[3] = tmp;
  return systerrmatrix;
}
