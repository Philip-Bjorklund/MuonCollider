//usage: root -l macroExample.C\(\"inputfile.root\"\,\"outputfile.root\"\)
#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesStream.h"
#include "classes/SortableObject.h"
#include "modules/Delphes.h"
#include "external/ExRootAnalysis/ExRootProgressBar.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTask.h"
#endif
//put header files you need here

void macroExample(const char *inputFile, const char *outputFile){
     gSystem->Load("libDelphes.so");
     TFile *file_sig = new TFile(inputFile);
     TFile *output = new TFile(outputFile, "recreate");
     TTree *tree_sig = (TTree*)file_sig->Get("Delphes");
     TTree *tree_output = new TTree("tree_output","Delphes");

     Int_t nEntries = tree_sig->GetEntries();

     for(Long64_t entry=0; entry < nEntries; entry++){
	 tree_sig->GetEntry(entry);
	 tree_output->GetEntry(entry);
	 //write macro algorithm here
     }
     tree_output->Write();
     output->Close();
     file_sig->Close();
}

