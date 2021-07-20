#define HistFit_cxx
#include "HistFit.h"

#include <TH1D.h>
#include <TH2D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TF1.h>

TH1D * HistLinear = nullptr;

TFile * File = nullptr;
TCanvas * c1 = nullptr;

void HistFit::Begin(TTree * /*tree*/)
{
   TString option = GetOption();

   HistLinear = new TH1D("#tau(sec)", "Muon Decay Time", 20, 0, 20);
   HistLinear->GetXaxis()->SetTitle("#musec");
   HistLinear->GetYaxis()->SetTitle("Events per 1 #musec");
   HistLinear->Sumw2();
   HistLinear->SetLineColor(kBlack);


   File = new TFile("FittedData.root", "RECREATE");
gFile = File;

c1 = new TCanvas("canvas", "Test Canvas");
}

void HistFit::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();

}

Bool_t HistFit::Process(Long64_t entry)
{
  GetEntry(entry);
   fReader.SetEntry(entry);

HistLinear->Fill(*RealData/1000);

   return kTRUE;
}

void HistFit::SlaveTerminate()
{

}

void HistFit::Terminate()
{
c1->cd();


TF1 *DecayFit = new TF1("f1","7538*[0]*exp(-[0]*x)+[1]", 0., 20.);
DecayFit->SetParameter(0, 0.4);
HistLinear->Fit("f1");

Double_t sum = 0;
for (Int_t i=1;i<20;i++) {
Double_t x = HistLinear->GetBinCenter(i);
Double_t val = DecayFit->Eval(x);
Double_t sigma = sqrt(val);
Double_t chi2 = (HistLinear->GetBinContent(i)-val)*(HistLinear->GetBinContent(i)-val)/(sigma*sigma);
sum += chi2;
  }
Double_t chi2pdof = sum/20;

double Lifetime = 1/DecayFit->GetParameter(0);
TLatex Tl;
Tl.SetTextAlign(12);
Tl.SetTextSize(0.04);
Tl.DrawLatexNDC(0.45, 0.65, "#scale[0.75]{D(t) = N_{0} * #lambda * e^{-#lambdat} + Background}");
Tl.DrawLatexNDC(0.45, 0.60, Form("#scale[0.75]{D(t) = 7538 * %f * e^{-%ft} + 8}", DecayFit->GetParameter(0), DecayFit->GetParameter(0)));
Tl.DrawLatexNDC(0.45, 0.55, Form("#scale[0.75]{#tau = 1/#lambda = %f #pm %f #musec}", Lifetime, DecayFit->GetParError(0)));
Tl.DrawLatexNDC(0.45, 0.50, Form("#scale[0.75]{#chi^{2}/D.O.F = %f}", chi2pdof));

c1->Write("Real Data - Linear Scale");

}
