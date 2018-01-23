<?php

/*
 * get_transcript_details()
 */
function get_transcript_details($argumentType) {
  // Do not update the session table.
  try{
  global $user;
  global $base_url;

	$del_type_image = array("lrn_cls_dty_ilt" => "ilt-icon", "lrn_cls_dty_wbt" => "wbt-icon", "lrn_cls_dty_vod" => "vod-icon", "lrn_cls_dty_vcl" => "vcl-icon","lrn_cls_dty_tpm" => "lrn-pln-icon", "cre_sys_obt_trn" => "lrn-pln-icon", "cre_sys_obt_cur" => "curr-icon","cre_sys_obt_crt" => "cert-icon");

  // Solr Check starts here
  $srcSource = getConfigValue('solr_search');
  $solrResult = FALSE;
  // Solr is turned on
  if($srcSource == 1){
  	require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_mytranscript.php';
  	try{
  		expDebug::dPrint("Solr Search Testing",1);
  		$solrObj = new MyTranscriptSolrSearch();
  		$getTransQryRes = $solrObj->TrasncriptSolrSearch();
  		expDebug::dPrint('Complete Result set from solr'.print_r($getTransQryRes,true),5);
  		$myTranscriptList = $getTransQryRes['result'];
  		expDebug::dPrint('TranscriptList Result set from solr'.print_r($myTranscriptList,true),5);
  		$myTranscriptCount = $getTransQryRes['count'];
  		expDebug::dPrint('Count Result set from solr'.print_r($myTranscriptCount,true),5);
  		$page    = $_GET['page'];
  		$total_pages  = ceil($myTranscriptCount/$_GET['rows']);
  		$solrResult = TRUE;
  	}
  	catch(Exception $e){
  		expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
  	}
  	expDebug::dPrint("Solr search result - Final ".print_r($getTransQryRes,1),4);
  }
  // Solr is turned Off
  else {
  	$page = $_GET['page']; // get the requested page
  	$limit = $_GET['rows'];
  $userId = getSltpersonUserId();
  
  $db1 = db_select('slt_enrollment','enrl');
  $db1->join('slt_course_class','crls','enrl.class_id = crls.id');
  $db1->leftJoin('slt_profile_list_items','prof','prof.code = enrl.grade');
  $db1->leftjoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=crls.delivery_type');
  $db1->addField('crls','title','title');
  $db1->addField('enrl','id','enroll_id');
  $db1->addField('enrl','comp_date','comp_date');
  $db1->addField('enrl','class_id','class_id');
  $db1->addField('enrl','course_id','course_id');
  $db1->addField('splt_deliverytype','name','object_type_name');
  $db1->addField('crls','delivery_type','delivery_type');
  $db1->addField('enrl','score','score');
  $db1->addField('prof','name','grade');
  $db1->addExpression("'CLASS'", 'type');
  $db1->condition('enrl.user_id', $userId);
  $db1->condition('enrl.comp_status', 'lrn_crs_cmp_cmp');
  $db1->condition('enrl.reg_status', 'lrn_crs_reg_cnf');
  
  /*changes by Rajkumar U
  to avoid duplicate records, the result set is filtered with class_id */
	//$db1->groupBy('enrl.class_id');
	
  /* UNCOMMENTING THE BELOW LINE WILL KEEP DISPLAYING 
   * CERTIFICATION COMPLETION AND CLASS COMPLETION EXCLUDING THE CLASSES
   * INSIDE THE CERTIFICATION */
  $db1->isNull('enrl.master_enrollment_id'); 
  
  $db2 = db_select('slt_master_enrollment','enrlm');
  $db2->join('slt_program','prog','enrlm.program_id = prog.id');
  $db2->leftjoin('slt_profile_list_items', 'splt_deliverytype1', 'splt_deliverytype1.code=prog.object_type');
  $db2->addField('prog','title','title');
  $db2->addField('enrlm','id','enroll_id');
  $db2->addField('enrlm','comp_date','comp_date');
  $db2->addField('enrlm','program_id','class_id');
  $db2->addField('enrlm','program_id','course_id');
  $db2->addField('splt_deliverytype1','name','object_type_name');
  $db2->addField('splt_deliverytype1','code','delivery_type');
  $db2->addExpression('NULL','score');
  $db2->addExpression('NULL','grade');
  $db2->addExpression("'LP'",'type');
  $db2->condition('enrlm.user_id', $userId);
  $db2->condition('enrlm.overall_status', 'lrn_tpm_ovr_cmp');
  $db2->condition('enrlm.is_current', 'Y');
  $db2->orderBy('comp_date','desc');
  
  $db1->union($db2,'UNION ALL');
  //expDebug::dPrintDBAPI('$db1', $db1);
  $mytranscript = $db1->execute()->fetchAll();
  expDebug::dPrintDBAPI('get transcript details', $db1);
  expDebug::dPrint('transcript details = ' . print_r($mytranscript, true) , 3);
  $myTranscriptCount = count($mytranscript);
  expDebug::dPrint('$myTranscriptCount = ' . $myTranscriptCount , 3);
  
  if($myTranscriptCount > 0) {
    $total_pages = ceil($myTranscriptCount / $limit);
  } else {
    $total_pages = 0;
  }
  
  if ($page > $total_pages) {
    $page = $total_pages;
  }

  $start = (($limit * $page) - $limit); // do not put $limit*($page - 1)
  if ($start <= 0) {
    $start = 0;
  } 

  $db2->range($start, $limit);

  // Execure the query and fetch all the records
  $myTranscriptList = $db1->execute()->fetchAll();
  expDebug::dPrintDBAPI('$db1', $db1);
  expDebug::dPrint('$myTranscriptList = ' . print_r($myTranscriptList, true) , 3);
  }
  // Prepare and send the response
  $response= new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $myTranscriptCount;
  
  $i=0;
  foreach ($myTranscriptList as $currentRecord) {
    $imgPath = $del_type_image[$currentRecord->delivery_type];
    $response->rows[$i]['id'] = core_encrypt($userId); // URL encryption added while print certificate
    $response->rows[$i]['cell']['title'] = $currentRecord->title;
    $response->rows[$i]['cell']['short_title'] = sanitize_data($currentRecord->title);
    $response->rows[$i]['cell']['object_type_name'] = $currentRecord->object_type_name;
    $response->rows[$i]['cell']['enroll_id'] = core_encrypt($currentRecord->enroll_id); // URL encryption added while print certificate
    $response->rows[$i]['cell']['comp_date'] = dateTimeStrToUnivShort($currentRecord->comp_date,true);
    $response->rows[$i]['cell']['course_id'] = $currentRecord->course_id;
    $response->rows[$i]['cell']['class_id'] = core_encrypt($currentRecord->class_id); // URL encryption added while print certificate
    $response->rows[$i]['cell']['score'] = ($currentRecord->score == NULL)? '-' : $currentRecord->score;
    $response->rows[$i]['cell']['grade'] = ($currentRecord->grade == NULL)? '-' : $currentRecord->grade;
    $response->rows[$i]['cell']['type'] = $currentRecord->type;
    $response->rows[$i]['cell']['image_url'] = $imgPath;
    $response->rows[$i]['cell']['argument_type'] = $argumentType;
    
    if(isset($currentRecord->type)) {
    	$entityType = ($currentRecord->type=='CLASS') ? 'cre_sys_obt_crs' : 'cre_sys_obt_trp' ;
    	$templateInfoRec = getTemplateDetails($currentRecord->course_id,$entityType);
    	$cert_status = ($templateInfoRec->mapcount!=0) ? 'Y' : 'N';
    	$response->rows[$i]['cell']['cstatus'] = $cert_status;
    }
    
    $i++;
  }
  expDebug::dPrint('$response = ' . print_r($response, true), 4);
  echo drupal_json_encode($response);
  } catch (Exception $ex) {
  	watchdog_exception('get_transcript_details', $ex);
  	expertusErrorThrow($ex);
  }
}

function getTemplateDetails($entityId,$entityType) {
	$selectTemplate = db_select('slt_entity_notification_mapping', 'nmap');
	$selectTemplate->leftJoin('slt_notification_info', 'ninfo', 'nmap.notify_id = ninfo.id');
	$selectTemplate->addExpression('COUNT(ninfo.id)', 'mapcount');
	$selectTemplate->condition('nmap.entity_id', $entityId);
	$selectTemplate->condition('nmap.entity_type', $entityType);
	$selectTemplate->condition('nmap.notify_send_type', 'Certificate');
	$selectTemplate->condition('ninfo.status', 'cre_ntn_sts_atv');
	$selectTemplate->condition('nmap.status', 'Y');

	expDebug::dPrintDBAPI('transcript.inc: $selectTemplate', $selectTemplate);

	$templateInfoRec1 = $selectTemplate->execute()->fetch();
	
	if ($templateInfoRec1->mapcount<=0) {
		$selectTemplate = db_select('slt_notification_frame', 'nframe');
		$selectTemplate->leftJoin('slt_notification_info', 'ninfo', 'nframe.notification_id = ninfo.id');
		$selectTemplate->leftJoin('slt_profile_list_items', 'spl', 'spl.code = nframe.lang_code');
		$selectTemplate->addExpression('COUNT(ninfo.id)', 'mapcount');
		$selectTemplate->condition('nframe.send_options', 'Certificate', '=');
		$selectTemplate->condition('ninfo.lang_code', 'cre_sys_lng_eng', '=');
		$selectTemplate->condition('ninfo.status', 'cre_ntn_sts_atv');
		$selectTemplate->condition('ninfo.notification_code', 'prncert', '=');
		$templateInfoRec1 = $selectTemplate->execute()->fetch();
	}
	expDebug::dPrint('transcript.inc: $templateInfoRec = ' . print_r($templateInfoRec1, true),5);
	return $templateInfoRec1;
}


?>