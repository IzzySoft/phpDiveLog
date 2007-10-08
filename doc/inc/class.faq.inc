<?php
 #############################################################################
 # phpApi                                   (c) 2001-2004 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Simple PageMaker                                                          #
 #############################################################################

 /* $Id$ */

#=========================================================[ initial setup ]===

#====================================================[ Build page content ]===
/** Page creator
 * @package Api
 * @class pagemaker
 */
class pagemaker {

 VAR $t,       // template class
     $block,   // active block
     $itemc;   // item counter

 /** Constructor: Setting up the templates
  * @constructor pagemaker
  */
 function pagemaker(&$tpl_class,$tpl_file) {
  $this->t = $tpl_class;
  $this->t->set_file(array("main"=>$tpl_file));
  $this->t->set_block("main","tocblock","toc");
  $this->t->set_block("main","titleblock","titlelist");
  $this->t->set_block("titleblock","textblock","textlist");
  $this->block->name = "";
  $this->itemc = 0;
 }

 function add_item($text) {
   $text = trim($text);
   $this->t->set_var("item","<A HREF='#".$this->itemc."'>$text</A>");
   $this->t->parse("toc","tocblock",TRUE);
 }

 /** Replacing placeholders for variables and translations within the
  *  input files current block, then add it to the content to parse into
  *  the templates
  * @package Help
  * @class pagemaker
  * @method add_block
  * @param string content content block to parse
  */
 function add_block($content) {
  if (!$this->block->name) return;
  if ( preg_match_all("/\{\S+\}/",$content,$matches) ) { // replace variables
   $var = substr($matches[0],1,strlen($matches[0])-2);
   for ($i=0;$i<count($matches[0]);$i++) {
     $var = substr($matches[0][$i],1,strlen($matches[0][$i])-2);
     $pos = strpos($var,"->");
     if ($pos) {
      $obj = substr($var,0,$pos); $prop = substr($var,$pos+2);
      GLOBAL $$obj;
      $rvar = $$obj->$prop;
     } else { $rvar = $GLOBALS[$var]; }
     $content = preg_replace("/\{$var\}/",$rvar,$content);
   }
  }
/* if ( preg_match_all("/\*\S+\#/",$content,$matches) ) { // replace translations
   $var = substr($matches[0],1,strlen($matches[0])-2);
   for ($i=0;$i<count($matches[0]);$i++) {
     $var  = substr($matches[0][$i],1,strlen($matches[0][$i])-2);
     $rvar = lang($var);
     $content = preg_replace("/\*$var\#/",$rvar,$content);
   }
  } */
  $this->block->content .= $content;
 }

 /** Parse the current block into the template
  * @package Help
  * @class pagemaker
  * @method parse_block
  */
 function parse_block($keep=1) {
  if (!$this->block->name) return;
  $list   = $this->block->name . "list";
  $pblock = $this->block->name . "block";
  $this->t->set_var($this->block->name,trim($this->block->content));
  $this->t->parse($list,$pblock,$this->block->append);
  if (!$this->block->append) $this->block->append = 1;
  $this->block->content = "";
 }

 /** Close the current block. This means, next block won't be appended, but
  *  a new block will be started
  * @package Help
  * @class pagemaker
  * @method close_block
  */
 function close_block () {
  if (!$this->block->name) return;
  $this->parse_block();
  $this->block->append  = 0;
 }

 /** Set template variables. Wrapper around pagemaker::t::set_var
  * @package Help
  * @class pagemaker
  * @method set_nav
  * @param string name name of template variable
  * @param string content value of template variable
  */
 function set_nav($name,$content) {
   $this->t->set_var($name,$content);
 }

 /** Main method - and the only one that should be called from the public.
  *  This will build the page from the given file, using the given title
  *  for the headers, and send it to the browser
  *  If no title is given, obviously we parse no complete page but a
  *  subblock
  * @package Help
  * @class pagemaker
  * @method make_page
  * @param string file input file
  * @param optional string title page title
  */
 function make_page($file,$title="") {
  $input = file($file);
  if (substr(trim($input[0]),0,1)=="!") { // "symbolic link"
    $file = dirname($file) . "/" . trim(substr($input[0],1));
    $input = file($file);
  }
  $line  = 0;
  while ( $line<count($input) ) {
   switch ( trim(strtolower($input[$line])) ) {
    case "[title]"   : if ($this->block->name) {
                        if ($this->block->name=="title") {
                         $this->close_block();
                        } else {
			 $this->close_block();
                         $this->block->name = "title";
                         $this->add_block($this->block->title);
			 $this->block->append  = 1;
			 $this->close_block();
			 $this->block->content = "";
			}
		       }
                       $this->block->name = "title";
		       ++$line;
                       continue;
		       break;
    case "[text]"    : if ($this->block->name) {
                        if ($this->block->name=="text") {
                         $this->parse_block();
                        } else {
			 $this->itemc++;
                         $this->block->title   = "<A NAME='".$this->itemc."'></A>".$this->block->content;
			 $this->add_item($this->block->content); // add to TOC
			 $this->block->content = "";
                        }
		       }
                       $this->block->name = "text";
		       ++$line;
                       continue;
		       break;
    case "[eof]"     : $eof = TRUE; break;
    default          : break;
   }
   if ($eof) {
    $this->close_block();
    $this->block->name = "title";
    $this->add_block($this->block->title);
    $this->block->append  = 1;
    $this->close_block();
    break;
   }
   if ( substr(trim($input[$line]),0,1) == "#" ) { // comment line
     $line++;
     continue;
   }
   switch ($this->block->name) {
    case "title"   : $this->block->content .= $input[$line]; break;
    default        : //$this->block->title  = "";
                     $this->add_block($input[$line]);
		     break;
   }
   $line++;
  }
  $this->t->set_var("listtitle",$title);
 } // end function make_page

 function output() {
  $this->t->pparse("out","main");
 }

 /** Template Class to built up pages
  * @package Help
  * @class pagemaker
  * @attribute object t
  */
 /** Active block we work on
  * @package Help
  * @class pagemaker
  * @attribute object block
  */

} // end class pagemaker

?>