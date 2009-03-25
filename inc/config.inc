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
 #-----------------------------------------------------[ General Settings ]---
 $display_limit = 12;     # how may records/page should be displayed for lists
                          # may be overridden by the "displaylimit" URL parameter
 $pix_per_row   = 3;      # how many pictures to place in one row in the photo
                          # sections. This can be overridden with the URL
                          # parameter "pixinrow"
 $template_set  = "aqua"; # prefered template set: default|aqua
                          # may be overridden with the "tpl" parameter
 $lang          = "en";   # default language if not (yet) specified by visitor
                          # may be overridden by the URL parameter "lang"
 $title = "Demos phpDiveLog"; # site name
 $enable_index  = TRUE;   # whether the buddylist index is available
 $default_page  = "user"; # where to route requests without the "?diver=nick"
                          # to: user|index|error ("index" is not supported yet
                          # and will be handled as "error")
 $default_user  = "demo"; # if default_page=user: which user is the default?
                          # must be a valid account (subdir diver/<user>)

 #----------------------------------------------------[ Geographical Stuff ]---
 $mapsite = "Google";     # Where to show the dive site in a map. Possible
                          # values: "Mapquest" (old style), "Google" (Google Maps),
			  # "Yahoo" (Yahoo Maps)
 $global_kml = TRUE;      # Offer links to global KML files (TRUE) on the global
                          # places list or not (FALSE)

 #--------------------------------------------------------[ Graph Settings ]---
 $sitepix_if_no_divepix = 1; # shall sitepix be included on divepage if there
                          # are no divepix? Only has effect with $sitepix_on_divepage=0
 $sitepix_on_divepage = 0;# whether existing site photos shall be generally
                          # included on corresponding dive pages (1) or not (0)
 $sitepix_first = 0;      # if included on divepage, shall sitepix come first
                          # (1) or follow divepix (0)?
 $sitepix_separate = 0;   # if included on divepage, shall sitepix be separated
                          # from divepix (1) or not (0)?
 $use_dyn_profile_png = 0;# create the dive profile image dynamically (1) or
                          # use the one from the conduit (0)
 $schedule_graph = "integrated";# How to draw the schedule graph (if we have
                          # data for it): "integrated" into the profile graph
			  # (if there is one, else separate), as a "separate"
			  # graph in the schedule block, or "none"
 $hide_schedule_table = TRUE; # Do we really need the data displayed separately?
 $values_in_statgraphs = "auto";# whether you want values printed in the bars
                          # of the graphs on the stat page (yes) or not (no),
			  # or leave the decision to PDL (auto)
 $graph_bg_transparent = FALSE; # use transparent backgroud for graphs?
 $ignore_zero_degrees  = TRUE; # ignore a temperature of 0°C for stats?
 $ignore_zero_degrees_comment = TRUE; # place a comment below the graph if zero
                          # degrees are ignored
 $depth_stat_interval = 5; # intervals for the depth graph (either 5 or 10 m)
 $temp_stat_interval = 10; # intervals for the temperature graph (either 5 or 10 degrees)
 $time_stat_interval = 10; # intervals for the dive duration graph (either 10 or 20 minutes)

 #------------------------------------------------[ Data Transfer Settings ]---
 $pwdfile = "/etc/pdlpwd";# password file for data transfers
 $transfer_dir = "/home/pdl/transfer/"; # base directory of user data transfers

 #--------------------------------------------------------------[ sorting ]----
 $logbook_default_sort   = "date";
 $logbook_default_order  = "desc";
 $sitelist_default_sort  = "location";
 $sitelist_default_order = "asc";

 #-------------------------------------------------------------------[ PDF ]---
 $tcpdf_path = "/usr/share/tcpdf/"; # path to the TCPDF Api files
 $pdf_page_format = "A5";           # paper size: A0..5, B0..5, or LETTER
 $pdf_page_orientation = "P";       # (P)ortrait or (L)andscape. Recommended
                                    # combination: A5+P (A4+L should be handled
                                    # by your printer driver then)
 $pdf_page_gutter = 25;             # margin for the gutter
 $pdf_page_margin = 5;              # margin for the opposite page side
 $pdf_no_profile = "dummy";         # what to use if we have no profile: "dummy"
                                    # for a dummy profile, "blank" for nothing
 $pdf_with_fotos = TRUE;            # include photos in the PDF files? URL
                                    # override: pdfwithfotos=0|1
 $pdf_pageno_from_diveno = TRUE;    # use the dive# as page# to calculate gutter?
 $pdf_enable = TRUE;                # enable PDF support?
 $pdf_max_notechars = 1900;         # restrict notes length so it fits on page
 $pdf_chars_per_pix = 10;           # 10px height make one row - so for each 10px
                                    # we lose ~100 chars, which means 10 chars per pixel

 #-----------------------------[ configuration overrides by divers logbook ]---
 $override_protection = "explicit"; # "implicit": all except for $allow_override
                                    # "explicit": none except for $protected_options
 $allow_override = "";              # which settings may be adjusted via the
                                    # diver/*/system.conf - space separated values
 $protected_options = "enable_index default_page default_user pwd_file transfer_dir tcpdf_path pdf_enable debug_level debug_show debug_log";
                                    # space separated values
                                    # ignored when $override_protection=implicit

 #------------------------------------------------------------------[ misc ]---
 $debug_level = "EW";     # What shall be logged: [E]rrors, [W]arnings,
                          # [N]otices, [A]ll?
 $debug_show = 0;         # Show errors on screen (1) or not (0)
 $debug_log  = 1;         # Save errors to the servers error.log (1) or not (0)
?>
