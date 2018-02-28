HOMEWORK 9:  PRIORITY QUEUES FOR MESH SIMPLIFICATION


NAME:  Simon Hopkins


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Evan, Rhaad, stackexchage.com, Sinclair

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  20


ALGORITHM ANALYSIS: 

Assume v_0 vertices, e_0 edges, and t_0 triangles in the initial mesh,
k edges connected to each vertex (average or worst case),
reduced to t_final triangles using the -shortest criteria.

What is the overall order notation for the program?  Analyze the
separate compoenents of the program to justify your answer.

ALGORITHM ANALYSIS -- LINEAR:

find min- O(e)
loop through all triangles- O(t)
remove edge- O(log(e))
add edge- O(log(e))

O(e)+O(t)+O(log(e))




ALGORITHM ANALYSIS -- PRIORITY QUEUE:

find min- O(1)
loop through triangles - O(t)
add edge- O(log(e))
remove edge- O(log(e))
update edges - O(log(e))

O(1)+ O(t)+ O(log(e))



ALGORITHM ANALYSIS -- IMPACT OF -preserve_area command line argument?
Do your answers above change with or without the -preserve_area
command line argument?  Describe.

preserve area adds another O(t), but it does this regardless of priority queue or linear. It will just
add another O(t) to each of their run times




EXTRA CREDIT:  COLOR & OVERALL APPERANCE
Discuss the quality of your results in your README.txt and include
screenshots of your more impressive results.  If you have submitted
screenshots, give the file names and describe the results.



MISC. COMMENTS TO GRADER:  
Optional, please be concise!






