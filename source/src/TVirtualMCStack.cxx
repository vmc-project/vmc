// -----------------------------------------------------------------------
// Copyright (C) 2019 CERN and copyright holders of VMC Project.
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
//
// See https://github.com/vmc-project/vmc for full licensing information.
// -----------------------------------------------------------------------

// Author: Ivana Hrivnacova, 27/03/2002

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 2002, ALICE Experiment at CERN.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TVirtualMCStack.h"

/** \class TVirtualMCStack
    \ingroup vmc

Interface to a user defined particles stack.
*/

ClassImp(TVirtualMCStack);

////////////////////////////////////////////////////////////////////////////////
/// Default constructor

TVirtualMCStack::TVirtualMCStack() : TObject() {}

////////////////////////////////////////////////////////////////////////////////
/// Destructor

TVirtualMCStack::~TVirtualMCStack() {}
