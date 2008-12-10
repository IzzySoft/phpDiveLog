<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Internal configuration - do not modify except you know what you do!       #
 #############################################################################

 /* $Id$ */

 $pdl->config->version = "0.2.9";
 #=============================================================[ Debugging ]===
 error_reporting(0); // initially turn all off
 function set_phpdebug($level) {
   $pdbug = error_reporting();
   error_reporting($pdbug | $level);
 }
 if ( strpos(strtoupper($debug_level),"E")!==false ) set_phpdebug(E_ERROR);
 if ( strpos(strtoupper($debug_level),"W")!==false ) set_phpdebug(E_WARNING);
 if ( strpos(strtoupper($debug_level),"N")!==false ) set_phpdebug(E_NOTICE);
 if ($debug_show==0) ini_set('display_errors',0);
 else ini_set('display_errors',1);
 if ($debug_log==0) ini_set('log_errors',0);
 else ini_set('log_errors',1);

 #===========================================================[ Diver Setup ]===
 $pdl->config->enable_index = $enable_index;
 if (!empty($_REQUEST["diver"])) $pdl->params->diver = $_REQUEST["diver"];
 elseif (strtolower($default_page) == "user") $pdl->params->diver = $default_user;
 elseif (strtolower($default_page) == "index") {
   if ( basename($_SERVER["SCRIPT_FILENAME"])!= "buddylist.php" ) {
     header("Location: buddylist.php");
     exit;
   }
 }
 $relPath = "diver/".$pdl->params->diver."/";
 $pdl->config->user_name = $pdl->params->diver;

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
 $pdl->config->values_in_statgraphs = strtolower($values_in_statgraphs);

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
       $ac,$pwdfile,$transfer_dir,$values_in_statgraphs);
?>
