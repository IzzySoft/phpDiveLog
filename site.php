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
 $t->set_block("template","notesblock","notb");
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("fotoblock","fotoitemblock","pic");
 $t->set_block("fotoblock","multifotoblock","multi");

 function mk_coord($str) {
   if ( preg_match("/([0-9]+)[^0-9]+([0-9]+)[^0-9.]+([0-9.]+)[^0-9]+/",$str,$match) ) {
     $code[0] = $match[1]; $code[1] = $match[2]; $code[2] = $match[3];
   } elseif ( preg_match("/([0-9]+)[^0-9]+([0-9.]+)[^0-9.]+/",$str,$match) ) {
     $code[0] = $match[1]; $code[1] = floor($match[2]);
     $code[2] = ($match[2] - floor($match[2])) * 60;
   } elseif ( preg_match("/([0-9.]+)[^0-9.]+/",$str,$match) ) {
     $code[0] = floor($match[1]);
     $code[1] = floor( ($match[1] - $code[0]) * 60);
     $code[2] = floor( (($match[1] - $code[0]) * 60 - $code[1]) * 60 );
   }
   if ( substr($str,strlen($str)-1)=="N" || substr($str,strlen($str)-1)=="E" ) {
     $code[4] = 1;
   } else {
     $code[4] = -1;
   }
   return $code;
 }

 #==============================================[ Import dive data from DB ]===
 $site = $pdl->db->get_site($id);

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("sites");
 $pdl->tabs->parse();
 if ($prev=$site["prev_site#"]) {
   $t->set_var("nav_left",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?id=$prev","<img src='".$pdl->config->tpl_url."images/left.gif'>"));
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$site["next_site#"]) {
   $t->set_var("nav_right",$pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?id=$next","<img src='".$pdl->config->tpl_url."images/right.gif'>"));
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
 $lat  = mk_coord($site["latitude"]);
 $long = mk_coord($site["longitude"]);
 $maplink = $pdl->link->map($lat,$long);
 if ( !empty($maplink) ) {
   $t->set_var("mapunlink","</A>");
   $t->set_var("maplink","<A HREF='$maplink' TARGET='pdlmap'>");
 }

 #-------------------------------[ Notes ]---
 $notes[1] = $pdl->common->tagreplace(nl2br($site["description"]));
 $notes[2] = $pdl->file->getNotes($id,"site");
 if ( !empty($notes[2]) ) {
   if ( !empty($notes[1]) ) $notes[1] .= "<br>";
   $notes[2] = $pdl->common->tagreplace(nl2br($notes[2]));
 }
 $notb = $notes[1].$notes[2];
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
     $t->set_var("fdesc",$fotos[$i]->desc);
     $t->parse("pic","fotoitemblock",TRUE);
     if ( ($i+1)%3==0 && $fc>3 && $i+1!=$fc ) {
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