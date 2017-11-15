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
	Float_t cover;




	TH1F * coverage= new TH1F("coverage","coverage", total_patterns,0,total_patterns);
	coverage->SetTitle(";patternID;CumulativeCoverage");

	cout << "pino" << endl;
	for (jentry=0; jentry<nentries; ++jentry) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;


		event_count++;
		pattern_count += frequency;
		cover = (float(pattern_count) / float(total_events)) * (1 - float(total_patterns) / float(total_events));

		cout << " coverage" << cover << endl;
		coverage->Fill(jentry,cover);








		if (jentry%1000==0) cout << flush << "@";

	}


	coverage -> Draw("histo");

}




//***** MAIN  *******
void read_patternBank_coverage(){


	gStyle->SetOptStat(111111);
	TString key;
	TString fName;
	TString name;


	name="patternBankTilted_tt25_sf1_nz1_pt3_attributes_0712"; //master name of file root

	fName = "../patternBank/"+name+".root"; //name of file root to read, with path
	key = "_"+name;     //just a key for histos

	//HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK
	//file di output contenente gli istogrammi

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile(name+"_coverage.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
		return;
	}


	read_patternBank a(fName);
	cout << "pino" << endl;
	f -> cd();
	a.Loop(key);

	//gDirectory->pwd();
	f -> Write();

	//cout << "ho scritto" << endl;

	//	f -> Close();   //IF CLOSE CANNOT SHOW PLOT, BUT CAN DO OPERATIONS!! OK


	return;


}
