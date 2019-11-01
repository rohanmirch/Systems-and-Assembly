set terminal png size 800,600
set output 'images/perf-iter.png'

set title "Iterate over Elements:  Array List vs. Linked List"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Visited"
set yrange [0 : 350]

plot "data/iter-al-initial.txt" skip 1 using 1:3 with lines \
         title "Array List (initial)", \
     "data/iter-al-fastest.txt" skip 1 using 1:3 with lines \
         title "Array List (-O3/opt)", \
     "data/iter-ll-initial.txt" skip 1 using 1:3 with lines \
         title "Linked List (initial)", \
     "data/iter-ll-nodepool.txt" skip 1 using 1:3 with lines \
         title "Linked List (nodepool)", \
     "data/iter-ll-fastest.txt" skip 1 using 1:3 with lines \
         title "Linked list (-O3/opt)"

