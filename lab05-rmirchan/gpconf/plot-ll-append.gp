set terminal png size 800,600
set output 'images/perf-ll-append.png'

set title "Append:  Linked List Variants"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Appended"
set yrange [0 : 400]

plot "data/append-ll-initial.txt" skip 1 using 1:3 with lines \
         title "Linked List (initial)", \
     "data/append-ll-nodepool.txt" skip 1 using 1:3 with lines \
         title "Linked List (nodepool)", \
     "data/append-ll-nodepool-badcache.txt" skip 1 using 1:3 with lines \
         title "Linked List (fat-nodepool)", \
     "data/append-ll-fastest.txt" skip 1 using 1:3 with lines \
         title "Linked List (-O3/opt)"
