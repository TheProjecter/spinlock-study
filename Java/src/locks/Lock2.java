/**
 * Note: this may behave incorrectly as variables next_ticket and now_serving
 *       wrap around and become negative, because the original algorithm
 *       calls for unsigned integers (which Java does not provide).
 */

package locks;

import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

/**
 *
 * @author trbot
 */
public class Lock2 implements AbstractLock {
    
    // updater to perform CAS on ``next_ticket''
    private final static AtomicIntegerFieldUpdater<Lock2> up =
            AtomicIntegerFieldUpdater.newUpdater(Lock2.class, "next_ticket");
    
    // represents the actual lock
    private volatile int next_ticket = 0;   // warning: this should really be unsigned
    private int now_serving = 0;            // warning: this should really be unsigned
    
    @Override
    public void acquire() throws Exception {
        final int my_ticket = up.getAndIncrement(this);
        for (;;) {
            // following line might not work if my_ticket is negative
            // (can do, e.g., (((long)my_ticket)&0xffffffffL) to do arithmetic
            //  with longs instead, but this might slow things down quite a bit)
            Thread.sleep(my_ticket - now_serving);
            if (now_serving == my_ticket) return;
        }
    }
    
    @Override
    public void release() {
        now_serving++;
    }
    
}
