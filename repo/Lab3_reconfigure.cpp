#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include <sstream>
#include <thread> 
#include <mutex>
#include <queue>
#include <stdlib.h>

#include "Binary_st.cpp"

using namespace std;

vector<string> words;
int words_in = 0;
const int size = pow(2,22);
int total = 0;
BinaryTree map;

// Indivudal operation queue
queue<string> actionQueue;
queue<pthread_t> threads;
queue<string> output;

// Pthread things
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexer = PTHREAD_MUTEX_INITIALIZER;

// Methods
void* letEmRun(void *);

void* insert(void*);
void inserter(string);

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

        // Queue up threads
        index = 0;
        while(index<threadcount && !actionQueue.empty()) {
        string temp;
        value = "";
        string nextAction = actionQueue.front();
        stringstream words_A(nextAction);

        words_A >> type;
        words_A >> key;
        //cout << key << "\n";
        int keyRand = stoi(key);
        while (words_A >> temp)
            value += temp + " ";
        
        // Proccess action
        
        if(type.compare("I") == 0) {
            //insert
            if(total<size) {
                string holder = to_string(keyRand) + " " + value;
                words.push_back(holder);
            threads.push( pthread_create(&ptid[index], NULL, &insert, (void*) &words_in));
            words_in += 1;
            } else {
                string out = "Max size of " + to_string(size) + " reached";
                output.push(out);
            }

        } else if (type.compare("D") == 0) {
            //delete
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &deleter, (void*) h));
        } else if (type.compare("L") == 0) {
            //lookup
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &lookup, (void*) h));
        } else {
            cout << "Invalid Format\n";
        }
        actionQueue.pop();
        index++;
        }

        // Proccess the queued threads
        for(int i = 0; i<threads.size(); i++) {
            pthread_t val;
            pthread_create(&val, NULL, &letEmRun, (void *) val);
            pthread_join(val, NULL);
        }
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

void* letEmRun(void *v) {
    pthread_mutex_lock(&mutexer);
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mutexer);
    pthread_exit(NULL);
}

void* insert(void *v) {
    pthread_mutex_lock(&mutexer);
    pthread_cond_wait(&cond1, &mutexer);
    string keyAndValue = words.at((*((int *)v)-1));
    threads.pop();
    
    inserter(keyAndValue);
    
    pthread_mutex_unlock(&mutexer);
    
    pthread_exit(NULL);

}
void inserter(string keyAndValue) {
    stringstream ss(keyAndValue);
    string kv, value, hold;
    ss >> kv;
    while (ss>>hold) {
        value += hold + " ";
    }

    int key = stoi(kv);
    
    if(map.insert(key, value)) {
        cout << value << " : " << key <<  " OK\n";
        total++;
        output.push("OK");
    } else {
        cout << value << " : " << key <<  " FAIL\n";
        output.push("FAIL");
    }
}

void* deleter(void *v) {

    pthread_mutex_lock(&mutexer);

    int *key = (int *) v;
    deleter_helper(*key);

    threads.pop();

    pthread_mutex_unlock(&mutexer);

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
    pthread_mutex_lock(&mutexer);

    int *key = (int *) v;
    lookup_helper(*key);

    threads.pop();

    pthread_mutex_unlock(&mutexer);
    
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