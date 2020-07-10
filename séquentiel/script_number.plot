set terminal png
set ylabel "time execution (seconds)"
set xlabel "number of patterns "
set xtics 2
set title "APM execution time according to pattern number with a fixed number of threads 16"
set output "Sortie_number.png"

plot 'entrée_number.data' using 1:($3) title 'APM' with linespoints;
