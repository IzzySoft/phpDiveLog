<table align="center" class="transp"><tr><td class="transp">

<table cellpadding="0" align="left" class="nav">
 <tr>
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
 <tr><td>{nav_left}</td><th>{date_name}</th><th>{time_name}</th><th>{loc_name}</th><th>{rat_name}</th>
     <th>{ddt_name}</td><th>{buddy_name}</th><td align="right">{nav_right}</td></tr>

<!-- BEGIN itemblock -->
 <tr><th align="right"><a href="{dive_ref}">{dive#}</a></th>
  <td align="center">{date}</td>
  <td align="center">{time}</td>
  <td align="center">{location}:<br>{place}</td>
  <td align="center"><img src="{rating}"></td>
  <td align="center">{depth}<br>{divetime}</td>
  <td align="center">{buddy}</td>
  <th>{pix}</th>
 </tr>
<!-- END itemblock -->

</td></tr></table>
