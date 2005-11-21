<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2005 by Itzchak Rehberg #
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
# $database["type"]      = "mysql";     # csv|mysql|pgsql (only csv supported now)
 $database["type"]      = "cvs";     # csv|mysql|pgsql (only csv supported now)
 $database["host"]      = "localhost"; # your hosts name
 $database["database"]  = "pdl";       # what database holds the tables
 $database["user"]      = "pdl";       # db user with at least following
                                       # privileges on these tables:
				       # SELECT,UPDATE,INSERT,DELETE
 $database["password"]  = "";          # this users password for the db

 #==========================================================[ preferences ]===
 $display_limit = 12;     # how may records/page should be displayed for lists
 $template_set  = "aqua"; # prefered template set: default|aqua
 $lang          = "en";   # default language if not (yet) specified by visitor
 $title = "phpDiveLog Dev"; # site name
 $enable_index  = TRUE;   # whether the buddylist index is available
 $default_page  = "user"; # where to route requests without the "?diver=nick"
                          # to: user|index|error ("index" is not supported yet
                          # and will be handled as "error")
 $default_user  = "demo"; # if default_page=user: which user is the default?
                          # must be a valid account (subdir diver/<user>)
 $allow_override = "title lang display_limit template_set userdef1 userdef2"; 
                          # which settings may be adjusted via the
                          # diver/*/system.conf - space separated values
 $pwdfile = "/etc/pdlpwd";# password file for data transfers
 $transfer_dir = "/home/pdl/transfer/"; # base directory of user data transfers
?>
