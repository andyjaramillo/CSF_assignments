CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.

Andreas and Owen pair programmed to complete the functionality of the program. Andreas wrote the
analysis of the README and Owen reviewed and revised it accordingly. 

REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.

Test run with threshold 2097152

real    0m0.446s
user    0m0.418s
sys     0m0.021s
Test run with threshold 1048576

real    0m0.310s
user    0m0.537s
sys     0m0.031s
Test run with threshold 524288

real    0m0.265s
user    0m0.696s
sys     0m0.060s
Test run with threshold 262144

real    0m0.256s
user    0m1.118s
sys     0m0.050s
Test run with threshold 131072

real    0m0.244s
user    0m0.961s
sys     0m0.122s
Test run with threshold 65536

real    0m0.264s
user    0m1.119s
sys     0m0.181s
Test run with threshold 32768

real    0m0.239s
user    0m1.072s
sys     0m0.134s
Test run with threshold 16384

real    0m0.294s
user    0m1.283s
sys     0m0.190s

The threshold starts at 2097152 and goes to 16384, so through the tests the threshold is decreasing. From the
tests it can be seen then in each call the real-time continues to decrease. This is because the threshold determines
the number of child processes that are created during the execution of the program. As the threshold decreases, the if statement
in the code that says:
        if(num_elements <= threshold){
            qsort(arr+begin, num_elements, sizeof(int64_t),compare_i64);
            return;
        }
will create more child processes as the threshold decreases. Since there are more child processes, there will be more parallelism
done in the same time and therefore quicker sorting.

One last result from this is that the time increases at the end as seen below:
        Test run with threshold 32768

        real    0m0.239s
        user    0m1.072s
        sys     0m0.134s
        Test run with threshold 16384

        real    0m0.294s
        user    0m1.283s
        sys     0m0.190s
The last one although should be faster, it has more child processes than the previous tests, so it has an adverse effect since it has to wait for all 
the processes to finish before merging in the parent. From this, we can see a bell curve form of return on parallelism. At some point of a threshold
of roughly 32768 we get the quickest sorting from parallelism, and any more after that will only take longer since the return from more parallel processes does not
outweigh having to weigh the fact that they must all finish before merging.