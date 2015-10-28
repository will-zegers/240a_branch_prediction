//#include "math.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int historyLength = 32;

//const static float theta = 1.93 * historyLength + 14;
//const static int tableSize = pow(2,floor(log2(BUDGET/(historyLength * 8))));
const static float theta = historyLength/2; 
const static int tableSize = 128;

char W[tableSize][historyLength];
int H;

int index;
int y;

void init_predictor ()
{
    for(int i = 0; i < tableSize; i++)
        for(int j = 0; j < historyLength; j++)
            W[i][j] = 0;

    H = 0;
}

bool make_prediction (unsigned int pc)
{
    y = 0;
    
    index = pc % tableSize;
    for(int i = 0; i < historyLength; i++)
    {
        y += W[index][i] * (H >> (31 - i) & 1); 
    }

    bool predict = (y > 0) ? TAKEN : NOT_TAKEN;    
    return predict;
}

void train_predictor (unsigned int pc, bool outcome)
{
    int t = (outcome == TAKEN) ? 1 : -1;

    for(int i = 0; i < historyLength; i++)
    {
        if(((y < 0) != (t < 0)) || (y < theta && y > -theta))
        {
            W[index][i] += (H >> (31 - i) & 1)*t;
        }           
    }
    H <<= 1;
    H += (int)outcome;
}
