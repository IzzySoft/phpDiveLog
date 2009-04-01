<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Setup the navigation tabs                                                 #
 #############################################################################

 # $Id$

 #======================================================[ Set the help URL ]===
 $help_url = "http://projects.izzysoft.de/trac/phpdivelog/wiki/UserManual/";
 switch($helppage) {
   case "buddylist"  : $help_url .= "GlobalMode/Divers"; break;
   case "globalsites": $help_url .= "GlobalMode/Sites"; break;
   case "prefs"      : $help_url .= "Preferences"; break;
 }

 #================================================[ set up navigation tabs ]===
 require_once(dirname(__FILE__)."/class.tabs.inc");
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $pdl->tabs = new tabs(1);

 $pdl->tabs->add_tab("diver","buddylist.php","tab_buddylist.gif");
 $pdl->tabs->add_tab("sites","places.php","tab_sites.gif");
 $pdl->tabs->add_tab("prefs","prefs.php?tab2=1","tab_preferences.gif");
 $pdl->tabs->add_tab("help",$help_url,"tab_help.gif",1);
 $t->set_block("template","tabblock","tab");

?>