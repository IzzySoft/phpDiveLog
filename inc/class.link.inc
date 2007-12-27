<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
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
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2001-2004 by Itzchak Rehberg and IzzySoft
  */
 class link {

  /** Constructor: Initiate
   * @constructor link
   */
  function link() {
    $this->mapurl["Mapquest"] = "http://www.mapquest.com/maps/map.adp?latlongtype=decimal&latitude={lat}&longitude={long}";
    $this->mapurl["Google"]   = "http://maps.google.com/?ll={lat},{long}&q={lat},{long}+({desc})&t=h";
    $this->mapurl["Yahoo"]    = "http://maps.yahoo.com/#mvt=h&lat={lat}&lon={long}";
    $this->mapsite            = $GLOBALS["mapsite"];
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

  /** Convert coordinates to decimal format
   * @method coord2dec
   * @param string coords in Grad, Minutes, Seconds (x° y' z.a") format'
   * @return string coords in decimal format
   */
  function coord2dec($str) {
    if ( preg_match("/([0-9]+)[^0-9]+([0-9]+)[^0-9.]+([0-9.]+)[^0-9]+/",$str,$match) ) {
      $code[0] = $match[1]; $code[1] = $match[2]; $code[2] = $match[3];
    } elseif ( preg_match("/([0-9]+)[^0-9]+([0-9.]+)[^0-9.]+/",$str,$match) ) {
      $code[0] = $match[1]; $code[1] = floor($match[2]);
      $code[2] = ($match[2] - floor($match[2])) * 60;
    } elseif ( preg_match("/([0-9.]+)[^0-9.]+/",$str,$match) ) {
      $code[0] = floor($match[1]);
      $code[1] = floor( ($match[1] - $code[0]) * 60);
      $code[2] = floor( (($match[1] - $code[0]) * 60 - $code[1]) * 60 );
    }
    if ( substr($str,strlen($str)-1)=="N" || substr($str,strlen($str)-1)=="E" ) {
      $code[4] = 1;
    } else {
      $code[4] = -1;
    }
    $min  = $code[1] + $code[2]/60;
    $coord = ($code[0] + $min/60) * $code[4];
    return $coord;
  }

  /** Create link to map site
   * @class link
   * @method map
   * @param string latitude Latitude with details grad, minute, second
   * @param string longitude Longitude with details grad, minute, second
   * @param optional string desc Description of the point to map to (default: empty)
   * @return string url
   */
  function map($lati,$longi,$desc="") {
    $lat  = $this->coord2dec($lati);
    if (empty($lat)) return "";
    $long = $this->coord2dec($longi);
    if ($desc!="") $desc = "+(".urlencode($desc).")";
    $url  = str_replace(array("{lat}","{long}","+({desc})"),array($lat,$long,$desc),$this->mapurl[$this->mapsite]);
    return($url);
  }

  /** Evaluate the base URL of PDL
   * @method get_baseurl
   * @return string url
   */
  function get_baseurl() {
    $pdl_url = strtolower($_SERVER["SERVER_PROTOCOL"]);
    $pos = strpos ($pdl_url,"/");
    $pdl_url = substr($pdl_url,0,$pos)."://".$_SERVER["HTTP_HOST"];
    if ($_SERVER["SERVER_PORT"]!=80) $pdl_url .= ":".$_SERVER["SERVER_PORT"];
    $pdl_url .= dirname($_SERVER["PHP_SELF"]);
    return $pdl_url;
  }

 } // end class link

 $pdl->link = new link;
?>