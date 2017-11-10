/*

 * functions.h
 *
 *  Created on: 08 giu 2017
 *      Author: gvidale
 */

#ifndef MYMACRO_FUNCTIONS_H_
#define MYMACRO_FUNCTIONS_H_

#include <TROOT.h>
#include <TH1F.h>
#include <TEfficiency.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TColor.h>
#include <TPaveText.h>
#include <TFile.h>
#include <assert.h>
#include <iostream>
#include <TCanvas.h>
#include <TStyle.h>

using namespace std;

TFile* TFile_input_opener(TString name,TString prefix_path, TString suffix){
	//prefix_path: "../roads/mu_PU200_8K_sfXX_nzXX_pt3_0718/"
//	+name
//	+ suffix: "_efficiency_248.root"
	TFile * f_input1=new TFile(prefix_path+name+suffix);
	if (f_input1->IsZombie()) {
		cout << "ERROR. Not able to load the input ntuple file. Exiting..." << endl;
//		exit (EXIT_FAILURE);
	}
	return f_input1;
}

Int_t* phi_zeta(Int_t modId){

	Int_t* lpz= new Int_t[3];
//	Int_t layer, phi, zeta;

	lpz[0] = modId/10000;
	lpz[1] = ((modId)/100)%100;
	lpz[2] = (modId)%100;

	return lpz;
}

Double_t quantile_single(TH1 * h, Double_t x){
	Double_t xq[1]={x};
	Double_t yq[1];
	h->GetQuantiles(2,yq,xq);
	return yq[0];
}
//This function calculate the numerical roads efficiency for 200, 400, 800 (maxium efficiency provided)
//and store them in an already created ntuple (with 2 branches: road truncation, efficiency)
//Also, an histo with the cumulative road efficiencies with different trunation is build ( r_Eff_per_r_cutoff)
Float_t nroads_before_rec_cutoff(TH1I * nroads, float_t maximum_eff,float_t eff_wanted = 0.95){


	TCanvas * c_roads = new TCanvas();
	Int_t normalization_integral = nroads->Integral();

	Int_t bin_min = 0;
	Int_t bin_up = 1;

	Int_t bin_max = nroads->GetNbinsX();
	Float_t integr =0;
	Float_t eff=0;
	eff_wanted = 0.95;
	Float_t road_at95=0;

	TH1F * r_eff_per_r_cutoff = new TH1F("r_eff_per_r_cutoff","r_eff_per_r_cutoff",bin_max,0,1000);

	r_eff_per_r_cutoff->SetTitle("Road Efficiency @5 stub; fixed road read cutoff; efficiency");

	for(bin_up =0; bin_up<bin_max; ++bin_up){
		integr = nroads->Integral(bin_min,bin_up);
		eff = maximum_eff * (integr / normalization_integral );
		r_eff_per_r_cutoff->SetBinContent(bin_up,eff);
		if(eff>=0.95 && road_at95==0) road_at95=r_eff_per_r_cutoff->GetXaxis()->GetBinCenter(bin_up); //return road at desiderd efficiency
	}

	Int_t n_road_truncation[3] = {200,400,800};
	Float_t contentTrunc[3];
	Int_t binTrunc[3];

	for(Int_t index_cutoff = 0; index_cutoff <3; ++index_cutoff ){
	binTrunc[index_cutoff]=r_eff_per_r_cutoff->GetXaxis()->FindBin(n_road_truncation[index_cutoff]);
	contentTrunc[index_cutoff]= r_eff_per_r_cutoff->GetBinContent(binTrunc[index_cutoff]);


//	cout << "roads efficency at "<< n_road_truncation[index_cutoff] <<": " <<contentTrunc[index_cutoff] << endl;
	}

	r_eff_per_r_cutoff->Draw("histo");

	return road_at95;
}

void efficiency_turnon (TH1F * h, TH1F * h_tot, TEfficiency * &e,TString name,TString var, TString key){
	  e = new TEfficiency(*h,*h_tot);
	  e->SetName(name);
	  e->SetTitle("Eff - "+key+" - "+var +";"+var+";efficiency");
	  e->SetDirectory(gDirectory);
}

void draw_efficiencies_final(TEfficiency * e200,TEfficiency * e400,TEfficiency * e800,TEfficiency * einf,TCanvas * c_eff, TString key, Int_t counter=3){
//	Rule counter: if 1, only 200
//	if 2, 200, 400;
//	if 3, 200, 400, 800;
//	if 4, all.
//	if 5, only inf
	c_eff->cd();
	c_eff->SetGridx();
	c_eff->SetGridy();
	e800->SetLineColor(kBlack);
	e800->Draw();

	e400->SetLineColor(kRed);
	e400->Draw("same");

	e200->SetLineColor(kBlue);
	e200->Draw("same");

	einf->SetLineColor(kGreen);
	if(counter >= 4 )	einf->Draw("same");

//	TLegend *legend = new TLegend();//(0.1,0.7,0.48,0.9); TOP LEFT (x1,y1,x2,y2) heigh 0.2 width 0.38 , normalized cord [0,1] from (0,0)
	TLegend * legend = new TLegend(0.4,0.1, 0.6,0.3);
	//	   legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
	if(counter >= 1)	legend->AddEntry(e200,"Eff @ 200"+key);
	if(counter >= 2) 	legend->AddEntry(e400,"Eff @ 400"+key);
	if(counter >= 3)	legend->AddEntry(e800,"Eff @ 800"+key);
	if(counter >= 4) 	legend->AddEntry(einf,"Eff @ inf"+key);

	legend->Draw();

}

void draw_combs_road(TH1F * c200,TH1F * c400, TH1F * c800,TCanvas * c, TString key,Int_t counter = 3){
	c->cd();
	c->SetGridx();
	c->SetGridy();
//	c->SetOptTitle(0);
//	TPaveText *t = new TPaveText(0.3, 0.9, 0.7, 1.0, "brNDC"); // left-up
//	t->AddText(key+" combs/bx ");
//	t->SetBorderSize(0);
//	t->SetFillColor(gStyle->GetTitleFillColor());
	Double_t xq[2]={0.95,0.99};
	Double_t yq[2];
	c200->SetLineColor(kBlue);
	c200->SetTitle("tot_combs/bx "+key); //titolo complessivo della canvas
	if(counter >= 1)					c200->Draw();
	c400->SetLineColor(kRed);
	if(counter >= 2)					c400->Draw("same");
	c800->SetLineColor(kBlack);
	if(counter >= 3)					c800->Draw("same");
	TLegend * legend = new TLegend(0.4,0.65, 0.9,0.9);
	char c_legend[100];
	if(counter >= 1){
		c200->GetQuantiles(2,yq,xq);
		sprintf(c_legend,", combs/bx@200, mu=%4.0f, q95=%4.0f",c200->GetMean(),yq[0]);
		legend->AddEntry(c200,key+TString(c_legend));
	}
	if(counter >= 2) 					{
		c400->GetQuantiles(2,yq,xq);
		sprintf(c_legend,", combs/bx@400, mu=%4.0f, q95=%4.0f",c400->GetMean(),yq[0]);
		legend->AddEntry(c400,key+TString(c_legend));
	}
	if(counter >= 3){
		c800->GetQuantiles(2,yq,xq);
		sprintf(c_legend,", combs/bx@800, mu=%4.0f, q95=%4.0f",c800->GetMean(),yq[0]);
		legend->AddEntry(c800,key+TString(c_legend));
	}
	legend->Draw();

}

void draw_combs_fak_dup_inf(TH1F * cinf,TH1F * dupinf, TH1F * fakinf,TCanvas * c, TString key=""){
	c->cd();
	c->SetGridx();
	c->SetGridy();
	c->SetLogx();
	c->SetLogy();

	Double_t xq[2]={0.95,0.99};
	Double_t yq[2];
	cinf->SetTitle("comb_perroad/bx "+key); //titolo complessivo della canvas
	cinf->SetLineColor(kBlack);
	cinf->SetFillStyle(3002);
	cinf->SetFillColor(kBlack);
	dupinf->SetLineColor(kBlue);
	dupinf->SetFillStyle(3002);
	dupinf->SetFillColor(kBlue);
	fakinf->SetLineColor(kRed);
	fakinf->SetFillStyle(3002);
	fakinf->SetFillColor(kRed);
	cinf->Draw("");
	dupinf->Draw("same");
	fakinf->Draw("same");

	TLegend * legend = new TLegend(0.4,0.65, 0.9,0.9);
	char c_legend[100];

	cinf->GetQuantiles(2,yq,xq);
	sprintf(c_legend,", road_combs/bx, mu=%4.0f, q95=%4.0f",cinf->GetMean(),yq[0]);
	legend->AddEntry(cinf,key+TString(c_legend));


	dupinf->GetQuantiles(2,yq,xq);
	sprintf(c_legend,", road_duplicate_combs/bx, mu=%4.0f, q95=%4.0f",dupinf->GetMean(),yq[0]);
	legend->AddEntry(dupinf,key+TString(c_legend));


	fakinf->GetQuantiles(2,yq,xq);
	sprintf(c_legend,", road_fake_combs/bx, mu=%4.0f, q95=%4.0f",fakinf->GetMean(),yq[0]);
	legend->AddEntry(fakinf,key+TString(c_legend));

	legend->Draw();

}

void draw_single_histo(TH1 * h, TString name_legend, TCanvas *c,TString key = "", Color_t color = kBlue){ //Tcolor optional
	gStyle->SetOptStat(0);
	c->cd();
	c->SetGridx();
	c->SetGridy();
	char c_legend[100];

	sprintf(c_legend,", mu=%4.0f, q95=%4.0f",h->GetMean(),quantile_single(h,0.95));
	TLegend * legend = new TLegend(0.4,0.65, 0.9,0.9);
//	TPaveText * text = new TPaveText(0.4,0.4, 0.9,0.65);
	legend->AddEntry(h,name_legend+TString(c_legend));
//	text->AddText(c_legend);


	h->SetLineColor(color);
	h->Draw("same");
	legend->Draw();
//	text->Draw();

}
#endif /* MYMACRO_FUNCTIONS_H_ */
