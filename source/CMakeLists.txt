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
  TGeoMCBranchArrayContainer.h
  TGeoMCGeometry.h
  TMCAutoLock.h
  TMCManager.h
  TMCManagerStack.h
  TMCOptical.h
  TMCParticleStatus.h
  TMCParticleType.h
  TMCProcess.h
  TMCVerbose.h
  TMCtls.h
  TVirtualMC.h
  TVirtualMCApplication.h
  TVirtualMCGeometry.h
  TVirtualMCSensitiveDetector.h
  TVirtualMCStack.h
  MODULE ${library_name}
  OPTIONS "-I${CMAKE_INSTALL_PREFIX}/include/${base_name}"
    -excludePath "${CMAKE_CURRENT_BINARY_DIR}"
    -excludePath "${PROJECT_SOURCE_DIR}/source"
  LINKDEF include/LinkDef.h)

# Files produced by the dictionary generation
SET(root_dict
  ${library_name}_dict.cxx)
SET(root_dict_libs
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}_rdict.pcm
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${library_name}.rootmap)

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
