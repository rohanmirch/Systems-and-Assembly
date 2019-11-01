set terminal png size 800,600
set output 'images/perf-ll-insert.png'

set title "Insert at 0:  Linked List Variants"
set xlabel "Total Collection Size"
set ylabel "Average Clocks Per Element Inserted"
set yrange [0 : 400]

plot "data/insert-ll-initial.txt" skip 1 using 1:3 with lines \
         title "Linked List (initial)", \
     "data/insert-ll-nodepool.txt" skip 1 using 1:3 with lines \
         title "Linked List (nodepool)", \
     "data/insert-ll-nodepool-badcache.txt" skip 1 using 1:3 with lines \
         title "Linked List (fat-nodepool)", \
     "data/insert-ll-fastest.txt" skip 1 using 1:3 with lines \
         title "Linked List (-O3/opt)"
