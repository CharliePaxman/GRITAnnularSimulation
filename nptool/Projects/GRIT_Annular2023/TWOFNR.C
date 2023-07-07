////-------------------------
//int    A = 47;
//int    Z = 19;
//double spin_inc = 0.5; //i.e. g.s. of beam particle
//string add = "47Kdp"; string reactionName = "(d,p)"; double QBase =  2.419;
////string add = "47Kdt"; string reactionName = "(d,t)"; double QBase = -2.112;
////string add = "47Kpd"; string reactionName = "(p,d)"; double QBase = -6.145;
////string add = "47Kpt"; string reactionName = "(p,t)"; double QBase = -6.757;
////------------
//int    A = 94;
//int    Z = 36;
//double spin_inc = 0.0; //i.e. g.s. of beam particle
////string add = "94Krdp"; string reactionName = "(d,p)";   double QBase =  0.658;
////string add = "94Krdt"; string reactionName = "(d,t)"; double QBase =  0.974;
////string add = "94Krpd"; string reactionName = "(p,d)"; double QBase = -3.059;
////string add = "94Krpt"; string reactionName = "(p,t)"; double QBase = -0.239;
////------------
//int    A = 132;
//int    Z =  50;
//double spin_inc = 0.0; //i.e. g.s. of beam particle
////string add = "132Sndp"; string reactionName = "(d,p)"; double QBase =  0.174;
////string add = "132Sndt"; string reactionName = "(d,t)"; double QBase = -1.096;
////string add = "132Snpd"; string reactionName = "(p,d)"; double QBase = -5.129;
////string add = "132Snpt"; string reactionName = "(p,t)"; double QBase = -4.075;
////------------
////-------------------------
string mvname    = "~/Programs/nptool/Inputs/CrossSection/GRIT_"+add+"_swave_10MeVu.txt";
string mvnamehvy = "~/Programs/nptool/Inputs/CrossSection/GRIT_"+add+"_swave_10MeVu_heavy.txt";
////-------------------------
int    transN = 3;//0;//1;// 2;//3;
int    transL = 0;
double transJ = 0.5;
//double spin_out = 0; // For (p,t)
double spin_out = transJ; // For all others
double BeamEnergy = 10.0;
double StateE = 0.0;
//-------------------------

////////////////////////////////////////////////////////////////////////////////
void TWOFNR(){
  /* This function moves between directories in order to run TWOFNR in proper *
   * location. This is, weirdly, the least tempremental way of doing this.    */

  cout << "========================================================" << endl;
  
  double Q = QBase - StateE;
  int johnson, tandyval, modelA, modelB;
  string ngrit;
  string ngrithvy = "23.grit";
  if(reactionName=="(d,p)"){
    cout << "Using Johnson-Soper ..."; johnson=5; tandyval=0;
    cout << " ... and Chapel-Hill." << endl; modelA=2; modelB=2;
    ngrit.append("24.grit");
  } else if (reactionName=="(d,t)"){
    johnson=1; //just makess it work below with the dp/dt matching
    ngrit.append("21.grit");
  } else if (reactionName=="(p,d)"){
    johnson=1; //just makess it work below with the dp/dt matching
    ngrit.append("21.grit");
    //ngrit.append("24.grit");
  } else if (reactionName=="(p,t)"){
    johnson=1; //just makess it work below with the dp/dt matching
    ngrit.append("21.grit");
  }
  cout << "!!!! LOOK AT " << ngrit.c_str() << " !!!!" << endl;

  char origDirchar[200];
  getcwd(origDirchar,200);
  string origDir{origDirchar};
  string twofnrDir = "/home/charlottepaxman/Programs/TWOFNR";
  cout << "Current directory    " << origDir << endl;
  cout << "Moving to directory  " << twofnrDir << endl;
  chdir(twofnrDir.c_str());
  //Check
  system("pwd"); 
  cout << "===================================" << endl;

  /* Delete existing tran.grit & 24.grit files */
  remove("tran.grit");
  remove(ngrit.c_str());

  std::ofstream Front_Input("in.front");
  Front_Input << "grit" << std::endl;
  Front_Input << "pipo" << std::endl;
  if(reactionName=="(d,p)"){
    Front_Input << 2 << std::endl;
  } else if (reactionName=="(d,t)"){
    Front_Input << 5 << std::endl;
  } else if (reactionName=="(p,d)"){
    Front_Input << 1 << std::endl;
  } else if (reactionName=="(p,t)"){
    Front_Input << 8 << std::endl;
  }
  Front_Input << 0 << std::endl;
  Front_Input << 0 << std::endl;
  Front_Input << BeamEnergy << std::endl;
  Front_Input << A << " " << Z << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << "0 0 0" << std::endl;
  if(reactionName=="(p,t)"){
    Front_Input << transL << " " << transL << std::endl;
  } else {
    Front_Input << transL << " " << transJ << std::endl;
  }
  Front_Input << transN << std::endl;
  Front_Input << 2 << std::endl;
  Front_Input << Q << std::endl; 
  Front_Input << 1 << std::endl;
  Front_Input << spin_inc << std::endl;
  Front_Input << 1 << std::endl;
  if(reactionName=="(p,d)"||reactionName=="(p,t)"){
  Front_Input << 6 << std::endl;
  } else {
    Front_Input << johnson << std::endl;
    if(johnson==6){//JTandy selected, give version
      Front_Input << tandyval << std::endl;
    }
  }
  Front_Input << 1 << std::endl;
  Front_Input << spin_out << std::endl;
  if(reactionName=="(d,p)"){
    Front_Input << 1 << std::endl;
    Front_Input << modelA << std::endl;
    Front_Input << modelB << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
  } else if (reactionName=="(d,t)"){
    Front_Input << 1 << std::endl;
    Front_Input << 2 << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
  } else if (reactionName=="(p,d)"){
    Front_Input << 1 << std::endl;
    Front_Input << 5 << std::endl;
    Front_Input << 4 << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
  } else if (reactionName=="(p,t)"){
    Front_Input << 1 << std::endl;
    Front_Input << 2 << std::endl;
    Front_Input << 1 << std::endl;
    Front_Input << 1 << std::endl;
  }
  Front_Input << 1.25 << " " << 0.65 << std::endl;
  if (reactionName==!"(p,t)"){
    Front_Input << 6.0 << std::endl;
  }
  Front_Input << 0 << std::endl;
  Front_Input << 0 << std::endl;
  Front_Input.close();

  cout << "Filled front20 input file." << endl;
  cout << "Executing front20..." << endl;
  system("(exec ~/Programs/TWOFNR/front20 < in.front > /dev/null)"); 
    ifstream checkfront("tran.grit");
    if(!checkfront){
      //front20 fail!
      cout << " !! ERROR !! !! ERROR !! !! ERROR !!\n front20 failed to complete" << endl;
      return 0;
    } else {
      cout << "-> tran.grit generated (success not guaranteed)" << endl;
      checkfront.close();
    }

  /* in.twofnrgrit instructs twofnr20 to evaluate tran.grit */
  cout << "Executing twofnr20..." << endl;
  system("(exec ~/Programs/TWOFNR/twofnr20 < in.twofnrgrit > /dev/null)");
    ifstream checktwofnr(ngrit.c_str());
    if(!checktwofnr){
      //twofnr20 fail!
      cout << " !! ERROR !! !! ERROR !! !! ERROR !! \n twofnr20 failed to complete" << endl;
      terminate();
//      return 0;
    } else {
      cout << "-> twofnr20 complete!" << endl;
      checktwofnr.close();
    }

  TGraph* CS = new TGraph(ngrit.c_str());
  CS->Draw();

  string command    = "cp " + ngrit    + " " + mvname;
  system(command.c_str());
  
  string commandhvy = "cp " + ngrithvy + " " + mvnamehvy;
  system(commandhvy.c_str());


  cout << "===================================" << endl;
  cout << "Current directory    " << twofnrDir << endl;
  cout << "Moving to directory  " << origDir << endl;
  chdir(origDir.c_str());
  system("pwd"); 
  cout << "========================================================" << endl;

}

