<table align="center"  style="background-color:transparent;" border="0"><tr class="td_blank"><td class="td_blank">

<table cellpadding="0" align="left" style="background-color:transparent;" border="1">
 <tr style="background-color:transparent;">
   <td class="tab_active"><table class="tab_active" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_active" style="vertical-align:middle"><a href="{dives_ref}">{dive_tab_img}</a></td>
     <td class="tab_active" style="vertical-align:middle"><a href="{dives_ref}">{dive_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><a href="stats.php"><table class="tab_inactive" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_img}</a></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><table class="tab_inactive" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_inactive" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_img}</a></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_name}</a></td></tr>
    </tr></table></td>
</table>

</td></tr><tr class="td_blank"><td class="td_blank"><!-- Navigation -->
 <table border="0" cellpadding="2" width="100%" style="background-color:transparent;"><tr>
  <td class="td_blank" colspan="5" height="5"></td></tr><tr>
  <td class="td_blank" align="left" width="12">{nav_left}</td>
  <td align="left" class="td_big" style="vertical-align:middle">Dive#: {dive#}</td>
  <td width="20"></td>
  <td style="vertical-align:middle;background-color:transparent;">{loc_img}</td>
  <td class="td_big" style="vertical-align:middle;background-color:transparent;"><h2>{location} / {place}</h2></td>
  <td width="20"></td>
  <td align="right" class="td_big" style="vertical-align:middle">{date} {time}</td>
  <td class="td_blank" align="right" width="12">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_blank"><td class="td_blank">
 <table align="center" border="0" cellpadding="2"><!-- Conditions -->
   <tr><th colspan="3" valign="middle"><h3>{cond_img} {cond_name}</h3></th></tr>
   <tr class="td_blank"><td class="td_blank" valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN sumblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END sumblock -->
     </table>
   </td><td class="td_blank">&nbsp;</td><td class="td_blank" valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN condblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END condblock -->
     </table>
   </td></tr>
 </table>

</td></tr><tr class="td_blank"><td class="td_blank">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Equipment -->
 <tr><th colspan="2" valign="middle"><h3>{equi_img} {equi_name}</h3></th></tr>
<!-- BEGIN equiblock -->
 <tr><td><b>{item_name}</b></td><td>{item_data}</td></tr>
<!-- END equiblock -->
{tank}
</table>

</td></tr><tr class="td_blank"><td class="td_blank">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Notes -->
 <tr><th valign="middle"><h3>{notes_img} {notes_name}</h3></th></tr>
 <tr><td>{notes_text}</td></tr>
</table>

</td></tr>
</table>
