HOMEWORK 3: GPS TRACKING & STACK HACKING


NAME:  Simon Hopkins


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassment, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Evan, ALAC tutoring, Martin Duffy

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  16h



PART 2: DIAGRAMMING MEMORY & MAKING HYPOTHESES
List the name of the images files of the diagrams you are uploading
with this portion of the assignment.  Describe the scenario shown in
each diagram, and your predictions for the errors that will arise from
incorrect use of your distance and filter functions.

distance_correct.jpg

In this scenerio, I am showcasing the distance function and its correct implimentation when n is set to the count.

distance_incorrect.jpg

In this scenerio, I am showcasing the distance function when n is set to more than count. I predict with this error the extra values will be set to garbage values in the stack, set to whatever previous values were in the extra allotted memory locations.

filter_correct.jpg

In this scenerio, I am showcasing the filter function and its correct implimentation when n is set to the count.

filter_incorrect.jpg

In this scenerio, I am showing what will happen if you understate the amount of values. The smoothing operation will make the length of output too short, which will severely underestimate the total distance traveled after it is filtered one time.


PART 3: TESTING YOUR HYPOTHESES
Describe the testing of your hypotheses for part 2.  Paste small
examples of the stack visualization that support your investigation.
Describe the program behavior/errors that results when you exaggerate
the mis-use of memory with these functions.

My hypothesis of the excess space being filled with garbage was confirmed when running the program and
manualy setting the variable n to a value higher than the actual count. This led to the results printing 
absurdly high, and negative, values that must have been derived from leftover values in the stack. When I tried doing the same thing with my filter function, manually setting the value to greater than the count, I got a segmentation fault. This was undoubtably due to the fact that, in the for loop where it loops n-1 times, any n value over the count does not exist in input. Also you can see, by printing the stack before and after filtering, each GPSData object in input had it's speed modified, which makes sense because it's values were filtered to become more uniform.


PART 4: PASS-BY-VALUE VS PASS-BY-REFERENCE
Paste the relevant portion of the visualization and describe in your
own words what the visualization demonstrates.

ave_feet_per_minute, original_avg_speed location: 0xbffa7604  VALUE:   738
ave_feet_per_minute, filtered_avg_speed location: 0xbff385e4  garbage or float? 
ave_feet_per_minute, recursive_filtered_avg_speed location: 0xbffa75c4  VALUE:   630

n location: 0xbff57340  VALUE:   6


This shows that ave_feet_per_minute is created 3 times, each time at the memory location that the variable you are passing is stored. Alternatively, n is the only variable stored at memory location 0xbff57340, and also holds a value copied from the argument, count. This demonstrates how passing by reference is really just passing the memory location of the variable, and not copying the value of the variable, rather than passing by value, in which you copy the value of the variable.



PART 5: STACK FRAMES OF A RECURSIVE FUNCTION
Paste the relevant portion of the stack visualization (abbreviate as
appropriate) and your observations about the operation of recursive
functions on the stack.


As you can see here, temp and its values are being recursively smoothed using the filter function. It will do this recursively until the percent_change is less than a specified percent_change_threshold. This is why it will print out the stack more if the percent_change_threshold is smaller, because it needs to run more iterations of the recursive funtion to smooth the data points. 
						
						  location: 0xbff430b4  VALUE:   70
                          location: 0xbff430b0  VALUE:   30
                          location: 0xbff430ac  VALUE:   429
                          location: 0xbff430a8  VALUE:   75
                          location: 0xbff430a4  VALUE:   0
                      tmp location: 0xbff430a0  VALUE:   0
						...
                          location: 0xbff42ed4  VALUE:   68
                          location: 0xbff42ed0  VALUE:   36
                          location: 0xbff42ecc  VALUE:   517
                          location: 0xbff42ec8  VALUE:   75
                          location: 0xbff42ec4  VALUE:   0
                      tmp location: 0xbff42ec0  VALUE:   0
                      ...
                          location: 0xbff42cf4  VALUE:   65
                          location: 0xbff42cf0  VALUE:   38
                          location: 0xbff42cec  VALUE:   554
                          location: 0xbff42ce8  VALUE:   75
                          location: 0xbff42ce4  VALUE:   0
                      tmp location: 0xbff42ce0  VALUE:   0
MISC. COMMENTS TO GRADER:  
Optional, please be concise!

