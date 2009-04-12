<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Generating Logbook Page for a single dive                                 #
 #############################################################################
 # $Id$

#=============================================[ Initialize & Setup PDF Api ]===
require_once(dirname(__FILE__)."/inc/includes.inc");
if (USE_DYN_PROFILE_PNG && PDF_CREATE_MISSING_GRAPH) {
  include("inc/class.graph.inc");
  $graph = new graph();
}
#------------------------------------------[ DiveRecord specific constants ]---
if (isset($_REQUEST["pageno"]) && preg_match('!^[1-9][0-9]*$!',$_REQUEST["pageno"])) $pagenr = $_REQUEST["pageno"];
else $pagenr = $_REQUEST["nr"];
if ($pagenr<1) $pagenr = 1;
$start = $pagenr;
if (isset($_REQUEST["lastnr"]) && preg_match('!^[1-9][0-9]*$!',$_REQUEST["lastnr"])) {
  $end = $_REQUEST["lastnr"];
} else {
  $end = $pagenr;
}
if ($start==$end) define('MULTI_PAGE',FALSE); // just a single page or the entire book?
else define('MULTI_PAGE',TRUE);

#------------------------------------------------[ create new PDF document ]---
require_once(dirname(__FILE__)."/inc/pdf_init.inc");
$pdf = new pdlPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false); 

#------------------------------------------------------[ Setup Page Design ]---
$pdf->setPrintHeader(false);
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);
$pdf->SetAutoPageBreak(FALSE, PDF_MARGIN_BOTTOM);
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO); 
$pdf->setLanguageArray($l); 
#$pdf->SetFont('helvetica', 'BI', 16);
$pdf->SetFont('freesans', '', 10); // provides better UTF8 capabilities

#-----------------------------------------------[ Set Document Information ]---
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor(ucfirst($pdl->params->diver));
$pdf->SetTitle($title);
$pdf->SetSubject($title);
if (MULTI_PAGE) $pdf->SetKeywords('divelog, '.$pdl->params->diver.', dive');

#=======================================================[ Prepare the Page ]===
#---------------------------------------------------------[ Template Setup ]---
$t = new Template($pdl->config->base_path."templates/aqua/");
$t->set_file(array("template"=>"dive_pdf.tpl"));
$t->set_block("template","sumblock","sum");
$t->set_block("template","equiblock","equi");
$t->set_block("template","tankblock","tank");
$t->set_block("template","fotoblock","fotos");
$t->set_block("template","nofotoblock","nofotos");
$t->set_block("template","topleftblock","topleft");
$t->set_block("template","toprightblock","topright");

#==========================================================[ Process Pages ]===
#-------[ Starting with even page number? Shift pages for duplex printing! ]---
if (MULTI_PAGE && preg_match('!^.{1}5$!',PDF_PAGE_FORMAT)) $pdf->duplexInit($start,$end,$_REQUEST["duplex"]);

#----------------------------------------------------------[ Retrieve Data ]---
$records = $pdl->db->get_dives($start-1,$end+1-$start,FALSE,"id","ASC");
$dc = count($records);
for ($i=0;$i<$dc;++$i) $dives[$records[$i]["dive#"]] = $records[$i];
if ($end>$records[$dc-1]["dive#"]) $end = $records[$dc-1]["dive#"];
unset($records);

for ($nr=$start;$nr<=$end;++$nr) {
  $title .= ": ".lang("dive#")." $nr";
  $dive = $dives[$nr];

#-----------------------------------------------------------[ Setup Header ]---
  $t->set_var("divenr",lang("dive#"));
  $t->set_var("dive#",$nr);
  $t->set_var("time",$dive["time"]);
  $t->set_var("date",$dive["date"]);
  $t->set_var("location",$dive["location"]);
  $t->set_var("place",$dive["place"]);
  if ($nr%2) {
    $t->parse("topleft","topleftblock");
    $t->set_var("topright","");
  } else {
    $t->parse("topright","toprightblock");
    $t->set_var("topleft","");
  }

#----------------------------------------------------------[ Setup Summary ]---
  $t->set_var("cond_name",lang("conditions"));
  $t->set_var("item_name",lang("max_depth").":");
  $t->set_var("item_data",$dive["depth"]);
  $t->parse("sum","sumblock");
  $t->set_var("item_name",lang("dive_time").":");
  $t->set_var("item_data",$dive["divetime"]);
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("buddy").":");
  $t->set_var("item_data",$dive["buddy"]);
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("rating").":");
  if ($dive["rating"]=="-")  $starwid = 8;
  else $starwid = $dive["rating"]*8;
  $t->set_var("item_data","<img src='".$pdl->config->base_url."templates/aqua/images/".$dive["rating"]."star.gif"."' alt='Rating:".$dive["rating"]."' HEIGHT='8px' WIDTH='${starwid}px' />");
  $t->parse("sum","sumblock",TRUE);
  $nrpad = str_pad($nr,5,"0",STR_PAD_LEFT);
  if (USE_DYN_PROFILE_PNG && PDF_CREATE_MISSING_GRAPH) $graph->profileCheck($nr);
  if (USE_DYN_PROFILE_PNG && file_exists($pdl->config->user_path . "profiles/dive${nrpad}_profile.png"))
    $t->set_var("prof_img",$pdl->config->user_url . "profiles/dive${nrpad}_profile.png");
  elseif ( strlen($prof_img=$pdl->file->getProfPic($nr)) ) {
    $t->set_var("prof_img",$prof_img);
  } else {
    switch (PDF_NO_PROFILE) {
      case "dummy" : $t->set_var("prof_img",$pdl->config->base_url."templates/aqua/images/dummy_profile.png"); break;
      default      : $t->set_var("prof_img",$pdl->config->base_url."templates/aqua/images/_blank.png");
    }
  }

#-------------------------------------------------------[ Setup Conditions ]---
  $t->set_var("item_name",lang("visibility").":");
  $t->set_var("item_data",$dive["visibility"]);
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("water_temp").":");
  $t->set_var("item_data",$dive["watertemp"]);
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("air_temp").":");
  $t->set_var("item_data",$dive["airtemp"]);
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("current").":");
  $t->set_var("item_data",ucfirst($dive["current"]));
  $t->parse("sum","sumblock",TRUE);
  $t->set_var("item_name",lang("workload").":");
  $t->set_var("item_data",ucfirst($dive["workload"]));
  $t->parse("sum","sumblock",TRUE);

#--------------------------------------------------------[ Setup Equipment ]---
  $t->set_var("equi_name",lang("equipment"));
  $t->set_var("item_name",lang("suit").":");
  $t->set_var("item_data",$dive["suittype"].", ".$dive["suitname"]);
  $t->parse("equi","equiblock");
  if (!empty($userdef1) && !empty($dive["userdef1"])) {
    $t->set_var("item_name","${userdef1}:");
    $t->set_var("item_data",$dive["userdef1"]);
    $t->parse("equi","equiblock",TRUE);
  }
  if (!empty($userdef2) && !empty($dive["userdef2"])) {
    $t->set_var("item_name","${userdef2}:");
    $t->set_var("item_data",$dive["userdef2"]);
    $t->parse("equi","equiblock",TRUE);
  }
  $t->set_var("item_name",lang("weight").":");
  $t->set_var("item_data",$dive["weight"]);
  $t->parse("equi","equiblock",TRUE);
  #--[ Tank ]--
  $tc = count($dive["tank"]);
  $t->set_var("tank_trans",lang("tank"));
  $t->set_var("tank_name_name",lang("name"));
  $t->set_var("tank_gas_name",lang("gas"));
  $t->set_var("tank_type_name",lang("type"));
  $t->set_var("tank_volume_name",lang("volume"));
  $t->set_var("pressure",lang("pressure"));
  $t->set_var("tank_in_name",lang("tank_in"));
  $t->set_var("tank_out_name",lang("tank_out"));
  for ($i=0;$i<$tc;++$i) {
    $t->set_var("tank_nr",$dive["tank"][$i]->nr);
    $t->set_var("tank_name",$dive["tank"][$i]->name);
    $t->set_var("tank_gas",$dive["tank"][$i]->gas);
    $t->set_var("tank_type",$dive["tank"][$i]->type);
    $t->set_var("tank_volume",$dive["tank"][$i]->volume);
    $t->set_var("tank_in",$dive["tank"][$i]->in);
    $t->set_var("tank_out",$dive["tank"][$i]->out);
    $t->parse("tank","tankblock",$i);
  }

#------------------------------------------------------------[ Setup Notes ]---
  $t->set_var("notes_name",lang("notes"));
  $notes[1] = $pdl->common->nl2br($dive["notes"],1,1);
  $notes[2] = $pdl->file->getNotes($nr,"dive");
  $notb = $notes[1].$pdl->common->nl2br($notes[2]);
  $pdf->notesSubset($notb); // Select the subset defined for PDF export
  $pdf->htmlAdjust($notb); // Some fixup is required for TCPDF - e.g. make sure to use XHTML
  $pdf->notesRestrictLen($notb); // we need to restrict text length so it hopefully fits in
  $t->set_var("notes_text",$notb);

#------------------------------------------------------------[ Setup Fotos ]---
  $t->set_var("fotos_name",lang("fotos"));
  $fotos  = $pdl->file->getDivePix($nr);
  $fc = count($fotos);
  if ($fc>0 && PDF_WITH_FOTOS) {
    for ($i=0;$i<$fc;++$i) {
      if ($i>2) break;
      $t->set_var("foto",$fotos[$i]->url);
      $t->set_var("foto_text",$fotos[$i]->desc);
      $t->parse("fotos","fotoblock",$i);
    }
    $t->set_var("nofotos","");
  } else {
    $t->parse("nofotos","nofotoblock");
    $t->set_var("fotos","");
  }

#-----------------------------------------------[ Setup Marks & Signatures ]---
  $t->set_var("mark_name",lang("mark_name"));
  $t->set_var("mark1_name",lang("buddy"));
  $t->set_var("mark2_name",lang("divemaster"));
  $t->set_var("mark3_name",lang("instructor"));

#=======================================================[ Generate Content ]===
  $out = $t->parse("out","template");
  $out = preg_replace('|\s+|',' ',$out); // TCPDF treats all spaces as hard spaces
  $out = preg_replace('|\s+<T|i','<T',$out);// TCPDF converts each table cell into a separate "box"
  $out = preg_replace('|\s+</T|i','</T',$out);// TCPDF converts each table cell into a separate "box"

  if ($nr%2) $pdf->SetMargins(PDF_PAGE_GUTTER, PDF_MARGIN_TOP_NOHEAD, PDF_PAGE_MARGIN);
  else $pdf->SetMargins(PDF_PAGE_MARGIN, PDF_MARGIN_TOP_NOHEAD, PDF_PAGE_GUTTER);
  $pdf->AddPage();
  $pdf->setPrintFooter(TRUE);
  $pdf->writeHTML($out,true,0,true,0);
}

#=========================================================[ Create the PDF ]===
#-----------------------------------------[ Shuffle pages for Duplex Print ]---
if (MULTI_PAGE && preg_match('!^.{1}5$!',PDF_PAGE_FORMAT)) $pdf->duplexFinish($start,$end,$_REQUEST["duplex"]);

#-------------------------------------------------------------[ Output PDF ]---
if ($start==$end) $pdf->Output("dive_${start}.pdf", 'I');
else $pdf->Output("dives.pdf", 'I');

?>