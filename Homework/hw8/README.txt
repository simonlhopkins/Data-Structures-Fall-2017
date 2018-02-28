HOMEWORK 8: QUAD TREES AND TREE ITERATION


NAME:  Simon Hopkins


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Sinclair, Evan, ALAC, cplusplus.com, stackexchange

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  16h



ORDER NOTATION ANALYSIS: 
Give the big O notation of each of the QuadTree operations and justify
your answer for the non trivial operations (please be concise!)
Analyze both the running time and the additional memory usage needed
(beyond the space allocated for the existing tree structure).  You may
assume that the tree is reasonably well balanced for all operations.


n = the number of elements in the tree


size()
  running time: O(1)
  memory usage: O(1)

insert()
  running time: O(log (base 4) n)
  memory usage: O(1)

find()
  running time: O(log (base 4) n)
  memory usage: O(1)

height()
  running time: O(1)
  memory usage: O(1)

begin()
  running time: O(1)
  memory usage: O(1)

end()
  running time: O(1)
  memory usage: O(1)

bf_begin()
  running time: O(1)
  memory usage: O(1)

bf_end()
  running time: O(1)
  memory usage: O(1)

operator++()
  running time: O(log (base 4) n)
  memory usage: O(1)

operator*()
  running time: O(1)
  memory usage: O(1)

getLabel()
  running time: O(1)
  memory usage: O(1)

getDepth()
  running time: O(1)
  memory usage: O(1)

copy constructor
  running time: O(n)
  memory usage: O(n)

assignment operator
  running time: O(n)
  memory usage: O(n)

destructor
  running time: O(n)
  memory usage: O(1)



EXTRA CREDIT:  TREE BALANCING
How does the point insertion order affect the shape of the resulting
QuadTree object? What are specific examples of the worst case and best
case?  Discuss your stratgy for reordering the input to rebalance the
tree.  Paste example output with and without your rebalancing.






MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


