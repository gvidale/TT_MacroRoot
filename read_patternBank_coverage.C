#define read_patternBank_cxx
#include "read_patternBank.h"
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <algorithm>
#include <utility>
#include <iostream>


void read_patternBank::Loop(TString key)
{
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	Long64_t jentry =0;

	Int_t total_patterns = nentries;
	Long64_t total_events = 17165497;
	Int_t event_count=0;
	Int_t pattern_count=0;
	Float_t cover=0;




	TH1F * coverage= new TH1F("coverage"+key,"coverage"+key, total_patterns,0,total_patterns);
	coverage->SetTitle(";patternID;CumulativeCoverage");

	cout << "pino" << endl;
	for (jentry=0; jentry<nentries; ++jentry) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		cover+=frequency;

		coverage->Fill(jentry,cover);









		if (jentry%1000==0) cout << flush << "@";

	}


	coverage->Scale(1/cover);
//	coverage -> Draw("histo");

}




//***** MAIN  *******
void read_patternBank_coverage(){


	gStyle->SetOptStat(111111);
	TString key;
	TString fName;
	const int size = 4;
	TString name[size] ={
//			"root://cmseos.fnal.gov//store/user/gvidale/TT25/TT25_muPU200_fount_sf1_sf1_50_2610_bank",
//			"TT41_muPU200_flowonly_sf1_sf05_pt08_100_2610_bank",
//			"TT41_muPU200_fount_sf1_sf05_100_2610_bank",
//			"TT41_muPU200_fount_sf1_sf1_100_2610_bank",
//			"TT41_muPU200_flowonly_sf1_sf1_pt08_100_2610_bank"};
//
				"TT33_muPU200_flowonly_sf1_sf1_pt08_100_2610_bank",
				"TT33_muPU200_fount_sf1_sf1_100_2610_bank",
				"TT33_muPU200_fount_sf07_sf1_100_2610_bank",
				"TT33_muPU200_flowonly_sf07_sf1_pt08_100_2610_bank"};

//	fName = "../patternBank/"+name+".root"; //name of file root to read, with path
//	key = "_"+name;     //just a key for histos

	//HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK
	//file di output contenente gli istogrammi

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile("TT33_coverage.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
		return;
	}


	for(int r=0; r< size; r++){
		key = "_"+name[r];     //just a key for histos
		read_patternBank a("root://cmsxrootd.fnal.gov//store/user/gvidale/TT33/"+name[r]+".root");
		f->cd();
		a.Loop(key);
		}

	f -> Write();

	//cout << "ho scritto" << endl;

	//	f -> Close();   //IF CLOSE CANNOT SHOW PLOT, BUT CAN DO OPERATIONS!! OK


	return;


}
