#! /bin/sh
# Berlin Brown


if [ -n "$CLASSPATH" ] ; then
	LOCALCLASSPATH="$CLASSPATH"
fi

if [ -z "$LOCALCLASSPATH" ] ; then
	LOCALCLASSPATH="./build/HelpKernel.jar"
else
	LOCALCLASSPATH="./build/HelpKernel":"$LOCALCLASSPATH"
fi

java -classpath "$LOCALCLASSPATH" org.retro.gis.HelpKernel

