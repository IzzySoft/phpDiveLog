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

</td></tr><tr class="td_transp"><td class="td_transp">

<table cellpadding="5" align="center" style="background-color:transparent;" border="0">
 <tr class="td_blank"></tr>
</table>
<table border="0" cellpadding="4" align="center">
 <tr><th colspan="5"><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_notes.gif" width="20" height="15" border="0" alt="Stats" align="middle"> {title}</h3></th></tr>
 <tr><th>{max_depth_name}</th><td align="right">{max_depth}</td>
  <td class="td_blank"></td>
  <th>{max_time_name}</th><td align="right">{max_time}</td></tr>
 <tr><th>{avg_depth_name}</th><td align="right">{avg_depth}</td>
  <th class="td_blank"></th>
  <th>{avg_time_name}</th><td align="right">{avg_time}</td></tr>
 <tr><th>{dive_num_name}</th><td align="right">{dive_num}</td>
  <td class="td_blank"></td>
  <th>{cum_time_name}</th><td>{cum_time}</td></tr>
</table>

</td></tr></table>
