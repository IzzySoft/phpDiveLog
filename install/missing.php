#!/usr/bin/php
<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Get terms which are missing in a translation file                         #
 # Syntax:                                                                   #
 #   missing <trans_file>                                                    #
 #############################################################################
 # $Id$

#==========================================================[ Syntax Check ]===
if (empty($argv[1])) {
  die("You must pass the translation file to check as the first parameter.\n");
} elseif ($argv[1]=="trans.en") {
  die("Hey, what you are doing? 'trans.en' is the reference file :-)\n");
} elseif(!file_exists($argv[1])) {
  die("The file you specified (".$argv[1].") could not be found.\n");
} elseif(!file_exists("trans.en")) {
  die("The reference file (trans.en) could not be found.\nPlease make sure you run this script from inside the 'lang/' directory!\n");
}

#======================================================[ Helper Functions ]===
#------------------------[ return records of $a1 which are missing in $a2 ]---
function array_substract($a1, $a2) {
  foreach($a1 as $key => $val) {
    if (!empty($key) && !array_key_exists($key,$a2)) $res[$key] = $val;
  }
  return $res;
}

#--------------------------------------------[ make the translation array ]---
function file2array($file) {
  foreach ($file as $f) {
    $pos = strpos($f,";");
    $trans[substr($f,0,$pos)] = substr($f,$pos+1);
  }
  return $trans;
}

#=============================================[ Process translation files ]===
#---------------------------------[ Read files and find the missing terms ]---
$test = file($argv[1]);
$full = file("trans.en");
$diff = array_substract(file2array($full),file2array($test));

#-------------------------------------------------------[ Generate output ]---
if (empty($diff)) {
  die("The translations of '".$argv[1]."' seem to be complete.\n");
} else {
  $list = "";
  foreach ($diff as $var => $val) $list .= "$var;$val";
  $pos = strpos($argv[1],".");
  $file = "diff.en-".substr($argv[1],$pos+1);
  file_put_contents($file,$list);
  die("Missing terms have been written to '$file'\n");
}
?>