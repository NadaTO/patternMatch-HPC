#!/bin/bash

replication=2
data=../../dna/small_chrY.fa
pattern="TTCG"
echo "" > entrée.data
for th in $( seq 32 ); do
	log_file=logs/apm_${replication}_${th}.log
	OMP_NUM_THREADS=$th ./data_split  $replication  $data  $pattern > $log_file

  time=$(grep "APM done in" $log_file|awk '{print $4}')
	if [ "$th" -eq 1 ]; then
		tseq=$time
	fi
	echo -e "$th \t $tseq \t $time" >> entrée.data
done
