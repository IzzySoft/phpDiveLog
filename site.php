<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://projects.izzysoft.de/                                              #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display a single site record                                              #
 #############################################################################

 #================================================[ Initialize environment ]===
 $helppage = "site-details";
 include("inc/includes.inc");
 $id = $_GET["id"];
 $title .= ": Site# $id";
 $robots_index   = ROBOTS_INDEX_PAGES;
 $robots_revisit = ROBOTS_REVISIT_PAGES;
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"site.tpl"));
 $t->set_block("template","notesblock","notb");
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("fotoblock","fotoitemblock","pic");
 $t->set_block("fotoblock","multifotoblock","multi");

 #==============================================[ Import dive data from DB ]===
 $site = $pdl->db->get_site($id);

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("sites");
 $pdl->tabs->parse();
 $arrowheight = "height='9'";

 #----------------------------------------------[ Context Navigation Setup ]---
 $ctxnav = "";
 $ctx_iconstyle = "style='margin-top:1px;'";
 if (K_PATH_MAIN!='') $ctxnav .= " ".$pdl->link->linkurl("site_pdf.php?nr=$id","<img src='".$pdl->config->icons["pdf"]."' width='16' height='16' title='".lang("export_dive_pdf")."' alt='PDF' $ctx_iconstyle>");
 if (!empty($site["loc"])) {
   $arr = urlencode("loc|eq|".$site["loc"]);
   $ctxnav .= " ".$pdl->link->linkurl("sitelist.php?filter=$arr","<img src='".$pdl->config->tpl_url."images/globe.gif' width='16' height='16' title='".lang("sites_at_this_location")."' alt='Location' $ctx_iconstyle>");
   $arr = urlencode("location|eq|".$site["loc"]);
   $ctxnav .= " ".$pdl->link->linkurl("index.php?filter=$arr","<img src='".$pdl->config->icons["location"]."' width='16' height='16' title='".lang("dives_at_this_location")."' alt='Location' $ctx_iconstyle>");
 }
 if (!empty($site["place"])) {
   $arr = urlencode("place|eq|".$site["place"]);
   $ctxnav .= " ".$pdl->link->linkurl("index.php?filter=$arr","<img src='".$pdl->config->icons["place"]."' width='16' height='16' title='".lang("dives_at_this_place")."' alt='Place' $ctx_iconstyle>");
 }
 $t->set_var("pages",trim($ctxnav));
 #----------------------------------------------------------[ Back + Forth ]---
 if ($prev=$site["prev_site#"]) {
   $t->set_var("nav_left",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?id=$prev","<img src='".$pdl->config->tpl_url."images/left.gif' $arrowheight>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$site["next_site#"]) {
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?id=$next","<img src='".$pdl->config->tpl_url."images/right.gif' $arrowheight>"));
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }

 #===============================================[ set up the table header ]===
 $t->set_var("lat_name",lang("latitude").":");
 $t->set_var("long_name",lang("longitude").":");
 $t->set_var("alt_name",lang("altitude").":");
 $t->set_var("md_name",lang("max_depth").":");
 $t->set_var("water_name",lang("water").":");
 $t->set_var("divetype_name",lang("divetype").":");
 $t->set_var("rating_name",lang("rating").":");
 $t->set_var("notes_name",lang("notes").":");

 #============================[ Walk through the list and set up the table ]===
 $details = array ("id","loc","place","depth","latitude","longitude","altitude","water","type");
 foreach($details AS $detail) {
   $t->set_var("$detail",$site[$detail]);
 }
 if ( $site["rating"] > 0 )
   $t->set_var("rating","<img src='".$pdl->config->tpl_url."images/".$site["rating"]."star.gif"."' alt='Rating:".$site["rating"]."'>");
 else $t->set_var("rating","");
 $maplink = $pdl->link->map($site["latitude"],$site["longitude"],$site["loc"].": ".$site["place"]);
 if ( empty($maplink) ) {
   $t->set_var("anim","");
 } else {
   $t->set_var("mapunlink","</A>");
   $t->set_var("maplink","<A HREF='$maplink' TARGET='pdlmap'>");
   $t->set_var("anim","_anim");
 }

 #-------------------------------[ Notes ]---
 $notes[1] = $pdl->common->nl2br($site["description"],1,1);
 $notes[2] = $pdl->file->getNotes($id,"site");
 $notb = $notes[1].$pdl->common->nl2br($notes[2]);
 if ( !empty($notb) ) {
   $t->set_var("description",$notb);
   $t->parse("notb","notesblock");
 }

# $t->set_var("rating",$pdl->config->tpl_url."images/".$dives[$i]["rating"]."star.gif");
# $t->parse("item","itemblock",TRUE);

 #-------------------------------[ Fotos ]---
 $fotos = $pdl->file->getSitePix($id);
 $fc = count($fotos);
 if ($fc>0) {
   $picdir = $pdl->config->user_url;
   for ($i=0;$i<$fc;++$i) {
     if (!empty($fotos[$i]->bigurl)) {
       $t->set_var("unref","</a>");
       $t->set_var("bigref","<a href=\"".$fotos[$i]->bigurl."\">");
     } else {
       $t->set_var("unref","");
       $t->set_var("bigref","");
     }
     $t->set_var("foto",$fotos[$i]->url);
     if (isset($fotos[$i]->desc)) $t->set_var("fdesc",$pdl->common->tagreplace($fotos[$i]->desc));
     else $t->set_var("fdesc","");
     $t->parse("pic","fotoitemblock",$i);
     if ( ($i+1)%PIX_PER_ROW==0 && $fc>PIX_PER_ROW && $i+1!=$fc ) {
       $t->parse("pic","multifotoblock",TRUE);
     }
   }
   $t->set_var("fotos_name",lang("fotos"));
   $t->parse("fotos","fotoblock");
 } else {
   $t->set_var("fotos","");
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>