#!/bin/bash

for map in random-8-8-20.map #empty-8-8.map
do
	for ag_per_t in 5 10
	do
		max=$((40/$ag_per_t))
		for teams in `seq 1 $max`
		do
			echo "generator -t $teams -a $ag_per_t -m $map -x 5 -s 10"
			#./generator -t $teams -a $ag_per_t -m $map -x 5 -s 17
			# empty -t 10 -a 5 -s 21
			# empty -t 5 -a 10 -s 23
			timeout 2 ./generator -t $teams -a $ag_per_t -m $map -x 5 -s 31
		done
	done
done

return

for map in empty-16-16.map empty-32-32.map random-16-16-20.map random-32-32-20.map
do
	for ag_per_t in 5 10
	do
		max=$((100/$ag_per_t))
		for teams in `seq 1 $max`
		do
			echo "generator -t $teams -a $ag_per_t -m $map -x 5 -s 10"
			./generator -t $teams -a $ag_per_t -m $map -x 5 -s 10
		done
	done
done
