<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Import of (user transfered) divelog data                                  #
 #############################################################################

 /* $Id$ */

 /** Import of (user transfered) divelog data
  * @package Api
  * @class import
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2001-2004 by Itzchak Rehberg and IzzySoft
  */
 class import {

  /** Constructor: Initiate
   * @constructor import
   */
  function import() {
    $this->formats[] = $this->addFormat("pdl","phpDiveLog");
#    $this->formats[] = $this->addFormat("sdm","Suunto DiveManager");
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

 } // end class import

 $pdl->import = new import;
?>