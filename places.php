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
 $places = $pdl->db->getAllPlaces();
 $max   = count($places);
 $records = $max;

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
 $t->set_var("hit_name",lang("hit_count"));
 if ( !empty($showPlace) ) {
   $t->set_var("site_name",lang("place"));
 }

 #============================[ Walk through the list and set up the table ]===
 if ( empty($showPlaces) ) {
   for ($i=0;$i<count($places);++$i) {
     $t->set_var("place",$places[$i]->name);
     $t->set_var("hits",$places[$i]->num);
     $t->parse("pitem","pitemblock",TRUE);
   }
   $t->parse("place","placeblock");
 }

/*
 $details = array ("id","loc","place","depth");
 for ($i=0;$i<$max;++$i) {
   foreach($details AS $detail) {
     $t->set_var("$detail",$sites[$i][$detail]);
   }
   if (!$sites[$i]["depth"]) {
     $t->set_var("depth","&nbsp;");
   } else {
     $t->set_var("depth",$sites[$i]["depth"]."m");
   }
   $t->set_var("site_ref",$pdl->link->linkurl("site.php?id=".$sites[$i]["id"],$sites[$i]["id"]));
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$sites[$i]["rating"]."star.gif");
   if ( $pdl->file->havePix($sites[$i]["id"],"site") ) {
     $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
   } else {
     $t->set_var("pix","");
   }
   $t->parse("item","itemblock",TRUE);
 }
*/
 $t->pparse("out","template");

 include("inc/footer.inc");
?>