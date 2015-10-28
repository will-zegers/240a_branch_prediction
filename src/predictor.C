#include "math.h"
#include "stdlib.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int historyLength = 59;
const static float theta = 1.93 * historyLength + 14;
const static int tableSize = pow(2,6);

char W[tableSize][historyLength];
bool H[historyLength];

int index;
int y;

void shiftH(bool outcome)
{
    for(int i = 0; i < historyLength-1; i++)
    {
        H[i] = H[i+1];
    }
    H[historyLength-1] = outcome;
}

unsigned int boolArrayToInt(bool A[])
{
    int h = 0;
    for(int i = 0; i < 32; i++)
    {
       h |= (int)(H[historyLength-1-i] << i);
    }
    return h;
}

void init_predictor ()
{
    for(int i = 0; i < tableSize; i++)
        for(int j = 0; j < historyLength; j++)
            W[i][j] = 0;

    for(int i = 0; i < historyLength; i++) 
        H[i] = 0;
}

bool make_prediction (unsigned int pc)
{
    y = 0;
    
    index = pc % tableSize;
    for(int i = 0; i < historyLength; i++)
    {
        y += (int)W[index][i] * (int)H[i];
    }

    bool predict = (y > 0) ? TAKEN : NOT_TAKEN;    
    return predict;
}

void train_predictor (unsigned int pc, bool outcome)
{
    int t = outcome ? 1 : -1;

    for(int i = 0; i < historyLength; i++)
    {
        if(((y < 0) != (t < 0)) || abs(y) < theta)
        {
            W[index][i] += H[i] * t;
        }           
    }
    shiftH(outcome);
}
