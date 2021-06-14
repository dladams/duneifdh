# duneifdh/Utility/test/CMakeLists.txt

# Test for utility.
add_executable(test_IfdhRunInfo test_IfdhRunInfo.cxx)
target_link_libraries(test_IfdhRunInfo ${PKGLIB})
install(TARGETS test_IfdhRunInfo DESTINATION test)
