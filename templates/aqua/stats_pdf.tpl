<HTML><HEAD>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<STYLE><!--
/* === Common === */
  BODY {
   font-family: Verdana,Arial,Helvetica,sans-serif;
   font-size: 8;
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
   font-size: 8;
   font-family: Verdana,Arial,Helvetica,sans-serif;
  }

/* === Images === */

  IMG { border:0; }
//--></STYLE>
</HEAD><BODY>


<TABLE WIDTH="328" BORDER="0" STYLE="empty-cells: show; border-spacing:0; font-size:8; table-layout:fixed;">
 <TR><!-- First row: Header -->
  <TD COLSPAN="2"><TABLE WIDTH="328" STYLE="border:1px solid black;padding-bottom:10px;"><TR><TD><TABLE WIDTH="328" BORDER="0"><TR>
<!-- BEGIN topleftblock -->
     <TD WIDTH="60" STYLE="font-size:12;" ALIGN="right">&nbsp;</TD>
     <TD WIDTH="208" STYLE="font-size:10;"><DIV ALIGN="center"><B>{stats_name}</B></DIV></TD>
     <TD WIDTH="60" STYLE="font-size:8;" ALIGN="right"><B>{date}</B>&nbsp;</TD>
<!-- END topleftblock -->
<!-- BEGIN toprightblock -->
     <TD WIDTH="40" STYLE="font-size:10;">&nbsp;<B>#{date}</B></TD>
     <TD WIDTH="280" STYLE="font-size:10;"><DIV ALIGN="center"><B>{stats_name}</B></DIV></TD>
     <TD WIDTH="40" STYLE="font-size:12;" ALIGN="right">&nbsp;</TD>
<!-- END toprightblock -->
    </TR></TABLE></TD></TR></TABLE></TD>
 </TR>

 <TR STYLE="font-size:20px;"><TD></TD></TR><!-- Spacer for TCPDF -->

 <TR><!-- Second row: Details -->
  <TD WIDTH="162"><TABLE BORDER="0" WIDTH="162"><!-- Left side: Data -->
<!-- BEGIN sumblock -->
   <TR><TD ALIGN="right" WIDTH="111">{item_name}</TD><TD WIDTH="4">&nbsp;</TD><TD WIDTH="51">{item_data}</TD></TR>
<!-- END sumblock -->
   <TR STYLE="font-size:15;"><TD></TD></TR><!-- Spacer for TCPDF -->
   <TR><TD COLSPAN="2" WIDTH="162"><DIV ALIGN="center">
<!-- BEGIN lgraphblock -->
      <B>{graph_name}</B><BR /><IMG SRC="{graph_src}" WIDTH="{graph_width}" HEIGHT="{graph_height}" ALT="{graph_alt}" />&nbsp;
<!-- END lgraphblock -->
   </DIV></TD></TR></TABLE></TD><TD WIDTH="162"><TABLE ALIGN="center"><!-- Right side: Graphs -->
<!-- BEGIN rgraphblock -->
      <TR><TD><B>{graph_name}</B></TD></TR><TR><TD><IMG SRC="{graph_src}" WIDTH="{graph_width}" HEIGHT="{graph_height}" ALT="{graph_alt}" /></TD></TR>
<!-- END rgraphblock -->
   </TABLE></TD>
 </TR>

</TABLE>

</BODY></HTML>