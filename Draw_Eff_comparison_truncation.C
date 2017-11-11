/*
 * Draw_Eff_comparison.C
 *
 *  Created on: 11 nov 2017
 *      Author: gvidale
 */

#include "readtree_roads.h"
#include <TH1.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include "functions.h"
#include <TEfficiency.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <fstream>
#include <string>
//#include <boost/tokenizer.hpp>



void Draw_Eff33_comparison_truncation()
{

   TFile * f = new TFile ("effciency_comparison_TT41.root", "RECREATE");
   gStyle->SetOptFit();


   // draw a frame to define the range
   auto mg = new TMultiGraph();
   mg->SetTitle("TT33_eff_comp_fount_sf; disk scaling factor; efficiency [%]");
   auto mgc = new TMultiGraph();
    mgc->SetTitle("TT33_comb_comp_fount_sf; disk scaling factor; avg #combinations");

    TLegend * legend = new TLegend(0.4,0.3, 0.8,0.5);
    TLegend * legendcomb = new TLegend(0.4,0.3, 0.8,0.5);

//   TT33 FOUNT all info
   const Int_t n1 = 6;
   Double_t px1[] = {0.7,     0.8,     0.9,    1.0,    1.1,    1.2};

   double e[3][n1];
   double c[3][n1];
   double err[3][n1];
   e[1] = {87.2,   89.49,	90.09,	90.82, 89.99,	0.894};
   e[2] = {87.775, 90.80,	93.3,	93.85,	94.13,	0.942};
   e[3] = {87.778, 91.06,	93.6,	94.7, 95.45,	0.957};

    c[1] = {312,		457,	595,	712,	822,	922};
    c[2] = {342,		555,	812,	1084,	1357,	1600};
    c[3] = {346,		557,	897,	1306,	1794,	2300};

   Double_t ar[n1] =    {111, 168,	240,	330, 438,	555};

   err[1] = {0.5, 0.46, 0.43,	0.44,  0.45 ,	0.45};
    err[2] = {0.5, 0.44, 0.38,	0.36,  0.35,	0.35 };
    err[3] = {0.5, 0.43, 0.37,	0.34,  0.31,	0.3 };

    Double_t ex1[n1] = {0};

    TGraphErrors* eff[3];
    TGraphErrors * comb[3];
    char a[50];

   for (int i=0; i<3; i++){

	   if(i=0) sprintf(a,"fountain @ %2i 256K",200);
	   if(i=1) sprintf(a,"fountain @ %2i 256K",400);
	   if(i=2) sprintf(a,"fountain @ %2i 256K",800);

	   eff[i] = new TGraphErrors(n1,px1,e[i],ex1,err[i]);
	   comb[i] = new TGraphErrors(n1,px1,c[i],ex1,ex1);
	   eff[i]->SetMarkerColor(i+1);
	   eff[i]->SetMarkerStyle(21);
	   comb[i]->SetMarkerColor(i+1);
	   comb[i]->SetMarkerStyle(21);
	   mg->Add(eff[i]);
	   legend->AddEntry(eff[i],a);
	   mgc->Add(comb[i]);
	   legendcomb->AddEntry(comb[i],a);

   }

   auto c1 = new TCanvas("eff","efficiencies",700,500);
     c1->SetGrid();

   mg->Draw("ap");
   legend->Draw();
   c1->Update();
     c1->Write();


   auto c2 = new TCanvas("comb","combinations",700,500);
    c2->SetGrid();

   mg->Draw("ap");
   legendcomb->Draw();
   legend->Draw();


   c2->Update();
   c2->Write();

}
