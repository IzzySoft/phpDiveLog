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

 $title = "Izzys Dive LogBook";
 include("inc/includes.inc");
 include("inc/header.inc");
 if (!$start) $start = 0;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"sitelist.tpl"));
 $t->set_block("template","itemblock","item");

 #=========================================[ Icons for the navigation tabs ]===
 $t->set_var("dive_tab_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag2.gif" width="20" height="15" border="0" alt="DiveFlag">');
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_img",'<img src="'.$pdl->config->tpl_url.'images/btn_notes.gif" width="20" height="15" border="0" alt="Stats" align="middle">');
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("stats_ref","stats.php");
 $t->set_var("sites_tab_img",'<img src="'.$pdl->config->tpl_url.'images/globe.gif" width="15" height="15" border="0" alt="Globe" align="middle">');
 $t->set_var("sites_tab_name","Sites");


 #==============================================[ Import dive data from DB ]===
 $sites = $pdl->db->get_sites(); // $start,$pdl->config->display_limit);
 $max   = count($sites);
 $records = $pdl->db->sites;

 #=============================================[ set up the navigation bar ]===
 if ($start) {
   $prev = $start - $pdl->config->display_limit;
   if ($prev<0) $prev=0;
   $t->set_var("nav_left","<a href='$PHP_SELF?start=$prev'><img src='".$pdl->config->tpl_url."images/left.gif'></a>");
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if (TRUE) { // ($records - $start < $pdl->config->display_limit) {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 } else {
   $next = $start + $pdl->config->display_limit;
   $t->set_var("nav_right","<a href='$PHP_SELF?start=$next'><img src='".$pdl->config->tpl_url."images/right.gif'></a>");
 }

 #============================[ Walk through the list and set up the table ]===
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
   $t->set_var("site_ref","site.php?id=".$sites[$i]["id"]);
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
   $t->parse("item","itemblock",TRUE);
 }

 $t->pparse("out","template");

# echo "<b>SiteData:</b><pre>";print_r($pdl->db->sitedata);echo "</pre>";

 include("inc/footer.inc");
?>