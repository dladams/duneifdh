# duneifdh/Tool/CMakeLists.txt

# Build a tool.
add_library(IfdhRunDataTool_tool SHARED IfdhRunDataTool_tool.cc)
target_link_libraries(IfdhRunDataTool_tool ${tool_lib_list})
target_include_directories(IfdhRunDataTool_tool BEFORE PRIVATE ../Utility)
target_link_libraries(IfdhRunDataTool_tool ${PROJECT_NAME})

message("            PKGLIB: ${PROJECT_NAME}")
install(TARGETS IfdhRunDataTool_tool DESTINATION lib)

add_subdirectory(test)
