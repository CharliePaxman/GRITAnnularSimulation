#!/bin/bash

for file in ./94Kr*/*_R2448_Q04*MeV.root
do
  #            string/substring/substitution
  mv "$file" "${file/2448/2699}"
done



#mv $1 $moveme


