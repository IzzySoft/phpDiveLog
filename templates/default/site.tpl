<table align="center"  style="background-color:transparent;" border="0"><tr class="td_transp"><td class="td_transp">

<table cellpadding="0" align="left" style="background-color:transparent;" border="1">
 <tr style="background-color:transparent;">
   <td class="tab_inactive"><table class="tab_inactive" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_inactive" style="vertical-align:middle"><a href="{dives_ref}"><IMG SRC="{tpl_dir}images/dive_flag2.gif" width="20" height="15" alt="DiveFlag"></a></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{dives_ref}">{dive_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_inactive"><a href="stats.php"><table class="tab_inactive" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}"><IMG SRC="{tpl_dir}images/btn_notes.gif" width="20" height="15" alt="Stats" align="middle"></a></td>
     <td class="tab_inactive" style="vertical-align:middle"><a href="{stats_ref}">{stats_tab_name}</a></td>
    </tr></table></td>
   <td class="tab_active"><table class="tab_active" border="0">
    <tr style="background-color:transparent;">
     <td class="tab_active" style="vertical-align:middle"><a href="{sites_ref}"><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Globe" align="middle"></a></td>
     <td class="tab_active" style="vertical-align:middle"><a href="{sites_ref}">{sites_tab_name}</a></td></tr>
    </tr></table></td>
</table>

</td></tr><tr class="td_transp"><td class="td_transp">
 <table border="0" cellpadding="2" width="100%" style="background-color:transparent;"><tr>
  <td class="td_transp" align="left">{nav_left}</td>
  <td class="td_transp" align="right">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp">

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

<!-- BEGIN fotoblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table align="center" border="0" cellpadding="2" style="margin-top:2px"><!-- Fotos -->
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_fotos.gif" width="37" height="15" alt="Fotos"> {fotos_name}</th></tr>
 <tr><td align="center"><table><tr>
<!-- BEGIN fotoitemblock -->
   <td><span class="thumbnail"><img src="{foto}" align="center"><br>{fdesc}</span></td>
<!-- END fotoitemblock -->
   </tr></table></td></tr>
</table>
<!-- END fotoblock -->

</td></tr></table>