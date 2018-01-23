<?php
/**
* @class or PHP Name		: WebServiceDAO
* @author(s)				: Vincent.S
* Version         			: 1.0
* Date						: 09/07/2009
* PHP Version          		: 5.2.6
* Description     			: DAO Class to do basic CRUD operations in user table.
*/


include_once "AbstractDAO.php";

class WebServiceDAO extends AbstractDAO{
	
	public function __construct(){
		parent::__construct();
	}
	
	public function fetch($actionkey){
		try{
			
			$qry = "call slp_webservice_sel(";
			$qry .= ($actionkey!=null || $actionkey!='')?'"'.$actionkey.'"':'null';
			$qry .= ");";
			expDebug::dPrint("WebServiceDAO Query :".$qry);
			$this->connect();
			$res = $this->query($qry);
			$result = $this->fetchResult();
			
			return $result;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	
}
?>