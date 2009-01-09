<table align="center"  style="background-color:transparent;" border="0"><tr class="td_transp"><td class="td_transp">

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

</td></tr><tr class="td_transp"><td class="td_transp"><!-- Navigation -->
 <table border="0" cellpadding="2" width="100%" style="background-color:transparent;"><tr>
  <td class="td_transp" colspan="5" height="5"></td></tr><tr>
  <td class="td_transp" align="left" width="12">{nav_left}</td>
  <td align="left" class="td_big" style="vertical-align:middle;width:12em;">{divenr}: {dive#}</td>
  <td class="td_blank" style="vertical-align:middle;text-align:right;"><img src="{tpl_dir}images/dive_flag1.gif" width="23" height="15" alt="Globe" valign="middle" style="margin-left:20;"></td>
  <td class="td_blank" style="vertical-align:middle;"><h2 style="margin-bottom:0;margin-right:20;">{location} / {place}</h2></td>
  <td align="right" class="td_big" style="vertical-align:middle;width:12em;">{date} {time}</td>
  <td class="td_transp" align="right" width="12">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp">
 <table align="center" border="0" cellpadding="2"><!-- Conditions -->
   <tr><th colspan="3" valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_conditions.gif" width="37" height="15" alt="Conditions"> {cond_name}</h3></th></tr>
   <tr class="td_transp"><td class="td_transp" valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN sumblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END sumblock -->
     </table>
   </td><td class="td_blank">&nbsp;</td><td class="td_transp" valign="top">
     <table align="center" border="0" cellpadding="2">
<!-- BEGIN condblock -->
       <tr><th>{item_name}</th><td>{item_data}</td></tr>
<!-- END condblock -->
     </table>
   </td></tr>
 </table>

</td></tr><tr class="td_transp"><td class="td_transp">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Equipment -->
 <tr><th colspan="2" valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_equipment.gif" width="37" height="15" alt="Equipment"> {equi_name}</h3></th></tr>
<!-- BEGIN equiblock -->
 <tr><td><b>{item_name}</b></td><td>{item_data}</td></tr>
<!-- END equiblock -->
<!-- BEGIN tankblock -->
 <tr><td rowspan="2"><b>{tank_trans} {tank_nr}:</b></td>
     <td><table border="0">
       <tr><td>{tank_name_name}/{tank_gas_name}</td><td>{tank_name} / {tank_gas}</td></tr>
       <tr><td>{tank_type_name},{tank_volume_name}:</td><td>{tank_type}, {tank_volume}</td></tr>
       <tr><td>{pressure}:</td><td>{tank_in_name}: {tank_in}, {tank_out_name}: {tank_out}</td></tr>
     </table></td></tr>
<!-- END tankblock -->
</table>

<!-- BEGIN scheduleblock -->
</td></tr><tr class="td_transp"><td class="td_transp">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Schedule -->
 <tr><th valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_schedule.gif" width="37" height="15" alt="Schedule"> {sched_name}</h3></th></tr>
<!-- BEGIN schedimageblock -->
 <tr>
   <td align="center"><span class="thumbnail" style="margin:0"><img src="{sched_img}" alt="DiveSchedule"></span></td>
 </tr>
<!-- END schedimageblock -->
 <tr><td><table align="center" border="0" cellpadding="2">
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
 </table></td></tr>
</table>
<!-- END scheduleblock -->

<!-- BEGIN profileblock -->
</td></tr><tr class="td_transp"><td class="td_transp"><!-- Profile -->
<table align="center" border="0" cellpadding="2" style="margin-top:2px">
 <tr><th valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_profile.gif" width="37" height="15" alt="Fotos"> {prof_name}</h3></th></tr>
 <tr><td><span class="thumbnail" style="margin:0">{prof_map}<img src="{prof_img}" alt="DiveProfile" {use_map}></span></td></tr>
</table>
<!-- END profileblock -->

<!-- BEGIN notesblock -->
</td></tr><tr class="td_transp"><td class="td_transp"><!-- Notes -->
<table align="center" border="0" cellpadding="2" style="margin-top:2px">
 <tr><th valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_notes.gif" width="37" height="15" alt="Notes"> {notes_name}</h3></th></tr>
 <tr><td class="notes">{notes_text}</td></tr>
</table>
<!-- END notesblock -->

<!-- BEGIN fotoblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Fotos -->
 <tr><th valign="middle"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_fotos.gif" width="37" height="15" alt="Fotos"> {fotos_name}</h3></th></tr>
 <tr><td align="center"><table><tr>
<!-- BEGIN fotoitemblock -->
   <td><span class="thumbnail">{bigref}<img src="{foto}" align="center">{unref}<br>{fdesc}</span></td>
<!-- END fotoitemblock -->
<!-- BEGIN multifotoblock -->
   </tr><tr>
<!-- END multifotoblock -->
   </tr></table></td></tr>
</table>
<!-- END fotoblock -->

</td></tr>
</table>
