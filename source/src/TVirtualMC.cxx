// -----------------------------------------------------------------------
// Copyright (C) 2019 CERN and copyright holders of VMC Project.
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
//
// See https://github.com/vmc-project/vmc for full licensing information.
// -----------------------------------------------------------------------

// Authors: Ivana Hrivnacova, Rene Brun , Federico Carminati 13/04/2002

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 2002, ALICE Experiment at CERN.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TVirtualMC.h"
#include "TError.h"
#include "TMCVersion.h"
#include "Riostream.h"

namespace {
//_____________________________________________________________________________
void PrintVersion()
{
   /// Prints the  version banner

   std::cout << std::endl
             << "=============================================================" << std::endl
             << " Virtual Monte Carlo Library" << std::endl
             << " Version " << VMC_RELEASE << " ( " << VMC_RELEASE_DATE << " )" << std::endl
             << "=============================================================" << std::endl;
}

} // namespace

/** \class TVirtualMC
    \ingroup vmc

Abstract Monte Carlo interface

Virtual MC provides a virtual interface to Monte Carlo.
It enables the user to build a virtual Monte Carlo application
independent of any actual underlying Monte Carlo implementation itself.

A user will have to implement a class derived from the abstract
Monte Carlo application class, and provide functions like
ConstructGeometry(), BeginEvent(), FinishEvent(), ... .
The concrete Monte Carlo (Geant3, Geant4) is selected at run time -
when processing a ROOT macro where the concrete Monte Carlo is instantiated.
*/

TMCThreadLocal TVirtualMC *TVirtualMC::fgMC = nullptr;
////////////////////////////////////////////////////////////////////////////////
///
/// Standard constructor
///

TVirtualMC::TVirtualMC(const char *name, const char *title, Bool_t /*isRootGeometrySupported*/)
   : TNamed(name, title), fApplication(nullptr), fId(0), fStack(nullptr), fManagerStack(nullptr), fDecayer(nullptr),
     fRandom(nullptr), fMagField(nullptr)
{
   PrintVersion();

   fApplication = TVirtualMCApplication::Instance();

   if (fApplication) {
      fApplication->Register(this);
   } else {
      ::Fatal("TVirtualMC::TVirtualMC", "No user MC application is defined.");
   }
   fgMC = this;
   fRandom = gRandom;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Default constructor
///

TVirtualMC::TVirtualMC()
   : TNamed(), fApplication(nullptr), fId(0), fStack(nullptr), fManagerStack(nullptr), fDecayer(nullptr),
     fRandom(nullptr), fMagField(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
///
/// Destructor
///

TVirtualMC::~TVirtualMC()
{
   fgMC = nullptr;
}

//
// methods
//

////////////////////////////////////////////////////////////////////////////////
///
/// Static access method
///

TVirtualMC *TVirtualMC::GetMC()
{
   return fgMC;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Process one event with given eventId
///

void TVirtualMC::ProcessEvent(Int_t eventId)
{
   ProcessEvent(eventId, kFALSE);
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set particles stack.
///

void TVirtualMC::SetStack(TVirtualMCStack *stack)
{
   fStack = stack;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set external decayer.
///

void TVirtualMC::SetExternalDecayer(TVirtualMCDecayer *decayer)
{
   fDecayer = decayer;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set random number generator.
///

void TVirtualMC::SetRandom(TRandom *random)
{
   gRandom = random;
   fRandom = random;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set magnetic field.
///

void TVirtualMC::SetMagField(TVirtualMagField *field)
{
   fMagField = field;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set the VMC id
///

void TVirtualMC::SetId(UInt_t id)
{
   fId = id;
}

////////////////////////////////////////////////////////////////////////////////
///
/// Set container holding additional information for transported TParticles
///
void TVirtualMC::SetManagerStack(TMCManagerStack *stack)
{
   fManagerStack = stack;
}
