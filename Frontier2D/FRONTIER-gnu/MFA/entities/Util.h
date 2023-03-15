#ifndef MFA_UTILS_H
#define MFA_UTILS_H

#include <math.h>

// simple random numbers by numerical recipies
// ran0 returns a value between 0.0 and 1.0
class Random {
private:
    int   idum;
public:
    Random(int seed) {idum = seed;}

    // numerical recipies p. 279
    float ran0() ;
};

static const int   RAND_IA   = 16807;
static const int   RAND_IM   = 2147483647;
static const float RAND_AM   = 1.0/RAND_IM;
static const int   RAND_IQ   = 127773;
static const int   RAND_IR   = 2836;
static const int   RAND_MASK = 123459876;

//Refere IEEE 745 standard format for 64-bit floating- point number at
//Computer architecture and organization by Hayes (ISBN 0-07-027366-9) p.196
float toFloat(int readed);

#endif