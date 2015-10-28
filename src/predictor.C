//#include "math.h"

#define TAKEN     true
#define NOT_TAKEN false
#define BUDGET    32768 // (32Kb)

const static int h = 31;

//const static float theta = 1.93 * h + 14;
//const static int n = pow(2,floor(log2(BUDGET/(h * 8))));
const static float theta = h/2; 
const static int n = 128;

char W[n][h+1];
int H;

int index;
int y;

void init_predictor ()
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < h; j++)
            W[i][j] = 0;

    H = 0;
}

bool make_prediction (unsigned int pc)
{
    index = pc % n;

    y = W[index][0];
    for(int i = 1; i < h+1; i++)
    {
        y += W[index][i] * (H >> (h - i) & 1); 
    }

    return (y > 0) ? TAKEN : NOT_TAKEN;
}

void train_predictor (unsigned int pc, bool outcome)
{
    int t = (outcome == TAKEN) ? 1 : -1;

    if(((y < 0) != (t < 0)) || (y < theta && y > -theta))
    {
        W[index][0] += t;
        for(int i = 1; i < h+1; i++)
        {
            W[index][i] += (H >> (h - i) & 1)*t;
        }           
    }
    H <<= 1;
    H += (int)outcome;
}
