<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Generating Logbook Page for a single dive                                 #
 #############################################################################
 # $Id$

#=============================================[ Initialize & Setup PDF Api ]===
require_once(dirname(__FILE__)."/inc/includes.inc");
#------------------------------------------[ DiveRecord specific constants ]---
define('MULTI_PAGE',FALSE); // just a single page or the entire book?
if (isset($_REQUEST["pageno"])) $pagenr = $_REQUEST["pageno"];
elseif ($pdf_pageno_from_diveno) $pagenr = $_REQUEST["nr"];
else $pagenr = 1;
if ($pagenr%2) define ('PAGE_GUTTER_LEFT',1);
else define ('PAGE_GUTTER_LEFT',0);

#------------------------------------------------[ create new PDF document ]---
require_once(dirname(__FILE__)."/inc/pdf_init.inc");
$pdf = new pdlPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false); 

#---------------------------------------------------------[ URL parameters ]---
$nr = $pdl->params->nr;
$site = $pdl->db->get_site($nr);
$title .= ": Site# $nr";

#------------------------------------------------------[ Setup Page Design ]---
if (MULTI_PAGE) {
  $pdf->SetHeaderData(PDF_HEADER_LOGO, PDF_HEADER_LOGO_WIDTH, PDF_HEADER_TITLE, PDF_HEADER_STRING);
  $pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
} else {
  $pdf->setPrintHeader(false);
  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP_NOHEAD, PDF_MARGIN_RIGHT);
}
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);
$pdf->SetAutoPageBreak(FALSE, PDF_MARGIN_BOTTOM);
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO); 
$pdf->setLanguageArray($l); 
#$pdf->SetFont('helvetica', 'BI', 16);
$pdf->SetFont('freesans', '', 10); // provides better UTF8 capabilities

#----------------------------------------------------------[ Retrieve Data ]---
$dive = $pdl->db->get_dive($nr);

#-----------------------------------------------[ Set Document Information ]---
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor('Itzchak Rehberg');
$pdf->SetTitle($title);
$pdf->SetSubject($title);
$pdf->SetKeywords($dive["location"].', '.$dive["place"].', '.$pdl->params->diver.', dive');

#=======================================================[ Prepare the Page ]===
#---------------------------------------------------------[ Template Setup ]---
$t = new Template($pdl->config->base_path."templates/aqua/");
$t->set_file(array("template"=>"site_pdf.tpl"));
$t->set_block("template","sumblock","sum");
$t->set_block("template","fotoblock","fotos");
$t->set_block("template","nofotoblock","nofotos");
$t->set_block("template","topleftblock","topleft");
$t->set_block("template","toprightblock","topright");

#-----------------------------------------------------------[ Setup Header ]---
#$t->set_var("divenr",lang("dive#"));
$t->set_var("id",$nr);
$t->set_var("location",$site["loc"]);
$t->set_var("place",$site["place"]);
if (PAGE_GUTTER_LEFT) {
  $t->parse("topleft","topleftblock");
  $t->set_var("topright","");
} else {
  $t->parse("topright","toprightblock");
  $t->set_var("topleft","");
}

#----------------------------------------------------------[ Setup Summary ]---
$t->set_var("status",lang("status"));
$t->set_var("item_name",lang("latitude").":");
$t->set_var("item_data",$site["latitude"]);
$t->parse("sum","sumblock");
$t->set_var("item_name",lang("longitude").":");
$t->set_var("item_data",$site["longitude"]);
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("altitude").":");
$t->set_var("item_data",$site["altitude"]." m");
$t->parse("sum","sumblock",TRUE);
$t->set_var("item_name",lang("max_depth").":");
$t->set_var("item_data",$site["depth"]);
$t->parse("sum","sumblock",TRUE);

#------------------------------------------------------------[ Setup Notes ]---
$t->set_var("notes_name",lang("notes"));
$notes[1] = $pdl->common->nl2br($site["description"],1,1);
$notes[2] = $pdl->file->getNotes($nr,"site");
$notb = $notes[1].$pdl->common->nl2br($notes[2]);
$pdf->notesSubset($notb); // Select the subset defined for PDF export
$pdf->htmlAdjust($notb); // Some fixup is required for TCPDF - e.g. make sure to use XHTML
$pdf->notesRestrictLen($notb); // we need to restrict text length so it hopefully fits in
$t->set_var("notes_text",$notb);

#------------------------------------------------------------[ Setup Fotos ]---
$t->set_var("fotos_name",lang("fotos"));
$fotos = $pdl->file->getSitePix($nr);
$fc = count($fotos);
if ($fc>0 && PDF_WITH_FOTOS) {
  for ($i=0;$i<$fc;++$i) {
    if ($i>2) break;
    $t->set_var("foto",$fotos[$i]->url);
    $t->set_var("foto_text",$fotos[$i]->desc);
    $t->parse("fotos","fotoblock",$i);
  }
  $t->set_var("nofotos","");
} else {
  $t->parse("nofotos","nofotoblock");
  $t->set_var("fotos","");
}

#=======================================================[ Generate Content ]===
$out = $t->parse("out","template");
#echo($out);
#echo "<pre>";die(htmlentities($out));
#exit;
$out = preg_replace('|\s+|',' ',$out); // TCPDF treats all spaces as hard spaces
$out = preg_replace('|\s+<T|i','<T',$out);// TCPDF converts each table cell into a separate "box"
$out = preg_replace('|\s+</T|i','</T',$out);// TCPDF converts each table cell into a separate "box"

$pdf->AddPage();
$pdf->writeHTML($out,true,0,true,0);
$pdf->Output("site_${nr}.pdf", 'I');

?>