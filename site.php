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

 #================================================[ Initialize environment ]===
 include("inc/includes.inc");
 $id = $_GET["id"];
 $title .= ": Site# $id";
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"site.tpl"));
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("fotoblock","fotoitemblock","pic");

 #==============================================[ Import dive data from DB ]===
 $site = $pdl->db->get_site($id); // $start,$pdl->config->display_limit);

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("sites");
 $pdl->tabs->parse();
 if ($prev=$site["prev_site#"]) {
   $t->set_var("nav_left",$pdl->link->linkurl("$PHP_SELF?id=$prev","<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$site["next_site#"]) {
   $t->set_var("nav_right",$pdl->link->linkurl("$PHP_SELF?id=$next","<img src='".$pdl->config->tpl_url."images/right.gif'>"));
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }

 #===============================================[ set up the table header ]===
 $t->set_var("lat_name",lang("latitude").":");
 $t->set_var("long_name",lang("longitude").":");
 $t->set_var("alt_name",lang("altitude").":");
 $t->set_var("md_name",lang("max_depth").":");
 $t->set_var("notes_name",lang("notes").":");

 #============================[ Walk through the list and set up the table ]===
 $details = array ("id","loc","place","depth","latitude","longitude","altitude");
 foreach($details AS $detail) {
   $t->set_var("$detail",$site[$detail]);
 }
 #-------------------------------[ Notes ]---
 $t->set_var("description",nl2br($site["description"]));
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
# $t->parse("item","itemblock",TRUE);

 #-------------------------------[ Fotos ]---
 $fotos = $pdl->file->getSitePix($id);
 $fc = count($fotos);
 if ($fc>0) {
   $picdir = $pdl->config->user_url;
   for ($i=0;$i<$fc;++$i) {
     $t->set_var("foto",$fotos[$i]->url);
     $t->set_var("fdesc",$fotos[$i]->desc);
     $t->parse("pic","fotoitemblock",TRUE);
   }
   $t->set_var("fotos_name",lang("fotos"));
   $t->parse("fotos","fotoblock");
 } else {
   $t->set_var("fotos","");
 }

 $t->pparse("out","template");

# echo "<b>SiteData:</b><pre>";print_r($site);echo "</pre>";

 include("inc/footer.inc");
?>