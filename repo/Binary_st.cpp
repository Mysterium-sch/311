#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include<cmath>
#include <chrono>
using namespace std;

class node {
    public:
        int key;
        string value;
        node* left = NULL;
        node* right = NULL;
        bool nulll;

        node(int k, string v) {
            key = k;
            value = v;
            nulll = false;
        }
        node () {
            key = -1;
            value = "";
            nulll = true;
        }

    };

 void insert(node*, string, int);
 void remove(node*, int);
 void search();
 bool Keysearcher(int, bool);
 void print(node*);
void remove(node*, int);
int getRand();

const int size = pow(2,22); 
node* root = new node();
int total = 0;
int main(int argc, char **argv)
{ 
    bool hasRoot = false;
    bool working = true;
    std::ifstream input_file;
    string filename;

    cout << "Please input name of ASCII file to sort (please include file extension)\n";
    cin >> filename;
    input_file.open(filename);

    srand (time(0));
    while (working) {

        if ( input_file.is_open() ) {
        string value;
        while ( input_file.good() ) {
            input_file >> value;
            int key = rand()%size;
            if(!(total > size)) {
                total++;
                if(hasRoot) {
                    insert(root, value, key); 
                } else {
                    hasRoot = true;
                    root =new node(key, value);
                    root->left = new node();
                    root->right = new node();
                }   
            } else {
                cout << "Max size reached.\n";
            }
        } 
        } else {
            cout << "Input file is not avaialble";  
        }

    print(root);
    working = false;
}
}


 void insert(node* cur, string value, int key) {
    int holderL = cur->key;
       if (holderL < key ) {
            bool right = cur->right->nulll;
            if (right) {
                cur->right = new node(key, value);
                node* baby = cur->right;
                baby->left = new node();
                baby->right = new node();
            } else {
                insert(cur->right, value, key);
            }
        } else {
            bool left = cur->left->nulll;
            if (left) {
                cur->left = new node(key, value);
                node* baby = cur->left;
                baby->left = new node();
                baby->right = new node();
            } else {
                insert(cur->left, value, key);
            }
        }

}
 
 void remove(node* cur, int value) {

    //Node to deltete is at bottom if tree
    bool done1 = false;
    bool complete = false;
    node* temp1 = root;
    node* prev = new node();
    bool left = false;
    while(!done1) {
        if(temp1->key == value && temp1->left->nulll && temp1->right->nulll) {
            if(left) {
                prev->left = new node();
            } else {
                prev->right = new node();
            }
            done1=true;
            complete = true;
        } else if (temp1->key > value && !temp1->left->nulll) {
            prev = temp1;
            left = true;
            temp1 = temp1->left;
        } else if (temp1->key < value && !temp1->right->nulll) {
            prev = temp1;
            left = false;
            temp1 = temp1->right;
        } else {
            done1 = true;
        }
    }
    //Node to delete has 1 child
    if(!complete) {
    bool done2 = false;
    node* temp2 = root;
    node* prev = new node();
    bool left = false;
    while(!done2) {
        if(root->key == value && (root->left->nulll || root->right->nulll)) {
                if(root->left->nulll) {
                    root = root->right;
                } else {
                    root = root->left;
                }
        }
        else if(temp2->key == value && (temp2->left->nulll || temp2->right->nulll)) {
            if(temp2->left->nulll) {
                if(left) {
                    prev->left = temp2->right;
                } else {
                    prev->right = temp2->right;
                }
                
            } else {
                if(left) {
                    prev->left = temp2->left;
                } else {
                    prev->right = temp2->left;
                }
            }
            done2=true;
            complete = true;
        } else if (temp2->key > value && !temp2->left->nulll) {
            prev = temp2;
            left = true;
            temp2 = temp2->left;
        } else if (temp2->key < value && !temp2->right->nulll) {
            prev = temp2;
            left = false;
            temp2 = temp2->right;
        } else {
            done2 = true;
        }
    }
    }

    //Other
    if(!complete) {
         bool done3 = false;
         node* temp3 = root;

        while(!done3) {
        if(temp3->key == value) {
            node* cur = temp3->right;
            node* prevmin = temp3;
          while (cur && !cur->left->nulll) {
            prevmin = cur;
            cur = cur->left;
        }
            prevmin->left = new node();
            temp3->value = cur->value;
            temp3->key = cur->key;
            done1=true;
            complete = true;
        } else if (temp3->key > value && !temp3->left->nulll) {
            temp3 = temp3->left;
        } else if (temp3->key < value && !temp3->right->nulll) {
            temp3 = temp3->right;
        } else {
            done3 = true;
        }

  
    }

    }
 }
 
 void search() {
    int key;
    bool found;
        cout << "What key are you looking for?\n";
        cin >> key;
        found = Keysearcher(key, false);

    if(found) {
        cout << key << " exists within the binary tree.\nHere is the path traversed:\n";
    } else {
        cout << key << " does not exists within the binary tree.\nHere is the path traversed:\n";
    }
}

bool Keysearcher(int key, bool printer) {
bool done = false;
    node cur = *root;
    while(!done) {
        int curV = cur.key;
        if (cur.key == key) {
            if(printer) cout << key << "\n";
            return true;
        } else if (cur.key < key && !cur.right->nulll) {
            if(printer) cout << cur.key << " ";
            cur = *cur.right;
        } else if (cur.key > key  && !cur.left->nulll) {
            if(printer) cout << cur.value << " ";
            cur = *cur.left;
        } else {
            done = true;
        }

    }
    return false;
}

void print(node* cur)
{
    if (cur->nulll)
        return;
 
    print(cur->left);
    cout << '(' << cur->key << ',' << cur->value << ") \n";
    print(cur->right);
}
