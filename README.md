# duneifdh

Use IFDH to access file lists and metadata for DUNE runs.

The utility class [IfdhRunInfo](Utility/IfdhRunInfo.h) retrieves
the metatdata for a specified run in a specified campaign
(protodune or iceberg) and provides it in the form of
an IFDH blob, name-value map or a
[RunData](https://cdcvs.fnal.gov/redmine/projects/dunetpc/repository/revisions/develop/entry/dune/DuneInterface/Data/RunData.h)
object.

The tool IfdhRunDataTool uses this utility to implement the
[RunDataTool](https://cdcvs.fnal.gov/redmine/projects/dunetpc/repository/revisions/develop/entry/dune/DuneInterface/Tool/RunDataTool.h)
interface.
Configurations  to fetch run data for iceberg and protoDUNE may be found in
[duneifdh.fcl](Fcl/duneifdh.fcl).
