<?php
 #############################################################################
 # phpApi                                        (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Translation handling methods                                              #
 #############################################################################

 /* $Id$ */

 /** Translation handling methods
  * @package Api
  * @class trans
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class trans {

   /** Initialize
    * @constructor trans
    */
   function trans($lang_id) {
     GLOBAL $pdl;
     if ( empty($lang_id) ) $lang_id = "en";
     $this->translations = $pdl->db->get_translations($lang_id);
   }

   /** Get list of available language files
    * @class trans
    * @method favail
    * @return array lang_keys ( n=&gt;key )
    */
   function favail() {
     GLOBAL $pdl;
     $dirname = $pdl->config->base_path."lang";
     $prefix  = "trans";
     $dir = dir($dirname);
     while ( $file=$dir->read() ) {
       if ( strpos($file,$prefix)===0 ) {
         $pos    = strpos($file,".");
         $lang[] = substr($file,$pos+1);
       }
     }
     return $lang;
   }

   /** Get list of available languages
    * @class trans
    * @method avail
    * @return array lang_keys ( n=&gt;key )
    */
   function avail() {
     return $this->favail();
   }

   /** get translate a given reference
    *  if no translation is found, the content of the passed param $key is
    *  returned instead
    * @class trans
    * @method trans
    * @param string key translation key
    * @param optional array m1 replace placeholder (%1..%10 in translations)
    * @param optional string m2 instead of $m1 being an array, all 10 replacements may be passed separately as strings
    */
   function transl($key,$m1="",$m2="",$m3="",$m4="",$m5="",$m6="",$m7="",$m8="",$m9="",$m10="") {
     if (is_array($m1)){
       $vars = $m1;
     } else {
       $vars = array($m1,$m2,$m3,$m4,$m5,$m6,$m7,$m8,$m9,$m10);
     }
     return $this->translate("$key",$vars);
   }

   /** build the translations (helper func to method trans)
    * @class trans
    * @method translate
    * @param string key translation key
    * @param optional array vars array of replacement strings (see function lang)
    */
   function translate($key,$vars=FALSE) {
     $trans = $this->translations[strtolower($key)];
     if (!$trans) $trans = $key;
     if (!$vars) $vars=array();
     $ndx = 1;
     while ( list($k,$v)=each($vars) ) {
       $trans = preg_replace("/%$ndx/",$v,$trans);
       $ndx++;
     }
     return $trans;
   }

 } // end class trans

?>