#include <iostream>
#include <string>
#include<cmath>
#include <fstream>
#include "Binary_st.cpp"

using namespace std;

int main(int argc, char **argv)
{ 
    string filename;
    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;

    BinaryTree map;
    map.input(filename);
    map.printer();

}