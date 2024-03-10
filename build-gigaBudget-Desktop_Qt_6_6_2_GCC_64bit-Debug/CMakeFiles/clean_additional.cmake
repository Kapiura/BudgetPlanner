# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/gigaBudget_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/gigaBudget_autogen.dir/ParseCache.txt"
  "gigaBudget_autogen"
  )
endif()
