# Lab 3 specifcs
The project descirption and requirments are located in Lab3.pdf

# Describing your approach
I took a more simple approach to this project.
1. The program takes in the input filename and output filename from the arguments
1. The program reads the first line (declaring the max thread count) and processes each line into an action queue
1. Then, while the action queue is not empty, create threads to thread count, join and detach threads to thread count and repeat
1. The threads call a method letEmRun, which takes the front of the action queue and executes that line appropriately.
    1. This method breaks up the action queue line into "type", "key", then if appropriate "value"
    1. Then depending on the type, calls another method (insert, lookup, and insert)
    1. The individual methods push to an output queue that holds the program's output during thread execution
1. After the action queue is empty, the programs loops through the output queue and reads to a file
1. Once the program has been completed, the execution time and a confirmation method are printed on the terminal 

# Describing your testing
For testing, I did a few different things.
1. I created a file that randomly creates a file with 500 random commands (I, L, and D)
1. For 4, 3, 2, and 1 threads I ran the same file 10 times each and averaged the times.
    1. All the test can be found in execution_time.txt

# Categorize your performance on reasonable input
I would argue that the performance is high. The program can handle many inserts, deletes, and lookups and are very accurate. The Program also scales (from 118,730.5 fro 1 thread to 52,965.6 for 4). While the program does have area of improvement, their is a high performance rate.