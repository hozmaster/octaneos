//
// Berlin Brown
//
// bigbinc@hotmail.com
//
//
// $Id: GetSnapshot.java,v 1.2 2004/04/06 14:38:51 bigbinc Exp $
//
package org.retro.gis;

import java.util.*;
import java.io.*;

public class GetSnapshot {

    public static void main(String [] args) {

	// we just need the filename
	if (args.length != 1) {
	    
	    System.out.println("usage: java GetSnapshot the_file_name");

	} else {
	    
	    // otherwise output text to stdout with date
	    Date _d = new Date();

	    System.out.println("<tr>");
	    System.out.println("<td style=\"border:solid #BBBBBB 1px;\" bgcolor=\"#e1e6f5\">");
	    System.out.println(" CVS-Snapshot on " + _d + " <br>[ at retroevolution.com ]");
	    System.out.println("</td>");
	    System.out.println("</tr>");
	    	  
	    System.out.println("<tr>");
	    System.out.println("<td style=\"border:solid #BBBBBB 1px;\" bgcolor=\"#f5f5f5\">");

	    System.out.print("<a href=\"http://www.retroevolution.com/src/");
	    System.out.print(args[0]);

	    System.out.println(".tar.gz\">cvs-snapshot on " + _d + "</a>");
	    System.out.println("</td>");
	    System.out.println("</tr>");
	    

	} // end of the if - else

    } // end of the functino -=-=-=

} // end of the class --==-=-=-=-
