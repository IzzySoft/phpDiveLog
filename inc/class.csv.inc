<?php
 #############################################################################
 # phpApi                                        (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # CSV File handling (import)                                                #
 #############################################################################

 /* $Id$ */

 /** CSV file handling
  * @package Api
  * @class auth
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class csv {

   /** Initialize
    * @constructor csv
    * @param optional string separator (default: ';')
    * @param optional string textmarker
    *   (by what characters are textfields enclosed; default: '"')
    */
   function csv($sep=";",$txt='"',$trim=FALSE) {
     $this->sep  = $sep;
     $this->txt  = $txt;
     $this->trim = $trim;
   }

   /** Import CSV file
    * @class csv
    * @method import
    * @param string filename file to import
    */
   function import($filename) {
     $this->read_file($filename);
     $pos = strpos($this->text,"\n");
     $line = trim(substr($this->text,0,$pos));
     $this->text = substr($this->text,$pos+1);
     $this->read_fields($line);
     $this->read_data();
   }

   /** Get the field names
    *  Reads the field names from the specification line and sets up the
    *  $this->field array
    * @class csv
    * @method read_fields
    * @param string line field specification line from the CSV file to import
    */
   function read_fields($line) {
     $fields = explode($this->sep,$line);
     $fc = count($fields);
     for ($i=0;$i<$fc;++$i) {
       if (substr($fields[$i],0,1)==$this->txt) {
         $this->field[$i]->txt  = TRUE;
	 $this->field[$i]->name = substr($fields[$i],1,strlen($fields[$i])-2);
       } else {
         $this->field[$i]->txt  = FALSE;
	 $this->field[$i]->name = $fields[$i];
       }
       if ($this->trim) $this->field[$i]->name = trim($this->field[$i]->name);
     }
     if ( $this->field[$fc -1]->txt ) {
       $this->rec_end = $this->txt."\n";
     } else {
       $this->rec_end = "\n";
     }
   }

   /** Get the file content
    *  Reads the CSV file into the internal string $this->text
    * @class csv
    * @method read_file
    * @param string filename file to read
    */
   function read_file($filename) {
     $fp = fopen($filename, "rb");
     $buffer = fread($fp, filesize($filename));
     fclose($fp);
     $this->text = preg_replace("/\r?\n|\r/", "\n", $buffer);
   }

   /** Get CSV data
    *  Reads data from the imported CSV file and stores it into
    *  the $this->data array (array of ojects with the elements: name, data)
    * @class csv
    * @method read_data
    */
   function read_data() {
     $fc = count($this->field);
     $rec = 0;
     while (!empty($this->text)) {
       $pos  = strpos($this->text,$this->rec_end);
       $line = trim(substr($this->text,0,$pos));
       if (empty($line)) {
         $this->text = substr($this->text,$pos+1);
       } else {
         for ($i=0;$i<$fc;++$i) { // record start
           $name = $this->field[$i]->name;
           if ($this->field[$i]->txt) {
             if ($i+1<$fc) {
               $pos = strpos($this->text,$this->txt.$this->sep);
             } else {
	       $pos = strpos($this->text,$this->txt."\n");
             }
             $data[$name] = substr($this->text,1,$pos-1);
             $this->text = substr($this->text,$pos+2);
           } else {
             if ($i+1<$fc) {
               $pos = strpos($this->text,$this->sep);
             } else {
               $pos = strpos($this->text,"\n");
             }
             $data[$name] = substr($this->text,0,$pos);
             $this->text = substr($this->text,$pos+1);
           }
           if ($this->trim) $data[$name] = trim($data[$name]);
           $this->data[$rec] = $data;
         } // record end
       }
       ++$rec;
     }
   }

   /** Clear all data
    *  Remove all internal data, e.g. to import a different file
    * @class csv
    * @method clear_data
    */
   function clear_data() {
     unset ($this->data);
     unset ($this->field);
   }

 } // end class csv

 /** CVS data: array[0..n] of imported records.
  *  Each Record is an array[0..n] of objects with the properties: name,data
  * @class cvs
  * @attribute array data
  */
 /** CVS field separator (defaults to ";")
  * @class cvs
  * @attribute string sep
  */
 /** CVS text field encloser (defaults to '"')
  * @class cvs
  * @attribute string txt
  */
 /** trim imported values? (Defaults to FALSE)
  * @class cvs
  * @attribute boolean trim
  */
?>