//
//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// BloggerX
//
// $Id: BloggerX.java,v 1.3 2004/04/13 18:25:49 bigbinc Exp $
// 
// - read incoming data from a file
// and log it
// based on a predefined HTML format
//
//
package org.retro.gis;

import java.io.*;
import java.util.*;


public class BloggerX {

    public static void main(String [] args) {

	System.out.println(" Running blogger...");

	
	// only take data from one file
	// and output to stdout
	if (args.length != 1) {

	    System.out.println("usage: BloggerX _input_file_name");

	} else {
	    
	    StringBuffer _blog_buf = new StringBuffer();

	    BufferedReader _input_stream = null;

	    try {
		
		FileReader _file_reader = new FileReader(args[0]);
		
		_input_stream = new BufferedReader(_file_reader);
		
		String _line = _input_stream.readLine();

		while (_line != null) {

		    _line = _input_stream.readLine();
		   
		    if (_line != null) {
		    	System.out.println(" [ Read: ] " + _line);    
		    	_blog_buf.append(_line + "<br>" + "\n");
		    } // end of the if --==--=

		} // end of the while

		Date  __date = new Date();


		// [ output to stdout here ]
		System.out.println("------------------- [ Copy-Paste ] ----------------------");
		System.out.println();
		
		// [ first row with the date ] 
		System.out.println("<tr>");
		System.out.println("<td bgcolor=\"#F4F4CE\" style=\"border:solid #BBBBBB 1px;\">");
		
		System.out.println("<font size=\"2\" color=\"#777777\">");
		
		System.out.println(__date);

		System.out.println("</font>");


		System.out.println("</td>");
		System.out.println("</tr>");

		// [ second row ]
		System.out.println("<tr>");
		System.out.println("<td bgcolor=\"#F6F6F6\" style=\"border:solid #BBBBBB 1px;\">");		
		System.out.println("<font size=\"2\" color=\"#666666\">");	       
		System.out.println(_blog_buf);
		System.out.println("</font>");
		System.out.println("</td>");
		System.out.println("</tr>");

		// [ hr ]
		System.out.println("<tr>");
		System.out.println("<td>");
		System.out.println("<hr>");
		System.out.println("</td>");
		System.out.println("</tr>");
		

		System.out.println();
		System.out.println("------------------- [ [ End ] Copy-Paste ] --------------");

		_input_stream.close();
			
	    } catch(IOException e) {

		e.printStackTrace();

	    } // end of the try - catch -----
	    

	} // end of the if - else
	
	
    } // end of the function main -=-=-=-=-

} // end of the class -=-=-=---=-=-=-=
