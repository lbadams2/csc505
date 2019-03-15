#include <iostream>

using namespace std;

long numMatrices = 0;

long calculateCost() {
    long tmp = 0;
    long buf[3];
    for(int i = 0; i < 3; i++)
        buf[i] = 0;

    long cost = 0;
    bool firstNum = true;
    while(scanf("%ld", &tmp) != -1) {
        if(firstNum) {
            numMatrices = tmp;
            firstNum = false;
            continue;
        }
        if(buf[0] == 0) 
            buf[0] = tmp;
        else if(buf[1] == 0) 
            buf[1] = tmp;
        else if(buf[2 == 0]) {
            buf[2] = tmp;
            cost += buf[0] * buf[1] * buf[2];
            buf[1] = buf[2];
            buf[2] = 0;
        }
    }
    return cost;
}

int main(int argc, char** argv) {
    long cost = calculateCost();
    cout << cost << '\n';
    return 0;
}