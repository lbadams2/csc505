#include <iostream>

using namespace std;

int numVals = 0;

int* readInput() {
    int* dims;
    int tmp = 0;
    bool firstNum = true;
    while(scanf("%d", &tmp) != -1) {
        if(firstNum) {
            numVals = tmp + 1;
            dims = new int(numVals);
            firstNum = false;
            continue;
        }
        *dims = tmp;
        dims++;
    }
    return dims;
}

long calculateCost(int* dims, int start, int end) {
    if(end - start <= 1)
        return 0;
    long cost = 0;
    int minIndex = 0, minVal = -1;
    if(end - start > 2) {
        for(int i = start + 1; i < end - 1; i++) {
            int tmp = dims[i];
            if(minVal == -1 || tmp < minVal) {
                minVal = tmp;
                minIndex = i;
            }
        }
    } else {
        minIndex = start + 1;
    }
    cost = dims[start] * dims[minIndex] * dims[end];
    cost = cost + calculateCost(dims, start, minIndex) + calculateCost(dims, minIndex + 1, end);
    return cost;
}


int main(int argc, char** argv) {
    int* dims = readInput();
    long cost = calculateCost(dims, 0, numVals - 1);
    delete dims;
    cout << cost << '\n';
    return 0;
}