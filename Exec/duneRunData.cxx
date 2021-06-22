// duneRunData.cxx
//
// David Adams
// June 2021
//
// Displays run data for a specified run using the
// tool runDataTool.

#include "dune/DuneInterface/Tool/RunDataTool.h"
#include "dune/ArtSupport/DuneToolManager.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int main(int narg, char* argv[]) {
  const string myname = "duneRunData: ";

  DuneToolManager& dtm = *DuneToolManager::instance();
  string tnam = "runDataTool";
  const RunDataTool* prdt = dtm.getShared<RunDataTool>(tnam);
  if ( prdt == nullptr ) {
    cout << myname << "ERROR: Tool " << tnam << " not found." << endl;
    return 1;
  }

  if ( narg > 1 ) {
    string sarg = argv[narg-1];
    int irun = std::stoi(sarg);
    RunData rdat = prdt->runData(irun);
    cout << rdat << endl;
    return 0;
  }

  cout << "Usage: argv[0] RUN" << endl;
  return 0;

};
  
