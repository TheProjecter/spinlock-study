/* 
 * File:   microbench1.h
 * Author: trbot
 *
 * Created on March 20, 2012, 9:21 PM
 */

#ifndef MICROBENCH1_H
#define	MICROBENCH1_H

#define DEBUG if(0)

#include <vector>
#include <cassert>
#include <pthread.h>
#include <iostream>
#include "alllocks.h"
using namespace std;

struct threadResponse {
    double tstart;              // for thread output
    double tend;                // for thread output
    int acquireCount;           // for thread output
    int nprocs;                 // for thread input
    double seconds;             // for thread input
    abstractlock *lock;         // for thread input
    void *other_in;             // for thread input
    volatile int *waiting1;     // for thread synchronization
    volatile int *waiting2;     // for thread synchronization
    int prevent_optimizing;     // dummy output var. to prevent compiler from optimizing out code
};

void barrier_wait(volatile int *wait_var, struct threadResponse *resp) {
    // quick and dirty barrier implementation so threads can start/stop together
    (*wait_var)++;
    DEBUG cout<<"wait_var="<<(*wait_var)<<" nprocs="<<resp->nprocs<<endl;
    while (*wait_var < resp->nprocs);
}

void *microbench_instant_thread_fn(void *arg) {
    struct threadResponse *resp = (struct threadResponse*) arg;
    
    barrier_wait(resp->waiting1, resp);
    
    double t = time_sec();
    resp->tstart = t;
    
    // execute trials until TRIAL_MS has elapsed
    // (check only once every 256 lock acquisitions for efficiency)
    int done = 0;
    while ((done % 256) || (time_sec()-t) < resp->seconds) {

        resp->lock->acquire();
        resp->lock->release();
        done++;

    }
    
    barrier_wait(resp->waiting2, resp);
    
    resp->tend = time_sec();
    resp->acquireCount = done;
    
    pthread_exit(NULL);
}

void *microbench_busywaitms_thread_fn(void *arg) {
    struct threadResponse *resp = (struct threadResponse*) arg;
    double delay_sec = *((double*) resp->other_in);
    
    barrier_wait(resp->waiting1, resp);
    
    double t = time_sec();
    resp->tstart = t;
    
    // execute trials until TRIAL_MS has elapsed
    int done = 0;
    while ((time_sec()-t) < resp->seconds) {

        resp->lock->acquire();
        
        // wait for at least "delay_sec" seconds
        double delay_start_time = time_sec();
        for (int i=0;(time_sec()-delay_start_time) < delay_sec;i++) {
            (resp->prevent_optimizing) += i;
        }
        
        resp->lock->release();
        done++;

    }
    
    barrier_wait(resp->waiting2, resp);
    
    resp->tend = time_sec();
    resp->acquireCount = done;
    
    pthread_exit(NULL);
}

void *microbench_busywait_thread_fn(void *arg) {
    struct threadResponse *resp = (struct threadResponse*) arg;
    int delay = *((int*) resp->other_in);
    
    barrier_wait(resp->waiting1, resp);
    
    double t = time_sec();
    resp->tstart = t;
    
    // execute trials until TRIAL_MS has elapsed
    int done = 0;
    while ((time_sec()-t) < resp->seconds) {

        resp->lock->acquire();
        
        // wait for O(delay) cycles
        for (int i=0;i<delay;i++) {
            (resp->prevent_optimizing) += i;
        }
        
        resp->lock->release();
        done++;

    }
    
    barrier_wait(resp->waiting2, resp);
    
    resp->tend = time_sec();
    resp->acquireCount = done;
    
    pthread_exit(NULL);
}

class microbench {
private:
    volatile int waiting1; // for thread sync (serves as a simple barrier)
    volatile int waiting2; // for thread sync (serves as a simple barrier)

    int nprocs;
    double seconds;
    abstractlock *lock;
    void* (*fn)(void*);
    void* other_input;
    pthread_t *threads;
    struct threadResponse *responses;
public:
    microbench(int _nprocs, double _seconds, abstractlock *_lock, void* (*_fn)(void*), void* _other_input) {
        nprocs = _nprocs;
        seconds = _seconds;
        lock = _lock;
        fn = _fn;
        other_input = _other_input;
        threads = new pthread_t[nprocs];
        responses = new struct threadResponse[nprocs];
    }
    void cleanup() {
        delete[] threads;
        delete[] responses;
    }
    void run() {
        // initialize barriers
        waiting1 = 0;
        waiting2 = 0;
        
        // create (and start) threads
        // note: threads will wait on barriers to start + stop together
        DEBUG cout<<"creating threads..."<<endl;
        int prevent_optimizing = 0;
        for (int i=0;i<nprocs;i++) {
            responses[i].nprocs = nprocs;
            responses[i].seconds = seconds;
            responses[i].lock = lock;
            responses[i].other_in = other_input;
            responses[i].waiting1 = &waiting1;
            responses[i].waiting2 = &waiting2;
            responses[i].prevent_optimizing = 0;
            DEBUG cout<<"creating thread 1"<<endl;
            pthread_create(&threads[i], NULL, fn, &responses[i]);
        }
        
        // get thread output
        for (int i=0;i<nprocs;i++) {
            pthread_join(threads[i], NULL);
            prevent_optimizing += responses[i].prevent_optimizing;
        }
        cout<<"dummy output preventing code optimization: "<<prevent_optimizing<<endl;
    }
    vector<double> getStartTimes() {
        vector<double> results;
        for (int i=0;i<nprocs;i++) results.push_back(responses[i].tstart);
        return results;
    }
    vector<double> getEndTimes() {
        vector<double> results;
        for (int i=0;i<nprocs;i++) results.push_back(responses[i].tend);
        return results;
    }
    vector<double> getElapsedTimes() {
        vector<double> results;
        for (int i=0;i<nprocs;i++) results.push_back(responses[i].tend-responses[i].tstart);
        return results;
    }
    vector<double> getThroughputs() {
        vector<double> results;
        for (int i=0;i<nprocs;i++) {
            results.push_back((double) responses[i].acquireCount /
            (responses[i].tend - responses[i].tstart));
        }
        return results;
    }
    double getTotalThroughput() {
        vector<double> results = getThroughputs();
        double result = 0;
        for (int i=0;i<nprocs;i++) {
            result += results[i];
        }
        return result;
    }
};

#endif	/* MICROBENCH1_H */

