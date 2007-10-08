<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Dive Statistics                                                           #
 #############################################################################

 # $Id$

 include("inc/includes.inc");
 $title .= ": ".lang("statistics");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"stats.tpl"));

 #================================================[ set up navigation tabs ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("stats",TRUE);
 $pdl->tabs->parse();

 #==================================================[ Set up table headers ]===
 $t->set_var("title",lang("dive_stats"));
 $t->set_var("max_depth_name",lang("max_depth").":");
 $t->set_var("max_time_name",lang("max_divetime").":");
 $t->set_var("avg_depth_name",lang("avg_depth").":");
 $t->set_var("avg_time_name",lang("avg_divetime").":");
 $t->set_var("dive_num_name",lang("num_dives").":");
 $t->set_var("cum_time_name",lang("cum_divetime").":");
 $t->set_var("site_num_name",lang("num_sites").":");
 $t->set_var("avg_sd_name",lang("avg_dives_per_site").":");

 #================================================[ Import statistics data ]===
 $stats = $pdl->db->get_stats();
 $t->set_var("max_depth",$stats["max_depth"]);
 $t->set_var("max_time",$stats["max_time"]);
 $t->set_var("avg_depth",$stats["avg_depth"]);
 $t->set_var("avg_time",$stats["avg_time"]);
 $t->set_var("dive_num",$stats["num_dives"]);
 $t->set_var("cum_time",$stats["cum_dive_time"]);
 $t->set_var("site_num",$pdl->db->sites);
 $t->set_var("avg_sd",round($stats["num_dives"] / $pdl->db->sites,3));

 $t->pparse("out","template");

 include("inc/footer.inc");
?>