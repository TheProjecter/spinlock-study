package csc2227proj;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

/**
 *
 * @author trbot
 */
public class PrimitiveBench {

    public PrimitiveBench(String[] args) {
        int runs = 100000000;
        int cur = 0;
        if (args.length != 1) {
            System.out.println("usage: java " + getClass().getSimpleName() + " CASE_NUMBER");
            System.out.println("\twhere CASE_NUMBER is in {1,2,..,5}");
            System.exit(-1);
        }
        int caseno = Integer.parseInt(args[0]);
        boolean result1 = false, result2 = false;
        
        switch (caseno) {
            //case 1: break;
            //case 2: break;
            //case 3: break;
            case 4: {
                System.out.println("fetch and add");
                AtomicInteger addr = new AtomicInteger(0);
                for (int i=0;i<runs;i++) {
                    cur = addr.getAndAdd(1);
                    cur = addr.getAndAdd(-1);
                }
                if (cur != 1) throw new RuntimeException("cur = " + cur);
                if (addr.get() != 0) throw new RuntimeException("addr.get() = " + addr.get());
            } break;
            case 5: {
                System.out.println("compare and swap");
                AtomicInteger addr = new AtomicInteger(0);
                for (int i=0;i<runs;i++) {
                    result1 = addr.compareAndSet(cur, cur+1);
                    result2 = addr.compareAndSet(cur+1, cur);
                }
                if (!result1 || !result2) throw new RuntimeException("result1 = " + result1 + " result2 = " + result2);
                if (addr.get() != 0) throw new RuntimeException("addr.get() = " + addr.get());
            } break;
            case 6: {
                System.out.println("atomic updater cas");
                IntHolder addr = new IntHolder(0);
                AtomicIntegerFieldUpdater<IntHolder> updater = AtomicIntegerFieldUpdater.newUpdater(IntHolder.class, "v");
                for (int i=0;i<runs;i++) {
                    result1 = updater.compareAndSet(addr, cur, cur+1);
                    result2 = updater.compareAndSet(addr, cur+1, cur);
                }
                if (!result1 || !result2) throw new RuntimeException("result1 = " + result1 + " result2 = " + result2);
                if (addr.v != 0) throw new RuntimeException("addr.v = " + addr.v);
            } break;
            default: {
                System.out.println("bad case number specified: " + caseno);
            } break;
        }
    }
    
    private class IntHolder {
        volatile int v;
        public IntHolder(final int v) {
            this.v = v;
        }
    }
    
    public static void main(String[] args) {
        new PrimitiveBench(args);
    }
}
