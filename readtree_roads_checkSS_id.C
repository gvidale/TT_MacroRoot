/*
Road efficiency & turn on curves

19/07/2017

Giorgio Vidale

 */

#define readtree_roads_cxx
#include "readtree_roads.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include "functions.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"

void readtree_roads::Loop(TString key,Int_t charge)
{

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
	Int_t n_ss;
	Int_t n_stubs;
	Int_t trk_index;
	Int_t stub_index;
	Float_t trkParts_ChargeOverPt;
	int ss;
	float x,y;

	TH2F * ssl1[10];
	char name[20];
	for(int s =0; s<10; ++s){
		sprintf(name,"l1ssid[%3i]",s);
		ssl1[s]= new TH2F(name,name,2000,-50,150,2000,0,150);
		ssl1[s]->SetTitle(TString(name)+";x;y");
		ssl1[s]->SetMarkerColor(s);

	}

	TH1F *ssid = new TH1F ("ssid","ssid",300,0,300);
	TH1F *ss_lay_histo = new TH1F ("ssidhito","ssidhisto",20,0,20);

	Long64_t nbytes = 0, nb = 0;
	//	LOOP EVENTS
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		if (jentry % 500 == 0) cout << "@"<< endl;

		Int_t n_trkparts = trkParts_pt->size();
		for (Int_t t=0; t < n_trkparts; ++t ) {
			if ( (trkParts_pdgId->at(t) == 13 || trkParts_pdgId->at(t) == -13)
					&& trkParts_signal->at(t) && trkParts_intime->at(t) && trkParts_primary->at(t) ) {
				trkParts_ChargeOverPt = float(trkParts_charge->at(t))/float(trkParts_pt->at(t));
				int aux_TT = TrackParametersToTT().get_tt(trkParts_phi->at(t), trkParts_ChargeOverPt,trkParts_eta->at(t),trkParts_vz->at(t));
				if (aux_TT != 41) continue;
			}
		}

			//LOOP OVER STUBS LIST
			n_stubs=TTStubs_modId->size();
			for (Int_t stub_count = 0; stub_count< n_stubs; ++ stub_count){
				trk_index=TTStubs_tpId->at(stub_count);
//				if (trk_index !=0) continue;
				//obtain ssId
				ss=TTStubs_superstripId->at(stub_count);
				int ss_lay=ss/4096;
				ss_lay_histo->Fill(ss_lay);
				//keep only those on layer 0

				if(ss_lay==14 //&& TTStubs_superstripId->at(stub_count)%4096 == 0
						){
					x=TTStubs_r->at(stub_count)*cos(TTStubs_phi->at(stub_count));
					y=TTStubs_r->at(stub_count)*sin(TTStubs_phi->at(stub_count));

					ssl1[(ss%4096)/30]->Fill(x,y);
					ssid->Fill(TTStubs_superstripId->at(stub_count)%4096);
				}

			}

		}
	}
	//

	TCanvas * c[2];
	for(Int_t k=0; k<2 ;++k){
		c[k]=new TCanvas();
	}

	c[0]->cd();
	//c[0]->SetFixedAspectRatio();
	for(int s =0; s<10; ++s){
		ssl1[s]->GetXaxis()->SetRangeUser(-10,50);
		ssl1[s]->GetYaxis()->SetRangeUser(10,70);
		ssl1[s]->Draw("same");
	}

	c[1]->cd();

	ssid->Draw();
}




//***** MAIN  *******
void readtree_roads_checkSS_id(){


	gStyle->SetOptStat(111111);
	TString key;
	TString fName;
	TString name;


	//	name="roads_Tilted_tt25_sf1_nz1_pt3_100k_TT25sample"; //master name of file root
	name="roads_TT41_flower_pt12_pos_sf1_nz1_nr1_sf0.9_128K_0819"; //PU200 (8400 evt)
//	name="roads_mu_PU0_TT25_50M_sf1_nz1_0719"; //only mu ((10000 evt)

	fName = "../roads/"+name+".root"; //name of file root to read, with path
	key = "_"+name;     //just a key for histos

	//HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK
	//file di output contenente gli istogrammi

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile(name+"checkssid.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
		return;
	}


	readtree_roads a(fName);
	f -> cd();
	a.Loop(key,1); //l'int e' per selezionar la charge.

	//	gDirectory->pwd();
	f -> Write();
	//
	cout << "ho scritto" << endl;

	//	f -> Close();   //IF CLOSE CANNOT SHOW PLOT, BUT CAN DO OPERATIONS!! OK


	return;


}

