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

#include "TFile.h"
#include "TTree.h"
#include <ROOT/REntry.hxx>
#include <ROOT/RField.hxx>
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleFillStatus.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleParallelWriter.hxx>
#include <ROOT/RNTupleWriter.hxx>

class TParticle;

using ROOT::RNTupleFillStatus;
using REntry = ROOT::REntry;
using RNTupleModel = ROOT::RNTupleModel;
using RNTupleWriter = ROOT::RNTupleWriter;

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
      kRNTuple // RNTuple storage
   };

public:
   // static access method
   static TMCRootManager *Instance();

   // static method for activating debug mode
   static void SetDebug(Bool_t debug);
   static Bool_t GetDebug();

   TMCRootManager(const char *projectName, FileMode fileMode = kWrite, Int_t threadRank = -1);
   TMCRootManager(const char *projectName, StorageMode storageMode, FileMode fileMode = kWrite, Int_t threadRank = -1);
   virtual ~TMCRootManager();

   // methods
   template<typename T>
   void Register(const char *name, T *&obj);
   void Register(const char *name, const char *className, void *objAddress);
   void Register(const char *name, const char *className, const void *objAddress);
   void Fill();
   void WriteAll();
   void Close();
   void WriteAndClose();
   void ReadEvent(Int_t i);

   void CreateRNTuple();

private:
   // not implemented
   TMCRootManager(const TMCRootManager &rhs);
   TMCRootManager &operator=(const TMCRootManager &rhs);

   // global static data members
   static Int_t fgCounter; // The counter of instances
   // static data members
   static Bool_t fgDebug; // Option to activate debug printings

#if !defined(__CINT__)
   static TMCThreadLocal TMCRootManager *fgInstance; // singleton instance
#else
   static TMCRootManager *fgInstance; // singleton instance
#endif

   // Methods
   void OpenFile(const char *projectName, FileMode fileMode, Int_t threadRank);

   // data members
   Int_t fId;        // This manager ID
   TFile *fFile {0};     // Root output file
   TTree *fTree {0};     // Root output tree

   std::string fStorageName {};
   std::vector<std::pair<std::string, void*>> fNameAddress;
   std::unique_ptr<REntry> fEntry;
   std::unique_ptr<RNTupleModel> fModel;
   std::unique_ptr<RNTupleWriter> fWriter;

   StorageMode fStorageMode {kTTree};

   Bool_t fIsClosed {false}; // Info whether its file was closed
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

template<typename T>
void TMCRootManager::Register(const char* brname, T*& obj)
{
   if (fStorageMode == kTTree) {
      fFile->cd();
      if (!fTree->GetBranch(brname))
         fTree->Branch(brname, &obj, 32000, 99);
      else
         fTree->GetBranch(brname)->SetAddress(&obj);
   }
   if (fStorageMode == kRNTuple) {
      if (fModel) {
         fModel->MakeField<T>(brname);
      }
      std::string oString;
      oString = brname;
      fNameAddress.push_back(std::make_pair(oString, obj));
   }
}

#endif // ROOT_TMCRootManager
