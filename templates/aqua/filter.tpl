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

<table class="outer" cellspacing="0" cellpadding="2" align="center" style="margin-top:7;">
 <tr>
     <th class="head" colspan="7"><img src="{tpl_dir}images/filter.png" width="16" height="16" alt="Filter"> {ptitle}</th>
 </tr>
</table>

<!-- BEGIN formblock -->
<form name="{formname}" action="{formtarget}" method="{formmethod}">
<table class="outer" cellpadding="2" cellspacing="0" align="center" style="margin-top:4px">
 <tr><th class="head" valign="middle" colspan="3"><img src="{icon_src}" width="{icon_width}" height="{icon_height}" alt="{icon_alt}"> {segment_name}</th></tr>
<!-- BEGIN itemblock -->
 <tr><td title="{name_bubble}">{name}</td><td title="{comp_bubble}"><select name="{comp}">{comp_opts}</select></td><td title="{val_bubble}"><input type="text" name="{input}" value={value}></td></tr>
<!-- END itemblock -->
 <tr><td colspan="3" align="center" title="{submit_bubble}"><input type="submit" name="{submit_name}" value="{submit_value}"></td></tr>
</table>
</form>
<!-- END formblock -->

</td></tr></table>