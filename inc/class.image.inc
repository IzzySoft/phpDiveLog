<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2017 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
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
  * @author Izzy (izzysoft AT qumran DOT org)
  * @copyright (c) 2004-2017 by Itzchak Rehberg and IzzySoft
  */
 class image {

  /** Constructor: Initiate
   * @constructor image
   */
  function image() {
  }

  /** Get the size of a given image
   * @class image
   * @method getSize
   * @param string filename
   * @return array size (x,y - 0,0 if the graphic functions not available)
   */
  function getSize($file) {
    if ( !function_exists("getimagesize") ) return array(0,0);
    return getimagesize($file);
  }

  /** Create the HREF for the popup
   * @class image
   * @method mkPopup
   * @param string path
   * @param string url
   * @param string filename
   * @return string href
   */
  function mkPopup($path,$url,$file) {
    $size = $this->getSize($path.$file);
    if ( $size[0] == 0 ) $size[0] = 640;
    if ( $size[1] == 0 ) $size[1] = 480;
    return "JavaScript:show_img('".$url.$file."',".$size[0].",".$size[1].")";
  }

 } // end class image

?>