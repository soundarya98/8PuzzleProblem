# 8PuzzleProblem

This is a fun little project in C++ to implement a depth-limited version of the A star algorithm in order to find the number of steps required to reach the final configuration.

The initial state is a randomly generated reachable state. The 4 things we are most interested in are 

1. Normal Depth(=1), Heuristic: Tile Mismatch

2. User-specified depth, Heuristic: Tile Mismatch

3. Normal Depth(=1), Heuristic: Manhattan Distance

4. User-specified depth, Heuristic: Manhattan Distance

The program checks the running times, maximum number of elements in the fringe, and number of nodes expanded (other than the number of moves to get to the final state). 

![User Interface](https://github.com/soundarya98/8PuzzleProblem/blob/master/8PuzzleInterface.png)

