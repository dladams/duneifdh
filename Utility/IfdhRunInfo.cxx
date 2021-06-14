// IfdhRunInfo.cxx

#include "IfdhRunInfo.h"
#include "dune/DuneCommon/StringManipulator.h"
#include "ifdh.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using StringVector = StringManipulator::StringVector;

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
    cout << myname << sfld << ": " << sval << endl;
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

IfdhRunInfo::IfdhRunInfo(Name scam, Index irun)
: m_scam(scam), m_irun(irun), m_ifil(maxFileCount()) {
  ifdh_ns::ifdh ifd;
  ifd.getProxy();
  Name sqry = "Dune.campaign " + scam + " and run_number "
                + std::to_string(irun);
  m_files = ifd.translateConstraints(sqry);
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
