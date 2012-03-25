package csc2227proj;

import java.util.ArrayList;

/**
 *
 * @author trbot
 */
public class NanoTimer {
    private ArrayList<Long> times = new ArrayList<>();
    
    public NanoTimer() {
        
    }
    
    public void recordTime() {
        times.add(System.nanoTime());
    }
    
    /**
     * Returns the number of seconds elapsed between
     * the i-th recorded time and the j-th recorded time
     * (must have i less than j, and have recorded at least j+1 times).
     */
    public double getSeconds(int i, int j) {
        if (i >= j) throw new RuntimeException("invalid interval--must have i < j");
        if (j >= times.size()) throw new RuntimeException("invalid j--must have recorded at least j+1 times");
        return (times.get(j)-times.get(i))/1e9;
    }
    
    /**
     * Same as getSecond(i, j), but returns a string representation of
     * the number of seconds.
     */
    public String toString(int i, int j) {
        return String.format("%.2fs", getSeconds(i, j));
    }
    
    /**
     * Prints toString(i, j) to stdout.
     */
    public void printTime(int i, int j) {
        System.out.println(toString(i, j));
    }
    
    /**
     * Prints toString(i-1, i) to stdout,
     * where the last call to recordTime() was the i-th such call.
     */
    public void printTime() {
        if (times.size() < 2) throw new RuntimeException("invalid call to printTime()--must have recorded at least two times");
        System.out.println(toString(times.size()-2, times.size()-1));
    }
}
