#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include <thread> 
#include <mutex>
#include <queue>

#include "Binary_st.cpp"

using namespace std;

const int size = pow(2,22);
int total = 0;
BinaryTree map;

queue<thread> threadQueue;
vector<string> values;
mutex in,del,look;

void insert_helper(int, int);
void inserter(int, string);
void deleter(int);
void lookup(int);


int main(int argc, char **argv) { 
    int valuesIndex = 0;

    while(!threadQueue.empty()) {
        threadQueue.pop();
    }

    string type;
    int threadcount;
    string holder;
    string value;

    string filename;
    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;

    ifstream input_file;
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
        while(threadQueue.front().joinable()) {
            try {
                threadQueue.front().detach();
                threadQueue.pop();
            } catch (exception& exc) { 
                threadQueue.front().join();
                std::this_thread::sleep_for (std::chrono::seconds(1));
            
            }
        }
        }
        
        input_file >> type;
        // Get key from file
        string key_hold;
        input_file >> key_hold;
        int key;
        key = stoi(key_hold);
  
        value = "";
        //cout << threadQueue.size() << "\n";
        if (type.compare("I") == 0 && getline( input_file, value )) {
            if (total>=size) {
                cout << "Max size reached";
            } else {
                //cout << threadQueue.size() << "\n";
                threadQueue.push(std::thread(insert_helper, (key), valuesIndex));
                values.push_back(value);
                valuesIndex++;
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

    while(!threadQueue.empty()) {
        cout << threadQueue.size() << " : " << threadcount << "\n";
        while(threadQueue.front().joinable()) {
            try {
                threadQueue.front().join();
            } catch (exception& exc) { 
                std::this_thread::sleep_for (std::chrono::seconds(1));
            }
        }
        threadQueue.front().detach();
        threadQueue.pop();

    }

    map.reset();
    input_file.close();
}

// Needs to wait on lookup and deleter
void insert_helper(int key, int index) {
    while(in.try_lock()) { }
    lock (del,look);
    threadQueue.front().detach();
    threadQueue.pop();

    string v = values.at(index);
    inserter(key, v);

    del.unlock();
    look.unlock();
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

// Needs to wait on lookup and insert
void deleter(int key) {
    while(del.try_lock()) { }
    lock (in,look);
    threadQueue.front().detach();
    threadQueue.pop();
    if(map.remove_key(key)) {
        cout << "OK\n";
        total--;
    } else {
        cout << "FAIL\n";
    }

    in.unlock();
    look.unlock();

}

// Needs to wait on insert and delete
void lookup(int key) {
    while(look.try_lock()) { }
    lock (del,in);

    threadQueue.front().detach();
    threadQueue.pop();

    string found = map.Keysearcher(key);
        if(found.compare("nulll") != 0) {
            cout << found << "\n";
        } else {
            cout << "no " << key << "\n";
    }

    del.unlock();
    in.unlock();
}

