#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <string>
#include <math.h>
#include <array>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include <Math/GenVector/LorentzVector.h>
#include "TCanvas.h"
#include <Math/GenVector/PxPyPzE4D.h>


using namespace std;


void physicsReader(){
//Declare helper functions
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> leading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1)
    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> subleading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1)
//Declare leaf types;
	vector<Double_t> *mass;
	vector<Double_t> *energy;
	vector<Int_t> *pid;
	vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> *p4;

//Create histograms for invariant masses & rapidity
	TH1F *wBoson = new TH1F("wBoson", "Mass of W Bosons", 20000, 0, 300);
	TH1F *higg = new TH1F("higg", "Mass of Higgs", 10000, 0, 300);
	TH1F *rapidWlead = new TH1F("rapidWlead", "Rapidity of leading W Bosons relative to the Z axis", 10000, -100, 100);
	TH1F *rapidWsub = new TH1F("rapidWsub", "Rapidity of subleading W Bosons relative to the Z axis", 10000, -100, 100);
	TH1F *rapidHigg = new TH1F("rapidHigg", "Rapidity of Higgs relative to the Z axis", 10000, -100, 100);

//Create an array of histograms
    TH1F *pT[10];
    TH1F *eTa[10];
    TH1F *phi[10];

    for (int i = 0; i < 10; i++) {
        pT[i] = new TH1F;
        eTa[i] = new TH1F;
        phi[i] = new TH1F;
    }
//Open the root file
	TFile *f = TFile::Open("/home/jotimelord/root/assignment/TTree.root");
	if (f == 0) {
		cout << "Either the file does not exist any more or the path is wrong." << endl;
	}

//Simple pointer t
	TTree *t = (TTree *)f->Get("Physics");

//Access pid branches
	t->SetBranchAddress("PID", &pid);
	t->SetBranchAddress("M", &mass);
	t->SetBranchAddress("E", &energy);
	t->SetBranchAddress("P4", &p4)

//Get entry number
	Long64_t entries = t->GetEntries();
	if (entries != 10000) {
		cout << "Event size is not equal to 10000." << endl;
	}
//ordering the array
    string order[10] = {"leading W", "subleading W", "higgs", "leading bottom quark", "subleading bottom quark", "leading charged lepton",
    "subleading charged lepton", "leading VBF quark", "subleading VBF quark", "neutrino+neutrino"};

//loop over all of the events
	for (Long64_t i = 0; i < entries; i++) {
        ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> ordered[10];
//find all the bottom quarks
		t->GetEntry(i);
		for (Int_t j = 0; j < 8; j++) {
			Int_t pidj = pid->at(j);
//Sort out different particles
            vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> bQ, Neu, Muon, qVBF;
            if (TMath::Abs(pidj) == 5) {
                bQ.push_back(p4->at(j));
            }
            if (TMath::Abs(pidj) == 11 || TMath::Abs(pidj) == 13 || TMath::Abs(pidj) == 15 ) {
                Muon.push_back(p4->at(j));
            }
            if (TMath::Abs(pidj) == 12 || TMath::Abs(pidj) == 14 || TMath::Abs(pidj) == 16 ) {
                Neu.push_back(p4->at(j));
            }
            else {
                qVBF.push_back(p4->at(j));
            }
        }
        ordered[3] = leading(bQ);
        ordered[4] = subleading(bQ);
        ordered[5] = leading(Muon);
        ordered[6] = subleading(Muon);
        ordered[7] = leading(qVBF);
        ordered[8] = subleading(qVBF);
        ordered[9] = Neu[0] + Neu[1];
//reconstruct higgs
        ordered[2] = bQ[0] + bQ[1];
//reconstruct w Bosons
        vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> wB;
        Double_t mW1 = (Neu[0] + Muon[0]).M();
        Double_t mW2 = (Neu[0] + Muon[1]).M();
        if (TMath::Abs(mW1 - 80) < TMath::Abs(mW2 - 80)) {
            wB.push_back((Neu[0] + Muon[0]));
            wB.push_back((Neu[1] + Muon[1]));
        }
        else {
            wB.push_back((Neu[0] + Muon[1]));
            wB.push_back((Neu[1] + Muon[0]));
        }
        ordered[0] = leading(wB);
        ordered[1] = subleading(wB);
//fill histograms
        for (int n = 0; n < 10; n++) {
            pT[i]->Fill(ordered[i].Pt());
            eTa[i]->Fill(ordered[i].Eta());
            phi[i]->Fill(ordered[i].Phi());
        }
        wBoson->Fill(ordered[0].M());
        wBoson->Fill(ordered[1].M());
        higg->Fill(ordered[2].M());
        rapidWlead->Fill(ordered[0].Rapidity());
        rapidWsub->Fill(ordered[1].Rapidity());
        rapidHigg->Fill(ordered[2].Rapidity());
	}
//name histograms
    for (int m = 0; m < 10; m++) {
        pT[i]->SetTitle("transverse momentum of " + order[m]);
        eTa[i]->SetTitle("Eta of " + order[m]);
        phi[i]->SetTitle("Azimuthal angle of " + order[m]);
    }
}

ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> leading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1) {
    if (v1[0].Pt() > v1[1].Pt()) {
        return v1[0];
    }
    else {
        return v1[1];
    }
}

ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> subleading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1) {
    if (v1[0].Pt() > v1[1].Pt()) {
        return v1[1];
    }
    else {
        return v1[0];
    }
}
