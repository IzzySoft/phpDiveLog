<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2006 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Check for vulnerabilities and reject requests that look like intrusion    #
 #############################################################################

 /* $Id$ */

 #=========================================================[ helper funcs ]===
 function vul_kick() {
   $protocol = $_SERVER['SERVER_PROTOCOL'];
   header("$protocol 403 Forbidden");
   header("Status: 403 Forbidden");
   header("Connection: close");
   $pdl_vul = TRUE;
   $title = "Access Denied!";
   $pdl->config->tpl_url = "templates/aqua/";
   include("inc/header.inc");
   echo "<DIV STYLE='border: 1px solid black;background-image:url(templates/aqua/images/aqua-light.jpg)'><IMG SRC='templates/aqua/images/stop.gif' ALIGN='right'><IMG SRC='templates/aqua/images/stop.gif' ALIGN='left'><H2><FONT COLOR='#f00'>$title</FONT></H2><P ALIGN='justify'>Access to this page was denied due to wrong parameters. The intrusion detection of this application triggered an alert: Looks like you want to attack the system and place your own code here. We don't like that, so we aborted your request. If you think that's not your fault (i.e. you did not intentionally manipulate the URL string or POST data, but used the application in a \"normal way\" only), you may want to report this to the author(s) to fix the problem.</P></DIV></BODY></HTML>";
   exit;
 }
 function vul_num($var) {

   if (!empty($_GET[$var])) {
     if (!is_numeric($_GET[$var])) vul_kick(); }
   if (!empty($_REQUEST[$var])) {
     if (!is_numeric($_REQUEST[$var])) vul_kick(); }
 }
 function vul_alnum($var) {
   if (!empty($_GET[$var])) {
     if (preg_match("/[^\w\s-]/",$_GET[$var])) vul_kick(); }
   if (!empty($_REQUEST[$var])) {
     if (preg_match("/[^\w\s-]/",$_REQUEST[$var])) vul_kick(); }
 }

 #==========================================================[ common vars ]===
 if (!empty($_GET["lang"]) && strlen($_GET["lang"]) > 2) vul_kick();
 vul_alnum("diver");
 vul_alnum("tpl");
 if (!empty($_REQUEST["nr"])) vul_num("nr");
 if (!empty($_REQUEST["id"])) vul_num("id");
 vul_num("start");

 #============================================================[ site.php ]===
 vul_num("id");

 #==========================================================[ places.php ]===
 vul_alnum("place");

 #===========================================================[ prefs.php ]===
 vul_alnum("tab2");

 #========================================================[ sitelist.php ]===
# if (basename($_SERVER["SCRIPT_NAME"])=="sitelist.php") {
# }
?>