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
#include "TMCRNTupleWriter.h"
#include "TMCRNTupleParallelWriter.h"
#include "TMCTTreeWriter.h"

#include <Rtypes.h>

class TFile;

/// \brief The Root IO manager for VMC examples for both sequential and
/// multi-threaded applications.
///
/// It facilitates use of ROOT IO in VMC examples and also handles necessary
/// locking in multi-threaded applications.

class TMCRootManager {
public:
   /// Root file mode
   enum FileMode {
      kRead, // Read mode
      kWrite // Write mode
   };
   enum StorageMode {
      kTTree,  // TTree storage
      kRNTuple, // RNTuple storage
      kRNTupleParallel // RNTuple storage with parallel write
   };

public:
   // static access method
   static TMCRootManager *Instance();

   // static method for activating debug mode
   static void SetDebug(Bool_t debug);
   static Bool_t GetDebug();
   static void SetStorageMode(StorageMode mode);
   static StorageMode GetStorageMode();
   static TString GetFileModifier(StorageMode mode);

   TMCRootManager(const char *projectName, FileMode fileMode = kWrite, Int_t threadRank = -1);
   virtual ~TMCRootManager();

   // methods
   template <typename T>
   void Register(const char *name, T *&obj);
   void Register(const char *name, const char *className, void *objAddress);
   void Register(const char *name, const char *className, const void *objAddress);
   void CreateRNTuple();
   void Fill();
   void WriteAll();
   void Close();
   void WriteAndClose();
   void ReadEvent(Int_t i);

   // get methods
   TString GetFileModifier() const;
   TMCVNtupleWriter* GetNtupleWriter() {return fNtupleWriter; }

private:
   // not implemented
   TMCRootManager(const TMCRootManager &rhs);
   TMCRootManager &operator=(const TMCRootManager &rhs);

   // global static data members
   static Int_t fgCounter; // The counter of instances
   // static data members
   static Bool_t fgDebug; // Option to activate debug printings
   static StorageMode fgStorageMode;

#if !defined(__CINT__)
   static TMCThreadLocal TMCRootManager *fgInstance; // singleton thread local instance
#else
   static TMCRootManager *fgInstance; // singleton instance
#endif

   // Methods
   void OpenFile(const char *projectName, FileMode fileMode, Int_t threadRank);

   // data members
   Int_t fId{0};          // This manager ID
   TFile *fFile{nullptr}; // Root output file
   TMCVNtupleWriter* fNtupleWriter{nullptr}; // ntuple manager
   Bool_t fIsClosed{false}; // Info whether its file was closed
};

// inline functions

inline void TMCRootManager::SetDebug(Bool_t debug)
{
   fgDebug = debug;
}

inline Bool_t TMCRootManager::GetDebug()
{
   return fgDebug;
}

inline TMCRootManager::StorageMode TMCRootManager::GetStorageMode()
{
   return fgStorageMode;
}

template <typename T>
void TMCRootManager::Register(const char *brname, T *&obj)
{
   if (fgStorageMode == kTTree) {
      dynamic_cast<TMCTTreeWriter*>(fNtupleWriter)->Register<T>(brname, obj);
   }
   if (fgStorageMode == kRNTuple) {
      dynamic_cast<TMCRNTupleWriter*>(fNtupleWriter)->Register<T>(brname, obj);
   }
   if (fgStorageMode == kRNTupleParallel) {
      dynamic_cast<TMCRNTupleParallelWriter*>(fNtupleWriter)->Register<T>(brname, obj);
   }
}

#endif // ROOT_TMCRootManager
