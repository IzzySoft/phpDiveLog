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

<table class="pagetab" border="0" cellpadding="0" align="center" style="margin-top:7;margin-bottom:7;min-width:400px;">
 <tr><td style="width:25px;">{nav_left}</td><td align="center">{pages}</td><td align="right" style="width:25px;">{nav_right}</td></tr>
</table>

<table cellpadding="2" border="0" align="center">
 <tr><th colspan="5"><h3 style="margin-bottom:0;">{maplink}<IMG SRC="{tpl_dir}images/globe{anim}.gif" width="15" height="15" alt="Conditions">{mapunlink} {loc}: {place}</h3></th></tr>
 <tr>
  <th style="text-align:right">{lat_name}</th> <td>{latitude}</td>
  <td class="td_transp"></td>
  <th style="text-align:right">{long_name}</th> <td>{longitude}</td>
 </tr><tr>
  <th style="text-align:right">{alt_name}</th> <td>{altitude}</td>
  <td class="td_transp"></td>
  <th style="text-align:right">{md_name}</th> <td>{depth}</td>
 </tr><tr>
  <th>&nbsp;</th>
  <th style="text-align:right">{water_name}</th> <td>{water}</td>
  <th>&nbsp;</th>
  <th style="text-align:right">{divetype_name}</th> <td>{type}</td>
  <th>&nbsp;</th>
 </tr><tr>
  <th>&nbsp;</th>
  <th style="text-align:right">{rating_name}</th> <td>{rating}</td>
  <th>&nbsp;</th>
  <th style="text-align:right">&nbsp;</th> <td>&nbsp;</td>
  <th>&nbsp;</th>
 </tr>
</table>
<br>
<!-- BEGIN notesblock -->
<table cellpadding="2" border="0" align="center">
 <tr><th><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_notes.gif" width="37" height="15" alt="Notes"> {notes_name}</h3></th></tr>
 <tr><td class="notes">{description}</td></tr>
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

<table class="pagetab" border="0" cellpadding="0" align="center" style="margin-top:7;min-width:400px;">
 <tr><td style="width:25px;">{nav_left}</td><td align="center">{pages}</td><td align="right" style="width:25px;">{nav_right}</td></tr>
</table>

</td></tr></table>