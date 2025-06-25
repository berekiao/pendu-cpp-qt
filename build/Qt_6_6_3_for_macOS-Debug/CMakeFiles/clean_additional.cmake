# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Pendu_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Pendu_autogen.dir/ParseCache.txt"
  "Pendu_autogen"
  )
endif()
