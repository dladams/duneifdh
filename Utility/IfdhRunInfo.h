// IfdhRunInfo.h
//
// Use ifdh to find metadata for a run.

#include "dune/DuneInterface/Data/RunData.h"
#include <string>
#include <vector>
#include <map>

class IfdhRunInfo {

public:
  using Name = std::string;
  using NameVector = std::vector<Name>;
  using NameMap = std::map<Name, Name>;
  using Index = unsigned int;

  // Maximum number of files.
  Index maxFileCount() { return 9999999; }

  // Convert a blob into a map.
  static int convert(const Name& blob, NameMap& nmap);

  // Convert a map into a RunData object.
  static int convert(const NameMap& nmap, RunData& rda);

  // Ctor from campaign name and run number.
  IfdhRunInfo(Name scam, Index irun);

  Name campaign() const { return m_scam; }
  Index run() const { return m_irun; }
  Index fileCount() const { return m_files.size(); }
  const NameVector& files() const { return m_files; }
  Name file(Index ifil) const { return ifil<fileCount() ? m_files[ifil] : ""; }

  // Fetch the current metadata blob for a file.
  Name getMetadataBlob(Index ifil =0);

  // Fetch a metadata field as a string from the current blob.
  // sfld - field name, e.g. "DUNE_data.fegain"
  Name getMetadata(Name sfld) const;

  // Fetch a metadata field as an int from the current blob.
  // sfld - field name, e.g. "DUNE_data.fegain"
  // idef - value reuturned if filed is missing or not int
  int getIntMetadata(Name sfld, int idef =0) const;

  // Convert metadata to RunData.
  RunData getRunData() const;

private:
  Name m_scam;
  Index m_irun;
  NameVector m_files;
  Index m_ifil;
  Name m_sfil;
  Name m_blob;
  NameMap m_nmap;

};
