#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <cmath>
#include <map>
#include <iomanip>
#include <sys/time.h>
#include <ctime>

using namespace std;

void delay(int secs)
{
    for (int i = (time(NULL) + secs); time(NULL) != i; time(NULL))
        ;
}

int main()
{
    // stringstream p;
    // string s = "Diferentes     palabras en un string";

    // p << s;
    // while (p >> s)
    //     cout << s << endl;

    // time_t now = time(nullptr);

    // delay(5);
    // time_t after = time(nullptr);
    // cout << after - now << endl;
    int i;
    for (i = 0; i < 10; i++)
    {
        cout << "e" << endl;
    }
    cout << i << endl;

    return 0;
}