// A very stupid predictor.  It will always predict not taken.
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

#define DEBUG 0 
#define TAKEN     true
#define NOT_TAKEN false

#define BUDGET 15 // 2^15 Bytes (32K)

const static int historyLength = 59;
const static float theta = 1.93 * theta + 14;
const static int tableSize = pow(2,6);
char W[tableSize][historyLength] = {0};
bool H[historyLength] = {0};

int index;
bool predict;
int y;

void init_predictor ()
{
}

void shiftH(bool outcome) {
    for(int i = 0; i < historyLength-1; i++) {
        H[i] = H[i+1];
    }
    H[historyLength-1] = outcome;
}

bool make_prediction (unsigned int pc)
{
    y = 0;
    
    index = pc % tableSize;
    for(int i = 0; i < historyLength; i++) {
        y += (int)W[index][i] * (int)H[i];
    }

    predict = (y > 0) ? TAKEN : NOT_TAKEN;    
    return predict;
}

void train_predictor (unsigned int pc, bool outcome)
{
    int t = outcome ? 1 : -1;

    for(int i = 0; i < historyLength; i++) {
        if(((y < 0) != (t < 0)) || abs(y) < theta) {
            W[index][i] += H[i] * t;
        }           
    }
    shiftH(outcome);
}
