/* 
 * File:   timer_test.cpp
 * Author: trbot
 *
 * Created on March 25, 2012, 12:53 AM
 */

#include "alllocks.h"
#include <iostream>
#include <cmath>
using namespace std;

int delay(int amt) {
    int c = 0;
    for (int i=0;i<amt;i++) {
        c += i;
    }
    return c;
}

int main(int argc, char** argv) {
    double t1, t2;
    
    t1 = time_sec();
    int c = delay(1000000000);
    t2 = time_sec();
    
    cout<<"dummy: "<<c<<endl;
    cout<<"time : "<<(t2-t1)<<"s"<<endl;
    return 0;
}

