#include <math.h>
#include "DefineColours.h"
#include "NPReaction.h"
#include <string>
#include <sstream>
using namespace std;
TChain* chain=NULL ;

//EColor myColours[5] = {kBlack, (EColor)(kRed+1), (EColor)(kGreen+1), (EColor)(kBlue), (EColor)(kMagenta+1)};
EColor myColours[5] = {(EColor) kGreen, (EColor)(kGreen+1), (EColor)(kGreen+2), (EColor)(kGreen+3), (EColor)(kGreen+4)};
EColor myColours3[3] = {(EColor) kCyan, (EColor)(kCyan+2), (EColor)(kCyan+4)};

//======================================================================================

string harddrive = "/media/charlottepaxman/Elements/ActiveWork/";

string ConvertPDTA(string letter){
  string output;

  if(letter == "p"){
    output = "1H";
  } else if (letter == "d") {
    output = "2H";
  } else if (letter == "t") {
    output = "3H";
  } else if (letter == "a") {
    output = "4He";
  }

  return output;
}

string FindMyFolder(string reaction){

  int isoStop = reaction.find("(") ;
  int isoStart = reaction.find(")") ;

  string beam = reaction.substr(0,isoStop);
  string target = reaction.substr(isoStop+1,1);
  string light = reaction.substr(isoStop+3,1);
  string heavy = reaction.substr(isoStart+1,5);

  string output = beam;
  output.append(ConvertPDTA(target));
  output.append("-");
  output.append(ConvertPDTA(light));
  output.append(heavy);

//  {
//
////  int reacStop = reaction.find(")");
////  string reac = reaction.substr(isoStop,reacStop);
//
////  //please don't judge my horrible nested loops :(
////  if(reac=="(p,p)"){
////        output.append(iso);
////	output.insert(0,"Forward_OnProton/");
////  } else if (reac=="(p,d)"){
////    if(iso=="47K"){
////        output.append("46K");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="94Kr"){
////        output.append("93Kr");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="132Sn"){
////        output.append("131Sn");
////	output.insert(0,"Forward_OnProton/");
////    }
////  } else if (reac=="(p,t)") {
////     if(iso=="47K"){
////        output.append("45K");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="94Kr"){
////        output.append("92Kr");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="132Sn"){
////        output.append("130Sn");
////	output.insert(0,"Forward_OnProton/");
////    }
////  } else if (reac=="(p,a)"){
////     if(iso=="47K"){
////        output.append("44Ar");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="94Kr"){
////        output.append("91Br");
////	output.insert(0,"Forward_OnProton/");
////    } else if (iso=="132Sn"){
////        output.append("129In");
////	output.insert(0,"Forward_OnProton/");
////    }
////  } else if (reac=="(d,p)"){
////     if(iso=="47K"){
////        output.append("48K");
////	output.insert(0,"Backward_OutProton/");
////    } else if (iso=="94Kr"){
////        output.append("95Kr");
////	output.insert(0,"Backward_OutProton/");
////    } else if (iso=="132Sn"){
////        output.append("133Sn");
////	output.insert(0,"Backward_OutProton/");
////    }
////  } else if (reac=="(t,p)"){
////     if(iso=="47K"){
////        output.append("49K");
////	output.insert(0,"Backward_OutProton/");
////    } else if (iso=="94Kr"){
////        output.append("96Kr");
////	output.insert(0,"Backward_OutProton/");
////    } else if (iso=="132Sn"){
////        output.append("134Sn");
////	output.insert(0,"Backward_OutProton/");
////    }
////  } else if (reac=="(d,t)"){
////     if(iso=="47K"){
////        output.append("46K");
////    } else if (iso=="94Kr"){
////        output.append("93Kr");
////    } else if (iso=="132Sn"){
////        output.append("131Sn");
////    }
////
////  }
//  }

  return output;
}

void plot_kine(NPL::Reaction r, double Ex,Color_t c,int w, int s){
  r.SetExcitation4(Ex);
  TGraph* g= r.GetKinematicLine3();
  g->SetLineColor(c) ;
  g->SetLineStyle(s) ;
  g->SetLineWidth(w) ;
  g->Draw("c same");
}

TGraph* plot_kine_heavy(string reacname, double Ex,Color_t c,int w, int s){
  NPL::Reaction r(reacname.c_str());
  r.SetExcitation4(Ex);
  TGraph* g= r.GetKinematicLine4();
  g->SetLineColor(c) ;
  g->SetLineStyle(s) ;
  g->SetLineWidth(w) ;
  return g;
}

TGraph* plot_kine_light(string reacname, double Ex,Color_t c,int w, int s){
  NPL::Reaction r(reacname.c_str());
  r.SetExcitation4(Ex);
  TGraph* g= r.GetKinematicLine3();
  g->SetLineColor(c) ;
  g->SetLineStyle(s) ;
  g->SetLineWidth(w) ;
  return g;
}

TTree* LoadPhysicsTree(string fileName){
  string doMe = harddrive + fileName;

cout << " LOAD:: " << doMe << endl;

  //TFile* f = new TFile(fileName.c_str(),"READ");
  TFile* f = new TFile(doMe.c_str(),"READ");
  return (TTree*) f->Get("PhysicsTree");  
}

TTree* LoadSimulatedTree(string fileName){
  TFile* f = new TFile(fileName.c_str(),"READ");
  return (TTree*) f->Get("SimulatedTree");  
}

TCanvas* MakeCanv(string canvName){
  return new TCanvas(canvName.c_str(),canvName.c_str(),1000,1000);
}

TH1F* SetTitles(TH1F* hist, string title, string xtit, string ytit){
  hist->SetTitle(title.c_str());
  hist->GetXaxis()->SetTitle(xtit.c_str());
  hist->GetYaxis()->SetTitle(ytit.c_str());
  return hist; 
}

TH2F* SetTitles(TH2F* hist, string title, string xtit, string ytit){
  hist->SetTitle(title.c_str());
  hist->GetXaxis()->SetTitle(xtit.c_str());
  hist->GetYaxis()->SetTitle(ytit.c_str());
  return hist; 
}

void DrawKinem(NPL::Reaction NPLReac, string folder, string reacfile){
  gStyle->SetPalette(kViridis);

  //string sbase = "./"+folder+"/"+reacfile;
  string sbase = folder+"/"+reacfile;
  string draw1 = "ELab:ThetaLab>>h(50,0,30,2000,0,100)";
  string draw2 = "!PunchThrough";//"Mugast.";
  string loadtemp = sbase +".root"; string title = folder+"_"+reacfile;
  string canv = "c_"+folder+"_"+reacfile;
  string save = "./pdfs/kines/"+folder+"_"+reacfile+".pdf";

  TTree* stemp = LoadPhysicsTree(loadtemp.c_str());
    TCanvas* ctemp = MakeCanv(canv.c_str());
    stemp->Draw(draw1.c_str(), draw2.c_str(), "colz");
    TH1F* htemp = (TH1F*) gDirectory->Get("h");
    htemp  = SetTitles(htemp, title.c_str(),"ThetaLab [deg]","ELab [MeV]");
    plot_kine(NPLReac, 0.000, kRed, 2, 1);
    //htemp->GetXaxis()->SetRangeUser(min,max); htemp->GetYaxis()->SetRangeUser(min,max);
  ctemp->SaveAs(save.c_str());
//  delete ctemp;
}

//======================================================================================

TCanvas* DrawKinem_OneEnergy_AllThick(TCanvas* c, int cd, string boxtitle,
		//NPL::Reaction reac, string fldr, string rfile, double ymax){
		NPL::Reaction reac, string fldr, string rfile, double ymax, double xmax){
  gStyle->SetPalette(kViridis);
  c->cd(cd);
  c->GetPad(cd)->SetLogz();

  //string sbase = "./"+fldr+"/"+rfile;
  string sbase = fldr+"/"+rfile;
  string draw1 = "ELab:ThetaLab>>h(60,0,60,4000,0,200)";
  string draw2 = "Mugast.TelescopeNumber==10";
  string loadtemp = sbase +".root";
  string tit = "T = "+boxtitle+" um";

  cout << "|| LOADING " << loadtemp << " ||" << endl;
  TTree* stemp = LoadPhysicsTree(loadtemp.c_str());
  stemp->Draw(draw1.c_str(), draw2.c_str(), "colz");
  TH1F* htemp = (TH1F*) gDirectory->Get("h");
  htemp  = SetTitles(htemp, tit.c_str(),"ThetaLab [deg]","ELab [MeV]");
  plot_kine(reac, 0.000, kRed, 1, 1);
  htemp->GetYaxis()->SetRangeUser(1,ymax);
  htemp->GetXaxis()->SetRangeUser(0,xmax);
  
  return c;
}

void DrawKinem_OneReac_AEnergyAThick(int beamA, string base, string fold, 
//		                     double ybase, double yjump){
		                     double ymax, double xmax){

  //for(int e = 0; e<6; e++){
  for(int e = 5; e>-1; e--){
  //for(int e = 5; e>4; e--){
//    double ymax = ybase + (e*yjump);

    int MeVu = 5+e;
    int energy = ceil(MeVu*(double)beamA);

    int nameStop = base.find("(") - 1;
    int reacLetter = base.find(",") + 1;

    string ctitle = "c"
	          + base.substr(0,nameStop) 
		  +"p" + base.substr(reacLetter,1) + "_"
		  + to_string(MeVu)
		  +"MeVu";
    string savert = "Outputs/"+ctitle+".root";
    string saveep = "Outputs/"+ctitle+".eps";

    TCanvas* cEnergy = new TCanvas(ctitle.c_str(),
		                   ctitle.c_str(),
   				   2000,1000);
    cEnergy->Divide(7,2);
    //cEnergy->Divide(8,2);
    cEnergy->ToggleEditor();

    for(int t = 0; t<14; t++){
    //for(int t = 0; t<8; t++){
    //for(int t = 0; t<16; t++){
      int thick = 500 + (500*t);
      //string sname = "Sim_R2699_Q04R32S32_T"
      string sname = "Sim_R2699_Q01R32S32_T"
	           + to_string(thick)
		   + "_"
		   + to_string(energy)
		   //+ "MeV";
		   //+ "MeV_TargT5";
		   + "MeV_TargT2_flat";

      string spx = base + to_string(energy);
      NPL::Reaction px(spx.c_str());
      cout << energy << endl;

      DrawKinem_OneEnergy_AllThick(cEnergy,t+1,to_string(thick),
		      //px, fold.c_str(), sname.c_str(), ymax);
		      px, fold.c_str(), sname.c_str(), ymax, xmax);
    }

  cEnergy->SaveAs(savert.c_str());
  cEnergy->SaveAs(saveep.c_str());
  }
}

void Draw20MeV(string reaction, int maxT, double xrange, double yrange){
  string title = "Reconstructed energy from #Delta#mbox{E} -- "+reaction+" @ 20 MeV/u";

  //string base = "./" + FindMyFolder(reaction)
  string base = FindMyFolder(reaction)
	      + "/Sim_R2699_Q04R32S32_T";
  int isoStop = reaction.find("(") - 1;
  int E = stoi(reaction.substr(0,isoStop))*20;

  TH1F* harray[7];

  TCanvas* ctemp =  new TCanvas("ctemp","ctemp",200,200);
  for(int t = 0; t<maxT; t++){
    string hname = "h"+to_string(t+1)+"000";
    string draw = "Ex>>" + hname + "(500,-50,50)";
    string loadme = base + to_string(t+1) 
	          + "000_" + to_string(E)
	          +"MeV.root";
    TTree* tree = LoadPhysicsTree(loadme);

    tree->Draw(draw.c_str(),"PunchThrough==1","");
    harray[t] = (TH1F*) gDirectory->Get(hname.c_str());
    harray[t]->SetTitle(hname.c_str());
    harray[t]->SetName(hname.c_str());
    harray[t]->SetLineColor(t+1);
    harray[t]->SetFillColor(t+1);
    harray[t]->SetFillStyle(3003);

    harray[t]->GetXaxis()->SetRangeUser(-xrange,xrange);
    harray[t]->GetYaxis()->SetRangeUser(0,yrange);
    harray[t]->GetXaxis()->SetTitle("Ex [MeV]");
    harray[t]->GetYaxis()->SetTitle("Counts / 0.2 MeV");
    harray[t]->SetTitle(title.c_str());

  } /// NOT WORKING!!!!!!!!!!!!!!!!!!!!!!!!!
  delete ctemp;

  TCanvas* canv =  new TCanvas("canv","canv",1000,1000);
  harray[0]->Draw();
  for(int t = 1; t<maxT; t++){
    harray[t]->Draw("same");
  }

  auto legend = new TLegend(0.1,0.5,0.3,0.9);
  legend->SetHeader("Annular Thickness","C");
  for(int t = 0; t<maxT; t++){
    string name = to_string(t+1) + "000 um";
    legend->AddEntry(harray[t],name.c_str(),"f");
  }
  legend->Draw();


}

void GraphFits(string rstring, int loopTarg, int loopRing, string orb, string LayerGates, double ymax_Ex, double ymax_ELab, double ymax_StripCheck, double ymax_Sigma, bool writefiles, bool gotCut){
  double sig[loopTarg][loopRing];
  double dSig[loopTarg][loopRing];
  //double vTarg[4] = {1.,2.,4.,6.}; //initilizing
  double vTarg[5] = {1.,2.,4.,6.,8.}; //initilizing
  double vRing[5] = {16., 32.,64.,96.,128.};
  string ringName[5] = {"16", "32","64","96","128"};
  double zero[5] = {0.,0.,0.,0.,0.};

  //TF1 *f1 = new TF1("gausRange","gaus",-30.0, 30.0);
  TF1 *f1 = new TF1("gausRange","gaus",-1., 1.);

//  gStyle->SetOptStat(1);
//  gStyle->SetStatW(0.4);

  //// Run selectors ///////////
  int sithick = 1500;
//  int MeVu = 10; //// SEE BELOW

  //// Graph scaling values ////
//  double ymax_Sigma = 0.3;  
//  double ymax_ELab = 20.;//4.;//30.;
//  double ymax_StripCheck =1000.;
  //////////////////////////////


  string reactionInput = rstring.substr(0,rstring.find("@"));
  cout << reactionInput << endl;

  //Find proper file indentifiers
  int isoStop = reactionInput.find("(") - 1;
  //string strE = to_string(stoi(reactionInput.substr(0,isoStop))*8);
  //string strE = to_string(stoi(reactionInput.substr(0,isoStop))*MeVu);
  string strE = rstring.substr(rstring.find("@")+1,4);
  int reacStop = reactionInput.find(")");
  string reac = reactionInput.substr(isoStop+1,5);

  cout << strE << endl;


  int MeVu;
  if(reac=="(d,p)"){
    MeVu = 8;
  } else if (reac=="(d,t)"){
    MeVu = 15;
  } else if (reac=="(p,d)"){
    MeVu = 10;
  }


  //secondary canvas
  TCanvas* cELab = new TCanvas("cELab","cELab",1000,1000);
  cELab->Divide(loopRing, loopTarg);

  //Build fitting canvas
  TCanvas* cFits = new TCanvas("cFits","cFits",1500,1000);
  cFits->Divide(loopRing, loopTarg);

  //Build fancy output canvas
  //TCanvas* cFigure = new TCanvas("cFigure","cFigure",1000,1000);
  TCanvas* cFigure = new TCanvas("cFigure","cFigure",1000,800);
  cFigure->cd()->SetLeftMargin(0.12);
  auto legend1 = new TLegend(0.17,0.82,0.95,0.9);
//  legend1->SetNColumns(5);
  legend1->SetNColumns(3);
  legend1->SetTextSize(0.04);
  legend1->SetTextFont(132);
//  legend1->SetEntrySeparation(0.75);

  NPL::Reaction r(rstring.c_str());
  r.SetExcitation4(0);
  TGraph* gKine = r.GetKinematicLine3();
  gKine->SetLineColor(kRed);

  TCutG* cut;
  if(gotCut){
    string cutname = harddrive + FindMyFolder(reactionInput) + "/Cut.root";
    TFile gcIn(cutname.c_str());
    cut = (TCutG*) gcIn.FindObjectAny("cut");
    cut->SetName("cut");

    LayerGates.append(" && cut");
  }


  for(int nT = 0; nT < loopTarg; nT++){
    cout << "============================" << endl;
    cout << "----------------------------" << endl;
    //vTarg[nT] = 2+nT*2;
    cout << "TThick = " << vTarg[nT] << " um"<< endl;
  
    string legendtxt[loopRing];

cout << RED << " HERE  PRE-RING LOOP" << RESET << endl;

    double fullMax = 0.;
    for(int nR = 0; nR < loopRing; nR++){
      cout << "----------------------------" << endl;
      cout << "#Rings = " << ringName[nR] << endl;

      //Load tree
      //string name = "./" + FindMyFolder(reactionInput)
      string name = FindMyFolder(reactionInput)
	          + "/Sim_R2699_Q01R" + ringName[nR]
                  + "S32_T" + to_string(sithick)
                  + "_" + strE
		  + "MeV_TargT" + to_string((int)vTarg[nT])
		  + "_" + orb
		  + "_10MeVu_Full"
		  + ".root";
      cout << name << endl;
      auto tree = LoadPhysicsTree(name.c_str());

      //Draw
      cFits->cd((5*nT)+nR+1);
      tree->Draw("Ex>>h(50,-1.0,1.0)",LayerGates.c_str(),"");
      //tree->Draw("Ex>>h(10000,-1.0,1.0)",LayerGates.c_str(),"");
      //tree->Draw("Ex>>h(2000,-1.0,1.0)",LayerGates.c_str(),"");
      //tree->Draw("Ex>>h(200,-1.0,1.0)",LayerGates.c_str(),"");
      TH1F* h = (TH1F*) gDirectory->Get("h");
  

      //Cosmetic adjustments
      string histname = "TThick = " + to_string((int)vTarg[nT]) 
	              + ", #Rings = " + ringName[nR];
      h->SetTitle(histname.c_str());
      h->GetYaxis()->SetRangeUser(0,ymax_Ex);

      //Apply fit
      auto fit = h->Fit(f1,"s q r");
      cout << fit->Parameter(2) 
           << " +- " 
           << fit->ParError(2) 
           << endl; 
//////////////////////////////////////////////////////////

      //Fill arrays
      sig[nT][nR]  = fit->Parameter(2);
      dSig[nT][nR] = fit->ParError(2);

      cout << GREEN << " FIT CHI2/NDF = " << fit->Chi2() << " / " << fit->Ndf() << " = " << fit->Chi2()/fit->Ndf() << RESET << endl;


      //if(fit->Chi2()/fit->Ndf() > 150.){
      if(nT>-1){
        cout << RED << " USING ALTERNATIVE SIGMA METHOD!!! " << RESET << endl;
        //ALTERNATIVE: FWHM!
        //double fullMax = h->GetBinContent(h->FindBin(0.0)); // assuming correct shape, gaussian around 0?
        h->GetXaxis()->SetRangeUser(-0.2,0.2);
        cout << "maxbincontent = " << h->GetBinContent(h->GetMaximumBin()) << endl;
  
        if(nR==0){
          fullMax = h->GetBinContent(h->GetMaximumBin());
        }
        h->GetXaxis()->SetRangeUser(-1.5,1.5);
        double posL = h->GetXaxis()->GetBinCenter(h->FindFirstBinAbove(fullMax/2.));
        double posH = h->GetXaxis()->GetBinCenter(h->FindLastBinAbove(fullMax/2.));
        double fwhm = posH - posL;
  
        sig[nT][nR]  = fwhm/(2.*sqrt(2.)*log(2.));
        dSig[nT][nR] = sig[nT][nR] * sqrt( pow((sqrt(fullMax)/fullMax),2) + 2*pow((sqrt((fullMax*2.))/(fullMax*2.)),2) );
  
        cout << "max = " << fullMax << " posL = " << posL << " posH = " << posH << " fwhm = " << fwhm << " +- " << dSig[nT][nR] << endl;
  
  //////////////////////////////////////////////////////////
      }





//cout << "!!!!!!!!!!!!!!!!!!!!!!  " 
//     << h->FindBin(-1.) 
//     << " TO "  
//     << h->FindBin(+1.) 
//     << endl;

      string text = to_string((int)h->Integral(h->FindBin(-1.),h->FindBin(1.))) + " counts";
      TLatex l;
      l.SetTextSize(0.06);
      l.DrawLatex(0.3,h->GetBinContent(h->GetMaximumBin())*0.8,text.c_str());

      cELab->cd((5*nT)+nR+1);

      string drawme = "ELab:ThetaLab>>hELab(360,0,180," 
      //string drawme = "ELab:ThetaLab>>hELab(180,0,180," 
	            + to_string(ymax_ELab*5.) + ",0," 
	            //+ to_string(ymax_ELab*20.) + ",0," 
		    + to_string(ymax_ELab) + ")";

      tree->Draw(drawme.c_str(),LayerGates.c_str(),"colz");
      TH2F* hELab = (TH2F*) gDirectory->Get("hELab");
      if(reac=="(d,p)"){
        hELab->GetXaxis()->SetRangeUser(140,180);
      } else {
        hELab->GetXaxis()->SetRangeUser(  0, 50);
      }
      gKine->Draw("same");

      gROOT->SetBatch(1);
      TCanvas* cScrap = new TCanvas("cScrap","cScrap",1500,1000);
      TH1F* hcpy[loopRing];
      if(nT==0){
	//T=2um
        cScrap->cd();
        //tree->Draw("Ex>>hcpy(20,-1.0,1.0)",LayerGates.c_str(),"");
        tree->Draw("Ex>>hcpy(200,-1.0,1.0)",LayerGates.c_str(),"");
        hcpy[nR] = (TH1F*) gDirectory->Get("hcpy");
        hcpy[nR]->SetLineColor(  kCyan+nR);
	hcpy[nR]->SetLineWidth(2);
	hcpy[nR]->GetXaxis()->SetRangeUser(-0.5,+0.5);
	hcpy[nR]->GetYaxis()->SetRangeUser(0,ymax_Ex);
	hcpy[nR]->SetTitle("Target Thickness = 1#mum");
	hcpy[nR]->GetXaxis()->SetTitle("Ex [MeV]");
	//hcpy[nR]->GetYaxis()->SetTitle("Counts / 0.02 MeV");
	hcpy[nR]->GetYaxis()->SetTitle("Counts / 0.01 MeV");
	//hcpy[nR]->GetYaxis()->SetTitle("Counts / 0.005 MeV");
	hcpy[nR]->GetXaxis()->CenterTitle();
	hcpy[nR]->GetYaxis()->CenterTitle();
        hcpy[nR]->GetXaxis()->SetTitleSize(0.05);
        hcpy[nR]->GetYaxis()->SetTitleSize(0.05);
        hcpy[nR]->GetXaxis()->SetLabelSize(0.04);
        hcpy[nR]->GetYaxis()->SetLabelSize(0.04);
        cFigure->cd();
        cFigure->cd()->SetLeftMargin(0.17);
        cFigure->cd()->SetTopMargin(0.2);
        cFigure->cd()->SetRightMargin(0.05);
	legendtxt[nR] = ringName[nR] + " rings";
        if(nR==0){
          hcpy[nR]->Draw();
          legend1->AddEntry(hcpy[nR],legendtxt[nR].c_str(),"l");
	//} else{
	} else if((nR%2)==0){
          hcpy[nR]->Draw("same");
          legend1->AddEntry(hcpy[nR],legendtxt[nR].c_str(),"l");
	}
        //legend1->AddEntry(hcpy[nR],legendtxt[nR].c_str(),"l");
//	if(nR==4){
//cout << RED << " HERE  IN LEGEND LOOP" << RESET << endl;
////          auto legend = new TLegend(0.12,0.82,0.9,0.9);
//          legend->SetNColumns(5);
//          //auto legend = new TLegend(0.8,0.8,0.9,0.9);
//          //legend->AddEntry(hcpy[0],legendtxt[0].c_str(),"l");
//          //legend->AddEntry(hcpy[1],legendtxt[1].c_str(),"l");
//          //legend->AddEntry(hcpy[2],legendtxt[2].c_str(),"l");
//          //legend->AddEntry(hcpy[3],legendtxt[3].c_str(),"l");
//          //legend->AddEntry(hcpy[4],legendtxt[4].c_str(),"l");
//          //legend->Draw();
//	}
        legend1->Draw();
      }
      delete cScrap;
      gROOT->SetBatch(0);
    }
  }

  //=====================================================================
  
  //TCanvas* cRings = new TCanvas("cRings","cRings",1000,1000);
  TCanvas* cRings = new TCanvas("cRings","cRings",1000,800);
  cRings->cd();
  cRings->cd()->SetLeftMargin(0.17);
  cRings->cd()->SetRightMargin(0.05);
  cRings->cd()->SetTopMargin(0.2);
  TGraphErrors *g[loopTarg];
  TMultiGraph *mg = new TMultiGraph();
  string namesR[loopTarg];
  for (int i=0; i<loopTarg; i++) {
    g[i] = new TGraphErrors(loopRing, vRing, sig[i],zero,dSig[i]);
    namesR[i] = "T = "+to_string((int)vTarg[i])+" #mum";
    g[i]->SetNameTitle(namesR[i].c_str());
    g[i]->SetMarkerSize(1.3);
    g[i]->SetMarkerStyle(20+i);
    g[i]->SetMarkerColor(kCyan+i);
    g[i]->SetLineColor(  kCyan+i);
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << i << endl;
    mg->Add(g[i]);
  }
  mg->SetTitle((reactionInput+"; Number of rings; Sigma [MeV]").c_str());
  //mg->SetTitle((reactionInput+"; Number of rings; FWHM [MeV]").c_str());
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitleSize(0.05);
  mg->GetYaxis()->SetTitleSize(0.05);
  mg->GetXaxis()->SetLabelSize(0.04);
  mg->GetYaxis()->SetLabelSize(0.04);
  //mg->GetYaxis()->SetRangeUser(0,ymax_Sigma);
  mg->Draw("AL P");

  auto legend = new TLegend(0.17,0.82,0.95,0.9);
  legend->SetTextFont(132);
  legend->SetNColumns(5);
  legend->AddEntry(g[0],namesR[0].c_str(),"lep");
  legend->AddEntry(g[1],namesR[1].c_str(),"lep");
  legend->AddEntry(g[2],namesR[2].c_str(),"lep");
  legend->AddEntry(g[3],namesR[3].c_str(),"lep");
  legend->AddEntry(g[4],namesR[4].c_str(),"lep");
  legend->Draw();
  

//  //=====================================================================
//  TCanvas* cTargs = new TCanvas("cTargs","cTargs",1000,1000);
//  cTargs->cd();
//  TGraphErrors *g2[loopRing];
//  TMultiGraph *mg2 = new TMultiGraph();
//  string namesT[loopRing];
//  for (int nR=0; nR<loopRing; nR++) {
//    double temp[loopTarg];
//    double dtemp[loopTarg];
//    for(int nT = 0; nT < loopTarg; nT++){
//      temp[nT] = sig[nT][nR];
//      dtemp[nT] = dSig[nT][nR];
//      namesT[nR] = "#R = "+to_string(vRing[nR]);
//    }
//    g2[nR] = new TGraphErrors(loopTarg, vTarg, temp, zero, dtemp);
//    g2[nR]->SetNameTitle(namesT[nR].c_str());
//    if(nR+1 == 5){g2[nR]->SetLineColor(6);}
//    else{g2[nR]->SetLineColor(nR+1);}
//    mg2->Add(g2[nR]);
//  }
//  mg2->SetTitle((reactionInput+"; Target thickness [um]; Sigma [MeV]").c_str());
//  //mg2->GetYaxis()->SetRangeUser(0,ymax_Sigma);
//  mg2->Draw("AL");
//
//  auto legend2 = new TLegend(0.1,0.7,0.3,0.9);
//  legend2->SetTextFont(132);
//  legend2->AddEntry(g2[0],namesT[0].c_str(),"lep");
//  legend2->AddEntry(g2[1],namesT[1].c_str(),"lep");
//  legend2->AddEntry(g2[2],namesT[2].c_str(),"lep");
//  legend2->AddEntry(g2[3],namesT[3].c_str(),"lep");
//  legend2->AddEntry(g2[4],namesT[4].c_str(),"lep");
//  legend2->Draw();
//
//  //=====================================================================
//  TCanvas* cSimStrips = new TCanvas("cSimStrips","cSimStrips",1000,1000);
//  cSimStrips->Divide(loopRing, loopTarg);
//  TCanvas* cMultiplicity = new TCanvas("cMultiplicity","cMultiplicity",1500,1000);
//  cMultiplicity->Divide(loopRing, loopTarg);
//
//  string simfile = harddrive;
//  simfile.append("GRIT_");
//  simfile.append(FindMyFolder(reactionInput));
//  simfile.append("_R2699_Q01R");
//
//  for(int nT = 0; nT < loopTarg; nT++){
//    for(int nR = 0; nR < loopRing; nR++){
//      string load = simfile
//	          + to_string((int)vRing[nR])
//                  + "S32_T" + to_string(sithick)
//                  + "_" + strE
//                  + "MeV_TargT" + to_string((int)vTarg[nT])
//                  + "_" + orb
//		  + "-1.root";
//      cout << load << endl;
//
//      string name = "TThick = " + to_string((int)vTarg[nT]) 
//	          + ", #Rings = " + ringName[nR];
//
//      TTree* stree = LoadSimulatedTree(load.c_str());
//
//      cSimStrips->cd((loopRing*nT)+nR+1);
//      stree->Draw("Mugast.fMG_DSSDXE_StripNbr>>hCheck(130,0,130)",
//	  "Mugast.fMG_DSSDXE_DetectorNbr==10",
//	  //"Mugast.fMG_DSSDXE_DetectorNbr==10 && Mugast.fMG_DSSDXE_DetectorNbr==11 ",
//	  "");
//      TH1F* hCheck = (TH1F*) gDirectory->Get("hCheck");
//      hCheck = SetTitles(hCheck,name.c_str(),"Strip #","Counts");
//      hCheck->GetYaxis()->SetRangeUser(0,ymax_StripCheck);
//
//      string text = to_string((int)hCheck->Integral(0,130)) + " counts";
//
//      TLatex l;
//      l.SetTextSize(0.06);
//      l.DrawLatex(80,2500,text.c_str());
//
//      cMultiplicity->cd((loopRing*nT)+nR+1);
//      stree->Draw("fMG_DSSDXE_StripNbr@.size():fMG_DSSDYE_StripNbr@.size()>>hMult(10,0,5,10,0,5)",
//	          "fMG_DSSDXE_DetectorNbr==10 && fMG_DSSDXE_DetectorNbr!=11",
//		  "colz");
//      TH2F* hMult = (TH2F*) gDirectory->Get("hMult");
//      hMult = SetTitles(hMult,name.c_str(),"Multiplicity X","Multiplicity Y");
//
//      TLine *line = new TLine(0,0,5,5);
//      line->SetLineColor(kRed);
//      line->SetLineStyle(7);
//      line->SetLineWidth(2);
//      line->Draw("same");
//
//    }
//  }

  if(writefiles){
    //save canvases
    string reacAppend = FindMyFolder(reactionInput) + "_" + orb + "wave";
    string sFits  = "./Outputs/stripPitch_fits_"   + reacAppend;
    string sXTarg = "./Outputs/stripPitch_XTarg_" + reacAppend;
    string sXStrp = "./Outputs/stripPitch_XStrp_" + reacAppend;
//    string sCheck  = "./Outputs/stripPitch_check_"   + reacAppend;
//    string sMult  = "./Outputs/stripPitch_mult_"   + reacAppend;
    string sFigure  = "./Outputs/stripPitch_figure_"   + reacAppend;
 
//    cFits->SaveAs((sFits+".root").c_str());
//    cFits->SaveAs((sFits+".pdf").c_str());
//    cTargs->SaveAs((sXTarg+".root").c_str());
//    cTargs->SaveAs((sXTarg+".pdf").c_str());
    cRings->SaveAs((sXStrp+".root").c_str());
    cRings->SaveAs((sXStrp+".pdf").c_str());
//    cSimStrips->SaveAs((sCheck+".root").c_str());
//    cSimStrips->SaveAs((sCheck+".pdf").c_str());
//    cMultiplicity->SaveAs((sMult+".root").c_str());
//    cMultiplicity->SaveAs((sMult+".pdf").c_str());
    cFigure->SaveAs((sFigure+".root").c_str());
    cFigure->SaveAs((sFigure+".pdf").c_str());
  }
}

void GeometricEffic(string rstring, bool gotCut){


  TCanvas* cEx = new TCanvas("cEx","cEx",1000,1000);
  TCanvas* cELTL = new TCanvas("cELTL","cELTL",1000,1000);


  string reactionInput = rstring.substr(0,rstring.find("@"));
  cout << reactionInput << endl;

  //Find proper file indentifiers
  int isoStop = reactionInput.find("(") - 1;
  string strE = rstring.substr(rstring.find("@")+1,4);
  int reacStop = reactionInput.find(")");
  string reac = reactionInput.substr(isoStop+1,5);

  vector<double> sithicks = { 500.,
	                     1000.,
	                     1500.,
	                     2000.,
	                     2500.,
	                     3000.,
	                     3500.,
	                     4000.,
	                     4500.
                            };


  if(gotCut){
//    cELTL->Divide(sithicks.size()+1);
    //cELTL->Divide(5,2);
    cELTL->Divide(3,3);
    gStyle->SetTitleFontSize(0.08);
    gStyle->SetLabelSize(0.06,"x");
    gStyle->SetLabelSize(0.06,"y");
    gStyle->SetTitleSize(0.08,"x");
    gStyle->SetTitleSize(0.08,"y");
    gStyle->SetTitleX(0.5);
    gStyle->SetTitleAlign(23);

    for(int i=0; i<11; i++){
      cELTL->cd(i)->SetLeftMargin(0.2);
      cELTL->cd(i)->SetRightMargin(0.15);
      cELTL->cd(i)->SetBottomMargin(0.2);
    }
  }

  string suffix =//   "MeV_TargT2_flat_oneDet.root";
                  "MeV_TargT2_swave_10MeVu_Full_oneDet.root";
                 // "MeV_TargT2_swave_10MeVu_LowAnglesOnly_oneDet.root";

  //string name = "./" + FindMyFolder(reactionInput)
  string name = FindMyFolder(reactionInput)
              + "/Sim_R2699_Q01R16S16_T20000_" + strE + suffix;
  cout << name << endl;
  auto tree20k = LoadPhysicsTree(name.c_str());

  cEx->cd();
  tree20k->Draw("Ex>>h20k(300,-3,3)","!Layer12 && !Layer11","");
  TH1F* h20k = (TH1F*) gDirectory->Get("h20k");

  int FullEffic = h20k->Integral(1,299);

  cout << " | T = 20000 \t Counts = " << FullEffic << "\t -> " << (FullEffic/FullEffic)*100. << "%"<< endl;

  if(gotCut){
    cELTL->cd(1);
  } else {
    cELTL->cd();
  }

  //tree20k->Draw("ELab:ThetaLab>>h20kELTL(120,0,180,10000,0,100)"," !Layer12 && !Layer11 && ELab<50","colz");
  tree20k->Draw("ELab:ThetaLab>>h20kELTL(120,0,180,20000,0,200)"," !Layer12 && !Layer11 && !PunchThrough","colz");
  TH1F* h20kELTL = (TH1F*) gDirectory->Get("h20kELTL");
  h20kELTL->GetXaxis()->SetTitle("#Theta_{Lab}");
  h20kELTL->GetXaxis()->CenterTitle();
  h20kELTL->GetXaxis()->SetRangeUser(0,30);
  h20kELTL->GetYaxis()->SetTitle("E_{Lab}");
  h20kELTL->GetYaxis()->CenterTitle();
  h20kELTL->GetYaxis()->SetRangeUser(0,50);
  string titletemp = "T = 20 mm";
  h20kELTL->SetTitle(titletemp.c_str());

  NPL::Reaction r(rstring.c_str());
  r.SetExcitation4(0);
  TGraph* gKine = r.GetKinematicLine3();
  gKine->SetLineColor(kRed);
  gKine->Draw("same");


  if(gotCut){
  
    string cutname = harddrive + FindMyFolder(reactionInput) + "/GeoEfficCut.root";
    cout << "CUTLOAD :: " << cutname << endl;
    TFile gcIn(cutname.c_str());
    TCutG* cut = (TCutG*) gcIn.FindObjectAny("cut");
    cut->SetName("cut");

    cout << "==========================================" << endl;
    for(int i=0; i<sithicks.size(); i++){
      //Load tree
      //string name = "./" + FindMyFolder(reactionInput)
      string name = FindMyFolder(reactionInput)
                  + "/Sim_R2699_Q01R16S16_T" + to_string((int)sithicks.at(i))
                  + "_" + strE + suffix;
      auto tree = LoadPhysicsTree(name.c_str());
  
      cEx->cd();
      tree->Draw("Ex>>hsmall(300,-3,3)","!Layer12 && !Layer11 && cut","same");
      //tree->Draw("Ex>>hsmall(300,-3,3)","!Layer12 && !Layer11 && cut && ELab<50","same");
      TH1F* hsmall = (TH1F*) gDirectory->Get("hsmall");
  
      int counts = hsmall->Integral(1,299);
  
      cout << " | T = " << sithicks.at(i) << " \t Counts = " << counts
                                          << "\t -> " << ((double)counts/(double)FullEffic)*100. << "%"
  					  << endl;
    
      //cELTL->cd(2+i);
      cELTL->cd(1+i);


//      gStyle->SetTitleFontSize(0.1,"y");
//      gStyle->SetTitleFontSize(0.3,"t");
      tree->Draw("ELab:ThetaLab>>hELTL(120,0,180,10000,0,100)","!Layer12 && !Layer11 && cut","colz");
      //tree->Draw("ELab:ThetaLab>>hELTL(120,0,180,10000,0,100)","!Layer12 && !Layer11 && cut","colz");
      TH1F* hELTL = (TH1F*) gDirectory->Get("hELTL");
      hELTL->GetXaxis()->SetTitle("#Theta_{Lab}");
      hELTL->GetXaxis()->CenterTitle();
      hELTL->GetXaxis()->SetRangeUser(0,30);
      hELTL->GetYaxis()->SetTitle("E_{Lab}");
      hELTL->GetYaxis()->CenterTitle();
      hELTL->GetYaxis()->SetRangeUser(0,50);
      string titletemp = "T = " + to_string(sithicks.at(i)/1000) + " mm";
      hELTL->SetTitle(titletemp.c_str());
      
      gKine->Draw("same");
    }
  }

}

void HeavyKinematics(){
  TCanvas* c = new TCanvas("c","c",1200,1000);
  c->cd(); c->cd()->SetLeftMargin(0.12);
  c->cd(); c->cd()->SetRightMargin(0.20);
  TMultiGraph *mg = new TMultiGraph();
  
  TGraph* g1 = plot_kine_heavy("47K(d,p)48K@470",0,(Color_t) kRed,2,1);
    mg->Add(g1);
  TGraph* g2 = plot_kine_heavy("47K(d,t)46K@470",0,(Color_t) kRed,2,7);
    mg->Add(g2);
  TGraph* g3 = plot_kine_heavy("47K(p,d)46K@470",0,(Color_t) kRed,2,3);
    mg->Add(g3);
  TGraph* g4 = plot_kine_heavy("47K(p,t)45K@470",0,(Color_t) kRed,2,6);
    mg->Add(g4);
  TGraph* g5 = plot_kine_heavy("47K(p,p)47K@470",0,(Color_t) kRed,2,5);
    mg->Add(g5);
  TGraph* g6 = plot_kine_heavy("47K(d,d)47K@470",0,(Color_t) kRed,2,9);
    mg->Add(g6);

  TGraph* h1 = plot_kine_heavy("94Kr(d,p)95Kr@940",0,(Color_t) kViolet,2,1);
    mg->Add(h1);
  TGraph* h2 = plot_kine_heavy("94Kr(d,t)93Kr@940",0,(Color_t) kViolet,2,7);
    mg->Add(h2);
  TGraph* h3 = plot_kine_heavy("94Kr(p,d)93Kr@940",0,(Color_t) kViolet,2,3);
    mg->Add(h3);
  TGraph* h4 = plot_kine_heavy("94Kr(p,t)92Kr@940",0,(Color_t) kViolet,2,6);
    mg->Add(h4);
  TGraph* h5 = plot_kine_heavy("94Kr(p,p)94Kr@940",0,(Color_t) kViolet,2,5);
    mg->Add(h5);
  TGraph* h6 = plot_kine_heavy("94Kr(d,d)94Kr@940",0,(Color_t) kViolet,2,9);
    mg->Add(h6);

  TGraph* j1 = plot_kine_heavy("132Sn(d,p)133Sn@1320",0,(Color_t) kGreen,2,1);
    mg->Add(j1);
  TGraph* j2 = plot_kine_heavy("132Sn(d,t)131Sn@1320",0,(Color_t) kGreen,2,7);
    mg->Add(j2);
  TGraph* j3 = plot_kine_heavy("132Sn(p,d)131Sn@1320",0,(Color_t) kGreen,2,3);
    mg->Add(j3);
  TGraph* j4 = plot_kine_heavy("132Sn(p,t)130Sn@1320",0,(Color_t) kGreen,2,6);
    mg->Add(j4);
  TGraph* j5 = plot_kine_heavy("132Sn(p,p)132Sn@1320",0,(Color_t) kGreen,2,5);
    mg->Add(j5);
  TGraph* j6 = plot_kine_heavy("132Sn(d,d)132Sn@1320",0,(Color_t) kGreen,2,9);
    mg->Add(j6);

  TGraph* o1 = plot_kine_heavy("22O(d,p)23O@220",0,(Color_t) kAzure,2,1);
    mg->Add(o1);
  TGraph* o2 = plot_kine_heavy("22O(d,t)21O@220",0,(Color_t) kAzure,2,7);
    mg->Add(o2);
  TGraph* o3 = plot_kine_heavy("22O(p,d)21O@220",0,(Color_t) kAzure,2,3);
    mg->Add(o3);
  TGraph* o4 = plot_kine_heavy("22O(p,t)20O@220",0,(Color_t) kAzure,2,6);
    mg->Add(o4);
  TGraph* o5 = plot_kine_heavy("22O(p,p)22O@220",0,(Color_t) kAzure,2,5);
    mg->Add(o5);
  TGraph* o6 = plot_kine_heavy("22O(d,d)22O@220",0,(Color_t) kAzure,2,9);
    mg->Add(o6);

  TGraph* x2 = plot_kine_heavy("9Be(d,t)8Be@90",0,(Color_t) kBlack,0,7);
    mg->Add(x2);

  mg->GetXaxis()->SetTitle("Lab angle [deg]");
  mg->GetYaxis()->SetTitle("Heavy residue energy [MeV]");


//  auto legend = new TLegend(0.81,0.10,0.98,0.90);
////  legend->SetNColumns(3);
//  
//  legend->AddEntry(j1,"132Sn(d,p)133Sn","l");
//  legend->AddEntry(j3,"132Sn(p,d)131Sn","l");
//  legend->AddEntry(j4,"132Sn(p,t)130Sn","l");
//  legend->AddEntry(j2,"132Sn(d,t)131Sn","l");
//  legend->AddEntry(j5,"132Sn(p,p)132Sn","l");
//  legend->AddEntry(j6,"132Sn(d,d)132Sn","l");
//
//  legend->AddEntry(h1,"94Kr(d,p)95Kr","l");
//  legend->AddEntry(h3,"94Kr(p,d)93Kr","l");
//  legend->AddEntry(h4,"94Kr(p,t)92Kr","l");
//  legend->AddEntry(h2,"94Kr(d,t)93Kr","l");
//
//
//  mg->GetXaxis()->SetTitle("Lab angle [deg]");
//  mg->GetYaxis()->SetTitle("Heavy residue energy [MeV]");


  auto legend = new TLegend(0.81,0.10,0.98,0.90);
  //legend->SetNColumns(2);
  
  legend->AddEntry(j1,"132Sn(d,p)133Sn","l");
  legend->AddEntry(j3,"132Sn(p,d)131Sn","l");
  legend->AddEntry(j4,"132Sn(p,t)130Sn","l");
  legend->AddEntry(j2,"132Sn(d,t)131Sn","l");
  legend->AddEntry(j5,"132Sn(p,p)132Sn","l");
  legend->AddEntry(j6,"132Sn(d,d)132Sn","l");

  legend->AddEntry(h1,"94Kr(d,p)95Kr","l");
  legend->AddEntry(h3,"94Kr(p,d)93Kr","l");
  legend->AddEntry(h4,"94Kr(p,t)92Kr","l");
  legend->AddEntry(h2,"94Kr(d,t)93Kr","l");
  legend->AddEntry(h5,"94Kr(p,p)94Kr","l");
  legend->AddEntry(h6,"94Kr(d,d)94Kr","l");

  legend->AddEntry(g1,"47K(d,p)48K","l");
  legend->AddEntry(g3,"47K(p,d)46K","l");
  legend->AddEntry(g4,"47K(p,t)45K","l");
  legend->AddEntry(g2,"47K(d,t)46K","l");
  legend->AddEntry(g5,"47K(p,p)47K","l");
  legend->AddEntry(g6,"47K(d,d)47K","l");

  legend->AddEntry(o1,"22O(d,p)23O","l");
  legend->AddEntry(o3,"22O(p,d)21O","l");
  legend->AddEntry(o4,"22O(p,t)20O","l");
  legend->AddEntry(o2,"22O(d,t)21O","l");
  legend->AddEntry(o5,"22O(p,p)22O","l");
  legend->AddEntry(o6,"22O(d,d)22O","l");

  double axis1max = 7.;
  double ymax = 1400.;

  mg->Draw("AL");
  gPad->Modified();
  mg->GetXaxis()->SetRangeUser(0,axis1max);
  mg->GetXaxis()->SetNdivisions(510);
  mg->GetYaxis()->SetRangeUser(0,ymax);
  mg->GetXaxis()->CenterTitle(true);
  mg->GetXaxis()->SetTitleOffset(1.1);
  mg->GetYaxis()->CenterTitle(true);
  mg->GetYaxis()->SetTitleOffset(1.6);
  legend->Draw();

  double axis2max = 165.*tan(axis1max/(180./pi));
  cout << axis2max << endl;

  TF1 *relfunc = new TF1("relfunc","(180/pi)*atan(x/165)",0.0,20.3);
  TGaxis *axis = new TGaxis(0, ymax, axis1max, ymax,"relfunc",510,"-");

  //TGaxis *axis = new TGaxis(0, ymax, axis1max, ymax,
  //			    0,axis2max,520,"-L");
  axis->SetLabelFont(mg->GetXaxis()->GetLabelFont());
  axis->SetLabelSize(0.04);
  axis->SetTitleFont(mg->GetXaxis()->GetTitleFont());
  axis->SetTitle("Annular radius [mm]");
  axis->CenterTitle(true);
  axis->SetTitleOffset(1.1);
  axis->Draw();

}

void LightKinematics(bool forwards){
  TCanvas* c = new TCanvas("c","c",1200,1000);
  c->cd(); c->cd()->SetLeftMargin(0.12);
  c->cd(); c->cd()->SetRightMargin(0.20);
  TMultiGraph *mg = new TMultiGraph();
  TLegend* legend;// = new TLegend(0.6,0.6,0.9,0.9);
 
  TF1* relfunc;

  string sFunc;
  int lowAng, highAng;

  double annulMinX, annulMaxX, maxy;

  if(!forwards){
    TGraph* g1 = plot_kine_light("47K(d,p)48K@470",0,(Color_t) kRed,2,1);
    mg->Add(g1);
    TGraph* h1 = plot_kine_light("94Kr(d,p)95Kr@940",0,(Color_t) kViolet,2,1);
    mg->Add(h1);
    TGraph* j1 = plot_kine_light("132Sn(d,p)133Sn@1320",0,(Color_t) kGreen,2,1);
    mg->Add(j1);
 
    TGraph* g2 = plot_kine_light("47K(t,p)49K@470",0,(Color_t) kRed,2,5);
    mg->Add(g2);
    TGraph* h2 = plot_kine_light("94Kr(t,p)96Kr@940",0,(Color_t) kViolet,2,5);
    mg->Add(h2);
    TGraph* j2 = plot_kine_light("132Sn(t,p)134Sn@1320",0,(Color_t) kGreen,2,5);
    mg->Add(j2);
    
    TGraph* g3 = plot_kine_light("47K(a,p)50Ca@470",0,(Color_t) kRed,2,7);
    mg->Add(g3);
    TGraph* h3 = plot_kine_light("94Kr(a,p)97Rb@940",0,(Color_t) kViolet,2,7);
    mg->Add(h3);
    TGraph* j3 = plot_kine_light("132Sn(a,p)135Sb@1320",0,(Color_t) kGreen,2,7);
    mg->Add(j3);
    
    lowAng = 120; highAng = 180;

    legend = new TLegend(0.81,0.10,0.98,0.90);
    legend->AddEntry(j1,"132Sn(d,p)133Sn","l");
    legend->AddEntry(h1,"94Kr(d,p)95Kr","l");
    legend->AddEntry(g1,"47K(d,p)48K","l");
    legend->AddEntry(j2,"132Sn(t,p)134Sn","l");
    legend->AddEntry(h2,"94Kr(t,p)96Kr","l");
    legend->AddEntry(g2,"47K(t,p)49K","l");
    legend->AddEntry(j3,"132Sn(a,p)135Sb","l");
    legend->AddEntry(h3,"94Kr(a,p)97Rb","l");
    legend->AddEntry(g3,"47K(a,p)50Ca","l");

    annulMinX = 163.5;
    annulMaxX = 175.5;
    maxy = 6.;
  
    relfunc = new TF1("relfunc","-(180./pi)*atan(x/165.)",0.0,60);
  
  } else {
    TGraph* g2 = plot_kine_light("47K(d,t)46K@470",0,(Color_t) kRed,2,1);
    mg->Add(g2);
    TGraph* h2 = plot_kine_light("94Kr(d,t)93Kr@940",0,(Color_t) kViolet,2,1);
    mg->Add(h2);
    TGraph* j2 = plot_kine_light("132Sn(d,t)131Sn@1320",0,(Color_t) kGreen,2,1);
    mg->Add(j2);
  
    TGraph* g3 = plot_kine_light("47K(p,d)46K@470",0,(Color_t) kRed,2,3);
    mg->Add(g3);
    TGraph* h3 = plot_kine_light("94Kr(p,d)93Kr@940",0,(Color_t) kViolet,2,3);
    mg->Add(h3);
    TGraph* j3 = plot_kine_light("132Sn(p,d)131Sn@1320",0,(Color_t) kGreen,2,3);
    mg->Add(j3);
 
    //TGraph* g4 = plot_kine_light("47K(p,a)44Ar@470",0,(Color_t) kRed,2,3);
    //mg->Add(g4);
    //TGraph* h4 = plot_kine_light("94Kr(p,a)91Br@940",0,(Color_t) kViolet,2,3);
    //mg->Add(h4);
    //TGraph* j4 = plot_kine_light("132Sn(p,a)129In@1320",0,(Color_t) kGreen,2,3);
    //mg->Add(j4);
 
    TGraph* g5 = plot_kine_light("47K(p,t)45K@470",0,(Color_t) kRed,2,8);
    mg->Add(g5);
    TGraph* h5 = plot_kine_light("94Kr(p,t)92Kr@940",0,(Color_t) kViolet,2,8);
    mg->Add(h5);
    TGraph* j5 = plot_kine_light("132Sn(p,t)130Sn@1320",0,(Color_t) kGreen,2,8);
    mg->Add(j5);
  
    lowAng = 0; highAng = 59;//286;

    legend = new TLegend(0.81,0.10,0.98,0.90);
  
  
    legend->AddEntry(j2,"132Sn(d,t)131Sn","l");
    legend->AddEntry(j3,"132Sn(p,d)131Sn","l");
    legend->AddEntry(j5,"132Sn(p,t)130Sn","l");
    //legend->AddEntry(j4,"132Sn(p,a)129In","l");
    legend->AddEntry(h2,"94Kr(d,t)93Kr","l");
    legend->AddEntry(h3,"94Kr(p,d)93Kr","l");
    legend->AddEntry(h5,"94Kr(p,t)92Kr","l");
    //legend->AddEntry(h4,"94Kr(p,a)91Br","l");
    legend->AddEntry(g2,"47K(d,t)46K","l");
    legend->AddEntry(g3,"47K(p,d)46K","l");
    legend->AddEntry(g5,"47K(p,t)45K","l");
    //legend->AddEntry(g4,"47K(p,a)44Ar","l");
    //legend->SetNColumns(2);


    annulMinX = 4.5;
    annulMaxX = 16.5;
    maxy = 100.;

    relfunc = new TF1("relfunc","(180./pi)*atan(x/165.)",0.0,286);
    //relfunc = new TF1("relfunc","165.*tan(x/(180./pi))",0.0,286);
    //sFunc = "-(180./pi)*atan(x/165.)";

  }

  TLine* lmin = new TLine(annulMinX,0,annulMinX,maxy);
  TLine* lmax = new TLine(annulMaxX,0,annulMaxX,maxy);
  lmin->SetLineWidth(2);
  lmax->SetLineWidth(2);
  lmin->SetLineStyle(7);
  lmax->SetLineStyle(7);

  mg->GetXaxis()->SetTitle("Lab angle [deg]");
  mg->GetYaxis()->SetTitle("Light ejectile energy [MeV]");

//  legend->AddEntry(o1,"22O(d,p)23O","l");
//  legend->AddEntry(o3,"22O(p,d)21O","l");
//  legend->AddEntry(o2,"22O(d,t)21O","l");

 
  mg->Draw("AL");
  gPad->Modified();
  mg->GetXaxis()->SetRangeUser(lowAng,highAng);
  mg->GetYaxis()->SetRangeUser(0,maxy);
  mg->GetXaxis()->CenterTitle(true);
  mg->GetYaxis()->CenterTitle(true);
  legend->Draw();
  lmin->Draw("same");
  lmax->Draw("same");


  double axis2min = 165.*tan(lowAng/(180./pi));
  double axis2max = 165.*tan(highAng/(180./pi));

  //TF1 *relfunc = new TF1("relfunc","165.*tan(x/(180./pi))",0.0,60);
  //TF1 *relfunc = new TF1("relfunc",sFunc.c_str(),0.0,axisMax);
  //relfunc->Draw();

  TGaxis *axis = new TGaxis(lowAng, maxy, highAng, maxy,"relfunc",510,"-");
  //TGaxis *axis = new TGaxis(lowAng, maxy, highAng, maxy,"relfunc",1050,"-");
  axis->SetLabelFont(mg->GetXaxis()->GetLabelFont());
  axis->SetLabelSize(0.04);
  axis->SetTitleFont(mg->GetXaxis()->GetTitleFont());
  axis->SetTitle("Annular radius [mm]");
  axis->CenterTitle(true);
  axis->SetTitleOffset(1.1);
  axis->Draw();



}

//======================================================================================

void Plotting(){
  gStyle->SetOptStat(0);
  //gStyle->SetOptStat(11111111);

//GraphFits(     "47K(d,p)48K@470", 5, 5, "swave", "!PunchThrough", 16000, 5, 7000,  0.2, 1, 0);
//GraphFits(   "94Kr(d,p)95Kr@940", 5, 5, "swave", "!PunchThrough", 12000, 5, 4500,  0.2, 1, 0);
//GraphFits("132Sn(d,p)133Sn@1320", 5, 5, "swave", "!PunchThrough",  6000, 3, 1600,  0.2, 1, 0);

//GraphFits(     "47K(d,t)46K@470", 5, 5, "swave", "!PunchThrough", 26000,  5, 9000,  0.2, 1, 0);
//GraphFits(   "94Kr(d,t)93Kr@940", 5, 5, "swave", "!PunchThrough",  5000,  5, 5000,  0.2, 1, 0);
//GraphFits("132Sn(d,t)131Sn@1320", 5, 5, "swave", "!PunchThrough",  8000,  5,  800,  0.2, 1, 0);

//GraphFits(     "47K(p,d)46K@470", 5, 5, "swave", "!PunchThrough",  6000, 30, 8000, 0.2, 1, 0);
//GraphFits(   "94Kr(p,d)93Kr@940", 5, 5, "swave", "!PunchThrough", 10000, 50, 600,  0.2, 1, 0);
//GraphFits("132Sn(p,d)131Sn@1320", 5, 5, "swave", "!PunchThrough", 12000, 100,1200, 0.2, 1, 0);

//GraphFits(     "47K(p,t)45K@470", 5, 5, "swave", "!PunchThrough&& Layer11&&!Layer12", 16000, 100, 9000,  0.2, 1, 0);
//GraphFits(   "94Kr(p,t)92Kr@940", 5, 5, "swave", "!PunchThrough&&!Layer11&&!Layer12",  2500, 100, 9000,  0.2, 1, 0);
//GraphFits("132Sn(p,t)130Sn@1320", 5, 5, "swave", "!PunchThrough&&!Layer11&&!Layer12", 10000, 100, 9000,  0.2, 1, 0);
}
