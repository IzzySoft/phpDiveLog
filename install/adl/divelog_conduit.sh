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
# the location of phpDiveLogs user dir (where the data/ and images/ dirs are)
PDLBASE=/web/divelog/diver/demo
# which units to use for the values: imperial|metric|bothunits
UNITS=bothunits
# date format for logging
DATEFORMAT="%Y-%m-%d %H:%M:%S"

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
echo `date +"$DATEFORMAT"` "Initializing..."
for i in csv images dives divesites; do
  rm $LOGDIR/$i/* &>/dev/null
done

# get the DiveLog data
echo `date +"$DATEFORMAT"` "Converting AquaDiveLog Data..."
java -jar conduit.jar -$UNITS $* >/dev/null

# transfer CSV files to web target
echo `date +"$DATEFORMAT"` "Moving files to the web target dir..."
mv $LOGDIR/csv/* $PDLBASE/data/
mv $LOGDIR/images/* $PDLBASE/images/ &>/dev/null

# Finito
echo `date +"$DATEFORMAT"` "Finnished.

#############################################################################"

exit
