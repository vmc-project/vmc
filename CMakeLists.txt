# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# Configuration file for CMake build for the VMC package.
# Inspired by Geant4, Geant4 VMC and VGM projects.
#
# I. Hrivnacova, 09/04/2019

#---  Enforce an out-of-source builds before anything else ---------------------
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "VMC requires an out-of-source build.")
    message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
    message(STATUS "CMakeCache.txt")
    message(STATUS "CMakeFiles")
    message(STATUS "Once these files are removed, create a separate directory")
    message(STATUS "and run CMake from there")
    message(FATAL_ERROR "in-source build detected")
endif()

#--- Define CMake requirements -------------------------------------------------
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

#--- Prepend our own CMake Modules to the search path --------------------------
set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH}) 

#--- Project definitions -------------------------------------------------------
project(VMC)

#--- Project version -----------------------------------------------------------
#include(VMCVersion)
set(${PROJECT_NAME}_VERSION_MAJOR "2")
set(${PROJECT_NAME}_VERSION_MINOR "0")
set(${PROJECT_NAME}_VERSION_PATCH "0")
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

#--- Default project build mode ------------------------------------------------
include(VMCBuildMode)

#--- Options -------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build the dynamic libraries" ON)

#--- Find required packages ----------------------------------------------------
include(VMCRequiredPackages)

#--- Utility to defined installation lib directory -----------------------------
include(VMCInstallLibDir)

#--- Add the packages sources --------------------------------------------------
add_subdirectory(source)

#--- Build project configuration -----------------------------------------------
include(VMCBuildProject)

#--- CPack ---------------------------------------------------------------------
include(VMCCPack)
