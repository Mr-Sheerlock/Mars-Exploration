# Mars-Exploration
This is the Data Structures and Algorithms Course Project where we were to design and choose the best fitting data structures for the tasks to achieve the best complexity possible, thus reducing the time of operation with higher sizes of input. 

## How to develop a similar project? 

1-Well, you start by first thinking carefully about each operation and the worst possible case. For instance, we chose the priority queue data structure to keep track of the in-execution rovers, which were rovers that were already sent and that have not returned to the master station yet. This achieved O(1) complexity in all required operations, because by arranging them according to their arrival day where the rover arriving first will have the highest priority, this ensures that the operation takes only one step. Except perhaps... whoops! There is a corner case here, specifically in case of mission failure, where we need to determine which rover exactly failed in its mission, thus we need to specify it and retrieve it, this meant that this operation was O(n).  

2-Secondly, it was required that we start our project files and divide it into modules to achieve insulation and ease at writing the code. 

3-Finally, we started writing the code, we needed to prepare the data strucutre choosing the best implementation for each structure, then we implemented the input and output classes, then the classes like rovers, missions each with their types, then finally the Master Station class. 


