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

 $pdl->config->version = "0.1.4";
 if (!empty($_GET["diver"])) {
   $relPath = "diver/".$_GET["diver"]."/";
 } elseif (strtolower($default_page) == "user") {
   $relPath = "diver/$default_user/";
 }

 #==========================================================[ preferences ]===
 if ( !empty($_GET["tpl"]) )  $template_set = $_GET["tpl"];
 $pdl->config->display_limit = $display_limit;
 if ( !empty($_GET["lang"]) ) {
   $lang = $_GET["lang"];
 }

 #==========================================================[ directories ]===
 $pdl->config->base_path = ""; # if set, has to include trailing slash - if
 $pdl->config->base_url  = ""; #   not, must be empty
 $pdl->config->tpl_path  = $pdl->config->base_path."templates/$template_set/";
 $pdl->config->tpl_url   = $pdl->config->base_url."templates/$template_set/";
 $pdl->config->user_path = $pdl->config->base_path.$relPath;
 $pdl->config->user_url  = $pdl->config->base_url.$relPath;

 #=============================================================[ database ]===
 $pdl->config->db_type  = $database["type"];
 $pdl->config->datadir  = $pdl->config->user_path."data/";
 $pdl->config->divedata = $pdl->config->datadir."logbook.csv";
 $pdl->config->sites    = $pdl->config->datadir."divesites.csv";
 unset($database,$display_limit);
?>
