//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 12 13:59:04 2017 by ROOT version 6.08/07
// from TTree patternBank/
// found on file: patternBankTilted_tt25_sf1_nz1_pt3_TEST_0712.root
//////////////////////////////////////////////////////////

#ifndef read_patternBank_h
#define read_patternBank_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
using namespace std;

class read_patternBank {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UShort_t        frequency;
   vector<unsigned int> *superstripIds;

   // List of branches
   TBranch        *b_frequency;   //!
   TBranch        *b_superstripIds;   //!

   TTree * tree;
   read_patternBank(TString fName);
   virtual ~read_patternBank();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString key);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef read_patternBank_cxx
read_patternBank::read_patternBank(TString fName) : fChain(0)
{


	   TFile *f = TFile::Open(fName);
	   if (!f || !f->IsOpen()) {
		   f = new TFile(fName);
	   }
      f->GetObject("patternBank",tree);


   Init(tree);
}

read_patternBank::~read_patternBank()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t read_patternBank::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t read_patternBank::LoadTree(Long64_t entry)
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

void read_patternBank::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   superstripIds = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("frequency", &frequency, &b_frequency);
   fChain->SetBranchAddress("superstripIds", &superstripIds, &b_superstripIds);
   Notify();
}

Bool_t read_patternBank::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void read_patternBank::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t read_patternBank::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef read_patternBank_cxx
