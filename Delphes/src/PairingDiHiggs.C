//usage: root -l PairingDiHiggs.C\(\"inputfile.root\"\,\"outputfile.root\"\)
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

#include <TVector.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TLatex.h>

void PairingDiHiggs(const char *inputFile, const char *outputFile){
     gSystem->Load("libDelphes.so");
     //TChain chain("Delphes");
     //chain.Add(inputFile);
     //ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
     TFile *file_sig = new TFile(inputFile);
     TFile *output = new TFile(outputFile, "recreate");
     TTree *tree_sig = (TTree*)file_sig->Get("Delphes");
     TTree *tree_output = new TTree("tree_output","Delphes");

     cout << "Initiating..." <<endl; 
     
     TLeaf *AKTjet_size = tree_sig->GetLeaf("AKTjet_size");
     TLeaf *AKTjet_eta = tree_sig->GetLeaf("AKTjet.Eta");
     TLeaf *AKTjet_phi = tree_sig->GetLeaf("AKTjet.Phi");
     TLeaf *AKTjet_pt = tree_sig->GetLeaf("AKTjet.PT");
     TLeaf *AKTjet_mass = tree_sig->GetLeaf("AKTjet.Mass");
 
     TLeaf *GenJet_size = tree_sig->GetLeaf("GenJet_size");
     TLeaf *GenJet_eta = tree_sig->GetLeaf("GenJet.Eta");
     TLeaf *GenJet_phi = tree_sig->GetLeaf("GenJet.Phi");
     TLeaf *GenJet_pt = tree_sig->GetLeaf("GenJet.PT");
     TLeaf *GenJet_mass = tree_sig->GetLeaf("GenJet.Mass");

     Int_t nEntries = tree_sig->GetEntries();

     TH1D *AKTjetMass1 = new TH1D("AKTjetMass1", "Anti_KTjet leading jets pair invariant mass", 150 , 0, 400); 
     TH1D *AKTjetMass2 = new TH1D("AKTjetMass2", "Anti_KTjet sub-leading jets pair invariant mass", 150 , 0, 400); 
     TH1D *GenAKTMass2 = new TH1D("GenAKTMass2", "GenAKTMass2", 100 , 0, 600); 
     TH1D *AKTGenMass1Comp = new TH1D("AKTGenMass1Comp", "AKTGenMass1Comp", 200 , -1, 2); 
     TH1D *AKTGenMass2Comp = new TH1D("AKTGenMass2Comp", "AKTGenMass2Comp", 200 , -1, 2); 
     TH1D *GenUncutMass2 = new TH1D("GenUncutMass2", "GenUncutMass2", 100 , 0, 600); 
 
     TH1D *AKTGenPt1Comp = new TH1D("AKTGenPt1Comp", "AKTGenPt1Comp", 200 , -1, 9); 
     TH1D *AKTGenPt2Comp = new TH1D("AKTGenPt2Comp", "AKTGenPt2Comp", 200 , -1, 9); 
     TH1D *AKTGenPt3Comp = new TH1D("AKTGenPt3Comp", "AKTGenPt3Comp", 200 , -1, 9); 
     TH1D *AKTGenPt4Comp = new TH1D("AKTGenPt4Comp", "AKTGenPt4Comp", 200 , -1, 2); 

     TH2D *jet1Reso_Pt = new TH2D("jet1Reso_Pt", "jet1Reso_Pt", 70, 0, 400, 70, -1, 4); 
     TH2D *jet2Reso_Pt = new TH2D("jet2Reso_Pt", "jet2Reso_Pt", 70, 0, 400, 70, -1, 4); 
     TH2D *jet3Reso_Pt = new TH2D("jet3Reso_Pt", "jet3Reso_Pt", 70, 0, 400, 70, -1, 4); 
     TH2D *jet4Reso_Pt = new TH2D("jet4Reso_Pt", "jet4Reso_Pt", 70, 0, 400, 70, -1, 4); 

     TH2D *jet1Reso_DeltaR = new TH2D("jet1Reso_DeltaR", "jet1Reso_DeltaR", 50, 0, 0.5, 50, -1, 4); 
     TH2D *jet2Reso_DeltaR = new TH2D("jet2Reso_DeltaR", "jet2Reso_DeltaR", 50, 0, 0.5, 50, -1, 4); 
     TH2D *jet3Reso_DeltaR = new TH2D("jet3Reso_DeltaR", "jet3Reso_DeltaR", 50, 0, 0.5, 50, -1, 4); 
     TH2D *jet4Reso_DeltaR = new TH2D("jet4Reso_DeltaR", "jet4Reso_DeltaR", 50, 0, 0.5, 50, -1, 4); 

     TH1D *badjet1DeltaR = new TH1D("badjet1DeltaR", "badjet1DeltaR", 100, 0, 0.5); 
     TH1D *badjet2DeltaR = new TH1D("badjet2DeltaR", "badjet2DeltaR", 100, 0, 0.5); 
     TH1D *badjet3DeltaR = new TH1D("badjet3DeltaR", "badjet3DeltaR", 100, 0, 0.5); 
     TH1D *badjet4DeltaR = new TH1D("badjet4DeltaR", "badjet4DeltaR", 100, 0, 0.5); 
     /*
     TH3D *jet1Reso_DeltaR_Pt = new TH3D("jet1Reso_DeltaR_Pt", "jet1Reso_DeltaR_Pt", 20, 0, 400, 20, 0, 0.5, 20, -1, 4); 
     TH3D *jet2Reso_DeltaR_Pt = new TH3D("jet2Reso_DeltaR_Pt", "jet2Reso_DeltaR_Pt", 20, 0, 400, 20, 0, 0.5, 20, -1, 4); 
     TH3D *jet3Reso_DeltaR_Pt = new TH3D("jet3Reso_DeltaR_Pt", "jet3Reso_DeltaR_Pt", 20, 0, 400, 20, 0, 0.5, 20, -1, 4); 
     TH3D *jet4Reso_DeltaR_Pt = new TH3D("jet4Reso_DeltaR_Pt", "jet4Reso_DeltaR_Pt", 20, 0, 400, 20, 0, 0.5, 20, -1, 4); 
*/


     Double_t AKTjet1eta1;
     Double_t AKTjet1phi1;
     Double_t AKTjet1pt1;
     Double_t AKTjet1mass1;
     Double_t AKTjet1eta2;
     Double_t AKTjet1phi2;
     Double_t AKTjet1pt2;
     Double_t AKTjet1mass2;
     Double_t AKTjet2eta1;
     Double_t AKTjet2phi1;
     Double_t AKTjet2pt1;
     Double_t AKTjet2mass1;
     Double_t AKTjet2eta2;
     Double_t AKTjet2phi2;
     Double_t AKTjet2pt2;
     Double_t AKTjet2mass2;
     Double_t AKTjetpairmass;
    
     Double_t Gen1eta;
     Double_t Gen1phi;
     Double_t Gen2eta;
     Double_t Gen2phi;
     Double_t Gen1pt;
     Double_t Gen2pt;
     Double_t Gen1mass;
     Double_t Gen2mass;

     Double_t Gen3eta;
     Double_t Gen3phi;
     Double_t Gen4eta;
     Double_t Gen4phi;
     Double_t Gen3pt;
     Double_t Gen4pt;
     Double_t Gen3mass;
     Double_t Gen4mass;

     Int_t pair1jet1entry;
     Int_t pair1jet2entry;
     Int_t pair2jet1entry;
     Int_t pair2jet2entry;
     
     Int_t AKTjet2entry1;
     Int_t AKTjet2entry2;

     Double_t diHiggsdis;
         
     Double_t AKTjetpair1Mass;
     Double_t AKTjetpair2Mass;

     Int_t AKTjetpaircnt;
     
     TLorentzVector AKTh1;
     TLorentzVector AKTh2;
     TLorentzVector AKTjet1;
     TLorentzVector AKTjet2;
         
     Double_t GenJetMass = 0;
     TLorentzVector Genh2;
     TLorentzVector GenJet1;
     TLorentzVector GenJet2;
     TLorentzVector GenJet3;
     TLorentzVector GenJet4;
     
     Double_t jet1DeltaR;
     Double_t jet2DeltaR;
     Double_t jet3DeltaR;
     Double_t jet4DeltaR;

     cout << "Running Pairing up Algo..." << endl;
//run over event entries
     for(Long64_t entry=0; entry < nEntries; entry++){
	 tree_sig->GetEntry(entry);
	 tree_output->GetEntry(entry);
         AKTjet_size->GetBranch()->GetEntry(entry);

	 Int_t nAKTjet = AKTjet_size->GetValue();
	 Int_t nGenJet = GenJet_size->GetValue();

	 AKTjet_phi->GetBranch()->GetEntry(entry);
	 AKTjet_phi->GetBranch()->GetEntry(entry);
	 AKTjet_pt->GetBranch()->GetEntry(entry);
	 AKTjet_mass->GetBranch()->GetEntry(entry);

	 GenJet_eta->GetBranch()->GetEntry(entry);
	 GenJet_phi->GetBranch()->GetEntry(entry);
	 GenJet_pt->GetBranch()->GetEntry(entry);
	 GenJet_mass->GetBranch()->GetEntry(entry);
         
	 AKTjetpair1Mass = 10000;
	 AKTjetpair2Mass = 10000;
         
	 pair1jet1entry = 0;
         pair1jet2entry = 0;
         pair2jet1entry = 0;
         pair2jet2entry = 0;

         AKTjet2entry1 = 0;
         AKTjet2entry2 = 0;


	 diHiggsdis = 10000;

//Pairing up Anti-kt jets

         Double_t AKTjetpair[nAKTjet*(nAKTjet-1)/2][6];

         AKTjetpaircnt = 0;
	 if (nAKTjet >= 4) {
             //Pairing up leading anti-KT jet pair
	     for (Int_t akt1entry=0; akt1entry < nAKTjet; akt1entry++){
	         for (Int_t akt2entry=akt1entry+1; akt2entry < nAKTjet; akt2entry++){

		     AKTjet1eta1 = AKTjet_eta->GetValue(akt1entry);
         	     AKTjet1phi1 = AKTjet_phi->GetValue(akt1entry);
         	     AKTjet1pt1 = AKTjet_pt->GetValue(akt1entry);
         	     AKTjet1mass1 = AKTjet_mass->GetValue(akt1entry);
                     AKTjet1eta2 = AKTjet_eta->GetValue(akt2entry);
         	     AKTjet1phi2 = AKTjet_phi->GetValue(akt2entry);
         	     AKTjet1pt2 = AKTjet_pt->GetValue(akt2entry);
         	     AKTjet1mass2 = AKTjet_mass->GetValue(akt2entry);
		     AKTjetpairmass = 0;

		     AKTjet1.SetPtEtaPhiM(AKTjet1pt1, AKTjet1eta1, AKTjet1phi1,AKTjet1mass1);
		     AKTjet2.SetPtEtaPhiM(AKTjet1pt2, AKTjet1eta2, AKTjet1phi2,AKTjet1mass2);
		     AKTh1=AKTjet1+AKTjet2;
		     AKTjetpairmass = AKTh1.Mag();

		     AKTjetpair[AKTjetpaircnt][0]=AKTjetpairmass;
		     AKTjetpair[AKTjetpaircnt][1]=akt1entry;
		     AKTjetpair[AKTjetpaircnt][2]=akt2entry;
                     
		     Int_t akt1entry2;
		     Int_t akt2entry2;
		     for (akt1entry2 = 0; akt1entry2 < nAKTjet; akt1entry2++){
	                 if ((akt1entry2 != akt1entry) and (akt1entry2 != akt2entry)){
		             for (akt2entry2 = 0; akt2entry2 < nAKTjet; akt2entry2++){
		                 if (((akt2entry2 != akt1entry) and (akt2entry2 != akt2entry)) and (akt2entry2 != akt1entry)){
			      
			             AKTjet2eta1 = AKTjet_eta->GetValue(akt1entry2);
         	                     AKTjet2phi1 = AKTjet_phi->GetValue(akt1entry2);
         	                     AKTjet2pt1 = AKTjet_pt->GetValue(akt1entry2);
         	                     AKTjet2mass1 = AKTjet_mass->GetValue(akt1entry2);
                                     AKTjet2eta2 = AKTjet_eta->GetValue(akt2entry2);
         	                     AKTjet2phi2 = AKTjet_phi->GetValue(akt2entry2);
         	                     AKTjet2pt2 = AKTjet_pt->GetValue(akt2entry2);
         	                     AKTjet2mass2 = AKTjet_mass->GetValue(akt2entry2);
		                     AKTjetpairmass = 0;

		                     AKTjet1.SetPtEtaPhiM(AKTjet2pt1, AKTjet2eta1, AKTjet2phi1, AKTjet2mass1);
		                     AKTjet2.SetPtEtaPhiM(AKTjet2pt2, AKTjet2eta2, AKTjet2phi2, AKTjet2mass2);
		                     AKTh2=AKTjet1+AKTjet2;
		                     AKTjetpairmass = AKTh2.Mag();
                                     
				     if (abs(125 - AKTjetpairmass) < abs(125 - AKTjetpair2Mass)) {
				         AKTjetpair2Mass = AKTjetpairmass;	     
					 AKTjet2entry1 = akt1entry2;
					 AKTjet2entry2 = akt2entry2;
				     }
		                 }
	                     }  
	                 }        
	             }
	             
                     AKTjetpair[AKTjetpaircnt][3]=AKTjetpair2Mass;
		     AKTjetpair[AKTjetpaircnt][4]=AKTjet2entry1;
		     AKTjetpair[AKTjetpaircnt][5]=AKTjet2entry2;
	         }  

		 AKTjetpaircnt += 1;
	     }
             //cout << AKTjetpaircnt;
	     for (Int_t entry = 0; entry < (nAKTjet-1)*nAKTjet/2; entry++) {
	         if (abs(125 - AKTjetpair[entry][0])*abs(125 - AKTjetpair[entry][3]) < diHiggsdis) {
                     
		     pair1jet1entry =  AKTjetpair[entry][1];
                     pair1jet2entry =  AKTjetpair[entry][2];
                     pair2jet1entry =  AKTjetpair[entry][4];
                     pair2jet2entry =  AKTjetpair[entry][5];

		     AKTjetpair1Mass = AKTjetpair[entry][0];
		     AKTjetpair2Mass = AKTjetpair[entry][3];
		     
		     diHiggsdis = abs(125 - AKTjetpair[entry][0])*abs(125 - AKTjetpair[entry][3]);
		 }		 
	     }
	      
	     AKTjet1eta1 = AKTjet_eta->GetValue(pair1jet1entry);
             AKTjet1phi1 = AKTjet_phi->GetValue(pair1jet1entry);
             AKTjet1pt1 = AKTjet_pt->GetValue(pair1jet1entry);
             AKTjet1mass1 = AKTjet_mass->GetValue(pair1jet1entry);
             AKTjet1eta2 = AKTjet_eta->GetValue(pair1jet2entry);
             AKTjet1phi2 = AKTjet_phi->GetValue(pair1jet2entry);
             AKTjet1pt2 = AKTjet_pt->GetValue(pair1jet2entry);
             AKTjet1mass2 = AKTjet_mass->GetValue(pair1jet2entry);

             AKTjet2eta1 = AKTjet_eta->GetValue(pair2jet1entry);
             AKTjet2phi1 = AKTjet_phi->GetValue(pair2jet1entry);
             AKTjet2pt1 = AKTjet_pt->GetValue(pair2jet1entry);
             AKTjet2mass1 = AKTjet_mass->GetValue(pair2jet1entry);
             AKTjet2eta2 = AKTjet_eta->GetValue(pair2jet2entry);
             AKTjet2phi2 = AKTjet_phi->GetValue(pair2jet2entry);
             AKTjet2pt2 = AKTjet_pt->GetValue(pair2jet2entry);
             AKTjet2mass2 = AKTjet_mass->GetValue(pair2jet2entry);

	     bool AKT1jet1flag = false;
	     bool AKT1jet2flag = false;
             bool AKT2jet1flag = false;
	     bool AKT2jet2flag = false;

             jet1DeltaR = 100;
             jet2DeltaR = 100;
             jet3DeltaR = 100;
             jet4DeltaR = 100;

             Float_t jet1DeltaPhi;
             Float_t jet2DeltaPhi;
             Float_t jet3DeltaPhi;
             Float_t jet4DeltaPhi;

	     Int_t jet1entry;
	     Int_t jet2entry;
	     Int_t jet3entry;
	     Int_t jet4entry;
//Truth matching for anti-KT jets
             for (Int_t gen1entry=0; gen1entry < nGenJet; gen1entry++){
  	         Gen1eta = GenJet_eta->GetValue(gen1entry);
	         Gen1phi = GenJet_phi->GetValue(gen1entry);
		 jet1DeltaPhi = std::abs(AKTjet1phi1-Gen1phi);                
		 if (jet1DeltaPhi > TMath::Pi()) {
		     jet1DeltaPhi -= 2 * TMath::Pi();
		 } 
	         Float_t jet1DeltaRtmp = TMath::Sqrt(pow((AKTjet1eta1-Gen1eta),2)+pow(jet1DeltaPhi,2));
	         if (jet1DeltaRtmp < jet1DeltaR){
	             jet1DeltaR = jet1DeltaRtmp;
		     if (jet1DeltaR < 0.5){
		         //if (abs(Gen1eta) < 2.25){
		             AKT1jet1flag = true;
		             jet1entry = gen1entry;
		         //}
		     }
	         }  
             }
             for (Int_t gen2entry=0; gen2entry < nGenJet; gen2entry++){
  	         Gen2eta = GenJet_eta->GetValue(gen2entry);
	         Gen2phi = GenJet_phi->GetValue(gen2entry);
                 jet2DeltaPhi = std::abs(AKTjet1phi2-Gen2phi);                
		 if (jet2DeltaPhi > TMath::Pi()) {
		     jet2DeltaPhi -= 2 * TMath::Pi();
		 } 
	         Float_t jet2DeltaRtmp = TMath::Sqrt(pow((AKTjet1eta2-Gen2eta),2)+pow(jet2DeltaPhi,2));
	         if (gen2entry != jet1entry){
                     if (jet2DeltaRtmp < jet2DeltaR){
	                 jet2DeltaR = jet2DeltaRtmp;
                         if (jet2DeltaR < 0.5){
		             //if (abs(Gen2eta) < 2.25){
		                 AKT1jet2flag = true;
		                 jet2entry = gen2entry;
			     //}
		         }
	             }
	         }
             }

	     for (Int_t gen3entry=0; gen3entry < nGenJet; gen3entry++){
  	         Gen3eta = GenJet_eta->GetValue(gen3entry);
	         Gen3phi = GenJet_phi->GetValue(gen3entry);
                 jet3DeltaPhi = std::abs(AKTjet2phi1-Gen3phi);                
		 if (jet3DeltaPhi > TMath::Pi()) {
		     jet3DeltaPhi -= 2 * TMath::Pi();
		 } 
	         Float_t jet1DeltaRtmp = TMath::Sqrt(pow((AKTjet2eta1-Gen3eta),2)+pow(jet3DeltaPhi,2));
	         if (jet1DeltaRtmp < jet3DeltaR){
	             jet3DeltaR = jet1DeltaRtmp;
		     if (jet3DeltaR < 0.5){
		         //if (abs(Gen1eta) < 2.25){
		             AKT2jet1flag = true;
		             jet3entry = gen3entry;
		         //}
		     }
	         }  
             }
             for (Int_t gen4entry=0; gen4entry < nGenJet; gen4entry++){
  	         Gen4eta = GenJet_eta->GetValue(gen4entry);
	         Gen4phi = GenJet_phi->GetValue(gen4entry);
                 jet4DeltaPhi = std::abs(AKTjet2phi2-Gen4phi);                
		 if (jet4DeltaPhi > TMath::Pi()) {
		     jet4DeltaPhi -= 2 * TMath::Pi();
		 } 
	         Float_t jet2DeltaRtmp = TMath::Sqrt(pow((AKTjet2eta2-Gen4eta),2)+pow(jet4DeltaPhi,2));
	         if (gen4entry != jet1entry){
                     if (jet2DeltaRtmp < jet4DeltaR){
	                 jet4DeltaR = jet2DeltaRtmp;
                         if (jet4DeltaR < 0.5){
		             //if (abs(Gen2eta) < 2.25){
		                 AKT2jet2flag = true;
		                 jet4entry = gen4entry;
			     //}
		         }
	             }
	         }
             }
	     
	     if (AKTjetpair1Mass < AKTjetpair2Mass) {
	         swap(AKTjetpair1Mass, AKTjetpair2Mass);
		 swap(jet1entry,jet3entry);
		 swap(jet2entry,jet4entry);
	     }
	      
             Gen1eta = GenJet_eta->GetValue(jet1entry);
             Gen1phi = GenJet_phi->GetValue(jet1entry);
             Gen1pt = GenJet_pt->GetValue(jet1entry);
             Gen1mass = GenJet_mass->GetValue(jet1entry);
             Gen2eta = GenJet_eta->GetValue(jet2entry);
             Gen2phi = GenJet_phi->GetValue(jet2entry);
             Gen2pt = GenJet_pt->GetValue(jet2entry);
             Gen2mass = GenJet_mass->GetValue(jet2entry);
         
	     GenJet1.SetPtEtaPhiM(Gen1pt, Gen1eta, Gen1phi,Gen1mass);
	     GenJet2.SetPtEtaPhiM(Gen2pt, Gen2eta, Gen2phi,Gen2mass);
	     Genh2=GenJet1+GenJet2;
	     GenJetMass = Genh2.Mag();
             Double_t AKTGenMass1diff = (AKTjetpair1Mass - GenJetMass)/GenJetMass;

	     Gen3eta = GenJet_eta->GetValue(jet3entry);
             Gen3phi = GenJet_phi->GetValue(jet3entry);
             Gen3pt = GenJet_pt->GetValue(jet3entry);
             Gen3mass = GenJet_mass->GetValue(jet3entry);
             Gen4eta = GenJet_eta->GetValue(jet4entry);
             Gen4phi = GenJet_phi->GetValue(jet4entry);
             Gen4pt = GenJet_pt->GetValue(jet4entry);
             Gen4mass = GenJet_mass->GetValue(jet4entry);
         
	     GenJet3.SetPtEtaPhiM(Gen3pt, Gen3eta, Gen3phi,Gen3mass);
	     GenJet4.SetPtEtaPhiM(Gen4pt, Gen4eta, Gen4phi,Gen4mass);
	     Genh2=GenJet3+GenJet4;
	     GenJetMass = Genh2.Mag();
             Double_t AKTGenMass2diff = (AKTjetpair2Mass - GenJetMass)/GenJetMass;
             
	     Double_t AKTpt[] = { AKTjet1pt1, AKTjet1pt2, AKTjet2pt1, AKTjet2pt2 };
	     //int AKTsize = sizeof(AKTpt) / sizeof(AKTpt[0]);
             
	     Double_t Genpt[] = { Gen1pt, Gen2pt, Gen3pt, Gen4pt };
	     //int Gensize = sizeof(Genpt) / sizeof(Genpt[0]);

	     Double_t deltaR[] = { jet1DeltaR, jet2DeltaR, jet3DeltaR, jet4DeltaR };
             //sort pt 
	     for (int i=0; i<4; i++) {
	         for (int j=i+1; j<4; j++) {
		     if (AKTpt[j] > AKTpt[i]) {
                         swap(AKTpt[j], AKTpt[i]);
			 swap(Genpt[j], Genpt[i]);
			 swap(deltaR[j], deltaR[i]);
		     }
		 }
	     }
	     //calculate resolution
	     Double_t AKTGenPt1diff = (AKTpt[0] - Genpt[0])/Genpt[0];
	     Double_t AKTGenPt2diff = (AKTpt[1] - Genpt[1])/Genpt[1];
	     Double_t AKTGenPt3diff = (AKTpt[2] - Genpt[2])/Genpt[2];
	     Double_t AKTGenPt4diff = (AKTpt[3] - Genpt[3])/Genpt[3];

             if (AKT1jet1flag==true and AKT1jet2flag==true and AKT2jet1flag==true and AKT2jet2flag==true){
	         AKTGenPt1Comp->Fill(AKTGenPt1diff);
		 AKTGenPt2Comp->Fill(AKTGenPt2diff);
		 AKTGenPt3Comp->Fill(AKTGenPt3diff);
		 AKTGenPt4Comp->Fill(AKTGenPt4diff);
		 if (abs(AKTGenPt1diff)>=0.15) {
		     jet1Reso_Pt->Fill(Genpt[0], AKTGenPt1diff);
                     jet1Reso_DeltaR->Fill(deltaR[0], AKTGenPt1diff);
		     badjet1DeltaR->Fill(deltaR[0]);
		     //jet1Reso_DeltaR_Pt->Fill(Genpt[0], deltaR[0], AKTGenPt1diff);
		 }
                 if (abs(AKTGenPt2diff)>=0.15) {
		     jet2Reso_Pt->Fill(Genpt[1], AKTGenPt2diff);
		     jet2Reso_DeltaR->Fill(deltaR[1], AKTGenPt2diff);
		     badjet2DeltaR->Fill(deltaR[1]);
		     //jet2Reso_DeltaR_Pt->Fill(Genpt[1], deltaR[1], AKTGenPt2diff);
		 }
                 if (abs(AKTGenPt3diff)>=0.15) {
		     jet3Reso_Pt->Fill(Genpt[2], AKTGenPt3diff);
		     jet3Reso_DeltaR->Fill(deltaR[2], AKTGenPt3diff);
		     badjet3DeltaR->Fill(deltaR[2]);
		     //jet3Reso_DeltaR_Pt->Fill(Genpt[2], deltaR[2], AKTGenPt3diff);
		 }
                 if (abs(AKTGenPt4diff)>=0.15) {
		     jet4Reso_Pt->Fill(Genpt[3], AKTGenPt4diff);
		     jet4Reso_DeltaR->Fill(deltaR[3], AKTGenPt4diff);
		     badjet4DeltaR->Fill(deltaR[3]);
		     //jet4Reso_DeltaR_Pt->Fill(Genpt[3], deltaR[3], AKTGenPt4diff);
		 }

	         if (abs(AKTGenPt1diff)<0.15 and abs(AKTGenPt2diff)<0.15 and abs(AKTGenPt3diff)<0.15 and abs(AKTGenPt4diff)<0.15) { 
		     AKTjetMass1->Fill(AKTjetpair1Mass);
                     AKTjetMass2->Fill(AKTjetpair2Mass);
		     //GenAKTMass2->Fill(GenJetMass);
	             AKTGenMass1Comp->Fill(AKTGenMass1diff);
	             AKTGenMass2Comp->Fill(AKTGenMass2diff);
		 }
	     } 
             //AKTjetMass1->Fill(AKTjetpair1Mass);
             //AKTjetMass2->Fill(AKTjetpair2Mass);

         }
	 //GenUncutMass2->Fill(GenJetMass);
     }
//Fitting and plotting
     TF1 *jetpair1fit = new TF1("jetpair1fit", "[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[1])/[4])^2)",25,600);
     TF1 *jetpair2fit = new TF1("jetpair2fit", "[0]*exp(-0.5*((x-[1])/[2])^2)+[3]*exp(-0.5*((x-[1])/[4])^2)+expo(5)",25,600);
     TF1 *fSignal = new TF1("fSignal","gaus+gaus(3)",20,600);
     TF1 *fBackground = new TF1("fBackground","expo",20,600);
     TF1 *f1peak = new TF1("f1peak","gaus",20,600);
     TF1 *f2peak = new TF1("f2peak","gaus",20,600);
     Double_t param[8];

     jetpair2fit->SetParameters(200,120,10,20,10,2,-0.0001);
     jetpair2fit->SetParLimits(0,30,100);
     jetpair2fit->SetParLimits(1,100,140);
     jetpair2fit->SetParLimits(2,10,30);
     jetpair2fit->SetParLimits(5,0,20);
     jetpair2fit->SetParLimits(6,-1,-0.0001);
     //jetpair2fit->SetParLimits(4,50,109);
     jetpair2fit->SetParLimits(3,10,50);
     jetpair2fit->SetParLimits(4,0,10);
     
     jetpair1fit->SetParameters(300,120,10,40,20);
     jetpair1fit->SetParLimits(0,50,350);
     jetpair1fit->SetParLimits(1,100,140);
     jetpair1fit->SetParLimits(2,5,30);
     /*jetpair1fit->SetParLimits(6,0,8);
     jetpair1fit->SetParLimits(7,-1.5,-0.0001);*/
     jetpair1fit->SetParLimits(3,0,60);
     jetpair1fit->SetParLimits(4,5,40);
	     
     TCanvas *mycanvas = new TCanvas("mycanvas","My Canvas",200,10,600,480);
     cout <<endl<< "Run gaussian fit for the leading anti-KT jet pair..."<<endl;
     AKTjetMass1->Fit("jetpair1fit","R");
     jetpair1fit->GetParameters(param);
     fSignal->SetParameters(&param[0]);
     fBackground->SetParameters(&param[6]);
     f1peak->SetParameters(&param[0]);
     f2peak->SetParameters(param[3],param[1],param[4]);
     TH1D *AKTjetMass1Signal = new TH1D(*AKTjetMass1);
     AKTjetMass1Signal->Sumw2();
     AKTjetMass1Signal->Add(fBackground,-1);
     AKTjetMass1->GetXaxis()->SetTitle("M [GeV]");
     AKTjetMass1->GetYaxis()->SetTitle("Events");
     AKTjetMass1->Draw();fBackground->SetLineColor(4);fBackground->Draw("SAME");
     f1peak->SetLineColor(3);f1peak->Draw("SAME");
     f2peak->SetLineColor(6);f2peak->Draw("SAME");
     mycanvas->SaveAs("AKTjetpair1Mass.png");

     AKTjetMass2->Fit("jetpair2fit","R");
     jetpair2fit->GetParameters(param);
     fSignal->SetParameters(&param[0]);
     fBackground->SetParameters(&param[5]);
     f1peak->SetParameters(&param[0]);
     f2peak->SetParameters(param[3],param[1],param[4]);
     TH1D *AKTjetMass2Signal = new TH1D(*AKTjetMass2);
     AKTjetMass2Signal->Sumw2();
     AKTjetMass2Signal->Add(fBackground,-1);
     AKTjetMass2->GetXaxis()->SetTitle("M [GeV]");
     AKTjetMass2->GetYaxis()->SetTitle("Events");
     AKTjetMass2->Draw();fBackground->SetLineColor(4);fBackground->Draw("SAME"); 
     f1peak->SetLineColor(3);f1peak->Draw("SAME");
     f2peak->SetLineColor(6);f2peak->Draw("SAME");
     //AKTjetMass2Signal->Draw("SAME"); fSignal->Draw("SAME");
     mycanvas->SaveAs("AKTjetpair2Mass.png");
     cout <<endl<< "Run gaussian fit for the sub-leading GenJet pair under selection of AKT..."<<endl;
     //GenAKTMass2->Fit("jetpair2fit","R");
     //mycanvas->SaveAs("GenAKTMass2.png");
     cout <<endl<< "Run gaussian fit for the sub-leading GenJet pair (Uncut)..."<<endl;
     //GenUncutMass2->Fit("jetpair2fit","R");
     //mycanvas->SaveAs("GenUncutMass2.png");
     
     AKTGenMass1Comp->GetXaxis()->SetTitle("(M_{H1}-M_{Gen})/M_{Gen}");
     AKTGenMass1Comp->GetYaxis()->SetTitle("Events");
     AKTGenMass1Comp->Draw();
     mycanvas->SaveAs("AKTGenMass1Comp.png");
     
     AKTGenMass2Comp->GetXaxis()->SetTitle("(M_{H2}-M_{Gen})/M_{Gen}");
     AKTGenMass2Comp->GetYaxis()->SetTitle("Events");
     AKTGenMass2Comp->Draw();
     mycanvas->SaveAs("AKTGenMass2Comp.png");

     AKTGenPt1Comp->GetXaxis()->SetTitle("(P_{T_1}-P_{T_{Gen1}})/P_{T_{Gen1}}");
     AKTGenPt1Comp->GetYaxis()->SetTitle("Events");
     AKTGenPt1Comp->Draw();
     mycanvas->SaveAs("AKTGenPt1Comp.png");
 
     AKTGenPt2Comp->GetXaxis()->SetTitle("(P_{T_2}-P_{T_{Gen2}})/P_{T_{Gen2}}");
     AKTGenPt2Comp->GetYaxis()->SetTitle("Events");
     AKTGenPt2Comp->Draw();
     mycanvas->SaveAs("AKTGenPt2Comp.png");
 
     AKTGenPt3Comp->GetXaxis()->SetTitle("(P_{T_3}-P_{T_{Gen3}})/P_{T_{Gen3}}");
     AKTGenPt3Comp->GetYaxis()->SetTitle("Events");
     AKTGenPt3Comp->Draw();
     mycanvas->SaveAs("AKTGenPt3Comp.png");
 
     AKTGenPt4Comp->GetXaxis()->SetTitle("(P_{T_4}-P_{T_{Gen4}})/P_{T_{Gen4}}");
     AKTGenPt4Comp->GetYaxis()->SetTitle("Events");
     AKTGenPt4Comp->Draw();
     mycanvas->SaveAs("AKTGenPt4Comp.png");
      
     jet1Reso_Pt->GetXaxis()->SetTitle("P_{T} [GeV]");
     jet1Reso_Pt->GetYaxis()->SetTitle("Resolution (P_{T})");
     jet1Reso_Pt->Draw();//"COLZ");
     mycanvas->SaveAs("jet1Reso_Pt.png");
     
     jet2Reso_Pt->GetXaxis()->SetTitle("P_{T} [GeV]");
     jet2Reso_Pt->GetYaxis()->SetTitle("Resolution (P_{T})");
     jet2Reso_Pt->Draw();//"COLZ");
     mycanvas->SaveAs("jet2Reso_Pt.png");

     jet3Reso_Pt->GetXaxis()->SetTitle("P_{T} [GeV]");
     jet3Reso_Pt->GetYaxis()->SetTitle("Resolution (P_{T})");
     jet3Reso_Pt->Draw();//"COLZ");
     mycanvas->SaveAs("jet3Reso_Pt.png");
 
     jet4Reso_Pt->GetXaxis()->SetTitle("P_{T} [GeV]");
     jet4Reso_Pt->GetYaxis()->SetTitle("Resolution (P_{T})");
     jet4Reso_Pt->Draw();//"COLZ");
     mycanvas->SaveAs("jet4Reso_Pt.png");
     
     jet1Reso_DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     jet1Reso_DeltaR->GetYaxis()->SetTitle("Resolution (P_{T})");   
     jet1Reso_DeltaR->Draw();//"COLZ");
     mycanvas->SaveAs("jet1Reso_DeltaR.png");
 
     jet2Reso_DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     jet2Reso_DeltaR->GetYaxis()->SetTitle("Resolution (P_{T})"); 
     jet2Reso_DeltaR->Draw();//"COLZ");
     mycanvas->SaveAs("jet2Reso_DeltaR.png");

     jet3Reso_DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     jet3Reso_DeltaR->GetYaxis()->SetTitle("Resolution (P_{T})"); 
     jet3Reso_DeltaR->Draw();//"COLZ");
     mycanvas->SaveAs("jet3Reso_DeltaR.png");

     jet4Reso_DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     jet4Reso_DeltaR->GetYaxis()->SetTitle("Resolution (P_{T})");     
     jet4Reso_DeltaR->Draw();//"COLZ");
     mycanvas->SaveAs("jet4Reso_DeltaR.png");

     badjet1DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     badjet1DeltaR->GetYaxis()->SetTitle("Event");   
     badjet1DeltaR->Draw();
     mycanvas->SaveAs("badjet1DeltaR.png");
 
     badjet2DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     badjet2DeltaR->GetYaxis()->SetTitle("Event");   
     badjet2DeltaR->Draw();
     mycanvas->SaveAs("badjet2DeltaR.png");
 
     badjet3DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     badjet3DeltaR->GetYaxis()->SetTitle("Event");   
     badjet3DeltaR->Draw();
     mycanvas->SaveAs("badjet3DeltaR.png");
 
     badjet4DeltaR->GetXaxis()->SetTitle("#Delta_{R}");
     badjet4DeltaR->GetYaxis()->SetTitle("Event");   
     badjet4DeltaR->Draw();
     mycanvas->SaveAs("badjet4DeltaR.png");
/*
     jet1Reso_DeltaR_Pt->Draw("COLZ");
     mycanvas->SaveAs("jet1Reso_DeltaR_Pt.png");
 
     jet2Reso_DeltaR_Pt->Draw("COLZ");
     mycanvas->SaveAs("jet2Reso_DeltaR_Pt.png");
 
     jet3Reso_DeltaR_Pt->Draw("COLZ");
     mycanvas->SaveAs("jet3Reso_DeltaR_Pt.png");
 
     jet4Reso_DeltaR_Pt->Draw("COLZ");
     mycanvas->SaveAs("jet4Reso_DeltaR_Pt.png");
*/
     cout <<endl<< "Output in TTree..."<<endl;

     AKTjetMass1->Write();
     AKTjetMass2->Write();
     //GenAKTMass2->Write();
     AKTGenMass1Comp->Write();
     AKTGenMass2Comp->Write();
     //GenUncutMass2->Write();
     AKTGenPt1Comp->Write();
     AKTGenPt2Comp->Write();
     AKTGenPt3Comp->Write();
     AKTGenPt4Comp->Write();
    
     jet1Reso_Pt->Write();
     jet2Reso_Pt->Write();
     jet3Reso_Pt->Write();
     jet4Reso_Pt->Write();

     jet1Reso_DeltaR->Write();
     jet2Reso_DeltaR->Write();
     jet3Reso_DeltaR->Write();
     jet4Reso_DeltaR->Write();

     badjet1DeltaR->Write();
     badjet2DeltaR->Write();
     badjet3DeltaR->Write();
     badjet4DeltaR->Write();
/*
     jet1Reso_DeltaR_Pt->Write();
     jet2Reso_DeltaR_Pt->Write();
     jet3Reso_DeltaR_Pt->Write();
     jet4Reso_DeltaR_Pt->Write();
*/
     tree_output->Write();

     output->Close();
     file_sig->Close();
}


