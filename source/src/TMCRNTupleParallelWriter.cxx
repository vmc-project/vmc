//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRNTupleParallelWriter.cxx
/// \brief Implementation of the TMCRNTupleParallelWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TMCRNTupleParallelWriter.h"

#include <TFile.h>

#if (ROOT_VERSION_CODE >= ROOT_VERSION(6, 38, 0))

std::shared_ptr<RNTParaWriter> TMCRNTupleParallelWriter::fgParaWriter = nullptr;

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRNTupleParallelWriter::TMCRNTupleParallelWriter(const char *storageName, TFile* file, Bool_t workerMode)
 : TMCVNtupleWriter(storageName, file, workerMode)
 {
   if (!workerMode) {
     fModel = RNTupleModel::Create();
   }
 }   

//
// public methods
//

//_____________________________________________________________________________
void TMCRNTupleParallelWriter::CreateRNTuple()
{
   if (!fWorkerMode) {
      if (!fFile) {
         Error("TMCRNTupleParallelWriter", "File not found, cannot create rntuple.");
         return;
      }
      fgParaWriter = std::shared_ptr<RNTParaWriter>(
         RNTupleParallelWriter::Append(std::move(fModel), fStorageName.c_str(), *fFile).release());
   } else {
      fFillContext = fgParaWriter->CreateFillContext();
      fEntry = fFillContext->CreateEntry();
      for (auto nameAddress : fNameAddress) {
         fEntry->BindRawPtr(nameAddress.first, nameAddress.second);
      }
   }
}

//_____________________________________________________________________________
void TMCRNTupleParallelWriter::Fill()
{
   /// Fill the RNTuple.
   fFillContext->Fill(*fEntry);
}

//_____________________________________________________________________________
void TMCRNTupleParallelWriter::WriteAll()
{
   /// Write the RNTuple in the file.
   fEntry.reset();
   fFillContext.reset();
}

//_____________________________________________________________________________
void TMCRNTupleParallelWriter::Close()
{
   fgParaWriter->CommitDataset();
}

#endif
