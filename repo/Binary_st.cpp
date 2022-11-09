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

void input(string);
bool insert(int, string);
void insert_real(node*, string, int);
bool remove(int);
void remove_real(node*, int);
bool Stringsearch(string);
void StringSearcher_real(node*, string);
bool Keysearcher(int);
void printer();
void print_real(node*);

const int size = pow(2,22);
bool found = false; 
bool hasRoot = false;
node* root = new node();
int total = 0;

class BinaryTree {
public:
    BinaryTree() {
    }

void input(string filename)
{ 
    bool working = true;
    std::ifstream input_file;

    input_file.open(filename);

    srand (time(0));
    while (working) {

        if ( input_file.is_open() ) {
        string value;
        while ( input_file.good() && getline( input_file, value ) ) {
            int key = rand()%size;
            if(!(total > size)) {
                if (insert(key, value)) {
                    total++;
                }    
            } else {
                cout << "Max size reached.\n";
            }
        } 
        } else {
            cout << "Input file is not avaialble";  
        }
    working = false;
}
    input_file.close();
}

bool insert(int key, string value) {
    if(!(Keysearcher(key))) {
        if(hasRoot) {
        insert_real(root, value, key);
        } else {
            hasRoot = true;
            root =new node(key, value);
            root->left = new node();
            root->right = new node();
        }
        return true;
    } else {
        return false;
    }
}
void insert_real(node* cur, string value, int key) {
    int holderL = cur->key;
       if (holderL < key ) {
            bool right = cur->right->nulll;
            if (right) {
                cur->right = new node(key, value);
                node* baby = cur->right;
                baby->left = new node();
                baby->right = new node();
            } else {
                insert_real(cur->right, value, key);
            }
        } else {
            bool left = cur->left->nulll;
            if (left) {
                cur->left = new node(key, value);
                node* baby = cur->left;
                baby->left = new node();
                baby->right = new node();
            } else {
                insert_real(cur->left, value, key);
            }
        }

}
 
 bool remove(int v) {
    if(Keysearcher(v)) {
        remove_real(root, v);
        return true;
    } else {
        return false;
    }
 }
 void remove_real(node* cur, int value) {

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

bool Keysearcher(int key) {
    if(hasRoot) {
    bool done = false;
    node cur = *root;
    while(!done) {
        int curV = cur.key;
        if (cur.key == key) {
            return true;
        } else if (cur.key < key && !cur.right->nulll) {
            cur = *cur.right;
        } else if (cur.key > key  && !cur.left->nulll) {
            cur = *cur.left;
        } else {
            done = true;
        }

    }
    }
    return false;
}

void printer() {
    if(hasRoot)
        print_real(root);
}

void print_real(node* cur)
{
    if (cur->nulll)
        return;
 
    print_real(cur->left);
    cout << '(' << cur->key << ',' << cur->value << ") \n";
    print_real(cur->right);
}

bool StringSearcher(string value) {
    if(hasRoot) {
    found = false;
    StringSearcher_real(root, value);
    return found;
    }
    return false;
}
void StringSearcher_real(node* cur, string value) {
    if (cur->value.compare(value) == 0) {
        found = true;
    } 
    
    if(!cur->left->nulll)
        StringSearcher_real(cur->left, value);
    if(!cur->right->nulll)
        StringSearcher_real(cur->right, value);

}
};
