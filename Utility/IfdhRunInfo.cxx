// IfdhRunInfo.cxx

#include "IfdhRunInfo.h"
#include "dune/DuneCommon/StringManipulator.h"
#include "ifdh.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using StringVector = StringManipulator::StringVector;
using NameMap = IfdhRunInfo::NameMap;
using FloatVector = std::vector<float>;

//**********************************************************************

namespace {

int mapint(const NameMap& nmap, string nam, int def) {
  NameMap::const_iterator kval = nmap.find(nam);
  if ( kval == nmap.end() ) return def;
  return std::stoi(kval->second);
}

}

//**********************************************************************

int IfdhRunInfo::convert(const Name& blob, NameMap& nmap) {
  const Name myname = "IfdhRunInfo::convert: ";
  StringManipulator smblob(blob);
  for ( string slin : smblob.split("\n") ) {
    StringVector swrds = StringManipulator(slin).split(": ");
    string sfld;
    string sval;
    string::size_type len = slin.size();
    string::size_type ipos = 0;
    while ( ipos < len && slin[ipos] == ' ' ) ++ipos;
    string::size_type jpos = ipos;
    while ( jpos < len && slin[jpos] != ':' ) ++jpos;
    sfld = slin.substr(ipos, jpos-ipos);
    if ( jpos+2 < len ) sval = slin.substr(jpos+2);
    //cout << myname << sfld << ": " << sval << endl;
    if ( sfld.size() == 0 || sval.size() == 0 ) {
      cout << myname << "ERROR: Unexpected line: " << "\"" << slin << "\"" << endl;
      return 1;
    }
    if ( nmap.count(sfld) ) {
      cout << myname << "WARNING: Replacing duplicate entry for " << sfld << endl;
    }
    nmap[sfld] = sval;
  }
  return 0;
}

//**********************************************************************

int IfdhRunInfo::convert(const NameMap& nmap, RunData& rda) {
  const Name myname = "IfdhRunInfo::convert: ";
  rda.clear();
  NameMap::const_iterator kruns = nmap.find("Runs");
  if ( kruns == nmap.end() ) {
    cout << myname << "ERROR: Metadata is missing Runs field." << endl;
    return 1;
  }
  string sruns = kruns->second;
  string sdet;
  Index irun = 0;
  if ( sruns.size() ) {
    string::size_type ipos = sruns.find(".");
    string::size_type jpos = sruns.find(" (");
    string::size_type kpos = sruns.find(")");
    if ( ipos < jpos && jpos < kpos && (kpos+1) == sruns.size() ) {
      string srun = sruns.substr(0, ipos);
      irun = std::stoi(srun);
      sdet = sruns.substr(jpos+2, kpos-jpos-2);
    }
    FloatVector gainVals = {4.7, 7.8, 14.0, 25.0, 0.0};
    FloatVector shapingVals = {0.5, 1.0, 2.0, 3.0, 0.0};
    FloatVector shapingValsIb5 = {1.0, 0.5, 3.0, 2.0, 0.0};
    FloatVector pulserSourceVals = {0, 1, 2, 99};
    if ( sdet == "protodune-sp" ) {
    } else if ( sdet == "iceberg" ) {
      if ( irun >= 8198 && irun <=9743 ) shapingVals = shapingValsIb5;
      rda.setGain(gainVals[mapint(nmap, "DUNE_data.fegain", 4)]);
      rda.setShaping(shapingVals[mapint(nmap, "DUNE_data.feshapingtime", 4)]);
      rda.setPulserSource(mapint(nmap, "DUNE_data.calibpulsemode", 99));
      rda.setPulserAmplitude(mapint(nmap, "DUNE_data.calibpulsedac", 999));
    } else {
      cout << myname << "ERROR: Metadata has invalid Runs field: " << sruns << endl;
      cout << myname << "       " << ipos << ", " << jpos << ", " << kpos
           << ": " << sdet << endl;
      return 2;
    }
  }
  rda.setRun(irun);
  return 0;
}

//**********************************************************************

IfdhRunInfo::IfdhRunInfo(Name scam, Index irun)
: m_scam(scam), m_irun(irun), m_ifil(maxFileCount()) {
  ifdh_ns::ifdh ifd;
  ifd.getProxy();
  // Flag to include campaign in query.
  bool queryCampaign = true;
  // Required file prefix.
  string filePrefix;
  if ( scam == "np04" ) {
    queryCampaign = false;
    filePrefix = "np04_";
  }
  // For np04 (protoDUNE-SP)
  Index lpre = filePrefix.size();
  Name sqry = "run_number " + std::to_string(irun);
  if ( queryCampaign ) sqry += " and DUNE.campaign " + scam;
  for ( string sfil : ifd.translateConstraints(sqry) ) {
    if ( lpre == 0 || sfil.substr(0, lpre) == filePrefix ) {
      m_files.push_back(sfil);
    }
  }
  getMetadataBlob(0);
}

//**********************************************************************

string IfdhRunInfo::getMetadataBlob(Index ifil) {
  if ( ifil >= fileCount() ) return "";
  ifdh_ns::ifdh ifd;
  if ( ifil == m_ifil ) return m_blob;
  Name fnam = m_files[ifil];
  m_ifil = ifil;
  m_sfil = fnam;
  m_blob = ifd.getMetadata(fnam);
  convert(m_blob, m_nmap);
  return m_blob;
}

//**********************************************************************

string IfdhRunInfo::getMetadata(Name sfld) const {
  NameMap::const_iterator ient = m_nmap.find(sfld);
  if ( ient == m_nmap.end() ) return "";
  return ient->second;
}

//**********************************************************************

int IfdhRunInfo::getIntMetadata(Name sfld, int idef) const {
  NameMap::const_iterator ient = m_nmap.find(sfld);
  if ( ient == m_nmap.end() ) return idef;
  int ival = idef;
  ival = std::stoi(ient->second);
  return ival;
}

//**********************************************************************

RunData IfdhRunInfo::getRunData() const {
  const Name myname = "IfdhRunInfo::getRunData: ";
  RunData rda;
  if ( convert(m_nmap, rda) == 0 ) {
    if ( rda.run() != run() ) {
      cout << myname << "ERROR: Metadata has inconsistent run: "
           << rda.run() << " != " << run() << endl;
      rda.clear();
    }
  }
  return rda;
}

//**********************************************************************
