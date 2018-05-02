#include <iostream>

#include "include/setstyle.hh"
#include "include/fit_functions.hh"
#include "../include/xADCBase.hh"

using namespace std;

const double fC_per_xADC_count = 0.3 / 4.096;

int main(int argc, char* argv[]){
  setstyle();

  char inputFileName[400];
  char outputFileName[400];

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input .root file";
    cout << " and an (optional) output filename" << endl;
    cout << "Example:   ./Fit_xADC input_file.root" << endl;
    cout << "Example:   ./Fit_xADC input_file.root -o output_file.root" << endl;
    return 1;
  }

  bool user_output = false;
  for (int i=0;i<argc;i++){
    sscanf(argv[1],"%s", inputFileName);
    if (strncmp(argv[i],"-o",2)==0){
      sscanf(argv[i+1],"%s", outputFileName);
      user_output = true;
    }
  }

  string output_name;
  if(!user_output){
    string input_name = string(inputFileName);
    // strip path from input name
    while(input_name.find("/") != string::npos)
      input_name.erase(input_name.begin(),
		       input_name.begin()+
		       input_name.find("/")+1);
    if(input_name.find(".root") != string::npos)
      input_name.erase(input_name.find(".root"),5);
    output_name = input_name+"_xADCfit.root";
    sprintf(outputFileName,"%s.root",inputFileName);
  } else {
    output_name = string(outputFileName);
  }

  TChain* tree = new TChain("calib");
  tree->AddFile(inputFileName);

  xADCBase* base = new xADCBase(tree);

  int N = tree->GetEntries();
  if(N == 0) return 0;

  map<pair<int,int>, int> MMFE8VMM_to_index;

  vector<map<int, TH1D*> > vDAC_to_hist;
  
  vector<int>             vMMFE8;  // board ID
  vector<int>             vVMM;    // VMM number
  vector<vector<TH1D*> >  vhist;   // xADC histograms
  vector<vector<int> >    vDAC;    // DAC values
  vector<vector<string> > vlabel;  // labels

  int MMFE8;
  int VMM;
  int DAC;
  for (int i = 0; i < N; i++){
    base->GetEntry(i);

    MMFE8 = base->boardId;
    VMM   = base->chip;
    DAC   = base->pulserCounts;

    // add a new vDAC_to_hist map if it 
    // is a new MMFE8+VMM combination
    if(MMFE8VMM_to_index.count(pair<int,int>(MMFE8,VMM)) == 0){
      vDAC_to_hist.push_back(map<int,TH1D*>());
      int ind = int(vDAC_to_hist.size())-1;
      MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)] = ind;
      vMMFE8.push_back(MMFE8);
      vVMM.push_back(VMM);
      vhist.push_back(vector<TH1D*>());
      vDAC.push_back(vector<int>());
      vlabel.push_back(vector<string>());
    }
    
    // MMFE8+VMM index
    int index = MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)];
    
    // add a new histogram if this DAC
    // combination is new for the MMFE8+VMM combo
    if(vDAC_to_hist[index].count(DAC) == 0){
      char sDAC[20];
      sprintf(sDAC,"%d",DAC);
      char shist[20];
      sprintf(shist,"%d_%d_%d",MMFE8,VMM,DAC);
      TH1D* hist = new TH1D(("h_"+string(shist)).c_str(),
                            ("h_"+string(shist)).c_str(),
                            4096, -0.5, 4095.5);
      vDAC_to_hist[index][DAC] = hist;
      vhist[index].push_back(hist);
      vDAC[index].push_back(DAC);
      vlabel[index].push_back("DAC = "+string(sDAC));
    }
    if (base->samples > 40.) {
      vDAC_to_hist[index][DAC]->Fill(base->samples); // get rid of baseline
    }
  }

  int Nindex = vMMFE8.size();

  TFile* fout = new TFile(output_name.c_str(), "RECREATE");

  // add plots of xADC values for each
  // MMFE8+VMM combo to output file
  fout->mkdir("xADC_plots");
  fout->cd("xADC_plots");
  for(int i = 0; i < Nindex; i++){
    char stitle[50];
    sprintf(stitle, "Board #%d, VMM #%d", vMMFE8[i], vVMM[i]);
    char scan[50];
    sprintf(scan, "c_xADC_Board%d_VMM%d", vMMFE8[i], vVMM[i]);
    TCanvas* can = Plot_1D(scan, vhist[i], "xADC Readout", "Count", stitle, vlabel[i]);
    can->Write();
    delete can;
  }
  fout->cd("");

  // write xADCBase tree to outputfile
  //TTree* newtree = tree->CloneTree();
  //fout->cd();
  //newtree->Write();
  //delete newtree;
  delete base;
  delete tree;

  // Output xADC fit tree
  double fit_MMFE8;
  double fit_VMM;
  double fit_DAC;
  double fit_meanQ;
  double fit_meanQerr;
  double fit_sigmaQ;
  double fit_sigmaQerr;
  double fit_chi2;
  double fit_prob;
  
  TTree* fit_tree = new TTree("xADC_fit", "xADC_fit");
  fit_tree->Branch("MMFE8", &fit_MMFE8);
  fit_tree->Branch("VMM", &fit_VMM);
  fit_tree->Branch("DAC", &fit_DAC);
  fit_tree->Branch("meanQ", &fit_meanQ);
  fit_tree->Branch("meanQerr", &fit_meanQerr);
  fit_tree->Branch("sigmaQ", &fit_sigmaQ);
  fit_tree->Branch("sigmaQerr", &fit_sigmaQerr);
  fit_tree->Branch("chi2", &fit_chi2);
  fit_tree->Branch("prob", &fit_prob);

  // Perform fits on each vector of MMFE8+VMM histograms
  fout->mkdir("xADCfit_plots");
  fout->cd("xADCfit_plots");

  vector<TF1*> vfunc;

  for(int index = 0; index < Nindex; index++){
    char sfold[50];
    sprintf(sfold, "xADCfit_plots/Board%d_VMM%d", vMMFE8[index], vVMM[index]);
    //sprintf(sfold, "Board%d_VMM%d", vMMFE8[index], vVMM[index]);
    fout->mkdir(sfold);
    fout->cd(sfold);

    int Ndac = vhist[index].size();

    // check to see whether xADC histograms actually have 
    // two peaks for higher DAC
    bool two_peak = false;

    for(int d = 0; d < Ndac; d++){

      char stitle[50];
      sprintf(stitle, "Board #%d, VMM #%d, DAC = %d", vMMFE8[index], vVMM[index], vDAC[index][d]);
      char scan[50];
      sprintf(scan, "c_xADC_Board%d_VMM%d_DAC%d", vMMFE8[index], vVMM[index], vDAC[index][d]);
      TCanvas* can = Plot_1D(scan, vhist[index][d], "xADC Readout", "Count", stitle);
      can->Write();
      delete can;

      double I = vhist[index][d]->Integral();

      fit_MMFE8 = vMMFE8[index];
      fit_VMM = vVMM[index];
      fit_DAC = vDAC[index][d];
      fit_meanQ = vhist[index][d]->GetMean()*fC_per_xADC_count;
      fit_meanQerr = vhist[index][d]->GetRMS()*fC_per_xADC_count/sqrt(I);
      fit_sigmaQ = vhist[index][d]->GetRMS()*fC_per_xADC_count;
      fit_sigmaQerr = vhist[index][d]->GetRMSError()*fC_per_xADC_count;
      fit_chi2 = 1.;
      fit_prob = 0.;
      
      fit_tree->Fill();

    }

    char stitle[50];
    sprintf(stitle, "Board #%d, VMM #%d", vMMFE8[index], vVMM[index]);
    char scan[50];
    sprintf(scan, "c_xADC_Board%d_VMM%d_all", vMMFE8[index], vVMM[index]);
    TCanvas* can = Plot_1D(scan, vhist[index], "xADC Readout", "Count", stitle, vlabel[index]);
    can->Write();
    delete can;
    fout->cd("xADCfit_plots");
  }

  // Write fit_tree to output file
  fout->cd("");
  fit_tree->Write();
  fout->Close();
}
