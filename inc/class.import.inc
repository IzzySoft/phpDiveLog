<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Import of (user transfered) divelog data                                  #
 #############################################################################

 /* $Id$ */

 require_once(dirname(__FILE__)."/class.db.inc");
 require_once(dirname(__FILE__)."/class.db_text.inc");

 /** Import PDL data to DB
  * @package Api
  * @class import
  * @author Izzy (izzysoft AT qumran DOT org)
  * @copyright (c) 2004-2009 by Itzchak Rehberg and IzzySoft
  */
 class import {

  /** Constructor: Initiate
   * @constructor import_db
   */
  function import() {
    GLOBAL $pdl;
    $this->db  = $pdl->db;
    $this->csv = new db_text();
  }

  /** Add a supported format to the list
   * @class import
   * @method addFormat
   * @param string name
   * @param string description
   * @return array (name,desc)
   */
  function addFormat($name,$desc) {
    $t->name = $name;
    $t->desc = $desc;
    return $t;
  }

  /** Import files from the transfer directory
   * @class import
   * @method import
   * @param string format of the divelog to import
   *  (must be a name from import::formats)
   * @return boolean success
   */
  function getData($format) {
    GLOBAL $pdl;
    switch ($format) {
      case "pdl" : return $pdl->file->copyDir($pdl->config->transfer_path.$pdl->config->user_name,$pdl->config->user_path); break;
      case "sdm" : return FALSE; break;
    }
  }

  /** Convert coordinates
   * @method mk_coord
   * @param string str (Coords as from ADL)
   * @return integer coord
   */
  function mk_coord($str) {
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

  /** Update translations
   */
  function translations_update($lang_id) {
    $trans = $this->csv->get_translations($lang_id);
    return $this->db->translations_update($lang_id,$trans);
  }

 } // end class import_db

?>