<?
 $title = "Izzys DiveLog";
 include("inc/includes.inc");
# include("inc/config_internal.inc");
# include("inc/class.csv.inc");
# include("inc/class.db_text.inc");
 include("inc/header.inc");

 $pdl->db = new db_text();
 $divedata = $pdl->db->get_dives(2,3);
 echo "<b>Dive# 2+3:</b><pre>";print_r($divedata);echo "</pre>\n";

/*
 $csv = new csv(";",'"',TRUE);
 $csv->import("data/logbook.csv");
 echo "<b>Imported data:</b><br><pre>";
 print_r($csv->data);
 echo "</pre>\n";
*/
 include("inc/footer.inc");
?>