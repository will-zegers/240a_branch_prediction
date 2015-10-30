#include "stdlib.h"
#include "stdio.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int n = 128;
const static int h = 15;
const static float theta = h/2; //2.14 * (h+1) + 20.58;

int W[n][h+1];
bool SG[h+1];
bool G[h+1];
int SR[h+1];
int R[h+1];
int v[h+1];
int sv[h+1];

typedef struct {
    int y;
    int i;
    int v[h+1];
    bool H[h+1];
    bool predict;
} Branch;

Branch B;

int satCnt(int w, bool inc) {

    if(inc)
        if(w < 127) w++;
    else
        if(w > -128) w--;

    return w;
}

void shiftArray(bool *A, bool b) {

    for(int j = h; j > 0; j--) {
        A[j] = A[j-1];
    }
    A[0] = b;
}

void shiftArray(int *A, int x) {

    for(int j = h; j > 0; j--) {
        A[j] = A[j-1];
    }
    A[0] = x;
}

void init_predictor ()
{
    for(int j = 0; j < h+1; j++) {
        SR[j] = 0;
        R[j]  = 0;
        G[j]  = false;
        SG[j] = false;
        v[j]  = 0;

        for(int k = 0; k < n; k++)
            W[j][k] = 0;
    }
}

bool make_prediction (unsigned int pc)
{
    int i = pc % n;

    shiftArray(sv,i);

    for(int j = 0; j < h + 1; j++) {
        B.v[j] = sv[j];
        B.H[j] = SG[j];
    }

    int y = SR[h] + W[i][0];
    
    bool predict = (y >= 0);
    
    for(int j = 1; j < h + 1; j++) {
        int k = h - j;
        if(predict == TAKEN)
            SR[k+1] = SR[k] + W[i][j];
        else
            SR[k+1] = SR[k] - W[i][j];
    }

    SR[0] = 0;
    shiftArray(SG, predict);

    B.y = y;
    B.i = i;
    B.predict = predict;

    return predict;
}

void train_predictor (unsigned int pc, bool outcome)
{

    int i = B.i;
    int y = B.y;

    int k;
    for(int j = 1; j < h + 1; j++) {
        k = h - j;
        if(outcome == TAKEN)
            R[k+1] = R[k] + W[i][j];
        else
            R[k+1] = R[k] - W[i][j];
    }
    R[0] = 0;

    shiftArray(G,outcome);
    shiftArray(v,B.i);

    if(B.predict != outcome) {
        for(int j = 0; j < h + 1; j++) {
            SR[j] = R[j];
            SG[j] = G[j];
            sv[j] = v[j];
        }
    }

    if(B.predict != outcome || (y < theta && y > -theta)) {
        W[i][0] = satCnt(W[i][0],outcome); 
        for(int j = 1; j < h + 1; j++) {
            k = B.v[j];
            W[k][j] = satCnt(W[k][j],outcome == B.H[j]); 
        }
    }       
}
