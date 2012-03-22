//#include "alllocks.h"
#include "microbench.h"

#include <iostream>
using namespace std;

ostream& operator<<(ostream& os, vector<int> v) {
    for (int i=0;i<v.size();i++) os<<(i>0?" ":"")<<v[i];
    return os;
}

ostream& operator<<(ostream& os, vector<double> v) {
    for (int i=0;i<v.size();i++) os<<(i>0?" ":"")<<v[i];
    return os;
}

ostream& operator<<(ostream& os, microbench mb) {
    os<<mb.getStartTimes()<<endl;
    os<<mb.getEndTimes()<<endl;
    os<<mb.getThroughputs()<<endl;
    os<<mb.getTotalThroughput()<<endl;
    return os;
}

int main(int argc, char** argv) {
//    int nprocs = 1;
//    abstractlock l[] = {lock1(), lock2(), lock3(nprocs)};
//    for (int i=0;i<sizeof(l);i++) {
//        l[i].acquire();
//        l[i].release();
//        cout<<"acquired and released lock"<<endl;
//    }
    
    int nprocs = 1;
    double nseconds = 1.0;
    abstractlock l = lock1();
    
//    {
//    microbench mb (nprocs, nseconds, &l, microbench_instant_thread_fn, NULL);
//    mb.run();
//    cout<<mb<<endl;
//    }
    
    
    int delayms = 100;
    microbench mb (nprocs, nseconds, &l, microbench_busywait_thread_fn, &delayms);
    mb.run();
    cout<<mb<<endl;
    
    pthread_exit(0);
}
