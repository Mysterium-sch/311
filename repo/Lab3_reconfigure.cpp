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

// Pthread things
pthread_mutex_t mutexer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataNotInserted = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotDeleted = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotFound = PTHREAD_COND_INITIALIZER;

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

    string filename;
    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;

    ifstream input_file;
    input_file.open(filename);

    // Get thread count
    input_file >> type;
    if (type.compare("N")==0) {
        input_file >> holder;
        threadcount = stoi(holder);
        cout << "Using " << threadcount << " threads\n";
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
            string holder = key + " " + value;
            threads.push( pthread_create(&ptid[index], NULL, &insert, (void*) &holder));
            std::this_thread::sleep_for (std::chrono::microseconds(10));

        } else if (type.compare("D") == 0) {
            //delete
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &deleter, (void*) h));
            std::this_thread::sleep_for (std::chrono::microseconds(10));
        } else if (type.compare("L") == 0) {
            //lookup
            int *h = &keyRand;
            threads.push( pthread_create(&ptid[index], NULL, &lookup, (void*) h));
            std::this_thread::sleep_for (std::chrono::microseconds(10));
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
        index = (index+1)%threadcount;

    }

    cout << "done\n";

}

void* insert(void *v) {
    // wait on lookup/mutex
    // wait on delete/mutex

    pthread_mutex_lock(&mutexer);

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
    pthread_mutex_unlock(&mutexer);
    return 0;

}
void inserter(int key, string textt) {
    if(map.insert(key, textt)) {
        cout << textt << " : " << key <<  " OK\n";
        total++;
        //cout << "OK\n";
    } else {
        cout << "FAIL\n";
    }
}

void* deleter(void *v) {
    // wait on insert/mutex
    // wait on lookup/mutex

    pthread_mutex_lock(&mutexer);

    int *key = (int *) v;
    deleter_helper(*key);

    threads.pop();
    pthread_mutex_unlock(&mutexer);

    return 0;
    
}
void deleter_helper(int key) {
    if(map.remove_key(key)) {
        cout << "OK\n";
        total--;
    } else {
        cout << "FAIL\n";
    }
}

void* lookup (void *v) {
    // wait on insert/mutex
    // wait on delete/mutex
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
            cout << found << "\n";
        } else {
            cout << "no " << key << "\n";
    }

}


