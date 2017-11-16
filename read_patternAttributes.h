//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 12 21:05:35 2017 by ROOT version 6.08/07
// from TTree patternAttributes/
// found on file: patternBankTilted_tt25_sf1_nz1_pt3_attributes_0712.root
//////////////////////////////////////////////////////////

#ifndef read_patternAttributes_h
#define read_patternAttributes_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class read_patternAttributes {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         invPt_mean;
   Float_t         invPt_sigma;
   Float_t         cotTheta_mean;
   Float_t         cotTheta_sigma;
   Float_t         phi_mean;
   Float_t         phi_sigma;
   Float_t         z0_mean;
   Float_t         z0_sigma;

   // List of branches
   TBranch        *b_invPt_mean;   //!
   TBranch        *b_invPt_sigma;   //!
   TBranch        *b_cotTheta_mean;   //!
   TBranch        *b_cotTheta_sigma;   //!
   TBranch        *b_phi_mean;   //!
   TBranch        *b_phi_sigma;   //!
   TBranch        *b_z0_mean;   //!
   TBranch        *b_z0_sigma;   //!

   TTree * tree;
   read_patternAttributes(TString fName);
   virtual ~read_patternAttributes();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString key);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef read_patternAttributes_cxx
read_patternAttributes::read_patternAttributes(TString fName) : fChain(0)
{


	   TFile *f = TFile::Open(fName);
	   if (!f || !f->IsOpen()) {
		   f = new TFile(fName);
	   }
      f->GetObject("patternAttributes",tree);


   Init(tree);
}

read_patternAttributes::~read_patternAttributes()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t read_patternAttributes::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t read_patternAttributes::LoadTree(Long64_t entry)
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

void read_patternAttributes::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("invPt_mean", &invPt_mean, &b_invPt_mean);
   fChain->SetBranchAddress("invPt_sigma", &invPt_sigma, &b_invPt_sigma);
   fChain->SetBranchAddress("cotTheta_mean", &cotTheta_mean, &b_cotTheta_mean);
   fChain->SetBranchAddress("cotTheta_sigma", &cotTheta_sigma, &b_cotTheta_sigma);
   fChain->SetBranchAddress("phi_mean", &phi_mean, &b_phi_mean);
   fChain->SetBranchAddress("phi_sigma", &phi_sigma, &b_phi_sigma);
   fChain->SetBranchAddress("z0_mean", &z0_mean, &b_z0_mean);
   fChain->SetBranchAddress("z0_sigma", &z0_sigma, &b_z0_sigma);
   Notify();
}

Bool_t read_patternAttributes::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void read_patternAttributes::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t read_patternAttributes::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef read_patternAttributes_cxx
