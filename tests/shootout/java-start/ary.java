// $Id: ary.java,v 1.2 2004-09-07 20:33:26 mwh Exp $
// http://www.bagley.org/~doug/shootout/

// this program is modified from:
//   http://cm.bell-labs.com/cm/cs/who/bwk/interps/pap.html
// Timing Trials, or, the Trials of Timing: Experiments with Scripting
// and User-Interface Languages</a> by Brian W. Kernighan and
// Christopher J. Van Wyk.

import java.io.*;
import java.util.*;

public class ary {
    public static void main(String args[]) {
	int i, j, n = Integer.parseInt(args[0]);
	//@START
	int x[] = new int[n];
	int y[] = new int[n];

	for (i = 0; i < n; i++)
	    x[i] = i ;
	for (j = n-1; j >= 0; j--)
	    y[j] = x[j];

	System.out.println(y[n-1]);
	//@END
    }
}
