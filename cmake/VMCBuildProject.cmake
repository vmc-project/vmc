# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# Configuration file for CMake build for VMC library
# which defines customised installation.
#
# I. Hrivnacova, 09/04/2019

# Copy the custom cmake modules into the build tree
foreach(_mod CMakeMacroParseArguments VMCBuildMode VMCInstallLibDir)
  configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/${_mod}.cmake
    ${PROJECT_BINARY_DIR}/Modules/${_mod}.cmake
    COPYONLY
  )
endforeach()

# Find modules
#foreach(_find_mod ROOT)
#  configure_file(
#    ${PROJECT_SOURCE_DIR}/cmake/Find${_find_mod}.cmake
#    ${PROJECT_BINARY_DIR}/Modules/Find${_find_mod}.cmake
#    COPYONLY
#  )
#endforeach()

# Set needed variables for the install tree
set(VMC_CMAKE_DIR ${CMAKE_INSTALL_PREFIX}/cmake)

# Install the custom modules for the examples
install(DIRECTORY
  ${PROJECT_BINARY_DIR}/Modules/
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/VMC-${VMC_VERSION}/Modules
  COMPONENT Development
)

#
# Install the VMCConfig, VMCConfigVersion
#
configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/VMCConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/VMCConfig.cmake" @ONLY)

configure_file(
  "${PROJECT_SOURCE_DIR}/cmake/VMCConfigVersion.cmake.in"
  "${PROJECT_BINARY_DIR}/VMCConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/VMCConfig.cmake"
  "${PROJECT_BINARY_DIR}/VMCConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/VMC-${VMC_VERSION})

install(EXPORT VMCTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/VMC-${VMC_VERSION})
