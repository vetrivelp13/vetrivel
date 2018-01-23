<?php
require_once "url_pdf_converter.php";
//require_once "url_pdf_converter_report.php";
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
    private $site_name;

    public function createHtml(){
        $obj=new GlobalUtil();
        $conf=$obj->getConfig();
        $this->tmp_url=$conf["pdf_temp_dir"];
        $this->site_name=$conf["admin_site_name"];

        $body = ConvertCustomUtf(urldecode($_POST["bodyContent"]));  

        expDebug::dPrint("PDF temp file body : ".$body);
        $this->html = '<html>';
        $this->html .= $this->generateHtmlHead();
        $this->html .= $this->generateHtmlBody($body);
        $this->html .= $this->generateHtmlFooter();
        $this->html .= "</html>";
        $filename = time().'.html';
        $filepath = $this->tmp_url."tmp/".$filename;
        $this->fp = fopen('../../../tmp/'.$filename,'w+');
        expDebug::dPrint("PDF temp file : ".$filepath);
        //$this->fp = fopen("ftp://192.168.2.241/".$filename,'w');
        if($this->fp==false){
            echo "File not created <br/>";
            die("unable to create file");
        }
        if($this->fp)
        fwrite($this->fp,$this->html);
        fclose($this->fp);

        //$status = generateMultibytePDF($filepath,'');
        convertHtmlToPdf($filename,'cre_sys_lng_eng');
    }

    private function generateHtmlHead(){
        //expDebug::dPrint("ThemePath:->".path_to_theme());
        $var='<head>';
        $var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/html-elements.css?w" />';
        $var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/style.css?w" />';
        $var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/style-widget.css?w" />';
        $var.='<link type="text/css" rel="stylesheet" media="all" href="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/jqgrid/steel/grid.css?w" />';
        $var.='</head>';
        $var.='<body style="background-color:#ffffff;padding: 20px;padding-top:0px;width:1024px;">';
        //$var.='<div id="header"><img id="logo" alt="Home" src="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/exone.jpg" width="132" height="68" /></div>';

        expDebug::dPrint("HEAD INFO:->".$var);
        return $var;
    }

    private function generateHtmlBody($body){
        //$var ='<body style="background-color:#ffffff;padding: 20px;width:800px;">';
         //$pdf = pdf_new();
        $var='';
        //$var.='<div id="CatalogServiceTemplatePreview" class="preview-title" style="padding: 20px;"><div id="CatalogServiceActionTop"><div id="TemplatePreviewContent"><div id="CatalogServiceTemplateDt"><div><div class="itemTitle"><b>**Test ILT Course</b></div><div class="itemTitle">-</div><div class="itemType">TestILTCourse</div><div style="overflow: hidden; height: 10px;"></div><div class="itemBody"><b>Tags:</b> </div><div style="overflow: hidden; height: 10px;"></div><div class="itemBody"><b>Description:</b> </div><div style="overflow: hidden; height: 10px;"></div><div class="itemStatus"><b>Status:</b> Active</div><div style="overflow: hidden; height: 10px;"></div><div class="itemBody"><b>Registration Statistics:</b></div><div class="itemBody"><b>In progress</b>: 0, <b>Completed</b>: 0, <b>Waitlist</b>: 0, <b>Cancelled</b>: 0</div><div style="overflow: hidden; height: 10px;"></div><div class="itemBody"><b>Price:</b>  ()</div><div style="overflow: hidden; height: 10px;"></div><div class="itemBody"><b>Training Units:</b> </div></div></div><div id="CatalogServiceProfileDt"></div><div id="CatalogServiceOfferingDt"><div style="margin-top: 20px;"><fieldset><legend><b>Catalog Items</b></legend><div><div class="itemTitle"><b><a href="javascript:void(0);">TestCourse1</a></b></div><div class="itemTitle">-</div><div class="itemType">WBT</div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> CR0001</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Description for testcourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemLanguage"><b>Language:</b> English</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Active</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Registration Statistics:</b> </div><div class="itemRegns"><b>In progress</b>: 1, <b>Completed</b>: 2, <b>Waitlist</b>: 3, <b>Cancelled</b>: 4</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Session Detail:</b></div><div id="SD1"><div class="session-view"><div><table style="border-collapse: collapse;" id="CatalogServiceaddWBTLesson"><tbody><tr><td class="sp_label session-view-header" style="width: 325px;">Name</td><td class="sp_label session-view-header" style="width: 150px;">Pass %</td><td class="sp_label session-view-header" style="width: 150px;">Maximum Attempts</td></tr><tr id="0_iltSesRec" class="session-view-record"><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>What is New in AutoCAD Map 2010</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>40</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>5</div></td></tr></tbody></table></div></div></div></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">TestCourse1</a></b></div><div class="itemTitle">-</div><div class="itemType">VC</div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> CR0001</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Description for testcourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemLanguage"><b>Language:</b> French</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Active</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Registration Statistics:</b> </div><div class="itemRegns"><b>In progress</b>: 1, <b>Completed</b>: 2, <b>Waitlist</b>: 3, <b>Cancelled</b>: 4</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Session Detail:</b></div><div id="SD2"><div class="session-view"><div><table style="border-collapse: collapse;" id="CatalogServiceaddILTSession"><tbody><tr class="session-view-header"><td class="sp_label session-view-header">Start Date &amp; Time</td><td class="sp_label session-view-header">End Date &amp; Time</td><td class="sp_label session-view-header">URL</td><td class="sp_label session-view-header">Time Zone</td><td class="sp_label session-view-header">Instructor</td></tr><tr></tr><tr id="0_iltSesRec" class="session-view-record"><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>2009-11-29 17:53:00</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>2009-11-29 17:53:00</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>http://yahoo.com</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>Middle East Time - GMT+3:30</div></td><td class="sp_field session-view-record" style="border-bottom: 0px none;"><div>dd</div></td></tr></tbody></table></div></div><div class="session-view"><table style="border-collapse: collapse;"><tbody><tr class="session-view-footer"><td class="sp_label session-view-footer" style="width: 70px;">Capacity Min:</td><td style="width: 50px;" class="session-view-footer"><div>12</div></td><td class="sp_label session-view-footer" style="width: 35px;">Max:</td><td style="width: 50px;" class="session-view-footer"><div>12</div></td><td class="sp_label session-view-footer" style="width: 90px;">WaitList Seats:</td><td class="session-view-footer"><div>12</div></td><td class="sp_label session-view-footer" style="width: 40px;">Type:</td><td class="session-view-footer"><div>Centra</div></td></tr></tbody></table></div></div></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">**Offering of Test ILT Course</a></b></div><div class="itemTitle">-</div><div class="itemType">Class</div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTClass</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemLanguage"><b>Language:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Active</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Registration Statistics:</b> </div><div class="itemRegns"><b>In progress</b>: 1, <b>Completed</b>: 2, <b>Waitlist</b>: 3, <b>Cancelled</b>: 4</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Session Detail:</b></div><div id="SD88"><div class="sp_erromsg" style="margin-left: 3px; margin-bottom: 5px;">No items are defined in this course.</div></div></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">**Test ILT Course</a></b></div><div class="itemTitle">-</div><div class="itemType">ILT</div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemLanguage"><b>Language:</b> English</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Draft</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Registration Statistics:</b> </div><div class="itemRegns"><b>In progress</b>: , <b>Completed</b>: , <b>Waitlist</b>: , <b>Cancelled</b>: </div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Session Detail:</b></div><div id="SD1330"><div class="sp_erromsg" style="margin-left: 3px; margin-bottom: 5px;">No items are defined in this course.</div></div></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">**Test ILT Course</a></b></div><div class="itemTitle">-</div><div class="itemType">WBT</div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemLanguage"><b>Language:</b> English</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Draft</div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Registration Statistics:</b> </div><div class="itemRegns"><b>In progress</b>: , <b>Completed</b>: , <b>Waitlist</b>: , <b>Cancelled</b>: </div><div style="overflow: hidden; height: 5px;"></div><div class="ItemStats"><b>Session Detail:</b></div><div id="SD1331"><div class="sp_erromsg" style="margin-left: 3px; margin-bottom: 5px;">No lessons are defined in this course.</div></div></div><div class="sp_rowseparator"></div></fieldset></div></div><div id="CatalogServicePreReqDt"><div style="margin-top: 20px;"><fieldset><legend><b>Pre-Requisite</b></legend><div><div class="itemTitle"><b><a href="javascript:void(0);">**Test ILT Course</a></b></div><div class="itemCode" style="float: right;"><b>Required For:</b> Mandatory</div><div class="itemCode"><b>Tags:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Title for the course **Test ILT Course and course code is TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Published</div><div style="overflow: hidden; height: 5px;"></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">zVidya_Test</a></b></div><div class="itemCode" style="float: right;"><b>Required For:</b> Mandatory</div><div class="itemCode"><b>Tags:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> Vi_001</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Title for the course zVidya_Test and course code is Vi_001</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Published</div><div style="overflow: hidden; height: 5px;"></div><div class="sp_rowseparator"></div></div></div></fieldset></div></div><div id="CatalogServiceEquivalanceDt"><div style="margin-top: 20px;"><fieldset><legend><b>Equivalence</b></legend><div><div class="itemTitle"><b><a href="javascript:void(0);">**Test ILT Course</a></b></div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Tags:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Title for the course **Test ILT Course and course code is TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Published</div><div style="overflow: hidden; height: 5px;"></div><div class="sp_rowseparator"></div><div><div class="itemTitle"><b><a href="javascript:void(0);">**Test ILT Course</a></b></div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Tags:</b> </div><div style="overflow: hidden; height: 5px;"></div><div class="itemCode"><b>Code:</b> TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemBody"><b>Description:</b> Title for the course **Test ILT Course and course code is TestILTCourse</div><div style="overflow: hidden; height: 5px;"></div><div class="itemStatus"><b>Status:</b> Published</div><div style="overflow: hidden; height: 5px;"></div><div class="sp_rowseparator"></div></div></div></fieldset></div></div></div>';
       $var .= $body;
       // $var .=pdf_show_xy($pdf,$body,50,750);
        $var.='</body>';
        expDebug::dPrint("HEAD INFO:->".$var);
        return $var;
    }
    private function generateHtmlFooter()
    {
    	$var ='<div id="footer"><div class="copyright-page"><div style="height: 12px;">Copyright 2010 Expertus, Inc. All rights reserved.Privacy Policy - Legal Notices &amp; Trademark - Report Piracy - Sitemap.</div><div style="text-align: right; position: relative; top: -17px;"><img src="'.$this->tmp_url.'sites/'.$this->site_name.'/themes/SmartTheme/images/expertus_power.png"></div></div></div>';
    	return $var;
    	
    }
}

$obj = new PDFTemplate();
$obj->createHtml();
echo "Process Done. File created.";
?>