CONTRIBUTIONS

TODO: write a brief summary of how each team member contributed to
the project.

REPORT

TODO: add your report according to the instructions in the
"Experiments and analysis" section of the assignment description.

Wrote 16777216 bytes to '/tmp/mkim225/data_16M.in'
In this experiment, it seems like you can't have too much or too little for the threshold amount. If the threshold is high, it will take the longest time. 
However, the threshold that is between the highest and the lowest would take the least amount of time. It is possible having a low threshold would have too much
fork processing the mergesort, making a bunch of fork executing mergesort instead of making it quicker by doing qsort for a smaller array. Having a high threshold 
would make the qsort execute early. Since qsort can sometimes be worse than mergesort, it is possible for the qsort to be quadratic time, which is worst time case.

Test run with threshold 2097152

real    0m0.380s
user    0m0.369s
sys     0m0.008s
Test run with threshold 1048576

real    0m0.224s
user    0m0.388s
sys     0m0.020s
Test run with threshold 524288

real    0m0.151s
user    0m0.415s
sys     0m0.029s
Test run with threshold 262144

real    0m0.137s
user    0m0.522s
sys     0m0.069s
Test run with threshold 131072

real    0m0.132s
user    0m0.524s
sys     0m0.068s
Test run with threshold 65536

real    0m0.136s
user    0m0.541s
sys     0m0.084s
Test run with threshold 32768

real    0m0.148s
user    0m0.547s
sys     0m0.126s
Test run with threshold 16384

real    0m0.147s
user    0m0.561s
sys     0m0.168s
