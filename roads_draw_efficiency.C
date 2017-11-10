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

void roads_draw_efficiency(TString name ="0",TString key ="0",TString option="erc"){
//	\(\"roads_mu_PU200_8K_sf0.5_nz1_pt3nosort_256K_0808\",\"sf0_5-256K\"\)
	Bool_t draw_eff = false, draw_roads = false, draw_combs = false;
	if(option.Index("e")!=-1) draw_eff = true;
	if(option.Index("r")!=-1) draw_roads = true;
	if(option.Index("c")!=-1) draw_combs = true;

	if(name!="0" && key =="0"){
			cout << "Please provide 'key' string as 2nd arg (sf_XX-64K)" << endl;
			return;
		}
	if(name=="0"){//otherwise put it in there
		name="roads_mu_PU200_8K_sf0.8_nz1_pt3nosort_64K_0725";
		key = " sf0_8-64K";
		}

//	*****************************************************************
//	LOAD INPUT _efficiency.root  with all the info u need
//	*****************************************************************
	TFile *f_input = new TFile("../roads/mu_PU200_8K_sfXX_nzXX_pt3_0718/"+name+"_efficiency_248.root");
	if (f_input->IsZombie()) {
		cout << "ERROR. Not able to load the input ntuple file. Exiting..." << endl;
		return;
	}


//	************************
//	IMPORT OBJECT (thei c++ name must be different from the one in memory)
//	************************

	if(draw_eff){
	TEfficiency * e_phi_inf;
	f_input->GetObject("eff_phi_inf",e_phi_inf);

	TEfficiency * e_phi_200;
	f_input->GetObject("eff_phi_200",e_phi_200);
	TEfficiency * e_phi_400;
	f_input->GetObject("eff_phi_400",e_phi_400);
	TEfficiency * e_phi_800;
	f_input->GetObject("eff_phi_800",e_phi_800);

	TEfficiency * e_pt_inf;
	f_input->GetObject("eff_pt_inf",e_pt_inf);
	TEfficiency * e_pt_200;
	f_input->GetObject("eff_pt_200",e_pt_200);
	TEfficiency * e_pt_400;
	f_input->GetObject("eff_pt_400",e_pt_400);
	TEfficiency * e_pt_800;
	f_input->GetObject("eff_pt_800",e_pt_800);

	TEfficiency * e_eta_inf;
	f_input->GetObject("eff_eta_inf",e_eta_inf);
	TEfficiency * e_eta_200;
	f_input->GetObject("eff_eta_200",e_eta_200);
	TEfficiency * e_eta_400;
	f_input->GetObject("eff_eta_400",e_eta_400);
	TEfficiency * e_eta_800;
	f_input->GetObject("eff_eta_800",e_eta_800);

//
	TCanvas *c_e_pt = new TCanvas("c_e_pt");
	TCanvas *c_e_eta = new TCanvas("c_e_eta");
	TCanvas *c_e_phi = new TCanvas("c_e_phi");


	draw_efficiencies_final(e_pt_200,e_pt_400,e_pt_800,e_pt_inf,c_e_pt,3);
	draw_efficiencies_final(e_phi_200,e_phi_400,e_phi_800,e_phi_inf,c_e_phi,3);
	draw_efficiencies_final(e_eta_200,e_eta_400,e_eta_800,e_eta_inf,c_e_eta,3);
	}

	if(draw_roads){

		TH1F * pt_numerator_inf;
		f_input->GetObject("mu_trk_pT_4",pt_numerator_inf);
		TH1F * pt_denominator;
		f_input->GetObject("mu_trk_pT",pt_denominator);
		TH1I * r_bef_rec;
		f_input->GetObject("road_read_before_reconstruction",r_bef_rec);

		Float_t coverage = 0.95;
		Float_t roadateff95 = nroads_before_rec_cutoff(r_bef_rec,pt_numerator_inf->GetEntries()/pt_denominator->GetEntries(),coverage);

		cout << "roads @ "<< coverage <<"%: " << roadateff95 << endl;
		TCanvas * c_r = new TCanvas("c_r");
		draw_single_histo(r_bef_rec,"#roads read before reconstruction"+key,c_r);
	}

	if(draw_combs){

		TH1F * c_inf;
		f_input->GetObject("combs_inf",c_inf);
		TH1F * c_tot_200;
		TH1F * c_tot_400;
		TH1F * c_tot_800;

		f_input->GetObject("tot_combins_evt_200",c_tot_200);
		f_input->GetObject("tot_combins_evt_400",c_tot_400);
		f_input->GetObject("tot_combins_evt_800",c_tot_800);

		//	Combinations

		TCanvas * canv_inf = new TCanvas("c_inf");
		draw_single_histo(c_inf, "combs per road (no truncation)", canv_inf,key);

		TCanvas * c_combs_roadlimit = new TCanvas("c_combs_roadlimit");
		draw_combs_road(c_tot_200,c_tot_400,c_tot_800,c_combs_roadlimit,key,3);
	}


	TNtuple *ntuple;
	f_input->GetObject("raw_roads_combs",ntuple);
	ntuple->Scan("","","colsize=14 precision=4");

	return;
}


