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
 <colgroup><col width="50%"><col width="50%"></colgroup>
 <form name="impform" method="post" action="{formtarget}">
 <tr><th colspan="2">{title}</th></tr>
 <tr><td colspan="2" class="notes">{notes}</td></tr>
 <tr><td align="right"><input name="passwd" type="password"></td>
     <td align="left"><input type="submit" name="submit" value="{submit}"></td></tr>
 </form>
</table>

</td></tr></table>
