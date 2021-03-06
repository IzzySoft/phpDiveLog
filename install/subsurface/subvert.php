#!/usr/bin/php
<?php
require(__DIR__.'/subsurface.class.php');

function syntax() {
  //$TBOLD = "\033[1;37m"; $TNORMAL = "\033[0;37m";
  $TBOLD = "\e[1m"; $TNORMAL = "\e[0m";
  echo "\n";
  echo $TBOLD."Syntax:".$TNORMAL." ".$GLOBALS['argv'][0]." [options] create [command] | export [command]>\n";
  echo $TBOLD."Examples:".$TNORMAL."\n  ".$GLOBALS['argv'][0]." -f foobar.xml -1 Accessories create\n  ".$GLOBALS['argv'][0]." -f foobar.xml update dives\n  ".$GLOBALS['argv'][0]." -f foobar.xml export\n  ".$GLOBALS['argv'][0]." -f foobar.xml -i5 export dives\n";
  echo $TBOLD."Options:".$TNORMAL."\n"
     . "  -f <file_name>: name of the Subsurface XML file to read (MUST be specified)\n"
     . "  -1 <userdef1> : name of the 'userdef1' column\n"
     . "  -2 <userdef2> : name of the 'userdef2' column\n"
     . "  -i <num>      : ignore dive profiles shorter than <num> on export"
     . "\n";
}

$opts = getopt("f:1:2:i:");
$optind = 1;
if ( isset($opts['f']) && !empty($opts['f']) ) { ++$optind; ++$optind; $file = $opts['f']; } else { syntax(); exit; }
if ( isset($opts['1']) ) { ++$optind; ++$optind; $userdef1 = $opts['1']; } else $userdef1 = '';
if ( isset($opts['2']) ) { ++$optind; ++$optind; $userdef2 = $opts['2']; } else $userdef2 = '';
if ( isset($opts['i']) ) { ++$optind; ++$optind; $min_prof_len = $opts['i']; } else $min_prof_len = 0;

switch($argv[$optind]) {
  case 'create':
    $sub = new subsurface($file,$userdef1,$userdef2);
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='sites' ) {
      $res = $sub->create_sitemap();
      echo $res[1]."\n";
    }
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='dives' ) {
      $res = $sub->create_divemap();
      echo $res[1]."\n";
    }
    break;
  case 'update':
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='sites' ) {
      $res = $sub->update_sitemap();
      echo $res[1]."\n";
    }
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='dives' ) {
      $res = $sub->update_divemap();
      echo $res[1]."\n";
    }
    break;
  case 'export':
    $sub = new subsurface($file,$userdef1,$userdef2);
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='sites' ) {
      $res = $sub->export_sites();
      echo $res[1]."\n";
    }
    if ( empty($argv[$optind +1]) || $argv[$optind +1]=='dives' ) {
      $res = $sub->export_dives($min_prof_len);
      echo $res[1]."\n";
    }
    break;
  default: syntax(); exit; break;
}
?>