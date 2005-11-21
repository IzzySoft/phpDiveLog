<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2005 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft@qumran.org>                          #
 # http://www.qumran.org/homes/izzy/                                         #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Commonly used methods (alerts etc.)                                       #
 #############################################################################

 /* $Id$ */

 /** Commonly used methods (alerts etc.)
  * @package Api
  * @class common
  * @author Izzy (izzysoft@qumran.org)
  * @copyright (c) 2004 by Itzchak Rehberg and IzzySoft
  */
 class common {

  /** Constructor: Set up directories
   * @constructor common
   */
  function common() {
    $this->dir->diverroot = "diver/";
    $this->dir->sitepix   = "fotos/site";
    $this->dir->divepix   = "fotos/dive";
    $this->dir->privpix   = "fotos/person";
    $this->dir->notes     = "notes";
    $this->dir->text      = "text";
  }

  /** Build up alert page with a given message
   * @class common
   * @method alert
   * @param string alert message
   */
  function alert($message) {
    GLOBAL $pdl;
    $t = new Template($pdl->config->tpl_path);
    $t->set_file(array("template"=>"alert.tpl"));
    $t->set_var("tpl_dir",$pdl->config->tpl_url);
    $t->set_var("message",$message);
    $t->pparse("out","template");
  }

  /* Helper to tagreplace: replace dirnames in URLs (private!) */
  function dirReplace($string,$url=0) {
    GLOBAL $pdl;
    if (substr($string,0,1)=="~") {
      if ($url) $root = $pdl->config->base_url;
        else $root = $pdl->config->base_path;
      $pos  = strpos($string,"/");
      if (substr($string,2,1)=="@") {
        $root .= $this->dir->diverroot.substr($string,3,$pos-2);
      } else {
        $root .= $pdl->config->user_url;
      }
      switch(substr($string,1,1)) {
        case "d" : $root.=$this->dir->divepix; break;
	case "n" : $root.=$this->dir->notes; break;
	case "p" : $root.=$this->dir->privpix; break;
	case "s" : $root.=$this->dir->sitepix; break;
	case "t" : $root.=$this->dir->text; break;
      }
      return $root.substr($string,$pos);
    }
    return $string;
  }

  /** Search a text block for URLs & cater for the [img],[imgr] and [url] tags
   * @class common
   * @method tagreplace
   * @param string data textblock to format
   * @return string newText formatted text block
   */
  function tagreplace($data) {
    GLOBAL $pdl;
    if(empty($data)) {
      return $data;
    }
    $lines = split("\n",$data);
    while ( list ($key,$line) = each ($lines)) {
      if ( preg_match_all("/\[href\]\S+\[\/href\]/i",$line,$matches) ) {
        for ($i=0;$i<count($matches[0]);++$i) {
          $sub  = eregi_replace("\[href\](.*)\[\/href\]","<A HREF=\"\\1\" TARGET=\"_blank\">\\1</A>",$matches[0][$i]);
          $line = str_replace($matches[0][$i],$sub,$line);
        }
      }
      if ( preg_match_all("/\[file\].+\[\/file\]/i",$line,$matches) ) {
        for ($i=0;$i<count($matches[0]);++$i) {
          $link = $this->dirReplace( eregi_replace("\[file\](.*)\[\/file\]","\\1",$matches[0][$i]) );
	  $text = $this->tagreplace( $pdl->file->read_file($link) );
          $sub  = eregi_replace("\[file\](.*)\[\/file\]",$text,$matches[0][$i]);
          $line = str_replace($matches[0][$i],$sub,$line);
        }
      }
      if ( preg_match_all("/\[img\].+\[\/img\]/i",$line,$matches) ) {
        for ($i=0;$i<count($matches[0]);++$i) {
          $link  = $this->dirReplace( eregi_replace("\[img\]([^|]+)\|{0,1}(.*)\[\/img\]","\\1",$matches[0][$i]) );
          $sub  = eregi_replace("\[img\]([^|]+)\|{0,1}(.*)\[\/img\]","<SPAN CLASS='thumbnail-left'><IMG ALIGN='center' SRC='$link'><BR>\\2</SPAN>",$matches[0][$i]);
          $line = str_replace($matches[0][$i],$sub,$line);
        }
      }
      if ( preg_match_all("/\[imgr\].+\[\/imgr\]/i",$line,$matches) ) {
        for ($i=0;$i<count($matches[0]);++$i) {
          $link  = $this->dirReplace( eregi_replace("\[imgr\]([^|]+)\|{0,1}(.*)\[\/imgr\]","\\1",$matches[0][$i]) );
          $sub  = eregi_replace("\[imgr\]([^|]+)\|{0,1}(.*)\[\/imgr\]","<SPAN CLASS='thumbnail-right'><IMG ALIGN='center' SRC='$link'><BR>\\2</SPAN>",$matches[0][$i]);
          $line = str_replace($matches[0][$i],$sub,$line);
        }
      }
      $line = eregi_replace("\[imgr\](.*)\[\/img\]","<IMG ALIGN=\"right\" SRC=\"\\1\">",$line);
      if ( preg_match_all("/\[url\].+\[\/url\]/i",$line,$matches) ) {
        for ($i=0;$i<count($matches[0]);++$i) {
          $link  = $this->dirReplace( eregi_replace("\[url\]([^|]+)\|{0,1}(.*)\[\/url\]","\\1",$matches[0][$i]) );
          $text  = $this->dirReplace( eregi_replace("\[url\]([^|]+)\|{0,1}(.*)\[\/url\]","\\2",$matches[0][$i]) );
	  if (empty($text)) $text = $link;
          $sub  = eregi_replace("\[url\]([^|]+)\|{0,1}(.*)\[\/url\]","<A HREF='$link' TARGET='_blank'>$text</A>",$matches[0][$i]);
          $line = str_replace($matches[0][$i],$sub,$line);
        }
      }
      $newText .= "$line\n";
    }
    return $newText;
  }

  /** Format the input from plain text files (improved nl2br)
   * @class common
   * @method nl2br
   * @param string input
   * @param optional boolean tagreplace
   *  Whether to automatically run the tagreplace (default: TRUE)
   * @param optional boolean br Replace ALL nl with BR? (default: FALSE)
   * @return string formated input
   * @comment based on the enhanced autop() function by Photo Matt
   *  (see http://photomatt.net/scripts.php/autop)
   */
  function nl2br($pee, $tagreplace=1, $br=0) {
    if ( empty($pee) ) return $pee;
    if ($tagreplace) $pee = $this->tagreplace($pee);
    $pee = $pee . "\n"; // just to make things a little easier, pad the end
    $pee = preg_replace('|<br />\s*<br />|', "\n\n", $pee);
    $pee = preg_replace('!(<(?:table|ul|ol|li|pre|form|blockquote|h[1-6])[^>]*>)!', "\n$1", $pee); // Space things out a little
    $pee = preg_replace('!(</(?:table|ul|ol|li|pre|form|blockquote|h[1-6])>)!', "$1\n", $pee); // Space things out a little
    $pee = preg_replace("/(\r\n|\r)/", "\n", $pee); // cross-platform newlines 
    $pee = preg_replace("/\n\n+/", "\n\n", $pee); // take care of duplicates
    $pee = preg_replace('/\n?(.+?)(?:\n\s*\n|\z)/s', "<p>$1</p>\n", $pee); // make paragraphs, including one at the end 
    $pee = preg_replace('|<p>\s*?</p>|', '', $pee); // under certain strange conditions it could create a P of entirely whitespace 
    $pee = preg_replace("|<p>(<li.+?)</p>|", "$1", $pee); // problem with nested lists
    $pee = preg_replace('|<p><blockquote([^>]*)>|i', "<blockquote$1><p>", $pee);
    $pee = str_replace('</blockquote></p>', '</p></blockquote>', $pee);
    $pee = preg_replace('!<p>\s*(</?(?:table|tr|td|th|div|ul|ol|li|pre|select|form|blockquote|p|h[1-6])[^>]*>)!', "$1", $pee);
    $pee = preg_replace('!(</?(?:table|tr|td|th|div|ul|ol|li|pre|select|form|blockquote|p|h[1-6])[^>]*>)\s*</p>!', "$1", $pee); 
    if ($br) $pee = preg_replace('|(?<!<br />)\s*\n|', "<br />\n", $pee); // optionally make line breaks
    $pee = preg_replace('!(</?(?:table|tr|td|th|div|dl|dd|dt|ul|ol|li|pre|select|form|blockquote|p|h[1-6])[^>]*>)\s*<br />!', "$1", $pee);
    $pee = preg_replace('!<br />(\s*</?(?:p|li|div|th|pre|td|ul|ol)>)!', '$1', $pee);
    $pee = preg_replace('/&([^#])(?![a-z]{1,8};)/', '&#038;$1', $pee);
    $pee = str_replace("<p><hr></p>","<hr>",$pee);
    return $pee; 
  }

  /** Format time to human readable style
   * @method time_format
   * @param integer time in seconds
   * @return string formatted time
   */
  function time_format($time) {
    $string = $time;
    if ($time<60) return $string;
    // mins
    $secs = $time % 60;
    if ($secs<10) $secs = "0$secs";
    $mins = round($time/60);
    $string = $mins.":".$secs;
    if ($time<3600) return $mins.":".$secs;
    // hours
    $hours = round($mins/60);
    $mins = $mins % 60;
    return $hours.":".$mins.":".$secs;
  }

 } // end class common

 $pdl->common = new common;
?>