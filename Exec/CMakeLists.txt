# duneifdh/Exec/CMakeLists.txt

find_library(libartsupport NAMES dune_ArtSupport)

# Build an executable.
add_executable(duneRunData duneRunData.cxx)
target_link_libraries(duneRunData ${tool_lib_list})
target_link_libraries(duneRunData ${libartsupport})
install(TARGETS duneRunData DESTINATION bin)
