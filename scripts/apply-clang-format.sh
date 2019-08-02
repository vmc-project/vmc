#!/bin/bash

# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# Process the VMC package source code files with clang-format.
# The script should be run from the top vmc directory.
#
# To apply clang-format on a single file:
# clang-format -style=file -i path/filename
#
# I. Hrivnacova 02/08/2019
#

#set -x 

if [ ! -d source ]; then
  echo "The script should be run from the top vmc directory."
  exit 1;
fi

for DIR in source
do
  echo "... processing directory $DIR"
  for FILE in `find $DIR -iname "*.h" -o -iname "*.icc" -o -iname "*.cxx"`
  do
    # Exclude files which were not formatted in ROOT
    # in order to avoid diverging with the vmc in ROOT
    PASS=TRUE
    for TO_BE_SKIPPED in \
      source/include/TGeoMCGeometry.h \
      source/include/TMCAutoLock.h \
      source/include/TMCOptical.h \
      source/include/TMCParticleType.h \
      source/include/TMCProcess.h \
      source/include/TMCtls.h \
      source/include/TVirtualMCGeometry.h \
      source/include/TVirtualMCStack.h \
      source/src/TGeoMCGeometry.cxx \
      source/src/TMCManager.cxx \
      source/src/TMCVerbose.cxx \
      source/src/TVirtualMCGeometry.cxx \
      source/src/TVirtualMCStack.cxx
    do
      if [[ $FILE = "$TO_BE_SKIPPED" ]]; then
        PASS=FALSE
      fi
    done

    if [[ $PASS = "TRUE" ]]; then
      echo "...... processing file $FILE"
      clang-format -style=file -i $FILE
    else
      echo "...... skipped file $FILE"
    fi
  done
done
