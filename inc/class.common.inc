<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Commonly used methods (alerts etc.)                                       #
 #############################################################################

 /* $Id$ */

 /** Commonly used methods (alerts etc.)
  * @package Api
  * @class common
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class common {

  /** Constructor: Initiate
   * @constructor common
   */
  function common() {
  }

  /** Build up alert page with a given message
   * @class common
   * @method alert
   * @param string alert message
   */
  function alert($message) {
    GLOBAL $pdl;
    $t = new Template($pdl->config->tpl_path);
    $t->set_file(array("template"=>"alert.tpl"));
    $t->set_var("tpl_dir",$pdl->config->tpl_url);
    $t->set_var("message",$message);
    $t->pparse("out","template");
  }

 } // end class common

 $pdl->common = new common;
?>