<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Buddy List                                                                #
 #############################################################################

 # $Id$

 include("inc/includes.inc");
 $title .= ": BuddyList";
 include("inc/header.inc");
 if ( !$pdl->config->enable_index ) {
   $pdl->common->alert(lang("index_disabled"));
   include("inc/footer.inc");
   exit;
 }
 if (!$start) $start = 0;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"buddylist.tpl"));
 $t->set_block("template","itemblock","item");

 #===================================================[ Get list of buddies ]===
 $buddies = $pdl->file->get_buddies($start,$pdl->config->display_limit);
 $max     = count($buddies);
 $records = $pdl->file->buddies;
 if ($records==0) {
   $pdl->common->alert(lang("no_public_divelogs"));
   include("inc/footer.inc");
   exit;
 }

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
# $pdl->tabs->activate("dives",TRUE);
# $pdl->tabs->parse();
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
# $t->set_var("dive_name",lang("dive"));
 $t->set_var("nick_name",lang("buddy"));
 $t->set_var("name_name",lang("person_name"));
 $t->set_var("loc_name",lang("location"));
 $t->set_var("status_name",lang("status"));

 #============================[ Walk through the list and set up the table ]===
 for ($i=0;$i<$max;++$i) {
   $diver = $pdl->file->read_conf($pdl->config->base_path."diver/".$buddies[$i]."/diver.conf");
   $t->set_var("nick",$pdl->link->linkurl("index.php?diver=".$buddies[$i],ucfirst($buddies[$i])));
   if ($diver["person"]["buddylist"]) {
     $t->set_var("name",trim($diver["person"]["firstname"]." ".$diver["person"]["name"]));
     $location = $diver["person"]["city"];
     if ( !empty($diver["person"]["state"]) )
       if ( empty($location) ) { $location = $diver["person"]["state"]; }
       else { $location .= ", ".$diver["person"]["state"]; }
     if ( !empty($diver["person"]["country"]) )
       if ( empty($location) ) { $location = $diver["person"]["country"]; }
       else { $location .= ", ".$diver["person"]["country"]; }
     $t->set_var("location",$location);
     $t->set_var("status",$diver["person"]["status"]);
     if ( !empty($diver["person"]["foto"]) ) {
       $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
     } else {
       $t->set_var("pix","");
     }
   } else {
     $t->set_var("name","");
     $t->set_var("location","");
     $t->set_var("status","");
     $t->set_var("pix","");
   }
   $t->parse("item","itemblock",TRUE);
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>