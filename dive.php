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
 $helppage = "dive";
 $listtype = "logbook";
 include("inc/includes.inc");
 $nr = $pdl->params->nr;
 $title .= ": ".lang("dive#")." $nr";
 if (USE_DYN_PROFILE_PNG) {
   include("inc/class.graph.inc");
   $graph = new graph();
 }
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
 $t->set_block("fotoblock","fotosubblock","sub");
 $t->set_block("fotosubblock","fotosubname","subname");
 $t->set_block("fotosubblock","fotoitemblock","pic");
 $t->set_block("fotosubblock","multifotoblock","multi");

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
 $arrowheight = "height='9'";
 #----------------------------------------------[ Context Navigation Setup ]---
 $ctxnav = "";
 $ctx_iconstyle = "style='margin-top:1px;'";
 if (K_PATH_MAIN!='') $ctxnav .= " ".$pdl->link->linkurl("dive_pdf.php?nr=$nr","<img src='".$pdl->config->icons["pdf"]."' width='16' height='16' title='".lang("export_dive_pdf")."' alt='PDF' $ctx_iconstyle>");
 if (!empty($dive["buddy"])) {
   $arr = urlencode("buddy|~|".$dive["buddy"]);
   $ctxnav .= " ".$pdl->link->linkurl("index.php?filter=$arr","<img src='".$pdl->config->icons["buddy"]."' width='16' height='16' title='".lang("dives_with_this_buddy")."' alt='Buddy' $ctx_iconstyle>");
 }
 if (!empty($dive["location"])) {
   $arr = urlencode("loc|eq|".$dive["location"]);
   $ctxnav .= " ".$pdl->link->linkurl("sitelist.php?filter=$arr","<img src='".$pdl->config->tpl_url."images/globe.gif' width='16' height='16' title='".lang("sites_at_this_location")."' alt='Location' $ctx_iconstyle>");
   $arr = urlencode("location|eq|".$dive["location"]);
   $ctxnav .= " ".$pdl->link->linkurl("index.php?filter=$arr","<img src='".$pdl->config->icons["location"]."' width='16' height='16' title='".lang("dives_at_this_location")."' alt='Location' $ctx_iconstyle>");
 }
 if (!empty($dive["place"])) {
   $arr = urlencode("place|eq|".$dive["place"]);
   $ctxnav .= " ".$pdl->link->linkurl("index.php?filter=$arr","<img src='".$pdl->config->icons["place"]."' width='16' height='16' title='".lang("dives_at_this_place")."' alt='Place' $ctx_iconstyle>");
 }
 $t->set_var("pages",trim($ctxnav));
 #----------------------------------------------------------[ Back + Forth ]---
 if (isset($dive["prev_dive#"])) {
   $t->set_var("nav_left",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?nr=".$dive["prev_dive#"],"<img src='".$pdl->config->tpl_url."images/left.gif' $arrowheight>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if (isset($dive["next_dive#"])) {
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?nr=".$dive["next_dive#"],"<img src='".$pdl->config->tpl_url."images/right.gif' $arrowheight>"));
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
 $t->set_var("item_data","<img src='".$pdl->config->tpl_url."images/".$dive["rating"]."star.gif"."' alt='Rating:".$dive["rating"]."'>");
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
 $t->set_var("item_data",ucfirst($dive["current"]));
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name",lang("workload").":");
 $t->set_var("item_data",ucfirst($dive["workload"]));
 $t->parse("cond","condblock",TRUE);
 #---------------------------[ Equipment ]---
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
 $profilepng_url = $pdl->config->user_url . "profiles/dive${nr}_profile.png";
 $profilemap = $pdl->config->user_path . "profiles/dive${nr}_profile.map";
 $schedulepng = $pdl->config->user_path . "profiles/dive${nr}_schedule.png";
 $schedulepng_url = $pdl->config->user_url . "profiles/dive${nr}_schedule.png";
 if (USE_DYN_PROFILE_PNG) {
   $graph->profileCheck($nr);
   // use dynamic profile if exists
   if (file_exists($profilepng)) {
     $t->set_var("prof_name",lang("profile"));
     $t->set_var("prof_img",$profilepng_url);
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
     $t->set_var("sched_img",$schedulepng_url);
     $t->parse("schedimg","schedimageblock");
   }
   if ($parse_schedule) $t->parse("sched","scheduleblock");
 } else {
   $t->set_var("sched","");
 }

 #-------------------------------[ Notes ]---
 $notes[1] = $pdl->common->nl2br($dive["notes"],1,1);
 $notes[2] = $pdl->file->getNotes($nr,"dive");
 $notb = $notes[1].$pdl->common->nl2br($notes[2]);
 if (!empty($notb)) {
   $t->set_var("notes_text",$notb);
   $t->parse("notb","notesblock");
 }

 #-------------------------------[ Fotos ]---
 function parse_fotos($fotos,$start=0) {
   GLOBAL $pdl, $t;
   $fc = count($fotos);
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
       if (isset($fotos[$i]->desc)) $t->set_var("fdesc",$fotos[$i]->desc);
       else $t->set_var("fdesc","");
       $t->parse("pic","fotoitemblock",$i+$start);
       if ( ($start+$i+1)%PIX_PER_ROW==0 && $i+1<$fc) {
         $t->parse("pic","multifotoblock",TRUE);
       }
     }
   }
   return $fc;
 }

 #--=[ Get photo data ]=--
 $fotos  = $pdl->file->getDivePix($nr);
 $sfotos = $pdl->file->getSitePix($dive["site_id"]);

 #--=[ Include SitePix on Top ]=--
 if ($sitepix_on_divepage && $sitepix_first) {
   $sfc = parse_fotos($sfotos);
   if ($sfc>0) {
     if ($sitepix_separate) {
       if ( ($sfc)%PIX_PER_ROW!=0 ) {
         $sfc += PIX_PER_ROW - ($sfc)%PIX_PER_ROW;
       }
       if (!empty($fotos)) {
         $t->set_var("fotos_sub_name",lang("site_pix"));
         $t->parse("subname","fotosubname");
         $t->parse("sub","fotosubblock");
         $block1 = TRUE;
       } else $block1 = FALSE;
       if (!empty($fotos)) $t->set_var("pic","");
     } else {
       if ( ($sfc)%PIX_PER_ROW==0 ) {
         $t->parse("pic","multifotoblock",TRUE);
       }
       $block1 = FALSE;
     }
   } else {
     $block1 = FALSE;
   }
   $fc  = parse_fotos($fotos,$sfc);
   if ($fc>0 && $sfc>0 && $sitepix_separate) {
     $t->set_var("fotos_sub_name",lang("dive_pix"));
     $t->parse("subname","fotosubname");
   }
   $t->parse("sub","fotosubblock",$block1);

 #--=[ Include SitePix below DivePix ]=--
 } elseif ($sitepix_on_divepage) {
   $fc  = parse_fotos($fotos);
   if ($fc>0) {
     if ($sitepix_separate) {
       if ( ($fc)%PIX_PER_ROW!=0 ) {
         $fc += PIX_PER_ROW - ($fc)%PIX_PER_ROW;
       }
       if (!empty($sfotos)) {
         $t->set_var("fotos_sub_name",lang("dive_pix"));
         $t->parse("subname","fotosubname");
         $t->parse("sub","fotosubblock");
         $block1 = TRUE;
       } else $block1 = FALSE;
       if (!empty($sfotos)) $t->set_var("pic","");
     } else { 
       if ( ($fc)%PIX_PER_ROW==0 ) {
         $t->parse("pic","multifotoblock",TRUE);
       }
       $block1 = FALSE;
     }
   }
   $sfc = parse_fotos($sfotos,$fc);
   if ($fc>0 && $sfc>0 && $sitepix_separate) {
     $t->set_var("fotos_sub_name",lang("site_pix"));
     $t->parse("subname","fotosubname");
   }
   $t->parse("sub","fotosubblock",$block1);

 #--=[ DivePix Only ]=--
 } else {
   if (!empty($fotos)) {
     $fc  = parse_fotos($fotos);
     $t->parse("sub","fotosubblock",$block1);
     $sfc = 0;
   } elseif ($sitepix_if_no_divepix && !empty($sfotos)) {
     $sfc  = parse_fotos($sfotos);
     $t->parse("sub","fotosubblock",$block1);
     $fc = 0;
   } else {
     $fc = $sfc = 0;
   }
 }

 #--=[ Finish the FotoBlock ]=--
 if ($fc+$sfc>0) {
   $t->set_var("fotos_name",lang("fotos"));
   $t->parse("fotos","fotoblock");
 } else {
   $t->set_var("fotos","");
 }

 #--=[ Output Content ]=--
 $t->pparse("out","template");

 include("inc/footer.inc");
?>