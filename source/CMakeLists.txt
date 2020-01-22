# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# CMake Configuration file for vmc
#
# I. Hrivnacova, 09/04/2019

#---CMake required version -----------------------------------------------------
#cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

#----------------------------------------------------------------------------
# Define installed names
#
set(base_name vmc)
set(library_name ${PROJECT_NAME}Library)

#-- ROOT (required) ------------------------------------------------------------
# (with ROOTConfig & UseROOT files from ROOT installation)
#
if(NOT ROOT_FOUND)
  find_package(ROOT CONFIG REQUIRED)
endif()
include(${ROOT_USE_FILE})

#--- Utility to defined installation lib directory -----------------------------
if("${CMAKE_INSTALL_LIBDIR}" MATCHES "")
  include(VMCInstallLibDir)
endif()

#----------------------------------------------------------------------------
# Setup project include directories; compile definitions; link libraries
#
include_directories(
  ${PROJECT_SOURCE_DIR}/source/include 
  ${CMAKE_CURRENT_BINARY_DIR})

#----------------------------------------------------------------------------
# Generate Root dictionaries
# (with macro from ROOT)
#
ROOT_GENERATE_DICTIONARY(
  ${library_name}_dict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TGeoMCBranchArrayContainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TGeoMCGeometry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCAutoLock.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCManager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCManagerStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCOptical.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCParticleStatus.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCParticleType.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCProcess.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCVerbose.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TMCtls.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TVirtualMC.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TVirtualMCApplication.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TVirtualMCGeometry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TVirtualMCSensitiveDetector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/TVirtualMCStack.h
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkDef.h)

# Files produced by the dictionary generation
SET(root_dict
  ${library_name}_dict.cxx)
SET(root_dict_libs
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_dict_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_dict.rootmap)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/source/include/*.h)

#---Add library-----------------------------------------------------------------
set(ROOT_DEPS ROOT::Core ROOT::Physics ROOT::Geom ROOT::EG)
add_library(${library_name} ${sources} ${root_dict} ${headers})
target_link_libraries(${library_name} ${ROOT_DEPS})
set_target_properties(${library_name} PROPERTIES INTERFACE_LINK_LIBRARIES "${ROOT_DEPS}")
target_include_directories(${library_name} INTERFACE $<INSTALL_INTERFACE:include/${base_name}>)

#----Installation---------------------------------------------------------------
install(DIRECTORY include/ DESTINATION include/${base_name})
install(TARGETS ${library_name} EXPORT VMCTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${root_dict_libs} DESTINATION ${CMAKE_INSTALL_LIBDIR})
