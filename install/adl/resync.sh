#!/bin/bash
#############################################################################
# phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
# written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
# http://www.izzysoft.de/                                                   #
# ------------------------------------------------------------------------- #
# This program is free software; you can redistribute and/or modify it      #
# under the terms of the GNU General Public License (see doc/LICENSE)       #
# ------------------------------------------------------------------------- #
# Transfer the data files from a remote server back to the local PDL        #
# installation using RSync                                                  #
#############################################################################

#=========================================================[ Intro Output ]===
echo "
#############################################################################
# phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
# ------------------------------------------------------------------------- #
# Remote Data Retriever                                                     #
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
$CONFIG

#=========================================================[ Let's do it! ]===
# RSync local installation from REMOTE machine
RLOGDIR=$PDLBASE
LDATADIR=$PDLBASE/data
echo `date +"$DATEFORMAT"` "Retrieving files from the remote server via RSync..." | tee -a $LOGFILE
rsync -ae ssh $SCPBASE/* $PDLBASE &>/dev/null
# Finito
echo `date +"$DATEFORMAT"` "Finnished." | tee -a $LOGFILE
echo "">>$LOGFILE

#############################################################################"

exit
