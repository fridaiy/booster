
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was module-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

####################################################################################

if(NOT TARGET booster::booster)
    include("${CMAKE_CURRENT_LIST_DIR}/boosterTargets.cmake")
endif()

set_and_check(booster_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include/booster-1.4.0/booster/")
set_and_check(booster_LIBRARIES_DIRS "${PACKAGE_PREFIX_DIR}/lib/booster-1.4.0/")

set(booster_LIBRARIES booster;booster-test)
set(BOOSTER_BASE_DIR "${PACKAGE_PREFIX_DIR}/include/booster-1.4.0/")
