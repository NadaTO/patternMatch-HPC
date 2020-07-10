set terminal png
set ylabel "time execution (seconds)"
set xlabel "size of file "
set xtics 40000000
set title "APM execution time according to file size"
set output "Sortie_file_size.png"

plot 'entrée_file_size.data' using 1:($2) title 'APM' with linespoints;
