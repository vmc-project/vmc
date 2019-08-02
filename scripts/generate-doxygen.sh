#!/bin/bash

# ------------------------------------------------------------------------
# Copyright (C) 2019 CERN and copyright holders of VMC Project.
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# See https://github.com/vmc-project/vmc for full licensing information.
# ------------------------------------------------------------------------

# Script to generate vmc doxygen documentation.
#
# I. Hrivnacova 02/08/2019
#

echo "Generating Doxygen code documentation..."

for DIR in source
do
  echo "... processing $DIR"
  doxygen doc/doxygen/Doxyfile
done
