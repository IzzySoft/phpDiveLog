<table align="center" style="background-color:transparent;" border="0"><tr class="td_transp"><td class="td_transp">

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

<!-- BEGIN personblock -->
<table cellpadding="2" cellspacing="2" align="center" style="margin-top:7px">
 <tr><th valign="middle" colspan="2"><img src="{tpl_dir}images/btn_personal.gif" width="37" height="15" alt="Fotos"> {personal_name}</th></tr>
 <tr><td valign="middle" align="center"><img src="{portrait}"></td>
     <td valign="middle" align="center"><table>
<!-- BEGIN pdetailblock -->
 <tr><td><b>{name}</b></td><td>{description}</td></tr>
<!-- END pdetailblock -->
     </table></td></tr>
</table>
<!-- END personblock -->

<!-- BEGIN certblock -->
<table cellpadding="2" cellspacing="2" align="center" style="margin-top:4px">
 <tr><th valign="middle" colspan="3"><img src="{tpl_dir}images/btn_certify.gif" width="37" height="15" alt="Fotos"> {certify_name}</th></tr>
 <tr><td align="center"><b>{date_name}</b></td><td align="center"><b>{course_name}</b></td><td align="center"><b>{place_name}</b></td></tr>
<!-- BEGIN cdetailblock -->
 <tr><td>{date}</td><td>{course}</td><td>{place}</td></tr>
<!-- END cdetailblock -->
</table>
<!-- END certblock -->

<!-- BEGIN notesblock -->
<br>
<table cellpadding="2" border="0" align="center">
 <tr><th><h3 style="margin-bottom:0;"><img src="{tpl_dir}images/btn_notes.gif" width="37" height="15" alt="Notes"> {notes_name}</h3></th></tr>
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

</td></tr></table>
