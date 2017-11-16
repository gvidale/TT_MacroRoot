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

	TH1F * pino = new TH1F ("pino"+key, "Pino"+key,100,0,100);
   TH2F * invpt = new TH2F("invpt"+key,"invpt"+key,200,0,0.34,200,0,0.15);
   TProfile * prof = new TProfile("prof"+key,"prof"+key,400,-0.34,0.34);
   prof->Sumw2();
   prof->BuildOptions(0,0,"s");
   invpt->SetTitle("1/pT"+key+"; +q/pT ; #rms(q/pt)");
   prof->SetTitle("q/p_{T}"+key+"; q/p_{T} ; rms(q/p_{t})");

	if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   int n=0;
   if (key.Contains("fount")) n=256000;
   else if (key.Contains("flowonly")) n=128000;
   else {cout << "wrong type size"; return;}
   for (Long64_t jentry=0; jentry<n;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      invpt->Fill(invPt_mean, invPt_sigma);
      prof->Fill(invPt_mean, invPt_sigma);
      if(jentry%1000==0) cout <<  flush << "@";
   }

}

void read_patternAttributes_ptres(){
	gStyle->SetOptStat(111111);
	TString key,fName;
	const int size = 4;
	TString name[size] ={
//			"root://cmseos.fnal.gov//store/user/gvidale/TT25/TT25_muPU200_fount_sf1_sf1_50_2610_bank",
			"TT41_muPU200_flowonly_sf1_sf05_pt08_100_2610_bank",
			"TT41_muPU200_fount_sf1_sf05_100_2610_bank",
			"TT41_muPU200_fount_sf1_sf1_100_2610_bank",
			"TT25_muPU200_fount_sf1_sf1_50_2610_bank",
			"TT33_muPU200_flowonly_sf1_sf1_pt08_100_2610_bank",
							"TT33_muPU200_fount_sf1_sf1_100_2610_bank",
			"TT41_muPU200_flowonly_sf1_sf1_pt08_100_2610_bank"};


	bool isfOpen;
	TFile * f = 0; isfOpen = false;
	f = new TFile("aaastudy_invPT.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the file. Exiting..." << endl;
		return;
	}
	for(int r=0; r< size; r++){
	key = "_"+name[r];     //just a key for histos
	read_patternAttributes a("root://cmsxrootd.fnal.gov//store/user/gvidale/TT41/"+name[r]+".root");
	f->cd();
	a.Loop(key);
	}
	f -> Write();

//	return;
}
