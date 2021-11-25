#!/bin/bash

timelimit=300


for file in ../instances/*
do
	name=$(echo $file | cut -d/ -f3)
	for solver in c_c_basic #c_c_single #c_basic 
	do
		echo $solver solving $name

		timeout $timelimit ./picat $solver $file > tmp

		#SOC=$(grep Cost tmp | cut -d" " -f2)
		el_time=$(grep CPU tmp | cut -d" " -f3)
		#SOCLB=$(grep SocLB tmp | cut -d" " -f2)
		mks_LB=$(grep LB tmp | cut -d" " -f3)
		mks_final=$(grep timesteps tmp | cut -d" " -f2)

		#echo $name'_'$solver'_'$SOCLB'_'$SOC'_'$el_time >> results_$solver.txt
		echo $name'_'$solver'_'$mks_LB'_'$mks_final'_'$el_time >> ../results.txt
		#rm -f tmp

		echo "done"
	done
	mv ../instances/$name ../solved/$name
done
