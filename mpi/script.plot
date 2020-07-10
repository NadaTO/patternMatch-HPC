set terminal png
set ylabel "speedup"
set xlabel "number of threads"
set xtics 1
set title "APM Speedup for MPI execution"
set output "Sortie.png"

plot x title 'Speedup max' with lines, \
     'entrée.data' using 1:($2/$3) title 'APM' with linespoints;
