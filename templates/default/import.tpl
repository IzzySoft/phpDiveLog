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
 <table border="0" cellpadding="2" width="100%" style="background-color:transparent;"><tr>
  <td class="td_transp" align="left">{nav_left}</td>
  <td class="td_transp" align="right">{nav_right}</td>
 </tr></table>

</td></tr><tr class="td_transp"><td class="td_transp">

<table border="1" cellpadding="2" align="center">
 <colgroup><col width="50%"><col width="50%"></colgroup>
 <form name="impform" method="post" action="{formtarget}">
 <tr><th colspan="2">{title}</th></tr>
 <tr><td colspan="2" class="notes">{notes}</td></tr>
 <tr><td align="right"><input name="passwd" type="password"></td>
     <td align="left"><input type="submit" name="submit" value="{submit}"></td></tr>
 </form>
</table>

</td></tr></table>
