//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jul  4 12:48:27 2017 by ROOT version 6.08/07
// from TTree tree/
// found on file: 50M_cleaned_TT25.root
//////////////////////////////////////////////////////////

#ifndef readtree_TT_h
#define readtree_TT_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>

// Header file for the classes stored in the TTree if any.
#include <vector>

using namespace std;

class readtree_TT {
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
   vector<float>   *TTStubs_coordx;
   vector<float>   *TTStubs_coordy;
   vector<float>   *TTStubs_eta;
   vector<float>   *TTStubs_phi;
   vector<float>   *TTStubs_r;
   vector<float>   *TTStubs_trigBend;
   vector<float>   *TTStubs_z;
   vector<int>     *genParts_charge;
   vector<int>     *TTStubs_tpId;
   vector<unsigned int> *TTStubs_modId;

   // List of branches
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
   TBranch        *b_genParts_charge;   //!
   TBranch        *b_TTStubs_tpId;   //!
   TBranch        *b_TTStubs_modId;   //!

   TTree *tree;
   readtree_TT(TString fName);
   virtual ~readtree_TT();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString key);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readtree_TT_cxx
readtree_TT::readtree_TT(TString fName) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.

	//   if (tree == 0) {
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fName);
	if (!f || !f->IsOpen()) {
		f = new TFile(fName);
	}
	TDirectory * dir = (TDirectory*)f->Get(fName+TString(":/ntupler"));
	dir->GetObject("tree",tree);
	std::cout << "Initializing " << fName.Data() <<  std::endl;
	//   }
	Init(tree);
}

readtree_TT::~readtree_TT()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readtree_TT::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readtree_TT::LoadTree(Long64_t entry)
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

void readtree_TT::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
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
   genParts_charge = 0;
   TTStubs_tpId = 0;
   TTStubs_modId = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

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
   fChain->SetBranchAddress("genParts_charge", &genParts_charge, &b_genParts_charge);
   fChain->SetBranchAddress("TTStubs_tpId", &TTStubs_tpId, &b_TTStubs_tpId);
   fChain->SetBranchAddress("TTStubs_modId", &TTStubs_modId, &b_TTStubs_modId);
   Notify();
}

Bool_t readtree_TT::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readtree_TT::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readtree_TT::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readtree_TT_cxx
