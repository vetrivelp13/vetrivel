<?php

/*
 * exp_sp_printpdftemplate.inc - Template to print data to PDF.
 */

/*
 * Preprocessor
 * defines the font path of the fpdf library
 * includes the fpdf.php
 * includes the exp_sp_administration.inc
 */
define('tFPDF_FONTPATH',$_SERVER['DOCUMENT_ROOT'].'/sites/all/commonlib/fpdf17/font/unifont');
require($_SERVER['DOCUMENT_ROOT'].'/sites/all/commonlib/fpdf17/tfpdf.php');
include_once('exp_sp_administration.inc');

/*
 * Class AdminPDF extending tFPDF
 */
class AdminPDF extends tFPDF{

	// Global Variables used to specify data and dimensions.
	// all dimensions are in mm.
	var $availWidth = 0; //the available width
	var $footerHeight = 0; // height of the footer 
	var $topMargin = 0;	//top margin
	var $rightMargin = 0;	//right margin
	var $leftMargin = 0;	//left margin
	var $bottomMargin = 0;	//bottom margin
	var $lineHeight = 0; //line height
	var $searchName = ''; //the search name
	var $pageWidth = 0; //page width
	var $pageHeight = 0; //page height
	var $availHeight = 0; //available height
	var $maxLines = 0;// maximum lines per page.
	var $imgPath = '';//header image path default : null
	var $imgW = 0;// header image width. default : 0
	var $imgH = 0;// header image height. default : 0
	var $printInAll = ''; // character flag to print the header in all the pages and setting the printFlag. default : 'N'
	var $printFlag = true;// boolean print flag, true if header is required in all the pages, else flase; default : false
	var $appliedFilters = array(); //the applied search filters
	var $newHeaderMap = array(); // global variable to print the table header
	var $headerCellWidth = array(); // width array to store the cell width.
	var $tblFontSize = 0;	//font size
	var $variableWidth = 0;	//the variable width of the table cols
	var $imgPos = 0;	//image position
	var $regularFont = ''; // regular ttf font basename
	var $boldFont = ''; // bold ttf font file basename

	/*
	 * constructor
	 * @params:
	 * 
	 * $orientation			:	'P' or 'L'
	 * $size						:	'A4' or 'Letter' or 'Legal'
	 * $name						:	name of the search performed
	 * $imgP						:	path of the header image.
	 * $imgW 						:	width of the header image.
	 * $imgH						:	height of the header image.
	 * $iPos						: position of the image (left, right, center, stretch)
	 * $printInAll 			:	flag to specify whether the header is required in all pages or not. 'Y' or 'N'
	 * $appliedFilters	: array of the search filters that are applied.
	 * $lm 							: left margin
	 * $rm = 						:	right margin
	 * $tm = 						: top margin
	 * $bm = 						: bottom margin
	 * $fs							:	Font Size ( in pts )
	 */
	function AdminPDF($orientation='P', $size='A4', $name, $imgP = 'noimg', $imgW = 0, $imgH = 0,$iPos = 'stretch', $printInAll = 'N', $appliedFilters=array(),$lm = 10,$rm = 10,$tm = 10, $bm = 10,$fs = 11){
		
	try{
		// Calling the tFPDF's parent constructor to make the page ready for printing.
		$this->tFPDF($orientation, 'mm', $size);
		
		// Setting the image width, height and path.
		$this->imgW = $imgW;
		$this->imgH = $imgH;
		$this->imgPath = $imgP;
		
		//setting the printFlag depending upon printInAll.
		if($printInAll == 'Y'){
			$this->printInAll = $printInAll;
		} else {
			$this->printInAll = 'N';
		}
		
		$this->tblFontSize = $fs;	// font size in pt
		$this->lineHeight = round(24.5 * ($this->tblFontSize / 72)) + 2;	// (25.4 * (1/72)) mm = 1 pt, (25.4 * (x/72)) mm = x pt
		$this->searchName = $name;		// setting the search name
		$this->pageWidth = $this->w; 	// setting the pagewidth
		$this->pageHeight = $this->h;	// setting the page height
		$this->topMargin = $tm;	//setting the top margin
		$this->bottomMargin = $bm;	//setting the bottom margin
		$this->leftMargin = $lm;	//setting the left margin
		$this->rightMargin = $rm;	//setting the right margin
		$this->footerHeight = $this->bottomMargin;
		//calculating the available page width. (availablewidth = wholewidth - margins)
		$this->availWidth = $this->pageWidth - ($this->leftMargin +  $this->rightMargin );
		//calculating the available page height. (availableheight  = pageheight - margins - footerheight)
    $this->availHeight = floor($this->pageHeight) - ($this->topMargin + $this->footerHeight);
    //calculating the maximum lines that the page can hold. (maxlines = availableheight/lineheight)
		$this->maxLines = floor($this->availHeight / $this->lineHeight); //four lines are subracted from the maximum lines.
		//checking for image position
		if($iPos == 'center'){
			$cent = $this->pageWidth/2;	//page center
			$icent = $this->imgW / 2;		//image center
			$this->imgPos = $cent - $icent; // new position  = page center - image center 
			if($imgW > $this->availWidth)
				$this->imgPath = 'noimg';
		} else if($iPos == 'left'){
			$this->imgPos = $this->leftMargin;
		} else if($iPos == 'right'){
			$this->imgPos = $this->availWidth +$this->leftMargin - $this->imgW;
		} else {
			$this->imgPos = $this->leftMargin;
			$this->imgW = $this->availWidth;
		}
		//checking for wrong image or the image height that exceeds the available height.
		if(($imgW == 0 || $imgH == 0) || ($imgW > $this->availWidth || $imgH > $this->availHeight ))
			$this->imgPath = 'noimg';

		$this->appliedFilters = $appliedFilters;
		expDebug::dPrint("appliedFilters>>>>".$this->appliedFilters , 4);
		$this->SetMargins($this->leftMargin,$this->topMargin,$this->rightMargin);
		$this->footerHeight = $this->bottomMargin;
		$this->SetAutoPageBreak(true,$this->footerHeight);
		require_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/services/GlobalUtil.php";
    $util=new GlobalUtil();
    $config=$util->getConfig();
    expDebug::dPrint('$config = ' . print_r($config, true), 5);
    $regularFont = '';//$config["pdf_font_regular"];
    $boldFont = '';//$config["pdf_font_bold"];
    if (!empty($regularFont)) {
      $this->AddFont($regularFont, '', $regularFont . '.ttf', true);
      $this->regularFont = $regularFont;
      $this->boldFont = $regularFont;
      if (!empty($boldFont)) {
        $this->AddFont($boldFont, '', $boldFont . '.ttf', true);
        $this->boldFont = $boldFont;
      }
    }
    else {
      $this->AddFont('DejaVu','','DejaVuSans.ttf',true);
      $this->AddFont('DejaVuB','','DejaVuSans-Bold.ttf',true);
      $this->regularFont = 'DejaVu';
      $this->boldFont = 'DejaVuB';
    }
    } catch (Exception $ex) {
    	watchdog_exception('AdminPDF', $ex);
    	expertusErrorThrow($ex);
    }
	}

	/*
	 * default header override
	 */
	function header(){
		try{
		//checking for the image and printing the image
		if($this->imgPath != 'noimg' && $this->printFlag){
			$this->Image($this->imgPath,$this->imgPos,$this->topMargin,$this->imgW,$this->imgH);
			$this->Ln($this->imgH);
		}
		$hLineheight = 0;
		//printing the search options
		if($this->printFlag){ 

			// setting font properties
			$this->SetFont($this->boldFont, '', $this->tblFontSize + 3);
			//printing the search name
			$this->Cell($this->availWidth,$this->lineHeight+5,$this->searchName,0,1,'C');
			//resetting the font.
			$this->SetFont($this->regularFont, '', $this->tblFontSize);
			$prevY = $this->GetY();
			$curY = 0;

			//printing the search options as multicell data.
			foreach($this->appliedFilters as $key => $val){
			  $this->MultiCell($this->availWidth, $this->lineHeight, $key . " : " . $val, 0,'L');
			  //getting the current y position
			  $curY=$this->GetY();
			  //calculating the line height
	      $hLineheight+=$curY-$prevY;	
	      //setting the current y position to prevy
	      $prevY=$this->GetY();	
			}
			$this->Ln($this->lineHeight);
		}
		//recalculating the available height and maximim lines in a page.
		$this->availHeight = floor($this->pageHeight) - ($this->imgH + $hLineheight + $this->topMargin + $this->footerHeight);	
		$this->maxLines = floor($this->availHeight / $this->lineHeight);
	  if($this->printInAll == 'N'){
			$this->printFlag = false;
		}
		} catch (Exception $ex) {
			watchdog_exception('header', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * default footer override.
	 */
	function footer(){
		try{
		//setting the font
        $this->SetFont($this->regularFont, '', $this->tblFontSize);
		$this->SetY(-($this->bottomMargin));
		$this->Cell($this->availWidth,$this->lineHeight,t('LBL989').' '.$this->PageNo().' ' .t('LBL981').' '. '{nb}','T',1,'C');
		} catch (Exception $ex) {
			watchdog_exception('footer', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * calculating the width of each cell
	 * @param:
	 * $headerMap	: the array of the table header items
	 * 
	 * @return:
	 * $colWidthSet	: the column width array
	 */
	function calcCellWidth($headerMap){
		try{
		$this->calcVariableWidth($headerMap);
		$colWidthSet = array();
		foreach($headerMap as $key => $val){
			if(isset($val[1])){
				$colWidthSet[$val[0]] = $this->convertTomm($val[1]);
			}
		}
		$colWidthSet['username'] = '25';
		expDebug::dPrint("colwidthset>>>".print_r($colWidthSet,true) , 4);
		return $colWidthSet;
		} catch (Exception $ex) {
			watchdog_exception('calcCellWidth', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * variable unit conversion
	 * @params:
	 * $data	:	the cellwidth
	 * 
	 * @return:
	 * $mmVal	:	mm equivalent of the given cellwidth
	 */
	function convertTomm($data){
		try{
		$mmVal = 0;
		$mmPos = strpos($data,'mm');
		if($mmPos === false){
			$mmVal = $this->percentageToMm($data);
		}	else	{
			$mmVal = intval($data);
		}
		return $mmVal;
		} catch (Exception $ex) {
			watchdog_exception('convertTomm', $ex);
			expertusErrorThrow($ex);
		}
	}
	
	/*
	 * calculating the available variable width
	 * @params:
	 * $headermap	:	the cell width mixed array 
	 */
	function calcVariableWidth($data){
		try{
		$totWidth = 0;
		foreach($data as $key => $val){
			$mmPos = strpos($val[1],'mm');
			if($mmPos === false){
				//do nothing
			} else {
				$totWidth += intval($val[1]);
				
			}
		}
		$this->variableWidth = $this->availWidth - $totWidth;
		} catch (Exception $ex) {
			watchdog_exception('calcVariableWidth', $ex);
			expertusErrorThrow($ex);
		}
	}
	/*
	 * conversion of percentage to mm
	 * @param:
	 * $data	: col width percentage
	 * $aWidth:	available width
	 * 
	 * @return:
	 * $retData	:  mm width
	 */
	function percentageToMm($data){
		try{
		$retData =0;
		$retData = ($data * $this->variableWidth)/100;
		 expDebug::dPrint("varwidth test1>>>>".$this->variableWidth." retdata>>>>".$retData , 4);
		return $retData;
		} catch (Exception $ex) {
			watchdog_exception('percentageToMm', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * 	calculate the printing truncation
	 * @param:
	 * $str		: the full string
	 * $char	: the number of maximum characters
	 * 
	 * @return:
	 * $newStr	: the truncated string
	 */
	function truncatePrintText($str,$charlen){
		try{
		$maxlength = $charlen;
		expDebug::dPrint("maxcharacters of '$str' : ".$strlength , 4);
		$newStr = $str;
		if($maxlength != 0){
			$newStr = substr($str,0,$maxlength-5);
			$newStr .="...";
		}
		return $newStr;
		} catch (Exception $ex) {
			watchdog_exception('truncatePrintText', $ex);
			expertusErrorThrow($ex);
		}
	}


	/*
	 * calculating number of lines in multicell
	 * 
	 * this function is a variant of the MultiCell function available in the FPDF library
	 * 
	 * @param:
	 * $width	: column width
	 * $str		: the string for which maxlines has to be calculated.
	 * 
	 * @return:
	 * an array containing $nLines and $maxCharacters
	 */
	function calcLines($width, $str){
		try{
			$maxCharacters; //max characters
			
			$cw = &$this->CurrentFont['cw']; //current font width
			
			if($width == 0)
				$width = $this->w - $this->rMargin - $this->x; //if width is 0 then it will take the available page width as width
			
			$wmax = ( $width - 2 * $this->cMargin );
			$s = str_replace("\r", '', $str);
			if ($this->unifontSubset) {
				$nb=mb_strlen($s, 'utf-8');
				while($nb>0  &&  mb_substr($s,$nb-1,1,'utf-8') == "\n")	
					$nb--;
			} else {
				$nb = strlen($s);
				if($nb>0 && $s[$nb-1]=="\n")
					$nb--;
			}
			$sep = -1;
			$i = 0;
			$j = 0;
			$l = 0;
			$ns = 0;
			$nLines = 1;
			
			while($i < $nb) {
					if ($this->unifontSubset) {
				$c = mb_substr($s,$i,1,'UTF-8');
			}
			else {
				$c=$s[$i];
			}
			
				if($c == "\n") {
					$i++;
					$sep = -1;
					$j = $i;
					$l = 0;
					$ns = 0;
					$nLines++;
					continue;
				}
				
				if($c == ' '){
					$sep = $i;
					$ls = $l;
					$ns++;
				}
				
				if ($this->unifontSubset) { 
					$l += $this->GetStringWidth($c); 
				} else { 
					$l += $cw[$c]*$this->FontSize/1000;
				}
				if($l > $wmax) {
					if($sep == -1) {
						if($i == $j)
							$i++;
					} else {
						$i = $sep+1;
					}
					$sep = -1;
					$j = $i;
					$l = 0;
					$ns = 0;
					$nLines++;
				} else
					$i++;
				//return the array of lines and max characters if maxlines is reached.
				if($nLines == $this->maxLines){
					$maxCharacters = $i;
					return array($nLines,$maxCharacters);
				} else if($nLines<$this->maxLines) {
					$maxCharacters = 0;
				} else {
					//do nothing
				}
			}
			expDebug::dPrint("the number of lines of $str in Multi cell is ".$nb , 4);
			expDebug::dPrint("the number of maximum characters in $str in Multi cell is ".$i , 4);
			return array($nLines,$maxCharacters);
			} catch (Exception $ex) {
				watchdog_exception('calcLines', $ex);
				expertusErrorThrow($ex);
			}
		}

	/*
	 * check page break
	 * @param:
	 * $h	:	the height of the current row.
	 */
	function CheckPageBreak($h){
		try{
		if($this->GetY()+$h > $this->PageBreakTrigger){
		  $this->AddPage($this->CurOrientation);
		  $this->printMultiCellTableHeader($this->newHeaderMap,$this->headerCellWidth, array('0'),"N"); //printing the table header after adding a new page
		}
		
		} catch (Exception $ex) {
			watchdog_exception('CheckPageBreak', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * printing a multicell row
	 * @param:
	 * $data			:	the data to be printing
	 * $colWidth	:	array of column widths
	 */
	function Row($data = array(),$colWidth = array()){
		try{
		$nb=0;
		$maxCharSet = array();
		foreach($data as $key => $val){
				$maxCharSet[$key] = $this->calcLines($colWidth[$key], $val);	//calculating the number of lines each record would take
				$nb=max($nb, $maxCharSet[$key][0]);
		}
		//calculate the height of the row
		$height=$this->lineHeight*$nb;
		//check for page break
		$this->CheckPageBreak($height);
		//print the multicell table row
		foreach($data as $key => $val){
				$width=$colWidth[$key];	//width of the current column
				$xPos=$this->GetX();	//x position of the current column
				$yPos=$this->GetY();	//y position of the current column
				$this->Rect($xPos, $yPos, $width, $height);	//draw the cell(i.e. column)
				$this->MultiCell($width, $this->lineHeight, $this->truncatePrintText($val,$maxCharSet[$key][1]), 0,'L'); //print the text
				$this->SetXY($xPos+$width, $yPos);	//reset the x position and y position to print the next column.
		}
		$this->Ln($height);
		} catch (Exception $ex) {
			watchdog_exception('Row', $ex);
			expertusErrorThrow($ex);
		}
	}
	

	/*
	 * printMultiCellTable	:	entry point to start printing the table.
	 * @param:
	 * $data			:	the data array
	 * $headermap	:	the table headers
	 */
	function printMultiCellTable($data = array(), $headermap = array()){
		try {
		$this->AddPage();
		$cellWidth = $this->calcCellWidth($headermap);
		$this->printMultiCellTableHeader($headermap,$cellWidth, $data);
		} catch (Exception $ex) {
			watchdog_exception('printMultiCellTable', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * printing the table header
	 * @param:
	 * $headermap	:	header array
	 * $cellWidth	:	cell widths
	 * $data			:	data array
	 * $firstTime	:	whether the data is printed for the first time.
	 */
	function printMultiCellTableHeader($headermap = array(),$cellWidth, $data = array(),$firstTime="Y"){
		try{
		//setting the font
		$this->SetFont($this->boldFont, '', $this->tblFontSize);
		//checking for the first time
		if($firstTime=="Y"){
			foreach($headermap as $header => $val){
				$this->newHeaderMap[$val[0]] = $header;
			}
		    $this->headerCellWidth=$cellWidth;
		}
		
		$this->Row($this->newHeaderMap,$cellWidth);	
		//reset the font
		$this->SetFont($this->regularFont, '', $this->tblFontSize);
		if($firstTime=="Y"){
	      $this->printMultiCellTableBody($data, $cellWidth, $this->newHeaderMap);//print the table body
		}
		} catch (Exception $ex) {
			watchdog_exception('printMultiCellTableHeader', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * printing the table body
	 * @param:
	 * $data			:	the data array.
	 * $cellWidth	:	the cell width array.
	 * $headermap	:	headermap array to fetch the keys.
	 */
	
	function printMultiCellTableBody($data = array(), $cellWidth, $newHeaderMap){
		try{
		//calculating the record count
		$this->recordCount = count($data);
		//setting the font
        $this->SetFont($this->regularFont, '', $this->tblFontSize);
		foreach($data as $key => $val){
			$newVal = array();
			//preparing the data array for printing
			foreach($newHeaderMap as $key => $value){
				$newVal[$key] = $val[$key];
			}
			expDebug::dPrint('new Val > '.print_r($newVal,true) , 4);
			$this->Row($newVal,$cellWidth);	//printing the row
		}
		$this->postProcessPDF(); //postprocessing functions
		} catch (Exception $ex) {
			watchdog_exception('printMultiCellTableBody', $ex);
			expertusErrorThrow($ex);
		}
	}

	/*
	 * post processing the pdf
	 */
	function postProcessPDF(){
		try{
		$this->AliasNbPages(); //counting the number of pages
		$this->SetDisplayMode('fullpage');//displaying the pages in the pdf reader.
		} catch (Exception $ex) {
			watchdog_exception('deleteDrupalUsers', $ex);
			expertusErrorThrow($ex);
		}
	}
}
?>