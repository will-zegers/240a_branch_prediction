//#include "math.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int h = 18;
const static int n = 18;
const static int b = 8;
const static float theta = h/2; 
const static int min_weight = -(1 << (b-1));
const static int max_weight = -min_weight - 1;

bool G[h+1];
bool SG[h+1];
int W[n][h+1];
int R[h+1];
int SR[h+1];
int v[h+1];
int sv[h+1];

int index;
int y;

void init_predictor ()
{
    for(int i = 0; i < h+1; i++) {
        G[i]  = false;
        SG[i] = false;
        R[i]  = 0;
        SR[i] = 0;
        v[i]  = 0;
        sv[i] = 0;
        for(int j = 0; j < n; j++)
            W[i][j] = 0;
}

bool make_prediction (unsigned int pc)
{
    return false;
}

void train_predictor (unsigned int pc, bool outcome)
{
}
