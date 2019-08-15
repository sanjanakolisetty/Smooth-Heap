# Smooth-Heap
Implemented the Smooth Heap data structure for EECS 477 (Introduction to Algorithms) which was released in 2018 in a research paper (https://arxiv.org/abs/1802.05471). Analyzed variations in time complexity when using a Smooth Heap, Fibonacci Heap, Pairing Heap, and Binary Heap when implementing "Extract-Min" (i.e. finding and removing the smallest key, or the "closest node") in Dijkstra's algorithm on road networks. Many thanks to Linh Le and Premnath Shenoy (collaborators) and Seth Pettie (professor).

From the paper on Smooth Heaps:

We obtain a new, simple and efficient heap algorithm, which we call the smooth heap. We show the smooth heap to be the heap-counterpart of Greedy, the BST algorithm with the strongest proven and conjectured properties from the literature, widely believed to be instance-optimal. Assuming the optimality of Greedy, the smooth heap is also optimal within our model of heap algorithms. As corollaries of results known for Greedy, we obtain instance-specific upper bounds for the smooth heap, with applications in adaptive sorting. 

Intriguingly, the smooth heap, although derived from a non-practical BST algorithm, stores no auxiliary data besides the keys and tree pointers. It can be seen as a variation on the popular pairing heap data structure, extending it with a "power-of-two-choices" type of heuristic.
