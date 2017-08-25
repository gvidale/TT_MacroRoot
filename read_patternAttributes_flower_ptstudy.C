/*		MACRO TO READ PATTERN ATTRIBUTES
 *
 * 		in particular, this one fills an histogram about 1/pT.
 *
 */
#define read_patternAttributes_cxx
#include "read_patternAttributes.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TProfile.h>
using namespace std;

void read_patternAttributes::Loop(TString key)
{
	cout << "*** " << key << endl;
   TH1F * invpt = new TH1F ("invpt"+key,"",200,0,0.34);
   invpt->SetTitle("1/pT"+key+"; +q/pT [{r}]; #patterns");



	if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      invpt->Fill(invPt_mean);

      if(jentry%1000==0) cout <<  flush << "@";
   }
  
   cout << endl;
}

void read_patternAttributes_flower_ptstudy(){
	gStyle->SetOptStat(111111);
	TString key,fName;
	TString name[5] = {
			"TT41_flower_pt3_pos_sf1_nz1_nr1_sf1_0818.root",
			"TT41_flower_pt5_pos_sf1_nz1_nr1_sf1_0818.root",
			"TT41_flower_pt8_sf1_nz1_nr1_sf1_pos_0818.root",
			"TT41_flower_pt12_pos_sf1_nz1_nr1_sf1_0818.root",
			"TT41_flower_pt13_sf1_nz1_nr1_sf1_pos_0818.root",

	};

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile("flower_sf1_nz1_nr1_sf1_invPT.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the file. Exiting..." << endl;
		return;
	}

	for (int r=0;r<5;++r){
	key = "_"+name[r];     //just a key for histos
	cout << "key: " << key << endl;
	read_patternAttributes a("../patternBank/flowerPTstudy/"+name[r]);
	f -> cd();
	a.Loop(key);
	}
	
	gDirectory->pwd();
	f -> Write();

	return;
}
