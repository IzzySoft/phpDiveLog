<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display a single dive record                                              #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 include("inc/includes.inc");
 $nr = $_GET["nr"];
 $title .= ": ".lang("dive#")." $nr";
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"dive.tpl"));
 $t->set_block("template","sumblock","sum");
 $t->set_block("template","condblock","cond");
 $t->set_block("template","equiblock","equi");
 $t->set_block("template","tankblock","tank");
 $t->set_block("template","scheduleblock","sched");
 $t->set_block("scheduleblock","scheditemblock","scheditem");
 $t->set_block("template","profileblock","profile");
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("fotoblock","fotoitemblock","pic");

 #=================================================[ general template data ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("dives");
 $pdl->tabs->parse();
 $t->set_var("cond_name",lang("conditions"));
 $t->set_var("notes_name",lang("notes"));
 $t->set_var("equi_name",lang("equipment"));

 #==============================================[ Import dive data from DB ]===
 $dive = $pdl->db->get_dive($nr);

 #=============================================[ set up the navigation bar ]===
 if ($prev=$dive["prev_dive#"]) {
   $t->set_var("nav_left",$pdl->link->linkurl("$PHP_SELF?nr=$prev","<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$dive["next_dive#"]) {
   $t->set_var("nav_right",$pdl->link->linkurl("$PHP_SELF?nr=$next","<img src='".$pdl->config->tpl_url."images/right.gif'>"));
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }
 $t->set_var("divenr",lang("dive#"));
 #==================================================[ set up the dive data ]===
 $t->set_var("dive#",$dive["dive#"]);
 $t->set_var("time",$dive["time"]);
 $t->set_var("date",$dive["date"]);
 $t->set_var("location",$dive["location"]);
 $t->set_var("place",$pdl->link->linkurl("site.php?id=".$dive["site_id"],$dive["place"]));
 #--------------------------[ Summary ]---
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
 $t->set_var("item_data","<img src='".$pdl->config->tpl_url."images/".$dive["rating"]."star.gif"."' alt='Rating:".$dive["rating"]."'");
 $t->parse("sum","sumblock",TRUE);
 $t->set_var("item_name","&nbsp;"); // dummy empty line to match the
 $t->set_var("item_data","&nbsp;"); // neighbour table
 $t->parse("sum","sumblock",TRUE);
 #--------------------------[ Conditions ]---
 $t->set_var("item_name",lang("visibility").":");
 $t->set_var("item_data",$dive["visibility"]);
 $t->parse("cond","condblock");
 $t->set_var("item_name",lang("water_temp").":");
 $t->set_var("item_data",$dive["watertemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name",lang("air_temp").":");
 $t->set_var("item_data",$dive["airtemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name",lang("current").":");
 $t->set_var("item_data",$dive["current"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name",lang("workload").":");
 $t->set_var("item_data",$dive["workload"]);
 $t->parse("cond","condblock",TRUE);
 #---------------------------[ Equipment ]---
 $t->set_var("item_name",lang("suit").":");
 $t->set_var("item_data",$dive["suittype"].", ".$dive["suitname"]);
 $t->parse("equi","equiblock");
 $t->set_var("item_name",lang("weight").":");
 $t->set_var("item_data",$dive["weight"]);
 $t->parse("equi","equiblock",TRUE);
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
   $t->set_var("tank_nr",$dive[tank][$i]->nr);
   $t->set_var("tank_name",$dive[tank][$i]->name);
   $t->set_var("tank_gas",$dive[tank][$i]->gas);
   $t->set_var("tank_type",$dive[tank][$i]->type);
   $t->set_var("tank_volume",$dive[tank][$i]->volume);
   $t->set_var("tank_in",$dive[tank][$i]->in);
   $t->set_var("tank_out",$dive[tank][$i]->out);
   $t->parse("tank","tankblock",TRUE);
 }
 #----------------------------[ Schedule ]---
 $sched = $pdl->db->get_schedule($nr);
 if ($sched) {
   $t->set_var("sched_name",lang("schedule"));
   $t->set_var("s_depth_name",lang("depth"));
   $t->set_var("s_time_name",lang("time"));
   $t->set_var("s_runtime_name",lang("runtime"));
   $t->set_var("s_gas_name",lang("gas"));
   $sc = count($sched);
   for ($i=0;$i<$sc;++$i) {
     $t->set_var("s_depth",$sched[$i]["depth"]);
     list($time_h, $time_m) = sscanf($sched[$i]["time"],"%d:%d");
     $time = "$time_h:".sprintf("%02d",$time_m,0);
     $t->set_var("s_time",$time);
     list($time_h, $time_m) = sscanf($sched[$i]["runtime"],"%d:%d");
     $time = "$time_h:".sprintf("%02d",$time_m,0);
     $t->set_var("s_runtime",$time);
     $t->set_var("s_gas",$sched[$i]["gas"] ." [".$sched[$i]["tank#"]."]");
     $t->parse("scheditem","scheditemblock",TRUE);
   }
   $t->parse("sched","scheduleblock");
 } else {
   $t->set_var("sched","");
 }
 #-----------------------------[ Profile ]---
 if ( strlen($prof_img=$pdl->file->getProfPic($nr)) ) {
   $t->set_var("prof_name",lang("profile"));
   $t->set_var("prof_img",$prof_img);
   $t->parse("profile","profileblock");
 }

 #-------------------------------[ Notes ]---
 $t->set_var("notes_text",nl2br($dive["notes"]));

 #-------------------------------[ Fotos ]---
 $fotos = $pdl->file->getDivePix($nr);
 $fc = count($fotos);
 if ($fc>0) {
   $picdir = $pdl->config->user_url;
   for ($i=0;$i<$fc;++$i) {
     $t->set_var("foto",$fotos[$i]->url);
     $t->set_var("fdesc",$fotos[$i]->desc);
     $t->parse("pic","fotoitemblock",TRUE);
   }
   $t->set_var("fotos_name",lang("fotos"));
   $t->parse("fotos","fotoblock");
 } else {
   $t->set_var("fotos","");
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>