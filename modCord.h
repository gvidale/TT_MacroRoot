//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 30 06:14:11 2017 by ROOT version 6.08/07
// from TTree tree/tree_modCoords
// found on file: extracted_modCoord.root
//////////////////////////////////////////////////////////

#ifndef modCord_h
#define modCord_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

using namespace std;

// Header file for the classes stored in the TTree if any.
#include "vector"

class modCord {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           module_id;
   Int_t           n_strip;
   Int_t           n_segment;
   Bool_t          is_ps;
   vector<float>   *x_coords;
   vector<float>   *y_coords;
   vector<float>   *z_coords;
   vector<float>   *r_coords;
   vector<float>   *phi_coords;
   Float_t         phi_min;
   Float_t         phi_max;

   // List of branches
   TBranch        *b_module_Id;   //!
   TBranch        *b_n_str;   //!
   TBranch        *b_n_seg;   //!
   TBranch        *b_is_ps;   //!
   TBranch        *b_x_coords;   //!
   TBranch        *b_y_coords;   //!
   TBranch        *b_z_coords;   //!
   TBranch        *b_r_coords;   //!
   TBranch        *b_phi_coords;   //!
   TBranch        *b_phi_min;   //!
   TBranch        *b_phi_max;   //!

   modCord(TTree *tree=0);
   virtual ~modCord();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef modCord_cxx
modCord::modCord(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("extracted_modCoord.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("extracted_modCoord.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

modCord::~modCord()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t modCord::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t modCord::LoadTree(Long64_t entry)
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

void modCord::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   x_coords = 0;
   y_coords = 0;
   z_coords = 0;
   r_coords = 0;
   phi_coords = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("module_id", &module_id, &b_module_Id);
   fChain->SetBranchAddress("n_strip", &n_strip, &b_n_str);
   fChain->SetBranchAddress("n_segment", &n_segment, &b_n_seg);
   fChain->SetBranchAddress("is_ps", &is_ps, &b_is_ps);
   fChain->SetBranchAddress("x_coords", &x_coords, &b_x_coords);
   fChain->SetBranchAddress("y_coords", &y_coords, &b_y_coords);
   fChain->SetBranchAddress("z_coords", &z_coords, &b_z_coords);
   fChain->SetBranchAddress("r_coords", &r_coords, &b_r_coords);
   fChain->SetBranchAddress("phi_coords", &phi_coords, &b_phi_coords);
   fChain->SetBranchAddress("phi_min", &phi_min, &b_phi_min);
   fChain->SetBranchAddress("phi_max", &phi_max, &b_phi_max);
   Notify();
}

Bool_t modCord::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void modCord::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t modCord::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef modCord_cxx
