<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
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
 $pdl_url = strtolower($_SERVER["SERVER_PROTOCOL"]);
 $pos = strpos ($pdl_url,"/");
 $pdl_url = substr($pdl_url,0,$pos)."://".$_SERVER["HTTP_HOST"];
 if ($_SERVER["SERVER_PORT"]!=80) $pdl_url .= ":".$_SERVER["SERVER_PORT"];
 $pdl_url .= dirname($_SERVER["PHP_SELF"]);
 $img_url = $pdl_url."/".$pdl->config->tpl_url."images/";

 #-=[ Convert coordinates to decimal ]=-
 function mk_coord($str) {
   if ( preg_match("/([0-9]+)[^0-9]+([0-9]+)[^0-9.]+([0-9.]+)[^0-9]+/",$str,$match) ) {
     $code[0] = $match[1]; $code[1] = $match[2]; $code[2] = $match[3];
   } elseif ( preg_match("/([0-9]+)[^0-9]+([0-9.]+)[^0-9.]+/",$str,$match) ) {
     $code[0] = $match[1]; $code[1] = floor($match[2]);
     $code[2] = ($match[2] - floor($match[2])) * 60;
   } elseif ( preg_match("/([0-9.]+)[^0-9.]+/",$str,$match) ) {
     $code[0] = floor($match[1]);
     $code[1] = floor( ($match[1] - $code[0]) * 60);
     $code[2] = floor( (($match[1] - $code[0]) * 60 - $code[1]) * 60 );
   }
   if ( substr($str,strlen($str)-1)=="N" || substr($str,strlen($str)-1)=="E" ) {
     $code[4] = 1;
   } else {
     $code[4] = -1;
   }
   $min  = $code[1] + $code[2]/60;
   $coord = ($code[0] + $min/60) * $code[4];
   return $coord;
 }

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
# $sites = $pdl->db->get_sites($start,$pdl->config->display_limit,FALSE,$sort,$order);
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
   $latitude  = mk_coord($sites[$i]["latitude"]);
   $longitude = mk_coord($sites[$i]["longitude"]);
   if ($latitude == 0 || $longitude == 0) continue;
   $depth = $sites[$i]["depth"]; # if (!empty($depth)) $depth .= "m";
   $t->set_var("placemarkname",$sites[$i]["loc"].": ".$sites[$i]["place"]." ($depth)");
   $t->set_var("latitude",$latitude);
   $t->set_var("longitude",$longitude);
   $sites[$i]["description"] = $pdl->common->nl2br($sites[$i]["description"]);
   if ($include_firstpic) {
     $fotos = $pdl->file->getSitePix($sites[$i]["id"]);
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