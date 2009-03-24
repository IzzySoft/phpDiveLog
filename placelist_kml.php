<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Generate .kml file for Google Earth / Google Maps                         #
 #############################################################################

 # $Id$

 #=================================================[ Setup & Configuration ]===
 include("inc/includes.inc");
 #---------------------------------------------[ KickOff when switched off ]---
 if (!$pdl->config->global_kml) {
   header("HTTP/1.1 403 Forbidden");
   $title .= ": ".lang("feature_unavailable_title",lang("global_kml"));
   include("inc/header.inc");
   $pdl->common->alert(lang("feature_unavailable_desc"));
   include("inc/footer.inc");
   exit;
 }
 $refresh_interval = 604800; // 1 week
 $tree_open        = 0; // open all elements in GE (0|1)

 #-=[ Get base URL for images ]=-
 $pdl_url = $pdl->link->get_baseurl();
 $img_url = $pdl_url . "/" . substr($pdl->config->tpl_url."images/",strlen($pdl->config->base_url));

 #-=[ Setup Template ]=-
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"placelist_kml.tpl"));
 $t->set_block("template","linkblock","link");

 #===================================================[ Get list of buddies ]===
 $buddies = $pdl->file->get_buddies();
 $max     = count($buddies);
 $records = $pdl->file->buddies;

 #=========================[ Walk through the list and set up the KML data ]===
 $filename = "pdl_allsites.kml";
 $t->set_var("doc_name","DiveLog Collection by ".$pdl->config->site_title);
 $t->set_var("open",$tree_open);
 for ($i=0;$i<$max;++$i) {
   if (empty($buddies[$i])) continue;
   $t->set_var("link_name","DiveSites of ".ucfirst($buddies[$i]));
   $t->set_var("link_description","DiveSites of ".ucfirst($buddies[$i]));
   $t->set_var("link_url","${pdl_url}/sitelist_kml.php?diver=".$buddies[$i]);
   $t->set_var("link_refresh_interval",$refresh_interval);
   $t->parse("link","linkblock",TRUE);
 }

 #====================================================[ Send the .kml file ]===
 header("Content-Type: application/vnd.google-earth.kml+xml");
 header("Content-Disposition: attachment; filename=$filename");
 $t->pparse("out","template");
?>