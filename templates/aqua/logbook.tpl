<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
   <td><table class="tab_active" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_active" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/dive_flag2.gif" width="20" height="15" alt="DiveFlag"></td>
     <td class="tab_active" style="vertical-align:middle">{dive_tab_name}</td>
    </tr></table></td>
   <td><table class="tab_inactive">
    <tr class="tab_inactive">
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/btn_notes.gif" width="20" height="15" alt="Stats" align="middle"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_name}</a></td>
    </tr></table></td>
   <td><table class="tab_inactive">
    <tr>
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Globe" align="middle"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_name}</a></td></tr>
    </tr></table></td>
</table>

</td><td><img src="{tpl_dir}images/logo.gif" height="23" align="right">

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
 <table class="transp" width="100%"><tr>
  <td class="td_transp" align="left">{nav_left}</td>
  <td class="td_transp" align="right">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">

<table border="1" cellpadding="2" align="center">
 <tr><td></td><th>{date_name}</th><th>{time_name}</th><th>{loc_name}</th><th>{rat_name}</th>
     <th>&nbsp;</th>
     <th>{ddt_name}</td><th>{buddy_name}</th></tr>

<!-- BEGIN itemblock -->
 <tr><th align="right"><a href="{dive_ref}">{dive#}</a></th>
  <td align="center">{date}</td>
  <td align="center">{time}</td>
  <td align="center">{location}:<br>{place}</td>
  <td align="center"><img src="{rating}"></td>
  <th>{pix}</th>
  <td align="center">{depth}<br>{divetime}</td>
  <td align="center">{buddy}</td>
 </tr>
<!-- END itemblock -->

</td></tr></table>
