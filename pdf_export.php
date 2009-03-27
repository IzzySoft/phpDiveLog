<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://projects.izzysoft.de/                                              #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Initial form for multi-page PDF generation                                #
 #############################################################################

 # $Id$

 #================================================[ Initialize environment ]===
 include("inc/includes.inc");
 $id = $_GET["id"];
 $title .= ": ".lang("pdf_export");

 #================================================[ Process submitted form ]===
 switch($_POST["submit"]) {
 #----------------------------------------------------------[ Export Dives ]---
   case lang("export_dives") :
     if ( empty($_POST["from"]) || empty($_POST["to"]) ) {
       $dives = $pdl->db->get_dives("","",FALSE,"id","ASC");
       if ( empty($_POST["from"]) ) $_POST["from"] = $dives[0]["dive#"];
       if ( empty($_POST["to"]) )   $_POST["to"]   = $dives[$pdl->db->dives-1]["dive#"];
     }
     $url = $pdl->link->slink("dive_pdf.php?nr=".$_POST["from"]."&lastnr=".$_POST["to"]."&duplex=".$_POST["duplex"]."&pdfwithfotos=".$_POST["foto"]);
     header("Location: $url");
     exit;
     break;
 }

 #===================================================[ Initialize template ]===
 include("inc/header.inc");
 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"pdf_export.tpl"));
 $t->set_block("template","formblock","form");

 #=============================================[ set up the navigation bar ]===
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("pdf_export");
 $pdl->tabs->parse();
 $arrowheight = "height='9'";

 #===============================[ set up the table header and field names ]===
 $t->set_var("ptitle",lang("pdf_export"));
 $t->set_var("fsel_name",lang("pdf_select_pages").":");
 $t->set_var("foutput_name",lang("pdf_output_type").":");
 $t->set_var("foutput_viewname",lang("pdf_output_view"));
 $t->set_var("foutput_innername",lang("pdf_output_duplexprint_inner"));
 $t->set_var("foutput_outername",lang("pdf_output_duplexprint_outer"));
 $t->set_var("foutput_sidename",lang("pdf_output_duplexprint_side"));
 $t->set_var("ffoto_name",lang("pdf_include_fotos").":");
 $t->set_var("fyes",lang("yes"));
 $t->set_var("fno",lang("no"));
 $t->set_var("formmethod","POST");
 $t->set_var("formtarget",$_SERVER["PHP_SELF"]);
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
 $t->set_var("output_bubble",lang("pdf_output_type_desc"));
 $t->set_var("output_view_bubble",lang("pdf_output_view_desc"));
 $t->set_var("output_innergutter_bubble",lang("pdf_output_duplex_innergutter_desc"));
 $t->set_var("output_outergutter_bubble",lang("pdf_output_duplex_outergutter_desc"));
 $t->set_var("output_sidegutter_bubble",lang("pdf_output_duplex_sidegutter_desc"));
 $t->set_var("foto_bubble",lang("pdf_include_fotos_desc"));
 $t->set_var("foto_yes_bubble",lang("pdf_include_fotos_yes"));
 $t->set_var("foto_no_bubble",lang("pdf_include_fotos_no"));

 #============================================================[ Dives Form ]===
 $t->set_var("formname","dives");
 $t->set_var("icon_src",$pdl->config->base_url."templates/aqua/images/tab_buddylist.gif");
 $t->set_var("icon_width","30");
 $t->set_var("icon_height","15");
 $t->set_var("icon_alt",lang("dives"));
 $t->set_var("segment_name",lang("dives"));
 $t->set_var("submit_value",lang("export_dives"));
 $t->parse("form","formblock");

 #===========================================================[ End of Page ]===
 $t->pparse("out","template");

 include("inc/footer.inc");
?>