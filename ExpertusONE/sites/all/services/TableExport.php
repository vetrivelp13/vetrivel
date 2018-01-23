<?php
/**
* @class or PHP Name		: TableExport Service
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 29/09/2009
* PHP Version          		: 5.2.6
* Description     			: This service is used to get export datatable data as xls
*/

include_once "ServiceBase.php";

class TableExport extends ServiceBase{
	function __construct(){$this->process();}
	
	function process(){
			expDebug::dPrint("Export Testing1");
		try{
			$exportVal=$_POST["content"];
			$exportVal = str_replace("Â®","®",$exportVal);
			$exportVal = str_replace("â„¢","™",$exportVal);
			$exportVal = str_replace("â€“","–",$exportVal);
			$exportVal = str_replace("Â©","©",$exportVal);
			$exportVal = str_replace("<br/>","\n",$exportVal); // This line added by Nanthini for Exporting the Reports for IE
			$val=urlDecode($exportVal);
			$filename="export.csv";
			header("Content-type: application/octet-stream");
			header("Content-Disposition: attachment; filename=\"".$filename."\"");
			echo $val;
			flush();
				
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
}

$obj = new TableExport();
