#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct Node {
    int key;
    int val;
};

void insertValue(Node &node) {

}

Node removeMin() {

}

Node* readInput(int branches) {
    for(string line; getline(cin, line);) {
        if(line == "-1")
            removeMin();
        else {
            stringstream ss(line);
            string item;
            int i = 0;
            Node node;
            while(getline(ss, item, ' ')) {                
                if (i == 0) {
                    node.key = stoi(item);
                    i++;
                }
                else
                    node.val = stoi(item);
                insertValue(node);
            }
        }
    }
}

int main(int argc, char** argv) {
    int branches = 0;
    if(argc > 0) {
        branches = atoi(argv[0]);
    }
    else
        branches = 2;
    readInput(branches);
    return 0;
}