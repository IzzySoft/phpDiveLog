<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Internal configuration - do not modify except you know what you do!       #
 #############################################################################

 /* $Id$ */

 include("inc/vul_check.inc");
 if (!empty($_GET["diver"])) $diver = $_GET["diver"];
 if (!empty($_GET["lang"])) $lang  = $_GET["lang"];
 if (!empty($_GET["tpl"])) $tpl   = $_GET["tpl"];
 #==========================================================[ include API ]===
 require_once(dirname(__FILE__)."/config.inc");
 if (file_exists(dirname(__FILE__)."/config_local.inc")) include(dirname(__FILE__)."/config_local.inc");
 require_once(dirname(__FILE__)."/class.csv.inc");
 require_once(dirname(__FILE__)."/config_internal.inc");
 switch($pdl->config->db_type) {
   case "csv"   :
     require_once(dirname(__FILE__)."/class.db_text.inc");
     $pdl->db = new db_text();
     break;
   default      :
     require_once(dirname(__FILE__)."/class.db_".$pdl->config->db_type.".inc");
     require_once(dirname(__FILE__)."/class.db.inc");
     $pdl->db = new sql;
 }
 require_once(dirname(__FILE__)."/class.template.inc");
 require_once(dirname(__FILE__)."/class.translation.inc");
 $pdl->trans = new trans();
 require_once(dirname(__FILE__)."/class.link.inc");
 require_once(dirname(__FILE__)."/class.common.inc");
 require_once(dirname(__FILE__)."/class.image.inc");

 #=======================================================[ setup shortkeys ]===
 function lang($key,$m1="",$m2="",$m3="",$m4="",$m5="") {
   GLOBAL $pdl;
   return $pdl->trans->transl($key,$m1,$m2,$m3,$m4,$m5);
 }

 #===========================================================[ setup prefs ]===
 if ( empty($_GET["lang"]) && !empty($pdl->trans->browserpref) ) {
   $lang = $pdl->trans->browserpref;
 }
 $pdl->trans->get_translations($lang);

 #==============================================[ alert on invalid account ]===
 if ( !in_array(basename($_SERVER["SCRIPT_FILENAME"]),array("buddylist.php","placelist_kml.php")) ) {
   if (empty($relPath)) { // no account specified
     include(dirname(__FILE__)."/header.inc");
     $pdl->common->alert(lang("no_diver",lang("diver_url_syntax")));
     include(dirname(__FILE__)."/footer.inc");
     trigger_error("The default_user '$default_user' specified in config.inc does not exist",E_USER_NOTICE);
     exit;
   } elseif (!is_dir($pdl->config->user_path)) { // unknown account
     include(dirname(__FILE__)."/header.inc");
     $pdl->common->alert(lang("no_such_diver",ucfirst($diver)));
     include(dirname(__FILE__)."/footer.inc");
     exit;
   }
 }

?>
