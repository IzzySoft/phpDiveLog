<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Internal configuration - do not modify except you know what you do!       #
 #############################################################################

 /* $Id$ */

 #==========================================================[ include API ]===
 include("inc/config.inc");
 include("inc/config_internal.inc");
 switch($pdl->config->db_type) {
   case "cvs"   :
   default      :
     include("inc/class.csv.inc");
     include("inc/class.db_text.inc");
 }
 $pdl->db = new db_text();
 include("inc/class.template.inc");
 include("inc/class.translation.inc");
 $pdl->trans = new trans();
 include("inc/class.link.inc");
 include("inc/class.common.inc");

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

 if ( basename($PHP_SELF)!= "buddylist.php" ) {
   if (empty($relPath)) { // no account specified
     include("inc/header.inc");
     $pdl->common->alert(lang("no_diver",lang("diver_url_syntax")));
     include("inc/footer.inc");
     exit;
   } elseif (!is_dir($pdl->config->user_path)) { // unknown account
     include("inc/header.inc");
     $pdl->common->alert(lang("no_such_diver",ucfirst($diver)));
     include("inc/footer.inc");
     exit;
   }
 }

?>
