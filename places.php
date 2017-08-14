<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Global Sites Index                                                        #
 #############################################################################

 # $Id$

 $helppage = "globalsites";
 include("inc/includes.inc");
 if (isset($_REQUEST["place"])) $showPlace = $_REQUEST["place"];
 else $showPlace = "";
 $title .= ": AllPlaces";
 if ( !empty($showPlace) ) $title .= ": $showPlace";
 $robots_index   = ROBOTS_INDEX_LISTS;
 $robots_revisit = ROBOTS_REVISIT_LISTS;
 include("inc/header.inc");
 if ( !$pdl->config->enable_index ) {
   $pdl->common->alert(lang("index_disabled"));
   include("inc/footer.inc");
   exit;
 }
 $start = $pdl->params->start;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"places.tpl"));
 $t->set_block("template","placeblock","place");
 $t->set_block("placeblock","pitemblock","pitem");
 $t->set_block("template","siteblock","site");
 $t->set_block("siteblock","sitemblock","sitem");
 $t->set_block("template","kmlblock","gkml");

 #==============================================[ Import dive data from DB ]===
 if ( empty($showPlace) ) {
   $places = $pdl->db->getAllPlaces($start,$end);
 } else {
   $places = $pdl->db->getAllPlaces($start,$end,$showPlace);
 }
 $max   = count($places);
 $records = $pdl->db->allplaces;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup2.inc");
 $pdl->tabs->activate("sites",TRUE);
 $pdl->tabs->parse();
 $arrowheight = "height='9px'";
 if ($start) {
   $prev = $start - $pdl->config->display_limit;
   if ($prev<0) $prev=0;
   $first = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=0","<img src='".$pdl->config->tpl_url."images/first.gif' $arrowheight>");
   $t->set_var("nav_left",$first.$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$prev","<img src='".$pdl->config->tpl_url."images/left.gif' alt='prev' $arrowheight>"));
 } else {
   $first = "<img src='".$pdl->config->tpl_url."images/first-grey.gif' $arrowheight>";
   $t->set_var("nav_left",$first."<img src='".$pdl->config->tpl_url."images/left-grey.gif' alt='prev' $arrowheight>");
 }
 if ($records - $start < $pdl->config->display_limit) {
   $last = "<img src='".$pdl->config->tpl_url."images/last-grey.gif' $arrowheight>";
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif' alt='next' $arrowheight>".$last);
 } else {
   $next = $start + $pdl->config->display_limit;
   $last = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=".floor($records/$pdl->config->display_limit)*$pdl->config->display_limit,"<img src='".$pdl->config->tpl_url."images/last.gif' $arrowheight>");
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?start=$next","<img src='".$pdl->config->tpl_url."images/right.gif' alt='next' $arrowheight>".$last));
 }

 $t->set_var("pages",$pdl->common->pages($records,$start));

 #===============================================[ set up the table header ]===
 $t->set_var("place_name",lang("location"));
 if ( empty($showPlace) ) {
   $t->set_var("hit_name",lang("hit_count"));
 } else {
   $t->set_var("site_name",lang("place"));
   $t->set_var("hit_name",lang("diver"));
 }
 if ($pdl->config->global_kml) {
   $pdl_url = $pdl->link->get_baseurl()."/placelist_kml.php";
   $t->set_var("dl_kml",lang("show_kml")." [<a href='placelist_kml.php'>Google Earth</a> | <a href='http://maps.google.com/?q=".urlencode($pdl_url)."' TARGET='_blank'>Google Maps</a>]");
   $t->parse("gkml","kmlblock");
 }

 #============================[ Walk through the list and set up the table ]===
 if ( empty($showPlace) ) {
   for ($i=0;$i<count($places);++$i) {
     $t->set_var("place",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?place=".urlencode($places[$i]->name),$places[$i]->name));
     $t->set_var("hits",$places[$i]->num);
     $t->parse("pitem","pitemblock",$i);
   }
   $t->parse("place","placeblock");
   $t->set_var("site","");
 } else {
   for ($i=0;$i<count($places);++$i) {
     $t->set_var("serial",$pdl->link->linkurl("site.php?diver=".$places[$i]->diver."&id=".$places[$i]->id,$start+$i+1));
     $t->set_var("place",$places[$i]->name);
     $t->set_var("site",$places[$i]->sitename);
     $t->set_var("diver",$pdl->link->linkurl("person.php?diver=".$places[$i]->diver,ucfirst($places[$i]->diver)));
     if ($pdl->file->havePix($places[$i]->id,"site",$places[$i]->diver)) {
       $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
     } else {
       $t->set_var("pix","&nbsp;");
     }
     $t->parse("sitem","sitemblock",$i);
   }
   $t->parse("site","siteblock");
   $t->set_var("place","");
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>