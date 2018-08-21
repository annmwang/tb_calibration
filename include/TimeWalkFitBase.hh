//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun 27 09:59:15 2018 by ROOT version 6.12/06
// from TTree TimeWalk_fit/TimeWalk_fit
// found on file: newData/fit_board0.root
//////////////////////////////////////////////////////////

#ifndef TimewalkFitBase_h
#define TimewalkFitBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

class TimewalkFitBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        MMFE8;
   Double_t        VMM;
   Double_t        CH;
   vector<double>  *meanTdo;
   vector<double>  *sigmaTdo;
   vector<int>     *Pdo;
   Int_t           max_pdo;
   Int_t           min_pdo;

   // List of branches
   TBranch        *b_MMFE8;   //!
   TBranch        *b_VMM;   //!
   TBranch        *b_CH;   //!
   TBranch        *b_meanTdo;   //!
   TBranch        *b_sigmaTdo;   //!
   TBranch        *b_Pdo;   //!
   TBranch        *b_max_pdo;   //!
   TBranch        *b_min_pdo;   //!

   TimewalkFitBase(TTree *tree=0);
   virtual ~TimewalkFitBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   //virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

//#ifdef TimewalkFitBase_cxx
TimewalkFitBase::TimewalkFitBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("newData/fit_board0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("newData/fit_board0.root");
      }
      f->GetObject("TimeWalk_fit",tree);

   }
   Init(tree);
}

TimewalkFitBase::~TimewalkFitBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TimewalkFitBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TimewalkFitBase::LoadTree(Long64_t entry)
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

void TimewalkFitBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   meanTdo = 0;
   sigmaTdo = 0;
   Pdo = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("MMFE8", &MMFE8, &b_MMFE8);
   fChain->SetBranchAddress("VMM", &VMM, &b_VMM);
   fChain->SetBranchAddress("CH", &CH, &b_CH);
   fChain->SetBranchAddress("meanTdo", &meanTdo, &b_meanTdo);
   fChain->SetBranchAddress("sigmaTdo", &sigmaTdo, &b_sigmaTdo);
   fChain->SetBranchAddress("Pdo", &Pdo, &b_Pdo);
   fChain->SetBranchAddress("max_pdo", &max_pdo, &b_max_pdo);
   fChain->SetBranchAddress("min_pdo", &min_pdo, &b_min_pdo);
   Notify();
}

Bool_t TimewalkFitBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TimewalkFitBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TimewalkFitBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
//#endif // #ifdef TimewalkFitBase_cxx
