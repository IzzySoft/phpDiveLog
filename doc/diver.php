<?
 $title = "phpDiveLog: Divers configuration";
 include("inc/include.inc");

 #==========================================[ Setup the Data ]===
# $mailto = smailto("izzysoft@qumran.org","Izzy");
 $input_file = "text/diver.txt";
 $pm = new pagemaker($t,"data.tpl");
 $pm->make_page($input_file,$title);
 include("inc/tab_setup.inc");
 $pdl->tabs->activate("diver",1);
 $pdl->tabs->parse();
 $pm->output();

 include("$inc/footer.inc");
?>
