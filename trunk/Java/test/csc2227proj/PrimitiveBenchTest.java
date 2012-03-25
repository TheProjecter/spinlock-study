/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package csc2227proj;

import org.junit.*;
import static org.junit.Assert.*;

/**
 *
 * @author trbot
 */
public class PrimitiveBenchTest {
    
    public PrimitiveBenchTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }
    
    @Before
    public void setUp() {
    }
    
    @After
    public void tearDown() {
    }

    /**
     * Test of main method, of class PrimitiveBench.
     */
    @Test
    public void testMain() {
        System.out.println("main");
//        System.out.println("dry runs for hotspot compiler...");
//        int dryruns = 6;
//        for (int i=0;i<dryruns;i++) PrimitiveBench.main(new String[]{"4"});
//        for (int i=0;i<dryruns;i++) PrimitiveBench.main(new String[]{"5"});
//        System.out.println("real runs...");
        NanoTimer timer = new NanoTimer();
//        timer.recordTime();
//        PrimitiveBench.main(new String[]{"4"});
//        timer.recordTime();
//        timer.printTime();
//        PrimitiveBench.main(new String[]{"5"});
//        timer.recordTime();
//        timer.printTime();
        int runs = 5;
        timer.recordTime();
        for (int i=0;i<runs;i++) {
            PrimitiveBench.main(new String[]{"4"});
            timer.recordTime();
            timer.printTime();
        }
        for (int i=0;i<runs;i++) {
            PrimitiveBench.main(new String[]{"5"});
            timer.recordTime();
            timer.printTime();
        }
        for (int i=0;i<runs;i++) {
            PrimitiveBench.main(new String[]{"6"});
            timer.recordTime();
            timer.printTime();
        }
    }
}
