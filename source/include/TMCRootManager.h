#ifndef ROOT_TMCRootManager
#define ROOT_TMCRootManager

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRootManager.h
/// \brief Definition of the TMCRootManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TMCtls.h"
#include <Rtypes.h>

class TParticle;
class TFile;
class TTree;

/// \brief The Root IO manager for VMC examples for both sequential and
/// multi-threaded applications.
///
/// It facilitates use of ROOT IO in VMC examples and also handles necessary
/// locking in multi-threaded applications.

class TMCRootManager
{
 public:
  /// Root file mode
  enum FileMode
  {
    kRead, // Read mode
    kWrite // Write mode
  };

 public:
  // static access method
  static TMCRootManager* Instance();

  // static method for activating debug mode
  static void SetDebug(Bool_t debug);
  static Bool_t GetDebug();

  TMCRootManager(
    const char* projectName, FileMode fileMode = kWrite, Int_t threadRank = -1);
  virtual ~TMCRootManager();

  // methods
  void Register(const char* name, const char* className, void* objAddress);
  void Register(
    const char* name, const char* className, const void* objAddress);
  void Fill();
  void WriteAll();
  void Close();
  void WriteAndClose();
  void ReadEvent(Int_t i);

 private:
  // not implemented
  TMCRootManager(const TMCRootManager& rhs);
  TMCRootManager& operator=(const TMCRootManager& rhs);

  // global static data members
  static Int_t fgCounter; // The counter of instances
  // static data members
  static Bool_t fgDebug; // Option to activate debug printings

#if !defined(__CINT__)
  static TMCThreadLocal TMCRootManager* fgInstance; // singleton instance
#else
  static TMCRootManager* fgInstance; // singleton instance
#endif

  // Methods
  void OpenFile(const char* projectName, FileMode fileMode, Int_t threadRank);

  // data members
  Int_t fId;        // This manager ID
  TFile* fFile;     // Root output file
  TTree* fTree;     // Root output tree
  Bool_t fIsClosed; // Info whether its file was closed
};

// inline functions

inline void TMCRootManager::SetDebug(Bool_t debug) { fgDebug = debug; }

inline Bool_t TMCRootManager::GetDebug() { return fgDebug; }

#endif // ROOT_TMCRootManager
