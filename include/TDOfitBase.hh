//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun 29 19:44:57 2016 by ROOT version 5.34/25
// from TTree TDO_fit/TDO_fit
// found on file: TDO_fit.root
//////////////////////////////////////////////////////////

#ifndef TDOfitBase_h
#define TDOfitBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class TDOfitBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        MMFE8;
   Double_t        VMM;
   Double_t        CH;
   Double_t        Delay;
   Double_t        meanTDO;
   Double_t        meanTDOerr;
   Double_t        sigmaTDO;
   Double_t        sigmaTDOerr;
   Double_t        chi2;
   Double_t        prob;
   Double_t        minTDO;
   Double_t        maxTDO;

   // List of branches
   TBranch        *b_MMFE8;   //!
   TBranch        *b_VMM;   //!
   TBranch        *b_CH;   //!
   TBranch        *b_Delay;   //!
   TBranch        *b_meanTDO;   //!
   TBranch        *b_meanTDOerr;   //!
   TBranch        *b_sigmaTDO;   //!
   TBranch        *b_sigmaTDOerr;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_prob;   //!
   TBranch        *b_minTDO;   //!
   TBranch        *b_maxTDO;   //!

   TDOfitBase(TTree *tree=0);
   virtual ~TDOfitBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

inline TDOfitBase::TDOfitBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("TDO_fit.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("TDO_fit.root");
      }
      f->GetObject("TDO_fit",tree);

   }
   Init(tree);
}

inline TDOfitBase::~TDOfitBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

inline Int_t TDOfitBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t TDOfitBase::LoadTree(Long64_t entry)
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

inline void TDOfitBase::Init(TTree *tree)
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

   fChain->SetBranchAddress("MMFE8", &MMFE8, &b_MMFE8);
   fChain->SetBranchAddress("VMM", &VMM, &b_VMM);
   fChain->SetBranchAddress("CH", &CH, &b_CH);
   fChain->SetBranchAddress("Delay", &Delay, &b_Delay);
   fChain->SetBranchAddress("meanTDO", &meanTDO, &b_meanTDO);
   fChain->SetBranchAddress("meanTDOerr", &meanTDOerr, &b_meanTDOerr);
   fChain->SetBranchAddress("sigmaTDO", &sigmaTDO, &b_sigmaTDO);
   fChain->SetBranchAddress("sigmaTDOerr", &sigmaTDOerr, &b_sigmaTDOerr);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("prob", &prob, &b_prob);
   fChain->SetBranchAddress("minTDO", &minTDO, &b_minTDO);
   fChain->SetBranchAddress("maxTDO", &maxTDO, &b_maxTDO);
   Notify();
}

inline Bool_t TDOfitBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void TDOfitBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t TDOfitBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
