set terminal png size 800,600
set output 'images/perf-al-insert.png'

set title "Insert at 0:  Array List Variants"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Inserted"
set yrange [0 : 1000]

plot "data/insert-al-initial.txt" skip 1 using 1:3 with lines \
         title "Array List (initial)", \
     "data/insert-al-insertopt.txt" skip 1 using 1:3 with lines \
         title "Array List (ins-opt)", \
     "data/insert-al-resizeopt.txt" skip 1 using 1:3 with lines \
         title "Array List (res-opt)", \
     "data/insert-al-insertopt-resizeopt.txt" skip 1 using 1:3 with lines \
         title "Array List (ins/res-opt)", \
     "data/insert-al-fastest.txt" skip 1 using 1:3 with lines \
         title "Array List (-O3/opt)"
