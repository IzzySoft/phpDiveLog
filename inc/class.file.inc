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

   /** Get the ADL formated file prefix
    * @class file
    * @method mkADLprefix
    * @param integer dive# Dive# to retrieve the pix for
    * @param string type [dive|site]
    * @return string formated prefix
    */
   function mkADLprefix($id,$type) {
     return $type.sprintf("%05d",$id);
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
     $prefix = $this->mkADLprefix($id,$type)."-";
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

   function havePix($id,$type="dive") {
     GLOBAL $pdl;
     $dirname = $pdl->config->user_path."fotos/$type";
     if (!is_dir($dirname)) return FALSE;
     $prefix = $this->mkADLprefix($id,$type)."-";
     $plen   = strlen($prefix);
     $dir = dir($dirname);
     while ( $file=$dir->read() ) {
       if ( strpos($file,$prefix)===0 ) {
         $pos = strpos($file,".");
         $pic = substr($file,0,$pos);
         switch ($suffix = substr($file,$pos+1)) {
           case "jpg" :
           case "gif" :
           case "png" : return TRUE; break;
         }
       }
     }
     return FALSE;
   }

   /** Find profile pictures
    * @class file
    * @method getProfPic
    * @param integer dive# Dive# to retrieve the pic for
    * @return string filename (empty if none found)
    */
   function getProfPic($id) {
     GLOBAL $pdl;
     $filename = $pdl->config->user_path."images/".$this->mkADLprefix($id,"dive")."_profile.png";
     if ( file_exists($filename) ) return $filename;
     return "";
   }

   /** Read a user-defined config file
    * @class file
    * @method read_conf
    * @param string filename config file to read
    * @return array (var=value for "flat" files, block[var]=value with [blocks])
    */
   function read_conf($file) {
     $def = file($file);
     $lc  = count($def);
     for ($i=0;$i<$lc;++$i) {
       switch(substr(trim($def[$i]),0,1)) {
         case "#" :
         case ""  : break;
         case "[" : $var = trim($def[$i]);
                    $len = strlen($var)-2;
                    $block = substr($var,1,$len);
         default  : $pos = strpos($def[$i],"=");
                    if (!$pos) break; // invalid syntax in conf file
                    $var   = trim(substr($def[$i],0,$pos));
                    $value = trim(substr($def[$i],$pos+1));
                    if (empty($block)) {
                      if ( $pos= strpos($var,"[]") ) {
                        $var = substr($var,0,$pos);
                        $conf[$var][] = $value;
                      } else {
                        $conf[$var] = $value;
                      }
                    } else {
                      if ( $pos= strpos($var,"[]") ) {
                        $var = substr($var,0,$pos);
                        $conf[$block][$var][] = $value;
                      } else {
                        $conf[$block][$var] = $value;
                      }
                    }
       }
     }
     return $conf;
   }

 } // end class file

?>