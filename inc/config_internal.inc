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

 $pdl->config->version  = "0.1.1";

 #==========================================================[ directories ]===
 $pdl->config->tpl_url  = "templates/default/";
 $pdl->config->tpl_path = "templates/default/";

 #=============================================================[ database ]===
 $pdl->config->db_type  = $database["type"];
 $pdl->config->datadir  = $database["datadir"];
 $pdl->config->divedata = $database["datadir"]."logbook.csv";
 $pdl->config->sites    = $database["datadir"]."divesites.csv";
 unset($database);

 #==========================================================[ preferences ]===
 $pdl->config->display_limit = $display_limit;
 unset($display_limit);
?>
