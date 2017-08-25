#define read_patternBank_siblings2C_cxx
#include "read_patternBank.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <algorithm>
#include <utility>



void read_patternBank::Loop(TString key)
{

	vector<unsigned int> id;
	Float_t running_siblings_perpattern=0;
	Float_t siblings_perpattern=0;
	Int_t count = 0;
	

	Int_t npattern_interval=0;
	Int_t min[3] = {0,59000,107000}; //intervals of 500 near boundaries of coverage ( 95% 99%)
	Int_t max[3] = {500,59500,107500};
	vector<TString> intervals = {"most frequent", "95%", "99%"};//first interval is just the most frequent one...
	Int_t interval=0;

	if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   Long64_t jentry =0;

   cout << "estimating mean # siblings... every '.' is 100 pattern read..." << endl;

  while(jentry>=min[interval] && jentry < max[interval]) {

	  npattern_interval++;

	   
	   id.clear();

	   Long64_t ientry = LoadTree(jentry);
	   if (ientry < 0) break;
	   nb = fChain->GetEntry(jentry);   nbytes += nb;
	   // if (Cut(ientry) < 0) continue;

	   Int_t size = superstripIds->size();
	   for(Int_t i = 0; i<size;++i){
		   id.push_back(superstripIds->at(i));
	   }


	   //      COUNT HOW MANY SIBLING THAT PATTERN HAS; CYCLE THROUGH ALL OTHER PATTERN
	   siblings_perpattern=0;

	   Long64_t nbytes1 = 0, nb1 = 0;
	   for (Long64_t jentry1=0; jentry1<nentries;jentry1++) {
		   //if(find(already_sibling.begin(),already_sibling.end(),jentry1)!= already_sibling.end() ) continue;
		   if(jentry1==jentry) continue;

		   count = 0;

		   Long64_t ientry1 = LoadTree(jentry1);
		   if (ientry1 < 0) break;
		   nb1 = fChain->GetEntry(jentry1);   nbytes1 += nb1;
		   // if (Cut(ientry) < 0) continue;

		   Int_t size1 = superstripIds->size();
		   for(Int_t i = 0; i<size1;++i){
			   if(superstripIds->at(i)==id.at(i)) count++;

			   }

		   if(count>=5) {
			   siblings_perpattern++;
//			   already_sibling.push_back(jentry1);
		   }

	   }

	   running_siblings_perpattern=(running_siblings_perpattern+siblings_perpattern);


	   jentry++;

	   if (jentry%100==0) cout << flush << "@";

	   if (jentry==max[interval]) {
		   cout<<"Pattern interval " << intervals[interval] << ": [ " << min[interval] << " : " << max[interval] << " ]  --- mean # siblings: " << running_siblings_perpattern / npattern_interval << endl;
		   interval++;
		   jentry=min[interval];
		   npattern_interval=0;
		   running_siblings_perpattern=0;
	   }
	   if (interval>2)break;
   }



}




//***** MAIN  *******
void read_patternBank_siblings2C(){


	gStyle->SetOptStat(111111);
	TString key;
	TString fName;
	TString name;


	name="patternBankTilted_tt25_sf1_nz1_pt3_attributes_0712"; //master name of file root

	fName = "../patternBank/"+name+".root"; //name of file root to read, with path
	key = "_"+name;     //just a key for histos

	//HERE I DEFINE OUTPUT .ROOT WITH THE HISTOGRAMS TO CHECK
	//file di output contenente gli istogrammi

//	bool isfOpen;
//	TFile* f = 0; isfOpen = false;
//	f = new TFile(name+"_siblings.root","RECREATE");
//	isfOpen = f->IsOpen();
//	if (!isfOpen) {
//		cout << "ERROR. Not able to load the confrontoBranches file. Exiting..." << endl;
//		return;
//	}


	read_patternBank a(fName);
//	f -> cd();
	a.Loop(key);

	//gDirectory->pwd();
//	f -> Write();

	//cout << "ho scritto" << endl;

	//	f -> Close();   //IF CLOSE CANNOT SHOW PLOT, BUT CAN DO OPERATIONS!! OK


	return;


}
