<?php
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # DB Emulation for text "databases" (CSV & UDDF files)                      #
 #############################################################################

 /* $Id$ */

 /** DB Emulation for text "databases" (CSV and UDDF files)
  * @package Api
  * @class auth
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class db_text {

   /** Initialize: Read all data from the "text database" into the object
    * @constructor db_text
    */
   function db_text() {
     GLOBAL $pdl;
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $csv = new csv(";",'"',TRUE);
	 if (!file_exists($pdl->config->divedata)) die ("DiveData file '".$pdl->config->divedata."' not found!<br>\n");
	 $csv->import($pdl->config->divedata);
	 $this->divedata = $csv->data;
	 $this->dives    = count($this->divedata);
	 $this->setup_dive_records(); // temporary - see func *!*
	 $csv->clear_data();
	 $csv->import($pdl->config->sites);
         $this->sitedata = $csv->data;
         $this->sites    = count($this->sitedata);
	 $this->setup_site_records(); // temporary - see func *!*
     }
   }

  #============================================================[ Dive Data ]===
   function setup_dive_records() { # temporary until the prev/next are available in CSV data
     for ($i=0;$i<$this->dives;++$i) {
       $this->divedata[$i]["prev_dive#"] = $this->divedata[$i-1]["dive#"];
       $this->divedata[$i]["next_dive#"] = $this->divedata[$i+1]["dive#"];
     }
   }

   /** Get dive records
    *  Retrieve a list of dives or a single record. Returns all dives if neither
    *  $start nor $end are specified, a range if both and the specified dive
    *  record if $start is specified but $end is not
    *  If id=FALSE, $end specifies the maximal count of records to return
    * @class db_text
    * @method get_dives
    * @param optional string start first dive# to return
    * @param optional string end last dive# to return
    * @param optional boolean id whether to identify by dive# (TRUE) or
    *  record set (FALSE), defaults to FALSE
    * @return array divedata (either array of dive records or single record)
    */
   function get_dives($start="",$end="",$id=FALSE) {
     if ($start!=="") {
       if ($id) { // identify by dive#
         if ($end!=="") { // range of records
           for ($i=0;$i<$this->dives;++$i) {
             if (($this->divedata[$i]["dive#"] >= $start) && ($this->divedata[$i]["dive#"] <= $end)) {
               $data[] = $this->divedata[$i];
             }
	   }
           return $data;
         } else { // single record
           for ($i=0;$i<$this->dives;++$i) {
	     if ($this->divedata[$i]["dive#"] == $start)
	       return $this->divedata[$i];
	   }
         }
       } else { // identify by record
         if ($end!=="") {
	   $max = $start + $end;
	   if ($max > $this->dives) $max = $this->dives;
	   for ($i=$start;$i<$max;++$i) {
	     $data[] = $this->divedata[$i];
	   }
	   return $data;
	 } else {
	   return $this->divedata[$start];
	 }
       }
     } else { // all records
       return $this->divedata;
     }
   }

   /** Get dive data for a record specified by dive#
    *  Wrapper to get_dives
    * @class db_text
    * @method get_dive
    * @param integer dive#
    * @return array divedata (name=value)
    */
   function get_dive($nr) {
     return $this->get_dives($nr,"",TRUE);
   }

  #============================================================[ Locations ]===
   function setup_site_records() { // temporary: we need an ID for reference
     for ($i=0;$i<$this->sites;++$i) {
       $this->sitedata[$i]["site#"] = $i;
       $this->sitedata[$i]["prev_site#"] = $i-1;
       $this->sitedata[$i]["next_site#"] = $i+1;
     }
     unset($this->sitedata[0]["prev_site#"]);
     unset($this->sitedata[$this->sites -1]["next_site#"]);
   }

   function get_sites($start="",$end="",$id=FALSE) {
     if ($start==="") {
       return $this->sitedata;
     } else {
       return $this->sitedata[$start];
     }
   }

   function get_site($id) {
     return $this->get_sites($id,"",TRUE);
   }

 } // end class db_text

 /** Dive data: array[0..n] of imported dive records.
  *  Each Record is an array[0..n] of objects with the properties: name,data
  * @class db_text
  * @attribute array divedata
  */
 /** Count of imported dive records
  * @class db_text
  * @attribute integer dives
  */
?>