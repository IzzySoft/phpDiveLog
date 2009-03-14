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
     <TD WIDTH="32" STYLE="font-size:12px;" ALIGN="right">&nbsp;</TD>
     <TD WIDTH="264" STYLE="font-size:10px;"><DIV ALIGN="center"><B>{location} / {place}</B></DIV></TD>
     <TD WIDTH="32" STYLE="font-size:12px;" ALIGN="right"><B>#{id}</B>&nbsp;</TD>
<!-- END topleftblock -->
<!-- BEGIN toprightblock -->
     <TD WIDTH="32" STYLE="font-size:12px;">&nbsp;<B>#{id}</B></TD>
     <TD WIDTH="296" STYLE="font-size:10px;"><DIV ALIGN="center"><B>{location} / {place}</B></DIV></TD>
     <TD WIDTH="32" STYLE="font-size:12px;" ALIGN="right">&nbsp;</TD>
<!-- END toprightblock -->
    </TR></TABLE></TD></TR></TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <!--TR><TH COLSPAN="2" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{cond_name}</B></DIV></TH></TR-->
 <TR><!-- Second row: Details -->
  <TD WIDTH="328"><TABLE BORDER="0" WIDTH="328"><!-- Left side: Data -->
<!-- BEGIN sumblock -->
   <TR><TD>{item_name}</TD><TD>{item_data}</TD></TR>
<!-- END sumblock -->
   </TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- 3rd row: Notes -->
  <TD COLSPAN="2" WIDTH="328"><TABLE BORDER="0" WIDTH="328">
   <TR><TH WIDTH="328" BGCOLOR="#eeeeee"><DIV ALIGN="center"><B>{notes_name}</B></DIV></TH></TR>
   <TR><TD WIDTH="328" HEIGHT="80" STYLE="text-align:justify;">{notes_text}</TD></TR>
  </TABLE></TD>
 </TR>

 <TR STYLE="font-size:1px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- 4th row: Fotos -->
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

</TABLE>

</BODY></HTML>