#ifndef ROOT_TMCTTreeWriter
#define ROOT_TMCTTreeWriter

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCTTreeWriter.h
/// \brief Definition of the TMCTTreeWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TMCVNtupleWriter.h"

#include <TFile.h>
#include <TTree.h>

class TMCTTreeWriter : public TMCVNtupleWriter {
public:
   TMCTTreeWriter(const char *storageName, TFile* file, Bool_t workerMode);
   TMCTTreeWriter() = delete;  
   virtual ~TMCTTreeWriter() = default;

   template <typename T>
   void Register(const char *name, T *&obj);
   void Register(const char *name, const char *className, void *objAddress);
   void Register(const char *name, const char *className, const void *objAddress);

   // Standard virtual interface for the rest of the lifecycle
   virtual void CreateRNTuple() override {}
   virtual void Fill() override;
   virtual void WriteAll() override;
   virtual void Close() override {}
   virtual void ReadEvent(Int_t i) override;

private:
    TTree* fTree{nullptr};
};

#include <iostream>

template <typename T>
void TMCTTreeWriter::Register(const char *brname, T *&obj)
{
   if (!fTree) return;

   fFile->cd();
   if (!fTree->GetBranch(brname)) {
      std::cout << "Create branch " << brname << std::endl;
      fTree->Branch(brname, &obj, 32000, 99);
   }
   else
      fTree->GetBranch(brname)->SetAddress(&obj);
}

#endif // ROOT_TMCTTreeWriter
