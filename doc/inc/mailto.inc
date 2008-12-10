<?php
##############################################################################
# mailto.php version 1.2.0 Gavin Brown (gavin.brown@uk.com)
# Future releases will be at http://jodrell.net/
# ----------------------------------------------------------------------------
# this  library provides  a way of putting mailto:  links in  HTML
# pages  without  exposing your e-mail  address to spammers.  When
# ----------------------------------------------------------------------------
# #Id: mailto.php,v 1.7 2002/08/14 10:54:19 jodrell Exp #
# $Id$
##############################################################################

 # URI encode a string of ASCII text:
 function uri_escape($str) {
   $escaped = "";
   for ($i = 0; $i < strlen($str); $i++) {
      $escaped .= '%' . dechex(ord(substr($str, $i, 1)));
   }
   return $escaped;
 }

 /** Encode an EMail address into JS mailto: link (written directly)
  * @function mailto
  * @param string address The EMail address to encode
  * @param optional string name The name displayed
  * @return boolean success
  */
  function mailto() {
    if (func_num_args() == 2) {
       list($email, $string) = func_get_args();
    } elseif (func_num_args() == 1) {
       list($email, $string) = array(func_get_arg(0), '');
    } else {
       return false;
    }
    print smailto($email, $string);
    return true;
  }

  # The Encoding sub-function
  function smailto() {
    if (func_num_args() == 2) {
       list($email, $string) = func_get_args();
    } elseif (func_num_args() == 1) {
       list($email, $string) = array(func_get_arg(0), '');
    } else {
       return false;
    }

    // if the string is empty, use the e-mail address:
    if ($string == '') $string = $email;

    // the JavaScript code to print the HTML:
    // (putting newlines into $code seems to mess up the encoding, so don't)
    $enc_email = uri_escape($email);
    $enc_str   = uri_escape($string);
    $code      = "var addr = '$enc_email';" .
                 "var string = '$enc_str';" .
                 "document.write('<a href=\"mailto:' + unescape(addr) + '\">' + unescape(string) + '</a>');";

    // encode the JavaScript code:
    $encoded = uri_escape($code);

    // generate stuff to go inside <noscript></noscript> tags:
    if ($string == $email) {
       $noscript = "(e-mail address hidden)";
    } else {
       $noscript = $string;
    }

    // print the JavaScript which prints the JavaScript which prints the HTML:
    return    "<script language=\"JavaScript\" type=\"text/javascript\">" .
              "eval(unescape('$encoded'));" . "</script>" .
              "<noscript>$noscript</noscript>";
    return true;
  }

 /** scan a chunk of text and replaces all e-mail addresses with smailto() calls:
  * @function mailto_text
  * @param string text text to scan
  * @return string replaced Original text with replaced addresses
  */
  function mailto_text($text) {
    preg_match_all("/([A-Za-z0-9\.\_\-]+\@{1}[A-Za-z0-9\.\_\-]+)/", $text, $addresses);
    foreach ($addresses[0] as $address) {
      $text = str_replace($address, smailto($address), $text);
    }
    return $text;
  }
?>