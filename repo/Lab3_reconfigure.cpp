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

// Pthread things
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataNotInserted = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotDeleted = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotFound = PTHREAD_COND_INITIALIZER;

// Methods
void insert_helper(int, int);
void inserter(int, string);
void deleter(int);
void lookup(int);
void* executer(void*);


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

    pthread_t threads[threadcount];
    int threadID[threadcount];

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
        int keyRand = stoi(key);
        while (words >> temp)
            value += temp + " ";
        
        
        if(type.compare("I") == 0) {
            //insert
        } else if (type.compare("D") == 0) {
            //delete
        } else if (type.compare("L") == 0) {
            //lookup
        } else {
            cout << "Invalid Format\n";
        }
        actionQueue.pop();





    }



}


