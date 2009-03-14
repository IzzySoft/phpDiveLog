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
}

?>