<?php

require_once "url_pdf_converter.php";
require_once "../services/Trace.php";
require_once "../services/GlobalUtil.php";

function ConvertCustomUtf($pmChar){
  $pmChar = str_replace('%u2122','&trade;',$pmChar);
  return $pmChar;
}

class PDFTemplate {
  private $html="";
  private $fp;
  private $tmp_url;
  //private $site_name;

  public function createHtml(){
    $obj=new GlobalUtil();
    $conf=$obj->getConfig();
    $this->tmp_url=$conf["pdf_temp_dir"];
    // Not used anywhere. 
    //$this->site_name= '';

    $body = $_POST["bodyContent"];
    expDebug::dPrint("PDF temp file body after utf8_urldecode : ".$body, 5);
    $langCode = $_POST["contentLanguage"];

    $this->html = '<html>';
    $this->html .= $this->generateHtmlHead();
    $this->html .= $this->generateHtmlBody($body);
    $this->html .= "</html>";
    $urlvars = parse_url($_SERVER['HTTP_REFERER']);    
    if($urlvars['query'] == "q=admincalendar"){//#61417    	
    	$baseName = 'export';
    	$filename = $baseName."_".date('dmY')."_".date('His')."."."html";
    }else{
    	$filename = time().'.html';
    }    
    $this->fp = fopen('../../../tmp/'.$filename,'w+');
    expDebug::dPrint('$filename = ' . $filename, 5);
    if($this->fp==false){
      echo "File not created <br/>";
      die("unable to create file");
    }
    if($this->fp)
    fwrite($this->fp,$this->html);
    fclose($this->fp);
    
    convertHtmlToPdf($filename, $langCode);
  }

  private function generateHtmlHead(){
    //expDebug::dPrint("ThemePath:->".path_to_theme());
    $var='<head>';
    $var.='<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />';
    //$var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/html-elements.css?w" />';
    //$var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/style.css?w" />';
    //$var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/style-widget.css?w" />';
    //$var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/jqgrid/steel/grid.css?w" />';
    $var.='</head>';
    expDebug::dPrint("HEAD INFO:->".$var, 5);
    return $var;
  }

  private function generateHtmlBody($body){
    $var ='<body style="background-color:#ffffff;padding: 20px;width:800px;">';
    $var .= $body;
    $var.='</body>';
    return $var;
  }
}

$obj = new PDFTemplate();
$obj->createHtml();
echo "Process Done. File created.";
?>