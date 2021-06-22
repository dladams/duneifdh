// test_IfdhRunDataTool.cxx
//
// David Adams
// June 2021
//
// Test IfdhRunDataTool.

#include "dune/DuneInterface/Tool/RunDataTool.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "dune/ArtSupport/DuneToolManager.h"
#include "TH1F.h"

#undef NDEBUG
#include <cassert>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::istringstream;
using fhicl::ParameterSet;
using Index = unsigned int;

//**********************************************************************

int test_IfdhRunDataTool(bool useExistingFcl, Index irun) {
  const string myname = "test_IfdhRunDataTool: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_IfdhRunDataTool.fcl";
  if ( ! useExistingFcl ) {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "tools: {" << endl;
    fout << "  mytool: {" << endl;
    fout << "       tool_type: IfdhRunDataTool" << endl;
    fout << "     LogLevel: 2" << endl;
    fout << "     Campaign: iceberg" << endl;
    fout << "  }" << endl;
    fout << "}" << endl;
    fout.close();
  } else {
    cout << myname << "Using existing top-level FCL." << endl;
  }

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  assert ( ptm != nullptr );
  DuneToolManager& tm = *ptm;
  tm.print();
  assert( tm.toolNames().size() >= 1 );

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto pto = tm.getPrivate<RunDataTool>("mytool");
  assert( pto != nullptr );

  cout << myname << line << endl;
  cout << myname << "Check run data for run " << irun << endl;
  RunData rdat = pto->runData(irun);
  cout << rdat << endl;
  assert( rdat.run() == irun );

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  bool useExistingFcl = false;
  Index irun = 8990;
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << " [keepFCL] [RUN]" << endl;
      cout << "  If keepFCL = true, existing FCL file is used." << endl;
      cout << "  If RUN is nonzero, the data for that run are displayed." << endl;
      return 0;
    }
    useExistingFcl = sarg == "true" || sarg == "1";
    if ( argc > 2 ) {
      sarg = argv[2];
      irun = std::stoi(sarg);
    }
  }
  return test_IfdhRunDataTool(useExistingFcl, irun);
}

//**********************************************************************
