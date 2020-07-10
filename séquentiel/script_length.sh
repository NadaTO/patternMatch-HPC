#!/bin/bash

replication=2
data=../dna/upstream2000.fa
pattern="TTCG"

for length in $( seq 32 ); do
    if [ $(($length % 3)) == 0 ]
  then
       var="A"
    elif [ $(($length % 3)) == 0 ]
    then
	 var="T"
    else var="G"
    fi
    pattern=$pattern$var
    log_file=logs/apm_${replication}_length.log
    ./apm  $replication  $data  $pattern > $log_file

  time=$(grep "APM done in" $log_file|awk '{print $4}')
	if [ "$length" -eq 1 ]; then
		tseq=$time
	fi
	echo -e "$length \t $tseq \t $time"
done
