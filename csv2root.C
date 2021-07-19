#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

void testcsv() {

   TString dir = gSystem->UnixPathName(__FILE__);
   dir.ReplaceAll("UScsvToRoot.C","");
   dir.ReplaceAll("/./","/");

   TFile *f = new TFile("MuonData.root","RECREATE");
   TTree *tree = new TTree("ntuple","data from csv file");
   tree->ReadFile("RealData.csv","Muon/I",',');
   f->Write();
 }
