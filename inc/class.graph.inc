<?php
#############################################################################
# phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
# written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
# http://www.izzysoft.de/                                                   #
# ------------------------------------------------------------------------- #
# This program is free software; you can redistribute and/or modify it      #
# under the terms of the GNU General Public License (see doc/LICENSE)       #
# ------------------------------------------------------------------------- #
# Generating graphs and diagrams from CSV data                              #
#############################################################################

/* $Id$ */

require_once ("inc/diagram.inc");

/** Generating graphs and diagrams from CSV data
 * @package Api
 * @class graph
 */
class graph {

  /** Initialize
   * @constructor graph
   */
  function graph() {
  }

#---------------------[ Helper to profile() and schedule(): Create GraphBG ]---
  /** Create background for profile/schedule graph
   * @method private graphbg
   * @param ref object diagram
   * @param integer width
   * @param integer height
   * @param float max_depth
   * @param integer divetime
   */
  function graphbg(&$D,$width,$height,$max_depth,$divetime) {
    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    ImageColorAllocate($D->Img, 200,200,200); // background color 
    $D->SetFrame(40, 30, $width-10, $height-30); 
    $D->SetBorder(UTC(2000,01,01,0,0,0), UTC(2000,01,01,0,0,$divetime+20), $max_depth+1, 0); 
    // Setup Grid
    $D->XGridDelta = 300; // 5 min
    if ($divetime > 3900) $D->XGridDelta = 600; // 10' Grid for > 70' divetime
    $D->XSubGrids = 0;
    if ($divetime > 2700) $D->XSubGrids = 2; // 45'
    elseif ($divetime > 1300) $D->XSubGrids = 3; // 21.6'
    elseif ($divetime > 600)  $D->XSubGrids = 5; // 10'
    $D->YSubGrids = 0;
    if ($max_depth < 20) $D->YSubGrids = 2;
    elseif ($max_depth < 25) $D->YSubGrids = 3;
    elseif ($max_depth < 50) $D->YSubGrids = 2;
    else $D->YSubGrids = 1;
    // Setup Labels
    $D->XScalePosition = "left";
    $D->YScale=" m";
    $D->XScale = 5;
    $D->Font = 3;
    $D->SetText("","","");
    $D->SetGridColor("#cccccc", "#99ccff");
    $D->Draw("#3333ff", "#000000", false);
  }

#----------------------------------------------------------[ Dive Profiles ]---
  /** Generate dive profile graph
   * @method profile
   * @param integer divenr dive number
   */
  function profile($nr) {
    GLOBAL $pdl;
    // setup data
    while (strlen($nr)<5) $nr = "0$nr";
    $csvfile = $pdl->config->datadir."dive${nr}_profile.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $csv2file = $pdl->config->datadir."dive${nr}_schedule.csv";
    $profilepng = $pdl->config->user_path . "profiles/dive${nr}_profile.png";
    $profilemap = $pdl->config->user_path . "profiles/dive${nr}_profile.map";

    // still here? So we have to create graph and map:
    if (empty($width))  $width  = 468;
    if (empty($height)) $height = 300;
    $csv = new csv(";",'"',TRUE);
    $csv->import($csvfile);

    // setup the graph
    $time = array();
    $max_depth = 0;
    for($i=0;$i<count($csv->data);$i++) {
      // Get max depth
      $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
      if($csv->data[$i]['depth'] > $max_depth) $max_depth = $csv->data[$i]['depth'];
      // Calculate times
      $x = explode(":",$csv->data[$i]['time']);
      $time[] = $x[0]*60 + $x[1];
      // Correct image height for small depths
      if($max_depth < 5)  $height = 200;
      if($max_depth < 10) $height = 250;
    }
    $divetime = $time[count($time)-1]; // dive time is the last value

    // Create the Diagram
    $D=new Diagram(); 
    $this->graphbg($D,$width,$height,$max_depth,$divetime);
    // Draw Graph
    $base=$D->ScreenY(0.01);
    $toff=UTC(2000,01,01,0,0,0);
    for($i=1;$i<count($csv->data);$i++) {
      $D->Area($D->ScreenX($toff+$time[$i-1]),$D->ScreenY($csv->data[$i-1]['depth']),$D->ScreenX($toff+$time[$i]),$D->ScreenY($csv->data[$i]['depth']),"99ccff",$base,"","");
      $D->Line($D->ScreenX($toff+$time[$i-1]),$D->ScreenY($csv->data[$i-1]['depth']),$D->ScreenX($toff+$time[$i]),$D->ScreenY($csv->data[$i]['depth']),"000000",1,"","");
      if ($csv->data[$i]['warning'] != '') {
        $D->Dot($D->ScreenX($toff+$time[$i]), $D->ScreenY($csv->data[$i]['depth']),12, 0, "#ff0000", $csv->data[$i]['warning']);
      }
    }
    // Go for the schedule, if we have one
    // !!! add the start time to runtime !!!
    if ($GLOBALS["schedule_graph"]=="integrated" && file_exists($csv2file)) {
      unset ($csv);
      $csv = new csv(";",'"',TRUE);
      $csv->import($csv2file);
      $dc = count($csv->data);
      for ($i=0;$i<$dc;++$i) {
        $x = explode(":",$csv->data[$i]['time']);
        $csv->data[$i]['time'] = $x[0]*60 + $x[1];
        $x = explode(":",$csv->data[$i]['runtime']);
        $csv->data[$i]['runtime'] = $x[0]*60 + $x[1];
        $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
        $D->Line($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
	         $D->ScreenY($csv->data[$i]['depth']),
		 $D->ScreenX($toff+$csv->data[$i]["runtime"]),
		 $D->ScreenY($csv->data[$i]['depth']),
		 "00BBBB",3,"","");
	if ($i+1<$dc) $end = $csv->data[$i+1]['depth']; else $end = 0;
        $D->Line($D->ScreenX($toff+$csv->data[$i]["runtime"]),
	         $D->ScreenY($csv->data[$i]['depth']),
		 $D->ScreenX($toff+$csv->data[$i]["runtime"]),
		 $D->ScreenY($end),
		 "00BBBB",1,"","");
      }
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  } // end method profile

#---------------------------------------------------------------[ Schedule ]---
  /** Generate the dive schedule graph
   * @method schedule
   * @param integer divenumber
   */
  function schedule($nr) {
    GLOBAL $pdl;
    // setup data
    while (strlen($nr)<5) $nr = "0$nr";
    $csvfile = $pdl->config->datadir."dive${nr}_schedule.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/dive${nr}_schedule.png";

    // setup graph
    if (empty($width))  $width  = 468;
    if (empty($height)) $height = 300;
    $csv = new csv(";",'"',TRUE);
    $csv->import($csvfile);
    $dc = count($csv->data);
    $time = array();
    $max_depth = 0;
    for($i=0;$i<$dc;$i++) {
      // Get max depth
      $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
      if($csv->data[$i]['depth'] > $max_depth) $max_depth = $csv->data[$i]['depth'];
      // Calculate times
      $x = explode(":",$csv->data[$i]['time']);
      $time[] = $x[0]*60 + $x[1];
      $x = explode(":",$csv->data[$i]['runtime']);
      $runtime[] = $x[0]*60 + $x[1];
      // Correct image height for small depths
      if($max_depth < 5)  $height = 200;
      if($max_depth < 10) $height = 250;
    }
    $divetime = $runtime[count($runtime)-1]; // dive time is the last value

    // Create the Diagram
    $D=new Diagram(); 
    $this->graphbg($D,$width,$height,$max_depth,$divetime);
    // Draw Graph
    $base=$D->ScreenY(0.01);
    $toff=UTC(2000,01,01,0,0,0);
    for ($i=0;$i<$dc;++$i) {
      $x = explode(":",$csv->data[$i]['time']);
      $csv->data[$i]['time'] = $x[0]*60 + $x[1];
      $x = explode(":",$csv->data[$i]['runtime']);
      $csv->data[$i]['runtime'] = $x[0]*60 + $x[1];
      $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
      $D->Area($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
               $D->ScreenY($csv->data[$i]['depth']),
               $D->ScreenX($toff+$csv->data[$i]['runtime']),
               $D->ScreenY($csv->data[$i]['depth']),
               "99ccff",$base,"","");
      $D->Line($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
	       $D->ScreenY($csv->data[$i]['depth']),
	       $D->ScreenX($toff+$csv->data[$i]["runtime"]),
	       $D->ScreenY($csv->data[$i]['depth']),
	       "00BBBB",3,"","");
      if ($i+1<$dc) $end = $csv->data[$i+1]['depth']; else $end = 0;
      $D->Line($D->ScreenX($toff+$csv->data[$i]["runtime"]),
	       $D->ScreenY($csv->data[$i]['depth']),
	       $D->ScreenX($toff+$csv->data[$i]["runtime"]),
	       $D->ScreenY($end),
	       "00BBBB",1,"","");
      }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    ImageDestroy($D->Img); 
  } // end method schedule

#---------------------------------------------------------[ Dives per Year ]---
  /** Generate dive stats graph
   * @method dives
   */
  function dives() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/divestat.png";
    $profilemap = $pdl->config->user_path . "profiles/divestat.map"; // not used yet
    if (empty($width))  $width  = 468;
    if (empty($height)) $height = 200;
    $csv = new csv(";",'"',TRUE);
    $csv->import($csvfile);

    // setup the graph
    $time = array();
    $dc = count($csv->data);
    $min_date = preg_replace('|.*(\d{4}).*|','$1',$csv->data[0]['date']);
    $max_date = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$dc-1]['date']);
    for($i=0;$i<count($csv->data);$i++) {
      // Zeiten berechnen
      $x = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$i]['date']);
      if (empty($dives[$x])) $dives[$x] = 1;
      else ++$dives[$x];
    }
    $dives[$max_date+1] = 0;
    ++$max_date;
    $max_dives = 0;
    $year_in_s = 365 * 24 * 3600;
    for ($i=$min_date,$k=0;$i<=$max_date;++$i,++$k) {
      $time[$k] = ($i * $year_in_s - 0.5 * $year_in_s) - (1970 * $year_in_s);
      $data[$k] = $dives[$i];
      $year[$k] = $i;
      if ($data[$k]>$max_dives) $max_dives = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    ImageColorAllocate($D->Img, 200,200,200); // background color 
    $D->SetFrame(60, 30, $width-20, $height-30);
    if ($years<5)
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($min_date+4,12,31,0,0,0), 0, $max_dives+1); 
    else
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($max_date-1,12,31,0,0,0), 0, $max_dives+1); 
    $D->XSubGrids = 1;
    $D->YSubGrids = 1;
    // Setup Labels
    $D->XScalePosition = "left";
    $D->XScale = 5;
    $D->Font = 3;
    $D->SetText("","","");
    $D->SetGridColor("#3333ff", "#3333ff");
    // Draw Graph
    $D->Draw("#99ccff", "#000000", false);
    $base=$D->ScreenY(0.01);
    if ($years < 10) $boxborder = 2;
    else {
      $boxborder = 1;
      $D->BFont = 4;
    }
    if ($years>20) $D->BFont = 1;
    elseif ($years>15) $D->BFont = 2;
    for($i=1;$i<$years;$i++) {
      if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>25)) $dispval = "";
      else $dispval = $data[$i-1];
      $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".$data[$i-1]);
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  }

#------------------------------------------------------[ Divetime per Year ]---
  /** Generate dive stats graph
   * @method divetime
   */
  function divetime() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/timestat.png";
    $profilemap = $pdl->config->user_path . "profiles/timestat.map"; // not used yet
    if (empty($width))  $width  = 468;
    if (empty($height)) $height = 200;
    $csv = new csv(";",'"',TRUE);
    $csv->import($csvfile);

    // setup the graph
    $time = array();
    $dc = count($csv->data);
    $min_date = preg_replace('|.*(\d{4}).*|','$1',$csv->data[0]['date']);
    $max_date = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$dc-1]['date']);
    for($i=0;$i<count($csv->data);$i++) {
      // Zeiten berechnen
      $x = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$i]['date']);
      if (empty($divetime[$x])) $divetime[$x] = (int) $csv->data[$i]['divetime'];
      else $divetime[$x] += (int) $csv->data[$i]['divetime'];
    }
    $divetime[$max_date+1] = 0;
    ++$max_date;
    $max_time = 0;
    $year_in_s = 365 * 24 * 3600;
    for ($i=$min_date,$k=0;$i<=$max_date;++$i,++$k) {
      $time[$k] = ($i * $year_in_s - 0.5 * $year_in_s) - (1970 * $year_in_s);
      $data[$k] = $divetime[$i];
      $year[$k] = $i;
      if ($data[$k]>$max_time) $max_time = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    ImageColorAllocate($D->Img, 200,200,200); // background color 
    $D->SetFrame(60, 30, $width-20, $height-30);
    if ($years<5) $edate = $min_date +4;
    else $edate = $max_date -1;
    if ($max_time < 600) {
      $D->YScale = "'";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($edate,12,31,0,0,0), 0, $max_time+60);
    } else {
      $D->YScale = "h";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($edate,12,31,0,0,0), 0, ($max_time+60)/60);
    }
    $D->XSubGrids = 1;
    $D->YSubGrids = 1;
    // Setup Labels
    $D->XScalePosition = "left";
    $D->XScale = 5;
    $D->Font = 3;
    $D->SetText("","","");
    $D->SetGridColor("#3333ff", "#3333ff");
    // Draw Graph
    $D->Draw("#99ccff", "#000000", false);
    $base=$D->ScreenY(0.01);
    if ($years < 10) $boxborder = 2;
    else {
      $boxborder = 1;
      $D->BFont = 2;
    }
    if ($years>15) $D->BFont = 1;
    for($i=1;$i<$years;$i++) {
      if ($max_time < 600) {
        if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>25)) $dispval = "";
        else $dispval = $data[$i-1];
        $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".$data[$i-1]."min");
      } else {
        if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>25)) $dispval = "";
        else $dispval = round($data[$i-1]/60,1);
        $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY(round($data[$i-1]/60,1)),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".round($data[$i-1]/60,1)."h");
      }
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  }


} // end class graph

?>
