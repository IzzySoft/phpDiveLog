<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2005 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # General database class                                                    #
 #############################################################################

 /* $Id$ */

 /** General database class
  * @package Api
  * @class sql
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004-2005 by Itzchak Rehberg and IzzySoft
  */
 class sql extends DB_Sql {
   VAR $limit;

   /** Constructor: initialize base class, initialize limit property depending on database type
    * @constructor sql
    */
   function sql() { // init
     GLOBAL $pdl;
     $this->Host     = $pdl->config->database["host"];
     $this->Database = $pdl->config->database["database"];
     $this->User     = $pdl->config->database["user"];
     $this->Password = $pdl->config->database["password"];
     $this->DB_Sql(); // constructor of base class needs explicit call
#     $this->Halt_On_Error = "no";
     $this->table_def(); // setup table names
     $userid = $this->userIDget();
   }

   ############################################################################
   # common stuff
   ############################################################################
   /** setup sql clause to limit result set (depending on database type)
    * @method private limit
    * @param optional integer start
    * @return string limit SQL limit clause for the configured database type
    */
   function limit($start=0) {
     GLOBAL $pdl;
     switch($pdl->config->db_type) {
       case "mysql" : $this->limit = " LIMIT $start," . $pdl->config->display_limit;
                      break;
       case "pgsql" : $this->limit = " LIMIT " . $pdl->config->display_limit . "OFFSET $start";
                      break;
       default      : $this->limit = ""; break;
     }
     return $this->limit;
   }

   /** do a query on the db and provide debug output
    * @method dbquery
    * @param string query SQL statement to execute
    * *variable array colors
    * @return boolean success
    */
   function dbquery($query) {
#     debug("S","<SPAN CLASS='ok'>$query</SPAN><BR>\n");
     if ( $this->query($query) ) {
       return TRUE;
     }
     return FALSE;
   }

   /** Query db using the LIMIT clause. returns count of TOTAL datasets found
    *  (i.e. UNLIMITED query result count). $query must NOT contain any limit
    *  clause!!!
    * @method private lim_query
    * @param string query SQL statement to execute
    * @param optional integer start
    * @return integer totals (result set count)
    */
   function lim_query($query,$start=0) {
     if ( $this->dbquery($query) ) {
       $totals = $this->num_rows();
       $this->limit($start);
       $query .= $this->limit;
       $this->dbquery($query);
       return $totals;
     }
     return 0;
   }

   /** Start transaction
    * @method begin
    */
   function begin() {
     return $this->dbquery("BEGIN");
   }

   /** Rollback transaction
    * @method rollback
    */
   function rollback() {
     return $this->dbquery("ROLLBACK");
   }

   /** Commit transaction
    * @method commit
    */
   function commit() {
     return $this->dbquery("COMMIT");
   }

   /** Delete a complete row from a table
    * @method private delete_row
    * @param string table table name
    * @param string id row id(s) to delete (multiple IDs must be separated by
    *  commata; row id is the value of the id column)
    * @param integer user_id
    * @return boolean success
    */
   function delete_row($table,$id,$user_id) {
     return $this->dbquery("DELETE FROM $table WHERE id IN ($id) and user_id=$user_id"); 
   }

   /** Drop all dive records for a given user (usually for re-import or
    *  complete user drop)
    * @method log_drop
    * @param integer user_id
    * @return boolean success
    */
   function log_drop($user_id) {
     $details = array("pdl_logbook","pdl_sites","pdl_suits","pdl_tanks");
     foreach ($this->tables->logs as $table) {
       if (!$this->dbquery("DELETE FROM $table WHERE user_id=$user_id")) return FALSE;
     }
     return TRUE;
   }

   /** Table definitions
    * @method table_def
    */
   function table_def() {
     $this->tables->user        = "pdl_users";
     $this->tables->session     = "pdl_sessions";
     $this->tables->logs[]      = "pdl_logbook";
     $this->tables->logs[]      = "pdl_sites";
     $this->tables->logs[]      = "pdl_suits";
     $this->tables->logs[]      = "pdl_tanks";
     $this->tables->site        = "pdl_sites";
     $this->tables->suit        = "pdl_suits";
     $this->tables->tank        = "pdl_tanks";
     $this->tables->logbook     = "pdl_logbook";
     $this->tables->languages   = "pdl_languages";
     $this->tables->translations= "pdl_translations";
   }

   ###########################################################################
   # Translation Management
   ###########################################################################
   /** Add a language
    * @method language_add
    * @param string id language id (2 letters)
    * @param string name language name
    * @param optional integer available (0|1)
    * @return boolean success
    */
   function language_add($id,$name,$available=0) {
     GLOBAL $pdl;
     $query = "SELECT count(lang_name) langs FROM ".$this->tables->languages
            . " WHERE id='$id'";
     $this->dbquery($query);
     $this->next_record();
     $langs = $this->f('langs');
     if ($langs!=0) {
       $pdl->last_error = "duplicate_lang_id";
       return FALSE;
     }
     $query = "INSERT INTO ".$this->tables->languages
            . "(id,lang_name,available) VALUES ('$id','$name',$available)";
     $this->begin();
     if ($this->dbquery($query)) {
       $this->commit();
       return TRUE;
     }
     $this->rollback();
     return FALSE;
   }

   /** (De)activate language
    * @method language_active
    * @param string lang_id language id (2 letters)
    * @param optional integer active [0|1] default 1
    * @return boolean success
    */
   function language_active($id,$active=1) {
     $query = "UPDATE ".$this->tables->languages
            . " SET available=$active WHERE id='$id'";
     return ($this->dbquery($query));
   }

   /** Update/Insert translations
    * @method translations_update
    * @parameter string lang_id language id (2 letters)
    * @parameter array translations (message_id=>content)
    * @return boolean success
    */
   function translations_update($lang_id,$translations) {
     GLOBAL $pdl;
     $records = count($translations);
     $this->begin();
     $query = "DELETE FROM ".$this->tables->translations
            . " WHERE lang_id='$lang_id'";
     if (!$this->dbquery($query)) {
       $this->rollback();
       $pdl->last_error = "translation_truncate_failed";
       return FALSE;
     }
     foreach ($translations as $var=>$val) {
       $query = "INSERT INTO ".$this->tables->translations
              . "(lang_id,message_id,content) VALUES ('$lang_id','$var','".addslashes($val)."')";
       if (!$this->dbquery($query)) {
         $this->rollback();
         $pdl->last_error = "translation_insert_failed";
         return FALSE;
       }
     }
     $this->language_active($lang_id);
     $this->commit();
     return TRUE;
   }

   /** Read in all available translations for a given language
    * @method translations_get
    * @param string lang_id two character language ID (ISO)
    * @param optional array trans already set-up translations array to
    *  add/overwrite to
    * @return array translations (ref=>trans)
    */
   function translations_get($lang_id="en",$trans=array()) {
     GLOBAL $pdl;
     if ( $lang_id!="en" ) {
       $trans = $this->translations_get("en");
     }
     $query = "SELECT message_id,content FROM ".$this->tables->translations
            . " WHERE lang_id='en'";
     $this->dbquery($query);
     while ($this->next_record()) {
       $id = $this->f('message_id');
       $trans[$id] = $this->f('content');
     }
     return $trans;
   }

   ###########################################################################
   # User Management
   ###########################################################################
   /** Add user
    * @method user_add
    * @param object user (login,pwd,name,firstname,comment,admin)
    * @return boolean success
    */
   function user_add($user) {
     GLOBAL $pdl;
     # $login   = $pvp->common->safeinput($user->login); # *!*
     # $comment = $pvp->common->safeinput($user->comment); # *!*
     // check if duplicate
     $this->dbquery("SELECT id FROM ".$this->tables->user." WHERE login='".$user->login."'");
     $this->next_record();
     if ($this->f('id')) {
       $pdl->last_error = "duplicate_user";
       return FALSE;
     }
     // if still here, add the user
     $query  = "INSERT INTO ".$this->tables->user." (login";
     $values = "('".$user->login."'";
     if (!empty($user->pwd)) {
       $query  .= ",pwd";
       $values .= ",'".md5($user->pwd)."'";
     }
     $details = array("name","firstname","comment","admin");
     foreach ($details as $var) {
       if (!empty($user->$var)) {
         $query  .= ",$var";
         $values .= ",'".$user->$var."'";
       }
     }
     $query .= ") VALUES $values)";
     $this->dbquery($query);
     if ( $this->affected_rows() < 0 ) {
       $pdl->last_error = "no_affected_rows";
       return FALSE;
     }
     return TRUE;
   }

   /** Drop user
    * @method user_drop
    * @param integer user_id
    * @return boolean success
    */
   function user_drop($user_id) {
     GLOBAL $pdl;
     if (empty($user_id)) {
       $pdl->last_error = "user_id_empty";
       return FALSE;
     }
     $this->begin();
     if (!$this->log_drop($user_id)) {
       $pdl->last_error = "log_drop_failed";
       $this->rollback();
       return FALSE;
     }
     if (!$this->dbquery("DELETE FROM ".$this->tables->user." WHERE id=$user_id")) {
       $pdl->last_error = "user_delete_failed";
       $this->rollback();
       return FALSE;
     }
     $this->commit();
     return TRUE;
   }

   /** Obtain user_id
    * @method userIDget
    * @return integer user_id
    */
   function userIDget() {
     GLOBAL $pdl;
     static $userid;
     if (!empty($userid)) return $userid;
     $this->dbquery("SELECT id FROM ".$this->tables->user." WHERE login='".$pdl->config->user_name."'");
     $this->next_record();
     $userid = $this->f('id');
     return $userid;
   }

   ###########################################################################
   # Site Management
   ###########################################################################
   /** Add site
    * @method site_add
    * @param array site (id,loc,place,latitude,longitude,altitude,depth,
    *                     water,type,rating,description)
    * return boolean success
    */
   function site_add($site) {
     GLOBAL $pdl;
     $userID = $this->userIDget();
     $fields = "(id,user_id";
     $values = "VALUES (".$site["id"].",$userID";
     if (!empty($site["depth"])) {
       $pos = strpos($site["depth"],' ');
       $site["depth"] = substr($site["depth"],0,$pos)*10;
     }
     if (!empty($site["description"])) $site["description"] = addslashes($site["description"]);
     if (!is_numeric($site["rating"])) $site["rating"] = '';
     $details = array("loc","place","latitude","longitude","altitude","depth","water","type","rating","description");
     foreach ($details as $var) {
       if (!empty($site[$var])) {
         if ($var == "type") $fields .= ",stype";
         else $fields .= ",$var";
         $values .= ",'".$site[$var]."'";
       }
     }
     $query = "INSERT INTO ".$this->tables->site." $fields) $values)";
     return $this->dbquery($query);
   }

   ###########################################################################
   # Equipment Management
   ###########################################################################
   /** Add a suit
    * @method suit_add
    * @param array suit (id,name,stype,weight)
    */
   function suit_add($suit) {
     GLOBAL $pdl;
     $this->dbquery("SELECT id,name,stype,weight FROM ".$this->tables->suit." WHERE id=".$suit["id"]);
     $this->next_record();
     $id = $this->f('id');
     if ($id==$suit["id"]) {
       if ( ($this->f('name')!=$suit["name"]) || ($this->f('stype')!=$suit["stype"]) || ($this->f('weight')!=$suit["weight"]) )
         return FALSE;
       else return TRUE; //already have this
     }
     $userID = $this->userIDget();
     $fields = "(id,user_id";
     $values = "VALUES (".$suit["id"].",$userID";
     $details = array("name","stype","weight");
     foreach ($details as $var) {
       if (!empty($suit[$var])) {
         $fields .= ",$var";
         $values .= ",'".$suit[$var]."'";
       }
     }
     $query = "INSERT INTO ".$this->tables->suit." $fields) $values)";
     return $this->dbquery($query);
   }

   /** Add a tank
    * @method tank_add
    * @param array tank (id,name,volume,ttype)
    */
   function tank_add($tank) {
     GLOBAL $pdl;
     $this->dbquery("SELECT id,name,ttype,volume FROM ".$this->tables->tank." WHERE id=".$tank["id"]);
     $this->next_record();
     $id = $this->f('id');
     if ($id===0||$id==='0') return TRUE; // *!* workaround wrong entry (id=0)
     if ($id==$tank["id"]) {
       if ( ($this->f('name')!=$tank["name"]) || ($this->f('volume')!=$tank["volume"]) || ($this->f('ttype')!=$tank["ttype"]) ) {
         return FALSE; }
       else return TRUE; //already have this
     }
     $userID = $this->userIDget();
     $fields = "(id,user_id";
     $values = "VALUES (".$tank["id"].",$userID";
     $details = array("name","ttype","volume");
     foreach ($details as $var) {
       if (!empty($tank[$var])) {
         $fields .= ",$var";
         $values .= ",'".$tank[$var]."'";
       }
     }
     $query = "INSERT INTO ".$this->tables->tank." $fields) $values)";
     return $this->dbquery($query);
   }

   ###########################################################################
   # Dives Management
   ###########################################################################
   /** Add a dive
    * @method dive_add
    * @param array dive
    * @return boolean success
    */
   function dive_add($dive) {
     GLOBAL $pdl;
     $this->dbquery("SELECT id FROM ".$this->tables->logbook." WHERE id=".$dive["dive#"]);
     $this->next_record();
     $id = $this->f('id');
     if (!empty($id)) {
       return FALSE;
     }
     // prepare some fields
     if (!is_numeric($dive["rating"])) $dive["rating"] = '';
     $dive["zeit"] = $dive["time"];
     $dive["tanks"] = count($dive["tank"]);
     for ($i=0;$i<$dive["tanks"];++$i) {
       $dive["tank_id"]  .= ",".$dive["tank"][$i]->id;
       $dive["tank_gas"] .= ",".$dive["tank"][$i]->gas;
       $dive["tank_in"]  .= ",".$dive["tank"][$i]->in;
       $dive["tank_out"] .= ",".$dive["tank"][$i]->out;
     }
     $dive["tank_id"] .= ",";
     $dive["tank_gas"] .= ",";
     $dive["tank_in"] .= ",";
     $dive["tank_out"] .= ",";
     $dive["notes"] = addslashes($dive["notes"]);
     // create and execute statement
     $userID = $this->userIDget();
     $fields = "(id,user_id";
     $values = "VALUES (".$dive["dive#"].",$userID";
     $details = array("site_id","suit_id","rating","depth","divetime","tanks","visibility","watertemp","airtemp");
     foreach ($details as $var) { // numbers
       if(!empty($dive[$var])) {
         $fields .= ",$var";
         $values .= ",".$dive[$var];
       }
     }
     $details = array("datum","zeit","tank_id","tank_gas","tank_in","tank_out","buddy","weight","userdef1","userdef2","workload","wcurrent","notes");
     foreach ($details as $var) { // strings
       if(!empty($dive[$var])) {
         $fields .= ",$var";
         $values .= ",'".$dive[$var]."'";
       }
     }
     $query = "INSERT INTO ".$this->tables->logbook." $fields) $values)";
     return $this->dbquery($query);
     echo "$query<br>";
     return TRUE;
   }

   function tank_details_get($id,$user_id) {
     $query = "SELECT name,volume,ttype type"
            . "  FROM ".$this->tables->tank
            . " WHERE id=$id AND user_id=$user_id";
     $this->dbquery($query);
     $this->next_record();
     $details = array("name","volume","type");
     foreach ($details as $var) {
       $tank->$var = $this->f($var);
     }
     return $tank;
   }

   /** Create the base SQL statement for dive records
    * @method private dives_base_query
    * @return string query
    */
   function dives_base_query() {
     $user_id = $this->userIDget();
     $query = "SELECT l.id,l.datum,l.zeit,l.depth/10 depth,l.divetime/60 divetime,"
            . "       l.tanks,l.tank_id,l.tank_gas,l.tank_in,l.tank_out,buddy,"
            . "       l.visibility/10 visibility,l.watertemp,l.airtemp,"
            . "       l.wcurrent current,l.workload,l.weight/2 weight,l.userdef1,"
            . "       l.userdef2,l.rating,l.notes,l.site_id,"
            . "       su.name suitname,su.stype suittype,su.weight/2 suitweight,"
            . "       si.loc,si.place"
            . "  FROM ".$this->tables->logbook." l, ".$this->tables->suit." su,"
            .         $this->tables->site." si"
            . " WHERE l.user_id=$user_id AND su.user_id=$user_id AND si.user_id=$user_id"
            . "   AND l.suit_id=su.id AND l.site_id=si.id";
     return $query;
   }

   /** Setup the dive record from fields defined by the dives_base_query.
    *  db::nextrecord() is NOT called here, just a single record ist set up
    *  and returned.
    * @method private dive_record_setup
    * @param boolean single single record full setup (breaks list!)
    * @return array dive_record
    */
   function dive_record_setup($full=FALSE) {
     $dive["dive#"] = $this->f('id');
     $dive["date"]  = $this->f('datum');
     $dive["time"]  = $this->f('zeit');
     $dive["location"] = $this->f('loc');
     $dive["divetime"] = round($this->f('divetime'))." min";
     $dive["depth"] = round($this->f('depth'),1)." m";
     $dive["visibility"] = round($this->f('visibility'),1)." m";
     $dive["suitweight"] = round($this->f('suitweight')/2,1)." kg";
     $dive["weight"] = round($this->f('weight')/2,1)." kg";
     $details = array("site_id","place","buddy","watertemp","airtemp","current",
                      "workload","suitname","suittype","userdef1","userdef2",
                      "rating","notes");
     foreach ($details as $var) {
       $dive[$var] = $this->f($var);
     }
     if (empty($dive["rating"])) $dive["rating"] = "-";
     $dive["watertemp"] .= " °C";
     $dive["airtemp"] .= " °C";
     $details = array("tank_id","tank_gas","tank_in","tank_out");
     foreach ($details as $var) {
       $x = $this->f($var);
       $x = substr($x,1,strlen($x)-2);
       ${$var} = explode(",",$x);
     }
     if ($full) {
       $tc = $this->f('tanks');
       $user_id = $this->userIDget();
       for ($i=0;$i<$tc;++$i) {
         $dive["tank"][$i] = $this->tank_details_get($tank_id[$i],$user_id);
         $dive["tank"][$i]->in  = $tank_in[$i]." bar";
         $dive["tank"][$i]->out = $tank_out[$i]." bar";
         $dive["tank"][$i]->gas = $tank_gas[$i];
       }
       $query = "SELECT max(id) prev_id FROM ".$this->tables->logbook
              . " WHERE id <".$dive["dive#"]." AND user_id=$user_id";
       $this->dbquery($query);
       if ($this->next_record()) {
         $dive["prev_dive#"] = $this->f('prev_id');
       }
       $query = "SELECT min(id) next_id FROM ".$this->tables->logbook
              . " WHERE id >".$dive["dive#"]." AND user_id=$user_id";
       $this->dbquery($query);
       if ($this->next_record()) {
         $dive["next_dive#"] = $this->f('next_id');
       }
     }
     return $dive;
   }

   /** Get dive records
    *  Retrieve a list of dives or a single record. Returns all dives if neither
    *  $start nor $end are specified, a range if both and the specified dive
    *  record if $start is specified but $end is not
    *  If id=FALSE, $end specifies the maximal count of records to return
    * @method dives_get
    * @param optional string start first dive# to return
    * @param optional string end last dive# to return
    * @param optional boolean id whether to identify by dive# (TRUE) or
    *  record set (FALSE), defaults to FALSE (ignored; only compatibility
    *  for the CSV "database")
    * @return array divedata (either array of dive records or single record)
    */
   function dives_get($start="",$end="",$id=FALSE) {
     GLOBAL $pdl;
     $query = $this->dives_base_query();
     $mode  = "list"; // list of dives or single dive?
     if ($start!=="") {
       if ($end!=="") { // dive list
         $query .= " ORDER BY l.id ";
         $ok = $this->lim_query($query,$start);
       } else { // single record
         $mode = "single";
         $query .= " AND l.id=$start";
         $ok = $this->dbquery($query);
       }
     } else { // return all data
       $ok = $this->dbquery($query);
     }
     if (!$ok) {
       $pdl->last_error = "dive_query_execute_failed";
       return array();
     }
     if ($mode=="single") {
       $this->next_record();
       $dives = $this->dive_record_setup(TRUE);
     } else {
       while ($this->next_record()) {
         $dives[] = $this->dive_record_setup();
       }
       $this->dives = $ok;
     }
     return $dives;
   }

   #########################################################################
   # Temporary wrappers
   #########################################################################
   function get_dives($start="",$end="",$id=FALSE) {
     return $this->dives_get($start,$end,$id);
   }
   function get_dive($nr) {
     return $this->dives_get($nr);
   }
   function get_schedule($nr) { // not yet implemented *!*
     return FALSE;
     return $this->schedule_get($nr);
   }
   function get_translations($lang,$trans=array()) {
     return $this->translations_get($lang,$trans);
   }

 } // end class sql

# $pdl->db = new sql;
?>