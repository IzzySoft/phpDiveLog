<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Image related stuff (get size, open in new window...)                     #
 #############################################################################

 /* $Id$ */

 /** Image related stuff (get size, open in new window...)
  * @package Api
  * @class image
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2001-2004 by Itzchak Rehberg and IzzySoft
  */
 class image {

  /** Constructor: Initiate
   * @constructor image
   */
  function link() {
  }

  /** Get the size of a given image
   * @class image
   * @method getSize
   * @param string filename
   * @return array size (x,y - 0,0 if the graphic functions not available)
   */
  function getSize($file) {
    if ( !function_exists(getimagesize) ) return array(0,0);
    return getimagesize($file);
  }

 } // end class image

 $pdl->image = new image;
?>