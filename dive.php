<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display a single dive record                                              #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 $listtype = "logbook";
 include("inc/includes.inc");
 $nr = $pdl->params->nr;
 $title .= ": ".lang("dive#")." $nr";
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"dive.tpl"));
 $t->set_block("template","sumblock","sum");
 $t->set_block("template","condblock","cond");
 $t->set_block("template","equiblock","equi");
 $t->set_block("template","tankblock","tank");
 $t->set_block("template","notesblock","notb");
 $t->set_block("template","scheduleblock","sched");
 $t->set_block("scheduleblock","scheditemblock","scheditem");
 $t->set_block("scheduleblock","schedimageblock","schedimg");
 $t->set_block("template","profileblock","profile");
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("fotoblock","fotoitemblock","pic");
 $t->set_block("fotoblock","multifotoblock","multi");

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
 if (isset($dive["prev_dive#"])) {
   $t->set_var("nav_left",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?nr=".$dive["prev_dive#"],"<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if (isset($dive["next_dive#"])) {
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?nr=".$dive["next_dive#"],"<img src='".$pdl->config->tpl_url."images/right.gif'>"));
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
 if (!empty($userdef1) && !empty($dive["userdef1"])) {
   $t->set_var("item_name",lang($userdef1).":");
   $t->set_var("item_data",$dive["userdef1"]);
   $t->parse("equi","equiblock",TRUE);
 }
 if (!empty($userdef2) && !empty($dive["userdef2"])) {
   $t->set_var("item_name",lang($userdef2).":");
   $t->set_var("item_data",$dive["userdef2"]);
   $t->parse("equi","equiblock",TRUE);
 }
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
   $t->set_var("tank_nr",$dive["tank"][$i]->nr);
   $t->set_var("tank_name",$dive["tank"][$i]->name);
   $t->set_var("tank_gas",$dive["tank"][$i]->gas);
   $t->set_var("tank_type",$dive["tank"][$i]->type);
   $t->set_var("tank_volume",$dive["tank"][$i]->volume);
   $t->set_var("tank_in",$dive["tank"][$i]->in);
   $t->set_var("tank_out",$dive["tank"][$i]->out);
   $t->parse("tank","tankblock",$i);
 }
 #-----------------------------[ Profile ]---
 while (strlen($nr)<5) $nr = "0$nr";
 $csvfile = $pdl->config->datadir."dive${nr}_profile.csv";
 $schedulecsv = $pdl->config->datadir."dive${nr}_schedule.csv";
 $profilepng = $pdl->config->user_path . "profiles/dive${nr}_profile.png";
 $profilemap = $pdl->config->user_path . "profiles/dive${nr}_profile.map";
 $schedulepng = $pdl->config->user_path . "profiles/dive${nr}_schedule.png";
 if ($use_dyn_profile_png) {
   // generate dynamic profile/schedule graphs
   if (!file_exists($profilepng) || filemtime($profilepng) < filemtime($csvfile)) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->profile($nr);
   }
   if ((($schedule_graph=="integrated" && !file_exists($profilepng)) || $schedule_graph=="separate")
      && (!file_exists($schedulepng) || filemtime($schedulepng) < filemtime($schedulecsv))) {
     include_once("inc/class.graph.inc");
     $graph = new graph();
     $graph->schedule($nr);
   }
   // use dynamic profile if exists
   if (file_exists($profilepng)) {
     $t->set_var("prof_name",lang("profile"));
     $t->set_var("prof_img",$profilepng);
     if (file_exists($profilemap)) {
       $t->set_var("prof_map","<map name='prof${nr}' id='prof${nr}'>".file_get_contents($profilemap)."</map>");
       $t->set_var("use_map","USEMAP='#prof${nr}'");
     } else {
       $t->set_var("prof_map","");
       $t->set_var("use_map","");
     }
     $t->parse("profile","profileblock");
   }
 } else {
   if ( strlen($prof_img=$pdl->file->getProfPic($nr)) ) {
     $t->set_var("prof_name",lang("profile"));
     $t->set_var("prof_img",$prof_img);
     $t->set_var("prof_map","");
     $t->set_var("use_map","");
     $t->parse("profile","profileblock");
   }
 }

 #----------------------------[ Schedule ]---
 $sched = $pdl->db->get_schedule($nr);
 if ($sched) {
   $parse_schedule = TRUE;
   if ($hide_schedule_table) {
     if ($use_dyn_profile_png && file_exists($schedulepng)) {
       $t->set_var("sched_name",lang("schedule"));
     } else {
       $t->set_var("sched","");
       $parse_schedule = FALSE;
     }
   } else {
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
   }
   if ($use_dyn_profile_png && file_exists($schedulepng)) {
     $t->set_var("sched_img",$schedulepng);
     $t->parse("schedimg","schedimageblock");
   }
   if ($parse_schedule) $t->parse("sched","scheduleblock");
 } else {
   $t->set_var("sched","");
 }

 #-------------------------------[ Notes ]---
 $notes[1] = $pdl->common->nl2br($dive["notes"]);
 $notes[2] = $pdl->file->getNotes($nr,"dive");
 $notb = $notes[1].$pdl->common->nl2br($notes[2]);
 if (!empty($notb)) {
   $t->set_var("notes_text",$notb);
   $t->parse("notb","notesblock");
 }

 #-------------------------------[ Fotos ]---
 $fotos = $pdl->file->getDivePix($nr);
 $fc = count($fotos);
 if ($sitepix_on_divepage > 0) { // optionally include sitepix
   $sfotos = $pdl->file->getSitePix($dive["site_id"]);
   $sfc = count($sfotos);
   if ($sfc>0) {
     $picdir = $pdl->config->user_url;
     for ($i=0;$i<$sfc;++$i) {
       if (!empty($sfotos[$i]->bigurl)) {
         $t->set_var("unref","</a>");
	 $t->set_var("bigref","<a href=\"".$sfotos[$i]->bigurl."\">");
       } else {
         $t->set_var("unref","");
	 $t->set_var("bigref","");
       }
       $t->set_var("foto",$sfotos[$i]->url);
       $t->set_var("fdesc",$sfotos[$i]->desc);
       $t->parse("pic","fotoitemblock",TRUE);
       if ( ($i+1)%3==0 && $sfc+$fc>3 && ($fc>0 || $i+1!=$sfc) ) {
         $t->parse("pic","multifotoblock",TRUE);
       }
     }
   }
 } else $sfc = 0;
 if ($fc>0) {
   $picdir = $pdl->config->user_url;
   for ($i=0;$i<$fc;++$i) {
     if (!empty($fotos[$i]->bigurl)) {
       $t->set_var("unref","</a>");
       $t->set_var("bigref","<a href=\"".$fotos[$i]->bigurl."\">");
     } else {
       $t->set_var("unref","");
       $t->set_var("bigref","");
     }
     $t->set_var("foto",$fotos[$i]->url);
     $t->set_var("fdesc",$pdl->common->tagreplace($fotos[$i]->desc));
     $t->parse("pic","fotoitemblock",TRUE);
     if ( ($sfc+$i+1)%3==0 && $sfc+$fc>3 && $i+1!=$fc ) {
       $t->parse("pic","multifotoblock",TRUE);
     }
   }
 }
 if ($fc+$sfc>0) {
   $t->set_var("fotos_name",lang("fotos"));
   $t->parse("fotos","fotoblock");
 } else {
   $t->set_var("fotos","");
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>