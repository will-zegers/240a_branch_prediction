#include "stdint.h"
#include "math.h"

#define TAKEN     true
#define NOT_TAKEN false

#define MEM_BUDGET  15 // 32KB (2^15)

const static short bhtSize = pow(2,MEM_BUDGET - 2);
unsigned short BHT[bhtSize];
unsigned int GHR;
unsigned int index;

void init_predictor ()
{
    for (int i = 0; i < bhtSize; i++) {
        BHT[i] = 0;
    }
    GHR = 0;
}

bool make_prediction (unsigned int pc)
{
    index = pc ^ GHR;
    index %= bhtSize;
    return (BHT[index] > 1);
}

void train_predictor (unsigned int pc, bool outcome)
{
    GHR <<= 1;
    if (outcome == TAKEN && BHT[index] < 3) {
        BHT[index]++;
        GHR++;
    } else if (outcome == NOT_TAKEN && BHT[index] > 0) {
        BHT[index]--;      
    }
}
