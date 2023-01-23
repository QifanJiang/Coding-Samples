HOMEWORK 8: ROPES


NAME:  Qifan Jiang


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

no collaborators

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT: 10 hours

BALANCED/WORST CASE RUNNIG TIME ANALYSIS:
n = characters in the string of the current Rope (*this)
m = characters in the string of the Rope passed in (for functions that take
    a second rope)
v = longest "value" of a leaf
p = longest path from root to a leaf
Using these variables, state and explain the big-O running time of each 
of the following operations two ways: 1) with a balanced Rope, 2) with an 
extremely unbalanced Rope. Explain what you considered an extremely 
unbalanced rope for the string "Hello my name is Simon".

Copy Constructor:
1.O[log(p)]
2.O(p)
Construct from Node*:
1.O[log(p)]
2.
Index:
1.O[log(p)]
2.O(p)
Report:
1.O[log(p)+n]
2.O(p)
iterator operator++:
1.O[log(p)]
2.O(p)
Split:
1.O[log(p)*p]
2.O[p^2]
Concat:
1.O[log(p)+m]
2.O[p+m]

TESTING & DEBUGGING STRATEGY: 
Briefly describe the tests in your StudentTests() function.
How did you test the "corner cases" of your implementation?
I test several corner cases included the leftest and rightest node's condition.
Especially for the split function.


MISC. COMMENTS TO GRADER:  
