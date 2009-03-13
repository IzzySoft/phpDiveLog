<?
 #############################################################################
 # phpDiveLog                               (c) 2004-2007 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Setup the navigation tabs                                                 #
 #############################################################################

 # $Id$

 class tabs {
   function tabs() {
     GLOBAL $pm, $pdl;
     $pm->t->set_block("main","homeblock","home");
     $this->add_tab("common","common.php","tab_dives.gif");
     $this->add_tab("admin","admin.php","tab_stats.gif");
     $this->add_tab("diver","diver.php","tab_buddylist.gif");
     $this->add_tab("visitor","visitor.php","tab_usage.gif");
     $pm->t->set_block("main","tabblock","tab");
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
     GLOBAL $pm,$pdl;
     $tc = count($this->tabnames);
     for ($i=0;$i<$tc;++$i) {
       if ($this->tab[$this->tabnames[$i]]->active) {
         $class = "tab_active"; } else { $class = "tab_inactive"; }
       $pm->t->set_var("tab_class","$class");
       $name = $this->tabnames[$i];
       $sname = ucfirst($name);
       $pm->t->set_var("tab_img",$this->tab[$name]->img);
       if (!$this->tab[$name]->hideurl)
         $name = $pdl->link->linkurl($this->tab[$name]->url,$sname);
	 else $name = ucfirst($name);
       $pm->t->set_var("tab_name",$name);
       $pm->t->parse("tab","tabblock",$i);
     }
   }
 }

 #================================================[ set up navigation tabs ]===
 $pm->t->set_var("tpl_dir","../templates/aqua/");
 $pdl->tabs = new tabs;

?>