<? header('Content-type: text/html; charset=utf-8'); ?>
<html><head>
 <title><?=$title?></title>
 <meta name="description" content="DiveLog">
 <meta name="author" content="Izzy">
 <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
 <link rel="stylesheet" type="text/css" href="<?=$pdl->config->tpl_url?>divelog.css">
 <script language="JavaScript" type="text/javascript">//<!--
  function show_img(url,sx,sy) {
    sx += 20; sy += 20;
//    var pos = (screen.width/2)-400;
    var pos = 200;
    campus=eval("window.open(url,'image','toolbar=no,location=no,titlebar=no,directories=no,status=yes,resizable=yes,scrollbars=yes,copyhistory=no,width=" + sx + ",height=" + sy + ",top=0,left=" + pos + "')");
  }
 //--></script>
</head><body>
