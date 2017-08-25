#define readtree_roads_cxx
#include "readtree_roads.h"
#include <TH2.h>
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>

void readtree_roads::Loop(TString key, Int_t charge)
{
   if (fChain == 0) return;

   TH2F * n_stub_ss_lay = new TH2F ("n_stub_ss_lay","",11,0,11,512,0,512); //maybe could ignore the first bin 0 and the last above zero in y when drawing.
   n_stub_ss_lay->SetTitle("n_stub_ss_lay; layer; ssId");

   TH2F * n_stub_ss_r[11];
   char name[100];
   for(int r =0; r<11; ++r){
	   sprintf(name, "occupancy_(norm_dphi),_r_vs_ssId,_layer_%1i", r);
	   n_stub_ss_r[r] = new TH2F(name,name,250,0,250,200,19,110);
	   n_stub_ss_r[r]->SetTitle(TString(name)+";ssId;r[cm]");
   }

   float phiWidths_ [16]= {
		   0.00381*2, 0.00439, 0.00459, 0.00485, 0.00523, 0.00575,
		   0.0048   , 0.0050 , 0.0058 , 0.0064 , 0.0070 , // RR guestimates based on the 1st barrel 2S and scattering studies on disks a-la fountain
		   0.0048   , 0.0050 , 0.0058 , 0.0064 , 0.0070
   };

   cout << "pino"<<endl;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;

	   int lay, ssid, n_stubs, trk_index, stub_index;
	   Int_t n_trkparts = trkParts_pt->size();
	   float trkParts_ChargeOverPt;
	   for (Int_t t=0; t < n_trkparts; ++t ) {
		   if ( (trkParts_pdgId->at(t) == 13 || trkParts_pdgId->at(t) == -13)
				   && trkParts_signal->at(t) && trkParts_intime->at(t) && trkParts_primary->at(t) ) {
			   trkParts_ChargeOverPt = float(trkParts_charge->at(t))/float(trkParts_pt->at(t));
			   int aux_TT = TrackParametersToTT().get_tt(trkParts_phi->at(t), trkParts_ChargeOverPt,trkParts_eta->at(t),trkParts_vz->at(t));
			   if (aux_TT != 41) continue;
		   }
	   }

//layer# 11: 233; % respect to fountain 1.01807 * layer# 12: 219; % respect to fountain 1.02174 *
//layer# 13: 185; % respect to fountain 1.02741 * layer# 14: 163; % respect to fountain 1.03047 *
//layer# 15: 143; % respect to fountain 1.0263 *


	   int n_roads_fired = AMTTRoads_patternRef->size();
	   n_stubs = TTStubs_modId->size();
	   for (int i=0; i<n_stubs; ++i){
		   lay = TTStubs_superstripId->at(i)/4096;
		   ssid = TTStubs_superstripId->at(i)%4096;
		   n_stub_ss_lay->Fill(lay,ssid); //normalize per phiwidth of layer adn number of roads
		   n_stub_ss_r[lay]->Fill(ssid,TTStubs_r->at(i));
		   if(lay>11) cout<<"founf layer > 11 "<<endl;
		   if(ssid>512 ) cout << "found ssid > 512" << endl;
	   }



	   if(jentry%1000==0) cout <<flush<< "@";
   }

   TCanvas * c = new TCanvas();
//   n_stub_ss_lay->SetMaximum(3);
//   n_stub_ss_lay->SetMinimum(1); //so that we can manage ssid 0 and maxium that have more stubs due to under over flow.
   n_stub_ss_lay->GetYaxis()->SetRangeUser(1,228);
   n_stub_ss_lay->Draw("colz");

//   TLegend * l = new TLegend(0.1,0.3,0.9,0.7);
   TCanvas * r = new TCanvas();
   r->Divide(3,2);
   for (int a=6; a< 11; ++a){
	   r->cd(a-5);
	   n_stub_ss_r[a]->GetXaxis()->SetRangeUser(1,228);
	   n_stub_ss_r[a]->Draw("colz");
   }
   cout << endl;

}

void readtree_roads_densitystubs(TString name = "0", TString key = "0",Int_t charge=1){

	gStyle->SetOptStat(0);
	TString fName;
	if(name!="0" && key =="0"){
		cout << "Please provide 'key' string as 2nd arg (sf_XX-64K)" << endl;
		return;
	}
	if(name=="0" && key =="0"){//otherwise put it in there
		name="roads_TT41_flower_pt12_pos_sf1_nz1_nr1_sf0.9_128K_0819";
//		name="old/roads_Tilted_tt25_sf1_nz1_pt3_100k_TT25sample";
		key = "sf09_flower_pt12";
	}


	cout << "______________________________" << endl;
	cout << "FILE INPUT: " << name << endl;

	fName = "../roads/TT41_old/"+name+".root"; //name of file root to read, with path
	if(key.Index("cmseos")!=-1) fName = "root://cmsxrootd.fnal.gov//store/user/gvidale/"+name+".root"; //for eos file in my home
	name = (charge==1)? name+"ch1" : (charge==-1)? name +"ch-1" : (charge==0)? name+"chALL" : (charge==11)? name + "ch1-1" :  "wrong charge number. [1,-1,0,11 ]";
	if(name =="wrong charge number. [1,-1,0,11 ]" ) {
		cout << name << endl;
		return;
	}


	cout << "______________________________" << endl;
	cout << "FILE OUTPUT: " << name+"_stub_density_ss.root" << endl;

	//	HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
		f = new TFile(name+"_stub_density_ss.root","RECREATE");
//	f = new TFile("provaTT25_stub_density_ss.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the output file. Exiting..." << endl;
		return;
	}



	if (charge==11){
		readtree_roads b(fName);
		f -> cd();
		b.Loop(key,1);
		readtree_roads c(fName);
		f -> cd();
		c.Loop(key,-1);
	}
	else {
		readtree_roads a(fName);
		//if charge =1 : -1 -> do only one cycle. If charge =0 passa tutto. If charge = 11 -> fai giro due volte.
		f -> cd();
		a.Loop(key,charge);
	}


	f -> Write();
	return;
}

