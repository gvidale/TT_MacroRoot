

#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include "functions.h"
#include <TLine.h>
#include <TMath.h>
#include <TPavesText.h>
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "vector"
#include "TChain.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"

using namespace std;
int bin_mean(TH1F *);
int bin_integral(TH1F *);
float bin_center(TH1F *, int);



void readtree_std_deltaphi() {
	// Create a histogram for the values we read.
	TH1F * myHist = new TH1F("h1", "ntuple", 100, -4, 4);
	TChain *chain = new TChain("/ntupler/tree");

	char namefile[200][200];
	for(Int_t i= 50; i <80; ++i){
		sprintf(namefile[i],"root://cmsxrootd.fnal.gov//store/group/l1upgrades/SLHC/GEN/920/SingleMu/TT41/ParticleGuns/CRAB3/170823_002349/0000/PGun_TT41_pt3_Tilted_ntuple_%0i.root",i);
		//	   cout << name[i] <<endl;
		chain -> Add(namefile[i]);
	}
	// Create a TTreeReader for the tree, for instance by passing the
	// TTree's name and the TDirectory / TFile it is in.
	TTreeReader myReader(chain);

	TTreeReaderValue<vector <Float_t> > TTStubs_eta(myReader, "TTStubs_eta");
	TTreeReaderValue<vector <Float_t> > TTStubs_phi(myReader, "TTStubs_phi");
	TTreeReaderValue<vector <Float_t> > TTStubs_r(myReader, "TTStubs_r");
	TTreeReaderValue<vector <Float_t> > TTStubs_z(myReader, "TTStubs_z");
	TTreeReaderValue<vector <Float_t> > TTStubs_x(myReader, "TTStubs_x");
	TTreeReaderValue<vector <Float_t> > TTStubs_y(myReader, "TTStubs_y");
	TTreeReaderValue<vector <unsigned int> > TTStubs_modId(myReader, "TTStubs_modId");
	TTreeReaderValue<vector <Int_t> > genParts_charge(myReader, "genParts_charge");
	TTreeReaderValue<vector <Float_t> > genParts_pt(myReader, "genParts_pt");
	TTreeReaderValue<vector <Float_t> > genParts_phi(myReader, "genParts_phi");
	TTreeReaderValue<vector <Float_t> > genParts_vz(myReader, "genParts_vz");
	TTreeReaderValue<vector <Float_t> > genParts_eta(myReader, "genParts_eta");
	TTreeReaderValue<vector <bool> > TTStubs_isps(myReader, "TTStubs_psmodule");

	gStyle->SetOptStat(0);

	TH1F * d_phi_ps[16];
	TH2F * d_phi_r_ps[16];
	TH1F * d_phi_ss[16];
	TH2F * d_phi_r_ss[16];
	char name[20];
	for(int t=0; t<16;++t){
		sprintf(name,"deltpahi_ps_lay%1i",t);
		d_phi_ps[t]=new TH1F(name,name,1000,-0.02,0.02);
		d_phi_ps[t]->SetLineColor(kBlue);
		sprintf(name,"deltpahi_ps_lay_R%1i",t);
		d_phi_r_ps[t]=new TH2F(name,name,1000,-0.02,0.02,100,17,120);
		d_phi_r_ps[t]->SetMarkerColor(kBlue);
		d_phi_r_ps[t]->SetTitle("deltaphi_r_lay;phi [rad]; r [cm]");
		sprintf(name,"deltpahi_ss_lay%1i",t);
		d_phi_ss[t]=new TH1F(name,name,1000,-0.02,0.02);
		d_phi_ss[t]->SetLineColor(kRed);
		sprintf(name,"deltpahi_ss_lay_r%1i",t);
		d_phi_r_ss[t]=new TH2F(name,name,1000,-0.02,0.02,100,17,120);
		d_phi_r_ss[t]->SetTitle("deltaphi_r_lay;phi [rad]; r [cm]");
		d_phi_r_ss[t]->SetMarkerColor(kRed);
	}

	float stub_x,stub_y;
	float gen_phi;
	float deltax=0;
	float deltaphi=0;
	int lay=0;

	Int_t entries=0;
	while (myReader.Next() ) {

		int aux_TT = TrackParametersToTT().get_tt(genParts_phi->at(0), genParts_charge->at(0)/genParts_pt->at(0),genParts_eta->at(0),genParts_vz->at(0));
		if (aux_TT != 41) continue;
		int nstubs=TTStubs_eta->size();

		for(int i=0; i<nstubs; ++i){
			stub_x=TTStubs_r->at(i)*cos(TTStubs_phi->at(i));
			stub_y=TTStubs_r->at(i)*sin(TTStubs_phi->at(i));
			gen_phi=-asin( TTStubs_r->at(i)/100 /genParts_charge->at(0)/ 2 *0.3*3.8 / genParts_pt->at(0)) + genParts_phi->at(0);


			deltaphi=TTStubs_phi->at(i) - gen_phi;
			lay=TTStubs_modId->at(i)/10000;


			if(TTStubs_isps->at(i)) {
				d_phi_ps[lay-5]->Fill(deltaphi);
				d_phi_r_ps[lay-5]->Fill(deltaphi,TTStubs_r->at(i));
			}
			else{
				d_phi_ss[lay-5]->Fill(deltaphi);
				d_phi_r_ss[lay-5]->Fill(deltaphi,TTStubs_r->at(i)); // 0->5 barrel; 6->10 disks. 11->15 disks
			}
		}

		entries++;
	    if(entries%500000==0) cout << "entres: " << entries<< endl;
	}

	TCanvas * c = new TCanvas();
	c->Divide(5);
	TCanvas * c_ss = new TCanvas();
	c_ss->Divide(5);
	TCanvas * d=new TCanvas();
	d->Divide(5);
	TCanvas * e=new TCanvas();
	e->Divide(3,3);
	TCanvas * b_ss=new TCanvas();
	b_ss->Divide(3,3);
	TLine * l[16];
	TPavesText * t[16];

	//	from 0 to 15.
	for(int f=0; f<6;++f){
		e->cd(f+1);
		int bin48= bin_integral(d_phi_ps[f]);
		float interval = 2* bin_center(d_phi_ps[f],bin48);
		sprintf(name,"d_phi_ps_lay%1iD@Q95:%1.4f",f,interval);
		d_phi_ps[f]->SetTitle(TString(name)+";phi [rad]; counts");
		t[f] = new TPavesText(0.1,0.6,0.9,0.9);
		t[f]->AddText(name);
		d_phi_ps[f]->Draw();

		b_ss->cd(f+1);
		bin48= bin_integral(d_phi_ss[f]);
		interval = 2* bin_center(d_phi_ss[f],bin48);
		sprintf(name,"d_phi_ss_lay%1i-D@Q95:%1.4f",f,interval);
		d_phi_ss[f]->SetTitle(TString(name)+";phi [rad]; counts");
		//		d_phi_ss[f]->Draw("same");
		t[f]->AddText(name);
		d_phi_ss[f]->Draw();
	}

	// disks foreward
	for(int f=6; f<=10; ++f){
		//		disks
		c->cd(f-5);
		int bin48= bin_integral(d_phi_ps[f]);
		float interval = 2* bin_center(d_phi_ps[f],bin48);
		sprintf(name,"d_phi_ps_lay%1i-D@Q95:%1.4f",f,interval);
		d_phi_ps[f]->SetTitle(TString(name)+";phi [rad]; counts");
		//			d_phi_ps[f]->Draw();
		t[f] = new TPavesText(0.1,0.5,0.9,0.9);
		t[f]->AddText(name);
		d_phi_ps[f]->Draw();

		c_ss->cd(f-5);
		bin48= bin_integral(d_phi_ss[f]);
		interval = 2* bin_center(d_phi_ss[f],bin48);
		sprintf(name,"d_phi_ss_lay%1i-D@Q95:%1.4f",f,interval);
		d_phi_ss[f]->SetTitle(TString(name)+";phi [rad]; counts");
		//			d_phi_ss[f]->Draw("same");
		t[f]->AddText(name);
		d_phi_ss[f]->Draw();

		//			c->cd(f-5+5);
		//			t[f]->Draw();
					d->cd(f-5);
					d->SetGridx();
					d_phi_r_ps[f]->Draw();
					d_phi_r_ss[f]->Draw("same");


	}
	return;
}

int bin_mean(TH1F * h){
	float mean = h->GetMean();
	return h->GetXaxis()->FindBin(mean);
}

int bin_integral(TH1F * h){
	int meanbin = bin_mean(h);
	float full_integral=h->Integral();
	float integral=0;
	for (int i=meanbin; i<h->GetXaxis()->GetNbins(); i++){
		integral=h->Integral(meanbin,i);
		if (integral/full_integral>0.475){
			cout << h->GetName() <<" full integral " << full_integral << "binmean" << meanbin <<" bin_integral: " << i << " p48integral " << integral << endl;
			return i;
		}


	}

	return 0;
}

float bin_center(TH1F * h,int bin){
	return h->GetXaxis()->GetBinCenter(bin);

}

