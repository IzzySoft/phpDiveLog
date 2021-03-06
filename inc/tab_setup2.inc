<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Setup the navigation tabs                                                 #
 #############################################################################

 # $Id$

 #================================================[ set up navigation tabs ]===
 require_once(dirname(__FILE__)."/class.tabs.inc");
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $pdl->tabs = new tabs("global");

 $pdl->tabs->add_tab("diver","buddylist.php","tab_buddylist.gif");
 $pdl->tabs->add_tab("sites","places.php","tab_sites.gif");
 $pdl->tabs->add_tab("prefs","prefs.php?tab2=1","tab_preferences.gif");
 if (!empty($helppage)) $pdl->tabs->add_tab("help",$pdl->config->help_base."#${helppage}","tab_help.gif",1);
 $t->set_block("template","tabblock","tab");

?>