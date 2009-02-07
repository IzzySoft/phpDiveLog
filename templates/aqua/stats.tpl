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

<table class="outer" border="0" cellpadding="4" cellspacing="0" align="center" style="margin-top:10;">
 <tr><th class="big" colspan="7"><img src="{tpl_dir}images/tab_stats.gif" width="20" height="15" border="0" alt="Stats" valign="middle"> {title}</th></tr>
 <tr><th rowspan="4">&nbsp;</th><th>{max_depth_name}</th><td align="right">{max_depth}</td>
  <th>&nbsp;</th>
  <th>{max_time_name}</th><td align="right">{max_time}</td><th rowspan="4">&nbsp;</th></tr>
 <tr><th>{avg_depth_name}</th><td align="right">{avg_depth}</td>
  <th>&nbsp;</th>
  <th>{avg_time_name}</th><td align="right">{avg_time}</td></tr>
 <tr><th>{dive_num_name}</th><td align="right">{dive_num}</td>
  <th>&nbsp;</th>
  <th>{cum_time_name}</th><td>{cum_time}</td></tr>
 <tr><th>{site_num_name}</th><td align="right">{site_num}</td>
  <th>&nbsp;</th>
  <th>{avg_sd_name}</th><td align="right">{avg_sd}</td></tr>
 <tr><th colspan="7">&nbsp;</th></tr>
</table>

<!-- BEGIN diveyearblock -->
<table class="outer" border="0" cellpadding="4" cellspacing="0" align="center" style="margin-top:10;">
 <tr><th><img src="{tpl_dir}images/tab_stats.gif" width="20" height="15" border="0" alt="Stats" style="vertical-align:middle"> {ytitle}</th></tr>
 <tr><td align="center"><img src="{yearstat_png}" alt="{yearstat_alt}" {usemap}>{yearmap}</td></tr>
</table>
<!-- END diveyearblock -->
</td></tr></table>
