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

 $pdl->config->version = "0.1.3";
 $pdl->config->user    = "";
 if ( !empty($_GET["tpl"]) ) $template_set = $_GET["tpl"];

 #==========================================================[ directories ]===
 $pdl->config->base_path = ""; # if set, has to include trailing slash - if
 $pdl->config->base_url  = ""; #   not, must be empty
 $pdl->config->tpl_path  = $pdl->config->base_path."templates/$template_set/";
 $pdl->config->tpl_url   = $pdl->config->base_url."templates/$template_set/";
 if (!empty($pdl->config->user)) $relPath = $pdl->config->user."/";
 $pdl->config->user_path = $pdl->config->base_path.$relPath;
 $pdl->config->user_url  = $pdl->config->base_url.$relPath;

 #=============================================================[ database ]===
 $pdl->config->db_type  = $database["type"];
 $pdl->config->datadir  = $database["datadir"];
 $pdl->config->divedata = $database["datadir"]."logbook.csv";
 $pdl->config->sites    = $database["datadir"]."divesites.csv";
 unset($database);

 #==========================================================[ preferences ]===
 $pdl->config->display_limit = $display_limit;
 unset($display_limit,$template_set);
?>
