
// Unit tests for AverageTools
// S. Kluth, 9/2012

#include "AverageDataParser.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMatrixDSym.h>

// BOOST test stuff:
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE averagedataparsertests
#include <boost/test/unit_test.hpp>

// Namespaces:
using std::string;
using std::vector;
using std::map;

// Helpers:
void checkVector( const TVectorD& obtained,
		  const TVectorD& expected ) {
  BOOST_CHECK_EQUAL( obtained.GetNoElements(), expected.GetNoElements() );
  for( Int_t i= 0; i < expected.GetNoElements(); i++ ) {
    BOOST_CHECK_CLOSE(  obtained[i], expected[i], 1.0e-4 );
  }
}
void checkMatrix( const TMatrixD& obtained,
		  const TMatrixD& expected ) {
  Int_t nrowo= obtained.GetNrows();
  Int_t nrowe= expected.GetNrows();
  Int_t ncolo= obtained.GetNcols();
  Int_t ncole= expected.GetNcols();
  BOOST_CHECK_EQUAL( nrowo, nrowe );
  BOOST_CHECK_EQUAL( ncolo, ncole );
  for( Int_t irow= 0; irow < nrowe; irow++ ) {
    for( Int_t icol= 0; icol < ncole; icol++ ) {
      BOOST_CHECK_CLOSE( obtained(irow,icol), expected(irow,icol), 1.0e-4 );
    }
  }
}
void checkVectorMap( const map<string,TVectorD>& obtained, 
		     const map<string,TVectorD>& expected ) {
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  for( map<string,TVectorD>::const_iterator itr= obtained.begin(),
	 expitr= expected.begin();
       expitr != expected.end(); itr++, expitr++ ) {
    BOOST_CHECK_EQUAL( itr->first, expitr->first );
    checkVector( itr->second, expitr->second );
  }
  return;
}
void checkMatrixMap( const map<string,TMatrixDSym>& obtained, 
		     const map<string,TMatrixDSym>& expected ) {
  BOOST_CHECK_EQUAL( obtained.size(), expected.size() );
  for( map<string,TMatrixDSym>::const_iterator itr= obtained.begin(),
	 expitr= expected.begin();
       expitr != expected.end(); itr++, expitr++ ) {
    BOOST_CHECK_EQUAL( itr->first, expitr->first );
    checkMatrix( itr->second, expitr->second );
  }
  return;
}
void checkStringMap( const map<string,string>& obtained,
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


// Test fixture for "test.txt":
class AverageDataParserTestFixture {
public:
  AverageDataParserTestFixture() : parser( "test.txt" ) {}
  AverageDataParser parser;
  std::ostringstream osst;
};

// Declare test suite name and fixture class to BOOST:
BOOST_FIXTURE_TEST_SUITE( averagedataparsersuite, AverageDataParserTestFixture )

// Test cases:

BOOST_AUTO_TEST_CASE( testgetNames ) {
  BOOST_MESSAGE( "testgetNames" );
  vector<string> names= parser.getNames();
  vector<string> expectednames;
  expectednames.push_back( "val1" );
  expectednames.push_back( "val2" );
  expectednames.push_back( "val3" );
  BOOST_CHECK_EQUAL( names.size(), expectednames.size() );
  BOOST_CHECK_EQUAL_COLLECTIONS( names.begin(), names.end(),
                                 expectednames.begin(), expectednames.end() );
}

BOOST_AUTO_TEST_CASE( testgetValues ) {
  BOOST_MESSAGE( "testgetValues" );
  TVectorD values= parser.getValues();
  Double_t data[]= { 171.5, 173.1, 174.5 };
  TVectorD expectedValues( 3, data );
  checkVector( values, expectedValues );
}

BOOST_AUTO_TEST_CASE( testgetErrors ) {
  BOOST_MESSAGE( "testgetErrors" );
  map<string,TVectorD> errorsmap= parser.getErrors();
  map<string,TVectorD> expectederrorsmap;
  Double_t datastat[]= { 0.3, 0.33, 0.4 };
  expectederrorsmap.insert( map<string,TVectorD>::value_type( "00stat", TVectorD( 3, datastat ) ) );
  Double_t dataerr1[]= { 1.1, 1.3, 1.5 };
  expectederrorsmap.insert( map<string,TVectorD>::value_type( "01err1", TVectorD( 3, dataerr1 ) ) );
  Double_t dataerr2[]= { 0.9, 1.5, 1.9 };
  expectederrorsmap.insert( map<string,TVectorD>::value_type( "02err2", TVectorD( 3, dataerr2 ) ) );
  Double_t dataerr3[]= { 2.4, 3.1, 3.5 };
  expectederrorsmap.insert( map<string,TVectorD>::value_type( "03err3", TVectorD( 3, dataerr3 ) ) );
  Double_t dataerr4[]= { 1.4, 2.9, 3.3 };
  expectederrorsmap.insert( map<string,TVectorD>::value_type( "04err4", TVectorD( 3, dataerr4 ) ) );
  checkVectorMap( errorsmap, expectederrorsmap );
}

BOOST_AUTO_TEST_CASE( testgetTotalErrors ) {
  BOOST_MESSAGE( "testgetTotalErrors" );
  TVectorD totalerrors= parser.getTotalErrors();
  Double_t data[]= { 3.1352830813, 4.6977547828, 5.3999999999 };
  TVectorD expectedtotalerrors( 3, data );
  checkVector( totalerrors, expectedtotalerrors );
}

BOOST_AUTO_TEST_CASE( testgetCovoption ) {
  BOOST_MESSAGE( "testgetCovoption" );
  map<string, string> covopts= parser.getCovoption();
  map<string, string> expectedcovopts;
  expectedcovopts["00stat"]= string( "c" );
  expectedcovopts["01err1"]= string( "m" );
  expectedcovopts["02err2"]= string( "m" );
  expectedcovopts["03err3"]= string( "p" );
  expectedcovopts["04err4"]= string( "f" );
  checkStringMap( covopts, expectedcovopts );
}

BOOST_AUTO_TEST_CASE( testgetCorrelations ) {
  BOOST_MESSAGE( "testgetCorrelations" );
  map<string, string > correlations= parser.getCorrelations();
  map<string, string > expectedcorrelations;
  string tmp= "1. 0. 0. 0. 1. 0. 0. 0. 1.";
  expectedcorrelations["00stat"]= tmp;
  tmp= "p p p p p p p p p";
  expectedcorrelations["01err1"]= tmp;
  tmp= "f f f f f f f f f";
  expectedcorrelations["02err2"]= tmp;
  checkStringMap( correlations, expectedcorrelations );
}

BOOST_AUTO_TEST_CASE( testgetCovariances ) {
  BOOST_MESSAGE( "testgetCovariances" );
  map<string,TMatrixDSym> covariances= parser.getCovariances();
  map<string,TMatrixDSym> expectedCovariances;
  double matrixStat[]= { 0.09, 0.0,  0.0,
			 0.0, 0.1089, 0.0,
			 0.0, 0.0, 0.16 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "00stat", TMatrixDSym( 3, matrixStat ) ) );
  double matrixErr1[]= { 1.21, 1.21, 1.21,
			 1.21, 1.69, 1.69,
			 1.21, 1.69, 2.25 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "01err1", TMatrixDSym( 3, matrixErr1 ) ) );
  double matrixErr2[]= { 0.81, 1.35, 1.71,
			 1.35, 2.25, 2.85,
			 1.71, 2.85, 3.61 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "02err2", TMatrixDSym( 3, matrixErr2 ) ) );
  double matrixErr3[]= { 5.76, 5.76, 5.76 ,
			 5.76, 9.61, 9.61,
			 5.76, 9.61, 12.25 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "03err3", TMatrixDSym( 3, matrixErr3 ) ) );
  double matrixErr4[]= { 1.96, 4.06, 4.62,
			 4.06, 8.41, 9.57,
			 4.62, 9.57, 10.89 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "04err4", TMatrixDSym( 3, matrixErr4 ) ) );
  checkMatrixMap( covariances, expectedCovariances );
}

BOOST_AUTO_TEST_CASE( testgetSysterrorMatrix ) {
  BOOST_MESSAGE( "testgetSysterrorMatrix" );
  map<int,TVectorD> systerrmatrix= parser.getSysterrorMatrix();
  map<int,TVectorD> expectedSysterrmatrix;
  Double_t systerrs2[]= { 0.9, 1.5, 1.9 };
  expectedSysterrmatrix.insert( map<int,TVectorD>::value_type( 2, TVectorD( 3, systerrs2 ) ) );
  Double_t systerrs4[]= { 1.4, 2.9, 3.3 };
  expectedSysterrmatrix.insert( map<int,TVectorD>::value_type( 4, TVectorD( 3, systerrs4 ) ));
  BOOST_CHECK_EQUAL( systerrmatrix.size(), expectedSysterrmatrix.size() );
  for( map<int,TVectorD>::const_iterator mapitr= systerrmatrix.begin(),
	 expmapitr= expectedSysterrmatrix.begin();
       expmapitr != expectedSysterrmatrix.end(); mapitr++, expmapitr++ ) {
    BOOST_CHECK_EQUAL( mapitr->first, expmapitr->first );
    checkVector( mapitr->second, expmapitr->second );
  }
}

BOOST_AUTO_TEST_CASE( testgetReducedCovariances ) {
  BOOST_MESSAGE( "testgetReducedCovariances" );
  map<string,TMatrixDSym> covariances= parser.getReducedCovariances();
  map<string,TMatrixDSym> expectedCovariances;
  double matrixStat[]= { 0.09, 0.0,  0.0,
			 0.0, 0.1089, 0.0,
			 0.0, 0.0, 0.16 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "00stat", TMatrixDSym( 3, matrixStat ) ) );
  double matrixErr1[]= { 1.21, 1.21, 1.21,
			 1.21, 1.69, 1.69,
			 1.21, 1.69, 2.25 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "01err1", TMatrixDSym( 3, matrixErr1 ) ) );
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "02err2", TMatrixDSym( 3 ) ) );
  double matrixErr3[]= { 5.76, 5.76, 5.76 ,
			 5.76, 9.61, 9.61,
			 5.76, 9.61, 12.25 };
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "03err3", TMatrixDSym( 3, matrixErr3 ) ) );
  expectedCovariances.insert( map<string,TMatrixDSym>::value_type( "04err4", TMatrixDSym( 3 ) ) );
  checkMatrixMap( covariances, expectedCovariances );
}

BOOST_AUTO_TEST_CASE( testgetTotalCovariances ) {
  BOOST_MESSAGE( "testgetTotalCovariances" );
  TMatrixDSym totalcov= parser.getTotalCovariances();
  Double_t data[]= { 9.83, 12.38, 13.3,
		     12.38, 22.0689, 23.72,
		     13.3 , 23.72, 29.16 };
  TMatrixDSym expectedtc( 3, data );
  checkMatrix( totalcov, expectedtc );
}

BOOST_AUTO_TEST_CASE( testgetTotalReducedCovariances ) {
  BOOST_MESSAGE( "testgetTotalReducedCovariances" );
  TMatrixDSym totalredcov= parser.getTotalReducedCovariances();
  Double_t data[]= { 7.06, 6.97, 6.97,
		     6.97, 11.4089, 11.3,
		     6.97, 11.3, 14.66 };
  TMatrixDSym expectedtrc( 3, data );
  checkMatrix( totalredcov, expectedtrc );
}

BOOST_AUTO_TEST_CASE( testprintFilename ) {
  BOOST_MESSAGE( "testprintFilename" );
  parser.printFilename( osst );
  string obtained= osst.str();
  string expected= "AverageDataParser: input from test.txt\n";  
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintNames ) {
  BOOST_MESSAGE( "testprintNames" );
  parser.printNames( osst );
  string obtained= osst.str();
  string expected= " Variables:       val1       val2       val3\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintValues ) {
  BOOST_MESSAGE( "testprintValues" );
  parser.printValues( osst );
  string obtained= osst.str();
  string expected= "    Values:   171.5000   173.1000   174.5000\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintErrors ) {
  BOOST_MESSAGE( "testprintErrors" );
  parser.printErrors( osst );
  string obtained= osst.str();
  string expected= 
    "      stat:     0.3000     0.3300     0.4000 c\n"
    "      err1:     1.1000     1.3000     1.5000 m\n"
    "      err2:     0.9000     1.5000     1.9000 m\n"
    "      err3:     2.4000     3.1000     3.5000 p\n"
    "      err4:     1.4000     2.9000     3.3000 f\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintTotalErrors ) {
  BOOST_MESSAGE( "testprintTotalErrors" );
  parser.printTotalErrors( osst );
  string obtained= osst.str();
  string expected= "     total:     3.1353     4.6978     5.4000\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintCorrelations ) {
  BOOST_MESSAGE( "testprintCorrelations" );
  parser.printCorrelations( osst );
  string obtained= osst.str();
  string expected= 
    "Correlations:\n\n"
    " stat:\n"
    "  1.00  0.00  0.00\n"  
    "  0.00  1.00  0.00\n"  
    "  0.00  0.00  1.00\n\n"
    " err1:\n"
    " p p p\n"
    " p p p\n"
    " p p p\n\n"
    " err2:\n"
    " f f f\n"
    " f f f\n"
    " f f f\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintCovariances ) {
  BOOST_MESSAGE( "testprintCovariances" );
  parser.printCovariances( osst, std::ios::scientific, 3 );
  string obtained= osst.str();
  string expected=
    "Covariances:\n\n"
    " stat:\n"
    "  9.000e-02  0.000e+00  0.000e+00\n"
    "  0.000e+00  1.089e-01  0.000e+00\n"
    "  0.000e+00  0.000e+00  1.600e-01\n\n"
    " err1:\n"
    "  1.210e+00  1.210e+00  1.210e+00\n"
    "  1.210e+00  1.690e+00  1.690e+00\n"
    "  1.210e+00  1.690e+00  2.250e+00\n\n"
    " err2:\n"
    "  8.100e-01  1.350e+00  1.710e+00\n"
    "  1.350e+00  2.250e+00  2.850e+00\n"
    "  1.710e+00  2.850e+00  3.610e+00\n\n"
    " err3:\n"
    "  5.760e+00  5.760e+00  5.760e+00\n"
    "  5.760e+00  9.610e+00  9.610e+00\n"
    "  5.760e+00  9.610e+00  1.225e+01\n\n"
    " err4:\n"
    "  1.960e+00  4.060e+00  4.620e+00\n"
    "  4.060e+00  8.410e+00  9.570e+00\n"
    "  4.620e+00  9.570e+00  1.089e+01\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_CASE( testprintCorrelationMatrices ) {
  BOOST_MESSAGE( "testprintCorrelationMatrices" );
  parser.printCorrelationMatrices( osst );
  string obtained= osst.str();
  string expected=
    "Correlation matrices:\n\n"
    " stat:\n"
    "  1.00  0.00  0.00\n"
    "  0.00  1.00  0.00\n"
    "  0.00  0.00  1.00\n\n"
    " err1:\n"
    "  1.00  0.85  0.73\n"
    "  0.85  1.00  0.87\n"
    "  0.73  0.87  1.00\n\n"
    " err2:\n"
    "  1.00  1.00  1.00\n"
    "  1.00  1.00  1.00\n"
    "  1.00  1.00  1.00\n\n"
    " err3:\n"
    "  1.00  0.77  0.69\n"
    "  0.77  1.00  0.89\n"
    "  0.69  0.89  1.00\n\n"
    " err4:\n"
    "  1.00  1.00  1.00\n"
    "  1.00  1.00  1.00\n"
    "  1.00  1.00  1.00\n";
  BOOST_CHECK_EQUAL( obtained, expected );
}

BOOST_AUTO_TEST_SUITE_END()


// Fixture for options tests:
class AverageDataParserOptionsFixture {
public:
  AverageDataParserOptionsFixture() {
    names.push_back( "Val1" );
    names.push_back( "Val2" );
    names.push_back( "Val3" );
    Double_t data[]= { 171.5, 173.1, 174.5 };
    values.ResizeTo( 3 );
    values.SetElements( data );
  }
  vector<string> names;
  TVectorD values;
  map<string,TVectorD> errorsmap;
  map<string,string> covopts;
};

BOOST_FIXTURE_TEST_SUITE( averagedataparseroptionssuite, 
			  AverageDataParserOptionsFixture )

BOOST_AUTO_TEST_CASE( testOptionPercentU ) {
  BOOST_MESSAGE( "testOptionPercentU" );
  Double_t dataerrs[]= { 0.2, 0.22, 0.3 };
  TVectorD errors( 3, dataerrs );
  errorsmap.insert( map<string,TVectorD>::value_type( "00stat", errors ) );
  covopts["00stat"]= "%u";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,TVectorD> obtainederrorsmap= myparser.getErrors();
  TVectorD obtainederrors= obtainederrorsmap["00stat"];
  Double_t datastat[]= { 0.343, 0.38082, 0.5235 };
  TVectorD expectederrors( 3, datastat );
  checkVector( obtainederrors, expectederrors );
  map<string,TMatrixDSym> covariancesmap= myparser.getCovariances();
  TMatrixDSym obtainedcovm= covariancesmap["00stat"];
  double matrixStat[]= { 0.117649, 0.0, 0.0,  0.0, 0.14502387, 
			 0.0, 0.0,  0.0,  0.27405225 };
  TMatrixDSym expectedcovm( 3, matrixStat );
  checkMatrix( obtainedcovm, expectedcovm );
}

BOOST_AUTO_TEST_CASE( testOptionGP ) {
  BOOST_MESSAGE( "testOptionGP" );
  Double_t dataerrs[]= { 0.9, 1.5, 1.9 };
  TVectorD errors( 3, dataerrs );
  errorsmap.insert( map<string,TVectorD>::value_type( "02errb", errors ) );
  covopts["02errb"]= "gp";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,TVectorD> obtainederrorsmap= myparser.getErrors();
  TVectorD obtainederrors= obtainederrorsmap["02errb"];
  checkVector( obtainederrors, errors );
  map<string,TMatrixDSym> covariancesmap= myparser.getCovariances();
  TMatrixDSym obtainedcovm= covariancesmap["02errb"];
  double matrixErrb[]= { 0.81,  0.81,  0.81,  0.81,  2.25,  0.81, 0.81,  
			 0.81,  3.61 };
  TMatrixDSym expectedcovm( 3, matrixErrb );
  checkMatrix( obtainedcovm, expectedcovm );
  map<string,TMatrixDSym> reducedcovariancesmap= myparser.getReducedCovariances();
  TMatrixDSym obtainedreducedcovm= reducedcovariancesmap["02errb"];
  double matrixErrbRed[]= { 0.0, 0.0, 0.0, 0.0, 1.44, 0.0, 0.0, 0.0, 2.8 };
  TMatrixDSym expectedreducedcovm( 3, matrixErrbRed );
  checkMatrix( obtainedreducedcovm, expectedreducedcovm );
  map<int,TVectorD> systerrmap= myparser.getSysterrorMatrix();
  TVectorD systerrs= systerrmap[0];
  Double_t data[]= { 0.9, 0.9, 0.9 };
  TVectorD expectedSysterrs( 3, data );
  checkVector( systerrs, expectedSysterrs );
}

BOOST_AUTO_TEST_CASE( testOptionGPR ) {
  BOOST_MESSAGE( "testOptionGPR" );
  Double_t dataerrs[]= { 2.4, 3.1, 3.5 };
  TVectorD errors( 3, dataerrs );
  errorsmap.insert( map<string,TVectorD>::value_type( "03errc", errors ) );
  covopts["03errc"]= "gpr";
  AverageDataParser myparser( names, values, errorsmap, covopts );
  map<string,TVectorD> obtainederrorsmap= myparser.getErrors();
  TVectorD obtainederrors= obtainederrorsmap["03errc"];
  checkVector( obtainederrors, errors );
  map<string,TMatrixDSym> covariancesmap= myparser.getCovariances();
  TMatrixDSym obtainedcovm= covariancesmap["03errc"];
  double matrixErrc[]= { 5.76, 5.81373761, 5.86075802,  5.81373761, 9.61, 
			 5.91543564, 5.86075802, 5.91543564, 12.25 };
  TMatrixDSym expectedcovm( 3, matrixErrc );
  checkMatrix( obtainedcovm, expectedcovm );
  map<string,TMatrixDSym> reducedcovariancesmap= myparser.getReducedCovariances();
  TMatrixDSym obtainedreducedcovm= reducedcovariancesmap["03errc"];
  double matrixErrcRed[]= { 0.0, 0.0, 0.0, 0.0, 3.742023, 0.0, 0.0, 0.0, 6.286721 };
  TMatrixDSym expectedreducedcovm( 3, matrixErrcRed );
  checkMatrix( obtainedreducedcovm, expectedreducedcovm );
  map<int,TVectorD> systerrmap= myparser.getSysterrorMatrix();
  TVectorD systerrs= systerrmap[0];
  Double_t data[]= { 2.4, 2.42239067, 2.4419825 };
  TVectorD expectedSysterrs( 3, data );
  checkVector( systerrs, expectedSysterrs );
}

  
BOOST_AUTO_TEST_SUITE_END()


class AverageDataParserGroupFixture {
public:
  AverageDataParserGroupFixture() : parser( "valassi1.txt" ) {}
  AverageDataParser parser;
};

BOOST_FIXTURE_TEST_SUITE( averagedataparsergroupsuite, 
			  AverageDataParserGroupFixture )

BOOST_AUTO_TEST_CASE( testGetGroups ) {
  BOOST_MESSAGE( "testgetGroups" );
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

BOOST_AUTO_TEST_CASE( testGetUniqueGroups ) {
  BOOST_MESSAGE( "testgetUniqueGroups" );
  vector<string> groups= parser.getUniqueGroups();
  vector<string> expectedGroups;
  expectedGroups.push_back( "a" );
  expectedGroups.push_back( "b" );
  BOOST_CHECK_EQUAL( groups.size(), expectedGroups.size() );
  for( size_t i= 0; i < expectedGroups.size(); i++ ) {
    BOOST_CHECK_EQUAL( groups[i], expectedGroups[i] );
  }
}

BOOST_AUTO_TEST_CASE( testGetGroupMatrix ) {
  BOOST_MESSAGE( "testgetGroupMatrix" );
  TMatrixD groupmatrix= parser.getGroupMatrix();
  Double_t gmdata[]= { 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0 };
  TMatrixD expectedgm( 4, 2, gmdata );
  checkMatrix( groupmatrix, expectedgm );
}

BOOST_AUTO_TEST_SUITE_END()
