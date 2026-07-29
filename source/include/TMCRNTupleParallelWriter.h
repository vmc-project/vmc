#ifndef ROOT_TMCRNTupleParallelWriter
#define ROOT_TMCRNTupleParallelWriter

//------------------------------------------------
// The Geant4 Virtual Monte Carlo package
// Copyright (C) 2013 - 2018 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: root-vmc@cern.ch
//-------------------------------------------------

/// \file TMCRNTupleParallelWriter.h
/// \brief Definition of the TMCRNTupleParallelWriter class
///
/// Refactored from TMCRootManager class updated for RNTuple output
/// by R. Karabowitz; GSI
///
/// \author I. Hrivnacova; IJCLab Orsay

#include <RVersion.h>

#if (ROOT_VERSION_CODE >= ROOT_VERSION(6, 38, 0))

#include "TMCVNtupleWriter.h"

#include <ROOT/REntry.hxx>
#include <ROOT/RNTupleFillContext.hxx>
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleParallelWriter.hxx>
#include <ROOT/RNTupleReader.hxx>

using ROOT::RNTuple;
using ROOT::RNTupleParallelWriter;
using REntry = ROOT::REntry;
using RNTupleModel = ROOT::RNTupleModel;
using RNTupleFillContext = ROOT::RNTupleFillContext;
using RNTParaWriter = ROOT::RNTupleParallelWriter;

class TMCRNTupleParallelWriter : public TMCVNtupleWriter {
public:
   TMCRNTupleParallelWriter(const char *storageName, TFile* file, Bool_t workerMode);
   TMCRNTupleParallelWriter() = delete;  
   virtual ~TMCRNTupleParallelWriter() = default;

   template <typename T>
   void Register(const char *name, T *&obj);

   // Standard virtual interface for the rest of the lifecycle
   virtual void CreateRNTuple() override;
   virtual void Fill() override;
   virtual void WriteAll() override;
   virtual void Close() override;

private:
   // static data members
   static std::shared_ptr<RNTParaWriter> fgParaWriter;

   // data members
   std::unique_ptr<RNTuple> fRNTuple;
   std::vector<std::pair<std::string, void *>> fNameAddress;
   std::unique_ptr<REntry> fEntry;
   std::unique_ptr<RNTupleModel> fModel;
   std::shared_ptr<RNTupleFillContext> fFillContext;
};

template <typename T>
void TMCRNTupleParallelWriter::Register(const char *brname, T *&obj)
{
   if (fModel) {
      fModel->MakeField<T>(brname);
   }
   fNameAddress.push_back(std::make_pair(std::string(brname), obj));
}

#endif // ROOT_VERSION_CODE

#endif // ROOT_TMCRNTupleParallelWriter
