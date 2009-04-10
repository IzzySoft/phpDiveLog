<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://projects.izzysoft.de/                                              #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Filter/Search page                                                        #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 $helppage = "filter";
 include("inc/includes.inc");
 $title .= ": ".lang("filter");

 #================================================[ Process submitted form ]===
 if (isset($_POST["submit"])) switch($_POST["submit"]) {
 #----------------------------------------------------------[ Export Dives ]---
   case lang("filter_dives") :
     $fields = array("date","location","place","rating","depth","divetime","buddy");
     $filter = "";
     foreach ($fields as $field) {
       if ( isset($_POST[$field]) && !empty($_POST[$field]) ) {
         $filter .= "|$field|".$_POST["${field}_comp"]."|".$_POST[$field];
       }
     }
     if (!empty($filter)) {
       $url = $pdl->link->slink("index.php?filter=".substr($filter,1));
       header("Location: $url");
       exit;
     }
     break;
   case lang("filter_sites") :
     $fields = array("location","place","depth");
     $filter = "";
     foreach ($fields as $field) {
       if ( isset($_POST[$field]) && !empty($_POST[$field]) ) {
         $filter .= "|$field|".$_POST["${field}_comp"]."|".$_POST[$field];
       }
     }
     if (!empty($filter)) {
       $url = $pdl->link->slink("sitelist.php?filter=".substr($filter,1));
       header("Location: $url");
       exit;
     }
     break;
 }

 #===================================================[ Initialize template ]===
 include("inc/header.inc");
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"filter.tpl"));
 $t->set_block("template","formblock","form");
 $t->set_block("formblock","itemblock","item");

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("filter");
 $pdl->tabs->parse();

 #===============================[ set up the table header and field names ]===
 $t->set_var("ptitle",lang("filter"));
 $t->set_var("comp_bubble",lang("filter_comp_desc"));
 $compOpts = "<option value='~'>LIKE</option><option value='eq'>=</option>"
           . "<option value='ne'>!=</option><option value='lt'>&lt;</option>"
           . "<option value='le'>&lt;=</option><option value='ge'>&gt;=</option>"
           . "<option value='gt'>&gt;</option>";
 $t->set_var("comp_opts",$compOpts);
 $t->set_var("formmethod","POST");
 $t->set_var("formtarget",$_SERVER["REQUEST_URI"]);
 $t->set_var("submit_name","submit");

 #============================================================[ Dives Form ]===
 $t->set_var("formname","dives");
 $t->set_var("icon_src",$pdl->config->base_url."templates/aqua/images/tab_dives.gif");
 $t->set_var("icon_width","30");
 $t->set_var("icon_height","15");
 $t->set_var("icon_alt",lang("dives"));
 $t->set_var("submit_bubble",lang("filter_dives_exec_desc"));
 #---------------------------------------------------------------[ Filters ]---
 // Date
 $t->set_var("name_bubble",lang("filter_date_desc"));
 $t->set_var("val_bubble",lang("filter_date_val_desc"));
 $t->set_var("name",lang("date"));
 $t->set_var("comp","date_comp");
 $t->set_var("input","date");
 $t->set_var("value","");
 $t->parse("item","itemblock");
 // Location
 $t->set_var("name_bubble",lang("filter_location_desc"));
 $t->set_var("val_bubble",lang("filter_location_val_desc"));
 $t->set_var("name",lang("location"));
 $t->set_var("comp","location_comp");
 $t->set_var("input","location");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Place
 $t->set_var("name_bubble",lang("filter_place_desc"));
 $t->set_var("val_bubble",lang("filter_place_val_desc"));
 $t->set_var("name",lang("place"));
 $t->set_var("comp","place_comp");
 $t->set_var("input","place");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Rating
 $t->set_var("name_bubble",lang("filter_rating_desc"));
 $t->set_var("val_bubble",lang("filter_rating_val_desc"));
 $t->set_var("name",lang("rating"));
 $t->set_var("comp","rating_comp");
 $t->set_var("input","rating");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Depth
 $t->set_var("name_bubble",lang("filter_depth_desc"));
 $t->set_var("val_bubble",lang("filter_depth_val_desc"));
 $t->set_var("name",lang("depth"));
 $t->set_var("comp","depth_comp");
 $t->set_var("input","depth");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Divetime
 $t->set_var("name_bubble",lang("filter_divetime_desc"));
 $t->set_var("val_bubble",lang("filter_divetime_val_desc"));
 $t->set_var("name",lang("divetime"));
 $t->set_var("comp","divetime_comp");
 $t->set_var("input","divetime");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Buddy
 $t->set_var("name_bubble",lang("filter_buddy_desc"));
 $t->set_var("val_bubble",lang("filter_buddy_val_desc"));
 $t->set_var("name",lang("buddy"));
 $t->set_var("comp","buddy_comp");
 $t->set_var("input","buddy");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);

 $t->set_var("segment_name",lang("dives"));
 $t->set_var("submit_value",lang("filter_dives"));
 $t->parse("form","formblock");

 #============================================================[ Sites Form ]===
 $t->set_var("formname","sites");
 $t->set_var("icon_src",$pdl->config->base_url."templates/aqua/images/tab_sites.gif");
 $t->set_var("icon_width","15");
 $t->set_var("icon_height","15");
 $t->set_var("icon_alt",lang("sites"));
 $t->set_var("submit_bubble",lang("filter_sites_exec_desc"));
 #---------------------------------------------------------------[ Filters ]---
 // Location
 $t->set_var("name_bubble",lang("filter_location_desc"));
 $t->set_var("val_bubble",lang("filter_location_val_desc"));
 $t->set_var("name",lang("location"));
 $t->set_var("comp","location_comp");
 $t->set_var("input","location");
 $t->set_var("value","");
 $t->parse("item","itemblock");
 // Place
 $t->set_var("name_bubble",lang("filter_place_desc"));
 $t->set_var("val_bubble",lang("filter_place_val_desc"));
 $t->set_var("name",lang("place"));
 $t->set_var("comp","place_comp");
 $t->set_var("input","place");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);
 // Depth
 $t->set_var("name_bubble",lang("filter_depth_desc"));
 $t->set_var("val_bubble",lang("filter_depth_val_desc"));
 $t->set_var("name",lang("depth"));
 $t->set_var("comp","depth_comp");
 $t->set_var("input","depth");
 $t->set_var("value","");
 $t->parse("item","itemblock",TRUE);

/*
 $t->set_var("name_bubble",lang(""));
 $t->set_var("val_bubble",lang(""));
 $t->set_var("name",lang(""));
 $t->set_var("comp",);
 $t->set_var("input",);
 $t->set_var("value",);
 $t->parse("item","itemblock",TRUE);
*/

 $t->set_var("segment_name",lang("sites"));
 $t->set_var("submit_value",lang("filter_sites"));
 $t->parse("form","formblock",TRUE);

 #===========================================================[ End of Page ]===
 $t->pparse("out","template");

 include("inc/footer.inc");
?>