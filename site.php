<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display a single site record                                              #
 #############################################################################

 # $Id$

 $title = "Izzys Dive LogBook";
 include("inc/includes.inc");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"site.tpl"));

 #================================================[ set up navigation tabs ]===
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("stats_ref","stats.php");
 $t->set_var("sites_tab_name","Sites");
 $t->set_var("sites_ref","sitelist.php");


 #==============================================[ Import dive data from DB ]===
 $site = $pdl->db->get_site($id); // $start,$pdl->config->display_limit);

 #=================================================[ general template data ]===
 $t->set_var("site_img","<img src='".$pdl->config->tpl_url."images/globe.gif' width='15' height='15' alt='Conditions'>");

 #=============================================[ set up the navigation bar ]===
 if ($prev=$site["prev_site#"]) {
   $t->set_var("nav_left","<a href='$PHP_SELF?id=$prev'><img src='".$pdl->config->tpl_url."images/left.gif'></a>");
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$site["next_site#"]) {
   $t->set_var("nav_right","<a href='$PHP_SELF?id=$next'><img src='".$pdl->config->tpl_url."images/right.gif'></a>");
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }

 #============================[ Walk through the list and set up the table ]===
 $details = array ("id","loc","place","depth","latitude","longitude","altitude");
 foreach($details AS $detail) {
   $t->set_var("$detail",$site[$detail]);
 }
 $t->set_var("description",nl2br($site["description"]));
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
# $t->parse("item","itemblock",TRUE);

 $t->pparse("out","template");

# echo "<b>SiteData:</b><pre>";print_r($site);echo "</pre>";

 include("inc/footer.inc");
?>