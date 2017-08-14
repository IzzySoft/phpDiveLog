<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Logbook index                                                             #
 #############################################################################

 # $Id$

 $listtype = "logbook";
 $helppage = "divelist";
 include("inc/includes.inc");
 $title .= ": DiveIndex";
 $robots_index   = ROBOTS_INDEX_LISTS;
 $robots_revisit = ROBOTS_REVISIT_LISTS;
 include("inc/header.inc");
 $start = $pdl->params->start;
 $end = $start + $pdl->config->display_limit;

 if (isset($_REQUEST["filter"])) {
   $arr = explode("|",$_REQUEST["filter"]);
   $ac = count($arr);
   for ($i=0;$i<$ac;$i+=3) {
     if ($arr[$i]=="loc") $arr[$i] = "location";
     $filter[] = array("column"=>$arr[$i],"compare"=>$arr[$i+1],"value"=>$arr[$i+2]);
   }
 } else {
   $filter = "";
 }

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"logbook.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $sort = $pdl->params->sort; $order = $pdl->params->order;
 if (!in_array($sort,array("id","date","time","location","place","rating","depth","buddy","divetime"))) $sort = "";
 if (!in_array($order,array("desc","asc"))) $order = "";
 if (empty($sort) && !empty($pdl->config->logbook_default_sort)) {
   $sort  = $pdl->config->logbook_default_sort;
   $order = $pdl->config->logbook_default_order;
 }
 $dives = $pdl->db->get_dives($start,$pdl->config->display_limit,FALSE,$sort,$order,$filter);
 $max   = count($dives);
 $records = $pdl->db->dives;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("dives",TRUE);
 $pdl->tabs->parse();
 $pdl->common->prevNext($records,$start);
 $t->set_var("pages",$pdl->common->pages($records,$start));

 #===============================================[ set up the table header ]===
 #--------------------------------------------------------[ sorting images ]---
 $sortimg["up"]["id"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=id&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["id"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=id&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["date"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=date&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["date"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=date&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["time"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=time&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["time"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=time&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["location"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=location&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["location"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=location&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["place"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=place&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["place"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=place&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["rating"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=rating&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["rating"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=rating&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["depth"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=depth&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["depth"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=depth&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["divetime"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=divetime&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["divetime"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=divetime&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["buddy"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=buddy&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["buddy"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=buddy&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 switch ($sort) {
   case "id"    : if ($order=="desc") {
                    $sortimg["down"]["id"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["id"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "date"  : if ($order=="desc") {
                    $sortimg["down"]["date"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["date"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "time"  : if ($order=="desc") {
                    $sortimg["down"]["time"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["time"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "location" : if ($order=="desc") {
                    $sortimg["down"]["location"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["location"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "place" : if ($order=="desc") {
                    $sortimg["down"]["place"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["place"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "rating": if ($order=="desc") {
                    $sortimg["down"]["rating"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["rating"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "depth" : if ($order=="desc") {
                    $sortimg["down"]["depth"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["depth"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "divetime" : if ($order=="desc") {
                    $sortimg["down"]["divetime"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["divetime"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
   case "buddy" : if ($order=="desc") {
                    $sortimg["down"]["buddy"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["buddy"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
 }
 #--------------------------------------------[ table header template vars ]---
 $t->set_var("id_name","ID&nbsp;".$sortimg["up"]["id"].$sortimg["down"]["id"]);
 $t->set_var("date_name",lang("date")."&nbsp;".$sortimg["up"]["date"].$sortimg["down"]["date"]);
 $t->set_var("time_name",lang("time")."&nbsp;".$sortimg["up"]["time"].$sortimg["down"]["time"]);
 $t->set_var("loc_name",$sortimg["up"]["location"].$sortimg["down"]["location"]."&nbsp;".lang("place")."&nbsp;".$sortimg["up"]["place"].$sortimg["down"]["place"]);
 $t->set_var("rat_name",lang("rating")."&nbsp;".$sortimg["up"]["rating"].$sortimg["down"]["rating"]);
 $t->set_var("ddt_name",$sortimg["up"]["depth"].$sortimg["down"]["depth"]."&nbsp;".lang("depth+divetime")."&nbsp;".$sortimg["up"]["divetime"].$sortimg["down"]["divetime"]);
 $t->set_var("buddy_name",lang("buddy")."&nbsp;".$sortimg["up"]["buddy"].$sortimg["down"]["buddy"]);

 #============================[ Walk through the list and set up the table ]===
 $details = array ("dive#","date","time","depth","divetime","buddy","rating");
 for ($i=0;$i<$max;++$i) {
   foreach($details AS $detail) {
     $t->set_var("$detail",$pdl->common->null2nbsp($dives[$i][$detail]));
   }
   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
   $t->set_var("dive#",$pdl->link->linkurl("dive.php?nr=".$dives[$i]["dive#"],$dives[$i]["dive#"]));
   $t->set_var("place",$pdl->link->linkurl("site.php?id=".$dives[$i]["site_id"],$dives[$i]["place"]));
   $t->set_var("location",$pdl->link->linkurl("places.php?place=".$dives[$i]["location"],$dives[$i]["location"]));
   if ( $pdl->file->havePix($dives[$i]["dive#"],"dive") ) {
     $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
   } else {
     $t->set_var("pix","&nbsp;");
   }
   $t->parse("item","itemblock",$i);
 }
 $t->pparse("out","template");

 include("inc/footer.inc");
?>