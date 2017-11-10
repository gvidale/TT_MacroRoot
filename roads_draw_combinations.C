/*
 * roads_draw_efficiency.C
 *
 *  Created on: Aug 5, 2017
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

using namespace std;

void roads_draw_combinations(){

	TString name = "roads_mu_PU200_8K_sf0.8_nz1_pt3nosort_64K_0725";
//	*****************************************************************
//	LOAD INPUT _efficiency.root  with all the info u need
//	*****************************************************************
	TFile *f_input = new TFile("../roads/mu_PU200_8K_sfXX_nzXX_pt3_0718/"+name+"_combination.root");
	if (f_input->IsZombie()) {
		cout << "ERROR. Not able to load the input ntuple file. Exiting..." << endl;
		return;
	}


//	************************
//	IMPORT OBJECT (thei c++ name must be different from the one in memory)
//	************************
	TH1F * totcomb_200;
	TH1F * totcomb_400;
	TH1F * totcomb_800;

	f_input->GetObject("tot_combins_evt_200",totcomb_200);
	f_input->GetObject("tot_combins_evt_400",totcomb_400);
	f_input->GetObject("tot_combins_evt_800",totcomb_800);



//









	return;
}


