#!/usr/bin/env python
#
#
# Berlin Brown
# bigbinc@hotmail.com
#
#
# $Id: print_version.py,v 1.2 2004/04/04 19:09:17 bigbinc Exp $
#
# - just print the version of the system to
#    a C file so that it can be printed
#

import os
import sys

def main(args):
    print "...... [ Printing System Version to File ]....."
    print args

    final = "Vers: ["

    for arg in args:
        final = final + arg + "."

    final = final + "]"
    print final

    fx = open("init/getversion.c", "w")

    # ++ Write the C file with the build ct ++
    fx.write("//\n")
    fx.write("// Berlin Brown\n")
    fx.write("//\n")
    fx.write("// Note: generated from python script\n");
    fx.write("// $Id: print_version.py,v 1.2 2004/04/04 19:09:17 bigbinc Exp $\n")
    fx.write("//\n")
    fx.write("\n")

    fx.write("char *main_get_version = \"%s\";\n\n" % final)
    fx.write("//\n\n");
    
    fx.close()
    

if __name__ == '__main__':
    main(sys.argv[1:])
