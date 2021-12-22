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
 * Copyright (C) 2002, ALICE Experiment at CERN.                         *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMCOptical
#define ROOT_TMCOptical
//
// Enums for setting of optical photon physics
//
#include "Rtypes.h"

/// Optical surface models
enum EMCOpSurfaceModel
{
   kGlisur,                ///< original GEANT3 model
   kUnified,               ///< UNIFIED model
   kLUT,                   ///< Look-Up-Table model (LBNL model)
   kDAVIS,                 ///< DAVIS model
   kdichroic               ///< dichroic filter
};

/// Optical surface types
enum EMCOpSurfaceType
{
   kDielectric_metal,      ///< dielectric-metal interface
   kDielectric_dielectric, ///< dielectric-dielectric interface
   kDielectric_LUT,        ///< dielectric-Look-Up-Table interface
   kDielectric_LUTDAVIS,   ///< dielectric-Look-Up-Table DAVIS interface
   kDielectric_dichroic,   ///< dichroic filter interface
   kFirsov,                ///< for Firsov Process
   kXray                   ///< for x-ray mirror process
};

/// Optical surface finish types
enum EMCOpSurfaceFinish
{
   kPolished,              ///<  smooth perfectly polished surface
   kPolishedfrontpainted,  ///<  smooth top-layer (front) paint
   kPolishedbackpainted,   ///<  same is 'polished' but with a back-paint

   kGround,                ///<  rough surface
   kGroundfrontpainted,    ///<  rough top-layer (front) paint
   kGroundbackpainted,     ///<  same as 'ground' but with a back-paint

   // for LBNL LUT model
   kPolishedlumirrorair,   ///<  mechanically polished surface, with lumirror
   kPolishedlumirrorglue,  ///<  mechanically polished surface, with lumirror &
                           ///   meltmount
   kPolishedair,           ///<  mechanically polished surface
   kPolishedteflonair,     ///<  mechanically polished surface, with teflon
   kPolishedtioair,        ///<  mechanically polished surface, with tio paint
   kPolishedtyvekair,      ///<  mechanically polished surface, with tyvek
   kPolishedvm2000air,     ///<  mechanically polished surface, with esr film
   kPolishedvm2000glue,    ///<  mechanically polished surface, with esr film &
                           ///   meltmount

   kEtchedlumirrorair,     ///<  chemically etched surface, with lumirror
   kEtchedlumirrorglue,    ///<  chemically etched surface, with lumirror & meltmount
   kEtchedair,             ///<  chemically etched surface
   kEtchedteflonair,       ///<  chemically etched surface, with teflon
   kEtchedtioair,          ///<  chemically etched surface, with tio paint
   kEtchedtyvekair,        ///<  chemically etched surface, with tyvek
   kEtchedvm2000air,       ///<  chemically etched surface, with esr film
   kEtchedvm2000glue,      ///<  chemically etched surface, with esr film & meltmount

   kGroundlumirrorair,     ///<  rough-cut surface, with lumirror
   kGroundlumirrorglue,    ///<  rough-cut surface, with lumirror & meltmount
   kGroundair,             ///<  rough-cut surface
   kGroundteflonair,       ///<  rough-cut surface, with teflon
   kGroundtioair,          ///<  rough-cut surface, with tio paint
   kGroundtyvekair,        ///<  rough-cut surface, with tyvek
   kGroundvm2000air,       ///<  rough-cut surface, with esr film
   kGroundvm2000glue,      ///<  rough-cut surface, with esr film & meltmount

   // for DAVIS model
   kRough_LUT,             ///<  rough surface
   kRoughTeflon_LUT,       ///<  rough surface wrapped in Teflon tape
   kRoughESR_LUT,          ///<  rough surface wrapped with ESR
   kRoughESRGrease_LUT,    ///<  rough surface wrapped with ESR
                           ///   and coupled with optical grease
   kPolished_LUT,          ///   polished surface
   kPolishedTeflon_LUT,    ///<  polished surface wrapped in Teflon tape
   kPolishedESR_LUT,       ///<  polished surface wrapped with ESR
   kPolishedESRGrease_LUT, ///<  polished surface wrapped with ESR
                           ///   and coupled with optical grease
   kDetector_LUT           ///   polished surface with optical grease

};

#endif //ROOT_TMCOPtical
