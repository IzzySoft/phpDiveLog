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
   case "dive"     : $help_url .= "LogBook/DiveDetails"; break;
   case "divelist" : $help_url .= "LogBook/DiveList"; break;
   case "pdf"      : $help_url .= "PdfExport"; break;
   case "prefs"    : $help_url .= "Preferences"; break;
   case "site"     : $help_url .= "Sites/SiteDetails"; break;
   case "sitelist" : $help_url .= "Sites/SiteList"; break;
 }

 #================================================[ set up navigation tabs ]===
 require_once(dirname(__FILE__)."/class.tabs.inc");
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $pdl->tabs = new tabs;

 $pdl->tabs->add_tab("dives","index.php","tab_dives.gif");
 $pdl->tabs->add_tab("stats","stats.php","tab_stats.gif");
 $pdl->tabs->add_tab("sites","sitelist.php","tab_sites.gif");
 if ($pdl->config->display_personal)
   $pdl->tabs->add_tab("person","person.php","tab_person.gif");
 $pdl->tabs->add_tab("prefs","prefs.php","tab_preferences.gif");
 $pdl->tabs->add_tab("pdf_export","pdf_export.php","apdf.png");
 $pdl->tabs->add_tab("help",$help_url,"tab_help.gif",1);
 $t->set_block("template","tabblock","tab");

?>