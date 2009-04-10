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

 #================================================[ set up navigation tabs ]===
 require_once(dirname(__FILE__)."/class.tabs.inc");
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $pdl->tabs = new tabs("logbook");

 $pdl->tabs->add_tab("dives","index.php","tab_dives.gif");
 $pdl->tabs->add_tab("stats","stats.php","tab_stats.gif");
 $pdl->tabs->add_tab("sites","sitelist.php","tab_sites.gif");
 if ($pdl->config->display_personal)
   $pdl->tabs->add_tab("person","person.php","tab_person.gif");
 $pdl->tabs->add_tab("prefs","prefs.php","tab_preferences.gif");
 $pdl->tabs->add_tab("pdf_export","pdf_export.php","apdf.png");
 if (basename($_SERVER["PHP_SELF"])=="filter.php") {
   $pdl->tabs->add_tab("filter","filter.php?filter=","filterstop.png");
 } else {
   $pdl->tabs->add_tab("filter","filter.php","filter.png");
 }
 if (!empty($helppage)) $pdl->tabs->add_tab("help",$helppage,"tab_help.gif",1);
 $t->set_block("template","tabblock","tab");

?>