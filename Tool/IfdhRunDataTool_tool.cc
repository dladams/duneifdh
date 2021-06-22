//  IfdhRunDataTool_tool.cc
//
// David Adams
// June 2021
//
// Tool to fetch run data for a run using the IfdhRunInfo utility.

#include "IfdhRunInfo.h"
#include "dune/DuneInterface/Tool/RunDataTool.h"
#include "fhiclcpp/ParameterSet.h"
#include "art/Utilities/ToolMacros.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std:: string;

class IfdhRunDataTool : public RunDataTool {
public:
  explicit IfdhRunDataTool(fhicl::ParameterSet const& ps);
  RunData runData(Index irun, Index isubrun =0) const;
private:
  // Configuration parameters.
  Index m_LogLevel;
  string m_Campaign;
};

IfdhRunDataTool::IfdhRunDataTool(const fhicl::ParameterSet& ps)
: m_LogLevel(ps.get<Index>("LogLevel")),
  m_Campaign(ps.get<string>("Campaign")) {
  const string myname = "IfdhRunDataTool::ctor: ";
  cout << myname << "  LogLevel: " << m_LogLevel << endl;
  cout << myname << "  Campaign: " << m_Campaign << endl;
}

RunData IfdhRunDataTool::runData(Index irun, Index) const {
  const string myname = "IfdhRunDataTool::runData: ";
  IfdhRunInfo iri(m_Campaign, irun);
  if ( m_LogLevel >=2 ) {
    cout << myname << "Fetching run data for run " << irun << endl;
  }
  RunData rda = iri.getRunData();
  if ( ! rda.isValid() && m_LogLevel >= 2 ) {
    cout << myname << "Unable to decode metadata." << endl;
    cout << myname << "----- IFDH blob:" << endl;
    cout << iri.getMetadataBlob();
    cout << myname << "----------------" << endl;
  }
  return rda;
}

DEFINE_ART_CLASS_TOOL(IfdhRunDataTool)

