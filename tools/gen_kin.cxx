
#include<iostream>
#include<sstream>
#include<string>

#include<TCanvas.h>
#include<TFile.h>
#include<TTree.h>
#include<TH1D.h>
#include<TH2D.h>
#include<TLorentzVector.h>

void gen_kin(char* infile_name = NULL, char* outfile_name = NULL) {

	const double PION_MASS = 0.13957018;

	TFile* infile;
	if(infile_name == NULL) {
		infile = TFile::Open("/afs/cern.ch/user/k/kbicker/scratch0/filtered_run1/files_H_2008_10.root");
	} else {
		infile = TFile::Open(infile_name);
	}
	if(infile == NULL) {
		return;
	}

	TTree* intree = (TTree*)infile->Get("USR55");

	TFile* outfile;
	if(outfile_name == NULL) {
		outfile = TFile::Open("outfile.root", "RECREATE");
	} else {
		outfile = TFile::Open(outfile_name, "NEW");
	}
	if(outfile == NULL) {
		return;
	}

	double px1, py1, pz1;
	TLorentzVector p1;
	double px2, py2, pz2;
	TLorentzVector p2;
	double px3, py3, pz3;
	TLorentzVector p3;
	double px4, py4, pz4;
	TLorentzVector p4;
	double px5, py5, pz5;
	TLorentzVector p5;

	TLorentzVector pTot;

	int isKaon;

	intree->SetBranchAddress("Mom_x1", &px1);
	intree->SetBranchAddress("Mom_y1", &py1);
	intree->SetBranchAddress("Mom_z1", &pz1);
	intree->SetBranchAddress("Mom_x2", &px2);
	intree->SetBranchAddress("Mom_y2", &py2);
	intree->SetBranchAddress("Mom_z2", &pz2);
	intree->SetBranchAddress("Mom_x3", &px3);
	intree->SetBranchAddress("Mom_y3", &py3);
	intree->SetBranchAddress("Mom_z3", &pz3);
	intree->SetBranchAddress("Mom_x4", &px4);
	intree->SetBranchAddress("Mom_y4", &py4);
	intree->SetBranchAddress("Mom_z4", &pz4);
	intree->SetBranchAddress("Mom_x5", &px5);
	intree->SetBranchAddress("Mom_y5", &py5);
	intree->SetBranchAddress("Mom_z5", &pz5);
	intree->SetBranchAddress("isKaon", &isKaon);

	std::vector<TH1*> hists;
	hists.push_back(new TH1D("5_pi_mass", "5 Pion Mass", 500, 0, 7));
	hists.push_back(new TH1D("4_pi_mass", "4 Pion Subsystem", 250, 0.5, 3.5));
	hists.push_back(new TH1D("3_pi_mass", "3 Pion Subsystem", 750, 0.5, 2.5));
	hists.push_back(new TH2D("3_dalitz", "3 Pion Subsystem", 500, 0, 2.5, 500, 0, 2.5));
	hists.push_back(new TH2D("3_dalitz_a2", "3 Pion Subsystem in a2 region", 350, 0, 2.5, 350, 0, 2.5));
	hists.push_back(new TH1D("2_pi_mass", "2 Pion Subsystem", 350, 0.2, 1.5));
	hists.push_back(new TH2D("2_pi_4_pi_dalitz", "4 Pion Subsystem", 300, 0.5, 3, 300, 0.2, 1));

	std::vector<double> bounds;
	bounds.resize(10, 0.784);
	bounds.at(1) = 0.961;
	bounds.at(2) = 1.137;
	bounds.at(3) = 1.312;
	bounds.at(4) = 1.488;
	bounds.at(5) = 1.664;
	bounds.at(6) = 1.840;
	bounds.at(7) = 2.016;
	bounds.at(8) = 2.192;
	bounds.at(9) = 2.368;

	for(unsigned int i = 1; i < bounds.size(); ++i) {
		std::ostringstream strs;
		strs << bounds.at(i-1);
		std::string lb = strs.str();
		std::ostringstream strs2;
		strs2 << bounds.at(i);
		std::string ub = strs2.str();
		std::string name("2_pi_4_pi_dalitz_" + lb + "_" + ub);
		std::string title("2 Pion [" + lb + ", " + ub + "[");
		hists.push_back(new TH2D(name.c_str(), title.c_str(), 100, 0.3, 1.3, 100, 0.3, 1.4));
	}

	for(unsigned int i = 0; i < intree->GetEntries(); ++i) {

		intree->GetEntry(i);

		p1.SetXYZM(px1, py1, pz1, PION_MASS);
		p2.SetXYZM(px2, py2, pz2, PION_MASS);
		p3.SetXYZM(px3, py3, pz3, PION_MASS);
		p4.SetXYZM(px4, py4, pz4, PION_MASS);
		p5.SetXYZM(px5, py5, pz5, PION_MASS);

		pTot = p1+p2+p3+p4+p5;

		hists.at(0)->Fill(pTot.M());
		hists.at(1)->Fill((p1+p2+p4+p5).M());
		hists.at(1)->Fill((p1+p3+p4+p5).M());
		hists.at(1)->Fill((p2+p3+p4+p5).M());
		hists.at(2)->Fill((p1+p2+p5).M());
		hists.at(2)->Fill((p1+p3+p5).M());
		hists.at(2)->Fill((p2+p3+p5).M());
		hists.at(2)->Fill((p1+p2+p4).M());
		hists.at(2)->Fill((p1+p3+p4).M());
		hists.at(2)->Fill((p2+p3+p4).M());

		hists.at(3)->Fill((p1+p5).M(), (p2+p5).M());
		hists.at(3)->Fill((p1+p5).M(), (p3+p5).M());
		hists.at(3)->Fill((p2+p5).M(), (p3+p5).M());
		hists.at(3)->Fill((p1+p4).M(), (p2+p4).M());
		hists.at(3)->Fill((p1+p4).M(), (p3+p4).M());
		hists.at(3)->Fill((p2+p4).M(), (p3+p4).M());

		const double a2_mass = 1.318;
		if(std::fabs((p1+p2+p5).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p1+p5).M(), (p2+p5).M());
		}
		if(std::fabs((p1+p3+p5).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p1+p5).M(), (p3+p5).M());
		}
		if(std::fabs((p2+p3+p5).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p2+p5).M(), (p3+p5).M());
		}
		if(std::fabs((p1+p2+p4).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p1+p4).M(), (p2+p4).M());
		}
		if(std::fabs((p1+p3+p4).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p1+p4).M(), (p3+p4).M());
		}
		if(std::fabs((p2+p3+p4).M()) - a2_mass < 0.107) {
			hists.at(4)->Fill((p2+p4).M(), (p3+p4).M());
		}

		hists.at(5)->Fill((p1+p5).M());
		hists.at(5)->Fill((p2+p5).M());
		hists.at(5)->Fill((p3+p5).M());
		hists.at(5)->Fill((p1+p4).M());
		hists.at(5)->Fill((p2+p4).M());
		hists.at(5)->Fill((p3+p4).M());

		hists.at(6)->Fill((p1+p2+p4+p5).M(), (p1+p4).M());
		hists.at(6)->Fill((p1+p2+p4+p5).M(), (p1+p5).M());
		hists.at(6)->Fill((p1+p2+p4+p5).M(), (p2+p4).M());
		hists.at(6)->Fill((p1+p2+p4+p5).M(), (p2+p5).M());
		hists.at(6)->Fill((p1+p3+p4+p5).M(), (p1+p4).M());
		hists.at(6)->Fill((p1+p3+p4+p5).M(), (p1+p5).M());
		hists.at(6)->Fill((p1+p3+p4+p5).M(), (p3+p4).M());
		hists.at(6)->Fill((p1+p3+p4+p5).M(), (p3+p5).M());
		hists.at(6)->Fill((p2+p3+p4+p5).M(), (p2+p4).M());
		hists.at(6)->Fill((p2+p3+p4+p5).M(), (p2+p5).M());
		hists.at(6)->Fill((p2+p3+p4+p5).M(), (p3+p4).M());
		hists.at(6)->Fill((p2+p3+p4+p5).M(), (p3+p5).M());

		for(unsigned int i = 1; i < bounds.size(); ++i) {
			if(((p1+p2+p4+p5).M() >= bounds.at(i-1)) && ((p1+p2+p4+p5).M() < bounds.at(i))) {
				hists.at(6+i)->Fill((p1+p4).M(), (p2+p5).M());
				hists.at(6+i)->Fill((p2+p4).M(), (p1+p5).M());
			}
			if(((p1+p3+p4+p5).M() >= bounds.at(i-1)) && ((p1+p3+p4+p5).M() < bounds.at(i))) {
				hists.at(6+i)->Fill((p1+p4).M(), (p3+p5).M());
				hists.at(6+i)->Fill((p3+p4).M(), (p1+p5).M());
			}
			if(((p2+p3+p4+p5).M() >= bounds.at(i-1)) && ((p2+p3+p4+p5).M() < bounds.at(i))) {
				hists.at(6+i)->Fill((p2+p4).M(), (p3+p5).M());
				hists.at(6+i)->Fill((p3+p4).M(), (p2+p5).M());
			}
		}

	}

	outfile->cd();
	outfile->mkdir("Canvases");
	TCanvas* c2 = new TCanvas("2_pions_combined", "2_pions_combined");
	unsigned int needed = bounds.size() - 1;
	double root = std::sqrt(needed);
	int spl1 = (int)root;
	double spl2d = needed / spl1;
	int spl2;
	if((spl2d - std::floor(spl2d)) > 0.) {
		spl2 = std::floor(spl2d + 1.);
	} else {
		spl2 = (int)spl2d;
	}
	c2->Divide(spl2, spl1);

	for(unsigned int i = 0; i < hists.size(); ++i) {
		hists.at(i)->Write();
		std::string name(std::string(hists.at(i)->GetName()) + "_c");
		outfile->Cd("Canvases");
		TCanvas* c;
		c = new TCanvas(name.c_str(), hists.at(i)->GetTitle());
		hists.at(i)->Draw();
		if(dynamic_cast<TH2D*>(hists.at(i)) != NULL) {
			hists.at(i)->SetDrawOption("colz");
		}
		c->Write();
		if((i >= 7) && (i < (6+bounds.size()))) {
			c2->cd(i-6);
			hists.at(i)->Draw();
			hists.at(i)->SetDrawOption("colz");
		}
		outfile->cd();
	}
	outfile->Cd("Canvases");
	c2->Write();
	outfile->Close();

}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		gen_kin();
	} else {
		gen_kin(argv[1], argv[2]);
	}
}
