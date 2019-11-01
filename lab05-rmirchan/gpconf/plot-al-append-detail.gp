set terminal png size 800,600
set output 'images/perf-al-append-detail.png'

set title "Append:  Array List Variants"
set xlabel "Total Collection Size"
set xrange [0 : 1100]
set ylabel "Average Clocks Per Element Appended"
set yrange [0 : 100]

plot "data/append-al-initial.txt" skip 1 using 1:3 with lines \
         title "Array List (initial)", \
     "data/append-al-insertopt-resizeopt.txt" skip 1 using 1:3 with lines \
         title "Array List (ins/res-opt)", \
     "data/append-al-fastest.txt" skip 1 using 1:3 with lines \
         title "Array List (-O3/opt)"
