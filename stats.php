<?
 $title = "Izzys Dive LogBook";
 include("inc/includes.inc");
 include("inc/header.inc");

 $t = new Template($pdl->config->tpl_path);
 $t->set_file(array("template"=>"stats.tpl"));

 #=========================================[ Icons for the navigation tabs ]===
 $t->set_var("dive_tab_img",'<img src="'.$pdl->config->tpl_url.'images/dive_flag2.gif" width="20" height="15" border="0" alt="DiveFlag">');
 $t->set_var("dive_tab_name","Dives");
 $t->set_var("dives_ref","index.php");
 $t->set_var("stats_tab_img",'<img src="'.$pdl->config->tpl_url.'images/btn_notes.gif" width="20" height="15" border="0" alt="Stats" align="middle">');
 $t->set_var("stats_tab_name","Stats");
 $t->set_var("sites_tab_img",'<img src="'.$pdl->config->tpl_url.'images/globe.gif" width="15" height="15" border="0" alt="Globe" align="middle">');
 $t->set_var("sites_tab_name","Sites");
 $t->set_var("sites_ref","sitelist.php");


 $t->pparse("out","template");

 virtual("dives/index.html");
 echo "</td></tr></table>\n";

 include("inc/footer.inc");
?>