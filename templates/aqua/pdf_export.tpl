<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
<!-- BEGIN homeblock -->
   <td class="tab_inactive">{home_ref}</td>
<!-- END homeblock -->
<!-- BEGIN tabblock -->
   <td><table class="{tab_class}">
    <tr class="{tab_class}">
     <td class="{tab_class}" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/{tab_img}" alt="TabImg"></td>
     <td class="{tab_class}" style="vertical-align:middle">{tab_name}</td>
    </tr></table></td>
<!-- END tabblock -->
 </tr>
</table>

</td><td><img src="{tpl_dir}images/logo.gif" height="23" align="right">
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">

<table class="outer" cellspacing="0" cellpadding="2" align="center" style="margin-top:7;">
 <tr>
     <th class="head" colspan="7"><img src="{tpl_dir}images/apdf.png" width="16" height="16" alt="PDF"> {ptitle}</th>
 </tr>
</table>

<!-- BEGIN formblock -->
<form name="{formname}" action="{formtarget}" method="{formmethod}">
<table class="outer" cellpadding="2" cellspacing="0" align="center" style="margin-top:4px">
 <tr><th class="head" valign="middle" colspan="2"><img src="{icon_src}" width="{icon_width}" height="{icon_height}" alt="{icon_alt}"> {segment_name}</th></tr>
 <tr title="{pages_bubble}"><td>{fsel_name}</td><td><input type="text" name="from" size="4">&nbsp;-&nbsp;<input type="text" name="to" size="4"></td></tr>
 <tr><th class="sub" colspan="2">{export_options}</th></tr>
 <tr><td title="{output_bubble}">{foutput_name}</td><td><span title="{output_view_bubble}"><input type="radio" name="duplex" value="off" {foutput_viewcheck}>&nbsp;{foutput_viewname}</span><br>
     <span title="{output_innergutter_bubble}"><input type="radio" name="duplex" value="inner" {foutput_innercheck}>&nbsp;{foutput_innername}</span><br>
     <span title="{output_outergutter_bubble}"><input type="radio" name="duplex" value="outer" {foutput_outercheck}>&nbsp;{foutput_outername}</span><br>
     <span title="{output_sidegutter_bubble}"><input type="radio" name="duplex" value="side" {foutput_sidecheck}>&nbsp;{foutput_sidename}</span></td></tr>
 <tr><td title="{foto_bubble}">{ffoto_name}</td><td><span title="{foto_yes_bubble}"><input type="radio" name="foto" value="1" {ffoto_yescheck}>&nbsp;{fyes}&nbsp;</span>
     <span title="{foto_no_bubble}"><input type="radio" name="foto" value="0" {ffoto_nocheck}>&nbsp;{fno}</span></td></tr>
<!-- BEGIN missingblock -->
 <tr><td title="{missing_bubble}">{fmissing_name}</td><td><span title="{missing_yes_bubble}"><input type="radio" name="graph" value="1" {fmissing_check}>&nbsp;{fyes}&nbsp;</span>
     <span title="{missing_no_bubble}"><input type="radio" name="graph" value="0" {fmissing_nocheck}>&nbsp;{fno}</span></td></tr>
<!-- END missingblock -->
 <tr><th class="sub" colspan="2">{export_head}</th></tr>
 <tr><td colspan="2" align="center"><input type="submit" name="{submit_name}" value="{submit1_value}" title="{submit1_title}">&nbsp;<input type="submit" name="{submit_name}" value="{submit2_value}" title="{submit2_title}">&nbsp;<input type="submit" name="{submit_name}" value="{submit3_value}" title="{submit3_title}"></td></tr>
 <tr><th class="sub" colspan="2">{create_head}</th></tr>
 <tr><td title="{empty_divesheet_bubble}" colspan="2" align="center"><input type="text" name="dsheets" size="4">&nbsp;
     <input type="submit" name="{submit_name}" value="{submit4_value}"></td></tr>
</table>
</form>
<!-- END formblock -->

</td></tr></table>