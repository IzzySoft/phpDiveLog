<?
 #############################################################################
 # phpDiveLog                                    (c) 2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Setup the navigation tabs                                                 #
 #############################################################################

 # $Id$

 class tabs {
   function tabs() {
     GLOBAL $t,$pdl;
     $t->set_block("template","homeblock","home");
     if ( $pdl->config->enable_index ){
       $img = $pdl->link->linkurl("buddylist.php","<img src='".$pdl->config->tpl_url."images/tab_switch.gif'>");
       $t->set_var("home_ref",$img);
       $t->parse("home","homeblock");
     }
     $this->add_tab("dives","index.php","tab_dives.gif");
     $this->add_tab("stats","stats.php","tab_stats.gif");
     $this->add_tab("sites","sitelist.php","tab_sites.gif");
     if ($pdl->config->display_personal)
       $this->add_tab("person","person.php","tab_person.gif");
     $this->add_tab("prefs","prefs.php","tab_preferences.gif");
     $t->set_block("template","tabblock","tab");
   }

   function add_tab($name,$url,$img) {
     $this->tab[$name]->url     = $url;
     $this->tab[$name]->active  = FALSE;
     $this->tab[$name]->hideurl = FALSE;
     $this->tab[$name]->img     = $img;
     $this->tabnames[] = $name;
   }

   function activate($name,$hideurl=FALSE) {
     $this->tab[$name]->active = TRUE;
     if ($hideurl) $this->tab[$name]->hideurl = TRUE;
   }

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
       if (!$this->tab[$name]->hideurl)
         $name = $pdl->link->linkurl($this->tab[$name]->url,$sname);
	 else $name = lang("tab_".$name);
       $t->set_var("tab_name",$name);
       $t->parse("tab","tabblock",TRUE);
     }
   }
 }

 #================================================[ set up navigation tabs ]===
 $t->set_var("tpl_dir",$pdl->config->tpl_url);
 $pdl->tabs = new tabs;

?>