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
 include("inc/config.inc");
 include("inc/class.csv.inc");
 include("inc/config_internal.inc");
 switch($pdl->config->db_type) {
   case "csv"   :
     require_once("inc/class.db_text.inc");
     $pdl->db = new db_text();
     break;
   default      :
     require_once("inc/class.db_".$pdl->config->db_type.".inc");
     require_once("inc/class.db.inc");
     $pdl->db = new sql;
 }
 include("inc/class.template.inc");
 include("inc/class.translation.inc");
 $pdl->trans = new trans();
 include("inc/class.link.inc");
 include("inc/class.common.inc");
 include("inc/class.image.inc");

 #======================================================[ setup shortkeys ]===
 function lang($key,$m1="",$m2="",$m3="",$m4="",$m5="") {
   GLOBAL $pdl;
   return $pdl->trans->transl($key,$m1,$m2,$m3,$m4,$m5);
 }

 #======================================================[ setup prefs ]===
 if ( empty($_GET["lang"]) && !empty($pdl->trans->browserpref) ) {
   $lang = $pdl->trans->browserpref;
 }
 $pdl->trans->get_translations($lang);

 if ( basename($_SERVER["SCRIPT_FILENAME"])!= "buddylist.php" ) {
   if (empty($relPath)) { // no account specified
     include("inc/header.inc");
     $pdl->common->alert(lang("no_diver",lang("diver_url_syntax")));
     include("inc/footer.inc");
     trigger_error("The default_user '$default_user' specified in config.inc does not exist",E_USER_NOTICE);
     exit;
   } elseif (!is_dir($pdl->config->user_path)) { // unknown account
     include("inc/header.inc");
     $pdl->common->alert(lang("no_such_diver",ucfirst($diver)));
     include("inc/footer.inc");
     exit;
   }
 }

?>
