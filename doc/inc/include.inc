<?
 $inc = "../inc";
 $pdl->config->tpl_url = "../templates/aqua/";
 include("$inc/class.link.inc");
 include("$inc/class.template.inc");
 include("inc/class.faq.inc");
 include("$inc/header.inc");
 $t = new Template("templates/");
 include("inc/mailto.inc");

 $mailto = smailto("izzysoft@qumran.org","IzzySoft");
 $pdl->config->version = "0.2.0";

?>
