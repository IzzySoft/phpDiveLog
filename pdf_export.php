<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://projects.izzysoft.de/                                              #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Initial form for multi-page PDF generation                                #
 #############################################################################

 #================================================[ Initialize environment ]===
 $helppage = "pdf-export";
 include("inc/includes.inc");
 $title .= ": ".lang("pdf_export");

 #================================================[ Process submitted form ]===
 if (isset($_POST["submit"])) switch($_POST["submit"]) {
 #----------------------------------------------------------[ Export Dives ]---
   case lang("dives") :
     if ( empty($_POST["from"]) || empty($_POST["to"]) ) {
       $dives = $pdl->db->get_dives("","",FALSE,"id","ASC");
       if ( empty($_POST["from"]) ) $_POST["from"] = $dives[0]["dive#"];
       if ( empty($_POST["to"]) )   $_POST["to"]   = $dives[$pdl->db->dives-1]["dive#"];
     }
     $url = $pdl->link->slink("dive_pdf.php?nr=".$_POST["from"]."&lastnr=".$_POST["to"]."&duplex=".$_POST["duplex"]."&pdfwithfotos=".$_POST["foto"]."&pdfcreatemissinggraph=".$_POST["graph"]);
     header("Location: $url");
     exit;
     break;
   case lang("sites") :
     if ( empty($_POST["from"]) || empty($_POST["to"]) ) {
       $sites = $pdl->db->get_sites("","",FALSE,"id","ASC");
       if ( empty($_POST["from"]) ) $_POST["from"] = $sites[0]["id"];
       if ( empty($_POST["to"]) )   $_POST["to"]   = $sites[$pdl->db->sites-1]["id"];
     }
     $url = $pdl->link->slink("site_pdf.php?nr=".$_POST["from"]."&lastnr=".$_POST["to"]."&duplex=".$_POST["duplex"]."&pdfwithfotos=".$_POST["foto"]);
     header("Location: $url");
     exit;
     break;
   case lang("dive_stats") :
     $url = $pdl->link->slink("stats_pdf.php?duplex=".$_POST["duplex"]."&pdfwithfotos=".$_POST["foto"]);
     header("Location: $url");
     exit;
     break;
   case lang("export_empty_dives") :
     $url = $pdl->link->slink("dive_pdf.php?nr=0&count=".$_POST["dsheets"]."&duplex=".$_POST["duplex"]."&pdfwithfotos=".$_POST["foto"]."&pdfcreatemissinggraph=".$_POST["graph"]);
     header("Location: $url");
     exit;
     break;
 }

 #===================================================[ Initialize template ]===
 include("inc/header.inc");
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"pdf_export.tpl"));
 $t->set_block("template","formblock","form");
 $t->set_block("formblock","missingblock","missing");

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("pdf_export");
 $pdl->tabs->parse();
 $arrowheight = "height='9'";

 #===============================[ set up the table header and field names ]===
 $t->set_var("ptitle",lang("pdf_export"));
 $t->set_var("fsel_name",lang("pdf_select_pages").":");
 $t->set_var("foutput_name",lang("pdf_duplex_type").":");
 $t->set_var("foutput_viewname",lang("pdf_duplex_none"));
 $t->set_var("foutput_innername",lang("pdf_duplex_inner"));
 $t->set_var("foutput_outername",lang("pdf_duplex_outer"));
 $t->set_var("foutput_sidename",lang("pdf_duplex_side"));
 $t->set_var("ffoto_name",lang("pdf_include_fotos").":");
 $t->set_var("fyes",lang("yes"));
 $t->set_var("fno",lang("no"));
 $t->set_var("formmethod","POST");
 $t->set_var("formtarget",$_SERVER["REQUEST_URI"]);
 $t->set_var("foutput_viewcheck","");
 $t->set_var("foutput_innercheck","CHECKED");
 $t->set_var("foutput_outercheck","");
 $t->set_var("foutput_sidecheck","");
 if (PDF_WITH_FOTOS) {
   $t->set_var("ffoto_yescheck","CHECKED");
   $t->set_var("ffoto_nocheck","");
 } else {
   $t->set_var("ffoto_yescheck","");
   $t->set_var("ffoto_nocheck","CHECKED");
 }
 $t->set_var("submit_name","submit");
 $t->set_var("pages_bubble",lang("pdf_select_pages_desc"));
 $t->set_var("output_bubble",lang("pdf_duplex_type_desc"));
 $t->set_var("output_view_bubble",lang("pdf_duplex_none_desc"));
 $t->set_var("output_innergutter_bubble",lang("pdf_duplex_inner_desc"));
 $t->set_var("output_outergutter_bubble",lang("pdf_duplex_outer_desc"));
 $t->set_var("output_sidegutter_bubble",lang("pdf_duplex_side_desc"));
 $t->set_var("foto_bubble",lang("pdf_include_fotos_desc"));
 $t->set_var("foto_yes_bubble",lang("pdf_include_fotos_yes"));
 $t->set_var("foto_no_bubble",lang("pdf_include_fotos_no"));
 $t->set_var("create_head",lang("create_empty"));
 $t->set_var("export_head",lang("export_stuff"));
 $t->set_var("export_options",lang("options"));

 #============================================================[ Dives Form ]===
 if (USE_DYN_PROFILE_PNG) {
   $t->set_var("missing_bubble",lang("pdf_create_missing_graph_desc"));
   $t->set_var("fmissing_name",lang("pdf_create_missing_graph").":");
   $t->set_var("missing_yes_bubble",lang("pdf_create_missing_graph_yes"));
   $t->set_var("missing_no_bubble",lang("pdf_create_missing_graph_no"));
   if (PDF_CREATE_MISSING_GRAPH) {
     $t->set_var("fmissing_check","CHECKED");
     $t->set_var("fmissing_nocheck","");
   } else {
     $t->set_var("missing_nocheck","CHECKED");
     $t->set_var("missing_check","");
   }
   $t->parse("missing","missingblock");
 } else {
   $t->set_var("missing","");
 }
 $t->set_var("formname","dives");
 $t->set_var("icon_src",$pdl->config->base_url."templates/aqua/images/tab_dives.gif");
 $t->set_var("icon_width","30");
 $t->set_var("icon_height","15");
 $t->set_var("icon_alt",lang("dives")." / ".lang("sites"));
 $t->set_var("segment_name",lang("dives")." / ".lang("sites"));
 $t->set_var("submit1_value",lang("dives"));
 $t->set_var("submit1_title",lang("export_dives_desc"));
 $t->set_var("submit2_value",lang("sites"));
 $t->set_var("submit2_title",lang("export_sites_desc"));
 $t->set_var("submit3_value",lang("dive_stats"));
 $t->set_var("submit3_title",lang("export_stats_desc"));
 $t->set_var("submit4_value",lang("export_empty_dives"));
 $t->set_var("empty_divesheet_bubble",lang("export_empty_dives_desc"));
 $t->parse("form","formblock");

 #===========================================================[ End of Page ]===
 $t->pparse("out","template");

 include("inc/footer.inc");
?>