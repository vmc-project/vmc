//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCTTreeWriter.cxx
/// \brief Implementation of the TMCTTreeWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TMCTTreeWriter.h"

#include <TError.h>

#include <iostream>

//
// ctors/dtors
//

//_____________________________________________________________________________
TMCTTreeWriter::TMCTTreeWriter(const char *storageName, TFile* file, Bool_t workerMode)
 : TMCVNtupleWriter(storageName, file, workerMode)
{
   if (!fFile) {
      Error("TMCTTreeWriter", "File not found, cannot create tree.");
      return;
   }

   if (fFile->IsWritable()) {
      // we are in writing mode 
      fTree = new TTree(storageName, fStorageName.c_str());
   }
   else {
      // we are in reading mode 
      fTree = (TTree *)fFile->Get(storageName);
      if (!fTree) {
        Error("TMCTTreeWriter", "Failed to get tree %s from the file.", storageName);
      }
   }
}

//
// public methods
//

//_____________________________________________________________________________
void TMCTTreeWriter::Register(const char *name, const char *className, void *objAddress)
{
   if (!fTree) {
      Error("Register", "Tree not found, cannot register data.");
      return;
   }

   // Check if we are in READ mode or WRITE mode
   TBranch* branch = fTree->GetBranch(name);

   if (branch) {
      // --- READING MODE ---
      // The branch exists in the file; attach memory address to read into
      // std::cout << "Going to set branch address: " << name  << " " << objAddress << std::endl;
      branch->SetAddress(objAddress);
   }
   else if (fFile && fFile->IsWritable()) {
      // --- WRITING MODE ---
      // The branch doesn't exist AND file is open for writing; create new branch
      // std::cout << "Going to create branch: " << name << " " << objAddress << std::endl;
      fFile->cd();
      fTree->Branch(name, className, objAddress, 32000, 99);
   }
   else {
      Error("Register", "Branch %s not found in tree for reading!", name);
   }
}
//_____________________________________________________________________________
void TMCTTreeWriter::Register(const char *name, const char *className, const void *objAddress)
{
   /// Create a branch and associates it with the given address.
   /// \param name       The branch name
   /// \param className  The class name of the object
   /// \param objAddress The object address

   Register(name, className, const_cast<void *&>(objAddress));
}

//_____________________________________________________________________________
void TMCTTreeWriter::Fill()
{
   /// Fill the Root tree.
   fFile->cd();
   fTree->Fill();
}

//_____________________________________________________________________________
void TMCTTreeWriter::WriteAll()
{
   /// Write the Root tree in the file.
   fFile->cd();
   fFile->Write();
}

//_____________________________________________________________________________
void TMCTTreeWriter::ReadEvent(Int_t i)
{
   /// Read the event data for \em i -th event for all connected branches.
   /// \param i  The event to be read

   fTree->GetEntry(i);
}
