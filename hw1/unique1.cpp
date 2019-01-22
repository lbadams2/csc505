#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int len;

const vector<int>* readInputFromFile() {
    vector<int> *sequence = new vector<int>();
    ifstream infile("input-5.txt");
    string line;
    int i = 0;
    while(getline(infile, line)) {
        if(i == 0) {
            i++;
            len = stoi(line);
            continue;
        }
        stringstream ss(line);
        string item;
        while(getline(ss, item, ' ')) {
            int elem = stoi(item);
            sequence->push_back(elem);
        }
    }
    return sequence;
}

const vector<int>* readInput() {
    int i = 0;
    vector<int> *sequence = new vector<int>();
    for(string line; getline(cin, line);) {
        if(i == 0) {
            i++;
            len = stoi(line);
            continue;
        }
        stringstream ss(line);
        string item;
        while(getline(ss, item, ' ')) {
            int elem = stoi(item);
            sequence->push_back(elem);
        }
    }
    return sequence;
}

const pair<int, int>* getLongestSubsequence(const vector<int> *sequence) {
    pair<int, int> *output = new pair<int, int>();
    int size = sequence->size();
    vector<int> longestSubsequence;
    int startIndex = 0;
    for(int i = 0;  i < size; i++) {
        vector<int> currentSubsequence;
        for(int j = i; j < size; j++) {
            int item = sequence->at(j);
            bool foundDup = false;
            for(int subItem: currentSubsequence)
                if(item == subItem) {
                    foundDup = true;
                    break;
                }
            if(foundDup) {
                if(currentSubsequence.size() >= longestSubsequence.size()) {
                    if(currentSubsequence.size() == longestSubsequence.size()) {
                        int potentialStart = j - currentSubsequence.size();
                        if(potentialStart < startIndex) {
                            longestSubsequence = currentSubsequence;
                            startIndex = j - currentSubsequence.size();    
                        }
                    }
                    else {
                        longestSubsequence = currentSubsequence;
                        startIndex = j - currentSubsequence.size();
                    }
                }
                currentSubsequence.clear();
                continue;
            }
            else
                currentSubsequence.push_back(item);
        }
        // no duplicates in entire sequence
        if(currentSubsequence.size() > longestSubsequence.size()) {
            longestSubsequence = currentSubsequence;
            startIndex = i;
        }
    }
    output->first = startIndex;
    output->second = longestSubsequence.size();
    return output;
}

int main(int argc, char** argv) {
    const vector<int> *sequence = readInputFromFile(); 
    const pair<int, int> *output = getLongestSubsequence(sequence);
    delete sequence;
    cout << output->first << " " << output->second << endl;
    delete output;
}