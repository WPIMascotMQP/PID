#include <iostream>
#include <thread>
#include <stdio.h>
using namespace std;

//test fn
void PID1(int Z)
{
    cout << "PID simulation servo1, the loops will go here\n";
}
void PID2(int Z)
{
    cout << "PID simulation servo2, the loops will go here\n";
}

int main()
{
    int setpoint = 10;
    thread M1(PID1, setpoint);

    //thread launched by using function object as callable, 3 in para input
    thread M2(PID2, setpoint);


    // Wait for the threads to finish
    M1.join();
    M2.join();
    //End
    return 0;
}