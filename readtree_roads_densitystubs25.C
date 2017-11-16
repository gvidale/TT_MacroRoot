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

	TH2F * n_stub_ss_lay = new TH2F ("n_stub_ss_lay","",11,0,11,1024,0,1024);

	n_stub_ss_lay->SetTitle("n_stub_ss_lay_avgevt"+key+"; layer; ssId");

	TH2F * ssid_lay_road = new TH2F("ssid_lay_road","",11,0,11,512,0,512);
	ssid_lay_road->SetTitle("ssid_lay_roadfired_avgevt_"+key+"; layer; ssId");

	TH2F * xy[11];
	char name[100];



	for(int l =0; l<11; ++l){
		sprintf(name,"nstubs_xy_id0_lay_%1i_charge%1i",l,charge);
		xy[l] = new TH2F(TString(name),"xy_",500,-30,130,500,-10,130);
		sprintf(name,"nstubs_xy_road_w/_id0_per_lay_%1i_charge%1i",l,charge);
		xy[l]->SetTitle(TString(name)+";x [cm];y [cm]");
	}

	TH2F * n_stub_ss_r[11];

	for(int r =0; r<11; ++r){
		sprintf(name, "occupancy_(norm_dphi)1_r_vs_ssId,_layer_%1i_charge%1i", r,charge);
		n_stub_ss_r[r] = new TH2F(name,name,250,0,250,200,19,110);
		n_stub_ss_r[r]->SetTitle(TString(name)+";ssId;r[cm]");
	}

	int n_lay_w_ssid0[11]={0};

//	float phiWidths_ [16]= {
//			0.00381*2, 0.00439, 0.00459, 0.00485, 0.00523, 0.00575,
//			0.0048   , 0.0050 , 0.0058 , 0.0064 , 0.0070 , // RR guestimates based on the 1st barrel 2S and scattering studies on disks a-la fountain
//			0.0048   , 0.0050 , 0.0058 , 0.0064 , 0.0070
//	};




	cout << "pino"<<endl;
	Long64_t nentries = fChain->GetEntriesFast();
	nentries = 1000; //set #evt to check
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
				if (aux_TT != 25) continue;
		
				//--->Keep only positive for positive bank
				if (charge*trkParts_charge->at(t) ==-1 )  continue;
			}
		}


		int nroads = AMTTRoads_superstripIds->size();
		vector<float> phi =*TTStubs_phi;
		vector<float>  r = *TTStubs_r;

		for(int r =0; r<nroads; ++r){

			vector<unsigned int> full_ssid = AMTTRoads_superstripIds->at(r);   //take the pattern "r", made by 6 (ssids)
			vector<vector<unsigned int> >  stubref = AMTTRoads_stubRefs->at(r);
			for(int l=0; l<6;++l){
				lay=  full_ssid.at(l)/16384;
				ssid = full_ssid.at(l)%16384;
				n_stub_ss_lay->Fill(lay,ssid,stubref[l].size()*float(1./nentries)); //normalized per nentries
				//we should also normalize per # road that light up the same ssid in that layer, in that event. A good estimator of this could be the # of siblings. (average is 8)
				ssid_lay_road->Fill(lay,ssid,float(1./nentries));

				if(ssid >= 0 && ssid < 30) {
					n_lay_w_ssid0[lay]++;
					unsigned int stub;

					for(int u=0;u<6;++u){
						cout << full_ssid.at(u)/4096 << " " << full_ssid.at(u)%4096 << "    ";
						}
					cout << endl;
					for(int y=0,s=0;y<stubref[s].size()&&s<6;y++,s++){
						stub=stubref[s][y];
						xy[s]->Fill(TTStubs_r->at(stub)*cos(TTStubs_phi->at(stub)),TTStubs_r->at(stub)*sin(TTStubs_phi->at(stub)));
					}
				}
			}
		}



		if(jentry%1000==0) cout <<flush<< "@";
	}

//	TCanvas * c = new TCanvas();
//	//   n_stub_ss_lay->SetMaximum(3);
//	//   n_stub_ss_lay->SetMinimum(1); //so that we can manage ssid 0 and maxium that have more stubs due to under over flow.
//	//   n_stub_ss_lay->GetYaxis()->SetRangeUser(0,230);
//	n_stub_ss_lay->Draw("colz");
//
//	TCanvas * r = new TCanvas();
//	ssid_lay_road->Draw("colz");
//
//	TCanvas * lay = new TCanvas();
//	lay->SetCanvasSize(1440,900);
//	lay->Divide(4,3);
//	for ( int i =0; i< 11 ; ++i){
//		lay->cd(i+1);
//		xy[i]->Scale(1./n_lay_w_ssid0[i]); //normalize per # lay that have ssid0 in them.
//		xy[i]->Draw("colz");
//	}
}
//   TLegend * l = new TLegend(0.1,0.3,0.9,0.7);

/*  r->Divide(3,2);
   for (int a=6; a< 11; ++a){
	   r->cd(a-5);
	   n_stub_ss_r[a]->GetXaxis()->SetRangeUser(0,230);
	   n_stub_ss_r[a]->Draw("colz");
   }
   cout << endl;
 */


void readtree_roads_densitystubs25(TString name = "0", TString key = "0",Int_t charge=1){

	gStyle->SetOptStat(0);
	TString fName;
	if(name!="0" && key =="0"){
		cout << "Please provide 'key' string as 2nd arg (sf_XX-64K)" << endl;
		return;
	}
	if(name=="0" && key =="0"){//otherwise put it in there
		//	name = "root://cmsxrootd.fnal.gov//store/user/gvidale/TT41/roads_TT33_fount_sf0.8_nz1_nr1_sf1_1710";
		//		name="roads_TT41new_flower_pt12_sf1_nz1_nr1_sf09_0818";
		//		name="old/roads_Tilted_tt25_sf1_nz1_pt3_100k_TT25sample";
		key = "sf09_flower_pt12";
	}


	cout << "______________________________" << endl;
	cout << "FILE INPUT: " << name << endl;

	fName ="../roads/"+ name+".root"; //name of file root to read, with path
	if(key.Index("cmseos")!=-1) fName = "root://cmsxrootd.fnal.gov//store/user/gvidale/TT25/"+name+".root"; //for eos file in my home
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
	f = new TFile("../roads/TT25/"+name+".root","RECREATE");
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

