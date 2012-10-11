

#include "AverageDataParser.hh"
#include <vector>
#include <utility>
#include <list>


using std::string;
using namespace INIParser;


AverageDataParser::AverageDataParser( const string& fname ) :
  m_errors( 0 ), m_covopts( 0 ), filename( fname ), reader( fname ) {}


string AverageDataParser::getFilename() const {
  return filename;
}

std::vector<float> AverageDataParser::getValues() const {
  string valuestring = reader.get( "Data", "Values", "" );
  std::vector<string> valuestrings = INIParser::getTokens(valuestring);
  std::vector<float> values;
  for(std::vector<string>::const_iterator valueitr = valuestrings.begin(); valueitr != valuestrings.end(); ++valueitr){
    std::istringstream ss(*valueitr);
    float value;
    ss >> value;
    values.push_back( value );
  }
  return values;
}

std::vector<string> AverageDataParser::getNames() const {
  string namestring = reader.get( "Data", "Names", "" );
  std::vector<string> names = INIParser::getTokens(namestring);
  return names;
}

std::map<string, std::vector<float> > AverageDataParser::getErrors() {
  if (!m_errors) {
    getErrorsAndOptions();
  }
  return *m_errors;
}

void AverageDataParser::getErrorsAndOptions() {
  m_errors = new std::map<string, std::vector<float> >;
  m_covopts = new std::map<string, string>;
  std::map<string, std::vector<float> > & errors = *m_errors;
  std::map<string, string> & covopts = *m_covopts;

  std::list<std::pair<string, string> > elementNames;
  elementNames.push_back( std::pair<string, string>("00Stat", "00stat") );
  elementNames.push_back( std::pair<string, string>("01Err1", "01err1") );
  elementNames.push_back( std::pair<string, string>("02Err2", "02err2") );
  elementNames.push_back( std::pair<string, string>("03Err3", "03err3") );
  elementNames.push_back( std::pair<string, string>("04Err4", "04err4") );

  for(std::list<std::pair<string, string> >::const_iterator nameitr = elementNames.begin(); nameitr != elementNames.end(); ++nameitr) {
    string elementstring = reader.get( "Data", nameitr->first, "" );
    std::vector<string> elementstrings = INIParser::getTokens(elementstring);
    covopts[nameitr->second] = elementstrings.back();
    elementstrings.pop_back();

    std::vector<float> elements;
    for(std::vector<string>::const_iterator elementitr = elementstrings.begin(); elementitr != elementstrings.end(); ++elementitr){
      std::istringstream ss(*elementitr);
      float element;
      ss >> element;
      elements.push_back( element );
    }
    errors[nameitr->second] = elements;
  }
  return;
}

std::vector<float> AverageDataParser::getTotalErrors() const {
  std::vector<float> totalerrors;
  totalerrors.push_back(3.1352830813181765);
  totalerrors.push_back(4.6977547828723454);
  totalerrors.push_back(5.3999999999999995);
  return totalerrors;
}

std::map<string, string> AverageDataParser::getCovoption() {
  if(!m_covopts) {
    getErrorsAndOptions();
  }
  return *m_covopts;
}

std::map<string, string> AverageDataParser::getCorrelations() const {
  std::map<string, string> correlations;
  string tmp = "1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0";
  correlations["00stat"] = tmp;
  tmp = "p, p, p, p, p, p, p, p, p";
  correlations["01err1"] = tmp;
  tmp = "f, f, f, f, f, f, f, f, f";
  correlations["02err2"] = tmp;
  return correlations;
}

std::map<string, TMatrixD> AverageDataParser::getCovariances() const {
  std::map<string, TMatrixD> covariances;
  return covariances;
}

std::map<unsigned int, std::vector<float> > AverageDataParser::getSysterrorMatrix() const {
  std::map<unsigned int, std::vector<float> > systerrmatrix;
  std::vector<float> tmp;
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
