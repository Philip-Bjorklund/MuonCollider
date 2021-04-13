//usage: root -l AnalyzeM.C\(\"inputfile.root\"\,\"outputfile.root\"\)
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

void AnalyzeM(const char *inputFile, const char *outputFile){
     gSystem->Load("libDelphes.so");
     //TChain chain("Delphes");
     //chain.Add(inputFile);
     //ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
     TFile *file_sig = new TFile(inputFile);
     TFile *output = new TFile(outputFile, "recreate");
     TTree *tree_sig = (TTree*)file_sig->Get("Delphes");
     TTree *tree_output = new TTree("tree_output","Delphes");

     cout << "Initiating..." <<endl; 

     TLeaf *VLCjetR05N4_size = tree_sig->GetLeaf("VLCjetR05N4_size");
     TLeaf *VLCjetR05N4_eta = tree_sig->GetLeaf("VLCjetR05N4.Eta");
     TLeaf *VLCjetR05N4_phi = tree_sig->GetLeaf("VLCjetR05N4.Phi");
     TLeaf *VLCjetR05N4_pt = tree_sig->GetLeaf("VLCjetR05N4.PT");
     TLeaf *VLCjetR05N4_mass = tree_sig->GetLeaf("VLCjetR05N4.Mass");
     
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

     TH1D *VLCR05N4Mass1 = new TH1D("VLCR05N4Mass1", "VLCR05N4Mass1", 100 , 0, 600); 
     TH1D *VLCR05N4Mass2 = new TH1D("VLCR05N4Mass2", "VLCR05N4Mass2", 100 , 0, 600); 
     TH1D *GenVLCMass2 = new TH1D("GenVLCMass2", "GenVLCMass2", 100 , 0, 600); 
     TH1D *VLCGenMass2Comp = new TH1D("VLCGenMass2Comp", "VLCGenMass2Comp", 100 , -1.5, 1.5); 

     TH1D *AKTjetMass1 = new TH1D("AKTjetMass1", "Anti_KTjet leading jets pair invariant mass", 100 , 0, 600); 
     TH1D *AKTjetMass2 = new TH1D("AKTjetMass2", "Anti_KTjet sub-leading jets pair invariant mass", 100 , 0, 600); 
     TH1D *GenAKTMass2 = new TH1D("GenAKTMass2", "GenAKTMass2", 100 , 0, 600); 
     TH1D *AKTGenMass2Comp = new TH1D("AKTGenMass2Comp", "AKTGenMass2Comp", 100 , -1.5, 1.5); 
     
     TH1D *GenUncutMass2 = new TH1D("GenUncutMass2", "GenUncutMass2", 100 , 0, 600); 
 
     Double_t VLC1eta;
     Double_t VLC1phi;
     Double_t VLC1pt;
     Double_t VLC1mass;
     Double_t VLC2eta;
     Double_t VLC2phi;
     Double_t VLC2pt;
     Double_t VLC2mass;
     Double_t VLCR05N4pairmass;
  
     Double_t AKT1eta;
     Double_t AKT1phi;
     Double_t AKT1pt;
     Double_t AKT1mass;
     Double_t AKT2eta;
     Double_t AKT2phi;
     Double_t AKT2pt;
     Double_t AKT2mass;
     Double_t AKTjetpairmass;
         
     Double_t Gen1eta;
     Double_t Gen1phi;
     Double_t Gen2eta;
     Double_t Gen2phi;
     Double_t Gen1pt;
     Double_t Gen2pt;
     Double_t Gen1mass;
     Double_t Gen2mass;

     Int_t pair1jet1entry;
     Int_t pair1jet2entry;
     Int_t pair2jet1entry;
     Int_t pair2jet2entry;

     Double_t VLCR05N4pair1Mass = 0;
     Double_t VLCR05N4pair2Mass = 0;
         
     Double_t AKTjetpair1Mass = 0;
     Double_t AKTjetpair2Mass = 0;

     cout << "Running Pairing up Algo..." << endl;
//run over event entries
     for(Long64_t entry=0; entry < nEntries; entry++){
	 tree_sig->GetEntry(entry);
	 tree_output->GetEntry(entry);
	 VLCjetR05N4_size->GetBranch()->GetEntry(entry);
         AKTjet_size->GetBranch()->GetEntry(entry);

	 Int_t nVLCjet = VLCjetR05N4_size->GetValue();
	 Int_t nAKTjet = AKTjet_size->GetValue();
	 Int_t nGenJet = GenJet_size->GetValue();

	 VLCjetR05N4_phi->GetBranch()->GetEntry(entry);
	 VLCjetR05N4_phi->GetBranch()->GetEntry(entry);
	 VLCjetR05N4_pt->GetBranch()->GetEntry(entry);
	 VLCjetR05N4_mass->GetBranch()->GetEntry(entry);

	 AKTjet_phi->GetBranch()->GetEntry(entry);
	 AKTjet_phi->GetBranch()->GetEntry(entry);
	 AKTjet_pt->GetBranch()->GetEntry(entry);
	 AKTjet_mass->GetBranch()->GetEntry(entry);

	 GenJet_eta->GetBranch()->GetEntry(entry);
	 GenJet_phi->GetBranch()->GetEntry(entry);
	 GenJet_pt->GetBranch()->GetEntry(entry);
	 GenJet_mass->GetBranch()->GetEntry(entry);

	 VLCR05N4pair1Mass = 10000;
	 VLCR05N4pair2Mass = 10000;
         
	 AKTjetpair1Mass = 10000;
	 AKTjetpair2Mass = 10000;
         
	 pair1jet1entry = 0;
         pair1jet2entry = 0;
         pair2jet1entry = 0;
         pair2jet2entry = 0;

//Pairing up Valencia Jets
	 for(Int_t vlc1entry=0; vlc1entry < nVLCjet; vlc1entry++){
	     for(Int_t vlc2entry=vlc1entry+1; vlc2entry < nVLCjet; vlc2entry++){
		 VLC1eta = VLCjetR05N4_eta->GetValue(vlc1entry);
         	 VLC1phi = VLCjetR05N4_phi->GetValue(vlc1entry);
         	 VLC1pt = VLCjetR05N4_pt->GetValue(vlc1entry);
         	 VLC1mass = VLCjetR05N4_mass->GetValue(vlc1entry);
                 VLC2eta = VLCjetR05N4_eta->GetValue(vlc2entry);
         	 VLC2phi = VLCjetR05N4_phi->GetValue(vlc2entry);
         	 VLC2pt = VLCjetR05N4_pt->GetValue(vlc2entry);
         	 VLC2mass = VLCjetR05N4_mass->GetValue(vlc2entry);
		 VLCR05N4pairmass = 0;

                 TLorentzVector VLCh1;
                 TLorentzVector VLCjet1;
                 TLorentzVector VLCjet2;
		 VLCjet1.SetPtEtaPhiM(VLC1pt, VLC1eta, VLC1phi,VLC1mass);
		 VLCjet2.SetPtEtaPhiM(VLC2pt, VLC2eta, VLC2phi,VLC1mass);
		 VLCh1=VLCjet1+VLCjet2;
		 VLCR05N4pairmass = VLCh1.Mag();
		 
		 if(abs(125 - VLCR05N4pairmass) < abs(125 -VLCR05N4pair1Mass)){
		     VLCR05N4pair1Mass = VLCR05N4pairmass;
		     pair1jet1entry = vlc1entry;
		     pair1jet2entry = vlc2entry;

		 }		 	 
	     }  
	 }

	 for(Int_t vlcentry=0; vlcentry < nVLCjet; vlcentry++){
	     if((vlcentry!=pair1jet1entry) and (vlcentry!=pair1jet2entry)){
                 pair2jet1entry = vlcentry;
		 break;
	     }
	 }
         for(Int_t vlcentry=0; vlcentry < nVLCjet; vlcentry++){
	     if(((vlcentry!=pair1jet1entry) and (vlcentry!=pair1jet2entry)) and (vlcentry!=pair2jet1entry)){
	         pair2jet2entry = vlcentry;	     
	     }
	 }
         VLC1eta = VLCjetR05N4_eta->GetValue(pair2jet1entry);
         VLC1phi = VLCjetR05N4_phi->GetValue(pair2jet1entry);
         VLC1pt = VLCjetR05N4_pt->GetValue(pair2jet1entry);
         VLC1mass = VLCjetR05N4_mass->GetValue(pair2jet1entry);
         VLC2eta = VLCjetR05N4_eta->GetValue(pair2jet2entry);
         VLC2phi = VLCjetR05N4_phi->GetValue(pair2jet2entry);
         VLC2pt = VLCjetR05N4_pt->GetValue(pair2jet2entry);
         VLC2mass = VLCjetR05N4_mass->GetValue(pair2jet2entry);

	 bool VLC1flag = false;
	 bool VLC2flag = false;
         Float_t jet1DeltaR = 100;
         Float_t jet2DeltaR = 100;

	 Int_t jet1entry;
	 Int_t jet2entry;
//Truth matching for Valencia jets
	 for(Int_t gen1entry=0; gen1entry < nGenJet; gen1entry++){
  	     Gen1eta = GenJet_eta->GetValue(gen1entry);
	     Gen1phi = GenJet_phi->GetValue(gen1entry);
	     Float_t jet1DeltaRtmp = TMath::Sqrt(pow((VLC1eta-Gen1eta),2)+pow((VLC1phi-Gen1phi),2));
	     if(jet1DeltaRtmp < jet1DeltaR){
	         jet1DeltaR = jet1DeltaRtmp;
		 jet1entry = gen1entry;
		 if (jet1DeltaR < 0.5){
		     if (abs(Gen1eta) < 2.25){
		         VLC1flag = true;
		     }
		 }
	     }  
         }
         for(Int_t gen2entry=0; gen2entry < nGenJet; gen2entry++){
  	     Gen2eta = GenJet_eta->GetValue(gen2entry);
	     Gen2phi = GenJet_phi->GetValue(gen2entry);
	     Float_t jet2DeltaRtmp = TMath::Sqrt(pow((VLC2eta-Gen2eta),2)+pow((VLC2phi-Gen2phi),2));
	     if (gen2entry != jet1entry){
                 if(jet2DeltaRtmp < jet2DeltaR){
	             jet2DeltaR = jet2DeltaRtmp;
		     jet2entry = gen2entry;
                     if (jet2DeltaR < 0.5){
		         if (abs(Gen2eta) < 2.25){
		             VLC2flag = true;
			 }
		     }
	         }
	     }
         }
	 
         //Double_t VLCR05N4pair2Mass = 0;
         TLorentzVector VLCh2;
         TLorentzVector VLCjet1;
         TLorentzVector VLCjet2;
 	 VLCjet1.SetPtEtaPhiM(VLC1pt, VLC1eta, VLC1phi,VLC1mass);
	 VLCjet2.SetPtEtaPhiM(VLC2pt, VLC2eta, VLC2phi,VLC2mass);
	 VLCh2=VLCjet1+VLCjet2;
	 VLCR05N4pair2Mass = VLCh2.Mag();
         
	 Gen1eta = GenJet_eta->GetValue(jet1entry);
         Gen1phi = GenJet_phi->GetValue(jet1entry);
         Gen1pt = GenJet_pt->GetValue(jet1entry);
         Gen1mass = GenJet_mass->GetValue(jet1entry);
         Gen2eta = GenJet_eta->GetValue(jet2entry);
         Gen2phi = GenJet_phi->GetValue(jet2entry);
         Gen2pt = GenJet_pt->GetValue(jet2entry);
         Gen2mass = GenJet_mass->GetValue(jet2entry);
         
	 Double_t GenJetMass = 0;
         TLorentzVector Genh2;
         TLorentzVector GenJet1;
         TLorentzVector GenJet2;
 	 GenJet1.SetPtEtaPhiM(Gen1pt, Gen1eta, Gen1phi,Gen1mass);
	 GenJet2.SetPtEtaPhiM(Gen2pt, Gen2eta, Gen2phi,Gen2mass);
	 Genh2=GenJet1+GenJet2;
	 GenJetMass = Genh2.Mag();
         Double_t VLCGendiff = (VLCR05N4pair2Mass-GenJetMass)/GenJetMass;
         
	 
	 if (VLC1flag==true and VLC2flag==true){
	     VLCR05N4Mass1->Fill(VLCR05N4pair1Mass);
             VLCR05N4Mass2->Fill(VLCR05N4pair2Mass);
	     GenVLCMass2->Fill(GenJetMass);
	     VLCGenMass2Comp->Fill(VLCGendiff);

	 } /*else {
	     cout << "the second jet pair in the "<<entry<< "th event failed check."<<endl;	 
         }*/
//Pairing up Anti-kt jets
         TLorentzVector AKTh1;
         TLorentzVector AKTjet1;
         TLorentzVector AKTjet2;
         TLorentzVector AKTh2;

	 if (nAKTjet == 4) {
             //Pairing up leading anti-KT jet pair
	     for(Int_t akt1entry=0; akt1entry < nAKTjet; akt1entry++){
	         for(Int_t akt2entry=akt1entry+1; akt2entry < nAKTjet; akt2entry++){
		     AKT1eta = AKTjet_eta->GetValue(akt1entry);
         	     AKT1phi = AKTjet_phi->GetValue(akt1entry);
         	     AKT1pt = AKTjet_pt->GetValue(akt1entry);
         	     AKT1mass = AKTjet_mass->GetValue(akt1entry);
                     AKT2eta = AKTjet_eta->GetValue(akt2entry);
         	     AKT2phi = AKTjet_phi->GetValue(akt2entry);
         	     AKT2pt = AKTjet_pt->GetValue(akt2entry);
         	     AKT2mass = AKTjet_mass->GetValue(akt2entry);
		     AKTjetpairmass = 0;

		     AKTjet1.SetPtEtaPhiM(AKT1pt, AKT1eta, AKT1phi,AKT1mass);
		     AKTjet2.SetPtEtaPhiM(AKT2pt, AKT2eta, AKT2phi,AKT1mass);
		     AKTh1=AKTjet1+AKTjet2;
		     AKTjetpairmass = AKTh1.Mag();
		 
		     if(abs(125 - AKTjetpairmass) < abs(125 -AKTjetpair1Mass)){
		         AKTjetpair1Mass = AKTjetpairmass;
		         pair1jet1entry = akt1entry;
		         pair1jet2entry = akt2entry;

		     }		 	 
	         }  
	     }
//Pairing up sub-leading anti-KT jet pair
             Int_t findflag = 0;
	     for(Int_t akt1entry=0; akt1entry < nAKTjet; akt1entry++){
	         if((akt1entry!=pair1jet1entry) and (akt1entry!=pair1jet2entry)){
		     for(Int_t akt2entry=0; akt2entry < nAKTjet; akt2entry++){
		         if(((akt2entry!=pair1jet1entry) and (akt2entry!=pair1jet2entry)) and (akt2entry!=akt1entry)){
			      
			     AKT1eta = AKTjet_eta->GetValue(akt1entry);
         	             AKT1phi = AKTjet_phi->GetValue(akt1entry);
         	             AKT1pt = AKTjet_pt->GetValue(akt1entry);
         	             AKT1mass = AKTjet_mass->GetValue(akt1entry);
                             AKT2eta = AKTjet_eta->GetValue(akt2entry);
         	             AKT2phi = AKTjet_phi->GetValue(akt2entry);
         	             AKT2pt = AKTjet_pt->GetValue(akt2entry);
         	             AKT2mass = AKTjet_mass->GetValue(akt2entry);
		             AKTjetpairmass = 0;

		             AKTjet1.SetPtEtaPhiM(AKT1pt, AKT1eta, AKT1phi,AKT1mass);
		             AKTjet2.SetPtEtaPhiM(AKT2pt, AKT2eta, AKT2phi,AKT1mass);
		             AKTh2=AKTjet1+AKTjet2;
		             AKTjetpairmass = AKTh2.Mag();
		 
		             if(abs(125 - AKTjetpairmass) < abs(125 - AKTjetpair2Mass)){
		                 AKTjetpair2Mass = AKTjetpairmass;
				 pair2jet1entry = akt1entry;
		                 pair2jet2entry = akt2entry;
		             }
	                 }  
	             }        
	         }
	     }

             AKT1eta = AKTjet_eta->GetValue(pair2jet1entry);
             AKT1phi = AKTjet_phi->GetValue(pair2jet1entry);
             AKT1pt = AKTjet_pt->GetValue(pair2jet1entry);
             AKT1mass = AKTjet_mass->GetValue(pair2jet1entry);
             AKT2eta = AKTjet_eta->GetValue(pair2jet2entry);
             AKT2phi = AKTjet_phi->GetValue(pair2jet2entry);
             AKT2pt = AKTjet_pt->GetValue(pair2jet2entry);
             AKT2mass = AKTjet_mass->GetValue(pair2jet2entry);

	     bool AKT1flag = false;
	     bool AKT2flag = false;
             Float_t jet1DeltaR = 100;
             Float_t jet2DeltaR = 100;

	     Int_t jet1entry;
	     Int_t jet2entry;
//Truth matching for anti-KT jets
	     for(Int_t gen1entry=0; gen1entry < nGenJet; gen1entry++){
  	         Gen1eta = GenJet_eta->GetValue(gen1entry);
	         Gen1phi = GenJet_phi->GetValue(gen1entry);
	         Float_t jet1DeltaRtmp = TMath::Sqrt(pow((AKT1eta-Gen1eta),2)+pow((AKT1phi-Gen1phi),2));
	         if(jet1DeltaRtmp < jet1DeltaR){
	             jet1DeltaR = jet1DeltaRtmp;
		     jet1entry = gen1entry;
		     if (jet1DeltaR < 0.5){
		         if (abs(Gen1eta) < 2.25){
		             AKT1flag = true;
		         }
		     }
	         }  
             }
             for(Int_t gen2entry=0; gen2entry < nGenJet; gen2entry++){
  	         Gen2eta = GenJet_eta->GetValue(gen2entry);
	         Gen2phi = GenJet_phi->GetValue(gen2entry);
	         Float_t jet2DeltaRtmp = TMath::Sqrt(pow((AKT2eta-Gen2eta),2)+pow((AKT2phi-Gen2phi),2));
	         if (gen2entry != jet1entry){
                     if(jet2DeltaRtmp < jet2DeltaR){
	                 jet2DeltaR = jet2DeltaRtmp;
		         jet2entry = gen2entry;
                         if (jet2DeltaR < 0.5){
		             if (abs(Gen2eta) < 2.25){
		                 AKT2flag = true;
			     }
		         }
	             }
	         }
             }
	 
	     AKTjet1.SetPtEtaPhiM(AKT1pt, AKT1eta, AKT1phi,AKT1mass);
	     AKTjet2.SetPtEtaPhiM(AKT2pt, AKT2eta, AKT2phi,AKT2mass);
	     AKTh2=AKTjet1+AKTjet2;
	     AKTjetpair2Mass = AKTh2.Mag();
	 
             Gen1eta = GenJet_eta->GetValue(jet1entry);
             Gen1phi = GenJet_phi->GetValue(jet1entry);
             Gen1pt = GenJet_pt->GetValue(jet1entry);
             Gen1mass = GenJet_mass->GetValue(jet1entry);
             Gen2eta = GenJet_eta->GetValue(jet2entry);
             Gen2phi = GenJet_phi->GetValue(jet2entry);
             Gen2pt = GenJet_pt->GetValue(jet2entry);
             Gen2mass = GenJet_mass->GetValue(jet2entry);
         
	     //Double_t GenJetMass = 0;
             //TLorentzVector Genh2;
             //TLorentzVector GenJet1;
             //TLorentzVector GenJet2;
 	     GenJet1.SetPtEtaPhiM(Gen1pt, Gen1eta, Gen1phi,Gen1mass);
	     GenJet2.SetPtEtaPhiM(Gen2pt, Gen2eta, Gen2phi,Gen2mass);
	     Genh2=GenJet1+GenJet2;
	     GenJetMass = Genh2.Mag();
             Double_t AKTGendiff = (AKTjetpair2Mass-GenJetMass)/GenJetMass;
	 
             if (AKT1flag==true and AKT2flag==true){
	         AKTjetMass1->Fill(AKTjetpair1Mass);
                 AKTjetMass2->Fill(AKTjetpair2Mass);
		 GenAKTMass2->Fill(GenJetMass);
	         AKTGenMass2Comp->Fill(AKTGendiff);
	     } 
         }
	 GenUncutMass2->Fill(GenJetMass);
     }
//Fitting and plotting
     TF1 *jetpair1fit = new TF1("jetpair1fit", "gaus",10,200);
     TF1 *jetpair2fit = new TF1("jetpair2fit", "gaus+expo(3)",20,600);
     TF1 *fSignal = new TF1("fSignal","gaus",20,600);
     TF1 *fBackground = new TF1("fBackground","expo",20,600);
     Double_t param[5];

     jetpair2fit->SetParameters(25,125,10,2,-0.0001);
     jetpair2fit->SetParLimits(1,60,150);
     jetpair2fit->SetParLimits(2,10,50);
     jetpair2fit->SetParLimits(3,0,8);
     jetpair2fit->SetParLimits(4,-1,-0.0001);
	     
     TCanvas *mycanvas = new TCanvas("mycanvas","My Canvas",200,10,600,480);
     cout <<endl<< "Run gaussian fit for the leading Valencia jet pair..."<<endl;
     VLCR05N4Mass1->Fit("jetpair1fit","R");
     mycanvas->SaveAs("VLCR05N4pair1Mass.png");
     cout <<endl<< "Run gaussian fit for the sub-leading Valencia jet pair..."<<endl;
     
     VLCR05N4Mass2->Fit("jetpair2fit","R");
     jetpair2fit->GetParameters(param);
     fSignal->SetParameters(&param[0]);
     fBackground->SetParameters(&param[3]);
     TH1D *VLCR05N4Mass2Signal = new TH1D(*VLCR05N4Mass2);
     VLCR05N4Mass2Signal->Sumw2();
     VLCR05N4Mass2Signal->Add(fBackground,-1);
     VLCR05N4Mass2->GetXaxis()->SetTitle("M [GeV]");
     VLCR05N4Mass2->GetYaxis()->SetTitle("Events");
     VLCR05N4Mass2->Draw();fBackground->SetLineColor(4);fBackground->Draw("SAME"); 
     //VLCR05N4Mass2Signal->Draw("SAME"); fSignal->Draw("SAME");
     mycanvas->SaveAs("VLCR05N4pair2Mass.png");
     cout <<endl<< "Run gaussian fit for the sub-leading GenJet pair under selection of VLC..."<<endl;
     //GenVLCMass2->Fit("jetpair2fit","R");
     //mycanvas->SaveAs("GenVLCMass2.png");
     cout <<endl<< "Run gaussian fit for the leading anti-KT jet pair..."<<endl;
     AKTjetMass1->Fit("jetpair1fit","R");
     mycanvas->SaveAs("AKTjetpair1Mass.png");
     cout <<endl<< "Run gaussian fit for the sub-leading anti-KT jet pair..."<<endl;

     AKTjetMass2->Fit("jetpair2fit","R");
     jetpair2fit->GetParameters(param);
     fSignal->SetParameters(&param[0]);
     fBackground->SetParameters(&param[3]);
     TH1D *AKTjetMass2Signal = new TH1D(*AKTjetMass2);
     AKTjetMass2Signal->Sumw2();
     AKTjetMass2Signal->Add(fBackground,-1);
     AKTjetMass2->GetXaxis()->SetTitle("M [GeV]");
     AKTjetMass2->GetYaxis()->SetTitle("Events");
     AKTjetMass2->Draw();fBackground->SetLineColor(4);fBackground->Draw("SAME"); 
     //AKTjetMass2Signal->Draw("SAME"); fSignal->Draw("SAME");
     mycanvas->SaveAs("AKTjetpair2Mass.png");
     cout <<endl<< "Run gaussian fit for the sub-leading GenJet pair under selection of AKT..."<<endl;
     //GenAKTMass2->Fit("jetpair2fit","R");
     //mycanvas->SaveAs("GenAKTMass2.png");
     cout <<endl<< "Run gaussian fit for the sub-leading GenJet pair (Uncut)..."<<endl;
     //GenUncutMass2->Fit("jetpair2fit","R");
     //mycanvas->SaveAs("GenUncutMass2.png");


     //VLCGenMass2Comp->Draw();
     //mycanvas->SaveAs("VLCGenMass2Comp.png");
     //AKTGenMass2Comp->Draw();
     //mycanvas->SaveAs("AKTGenMass2Comp.png");

     cout <<endl<< "Output in TTree..."<<endl;

     VLCR05N4Mass1->Write();
     VLCR05N4Mass2->Write();
     //GenVLCMass2->Write();
     //VLCGenMass2Comp->Write();
     AKTjetMass1->Write();
     AKTjetMass2->Write();
     //GenAKTMass2->Write();
     //AKTGenMass2Comp->Write();
     //GenUncutMass2->Write();

     tree_output->Write();

     output->Close();
     file_sig->Close();
}


