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

<table class="outer" border="1" cellpadding="2" align="center" style="margin-top:7">
<!-- BEGIN placeblock -->
 <tr><td>{nav_left}</td><th>{place_name}</th><th>{hit_name}</th><td align="right">{nav_right}</td></tr>
<!-- BEGIN pitemblock -->
 <tr><th>&nbsp;</th>
  <td align="center">{place}</td>
  <td align="right">{hits}</td>
  <th>&nbsp;</th>
 </tr>
<!-- END pitemblock -->
<!-- END placeblock -->

<!-- BEGIN siteblock -->
 <tr><td>{nav_left}</td><th>{place_name}</th><th>{site_name}</th><th>{hit_name}</th><td align="right">{nav_right}</td></tr>
<!-- BEGIN sitemblock -->
 <tr><th>&nbsp;</th>
  <td align="center">{place}</td>
  <td align="center">{site}</td>
  <td align="right">{hits}</td>
  <th>&nbsp;</th>
 </tr>
<!-- END sitemblock -->
<!-- END siteblock -->
</table>

</td></tr></table>
