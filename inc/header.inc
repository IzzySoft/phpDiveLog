<?
 header('Content-type: text/html; charset=utf-8');
 if ( !isset($robots_revisit) || empty($robots_revisit) && $robots_revisit!==0 ) $robots_revisit = 60;
 if ( !isset($robots_index)   || empty($robots_index) ) $robots_index = "index,follow";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<!--DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"-->
<html><head>
 <title><?=$title?></title>
 <meta name="description" content="DiveLog">
 <meta name="author" content="Izzy">
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
 <meta name="revisit-after" content="<?=$robots_revisit?> days">
 <meta name="robots" content="<?=$robots_index?>">
 <link rel="stylesheet" type="text/css" href="<?=$pdl->config->tpl_url?>divelog.css">
 <script language="JavaScript" type="text/javascript">//<!--
  function show_img(url,sx,sy) {
    sx += 20; sy += 20;
    var pos = 200;
    campus=eval("window.open(url,'image','toolbar=no,location=no,titlebar=no,directories=no,status=yes,resizable=yes,scrollbars=yes,copyhistory=no,width=" + sx + ",height=" + sy + ",top=0,left=" + pos + "')");
  }
  function open_help(page) {
    sx = 600; sy = 500;
    var pos = 300;
    //url = 'doc/'+page;
    url = 'help.php?topic='+page;
    campus=eval("window.open(url,'help','toolbar=no,location=no,titlebar=no,directories=no,status=yes,resizable=yes,scrollbars=yes,copyhistory=no,width=" + sx + ",height=" + sy + ",top=0,left=" + pos + "')");
  }
 //--></script>
</head><body>
