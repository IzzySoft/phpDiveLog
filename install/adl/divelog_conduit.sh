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
# which units to use for the values: imperial|metric|bothunits
UNITS=bothunits
# date format for logging
DATEFORMAT="%Y-%m-%d %H:%M:%S"
#------------------------------------------------------[ Local Transfers ]---
# Do we use the local transfer? [0|1]
USELOCAL=1
# the location of phpDiveLogs user dir (where the data/ and images/ dirs are)
PDLBASE=/web/divelog/diver/demo
#-----------------------------------------------------[ Remote Transfers ]---
# Do we intend to use SCP transfers? [0|1]
USESCP=0
# If so, we need the target base directory
SCPBASE=user@machine:/path_to_PDL/diver/demo

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

# SCP transfer to REMOTE machine
if [ $USESCP -eq 1 ]; then
  echo `date +"$DATEFORMAT"` "Copying files to the remote web target dir..."
  scp $LOGDIR/csv/* $SCPBASE/data
  scp $LOGDIR/images/* $SCPBASE/images/ &>/dev/null
fi

# transfer CSV files to LOCAL web target
if [ $USELOCAL -eq 1 ]; then
  echo `date +"$DATEFORMAT"` "Copying files to the local web target dir..."
  cp $LOGDIR/csv/* $PDLBASE/data/
  cp $LOGDIR/images/* $PDLBASE/images/ &>/dev/null
fi

# Finito
echo `date +"$DATEFORMAT"` "Finnished.

#############################################################################"

exit
