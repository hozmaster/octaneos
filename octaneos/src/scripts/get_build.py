#!/usr/bin/python
#
# $Id: get_build.py,v 1.7 2005/05/21 06:08:10 bigbinc Exp $
# Berlin Brown
#
# Find the total lines of source we have
#

import string
import sys
from re import split

if __name__=="__main__":
    
    print "INFO: Getting build and writing to file ..."    
    f = open("build", "r")
    line = f.readline()

    ctr = 0    
    str = line
    
    # convert first chars to number
    val = int(str)
    ctr = val + 1
    line = f.readline()    
    f.close()

    print ">>> New Build Count: %d <<<" % (ctr-1)

    # over-write the existing build file
    fy = open("build", "w")
    fy.write("%d" % (ctr))
    fy.close()    
    fx = open("init/buildct.c", "w")

    # ++ Write the C file with the build ct ++
    fx.write("//\n")
    fx.write("// Berlin Brown\n")
    fx.write("//\n")
    fx.write("// Note: generated from python script\n");
    fx.write("// $Id: get_build.py,v 1.7 2005/05/21 06:08:10 bigbinc Exp $\n")
    fx.write("//\n")
    fx.write("\n")

    fx.write("unsigned long main_get_build_count = %ld;" % ctr)
    fx.write("\n//\n\n");    
    fx.close()
        
# // End of Main //
