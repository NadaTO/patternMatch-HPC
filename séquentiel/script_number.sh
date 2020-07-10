#!/bin/bash

replication=2
data=../dna/upstream2000.fa
pattern=("TTCG" "TAAA" "TGGA" "TTAA" "TGTG" "AAAA" "TTTG" "ATTC" "AGGC")
nb=${#pattern[@]}

for l in $( seq $nb ); do
    patternl=${pattern[@]:nb-l} 
    log_file=logs/apm_${replication}_length.log
    ./apm  $replication  $data  $patternl > $log_file

    time=$(grep "APM done in" $log_file|awk '{print $4}')
	if [ "$l" -eq  1 ]; then
		tseq=$time
	fi
    echo -e "$l \t $tseq \t $time"
done
