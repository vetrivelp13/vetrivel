<?php
/**
 * @Desc: To retrieve class details for the given class id
 * @author: Ilayaraja Esackimuthu
 * @since: 27-Jul-2011
 */
function getClassDetailsSurvey($userId,$classid){
  try{  
  //echo drupal_json_encode("Test Result");
  $select = db_select('slt_course_class', 'sl');	
  $select->addField('sl', 'title');    
  $select->condition('sl.id', $classid, '=');    
  $lpQryRes = $select->execute()->fetchAll();
  foreach ($lpQryRes as $record) {
    $classTitle  = $record->title;
  }
  echo drupal_json_encode($classTitle);
  }catch (Exception $ex) {
    watchdog_exception('getClassDetails', $ex);
    expertusErrorThrow($ex);
  }   
}//End of getClassDetails()

/**
 * @Desc: To retrieve assigned surveys for the given class
 * @author: Ilayaraja Esackimuthu
 * @since: 30-Jul-2011
 */
function getAssignedSurveys($userid,$objectId,$objectType,$surveytype='',$preStatus='',$enrollId='',$returnToCode='',$content_player_call=''){
  try{
  	expDebug::dPrint('getMaxAttempts $$$preStatus: '.print_r($preStatus, true) , 4);
  $qryRes      = array();  
  if(!empty($objectId)){

  	
  	if($objectType=="cre_sys_obt_crt") {  		
  	$moduleId = getModuleIdforSurvey($objectId,$userid,$enrollId); 
    
    if(!empty($moduleId)) {    	
    	$module_table = "left join slt_module module on module.id=map.module_id";
    	$module_condition = " module.id = ".$moduleId." and ";    	
    }    
  	
  	}
  	
    $regStatus  = ($objectType  == 'cre_sys_obt_cls')?getEnrollmentStatus($userid,$objectId):'lrn_crs_reg_cnf';
    if(!empty($regStatus) && $regStatus!='NULL'){
    	$asschk = ($preStatus==1) ? 'pre-assessment' :'post-assessment';
      /*//$subQuerySql  = 'select count(id) as surCount from slt_survey_results as surRes where surRes.survey_id = sur.id AND surRes.entity_id='.$classid.' AND surRes.entity_type= '.$objectType.'
		//			AND surRes.user_id='.$userid;
      $query  = db_select('slt_survey', 'sur');
      $query->join('slt_survey_mapping', 'map','sur.id=map.survey_id');  	 
      $query->leftJoin('slt_survey_results', 'surRes','surRes.id=sur.id');
      //$query->add_subquery($subQuerySql,array(),array(),'');
      $query->addField('sur', 'id','surveyId');    
      $query->addField('sur', 'title','surveyTitle');
      $query->addField('sur', 'code','surveyCode');
      $query->addField('map', 'view_option','viewOption');
      $query->addField('map', 'after_completion','afterCompletion');
    //  $query->addField('map', 'object_type','objectType');
     // $query->addField('map', 'object_id','objectId');
      $query->addField('surRes', 'id','Id');
      $query->addExpression('COUNT(surRes.Id)', 'surveyCount');
      $query->condition('map.object_type', $objectType, '=');    
      $query->condition('sur.status', 'sry_det_sry_atv', '=');
      $query->condition('map.object_id', $classid, '=');
      $qryRes = $query->execute()->fetchAll();
      print_r($qryRes);*/
      $surveyStatus  = 'sry_det_sry_atv';
      $assessmentValue  = 'sry_det_typ_ass';
      if($surveytype == 'assessment'){
        $surveyTypeArr['survey'] = 'sry_det_typ_sry';
        $surveyTypeArr['assessment'] = 'sry_det_typ_ass';
        $typeCondition = $surveytype==''?'':" and sur.type = '$surveyTypeArr[$surveytype]' ";
        $typeCondition = " and sur.type = 'sry_det_typ_ass' ";
        /*-- 36248: Unable to take assessment during Recertification --*/
        if ($objectType == 'cre_sys_obt_cls' ) {
          $join_table = 'slt_enrollment';
          $field = 'comp_status';
        } else {
          $join_table = 'slt_master_enrollment';
          $field = 'overall_status';
        } 
        $get_ass_count='';
        if(!empty($content_player_call))
        	$get_ass_count='(select count(surRes.id) from slt_survey_results as surRes
                    inner join ' . $join_table . ' enroll on enroll.id = surRes.enrollment_id and enroll.id = :enrollId 
      				left join slt_survey_mapping as surMap on surMap.object_id=surRes.entity_id and surMap.object_type=surRes.entity_type
      		    where surRes.survey_id = sur.id AND surRes.entity_id=:objectId AND surRes.entity_type=:objectType AND
      		    surRes.user_id=:userId AND surRes.re_attend = 0 AND surRes.no_of_attempts >= 1 AND surMap.pre_status=:preStatus) as surveyattemptcount,';
        /*$query  = db_query('select IF(sum(grp_qus.score)=sur.max_mark, 1, 0) as show_survey, sur.id as surveyId, sur.title as surveyTitle, sur.code as suveyCode, map.view_option as viewOption, 
  						map.after_completion as afterCompletion, map.object_type as objectType, map.object_id as objectId, 
  						(select count(id) from slt_survey_results as surRes where surRes.survey_id = sur.id AND 
  						surRes.entity_id=:objectId AND surRes.entity_type=:objectType AND surRes.user_id=:userId) as surveyCount 
  						from slt_survey as sur 
  						left join slt_survey_mapping as map on sur.id=map.survey_id
  						left join slt_survey_groups_questions grp_qus on sur.id = grp_qus.survey_id 
                        left join slt_survey_groups grp on grp.id = grp_qus.survey_group_id and grp.status = :surveyGroupStatus                          
  						where map.object_type=:objectType and sur.status=:surveyStatus and map.object_id=:objectId '.$typeCondition.' group by sur.id having show_survey = 1', 
                        array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus,':surveyGroupStatus' => 'sry_det_grp_atv'));
                        */
      $query  = db_query('select IF(sum(grp_qus.score)=sur.max_mark, 1, 0) as show_survey, sur.id as surveyId, sur.title as surveyTitle, sur.code as suveyCode, sur.type as surtype, map.no_of_attempts,map.view_option as viewOption, map.pre_status as preStatus, map.preview_option as previewOption, sur.min_mark as minMark, sur.max_mark as maxMark,
  						map.after_completion as afterCompletion, map.object_type as objectType, map.object_id as objectId, 
  						(select count(surRes.id) from slt_survey_results as surRes
                    inner join ' . $join_table . ' enroll on enroll.id = surRes.enrollment_id and enroll.id = :enrollId 
      				left join slt_survey_mapping as surMap on surMap.object_id=surRes.entity_id and surMap.object_type=surRes.entity_type
      		    where surRes.survey_id = sur.id AND surRes.entity_id=:objectId AND surRes.entity_type=:objectType AND
      		    surRes.user_id=:userId AND surRes.re_attend = 0 AND surRes.no_of_attempts >= surMap.no_of_attempts AND surMap.pre_status=:preStatus) as surveyCount,'.$get_ass_count.'
      		    (select en.'.$field.' from '.$join_table.' en where id = :enrollId ) as compStatus,
      		 (select en.pre_status from '.$join_table.' en where id = :enrollId ) as pStatus,
      				(select max(res.no_of_attempts) from slt_survey_results as res inner join  ' . $join_table . '  enroll on enroll.id = res.enrollment_id and enroll.id = :enrollId where res.survey_id = sur.id AND 
  						res.entity_id=:objectId AND res.entity_type=:objectType AND res.user_id=:userId AND res.re_attend = 0 limit 1) as attempts, :asschk as asschk
  						from slt_survey as sur 
                          left join slt_survey_mapping map on map.object_id=:objectId and map.survey_id = sur.id
                          left join slt_survey_groups_questions grp_qus on grp_qus.survey_id = sur.id '.$module_table.' 		
                      where '.$module_condition.'  map.object_type=:objectType and map.pre_status=:preStatus and sur.status=:surveyStatus and map.object_id=:objectId '.$typeCondition.' group by sur.id having show_survey = 1',  // 
                        array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':preStatus' => $preStatus,':surveyStatus' => $surveyStatus,':surveyGroupStatus' => 'sry_det_grp_atv',':asschk'=>$asschk, ':enrollId' => $enrollId));

        expDebug::dPrintDBAPI('big query', $query,array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':preStatus' => $preStatus,':surveyStatus' => $surveyStatus,':surveyGroupStatus' => 'sry_det_grp_atv',':asschk'=>$asschk, ':enrollId' => $enrollId));
                       /* echo 'select IF(sum(grp_qus.score)=sur.max_mark, 1, 0) as show_survey, sur.id as surveyId, sur.title as surveyTitle, sur.code as suveyCode, map.view_option as viewOption, 
  						map.after_completion as afterCompletion, map.object_type as objectType, map.object_id as objectId, 
  						(select count(id) from slt_survey_results as surRes where surRes.survey_id = sur.id AND 
  						surRes.entity_id=:objectId AND surRes.entity_type=:objectType AND surRes.user_id=:userId) as surveyCount 
  						from slt_survey as sur 
                          left join slt_survey_mapping map on map.object_id=:objectId 
                          left join slt_survey_groups_questions grp_qus on grp_qus.survey_id = sur.id                          
  						where map.object_type=:objectType and sur.status=:surveyStatus and map.object_id=:objectId '.$typeCondition.' group by sur.id having show_survey = 1';
                        
                        print_r(array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus,':surveyGroupStatus' => 'sry_det_grp_atv'));
                        */
        $qryRes = $query->fetchAll();
        expDebug::dPrint('first query result ' . print_r( $qryRes, 1));
        if(count($qryRes) < 1) {
         $qryRes[0]->scoreConflict = true;
        }
        $assquery = db_query('select count(surMapping.id) as preassCount, count(surResult.id) as presurveyCount from slt_survey_mapping as surMapping
      			 left join slt_survey_results as surResult on surResult.entity_id = surMapping.object_id AND surResult.entity_type = surMapping.object_type
        		 AND surResult.user_id=:userId AND surResult.re_attend = 0  and surResult.no_of_attempts > 0
        			left join slt_survey as sur on surMapping.survey_id = sur.id 
      		    where surMapping.survey_id = sur.id AND surMapping.object_id=:objectId AND surMapping.object_type=:objectType
      		    AND surMapping.pre_status=1 AND sur.status=:surveyStatus',
        		array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus));
        expDebug::dPrintDBAPI('query result-->',$assquery,array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus));
        $assRes = $assquery->fetchAll();
        $qryRes[0]->presurveycount = $assRes[0]->presurveycount;
        $qryRes[0]->preassCount = $assRes[0]->preasscount;
        $statusquery = db_query('select count(surRes.id) as statusCount from slt_survey_results as surRes
      				left join slt_survey_mapping as surMap on surMap.object_id=surRes.entity_id and surMap.object_type=surRes.entity_type
      		    where surRes.entity_id=:objectId  AND surRes.entity_type=:objectType AND
      		    surRes.user_id=:userId AND surRes.pre_status=:preStatus',array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':preStatus' => $preStatus));
        expDebug::dPrintDBAPI('query result $statusquery-->',$statusquery,array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':preStatus' => $preStatus));
        $statusRes = $statusquery->fetchAll();
        expDebug::dPrint('$statusRes'.print_r($statusRes,true),4);
        $qryRes[0]->statusCount = $statusRes[0]->statuscount;
      } else {
             if($objectType=='cre_sys_obt_trn' or $objectType=='cre_sys_obt_trp' or $objectType=='cre_sys_obt_cur' or $objectType=='cre_sys_obt_crt'){
              $colName = 'overall_status';
              $tblName = 'slt_master_enrollment';
          }else{
              $colName = 'comp_status';
              $tblName = 'slt_enrollment';
          }
          $query  = db_query('select sur.id as surveyId, sur.title as surveyTitle, sur.code as suveyCode, map.view_option as viewOption, map.after_completion as afterCompletion, map.object_type as objectType, map.object_id as objectId, (select count(id) from slt_survey_results as surRes where surRes.survey_id = sur.id AND surRes.entity_id=:objectId AND surRes.entity_type=:objectType
  					AND surRes.user_id=:userId AND surRes.re_attend = 0 AND surRes.enrollment_id = :enrollId) as surveyCount ,enr.'.$colName.' As completionstatus from slt_survey as sur left join slt_survey_mapping as map on sur.id=map.survey_id  '.$module_table.' left join '.$tblName.' enr on enr.id = :enrollId where '.$module_condition.' sur.type<>:assessmentValue and map.object_type=:objectType and sur.status=:surveyStatus and map.object_id=:objectId order by map.id',array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus, 'assessmentValue' => $assessmentValue, ':enrollId' => $enrollId,));
          expDebug::dPrintDBAPI('big survey query', $query,array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus, 'assessmentValue' => $assessmentValue, ':enrollId' => $enrollId));
        expDebug::dPrint("12342412".print_r(array(':userId' => $userid,':objectId' => $objectId,':objectType' => $objectType,':surveyStatus' => $surveyStatus, 'assessmentValue' => $assessmentValue, ':enrollId' => $enrollId),true));
          $qryRes = $query->fetchAll();
      }
    }
  }
  if($_REQUEST['apiname'] == 'GetAssignedSurveysAssessmentsAPI' || $returnToCode == 'yes'){
    //expdebug::dPrint('getAssignedSurveysAPI 6');
    return $qryRes;
  }else{
    //expdebug::dPrint('getAssignedSurveysAPI 7');
    foreach ($qryRes as $result) {
    	$result->enrollId = $enrollId;
    }
    echo drupal_json_encode($qryRes);  
  }
  
  }catch (Exception $ex) {
    watchdog_exception('getAssignedSurveys', $ex);
    expertusErrorThrow($ex);
  } 
}//End of getAssignedSurveys()

function getEnrollmentStatus($userid,$classid){
  try{
  $resultStatus  = '';
  $regQry  = db_select('slt_enrollment','enr');
  $regQry->addField('enr','reg_status');
  $regQry->condition('enr.class_id',$classid,'=');
  $regQry->condition('enr.user_id',$userid,'=');
  $regQry->condition('enr.reg_status','lrn_crs_reg_cnf','=');
  //$regQry->condition('enr.master_enrollment_id');
  $regQryRes = $regQry->execute()->fetchAssoc();
  $resultStatus  = $regQryRes['reg_status'];
  return $resultStatus;
  }catch (Exception $ex) {
    watchdog_exception('getEnrollmentStatus', $ex);
    expertusErrorThrow($ex);
  } 		
}

/**
 * @Desc: To retrieve survey questions for the given survey
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getSurveyQuestions($enrollId = ''){
  try{
  global $showPreview;
  global $objectId;
  global $objectType;
  global $userId;
  global $surveyId;
  $inObj     = $_REQUEST;
  $surveyId  = ($inObj['SurveyID']!=null && $inObj['SurveyID'])!=''?stripQuotes($inObj['SurveyID']):'null';
  $objectId  = ($inObj['ObjectId']!=null && $inObj['ObjectId']!='')?stripQuotes($inObj['ObjectId']):'null';
  $objectType= ($inObj['ObjectType']!=null && $inObj['ObjectType']!='')?$inObj['ObjectType']:'null';
  $userId    = ($inObj['UserID']!=null && $inObj['UserID']!='')?stripQuotes($inObj['UserID']):'null';
  $showPreview    = ($inObj['ShowPreview']!=null && $inObj['ShowPreview']!='')?$inObj['ShowPreview']:'null';
  $surveyStatus  = 'sry_det_sry_atv';
  $groupStatus   = 'sry_det_grp_atv';
 
//code added by Ashutosh

 if(isset($_REQUEST["mobile_request"])){
  	
  	$userId = $_REQUEST['userid'];
  	$enrollId = $_REQUEST['enrollId'];
       $objectType = 'cre_sys_obt_cls'; 
 }

//End of code
 
  
  $surQry  = db_select('slt_survey_results','sur_res');
  $surQry->leftjoin('slt_survey_mapping','sur_map','sur_map.object_id=sur_res.entity_id and sur_map.object_type=sur_res.entity_type');
  $surQry->addExpression('count(*)', 'surveyCount');
  $surQry->condition('sur_res.survey_id',$surveyId,'=');
  $surQry->condition('sur_res.entity_id',$objectId,'=');
  $surQry->condition('sur_res.entity_type',$objectType,'=');
  $surQry->condition('sur_res.user_id',$userId,'=');
  $surQry->condition('sur_res.re_attend',0,'=');
  $surQry->where('sur_res.no_of_attempts > sur_map.no_of_attempts');
  expDebug::dPrintDBAPI('query for question survey count-->>',$surQry);
  $surQryRes = $surQry->execute()->fetchAssoc();
  $surveyCount  = $surQryRes['surveycount'];
  
  //echo "Survv : ".$surveyId." Objecttid :".$objectId;
  
  $query  = db_select('slt_survey_groups_questions','sur_groups_qst');
  $query->join('slt_survey_groups','sur_groups','sur_groups.id= sur_groups_qst.survey_group_id');  
  $query->join('slt_survey','sur','sur.id=sur_groups_qst.survey_id and sur.id='.$surveyId);
  $query->join('slt_survey_mapping','sur_map','sur_map.survey_id= sur_groups_qst.survey_id');

  /*$query  = db_select('slt_survey_groups','sur_groups');
  $query->join('slt_survey','sur','sur_groups.survey_id=sur.id');*/
  if ($enrollId > 0) {
  	if ($objectType != 'cre_sys_obt_cls') {
  		$programDetails 	= getProgramDetails($enrollId);
  		$enrollId 			= $programDetails[0]->id; 
  	}	
  	$select = db_select('slt_attendance_summary','attn');
  	$select->addField('attn', 'suspend_data','suspend_data');
  	$select->condition('attn.assessment_id', $surveyId);
  	$select->condition('attn.enrollment_id', $enrollId);
  	$select->condition('attn.user_id', $userId);
  	expdebug::dPrintDBAPI('suspend data List', $select);
  	$suspendData = $select->execute()->fetchField();
  }
  
  
  $query->addField('sur', 'id','surveyId');    
  $query->addField('sur', 'code','surveyCode');
  $query->addField('sur', 'title','surveyTitle');
  $query->addField('sur', 'question_per_page', 'questionPerPage');
  $query->addField('sur', 'type', 'surveytype');
  $query->addField('sur', 'random', 'random');
  $query->addField('sur_groups', 'id','groupId');
  $query->addField('sur_groups', 'title','groupTitle');
  $query->addField('sur_groups', 'code','groupCode');
  $query->addField('sur_groups', 'sequence','groupSequence');
  $query->addField('sur_map', 'pre_status','pre_status');
  $query->addField('sur_map', 'preview_option','preview_option');
  $query->condition('sur_groups.status',$groupStatus,'=');
  $query->condition('sur.status',$surveyStatus,'=');
  $query->condition('sur.id',$surveyId,'=');
  $query->condition('sur_groups_qst.survey_id',$surveyId,'=');
  $query->groupBy('sur_groups.id');
//sur_groups.id
  //$query->condition('survey_id',$surveyId,'=');  
  $query->orderBy('sur_groups.sequence', 'asc');
  
  
  expdebug::dPrintDBAPI('Question List', $query);
  
  $qryRes = $query->execute()->fetchAll();
  $qryRes[0]->suspend_data = $suspendData;
  if($_REQUEST['apiname'] == 'GetSurveyAssessmentQuestionsAPI'){
  	$numRows = count($qryRes);
  	$surveys['totalrow'] = $numRows;
  }
  $surveys[0]  = getLongSurvey($qryRes,$surveyCount);
  if($_REQUEST['apiname'] == 'GetSurveyAssessmentQuestionsAPI'){
    return $surveys;
  }
  else{
    echo drupal_json_encode($surveys);
  }
  }catch (Exception $ex) {
    watchdog_exception('getSurveyQuestions', $ex);
    expertusErrorThrow($ex);
  } 
}//End of getSurveyQuestions()

// isBusinessRuleAvailable() #0032105
function isBusinessRuleAvailable($classId){
	try{
		$select = db_select('slt_business_rule_mapping', 'rule');
		$select->leftJoin('slt_profile_list_items', 'prof','prof.code = rule.business_rule_code');
		$select->addExpression('COUNT(rule.id)', 'count');
		$select->condition('rule.entity_id', $classId);
		$select->condition('rule.business_rule_code', 'cre_sys_brl_rra','=');
		$select->condition('prof.is_active', 'Y','=');
		expDebug::dPrintDBAPI(' isBusinessRuleAvailable $select = ' , $select);
		$rowCount = $select->execute()->fetchField();
		return $rowCount;
	}catch (Exception $ex) {
		watchdog_exception('isBusinessRuleAvailable', $ex);
		expertusErrorThrow($ex);
	}
}// End isBusinessRuleAvailable(0

/**
 * @Desc: To resturcture the survey details and its template
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getLongSurvey($respData,$surveyCount){
  try{
  $survey          = new StdClass();
  $survey->ID      = $respData[0]->surveyid;
  $survey->Code    = $respData[0]->surveycode;
  $survey->Title   = $respData[0]->surveytitle;
  $survey->QuestionPerPage   = $respData[0]->questionperpage;
  $survey->PreStatus   = $respData[0]->pre_status;
  $survey->PreviewOptions   = $respData[0]->preview_option;
  $survey->SuspendData = json_decode(unserialize($respData[0]->suspend_data));
  $surveyAttempt   = ($surveyCount >0) ? 'Yes': 'No';
  $survey->Attempt = $surveyAttempt;
  $groups          = array();
  $flagShuffle = false;
 // expDebug::dPrint('suspendata value: ' .print_r($survey->SuspendData, 1));
  $tempAnswers = (array) $survey->SuspendData->answers;
 // expDebug::dPrint('temp answers group 1: ' . print_r($tempAnswers, 1));
  $ansSize = count($tempAnswers);
//   foreach($respData as $key=>$val) {
//   	$groups[]  	 	= getGroup($val);
//   }
  foreach($respData as $key=>$val) {			
   	$tempQuestGroup 		= getGroup($val);
	$tempQuestGroupSize 	= count($tempQuestGroup->Questions);
    if($respData[0]->random == 'sry_det_typ_ass_ran_bot' || $respData[0]->random == 'sry_det_typ_ass_ran_qus'){ //Added for #0082913
	   $flagShuffle = ($ansSize > $tempQuestGroupSize) ? false : true;
	}  
	//expDebug::dPrint('getLongSurvey $$ansSize:' . $ansSize);
	$ansSize    	= $ansSize - $tempQuestGroupSize;
	$tempAnswers 	= (array) array_slice($tempAnswers, 0, $tempGroupCount, true);
 	//expDebug::dPrint('getLongSurvey tempQuestGroupSize: ' . $tempQuestGroupSize);
 	//expDebug::dPrint('getLongSurvey flagShuffle: ' . var_export($flagShuffle, 1));
 	//expDebug::dPrint('temp question group: ' . print_r($tempQuestGroup->Questions, 1));
 	//expDebug::dPrint('temp orders group: ' . print_r($survey->SuspendData->orders[$key], 1));
 	//expDebug::dPrint('temp answers group 2: ' . print_r($tempAnswers, 1));
	$orderGroup  	= orderGroupQuestions($tempQuestGroup->Questions, $survey->SuspendData->orders[$key], $tempAnswers, $flagShuffle);		
	$tempQuestGroup->Questions = $orderGroup;
    $groups[]  	 	= $tempQuestGroup;
  }
  $survey->Groups  = $groups;	
  //expDebug::dPrint('getLongSurvey suspenddata: ' . print_r($survey->SuspendData->orders, 1));
  //expDebug::dPrint('getLongSurvey Group: ' . print_r($groups, 1));
  $param->surveyId   = $respData[0]->surveyId;
  $param->ObjectType = "Survey";
  $respData          = getThemeTemplate($param);	
  
  $survey->Template  = getTemplate($respData);
  return $survey;
  }catch (Exception $ex) {
    watchdog_exception('getLongSurvey', $ex);
    expertusErrorThrow($ex);
  } 
}//End of getLongSurvey()

function orderGroupQuestions($questionGroup = '', $ordersGroup = '', &$answersGroup = '',  $shuffle = false) {
	try{
		$tempArray = array();
		if ($shuffle) {
			if (count($answersGroup) > 0) {
				$questionGroup = UpdateQuestionIdAsKey($questionGroup);
				$answersGroupkeys = array_keys($answersGroup);
				foreach ($ordersGroup as $key => $val){
					if (in_array($val, $answersGroupkeys)) {
						//expDebug::dPrint('Answer found in the list' . $val);
						$tempArray[$val] = $questionGroup[$val];
						unset($answersGroup[$val]); // remove tat element
						unset($questionGroup[$val]); // remove tat element
					}
				}
				shuffle($questionGroup);
			} else {
				shuffle($questionGroup);
			}
			//expDebug::dPrint('shuffle true part tempArray' . print_r($tempArray, 1));
			//expDebug::dPrint('shuffle true part questionGroup' . print_r($questionGroup, 1));
			$tempArray = array_merge($tempArray, $questionGroup);
		} else {
			if (count($answersGroup) > 0) {
				$questionGroup = UpdateQuestionIdAsKey($questionGroup);
				$answersGroupkeys = array_keys($answersGroup);
				foreach ($ordersGroup as $key => $val){
					if (in_array($val, $answersGroupkeys)) {
				//		expDebug::dPrint('Answer found in the list' . $val);
						$tempArray[$val] = $questionGroup[$val];
						unset($answersGroup[$val]); // remove tat element
						unset($questionGroup[$val]); // remove tat element
					}
				}
				shuffle($questionGroup);
			}
				//expDebug::dPrint('shuffle else part tempArray' . print_r($tempArray, 1));
				//expDebug::dPrint('shuffle else part questionGroup' . print_r($questionGroup, 1));
			$tempArray = array_merge($tempArray, $questionGroup);
		}
		//expDebug::dPrint('Final temp array:' . print_r($tempArray, 1));
		return $tempArray;
	}catch (Exception $ex) {
		watchdog_exception('orderGroupQuestions', $ex);
		expertusErrorThrow($ex);
	}
}

function UpdateQuestionIdAsKey($questionGroup = '') {
	try {
		$updatedValue = array();
		foreach($questionGroup as $key => $val) {
			$updatedValue[$val->ID] = $val;
		}	
		return $updatedValue;
	}catch (Exception $ex) {
		watchdog_exception('UpdateQuestionId', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * @Desc: To get survey group details
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getGroup($data){
  try{
  global $showPreview;
  global $objectId;
  global $objectType;
  global $userId;
  global $surveyId;
  expDebug::dPrint('Group Data: '.$objectId.$objectType.$userId.' | survey : '.$surveyId , 4);
  $group=new StdClass();
  $group->ID=$data->groupid;
  $group->Code=$data->groupcode;
  $group->SeqNo=$data->groupsequence;
  $group->Title=$data->grouptitle;
  $group->surveytype=$data->surveytype;
  $group->random=$data->random;
  
  $query  = db_select('slt_survey_questions', 'sur_ques');
  $query->join('slt_survey_groups_questions', 'map','sur_ques.id=map.question_id');
  $query->join('slt_survey_groups', 'sur_groups','map.survey_group_id=sur_groups.id');
  if($_REQUEST['apiname'] == 'GetSurveyAssessmentQuestionsAPI'){
  $query->leftJoin('slt_tag_entity','te','(te.entity_id=sur_ques.id and te.entity_type IN(\'sry_det_typ_sry_qus\',\'sry_det_typ_ass_qus\'))');
  $query->leftJoin('slt_tagdefn','td','te.tagid= td.id');
  $query->addExpression("group_concat(distinct td.tagname)","tagname");
  }
  $query->leftjoin('slt_survey_results', 'sur_res','sur_res.question_id=map.question_id and sur_res.survey_id = map.survey_id');
  $query->addField('sur_ques','id','questionId');    
  $query->addField('sur_ques','code', 'questionCode');
  $query->addField('sur_ques','question_type', 'questionType');
  $query->addField('sur_ques','question_txt', 'questionTxt');
  $query->addField('sur_ques','answer_choice_txt', 'answerTxt');
  $query->addField('sur_ques','displayrating', 'displayRating');
  $query->addField('sur_ques','image_thumbnail', 'imageThumbnail');
  $query->addField('map','sequence', 'questionSequence');
  $query->addField('map','mandatoryOption', 'questionMandatory');
  $query->addField('sur_ques','right_answer', 'rightAnswer'); 
  $query->addField('sur_res','answer_txt', 'answergiven');
  $query->addField('sur_res','answer_status', 'answerstatus');
  $query->condition('sur_groups.id', $group->ID, '=');
  $query->condition('map.survey_id', $surveyId, '=');
  expDebug::dPrint('$showPreview : '.$showPreview, 4);
  if($showPreview==1){
  	$getmaxid=getNoOfAttempts($userId ,$surveyId,$objectId); // get the Max of Attempts 
  	if($objectType == 'cre_sys_obt_cls') {
  		$res = db_query("select id from slt_enrollment where user_id= '$userId' and class_id= '$objectId' order by id desc limit 1");
  	}
  	if($objectType == 'cre_sys_obt_trn' || $objectType == 'cre_sys_obt_crt' || $objectType == 'cre_sys_obt_cur') {
  		$res = db_query("select id from slt_master_enrollment where user_id= '$userId' and program_id= '$objectId' order by id desc limit 1");
  	}
  	$enrDetails = $res->fetchAll();
  	$enrid = $enrDetails[0]->id; 
  	if($objectType == 'cre_sys_obt_cls') {
  		$res = db_query("select id from slt_enrollment where user_id= '$userId' and class_id= '$objectId' order by id desc limit 1");
  	}
  	if($objectType == 'cre_sys_obt_trn' || $objectType == 'cre_sys_obt_crt' || $objectType == 'cre_sys_obt_cur') {
  		$res = db_query("select id from slt_master_enrollment where user_id= '$userId' and program_id= '$objectId' order by id desc limit 1");
  	}
  	$enrDetails = $res->fetchAll();
  	$enrid = $enrDetails[0]->id;  	
    $query->condition('sur_res.entity_id', $objectId, '=');
    $query->condition('sur_res.entity_type', $objectType, '=');
    $query->condition('sur_res.user_id', $userId, '=');
    $query->condition('sur_res.survey_id', $surveyId, '=');
    $query->condition('sur_res.no_of_attempts', $getmaxid, '='); // get the Preview of the  present assesment Details 
    $query->condition('sur_res.enrollment_id', $enrid, '=');
  }
  $query->groupBy('sur_ques.id');
  
  if($group->surveytype == 'sry_det_typ_ass' && ($group->random == "sry_det_typ_ass_ran_qus" || $group->random == "sry_det_typ_ass_ran_bot")) {
     $query->orderBy('rand()');    
  	//$query->orderBy('map.sequence', 'asc');
  }else{
    $query->orderBy('map.sequence', 'asc');
  }
  
   expdebug::dPrintDBAPI('Question List', $query);
   
  $respData = $query->execute()->fetchAll();
  $questions=array();		
  $questions= getQuestion($respData);		
  $group->Questions=$questions;
  return $group;
  }catch (Exception $ex) {
    watchdog_exception('getGroup', $ex);
    expertusErrorThrow($ex);
  } 
}//End of getGroup()

/**
 * @Desc: To get survey questions
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getQuestion($respData){
  try{
  global $showPreview;
  $opDtArr = Array();
  for($i=0;$i<count($respData);$i++) {			
    $question=new StdClass();
    $question->ID=$respData[$i]->questionid;
    $question->Code=$respData[$i]->questioncode;
    $question->QuestionType=$respData[$i]->questiontype;
    $question->QuestionTxt=$respData[$i]->questiontxt;
    $question->AnswerTxt=$respData[$i]->answertxt;			
    $question->QuestionSeqNo=$respData[$i]->questionsequence;
    $question->IsMandatory=$respData[$i]->questionmandatory;
    $question->DisplayRating=$respData[$i]->displayrating;
    $question->ImageThumbnail=$respData[$i]->imagethumbnail;
    if($showPreview == 1){ 
      $question->RightAnswer=$respData[$i]->rightanswer; 
    } else {
      $question->RightAnswer=''; 
    }
    $question->AnswerGiven=$respData[$i]->answergiven;
    $question->AnswerStatus=$respData[$i]->answerstatus;
    if($_REQUEST['apiname'] == 'GetSurveyAssessmentQuestionsAPI')    
        $question->TagName=$respData[$i]->tagname;
    $opDtArr[] = $question;
  }
  return $opDtArr;
  }catch (Exception $ex) {
    watchdog_exception('getQuestion', $ex);
    expertusErrorThrow($ex);
  } 
}//End of getQuestion()

/**
 * @Desc: To get survey theme
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getThemeTemplate($inObj){
  try{
    $surveyId    = ($inObj->surveyId!=null && $inObj->surveyId!='')?$inObj->surveyId:'null';
    $objectType  = ($inObj->ObjectType!=null && $inObj->ObjectType!='')?$inObj->ObjectType:'null';
    $qry         = db_select('slt_themeassociate','thm');
    $qry->addField('thm','id');
    $qry->condition('thm.objectid',$surveyId,'=');
    $qry->condition('thm.objecttype',$objectType,'=');
	$qryRes = $qry->execute()->fetchAssoc();
    expDebug::dPrint("getThemeTemplate query res:".$qryRes , 3);
    return $qryRes;
  }catch (Exception $ex) {
    watchdog_exception('getThemeTemplate', $ex);
    expertusErrorThrow($ex);
  }     
}//End of  getThemeTemplate()

/**
 * @Desc: To get survey template for the selected theme
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function getTemplate($respData){	
  try{	
	$template=new StdClass();
	$template->ID         = $respData[0]->themeId;
	$template->ThemeName  = '';
	return $template;	
  }catch (Exception $ex) {
    watchdog_exception('getTemplate', $ex);
    expertusErrorThrow($ex);
  }  
}//End of getTemplate()

/**
 * @Desc: To get Class Or Tp Title For the Assesment
 */
function getClassOrTpDetails($id,$type){
	try{
			if($type == 'cre_sys_obt_cls'){
				$select = db_select('slt_course_class', 'cls');
				$select->addField('cls', 'title');
				$select->condition('cls.id', $id, '=');
			}else{
				$select = db_select('slt_program', 'prg');
				$select->addField('prg', 'title');
				$select->condition('prg.id', $id);
			}
			expDebug::dPrintDBAPI('select query please', $select);
			$result = $select->execute()->fetchField();
			return $result;
	}catch (Exception $ex) {
		watchdog_exception('getClassDetails', $ex);
		expertusErrorThrow($ex);
	}
}//End of getClassTpDetails()

/**
 * @Desc: Receive suvery questions and prepare it for insert
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function submitSurvey($type,$survey_arr = ''){
  try{
  $params = new stdClass();
  $requestData  = "";
  if($_REQUEST['apiname'] == 'UpdateSurveyAssessmentResultsAPI'){
    $requestData  = $survey_arr;
  }
  else{
    $requestData  = $_POST;
    //0053584: Unable to submit survey
    //For security reason the add the __csrf_magic variable in post array so it will increase the count.
    unset($requestData['__csrf_magic']);
    unset($requestData['totalTime']);
  } 
  
   //Added by ganeshbabuv, july 18 2016 12:55 PM If cookie set at salesforce, unset the exp_sess_id variable to avoid the assessment submit issue #0066380: Assessment issue in Salesforce site
   if(module_exists(salesforce_integration)){      
        expDebug::dPrint('$requestData  : '.print_r($requestData,true), 5);
      
        if(isset($_COOKIE) && count($_COOKIE)>0 && trim($_COOKIE['SPLearnerInfo'])!="" && trim($_COOKIE['SPCertificate'])!=""){
             
            unset($requestData['exp_sess_id']);
        } 
   }      
  
  if(sizeof($requestData)>0){
  	$params->EnrollId=$requestData[0]['enrollId'];
    $params->ObjectId=$requestData[0]['ObjectId'];
    $params->ObjectType=$requestData[0]['ObjectType'];
    $params->LearerId=$requestData[0]['LearerId'];
    $params->SurveyId=$requestData[0]['SurveyId'];
    $params->PreStatus = $requestData[0]['PreStatus'];
 
 
    $select_result = db_select('slt_survey_results', 'sur_result');
    $select_result->addExpression('count(*)', 'surveyResultCount');
    $select_result->addExpression('MAX(id)', 'id');
    $select_result->addExpression('MAX(no_of_attempts)', 'attempts');    
    $select_result->condition('sur_result.survey_id',  $params->SurveyId,'=');
    $select_result->condition('sur_result.enrollment_id', $params->EnrollId,'=');
    $select_result->condition('sur_result.entity_id', $params->ObjectId,'=');
    $select_result->condition('sur_result.entity_type', $params->ObjectType,'=');
    $resultSurvey = $select_result->execute()->fetchAssoc();
    $surveyresultCount  = $resultSurvey['surveyresultcount'];    
    $attempts = $resultSurvey['attempts']+1;
    if($surveyresultCount <= 0){
    	$params->attempts=1;
    }else{
    	$params->attempts = $attempts;
    }
   
    for($c=0;$c<sizeof($requestData);$c++){
      $params->QuestionId=$requestData[$c]['QuestionId'];
      $params->QuestionType=$requestData[$c]['QuestionType'];
      $params->Question=$requestData[$c]['Question'];
      $params->QuestionAns=$requestData[$c]['QuestionAns'];

      expDebug::dPrint('Survey ID ='.$params->SurveyId);
      
      expDebug::dPrint('Assessment Result count: surveyid '.$params->SurveyId.' objid '.$params->ObjectId.' objecttype '.$params->ObjectType.' QuestionId'.$params->QuestionId." QuestionType ".$params->QuestionType."Question ".$params->Question." QuestionAns ".$params->QuestionAns."enrollId".$params->EnrollId , 4);
      
      $result = insertLnrsurvey($params);
    }//end for
    $score = "";
    $assCompStatus = "";
    $percentCompletion = "";
    $minMark = "";
    $maxMark = "";
    if($type != 'survey'){
    	$surveyResultArr = calculateScore($params->EnrollId,$params->ObjectId, $params->ObjectType, $params->SurveyId, $params->LearerId, $params->attempts,$params->PreStatus);
    	// To Get the No of attempts Taken and Total No of attempts.
    	$output = getAssignedSurveys($params->LearerId,$params->ObjectId,$params->ObjectType,'assessment',$params->PreStatus,$params->EnrollId,'yes');
    	foreach ($output as $key => $val) {
    		$output[$key] = (array) $val;
    	}
    	expDebug::dPrint('$output : '.print_r($output,true), 4);
    	expDebug::dPrint('attempts : '.print_r($output[0]['attempts'],true), 4);
    	expDebug::dPrint('no_of_attempts  : '.print_r($output[0]['no_of_attempts'],true), 4);
    	// preview Enable for Last attempt Only #0038540
    	//32099: Feature request - ability to disable assessment preview option
    	expDebug::dPrint('previewoption  : '.print_r($output[0]['previewoption'],true), 4);
    	if($output[0]['previewoption']==0)
    		$surveyResultArr[] = 'no';
    	elseif ($output[0]['previewoption']==1)
    		$surveyResultArr[] =($output[0]['no_of_attempts'] == $output[0]['attempts']) ? 'yes' :'no';

    	$surveyResult = implode('###', $surveyResultArr);
    	$score = floatval($surveyResultArr[2]);
    	$passScore = floatval($surveyResultArr[3]);
    	$percentCompletion = floatval($surveyResultArr[4]);
    	$minMark = floatval($surveyResultArr[0]);
    	$maxMark = floatval($surveyResultArr[1]);
    	if($percentCompletion >= $passScore)
    		$assCompStatus = "PASSED";
    	else
    		$assCompStatus = "FAILED";
    	expDebug::dPrint('Assessment Result: '.$surveyResult , 4);
    }
    expDebug::dPrint('TYPE---> '.$type , 4);
    if(!contentPlayerIsActive() && $type == 'survey'){
	    if($params->ObjectType == 'cre_sys_obt_cls'){
	   	 	$course_id = getCourseIdFromClassId($params->ObjectId);
	    	$class_id = $params->ObjectId;
	    }else{
	    	$resultenr = getEnrollmentDetails($params->EnrollId,array('class_id','course_id','master_enrollment_id'));
	    	$course_id = $resultenr->course_id;
	    	$class_id = $resultenr->class_id;
	    	$lesson_id= $master_enrollment_id = $resultenr->master_enrollment_id;
	    }
	    //Insert slt_attendance_summary
	    $insertStmt = db_insert('slt_attendance_summary');
	    $fields = array(
	    		'enrollment_id'    => $params->EnrollId,
	    		'user_id'  => $params->LearerId,
	    		'course_id'  => $course_id,
	    		'class_id'  => $class_id,
	    		'content_version_id'  => 0,
	    		'lesson_id'  => $lesson_id,
	    		'assessment_id'  => $params->SurveyId,
	    		'content_type' => 'sry_det_typ_sry',
	    		'progress' => 100,
	    		'status' => 'lrn_crs_cmp_cmp',
	    		'total_attempts' => 1,
	    		'last_attempt_date' => now(),
	    		'total_time_spend' => $_REQUEST['totalTime'],
	    		'score' => $score,
	    		'suspend_data' => $surveyResultArr,
	    		'content_status' => $assCompStatus,
	    		'completion_status' => 'lrn_crs_cmp_cmp',
	    		'created_by' => $params->LearerId,
	    		'created_on' => now(),
	    		'updated_by' => $params->LearerId,
	    		'updated_on' => now()
	    );
	    $insertStmt->fields($fields);
	    expDebug::dPrintDBAPI("Insert slt_attendance_summary ", $insertStmt);
	    $attend_Id = $insertStmt->execute();
    }
    if($_REQUEST['apiname'] == 'UpdateSurveyAssessmentResultsAPI'){
      $tmp = new stdClass();
      $tmp->status = "success";
	  	$tmp->completionstatus = $assCompStatus;
      $tmp->score = $score;
      $tmp->completion_percentage = $percentCompletion;
      $tmp->minmark = $minMark;
      $tmp->maxmark = $maxMark;
      $tmparr=array();
	  $tmparr[0]=$tmp;
	  return $tmparr;
    }else{
      echo drupal_json_encode("success###".$surveyResult);
    }
  }//end if
  syncSolrData('Enrollment');
  }catch (Exception $ex) {
    watchdog_exception('submitSurvey', $ex);
    expertusErrorThrow($ex);
  } 
}//End of submitSurvey()

/**
 * @Desc: Receives suvery/assessment id and userid and returns the score
 */
function calculateScore($enrollId = '',$entityId, $entityType, $surveyId, $learnerId, $attempts,$preStatus){
  try{
  // Get the result
  $surSelQry  = db_select('slt_survey','sur');
  $surSelQry->addField('sur','min_mark');
  $surSelQry->addField('sur','max_mark');
  $surSelQry->addField('sur_grp_ques','score');
  $surSelQry->addField('sur_ques','right_answer');
  $surSelQry->addField('sur_ques','question_type ');
  $surSelQry->addField('sur_res','answer_txt');
  $surSelQry->addField('sur_res','id','result_id');
  $surSelQry->addField('sur_res','no_of_attempts','attempts');
  $surSelQry->addField('sur_res','pre_status','pre_status');
  $surSelQry->addField('sur_ques','answer_type','answer_type');
  $surSelQry->addField('sur_ques','answer_min_criteria','answer_min_criteria');
  //$surSelQry->join('slt_survey_groups','sur_grp', 'sur.id = sur_grp.survey_id');  
  $surSelQry->join('slt_survey_groups_questions','sur_grp_ques', 'sur.id = sur_grp_ques.survey_id');
  $surSelQry->join('slt_survey_groups','sur_grp', 'sur_grp.id = sur_grp_ques.survey_group_id');  
  $surSelQry->join('slt_survey_questions','sur_ques', 'sur_ques.id = sur_grp_ques.question_id ');
  $surSelQry->join('slt_survey_results','sur_res', 'sur.id = sur_res.survey_id and sur_res.question_id = sur_grp_ques.question_id and sur_res.question_id = sur_ques.id ');
  $surSelQry->condition('sur_grp.status','sry_det_grp_atv','=');
  $surSelQry->condition('sur_res.survey_id',$surveyId,'=');
  $surSelQry->condition('sur_res.user_id',$learnerId,'=');
  $surSelQry->condition('sur_res.entity_id',$entityId,'=');
  $surSelQry->condition('sur_res.entity_type',$entityType,'=');
  $surSelQry->condition('sur_res.no_of_attempts',$attempts,'=');
  if(!empty($enrollId))
  $surSelQry->condition('sur_res.enrollment_id',$enrollId,'=');
  
  $surSelQry->groupBy('sur_res.id');
  $surSelQryRes = $surSelQry->execute()->fetchAll();
  expDebug::dPrintDBAPI('calculate score ', $surSelQry);
  
  $totalScore = 0;
  $minMark = 0;
  $maxMark = 0;
  $totalScorePer = 0;
  $search_string 	= array("'");
  $replace_string   = array("&#39;");
  // sry_qtn_typ_cmt, sry_qtn_typ_dpn, sry_qtn_typ_mch, sry_qtn_typ_rtg, sry_qtn_typ_trf, sry_qtn_typ_yno
  foreach ($surSelQryRes as $record) {
    $resultId      = $record->result_id;
    $minMark      = $record->min_mark;
    $maxMark      = $record->max_mark;
    $score         = $record->score;
    $right_answer  = str_replace($search_string, $replace_string, $record->right_answer);
    $answer_txt    = $record->answer_txt;
    $question_type    = $record->question_type;
    $pre_status    = $record->pre_status;
    switch($question_type){
      case 'sry_qtn_typ_trf':
      case 'sry_qtn_typ_yno':
      case 'sry_qtn_typ_dpn':  
      	$right_answer = changeStringQuotes($right_answer);
      	if(strtolower($answer_txt) == strtolower($right_answer)){
          // Update the score in result table
          updateSurveyResult($resultId, $score, 'CORRECT');
            $totalScore += $score; 
        } else {
          // Update the score in result table
          updateSurveyResult($resultId, 0, 'WRONG');
        }
        break;
      case 'sry_qtn_typ_cmt':
      case 'sry_qtn_typ_rtg':
        if(strlen($answer_txt)>0){
          updateSurveyResult($resultId, $score, 'CORRECT');
            $totalScore += $score;
        } else {
          // Update the score in result table
          updateSurveyResult($resultId, 0, 'WRONG');
        }
        break;
      case 'sry_qtn_typ_mch':
      	$right_answer = changeStringQuotes($right_answer);
      	$right_answer_arr = explode("##", $right_answer);
        $answer_txt_arr = explode(EXP_AC_SEPARATOR, $answer_txt);
        $matchedAnswers = array_intersect($right_answer_arr, $answer_txt_arr);
        $unMatchedAnswers = array_diff($answer_txt_arr, $right_answer_arr);
        $unMatchedAnswersCount = count($unMatchedAnswers);
        $correct = 0;
        if($unMatchedAnswersCount == 0) {
	        if($record->answer_type == 'sry_ans_typ_all') {
	        	if(count($matchedAnswers) == count($right_answer_arr)) {
	        		$correct = 1;
	        	}
	        }else if($record->answer_type == 'sry_ans_typ_atl') {
	        	if(count($matchedAnswers) >= $record->answer_min_criteria) {
	        		$correct = 1;
	        	}
	        }
	        /*else if($record->answer_type == 'sry_ans_typ_ran') {
		        if(count($matchedAnswers) >= $record->answer_min_criteria && (count($matchedAnswers) <= $record->answer_max_criteria)) {
					$correct = 1;
				} 
	        }else if($record->answer_type == 'sry_ans_typ_xct') {
	        	if(count($matchedAnswers) == $record->answer_min_criteria) {
        $correct = 1;
          }
        }*/
        }
        if($correct==1){
          updateSurveyResult($resultId, $score, 'CORRECT');
            $totalScore += $score;
        } else {
          updateSurveyResult($resultId, 0, 'WRONG');
        }
        break;
    }
  } 
  $minMarkPer = ( $minMark / $maxMark ) * 100;
  $totalScorePer = ( $totalScore / $maxMark ) * 100;
  $totalScorePer = round($totalScorePer, 0);
  // This Issue is Fixed for this Ticket #0036526 To Get the Course Or TP name To the Assement
  $title = getClassOrTpDetails($entityId,$entityType);
  return array($minMark, $maxMark, $totalScore, $minMarkPer, $totalScorePer,$pre_status,$surveyId,$title);
  }catch (Exception $ex) {
    watchdog_exception('calculateScore', $ex);
    expertusErrorThrow($ex);
  }   
}


/**
 * @Desc: Updates individual score and answer correct/wrong
 */
function updateSurveyResult($resultId, $score, $resultstatus){
  $txn = db_transaction();
  try{
    db_update('slt_survey_results')
      ->fields(array('answer_status' => $resultstatus,'score' => $score))
      ->condition('id',$resultId, '=')
      ->execute();
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateSurveyResult', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);      
}
/**
 * @Desc: To add new survey
 * @author: Ilayaraja Esackimuthu
 * @since: 01-Aug-2011
 */
function insertLnrsurvey($inObj){
  $txn = db_transaction();
  try{
  $search_string 	= array("'");
  $replace_string   = array("&#39;");
  $QuestionAns  	= str_replace($search_string, $replace_string, $inObj->QuestionAns);
  $Question     	= str_replace($search_string, $replace_string ,$inObj->Question);
  $enrollId    = ($inObj->EnrollId) ? $inObj->EnrollId : NULL;
  $entityId    = $inObj->ObjectId;
  $entityType  = $inObj->ObjectType;
  $userId      = $inObj->LearerId;
  $surveyId    = $inObj->SurveyId;
  $questionId  = $inObj->QuestionId;
  $questionType= $inObj->QuestionType;
  $preStatus = $inObj->PreStatus;
  $question    = $Question;
  $questionAns = $QuestionAns;
  $createdOn    = now();
  
  $surQry  = db_select('slt_survey','sur');
  $surQry -> addField('sur', 'type', 'surveyType');
  $surQry -> condition('sur.id', $surveyId, '=');
  $surveyType = $surQry->execute()->fetchField();
  $attempts = null;
  if($surveyType == 'sry_det_typ_ass'){
  	$attempts    = $inObj->attempts;  	
  }
    
  $fields = array('enrollment_id' => $enrollId,
  'entity_id' => $entityId,
  'entity_type' => $entityType, 
  'user_id' => $userId,
  'survey_id' => $surveyId,
  'question_id' => $questionId,
  'question_type' => $questionType,
  'question_txt' => $question,
  'answer_txt' => $questionAns,
  'created_by' => $userId,
  'no_of_attempts' => $attempts,
  'created_on' => $createdOn,
  'updated_by' => $userId,
  'updated_on' => $createdOn,
  'isanonymous' => 'No',
  'pre_status' => $preStatus == ''? null : $preStatus	//for 41127: For VOD and WBT class when keeping content attempts in my learning class not moved to completed tab
  );
  db_insert('slt_survey_results')
  ->fields($fields)
  ->execute();
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('insertLnrsurvey', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}//End of insertLnrsurvey()

/**
 * @Desc: To update survey
 * @author: jayasri
 * @since: 05-May-2014
 */
/*
function updateLnrsurvey($inObj){
	$txn = db_transaction();
	try{
		$QuestionAns  = str_replace("'", "&quote", $inObj->QuestionAns);
		$Question     = str_replace("'", "&quote",$inObj->Question);
		$enrollId    = ($inObj->EnrollId) ? $inObj->EnrollId : NULL;
		$entityId    = $inObj->ObjectId;
		$entityType  = $inObj->ObjectType;
		$userId      = $inObj->LearerId;
		$surveyId    = $inObj->SurveyId;
		$questionId  = $inObj->QuestionId;
		$questionType= $inObj->QuestionType;
		$attempts    = $inObj->attempts;
		$question    = $Question;
		$questionAns = $QuestionAns;
		$createdOn    = now();
		expDebug::dPrint('getMaxAttempts $$inObj: '.print_r($inObj, true) , 3);
		$surQry  = db_select('slt_survey','sur');
		$surQry -> addField('sur', 'type', 'surveyType');
		$surQry -> condition('sur.id', $surveyId, '=');
		$surveyType = $surQry->execute()->fetchField();
		$surveynoOfAttempts = null;
		if($surveyType == 'sry_det_typ_ass'){	
			$updateQuery = db_update('slt_survey_results');
			$updateQuery->fields(array(
					'no_of_attempts' => $attempts+1,		
			));
			$updateQuery->condition('enrollment_id', $enrollId, '=');
			//$updateQuery->ccondition('survey_id', $surveyId, '=');
			//$updateQuery->condition('entity_id', $entityId, '=');
			//$updateQuery->condition('entity_type', $entityType, '=');
			$updateQuery->execute();	
			expDebug::dPrintDBAPI('update query', $updateQuery);
		}
	}catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('updateLnrsurvey', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}//End of updateLnrsurvey()
*/


function getAssessmentDetailsAPI(){
	try {
		$userId = $_REQUEST['userid'];
		$objectId = $_REQUEST['classid'];
		$objectType = 'cre_sys_obt_cls';//$_REQUEST['objecttype'];
		$surveyType = $_REQUEST['surveytype'];
		
		$select = db_select('slt_survey_mapping','sc');
		$select->join('slt_survey', 'ss', 'ss.id = sc.survey_id');
		$select->addField('sc','pre_status');
		$select->condition('sc.object_id',$objectId);
		$select->condition('sc.object_type',$objectType);
		$select->condition('ss.type', 'sry_det_typ_ass');
		expDebug::dPrintDBAPI('query for nodeid',$select);
		$result = $select->execute()->fetchAll();
		
		return  $result;
	}catch (Exception $ex) {
		watchdog_exception('getAssessmentDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}
}

function getAssignedSurveysAssessmentsAPI(){
 try {	
	  $userId = $_REQUEST['userid'];
	  $objectId = $_REQUEST['classid'];
	  $objectType = 'cre_sys_obt_cls';//$_REQUEST['objecttype'];
	  $surveyType = $_REQUEST['surveytype'];
	  $enrollId = $_REQUEST['enrollId'];
	    if($surveyType == 'assessment')
	    {
	      $preStatus = explode("-~",$_REQUEST['preStatus']);	

	      expDebug::dPrint('rfrfrgttg5656'.print_r(count($preStatus),true),5);
	      
	      for($i=0; $i < count($preStatus); $i++ )
	 	  {
	 		 $output = getAssignedSurveys($userId,$objectId,$objectType,$surveyType,$preStatus[$i],$enrollId);
			  $result[$i] = $output[0];

                              //Code added by Ashutosh
                                if(isset($_REQUEST["mobile_request"])){
			  	//$query = 'select * from slt_attendance_summary where enrollment_id=725';
			  	expDebug::dPrint('surveyyyyyyyyyyyyIdddd'.print_r($result[$i]->surveyid,true),5);
			  	if($result[$i]->surveyid != ""){
			  		expDebug::dPrint('frfrfrfrfrfgtgtgt'.print_r($result[$i]->surveyid,true),5);
			  	$user_detail=db_query("select * from slt_attendance_summary where enrollment_id=".$enrollId." and assessment_id =" .$result[$i]->surveyid );
			  	expDebug::dPrintDBAPI("query12345rchangese3edede ", $user_detail);
			  	$userDetail = $user_detail->fetchAll();
			  	expDebug::dPrint('calling after 1125433'.print_r($enrollId,true),5);
			  	expDebug::dPrint('calling after 1125433'.print_r($userDetail[0]->progress,true),5);
			  	$result[$i]->progress = $userDetail[0]->progress;
			  	$result[$i]->attempts_taken = $userDetail[0]->total_attempts;
			  	}
	 	               }


                             //End of code

	 	  }
	    }
	    else if($surveyType == 'survey')
	    {
	       $select = db_select('slt_survey_mapping','sm');
	       
	       $select->addExpression('COUNT(1)','cnt');
	       $select->leftJoin('slt_survey', 'surv', 'surv.id = sm.survey_id');
	       $select->condition('sm.object_id',$objectId);
	       $select->condition('surv.type','sry_det_typ_sry');
	       
           expDebug::dPrintDBAPI('query for survey count',$select);
           $Count_val = $select->execute()->fetchColumn();
           $result['totalrow'] = $Count_val;	//count needs to be calculated irrespective of limit value. 
			//	So we have to write separate query to get the total records when limit is used 
          for($i=0; $i < $Count_val; $i++ )
		  {
			  $output = getAssignedSurveys($userId,$objectId,$objectType,$surveyType,$preStatus[$i],$enrollId);
			  $result[$i] = $output[$i];

                          //Code added by Ashutosh
                          if(isset($_REQUEST["mobile_request"])){
	               	//$query = 'select * from slt_attendance_summary where enrollment_id=725';
                          	expDebug::dPrint('ashutoshsurveyiddddd'.print_r($output[$i]->surveyid,true),5);
                          	
                          	if($output[$i]->surveyid != ""){
                          	expDebug::dPrintDBAPI("query12345rchanges ", $user_detail);
                          	
			  	        	$user_detail=db_query("select * from slt_attendance_summary where enrollment_id=".$enrollId." and assessment_id = ".$output[$i]->surveyid);
			  	        	expDebug::dPrintDBAPI("query12345rchanges ", $user_detail);
			  	        	$userDetail = $user_detail->fetchAll();
			  
			  	        	//$progress = $this->query($query)->fetchAll();
			  	        	expDebug::dPrint('calling after 1125433'.print_r($enrollId,true),5);
			  	        	expDebug::dPrint('calling after 1125433'.print_r($userDetail[0]->progress,true),5);
			  	        	 $result[$i]->progress = $userDetail[0]->progress;
			  	        	 $qryRes[$i]->attempts_taken = $userDetail[0]->total_attempts;
		  }
                          }

//End of the code

		  }
	    }
	  expdebug::dPrint('getAssignedSurveysAPI 1');
	  expDebug::dPrint('calling after 1'.print_r($result,true),5);
  	return  $result;
  }catch (Exception $ex) {
  	watchdog_exception('getAssignedSurveysAssessmentsAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSurveyAssessmentQuestionsAPI(){
	try {
  $res = getSurveyQuestions();
  expdebug::dPrint('getSurveyQuestionsAPI 1');
  expdebug::dPrint($res);
  return $res;
  }catch (Exception $ex) {
  	watchdog_exception('getSurveyAssessmentQuestionsAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

function UpdateSurveyAssessmentResultsAPI(){
	try {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);  
  $answers = $_REQUEST["answers"];
  $_REQUEST["LearerId"] = $_REQUEST["userid"];
  $tmp_arr = explode(",~^",$answers);
  $survey_arr = array();
  $survey_arr[0]['enrollId'] = $_REQUEST["enrollId"];
  $survey_arr[0]['ObjectId'] = $_REQUEST["ObjectId"];
  $survey_arr[0]['ObjectType'] = $_REQUEST["ObjectType"];
  $survey_arr[0]['LearerId'] = $_REQUEST["LearerId"];
  $survey_arr[0]['PreStatus'] = $_REQUEST["prestatus"];     
  for($i=0; $i < count($tmp_arr);$i++)
  {
     $row = $tmp_arr[$i];
     $tokens = explode("~^",$row);
     $surveyId = $tokens[0];
     $questionId = $tokens[1];
     $questionType = $tokens[2];
     $question = $tokens[3];
     $questionAns = $tokens[4];
     //expdebug::dPrint("UpdateSurveyResultsAPI 2.".$surveyId." - ".$questionId."-".$questionType."-".$question."-".$questionAns);
     expdebug::dPrint("Surevyeeeyyeyy ID =".$surveyId);
     
     $survey_arr[0]['SurveyId'] = $surveyId;
     $survey_arr[$i]['QuestionId'] = $questionId;
     $survey_arr[$i]['QuestionType'] = $questionType;
     $survey_arr[$i]['Question'] = $question;
     $survey_arr[$i]['QuestionAns'] = $questionAns;
  }

  
  if(($_REQUEST['apiname'] == 'UpdateSurveyAssessmentResultsAPI') && $_REQUEST['type'] != 'survey'){
  
  $select = db_select('slt_survey_mapping','sc');
  
  $select->addField('sc','no_of_attempts');
  $select->condition('sc.object_id',$_REQUEST["ObjectId"]);
  $select->condition('sc.object_type',$_REQUEST["ObjectType"]);
  $select->condition('sc.survey_id', $surveyId);
  expDebug::dPrintDBAPI('query for no of atempts',$select);
  $attempts_set = $select->execute()->fetchField();
  expDebug::dPrint('calling after attempts'.print_r($attempts_set,true),5);
  $max_attempts = getTotalAttemptCountSurAss($_REQUEST["enrollId"],$surveyId,$_REQUEST["ObjectId"],$_REQUEST["ObjectType"],$_REQUEST["LearerId"]);
  
  if($max_attempts>= $attempts_set){
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "There are no attempts left";
  	return $errobj;
  }
  }
  return submitSurvey($_REQUEST["type"], $survey_arr);
  }catch (Exception $ex) {
  	watchdog_exception('UpdateSurveyAssessmentResultsAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 *  Get No of Max Attempts of the User In Present Assesment
 */
function getNoOfAttempts($userId,$surveyId,$objectId){
	try {
		expDebug::dPrint('$userId : '.$userId.' $surveyId : '.$surveyId .' $objectId : '.$objectId , 4);
			
		$select = db_select('slt_survey_results', 'sur_result');
		$select->addExpression('MAX(no_of_attempts)', 'attempts');
		$select->condition('sur_result.user_id',  $userId,'=');
		$select->condition('sur_result.survey_id', $surveyId,'=');
		$select->condition('sur_result.entity_id', $objectId, '=');
		expDebug::dPrintDBAPI('$select', $select);
		$result = $select->execute()->fetchField();
		
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getNoOfAttempts', $ex);
		expertusErrorThrow($ex);
	}
}

function changeStringQuotes($inputStr = '') {
	$search_array 	= array("'");
	$replace_array   = array("&#39;");
	return str_replace($search_array, $replace_array, $inputStr);
}
?>