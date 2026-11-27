#include <iostream>
#include<cctype>
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

   //check for null pointers
     if(str==NULL)
    {
     return;
    }

    //initialize start;
    int start=0; 
    int end=0;

   //increment start until a caharter is found
    while(str[start]!='\0' && isspace((unsigned char)str[start]))
    {
        start++;
    }

    //increment end until the ending of the string
    while (str[start+end]!='\0')
    {
       end++;
    }

    //decreemnt end, check string from backwards until a character is found
    while(isspace((unsigned char)str[start+end-1]) && end>0)
    {
        end--;
    }

    //now end stores the index-1 where last character is the string is present
    //start stores the index+1 where begining character is present
    int i=0;
    while(i<end)
    {
        str[i]=str[start+i];
        i++;
    }

    //set the ith index null
    str[i]='\0';


}

bool stringEquals(const char* a, const char* b)
{
    int i=0;

    while(a[i]!='\0' && b[i]!='\0' )
    {
        if(a[i]!=b[i])
        return false;
        i++;
    }

    return a[i]=='\0' && b[i]=='\0';
}

void printHeader(const char* title)
{

    cout << "============================================\n";
    cout << "              " << title << "                \n";
    cout << "============================================\n";
    cout << endl;

}

void clearScreen()
{
       cout << "\033[2J\033[H";
}