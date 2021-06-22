# duneifdh/Tool/test/CMakeLists.txt

# Test for tool.
add_executable(test_IfdhRunDataTool test_IfdhRunDataTool.cxx)
target_include_directories(test_IfdhRunDataTool BEFORE PRIVATE ../../Interface)
find_library(LIB_dune_ArtSupport "libdune_ArtSupport.so")
target_link_libraries(test_IfdhRunDataTool ${LIB_dune_ArtSupport})
target_link_libraries(test_IfdhRunDataTool ${source_lib_list})
target_link_libraries(test_IfdhRunDataTool ${ROOT_LIBRARIES})
install(TARGETS test_IfdhRunDataTool DESTINATION test)
