#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include <sstream>
#include <thread> 
#include <mutex>
#include <queue>

#include "Binary_st.cpp"

using namespace std;

const int size = pow(2,22);
int total = 0;
BinaryTree map;

// Indivudal operation queue
queue<string> actionQueue;
queue<pthread_t> threads;
queue<string> output;

// Pthread things
mutex mutexer;

// Methods
void* insert(void*);
void inserter(int, string);

void* deleter(void*);
void deleter_helper(int);

void* lookup(void*);
void lookup_helper(int);

int main(int argc, char **argv) { 

    string type, holder, value, key;
    int threadcount;

    string in_filename;
    in_filename = argv[1];
    string out_filename;
    out_filename = argv[2];

    ifstream input_file;
    input_file.open(in_filename);

    // Get thread count
    input_file >> type;
    if (type.compare("N")==0) {
        input_file >> holder;
        threadcount = stoi(holder);
        string out = "Using " + to_string(threadcount) + " threads";
        output.push(out);
    }

    pthread_t ptid[threadcount];
    int index = 0;

    //Pull all actions from input file
    while(input_file.good()) {
        string nextline;
        getline( input_file, nextline );
        actionQueue.push(nextline);
    }
        input_file.close();

        // Gets rid of first line
        actionQueue.pop();

        // Proccess Info
    while(!actionQueue.empty()) {
        string temp;
        value = "";
        string nextAction = actionQueue.front();
        stringstream words(nextAction);

        words >> type;
        words >> key;
        //cout << key << "\n";
        int keyRand = stoi(key);
        while (words >> temp)
            value += temp + " ";
        
        // Proccess action
        
        if(type.compare("I") == 0) {
            //insert
            if(total<size) {
            string holder = key + " " + value;
            threads.push( pthread_create(&ptid[index], NULL, &insert, (void*) &holder));
            std::this_thread::sleep_for (std::chrono::milliseconds(10));
            } else {
                string out = "Max size of " + to_string(size) + " reached";
                output.push(out);
            }

        } else if (type.compare("D") == 0) {
            //delete
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &deleter, (void*) h));
            std::this_thread::sleep_for (std::chrono::milliseconds(10));
        } else if (type.compare("L") == 0) {
            //lookup
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &lookup, (void*) h));
            std::this_thread::sleep_for (std::chrono::milliseconds(10));
        } else {
            cout << "Invalid Format\n";
        }
        actionQueue.pop();

        // Make sure threads stay under 4
        if(threads.size() >= threadcount) {
            pthread_join(threads.front(), NULL);
            pthread_exit(NULL);
        }

        index = (index+1)%threadcount;

    }

    // Empty queue
    while(!threads.empty()) {
        pthread_join(threads.front(), NULL);
        pthread_exit(NULL);

    }

    //Output to file
    ofstream outFile;
    outFile.open(out_filename);
    while(!output.empty()) {
        outFile << output.front() << "\n";
        output.pop();

    }

    cout << "File Execution Completed\n";

}

void* insert(void *v) {
    // wait on lookup/mutex
    // wait on delete/mutex

    //std::lock_guard<std::mutex> guard(mutexer);

    string *keyAndValue = (string *) v;
   
    stringstream ss(*keyAndValue);
    string kv, value, hold;
    ss >> kv;
    while (ss>>hold) {
        value += hold + " ";
    }

    int key = stoi(kv);
    inserter(key, value);
    threads.pop();

    return 0;

}
void inserter(int key, string textt) {
    if(map.insert(key, textt)) {
        //cout << textt << " : " << key <<  " OK\n";
        total++;
        output.push("OK");
    } else {
        output.push("FAIL");
    }
}

void* deleter(void *v) {
    // wait on insert/mutex
    // wait on lookup/mutex

    //std::lock_guard<std::mutex> guard(mutexer);

    int *key = (int *) v;
    deleter_helper(*key);

    threads.pop();

    return 0;
    
}
void deleter_helper(int key) {
    if(map.remove_key(key)) {
         output.push("OK");
        total--;
    } else {
        output.push("FAIL");
    }
}

void* lookup (void *v) {
    // wait on insert/mutex
    // wait on delete/mutex
   //std::lock_guard<std::mutex> guard(mutexer);

    int *key = (int *) v;
    lookup_helper(*key);

    threads.pop();
    
    return 0;
}
void lookup_helper(int key) {
    string found = map.Keysearcher(key);
        if(found.compare("nulll") != 0) {
            output.push(found);
        } else {
            string out = "no " + to_string(key); 
            output.push(out);
    }

}