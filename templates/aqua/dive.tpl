<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
<!-- BEGIN tabblock -->
   <td><table class="{tab_class}">
    <tr class="{tab_class}">
     <td class="{tab_class}" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/{tab_img}" width="20" height="15" alt="DiveFlag"></td>
     <td class="{tab_class}" style="vertical-align:middle">{tab_name}</td>
    </tr></table></td>
<!-- END tabblock -->
 </tr>
</table>

</td><td><img src="{tpl_dir}images/logo.gif" height="23" align="right">

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2"><!-- Navigation -->

 <table class="outer" border="0" cellpadding="2" cellspacing="0" width="100%" style="margin-top:7"><tr>
  <td class="tdl" align="left" width="12">{nav_left}</td>
  <td align="left" class="td_blank" style="vertical-align:middle;width:12em;">{divenr}: {dive#}</td>
  <th class="big" style="vertical-align:middle;"><img src="{tpl_dir}images/dive_flag1.gif" width="23" height="15" alt="Globe" valign="middle" style="margin-left:20;">
    <span style="margin-right:20;vertical-align:middle;">{location} / {place}</span></td>
  <td align="right" class="td_blank" style="vertical-align:middle;width:12em;">{date} {time}</td>
  <td class="tdl" align="right" width="12">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
 <table class="outer" align="center" border="0" cellpadding="0" cellspacing="0" style="margin-top:3px;"><!-- Conditions -->
   <tr><th class="head" colspan="3" valign="middle"><img src="{tpl_dir}images/btn_conditions.gif" width="37" height="15" alt="Conditions"> {cond_name}</th></tr>
   <tr><td valign="top">
     <table align="center" border="0" cellpadding="2" cellspacing="0">
<!-- BEGIN sumblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END sumblock -->
     </table>
   </td><td>&nbsp;</td><td valign="top">
     <table align="center" border="0" cellpadding="2" cellspacing="0">
<!-- BEGIN condblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END condblock -->
     </table>
   </td></tr>
 </table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table class="outer" align="center" border="0" cellpadding="2" cellspacing="0" style="margin-top:3px"><!-- Equipment -->
 <tr><th class="head" colspan="2" valign="middle"><img src="{tpl_dir}images/btn_equipment.gif" width="37" height="15" alt="Equipment"> {equi_name}</th></tr>
<!-- BEGIN equiblock -->
 <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END equiblock -->
<!-- BEGIN tankblock -->
 <tr><th rowspan="2">{tank_trans} {tank_nr}:</th>
     <td><table border="0">
       <tr><td><b>{tank_name_name}/{tank_gas_name}:</b></td><td>{tank_name} / {tank_gas}</td></tr>
       <tr><td><b>{tank_type_name},{tank_volume_name}:</b></td><td>{tank_type}, {tank_volume}</td></tr>
       <tr><td><b>{pressure}:</b></td><td>{tank_in_name}: {tank_in}, {tank_out_name}: {tank_out}</td></tr>
     </table></td></tr>
<!-- END tankblock -->
</table>

<!-- BEGIN scheduleblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table class="outer" align="center" border="0" cellpadding="2" cellspacing="0" style="margin-top:3px"><!-- Schedule -->
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

<!-- BEGIN profileblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2"><!-- Profile -->
<table class="outer" align="center" border="0" cellpadding="0" cellspacing="0" style="margin-top:3px">
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_profile.gif" width="37" height="15" alt="Fotos"> {prof_name}</th></tr>
 <tr><td><span class="thumbnail" style="margin:0"><img src="{prof_img}" alt="DiveProfile"></span></td></tr>
</table>
<!-- END profileblock -->

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2"><!-- Notes -->
<table class="outer" align="center" border="0" cellpadding="2" cellspacing="0" style="margin-top:3px">
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_notes.gif" width="37" height="15" alt="Notes"> {notes_name}</th></tr>
 <tr><td>{notes_text}</td></tr>
</table>

<!-- BEGIN fotoblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table class="outer" align="center" border="0" cellpadding="2" cellspacing="0" style="margin-top:3px"><!-- Fotos -->
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_fotos.gif" width="37" height="15" alt="Fotos"> {fotos_name}</th></tr>
 <tr><td align="center"><table><tr>
<!-- BEGIN fotoitemblock -->
   <td><span class="thumbnail"><img src="{foto}" align="center"><br>{fdesc}</span></td>
<!-- END fotoitemblock -->
   </tr></table></td></tr>
</table>
<!-- END fotoblock -->

</td></tr>
</table>
