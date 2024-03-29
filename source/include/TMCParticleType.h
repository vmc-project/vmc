// -----------------------------------------------------------------------
// Copyright (C) 2019 CERN and copyright holders of VMC Project.
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "LICENSE".
//
// See https://github.com/vmc-project/vmc for full licensing information.
// -----------------------------------------------------------------------

// Author: Alice collaboration

/*************************************************************************
 * Copyright (C) 2006, Rene Brun and Fons Rademakers.                    *
 * Copyright (C) 1998-1999, ALICE Experiment at CERN.                    *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMCParticleType
#define ROOT_TMCParticleType

//
// List of MonteCarlo particle types
// (used when adding user own particles to MC)
//

#include "Rtypes.h"

/// VMC particle type codes
enum TMCParticleType {
   kPTGamma,           ///< particle tracked as gamma
   kPTElectron,        ///< particle tracked as electron
   kPTNeutron,         ///< particle tracked as neutron
   kPTHadron,          ///< particle tracked as hadron
   kPTMuon,            ///< particle tracked as muon
   kPTGeantino,        ///< particle tracked as geantino
   kPTChargedGeantino, ///< particle tracked as charged geantino
   kPTOpticalPhoton,   ///< particle tracked as light photon
   kPTIon,             ///< particle tracked as heavy ion
   kPTUndefined        ///< unknown particle type
};

#endif
