#include <iostream>
#include "util.h"
using namespace std;


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
    return true;
}

void printHeader(const char* title)
{
    void printHeader();
{
    cout << "============================================\n";
    cout << "              THE LAST TRAIN                \n";
    cout << "============================================\n";
    cout << endl;
}
}

void clearScreen()
{
       cout << "\033[2J\033[H";
}