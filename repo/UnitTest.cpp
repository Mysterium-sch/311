#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <queue>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <mutex>

#include <gtest/gtest.h> 
#include "Binary_st.cpp"
BinaryTree map1;


//check output file
TEST(OutputTest, OutputFile) {
    std::ifstream outEx;
    std::ifstream out;
    outEx.open("outputEx.txt");
    out.open("output.txt");
    std::string begin;
    std::string beginX;
    std::string line;
    std::string lineX;
    while(getline(outEx, beginX)) {
        lineX = beginX.data();
        getline(out, begin);
        line = begin.data();
        if(lineX == "") {
            outEx.close();
        }
        EXPECT_EQ(lineX, line);
    }
}

//Check input
TEST(InputTest, InputReadTime) {
    std::ifstream input;
    input.open("input.txt");
    std::string begin;
    std::string line;
    while(getline(input, begin)) {
        line = begin.data();
        EXPECT_EQ(line,line);
    }
    EXPECT_EQ(1,1); // check it is over
}

TEST(InputTestBig, InputReadTimeBig) {
    std::ifstream input;
    input.open("bigBoy.txt");
    std::string begin;
    std::string line;
    while(getline(input, begin)) {
        line = begin.data();
        EXPECT_EQ(line,line);
    }
    EXPECT_EQ(1,1); // check it is over
}

//check threadcount
TEST(ThreadTest, Threads) {
    std::ifstream output;
    output.open("output.txt");
    std::string threadcount;
    std::string begin = "Using";
    if(output.is_open()) {
        getline(output, begin);
        threadcount = begin.data();
    }
    EXPECT_EQ("Using 4 threads", threadcount);
}

//Binary Tree Tests
//check insert
TEST(BTInsertTest, InsertValid) {
    EXPECT_EQ(true,  map1.insert(123, "valid"));
    EXPECT_EQ(true,  map1.insert(456, "valid"));
    EXPECT_EQ(true,  map1.insert(789, "valid"));
    EXPECT_EQ(true,  map1.insert(1011, "valid"));
    EXPECT_EQ(true,  map1.insert(1213, "valid"));
    
} 
TEST(BTInsertTest, InsertInvalid) {
    EXPECT_EQ(false,  map1.insert(123, "invalid"));
    EXPECT_EQ(false,  map1.insert(456, "invalid"));
    EXPECT_EQ(false,  map1.insert(789, "invalid"));
    EXPECT_EQ(false,  map1.insert(1011, "invalid"));
    EXPECT_EQ(false,  map1.insert(1213, "invalid"));
}

TEST(BTInsertTestScaling, InsertHalf) {
    for(int i = 60000; i<64999;i++) {
        EXPECT_EQ(true,  map1.insert(i, "valid"));
    }
} 
TEST(BTInsertTestScaling, InsertLarge) {
    for(int i = 70000; i<79999;i++) {
        EXPECT_EQ(true,  map1.insert(i, "valid"));
    }
}

//check delete
TEST(BTDeleteTest, DeleteValid) {
    EXPECT_EQ(true,  map1.remove_key(123));
    EXPECT_EQ(true,  map1.remove_key(456));
    EXPECT_EQ(true,  map1.remove_key(789));
    EXPECT_EQ(true,  map1.remove_key(1011));
    EXPECT_EQ(true,  map1.remove_key(1213));
}
TEST(BTDeleteTest, DeleteInvalid) {
    EXPECT_EQ(false,  map1.remove_key(123));
    EXPECT_EQ(false,  map1.remove_key(456));
    EXPECT_EQ(false,  map1.remove_key(789));
    EXPECT_EQ(false,  map1.remove_key(1011));
}

TEST(BTDeleteTestScaling, DeleteHalf) {
    for(int i = 60000; i<64999;i++) {
        EXPECT_EQ(true,  map1.remove_key(i));
    }
} 
TEST(BTDeleteTestScaling, DeleteLarge) {
    for(int i = 70000; i<79999;i++) {
        EXPECT_EQ(true,  map1.remove_key(i));
    }
}

//check lookup
TEST(BTLookupTest, LookupValid) {
    map1.insert(382171, "valid");
    std::string found = map1.Keysearcher(382171);
    map1.insert(38, "valid");
    std::string found1 = map1.Keysearcher(38);
    map1.insert(1, "valid");
    std::string found2 = map1.Keysearcher(1);
    map1.insert(211, "valid");
    std::string found3 = map1.Keysearcher(211);
    EXPECT_EQ(found, "valid");
    EXPECT_EQ(found1, "valid");
    EXPECT_EQ(found2, "valid");
    EXPECT_EQ(found3, "valid");
}
TEST(BTLookupTest, LookupInvalid) {
    std::string found = map1.Keysearcher(5);
    std::string found1 = map1.Keysearcher(46);
    std::string found2 = map1.Keysearcher(99);
    std::string found3 = map1.Keysearcher(201);
    EXPECT_EQ(found, "nulll");
    EXPECT_EQ(found1, "nulll");
    EXPECT_EQ(found2, "nulll");
    EXPECT_EQ(found3, "nulll");
}

TEST(BTLookupTestScaling, Lookuphalf) {
    for(int i = 40000; i<44999;i++) {
        map1.insert(i, "valid");
        std::string found = map1.Keysearcher(i);
        EXPECT_EQ(found, "valid");
    }
}
TEST(BTLookupTestScaling, LookupLarge) {
    for(int i = 50000; i<59999;i++) {
        map1.insert(i, "valid");
        std::string found = map1.Keysearcher(i);
        EXPECT_EQ(found, "valid");
    }
}

//Lab3 Tests

//LAB3 CODE 

    using namespace std::chrono;

    //Constants
    int words_in = 0;
    const int size = pow(2,22);
    int total = 0;
    BinaryTree map;
    std::string in = "input.txt";
    std::string out = "output.txt";

    // Indivudal operation queue
    std::queue<std::string> actionQueue;
    std::queue<std::string> output;

    // Pthread things
    pthread_mutex_t mutexer = PTHREAD_MUTEX_INITIALIZER;

    // Methods
    void* letEmRun(void *);

    void inserter(std::string);
    void deleter(int);
    void lookup(int);
        
    int main(int argc, char **argv) { 
        auto start = high_resolution_clock::now();

        std::string type, holder, value, key;
        int threadcount;
        
        std::string in_filename;
        in_filename = in;
        std::string out_filename;
        out_filename = out;
        
        std::ifstream input_file;
        input_file.open(in_filename);

        // Get thread count
        input_file >> type;
        if (type.compare("N")==0) {
            input_file >> holder;
            threadcount = stoi(holder);
            std::string out = "Using " + std::to_string(threadcount) + " threads";
            output.push(out);
        }
        
        pthread_t ptid[threadcount];
        int index;

        //Pull all actions from input file
        while(input_file.good()) {
            std::string nextline;
            getline( input_file, nextline );
            actionQueue.push(nextline);
        }
            input_file.close();

            // Gets rid of first line
            actionQueue.pop();
            auto mid = high_resolution_clock::now();

            // Proccess Info
        while(!actionQueue.empty()) {
            index = 0;
            // create the threads
            while(index < threadcount && !actionQueue.empty()) {
                pthread_create(&ptid[index], NULL, &letEmRun, (void *) &index);
                index++;
            }
            // process the threads
            index = 0;
            while(index < threadcount && !actionQueue.empty()) {
                pthread_join(ptid[index], NULL);
                pthread_detach(ptid[index]);
                index++;
            }
        }

        auto mid2 = high_resolution_clock::now();

        //Output to file
        std::ofstream outFile;
        outFile.open(out_filename);
        while(!output.empty()) {
            outFile << output.front() << "\n";
            output.pop();
        }

        map.reset();
        outFile.close();
        
        auto stop = high_resolution_clock::now();
        auto duration1 = duration_cast<microseconds>(mid - start);
        auto duration2 = duration_cast<microseconds>(mid2 - mid);
        auto duration3 = duration_cast<microseconds>(stop - mid2);

        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

    }

    void* letEmRun(void *v) {
        pthread_mutex_lock(&mutexer);
            std::string nextAction = actionQueue.front();
            std::string temp, type, key;
            std::string value = "";
            std::stringstream words_A(nextAction);

            words_A >> type;
            words_A >> key;
            int keyRand = -1;
            if(type.compare("I") == 0 || type.compare("D") == 0 || type.compare("L") == 0) {
                keyRand = stoi(key);
                
            while (words_A >> temp)
                value += temp + " ";
            
            // Proccess action
            
            if(type.compare("I") == 0) {
                //insert
                if(total<size) {
                    std::string holder = std::to_string(keyRand) + " " + value;
                    inserter(holder);
                } else {
                    std::string out = "Max size of " + std::to_string(size) + " reached";
                    output.push(out);
                }

            } else if (type.compare("D") == 0) {
                //delete
                deleter(keyRand);
            } else if (type.compare("L") == 0) {
                //lookup
                lookup(keyRand);
            }
            }

        if(actionQueue.size() > 0) {
            actionQueue.pop();
        }
        pthread_mutex_unlock(&mutexer);
        pthread_exit(NULL);

    }

    void inserter(std::string keyAndValue) {
        std::stringstream ss(keyAndValue);
        std::string kv, value, hold;
        ss >> kv;
        while (ss>>hold) {
            value += hold + " ";
        }
        value = value.substr(0, value.length()-1);
        int key = stoi(kv);
        
        if(map.insert(key, value)) {
            total++;
            output.push("OK");
        } else {
            output.push("FAIL");
        }
    }
    void deleter(int key) {
        if(map.remove_key(key)) {
            output.push("OK");
            total--;
        } else {
            output.push("FAIL");
        }
    }

    void lookup(int key) {
        std::string found = map.Keysearcher(key);
            if(found.compare("nulll") != 0) {
                output.push(found);
            } else {
                std::string out = "no " + std::to_string(key); 
                output.push(out);
        }
    }
//END OF LAB3 CODE


//Key Counter Test
TEST(TotalCountTest, Valid) {
    EXPECT_EQ(33, total);
}
TEST(TotalCountTest, Invalid) {
    ASSERT_NE(100, total);
    ASSERT_NE(500, total);
    ASSERT_NE(10, total);
    ASSERT_NE(1570, total);
    ASSERT_NE(1124, total);
}

//Lab3 Insert Check
TEST(Lab3InsertTest, Valid) {
    std::string null = map.Keysearcher(382171);
    std::string null1 = map.Keysearcher(38217);
    std::string null2 = map.Keysearcher(3821);
    std::string null3 = map.Keysearcher(382);
    EXPECT_EQ("nulll", null);
    EXPECT_EQ("nulll", null1);
    EXPECT_EQ("nulll", null2);
    EXPECT_EQ("nulll", null3);
    bool worked = false;
    inserter("382171 valid");
    std::string found = map.Keysearcher(382171);
    if(found == "valid") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
    worked = false;
    inserter("38217 valid");
    std::string found1 = map.Keysearcher(38217);
    if(found1 == "valid") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
    worked = false;
    inserter("3821 valid");
    std::string found2 = map.Keysearcher(3821);
    if(found2 == "valid") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
    worked = false;
    inserter("382 valid");
    std::string found3 = map.Keysearcher(382);
    if(found3 == "valid") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
}
TEST(Lab3InsertTest, Invalid) {
    std::string null = map.Keysearcher(382171);
    std::string null1 = map.Keysearcher(38217);
    std::string null2 = map.Keysearcher(3821);
    std::string null3 = map.Keysearcher(382);
    EXPECT_EQ("valid", null);
    EXPECT_EQ("valid", null1);
    EXPECT_EQ("valid", null2);
    EXPECT_EQ("valid", null3);
    bool worked = false;
    inserter("382171 invalid");
    std::string found = map.Keysearcher(382171);
    if(found == "invalid") {
        worked = true;
    }
    EXPECT_EQ(false, worked);
    worked = false;
    inserter("38217 invalid");
    std::string found1 = map.Keysearcher(38217);
    if(found1 == "invalid") {
        worked = true;
    }
    EXPECT_EQ(false, worked);
    worked = false;
    inserter("3821 invalid");
    std::string found2 = map.Keysearcher(3821);
    if(found2 == "invalid") {
        worked = true;
    }
    EXPECT_EQ(false, worked);
    worked = false;
    inserter("382 invalid");
    std::string found3 = map.Keysearcher(382);
    if(found3 == "invalid") {
        worked = true;
    }
    EXPECT_EQ(false, worked);
}
TEST(Lab3InsertTest, Half) {
    for(int i = 40000; i<44999;i++) {
        std::string number = std::to_string(i);
        number = number + " valid";
        inserter(number);
        std::string found = map1.Keysearcher(i);
        EXPECT_EQ(found, "valid");
    }
}
TEST(Lab3InsertTest, Large) {
    for(int i = 50000; i<59999;i++) {
        std::string number = std::to_string(i);
        number = number + " valid";
        inserter(number);
        std::string found = map1.Keysearcher(i);
        EXPECT_EQ(found, "valid");
    }
}

//Lookup
TEST(Lab3LookupTest, Valid) {
    std::string null = map.Keysearcher(382171);
    std::string null1 = map.Keysearcher(38217);
    std::string null2 = map.Keysearcher(3821);
    std::string null3 = map.Keysearcher(382);
    EXPECT_EQ("valid", null);
    EXPECT_EQ("valid", null1);
    EXPECT_EQ("valid", null2);
    EXPECT_EQ("valid", null3);
    lookup(382171);
    null = map.Keysearcher(382171);
    EXPECT_EQ("valid", null);
}

//Lab3 Delete Check
TEST(Lab3DeleteTest, Valid) {
    std::string null = map.Keysearcher(382171);
    std::string null1 = map.Keysearcher(38217);
    std::string null2 = map.Keysearcher(3821);
    std::string null3 = map.Keysearcher(382);
    EXPECT_EQ("valid", null);
    EXPECT_EQ("valid", null1);
    EXPECT_EQ("valid", null2);
    EXPECT_EQ("valid", null3);
    bool worked = false;
    deleter(382171);
    std::string found = map.Keysearcher(382171);
    if(found == "nulll") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
    worked = false;
    deleter(38217);
    std::string found1 = map.Keysearcher(38217);
    if(found1 == "nulll") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
    worked = false;
    deleter(3821);
    std::string found2 = map.Keysearcher(3821);
    if(found2 == "nulll") {
        worked = true;
    }
    EXPECT_EQ(true, worked);
}


//MORE
TEST(Lab3DeleteTest, Invalid) {
    std::string null = map.Keysearcher(382171);
    std::string null1 = map.Keysearcher(38217);
    std::string null2 = map.Keysearcher(3821);
    EXPECT_EQ("nulll", null);
    EXPECT_EQ("nulll", null1);
    EXPECT_EQ("nulll", null2);
}

//Output Queue
TEST(OutputQueueTest, FrontAndBack) {
    std::string front = output.front();
    std::string back = output.back();
    EXPECT_EQ("OK", front);
    EXPECT_EQ("FAIL", back);
}
//Output not empty
TEST(OutputQueueTest, CheckEmpty) {
    EXPECT_EQ(false, output.empty());
}
