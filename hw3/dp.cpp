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

long calculateCost(long* dims) {
    long n = numVals;
    long **m = new long*[n];
    for(size_t i = 0; i < n; i++)
        m[i] = new long[n];
    for(int i = 0; i < n; i++)
        m[i][i] = 0;
    
    for(int l = 2; l < n; l++)
        for(int i = 1; i < n-l+1; i++) {
            int j = i + l - 1;
            m[i][j] = LONG_MAX;
            for(int k = i; k < j; k++) {
                long q = m[i][k] + m[k+1][j] + dims[i-1]*dims[k]*dims[j];
                if(q < m[i][j])
                    m[i][j] = q;
            }
        }
    long cost = m[1][n-1];
    for(size_t i = n; i > 0;)
        delete[] m[--i];
    delete[] m;
    return cost;
}

int main(int argc, char** argv) {
    long* dims = readInput();
    long cost = calculateCost(dims);
    delete[] dims;
    cout << cost << '\n';
    return 0;
}