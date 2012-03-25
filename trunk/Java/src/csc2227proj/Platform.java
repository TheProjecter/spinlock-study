/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package csc2227proj;

/**
 *
 * @author trbot
 */
public class Platform {
    public static final int CACHE_LINE_SIZE_BYTES = 128;
    public static final int SLOT_STEP_INT = CACHE_LINE_SIZE_BYTES/4;
    public static final int SLOT_STEP_BOOL = CACHE_LINE_SIZE_BYTES/1;
}
