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
PDLROOT=/web/divelog/
CSVTARGET=$PDLROOT/data
HTMLTARGET=$PDLROOT/dives
# the conduit of ADL v0.98 allows only for one *.cvs file, so we have to
# work around by running it twice with two different templates
SITES_INI=divelog.sites
DIVES_INI=divelog.data

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
rm log/* &>/dev/null

# get the dive sites info
echo "Converting Dive Sites Info..."
cp $SITES_INI divelog.ini
java -jar conduit.jar -bothunits %1 %2 %3 %4 %5 >/dev/null
mv log/logbook.csv $CSVTARGET/sites.csv

# get the dive log info
echo "Converting the LogFile entries..."
cp $DIVES_INI divelog.ini
java -jar conduit.jar -bothunits %1 %2 %3 %4 %5 >/dev/null
mv log/logbook.csv $CSVTARGET

# remaining static data we cannot get via CSV
echo "Moving the static data to its destination..."
dos2unix log/* &>/dev/null
chmod g+r log/*
chmod o+r log/*
mv log/dive* $HTMLTARGET
mv log/index* $HTMLTARGET

# cleanup
echo "Cleanup..."
rm log/*

# Finito
echo "Finnished.
#############################################################################"

exit
