#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include "Binary_st.cpp"

using namespace std;

const int size = pow(2,22);
int total = 0;

int main(int argc, char **argv)
{ 
    string filename;
    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;

    std::ifstream input_file;
    input_file.open(filename);

    BinaryTree map;
    srand (time(0));

    while(input_file.good()) {
        string type;
        string value;
        input_file >> type;
        if (type.compare("I") == 0 && getline( input_file, value )) {
            int key = rand()%size;
            if (total>=size) {
                cout << "Max size reached";
            } else if(map.insert(key, value)) {
                total++;
            }
        } else if (type.compare("D") == 0 && getline( input_file, value )) {
            // need delete based on string value
            if(true) {
                total--;
            }
        } else if (type.compare("L") == 0 && getline( input_file, value )) {
            map.StringSearcher(value);
        } else if (type.compare("P") == 0) {
            map.printer();
        } else {
            cout << "Invalid command\n";
        }

    }
    

}