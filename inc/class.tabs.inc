<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Setup the navigation tabs                                                 #
 #############################################################################

 # $Id$

 /** Tabs for Main Navigation
  * @package Api
  * @class tabs
  * @author Izzy (izzysoft AT qumran DOT org)
  * @copyright (c) 2001-2009 by Itzchak Rehberg and IzzySoft
  */
 class tabs {
   /** Constructor does all the job
    * @constructor tabs
    * @param string segment (logbook|global|loghelp|globalhelp)
    */
   function tabs($segment) {
     GLOBAL $t,$pdl;
     switch(strtolower($segment)) {
       case "logbook" : $url = "buddylist.php"; break;
       case "global"  : $url = "index.php"; break;
       case "loghelp" : $url = "help.php?topic=buddylist"; break;
       case "globalhelp" : $url = "help.php?topic=dive"; break;
     }
     $t->set_block("template","homeblock","home");
     if ( $pdl->config->enable_index ){ // switch-button
       $img = $pdl->link->linkurl($url,"<img src='".$pdl->config->tpl_url."images/tab_switch.gif' alt='switch'>");
       $t->set_var("home_ref",$img);
       $t->parse("home","homeblock");
     }
     if ( $pdl->config->user_pwd ) { // import-button
       $img = $pdl->link->linkurl("import.php","<img src='".$pdl->config->tpl_url."images/tab_import.gif' alt='import'>");
       $t->set_var("home_ref",$img);
       $t->parse("home","homeblock",TRUE);
     }
   }

   /** Add a tab to the tab bar. Each tab will be added as inactive tab
    * @method add_tab
    * @param string name what to show in the tab
    * @param string url where it shall point to
    * @param string img name of the icon to use (file must reside in the templates img dir)
    * @param optional boolean newwin Does the link leave this app? This implicates
             opening it in a new window/tab. Defaults to FALSE
    */
   function add_tab($name,$url,$img,$newwin=FALSE) {
     $this->tab[$name]->url     = $url;
     $this->tab[$name]->active  = FALSE;
     $this->tab[$name]->hideurl = FALSE;
     $this->tab[$name]->img     = $img;
     if ($newwin) $this->tab[$name]->newwin = TRUE;
     $this->tabnames[] = $name;
   }

   /** Mark a tab as active
    * @method activate
    * @param string name which tab to activate (corresponds to name parameter of add_tab)
    * @param optional boolean hideurl whether to remove the href (default: FALSE)
    */
   function activate($name,$hideurl=FALSE) {
     $this->tab[$name]->active = TRUE;
     if ($hideurl) $this->tab[$name]->hideurl = TRUE;
   }

   /** Create the navigation bar
    * @method parse
    */
   function parse() {
     GLOBAL $t,$pdl;
     $tc = count($this->tabnames);
     for ($i=0;$i<$tc;++$i) {
       if ($this->tab[$this->tabnames[$i]]->active) {
         $class = "tab_active"; } else { $class = "tab_inactive"; }
       $t->set_var("tab_class","$class");
       $name = $this->tabnames[$i];
       if ($name=="person") { $sname = ucfirst($pdl->config->user_name);
       } else { $sname = lang("tab_".$name); }
       $t->set_var("tab_img",$this->tab[$name]->img);
       if (!$this->tab[$name]->hideurl) {
         if (isset($this->tab[$name]->newwin)) {
           $name = "<A HREF='JavaScript:open_help(\"".$this->tab[$name]->url."\")'>$sname</A>";
         } else
           $name = $pdl->link->linkurl($this->tab[$name]->url,$sname);
       } else { $name = lang("tab_".$name); }
       $t->set_var("tab_name",$name);
       $t->parse("tab","tabblock",$i);
     }
   }
 } // end class tabs

?>