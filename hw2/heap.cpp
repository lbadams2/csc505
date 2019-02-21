#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    int val;

    Node(int key=0, int val=0) : key(key), val(val){}

    bool operator<(const Node& otherNode) const {
        return key < otherNode.key;
    }

    bool operator>(const Node& otherNode) const {
        return key > otherNode.key;
    }

    bool operator==(const Node& otherNode) const {
        return key == otherNode.key;
    }
};

vector<Node> heap;
//int heapSize = 0;
int lgBranches = 0;
int branches = 0;
int heapComparisons = 0;

void minHeapify(int i) {
    int smallest = i;
    int firstChild = 1;
    if(i > 0)
        firstChild = (i << lgBranches) + 1;
    int lastChild = firstChild + (branches - 1);
    
    for(int j = firstChild; j <= lastChild; j++)
        if(j < heap.size()) {
            heapComparisons++;
            if(heap[j] < heap[smallest])
                smallest = j;
        }
    
    if(smallest != i) {
        iter_swap(heap.begin() + i, heap.begin() + smallest);        
        minHeapify(smallest); // keep moving large node down the heap
    }

}

void insertValue(Node &node) {
    //heap.insert(heap.begin() + heapSize++, node);
    heap.push_back(node);
    int startIndex = heap.size() >> 1; // divide by 2, floor division
    for(int i = startIndex; i >= 0; i--)
        minHeapify(i);    
}

Node removeMin() {
    Node min = heap[0];
    //int lastIndex = heapSize - 1;
    int lastIndex = heap.size() - 1;
    iter_swap(heap.begin(), heap.begin() + lastIndex);
    heap.pop_back();
    //heapSize--;
    minHeapify(0);
    return min;
}

void readInput() {
    int key = 0, tmp = 0;
    bool gotKey = false;
    while(scanf("%d", &tmp) != -1) {
        if(!gotKey && tmp == -1) {
            Node min = removeMin();
            cout << min.key << " " << min.val << '\n';
        } else if(!gotKey && tmp != -1) {
            key = tmp;
            gotKey = true;
        } else if(gotKey) {
            Node node;
            node.key = key;
            node.val = tmp;
            insertValue(node);
            gotKey = false;
        } else {
            throw invalid_argument("error reading file");
        }
    }
}

int main(int argc, char** argv) {
    if(argc > 1) {
        branches = atoi(argv[1]);
        if(branches == 0)
            throw invalid_argument("branching factor must be greater than 0");
        if( !( ((branches & (branches-1)) == 0) ) )
            throw invalid_argument("branching factor not power of 2");
        int tmp = branches;
        while(tmp >>= 1 != 0)
            lgBranches++;
    }
    else {
        branches = 2;
        lgBranches = 1;
    }
    readInput();
    cout << "key comparisons: " << heapComparisons << '\n';
    return 0;
}