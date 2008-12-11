<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2008 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
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
  * @copyright (c) 2004-2008 by Itzchak Rehberg and IzzySoft
  */
 class db_text {

   /** Initialize: Read all data from the "text database" into the object
    * @constructor db_text
    */
   function db_text() {
   }

  #=========================================================[ Translations ]===
  /** Read in all available translations for a given language
   * @class db_text
   * @method get_translations
   * @param string lang_id two character language ID (ISO)
   * @param optional array trans already set-up translations array to
   *  add/overwrite to
   * @return array translations (ref=>trans)
   */
   function get_translations($lang_id="en",$trans=array()) {
     GLOBAL $pdl;
     $trans = array();
     if ( $lang_id!="en" ) {
       $filename = $pdl->config->base_path."lang/trans.en";
       if ( !file_exists($filename) ) return $trans;
       $trans = $this->read_translations($filename,$trans);
     }
     $filename = $pdl->config->base_path."lang/trans.$lang_id";
     if ( !file_exists($filename) ) return $trans;
     return $this->read_translations($filename,$trans);
   }

   function read_translations($filename,$trans) {
     GLOBAL $pdl;
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $csv = new csv(";",'"',TRUE,FALSE);
	 $csv->import($filename);
         $dc = count($csv->data);
	 for ($i=0;$i<$dc;++$i) {
	   $trans[$csv->data[$i]["ref"]] = $csv->data[$i]["trans"];
	 }
	 return $trans;
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
  /** Prepare divedata for numeric sort (convert field to float)
   * @method private divedat2num
   * @param ref object csv
   * @param string fieldname
   */
   function divedat2num(&$csv,$field) {
     $dc = count($csv->data);
     for ($i=0;$i<$dc;++$i) $csv->data[$i][$field] = (float) $csv->data[$i][$field];
   }
  /** Add back the unit to fields where it has been removed by divedat2num
   * @method private divedat2str
   * @param ref object csv
   * @param string fieldname
   * @param string unit
   */
   function divedat2str(&$csv,$field,$unit) {
     $dc = count($csv->data);
     for ($i=0;$i<$dc;++$i) $csv->data[$i][$field] .= " $unit";
   }
   /** Read the dive data file and setup the dive data
    * @class db_text
    * @method read_divedata
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    */
   function read_divedata($sort="",$order="") {
     GLOBAL $pdl;
     if (isset($this->dives)) return; // prevent multiple reads
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $csv = new csv(";",'"',TRUE);
         if (file_exists($pdl->config->divedata)) {
           $csv->import($pdl->config->divedata);
           switch($sort) {
             case "date"  :
             case "time"  : $sort = "dive#"; $type="num"; break;
             case "location" :
             case "place" :
             case "rating":
             case "buddy" : $type = "str"; break;
             case "depth" :
	     case "divetime" : $type="num";
	          $datastr = explode(" ",$csv->data[0][$sort]);
		  $unit    = $datastr[1];
		  $this->divedat2num($csv,$sort);
	          break;
           }
           if (!empty($sort)) {
             $csv->sort($sort,$order,$type);
           }
	   switch($sort) {
	     case "depth"    :
	     case "divetime" : $this->divedat2str(&$csv,$sort,$unit); break;
	     default         : break;
	   }
           $this->divedata = $csv->data;
           $this->dives    = count($this->divedata);
           $this->setup_dive_records();
           $csv->clear_data();
         }
     }
   }

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
       if ($i>0) $this->divedata[$i]["prev_dive#"] = $this->divedata[$i-1]["dive#"];
       if ($i+1<$this->dives) $this->divedata[$i]["next_dive#"] = $this->divedata[$i+1]["dive#"];
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
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    * @return array divedata (either array of dive records or single record)
    */
   function get_dives($start="",$end="",$id=FALSE,$sort="",$order="") {
     $this->read_divedata($sort,$order);
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
   /** Read the site data file and setup the site data
    * @class db_text
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    * @method read_divedata
    */
   function read_sitedata($sort="",$order="") {
     GLOBAL $pdl;
     if (isset($this->sites)) return; // prevent multiple reads
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $csv = new csv(";",'"',TRUE);
	 if (file_exists($pdl->config->sites)) {
           $csv->import($pdl->config->sites);
           switch ($sort) {
             case "location" : $sort="loc"; $type="str"; break;
             case "place"    :
             case "depth"    : $type="str"; break;
           }
           if (!empty($sort)) {
             $csv->sort($sort,$order,$type);
           }
           $this->sitedata = $csv->data;
           $this->sites    = count($this->sitedata);
           $this->setup_site_records(); // temporary - see func *!*
         }
     }
   }

   function setup_site_records() { // temporary: we need an ID for reference
     $this->sitedata[0]["next_site#"] = $this->sitedata[1]["id"];
     for ($i=1;$i+1<$this->sites;++$i) {
       $this->sitedata[$i]["prev_site#"] = $this->sitedata[$i-1]["id"];
       $this->sitedata[$i]["next_site#"] = $this->sitedata[$i+1]["id"];
     }
     $this->sitedata[$i+1]["prev_site#"] = $this->sitedata[$i]["id"];
     unset($this->sitedata[0]["prev_site#"]);
     unset($this->sitedata[$this->sites -1]["next_site#"]);
   }

   /** Get site records
    *  Retrieve a list of sites or a single record. Returns all sites if neither
    *  $start nor $end are specified, a range if both and the specified dive
    *  record if $start is specified but $end is not
    *  If id=FALSE, $end specifies the maximal count of records to return
    * @class db_text
    * @method get_sites
    * @param optional string start first site# to return
    * @param optional string end last site# to return
    * @param optional boolean id whether to identify by site# (TRUE) or
    *  record set (FALSE), defaults to FALSE
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    * @return array sitedata (either array of site records or single record)
    */
   function get_sites($start="",$end="",$id=FALSE,$sort="",$order="") {
     $this->read_sitedata($sort,$order);
     if ($start==="") { // all records
       return $this->sitedata;
     } else {
       if ($id) { // identify by site#
         if ($end!=="") { // range of records
           for ($i=0;$i<$this->sites;++$i) {
             if (($this->sitedata[$i]["id"] >= $start) && ($this->sitedata[$i]["id"] <= $end)) {
               $data[] = $this->sitedata[$i];
             }
	   }
           return $data;
         } else { // single record
           for ($i=0;$i<$this->sites;++$i) {
	     if ($this->sitedata[$i]["id"] == $start)
	       return $this->sitedata[$i];
	   }
         }
       } else { // identify by record
         if ($end==="") {
           return $this->sitedata[$start];
	 } else {
	   $max = $start + $end;
	   if ($max > $this->sites) $max = $this->sites;
	   for ($i=$start;$i<$max;++$i) {
	     $data[] = $this->sitedata[$i];
	   }
	   return $data;
         }
       }
     }
   }

   function get_site($id) {
     return $this->get_sites($id,"",TRUE);
   }

   /** Get an alphabetical list of all dive places from all divers
    *  (not to be confused with the dive sites - it is just the location here)
    * @class db_text
    * @method getAllPlaces
    * @param optional integer start
    * @param optional integer end
    * @param optional string place
    * @return array places
    */
   function getAllPlaces($start="",$end="",$showPlace="") {
     GLOBAL $pdl;
     $csv = new csv(";",'"',TRUE);
     $udirs = $pdl->file->get_subdirs($pdl->config->base_path."diver");
     $dircount = count($udirs);
     $cnum = array();
     for ($i=0;$i<$dircount;++$i) {
       if ( !file_exists($pdl->config->base_path."diver/".$udirs[$i]."/public") ) continue;
       $file = $pdl->config->base_path."diver/".$udirs[$i]."/data/divesites.csv";
       if (file_exists($file)) {
         $csv->clear_data();
         $csv->import($file);
         $sitecount = count($csv->data);
         for ($k=0;$k<$sitecount;++$k) {
           if ( !empty($showPlace) ) {
             if ( $showPlace != $csv->data[$k]["loc"] ) continue;
             $data->name     = $csv->data[$k]["loc"];
             $data->sitename = $csv->data[$k]["place"];
             $data->diver    = $udirs[$i];
             $data->id       = $csv->data[$k]["id"];
             $place[] = $data;
	     unset($data);
           } else {
             $place[] = $csv->data[$k]["loc"];
             if (isset($cnum[$csv->data[$k]["loc"]])) ++$cnum[$csv->data[$k]["loc"]];
	     else $cnum[$csv->data[$k]["loc"]] = 1;
           }
         }
       }
     }
     if (!is_array($place)) $place = array();
     if ( !empty($showPlace) ) {
       sort($place); reset($place);
       $this->allplaces = count($place);
       if ( $start!=="" ) { // return subset
         return $this->subset_places($place,$start,$end);
       }
       return $place;
     }
     foreach ($place as $key => $val) $tmp[$key] = serialize($val);
     foreach (array_unique($tmp) as $key => $val) {
       $p->name = $place[$key];
       $p->num  = $cnum[$place[$key]];
       $splace[] = $p;
       unset($p);
     }
     unset($place);
     sort($splace); reset($splace);
     $this->allplaces = count($splace);
     if ( $start!=="" ) { // return subset
       return $this->subset_places($splace,$start,$end);
     }
     return $splace; // return all otherwise
   }

   function subset_places($arr,$start,$end) {
     GLOBAL $pdl;
     if ( empty($end) || $end > $this->allplaces ) $end = $this->allplaces;
     if ( $end - $start > $pdl->config->display_limit )
       $end = $start + $pdl->config->display_limit;
     for ($i=$start;$i<$end;++$i) {
       $place[] = $arr[$i];
     }
     return $place;
   }

  #======================================================[ Dive Statistics ]===
   /** Get dive statistics
    * @class db_text
    * @method get_stats
    * @return array stats (properties: max_depth,max_time,avg_depth,cum_time)
    */
  function get_stats() {
     GLOBAL $pdl;
     $this->read_sitedata();
     $file = $pdl->config->datadir."global.";
     switch ($pdl->config->db_type) {
       case "csv"  :
       default     :
         $file .= "csv";
         if (!file_exists($file)) return FALSE;
         $csv = new csv(";",'"',TRUE);
         $csv->import($file);
         return $csv->data[0];
     }    
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
 /** Count of dive locations of ALL divers
  * @class db_text
  * @attribute integer allplaces
  */
?>