#include <iostream>

#include "include/setstyle.hh"
#include "include/fit_functions.hh"
#include "include/VMM_data.hh"

using namespace std;

int main(int argc, char* argv[]){
  setstyle();

  char inputFileName[400];
  char outputFileName[400];

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input .root file ";
    cout << "and an (optional) output filename" << endl;
    cout << "Example:   ./Fit_TimeWalk input_file.root" << endl;
    cout << "Example:   ./Fit_TimeWalk input_file.root -o output_file.root" << endl;
    return 1;
  }

  sscanf(argv[1],"%s", inputFileName);
  bool user_output = false;
  for (int i=0;i<argc;i++){
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
    output_name = input_name+"_TimeWalkfit.root";
    sprintf(outputFileName,"%s.root",inputFileName);
  } else {
    output_name = string(outputFileName);
  }

  TChain* tree = new TChain("calib");
  tree->AddFile(inputFileName);

  VMM_data* base = new VMM_data(tree);

  int N = tree->GetEntries();
  if(N == 0) return 0;

  map<pair<int,int>, int> MMFE8VMM_to_index;
  vector<map<int,int> >   vCH_to_index;
  vector<map<int,int> >   vCH_to_maxDAC;

  vector<vector<map<int, TH1D*> > > vDelay_to_hist;
  
  vector<int>             vMMFE8;               // board ID
  vector<int>             vVMM;                 // VMM number
  vector<vector<int> >    vCH;                  // channel number
  vector<vector<TH2D*> >           vhist_all;   // all TimeWalk histograms
  vector<vector<TH1D*> >           vhist_tdo;   // all TimeWalk histograms
  vector<vector<TH2D*> >           vhist_all_corr;   // all TimeWalk histograms, corrected
  vector<vector<TProfile*> >           vhist_all_corr_prof;   // all TimeWalk histograms, corrected
  vector<vector<vector<TH1D*> > >  vhist;              // TimeWalk histograms by delay
  vector<vector<vector<int> > >    vDelay;      // Delay value
  vector<vector<vector<string> > > vlabel;      // label
  vector<vector<int> >           tdo_min;   // tdo min
  vector<vector<int> >           tdo_max;   // tdo max
  vector<vector<double> >           vhist_all_rms; // rms

  int MMFE8;
  int VMM;
  int Delay;
  int CH;
  double min_TDO;
  double max_TDO;

  cout << "Processing Trees" << endl;
  for (int i = 0; i < N; i++){
    base->GetEntry(i);
    
    for (int j = 0; j < base->chip->size(); j++){ 

      for (int k = 0; k < base->channel->at(j).size(); k++){

        if(base->pdo->at(j).at(k) <=  0)
          continue;

        if(base->pdo->at(j).at(k) >=  1023)
          continue;

        if(base->tdo->at(j).at(k) <=  0)
          continue;

        MMFE8 = base->boardId->at(j);
        VMM   = base->chip->at(j);
        //Delay = base->Delay;
        Delay = (int)base->tpSkew;
        CH    = base->channel->at(j).at(k);
    
//         if (Delay > 4)
//           continue;

        // add a new MMFE8+VMM entry if
        // is a new MMFE8+VMM combination
        if(MMFE8VMM_to_index.count(pair<int,int>(MMFE8,VMM)) == 0){
          int ind = int(vMMFE8.size());
          MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)] = ind;
          vMMFE8.push_back(MMFE8);
          vVMM.push_back(VMM);
          vCH.push_back(vector<int>());
          vCH_to_index.push_back(map<int,int>());
          vCH_to_maxDAC.push_back(map<int,int>());
          vDelay_to_hist.push_back(vector<map<int,TH1D*> >());
          vhist.push_back(vector<vector<TH1D*> >());
          vDelay.push_back(vector<vector<int> >());
          vlabel.push_back(vector<vector<string> >());
          vhist_all.push_back(vector<TH2D*>());
          vhist_tdo.push_back(vector<TH1D*>());
          vhist_all_corr.push_back(vector<TH2D*>());
          vhist_all_corr_prof.push_back(vector<TProfile*>());
          tdo_min.push_back(vector<int>());
          tdo_max.push_back(vector<int>());
        }
        
        // MMFE8+VMM index
        int index = MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)];
        
        // add a new channel if is new for
        // this MMFE8+VMM combination
        if(vCH_to_index[index].count(CH) == 0){
          int ind = int(vCH[index].size());
          vCH_to_index[index][CH] = ind;
          vCH_to_maxDAC[index][CH] = base->pulserCounts;
          vCH[index].push_back(CH);
          vDelay_to_hist[index].push_back(map<int,TH1D*>());
          vhist[index].push_back(vector<TH1D*>());
          vDelay[index].push_back(vector<int>());
          vlabel[index].push_back(vector<string>());
          char shist_tdo[20];
          sprintf(shist_tdo,"%d_%d_%d_tdo",MMFE8,VMM,CH);
          TH1D* hi = new TH1D(("h_"+string(shist_tdo)).c_str(),
                                ("h_"+string(shist_tdo)).c_str(),
                                256, -0.5, 255.5);
          hi->Sumw2();
          vhist_tdo[index].push_back(hi);

          char shist_all[20];
          sprintf(shist_all,"%d_%d_%d_all",MMFE8,VMM,CH);
          TH2D* hist = new TH2D(("h_"+string(shist_all)).c_str(),
                                ("h_"+string(shist_all)).c_str(),
                                1024, -0.5, 1023.5,
                                256, -0.5, 255.5);
          hist->Sumw2();
          vhist_all[index].push_back(hist);

          char shist2d_all[20];
          sprintf(shist2d_all,"%d_%d_%d_all_corr",MMFE8,VMM,CH);
          TH2D* hist2d = new TH2D(("h_"+string(shist2d_all)).c_str(),
                                  ("h_"+string(shist2d_all)).c_str(),
                                  1024, -0.5, 1023.5,
                                  256, -0.5, 255.5);
          hist2d->Sumw2();
          vhist_all_corr[index].push_back(hist2d);

          char sprof[20];
          sprintf(sprof,"%d_%d_%d_all_corr_prof",MMFE8,VMM,CH);
//           TProfile* prof = new TProfile(("h_"+string(sprof)).c_str(),
//                                   ("h_"+string(sprof)).c_str(),
//                                   1024, -0.5, 1023.5,
//                                   256, -0.5, 255.5);
          //vhist_all_corr_prof[index].push_back(prof);

          tdo_min[index].push_back(256);
          tdo_max[index].push_back(-1);
        }
        
        // CH index
        int cindex = vCH_to_index[index][CH];
        
        //vhist_all[index][cindex]->Fill(base->TimeWalk);
        
        if(base->pulserCounts > vCH_to_maxDAC[index][CH])
          vCH_to_maxDAC[index][CH] = base->pulserCounts;
      }
    }
  }

  for (int i = 0; i < N; i++){

    base->GetEntry(i);

    for (int j = 0; j < base->chip->size(); j++){ 

      for (int k = 0; k < base->channel->at(j).size(); k++){

        if(base->pdo->at(j).at(k) <=  0)
          continue;

        if(base->pdo->at(j).at(k) >=  1023)
          continue;
        
        if(base->tdo->at(j).at(k) <=  0)
          continue;

        MMFE8 = base->boardId->at(j);
        VMM   = base->chip->at(j);
        //Delay = base->Delay;
        Delay = (int)base->tpSkew;
        CH    = base->channel->at(j).at(k);

        // MMFE8+VMM index
        int index = MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)];
        
        // CH index
        int cindex = vCH_to_index[index][CH];
        
        vhist_tdo[index][cindex]->Fill(base->tdo->at(j).at(k));

      }
    }
  }

  for (int i = 0; i < N; i++){

    base->GetEntry(i);

    for (int j = 0; j < base->chip->size(); j++){ 

      for (int k = 0; k < base->channel->at(j).size(); k++){

        if(base->pdo->at(j).at(k) <=  0)
          continue;

        if(base->pdo->at(j).at(k) >=  1023)
          continue;
        
        if(base->tdo->at(j).at(k) <=  0)
          continue;

        MMFE8 = base->boardId->at(j);
        VMM   = base->chip->at(j);
        //Delay = base->Delay;
        Delay = (int)base->tpSkew;
        CH    = base->channel->at(j).at(k);

        // MMFE8+VMM index
        int index = MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)];
        
        // CH index
        int cindex = vCH_to_index[index][CH];
        
        int m_tdo = base->tdo->at(j).at(k);

        min_TDO = tdo_min[index][cindex];
        max_TDO = tdo_max[index][cindex];
        if (base->pdo->at(j).at(k) > 300)
          continue;
        if (base->tdo->at(j).at(k) == 56 || base->tdo->at(j).at(k) == 57 || base->tdo->at(j).at(k) == 152)
          continue;

        if (vhist_tdo[index][cindex]->GetBinContent(vhist_tdo[index][cindex]->FindBin(m_tdo)) < 10)
          continue;
        if (base->tdo->at(j).at(k) < min_TDO)
          tdo_min[index][cindex] = base->tdo->at(j).at(k);
        if (base->tdo->at(j).at(k) > max_TDO)
          tdo_max[index][cindex] = base->tdo->at(j).at(k);
      }
    }
  }

  double middle_tdo = -1.;
  double range_tdo = -1.;
  for (int i = 0; i < N; i++){

    base->GetEntry(i);

    for (int j = 0; j < base->chip->size(); j++){ 

      for (int k = 0; k < base->channel->at(j).size(); k++){

        if(base->pdo->at(j).at(k) <=  0)
          continue;

        if(base->pdo->at(j).at(k) >=  1023)
          continue;
        
        if(base->tdo->at(j).at(k) <=  0)
          continue;

        MMFE8 = base->boardId->at(j);
        VMM   = base->chip->at(j);
        //Delay = base->Delay;
        Delay = (int)base->tpSkew;
        CH    = base->channel->at(j).at(k);

        // MMFE8+VMM index
        int index = MMFE8VMM_to_index[pair<int,int>(MMFE8,VMM)];
        
        // CH index
        int cindex = vCH_to_index[index][CH];
        
        middle_tdo = (tdo_min[index][cindex]+tdo_max[index][cindex])/2.;
        if (MMFE8==3)
          middle_tdo = (tdo_min[index][cindex]+tdo_max[index][cindex])/3.;
        range_tdo = (-tdo_min[index][cindex]+tdo_max[index][cindex]);

        vhist_all[index][cindex]->Fill(base->pdo->at(j).at(k), base->tdo->at(j).at(k));
        if (vhist_tdo[index][cindex]->GetRMS() > 9.){
          if (base->tdo->at(j).at(k) < middle_tdo)
            vhist_all_corr[index][cindex]->Fill(base->pdo->at(j).at(k), base->tdo->at(j).at(k));
          else
            vhist_all_corr[index][cindex]->Fill(base->pdo->at(j).at(k), base->tdo->at(j).at(k)-range_tdo+1);
        }
        else {
          vhist_all_corr[index][cindex]->Fill(base->pdo->at(j).at(k), base->tdo->at(j).at(k));
        }
      }
    }
  }

  for (int i = 0; i < vhist_all_corr.size(); i++) {
    for (int j = 0; j < vhist_all_corr[i].size(); j++) {
      vhist_all_corr[i][j]->RebinX(4);
      vhist_all_corr_prof[i].push_back(vhist_all_corr[i][j]->ProfileX());
    }
  }

  int Nindex = vMMFE8.size();

  TFile* fout = new TFile(output_name.c_str(), "RECREATE");
  fout->cd("");

  // write VMM_data tree to outputfile
  // TTree* newtree = tree->CloneTree();
  // fout->cd();
  // newtree->Write();
  // delete newtree;
  // delete base;
  // delete tree;

  // Output PDO fit tree
  double fit_MMFE8;
  double fit_VMM;
  double fit_CH;
  double fit_Delay;
  double fit_meanTimeWalk;
  double fit_sigmaTimeWalk;
  double fit_meanTimeWalkerr;
  double fit_sigmaTimeWalkerr;
  double fit_chi2;
  double fit_prob;
  double fit_minTimeWalk;
  double fit_maxTimeWalk;

  
  TTree* fit_tree = new TTree("TimeWalk_fit", "TimeWalk_fit");
  fit_tree->Branch("MMFE8", &fit_MMFE8);
  fit_tree->Branch("VMM", &fit_VMM);
  fit_tree->Branch("CH", &fit_CH);
  fit_tree->Branch("Delay", &fit_Delay);
  fit_tree->Branch("meanTimeWalk", &fit_meanTimeWalk);
  fit_tree->Branch("meanTimeWalkerr", &fit_meanTimeWalkerr);
  fit_tree->Branch("sigmaTimeWalk", &fit_sigmaTimeWalk);
  fit_tree->Branch("sigmaTimeWalkerr", &fit_sigmaTimeWalkerr);
  fit_tree->Branch("chi2", &fit_chi2);
  fit_tree->Branch("prob", &fit_prob);
  fit_tree->Branch("minTimeWalk", &fit_minTimeWalk);
  fit_tree->Branch("maxTimeWalk", &fit_maxTimeWalk);

  cout << "Extracting TimeWalk values" << endl;

  // min and max TimeWalk (TimeWalk window for each channel) calculated from simple fit

  
  vector<TF1*> vfunc;
  for(int i = 0; i < Nindex; i++){
    fit_MMFE8 = vMMFE8[i];
    fit_VMM = vVMM[i];
    int Ncindex = vCH[i].size();
    for(int c = 0; c < Ncindex; c++){
      fit_CH = vCH[i][c];

      fit_minTimeWalk = 0; 
      fit_maxTimeWalk = 4000;

//       int Ndelay = vDelay[i][c].size();
//       for(int d = 0; d < Ndelay; d++){
// 	fit_Delay = vDelay[i][c][d];
// 	double I = vhist[i][c][d]->Integral();
// 	fit_meanTimeWalk = vhist[i][c][d]->GetMean();
// 	fit_meanTimeWalkerr = max(vhist[i][c][d]->GetRMS()/sqrt(I),1.);
// 	fit_sigmaTimeWalk = max(vhist[i][c][d]->GetRMS(),1.);
// 	fit_sigmaTimeWalkerr = max(vhist[i][c][d]->GetRMSError(),1.);
// 	fit_chi2 = 0.;
// 	fit_prob = 1.;
	
// 	fit_tree->Fill();
//       }
    }
  }

  // add plots of TimeWalk values for each
  // MMFE8+VMM+CH combo to output file
  fout->mkdir("TimeWalk_plots");
  fout->cd("TimeWalk_plots");
  cout << "Drawing Histograms" << endl;
  for(int i = 0; i < Nindex; i++){ 
    char sfold[50];
    sprintf(sfold, "TimeWalk_plots/Board%d_VMM%d", vMMFE8[i], vVMM[i]);
    fout->mkdir(sfold);
    fout->cd(sfold);

    int Ncindex = vCH[i].size();
    for(int c = 0; c < Ncindex; c++){
      char stitle[50];
      sprintf(stitle, "Board #%d, VMM #%d , CH #%d", vMMFE8[i], vVMM[i], vCH[i][c]);
      if(vhist[i][c].size() > 0){
	char scan[50];
	sprintf(scan, "c_TimeWalk_Board%d_VMM%d_CH%d", vMMFE8[i], vVMM[i], vCH[i][c]);
	TCanvas* can = Plot_1D(scan, vhist[i][c], "TimeWalk", "Count", stitle, vlabel[i][c]);
	can->Write();
	delete can;
        
      }
	char scan2[50];
	sprintf(scan2, "c_PDO_TDO_Board%d_VMM%d_CH%d", vMMFE8[i], vVMM[i], vCH[i][c]);
        TCanvas* can2 = Plot_2D(scan2, vhist_all_corr[i][c], "PDO", "TDO", "", "");
        can2->Write();
        delete can2;

        char scan_all[100];
        sprintf(scan_all, "c_allTimeWalk_Board%d_VMM%d_CH%d", vMMFE8[i], vVMM[i], vCH[i][c]);
        TCanvas* can_all = Plot_2D(scan_all, vhist_all[i][c], "PDO", "TDO", "",stitle);
        can_all->Write();
        delete can_all;

        // fitting the stupid profile

        char fname[50];

        sprintf(fname, "funcpol8_MMFE8-%d_VMM-%d_CH-%d",
                vMMFE8[i], vVMM[i], vCH[i][c]);
        int ifunc = vfunc.size();
        if (ifunc > 2)
          continue;
        vfunc.push_back(new TF1(fname, "pol9(0)", 200., 980.));

        //vfunc[ifunc]->SetParameter(0, vhist_all_corr_prof[i][c]->GetMean(2));
        std::cout << "FITTING:"<< i << "," << c << std::endl;
        vhist_all_corr_prof[i][c]->Fit(fname,"EQR");

        char scan_prof[100];
        sprintf(scan_prof, "c_Profile_Board%d_VMM%d_CH%d", vMMFE8[i], vVMM[i], vCH[i][c]);
        TCanvas* can_prof = Plot_2D_Prof(scan_prof, vhist_all_corr_prof[i][c], "PDO", "TDO", "",stitle);
        vfunc[ifunc]->Draw("same");
        can_prof->Write();
        delete can_prof;
    }
  }

  // Write fit_tree to output file
  fout->cd("");
  fit_tree->Write();
  fout->Close();
}
