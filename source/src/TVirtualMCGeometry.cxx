// -----------------------------------------------------------------------
// Copyright (C) 2019 CERN and copyright holders of VMC Project.
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
//
// See https://github.com/vmc-project/vmc for full licensing information.
// -----------------------------------------------------------------------

// Authors: Alice collaboration 25/06/2002

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 2002, ALICE Experiment at CERN.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TVirtualMCGeometry.h"

/** \class TVirtualMCGeometry
    \ingroup vmc

Interface to Monte Carlo geometry construction.

This class has been separated from VirtualMC and it is used
internally in TVirtualMC implementations.
*/

ClassImp(TVirtualMCGeometry);

////////////////////////////////////////////////////////////////////////////////
///
/// Standard constructor
///

TVirtualMCGeometry::TVirtualMCGeometry(const char *name, const char *title) : TNamed(name, title) {}

////////////////////////////////////////////////////////////////////////////////
///
/// Default constructor
///

TVirtualMCGeometry::TVirtualMCGeometry() : TNamed() {}

////////////////////////////////////////////////////////////////////////////////
///
/// Destructor
///

TVirtualMCGeometry::~TVirtualMCGeometry() {}
