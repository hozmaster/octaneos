#!/usr/bin/python
#
# Berlin Brown
#
# $Id: find_count.py,v 1.1 2004/03/14 03:23:59 bigbinc Exp $
#

import string
import sys
from re import split

def getCountValue(xstring):
    nstr = string.lstrip(xstring)

    nlist = nstr.split(' ')
    ssval = string.rstrip(nlist[0])

    if ssval != '':
        ival = int(ssval)
    else:
        ival = 0
        print "Invalid String"
        
    return ival
    
if __name__=="__main__":
    
    print "// Running python script: find_count.py"
    
    f = open("PureReport.txt", "r")

    line = f.readline()

    ctr = 0
    
    while line:
        str = line
        # convert first chars to number
        val = getCountValue(str)
        ctr = ctr + val        
        line = f.readline()
        
    f.close()

    print " // Total SRC Lines: %d //" % (ctr)

    fx = open("TotalSrc.txt", "w")
    fx.write("\n// Total SRC Lines: %d //\n" % (ctr))
    fx.close()
        
# End of Script
