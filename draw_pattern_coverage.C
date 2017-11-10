/*
 * draw_pattern_coverage.C
 *
 *  Created on: Aug 9, 2017
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
#include <TGraph.h>
#include <TMultiGraph.h>

void draw_pattern_coverage(){



	Int_t x[3]={90,95,99};

	Int_t * y[5][3] = {
						{42583, 58953, 107549},	//sf1
						{78772, 114619, 232377},//sf08
						{116820,175823,38422},//sf07
						{190964,299682, 661913}, //sf06
						{358281,603486,1042388}, //sf05
					  };
	Float_t sf[5]={1,0.8,0.7,0.6,0.5};




	char name[50];
	TGraph * gr[3];
	TCanvas * confront_eff = new TCanvas();
	TLegend * legend = new TLegend(0.6,0.3, 0.8,0.5);
	TMultiGraph *mg = new TMultiGraph();

	for(Int_t i =0; i<5;++i){

		sprintf(name, "sf %1.1f",sf[i]);
		gr[i]= new TGraph(3,x,*y[i]);
		gr[i]->SetMarkerStyle(21);
		gr[i]->SetMarkerSize(2);
		gr[i]->SetMarkerColor(i+1);
		gr[i]->SetLineColor(i+1); //0 is white
		gr[i]->SetTitle(name);
		gr[i]->SetFillStyle(0);
		mg->Add(gr[i],"");
		legend->AddEntry(gr[i],name);
	}

	mg->Draw("apl");

	legend->Draw();




}

