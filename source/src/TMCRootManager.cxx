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

#include <atomic>
#include <cstdio>
#include <thread>
#include <vector>


namespace {
// Define mutexes per operation which modify shared data
TMCMutex createMutex = TMCMUTEX_INITIALIZER;
TMCMutex deleteMutex = TMCMUTEX_INITIALIZER;

// A global counter to assign numbers sequentially
std::atomic<int> global_thread_counter{0};

int get_clean_thread_id() {
    // This variable is unique to each thread.
    // It initializes ONLY the first time this function is called on that thread.
    thread_local int my_id = global_thread_counter++;
    return my_id;
}

void threadWorker() {
    // No arguments passed, but the thread can still get its 0, 1, 2 ID
    std::cout << "Thread " << std::this_thread::get_id()
              << " assigned itself Custom ID: " << get_clean_thread_id() << "\n";
}



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
   : TMCRootManager(projectName, TMCRootManager::kTTree, fileMode, threadRank)
{
}

//_____________________________________________________________________________
TMCRootManager::TMCRootManager(const char *projectName, TMCRootManager::StorageMode storageMode, TMCRootManager::FileMode fileMode, Int_t threadRank)
   : fStorageMode(storageMode)
{
   /// Standard constructor
   /// \param projectName  The project name (passed as the Root tree name)
   /// \param fileMode     Option for opening Root file (read or write mode)
   /// \param threadRank   >0 when MT mode, -1 when sequential mode

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
   TString fileName = Form("T%s",projectName);
   if (fStorageMode == kRNTuple) {
      fileName = Form("R%s",projectName);
   }

   if (threadRank > 0) {
      Int_t threadId = get_clean_thread_id();
      fileName += "_";
      fileName += threadId;
   }
   fileName += ".root";

   TString treeTitle(projectName);
   treeTitle += " tree";

   switch (fileMode) {
   case TMCRootManager::kRead:
      fFile = new TFile(fileName);
      if (fStorageMode == kTTree)
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
      if (fStorageMode == kTTree)
         fTree = new TTree(projectName, treeTitle);
      if (fStorageMode == kRNTuple) {
         fStorageName = projectName;
         fModel = RNTupleModel::Create();
      }
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
void TMCRootManager::CreateRNTuple()
{
   if (fStorageMode == kRNTuple) {
      fWriter = RNTupleWriter::Append(std::move(fModel), fStorageName.c_str(), *fFile);
      fEntry = fWriter->GetModel().CreateBareEntry();
      for (auto nameAddress : fNameAddress) {
         fEntry->BindRawPtr(nameAddress.first, nameAddress.second);
      }
   }
}

//_____________________________________________________________________________
void TMCRootManager::Fill()
{
   if (fStorageMode == kTTree) {
      /// Fill the Root tree.
      fFile->cd();
      fTree->Fill();
   }
   else if (fStorageMode == kRNTuple) {
      /// Fill the RNTuple.
      RNTupleFillStatus status;
      fWriter->FillNoFlush(*fEntry, status);
      if (status.ShouldFlushCluster()) {
         // If we are asked to flush, first try to do as much work as possible outside of the critical section:
         // FlushColumns() will flush column data and trigger compression, but not actually write to storage.
         // (A framework may of course also decide to flush more often.)
         fWriter->FlushColumns();
         {
            // FlushCluster() will flush data to the underlying TFile, so it requires synchronization.
            fWriter->FlushCluster();
         }
      }
   }
}

//_____________________________________________________________________________
void TMCRootManager::WriteAll()
{

   if (fStorageMode == kTTree) {
      /// Write the Root tree in the file.
      fFile->cd();
      fFile->Write();
   }
   else if (fStorageMode == kRNTuple) {
      /// Write the RNTuple in the file.
      fModel.reset();
      fWriter.reset();
   }
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
