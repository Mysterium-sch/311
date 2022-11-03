#include <iostream>
#include <string>
#include<cmath>
using namespace std;

class input {
    public:
        int key;
        string value;

        input() {
            key = 0;
            value = "";
        }

        input(int k, string s) {
            key = k;
            value = s;
        }
};

const int size = pow(2,22); 


int main(int argc, char **argv)
{ 
    input map[size];
  

}