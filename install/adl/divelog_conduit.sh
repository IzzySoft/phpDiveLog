#!/bin/bash
#############################################################################
# phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
# written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
# http://www.izzysoft.de/                                                   #
# ------------------------------------------------------------------------- #
# This program is free software; you can redistribute and/or modify it      #
# under the terms of the GNU General Public License (see doc/LICENSE)       #
# ------------------------------------------------------------------------- #
# Shell Script to invoke the Aqua DiveLog conduit, create the CSV data and  #
# move them to their location in the web tree                               #
#############################################################################

#=========================================================[ Intro Output ]===
echo "
#############################################################################
# Aqua DiveLog                               (c) 2001-2004 by Stephan Veigl #
# phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
# ------------------------------------------------------------------------- #
# Data Conversion Unit                                                      #
#############################################################################
"

#========================================================[ Configuration ]===
#BINDIR=`pwd`
BINDIR=${0%/*}
SCRIPT=${0##*/}
CONFIG=$BINDIR/config

function syntax() {
  echo "Syntax: ${SCRIPT} [Options]"
  echo "  Options:"
  echo "     -c <ConfigFile> (Default: 'config' in the scripts directory)"
  echo "     -h              (Print this help screen and exit)"
  exit;
}

while [ "$1" != "" ] ; do
  case "$1" in
    -?) syntax;;
    -h) syntax;;
    --help) syntax;;
    -c) shift; CONFIG=$1;;
  esac
  shift
done

cd $BINDIR
. $CONFIG

#=========================================================[ Let's do it! ]===
# clean up data from possible previous run - the conduit does not update
# correctly otherwise
echo `date +"$DATEFORMAT"` "Initializing..." | tee -a $LOGFILE
for i in csv images dives divesites; do
  rm $LOGDIR/$i/* &>/dev/null
done

# get the DiveLog data
echo `date +"$DATEFORMAT"` "Converting AquaDiveLog Data..." | tee -a $LOGFILE
oriLang=$LANG
export LANG=$PALMLOCALE
java -jar conduit.jar -$UNITS $PROFUPD $* >/dev/null
export LANG=$oriLang

# transfer CSV files to LOCAL web target
if [ $USELOCAL -eq 1 ]; then
  echo `date +"$DATEFORMAT"` "Copying files to the local web target dir..." | tee -a $LOGFILE
  cp $LOGDIR/csv/* $PDLBASE/data/ &>/dev/null
  cp $LOGDIR/images/* $PDLBASE/images/ &>/dev/null
fi

# SCP transfer to REMOTE machine
if [ $USESCP -eq 1 ]; then
  echo `date +"$DATEFORMAT"` "Copying files to the remote web target dir via SCP..." | tee -a $LOGFILE
  scp $LOGDIR/csv/* $SCPBASE/data &>/dev/null
  scp $LOGDIR/images/* $SCPBASE/images/ &>/dev/null

# RSync with REMOTE machine
elif [ $USESCP -eq 2 ]; then
  if [ $RSYNCBASE -eq 1 ]; then
    RLOGDIR=$PDLBASE
    LDATADIR=$PDLBASE/data
  else
    RLOGDIR=$LOGDIR
    LDATADIR=$LOGDIR/csv
  fi
  echo `date +"$DATEFORMAT"` "Syncronizing with remote web target dir via RSync..." | tee -a $LOGFILE
  echo `date +"$DATEFORMAT"` "- Sync DataFiles..." | tee -a $LOGFILE
  rsync -ae ssh $LDATADIR/* $SCPBASE/data &>/dev/null
  echo `date +"$DATEFORMAT"` "- Sync DiveProfiles..." | tee -a $LOGFILE
  rsync -ae ssh $RLOGDIR/images/* $SCPBASE/images &>/dev/null
  echo `date +"$DATEFORMAT"` "- Sync Fotos..." | tee -a $LOGFILE
  rsync -ae ssh $RLOGDIR/fotos/* $SCPBASE/fotos &>/dev/null
  echo `date +"$DATEFORMAT"` "- Sync TextFiles..." | tee -a $LOGFILE
  rsync -ae ssh $RLOGDIR/notes/* $SCPBASE/notes &>/dev/null
  rsync -ae ssh $RLOGDIR/text/* $SCPBASE/text &>/dev/null
fi

# Finito
echo `date +"$DATEFORMAT"` "Finnished." | tee -a $LOGFILE
echo "">>$LOGFILE

#############################################################################"

exit
