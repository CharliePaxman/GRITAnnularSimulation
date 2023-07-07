/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 * Edited for e793s                                                          *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    : May 2021                                                 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class describing the property of an Analysis object                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include<iostream>
#include<string>

#include<fstream>

using namespace std;

#include "Analysis.h"
#include "NPFunction.h"
#include "NPAnalysisFactory.h"
#include "NPDetectorManager.h"
#include "NPOptionManager.h"
#include "DefineColours.h"

////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis(){
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::Init() {
 ///////////////////////////////////////////////////////////////////////////////  

  if(NPOptionManager::getInstance()->HasDefinition("Sim")){
    cout << " == == == == SIMULATION == == == ==" << endl;
      isSim=true;
      isPhaseSpace=false;
  } else if (NPOptionManager::getInstance()->HasDefinition("Exp")) {
    cout << " == == == == EXPERIMENT == == == ==" << endl;
      isSim=false;
      isPhaseSpace=false;
  } else {
    cout << " == == == == PHASE SPACE == == == ==" << endl;
      isSim=false;
      //isSim=true;
      isPhaseSpace=true;
  }

  if(       NPOptionManager::getInstance()->HasDefinition(" 500")){
    SiThick = 0.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition(" 100")){
    SiThick = 0.2*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition(" 200")){
    SiThick = 0.2*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition(" 300")){
    SiThick = 0.2*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition(" 400")){
    SiThick = 0.2*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("1000")){
    SiThick = 1.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("1500")){
    SiThick = 1.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("2000")){
    SiThick = 2.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("2500")){
    SiThick = 2.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("3000")){
    SiThick = 3.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("3500")){
    SiThick = 3.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("4000")){
    SiThick = 4.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("4500")){
    SiThick = 4.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("5000")){
    SiThick = 5.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("5500")){
    SiThick = 5.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("6000")){
    SiThick = 6.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("6500")){
    SiThick = 6.5*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("7000")){
    SiThick = 7.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("10000")){
    SiThick = 10.0*mm;
  } else if(NPOptionManager::getInstance()->HasDefinition("20000")){
    SiThick = 20.0*mm;
  }


  if(NPOptionManager::getInstance()->HasDefinition("TreatPunchThru")){
    treatPunchThru = true;
  } else if (NPOptionManager::getInstance()->HasDefinition("DontTreatPunchThru")){
    treatPunchThru = false;
  } else {
    cout << "!!!!!!!!!!!! NO PUNCHTHRU TREATMENT DECIDED? !!!!!!!!!!!!!!!" << endl;
    treatPunchThru = true;
  }


  Initial = new TInitialConditions();
  ReactionConditions = new TReactionConditions(); 
  RootInput::getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial);

  // Initilize MGX histograms, because must be inside function
  string base1 = "ThetaCM_detected_MG";
  string base2 = "ThetaLab_detected_MG";
  for(int i=0; i<18; i++){
    int j=i+1;
    string name1 = base1 + to_string(j);	  
    string name2 = base2 + to_string(j);	  
    ThetaCM_detected_MGX[i] = new TH1F(name1.c_str(),name1.c_str(),NumThetaAngleBins,0,180); //900 bins for 0.2 angular bin width
    ThetaLab_detected_MGX[i] = new TH1F(name2.c_str(),name2.c_str(),NumThetaAngleBins,0,180);
  }

  // initialize input and output branches
  InitOutputBranch();
  InitInputBranch();
  
  MG = (TMugastPhysics*) m_DetectorManager -> GetDetector("Mugast");
  ML = (TModularLeafPhysics*) m_DetectorManager -> GetDetector("ModularLeaf"); 

  // get reaction information
  reaction.ReadConfigurationFile(NPOptionManager::getInstance()->GetReactionFile());
  OriginalBeamEnergy = reaction.GetBeamEnergy();
  reaction.Print(); 

  // get beam position from .reaction file
  Beam = (NPL::Beam*) reaction.GetParticle1(); 
  XBeam = Beam->GetMeanX();
  YBeam = Beam->GetMeanY();

  cout << " ---------------- Beam Position ---------------- " << endl;
  cout << " \tX = " << XBeam << " mm\tY = " << YBeam  << " mm" << endl;
  cout << " ----------------------------------------------- " << endl;

  // target thickness
  TargetThickness = m_DetectorManager->GetTargetThickness();
  string TargetMaterial = m_DetectorManager->GetTargetMaterial();
  
  cout << " ---------------- Target Values ---------------- " << endl;
  cout << " \tZ = " << m_DetectorManager->GetTargetZ() << " mm\tT = " << TargetThickness  << " mm" << endl;
  cout << " ----------------------------------------------- " << endl;
  
  // energy losses
  string light = NPL::ChangeNameToG4Standard(reaction.GetParticle3()->GetName());
  string beam = NPL::ChangeNameToG4Standard(reaction.GetParticle1()->GetName());
  LightTarget = NPL::EnergyLoss(light+"_"+TargetMaterial+".G4table","G4Table",100 );
  LightAl = NPL::EnergyLoss(light+"_Al.G4table" ,"G4Table",100);
  LightSi = NPL::EnergyLoss(light+"_Si.G4table" ,"G4Table",100);
  BeamTargetELoss = NPL::EnergyLoss(beam+"_"+TargetMaterial+".G4table","G4Table",100);

  FinalBeamEnergy = BeamTargetELoss.Slow(OriginalBeamEnergy, 0.5*TargetThickness, 0);
  reaction.SetBeamEnergy(FinalBeamEnergy); 

  cout << "\033[91m Running for reaction "
       << reaction.GetParticle1()->GetName() << "("
       << reaction.GetParticle2()->GetName() << ","
       << reaction.GetParticle3()->GetName() << ")"
       << reaction.GetParticle4()->GetName() << endl;

  cout << "\033[36m Beam energy at mid-target: " << FinalBeamEnergy << "\033[37m"<< endl;

  // initialize various parameters
  Rand = TRandom3();
  ParticleMult = 0;
  GammaMult = 0;
  DetectorNumber = 0;
  ThetaNormalTarget = 0;
  ThetaMGSurface = 0;
  Energy = 0;
  ThetaGDSurface = 0;
  elab_tmp = 0;
  thetalab_tmp = 0;
  philab_tmp = 0;
  ThetaGDSurface = 0;
  X.clear();
  Y.clear();
  Z.clear();
  dE = 0;
  BeamDirection = TVector3(0,0,1);
  AHeavy=reaction.GetParticle4()->GetA();
  ALight=reaction.GetParticle3()->GetA(); 
  MHeavy=reaction.GetParticle4()->Mass();
  MLight=reaction.GetParticle3()->Mass();

  for(int i=0;i<GATCONF_SIZE;i++){ // loop over the bits
    GATCONF_Counter[i] = 0 ; 
  }

//  ThetaCM_detected->Sumw2();
//  ThetaLab_detected->Sumw2();
//cout << "here_endInit" << endl;
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent(){
  // Reinitiate calculated variable
  ReInitValue();

//cout << "---- Event ----" << endl; int writecount = 0;

  ThetaCM_emmitted->Fill(ReactionConditions->GetThetaCM());
  ThetaLab_emmitted->Fill(ReactionConditions->GetTheta(0));

  GATCONF_MASTER=ML->GetCalibratedValue("GATCONF_MASTER");
  for(int i=0;i<GATCONF_SIZE;i++){ // loop over the bits
    if(GATCONF_MASTER & (unsigned int)pow(2,i)){ // test if ith bit is on
      GATCONF_Counter[i]++ ; // increment the array
    }
  }

//cout << writecount << endl; writecount++;

  OriginalELab = ReactionConditions->GetKineticEnergy(0);
  OriginalThetaLab = ReactionConditions->GetTheta(0);
  BeamEnergy = ReactionConditions->GetBeamEnergy();

//cout << writecount << endl; writecount++;

  TVector3 BeamDirection(0.,0.,1.);
  BeamImpact = TVector3(XBeam,YBeam,m_DetectorManager->GetTargetZ()); 

//cout << writecount << endl; writecount++;
  unsigned int sizeMG = MG->DSSD_E.size();
//cout << "sizeMG = " << sizeMG << endl;
  if(sizeMG==0){
    //cout << "NEED TO RETURN!" << endl; 
    return;
  }
  bool punch = false;
  if(sizeMG>1){
    punch = true;
  }

//cout << writecount << endl; writecount++;
  int Count10 = -10;
  Energy = elab_tmp = 0;
  //bool layers[3] = {0};
  bool layers[4] = {0};

//cout << writecount << endl; writecount++;
  for(unsigned int countMugast = 0; countMugast<sizeMG; countMugast++){
    if(MG->TelescopeNumber[countMugast] == 10){
//cout << "MG10" << endl;
      layers[0]=1; 
      Energy += MG->GetEnergyDeposit(countMugast);
      Count10 = countMugast;
    } else if(MG->TelescopeNumber[countMugast] == 11){
//cout << "MG11" << endl;
      layers[1]=1; 
      Energy += MG->GetEnergyDeposit(countMugast);
    } else if(MG->TelescopeNumber[countMugast] == 12){
//cout << "MG12" << endl;
      layers[2]=1; 
      //Don't add me to energy
    } else if(MG->TelescopeNumber[countMugast] == 13){
      layers[3]=1; 
    }
  }

//cout << writecount << endl; writecount++;
  if(Count10==-10){
    //cout << "NEED TO RETURN!" << endl; 
    return;
  }
    
  // Part 1 : Impact Angle
  ThetaMGSurface = 0;
  ThetaNormalTarget = 0;
  thetalab_tmp = 0;
  philab_tmp = 0;
//cout << "beforeCount10" << endl;
  //TVector3 HitDirection = MG->GetPositionOfInteraction(Count10) - BeamImpact;
  TVector3 HitDirection = MG->GetPositionOfInteraction(0) - BeamImpact;
//cout << "afterCount10" << endl;
  thetalab_tmp = HitDirection.Angle(BeamDirection);
  philab_tmp = HitDirection.Phi();

//cout << writecount << endl; writecount++;

  X.push_back( MG -> GetPositionOfInteraction(Count10).X());
  Y.push_back( MG -> GetPositionOfInteraction(Count10).Y());
  Z.push_back( MG -> GetPositionOfInteraction(Count10).Z());

//cout << writecount << endl; writecount++;

  ThetaMGSurface = HitDirection.Angle(TVector3(0.0,0.0,-1.0)); //NORMAL OF DOWNSTREAM
  ThetaNormalTarget = HitDirection.Angle(TVector3(0.0,0.0,1.0));

  // Part 2 : Impact Energy
  RawEnergy.push_back(Energy);
  elab_tmp = Energy; //so I can add and remove sections
 
//cout << writecount << endl; writecount++;

  bool tempPunch = false;

  if(treatPunchThru && layers[3]){
    tempPunch = true;
    elab_tmp = LightSi.EvaluateEnergyFromDeltaE(
               elab_tmp,       //particle energy after leaving detector
       	       SiThick + 0.5* millimeter,//thickness of detector passed through
      	       ThetaMGSurface, //angle of entry to detector
      	       0.001*MeV,      //minimum punchthrough energy
      	       500.0*MeV,      //computational upper energy limit
      	       0.001*MeV,      //computational resolution limit
      	       1000            //max steps
      	       );
  }

//cout << writecount << endl; writecount++;

  elab_tmp = LightTarget.EvaluateInitialEnergy(
      	     elab_tmp,            //particle energy after leaving target
      	     TargetThickness*0.5, //distance passed through target
      	     ThetaNormalTarget);  //angle of exit from target

  PunchThrough.push_back(tempPunch);
  Layer10.push_back(layers[0]);
  Layer11.push_back(layers[1]);
  Layer12.push_back(layers[2]);
  Layer13.push_back(layers[3]);
  ELab.push_back(elab_tmp);
  X.push_back( MG -> GetPositionOfInteraction(Count10).X());
  Y.push_back( MG -> GetPositionOfInteraction(Count10).Y());
  Z.push_back( MG -> GetPositionOfInteraction(Count10).Z());
  Ex.push_back(reaction.ReconstructRelativistic(elab_tmp,thetalab_tmp));
  Ecm.push_back(elab_tmp*(AHeavy+ALight)/(4*AHeavy*cos(thetalab_tmp)*cos(thetalab_tmp)));
  ThetaLab.push_back(thetalab_tmp/deg);
  PhiLab.push_back(philab_tmp/deg);
  ThetaCM.push_back(reaction.EnergyLabToThetaCM(elab_tmp, thetalab_tmp)/deg);

//cout << writecount << endl; writecount++;

  if(sizeMG==1){
    MG_T = MG->DSSD_T[Count10];
    MG_E = MG->DSSD_E[Count10];
    MG_X = MG->DSSD_X[Count10];
    MG_Y = MG->DSSD_Y[Count10];
    MG_D = MG->TelescopeNumber[Count10];
  }
}

////////////////////////////////////////////////////////////////////////////////
void FillSolidAngles(TH1F* hSA, TH1F* hDet, TH1F* hEmm){
//cout << "here_FillSolidAngles" << endl;
  if(!filledCline){
    for(int t=0; t<NumThetaAngleBins; t++){
      double angleMin = (t)*(180./NumThetaAngleBins);
      double angleMax = (t+1)*(180./NumThetaAngleBins);
      clineVal.push_back(2.0*M_PI*(cos(angleMin*degtorad) - cos(angleMax*degtorad)));
      clineX.push_back(angleMin+((angleMax-angleMin)/2.0));
      filledCline=true;
    }
  }

  for (int i=0; i<hSA->GetNbinsX(); ++i){
    double val = hDet->GetBinContent(i) / hEmm->GetBinContent(i);
    double valerr = val * sqrt( 
      pow(hDet->GetBinError(i) / hDet->GetBinContent(i), 2) +
      pow(hEmm->GetBinError(i) / hEmm->GetBinContent(i), 2) );
    if (isnan(val)) { val = 0; valerr = 0; }
    val *= clineVal.at(i);
    valerr *= clineVal.at(i);

    hSA->SetBinContent(i, val);
    hSA->SetBinError(i, valerr);
  }
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::End(){
  cout << endl << "\e[1;32m GATCONF Statistics " << endl ;
  for(int i=0;i<GATCONF_SIZE;i++){ // loop over the bits
    cout << GATCONF_Label[i]  << "\t\t" << GATCONF_Counter[i] << endl ; 
  }
  cout << endl ;

  if(isSim && !isPhaseSpace){
    
    //TObjArray HistList(0);
    TList *HistList = new TList();
    HistList->Add(ThetaCM_emmitted);
    HistList->Add(ThetaLab_emmitted);

    //MUGAST
    auto Efficiency_CM_MG = new TH1F(*ThetaCM_detected_MG);
    Efficiency_CM_MG->SetName("Efficiency_CM_MG");
    Efficiency_CM_MG->SetTitle("Efficiency_CM_MG");
    Efficiency_CM_MG->Sumw2();
    auto Efficiency_Lab_MG = new TH1F(*ThetaLab_detected_MG);
    Efficiency_Lab_MG->SetName("Efficiency_Lab_MG");
    Efficiency_Lab_MG->SetTitle("Efficiency_Lab_MG");
    Efficiency_Lab_MG->Sumw2();
     
    auto SolidAngle_CM_MG = new TH1F(*ThetaCM_detected_MG);
    SolidAngle_CM_MG->SetName("SolidAngle_CM_MG");
    SolidAngle_CM_MG->SetTitle("SolidAngle_CM_MG");
    auto SolidAngle_Lab_MG = new TH1F(*ThetaLab_detected_MG);
    SolidAngle_Lab_MG->SetName("SolidAngle_Lab_MG");
    SolidAngle_Lab_MG->SetTitle("SolidAngle_Lab_MG");

    Efficiency_CM_MG->Divide(ThetaCM_emmitted);
    Efficiency_Lab_MG->Divide(ThetaLab_emmitted);

    FillSolidAngles(SolidAngle_CM_MG, ThetaCM_detected_MG, ThetaCM_emmitted);
    FillSolidAngles(SolidAngle_Lab_MG, ThetaLab_detected_MG, ThetaLab_emmitted);

    HistList->Add(ThetaCM_detected_MG);
    HistList->Add(ThetaLab_detected_MG);
    HistList->Add(Efficiency_CM_MG);
    HistList->Add(Efficiency_Lab_MG);
    HistList->Add(SolidAngle_CM_MG);
    HistList->Add(SolidAngle_Lab_MG);

    // MUGAST INDIVIDUAL
    TH1F *SolidAngle_CM_MGX[18];
    string base1 = "SolidAngle_CM_MG";
    for(int i=0;i<18;i++){
      int j=i+1;
      string name = base1 + to_string(j);	  
      SolidAngle_CM_MGX[i] = new TH1F(*ThetaCM_detected_MGX[i]);
      SolidAngle_CM_MGX[i]->SetName(name.c_str());
      SolidAngle_CM_MGX[i]->SetTitle(name.c_str());
      FillSolidAngles(SolidAngle_CM_MGX[i], ThetaCM_detected_MGX[i], ThetaCM_emmitted);
    }

    TH1F *SolidAngle_Lab_MGX[18];
    string base2 = "SolidAngle_Lab_MG";
    for(int i=0;i<18;i++){
      int j=i+1;
      string name = base2 + to_string(j);	  
      SolidAngle_Lab_MGX[i] = new TH1F(*ThetaLab_detected_MGX[i]);
      SolidAngle_Lab_MGX[i]->SetName(name.c_str());
      SolidAngle_Lab_MGX[i]->SetTitle(name.c_str());
      FillSolidAngles(SolidAngle_Lab_MGX[i], ThetaLab_detected_MGX[i], ThetaLab_emmitted);
    }

    for(int i=0; i<18; i++){HistList->Add(ThetaCM_detected_MGX[i]);}
    for(int i=0; i<18; i++){HistList->Add(ThetaLab_detected_MGX[i]);}

    for(int i=0; i<18; i++){HistList->Add(SolidAngle_CM_MGX[i]);}
    for(int i=0; i<18; i++){HistList->Add(SolidAngle_Lab_MGX[i]);}
    
    auto clineValGraph = new TGraph(NumThetaAngleBins);
    clineValGraph->SetName("clineValGraph");
    clineValGraph->SetTitle("clineValGraph");
    for(int b=0; b<NumThetaAngleBins; b++){
      clineValGraph->SetPoint(b,clineX.at(b),clineVal.at(b));
    }
    HistList->Add(clineValGraph);

    auto HistoFile = new TFile("./SolidAngle_HistFile_New.root","RECREATE");
    HistList->Write();
    HistoFile->Close();


    cout << "!!! RENAME SOLID ANGLE FILE! !!!" << endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch(){
//cout << "here_InitOutBranch" << endl;
  RootOutput::getInstance()->GetTree()->Branch("PunchThrough",&PunchThrough);
  RootOutput::getInstance()->GetTree()->Branch("Layer10",&Layer10);
  RootOutput::getInstance()->GetTree()->Branch("Layer11",&Layer11);
  RootOutput::getInstance()->GetTree()->Branch("Layer12",&Layer12);
  RootOutput::getInstance()->GetTree()->Branch("Layer13",&Layer13);
  RootOutput::getInstance()->GetTree()->Branch("Ex",&Ex);
  RootOutput::getInstance()->GetTree()->Branch("EDC",&EDC);
  RootOutput::getInstance()->GetTree()->Branch("ELab",&ELab);
  RootOutput::getInstance()->GetTree()->Branch("Ecm",&Ecm);
  RootOutput::getInstance()->GetTree()->Branch("RawEnergy",&RawEnergy);
  RootOutput::getInstance()->GetTree()->Branch("ELoss_Al",&ELoss_Al);
  RootOutput::getInstance()->GetTree()->Branch("ELoss_Target",&ELoss_Target);
  RootOutput::getInstance()->GetTree()->Branch("ELoss",&ELoss);
  RootOutput::getInstance()->GetTree()->Branch("ThetaLab",&ThetaLab);
  RootOutput::getInstance()->GetTree()->Branch("PhiLab",&PhiLab);
  RootOutput::getInstance()->GetTree()->Branch("ThetaCM",&ThetaCM);
  RootOutput::getInstance()->GetTree()->Branch("Run",&Run,"Run/I");
  RootOutput::getInstance()->GetTree()->Branch("X",&X);
  RootOutput::getInstance()->GetTree()->Branch("Y",&Y);
  RootOutput::getInstance()->GetTree()->Branch("Z",&Z);
  RootOutput::getInstance()->GetTree()->Branch("dE",&dE,"dE/D");
  RootOutput::getInstance()->GetTree()->Branch("MG_T",MG_T);
  RootOutput::getInstance()->GetTree()->Branch("MG_E",MG_E);
  RootOutput::getInstance()->GetTree()->Branch("MG_X",MG_X);
  RootOutput::getInstance()->GetTree()->Branch("MG_Y",MG_Y);
  RootOutput::getInstance()->GetTree()->Branch("MG_D",MG_D);

  RootOutput::getInstance()->GetTree()->Branch("OriginalELab",&OriginalELab,"OriginalELab/D");
  RootOutput::getInstance()->GetTree()->Branch("OriginalThetaLab",&OriginalThetaLab,"OriginalThetaLab/D");
  RootOutput::getInstance()->GetTree()->Branch("BeamEnergy",&BeamEnergy,"BeamEnergy/D");
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch(){
//cout << "here_InitInBranch" << endl;
  SetBranchStatus();
  RootInput::getInstance()->GetChain()->SetBranchAddress("InitialConditions",&Initial);
  RootInput::getInstance()->GetChain()->SetBranchAddress("InteractionCoordinates",&Interaction);
  RootInput::getInstance()->GetChain()->SetBranchAddress("ReactionConditions",&ReactionConditions);
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::SetBranchStatus(){
//cout << "here_SetBranch" << endl;
  // Set Branch status 
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InitialConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fIC_*",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("InteractionCoordinates",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("ReactionConditions",true );
  RootInput:: getInstance()->GetChain()->SetBranchStatus("fRC_*",true );
}

////////////////////////////////////////////////////////////////////////////////
void Analysis::ReInitValue(){
//cout << "here_ReInit" << endl;
  PunchThrough.clear();
  Layer10.clear();
  Layer11.clear();
  Layer12.clear();
  Layer13.clear();
  Ex.clear();
  Ecm.clear();
  ELab.clear();
  RawEnergy.clear(); 
  ELoss_Al.clear();
  ELoss_Target.clear();
  ELoss.clear();
  ThetaLab.clear();
  PhiLab.clear();
  ThetaCM.clear();
  X.clear();
  Y.clear();
  Z.clear();
  dE= -1000;
  ParticleMult = 0;
  GammaMult = 0;
  DetectorNumber = 0;
  ThetaNormalTarget = 0;
  ThetaMGSurface = 0;
  Energy = 0;
  ThetaGDSurface = 0;
  BeamDirection = TVector3(0,0,1);
  elab_tmp = 0;
  thetalab_tmp = 0;
  philab_tmp = 0;

  filledCline=false;

  MG_T=-1000;
  MG_E=-1000;
  MG_X=-1000;
  MG_Y=-1000;
  MG_D=-1000;

  EDC.clear();
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the AnalysisFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
  return (NPL::VAnalysis*) new Analysis();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
  class proxy_analysis{
    public:
      proxy_analysis(){
        NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
      }
  };

  proxy_analysis p_analysis;
}

