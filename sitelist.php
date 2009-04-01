<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Sites Index                                                               #
 #############################################################################

 # $Id$

 $helppage = "sitelist";
 include("inc/includes.inc");
 $title .= ": SiteIndex";
 include("inc/header.inc");
 $start = $pdl->params->start;
 $end = $start + $pdl->config->display_limit;

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"sitelist.tpl"));
 $t->set_block("template","itemblock","item");

 #==============================================[ Import dive data from DB ]===
 $sort = $_REQUEST["sort"]; $order = $_REQUEST["order"];
 if (!in_array($sort,array("id","location","place","depth"))) $sort = "";
 if (!in_array($order,array("desc","asc"))) $order = "";
 if (empty($sort) && !empty($pdl->config->sitelist_default_sort)) {
   $sort  = $pdl->config->sitelist_default_sort;
   $order = $pdl->config->sitelist_default_order;
 }
 $sites = $pdl->db->get_sites($start,$pdl->config->display_limit,FALSE,$sort,$order);
 $max   = count($sites);
 $records = $pdl->db->sites;

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("sites",TRUE);
 $pdl->tabs->parse();
 $pdl->common->prevNext($records,$start);
 $t->set_var("pages",$pdl->common->pages($records,$start));

 #===============================================[ set up the table header ]===
 #--------------------------------------------------------[ sorting images ]---
 $sortimg["up"]["id"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=id&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["id"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=id&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["location"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=location&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["location"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=location&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["place"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=place&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["place"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=place&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 $sortimg["up"]["depth"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=depth&order=asc","<img src='".$pdl->config->tpl_url."images/up.gif'>");
 $sortimg["down"]["depth"] = $pdl->link->linkurl($_SERVER["SCRIPT_NAME"]."?sort=depth&order=desc","<img src='".$pdl->config->tpl_url."images/down.gif'>");
 switch ($sort) {
   case "id"       : if ($order=="desc") {
                       $sortimg["down"]["id"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                     } else {
                       $sortimg["up"]["id"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                     } break;
   case "location" : if ($order=="desc") {
                       $sortimg["down"]["location"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                     } else {
                       $sortimg["up"]["location"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                     } break;
   case "place"    : if ($order=="desc") {
                       $sortimg["down"]["place"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                     } else {
                       $sortimg["up"]["place"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                     } break;
   case "depth"    : if ($order=="desc") {
                       $sortimg["down"]["depth"] = "<img src='".$pdl->config->tpl_url."images/down-grey.gif'>";
                     } else {
                       $sortimg["up"]["depth"] = "<img src='".$pdl->config->tpl_url."images/up-grey.gif'>";
                     } break;
 }
 #--------------------------------------------[ table header template vars ]---
 $t->set_var("id_name","ID&nbsp;".$sortimg["up"]["id"].$sortimg["down"]["id"]);
 $t->set_var("loc_name",lang("location")."&nbsp;".$sortimg["up"]["location"].$sortimg["down"]["location"]);
 $t->set_var("place_name",lang("place")."&nbsp;".$sortimg["up"]["place"].$sortimg["down"]["place"]);
 $t->set_var("md_name",lang("max_depth")."&nbsp;".$sortimg["up"]["depth"].$sortimg["down"]["depth"]);
 $pdl_url = $pdl->link->get_baseurl()."/sitelist_kml.php?diver=$diver";
 $t->set_var("dl_kml",lang("show_kml")." [<a href='sitelist_kml.php?diver=$diver'>Google Earth</a> | <a href='http://maps.google.com/?q=".urlencode($pdl_url)."' TARGET='_blank'>Google Maps</a>]");

 #============================[ Walk through the list and set up the table ]===
 $details = array ("id","loc","place","depth");
 for ($i=0;$i<$max;++$i) {
   foreach($details AS $detail) {
     $t->set_var("$detail",$pdl->common->null2nbsp($sites[$i][$detail]));
   }
   if (!$sites[$i]["depth"]) {
     $t->set_var("depth","&nbsp;");
   } else {
     $t->set_var("depth",$sites[$i]["depth"]);
   }
   $t->set_var("site_ref",$pdl->link->linkurl("site.php?id=".$sites[$i]["id"],$sites[$i]["id"]));
#   $t->set_var("rating",$pdl->config->tpl_url."images/".$sites[$i]["rating"]."star.gif");
   if ( $pdl->file->havePix($sites[$i]["id"],"site") ) {
     $t->set_var("pix",'<img src="'.$pdl->config->tpl_url.'images/camera.gif" valign="middle">');
   } else {
     $t->set_var("pix","&nbsp;");
   }
   $t->parse("item","itemblock",$i);
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>