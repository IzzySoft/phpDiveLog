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
 $title .= ": SiteIndex";
 include("inc/header.inc");
 $start = $_GET["start"];
 if (!$start) $start = 0;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"sitelist.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $sites = $pdl->db->get_sites($start,$pdl->config->display_limit);
 $max   = count($sites);
 $records = $pdl->db->sites;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("sites",TRUE);
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
 $t->set_var("loc_name",lang("location"));
 $t->set_var("place_name",lang("place"));
 $t->set_var("md_name",lang("max_depth"));

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
   $t->set_var("site_ref",$pdl->link->linkurl("site.php?id=".$sites[$i]["id"],$sites[$i]["id"]));
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$sites[$i]["rating"]."star.gif");
   if ( $pdl->file->havePix($sites[$i]["id"],"site") ) {
     $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
   } else {
     $t->set_var("pix","");
   }
   $t->parse("item","itemblock",TRUE);
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>