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

<!-- BEGIN personblock -->
<table class="outer" cellpadding="2" cellspacing="0" align="center" style="margin-top:4px">
 <tr><th class="head" valign="middle" colspan="2"><img src="{tpl_dir}images/btn_personal.gif" width="37" height="15" alt="Fotos"> {personal_name}</th></tr>
 <tr><td valign="middle"><img src="{portrait}"></td><td valign="middle">
  <table align="center">
<!-- BEGIN pdetailblock -->
   <tr><td><b>{name}</b></td><td>{description}</td></tr>
<!-- END pdetailblock -->
  </table></td></tr>
</table>
<!-- END personblock -->

<!-- BEGIN certblock -->
<table class="outer" cellpadding="2" cellspacing="0" align="center" style="margin-top:4px">
 <tr><th class="head" valign="middle" colspan="3"><img src="{tpl_dir}images/btn_certify.gif" width="37" height="15" alt="Fotos"> {certify_name}</th></tr>
 <tr><td align="center"><b>{date_name}</b></td><td align="center"><b>{course_name}</b></td><td align="center"><b>{place_name}</b></td></tr>
<!-- BEGIN cdetailblock -->
 <tr><td>{date}</td><td>{course}</td><td>{place}</td></tr>
<!-- END cdetailblock -->
</table>
<!-- END certblock -->

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