<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
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
  * @author Izzy (izzysoft AT qumran DOT org)
  * @copyright (c) 2004-2007 by Itzchak Rehberg and IzzySoft
  */
 class link {

  /** Constructor: Initiate
   * @constructor link
   */
  function link() {
    $this->mapsite   = "http://www.mapquest.com/maps/map.adp";
    $this->mapparam  = "latlongtype=decimal";
    $this->coordtype = "decimal";
    $this->maplat    = "latitude";
    $this->maplong   = "longitude";
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
    $order = $GLOBALS["order"]; $sort = $GLOBALS["sort"];
    $params = "";
    $details = array("lang","tpl","diver","sort","order");
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

  /** Create link to map site
   * @class link
   * @method map
   * @param array latitude Latitude with details grad, minute, second
   * @param array longitude Longitude with details grad, minute, second
   * @return string url
   */
  function map($lat,$long) {
    if ($lat[0]+$lat[1]+$lat[2]==0) return "";
    if ($this->coordtype == "decimal") {
      $min  = $lat[1] + $lat[2]/60;
      $lati = ($lat[0] + $min/60) * $lat[4];
      $min  = $long[1] + $long[2]/60;
      $longi= ($long[0] + $min/60) * $long[4];
      $url = $this->mapsite."?";
      if (!empty($this->mapparam)) {
        $url .= $this->mapparam."&";
      }
      $url .= $this->maplat."=".$lati."&".$this->maplong."=".$longi;
      return $url;
    }
    return "";
  }

 } // end class link

 $pdl->link = new link;
?>