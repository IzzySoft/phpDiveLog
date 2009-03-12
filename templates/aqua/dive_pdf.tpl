<HTML><HEAD>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<STYLE><!--
/* === Common === */
  BODY {
   font-family: Verdana,Arial,Helvetica,sans-serif;
   font-size: 8px;
   background-color: #ffffff;
   color: #0000ee;
  }
  P {
   text-align: justify;
  }

/* === Tables === */

  TABLE, TABLE.outer {
   background-image:url(images/aqua-light.jpg);
   background-attachment:fixed;
   empty-cells: show;
   border:0;
   border-spacing:0;
  }
  TD,TH {
   font-size: 8px;
   font-family: Verdana,Arial,Helvetica,sans-serif;
  }

/* === Images === */

  IMG { border:0; }
//--></STYLE>
</HEAD><BODY>


<TABLE WIDTH="328" BORDER="0" STYLE="empty-cells: show; border-spacing:0; font-size:8px; table-layout:fixed;">
 <TR><!-- First row: Header -->
  <TD COLSPAN="2"><TABLE WIDTH="328" STYLE="border:1px solid black;padding-bottom:10px;"><TR><TD><TABLE WIDTH="328" BORDER="0"><TR>
<!-- BEGIN topleftblock -->
     <TD HEIGHT="25px" WIDTH="72" STYLE="font-size:8px;"> {date}<BR/> {time}</TD>
     <TD WIDTH="184" STYLE="font-size:10px;"><DIV ALIGN="center"><B>{location} / {place}</B></DIV></TD>
     <TD WIDTH="72" STYLE="font-size:12px;" ALIGN="right"><B>#{dive#}</B>&nbsp;</TD>
<!-- END topleftblock -->
<!-- BEGIN toprightblock -->
     <TD WIDTH="72" STYLE="font-size:12px;">&nbsp;<B>#{dive#}</B></TD>
     <TD WIDTH="184" STYLE="font-size:10px;"><DIV ALIGN="center"><B>{location} / {place}</B></DIV></TD>
     <TD HEIGHT="25px" WIDTH="72" ALIGN="right" STYLE="font-size:8px;">{date} <BR/>{time}&nbsp;</TD>
<!-- END toprightblock -->
    </TR></TABLE></TD></TR></TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><TH COLSPAN="2" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{cond_name}</B></DIV></TH></TR>
 <TR><!-- Second row: Details -->
  <TD WIDTH="144"><TABLE BORDER="0" WIDTH="144"><!-- Left side: Data -->
<!-- BEGIN sumblock -->
   <TR><TD>{item_name}</TD><TD>{item_data}</TD></TR>
<!-- END sumblock -->
   </TABLE></TD><!-- Right side: Profile Image -->
  <TD WIDTH="184" ALIGN="right"><TABLE WIDTH="184" BORDER="0">
   <!--TR STYLE="font-size:1px;"><TD></TD></TR-->
   <TR><TD ALIGN="right" WIDTH="184"><IMG SRC="{prof_img}" ALT="Profile" WIDTH="180" HEIGHT="90" BORDER="0" /></TD></TR>
  </TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- Third row: Equipment -->
  <TD WIDTH="328"><TABLE BORDER="0" WIDTH="328">
   <TR><TH COLSPAN="2" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{equi_name}</B></DIV></TH></TR>
<!-- BEGIN equiblock -->
   <TR><TD WIDTH="72">{item_name}</TD><TD WIDTH="246">{item_data}</TD></TR>
<!-- END equiblock -->
   <TR><TD WIDTH="72">{tank_trans} {tank_nr}:</TD><TD WIDTH="246"><TABLE BORDER="0" WIDTH="246">
<!-- BEGIN tankblock -->
    <TR><TD WIDTH="72">{tank_name_name}/{tank_gas_name}:</TD><TD WIDTH="174">{tank_name} / {tank_gas}</TD></TR>
    <TR><TD WIDTH="72">{tank_type_name},{tank_volume_name}:</TD><TD WIDTH="174">{tank_type}, {tank_volume}</TD></TR>
    <TR><TD WIDTH="72">{pressure}:</TD><TD WIDTH="174">{tank_in_name}: {tank_in}, {tank_out_name}: {tank_out}</TD></TR>
<!-- END tankblock -->
   </TABLE></TD></TR></TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- 4th row: Notes -->
  <TD COLSPAN="2" WIDTH="328"><TABLE BORDER="0" WIDTH="328">
   <TR><TH WIDTH="328" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{notes_name}</B></DIV></TH></TR>
   <TR><TD WIDTH="328" HEIGHT="80" STYLE="text-align:justify;">{notes_text}</TD></TR>
  </TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- 5th row: Fotos -->
  <TD COLSPAN="2" WIDTH="328"><TABLE BORDER="0" WIDTH="328">
   <TR><TH WIDTH="328" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{fotos_name}</B></DIV></TH></TR>
   <TR><TD><TABLE WIDTH="328" CELLPADDING="5" BORDER="0"><TR>
<!-- BEGIN fotoblock -->
    <TD STYLE="font-size:7px;" HEIGHT="120"><DIV ALIGN="center"><IMG SRC="{foto}" ALIGN="center" WIDTH="100" HEIGTH="75" /><BR />{foto_text}</DIV></TD>
<!-- END fotoblock -->
<!-- BEGIN nofotoblock -->
    <TD HEIGHT="120"></TD>
<!-- END nofotoblock -->
   </TR></TABLE></TD></TR>
  </TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><TH COLSPAN="2" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{mark_name}</B></DIV></TH></TR>
 <TR><!-- 6th row: Marks and signatures -->
  <TD><TABLE BORDER="0" WIDTH="328" CELLPADDING="5"><TR>
   <TD><TABLE BORDER="1" WIDTH="100%" HEIGHT="50px"><TR><TD HEIGHT="50px"></TD></TR></TABLE><DIV ALIGN="center">{mark1_name}</DIV></TD>
   <TD><TABLE BORDER="1" WIDTH="100%" HEIGHT="50px"><TR><TD HEIGHT="50px"></TD></TR></TABLE><DIV ALIGN="center">{mark2_name}</DIV></TD>
   <TD><TABLE BORDER="1" WIDTH="100%" HEIGHT="50px"><TR><TD HEIGHT="50px"></TD></TR></TABLE><DIV ALIGN="center">{mark3_name}</DIV></TD>
  </TR></TABLE></TD>
 </TR>

</TABLE>

</BODY></HTML>