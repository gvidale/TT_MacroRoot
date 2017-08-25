//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 17 12:39:38 2017 by ROOT version 6.08/07
// from TTree tree/
// found on file: roads_Tilted_tt25_sf1_nz1_pt3_100k_TT25sample.root
//////////////////////////////////////////////////////////

#ifndef readtree_roads_h
#define readtree_roads_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

// Header file for the classes stored in the TTree if any.
#include "vector"

using namespace std;

class readtree_roads {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
  
   vector<float>   *genParts_eta;
   vector<float>   *genParts_phi;
   vector<float>   *genParts_pt;
   vector<float>   *genParts_vx;
   vector<float>   *genParts_vy;
   vector<float>   *genParts_vz;
   vector<int>     *genParts_charge;
   
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_eta;
   vector<float>   *TTStubs_phi;
   vector<float>   *TTStubs_r;
   vector<float>   *TTStubs_trigBend;
   vector<float>   *TTStubs_z;
   vector<int>     *TTStubs_tpId;					// stub tracking particle ID
   vector<unsigned int> *TTStubs_clusRef0;
   vector<unsigned int> *TTStubs_clusRef1;
   vector<unsigned int> *TTStubs_modId;
   vector<string>  *TTStubs_bitString;
   vector<unsigned int> *TTStubs_superstripId;
   
   vector<float>   *trkParts_eta;
   vector<float>   *trkParts_phi;
   vector<float>   *trkParts_pt;
   vector<float>   *trkParts_vx;
   vector<float>   *trkParts_vy;
   vector<float>   *trkParts_vz;
   vector<bool>    *trkParts_intime;	// is particle from BX 0?
   vector<bool>    *trkParts_primary;   // is particle produced at a primary vertex?
   vector<bool>    *trkParts_signal;	// is particle from hard scattering vertex?
   vector<int>     *trkParts_charge;	 
   vector<int>     *trkParts_pdgId;		// particle pdgID
   
   vector<unsigned int> *AMTTRoads_patternRef;						// road pattern id
   vector<unsigned int> *AMTTRoads_tower;                           // road tower
   vector<unsigned int> *AMTTRoads_nstubs;                          // road total number of stubs
   vector<float>   *AMTTRoads_patternInvPt;                         // road pattern q/pT [1/GeV]
   vector<unsigned int> *AMTTRoads_patternFreq;                     // road pattern popularity
   vector<vector<unsigned int> > *AMTTRoads_superstripIds;          // road superstrip ids
   vector<vector<vector<unsigned int> > > *AMTTRoads_stubRefs;      // road stub ids

   // List of branches
   TBranch        *b_trkParts_intime;   //!
   TBranch        *b_trkParts_primary;   //!
   TBranch        *b_trkParts_signal;   //!
   TBranch        *b_genParts_eta;   //!
   TBranch        *b_genParts_phi;   //!
   TBranch        *b_genParts_pt;   //!
   TBranch        *b_genParts_vx;   //!
   TBranch        *b_genParts_vy;   //!
   TBranch        *b_genParts_vz;   //!
   TBranch        *b_TTStubs_coordx;   //!
   TBranch        *b_TTStubs_coordy;   //!
   TBranch        *b_TTStubs_eta;   //!
   TBranch        *b_TTStubs_phi;   //!
   TBranch        *b_TTStubs_r;   //!
   TBranch        *b_TTStubs_trigBend;   //!
   TBranch        *b_TTStubs_z;   //!
   TBranch        *b_trkParts_eta;   //!
   TBranch        *b_trkParts_phi;   //!
   TBranch        *b_trkParts_pt;   //!
   TBranch        *b_trkParts_vx;   //!
   TBranch        *b_trkParts_vy;   //!
   TBranch        *b_trkParts_vz;   //!
   TBranch        *b_genParts_charge;   //!
   TBranch        *b_TTStubs_tpId;   //!
   TBranch        *b_trkParts_charge;   //!
   TBranch        *b_trkParts_pdgId;   //!
   TBranch        *b_TTStubs_clusRef0;   //!
   TBranch        *b_TTStubs_clusRef1;   //!
   TBranch        *b_TTStubs_modId;   //!
   TBranch        *b_TTStubs_bitString;   //!
   TBranch        *b_TTStubs_superstripId;   //!
   TBranch        *b_AMTTRoads_patternRef;   //!
   TBranch        *b_AMTTRoads_tower;   //!
   TBranch        *b_AMTTRoads_nstubs;   //!
   TBranch        *b_AMTTRoads_patternInvPt;   //!
   TBranch        *b_AMTTRoads_patternFreq;   //!
   TBranch        *b_AMTTRoads_superstripIds;   //!
   TBranch        *b_AMTTRoads_stubRefs;   //!

   TTree *tree;
   readtree_roads(TString fName);
   virtual ~readtree_roads();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString key,Int_t charge);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readtree_roads_cxx
readtree_roads::readtree_roads(TString fName) : fChain(0)
{


//	   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fName);
//	   if (!f || !f->IsOpen()) {
//		   f = new TFile(fName);
//	   }

	   TFile *f = TFile::Open(fName);
	   if (f->IsZombie()) {
		   cout << "ERROR. Not able to load the input file. Exiting..." << endl;
		   return;
	   }
//	   TDirectory * dir = (TDirectory*)f->Get(fName+TString(":/ntupler"));
	   f->GetObject("/ntupler/tree",tree);
	   cout << "Initializing " << fName.Data() <<  endl;

	   Init(tree);
}

readtree_roads::~readtree_roads()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();

}

Int_t readtree_roads::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readtree_roads::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void readtree_roads::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   trkParts_intime = 0;
   trkParts_primary = 0;
   trkParts_signal = 0;
   genParts_eta = 0;
   genParts_phi = 0;
   genParts_pt = 0;
   genParts_vx = 0;
   genParts_vy = 0;
   genParts_vz = 0;
   TTStubs_coordx = 0;
   TTStubs_coordy = 0;
   TTStubs_eta = 0;
   TTStubs_phi = 0;
   TTStubs_r = 0;
   TTStubs_trigBend = 0;
   TTStubs_z = 0;
   trkParts_eta = 0;
   trkParts_phi = 0;
   trkParts_pt = 0;
   trkParts_vx = 0;
   trkParts_vy = 0;
   trkParts_vz = 0;
   genParts_charge = 0;
   TTStubs_tpId = 0;
   trkParts_charge = 0;
   trkParts_pdgId = 0;
   TTStubs_clusRef0 = 0;
   TTStubs_clusRef1 = 0;
   TTStubs_modId = 0;
   TTStubs_bitString = 0;
   TTStubs_superstripId = 0;
   AMTTRoads_patternRef = 0;
   AMTTRoads_tower = 0;
   AMTTRoads_nstubs = 0;
   AMTTRoads_patternInvPt = 0;
   AMTTRoads_patternFreq = 0;
   AMTTRoads_superstripIds = 0;
   AMTTRoads_stubRefs = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("trkParts_intime", &trkParts_intime, &b_trkParts_intime);
   fChain->SetBranchAddress("trkParts_primary", &trkParts_primary, &b_trkParts_primary);
   fChain->SetBranchAddress("trkParts_signal", &trkParts_signal, &b_trkParts_signal);
   fChain->SetBranchAddress("genParts_eta", &genParts_eta, &b_genParts_eta);
   fChain->SetBranchAddress("genParts_phi", &genParts_phi, &b_genParts_phi);
   fChain->SetBranchAddress("genParts_pt", &genParts_pt, &b_genParts_pt);
   fChain->SetBranchAddress("genParts_vx", &genParts_vx, &b_genParts_vx);
   fChain->SetBranchAddress("genParts_vy", &genParts_vy, &b_genParts_vy);
   fChain->SetBranchAddress("genParts_vz", &genParts_vz, &b_genParts_vz);
   fChain->SetBranchAddress("TTStubs_coordx", &TTStubs_coordx, &b_TTStubs_coordx);
   fChain->SetBranchAddress("TTStubs_coordy", &TTStubs_coordy, &b_TTStubs_coordy);
   fChain->SetBranchAddress("TTStubs_eta", &TTStubs_eta, &b_TTStubs_eta);
   fChain->SetBranchAddress("TTStubs_phi", &TTStubs_phi, &b_TTStubs_phi);
   fChain->SetBranchAddress("TTStubs_r", &TTStubs_r, &b_TTStubs_r);
   fChain->SetBranchAddress("TTStubs_trigBend", &TTStubs_trigBend, &b_TTStubs_trigBend);
   fChain->SetBranchAddress("TTStubs_z", &TTStubs_z, &b_TTStubs_z);
   fChain->SetBranchAddress("trkParts_eta", &trkParts_eta, &b_trkParts_eta);
   fChain->SetBranchAddress("trkParts_phi", &trkParts_phi, &b_trkParts_phi);
   fChain->SetBranchAddress("trkParts_pt", &trkParts_pt, &b_trkParts_pt);
   fChain->SetBranchAddress("trkParts_vx", &trkParts_vx, &b_trkParts_vx);
   fChain->SetBranchAddress("trkParts_vy", &trkParts_vy, &b_trkParts_vy);
   fChain->SetBranchAddress("trkParts_vz", &trkParts_vz, &b_trkParts_vz);
   fChain->SetBranchAddress("genParts_charge", &genParts_charge, &b_genParts_charge);
   fChain->SetBranchAddress("TTStubs_tpId", &TTStubs_tpId, &b_TTStubs_tpId);
   fChain->SetBranchAddress("trkParts_charge", &trkParts_charge, &b_trkParts_charge);
   fChain->SetBranchAddress("trkParts_pdgId", &trkParts_pdgId, &b_trkParts_pdgId);
   fChain->SetBranchAddress("TTStubs_clusRef0", &TTStubs_clusRef0, &b_TTStubs_clusRef0);
   fChain->SetBranchAddress("TTStubs_clusRef1", &TTStubs_clusRef1, &b_TTStubs_clusRef1);
   fChain->SetBranchAddress("TTStubs_modId", &TTStubs_modId, &b_TTStubs_modId);
   fChain->SetBranchAddress("TTStubs_bitString", &TTStubs_bitString, &b_TTStubs_bitString);
   fChain->SetBranchAddress("TTStubs_superstripId", &TTStubs_superstripId, &b_TTStubs_superstripId);
   fChain->SetBranchAddress("AMTTRoads_patternRef", &AMTTRoads_patternRef, &b_AMTTRoads_patternRef);
   fChain->SetBranchAddress("AMTTRoads_tower", &AMTTRoads_tower, &b_AMTTRoads_tower);
   fChain->SetBranchAddress("AMTTRoads_nstubs", &AMTTRoads_nstubs, &b_AMTTRoads_nstubs);
   fChain->SetBranchAddress("AMTTRoads_patternInvPt", &AMTTRoads_patternInvPt, &b_AMTTRoads_patternInvPt);
   fChain->SetBranchAddress("AMTTRoads_patternFreq", &AMTTRoads_patternFreq, &b_AMTTRoads_patternFreq);
   fChain->SetBranchAddress("AMTTRoads_superstripIds", &AMTTRoads_superstripIds, &b_AMTTRoads_superstripIds);
   fChain->SetBranchAddress("AMTTRoads_stubRefs", &AMTTRoads_stubRefs, &b_AMTTRoads_stubRefs);
   Notify();
}

Bool_t readtree_roads::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readtree_roads::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readtree_roads::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readtree_roads_cxx
