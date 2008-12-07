<?php
#############################################################################
# phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
# written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
# http://www.izzysoft.de/                                                   #
# ------------------------------------------------------------------------- #
# This program is free software; you can redistribute and/or modify it      #
# under the terms of the GNU General Public License (see doc/LICENSE)       #
# ------------------------------------------------------------------------- #
# Generating graphs and diagrams from CSV data                              #
#############################################################################

/* $Id$ */

include ("inc/diagram.inc");

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

#----------------------------------------------------------[ Dive Profiles ]---
  /** Generate dive profile graph
   * @method profile
   * @param integer divenr dive number
   * @return array filenames (png: the image, map: imagemap data file)
   */
  function profile($nr) {
    GLOBAL $pdl;
    // setup data
    while (strlen($nr)<5) $nr = "0$nr";
    $csvfile = $pdl->config->datadir."dive${nr}_profile.csv";
    if (!file_exists($csvfile)) die("CSV File '$csvfile' was not found.<br>");
    $profilepng = $pdl->config->user_path . "profiles/dive${nr}_profile.png";
    $profilemap = $pdl->config->user_path . "profiles/dive${nr}_profile.map";
    // check whether we have something to do
    //if (file_exists($profilepng) && filemtime($profilepng) > filemtime($csvfile))
    //  return array(png=>$profilepng,map=>$profilemap);

    // still here? So we have to create graph and map:
    if (empty($width))  $width  = 468;
    if (empty($height)) $height = 300;
    $csv = new csv(";",'"',TRUE);
    $csv->import($csvfile);

    // setup the graph
    $time = array();
    $max_depth = 0;
    for($i=0;$i<count($csv->data);$i++) {
      // Maximaltiefe ermitteln
      $csv->data[$i]['depth'] = floatval($csv->data[$i]['depth']);
      if($csv->data[$i]['depth'] > $max_depth) $max_depth = $csv->data[$i]['depth'];
      // Zeiten berechnen
      $x = explode(":",$csv->data[$i]['time']);
      $time[] = $x[0]*60 + $x[1];
      // Correct image height for small depths
      if($max_depth < 5)  $height = 200;
      if($max_depth < 10) $height = 250;
    }
    $divetime = $time[count($time)-1]; // dive time is the last value

    // Create the Diagram
    $D=new Diagram(); 
    $D->Img=@ImageCreate($width, $height) or die("Cannot create a new GD image."); 
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
    // Draw Graph
    $D->Draw("#3333ff", "#000000", false);
    $base=$D->ScreenY(0.01);
    $toff=UTC(2000,01,01,0,0,0);
    for($i=1;$i<count($csv->data);$i++) {
      $D->Area($D->ScreenX($toff+$time[$i-1]),$D->ScreenY($csv->data[$i-1]['depth']),$D->ScreenX($toff+$time[$i]),$D->ScreenY($csv->data[$i]['depth']),"99ccff",$base,"","");
      $D->Line($D->ScreenX($toff+$time[$i-1]),$D->ScreenY($csv->data[$i-1]['depth']),$D->ScreenX($toff+$time[$i]),$D->ScreenY($csv->data[$i]['depth']),"00000",1,"","");
      if ($csv->data[$i]['warning'] != '') {
        $D->Dot($D->ScreenX($toff+$time[$i]), $D->ScreenY($csv->data[$i]['depth']),12, 0, "#ff0000", $csv->data[$i]['warning']);
      }
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  } // end method profile

#---------------------------------------------------------[ Dives per Year ]---
  /** Generate dive stats graph
   * @method dives
   */
  function dives() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) die("CSV File '$csvfile' was not found.<br>");
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
    $year = 0;
    for($i=0;$i<count($csv->data);$i++) {
      // Zeiten berechnen
      $x = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$i]['date']);
      if ($x>$year) $year = $x;
      ++$dives[$year];
    }
    $dives[$max_date+1] = 0;
    ++$max_date;
    $max_dives = 0;
    $year_in_s = 365 * 24 * 3600;
    for ($i=$min_date,$k=0;$i<=$max_date;++$i,++$k) {
      $time[$k] = ($i * $year_in_s - 0.5 * $year_in_s) - (1970 * $year_in_s);
      $data[$k] = $dives[$i];
      if ($data[$k]>$max_dives) $max_dives = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    $D->Img=@ImageCreate($width, $height) or die("Cannot create a new GD image."); 
    ImageColorAllocate($D->Img, 200,200,200); // background color 
    $D->SetFrame(40, 30, $width-20, $height-30); 
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
    else $boxborder = 1;
    for($i=1;$i<$years;$i++) {
      $D->Box($D->ScreenX($time[$i-1]),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]),$D->ScreenY(0),"3333ff",$data[$i-1],"ffffff","$boxborder");
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    // if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  }

#------------------------------------------------------[ Divetime per Year ]---
  /** Generate dive stats graph
   * @method divetime
   */
  function divetime() {
    GLOBAL $pdl;
    $csvfile = $pdl->config->datadir."logbook.csv";
    if (!file_exists($csvfile)) die("CSV File '$csvfile' was not found.<br>");
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
    $year = 0;
    for($i=0;$i<count($csv->data);$i++) {
      // Zeiten berechnen
      $x = preg_replace('|.*(\d{4}).*|','$1',$csv->data[$i]['date']);
      if ($x>$year) $year = $x;
      $divetime[$year] += (int) $csv->data[$i]['divetime'];
    }
    $divetime[$max_date+1] = 0;
    ++$max_date;
    $max_time = 0;
    $year_in_s = 365 * 24 * 3600;
    for ($i=$min_date,$k=0;$i<=$max_date;++$i,++$k) {
      $time[$k] = ($i * $year_in_s - 0.5 * $year_in_s) - (1970 * $year_in_s);
      $data[$k] = $divetime[$i];
      if ($data[$k]>$max_time) $max_time = $data[$k];
    }
    $years = count($data);

    // Create the Diagram
    $D=new Diagram(); 
    $D->Img=@ImageCreate($width, $height) or die("Cannot create a new GD image."); 
    ImageColorAllocate($D->Img, 200,200,200); // background color 
    $D->SetFrame(60, 30, $width-20, $height-30);
    if ($max_time < 600) {
      $D->YScale = "'";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($max_date-1,12,31,0,0,0), 0, $max_time+60);
    } else {
      $D->YScale = "h";
      $D->SetBorder(UTC($min_date-1,01,01,0,0,0), UTC($max_date-1,12,31,0,0,0), 0, ($max_time+60)/60);
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
    else $boxborder = 1;
    for($i=1;$i<$years;$i++) {
      if ($max_time < 600)
      $D->Box($D->ScreenX($time[$i-1]),$D->ScreenY($data[$i-1]),$D->ScreenX($time[$i]),$D->ScreenY(0),"3333ff",$data[$i-1],"ffffff","$boxborder");
      else
      $D->Box($D->ScreenX($time[$i-1]),$D->ScreenY(round($data[$i-1]/60,1)),$D->ScreenX($time[$i]),$D->ScreenY(0),"3333ff",round($data[$i-1]/60,1),"ffffff","$boxborder");
    }

    // Save Image & Data
    ImagePng($D->Img,$profilepng);
    // if (!empty($D->ImgMapData)) file_put_contents($profilemap,$D->ImgMapData);
    ImageDestroy($D->Img); 

  }


} // end class graph

?>
