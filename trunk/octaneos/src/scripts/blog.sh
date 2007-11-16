#! /bin/sh
# Berlin Brown
#
# $Id: blog.sh,v 1.1 2004/04/12 13:58:42 bigbinc Exp $
#


CURDIR=`pwd`
JDIR=${CURDIR}/java

echo " Run on $JDIR"

cd $JDIR
ant
cd $CURDIR

LOCALCLASSPATH="$JDIR/build/SpiritScripts.jar"

java -classpath "$LOCALCLASSPATH" org.retro.gis.BloggerX "input/blog.txt"

echo 
echo "...done."
echo

