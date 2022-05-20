Project 5 - CS315 with Dr.Raphael Finkel - Genealogy  
Author: Tanner Palin  
Sources: Looked into an old BFS implementation I used for CS216 with professor Pike a couple years ago.  
Discussion: Discussed techniques for BFS and the verification of data with Will Shapiro and Gave Rivera.  

Purpose: This program takes in a formatted text file to generate a bipartite graph containing people and families.  
Each person has two families, one they are born into, and one they are married into. Using the families we can  
check our data for integrity/consistency by ensuring a person is doubly linked to their families. In a similar way  
we can also determine how two people are related to each other by performing a breadth first search through the graph.  
Each family or person is recognized by an integer ID instead of a name/surname.  

Input: Here is an example of what the formatted text file should look like:  

Person 1 Parent 2 Marriage 1  
Person 4 Parent 5 Marriage 1  
Family 1 Husband 1 Wife 4 Child 7 Child 8  
Person 2 Parent 3 Marriage 2  
Person 3 Parent 4 Marriage 2  
Family 2 Husband 2 Wife 3 Child 13 Child 14 Child 1 Child 15  
Family 3 Husband 9 Wife 10 Child 2 Child 16  
Family 4 Husband 11 Wife 12 Child 3 Child 17  
Person 7 Parent 1 Marriage 0  
Person 8 Parent 1 Marriage 0  
Verify  
Person 13 Parent 2 Marriage 6  
Person 14 Parent 2 Marriage 0  
Family 5 Husband 5 Wife 6 Child 4 Child 19 Child 20  
Relate 1 7  
Relate 3 2  
