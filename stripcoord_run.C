#define stripcoord_cxx
#include "stripcoord.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TH2.h>
#include <TTree.h>
#include <vector>
#include <algorithm>
#include <TMath.h>
#include <TVector2.h>
#include <fstream>

using namespace std;

float get_phi(float,float);
float convert_mpi_pi(float);
float delta_phi(float a, float b);
void stripcoord::Loop()
{
	//	Define new type "mod_cord", that is a struct
	typedef struct {
		int module_Id;
		bool is_ps;
		int n_str;
		int n_seg;
		vector <float> x;
		vector <float> y;
		vector <float> z;
		vector <float> r;
		vector <float> phi;
 		float phi_min;
		float phi_max;
	} mod_coord_struct_t;


	//   create the file, the Tree and a few branches with
	//   a subset of the variable mod_coord_, of type mod_coord_struct_t

	mod_coord_struct_t mod_coord;
	TTree * tree = new TTree ("tree", "tree_modCoords");

//	BRANCH: first argument is  CINT label of the branch ,
//	second is the address of the local variable that will fill,
//	third is the CINT label of the leaf. When drawingI suppose you can use the leaf name, or the branch name if you have only one leaf.
	tree->Branch("module_id", &mod_coord.module_Id,"module_Id/I" );
	tree->Branch("n_strip", &mod_coord.n_str,"n_str/I" );
	tree->Branch("n_segment", &mod_coord.n_str,"n_seg/I" );
	tree->Branch("is_ps", &mod_coord.is_ps, "is_ps/O");
	tree->Branch("x_coords",&mod_coord.x);
	tree->Branch("y_coords",&mod_coord.y);
	tree->Branch("z_coords",&mod_coord.z);
	tree->Branch("r_coords",&mod_coord.r);
	tree->Branch("phi_coords",&mod_coord.phi);
	tree->Branch("phi_min", &mod_coord.phi_min);
	tree->Branch("phi_max", &mod_coord.phi_max);

	//create variaboles

	float x1,x2,x3,x4,y1,y2,y3,y4,strip_pitch, z_a, z_b, phi_a,phi_b;
	const int n_seg_ps = 32;
	const int n_seg_ss = 2;
	const int n_str_ps = 960;
	const int n_str_ss = 1016;

	ofstream out("modules_coord_phi.dat");
	out << "# Vertex info of each module. Each vertex is displayed like a stub (x, y, z, r, phi). Each module has 4 vertex" << endl;
	out << "# moduleId --- x --- y --- z --- r --- phi [-pi,pi] " <<endl;


	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;


		//check if ps
		strip_pitch = sqrt(strip_dx*strip_dx+strip_dy*strip_dy);
		(strip_pitch > 0.0095) ? mod_coord.is_ps = true : mod_coord.is_ps = false;

		if (mod_coord.is_ps){
			mod_coord.n_str = n_str_ps;
			mod_coord.n_seg = n_seg_ps;
		}
		else {
			mod_coord.n_str = n_str_ss;
			mod_coord.n_seg = n_seg_ss;
		}

		//check module id
		(floor(modid/10000) == 7 && floor(modid%100) > 11) ? mod_coord.module_Id = modid + 1 : mod_coord.module_Id = modid;

		//push back in vector x & y the 4 points
		x1 = xm + 0.5*mod_coord.n_str*(strip_dx) - 0.5*mod_coord.n_seg*(seg_dx);
		x2 = xm + 0.5*mod_coord.n_str*(strip_dx) + 0.5*mod_coord.n_seg*(seg_dx);
		x3 = xm - 0.5*mod_coord.n_str*(strip_dx) + 0.5*mod_coord.n_seg*(seg_dx);
		x4 = xm - 0.5*mod_coord.n_str*(strip_dx) - 0.5*mod_coord.n_seg*(seg_dx);

		y1 = ym + 0.5*mod_coord.n_str*(strip_dy) - 0.5*mod_coord.n_seg*(seg_dy);
		y2 = ym + 0.5*mod_coord.n_str*(strip_dy) + 0.5*mod_coord.n_seg*(seg_dy);
		y3 = ym - 0.5*mod_coord.n_str*(strip_dy) + 0.5*mod_coord.n_seg*(seg_dy);
		y4 = ym - 0.5*mod_coord.n_str*(strip_dy) - 0.5*mod_coord.n_seg*(seg_dy);

		//vertex 0 and 3 are the closest to beam -> must have the closest z to z = 0
		//ATTENTION, when zm changes sign, changes also simmetrically sign(seg_dy) along vertical axis --->> 0,3 changes to 1,2
		z_a = zm + 0.5*mod_coord.n_seg*TMath::Sign(seg_dz,zm);
		z_b = zm - 0.5*mod_coord.n_seg*TMath::Sign(seg_dz,zm);

		mod_coord.x.clear();
		mod_coord.y.clear();
		mod_coord.z.clear();
		mod_coord.r.clear();
		mod_coord.phi.clear();

		mod_coord.x.push_back(x1);
		mod_coord.x.push_back(x2);
		mod_coord.x.push_back(x3);
		mod_coord.x.push_back(x4);

//		cout << "size vector x " << mod_coord.x.size() << endl;

		mod_coord.y.push_back(y1);
		mod_coord.y.push_back(y2);
		mod_coord.y.push_back(y3);
		mod_coord.y.push_back(y4);

		if (zm>=0){
		mod_coord.z.push_back(z_b); //1
		mod_coord.z.push_back(z_a); //2
		mod_coord.z.push_back(z_a); //3
		mod_coord.z.push_back(z_b); //4
		}
		else {
			mod_coord.z.push_back(z_a); //1
			mod_coord.z.push_back(z_b); //2
			mod_coord.z.push_back(z_b); //3
			mod_coord.z.push_back(z_a); //4
		}

		//vertex r
		for (int i = 0; i < 4 ; ++i){
		mod_coord.r.push_back( sqrt(mod_coord.x[i]*mod_coord.x[i] + mod_coord.y[i]*mod_coord.y[i]) );
		}

		//vertex phi (like a stub)
		for (int i = 0; i < 4 ; ++i){
			mod_coord.phi.push_back( convert_mpi_pi(get_phi(mod_coord.x[i],mod_coord.y[i])) ); //[-pi,pi];
		}


		//Find the phi of the border in x,y plane. Vertex internal are 0 and 3 (vector position)

		//		need to add +pi for second quadrant, -pi for III

		phi_a = get_phi(mod_coord.x[0],mod_coord.y[0]);
		phi_b = get_phi(mod_coord.x[3],mod_coord.y[3]);


		//proper min max for modules across x axis around phi = zero
		(delta_phi(phi_a,phi_b)< M_PI) ? mod_coord.phi_min = min(phi_a,phi_b) : mod_coord.phi_min = max(phi_a,phi_b);
		(delta_phi(phi_a,phi_b)< M_PI) ? mod_coord.phi_max = max(phi_a,phi_b) : mod_coord.phi_max = min(phi_a,phi_b);

		//convert in phi range [-pi,pi]
		mod_coord.phi_min = convert_mpi_pi(mod_coord.phi_min); //[-pi, pi]
		mod_coord.phi_max = convert_mpi_pi(mod_coord.phi_max);


		//Fill the tree
		tree->Fill();

		for (int i = 0; i < 4; ++i){
			out << modid << "		" << mod_coord.x[i] << "		" << mod_coord.y[i] << "		" << mod_coord.z[i] << "		" << mod_coord.r[i] << "		" << mod_coord.phi[i] << endl;
		}



	}

out.close();
}

void stripcoord_run(){

	gStyle->SetOptStat(0);

		cout << "______________________________" << endl;
		cout << "FILE OUTPUT: " << "extracted_modCoord.root"<< endl;

		//	HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK

		bool isfOpen; TFile* f = 0; isfOpen = false;
		f = new TFile("extracted_modCoord.root","RECREATE");
		isfOpen = f->IsOpen();
		if (!isfOpen) {
			cout << "ERROR. Not able to load the output file. Exiting..." << endl;
			return;
		}




	stripcoord a;
	f -> cd();
	a.Loop();

	f -> Write();


	return;
}


float get_phi(float a1, float b1){

	float phi = 0;
	TVector2 v(a1,b1);
	phi = v.Phi_0_2pi(v.Phi()); //phi of the point [0,2p]
	return phi;
}

float convert_mpi_pi (float phi){
	TVector2 a(1,0);
    return a.Phi_mpi_pi(phi);
}

float delta_phi(float a, float b){

	return a -b;
}
