<?
 $inc = "../inc";
 include("$inc/config.inc");
 include("$inc/config_internal.inc");
 $pdl->config->tpl_url = "../templates/aqua/";
 $pdl->config->db_type = "csv";
 include("$inc/class.link.inc");
 include("$inc/class.template.inc");
 include("inc/class.faq.inc");
 include("$inc/header.inc");
 $t = new Template("templates/");
 include("inc/mailto.inc");

 $mailto = smailto("izzysoft@qumran.org","IzzySoft");
 $intro_page = $pdl->link->linkurl("common.php","Intro");
 $install_page = $pdl->link->linkurl("install.php","Installation Instructions");
 $admin_page = $pdl->link->linkurl("admin.php","Site Administration");
 $diver_page = $pdl->link->linkurl("diver.php","Diver Configuration");
?>
