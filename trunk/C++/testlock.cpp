//#include "alllocks.h"
#include "microbench.h"

#include <iostream>
#include <cmath>
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
    os<<"elapsed times : "<<mb.getElapsedTimes()<<endl;
    os<<"throughputs   : "<<mb.getThroughputs()<<endl;
    os<<"total         : "<<mb.getTotalThroughput()<<endl;
    return os;
}

struct experiment {
    int nprocs;
    double nseconds;
    abstractlock lock;
    double lock_hold_time_sec;
    void *(*microbench_thread_fn)(void *);
    
    experiment(int _nprocs, double _nseconds, abstractlock _lock,
                double _lock_hold_time_sec, void *(*_fn)(void*)) {
        nprocs = _nprocs;
        nseconds = _nseconds;
        lock = _lock;
        lock_hold_time_sec = _lock_hold_time_sec;
        microbench_thread_fn = _fn;
    }
};

int main(int argc, char** argv) {
    
    vector<experiment> exps;
    exps.push_back(experiment(1, 1.0, lock1(), 0, microbench_busywaitms_thread_fn));
    for (int i=8;i>=0;i--) {
        exps.push_back(experiment(1, 1.0, lock1(), pow(10, -i), microbench_busywaitms_thread_fn));
    }
    
    for (int i=0;i<exps.size();i++) {
        microbench mb (exps[i].nprocs,
                       exps[i].nseconds,
                      &exps[i].lock,
                       exps[i].microbench_thread_fn,
                      &exps[i].lock_hold_time_sec);
        mb.run();
        cout<<mb<<endl;
        mb.cleanup();
    }
    
    pthread_exit(0);
}
