<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Link related stuff: highlighting, linking targets etc.                    #
 #############################################################################

 /* $Id$ */

 /** Link related stuff: highlighting, linking targets etc.
  * @package Api
  * @class link
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2001-2004 by Itzchak Rehberg and IzzySoft
  */
 class link {

  /** Constructor: Initiate
   * @constructor link
   */
  function link() {
  }

  /** Provide a link with session info, if necessary
   * @class link
   * @method slink
   * @param string target url
   * @return string target url
   */
  function slink($target) {
#    GLOBAL $pdl;
    GLOBAL $lang, $template_set;
    if (empty($lang)) $lang = "en";
    $tpl = $_GET["tpl"]; if (empty($tpl)) $tpl = $template_set;
    $diver = $_GET["diver"];
    $params = "";
    $details = array("lang","tpl","diver");
    foreach ($details as $element) {
     if ( !(strpos($target,"$element=")) && !empty(${$element}) ) {
      if (empty($params)) {
        $params = "$element=${$element}";
      } else {
        $params .= "&$element=${$element}";
      }
     }
    }
#    if (!$pdl->config->enable_cookies) {
      if (empty($params)) return $target;
      $pos = strpos($target,"?");
      if ($pos) {
        $target .= "&$params";
      } else {
        $target .= "?$params";
      }
#    }
    return $target;
  }

  /** Create a complete HREF for an URL and description
   * @class link
   * @method linkurl
   * @param string target url
   * @param string desc url-desc
   * @param string opt options for the HREF tag
   * @return string target complete href tag
   */
  function linkurl($target,$desc,$opt="") {
    $url = "<A HREF='" .$this->slink($target). "'";
    if ($opt) $url .= " $opt";
    $url .= ">$desc</A>";
    return $url;
  }

 } // end class link

 $pdl->link = new link;
?>