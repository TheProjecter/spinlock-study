/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package locks;

/**
 *
 * @author trbot
 */
public interface AbstractLock {
    
    /**
     * Acquire the lock (blocking call).
     * May throw an InterruptedException in the event that a thread sleep fails.
     */
    public void acquire() throws Exception;
    
    /**
     * Release the lock.
     * Precondition: the calling process must hold the lock
     */
    public void release();
}
