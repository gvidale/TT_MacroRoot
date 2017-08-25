/*
Road efficiency & turn on curves

19/07/2017

Giorgio Vidale

 */

#define readtree_roads_cxx
#include "readtree_roads.h"
#include <TH1.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include "functions.h"
#include "SLHCL1TrackTriggerSimulations/AMSimulation/interface/StubCleaner.h"
#include <TNtuple.h>
#include <TEfficiency.h>
#include <TLegend.h>
#include <iomanip>

void readtree_roads::Loop(TString key,Int_t charge)
{

	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();

//	**************************
//	ROAD EFFICIENCY PARAMETERS
//	**************************
	Bool_t test = false; //ONLY 1000 entries checked.
	Bool_t parse_all_roads_combs = true; //look over all roads fired, to count the total number of combinations. Or, stop at first candidate found.
	const Int_t stub_threshold = 5;
	int select_charge = charge;  //select charge to process. "0" keeps all charge

	const Int_t roads_cutoff = 1000000; //infinite bank
//	const Int_t roads_cutoff = 200;
	Bool_t draw_histos = false;
	int tower = 41;
	Bool_t is_evt_reconstr = false;

	Float_t average_roads_before_reconstruction[4];
	Float_t true_mu=0;
	Float_t n_recognized_mu=0;
	Float_t average_recognized_mutrack=0;
	Float_t average_roads_fired=0;
	Float_t average_candidate_roads_perevt=0;

	Int_t evt_not_reconstructed=0;
	Int_t roads_before_reconstruction=0;
	Int_t genuine_mu=0;
	Int_t all_stub=0;
	Int_t n_roads_fired=0;
	Int_t n_evt_noroads=0;
	Int_t n_stubs;
	Int_t trk_index;
	unsigned int stub_index;
	Int_t good_entries=0;
	Int_t evt_fully_reconstructed=0;

	Int_t * layer_phi_zeta;
	Float_t trkParts_ChargeOverPt;
	set<Int_t> setOflayers;		//remove overlap in same layer
	pair < set<Int_t>::iterator , Bool_t>  insertYES;

	map<Int_t, Int_t > setOfmu;		//keep trace of a single true-mu trkpart
	map<Int_t, Bool_t> setOfmu_recognized; //recognized bool
	pair < map<Int_t, Bool_t>::iterator , Bool_t>  alreadyRecognYES;

	TNtuple * ntuple_allinfo = new TNtuple("ntuple_allinfo","ntuple_allinfo","event:trkPart_index:good_road_index");
	TNtuple * ntuple_not_reconstr = new TNtuple("ntuple_not_reconstr","ntuple_not_reconstr","event:trkPart_index:trkPart_pdgID:trkPart_pt:trkPart_eta:trkPart_phi");
	TNtuple * raw_roads_eff = new TNtuple("raw_roads_eff","raw_roads_eff", "road_truncation:efficiency");
	TNtuple * raw_roads_combs = new TNtuple("raw_roads_combs","Results with truncation"+key, "road_truncation:avg_r_fired:avg_r_f95:avg_rds_before_reconstr:avg_r_quant95:comb_mean:comb_quant95:road_eff:errUp:errDown");

//	ntuple_not_reconstr->Scan("","","colsize=11 precision=4")
//	command to print ntuple in good format

//	***********************************************************
//	HISTO for ROAD EFFICIENCY AND TURN ON CURVES (pt, eta, phi)
//	************************************************************
//	Double_t xbins[25] = {...} array of low-edges (xbins[25] is the upper edge of last bin

	Float_t xbins[18]={2,3,4,5,6,7,8,9,10,20,30,40,50,70,90,110,150,200}; //18 here -> nbins = xbins size -1 = 17
	TH1F * phi[5]; TH1F * eta[5]; TH1F * pt[5];
	TEfficiency *eff_phi[4]; TEfficiency * eff_eta[4]; TEfficiency * eff_pt[4]; TH1I * road_read_before_reconstruction[4];
	TH1F * combs_evt[3];
	const int trunc[3]={200,400,800};
	TString namehisto[5] = {"_denominator","_infroads","_200","_400","_800"};
	TString name_charge;
	(select_charge==1)? name_charge = "_ch1" : (select_charge==-1)? name_charge="_ch-1" : name_charge = "_chALL";
	char name[100];

	//41 eta 1.2 2.4; 25 eta -0.5 1.5
	//41 phi 0.6 1.8; 25 phi 0.6 1.75
	
	for (int y=0; y<5;++y){
		sprintf(name,"mu_phi_%2i"+namehisto[y]+name_charge,tower);
		phi[y] = new TH1F(name,name,20,0.60,1.8);
		phi[y]->Sumw2();
		phi[y]->SetTitle(TString(name)+";phi;counts");
		sprintf(name,"mu_eta_%2i"+namehisto[y]+name_charge,tower);
		eta[y] = new TH1F(name,name, 20,1.2,2.4);
		eta[y]->Sumw2();
		eta[y]->SetTitle(TString(name)+";eta;counts");
		sprintf(name,"mu_pt_%2i"+namehisto[y]+name_charge,tower);
		pt[y]  = new TH1F(name,name,17,xbins);
		pt[y]->Sumw2();
		pt[y]->SetTitle(TString(name)+";pt;counts");
	}

//	}
	for (int y=0; y<3;++y){
		combs_evt[y] = new TH1F("tot_combs_evt@"+namehisto[y+2]+name_charge,"",100,0,10000);
		combs_evt[y]->SetTitle("combs per evt @"+namehisto[y+2]+name_charge+"; # total combs per evt; counts");
		}
	
	for (int y=0; y<4;++y){
		road_read_before_reconstruction[y]=new TH1I("road_read_before_reconstruction@"+namehisto[y+1]+name_charge,"",100,0,1000);
		road_read_before_reconstruction[y]->SetTitle("road_read_before_reconstruction@"+namehisto[y+1]+name_charge+"; # roads read; counts");

	}

	TH1I * road_fired = new TH1I("road_fired"+name_charge,"road_fired",65,0,5000);
	road_fired->SetTitle("road_fired - "+key+name_charge+"; # roads read; counts");

	TH1I * candid_r_evt = new TH1I("candid_r_evt"+name_charge,"candid_r_evt",1000,0,1000);
	candid_r_evt->SetTitle("candid_r_evt - "+key+name_charge+"; # candidate road/evt; counts");

//	**********
//	SET HISTOS (combination)
//	**********
	TH1F * combs_inf = new TH1F ("combs_inf"+name_charge,"",100,0,300);
	combs_inf->SetTitle("combs_inf"+name_charge+";# combinations per road;# counts");
	Int_t n_stubs_for_combs;
	Int_t combination_for_a_road;
	Float_t average_combination_for_road=0;
	Float_t out_combinations_per_evt=0;
	Int_t n_candidate_roads=0;

	//inf, 200,400,800
	Float_t tot_combs[4] = {0.,0.,0.,0.};
	Float_t average_tot_combs[4] = {0.,0.,0.,0.};

//	***********
//	LOOP EVENTS
//	***********

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;

		if(test){
		if (ientry > 100) break;
		}
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		// if (Cut(ientry) < 0) continue;

		if (jentry % 1000 == 0) {
			cout << flush << "@";
		}


//		Reset counters
		setOfmu.erase(setOfmu.begin(),setOfmu.end()); //each event has a new set of authentic mu
		setOfmu_recognized.erase(setOfmu_recognized.begin(),setOfmu_recognized.end());
		is_evt_reconstr = false;
		for(int r=0;r<4;r++){
			tot_combs[r]=0;
		}
		average_candidate_roads_perevt=0;

//		**************************
//		APPLY FILTERS on TRK PARTS
//		**************************
//		Filter: 1. must be mu;
//				2. must be true intime, primary, signal;
//				3. must be in TT25

		Int_t n_trkparts = trkParts_pt->size();
		UInt_t mu_count=0;
		for (Int_t t=0; t < n_trkparts; ++t ) {
			if ( (trkParts_pdgId->at(t) == 13 || trkParts_pdgId->at(t) == -13)
					&& trkParts_signal->at(t) && trkParts_intime->at(t) && trkParts_primary->at(t) ) {

//				---> APPLY PHASE SPACE CUT TU mu that satisfy filter 1 & 2)
				trkParts_ChargeOverPt = float(trkParts_charge->at(t))/float(trkParts_pt->at(t));
				int aux_TT = TrackParametersToTT().get_tt(trkParts_phi->at(t), trkParts_ChargeOverPt,trkParts_eta->at(t),trkParts_vz->at(t));
				if (aux_TT != tower) continue; //TOWER41

//				--->Keep only positive for positive bank
				if (select_charge*trkParts_charge->at(t) ==-1 )  continue;
//				***
//				keep track of the true mu-trk that must be recognized for that event.
//				The key is the trkparts index of the mu. It's value is initialized at 0.
//				Than it will be the number of mu stub it has.

//				Also keep track if that mu is recognized or not. The map is initialized with the mu not recognized (false)
				setOfmu.insert(make_pair(t,0));
				true_mu++; // number of true mu that pass the filters.

				phi[0]->Fill(trkParts_phi->at(t));
				pt[0]->Fill(trkParts_pt->at(t));
				eta[0]->Fill(trkParts_eta->at(t));
			}
		}
//		*****

//		---> CONTINUE ONLY IF SOME AUTHETIC MU HAS BEEN FOUND
		if(setOfmu.size()>0) {
			good_entries++;
		}

		else continue;
		//go to next event

//		***********************
//		LOOP OVER ROADS FIRED
//		***********************

		roads_before_reconstruction = 0;

		n_roads_fired=AMTTRoads_patternRef->size();
		road_fired->Fill(n_roads_fired);

		if(n_roads_fired<1) {
			++n_evt_noroads;	//# of roads that has not fired
			continue; //jump to other event if this one hasn't fired.
		}

		for (Int_t patt_count=0; patt_count<n_roads_fired; ++patt_count){

			for(map<Int_t,Int_t>::iterator it = setOfmu.begin() ; it!=setOfmu.end(); it++){
				it->second=0;
			}
			genuine_mu = 0;
//			Erase the setOfLayer, before checking new road; reset combination per road
			setOflayers.erase(setOflayers.begin(),setOflayers.end());
			combination_for_a_road=1;
			roads_before_reconstruction++;

			//LOOP OVER layers for each pattern
			Int_t l=(*AMTTRoads_stubRefs)[patt_count].size();
			assert(l==6);
			for (l =0; l<6; ++l){

				//LOOP OVER STUBS LIST for that layer, in that pattern, for that event.
				n_stubs=(*AMTTRoads_stubRefs)[patt_count][l].size();
				n_stubs_for_combs=n_stubs; //use another counter for combinations
				//count combinations stubs
				if(n_stubs_for_combs==0) n_stubs_for_combs=1;
				combination_for_a_road=combination_for_a_road*n_stubs_for_combs;

				for (Int_t stub_count = 0; stub_count< n_stubs; ++stub_count){

					stub_index=(*AMTTRoads_stubRefs)[patt_count][l][stub_count];
					trk_index=TTStubs_tpId->at(stub_index);

//					if that trkpart is one of the "true" mu of the event, go ahead
					if( setOfmu.find(trk_index)== setOfmu.end()) continue;

					layer_phi_zeta = phi_zeta(TTStubs_modId->at(stub_index)); //extract layer info //a[0] is the layer
					insertYES = setOflayers.insert(layer_phi_zeta[0]);   //  do not overcount overlap;

					if(insertYES.second) {
						genuine_mu++;
						setOfmu[trk_index]++;
						//for that particular mu-trk, count the number of stubs that leaves in the road
					}
				}
			}

//			*********************************
//			Count total combinations 'til now'
//			**********************************
			combs_inf->Fill(combination_for_a_road);
			tot_combs[0]+=combination_for_a_road;
			for(int r=0;r<3;r++){
						if(patt_count<=trunc[r]) tot_combs[r+1]+=combination_for_a_road;
					}

//			keep trace of how many roads I need to throw out AM to get a reconstruction candidate.


//			*******************************
//			ROAD EFFICIENCY 4 - 5 - 6 Stub?
//			*******************************

//			setofmu< trkindex, # stub genuine seen, one for ss >
//			the iterator is the key (map.first). the value is the number of good stub.
//			CHECK IF AT LEAST ONE !!!!!! OF THE ORIGINAL TRUE MU TRACKs (USUALLU ONLY ONE, but maybe...) is recognized, if true, goto next event
//			Fill also an ntuple if anything is found

			for(map<Int_t,Int_t>::iterator it = setOfmu.begin() ; it!=setOfmu.end(); it++){

				if (it->second>=5){
					average_candidate_roads_perevt++;
					ntuple_allinfo->Fill(jentry,it->first,patt_count); //fill ntuple with good event. info as road id.
					alreadyRecognYES=setOfmu_recognized.insert(make_pair(it->first,true));
					if(alreadyRecognYES.second) {
						n_recognized_mu++; //decide if the mu has been already recognized (possible multiple good mu for event)
						for(int r=0;r<3;r++){
							if(patt_count<=trunc[r]) {
								phi[r+2]->Fill(trkParts_phi->at(it->first));
								pt[r+2]->Fill(trkParts_pt->at(it->first));
								eta[r+2]->Fill(trkParts_eta->at(it->first));
							}
						}
						phi[1]->Fill(trkParts_phi->at(it->first));
						pt[1]->Fill(trkParts_pt->at(it->first));
						eta[1]->Fill(trkParts_eta->at(it->first));
					}
				}
			}
//			if i find the roads that reconstruct all the trkparts of the event:
//			(NOT break); increase the counters; check a flag of evt_Fully rec; go ahead in such a way that
//			the counters are not increased again if you find another good road for an event that 
//			has been already recognized.

			if(setOfmu_recognized.size()==setOfmu.size() && is_evt_reconstr == false) {
				road_read_before_reconstruction[0]->Fill(roads_before_reconstruction); //no truncation
				for(int r=0;r<3;r++){
					if(patt_count<=trunc[r]) {
						road_read_before_reconstruction[r+1]->Fill(roads_before_reconstruction); //if truncation
					}
				}
				evt_fully_reconstructed++;
				is_evt_reconstr=true;
				if(parse_all_roads_combs==false) break; //if true go ahead and check all roads for good ones.
			}

//			Anyway, try to find the roads until the cutoff is reached, or the roads end.
//			break with the number of mu that has been possible to recognise.
//			**** ROADS CUTOFF ****
			if((patt_count==n_roads_fired-1 || patt_count == roads_cutoff) && is_evt_reconstr==false){
				evt_not_reconstructed++;
				ntuple_not_reconstr->Fill(jentry, setOfmu.begin()->first,trkParts_pdgId->at(setOfmu.begin()->first),trkParts_pt->at(setOfmu.begin()->first),trkParts_eta->at(setOfmu.begin()->first),trkParts_phi->at(setOfmu.begin()->first));
				break;
			}
		}

//		**********************
//		Total combs ; candidate roads per event
//		**********************
		for(int r=0;r<3;r++){
			combs_evt[r]->Fill(tot_combs[r+1]);
		}
		candid_r_evt->Fill(average_candidate_roads_perevt);


	}

//	************************
//	FINAL AVERAGE QUANTITIES
//	************************

	average_roads_fired=road_fired->GetMean();
	for(int r=0;r<4;r++){
		average_roads_before_reconstruction[r]=road_read_before_reconstruction[r]->GetMean();
			}
	average_candidate_roads_perevt=candid_r_evt->GetMean();
	Float_t efficiency=n_recognized_mu/true_mu;

	TEfficiency e;
////	**************************************
////	Ex - post efficiencies (just the number @200, @400 @800)
////	***************************************
//
//	if(roads_cutoff>800){
//		//		If no limits has been specified on roads cutoff,
//		//		compute ex post efficiencies at 200, 400, 800 and print them.
//		nroads_before_rec_cutoff(road_read_before_reconstruction,efficiency,raw_roads_eff); //do the work
//	}


//	Fill ntuple con final numbers combinations
	Float_t r_eff[3],r_eff_errUp[3],r_eff_errDown[3], tcomb[3], tcombQ95[3];
	for(int r=0;r<3;r++){
		tcomb[r]=combs_evt[r]->GetMean();
		tcombQ95[r] = quantile_single(combs_evt[r],0.95);
		r_eff[r] = 	pt[r+2]->GetEntries()/pt[0]->GetEntries();
		r_eff_errUp[r]   = 	e.ClopperPearson(pt[0]->GetEntries(),pt[r+2]->GetEntries(),0.683,true) -pt[r+2]->GetEntries()/pt[0]->GetEntries();
		r_eff_errDown[r] = -e.ClopperPearson(pt[0]->GetEntries(),pt[r+2]->GetEntries(),0.683,false)+pt[r+2]->GetEntries()/pt[0]->GetEntries();
	}

	for(int r=0;r<3;r++){
		raw_roads_combs->Fill(trunc[r],
				average_roads_fired,quantile_single(road_fired,0.95),
				average_roads_before_reconstruction[r+1],quantile_single(road_read_before_reconstruction[r+1],0.95),
				tcomb[r],tcombQ95[r], r_eff[r], r_eff_errUp[r], r_eff_errDown[r]);
	}

	cout << endl;
	cout << "********************************************" << endl;
	cout << "Global results with no road limit truncation" << endl;
	cout << "********************************************" << endl;

	cout << "CHARGE SELECTED: " << select_charge<<endl;
	cout << "stub threshold: " << stub_threshold << " *** roads cutoff: " << roads_cutoff <<endl;
	cout << "# total events read: " << nentries << endl;
	cout << "# events with at least one authentic mu: " << good_entries << endl;
	cout << "# authentic mu the have passed filters: "<< true_mu << endl;
	cout << "# authentic mu reconstructed: " << n_recognized_mu << endl;
	cout << "# evts fully reconstructed " <<evt_fully_reconstructed << endl;
	cout << "# evts not fully reconstructed (not at all or partially reconstructed): " << evt_not_reconstructed << endl;
	cout << "... average of " << average_roads_fired << " fired roads per event" << endl;
	cout << "... average of " << average_roads_before_reconstruction[0] << " roads before reconstruction @ inf roads" << " * quantile 95% " << quantile_single(road_read_before_reconstruction[0],0.95) <<endl;
	cout << "... average of " << average_candidate_roads_perevt << " candidate roads per event" << endl;
	cout << "... average of " << true_mu/nentries << " authentic mu track per event" <<endl;
	cout << "Road efficiency at ";
			if(roads_cutoff < 50000) cout<<roads_cutoff <<": "<< efficiency <<endl;
			if(roads_cutoff > 50000) cout<<"infinite cutoff: "<< efficiency <<endl;


	cout << "********************************************" << endl;
	cout << "Results with road truncation" << endl;
	cout << "********************************************" << endl;
	cout << setw(10) << "Bank name" <<
	        setw(25) << "max roads" <<
			setw(10) << "<roads>" <<
			setw(10) << "95%" <<
	        setw(10) << "<#r bef 1st>" <<
			setw(10) << "95%" <<
			setw(10) << "<combs>" <<
	        setw(10) << "95%" <<
			setw(12) << "muEffAll" <<
			setw(10) << "+" <<
			setw(10) << "-" <<'\n';
	for(int r=0;r<3;r++){
		cout << setw(10) << key <<
					setw(10) << trunc[r] <<
					setw(10) << average_roads_fired <<
					setw(10) << quantile_single(road_fired,0.95) <<
					setw(10) << average_roads_before_reconstruction[r+1] <<
					setw(10) << quantile_single(road_read_before_reconstruction[r+1],0.95) <<
					setw(10) << tcomb[r] <<
					setw(10) << tcombQ95[r] <<
					setw(10) << r_eff[r]<<
					setw(10) << r_eff_errUp[r]<<
					setw(10) << r_eff_errDown[r]<<'\n';
	}

//	*************************
//	COMPUTE EFFICIENCIES PLOT
//	*************************

	for(int r=0;r<4;r++){
		efficiency_turnon(phi[r+1],phi[0],eff_phi[r],"eff_phi"+namehisto[r+1]+name_charge,"phi",key);
		efficiency_turnon(eta[r+1],eta[0],eff_eta[r],"eff_eta"+namehisto[r+1]+name_charge,"eta",key);
		efficiency_turnon(pt[r+1],pt[0],eff_pt[r],"eff_pT"+namehisto[r+1]+name_charge,"pT",key);
	}
//	efficiency_turnon(phi[1],phi[0],eff_phi[0],"eff_phi_inf"+name_charge,"phi",key); //eff phi inf
//	efficiency_turnon(phi[2],phi[0],eff_phi[1],"eff_phi_200"+name_charge,"phi",key);
//	efficiency_turnon(phi[3],phi[0],eff_phi[2],"eff_phi_400"+name_charge,"phi",key);
//	efficiency_turnon(phi[4],phi[0],eff_phi[3],"eff_phi_800"+name_charge,"phi",key);
//
//	efficiency_turnon(eta[1],eta[0],eff_eta[0],"eff_eta_inf"+name_charge,"eta",key); //eff eta inf
//	efficiency_turnon(eta[2],eta[0],eff_eta[1],"eff_eta_200"+name_charge,"eta",key);
//	efficiency_turnon(eta[3],eta[0],eff_eta[2],"eff_eta_400"+name_charge,"eta",key);
//	efficiency_turnon(eta[4],eta[0],eff_eta[3],"eff_eta_800"+name_charge,"eta",key);
//
//	efficiency_turnon(pt[1],pt[0],eff_pt[0],"eff_pt_inf"+name_charge,"pT",key); //eff pt inf
//	efficiency_turnon(pt[2],pt[0],eff_pt[1],"eff_pt_200"+name_charge,"pT",key);
//	efficiency_turnon(pt[3],pt[0],eff_pt[2],"eff_pt_400"+name_charge,"pT",key);
//	efficiency_turnon(pt[4],pt[0],eff_pt[3],"eff_pt_800"+name_charge,"pT",key);

//	**********
//	DRAW PLOTS
//	**********
//	TurnOn
	TCanvas *c_eff_pt = new TCanvas("c_eff_pt"+name_charge);
	TCanvas *c_eff_eta = new TCanvas("c_eff_eta"+name_charge);
	TCanvas *c_eff_phi = new TCanvas("c_eff_phi"+name_charge);

	draw_efficiencies_final(eff_pt[1],eff_pt[2],eff_pt[3],eff_pt[0],c_eff_pt,key); //counter 3, 200, 400, 800; if 4 also inf, if 5 only inf.
	draw_efficiencies_final(eff_phi[1],eff_phi[2],eff_phi[3],eff_phi[0],c_eff_phi,key);
	draw_efficiencies_final(eff_eta[1],eff_eta[2],eff_eta[3],eff_eta[0],c_eff_eta,key);

//	Combinations
	TCanvas * c_combs_per_road = new TCanvas("c_combs_per_road"+name_charge);
	draw_single_histo(combs_inf, "combs per road (no truncation)", c_combs_per_road,key+name_charge);

	TCanvas * c_combs_roadlimit = new TCanvas("c_combs_roadlimit"+name_charge);
	draw_combs_road(combs_evt[0],combs_evt[1],combs_evt[2],c_combs_roadlimit,key+name_charge,3);

	TCanvas * c_road = new TCanvas("c_road"+name_charge);
	draw_single_histo(road_read_before_reconstruction[0],"#roads read before reconstruction @ infinte trunc "+key+name_charge,c_road);

	TCanvas * c_road_fired = new TCanvas("c_road_fired"+name_charge);
	draw_single_histo(road_fired,"#roads fired "+key+name_charge,c_road_fired);
}

//      **********************
//               MAIN (root -l ---> .x macro.C("roads_sdsdsdsds")
//      **********************

void readtree_roads_efficiency_turnon_full_ext_248_allroads_41(TString name = "0", TString key = "0",Int_t charge=1){

	gStyle->SetOptStat(111111);
	TString fName;
	if(name!="0" && key =="0"){
			cout << "Please provide 'key' string as 2nd arg (sf_XX-64K)" << endl;
			return;
		}
	if(name=="0"){//otherwise put it in there
//		name="roads_mu_PU200_8K_sf0.8_nz1_pt3nosort_64K_0725";
//		key = "sf0_8-64K";
		cout << "please provide file name" << endl;
		return;
		}

	cout << "______________________________" << endl;
	cout << "FILE INPUT: " << name << endl;

	fName = "../roads/"+name+".root"; //name of file root to read, with path
	if(key.Index("cmseos")!=-1) fName = "root://cmsxrootd.fnal.gov//store/user/gvidale/"+name+".root"; //for eos file in my home
	name = (charge==1)? name+"ch1" : (charge==-1)? name +"ch-1" : (charge==0)? name+"chALL" : (charge==11)? name + "ch1-1" :  "wrong charge number. [1,-1,0,11 ]";
	if(name =="wrong charge number. [1,-1,0,11 ]" ) {
		cout << name << endl;
		return;
	}


	cout << "______________________________" << endl;
	cout << "FILE OUTPUT: " << name+"_efficiency_248.root" << endl;

//	HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK

	bool isfOpen;
	TFile* f = 0; isfOpen = false;
	f = new TFile(name+"_efficiency_248.root","RECREATE");
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




