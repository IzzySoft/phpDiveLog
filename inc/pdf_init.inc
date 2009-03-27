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
  GLOBAL ${$varname};
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
define ('PDF_AUTHOR', $pdl->config->user_name);
define ('PDF_HEADER_TITLE', 'phpdiveLog of '.PDF_AUTHOR);
define ('PDF_HEADER_STRING',"");
define ('PDF_HEADER_LOGO', 'logo.gif');
define ('PDF_PAGE_GUTTER', config_value("pdf_page_gutter",'/^[1-9][0-9]*$/',25,1));
define ('PDF_PAGE_MARGIN', config_value("pdf_page_margin",'/^[1-9][0-9]*$/',5,1));
if (PAGE_GUTTER_LEFT) { // should be removed when site is prepared for MULTI_PAGE as well
  define ('PDF_MARGIN_LEFT', 25);
  define ('PDF_MARGIN_RIGHT', 5);
} else {
  define ('PDF_MARGIN_LEFT', 5);
  define ('PDF_MARGIN_RIGHT', 25);
}
define ('PDF_MAX_NOTECHARS', config_value("pdf_max_notechars",'/^[0-9]+$/',1900,1));
define ('PDF_CHARS_PER_PIX', config_value("pdf_chars_per_pix",'/^[0-9]+$/',10,1));
$pdf_no_profile = strtolower($pdf_no_profile);
define ('PDF_NO_PROFILE', config_value("pdf_no_profile",'/^(dummy|blank)$/',"dummy",1));

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
  /** Initialize duplex set (push in empty pages if needed)
   * @method duplexInit
   * @param integer start number of first page
   * @param integer end number of last page
   * @param string gutter arrangement for gutter (inner|outer|side)
   */
  function duplexInit($start,$end,$gutter) {
    if ($start==$end) return; // nothing to do for a single record
    switch($gutter) {
      case "inner":
      case "outer":
      case "side":
         if (MULTI_PAGE && !($start%2) && preg_match('!^.{1}5$!',PDF_PAGE_FORMAT)) {
           $this->setPrintFooter(FALSE);
           $this->SetMargins(PDF_PAGE_MARGIN, PDF_MARGIN_TOP_NOHEAD, PDF_PAGE_GUTTER);
           for ($i=0;$i<3;++$i) {
             $this->AddPage();
             $this->writeHTML("<BR />",true,0,true,0);
           }
         }
         break;
      default: break;
    } // end switch
  } // end duplexInit
  /** Finish duplex set (shuffle pages for duplex printing)
   * @param integer start number of first page
   * @param integer end number of last page
   * @param string gutter arrangement for gutter (inner|outer|side)
   */
  function duplexFinish($start,$end,$gutter) {
    if ($start==$end) return; // nothing to do for a single record
    $pages = $this->getNumPages();
    switch($gutter) {
      case "inner": // 4-1-2-3
         for ($i=1;$i<=$pages;$i+=4) { // step 4 pages (A5 duplex on A4)
           switch($pages-$i) {
             case  0: break; // a single page does not matter where to go
             case  1: // last 2 pages
               $this->AddPage();
               $this->setPrintFooter(FALSE);
               $this->writeHTML("<BR />",true,0,true,0);
               $this->movePage($i+2,$i);
               break;
             case  2: // last 3 pages
               $this->AddPage();
               $this->setPrintFooter(FALSE);
               $this->writeHTML("<BR />",true,0,true,0);
               $this->movePage($i+3,$i);
               break;
             default: // we still have at least 4 pages
               $this->movePage($i+3,$i);
               break;
           }
         }
         break;
      case "outer":
      case "side": // 1-3-4-2
         for ($i=1;$i<=$pages;$i+=4) { // step 4 pages (A5 duplex on A4)
           switch($pages-$i) {
             case  0: break; // a single page does not matter where to go
             case  1: // last 2 pages
               $this->AddPage();
               $this->setPrintFooter(FALSE);
               $this->writeHTML("<BR />",true,0,true,0);
               $this->movePage($i+2,$i);
               break;
             case  2: // last 3 pages
               $this->AddPage();
               $this->setPrintFooter(FALSE);
               $this->writeHTML("<BR />",true,0,true,0);
               $this->movePage($i+3,$i);
               break;
             default: // we still have at least 4 pages
               $this->movePage($i+2,$i+1);
               $this->movePage($i+3,$i+2);
               break;
           }
         }
         break;
      default: break;
    } // end switch
  } // end duplexFinish
}

?>