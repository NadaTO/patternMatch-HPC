#!/bin/bash

replication=2
data=(../dna/small_chrY.fa ../dna/upstream2000.fa ../dna/mrna.fa ) 
pattern="TTCG"

for length in $( seq 3 ); do
    FILENAME=${data[$length-1]}
    size=$(stat -c%s "$FILENAME")
    log_file=logs/apm_${replication}_size.log
    ./apm  $replication  ${data[length-1]}  $pattern > $log_file

     time=$(grep "APM done in" $log_file|awk '{print $4}')
     echo -e "$size \t $time"
done

