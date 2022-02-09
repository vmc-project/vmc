//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRootManager.cxx
/// \brief Implementation of the TMCRootManager class
///
/// \author I. Hrivnacova; IPN Orsay

#include "TMCRootManager.h"
#include "Riostream.h"
#include "TError.h"
#include "TFile.h"
#include "TMCAutoLock.h"
#include "TThread.h"
#include "TTree.h"

#include <cstdio>

namespace {
// Define mutexes per operation which modify shared data
TMCMutex createMutex = TMCMUTEX_INITIALIZER;
TMCMutex deleteMutex = TMCMUTEX_INITIALIZER;
} // namespace

//
// static data, methods
//

Int_t TMCRootManager::fgCounter = 0;
Bool_t TMCRootManager::fgDebug = false;
TMCThreadLocal TMCRootManager *TMCRootManager::fgInstance = 0;

//_____________________________________________________________________________
TMCRootManager *TMCRootManager::Instance()
{
   /// \return The singleton instance.

   return fgInstance;
}

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRootManager::TMCRootManager(const char *projectName, TMCRootManager::FileMode fileMode, Int_t threadRank)
   : fFile(0), fTree(0), fIsClosed(false)
{
   /// Standard constructor
   /// \param projectName  The project name (passed as the Root tree name)
   /// \param fileMode     Option for opening Root file (read or write mode)
   /// \param threadRank   The thread Id (-1 when sequential mode)

   if (fgDebug)
      printf("TMCRootManager::TMCRootManager %p \n", this);

   // lock mutex
   TMCAutoLock lk(&createMutex);

   // Set Id
   fId = fgCounter;

   // Increment counter
   ++fgCounter;

   // singleton instance
   if (fgInstance) {
      Fatal("TMCRootManager", "Attempt to create two instances of singleton.");
      return;
   }

   fgInstance = this;

   // open file and create a tree
   OpenFile(projectName, fileMode, threadRank);

   // unlock mutex
   lk.unlock();

   if (fgDebug)
      printf("Done TMCRootManagerMT::TMCRootManagerMT %p \n", this);
}

//_____________________________________________________________________________
TMCRootManager::~TMCRootManager()
{
   /// Destructor

   if (fgDebug)
      printf("TMCRootManager::~TMCRootManager %p \n", this);

   // lock mutex
   TMCAutoLock lk(&deleteMutex);

   if (fFile && !fIsClosed)
      fFile->Close();
   delete fFile;

   --fgCounter;

   // unlock mutex
   lk.unlock();

   if (fgDebug)
      printf("Done TMCRootManager::~TMCRootManager %p \n", this);
}

//
// privatemethods
//

//_____________________________________________________________________________
void TMCRootManager::OpenFile(const char *projectName, FileMode fileMode, Int_t threadRank)
{
   TString fileName(projectName);
   if (threadRank >= 0) {
      fileName += "_";
      fileName += threadRank;
   }
   fileName += ".root";

   TString treeTitle(projectName);
   treeTitle += " tree";

   switch (fileMode) {
   case TMCRootManager::kRead:
      fFile = new TFile(fileName);
      fTree = (TTree *)fFile->Get(projectName);
      break;

   case TMCRootManager::kWrite:
      if (fgDebug)
         printf("Going to create Root file \n");
      fFile = new TFile(fileName, "recreate");
      if (fgDebug)
         printf("Done: file %p \n", fFile);

      if (fgDebug)
         printf("Going to create TTree \n");
      fTree = new TTree(projectName, treeTitle);
      if (fgDebug)
         printf("Done: TTree %p \n", fTree);
      ;
      ;
   }
}

//
// public methods
//

//_____________________________________________________________________________
void TMCRootManager::Register(const char *name, const char *className, void *objAddress)
{
   /// Create a branch and associates it with the given address.
   /// \param name       The branch name
   /// \param className  The class name of the object
   /// \param objAddress The object address

   fFile->cd();
   if (!fTree->GetBranch(name))
      fTree->Branch(name, className, objAddress, 32000, 99);
   else
      fTree->GetBranch(name)->SetAddress(objAddress);
}

//_____________________________________________________________________________
void TMCRootManager::Register(const char *name, const char *className, const void *objAddress)
{
   /// Create a branch and associates it with the given address.
   /// \param name       The branch name
   /// \param className  The class name of the object
   /// \param objAddress The object address

   Register(name, className, const_cast<void *>(objAddress));
}

//_____________________________________________________________________________
void TMCRootManager::Fill()
{
   /// Fill the Root tree.

   fFile->cd();
   fTree->Fill();
}

//_____________________________________________________________________________
void TMCRootManager::WriteAll()
{
   /// Write the Root tree in the file.

   fFile->cd();
   fFile->Write();
}

//_____________________________________________________________________________
void TMCRootManager::Close()
{
   /// Close the Root file.

   if (fIsClosed) {
      Error("Close", "The file was already closed.");
      return;
   }

   fFile->cd();
   fFile->Close();
   fIsClosed = true;
}

//_____________________________________________________________________________
void TMCRootManager::WriteAndClose()
{
   /// Write the Root tree in the file and close the file

   WriteAll();
   Close();
}

//_____________________________________________________________________________
void TMCRootManager::ReadEvent(Int_t i)
{
   /// Read the event data for \em i -th event for all connected branches.
   /// \param i  The event to be read

   fTree->GetEntry(i);
}
