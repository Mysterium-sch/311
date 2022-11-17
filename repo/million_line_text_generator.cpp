#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class FileGenerate {
    
    public:
    FileGenerate() {
    }

void creator()
{
    int cases = 0;
    srand (time(0));
    vector<int> keys;

    string filename1 = "ten_"+to_string(cases)+".txt";
    std::ofstream output_file1(filename1);

    while(cases < 1000000) {
        string comand;
        int key = rand()%(4000000);
        int dec = rand()%3;

        if(dec == 1) {
            comand = "L" + to_string(keys.at((int)(rand()%keys.size())));
        } else if (dec == 2){
            comand = "D" + to_string(key);
        } else {
            string ker = to_string(key);
            comand = "I" + ker + "whoop whoop " + ker;
        }
    
    
    output_file1 << comand << "\n";

    cases++;

}
output_file1.close();
}
};

