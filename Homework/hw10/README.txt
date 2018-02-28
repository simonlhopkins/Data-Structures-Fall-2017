HOMEWORK 10: IMAGE COMPARISON HASHING


NAME: Simon Hopkins


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cplusplus.comp, stackoverflow.com

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  16 hrs




i = # of images
w = width x h = height (average size image)
s x s = seed size
r x r = typical subregion match. 
t = hash table size 
c = fraction of hash table to compare


ANALYSIS OF SIMPLE ALGORITHM (RUNNING TIME & MEMORY USAGE)
Include a short explanation/justification of your answer.

Running time:
O(w)- loop a width
O(h)- loop a height
O(w)- loop b width
O(h)- loop b height
O(s)- loop seed for the y direction
O(s)- loop seed for the x direction
+
O(r)- loop subregion width
O(r)- loop subregion height

O(w^2h^2s^2 + r^2)

Memory usage:

No new memory is allocated on the heap. Only on the stack



ANALYSIS OF HASH TABLE ALGORITHM (RUNNING TIME & MEMORY USAGE)
Include a short explanation/justification of your answer.

Running time:

O(w)*O(h)*O(s)*O(s)+
O(w)*O(h)*O(s)*O(s)*O(number of items in each hash slot (usually one one))

+O(hits)+O(c)

Memory usage:

2 arrays of size tableSize need to be created for the two hash tables. This is much faster, but much
less memory efficient.


SUMMARY OF RESULTS: 
Test your implementation on a variety of images and command line
arguments and summarize and discuss the results (both running time and
quality of results).

After running with the sunflowers, the marbles, and running with the cats, it is plain to see that it is much more effcient than the simple compare. When I ran it with just the plain marbles, it was very quick and very accurate, with no outliers for 1% of the results. However, when I moved onto the noisy marbles, there were a couple outliers, but a very similar run time. For the sunflowers however, since it had to compare all of the smaller images to the huge sunflower image, each time I had to repopulate the hash table with the giant sunflower image, slowing it down significantly. However, this is still much faster than looping through every pixel for each pixel in the smaller image, for you only have to do this huge loop one time per comarison.


EXTRA CREDIT -- MULTIPLE ROTATION AND/OR 90/180/270 ROTATION AND
HORIZONTAL/VERTICAL FLIP:
Discuss your implementation and results of testing.




MISC. COMMENTS TO GRADER:  
Optional, please be concise!



                                                       :---::::.......::::                          
                                                      :---:::........::::                           
                                                    .----::..........::.                            
                                                   :----:::::......::::                             
                                               :-===----:::::::::::::.                              
                                            .--=-----:::::::::::::::.                               
                                          :=-----::::::::::::::::::.                                
                                        :==---:::::::...:::::::::-:                                 
                                     :-=---::::..........::::::::-.                                 
                                 :---::::::::............:::::::-:                                  
                            .----:::::..................:::::::--.                                  
                          .---:::::.....................:::::::-:                                   
                          .::................:::.......::::::::-.                                   
                          .::.......:.    ..::..........::::::--                                    
                          ::::..:..           .:.:......::::::-:                                    
                         .--:.::.               .::.....:::::--.                                    
                         :::::                   ::::::.:::::--.                                    
                         .:::-=:                .:==-..:::::---                                     
                          .:::---              .---:.....:::--:                                     
                           .::::-=--:      .:::=-:::.......:--:                                     
                            .::.::::-=-:----::::...........:::.                                     
                              .:....::::::.................:::                                      
                                 .:......................::::-.                                     
                                    .::...::....  ........:::-:                                     
                                                  ::....:::::--.                                    
                                                 ::....:::..::--.                                   
                                                .:.....:::..::---                                   
                                               .::.....::....:-:--.                                 
                                               .::...::.:....:::--:                                 
                                               :::..::: .:...:.                                     
                                              .-:..:::  .:...:                                      
                                              :::...:   .:...:                                      
                                             .::...:.    :...:                                      
                                             ::...::     :...:                                      
                                            .-:...:      ::.::                                      
                                            :-:..:.      ::.::                                      
                                           .-::.::       ::.:-                                      
                                           :-:.::        .::.-.                                     
                                           --:.-.          .::                                      
                                           :-::-                                                    
                                            .::.                                                    

