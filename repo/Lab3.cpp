#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <chrono>
#include <queue>
#include <pthread.h>

#include "Binary_st.cpp"
using namespace std::chrono;

//Constants
int words_in = 0;
const int size = pow(2,22);
int total = 0;
BinaryTree map;

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
    in_filename = argv[1];
    std::string out_filename;
    out_filename = argv[2];
    
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
    std::cout << "For " << threadcount << " threads:\tRead File Time: " << duration1.count() << "\tExecution Time: " << duration2.count() << "\tOutput File Time: " << duration3.count() << "\n";
    std::cout << "File Execution Completed\n";
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