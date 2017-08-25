
#define readtree_TT_Cleaned_cxx
#include "readtree_TT.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>

void readtree_TT::Loop(TString key)
{

//	Histo eta phi

	TH2F * ttStubs_eta_phi =  new TH2F("ttStubs_eta_phi"+key,"ttStubs_eta_phi"+key,200,-0.5,1.2, 200,0.6,1.8);
	ttStubs_eta_phi->SetTitle("TTStubs_eta_phi;eta;phi");
	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;


		//      Dimensione vector tipo TTStubs
		Int_t ttStubs_size = TTStubs_modId->size();
		Int_t genParts_size = genParts_eta->size();

		////////CYCLE THROUGH VECTOR EACH EVENT////////////////


		//      Divide GenParts family in 2 sub family, one that generates Stubs in the inner radius
		//		pf first layer (21, 23) and onw with radius between (23,25)

		////  TTStubs

		for (Int_t w = 0; w < ttStubs_size; w++) {

			ttStubs_eta_phi->Fill(TTStubs_eta->at(w),TTStubs_phi->at(w));



		}







      // if (Cut(ientry) < 0) continue;

		if(jentry%10000==0) cout << jentry << endl;



   }
//   FROM HERE, OUT OF ENTRY LOOP

	Int_t lastfull_bin_phi, firstfull_bin_phi, lastfull_bin_eta, firstfull_bin_eta;
	Float_t lastfull_bin_phi_value, firstfull_bin_phi_value, lastfull_bin_eta_value, firstfull_bin_eta_value;
	lastfull_bin_eta = ttStubs_eta_phi->FindLastBinAbove(0,1); //above 0, for axis 1 = x = eta;
	firstfull_bin_eta= ttStubs_eta_phi->FindFirstBinAbove(0,1);

	lastfull_bin_phi = ttStubs_eta_phi->FindLastBinAbove(0,2);
	firstfull_bin_phi= ttStubs_eta_phi->FindFirstBinAbove(0,2);

	lastfull_bin_eta_value = ttStubs_eta_phi->GetXaxis()->GetBinCenter(lastfull_bin_eta);
	lastfull_bin_phi_value = ttStubs_eta_phi->GetYaxis()->GetBinCenter(lastfull_bin_phi);

	firstfull_bin_eta_value = ttStubs_eta_phi->GetXaxis()->GetBinCenter(firstfull_bin_eta);
	firstfull_bin_phi_value = ttStubs_eta_phi->GetYaxis()->GetBinCenter(firstfull_bin_phi);

	cout << "range in phi" << "[ " << firstfull_bin_phi_value << " : " << lastfull_bin_phi_value << " ]" <<endl;
	cout << "range in eta" << "[ " << firstfull_bin_eta_value << " : " << lastfull_bin_eta_value << " ]" <<endl;
	ttStubs_eta_phi->Draw();

}
//FROM HERE; OUT OF ::LOOP



//////////////////////////   MAIN   /////////////////////////////////

//Argomenti:
//Gli passo sempre through Loop, e check genuine, richiesti dal .h. Check genuine qui non lo uso.

void readtree_TT_Cleaned_run() {

	//INPUT NTUPLE

	gStyle->SetOptStat(111111);
	TString keyA;
	TString fNameA;
	TString name;


	name="50M_cleaned_TT25_after_phasespacecut"; //master name of file root

	fNameA = "../"+name+".root"; //name of file root to read, with path
	keyA = "_"+name;     //just a key for histos

	//HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK
	//file di output contenente gli istogrammi

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile(name+"_run.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
		return;
	}

	readtree_TT a(fNameA);
	f -> cd();
	a.Loop(keyA);




	cout << f << endl;
	gDirectory->pwd();
	f -> Write();

	cout << "ho scritto" << endl;

//	f -> Close();   //IF CLOSE CANNOT SHOW PLOT, BUT CAN DO OPERATIONS!! OK


	return;
}
