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

 #=============================================================[ database ]===
 $database["type"]      = "csv";   # csv|mysql|pgsql (only csv supported now)

 #==========================================================[ preferences ]===
 $display_limit = 10;     # how may records/page should be displayed for lists
 $template_set  = "aqua"; # prefered template set: default|aqua
 $lang          = "en";   # default language if not (yet) specified by visitor
 $title = "phpDiveLog Dev"; # site name
?>
