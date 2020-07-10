set terminal png
set ylabel "time execution (seconds)"
set xlabel "length of pattern "
set xtics 2
set title "APM execution time according to pattern length"
set output "Sortie_length.png"

plot 'entrée_length.data' using 1:($3) title 'APM' with linespoints;
