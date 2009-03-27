<table align="center" style="background-color:transparent;" border="0"><tr class="td_transp"><td class="td_transp">

<table cellpadding="0" align="left" style="background-color:transparent;" border="1">
 <tr style="background-color:transparent;">
<!-- BEGIN homeblock -->
   <td class="tab_inactive">{home_ref}</td>
<!-- END homeblock -->
<!-- BEGIN tabblock -->
   <td class="{tab_class}"><table class="{tab_class}" border="0">
    <tr style="background-color:transparent;">
     <td class="{tab_class}" style="vertical-align:middle"><img src="{tpl_dir}images/{tab_img}" alt="TabImg"></td>
     <td class="{tab_class}" style="vertical-align:middle">{tab_name}</td>
    </tr></table></td>
<!-- END tabblock -->
 </tr>
</table>

</td></tr><tr class="td_transp"><td class="td_transp">

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
 <tr><td title="{output_bubble}">{foutput_name}</td><td><input type="radio" name="duplex" value="off" {foutput_viewcheck} title="{output_view_bubble}">&nbsp;{foutput_viewname}<br>
     <input type="radio" name="duplex" value="inner" {foutput_innercheck} title="{output_innergutter_bubble}">&nbsp;{foutput_innername}<br>
     <input type="radio" name="duplex" value="outer" {foutput_outercheck} title="{output_outergutter_bubble}">&nbsp;{foutput_outername}<br>
     <input type="radio" name="duplex" value="side" {foutput_sidecheck} title="{output_sidegutter_bubble}">&nbsp;{foutput_sidename}</td></tr>
 <tr><td title="{foto_bubble}">{ffoto_name}</td><td><input type="radio" name="foto" value="1" {ffoto_yescheck} title="{foto_yes_bubble}">&nbsp;{fyes}&nbsp;
     <input type="radio" name="foto" value="0" {ffoto_nocheck} title="{foto_no_bubble}">&nbsp;{fno}</td></tr>
 <tr><td colspan="2" align="center"><input type="submit" name="{submit_name}" value="{submit_value}"></td></tr>
</table>
</form>
<!-- END formblock -->

</td></tr></table>