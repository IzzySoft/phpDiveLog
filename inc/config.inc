<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # User's configuration file - the only one for the user to edit!            #
 #############################################################################

 /* $Id$ */

 #==========================================================[ directories ]===
 #$base_url   = ""; # path relative to web server's DOCUMENT_ROOT
 #$base_path  = ""; # absolute file systems path

 #=============================================================[ database ]===
 $database["type"]      = "csv";     # csv|mysql|pgsql (only csv supported now)
 #$database["host"]      = "";        # your hosts name
 #$database["database"]  = "";        # what database holds the tables
 #$database["user"]      = "";        # db user with at least following
                                      # privileges on these tables:
				      # SELECT,UPDATE,INSERT,DELETE
 #$database["password"]  = "";  # this users password for the db

 # the following lines apply for text databases (CSV/UDDF) only
 $database["divedata"]  = "data/logbook.csv";
 $database["sites"]  = "data/sites.csv";
?>
