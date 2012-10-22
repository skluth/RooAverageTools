
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "AverageDataParser.hh"

#include <stdio.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include <boost/smart_ptr.hpp>
#include <TObject.h>
#include <TMatrixD.h>

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE averagedataparsertests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;
using std::map;

// Helpers:
void checkVectorD( const vector<double>& obtained,
		   const vector<double>& expected ) {
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  for( size_t i= 0; i < obtained.size(); i++ ) {
    BOOST_CHECK_CLOSE( expected[i], obtained[i], 1.0e-4 );
  }
}
void checkMatrix( const TMatrixD& obtained,
		  const TMatrixD& expected ) {
  Int_t nobtained= obtained.GetNrows();
  Int_t nexpected= expected.GetNrows();
  BOOST_CHECK_EQUAL( nobtained, nexpected );
  for( Int_t i= 0; i < nexpected; i++ ) {
    for( Int_t j= 0; j < nexpected; j++ ) {
      BOOST_CHECK_CLOSE( obtained(i,j), expected(i,j), 1.0e-4 );
    }
  }
}

void checkStringMaps( const map<string,string>& obtained,
		      const map<string,string>& expected ) {
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  for( map<string, string >::const_iterator itr= obtained.begin(),
	 expecteditr= expected.begin(); 
       expecteditr != expected.end(); itr++, expecteditr++ ) {
    BOOST_CHECK_EQUAL( itr->first, expecteditr->first );
    BOOST_CHECK_EQUAL( itr->second, expecteditr->second );
  }
  return;
}

void checkMatrixMaps( const map<string,TMatrixD>& obtained, 
		      const map<string,TMatrixD>& expected ) {
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  for( map<string,TMatrixD>::const_iterator itr= obtained.begin(),
	 expitr= expected.begin();
       expitr != expected.end(); itr++, expitr++ ) {
    string errorkey= itr->first;
    string expectederrorkey= expitr->first;
    BOOST_CHECK_EQUAL( errorkey, expectederrorkey );
    TMatrixD covm= itr->second;
    TMatrixD expectedcovm= expitr->second;
    Int_t nerr= covm.GetNrows();
    BOOST_CHECK_EQUAL( nerr, expectedcovm.GetNrows() );
    for( Int_t ierr= 0; ierr < nerr; ierr++ ) {
      for( Int_t jerr= 0; jerr < nerr; jerr++ ) {
	BOOST_CHECK_CLOSE( covm(ierr,jerr), expectedcovm(ierr,jerr), 1.0e-4 );
      }
    }
  }
  return;
}

// Test fixture for "test.txt":
class AverageDataParserTestFixture {
public:
  AverageDataParserTestFixture() : parser( "test.txt" ) {
    BOOST_MESSAGE( "Create AverageDataParserTestFixture" );
  }
  virtual ~AverageDataParserTestFixture() {
    BOOST_MESSAGE( "Tear down AverageDataParserTestFixture" );
  }
  AverageDataParser parser;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( averagedataparsersuite, AverageDataParserTestFixture )

// Test cases:

BOOST_AUTO_TEST_CASE( testgetValues ) {
  vector<double> values= parser.getValues();
  vector<double> expectedvalues;
  expectedvalues.push_back( 171.5 );
  expectedvalues.push_back( 173.1 );
  expectedvalues.push_back( 174.5 );
  BOOST_CHECK_EQUAL( values.size(), expectedvalues.size() );
  for( size_t i= 0; i < values.size(); i++ ) {
    BOOST_CHECK_CLOSE( values[i], expectedvalues[i], 1.0e-4 );
  }
}

BOOST_AUTO_TEST_CASE( testgetNames ) {
  vector<string> names= parser.getNames();
  vector<string> expectednames;
  expectednames.push_back( "val1" );
  expectednames.push_back( "val2" );
  expectednames.push_back( "val3" );
  BOOST_CHECK_EQUAL( names.size(), expectednames.size() );
  BOOST_CHECK_EQUAL_COLLECTIONS( names.begin(), names.end(),
                                 expectednames.begin(), expectednames.end() );
}

BOOST_AUTO_TEST_CASE( testgetErrors ) {
  map<string, vector<double> > errorsmap= parser.getErrors();
  map<string, vector<double> > expectederrorsmap;
  vector<double> tmp;
  tmp.push_back( 0.3 );
  tmp.push_back( 0.33 );
  tmp.push_back( 0.4 );
  expectederrorsmap["00stat"]= tmp;
  tmp.clear();
  tmp.push_back( 1.1 );
  tmp.push_back( 1.3 );
  tmp.push_back( 1.5 );
  expectederrorsmap["01err1"]= tmp;
  tmp.clear();
  tmp.push_back( 0.9 );
  tmp.push_back( 1.5 );
  tmp.push_back( 1.9 );
  expectederrorsmap["02err2"]= tmp;
  tmp.clear();
  tmp.push_back( 2.4 );
  tmp.push_back( 3.1 );
  tmp.push_back( 3.5 );
  expectederrorsmap["03err3"]= tmp;
  tmp.clear();
  tmp.push_back( 1.4 );
  tmp.push_back( 2.9 );
  tmp.push_back( 3.3 );
  expectederrorsmap["04err4"]= tmp;
  tmp.clear();
  BOOST_CHECK_EQUAL( errorsmap.size(), expectederrorsmap.size() );
  for( map<string, vector<double> >::const_iterator itr= errorsmap.begin(),
	 expecteditr= expectederrorsmap.begin(); 
       expecteditr != expectederrorsmap.end(); itr++, expecteditr++ ) {
    BOOST_CHECK_EQUAL( itr->first, expecteditr->first );
    vector<double> errors= itr->second;
    vector<double> expectederrors= expecteditr->second;
    BOOST_CHECK_EQUAL( errors.size(), expectederrors.size() );
    for( size_t i= 0; i < errors.size(); i++ ) {
      BOOST_CHECK_CLOSE( errors[i], expectederrors[i], 1.0e-4 );
    }
  }
}

BOOST_AUTO_TEST_CASE( testgetTotalErrors ) {
  vector<double> totalerrors= parser.getTotalErrors();
  vector<double> expectedtotalerrors;
  expectedtotalerrors.push_back( 3.1352830813 );
  expectedtotalerrors.push_back( 4.6977547828 );
  expectedtotalerrors.push_back( 5.3999999999 );
  for( size_t i= 0; i < totalerrors.size(); i++ ) {
    BOOST_CHECK_CLOSE( totalerrors[i], expectedtotalerrors[i], 1.0e-4 );
  }
}

BOOST_AUTO_TEST_CASE( testgetCovoption ) {
  map<string, string> covopts= parser.getCovoption();
  map<string, string> expectedcovopts;
  expectedcovopts["00stat"]= string( "c" );
  expectedcovopts["01err1"]= string( "m" );
  expectedcovopts["02err2"]= string( "m" );
  expectedcovopts["03err3"]= string( "p" );
  expectedcovopts["04err4"]= string( "f" );
  checkStringMaps( covopts, expectedcovopts );
}

BOOST_AUTO_TEST_CASE( testgetCorrelations ) {
  map<string, string > correlations= parser.getCorrelations();
  map<string, string > expectedcorrelations;
  string tmp= "1. 0. 0. 0. 1. 0. 0. 0. 1.";
  expectedcorrelations["00stat"]= tmp;
  tmp= "p p p p p p p p p";
  expectedcorrelations["01err1"]= tmp;
  tmp= "f f f f f f f f f";
  expectedcorrelations["02err2"]= tmp;
  checkStringMaps( correlations, expectedcorrelations );
}

BOOST_AUTO_TEST_CASE( testgetCovariances ) {
  map<string,TMatrixD> covariances= parser.getCovariances();
  map<string,TMatrixD> expectedCovariances;
  double matrixStat[]= { 0.09, 0.0,  0.0,
			 0.0, 0.1089, 0.0,
			 0.0, 0.0, 0.16 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "00stat", TMatrixD( 3, 3, matrixStat ) ) );
  double matrixErr1[]= { 1.21, 1.21, 1.21,
			 1.21, 1.69, 1.69,
			 1.21, 1.69, 2.25 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "01err1", TMatrixD( 3, 3, matrixErr1 ) ) );
  double matrixErr2[]= { 0.81, 1.35, 1.71,
			 1.35, 2.25, 2.85,
			 1.71, 2.85, 3.61 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "02err2", TMatrixD( 3, 3, matrixErr2 ) ) );
  double matrixErr3[]= { 5.76, 5.76, 5.76 ,
			 5.76, 9.61, 9.61,
			 5.76, 9.61, 12.25 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "03err3", TMatrixD( 3, 3, matrixErr3 ) ) );
  double matrixErr4[]= { 1.96, 4.06, 4.62,
			 4.06, 8.41, 9.57,
			 4.62, 9.57, 10.89 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "04err4", TMatrixD( 3, 3, matrixErr4 ) ) );
  checkMatrixMaps( covariances, expectedCovariances );
}


BOOST_AUTO_TEST_CASE( testgetSysterrorMatrix ) {
  map<int,vector<double> > systerrmatrix= parser.getSysterrorMatrix();
  map<int,vector<double> > expectedSysterrmatrix;
  vector<double> systerrs2;
  systerrs2.push_back( 0.9 ); 
  systerrs2.push_back( 1.5 );
  systerrs2.push_back( 1.9 );
  expectedSysterrmatrix.insert( map<int,vector<double> >::value_type( 2, systerrs2 ) );
  vector<double> systerrs4;
  systerrs4.push_back( 1.4 ); 
  systerrs4.push_back( 2.9 );
  systerrs4.push_back( 3.3 );
  expectedSysterrmatrix.insert( map<int,vector<double> >::value_type( 4, systerrs4 ) );
  BOOST_CHECK_EQUAL( systerrmatrix.size(), expectedSysterrmatrix.size() );
  for( map<int,vector<double> >::const_iterator mapitr= systerrmatrix.begin(),
	 expmapitr= expectedSysterrmatrix.begin();
       expmapitr != expectedSysterrmatrix.end(); mapitr++, expmapitr++ ) {
    int index= mapitr->first;
    int expectedIndex= expmapitr->first;
    BOOST_CHECK_EQUAL( index, expectedIndex );
    vector<double> systerrs= mapitr->second;
    vector<double> expectedSysterrs= expmapitr->second;
    for( size_t ierr= 0; ierr < systerrs.size(); ierr++ ) {
      BOOST_CHECK_CLOSE( systerrs[ierr], expectedSysterrs[ierr], 1.0e-4 );
    }
  }
}

BOOST_AUTO_TEST_CASE( testgetReducedCovariances ) {
  map<string,TMatrixD> covariances= parser.getReducedCovariances();
  map<string,TMatrixD> expectedCovariances;
  double matrixStat[]= { 0.09, 0.0,  0.0,
			 0.0, 0.1089, 0.0,
			 0.0, 0.0, 0.16 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "00stat", TMatrixD( 3, 3, matrixStat ) ) );
  double matrixErr1[]= { 1.21, 1.21, 1.21,
			 1.21, 1.69, 1.69,
			 1.21, 1.69, 2.25 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "01err1", TMatrixD( 3, 3, matrixErr1 ) ) );
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "02err2", TMatrixD( 3, 3 ) ) );
  double matrixErr3[]= { 5.76, 5.76, 5.76 ,
			 5.76, 9.61, 9.61,
			 5.76, 9.61, 12.25 };
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "03err3", TMatrixD( 3, 3, matrixErr3 ) ) );
  expectedCovariances.insert( map<string,TMatrixD>::value_type( "04err4", TMatrixD( 3, 3 ) ) );
  checkMatrixMaps( covariances, expectedCovariances );
}



BOOST_AUTO_TEST_SUITE_END()

// Fixture for options tests:
class AverageDataParserOptionsFixture {
public:
  AverageDataParserOptionsFixture() {
    BOOST_MESSAGE( "Create AverageDataParserOptionsFixture" );
    names.push_back( "Val1" );
    names.push_back( "Val2" );
    names.push_back( "Val3" );
    values.push_back( 171.5 );
    values.push_back( 173.1 );
    values.push_back( 174.5 );
  }
  virtual ~AverageDataParserOptionsFixture() {
    BOOST_MESSAGE( "Tear down AverageDataParserOptionsFixture" );
  }
  vector<string> names;
  vector<double> values;
  vector<double> errors;
  vector<double> expectederrors;
  map<string,vector<double> > errorsmap;
  map<string,string> covopts;
};

BOOST_FIXTURE_TEST_SUITE( averagedataparseroptionssuite, 
			  AverageDataParserOptionsFixture )

BOOST_AUTO_TEST_CASE( testOptionPercentU ) {
  errors.push_back( 0.2 );
  errors.push_back( 0.22 );
  errors.push_back( 0.3 );
  errorsmap["00stat"]= errors;
  covopts["00stat"]= "%u";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,vector<double> > obtainederrorsmap= myparser.getErrors();
  vector<double> obtainederrors= obtainederrorsmap["00stat"];
  expectederrors.push_back( 0.343 );
  expectederrors.push_back( 0.38082 );
  expectederrors.push_back( 0.5235 );
  checkVectorD( obtainederrors, expectederrors );
  map<string,TMatrixD> covariancesmap= myparser.getCovariances();
  TMatrixD obtainedcovm= covariancesmap["00stat"];
  double matrixStat[]= { 0.117649, 0.0, 0.0,  0.0, 0.14502387, 
			 0.0, 0.0,  0.0,  0.27405225 };
  TMatrixD expectedcovm( 3, 3, matrixStat );
  checkMatrix( obtainedcovm, expectedcovm );
}

BOOST_AUTO_TEST_CASE( testOptionGP ) {
  errors.push_back( 0.9 );
  errors.push_back( 1.5 );
  errors.push_back( 1.9 );
  errorsmap["02errb"]= errors;
  covopts["02errb"]= "gp";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,vector<double> > obtainederrorsmap= myparser.getErrors();
  vector<double> obtainederrors= obtainederrorsmap["02errb"];
  checkVectorD( obtainederrors, errors );
  map<string,TMatrixD> covariancesmap= myparser.getCovariances();
  TMatrixD obtainedcovm= covariancesmap["02errb"];
  double matrixErrb[]= { 0.81,  0.81,  0.81,  0.81,  2.25,  0.81, 0.81,  
			 0.81,  3.61 };
  TMatrixD expectedcovm( 3, 3, matrixErrb );
  checkMatrix( obtainedcovm, expectedcovm );
  map<string,TMatrixD> reducedcovariancesmap= myparser.getReducedCovariances();
  TMatrixD obtainedreducedcovm= reducedcovariancesmap["02errb"];
  double matrixErrbRed[]= { 0.0, 0.0, 0.0, 0.0, 1.44, 0.0, 0.0, 0.0, 2.8 };
  TMatrixD expectedreducedcovm( 3, 3, matrixErrbRed );
  checkMatrix( obtainedreducedcovm, expectedreducedcovm );
  map<int,vector<double> > systerrmap= myparser.getSysterrorMatrix();
  vector<double> systerrs= systerrmap[0];
  vector<double> expectedSysterrs;
  expectedSysterrs.push_back( 0.9 );
  expectedSysterrs.push_back( 0.9 );
  expectedSysterrs.push_back( 0.9 );
  checkVectorD( systerrs, expectedSysterrs );
}

BOOST_AUTO_TEST_CASE( testOptionGPR ) {
  errors.push_back( 2.4 );
  errors.push_back( 3.1 );
  errors.push_back( 3.5 );
  errorsmap["03errc"]= errors;
  covopts["03errc"]= "gpr";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,vector<double> > obtainederrorsmap= myparser.getErrors();
  vector<double> obtainederrors= obtainederrorsmap["03errc"];
  checkVectorD( obtainederrors, errors );
  map<string,TMatrixD> covariancesmap= myparser.getCovariances();
  TMatrixD obtainedcovm= covariancesmap["03errc"];
  double matrixErrc[]= { 5.76, 5.81373761, 5.86075802,  5.81373761, 9.61, 
			 5.91543564, 5.86075802, 5.91543564, 12.25 };
  TMatrixD expectedcovm( 3, 3, matrixErrc );
  checkMatrix( obtainedcovm, expectedcovm );
  map<string,TMatrixD> reducedcovariancesmap= myparser.getReducedCovariances();
  TMatrixD obtainedreducedcovm= reducedcovariancesmap["03errc"];
  double matrixErrcRed[]= { 0.0, 0.0, 0.0, 0.0, 3.742023, 0.0, 0.0, 0.0, 6.286721 };
  TMatrixD expectedreducedcovm( 3, 3, matrixErrcRed );
  checkMatrix( obtainedreducedcovm, expectedreducedcovm );
  map<int,vector<double> > systerrmap= myparser.getSysterrorMatrix();
  vector<double> systerrs= systerrmap[0];
  vector<double> expectedSysterrs;
  expectedSysterrs.push_back( 2.4 );
  expectedSysterrs.push_back( 2.42239067 );
  expectedSysterrs.push_back( 2.4419825 );
  checkVectorD( systerrs, expectedSysterrs );
}

  
BOOST_AUTO_TEST_SUITE_END()


class AverageDataParserGroupFixture {
public:
  AverageDataParserGroupFixture() : parser( "valassi1.txt" ) {
    BOOST_MESSAGE( "Create AverageDataParserGroupFixture" );
  }
  virtual ~AverageDataParserGroupFixture() {
    BOOST_MESSAGE( "Tear down AverageDataParserGroupFixture" );
  }
  AverageDataParser parser;
};

BOOST_FIXTURE_TEST_SUITE( averagedataparsergroupsuite, 
			  AverageDataParserGroupFixture )

BOOST_AUTO_TEST_CASE( testGetGroups ) {
  vector<string> groups= parser.getGroups();
  vector<string> expectedGroups;
  expectedGroups.push_back( "a" );
  expectedGroups.push_back( "a" );
  expectedGroups.push_back( "b" );
  expectedGroups.push_back( "b" );
  BOOST_CHECK_EQUAL( groups.size(), expectedGroups.size() );
  for( size_t i= 0; i < expectedGroups.size(); i++ ) {
    BOOST_CHECK_EQUAL( groups[i], expectedGroups[i] );
  }
}

BOOST_AUTO_TEST_CASE( testGetGroupMatrix ) {
  TMatrixD groupmatrix= parser.getGroupMatrix();
  Double_t gmdata[]= { 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 };
  TMatrixD expectedgm( 2, 4, gmdata );
  Int_t nrowexp= expectedgm.GetNrows();
  Int_t ncolexp= expectedgm.GetNcols();
  Int_t nrow= groupmatrix.GetNrows();
  Int_t ncol= groupmatrix.GetNcols();
  BOOST_CHECK_EQUAL( nrow, nrowexp );
  BOOST_CHECK_EQUAL( ncol, ncolexp );
  for( Int_t irow= 0; irow < nrow; irow++ ) {
    for( Int_t icol= 0; icol < ncol; icol++ ) {
      BOOST_CHECK_CLOSE( groupmatrix(irow,icol), 
			 expectedgm(irow,icol), 1.0e-4 );
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
