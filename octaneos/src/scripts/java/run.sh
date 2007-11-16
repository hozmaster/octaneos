#! /bin/sh
# Berlin Brown


if [ -n "$CLASSPATH" ] ; then
	LOCALCLASSPATH="$CLASSPATH"
fi

if [ -z "$LOCALCLASSPATH" ] ; then
	LOCALCLASSPATH="./build/SpiritScripts.jar"
else
	LOCALCLASSPATH="./build/SpiritScripts":"$LOCALCLASSPATH"
fi

java -classpath "$LOCALCLASSPATH" org.retro.gis.SpiritScripts

