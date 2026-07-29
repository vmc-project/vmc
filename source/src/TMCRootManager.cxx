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
#include "TMCAutoLock.h"

#include <Riostream.h>
#include <TError.h>
#include <TFile.h>

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

int get_clean_thread_id()
{
   // This variable is unique to each thread.
   // It initializes ONLY the first time this function is called on that thread.
   thread_local int my_id = ++global_thread_counter;
   return my_id;
}

void threadWorker()
{
   // No arguments passed, but the thread can still get its 0, 1, 2 ID
   std::cout << "Thread " << std::this_thread::get_id() << " assigned itself Custom ID: " << get_clean_thread_id()
             << "\n";
}

} // namespace

//
// static data, methods
//

Int_t TMCRootManager::fgCounter = 0;
Bool_t TMCRootManager::fgDebug = false;
TMCRootManager::StorageMode TMCRootManager::fgStorageMode = kTTree;
TMCThreadLocal TMCRootManager *TMCRootManager::fgInstance = nullptr;

//_____________________________________________________________________________
TMCRootManager *TMCRootManager::Instance()
{
   /// \return The singleton instance.

   return fgInstance;
}

//_____________________________________________________________________________
void TMCRootManager::SetStorageMode(StorageMode storageMode)
{
  /// \return Set storage mode only if available

#if (ROOT_VERSION_CODE < ROOT_VERSION(6, 38, 0))
   // Check if selected storage mode 
      if (storageMode == kRNTuple || storageMode == kRNTupleParallel) {
         Error("SetStorageMode", "kRNTuple mode is available only with ROOT versin >= 6.38/00");
         return;
      }
#endif

   fgStorageMode = storageMode;
}

//_____________________________________________________________________________
TString TMCRootManager::GetFileModifier(StorageMode storageMode)
{
  // According to the storage mode returns a prefix that can be use
  // to define a unique file name per strorage type. 

  TString fileModifier = "T";
  if (storageMode == kRNTuple) fileModifier = "R";
  if (storageMode == kRNTupleParallel) fileModifier = "PR";

  return fileModifier;
}

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRootManager::TMCRootManager(const char *projectName, TMCRootManager::FileMode fileMode, Int_t threadRank)
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
   // The Id on master and in sequential mode is always 0
   // The Id on workers are >0.
   fId = fgCounter;

   // Increment counter
   ++fgCounter;

   // Check if an instance already exists
   if (fgInstance != nullptr) {
      Fatal("TMCRootManager", "Attempt to create two instances of singleton.");
      return;
   }

   fgInstance = this;

   // SingleThreaded or MT worker with TTree output or MT main thread
   // open file and create a tree
   OpenFile(projectName, fileMode, threadRank);

   // unlock mutex
   lk.unlock();

   if (fgDebug)
      printf("Done TMCRootManager::TMCRootManager %p \n", this);
}

//_____________________________________________________________________________
TMCRootManager::~TMCRootManager()
{
   /// Destructor

   if (fgDebug)
      printf("TMCRootManager::~TMCRootManager %p \n", this);

   // lock mutex
   TMCAutoLock lk(&deleteMutex);

   if (fFile && !fIsClosed) {
      fFile->Close();
   }
   delete fFile;

   delete fNtupleWriter;

   --fgCounter;

   // unlock mutex
   lk.unlock();

   if (fgDebug)
      printf("Done TMCRootManager::~TMCRootManager %p \n", this);
}

//
// private methods
//

//_____________________________________________________________________________
void TMCRootManager::OpenFile(const char *projectName, FileMode fileMode, Int_t threadRank)
{
   if (fgDebug)
      printf("TMCRootManager::OpenFile %p \n", this);

   // add thread Id to fileName
   TString fileName(projectName);
   if (threadRank > 0) {
      fileName += "_";
      if (fileMode == kWrite) {
        Int_t threadId = get_clean_thread_id();
        fileName += threadId;
      }
      if (fileMode == kRead) {
        fileName += threadRank;
      }
   }
   fileName += ".root";
   std::cout << "TMCRootManager::OpenFile: " << fileName << std:: endl;

   TString option;
   if (fileMode == kWrite) option = "recreate";
   if (fileMode == kRead) option = "read";
   Bool_t isWorker = threadRank>0;

   if ( (!isWorker) || fgStorageMode != kRNTupleParallel || fileMode == kRead) {
      // Create file unless we are on worker when parallelWrite is active
      if (fgDebug)
         printf("Going to open Root file \n");
      fFile = new TFile(fileName, option);
      if (fgDebug)
         printf("Done: file %p \n", fFile);
   }

   if (fgStorageMode == kTTree) {
      fNtupleWriter = new TMCTTreeWriter(projectName, fFile, threadRank>0);
      if (fgDebug)
        printf("Created TTreeWriter \n");   
   }
   if (fgStorageMode == kRNTuple) {
      fNtupleWriter = new TMCRNTupleWriter(projectName, fFile, threadRank>0);
      if (fgDebug)
        printf("Created RNTupleWriter \n");   
   }
   if (fgStorageMode == kRNTupleParallel) {
      fNtupleWriter = new TMCRNTupleParallelWriter(projectName, fFile, threadRank>0);
      if (fgDebug)
        printf("Created RNTupleParallelWriter \n");   
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

   if (fgStorageMode == kRNTuple || fgStorageMode == kRNTupleParallel) {
      Error("Register", "Register methods with 'className' are available only with 'kTTree' storage mode.");
      return;
   }
     
   dynamic_cast<TMCTTreeWriter*>(fNtupleWriter)->Register(name, className, objAddress);
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
   /// RNTuple must be create after registering all the user data to RNTupleModel
   /// For kTTree storage mode this function is empty.

   fNtupleWriter->CreateRNTuple();
}

//_____________________________________________________________________________
void TMCRootManager::Fill()
{
   fNtupleWriter->Fill();
}

//_____________________________________________________________________________
void TMCRootManager::WriteAll()
{
   fNtupleWriter->WriteAll();
}

//_____________________________________________________________________________
void TMCRootManager::Close()
{
   /// Close the Root file.

   if ((fId > 0) && (fgStorageMode == kRNTupleParallel)) {
      // Do nothing if in parallel mode on worker
      return;
   }

   if (fIsClosed) {
      Error("Close", "The file was already closed.");
      return;
   }

   fNtupleWriter->Close();
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
   /// \param i  The event(entry) to be read

   fNtupleWriter->ReadEvent(i);
}
