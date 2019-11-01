set terminal png size 800,600
set output 'images/perf-append.png'

set title "Append:  Array List vs. Linked List"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Appended"
set yrange [0 : 400]

plot "data/append-al-initial.txt" skip 1 using 1:3 with lines \
         title "Array List (initial)", \
     "data/append-al-insertopt-resizeopt.txt" skip 1 using 1:3 with lines \
         title "Array List (ins/res-opt)", \
     "data/append-al-fastest.txt" skip 1 using 1:3 with lines \
         title "Array List (-O3/opt)", \
     "data/append-ll-initial.txt" skip 1 using 1:3 with lines \
         title "Linked List (initial)", \
     "data/append-ll-nodepool.txt" skip 1 using 1:3 with lines \
         title "Linked List (nodepool)", \
     "data/append-ll-fastest.txt" skip 1 using 1:3 with lines \
         title "Linked List (-O3/opt)"
