//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May  2 15:06:44 2018 by ROOT version 6.12/06
// from TTree calib/calib
// found on file: boardIP2_TPskew_peakTime100_TACslope100.root
//////////////////////////////////////////////////////////

#ifndef VMM_data_h
#define VMM_data_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class VMM_data {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           runNumber;
   Float_t         gain;
   Float_t         tacSlope;
   Float_t         peakTime;
   Float_t         dacCounts;
   Float_t         pulserCounts;
   Float_t         tpSkew;
   vector<int>     *channelTrim;
   Int_t           subhysteresis;
   Int_t           neighbor_trigger;
   Int_t           nExpected;
   vector<int>     *chip;
   vector<int>     *boardId;
   vector<vector<int> > *channel;
   vector<vector<int> > *pdo;
   vector<vector<int> > *tdo;
   vector<int>     *eventSize;
   vector<vector<int> > *threshold;
   vector<vector<int> > *flag;
   vector<vector<int> > *bcid;
   vector<vector<int> > *grayDecoded;
   vector<vector<int> > *isNeighbor;

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_gain;   //!
   TBranch        *b_tacSlope;   //!
   TBranch        *b_peakTime;   //!
   TBranch        *b_dacCounts;   //!
   TBranch        *b_pulserCounts;   //!
   TBranch        *b_tpSkew;   //!
   TBranch        *b_channelTrim;   //!
   TBranch        *b_subhysteresis;   //!
   TBranch        *b_neighbor_trigger;   //!
   TBranch        *b_nExpected;   //!
   TBranch        *b_chip;   //!
   TBranch        *b_boardId;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_pdo;   //!
   TBranch        *b_tdo;   //!
   TBranch        *b_eventSize;   //!
   TBranch        *b_threshold;   //!
   TBranch        *b_flag;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_grayDecoded;   //!
   TBranch        *b_isNeighbor;   //!

   VMM_data(TTree *tree=0);
   virtual ~VMM_data();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

inline VMM_data::VMM_data(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("boardIP2_TPskew_peakTime100_TACslope100.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("boardIP2_TPskew_peakTime100_TACslope100.root");
      }
      f->GetObject("calib",tree);

   }
   Init(tree);
}

inline VMM_data::~VMM_data()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

inline Int_t VMM_data::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
inline Long64_t VMM_data::LoadTree(Long64_t entry)
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

inline void VMM_data::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   channelTrim = 0;
   chip = 0;
   boardId = 0;
   channel = 0;
   pdo = 0;
   tdo = 0;
   eventSize = 0;
   threshold = 0;
   flag = 0;
   bcid = 0;
   grayDecoded = 0;
   isNeighbor = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("gain", &gain, &b_gain);
   fChain->SetBranchAddress("tacSlope", &tacSlope, &b_tacSlope);
   fChain->SetBranchAddress("peakTime", &peakTime, &b_peakTime);
   fChain->SetBranchAddress("dacCounts", &dacCounts, &b_dacCounts);
   fChain->SetBranchAddress("pulserCounts", &pulserCounts, &b_pulserCounts);
   fChain->SetBranchAddress("tpSkew", &tpSkew, &b_tpSkew);
   fChain->SetBranchAddress("channelTrim", &channelTrim, &b_channelTrim);
   fChain->SetBranchAddress("subhysteresis", &subhysteresis, &b_subhysteresis);
   fChain->SetBranchAddress("neighbor_trigger", &neighbor_trigger, &b_neighbor_trigger);
   fChain->SetBranchAddress("nExpected", &nExpected, &b_nExpected);
   fChain->SetBranchAddress("chip", &chip, &b_chip);
   fChain->SetBranchAddress("boardId", &boardId, &b_boardId);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("pdo", &pdo, &b_pdo);
   fChain->SetBranchAddress("tdo", &tdo, &b_tdo);
   fChain->SetBranchAddress("eventSize", &eventSize, &b_eventSize);
   fChain->SetBranchAddress("threshold", &threshold, &b_threshold);
   fChain->SetBranchAddress("flag", &flag, &b_flag);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("grayDecoded", &grayDecoded, &b_grayDecoded);
   fChain->SetBranchAddress("isNeighbor", &isNeighbor, &b_isNeighbor);
   Notify();
}

inline Bool_t VMM_data::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void VMM_data::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
inline Int_t VMM_data::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
