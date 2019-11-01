# perflab
## Q1
       The array list implementation's append function calls the
       ensure_space_available function when for every append call.
       ensure_space_available() checks if the current lits is too 
       small to accomodate an addition. If it is, then the list 
       size is multiplied by 2 and a new chunk of memory is 
       allocated for the list using malloc and memcpy. Smaller 
       lists will be too small often and thus need their size 
       doubles with malloc and memcpy, which takes time. Bigger 
       lists will go though this process less because they are 
	   bigger to start off with. This is why we we that smaller 
	   arraylists have higher average clock times than bigger ones.
## Q3
       The optimized realloc() array list decreases the average
       speed by about 5 clocks/elem for all sizes. Using malloc(), 
       memcpy() and free() copies the first half of the list, 
       allocates a size of 2n somewhere else, then moves the first 
       half over. Using realloc() just extends the current list 
       allocation by size n, which is faster and more efficient. 
       The improvement seems to be constant at larger scales.
## Q5
       My code seems to show a 4x improvement in clock speeds after
       making the memmove() optimization. This makes sense because
       before, the manual-shifting shifts every byte indivually. 
       However, the memmove() method shifts the elements by words,
       which are 4 bytes each (which is 4x faster). The insertion time 
       still increases linearly with size, but the multiple is 4 times smaller. 
## Q8
       For both the insert and append, using the small-object pool 
       decreases the average clocks/elem by about 100. The 
       advantage of the small object pool is that memory is 
       already allocated when you want to append/insert an object. 
       This is much faster than the original inplementation, which 
       allocated memory for every new insertion/appending.
       The insert and append graphs seems to be mostly similar, but
       the append graph has more noise at smaller sizes. This 
       could simply be a result of my computer concurrently running
       other processes during a specific test. The reason the 
       insert/append is so similar is that they both simply get 
       memory from the small object pool and edit the pointers of 
       the list. The process is basically the same so the speed 
       graphs will also be the same.

## Q9
       The linked list show a small, sharp increase (in time) right around the 
       smallest sizes, then seems to plateau (with minor increase) until
       size 200000. There, the graph turns concave with a short, steep linear 
       increase in time until around size 400000. There, it turns concave and 
       increases linearly until the max size, but with a very shallow slope that
       decreases as the size increases (it looks like it's approaching a
       plateau). In general, the trand is that performance decreases as size
       increases with the linked list.
## Q10
       Write your answer here.

# Extra Credit
## EC1
       No, there is no difference between the O2 and O3 
       levels of optimization.
## EC2
       The way the algorithm works is that it doubles the size
       of the list when it runs out of space. This size-doubling 
       will happen at powers of 2, so that is why we see spikes in 
       time at powers of 2. 
## EC3
       So many pretty colors wowowowowowow :)
