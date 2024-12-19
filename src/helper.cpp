#include "helper.h"

double clamp(double value, double upper, double lower)
{
    if(value > upper)
        return upper;
    else if(value < lower)
        return lower;

    return value;
}