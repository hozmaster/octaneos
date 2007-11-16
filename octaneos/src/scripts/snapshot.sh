#! /bin/sh
# Berlin Brown
#
# $Id: snapshot.sh,v 1.1 2004/04/06 14:01:11 bigbinc Exp $
#

# assuming of course we
# run this script from orange-root

CURDIR=`pwd`
JDIR=${CURDIR}/scripts/java

echo " Run on $JDIR"

if [ -z $1 ] ; then
	echo " usage: snapshot.sh the_snapshot_file_name"
	exit 0
fi

cd $JDIR
ant
cd $CURDIR

LOCALCLASSPATH="$JDIR/build/SpiritScripts.jar"

java -classpath "$LOCALCLASSPATH" org.retro.gis.GetSnapshot $1

echo 
echo "...done."
echo
