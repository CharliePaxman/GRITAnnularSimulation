#!/bin/bash

#==================================================================
#==================================================================

clear
cd ~/Programs/nptool/Projects/GRIT_Annular2023;
cmake ./;
make -j6;

###==================================================================
###==================================================================

sect=32
thick=1500 #NOW APPLIES ONLY TO THE THICK DETECTOR(S)!!!!
MeVu=10

#for targT in 1 2 4 6 8
##for targT in 8
##for targT in 1
#do
#for ring in 16 32 64 96 128
##for ring in 16
#do
#
#  beamE=$((MeVu*47))
##  ./sim.sh 47K 2H 1H 48K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 47K 2H 3H 46K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
#  ./sim.sh 47K 1H 2H 46K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 27 multiDet
#  ./sim.sh 47K 1H 3H 45K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 27 multiDet
#  
#  beamE=$((MeVu*94))
##  ./sim.sh 94Kr 2H 1H 95Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 94Kr 2H 3H 93Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 94Kr 1H 2H 93Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 94Kr 1H 3H 92Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  #####./sim.sh 94Kr 1H 3H 92Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 40 multiDet
# 
#  beamE=$((MeVu*132))
##  ./sim.sh 132Sn 2H 1H 133Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 132Sn 2H 3H 131Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 132Sn 1H 2H 131Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
##  ./sim.sh 132Sn 1H 3H 130Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 20 multiDet
#
#done
#done

for targT in 1 2 4 6 8
do
for ring in 16 32 64 96 128
do
  beamE=$((MeVu*94))
#  ./sim.sh 94Kr 1H 2H 93Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 27 multiDet
  ./sim.sh 94Kr 1H 3H 92Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 50 multiDet
done
done

#for targT in 1 2 4 6 8
##for targT in 2 4 6 8
#do
#for ring in 16 32 64 96 128
#do
#  beamE=$((MeVu*132))
#  ./sim.sh 132Sn 1H 2H 131Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 27 multiDet
#  ./sim.sh 132Sn 1H 3H 130Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 27 multiDet
#done
#done

























##==================================================================
##==================================================================
#
#ring=16
#sect=16
#targT=2
#MeVu=10
#
#for thick in 20000 500 1000 1500 2000 2500 3000 3500 4000 4500
#do
#
#  beamE=$((MeVu*47))
#  ./sim.sh 47K 2H 1H 48K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 47K 2H 3H 46K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 47K 1H 2H 46K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 47K 1H 3H 45K     $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#done
#
#
#for thick in 20000 500 1000 1500 2000 2500 3000 3500 4000 4500
#do
#  beamE=$((MeVu*94))
#  ./sim.sh 94Kr 2H 1H 95Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 94Kr 2H 3H 93Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 94Kr 1H 2H 93Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 94Kr 1H 3H 92Kr   $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#done
#
#for thick in 20000 500 1000 1500 2000 2500 3000 3500 4000 4500
#do
#  beamE=$((MeVu*132))
#  ./sim.sh 132Sn 2H 1H 133Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 132Sn 2H 3H 131Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 132Sn 1H 2H 131Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#  ./sim.sh 132Sn 1H 3H 130Sn $beamE $thick $ring $sect $targT TreatPunchThru swave_10MeVu_Full 5 oneDet
#done
#
#==================================================================
#==================================================================
