#include "util.h"

// Primitive helper utilities, clamping, and screen control
int clampInt(int val, int minVal, int maxVal)
{
    if(val<minVal)
    return minVal;

    if(val>maxVal)
    return maxVal;

    return val;

}



void trimWhitespace(char* str)
{
    
    // Implementation for trimming whitespacea
}

bool stringEquals(const char* a, const char* b)
{
    // Implementation for comparing strings
}

void printHeader(const char* title)
{
    // Implementation for printing a header
}

void clearScreen()