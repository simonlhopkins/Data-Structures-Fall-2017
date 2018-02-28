HOMEWORK 6: HASHI RECURSION


NAME:  Simon Hopkins



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cplusplus.com, stackexchange.com

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  16h



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)
The dimensions of the board (w and h) 
The number of nodes/islands (n)? 
The number of edges/bridges (e)?
Etc. 

Since I make my recursive function based on all of the possible bridges, the most expensive operation
will be based on e. Each recursive iteration, I check the graph at it's current state, and then return
a list of possible bridges that could exist in the graph's state. I then loop through and add each bridge that could exsist. Since every time I add a bridge, there will be one less bridge that I need to check against. This means that the order notation of of my program will be e!, where e is the maximum number of edges you could have.



SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES:
# of solutions & approximate wall clock running time for different
puzzles for a single solution or all solutions.  Also, describe the
image in words.

Right now, the running time is the same for single solution and all solutions. I could ask the recursive function to return after it finds one solution, but if the connected flag is there and the first connection it finds is not conneted, then you will get the wrong solution. My program runs well for low numbered puzzles, in where there are a low number of nodes, and consequently, a low number of possible edges. However, once the numbers increase, my program times out, but not before printing out when it finds a solution. This means that it is working and with enough time, it will return the right connections.

Puzzle 1:
Solutions: 1
approximate wall clock running time: 0.010s
4===3 
#   | 
#   | 
#   | 
#   1 
#     
2
Puzzle 2:
Solutions: 1
approximate wall clock running time: 0.438s

     2     
     #     
     #     
     #     
 2===8===2 
     #     
     #     
     #     
     2

Puzzle 3:
Solutions: 4
approximate wall clock running time: 0.036s
 1-------1 
           
           
           
 2=======2 
           
           
           
 1-------1 
           
**********
           
 1       1 
 |       | 
 |       | 
 |       | 
 2-------2 
           
           
           
 1-------1 
           
**********
           
 1-------1 
           
           
           
 2-------2 
 |       | 
 |       | 
 |       | 
 1       1 
           
**********
           
 1       1 
 |       | 
 |       | 
 |       | 
 2       2 
 |       | 
 |       | 
 |       | 
 1       1 

Puzzle 4:
Solutions: 1
approximate wall clock running time: 0.132s

     2     
     #     
 1   #   1 
 |   #   | 
 |   #   | 
 |   #   | 
 2   #   2 
 |   #   | 
 |   #   | 
 |   #   | 
 1   #   1 
     #     
     2    
Puzzle 5:
Solutions: 1
approximate wall clock running time: ?

Puzzle 6:
Solutions: 1
approximate wall clock running time: ?

Puzzle 7:
Solutions: 5
approximate wall clock running time: 0.631s

************
           
 1   3---2 
 |   #   | 
 |   #   | 
 |   #   | 
 |   #   | 
 |   #   | 
 1   3---2 
           
************
           
 1   3===2 
 |   |     
 |   |     
 |   |     
 |   |     
 |   |     
 1   3===2 
           
************
           
 1---3   2 
     #   # 
     #   # 
     #   # 
     #   # 
     #   # 
 1---3   2 
           
************
           
 1---3---2 
     |   | 
     |   | 
     |   | 
     |   | 
     |   | 
 1---3---2 
           
************
           
 1---3===2 
           
           
           
           
           
 1---3===2 

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


