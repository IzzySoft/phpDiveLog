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

 $title = "Izzys Dive LogBook: Dive# $nr";
 include("inc/includes.inc");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"dive.tpl"));
 $t->set_block("template","sumblock","sum");
 $t->set_block("template","condblock","cond");
 $t->set_block("template","equiblock","equi");
 $t->set_block("template","tankblock","tank");
 $t->set_block("template","scheduleblock","sched");
 $t->set_block("scheduleblock","scheditemblock","scheditem");

 #=========================================[ Icons for the navigation tabs ]===
 $t->set_var("dive_tab_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag2.gif" width="20" height="15" border="0" alt="DiveFlag">');
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_img",'<img src="'.$pdl->config->tpl_url.'images/btn_notes.gif" width="20" height="15" border="0" alt="Stats" align="middle">');
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("stats_ref","stats.php");
 $t->set_var("sites_tab_img",'<img src="'.$pdl->config->tpl_url.'images/globe.gif" width="15" height="15" border="0" alt="Globe" align="middle">');
 $t->set_var("sites_tab_name","Sites");
 $t->set_var("sites_ref","sitelist.php");

 #=================================================[ general template data ]===
 $t->set_var("cond_img","<img src='".$pdl->config->tpl_url."images/btn_conditions.gif' width='37' height='15' alt='Conditions'>");
 $t->set_var("cond_name","Conditions");
 $t->set_var("notes_img","<img src='".$pdl->config->tpl_url."images/btn_notes.gif' width='37' height='15' alt='Notes'>");
 $t->set_var("notes_name","Notes");
 $t->set_var("equi_img","<img src='".$pdl->config->tpl_url."images/btn_equipment.gif' width='37' height='15' alt='Equipment'>");
 $t->set_var("equi_name","Equipment");

 #==============================================[ Import dive data from DB ]===
 $dive = $pdl->db->get_dive($nr);

 #=============================================[ set up the navigation bar ]===
 if ($prev=$dive["prev_dive#"]) {
   $t->set_var("nav_left","<a href='$PHP_SELF?nr=$prev'><img src='".$pdl->config->tpl_url."images/left.gif'></a>");
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$dive["next_dive#"]) {
   $t->set_var("nav_right","<a href='$PHP_SELF?nr=$next'><img src='".$pdl->config->tpl_url."images/right.gif'></a>");
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }
 #==================================================[ set up the dive data ]===
 $t->set_var("dive#",$dive["dive#"]);
 $t->set_var("time",$dive["time"]);
 $t->set_var("date",$dive["date"]);
 $t->set_var("location",$dive["location"]);
 $t->set_var("place","<a href='site.php?id=".$dive["site_id"]."'>".$dive["place"]."</a>");
 #--------------------------[ Summary ]---
 $t->set_var("item_name","Max. Depth:");
 $t->set_var("item_data",$dive["depth"]);
 $t->parse("sum","sumblock");
 $t->set_var("item_name","Dive Time:");
 $t->set_var("item_data",$dive["divetime"]);
 $t->parse("sum","sumblock",TRUE);
 $t->set_var("item_name","Buddy:");
 $t->set_var("item_data",$dive["buddy"]);
 $t->parse("sum","sumblock",TRUE);
 $t->set_var("item_name","Rating:");
 $t->set_var("item_data","<img src='".$pdl->config->tpl_url."images/".$dive["rating"]."star.gif"."' alt='Rating:".$dive["rating"]."'");
 $t->parse("sum","sumblock",TRUE);
 #--------------------------[ Conditions ]---
 $t->set_var("item_name","Visibility:");
 $t->set_var("item_data",$dive["visibility"]);
 $t->parse("cond","condblock");
 $t->set_var("item_name","Water Temp.:");
 $t->set_var("item_data",$dive["watertemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Air Temp.:");
 $t->set_var("item_data",$dive["airtemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Current:");
 $t->set_var("item_data",$dive["current"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Workload:");
 $t->set_var("item_data",$dive["workload"]);
 $t->parse("cond","condblock",TRUE);
 #---------------------------[ Equipment ]---
 $t->set_var("item_name","Suit:");
 $t->set_var("item_data",$dive["suittype"].", ".$dive["suitname"]);
 $t->parse("equi","equiblock");
 $t->set_var("item_name","Weight:");
 $t->set_var("item_data",$dive["weight"]);
 $t->parse("equi","equiblock",TRUE);
 $tc = count($dive["tank"]);
 $t->set_var("tank_trans","Tank");
 $t->set_var("tank_name_name","Name");
 $t->set_var("tank_gas_name","Gas");
 $t->set_var("tank_type_name","Type");
 $t->set_var("tank_volume_name","Volume");
 $t->set_var("pressure","Pressure");
 $t->set_var("tank_in_name","In");
 $t->set_var("tank_out_name","Out");
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
   $t->set_var("sched_img",'<img src="'.$pdl->config->tpl_url.'images/btn_schedule.gif" width="37" height="15" border="0" alt="Schedule">');
   $t->set_var("sched_name","Schedule");
   $t->set_var("s_depth_name","Depth");
   $t->set_var("s_time_name","Time");
   $t->set_var("s_runtime_name","Runtime");
   $t->set_var("s_gas_name","Gas");
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
 #-------------------------------[ Notes ]---
 $t->set_var("notes_text",nl2br($dive["notes"]));

 $t->set_var("loc_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag1.gif" width="23" height="15" border="0" alt="Globe" align="middle">');
 $t->pparse("out","template");

 include("inc/footer.inc");
?>