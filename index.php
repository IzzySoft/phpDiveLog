<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
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
 if (!$start) $start = 0;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"logbook.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $dives = $pdl->db->get_dives($start,$pdl->config->display_limit);
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
# $t->set_var("dive_name",lang("dive"));
 $t->set_var("date_name",lang("date"));
 $t->set_var("time_name",lang("time"));
 $t->set_var("loc_name",lang("place"));
 $t->set_var("rat_name",lang("rating"));
 $t->set_var("ddt_name",lang("depth+divetime"));
 $t->set_var("buddy_name",lang("buddy"));

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