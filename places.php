<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Sites Index                                                               #
 #############################################################################

 # $Id$

 include("inc/includes.inc");
 $title .= ": AllPlaces";
 include("inc/header.inc");
 $start = $_GET["start"];
 if (!$start) $start = 0;
 $end = $start + $pdl->config->display_limit;
 $showPlace = $_GET["place"];

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"places.tpl"));
 $t->set_block("template","placeblock","place");
 $t->set_block("placeblock","pitemblock","pitem");
 $t->set_block("template","siteblock","site");
 $t->set_block("siteblock","sitemblock","sitem");

 #==============================================[ Import dive data from DB ]===
 if ( !empty($showPlace) ) {
   $places = $pdl->db->getAllPlaces($start,$end,$showPlace);
 } else {
   $places = $pdl->db->getAllPlaces($start,$end);
 }
 $max   = count($places);
 $records = $pdl->db->allplaces;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup2.inc");
 $pdl->tabs->activate("sites",TRUE);
 $pdl->tabs->parse();
 if ($start) {
   $prev = $start - $pdl->config->display_limit;
   if ($prev<0) $prev=0;
   $t->set_var("nav_left",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$prev","<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($records - $start < $pdl->config->display_limit) {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 } else {
   $next = $start + $pdl->config->display_limit;
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$next","<img src='".$pdl->config->tpl_url."images/right.gif'>"));
 }

 #===============================================[ set up the table header ]===
 $t->set_var("place_name",lang("location"));
 if ( empty($showPlace) ) {
   $t->set_var("hit_name",lang("hit_count"));
 } else {
   $t->set_var("site_name",lang("place"));
   $t->set_var("hit_name",lang("diver"));
 }

 #============================[ Walk through the list and set up the table ]===
 if ( empty($showPlace) ) {
   for ($i=0;$i<count($places);++$i) {
     $t->set_var("place",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?place=".urlencode($places[$i]->name),$places[$i]->name));
     $t->set_var("hits",$places[$i]->num);
     $t->parse("pitem","pitemblock",TRUE);
   }
   $t->parse("place","placeblock");
   $t->set_var("site","");
 } else {
   for ($i=0;$i<count($places);++$i) {
     $t->set_var("place",$places[$i]->name);
     $t->set_var("site",$places[$i]->sitename);
     $t->set_var("hits",$pdl->link->linkurl("site.php?diver=".$places[$i]->diver."&id=".$places[$i]->id,ucfirst($places[$i]->diver)));
     $t->parse("sitem","sitemblock",TRUE);
   }
   $t->parse("site","siteblock");
   $t->set_var("place","");
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>