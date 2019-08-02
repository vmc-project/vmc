# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# Configuration file for CMake build for VMC library
#
# I. Hrivnacova, 09/04/2019

set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_VERSION_PATCH})
set(CPACK_SOURCE_GENERATOR "TGZ")

if(${${PROJECT_NAME}_VERSION_PATCH} EQUAL "0")
  set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "vmc.${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
else()
  set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "vmc.${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.p${CPACK_PACKAGE_VERSION_PATCH}")
endif()      
# Do not use CPACK_SOURCE_IGNORE_FILES as this would exclude test scripts
# and log_ref directory
#set(CPACK_SOURCE_IGNORE_FILES
#  "/build/;/.bzr/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
