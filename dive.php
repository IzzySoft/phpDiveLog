<?
 $title = "Izzys Dive LogBook: Dive# $nr";
 include("inc/includes.inc");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"dive.tpl"));
 $t->set_block("template","sumblock","sum");
 $t->set_block("template","condblock","cond");
 $t->set_block("template","equiblock","equi");

 #=========================================[ Icons for the navigation tabs ]===
 $t->set_var("dive_tab_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag2.gif" width="20" height="15" border="0" alt="DiveFlag">');
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_img",'<img src="'.$pdl->config->tpl_url.'images/btn_notes.gif" width="20" height="15" border="0" alt="Stats" align="middle">');
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("stats_ref","stats.php");
 $t->set_var("sites_tab_img",'<img src="'.$pdl->config->tpl_url.'images/globe.gif" width="15" height="15" border="0" alt="Globe" align="middle">');
 $t->set_var("sites_tab_name","Sites");
 $t->set_var("sites_ref","sitelist.php");

 #=================================================[ general template data ]===
 $t->set_var("cond_img","<img src='".$pdl->config->tpl_url."images/btn_conditions.gif' width='37' height='15' alt='Conditions'>");
 $t->set_var("cond_name","Conditions");
 $t->set_var("notes_img","<img src='".$pdl->config->tpl_url."images/btn_notes.gif' width='37' height='15' alt='Notes'>");
 $t->set_var("notes_name","Notes");
 $t->set_var("equi_img","<img src='".$pdl->config->tpl_url."images/btn_equipment.gif' width='37' height='15' alt='Equipment'>");
 $t->set_var("equi_name","Equipment");

 #==============================================[ Import dive data from DB ]===
 $dive = $pdl->db->get_dive($nr);

 #=============================================[ set up the navigation bar ]===
 if ($prev=$dive["prev_dive#"]) {
   $t->set_var("nav_left","<a href='$PHP_SELF?nr=$prev'><img src='".$pdl->config->tpl_url."images/left.gif'></a>");
 } else {
   $t->set_var("nav_left","<img src='".$pdl->config->tpl_url."images/left-grey.gif'>");
 }
 if ($next=$dive["next_dive#"]) {
   $t->set_var("nav_right","<a href='$PHP_SELF?nr=$next'><img src='".$pdl->config->tpl_url."images/right.gif'></a>");
 } else {
   $t->set_var("nav_right","<img src='".$pdl->config->tpl_url."images/right-grey.gif'>");
 }
 #==================================================[ set up the dive data ]===
 $t->set_var("dive#",$dive["dive#"]);
 $t->set_var("time",$dive["time"]);
 $t->set_var("date",$dive["date"]);
 $t->set_var("location",$dive["location"]);
 $t->set_var("place",$dive["place"]);
 #--------------------------[ Summary ]---
 $t->set_var("item_name","Max. Depth:");
 $t->set_var("item_data",$dive["depth"]);
 $t->parse("sum","sumblock");
 $t->set_var("item_name","Dive Time:");
 $t->set_var("item_data",$dive["divetime"]);
 $t->parse("sum","sumblock",TRUE);
 $t->set_var("item_name","Buddy:");
 $t->set_var("item_data",$dive["buddy"]);
 $t->parse("sum","sumblock",TRUE);
 $t->set_var("item_name","Rating:");
 $t->set_var("item_data","<img src='".$pdl->config->tpl_url."images/".$dive["rating"]."star.gif"."' alt='Rating:".$dive["rating"]."'");
 $t->parse("sum","sumblock",TRUE);
 #--------------------------[ Conditions ]---
 $t->set_var("item_name","Visibility:");
 $t->set_var("item_data",$dive["visibility"]);
 $t->parse("cond","condblock");
 $t->set_var("item_name","Water Temp.:");
 $t->set_var("item_data",$dive["watertemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Air Temp.:");
 $t->set_var("item_data",$dive["airtemp"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Current:");
 $t->set_var("item_data",$dive["current"]);
 $t->parse("cond","condblock",TRUE);
 $t->set_var("item_name","Workload:");
 $t->set_var("item_data",$dive["workload"]);
 $t->parse("cond","condblock",TRUE);
 #---------------------------[ Equipment ]---
 $t->set_var("item_name","Suit:");
 $t->set_var("item_data",$dive["suittype"].", ".$dive["suitname"]);
 $t->parse("equi","equiblock");
 $t->set_var("item_name","Weight:");
 $t->set_var("item_data",$dive["weight"]);
 $t->parse("equi","equiblock",TRUE);
 $filename = "dives/dive".sprintf("%05d",$nr).".html";
 $fp = fopen($filename, "rb");
 $buffer = fread($fp, filesize($filename));
 fclose($fp);
 $tank = preg_replace("/\r?\n|\r/", "\n", $buffer);
 $t->set_var("tank",$tank);
 #----------------------------[ Schedule ]---
 # inside the tank for now
 #-------------------------------[ Notes ]---
 $t->set_var("notes_text",nl2br($dive["notes"]));

 $t->set_var("loc_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag1.gif" width="23" height="15" border="0" alt="Globe" align="middle">');
 $t->pparse("out","template");

 include("inc/footer.inc");
?>