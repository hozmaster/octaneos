#!/usr/bin/env python
#
# Berlin Brown
# bigbinc@hotmail.com
#
# $Id: print_version.py,v 1.2 2004/04/04 19:09:17 bigbinc Exp $
#
# Just print the version of the system to
#    a C file so that it can be printed

import os
import sys

def main(args):
	print "INFO: Printing System Version to File"
	final = [ "INFO: Build Vers=" ]
	for arg in args:
		final.append(arg + ".")		
	print ''.join(final)
	fx = open("init/getversion.c", "w")	
    # Write the C file with the build ct
	str_data = [
		"//\n",
		"// Berlin Brown\n",
		"//\n",
		"// Note: generated from python script\n",
		"// $Id: print_version.py,v 1.2 2004/04/04 19:09:17 bigbinc Exp $\n",
		"//\n",
		"\n",
		"char *main_get_version = \"%s\";\n\n" % final,
		"// (END OF FILE)\n\n" ]
	fx.write(''.join(str_data))
	fx.close()
    
if __name__ == '__main__':
    main(sys.argv[1:])
