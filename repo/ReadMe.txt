Lab 3 specifics

Running the project:
1. Download and extract the project
2. Move to repo directly in the terminal
3. run cmake CMakeLists.txt
4. run make
5. Now there are two useful executables: Lab_3 and UnitTest_test
    i. to run the lab's executable an input and output file need to be given
        a. ie. "./Lab_3 input.txt output.txt"
    ii.  to run the test's executable all that is needed is the executable
        a. ie. "./UnitTest_test"

Describing your approach:
I took a more simple approach to this project.
1. The program takes in the input filename and output filename from the arguments
2. The program reads the first line (declaring the max thread count) and processes each line into an action queue
3. Then, while the action queue is not empty, create threads to thread count, join and detach threads to thread count and repeat
4. The threads call a method letEmRun, which takes the front of the action queue and executes that line appropriately.
    i. This method breaks up the action queue line into "type", "key", then if appropriate "value"
    ii. Then depending on the type, calls another method (insert, lookup, and insert)
    iii. The individual methods push to an output queue that holds the program's output during thread execution
5. After the action queue is empty, the programs loops through the output queue and reads to a file
6. Once the program has been completed, the execution time and a confirmation method are printed on the terminal 


Describing your testing:

Testing done by Lily:
For testing, I did a few different things.
1. I created a file that randomly creates a file with 500 random commands (I, L, and D)
2. For 4, 3, 2, and 1 threads I ran the same file 10 times each and averaged the times.
    i. All the tests can be found in execution_time.txt

Testing done by Valerie:
For unit testing, I chose GTest. Within my testing file (UnitTest.cpp) I tested many aspects of our project.
1. I started by checking the input and output files. For the output files, I checked the generated output file with an output file I created by hand. I checked these two files line by line to ensure they were the same. For the input file, I read the file line by line to see how long it takes to read this file. I also checked the first line of the input file for the thread count to ensure it was what was expected.
2. I then created tests to check out the data structure (binary search tree). I created tests to check the insert, delete, and lookup functions.
3. The insert tests checked for correct insertion of given values and how long it takes to insert a relatively small amount of inserts and a relatively large amount of inserts. 
4. The delete tests checked for correct deletion of given values and how long it takes to delete a relatively small amount of deletions and a relatively large amount of deletions. 
5. The lookup tests checked for correct lookups of given values and how long it takes to lookup a relatively small amount of values and a relatively large amount of values. 
6. The next section of the tests was dedicated to testing the main file (Lab3). I first checked the counter to make sure it was at the expected value. I then checked the Lab3 insertion method for the same things I looked for in the binary tree insertion tests. I also checked the deletion of valid values. Lastly, checked the output queue to make sure it was not empty, and had the correct starting and ending values.

Categorize your performance on reasonable input:
I would argue that the performance is high. The program can handle many inserts, deletes, and lookups and are very accurate. The Program also scales (from 118,730.5 for 1 thread to 52,965.6 for 4). While the program does have areas for improvement, there is a high-performance rate. The binary tree structure allows for this high performance. It is able to handle insertions, deletions and lookups without error, aswell as the ability to handle high amounts of each. The unit testing also further proves this. The program takes in the input file correctly and outputs to the corresponding output file with correct information.

It should be noted that while the performance runs accurately, it does have some difficulty with high volume files. The largest test file tested successfully was 500 lines; however, when attempting to run a file with one million lines - the program was seemingly caught in a loop. Other than this, the performance is high.
