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
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">

<table class="outer" cellspacing="0" cellpadding="2" align="center" style="margin-top:7;">
 <tr><td>{nav_left}</td>
     <th class="head" colspan="5"><IMG SRC="{tpl_dir}images/globe.gif" width="15" height="15" alt="Location"> {loc}: {place}</th>
     <td align="right">{nav_right}</td>
     </tr>
 <tr>
  <th>&nbsp;</th>
  <th style="text-align:right">{lat_name}</th> <td>{latitude}</td>
  <th>&nbsp;</th>
  <th style="text-align:right">{long_name}</th> <td>{longitude}</td>
  <th>&nbsp;</th>
 </tr><tr>
  <th>&nbsp;</th>
  <th style="text-align:right">{alt_name}</th> <td>{altitude}</td>
  <th>&nbsp;</th>
  <th style="text-align:right">{md_name}</th> <td>{depth}</td>
  <th>&nbsp;</th>
 </tr>
</table>
<table class="outer" cellpadding="2" cellspacing="0" align="center" style="margin-top:4px">
 <tr><th>{notes_name}</th></tr>
 <tr><td>{description}</td></tr>
</table>

<!-- BEGIN fotoblock -->
</td></tr><tr class="td_transp"><td class="td_transp" colspan="2">
<table class="outer" align="center" cellpadding="2" cellspacing="0" style="margin-top:3px"><!-- Fotos -->
 <tr><th class="head" valign="middle"><img src="{tpl_dir}images/btn_fotos.gif" width="37" height="15" alt="Fotos"> {fotos_name}</th></tr>
 <tr><td align="center"><table><tr>
<!-- BEGIN fotoitemblock -->
   <td><span class="thumbnail"><img src="{foto}" align="center"><br>{fdesc}</span></td>
<!-- END fotoitemblock -->
   </tr></table></td></tr>
</table>
<!-- END fotoblock -->

</td></tr></table>