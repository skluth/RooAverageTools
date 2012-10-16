

#include "AverageDataParser.hh"
#include <vector>
#include <utility>
#include <algorithm>
#include <list>
#include <math.h>


using std::string;
using std::vector;
using std::map;
using namespace INIParser;


AverageDataParser::AverageDataParser( const string& fname ) :
  filename( fname ), reader( fname ) {
  getErrorsAndOptions();
}

string AverageDataParser::getFilename() const {
  return filename;
}

vector<float> AverageDataParser::getValues() const {
  string valuestring= reader.get( "Data", "Values", "" );
  vector<string> valuestrings= INIParser::getTokens( valuestring );
  vector<float> values;
  for( vector<string>::const_iterator valueitr= valuestrings.begin(); 
       valueitr != valuestrings.end(); ++valueitr ) {
    std::istringstream iss( *valueitr );
    float value;
    iss >> value;
    values.push_back( value );
  }
  return values;
}

vector<string> AverageDataParser::getNames() const {
  string namestring= reader.get( "Data", "Names", "" );
  vector<string> names= INIParser::getTokens( namestring );
  return names;
}

map<string, vector<float> > AverageDataParser::getErrors() {
  return m_errors;
}

map<string, string> AverageDataParser::getCovoption() {
  return m_covopts;
}

void AverageDataParser::getErrorsAndOptions() {
  vector<string> keys= reader.getNames( "Data" );
  vector<string> removekeys;
  removekeys.push_back( "Names" );
  removekeys.push_back( "Values" );
  for( size_t ikey= 0; ikey != removekeys.size(); ikey++ ) {
    vector<string>::iterator pos= std::find( keys.begin(),
					     keys.end(),
					     removekeys[ikey] );
    if( pos != keys.end() ) keys.erase( pos );
  }
  for( size_t ikey= 0; ikey != keys.size(); ikey++ ) {
    string key= keys[ikey];
    string elementstring= reader.get( "Data", key, "" );
    vector<string> elementtokens= INIParser::getTokens( elementstring );
    string covopt = elementtokens.back();
    m_covopts[key] = covopt;

    elementtokens.pop_back();
    vector<float> elements;
    for( size_t itok= 0; itok != elementtokens.size(); itok++ ) {
      float element= INIParser::stringToType( elementtokens[itok], 0.0 );      
      elements.push_back( element );
    }
    if(covopt.find( "%" ) != string::npos) {
      vector<float> values = getValues();
      for(size_t index = 0; index != elements.size(); index++ ) {
        elements[index] *= values[index] / 100.0;
      }
    }
    m_errors[key]= elements;
  }
  return;
}

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

map<string, TMatrixD> AverageDataParser::getCovariances() const {
  map<string, TMatrixD> covariances;
  double matrixData1 [] = {  0.117649, 0.0, 0.0,  0.0, 0.14502387, 0.0, 0.0,  0.0,  0.27405225 };
  TMatrixD myMatrix1(3,3,matrixData1);
  covariances["00stat"] = myMatrix1;
  double matrixData2 [] = { 3.55888225, 3.55888225, 3.55888225, 3.55888225,  5.06385009,  3.55888225, 3.55888225,  3.55888225,  6.85130625};
  TMatrixD myMatrix2(3,3,matrixData2);
  covariances["01erra"] = myMatrix2;
  double matrixData3 [] = {  0.81,  0.81,  0.81,  0.81,  2.25,  0.81, 0.81,  0.81,  3.61};
  TMatrixD myMatrix3(3,3,matrixData3);
  covariances["02errb"] = myMatrix3;
  double matrixData4 [] = { 5.76, 5.81373761, 5.86075802,  5.81373761, 9.61, 5.91543564, 5.86075802, 5.91543564, 12.25 };
  TMatrixD myMatrix4(3,3,matrixData4);
  covariances["03errc"] = myMatrix4;
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
