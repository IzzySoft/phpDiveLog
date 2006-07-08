<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2006 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Logbook index                                                             #
 #############################################################################

 # $Id$

 include("inc/includes.inc");
 $title .= ": DiveIndex";
 include("inc/header.inc");
 $start = $_GET["start"];
 if (!$start||!is_numeric($start)) $start = 0;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"logbook.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $sort = $_REQUEST["sort"]; $order = $_REQUEST["order"];
 if (!in_array($sort,array("date","time","location","place","rating","depth","buddy"))) $sort = "";
 if (!in_array($order,array("desc","asc"))) $order = "";
 $dives = $pdl->db->get_dives($start,$pdl->config->display_limit,FALSE,$sort,$order);
 $max   = count($dives);
 $records = $pdl->db->dives;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("dives",TRUE);
 $pdl->tabs->parse();
 if ($start) {
   $prev = $start - $pdl->config->display_limit;
   if ($prev<0) $prev=0;
   $first = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=0","<img src='".$pdl->config->tpl_url."images/first.gif'>");
   $t->set_var("nav_left",$first.$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$prev","<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $first = "<img src='".$pdl->config->tpl_url."images/first-grey.gif'>";
   $t->set_var("nav_left","$first<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($records - $start < $pdl->config->display_limit) {
   $last = "<img src='".$pdl->config->tpl_url."images/last-grey.gif'>";
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>$last");
 } else {
   $last = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=".floor($records/$pdl->config->display_limit)*$pdl->config->display_limit,"<img src='".$pdl->config->tpl_url."images/last.gif'>");
   $next = $start + $pdl->config->display_limit;
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$next","<img src='".$pdl->config->tpl_url."images/right.gif'>$last"));
 }

 #===============================================[ set up the table header ]===
 #--------------------------------------------------------[ sorting images ]---
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
 $sortimg["up"]["buddy"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=buddy&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["buddy"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=buddy&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 switch ($sort) {
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
   case "buddy" : if ($order=="desc") {
                    $sortimg["down"]["buddy"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                  } else {
                    $sortimg["up"]["buddy"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                  } break;
 }
 #--------------------------------------------[ table header template vars ]---
 $t->set_var("date_name",lang("date")."&nbsp;".$sortimg["up"]["date"].$sortimg["down"]["date"]);
 $t->set_var("time_name",lang("time")."&nbsp;".$sortimg["up"]["time"].$sortimg["down"]["time"]);
 $t->set_var("loc_name",$sortimg["up"]["location"].$sortimg["down"]["location"]."&nbsp;".lang("place")."&nbsp;".$sortimg["up"]["place"].$sortimg["down"]["place"]);
 $t->set_var("rat_name",lang("rating")."&nbsp;".$sortimg["up"]["rating"].$sortimg["down"]["rating"]);
 $t->set_var("ddt_name",lang("depth+divetime")."&nbsp;".$sortimg["up"]["depth"].$sortimg["down"]["depth"]);
 $t->set_var("buddy_name",lang("buddy")."&nbsp;".$sortimg["up"]["buddy"].$sortimg["down"]["buddy"]);

 #============================[ Walk through the list and set up the table ]===
 $details = array ("dive#","date","time","depth","divetime","buddy","rating");
 for ($i=0;$i<$max;++$i) {
   foreach($details AS $detail) {
     $t->set_var("$detail",$dives[$i][$detail]);
   }
   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
   $t->set_var("dive#",$pdl->link->linkurl("dive.php?nr=".$dives[$i]["dive#"],$dives[$i]["dive#"]));
   $t->set_var("place",$pdl->link->linkurl("site.php?id=".$dives[$i]["site_id"],$dives[$i]["place"]));
   $t->set_var("location",$pdl->link->linkurl("places.php?place=".$dives[$i]["location"],$dives[$i]["location"]));
   if ( $pdl->file->havePix($dives[$i]["dive#"],"dive") ) {
     $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
   } else {
     $t->set_var("pix","");
   }
   $t->parse("item","itemblock",TRUE);
 }
 $t->pparse("out","template");

 include("inc/footer.inc");
?>