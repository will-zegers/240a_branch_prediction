//#include "math.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int h = 20;
const static int n = 188;
const static float theta = h/2; //2.14 * (h+1) + 20.58;

static bool H[h+1];
static bool G[h+1];
static bool SG[h+1];
static int W[n][h+1];
static int R[h+1];
static int SR[h+1];
static int v[h+1];
static int sv[h+1];

static int i;
static int y;
static bool predict;

void shiftBool(bool *A, bool b) {
    for(int j = 1; j < h; j++) {
        A[j+1] = A[j];
    }
    A[0] = b;
}

void shiftInt(int *A, int x) {
    for(int j = 1; j < h; j++) {
        A[j+1] = A[j];
    }
    A[0] = x;
}

void init_predictor ()
{
    for(int j = 0; j < h+1; j++) {
        G[j]  = false;
        SG[j] = false;
        R[j]  = 0;
        SR[j] = 0;
        v[j]  = 0;
        sv[j] = 0;
        H[j]  = 0;
        for(int k = 0; k < n; k++)
            W[j][k] = 0;
    }
}

bool make_prediction (unsigned int pc)
{
    i = pc % n;

    shiftInt(sv,i);

    for(int j = 0; j < h + 1; j++) {
        v[j] = sv[j];
        H[j] = SG[j];
    }

    y = SR[h] + W[i][0];
    
    predict = (y >= 0);
    
    for(int j = 1; j < h + 1; j++) {
        int k = h - j;
        if(predict == TAKEN)
            SR[k+1] = SR[k] + W[i][j];
        else
            SR[k+1] = SR[k] - W[i][j];
    }

    SR[0] = 0;
    shiftBool(SG, predict);

    return predict;
}

void train_predictor (unsigned int pc, bool outcome)
{
    for(int j = 1; j < h + 1; j++) {
        int k = h - j;
        if(outcome == TAKEN)
            R[k+1] = R[k] + W[i][j];
        else
            R[k+1] = R[k] - W[i][j];
    }
    R[0] = 0;

    shiftBool(G,outcome);
    shiftInt(v,i);

    if(predict != outcome) {
        for(int j = 0; j < h + 1; j++) {
            SR[j] = R[j];
            SG[j] = G[j];
            sv[j] = v[j];
        }
    }
    
    int t = (outcome == TAKEN) ? 1 : -1;
    if(predict != outcome || (y < theta && y > -theta)) {
        W[i][0] = W[i][0] + t; 
        for(int j = 1; j < h + 1; j++) {
            int k = v[j];
            int s = (outcome == H[j]) ? 1 : -1;
            W[k][j] = W[k][j] + s; 
        }
    }       
}
