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
#include <ctime>
#include <cassert>
#include <pthread.h>
#include <iostream>
#include "alllocks.h"
using namespace std;

struct threadResponse {
    int tstart;                 // for thread output
    int tend;                   // for thread output
    int throughput;             // for thread output
    int nprocs;                 // for thread input
    double seconds;             // for thread input
    abstractlock *lock;         // for thread input
    void *other;                // for thread input
    volatile int *waiting1;     // for thread synchronization
    volatile int *waiting2;     // for thread synchronization
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
    
    int t = clock();
    resp->tstart = t;
    
    // execute trials until TRIAL_MS has elapsed
    // (check only once every 256 lock acquisitions for efficiency)
    int done = 0;
    while ((done % 256) || (clock()-t) < resp->seconds*CLOCKS_PER_SEC) {

        resp->lock->acquire();
        resp->lock->release();
        done++;

    }
    
    barrier_wait(resp->waiting2, resp);
    
    resp->tend = clock();
    resp->throughput = done;
    
    pthread_exit(NULL);
}

void *microbench_busywait_thread_fn(void *arg) {
    struct threadResponse *resp = (struct threadResponse*) arg;
    int delayms = *((int*) resp->other);
    
    barrier_wait(resp->waiting1, resp);
    
    int t = clock();
    resp->tstart = t;
    
    // execute trials until TRIAL_MS has elapsed
    // (check only once every 256 lock acquisitions for efficiency)
    int done = 0;
    while ((clock()-t) < resp->seconds*CLOCKS_PER_SEC) {

        resp->lock->acquire();
        
        //cout<<"waiting for delay time "<<delayms<<endl;
        int delay_start_time = clock();
        while ((clock()-delay_start_time)*1000./CLOCKS_PER_SEC < delayms);
        
        resp->lock->release();
        done++;

    }
    
    barrier_wait(resp->waiting2, resp);
    
    resp->tend = clock();
    resp->throughput = done;
    
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
    microbench(int nprocs, double seconds, abstractlock *lock, void* (*fn)(void*), void* other_input) {
        this->nprocs = nprocs;
        this->seconds = seconds;
        this->lock = lock;
        this->fn = fn;
        this->other_input = other_input;
        this->threads = new pthread_t[nprocs];
        this->responses = new struct threadResponse[nprocs];
    }
    ~microbench() {
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
        for (int i=0;i<nprocs;i++) {
            responses[i].nprocs = nprocs;
            responses[i].seconds = seconds;
            responses[i].lock = lock;
            responses[i].other = other_input;
            responses[i].waiting1 = &waiting1;
            responses[i].waiting2 = &waiting2;
            DEBUG cout<<"creating thread 1"<<endl;
            pthread_create(&threads[i], NULL, fn, &responses[i]);
        }
        
        // get thread output
        for (int i=0;i<nprocs;i++) {
            pthread_join(threads[i], NULL);
        }
    }
    vector<int> getStartTimes() {
        vector<int> results;
        for (int i=0;i<nprocs;i++) results.push_back(responses[i].tstart);
        return results;
    }
    vector<int> getEndTimes() {
        vector<int> results;
        for (int i=0;i<nprocs;i++) results.push_back(responses[i].tend);
        return results;
    }
    vector<double> getThroughputs() {
        vector<double> results;
        for (int i=0;i<nprocs;i++) {
            results.push_back((double)responses[i].throughput /
            ((responses[i].tend-responses[i].tstart)/(double)CLOCKS_PER_SEC));
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

