Andreas Jaramillo
Owen Jeffrey Bianchi

Work Participation:
    Andreas: Wrote isLoadHitorMiss, isStoreHitorMiss, while loop structure in main.cpp and counters for all including total cycles, and added Input validation.

    Owen: Supported with all functions, designed structure for isLoadHitorMiss and isStoreHitorMiss, supported
    development of cycle counters, and wrote best cache configuration with all tests.



In order to report a “best” cache configuration, our group ran a bash script to run and report the statistics of several 
different possible caches on a realistic trace, gcc trace.  In order to reduce the number of configurations to a manageable 
number, we made it so that each of the caches had a total data capacity of around 1 MB.  We had one set of experiments with 
a “small” block size of 16 bytes and one set of experiments with a “large” block size of 256 bytes.  For each block size, we 
iterated through all possible combinations of the number of sets and the number of blocks such that the total data capacity 
was approximately 1 MB.  This went from values as small as 1 to as large as 65536 for each.  For each cache configuration, we 
tested all three write parameter combinations: write-allocate & write-through, write-allocate & write-back, and no-write-allocate 
and write-back.  For each of those, we also tested both fifo and lru eviction methods.  The bash file we created looked like this:

#!/usr/bin/env bash

for nsets in 1 2 4 8 16 32 64 128 256 512 1024 2048 4096
do
    for nblocks in 4096 2048 1024 512 256 128 64 32 16 8 4 2 1 
    do 
        for nbytes in 256
        do
            prod=$((nsets * nblocks))
            if [ $prod -eq 4096 ]
            then
                for evict in lru fifo
                do  
                    printf "$nsets $nblocks $nbytes write-allocate write-through $evict < gcc.trace" >> out.txt
                    printf "\n" >> out.txt
                    ./csim $nsets $nblocks $nbytes write-allocate write-through $evict < gcc.trace >> out.txt
                    printf "\n" >> out.txt

                    printf "$nsets $nblocks $nbytes write-allocate write-back $evict < gcc.trace" >> out.txt
                    printf "\n" >> out.txt
                    ./csim $nsets $nblocks $nbytes write-allocate write-back $evict < gcc.trace >> out.txt
                    printf "\n" >> out.txt

                    printf "$nsets $nblocks $nbytes no-write-allocate write-through $evict < gcc.trace" >> out.txt
                    printf "\n" >> out.txt
                    ./csim $nsets $nblocks $nbytes no-write-allocate write-through $evict < gcc.trace >> out.txt
                    printf "\n" >> out.txt
                done
            fi
        done
    done
done

We used python in order to find the lowest total cycle count and associated command line argument:

file = open("out.txt")
lines = file.readlines()

dict = {}
for i in range(0, len(lines)-8, 9):
  com = lines[i]
  cyc = lines[i+7]
  dict[int(cyc[14:])] = com
cycles = list(dict.keys())
commands = list(dict.values())

print(min(cycles))
print(dict[min(cycles)])

This gave us a large list of performances of different caches.  From this list, we found that the smallest cycle count and thus the 
most optimal cache configuration was when we ran “2048 32 16 write-allocate write-back fifo < gcc.trace”.  2048 sets, 32 blocks in 
each set, 16 bytes per block, write-allocate & write-back using the fifo eviction method.