<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Main configuration file - the only one for the admin to edit!             #
 # (The diver may partially override settings in his/her diver/*/system.conf)#
 #############################################################################

 /* $Id$ */

 #=============================================================[ database ]===
 $database["type"]      = "csv";   # csv|mysql|pgsql (only csv supported now)

 #==========================================================[ preferences ]===
 $display_limit = 12;     # how may records/page should be displayed for lists
 $template_set  = "aqua"; # prefered template set: default|aqua
 $lang          = "en";   # default language if not (yet) specified by visitor
 $title = "Demos phpDiveLog"; # site name
 $enable_index  = TRUE;   # whether the buddylist index is available
 $default_page  = "user"; # where to route requests without the "?diver=nick"
                          # to: user|index|error ("index" is not supported yet
                          # and will be handled as "error")
 $default_user  = "demo"; # if default_page=user: which user is the default?
                          # must be a valid account (subdir diver/<user>)
 $sitepix_on_divepage = 0;# whether existing site photos shall be included on
                          # corresponding dive pages (1) or not (0)
 $use_dyn_profile_png = 0;# create the dive profile image dynamically (1) or
                          # use the one from the conduit (0)
 $schedule_graph = "integrated";# How to draw the schedule graph (if we have
                          # data for it): "integrated" into the profile graph
			  # (if there is one, else separate), as a "separate"
			  # graph in the schedule block, or "none"
 $values_in_statgraphs = "auto";# whether you want values printed in the bars
                          # of the graphs on the stat page (yes) or not (no),
			  # or leave the decision to PDL (auto)
 $graph_bg_transparent = FALSE; # use transparent backgroud for graphs?
 $ignore_zero_degrees  = TRUE; # ignore a temperature of 0°C for stats?
 $ignore_zero_degrees_comment = TRUE; # place a comment below the graph if zero
                          # degrees are ignored
 $allow_override = "mapsite title lang display_limit template_set userdef1 userdef2 sitepix_on_divepage values_in_statgraphs graph_bg_transparent ignore_zero_degrees ignore_zero_degrees_comment"; 
                          # which settings may be adjusted via the
                          # diver/*/system.conf - space separated values
 $pwdfile = "/etc/pdlpwd";# password file for data transfers
 $transfer_dir = "/home/pdl/transfer/"; # base directory of user data transfers
 $mapsite = "Google";     # Where to show the dive site in a map. Possible
                          # values: "Mapquest" (old style), "Google" (Google Maps),
			  # "Yahoo" (Yahoo Maps)
 $global_kml = TRUE;      # Offer links to global KML files (TRUE) on the global
                          # places list or not (FALSE)
 #--------------------------------------------------------------[ sorting ]----
 $logbook_default_sort   = "date";
 $logbook_default_order  = "desc";
 $sitelist_default_sort  = "location";
 $sitelist_default_order = "asc";

 #------------------------------------------------------------------[ misc ]---
 $debug_level = "EW";     # What shall be logged: [E]rrors, [W]arnings,
                          # [N]otices, [A]ll?
 $debug_show = 0;         # Show errors on screen (1) or not (0)
 $debug_log  = 1;         # Save errors to the servers error.log (1) or not (0)
?>
