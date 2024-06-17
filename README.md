# DNA-Sequence-Alignment

The Sequence Alignment problem is one of the fundamental problems of Biological Sciences, aimed at finding the similarity of two amino-acid sequences. Comparing amino-acids is of prime importance to humans, since it gives vital information on evolution and development.

In this project we implement two sequence alignment algorithms.

basic.cpp is pure dynamic programming approach.
efficient.cpp is memory efficient approach which combines Divide and conquer and dynamic programming.

1. Basic
For the fundamental approach, memory grows polynomially in proportion to the problem
size, which is O(nm), where m is the first word's size and n is the second word's size and DP
table having size of m rows and n cols.

2. Efficient
In efficient approach, memory grows O(2 * m) linearly and it includes the stack used for
recursion and dp memory. A DP table has m rows and 2 cols.
Therefore, the basic approach consumes more memory, O(m*n), whereas the efficient
solution consumes less memory, O(2*m), and grows linearly.


Even though the basic and efficient approaches have the same time complexity i.e.
O(m * n), where m is the size of the first word and n is the size of the second word, the
efficient approach takes a little longer than the basic approach because it requires more
computations, making it slightly less efficient in terms of time

![image](https://github.com/smdp2000/DNA-Sequence-Alignment/assets/40947644/5116b242-18b2-4b0e-b127-be812d8a7320)

![image](https://github.com/smdp2000/DNA-Sequence-Alignment/assets/40947644/67fd29e0-a240-4a9c-8caa-8fdd2da57f51)

![image](https://github.com/smdp2000/DNA-Sequence-Alignment/assets/40947644/20e467c4-7c50-47ff-b47f-07d2ae8bec4a)


