<?
 include("inc/include.inc");

 #==========================================[ Setup the Data ]===
# $mailto = smailto("izzysoft@qumran.org","Izzy");
 $input_file = "text/common.txt";
 $pm = new pagemaker($t,"data.tpl");
 $pm->make_page($input_file,"phpDiveLog: Common Information");
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("common",1);
 $pdl->tabs->parse();
 $pm->output();

 include("$inc/footer.inc");
?>
