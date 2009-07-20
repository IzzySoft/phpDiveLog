<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Dive Statistics PDF Exporter                                              #
 #############################################################################

 # $Id$

include("inc/includes.inc");
$title .= ": ".lang("statistics");
# include("inc/header.inc");

$t = new Template($pdl->config->base_path."templates/aqua/");
$t->set_file(array("template"=>"stats_pdf.tpl"));
$t->set_block("template","topleftblock","topleft");
$t->set_block("template","toprightblock","topright");
$t->set_block("template","sumblock","sum");
$t->set_block("template","lgraphblock","lgraph");
$t->set_block("template","rgraphblock","rgraph");

#============================================================[ Header Data ]===
$t->set_var("stats_name",lang("dive_stats"));
$t->set_var("date",date('Y-m-d'));
$t->parse("topleft","topleftblock");
$t->set_var("topright","");

#========================================================[ Statistics Data ]===
$stats = $pdl->db->get_stats();
$t->set_var("item_name",lang("max_depth").":");
$t->set_var("item_data",$stats["max_depth"]);
$t->parse("sum","sumblock");
$t->set_var("item_name",lang("max_divetime").":");
$t->set_var("item_data",$stats["max_time"]);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("avg_depth").":");
$t->set_var("item_data",$stats["avg_depth"]);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("avg_divetime").":");
$t->set_var("item_data",$stats["avg_time"]);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("num_dives").":");
$t->set_var("item_data",$stats["num_dives"]);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("cum_divetime").":");
$t->set_var("item_data",preg_replace('!(\d+)[^\d]+(\d+)[^\d]+!','$1:$2 h',$stats["cum_dive_time"]));
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("num_sites").":");
$t->set_var("item_data",$pdl->db->sites);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("avg_dives_per_site").":");
$t->set_var("item_data",round($stats["num_dives"] / $pdl->db->sites,3));
$t->parse("sum","sumblock",TRUE);

#========================================================[ Include Graphs ]===
if (function_exists("imagepng") && is_writable($pdl->config->user_path . "profiles")) {
  include_once("inc/class.graph.inc");
  $graph = new graph();
  $t->set_var("graph_width","160");
  $t->set_var("graph_height","97");
  #------------------------------------------------------[ Dives per Year ]---
  $graphfile = $pdl->config->user_path . "profiles/divestat.png";
  $mapfile   = $pdl->config->user_path . "profiles/divestat.map";
  $graph->divesCheck();
  $t->set_var("graph_name",lang("year_stat"));
  $t->set_var("graph_src",$pdl->config->user_url."profiles/divestat.png");
  $t->set_var("graph_alt","YearStat");
  $t->parse("lgraph","lgraphblock");

  #---------------------------------------------------[ DiveTime per Year ]---
  $graphfile = $pdl->config->user_path . "profiles/timestat.png";
  $mapfile   = $pdl->config->user_path . "profiles/timestat.map";
  $graph->divetimeCheck();
  $t->set_var("graph_name",lang("time_stat"));
  $t->set_var("graph_src",$pdl->config->user_url."profiles/timestat.png");
  $t->set_var("graph_alt","TimeStat");
  $t->parse("lgraph","lgraphblock",TRUE);

  #-----------------------------------------------------[ Dives per Depth ]---
  $graphfile = $pdl->config->user_path . "profiles/depthstat.png";
  $mapfile   = $pdl->config->user_path . "profiles/depthstat.map";
  $graph->depthCheck();
  $t->set_var("graph_name",lang("depth_stat"));
  $t->set_var("graph_src",$pdl->config->user_url."profiles/depthstat.png");
  $t->set_var("graph_alt","DepthStat");
  $t->parse("rgraph","rgraphblock");

  #-----------------------------------------------[ Dives per Temperature ]---
  $graphfile = $pdl->config->user_path . "profiles/tempstat.png";
  $mapfile   = $pdl->config->user_path . "profiles/tempstat.map";
  $graph->temperatureCheck();
  $t->set_var("graph_name",lang("temp_stat"));
  $t->set_var("graph_src",$pdl->config->user_url."profiles/tempstat.png");
  $t->set_var("graph_alt","TemperatureStat");
  if ($ignore_zero_degrees && $ignore_zero_degrees_comment) {
    $comment = lang("stat_ignored_zero_degrees");
  } else {
    $comment = "";
  }
  $t->parse("rgraph","rgraphblock",TRUE);

  #--------------------------------------------------[ Dives per Duration ]---
  $graphfile = $pdl->config->user_path . "profiles/durastat.png";
  $mapfile   = $pdl->config->user_path . "profiles/durastat.map";
  $graph->durationCheck();
  $t->set_var("graph_name",lang("dura_stat"));
  $t->set_var("graph_src",$pdl->config->user_url."profiles/durastat.png");
  $t->set_var("graph_alt","DurationStat");
  $t->parse("rgraph","rgraphblock",TRUE);

} // end function_exists(imagepng)

#================================================[ create new PDF document ]===
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
$pdf->SetKeywords(lang("dive_stats").', '.$pdl->params->diver.', dive');

#-------------------------------------------------------[ Generate content ]---
# $t->pparse("out","template");
$out = $t->parse("out","template");
$out = preg_replace('|\s+|',' ',$out); // TCPDF treats all spaces as hard spaces
$out = preg_replace('|\s+<T|i','<T',$out);// TCPDF converts each table cell into a separate "box"
$out = preg_replace('|\s+</T|i','</T',$out);// TCPDF converts each table cell into a separate "box"

#if ($nr%2)
$pdf->SetMargins(PDF_PAGE_GUTTER, PDF_MARGIN_TOP_NOHEAD, PDF_PAGE_MARGIN);
#else $pdf->SetMargins(PDF_PAGE_MARGIN, PDF_MARGIN_TOP_NOHEAD, PDF_PAGE_GUTTER);
$pdf->AddPage();
$pdf->writeHTML($out,true,0,true,0);
$pdf->Output("stats.pdf", 'I');

?>