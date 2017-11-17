//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 23 12:32:36 2017 by ROOT version 6.08/07
// from TTree StripCoords/
// found on file: extracted.root
//////////////////////////////////////////////////////////

#ifndef stripcoord_h
#define stripcoord_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class stripcoord {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           layer;
   Int_t           module;
   Int_t           ladder;
   Int_t           modid;
   Float_t         xm;
   Float_t         ym;
   Float_t         zm;
   Float_t         strip_dx;
   Float_t         strip_dy;
   Float_t         strip_dz;
   Float_t         seg_dx;
   Float_t         seg_dy;
   Float_t         seg_dz;

   // List of branches
   TBranch        *b_layer;   //!
   TBranch        *b_module;   //!
   TBranch        *b_ladder;   //!
   TBranch        *b_modid;   //!
   TBranch        *b_xm;   //!
   TBranch        *b_ym;   //!
   TBranch        *b_zm;   //!
   TBranch        *b_strip_dx;   //!
   TBranch        *b_strip_dy;   //!
   TBranch        *b_strip_dz;   //!
   TBranch        *b_seg_dx;   //!
   TBranch        *b_seg_dy;   //!
   TBranch        *b_seg_dz;   //!

   stripcoord(TTree *tree=0);
   virtual ~stripcoord();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef stripcoord_cxx
stripcoord::stripcoord(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("extracted.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("extracted.root");
      }
      f->GetObject("StripCoords",tree);

   }
   Init(tree);
}

stripcoord::~stripcoord()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t stripcoord::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t stripcoord::LoadTree(Long64_t entry)
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

void stripcoord::Init(TTree *tree)
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

   fChain->SetBranchAddress("layer", &layer, &b_layer);
   fChain->SetBranchAddress("module", &module, &b_module);
   fChain->SetBranchAddress("ladder", &ladder, &b_ladder);
   fChain->SetBranchAddress("modid", &modid, &b_modid);
   fChain->SetBranchAddress("xm", &xm, &b_xm);
   fChain->SetBranchAddress("ym", &ym, &b_ym);
   fChain->SetBranchAddress("zm", &zm, &b_zm);
   fChain->SetBranchAddress("strip_dx", &strip_dx, &b_strip_dx);
   fChain->SetBranchAddress("strip_dy", &strip_dy, &b_strip_dy);
   fChain->SetBranchAddress("strip_dz", &strip_dz, &b_strip_dz);
   fChain->SetBranchAddress("seg_dx", &seg_dx, &b_seg_dx);
   fChain->SetBranchAddress("seg_dy", &seg_dy, &b_seg_dy);
   fChain->SetBranchAddress("seg_dz", &seg_dz, &b_seg_dz);
   Notify();
}

Bool_t stripcoord::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void stripcoord::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t stripcoord::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef stripcoord_cxx
