<?
 include("inc/include.inc");

 #==========================================[ Setup the Data ]===
# $mailto = smailto("izzysoft@qumran.org","Izzy");
 $input_file = "text/install.txt";
 $pm = new pagemaker($t,"data.tpl");
 $pm->make_page($input_file,"phpDiveLog: Installation");
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("install",1);
 $pdl->tabs->parse();
 $pm->output();

 include("$inc/footer.inc");
?>
