<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
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
  * @author Izzy (izzysoft AT qumran DOT org)
  * @copyright (c) 2004-2007 by Itzchak Rehberg and IzzySoft
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
     $user  = $this->user_logins(); // initialize user records
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
     $this->tables->config      = "pdl_config";
     $this->tables->userprefs   = "pdl_userprefs";
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
       $pdl->last_error[] = "duplicate_lang_id";
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
       $pdl->last_error[] = "translation_truncate_failed";
       return FALSE;
     }
     foreach ($translations as $var=>$val) {
       $query = "INSERT INTO ".$this->tables->translations
              . "(lang_id,message_id,content) VALUES ('$lang_id','$var','".addslashes($val)."')";
       if (!$this->dbquery($query)) {
         $this->rollback();
         $pdl->last_error[] = "translation_insert_failed";
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
       $pdl->last_error[] = "duplicate_user";
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
       $pdl->last_error[] = "no_affected_rows";
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
       $pdl->last_error[] = "user_id_empty";
       return FALSE;
     }
     $this->begin();
     if (!$this->log_drop($user_id)) {
       $pdl->last_error[] = "log_drop_failed";
       $this->rollback();
       return FALSE;
     }
     if (!$this->dbquery("DELETE FROM ".$this->tables->user." WHERE id=$user_id")) {
       $pdl->last_error[] = "user_delete_failed";
       $this->rollback();
       return FALSE;
     }
     $this->commit();
     return TRUE;
   }

   /** Obtain user_id
    * @method userIDget
    * @param optional string login
    * @return integer user_id
    */
   function userIDget($login="") {
     GLOBAL $pdl;
     static $userid;
     if ($login==="" && !empty($userid)) return $userid;
     if ($login==="") $login = $pdl->config->user_name;
     $this->dbquery("SELECT id FROM ".$this->tables->user." WHERE login='$login'");
     $this->next_record();
     $user_id = $this->f('id');
     if ($login==$pdl->config->user_name) $userid = $user_id;
     return $user_id;
   }

   /** List users
    * @method user_logins
    * @param optional integer user_id
    * @return mixed array (id=>login) or single login for given id
    */
   function user_logins($user_id="") {
     GLOBAL $pdl;
     static $logins;
     if (empty($logins)) {
       $query = "SELECT id,login FROM ".$this->tables->user;
       if (!$this->dbquery($query)) {
         $pdl->last_error[] = "user_logins_query_failed";
         return FALSE;
       }
       while ($this->next_record()) {
         $id    = $this->f('id');
         $logins[$id] = $this->f('login');
       }
     }
     if ($user_id!=="") return $logins[$user_id];
     return $logins;
   }

   /** Get a user pref
    * @method user_pref_get
    * @param optional string name preference name
    * @return mixed pref (string if name specified, FALSE on failure,
    *         array(name,pref) otherwise
    */
   function user_pref_get($name="",$user_id="") {
     GLOBAL $pdl;
     if ($user_id=="") $user_id = $this->userIDget();
     $query = "SELECT name,value FROM ".$this->tables->userprefs
            . " WHERE user_id=$user_id";
     $mode = "list";
     if (!empty($name)) {
       $query .= " AND name='$name'";
       $mode = "single";
     }
     if (!$this->dbquery($query)) {
       $pdl->last_error[] = "userpref_query_failed";
       return FALSE;
     }
     if ($mode=="single") {
       $this->next_record();
       return $this->f('value');
     }
     while ($this->next_record()) {
       $pname = $this->f('name');
       $pref[$pname] = $this->f('value');
     }
     return $pref;
   }

   /** Set a user pref
    * @method user_pref_set
    * @param string name pref name
    * @param string value pref value
    * @param optional string login name (if not current user)
    * @return boolean success
    */
   function user_pref_set($name,$value,$login="") {
     GLOBAL $pdl;
     if ($login==="") $login = $pdl->config->user_name;
     $user_id = $this->userIDget($login);
     $test = $this->user_pref_get($name,$user_id);
     if (empty($test) && $test!=0) { // not set at all
       $query = "INSERT INTO ".$this->tables->userprefs
              . "(user_id,name,value) VALUES ($user_id,'$name','$value')";
     } else { // pref already in DB
       if ($test===$value) { // no changes needed
         return TRUE;
       }
       $query = "UPDATE ".$this->tables->userprefs
              . "   SET value='$value'"
              . " WHERE user_id=$user_id AND name='$name'";
     }
     if ($this->dbquery($query)) return TRUE;
     $pdl->last_error[] = "set_userpref_failed";
     return FALSE;
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
     $details = array("description","loc","place","water","type");
     foreach ($details as $var) { // strings
       if (!empty($site[$var])) {
         if ($var == "type") $fields .= ",stype";
         else $fields .= ",$var";
         $values .= ",'".addslashes($site[$var])."'";
       }
     }
#     if (!empty($site["description"])) $site["description"] = addslashes($site["description"]);
     if (!is_numeric($site["rating"])) $site["rating"] = '';
     $details = array("latitude","longitude","altitude","depth","rating");
     foreach ($details as $var) { // numbers
       if (!empty($site[$var])) {
         $fields .= ",$var";
         $values .= ",".$site[$var];
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
     $userID = $this->userIDget();
     if (empty($suit["id"]) && $suit["id"]!==0) {
       $pdl->last_error[] = "suit_id_not_set";
       return FALSE;
     }
     $this->dbquery("SELECT id,name,stype,weight FROM ".$this->tables->suit." WHERE id=".$suit["id"]." AND user_id=$userID");
     $this->next_record();
     $id = $this->f('id');
     if ($id==$suit["id"]) {
       if ( ($this->f('name')!=$suit["name"]) || ($this->f('stype')!=$suit["stype"]) || ($this->f('weight')!=$suit["weight"]) ) {
         $pdl->last_error[] = "suit_details_no_match";
         return FALSE;
       } else return TRUE; //already have this
     }
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
     $userID = $this->userIDget();
     $this->dbquery("SELECT id,name,ttype,volume FROM ".$this->tables->tank." WHERE id=".$tank["id"]." AND user_id=$userID");
     $this->next_record();
     $id = $this->f('id');
     if ($id===0||$id==='0') return TRUE; // *!* workaround wrong entry (id=0)
     if ($id==$tank["id"]) {
       if ( ($this->f('name')!=$tank["name"]) || ($this->f('volume')!=$tank["volume"]) || ($this->f('ttype')!=$tank["ttype"]) ) {
         $pdl->last_error[] = "tank_details_no_match";
         return FALSE;
       } else return TRUE; //already have this
     }
     $fields = "(id,user_id,volume";
     $values = "VALUES (".$tank["id"].",$userID,".$tank["volume"];
     $details = array("name","ttype");
     foreach ($details as $var) {
       if (!empty($tank[$var])) {
         $fields .= ",$var";
         $values .= ",'".addslashes($tank[$var])."'";
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
     $userID = $this->userIDget();
     $this->dbquery("SELECT id FROM ".$this->tables->logbook." WHERE id=".$dive["dive#"]." AND user_id=$userID");
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
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    * @return array divedata (either array of dive records or single record)
    */
   function dives_get($start="",$end="",$id=FALSE,$sort="",$order="") {
     GLOBAL $pdl;
     $query = $this->dives_base_query();
     $mode  = "list"; // list of dives or single dive?
     switch($sort) {
       case "date"  : $sort = "l.datum"; break;
       case "time"  : $sort = "l.zeit"; break;
       case "location" : $sort = "si.loc,si.place"; break;
       case "place" : $sort = "si.place"; break;
       case "rating": $sort = "l.rating"; break;
       case "depth" : $sort = "l.depth"; break;
       case "buddy" : $sort = "l.buddy"; break;
       default      : $sort = "l.id"; break;
     }
     if ($start!=="") {
       if ($end!=="") { // dive list
         $query .= " ORDER BY $sort $order";
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
       $pdl->last_error[] = "dive_query_execute_failed";
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

   ###########################################################################
   # Sites Management
   ###########################################################################
   /** Create the base SQL statement for site records
    * @method private sites_base_query
    * @return string query
    */
   function sites_base_query() {
     $query = "SELECT id,loc,place,latitude,longitude,altitude,depth,water,stype,rating,description"
            . "  FROM ".$this->tables->site;
     return $query;
   }

   /** Setup the site record from fields defined by the sites_base_query.
    *  db::nextrecord() is NOT called here, just a single record ist set up
    *  and returned.
    * @method private site_record_setup
    * @param boolean single single record full setup (breaks list!)
    * @return array dive_record
    */
   function site_record_setup($full=FALSE) {
     $details = array("id","latitude","longitude","altitude","depth","rating",
                      "loc","place","water","description");
     foreach ($details as $var) {
       $site[$var] = $this->f($var);
     }
     $site["type"] = $this->f('stype');
     $site["longitude"] .= "°";
     $site["latitude"]  .= "°";
     $site["altitude"]  .= "m";
     $site["depth"]      = round($site["depth"]/10)."m";
     if ($full) {
       $user_id = $this->userIDget();
       $query = "SELECT max(id) prev_id FROM ".$this->tables->site
              . " WHERE id <".$site["id"]." AND user_id=$user_id";
       $this->dbquery($query);
       if ($this->next_record()) {
         $site["prev_site#"] = $this->f('prev_id');
       }
       $query = "SELECT min(id) next_id FROM ".$this->tables->site
              . " WHERE id >".$site["id"]." AND user_id=$user_id";
       $this->dbquery($query);
       if ($this->next_record()) {
         $site["next_site#"] = $this->f('next_id');
       }
     }
     return $site;
   }

   /** Get site records
    *  Retrieve a list of sites or a single record. Returns all sites if neither
    *  $start nor $end are specified, a range if both and the specified dive
    *  record if $start is specified but $end is not
    *  $id is just kept for compatibility to the CVS database.
    * @method sites_get
    * @param optional string start first site# to return
    * @param optional string end last site# to return
    * @param optional string sort column to sort by
    * @param optional string order sort order (asc or desc)
    * @return array sitedata (either array of site records or single record)
    */
   function sites_get($start="",$end="",$id=FALSE,$sort="",$order="") {
     GLOBAL $pdl;
     $user_id = $this->userIDget();
     $query = $this->sites_base_query(). " WHERE user_id=$user_id";
     $mode = "list"; // list of sites or single record?
     switch ($sort) {
       case "location" : $sort = " ORDER BY loc"; break;
       case "place"    : $sort = " ORDER BY place"; break;
       case "depth"    : $sort = " ORDER BY depth"; break;
       default         : $sort = ""; break;
     }
     if (!empty($sort)) $sort .= " $order";
     if ($start!=="") {
       if ($end!=="") { // dive list
         $ok = $this->lim_query($query.$sort,$start);
       } else { // single record
         $mode = "single";
         $query .= " AND id=$start";
         $ok = $this->dbquery($query);
       }
     } else { // return all data
       $ok = $this->dbquery($query.$sort);
     }
     if (!$ok) {
       $pdl->last_error[] = "site_query_execute_failed";
       return array();
     }
     if ($mode=="single") {
       $this->next_record();
       $sites = $this->site_record_setup(TRUE);
     } else {
       while ($this->next_record()) {
         $sites[] = $this->site_record_setup();
       }
       $this->sites = $ok;
     }
     return $sites;
   }

   function site_get($id) {
     return $this->sites_get($id);
   }

   /** Get an alphabetical list of all dive places from all divers
    *  (not to be confused with the dive sites - it is just the location here)
    * @method places_get_all
    * @param optional integer start
    * @param optional integer end
    * @param optional string place
    * @return array places
    */
   function places_get_all($start="",$end="",$showPlace="") {
     GLOBAL $pdl;
     if (empty($showPlace)) { // only return list of loc
       $query = "SELECT loc, count(place) num"
              . "  FROM ".$this->tables->site." s,".$this->tables->userprefs." p"
              . " WHERE s.user_id=p.user_id AND p.name='public' AND p.value=1"
              . " GROUP BY loc";
       if (!$this->dbquery($query)) {
         $pdl->last_error[] = "location_query_failed";
         return array();
       }
       while ($this->next_record()) {
         $p->name = $this->f('loc');
         $p->num  = $this->f('num');
         $list[]    = $p;
       }
     } else {
       $query = "SELECT loc, place, id, s.user_id"
              . "  FROM ".$this->tables->site." s,".$this->tables->userprefs." p"
              . " WHERE s.user_id=p.user_id AND p.name='public' AND p.value=1"
              . "   AND loc='$showPlace'";
       if (!$this->dbquery($query)) {
         $pdl->last_error[] = "location_query_failed";
         return array();
       }
       while ($this->next_record()) {
         $p->name     = $this->f('loc');
         $p->sitename = $this->f('place');
         $p->id       = $this->f('id');
         $uid         = $this->f('user_id');
         $p->diver    = $this->user_logins($uid);
         $list[]      = $p;
       }
     }
     return $list;
   }

   #########################################################################
   # Statistics
   #########################################################################
   /** Get dive statistics
    * @method stats_get
    * @return array stats (properties: max_depth,max_time,avg_depth,cum_time)
    */
   function stats_get() {
     GLOBAL $pdl;
     $user_id = $this->userIDget();
     $query = "SELECT max(depth) max_depth,max(divetime) max_time,"
            . "       avg(depth) avg_depth,sum(divetime) cum_dive_time,"
            . "       avg(divetime) avg_time,count(id) num_dives"
            . "  FROM ".$this->tables->logbook
            . " WHERE user_id=$user_id";
     if (!$this->dbquery($query)) {
       $pdl->last_error[] = "stats_query_failed";
       return array();
     }
     $this->next_record();
     $details = array("max_depth","max_time","avg_depth","cum_dive_time",
                      "avg_time","num_dives");
     foreach ($details as $var) {
       $stats[$var] = $this->f($var);
     }
     $stats["avg_depth"] = round($stats["avg_depth"]/10,1)."m";
     $stats["max_depth"] = $stats["max_depth"]/10 ."m";
     $stats["max_time"] = $pdl->common->time_format($stats["max_time"]);
     $stats["avg_time"] = $pdl->common->time_format(round($stats["avg_time"]));
     $stats["cum_dive_time"] = $pdl->common->time_format($stats["cum_dive_time"]);
     $query = "SELECT count(id) sites FROM ".$this->tables->site." WHERE user_id=$user_id";
     if (!$this->dbquery($query)) {
       $pdl->last_error[] = "stats_query_failed";
       return array();
     }
     $this->next_record();
     $this->sites = $this->f('sites');
     return $stats;
   }

   #########################################################################
   # Temporary wrappers
   #########################################################################
   function get_dives($start="",$end="",$id=FALSE,$sort="",$order="") {
     return $this->dives_get($start,$end,$id,$sort,$order);
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
   function get_sites($start="",$end="",$id=FALSE,$sort="",$order="") {
     return $this->sites_get($start,$end,$id,$sort,$order);
   }
   function get_site($id) {
     return $this->sites_get($id);
   }
   function get_stats() {
     return $this->stats_get();
   }
   function getAllPlaces($start="",$end="",$showPlace="") {
     return $this->places_get_all($start,$end,$showPlace);
   }

 } // end class sql

# $pdl->db = new sql;
?>