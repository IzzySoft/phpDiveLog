<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Display a single dive record                                              #
 #############################################################################

 # $Id$

#================================================[ Initialize environment ]===
include("inc/includes.inc");

#=============================[ Check whether we have the Cache available ]===
$cache_dir = $pdl->config->base_path."cache";
if (!is_dir($cache_dir) || !is_writeable($cache_dir)) {
  include("inc/header.inc");
  if (!is_dir($cache_dir)) $pdl->common->alert(lang("no_cache_dir"));
  else $pdl->common->alert(lang("cache_dir_readonly"));
  include("inc/footer.inc");
  exit;
}

#=======================================[ find out which page URL we need ]===
$project_site = "http://projects.izzysoft.de/";
$wiki_url = "${project_site}trac/phpdivelog/wiki/UserManual/";
switch($_REQUEST["topic"]) {
  case "buddylist" :
    $url  = "${wiki_url}GlobalMode/Divers";
    $name = "buddylist";
    $tactive = "diver";
    break;
  case "dive" :
    $url  = "${wiki_url}LogBook/DiveDetails";
    $name = "dives";
    break;
  case "divelist" :
    $url  = "${wiki_url}LogBook/DiveList";
    $name = "divelist";
    $tactive = "dives";
    break;
  case "globalsites":
    $url  = "${wiki_url}GlobalMode/Sites";
    $name = "globalsites";
    $tactive = "sites";
    break;
  case "pdf" :
    $url  = "${wiki_url}PdfExport";
    $name = "pdf_export";
    break;
  case "prefs"    :
    $url  = "${wiki_url}Preferences";
    $name = "preferences";
    $tactive = "prefs";
    break;
  case "site" :
    $url  = "${wiki_url}Sites/SiteDetails";
    $name = "sites";
    break;
  case "sitelist" :
    $url  = "${wiki_url}Sites/SiteList";
    $name = "sitelist";
    $tactive = "sites";
    break;
  case "stats":
    $url  = "${wiki_url}Statistics";
    $name = "statistics";
    $tactive = "stats";
    break;
}
if (empty($tactive)) $tactive = $name;

#==========================================[ check if the cache is expired ]===
if (!file_exists("$cache_dir/.version")) file_put_contents("$cache_dir/.version",$pdl->config->version);
preg_match('!^([0-9\.]+)!',file_get_contents("$cache_dir/.version"),$match);
$cache_version = $match[1];
preg_match('!^([0-9\.]+)!',$pdl->config->version,$match);
$prog_version = $match[1];
if ($cache_version != $prog_version) { // cache expired: new PDL version
  $dir = dir($cache_dir);
  while (false !== $entry = $dir->read()) {
    if (substr($entry,0,1)!=".") unlink("$cache_dir/$entry");
  }
  file_put_contents("$cache_dir/.version",$pdl->config->version);
}

#===========================================[ Get user preferred languages ]===
$langsupp = array("en","de","nl"); // what we provide in the wiki
$langs = explode(",",$_SERVER["HTTP_ACCEPT_LANGUAGE"]); // what the user wants
$lc = count($langs);
for ($i=0;$i<$lc;++$i) {
  $ext = substr($langs[$i],0,2);
  if (in_array($ext,$langsupp)) {
    $nego = $ext;
    break;
  }
}
if (empty($nego)) $nego = "en"; // fallback

#======================================================[ Get the help file ]===
$file = "$cache_dir/${name}.$nego";
if (!empty($url) && !file_exists($file)) {
  $pos = strpos($_SERVER["SERVER_PROTOCOL"],"/");
  $me = strtolower(substr($_SERVER["SERVER_PROTOCOL"],0,$pos))."://".$_SERVER["SERVER_NAME"].$_SERVER["PHP_SELF"];
  $opts = array('http' =>
      array(
          'method'  => 'GET',
          'header'  => "Accept-Language: ".$_SERVER["HTTP_ACCEPT_LANGUAGE"]."\r\nAccept-Charset: utf-8\r\nReferer: $me\r\n",
          'user_agent' => $_SERVER["HTTP_USER_AGENT"]
      )
  );
  $context  = stream_context_create($opts);
  $html = file_get_contents($url, false, $context);
  $html = preg_replace('!(<head>)!i','$1'."\n".'<meta http-equiv="Content-Type" content="text/html; charset=utf-8">',$html);
  file_put_contents($file,$html);
}

#============================================[ extract the content we need ]===
$doc = new DOMDocument();
$doc->loadHTMLFile($file);
$wiki = $doc->getElementById("content");
$wiki_tmp = new DOMDocument();
$wiki_tmp->appendChild($wiki_tmp->importNode($wiki,true));
$wikitext = $wiki_tmp->saveHTML();
unset($doc,$wiki,$wiki_tmp);

# Kick off the breadcrumbs
$wikitext = preg_replace('!\s*<p class="path">(.*?)</p>\s*!ims','',$wikitext);
# Kick off the TOC
$wikitext = preg_replace('!\s*<div class="wiki-toc">.*?</div>!ims','',$wikitext);
# Kick off LastModified
$wikitext = preg_replace('!\s*<div class="lastmodified">.*?</div>!ims','',$wikitext);
# Links open in new window/tab
$wikitext = preg_replace('!(<img.*?src=")/!i','$1'.$project_site,$wikitext);
$wikitext = preg_replace('!(href=")/!i','$1'.$project_site,$wikitext);
$wikitext = preg_replace('!\s+(href=)!i',' target="izzysoft" $1',$wikitext);
# CleanUp
$wikitext = preg_replace('!^\s*$!ms','',$wikitext);
$wikitext = preg_replace('!<p>\s*</p>!ms','',$wikitext);

#============================================================[ Output wiki ]===
if (isset($name)) $title = "Help: ".lang($name);
else $title = "Help";
include("inc/header.inc");
#-------------------------------------------------[ set up navigation tabs ]---
require_once("inc/class.tabs.inc");
$t = new Template($pdl->config->tpl_path);
$t->set_var("tpl_dir",$pdl->config->tpl_url);
$t->set_file(array("template"=>"help.tpl"));
if ( in_array($_REQUEST["topic"],array("buddylist","globalsites")) ) {
  $pdl->tabs = new tabs("globalhelp");
  $pdl->tabs->add_tab("diver",$_SERVER["PHP_SELF"]."?topic=buddylist","tab_buddylist.gif");
  $pdl->tabs->add_tab("sites",$_SERVER["PHP_SELF"]."?topic=globalsites","tab_sites.gif");
  $pdl->tabs->add_tab("prefs",$_SERVER["PHP_SELF"]."?topic=prefs","tab_preferences.gif");
} else {
  $pdl->tabs = new tabs("loghelp");
  $pdl->tabs->add_tab("dives",$_SERVER["PHP_SELF"]."?topic=dive","tab_dives.gif");
  $pdl->tabs->add_tab("stats",$_SERVER["PHP_SELF"]."?topic=stats","tab_stats.gif");
  $pdl->tabs->add_tab("sites",$_SERVER["PHP_SELF"]."?topic=sitelist","tab_sites.gif");
  $pdl->tabs->add_tab("prefs",$_SERVER["PHP_SELF"]."?topic=prefs","tab_preferences.gif");
  $pdl->tabs->add_tab("pdf_export",$_SERVER["PHP_SELF"]."?topic=pdf","apdf.png");
}
$t->set_block("template","tabblock","tab");
$pdl->tabs->activate($tactive);
$pdl->tabs->parse();
$t->pparse("out","template");

echo "<BR>$wikitext";
include("inc/footer.inc");
?>