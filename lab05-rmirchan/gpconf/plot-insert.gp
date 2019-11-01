set terminal png size 800,600
set output 'images/perf-insert.png'

set title "Insert at 0:  Array List vs. Linked List"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Inserted"
set yrange [0 : 1000]

plot "data/insert-al-initial.txt" skip 1 using 1:3 with lines \
         title "Array List (initial)", \
     "data/insert-ll-initial.txt" skip 1 using 1:3 with lines \
         title "Linked List (initial)", \
     "data/insert-al-insertopt-resizeopt.txt" skip 1 using 1:3 with lines \
         title "Array List (ins/res-opt)", \
     "data/insert-al-fastest.txt" skip 1 using 1:3 with lines \
         title "Array List (-O3/opt)", \
     "data/insert-ll-nodepool.txt" skip 1 using 1:3 with lines \
         title "Linked List (nodepool)", \
     "data/insert-ll-fastest.txt" skip 1 using 1:3 with lines \
         title "Linked List (-O3/opt)"
