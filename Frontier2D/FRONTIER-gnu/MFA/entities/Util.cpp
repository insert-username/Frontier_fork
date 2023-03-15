#include "entities/Util.h"


float Random::ran0() {
    long int   k;
    float    ans;
    idum ^= RAND_MASK;
    k  = idum/RAND_IQ;
    idum = RAND_IA * (idum - k*RAND_IQ) - RAND_IR*k;
    if (idum < 0) idum += RAND_IM;
    ans = RAND_AM * idum;
    idum ^= RAND_MASK;
    return ans;
}

float toFloat(int readed) {
    int i, exponent;
    float mantissa, floatNumber;
    exponent = (readed>>20)&2047;      // 2^11 - 1 == 2047
    exponent = exponent - 1023;
    mantissa = (readed&1048575);       // 2^20 - 1 == 1048575
    mantissa =mantissa / 1048575.0;    // use 20 bit mantissa

    if(readed&(1<<31))
        floatNumber=-1.0;
    else
        floatNumber=1.0;

    if(exponent>0)
        for(i=0;i<exponent;i++)
            floatNumber=floatNumber*2.0;
    else
        for(i=0;i<-exponent;i++)
            floatNumber=floatNumber/2.0;
    floatNumber=floatNumber*(1.0+mantissa);
    return floatNumber;
}
