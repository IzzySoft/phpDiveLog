<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Dive Statistics                                                           #
 #############################################################################

 # $Id$

 $title = "Izzys Dive LogBook";
 include("inc/includes.inc");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"stats.tpl"));

 #================================================[ set up navigation tabs ]===
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("sites_tab_name","Sites");
 $t->set_var("sites_ref","sitelist.php");

 #==================================================[ Set up table headers ]===
 $t->set_var("title","Dive Statistics");
 $t->set_var("max_depth_name","Max Depth:");
 $t->set_var("max_time_name","Max Divetime:");
 $t->set_var("avg_depth_name","Avg Depth:");
 $t->set_var("avg_time_name","Avg Divetime:");
 $t->set_var("dive_num_name","Number of Dives:");
 $t->set_var("cum_time_name","Cumulated Divetime:");

 #================================================[ Import statistics data ]===
 $stats = $pdl->db->get_stats();
 $t->set_var("max_depth",$stats["max_depth"]);
 $t->set_var("max_time",$stats["max_time"]);
 $t->set_var("avg_depth",$stats["avg_depth"]);
 $t->set_var("avg_time",$stats["avg_time"]);
 $t->set_var("dive_num",$stats["dive_num"]);
 $t->set_var("cum_time",$stats["cum_dive_time"]);

 $t->pparse("out","template");

 include("inc/footer.inc");
?>