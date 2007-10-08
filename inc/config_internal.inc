<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Internal configuration - do not modify except you know what you do!       #
 #############################################################################

 /* $Id$ */

 $pdl->config->version = "0.2.7";
 $pdl->config->enable_index = $enable_index;
 if (!empty($_GET["diver"])) {
   $relPath = "diver/".$_GET["diver"]."/";
   $pdl->config->user_name = $_GET["diver"];
 } elseif (strtolower($default_page) == "user") {
   $relPath = "diver/$default_user/";
   $pdl->config->user_name = $default_user;
 } elseif (strtolower($default_page) == "index") {
   if ( basename($_SERVER["SCRIPT_FILENAME"])!= "buddylist.php" ) {
     header("Location: buddylist.php");
     exit;
   }
 }

 include("inc/class.file.inc");
 $pdl->file = new file();

 #==========================================================[ directories ]===
 $pdl->config->base_path = ""; # if set, has to include trailing slash - if
 $pdl->config->base_url  = ""; #   not, must be empty
 $pdl->config->user_path = $pdl->config->base_path.$relPath;
 $pdl->config->user_url  = $pdl->config->base_url.$relPath;
 $pdl->config->pwdfile   = $pwdfile;
 $pdl->config->transfer_path = $transfer_dir;

 #===========================================================[ users conf ]===
 if ( file_exists($pdl->config->user_path."system.conf") ) {
   $uconf = $pdl->file->read_conf($pdl->config->user_path."system.conf");
   $allowed = explode(" ",$allow_override);
   $ac = count($allowed);
   for ($i=0;$i<$ac;++$i) {
     if ( !empty($uconf[$allowed[$i]]) ) 
       ${$allowed[$i]} = $uconf[$allowed[$i]];
   }
   if ($uconf["personal"]) $pdl->config->display_personal = TRUE;
 }
 $pdl->config->user_pwd = $pdl->file->getPwd();

 #==========================================================[ preferences ]===
 if ( !empty($_GET["tpl"]) )  $template_set = $_GET["tpl"];
 $pdl->config->display_limit = $display_limit;
 if ( !empty($_GET["lang"]) ) {
   $lang = $_GET["lang"];
 }
 $pdl->config->tpl_path  = $pdl->config->base_path."templates/$template_set/";
 $pdl->config->tpl_url   = $pdl->config->base_url."templates/$template_set/";

 #=============================================================[ database ]===
 $pdl->config->db_type  = $database["type"];
 $pdl->config->datadir  = $pdl->config->user_path."data/";
 $pdl->config->divedata = $pdl->config->datadir."logbook.csv";
 $pdl->config->sites    = $pdl->config->datadir."divesites.csv";
 $pdl->config->database = $database;

 #======================================================[ default sorting ]===
 $sa = array("logbook_default_sort","logbook_default_order",
             "sitelist_default_sort","sitelist_default_order");
 foreach ($sa as $item) {
   $pdl->config->$item = ${$item};
   unset($item);
 }

 #=============================================================[ clean up ]===
 unset($database,$display_limit,$allow_override,$enable_index,$uconf,$allowed,
       $ac,$pwdfile,$transfer_dir);
?>
