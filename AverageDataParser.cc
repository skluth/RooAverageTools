

#include "AverageDataParser.hh"
#include <vector>


using std::string;
using namespace INIParser;


AverageDataParser::AverageDataParser( const string& fname ) :
  filename( fname ), reader( fname ) {}


string AverageDataParser::getFilename() const {
  return filename;
}

std::vector<float> AverageDataParser::getValues() const {
  std::vector<float> values;
  values.push_back(171.5);
  values.push_back(173.1);
  values.push_back(174.5);
  return values;
}

std::vector<string> AverageDataParser::getNames() const {
  std::vector<string> names;
  names.push_back("Val1");
  names.push_back("Val2");
  names.push_back("Val3");
  return names;
}

std::map<string, std::vector<float> > AverageDataParser::getErrors() const {
  std::map<string, std::vector<float> > errors;
  std::vector<float> tmp;
  tmp.push_back(0.3);
  tmp.push_back(0.33);
  tmp.push_back(0.4);
  errors["00stat"] = tmp;
  tmp.clear();
  tmp.push_back(1.1);
  tmp.push_back(1.3);
  tmp.push_back(1.5);
  errors["01err1"] = tmp;
  tmp.clear();
  tmp.push_back(0.9);
  tmp.push_back(1.5);
  tmp.push_back(1.9);
  errors["02err2"] = tmp;
  tmp.clear();
  tmp.push_back(2.4);
  tmp.push_back(3.1);
  tmp.push_back(3.5);
  errors["03err3"] = tmp;
  tmp.clear();
  tmp.push_back(1.4);
  tmp.push_back(2.9);
  tmp.push_back(3.3);
  errors["04err4"] = tmp;
  tmp.clear();
  return errors;
}

std::vector<float> AverageDataParser::getTotalErrors() const {
  std::vector<float> totalerrors;
  totalerrors.push_back(3.1352830813181765);
  totalerrors.push_back(4.6977547828723454);
  totalerrors.push_back(5.3999999999999995);
  return totalerrors;
}

std::map<string, string> AverageDataParser::getCovoption() const {
  std::map<string, string> covopts;
  covopts["00stat"] = string("c"); 
  covopts["01err1"] = string("m");
  covopts["02err2"] = string("m");
  covopts["03err3"] = string("p");
  covopts["04err4"] = string("f");
  return covopts;
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
