<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display divers personal data                                              #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 include("inc/includes.inc");
 if ( !empty($_GET["diver"]) ) $title .= ": ".ucfirst($_GET["diver"]);
 include("inc/header.inc");
 if (!$pdl->config->display_personal) {
   $pdl->common->alert(lang("personal_no_public"));
   include("inc/footer.inc");
   exit;
 }

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"person.tpl"));
 $t->set_block("template","personblock","person");
 $t->set_block("personblock","pdetailblock","pdetail");
 $t->set_block("template","certblock","cert");
 $t->set_block("certblock","cdetailblock","cdetail");
 $t->set_block("template","fotoblock","fotos");
 $t->set_block("template","notesblock","notb");
 $t->set_block("fotoblock","fotoitemblock","pic");
 $t->set_block("fotoblock","multifotoblock","multi");

 #=================================================[ general template data ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("person",TRUE);
 $pdl->tabs->parse();
 $t->set_var("personal_name",lang("person"));
 $t->set_var("certify_name",lang("certifications"));

 #=====================================================[ Import diver data ]===
 $diver = $pdl->file->read_conf($pdl->config->user_path."diver.conf");
 if ( file_exists($pdl->config->user_path."diver.txt.$lang") ) {
   $diver["notes"] = $pdl->file->read_file($pdl->config->user_path."diver.txt.$lang");
 } elseif ( file_exists($pdl->config->user_path."diver.txt") ) {
   $diver["notes"] = $pdl->file->read_file($pdl->config->user_path."diver.txt");
 }

 #=================================================[ set up the diver data ]===
 #-------------------------[ personal data ]---
 if ( is_array($diver["person"]) ) { // personal data provided
   if ( !empty($diver["person"]["name"]) || !empty($diver["person"]["firstname"]) ) {
     $t->set_var("name",lang("person_name"));
     $t->set_var("description",trim($diver["person"]["firstname"]." ".$diver["person"]["name"]));
     $t->parse("pdetail","pdetailblock");
   }
   $location = $diver["person"]["city"];
   if ( !empty($diver["person"]["state"]) )
     if ( empty($location) ) { $location = $diver["person"]["state"]; }
     else { $location .= ", ".$diver["person"]["state"]; }
   if ( !empty($diver["person"]["country"]) )
     if ( empty($location) ) { $location = $diver["person"]["country"]; }
     else { $location .= ", ".$diver["person"]["country"]; }
   if ( !empty($location) ) {
     $t->set_var("name",lang("location"));
     $t->set_var("description",$location);
     $t->parse("pdetail","pdetailblock",TRUE);
   }
   if ( !empty($diver["person"]["status"]) ) {
     $t->set_var("name",lang("status"));
     $t->set_var("description",$diver["person"]["status"]);
     $t->parse("pdetail","pdetailblock",TRUE);
   }
   if ( !empty($diver["person"]["foto"]) ) {
     $t->set_var("portrait",$pdl->config->user_url."fotos/person/".$diver["person"]["foto"]);
   } else {
     $t->set_var("portrait",$pdl->config->tpl_url."images/dummy_person.jpg");
   }
   $t->parse("person","personblock");
 }
 #------------------------[ certifications ]---
 if ( is_array($diver["certification"]) ) { // certification data provided
   $cc = count($diver["certification"]["course"]);
   for ($i=0;$i<$cc;++$i) {
     $t->set_var("date",$diver["certification"]["date"][$i]);
     $t->set_var("course",$diver["certification"]["course"][$i]);
     $t->set_var("place",$diver["certification"]["place"][$i]);
     $t->parse("cdetail","cdetailblock",TRUE);
   }
   $t->set_var("date_name",lang("date"));
   $t->set_var("course_name",lang("course"));
   $t->set_var("place_name",lang("location"));
   $t->parse("cert","certblock");
 }

 #-------------------------------[ Notes ]---
 $notes = $pdl->common->nl2br($diver["notes"]);
 if (!empty($notes)) {
   $t->set_var("notes_name",lang("notes"));
   $t->set_var("notes_text",$notes);
   $t->parse("notb","notesblock");
 }

 #-------------------------------[ Fotos ]---
 $fotos = $pdl->file->getPersonPix();
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
     $t->set_var("fdesc",$pdl->common->tagreplace($fotos[$i]->desc));
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