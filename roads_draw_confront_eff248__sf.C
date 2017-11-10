/*
 * roads_draw_confront_eff248__sf.C
 *
 *  Created on: Aug 6, 2017
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
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>

using namespace std;

void roads_draw_confront_eff248__sf(string file_list){

	typedef boost::tokenizer<boost::char_separator<char> > Tokenizer; //Class
	boost::char_separator<char> sep("_");// default constructed
	vector < vector<string> > fName;
	vector <string> name_sf(5,"");


	string line;
	ifstream myfile (file_list);
	std::string string_toparse;
	std::vector<TString> token;
	if (myfile.is_open())
	{
		cout << "reading files..." << endl;
		while ( getline(myfile,line) )
		{
			cout << line << '\n';
			string_toparse = line;
			Tokenizer tok(string_toparse, sep);
			for(Tokenizer::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter){
				token.push_back(*tok_iter);
				//		std::cout<< *tok_iter << std::endl;//maybe need to be stringed. (what the iterator cointains)
			}

			name_sf[0]=line; //full name file
			name_sf[1]=token[6];//sfbarrel
			name_sf[2]=token[7];//sf_disks
			name_sf[3]=token[5];//type ss
			name_sf[4]=token[10];//AM size

			fName.push_back(name_sf); //si piglia il puntatore al primo elemento della coppia
		}
		myfile.close();
	}
	else cout << "Unable to open file";


	// Parse the definition string -->  file list = xxxxx_sf1_sf8_xxxxxx



	//	std::cout << "tokens:" << std::endl;

	Int_t size = fName.size();



	TNtuple * eff[size];
	TGraph * gr[size];
	TCanvas * confront_eff = new TCanvas();
	TLegend * legend = new TLegend(0.4,0.3, 0.8,0.5);
	TMultiGraph *mg = new TMultiGraph();
	mg->SetTitle("road eff - comparison - 200,400,800; # road truncation; efficiency");

	confront_eff->SetGridx();
	confront_eff->SetGridy();

//	*****************************************************************
//	LOAD INPUT _efficiency.root  with all the info u need
//	*****************************************************************
	TFile *f_input[size];
	TString eff_name1, eff_name2;
	for(Int_t i=0; i<size; i++){
//		cout << fName[i][0]<<endl;
		f_input[i] = new TFile(TString(fName[i][0]));
		if (f_input[i]->IsZombie()) {
			cout << "ERROR. Not able to load the input ntuple file. Exiting..." << endl;
			return;
		}
		f_input[i]->GetObject("raw_roads_combs",eff[i]);

		eff_name1 = "barr_sf "+TString(fName[i][1])+" | disks_sf "+TString(fName[i][2]);
		eff_name2 = "type "+TString(fName[i][3])+" | AM "+TString(fName[i][4]);
//		eff[i]->SetName(eff_name);
		eff[i]->Draw("road_eff:road_truncation","","goff"); //what leafs to plot
		gr[i]= new TGraph(eff[i]->GetSelectedRows(), eff[i]->GetV2(), eff[i]->GetV1());
		gr[i]->SetMarkerStyle(21);
		gr[i]->SetMarkerSize(2);
		gr[i]->SetMarkerColor(i+1);
		gr[i]->SetLineColor(i+1); //0 is white
//		gr[i]->SetTitle(eff_name);
		gr[i]->SetFillStyle(0);
		mg->Add(gr[i],"");
		legend->AddEntry(gr[i],"#splitline{"+eff_name1+"}{"+eff_name2+"}");
	}

	mg->Draw("apl");

	legend->Draw();

	return;
}



