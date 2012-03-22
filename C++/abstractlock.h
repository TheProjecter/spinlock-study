/* 
 * File:   abstractlock.h
 * Author: trbot
 *
 * Created on March 20, 2012, 9:10 PM
 */

#ifndef ABSTRACTLOCK_H
#define	ABSTRACTLOCK_H

class abstractlock {
public:
    abstractlock() {
    }
    void acquire(void) {
    }
    void release(void) {
        // precondition: the calling process must hold the lock
    }
};

#endif	/* ABSTRACTLOCK_H */

