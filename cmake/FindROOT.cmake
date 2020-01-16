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

if(ROOT_FIND_COMPONENTS)
    find_package(ROOT CONFIG COMPONENTS ${ROOT_FIND_COMPONENTS} PATHS ${ROOT_DIR})
else()
    find_package(ROOT CONFIG  PATHS ${ROOT_DIR})
endif()

if(ROOT_FIND_REQUIRED AND NOT ROOT_FOUND)
    message(FATAL_ERROR "ROOT required, but not found")
endif()

# Determine CXX STD from ROOT
SET(CMAKE_CXX_STANDARD 11)
# Find ROOT CXX standard
string(FIND ${ROOT_CXX_FLAGS} "-std=" POSITION)
if (${POSITION} GREATER -1)
    string(SUBSTRING ${ROOT_CXX_FLAGS} ${POSITION} 11 ROOT_CXX_STD)
    if(${ROOT_CXX_STD} STREQUAL "-std=c++1z " OR ${ROOT_CXX_STD} STREQUAL "-std=c++17 ")
        SET(CMAKE_CXX_STANDARD 17)
    elseif(${ROOT_CXX_STD} STREQUAL "-std=c++1y " OR ${ROOT_CXX_STD} STREQUAL "-std=c++14 ")
        SET(CMAKE_CXX_STANDARD 14)
    endif()
endif()
message(STATUS "Build with CXX STD ${CMAKE_CXX_STANDARD}")
