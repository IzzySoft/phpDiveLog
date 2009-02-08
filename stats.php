<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Dive Statistics                                                           #
 #############################################################################

 # $Id$

 include("inc/includes.inc");
 $title .= ": ".lang("statistics");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"stats.tpl"));
 $t->set_block("template","diveyearblock","yearstat");

 #================================================[ set up navigation tabs ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("stats",TRUE);
 $pdl->tabs->parse();

 #==================================================[ Set up table headers ]===
 $t->set_var("title",lang("dive_stats"));
 $t->set_var("max_depth_name",lang("max_depth").":");
 $t->set_var("max_time_name",lang("max_divetime").":");
 $t->set_var("avg_depth_name",lang("avg_depth").":");
 $t->set_var("avg_time_name",lang("avg_divetime").":");
 $t->set_var("dive_num_name",lang("num_dives").":");
 $t->set_var("cum_time_name",lang("cum_divetime").":");
 $t->set_var("site_num_name",lang("num_sites").":");
 $t->set_var("avg_sd_name",lang("avg_dives_per_site").":");

 #================================================[ Import statistics data ]===
 $stats = $pdl->db->get_stats();
 $t->set_var("max_depth",$stats["max_depth"]);
 $t->set_var("max_time",$stats["max_time"]);
 $t->set_var("avg_depth",$stats["avg_depth"]);
 $t->set_var("avg_time",$stats["avg_time"]);
 $t->set_var("dive_num",$stats["num_dives"]);
 $t->set_var("cum_time",$stats["cum_dive_time"]);
 $t->set_var("site_num",$pdl->db->sites);
 $t->set_var("avg_sd",round($stats["num_dives"] / $pdl->db->sites,3));

 #========================================================[ Include Graphs ]===
 if (function_exists("imagepng") && is_writable($pdl->config->user_path . "profiles")) {
   $csvfile   = $pdl->config->datadir."logbook.csv";
   #------------------------------------------------------[ Dives per Year ]---
   $graphfile = $pdl->config->user_path . "profiles/divestat.png";
   $mapfile   = $pdl->config->user_path . "profiles/divestat.map";
   if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->dives();
   }
   $t->set_var("ytitle",lang("year_stat"));
   $t->set_var("yearstat_png",$pdl->config->user_url."profiles/divestat.png");
   $t->set_var("yearstat_alt","YearStat");
   if (file_exists($mapfile)) {
     $t->set_var("yearmap","<map name='divestat' id='divestat'>".file_get_contents($mapfile)."</map>");
     $t->set_var("usemap","USEMAP='#divestat'");
   } else {
     $t->set_var("yearmap","");
     $t->set_var("usemap","");
   }
   $t->parse("yearstat","diveyearblock");

   #---------------------------------------------------[ DiveTime per Year ]---
   $graphfile = $pdl->config->user_path . "profiles/timestat.png";
   $mapfile   = $pdl->config->user_path . "profiles/timestat.map";
   if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->divetime();
   }
   $t->set_var("ytitle",lang("time_stat"));
   $t->set_var("yearstat_png",$pdl->config->user_url."profiles/timestat.png");
   $t->set_var("yearstat_alt","TimeStat");
   if (file_exists($mapfile)) {
     $t->set_var("yearmap","<map name='timestat' id='timestat'>".file_get_contents($mapfile)."</map>");
     $t->set_var("usemap","USEMAP='#timestat'");
   } else {
     $t->set_var("yearmap","");
     $t->set_var("usemap","");
   }
   $t->parse("yearstat","diveyearblock",TRUE);

   #-----------------------------------------------------[ Dives per Depth ]---
   $graphfile = $pdl->config->user_path . "profiles/depthstat.png";
   $mapfile   = $pdl->config->user_path . "profiles/depthstat.map";
   if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->depth();
   }
   $t->set_var("ytitle",lang("depth_stat"));
   $t->set_var("yearstat_png",$pdl->config->user_url."profiles/depthstat.png");
   $t->set_var("yearstat_alt","DepthStat");
   if (file_exists($mapfile)) {
     $t->set_var("yearmap","<map name='depthstat' id='depthstat'>".file_get_contents($mapfile)."</map>");
     $t->set_var("usemap","USEMAP='#depthstat'");
   } else {
     $t->set_var("yearmap","");
     $t->set_var("usemap","");
   }
   $t->parse("yearstat","diveyearblock",TRUE);

   #-----------------------------------------------[ Dives per Temperature ]---
   $graphfile = $pdl->config->user_path . "profiles/tempstat.png";
   $mapfile   = $pdl->config->user_path . "profiles/tempstat.map";
   if (!file_exists($graphfile) || filemtime($graphfile) < filemtime($csvfile)) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->temperature();
   }
   $t->set_var("ytitle",lang("temp_stat"));
   $t->set_var("yearstat_png",$pdl->config->user_url."profiles/tempstat.png");
   $t->set_var("yearstat_alt","TemperatureStat");
   if ($ignore_zero_degrees && $ignore_zero_degrees_comment) {
     $comment = lang("stat_ignored_zero_degrees");
   } else {
     $comment = "";
   }
   if (file_exists($mapfile)) {
     $t->set_var("yearmap","<map name='tempstat' id='tempstat'>".file_get_contents($mapfile)."</map>".$comment);
     $t->set_var("usemap","USEMAP='#tempstat'");
   } else {
     $t->set_var("yearmap",$comment);
     $t->set_var("usemap","");
   }
   $t->parse("yearstat","diveyearblock",TRUE);

 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>