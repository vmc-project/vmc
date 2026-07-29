#ifndef ROOT_TMCRNTupleWriter
#define ROOT_TMCRNTupleWriter

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRNTupleWriter.h
/// \brief Definition of the TMCRNTupleWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include <RVersion.h>

#if (ROOT_VERSION_CODE >= ROOT_VERSION(6, 38, 0))

#include "TMCVNtupleWriter.h"

#include <ROOT/REntry.hxx>
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleFillStatus.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriter.hxx>

using ROOT::RNTupleFillStatus;
using REntry = ROOT::REntry;
using RNTupleModel = ROOT::RNTupleModel;
using RNTupleWriter = ROOT::RNTupleWriter;
using RNTupleFillContext = ROOT::RNTupleFillContext;

class TMCRNTupleWriter : public TMCVNtupleWriter {
public:
   TMCRNTupleWriter(const char *storageName, TFile* file, Bool_t workerMode);
   TMCRNTupleWriter() = delete;  
   virtual ~TMCRNTupleWriter() = default;

   template <typename T>
   void Register(const char *name, T *&obj);

   // Standard virtual interface for the rest of the lifecycle
   virtual void CreateRNTuple() override;
   virtual void Fill() override;
   virtual void WriteAll() override;
   virtual void Close() override {}

private:
   // data members
   std::vector<std::pair<std::string, void *>> fNameAddress;
   std::unique_ptr<REntry> fEntry;
   std::unique_ptr<RNTupleModel> fModel;
   std::unique_ptr<RNTupleWriter> fWriter;
   std::shared_ptr<RNTupleFillContext> fFillContext;
};

template <typename T>
void TMCRNTupleWriter::Register(const char *brname, T *&obj)
{
   if (fModel) {
      fModel->MakeField<T>(brname);
   }
   fNameAddress.push_back(std::make_pair(std::string(brname), obj));
}

#endif // ROOT_VERSION_CODE

#endif // ROOT_TMCRNTupleWriter
