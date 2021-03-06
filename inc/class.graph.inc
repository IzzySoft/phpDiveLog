<?php
#############################################################################
# phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
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
  /** Create background for profile/schedule graph (divetime based)
   * @method private graphbg
   * @param ref object diagram
   * @param integer width
   * @param integer height
   * @param float max_depth
   * @param integer divetime
   */
  function graphbg(&$D,$width,$height,$max_depth,$divetime) {
    // Create the Diagram
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }
    $D->SetFrame(50, 10, $width-10, $height-30); 
    $D->SetBorder(UTC(2000,01,01,0,0,0), UTC(2000,01,01,0,0,$divetime*1.1), $max_depth*1.1, 0); 
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
  /** Check whether to update a given profile and do it if necessary
   * @method profileCheck
   * @param integer nr (dive number)
   */
  function profileCheck($no) {
    GLOBAL $pdl;
    $nr = str_pad($no,5,"0",STR_PAD_LEFT);
    $csvfile = $pdl->config->datadir."dive${nr}_profile.csv";
    $schedulecsv = $pdl->config->datadir."dive${nr}_schedule.csv";
    $profilepng = $pdl->config->user_path . "profiles/dive${nr}_profile.png";
    $schedulepng = $pdl->config->user_path . "profiles/dive${nr}_schedule.png";
    if (USE_DYN_PROFILE_PNG) { // generate dynamic profile/schedule graphs
      if (!file_exists($profilepng) || filemtime($profilepng) < filemtime($csvfile)) {
        $this->profile($nr);
      }
      if (((SCHEDULE_GRAPH=="integrated" && !file_exists($profilepng)) || SCHEDULE_GRAPH=="separate")
         && (!file_exists($schedulepng) || filemtime($schedulepng) < filemtime($schedulecsv))) {
        $this->schedule($nr);
      }
    }
  }

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
        if (!preg_match("|[0123456789]|",$csv->data[$i]['depth'])) { // Ascent
          $s_depth = $csv->data[$i-1]['depth'];
          $e_depth = floatval($csv->data[$i+1]['depth']);
        } else { // normal
          $s_depth = $e_depth = floatval($csv->data[$i]['depth']);
        }
        $x = explode(":",$csv->data[$i]['time']);
        $csv->data[$i]['time'] = $x[0]*60 + $x[1];
        $x = explode(":",$csv->data[$i]['runtime']);
        $csv->data[$i]['runtime'] = $x[0]*60 + $x[1];
        $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
        $D->Line($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
                 $D->ScreenY($s_depth),
                 $D->ScreenX($toff+$csv->data[$i]["runtime"]),
                 $D->ScreenY($e_depth),
                 "00BBBB",3,"","");
        if ($i+1<$dc) {
          if (preg_match("|[1234567890]|",$csv->data[$i+1]['depth'])) $end = $csv->data[$i+1]['depth'];
          else continue;
        } else $end = 0;
        if ($s_depth==$e_depth)
          $D->Line($D->ScreenX($toff+$csv->data[$i]["runtime"]),
                   $D->ScreenY($e_depth),
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
      if (!preg_match("|[0123456789]|",$csv->data[$i]['depth'])) continue;
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
      if (!preg_match("|[0123456789]|",$csv->data[$i]['depth'])) { // Ascent
        $s_depth = $csv->data[$i-1]['depth'];
        $e_depth = floatval($csv->data[$i+1]['depth']);
      } else { // normal
        $s_depth = $e_depth = floatval($csv->data[$i]['depth']);
      }
      $D->Area($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
               $D->ScreenY($s_depth),
               $D->ScreenX($toff+$csv->data[$i]['runtime']),
               $D->ScreenY($e_depth),
               "99ccff",$base,"","");
      $D->Line($D->ScreenX($toff+$csv->data[$i]['runtime']-$csv->data[$i]['time']),
               $D->ScreenY($s_depth),
               $D->ScreenX($toff+$csv->data[$i]["runtime"]),
               $D->ScreenY($e_depth),
               "00BBBB",3,"","");
      if ($i+1<$dc) {
        if (preg_match("|[1234567890]|",$csv->data[$i+1]['depth'])) $end = $csv->data[$i+1]['depth'];
        else continue;
      } else $end = 0;
      if ($s_depth==$e_depth)
        $D->Line($D->ScreenX($toff+$csv->data[$i]["runtime"]),
                 $D->ScreenY($e_depth),
                 $D->ScreenX($toff+$csv->data[$i]["runtime"]),
                 $D->ScreenY($end),
                 "00BBBB",1,"","");
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    ImageDestroy($D->Img); 
  } // end method schedule

#---------------------------------------------------------[ Dives per Year ]---
  /** Check whether to update the dive stats graph do it if necessary
   * @method divesCheck
   */
  function divesCheck() {
    GLOBAL $pdl;
    $csvfile   = $pdl->config->datadir."logbook.csv";
    $graphfile = $pdl->config->user_path . "profiles/divestat.png";
    $mapfile   = $pdl->config->user_path . "profiles/divestat.map";
    if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
      $this->dives();
    }
  }

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
      if (isset($dives[$i])) $data[$k] = $dives[$i];
      else $data[$k] = 0;
      $year[$k] = $i;
      if ($data[$k]>$max_dives) $max_dives = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }
    $D->SetFrame(60, 10, $width-20, $height-30);
    if ($years<5)
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($min_date+4,12,31,0,0,0), 0, $max_dives*1.1);
    else
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($max_date-1,12,31,0,0,0), 0, $max_dives*1.1); 
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
    if ($years>30) $D->BFont = -1;
    elseif ($years>25) $D->BFont = -2;
    elseif ($years>20) $D->BFont = 1;
    elseif ($years>15) $D->BFont = 2;
    for($i=1;$i<$years;$i++) {
      if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>35)) $dispval = "";
      else $dispval = $data[$i-1];
      $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".$data[$i-1]);
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  }

#------------------------------------------------------[ Divetime per Year ]---
  /** Check whether to update the divetime stats graph do it if necessary
   * @method divetimeCheck
   */
  function divetimeCheck() {
    GLOBAL $pdl;
    $csvfile   = $pdl->config->datadir."logbook.csv";
    $graphfile = $pdl->config->user_path . "profiles/timestat.png";
    $mapfile   = $pdl->config->user_path . "profiles/timestat.map";
    if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
      $this->divetime();
    }
  }

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
      if (isset($divetime[$i])) $data[$k] = $divetime[$i];
      else $data[$k] = 0;
      $year[$k] = $i;
      if ($data[$k]>$max_time) $max_time = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image."); 
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }
    $D->SetFrame(60, 10, $width-20, $height-30);
    if ($years<5) $edate = $min_date +4;
    else $edate = $max_date -1;
    if ($max_time < 600) {
      $D->YScale = "'";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($edate,12,31,0,0,0), 0, $max_time*1.1);
    } else {
      $D->YScale = "h";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($edate,12,31,0,0,0), 0, ($max_time*1.1)/60);
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
    if ($years>25) $D->BFont = -1;
    elseif ($years>15) $D->BFont = -2;
    elseif ($years>10) $D->BFont = 1;
    for($i=1;$i<$years;$i++) {
      if ($max_time < 600) {
        if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>35)) $dispval = "";
        else $dispval = $data[$i-1];
        $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".$data[$i-1]."min");
      } else {
        if ($pdl->config->values_in_statgraphs == "no" || ($pdl->config->values_in_statgraphs == "auto" && $years>35)) $dispval = "";
        else $dispval = round($data[$i-1]/60,1);
        $D->Box($D->ScreenX($time[$i-1]+0.05*$year_in_s),$D->ScreenY(round($data[$i-1]/60,1)),$D->ScreenX($time[$i]-0.05*$year_in_s),$D->ScreenY(0),"3333ff",$dispval,"ffffff","$boxborder","",$year[$i-1].": ".round($data[$i-1]/60,1)."h");
      }
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 
  }

#------------------------------------------------------------[ Depth Stats ]---
  /** Check whether to update the depth stats graph do it if necessary
   * @method depthCheck
   */
  function depthCheck() {
    GLOBAL $pdl;
    $csvfile   = $pdl->config->datadir."logbook.csv";
    $graphfile = $pdl->config->user_path . "profiles/depthstat.png";
    $mapfile   = $pdl->config->user_path . "profiles/depthstat.map";
    if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
      $this->depth();
    }
  }

  /** Generate depth graph
   * @method depth
   */
  function depth() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/depthstat.png";
    $profilemap = $pdl->config->user_path . "profiles/depthstat.map"; // not used yet
    if (empty($height)) $height = 200;
    if (empty($width))  $width  = $height+130;

    // prepare data
    $data = $pdl->db->get_dives("","",FALSE,"depth","ASC");
    $dc = count($data);
    $datastr = explode(" ",$data[$dc-1]["depth"]);
    $unit    = $datastr[1];
    $max_depth = floatval($data[$dc-1]["depth"]);
    $ceil = $rceil = ceil(floatval($max_depth)/10)*10;

    // calculate base data
    $sum[$rceil] = $rfloor = 0;
    for ($i=0;$i<$dc;++$i) {
      $val = floatval($data[$i]["depth"]);
      if ($val==0) ++$val; // shift Robs special dives (0' at 0m)
      while ($val > $rfloor && $rfloor < 130) { $rfloor += DEPTH_STAT_INTERVAL; $sum[$rfloor] = 0; }
      ++$sum[$rfloor];
    }
    $max_val = 0;
    foreach($sum as $val) if ($val > $max_val) $max_val = $val;

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image.");
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }

    // Graph
    $base=$height/2;
    $max = $ceil/DEPTH_STAT_INTERVAL;
    $angle0 = 1;
    // Graph colors
    $color = array("5" => "#f5f5f5",   // WhiteSmoke
                   "10" => "#f0e68c",  // Khaki
                   "15" => "#ffa500",  // Orange
                   "20" => "#00fa9a",  // MediumSpringGreen
                   "25" => "#3cb371",  // MediumSeaGreen
                   "30" => "#006400",  // DarkGreen
                   "35" => "#008080",  // Teal
                   "40" => "#00ffff",  // Aqua
                   "45" => "#1e90ff",  // DodgerBlue
                   "50" => "#0000cd",  // MediumBlue
                   "55" => "#00008b",  // DarkBlue
                   "60" => "#191970",  // MidnightBlue
                   "65" => "#483d8b",  // DarkSlateBlue
                   "70" => "#6a5acd",  // SlateBlue
                   "75" => "#8a2be2",  // BlueViolett
                   "80" => "#9400d3",  // DarkViolett
                   "85" => "#8b008b",  // DarkMagenta
                   "90" => "#b22222",  // FireBrick
                   "95" => "#dc143c",  // Crimson
                   "100" => "#ff0000", // Red
                   "105" => "#d2b48c", // Tan
                   "110" => "#d3d3d3", // LightGray
                   "115" => "#bebebe", // Gray
                   "120" => "#a9a9a9", // DarkGrey
                   "125" => "#696969", // DimGray
                   "130" => "#000000");// Black
    $tcolor = array("5" => "#000000", "10" => "#000000", "15" => "#000000", "20" => "#000000",
                    "25" => "#ffffff", "30" => "#ffffff", "35" => "#ffffff", "40" => "#000000",
                    "45" => "#000000", "50" => "#ffffff", "55" => "#ffffff", "60" => "#ffffff",
                    "65" => "#ffffff", "60" => "#ffffff", "65" => "#ffffff", "70" => "#ffffff",
                    "75" => "#ffffff", "80" => "#ffffff", "85" => "#ffffff", "90" => "#ffffff",
                    "95" => "#ffffff", "95" => "#ffffff", "100" => "#ffffff", "105" => "#000000",
                    "110" => "#000000", "115" => "#000000", "120" => "#000000",
                    "125" => "#ffffff", "130" => "#ffffff"); // text colors
    $rows = $missing = 0;
    for ($i=0;$i<$max;++$i) { // get real row count
      if (!empty($sum[$i*DEPTH_STAT_INTERVAL])) ++$rows;
    }
    if ($rows>15) $D->BFont = 1;
    elseif ($rows>11) $D->BFont = 2;
    else $D->BFont = 3;

    for ($i=0;$i<=$max;++$i) {
      if (empty($sum[$i*DEPTH_STAT_INTERVAL])) { ++$missing; continue; }
      # Pie (XCenter,YCenter,Offset,Radius,Angle0,Angle1,Color[,tooltip]
      $angle1 = $sum[$i*DEPTH_STAT_INTERVAL] * 100/$dc;
      $tooltip = ($i-1)*DEPTH_STAT_INTERVAL ."-". $i*DEPTH_STAT_INTERVAL . $unit .": ".$sum[$i*DEPTH_STAT_INTERVAL];
      $D->Pie($base,$base,0,$base*0.9,$angle0*3.6,($angle0+$angle1)*3.6,$color[$i*DEPTH_STAT_INTERVAL],$tooltip);
      # Bar(left,top,right,bottom,color,text,textcolor)
      $row = $i - $missing;
      switch($D->BFont) {
        case 3 :
          $adjustY = ($height - (($rows+1)*15) +15)/2;
          $D->Bar($height+20,($row-1)*18+$adjustY,$height+120,($row-1)*18+15+$adjustY,$color[$i*DEPTH_STAT_INTERVAL],$tooltip,$tcolor[$i*DEPTH_STAT_INTERVAL]);
          # Bar(20px from cake,15px bar +3px space,100px width,add 15px bar,color...) // for BFont=3
          break;
        case 2:
          $adjustY = ($height - (($rows+1)*14) +14)/2;
          $D->Bar($height+20,($row-1)*17+$adjustY,$height+120,($row-1)*17+14+$adjustY,$color[$i*DEPTH_STAT_INTERVAL],$tooltip,$tcolor[$i*DEPTH_STAT_INTERVAL]);
          break;
        case 1:
          $adjustY = ($height - (($rows+1)*10) +10)/2;
          $D->Bar($height+20,($row-1)*13+$adjustY,$height+120,($row-1)*13+10+$adjustY,$color[$i*DEPTH_STAT_INTERVAL],$tooltip,$tcolor[$i*DEPTH_STAT_INTERVAL]);
          break;
      }
      $angle0 += $angle1;
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  } // end depth

#------------------------------------------------------[ Temperature Stats ]---
  /** Check whether to update the temperature stats graph do it if necessary
   * @method temperatureCheck
   */
  function temperatureCheck() {
    GLOBAL $pdl;
    $csvfile   = $pdl->config->datadir."logbook.csv";
    $graphfile = $pdl->config->user_path . "profiles/tempstat.png";
    $mapfile   = $pdl->config->user_path . "profiles/tempstat.map";
    if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
      $this->temperature();
    }
  }

  /** Generate temperature graph
   * @method temperature
   */
  function temperature() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/tempstat.png";
    $profilemap = $pdl->config->user_path . "profiles/tempstat.map"; // not used yet
    if (empty($height)) $height = 200;
    if (empty($width))  $width  = $height+130;

    // graph colors
    $color = array("0" => "#f5f5f5",  // WhiteSmoke
                   "5" => "#00bfff",  // DeepSkyBlue
                   "10" => "#4169e1", // RoyalBlue
                   "15" => "#00fa9a", // MediumSpringGreen
                   "20" => "#eee8aa", // PaleGoldenRod
                   "25" => "#d2b48c", // Tan
                   "30" => "#ffa500", // Orange
                   "35" => "#d2691e", // Chocolade
                   "40" => "#dc143c");// Crimson
    $tcolor = array("0" => "#000000", "5" => "#000000", "10" => "#ffffff", "15" => "#000000", "20" => "#000000",
                    "25" => "#000000", "30" => "#000000", "35" => "#000000", "40" => "#ffffff"); // text colors

    // prepare data
    $data = $pdl->db->get_dives("","",FALSE,"watertemp","ASC");
    $dc = count($data);
    $datastr = explode(" ",$data[0]["watertemp"]);
    $unit    = $datastr[1];
    $max_temp = floatval($data[$dc-1]["watertemp"]);
    $ceil = $rceil = ceil(floatval($max_temp)/10)*10;
    $min_temp = floatval($data[0]["watertemp"]);
    $floor = $rfloor = floor(floatval($min_temp)/10)*10;

    // calculate base data
    $sum[$rfloor] = $ignored = 0;
    for ($i=0;$i<$dc;++$i) {
      $val = floatval($data[$i]["watertemp"]);
      if ($GLOBALS["ignore_zero_degrees"] && $val == 0) {
        ++$ignored;
        continue;
      }
      while ($val > $rfloor && $rfloor < 40) { $rfloor += TEMP_STAT_INTERVAL; $sum[$rfloor] = 0; }
      ++$sum[$rfloor];
    }
    $max_val = 0;
    foreach($sum as $val) if ($val > $max_val) $max_val = $val;

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image.");
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }

    // Graph
    $base=$height/2;
    $max = $ceil/TEMP_STAT_INTERVAL;
    $angle0 = 1;
    $rows = $missing = 0;
    for ($i=0;$i<$max;++$i) { // get real row count
      if (!empty($sum[$i*TEMP_STAT_INTERVAL])) ++$rows;
    }
    if ($rows>15) $D->BFont = 1;
    elseif ($rows>11) $D->BFont = 2;
    else $D->BFont = 3;

    for ($i=0;$i<=$max;++$i) {
      if (empty($sum[$i*TEMP_STAT_INTERVAL])) { ++$missing; continue; }
      # Pie (XCenter,YCenter,Offset,Radius,Angle0,Angle1,Color[,tooltip]
      $angle1 = $sum[$i*TEMP_STAT_INTERVAL] * 100/($dc-$ignored);
      if ($i>0) {
        $tooltip = ($i-1)*TEMP_STAT_INTERVAL ."-". $i*TEMP_STAT_INTERVAL . " $unit: ". $sum[$i*TEMP_STAT_INTERVAL];
      } else {
        $tooltip = "<=". $i*TEMP_STAT_INTERVAL . " $unit: ". $sum[$i*TEMP_STAT_INTERVAL];
      }
      $D->Pie($base,$base,0,$base*0.9,$angle0*3.6,($angle0+$angle1)*3.6,$color[$i*TEMP_STAT_INTERVAL],$tooltip);
      # Bar(left,top,right,bottom,color,text,textcolor)
      $row = $i - $missing;
      switch($D->BFont) {
        case 3 :
          $adjustY = ($height - (($rows+1)*15) +15)/2;
          $D->Bar($height+20,($row-1)*18+$adjustY,$height+120,($row-1)*18+15+$adjustY,$color[$i*TEMP_STAT_INTERVAL],str_replace("°","�",$tooltip),$tcolor[$i*TEMP_STAT_INTERVAL]);
          # Bar(20px from cake,15px bar +3px space,100px width,add 15px bar,color...) // for BFont=3
          break;
        case 2:
          $adjustY = ($height - (($rows+1)*14) +14)/2;
          $D->Bar($height+20,($row-1)*17+$adjustY,$height+120,($row-1)*17+14+$adjustY,$color[$i*TEMP_STAT_INTERVAL],str_replace("°","�",$tooltip),$tcolor[$i*TEMP_STAT_INTERVAL]);
          break;
        case 1:
          $adjustY = ($height - (($rows+1)*10) +10)/2;
          $D->Bar($height+20,($row-1)*13+$adjustY,$height+120,($row-1)*13+10+$adjustY,$color[$i*TEMP_STAT_INTERVAL],str_replace("°","�",$tooltip),$tcolor[$i*TEMP_STAT_INTERVAL]);
          break;
      }
      $angle0 += $angle1;
    }
    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  } // end temperature

#-----------------------------------------------------[ DiveDuration Stats ]---
  /** Check whether to update the duration stats graph do it if necessary
   * @method durationCheck
   */
  function durationCheck() {
    GLOBAL $pdl;
    $csvfile   = $pdl->config->datadir."logbook.csv";
    $graphfile = $pdl->config->user_path . "profiles/durastat.png";
    $mapfile   = $pdl->config->user_path . "profiles/durastat.map";
    if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
      $this->duration();
    }
  }

  /** Generate graph for dive duration
   * @method duration
   */
  function duration() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) return; // die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/durastat.png";
    $profilemap = $pdl->config->user_path . "profiles/durastat.map"; // not used yet
    if (empty($height)) $height = 200;
    if (empty($width))  $width  = $height+130;

    // graph colors
    $color = array("10" => "#f5f5f5",  // WhiteSmoke
                   "20" => "#ffa500",  // Orange
                   "30" => "#00ff00",  // Lime
                   "40" => "#00ffff",  // Aqua
                   "50" => "#1e90ff",  // DodgerBlue
                   "60" => "#0000cd",  // MediumBlue
                   "70" => "#6a5acd",  // SlateBlue
                   "80" => "#9400d3",  // DarkViolett
                   "90" => "#b22222",  // FireBrick
                   "100" => "#dc143c",  // Crimson
                   "110" => "#ff0000", // Red
                   "120" => "#d2b48c", // Tan
                   "130" => "#bebebe");// Gray
    $tcolor = array("10" => "#000000", "20" => "#000000", "30" => "#000000", "40" => "#000000",
                    "50" => "#ffffff", "60" => "#ffffff", "70" => "#ffffff", "80" => "#ffffff",
                    "90" => "#ffffff", "100" => "#ffffff", "110" => "#ffffff", "120" => "#000000",
                    "130" => "#000000"); // text colors

    // prepare data
    $data = $pdl->db->get_dives("","",FALSE,"divetime","ASC");
    $dc = count($data);
    $datastr = explode(" ",$data[0]["divetime"]);
    $unit    = $datastr[1];
    $max_time = floatval($data[$dc-1]["divetime"]);
    $ceil = $rceil = ceil(floatval($max_time)/TIME_STAT_INTERVAL)*TIME_STAT_INTERVAL;
    $min_time = floatval($data[0]["divetime"]);
    $floor = $rfloor = floor(floatval($min_time)/TIME_STAT_INTERVAL)*TIME_STAT_INTERVAL;

    // calculate base data
    $sum[$rfloor] = 0;
    for ($i=0;$i<$dc;++$i) {
      $val = floatval($data[$i]["divetime"]);
      if ($val==0) ++$val; // shift Robs 0' dives to be ~1'
      while ($val > $rfloor && $rfloor < 130) { $rfloor += TIME_STAT_INTERVAL; $sum[$rfloor] = 0; }
      ++$sum[$rfloor];
    }
    $max_val = 0;
    foreach($sum as $val) if ($val > $max_val) $max_val = $val;

    // Create the Diagram
    $D=new Diagram(); 
    if (!$D->Img=@ImageCreate($width, $height)) return; // or die("Cannot create a new GD image.");
    if ($GLOBALS["graph_bg_transparent"]) {
      $transp = ImageColorAllocateAlpha($D->Img,0,0,0,127);
      ImageFill($D->Img,0,0,$transp);
      ImageSaveAlpha($D->Img,TRUE);
    } else {
      ImageColorAllocate($D->Img, 200,200,200); // background color
    }

    // Graph
    $base=$height/2;
    $angle0 = 0;
    $max = count($sum);
    $rows = 0;
    for ($i=0;$i<$max;++$i) { // get real row count
      if (!empty($sum[$i*TIME_STAT_INTERVAL])) ++$rows;
    }
    $missing = 0;
    if ($rows>15) $D->BFont = 1;
    elseif ($rows>11) $D->BFont = 2;
    else $D->BFont = 3;

    for ($i=0;$i<$max;++$i) {
      if (empty($sum[($i+1)*TIME_STAT_INTERVAL])) { ++$missing; continue; }
      # Pie (XCenter,YCenter,Offset,Radius,Angle0,Angle1,Color[,tooltip]
      $angle1 = $sum[($i+1)*TIME_STAT_INTERVAL] * 100/$dc;
      if ( ($i+1)*TIME_STAT_INTERVAL == 130 ) $tooltip = "&gt; 120 $unit: ". $sum[($i+1)*TIME_STAT_INTERVAL];
      else $tooltip = ($i)*TIME_STAT_INTERVAL ."-". ($i+1)*TIME_STAT_INTERVAL . " $unit: ". $sum[($i+1)*TIME_STAT_INTERVAL];
      $D->Pie($base,$base,0,$base*0.9,$angle0*3.6,($angle0+$angle1)*3.6,$color[($i+1)*TIME_STAT_INTERVAL],$tooltip);
      # Bar(left,top,right,bottom,color,text,textcolor)
      $row = $i - $missing;
      switch($D->BFont) {
        case 3 :
          $adjustY = ($height - (($rows+1)*15) +15)/2;
          $D->Bar($height+20,($row-1)*18+$adjustY,$height+120,($row-1)*18+15+$adjustY,$color[($i+1)*TIME_STAT_INTERVAL],str_replace('&gt;','>',$tooltip),$tcolor[($i+1)*TIME_STAT_INTERVAL]);
          # Bar(20px from cake,15px bar +3px space,100px width,add 15px bar,color...) // for BFont=3
          break;
        case 2:
          $adjustY = ($height - (($rows+1)*14) +14)/2;
          $D->Bar($height+20,($row-1)*17+$adjustY,$height+120,($row-1)*17+14+$adjustY,$color[($i+1)*TIME_STAT_INTERVAL],str_replace('&gt;','>',$tooltip),$tcolor[($i+1)*TIME_STAT_INTERVAL]);
          break;
        case 1:
          $adjustY = ($height - (($rows+1)*10) +10)/2;
          $D->Bar($height+20,($row-1)*13+$adjustY,$height+120,($row-1)*13+10+$adjustY,$color[($i+1)*TIME_STAT_INTERVAL],str_replace('&gt;','>',$tooltip),$tcolor[($i+1)*TIME_STAT_INTERVAL]);
          break;
      }
      $angle0 += $angle1;
    }
    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  } // end duration


} // end class graph

?>
