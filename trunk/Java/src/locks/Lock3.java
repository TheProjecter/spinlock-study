package locks;

import csc2227proj.Platform;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

/**
 *
 * @author trbot
 */
public class Lock3 implements AbstractLock {
    
    private static final int HAS_LOCK = 1;
    private static final int MUST_WAIT = 0;
    
    // updater to perform CAS on ``next_slot''
    private final static AtomicIntegerFieldUpdater<Lock3> up =
            AtomicIntegerFieldUpdater.newUpdater(Lock3.class, "next_slot");
    
    // represents the process that currently has the right to hold the lock
    private volatile int next_slot = 0;
    private AtomicIntegerArray slots;
    
    private int my_place;
    
    private int nprocs;
    
    public Lock3(final int _nprocs) {
        assert(_nprocs > 0);
        nprocs = _nprocs;
        // note: following has the side-effect of initializing entries to MUST_WAIT
        slots = new AtomicIntegerArray(nprocs*Platform.SLOT_STEP_BOOL);
        slots.set(0, HAS_LOCK);
    }
    
    @Override
    public void acquire() throws Exception {
        int _my_place = up.getAndIncrement(this);
        if ((_my_place % nprocs) == 0) {
            up.getAndAdd(this, -nprocs);
        }
        _my_place %= nprocs;
        while (slots.get(_my_place) == MUST_WAIT);
        slots.set(_my_place, MUST_WAIT);
        
        // now that we've obtained the lock, we can use var. my_place safely
        my_place = _my_place;
    }
    
    @Override
    public void release() {
        // precondition: the calling process must hold the lock
        // note: this "my_place" must be the same as the last one obtained
        //       in acquire(), by anderson's algorithm, since only
        //       a process holding the lock can call release().
        slots.set((my_place+1) % nprocs, HAS_LOCK);
    }
    
}
