#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include <thread> 
#include <queue>
#include "Binary_st.cpp"

using namespace std;

const int size = pow(2,22);
int total = 0;
string value;
BinaryTree map;

queue<thread> threadQueue;

void insert_helper(int);
void inserter(int, string);
void deleter(int);
void lookup(int);

int main(int argc, char **argv)
{ 

    while(!threadQueue.empty()) {
        threadQueue.pop();
    }

    string type;
    int threadcount;
    string holder;

    string filename;
    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;

    std::ifstream input_file;
    input_file.open(filename);

    input_file >> type;

    if (type.compare("N")==0) {
        input_file >> holder;
        threadcount = stoi(holder);
        cout << "Using " << threadcount << " threads\n";
    }

    while(input_file.good()) {
        //cout << threadQueue.size() << " : " << threadcount << "\n";
        if(threadQueue.size() >= threadcount) {
          threadQueue.front().join();
          threadQueue.front().detach();
          threadQueue.pop();
        }
        input_file >> type;

        // Get key from file
        string key_hold;
        input_file >> key_hold;
        int key = stoi(key_hold);
        value = "";
        if (type.compare("I") == 0 && getline( input_file, value )) {
            if (total>=size) {
                cout << "Max size reached";
            } else {
                threadQueue.push(std::thread(insert_helper, key));
            }
        } else if (type.compare("D") == 0) {
            threadQueue.push(std::thread(deleter, key));
        } else if (type.compare("L") == 0) {
            threadQueue.push(std::thread(lookup, key));
        }  else {
            cout << "Invalid command\n";
        }
    }

    //cout << "done with page\n";
    cout << threadQueue.size() << " : " << threadcount << "\n";

    while(!threadQueue.empty()) {
        threadQueue.front().join();
    if(!threadQueue.front().joinable()) {
        threadQueue.front().detach();
        cout << "here\n";
        threadQueue.pop();
        }
    }

    map.reset();

    input_file.close();
}

    void insert_helper(int key) {
        inserter(key, value);
        threadQueue.front().detach();
        threadQueue.pop();
    }
    void inserter(int key, string textt) {
        if(map.insert(key, textt)) {
                cout << textt << " OK\n";
                total++;
            } else {
                cout << "FAIL\n";
            }
    }

    void deleter(int key) {
        if(map.remove_key(key)) {
                cout << "OK\n";
                total--;
            } else {
                cout << "FAIL\n";
            }
            threadQueue.front().detach();
            threadQueue.pop();
    }

    void lookup(int key) {
        string found = map.Keysearcher(key);
            if(found.compare("nulll") != 0) {
                cout << found << "\n";
            } else {
                cout << "no " << key << "\n";
            }
            threadQueue.front().detach();
            threadQueue.pop();
    }
    

