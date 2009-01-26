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
 $include_firstpic = TRUE;
 $start = 0;

 #-=[ Get base URL for images ]=-
 $pdl_url = $pdl->link->get_baseurl();
 $img_url = $pdl_url."/".$pdl->config->tpl_url."images/";

 #-=[ Setup Template ]=-
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"sitelist_kml.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $sort = $_REQUEST["sort"]; $order = $_REQUEST["order"];
 if (!in_array($sort,array("location","place","depth"))) $sort = "";
 if (!in_array($order,array("desc","asc"))) $order = "";
 if (empty($sort) && !empty($pdl->config->sitelist_default_sort)) {
   $sort  = $pdl->config->sitelist_default_sort;
   $order = $pdl->config->sitelist_default_order;
 }
 $sites = $pdl->db->get_sites($start,999999,FALSE,$sort,$order);
 $max   = count($sites);
 $records = $pdl->db->sites;

 #=========================[ Walk through the list and set up the KML data ]===
 $filename = "${diver}_sites.kml";
 $t->set_var("docname","$filename");
 $t->set_var("site_icon",$img_url."fin.gif");
 $t->set_var("foldername","DiveSites of ".ucfirst($diver));
 $t->set_var("viewingdistance","1000");
 $details = array ("altitude","description");
 for ($i=0;$i<$max;++$i) {
   $latitude  = $pdl->link->coord2dec($sites[$i]["latitude"]);
   $longitude = $pdl->link->coord2dec($sites[$i]["longitude"]);
   if ($latitude == 0 || $longitude == 0) continue;
   $depth = $sites[$i]["depth"]; # if (!empty($depth)) $depth .= "m";
   $loc = $sites[$i]["loc"].": ".$sites[$i]["place"];
   if (!empty($depth)) $loc .= " ($depth)";
   $t->set_var("placemarkname",$loc);
   $t->set_var("latitude",$latitude);
   $t->set_var("longitude",$longitude);
   $sites[$i]["description"] = $pdl->common->nl2br($sites[$i]["description"]);
   if ($include_firstpic) {
     $fotos = $pdl->file->getSitePix($sites[$i]["id"]);
     if (!empty($fotos[0]->url))
       $sites[$i]["description"] = "<IMG ALIGN='right' ALT='' SRC='$pdl_url/".$fotos[0]->url."'>".$sites[$i]["description"];
   }
   foreach($details AS $detail) {
     $t->set_var("$detail",$sites[$i][$detail]);
   }
   $t->parse("item","itemblock",TRUE);
 }

 #====================================================[ Send the .kml file ]===
 header("Content-Type: application/vnd.google-earth.kml+xml");
 header("Content-Disposition: attachment; filename=$filename");
 $t->pparse("out","template");
?>