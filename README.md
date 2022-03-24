# Baby-step-giant-step-memory-optimisation
This is a small project where I will implement the Baby-step giant-step algorithm. It is used for solving the discrete log problem. In my implementation, I will try to minimse the memory usage by implementing a hashtable o intervals.

## Description
This project has the purpose of analyzing the algorithm and proposing variations of it, by having space-time tradeoffs.
The steps of making this project will be:
- Finding an optimal datastructure for the initial algorithm
- Implementing the initial version in C
- Finding a new data structure that uses less memory than the first version
- Implmeneting one or more optimised versions in C
- Comparing the different implementations by doing benchmarks
- Drawing conclusions

## 1. Baby-step giant-step algorithm

The algorithm is used to solve the discrete log problem. It uses the meet-in-the-middle approach. Which is: if we want to find a collision between 2 sets, we compute the first set, store it in a HashMap, then iteratively look in the second set for entries in the HashMap. See more details on the [Wikipedia page](https://en.wikipedia.org/wiki/Baby-step_giant-step).

##. 2. Reasoning

The HashMap can be implemented with a [Hash table](https://en.wikipedia.org/wiki/Hash_table), which is basically an array of buckets. There are actually a lot of ways to implement the Hash table. The one I will be reffering to is the Open addressing one. It stores one element per bucket and it can be memory efficient.

Although we use a Hashtable that is memory efficient, the space complexity of Baby-step giant-step is O(\sqrt(n)). I think we can factor ths down by constant and keep the same time complexity.
