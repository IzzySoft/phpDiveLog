<table align="center" style="background-color:transparent;" border="0"><tr class="td_transp"><td class="td_transp">

<table cellpadding="0" align="left" style="background-color:transparent;" border="1">
 <tr style="background-color:transparent;">
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
 <tr><th>&nbsp;</th><th>{date_name}</th><th>{time_name}</th><th>{loc_name}</th><th>{rat_name}</th>
     <th>&nbsp;</th>
     <th>{ddt_name}</td><th>{buddy_name}</th></tr>

<!-- BEGIN itemblock -->
 <tr><th align="right"><a href="{dive_ref}">{dive#}</a></th>
  <td align="center">{date}</td>
  <td align="center">{time}</td>
  <td align="center">{location}:<br>{place}</td>
  <td align="center"><img src="{rating}"></td>
  <th>{pix}</th>
  <td align="center">{depth}<br>{divetime}</td>
  <td align="center">{buddy}</td>
 </tr>
<!-- END itemblock -->

</td></tr></table>
