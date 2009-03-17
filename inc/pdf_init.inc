<?php
 #############################################################################
 # phpDiveLog                               (c) 2004-2009 by Itzchak Rehberg #
 # written by Itzchak Rehberg <izzysoft AT qumran DOT org>                   #
 # http://www.izzysoft.de/                                                   #
 # ------------------------------------------------------------------------- #
 # This program is free software; you can redistribute and/or modify it      #
 # under the terms of the GNU General Public License (see doc/LICENSE)       #
 # ------------------------------------------------------------------------- #
 # Initializing the PDF engine                                               #
 #############################################################################
 # $Id$

require_once(dirname(__FILE__)."/includes.inc");

#===========================================================[ Helper funcs ]===
function config_value($varname,$regexp,$default,$neg=FALSE) {
  if ( $neg ) { if ( isset(${varname}) && preg_match($regexp,${$varname}) ) return ${$varname}; }
  elseif (! (!isset(${$varname}) || preg_match($regexp,${$varname})) ) return ${$varname};
  return $default;
}

#===============================================================[ Settings ]===
#---------------------------------------------------[ TCPDF API available? ]---
if (K_PATH_MAIN == '') {
  include(dirname(__FILE__)."/header.inc");
  $pdl->common->alert(lang("pdf_api_not_found"));
  include(dirname(__FILE__)."/footer.inc");
  exit;
}

#----------------------------------------------------[ some TCPDF settings ]---
define ('PDF_MARGIN_TOP_NOHEAD',10); // PDL specific constant
define ('K_PATH_IMAGES', $pdl->config->tpl_path.'images/');
define ('PDF_PAGE_FORMAT', config_value("pdf_page_format",'/^(LETTER|(A|B)[0..5])$/i','A5',1));
define ('PDF_PAGE_ORIENTATION', config_value("pdf_page_orientation",'/^[PL]$/i','P',1)); // 'P'ortrait / 'L'andscape
# define ('PDF_CREATOR', 'phpDiveLog/TCPDF');
define ('PDF_AUTHOR', $pdl->config->user_name);
define ('PDF_HEADER_TITLE', 'phpdiveLog of '.PDF_AUTHOR);
define ('PDF_HEADER_STRING',"");
define ('PDF_HEADER_LOGO', 'logo.gif');
# define ('PDF_FONT_NAME_MAIN', 'helvetica');
# define ('PDF_FONT_NAME_DATA', 'helvetica');
# define ('PDF_FONT_NAME_PRE', 'courier');
if (PAGE_GUTTER_LEFT) {
  define ('PDF_MARGIN_LEFT', 25);
  define ('PDF_MARGIN_RIGHT', 5);
} else {
  define ('PDF_MARGIN_LEFT', 5);
  define ('PDF_MARGIN_RIGHT', 25);
}
define ('PDF_MAX_NOTECHARS', config_value("pdf_max_notechars",'/^[0-9]+$/',1900,1));
define ('PDF_CHARS_PER_PIX', config_value("pdf_chars_per_pix",'/^[0-9]+$/',10,1));

#====================================================[ Include the PDF Api ]===
require_once(K_PATH_MAIN.'config/lang/eng.php');
require_once(K_PATH_MAIN.'tcpdf.php');

#=========================================================[ Extend the Api ]===
/** PDL extension for TCPDF
 * @class pdlPDF
 * @extends TCPDF
 */
class pdlPDF extends TCPDF {
  /** Custom Footer for PDL
   * @method Footer
   */
  public function Footer() {
    GLOBAL $pdl;
    $this->SetY(-10); // Position at 1.0 cm from bottom
    $this->SetFont('helvetica', 'I', 6); // Set font
    $this->Cell(0, 10, "Created by phpDiveLog v".$pdl->config->version, 0, 0, 'C', 0, "http://projects.izzysoft.de/trac/phpdivelog/");
  }
  /** Override the Error method to log its message according to PDL settings
   * @method Error
   * @param string error message
   */
  public function Error($msg) {
    trigger_error("TCPDF ERROR: $msg",E_USER_ERROR);
    die('<strong>TCPDF ERROR: </strong> A serious error forced this cript to be halted. Details can be found in the error log.');
  }
  /** Adjust HTML code (TCPDF only accepts 100% XHTML and does not handle certain elements)
   * @method htmlAdjust
   * @param ref string HTML text to check
   */
  public function htmlAdjust(&$html) {
    // no CSS classes supported by TCPDF
    $html = preg_replace('|<SPAN CLASS=.thumbnail-right.>(.*?)</SPAN>|ims','<SPAN STYLE="float:right;text-align:center;margin-left:5px;">$1</SPAN>',$html);
    // tags must conform XHTML standard (<BR> to <BR/> etc.)
    $tags = array("hr","br","img");
    foreach ($tags as $tag) {
      if (preg_match_all("|<$tag.*?>|ims",$html,$matches)) {
        $mc = count($matches[0]);
        for ($i=0;$i<$mc;++$i) {
          $fix = preg_replace('|(?<!/)>|',' />',$matches[0][$i]);
          if (strlen($fix)) $html = str_replace($matches[0][$i],$fix,$html);
        }
      }
    }
    // TCPDF takes spaces literally!
    $html = preg_replace('!(<(/p|hr)>)\s*<!ims','$1<',$html);
    // change negative font sizes to defined font size
    $html = preg_replace('!<FONT SIZE=("|\')\-\d+?("|\')!ims','<FONT STYLE="font-size:7px"',$html);
  }
  /** Select possible "PDF subset" from comment
   *  In a notes field/file, you can define what shall be considered for PDF
   *  export using the markers <code>&lt;!-- PDF_START --&gt;</code> and
   *  <code>&lt;!-- PDF_END --&gt;</code>. If (one of) these markers is missing,
   *  it is "assumed" to be at the very start/end of the full comment
   * @method notesSubset
   * @param ref string HTML to check
   */
  public function notesSubset(&$html) {
    $pdf_s = strpos($html,'<!-- PDF_START -->');
    $pdf_e = strpos($html,'<!-- PDF_END -->');
    if ($pdf_s!==FALSE) $pdf_s += 18; // we don't need the comment
    else $pdf_s = 0;
    if ($pdf_e) $html = substr($html,$pdf_s,$pdf_e - $pdf_s)."</p>";
    else $html = substr($html,$pdf_s);
    if ($pdf_s) $html = "<p>$html";
  }
  /** Restrict length of notes so they fit on the page
   * @method notesRestrictLen
   * @param ref string HTML to check
   */
  public function notesRestrictLen(&$html) {
    $maxlen = PDF_MAX_NOTECHARS;
    if (preg_match_all('!<img.*?>!ims',$html,$matches)) {
      $mc = count($matches);
      for ($i=0;$i<$mc;++$i) {
        if ( preg_match('!height\s*=\s*("|\')(.*?)("|\')!ims',$matches[0][$i],$match) )
          $maxlen -= $match[2]*PDF_CHARS_PER_PIX;
        else $maxlen -= 500;
      }
    }
    if (strlen($html)>$maxlen) {
      $pos  = strpos($html," ",$maxlen);
      $html = substr($html,0,$pos)." ...</p>";
    }
  }
}

?>