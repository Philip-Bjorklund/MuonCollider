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

     TH1D *AKTjetMass1 = new TH1D("AKTjetMass1", "Anti_KTjet leading jets pair invariant mass", 200 , 0, 600); 
     TH1D *AKTjetMass2 = new TH1D("AKTjetMass2", "Anti_KTjet sub-leading jets pair invariant mass", 200 , 0, 600); 
     TH1D *GenAKTMass2 = new TH1D("GenAKTMass2", "GenAKTMass2", 100 , 0, 600); 
     TH1D *AKTGenMass2Comp = new TH1D("AKTGenMass2Comp", "AKTGenMass2Comp", 100 , -1.5, 1.5); 
     
     TH1D *GenUncutMass2 = new TH1D("GenUncutMass2", "GenUncutMass2", 100 , 0, 600); 
 
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
	     for(Int_t akt1entry=0; akt1entry < nAKTjet; akt1entry++){
	         for(Int_t akt2entry=akt1entry+1; akt2entry < nAKTjet; akt2entry++){

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
		     for(akt1entry2 = 0; akt1entry2 < nAKTjet; akt1entry2++){
	                 if((akt1entry2 != akt1entry) and (akt1entry2 != akt2entry)){
		             for(akt2entry2 = 0; akt2entry2 < nAKTjet; akt2entry2++){
		                 if(((akt2entry2 != akt1entry) and (akt2entry2 != akt2entry)) and (akt2entry2 != akt1entry)){
			      
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

             Float_t jet1DeltaR = 100;
             Float_t jet2DeltaR = 100;

	     Int_t jet1entry;
	     Int_t jet2entry;
//Truth matching for anti-KT jets
             for(Int_t gen3entry=0; gen3entry < nGenJet; gen3entry++){
  	         Gen1eta = GenJet_eta->GetValue(gen3entry);
	         Gen1phi = GenJet_phi->GetValue(gen3entry);
	         Float_t jet1DeltaRtmp = TMath::Sqrt(pow((AKTjet1eta1-Gen1eta),2)+pow((AKTjet1phi1-Gen1phi),2));
	         if(jet1DeltaRtmp < jet1DeltaR){
	             jet1DeltaR = jet1DeltaRtmp;
		     jet1entry = gen3entry;
		     if (jet1DeltaR < 0.5){
		         //if (abs(Gen1eta) < 2.25){
		             AKT1jet1flag = true;
		         //}
		     }
	         }  
             }
             for(Int_t gen4entry=0; gen4entry < nGenJet; gen4entry++){
  	         Gen2eta = GenJet_eta->GetValue(gen4entry);
	         Gen2phi = GenJet_phi->GetValue(gen4entry);
	         Float_t jet2DeltaRtmp = TMath::Sqrt(pow((AKTjet1eta2-Gen2eta),2)+pow((AKTjet1phi2-Gen2phi),2));
	         if (gen4entry != jet1entry){
                     if(jet2DeltaRtmp < jet2DeltaR){
	                 jet2DeltaR = jet2DeltaRtmp;
		         jet2entry = gen4entry;
                         if (jet2DeltaR < 0.5){
		             //if (abs(Gen2eta) < 2.25){
		                 AKT1jet2flag = true;
			     //}
		         }
	             }
	         }
             }
             jet1DeltaR = 100;
             jet2DeltaR = 100;

	     for(Int_t gen1entry=0; gen1entry < nGenJet; gen1entry++){
  	         Gen1eta = GenJet_eta->GetValue(gen1entry);
	         Gen1phi = GenJet_phi->GetValue(gen1entry);
	         Float_t jet1DeltaRtmp = TMath::Sqrt(pow((AKTjet2eta1-Gen1eta),2)+pow((AKTjet2phi1-Gen1phi),2));
	         if(jet1DeltaRtmp < jet1DeltaR){
	             jet1DeltaR = jet1DeltaRtmp;
		     jet1entry = gen1entry;
		     if (jet1DeltaR < 0.5){
		         //if (abs(Gen1eta) < 2.25){
		             AKT2jet1flag = true;
		         //}
		     }
	         }  
             }
             for(Int_t gen2entry=0; gen2entry < nGenJet; gen2entry++){
  	         Gen2eta = GenJet_eta->GetValue(gen2entry);
	         Gen2phi = GenJet_phi->GetValue(gen2entry);
	         Float_t jet2DeltaRtmp = TMath::Sqrt(pow((AKTjet2eta2-Gen2eta),2)+pow((AKTjet2phi2-Gen2phi),2));
	         if (gen2entry != jet1entry){
                     if(jet2DeltaRtmp < jet2DeltaR){
	                 jet2DeltaR = jet2DeltaRtmp;
		         jet2entry = gen2entry;
                         if (jet2DeltaR < 0.5){
		             //if (abs(Gen2eta) < 2.25){
		                 AKT2jet2flag = true;
			     //}
		         }
	             }
	         }
             }
	     
	     if (AKTjetpair1Mass < AKTjetpair2Mass) {
	         swap(AKTjetpair1Mass, AKTjetpair2Mass);
	     }
	     /* 
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
             Double_t AKTGendiff = (AKTjetpair2Mass-GenJetMass)/GenJetMass;
	     */
	 
             if (AKT1jet1flag==true and AKT1jet2flag==true and AKT2jet1flag==true and AKT2jet2flag==true){
	         
		 AKTjetMass1->Fill(AKTjetpair1Mass);
                 AKTjetMass2->Fill(AKTjetpair2Mass);
		 //GenAKTMass2->Fill(GenJetMass);
	         //AKTGenMass2Comp->Fill(AKTGendiff);
	     } 
             //AKTjetMass1->Fill(AKTjetpair1Mass);
             //AKTjetMass2->Fill(AKTjetpair2Mass);

         }
	 //GenUncutMass2->Fill(GenJetMass);
     }
//Fitting and plotting
     TF1 *jetpair1fit = new TF1("jetpair1fit", "gaus+gaus(3)",25,600);
     TF1 *jetpair2fit = new TF1("jetpair2fit", "gaus+gaus(3)+expo(6)",25,600);
     TF1 *fSignal = new TF1("fSignal","gaus+gaus(3)",20,600);
     TF1 *fBackground = new TF1("fBackground","expo", 20,600);
     TF1 *f1peak = new TF1("f1peak","gaus",20,600);
     TF1 *f2peak = new TF1("f2peak","gaus",20,600);
     Double_t param[8];

     jetpair2fit->SetParameters(200,120,10,20,100,10,2,-0.0001);
     jetpair2fit->SetParLimits(0,80,200);
     jetpair2fit->SetParLimits(1,110,130);
     jetpair2fit->SetParLimits(2,5,25);
     jetpair2fit->SetParLimits(6,0,8);
     jetpair2fit->SetParLimits(7,-1,-0.0001);
     jetpair2fit->SetParLimits(4,50,109);
     jetpair2fit->SetParLimits(5,5,30);
     
     jetpair1fit->SetParameters(300,120,10,40,125,10,2,-0.0001);
     jetpair1fit->SetParLimits(0,100,400);
     jetpair1fit->SetParLimits(1,110,120);
     jetpair1fit->SetParLimits(2,5,30);
     /*jetpair1fit->SetParLimits(6,0,8);
     jetpair1fit->SetParLimits(7,-1.5,-0.0001);*/
     jetpair1fit->SetParLimits(4,120,140);
     jetpair1fit->SetParLimits(5,5,40);


	     
     TCanvas *mycanvas = new TCanvas("mycanvas","My Canvas",200,10,600,480);
     cout <<endl<< "Run gaussian fit for the leading anti-KT jet pair..."<<endl;
     AKTjetMass1->Fit("jetpair1fit","R");
     jetpair1fit->GetParameters(param);
     fSignal->SetParameters(&param[0]);
     fBackground->SetParameters(&param[6]);
     f1peak->SetParameters(&param[0]);
     f2peak->SetParameters(&param[3]);
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
     fBackground->SetParameters(&param[6]);
     f1peak->SetParameters(&param[0]);
     f2peak->SetParameters(&param[3]);
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

     //AKTGenMass2Comp->Draw();
     //mycanvas->SaveAs("AKTGenMass2Comp.png");

     cout <<endl<< "Output in TTree..."<<endl;

     AKTjetMass1->Write();
     AKTjetMass2->Write();
     //GenAKTMass2->Write();
     //AKTGenMass2Comp->Write();
     //GenUncutMass2->Write();

     tree_output->Write();

     output->Close();
     file_sig->Close();
}


