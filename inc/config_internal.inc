<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Internal configuration - do not modify except you know what you do!       #
 #############################################################################

 /* $Id$ */

 $pdl->config->version = "0.4.1+svn";
 $pdl->config->site_title = $title;

 #=============================================================[ Debugging ]===
 error_reporting(0); // initially turn all off
 function set_phpdebug($level) {
   $pdbug = error_reporting();
   error_reporting($pdbug | $level);
 }
 if ( strpos(strtoupper($debug_level),"A")!==false ) set_phpdebug(E_LL);
 else {
   if ( strpos(strtoupper($debug_level),"E")!==false ) set_phpdebug(E_ERROR);
   if ( strpos(strtoupper($debug_level),"W")!==false ) set_phpdebug(E_WARNING);
   if ( strpos(strtoupper($debug_level),"N")!==false ) set_phpdebug(E_NOTICE);
 }
 if ($debug_show==0) ini_set('display_errors',0);
 else ini_set('display_errors',1);
 if ($debug_log==0) ini_set('log_errors',0);
 else ini_set('log_errors',1);

 #===========================================================[ Diver Setup ]===
 $pdl->config->enable_index = $enable_index;
 if (!empty($_REQUEST["diver"])) $pdl->params->diver = $_REQUEST["diver"];
 elseif (strtolower($default_page) == "user") $pdl->params->diver = $default_user;
 elseif (strtolower($default_page) == "index") {
   if ( basename($_SERVER["SCRIPT_FILENAME"])!= "buddylist.php" ) {
     header("Location: buddylist.php");
     exit;
   }
 }
 $relPath = "diver/".$pdl->params->diver."/";
 $pdl->config->user_name = $pdl->params->diver;

 #======================================[ Process remaining URL parameters ]===
 function set_param($req,$default) {
   GLOBAL $pdl;
   if (isset($_REQUEST[$req])) $pdl->params->$req = $_REQUEST[$req];
   else $pdl->params->$req = $default;
 }
 set_param("start",0);
 if (!isset($listtype)) $listtype = "";
 switch ($listtype) {
   case "logbook" : set_param("order",$logbook_default_order);
                    set_param("sort",$logbook_default_sort);
		    break;
   case "sitelist": set_param("order",$sitelist_default_order);
                    set_param("sort",$sitelist_default_sort);
		    break;
   default        : set_param("order","asc");
                    set_param("sort","");
		    break;
 }
 set_param("nr",0);

 include("inc/class.file.inc");
 $pdl->file = new file();

 #==========================================================[ directories ]===
 $pdl->config->base_path = dirname(dirname(__FILE__));
 if ($pdl->config->base_path!="/") $pdl->config->base_path .= "/";
 $pdl->config->base_url  = dirname($_SERVER["PHP_SELF"]);
 if ($pdl->config->base_url!="/") $pdl->config->base_url .= "/";
# $pdl->config->base_path = "";
# $pdl->config->base_url  = "";
 $pdl->config->user_path = $pdl->config->base_path.$relPath;
 $pdl->config->user_url  = $pdl->config->base_url.$relPath;
 $pdl->config->pwdfile   = $pwdfile;
 $pdl->config->transfer_path = $transfer_dir;
 #----------------------------------------------------------------[ TCPDF ]---
 if ($pdf_enable) {
   if (strrpos($tcpdf_path,'/')!=strlen($tcpdf_path)) $tcpdf_path .= "/";
   if (file_exists($tcpdf_path."tcpdf.php")) define ('K_PATH_MAIN',$tcpdf_path);
   elseif (file_exists("/usr/share/tcpdf/tcpdf.php")) define ('K_PATH_MAIN',"/usr/share/tcpdf/tcpdf.php");
   elseif (file_exists("/usr/local/share/tcpdf/tcpdf.php")) define ('K_PATH_MAIN',"/usr/local/share/tcpdf/tcpdf.php");
   else define ('K_PATH_MAIN','');
 }
 else define ('K_PATH_MAIN','');

 #===========================================================[ users conf ]===
 if ( file_exists($pdl->config->user_path."system.conf") ) {
   $uconf = $pdl->file->read_conf($pdl->config->user_path."system.conf");
   if (strtolower($override_protection)=="implicit") {
     $allowed = explode(" ",$allow_override);
     $ac = count($allowed);
     for ($i=0;$i<$ac;++$i) {
       if ( !empty($uconf[$allowed[$i]]) ) 
         ${$allowed[$i]} = $uconf[$allowed[$i]];
     }
   } else {
     $prohib = explode(" ",$protected_options);
     foreach ($uconf as $var=>$val) {
       if ( !in_array($var,$prohib) ) ${$var} = $val;
     }
   }

   if ($uconf["personal"]) $pdl->config->display_personal = TRUE;
 }
 $pdl->config->user_pwd = $pdl->file->getPwd();

 #==================================================[ URL config overrides ]===
 function request_value($varname,$regexp,$config,$default,$neg=FALSE) {
   if ( $neg ) {
     if ( preg_match($regexp,$_REQUEST[$varname]) ) return $_REQUEST[$varname];
     elseif ( preg_match($regexp,$config) ) return $config;
     else return $default;
   }
   elseif (!preg_match($regexp,$_REQUEST[$varname])) return $_REQUEST[$varname];
   if ( empty($config) && $config!==0 ) return $default;
   return $config;
 }
 define("PIX_PER_ROW",request_value("pixinrow",'|[^\d]|',$pix_per_row,3));
 define("DISPLAY_LIMIT",request_value("displaylimit",'|[^\d]|',$display_limit,12));
 define("TEMPLATE_SET",request_value("tpl",'|[^A-Za-z0-9_]|',$template_set,"aqua"));
 define("LANG",request_value("lang",'|^[a-z]{2}$|',$lang,"en",TRUE));
 define("DEPTH_STAT_INTERVAL",request_value("depthstatinterval",'/^(5|10|20)$/',$depth_stat_interval,10,TRUE));
 define("TEMP_STAT_INTERVAL",request_value("tempstatinterval",'/^(5|10)$/',$temp_stat_interval,10,TRUE));
 define("TIME_STAT_INTERVAL",request_value("timestatinterval",'/^(10|20|30)$/',$time_stat_interval,10,TRUE));
 define("PDF_WITH_FOTOS",request_value("pdfwithfotos",'/^(0|1)$/',$pdf_with_fotos,TRUE,TRUE));
 define("USE_DYN_PROFILE_PNG",request_value("usedynprofilepng",'/^(0|1)$/',$use_dyn_profile_png,TRUE,TRUE));
 define("SCHEDULE_GRAPH",request_value("schedulegraph",'/^(integrated|separate|none)$/i',$schedule_graph,"integrated",TRUE));
 define("PDF_CREATE_MISSING_GRAPH",request_value("pdfcreatemissinggraph",'/^(0|1)$/',$pdf_create_missing_graph,TRUE,TRUE));

 #==========================================================[ preferences ]===
 $pdl->config->display_limit = DISPLAY_LIMIT;
 $lang = LANG;
 $pdl->config->tpl_path  = $pdl->config->base_path."templates/".TEMPLATE_SET."/";
 $pdl->config->tpl_url   = $pdl->config->base_url."templates/".TEMPLATE_SET."/";
 $pdl->config->values_in_statgraphs = strtolower($values_in_statgraphs);
 $pdl->config->global_kml = $global_kml;

 #=============================================================[ database ]===
 $pdl->config->db_type  = $database["type"];
 $pdl->config->datadir  = $pdl->config->user_path."data/";
 $pdl->config->divedata = $pdl->config->datadir."logbook.csv";
 $pdl->config->sites    = $pdl->config->datadir."divesites.csv";
 $pdl->config->database = $database;

 #======================================================[ default sorting ]===
 $sa = array("logbook_default_sort","logbook_default_order",
             "sitelist_default_sort","sitelist_default_order");
 foreach ($sa as $item) {
   $pdl->config->$item = ${$item};
   unset($item);
 }

 #================================================================[ Icons ]===
 $pdl->config->icons["pdf"]      = $pdl->config->base_url."templates/aqua/images/apdf.png";
 $pdl->config->icons["buddy"]    = $pdl->config->base_url."templates/aqua/images/buddy.png";
 $pdl->config->icons["location"] = $pdl->config->base_url."templates/aqua/images/fingerpost.gif";
 $pdl->config->icons["place"]    = $pdl->config->base_url."templates/aqua/images/anchor.png";

 #=============================================================[ clean up ]===
 unset($database,$display_limit,$pix_per_row,$allow_override,$enable_index,
       $uconf,$allowed,$ac,$pwdfile,$transfer_dir,$values_in_statgraphs,
       $global_kml);
?>
