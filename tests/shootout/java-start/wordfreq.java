// $Id: wordfreq.java,v 1.3 2004-10-09 17:15:47 mwh Exp $
// http://www.bagley.org/~doug/shootout/
// Collection class code is from my friend Phil Chu, Thanks Phil!

import java.io.*;
import java.util.*;
import java.text.*;

class Counter {
    int count = 1; 
}

public class wordfreq {

    public static void main(String[] args) {
	Reader r;
	if (args.length == 2) {
	    String inFile = args[1];
	    try {
		r = new BufferedReader(new FileReader(inFile));
	    } catch (IOException e) {
		System.err.println(e);
		return;
	    }
	    wf(r);
	}
	//@NOWARM
	//@START
	r = new BufferedReader(new InputStreamReader(System.in));
	wf(r);
	//@END
    }

    public static String padleft(String s,int n,char c) {
        int len = s.length();
        if( len>=n ) return s;
        char[] buf = new char[n];
        for( int i=0;i<n-len;i++ ) buf[i]=c;
        s.getChars(0,len,buf,n-len);
        return new String(buf);
    }
  
    public static void wf(Reader r) {
        HashMap map = new HashMap();
        try {
            StreamTokenizer st = new StreamTokenizer(r);
            st.lowerCaseMode(true);
            st.whitespaceChars( 0, 64 );
            st.wordChars(65, 90);
            st.whitespaceChars( 91, 96 );
            st.wordChars(97, 122);
            st.whitespaceChars( 123, 255 );
            int tt = st.nextToken();
            while (tt != StreamTokenizer.TT_EOF) {
                if (tt == StreamTokenizer.TT_WORD) {
                    if (map.containsKey(st.sval)) {
                        ((Counter)map.get(st.sval)).count++;
                    } else {
                        map.put(st.sval, new Counter());
                    }
                }
                tt = st.nextToken();
            }
        } catch (IOException e) {
            System.err.println(e);
            return;
        }

        Collection entries = map.entrySet();
	// flatten the entries set into a vector for sorting
	Vector rev_wf = new Vector(entries); 

        // Sort the vector according to its value
        Collections.sort(rev_wf, new Comparator() {
		public int compare(Object o1, Object o2) {
		    // First sort by frequency
		    int c = ((Counter)((Map.Entry)o2).getValue()).count - ((Counter)((Map.Entry)o1).getValue()).count;
		    if (c == 0) { // Second sort by lexicographical order
			c = ((String)((Map.Entry)o2).getKey()).compareTo((String)((Map.Entry)o1).getKey());
		    }
		    return c;
		}
	    }
			 );

        Iterator it = rev_wf.iterator();
	Map.Entry ent;
	String word;
	int count;
        while(it.hasNext()) {
            ent = (Map.Entry)it.next();
	    word = ((String)ent.getKey());
	    count = ((Counter)ent.getValue()).count;
	    System.out.println(padleft(Integer.toString(count),7,' ') + "\t" + word);
        }
    }
}
