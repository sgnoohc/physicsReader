#include "physicsReader.h"

int main(int argc, char** argv)
{
    physicsReader();
    return 0;
}

void physicsReader()
{

    // Open the output file
    TFile* output_file = new TFile("higgsAnalysis.root", "RECREATE");

    // Create histograms for invariant masses & rapidity
    enum Objects {
        kW0 = 0,  // leading W
        kW1,      // subleading W
        kHiggs,   // higgs
        kB0,      // leading bottom quark
        kB1,      // subleading bottom quark
        kL0,      // leading charged lepton
        kL1,      // subleading charged lepton
        kVBF0,    // leading VBF quark
        kVBF1,    // subleading VBF quark
        kMET,     // neutrino+neutrino
        NObjects, // Number of objects
    };

    // ordering the array
    std::vector<TString> hist_types = {
        "leading W",
        "subleading W",
        "higgs",
        "leading bottom quark",
        "subleading bottom quark",
        "leading charged lepton",
        "subleading charged lepton",
        "leading VBF quark",
        "subleading VBF quark",
        "neutrino+neutrino"
    };

    // ordering the array
    std::vector<TString> hist_shortname_types = {
        "W0",
        "W1",
        "Higgs",
        "B0",
        "B1",
        "L0",
        "L1",
        "VBF0",
        "VBF1",
        "MET"
    };

    // histogram range
    std::vector<std::pair<float, float>> hist_pt_ranges = {
        {0., 1500.}, // W0
        {0., 1500.}, // W1
        {0., 1500.}, // Higgs
        {0.,  750.}, // B0
        {0.,  550.}, // B1
        {0.,  750.}, // L0
        {0.,  550.}, // L1
        {0.,  250.}, // VBF0
        {0.,  150.}, // VBF1
        {0.,  650.}, // MET
    };

    std::vector<std::pair<float, float>> hist_eta_ranges = {
        {-5, 5.}, // W0
        {-5, 5.}, // W1
        {-5, 5.}, // Higgs
        {-5, 5.}, // B0
        {-5, 5.}, // B1
        {-5, 5.}, // L0
        {-5, 5.}, // L1
        {-5, 5.}, // VBF0
        {-5, 5.}, // VBF1
        {-5, 5.}, // MET
    };

    std::vector<std::pair<float, float>> hist_phi_ranges = {
        {-TMath::Pi(), TMath::Pi()}, // W0
        {-TMath::Pi(), TMath::Pi()}, // W1
        {-TMath::Pi(), TMath::Pi()}, // Higgs
        {-TMath::Pi(), TMath::Pi()}, // B0
        {-TMath::Pi(), TMath::Pi()}, // B1
        {-TMath::Pi(), TMath::Pi()}, // L0
        {-TMath::Pi(), TMath::Pi()}, // L1
        {-TMath::Pi(), TMath::Pi()}, // VBF0
        {-TMath::Pi(), TMath::Pi()}, // VBF1
        {-TMath::Pi(), TMath::Pi()}, // MET
    };

    // Create an array of histograms
    std::vector<TH1F*> histograms_pt;
    std::vector<TH1F*> histograms_eta;
    std::vector<TH1F*> histograms_phi;

    for (unsigned int i = 0; i < NObjects; ++i)
    {
        // Binning into 1080 bins because 1080 contains a lot of prime factors, and therefore it's easy to rebin in the future. (cf. highly composite number)
        // It's not perfect but it gets the job done
        int nbins = 1080;
        histograms_pt .push_back(new TH1F(TString::Format("pt%s" , hist_shortname_types[i].Data()), TString::Format("pt of %s" , hist_types[i].Data()), nbins, hist_pt_ranges[i] .first, hist_pt_ranges[i] .second));
        histograms_eta.push_back(new TH1F(TString::Format("eta%s", hist_shortname_types[i].Data()), TString::Format("eta of %s", hist_types[i].Data()), nbins, hist_eta_ranges[i].first, hist_eta_ranges[i].second));
        histograms_phi.push_back(new TH1F(TString::Format("phi%s", hist_shortname_types[i].Data()), TString::Format("phi of %s", hist_types[i].Data()), nbins, hist_phi_ranges[i].first, hist_phi_ranges[i].second));
    }

    // =================================================================================================================
    // Do your stuff
    // -----------------------------------------------------------------------------------------------------------------

    // Open the root file
    // TFile *f = TFile::Open("/home/jotimelord/root/assignment/TTree.root");
    TFile *f = TFile::Open("vbshww_lvlvbbjj.root");

    // Simple pointer t
    TTree *t = (TTree *)f->Get("Physics");

    // Declare leaf types;
    vector<Double_t> *mass;
    vector<Double_t> *energy;
    vector<Int_t> *pid;
    vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> *p4;

    // Access pid branches
    t->SetBranchAddress("PID", &pid);
    t->SetBranchAddress("M", &mass);
    t->SetBranchAddress("E", &energy);
    t->SetBranchAddress("P4", &p4);

    // Get entry number
    Long64_t entries = t->GetEntries();
    if (entries != 10000)
    {
        cout << "Event size is not equal to 10000." << endl;
    }

    // =================================================================================================================

    // Go to the output file and write the file
    output_file->cd(); // (TFile has a weird "ownership" that can be confusing...)
    for (unsigned int i = 0; i < NObjects; ++i)
    {
        histograms_pt [i]->Write();
        histograms_eta[i]->Write();
        histograms_phi[i]->Write();
    }

    output_file->Close();
}

ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> leading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1)
{
    if (v1[0].Pt() > v1[1].Pt())
    {
        return v1[0];
    }
    else
    {
        return v1[1];
    }
}

ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>> subleading(vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>> v1)
{
    if (v1[0].Pt() > v1[1].Pt())
    {
        return v1[1];
    }
    else
    {
        return v1[0];
    }
}
