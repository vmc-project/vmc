#------------------------------------------------
# The Geant4 Virtual Monte Carlo package
# Copyright (C) 2014 Ivana Hrivnacova
# All rights reserved.
#
# For the licensing terms see geant4_vmc/LICENSE.
# Contact: root-vmc@cern.ch
#-------------------------------------------------

# Configuration file for CMake build for VMC packages
# which finds all required and optional packages.
#
# I. Hrivnacova, 07/12/2021

#message(STATUS Processing Geant4VMCRequiredPackages)

#-- ROOT (required) ------------------------------------------------------------
find_package(ROOT CONFIG COMPONENTS Geom EG REQUIRED)

# Cannot mix VMC standalone and vmc in ROOT(deprecated)
if(ROOT_vmc_FOUND)
  message(FATAL_ERROR
          "Cannot use VMC standalone with ROOT built with vmc.")
endif()

# Root definitions
include(${ROOT_USE_FILE})
