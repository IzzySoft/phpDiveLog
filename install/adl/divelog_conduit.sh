#!/bin/bash
#############################################################################
# phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
# written by Itzchak Rehberg <izzysoft@qumran.org>                          #
# http://www.qumran.org/homes/izzy/                                         #
# ------------------------------------------------------------------------- #
# This program is free software; you can redistribute and/or modify it      #
# under the terms of the GNU General Public License (see doc/LICENSE)       #
# ------------------------------------------------------------------------- #
# Shell Script to invoke the Aqua DiveLog conduit, create the CSV data and  #
# move them to their location in the web tree                               #
#############################################################################

#========================================================[ Configuration ]===
# location of the Conduit output (must match the confuration in divelog.ini)
LOGDIR=log
# the location of phpDiveLogs data dir
DATADIR=/web/divelog/data

#=========================================================[ Intro Output ]===
echo "
#############################################################################
# Aqua DiveLog                               (c) 2001-2003 by Stephan Veigl #
# phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
# ------------------------------------------------------------------------- #
# Data Conversion Unit                                                      #
#############################################################################
"

#=========================================================[ Let's do it! ]===
# clean up data from possible previous run - the conduit does not update
# correctly otherwise
echo "Initializing..."
for i in csv dives divesites; do
  rm $LOGDIR/$i/* &>/dev/null
done

# get the DiveLog data
echo "Converting AquaDiveLog Data..."
java -jar conduit.jar -bothunits %1 %2 %3 %4 %5 >/dev/null

# transfer CSV files to web target
echo "Moving datafiles to the web target dir..."
mv $LOGDIR/csv/* $DATADIR

# cleanup
echo "Cleanup..."
for i in csv dives divesites; do
  rm $LOGDIR/$i/* &>/dev/null
done

# Finito
echo "Finnished.
#############################################################################"

exit
