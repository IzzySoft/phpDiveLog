<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
   <td class="tab_inactive"><table class="tab_inactive">
    <tr class="tab_inactive">
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/dive_flag2.gif" width="20" height="15" alt="DiveFlag"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{dives_ref}">{dive_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><table class="tab_inactive">
    <tr class="tab_inactive">
     <td class="tab_inactive" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/btn_notes.gif" width="20" height="15" alt="Stats" align="middle"></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_active"><table class="tab_active" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_active" style="vertical-align:middle"><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Globe" align="middle"></td>
     <td class="tab_active" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_name}</a></td></tr>
    </tr></table></td>
</table>

</td><td><img src="{tpl_dir}images/logo.gif" height="23" align="right">

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
 <table class="transp" width="100%"><tr>
  <td class="td_transp" align="left">{nav_left}</td>
  <td class="td_transp" align="right">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">

<table cellpadding="2" border="0" align="center">
 <tr><th colspan="5"><h3><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Conditions"> {loc}: {place}</h3></th></tr>
 <tr>
  <th style="text-align:right">{lat_name}</th> <td>{latitude}</td>
  <td class="td_transp"></td>
  <th style="text-align:right">{long_name}</th> <td>{longitude}</td>
 </tr><tr>
  <th style="text-align:right">{alt_name}</th> <td>{altitude}</td>
  <td class="td_transp"></td>
  <th style="text-align:right">{md_name}</th> <td>{depth}</td>
 </tr>
</table>
<br>
<table cellpadding="2" border="0" align="center">
 <tr><th>{notes_name}</th></tr>
 <tr><td>{description}</td></tr>
</table>

</td></tr></table>