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
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TCanvas.h>

void twoTauMonoHigg(const char *inputFile, const char *outputFile){
     gSystem->Load("libDelphes.so");
     TFile *file_sig = new TFile(inputFile);
     TFile *output = new TFile(outputFile, "recreate");
     TTree *tree_sig = (TTree*)file_sig->Get("Delphes");
     TTree *tree_output = new TTree("tree_output","Delphes");

     Int_t nEntries = tree_sig->GetEntries();
     TH1F *TEtaH = new TH1F("2TauEta", "2TauEta", 100 , -4, 4);  
     TH1F *TPhiH = new TH1F("2TauPhi", "2TauPhi", 100 , -4, 4); 
     TH1F *TPtH = new TH1F("2TauPt", "2TauPt", 100 , 0, 2000);
 
     TLeaf *TEta = tree_sig->GetLeaf("Particle.Eta");
     TLeaf *TPhi = tree_sig->GetLeaf("Particle.Phi");
     TLeaf *TPt = tree_sig->GetLeaf("Particle.PT");
     TLeaf *TPID = tree_sig->GetLeaf("Particle.PID");
     TLeaf *Tsize = tree_sig->GetLeaf("Particle_size");


     Float_t TauEta;
     Float_t TauPhi;
     Float_t TauPt;
     Float_t PID;
     for(Long64_t entry=0; entry < nEntries; entry++){
        tree_sig->GetEntry(entry);
        tree_output->GetEntry(entry);
        TPID->GetBranch()->GetEntry(entry);
        Int_t nTsize = Tsize->GetValue();
        TEta->GetBranch()->GetEntry(entry);
        TPhi->GetBranch()->GetEntry(entry);
        TPt->GetBranch()->GetEntry(entry);
        for(Long64_t entry1=0; entry1 < nTsize; entry1++){
	   PID = TPID->GetValue(entry1);  
	
        if (PID == 15.0){
	   TauEta = TEta->GetValue(entry1);
	   TauPhi = TPhi->GetValue(entry1);
	   TauPt = TPt->GetValue(entry1);
           TEtaH->Fill(TauEta);
           TPhiH->Fill(TauPhi);
           TPtH->Fill(TauPt);
//	   cout << "find tau" <<endl;
        }
     }
     }
     TCanvas *mycanvas = new TCanvas("mycanvas","My Canvas",1000,1000);
     TEtaH->Draw();
     TEtaH->GetXaxis()->SetTitle("#eta");
     TEtaH->GetYaxis()->SetTitle("number of events");
     mycanvas->SaveAs("TEta.png");
     TPhiH->Draw();
     TPhiH->GetXaxis()->SetTitle("Radians");
     TPhiH->GetYaxis()->SetTitle("number of events");
     mycanvas->SaveAs("TPhi.png");
     TPtH->Draw();
     TPtH->GetXaxis()->SetTitle("GeV");
     TPtH->GetYaxis()->SetTitle("number of events");
     mycanvas->SaveAs("TPt.png");
      
     TEtaH->Write();
     TPhiH->Write();
     TPtH->Write();
     tree_output->Write();
     output->Close();
     file_sig->Close();
}
