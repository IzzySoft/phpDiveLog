<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Dive Statistics                                                           #
 #############################################################################

 # $Id$

 $helppage = "prefs";
 include("inc/includes.inc");
 $title .= ": ".lang("preferences");
 if ( !empty($_GET["tab2"]) ) {
   $tab2 = TRUE;
   $urladd = "&tab2=1";
 } else {
   $tab2 = FALSE;
   $urladd = "";
 }
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"prefs.tpl"));
 $t->set_block("template","langblock","lang");
 $t->set_block("template","tplblock","tpl");

 #================================================[ set up navigation tabs ]===
 if ($tab2) {
   include("inc/tab_setup2.inc");
 } else {
   include("inc/tab_setup.inc");
 }
 $pdl->tabs->activate("prefs",TRUE);
 $pdl->tabs->parse();

 #==================================================[ Set up table headers ]===
 $t->set_var("title",lang("preferences"));
 $t->set_var("lang_title",lang("languages"));
 $t->set_var("tpl_title",lang("tpl_sets"));

 #=================================================[ Set up language block ]===
 $lavail = $pdl->trans->avail;
 $lc     = count($lavail);
 for ($i=0;$i<$lc;++$i) {
   $t->set_var("lang_ref",$pdl->link->slink($_SERVER["SCRIPT_NAME"]."?lang=".$lavail[$i].$urladd));
   $imgfile = "images/lang_".$lavail[$i].".jpg";
   if ( file_exists($pdl->config->tpl_path.$imgfile ) )
     $img = "<img src='".$pdl->config->tpl_url.$imgfile."' width='30' height='20'>";
     else $img = strtoupper($lavail[$i]);
   $t->set_var("lang_img",$img);
   $t->set_var("lang_name",lang("lang_".$lavail[$i]));
   $t->parse("lang","langblock",$i);
 }

 #=================================================[ Set up template block ]===
 $dirname = $pdl->config->base_path."templates";
 $dir = dir($dirname);
 $exclude = array(".","..","CVS");
 while ( $file=$dir->read() ) {
   if (!in_array($file,$exclude) ) {
     if ( is_dir("$dirname/$file") ) $tset[] = $file;
   }
 }
 $tc = count($tset);
 for ($i=0;$i<$tc;++$i) {
   $t->set_var("tpl_ref",$pdl->link->slink($_SERVER["SCRIPT_NAME"]."?tpl=".$tset[$i].$urladd));
   $imgfile = "images/tpl_".$tset[$i].".gif";
   if ( file_exists($pdl->config->tpl_path.$imgfile ) )
     $img = "<img src='".$pdl->config->tpl_url.$imgfile."'>";
     else $img ="";
   $t->set_var("tpl_img",$img);
   $t->set_var("tpl_name",ucfirst($tset[$i]));
   $t->parse("tpl","tplblock",$i);
 }

 $t->pparse("out","template");

 include("inc/footer.inc");
?>