#ifndef ROOT_TMCVNtupleWriter
#define ROOT_TMCVNtupleWriter

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCVNtupleWriter.h
/// \brief Definition of the TMCVNtupleWriter class
///
/// \author I. Hrivnacova; IJCLab Orsay

#include <Rtypes.h>

class TFile;

class TMCVNtupleWriter 
{
public:
   TMCVNtupleWriter(const char *storageName, TFile* file, Bool_t workerMode) :
     fStorageName(storageName), fFile(file), fWorkerMode(workerMode) {}   
   TMCVNtupleWriter() = default;
   virtual ~TMCVNtupleWriter() = default;

   // Standard virtual interface for the rest of the lifecycle
   virtual void CreateRNTuple() = 0;
   virtual void Fill() = 0;
   virtual void WriteAll() = 0;
   virtual void Close() = 0;
   virtual void ReadEvent(Int_t /*i*/) {}

protected:
   TFile* fFile{nullptr};
   std::string fStorageName{};
   Bool_t fWorkerMode{false};
};

#endif // ROOT_TMCVNtupleWriter
