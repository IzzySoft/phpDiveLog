<?php
 #############################################################################
 # phpApi                                        (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # File handling methods                                                     #
 #############################################################################

 /* $Id$ */

 /** File handling methods
  * @package Api
  * @class auth
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class file {

   /** Initialize
    * @constructor file
    */
   function file() {
   }

   /** Find dive pictures
    * @class file
    * @method getDivePix
    * @param integer dive# Dive# to retrieve the pix for
    * @return array [0..n] of object pix (properties: url,desc)
    */
   function getDivePix($id) {
     return $this->getPix($id,"dive");
   }

   /** Find site pictures
    * @class file
    * @method getSitePix
    * @param integer site# Site# to retrieve the pix for
    * @return array [0..n] of object pix (properties: url,desc)
    */
   function getSitePix($id) {
     return $this->getPix($id,"site");
   }

   function getPix($id,$type="dive") {
     GLOBAL $pdl;
     $dirname = $pdl->config->user_path."fotos/$type";
     if (!is_dir($dirname)) return FALSE;
     $prefix = $type.sprintf("%05d",$id)."-";
     $plen   = strlen($prefix);
     $dir = dir($dirname);
     while ( $file=$dir->read() ) {
       if ( strpos($file,$prefix)===0 ) {
         $pos = strpos($file,".");
         $pic = substr($file,0,$pos);
         switch ($suffix = substr($file,$pos+1)) {
           case "jpg" :
           case "gif" :
           case "png" :
             $list[$pic]->url = $pdl->config->user_url."fotos/$type/$file"; break;
           case "txt" :
             $text = file($pdl->config->user_path."fotos/$type/$file");
             $tl   = count($text);
             for ($i=0;$i<$tl;++$i) {
               $list[$pic]->desc .= $text[$i];
             }
             break;
         }
       }
     }
     if (count($list)>0 ) foreach ($list as $foto) {
       $result[] = $foto;
     }
     return $result;
   }

 } // end class file

?>