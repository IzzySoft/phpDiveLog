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
 if ($start) {
   $prev = $start - $pdl->config->display_limit;
   if ($prev<0) $prev=0;
   $t->set_var("nav_left","<a href='$PHP_SELF?start=$prev'><img src='".$pdl->config->tpl_url."images/left.gif'></a>");
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($records - $start < $pdl->config->display_limit) {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 } else {
   $next = $start + $pdl->config->display_limit;
   $t->set_var("nav_right","<a href='$PHP_SELF?start=$next'><img src='".$pdl->config->tpl_url."images/right.gif'></a>");
 }

 #===============================================[ set up the table header ]===
 $t->set_var("dive_name","Dive");
 $t->set_var("date_name","Date");
 $t->set_var("time_name","Time");
 $t->set_var("loc_name","Location");
 $t->set_var("rat_name","Rating");
 $t->set_var("ddt_name","Depth &amp; Dive Time");
 $t->set_var("buddy_name","Buddy");

 #============================[ Walk through the list and set up the table ]===
 $details = array ("dive#","date","time","location","depth","divetime","buddy","rating");
 for ($i=0;$i<$max;++$i) {
   foreach($details AS $detail) {
     $t->set_var("$detail",$dives[$i][$detail]);
   }
   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
   $t->set_var("dive#","<a href='dive.php?nr=".$dives[$i]["dive#"]."'>".$dives[$i]["dive#"]."</a>");
   $t->set_var("place","<a href='site.php?id=".$dives[$i]["site_id"]."'>".$dives[$i]["place"]."</a>");
   $t->parse("item","itemblock",TRUE);
 }
 $t->pparse("out","template");

 include("inc/footer.inc");
?>