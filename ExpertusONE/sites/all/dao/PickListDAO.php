<?php
/**
* @class or PHP Name		: PickListDAO
* @author(s)				: Vincent.S
* Version         			: 1.0
* Date						: 10/07/2009
* PHP Version          		: 5.2.6
* Description     			: DAO Class to do basic CRUD operations in user table.
*/

include_once "AbstractDAO.php";

class PickListDAO extends AbstractDAO{
	public function __construct(){
		parent::__construct();
	}
	
	public function fetchList($param){
		try{
			//$qry = " select a.id Id,a.name Name,a.listid ListId from slt_profile_list_items a, slt_profile_list b ";
		//	$qry .= " where a.listid=b.id and lower(b.name) like lower('%".$name."%') ";
			$qry = " call slp_list_items_sel(";
			$qry .= ($param->Code==null || $param->Code == 'undefined' || $param->Code == '')?"null":"'".$param->Code."'";
			$qry .= ($param->Language==null || $param->Language == 'undefined' || $param->Language == '')?",null":",'".$param->Language."'";
			$qry .= ")";
			expDebug::dPrint("PickListDAO fetchList Query :".$qry);
			$this->connect();
			$res = $this->query($qry);
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
}
?>