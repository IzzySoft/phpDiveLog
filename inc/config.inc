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
 $display_limit = 12;     # how may records/page should be displayed for lists
 $template_set  = "aqua"; # prefered template set: default|aqua
 $lang          = "en";   # default language if not (yet) specified by visitor
 $title = "phpDiveLog Dev"; # site name
 $enable_index  = TRUE;   # whether the buddylist index is available
 $default_page  = "user"; # where to route requests without the "?diver=nick"
                          # to: user|index|error ("index" needs enable_index
                          # set to TRUE)
 $default_user  = "demo"; # if default_page=user: which user is the default?
                          # must be a valid account (subdir diver/<user>)
 $allow_override = "title lang display_limit template_set"; 
                          # which settings may be adjusted via the
                          # diver/*/system.conf - space separated values
?>
