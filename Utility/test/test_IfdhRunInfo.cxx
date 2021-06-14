// test_IfdhRunInfo.cxx
//
// David Adams
// Marach 2020
//
// Test IfdhRunInfo.

#include "../IfdhRunInfo.h"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#undef NDEBUG
#include <cassert>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::istringstream;
using std::vector;

using Index = IfdhRunInfo::Index;

//**********************************************************************

int test_IfdhRunInfo(Index irun, int ivalexp) {
  const string myname = "test_IfdhRunInfo: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  float scale = 100.0;
  cout << myname << "Create object." << endl;
  string scam = "iceberg";
  IfdhRunInfo myo(scam, irun);

  cout << myname << line << endl;
  cout << myname << "Check params." << endl;
  assert( myo.campaign() == scam );
  assert( myo.run() == irun );

  cout << myname << line << endl;
  cout << myname << "Check file list" << endl;
  Index nfil = myo.fileCount();
  cout << myname << "File count: " << nfil << endl;
  assert( nfil >= 1 );

  cout << myname << line << endl;
  cout << myname << "Fetch metadata blob" << endl;
  string smd = myo.getMetadataBlob();
  assert( smd.size() );
  cout << myname << "----- Start metadata -----" << endl;
  cout << smd;
  cout << myname << "----- End metadata -----" << endl;
  assert( nfil == 1 );

  cout << myname << line << endl;
  string svalexp = std::to_string(ivalexp);
  cout << myname << "Fetch metadata string" << endl;
  string sfld = "DUNE_data.fegain";
  string sval = myo.getMetadata(sfld);
  cout << myname << "String value: " << sval << endl;
  assert( sval == svalexp );

  cout << myname << line << endl;
  cout << myname << "Fetch metadata int" << endl;
  int ival = myo.getIntMetadata(sfld);
  cout << myname << "Int value: " << ival << endl;
  assert( ival == ivalexp );

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  Index irun = 8990;
  int ivalexp = 2;
  if ( argc > 1 ) {
    string sarg = argv[1];
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << "[RUN [FEGAIN]]" << endl;
      return 0;
    }
    irun = stoi(sarg);
  }
  if ( argc > 2 ) {
    string sarg = argv[2];
    ivalexp = stoi(sarg);
  }
  return test_IfdhRunInfo(irun, ivalexp);
}

//**********************************************************************
