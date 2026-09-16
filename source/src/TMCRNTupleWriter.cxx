//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRNTupleWriter.cxx
/// \brief Implementation of the TMCRNTupleWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include "TMCRNTupleWriter.h"

#include <TFile.h>

#if (ROOT_VERSION_CODE >= ROOT_VERSION(6, 38, 0))

//
// ctors, dtor
//

//_____________________________________________________________________________
TMCRNTupleWriter::TMCRNTupleWriter(const char *storageName, TFile* file, Bool_t workerMode)
 : TMCVNtupleWriter(storageName, file, workerMode)
 {
   fModel = RNTupleModel::Create();
 }   

//
// public methods
//

//_____________________________________________________________________________
void TMCRNTupleWriter::CreateRNTuple()
{
   if (!fFile) {
      Error("TMCRNTupleWriter", "File not found, cannot create rntuple.");
      return;
   }

   fWriter = RNTupleWriter::Append(std::move(fModel), fStorageName.c_str(), *fFile);
   fEntry = fWriter->GetModel().CreateBareEntry();
   for (auto nameAddress : fNameAddress) {
      fEntry->BindRawPtr(nameAddress.first, nameAddress.second);
   }
}

//_____________________________________________________________________________
void TMCRNTupleWriter::Fill()
{
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

//_____________________________________________________________________________
void TMCRNTupleWriter::WriteAll()
{
   /// Write the RNTuple in the file.
   fModel.reset();
   fWriter.reset();
}

#endif
