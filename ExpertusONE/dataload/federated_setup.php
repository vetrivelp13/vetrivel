<?php 

include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";
require_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Encryption.php";

$cert = isset($_COOKIE['SPCertificate'])?$_COOKIE['SPCertificate']:'';
$pinfo = isset($_COOKIE['SPLearnerInfo'])?$_COOKIE['SPLearnerInfo']:'';
// Check if the user is logged
if(empty($cert) || empty($pinfo)) {print 'You are not loged in'; exit;}

$dcre = new Encrypt();
$cookVal = explode("##",$dcre->Decrypt($pinfo));
if($cookVal[0]!=1)
{
  print "You are not Authorized to access this Service."; 
}
else
{
  // Federated table creation starts
  
/*	include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/GlobalUtil.php";
	include_once $_SERVER['DOCUMENT_ROOT']."/sites/all/services/Trace.php";*/
	include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";
	
	$util=new GlobalUtil();
	$config=$util->getConfig();
	$fedURL = $config['FED_db_url'];
	$fedxURL = $config['FEDX_db_url'];
	
	$masterURL = $config['db_url'];
	$slaveURL = $config['report_db_url'];

	// Check federate params are set
	if(empty($fedURL) || empty($fedxURL)) {print 'Federated database params are not set'; exit;}
	
	// Check database params are set
	if(empty($masterURL) || empty($slaveURL)) {print 'Database params are not set'; exit;}
	
	try{
		register_shutdown_function('fatalErrorHandler');
		$mdb = new DLDatabase(); // DB connection for source (Master)
		$fed = new DLDatabase('DL'); // DB connection for federated 
		$fedx = new DLDatabase('DL','execute'); // DB connection for federated exec
		
		$info = parse_url($slaveURL);
		$qry = "select distinct base_table from slt_dataload_entity union select table_name as base_table from INFORMATION_SCHEMA.TABLES where  TABLE_SCHEMA = '".basename($info['path'])."' and table_name IN ('variable','slt_country','slt_state','taxonomy_term_data','taxonomy_term_hierarchy','slt_entity_profile_mapping','users')";
		expDebug::dPrint($qry);
		$mdb->callQuery($qry);
		$tbls = $mdb->fetchAllResults();
		expDebug::dPrint($tbls);
		$jobtbls = array(
				'slt_dataload_batches',
				'slt_dataload_entity',
				'slt_dataload_jobs',
				'slt_dataload_process_queue',
				'slt_dataload_table_mapping'
			);
		
		foreach($tbls as $col=>$tbl){
			$qry = 'SHOW CREATE TABLE '.$tbl->base_table;
			$mdb->callQuery($qry);
			$tblsc = $mdb->fetchAssociate();
			$dbsrc = $tblsc['Create Table'];
			expDebug::dPrint($dbsrc,5);
			
			$crtQry = substr($dbsrc,0,strrpos($dbsrc,')')).' ) ';
			$crtQry .= 'ENGINE=FEDERATED ';
			$crtQry .= 'DEFAULT CHARSET=utf8 ';
			
			$fedQry = $crtQry . "CONNECTION='" . $slaveURL ."/" .$tbl->base_table. "' ; ";
			$fedxQry = $crtQry . "CONNECTION='" . $masterURL ."/" .$tbl->base_table. "' ; ";
			
			//Drop before create if exists
			$fed->callExecute('DROP TABLE IF EXISTS '.$tbl->base_table);
			$fedx->callExecute('DROP TABLE IF EXISTS '.$tbl->base_table);
			
			expDebug::dPrint($fedQry,5);
			
			// Create federated tables
			$fed->callExecute($fedQry);
			$fedx->callExecute($fedxQry);
		}
		
		foreach($jobtbls as $tbl){
			$qry = 'SHOW CREATE TABLE '.$tbl;
			$mdb->callQuery($qry);
			$tblsc = $mdb->fetchAssociate();
			$dbsrc = $tblsc['Create Table'];
			expDebug::dPrint($dbsrc,5);
			
			$crtQry = substr($dbsrc,0,strrpos($dbsrc,')')).' ) ';
			$crtQry .= 'ENGINE=FEDERATED ';
			$crtQry .= 'DEFAULT CHARSET=utf8 ';
			
			//$fedQry = $crtQry . "CONNECTION='" . $slaveURL ."/" .$tbl. "' ; ";
			$fedxQry = $crtQry . "CONNECTION='" . $masterURL ."/" .$tbl. "' ; ";
			
			//Drop before create if exists
			$fed->callExecute('DROP TABLE IF EXISTS '.$tbl);
			$fedx->callExecute('DROP TABLE IF EXISTS '.$tbl);
			
			expDebug::dPrint($fedxQry,5);

			// Create federated tables
			$fed->callExecute($fedxQry);
			$fedx->callExecute($fedxQry);
		}
		print "Tables created successfully.";
	}catch(Exception $e){
		expDebug::dPrint("Error in federated table creations --- ".$e->getMessage(),1);
		print "Error in table creation. Contact system administrator.";
	}
}

function fatalErrorHandler(){
	$error = error_get_last();
	if(($error['type'] === E_ERROR) || ($error['type'] === E_USER_ERROR)){
		expDebug::dPrint("Unexpected error in federated table creations --- ".print_r($error,true),1);
		print "Error in table creation. Contact system administrator.";
	}
}
?>