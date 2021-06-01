//usage: root -l Cs_Ecm.C\(\"outputfile.root\"\)
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
#include <iostream>
#include <fstream>
#include <vector>
//put header files you need here
/*struct data_t{
    string IS;
    string FS;
    int Ecm;
    double Cs;
    double dCs;
};
*/
/*
std::istream &operator>>(std::istream &ist, data_t &data){
    char comma = ',' ;
    ist >> data.IS >> comma
	>> data.FS >> comma
	>> data.Ecm >> comma
	>> data.Cs >> comma
	>> data.dCs
    ;
    return ist;
}
*/
void Cs_Ecm(const char *outputFile){
     std::ifstream data("mm-cs-data.csv");
     std::string line;
     std::vector<std::vector<string>> datavect;
     while(std::getline(data,line)){
         //data_t data;
	 std::stringstream lineStream(line);
	 std::string cell;
	 std::vector<std::string> parsedRow;
	 while (std::getline(lineStream,cell,',')){
             parsedRow.push_back(cell);
	 }
	 datavect.push_back(parsedRow);
     }
     Int_t nEntries = datavect.size();
     TFile *output = new TFile(outputFile, "recreate");
     TTree *tree_output = new TTree("tree_output","Cs_Ecm");

     std::vector<double> aaCsVec;
     std::vector<double> ffCsVec;
     std::vector<double> mmhCsVec;
     std::vector<double> mmhhCsVec;
     std::vector<double> mmzCsVec;
     std::vector<double> zhCsVec;
     std::vector<double> zzCsVec;
     std::vector<double> vvaCsVec;
     std::vector<double> vvhCsVec;
     std::vector<double> vvhhCsVec;
     std::vector<double> vvzCsVec;
     std::vector<double> wwCsVec;
     std::vector<double> zaCsVec;

     std::vector<double> aaEcmVec;
     std::vector<double> ffEcmVec;
     std::vector<double> mmhEcmVec;
     std::vector<double> mmhhEcmVec;
     std::vector<double> mmzEcmVec;
     std::vector<double> zhEcmVec;
     std::vector<double> zzEcmVec;
     std::vector<double> vvaEcmVec;
     std::vector<double> vvhEcmVec;
     std::vector<double> vvhhEcmVec;
     std::vector<double> vvzEcmVec;
     std::vector<double> wwEcmVec;
     std::vector<double> zaEcmVec;
     for (Int_t entry=0; entry< nEntries; entry++){
	if (datavect[entry][1]=="aa") {
	    aaEcmVec.push_back(std::stod(datavect[entry][2]));
	    aaCsVec.push_back(std::stod(datavect[entry][3]));
	}

	if (datavect[entry][1]=="ff") {
	    ffEcmVec.push_back(std::stod(datavect[entry][2]));
	    ffCsVec.push_back(std::stod(datavect[entry][3]));
	}

	if (datavect[entry][1]=="mmh") {
	    mmhEcmVec.push_back(std::stod(datavect[entry][2]));
	    mmhCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="mmhh") {
            mmhhEcmVec.push_back(std::stod(datavect[entry][2]));
	    mmhhCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="mmz") {
            mmzEcmVec.push_back(std::stod(datavect[entry][2]));
	    mmzCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="zh") {
            zhEcmVec.push_back(std::stod(datavect[entry][2]));
	    zhCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="zz") {
            zzEcmVec.push_back(std::stod(datavect[entry][2]));
	    zzCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="vva") {
            vvaEcmVec.push_back(std::stod(datavect[entry][2]));
	    vvaCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="vvh") {
	    vvhEcmVec.push_back(std::stod(datavect[entry][2]));
	    vvhCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="vvhh") {
	    vvhhEcmVec.push_back(std::stod(datavect[entry][2]));
	    vvhhCsVec.push_back(std::stod(datavect[entry][3]));
	}
				
	if (datavect[entry][1]=="vvz") {
	    vvzEcmVec.push_back(std::stod(datavect[entry][2]));
	    vvzCsVec.push_back(std::stod(datavect[entry][3]));
	
	}
	
	if (datavect[entry][1]=="ww") {
	    wwEcmVec.push_back(std::stod(datavect[entry][2]));
	    wwCsVec.push_back(std::stod(datavect[entry][3]));
	}
	
	if (datavect[entry][1]=="za") {
            zaEcmVec.push_back(std::stod(datavect[entry][2]));
	    zaCsVec.push_back(std::stod(datavect[entry][3]));
	}
     }
     Double_t aaCs[aaCsVec.size()];
     Double_t ffCs[aaCsVec.size()];
     Double_t mmhCs[aaCsVec.size()];
     Double_t mmhhCs[aaCsVec.size()];
     Double_t mmzCs[aaCsVec.size()];
     Double_t zhCs[aaCsVec.size()];
     Double_t zzCs[aaCsVec.size()];
     Double_t vvaCs[aaCsVec.size()];
     Double_t vvhCs[aaCsVec.size()];
     Double_t vvhhCs[aaCsVec.size()];
     Double_t vvzCs[aaCsVec.size()];
     Double_t wwCs[aaCsVec.size()];
     Double_t zaCs[aaCsVec.size()];
     Double_t aaEcm[aaEcmVec.size()];
     Double_t ffEcm[aaEcmVec.size()];
     Double_t mmhEcm[aaEcmVec.size()];
     Double_t mmhhEcm[aaEcmVec.size()];
     Double_t mmzEcm[aaEcmVec.size()];
     Double_t zhEcm[aaEcmVec.size()];
     Double_t zzEcm[aaEcmVec.size()];
     Double_t vvaEcm[aaEcmVec.size()];
     Double_t vvhEcm[aaEcmVec.size()];
     Double_t vvhhEcm[aaEcmVec.size()];
     Double_t vvzEcm[aaEcmVec.size()];
     Double_t wwEcm[aaEcmVec.size()];
     Double_t zaEcm[aaEcmVec.size()];
     std::copy(aaEcmVec.begin(), aaEcmVec.end(), aaEcm);
     std::copy(ffEcmVec.begin(), ffEcmVec.end(), ffEcm);
     std::copy(mmhEcmVec.begin(), mmhEcmVec.end(), mmhEcm);
     std::copy(mmhhEcmVec.begin(), mmhhEcmVec.end(), mmhhEcm);
     std::copy(mmzEcmVec.begin(), mmzEcmVec.end(), mmzEcm);
     std::copy(zhEcmVec.begin(), zhEcmVec.end(), zhEcm);
     std::copy(zzEcmVec.begin(), zzEcmVec.end(), zzEcm);
     std::copy(vvaEcmVec.begin(), vvaEcmVec.end(), vvaEcm);
     std::copy(vvhEcmVec.begin(), vvhEcmVec.end(), vvhEcm);
     std::copy(vvhhEcmVec.begin(), vvhhEcmVec.end(), vvhhEcm);
     std::copy(vvzEcmVec.begin(), vvzEcmVec.end(), vvzEcm);
     std::copy(wwEcmVec.begin(), wwEcmVec.end(), wwEcm);
     std::copy(zaEcmVec.begin(), zaEcmVec.end(), zaEcm);
     std::copy(aaCsVec.begin(), aaCsVec.end(), aaCs);
     std::copy(ffCsVec.begin(), ffCsVec.end(), ffCs);
     std::copy(mmhCsVec.begin(), mmhCsVec.end(), mmhCs);
     std::copy(mmhhCsVec.begin(), mmhhCsVec.end(), mmhhCs);
     std::copy(mmzCsVec.begin(), mmzCsVec.end(), mmzCs);
     std::copy(zhCsVec.begin(), zhCsVec.end(), zhCs);
     std::copy(zzCsVec.begin(), zzCsVec.end(), zzCs);
     std::copy(vvaCsVec.begin(), vvaCsVec.end(), vvaCs);
     std::copy(vvhCsVec.begin(), vvhCsVec.end(), vvhCs);
     std::copy(vvhhCsVec.begin(), vvhhCsVec.end(), vvhhCs);
     std::copy(vvzCsVec.begin(), vvzCsVec.end(), vvzCs);
     std::copy(wwCsVec.begin(), wwCsVec.end(), wwCs);
     std::copy(zaCsVec.begin(), zaCsVec.end(), zaCs);
     TGraph *aa = new TGraph (7, aaEcm, aaCs); 
     TGraph *ff = new TGraph (7, ffEcm, ffCs); 
     TGraph *mmh = new TGraph (7, mmhEcm, mmhCs); 
     TGraph *mmhh = new TGraph (7, mmhhEcm, mmhhCs); 
     TGraph *mmz = new TGraph (7, mmzEcm, mmzCs); 
     TGraph *zh = new TGraph (7, zhEcm, zhCs); 
     TGraph *zz = new TGraph (7, zzEcm, zzCs); 
     TGraph *vva = new TGraph (7, vvaEcm, vvaCs); 
     TGraph *vvh = new TGraph (7, vvhEcm, vvhCs); 
     TGraph *vvhh = new TGraph (7, vvhhEcm, vvhhCs); 
     TGraph *vvz = new TGraph (7, vvzEcm, vvzCs); 
     TGraph *ww = new TGraph (7, wwEcm, wwCs); 
     TGraph *za = new TGraph (7, zaEcm, zaCs); 

     auto canvas = new TCanvas("canvas","Cross Section vs. Center of Mass Energy", 1000, 1000);
     auto pad = new TPad("pad", "", 0, 0, 1, 1);
     //pad->SetGrid();
     pad->Draw();
     pad->cd();
     aa->SetLineColor(1);
     ff->SetLineColor(920);
     mmh->SetLineColor(632);
     mmhh->SetLineColor(416);
     mmz->SetLineColor(600);
     zh->SetLineColor(400);
     zz->SetLineColor(616);
     vva->SetLineColor(900);
     vvh->SetLineColor(800);
     vvhh->SetLineColor(820);
     vvz->SetLineColor(840);
     ww->SetLineColor(860);
     za->SetLineColor(880);
     aa->SetLineWidth(3);
     ff->SetLineWidth(3);
     mmh->SetLineWidth(3);
     mmhh->SetLineWidth(3);
     mmz->SetLineWidth(3);
     zh->SetLineWidth(3);
     zz->SetLineWidth(3);
     vva->SetLineWidth(3);
     vvh->SetLineWidth(3);
     vvhh->SetLineWidth(3);
     vvz->SetLineWidth(3);
     ww->SetLineWidth(3);
     za->SetLineWidth(3);
     
     canvas->cd();
     TMultiGraph *mg = new TMultiGraph();
     mg->Add(aa);
     mg->Add(ff);
     mg->Add(mmh);
     mg->Add(mmh);
     mg->Add(mmz);
     mg->Add(zh);
     mg->Add(zz);
     mg->Add(vva);
     mg->Add(vvh);
     mg->Add(vvhh);
     mg->Add(vvz);
     mg->Add(ww);
     mg->Add(za);
     mg->SetTitle("Cross Section vs. Center of Mass Energy for Different Channels");
     mg->Draw("ALP");
     canvas->SetGrid();
     canvas->SetLogy(1);
     mg->GetXaxis()->SetTitle("E_{CM} [TeV]");
     mg->GetYaxis()->SetTitle("#sigma(#mu^{+}#mu^{-}#rightarrow X) [pb]");
     TLegend *legend = new TLegend(0.15,0.15,0.23, 0.4);
     legend->SetHeader("Channels");
     legend->AddEntry(aa,"#gamma#gamma","l");
     legend->AddEntry(ff,"ff","l");
     legend->AddEntry(mmh,"#mu#muh","l");
     legend->AddEntry(mmhh,"#mu#muhh","l");
     legend->AddEntry(mmz,"#mu#muZ","l");
     legend->AddEntry(zh,"Zh","l");
     legend->AddEntry(zz,"ZZ","l");
     legend->AddEntry(vva,"#nu#nu#gamma ","l");
     legend->AddEntry(vvh,"#nu#nuh","l");
     legend->AddEntry(vvhh,"#nu#nuhh","l");
     legend->AddEntry(vvz,"#nu#nuZ","l");
     legend->AddEntry(ww,"WW","l");
     legend->AddEntry(za,"Z#gamma","l");
     legend->Draw();
     canvas->cd();
     canvas->SaveAs("Cs_Ecm.png"); 
     aa->Write();
     ff->Write();
     mmh->Write();
     mmhh->Write();
     mmz->Write();
     zh->Write();
     zz->Write();
     vva->Write();
     vvh->Write();
     vvhh->Write();
     vvz->Write();
     ww->Write();
     za->Write();

     tree_output->Write();
     output->Close();
}

