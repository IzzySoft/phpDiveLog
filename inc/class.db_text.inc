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
	 $this->setup_dive_records();
	 $csv->clear_data();
	 if (!file_exists($pdl->config->sites)) die ("DiveSites file '".$pdl->config->sites."' not found!<br>\n");
	 $csv->import($pdl->config->sites);
         $this->sitedata = $csv->data;
         $this->sites    = count($this->sitedata);
	 $this->setup_site_records(); // temporary - see func *!*
     }
   }

  #========================================================[ Dive Schedule ]===
   /** Get dive schedule for a given dive#
    * @class db_text
    * @method get_schedule
    * @param integer dive# to retrieve the schedule for
    * @return mixed schedule (boolean FALSE if no schedule exists, array otherwise)
    */
  function get_schedule($nr) {
     GLOBAL $pdl;
     $file = $pdl->config->datadir."dive".sprintf("%05d",$nr)."_schedule.";
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $file .= "csv";
         if (!file_exists($file)) return FALSE;
         $csv = new csv(";",'"',TRUE);
         $csv->import($file);
         return $csv->data;
     }    
  }

  #============================================================[ Dive Data ]===
   function setup_dive_records() {
     for ($i=0;$i<$this->dives;++$i) {
       # split up the tank data
       $pos = strpos(":",$this->divedata[$i]["tank#"]);
       $tank_nr      = explode(":",$this->divedata[$i]["tank#"]);
       $tank_id      = explode(":",$this->divedata[$i]["tank_id"]);
       $tank_name    = explode(":",$this->divedata[$i]["tank_name"]);
       $tank_volume  = explode(":",$this->divedata[$i]["tank_volume"]);
       $tank_type    = explode(":",$this->divedata[$i]["tank_type"]);
       $tank_gas     = explode(":",$this->divedata[$i]["tank_gas"]);
       $tank_in      = explode(":",$this->divedata[$i]["tank_in"]);
       $tank_out     = explode(":",$this->divedata[$i]["tank_out"]);
       $tank_count   = count($tank_nr);
       for ($k=0;$k<$tank_count;++$k) {
         $tank->nr     = $tank_nr[$k];
         $tank->id     = $tank_id[$k];
         $tank->name   = $tank_name[$k];
         $tank->volume = $tank_volume[$k];
         $tank->type   = $tank_type[$k];
         $tank->gas    = $tank_gas[$k];
         $tank->in     = $tank_in[$k];
         $tank->out    = $tank_out[$k];
         $this->divedata[$i]["tank"][$k] = $tank;
       }
       unset ($this->divedata[$i]["tank#"],$this->divedata[$i]["tank_id"],
              $this->divedata[$i]["tank_name"],$this->divedata[$i]["tank_volume"],
              $this->divedata[$i]["tank_type"],$this->divedata[$i]["tank_gas"],
              $this->divedata[$i]["tank_in"],$this->divedata[$i]["tank_out"]);

       # temporary until the prev/next are available in CSV data
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