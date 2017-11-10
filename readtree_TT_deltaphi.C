#define readtree_TT_cxx
#include "readtree_TT.h"
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

using namespace std;
int bin_mean(TH1F *);
int bin_integral(TH1F *);
float bin_center(TH1F *, int);

void readtree_TT::Loop(TString key){

	TH1F * d_phi_ps[20];
	TH2F * d_phi_r_ps[20];
	TH1F * d_phi_ss[20];
	TH2F * d_phi_r_ss[20];
	char name[20];
	for(int t=0; t<20;++t){
		sprintf(name,"deltpahi_ps_lay%1i",t);
		d_phi_ps[t]=new TH1F(name,name,2000,-0.02,0.02);
		 d_phi_ps[t]->SetTitle(TString(name)+";#phi [rad]; counts");
		d_phi_ps[t]->SetLineColor(kBlue);
		d_phi_r_ps[t]=new TH2F(TString(name)+"_r",TString(name)+"_r",2000,-0.01,0.01,100,17,120);
		d_phi_r_ps[t]->SetMarkerColor(kBlue);
		d_phi_r_ps[t]->SetTitle(TString(name)+";#phi [rad]; r [cm]");
		sprintf(name,"deltpahi_ss_lay%1i",t);
		d_phi_ss[t]=new TH1F(name,name,2000,-0.02,0.02);
		d_phi_ss[t]->SetLineColor(kRed);
		 d_phi_ss[t]->SetTitle(TString(name)+";#phi [rad]; counts");
		d_phi_r_ss[t]=new TH2F(TString(name)+"_r",TString(name)+"_r",2000,-0.01,0.01,100,17,120);
		d_phi_r_ss[t]->SetMarkerColor(kRed);
		d_phi_r_ps[t]->SetTitle(TString(name)+";#phi [rad]; r [cm]");
	}

	float stub_x,stub_y;
	float gen_phi;
	float deltax=0;
	float deltaphi=0;
	int lay=0;
	float r_ps=0;

	if (fChain == 0) return;

	Long64_t nentries = fChain->GetEntriesFast();
cout << "Pino" <<endl;
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<10000000;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		int nstubs=TTStubs_eta->size();
		for(int i=0; i<nstubs; ++i){
			stub_x=TTStubs_r->at(i)*cos(TTStubs_phi->at(i));
			stub_y=TTStubs_r->at(i)*sin(TTStubs_phi->at(i));
			gen_phi=-asin( TTStubs_r->at(i)/100 /genParts_charge->at(0)/ 2 *0.3*3.8 / genParts_pt->at(0)) + genParts_phi->at(0);


			deltaphi=TTStubs_phi->at(i) - gen_phi;
			lay=TTStubs_modId->at(i)/10000;
			if(lay-5<=7) r_ps= 66.45;
			else r_ps=64.3;
			
//			cout << "ok here " << endl;
//			cout << "lay " << lay << endl;
			if(TTStubs_r->at(i)<r_ps) {
				d_phi_ps[lay-5]->Fill(deltaphi);
			
//				 cout << "ttstubs_r->att(i) " << TTStubs_r->at(i) <<endl;
				d_phi_r_ps[lay-5]->Fill(deltaphi,TTStubs_r->at(i));
			}
			else{
			d_phi_ss[lay-5]->Fill(deltaphi);
			
//			cout << "ttstubs_r->att(i) " << TTStubs_r->at(i) <<endl;
			d_phi_r_ss[lay-5]->Fill(deltaphi,TTStubs_r->at(i)); // 0->5 barrel; 6->10 disks. 11->15 disks
			}
		}



		if(jentry%100000==0) cout << "evt " << jentry <<endl;
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
		sprintf(name,"d_phi_ps_lay%1i - DELTA@Q95:%1.4f",f,interval);
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



//		t[f]->Draw();




	}
	e->Write();
	b_ss->Write();
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
			sprintf(name,"d_phi_ss_lay%1i - DELTA@Q95:%1.4f",f,interval);
			d_phi_ss[f]->SetTitle(TString(name)+";phi [rad]; counts");
//			d_phi_ss[f]->Draw("same");
			t[f]->AddText(name);

			d_phi_ss[f]->Draw();


//
//			c->cd(f-5+5);
//			t[f]->Draw();
			d->cd(f-5);
			d_phi_r_ss[f]->Draw();
			d_phi_r_ps[f]->Draw("same");



	}
	c->Write();
	c_ss->Write();
	d->Write();

}

int bin_mean(TH1F * h){
	float mean = h->GetMean();
	return h->GetXaxis()->FindBin(mean);
}

int bin_integral(TH1F * h){
	int meanbin = bin_mean(h);
	float meanbincontent = h->GetBinContent(meanbin);
	float full_integral=h->Integral();
	float integral=0;
	for (int i=0; i<h->GetXaxis()->GetNbins(); i++){
		integral=h->Integral(meanbin-i,meanbin+i);
		if (integral/full_integral >0.95){
			cout << h->GetTitle() <<" full integral " << full_integral << "binmean" << meanbin <<" bin_integral: " << i << " p48integral " << integral << " std dev " << h->GetStdDev() << endl;
			return meanbin+i;
		}


	}

	return 0;
}

float bin_center(TH1F * h,int bin){
	return h->GetXaxis()->GetBinCenter(bin);

}

void readtree_TT_deltaphi() {

	//INPUT NTUPLE

	gStyle->SetOptStat(0);
	TString keyA;
	TString fNameA;
	TString name;
//	name = "TT25sample_cleaned_0712";
	name="wholetracker_cleaned_0822"; //master name of file root
	fNameA = "root://cmsxrootd.fnal.gov//store/user/gvidale/Whole/wholetracker_cleaned_0822.root"; //name of file root to read, with path
	keyA = "_"+name;     //just a key for histos

	cout << fNameA << endl;
	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile(name+"_deltaphi.root","RECREATE");
	isfOpen = f->IsOpen();
	if (!isfOpen) {
		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
		return;
	}

	cout << "out opened " << endl;
	readtree_TT a(fNameA);
	f -> cd();
	
	cout << "readtreeTT created " << endl;

	
	a.Loop(keyA);

	f -> Write();
	cout << "ho scritto" << endl;
	return;
}

