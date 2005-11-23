<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2005 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
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
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004-2005 by Itzchak Rehberg and IzzySoft
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


  /** Import data from CSV to DB
   * @method import2db
   * @return boolean success
   */
  function import2db() {
    GLOBAL $pdl;
    $this->db->begin();
    $userID = $this->db->userIDget();
    if ($this->db->log_drop($userID)) {
      // Sites
      $sites = $this->csv->get_sites();
      for ($i=0;$i<$this->csv->sites;++$i) {
        $sites[$i]["latitude"] = $this->mk_coord($sites[$i]["latitude"]);
        $sites[$i]["longitude"] = $this->mk_coord($sites[$i]["longitude"]);
        if (!$this->db->site_add($sites[$i])) {
          $this->db->rollback();
          $pdl->last_error = "site_import_failed";
          return FALSE;
        }
      }
      // Dives + Equipment
      $dives = $this->csv->get_dives();
      $dc = count($dives);
      for ($i=0;$i<$dc;++$i) {
        // suit
        $suit["id"]     = $dives[$i]["suit_id"];
        $suit["name"]   = addslashes($dives[$i]["suitname"]);
        $suit["stype"]  = addslashes($dives[$i]["suittype"]);
        $pos = strpos($dives[$i]["suitweight"],' ');
        $dives[$i]["suitweight"] = substr($dives[$i]["suitweight"],0,$pos)*2;
        $suit["weight"] = $dives[$i]["suitweight"];
        if (!$this->db->suit_add($suit)) {
          $this->db->rollback();
          $pdl->last_error = "suit_import_failed";
          return FALSE;
        }
        // tanks
        for ($k=0;$k<count($dives[$i]["tank"]);++$k) {
          $tank["id"]     = $dives[$i]["tank"][$k]->id;
          $tank["name"]   = $dives[$i]["tank"][$k]->name;
          $tank["volume"] = $dives[$i]["tank"][$k]->volume;
          $tank["ttype"]  = $dives[$i]["tank"][$k]->type;
          $pos = strpos($tank["volume"],' ');
          $tank["volume"] = substr($tank["volume"],0,$pos);
          if (!$this->db->tank_add($tank)) {
            $this->db->rollback();
            $pdl->last_error = "tank_import_failed";
            return FALSE;
          }
          $pos = strpos($dives[$i]["tank"][$k]->in,' ');
          $dives[$i]["tank"][$k]->in = substr($dives[$i]["tank"][$k]->in,0,$pos);
          $pos = strpos($dives[$i]["tank"][$k]->out,' ');
          $dives[$i]["tank"][$k]->out = substr($dives[$i]["tank"][$k]->out,0,$pos);
          $pos = strpos($dives[$i]["visibility"],' ');
          $dives[$i]["visibility"] = substr($dives[$i]["visibility"],0,$pos)*10;
          $pos = strpos($dives[$i]["watertemp"],' ');
          $dives[$i]["watertemp"] = substr($dives[$i]["watertemp"],0,$pos);
          $pos = strpos($dives[$i]["airtemp"],' ');
          $dives[$i]["airtemp"] = substr($dives[$i]["airtemp"],0,$pos);
          $dives[$i]["wcurrent"] = $dives[$i]["current"];
        }
        // dives
        $months = array ( 'January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December' );
        $datum = explode(' ',$dive["date"]); # 'd m y'
        $monat = array_search($datum[1],$months) +1;
        $stamp = mktime(0,0,0,$monat,$datum[0],$datum[2]);
        $dives[$i]["datum"] = date('Y-m-d',$stamp);
        $pos = strpos($dives[$i]["depth"],' ');
        $dives[$i]["depth"] = substr($dives[$i]["depth"],0,$pos)*10;
        $pos = strpos($dives[$i]["divetime"],' ');
        $dives[$i]["divetime"] = substr($dives[$i]["divetime"],0,$pos)*60;
        $pos = strpos($dives[$i]["weight"],' ');
        $dives[$i]["weight"] = substr($dives[$i]["weight"],0,$pos)*2;
        if (!$this->db->dive_add($dives[$i])) {
          $this->db->rollback();
          $pdl->last_error = "dive_import_failed";
          return FALSE;
        }
      }
    } else {
      $pdl->last_error = "log_drop_failed";
      $this->db->rollback();
      return FALSE;
    }
    $this->db->commit();
    // preferences
    $this->db->begin();
    if ( file_exists($pdl->config->base_path."diver/".$pdl->config->user_name."/public") )
      $ok = $this->db->user_pref_set("public","1");
    else $ok = $this->db->user_pref_set("public","0");
    if (!$ok) {
      $pdl->last_error = "pref_update_failed";
      $this->db->rollback();
    }
    $this->db->commit();
    return TRUE;
  }

  /** Update translations
   */
  function translations_update($lang_id) {
    $trans = $this->csv->get_translations($lang_id);
    return $this->db->translations_update($lang_id,$trans);
  }

 } // end class import_db

# $pdl->import = new import;
?>