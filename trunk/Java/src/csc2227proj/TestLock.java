package csc2227proj;

import locks.*;

/**
 *
 * @author trbot
 */
public class TestLock {
    private AbstractLock lock;
    
    public TestLock(AbstractLock lock) {
        this.lock = lock;
    }
    
    public void testAcquireRelease() throws Exception {
        for (int i=0;i<2;i++) {
            lock.acquire();
            System.out.println("acquired lock");
            lock.release();
            System.out.println("released lock");
        }
    }
    
    public void testDeadlock() throws Exception {
        System.out.println("deadlocking");
        lock.acquire();
        lock.acquire();
    }
    
    public static void main(String[] args) throws Exception {
        AbstractLock[] locks = {new Lock1(), new Lock2(), new Lock3(1), new MCSLock()};
        for (AbstractLock lock : locks) {
            System.out.println("testing " + lock.getClass().getSimpleName());
            TestLock test = new TestLock(lock);
            test.testAcquireRelease();
        }
        
        //(new TestLock(locks[2])).testDeadlock();
        //(new TestLock(locks[3])).testDeadlock();
    }
}
