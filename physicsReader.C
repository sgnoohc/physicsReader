#include <vector>
#include <iostream>
#include <iterator>
#include <math.h>
#include <array>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH3.h"
#include "TH1.h"
#include "Math/LorentzVector.h"
#include "TCanvas.h"


using namespace std;

void physicsReader(){
//Declare leaf types;
	vector<Double_t> *pX, *pY, *pZ, *mass;
	vector<Double_t> *energy;
	vector<Int_t> *pid;

//Create histograms for invariant masses
	TH1F *wBoson = new TH1F("wBoson", "Mass of W Bosons", 20000, 0, 300);
	TH1F *higg = new TH1F("higg", "Mass of Higgs", 10000, 0, 300);


//Open the root file
	TFile *f = TFile::Open("/home/jotimelord/root/assignment/TTree.root");
	if (f == 0) {
		cout << "Either the file does not exist any more or the path is wrong." << endl;
	}

//Simple pointer t
	TTree *t = (TTree *)f->Get("Physics");

//Access pid branches
	t->SetBranchAddress("PID", &pid, &pidBranch);
	t->SetBranchAddress("P_X", &pX, &pXBranch);
	t->SetBranchAddress("P_Y", &pY, &pYBranch);
	t->SetBranchAddress("P_Z", &pZ, &pZBranch);
	t->SetBranchAddress("M", &mass, &massBranch);
	t->SetBranchAddress("E", &energy, &eBranch);

//Get entry number
	Long64_t entries = t->GetEntries();
	if (entries != 10000) {
		cout << "Event size is not equal to 10000." << endl;
	}

//loop over all of the events
	for (Long64_t i = 0; i < entries; i++) {
//find all the bottom quarks
		t->GetEntry(i);
		for (Int_t j = 0; j < 8; j++) {
			Int_t pid = pid[j];
			cout << pid << endl;
			Double_t pSqr = TMath::Power((pX)[j],2) + TMath::Power((pY)[j], 2) + TMath::Power((pZ)[j],2);
			Double_t energy = energy[j];
			Double_t pWi[4] = {0,0,0,0};
			Double_t eWi[4] = {0,0,0,0};
			switch (pid) {
				case 5:
					pHiggs[i] += pSqr;
					eHiggs[i] += energy;
					break;
				case -5:
					pHiggs[i] += pSqr;
					eHiggs[i] += energy;
					break;
				case 11:
					pWi[0] += pSqr;
					eWi[0] += energy;
					break;
				case 12:
					pWi[0] += pSqr;
					eWi[0] += energy;
					break;
				case 13:
					pWi[1] += pSqr;
					eWi[1] += energy;
					break;
				case 14:
					pWi[1] += pSqr;
					eWi[1] += energy;
					break;
				case 15:
					pWi[2] += pSqr;
					eWi[2] += energy;
					break;
				case 16:
					pWi[2] += pSqr;
					eWi[2] += energy;
					break;
				case 17:
					pWi[3] += pSqr;
					eWi[3] += energy;
					break;
				case 18:
					pWi[3] += pSqr;
					eWi[3] += energy;
					break;
			}
			pW[2*i] = 0;
			eW[2*i] = 0;
			for (Int_t x = 0; x < 4; x++) {
				if (pW[x] != 0) {
					pW[2*i] += pWi[x];
					eW[2*i] += eWi[x];
				}
			}
		}
	}

//Mass construction for both W and Higgs
	for (Long64_t m = 0; m < entries*2; m++) {
		mW[m] = TMath::Sqrt(TMath::Power(eW[m],2) - pW[2]);
		wBoson->Fill(mW[m]);	
	}
	for (Long64_t k = 0; k < entries; k++){
		mHiggs[k] = TMath::Sqrt(TMath::Power(eHiggs[k],2) - pHiggs[k]);
		higg->Fill(mHiggs[k]);
	}
	higg->Draw();
	wBoson->Draw();


}
