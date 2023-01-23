HOMEWORK 6: RICOCHET ROBOTS RECURSION


NAME:  Qifan Jiang



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

No collaborators, just learned some knowledge on the website.

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  50



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

i & j = dimensions of the board
    r = number of robots on the board
    g = number of goals
    w = number of interior walls
    m = maximum total number of moves allowed

There are two main function operated in the program --- dfs() and traverse()[bfs]
traverse() is used to find the shortest goal,
dfs() is used to find all solutions.

If m exists:
The complexity of traverse() is O(4*r^m)
The complexity of dfs() is O(4*r^m)

If m doesn't exist:
The complexity of traverse() is O(i*j)
The complexity of dfs() is O((i*j)^r)



SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES:
Correctness & approximate wall clock running time for various command
line arguments.

time ./a.out puzzle1.txt -max_moves 3 -visualize A
real    0m0.017s
user    0m0.016s
sys     0m0.016s

time ./a.out puzzle1.txt -max_moves 10 -visualize A
real    0m0.134s
user    0m0.125s
sys     0m0.016s

time ./a.out puzzle1.txt -max_moves 8
real    0m0.105s
user    0m0.016s
sys     0m0.000s

time ./a.out puzzle1.txt -max_moves 8 -all_solutions
real    0m0.187s
user    0m0.031s
sys     0m0.000s

time ./a.out puzzle2.txt -max_moves 4
real    0m0.030s
user    0m0.016s
sys     0m0.000s
--------------------------------------------------------------------
time ./a.out puzzle2.txt -max_moves 8 -all_solutions
real    0m0.094s
user    0m0.031s
sys     0m0.000s

time ./a.out puzzle3.txt -max_moves 10 -all_solutions
real    0m0.129s
user    0m0.109s
sys     0m0.000s

time ./a.out puzzle4.txt -max_moves 10 -all_solutions
real    0m0.048s
user    0m0.000s
sys     0m0.016s

time ./a.out puzzle5.txt -max_moves 10 -all_solutions
real    0m0.014s
user    0m0.000s
sys     0m0.016s

time ./a.out puzzle8.txt -max_moves 10 -all_solutions
    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16
  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
  |                   |                             |                             |
 1|                   |                             |                             |
  |                   |                             |                             |
  +    +    +    +    +    +    +    +    +    +    +    +    +    +    +    +    +
  |                                                                     |         |
 2|                                                                     |         |
  |                                                                     |         |
  +    +    +    +    +    +    +    +    +    +    +    +    +    +----+    +----+
  |                             |                                                 |
 3|                             |                                                 |
  |                             |                                                 |
  +    +    +    +    +    +----+    +    +    +----+    +    +    +    +    +    +
  |                                            |                                  |
 4|                                            | B                                |
  |                                            |                                  |
  +    +    +----+    +    +    +    +    +    +    +    +    +    +    +----+    +
  |              |                                                           |    |
 5|           G  |                                                           |    |
  |              |                                                           |    |
  +----+    +    +    +    +    +    +    +    +    +    +    +    +    +    +    +
  |                                  |                                            |
 6|                                  |                                            |
  |                                  |                                            |
  +    +----+    +    +    +    +    +----+    +    +    +    +    +    +    +    +
  |    |                                                      |                   |
 7|    |                                                      | Y                 |
  |    |                                                      |                   |
  +    +    +    +    +    +    +    +----+----+    +    +    +----+    +    +    +
  |                                  |         |                                  |
 8|                                  |         |                                  |
  |                                  |         |                                  |
  +    +    +    +    +    +----+    +    +    +    +    +    +    +    +    +    +
  |                             |    |         |                                  |
 9|                           ? |    |         |                                  |
  |                             |    |         |                                  |
  +    +    +    +    +    +    +    +----+----+    +    +    +    +    +    +    +
  |         |                                                      |              |
10|         |                                                      |              |
  |         |                                                      |              |
  +    +----+    +    +    +    +    +    +    +    +    +    +    +----+    +    +
  |                   |                                                           |
11|                   |                                                           |
  |                   |                                                           |
  +    +    +    +    +----+    +    +    +    +    +    +    +    +    +    +    +
  |                                                 |                             |
12|                                                 |                             |
  |                                                 |                             |
  +----+    +    +    +    +    +    +    +    +----+    +    +    +    +    +----+
  |                                                                               |
13|                                                                               |
  |                                                                               |
  +    +    +    +    +    +----+    +    +    +    +    +    +    +    +----+    +
  |                             |                                            |    |
14|                             |                                         R  |    |
  |                             |                                            |    |
  +    +    +    +----+    +    +    +    +    +    +----+    +    +    +    +    +
  |              |                                  |                             |
15|              | S                                |                             |
  |              |                                  |                             |
  +    +    +    +    +    +    +    +    +    +    +    +    +    +    +    +    +
  |                                  |                        |                   |
16|                                  |                        |                   |
  |                                  |                        |                   |
  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
1 different 7 move solutions:

robot S moves east
robot S moves south
robot S moves west
robot S moves north
robot S moves west
robot G moves south
robot G moves east
All goals are satisfied after 7 moves

real    0m2.693s!!!
user    0m2.656s
sys     0m0.031s

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

Hopefully you would be happy after reading my code :)
