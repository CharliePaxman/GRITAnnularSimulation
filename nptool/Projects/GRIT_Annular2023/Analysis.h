#ifndef Analysis_h 
#define Analysis_h
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
#include"NPVAnalysis.h"
#include"NPEnergyLoss.h"
#include"NPReaction.h"
#include"NPBeam.h"
#include"RootOutput.h"
#include"RootInput.h"
#include "TF1.h"
#include "TInitialConditions.h"
#include "TReactionConditions.h"
#include "TInteractionCoordinates.h"
#include "TMust2Physics.h"
#include "TMugastPhysics.h"
#include "GaspardTracker.h"
#include "TCATSPhysics.h"
//#include "../../NPLib/Detectors/ModularLeaf/TModularLeafPhysics.h"
#include "TModularLeafPhysics.h"
#include <TRandom3.h>
#include <TVector3.h>
#include <TMath.h>
#include <bitset>

int NumThetaAngleBins = 1800;// 180 = 1 deg, 360 = 0.5 deg, 900 = 0.2 deg, 1800 = 0.1 deg

auto ThetaCM_emmitted = new TH1F("ThetaCM_emmitted","ThetaCM_emmitted",NumThetaAngleBins,0,180);
//auto ThetaCM_detected_MM = new TH1F("ThetaCM_detected_MM","ThetaCM_detected_MM",NumThetaAngleBins,0,180);
auto ThetaCM_detected_MG = new TH1F("ThetaCM_detected_MG","ThetaCM_detected_MG",NumThetaAngleBins,0,180);

auto ThetaLab_emmitted = new TH1F("ThetaLab_emmitted","ThetaLab_emmitted",NumThetaAngleBins,0,180);
//auto ThetaLab_detected_MM = new TH1F("ThetaLab_detected_MM","ThetaLab_detected_MM",NumThetaAngleBins,0,180);
auto ThetaLab_detected_MG = new TH1F("ThetaLab_detected_MG","ThetaLab_detected_MG",NumThetaAngleBins,0,180);
//auto HistCline_MM = new TH1F("HistCline_MM","HistClint_MM",NumThetaAngleBins,0,180);
//auto HistCline_MG = new TH1F("HistCline_MG","HistClint_MG",NumThetaAngleBins,0,180);

double degtorad = M_PI/180.;
vector<double> clineVal, clineX;
bool filledCline;

TH1F *ThetaCM_detected_MGX[18];
//TH1F *ThetaCM_detected_MMX[5];
TH1F *ThetaLab_detected_MGX[18];
//TH1F *ThetaLab_detected_MMX[5];

class Analysis: public NPL::VAnalysis{
  public:
    Analysis();
    ~Analysis();

  public: 
    void Init();
    void TreatEvent();
    void End();

    void InitOutputBranch();
    void InitInputBranch();
    void SetBranchStatus();
    void CalculateVamos();
    void ReInitValue();
    static NPL::VAnalysis* Construct();

  private:
    unsigned int ParticleMult;
    unsigned int GammaMult;
    //double EDC;
    std::vector<double> EDC;
    vector<double> AddBack_EDC;
    vector<double> AddBack_EDC2;
    vector<double> AGATA_GammaPx;
    vector<double> AGATA_GammaPy;
    vector<double> AGATA_GammaPz;
    vector<double> AGATA_GammaE;
    vector<double> AGATA_OrigBetaX;
    vector<double> AGATA_OrigBetaY;
    vector<double> AGATA_OrigBetaZ;
    double EAgata;
    std::vector<double> ELab;
    std::vector<double> Ex;
    std::vector<double> Ecm;
    std::vector<double> RawEnergy;
    std::vector<double> ELoss_Al;
    std::vector<double> ELoss_Target;
    std::vector<double> ELoss;
    std::vector<double> ThetaLab;
    std::vector<double> PhiLab;
    std::vector<double> ThetaCM;
    std::vector<bool> PunchThrough;
    std::vector<bool> Layer10;
    std::vector<bool> Layer11;
    std::vector<bool> Layer12;
    std::vector<bool> Layer13;
  
    //For use in simulations
    double OriginalELab;
    double OriginalThetaLab;
    double BeamEnergy;

    NPL::Reaction reaction;
    //NPL::Reaction *reaction;
    //	Energy loss table: the G4Table are generated by the simulation
    NPL::EnergyLoss LightTarget;
    NPL::EnergyLoss LightAl;
    NPL::EnergyLoss LightSi;
    NPL::EnergyLoss BeamTargetELoss;

    double TargetThickness ;
    double WindowsThickness;
    // Beam Energy
    double OriginalBeamEnergy ; // AMEV
    double FinalBeamEnergy; 

    // Beam Position
    double XBeam;
    double YBeam;

    // intermediate variable
    TVector3 BeamDirection;
    TVector3 BeamImpact;
    TRandom3 Rand;
    int Run;
    int DetectorNumber;
    double ThetaNormalTarget;
//    double ThetaM2Surface;
    double ThetaMGSurface;
    double elab_tmp;
    double thetalab_tmp;
    double philab_tmp;
//    double Si_E_M2;
//    double CsI_E_M2;
    double Energy;
#define GATCONF_SIZE 16
    unsigned int GATCONF_Counter[GATCONF_SIZE];
    string GATCONF_Label[GATCONF_SIZE] = { 
      "MUVI1",
      "MUVI2",
      "MUVI3",
      "",
      "CATS2%",
      "VAMOS%",
      "MUST2%",
      "",
      "",
      "VAMOS",
      "ORAGATA", 
    } ;
    
    double MG_T;
    double MG_E;
    int    MG_X;
    int    MG_Y;
    int    MG_D;

    double ThetaGDSurface ;
    std::vector<double> X ;
    std::vector<double> Y ;
    std::vector<double> Z ;
    int AHeavy;
    int ALight;
    double MHeavy;
    double MLight;

    // Vamos Branches
    unsigned long long int LTS;
    unsigned short T_FPMW_CATS1; 
    float T_FPMW_CATS2_C; 
    unsigned short T_FPMW_HF; 
    unsigned short T_CATS2_HF;
    unsigned short T_MUGAST_FPMW;
    unsigned short T_FPMW_CATS2;
    float T_FPMW_HF_C; 
    float IC[12]; 
    unsigned short ICRawPUMult01[7]; 

    float DC0_X;
    float DC0_Y;

    float DC1_X;
    float DC1_Y;

    float DC2_X;
    float DC2_Y;

    float DC3_X;
    float DC3_Y;

    float Xf;
    float Tf;

    float Brho;
    float Theta;
    float Phi;
    float Path;

    unsigned short EWIRE_1_1;
    unsigned short EWIRE_1_2;
    unsigned short EWIRE_2_1;
    unsigned short EWIRE_2_2;

    int MW_Nr;
    float MW_T[1000];
    float MWT[1000];
    unsigned short MW_N[1000];
    unsigned long long int AGAVA_VAMOSTS;

    // Vamos Calculated
    double BrhoRef;
    double mT;    
    double mV;   
    double mD;    
    double mBeta;
    double mGamma;
    double mM_Q;  
    double mM;    
    double mE;    
    double mdE;   

    // Agata branches
    double agata_zShift;
    /*
    unsigned long long int TStrack;
    int nbHits;
    int nbTrack;
    float *trackE  = new float(1000);
    float *trackX1 = new float(1000);
    float *trackY1 = new float(1000);
    float *trackZ1 = new float(1000);
    float *trackT  = new float(1000);
    int *trackCrystalID = new int(1000);
    */
    int nbAdd;
    unsigned long long int TSHit;
    float AddE[1000] ;
    float AddX[1000] ;
    float AddY[1000] ;
    float AddZ[1000] ;

    double EventWeight; //double or uns long long int?

    //
    double dE;
    double dTheta;
    // Branches and detectors
//    TMust2Physics* M2;
    TMugastPhysics* MG;
    GaspardTracker* GP;
    //TCATSPhysics* CATS;
    TModularLeafPhysics* ML;

    bool warning=true;
    

    //Simulation Stuff
    bool isSim;
    bool isPhaseSpace;
    bool excludePoor;
    bool writetoscreen;
    TInitialConditions* Initial;
    TInteractionCoordinates* Interaction;
    TReactionConditions* ReactionConditions;

    double SiThick;
    bool treatPunchThru = true;


    // Beam object
    NPL::Beam* Beam;

    unsigned int GATCONF_MASTER;

    unsigned long long int count ;

};
#endif
