<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
   <td class="tab_active"><table class="tab_active" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_active" style="vertical-align:middle"><img src="{tpl_dir}images/dive_flag2.gif" width="20" height="15" alt="DiveFlag"></td>
     <td class="tab_active" style="vertical-align:middle"><a href="{dives_ref}">{dive_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><table class="tab_inactive">
    <tr class="tab_inactive">
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/btn_notes.gif" width="20" height="15" alt="Stats" align="middle"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><table class="tab_inactive">
    <tr>
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Globe" align="middle"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_name}</a></td></tr>
    </tr></table></td>
</table>

</td><td><img src="{tpl_dir}images/logo.gif" height="23" align="right">

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2"><!-- Navigation -->

 <table border="0" cellpadding="2" width="100%" class="transp" style="margin-top:7"><tr>
  <td class="td_transp" align="left" width="12">{nav_left}</td>
  <td align="left" class="td_blank" style="vertical-align:middle;width:12em;">Dive#: {dive#}</td>
  <th class="big" style="vertical-align:middle;"><img src="{tpl_dir}images/dive_flag1.gif" width="23" height="15" alt="Globe" align="middle" style="margin-left:20;">
    <span style="margin-right:20;vertical-align:middle;">{location} / {place}</span></td>
  <td align="right" class="td_blank" style="vertical-align:middle;width:12em;">{date} {time}</td>
  <td class="td_transp" align="right" width="12">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
 <table align="center" border="0" style="margin-top:2px"><!-- Conditions -->
   <tr><th class="head" colspan="3" valign="middle"><img src="{tpl_dir}images/btn_conditions.gif" width="37" height="15" alt="Conditions"> {cond_name}</th></tr>
   <tr><td valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN sumblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END sumblock -->
     </table>
   </td><td>&nbsp;</td><td valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN condblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END condblock -->
     </table>
   </td></tr>
 </table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Equipment -->
 <tr><th class="head" colspan="2" valign="middle"><img src="{tpl_dir}images/btn_equipment.gif" width="37" height="15" alt="Equipment"> {equi_name}</th></tr>
<!-- BEGIN equiblock -->
 <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END equiblock -->
<!-- BEGIN tankblock -->
 <tr><th rowspan="2">{tank_trans} {tank_nr}:</th>
     <td><table border="0">
       <tr><td>{tank_name_name}/{tank_gas_name}</td><td>{tank_name} / {tank_gas}</td></tr>
       <tr><td>{tank_type_name},{tank_volume_name}:</td><td>{tank_type}, {tank_volume}</td></tr>
       <tr><td>{pressure}:</td><td>{tank_in_name}: {tank_in}, {tank_out_name}: {tank_out}</td></tr>
     </table></td></tr>
<!-- END tankblock -->
</table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<!-- BEGIN scheduleblock -->
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Schedule -->
 <tr><th class="head" colspan="4" valign="middle"><img src="{tpl_dir}images/btn_schedule.gif" width="37" height="15" alt="Schedule"> {sched_name}</th></tr>
 <tr>
   <td align="center"><b>{s_depth_name}</b></td>
   <td align="center"><b>{s_time_name}</b></td>
   <td align="center"><b>{s_runtime_name}</b></td>
   <td align="center"><b>{s_gas_name}</b></td>
 </tr>
<!-- BEGIN scheditemblock -->
 <tr>
   <td align="center">{s_depth}</td>
   <td align="right">{s_time}</td>
   <td align="right">{s_runtime}</td>
   <td align="center">{s_gas}</td>
 </tr>
<!-- END scheditemblock -->
</table>
<!-- END scheduleblock -->

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Notes -->
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_notes.gif" width="37" height="15" alt="Notes"> {notes_name}</th></tr>
 <tr><td>{notes_text}</td></tr>
</table>

</td></tr>
</table>
