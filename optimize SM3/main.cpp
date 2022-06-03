#include <string.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "sm3.h"
using namespace std;

int main(int argc, char* argv[])
{
    DWORD star_time = GetTickCount();
    auto* input = (unsigned char*)"abc";   // input
    int iplen = 3;                   // input length
    unsigned char output[32];       // output
    int i;
    sm3_context ctx;

    cout << "Message:" << endl;
    cout << input << endl;

    sm3(input, iplen, output);
    cout << "Hash:" << endl;
    for (i = 0; i < 32; i++)
    {
        printf("%02x", output[i]);
        if (((i + 1) % 4) == 0) printf(" ");
    }
    printf("\n");
    DWORD end_time = GetTickCount();
    cout << "The total time is:" << (end_time - star_time) << "ms." << endl;
}