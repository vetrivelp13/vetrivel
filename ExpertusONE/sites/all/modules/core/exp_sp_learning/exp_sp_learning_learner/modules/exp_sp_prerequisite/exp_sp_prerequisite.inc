<?php
function getCatalogPrerequisite($courseId,$userId ='') {
 try{
  $userId  = empty($userId)? getSltpersonUserId() : $userId;
  $userId = !empty($userId) ? $userId : 0;

  $selectPreReq = db_select('slt_common_mapping','comMap');
  $selectPreReq->leftJoin('slt_common_mapping','map','map.id1 = comMap.id2 AND map.type = \'6\'  AND map.object_type = \'cre_sys_obt_crs\'');
  $selectPreReq->innerJoin('slt_course_template','crs','crs.id = comMap.id2');
  //$selectPreReq->leftjoin('slt_course_class', 'cl','ms.pk_id=cl.id AND ms.type=\'Class\'');
  $selectPreReq->leftJoin('slt_course_template','crsMap','crsMap.id = comMap.id1');
  //$selectPreReq->leftJoin('slt_enrollment','enr','enr.course_id = cl.course_id  AND cl.id = enr.class_id ');
  //$selectPreReq->leftJoin('slt_enrollment','enr','enr.course_id = cl.course_id  AND cl.id = enr.class_id AND enr.comp_status IN (:regstatus) AND enr.comp_status IN (:regstatus1)',array('regstatus' =>'lrn_crs_cmp_enr','regstatus1'=>"'lrn_crs_cmp_inp','lrn_crs_cmp_inc','lrn_crs_cmp_cmp'"));
  $selectPreReq->leftJoin('slt_enrollment','enr','(enr.course_id = crs.id or enr.course_id = map.id2)  AND (enr.comp_status IN (:regstatus) OR enr.comp_status IN (:regstatus1) OR enr.comp_status IN (:regstatus2) OR enr.comp_status IN (:regstatus3)) and enr.user_id =:userId',array('regstatus' =>'lrn_crs_cmp_enr','regstatus1' =>'lrn_crs_cmp_inp','regstatus2' =>'lrn_crs_cmp_inc','regstatus3' =>'lrn_crs_cmp_cmp','userId' => $userId));
  $selectPreReq->leftJoin('slt_profile_list_items','prof','enr.comp_status = prof.code');
  //AND enr.comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_inc','lrn_crs_cmp_cmp')
  // COUNT(cm.id1) AS noofReq
  $selectPreReq->condition('comMap.id1',$courseId);
  $selectPreReq->condition('comMap.type','5');

  	$selectPreReq->addField('crs','id','preId');
  	$selectPreReq->addField('crs','id');
  	$selectPreReq->addField('crs','id','program_id');
  	//$selectPreReq->addField('crsMap','program_id');
  	$selectPreReq->addField('crs','short_description','short_description');
	$selectPreReq->addField('crs','title','preTitle');
	$selectPreReq->addField('crs','code','preCode');
	$selectPreReq->addExpression('IFNULL(prof.name,\'Not Registered\')','regstatus');
	$selectPreReq->addExpression('IFNULL(prof.name,\'N\')','enrollstatus');
	$selectPreReq->addExpression('IFNULL(comMap.required_for,\'N\')','mandatory');
	$selectPreReq->addExpression('MIN(IF((map.id2 = enr.course_id and enr.comp_status = \'lrn_crs_cmp_cmp\'),\'Equivalence Completed\',if((map.id2 = enr.course_id and enr.comp_status = \'lrn_crs_cmp_enr\'),\'Equivalence Registered\',\'Zero\')))','equvStatus');//Zero is just a dummy value
	 $selectPreReq->groupBy('crs.id');



  $sortby = $_GET["sidx"];
  $xorderbylist = $_GET["sord"];
  if ($sortby == "name") {
      $xorderby = "crs.title";
    } else if ($sortby == "code") {
      $xorderby = "crs.code";
    } else if ($sortby == "status") {
      $xorderby="prof.name";
    } else {
      $sortby = 'asc';
      $xorderby = "crs.title";
    }

    $selectPreReq->orderBy($xorderby,$xorderbylist);



  $preReqQry = $selectPreReq->execute();
  //print_r($preReqQry);
  $preReqRs = $preReqQry->fetchAll();

  return $preReqRs;
  } catch (Exception $ex) {
  	watchdog_exception('getCatalogPrerequisite', $ex);
  	expertusErrorThrow($ex);
  }
}

function getCatalogClassDetailsPreReq($courseId) {
	try{
  $selectCls = db_select("slt_course_class", "cls");
  $selectCls->innerJoin("slt_profile_list_items","prof","cls.delivery_type = prof.code");
  $selectCls->leftJoin("slt_enrollment","enr","enr.class_id = cls.id and cls.course_id = enr.course_id");
  $selectCls->leftJoin("slt_course_class_session","sess","sess.class_id = cls.id and sess.course_id = cls.course_id");
  $selectCls->leftJoin("slt_location","loc","loc.id = sess.location_id");
  $selectCls->addField('cls','id','classId');
  $selectCls->addField('cls','title');
  $selectCls->addField('cls','description');
  $selectCls->addField('prof','name','classType');
  $selectCls->addField("loc","id","locationId");
  $selectCls->addField("enr","id","enrollId");
  $selectCls->addField("loc","name","locationName");
  $selectCls->addField("loc","addr1","loationAddr1");
  $selectCls->addField("loc","addr2","locationAddr2");
  $selectCls->addField("loc","city","LocationCity");
  $selectCls->addField("loc","state","locationState");
  $selectCls->addField("loc","zipcode","locationZip");
  $selectCls->addField("loc","phone","locationPhone");

  $selectCls->condition('cls.course_id',$courseId,'=');
  $selectCls->condition('cls.status','lrn_cls_sts_atv','=');

  $selectCls->groupBy('cls.id');

  $clsQry = $selectCls->execute();

  $clsResp = $clsQry->fetchAll();

  return $clsResp;
  } catch (Exception $ex) {
  	watchdog_exception('getCatalogClassDetailsPreReq', $ex);
  	expertusErrorThrow($ex);
  }

}

function fetch_classes_detail_list($courseId) {
 try{
	$userId  = getSltpersonUserId();
  $userId = !empty($userId) ? $userId : 0;
  // Added by Vincent on Oct 29, 2013 for #0028593
  $timeZone = date_default_timezone(false);
  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  //Modified for Multi currency support.
  $UserDefaultCurrencyArr = getUserDefaultCurrency();
  $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
  $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];

	$selectclass = srchCatalogQuery();
	//$selectclass->innerJoin('slt_common_mapping','coMap','coMap.id2 = cl.course_id and coMap.prereq_type = \'cre_sys_pre_crs\'');
	$selectclass->leftjoin('slt_enrollment','enr','enr.course_id = cl.course_id and enr.class_id = cl.id and enr.user_id = '.$userId);


	//$selectTpPreReq = db_select('slt_module_crs_mapping','crsMap');
  	//$selectTpPreReq->innerJoin('slt_program','prgm','prgm.id = crsMap.program_id AND prgm.object_type = prgm.object_type');


	/**/
	/*$selectclass = db_select('slt_master_search', 'ms');
    $selectclass->leftjoin('slt_course_class', 'cl','ms.pk_id=cl.id AND ms.type=\'Class\'');
    $selectclass->leftjoin('slt_course_class_session', 'cs', 'ms.pk_id=cs.class_id and cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1)');
    $selectclass->leftjoin('slt_location', 'ss','cs.location_id= ss.id');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=ms.delivery_type');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_status', 'splt_status.code=ms.status');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_language', 'splt_language.code=ms.lang_code');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=cs.timezone');
    $selectclass->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =ms.pk_id and if(ms.type=\'Class\',nl.entity_type = \'cre_sys_obt_cls\',nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\'))');
    $selectclass->leftjoin('slt_country', 'sc', 'sc.country_code= ss.country');
    $selectclass->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE ms.pk_id = cont.class_id AND ms.parent_pk = cont.course_id LIMIT 1 )');
    $selectclass->leftjoin('slt_entity_profile_mapping', 'sepm', 'sepm.entity_id=ms.pk_id AND sepm.entity_type IN (\'27\',\'28\',\'31\',\'32\')');
    $selectclass->leftjoin('slt_program', 'prm', 'prm.id=ms.pk_id AND ms.type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
	*/
	//$selectclass->addField('coMap','id1','program_id');
	$selectclass->addField('ms','type', 'object_type');
    $selectclass->addField('ms','parent_pk', 'crs_id');
    $selectclass->addField('ms','pk_id', 'cls_id');
    $selectclass->addField('ms','pk_id', 'preid');
    $selectclass->addField('ms','code', 'cls_code');
    $selectclass->addField('ms','name', 'cls_title');
    $selectclass->addField('ms','description', 'cls_short_description');
    $selectclass->addField('ms','delivery_type', 'delivery_type_code');
    $selectclass->addExpression('\''.$UserPrefCurrSym.'\'', 'currency_type');
    $selectclass->addField('cl','custom4', 'export_compliance');
    $selectclass->addField('splt_deliverytype','name', 'delivery_type_name');
    $selectclass->addField('splt_status','name', 'status');
    $selectclass->addField('splt_language','name', 'language');
    $selectclass->addField('ss','name', 'location');
    $selectclass->addField('nl','node_id', 'node_id');
    $selectclass->addField('cl','price', 'price');
    $selectclass->addField('cs','id', 'session_id');
    $selectclass->addField('cs','start_date', 'sess_start_date');
    $selectclass->addField('cs','start_time', 'sess_start_time');
    $selectclass->addField('cs','end_time', 'sess_end_time');
    $selectclass->addField('cs','end_date', 'sess_end_date');
    $selectclass->addField('cs','timezone', 'timezone'); // Added by Vincent on Oct 29, 2013 for #0028593
    $selectclass->addField('splt_timezone','attr2', 'sess_timezone');
    $selectclass->addField('sc','country_name', 'country_name');
    $selectclass->addField('cl','lang_code', 'language_code');
    $selectclass->addField('sc','country_code', 'country_code');
    $selectclass->addField('cl','registration_end_on', 'registration_end_on');

    $selectclass->addField('ss','name', 'locationName');
    $selectclass->addField('ss','addr1', 'loationAddr1');
    $selectclass->addField('ss','addr2', 'locationAddr2');
    $selectclass->addField('ss','city', 'LocationCity');
    $selectclass->addExpression('(select country_name from slt_country where country_code = ss.country)', 'locationcountryname');
    $selectclass->addField('ss','state', 'locationState');
    $selectclass->addExpression('(select state_name from slt_state where state_code = ss.state and country_code =  ss.country)', 'locationstatename');
    $selectclass->addField('ss','country', 'locationCountry');
    $selectclass->addField('ss','zipcode', 'locationZip');
    $selectclass->addField('ss','phone', 'locationPhone');
    $selectclass->addField('ss','name', 'location');

    $selectclass->addField('prm','expires_in_value', 'expires_in_value');
    $selectclass->addField('prm','expires_in_unit', 'expires_in_unit');
    $selectclass->addField('prm','price', 'prm_price');
    $selectclass->addField('prm','currency_type', 'prm_currency_type');
    $selectclass->addField('prm','end_date', 'prm_end_date');
    $selectclass->addField('prm','created_on', 'prm_created_on');
    $selectclass->addExpression('IFNULL(enr.id,\'N\')','regstatus');
    $selectclass->addExpression('IFNULL(enr.id,\'N\')','enrollstatus');
    // Added by Vincent on Oct 29, 2013 for #0028593
    $selectclass->addExpression("CONCAT(DATE_FORMAT(cs.start_date, _utf8'%Y-%m-%d'),' ',cs.start_time,':00')", 'session_end_datetime');

    // commented for timezone fixes
   /*
    $selectclass->where('IF(ms.type=\'Class\',
    ( splt_deliverytype.lang_code=\'cre_sys_lng_eng\'
    and splt_status.lang_code=\'cre_sys_lng_eng\'
    and splt_language.lang_code=\'cre_sys_lng_eng\'
    and nl.entity_type=\'cre_sys_obt_cls\'
    and ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\')  and crs.status = \'lrn_crs_sts_atv\'),1=1 )'
    );

    $selectclass->where('IF(ms.type!=\'Class\',
    (
    ms.status in (\'lrn_lpn_sts_atv\') AND map.program_id=ms.pk_id),1=1 )'
    );


    $startdate = Date("Y-m-d");
    $time = Date("H:i:s");

      $selectclass->where('IF(ms.type=\'Class\',
      (
      ( ms.delivery_type=\'lrn_cls_dty_ilt\' and cs.start_date >= \''.$startdate.'\' and
      	IF(cs.start_date =\''.$startdate.'\', cs.start_time >= \''.$time.'\',1=1) )
      or
      ( ms.delivery_type=\'lrn_cls_dty_vcl\' and cs.start_date >= \''.$startdate.'\' and
      	IF(cs.start_date =\''.$startdate.'\', cs.start_time >= \''.$time.'\',1=1)  )
      or
      ( ms.delivery_type=\'lrn_cls_dty_wbt\' and content.content_id IS NOT NULL )
      )
      or
      ( ms.delivery_type=\'lrn_cls_dty_vod\' and content.content_id IS NOT NULL ),
      1=1)'); */
    	$selectclass->condition('cl.status','lrn_cls_sts_atv','=');
      $selectclass->where('cl.dedicated_class_flag = \'N\' or cl.dedicated_class_flag is NULL');
      //64032: If past date WBT date class has been set as prerequest for the current class, it is allowing to register the closed WBT class --start
      if(isset($_SESSION['user_timezone']['zone_name']) && !empty($_SESSION['user_timezone']['zone_name']))
      	$user_time_zone = $_SESSION['user_timezone']['zone_name'];
      else
      	$user_time_zone = $defaultTimezone;
      $selectclass->where("IF(cl.registration_end_on is null,1,cl.registration_end_on >= CONVERT_TZ(DATE(now()), '".$defaultTimezone."' ,'".$user_time_zone."'))");
      //64032: If past date WBT date class has been set as prerequest for the current class, it is allowing to register the closed WBT class --end
     // addFilterForLoggedInUserRole($selectclass, 'sepm','','ms');
     filterByUserAccessPrivileges($selectclass, '','','','','Myteam');
    //$selectclass->condition('1','1','=');
    $selectclass->condition('cl.course_id',$courseId,'=');
    // Added by Vincent on Oct 29, 2013 for #0028593
    //$selectclass->groupBy('cl.id');
    $selectclass->groupBy("cl.id having session_end_datetime > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= cs.timezone)),NOW()) or session_end_datetime is null");
    expDebug::dPrintDBAPI("prereqisite class fetch query ", $selectclass);

	$clsQry = $selectclass->execute();
  	//print_r($clsQry);
  	$classesListRes = $clsQry->fetchAll();

    return $classesListRes;
    } catch (Exception $ex) {
    	watchdog_exception('fetch_classes_detail_list', $ex);
    	expertusErrorThrow($ex);
    }
  }



/**
 * Get the count of Registered Course.
 *
 * @param string $courseid
 *
 * @return
 *   count of the Registered Course.
 */


function getRegisteredCourseCheck($courseid){
  try{
  $userId = getSltpersonUserId();

  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(id)', 'registered_class');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  //$select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));
  $select->condition(db_and()->condition('enroll.comp_status',array('lrn_crs_cmp_cmp'), 'IN'));
  $no_of_registered_class = $select->execute()->fetchField();

  if ($no_of_registered_class >= 1 ) {
    $registeredOption = 1;   // Class Registered
  }else{
    $registeredOption = 0;   // Class Not Registered
  }

 // print $registeredOption;
  return $registeredOption;
  } catch (Exception $ex) {
  	watchdog_exception('getRegisteredCourseCheck', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getCatalogTpPrerequisite() - Get the list of prerequisite training programs
 *
 * @param string $courseid
 *
 * @return
 *   List of training programs
 */

function getCatalogTpPrerequisite($catalogId,$userId='') {
  try{
  	if(empty($userId)) {
  		$userId  = getSltpersonUserId();
  	}
 	 $userId = !empty($userId) ? $userId : 0;

  $selectTpPreReq = db_select('slt_common_mapping','comMap');
  $selectTpPreReq->leftJoin('slt_common_mapping','map','map.id1 = comMap.id2 AND map.type = \'6\'  AND map.object_type = \'cre_sys_obt_crs\'');
  $selectTpPreReq->leftJoin('slt_program','prgm','prgm.id = comMap.id2  AND comMap.prereq_type = \'cre_sys_pre_trp\'');
  $selectTpPreReq->leftJoin('slt_course_template','crs','crs.id = comMap.id2 AND comMap.prereq_type = \'cre_sys_pre_crs\'');
  $selectTpPreReq->leftJoin('slt_master_enrollment','msEnr','msEnr.program_id = prgm.id and msEnr.user_id = '.$userId);
  $selectTpPreReq->leftJoin('slt_enrollment','enr','enr.master_enrollment_id = msEnr.id and enr.user_id = '.$userId);
  $selectTpPreReq->leftJoin('slt_enrollment','enr1','(enr1.course_id= crs.id or enr1.course_id = map.id2) and enr1.user_id = '.$userId);
  $selectTpPreReq->leftJoin('slt_profile_list_items','prof','prgm.object_type = prof.code');
  $selectTpPreReq->leftJoin('slt_profile_list_items','prof1','enr1.comp_status = prof1.code');
  $selectTpPreReq->leftJoin('slt_profile_list_items','prof1','enr1.comp_status = prof1.code');
  $selectTpPreReq->leftJoin('slt_node_learning_activity','nod','nod.entity_id = prgm.id and nod.entity_type = prgm.object_type');
  $selectTpPreReq->condition('comMap.type','5');
  $selectTpPreReq->condition('comMap.id1',$catalogId);
  $selectTpPreReq->condition('comMap.object_type','cre_sys_obt_trp');

  $selectTpPreReq->addField('comMap','id');
  $selectTpPreReq->addField('prgm','id','program_id');
  $selectTpPreReq->addExpression('IFNULL(prgm.id,crs.id)','preId');
  $selectTpPreReq->addExpression('IFNULL(nod.node_id,0)','node_id');
  $selectTpPreReq->addExpression('IFNULL(crs.title,prgm.title)','preTitle');
  $selectTpPreReq->addExpression('IFNULL(crs.code,prgm.code)','preCode');
  $selectTpPreReq->addExpression('IFNULL(prgm.price,0)','price');
  $selectTpPreReq->addExpression('IFNULL(prgm.short_desc,crs.short_description)','shortDesc');
  $selectTpPreReq->addExpression('IFNULL(prof.name,\'Course\')','preType');
  $selectTpPreReq->addExpression('IFNULL(prof1.name,\'N\')','preEnrStatus');
  $selectTpPreReq->addExpression('IFNULL(enr.id,\'N\')','preMsEnrStatus');
  $selectTpPreReq->addExpression('IFNULL(enr.id,IFNULL(enr1.id,\'N\') )','enrollstatus');
  $selectTpPreReq->addExpression('MIN(IF((map.id2 = enr1.course_id and enr1.comp_status = \'lrn_crs_cmp_cmp\'),\'Equivalence Completed\',if((map.id2 = enr1.course_id and enr1.comp_status = \'lrn_crs_cmp_enr\'),\'Equivalence Registered\',\'Zero\')))','equvStatus');//Zero is just a dummy value
  // AS EnrStatusNew
  $selectTpPreReq->addField('prgm','object_type','prereqtype');
  $selectTpPreReq->groupBy('comMap.id');
  $preReqQry = $selectTpPreReq->execute();
  //print_r($preReqQry);
  $preReqRs = $preReqQry->fetchAll();
  //print_r($preReqRs);
  return $preReqRs;
  } catch (Exception $ex) {
  	watchdog_exception('getCatalogTpPrerequisite', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * getTpPrerequisite() - Get the list of training programs courses
 *
 * @param string $tpid
 *
 * @return
 *   count of the Registered Course.
 */
function getTpPrerequisite($tpid,$tpType) {
	try{
		$userId  = getSltpersonUserId();
  	$userId = !empty($userId) ? $userId : 0;

		$selectTpPreReq = db_select('slt_module_crs_mapping','crsMap');
  	$selectTpPreReq->innerJoin('slt_program','prgm','prgm.id = crsMap.program_id AND prgm.object_type = prgm.object_type');
  	//slt_common_mapping
  	$selectTpPreReq->innerJoin('slt_course_template','crs','crs.id = crsMap.course_id');
  	//$selectTpPreReq->innerJoin('slt_common_mapping','coMap','coMap.id2 = crs.id and coMap.prereq_type = \'cre_sys_pre_crs\'');
  	$selectTpPreReq->leftJoin('slt_master_enrollment','msEnr','msEnr.program_id = prgm.id AND msEnr.user_id ='.$userId);
  	//$selectTpPreReq->leftJoin('slt_enrollment','enr','enr.master_enrollment_id = msEnr.id and enr.course_id = crs.id');
  	$selectTpPreReq->leftJoin('slt_node_learning_activity','nod','nod.entity_id ='.$tpid.' and nod.entity_type=\''.$tpType.'\'');
  	$selectTpPreReq->leftJoin('slt_enrollment','enr1','enr1.course_id = crs.id and enr1.user_id='.$userId);
  	$selectTpPreReq->leftJoin('slt_profile_list_items','prof','prof.code = enr1.comp_status');

  	$selectTpPreReq->condition('crsMap.program_id',$tpid); //cre_sys_obt_trn
  	$selectTpPreReq->condition('crsMap.object_type',$tpType);

  	$selectTpPreReq->addField('crs','id','preId');
  	$selectTpPreReq->addField('crs','id');
  	$selectTpPreReq->addField('nod','node_id');
  	$selectTpPreReq->addField('crsMap','program_id');
  	$selectTpPreReq->addField('crs','short_description');
  	//$selectTpPreReq->addField('coMap','id1','program_id');
	$selectTpPreReq->addField('crs','title','preTitle');
	$selectTpPreReq->addField('crs','code','preCode');
	$selectTpPreReq->addExpression('IF((prgm.id != \'\'),\'program\',\'course\')','dispType');
	$selectTpPreReq->addExpression('IFNULL(prgm.price,0)','price');
	$selectTpPreReq->addExpression('IFNULL(prof.name,\'N\')','regstatus');
	$selectTpPreReq->addExpression('IFNULL(prof.name,\'N\')','enrollstatus');
	$selectTpPreReq->addExpression('IFNULL(crsMap.is_required,\'N\')','mandatory');

	$selectTpPreReq->groupBy('crsMap.id');

	$preReqQry = $selectTpPreReq->execute();
  	//print_r($preReqQry);
  	$preReqRs = $preReqQry->fetchAll();
	//print_r($preReqRs);

  	return $preReqRs;
  	} catch (Exception $ex) {
  		watchdog_exception('getTpPrerequisite', $ex);
  		expertusErrorThrow($ex);
  	}
}

/**
 * Get the  Registered Status for Course.
 *
 * @param string $courseid
 *
 * @return
 */


//function getRegisteredStatusForCourse($courseid){
function getRegisteredCourseStatus($courseid,$userId='') {
try{
	if(empty($userId)) {
  		$userId = getSltpersonUserId();
	}

  $select = db_select('slt_enrollment', 'enroll');
  $select->leftJoin('slt_profile_list_items','prof','prof.code = enroll.comp_status');
  //$select->addExpression('COUNT(id)', 'registered_class');
  $select->addField('prof','name','enroll_status');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));

  $registeredQry = $select->execute();//->fetchField();
  $registeredRs = $registeredQry->fetchAll();

  return $registeredRs;
  } catch (Exception $ex) {
  	watchdog_exception('getRegisteredCourseStatus', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 *  Check Pre-requisite for class Enroll status
 */

function checkCatalogPrerequisiteEnroll($catalogId,$catalogType,$userId='') {
  try{
  	if(empty($userId)){
    	$userId  = getSltpersonUserId();
  	}
    $userId = !empty($userId) ? $userId : 0;

	  $selectPreReq = db_select('slt_common_mapping','comMap');
	  $selectPreReq->leftJoin('slt_master_enrollment','msenr','msenr.program_id = comMap.id2 AND msenr.user_id = '.$userId . ' AND msenr.overall_status  IN(\'lrn_tpm_ovr_enr\')');
	  $selectPreReq->leftJoin('slt_enrollment','enr','enr.user_id = '.$userId.'  AND IF(comMap.prereq_type=\'cre_sys_pre_trp\',(msenr.id = enr.master_enrollment_id),(enr.course_id = comMap.id2 )) AND ((enr.comp_status in(\'lrn_crs_cmp_enr\')) OR ((enr.waitlist_flag = \'lrn_crs_reg_wtl\') AND enr.reg_status in (\'lrn_crs_reg_wtl\')))');
	  $selectPreReq->addField('comMap','id1');
	  $selectPreReq->condition('comMap.id1',$catalogId);
	  $selectPreReq->condition('comMap.object_type',$catalogType);
	 // $selectPreReq->where('(IF(comMap.prereq_type=\'cre_sys_pre_trp\',IFNULL(msenr.overall_status,\'\')=\'lrn_tpm_ovr_enr\',IFNULL(enr.comp_status,\'\')=\'lrn_crs_cmp_enr\'))');
	  $selectPreReq->where('(IF(comMap.prereq_type=\'cre_sys_pre_trp\',IFNULL(msenr.overall_status,\'\')=\'lrn_tpm_ovr_enr\',IF(enr.comp_status=\'lrn_crs_cmp_enr\' OR enr.comp_status=\'\' OR enr.waitlist_flag = \'lrn_crs_reg_wtl\',1,0)))'); // Fixed the enrolled status waitlist  ticket no :  0021486

	  $selectPreReq->groupBy('comMap.id');
	  $preReqQry = $selectPreReq->execute();
	  //echo "$catalogId |,| $catalogType"; print_r($preReqQry);
	  $preReqRs = $preReqQry->fetchAll();
	  expDebug::dPrintDBAPI("Db_PrintAPI prreqiste details", $selectPreReq);
	  return $preReqRs;
	  } catch (Exception $ex) {
	  	watchdog_exception('checkCatalogPrerequisiteEnroll', $ex);
	  	expertusErrorThrow($ex);
	  }
}

?>