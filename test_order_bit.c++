
#include <iostream>
// #include <bits/stdc++.h>
using namespace std;
int main()
{
    unsigned int i = 1;
    char *c = (char*)&i;
    if (*c == 1)
        cout<<"Little endian";
    else
        cout<<"Big endian";
    return 0;
}
 