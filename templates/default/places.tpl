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
 <table border="0" cellpadding="2" width="100%" style="background-color:transparent;"><tr>
  <td class="td_transp" align="left">{nav_left}</td>
  <td class="td_transp" align="right">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp">

<table border="1" cellpadding="2" align="center">
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
 <tr><th>{serial}</th>
  <td align="center">{place}</td>
  <td align="center">{site}</td>
  <td align="right">{diver}</td>
  <th>{pix}</th>
 </tr>
<!-- END sitemblock -->
<!-- END siteblock -->
</table>

</td></tr></table>
