#include <iostream>

using namespace std;

long numVals = 0;

long* readInput() {
    long* dims;
    long tmp;
    int i = 0;
    bool firstNum = true;
    while(scanf("%ld", &tmp) != -1) {
        if(firstNum) {
            numVals = tmp + 1;
            dims = new long[numVals];
            firstNum = false;
            continue;
        }
        dims[i++] = tmp;
    }
    return dims;
}

long calculateCost(long* dims, long start, long end) {
    if(end - start <= 1)
        return 0;
    long cost = 0;
    long minIndex = 0;
    long minVal = -1;
    if(end - start > 2) {
        for(long i = start + 1; i < end; i++) {
            long tmp = dims[i];
            if(minVal == -1 || tmp < minVal) {
                minVal = tmp;
                minIndex = i;
            }
        }
        
    } else {
        minIndex = start + 1;
    }
    cost = dims[start] * dims[minIndex] * dims[end];
    cost = cost + calculateCost(dims, start, minIndex) + calculateCost(dims, minIndex, end);
    return cost;
}


int main(int argc, char** argv) {
    long* dims = readInput();
    long cost = calculateCost(dims, 0, numVals - 1);
    delete dims;
    cout << cost << '\n';
    return 0;
}
