// C++
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

bool apply_baseline(std::string channel, int c_event, int pairType, int nleps, int nbjetscand, int isLeptrigger)
{
    // channel-dependant baseline selection
    if (channel == "tauTau")
    {
        if ((nleps == 0) && (nbjetscand > 1) && (pairType == 2) && (isLeptrigger == 1))
        {
            return 1;
        }
        else
            return 0;
    }
    else if (channel == "muTau")
    {
        throw std::invalid_argument("Channel muTau not implemented yet");
        //if (nleps != 0 || nbjetscand < 2 || pairType != 0 || isLeptrigger != 1)
        //{
            //return 0;
        //}
        //else
            //return 1;
    }
    else if (channel == "eTau")
    {
        throw std::invalid_argument("Channel eTau not implemented yet");
        //if (nleps != 0 || nbjetscand < 2 || pairType != 1 || isLeptrigger != 1)
        //{
            //return 0;
        //}
        //else
            //return 1;
    }
    else
    {
        std::cout << "Specified channel: " << channel << std::endl;
        throw std::invalid_argument("Channel should either be tauTau, muTau or eTau!");
    }
}

std::string get_region(std::string region, std::string channel, int isOS, float dau1_deepTauVsJet, float dau2_deepTauVsJet, float dau1_iso, float dau1_eleMVAiso)
{
    if (channel == "tauTau")
    {
        if (isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5)
            region = "SR"; // signal region: opposite sign, isolated taus
        else if (isOS == 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5)
            region = "SStight"; // B region
        else if (isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "OSinviso"; //  # C region
        else if (isOS == 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "SSinviso"; //  # D region
        else
            region = "";
    }
    else if (channel == "muTau")
    {
        if (isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5)
            region = "SR"; // signal region: opposite sign, isolated taus
        else if (isOS == 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5)
            region = "SStight"; // B region
        else if (isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "OSinviso"; //  # C region
        else if (isOS == 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "SSinviso"; //  # D region
        else
            region = "";
    }
    else if (channel == "eTau")
    {
        if (isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5)
            region = "SR"; // signal region: opposite sign, isolated taus
        else if (isOS == 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5)
            region = "SStight"; // B region
        else if (isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "OSinviso"; //  # C region
        else if (isOS == 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5)
            region = "SSinviso"; //  # D region
        else
            region = "";
    }
    return region;
}


void sample_lookup(std::string &sample, int &sample_id, Spin &spin, float &res_mass)
{
    spin = nonres;
    res_mass = 125;

    if (sample.find("_ggF_") != std::string::npos)
    {
        if (sample.find("Radion") != std::string::npos)
        {
            spin = radion;
            try
            {
                res_mass = std::stof(sample.substr(sample.find("_m") + 2));
            }
            catch (...)
            {
                std::cout << "Error in sample " << sample << " attempting to parse " << sample.substr(sample.find("_m") + 2) << "\n";
                assert(false);
            }
            if (res_mass <= 400)
            {
                sample_id = -15;
            }
            else if (res_mass <= 600)
            {
                sample_id = -16;
            }
            else
            {
                sample_id = -17;
            }
        }
        else if (sample.find("Graviton") != std::string::npos)
        {
            spin = graviton;
            try
            {
                res_mass = std::stof(sample.substr(sample.find("_m") + 2));
            }
            catch (...)
            {
                std::cout << "Error in sample " << sample << " attempting to parse " << sample.substr(sample.find("_m") + 2) << "\n";
                assert(false);
            }
            if (res_mass <= 400)
            {
                sample_id = -18;
            }
            else if (res_mass <= 600)
            {
                sample_id = -19;
            }
            else
            {
                sample_id = -20;
            }
        }
        else
        {
            sample_id = -999;
        }
    }
    else if (sample.find("_VBF_") != std::string::npos)
    {
        if (sample.find("Radion") != std::string::npos)
        {
            spin = radion;
            try
            {
                res_mass = std::stof(sample.substr(sample.find("_m") + 2));
            }
            catch (...)
            {
                std::cout << "Error in sample " << sample << " attempting to parse " << sample.substr(sample.find("_M") + 2) << "\n";
                assert(false);
            }
            if (res_mass <= 400)
            {
                sample_id = -21;
            }
            else if (res_mass <= 600)
            {
                sample_id = -22;
            }
            else
            {
                sample_id = -23;
            }
        }
        else if (sample.find("Graviton") != std::string::npos)
        {
            spin = graviton;
            try
            {
                res_mass = std::stof(sample.substr(sample.find("_m") + 2));
            }
            catch (...)
            {
                std::cout << "Error in sample " << sample << " attempting to parse " << sample.substr(sample.find("_M") + 2) << "\n";
                assert(false);
            }
            if (res_mass <= 400)
            {
                sample_id = -24;
            }
            else if (res_mass <= 600)
            {
                sample_id = -25;
            }
            else
            {
                sample_id = -26;
            }
        }
    }
    else if (sample.find("_Run") != std::string::npos)
    {
        sample_id = 0;
    }
    else if (sample.find("_TT_fully") != std::string::npos || sample.find("_TT_semi") != std::string::npos)
    {
        sample_id = 1;
    }
    else if (sample.find("_TTWJets") != std::string::npos || sample.find("_TTZTo") != std::string::npos)
    {
        sample_id = 2;
    }
    else if (sample.find("_TTWW") != std::string::npos || sample.find("_TTWZ") != std::string::npos || sample.find("_TTZZ") != std::string::npos)
    {
        sample_id = 3;
    }
    else if (sample.find("_ttH") != std::string::npos)
    {
        sample_id = 4;
    }
    else if (sample.find("_DY") != std::string::npos)
    {
        sample_id = 5;
    }
    else if (sample.find("_WJets_") != std::string::npos)
    {
        sample_id = 6;
    }
    else if (sample.find("_ggH") != std::string::npos || sample.find("VBFH") != std::string::npos)
    {
        sample_id = 7;
    }
    else if (sample.find("_ZH_") != std::string::npos)
    {
        sample_id = 8;
    }
    else if (sample.find("_WminusHTauTau") != std::string::npos)
    {
        sample_id = 9;
    }
    else if (sample.find("_WplusHTauTau") != std::string::npos)
    {
        sample_id = 10;
    }
    else if (sample.find("_EWK") != std::string::npos || sample.find("WplusH") != std::string::npos)
    {
        sample_id = 11;
    }
    else if (sample.find("_WW") != std::string::npos)
    {
        sample_id = 12;
    }
    else if (sample.find("_WZ") != std::string::npos)
    {
        sample_id = 13;
    }
    else if (sample.find("_ZH") != std::string::npos)
    {
        sample_id = 14;
    }
    else if (sample.find("_ZZ") != std::string::npos)
    {
        sample_id = 15;
    }
    else if (sample.find("_ST_") != std::string::npos)
    {
        sample_id = 16;
    }
    else
    {
        throw std::invalid_argument("Unrecognised sample: " + sample);
    }
}
bool skim_file(std::string filename, std::string ofilename,
               std::string channel, double sample_sum_w){
    std::cout << "Reading from file: " << filename << "\n";
    TFile *in_file = TFile::Open(filename.c_str());
    TTreeReader reader("HTauTauTree", in_file);
 
    TTreeReaderValue<unsigned long long> rv_evt(reader, "EventNumber");

    TTreeReaderValue<float> rv_MC_weight(reader, "MC_weight");
    TTreeReaderValue<float> rv_prescaleWeight(reader, "prescaleWeight");
    TTreeReaderValue<float> rv_L1pref_weight(reader, "L1pref_weight");
    TTreeReaderValue<float> rv_PUjetID_SF(reader, "PUjetID_SF");
    TTreeReaderValue<float> rv_PUReweight(reader, "PUReweight");
    TTreeReaderValue<float> rv_bTagweightReshape(reader, "bTagweightReshape");
    TTreeReaderValue<float> rv_trigSF(reader, "trigSF");
    TTreeReaderValue<float> rv_IdAndIsoAndFakeSF_deep_pt(reader, "IdAndIsoAndFakeSF_deep_pt");
    TTreeReaderValue<float> rv_DYscale_MTT(reader, "DYscale_MTT");

    double weight;
    float bTagweightReshape, PUReweight, PUjetID_SF, L1pref_weight, prescaleWeight, MC_weight;
    float trigSF, DYscale_MTT, IdAndIsoAndFakeSF_deep_pt;

    // Selection Stuff
    TTreeReaderValue<int> rv_pairType(reader, "pairType");
    TTreeReaderValue<int> rv_nleps(reader, "nleps");
    TTreeReaderValue<int> rv_nbjetscand(reader, "nbjetscand");
    TTreeReaderValue<int> rv_isLeptrigger(reader, "isLeptrigger");
    int pairType, nleps, nbjetscand, isLeptrigger;

    // region vars
    TTreeReaderValue<int> rv_isOS(reader, "isOS");
    TTreeReaderValue<int> rv_dau1_deepTauVsJet(reader, "dau1_deepTauVsJet");
    TTreeReaderValue<int> rv_dau2_deepTauVsJet(reader, "dau2_deepTauVsJet");
    int dau1_deepTauVsJet, dau2_deepTauVsJet;
    int region_id, sample, isOS;
    
    // prepare outfile
    TFile *out_file = new TFile(oname.c_str(), "recreate");
    TTree *tout = new TTree("TOut", "Rootfile stats");
    tout->Branch("weight", weight);
    tout->Branch("N_Events", n_events);
    tout->Branch("N_Events_tot", n_events_tot;)
    tout->Branch("Sum_w", sum_w);

    long int n_events_tot(reader.GetEntries(true));

    while (reader.Next())
    {
        // baseline selection
        pairType = *rv_pairType;
        nleps = *rv_nleps;
        nbjetscand = *rv_nbjetscand;
        isLeptrigger = *rv_isLeptrigger;
        bool pass_baseline;
        bool has_b_pair = true;
        pass_baseline = apply_baseline(channel, pairType, nleps, nbjetscand, isLeptrigger);
        if (pass_baseline == 0)
            continue;
        // determine region depending on channel
        std::string region;
        dau1_deepTauVsJet = *rv_dau1_deepTauVsJet;
        dau2_deepTauVsJet = *rv_dau2_deepTauVsJet;
        isOS = *rv_isOS;
        dau1_iso = *rv_l_1_iso;
        dau1_eleMVAiso = *rv_l_1_eleMVAiso;
        region = get_region(region, channel, isOS, dau1_deepTauVsJet, dau2_deepTauVsJet, dau1_iso, dau1_eleMVAiso);
        if (region.length() == 0)
        {
            continue;
        }

        bTagweightReshape = *rv_bTagweightReshape;
        PUReweight = *rv_PUReweight;
        PUjetID_SF = *rv_PUjetID_SF;
        L1pref_weight = *rv_L1pref_weight;
        prescaleWeight = *rv_prescaleWeight;
        MC_weight = *rv_MC_weight;
        trigSF = *rv_trigSF;
        DYscale_MTT = *rv_DYscale_MTT;
        IdAndIsoAndFakeSF_deep_pt = *rv_IdAndIsoAndFakeSF_deep_pt;
        // calc weight
        weight = 1.0;
        if (sample_id != 0)
        {
            weight *= bTagweightReshape * PUReweight * PUjetID_SF * L1pref_weight * prescaleWeight;
            weight *= MC_weight * trigSF * IdAndIsoAndFakeSF_deep_pt * DYscale_MTT;
            weight /= sample_sum_w;
        }
        tout->Fill()
    }
}
