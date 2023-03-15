#include "DegValues.h"

DegValues & DegValues::operator = (const DegValues &deg)
{
    int i;

    if( this != &deg)
    {
        for(i=0;i<8;i++)
            degValues[i]=deg.degValues[i];

    }
    return *this;
}