package locks;

import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

/**
 *
 * @author trbot
 */
public class Lock1 implements AbstractLock {
    private final static int LOCKED = 1;
    private final static int UNLOCKED = 0;
    
    // updater to perform CAS on ``lock''
    private final static AtomicIntegerFieldUpdater<Lock1> up =
            AtomicIntegerFieldUpdater.newUpdater(Lock1.class, "lock");
    
    // represents the actual lock
    private volatile int lock = UNLOCKED;
    
    @Override
    public void acquire() throws Exception {
            int delay = 1;
            while (!up.compareAndSet(this, UNLOCKED, LOCKED)) {
                Thread.sleep(delay);
                delay <<= 1;
            }
    }
    
    @Override
    public void release() {
        up.set(this, UNLOCKED);
    }
    
}
