IF(NOT EXISTS "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/install_manifest.txt\"")
ENDIF(NOT EXISTS "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/install_manifest.txt")

FILE(READ "F:/Documents/Visual Studio 2015/Projects/VCT_Engine/VCT_Engine/_build/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "C:/Program Files (x86)/CMake/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
