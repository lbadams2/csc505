#include <iostream>

using namespace std;

int numMatrices = 0;

long calculateCost() {
    int tmp = 0;
    int buf[3];
    long cost = 0;
    bool firstNum = true;
    while(scanf("%d", &tmp) != -1) {
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