<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Import logbook from the transfer directory                                #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 include("inc/includes.inc");
 $title .= ": Logbook Import";
 include("inc/header.inc");

 if ($_POST["submit"]) { // form was submitted
 #===================================================[ Import logbook data ]===
 #------------------------------------------------------[ Check the passwd ]---
   if (file_exists($pdl->config->pwdfile)) {
     if ( md5($_POST["passwd"]) == $pdl->config->user_pwd ) $pwd_ok = TRUE;
   } else {
     $pdl->common->alert(lang("no_pwd_file"));
     include("inc/footer.inc");
     exit;
   }
   if ( $pwd_ok ) {
 #-----------------------------------------------------[ Transfer the data ]---
     include("inc/class.import.inc");
     $pdl->import = new import;
     $import_ok = $pdl->import->getData("pdl");
     if ( $import_ok ) { // call to import routine here *!*
       $ptitle = lang("import_success_title");
       $notes  = lang("import_success_notes");
     } else {
       $pdl->common->alert(lang("import_failed"));
       include("inc/footer.inc");
       exit;
     }
   } else {
 #-----------------------------------------------------[ Handle pwd errors ]---
     $pdl->common->alert(lang("user_auth_fail"));
     include("inc/footer.inc");
     exit;
   }

 } else { // form not (yet) submitted
 #===================================================[ Query user for data ]===
   $ptitle = lang("import_init_title");
   $notes  = lang("import_init_notes");
 }

 #==================================================[ Generate HTML output ]===
 #-----------------------------------------------------[ Prepare templates ]---
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"import.tpl"));
 include("inc/tab_setup.inc");
# $pdl->tabs->activate("dives");
 $pdl->tabs->parse();
 #------------------------------------------------------[ Set up form data ]---
 $t->set_var("formtarget",$pdl->link->slink("$PHP_SELF"));
 $t->set_var("submit",lang("submit_form"));
 $t->set_var("title",$ptitle);
 $t->set_var("notes",$notes);
 $t->pparse("out","template");

 include("inc/footer.inc");
?>