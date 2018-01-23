<?php
function srchCatalogQuery($userId=''){
  try {
  if(empty($userId))
    $userId  = getIdOfLoggedInUser();
    $userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
  /* Derive timezone attribute of session from slt_profile_list_items table*/
    $select = db_select('slt_master_search', 'ms');
    $select->leftjoin('slt_course_class', 'cl','ms.pk_id=cl.id AND ms.type=\'Class\'');
    $select->leftjoin('slt_course_template', 'crs','cl.course_id=crs.id');
    $select->leftjoin('slt_course_class_session', 'cs', 'ms.type=\'Class\' AND ms.pk_id=cs.class_id and cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date, start_time LIMIT 1)');
    $select->leftjoin('slt_location', 'ss','cs.location_id= ss.id');
    $select->leftjoin('slt_facility', 'sf','sf.id = (SELECT id FROM slt_facility WHERE location_id=ss.id ORDER BY id LIMIT 1)');
    $select->leftjoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=ms.delivery_type');
    $select->leftjoin('slt_profile_list_items', 'splt_status', 'splt_status.code=ms.status');
    $select->leftjoin('slt_profile_list_items', 'splt_language', 'splt_language.code=ms.lang_code');
    $select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=cs.timezone');
    $select->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =ms.pk_id and if(ms.type=\'Class\',nl.entity_type = \'cre_sys_obt_cls\',nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\'))');
    $select->leftjoin('slt_country', 'sc', 'sc.country_code= sf.contact_country');
    $select->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE ms.pk_id = cont.class_id AND ms.parent_pk = cont.course_id LIMIT 1 )');
   //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=ms.pk_id AND IF(ms.type=\'Class\', catacs.entity_type = \'cre_sys_obt_cls\', catacs.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) ');
   // $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=ms.pk_id AND IF(ms.type=\'Class\', group_map.entity_type = \'cre_sys_obt_cls\', group_map.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) and group_map.group_type = 0');
   // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');

    $select->leftjoin('slt_program', 'prm', 'prm.id=ms.pk_id AND ms.type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
    $select->leftjoin('slt_module_crs_mapping','map','map.id = (SELECT id FROM  slt_module_crs_mapping WHERE program_id=ms.pk_id GROUP BY program_id)');
   // $select->leftjoin('slt_mro_mapping', 'mro', 'mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND IF(ms.type=\'Class\', mro.entity_type = \'cre_sys_obt_cls\', mro.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\'))');
   // $select->leftjoin('slt_profile_list_items', 'splt_mro', 'splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\'');
    $select->leftjoin('slt_tag_entity', 'te','((cl.id= te.entity_id AND te.entity_type=\'Class\') OR (crs.id=te.entity_id AND te.entity_type=\'Course\') OR (prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\')))');
    $select->leftjoin('slt_tagdefn', 'td','te.tagid= td.id');
	if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
    	$select->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and if(ms.type=\'Class\',vote.entity_type = \'Class\',vote.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\'))');
	}
    return $select;
  }catch(Exception $ex) {
	watchdog_exception('srchCatalogQuery', $ex);
	expertusErrorThrow($ex);
  }
}
function getRequestValue($param_name)
{   
    try{
        if(isset($_GET[$param_name])) {
            return $_GET[$param_name];
        } else {
            return $_REQUEST[$param_name];
        }
    } catch (Exception $ex) {

    }
}

function catalogClassSearchQuery($userId='',$noSession = false, $queryfrom='',$skipAccess=''){
	try{
		if(empty($userId))
			$userId  = getIdOfLoggedInUser();
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;

		//$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = cls.id AND group_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=cls.id
		//AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=cls.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';
		
		$UserDefaultCurrencyArr = getUserDefaultCurrency();
		$UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
		$UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
		$UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];
		
		$selectCls = db_select('slt_course_class', 'cls');
		$selectCls->leftJoin('slt_course_template','templ','cls.course_id = templ.id');
		$selectCls->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
		$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		// $selectCls->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\' and group_map.group_type = 0');
		//$selectCls->leftJoin('slt_group_mapping','group_map','('.$classsrQuery.') and group_map.group_type = 0');

		//$selectCls->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
		//$selectCls->leftJoin('slt_catalog_access','catacs','catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
		//$selectCls->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type = \'cre_sys_obt_cls\'');
		// $selectCls->leftJoin('slt_profile_list_items','splt_mro','splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\'');
		//$tag = isset($_GET['tag']) ? $_GET['tag']:(isset($_REQUEST['tag']) ? $_REQUEST['tag']:'');
		/*$tag=getRequestValue('tag');
		if(!empty($tag)){
			$selectCls->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}*/
		$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
		$enrollIncluded = 0;
		$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
		if((($mroType) && $userId) || $sortByMandatory){
	  $enrollIncluded = 1;
	  $selectCls->leftJoin('slt_enrollment','enroll','enroll.class_id = cls.id and enroll.user_id = \''.$userId.'\' and enroll.reg_status IN(\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\')');
		}
		$contrytype = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
		$locationtype = getRequestValue('location');
                $startdate = getRequestValue('startdate');
		$enddate = getRequestValue('enddate');
		$sortby = isset($_GET['sortby']) ? $_GET['sortby']:'';
		if((!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype) || ($sortby == 'ClassStartDate')) && $noSession==false){
			
			/* Added/changed by ganeshbabuv on Feb 13th 2016 for Unable to filter the ILT trainings in Learner Catalog if ILT class has more than one sessions #0072082 */
				
			//$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id and ses.id = (select id from slt_course_class_session where class_id=cls.id limit 1)');
			$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id');  
			
			if(!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype)){
			     
			    $timeZone = date_default_timezone(false);
			    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			    $sessionTZ = new DateTimeZone($defaultTimezone);
			    $toDay = new DateTime(null,$sessionTZ) ;
			    $toDayDT = $toDay->format('Y-m-d H:i');
				     
		        $cls_aliase = ($_REQUEST['apiname'] == 'CatalogSearchAPI') ? 'cls' : 'cls' ;			 
							
				$selectCls->leftJoin('slt_course_class_session','ses_past','ses_past.class_id='.$cls_aliase.'.id and ses_past.course_id='.$cls_aliase.'.course_id 
		    	and CONVERT_TZ(CONCAT(DATE_FORMAT(ses_past.start_date, _utf8\'%Y-%m-%d\'),\' \',ses_past.start_time),(select pro_past.attr2 from slt_profile_list_items pro_past where pro_past.code= ses_past.timezone),\''.$defaultTimezone.'\')<=\''.$toDayDT.'\'');
			    
  				$selectCls->where('ses_past.id is null or ses_past.id =\'\'');  
				
			}			
		}
		if($queryfrom=='AddTraining' && $userId){
			$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id');
		}
		if(!empty($contrytype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
			$selectCls->leftJoin('slt_country','con','con.country_code=loc.country');
		}
		
		/* Added/changed by ganeshbabuv on Jan 11th 2016 for  Location Filter #66404 */
		if(!empty($locationtype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
			$selectCls->leftJoin('slt_country','loc_ctry','loc_ctry.country_code=loc.country');
			$selectCls->leftJoin('slt_state','loc_stats','loc_stats.state_code = loc.state and loc_stats.country_code=loc.country');
		}

		if( $_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['cls_status'] == 'Incomplete'){


			$selectCls->leftJoin('slt_enrollment','enroll','enroll.class_id = cls.id and enroll.course_id = cls.course_id');
			$selectCls->condition('enroll.user_id',$userId);
			$selectCls->condition('enroll.comp_status','lrn_crs_cmp_inc');
			//mobile_analytics_issue
			if($_REQUEST['object_type'] == 'Class' && $_REQUEST['mobile_request']){
				$selectCls->condition('enroll.master_enrollment_id','', 'is null');
			}

			if($_REQUEST['compliance_mandatory'] == 1)
			{
				expDebug::dPrint("innnnnnnnn");
				$or_query = db_or();
				$or_query->condition('enroll.is_compliance', '1');
				$or_query->condition('enroll.mandatory', 'Y');
				$selectCls->condition($or_query);
			}
		}

		$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
		if(isset($_REQUEST['apiname'])){
			$ratingtype = $_REQUEST['rating_type'];
		}

		if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) {
			$selectCls->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =cls.id and nl.entity_type = \'cre_sys_obt_cls\'');
			$selectCls->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type = \'Class\'');
		}
		
		if ($queryfrom == 'access_filter') {
			$selectCls->addField('cls','id', 'cls_id');
			return $selectCls;
		}
		
		if($_REQUEST['apiname'] == 'CatalogSearchAPI' || ($queryfrom != 'catalogquery')){
			$selectCls->leftJoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=cls.delivery_type');
			$selectCls->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
			$selectCls->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=cls.currency_type');
		$selectCls->addField('cls','id', 'cls_id');
		$selectCls->addField('cls','code', 'cls_code');
		$selectCls->addField('cls','title', 'cls_title');
		$selectCls->addField('cls','short_description', 'cls_short_description');
		if($queryfrom == 'selectclass'){
		$selectCls->addExpression('slf_convert_price(cls.price,cls.currency_type,\''.$UserPrefCurrCode.'\')','price');
		
		}else{
			$selectCls->addField('cls','price', 'price');
		}
		$selectCls->addField('cls','currency_type', 'currency_type');
		$selectCls->addField('cls','registration_end_on', 'registration_end_on');
		$selectCls->addField('cls','created_on', 'created_on');
		$selectCls->addField('cls','published_on', 'published_on');
		$selectCls->addField('splt_currency','attr1', 'currency_code');
		$selectCls->addField('splt_currency','attr2', 'currency_symbol');
		$selectCls->addField('splt_currency','code', 'currency_long_code');
		//$selectCls->addField('td','tagname', 'tagname');
		
		// if($GLOBALS["user"]->uid != 1){
		//$selectCls->addField('group_map','mro', 'mro_id');
		if($skipAccess !=1){
			$selectCls->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
		}
		// $selectCls->addField('splt_mro','name', 'mro');
		$selectCls->addField('cls','lang_code', 'language_code');
		$selectCls->addField('splt_language','name', 'language');
		if($sortby == 'ClassStartDate'){
			$selectCls->addExpression('ses.start_date', 'start_date');
		}
		else{
			$selectCls->addExpression('null', 'start_date');
		}
		$selectCls->addExpression('cls.location_id', 'location_id');
		$selectCls->addExpression('cls.course_id', 'crs_id');
		$selectCls->addExpression('cls.delivery_type', 'delivery_type_code');
		$selectCls->addExpression('splt_deliverytype.name', 'delivery_type_name');
		$selectCls->addExpression('cls.custom4', 'export_compliance');
		$selectCls->addExpression('null', 'expires_in_value');
		$selectCls->addExpression('null', 'expires_in_unit');
		$selectCls->addExpression('null', 'prm_created_on');
		$selectCls->addExpression('\'Class\'', 'object_type');
		$selectCls->addExpression('cls.is_compliance', 'is_compliance');
		if($enrollIncluded){
			$selectCls->addExpression('enroll.mandatory', 'mandatory');
		}
		$selectCls->addExpression('null', 'avgvote');
				//$selectCls->addExpression('if(group_map.mro = \'cre_sys_inv_opt\',3,if(group_map.mro = \'cre_sys_inv_man\',1,2))', 'ascen');
		if($skipAccess !=1){
			$selectCls->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
		}
		$selectCls->addExpression('(select GROUP_CONCAT(tagsub.tagname) from
	    			(select cls.course_id as crsid,
	    			GROUP_CONCAT(td.tagname) as tagname
	    			from slt_course_class cls
	    			left join slt_tag_entity te on ((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))
	    			left join slt_tagdefn td ON te.tagid= td.id
	    			group by cls.id) tagsub
	    			where tagsub.crsid =  crs_id)','tagname');
		if(isset($_REQUEST['mobile_request'])){
			$selectCls->addField('cls','additional_info', 'additional_info');
			$selectCls->addField('cls','addn_catalog_show', 'addn_catalog_show');
		}
		
		}else{
			$selectCls->addField('cls','id', 'cls_id');
			$selectCls->addField('cls','code', 'cls_code');
			$selectCls->addField('cls','title', 'cls_title');
			$selectCls->addField('cls','short_description', 'cls_short_description');
			if($queryfrom == 'selectclass'){
			$selectCls->addExpression('slf_convert_price(cls.price,cls.currency_type,\''.$UserPrefCurrCode.'\')','price');
			
			}else{
				$selectCls->addField('cls','price', 'price');
			}
			$selectCls->addField('cls','currency_type', 'currency_type');
			$selectCls->addField('cls','registration_end_on', 'registration_end_on');
			$selectCls->addField('cls','created_on', 'created_on');
			$selectCls->addField('cls','published_on', 'published_on');
			//$selectCls->addField('group_map','mro', 'mro_id');
			$selectCls->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
			//$selectCls->addField('td','tagname', 'tagname');
		
			$selectCls->addExpression('cls.course_id', 'crs_id');
			if($sortby == 'ClassStartDate'){
				$selectCls->addExpression('ses.start_date', 'start_date');
			}
			else{
				$selectCls->addExpression('null', 'start_date');
			}
			$selectCls->addExpression('cls.custom4', 'export_compliance');
			$selectCls->addExpression('null', 'expires_in_value');
			$selectCls->addExpression('null', 'expires_in_unit');
			$selectCls->addExpression('null', 'prm_created_on');
			$selectCls->addExpression('\'Class\'', 'object_type');
			$selectCls->addExpression('cls.is_compliance', 'is_compliance');
			if($enrollIncluded){
				$selectCls->addExpression('enroll.mandatory', 'mandatory');
			}
			$selectCls->addExpression('null', 'avgvote');
			$selectCls->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
			$selectCls->addExpression('(select GROUP_CONCAT(tagsub.tagname) from
	    			(select cls.course_id as crsid,
	    			GROUP_CONCAT(td.tagname) as tagname
	    			from slt_course_class cls
	    			left join slt_tag_entity te on ((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))
	    			left join slt_tagdefn td ON te.tagid= td.id
	    			group by cls.id) tagsub
	    			where tagsub.crsid = crs_id)','tagname');
		}
		
		return $selectCls;
	}catch(Exception $ex) {
		watchdog_exception('catalogClassSearchQuery', $ex);
		expertusErrorThrow($ex);
	}
}

function catalogClassSearchQueryCron($userId='',$noSession = false){
	try{
		if(empty($userId))
			$userId  = getIdOfLoggedInUser();
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;

		//$classsrQuery = 'if((select count(1) from slt_group_mapping grp_map where grp_map.entity_id = cls.id AND group_map.entity_type = \'cre_sys_obt_cls\')>0,(group_map.entity_id=cls.id
		//AND group_map.entity_type = \'cre_sys_obt_cls\'),(group_map.entity_id=cls.course_id AND group_map.entity_type = \'cre_sys_obt_crs\'))';

		$selectCls = db_select('slt_course_class', 'cls');
		$selectCls->leftJoin('slt_course_template','templ','cls.course_id = templ.id');
		$selectCls->leftJoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=cls.delivery_type');
		$selectCls->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
		$selectCls->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=cls.currency_type');


		$tag = isset($_GET['tag']) ? $_GET['tag']:'';
		if(!empty($tag)){
			$selectCls->leftJoin('slt_tag_entity','te','((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}
		$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
		$enrollIncluded = 0;
		$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
		if((($mroType) && $userId) || $sortByMandatory){
	  $enrollIncluded = 1;
	  $selectCls->leftJoin('slt_enrollment','enroll','enroll.class_id = cls.id and enroll.user_id = \''.$userId.'\' and enroll.reg_status IN(\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\')');
		}
		$contrytype = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
		$locationtype = isset($_GET['location']) ? $_GET['location']:'';
		$startdate = isset($_GET['startdate']) ? $_GET['startdate']:'';
		$enddate = isset($_GET['enddate']) ? $_GET['enddate']:'';
		$sortby = isset($_GET['sortby']) ? $_GET['sortby']:'';
		if((!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype) || ($sortby == 'ClassStartDate')) && $noSession==false){
			$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id and ses.id = (select id from slt_course_class_session where class_id=cls.id limit 1)');
		}
		if(!empty($contrytype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
			$selectCls->leftJoin('slt_country','con','con.country_code=loc.country');
		}
		if(!empty($locationtype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
		}
		$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
		if(isset($_REQUEST['apiname'])){
			$ratingtype = $_REQUEST['rating_type'];
		}

		if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) {
			$selectCls->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =cls.id and nl.entity_type = \'cre_sys_obt_cls\'');
			$selectCls->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type = \'Class\'');
		}
		$selectCls->addField('cls','id', 'cls_id');
		$selectCls->addField('cls','code', 'cls_code');
		$selectCls->addField('cls','title', 'cls_title');
		$selectCls->addField('cls','short_description', 'cls_short_description');
		$selectCls->addField('cls','price', 'price');
		$selectCls->addField('cls','currency_type', 'currency_type');
		$selectCls->addField('cls','registration_end_on', 'registration_end_on');
		$selectCls->addField('cls','created_on', 'created_on');
		$selectCls->addField('cls','published_on', 'published_on');
		$selectCls->addField('splt_currency','attr1', 'currency_code');
		$selectCls->addField('splt_currency','attr2', 'currency_symbol');
		$selectCls->addField('splt_currency','code', 'currency_long_code');

		// if($GLOBALS["user"]->uid != 1){
		//$selectCls->addField('group_map','mro', 'mro_id');
		$selectCls->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
		//  }
		// $selectCls->addField('splt_mro','name', 'mro');
		$selectCls->addField('cls','lang_code', 'language_code');
		$selectCls->addField('splt_language','name', 'language');
		//$selectCls->addExpression('GROUP_CONCAT(distinct person.id)','userids');
		if($sortby == 'ClassStartDate'){
			$selectCls->addExpression('ses.start_date', 'start_date');
		}
		else{
			$selectCls->addExpression('null', 'start_date');
		}
		$selectCls->addExpression('cls.location_id', 'location_id');
		$selectCls->addExpression('cls.course_id', 'crs_id');
		$selectCls->addExpression('cls.delivery_type', 'delivery_type_code');
		$selectCls->addExpression('splt_deliverytype.name', 'delivery_type_name');
		$selectCls->addExpression('cls.custom4', 'export_compliance');
		$selectCls->addExpression('null', 'expires_in_value');
		$selectCls->addExpression('null', 'expires_in_unit');
		$selectCls->addExpression('null', 'prm_created_on');
		$selectCls->addExpression('\'Class\'', 'object_type');
		$selectCls->addExpression('cls.is_compliance', 'is_compliance');
		if($enrollIncluded){
			$selectCls->addExpression('enroll.mandatory', 'mandatory');
		}
		$selectCls->addExpression('null', 'avgvote');
		$selectCls->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
		//expDebug::dPrintDBAPI('variable class query', $selectCls);

		return $selectCls;
	}catch(Exception $ex) {
		watchdog_exception('catalogClassSearchQuery', $ex);
		expertusErrorThrow($ex);
	}
}

/* Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712
 * catalogTPCourseAccessQuery() - It checks the access for Classes under TP 
 */
function catalogTPCourseAccessQuery(&$selectStmt,$userId=''){ 
	
		if(empty($userId)) $userId  = getIdOfLoggedInUser();
		
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
		$userAccess  = getUserAccessPriviledges($userId);
		$enabledValues = getEnabledAttrForVisibility();
		$closeBracket='';
		$lastid = 1;
		$enabledCount = count($enabledValues); 
		$classQuery='';
			
		if($enabledCount > 0){
			
				$classQuery = " and (if(FIND_IN_SET($userId,grp.added_users)>0 OR usrmap.user_id = $userId,1=1, (";
				
			foreach($enabledValues as $key => $value){ 
				if($value == 'state')
					$userValue = $userAccess->country.'-'.$userAccess->$value;
				else
					$userValue = $userAccess->$value;

				if($value != 'role' && $value != 'hire_date'){

					if($value == 'job_role'){
						$resultjobrole = explode(',',$userValue);
							$classQuery .= ' (if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (grp.'.$value.' is null or grp.'.$value.'=\'\'';
							
						foreach ($resultjobrole as $job_role){
							expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
							$classQuery .= 'OR (FIND_IN_SET(\''.$job_role.'\',grp.'.$value.'))';
						}
							$classQuery .= '))) ';
							if($lastid != $enabledCount)
						$classQuery .= ' AND ';
					}else{

							$classQuery .= ' (if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(grp.'.$value.' is null or grp.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',grp.'.$value.'))))) ';

							if($lastid != $enabledCount)
						$classQuery .= ' AND ';
					}
				}


				if($value == 'role'){
					$isManager = empty($userAccess->is_manager) ? 'N' : $userAccess->is_manager;
						$classQuery .= '(CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\') THEN (\''.$userAccess->is_manager.'\' = grp.is_manager OR \''.$userAccess->is_instructor.'\' = grp.is_instructor) 
						WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\') THEN (\''.$userAccess->is_manager.'\' = grp.is_manager) 
					  WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\') THEN (\''.$userAccess->is_instructor.'\' = grp.is_instructor) ELSE 1=1 END) ';
						if($lastid != $enabledCount)
					$classQuery .= ' AND ';
				}
				
				if($value == 'hire_date'){
					$perHireDate = $userAccess->hire_date;
					/* $classQuery .= '(CASE WHEN grp.hire_search_mode = \'ste_con_hdt_btw\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') between UNIX_TIMESTAMP(grp.hire_start) AND UNIX_TIMESTAMP(grp.hire_end)
                     				 WHEN grp.hire_search_mode = \'ste_con_hdt_ltn\' THEN  UNIX_TIMESTAMP(\'' .$perHireDate. '\') <= UNIX_TIMESTAMP(grp.hire_start)
                                 	 WHEN grp.hire_search_mode = \'ste_con_hdt_gtn\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') >= UNIX_TIMESTAMP(grp.hire_start) ELSE 1=1 END) ';
					 */
					$classQuery .= '(IF((select att.id from slt_group_attributes att where att.group_id = grp.id)>0,
					  (select att.id from slt_group_attributes att where att.group_id = grp.id and (
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') >= DATE_FORMAT(att.on_or_after_start_date,\'%Y-%m-%d\') ) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') <= DATE_FORMAT(att.on_or_before_start_date,\'%Y-%m-%d\')) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') between DATE_FORMAT(att.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(att.between_end_date,\'%Y-%m-%d\'))
					  )) >0
					  ,1=1)) ';
					if($lastid != $enabledCount)
						$classQuery .= ' AND ';
				}
				
				
				$lastid++;
			}
			 $classQuery .= ' ) and ((grp.removed_users IS NULL) OR (FIND_IN_SET('.$userId.',grp.removed_users) <= 0)) AND usrmap.id IS NULL))';
			}
	 
            $selectStmt->leftJoin('slt_course_template','crs','crs.id=module.course_id');
            $selectStmt->leftJoin('slt_group_mapping','map','(map.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND map.parent_type = \'learning\') and map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = map.group_id)');
      $selectStmt->leftJoin('slt_user_access_mapping','usrmap','(usrmap.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND usrmap.parent_type = \'learning\') and usrmap.access_type = 0 ');
			$selectStmt->leftJoin('slt_groups','grp','grp.id=map.group_id and (grp.is_admin =0) '.$classQuery ); 
}

function catalogCourseSearchQuery($userId='') {
	try{
		if(empty($userId))
			$userId  = getIdOfLoggedInUser();

		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
		$userAccess  = getUserAccessPriviledges($userId);
		$enabledValues = getEnabledAttrForVisibility();
		$closeBracket='';
		$lastid = 1;
		$enabledCount = count($enabledValues);
		if($_REQUEST['apiname'] == 'CatalogSearchAPI'){
			
		if($enabledCount > 0){
			//$selectStmt->where('if((FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0),1=1,('.$aliasCatalogAccess.'.removed_users IS NULL) OR (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0)');
			//	$classQuery .= 'if(select (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0 from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and grp.is_admin IS NULL and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\'),1=1,('.$aliasCatalogAccess.'.removed_users IS NULL) OR (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0)';

				$classQuery = 'if(((select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) 
				and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and (FIND_IN_SET('.$userId.',grp.added_users)>0))>0 OR 
				((SELECT COUNT(1) FROM slt_user_access_mapping um WHERE um.entity_id = cls.id AND um.entity_type=\'cre_sys_obt_cls\' AND um.user_id='.$userId.')>0)),1=1,(
				if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
				(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' 
				AND ( ';
			foreach($enabledValues as $key => $value){
				//$closeBracket = ($lastid == $enabledCount) ? '))':'';
				if($value == 'state')
					$userValue = $userAccess->country.'-'.$userAccess->$value;
				else
					$userValue = $userAccess->$value;

				if($value != 'role' && $value != 'hire_date'){

					if($value == 'job_role'){
						$resultjobrole = explode(',',$userValue);
							$classQuery .= ' (if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (grp.'.$value.' is null or grp.'.$value.'=\'\'';
							
							/* $classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
							(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (grp.'.$value.' is null or grp.'.$value.'=\'\''; */
						foreach ($resultjobrole as $job_role){
							expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
							$classQuery .= 'OR (FIND_IN_SET(\''.$job_role.'\',grp.'.$value.'))';
						}
							$classQuery .= '))) ';
							if($lastid != $enabledCount)
						$classQuery .= ' AND ';
					}else{

							$classQuery .= ' (if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(grp.'.$value.' is null or grp.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',grp.'.$value.'))))) ';
							
							/* $classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
							(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(grp.'.$value.' is null or grp.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',grp.'.$value.')))))>0, 1=1)'; */
							if($lastid != $enabledCount)
						$classQuery .= ' AND ';
					}
				}


				if($value == 'role'){
					$isManager = empty($userAccess->is_manager) ? 'N' : $userAccess->is_manager;
						$classQuery .= '(CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\') THEN (\''.$userAccess->is_manager.'\' = grp.is_manager OR \''.$userAccess->is_instructor.'\' = grp.is_instructor) 
						WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\') THEN (\''.$userAccess->is_manager.'\' = grp.is_manager) 
					  WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\') THEN (\''.$userAccess->is_instructor.'\' = grp.is_instructor) ELSE 1=1 END) ';
						if($lastid != $enabledCount)
					$classQuery .= ' AND ';
				}
				
				if($value == 'hire_date'){
					$perHireDate = $userAccess->hire_date;
					
					/*$classQuery .= '(CASE WHEN grp.hire_search_mode = \'ste_con_hdt_btw\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') between UNIX_TIMESTAMP(grp.hire_start) AND UNIX_TIMESTAMP(grp.hire_end)
                                 					  WHEN grp.hire_search_mode = \'ste_con_hdt_ltn\' THEN  UNIX_TIMESTAMP(\'' .$perHireDate. '\') <= UNIX_TIMESTAMP(grp.hire_start)
                                 					  WHEN grp.hire_search_mode = \'ste_con_hdt_gtn\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') >= UNIX_TIMESTAMP(grp.hire_start) ELSE 1=1 END) ';
					*/
					$classQuery .= '(IF((select att.id from slt_group_attributes att where att.group_id = grp.id)>0,
					  (select att.id from slt_group_attributes att where att.group_id = grp.id and (
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') >= DATE_FORMAT(att.on_or_after_start_date,\'%Y-%m-%d\') ) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') <= DATE_FORMAT(att.on_or_before_start_date,\'%Y-%m-%d\')) or
					  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') between DATE_FORMAT(att.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(att.between_end_date,\'%Y-%m-%d\'))
					  )) >0
					  ,1=1)) ';
					
					
					if($lastid != $enabledCount)
						$classQuery .= ' AND ';
				}
				
				$lastid++;
			}
				$classQuery .= ' ) and ((grp.removed_users IS NULL) OR (FIND_IN_SET('.$userId.',grp.removed_users) <= 0))), 
				((SELECT COUNT(1) FROM slt_user_access_mapping um WHERE um.entity_id = cls.id AND um.entity_type=\'cre_sys_obt_cls\')=0)))';
			//$classQuery .= ' AND ';
			//$classQuery .= '((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and (grp_map.group_type=0 or grp_map.group_id is null))>0)';
				$classQuery .= '))';
				//$classQuery .= ' AND ';
				//	$classQuery .= 'if((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and grp_map.id is not null)>0,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=cls.id and gmap.group_type=0 and gmap.entity_type = \'cre_sys_obt_cls\' order by mro asc limit 1),1=1)';
			}
		}else{
			if($enabledCount > 0){
				$classQuery = ' AND ';
				$classQuery .= '(if(FIND_IN_SET('.$userId.',catacs.added_users)>0 OR umap.user_id = '.$userId.',1=1,';
				foreach($enabledValues as $key => $value){
					//$closeBracket = ($lastid == $enabledCount) ? '))':'';
					if($value == 'state')
						$userValue = $userAccess->country.'-'.$userAccess->$value;
					else
						$userValue = $userAccess->$value;
	
					if($value != 'role' && $value != 'hire_date'){
	
						if($value == 'job_role'){
							$resultjobrole = explode(',',$userValue);
							$classQuery .= ' (if(catacs.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (catacs.'.$value.' is null or catacs.'.$value.'=\'\'';
							
							/* $classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
							(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\', (grp.'.$value.' is null or grp.'.$value.'=\'\''; */
							foreach ($resultjobrole as $job_role){
								expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
								$classQuery .= 'OR (FIND_IN_SET(\''.$job_role.'\',catacs.'.$value.'))';
							}
			$classQuery .= ')))';
							if($lastid != $enabledCount)
								$classQuery .= ' AND ';
						}else{
	
							$classQuery .= ' (if(catacs.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(catacs.'.$value.' is null or catacs.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',catacs.'.$value.'))))) ';
							
							/* $classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
							(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and if(grp.'.$value.' = \'All\',\''.$userValue .'\'!=\'\',(grp.'.$value.' is null or grp.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',grp.'.$value.')))))>0, 1=1)'; */
							if($lastid != $enabledCount)
							$classQuery .= ' AND ';
						}
					}
	
	
					if($value == 'role'){
						$isManager = empty($userAccess->is_manager) ? 'N' : $userAccess->is_manager;
						$classQuery .= '(CASE WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'Y\') THEN (\''.$userAccess->is_manager.'\' = catacs.is_manager OR \''.$userAccess->is_instructor.'\' = catacs.is_instructor) 
						WHEN (catacs.is_manager=\'Y\' AND catacs.is_instructor=\'N\') THEN (\''.$userAccess->is_manager.'\' = catacs.is_manager) 
					  WHEN (catacs.is_manager=\'N\' AND catacs.is_instructor=\'Y\') THEN (\''.$userAccess->is_instructor.'\' = catacs.is_instructor) ELSE 1=1 END) ';
						if($lastid != $enabledCount)
							$classQuery .= ' AND ';
					}
					
					if($value == 'hire_date'){
						$perHireDate = $userAccess->hire_date;
						/* $classQuery .= '(CASE WHEN catacs.hire_search_mode = \'ste_con_hdt_btw\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') between UNIX_TIMESTAMP(catacs.hire_start) AND UNIX_TIMESTAMP(catacs.hire_end)
                                 					  WHEN catacs.hire_search_mode = \'ste_con_hdt_ltn\' THEN  UNIX_TIMESTAMP(\'' .$perHireDate. '\') <= UNIX_TIMESTAMP(catacs.hire_start)
                                 					  WHEN catacs.hire_search_mode = \'ste_con_hdt_gtn\' THEN UNIX_TIMESTAMP(\'' .$perHireDate. '\') >= UNIX_TIMESTAMP(catacs.hire_start) ELSE 1=1 END) '; */
						
						$classQuery .= '(IF((select att.id from slt_group_attributes att where att.group_id = catacs.id)>0,
									  (select att.id from slt_group_attributes att where att.group_id = catacs.id and (
									  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') >= DATE_FORMAT(att.on_or_after_start_date,\'%Y-%m-%d\') ) or
									  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') <= DATE_FORMAT(att.on_or_before_start_date,\'%Y-%m-%d\')) or
									  (DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') between DATE_FORMAT(att.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(att.between_end_date,\'%Y-%m-%d\'))
									  )) >0
									  ,1=1)) ';
						
						if($lastid != $enabledCount)
							$classQuery .= ' AND ';
					}
					$lastid++;
				}
				$classQuery .= ' and ((catacs.removed_users IS NULL) OR (FIND_IN_SET('.$userId.',catacs.removed_users) <= 0))';
			//$classQuery .= ' AND ';
				//$classQuery .= '((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and (grp_map.group_type=0 or grp_map.group_id is null))>0)';
				$classQuery .= 'AND( (group_map.group_type = 0) OR (group_map.group_id IS NULL AND umap.id IS NULL) )
												AND (IF((catacs.is_admin = 1 AND catacs.userslist IS NOT NULL AND catacs.userslist != \'\'),FIND_IN_SET('.$userId.',catacs.userslist)>0,1=1))))';
				//$classQuery .= ' AND ';
			//	$classQuery .= 'if((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and grp_map.id is not null)>0,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=cls.id and gmap.group_type=0 and gmap.entity_type = \'cre_sys_obt_cls\' order by mro asc limit 1),1=1)';
		}
		}
		
			   /* Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712 
			    * To change the course level catalog search api query if non-access user tries to  access the class if set start date or enrdate or country or location type
			    **/
			   
			    global $catalog_reg;
				
				$startdate1 = getRequestValue('startdate');
				$enddate1 = getRequestValue('enddate');
				$locationtype1 = getRequestValue('location');
				$contrytype1 = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
				
				expDebug::dPrint('$startdate1-->'.$startdate1.' $enddate1-->'.$enddate1. ' $locationtype1-->'.$locationtype1.'$contrytype1-->'.$contrytype1);
				
				if((!empty($startdate1) || !empty($enddate1) || !empty($locationtype1) || !empty($contrytype1)) && $catalog_reg=="Course" && trim($_REQUEST['apiname']) == 'CatalogSearchAPI'){ 
					$classQuery='1=1)';
				 }

		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
		$selectCls = db_select('slt_course_template', 'crs');
		if($_REQUEST['apiname'] == 'CatalogSearchAPI'){
			$clstblName = 'cls';
			// Commented by Rajesh.N regarding the Ticket (0065267: Course is showing as closed in the catalog page).
			//$selectCls->leftJoin('slt_course_class', 'cls', '(cls.course_id=crs.id AND (cls.status IN  (\'lrn_cls_sts_atv\',\'lrn_cls_sts_dld\')) AND ('.$classQuery.')');
			//$selectCls->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\',\'lrn_cls_sts_dld\')))');
			$selectCls->leftJoin('slt_course_class', 'cls', '(cls.course_id=crs.id AND (cls.status IN  (\'lrn_cls_sts_atv\')) AND ('.$classQuery.')');
			$selectCls->leftJoin('slt_user_access_mapping','umap','(umap.parent_id=concat(crs.id, \'-\', IFNULL(cls.id,0)) AND umap.parent_type = \'learning\') and umap.access_type = 0  ');
			$selectCls->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\')))');
			$selectCls->leftJoin('slt_tag_entity','te','((clsRollup.id=te.entity_id and te.entity_type=\'Class\') OR (crs.id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}else{
			$clstblName = 'clsRollup';
			// Commented by Rajesh.N regarding the Ticket (0065267: Course is showing as closed in the catalog page).
			//$selectCls->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\',\'lrn_cls_sts_dld\')))');
			$selectCls->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status IN  (\'lrn_cls_sts_atv\')))');
			// Below change done by Vincent for #0066758 on July 23, 2016
			// TODO: This should be changed in future once the flat structure is implement for performance tuning
			//$selectCls->leftJoin('slt_group_mapping','group_map','((group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR (group_map.entity_id=clsRollup.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
			$selectCls->leftJoin('slt_user_access_mapping','umap','((umap.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) or (umap.parent_id=concat(crs.id, \'-\', 0))) AND umap.parent_type = \'learning\') and umap.access_type = 0 and umap.user_id = '.$userId.' ');
			$selectCls->leftJoin('slt_group_mapping','group_map','(group_map.parent_id=concat(crs.id, \'-\', IFNULL(clsRollup.id,0)) AND group_map.parent_type = \'learning\') and group_map.group_type = 0 AND \'cre_sec_sts_atv\' = (SELECT status FROM slt_groups where id = group_map.group_id)');
			$selectCls->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) '.$classQuery );
			$selectCls->leftJoin('slt_tag_entity','te','((clsRollup.id=te.entity_id and te.entity_type=\'Class\') OR (crs.id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}
		
		/*$selectCls->leftJoin('slt_catalog_access','catacs','((catacs.entity_id=crs.id AND catacs.entity_type = \'cre_sys_obt_crs\') OR (catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'))');
		 $selectCls->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type IN (\'cre_sys_obt_crs\',\'cre_sys_obt_cls\')');
		$selectCls->leftJoin('slt_profile_list_items','splt_mro','splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\'');*/

		//	$selectCls->leftJoin('slt_group_mapping','group_map','((group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 0');
		//$selectCls->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');


		$lang = isset($_GET['lg_type'])?$_GET['lg_type']:'';
		$contrytype = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
		$locationtype = getRequestValue('location');
		expDebug::dPrint('lang-->'.$lang.' country-->'.$contrytype. ' location-->'.$locationtype);
		/*$tag = getRequestValue('tag');
		if(!empty($tag)){
			$selectCls->leftJoin('slt_tag_entity','te','((clsRollup.id=te.entity_id and te.entity_type=\'Class\') OR (crs.id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}*/
		$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
		$enrollIncluded = 0;
		$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
		if((($mroType) && $userId) || $sortByMandatory){
			$enrollIncluded = 1;
			$selectCls->leftJoin('slt_enrollment','enroll','enroll.class_id = '.$clstblName.'.id and enroll.user_id = \''.$userId.'\' and enroll.reg_status IN(\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\')');
		}


		$startdate = getRequestValue('startdate');
		$enddate = getRequestValue('enddate');
		$sortby = isset($_GET['sortby']) ? $_GET['sortby']:'';
		if(!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype) || ($sortby == 'ClassStartDate') ){
		 
		     /* Added/changed by ganeshbabuv on Feb 13th 2016 for Unable to filter the ILT trainings in Learner Catalog if ILT class has more than one sessions #0072082 */		
			//$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=clsRollup.id and ses.id = (select id from slt_course_class_session where class_id=clsRollup.id limit 1)');
			$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=clsRollup.id');
			
			/* Added/changed by ganeshbabuv on Feb 13th 2016 for  date, country and location filters issues #0071712 */
			if(!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype)){ 
     
			    $timeZone = date_default_timezone(false);
			    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
			    $sessionTZ = new DateTimeZone($defaultTimezone);
			    $toDay = new DateTime(null,$sessionTZ) ;
			    $toDayDT = $toDay->format('Y-m-d H:i');
			 
				$cls_aliase='clsRollup';				 
				
				$selectCls->leftJoin('slt_course_class_session','ses_past','ses_past.class_id='.$cls_aliase.'.id and ses_past.course_id='.$cls_aliase.'.course_id 
		   		 and CONVERT_TZ(CONCAT(DATE_FORMAT(ses_past.start_date, _utf8\'%Y-%m-%d\'),\' \',ses_past.start_time),(select pro_past.attr2 from slt_profile_list_items pro_past where pro_past.code= ses_past.timezone),\''.$defaultTimezone.'\')<=\''.$toDayDT.'\'');
			   
				$selectCls->where('(((group_map.id is null or group_map.id=\'\') and (catacs.id is null or catacs.id=\'\')) or (group_map.id!=\'\' and catacs.id!=\'\'))');
				$selectCls->where('ses_past.id is null or ses_past.id =\'\'');  
			}
			
		}
		if(!empty($contrytype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = '.$clstblName.'.location_id');
			$selectCls->leftJoin('slt_country','con','con.country_code=loc.country');
		}
		if(!empty($locationtype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = '.$clstblName.'.location_id');
		}



		$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
		if(isset($_REQUEST['apiname'])){
			$ratingtype = $_REQUEST['rating_type'];
		}

		if($_REQUEST['apiname'] == 'CatalogSearchAPI'){
			$selectCls->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
			$selectCls->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=cls.currency_type');
		$selectCls->addField('crs','id', 'cls_id'); // Course id from course_template table for course level registration
		$selectCls->addField('crs','code', 'cls_code');
		$selectCls->addField('crs','title', 'cls_title');
		$selectCls->addField('crs','short_description', 'cls_short_description');
		$selectCls->addField('cls','price', 'price');
		$selectCls->addField('cls','currency_type', 'currency_type');
		$selectCls->addField('cls','registration_end_on', 'registration_end_on');
		$selectCls->addField('crs','created_on', 'created_on');
		$selectCls->addField('crs','published_on', 'published_on');
		$selectCls->addField('splt_currency','attr1', 'currency_code');
		$selectCls->addField('splt_currency','attr2', 'currency_symbol');
		$selectCls->addField('splt_currency','code', 'currency_long_code');
		//$selectCls->addField('td','tagname', 'tagname');
		//if($GLOBALS["user"]->uid != 1){
		//$selectCls->addExpression('group_map.mro', 'mro_id');
		$selectCls->addExpression('if(umap.mro is not null, umap.mro, group_map.mro )', 'mro_id');
		//}
		//$selectCls->addExpression('null', 'mro');
		$selectCls->addExpression('cls.lang_code', 'language_code');
		$selectCls->addExpression('splt_language.name', 'language');
		
		if(isset($_REQUEST['mobile_request'])){
			$selectCls->addExpression('null', 'additional_info');
			$selectCls->addExpression('null', 'addn_catalog_show');
		}
		
		
		if($sortby == 'ClassStartDate'){
			$selectCls->addExpression('ses.start_date', 'start_date');
		}
		else{
			$selectCls->addExpression('null', 'start_date');
		}
		$selectCls->addExpression('cls.location_id', 'location_id');
		$selectCls->addExpression('cls.id', 'crs_id');
		$selectCls->addExpression('cls.id', 'cls_count');
		$selectCls->addExpression('cls.delivery_type', 'delivery_type_code');
		$selectCls->addExpression('null', 'delivery_type_name');
		$selectCls->addExpression('cls.custom4', 'export_compliance');
		$selectCls->addExpression('null', 'expires_in_value');
		$selectCls->addExpression('null', 'expires_in_unit');
		$selectCls->addExpression('null', 'prm_created_on');
		$selectCls->addExpression('\'Course\'', 'object_type');
		$selectCls->addExpression('crs.is_compliance', 'is_compliance');
		if($enrollIncluded){
			$selectCls->addExpression('enroll.mandatory', 'mandatory');
		}
		if((!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype))) {
			$selectCls->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =cls.id and nl.entity_type = \'cre_sys_obt_cls\'');
			$selectCls->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type = \'Class\'');
			$selectCls->addExpression('CEIL(sum(vote.value)/sum(if((vote.value=null or vote.value=\'\'),0,1)))', 'avgvote'); // Changed By Rajeshwar D for #72138 on 14 Feb 2017
		}else {
			$selectCls->addExpression('null', 'avgvote');
		}
		$selectCls->addExpression('if(if(umap.mro is not null, umap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(umap.mro is not null, umap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
		$selectCls->addExpression('(select GROUP_CONCAT(tagsub.tagname) from
	    			(select cls.course_id as crsid,
	    			GROUP_CONCAT(td.tagname) as tagname
	    			from slt_course_class cls
	    			left join slt_tag_entity te on ((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))
	    			left join slt_tagdefn td ON te.tagid= td.id
	    			group by cls.id) tagsub
	    			where tagsub.crsid =  cls_id)','tagname');
		
		}else{
			$selectCls->addField('crs','id', 'cls_id'); // Course id from course_template table for course level registration
			$selectCls->addField('crs','code', 'cls_code');
			$selectCls->addField('crs','title', 'cls_title');
			$selectCls->addField('crs','short_description', 'cls_short_description');		
			//$selectCls->addField('td','tagname', 'tagname');
			//$selectCls->addExpression('group_map.mro', 'mro_id');
			$selectCls->addExpression('if(umap.mro is not null, umap.mro, group_map.mro )', 'mro_id');
			$selectCls->addField('clsRollup','price', 'price');
			$selectCls->addField('clsRollup','currency_type', 'currency_type');
			$selectCls->addField('clsRollup','registration_end_on', 'registration_end_on');
			$selectCls->addField('crs','created_on', 'created_on');
			$selectCls->addField('crs','published_on', 'published_on');
			$selectCls->addExpression('clsRollup.id', 'crs_id');
			if($sortby == 'ClassStartDate'){
				$selectCls->addExpression('ses.start_date', 'start_date');
			}
			else{
				$selectCls->addExpression('null', 'start_date');
			}
			$selectCls->addExpression('clsRollup.custom4', 'export_compliance');
			
			
			//$selectCls->addExpression('cls.location_id', 'location_id');
			$selectCls->addExpression('null', 'expires_in_value');
			$selectCls->addExpression('null', 'expires_in_unit');
			$selectCls->addExpression('null', 'prm_created_on');
			$selectCls->addExpression('\'Course\'', 'object_type');
			$selectCls->addExpression('crs.is_compliance', 'is_compliance');
			if($enrollIncluded){
				$selectCls->addExpression('enroll.mandatory', 'mandatory');
			}
			if((!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype))) {
				$selectCls->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id ='.$clstblName.'.id and nl.entity_type = \'cre_sys_obt_cls\'');
				$selectCls->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type = \'Class\'');
				$selectCls->addExpression('CEIL(sum(vote.value)/sum(if((vote.value=null  or vote.value=\'\'),0,1)))', 'avgvote'); // Changed By Rajeshwar D for #71808 on 14 Feb 2017
			}else {
				$selectCls->addExpression('null', 'avgvote');
			}
			$selectCls->addExpression('if(if(umap.mro is not null, umap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(umap.mro is not null, umap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
			$selectCls->addExpression('(select GROUP_CONCAT(tagsub.tagname) from
	    			(select cls.course_id as crsid,
	    			GROUP_CONCAT(td.tagname) as tagname
	    			from slt_course_class cls
	    			left join slt_tag_entity te on ((cls.id=te.entity_id and te.entity_type=\'Class\') OR (cls.course_id=te.entity_id and te.entity_type=\'Course\'))
	    			left join slt_tagdefn td ON te.tagid= td.id
	    			group by cls.id) tagsub
	    			where tagsub.crsid = cls_id)','tagname');
		}
		
		return $selectCls;
	}catch(Exception $ex) {
		watchdog_exception('catalogCourseSearchQuery', $ex);
		expertusErrorThrow($ex);
	}
}
function catalogCourseSearchQueryCron($userId='') {
	try{
		if(empty($userId))
			$userId  = getIdOfLoggedInUser();
		$userAccess  = getUserAccessPriviledges($userId);
		$enabledValues = getEnabledAttrForVisibility();
		$closeBracket='';
		$lastid = 1;
		$enabledCount = count($enabledValues);
		if($enabledCount > 0){
			//$selectStmt->where('if((FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0),1=1,('.$aliasCatalogAccess.'.removed_users IS NULL) OR (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0)');
			//	$classQuery .= 'if(select (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.added_users)>0 from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and grp.is_admin IS NULL and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\'),1=1,('.$aliasCatalogAccess.'.removed_users IS NULL) OR (FIND_IN_SET('.$userId.','.$aliasCatalogAccess.'.removed_users) <= 0)';

			$classQuery = 'if(((select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and (FIND_IN_SET('.$userId.',grp.added_users)>0))>0
			 OR (select count(1) from slt_user_access_mapping umap where umap.entity_id = cls.id and umap.entity_type = \'cre_sys_obt_cls\' and umap.user_id= '.$userId.' and umap.access_type=0)),1=1,(';
			foreach($enabledValues as $key => $value){
				//$closeBracket = ($lastid == $enabledCount) ? '))':'';
				if($value == 'state')
					$userValue = $userAccess->country.'-'.$userAccess->$value;
				else
					$userValue = $userAccess->$value;

				if($value != 'role' && $value != 'hire_date'){
					if($value == 'job_role'){
						$resultjobrole = explode(',',$userValue);
						$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
						(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and grp.'.$value.' is null or grp.'.$value.'=\'\'';
						foreach ($resultjobrole as $job_role){
							expDebug::dPrint('$JOB_ROLE------>'.print_r($job_role,1),4);
							$classQuery .= 'OR (FIND_IN_SET(\''.$job_role.'\',grp.'.$value.'))';
						}
						$classQuery .= ')>0, 1=1)';
						$classQuery .= ' AND ';
					}else{
						$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
						(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and grp.'.$value.' is null or grp.'.$value.'=\'\' or (FIND_IN_SET(\''.$userValue.'\',grp.'.$value.')))>0, 1=1)';
						//if($lastid != $enabledCount)
						$classQuery .= ' AND ';
					}
				}


				if($value == 'role'){
					$isManager = empty($userAccess->is_manager) ? 'N' : $userAccess->is_manager;
					$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
					(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and grp.is_manager is null or grp.is_manager=\'\' or grp.is_manager = \''.$userAccess->is_manager.'\' or \''.$userAccess->is_manager.'\'=\'Y\')>0, 1=1)';
					$classQuery .= ' AND ';
					$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
					(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and grp.is_instructor is null or grp.is_instructor=\'\' or grp.is_instructor = \''.$userAccess->is_instructor.'\' or \''.$userAccess->is_instructor.'\'=\'N\')>0, 1=1)';
					$classQuery .= ' AND ';
				}
				
				if($value == 'hire_date'){
					$perHireDate = $userAccess->hire_date;
					$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
					(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\'  LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id where map.entity_id=cls.id and map.entity_type=\'cre_sys_obt_cls\' and (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\'' .$perHireDate. '\',\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)))>0, 1=1)';							
					if($lastid != $enabledCount)
						$classQuery .= ' AND ';
				}
				$lastid++;
			}
			$classQuery .= 'if((select count(1) from slt_group_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')>0,
			(select count(1) from slt_group_mapping map LEFT OUTER JOIN slt_groups grp ON grp.id=map.group_id and (grp.is_admin =0) and grp.status =\'cre_sec_sts_atv\' where map.entity_id=cls.id 
			and map.entity_type=\'cre_sys_obt_cls\' and ((grp.removed_users IS NULL) OR (FIND_IN_SET('.$userId.',grp.removed_users) <= 0))), 
			(select count(1) from slt_user_access_mapping where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\')=0))';
			//$classQuery .= ' AND ';
			//$classQuery .= '((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and (grp_map.group_type=0 or grp_map.group_id is null))>0)';
			$classQuery .= ')))';
			//$classQuery .= ' AND ';
			//	$classQuery .= 'if((select count(1) from slt_group_mapping grp_map where entity_id=cls.id and entity_type=\'cre_sys_obt_cls\' and grp_map.id is not null)>0,group_map.id = (select gmap.id from slt_group_mapping gmap where gmap.entity_id=cls.id and gmap.group_type=0 and gmap.entity_type = \'cre_sys_obt_cls\' order by mro asc limit 1),1=1)';
		}
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;
		$selectCls = db_select('slt_course_template', 'crs');
		$selectCls->leftJoin('slt_course_class', 'cls', '(cls.course_id=crs.id AND (cls.status = \'lrn_cls_sts_atv\') AND (cls.dedicated_class_flag = \'N\' or cls.dedicated_class_flag is null AND ('.$classQuery.')');
		$selectCls->leftJoin('slt_course_class', 'clsRollup', '(clsRollup.course_id=crs.id AND (clsRollup.status = \'lrn_cls_sts_atv\') AND (clsRollup.dedicated_class_flag = \'N\' or clsRollup.dedicated_class_flag is null))');
		$selectCls->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
		$selectCls->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=cls.currency_type');


		/*$selectCls->leftJoin('slt_catalog_access','catacs','((catacs.entity_id=crs.id AND catacs.entity_type = \'cre_sys_obt_crs\') OR (catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'))');
		 $selectCls->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type IN (\'cre_sys_obt_crs\',\'cre_sys_obt_cls\')');
		$selectCls->leftJoin('slt_profile_list_items','splt_mro','splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\'');*/

		//	$selectCls->leftJoin('slt_group_mapping','group_map','((group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\') OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\')) and group_map.group_type = 0');
		//$selectCls->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');


		$lang = isset($_GET['lg_type'])?$_GET['lg_type']:'';
		$contrytype = isset($_GET['cy_type']) ? $_GET['cy_type']:'';
		$locationtype = isset($_GET['location']) ? $_GET['location']:'';
		expDebug::dPrint('lang-->'.$lang.' country-->'.$contrytype. ' location-->'.$locationtype);
		$tag = isset($_GET['tag']) ? $_GET['tag']:'';
		if(!empty($tag)){
			$selectCls->leftJoin('slt_tag_entity','te','((clsRollup.id=te.entity_id and te.entity_type=\'Class\') OR (crs.id=te.entity_id and te.entity_type=\'Course\'))');
			$selectCls->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}
		$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
		$enrollIncluded = 0;
		$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
		if((($mroType) && $userId) || $sortByMandatory){
			$enrollIncluded = 1;
			$selectCls->leftJoin('slt_enrollment','enroll','enroll.class_id = cls.id and enroll.user_id = \''.$userId.'\' and enroll.reg_status IN(\'lrn_crs_reg_cnf\',\'lrn_crs_reg_wtl\')');
		}
		$startdate = isset($_GET['startdate']) ? $_GET['startdate']:'';
		$enddate = isset($_GET['enddate']) ? $_GET['enddate']:'';
		$sortby = isset($_GET['sortby']) ? $_GET['sortby']:'';
		if(!empty($startdate) || !empty($enddate) || !empty($locationtype) || !empty($contrytype) || ($sortby == 'ClassStartDate') ){
			$selectCls->leftJoin('slt_course_class_session','ses','ses.class_id=clsRollup.id and ses.id = (select id from slt_course_class_session where class_id=clsRollup.id limit 1)');
		}
		if(!empty($contrytype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
			$selectCls->leftJoin('slt_country','con','con.country_code=loc.country');
		}
		if(!empty($locationtype) ){
			$selectCls->leftJoin('slt_location','loc','loc.id = cls.location_id');
		}
		$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
		if(isset($_REQUEST['apiname'])){
			$ratingtype = $_REQUEST['rating_type'];
		}

		if((!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) || (!empty($_REQUEST['apiname']) && module_exists('exp_sp_fivestar'))) {
			$selectCls->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =cls.id and nl.entity_type = \'cre_sys_obt_cls\'');
			$selectCls->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type = \'Class\'');
		}
		$selectCls->addField('crs','id', 'cls_id'); // Course id from course_template table for course level registration
		$selectCls->addField('crs','code', 'cls_code');
		$selectCls->addField('crs','title', 'cls_title');
		$selectCls->addField('crs','short_description', 'cls_short_description');
		$selectCls->addField('cls','price', 'price');
		$selectCls->addField('cls','currency_type', 'currency_type');
		$selectCls->addField('cls','registration_end_on', 'registration_end_on');
		$selectCls->addField('crs','created_on', 'created_on');
		$selectCls->addField('crs','published_on', 'published_on');
		$selectCls->addField('splt_currency','attr1', 'currency_code');
		$selectCls->addField('splt_currency','attr2', 'currency_symbol');
		$selectCls->addField('splt_currency','code', 'currency_long_code');
		//if($GLOBALS["user"]->uid != 1){
		//$selectCls->addExpression('group_map.mro', 'mro_id');
		$selectCls->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
		//}
		//$selectCls->addExpression('null', 'mro');
		$selectCls->addExpression('cls.lang_code', 'language_code');
		$selectCls->addExpression('splt_language.name', 'language');
		//$selectCls->addExpression('GROUP_CONCAT(person.id)','userids');
		if($sortby == 'ClassStartDate'){
			$selectCls->addExpression('ses.start_date', 'start_date');
		}
		else{
			$selectCls->addExpression('null', 'start_date');
		}
		$selectCls->addExpression('cls.location_id', 'location_id');
		//$selectCls->addExpression('cls.location_id', 'location_id');
		$selectCls->addExpression('cls.id', 'crs_id');
		$selectCls->addExpression('cls.id', 'cls_count');
		$selectCls->addExpression('cls.delivery_type', 'delivery_type_code');
		$selectCls->addExpression('null', 'delivery_type_name');
		$selectCls->addExpression('cls.custom4', 'export_compliance');
		$selectCls->addExpression('null', 'expires_in_value');
		$selectCls->addExpression('null', 'expires_in_unit');
		$selectCls->addExpression('null', 'prm_created_on');
		$selectCls->addExpression('\'Course\'', 'object_type');
		$selectCls->addExpression('crs.is_compliance', 'is_compliance');
		if($enrollIncluded){
			$selectCls->addExpression('enroll.mandatory', 'mandatory');
		}
		if((!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) || (!empty($_REQUEST['apiname']) && module_exists('exp_sp_fivestar'))) {
			$selectCls->addExpression('avg(vote.value)', 'avgvote');
		}else {
			$selectCls->addExpression('null', 'avgvote');
		}
		$selectCls->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
		return $selectCls;
	}catch(Exception $ex) {
		watchdog_exception('catalogCourseSearchQuery', $ex);
		expertusErrorThrow($ex);
	}
}
function catalogTPSearchQuery($userId='',$queryFrom = '',$skipAccess=''){
	try{
	global $catalog_reg;
	if(empty($userId))
    $userId  = getIdOfLoggedInUser();
    $userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;

    $selectTp = db_select('slt_program', 'prm');
    //$selectTp->leftJoin('slt_profile_list_items','splt_language','splt_language.code=prm.lang_code');
   // $selectTp->leftJoin('slt_group_mapping','group_map','group_map.entity_id=prm.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 0');
   // $selectTp->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
    /* $selectTp->leftJoin('slt_catalog_access','catacs','catacs.entity_id=prm.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
    $selectTp->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
    $selectTp->leftJoin('slt_profile_list_items','splt_mro','splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\''); */
    $selectTp->leftJoin('slt_tag_entity','te','((prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
    $selectTp->leftJoin('slt_tagdefn','td','te.tagid= td.id');
  /*  $tag = getRequestValue('tag');
    if(!empty($tag)){
    	$selectTp->leftJoin('slt_tag_entity','te','((prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
    	$selectTp->leftJoin('slt_tagdefn','td','te.tagid= td.id');
    }*/
	$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
	$enrollIncluded = 0;
	$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
	if((($mroType) && $userId) || $sortByMandatory){
	    $enrollIncluded = 1;
	    // 'lrn_tpm_ovr_inc' condition added by Vincent on 05 Dec, 2013 for #0026273
    	$selectTp->leftJoin('slt_master_enrollment','masenr','masenr.program_id = prm.id and masenr.user_id = \''.$userId.'\' and masenr.overall_status NOT IN(\'lrn_tpm_ovr_rsc\',\'lrn_tpm_ovr_rsv\',\'lrn_tpm_ovr_cln\',\'lrn_tpm_ovr_inc\')');
    }


    if( $_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['cls_status'] == 'Incomplete'){
    	$selectTp->leftJoin('slt_master_enrollment','masenr','masenr.program_id = prm.id');
    	$selectTp->condition('masenr.user_id',$userId);
    	$selectTp->condition('masenr.overall_status','lrn_tpm_ovr_inc');

    	if($_REQUEST['compliance_mandatory'] == 1)
    	{
    		$db_or = db_or();
    		$db_or->condition('masenr.mandatory', 'Y');
    		$selectTp->condition($db_or);
    	}
    }

	$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
	if(isset($_REQUEST['apiname'])){
		$ratingtype = $_REQUEST['rating_type'];
	}

	if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) {
		$selectTp->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =prm.id and nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
    	$selectTp->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
	}
	
	if ($queryFrom == 'access_filter') {
		$selectTp->addField('prm','id', 'cls_id');
		return $selectTp;
	}
	
	if($_REQUEST['apiname'] == 'CatalogSearchAPI'  || ($queryFrom != 'catalogquery')){
		$selectTp->leftJoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=prm.object_type');
    $selectTp->leftJoin('slt_profile_list_items','splt_language','splt_language.code=prm.lang_code');
    $selectTp->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=prm.currency_type');
    $selectTp->addField('prm','id', 'cls_id');
    $selectTp->addField('prm','code', 'cls_code');
    $selectTp->addField('prm','title', 'cls_title');
    $selectTp->addField('prm','short_desc', 'cls_short_description');
    $selectTp->addField('prm','price', 'price');
    $selectTp->addField('prm','currency_type', 'currency_type');
    $selectTp->addField('prm','end_date', 'registration_end_on');
    $selectTp->addField('prm','created_on', 'created_on');
    $selectTp->addField('prm','published_on', 'published_on');
    $selectTp->addField('splt_currency','attr1', 'currency_code');
    $selectTp->addField('splt_currency','attr2', 'currency_symbol');
    $selectTp->addField('splt_currency','code', 'currency_long_code');
	    //$selectTp->addField('group_map','mro', 'mro_id');
    if($skipAccess !=1){
		$selectTp->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
	}
    $selectTp->addField('prm','lang_code', 'language_code');
    $selectTp->addField('splt_language','name', 'language');
   // $selectTp->addField('td','tagname', 'tagname');
    $selectTp->addExpression('null', 'start_date');
    $selectTp->addExpression('null', 'location_id');
    $selectTp->addExpression('null', 'crs_id');
    if($catalog_reg == 'Course' && arg(1) != 'order') {
      $selectTp->addExpression('null', 'cls_count');
    }
    $selectTp->addExpression('null', 'delivery_type_code');
    $selectTp->addExpression('splt_deliverytype.name', 'delivery_type_name');
    $selectTp->addExpression('null', 'export_compliance');
    $selectTp->addExpression('prm.expires_in_value', 'expires_in_value');
    $selectTp->addExpression('prm.expires_in_unit', 'expires_in_unit');
    $selectTp->addExpression('prm.created_on', 'prm_created_on');
    $selectTp->addExpression('prm.object_type', 'object_type');
    $selectTp->addExpression('null', 'is_compliance');
	if($enrollIncluded){
      $selectTp->addExpression('masenr.mandatory', 'mandatory');
    }
    $selectTp->addExpression('null', 'avgvote');
	    if($skipAccess !=1){
		$selectTp->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
	}
	$selectTp->addExpression('(select tagsub.tagname from
	    	 (select prg.id as prgid,GROUP_CONCAT(td.tagname) as tagname
	    	 from slt_program prg
	    	 left join slt_tag_entity te on (prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\'))
	    	 left join slt_tagdefn td ON te.tagid= td.id
	    	 group by prg.id) tagsub
	    	 where (tagsub.prgid = prm.id))','tagname');
    if(isset($_REQUEST['mobile_request'])){
    	$selectTp->addField('prm','additional_info', 'additional_info');
    	$selectTp->addField('prm','addn_catalog_show', 'addn_catalog_show');
    }
	
	} else {
		$selectTp->addField('prm','id', 'cls_id');
    $selectTp->addField('prm','code', 'cls_code');
    $selectTp->addField('prm','title', 'cls_title');
    $selectTp->addField('prm','short_desc', 'cls_short_description');
    $selectTp->addField('prm','price', 'price');
    $selectTp->addField('prm','currency_type', 'currency_type');
    $selectTp->addField('prm','end_date', 'registration_end_on');
    $selectTp->addField('prm','created_on', 'created_on');
    $selectTp->addField('prm','published_on', 'published_on');
	    //$selectTp->addField('group_map','mro', 'mro_id');
    if($skipAccess !=1){
		$selectTp->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
	}
   // $selectTp->addField('td','tagname', 'tagname');
	$selectTp->addExpression('null', 'crs_id');
    $selectTp->addExpression('null', 'start_date');
    $selectTp->addExpression('null', 'export_compliance');
    $selectTp->addExpression('prm.expires_in_value', 'expires_in_value');
    $selectTp->addExpression('prm.expires_in_unit', 'expires_in_unit');
    $selectTp->addExpression('prm.created_on', 'prm_created_on');
    $selectTp->addExpression('prm.object_type', 'object_type');
    $selectTp->addExpression('null', 'is_compliance');
		if($enrollIncluded){
      $selectTp->addExpression('masenr.mandatory', 'mandatory');
    }
    $selectTp->addExpression('null', 'avgvote');
	    if($skipAccess !=1){
		$selectTp->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
	}
	$selectTp->addExpression('(select tagsub.tagname from
	    	 (select prg.id as prgid,GROUP_CONCAT(td.tagname) as tagname
	    	 from slt_program prg
	    	 left join slt_tag_entity te on (prg.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\'))
	    	 left join slt_tagdefn td ON te.tagid= td.id
	    	 group by prg.id) tagsub
	    	 where (tagsub.prgid = prm.id))','tagname');
	}
	
    return $selectTp;
	}catch(Exception $ex) {
		watchdog_exception('catalogTPSearchQuery', $ex);
		expertusErrorThrow($ex);
  	}
}

function catalogTPSearchQueryCron($userId=''){
	try{
		global $catalog_reg;
		if(empty($userId))
			$userId  = getIdOfLoggedInUser();
		$userId = ($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['userid']) ? $_REQUEST['userid'] :  $userId;

		$selectTp = db_select('slt_program', 'prm');
		$selectTp->leftJoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=prm.object_type');
		$selectTp->leftJoin('slt_profile_list_items','splt_language','splt_language.code=prm.lang_code');
		$selectTp->leftJoin('slt_profile_list_items','splt_currency','splt_currency.attr1=prm.currency_type');
		// $selectTp->leftJoin('slt_group_mapping','group_map','group_map.entity_id=prm.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\') and group_map.group_type = 0');
		// $selectTp->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
		/* $selectTp->leftJoin('slt_catalog_access','catacs','catacs.entity_id=prm.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
		 $selectTp->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$userId.'\') AND mro.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
		$selectTp->leftJoin('slt_profile_list_items','splt_mro','splt_mro.code = mro.mro_flag AND splt_mro.is_active=\'Y\''); */
		$tag = isset($_GET['tag']) ? $_GET['tag']:'';
		if(!empty($tag)){
			$selectTp->leftJoin('slt_tag_entity','te','((prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
			$selectTp->leftJoin('slt_tagdefn','td','te.tagid= td.id');
		}
		$mroType = isset($_GET['mro_type']) ? $_GET['mro_type']:'';
		$enrollIncluded = 0;
		$sortByMandatory =  ($_GET['sortby'] == 'Mandatory') ? 1 : 0;
		if((($mroType) && $userId) || $sortByMandatory){
			$enrollIncluded = 1;
			// 'lrn_tpm_ovr_inc' condition added by Vincent on 05 Dec, 2013 for #0026273
			$selectTp->leftJoin('slt_master_enrollment','masenr','masenr.program_id = prm.id and masenr.user_id = \''.$userId.'\' and masenr.overall_status NOT IN(\'lrn_tpm_ovr_rsc\',\'lrn_tpm_ovr_rsv\',\'lrn_tpm_ovr_cln\',\'lrn_tpm_ovr_inc\')');
		}
		$ratingtype = isset($_GET['rating_type']) ? $_GET['rating_type']:'';
		if(isset($_REQUEST['apiname'])){
			$ratingtype = $_REQUEST['rating_type'];
		}

		if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar) && !empty($ratingtype)) {
			$selectTp->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =prm.id and nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
			$selectTp->leftjoin('votingapi_cache', 'vote',	'vote.function = \'average\' and vote.entity_id =nl.node_id and vote.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
		}
		$selectTp->addField('prm','id', 'cls_id');
		$selectTp->addField('prm','code', 'cls_code');
		$selectTp->addField('prm','title', 'cls_title');
		$selectTp->addField('prm','short_desc', 'cls_short_description');
		$selectTp->addField('prm','price', 'price');
		$selectTp->addField('prm','currency_type', 'currency_type');
		$selectTp->addField('prm','end_date', 'registration_end_on');
		$selectTp->addField('prm','created_on', 'created_on');
		$selectTp->addField('prm','published_on', 'published_on');
		$selectTp->addField('splt_currency','attr1', 'currency_code');
		$selectTp->addField('splt_currency','attr2', 'currency_symbol');
		$selectTp->addField('splt_currency','code', 'currency_long_code');
		// if($GLOBALS["user"]->uid != 1){
		//$selectTp->addField('group_map','mro', 'mro_id');
		$selectTp->addExpression('if(usrmap.mro is not null, usrmap.mro, group_map.mro )', 'mro_id');
		// }
		//$selectTp->addField('splt_mro','name', 'mro');
		$selectTp->addField('prm','lang_code', 'language_code');
		$selectTp->addField('splt_language','name', 'language');
		//$selectTp->addExpression('GROUP_CONCAT(person.id)','userids');
		$selectTp->addExpression('null', 'start_date');
		$selectTp->addExpression('null', 'location_id');
		$selectTp->addExpression('null', 'crs_id');
		if($catalog_reg == 'Course' && arg(1) != 'order') {
			$selectTp->addExpression('null', 'cls_count');
		}
		$selectTp->addExpression('null', 'delivery_type_code');
		$selectTp->addExpression('splt_deliverytype.name', 'delivery_type_name');
		$selectTp->addExpression('null', 'export_compliance');
		$selectTp->addExpression('prm.expires_in_value', 'expires_in_value');
		$selectTp->addExpression('prm.expires_in_unit', 'expires_in_unit');
		$selectTp->addExpression('prm.created_on', 'prm_created_on');
		$selectTp->addExpression('prm.object_type', 'object_type');
		$selectTp->addExpression('null', 'is_compliance');
		if($enrollIncluded){
			$selectTp->addExpression('masenr.mandatory', 'mandatory');
		}
		$selectTp->addExpression('null', 'avgvote');
		$selectTp->addExpression('if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_opt\',3,if(if(usrmap.mro is not null, usrmap.mro, group_map.mro ) = \'cre_sys_inv_man\',1,2))', 'ascen');
		//if(group_map.mro = 'cre_sys_inv_opt',3,if(group_map.mro = 'cre_sys_inv_man',1,2)) as ascen
		//expDebug::dPrintDBAPI('$selectTp', $selectTp);
		return $selectTp;
	}catch(Exception $ex) {
		watchdog_exception('catalogTPSearchQuery', $ex);
		expertusErrorThrow($ex);
	}
}



//Add to cart when commerce module is enabled
function callAddToCart($action,$nodeId,$classId,$courseId,$waitList =  0,$isAPI = '',$user_id =''){
  $txn = db_transaction();
  try{
  global $user;
  if($isAPI != 'api'){
 		 	$vLMSUserId = getSltpersonUserId();
  	}else{
  		$uid = getDrupalIdOfUser($user_id);
  		$user->uid= $uid;
  		$vLMSUserId = $user_id;
  	}
  $setCart_msg='';
  expDebug::dPrint("callAddToCart action   ==>".$action , 4);
  if(!$vLMSUserId) {
      $setCart_msg = t('MSG027');
      $cartCount = '';
  }else{

        if($action == 'add'){
          $vCartId = uc_cart_get_id();
          $vNodeId = $nodeId;
          $data =  array('module', 'uc_product');
          $select = db_select('uc_cart_products', 'cart');
          $select -> addExpression('COUNT(cart.cart_item_id)', 'cnt');
          $select -> condition('cart.cart_id',$vCartId);
          $select -> condition('cart.nid',$vNodeId);
          $result = $select -> execute() -> fetchField();
          $vProductCount = $result;
          if( $vProductCount == 0 ){
            $result = conflictCheck($vLMSUserId, $courseId, $classId,'',false);
          	expDebug::dPrint('$result cartCheckIns :'.print_r($result ,true), 4);
            expDebug::dPrint('  $result->enrollmentstatus = ' . $result['enrollmentstatus'] , 4);
            if($result['enrollmentstatus'] =='0'  ){
              $setCart_msg = t('MSG247');
            }else if($result['enrollmentstatus'] =='-1'  ){
              $setCart_msg = t('ERR047');
            }else if($result['enrollmentstatus'] =='-2'  ){
              $setCart_msg = t('ERR048');
            }else if($result['enrollmentstatus'] =='-3'  ){
              //$setCart_msg = '<a id="set_shoppingCart_Waitlist" href=\'javascript:void(0);\' onclick=\'callShoppingCartDetails("LnrShoppingCartDetails","setShoppingCartWaitlist",'.$param.');\'>'.t('Click to Join Waitlist').'</a>';
              $setCart_msg = 'waitlisted';
            }else if($result['enrollmentstatus'] =='-4'  ){
              $setCart_msg = t('LBL542');
            }else if($result['enrollmentstatus'] =='-5'  ){
              $setCart_msg = t('ERR022');
            }else if($result['enrollmentstatus'] =='-6'  ){
              $setCart_msg = t('Invalid class.');
            }else if($result['enrollmentstatus'] =='-7'  ){
              // $setCart_msg = t('ERR057');
              $setCart_msg = t('ERR052'); // Already reserved #0038578 Message is changed for this Ticket
            }else if($result['enrollmentstatus'] =='-8'  ){
              $setCart_msg = t('ERR049');
            }else if($result['enrollmentstatus'] =='-10'  ){
              $setCart_msg = t('ERR075');
            }
            if(($result['enrollmentstatus'] == '0') || ($result['enrollmentstatus'] =='-3')){
	            if($isAPI != 'api'){
	              	$vCartId = uc_cart_get_id();
	            	}else{
	            		$vCartId = getDrupalIdOfUser($user_id);
	            	}
              $cart_add = uc_cart_add_item($vNodeId,  1, $data = NULL, $vCartId,TRUE,  TRUE,  TRUE);
              if(!$cart_add){
                $setCart_msg = t('ERR050');
              }
            }
          }
          else{
            $setCart_msg = t('ERR051');
          }
          $cartCount = getCartCount();
        }
    }
    if($isAPI != 'api'){
	    $cartReturn  = array(
	      'cart_msg'      => $setCart_msg,
	      'total_product' => $cartCount
	    );
	    expDebug::dPrint('  $cartReturn = ' . print_r($cartReturn, true) , 4);
	    echo drupal_json_output($cartReturn);
    }else{
    	return $setCart_msg;
    }
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('callAddToCart', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
}

//Add to cart TP when commerce module is enabled
function callAddToCartTP($action,$nodeId,$tpId,$courseId,$class_ids =0,$isAPI,$user_id){
	
 // $txn = db_transaction(); //Commented for dbStatementsExecution();
  try{
  global $user;

  $statements = array('statement' => true);
   if($isAPI != 'api'){
 		 	$vLMSUserId = getSltpersonUserId();
 		 	  $drupalUserId = $user->uid;

  	}else{
  		$uid = getDrupalIdOfUser($user_id);
  		$user->uid= $uid;
        $drupalUserId = $user->uid;
  		$vLMSUserId = $user_id;
  	}



  $setCart_msg='';
  expDebug::dPrint("callAddToCartTP action   ==>".$action , 4);
  if(!$vLMSUserId) {
      $setCart_msg = t('MSG027');
      $cartCount = '';
  }else{

    if($action == 'add'){
      $vCartId = uc_cart_get_id();
      $vNodeId = $nodeId;
      $data =  array('module', 'uc_product');
      $select = db_select('uc_cart_products', 'cart');
      $select -> addExpression('COUNT(cart.cart_item_id)', 'cnt');
      $select -> condition('cart.cart_id',$vCartId);
      $select -> condition('cart.nid',$vNodeId);
      $result = $select -> execute() -> fetchField();
      $vProductCount = $result;
      if( $vProductCount == 0 ){
      $entity_id = array();
      $crs_entity_id = array();
      $ins_conflict  = array();
  	  $listIds = explode(',',$class_ids);
  	  $getTPConflictCheck = trainingProgramMultiClassConflictCheck($listIds);
      if($getTPConflictCheck > 0){
           expDebug::dPrint(' return = ' . print_r($getTPConflictCheck, true) , 4);
           $entity_id[] = $getTPConflictCheck;
      }
      if($getTPConflictCheck == 0){
        for($i=0;$i<count($listIds);$i++){
          $conflictInstructor = true ;
          $conflictInstructorSession = 0;
         $getConflictCheck   = enrollmentsConflictCheck($vLMSUserId,$listIds[$i],'Class');
          $deliveryType = getClassDeliveryType($classId, 'Short');
          $Instr_check = is_instructor($userId);
          if(($deliveryType == 'ilt' || $deliveryType == 'vc') && $Instr_check == true){
          	// Confirm that the user not assigned this class to instruct for learners
          	$conflictInstructor = getClassInstructorConflict($vLMSUserId, $listIds[$i]);

          	// Confirm that the user not scheduled to some other class in same session time as instructor
          	$conflictInstructorSession = getSessionClassInstructorConflict($vLMSUserId, $listIds[$i]);
          }
      	  if($getConflictCheck > 0) {
            $entity_id[] = $listIds[$i];
      	  }elseif($conflictInstructor == false){
      	     $entity_id[] = $listIds[$i];
      	     $ins_conflict[] = "-10";
      	  }elseif($conflictInstructorSession > 0){
      	     $entity_id[] = $listIds[$i];
      	  }/* else{
      	    $courseConflict = courseConflictCheck($vLMSUserId, '', $listIds[$i], 'Class');
        	  if($courseConflict > 0) {
              $crs_entity_id[] = $listIds[$i];
        	  }
      	  } */
        }
      }
      if(count($ins_conflict) > 0){
        $title = getCourseTitle($entity_id[0]);
        $setCart_msg = t("ERR147")." - <b>".$title."</b>";
      }
      elseif(count($entity_id) > 0) {
      	$title = getCourseTitle($entity_id[0]);
        $setCart_msg = t("You have a conflicting class assignment under")." - <b>".$title."</b>";
      }
      elseif(count($crs_entity_id) > 0){
        $title = getCourseTitle($crs_entity_id[0]);
        $setCart_msg = t("ERR049")." - <b>".$title."</b>";
      }
      else{
          $vCartId = uc_cart_get_id();
          $cart_add = uc_cart_add_item($vNodeId,  1, $data = NULL, $vCartId,TRUE,  TRUE,  TRUE,$statements);
          if(!$cart_add){
            $setCart_msg = t('ERR050');
          }
          else{
            //update the cart table
            //$transaction = db_transaction();
            try{
              if($class_ids){
                $updateCart = db_update('uc_cart_products');
                $updateCart->fields(array(
                                    'class_ids' => $class_ids
                                    ));
                $updateCart->condition('nid', $nodeId, '=');
                $updateCart->condition('cart_id', $drupalUserId, '=');
                //$updateCart->execute();
                if(!empty($statements)) {
                	$statements[] = $updateCart;
                } else {
                	$updateCart->execute();
                }
              }
            }catch (Exception $e) {
            	//if(empty($statements))
                // $transaction->rollback();
                 throw $e;
                }
            $setCart_msg = t('MSG247');
          }
        }
      }
      else{
        $setCart_msg = t('ERR051');
      }
      $cartCount = getCartCount();
    }

   }
  $cartReturn  = array(
   	'cart_msg'      => $setCart_msg,
   	'total_product' => $cartCount
  );

  expDebug::dPrint('$cartReturn$cartReturn$cartReturn$cartReturn'.print_r($cartReturn,true),5);

 
  dbStatementsExecution($statements);
  if($_GET['apiname'] == 'addTPToCartAPI' || $_REQUEST['apiname'] == 'AddItemsToShoppingCartAPI'){

  	 
    return $cartReturn;

  }
  echo drupal_json_output($cartReturn);
  }catch (Exception $ex) {
   // $txn->rollback();
   //unset($txn);
    watchdog_exception('callAddToCartTP', $ex);
    expertusErrorThrow($ex);
  }
 // unset($txn);
}
function getUserClassCmpStatus($userID,$classID,$courseID,$enrollID){
	try{
		$select = db_select('slt_enrollment', 'enroll');
		$select->addExpression('COUNT(1)','comp_count');
		$select->condition('enroll.comp_status','lrn_crs_cmp_cmp','=');
		$select->condition('enroll.id', $enrollID);
		$select->condition('enroll.course_id', $courseID);
		$select->condition('enroll.class_id', $classID);
		$select->condition('enroll.user_id', $userID);
		expDebug::dPrintDBAPI('getUserClassCmpStatus = ', $select);
		$result = $select->execute()->fetchField();
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getUserClassEnrStatus', $ex);
		expertusErrorThrow($ex);
	}
}
function getUserPreAssCmpStatus($userID,$enrId,$classID,$obj_type){
	try{
		$select = db_select('slt_survey_mapping', 'survey');
		$select->join('slt_survey', 'sur', 'sur.id = survey.survey_id');
		$select->join('slt_enrollment','enr','enr.class_id = survey.object_id');
		$select->addExpression('COUNT(survey.id)', 'count');
		$select->condition('survey.object_id', $classID,'=');
		$select->condition('enr.id', $enrId,'=');
		$select->condition('enr.user_id', $userID,'=');
		$select->condition(db_or()->
				condition('enr.pre_status','lrn_crs_cmp_cmp','!=')->
				condition('enr.pre_status','', 'is null')
		);
		$select->condition('survey.object_type', $obj_type,'=');
		$select->condition('sur.type', 'sry_det_typ_ass', '=');
		$select->condition('survey.pre_status', 1, '=');
		expDebug::dPrintDBAPI('getUserPreAssCmpStatus $select', $select);
		$count = $select->execute()->fetchField();
		return $count;
	}catch (Exception $ex) {
		watchdog_exception('getUserPreAssCmpStatus', $ex);
		expertusErrorThrow($ex);
	}
}
function registeredToLaunch($userID,$classID,$courseID,$enrollID,$delType,$ajaxCall='',$regMsg='',$callFrom = ''){
	try{
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
			include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
			//$content_count = getContentCount($courseID,$classID);
			expDebug::dPrint('ENROLL ID----->'.print_r($enrollID,true),4);
			$pre_assessment_status = getUserPreAssCmpStatus($userID,$enrollID,$classID,'cre_sys_obt_cls');
			expDebug::dPrint('$pre_assessment_status---->'.print_r($pre_assessment_status,true),4);
			$sid = $userID."-".$courseID."-".$classID;
			$launchInfo_arr = getLaunchDetail($enrollID,'',$sid);
			$IsLaunch_flag = false;
			$content_flag =true;
			$single_video_flag = false;
			$launch_response = new stdClass();
			$showAttempts = false;
			$lessonCnt = 0;
			$progress = 0;
			$IsLaunch_flag_for_content_player = 0;
			if(arg(9))
				$lessonID=arg(9);
			foreach ($launchInfo_arr as $launchContentInfo){
				If($launchContentInfo['IsLaunchable']>=1)
					$IsLaunch_flag = true;
				if ($ajaxCall && $launchContentInfo['ID'] == $lessonID) {
					$launch_response->triggering_lesson_details = $launchContentInfo;
					$content_id = $launchContentInfo['ContentId'];
				}
				$lessonCnt++;
			}
			$html_out = '';
			$pre_msg = '';
			//$pre_msg_end = '';
			$launch_multi_content ="$('#lnr-catalog-search').data('enrollment').launchMultiContent(".$enrollID.",this);";
			if($pre_assessment_status) {
				$pre_msg = "$('#lnr-catalog-search').data('lnrcatalogsearch').preAssMsgBox($enrollID);";
				//$pre_msg_end = '");';
			}
			$usrCompStaCnt = 0;
			if($ajaxCall){
				require_once(drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_lnrenrollment.inc');
				$launch_response->triggering_content_quiz_status = getConsolidatedQuizStatus($enrollID, $content_id, $launchInfo_arr);
				$usrCompStaCnt = getUserClassCmpStatus($userID,$classID,$courseID,$enrollID);
				if($usrCompStaCnt>=1)
					$regMsg = 'Completed';
				else
					$regMsg = 'Registered';
			}
			expDebug::dPrint('LAUNCH INFO----->'.print_r($launchInfo_arr,true),4);
// 			$multi_register = is_multiregistration_allowed($classID);
// 			expDebug::dPrint('multi_register----->'.print_r($multi_register,true),4);
			if(contentPlayerIsActive()) {
				if($callFrom == 'lrnplan_course_class_list'){
				$content_details = content_playlist_callback($enrollID,1);
				
				$prevCompStatus = $content_details['classDetails']['prev_comp_status'];
				$prevClassStatus = $content_details['classDetails']['prev_class_status'];
				$prevClassTitle = rawurlencode($content_details['classDetails']['prev_class_title']);
				$ClassTitle = rawurlencode($content_details['classDetails']['cls_title']);
				$titleLength = (strlen($ClassTitle) > 50) ? 50 : strlen($ClassTitle);
				//$title = rawurlencode(str_replace('/', '()|()',$launchInfo['Title']));
				expDebug::dPrint('LAUNCH $content_details1----->'.print_r($content_details,true),4);
				}
				if($IsLaunch_flag && $content_flag && $usrCompStaCnt<=0){
				$launchString = "{masterEnrollId : 0, enrollId: $enrollID, courseId:$courseID, classId:$classID, LaunchFrom :'CL', defaultContent: '', pagefrom:'catalog'}";
				$referer_url = $_SERVER['HTTP_REFERER'];
				if((isset($_SESSION['widget']['pass_url']) && ($_SESSION['widget']['pass_url'] === TRUE || $_SESSION['widget']['pass_url'] == 1) && (strpos($referer_url, 'page_number') === false && strpos($referer_url, 'click_id') === false && strpos($referer_url, 'row_number') === false)) && (arg(1) == 'catalog-search' && arg(2) == 'search')) {
					$html_out.=" id=\"launch".$enrollID.'"'." onclick=\"passUrlCommonCall(this.id);".'"';
				}else{	
					if($prevCompStatus == 'notallow' && $prevClassStatus != 'lrn_crs_cmp_cmp' && $callFrom == 'lrnplan_course_class_list'){
					$html_out.=" id=\"launch".$enrollID.'"'." onclick=\"$('body').data('learningcore').callMessageWindow('$ClassTitle',Drupal.t('You need to complete')+' ' + '$prevClassTitle' + ' ' + Drupal.t('Course'),  '$titleLength');".'"';
					}
					else{			
					$html_out.=" id=\"launch".$enrollID.'"'." onclick=\"$('#lnr-catalog-search').data('contentPlayer').playContent( $launchString);".'"';
					}
				}
				$html_out.=" class=\"action-btn\" >".t('LBL199');
				$showAttempts = true;
					$IsLaunch_flag_for_content_player =1;
			}else{
					$html_out.=" id=\"registerCls_".$classID.'"'."  class=\"action-btn-disable\" >".t($regMsg);
					$showAttempts = false;
				}
				$progress = convertProgressValue(getProgressFromEnrollmentId($enrollID));
			}else{
			//commented bcz of 53395: Need to change the functionality of register and launch.
			if($IsLaunch_flag && $content_flag && $usrCompStaCnt<=0){
						if($delType == 'lrn_cls_dty_wbt'){
							//$wbt_launch_details = getWBTLaunchDetail($enrollID,$delType);
							expDebug::dPrint('WBT LAUNCH INFO----->'.print_r($launchInfo_arr,true),4);
							if(count($launchInfo_arr)==1){
								//expDebug::dPrint('WBT LAUNCH DETAILS----->'.print_r($wbt_launch_details,true),4);
									foreach ($launchInfo_arr as $launchInfo){
										if($pre_assessment_status) {
											$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".$pre_msg."  alt=".'"'."$('#lnr-catalog-search').data('contentLaunch').launchWBTContent({'LaunchFrom':'CL','Id':'".$launchInfo["ID"]."','enrollId':'".$enrollID."','VersionId':'".$launchInfo["VersionId"]."','url1':'".$launchInfo['LearnerLaunchURL']."','courseId':'".$courseID."','classId':'".$classID."','url2':'".$launchInfo["PresenterLaunchURL"]."','ErrMsg':''	,'contentType':'".$launchInfo["ContentType"]."','Status':'".$launchInfo["Status"]."','LessonLocation':'".$launchInfo["LessonLocation"]."','launch_data':'".$launchInfo["LaunchData"]."','suspend_data':'".$launchInfo["SuspendData"]."','exit':'".$launchInfo["CmiExit"]."','AICC_SID':'".$launchInfo["AICC_SID"]."','MasteryScore':'".$launchInfo["masteryscore"]."'});".'"';
										}else{
											$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".'"'."$('#lnr-catalog-search').data('contentLaunch').launchWBTContent({'LaunchFrom':'CL','Id':'".$launchInfo["ID"]."','enrollId':'".$enrollID."','VersionId':'".$launchInfo["VersionId"]."','VersionNum':'".$launchInfo["VersionNum"]."','url1':'".$launchInfo['LearnerLaunchURL']."','courseId':'".$courseID."','classId':'".$classID."','url2':'".$launchInfo["PresenterLaunchURL"]."','ErrMsg':''	,'contentType':'".$launchInfo["ContentType"]."','Status':'".$launchInfo["Status"]."','LessonLocation':'".$launchInfo["LessonLocation"]."','launch_data':'".$launchInfo["LaunchData"]."','suspend_data':'".$launchInfo["SuspendData"]."','exit':'".$launchInfo["CmiExit"]."','AICC_SID':'".$launchInfo["AICC_SID"]."','MasteryScore':'".$launchInfo["masteryscore"]."'});".'"';
										}
										  break;
									}
							}else{
								expDebug::dPrint('WBT LAUNCH INFO----->'.print_r($launchInfo_arr,true),4);
								if($pre_assessment_status) {
									$html_out.=" id=\"launch".$enrollID.'"'." onclick=".$pre_msg." alt=".'"'.$launch_multi_content.'"'." data =\"[";
								}else{
									$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".'"'.$launch_multi_content.'"'." data =\"[";
								}
								  foreach ($launchInfo_arr as $launchInfo){
								 			$html_out.="[{'LaunchFrom':'CL','Id':'".$launchInfo["ID"]."','ContentId':'".$launchInfo["ContentId"]."','VersionId':'".$launchInfo["VersionId"]."','VersionNum':'".$launchInfo["VersionNum"]."','enrollId':'".$enrollID."','Title':'".addslashes(htmlspecialchars($launchInfo["Title"]))."','ContentTitle':'".addslashes(htmlspecialchars($launchInfo["Code"]))."','url1':'".$launchInfo["LearnerLaunchURL"]."','courseId':'".$courseID."','IsLaunchable':'".$launchInfo["IsLaunchable"]."','AttemptLeft':'".$launchInfo["AttemptLeft"]."','ValidityDays':'".$launchInfo["ValidityDays"]."','remDays':'".$launchInfo["remDays"]."','daysLeft':'".$launchInfo["daysLeft"]."','contValidateMsg':'".$launchInfo["contValidateMsg"]."','classId':'".$classID."','url2':'".$launchInfo["PresenterLaunchURL"]."','ErrMsg':'','contentType':'".$launchInfo["ContentType"]."','contentSubTypeCode':'".$launchInfo["ContentSubTypeCode"]."','launchType':'".$launchInfo["LaunchType"]."','ClsScore':'".$launchInfo["ClsScore"]."','LesScore':'". $launchInfo["LesScore"]."','ContScore':'".$launchInfo["ContScore"]."','Status':'".$launchInfo["Status"]."','ContentStatus':'".$launchInfo["ContentStatus"]."','Lessoncnt':'".$launchInfo["Lessoncnt"]."','LessonLocation':'".$launchInfo["LessonLocation"]."','launch_data':'".$launchInfo["LaunchData"]."','suspend_data':'".$launchInfo["SuspendData"]."','exit':'".$launchInfo["CmiExit"]."','AICC_SID':'".$launchInfo["AICC_SID"]."','MasteryScore':'".$launchInfo["masteryscore"]."','contentQuizStatus':'".$launchInfo["contentQuizStatus"]."','ContentCompletionStatus':'".$launchInfo["ContentCompletionStatus"]."'}]	,";
								  }
									$html_out.="]".'"';

							}
							$html_out.=" class=\"action-btn\" >".t('LBL199');
							$showAttempts = true;
					}elseif ($delType == 'lrn_cls_dty_vod'){
							expDebug::dPrint('VOD LAUNCH INFO----->'.print_r($launchInfo_arr,true),4);
							if(count($launchInfo_arr)==1){
								if($pre_assessment_status) {
									$pre_msg = "$('#lnr-catalog-search').data('lnrcatalogsearch').preAssMsgBox($enrollID);";
								}
								foreach ($launchInfo_arr as $launchInfo){
				 					$title = rawurlencode(str_replace('/', '()|()',$launchInfo['Title']));
				 					if($pre_assessment_status) {
				 						$callLaunchUrlFn = "?q=learning/ajax/play_video/";
				 					}else{
				 						$callLaunchUrlFn = "/?q=learning/nojs/play_video/";
				 					}
				 					$callLaunchUrlFn .= $title . "/";
				 					$callLaunchUrlFn .= rawurlencode($launchInfo['ContentSubTypeCode']). "/";
				 					$callLaunchUrlFn .= rawurlencode(str_replace('/', '()|()',$launchInfo['LearnerLaunchURL'])).'/';
				 					$callLaunchUrlFn .= "ME/";
				 					$callLaunchUrlFn .= $courseID."/";
				 					$callLaunchUrlFn .= $classID."/";
				 					$callLaunchUrlFn .= $launchInfo['ID']."/";
				 					$callLaunchUrlFn .= $launchInfo['VersionId']."/";
// 				 					$callLaunchUrlFn .= $launchInfo['VersionNum']."/";
				 					$callLaunchUrlFn .= $enrollID."/";
				 					$callLaunchUrlFn .= $launchInfo['StatusCode']."/";	// added for 60563: Video tracking and reporting feature - The status of the content is getting updated inconsistently
									if(isset($launchInfo['SuspendData'])) {
										$suspend_data = json_decode(rawurldecode($launchInfo['SuspendData']));
										// $callLaunchUrlFn .= $suspend_data->progress."/";
										// expDebug::dPrint('VOD LAUNCH INFO $suspend_data----->'.print_r(rawurlencode($launchInfo['SuspendData']),true),4);
										$callLaunchUrlFn .= rawurlencode($launchInfo['SuspendData']);
									}
									$classvideo =  ($launchInfo['IsLaunchable'])?  'action-btn' : ' ';
									$hrfVideo = ($launchInfo['IsLaunchable'])?  $callLaunchUrlFn: '';
									if($pre_assessment_status) {
										$single_video_flag = true;
										$pre_msg = "$('#lnr-catalog-search').data('lnrcatalogsearch').preAssMsgBox($enrollID,'sinVod');";
										$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".'"'.$pre_msg.'"'." class=".'"'.$classvideo.'"'." alt=".'"'.$hrfVideo.'"';
									}else{
										$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".'"'.$pre_msg.'"'." class=".'"'.$classvideo.' enroll-launch use-ajax ctools-modal-ctools-video-style"'." href=".'"'.$hrfVideo.'"';
									}
				 					break;
								}
									//h5pcustomize
									$html_out.= " subtype='".$launchInfo["subtype"]."' ";
									
				 					$html_out.=" >";
				 			}else{
				 				if($pre_assessment_status) {
				 					$html_out.=" id=\"launch".$enrollID.'"'." onclick=".$pre_msg."  alt=".'"'.$launch_multi_content.'"';
				 				}else{
									$html_out.=" id=\"launch".$enrollID.'"'."  onclick=".'"'.$launch_multi_content.'"';
				 				}
									$html_out.=" data =\"[";
									foreach ($launchInfo_arr as $launchInfo){
								 			$html_out.="[{'LaunchFrom':'CL','Id':'".$launchInfo["ID"]."','ContentId':'".$launchInfo["ContentId"]."','VersionId':'".$launchInfo["VersionId"]."','VersionNum':'".$launchInfo["VersionNum"]."','enrollId':'".$enrollID."','Title':'".addslashes(htmlspecialchars($launchInfo["Title"]))."','ContentTitle':'".addslashes(htmlspecialchars($launchInfo["Code"]))."','url1':'".$launchInfo["LearnerLaunchURL"]."','courseId':'".$courseID."','IsLaunchable':'".$launchInfo["IsLaunchable"]."','AttemptLeft':'".$launchInfo["AttemptLeft"]."','ValidityDays':'".$launchInfo["ValidityDays"]."','remDays':'".$launchInfo["remDays"]."','daysLeft':'".$launchInfo["daysLeft"]."','contValidateMsg':'".$launchInfo["contValidateMsg"]."','classId':'".$classID."','url2':'".$launchInfo["PresenterLaunchURL"]."','ErrMsg':'','contentType':'".$launchInfo["ContentType"]."','contentSubTypeCode':'".$launchInfo["ContentSubTypeCode"]."','launchType':'".$launchInfo["LaunchType"]."','ClsScore':'".$launchInfo["ClsScore"]."','LesScore':'". $launchInfo["LesScore"]."','ContScore':'".$launchInfo["ContScore"]."','Status':'".$launchInfo["Status"]."','ContentStatus':'".$launchInfo["ContentStatus"]."','Lessoncnt':'".$launchInfo["Lessoncnt"]."','LessonLocation':'".$launchInfo["LessonLocation"]."','launch_data':'".$launchInfo["LaunchData"]."','suspend_data':'".$launchInfo["SuspendData"]."','exit':'".$launchInfo["CmiExit"]."','AICC_SID':'".$launchInfo["AICC_SID"]."','MasteryScore':'".$launchInfo["masteryscore"]."','contentQuizStatus':'".$launchInfo["contentQuizStatus"]."','StatusCode':'".$launchInfo["statuscode"]."','ContentCompletionStatus':'".$launchInfo["ContentCompletionStatus"]."'}]	,";
								  }
									$html_out.="]".'"'." class=\"action-btn\" >";
						  }
				 				$html_out.= t('LBL199');
				 				$showAttempts = true;
				 }
			}else{
				$html_out.=" id=\"registerCls_".$classID.'"'."  class=\"action-btn-disable\" >".t($regMsg);
			}
			}
			if($single_video_flag && !contentPlayerIsActive()){
				$html_out.='<a href="'.$hrfVideo.'" id="dummylaunch'.$enrollID.'" class="action-btn enroll-launch use-ajax ctools-modal-ctools-video-style" style="display:none;">dummy</a>';
			}
			//$html_out =" id=\"registerCls_".$classID.'"'."  class=\"action-btn-disable\" >".t($regMsg);
			if($ajaxCall){
				if(contentPlayerIsActive())
					print $html_out.'*~*'.json_encode($launch_response).'*~*'.$lessonCnt.'*~*'.$progress.'*~*'.$IsLaunch_flag_for_content_player;
			else
				print $html_out.'*~*'.json_encode($launch_response).'*~*'.$lessonCnt;
			}
			else
				return array($html_out,$showAttempts);
	}catch (Exception $ex) {
		watchdog_exception('registeredToLaunch', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Register user to course class.
 */
function register_user_to_course_class($userId, $courseId, $classId, $waitlist, $isAdminSide, $response = true,$fromAPI=false,$regDate = '',$extraParam =  array(),$fromCloneEnrId='',$fromCloneClsId='',$triggerElementName='',$isMandatory=''){
  try{
  global $user;
  /*
   * To handle Sql Injection without session cookie (anonymous users)
  * Condition Added by : Vimal DM
  */
  	$sltPersonUserId = getIdOfLoggedInUser();
	if($fromAPI){
		$drupalUid = getDrupalIdOfUser($userId);
		$account = user_load($drupalUid);
		expDebug::dPrint(' register_user_to_course_class $user->roles api userid ' . $userId." ".print_r($account->roles, true) , 4);
		$vDrupalUserId = $account->uid;
		$vUserName = $account->name;
	}
	else
	{
		expDebug::dPrint(' register_user_to_course_class with server script name ' . print_r(basename($_SERVER['SCRIPT_NAME']), true) , 4);
		if(($sltPersonUserId!=$userId) && $isAdminSide != 'Y' && basename($_SERVER['SCRIPT_NAME']) != 'clicron.php' && basename($_SERVER['SCRIPT_NAME']) != 'index.php'){
			$userRoles =  $user->roles;
			if(isset($userRoles) && !in_array('administrator',$userRoles)){
				throw new Exception('Userid and Logged in user doesnot match.');
			}
		}
		$vDrupalUserId      = $user->uid;
		$vUserName          = $user->name;
		$userDetails 		= getPersonDetails($userId, array('full_name'));
		$userFullName		= $userDetails['full_name'];
	}
	if($userId){
	  $aReq = array('LMSUserId'=>$userId,'DrupalUserId'=>$vDrupalUserId,'UserName'=>$vUserName,
	        'CourseId'=>$courseId,'ClassId'=>$classId,'ForceReg'=>$waitlist,'RegDate' => $regDate,'ExtraParam' => $extraParam);

	  $aCatalogRegStatus = getCatalogRegistrationStatus($aReq, $isAdminSide, $fromAPI,$fromCloneEnrId,$fromCloneClsId,$triggerElementName,$sltPersonUserId,$isMandatory);
	  $cart_msg=$aCatalogRegStatus;
	  expDebug::dPrint('$cart_msg$cart_msg$cart_msg$cart_msg : '.$cart_msg , 4);
	  if($cart_msg =='Waitlist'){
	    $param = '{"classid" : "'.$cClassId.'","courseid" : "'.$cCourseId.'","orderid" : "'.$cOrderId.'","nid" : "'.$nodeid.'","userid" : "'.$lmsUserId.'"}';
	    $setCart_msg = t('Waitlisted');
	    $response = true;
	  }
	  else if($cart_msg =='InstructorConflict'){
	  	if ($isAdminSide == "Y") {
	  		$setCart_msg = t('ERR147');
	  	}
	  	else if(isset($_POST['isManager']) && $_POST['isManager'] == 'Y') {	//change by ayyappans for 28817: In team, the status message of the prerequisite class is showing as 'Assign'
	  		$setCart_msg = $userFullName.' '.t('ERR147');
	  	}
	  	else {
	      $setCart_msg = t('ERR075');
	  	}
	  }
	  else if($cart_msg =='AlreadyEnrolled'){
	    if ($isAdminSide == "Y" || (isset($_POST['isManager']) && $_POST['isManager'] == 'Y')) {
	  		$setCart_msg = t('ERR020');
	  	}
	  	else {
	      $setCart_msg = t('ERR047');
	  	}
	  }
	  else if($cart_msg =='Conflict'){
	  	if ($isAdminSide == "Y") {
	  		$setCart_msg = t('ERR191');
	  	}
	  	else if(isset($_POST['isManager']) && $_POST['isManager'] == 'Y') {
	  		$setCart_msg = $userFullName.' '.t('ERR191');
	  	}
	  	else {
	      $setCart_msg = t('ERR048');
	  	}
	  }else if($cart_msg =='CompletionDateConflict'){
	  	if ($isAdminSide == "Y") {
	  		$setCart_msg = t('MSG863');
	  	}
	  	else if(isset($_POST['isManager']) && $_POST['isManager'] == 'Y') {
	  		$setCart_msg = $userFullName.' '.t('MSG863').' '.t('MSG868');
	  	}
	  	else {
	      $setCart_msg = t('MSG862');
	  }
	  }
	  else if($cart_msg =='AlreadyReserved'){
	    $setCart_msg = t('ERR052');
	  }
	  else if($cart_msg =='Invalid'){
	    $setCart_msg = t('ERR133');
	  }
	  else if($cart_msg =='Full'){
	    $setCart_msg = t('ERR022');
	  }
	  else if($cart_msg =='InvalidUser'){
	    $setCart_msg = t('LBL542');
	  }
	  else if($cart_msg =='Reserved'){
	    $multiRegAllowed = is_multiregistration_allowed($classId);
	    if($multiRegAllowed == 0) {
	    	if($isAdminSide =='Nlnrsearch'){
		    	$select = db_select('slt_enrollment', 'enr');
		    	$select->addExpression('max(enr.id)','id');
		    	$select->condition('enr.class_id', $classId);
		    	$select->condition('enr.course_id', $courseId);
		    	$select->condition('enr.user_id', $userId);
		    	$select->condition('enr.reg_status' , array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
		    	// Below condition modified by Vincent on 05 Dec, 2013 for #0026273
		    	$select->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
		    	$select->range(0,1);
		    	$enrId = $select->execute()->fetchField();
		    	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
		    	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
		    	$sid = $userId."-".$courseId."-".$classId;
		    	$launchInfo_arr = getLaunchDetail($enrId,'',$sid);
		    	$pre_assessment_status = getUserPreAssCmpStatus($userId,$enrId,$classId,'cre_sys_obt_cls');//getPreAssessmentDetail($classId,'cre_sys_obt_cls');
		    	$del_type = getClassDeliveryType($classId,'Short');
		    	expDebug::dPrint(' DELEVERY TYPES = ' . print_r($del_type, true) , 4);
		    	if(contentPlayerIsActive()){
		    		$html_out ='';
		    		$launchString = "{masterEnrollId : 0, enrollId: $enrId, courseId:$courseId, classId:$classId, LaunchFrom :'CL', defaultContent: '', pagefrom:'catalog'}";		    			
		    		$html_out.=" id=\"launch".$enrId.'"'." onclick=\"$('#lnr-catalog-search').data('contentPlayer').playContent( $launchString);".'"';
		    		$html_out.=" class=\"action-btn\" >".t('LBL199');
		    		$setCart_msg = t('Registered').'~~'.$html_out.'~~'.$enrId.'~~'.$del_type.'~~'.$pre_assessment_status; //Your registration was successful
		    	}else
		    	$setCart_msg = t('Registered').'~~'.json_encode($launchInfo_arr).'~~'.$enrId.'~~'.$del_type.'~~'.$pre_assessment_status; //Your registration was successful
	    	}else{
	    		$setCart_msg = t('Registered');
	    	}
	    } else {
	      if($isAdminSide == 'Y') {
	        $setCart_msg = 'RecurringRegistration';
	      } else {
	        //$setCart_msg = t('MSG311').'<br/><br/>'.t('MSG312');
	      	if($isAdminSide =='Nlnrsearch'){
	      		$select = db_select('slt_enrollment', 'enr');
	      		$select->addExpression('max(enr.id)','id');
	      		$select->condition('enr.class_id', $classId);
	      		$select->condition('enr.course_id', $courseId);
	      		$select->condition('enr.user_id', $userId);
	      		$select->condition('enr.reg_status' , array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
	      		// Below condition modified by Vincent on 05 Dec, 2013 for #0026273
	      		$select->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
	      		$select->range(0,1);
	      		$enrId = $select->execute()->fetchField();
	      		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
	      		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrlearningplan/exp_sp_lnrlearningplan.inc');
	      		$sid = $userId."-".$courseId."-".$classId;
	      		$launchInfo_arr = getLaunchDetail($enrId,'',$sid);
	      		$pre_assessment_status = getUserPreAssCmpStatus($userId,$enrId,$classId,'cre_sys_obt_cls');//getPreAssessmentDetail($classId,'cre_sys_obt_cls');
	      		$del_type = getClassDeliveryType($classId,'Short');
	      		expDebug::dPrint(' DELEVERY TYPES = ' . print_r($del_type, true) , 4);
	      		$setCart_msg = t('Registered').'~~'.json_encode($launchInfo_arr).'~~'.$enrId.'~~'.$del_type.'~~'.$pre_assessment_status; //Your registration was successful
	      	}else{
	      		$setCart_msg ='<div class="cls-multiple-reg-msg">'.t('MSG311').'</div><div class="cls-reg-comp-msg">'.t('MSG312').'</div>';
	      	}
	      }
	    }
	  }
	  else if($cart_msg =='CourseConflict'){
	 	if ($isAdminSide == "Y") {
	  		$setCart_msg = t('ERR190');
	  	}
	  	else if(isset($_POST['isManager']) && $_POST['isManager'] == 'Y') {
	  		$setCart_msg = $userFullName.' '.t('ERR190');
	  	}
	  	else {
	      $setCart_msg = t('ERR049');
	  	}
	  }else if($cart_msg =='PastRegistration'){
	    $setCart_msg = t('ERR053'); //Registration is closed.
	  }else if($cart_msg =='Waitlisted') {
	    $setCart_msg = t('LBL190'); //waitListCheck($courseId, $classId);
	  }
	}
  if($response){
    if($userId) {
      if($isAdminSide == 'Y') {
        return $setCart_msg;
      }else{
        print $setCart_msg;
      }
    }else{
      print t('MSG027');
    }
  }
  if($fromAPI){
    $select = db_select('slt_enrollment', 'enr');
    $select->addExpression('max(enr.id)','id'); //#0031064
    $select->condition('enr.class_id', $classId);
    $select->condition('enr.course_id', $courseId);
    $select->condition('enr.user_id', $userId);
    $select->condition('enr.reg_status' , array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
     // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
    $select->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
    $select->range(0,1);
    $enrId = $select->execute()->fetchField();
    $stdObj=new stdClass();
    $stdObj->id = $enrId;
    $stdObj->userid = $userId;
    $stdObj->status = t($cart_msg);
    return $stdObj;
  }
  }catch (Exception $ex) {
    watchdog_exception('register_user_to_course_class', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
  syncSolrData('Enrollment');
  syncSolrData('User');
}

function getCatalogRegistrationStatus($pmReq, $isAdminSide = 'N', $fromAPI = false,$fromCloneEnrId='',$fromCloneClsId='',$triggerElementName='',$sltPersonUserId='',$isMandatory=''){
  
  try {
		//$dao = new AbstractDAO();
    $vDrupalUserId  = $pmReq['DrupalUserId'];
    $vLMSUserId     = $pmReq['LMSUserId'];
    $vCourseId  = $pmReq['CourseId'];
    $vClassId   = $pmReq['ClassId'];
    $vUserName  = $pmReq['UserName'];
    $vOrderId   = '';
    $regDate    = $pmReq['RegDate'];
    $extraParam = $pmReq['ExtraParam'];
    $vShoppingCartFlag='N';
    $vForceWaitingReg=$pmReq['ForceReg'];
    enrollmentStatusChangeUpdate($vLMSUserId, $vCourseId, $vClassId, $vLMSUserId);
    $vClassNotify = ($triggerElementName == 'clone_enrollment') ? false : true;
   
   
    $enrollmentStatus = enrollUserToClass($vLMSUserId, $vOrderId, $vUserName, $vCourseId, $vClassId, $vForceWaitingReg, $vLMSUserId,
                                                                         $vShoppingCartFlag, $vClassNotify,$isMandatory, $regDate, $isAdminSide,$extraParam, $fromAPI,$fromCloneEnrId,$fromCloneClsId);
    expDebug::dPrint(' $enrollmentStatus = ' . print_r($enrollmentStatus, true) , 4);
    $vRegStatus='';
    if($enrollmentStatus['id'] == '0'  ){
      $vRegStatus='Reserved';
    }else if($enrollmentStatus['id'] =='-1'  ){
      $vRegStatus='AlreadyEnrolled';
    }else if($enrollmentStatus['id'] =='-2'  ){
      $vRegStatus='Conflict';
    }else if($enrollmentStatus['id'] =='-3'  ){
      $vRegStatus ='Waitlist';
    }else if($enrollmentStatus['id'] =='-4'  ){
      $vRegStatus ='InvalidUser';
    }else if($enrollmentStatus['id'] =='-5'  ){
      $vRegStatus ='Full';
    }else if($enrollmentStatus['id'] =='-6'  ){
      $vRegStatus ='Invalid';
    }else if($enrollmentStatus['id'] =='-7'  ){
      $vRegStatus ='AlreadyReserved';
    }else if($enrollmentStatus['id'] =='-8'  ){
      $vRegStatus ='CourseConflict';
    }else if($enrollmentStatus['id'] =='-9'  ){
      $vRegStatus ='PastRegistration';
    }else if($enrollmentStatus['id'] =='-10'  ){
      $vRegStatus ='InstructorConflict';
    }else if($enrollmentStatus['id'] =='-11'  ){
      $vRegStatus ='CompletionDateConflict';
    }else if($enrollmentStatus['id'] =='-12'  ){
      $vRegStatus ='Course is expired for the user.';
    }else if($enrollmentStatus['id'] =='1'  ){
      $vRegStatus ='Waitlisted';
    }
    if($enrollmentStatus['orderId']){
      $vOrderId = $enrollmentStatus['orderId'];
      $loginID=empty($sltPersonUserId)? getIdOfLoggedInUser():$sltPersonUserId;
      insertOrderItem($vOrderId, $vCourseId, $vClassId, $vLMSUserId, null, '', $loginID, '', '', '', '', '');
    }
    return $vRegStatus;
  } catch (Exception $ex){
    watchdog_exception('getCatalogRegistrationStatus', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}

function enrollmentStatusChangeUpdate($xuserid, $xcourseid, $xclasid, $xupduser)  {

	//$txn = db_transaction();
  try{
  expDebug::dPrint(' enrollmentStatusChangeUpdate() called with' .
  								' $xuserid = ' . print_r($xuserid, true) .
                                ' $xcourseid = ' . print_r($xcourseid, true) .
                                ' $xclasid = ' . print_r($xclasid, true) .
                                ' $xupduser = ' . print_r($xupduser, true) , 4);
  $exists = enrollmentExistsCheck($xuserid, $xclasid, 'Class');
  expDebug::dPrint(' enrollmentExistsCheck() returned $exists = ' .
                               print_r($exists, true) , 4);
  if ($exists == 2) {
    // Select table to update
    $updateStmt = db_update('slt_enrollment');
    // Set conditions
    $updateStmt->condition('user_id', $xuserid);
    $updateStmt->condition('reg_status', 'lrn_crs_reg_rsv');
    $updateStmt->condition('course_id', $xcourseid);
    $updateStmt->condition('class_id', $xclasid);
    // Set new values of fields
    $updateStmt->fields(array(
                                  'reg_status' => 'lrn_crs_reg_rsc', //Update status to ReservedCancelled
                                  'comp_status' => null,
                                  'comp_date' => null,
                                  'reg_status_date' => now(),
                                  'updated_on' => now(),
                                  'updated_by' => $xupduser
                        ));
    expDebug::dPrintDBAPI(' $updateStmt = ' , $updateStmt);
    // Execute query
    $num_updated = $updateStmt->execute();
    expDebug::dPrint(' $num_updated = ' . print_r($num_updated, true) , 3);
  }
  }catch (Exception $ex) {
		//$txn->rollback();
		//unset($txn);
    watchdog_exception('enrollmentStatusChangeUpdate', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
	//unset($txn);
  syncSolrData('Enrollment');
}


/*
 * Please include functions which can be shared among all the modules which are under learning package.
 */

/*
 * getEnrollmentRegistrationStatus() - Get a person's current enrollment registration status.
 *                                   - Ignores records with cancelled statuses (lrn_crs_reg_can, lrn_crs_reg_rsc).
 */
function getEnrollmentRegistrationStatus ($personId, $classId) {

  try {
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $reg_status_condition_and = db_and();
  $reg_status_condition_and->condition('enr.user_id', $personId);
  $classIds = explode('-', $classId);
  $reg_status_condition_and->condition('enr.class_id', $classIds, 'IN');
  $reg_status_condition_or = db_or();
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_cnf');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_rsv');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_wtl');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_ppm');
  $reg_status_condition_and->condition($reg_status_condition_or);
  $selectStmt->condition($reg_status_condition_and);
 // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  $selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  // Select fields
  $selectStmt->addField('enr', 'reg_status', 'regStatus');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmtgetEnrollmentRegistrationStatus = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
//   expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getEnrollmentRegistrationStatus', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * enrollmentExistsCheck() - Determines whether or not the person is already enrolled to the class.
 */
function enrollmentExistsCheck ($personId, $objectId, $objectType) {

  if ($objectType != 'Class') {
    return 0;
  }

	//$txn = db_transaction();

  $regStatus = getEnrollmentRegistrationStatus($personId, $objectId);
  $multRegAllowed = is_multiregistration_allowed($objectId);
  
  $selectStmt = db_select('slt_enrollment', 'enr');


  $selectStmt->condition('enr.user_id', $personId);
  $classIds = explode('-', $objectId);
  $selectStmt->condition('enr.class_id', $classIds, 'IN');

  $selectStmt->condition('enr.reg_status', 'lrn_crs_reg_cnf');
  $selectStmt->isNull('enr.master_enrollment_id');
  $selectStmt->addField('enr', 'comp_status', 'regStatus');
  $selectStmt->orderBy('enr.id', 'DESC');
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmtgetEnrollmentRegistrationStatus sannn= ' , $selectStmt);
  // Execute query
  $compStatusresult = $selectStmt->execute()->fetchField();
  
  expDebug::dPrint(' $regStatus aaa= ' . print_r($result, true) , 4);
  $enrolledInp = array("lrn_crs_cmp_enr","lrn_crs_cmp_inp");
  if ($regStatus == null || $regStatus == '' || ($multRegAllowed &&  !in_array($compStatusresult, $enrolledInp))) {
    return 0; // Person is not already enrolled
  }
  else if ($regStatus == 'lrn_crs_reg_rsv') {

  expDebug::dPrint(' _REQUEST = ' . $_REQUEST['action']);
     // Class is already reserved by the person.
     // Below condition is added to allow the user to purchase class from mobile evn if the user is in cart page in web.
     if($_REQUEST['action'] != 'get')
 	 {
  		return 2;
  	}
  	else
  	{
 		 return 0;
  	}
  }
  else {
    return 1; // Person is already enrolled to the class.
  }

  try {
    // If comes here, then $multRegAllowedId was not found in $idInProfileSettings
    //throw new Exception('CheckWhetherClassWasReserved');
  }
  catch (Exception $ex) {
		//$txn->rollback();
		//unset($txn);
    if ($ex->getMessage() != null &&  $ex->getMessage() == 'CheckWhetherClassWasReserved') {
      //expDebug::dPrint('exp_sp_learning.inc : enrollmentExistsCheck() : In exception handler with $regStatus = ' . print_r($regStatus, true));
      if ($regStatus == 'lrn_crs_reg_rsv') {
        return 2; // Class is already reserved by the person.
      }
      else {
        return 1; // Person is already enrolled to the class.
      }
    }
    else {
      throw $ex; // Rethrow all other exceptions.
    }
  }
	//unset($txn);
}

/*
 *
 * courseConflictCheck() - Check whether the user has already enrolled for another class in the course. Checks also
 *                         whether multiple registrations are allowed, in which case there is no conflict.
 */
function courseConflictCheck($personId, $courseId, $objectId, $objectType, $iscompliance=0) {
	try {
		/* if ($objectType != 'Class') {
		 return 0; // No conflict
		}

		$txn = db_transaction();

  $otherClassesCount = getCountOtherRegisteredClassesOfCourse($personId, $courseId, $objectId,$iscompliance);
		expDebug::dPrint(' $otherClassesCount = ' . print_r($otherClassesCount, true) , 4);

		if ($otherClassesCount <= 0) {
		return 0; // No conflict
		} else {
		return 1; // Conflict
		} */

		return 0;

	}catch (Exception $ex) {
		watchdog_exception('courseConflictCheck', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getCountOtherRegisteredClassesOfCourse() fetches count of all other registered classes of a course by a person.
 */
function getCountOtherRegisteredClassesOfCourse($personId, $courseId, $classId,$iscompliance)
{
  try{
  if(empty($courseId)){
    $courseId = getCourseIdFromClassId($classId);
  }
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  if($iscompliance == 1){
  	$selectStmt->leftJoin('slt_course_template','crs', 'crs.id=enr.course_id');
  	$selectStmt->innerJoin('slt_notification_info','notify', 'notify.notification_code=\'compliance_expiry_remainder\' and enr.is_compliance = 1');
  	$selectStmt->where('if(enr.comp_status= \'lrn_crs_cmp_cmp\',
  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day), interval notify.notify_before day)),
  			(DATE_SUB(crs.validity_date, interval notify.notify_before-1 day)))) < 0),1=1)');
  }

  // Set conditions
  $selectStmt->condition('enr.user_id', $personId);
  $selectStmt->condition('enr.course_id', $courseId);
  $selectStmt->condition('enr.class_id', $classId, '!='); // should not be $classId
  $selectStmt->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
  // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  $selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  // Select fields and/or add expressions
  $selectStmt->addExpression('COUNT(*)', 'Count');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getCountOtherRegisteredClassesOfCourse', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * enrollmentsConflictCheck() - Checks whether the class ($objectId) timings overlap with the timings of the classes
 *                              to which the user has already enrolled ('lrn_crs_reg_cnf', 'lrn_crs_reg_rsv',
 *                              'lrn_crs_reg_wtl', 'lrn_crs_reg_ppm').
 *                              Returns first conflicting class id, else returns 0 when there is no conflict.
 */
function enrollmentsConflictCheck($personId, $objectId, $objectType) {
  if ($objectType != 'Class') {
    expDebug::dPrint(' Not a class', 4);
    return -1; // Not a class
  }
  // Datetime conflict, if any, will happen only for ilt or vc class
  $deliveryType = getClassDeliveryType($objectId, 'Short');
  expDebug::dPrint(' class $deliveryType = ' . print_r($deliveryType, true) , 4);
  if ($deliveryType != 'ilt' && $deliveryType != 'vc') {
    expDebug::dPrint(' Class is not ilt or vc - not conflict', 4);
    return 0; // Not Conflict
  }
  // Get the date, start_time and end_time of all the sessions in the class
  // The results are sorted by date ascending
  // Each session starts and ends the same day.
  $classSessionsDateTimeList = getClassSessionsDateTime($objectId);
  expDebug::dPrint(' $classSessionsDateTimeList = ' . print_r($classSessionsDateTimeList, true) , 4);
  $numClassSessions = count($classSessionsDateTimeList);

  // If this class does not have any sessions, there is no conflict
  // However, user is likely already prevented from registering to a ILT or VC class with no sessions in the UI.
  if ($numClassSessions <= 0) {
    expDebug::dPrint(' Class does not have any sessions - not conflict', 4);
    return 0; // Not Conflict
  }

  // Get all the classes to which the user is currently enrolled
  $enrolledClassesList = getUserEnrollments($personId);
  expDebug::dPrint(' $enrolledClassesList = ' . print_r($enrolledClassesList, true) ,4);

  // Check for session timing overlap
  $conflictStatus = 0;
  try {
    // Check conflict with each enrolled class
    foreach ($enrolledClassesList as $enrolledClass) {

      // if the registered class is same in traing plan
      if($enrolledClass['class_id'] == $objectId)
      {
        expDebug::dPrint(' if the registered class is in traing plan then no conflict', 4);
        continue; // There is no conflict if the registered class is in traing plan then no conflict
      }
      // If the enrolledClass is not ilt or vc, we do not have to check for datetime conflict
      $enrClassDeliveryType = getClassDeliveryType($enrolledClass['class_id'], 'Short');
      expDebug::dPrint(' $enrClassDeliveryType = ' . print_r($enrClassDeliveryType, true) , 4);
      if ($enrClassDeliveryType != 'ilt' && $enrClassDeliveryType != 'vc') {
        expDebug::dPrint(' enrolled Class is not ilt or vc - skip', 4);
        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class
      }
      // Get the date, start_time and end_time of all the sessions in the enrolledClass.
      // The results are sorted by date ascending.
      // Each session starts and ends the same day.
      $enrClassSessionsDateTimeList = getClassSessionsDateTime($enrolledClass['class_id']);
      expDebug::dPrint(' $enrClassSessionsDateTimeList = ' . print_r($enrClassSessionsDateTimeList, true) , 4);
      $numEnrClassSessions = count($enrClassSessionsDateTimeList);

      // If the enrolled class does not have any sessions, there is no datetime conflict with this class.
      // However, when admin alters (deletes all in particular) sessions of a class, the user enrollment for the class
      // is most likely automatically cancelled. Cancelled classes would not be returned by getUserEnrollments() above.
      if ($numEnrClassSessions <= 0) {
        expDebug::dPrint(' enrolled Class does not have sessions - skip', 4);
        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class
      }

      // If the enrolledClass's min date - max date are outside the this class's min date - max date, there is no
      // datetime conflict with this class.
      // If class min start date > enrClass max start date, there is no conflict.
      // If class max start date < enrClass min start date, there is no conflict
      if ($classSessionsDateTimeList[0]['date'] > $enrClassSessionsDateTimeList[$numEnrClassSessions - 1]['date'] ||
            $classSessionsDateTimeList[$numClassSessions - 1]['date'] < $enrClassSessionsDateTimeList[0]['date'] ) {
        expDebug::dPrint(' class min-max dates outside enr class min-max dates - skip', 4);
        continue; // There is no conflict with this enrolled class. Skip to the next enrolled class.
      }

      // Check for conflict for each classSession with every enrClassSession
      foreach ($classSessionsDateTimeList as $classSessionsDateTime) {
        foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime) {
          // If the classSession is on a different date than enrClassSession, there is no conflict
          if ($classSessionsDateTime['date'] != $enrClassSessionsDateTime['date']) {
            expDebug::dPrint(' enrClassSession date is different - skip', 4);
            continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
          }

          // classSession is on the same date as enrClassSession
          // If the classSession's start_time - end_time is outside enrClassSession's start_time - end_time, there is no
          // conflict
          // If classSession start_time >= enrClassSession end_time, there is no conflict.
          // If classSession end_time <= enrClassSession start_time, there is no conflict
          if ($classSessionsDateTime['start_time'] >= $enrClassSessionsDateTime['end_time'] ||
                $classSessionsDateTime['end_time'] <= $enrClassSessionsDateTime['start_time']) {
            expDebug::dPrint(' classSession\'s start_time - end_time is outside enrClassSession\'s start_time - end_time - skip', 4);
            continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
          }

          // A conflict is found. Save conflicting classId in $conflictStatus exit all loops
          $conflictStatus = $enrolledClass['class_id'];
          throw new Exception('Conflict');
        } // end foreach ($enrClassSessionsDateTimeList as $enrClassSessionsDateTime)
      } // end foreach ($classSessionsDateTimeList as $classSessionsDateTime)
    } // end foreach ($enrolledClassesList as $enrolledClass)
  } // end try
  catch (Exception $ex) {
    // Only handle Conflict exceptions
    if ($ex->getMessage() == null || $ex->getMessage() != 'Conflict') {
      throw $ex; // Rethrow all other exceptions.
    }
    expDebug::dPrint(' In exception handler with $conflictStatus = ' . print_r($conflictStatus, true) , 4);
  }

  expDebug::dPrint(' Return value:  $conflictStatus = ' . print_r($conflictStatus, true) , 4);
  return $conflictStatus;
	//unset($txn);
}

/*
* getSessionClassInstructorConflict() - Checks whether the class ($objectId) timings overlap with the timings of the classes
*                              to which the user has scheduled as Instructor
*                              Returns first conflicting class id, else returns 0 when there is no conflict.
*/
function getSessionClassInstructorConflict($personId, $objectId){
	// Get the date, start_time and end_time of all the sessions in the class
	// The results are sorted by date ascending
	// Each session starts and ends the same day.
	$classSessionsDateTimeList = getClassSessionsDateTime($objectId);
	expDebug::dPrint(' getSessionClassInstructorConflict====>>>> ' . print_r($classSessionsDateTimeList, true) , 4);
	$numClassSessions = count($classSessionsDateTimeList);

	// If this class does not have any sessions, there is no conflict
	// However, user is likely already prevented from registering to a ILT or VC class with no sessions in the UI.
	if ($numClassSessions <= 0) {
		expDebug::dPrint(' Class does not have any sessions - not conflict');
		return 0; // Not Conflict
	}

	// Get all the classes to which the user is scheduled as instructor
	$instructorScheduledClass = getInstructorScheduledSession($personId,'');
	expDebug::dPrint(' $scheduledClassesList = ' . print_r($instructorScheduledClass, true) ,4);
	// Check for session timing overlap
	$conflictStatus = 0;
	try {
		// Check conflict with each enrolled class
		foreach ($instructorScheduledClass as $id => $row) {
			$vSesTimeZone = $row->session_timezone;
			$stDate = $row->ses_start_date. " " .$row->start_time;
			$enDate = $row->ses_start_date. " " .$row->end_time;
			$vSysPHPTimezonename = date_default_timezone(FALSE);
			$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
			$ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
			$tmpStDate = explode(' ',date_format($ses_start_date,'Y-m-d H:i'));
			$tmpEnDate = explode(' ',date_format($ses_end_date,'Y-m-d H:i'));
			// Check for conflict for each classSession with every scheduledClassSession
			foreach ($classSessionsDateTimeList as $classSessionsDateTime) {
					// If the classSession is on a different date than enrClassSession, there is no conflict
					if ($classSessionsDateTime['date'] != $tmpStDate[0]) {
						expDebug::dPrint(' enrClassSession date is different - skip',4);
						continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
					}
					// classSession is on the same date as enrClassSession
					// If the classSession's start_time - end_time is outside enrClassSession's start_time - end_time, there is no
					// conflict
					// If classSession start_time >= enrClassSession end_time, there is no conflict.
					// If classSession end_time <= enrClassSession start_time, there is no conflict
					if ($classSessionsDateTime['start_time'] >= $tmpEnDate[1] ||
							$classSessionsDateTime['end_time'] <= $tmpStDate[1]) {
						expDebug::dPrint(' classSession\'s start_time - end_time is outside enrClassSession\'s start_time - end_time - skip',4);
						continue; // There is no conflict with this enrolled class session. Skip to the next enrolled class session.
					}
					expDebug::dPrint(' enrClassSession date is different - skip',4);
					// A conflict is found. Save conflicting classId in $conflictStatus exit all loops
					$conflictStatus = $objectId;
					throw new Exception('Conflict');
			} // foreach ($classSessionsDateTimeList as $classSessionsDateTime)
		} // end foreach ($instructorScheduledClass as $id => $row)
	} // end try
	catch (Exception $ex) {
		// Only handle Conflict exceptions
		if ($ex->getMessage() == null || $ex->getMessage() != 'Conflict') {
			throw $ex; // Rethrow all other exceptions.
		}
		expDebug::dPrint(' In exception handler with $conflictStatus = ' . print_r($conflictStatus, true) , 4);
	}
	expDebug::dPrint(' Return value:  $conflictStatus = ' . print_r($conflictStatus, true) , 4);
	return $conflictStatus;
}

/*
 * getUserEnrollments() - Gets the list of classes to which the user has already enrolled.
 *                        Gets only the classes in reg_status 'lrn_crs_reg_cnf', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl',
 *                        'lrn_crs_reg_ppm'.
 */
function getUserEnrollments($personId, $iltVC = false) {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $selectStmt->condition('enr.user_id', $personId);
  /*$selectStmt->condition(db_or()
                         ->condition('enr.reg_status', 'lrn_crs_reg_cnf')
                         ->condition('enr.reg_status', 'lrn_crs_reg_rsv')
                         ->condition('enr.reg_status', 'lrn_crs_reg_wtl')
                         ->condition('enr.reg_status', 'lrn_crs_reg_ppm'));*/
  if ($iltVC == true) {
  	$selectStmt->innerJoin('slt_course_class','cls', 'enr.class_id=cls.id and cls.delivery_type IN (\'lrn_cls_dty_ilt\',\'lrn_cls_dty_vcl\')');
//  	$selectStmt->condition('cls.delivery_type', array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'), 'IN');
  }
  $selectStmt->condition('enr.reg_status' ,array('lrn_crs_reg_cnf', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl', 'lrn_crs_reg_ppm'), 'IN');
  $selectStmt->condition('enr.comp_status' ,array('lrn_crs_cmp_nsw', 'lrn_crs_cmp_cmp'), 'NOT IN');
  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'class_id', 'class_id');
  $selectStmt->addField('enr', 'mandatory', 'mandatory');
  expDebug::dPrintDBAPI(' $selectStmt Object = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  $userEnrollments = $result->fetchAll(PDO::FETCH_ASSOC);
  expDebug::dPrint(' $userEnrollments = ' . print_r($userEnrollments, true) , 3);
  return $userEnrollments;
  }catch (Exception $ex) {
    watchdog_exception('getUserEnrollments', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getClassSessionsDateTime() - Returns start_date, start_time and end_time of all sessions of a class
 */
function getClassSessionsDateTime($classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'ses');

  // Set conditions
  $selectStmt->condition('ses.class_id', $classId);

  // Select fields and/or add expressions
  $selectStmt->addField('ses', 'start_date', 'date');
  $selectStmt->addField('ses', 'start_time', 'start_time');
  $selectStmt->addField('ses', 'end_time', 'end_time');
  $selectStmt->addExpression("DATE_FORMAT((ses.start_date),'%Y-%m-%d')", 'ses_start_date');
  $selectStmt->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
  $selectStmt->addField('splt_timezone','attr2', 'session_timezone');
  // Order results by start_date
  $selectStmt->orderBy('start_date', 'ASC');
  expDebug::dPrintDBAPI(' $selectStmt Object = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
//   expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  $sesssionsDateTime = $result->fetchAll(PDO::FETCH_ASSOC);
  expDebug::dPrint(' $sesssionsDateTime = ' . print_r($sesssionsDateTime, true) , 3);
  // Added by Vincent on Nov 08, 2013 for #0028593
  // Convert session date and time to system timezone from its respective timezone
  $sessionDetail = array();
  $cnt = 0;
	foreach($sesssionsDateTime as $sesssionsDt){
		$vSesTimeZone = $sesssionsDt['session_timezone'];
		$stDate = $sesssionsDt['ses_start_date']. " " .$sesssionsDt['start_time'];
		$enDate = $sesssionsDt['ses_start_date']. " " .$sesssionsDt['end_time'];
		$vSysPHPTimezonename = date_default_timezone(FALSE);
   	$ses_start_date = timeZoneConvert($stDate, $vSesTimeZone,  $vSysPHPTimezonename);
	  $ses_end_date = timeZoneConvert($enDate, $vSesTimeZone,  $vSysPHPTimezonename);
	  $tmpStDate = explode(' ',date_format($ses_start_date,'Y-m-d H:i'));
	  $tmpEnDate = explode(' ',date_format($ses_end_date,'Y-m-d H:i'));
	  $sessionDetail[$cnt]['date']			=$tmpStDate[0];
	  $sessionDetail[$cnt]['start_time']=$tmpStDate[1];
	  $sessionDetail[$cnt]['end_time']	=$tmpEnDate[1];
	  $cnt++;
	}
  expDebug::dPrint(' $sessionDetail = ' . print_r($sessionDetail, true) , 3);
  return $sessionDetail;
  }catch (Exception $ex) {
    watchdog_exception('getClassSessionsDateTime', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 *  waitListCheck() - Returns 0 if class max capacity would not be reached with the user registering for this class
 *                      or there is no max capacity restriction.
 *                    Returns -1 if class max capacity would be reached with the user registering for this class.
 *                    Else, returns the waitlist number of the new enrollment.
 */
function waitListCheck($courseId, $classId) {

	//$txn = db_transaction();
  try{
  $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
  $classEnrolledCount = getClassEnrollmentCountForRegistrationStatuses($courseId, $classId, $regStatusesList);
  expDebug::dPrint(' $classEnrolledCount ======== ' . print_r($classEnrolledCount, true) , 4);
  $regStatusesList = array('lrn_crs_reg_wtl');
  $classWaitListCount =  getClassEnrollmentCountForRegistrationStatuses($courseId, $classId, $regStatusesList);
  expDebug::dPrint(' $classWaitListCount ========== ' . print_r($classWaitListCount, true) , 4);
  $classMaxCapacity = getClassMaxCapacity($courseId, $classId);
  expDebug::dPrint(' $classMaxCapacity ========== ' . print_r($classMaxCapacity, true) , 4);
  if ($classMaxCapacity == null || $classMaxCapacity == '' || $classMaxCapacity <= 0) { // There is no max capacity restriction for this class
    return 0; // No Waitlist, max capacity not reached.
  }
  if ($classEnrolledCount >= $classMaxCapacity) {
    return ($classWaitListCount + 1); //Waitlist number for the new class.
  }
  if ($classEnrolledCount == $classMaxCapacity - 1) {
  	return -1; // No waitlist, max capacity reached.
  }
  return 0; // No Waitlist, max capacity not reached.
  }catch (Exception $ex) {
		//	$txn->rollback();
		//	unset($txn);
    watchdog_exception('waitListCheck', $ex);
		throw new Exception($ex);
	}
    expertusErrorThrow($ex);
  }

function waitListCheckProgram($courseId, $classId) {
  $txn = db_transaction();
  try{
  $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
  $classEnrolledCount = getClassEnrollmentCountForRegistrationStatuses($courseId, $classId, $regStatusesList);
  expDebug::dPrint(' $classEnrolledCount ======== ' . print_r($classEnrolledCount, true) , 4);

  $waitlistFlagStatusesList = array('lrn_crs_reg_wtl');
  $classWaitListCount =  getClassEnrollmentCountForWaitlistFlagStatuses($courseId, $classId, $waitlistFlagStatusesList);
  expDebug::dPrint(' $classWaitListCount ========== ' . print_r($classWaitListCount, true) , 4);

  $classMaxCapacity = getClassMaxCapacity($courseId, $classId);
  expDebug::dPrint(' $classMaxCapacity ========== ' . print_r($classMaxCapacity, true) , 4);

  if ($classMaxCapacity == null || $classMaxCapacity == '' || $classMaxCapacity <= 0) { // There is no max capacity restriction for this class
    return 0; // No Waitlist, max capacity not reached.
  }
  if ($classEnrolledCount >= $classMaxCapacity) {
    return ($classWaitListCount + 1); //Waitlist number for the new class.
  }

  if ($classEnrolledCount == $classMaxCapacity - 1) {
  	return -1; // No waitlist, max capacity reached.
  }
  return 0; // No Waitlist, max capacity not reached.
  }catch (Exception $ex) {
  	$txn->rollback();
  	unset($txn);
    watchdog_exception('waitListCheckProgram', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
}

/*
 * getClassMaxCapacity() -  Returns the max capacity of a class from slt_course_class_session table.
 */
function getClassMaxCapacity($courseId, $classId) {
  try {
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'ses');
  // Set conditions
  $selectStmt->condition('ses.course_id', $courseId);
  $selectStmt->condition('ses.class_id', $classId);
  // Select fields
  $selectStmt->addField('ses', 'capacity_max', 'class_max_capacity');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getClassMaxCapacity', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getClassWaitlistCapacity() -  Returns the waitlist capacity of a class from slt_course_class_session table.
 */
function getClassWaitlistCapacity($courseId, $classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'ses');
  // Set conditions
  if (!is_null($courseId)) {
    $selectStmt->condition('ses.course_id', $courseId);
  }
  $selectStmt->condition('ses.class_id', $classId);
  // Select fields
  $selectStmt->addField('ses', 'waitlist_count', 'class_waitlist_capacity');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getClassWaitlistCapacity', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getWaitListAvailable() - Checks whether any seat in a class is available for waitlist users to register.
 */
function getWaitListAvailable($courseId, $classId) {

  $txn = db_transaction();
  try{

  $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm');
  $totalEnrollments =  getClassEnrollmentCountForRegistrationStatuses($courseId, $classId, $regStatusesList);
  expDebug::dPrint(' $totalEnrollments = ' . print_r($totalEnrollments, true) , 4);

  $classMaxCapacity = getClassMaxCapacity($courseId, $classId);
  expDebug::dPrint(' $classMaxCapacity = ' . print_r($classMaxCapacity, true) , 4);

  if ($classMaxCapacity != 0 && $totalEnrollments != 0 && $classMaxCapacity == $totalEnrollments) {
    return 1; // All seats got occupied. No seats available for waitlisted users.
  }

  return 0; // Seats available for waitlisted users.
  }catch (Exception $ex) {
  	$txn->rollback();
  	unset($txn);
    watchdog_exception('getWaitListAvailable', $ex);
    expertusErrorThrow($ex);
  }
  unset($txn);
}

/*
 * getClassEnrollmentCountForRegistrationStatuses() - returns count of enrollment records for a class from slt_enrollment table
 *                                                           matching the registration statuses in $regStatusesList
 */
function getClassEnrollmentCountForRegistrationStatuses($courseId, $classId, $regStatusesList, $userId = '') {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  if ($courseId != null) {
    $selectStmt->condition('enr.course_id', $courseId);
  }
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.reg_status', $regStatusesList, 'IN');
  //Below condition added by Priya for 	#0032785
  if(!empty($userId))
  	$selectStmt->condition('enr.user_id', $userId, '!=');
  // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  $selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\',\'lrn_crs_cmp_nsw\',\'lrn_crs_cmp_inc\'),1=1)');
  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'id');
  $selectStmt->groupBy('enr.user_id');
  $selectStmt->groupBy('enr.reg_status');
  expDebug::dPrintDBAPI(' $selectStmt getClassEnrollmentCount= ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  $regStatusCount = $result->rowCount();
  expDebug::dPrint(' $result $compStatusCount = ' .$regStatusCount , 4);
  return $regStatusCount;
  }catch (Exception $ex) {
    watchdog_exception('getClassEnrollmentCountForRegistrationStatuses', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}

/*
 * getClassEnrollmentCountForComplitionStatuses() - returns count of enrollment records for a class from slt_enrollment table
 *                                                           matching the complition statuses in $regStatusesList
 */
function getClassEnrollmentCountForComplitionStatuses($classId, $compStatusList) {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.comp_status', $compStatusList, 'IN');

  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'id');
  $selectStmt->groupBy('enr.user_id');
  $selectStmt->groupBy('enr.comp_status');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  $compStatusCount = $result->rowCount();
  expDebug::dPrint(' $result $compStatusCount = ' .$compStatusCount , 4);
  return $compStatusCount;
  }catch (Exception $ex) {
    watchdog_exception('getClassEnrollmentCountForComplitionStatuses', $ex);
    expertusErrorThrow($ex);
  }
}


function getClassEnrollmentCountForWaitlistFlagStatuses($courseId, $classId, $waitlistFlagStatusesList) {
  try {
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  if ($courseId != null) {
    $selectStmt->condition('enr.course_id', $courseId);
  }
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.waitlist_flag', $waitlistFlagStatusesList, 'IN');
  // Select fields and/or add expressions
  $selectStmt->addExpression('COUNT(1)', 'Count');
  // Execute query
  $result = $selectStmt->execute();
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getClassEnrollmentCountForWaitlistFlagStatuses', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getClassDeliveryType() - Returns the long or short name of a class' delivery type as requested in $returnType
 */
function getClassDeliveryType($classId, $returnType) {
  try {
  // Select table
  $selectStmt = db_select('slt_course_class', 'cls');
  // Join other tables.
  $selectStmt->leftJoin('slt_profile_list_items', 'pli', 'cls.delivery_type = pli.code');
  // Set conditions
  $selectStmt->condition('cls.id', $classId);
  // Select fields and/or add expressions
  $selectStmt->addField('pli', 'name', 'long_name');
  $selectStmt->addField('pli', 'attr1', 'short_name');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  // expDebug::dPrint('exp_sp_learning.inc : getDeliveryType() : $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return '';
  }
  $deliveryTypeNames = $result->fetchAssoc(); //fetch first row in result set as an assosiative array
  expDebug::dPrint(' $deliveryTypeNames = ' . print_r($deliveryTypeNames, true) , 3);
  if ($returnType == 'Long') {
    return $deliveryTypeNames['long_name'];
  }
  if ($returnType == 'Short') {
  	return strtolower($deliveryTypeNames['short_name']);
  }
  return '';
  }catch (Exception $ex) {
    watchdog_exception('getClassDeliveryType', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getEntityNotificationOverrideInfo() - Fetches notification override entry
 *                                       from slt_entity_notification_mapping table
 */
function getEntityNotificationOverrideInfo($entityId, $entityType, $notificatonCode, $notificationSendType) {
  try {
  // Select table
  $selectStmt = db_select('slt_entity_notification_mapping', 'nmap');

  // Join other tables.
  $selectStmt->leftJoin('slt_notification_info', 'info', 'nmap.notify_id = info.id');

  // Set conditions
  $selectStmt->condition('nmap.entity_id', $entityId);
  $selectStmt->condition('nmap.entity_type', $entityType);
  $selectStmt->condition('info.notification_code', $notificatonCode);
  $selectStmt->condition('nmap.notify_send_type', $notificationSendType);

  // Select fields and/or add expressions
  $selectStmt->addField('nmap', 'status', 'notify_flag');
  $selectStmt->addField('nmap', 'notify_text', 'override_content');

  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();

  if ($result->rowCount() <= 0) {
    return null;
  }

  $result = $result->fetchAssoc();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getEntityNotificationOverrideInfo', $ex);
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
    }
    else {
      expertusErrorThrow($ex);
    }
  }
}
/*
 * getEnrollmentDetailsForNotifications() - Returns user_id, course_id, class_id, order_id, reg_status, completion_status, comp_date for the enrollment id
 *                                from slt_enrollment table.
 */
 function getEnrollmentDetailsForNotifications($enrollmentId) {
  try {
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $selectStmt->condition('enr.id', $enrollmentId);
  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'user_id', 'user_id');
  $selectStmt->addField('enr', 'course_id', 'course_id');
  $selectStmt->addField('enr', 'class_id', 'class_id');
  $selectStmt->addField('enr', 'order_id', 'order_id');
  $selectStmt->addField('enr', 'reg_status', 'reg_status');
  $selectStmt->addField('enr', 'comp_status', 'comp_status');
  $selectStmt->addField('enr', 'comp_date', 'comp_date');
  $selectStmt->addField('enr', 'score', 'score');
  $selectStmt->addField('enr', 'grade', 'grade');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint( '$result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getEnrollmentDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
 }
 /*
  * getEnrollmentandmasterEnrollmentDetailsForNotifications() - Returns user_id, course_id, class_id, order_id, reg_status, completion_status, comp_date,program_id for the enrollment id
 *                                from slt_enrollment as well as slt_master_enrollment table.
 */
 function getEnrollmentandmasterEnrollmentDetailsForNotifications($enrollmentId) {
 	try {
 		// Select table
 		$selectStmt = db_select('slt_enrollment', 'enr');
 		$selectStmt ->leftJoin('slt_master_enrollment', 'menr', 'menr.id = enr.master_enrollment_id');
 		// Set conditions
 		$selectStmt->condition('enr.id', $enrollmentId);
 		// Select fields and/or add expressions
 		$selectStmt->addField('enr', 'user_id', 'user_id');
 		$selectStmt->addField('enr', 'course_id', 'course_id');
 		$selectStmt->addField('enr', 'class_id', 'class_id');
 		$selectStmt->addField('enr', 'order_id', 'order_id');
 		$selectStmt->addField('enr', 'reg_status', 'reg_status');
 		$selectStmt->addField('enr', 'comp_status', 'comp_status');
 		$selectStmt->addField('enr', 'comp_date', 'comp_date');
 		$selectStmt->addField('menr', 'program_id', 'program_id');
 		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
 		// Execute query
 		$result = $selectStmt->execute();
 		expDebug::dPrint( '$result = ' . print_r($result, true) , 3);
 		if ($result->rowCount() <= 0) {
 			return null;
 		}
 		return $result->fetchAssoc();
 	}catch (Exception $ex) {
 		watchdog_exception('getEnrollmentDetailsForNotifications', $ex);
 		expertusErrorThrow($ex);
 	}
 }

/*
 * getNotificationTemplateInfo() - Returns the details of a notification template from slt_notification_info table
 */
function getNotificationTemplateInfo($notificationCode, $notificationLanguageCode) {
  try{
  // Select table
  $selectStmt = db_select('slt_notification_info', 'ninfo');
  // Set conditions
  $selectStmt->condition('ninfo.notification_code', $notificationCode);
  $selectStmt->condition('ninfo.lang_code', $notificationLanguageCode);
  // Select fields and/or add expressions
  $selectStmt->addField('ninfo', 'id', 'id');
  $selectStmt->addField('ninfo', 'notification_code', 'notification_code');
  $selectStmt->addField('ninfo', 'lang_code', 'lang_code');
  $selectStmt->addField('ninfo', 'locale_code', 'locale_code');
  $selectStmt->addField('ninfo', 'profile_code', 'profile_code');
  $selectStmt->addField('ninfo', 'notification_sendto', 'notification_sendto');
  $selectStmt->addField('ninfo', 'entity_type', 'entity_type');
  $selectStmt->addField('ninfo', 'notification_cc', 'notification_cc');
  $selectStmt->addField('ninfo', 'notification_admin', 'notification_admin');
  $selectStmt->addField('ninfo', 'notification_title', 'notification_title');
  $selectStmt->addField('ninfo', 'notification_description', 'notification_description');
  $selectStmt->addField('ninfo', 'notification_type', 'notification_type');
  $selectStmt->addField('ninfo', 'procedure_name', 'procedure_name');
  $selectStmt->addField('ninfo', 'notify_before', 'notify_before');
  $selectStmt->addField('ninfo', 'frequency_data_type', 'frequency_data_type');
  $selectStmt->addField('ninfo', 'frequency_data_value', 'frequency_data_value');
  $selectStmt->addField('ninfo', 'status', 'status');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  if ($result->rowCount() <= 0) {
    return null;
  }
  $result = $result->fetchAssoc();
  expDebug::dPrint('$result = ' . print_r($result, true) , 3);
  return $result;
  }
  catch (Exception $ex) {
    watchdog_exception('getNotificationTemplateInfo', $ex);
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
    }
    else {
      expertusErrorThrow($ex);
    }
  }
}
/*
 * function getLocationIdOfClass() - Fetches location id of a class from slt_course_class_session_table
 */
function getLocationIdOfClass($courseId, $classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'ses');
  // Set conditions
  $selectStmt->condition('ses.class_id', $classId);
  if (!empty($courseId)) {
    $selectStmt->condition('ses.course_id', $courseId);
  }
  // Select fields and/or add expressions
  $selectStmt->addField('ses', 'location_id', 'location_id');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getLocationIdOfClass', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * insertNotification() - Inserts notification in slt_notification. Replaces slp_notification_ins()
 */
function insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId, $iCal = NULL) {
    expDebug::dPrint(' Called with ' .
    						'$notificationType = ' . $notificationType . ', ' .
                            '$objectId1 = ' . $objectId1 . ', ' .
                            '$objectId2 = ' . $objectId2 . ', ' .
                            '$userId = ' . $userId . ', ' .
                            '$updateUserId = ' . $updateUserId . ', ' .
                            '$iCal = ' . $iCal, 4);

    //Notification should not be triggerring to Suspended/Deleted users need to be modified where ever necessary.
    try{
    // Initialize variables
    $notifyOverrideFlag = null;
    $overrideContent = '';
    $notificationCode = null;
    // Fetch notification overrride info
    switch ($notificationType) {
      case 'Register':
              // Get class delivery type
            $classDeliveryType = getClassDeliveryType($objectId2, 'Short');

            switch ($classDeliveryType) {
              case 'wbt':
              case 'vod':
                $notificationCode = 'register_wbt';
                break;
              case 'vc':
                $notificationCode = 'register_vc';
                break;
              default:
                $notificationCode = 'register';
                break;
            }

        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' Register $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;        	  
      case 'RegisterByAdmin':        	  
      	$classDeliveryType = getClassDeliveryType($objectId2, 'Short');
        
      	switch ($classDeliveryType) {
      		case 'wbt':
      		case 'vod':
      		  $notificationCode = 'register_wbt_by_admin';      				
      			break;
      		case 'vc':
      			$notificationCode = 'register_vc_by_admin';
      			break;
      		default:
      			$notificationCode = 'register_by_admin';
      			break;
      	}
      	
      	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
      	expDebug::dPrint(' RegisterByAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
      	if ($overrideInfo != null) {
      		$notifyOverrideFlag = $overrideInfo['notify_flag'];
      		$overrideContent = $overrideInfo['override_content'];
      	}
      	break;
      	
      case 'RegisterMandatoryCompliance':
      case 'RegisterVCCompliance':
      case 'RegisterWBTCompliance':
			$check_compliance_enrollment = compliance_class_enrollment($userId,$objectId1,$objectId2,true);

            $notificationCode = ($check_compliance_enrollment == 0) ? 'register_mandatory_compliance' : 'compliance_course_re_register';

            if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterMandatoryCompliance')) {
              $notificationCode = 'register_mandatory_compliance';
            } else if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterVCCompliance')) {
              $notificationCode = 'compliance_register_vc';
            } else if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterWBTCompliance')) {
              $notificationCode = 'compliance_register_wbt';
            }

            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            expDebug::dPrint(' ClassComplianceRegister $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            if ($overrideInfo != null) {
              $notifyOverrideFlag = $overrideInfo['notify_flag'];
              $overrideContent = $overrideInfo['override_content'];
            }

            break;
            
            case 'RegisterMandatoryComplianceByAdmin':
            case 'RegisterVCComplianceByAdmin':
            case 'RegisterWBTComplianceByAdmin':
            	
            	$check_compliance_enrollment = compliance_class_enrollment($userId,$objectId1,$objectId2,true);
            
            	$notificationCode = ($check_compliance_enrollment == 0) ? 'register_mandatory_compliance_by_admin' : 'compliance_course_re_register_by_admin';
            
            	if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterMandatoryComplianceByAdmin')) {
            		$notificationCode = 'register_mandatory_compliance_by_admin';
            	} else if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterVCComplianceByAdmin')) {
            		$notificationCode = 'compliance_register_vc_by_admin';
            	} else if(($check_compliance_enrollment == 0) && ($notificationType == 'RegisterWBTComplianceByAdmin')) {
            		$notificationCode = 'compliance_register_wbt_by_admin';
            	}
            
            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' ClassComplianceRegister by $check_compliance_enrollment$check_compliance_enrollment $overrideInfo = ' . print_r($check_compliance_enrollment, true) , 4);
            	expDebug::dPrint(' ClassComplianceRegister by admin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            
            	break;
            	
            
       case 'ComplianceClassReminder':
            $notificationCode = 'compliance_class_remind_session';

            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            expDebug::dPrint(' ComplianceClassReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            if ($overrideInfo != null) {
              $notifyOverrideFlag = $overrideInfo['notify_flag'];
              $overrideContent = $overrideInfo['override_content'];
            }

            break;
            case 'ComplianceExpiryRemainder':
            	$notificationCode = 'compliance_expiry_remainder';

            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' ComplianceClassReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}

            	break;
            case 'ComplianceClassExpired':
            case 'ComplianceClassExpiredAndEnrolled':
            	$notificationCode = 'compliance_expiry_remainder';

            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' ComplianceClassReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            	break;
            case 'MandatoryClassReminder' :
            	$notificationCode = 'mandatory_class_remind_session';
            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' MandatoryClassReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            	break;
            case 'MandatoryTPReminder' :
            	$notificationCode = 'mandatory_class_remind_session';
            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' MandatoryTPReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            	break;
            case 'MultipleComplianceClassNotification' :
            case 'MultipleMandatoryClassNotification' :
            	$notificationCode = 'man_comp_multiple_class_register';
            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' MandatoryClassReminder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            	break;
            case 'MultipleMantadoryTPNotification' :
            	$notificationCode = 'mandatory_tp_multiple_register';
            	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            	expDebug::dPrint(' MultipleMantadoryTPNotification $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            	if ($overrideInfo != null) {
            		$notifyOverrideFlag = $overrideInfo['notify_flag'];
            		$overrideContent = $overrideInfo['override_content'];
            	}
            	break;
      case 'ComplianceClassIncomplete':
            $notificationCode = 'compliance_class_incomplete';

            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            expDebug::dPrint(' ComplianceClassIncomplete $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            if ($overrideInfo != null) {
              $notifyOverrideFlag = $overrideInfo['notify_flag'];
              $overrideContent = $overrideInfo['override_content'];
            }
            break;
      case 'ClassMaxCapacityReached':
      	    $notificationCode = 'class_max_capacity_reached';

            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
            expDebug::dPrint(' ClassMaxCapacityReached $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            if ($overrideInfo != null) {
              $notifyOverrideFlag = $overrideInfo['notify_flag'];
              $overrideContent = $overrideInfo['override_content'];
            }
            break;

      case 'Waitlist':
        	  $notificationCode = 'waitlist';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' Waitlist $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
      case 'WaitlistByAdmin':
       	  	$notificationCode = 'waitlist_by_admin';
       	  	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
       	  	expDebug::dPrint(' WaitlistByAdmin confirmation by user $overrideInfo = ' . print_r($overrideInfo, true) , 4);
       	  	if ($overrideInfo != null) {
       	  		$notifyOverrideFlag = $overrideInfo['notify_flag'];
       	  		$overrideContent = $overrideInfo['override_content'];
       	  	}
       	  	break;        	  

      case 'WaitlistPromotion':
        	  $notificationCode = 'waitlist_promo';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WaitlistPromotion $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
              break;

      case 'WaitlistAllocated':
        	  $notificationCode = 'seat_occupied_for_waitlist';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WaitlistAllocated $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
      case 'WaitlistExpired':
        	  $notificationCode = 'waitlist_cancellation';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WaitlistCancellation $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
              break;
      case 'WaitlistDrop':
        	  $notificationCode = 'learner_drop_waitlist';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WaitlistDrop $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
              break;
      case 'Drop':
        	  $notificationCode = 'drop_register';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' Drop $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' Drop $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
      case 'WBTDropped':
        	  $notificationCode = 'drop_register_wbt_vod';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' WBTDropped $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WBTDropped $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'VCDropped':
        	  $notificationCode = 'drop_vc_register';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' VCDropped $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4 );
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' VCDropped $overrideInfo = ' . print_r($overrideInfo, true), 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

     case 'ClsRejectByInstructor':
        	  $notificationCode = 'class_rejection_by_instructor';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' ClsRejectByInstructor $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' ClsRejectByInstructor $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
       case 'ClsCompletedByInstructor':
        	  $notificationCode = 'course_completed_by_instructor';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' ClsCompletedByInstructor $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' ClsCompletedByInstructor $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
	   case 'ClsCompletedByInstructorAsNSW':
        	  $notificationCode = 'course_completed_by_instructor_as_nsw';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' ClsCompletedByInstructorAsNSW $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' ClsCompletedByInstructorAsNSW $overrideInfo = ' . print_r($overrideInfo, true) , 4);
	          if ($overrideInfo != null) {
	          		$notifyOverrideFlag = $overrideInfo['notify_flag'];
	       	  		$overrideContent = $overrideInfo['override_content'];
	      	  }
	        break;
     case 'ClsRejectByManager':
        	  $notificationCode = 'class_rejection_by_manager';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' ClsRejectByManager $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' ClsRejectByManager $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
     case 'ClsCompletedByManager':
        	  	$notificationCode = 'course_completed_by_manager';
        	  	$enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  	expDebug::dPrint(' ClscompleteByManager $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  	$classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  	$overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  	expDebug::dPrint(' ClsCompleteByManager $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  	if ($overrideInfo != null) {
        	  		$notifyOverrideFlag = $overrideInfo['notify_flag'];
        	  		$overrideContent = $overrideInfo['override_content'];
        	  	}
        	  	break;
    case 'DropRefund':
        	  $notificationCode = 'drop_class_register_refund';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' DropRefund $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' DropRefund $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'Cancel':
        	  $notificationCode = 'class_cancel';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' Cancel $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' Cancel $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

    case 'CancelClassInstructor':
        	  $notificationCode = 'class_cancel_to_instuctor';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint('CancelClassInstructor $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'WBTCancel':
        	  $notificationCode = 'class_cancel_wbt_vod';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' WBTCancel $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WBTCancel $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'ClassInActive':
        	  $notificationCode = 'class_inactive';
        	  $enrollmentDetails = getEnrollmentDetailsForNotifications($objectId1);
        	  expDebug::dPrint(' ClassInActive $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
        	  $classId = ($enrollmentDetails == null)? null : $enrollmentDetails['class_id'];
        	  $overrideInfo = getEntityNotificationOverrideInfo($classId, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' ClassInActive $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'Completed':
        	  $notificationCode = 'course_completed';
        	  $notifyOverrideFlag = 'Y';
        	  break;

      case 'pay_checkout_rejected':
        	  $notificationCode = 'pay_checkout_rejected';
              $notifyOverrideFlag = 'Y';
        	  break;

      case 'PaymentRegister':
        	  $notificationCode = 'pay_confirm_admin';
              $notifyOverrideFlag = 'Y';
        	  break;

      case 'WaitlistLocationAdmin':
        	  $notificationCode = 'waitlist_location_admin';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' WaitlistLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'CancelLocationAdmin':
        	  $notificationCode = 'cancel_location_admin';
        	  $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_cls', $notificationCode, 'Email');
        	  expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	  if ($overrideInfo != null) {
        	    $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	    $overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;

      case 'new_org':
              $notificationCode = 'new_org';
        	  $notifyOverrideFlag = 'Y';
        	  break;

      case 'new_announcement':
              $notificationCode = 'new_announcement';
        	  $notifyOverrideFlag = 'Y';
        	  break;
      case 'certCurrLPCancel':
            $notificationCode ='cert_curr_lp_cancel';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4 );
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
          break;
      case 'certCurrLPCancelRefund':
            $notificationCode ='cert_curr_lp_cancel_refund';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true), 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
          break;
      case 'certCurrLPRegister':
            $notificationCode 	= 'cert_curr_lp_register';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
            
     case 'certCurrLPRegisterByAdmin':
           $notificationCode 	= 'cert_curr_lp_register_by_admin';
           $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
           expDebug::dPrint(' certCurrLPRegisterByAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
           if ($overrideInfo != null) {
           $notifyOverrideFlag = $overrideInfo['notify_flag'];
           $overrideContent = $overrideInfo['override_content'];
           }
           break;
           
      case 'CertCurrLPComplete':
            $notificationCode 	= 'cert_curr_lp_completed';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CertCurrLPComplete $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
       case 'CertCurrLPCompleteByManager':
            $notificationCode 	= 'course_completed_by_manager';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
            expDebug::dPrint(' CertCurrLPCompleteByManager $overrideInfo = ' . print_r($overrideInfo, true) , 4);
            if ($overrideInfo != null) {
           		$notifyOverrideFlag = $overrideInfo['notify_flag'];
           		$overrideContent = $overrideInfo['override_content'];
           	}
           	break;
      case 'certCurrLPWaitlistRegister':
            $notificationCode 	= 'cert_curr_lp_waitlist_register';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' certCurrLPWaitlistRegister $overrideInfo = ' . print_r($overrideInfo, true) , 4 );
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
      case 'certCurrLPWaitlistRegisterByAdmin':
           	$notificationCode 	= 'cert_curr_lp_waitlist_register_by_admin';
           	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
           	expDebug::dPrint(' certCurrLPWaitlistRegisterByAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4 );
           	if ($overrideInfo != null) {
           		$notifyOverrideFlag = $overrideInfo['notify_flag'];
           		$overrideContent = $overrideInfo['override_content'];
           	}
           	break;            
      case 'certCurrLPWaitlistPromotion':
            $notificationCode 	= 'cert_curr_lp_waitlist_promotion';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' certCurrLPWaitlistPromotion $overrideInfo = ' . print_r($overrideInfo, true), 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
      case 'certCurrLPWaitlistCancel':
            $notificationCode 	= 'cert_curr_lp_drop_waitlist';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' certCurrLPWaitlistCancel $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
      case 'certCurrLPRecertify':
            $notificationCode 	= 'cert_curr_lp_recertify';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' certCurrLPRecertify $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
            break;
     case 'certCurrLPRecertifyByAdmin':
          	$notificationCode 	= 'cert_curr_lp_recertify_by_admin';
          	$overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
          	expDebug::dPrint(' certCurrLPRecertifyByAdmin  $overrideInfo = ' . print_r($overrideInfo, true) , 4);
          	if ($overrideInfo != null) {
          		$notifyOverrideFlag = $overrideInfo['notify_flag'];
          		$overrideContent = $overrideInfo['override_content'];
          	}
            break;
      /* case 'certCurrLPExpireRemainder':
            $notificationCode 	= 'cert_curr_lp_expire_reminder';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' certCurrLPExpireRemainder $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
        break; */
      case 'certCurrLPAssigned':
      case 'classAssign':
            $notificationCode 	= 'cert_curr_lp_assignment';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' classAssign $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
        break;
      /* case 'certCurrLPReminder':
            $notificationCode 	= 'cert_curr_lp_reminder';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true) , 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
        	break; */
      /* case 'certExpireReminderToManager':
            $notificationCode 	= 'certification_expire_manager';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true), 4 );
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
        	break; */
      case 'certCurrLPRejectionByManager':
            $notificationCode ='class_rejection_by_manager';
            $overrideInfo = getEntityNotificationOverrideInfo($objectId2, 'cre_sys_obt_trn', $notificationCode, 'Email');
        	expDebug::dPrint(' CancelLocationAdmin $overrideInfo = ' . print_r($overrideInfo, true), 4);
        	if ($overrideInfo != null) {
        	   $notifyOverrideFlag = $overrideInfo['notify_flag'];
        	   $overrideContent = $overrideInfo['override_content'];
        	}
          	break;
      /* case 'LPTerminationReminder':
            $notificationCode ='lp_termination_reminder';
            $notifyOverrideFlag = 'Y';
          	break; */
      case 'sendMobileAppLink':
        	  $notificationCode = 'mobile_app_download';
              $notifyOverrideFlag = 'Y';
        	  break;
      case 'SuspendUser':
        	  $notificationCode = 'suspend_user';
              $notifyOverrideFlag = 'Y';
        	  break;
      case 'ActivateUser':
        	  $notificationCode = 'activate_user';
              $notifyOverrideFlag = 'Y';
        	  break;
      case 'PasswordPolicyWarning':
        	  $notificationCode ='pwd_exp_warning';
        	  $overrideInfo = getEntityNotificationOverrideInfo($userId, 'cre_sys_obt_usr', $notificationCode, 'Email');
        	  expDebug::dPrint(' password_policy $$overrideInfo = ' . print_r($overrideInfo, true), 4);
        	  if ($overrideInfo != null) {
        	  	$notifyOverrideFlag = $overrideInfo['notify_flag'];
        	  	$overrideContent = $overrideInfo['override_content'];
        	  }
        	  break;
      case 'Opensurveymobile':
        	  	$notificationCode = 'open_survey_mobile';
        	  	$overrideInfo = getEntityNotificationOverrideInfo ( $userId, 'cre_sys_obt_usr', $notificationCode, 'Email' );
        	  	expDebug::dPrint ( ' open_survey_mobile $$overrideInfo = ' . print_r ( $overrideInfo, true ), 4 );
        	  	if ($overrideInfo != null) {
        	  		$notifyOverrideFlag = $overrideInfo ['notify_flag'];
        	  		$overrideContent = $overrideInfo ['override_content'];
        	  	}
        	  	break;
        	  	
        	  	case 'enrollment_waived':
        	  	case 'reverted_waived':
        	  		$notificationCode = $notificationType;
        	  		$overrideInfo = getEntityNotificationOverrideInfo ( $userId, 'cre_sys_obt_usr', $notificationCode, 'Email' );
        	  		expDebug::dPrint ( ' open_survey_mobile $$overrideInfo = ' . print_r ( $overrideInfo, true ), 4 );
        	  		if ($overrideInfo != null) {
        	  			$notifyOverrideFlag = $overrideInfo ['notify_flag'];
        	  			$overrideContent = $overrideInfo ['override_content'];
        	  		}
        	  		break;        	  	 

        	  		
      case 'RequestCertificateVerification':
      	$notificationCode = 'request_cert_verification';
      	$overrideInfo = getEntityNotificationOverrideInfo ( $userId, 'cre_sys_obt_usr', $notificationCode, 'Email' ) ;
      	if ($overrideInfo != null) {
      		$notifyOverrideFlag = $overrideInfo ['notify_flag'];
      		$overrideContent = $overrideInfo ['override_content'];
      	}
      	break;
      	
      	
      	case 'CertificateVerificationStatus':
      		$notificationCode = 'cert_verification_status';
      		$overrideInfo = getEntityNotificationOverrideInfo ( $userId, 'cre_sys_obt_usr', $notificationCode, 'Email' ) ;
      		if ($overrideInfo != null) {
      			$notifyOverrideFlag = $overrideInfo ['notify_flag'];
      			$overrideContent = $overrideInfo ['override_content'];
      		}
      		break;
	case 'DeleteCertificateByLearner':
	    $notificationCode = 'delete_cert_learner';
	    $overrideInfo = getEntityNotificationOverrideInfo ( $userId, 'cre_sys_obt_usr', $notificationCode, 'Email' ) ;
	    if ($overrideInfo != null) {
	        $notifyOverrideFlag = $overrideInfo ['notify_flag'];
	        $overrideContent = $overrideInfo ['override_content'];
	    }
	    break;
      	
      	
  } //end switch;
  // Get notification template status, recipient ('Admin' or 'User' - notification_sendto), and notification_cc e-mail (admin's email)

  /*
   * Start # 0039858 -  Salesforce Chatter Feed
  * Added By : Ganesh Babu V, Oct 10th 2014 11:15 AM
  * Description: Send the notification parametre to post the feed to salesforce user according to notification type and notification code
  * Ticket : #0039858: Implement the salesforce chatter feed in ExpertusONE
  */

  if(module_exists('salesforce_integration')){
  	module_load_include('inc','salesforce_integration','salesforce_integration_chatter');
  	if($notificationType!='LPTerminationReminder'){ //if notification type is not 'LPTerminationReminder' then call the salesforce chatter feed
  		//if($notificationType=='Register'){ // Tmp Fix for urgent - It works only for Registration Chatter Feed
  		postSalesforceFeed($notificationType,$notificationCode,$objectId1, $objectId2, $userId);
  	}
  }

  /* End # 0039858 -  Salesforce Chatter Feed */


  $notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
  expDebug::dPrint(' $notificationTemplateInfo = ' . print_r($notificationTemplateInfo, true) , 4);
  $notifyFlag = null;
  $emailSendTo = null;
  $ccEmail = null;
  if ($notificationTemplateInfo != null) {
    $notifyFlag = $notificationTemplateInfo['status'];
    $emailSendTo = $notificationTemplateInfo['notification_sendto'];
    $ccEmail = $notificationTemplateInfo['notification_cc'];
    //$adminEmail = $notificationTemplateInfo['notification_admin'];

    /*0040211: Need support to have CC in notifications to managers/admin
     *
     * 1.For this ticket we check $emailSendTo+$ccEmail content is more than one.
     * 2.If return result is more than one , check it that contain user-email
     * 3.If yes insert user-email as send-to  option others are in cc option
     * 4.Other wise first one is send-to option other or in cc option
     * ==> getSendToMailCc==> function  retrun cc values as a comma separated string..
     *
     */
    /*0040211: Need support to have CC in notifications to managers/admin
     * Start
     */

    if(!empty($ccEmail))
      	$mailIdWithCc=  $emailSendTo.",".$ccEmail;
    else
      	$mailIdWithCc=  $emailSendTo;
    expDebug::dPrint(' $mailIdWithCc = ' . print_r($mailIdWithCc, true) , 4);
    $drupalUser = getDrupalUserForNotificationInsert($userId,'','');
    $explodeMailIdWithCc = array_filter(array_unique(explode(",", $mailIdWithCc )));
    expDebug::dPrint('$explodeMailIdWithCc =' . print_r($explodeMailIdWithCc, true) , 4);
    if((count($explodeMailIdWithCc) > 1) &&(in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
     $ccValue = getSendToMailCc($mailIdWithCc, $drupalUser);
     expDebug::dPrint('$ccValue for learner = ' . print_r($ccValue, true) , 4);
    }
    elseif ((count($explodeMailIdWithCc) > 1) &&(!in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
      $removedFirstOne = $explodeMailIdWithCc;
      expDebug::dPrint('$ccValue for other than learner= ' . print_r($removedFirstOne, true) , 4);
      $removedFirstValue = array_shift($removedFirstOne);

      expDebug::dPrint('$ccValue for other than learner= ' . print_r($removedFirstOne, true) , 4);
		if($notificationType == 'ClassMaxCapacityReached'){
			$ccValue = getSendToMailCc(implode(',',$removedFirstOne),'');
		}else{
      		$ccValue = getSendToMailCc(implode(',',$removedFirstOne), $drupalUser);
		}
      expDebug::dPrint('$ccValue for other than learner= ' . print_r($ccValue, true) , 4);
    }

  }
  // Both 'Y' and 'cre_ntn_sts_atv' are active statuses
  if ($notifyFlag == 'cre_ntn_sts_atv') {
  	$notifyFlag = 'Y';
  }
  if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {  // Added
  	$notifyOverrideFlag = 'Y';
  }
  // WaitlistLocationAdmin and CancelLocationAdmin notification are not to be sent if class is of wbt type or location is null
  $classLocationId = null;
  if ($notificationType == 'WaitlistLocationAdmin' || $notificationType == 'CancelLocationAdmin') {
    $classLocationId = getLocationIdOfClass(null, $objectId2);
    expDebug::dPrint(' WaitlistLocationAdmin & CancelLocationAdmin $classLocationId = ' . print_r($classLocationId, true) , 4);
    $classDeliveryType = getClassDeliveryType(xobjectid2, 'Short');
    if ($classDeliveryType == 'wbt' || $classDeliveryType == 'vod' || $classLocationId == null) {
      $notifyFlag = 'N';
    }
  }
  // If the notification is disabled status in template, do not generate notification
  if ($notifyFlag != 'Y') {
    expDebug::dPrint('Notification is disabled');
    return;
  }
  // If the notification status has been overridden and set to disabled, do not generate notification.
  if ($notifyOverrideFlag != null && $notifyOverrideFlag != 'Y') {
    expDebug::dPrint(' Notification is overridded and disabled');
    return;
  }
  // Generate Notifications
  $notificationInfo = null;
  switch ($notificationType) {
    case 'new_announcement':
            $notificationInfo = generateNewAnnouncementNotification($objectId1, $objectId2);
            expDebug::dPrint('new_announcement $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            break;

    case 'new_org':
            $notificationInfo = generateNewOrgNotification($objectId1, $objectId2);
            expDebug::dPrint(' new_org $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            break;

    case 'Register':
    case 'RegisterByAdmin':
    case 'Waitlist':
    case 'WaitlistByAdmin':
    case 'WaitlistPromotion':
    case 'WaitlistAllocated':
    case 'WaitlistExpired':
    case 'WaitlistDrop':
    case 'WaitlistLocationAdmin':
    case 'RegisterMandatoryCompliance':
    case 'RegisterVCCompliance':
    case 'RegisterWBTCompliance':
    case 'RegisterMandatoryComplianceByAdmin':
    case 'RegisterVCComplianceByAdmin':
    case 'RegisterWBTComplianceByAdmin':    		 
    case 'ComplianceClassReminder':
    case 'MandatoryClassReminder':
    case 'MandatoryTPReminder' :
    case 'MultipleComplianceClassNotification':
    case 'MultipleMandatoryClassNotification' :
    case 'MultipleMantadoryTPNotification' :
    case 'MandatoryClassReminder' :
    case 'ComplianceClassIncomplete':
    case 'CancelClassInstructor':
            $notificationInfo = generateRegisterAndWaitlistNotifications($notificationType, $objectId1, $objectId2, $userId);
            expDebug::dPrint(' Reg&Wait $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            break;

    case 'ClassMaxCapacityReached':
            $notificationInfo = generateClassMaxCapacityReachedNotifications($notificationType, $objectId1, $objectId2, $userId);
            expDebug::dPrint(' ClassMaxCapacityReached $notificationInfo = ' . print_r($notificationInfo, true) , 4);
    	      break;
    case 'Drop':
    case 'WBTDropped':
    case 'VCDropped':
    case 'ClsRejectByInstructor':
    case 'ClsRejectByManager':
    case 'DropRefund':
    case 'Cancel':
    case 'WBTCancel':
    case 'ClassInActive':
    case 'CancelLocationAdmin':
            $notificationInfo = generateDropAndCancelNotifications($notificationType, $objectId1, $updateUserId);
            expDebug::dPrint(' Drop&Cancel $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            $overrideContent = convertNullValToNullStr($overrideContent);
            break;

    case 'Completed':
    case 'ClsCompletedByManager':
    case 'ClsCompletedByInstructor':
    case 'ClsCompletedByInstructorAsNSW':
            $notificationInfo = generateCompletedNotification($objectId1, $objectId2, $userId,$updateUserId,$notificationType);
            expDebug::dPrint(' Completed $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            break;

    case 'ComplianceExpiryRemainder':
    case 'ComplianceClassExpired':
    case 'ComplianceClassExpiredAndEnrolled':
    				$notificationInfo = generateComplianceExpiryNotification($notificationType,$objectId1, $objectId2, $userId);
    				expDebug::dPrint(' Compliance Expiry $notificationInfo = ' . print_r($notificationInfo, true) , 4);
    				break;
    case 'pay_checkout_rejected':
    case 'PaymentRegister':
            $notificationInfo = generateEcommerceNotifications($notificationType, $objectId1, $userId);
            expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
            break;
    case 'certCurrLPCancel':
          $notificationInfo = generateCertCurrLPCancelNotifications($notificationType, $objectId1, $objectId2, $userId);
          expDebug::dPrint(' Lp cancel $notificationInfo = ' . print_r($notificationInfo, true) , 4 );
      break;
    case 'certCurrLPCancelRefund':
          $notificationInfo = generateCertCurrLPCancelRefundNotifications($notificationType, $objectId1, $objectId2, $userId);
          expDebug::dPrint(' Lp cancel with refund $notificationInfo = ' . print_r($notificationInfo, true), 4);
      break;
    case 'certCurrLPRegisterByAdmin':
          $notificationInfo = generateCertCurrLPRegisterNotifications($notificationType, $objectId1, $objectId2, $userId,'Y');
          expDebug::dPrint(' Lp Register by admin $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
    case 'certCurrLPRegister':
      	$notificationInfo = generateCertCurrLPRegisterNotifications($notificationType, $objectId1, $objectId2, $userId,'N');
      	expDebug::dPrint(' Lp Register By User $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
    case 'CertCurrLPComplete':
    case 'CertCurrLPCompleteByManager':
          $notificationInfo = generateCertCurrLPRegisterNotifications($notificationType, $objectId1, $objectId2, $userId,'',$updateUserId);
          expDebug::dPrint(' Lp CertCurrLPComplete $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
    case 'certCurrLPWaitlistRegisterByAdmin':
          $notificationInfo = generateCertCurrLPWaitlistRegisterNotifications($notificationType, $objectId1, $objectId2, $userId,'Y');
          expDebug::dPrint(' certCurrLPWaitlistRegisterByAdmin $notificationInfo = ' . print_r($notificationInfo, true) , 4 );
      break;
    case 'certCurrLPWaitlistRegister':
      	 $notificationInfo = generateCertCurrLPWaitlistRegisterNotifications($notificationType, $objectId1, $objectId2, $userId,'N');
      	 expDebug::dPrint(' certCurrLPWaitlistRegister $notificationInfo = ' . print_r($notificationInfo, true) , 4 );
      break;
    case 'certCurrLPWaitlistPromotion':
          $notificationInfo = generateCertCurrLPWaitlistPromotionNotifications($notificationType, $objectId1, $objectId2, $userId);
          expDebug::dPrint(' Lp Register $notificationInfo = ' . print_r($notificationInfo, true), 4);
      break;
    case 'certCurrLPWaitlistCancel':
          $notificationInfo = generateCertCurrLPWaitlistCancelNotifications($notificationType, $objectId1, $objectId2, $userId);
          expDebug::dPrint(' Lp Register $notificationInfo = ' . print_r($notificationInfo, true) , 4);
    break;
    case 'certCurrLPRecertifyByAdmin':
          $notificationInfo = generateCertCurrLPRecertifyNotifications($notificationType, $objectId1, $objectId2, $userId,'Y');
          expDebug::dPrint(' Recertify $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
    case 'certCurrLPRecertify':
      	$notificationInfo = generateCertCurrLPRecertifyNotifications($notificationType, $objectId1, $objectId2, $userId,'N');
      	expDebug::dPrint(' Recertify By User $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
    /*case 'certCurrLPExpireRemainder':
          $notificationInfo = generateCertCurrLPExprRemainderNotifications($notificationType, $userId, $objectId1);
          expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;*/
    case 'certCurrLPAssigned':
    case 'certCurrLPRejectionByManager':
    case 'classAssign':
          $notificationInfo = generateCertCurrLPAssignCancelNotifications($notificationType, $objectId1, $objectId2, $userId, $updateUserId);
          expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      break;
  /* case 'certCurrLPReminder':
	      //Here objectId1 is programId of slt_program table
          $notificationInfo = generateCertCurrLPReminderNotifications($notificationType, $userId, $objectId1);
          expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      	  break;*/
    /*case 'certExpireReminderToManager':
    	  //Here objectId1 is manager_id of slt_person table
          $notificationInfo = generateCertExprReminderNotificationsForManager($notificationType, $userId, $objectId1);
          expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      	  break;*/
    /*case 'LPTerminationReminder':
          $notificationInfo = generateLPTerminateReminderNotifications($notificationType, $userId,$objectId1);
          expDebug::dPrint(' ECom $notificationInfo = ' . print_r($notificationInfo, true) , 4);
      	  break;*/
    case 'sendMobileAppLink':
        $notificationInfo = generateMobileAppDowloadNotification($objectId1);
        expDebug::dPrint('sendMobileAppLink ' . print_r($notificationInfo, true) , 4);
        break;
    case 'SuspendUser':
        $notificationInfo = generateSuspendUserNotification($userId);
        expDebug::dPrint('SuspendUser ' . print_r($notificationInfo, true) , 4);
        break;
    case 'ActivateUser':
        $notificationInfo = generateActivateUserNotification($userId);
        expDebug::dPrint('ActivateUser ' . print_r($notificationInfo, true) , 4);
        break;
    case 'PasswordPolicyWarning':
        $notificationInfo = generatePasswordWarningUserNotification($userId,$objectId1);
        expDebug::dPrint('PasswordPolicyWarning ' . print_r($notificationInfo, true) , 4);
        break;
    case 'Opensurveymobile':
        	$notificationInfo = generateOpenSurveyMobileNotification($userId,$objectId1,$objectId2);
        	expDebug::dPrint('PasswordPolicyWarning ' . print_r($notificationInfo, true) , 4);
        	break;
        	case 'enrollment_waived':
        	case 'reverted_waived':
        		$notificationInfo = generateWaivedEnrollmentNotification($notificationType,$objectId1,$objectId2,$userId);
        		expDebug::dPrint('Enrollment Wwaived ' . print_r($notificationInfo, true) , 4);
        		break;
        		
    case 'RequestCertificateVerification' :		
    	$notificationInfo = generateRequestVerificationNotification($userId,$objectId1,$objectId2);
    	expDebug::dPrint('RequestCertificateVerification ' . print_r($notificationInfo, true) , 4);
    	break;
    	
	case 'CertificateVerificationStatus' :
		$notificationInfo = CertificateVerificationStatus($userId,$objectId1,$objectId2);
		expDebug::dPrint('CertificateVerificationStatus ' . print_r($notificationInfo, true) , 4);
		break;
	case 'DeleteCertificateByLearner' :
	    $notificationInfo = GenerateDeleteCertificateByLearner($userId,$objectId1,$objectId2);
	    expDebug::dPrint('DeleteCertificateByLearner ' . print_r($notificationInfo, true) , 4);
	    break;
  } //End switch

  $setLanguage = 'cre_sys_lng_eng';
 $drupalUser = getDrupalUserForNotificationInsert($userId,'','');
  expDebug::dPrint(' $drupalUser = ' . print_r($drupalUser, true) , 4);
  $user_preffered_language = $drupalUser['preferred_language'];
  expDebug::dPrint(' $user_preffered_language = ' .$user_preffered_language .  '--- message id' .$notificationInfo['message_id'] , 4);
  $isPreferredNotification = 0 ;
  if(!empty($user_preffered_language) && $user_preffered_language != $setLanguage){
    $isPreferredNotification  = getPreferredNotification($notificationInfo['message_id'],$user_preffered_language);
    expDebug::dPrint(' $$isPreferredNotification = ' . $isPreferredNotification , 4);
    if($isPreferredNotification){
      $setLanguage = $user_preffered_language;
    }
  }
  if($isPreferredNotification == 0 ){
    expDebug::dPrint(' notification language = ' . $notificationInfo['lang_code'] , 4);
    if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
      $isNotificationLanguage  = getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
      expDebug::dPrint(' $$$isNotificationLanguage = ' . $isNotificationLanguage , 4);
      if($isNotificationLanguage){
        $setLanguage = $notificationInfo['lang_code'];
      }
    }
  }
		$txn = db_transaction();
		try{
  if($notificationType == 'sendMobileAppLink'){
    global $language;
    if($userId == ''){
      $select = db_select('slt_profile_list_items','spli');
      $select->addField('spli','code');
      $select->condition('spli.attr1',$language->language,'=');
      $setLanguage=$select->execute()->fetchField();
      expDebug::dPrintDBAPI(' query for default language' ,$select);
    }
    $sendToEmail = $objectId2;
  	$remark = '';
    $insertStmt1 = db_insert('slt_notification');
   	$currentUserId = getIdOfLoggedInUser();
    $insertStmt1->fields(array(
      'msg_id' => $notificationInfo['message_id'],
      'lang_code' => $setLanguage,
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
      'user_id' => NULL,
      'send_to_id' => NULL,
      'send_to_name' => '',
      'send_to_email' => $sendToEmail,
      'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => 'N',
      'attach_content' => $overrideContent,
      'remarks' => $remark,
      'created_by' => $currentUserId,
      'created_on' => now(),
    	'updated_by' => $currentUserId,
    	'updated_on' => now()
    ));

    $userNotificationId = $insertStmt1->execute();
    expDebug::dPrint(' $userNotificationId = ' . $setLanguage.'userid'.$userId , 3);
    return $userNotificationId;
  }
  if($notificationType == 'RequestCertificateVerification'){
  	global $language;
  	$userdet = getDrupalUserForNotificationInsert($objectId2,'','');
  	if($userId == ''){
  		$select = db_select('slt_profile_list_items','spli');
  		$select->addField('spli','code');
  		$select->condition('spli.attr1',$language->language,'=');
  		$setLanguage=$select->execute()->fetchField();
  		expDebug::dPrintDBAPI(' query for default language' ,$select);
  	}
  	
  	$CertificateDetailsquery = getMySkillDetails($userId,false,$objectId1,1);
  	$CertificateDetails = $CertificateDetailsquery->execute()->fetchAssoc();
  	expDebug::dPrint('generatedettt'.print_r($CertificateDetails,1),5);
  	
  	if($CertificateDetails['certificate_type'] == "External"){
  		$sendToEmail = $CertificateDetails['email'];
  		$setLanguage = "cre_sys_lng_eng";
  	}else{

  	     $sendToEmail = $userdet['email'];
  	     $select = db_select('slt_person','persn');
  		$select->addField('persn','preferred_language');
  		$select->condition('persn.user_name',$CertificateDetails['user_name'],'=');
  		$setLanguage=$select->execute()->fetchField();
  		expDebug::dPrintDBAPI(' query for default language' ,$select);
  	
  	}
  	$remark = '';
  	$insertStmt1 = db_insert('slt_notification');
  	$currentUserId = getIdOfLoggedInUser();
  	$insertStmt1->fields(array(
  			'msg_id' => $notificationInfo['message_id'],
  			'lang_code' => $setLanguage,
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
  			'user_id' => NULL,
  			'send_to_id' => NULL,
  			'send_to_name' => '',
  			'send_to_email' => $sendToEmail,
      'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => 'N',
  			'attach_content' => 1,
  			'remarks' => $remark,
  			'created_by' => $currentUserId,
      'created_on' => now(),
  			'updated_by' => $currentUserId,
  			'updated_on' => now()
    ));
    $userNotificationId = $insertStmt1->execute();
  	expDebug::dPrint(' $userNotificationId = ' . $setLanguage.'userid'.$userId , 3);
  	return $userNotificationId;
  }

  if($notificationType =="CertificateVerificationStatus" ){
  	expDebug::dPrint(' notificationsaaaaaaaaaaaaaaa= ' . $notificationType , 5);
  	global $language;
  	$userdet = getDrupalUserForNotificationInsert($userId,'','');
  	if($userId == ''){
  		$select = db_select('slt_profile_list_items','spli');
  		$select->addField('spli','code');
  		$select->condition('spli.attr1',$language->language,'=');
  		$setLanguage=$select->execute()->fetchField();
  		expDebug::dPrintDBAPI(' query for default language' ,$select);
  	}


  	 
  	$sendToEmail = $userdet['email'];
  	$remark = '';
    $insertStmt1 = db_insert('slt_notification');
  	$currentUserId = getIdOfLoggedInUser();
  	$insertStmt1->fields(array(
  			'msg_id' => $notificationInfo['message_id'],
  			'lang_code' => $setLanguage,
  			'msg_type' => $notificationInfo['message_type'],
  			'token_str' => $notificationInfo['tokens_string'],
  			'user_id' => NULL,
  			'send_to_id' => NULL,
  			'send_to_name' => '',
  			'send_to_email' => $sendToEmail,
  			'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
  			'send_type' => $notificationInfo['send_type'],
  			'send_status' => 'N',
  			//'attach_content' => '',
  			'remarks' => $remark,
  			'created_by' => $currentUserId,
  			'created_on' => now(),
  			'updated_by' => $currentUserId,
  			'updated_on' => now()
  	));
  	
  	$userNotificationId = $insertStmt1->execute();
  	
  	expDebug::dPrint(' $userNotificationId = ' . $setLanguage.'userid'.$userId , 3);
  }
  
  if($notificationType == 'DeleteCertificateByLearner'){
      global $language;
      $userdet = getDrupalUserForNotificationInsert($objectId2,'','');
      if($userId == ''){
          $select = db_select('slt_profile_list_items','spli');
          $select->addField('spli','code');
          $select->condition('spli.attr1',$language->language,'=');
          $setLanguage=$select->execute()->fetchField();
          expDebug::dPrintDBAPI(' query for default language' ,$select);
      }
       
      $CertificateDetailsquery = getMySkillDetails($userId,false,$objectId1,'','','',1);
      $CertificateDetails = $CertificateDetailsquery->execute()->fetchAssoc();
      expDebug::dPrint('generatedettt'.print_r($CertificateDetails,1),5);
       
      if($CertificateDetails['certificate_type'] == "External"){
          $sendToEmail = $CertificateDetails['email'];
          $setLanguage = "cre_sys_lng_eng";
      }
      else{
  
          $sendToEmail = $userdet['email'];
          $select = db_select('slt_person','persn');
          $select->addField('persn','preferred_language');
          $select->condition('persn.user_name',$CertificateDetails['user_name'],'=');
          $setLanguage=$select->execute()->fetchField();
          expDebug::dPrintDBAPI(' query for default language' ,$select);
      }
      $remark = '';
      $insertStmt1 = db_insert('slt_notification');
      $currentUserId = getIdOfLoggedInUser();
      $insertStmt1->fields(array(
              'msg_id' => $notificationInfo['message_id'],
              'lang_code' => $setLanguage,
              'msg_type' => $notificationInfo['message_type'],
              'token_str' => $notificationInfo['tokens_string'],
              'user_id' => NULL,
              'send_to_id' => NULL,
              'send_to_name' => '',
              'send_to_email' => $sendToEmail,
              'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
              'send_type' => $notificationInfo['send_type'],
              'send_status' => 'N',
              'attach_content' => 1,
              'remarks' => $remark,
              'created_by' => $currentUserId,
              'created_on' => now(),
              'updated_by' => $currentUserId,
              'updated_on' => now()
      ));
  
      $userNotificationId = $insertStmt1->execute();
      expDebug::dPrint(' $userNotificationId = ' . $setLanguage.'userid'.$userId , 3);
      return $userNotificationId;
  }
  $nullValue = NULL;
  $emailSendTo = explode(',', $emailSendTo);
  if ((in_array("cre_ntn_rpt_adm", $emailSendTo)) && (count($explodeMailIdWithCc) == 1) && $notificationType !="CertificateVerificationStatus") {
  	expDebug::dPrint(' notific cameeee heree  ' . $notificationType , 5);
    $customAdmin = 1;
    //$sendToEmail = variable_get('site_mail', 'info@expertusone.com');
    $insertStmt1 = db_insert('slt_notification');
    $insertStmt1->fields(array(
      'msg_id' => $notificationInfo['message_id'],
      'lang_code' => 'cre_sys_lng_eng',
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
      'user_id' => $userId,
      'send_to_id' => $nullValue,
      'send_to_name' => $nullValue,
      'send_to_email' => $nullValue,
      'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => 'N',
      'attach_content' => $overrideContent,
      'remarks' => $nullValue,
      'created_by' => $updateUserId,
      'created_on' => now(),
      'updated_by' => $updateUserId,
      'updated_on' => now(),
      'custom0'    => $customAdmin
    ));
    $userNotificationId = $insertStmt1->execute();
    expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
  }
   $sendStatus = 0;
  if($drupalUser['user_status'] == 'cre_usr_sts_atv'){
    $sendStatus = 1;
  }
  if($notificationType == 'SuspendUser'){
    $sendStatus =1;
  }
  if ((in_array("cre_ntn_rpt_usr", $emailSendTo) && $sendStatus)&& (count($explodeMailIdWithCc) == 1) &&  $notificationType !="CertificateVerificationStatus") {
  	$sendToEmail = $drupalUser['email'];
    //For mobile_push_Notification
    $mobile_Action_Details = getPushNotificationActionDetails($objectId1,$objectId2,$notificationInfo);
    $notifyMobile = $mobile_Action_Details['notifyMobile'];
    $action_detail = $mobile_Action_Details['action_detail'];
    $remark = $mobile_Action_Details['remark'];
    $customvalue = $mobile_Action_Details['customvalue'];
    $send_status = $mobile_Action_Details['send_status'];
    expDebug::dPrint(' The value of explodeMailIdWithCc = ' . print_r(count($explodeMailIdWithCc), true) , 5);
    expDebug::dPrint(' The value of mobile_Action_Details = ' . print_r($mobile_Action_Details, true) , 5);
    expDebug::dPrint(' The value of action_detail = ' . print_r($action_detail, true) , 5);
  	
    $insertStmt1 = db_insert('slt_notification');
    $insertStmt1->fields(array(
      'msg_id' => $notificationInfo['message_id'],
      'lang_code' => $setLanguage,
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
      'user_id' => $userId,
      'send_to_id' => $drupalUser['id'],
      'send_to_name' => $drupalUser['full_name'],
      'send_to_email' => $sendToEmail,
      'send_to_email_cc' => $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => $send_status,
      'attach_content' => $overrideContent,
      'ical' => $iCal,
      'remarks' => $remark,
      'mobile' => $notifyMobile,
      'detail_value' => $action_detail,
      'created_by' => $updateUserId,
      'created_on' => now(),
    	'updated_by' => $updateUserId,
    	'updated_on' => now(),
    		'custom0'    => $customvalue
    ));
    expDebug::dPrintDBAPI('$insertStmt1', $insertStmt1);
    $userNotificationId = $insertStmt1->execute();
    expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
    fireMobilePushNotification($userId,$notificationInfo,$action_detail,$drupalUser,$userNotificationId);
    
    }
  if ((in_array("cre_ntn_rpt_mgr", $emailSendTo)) && (count($explodeMailIdWithCc) == 1)) {
    $manager_id = $drupalUser['manager_id'];
    if(!empty($manager_id)) {
      $getManagerRes = getDrupalUserForNotificationInsert($manager_id);
      $sendToEmail = $getManagerRes['email'];
      $setLanguage = 'cre_sys_lng_eng';
      $manager_preffered_language = $getManagerRes['preferred_language'];
      expDebug::dPrint(' $$manager_preffered_language = ' .$manager_preffered_language .  '--- message id' .$notificationInfo['message_id'] , 4);
      $isPreferredNotification = 0 ;
      if(!empty($manager_preffered_language) && $manager_preffered_language != $setLanguage){
        $isPreferredNotification  = getPreferredNotification($notificationInfo['message_id'],$manager_preffered_language);
        expDebug::dPrint(' $$isPreferredNotification = ' . $isPreferredNotification , 4);
        if($isPreferredNotification){
          $setLanguage = $manager_preffered_language;
        }
      }
      if($isPreferredNotification == 0 ){
        expDebug::dPrint(' manager notification language = ' . $notificationInfo['lang_code'] , 4);
        if(!empty($notificationInfo['lang_code']) && $notificationInfo['lang_code'] != $setLanguage){
          $isNotificationLanguage  = getPreferredNotification($notificationInfo['message_id'],$notificationInfo['lang_code']);
          expDebug::dPrint(' manager $$$isNotificationLanguage = ' . $isNotificationLanguage , 4);
          if($isNotificationLanguage){
            $setLanguage = $notificationInfo['lang_code'];
          }
        }
      }
      $remark = '';
      $insertStmt1 = db_insert('slt_notification');
      $insertStmt1->fields(array(
        'msg_id' => $notificationInfo['message_id'],
        'lang_code' => $setLanguage,
        'msg_type' => $notificationInfo['message_type'],
        'token_str' => $notificationInfo['tokens_string'],
        'user_id' => $manager_id,
        'send_to_id' => $getManagerRes['id'],
        'send_to_name' => $getManagerRes['full_name'],
        'send_to_email' => $sendToEmail,
      	'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
        'send_type' => $notificationInfo['send_type'],
        'send_status' => 'N',
        'attach_content' => $overrideContent,
        'remarks' => $remark,
        'created_by' => $updateUserId,
        'created_on' => now(),
      	'updated_by' => $updateUserId,
      	'updated_on' => now()
      ));
      $userNotificationId = $insertStmt1->execute();
      expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true),3);
     }
    }
    if((in_array("cre_ntn_rpt_ins", $emailSendTo)) && (count($explodeMailIdWithCc) >= 1) && $notificationType == 'ClassMaxCapacityReached') {
    	$remark = '';
    	$selectStmt = db_select('slt_session_instructor_details', 'sess');
    	$selectStmt->join('slt_person', 'prsn','sess.instructor_id = prsn.id and prsn.status=\'cre_usr_sts_atv\'');
    	$selectStmt->join('users', 'usr', 'prsn.user_name = usr.name');
    	$selectStmt->distinct();
    	$selectStmt->addField('usr', 'uid', 'send_to_id');
  		$selectStmt->addField('prsn', 'full_name', 'send_to_name');
  		$selectStmt->addField('usr', 'mail', 'send_to_email');
    	$selectStmt->addExpression('"'.$notificationInfo['message_id'].'"', 'msg_id');
    	$selectStmt->addExpression('"'.$setLanguage.'"', 'lang_code');
    	$selectStmt->addExpression('"'.$notificationInfo['message_type'].'"','msg_type');
    	$selectStmt->addExpression("'".$notificationInfo['tokens_string']."'", 'token_str');    	
    	$selectStmt->addExpression('prsn.id', 'user_id');
    	$selectStmt->addExpression('if("'.$ccValue.'"!=\'\',"'.$ccValue.'", NULL)', 'send_to_email_cc');
    	$selectStmt->addExpression('"'.$notificationInfo['send_type'].'"', 'send_type');
    	$selectStmt->addExpression('\'N\'', 'send_status');
    	$selectStmt->addExpression('"'.$overrideContent.'"', 'attach_content');
    	$selectStmt->addExpression('"'.$iCal.'"', 'ical');
    	$selectStmt->addExpression('"'.$remark.'"', 'remarks');
    	$selectStmt->addExpression('"'.$updateUserId.'"', 'created_by');
    	$selectStmt->addExpression('now()', 'created_on');
    	$selectStmt->addExpression('"'.$updateUserId.'"', 'updated_by');
    	$selectStmt->addExpression('now()', 'updated_on');
    	$selectStmt->condition('sess.course_id', $objectId1, '=');
    	$selectStmt->condition('sess.class_id', $objectId2, '=');
    	expDebug::dPrintDBAPI('Notification $InsertIns', $selectStmt);
    	
		$InsertIns = db_insert('slt_notification');
		$InsertIns->from($selectStmt);
		expDebug::dPrintDBAPI('Select query $InsertIns', $selectStmt);
		expDebug::dPrintDBAPI('Insert query $InsertIns', $InsertIns);
		$userNotificationId = $InsertIns->execute();
		expDebug::dPrint(' $userNotificationId $InsertIns= ' . print_r($userNotificationId, true), 3);	 
    	
 	 
		//$usriddupchck = db_query("select instructor_id from slt_session_instructor_details WHERE course_id = '".$objectId1."' and class_id = '".$objectId2."' ")->fetchCol();
		// expDebug::dPrint('$usriddupchck= ' . print_r($usriddupchck, true) , 5);
		//sending an email to course creator(admin)
    	$selectCls = db_select('slt_course_class', 'cls');
    	$selectCls->join('slt_person', 'prsn','cls.created_by = prsn.id and prsn.status=\'cre_usr_sts_atv\'');
    	$selectCls->join('users', 'usr', 'prsn.user_name = usr.name');
    	$selectCls->addField('usr', 'uid', 'send_to_id');
    	$selectCls->addField('prsn', 'full_name', 'send_to_name');
    	$selectCls->addField('usr', 'mail', 'send_to_email');
    	$selectCls->addExpression('"'.$notificationInfo['message_id'].'"', 'msg_id');
    	$selectCls->addExpression('"'.$setLanguage.'"', 'lang_code');
    	$selectCls->addExpression('"'.$notificationInfo['message_type'].'"','msg_type');
    	$selectCls->addExpression("'".$notificationInfo['tokens_string']."'", 'token_str');
    	$selectCls->addExpression('prsn.id', 'user_id');
    	if($userNotificationId==0){
    		$selectCls->addExpression('if("'.$ccValue.'"!=\'\',"'.$ccValue.'", NULL)', 'send_to_email_cc');
    	}
    	$selectCls->addExpression('"'.$notificationInfo['send_type'].'"', 'send_type');
    	$selectCls->addExpression('\'N\'', 'send_status');
    	$selectCls->addExpression('"'.$overrideContent.'"', 'attach_content');
    	$selectCls->addExpression('"'.$iCal.'"', 'ical');
    	$selectCls->addExpression('"'.$remark.'"', 'remarks');
    	$selectCls->addExpression('"'.$updateUserId.'"', 'created_by');
    	$selectCls->addExpression('now()', 'created_on');
    	$selectCls->addExpression('"'.$updateUserId.'"', 'updated_by');
    	$selectCls->addExpression('now()', 'updated_on');
    	//if(!empty($usriddupchck))
    	//$selectCls->condition('prsn.id', $usriddupchck, 'NOT IN');
    	$selectCls->where('prsn.id NOT IN (select instructor_id from slt_session_instructor_details WHERE course_id = '.$objectId1.' and class_id = '.$objectId2.')');
    	$selectCls->condition('cls.course_id', $objectId1, '=');
    	$selectCls->condition('cls.id', $objectId2, '=');
    	expDebug::dPrintDBAPI('Notification $selectCls$selectCls', $selectCls);
    	
    	$InsertAdm = db_insert('slt_notification');
    	$InsertAdm->from($selectCls);
    	expDebug::dPrintDBAPI('Select query $InsertAdm',$selectCls);
    	expDebug::dPrintDBAPI('Insert query $InsertAdm',$InsertAdm);
    	$userAdmNotificationId = $InsertAdm->execute();
    	expDebug::dPrint(' $$userAdmNotificationId $InsertAdm= ' . print_r($userAdmNotificationId, true) , 3); 
    	
    	if(($userNotificationId==0 && $userAdmNotificationId==0) &&$ccValue!=NULL){
    		//send email to cc alone if no instructors and admin is suspended here
    		$insertStmtCC = db_insert('slt_notification');
    		$insertStmtCC->fields(array(
    				'msg_id' => $notificationInfo['message_id'],
    				'lang_code' => 'cre_sys_lng_eng',
    				'msg_type' => $notificationInfo['message_type'],
    				'token_str' => $notificationInfo['tokens_string'],
    				'user_id' => $userId,
    				'send_to_id' => NULL,
    				'send_to_name' => NULL,
    				'send_to_email' => (empty($ccValue)) ? NULL : $ccValue,
    				'send_to_email_cc' => NULL,
    				'send_type' => $notificationInfo['send_type'],
    				'send_status' => 'N',
    				'attach_content' => $overrideContent,
    				'remarks' => $remark,
    				'created_by' => $updateUserId,
    				'created_on' => now(),
    				'updated_by' => $updateUserId,
    				'updated_on' => now()
    		));
    		
    		$adminCCNotificationId = $insertStmtCC->execute();
    		expDebug::dPrint('$$adminCCNotificationId = ' . print_r($adminCCNotificationId, true) , 3);
    }
    }
    else if((in_array("cre_ntn_rpt_ins", $emailSendTo) && $sendStatus) && (count($explodeMailIdWithCc) == 1)) {
  	$sendToEmail = $drupalUser['email'];
  	 $remark = '';
    $insertStmt1 = db_insert('slt_notification');
    $insertStmt1->fields(array(
      'msg_id' => $notificationInfo['message_id'],
      'lang_code' => $setLanguage,
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
      'user_id' => $userId,
      'send_to_id' => $drupalUser['id'],
      'send_to_name' => $drupalUser['full_name'],
      'send_to_email' => $sendToEmail,
      'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => 'N',
      'attach_content' => $overrideContent,
    	'ical' => $iCal,
      'remarks' => $remark,
      'created_by' => $updateUserId,
      'created_on' => now(),
    	'updated_by' => $updateUserId,
    	'updated_on' => now()
    ));
    $userNotificationId = $insertStmt1->execute();
    expDebug::dPrint(' $userNotificationId elseif = ' . print_r($userNotificationId, true) , 3);
  }

  if ($notificationType == 'WaitlistLocationAdmin' || $notificationType == 'CancelLocationAdmin' || $notificationType == 'ClassMaxCapacityReached') {
  	$sendToEmail = getLocationContactEmail($notificationInfo['location_id']);
  	expDebug::dPrint(' LocationAdmin $sendToEmail = ' . print_r($sendToEmail, true) , 4);
  	if(!empty($sendToEmail)) {
      $remark = '';
      $insertStmt1 = db_insert('slt_notification');
      $insertStmt1->fields(array(
      'msg_id' => $notificationInfo['message_id'],
      'lang_code' => 'cre_sys_lng_eng',
      'msg_type' => $notificationInfo['message_type'],
      'token_str' => $notificationInfo['tokens_string'],
      'user_id' => $userId,
      'send_to_id' => $drupalUser['id'],
      'send_to_name' => $drupalUser['full_name'],
      'send_to_email' => $sendToEmail,
      'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
      'send_type' => $notificationInfo['send_type'],
      'send_status' => 'N',
      'attach_content' => $overrideContent,
      'remarks' => $remark,
      'created_by' => $updateUserId,
      'created_on' => now(),
      'updated_by' => $updateUserId,
      'updated_on' => now()
      ));
      $userNotificationId = $insertStmt1->execute();
      expDebug::dPrint(' $userNotificationId = ' . print_r($userNotificationId, true) , 3);
  	}
  }
 // Notify cc if required

  if (!empty($ccEmail)&& (count($explodeMailIdWithCc) == 1)) {
    $insertStmt2 = db_insert('slt_notification');
  	  $insertStmt2->fields(array(
  		  'msg_id' => $notificationInfo['message_id'],
  		  'lang_code' => 'cre_sys_lng_eng',
  		  'msg_type' => $notificationInfo['message_type'],
  		  'token_str' => $notificationInfo['tokens_string'],
  		  'user_id' => $userId,
  		  'send_to_id' => $drupalUser['id'],
  		  'send_to_name' => $drupalUser['full_name'],
  		  'send_to_email' => $ccEmail,
  	  	  'send_to_email_cc' => (empty($ccValue)) ? NULL : $ccValue,
  		  'send_type' => $notificationInfo['send_type'],
  	      'send_status' => 'N',
  		  'attach_content' => $overrideContent,
  		  'remarks' => $remark,
  		  'created_by' => $updateUserId,
  		  'created_on' => now(),
  	  	'updated_by' => $updateUserId,
  	  	'updated_on' => now()
  		));

  		$adminNotificationId = $insertStmt2->execute();
  		expDebug::dPrint('$adminNotificationId = ' . print_r($adminNotificationId, true) , 3);
  }


  if((count($explodeMailIdWithCc) > 1) &&(in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc))) {
  	//For mobile_push_Notification
  	$mobile_Action_Details = getPushNotificationActionDetails($objectId1,$objectId2,$notificationInfo);
  	$notifyMobile = $mobile_Action_Details['notifyMobile'];
  	$action_detail = $mobile_Action_Details['action_detail'];
  	$customvalue = $mobile_Action_Details['customvalue'];
  	$send_status = $mobile_Action_Details['send_status'];
  	expDebug::dPrint(' The value of mobile_Action_Details = ' . print_r($mobile_Action_Details, true) , 5);
  	 
   // $sendToEmail = $drupalUser['email'];
    $sendToEmail = getEmailId("cre_ntn_rpt_usr", $drupalUser);
    $ccValueExplode = explode(',', $ccValue);
    if(($key = array_search($sendToEmail,$ccValueExplode )) !== false) {
    	unset( $ccValueExplode[$key]);
    }
    $ccValue = implode(',', $ccValueExplode);
    $remark = '';
    $insertStmt1 = db_insert('slt_notification');
    $insertStmt1->fields(array(
        'msg_id' => $notificationInfo['message_id'],
        'lang_code' => $setLanguage,
        'msg_type' => $notificationInfo['message_type'],
        'token_str' => $notificationInfo['tokens_string'],
        'user_id' => $userId,
        'send_to_id' => $drupalUser['id'],
        'send_to_name' => $drupalUser['full_name'],
        'send_to_email' => $sendToEmail,
        'send_to_email_cc' => $ccValue,
        'send_type' => $notificationInfo['send_type'],
        'send_status' => $send_status,
        'attach_content' => $overrideContent,
        'ical' => $iCal,
        'remarks' => $remark,
    	'mobile' => $notifyMobile,
      	'detail_value' => $action_detail,
    	'created_by' => $updateUserId,
        'created_on' => now(),
        'updated_by' => $updateUserId,
        'updated_on' => now(),
    	'custom0'    => $customvalue
    ));
    expDebug::dPrintDBAPI('$insertStmt1  =>', $insertStmt1);
    $userNotificationId = $insertStmt1->execute();
    fireMobilePushNotification($userId,$notificationInfo,$action_detail,$drupalUser,$userNotificationId);
    
    
  }
  else if ((count($explodeMailIdWithCc) > 1) &&(!in_array("cre_ntn_rpt_usr", $explodeMailIdWithCc)) && $notificationType != 'ClassMaxCapacityReached') {

     $sendToEmail = getEmailId(array_shift(array_values($explodeMailIdWithCc)), $drupalUser);
     $ccValueExplode = explode(',', $ccValue);
     if(($key = array_search($sendToEmail,$ccValueExplode )) !== false) {
     	unset( $ccValueExplode[$key]);
     }
     $ccValue = implode(',', $ccValueExplode);
      $remark = '';
      $insertStmt1 = db_insert('slt_notification');
      $insertStmt1->fields(array(
        'msg_id' => $notificationInfo['message_id'],
        'lang_code' => $setLanguage,
        'msg_type' => $notificationInfo['message_type'],
        'token_str' => $notificationInfo['tokens_string'],
        'user_id' => $userId,
        'send_to_id' => $drupalUser['id'],
        'send_to_name' => $drupalUser['full_name'],
        'send_to_email' => $sendToEmail,
        'send_to_email_cc' => $ccValue,
        'send_type' => $notificationInfo['send_type'],
        'send_status' => 'N',
        'attach_content' => $overrideContent,
        'ical' => $iCal,
        'remarks' => $remark,
        'created_by' => $updateUserId,
        'created_on' => now(),
        'updated_by' => $updateUserId,
        'updated_on' => now()
    ));
    expDebug::dPrintDBAPI('$insertStmt1  =>', $insertStmt1);
    $userNotificationId = $insertStmt1->execute();
  }
			unset($txn);
  }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('insertNotification', $ex);
			if (isset($_GET['cron_key'])) {
				throw new Exception($ex);
			}
    expertusErrorThrow($ex);
  }


	}catch (Exception $ex) {
		watchdog_exception('insertNotification', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
  unset($txn);
}

function checkForManageNotification($userId,$notifType)
{

	$select = db_select('slt_manage_pushnotification','manage');
	$select->addField('manage','from_time', 'from_time');
	$select->addField('manage','to_time', 'to_time');
	$select->addField('manage','weekend', 'weekend');
	$select->addField('manage','certification', 'certification');
	$select->addField('manage','mylearning', 'mylearning');
	$select->addField('manage','myclasses', 'myclasses');
	$select->addField('manage','myteam', 'myteam');
	$select->addField('manage','receive_notification', 'receive_notification');
	$select->addField('manage','logged_out', 'logged_out');
	$select->condition('manage.user_id',$userId);
	expDebug::dPrintDBAPI('slt_manage_pushnotification query-->>',$select);
	$result = $select->execute()->fetchAll();

	expDebug::dPrint('Manage screen value = '.print_r($result,true), 4);

	if(empty($result))
	{
		expDebug::dPrint('No dataaaaaasssssss');
		return false;
	}

	if($result[0]->logged_out == 1)
	{
		return false;
	}

	$select = db_select('slt_person','per');
	$select ->leftJoin('slt_profile_list_items', 'item', 'per.time_zone = item.code');
	$select->addField('item','attr2', 'time_zone');
	$select->condition('per.id',$userId);
	expDebug::dPrintDBAPI('Timezone query = ',$select);
	$time_zone = $select->execute()->fetchField();
	expDebug::dPrint('Timezone = '.$time_zone);


	$datetime = new DateTime('NOW', new DateTimeZone('Australia/Melbourne'));
	expDebug::dPrint('Manage screen value = '.$datetime->format('H:i'));
	$datetime = new DateTime('NOW', new DateTimeZone('Asia/Kolkata'));
	expDebug::dPrint('Manage screen value = '.$datetime->format('H:i'));
	$datetime = new DateTime('NOW', new DateTimeZone($time_zone));
	expDebug::dPrint('Manage screen value = '.$datetime->format('H:i'));


	if($result[0]->receive_notification != 1)
	{
		return false;
	}

	
	$mobile_notify_message_array = array('register_by_admin','register_vc_by_admin','register_wbt_by_admin','class_cancel_wbt_vod','class_cancel','compliance_expiry_remainder','compliance_expired','open_survey_mobile');
   if(in_array($notifType,$mobile_notify_message_array))
   {
	if($result[0]->mylearning !=1)
		{
			return false;
		}
		else if($result[0]->weekend == 1)
		{
			// A time in London.
			$datetime = new DateTime('NOW', new DateTimeZone($time_zone));
			expDebug::dPrint('Manage screen value = '.$datetime->format('D'));
			if(strtolower($datetime->format('D')) == 'sat' || strtolower($datetime->format('D')) == 'sun')
			{
				return false;
			}else{
				return true;
			}
		}
		else if($result[0]->from_time != null && $result[0]->from_time != null)
		{

			$datetime = new DateTime('NOW', new DateTimeZone($time_zone));
			$currentTime = $datetime->format('H:i');

			$timeIsInBetween = checkTimeIsInBetween('12:00','24:00',$currentTime);

			expDebug::dPrint('Time difference = '.$timeIsInBetween);
			if($timeIsInBetween)
			{
				return false;
			}else{
				return true;
			}
		}
		else
		{
		return true;
		}
	}else if ($notifType == 'certification_expire' || $notifType == 'cert_curr_lp_expire_reminder') {
		if ($result [0]->certification != 1) {
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}


function checkTimeIsInBetween($from, $till, $input) {
	$fromTime = strtotime($from);
	$toTime = strtotime($till);
	$inputTime = strtotime($input);

	return($inputTime >= $fromTime and $inputTime <= $toTime);
}

/*
 * getPreferredNotification() - Gets person prefered notifications is there or not
 */
function getPreferredNotification($msgId, $languageCode){
  try{
	  $selectStmt = db_select('slt_notification_info', 'notiinfo');
	  $selectStmt ->leftJoin('slt_notification_frame', 'frame', 'frame.notification_id = notiinfo.id');
	  $selectStmt->addExpression('count(1)','cnt');
	  $selectStmt->condition('notiinfo.notification_code', $msgId);
	  $selectStmt->condition('frame.lang_code', $languageCode);
	  $selectStmt->condition('notiinfo.status', 'cre_ntn_sts_atv');
	  $result = $selectStmt->execute()->fetchField();
	  expDebug::dPrintDBAPI('getPreferredNotification',$selectStmt);
	  expDebug::dPrint('getPreferredNotification result'.print_r($result,true), 4);
	  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getPreferredNotification', $ex);
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
    }
    else {
      expertusErrorThrow($ex);
    }
  }
}
/*
 * getPersonDetailsForNotifications() - Gets person information for notifications
 */
function getPersonDetailsForNotifications($personId,$getDrupalLangCode=false) {
  try{
  // Select table
  $selectStmt = db_select('slt_person', 'per');
  if($getDrupalLangCode) {
  	$selectStmt->leftJoin('slt_profile_list_items', 'lang_spli', 'per.preferred_language = lang_spli.code && lang_spli.lang_code = \'cre_sys_lng_eng\'');
	$selectStmt->addField('lang_spli', 'attr1', 'drupal_lang_code');
  }
  // Set conditions
  $selectStmt->condition('per.id', $personId);
  // Select fields and/or add expressions
  $selectStmt->addField('per', 'id', 'id');
  $selectStmt->addField('per', 'first_name', 'first_name');
  $selectStmt->addField('per', 'last_name', 'last_name');
  $selectStmt->addField('per', 'phone_no', 'phone_no');
  $selectStmt->addField('per', 'email', 'email');
  $selectStmt->addField('per', 'user_name', 'user_name');
  $selectStmt->addField('per', 'org_id', 'org_id');
  $selectStmt->addField('per', 'full_name', 'full_name');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute()->fetchAssoc();
  expDebug::dPrint(' $result = ', 3);
  expDebug::dPrint($result, 3);
  if (count($result) <= 0) {
    return null;
  }
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getPersonDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getFullPersonDetailsForNotifications() - Gets person information for notifications
*/
function getFullPersonDetailsForNotifications($personId) {
	try{
		// Select table
		$selectStmt = db_select('slt_person', 'per');
		$selectStmt->leftjoin('slt_country', 'con', 'per.country = con.country_code');
		$selectStmt->leftjoin('slt_state', 'st', 'per.state = st.state_code and per.country = st.country_code');
		// Set conditions
		$selectStmt->condition('per.id', $personId);
		// Select fields and/or add expressions
		$selectStmt->addField('per', 'id', 'id');
		$selectStmt->addField('per', 'first_name', 'first_name');
		$selectStmt->addField('per', 'last_name', 'last_name');
		$selectStmt->addField('per', 'phone_no', 'phone_no');
		$selectStmt->addField('per', 'email', 'email');
		$selectStmt->addField('per', 'user_name', 'user_name');
		$selectStmt->addField('per', 'org_id', 'org_id');
		$selectStmt->addField('per', 'full_name', 'full_name');
		$selectStmt->addField('per', 'phone_no', 'phone_no');
		$selectStmt->addField('per', 'addr1', 'addr1');
		$selectStmt->addField('per', 'city', 'city');
		$selectStmt->addField('st', 'state_name', 'state_name');
		$selectStmt->addField('con', 'country_name', 'country_name');
		$selectStmt->addField('per', 'zip', 'zip');
		expDebug::dPrintDBAPI(' $selectStmt -->> ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchAssoc();
		expDebug::dPrint(' $result = ', 3);
		expDebug::dPrint($result, 3);
		if (count($result) <= 0) {
			return null;
		}
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getPersonDetailsForNotifications', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getAnnouncementInfoForNewAnnouncementNotification() - Gets annoucement information for new_announcement notification
 */
function getAnnouncementInfoForNewAnnouncementNotification($announcementId) {
  try{
  // Select table
  $selectStmt = db_select('slt_announcement_master', 'anm');
  // Set conditions
  $selectStmt->condition('anm.id', $announcementId);
  // Select fields and/or add expressions
  $selectStmt->addField('anm', 'title', 'title');
  $selectStmt->addField('anm', 'shortdesc', 'shortdesc');
  $selectStmt->addField('anm', 'description', 'description');
  $selectStmt->addField('anm', 'lang_code', 'lang_code');
  expDebug::dPrint(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getAnnouncementInfoForNewAnnouncementNotification', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateNewAnnouncementNotification() - Generates new_announcement notification
 */
function generateNewAnnouncementNotification($personId, $announcementId) {
  try{
  // Get person details
  $personInfo = getPersonDetailsForNotifications($personId);
  expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4);
  $personFirstName = '';
  $personLastName = '';
  $personPhoneNumber = '';
  $personEmailAddress = '';
  $fullName = '';
  $userName = '';
  if ($personInfo != null) {
    $personFirstName = convertNullValToNullStr($personInfo['first_name']);
    $personLastName =  convertNullValToNullStr($personInfo['last_name']);
    $personPhoneNumber = convertNullValToNullStr($personInfo['phone_no']);
    $personEmailAddress = convertNullValToNullStr($personInfo['email']);
    $fullName = convertNullValToNullStr($personInfo['full_name']);
    $userName = convertNullValToNullStr($personInfo['user_name']);
  }
  // Get announcement details
  $announcementInfo = getAnnouncementInfoForNewAnnouncementNotification($announcementId);
  expDebug::dPrint(' $announcementInfo = ' . print_r($announcementInfo, true) , 4);
  $announcementTitle = '';
  $announcementDescription = '';
  if ($announcementInfo != null) {
    $announcementTitle = convertNullValToNullStr($announcementInfo['title']);
    if ($announcementInfo['description'] != null) {
      $announcementDescription = $announcementInfo['description'];
    }
    else {
      $announcementDescription = convertNullValToNullStr($announcementInfo['shortdesc']);
    }
  }
  // Prepare notification info
  $notificationInfo = array();
  $notificationInfo['tokens_string'] =	'first_name>|' . $personFirstName . '~|' .
	                            		'last_name>|' . $personLastName . '~|' .
	                            		'user_email>|' . $personEmailAddress . '~|' .
										'user_phone>|' . $personPhoneNumber . '~|' .
										'announcement_title>|' . $announcementTitle . '~|' .
   										'full_name>|' . $fullName . '~|' .
                                        'learning_fullname>|' . $fullName . '~|' .
    									'user_name>|' . $userName . '~|' .
										'announcement_description>|' . $announcementDescription;
  $notificationInfo['message_id'] = 'new_announcement';
  $notificationInfo['message_type'] = 'New Announcement Creation';
  $notificationInfo['send_type'] = null;
  $notificationInfo['lang_code'] = $announcementInfo['lang_code'];
  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateNewAnnouncementNotification', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getOrganizationInfoForNotifications() - Gets organization info for notifications
 */
function getOrganizationInfoForNotifications($organizationId) {
  try{
  // Select table
  $selectStmt = db_select('slt_organization', 'org');
  // Join other tables.
  $selectStmt->leftJoin('slt_country', 'country', 'org.country = country.country_code');
  // Set conditions
  $selectStmt->condition('org.id', $organizationId);
  // Select fields and/or add expressions
  $selectStmt->addField('org', 'name', 'name');
  $selectStmt->addField('org', 'add1', 'add1');
  $selectStmt->addField('org', 'add2', 'add2');
  $selectStmt->addField('org', 'city', 'city');
  $selectStmt->addField('org', 'state', 'state');
  $selectStmt->addField('org', 'zip', 'zip');
  $selectStmt->addField('org', 'country', 'country_code');
  $selectStmt->addField('country', 'country_name', 'country_name');
  $selectStmt->addField('org', 'phone_no', 'phone_no');
  $selectStmt->addField('org', 'fax_no', 'fax_no');
  $selectStmt->addField('org', 'email', 'email');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getOrganizationInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateNewOrgNotification() - Generates new_org notification
 */
function generateNewOrgNotification($personId, $organizationId) {
  try{
  // Get person details
  $personInfo = getPersonDetailsForNotifications($personId);
  expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4);
  $personFirstName = '';
  $personLastName = '';
  $personPhoneNumber = '';
  $personEmailAddress = '';
  $fullName = '';
  $userName = '';
  if ($personInfo != null) {
      $personFirstName = convertNullValToNullStr($personInfo['first_name']);
      $personLastName =  convertNullValToNullStr($personInfo['last_name']);
      $personPhoneNumber = convertNullValToNullStr($personInfo['phone_no']);
      $personEmailAddress = convertNullValToNullStr($personInfo['email']);
      $fullName = convertNullValToNullStr($personInfo['full_name']);
      $userName = convertNullValToNullStr($personInfo['user_name']);
  }

  // Get organization details
  $organizationInfo = getOrganizationInfoForNotifications($organizationId);
  expDebug::dPrint(' $organizationInfo = ' . print_r($organizationInfo, true) , 4);
  $organizationName = '';
  $organizationAddressLine1 = '';
  $organizationAddressLine2 = '';
  $organizationCity = '';
  $organizationState = '';
  $organizationCountryCode = '';
  $organizationZip = '';
  $organizationPhone = '';

  if ($organizationInfo != null) {
      $organizationName = convertNullValToNullStr($organizationInfo['name']);
      $organizationAddressLine1 = convertNullValToNullStr($organizationInfo['add1']);
      $organizationAddressLine2 = convertNullValToNullStr($organizationInfo['add2']);
      $organizationCity = convertNullValToNullStr($organizationInfo['city']);
      $organizationState = convertNullValToNullStr($organizationInfo['state']);
      $organizationCountryCode = convertNullValToNullStr($organizationInfo['country_code']);
      $organizationZip = convertNullValToNullStr($organizationInfo['zip']);
      $organizationPhone = convertNullValToNullStr($organizationInfo['phone_no']);
  }

  // Prepare notification info
  $notificationInfo = array();
  $notificationInfo['tokens_string'] =	'first_name>|' . $personFirstName . '~|' .
  										'last_name>|' . $personLastName . '~|' .
  										'user_email>|' . $personEmailAddress . '~|' .
  										'user_phone>|' . $personPhoneNumber . '~|' .
   										'full_name>|' . $fullName . '~|' .
                                        'learning_fullname>|' . $fullName . '~|' .
    									'user_name>|' . $userName . '~|' .
  										'org_name>|' . $organizationName . '~|' .
  										'org_add1>|' . $organizationAddressLine1 . '~|' . // Note: Why is org_add2 not added to the tokens string?
  										'org_city>|' . $organizationCity . '~|' .
  										'org_state>|' . $organizationState . '~|' .
  										'org_zip>|' . $organizationZip . '~|' .
  										'org_country>|' . $organizationCountryCode . '~|' .
  										'org_phone>|' . $organizationPhone;
  $notificationInfo['message_id'] = 'new_org';
  $notificationInfo['message_type'] = 'New Organization Creation';
  $notificationInfo['send_type'] = null;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateNewOrgNotification', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCourseInfoForNotifications() - Gets course information for notifications
*/
function getCourseInfoForNotifications($courseId) {
  try{
    // Select table
    $selectStmt = db_select('slt_course_template', 'crs');
    $selectStmt->leftJoin('slt_enrollment','enr','enr.course_id=crs.id');
    // Set conditions
    $selectStmt->condition('crs.id', $courseId);
    // Select fields and/or add expressions
    $selectStmt->addField('crs', 'title', 'title');
    $selectStmt->addField('crs', 'code', 'code');
    $selectStmt->addExpression("DATE_FORMAT(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),'%d %M %Y')", 'complete_days');
    $selectStmt->addExpression("DATE_FORMAT(crs.complete_date,'%d %M %Y')",'complete_date');
    $selectStmt->addField('crs', 'lang_code', 'lang_code');
    $selectStmt->addField('crs', 'validity_date');
    $selectStmt->addField('crs', 'validity_days');
    expDebug::dPrintDBAPI(' $selectStmt course info = ' , $selectStmt);
    // Execute query
    $result = $selectStmt->execute();
    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    if ($result->rowCount() <= 0) {
      return null;
    }
    $courseInfo = $result->fetchAssoc();
    return $courseInfo;
  }catch (Exception $ex) {
    watchdog_exception('getCourseInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getCourseInfoForCompleteNotifications() - Gets course information for Complete notifications
*/
function getCourseInfoForCompleteNotifications($courseId,$userId='') {
	try{
		// Select table
		$selectStmt = db_select('slt_course_template', 'crs');
		$selectStmt->leftJoin('slt_enrollment','enr','enr.course_id=crs.id');
		// Set conditions
		$selectStmt->condition('crs.id', $courseId);
		if($userId != ''){
			$selectStmt->condition('enr.user_id', $userId);
			$selectStmt->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
		}
		// Select fields and/or add expressions
		$selectStmt->addField('crs', 'title', 'title');
		$selectStmt->addField('crs', 'code', 'code');
		$selectStmt->addExpression("DATE_FORMAT(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),'%d %M %Y')", 'complete_days');
		$selectStmt->addExpression("DATE_FORMAT(crs.complete_date,'%d %M %Y')",'complete_date');
		$selectStmt->addField('crs', 'lang_code', 'lang_code');
		$selectStmt->addField('crs', 'validity_date');
		$selectStmt->addField('crs', 'validity_days');
		expDebug::dPrintDBAPI(' $selectStmt course info = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		// expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
		if ($result->rowCount() <= 0) {
			return null;
		}
		$courseInfo = $result->fetchAssoc();
		return $courseInfo;
	}catch (Exception $ex) {
		watchdog_exception('getCourseInfoForNotifications', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getClassInfoForNotifications() - Gets class information for notifications
 */
function getClassInfoForNotifications($classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class', 'cls');
  // Set conditions
  $selectStmt->condition('cls.id', $classId);
  // Select fields and/or add expressions
  $selectStmt->addField('cls', 'title', 'title');
  $selectStmt->addField('cls', 'code', 'code');
  $selectStmt->addField('cls', 'course_id');
  $selectStmt->addField('cls', 'short_description','description');
  $selectStmt->addField('cls', 'delivery_type', 'delivery_type');
  $selectStmt->addField('cls', 'price', 'price');
  $selectStmt->addField('cls', 'currency_type', 'currency_code');
  $selectStmt->addField('cls', 'lang_code', 'lang_code');
  $selectStmt->addField('cls', 'status', 'status');
  $selectStmt->addField('cls', 'additional_info', 'additional_info');
  $selectStmt->addField('cls', 'addn_catalog_show', 'addn_catalog_show');
  $selectStmt->addField('cls', 'addn_notification_show', 'addn_notification_show');
  
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  // expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  $classInfo = $result->fetchAssoc();
  $classInfo['delivery_type_name'] = getProfileListItemName($classInfo['delivery_type']);
  return $classInfo;
  }catch (Exception $ex) {
    watchdog_exception('getClassInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getTpInfoForNotifications() - Gets TP information for notifications
*/

function getTpInfoForNotifications($programId) {
	try{
		// Select table
		$selectStmt = db_select('slt_program', 'prg');
		// Set conditions
		$selectStmt->condition('prg.id', $programId);
		// Select fields and/or add expressions
		$selectStmt->addField('prg', 'code', 'code');
		$selectStmt->addField('prg', 'title', 'title');
		$selectStmt->addField('prg','short_desc', 'description');	
		$selectStmt->addField('prg', 'price', 'price');
		$selectStmt->addField('prg', 'additional_info', 'additional_info');
  		$selectStmt->addField('prg', 'addn_catalog_show', 'addn_catalog_show');
 		$selectStmt->addField('prg', 'addn_notification_show', 'addn_notification_show');
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		// expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
		if ($result->rowCount() <= 0) {
			return null;
		}
		$classInfo = $result->fetchAssoc();
		return $classInfo;
	}catch (Exception $ex) {
		watchdog_exception('getTpInfoForNotifications', $ex);
		expertusErrorThrow($ex);
	}
}

function getEnrollInfoForNotifications($courseId, $classId, $userId) {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $selectStmt->condition('enr.course_id', $courseId);
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.user_id', $userId);
  $selectStmt->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl');
  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'waitlist_priority', 'waitlist_position');
  $selectStmt->addField('enr', 'is_compliance', 'is_compliance');
  // Execute query
  $result = $selectStmt->execute();
  if ($result->rowCount() <= 0) {
    return null;
  }
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  $enrollInfo = $result->fetchAssoc();
  return $enrollInfo;
  }catch (Exception $ex) {
    watchdog_exception('getEnrollInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getClassSessionInfoForNotifications() - Get class session information for notifications
 */
function getClassSessionInfoForNotifications($courseId, $classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class_session', 'ses');
  // Set conditions
  $selectStmt->condition('ses.course_id', $courseId);
  $selectStmt->condition('ses.class_id', $classId);
  // Join other tables.
  $selectStmt->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
  $selectStmt->leftJoin('slt_location', 'loc', 'ses.location_id = loc.id');
  // Select fields and/or add expressions
  $selectStmt->addField('loc', 'name', 'location_name');
  $selectStmt->addField('ses', 'start_date', 'start_date');
  $selectStmt->addField('ses', 'end_date', 'end_date');
  $selectStmt->addField('ses', 'timezone', 'timezone');
  $selectStmt->addField('ses', 'capacity_max', 'capacity_max');
  $selectStmt->addField('splt_timezone','attr2', 'session_timezone');
  $selectStmt->addField('splt_timezone','name', 'session_timezone_name');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' ,$selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  $classSessionInfo = $result->fetchAssoc();
  $classSessionInfo['start_date'] = dateTimeStrToUSShort($classSessionInfo['start_date'], true, true, true, false);
  $classSessionInfo['end_date'] = dateTimeStrToUSShort($classSessionInfo['end_date'], true, true, true, false);
  return $classSessionInfo;
  }catch (Exception $ex) {
    watchdog_exception('getClassSessionInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}



function classSessionNotificationDetailsForInstructor($last_run_date){
	try{
		expDebug::dPrint(' $$last_run_date = ' . print_r($last_run_date, true) , 3);
		// Select table
		$selectStmt = db_select('slt_course_class_session', 'ses');
		$selectStmt->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=ses.id');
		$selectStmt->leftjoin('slt_course_class', 'class', 'class.id=ses.class_id');
		// Join other tables.
		$selectStmt->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
		$selectStmt->leftJoin('slt_location', 'loc', 'ses.location_id = loc.id');
		$selectStmt->leftJoin('slt_country', 'country', 'loc.country = country.country_code');
		$selectStmt->leftJoin('slt_state', 'state', 'country.country_code = state.country_code &&  loc.state = state.state_code');

		// Select fields and/or add expressions
		$selectStmt->addField('ses', 'id', 'id');
		$selectStmt->addField('ses', 'title', 'title');
		$selectStmt->addField('ses', 'course_id', 'course_id');
		$selectStmt->addField('ses', 'class_id', 'class_id');
		//$selectStmt->addField('ses', 'instructor_id', 'instructor_id');
		$selectStmt->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'instructor_id');
		$selectStmt->addField('loc', 'name', 'location_name');
		$selectStmt->addField('loc', 'addr1', 'addr1');
		$selectStmt->addField('loc', 'addr2', 'addr2');
		$selectStmt->addField('loc', 'city', 'city');
		$selectStmt->addField('state', 'state_name', 'state');
		$selectStmt->addField('country', 'country_name', 'country');
		$selectStmt->addField('loc', 'zipcode', 'zipcode');
		$selectStmt->addField('ses', 'start_time', 'start_time');
		$selectStmt->addField('ses', 'end_time', 'end_time');
		$selectStmt->addField('ses', 'start_date', 'start_date');
		$selectStmt->addField('ses', 'end_date', 'end_date');
		$selectStmt->addField('ses', 'ical_sequence', 'seq');
		$selectStmt->addField('ses', 'timezone', 'timezone');
		$selectStmt->addField('ses', 'capacity_max', 'capacity_max');
		$selectStmt->addField('ses', 'custom4', 'previous_instructor_id');
		$selectStmt->addField('splt_timezone','attr2', 'session_timezone');
		$selectStmt->addField('splt_timezone','name', 'session_timezone_name');
		// Set conditions
		$select_or = db_or();
		//$select_or->condition('ses.created_on', $last_run_date,'>');
		//$select_or->condition('ses.updated_on', $last_run_date,'>');
		$select_or->where("DATE_FORMAT(ses.created_on, '%Y-%m-%d') = subdate(current_date, 1)");
		$select_or->where("DATE_FORMAT(ses.updated_on, '%Y-%m-%d') = subdate(current_date, 1)");
		$selectStmt->condition($select_or);
		$selectStmt->condition('class.status',array('lrn_cls_sts_atv','lrn_cls_sts_itv'), 'IN');
		$select_ins_or = db_or();
		$select_ins_or->where("DATE_FORMAT(ses_ins.created_on, '%Y-%m-%d') = subdate(current_date, 1)");
		$select_ins_or->where("DATE_FORMAT(ses_ins.updated_on, '%Y-%m-%d') = subdate(current_date, 1)");
		$select_ins_or = db_and();
		$select_ins_or->where("DATE_FORMAT(ses.start_date, '%Y-%m-%d') >=current_date");
		$selectStmt->condition($select_ins_or);
		// Limit to 1 record (the first record)
		//$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt notification instructor= ' ,$selectStmt);
		$selectStmt->groupBy('ses.id');
		// Execute query
		$result = $selectStmt->execute()->fetchAll();
		$new_result_arr = array();
		$previous_instructor_arr = array();
		$i = 0;
		$update_session_id = array(); 
		foreach ($result as $key=>$val){
			//if(!empty($val->instructor_id) && empty($val->previous_instructor_id))
			if(!empty($val->instructor_id))
				$new_result_arr[$val->class_id][$val->instructor_id][] = $result[$i];
			if(!empty($val->previous_instructor_id)){
				$previous_instructor_arr[$val->class_id][$val->previous_instructor_id][] = $result[$i];
				$update_session_id[] = $val->id;
			}
			$i++;
		}
		expDebug::dPrint(' REsturctured arr = ' . print_r($new_result_arr, true) , 3);
		expDebug::dPrint(' REsturctured arr previous_instructor_arr= ' . print_r($previous_instructor_arr, true) , 3);
		expDebug::dPrint(' REsturctured arr update_session_id= ' . print_r($update_session_id, true) , 3);
		//Send Notification if newly created or modified.
		$notfication_status = getPreferredNotification('class_session_notification_for_instructor','cre_sys_lng_eng');
		expDebug::dPrint(' INS NOTIFICAION STAT = ' . print_r($notfication_status, true) , 3);
		if(count($new_result_arr) && $notfication_status>0){
				foreach($new_result_arr as $key=>$val){
					 $classInfo = getClassInfoForNotifications($key);
					if ($classInfo != null) {
   	    $classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
        $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
                                            }
					 expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 3);
					 expDebug::dPrint(' $val = ' . print_r($val, true) , 3);
					 $notificationInfo = array();
					 foreach ($val as $instructor_id => $notificationVal){
					 		if(!empty($instructor_id)){
					 			$instructor_ids_arr = explode(',',$instructor_id);
						 			foreach ($instructor_ids_arr as $ins_id){
										$classInstructorInfo  = getUserInformations($ins_id);
										expDebug::dPrint(' $classInstructorInfo = ' . print_r($classInstructorInfo, true) , 3);
										$notificationInfo['send_to_mail'] = $classInstructorInfo['email'];
										$notificationInfo['user_id'] = $ins_id;
										$notificationInfo['send_to_name'] = $classInstructorInfo['first_name'];
										$notificationInfo['tokens_string'] =	'first_name>|' . $classInstructorInfo['first_name'] . '~|' .
												'last_name>|' . $classInstructorInfo['last_name'] . '~|' .
												'class_title>|' . $classInfo['title'] . '~|' .
												'class_code>|' . $classInfo['code'] . '~|' .
												(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';
										    expDebug::dPrint(' $notificationVal = ' . print_r($notificationVal, true) , 3);
										    $iCalArr = array();
											$iCalArr['notificationVal'] = $notificationVal;
											$iCalArr['classInfo'] = $classInfo;
											$iCalArr['classInstructorInfo'] = $classInstructorInfo;
											$classSessionInfo = generateNotificationInfoForInstructor($notificationVal);
											$iCal = getPublishICal($key, 'Class',null,$iCalArr);
											//$iCal = getCancelICal($key, 'Class',null,$iCalArr);
											expDebug::dPrint(' $icalllll = ' . print_r($iCal, true) , 3);
											expDebug::dPrint(' $$classSessionInfo = ' . print_r($classSessionInfo, true) , 3);
											$timeZone = '';
											$classSessionInformation = $classSessionInfo[0];
											if ($classSessionInformation == null) {
												$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
											}
	
											$sessionLocation = $classSessionInfo[1];
											if ($sessionLocation == null) {
												$LocationDel = ' - ';
											}
											if(!empty($sessionLocation)) {
											$LocationDel = ClassLocationDetails($notificationVal[0]->class_id,$sessionLocation);
											if(count($LocationDel) < 0)  $LocationDel = $sessionLocation;
											} else $LocationDel = ' - ';
											// Append tokens string with class session details
											$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
											'session_location>|' . $LocationDel . '~|' .
											'group_sessiondata>|' . $classSessionInformation;
	
											// For vc class append timezone to location
											if ( $classInfo['delivery_type'] == 'lrn_cls_dty_vc') {
												$notificationInfo['tokens_string'] = '~|'.$notificationInfo['tokens_string'] .
												' (' . $timeZone . ') ';
											}
	
											//class_session_notification_for_instructor
											$notificationInfo['message_id'] = 'class_session_notification_for_instructor';
											$notificationInfo['message_type'] = 'Class Session details for instructor';
											$notificationInfo['send_type'] = 'php mailer';
											$nullValue = NULL;
											$customAdmin = 1;
											$insertStmt1 = db_insert('slt_notification');
											$insertStmt1->fields(array(
													'msg_id' => $notificationInfo['message_id'],
													'lang_code' => $classInstructorInfo['preferred_language'],
													'msg_type' => $notificationInfo['message_type'],
													'token_str' => $notificationInfo['tokens_string'],
													'user_id' => $notificationInfo['user_id'],
													'send_to_id' => $notificationInfo['user_id'],
													'send_to_name' => $notificationInfo['send_to_name'],
													'send_to_email' => $notificationInfo['send_to_mail'],
													'send_type' => $notificationInfo['send_type'],
													'send_status' => 'N',
													'attach_content' => $nullValue,
													'ical' =>$iCal,
													'remarks' => $nullValue,
													'created_by' => $customAdmin,
													'created_on' => now(),
													'updated_by' => $customAdmin,
													'updated_on' => now(),
													'custom0'    => $nullValue
											));
											$userNotificationId = $insertStmt1->execute();
											add_audit_trail_entry(1, $key, $classInfo['delivery_type'], 'exp_sp_learning', $notificationInfo['send_type'],	$notificationInfo['message_type'] . $notificationInfo['send_to_mail']);
		
						 		}
					 		}
									//$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|';
					 }
				}
		}
		expDebug::dPrint(' $result = ' . print_r($new_result_arr, true) , 3);
		expDebug::dPrint(' $notificationInfo= ' . print_r($notificationInfo, true) , 3);


		//Send Notification if instructor modified.
		$notfication_status = getPreferredNotification('deallocated_as_instructor_for_assigned_class_session','cre_sys_lng_eng');
		if(count($previous_instructor_arr)>0 && $notfication_status>0){
				foreach($previous_instructor_arr as $key=>$val){
					$classInfo = getClassInfoForNotifications($key);
					$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
                    $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
					expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 3);
					expDebug::dPrint(' $val = ' . print_r($val, true) , 3);
					$notificationInfo = array();
					foreach ($val as $instructor_id => $notificationVal){
						if(!empty($instructor_id)){
							$instructor_ids_arr = explode(',',$instructor_id);
							foreach ($instructor_ids_arr as $ins_id){			
								$classInstructorInfo  = getUserInformations($ins_id);
								expDebug::dPrint(' $classInstructorInfo = ' . print_r($classInstructorInfo, true) , 3);
								$notificationInfo['send_to_mail'] = $classInstructorInfo['email'];
								$notificationInfo['user_id'] = $ins_id;
								$notificationInfo['send_to_name'] = $classInstructorInfo['first_name'];
								$notificationInfo['tokens_string'] =	'first_name>|' . $classInstructorInfo['first_name'] . '~|' .
										'last_name>|' . $classInstructorInfo['last_name'] . '~|' .
										'class_title>|' . $classInfo['title'] . '~|' .
										'class_code>|' . $classInfo['code'] . '~|' .
										(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';
								expDebug::dPrint(' $notificationVal = ' . print_r($notificationVal, true) , 3);
								$iCalArr = array();
								$iCalArr['notificationVal'] = $notificationVal;
								$iCalArr['classInfo'] = $classInfo;
								$iCalArr['classInstructorInfo'] = $classInstructorInfo;
								$iCal = getCancelICal($key, 'Class',null,$iCalArr,'change');
								$classSessionInfo = generateNotificationInfoForInstructor($notificationVal);
								expDebug::dPrint(' $$classSessionInfo = ' . print_r($classSessionInfo, true) , 3);
								$timeZone = '';
								$classSessionInformation = $classSessionInfo[0];
								if ($classSessionInformation == null) {
									$classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
								}
	
								$sessionLocation = $classSessionInfo[1];
								if ($sessionLocation == null) {
									$LocationDel = ' - ';
								}
								if(!empty($sessionLocation)) {
											$LocationDel = ClassLocationDetails($notificationVal[0]->class_id,$sessionLocation);
											if(count($LocationDel) < 0) $LocationDel = $sessionLocation;
								} else $LocationDel = ' - ';
								// Append tokens string with class session details
								$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
								'session_location>|' . $LocationDel . '~|' .
								'group_sessiondata>|' . $classSessionInformation;
	
								// For vc class append timezone to location
								if ( $classInfo['delivery_type'] == 'lrn_cls_dty_vc') {
									$notificationInfo['tokens_string'] = '~|'.$notificationInfo['tokens_string'] .
									' (' . $timeZone . ') ';
								}
	
								//class_session_notification_for_instructor
								$notificationInfo['message_id'] = 'deallocated_as_instructor_for_assigned_class_session';
								$notificationInfo['message_type'] = 'Deallocated As Instructor For Assigned Class session';
								$notificationInfo['send_type'] = 'php mailer';
								$nullValue = NULL;
								$customAdmin = 1;
								$insertStmt1 = db_insert('slt_notification');
								$insertStmt1->fields(array(
										'msg_id' => $notificationInfo['message_id'],
										'lang_code' => $classInstructorInfo['preferred_language'],
										'msg_type' => $notificationInfo['message_type'],
										'token_str' => $notificationInfo['tokens_string'],
										'user_id' => $notificationInfo['user_id'],
										'send_to_id' => $notificationInfo['user_id'],
										'send_to_name' => $notificationInfo['send_to_name'],
										'send_to_email' => $notificationInfo['send_to_mail'],
										'send_type' => $notificationInfo['send_type'],
										'send_status' => 'N',
										'attach_content' => $nullValue,
										'ical' =>$iCal,
										'remarks' => $nullValue,
										'created_by' => $customAdmin,
										'created_on' => now(),
										'updated_by' => $customAdmin,
										'updated_on' => now(),
										'custom0'    => $nullValue
								));
								$userNotificationId = $insertStmt1->execute();
								add_audit_trail_entry(1, $key, $classInfo['delivery_type'], 'exp_sp_learning', $notificationInfo['send_type'],	$notificationInfo['message_type'] . $notificationInfo['send_to_mail']);
	
							}
						//$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|';
						}
					}
				}
			//update previous session instructor details to null. Multiple instructor related changes
				$updateData = db_update('slt_course_class_session');
				$updateFields = array('custom4' => NULL);
				$updateData->fields($updateFields);
				$update_session_id = array_unique($update_session_id);
				expDebug::dPrint(' update_session_ids = ' . print_r($update_session_ids, true) , 3);
				$updateData->condition('id',$update_session_id, 'IN');
				expDebug::dPrintDBAPI(' $selectStmt notification instructor= ' ,$updateData);
				$updateData->execute();
		}
		syncSolrData('Class');
		expDebug::dPrint(' $$previous_instructor_arr = ' . print_r($previous_instructor_arr, true) , 3);
		expDebug::dPrint(' $notificationInfo= ' . print_r($notificationInfo, true) , 3);


	}catch (Exception $ex) {
		watchdog_exception('classSessionNotificationDetailsForInstructor', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}
function generateNotificationInfoForInstructor($sessionList){
	$classSessionInfo = '';
	$classSessionInfo1 = array();
		foreach ($sessionList as $session) {
			$sessionTitle = ($session->title == null)? ' ' : $session->title;
			$sessionStartDate = dateTimeStrToUSShort($session->start_date, true, true, true, false);
			$sessionStartDate = convertNullValToNullStr($sessionStartDate);
			$sessionEndDate   = dateTimeStrToUSShort($session->end_date, true, true, true, false);
			$sessionEndDate = convertNullValToNullStr($sessionEndDate);
			$sessionStartTime = ($session->start_time != '' && $session->start_time != null)?date_format(date_create($session->start_time),'g:i A'):'';
			$sessionEndTime   = ($session->end_time != '' && $session->end_time != null)?date_format(date_create($session->end_time),'g:i A'):'';
			$sessionLocation  = $session->location_name;
			$sessionLocation = convertNullValToNullStr($sessionLocation);
			$sessionTimeZone = convertNullValToNullStr($session->session_timezone_name);
			if(!empty($sessionTimeZone))
				$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
			$classSessionInfo =	$classSessionInfo .
			'session_name<|' . $sessionTitle . '~~' .
			'session_startdate<|' . $sessionStartDate . '~~' .
			'session_enddate<|' . $sessionEndDate . '~~' .
			'session_starttime<|' . $sessionStartTime . '~~' .
			'session_endtime<|' . $sessionEndTime .  '~~' .
			'session_location<|' . $sessionLocation . '~~' .
			'time_zone<|' . $sessionTimeZoneDisp .
			'~@';
		} // end foreach
		$classSessionInfo1[0] = $classSessionInfo;
		$classSessionInfo1[1] = $sessionLocation;
		$classSessionInfo1[2] = $sessionTimeZone;
		return $classSessionInfo1;
}

function getPublishICalEventsForInstructor($iCalArr) {
	try {
                expDebug::dPrint('$classSessionsInfoList $iCalArr--> '.print_r($iCalArr,1),4);
		$classSessionsInfoList = $iCalArr['notificationVal'];
		$classInstructorInfo = $iCalArr['classInstructorInfo'];
		$classInfo = $iCalArr['classInfo'];
		expDebug::dPrint('$classSessionsInfoList--> '.print_r($classSessionsInfoList,1),4);
		if (empty($classSessionsInfoList)) {
			return null;
		}
		$organizerEmail =  variable_get('site_mail', 'info@expertusone.com');
		$organizerName =  variable_get('site_name', 'ExpertusONE');
		$vEventsArr = array();
		$preLanguage = getProfileListItemAttr($classInstructorInfo['preferred_language']);
		foreach ($classSessionsInfoList as $classSessionInfo) {
				// Increment the session ical sequnce number for next ical
				$updateStmt = db_update('slt_course_class_session');
				$updateStmt->condition('id', $classSessionInfo->id, '=');
				//48648: Notification on class session assigned to Instructor getting triggered multiple times
				//$updateStmt->fields(array('updated_by' => 1, 'updated_on' => now()));
				$updateStmt->fields(array('updated_by' => 1));
				$updateStmt->expression('ical_sequence', 'ical_sequence + 1');
				expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
				$updateStmt->execute();
				// Generate event UID from TP enrollment id, Class enrollment id and Session id
				$eventUID = 'ExpertusONE_' . $classSessionInfo->class_id . '_' . $classSessionInfo->id . '_' . $classSessionInfo->instructor_id . '@' . $_SERVER['SERVER_NAME'];
				//$eventUID = 'ExpertusONE_' . $classSessionInfo->class_id . '_' . $classSessionInfo->id .'@' . $_SERVER['SERVER_NAME'];

				// Prepare event description and x-alt-desc
				$description = prepareICalendarVEventDescription($classSessionInfo, 'class', $preLanguage,$iCalArr);
				$altDescription = prepareICalendarVEventAltDescription($classSessionInfo, 'class', $preLanguage,$iCalArr);
				$summary = $classInfo['title'] . ((count($classSessionsInfoList) <= 1)? "" : " -  $classSessionInfo->title");
				$utcSystemDatetime = date_format(date_now('UTC'), 'Ymd\THis\Z');
				list($utcStartDatetime, $utcEndDatetime) = prepareICalEventDateTime($classSessionInfo->start_date, $classSessionInfo->start_time,
						$classSessionInfo->end_time, $classSessionInfo->session_timezone);
				$vEvent  = "BEGIN:VEVENT\r\n";
				$vEvent .= "ORGANIZER;CN=" . $organizerName . ":MAILTO:" . $organizerEmail . "\r\n"; //ORGANIZER;CN=Me@ExpertusGMAIL:mailto:sunilkumarg@expertus.com
				$vEvent .= "DTSTART:" . $utcStartDatetime . "\r\n";
				$vEvent .= "DTEND:" . $utcEndDatetime . "\r\n";
				$vEvent .= "LOCATION:" . $classSessionInfo->location_name . "\r\n";
				$vEvent .= "TRANSP:OPAQUE\r\n";
				$vEvent .= "UID:" . $eventUID . "\r\n";
				$vEvent .= "X-ALT-DESC;FMTTYPE=text/html:" . $altDescription . "\r\n";
				$vEvent .= "SEQUENCE:" . $classSessionInfo->seq . "\r\n"; // revision number
				$vEvent .= "SUMMARY:" . $summary . "\r\n";
				$vEvent .= "DESCRIPTION:" . $description . "\r\n";
				$vEvent .= "CREATED:" . $utcSystemDatetime . "\r\n";
				$vEvent .= "LAST-MODIFIED:" . $utcSystemDatetime . "\r\n";
				$vEvent .= "DTSTAMP:" . $utcSystemDatetime . "\r\n";
				$vEvent .= "PRIORITY:5\r\n";
				$vEvent .= "CLASS:PUBLIC\r\n";
				$vEvent .= "STATUS:CONFIRMED\r\n";
				$vEvent .= "ATTENDEE;CUTYPE=INDIVIDUAL;ROLE=REQ-PARTICIPANT;PARTSTAT=ACCEPTED:mailto:" . $classInstructorInfo['email'] . "\r\n"; // http://webapps.stackexchange.com/questions/31747/google-calendar-auto-adding-ics-events
				$vEvent .= "X-MICROSOFT-CDO-BUSYSTATUS:BUSY\r\n";
				$vEvent .= "X-MICROSOFT-CDO-IMPORTANCE:1\r\n";
				$vEvent .= "X-MICROSOFT-CDO-INTENDEDSTATUS:BUSY\r\n";
				$vEvent .= "X-MICROSOFT-DISALLOW-COUNTER:TRUE\r\n";
				$vEvent .= "X-MS-OLK-AUTOSTARTCHECK:FALSE\r\n";
				$vEvent .= "X-MS-OLK-AUTOFILLLOCATION:FALSE\n"; // http://www.how2guru.com/archives/php-generate-calendar-file-ics/
				$vEvent .= "X-MS-OLK-CONFTYPE:0\r\n";
				$vEvent .= "BEGIN:VALARM\r\n";
				$vEvent .= "TRIGGER:-PT15M\r\n";
				$vEvent .= "ACTION:DISPLAY\r\n";
				$vEvent .= "DESCRIPTION:Reminder\r\n";
				$vEvent .= "END:VALARM\r\n";
				$vEvent .= "END:VEVENT\r\n";
				$vEventsArr[] = $vEvent;
		} // end foeach
		syncSolrData('Class');
		expDebug::dPrint('$vEventsArr = ' . print_r($vEventsArr, true), 4);
		return $vEventsArr;
	}
	catch (Exception $ex) {
		watchdog_exception('getPublishICalEventsForInstructor', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares iCalendar cancelled vevents text for an enrollment
 */
function getCancelICalEventsForInstructor($iCalArr,$callFrom) {
	try {
		expDebug::dPrint('getCancelICalEventsForInstructor ' . print_r($iCalArr, true), 4);
		$classSessionsInfoList = $iCalArr['notificationVal'];
		$classInstructorInfo = $iCalArr['classInstructorInfo'];
		$classInfo = $iCalArr['classInfo'];

		if (empty($classSessionsInfoList)) {
			return null;
		}
		$organizerEmail =  variable_get('site_mail', 'info@expertusone.com');
		$organizerName =  variable_get('site_name', 'ExpertusONE');
		$vEventsArr = array();
		$preLanguage = getProfileListItemAttr($classInstructorInfo['preferred_language']);
		foreach ($classSessionsInfoList as $classSessionInfo) {

			// Increment the session ical sequnce number for next ical
			$updateStmt = db_update('slt_course_class_session');
			$updateStmt->condition('id', $classSessionInfo->id, '=');
			//48648: Notification on class session assigned to Instructor getting triggered multiple times
			//$updateStmt->fields(array('updated_by' => 1, 'updated_on' => now()));
			$updateStmt->fields(array('updated_by' => 1));
			$updateStmt->expression('ical_sequence', 'ical_sequence + 1');
			expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
			$updateStmt->execute();
			// Generate event UID from TP enrollment id, Class enrollment id and Session id
			if($callFrom == 'change')
				 $instructorId = $classSessionInfo->previous_instructor_id;
			else
				$instructorId = $classSessionInfo->instructor_id;
			$eventUID = 'ExpertusONE_' . $classSessionInfo->class_id . '_' . $classSessionInfo->id . '_' . $instructorId. '@' . $_SERVER['SERVER_NAME'];
			//$eventUID = 'ExpertusONE_' . $classSessionInfo->class_id . '_' . $classSessionInfo->id .'@' . $_SERVER['SERVER_NAME'];

			// Prepare event summary
			$summary = t('Canceled', array(), array('langcode' => $preLanguage)) . ': ';
			$summary .= $classInfo['title'] . ((count($classSessionsInfoList) <= 1)? "" : " -  $classSessionInfo->title");
			$utcSystemDatetime = date_format(date_now('UTC'), 'Ymd\THis\Z');
			list($utcStartDatetime, $utcEndDatetime) = prepareICalEventDateTime($classSessionInfo->start_date, $classSessionInfo->start_time,
					$classSessionInfo->end_time, $classSessionInfo->session_timezone);
			$vEvent = '';
			$vEvent .= "BEGIN:VEVENT\r\n";
			$vEvent .= "DTSTART:".$utcStartDatetime."\r\n";
			$vEvent .= "DTEND:" . $utcEndDatetime . "\r\n";
			$vEvent .= "ORGANIZER;CN=" . $organizerName . ":MAILTO:" . $organizerEmail . "\r\n"; //ORGANIZER;CN=Me@ExpertusGMAIL:mailto:sunilkumarg@expertus.com
			$vEvent .= "ATTENDEE;CUTYPE=INDIVIDUAL;ROLE=REQ-PARTICIPANT;PARTSTAT=ACCEPTED:mailto:" . $classInstructorInfo['email'] . "\r\n";
			$vEvent .= "UID:" . $eventUID . "\r\n";
			$vEvent .= "SEQUENCE:" . $classSessionInfo->seq . "\r\n"; // revision number
			$vEvent .= "PRIORITY:1\r\n"; // highest priority
			$vEvent .= "STATUS:CANCELLED\r\n";
			$vEvent .= "SUMMARY: " . $summary . "\r\n";
			$vEvent .= "TRANSP:TRANSPARENT\r\n";
			$vEvent .= "CLASS:PUBLIC\r\n";
			$vEvent .= "X-MICROSOFT-CDO-BUSYSTATUS:FREE\r\n";
			$vEvent .= "X-MICROSOFT-CDO-IMPORTANCE:2\r\n";
			$vEvent .= "X-MICROSOFT-DISALLOW-COUNTER:TRUE\r\n";
			$vEvent .= "X-MS-OLK-AUTOSTARTCHECK:FALSE\r\n";
			$vEvent .= "X-MS-OLK-AUTOFILLLOCATION:FALSE\n"; // http://www.how2guru.com/archives/php-generate-calendar-file-ics/
			$vEvent .= "X-MS-OLK-CONFTYPE:0\r\n";
			$vEvent .= "X-MS-OLK-APPTSEQTIME:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "DTSTAMP:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "END:VEVENT\r\n";
			$vEventsArr[] = $vEvent;
		} // end foeach
		expDebug::dPrint('$vEventsArr = ' . print_r($vEventsArr, true), 4);

		return $vEventsArr;
	}
	catch (Exception $ex) {
		watchdog_exception('getCancelICalEventsInstructor', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getClassMultiSessionInfoForNotifications() - Get class multiple session information for notifications
 */
function getClassMultiSessionInfoForNotifications($courseId, $classId, $delType, $userId = '', $addUserTZ = false) {
  /* Derive timezone attr of session from slt_profile_list_items table*/
  // Select table
  try {
  $selectStmt = db_select('slt_course_class_session', 'ses');
  $selectStmt->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=ses.timezone');
  // Set conditions
  $selectStmt->condition('ses.course_id', $courseId);
  $selectStmt->condition('ses.class_id', $classId);
  // Join other tables.
  $selectStmt->leftJoin('slt_location', 'loc', 'ses.location_id = loc.id');
  // Select fields and/or add expressions
  $selectStmt->addField('loc', 'name', 'location_name');
  $selectStmt->addField('ses', 'id', 'id');
  $selectStmt->addField('ses', 'start_date', 'start_date');
  $selectStmt->addField('ses', 'end_date', 'end_date');
  $selectStmt->addField('ses', 'timezone', 'timezone');
  $selectStmt->addField('ses', 'capacity_max', 'capacity_max');
  $selectStmt->addField('ses', 'title', 'title');
  $selectStmt->addField('ses', 'start_time', 'start_time');
  $selectStmt->addField('ses', 'end_time', 'end_time');
  $selectStmt->addField('splt_timezone','attr2', 'session_timezone');
  $selectStmt->addField('splt_timezone','name', 'session_timezone_name');
  expDebug::dPrintDBAPI(' $selectStmt = '  , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  $sessionList =  $result->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  expDebug::dPrint(' $$sessionList = ' . print_r($sessionList, true) , 3);
  $classSessionInfo1 = array();
  $sessionLocation = '';
  $classSessionInfo = '';
  $sessionTimeZone = '';
  $sessionTimeZoneDisp = '';
  foreach ($sessionList as $session) {
  	$ses_start_date_form     = explode(" ",$session['start_date']);
  	$ses_start_date_format   = $ses_start_date_form[0]." ".$session['start_time'];
  	$ses_end_date_form       = explode(" ",$session['start_date']);
  	$ses_end_date_format     = $ses_end_date_form[0]." ".$session['end_time'];
  	
  	$ilt_ses_start_date_format = $ses_start_date_format;
  	$ilt_ses_end_date_format		= $ses_end_date_format;
  	$currentDate;
  	if($userId == ''){
  	$userId = getSltpersonUserId();
  	}
  	$timezone_details = getPersonDetails($userId);
  	$vTimeZone	= $session['session_timezone']; //($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone;
  
  	$vStartDateTime = $ses_start_date_format; //date_format(date_create($ses_start_date_format),'Y-m-d H:i');
  	$vEndDateTime   = $ses_end_date_format; //date_format(date_create($ses_end_date_format),'Y-m-d H:i');
  	if($userId){
  		$UserPHPTimezoneName		= getUserPHPTimezoneName($userId);
  		$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
  		if(!$UserPHPTimezoneName[0]->zone_name){
  			$vUserPHPTimezonename = date_default_timezone(FALSE);
  		}
  	}else{
  		$vUserPHPTimezonename = date_default_timezone(FALSE);
  	}
  
  	$oStartDateTime  = timeZoneConvert($vStartDateTime , $vTimeZone,  $vUserPHPTimezonename);
  	$ses_start_date_format 	= date_format($oStartDateTime,'Y-m-d H:i');
  	$oEndDateTime    = timeZoneConvert($vEndDateTime, $vTimeZone,  $vUserPHPTimezonename);
  	$ses_end_date_format 	= date_format($oEndDateTime,'Y-m-d H:i');
  	$start_time = date_format($oStartDateTime,'H:i');
  	$end_time = date_format($oEndDateTime,'H:i');
  	$session_start = date_format(date_create($ses_start_date_format),'Y-m-d')." 00:00:00";
  	$session_day = ($session['start_time'] != null ) ? date_format(date_create($ses_start_date_format),'D') : '';
  	$sessionTZ = new DateTimeZone($vUserPHPTimezonename);
  	$currentDate = new DateTime(null,$sessionTZ) ;
  	
  	$session_start_date_format  = date_format(date_create($ses_start_date_format),'M d, Y');
  	$session_end_date_format    = date_format(date_create($ses_end_date_format),'M d, Y');
  	$session_start_time_format  = date_format(date_create($ses_start_date_format),'h:i');
  	$session_start_end_format   =date_format(date_create($ses_end_date_format),'h:i');
  	$session_start_time_form    = date_format(date_create($ses_start_date_format),'A');
  	$session_end_time_form      = date_format(date_create($ses_end_date_format),'A');
  	$startformat = $session_start_time_format.' '.$session_start_time_form;
  	$endformat = $session_start_end_format.' '.$session_end_time_form;
  	$usertimezone =  convertNullValToNullStr($UserPHPTimezoneName[0]->timezone_name);
  	$sessionInUserTimeZoneDisp = substr($usertimezone,stripos($usertimezone,')')+2);
  	
        $sessionTitle = ($session['title'] == null)? ' ' : $session['title'];
        $sessionStartDate = dateTimeStrToUSShort($session['start_date'], true, true, true, false);
        $sessionStartDate = convertNullValToNullStr($sessionStartDate);
        $sessionEndDate   = dateTimeStrToUSShort($session['end_date'], true, true, true, false);
        $sessionEndDate = convertNullValToNullStr($sessionEndDate);
        $sessionStartTime = ($session['start_time'] != '' && $session['start_time'] != null)?date_format(date_create($session['start_time']),'g:i A'):'';
        $sessionEndTime   = ($session['end_time'] != '' && $session['end_time'] != null)?date_format(date_create($session['end_time']),'g:i A'):'';
        $sessionLocation  = $session['location_name'];
		
		if(!empty($sessionLocation)) {
		$LocationDel = ClassLocationDetails($classId,$sessionLocation);
		if(count($LocationDel) < 0)  $LocationDel = $sessionLocation;
		} else $LocationDel = ' - ';
		 
        $sessionLocation = convertNullValToNullStr($sessionLocation);
        $sessionTimeZone = convertNullValToNullStr($session['session_timezone_name']);
    if(!empty($sessionTimeZone))
    		$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
    if($addUserTZ) {
    $classSessionInfo =	$classSessionInfo .
                      'session_name<|' . $sessionTitle . '~~' .
    	'session_startdate<|' . ('\', date_format(CONVERT_TZ(concat(date_format(\''.$session['start_date'].'\',\'%Y-%m-%d\'),\' \',\''.$session['start_time'].'\'),\''.$session['session_timezone'].'\', pro.attr2),\'%b %d, %Y\'), \'') . '~~' .
    	'session_enddate<|' . ('\', date_format(CONVERT_TZ(concat(date_format(\''.$session['start_date'].'\',\'%Y-%m-%d\'),\' \',\''.$session['end_time'].'\'),\''.$session['session_timezone'].'\', pro.attr2),\'%b %d, %Y\'), \'')  . '~~' .
    	'session_starttime<|' . ('\', date_format(CONVERT_TZ(concat(date_format(\''.$session['start_date'].'\',\'%Y-%m-%d\'),\' \',\''.$session['start_time'].'\'),\''.$session['session_timezone'].'\', pro.attr2),\'%h:%i %p\'), \'')  . '~~' .
    	'session_endtime<|' . ('\', date_format(CONVERT_TZ(concat(date_format(\''.$session['start_date'].'\',\'%Y-%m-%d\'),\' \',\''.$session['end_time'].'\'),\''.$session['session_timezone'].'\', pro.attr2),\'%h:%i %p\'), \'')  .  '~~' .
    	'session_location<|' . $sessionLocation . '~~' .
    	'time_zone<|' . ('\', SUBSTRING_INDEX(pro.name,\') \',-1), \'').
    	'~@';
    } else {
    $classSessionInfo =	$classSessionInfo .
                      'session_name<|' . $sessionTitle . '~~' .
                      'session_startdate<|' . $session_start_date_format . '~~' .
    				  'session_enddate<|' . $session_end_date_format . '~~' .
  					  'session_starttime<|' . $startformat . '~~' .
  					  'session_endtime<|' . $endformat .  '~~' .
    				  'session_location<|' . $LocationDel . '~~' .
                     'time_zone<|' . $sessionInUserTimeZoneDisp .
  					  '~@';
    				//'time_zone<|' . (!empty($sessionInUserTimeZoneDisp)?$sessionInUserTimeZoneDisp:$sessionTimeZoneDisp) .    $sessionInUserTimeZoneDisp
    }         
  } // end foreach
  //expDebug::dPrint(' $classSessionInfo = ' . print_r($classSessionInfo, true) , 3);
  $classSessionInfo1[0] = $classSessionInfo;
  $classSessionInfo1[1] = $sessionLocation;
  $classSessionInfo1[2] = $sessionTimeZone;
  return $classSessionInfo1;
}catch (Exception $ex) {
    watchdog_exception('getClassMultiSessionInfoForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getClassLaunchUrl() - Get class launch URL
 */
function getClassLaunchUrl($courseId, $classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_content_mapper', 'content');
  // Set conditions
  $selectStmt->condition('content.course_id', $courseId);
  $selectStmt->condition('content.class_id', $classId);
  // Select fields and/or add expressions
  $selectStmt->addField('content', 'launch_url', 'launch_url');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  // expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getClassLaunchUrl', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * generateClassMaxCapacityReachedNotifications() - Generates notifications for ClassMaxCapacityReached
 */
function generateClassMaxCapacityReachedNotifications($notificationType, $courseId, $classId, $userId) {
  try{
  // Get class details
  $classInfo = getClassInfoForNotifications($classId);
  $courseInfo =   getCourseInfoForNotifications($courseId);
  $deliveryTypeName = '';
  $classTitle = '';
  $classCode = '';
  $classDesc= '';
  if ($classInfo != null) {
    $deliveryTypeName = convertNullValToNullStr($classInfo['delivery_type_name']);
    $classTitle = convertNullValToNullStr($classInfo['title']);
    $classCode = convertNullValToNullStr($classInfo['code']);
	$classDesc = convertNullValToNullStr($classInfo['description']);
	$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
    $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
}
  if($courseInfo != null) {
    $courseTitle = convertNullValToNullStr($courseInfo['title']);
  }
  $classMaxCapacity = getClassMaxCapacity($courseId, $classId);
  // Get class delivery type
  $classDeliveryType = getClassDeliveryType($classId, 'Short');
  $classLocationName = '';
  if ($classDeliveryType == 'ilt') {
  	$classLocationId = getLocationIdOfClass($courseId, $classId);
  	if (!empty($classLocationId)) {
  		$classLocationDetails = getLocationDetails($classLocationId, array('name'));
  		$classLocationName = empty($classLocationDetails['name'])?'':$classLocationDetails['name'];
  	}
	if(!empty($classLocationName)) {
	$LocationDel = ClassLocationDetails($classId,$classLocationName);
	if(count($LocationDel) < 0)  $LocationDel = $classLocationName;
	} else $LocationDel = ' - ';

  }
  $notificationInfo = array();
  $notificationInfo['tokens_string'] = 'delivery_type>|' . $deliveryTypeName . '~|' .
                      'course_title>|' . $courseTitle . '~|' .
                      'class_title>|' . $classTitle . '~|' .
                    	'class_description>|' . $classDesc . '~|' .	
					  
                      'course_code>|' . $classCode . '~|' .
                      'seat_count>|' . $classMaxCapacity . '~|' .
                      'session_location>|' . $LocationDel . '~|'.
(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';  
  $notificationInfo['message_id'] = 'class_max_capacity_reached';
  $notificationInfo['message_type'] = 'Class Max Capacity Reached';
  $notificationInfo['send_type'] = 'php mailer';
  $notificationInfo['lang_code'] = $classInfo['lang_code'];
  $notificationInfo['location_id'] = $classLocationId;
  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateClassMaxCapacityReachedNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
// Function used to get Payment type. added 25/07/2014 yogaraja.
function getOrderTypeForCancelClass($courseId, $classId, $userId){
	try{
	$select = db_select('slt_enrollment', 'enr');
	$select->leftjoin('slt_order', 'ord','enr.order_id = ord.id');
	$select->addField('ord','order_type');
	$select->condition('enr.course_id', $courseId, '=');
	$select->condition('enr.class_id', $classId, '=');
	$select->condition('enr.user_id', $userId, '=');
	$select->condition('enr.master_enrollment_id', '', 'IS NULL');
	expDebug::dPrintDBAPI('$select notification get order type--->', $select);
	$orderDetails = $select->execute()->fetch();
	if(!empty($orderDetails))
		return $orderDetails->order_type;
	return false;
	}catch (Exception $ex) {
    watchdog_exception('getOrderTypeForCancelClass', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * generateRegisterAndWaitlistNotifications() - Generates notifications for
 * a. Register, b. Waitlist, c. WaitlistPromotion, d. WaitlistAllocation, c. WaitlistLocationAdmin
 */
function generateRegisterAndWaitlistNotifications($notificationType, $courseId, $classId, $userId) {
  try{
	  // Get person details
	  $personInfo = getPersonDetailsForNotifications($userId, true);
	  $firstName = '';
	  $lastName = '';
	  $fullName = '';
	  $userName = '';
	  $userEmail = '';
	  $userPhone = '';
	  $lang_code ='en-us';
  if ($personInfo != null) {
    $firstName = convertNullValToNullStr($personInfo['first_name']);
    $lastName = convertNullValToNullStr($personInfo['last_name']);
    $fullName = convertNullValToNullStr($personInfo['full_name']);
    $userName = convertNullValToNullStr($personInfo['user_name']);
    $userEmail = convertNullValToNullStr($personInfo['email']);
    $userPhone = convertNullValToNullStr($personInfo['phone_no']);
    $lang_code = $personInfo['drupal_lang_code'];
  }

  // Get class details
	  $classInfo = getClassInfoForNotifications($classId);
	  $courseInfo =   getCourseInfoForNotifications($courseId);
	  $completecourseInfo =   getCourseInfoForCompleteNotifications($courseId,$userId);
	  $tpInfo = getTpInfoForNotifications($courseId);
	  $multipleInfo = multipleComplianceMandatoryClassNotification($courseId);
	  $enrollInfo = getEnrollInfoForNotifications($courseId, $classId, $userId);
	  $deliveryTypeName = '';
	  $classTitle = '';
	  $classCode = '';
	  $classDesc = '';
	  $tpDesc = '';
  if ($classInfo != null) {
    $deliveryTypeName = convertNullValToNullStr($classInfo['delivery_type_name']);
    $classTitle = convertNullValToNullStr($classInfo['title']);
    $classCode  = convertNullValToNullStr($classInfo['code']);
	$classDesc = convertNullValToNullStr($classInfo['description']);
	$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
    $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
  }
  if($courseInfo != null) {
    $courseTitle = convertNullValToNullStr($courseInfo['title']);
    $complete_date  = convertNullValToNullStr($courseInfo['complete_date']);
    $complete_days  = convertNullValToNullStr($courseInfo['complete_days']);
    $courseCode = convertNullValToNullStr($courseInfo['code']);
  }
  if($completecourseInfo != null) {
  	$completecourseTitle = convertNullValToNullStr($completecourseInfo['title']);
  	$completecomplete_date  = convertNullValToNullStr($completecourseInfo['complete_date']);
  	$completecomplete_days  = convertNullValToNullStr($completecourseInfo['complete_days']);
  	$completecourseCode = convertNullValToNullStr($completecourseInfo['code']);
  }
  if($tpInfo != null) {
  	$tptitle = convertNullValToNullStr($tpInfo['title']);
  	$tpCode  = convertNullValToNullStr($tpInfo['code']);
	$tpDesc  = convertNullValToNullStr($tpInfo['description']);
  	expDebug::dPrint("class details ".print_r($tpInfo,true),4);
  }
  $complianceStack = array('RegisterMandatoryCompliance','ComplianceClassReminder','ComplianceClassIncomplete','RegisterVCCompliance','RegisterWBTCompliance','RegisterMandatoryComplianceByAdmin','RegisterVCComplianceByAdmin','RegisterWBTComplianceByAdmin');
  $is_compliance = in_array($notificationType,$complianceStack) ? 1 : 0;
  $isMandatory=getIfCourseMandatory($courseId,$userId,$classId);
  $is_compliance_noti = (($isMandatory == 'Y') ? t('Mandatory',array(),array('langcode'=>$lang_code)) : (($is_compliance == 1) ? t('Compliance',array(),array('langcode'=>$lang_code)) : ''));
  $notificationInfo = array();
  $notificationInfo['tokens_string'] =	'first_name>|' . $firstName . '~|' .
  										'last_name>|' . $lastName . '~|' .
  										'delivery_type>|' . $deliveryTypeName . '~|' .
  										'course_title>|'. $courseTitle . '~|' .
  										'class_title>|' . $classTitle . '~|' .
  										'class_description>|' . $classDesc . '~|' .
                                        'full_name>|' . $fullName . '~|' .
                                        'learning_fullname>|' . $fullName . '~|' .
    									'user_name>|' . $userName . '~|' .
  										'user_email>|' . $userEmail . '~|' .
                                        'user_phone>|' . $userPhone . '~|'.
  										'waitlist_position>|' . $enrollInfo['waitlist_position'] . '~|'.
  									    'compliance_mandatory>|' . $is_compliance_noti . '~|'.
											(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';
  // ComplianceClassReminder
  // Get class delivery type
  $classDeliveryType = getClassDeliveryType($classId, 'Short');
  if ($classDeliveryType == 'ilt' || $classDeliveryType == 'vc') {
    // Get class session details
	/* Added extra param = class del type attr*/
    $classSessionInfo = getClassMultiSessionInfoForNotifications($courseId, $classId,$classDeliveryType,$userId);
    $classInstructorInfo  = getClassInstructorInfoForNotifications($classId);
    $timeZone = '';
    $classSessionInformation = $classSessionInfo[0];
    if ($classSessionInformation == null) {
      $classSessionInformation = 'session_name<| ~~session_startdate<| ~~session_enddate<| ~~session_starttime<| ~~session_endtime<| ~~time_zone<| ~@';
    }

    $sessionLocation = $classSessionInfo[1];
    if ($sessionLocation == null) {
      $LocationDel = ' - ';
    } else { 
	$LocationDel = ClassLocationDetails($classId,$sessionLocation);
	if(count($LocationDel) < 0)  $LocationDel = $sessionLocation;
    }
    // Append tokens string with class session details
	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
										  'session_location>|' . $LocationDel . '~|' .
	  									  'group_sessiondata>|' . $classSessionInformation . '~|' .
                        'group_instructordata>|' . $classInstructorInfo ;

	// For vc class append timezone to location
	if ($classDeliveryType == 'vc') {
		$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] .
											 ' (' . $timeZone . ') ';
	}
	$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|';
  }
  else {
    // Append launch url for wbt class to the tokens string
    $launchUrl = convertNullValToNullStr(getClassLaunchUrl($courseId, $classId));
    $notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] .
    									 'launch_url>|' . $launchUrl . '~|';
  }
  $notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] .
  									   'dt_code>|' . $classDeliveryType. '~|';

  switch ($notificationType) {
    case 'Register':
            switch ($classDeliveryType) {
              case 'wbt':
              case 'vod':
                $notificationInfo['message_id'] = 'register_wbt';
            	  $notificationInfo['message_type'] = 'Course Register Wbt';
                break;
              case 'vc':
                $notificationInfo['message_id'] = 'register_vc';
            	  $notificationInfo['message_type'] = 'Course Register VC';
                break;
              default:
                $notificationInfo['message_id'] = 'register';
            	  $notificationInfo['message_type'] = 'Course Register';
                break;
            }
            break;
            case 'RegisterByAdmin':

            	switch ($classDeliveryType) {
            		case 'wbt':
            		case 'vod':
            			if($is_compliance == 1){            			
            			$notificationInfo['message_id'] = 'compliance_register_wbt_by_admin';
            			$notificationInfo['message_type'] = 'Course Register Mandatory/Compliance by admin';
            			}
            			else {            			
            			$notificationInfo['message_id'] = 'register_wbt_by_admin';
            			$notificationInfo['message_type'] = 'Course Register Wbt by an admin';
            			} 
            			break;
            		case 'vc':
            			if($is_compliance == 1){
            				$notificationInfo['message_id'] = 'compliance_register_vc_by_admin';
            				$notificationInfo['message_type'] = 'Course Register Mandatory/Compliance by admin';
            			}
            			else {
            			$notificationInfo['message_id'] = 'register_vc_by_admin';
            			$notificationInfo['message_type'] = 'Course Register VC by an admin';
            			}
            			break;
            		default:
            			if($is_compliance == 1){
            				$notificationInfo['message_id'] = 'register_mandatory_compliance_by_admin';
            				$notificationInfo['message_type'] = 'Course Register Mandatory/Compliance by admin';
            			}
            			else {
            			$notificationInfo['message_id'] = 'register_by_admin';
            			$notificationInfo['message_type'] = 'Course Register by an admin';
            			}
            			break;
            	}
            	break;            
            
    case 'RegisterMandatoryCompliance':
			  $check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);
              $notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'register_mandatory_compliance' : 'compliance_course_re_register';
          	  $notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course Register Compliance' : 'Compliance ILT Course Re-Register';
        break;
    case 'RegisterVCCompliance':
			  $check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);
              $notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'compliance_register_vc' : 'compliance_course_re_register';
          	  $notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course VC Register Compliance' : 'Compliance VC Course Re-Register';
        break;
    case 'RegisterWBTCompliance':
			  $check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);			  
              $notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'compliance_register_wbt' : 'compliance_course_re_register';
          	  $notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course WBT Register Compliance' : 'Compliance WBT Course Re-Register';
        break;
        
        
    case 'RegisterMandatoryComplianceByAdmin':
        	$check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);
        	$notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'register_mandatory_compliance_by_admin' : 'compliance_course_re_register_by_admin';
        	$notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course Register Compliance by an admin' : 'Compliance ILT Course Re-Register';
        	break;
    case 'RegisterVCComplianceByAdmin':
        	$check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);
        	$notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'compliance_register_vc_by_admin' : 'compliance_course_re_register_by_admin';
        	$notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course VC Register Compliance by an admin' : 'Compliance VC Course Re-Register';
        	break;
     case 'RegisterWBTComplianceByAdmin':
        	$check_compliance_enrollment 		= compliance_class_enrollment($userId,$courseId,$classId,true);
        	$notificationInfo['message_id'] 	= ($check_compliance_enrollment == 0) ? 'compliance_register_wbt_by_admin' : 'compliance_course_re_register_by_admin';
        	$notificationInfo['message_type'] = ($check_compliance_enrollment == 0) ? 'Course WBT Register Compliance by an admin' : 'Compliance WBT Course Re-Register by an Admin';
        	break;
        	
     case 'ComplianceClassReminder':
				    	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				    	'course_code>|' . $classCode . '~|';
				    	$classtype = 'Compliance';
				    	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				    	'compliance_mandatory>|' . $classtype . '~|';
				    	if($completecomplete_date != ""){
				    		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				    		'complete_by_date>|' . $completecomplete_date . '~|';
				    	}elseif($completecomplete_days != ""){
				    		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				    		'complete_by_date>|' . $completecomplete_days . '~|';
				    	}else{
				    		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
				    		'complete_by_date>|-~|';
				    	}

              $notificationInfo['message_id'] 	= 'compliance_class_remind_session';
          	  $notificationInfo['message_type'] = ' Compliance Course class Reminder';
        break;

        case 'MandatoryClassReminder' :
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'course_code>|' . $classCode . '~|';
        	$classtype = 'Mandatory';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'compliance_mandatory>|' . $classtype . '~|';
        	$notificationInfo['message_id'] 	= 'mandatory_class_remind_session';
        	$notificationInfo['message_type'] = ' Mandatory Course class Reminder';
        	break;
        case 'MandatoryTPReminder' :
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'course_title>|' . $tptitle . '~|';
			$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
			'tp_description>|' . $tpDesc . '~|';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'course_code>|' . $tpCode . '~|';
        	$classtype = 'Mandatory';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'compliance_mandatory>|' . $classtype . '~|';
        	$notificationInfo['message_id'] 	= 'mandatory_class_remind_session';
        	$notificationInfo['message_type'] = ' Mandatory TP Reminder';
        	break;

        case 'MultipleComplianceClassNotification' :
        	$classtype = 'Compliance';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'compliance_mandatory>|' . $classtype . '~|';

        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'course_code>|' . $courseCode . '~|';
        	expDebug::dPrint("fsdf sddate ".$courseCode);

        	if($complete_date != ""){
        		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        		'complete_by_date>|' . $complete_date . '~|';
        	}else{
        		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        		'complete_by_date>|' . $complete_days . '~|';
        	}

        	$classtittle = '';
        	$classcode = '';
        	$deleverytype = '';
        	$sessionlocation = '';
        	$session_startdate = '';
        	$i = 0;
        	foreach ($multipleInfo as $classvalues){
        		$classtittle = $classvalues->classtittle;
        		$classcode = $classvalues->classcode;
        		$deleverytype = $classvalues->deleverytype;
				$clsId = $classvalues->classid;

        		$sessionlocation = $classvalues->location;
        		$session_startdate = $classvalues->startdate;

				if(!empty($sessionLocation)) {
				$LocationDel = ClassLocationDetails($clsId,$sessionlocation);
				if(count($LocationDel) < 0) $LocationDel = $sessionLocation;
				} else $LocationDel = ' - ';
        		$classdetails .= 'class_title<|' . $classtittle . '~~' .
        				'class_code<|' . $classcode . '~~' .
        				'delivery_type<|' . $deleverytype . '~~' .
        				'session_location<|' .$LocationDel . '~~' .
        				'session_startdate<|' . $session_startdate . '~@';
        		$i++;
        	}
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'group_data>|' . $classdetails . '~|';


        	$notificationInfo['message_id'] 	= 'man_comp_multiple_class_register';
        	$notificationInfo['message_type'] = ' Multiple Compliance/Mandatory Class Notification';
        	break;
        case 'MultipleMandatoryClassNotification' :
        	$classtype = 'Mandatory';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'compliance_mandatory>|' . $classtype . '~|';

        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'course_code>|' . $courseCode . '~|';
        	expDebug::dPrint("fsdf sddate ".$complete_date);

        	if($complete_date != ""){
        		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        		'complete_by_date>|' . $complete_date . '~|';
        	}else{
        		$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        		'complete_by_date>|' . $complete_days . '~|';
        	}

        	$classtittle = '';
        	$classcode = '';
        	$deleverytype = '';
        	$sessionlocation = '';
        	$session_startdate = '';
        	$i = 0;
        	foreach ($multipleInfo as $classvalues){
        		$classtittle = $classvalues->classtittle;
        		$classcode = $classvalues->classcode;
        		$deleverytype = $classvalues->deleverytype;
        		$sessionlocation = $classvalues->location;
        		$session_startdate = $classvalues->startdate;
				$clsId = $classvalues->classid;
				if(!empty($sessionLocation)) {
				$LocationDel = ClassLocationDetails($clsId,$sessionlocation);
				if(count($LocationDel) < 0) $LocationDel = $sessionLocation;
				} else $LocationDel = ' - ';
        		$classdetails .= 'class_title<|' . $classtittle . '~~' .
        				'class_code<|' . $classcode . '~~' .
        				'delivery_type<|' . $deleverytype . '~~' .
        				'session_location<|' .$LocationDel . '~~' .
        				'session_startdate<|' . $session_startdate . '~@';
        		$i++;
        	}
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'group_data>|' . $classdetails . '~|';


        	$notificationInfo['message_id'] 	= 'man_comp_multiple_class_register';
        	$notificationInfo['message_type'] = ' Multiple Compliance/Mandatory Class Notification';
        	break;
        case 'MultipleMantadoryTPNotification' :
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'tp_title>|' . $tptitle . '~|';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'tp_code>|' . $tpCode . '~|';
			$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'tp_description>|' . $tpDesc . '~|';
        	$classtype = 'Mandatory';
        	$notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
        	'compliance_mandatory>|' . $classtype . '~|';
        	$notificationInfo['message_id'] 	= 'mandatory_tp_multiple_register';
        	$notificationInfo['message_type'] = ' Mandatory TP Multiple Register';
        	break;
    case 'ComplianceClassIncomplete':
              $notificationInfo['message_id'] 	= 'compliance_class_incomplete';
          	  $notificationInfo['message_type'] = ' Compliance Course class Incomplete Reminder';
        break;
    case 'Waitlist':
              $notificationInfo['message_id'] = 'waitlist';
          	  $notificationInfo['message_type'] = 'Waitlist Confirmation';
          	  break;
    case 'WaitlistByAdmin':
       	  	  $notificationInfo['message_id'] = 'waitlist_by_admin';
          	  $notificationInfo['message_type'] = 'Waitlist Confirmation by an Admin';
          	  break;          	  
    case 'CancelClassInstructor':
              $timeZone = convertNullValToNullStr($classSessionInfo[2]);
              $notificationInfo['tokens_string']  = $notificationInfo['tokens_string'] .
										  		'course_code>|' . $classCode . '~|' .
          										'session_timezone>|' . $timeZone . '~|' ;
              $notificationInfo['message_id'] = 'class_cancel_to_instuctor';
          	  $notificationInfo['message_type'] = 'Class cancel instructor';
          	  break;
    case 'WaitlistPromotion':
              // Get count of confirmed enrollments for the class
              $regStatusesList = array('lrn_crs_reg_cnf');
              $confirmedEnrollmentCount =  getClassEnrollmentCountForRegistrationStatuses(null, $classId, $regStatusesList);
              expDebug::dPrint(' WaitlistPromotion $confirmedEnrollmentCount = ' . print_r($confirmedEnrollmentCount, true) , 4);
              $maxCapacity = '';
              if ($classSessionInfo != null) {
               // $maxCapacity = convertNullValToNullStr($classSessionInfo['capacity_max']); commented For empty the capacity_max
              }

              // Compute available seat count
              $availableSeatCount = $maxCapacity - $confirmedEnrollmentCount;

              // Append tokens string to include available seat count
              $notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|' .
                                                   'seat_count>|' . $availableSeatCount;

              $notificationInfo['message_id'] = 'waitlist_promo';
          	  $notificationInfo['message_type'] = 'Automatic Waitlist Promotion';
          	  break;

    case 'WaitlistAllocated':
              $notificationInfo['message_id'] = 'seat_occupied_for_waitlist';
          	  $notificationInfo['message_type'] = 'Seat Occupied for Waitlisted learners';
          	  break;
    case 'WaitlistExpired':
              $notificationInfo['message_id'] = 'waitlist_cancellation';
          	  $notificationInfo['message_type'] = 'Waitlist seat automatically cancelled';
          	  //0034090: Feedback -- Cancellation of waitlisted seats for priced class notification modification
          	  $ordertype = getOrderTypeForCancelClass($courseId, $classId, $userId);
          	  if($ordertype!='credit' && $ordertype!=false)
          	  {
          	  	// Append tokens string to include Payment details
          	  	$notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|' .
          	  			'payment_details>|' . t('For details regarding payment, contact Support');
          	  }
            break;
    case 'WaitlistDrop':
              $notificationInfo['message_id'] = 'learner_drop_waitlist';
          	  $notificationInfo['message_type'] = 'Learner Drop waitlist seat';
            break;
    case 'WaitlistLocationAdmin':
              // Get organization details
              $organizationInfo = getOrganizationInfoForNotifications($personInfo['org_id']);
              expDebug::dPrint(' $organizationInfo = ' . print_r($organizationInfo, true) , 4);

              $organizationName = '';
              $organizationAddressLine1 = '';
              $organizationAddressLine2 = '';
              $organizationCity = '';
              $organizationState = '';
              $organizationZip = '';
              $organizationCountry = '';
              $organizationPhone = '';
              $organizationEmail = '';
              $organizationFax = '';

              if ($organizationInfo != null) {
                  $organizationName = convertNullValToNullStr($organizationInfo['name']);
                  $organizationAddressLine1 = convertNullValToNullStr($organizationInfo['add1']);
                  $organizationAddressLine2 = convertNullValToNullStr($organizationInfo['add2']);
                  $organizationCity = convertNullValToNullStr($organizationInfo['city']);
                  $organizationState = convertNullValToNullStr($organizationInfo['state']);
                  $organizationZip = convertNullValToNullStr($organizationInfo['zip']);
                  $organizationCountry = convertNullValToNullStr($organizationInfo['country_name']);
                  $organizationPhone = convertNullValToNullStr($organizationInfo['phone_no']);
                  $organizationEmail = convertNullValToNullStr($organizationInfo['email']);
                  $organizationFax = convertNullValToNullStr($organizationInfo['fax_no']);
              }

              // Append organization details to the tokens string
              $notificationInfo['tokens_string'] = $notificationInfo['tokens_string'] . '~|' .
                                                   'org_name>|' . $organizationName . '~|' .
                                                   'org_add1>|' . $organizationAddressLine1 . '~|' .
                                                   'org_add2>|' . $organizationAddressLine2 . '~|' .
                                                   'org_city>|' . $organizationCity . '~|' .
                                                   'org_state>|' . $organizationState . '~|' .
                                                   'org_zip>|' . $organizationZip . '~|' .
                                                   'org_country>|' . $organizationCountry . '~|' .
                                                   'org_phone>|' . $organizationPhone . '~|' .
                                                   'user_email>|' . $organizationEmail . '~|' .
                                                   'org_fax>|' . $organizationFax;

              $notificationInfo['message_id'] = 'waitlist_location_admin';
          	  $notificationInfo['message_type'] = 'Waitlist Location Admin';
          	  break;
  } // End switch

  $notificationInfo['send_type'] = 'php mailer';
  $notificationInfo['lang_code'] = $classInfo['lang_code'];

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateRegisterAndWaitlistNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateDropAndCancelNotifications() - Generates notifications for
 * a. Drop, b. Cancel, c. ClassInActive, d. CancelLocationAdmin
 */
function generateDropAndCancelNotifications($notificationType, $enrollmentId, &$updateUId) { // identifies $userId from enrollment id and returns
  try{
  // Get Course id, Class id and the enrolled User id from slt_enrollment
  $enrollmentDetails = getEnrollmentDetailsForNotifications($enrollmentId);
  expDebug::dPrint(' $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4 );
  $userId = null;
  $courseId = null;
  $classId = null;
  $orderId = null;
  if ($enrollmentDetails != null) {
    $userId = $enrollmentDetails['user_id'];
    $courseId = $enrollmentDetails['course_id'];
    $classId = $enrollmentDetails['class_id'];
    $orderId = $enrollmentDetails['order_id'];
  }
  // Get class details
  $classInfo = getClassInfoForNotifications($classId);
  $courseInfo =   getCourseInfoForNotifications($courseId);
  expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true), 4);
  $deliveryTypeName = '';
  $classTitle = '';
  $classDesc= '';
  if ($classInfo != null) {
    $deliveryTypeName = convertNullValToNullStr($classInfo['delivery_type_name']);
    $classTitle = convertNullValToNullStr($classInfo['title']);
	$classDesc = convertNullValToNullStr($classInfo['description']);
$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
    $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);

	
  }
  if($courseInfo != null) {
    $courseTitle = convertNullValToNullStr($courseInfo['title']);
  }
  // Get class delivery type
  $classDeliveryType = getClassDeliveryType($classId, 'Short');
  expDebug::dPrint(' $classDeliveryType = ' . print_r($classDeliveryType, true) , 4 );
  // Get class session detals
  $classSessionInfo = getClassSessionInfoForNotifications($courseId, $classId);
  expDebug::dPrint(' $classSessionInfo = ' . print_r($classSessionInfo, true), 4);
  $sessionStartDate = '';
  $sessionEndDate = '';
  $locationName = '';
  $sessionTimeZone = '';
  $sessionTimeZoneDisp = '';
  if ($classSessionInfo != null) {
      $sessionStartDate = convertNullValToNullStr($classSessionInfo['start_date']);
      $sessionEndDate = convertNullValToNullStr($classSessionInfo['end_date']);
      $locationName = convertNullValToNullStr($classSessionInfo['location_name']);
      $sessionTimeZone = convertNullValToNullStr($classSessionInfo['session_timezone_name']);
    	$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
  }
  // Get person details
  $personInfo = getPersonDetailsForNotifications($userId);
  expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4);

  $firstName = '';
  $lastName = '';
  $fullName = '';
  $userName = '';
  $userEmail = '';
  $userPhone = '';
  if ($personInfo != null) {
    $firstName = convertNullValToNullStr($personInfo['first_name']);
    $lastName = convertNullValToNullStr($personInfo['last_name']);
    $fullName = convertNullValToNullStr($personInfo['full_name']);
    $userName = convertNullValToNullStr($personInfo['user_name']);
    $userEmail = convertNullValToNullStr($personInfo['email']);
    $userPhone = convertNullValToNullStr($personInfo['phone_no']);
  }
  $instructorInfo  = getClassInstructorInfoForNotifications($classId);
  if(!empty($locationName)) {
  	$LocationDel = ClassLocationDetails($classId,$locationName);
 	if(count($LocationDel) < 0)  $LocationDel = $locationName;
	} else $LocationDel = ' - ';
  // Prepare notification info
  $notificationInfo = array();
  $notificationInfo['tokens_string'] = 'delivery_type>|' . $deliveryTypeName . '~|' .
                                       'course_title>|'. $courseTitle . '~|' .
                                       'training_title>|'. $courseTitle . '~|' .
  									   'class_title>|' . $classTitle . '~|' .
  									   'class_description>|' . $classDesc . '~|' .
									   'dt_code>|' . $classDeliveryType . '~|' .
									   'session_startdate>|' . $sessionStartDate . '~|' .
									   'session_enddate>|' . $sessionEndDate . '~|' .
									   'session_location>|' . $LocationDel . '~|' .
  									 'time_zone>|' . $sessionTimeZoneDisp . '~|' .
									   'first_name>|' . $firstName . '~|' .
									   'last_name>|' . $lastName . '~|' .
   									   'full_name>|' . $fullName . '~|' .
                                       'learning_fullname>|' . $fullName . '~|' .
    								   'user_name>|' . $userName . '~|' .
  									   'user_email>|' . $userEmail . '~|' .
                                       'user_phone>|' . $userPhone. '~|' .
  									   'group_instructordata>|' . $instructorInfo . '~|' .
  									   (($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '');

  switch ($notificationType) {
    case 'Drop':
              $notificationInfo['message_id'] = 'drop_register';
         	  $notificationInfo['message_type'] = 'Learner Drop';
         	  break;
    case 'WBTDropped':
              $notificationInfo['message_id'] = 'drop_register_wbt_vod';
         	  $notificationInfo['message_type'] = 'WBT/VOD Learner Drop';
         	  break;
    case 'VCDropped':
              $notificationInfo['message_id'] = 'drop_vc_register';
         	  $notificationInfo['message_type'] = 'Learner Drop - VC';
         	  break;
    case 'ClsRejectByManager':
              $mangerInfo = getPersonDetailsForNotifications($updateUId);
              $fullName = convertNullValToNullStr($mangerInfo['full_name']);
              $notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
              										'manager_name>|' . $fullName;
              $notificationInfo['message_id'] = 'class_rejection_by_manager';
         	  $notificationInfo['message_type'] = 'Class Rejection By Manager';
         	  break;
    case 'ClsRejectByInstructor':
              $instructorInfo = getPersonDetailsForNotifications($updateUId);
              $fullName = convertNullValToNullStr($instructorInfo['full_name']);
              $notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
              										'instructor_name>|' . $fullName;
              $notificationInfo['message_id'] = 'class_rejection_by_instructor';
         	  $notificationInfo['message_type'] = 'Class Rejection By Instructor';
         	  break;
    case 'DropRefund':
              $notificationInfo['message_id'] = 'drop_class_register_refund';
         	  $notificationInfo['message_type'] = 'Learner Drop Class with refund';

         	  $selectStmt = db_select('slt_order_items', 'ord');
              $selectStmt->addField('ord','refund_amount', 'refund_amount');
              $selectStmt->addField('ord','tax_amount', 'tax_amount');
              $selectStmt->addField('ord','item_price', 'item_price');
              $selectStmt->condition('ord.user_id', $userId);
              $selectStmt->condition('ord.course_id', $courseId);
              $selectStmt->condition('ord.class_id', $classId);
              $selectStmt->condition('ord.order_id', $orderId);
              $selectStmt->range(0, 1);
              $result_amount =$selectStmt->execute()->fetchAll();

              $nodeId = getNodeIdResults($classId,'cre_sys_obt_cls');
              $ucOrderDtl = getUCOrderIdFromOrderId($orderId);

              $ucOrderId = $ucOrderDtl['uc_order_id'];
              $currency_type = $ucOrderDtl['currency_type'];
              // Commented for issues with the currency type if there is no currency in attr1.
              /*if(empty($currency_type))
              	$currency_type = "usd";*/

              $discountAmount = getDropListDiscountAmount($nodeId,$ucOrderId);

              $refund_amount = convertNullValToCurrency($result_amount[0]->refund_amount);
              $price  = convertNullValToCurrency($result_amount[0]->item_price);
              $tax_amount  = convertNullValToCurrency($result_amount[0]->tax_amount);
              $discountAmount = convertNullValToCurrency($discountAmount);
              $totalAmount = (($price - $discountAmount + $tax_amount) < 0) ? 0 : ($price - $discountAmount + $tax_amount);

              $discountAmount = formatAmount($discountAmount, false, $currency_type);
              $refund_amount = formatAmount($refund_amount, false, $currency_type);
              $tax_amount = formatAmount($tax_amount, false, $currency_type);
              $price = formatAmount($price, false, $currency_type);
              $totalAmount = formatAmount($totalAmount, false, $currency_type);

              // Append Order details to the tokens string
              $notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
              										'course_price>|' . $price . '~|' .
              										'tax_amount>|' . $tax_amount . '~|' .
              										'discount_amount>|' . $discountAmount . '~|' .
              										'total_amount>|' . $totalAmount . '~|' .
              										'refund_amount>|' . $refund_amount;
         	  break;
    case 'Cancel':
              $notificationInfo['message_id'] = 'class_cancel';
          	  $notificationInfo['message_type'] = 'Class Cancel By Admin';
          	  break;
    case 'WBTCancel':
              $notificationInfo['message_id'] = 'class_cancel_wbt_vod';
          	  $notificationInfo['message_type'] = 'WBT/VOD Cancel by Admin';
          	  break;

    case 'ClassInActive':
              $notificationInfo['message_id'] = 'class_inactive';
          	  $notificationInfo['message_type'] = 'Class InActive by Admin';
          	  break;

    case 'CancelLocationAdmin':
              // Get organization details
              $organizationInfo = getOrganizationInfoForNotifications($personInfo['org_id']);
              expDebug::dPrint('CancelLocationAdmin $organizationInfo = ' . print_r($organizationInfo, true) , 4);

              $organizationName = '';
              $organizationAddressLine1 = '';
              $organizationAddressLine2 = '';
              $organizationCity = '';
              $organizationState = '';
              $organizationZip = '';
              $organizationCountry = '';
              $organizationPhone = '';
              $organizationEmail = '';
              $organizationFax = '';

              if ($organizationInfo != null) {
                  $organizationName = convertNullValToNullStr($organizationInfo['name']);
                  $organizationAddressLine1 = convertNullValToNullStr($organizationInfo['add1']);
                  $organizationAddressLine2 = convertNullValToNullStr($organizationInfo['add2']);
                  $organizationCity = convertNullValToNullStr($organizationInfo['city']);
                  $organizationState = convertNullValToNullStr($organizationInfo['state']);
                  $organizationZip = convertNullValToNullStr($organizationInfo['zip']);
                  $organizationCountry = convertNullValToNullStr($organizationInfo['country_name']);
                  $organizationPhone = convertNullValToNullStr($organizationInfo['phone_no']);
                  $organizationEmail = convertNullValToNullStr($organizationInfo['email']);
                  $organizationFax = convertNullValToNullStr($organizationInfo['fax_no']);
              }

              // Append organization details to the tokens string
              $notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
              										'org_name>|' . $organizationName . '~|' .
              										'org_add1>|' . $organizationAddressLine1 . '~|' .
              										'org_add2>|' . $organizationAddressLine2 . '~|' .
              										'org_city>|' . $organizationCity . '~|' .
              										'org_state>|' . $organizationState . '~|' .
              										'org_zip>|' . $organizationZip . '~|' .
              										'org_country>|' . $organizationCountry . '~|' .
              										'org_phone>|' . $organizationPhone . '~|' .
              										'user_email>|' . $organizationEmail . '~|' .
              										'org_fax>|' . $organizationFax;
              $notificationInfo['message_id'] = 'cancel_location_admin';
          	  $notificationInfo['message_type'] = 'Cancel Location Admin';
          	  break;
  } // End switch

  $notificationInfo['send_type'] = 'php mailer';
  $notificationInfo['lang_code'] = $classInfo['lang_code'];

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateDropAndCancelNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCompletedNotification() - Generates notification for notification type 'Completed'.
 */
function generateCompletedNotification($enrollmentId, $classId, $userId,$updateUId = '',$notificationType = '') {
  try{
  // Get class title
  $classInfo = getClassInfoForNotifications($classId);
  expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 4);
  $classTitle = '';
  $classDesc = '';
  if ($classInfo != null) {
    $deliveryTypeName = convertNullValToNullStr($classInfo['delivery_type_name']);
    $classTitle = convertNullValToNullStr($classInfo['title']);
	$classDesc = convertNullValToNullStr($classInfo['description']);
    $classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
    $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
  }
  // Get completion status and completion date
  $enrollmentDetails = getEnrollmentDetailsForNotifications($enrollmentId);
  expDebug::dPrint(' $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);
  $comp_status = '';
  $comp_date = '';
  if ($enrollmentDetails != null && $enrollmentDetails['reg_status'] == 'lrn_crs_reg_cnf') {
    $comp_status = $enrollmentDetails['comp_status'];
              // dateTimeStrToUSShort($dateTimeString, $year4, $showTime, $time24, $showSeconds)
    $comp_date = dateTimeStrToUSShort($enrollmentDetails['comp_date'], true, false, true, false);
  }
  // Get person details
  $personInfo = getPersonDetailsForNotifications($userId);
  expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4);
  $firstName = '';
  $lastName = '';
  $email = '';
  $fullName = '';
  $userName = '';
  $userPhone = '';
  if ($personInfo != null) {
    $firstName = convertNullValToNullStr($personInfo['first_name']);
    $lastName = convertNullValToNullStr($personInfo['last_name']);
    $email = convertNullValToNullStr($personInfo['email']);
    $fullName = convertNullValToNullStr($personInfo['full_name']);
    $userName = convertNullValToNullStr($personInfo['user_name']);
    $userPhone = convertNullValToNullStr($personInfo['phone_no']);
  }

	$sel_surveymap = db_select('slt_survey_mapping','mapp');
	$sel_surveymap->addExpression('count(mapp.id)','rcount');
	$sel_surveymap->condition('mapp.object_id',$classId,'=');
	$sel_surveymap->condition('mapp.object_type','cre_sys_obt_cls','=');
	$result_surveymap = $sel_surveymap->execute()->fetchAssoc(); 
 	
 	$encrypted1 = new Encrypt();
 	$encrypted2 = new Encrypt();
 	
 	$objType = $encrypted2->encrypt('Class');
 	
 	$survey_url = $GLOBALS['base_url']. '?q=share/surveylink/multiple/'.$encrypted1->encrypt($classId).'/'.$objType.'/';
	$survey_sharelink = '<a href="'.$survey_url.'">'.t('LBL3087').'</a>'.' '.t('MSG860', array('@delivry_type'=>t('Class')));

  // Prepare notification info
  $notificationInfo = array();
  $notificationInfo['tokens_string'] = 'first_name>|' . $firstName . '~|' .
  									   'last_name>|' . $lastName . '~|' .
  									   'user_email>|' . $email . '~|' .
  									   'course_title>|' . $classTitle . '~|' .
  									   'class_description>|' . $classDesc . '~|' .
									   'course_completed_status>|' . $comp_status . '~|' .
  									   'course_completed_date>|' . $comp_date. '~|' .
  									   'full_name>|' . $fullName . '~|' .
                                       'learning_fullname>|' . $fullName . '~|' .
    								   'user_name>|' . $userName . '~|' .
                                       'user_phone>|' . $userPhone . '~|' .
									   'survey_link>|' . (($result_surveymap['rcount']>0)?$survey_sharelink:'') . '~|' .
(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '');
  $notificationInfo['message_id'] = 'course_completed';
  $notificationInfo['message_type'] = 'Course Completed';
  $notificationInfo['send_type'] = 'php mailer';
  $notificationInfo['lang_code'] = $classInfo['lang_code'];

  //over ride
  switch ($notificationType) {
	case 'ClsCompletedByManager':
	  	$mangerInfo = getPersonDetailsForNotifications($updateUId);
	  	$fullName = convertNullValToNullStr($mangerInfo['full_name']);
	  	$score = (!empty($enrollmentDetails['score'])) ? $enrollmentDetails['score'] : 'NA';
	  	$grade = (!empty($enrollmentDetails['grade'])) ? $enrollmentDetails['grade'] : 'NA';
	  	$notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
	  			'manager_name>|' . $fullName. '~|' .
    			'training_title>|' . $classTitle. '~|' .
    			'score>|' . $score. '~|' .
    			'grade>|' . $grade;
	  	$notificationInfo['message_id'] = 'course_completed_by_manager';
	  	$notificationInfo['message_type'] = 'Class Completion By Manager';
	  	break;
  	case 'ClsCompletedByInstructor':
  		$instructorInfo = getPersonDetailsForNotifications($updateUId);
  		$fullName = convertNullValToNullStr($instructorInfo['full_name']);
  		$score = (!empty($enrollmentDetails['score'])) ? $enrollmentDetails['score'] : 'NA';
  		$grade = (!empty($enrollmentDetails['grade'])) ? $enrollmentDetails['grade'] : 'NA';
  		$notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
  				'instructor_name>|' . $fullName. '~|' .
    			'training_title>|' . $classTitle. '~|' .
    			'score>|' . $score. '~|' .
    			'grade>|' . $grade;
  		$notificationInfo['message_id'] = 'course_completed_by_instructor';
  		$notificationInfo['message_type'] = 'Class Completion By Instructor';
  		break;
    case 'ClsCompletedByInstructorAsNSW':
  		$instructorInfo = getPersonDetailsForNotifications($updateUId);
  		$fullName = convertNullValToNullStr($instructorInfo['full_name']);
  		$notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
  				'instructor_name>|' . $fullName. '~|' .
    			'training_title>|' . $classTitle;
  		$notificationInfo['message_id'] = 'course_completed_by_instructor_as_nsw';
  		$notificationInfo['message_type'] = 'Class marked as no show By Instructor';
  		break;
  }
  
  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCompletedNotification', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateComplianceExpiryNotification() - Generates notification for notification type 'Completed'.
*/
function generateComplianceExpiryNotification($notificationType,$enrollmentId, $classId, $userId) {
	try{
		// Get class title and description.
		$classInfo = getClassInfoForNotifications($classId);
		expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 4);
		$classTitle = '';
		$classDesc = '';
		
		if ($classInfo != null) {
			$deliveryTypeName = convertNullValToNullStr($classInfo['delivery_type_name']);
			$classTitle = convertNullValToNullStr($classInfo['title']);
			$classDesc = convertNullValToNullStr($classInfo['description']);
	       $classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
           $classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
	       
			
		}
		$courseInfo =   getCourseInfoForNotifications($classInfo['course_id']);
		// Get completion status and completion date
		$enrollmentDetails = getEnrDetForComplianceExpiryNotifications($enrollmentId);
		expDebug::dPrint(' $enrollmentDetails = ' . print_r($enrollmentDetails, true) , 4);

		$comp_status = '';
		$comp_date == '';
		/* if ($enrollmentDetails != null && $enrollmentDetails['reg_status'] == 'lrn_crs_reg_cnf') {
		 $comp_status = $enrollmentDetails['comp_status'];
		// dateTimeStrToUSShort($dateTimeString, $year4, $showTime, $time24, $showSeconds)
		$comp_date = dateTimeStrToUSShort($enrollmentDetails['comp_date'], true, false, true, false);
		}
		*/
		// Get person details
		$personInfo = getPersonDetailsForNotifications($userId);
		expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4);

		$firstName = '';
		$lastName = '';
		$email = '';
		$fullName = '';
		$userName = '';
		$userPhone = '';
		if ($personInfo != null) {
			$firstName = convertNullValToNullStr($personInfo['first_name']);
			$lastName = convertNullValToNullStr($personInfo['last_name']);
			$email = convertNullValToNullStr($personInfo['email']);
			$fullName = convertNullValToNullStr($personInfo['full_name']);
			$userName = convertNullValToNullStr($personInfo['user_name']);
			$userPhone = convertNullValToNullStr($personInfo['phone_no']);
		}

		$notificationInfo1['tokens_string'] = 'first_name>|' . $firstName . '~|' .
				'last_name>|' . $lastName . '~|' .
				'user_email>|' . $email . '~|' .
				'course_title>|' . $classTitle . '~|' .
				'class_description>|' . $classDesc . '~|' .
				'course_completed_status>|' . $comp_status . '~|' .
				'course_completed_date>|' . $comp_date. '~|' .
				'full_name>|' . $fullName . '~|' .
				'learning_fullname>|' . $fullName . '~|' .
				'user_name>|' . $userName . '~|' .
				'user_phone>|' . $userPhone . '~|' .
                (($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';		
		// $notificationInfo['message_id'] = 'course_completed';
		//$notificationInfo['message_type'] = 'Course Completed';
		$notificationInfo['send_type'] = 'php mailer';
		$notificationInfo['lang_code'] = $classInfo['lang_code'];
		/* if($courseInfo['validity_date'] !=""){
		 $validityDate = date("Y-m-d",strtotime($courseInfo['validity_date']));
		}else if($courseInfo['validity_days'] !=""){
		$compStatusDate = date("Y-m-d",strtotime($enrollmentDetails['comp_date']));
		$expiryDate = strtotime(date("Y-m-d", strtotime($compStatusDate)) . "+".$courseInfo['validity_days']."days");
		$validityDate = date("Y-m-d", $expiryDate);
		} */
		// Prepare notification info
		$notificationInfo = array();
		switch($notificationType){
			case 'ComplianceExpiryRemainder':
				$notificationInfo['tokens_string']  = $notificationInfo1['tokens_string'] .
				'expiry_date>|' . $enrollmentDetails['valdate'] . '~|' ;
				$notificationInfo['message_id'] 	= 'compliance_expiry_remainder';
				$notificationInfo['message_type'] = 'Compliance Class Expiry Reminder';
				break;
			case 'ComplianceClassExpired':
				$notificationInfo['tokens_string']  = $notificationInfo1['tokens_string'] .
				'course_completed_date>|' . $enrollmentDetails['compdate'] . '~|' .
				'tp_expirydate>|' . $enrollmentDetails['valdate'] . '~|' ;
				$notificationInfo['message_id'] 	= 'compliance_expired';
				$notificationInfo['message_type'] = 'Compliance Class Expired';
				break;
			case 'ComplianceClassExpiredAndEnrolled':
				$notificationInfo['tokens_string']  = $notificationInfo1['tokens_string'] .
				'course_completed_date>|' . $enrollmentDetails['compdate'] . '~|' .
				'class_re_registered>|' . 'You have already re-registered for the same course.One of the class under the course is in enrolled status.' . '~|' .
				'tp_expirydate>|' . $enrollmentDetails['valdate'] . '~|' ;
				$notificationInfo['message_id'] 	= 'compliance_expired';
				$notificationInfo['message_type'] = 'Compliance Class Expired';
				break;
		}


		return $notificationInfo;
	}catch (Exception $ex) {
		watchdog_exception('generateCompletedNotification', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUCOrderIdFromOrderId() - Fetches Ubercart order id from slt_order
 */
function getUCOrderIdFromOrderId($orderId) {
  try{
  // Select table
  $selectStmt = db_select('slt_order', 'ord');
  // Set conditions
  $selectStmt->condition('ord.id', $orderId);
  // Select fields and/or add expressions
  $selectStmt->addField('ord', 'uc_order_id', 'uc_order_id');
  $selectStmt->addField('ord', 'currency_type', 'currency_type');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getUCOrderIdFromOrderId', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getUCOrderDetailsForNotifications() - Fetches Ubercart order details for eCommerce notifications
 */
function getUCOrderDetailsForNotifications($ucOrderId) {
  try{
  // Select table
  $selectStmt = db_select('uc_orders', 'ucord');
  // Join other tables.
  $selectStmt->leftJoin('uc_order_comments', 'ucoc', 'ucord.order_id = ucoc.order_id');
  // Set conditions
  $selectStmt->condition('ucord.order_id', $ucOrderId);
  // Select fields and/or add expressions
  $selectStmt->addField('ucord', 'order_status', 'uc_order_status');
  $selectStmt->addField('ucord', 'created', 'uc_order_date');
  $selectStmt->addExpression('ucord.order_total', 'uc_order_sub_amount');
  $selectStmt->addExpression('ucord.order_total', 'uc_order_total_amount');
  $selectStmt->addField('ucord', 'payment_method', 'uc_order_type');
  $selectStmt->addField('ucoc', 'message', 'lnr_order_reference_number');
  $selectStmt->addField('ucoc', 'comment_id', 'uc_order_comment_id');
  // Add results ordering to the select statement
  $selectStmt->orderBy('uc_order_comment_id', 'DESC');
  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  $ucOrderDetails =  $result->fetchAssoc();
  expDebug::dPrint(' $ucOrderDetails before date format = ' . print_r($ucOrderDetails, true) , 4);
  //Format order date to US short date format
  $ucOrderDetails['uc_order_date'] = dateTimeStrToUSShort('@' . $ucOrderDetails['uc_order_date'], true, false, true, false);
  //Also add placehoder array element for uc_order_tax_amount in uc order details array
  $ucOrderDetails['uc_order_tax_amount'] = 'TBD';

  return $ucOrderDetails;
  }catch (Exception $ex) {
    watchdog_exception('getUCOrderDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getClassesInOrder() - Fetches (course id, class id,program_id) pairs from slt_enrollment as well as slt_master_enrollment table for an order.
 */
function getClassesInOrder($orderId) {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  $selectStmt->leftjoin('slt_master_enrollment','menr','menr.id=enr.master_enrollment_id');
  //$selectStmt->leftjoin('slt_order','ord','ord.id=enr.order_id');
  //$selectStmt->leftjoin('slt_order_items','orditems','orditems.order_id=ord.id');

  // Set conditions
  $selectStmt->condition('enr.order_id', $orderId);
  $selectStmt->condition('enr.reg_status', 'lrn_crs_reg_rsc', '<>');
  // Select fields and/or add expressions
  $selectStmt->addField('enr', 'id', 'id');
  //$selectStmt->addField('orditems', 'convertion_rate', 'convertion_rate');
  $selectStmt->addField('enr', 'class_id', 'class_id');
  $selectStmt->addField('enr', 'course_id', 'course_id');
  $selectStmt->addField('menr', 'program_id', 'program_id');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return  $result->fetchAllAssoc('id', PDO::FETCH_ASSOC); // Order result by id. Should this be ordered by course_id so that all classes of a course get listed together?
  }catch (Exception $ex) {
    watchdog_exception('getClassesInOrder', $ex);
    expertusErrorThrow($ex);
  }
}


function getOrderConvList($orderId) {
	try{
		// Select table
		$selectStmt = db_select('slt_order_items', 'orditems');
		$selectStmt->addField('orditems', 'id', 'id');
		$selectStmt->addField('orditems', 'class_id', 'class_id');
		$selectStmt->addField('orditems', 'program_id', 'program_id');
		$selectStmt->addField('orditems', 'convertion_rate', 'convertion_rate');
		$selectStmt->condition('orditems.order_id', $orderId);

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		$ConvertRateArr = array();
		$resultArr = $result->fetchAllAssoc('id', PDO::FETCH_ASSOC);
		expDebug::dPrint(' $result = ' . print_r($resultArr, true) , 5);
		foreach($resultArr as $k=>$v)
		{
			if($v['program_id'] > 0 && is_numeric($v['program_id']))
				$ConvertRateArr[$v['program_id']] = $v['convertion_rate'];
			else
				$ConvertRateArr[$v['class_id']] = $v['convertion_rate'];
		}


		return $ConvertRateArr;
	}catch (Exception $ex) {
		watchdog_exception('getClassesInOrder', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getClassesInfoForEcommerceNotifications() - Fetches formatted classes information for an order
 */
function getClassesInfoForEcommerceNotifications($orderId, $currency_type="") {
  try{

  // Get classes in order
  $classesList = getClassesInOrder($orderId);
  $OrdConversionRateList = getOrderConvList($orderId);
  expDebug::dPrint(' $$OrdConversionRateList = ' . print_r($OrdConversionRateList, true) , 4);
  //Commented for issues with the currency type if there is no currency in attr1.
  /*if(empty($currency_type))
  	$currency_type = "usd";*/

  $classesInfoStr = '';
  foreach ($classesList as $id => $courseClass) {
  	if($courseClass['program_id'] != ''){
  	$classInfo = getTpInfoForNotifications($courseClass['program_id']);
  	$convertion_rate = $OrdConversionRateList[$courseClass['program_id']];
  	}else{
    // Get title, sap_no, startdate, enddate, location, price
    $classInfo = getClassInfoForNotifications($courseClass['class_id']);
    $convertion_rate = $OrdConversionRateList[$courseClass['class_id']];
  	}
  	$convertion_rate = ($convertion_rate > 0) ? $convertion_rate : 1;
   expDebug::dPrint(' $classInfo = ' . print_r($classInfo, true) , 4);
    $classTitle = '';
    $formattedPrice = ' ';
    if ($classInfo != null) {
      $classTitle = convertNullValToNullStr($classInfo['title']);
	  //$classadditionalinfo = convertNullValToNullStr($classInfo['additional_info']);
      //$classissnoti = convertNullValToNullStr($classInfo['addn_notification_show']);
      $unformattedPrice = convertNullValToNullStr($classInfo['price']);
      $convertedPrice = $unformattedPrice * $convertion_rate;
      $formattedPrice = formatAmount($convertedPrice, false, $currency_type);
      $formattedPrice = ($formattedPrice == '') ? ' ' : $formattedPrice;
    }

    $sapNumber = getProfileSettingsColumn(18, $courseClass['course_id'], 'col1'); //Entity Type 18 => profile_cat = Custom Profile, profile_tag = Course (see table slt_profile_tagging_defn)
    expDebug::dPrint(' $sapNumber = ' . print_r($sapNumber, true) ,4) ;

    $sapNumber = ($sapNumber == null || $sapNumber == '') ? ' ' : $sapNumber;

    // Get class session details
    $classSessionInfo = getClassSessionInfoForNotifications($courseClass['course_id'], $courseClass['class_id']);
    expDebug::dPrint(' $classSessionInfo = ' . print_r($classSessionInfo, true) , 4);
    $sessionStartDate = ' ';
    $sessionEndDate = ' ';
    $locationName = ' ';
    $sessionTimeZone = '';
    $sessionTimeZoneDisp = '';
    if ($classSessionInfo != null) {
      $sessionStartDate = dateTimeStrToUSShort($classSessionInfo['start_date'], true, false, true, false);
      $sessionStartDate = ($sessionStartDate == '')? ' ' : $sessionStartDate;

      $sessionEndDate = dateTimeStrToUSShort($classSessionInfo['end_date'], true, false, true, false);
      $sessionEndDate = ($sessionEndDate == '')? ' ' : $sessionEndDate;

      $locationName = convertNullValToNullStr($classSessionInfo['location_name'], true, false, true, false);
      $locationName = ($locationName == '')? ' ' : $locationName;

      if(!empty($locationName)){ 
	  $LocationDel = ClassLocationDetails($courseClass['class_id'],$locationName);
	  if(count($LocationDel) < 0)  $LocationDel = $locationName;
	  } else $LocationDel = ' - ';
	
	
      $sessionTimeZone = convertNullValToNullStr($classSessionInfo['session_timezone_name']);
    	$sessionTimeZoneDisp = substr($sessionTimeZone,stripos($sessionTimeZone,')')+2);
    }

    $classesInfoStr = $classesInfoStr .
    				  'course_title<|' . $classTitle . '~~' .
                      'sap_no<|' . $sapNumber . '~~' .
                      'session_startdate<|' . $sessionStartDate . '~~' .
  					  'session_enddate<|' . $sessionEndDate . '~~' .
  					  'session_location<|' . $LocationDel . '~~' .
    					'time_zone<|' . $sessionTimeZoneDisp . '~~' .
  					  'course_price<|' . $formattedPrice .
  					  '~@';
  } // end foreach

  return $classesInfoStr;
  }catch (Exception $ex) {
    watchdog_exception('getClassesInfoForEcommerceNotifications', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getDiscountsInfoForEcommerceNotifications() - Fetches formatted discount information for an ubercart order id
 */
function getDiscountsInfoForEcommerceNotifications($ucOrderId, $callForTax = false, $currency_type="") {
  try{
  	//Commented for issues with the currency type if there is no currency in attr1.
  /*if(empty($currency_type))
      $currency_type = "usd";*/
  // Select table
  $selectStmt = db_select('uc_order_line_items', 'ucoli');

  // Set conditions
  $selectStmt->condition('ucoli.order_id', $ucOrderId);
  $selectStmt->condition('ucoli.type',
                           array('uc_discounts', 'cybersource_tax'),
                             'IN');


  // Select fields and/or add expressions
  $selectStmt->addField('ucoli', 'line_item_id', 'id');
  $selectStmt->addField('ucoli', 'title', 'title');
  //$selectStmt->addField('ucoli', 'amount', 'amount');
  $selectStmt->addExpression('sum(ucoli.amount)', 'amount');
  $selectStmt->groupBy("ucoli.title");

  expDebug::dPrintDBAPI(' $selectStmt123 = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  if ($result->rowCount() <= 0) {
    return null;
  }

  $discountsList =  $result->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  $discountsInfoStr = '';
  $taxInfoStr = '';
  foreach ($discountsList as $discount) {
    expDebug::dPrint(' $discountsList detail $result = ' . print_r($discount, true) , 3);
    $title = ($discount['title'] == null)? ' ' : $discount['title'];
    $formattedAmount = formatAmount($discount['amount'], false, $currency_type);
    $formattedAmount = ($formattedAmount == '') ? ' ' : $formattedAmount;
    if(strtolower(trim($title)) == 'tax') {
    	expDebug::dPrint(' entered into tax = '. $title);

    	$taxInfoStr =	$taxInfoStr .
    	'tax_title<|' . $title . '~~' .
    	'tax_amount<|' . $formattedAmount .
    	'~@';
    }
    else{
      expDebug::dPrint(' entered into $discountsInfoStr = '. $title);
      $discountsInfoStr =	$discountsInfoStr .
                        'discount_title<|' . $title . '~~' .
						'discount_amount<|' . $formattedAmount .
						'~@';
    }
  } // end foreach
  if($callForTax == true){
    return $taxInfoStr;
  }else{
    return $discountsInfoStr;
  }
  }catch (Exception $ex) {
    watchdog_exception('getDiscountsInfoForEcommerceNotifications', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * getBillingAddressForEcommerceNotifications() -
 */
function getBillingAddressForEcommerceNotifications($ucOrderId) {
  try{

  // Select table
  $selectStmt = db_select('uc_orders', 'ucord');
  // Join other tables.
  $selectStmt->leftJoin('uc_zones', 'ucz', 'ucord.billing_zone = ucz.zone_id');
  $selectStmt->leftJoin('uc_countries', 'ucc', 'ucord.billing_country = ucc.country_id');

  // Set conditions
  $selectStmt->condition('ucord.order_id', $ucOrderId);

  // Select fields and/or add expressions
  $selectStmt->addField('ucord', 'billing_first_name', 'first_name');
  $selectStmt->addField('ucord', 'billing_last_name', 'last_name');
  $selectStmt->addField('ucord', 'billing_street1', 'add1');
  $selectStmt->addField('ucord', 'billing_street2', 'add2');
  $selectStmt->addField('ucord', 'billing_city', 'city');
  $selectStmt->addField('ucz', 'zone_name', 'state');
  $selectStmt->addField('ucord', 'billing_postal_code', 'zip');
  $selectStmt->addField('ucc', 'country_name', 'country');
  $selectStmt->addField('ucord', 'billing_phone', 'phone');

  // Limit to 1 record (the first record)
  $selectStmt->range(0, 1);

  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  if ($result->rowCount() <= 0) {
    return null;
  }

  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('getBillingAddressForEcommerceNotifications', $ex);
    expertusErrorThrow($ex);
  }
}


/*
 * generateEcommerceNotifications() - Generates notification for notification types 'pay_checkout_rejected' and 'PaymentRegister'
 */
function generateEcommerceNotifications($notificationType, $enrollmentId, $userId) {
  try{
  // Get Order id from slt_enrollment
  //$enrollmentDetails = getEnrollmentDetailsForNotifications($enrollmentId);
  $enrollmentDetails = getEnrollmentandmasterEnrollmentDetailsForNotifications($enrollmentId);
  $orderId = null;
  if ($enrollmentDetails != null) {
    $orderId = $enrollmentDetails['order_id'];
  }
  $instructorInfo  = getClassInstructorInfoForNotifications($enrollmentDetails['class_id']);
  // Get ubercart order id
  $ucOrderDtl = getUCOrderIdFromOrderId($orderId);

  $ucOrderId = $ucOrderDtl['uc_order_id'];
  $currency_type = $ucOrderDtl['currency_type'];
  //Commented for issues with the currency type if there is no currency in attr1.
  /*if(empty($currency_type))
  	$currency_type = "usd";*/

  expDebug::dPrint(' $ucOrderId = ' . print_r($ucOrderId, true) , 4);
  // Get ubercart order details
  $ucOrderDetails = getUCOrderDetailsForNotifications($ucOrderId);
  expDebug::dPrint(' $ucOrderDetails = ' . print_r($ucOrderDetails, true) , 4);
  $orderType = '';
  $lnrOrderReferenceNumber = '';
  $orderStatus = '';
  $orderDate = '';
  $orderSubAmount = '';
  $orderTaxAmount = '';
  $orderTotalAmount = '';
  if ($ucOrderDetails != null) {
    if ($ucOrderDetails['uc_order_type'] == null) {
      $orderType = '';
    }
    else {
      $orderType = getPaymentMethodName($ucOrderDetails['uc_order_type'], 'name');
    }
    $orderType = ($orderType == 'Zero Cost') ? 'N/A' : convertNullValToNullStr($orderType);

    if ($ucOrderDetails['uc_order_status'] == null) {
      $orderStatus = '';
    }
    else {
      $orderStatus = getPaymentStatusName($ucOrderDetails['uc_order_status'], 'name');
    }
    $orderStatus = convertNullValToNullStr($orderStatus);

    $lnrOrderReferenceNumber = convertNullValToNullStr($ucOrderDetails['lnr_order_reference_number']);
    $orderDate = convertNullValToNullStr($ucOrderDetails['uc_order_date']);
    $orderSubAmount = formatAmount($ucOrderDetails['uc_order_sub_amount'], false, $currency_type);
    $orderTaxAmount = convertNullValToNullStr($ucOrderDetails['uc_order_tax_amount']);
    $orderTaxAmount = formatAmount($orderTaxAmount, false, $currency_type);
    $orderTotalAmount = formatAmount($ucOrderDetails['uc_order_total_amount'], false, $currency_type);
  }
  // Get person details
  $personInfo = getFullPersonDetailsForNotifications($userId);
  expDebug::dPrint(' $personInfo = ' . print_r($personInfo, true) , 4 );
  $personId = null;
  $fullName = '';
  $firstName = '';
  $lastName = '';
  $phone = '';
  $email = '';
  $userName = '';
  $address = '';
  $city = '';
  $state = '';
  $country = '';
  $postalcode = null;
  $orgId = null;
  if ($personInfo != null) {
    $personId = convertNullValToNullStr($personInfo['id']);
    $fullName = convertNullValToNullStr($personInfo['full_name']);
    $firstName = convertNullValToNullStr($personInfo['first_name']);
    $lastName = convertNullValToNullStr($personInfo['last_name']);
    $phone = convertNullValToNullStr($personInfo['phone_no']);
    $email = convertNullValToNullStr($personInfo['email']);
    $userName = convertNullValToNullStr($personInfo['user_name']);
    $orgId = convertNullValToNullStr($personInfo['org_id']);
    $address = convertNullValToNullStr($personInfo['addr1']);
    $city = convertNullValToNullStr($personInfo['city']);
    $state = convertNullValToNullStr($personInfo['state_name']);
    $country = convertNullValToNullStr($personInfo['country_name']);
    $postalcode = convertNullValToNullStr($personInfo['zip']);
  }
  $classesInfoStr = getClassesInfoForEcommerceNotifications($orderId, $currency_type);
  expDebug::dPrint(' $classesInfoStr = ' . print_r($classesInfoStr, true), 4 );
  $classInfo1 = getClassInfoForNotifications($enrollmentDetails['class_id']);
  //$classadditionalinfo = $classInfo1['additional_info'];
  //$classissnoti = $classInfo1['addn_notification_show'];
  $classesInfoStr = convertNullValToNullStr($classesInfoStr);
  $discountsInfoStr = getDiscountsInfoForEcommerceNotifications($ucOrderId, false, $currency_type);
  $taxInfoStr = getDiscountsInfoForEcommerceNotifications($ucOrderId, true, $currency_type);
  expDebug::dPrint(' $discountsInfoStr = ' . print_r($discountsInfoStr, true), 4);
  if ($discountsInfoStr == null) {
    $discountsInfoStr = 'discount_title<| ~~discount_amount<| ~@';
  }
  if ($taxInfoStr == null) {
    $taxInfoStr = 'tax_title<| ~~tax_amount<| ~@';
  }
  $lnrOrderReqId = getCCAuthCode($orderId);
  $orderId = convertNullValToNullStr($orderId);
  $organizationInfo = getOrganizationInfoForNotifications($orgId);
  expDebug::dPrint(' $organizationInfo = ' . print_r($organizationInfo, true) , 4 );
  $organizationName = '';
  $organizationAddressLine1 = '';
  $organizationAddressLine2 = '';
  $organizationCity = '';
  $organizationState = '';
  $organizationZip = '';
  $organizationCountry = '';
  $organizationPhone = '';
  $organizationEmail = '';
  $organizationFax = '';
  if ($organizationInfo != null) {
      $organizationName = convertNullValToNullStr($organizationInfo['name']);
      $organizationAddressLine1 = convertNullValToNullStr($organizationInfo['add1']);
      $organizationAddressLine2 = convertNullValToNullStr($organizationInfo['add2']);
      $organizationCity = convertNullValToNullStr($organizationInfo['city']);
      $organizationState = convertNullValToNullStr($organizationInfo['state']);
      $organizationZip = convertNullValToNullStr($organizationInfo['zip']);
      $organizationCountry = convertNullValToNullStr($organizationInfo['country_name']);
      $organizationPhone = convertNullValToNullStr($organizationInfo['phone_no']);
      $organizationEmail = convertNullValToNullStr($organizationInfo['email']);
      $organizationFax = convertNullValToNullStr($organizationInfo['fax_no']);
  }
  $billingAddr = getBillingAddressForEcommerceNotifications($ucOrderId);
  expDebug::dPrint(' $billingAddr = ' . print_r($billingAddr, true), 4);
  $billingFirstName = '';
  $billingLastName = '';
  $billingAddressLine1 = '';
  $billingAddressLine2 = '';
  $billingCity = '';
  $billingState = '';
  $billingZip = '';
  $billingCountry = '';
  $billingPhone = '';
  if ($billingAddr != null) {
    $billingFirstName = convertNullValToNullStr($billingAddr['first_name']);
    $billingLastName = convertNullValToNullStr($billingAddr['last_name']);
    $billingAddressLine1 = convertNullValToNullStr($billingAddr['add1']);
    $billingAddressLine2 = convertNullValToNullStr($billingAddr['add2']);
    $billingCity = convertNullValToNullStr($billingAddr['city']);
    $billingState = convertNullValToNullStr($billingAddr['state']);
    $billingZip = convertNullValToNullStr($billingAddr['zip']);
    $billingCountry = convertNullValToNullStr($billingAddr['country']);
    $billingPhone = convertNullValToNullStr($billingAddr['phone']);
  }

  // Prepare notification info
  $notificationInfo = array();
  $notificationInfo['tokens_string'] =   'full_name>|' . $fullName . '~|' .
    									 'first_name>|' . $firstName . '~|' .
    									 'last_name>|' . $lastName . '~|' .
    									 'user_email>|' . $email . '~|' .
    									 'user_add1>|' . $address . '~|' .
    									 'user_city>|' . $city . '~|' .
    									 'user_state>|' . $state . '~|' .
    									 'user_zip>|' . $postalcode . '~|' .
    									 'user_country>|' . $country . '~|' .
    									 'order_id>|' . $ucOrderId . '~|' .
    									 'order_status>|' . $orderStatus . '~|' .
    									 'order_date>|' . $orderDate . '~|' .
    									 'order_subtotal>|' . $orderSubAmount . '~|' .
    									 'order_tax>|' . $orderTaxAmount . '~|' .
    									 'order_totalamt>|' . $orderTotalAmount . '~|' .
    									 'order_type>|' . $orderType . '~|' .
    									 'group_data>|' . $classesInfoStr . '~|' .
    									 'group_discount_data>|' . $discountsInfoStr . '~|' .
    									 'group_tax_data>|' . $taxInfoStr . '~|' .
										 'user_name>|' . $userName . '~|' .
										 'user_phone>|' . $phone . '~|' .
										 'order_refnumber>|' . $lnrOrderReferenceNumber . '~|' .
										 'order_reqid>|' . $lnrOrderReqId . '~|' .
										 'org_name>|' . $organizationName . '~|' .
										 'org_add1>|' . $organizationAddressLine1 . '~|' .
										 'org_add2>|' . $organizationAddressLine2 . '~|' .
										 'org_city>|' . $organizationCity . '~|' .
										 'org_state>|' . $organizationState . '~|' .
										 'org_zip>|' . $organizationZip . '~|' .
										 'org_country>|' . $organizationCountry . '~|' .
										 'org_phone>|' . $organizationPhone . '~|' .
										 'org__email>|' . $organizationEmail . '~|' .
										 'org_fax>|' . $organizationFax . '~|' .
										 'bill_fistname>|' . $billingFirstName . '~|' .
										 'bill_lastname>|' . $billingLastName . '~|' .
										 'bill_add1>|' . $billingAddressLine1 . '~|' .
										 'bill_add2>|' . $billingAddressLine2 . '~|' .
										 'bill_city>|' . $billingCity . '~|' .
										 'bill_state>|' . $billingState . '~|' .
										 'bill_zip>|' . $billingZip . '~|' .
										 'bill_country>|' . $billingCountry . '~|' .
										 'bill_phone>|' . $billingPhone. '~|' .
  										 'group_instructordata>|' . $instructorInfo;
										//(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '');

  if ($notificationType == 'pay_checkout_rejected') {
    $notificationInfo['message_id'] = 'pay_checkout_rejected';
    $notificationInfo['message_type'] = 'Payment Rejected By Admin';
  }
  else { // $notificationType == 'PaymentRegister'
    $notificationInfo['message_id'] = 'pay_confirm_admin';
    $notificationInfo['message_type'] = 'Payment Confirmation by Admin';
  }

  $notificationInfo['send_type'] = 'php mailer';

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateEcommerceNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCertCurrLPCancelDetailsForNotifications() - Gets person information for notifications
 */
function getCertCurrLPCancelDetailsForNotifications($enrollId) {
  try{

  $selectCcld = db_select('slt_master_enrollment','mse');
  $selectCcld->innerJoin('slt_person','prs','mse.user_id = prs.id');
  $selectCcld->innerJoin('slt_program','prgm','prgm.id = mse.program_id');
  $selectCcld->leftJoin('slt_profile_list_items','prof','prof.code=prgm.object_type');
  $selectCcld->condition('mse.id',$enrollId);

  $selectCcld->addField('prs','full_name');
  $selectCcld->addField('prs','first_name');
  $selectCcld->addField('prof','name','delivery_type');
  $selectCcld->addField('prgm','title');
  $selectCcld->addField('prgm','short_desc','description');
  $selectCcld->addField('prgm','object_type');
  $selectCcld->addField('prgm','code');
  $selectCcld->addField('prgm','lang_code');
  $selectCcld->addField('prgm','additional_info','additional_info');
  $selectCcld->addField('prgm','addn_notification_show','addn_notification_show');

  // Execute query
  $result = $selectCcld->execute();

  if ($result->rowCount() <= 0) {
    return null;
  }
  $results = $result->fetchAssoc();
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPCancelDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCertCurrLPRegisterDetailsForNotifications() - Gets Cert/Curr/LP register details
 */

function getCertCurrLPRegisterDetailsForNotifications($masterEnrollId,$programId,$userId) {
  try{
  $selectCclRegDet = db_select('slt_program','prgm');
  $selectCclRegDet->innerJoin('slt_module_crs_mapping','crsmap','crsmap.program_id = prgm.id');
  $selectCclRegDet->innerJoin('slt_course_template','crs','crs.id = crsmap.course_id and crs.status = \'lrn_crs_sts_atv\'');
  $selectCclRegDet->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $selectCclRegDet->innerJoin('slt_enrollment','enr','enr.master_enrollment_id = mse.id AND enr.course_id = crs.id AND enr.user_id = mse.user_id');
  $selectCclRegDet->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $selectCclRegDet->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
  $selectCclRegDet->innerJoin('slt_module','modu','modu.id = crsmap.module_id');
  $selectCclRegDet->condition('prgm.id',$programId);
  $selectCclRegDet->condition('mse.id',$masterEnrollId);

  $selectCclRegDet->addField('prs','full_name','fullName');
  $selectCclRegDet->addField('prs','first_name','firstname');
  $selectCclRegDet->addField('prs','user_name','username');
  $selectCclRegDet->addField('prs','last_name','lastname');
  $selectCclRegDet->addField('prof','name','deliveryType');
  $selectCclRegDet->addField('crs','title','courseTitle');
  $selectCclRegDet->addField('crs','code','courseCode');
  $selectCclRegDet->addField('prgm','title','programTitle');
  $selectCclRegDet->addField('prgm','short_desc','description');
  $selectCclRegDet->addField('prgm','code','programCode');
  $selectCclRegDet->addField('prgm','lang_code','lang_code');
  $selectCclRegDet->addField('crsmap','is_required','CourseMandatory');
  $selectCclRegDet->addField('enr','comp_status','overallStatus');
  $selectCclRegDet->addField('mse','overall_status','tpoverallStatus');
  $selectCclRegDet->addField('enr','waitlist_priority','waitlist_priority');
  $selectCclRegDet->addField('mse','score','score');
  $selectCclRegDet->addField('mse','grade','grade');
  $selectCclRegDet->addField('mse','comp_date','comp_date');
  $selectCclRegDet->addField('modu','title','moduletitle');
  $selectCclRegDet->addField('prgm','additional_info','additional_info');
  $selectCclRegDet->addField('prgm','addn_notification_show','addn_notification_show');
  $selectCclRegDet->addField('enr','class_id','class_id');
  expDebug::dPrintDBAPI(' getCertCurrLPRegisterDetailsForNotifications() :  ' , $selectCclRegDet);
  // Execute query
  $result = $selectCclRegDet->execute();
  if ($result->rowCount() <= 0) {
    return null;
  }
  $results = $result->fetchAll();
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPRegisterDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getCertCurrLPRecertifyDetailsForNotifications() - Gets Cert/Curr/LP recertify details
 */

function getCertCurrLPRecertifyDetailsForNotifications($programId,$userId,$masterEnrollId) {
  try{
  $selectCclRegDet = db_select('slt_program','prgm');
  $selectCclRegDet->innerJoin('slt_module_crs_mapping','crs_map','crs_map.program_id = prgm.id');
  $selectCclRegDet->innerJoin('slt_module','modu','modu.id = crs_map.module_id');  
  $selectCclRegDet->innerJoin('slt_course_template','crs','crs.id = crs_map.course_id and crs.status = \'lrn_crs_sts_atv\'');
  $selectCclRegDet->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $selectCclRegDet->innerJoin('slt_enrollment','enr','enr.master_enrollment_id = mse.id AND enr.course_id = crs.id AND enr.user_id = mse.user_id');
  $selectCclRegDet->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $selectCclRegDet->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
  $selectCclRegDet->innerJoin('slt_profile_list_items','prof1','prof1.code = enr.comp_status');
  $selectCclRegDet->condition('prgm.id',$programId);
  $selectCclRegDet->condition('mse.user_id',$userId);
  $selectCclRegDet->condition('mse.id',$masterEnrollId);
  $selectCclRegDet->condition('mse.is_current','Y','=');

  $selectCclRegDet->addField('prs','full_name','fullName');
  $selectCclRegDet->addField('prs','first_name','firstname');
  $selectCclRegDet->addField('prs','user_name','username');
  $selectCclRegDet->addField('prof','name','deliveryType');
  $selectCclRegDet->addField('crs','title','courseTitle');
  $selectCclRegDet->addField('crs','code','courseCode');
  $selectCclRegDet->addField('prgm','title','programTitle');
  $selectCclRegDet->addField('prgm','short_desc','description');
  $selectCclRegDet->addField('prgm','code','programCode');
  $selectCclRegDet->addField('prgm','lang_code','lang_code');
  $selectCclRegDet->addField('crs_map','is_required','CourseMandatory');
  $selectCclRegDet->addField('prof1','name','overallStatus');
  $selectCclRegDet->addField('modu','title','moduletitle');
  // Execute query
  expDebug::dPrintDBAPI(' getCertCurrLPRecertifyDetailsForNotifications() :  ' , $selectCclRegDet);
  
  $result = $selectCclRegDet->execute();

  if ($result->rowCount() <= 0) {
    return null;
  }
  $results = $result->fetchAll();
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPRecertifyDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCertCurrLPCancelNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Cancelled
 */
function generateCertCurrLPCancelNotifications($notificationType, $programId, $enrollmentId, $userId) {
  try{
  $getInfo                           = getCertCurrLPCancelDetailsForNotifications($enrollmentId,$userId);
  $tp_description = convertNullValToNullStr($getInfo["description"]);
  $tpadditionalinfo = convertNullValToNullStr($getInfo['additional_info']);
  $tpissnoti = convertNullValToNullStr($getInfo['addn_notification_show']);
 
  expDebug::dPrint('ERR ---- > ' .print_r($tp_description,true),5);
 
  $notificationInfo                  = array();
  $notificationInfo['tokens_string'] = 'first_name>|' . $getInfo["first_name"] . '~|' .
                                       'full_name>|' . $getInfo["full_name"] . '~|' .
  									   'delivery_type>|' . $getInfo["delivery_type"] . '~|' .
									   'tp_title>|' . $getInfo["title"] . '~|' .
									  'tp_description>|' . $tp_description . '~|' .
							          'dt_code>|' . $getInfo["code"] . '~|' .
									   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '');
  $notificationInfo['message_id']    = 'cert_curr_lp_cancel';
  $notificationInfo['message_type']  = 'Certification/Curriculum/Learning Plan cancel by User';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code'] = $getInfo['lang_code'];

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPCancelNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCertCurrLPCancelRefundNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Cancelled
 */
function generateCertCurrLPCancelRefundNotifications($notificationType, $programId, $enrollmentId, $userId) {
  try{
  $getInfo = getCertCurrLPCancelDetailsForNotifications($enrollmentId,$userId);
  $tp_description = convertNullValToNullStr($getInfo["description"]);
  $tpadditionalinfo = convertNullValToNullStr($getInfo['additional_info']);
  $tpissnoti = convertNullValToNullStr($getInfo['addn_notification_show']);
  
  //SELECT item_price, refund_amount INTO xclassamt,xrefundamt FROM slt_order_items WHERE course_id=xxcourseid AND class_id=xxclid  AND user_id=xuserid AND order_id=xorderid LIMIT 1;
  $select = db_select('slt_enrollment', 'enr');
  $select->addField('enr','order_id', 'order_id');
  $select->condition('enr.master_enrollment_id', $enrollmentId);
  $select->condition('enr.user_id', $userId);
  $select->range(0, 1);
  $orderId =$select->execute()->fetchField();

  $selectStmt = db_select('slt_order_items', 'ord');
  $selectStmt->addField('ord','refund_amount', 'refund_amount');
  $selectStmt->addField('ord','tax_amount', 'tax_amount');
  $selectStmt->addField('ord','convertion_rate', 'convertion_rate');
  $selectStmt->condition('ord.program_id', $programId);
  $selectStmt->condition('ord.user_id', $userId);
  $selectStmt->condition('ord.order_id', $orderId);
  $selectStmt->range(0, 1);
  $refundResult =$selectStmt->execute()->fetchAll();

  $selectPrg = db_select('slt_program', 'prg');
  $selectPrg->addField('prg','price', 'price');
  $selectPrg->condition('prg.id', $programId);
  $price =$selectPrg->execute()->fetchField();

  $convertion_rate = $refundResult[0]->convertion_rate;
  $convertion_rate = ($convertion_rate > 0) ? $convertion_rate : 1;
  $price = $price * $convertion_rate;
  $nodeId = getNodeIdResults($programId,$getInfo["object_type"]);
  $ucOrderDtl = getUCOrderIdFromOrderId($orderId);

  $ucOrderId = $ucOrderDtl['uc_order_id'];
  $currency_type = $ucOrderDtl['currency_type'];
  //Commented for issues with the currency type if there is no currency in attr1.
  /*if(empty($currency_type))
  	$currency_type = "usd";*/

  $discountAmount = getDropListDiscountAmount($nodeId,$ucOrderId);

  $refund_amount = convertNullValToCurrency($refundResult[0]->refund_amount);
  $tax_amount = convertNullValToCurrency($refundResult[0]->tax_amount);
  $price  = convertNullValToCurrency($price);
  $discountAmount = convertNullValToCurrency($discountAmount);
  $totalAmt = ($price - $discountAmount + $tax_amount);

  $discountAmount = formatAmount($discountAmount, false, $currency_type);
  $refund_amount = formatAmount($refund_amount, false, $currency_type);
  $tax_amount = formatAmount($tax_amount, false, $currency_type);
  $price = formatAmount($price, false, $currency_type);
  $totalAmt = formatAmount($totalAmt, false, $currency_type);

  $notificationInfo                  = array();
  $notificationInfo['tokens_string'] = 'full_name>|' . $getInfo["full_name"] . '~|' .
  									   'delivery_type>|' . $getInfo["delivery_type"] . '~|' .
									   'tp_title>|' . $getInfo["title"] . '~|' .
									   'tp_description>|' . $tp_description . '~|' .
									   'course_title>|' . $getInfo["title"] . '~|' .
   									   'course_price>|' . $price . '~|' .
   									   'tax_amount>|' . $tax_amount . '~|' .
   									   'discount_amount>|' . $discountAmount . '~|' .
   									   'total_amount>|' . $totalAmt . '~|' .
   									   'refund_amount>|' . $refund_amount . '~|' .
									   'dt_code>|' . $getInfo["code"] . '~|' .
                                       (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '');
  $notificationInfo['message_id']    = 'cert_curr_lp_cancel_refund';
  $notificationInfo['message_type']  = 'Certification/Curriculum/Learning Plan cancel by User with Refund';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code'] = $getInfo['lang_code'];

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPCancelRefundNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCertCurrLPRegisterNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Register
 */
function generateCertCurrLPRegisterNotifications($notificationType, $masterEnrollId, $programId, $userId,$isadmin,$updateUserId ='') {

  try{
  $getInfo          = getCertCurrLPRegisterDetailsForNotifications($masterEnrollId,$programId,$userId);
  $notificationInfo = array();

  $classesInfoStr   = '';
  $isMandatory      = array();
  $fullname         = '';
  $delivery_type    = '';
  $course_title     = '';
  $dt_code          = '';
  $prg_lang_code    = '';
  $tp_description   = '';
  $tpadditional_info = '';
  $tpis_noti = '';
  $totalRec         = count($getInfo);

  $sno=1;
  foreach($getInfo as $getInfo) {
    if($getInfo->coursemandatory == 'Y') {
      $isMandatory[] = $getInfo->coursemandatory;
    }
     $fullname       = $getInfo->fullname;
     $firstname       = $getInfo->firstname;
     $lastname       = $getInfo->lastname;
     $username       = $getInfo->username;
     $delivery_type  = $getInfo->deliverytype;
     $course_title   = $getInfo->programtitle;
     $dt_code        = $getInfo->programcode;
	 $tp_description = convertNullValToNullStr($getInfo->description);
	 $tpadditional_info = $getInfo->additional_info;
	 $tpis_noti = $getInfo->addn_notification_show;
     $prg_lang_code  = $getInfo->lang_code;
     $module_title   = convertNullValToNullStr($getInfo->moduletitle);
     $course_status  = convertNullValToNullStr(getProfileListItemName($getInfo->overallstatus));
     $classesInfoStr .= 'tp_title<|' . $getInfo->coursetitle . '~~' .
                      'tp_code<|' . $getInfo->coursecode . '~~' .
     				  'serial_no<|' . $sno . '~~' .
     				  'module_title<|' . $module_title . '~~' .
                      'tp_mandatory_optional<|' . (($getInfo->coursemandatory =='Y') ? "Mandatory" : "Optional" ). '~~' .
  					  'tp_status<|' . $course_status . '~@';
     $sno++;
  }

  $mandatory       = count($isMandatory);
  $nonMandatory    = $totalRec - $mandatory;

  if($notificationType == 'CertCurrLPComplete'){
  	$sel_prolist = db_select('slt_profile_list_items','pro');
	$sel_prolist->addField('pro','code','delcode');
	$sel_prolist->condition('pro.name',$delivery_type,'=');
	$sel_prolist->condition('pro.lang_code','cre_sys_lng_eng','=');
	$result_pro = $sel_prolist->execute()->fetchAssoc();

	$sel_surveymap = db_select('slt_survey_mapping','mapp');
	$sel_surveymap->addExpression('count(mapp.id)','rcount');
	$sel_surveymap->condition('mapp.object_id',$programId,'=');
	$sel_surveymap->condition('mapp.object_type',$result_pro['delcode'],'=');
	$result_surveymap = $sel_surveymap->execute()->fetchAssoc(); 

 	$encrypted1 = new Encrypt();
 	$encrypted2 = new Encrypt();

 	$conv_deliverytype = preg_replace('/\s+/','',$delivery_type);
 	$objType = $encrypted1->encrypt($conv_deliverytype);
 
 	switch($conv_deliverytype) {
 		case 'Curricula':
 			$surObjectType = t('Curricula');
 			break;
 		case 'Certification':
 			$surObjectType = t('Certification');
 			break;
 		case 'Trainingplan':
 			$surObjectType = t('Training Plan');
 			break;
 		case 'LearningPlan':
 			$surObjectType = t('Learning Plan');
 			break;
 	}

 	$survey_url = $GLOBALS['base_url']. '?q=share/surveylink/multiple/'.$encrypted2->encrypt($programId).'/'.$objType.'/';
	$survey_sharelink = '<a href="'.$survey_url.'">'.t('LBL3087').'</a>'.' '.t('MSG860', array('@delivry_type'=>$surObjectType));
  }

  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
                                       'first_name>|' . $firstname . '~|' .
                                       'user_name>|' . $username . '~|' .
                                       'last_name>|' . $lastname . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'tp_title>|' . $course_title . '~|' .
									   'course_title>|' . $course_title . '~|' .
									   'tp_description>|' . $tp_description . '~|' .
									   'dt_code>|' . $dt_code . '~|'.
                                       'tp_mandatory_courses>|' . (($mandatory>0)?$mandatory:'Nil') . '~|' .
									   'tp_optional_course>|' . (($nonMandatory)>0?$nonMandatory:'Nil') . '~|' .
									   'survey_link>|' . (($result_surveymap['rcount']>0)?$survey_sharelink:''). '~|'. 
									   (($tpis_noti == 1) ? (($tpadditional_info) ? ('add_notes>|'. $tpadditional_info) : ('add_notes>|'.'-')) : '') . '~|' .
                                       'group_sessiondata>|' . $classesInfoStr;

  if($notificationType == 'CertCurrLPComplete'){
    $msgId = 'cert_curr_lp_completed';
    $msgType = 'Cert/Curr/LP Completed';
  }else if($notificationType == 'CertCurrLPCompleteByManager'){
      //issue fix for 0074589
       if($programId){
          $msgId = 'cert_curr_lp_completed';
          $msgType = 'Cert/Curr/LP Completed';
      }
      else{ 
  	$mangerInfo = getPersonDetailsForNotifications($updateUserId);
  	$fullName = convertNullValToNullStr($mangerInfo['full_name']);
  	$score = (!empty($getInfo->score)) ? $getInfo->score : 'NA';
  	$grade = (!empty($getInfo->grade)) ? $getInfo->grade : 'NA';
  	$comp_date = dateTimeStrToUSShort($getInfo->comp_date, true, false, true, false);
  	$notificationInfo['tokens_string'] =  $notificationInfo['tokens_string'] . '~|' .
  			'manager_name>|' . $fullName. '~|' .
  			'training_title>|' . $course_title. '~|' .
  			'score>|' . $score. '~|' .
  			'grade>|' . $grade. '~|' .
  			'course_completed_date>|' . $comp_date. '~|' .
  			'first_name>|' . $getInfo->first_name;
  	$msgId = 'course_completed_by_manager';
  	$msgType = 'Cert/Curr/LP Completed';
  }
  }else{
  	if($isadmin=="Y")  { 
      $msgId = 'cert_curr_lp_register_by_admin';
      $msgType = 'Cert/Curr/Learning Plan registered by an Admin';      
     }
    else {
    $msgId = 'cert_curr_lp_register';
    $msgType = 'Cert/Curr/Learning Plan registered';
  }
    
  	
  }
  $notificationInfo['message_id']    = $msgId;
  $notificationInfo['message_type']  = 'Cert/Curr/Learning Plan registered';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPRegisterNotifications', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * generateCertCurrLPWaitlistRegisterNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Waitlist Register
 */
function generateCertCurrLPWaitlistRegisterNotifications($notificationType, $masterEnrollId, $programId, $userId,$isadmin) {
  try{
  $getInfo          = getCertCurrLPRegisterDetailsForNotifications($masterEnrollId,$programId,$userId);
  $tpadditionalinfo = convertNullValToNullStr($getInfo[0]->additional_info);
  $tpissnoti = convertNullValToNullStr($getInfo[0]->addn_notification_show);
  $notificationInfo = array();

  $classesInfoStr   = '';
  $isMandatory      = array();
  $fullname         = '';
  $delivery_type    = '';
  $tp_title         = '';
  $dt_code          = '';
  $prg_lang_code    = '';
  $tp_description	= '';
  $totalRec         = count($getInfo);

  $sno=1;
  foreach($getInfo as $getInfo) {
    if($getInfo->coursemandatory == 'Y') {
      $isMandatory[] = $getInfo->coursemandatory;
    }
     $fullname       = $getInfo->fullname;
     $lastname       = $getInfo->lastname;
     $firstname       = $getInfo->firstname;
     $username       = $getInfo->username;
     $delivery_type  = $getInfo->deliverytype;
     $tp_title   = $getInfo->programtitle;
	$tp_description = convertNullValToNullStr($getInfo->description);

     $dt_code        = $getInfo->programcode;
     $prg_lang_code  = $getInfo->lang_code;
     $module_title   = convertNullValToNullStr($getInfo->moduletitle);
     $comp_status =  empty($getInfo->overallstatus)? 'Waitlist Position ' .$getInfo->waitlist_priority : convertNullValToNullStr(getProfileListItemName($getInfo->overallstatus));
     $classesInfoStr .= 'course_title<|' . $getInfo->coursetitle . '~~' .
                      'tp_code<|' . $getInfo->coursecode . '~~' .
     				  'serial_no<|' . $sno . '~~' .
     				  'module_title<|' . $module_title . '~~' .
                      'tp_mandatory_optional<|' . (($getInfo->coursemandatory =='Y') ? "Mandatory" : "Optional" ). '~~' .
  					  'tp_status<|' . $comp_status . '~@';
     $sno++;
  }

  $mandatory       = count($isMandatory);
  $nonMandatory    = $totalRec - $mandatory;



  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
                                       'first_name>|' . $firstname . '~|' .
                                       'user_name>|' . $username . '~|' .
                                       'last_name>|' . $lastname . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'tp_title>|' . $tp_title . '~|' .
									   'tp_description>|' . $tp_description . '~|' .
									   'course_title>|' . $tp_title . '~|' .
									   'dt_code>|' . $dt_code . '~|'.
                                       'tp_mandatory_courses>|' . (($mandatory>0)?$mandatory:'Nil') . '~|' .
									   'tp_optional_course>|' . (($nonMandatory)>0?$nonMandatory:'Nil') . '~|' .
									   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|' .
                                       'group_sessiondata>|' . $classesInfoStr;

	if($isadmin=="Y") {
	  $notificationInfo['message_id']    = 'cert_curr_lp_waitlist_register_by_admin';
	  $notificationInfo['message_type']  = 'Cert/Curr/Learning Plan Waitlist registered by an Admin';
	} else {
  $notificationInfo['message_id']    = 'cert_curr_lp_waitlist_register';
  $notificationInfo['message_type']  = 'Cert/Curr/Learning Plan Waitlist registered';
	}
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPWaitlistRegisterNotifications', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * generateCertCurrLPWaitlistPromotionNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Waitlist Promotion
 */
function generateCertCurrLPWaitlistPromotionNotifications($notificationType, $masterEnrollId, $programId, $userId) {
  try{
  $getInfo          = getCertCurrLPRegisterDetailsForNotifications($masterEnrollId,$programId,$userId);
  $tpadditionalinfo = convertNullValToNullStr($getInfo[0]->additional_info);
  $tpissnoti = convertNullValToNullStr($getInfo[0]->addn_notification_show);
  $notificationInfo = array();

  $classesInfoStr   = '';
  $isMandatory      = array();
  $fullname         = '';
  $delivery_type    = '';
  $tp_title         = '';
  $dt_code          = '';
  $tp_description   = '';
  $prg_lang_code    = '';
  $totalRec         = count($getInfo);

  $sno=1;
  foreach($getInfo as $getInfo) {
    if($getInfo->coursemandatory == 'Y') {
      $isMandatory[] = $getInfo->coursemandatory;
    }
     $fullname       = $getInfo->fullname;
     $firstname       = $getInfo->firstname;
     $lastname       = $getInfo->lastname;
     $username       = $getInfo->username;
     $delivery_type  = $getInfo->deliverytype;
     $tp_title   = $getInfo->programtitle;
	 $tp_description = convertNullValToNullStr($getInfo->description);
	
     $dt_code        = $getInfo->programcode;
     $prg_lang_code  = $getInfo->lang_code;
     $module_title   = convertNullValToNullStr($getInfo->moduletitle);
     $comp_status =  empty($getInfo->overallstatus)? 'Waitlist Position ' .$getInfo->waitlist_priority : convertNullValToNullStr(getProfileListItemName($getInfo->overallstatus));
     $classesInfoStr .= 'course_title<|' . $getInfo->coursetitle . '~~' .
                      'tp_code<|' . $getInfo->coursecode . '~~' .
     				  'serial_no<|' . $sno . '~~' .
     				  'module_title<|' . $module_title . '~~' .
                      'tp_mandatory_optional<|' . (($getInfo->coursemandatory =='Y') ? "Mandatory" : "Optional" ). '~~' .
  					  'tp_status<|' . $comp_status . '~@';
     $sno++;
  }

  $mandatory       = count($isMandatory);
  $nonMandatory    = $totalRec - $mandatory;



  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
                                       'first_name>|' . $firstname . '~|' .
                                       'user_name>|' . $username . '~|' .
                                       'last_name>|' . $lastname . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'tp_title>|' . $tp_title . '~|' .
									   'tp_description>|' . $tp_description . '~|' .
									   'dt_code>|' . $dt_code . '~|'.
                                       'tp_mandatory_courses>|' . (($mandatory>0)?$mandatory:'Nil') . '~|' .
									   'tp_optional_course>|' . (($nonMandatory)>0?$nonMandatory:'Nil') . '~|' .
									   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|' .
                                       'group_sessiondata>|' . $classesInfoStr;


  $notificationInfo['message_id']    = 'cert_curr_lp_waitlist_promotion';
  $notificationInfo['message_type']  = 'Cert/Curr/Learning Plan Waitlist promotion';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPWaitlistPromotionNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCertCurrLPWaitlistCancelNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Waitlist Cancellation
 */
function generateCertCurrLPWaitlistCancelNotifications($notificationType, $masterEnrollId, $programId, $userId) {
  try{
  $getInfo          = getCertCurrLPRegisterDetailsForNotifications($masterEnrollId,$programId,$userId);
  $tpadditionalinfo = convertNullValToNullStr($getInfo[0]->additional_info);
  $tpissnoti = convertNullValToNullStr($getInfo[0]->addn_notification_show);

  $notificationInfo = array();

  $classesInfoStr   = '';
  $isMandatory      = array();
  $fullname         = '';
  $delivery_type    = '';
  $tp_title         = '';
  $dt_code          = '';
  $prg_lang_code    = '';
  $tp_description	= '';
  $totalRec         = count($getInfo);

  $sno=1;
  foreach($getInfo as $getInfo) {
    if($getInfo->coursemandatory == 'Y') {
      $isMandatory[] = $getInfo->coursemandatory;
    }
     $fullname       = $getInfo->fullname;
     $firstname       = $getInfo->firstname;
     $lastname       = $getInfo->lastname;
     $username       = $getInfo->username;
     $delivery_type  = $getInfo->deliverytype;
     $tp_title   = $getInfo->programtitle;
	 $tp_description = convertNullValToNullStr($getInfo->description);
	 $dt_code        = $getInfo->programcode;
     $prg_lang_code  = $getInfo->lang_code;
	 
    // $module_title   = convertNullValToNullStr($getInfo->moduletitle);   
    $cls_delivery_type = getClassDeliveryType($getInfo->class_id, 'Long');
    
    if($cls_delivery_type == 'Classroom') {
    $LocationDel = ClassLocationDetails($getInfo->class_id);	 
	} else $LocationDel = '';
	
     $comp_status =  empty($getInfo->tpoverallstatus)? ' ' : convertNullValToNullStr(getProfileListItemName($getInfo->tpoverallstatus));
     $classesInfoStr .= 'course_title<|' . $getInfo->coursetitle . '~~' .
                      'tp_code<|' . $getInfo->coursecode . '~~' .
     				  //'serial_no<|' . $sno . '~~' .
     				  'session_location<|' . $LocationDel . '~~' .
                      'tp_mandatory_optional<|' . (($getInfo->coursemandatory =='Y') ? "Mandatory" : "Optional" ). '~@';
  					  //'tp_status<|' . $comp_status . '~@';
     $sno++;
  }

  $mandatory       = count($isMandatory);
  $nonMandatory    = $totalRec - $mandatory;



  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
                                       'first_name>|' . $firstname . '~|' .
                                       'last_name>|' . $lastname . '~|' .
                                       'user_name>|' . $username . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'tp_title>|' . $tp_title . '~|' .
									   'dt_code>|' . $dt_code . '~|'.
									   'tp_description>|' . $tp_description . '~|' .
                                       'tp_mandatory_courses>|' . (($mandatory>0)?$mandatory:'Nil') . '~|' .
									   'tp_optional_course>|' . (($nonMandatory)>0?$nonMandatory:'Nil') . '~|' .
									   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|' .
                                       'group_sessiondata>|' . $classesInfoStr;


  $notificationInfo['message_id']    = 'cert_curr_lp_drop_waitlist';
  $notificationInfo['message_type']  = 'Cert/Curr/Learning Plan Drop Waitlist';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPWaitlistCancelNotifications', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * generateCertCurrLPRecertifyNotifications() - Generates notification for Certification/Curriculum/Learning Plan types Register
 */
function generateCertCurrLPRecertifyNotifications($notificationType, $masterEnrollId, $programId, $userId,$isadmin) {
  try{

  $getInfo          = getCertCurrLPRecertifyDetailsForNotifications($programId,$userId,$masterEnrollId);
  $tpadditionalinfo = convertNullValToNullStr($getInfo[0]->additional_info);
  $tpissnoti = convertNullValToNullStr($getInfo[0]->addn_notification_show);
  $notificationInfo = array();

  $classesInfoStr   = '';
  $isMandatory      = array();
  $fullname         = '';
  $delivery_type    = '';
  $course_title     = '';
  $tp_description   = '';
  $dt_code          = '';
  $prg_lang_code    = '';
  $totalRec         = count($getInfo);

  foreach($getInfo as $getInfo) {
    if($getInfo->coursemandatory == 'Y') {
      $isMandatory[] = convertNullValToNullStr($getInfo->coursemandatory);
    }
     $fullname       = convertNullValToNullStr($getInfo->fullname);
     $firstname       = convertNullValToNullStr($getInfo->firstname);
     $username       = convertNullValToNullStr($getInfo->username);
     $delivery_type  = convertNullValToNullStr($getInfo->deliverytype);
     $course_title   = convertNullValToNullStr($getInfo->programtitle);
     $dt_code        = convertNullValToNullStr($getInfo->programcode);
	 $tp_description = convertNullValToNullStr($getInfo->description);
     $prg_lang_code  = $getInfo->lang_code;
     $module_title   = convertNullValToNullStr($getInfo->moduletitle);
     $classesInfoStr .= 'tp_title<|' . $getInfo->coursetitle . '~~' .
                      'tp_code<|' . $getInfo->coursecode . '~~' .
                      'module_title<|' . $module_title . '~~' .
                      'tp_mandatory_optional<|' . (($getInfo->coursemandatory =='Y') ? "Mandatory" : "Optional" ). '~~' .
  					  'tp_status<|' . $getInfo->overallstatus . '~@';
  }

  $mandatory       = count($isMandatory);
  $nonMandatory    = $totalRec - $mandatory;



  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
                                       'first_name>|' . $firstname . '~|' .
                                       'user_name>|' . $username . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'tp_title>|' . $course_title . '~|' .
									    'tp_description>|' . $tp_description . '~|' .
									   'course_title>|' . $course_title . '~|' .
									   'dt_code>|' . $dt_code . '~|'.
                    				   'tp_mandatory_courses>|' . (($mandatory>0)?$mandatory:'Nil') . '~|' .
									   'tp_optional_course>|' . (($nonMandatory)>0?$nonMandatory:'Nil') . '~|' .
									   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|' .
                                       'group_sessiondata>|' . $classesInfoStr;


  if($isadmin=="Y")  
	{
      $notificationInfo['message_id']    = 'cert_curr_lp_recertify_by_admin'; 
      $notificationInfo['message_type']  = 'Cert/Curr/LP Recertify by an Admin';
    }
  else 
    { 
  $notificationInfo['message_id']    = 'cert_curr_lp_recertify';
  $notificationInfo['message_type']  = 'Cert/Curr/LP Recertify';
    } 
  
  
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPRecertifyNotifications', $ex);
    expertusErrorThrow($ex);
  }

}

/*
 * getDrupalUserForNotificationInsert() - Fetches drupal user id and email from drupal users table. Also fetched full name from slt_person
 */
function getDrupalUserForNotificationInsert($personId, $getDrupalLangCode = false,$emailId='') {
  try{
  // Select table
  $selectStmt = db_select('slt_person', 'prsn');
  // Join other tables.
  $selectStmt->join('users', 'usr', 'prsn.user_name = usr.name');
  if ($getDrupalLangCode) {
    $selectStmt->leftJoin('slt_profile_list_items', 'lang_spli', 'prsn.preferred_language = lang_spli.code && lang_spli.lang_code = \'cre_sys_lng_eng\'');
  }
  if(empty($emailId)){
  	// Set conditions
  	$selectStmt->condition('prsn.id', $personId);
  }else{
  	$selectStmt->condition('prsn.email', $emailId);
  }
  // Select fields and/or add expressions
  $selectStmt->addField('usr', 'uid', 'id');
  $selectStmt->addField('prsn', 'full_name', 'full_name');
  $selectStmt->addField('prsn', 'manager_id', 'manager_id');
  $selectStmt->addField('usr', 'mail', 'email');
  $selectStmt->addField('prsn', 'preferred_language', 'preferred_language');
  $selectStmt->addField('prsn', 'status', 'user_status');
  if ($getDrupalLangCode) {
    $selectStmt->addField('lang_spli', 'attr1', 'drupal_lang_code');
  }
 expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  if ($result->rowCount() <= 0) {
    return null;
  }
  $result = $result->fetchAssoc();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getDrupalUserForNotificationInsert', $ex);
    if (isset($_GET['cron_key'])) {
      expertusLogError($ex);
    }
    else {
      expertusErrorThrow($ex);
    }
  }
}

/*
 * generateCertCurrLPExprRemainderNotifications() - gets the list of Certification/Curriculum/Learning Plan enrollment expiry remainder
 */

/*function generateCertCurrLPExprRemainderNotifications($notificationType, $userId, $prgmId) {
  try{
  $getInfo = getCertCurrLPExpiryDetailsForNotifications($userId,$prgmId);
  $fullname         = '';
  $delivery_type    = '';
  $course_title     = '';
  $session_enddate  = '';
  $session_endtime  = '';
  $prg_lang_code    = '';

  $fullname       = convertNullValToNullStr($getInfo['first_name']);
  $delivery_type  = convertNullValToNullStr($getInfo['delivery_type']);
  $course_title   = convertNullValToNullStr($getInfo['title']);
  $session_enddate  = convertNullValToNullStr($getInfo['register_end']);
  $session_endtime  = convertNullValToNullStr($getInfo['expire_days']);
  $prg_lang_code    = convertNullValToNullStr($getInfo['lang_code']);

  $notificationInfo['tokens_string'] = 'first_name>|' . $fullname . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
									   'training_title>|' . $course_title . '~|' .
									   'tp_title>|' . $course_title . '~|' .
									   'course_title>|' . $course_title . '~|' .
									   'session_enddate>|' . $session_enddate . '~|'.
                                       'session_endtime>|' . $session_endtime . '~|';

  $notificationInfo['message_id']    = 'cert_curr_lp_expire_reminder';
  $notificationInfo['message_type']  = 'Cert/Curr/Learning plan expiry remainder';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPExprRemainderNotifications', $ex);
    expertusErrorThrow($ex);
  }
}*/

/*
 *  check_monitor_jobs_status() - check monitoring job status
 */
function check_monitor_jobs_status($jobname) {
  try{
  //slt_monitoring_jobs
  $selectStmt = db_select('slt_monitoring_jobs', 'mjb');
  // Set conditions
  $selectStmt->condition('job_name', $jobname, '=');
  // Select fields and/or add expressions
  $selectStmt->addField('mjb', 'last_run_date', 'last_run');
		$selectStmt->addField('mjb', 'custom0', 'custom0');
  $result = $selectStmt->execute();
  return $result->fetchAssoc();
  }catch (Exception $ex) {
    watchdog_exception('check_monitor_jobs_status', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * update_monitor_job() - to update monitor job
 */
function update_monitor_job($jname) {
  $txn = db_transaction();
  try{
	  $updateMonitorjobs = db_update('slt_monitoring_jobs');
	  $updateMonitorjobs->condition('job_name', $jname,'=');
	  $updateMonitorjobs->fields(array(
	                      'last_run_date' => now(),
	  					  'updated_on'    => now()
	                      ));
	  $updateMonitorjobs->execute();
  }catch (Exception $ex) {
      $txn->rollback();
	  unset($txn);
	  watchdog_exception('update_monitor_job', $ex);
	  expertusErrorThrow($ex);
  }
  unset($txn);
}
/*
 * getCertCurrLPExpiryDetailsForNotifications() - get all Certification/Curriculum/Learning plan expiry enrollments
 */

/*function getCertCurrLPExpiryDetailsForNotifications($user_id,$programId='') {
  try{
  $config           = getConfig("exp_sp");
  $cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
  $select = db_select('slt_program','prgm');
  $select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $select->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
  $select->addField('prs','id','personId');
  $select->addField('prs','first_name');
  $select->addField('prgm','title');
  $select->addField('prgm','lang_code');
  $select->addField('prof','name','delivery_type');
  $select->addField('mse','comp_date','comp_date');
  $select->addField('prgm','id');
  $select->addExpression("DATE_FORMAT(
                            IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
                                  IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
                                        DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
                            '%b %e, %Y')", "register_end");
  $select->addExpression("DATEDIFF(
                            IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
                                  IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
                                        DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
                            NOW())", "expire_days");
  if($user_id>0) {
    $select->condition('prs.id',$user_id,'=');
  }
  if($programId > 0){
  	$select->condition('prgm.id',$programId,'=');
  }
  $select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
  $select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
  $select->isNotNull('mse.comp_date');
  $select->isNotNull('prgm.expires_in_value');
  $select->isNotNull('prgm.expires_in_unit');
  $select->condition('prgm.expires_in_value', 0, '>');
  $select->where("DATEDIFF(
                    IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
                          IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
                                DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
                    NOW()) = $cert_days_expire");

  expDebug::dPrintDBAPI('$select getCertCurrLPExpiryDetailsForNotifications', $select);
  $result = $select->execute();
  if($user_id>0) {
    $results = $result->fetchAssoc();
  } else {
    $results = $result->fetchAll();
  }
  expDebug::dPrint('$results = ' . print_r($results, true));
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPExpiryDetailsForNotifications', $ex);
  	if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}*/
/*
 * generateCertCurrLPAssignCancelNotifications() - prepare notification data for cert/curr/lp assignment
 */

function generateCertCurrLPAssignCancelNotifications($notificationType, $objectId1, $objectId2, $userId, $updateUserId) {
  try{
  global $base_url;
  expDebug::dPrint('$notificationType : '.print_r($notificationType, true),4);
  if($notificationType == 'certCurrLPAssigned') {
    $mstrEnrId        = $objectId1;
    $getInfo          = getCertCurrLPDetailsForNotifications($mstrEnrId);
    $order_type       = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('My Programs').'</a>';//'MY PROGRAMS';
    $message_type     = 'Cert/Curr/Learning plan Assignment';
    $notificationInfo['message_id']    = 'cert_curr_lp_assignment';
  } else if($notificationType == 'certCurrLPRejectionByManager') {
    $mstrEnrId        = $objectId1;
    $getInfo          = getCertCurrLPDetailsForNotifications($mstrEnrId);
    $order_type       = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('My Programs').'</a>';//'MY PROGRAMS';
    $message_type     = 'Cert/Curr/Learning plan Rejection By Manager';
    $notificationInfo['message_id']    = 'class_rejection_by_manager';
  } else {
    $getInfo          = getClassAssignDetailsForNotifications($objectId1, $objectId2, $userId, $updateUserId);
    $order_type       = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('My Enrollments').'</a>';//'MY Enrollments';
    $message_type     = 'Class Assignment';
    $notificationInfo['message_id']    = 'cert_curr_lp_assignment';
  }
  expDebug::dPrint('$getInfo : '.print_r($getInfo, true),4);
  $fullname         = '';
  $delivery_type    = '';
  $course_title     = '';
  $assign_by        = '';
  $prg_lang_code    = '';
  $tp_description   = '';
  //$assigned_to_first_name = ''; // Fix Issue 52867
  $fullname         = convertNullValToNullStr($getInfo['assigned_to']);
  $assigned_to_first_name         = convertNullValToNullStr($getInfo['assigned_to_first_name']); // Fix Issue 52867
  $assigned_to_user_name         = convertNullValToNullStr($getInfo['assigned_to_user_name']);
  $assigned_to_last_name       = convertNullValToNullStr($getInfo['assigned_to_last_name']);
  $delivery_type    = convertNullValToNullStr($getInfo['delivery_type']);
  $course_title     = convertNullValToNullStr($getInfo['program_title']);
  $tp_description = convertNullValToNullStr($getInfo['description']);
  $tpadditionalinfo = convertNullValToNullStr($getInfo['additional_info']);
  $tpissnoti = convertNullValToNullStr($getInfo['addn_notification_show']);
  $assign_by        = convertNullValToNullStr($getInfo['assigned_by']);
  $prg_lang_code    = convertNullValToNullStr($getInfo['lang_code']);
  $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|' .
    								   'first_name>|' . $assigned_to_first_name . '~|' .
    								   'user_name>|' . $assigned_to_user_name . '~|' .
    								   'last_name>|' . $assigned_to_last_name . '~|' .
  									   'manager_name>|' . $assign_by . '~|' .
  									   'order_type>|' . $order_type . '~|' .
  									   'delivery_type>|' . $delivery_type . '~|' .
  									   'training_title>|' . $course_title . '~|' .
  									   'tp_title>|' . $course_title . '~|' .
  									   'tp_description>|' . $tp_description . '~|' .
									   'course_title>|' . $course_title . '~|' .
        							   (($tpissnoti == 1) ? (($tpadditionalinfo) ? ('add_notes>|'. $tpadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';
  $notificationInfo['message_type']  = $message_type;
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;
  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPAssignCancelNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getCertCurrLPDetailsForNotifications() - To get notification assigned cert/curr/lp details
 */

function getCertCurrLPDetailsForNotifications($mstrEnrId) {
  try{
  $select = db_select('slt_master_enrollment','mse');
  $select->innerJoin('slt_program','prgm','prgm.id = mse.program_id');
  $select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
  $select->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $select->innerJoin('slt_person','prs1','prs1.id = mse.updated_by');
  $select->addField('prgm','title','program_title');
  $select->addField('prgm','short_desc','description');
  $select->addField('prgm','lang_code','lang_code');
  $select->addField('prs','full_name','assigned_to');
  $select->addField('prs','first_name','assigned_to_first_name'); // Fix Issue 52867
  $select->addField('prs','user_name','assigned_to_user_name');
  $select->addField('prs','last_name','assigned_to_last_name');
  $select->addField('prs1','full_name','assigned_by');
  $select->addField('prof','name','delivery_type');
  $select->addField('prgm','additional_info','additional_info');
  $select->addField('prgm','addn_notification_show','addn_notification_show');
  $select->condition('mse.id',$mstrEnrId,'=');
  expDebug::dPrintDBAPI('$select', $select);
  $exec_qry = $select->execute();

  $result = $exec_qry->fetchAssoc();

  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getClassAssignDetailsForNotifications() - To get the information of assigned class and the respective person details
 */

function getClassAssignDetailsForNotifications($courseId,$classId,$userId,$updUserId) {
  try{

  $select = db_select('slt_course_class','cls');
  $select->innerJoin('slt_enrollment','enr','enr.class_id = cls.id and enr.course_id = cls.course_id');
  $select->innerJoin('slt_person','prs','prs.id = enr.user_id');
  $select->innerJoin('slt_person','prs1','prs1.id = enr.created_by');
  $select->innerJoin('slt_profile_list_items','prof','prof.code = cls.delivery_type');

  $select->addField('cls','title','program_title');
  $select->addField('cls','lang_code','lang_code');
  $select->addField('prs','full_name','assigned_to');
  $select->addField('prs','first_name','assigned_to_first_name');
  $select->addField('prs','user_name','assigned_to_user_name');
  $select->addField('prs','last_name','assigned_to_last_name');
  $select->addField('prs1','full_name','assigned_by');
  $select->addField('prof','name','delivery_type');
  $select->addField('cls','additional_info','additional_info');
  $select->addField('cls','addn_notification_show','addn_notification_show');

  $select->condition('enr.course_id',$courseId,'=');
  $select->condition('enr.class_id',$classId,'=');
  $select->condition('enr.user_id',$userId,'=');
  $select->condition('enr.created_by',$updUserId,'=');

  $exec_qry = $select->execute();

  $result = $exec_qry->fetchAssoc();

  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getClassAssignDetailsForNotifications', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getLocationContactEmail() - Fetches location contact e-mail
 */
function getLocationContactEmail($locationId) {
  try{
  // Select table
  $selectStmt = db_select('slt_location', 'location');
  // Set conditions
  $selectStmt->condition('location.id', $locationId);
  // Select fields and/or add expressions
  $selectStmt->addField('location', 'email_id', 'contact_email');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return null;
  }
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getLocationContactEmail', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 * Get the count of Courses to be complete under Object type.
 *
 * @param string $objectid
 *
 * @return
 *   count of the Courses to be complete.
 */

function getCoursestobeComplete($objectid){
  try{

  $select = db_select('slt_program', 'prm');
  $select->join('slt_module_crs_mapping', 'crs_map','prm.id=crs_map.program_id');
  $select->join('slt_course_template', 'crs','crs_map.course_id=crs.id');
  $select->addExpression('COUNT(crs_map.id)', 'cnt');
  $select->condition('crs_map.is_required','Y','=');
  $select->condition('crs.status','lrn_crs_sts_atv','=');
  $select->condition('prm.id',$objectid,'=');
  $no_of_courses_to_be_complete = $select->execute()->fetchField();

  return $no_of_courses_to_be_complete;
  }catch (Exception $ex) {
    watchdog_exception('getCoursestobeComplete', $ex);
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


function getRegisteredStatusForCourse($courseid,$userId){
  try{
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(id)', 'registered_class');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));
  // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  $select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  expDebug::dPrintDBAPI('getRegisteredStatusForCourse ', $select);
  $registeredCnt = $select->execute()->fetchField();
  return $registeredCnt;
  }catch (Exception $ex) {
    watchdog_exception('getRegisteredStatusForCourse', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Get the count of Registered Class.
 *
 * @param string $courseid,$classid
 *
 * @return
 *   count of the Registered Class.
 */


function getRegisteredOrNot($courseid,$classid,$userId='',$widgetId = '',$isCompliance='',$cron='',$page='',$masenrId='',$callfrom=''){
  try{
    global $catalog_reg;
  if($userId=='')
  	$userId = getSltpersonUserId();
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('enroll','id', 'enrolled_id');
  $select->addField('enroll','reg_status', 'enrolled_status');
  $select->addField('enroll','reg_date', 'reg_date');
  $select->addField('enroll','comp_date', 'comp_date');
  $select->addField('enroll','comp_status', 'comp_status');
  $select->addField('enroll','waitlist_flag', 'waitlist_flag');
  $select->addField('enroll','waitlist_priority', 'waitlist_priority');
  $select->addField('enroll','master_enrollment_id', 'master_enrollment_id');
  if($callfrom=='class_details'){
  	$select->addField('enroll','reg_date', 'reg_date');
  	$select->addField('enroll','comp_date', 'comp_date');
  	$select->addField('enroll','comp_by', 'comp_by');
  	$select->addField('enroll','comp_on', 'comp_on');
  	$select->addField('enroll','created_by', 'created_by');
  }
  //if($catalog_reg != 'Course' || ($widgetId != 'catalog-search' && $catalog_reg == 'Course')){
  expDebug::dPrint('class ids'.print_r($classid, 1), 4);
  //$select->condition('enroll.class_id', $classid, '=');
  if($page =='lnr-Catalog'){
  	$class_arr = explode('-',$classid);
  }else{
  	$class_arr = explode(',',$classid);
  }
  $select->condition('enroll.class_id', $class_arr, 'IN');
  //}
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  if(!empty($enrollId))
  	$select->condition('enroll.id',$enrollId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl'), 'IN'));
  // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  //$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  if(!empty($masenrId)){
   $select->condition('enroll.master_enrollment_id','', 'IS NOT NULL');
  }
  //if($page =='lnr-Catalog' && $masenrId == NULL) {
  	//$select->condition('enroll.master_enrollment_id','', 'IS NULL');
  //}
  if($cron == 'cron' || $cron == 'login'){
  	$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_att\'),1=1)');
  }else{
  	$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  }
  if($isCompliance == 1){ // Added by Shobana for Compliance Re-registration work flow.
  	$select->leftJoin('slt_course_template','crs', 'crs.id=enroll.course_id');
  	$select->innerJoin('slt_notification_info','notify', 'notify.notification_code=\'compliance_expiry_remainder\'');
  	$select->where('if(enroll.comp_status= \'lrn_crs_cmp_cmp\' AND (crs.validity_date IS NOT NULL OR crs.validity_days IS NOT NULL),
  			if(((crs.validity_date IS NOT NULL and (curdate() > crs.validity_date)) OR (crs.validity_days IS NOT NULL and (curdate() > DATE_ADD(enroll.comp_date, interval crs.validity_days day)) )),1=2,
  			(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enroll.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
  			(DATE_SUB(crs.validity_date, interval notify.notify_before day)))) < 0)),1=1)');
  $select->orderBy('enroll.id', 'DESC');
  }else{
  	if(isset($_REQUEST['mobile_request'])){
  		$select->orderBy('enroll.id', 'DESC');
  	}else{
  	$select->orderBy('enroll.comp_status', 'ASC');
  }
  }
  $select->range(0, 1);
  expDebug::dPrintDBAPI('getRegisteredOrNot ', $select);
  $enrolled = $select->execute()->fetchObject();
  return $enrolled;
  }catch (Exception $ex) {
    watchdog_exception('getRegisteredOrNot', $ex);
    expertusErrorThrow($ex);
  }
}

function getClassEnrollmentId($courseid = '', $classid = '', $userId = '',$masterenrcheck = '') {
	try{
		$class_arr = explode(',',$classid);
		$select = db_select('slt_enrollment', 'enroll');
		$select->addField('enroll','id', 'enrolled_id');
		$select->condition('enroll.course_id', $courseid, '=');
		$select->condition('enroll.user_id',$userId,'=');
		// Added condition for #0079234.
		$select->condition('enroll.reg_status',array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
		$select->condition('enroll.class_id', $class_arr, 'IN');
		if($masterenrcheck != 1)
			$select->condition('enroll.master_enrollment_id','', 'IS NULL');
		$select->orderBy('enroll.id', 'DESC');
		$select->range(0, 1);
		expDebug::dPrintDBAPI('getClassEnrollmentId ', $select);
		$enrollId = $select->execute()->fetchField();
		return $enrollId;
	}catch (Exception $ex) {
    watchdog_exception('getClassEnrollmentId', $ex);
    expertusErrorThrow($ex);
  }
}
function getProgressFromEnrollmentId($enrollID = '',$masterEnrollId = '') {
	try{
		$select = db_select('slt_enrollment', 'enroll');
		$select->addField('enroll','progress', 'progress');
		$select->condition('enroll.id', $enrollID, '=');
		if($masterEnrollId !='' && $masterEnrollId >0)
			$select->condition('enroll.master_enrollment_id', $masterEnrollId, '=');
		$select->orderBy('enroll.id', 'DESC');
		$select->range(0, 1);
		expDebug::dPrintDBAPI('getProgressFromEnrollmentId ', $select);
		$progress = $select->execute()->fetchField();
		if(isset($_REQUEST["updateFrom"]) && $_REQUEST["updateFrom"]=='TP'){
			//$userId = getSltpersonUserId();
			$select = db_select('slt_master_enrollment', 'masenroll');
			$select->leftJoin('slt_enrollment','enroll','enroll.master_enrollment_id = masenroll.id');
			$select->addField('masenroll','id', 'master_enroll_id');
			$select->addField('masenroll','overall_status', 'overall_status');
			if(contentPlayerIsActive()) {
				$select->addField('masenroll','overall_progress', 'tp_progress');
			} else {
				$select->addField('masenroll','recertify_path', 'recertify_path');
				$select->addField('masenroll','program_id', 'program_id');
				$select->addField('masenroll','percentage_complete', 'tp_progress');
			}
			$select->condition('enroll.id', $enrollID, '=');
			$select->orderBy('masenroll.id', 'DESC');
			$select->range(0, 1);
			expDebug::dPrintDBAPI('getProgressFromEnrollmentId master progress ', $select);
			$master_enroll_progress = $select->execute()->fetchAll();
			
			if(contentPlayerIsActive()) {
				$tp_progress  = convertProgressValue($master_enroll_progress[0]->tp_progress);
			} else {
				require_once(drupal_get_path('module', 'exp_sp_lnrlearningplan').'/exp_sp_lnrlearningplan.inc');
				$objecttype = getTrainingProgramDetails($master_enroll_progress[0]->program_id, array('object_type'));
				$comp_status = getLPCourses($master_enroll_progress[0]->master_enroll_id, $master_enroll_progress[0]->program_id, $master_enroll_progress[0]->recertify_path,'GetCompStaus','',$objecttype['object_type']);
				$tp_progress  = $comp_status['comp_percentage'];
			}

			echo convertProgressValue($progress).'*~*'.$tp_progress.'*~*'.$master_enroll_progress[0]->master_enroll_id.'*~*'.$master_enroll_progress[0]->overall_status;
			exit;
		}
		if(isset($_REQUEST["calltype"]) && $_REQUEST["calltype"]=='assessment'){
			echo convertProgressValue($progress);
			exit;
		}
		return $progress;
	}catch (Exception $ex) {
		watchdog_exception('getClassEnrollmentId', $ex);
		expertusErrorThrow($ex);
	}
}
function getClassRegisterOrNotForParticularProgram($programId,$classId,$userId) {
	try{
		$select = db_select('slt_master_enrollment', 'masenroll');
		$select->leftJoin('slt_enrollment','enroll','enroll.master_enrollment_id = masenroll.id');
		$select->addField('masenroll','id', 'master_enroll_id');
		$select->addField('masenroll','overall_status', 'overall_status');
		$select->addField('enroll','id', 'enroll_id');
		$select->addField('enroll','comp_status', 'comp_status');
		$select->addField('enroll','reg_status', 'enrolled_status');
		$select->addField('enroll','comp_status', 'comp_status');
		$select->addField('enroll','waitlist_flag', 'waitlist_flag');
		$select->addField('enroll','waitlist_priority', 'waitlist_priority');
		$select->condition('masenroll.program_id', $programId, '=');
		$select->condition('masenroll.user_id', $userId, '=');
		$select->condition('enroll.class_id', $classId, '=');
		$select->orderBy('masenroll.id', 'DESC');
		$select->condition('masenroll.overall_status',array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl'), 'IN');
		$select->range(0, 1);
		expDebug::dPrintDBAPI('getClassRegisterOrNotForParticularProgram ', $select);
		$master_enroll_details = $select->execute()->fetchAll();
		return $master_enroll_details;
	}catch (Exception $ex) {
		watchdog_exception('getClassRegisterOrNotForParticularProgram', $ex);
		expertusErrorThrow($ex);
	}
}

function getMasterEnrollmentStatus($masterEnrollId = '', $enroll_id = '') {
	try{
		$validStatus = array( 'lrn_tpm_ovr_cmp', 'lrn_tpm_ovr_enr', 'lrn_tpm_ovr_inp');
		$select = db_select('slt_master_enrollment', 'master_enroll');
		$select->addField('master_enroll','id', 'enrolled_id');
		$select->condition('master_enroll.id', $masterEnrollId, '=');
		$select->condition('master_enroll.overall_status', $validStatus, 'IN');
		$select->range(0, 1);
		expDebug::dPrintDBAPI('getMasterEnrollmentStatus ', $select);
		$masterEnrollId = $select->execute()->fetchField();
		return ($masterEnrollId > 0) ?  $enroll_id : 0;
	}catch (Exception $ex) {
		watchdog_exception('getMasterEnrollmentStatus', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * To fetch the enrollments for Catalog course level view
 */
function getRegisteredOrNotForCourseLevel($courseid,$classid,$userId='',$widgetId = '',$isCompliance='',$cron='',$page='',$masenrId=''){
	try{
		global $catalog_reg;
		$userId = $userId == '' ? getSltpersonUserId() : $userId;

		 $select = db_select('slt_enrollment','t1');
		 $select->addField('t1','id','enrolled_id');
		 $select->addField('t1','reg_status', 'enrolled_status');
		 $select->addField('t1','comp_status','comp_status');
		 $select->addField('t1','waitlist_flag','waitlist_flag');
		 $select->addField('t1','waitlist_priority','waitlist_priority');
		 $select->addField('t1','master_enrollment_id','master_enrollment_id');
		 $select->where('t1.id = (SELECT MAX(t2.id) FROM slt_enrollment t2
		 WHERE t2.user_id = t1.user_id
		 and t2.class_id = t1.class_id)');
		 $select->condition('t1.course_id', $courseid, '=');
		 $select->condition('t1.user_id', $userId, '=');
		$clsIds = explode('-', $classid);
		if(count($classid) > 0){
		 $select->condition('t1.class_id', $clsIds, 'IN');
		}
		
		expDebug::dPrintDBAPI('select enrollments ', $select, 4);

 		if(!empty($masenrId)){
			$select->condition('t1.master_enrollment_id','', 'IS NOT NULL');
		}
/*		if($page =='lnr-Catalog' && $masenrId == NULL) {
			$select->condition('enroll.master_enrollment_id','', 'IS NULL');
		} */

		expDebug::dPrintDBAPI('getRegisteredOrNot ', $select);
		$enrolled = $select->execute()->fetchAll();
		$enrollments  = array();
		foreach ($enrolled as $enroll) {
// 			$select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl'), 'IN'));
// 			$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
			if((($enroll->enrolled_status == 'lrn_crs_reg_cnf' && ($enroll->comp_status == 'lrn_crs_cmp_cmp' || $enroll->comp_status == 'lrn_crs_cmp_inp' || $enroll->comp_status == 'lrn_crs_cmp_enr' || $enroll->comp_status == 'lrn_crs_cmp_att')) || $enroll->enrolled_status == 'lrn_crs_reg_ppm' || $enroll->enrolled_status == 'lrn_crs_reg_wtl') ) {
				$enrollments[] = $enroll;
			}
		}
		expDebug::dPrint('results from enrollments '.print_r($enrolled, 1));
		expDebug::dPrint('results from confirmed '.print_r($enrollments, 1));
		return $enrollments;
	}catch (Exception $ex) {
		watchdog_exception('getRegisteredOrNot', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Get the Last completed class from course.
 *
 * @param string $courseid
 *
 * @return
 *   Last completed enrolled_id,comp_status,comp_on.
 */


function getLastCompCourseClass($courseid,$userId='',$class_id,$menrId){
  try{
    global $catalog_reg;
  if($userId=='')
  	$userId = getSltpersonUserId();
  
  $select = db_select('slt_enrollment', 'enroll');
  $select->addField('enroll','id', 'enrolled_id');
  $select->addField('enroll','comp_status', 'comp_status');
  $select->addField('enroll','comp_on', 'comp_on');  
  
  // Modified by Vincent on 27 Jan 2017. For #0071682
  $select->addField('enroll','comp_by', 'comp_by'); 
  $select->addField('enroll','comp_date', 'comp_date');  
  $select->addField('enroll','score', 'score');  
  $select->addField('enroll','content_status', 'content_status');  
  $select->addField('enroll','comp_by', 'comp_by'); 
  $select->addField('enroll','comp_date', 'comp_date');  
  $select->addField('enroll','score', 'score');  
  $select->addField('enroll','content_status', 'content_status');  
  $select->addField('enroll','progress', 'progress');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  if(!empty($menrId)){
	$select->condition('enroll.class_id', $class_id, '=');
  }
  $select->condition(db_and()->condition('enroll.comp_status',array('lrn_crs_cmp_cmp'), 'IN'));
  $select->orderBy('enroll.comp_on', 'DESC'); 
  $select->range(0, 1);
  expDebug::dPrintDBAPI('getLastCompCourseClass ', $select);
  $enrolled = $select->execute()->fetchObject();
  return $enrolled;
  }catch (Exception $ex) {
    watchdog_exception('getLastCompCourseClass', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 * Get the count of Registered Class.
 *
 * @param string $courseid,$classid
 *
 * @return
 *   count of the Registered Class.
 */
function getObjectRegisteredOrNot($objectid,$userId='',$recertpath){
  try{
  if($userId=='')
  	$userId = getSltpersonUserId();

  $select = db_select('slt_master_enrollment', 'master_enroll');
  $select->leftJoin('slt_profile_list_items','prof','prof.code = master_enroll.overall_status');
  $select->leftJoin('slt_program','p','p.id = master_enroll.program_id');
  $select->addField('master_enroll','id', 'master_enrolled_id');
  $select->addField('master_enroll','overall_status', 'master_enrolled_status');
  $select->addField('prof','name', 'master_status_name');
  $select->addField('master_enroll','recertify_path', 'recertify_path');
  $select->addField('master_enroll','comp_date', 'comp_date');
  $select->addField('master_enroll','percentage_complete', 'percentage_complete');
  $select->addField('master_enroll','overall_progress', 'overall_progress');
  $select->addField('p','object_type', 'object_type');
  $select->addField('p','expires_in_value', 'expires_in_value');
  $select->addField('p','expires_in_unit', 'expires_in_unit');
  $select->condition('master_enroll.program_id', $objectid, '=');
  $select->condition('master_enroll.user_id',$userId,'=');
  if (!empty($recertpath)) {
  $select->condition('master_enroll.recertify_path',$recertpath,'=');
  }
  $select->condition(db_and()->condition('master_enroll.overall_status',array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppv','lrn_tpm_ovr_ppm','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl'), 'IN'));

  $master_enrolled = $select->execute()->fetchObject();
	expDebug::dPrintDBAPI("Check whether TP register or not ",$select);
  return $master_enrolled;
  }catch (Exception $ex) {
    watchdog_exception('getObjectRegisteredOrNot', $ex);
    expertusErrorThrow($ex);
  }
}

function getPrgWaitlist($courseId,$userId = ''){
  try{
  $selectStmt = db_select('slt_course_class', 'cls');
  $selectStmt->leftJoin('slt_course_template', 'crs', 'cls.course_id = crs.id');
  $selectStmt->leftjoin('slt_course_class_session', 'ses','ses.class_id=cls.id AND ses.course_id=cls.course_id');
  $selectStmt->leftjoin('slt_course_content_mapper', 'content','content.class_id=cls.id AND content.course_id=cls.course_id');
  $selectStmt->leftjoin('slt_profile_list_items', 'master_view','master_view.code=cls.delivery_type');
  $selectStmt->condition('crs.id', $courseId);
  $selectStmt->addField('cls', 'id', 'class_id');
  $selectStmt->addField('cls', 'delivery_type', 'del_type');
  $selectStmt->where('IF((master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\'), (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = ses.class_id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  NOW() = 0), 1=1)');
  $selectStmt->where('IF ((master_view.attr1=\'WBT\'), content.id IS NOT NULL, 1=1)');
  $selectStmt->condition('cls.status', 'lrn_cls_sts_atv');
  $retResult = $selectStmt->execute()->fetchAll();
	expDebug::dPrintDBAPI("getPrgWaitlist",$selectStmt);
  $nowaitlisted = 0;
  foreach ($retResult as $record) {
  	if($record->del_type == 'lrn_cls_dty_ilt' || $record->del_type == 'lrn_cls_dty_vcl') {
	 		$wlc = getWaitlistCatalogInfo($record->class_id,$userId,'tp');
 			if($wlc!='NA')
	 			$nowaitlisted=$nowaitlisted+$wlc;
  	}
  }
	expDebug::dPrint("Get Course Level Waitlist count :".$nowaitlisted,5);
  return $nowaitlisted;
  }catch (Exception $ex) {
    watchdog_exception('getPrgWaitlist', $ex);
    expertusErrorThrow($ex);
  }

}

function getProgramWaitlistStatus($programId,$userId,$isAdminSide = 'N',$recertifyValue=''){
  try{

  $maxCount = getCourseClassCountForLP($programId,$userId,$isAdminSide,$recertifyValue);

  if($maxCount[0]->cnt == '-1' || $maxCount[0]->cnt == 0) {
    $waitCnt = 0;
  } else {
    $select = db_select('slt_program', 'lp');
    $select->innerJoin('slt_module', 'module', 'lp.id = module.program_id');
    // Modified by Vincent on 27 Jan 2017. For #0071682
    if($recertifyValue=='R'){
    	$select->innerJoin('slt_re_certify', 'map','lp.id = map.program_id AND map.module_id = module.id');
    }else{
    $select->innerJoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id AND map.module_id = module.id');
    }
    $select->innerJoin('slt_course_class', 'cls', 'cls.course_id = map.course_id and cls.status=\'lrn_cls_sts_atv\' and cls.delivery_type in(\'lrn_cls_dty_ilt\',\'lrn_cls_dty_vcl\')');
    $select->addField('map', 'course_id', 		'course_id');
    $select->condition('lp.id', $programId, '=');
    $select->groupBy('map.course_id');
    $retResult = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI("Courses of TP : ",$select);
    $waitCnt = 0;
    $waitlist=array();
    $tmp1=0;
    $tmp2=0;
    $isFirst=0;
    foreach ($retResult as $record) {
    	$wl = getPrgWaitlist($record->course_id,$userId);
	    	if($isFirst==0){
					$tmp1=$wl;
					$tmp2=$wl;
					$isFirst=1;
				}
	    	if($tmp1>=$wl){
	    	   $tmp1=$wl;
	    	}else if(($tmp2<=$wl)){
	    		$tmp2=$wl;
	    	}
    }
   }
   if($tmp1==$tmp2){
   		$waitCnt=$tmp1;
   }else if($tmp1<$tmp2){
   		if($tmp1!=0){
   			$waitCnt=$tmp1;
   		}else{
   			$waitCnt=0;
   		}
   }
   expDebug::dPrint("Course Level waitlist count tmp1 ".$tmp1,5);
   expDebug::dPrint("Course Level waitlist count tmp2 ".$tmp2,5);
   expDebug::dPrint("Program Level waitlist count ".$waitCnt,5);
  return $waitCnt;
  }catch (Exception $ex) {
    watchdog_exception('getProgramWaitlistStatus', $ex);
    expertusErrorThrow($ex);
  }

}

/**
 * Get number of available seats for particular class.
 *
 * @param string $classid
 *   The class id.
 *
 * @return
 *   count of the available seats.
 */
function getAvailableSeats($classid){
  try{
  $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
  $no_of_confirmed = getClassEnrollmentCountForRegistrationStatuses(null, $classid, $regStatusesList);

  $select = db_select('slt_course_class_session', 'sc');
  $select->addField('sc','capacity_max', 'capacity_max');
  $select->condition('sc.class_id',$classid, '=');
  $select->range(0, 1);
  $qry_result_session = $select->execute()->fetchObject();
	expDebug::dPrintDBAPI("Class max seats for check seat availability : ",$select);
  if ($qry_result_session->capacity_max != 0) {
    $no_of_available_seats = $qry_result_session->capacity_max - $no_of_confirmed;
  }

  if ($no_of_available_seats < 0 ) {
    $no_of_available_seats = 0;
  }
  expDebug::dPrint("Available Seat count : ".$no_of_available_seats,5);
  return $no_of_available_seats;
  }catch (Exception $ex) {
    watchdog_exception('getAvailableSeats', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}

function callbackGetAvailableSeats($classid,$waitlist = false,$userId) {
  try{
  $avlSeatInfo = '';
  if($waitlist) {
    $availSeats   = getWaitlistPosition($classid,0,$userId);
    $avlSeatInfo  = t('LBL125').' '.$availSeats;
  } else {
    $availSeats   = getAvailableSeats($classid);
    $avlSeatInfo  = $availSeats.' '.($availSeats == 1 ? t('LBL052') : t('LBL053'));
  }
  print $avlSeatInfo;
  }catch (Exception $ex) {
    watchdog_exception('callbackGetAvailableSeats', $ex);
    expertusErrorThrow($ex);
  }
}
function getObjectdeliveryType($programId,$userId = '',$recertifyValue = ''){
try{
		$programId = orgValidate($programId);
		if(strpos($recertifyValue, 'R')>0){
			$recertify = explode('-',$recertifyValue);
			$path = $recertify[0]+1;
			$modId = getModuleIdFromObjectId($programId,$path);
		}
		else{
			$modId = getModuleIdFromObjectId($programId,1);
		}
		/*if($recertifyValue == 'R'){
			$availClass = db_query("SELECT cls.id classid,crs.course_id courseid ,cls.max_seats capmax, cls.delivery_type deltype
							FROM
							slt_course_class cls
							INNER JOIN slt_re_certify crs ON crs.course_id = cls.course_id
					INNER JOIN slt_course_class_session ses ON ses.class_id=cls.id AND ses.course_id=cls.course_id AND ses.id=(select id from slt_course_class_session where class_id=cls.id limit 1)
					LEFT JOIN  slt_profile_list_items master_view ON (master_view.code=cls.delivery_type)
					WHERE crs.program_id = '".$programId."' AND crs.module_id='".$modId[0]->module_id."'
					AND cls.status = 'lrn_cls_sts_atv'
					AND IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), concat(date_format(start_date,'%Y-%m-%d'),' ',start_time) >= NOW(), 1=1)
					GROUP BY  cls.course_id,cls.id");
		}else{*/
			$availClass = db_query("SELECT cls.id classid,crs.course_id courseid ,cls.max_seats capmax, cls.delivery_type deltype
					FROM
					slt_course_class cls
					INNER JOIN slt_module_crs_mapping crs ON crs.course_id = cls.course_id
					LEFT JOIN slt_course_class_session ses ON ses.class_id=cls.id AND ses.course_id=cls.course_id AND ses.id=(select id from slt_course_class_session where class_id=cls.id limit 1)
					LEFT JOIN  slt_profile_list_items master_view ON (master_view.code=cls.delivery_type)
					WHERE crs.program_id = '".$programId."' AND crs.module_id='".$modId[0]->module_id."'
					AND cls.status = 'lrn_cls_sts_atv'
					AND IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), concat(date_format(start_date,'%Y-%m-%d'),' ',start_time) >= NOW(), 1=1)
					GROUP BY  cls.course_id,cls.id");
		//}
		foreach($availClass as $rowClass){
			$deltype[] = $rowClass->deltype;
		}
		return $deltype;
	}
	catch (Exception $ex) {
		watchdog_exception('getObjectdeliveryType', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Get number of available seats for particular Object like Certification,Curricula and Learning plan.
 *
 * @param string $classid
 *   The class id.
 *
 * @return
 *   count of the available seats for Object like Certification,Curricula and Learning plan.
 */
function getObjectAvailableSeats($programId,$userId = '',$recertifyValue = ''){
	try{
		$programId = orgValidate($programId);
		
		if(strpos($recertifyValue, 'R')>0){
			$recertify = explode('-',$recertifyValue);
			$path = $recertify[0]+1;
			$modId = getModuleIdFromObjectId($programId,$path);
		}
		else{
			$modId = getModuleIdFromObjectId($programId,1);
		}
		 
		/*
		if($recertifyValue == 'R'){
			$availClass = db_query("SELECT cls.id classid,crs.course_id courseid ,cls.max_seats capmax, cls.delivery_type deltype
							FROM
							slt_course_class cls
							INNER JOIN slt_re_certify crs ON crs.course_id = cls.course_id
							INNER JOIN slt_course_class_session ses ON ses.class_id=cls.id AND ses.course_id=cls.course_id AND ses.id=(select id from slt_course_class_session where class_id=cls.id limit 1)
							LEFT JOIN  slt_profile_list_items master_view ON (master_view.code=cls.delivery_type)
							WHERE crs.program_id = '".$programId."'
							AND cls.status = 'lrn_cls_sts_atv'
							AND IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), concat(date_format(start_date,'%Y-%m-%d'),' ',start_time) >= NOW(), 1=1)
							GROUP BY  cls.course_id,cls.id");
		}else{
		*/
			$availClass = db_query("SELECT cls.id classid,crs.course_id courseid ,cls.max_seats capmax, cls.delivery_type deltype
							FROM
							slt_course_class cls
							INNER JOIN slt_module_crs_mapping crs ON crs.course_id = cls.course_id
							INNER JOIN slt_course_class_session ses ON ses.class_id=cls.id AND ses.course_id=cls.course_id AND ses.id=(select id from slt_course_class_session where class_id=cls.id limit 1)
							LEFT JOIN  slt_profile_list_items master_view ON (master_view.code=cls.delivery_type)
							WHERE crs.program_id = '".$programId."' AND crs.module_id='".$modId[0]->module_id."'
							AND cls.status = 'lrn_cls_sts_atv'
							AND IF ((master_view.attr1='ILT' OR master_view.attr1='VC'), concat(date_format(start_date,'%Y-%m-%d'),' ',start_time) >= NOW(), 1=1)
							GROUP BY  cls.course_id,cls.id");
// 		}
	$seatAvailable = -1;
	$isIltInvoked = 0;
	$minSeatArray = array();
  foreach($availClass as $rowClass){
    expDebug::dPrint("availClass". print_r($rowClass,true),3);
    $deliveryType = $rowClass->deltype;
    expDebug::dPrint('$deliveryType = ' . print_r($deliveryType, true) , 4 );
    if ($deliveryType == 'lrn_cls_dty_vcl' || $deliveryType == 'lrn_cls_dty_ilt') {
      $isEnrolled = 0;
      if($userId){
        $regStatusList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
        $isEnrolled = isUserEnrolledClass($userId,$rowClass->classid,$regStatusList);
      }
        // if(!$isEnrolled){ // 76452: Button is wrongly showing WAITLIST...
        $isIltInvoked = 1;
        $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
        $enrollCount = getClassEnrollmentCountForRegistrationStatuses(null, $rowClass->classid, $regStatusesList);

        $maxSeats = ($rowClass->capmax) ? $rowClass->capmax : 0;
        expDebug::dPrint("seats  : Classid - ". $rowClass->classid . " Max - " .$maxSeats . " enrollCount -" . $enrollCount ,4);
        $seatAvailable =  $maxSeats - $enrollCount;
        if (!isset($minSeatArray[$rowClass->courseid])) {
        	$minSeatArray[$rowClass->courseid] = $seatAvailable;
        }
        else {
          $minSeatArray[$rowClass->courseid] = max($minSeatArray[$rowClass->courseid], $seatAvailable);
        }
        // } 
      }
    }
  if($isIltInvoked){
    return min($minSeatArray);
  }
  return $seatAvailable;
	}catch (Exception $ex) {
    watchdog_exception('getObjectAvailableSeats', $ex);
    expertusErrorThrow($ex);
    }
}

function isUserEnrolledClass($userId,$classId,$regStatusesList){
	try{
  $selectStmt = db_select('slt_enrollment', 'enr');
  $selectStmt->addExpression('COUNT(id)', 'registered_class');
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.user_id', $userId);
  $selectStmt->condition('enr.reg_status', $regStatusesList, 'IN');
  // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
  $selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute()->fetchField();
  expDebug::dPrint(' $result $regStatusCount = ' .$result , 4);
  return $result;
	}catch (Exception $ex) {
    watchdog_exception('isUserEnrolledClass', $ex);
    expertusErrorThrow($ex);
    }
}
function callbackGetObjectAvailableSeats($programId) {
  try{
  $availSeats = getObjectAvailableSeats($programId);
  print $availSeats.' '.($availSeats == 1 ? t('Seat left') : t('Seats left'));
  }catch (Exception $ex) {
    watchdog_exception('callbackGetObjectAvailableSeats', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * isActiveClass() - Both course and class should be active (status = lrn_crs_sts_atv) and
 *                   class must belong to the course for this function to return true.
 */
function isActiveClass($courseId, $classId, $fromAPI = false) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class', 'cls');
  // Join other tables.
  $selectStmt->leftJoin('slt_course_template', 'crs', 'cls.course_id = crs.id');
  // Set conditions
  $selectStmt->condition('cls.id', $classId);
  $selectStmt->condition('crs.id', $courseId);
  // Select fields and/or add expressions
  $selectStmt->addField('crs', 'status', 'course_status');
  $selectStmt->addField('cls', 'status', 'class_status');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
//   expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  if ($result->rowCount() <= 0) {
    return false; // Either class is not present, or course is not present or class does not belong to the course
  }
  $statuses = $result->fetchAssoc();
  expDebug::dPrint(' API Call = '.$fromAPI);
  if($fromAPI == 1){
  	if(($statuses['course_status'] == 'lrn_crs_sts_atv' || $statuses['course_status'] == 'lrn_crs_sts_itv') && ($statuses['class_status'] == 'lrn_cls_sts_atv' || $statuses['class_status'] == 'lrn_cls_sts_itv' ||  $statuses['class_status'] == 'lrn_cls_sts_dld')){
  		return true; // Either (or both) class and course are active Or Class is in Delivered status.
  	}else{
  		return false;
  	}
  }else{
  	if ($statuses['course_status'] != 'lrn_crs_sts_atv' || $statuses['class_status'] != 'lrn_cls_sts_atv') {
  		return false; // Either (or both) class and course are not active.
  	}
  }
  return true;
  }catch (Exception $ex) {
    watchdog_exception('isActiveClass', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getClassPrice() - Get a Price added by yogaraja.
*/
function getClassPrice($classId) {
	try{
		// Select table
		$selectStmt = db_select('slt_course_class', 'cls');

		// Set conditions
		$selectStmt->condition('cls.id', $classId);

		// Select fields and/or add expressions
		$selectStmt->addField('cls', 'price', 'price');
		// Execute query
		$result = $selectStmt->execute();
		$price = $result->fetchField();
		if(is_null($price))
			$price = '0.00';
		return $price;
	}catch (Exception $ex) {
		watchdog_exception('getClassPrice', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * getClassTitle() - Get a class' title.
 */
function getClassTitle($classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_course_class', 'cls');
  // Set conditions
  $selectStmt->condition('cls.id', $classId);
  // Select fields and/or add expressions
  $selectStmt->addField('cls', 'title', 'title');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getClassTitle', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getWaitlistUsersOfClass() - Fetch the list of all waitlisted users of a class.
 */
function getWaitlistUsersOfClass($courseId, $classId) {
  try{
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $selectStmt->condition('enr.course_id', $courseId);
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.reg_status', 'lrn_crs_reg_wtl');
  // Select fields
  $selectStmt->addField('enr', 'id', 'enrollment_id');
  $selectStmt->addField('enr', 'user_id', 'user_id');
  expDebug::dPrintDBAPI(' $selectStmt = '  , $selectStmt);
  // Execute query
  $result = $selectStmt->execute();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result->fetchAllKeyed();
  }catch (Exception $ex) {
    watchdog_exception('getWaitlistUsersOfClass', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * registerEndDateCheck() - checking registration end date less than today.
 */
function registerEndDateCheck($courseId, $classId) {
  try{
  $selectStmt = db_select('slt_course_class', 'sc');
  $selectStmt -> addField('sc', 'registration_end_on', 'registration_end_on');
  $selectStmt -> condition('sc.course_id', $courseId);
  $selectStmt -> condition('sc.id', $classId);
  $result = $selectStmt->execute()->fetchObject();

  $status = 0;
  $startdate = Date("Y-m-d");
  if($result->registration_end_on !='' || $result->registration_end_on != null) {
    if (strtotime($result->registration_end_on) < strtotime($startdate)) {
      $status = 1;
    }
  }
  return $status;
  }catch (Exception $ex) {
    watchdog_exception('registerEndDateCheck', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}

function ifadminUser($pid)
{
	try{ 
		
	$select = db_select('slt_person', 'per');
	$select->innerJoin('users','usr','usr.name=per.user_name and usr.status=1');
	$select->innerJoin('users_roles','usrl','usrl.uid=usr.uid');
	$select->innerJoin('role','rl','rl.rid=usrl.rid');
	$select->innerJoin('slt_groups','grp','grp.is_admin=1 and grp.name=rl.name');
	$select->addExpression('count(*)', 'count');	
	$select->condition('grp.status', 'cre_sec_sts_atv', '=');
	$select->condition('per.id', $pid, '=');	
	$result = $select->execute()->fetchField();	
	return $result;	
		
	}catch (Exception $ex) {
		watchdog_exception('ifadminUser', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * enrollUserToClass() - Enrolls a user to a class. Replaces stored procedure slp_enrollmentlnrassignment_ins()
 * Argument xactive is dropped as was not being used.
 * xrtnid and xrtnvalue are now returned by this function as an associative array (id => xrtnid, value => xrtnvalue)
 */
function enrollUserToClass($userId, $orderId, $userFullName, $courseId, $classId, $forceRegister, $updateUserId, $cartFlag,
                                                                        $vClassNotify, $MadatoryMang="", $regDate = '', $isAdminSide = 'N',$extraParam =  array(), $fromAPI = false,$fromCloneEnrId='',$fromCloneClsId='') {

  expDebug::dPrint(' In enrollUserToClass() with $vClassNotify = ' . $vClassNotify , 4);
 // $txn = db_transaction();
  try{
  // Placeholder for the returned user enrollment status (id and value).
  
  	$loggedInUserId = getSltpersonUserId();
   	
  	$check_user_admin = isset($_REQUEST['apiname']) && isset($_REQUEST['userid']) ? $_REQUEST['userid'] : $loggedInUserId;
  	$apinamenotify = trim($_REQUEST['apiname']);
  	$isadmingrp = ifadminUser($check_user_admin);
  	
   
  $enrollmentStatus = array('id' => null, 'value' => null,'enrollid'=>null,'orderId' => null);
  $selectIsComp = db_select('slt_course_class', 'class');
  $selectIsComp->addField('class','is_compliance');
  $selectIsComp->condition('class.id', $classId);
  
  $waiveduser = getRegisteredEnrollmentExmeptedorNot('class',$userId,$courseId);
  if(empty($waiveduser)){  
  	$isCompliance = $selectIsComp->execute()->fetchField();
  }else{
  	$isCompliance = null;
  	$MadatoryMang = null;
  }
  // If ilt or vc class, check against capacity
  $deliveryType = getClassDeliveryType($classId, 'Short');

  // Cannot proceed if invalid User id or Class id
  if ($userId <= 0 || $classId <= 0) {
    $enrollmentStatus['id'] = -4; //InvalidUser
    $enrollmentStatus['value'] = 'Invalid user or class';
    return $enrollmentStatus;
  }
  expDebug::dPrint(' enrollUserToClass Status = '.$isAdminSide);
  
  $manCnt = getMandatoryOrNot($classId,$userId,'');  
  $clsmaxcnt = getIfSingleClass($courseId,'',$isCompliance);
 
  if(($isAdminSide=="Y") 
  		|| ($apinamenotify=="AddRosterAPI") 
  		|| (($apinamenotify=="RegistrationAPI") && ($isadmingrp > 0)) 
  		|| (($isCompliance==1 || $manCnt=='cre_sys_inv_man') &&  $clsmaxcnt[0]->maxcnt==1 && $isAdminSide=="N")) {
  	$isAdminSide="Y"; 
  } else { 
  	$isAdminSide="N";
  }
  expDebug::dPrint("test isAdminSide".$isAdminSide,5);

  
  // Both course and class should be active (status = lrn_crs_sts_atv) and
  // class must belong to the course
    if (in_array($isAdminSide,array('N','Nlnrsearch')) && !isActiveClass($courseId, $classId, $fromAPI)) {
    $enrollmentStatus['id'] = -6; // Invalid course or class
    $enrollmentStatus['value'] = 'Invalid course or class';
    return $enrollmentStatus;
  }

  // Confirm that the user is not already enrolled to the class
  $enrollmentExists = enrollmentExistsCheck ($userId, $classId, 'Class');
  expDebug::dPrint(' $enrollmentExists = ' . print_r($enrollmentExists, true) , 4);
  
  $isNotComplianceReg = 1;
  if(($extraParam['regFrom'] == 'compliance_reregistration' && ($deliveryType == 'wbt' || $deliveryType == 'vod')) ) {
  	$isNotComplianceReg = 0;
  }
  if ($enrollmentExists > 0 && $isNotComplianceReg) {
    if ($enrollmentExists == 2) {
		$enrollmentStatus['id'] = -7; // Already reserved
    }
    else {
		$enrollmentStatus['id'] = -1; // Already enrolled.
    }
    $classTitle = getClassTitle($classId);
	  $enrollmentStatus['value'] = 'Title:' . $classTitle;
    return $enrollmentStatus;
  }
  $expired = GetIfExpired($courseId,$userId);
  // completion date check

  $PastcompDate= 0;
  if($apinamenotify=="RegistrationAPI" || $apinamenotify =="AddRosterAPI"){
  $PastcompDate  =CheckCourseCompExpiry($courseId);
  }
  if($expired || $PastcompDate){
  	$enrollmentStatus['id'] =-12;
  	$enrollmentStatus['value'] = 'Course is expired for the user.';
  	return $enrollmentStatus;
  }
  $is_comp = ($isCompliance == 1) ? $isCompliance : 0;

  // Confirm that the user is not already enrolled for another class in the same course, if multiple enrollments are not allowed
  // and eCommmerce is off ($cartFlag == 'N')
  $courseConflict = courseConflictCheck($userId, $courseId, $classId, 'Class',$is_comp);

  // While course is compliance, then course conflict is not applicable
  $courseConflict = ($extraParam['regFrom'] == 'compliance_reregistration') ? 0 : $courseConflict;

  expDebug::dPrint(' $courseConflict = ' . print_r($courseConflict, true) , 4);

  if ($cartFlag == 'N' && $courseConflict <> 0) {
    $enrollmentStatus['id'] = -8; // Course conflict - user already enrolled for another class in the same course, etc.
    $enrollmentStatus['value'] = null;
    return $enrollmentStatus;
  }

  // Confirm that this class does not clash with any other class to which the user has already enrolled.
  $conflictingClassId = enrollmentsConflictCheck($userId, $classId, 'Class');
  expDebug::dPrint(' $conflictingClassId = ' . print_r($conflictingClassId, true) , 4 );
  if ($conflictingClassId > 0 ) {
	$enrollmentStatus['id'] = -2; //Class conflict - user has another class whose timings conflict with this class
	$conflictingClassTitle = getClassTitle($conflictingClassId);
	$enrollmentStatus['value'] = 'Title::' . $conflictingClassTitle;
	return $enrollmentStatus;
  }
  $deliveryType = getClassDeliveryType($classId, 'Short');
  $Instr_check = is_instructor($userId);
  if(($deliveryType == 'ilt' || $deliveryType == 'vc') && $Instr_check == true){
	  // Confirm that the user not assigned this class to instruct for learners
	  $conflictInstructor = getClassInstructorConflict($userId, $classId);
	  expDebug::dPrint(' $conflictInstructor = ' . $conflictInstructor, 4);
	  if ($conflictInstructor == false ) {
		$enrollmentStatus['id'] = -10; //Instructor conflict - User has already registered as instructor for this class
		return $enrollmentStatus;
	  }

	  // Confirm that the user not scheduled to some other class in same session time as instructor
	  $conflictInstructorSession = getSessionClassInstructorConflict($userId, $classId);
	  expDebug::dPrint(' $conflictsessionInstructor = ' . $conflictInstructorSession, 4);
	  if ($conflictInstructorSession > 0 ) {
	  	$enrollmentStatus['id'] = -2; //Class conflict - user has another class whose timings conflict with this class
	  	return $enrollmentStatus;
	  }

  }
  if(($deliveryType == 'ilt' || $deliveryType == 'vc')){
	  //session conflict with completion date
	  $sessCompConflict = getSessionandCompletionConflict($userId, $classId);
	  expDebug::dPrint("session conflict with completion date check". $sessCompConflict,5);
	  if($sessCompConflict) {
	  	$enrollmentStatus['id'] = -11; //Class conflict - user has another class whose timings conflict with this class
	  	return $enrollmentStatus;
	  }
  }
  $regStatus = null;
  $waitlistNumber = 0;
  $completionStatus = null;
  $maxCapacityReached = false;
  $allowRegistration = 0;
  $launchable  = 1;
  if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
    $waitlistNumber = waitListCheck($courseId, $classId);
    expDebug::dPrint(' $waitlistNumber = ' . print_r($waitlistNumber, true) , 4);

    if ($waitlistNumber == 0 || $waitlistNumber == -1) { // confirmed
			if ($cartFlag == 'Y') {
				$regStatus = 'lrn_crs_reg_rsv';
			}
			else {
				$regStatus = 'lrn_crs_reg_cnf';
				$completionStatus = 'lrn_crs_cmp_enr';
			}

	    if ($waitlistNumber == -1) {
	      $maxCapacityReached = true;
	    }

			$waitlistNumber = null;
    }
    else { // waitlist - do further check against waitlist capacity, otherwise class is full.
      $classWaitlistCapacity = getClassWaitlistCapacity(null, $classId);
      expDebug::dPrint(' $classWaitlistCapacity = ' . print_r($classWaitlistCapacity, true) , 4);
      $classWaitlistCapacity = (is_null($classWaitlistCapacity))? 0 : $classWaitlistCapacity;
	    if ($classWaitlistCapacity >= $waitlistNumber) {
		    $regStatus = 'lrn_crs_reg_wtl';
	    }
	    else {
		    $regStatus = 'Full';
		    $allowRegistration = 1;
	    }
    }
  }
  else { // delivery type == 'wbt' (or video)
    if ($cartFlag == 'Y') {
	    $regStatus = 'lrn_crs_reg_rsv';
    }
    else {
	    $regStatus = 'lrn_crs_reg_cnf';
	    $completionStatus = 'lrn_crs_cmp_enr';
    }
	  $waitlistNumber = null;
  }

  // Unless force register, for waitlist we do not want to insert record in slt_enrollment as user has to first confirm
  // whether interested in the waitlist seat.
  if ($forceRegister == 0 && !is_null($waitlistNumber)) {
		$allowRegistration = 1;
  }
  if ($allowRegistration != 0) {
    $classTitle = getClassTitle($classId);
    if ($regStatus == 'Full') {
	    $enrollmentStatus['id'] = -5;	  // Seats full
	    $enrollmentStatus['value'] = 'No seats are available';
    }
    else {
	    $enrollmentStatus['id'] = -3; // Waitlist
	    $enrollmentStatus['value'] = 'Title::' . $classTitle . ':|:Waitlist::' . $waitlistNumber;
    }
    return  $enrollmentStatus;
  }

  if ($MadatoryMang == "N") {
    $MadatoryMang = null;
   }   
  $waitlist_flag = null;
  if($waitlistNumber > 0) {
   $waitlist_flag = 'lrn_crs_reg_wtl';
  }

  
  //Added for launchable check
  
    if(($deliveryType == 'ilt' || $deliveryType == 'vc')){
		$launchable  = 0;
	 	$isPostAssessmentAttached = isPostAssessmentAttached($classId,'class');
	 	if($isPostAssessmentAttached == 1){
		    $launchable  = 1;
	 	}
  	}

		//if(empty($waiveduser)){
// 			$manCnt = getMandatoryOrNot($classId,$userId);
			

		//}else{
		//	$manCnt = '';
		//}
  if($isCompliance == 1){
  	$updateStmt = db_update('slt_enrollment');
  	// Set conditions
  	$updateStmt->condition('course_id',$courseId,'=');
  	$updateStmt->condition('user_id',$userId,'=');
  	$updateStmt->condition('is_compliance',1,'=');
  	$updateStmt->condition('comp_status',array('lrn_crs_cmp_exp','lrn_crs_cmp_cmp'),'IN');
  	// Set new values of fields
  	$updateStmt->fields(array(
  			'comp_status' => 'lrn_crs_cmp_cmp',
  			'cmpl_expired' => 1,
  			'updated_on'    => now(),
  			'updated_by'    => $user_id
  	));
  	expDebug::dPrintDBAPI(' $updateStmt for compliance ', $updateStmt);
  	$updateStmt->execute();
  }
	//$txn = db_transaction(); //Commented for dbStatementsExecution();
  try{
  	  $statements = array('statement' => true);
	  $aResp = CreateNewOrder($userId, $regStatus, $userId,$statements);
	  $vOrderId = $aResp['OrderId'];
	  $enrollmentStatus["orderId"] = $vOrderId;
	  

  $insertStmt = db_insert('slt_enrollment');
  $insertStmt->fields(array(
    'user_id' => $userId,
    'order_id' => $vOrderId,
    'class_id' => $classId,
    'course_id' => $courseId,
    'reg_status' => $regStatus,
    'reg_date' => ($regDate) ? $regDate :now(),
  	'reg_status_date' => ($regDate) ? $regDate :now(),
    'comp_status' => $completionStatus,
    'waitlist_priority' => $waitlistNumber,
    'waitlist_flag' => $waitlist_flag,
    'created_by' => $loggedInUserId,
    'created_on' => now(),
  	'updated_by' => $loggedInUserId,
  	'updated_on' => now(),
    'is_compliance' => $isCompliance,
    'mandatory' => (($manCnt == 'cre_sys_inv_man' || $MadatoryMang == 'Y') && $isCompliance!=1) ? 'Y' : NULL,
	'launchable'=>$launchable
  ));
  $insertaudit = array(
    'user_id' => $userId,
    'order_id' => $vOrderId,
    'class_id' => $classId,
    'course_id' => $courseId,
    'reg_status' => $regStatus,
    'reg_date' => ($regDate) ? $regDate :now(),
  	'reg_status_date' => ($regDate) ? $regDate :now(),
    'comp_status' => $completionStatus,
    'waitlist_priority' => $waitlistNumber,
    'waitlist_flag' => $waitlist_flag,
    'created_by' => $updateUserId,
    'created_on' => now(),
    'mandatory' => $MadatoryMang
  );
  expDebug::dPrintDBAPI(' To string = ' , $insertStmt);
  $newEnrollmentId = $insertStmt->execute();
  
  $result= new stdClass();
  $result->col_name = 'id';
  $result ->col_value = $newEnrollmentId;
  $result ->table_name ='slt_enrollment';
  $statements['insert'][] = $result;
  /* if(!empty($statements)) {
  	$statements[] = $insertStmt;
  } else {
  	$newEnrollmentId = $insertStmt->execute();
  } */
   /* insert into new table to differentiate users registered for multiple contents and their active versions */
  if($deliveryType == 'wbt' || $deliveryType == 'vod'){
  	$enrollContentObj = new stdClass();
    $enrollContentObj->enroll_id = $newEnrollmentId;
    $enrollContentObj->class_id = $classId;
    $enrollContentObj->course_id = $courseId;
    $enrollContentObj->user_id = $userId;
	    insertEnrollmentContentMapping($enrollContentObj,'AutoRegister',$statements);
  }
  // Attendance Detail, Summary and Survey Results entry for the admin clone
  if(!empty($fromCloneEnrId) && !empty($fromCloneClsId) && !empty($newEnrollmentId)){
  	adminCloneAssessmentScoreForClass($fromCloneEnrId,$newEnrollmentId,$courseId,$fromCloneClsId,$classId,$loggedInUserId,$statements);
  }
  expDebug::dPrint(" Final statements passed-->".print_r($statements,true),5);
  	dbStatementsExecution($statements);
			//unset($txn);
		expDebug::dPrint(" Registration end... code committed");
		}catch(Exception $ex){
  	//  $txn->rollback();
	//  unset($txn);
	  watchdog_exception('enrollUserToClass', $ex);
			if (isset($_GET['cron_key'])) {
				throw new Exception($ex);
			}
	  expertusErrorThrow($ex);
  }
 // unset($txn);
  $insertaudit['Enrolled_id'] = $newEnrollmentId;
  expDebug::dprint('modified id'.$loggedInUserId);
  expDebug::dprint('user id'.$userId);
  {
 if (in_array($isAdminSide,array('N','Nlnrsearch'))) {
 	 insertUserPointsForEachActionsPerformed($userId,'register_class',$newEnrollmentId,'insert','class');
 }
 else {
 	 insertUserPointsForEachActionsPerformed($userId,'register_class',$newEnrollmentId,'insert','class');
 }
  make_audit_trail_entries($insertaudit,$loggedInUserId , null ,$classId, 'Inserted','cre_sys_obt_cls','exp_sp_learning','enrollUserToClass',
                                          array('Enrolled_id','user_id', 'order_id', 'class_id', 'course_id', 'reg_status', 'comp_status','waitlist_priority','waitlist_flag','mandatory','created_by'),$userId);
 }
  //Added for API
  $enrollmentStatus["enrollid"]= $newEnrollmentId;
  expDebug::dPrint(' new enrollment id = ' . print_r($newEnrollmentId, true) , 4);
  $notifyString = null;
  if (is_null($waitlistNumber)) {
		$enrollmentStatus['id'] = 0;
		$enrollmentStatus['value'] = 'Registration is successful.';
		
		if(($isCompliance || $manCnt=='cre_sys_inv_man' || $MadatoryMang=='Y') && ($deliveryType == 'ilt')) {
			$notifyString = 'RegisterMandatoryCompliance';
		} else if(($isCompliance || $manCnt=='cre_sys_inv_man' || $MadatoryMang=='Y') && ($deliveryType == 'vc')) {
			$notifyString = 'RegisterVCCompliance';
		} else if(($isCompliance || $manCnt=='cre_sys_inv_man' || $MadatoryMang=='Y') && ($deliveryType == 'wbt')) {
			$notifyString = 'RegisterWBTCompliance';
		} else if(($isCompliance || $manCnt=='cre_sys_inv_man' || $MadatoryMang=='Y') && ($deliveryType == 'vod')) {
			$notifyString = 'RegisterWBTCompliance';
		}else{
			$notifyString = 'Register';
		}
		
		if($isAdminSide=="Y"){
			$notifyString = $notifyString .'ByAdmin';
		}
		

    if ($maxCapacityReached) {
        insertNotification('ClassMaxCapacityReached', $courseId, $classId, $userId, $updateUserId);
    }
    //#76723
    //return $enrollmentStatus;
  }
  else {
		$enrollmentStatus['id'] = 1;
		$enrollmentStatus['value'] = 'Waitlist::' . $waitlistNumber;
		
		if($isAdminSide=="Y") { 
		   $notifyString = 'WaitlistByAdmin';
		   }
		else {
		$notifyString = 'Waitlist';
  }
		  expDebug::dPrint(' waitList notifyString = ' .$notifyString, 5);
  }
  // Validate notification need to send or not
  if($vClassNotify) {
    if ($cartFlag != 'Y' || !is_null($waitlistNumber)) {
  		$iCal = null;
  		if (($notifyString == 'Register' || $notifyString == 'RegisterByAdmin' || $notifyString == 'RegisterVCCompliance' ||  $notifyString == 'RegisterVCComplianceByAdmin' || $notifyString == 'RegisterMandatoryCompliance' || $notifyString == 'RegisterMandatoryComplianceByAdmin') &&
    	         ($deliveryType == 'ilt' || $deliveryType == 'vc')) {
        $iCal = getPublishICal($newEnrollmentId, 'Class');
    	}
    	expDebug::dPrint(' Notify string check test = ' . print_r($seatsAvailableForWaitlistUsers, true) , 4 );

    	insertNotification($notifyString, $courseId, $classId, $userId, $updateUserId, $iCal);
  		if ($notifyString == 'Waitlist' || $notifyString == 'WaitlistByAdmin') {
  			insertNotification('WaitlistLocationAdmin', $courseId, $classId, $userId, $updateUserId);
  		}
    }
  }
  $seatsAvailableForWaitlistUsers = getWaitListAvailable($courseId, $classId);
  expDebug::dPrint(' $seatsAvailableForWaitlistUsers = ' . print_r($seatsAvailableForWaitlistUsers, true) , 4 );
  if ($regStatus == 'lrn_crs_reg_cnf') {
    if ($seatsAvailableForWaitlistUsers == 1) { // Not available
      // Notify all waitlisted users that all the seats that were available are now exhausted.
      $waitListUsers = getWaitlistUsersOfClass($courseId, $classId);
      expDebug::dPrint(' $waitListUsers = ' . print_r($waitListUsers, true), 4);
      foreach ($waitListUsers as $waitListUserId) {
        // Validate notification need to send or not
        if($vClassNotify) {
          insertNotification('WaitlistAllocated', $courseId, $classId, $waitListUserId, $updateUserId);
        }
      }
	}
  }
  syncSolrData('Enrollment');
  syncSolrData('User');
  return $enrollmentStatus;
  }catch (Exception $ex) {
    //  $txn->rollback();
	 // unset($txn);
	  watchdog_exception('enrollUserToClass', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
 // unset($txn);
}

/*
 * insertOrderItem() - Inserts order item record in slt_order_items table. Replaces stored procedure slp_order_items_ins()
 */
function insertOrderItem($orderId, $courseId, $classId, $userId, $programId, $programType,
                            $createdBy,
                               $custom0, $custom1, $custom2, $custom3, $custom4,$cartFrom='') {

  expDebug::dPrint(' insertOrderItem() called with ' .
  '$orderId = ' . $orderId .
  ', $courseId = ' . $courseId .
  ', $classId = ' . $classId .
  ', $userId = ' . $userId .
  ', $programId = ' . $programId .
  ', $programType = ' . $programType .
  ', $createdBy = ' . $createdBy .
  ', $custom0 = ' . $custom0 .
  ', $custom1 = ' . $custom1 .
  ', $custom2 = ' . $custom2 .
  ', $custom3 = ' . $custom3 .
  ', $custom4 = ' . $custom4 , 4
  );

	//$txn = db_transaction();
  try{

  $locationId = 0;
  $locationName = null;
  $organizationId = 0;
  $organizationName = null;
  $countryCode = '';
  $stateCode = '';
  $city = '';
  $zip = '';
  $taxExemption = '';
  $deliveryType = '';
  $taxableDeliveryType = '';
  $nexusValue = '';

  if ($programType == 'Course') {
    $deliveryType = getClassDeliveryType($classId, 'Short');
    expDebug::dPrint('$deliveryType = ' . print_r($deliveryType, true) , 4 );
    if ($deliveryType == 'ilt') {
      $taxableDeliveryType = 'ILT';
	  $locationId = getLocationIdOfClass($courseId, $classId);
	  expDebug::dPrint(' $locationId = ' . print_r($locationId, true), 4);
	  if (!empty($locationId) && $locationId != 'null') {
	    $locationDetails = getLocationDetails($locationId, array('name', 'country', 'state', 'city', 'zipcode'));
	    expDebug::dPrint(' $locationDetails = ' . print_r($locationDetails, true) , 4);
	    if (!empty($locationDetails)) {
	      $locationName = $locationDetails['name'];
	      $countryCode = convertNullValToNullStr($locationDetails['country']);
	      $stateCode = convertNullValToNullStr($locationDetails['state']);
	      $city = convertNullValToNullStr($locationDetails['city']);
	      $zip = convertNullValToNullStr($locationDetails['zipcode']);
	    }
	  }
    } else {
	  $taxableDeliveryType = 'WBT';
	  $personDetails = getPersonDetails($userId, array('org_id'));
	  expDebug::dPrint(' $personDetails = ' . print_r($personDetails, true) ,4);
	  if (!empty($personDetails)) {
	    $organizationId = $personDetails['org_id'];
	  }
	  if (!empty($organizationId) && $organizationId != 'null') {
	    $organizationDetails = getOrganizationDetails($organizationId, array('name', 'country', 'state', 'city', 'zip'));
	    expDebug::dPrint(' $organizationDetails = ' . print_r($organizationDetails, true) , 4);
	    if (!empty($organizationDetails)) {
	      $organizationName = $organizationDetails['name'];
	      $countryCode = convertNullValToNullStr($organizationDetails['country']);
	      $stateCode = convertNullValToNullStr($organizationDetails['state']);
	      $city = convertNullValToNullStr($organizationDetails['city']);
	      $zip = convertNullValToNullStr($organizationDetails['zip']);
	    }

	    $profileMapColumnName = getColumnNameForProfileField('Custom Profile','Tax Exempted?');
	    expDebug::dPrint(' $profileMapColumnName = ' . print_r($profileMapColumnName, true) , 4);
		if (!is_null($profileMapColumnName) && function_exists('hasTaxExemption')) {
		    $taxExemption = hasTaxExemption($organizationId, $profileMapColumnName);
		    expDebug::dPrint(' $taxExemption = ' . print_r($taxExemption, true) , 4);
        }
      }
    }
    if (function_exists('getTaxableStates')) { // to be called only when the commerce module is enabled.
      $nexusValue = getTaxableStates($countryCode, $taxableDeliveryType, 'State');
      expDebug::dPrint(' $nexusValue = ' . print_r($nexusValue, true) , 4);
    }
  }
  $user_pre_curr = (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))?$_SESSION['admin_shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];
  //$price_amount =  function_exists('getPriceOfClass') ? getPriceOfClass($courseId, $classId,$programId) : '';
  $currency_converted_amount =  function_exists('getPriceUserCurrencyOfClass') ? getPriceUserCurrencyOfClass($courseId, $classId,$programId) : '';
  $currency_converted_amount =empty($currency_converted_amount) ? NULL : $currency_converted_amount;
  //$item_price = empty($price_amount) ? NULL : $price_amount;
  $convertion_rate =  function_exists('getCurrencyconvertionRateOfClass') ? getCurrencyconvertionRateOfClass($courseId, $classId,$programId,$user_pre_curr) : 1;
  // Save the order item in database
  $insertStmt = db_insert('slt_order_items');
  $insertStmt->fields(array(
    'order_id' => $orderId,
    'user_id' => $userId,
    'course_id' => $courseId,
    'class_id' => $classId,
    'program_id' => ($programId == "NULL" || $programId == "null" )? null : $programId,
    'program_type' => $programType,
    'org_id' => $organizationId,
    'org_name' => $organizationName,
    'tax_exempted' => $taxExemption,
    'location_id' => $locationId,
    'location_name' => $locationName,
    'country_code' => $countryCode,
    'state_code' => $stateCode,
    'city' => $city,
    'zipcode' => $zip,
    'nexus_val' => $nexusValue,
    'vat_no' => (function_exists('getCountryVATNumber')? getCountryVATNumber($countryCode) : null),
    'product_code' => (function_exists('getProductCodeId')? getProductCodeId($countryCode, $stateCode) : null),
    'item_price' => $currency_converted_amount,
  	'currency_type' =>$user_pre_curr,
  	'convertion_rate' =>$convertion_rate,
    'created_by' => $createdBy,
    'created_on' => now(),
    'updated_by' => $createdBy,
    'updated_on' => now(),
    'custom0' => $custom0,
    'custom1' => $custom1,
    'custom2' => $custom2,
    'custom3' => $custom3,
    'custom4' => $custom4

  ));
  expDebug::dPrintDBAPI(' $insertStmt = ' , $insertStmt);
  $newOrderItemId = $insertStmt->execute();
  if($cartFrom == 'adminorder'){
  	foreach ($_SESSION['cartform']['clone_uc_cart_review_table']['#items'] as $item) {
  		$cClassId 	= $item->classId;
  		$cCourseId 	= $item->courseId;
  		$ucOrderId 	= $item->order_id;
  		$nodeid  		= $item->nid;
  		$Object_Type = $item->delivery_type_code;
  		$order_product_id = $item->order_product_id;
  		updateSltOrederItems($cCourseId,$cClassId,$Object_Type,$orderId,$userId,$order_product_id,$nodeid,$ucOrderId,$cartFrom);
  	}
  }
  expDebug::dPrint(' new order item id = ' . print_r($newOrderItemId, true) , 3);
  }catch (Exception $ex) {
		//$txn->rollback();
		//unset($txn);
	  watchdog_exception('insertOrderItem', $ex);
	  expertusErrorThrow($ex);
  }
  unset($txn);
}

/* *
 * Create new order entry in slt_order table.
 */
function CreateNewOrder($pmLMSUSerID, $pmStatus, $pmCredtedby,&$statements=array()){
	// $txn = db_transaction();
  try{
    // Select the table in which the order record is to be inserted.
    $insertSQL = db_insert('slt_order');
    $insertSQL->fields( array(
  					                 'user_id'      => $pmLMSUSerID,
  					                 'order_status' => $pmStatus,
  					                 'created_by'   => $pmCredtedby,
	                           'created_on'   => now(),
	                           'updated_by'   => $pmCredtedby,
	                           'updated_on'   => now(),
    )
    );
    expDebug::dPrintDBAPI(' $insertSQL = ' , $insertSQL);
    //Skipped this statement execution for dependent functions
    //$orderId = $insertSQL->execute();
   /*  if(!empty($statements)) {
    	$statements[] = $insertSQL;
    } else { */
    	$orderId = $insertSQL->execute();
    	
    	$result= new stdClass();
    	$result->col_name = 'id';
    	$result ->col_value = $orderId;
    	$result ->table_name ='slt_order';
    	$statements['insert'][] = $result;
    	
   // }
    expDebug::dPrint(' $orderId = ' . print_r($orderId, true) , 3);
    updateGroupMappingDetails($orderId, 'cme_pmt', $pmCredtedby,'emptyadd',0,'',$statements);
    $vRegStatus = '';
    return  array('OrderId' => $orderId);
  }catch (Exception $ex) {
		// $txn->rollback();
		// unset($txn);
	  watchdog_exception('CreateNewOrder', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
	// unset($txn);
}
/*
 * getTypeImageClass() - Returns the image based on the Object type.
 */
function getTypeImageClass($typecode,$recertify_path,$overall_status,$recert){
  try{
  if($typecode == 'lrn_cls_dty_ilt' )
  	return "ilt-icon";
  else if($typecode == 'lrn_cls_dty_wbt' )
  	return "wbt-icon";
  else if($typecode == 'lrn_cls_dty_vcl' )
  	return "vcl-icon";
  else if($typecode == 'lrn_cls_dty_vod' )
  	return "vod-icon";
  else if(($typecode == 'cre_sys_obt_crt' || $typecode == 'Certification') && $recertify_path > 1 && $recert=="")
  	return "recert-icon";
 else if(($typecode == 'cre_sys_obt_crt' || $typecode == 'Certification') && $recertify_path > 1 && $recert=="R")
 	return "recert-small-icon";
 else if(($typecode == 'cre_sys_obt_crt' || $typecode == 'Certification') && $recertify_path == 1 && $recert=="R")
 	return "cert-small-icon"; 
  else if($typecode == 'cre_sys_obt_crt' || $typecode == 'Certification')
  	return "cert-icon";
  else if($typecode == 'cre_sys_obt_cur' || $typecode == 'Curricula')
  	return "curr-icon";
  else if($typecode == 'cre_sys_obt_trn' || $typecode == 'Learning Plan')
  	return "lrn-pln-icon";
  else if($typecode == 'cre_sys_obt_crs' || strtolower($typecode) == 'course')
  	return "crs-icon";
  else
    return null;
  }catch (Exception $ex) {
    watchdog_exception('getTypeImageClass', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * getTypeTitle() - Returns the image based on the Object type.
 * by Rajkumar U, added on Nov 25 2011
 */
function getTypeTitle($results){
  try{
  if($results->object_type == 'Class' )
  	return $results->delivery_type_name;
  else if(($results->object_type == 'cre_sys_obt_crt' ) || ($results->delivery_type_code == 'cre_sys_obt_crt'))
  	return "Certification";
  else if(($results->object_type == 'cre_sys_obt_cur' ) || ($results->delivery_type_code == 'cre_sys_obt_cur'))
  	return "Curricula";
  else if(($results->object_type == 'cre_sys_obt_trn' ) || ($results->delivery_type_code == 'cre_sys_obt_trn'))
  	return "Learning Plan";
  else if(($results->object_type == 'cre_sys_obt_crs' ) || ($results->delivery_type_code == 'cre_sys_obt_crs'))
  	return "Course";
  else
    return null;
  }catch (Exception $ex) {
    watchdog_exception('getTypeTitle', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 *  Check Pre-requisite for class
 */
function checkCatalogPrerequisite($catalogId,$catalogType,$rosterUserId ='',$autoUserId='') {
   try{
   expDebug::dPrint('checkCatalogPrerequisite call',3);
   if(empty($rosterUserId)){
   $userId  = getSltpersonUserId();
   if(empty($autoUserId)){
   	$userId = !empty($userId) ? $userId : 0;
   }else{
   	$userId = $autoUserId;
   }
   }else{
     $userId = $rosterUserId;
   }
  $selectPreReq = db_select('slt_common_mapping','comMap');
  $selectPreReq->leftJoin('slt_common_mapping','map','map.id1 = comMap.id2 AND map.type = \'6\'  AND map.object_type = \'cre_sys_obt_crs\'');
  $selectPreReq->leftJoin('slt_master_enrollment','msenr','msenr.program_id = comMap.id2 AND msenr.user_id = '.$userId . ' AND msenr.overall_status  IN(\'lrn_tpm_ovr_cmp\')');
  $selectPreReq->leftJoin('slt_enrollment','enr','enr.user_id = '.$userId.'  AND IF(comMap.prereq_type=\'cre_sys_pre_trp\',(msenr.id = enr.master_enrollment_id),(enr.course_id = comMap.id2 OR enr.course_id=map.id2)) AND enr.comp_status in(\'lrn_crs_cmp_cmp\')');
  $selectPreReq->addField('comMap','id1'); 
  $selectPreReq->condition('comMap.id1',$catalogId); //comMap.id1 = '.$courseId.' and 
  $selectPreReq->condition('comMap.type',5); 
  $selectPreReq->condition('comMap.object_type',$catalogType); //comMap.id1 = '.$courseId.' and
  $selectPreReq->where('(IF(comMap.prereq_type=\'cre_sys_pre_trp\',IFNULL(msenr.overall_status,\'\')!=\'lrn_tpm_ovr_cmp\',IFNULL(enr.comp_status,\'\')!=\'lrn_crs_cmp_cmp\'))');
  $selectPreReq->groupBy('comMap.id1');
   if(!isset($_REQUEST['apiname'])){
  	$selectInd = $selectPreReq->extend("ExpertusSelectQueryExtender");
	$selectInd->setIndex(array('comMap'=>'sli_commap_type')); 
	expDebug::dPrintDBAPI('PREREQ INFO ' , $selectInd);
  $preReqQry = $selectInd->execute();
  $preReqRs = $preReqQry->fetchAll();
  }else{
  	expDebug::dPrintDBAPI('PREREQ INFO ' , $selectPreReq);
  	$preReqRs = $selectPreReq->execute()->fetchAll();
  }
  return $preReqRs;
  }catch (Exception $ex) {
    watchdog_exception('checkCatalogPrerequisite', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}
/*
 * Session details
 *
 * derived timezone attr for session from profile table
 */
function getSessionDetails($courseid,$classid,$delType,$timeFor="",$userId="",$callFrom="",$enrids=array()) {
  try{
  	expDebug::dPrint(" call from -->".print_r($callFrom,true),4);
  if(!empty($enrids)){
  	$selectSess = db_select('slt_enrollment', 'enr');
  	$selectSess->innerJoin('slt_course_class_session', 'sess','enr.class_id = sess.class_id and enr.course_id = sess.course_id');
  	$selectSess->addField('enr','id', 'enrid');
  }else{
  	$selectSess = db_select('slt_course_class_session', 'sess');
  }
  $selectSess->leftjoin('slt_session_instructor_details', 'ses_ins', 'ses_ins.session_id=sess.id');
  $selectSess->leftjoin('slt_person', 'per', 'per.id=ses_ins.instructor_id');
  $selectSess->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=sess.timezone');
  $selectSess->leftjoin('slt_location', 'splt_loc', 'splt_loc.id = sess.location_id');
  $selectSess->leftjoin('slt_profile_list_items', 'loc_timezone', 'loc_timezone.code=splt_loc.timezone');

  if($callFrom == 'class_details'){
  	$selectSess->leftJoin('users', 'u', 'u.name = per.user_name');
  	$selectSess->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
  	$selectSess->leftjoin('slt_profile_list_items', 'splt_job', 'splt_job.code=per.job_title');
  	$selectSess->addExpression('GROUP_CONCAT(if(pic.uri is null or pic.uri ="","",pic.uri) SEPARATOR "|~|")', 'uri');
  	$selectSess->addExpression('GROUP_CONCAT(if(splt_job.name is null or splt_job.name ="","",splt_job.name) SEPARATOR "|~|")', 'job_title');
  }
  $selectSess->addField('sess','id', 'session_id');
  $selectSess->addField('sess','start_date', 'session_start');
  $selectSess->addField('sess','end_date', 'session_end');
  $selectSess->addField('sess','start_time', 'session_start_time');
  $selectSess->addField('sess','end_time', 'session_end_time');
  $selectSess->addField('sess','title', 'session_title');
  $selectSess->addField('sess','course_id', 'course_id');
  $selectSess->addField('sess','class_id', 'class_id');
  $selectSess->addField('sess','type', 'type');
  //$selectSess->addField('sess','instructor_id', 'session_instructor_id');
  $selectSess->addExpression('GROUP_CONCAT(DISTINCT(ses_ins.instructor_id))', 'session_instructor_id');
  $selectSess->addExpression('GROUP_CONCAT(per.full_name SEPARATOR ", ")', 'session_instructor_name');
  $selectSess->addField('splt_timezone','attr2', 'session_timezone');
  $selectSess->addField('splt_timezone','name', 'session_fulltimezone');
  $selectSess->addField('splt_timezone','attr1', 'attr1');
  $selectSess->addField('splt_timezone','attr4', 'attr4');
  $selectSess->addField('loc_timezone','attr2', 'location_timezone');
  
  $selectSess->addField('loc_timezone','name', 'location_fulltimezone');
  $selectSess->addField('splt_loc','name', 'name');
  $selectSess->addField('splt_loc','addr1', 'contact_addr1');
  $selectSess->addField('splt_loc','addr2', 'contact_addr2');
  $selectSess->addExpression('(select country_name from slt_country where country_code = splt_loc.country)', 'contact_country');
  $selectSess->addExpression('(select state_name from slt_state where state_code = splt_loc.state and country_code =  splt_loc.country)', 'contact_state');
  $selectSess->addField('splt_loc','city', 'contact_city');
  $selectSess->addField('splt_loc','zipcode', 'contact_zipcode');
  $selectSess->addField('splt_loc','latitude', 'contact_latitude');
  $selectSess->addField('splt_loc','longitude', 'contact_longitude');
  
  if(!empty($enrids)){
  	$selectSess->condition('enr.id', $enrids,'IN');
  	$selectSess->groupBy('enr.id');
  }else{
  	$selectSess->condition('sess.class_id', $classid,'=');
  	$selectSess->condition('sess.course_id', $courseid,'=');
  }
  $selectSess->groupBy('sess.id');
  if($callFrom=='instructorDesk'){
  	$timeZone = date_default_timezone(false);
  	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
  	$selectSess->orderBy("CONVERT_TZ(CONCAT(DATE(sess.start_date),' ',CONCAT(sess.start_time,':00')),(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone), "."'".$defaultTimezone."')", 'asc');
  }else{
  	if(!empty($enrids)){
  		$selectSess->orderBy('enrid', 'asc');
  	}
	  $selectSess->orderBy('sess.start_date', 'asc');
	  $selectSess->orderBy('sess.start_time', 'asc');
  }
  expDebug::dPrintDBAPI("ILT/VC session detail : ",$selectSess);
  $sessQryRes = $selectSess->execute()->fetchAll();
  
  $sessionDetails = array();
  if(empty($userId)) {
  	$userId = getSltpersonUserId();
  }
  if(!empty($enrids)){
  	$eid = '';
  	$sdr = array();
  	foreach($sessQryRes as $r){
  		if($eid != $r->enrid){
  			$eid = $r->enrid;
  		}
  		$tmp = sessionMap(array($r),$userId,$delType,$timeFor,$callFrom);
  		$sdr[$eid][] = $tmp[0];
  	}
  	return $sdr;
  }
  $sdr = sessionMap($sessQryRes,$userId,$delType,$timeFor,$callFrom);
  return $sdr;
  }catch (Exception $ex) {
    watchdog_exception('getSessionDetails', $ex);
    expertusErrorThrow($ex);
  }
}
function sessionMap($sessQryRes,$userId,$delType,$timeFor,$callFrom=''){
	$i=0;
	foreach ($sessQryRes as $record) {
		
       $ses_start_date_form     = explode(" ",$record->session_start);
   	   $ses_start_date_format   = $ses_start_date_form[0]." ".$record->session_start_time;
   	   $ses_end_date_form       = explode(" ",$record->session_start);
   	   $ses_end_date_format     = $ses_end_date_form[0]." ".$record->session_end_time;

   	   $ilt_ses_start_date_format = $ses_start_date_format;
   	   $ilt_ses_end_date_format		= $ses_end_date_format;
   	   $currentDate;
   	   $timezone_details = getPersonDetails($userId);
	   /* conversion of timezone for vc and ilt classes */
   	   $vTimeZone	= $record->session_timezone; //($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone;
       if($delType == 'lrn_cls_dty_vcl' || ($delType == 'lrn_cls_dty_ilt' && $vTimeZone)){
            $vStartDateTime = $ses_start_date_format; //date_format(date_create($ses_start_date_format),'Y-m-d H:i');
        		$vEndDateTime   = $ses_end_date_format; //date_format(date_create($ses_end_date_format),'Y-m-d H:i');
            if($userId){
            		$UserPHPTimezoneName		= ($delType == 'lrn_cls_dty_vcl' || $delType == 'lrn_cls_dty_ilt' )?getUserPHPTimezoneName($userId):date_default_timezone(FALSE);//date_default_timezone(FALSE);
            		$vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
            		if(!$UserPHPTimezoneName[0]->zone_name){
            		  $vUserPHPTimezonename = date_default_timezone(FALSE);
            		}
    		    }else{
    		        $vUserPHPTimezonename = date_default_timezone(FALSE);
    		    }
        		$oStartDateTime  = timeZoneConvert($vStartDateTime , $vTimeZone,  $vUserPHPTimezonename);
        		$ses_start_date_format 	= date_format($oStartDateTime,'Y-m-d H:i');
        		$oEndDateTime    = timeZoneConvert($vEndDateTime, $vTimeZone,  $vUserPHPTimezonename);
        	  $ses_end_date_format 	= date_format($oEndDateTime,'Y-m-d H:i');
        	  $start_time = date_format($oStartDateTime,'H:i');
   	        $end_time = date_format($oEndDateTime,'H:i');
   	        $session_start = date_format(date_create($ses_start_date_format),'Y-m-d')." 00:00:00";
   	        $session_day = ($record->session_start != null ) ? date_format(date_create($ses_start_date_format),'D') : '';
   	        $sessionTZ = new DateTimeZone($vUserPHPTimezonename);
   	        $currentDate = new DateTime(null,$sessionTZ) ;
   	        $session_type			= $record->type;
       }else{
                $start_time = $record->session_start_time;
           	    $end_time = $record->session_end_time;
           	    $session_start = $record->session_start;
           	    $session_day = ($record->session_start != null ) ? date_format(date_create($record->session_start),'D') : '';
           	    $vUserPHPTimezonename = date_default_timezone(FALSE);
           	    $sessionTZ = new DateTimeZone($vUserPHPTimezonename);
           	    $currentDate = new DateTime(null,$sessionTZ) ;
       }
     $sessionDetails[$i] = array(
     	  	'session_id'                 => $record->session_id,
          'session_instructor_id'      => $record->session_instructor_id,
     		'session_instructor_name'      => $record->session_instructor_name,
     		  'courseid'                   => $courseid,
     		  'classid'                    => $classid,
     		  'facility_id'				 				 => $record->facility_id,
          'session_start'              => $session_start,
          'session_start_day'			 		 => $session_day,
          'session_end'                => $record->session_end,
     	    'session_start_time'         => $start_time,
     	    'session_end_time'           => $end_time,
          'session_title'				 			 => $record->session_title,
          'session_start_format'       => ($timeFor == "enroll") ? date_format(date_create($ses_start_date_format),'M d, Y h:i') : date_format(date_create($ses_start_date_format),'M d, Y'),
     	    'session_end_format'         => date_format(date_create($ses_end_date_format),'M d, Y h:i'),
     		  'session_start_date_format'  => date_format(date_create($ses_start_date_format),'M d, Y'),
     	    'session_end_date_format'    => date_format(date_create($ses_end_date_format),'M d, Y'),
     	    'session_start_time_format'  => date_format(date_create($ses_start_date_format),'h:i'),
     	    'session_start_end_format'   => date_format(date_create($ses_end_date_format),'h:i'),
     	    'session_start_time_form'    => date_format(date_create($ses_start_date_format),'A'),
     	    'session_end_time_form'      => date_format(date_create($ses_end_date_format),'A'),
     			'session_start_time_full'		 => date_format(date_create($ses_start_date_format),'M d, Y H:i'),
     			'session_end_time_full'  		 => date_format(date_create($ses_end_date_format),'M d, Y H:i'),
     		'user_tzcode'          => $timezone_details['attr4'],
     		'user_timezone'                          => $vUserPHPTimezonename,
     		'user_tz'            => $timezone_details['attr1'],
     		 
     			// To display the ILT session in respective locations timezone
     			'ilt_session_start_time'         => $record->session_start_time,
     	    'ilt_session_end_time'           => $record->session_end_time,
          'ilt_session_start_date_format'  => date_format(date_create($ilt_ses_start_date_format),'M d, Y'),
     	    'ilt_session_end_date_format'    => date_format(date_create($ilt_ses_end_date_format),'M d, Y'),
     	    'ilt_session_start_time_format'  => date_format(date_create($ilt_ses_start_date_format),'h:i'),
     	    'ilt_session_start_end_format'   => date_format(date_create($ilt_ses_end_date_format),'h:i'),
     	    'ilt_session_start_time_form'    => date_format(date_create($ilt_ses_start_date_format),'A'),
     	    'ilt_session_end_time_form'      => date_format(date_create($ilt_ses_end_date_format),'A'),
     		  'ilt_session_start_format'       => ($timeFor == "enroll") ? date_format(date_create($ilt_ses_start_date_format),'M d, Y h:i') : date_format(date_create($ilt_ses_start_date_format),'M d, Y'),
     		  'ilt_session_start_day'		 => ($record->session_start != null ) ? date_format(date_create($ilt_ses_start_date_format),'D') : '',
     		  'ilt_session_end_format'         => date_format(date_create($ilt_ses_end_date_format),'M d, Y h:i'),
     		  'ilt_session_start_time_full'		 => date_format(date_create($ilt_ses_start_date_format),'M d, Y H:i'),
     		  'ilt_session_end_time_full'  		 => date_format(date_create($ilt_ses_end_date_format),'M d, Y H:i'),
     		  'server_date_time'							 => $currentDate->format('M d, Y H:i'),
     		'session_code'			 				 => $record->attr1,
     		'tz_code'			 				 => $record->attr4,
          'session_name'			 				 => $record->name,
     		  'session_address1'			 		 => $record->contact_addr1,
    		  'session_address2'			 		 => $record->contact_addr2,
     		  'session_country'			 			 => $record->contact_country,
     		  'session_state'			 				 => $record->contact_state,
     		  'session_city'			 				 => $record->contact_city,
     		  'session_zipcode'			 			 => $record->contact_zipcode,
          'sess_timezone'							 => ($delType == 'lrn_cls_dty_vcl') ? $record->session_timezone : $record->location_timezone,
     		  'sess_fulltimezone'					 => ($delType == 'lrn_cls_dty_vcl') ? $record->session_fulltimezone : $record->location_fulltimezone,
          'session_latitude'			 		 => $record->contact_latitude,
          'session_longitude'			 		 => $record->contact_longitude,
     		'type'								=> $record->type
     	   );
     if($callFrom == 'class_details'){
     	$sessionDetails[$i]['session_instructor_uri'] =$record->uri;
     	$sessionDetails[$i]['job_title'] =$record->job_title;
   }
     $i++;
   }
   expDebug::dPrint(" groupIds123".print_r($sessionDetails,true),5);
   return $sessionDetails;
}
function getEnrollmentQuery($userid, $dueDate = 0){
   try{
   	
   $searchtxt = 	$_REQUEST['searchText'] ;
   	$groupIdArray = getGroupsDetails($userid,'','','',0);
   	$roleIdArr = getUserAttributeValues($userid);
   	$job_role_str = implode("','", $roleIdArr);
   	$tmp = array();
		foreach ($groupIdArray as $b) $tmp[] = $b->id; //pull out data
 		$groupIds = implode("','", $tmp); //and implode
    expDebug::dPrint(" groupIds".print_r($groupIds,true),5);

    if($_REQUEST['apiname'] == 'ListEnrollmentsAPI')
    	$UserDefaultCurrencyArr = getUserDefaultCurrency(1);
    else
    	$UserDefaultCurrencyArr = getUserDefaultCurrency();
    $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
    $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
    $UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];
  	$usertimezone = getUserPHPTimezoneName();
	$usertimezone = $usertimezone[0]->zone_name;
	$timeZone = date_default_timezone(false);
	$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	$usertimezone = empty($usertimezone)? $defaultTimezone : $usertimezone;
   	//38095: System allowing to cancel even mandatory class also (removed role base condtion for mro field)
   	
	
	if ($dueDate == 1) {
		$fieldQuery = "CASE WHEN crs.complete_date IS NOT NULL THEN date_format(CONVERT_TZ(crs.complete_date,spli.attr2,'$usertimezone'),'%Y-%m-%d') WHEN crs.complete_days IS NOT NULL THEN date_format(DATE_ADD(CONVERT_TZ(myenroll.reg_date,spli.attr2,'$usertimezone'), INTERVAL crs.complete_days DAY),'%Y-%m-%d') END AS `dueDate`, DATE_FORMAT(CONVERT_TZ(MIN(concat(date_format(sccs.start_date,'%Y-%m-%d'),' ',sccs.start_time)),spl.attr2,'$usertimezone'),'%Y-%m-%d %H:%i:%s') as sessionDateTime,";
		$joinQuery = "LEFT JOIN slt_course_class_session sccs ON sccs.course_id=myenroll.course_id and sccs.class_id=myenroll.class_id and ((DATE_FORMAT(convert_tz(concat(date_format(sccs.start_date,'%Y-%m-%d'),' ',sccs.start_time),(select sp.attr2 from slt_profile_list_items sp where sccs.timezone=sp.code),'$usertimezone'),'%Y-%m-%d %H:%i:%s')) > CONVERT_TZ(now(),'$defaultTimezone','$usertimezone'))";
	} else {
		$fieldQuery = '';
		$joinQuery = '';
	}
	if(!empty($searchtxt)){
		$fieldQueryforTags = "group_concat(distinct if(td.tagname is not null and td2.tagname is not null,concat(td.tagname,',',td2.tagname),
  							  if(td.tagname is not null,ifnull(td.tagname,''),ifnull(td2.tagname,'')))) as tagname,";
		$joinQueryfroTags = " LEFT JOIN slt_tag_entity ste  on ((ste.entity_id = myenroll.class_id and ste.entity_type='Class'))
							  LEFT JOIN slt_tag_entity ste1  on ((ste1.entity_id = myenroll.course_id  and ste1.entity_type='Course'))
							  LEFT JOIN slt_tagdefn td on (ste.tagid = td.id or ste1.tagid = td.id) and (td.activity_count > 0)
							  LEFT JOIN slt_tagdefn td2 on (ste1.tagid = td2.id) and (td2.activity_count > 0)";
	}else {
		$fieldQueryforTags = '';
		$joinQueryfroTags = '';
	}
	
	
	//Removed the below piece of code related to attempts from select query for #80927
/*(SELECT  ifnull(sum(ssm.no_of_attempts),0) FROM slt_survey_mapping ssm
	INNER JOIN slt_survey ss on ss.id = ssm.survey_id WHERE ss.type='sry_det_typ_ass' and
	ssm.object_id= myenroll.class_id  AND ssm.object_type = 'cre_sys_obt_cls' AND ssm.pre_status = 0) AS post_ass_total_attempts,
  (select ifnull(sum(sccm.max_attempts),0) from slt_course_content_mapper sccm 
  where  sccm.class_id = myenroll.class_id) AS `content_total_attempts`,
  ifnull(sum(`scm`.`total_attempts`),0) as `total_attempts`,*/
	
	$enrollQuery = " Select
  `myenroll`.`id` AS `id`,
  `myenroll`.`launchable` AS `launchable`,
	`myenroll`.`user_id`              AS `user_id`,
	`myenroll`.`master_enrollment_id` AS `master_enrollment_id`,
  `myenroll`.`class_id`             AS `class_id`,
  `myenroll`.`course_id`            AS `course_id`,
( SELECT IF((SELECT COUNT(1) FROM slt_group_mapping WHERE entity_id = myenroll.class_id)>0, myenroll.class_id, myenroll.course_id)) AS check_entity_id,
   ( SELECT IF((SELECT COUNT(1) FROM slt_group_mapping WHERE entity_id = myenroll.class_id)>0, 'cre_sys_obt_cls', 'cre_sys_obt_crs' )) AS check_entity_type,
  `myenroll`.`mandatory`            AS `mandatory`,
  `myenroll`.`is_compliance`            AS `is_compliance`,
  `crs`.`complete_date` 				AS `compliance_complete_date`,
  `crs`.`complete_days` 				AS `compliance_complate_days`,
  `crs`.`validity_date` 				AS `compliance_validity_date`,
  `crs`.`validity_days` 				AS `compliance_validity_days`,
  `crs`.`compliance_completed` 				AS `compliance_completed`,
  `myenroll`.`recertify_path`            AS `recertify_path`,
  (select name from slt_profile_list_items where code = if((myenroll.reg_status='lrn_crs_reg_cnf'),myenroll.comp_status,myenroll.reg_status)) As reg_status,
  IFNULL(`myenroll`.`comp_status`,`myenroll`.`reg_status`) AS `reg_status_code`,
  `myenroll`.`reg_status`           AS `status_code`,
  DATE_FORMAT(`myenroll`.`reg_date`,_utf8'%d-%b-%Y') AS `reg_date`,
 `myenroll`.`comp_status`          AS `comp_status`,
  DATE_FORMAT(`myenroll`.`comp_date`,_utf8'%d-%b-%Y') AS `comp_date`,
  DATE_FORMAT(`myenroll`.`updated_on`,_utf8'%d-%b-%Y') AS `update_date`,
  DATE_FORMAT(`myenroll`.`reg_status_date`,_utf8'%d-%b-%Y') AS `reg_status_date`,
  DATE_FORMAT(`myenroll`.`valid_from`,_utf8'%d-%b-%Y') AS `valid_from`,
  DATE_FORMAT(`myenroll`.`valid_to`,_utf8'%m/%d/%Y') AS `valid_to`,
  `myenroll`.`score`                AS `score`,
  `myenroll`.`grade`                AS `grade`,
  `myenroll`.`progress`                AS `progress`,
  DATE_FORMAT(`myenroll`.`updated_on`,_utf8'%d-%b-%Y') AS `updated_on`,
  `myenroll`.`comp_by` AS `updated_by`,
  `myenroll`.`created_by` AS `created_by`,
  `myenroll`.`waitlist_priority`            AS `waitlist_priority`,
  `myenroll`.`pre_status`            AS `preassesment_completion_status`,
  `crs`.`title`                     AS `title`,
  `crs`.`code`                     AS `crs_code`,
  `cat`.`title`                     AS `cls_title`,
  `cat`.`code`                      AS `CODE`,
  `cat`.`status`                    AS `cls_status`,
  `cat`.`short_description`               AS `description`,
  `cat`.`delivery_type`               AS `delivery_type_code`,
  `cat`.`dedicated_class_flag` AS `dedicated_class_flag`,
  (select name from slt_profile_list_items where code =cat.delivery_type) AS `delivery_type`,
  (select attr1 from slt_profile_list_items where code =cat.delivery_type) AS `BaseType`,
  `cat`.`course_id`                 AS `courseid`,
  slf_convert_price(`cat`.`price`,cat.currency_type,'$UserPrefCurrCode') AS `classprice`,
  '$UserPrefCurrSym' 				AS currency_type,
  '$UserPrefCurrSym' 				AS currency_symbol,
  '$UserPrefCurrCode' 				AS currency,
  '$UserPrefCurrLongCode' 			AS currency_code,
  `cat`.`lang_code`                AS `LangCode`,
  `node`.`node_id` AS `node_id`,
  `loc`.`id`			   AS `LocationId`,
  `loc`.`name`			   AS `LocationName`,
  `loc`.`addr1` AS `LocationAddr1`,
  `loc`.`addr2` AS `LocationAddr2`,
  `loc`.`city` AS `LocationCity`,
  `loc`.`latitude` AS `latitude`,
  `loc`.`longitude` AS `longitude`,
  (SELECT  IF(COUNT(state_name)>0,state_name,loc.state)  stname FROM slt_state WHERE state_code= loc.state limit 1) AS `LocationState`,
  `state`.`state_name` AS `stateName`,
  `loc`.`zipcode` AS `LocationZip`,
  `loc`.`phone` AS `LocationPhone`,
  (SELECT `country_name` FROM `slt_country` WHERE `country_code`=`loc`.`country` LIMIT 1) AS `LocationCountry`,
  (select name from slt_profile_list_items where code= cat.lang_code) AS Language,
 
  `p`.`user_name`                   AS `user_name`,
   DATE_FORMAT(`sor`.`order_date_time`,_utf8'%d-%b-%Y') AS `orderdatetime`,
  `ses`.`id`			   AS `session_id`,
  DATE_FORMAT(`ses`.`start_date`,_utf8'%d-%b-%Y')      AS `session_start`,
  `ses`.`start_time` AS `session_start_time`,
  `ses`.`end_time` AS `session_end_time`,
  `myenroll`.`content_status` AS `content_status`,
 `ses`.`title` AS `session_title`, if((mro.mro = 'cre_sys_inv_opt' OR exmp.exempted_status = 1),3,if(mro.mro = 'cre_sys_inv_man',1,2)) ascen, 
  `myenroll`.`cmpl_expired`  as `cmpl_expired`,
  `p`.`manager_id` as `managerID`,
  `p`.`hire_date` as `hire_date`,
  `spl`.`attr2` as `timezone`,
  `spli`.`attr2` as `ptimezone`,
  `myenroll`.`comp_date` as `completion_date`,
  CONVERT_TZ(myenroll.reg_date,spli.attr2,'$usertimezone') AS `registration_date`,
  ". $fieldQuery ."
  ".$fieldQueryforTags."
 	IF(exmp.exempted_status = 1 , NULL,(IF(usrmro.mro is not null,CONCAT(splt_usrmro.name,'##',splt_usrmro.code),CONCAT(splt_mro.name,'##',splt_mro.code)))) AS mro, ";
	// Modified by Vincent for performance 	
	if(!empty($job_role_str))
  	$enrollQuery .= "(select group_concat(name) from slt_profile_list_items where code IN ('".$job_role_str."')) as assigned_by,";
  else
  	$enrollQuery .= "null as assigned_by,";
  	
  $enrollQuery .= "(SELECT  IF(COUNT(1)=0,'FALSE','TRUE') FROM slt_survey_mapping ssm
	INNER JOIN slt_survey ss on ss.id = ssm.survey_id WHERE ss.type != 'sry_det_typ_ass' and 
	ssm.object_id= myenroll.class_id AND ssm.object_type = 'cre_sys_obt_cls') AS survey_status,
  (SELECT  IF(COUNT(1)=0,'FALSE','TRUE') FROM slt_survey_mapping ssm
	INNER JOIN slt_survey ss on ss.id = ssm.survey_id WHERE ss.type='sry_det_typ_ass' and
	ssm.object_id= myenroll.class_id  AND ssm.object_type = 'cre_sys_obt_cls' AND ssm.pre_status = 0) AS assessment_status,
	(SELECT  IF(COUNT(1)=0,'FALSE','TRUE') FROM slt_survey_mapping ssm
	INNER JOIN slt_survey ss on ss.id = ssm.survey_id WHERE ss.type='sry_det_typ_ass' and
	ssm.object_id= myenroll.class_id  AND ssm.object_type = 'cre_sys_obt_cls' AND ssm.pre_status = 1) AS preassessment_status,
	CONCAT(exmp.exempted_status,'|',exmp.created_by,'|',exmp.created_on) as exempted_sts,
	null AS percentage_complete,
	null AS prg_expires_on,
    null AS prg_expires_in_value,
    null AS prg_expires_in_unit,
    null  AS prg_start_date,
    null  AS prg_end_date,
	null AS enforce_sequence,
	null AS is_current
	
	FROM slt_enrollment myenroll
  LEFT JOIN slt_course_template crs ON crs.id=myenroll.course_id
  LEFT JOIN slt_course_class cat ON cat.id=myenroll.class_id AND cat.course_id=crs.id
  LEFT JOIN slt_course_class_session ses ON ses.course_id=myenroll.course_id and ses.class_id=myenroll.class_id
  ". $joinQuery ."
  ".$joinQueryfroTags."
  LEFT JOIN slt_location loc ON ses.location_id = loc.id
  LEFT JOIN slt_person p ON p.id=myenroll.user_id
  LEFT JOIN slt_order sor ON sor.id = myenroll.order_id
  LEFT JOIN slt_group_mapping mro ON ((mro.parent_id = concat(myenroll.course_id,'-',myenroll.class_id) AND mro.parent_type = 'learning')) AND mro.group_id IN('$groupIds') 
  LEFT JOIN slt_user_access_mapping usrmro ON (usrmro.parent_id = concat(myenroll.course_id,'-',myenroll.class_id) AND usrmro.parent_type = 'learning') AND usrmro.user_id = myenroll.user_id 
  LEFT JOIN slt_enrollment_exempted exmp ON exmp.id = (SELECT exp1.id FROM slt_enrollment_exempted exp1 WHERE exp1.enroll_type = 'class' AND exp1.enrollment_id = myenroll.id ORDER BY id DESC LIMIT 1)
  LEFT JOIN slt_profile_list_items splt_mro ON mro.mro = splt_mro.code 
  LEFT JOIN slt_profile_list_items splt_usrmro ON usrmro.mro = splt_usrmro.code 
  LEFT JOIN `slt_node_learning_activity` `node` ON `cat`.`id`  = `node`.`entity_id` AND `node`.`entity_type`='cre_sys_obt_cls'
  LEFT JOIN slt_attendance_summary scm on myenroll.id = scm.enrollment_id 
  LEFT JOIN slt_profile_list_items spl on ses.timezone = spl.code
  LEFT JOIN slt_profile_list_items spli on spli.code = p.time_zone
 
  LEFT JOIN slt_state state on loc.state=state.state_code and loc.country = state.country_code
  WHERE (`myenroll`.`reg_status` NOT IN(_latin1'lrn_crs_reg_rsc',_latin1'lrn_crs_reg_rjt',_latin1'lrn_crs_reg_rsv'))";
	$enrollQuery .=" AND myenroll.user_id=".$userid;
	if($_REQUEST['compliance_mandatory'] == 1 && $_REQUEST['apiname'] == 'ListEnrollmentsAPI')
	{
		$enrollQuery .=" AND (myenroll.is_compliance = 1 OR myenroll.mandatory = 'Y' )";
	}
	
	if($_REQUEST['apiname'] == 'ListEnrollmentsAPI'  && $_REQUEST['mobile_request'] && $_REQUEST['upcoming_class'] ==1 ){
		 
		$enrollQuery .= " and (ses.start_date between now() and now() + interval 7 DAY )";
	}
	

	if($_REQUEST['apiname'] == 'ListEnrollmentsAPI')
	{
		if(!empty($_REQUEST['date_from'])) {
			$date_from = trim($_REQUEST['date_from']);
			$from_timestamp = strtotime($date_from);
			$from_format = date("Y-m-d H:i:s", $from_timestamp);
		}
		if(!empty($_REQUEST['date_to'])) {
			$date_to = trim($_REQUEST['date_to']);
			$to_timestamp = strtotime($date_to);
			$to_format = date("Y-m-d 23:59:59", $to_timestamp);
		}
		if (($date_from == "") && (!empty($date_to))) {
			$enrollQuery .= " AND myenroll.reg_date<='$to_format' ";
		}
		else if(($date_to==  "") && (!empty($date_from))) {
			$enrollQuery .= " AND myenroll.reg_date>='$from_format' ";
		}
		else if(!empty($date_from) && !empty($date_to)) {
			if($from_timestamp <= $to_timestamp) {
				$enrollQuery .= " AND myenroll.reg_date BETWEEN '$from_format' AND '$to_format' ";
			}
		}
	}
	expDebug::dPrint('get enroll details '.print_r($_GET, 1), 5);
	if(isset($_GET['enr_id'])) {	// Added for #77753 by Ayyappans
		$enrollQuery .= " AND myenroll.id = " . escape_string($_GET['enr_id']);
	}
	if((isset($_REQUEST["mobile_version"]) && $_REQUEST['apiname'] == 'ListEnrollmentsAPI') ){
		if(empty($_REQUEST["enroll_id"])){
			$enrollQuery .=" AND myenroll.master_enrollment_id is null";//Added condition for survey DeepLink
		}
		
		/* added for completed and attempt left filter*/
			$regstatus  	= stripRegStatus($_REQUEST['regstatuschk']);
	 		if(!empty($regstatus)){
				$statusArray = explode('|',$regstatus);
				expDebug::dPrint('get enroll details 111'.count($statusArray), 5);
				
				
				$i = 0;
				if (in_array('lrn_crs_cmp_cmp_atl', $statusArray) && count($statusArray) == 2){
					$enrollQuery .= " AND (myenroll.comp_status IN ('lrn_crs_cmp_cmp') AND (myenroll.launchable = 1))";
					$i++;
 				}
 			}
	
		
		
		
   		//$enrollQuery .=" AND myenroll.master_enrollment_id is null";//code added by siva
   		$enrollQuery .=" GROUP BY myenroll.id "; // commented for Mobile TP
   		   
   	}
   	else
   	{
   		$enrollQuery .=" GROUP BY myenroll.id "; // commented for Mobile TP
   		   
   		$bool = true;
   		if(isset($_REQUEST["sortBy"])) {
   			$sortBy = $_REQUEST["sortBy"];
   			if($sortBy != 'startdate') {
   				$bool = false;
   				$enrollQuery .="  ORDER BY `myenroll`.`reg_date` DESC ";
   			}
   		}
   		
   		$enrollQuery .= ($bool) ? " ORDER BY ascen" : " ,ascen";
   		 
   	}
		
	
	expDebug::dPrintDBAPI('fetch enrollmenrs getEnrollmentQuery', $enrollQuery);
	return $enrollQuery;
  }catch (Exception $ex) {
    watchdog_exception('getEnrollmentQuery', $ex);
    expertusErrorThrow($ex);
  }
}



function commonRegisterHtml($results,$isPrereqCheck=''){
  try{
  	expDebug::dPrint("wqe das".print_r($results,true),5);
  return theme('exp_sp_learning-common-register-html', array('results' => $results,'isPrereqCheck' => $isPrereqCheck));
  }catch (Exception $ex) {
    watchdog_exception('commonRegisterHtml', $ex);
    expertusErrorThrow($ex);
  }
}

function commonLPRegisterHtml($results,$isPrereqCheck) {
  try{
  return theme("exp_sp_learning-common-lp-register-html", array('results' => $results,'isPrereqCheck' => $isPrereqCheck));
  }catch (Exception $ex) {
    watchdog_exception('commonLPRegisterHtml', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * generateCertCurrLPReminderNotifications() - gets the list of Certification/Curriculum/Learning Plan enrollment list to send reminder
 */
/*function generateCertCurrLPReminderNotifications($notificationType, $userId,$prgm_id) {
  try{
  $getInfo = getCertCurrLPDetailsForReminderNotifications($userId,$prgm_id);
  //print_r($getInfo); die;
  $fullname         = '';
  $delivery_type    = '';
  $course_title     = '';
  $session_startdate  = '';
  $session_due_days  = '';
  $prg_lang_code     = '';

  $firstname         = convertNullValToNullStr(ucfirst($getInfo['first_name']));
  $delivery_type    = convertNullValToNullStr($getInfo['delivery_type']);
  $course_title     = convertNullValToNullStr($getInfo['title']);
  $session_startdate = convertNullValToNullStr($getInfo['session_start']);
  $session_due_days  = convertNullValToNullStr($getInfo['expire_days']);
  $prg_lang_code     = convertNullValToNullStr($getInfo['lang_code']);

  $notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|' .
  									   'learning_delivery_type>|' . $delivery_type . '~|' .
									   'course_title>|' . $course_title . '~|' .
									   'session_startdate>|' . $session_startdate . '~|'.
                                       'session_endtime>|' . $session_due_days . '~|';


  $notificationInfo['message_id']    = 'cert_curr_lp_reminder';
  $notificationInfo['message_type']  = 'Cert/Curr/Learning plan reminder';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;
  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertCurrLPReminderNotifications', $ex);
    expertusErrorThrow($ex);
  }
}*/

/*
 * getCertCurrLPDetailsForReminderNotifications() - get all Certification/Curriculum/Learning plan expiry enrollments
 */

/*function getCertCurrLPDetailsForReminderNotifications($user_id,$prgm_id='') {
  try{
  $select = db_select('slt_program','prgm');
  $select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $select->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $select->innerJoin('slt_profile_list_items','prof','prof.code = prgm.object_type');
  $select->innerJoin('slt_enrollment','enr','enr.master_enrollment_id = mse.id');
  $select->innerJoin('slt_course_class_session','ses','ses.course_id = enr.course_id and ses.class_id = enr.class_id');
  $select->leftJoin('slt_notification_info','info','info.notification_code = \'cert_curr_lp_reminder\'');

  $select->addField('prs','id','personId');
  $select->addField('prs','first_name');
  $select->addField('prgm','title');
  $select->addField('prgm','lang_code');
  $select->addField('prof','name','delivery_type');
  $select->addField('ses','start_date','session_start');
  $select->addField('prgm','id','programId');
  $select->addExpression("DATEDIFF(ses.start_date,NOW())","expire_days");
  if($user_id>0) {
    $select->condition('prs.id',$user_id,'=');
  }
  if(!empty($prgm_id)){
  	$select->condition('prgm.id',$prgm_id,'=');
  }else{
	  $select->groupBy('personId');
	  $select->groupBy('first_name');
	  $select->groupBy('title');
	  $select->groupBy('delivery_type');
  }
  $select->condition('prgm.status','lrn_lpn_sts_atv','='); // added this Status Condition for this issue #0046707
  $select->condition('info.status','cre_ntn_sts_atv','=');
  $select->where("DATEDIFF(ses.start_date,NOW())  = info.frequency_data_value");
  $result = $select->execute();
  if($user_id>0) {
    $results = $result->fetchAssoc();
  } else {
    $results = $result->fetchAll();
  }
  expDebug::dPrintDBAPI('$select : ', $select);
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertCurrLPDetailsForReminderNotifications', $ex);
  	if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}*/

/*
 * generateCertExprReminderNotificationsForManager() - gets the list of Certification expiry reminder
 */
/*function generateCertExprReminderNotificationsForManager($notificationType, $managerId, $userId) {
  try{
  $getInfo = getCertExpiryDetailsForManagerNotifications($userId);
  $firstname    	= '';
  $fullname    		= '';
  $managername     	= '';
  $course_title  	= '';
  $session_enddate  = '';
  $certification_code  = '';
  $designation  	= '';
  $username			= '';
  $prg_lang_code    = '';
  $select = db_select('slt_person', 'per');
  $select->addField('per','full_name','manager_name');
  $select->addField('per','id','manager_id');
  $select->condition('per.id',$managerId,'=');
  $managername = $select->execute()->fetchField();
  $desination = $getInfo['designation'];
  if(!empty($desination)){
    $desination = getProfileListItemName($desination);
  }
  $firstname        = convertNullValToNullStr(ucwords($getInfo['employee_first_name']));
  $fullname         = convertNullValToNullStr(ucwords($getInfo['employee_name']));
  $course_title     = convertNullValToNullStr($getInfo['title']);
  $session_enddate  = convertNullValToNullStr($getInfo['expired_on']);
  $certification_code  = convertNullValToNullStr($getInfo['code']);
  $designation		= convertNullValToNullStr($desination);
  $username     	= convertNullValToNullStr($getInfo['employee_username']);
  $prg_lang_code    = convertNullValToNullStr($getInfo['lang_code']);

  $notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|' .
  									   'full_name>|' . $fullname . '~|' .
  									   'user_name>|' . $username . '~|' .
  									   'manager_name>|' . ucwords($managername) . '~|' .
									   'tp_title>|' . $course_title . '~|' .
									   'session_enddate>|' . $session_enddate . '~|'.
  									   'user_jobtitle>|' . $designation . '~|'.
                                       'tp_code>|' . $certification_code . '~|';

  $notificationInfo['message_id']    = 'certification_expire_manager';
  $notificationInfo['message_type']  = 'Certification expiry reminder to manager';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;

  return $notificationInfo;
  }catch (Exception $ex) {
    watchdog_exception('generateCertExprReminderNotificationsForManager', $ex);
    expertusErrorThrow($ex);
  }
}*/

/*
 * getCertExpiryDetailsForManagerNotifications() - get all Certification expiry enrollments
 */
/*function getCertExpiryDetailsForManagerNotifications($user_id) {
  try{
  $select = db_select('slt_program','prgm');
  $select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $select->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $select->addField('prs','id','personId');
  $select->addField('prs','full_name','employee_name');
  $select->addField('prs','first_name','employee_first_name');
  $select->addField('prs','user_name','employee_username');
  $select->addField('prgm','title');
  $select->addField('prgm','lang_code');
  $select->addField('prgm','code');
  $select->addField('prs','job_title','designation');
  $select->addField('prs','manager_id');
  $select->addExpression("DATE_FORMAT(
                            IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
                                  IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
                                        DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
                            '%b %e, %Y')", 'expired_on');

  if($user_id>0) {
    $select->condition('prs.id',$user_id,'=');
  }
  $select->isNotNull('prs.manager_id');
  $select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
  $select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
  $select->isNotNull('mse.comp_date');
  $select->isNotNull('prgm.expires_in_value');
  $select->isNotNull('prgm.expires_in_unit');
  $select->condition('prgm.expires_in_value', 0, '>');
  $select->where("DATEDIFF(
                    IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
                          IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
                                DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
                    NOW()) < 0");
  expDebug::dPrintDBAPI('$select', $select);
  $result = $select->execute();

  if($user_id>0) {
    $results = $result->fetchAssoc();
  } else {
    $results = $result->fetchAll();
  }
  expDebug::dPrint('$results = ' . print_r($results, true));
  return $results;
  }catch (Exception $ex) {
    watchdog_exception('getCertExpiryDetailsForManagerNotifications', $ex);
  	if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}*/
function getPrmCompletedOrNot($programId,$userId){
   try{
	$select = db_select('slt_master_enrollment','master');
	$select->addExpression('COUNT(master.id)', 'crscnt');
	$select->condition('master.overall_status', 'lrn_tpm_ovr_cmp','=');
	$select->condition('master.program_id', $programId,'=');
	$select->condition('master.user_id', $userId,'=');
	$result = $select->execute()->fetchField();

	return $result;
  }catch (Exception $ex) {
    watchdog_exception('getPrmCompletedOrNot', $ex);
    expertusErrorThrow($ex);
  }
}
 //change for the ticket 0020909 = For updating tp in skills//
function updateSkillForTpCompletion($programId,$enrolluserId,$masterEnrollId,$tpTitle,$userId){
	try {
	$insertStmt = db_insert('slt_skill_set');
	$fields = array(
		'skill_name'           => $tpTitle,
	        'type' =>  'skill',
		'display_to_others'    => 'Yes',
		'proficiency'          => 10,
		'user_id'              => $enrolluserId,
		'program_id'           => $programId,
		'master_enrollment_id' => $masterEnrollId,
	  'created_by'           => $userId,
		'created_on'           => now(),
	  'updated_by'           => $userId,
		'updated_on'           => now()
	);
	$insertStmt->fields($fields);
	expDebug::dPrintDBAPI('updateSkillForTpCompletion = ' , $insertStmt);
	$skillId = $insertStmt->execute();
	}catch (Exception $ex) {
    watchdog_exception('updateSkillForTpCompletion', $ex);
    expertusErrorThrow($ex);
  }
}
/**
 * Register user to Learning Plan.
 */
function register_user_to_training_plan($userId, $objectId, $classId, $showTitle=false,$isAdminSide,$showResponse=true,$fromPage,$masterMandatory,$regDate = '',$clcount=null,$recertifyValue = '',$isMandatory=''){
  try {
  	expDebug::dPrint("recert value".$recertifyValue,5);
  /* $recertify = explode("-",$recertifyVal);
  	$recertifyValue = $recertify[1];
	$recertifylevel = $recertify[0]; */
  	
  	if(strpos($recertifyValue, 'R')>0){
  		$recertifyflag = 'R';
  		$recertify = explode('-',$recertifyValue);
  		$recertifyval = $recertify[0]+1;  		
  	}
  	else{
  		$recertifyval = 1;
  	}
  	  	
	if($userId){
  	$vLMSUserId = $userId;
  }else{
  	$vLMSUserId = getSltpersonUserId();
  }
  // Fixed for this issue #0039406
  $userId 			    = $vLMSUserId;

  $createdBy = getSltpersonUserId(); // $createdBy is null string for user auto enrolled for mandatory tp while logging in
  if(!$vLMSUserId) {
    $program_msg = t('Please sign in to proceed');
  }else{

  	$check_user_admin = isset($_REQUEST['apiname']) && isset($_REQUEST['userid']) ? $_REQUEST['userid'] : $createdBy;
  	$apinamenotify = trim($_REQUEST['apiname']);  	 
  	$isadmingrp = ifadminUser($check_user_admin);
  	
  	expDebug::dPrint('isadmingrp'.$isadmingrp,5);
  	
  	$mobilereq = isset($_REQUEST['mobile_request'])? true: false;  
  	 
  	$mancnt = getMandatoryOrNot($objectId,$userId,'TP'); 	 

  	
      $entity_id = array();
  	  $crs_entity_id = array();
  	  $ins_conflict  = array();
  	  expDebug::dPrint("Learning Plan Register - classId ==>".$classId , 4);
  	  $listIds = explode(',',$classId);

  	  $getTPConflictCheck = trainingProgramMultiClassConflictCheck($listIds);
      if($getTPConflictCheck > 0){
           expDebug::dPrint(' return = ' . print_r($getTPConflictCheck, true) , 4);
           $entity_id[] = $getTPConflictCheck;
      }
      if($getTPConflictCheck == 0){
        for($i=0;$i<count($listIds);$i++){
          $conflictInstructor = true;
          $conflictInstructorSession = 0;
          $getConflictCheck  = enrollmentsConflictCheck($vLMSUserId,$listIds[$i],'Class');

          $deliveryType = getClassDeliveryType($listIds[$i], 'Short');
          $Instr_check = is_instructor($vLMSUserId);
          if(($deliveryType == 'ilt' || $deliveryType == 'vc') && $Instr_check == true){
          	// Confirm that the user not assigned this class to instruct for learners
          	$conflictInstructor = getClassInstructorConflict($vLMSUserId, $listIds[$i]);

          	// Confirm that the user not scheduled to some other class in same session time as instructor
          	$conflictInstructorSession = getSessionClassInstructorConflict($vLMSUserId, $listIds[$i]);
          }
      	  if($getConflictCheck > 0) {
            $entity_id[] = $listIds[$i];
      	  }elseif($conflictInstructor == false){
      	     $entity_id[] = $listIds[$i];
      	     $ins_conflict[] = "-10";
      	  }elseif($conflictInstructorSession > 0 ){
      	     $entity_id[] = $listIds[$i];
      	  }/* else{
      	    $courseConflict = courseConflictCheck($vLMSUserId, '', $listIds[$i], 'Class');
        	  if($courseConflict > 0) {
              $crs_entity_id[] = $listIds[$i];
        	  }
      	  } */
        }
      }
    $user_detail=db_query("select full_name from slt_person
						WHERE id = '".$vLMSUserId."'");

  $userDetail = $user_detail->fetchAll();
  $fullName = $userDetail[0]->full_name;
  $countClsConflict = count($entity_id);
  $activeClassCount = getCourseClassCountForLP($objectId,$vLMSUserId,$isAdminSide,$recertifyval);
  expDebug::dPrint('checking the active classes for the usr'.print_r($activeClassCount,true).'user id'.$vLMSUserId,4);

      if(isset($_REQUEST["mobile_request"])){
  	
  	if($fromPage != 'MyTeam') {
  		if ($countClsConflict > 0 ){
  			$result->status = 'InstructorConflict';
  			return $result;
  		}
  	}
  	if ($activeClassCount[0]->cnt == 0){
  		$result->status = 'closed';
  		return $result;
  	}
  }
  
    $selectEnr = db_select('slt_module_crs_mapping','map');
    $selectEnr->Join('slt_enrollment', 'enr', 'enr.course_id = map.course_id');
    $selectEnr->addExpression('COUNT(1)', 'crsenrcnt');
    $selectEnr->condition('enr.user_id', $vLMSUserId,'=');
    $selectEnr->condition('map.program_id', $objectId,'=');
    $selectEnr->condition('enr.reg_status', 'lrn_crs_reg_cnf','=');
    $selectEnr->condition('enr.comp_status', array('lrn_crs_cmp_enr', 'lrn_crs_cmp_inp'),'NOT IN');
    $selectEnr->condition('enr.class_id', array($classId),'IN');
    $result =  $selectEnr->execute()->fetchField();
    expDebug::dPrintDBAPI("query changes ", $selectEnr);
    expDebug::dPrint("testetst ".print_r($result,true),5);

       if(($countClsConflict > 0 || count($crs_entity_id) > 0 || $activeClassCount[0]->cnt <= 0) && $result <= 0) {
         if(count($ins_conflict) >0){
           $errMessage = t("ERR147");
         }else if($activeClassCount[0]->cnt <= 0){
         	//Commented for remove the enforcing child level visibility of classes under Training plan : Refer #0037979
         /* 	$totalcrsmappingcnt =  getCourseMappingCount($objectId);
         	 if((count(explode(',',$maxCount[0]->courseids)) == $totalcrsmappingcnt)){
         	 	$errMessage = t("ERR108");
         	 }else{
         	 	$errMessage = t("One of the class under the TP is not visible to you.");
         	 } */
         	if(count(explode(',',$maxCount[0]->courseids))){
         		$errMessage = t("ERR108");
         	}
         }else{
         		if($countClsConflict > 0){
         				$errMessage = ($isAdminSide == "Y") ? t("ERR191") : t("ERR048");
         			}else{
         				$errMessage = ($isAdminSide == "Y") ? t("ERR190") : t("ERR049");
         			}
         }
        $confilictClassId = ($countClsConflict > 0) ? $entity_id[0] : $crs_entity_id[0];
      	$title = getCourseTitle($confilictClassId);
      	if($showTitle == true) {
      		if($isAdminSide == "Y") {
      			$program_msg = $fullName." ". $errMessage ." - <b>".$title."</b>"."</br>";
      		}else{
        	  $program_msg = $errMessage;//." - <b>".$title."</b>";
      		}
      	}else{
      		if($isAdminSide == "Y") {
      			$program_msg = $fullName." ".$errMessage."</br>";
      		}else{
      		  $program_msg = $errMessage;
      		}
      	}
      }else{
      expDebug::dPrint("recertify value" .$recertifyval, 5);
        // Get certification expired remainder days from exp_sp.ini
        $config           = getConfig("exp_sp");
        $cert_days_expire = isset($config["cert_expire_reminder_days"]) ? $config["cert_expire_reminder_days"] : 10;
        expDebug::dPrint(' slp_program_enrollment_ins $cert_days_expire = '.$cert_days_expire);
        $manCnt = getMandatoryOrNot($objectId,$vLMSUserId,1);
        $mand = ($manCnt == 'cre_sys_inv_man') || ($masterMandatory=='Y')? 1 : 0;
        $user_pre_currency = (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))?$_SESSION['admin_shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];
       /* $dao = new AbstractDAO();
        $qry = "CALL slp_program_enrollment_ins($vLMSUserId,$objectId,'" . $classId . "','No','" . $createdBy . "','" . $regDate . "','". $recertifyValue ."','". $cert_days_expire ."',$mand,'".$user_pre_currency ."',@outval1,@outval2,@outval3,@outval4,@outval5,@outval6)";
        $dao->connect();
        expDebug::dPrint("Learning Plan Register - slp_program_enrollment_ins  ==>".$qry , 4);
        $dao->execute($qry);

		//print "fullName--".$fullName;
        $qry = "SELECT @outval1 AS status, @outval2 AS masterEnroll";
        $res = $dao->query($qry);
        $result=$dao->fetchResult();
        $dao->closeconnect();*/
        // Modified by Vincent on 27 Jan 2017. For #0071682
        $result = lp_program_enrollment_ins($vLMSUserId,$objectId,$classId,'No',$createdBy,$regDate,$recertifyval,$cert_days_expire,
									$mand,$user_pre_currency,$isMandatory);

        $tpEnrollStatus = $result->status;
        $defaultvalue[0] =new stdClass();
        $tpinitialStatus = '';
        $result->username = $fullName;
        if($result->status == 'RG') {
        	$tpinitialStatus = 'lrn_tpm_ovr_enr';
        	if(strpos($recertifyValue, 'R')>0 && ($isAdminSide == "Y")){
        		$moduleDet = getModuleIdFromObjectId($objectId,$recertifyval);
        		$result->status = t('MSG809').' '.$moduleDet[0]->module_title;
        	}else{
        	$result->status = 'Registered';
        	}
        }else if($result->status == 'WL') {
        	$tpinitialStatus = 'lrn_tpm_ovr_wtl';
        	$result->status = 'Waitlisted';
        }else if($result->status == 'EP') {
        	$compStatus = getPrmCompletedOrNot($objectId,$userId);
        	if($compStatus > 0) {
        		$tpinitialStatus = 'lrn_tpm_ovr_cmp';
        		$result->status = t('MSG441');
        	}else{
        		$tpinitialStatus = 'lrn_tpm_ovr_enr';
        		$result->status = t('MSG430');
        	}
        }else if($result->status == 'PS') {
        	$tpinitialStatus = 'lrn_tpm_ovr_ppm';
        	$result->status = t('MSG442').'.'.t('MSG443');
        }
        $defaultvalue[0]->overall_status = $tpinitialStatus;

        if($isAdminSide == "Y") {
        	// $program_msg = $fullName." - ". t($result->status)."<br />"; Commented For class and Tp Hmtl should Be Same In Enrollment Roster
        	$program_msg = "<ul><li><div><span class='enrolluser-name-list'>".$fullName."</span> - <span>".t($result->status)."</span></div></li></ul>";
        }else{
        $program_msg = $result->status;
        }
        $masterEnrollId = $result->masterEnroll;
        $result->userid = $vLMSUserId;

        if($fromPage == 'MyTeam') {
        		  $updateStmt = db_update('slt_master_enrollment');

                  // Set conditions
                  $updateStmt->condition('user_id', $vLMSUserId, '=');
                  $updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_cmp'),'IN');
                  $updateStmt->condition('id', $masterEnrollId,'=');
				  $masterMandatory = ($masterMandatory == 'Y') ? '1' : NULL ;
               /*  if($masterMandatory=='N'){
                    $masterMandatory= null;
                  } */
                  // Set new values of fields
                  $updateStmt->fields(array('mandatory' => $masterMandatory,'updated_on'=>now()));
                  $mandatory_updated = $updateStmt->execute();
                  expDebug::dPrintDBAPI(' $orderId = ' , $updateStmt);
        }
		if($masterEnrollId){
          // Notification for learning plan register
           if($fromPage == 'MyTeam') {
           	 $notificationType 	= "certCurrLPAssigned";

           	 // Add an audit trail entry for TP register
           	 $loggedInUserId = getSltpersonUserId();
           	 $tpDetails = getTrainingProgramDetails($objectId, array('type', 'object_type'));
           	 $objectTypeName = getProfileListItemName($tpDetails['object_type']);
           	 add_audit_trail_entry($loggedInUserId, $objectId, $tpDetails['object_type'], 'exp_sp_myteam', 'Assign Learning (' . $tpDetails['type'] . ')',
                                      'Manager enrolled a learner to a ' . $objectTypeName, $userId, null, null, $masterEnrollId);
           }
           else if($tpEnrollStatus == 'RG' ){

           	
           	if($mobilereq == false  && $isadmingrp > 0 && (($isAdminSide=="Y") || ($apinamenotify=="AddTPRosterAPI") || ($apinamenotify=="RegisterTPAPI"))) {            	
           	$notificationType 	= ($recertifyValue == 'R') ? "certCurrLPRecertifyByAdmin" : "certCurrLPRegisterByAdmin";
           	} else if($mobilereq == false && $mancnt == 'cre_sys_inv_man' && $isAdminSide=="N") {
           	$notificationType 	= ($recertifyValue == 'R') ? "certCurrLPRecertifyByAdmin" : "certCurrLPRegisterByAdmin";           		 
           	} else {
           	$notificationType 	= ($recertifyValue == 'R') ? "certCurrLPRecertify" : "certCurrLPRegister";
           	}
          	
          	 //audit trail entry
          	 $functionality_name = $recertifyValue ? 'tprecertify' : 'tpregistered' ;
          	 $loggedInUserId = getSltpersonUserId();
          	 $tpDetails = getTrainingProgramDetails($objectId, array('id','object_type'));
          	 $objectTypeName = getProfileListItemName($tpDetails['object_type']);

          	 $selectOverallStatus = db_select('slt_master_enrollment','mas');
    		 		 $selectOverallStatus->addField('mas','overall_status');
    		 		 $selectOverallStatus->condition('mas.id', $masterEnrollId);
          	 $OverallStatus   = $selectOverallStatus->execute()->fetchField();
             $tpDetails['overall_status'] =  $OverallStatus;
             expDebug::dprint('logged user : '.$userId,4);//user
             expDebug::dprint('modified user : '.$objectId,4);//admin
             expDebug::dprint('isAdminSide : '.$isAdminSide,4);
            // if($isAdminSide == "N" || $_GET['apiname'] == 'RegisterTPAPI') {
               make_audit_trail_entries($tpDetails, $loggedInUserId , null ,$objectId, 'Inserted',$tpDetails['object_type'],'exp_sp_learning',$functionality_name,
                                           array('id','object_type','overall_status'),$userId);
            // }

             if($recertifyflag != 'R')
             	 insertUserPointsForEachActionsPerformed($userId,'register_class',$masterEnrollId,'insert','tp');

            $compStatus = getPrmCompletedOrNot($objectId,$userId);
        	if((($compStatus > 0)&& ($isAdminSide == "N")) || (($compStatus > 0) && $_GET['apiname'] == 'RegisterTPAPI')){
        	 make_audit_trail_entries($tpDetails, $loggedInUserId, $defaultvalue, $objectId, 'updated',$tpDetails['object_type'],'exp_sp_learning','tpcompleted',
                                          array('overall_status'), $userId);
           }
           }
		   else if($tpEnrollStatus == 'WL'){
		   	
		   	if($mobilereq == false  && $isadmingrp > 0 && (($isAdminSide=="Y") || ($apinamenotify=="AddTPRosterAPI") || ($apinamenotify=="RegisterTPAPI"))) {  
		   	   $notificationType 	=  "certCurrLPWaitlistRegisterByAdmin";
		   	   }
		   	 else {
          	 $notificationType 	= "certCurrLPWaitlistRegister";
           }

           }
           
          $objectId1 			= $masterEnrollId;
          $objectId2 			= $objectId;
          $updateUserId 		= $vLMSUserId;

          $iCal = null;
    	    if ($notificationType == 'certCurrLPRegister' || certCurrLPRegister=="certCurrLPRegisterByAdmin" || $notificationType == 'certCurrLPAssigned' || $notificationType == 'certCurrLPRecertify' || $notificationType == 'certCurrLPRecertifyByAdmin') {
            $iCal = getPublishICal($masterEnrollId, 'TP');
    	    }

          insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId, $iCal);
		}
      }

      //check for already completed equivalence course.
      /**
       *0024925: Enforce equivalence in training plans(amaran)
       */
      $classids  = $activeClassCount[0]->classids;
      $courseids = $activeClassCount[0]->courseids;
      $explodedClassids = explode(',', $classids);
      $explodedCourseids= explode(',', $courseids);

      expDebug::dPrint('checking the active $explodedCourseids'.print_r($explodedCourseids,true).'user id'.$vLMSUserId,4);

      for($i=0;$i<count($explodedCourseids);$i++) {
        expDebug::dPrint('checking the active $explodedCourseids'.print_r($explodedCourseids[i],true),4);
        $isEquivalenceComplete = checkEquivalenceCourseComplete($vLMSUserId, $explodedCourseids[$i]);
        expDebug::dPrint('checking the active $isEquivalenceComplete'.print_r($isEquivalenceComplete,true).'user id'.$vLMSUserId,4);

        //if already finished equivalence course then update that course into completed status.
        if($isEquivalenceComplete){
        		if(count($explodedCourseids) > 2) {
          $slt_enrollment_updated = db_update('slt_enrollment'); // Table name no longer needs {}
          $slt_enrollment_updated->fields(array('comp_status' => 'lrn_crs_cmp_cmp',));
          $slt_enrollment_updated->condition('course_id', $explodedCourseids[$i], '=');
          $slt_enrollment_updated->condition('user_id', $vLMSUserId, '=');
          		expDebug::dPrintDBAPI("tp equivalenc check  course more than 2 ", $slt_enrollment_updated);
          $slt_enrollment_updated->execute();
        		}else {
        			expDebug::dPrint('checking the active $isEquivalenceComplete3333'.print_r($isEquivalenceComplete,true).'user id'.$vLMSUserId,4);
        			$query = db_select('slt_master_enrollment', 'sltmasenr');
        			$query->join('slt_enrollment','sltenr', 'sltmasenr.id = sltenr.master_enrollment_id');
        			$query->join('slt_program','sltpro', 'sltpro.id = sltmasenr.program_id');
        			$query->addField('sltmasenr', 'program_id', 'program_id');
        			$query->addField('sltpro', 'object_type', 'entity_type');
        			$query->addField('sltenr', 'master_enrollment_id', 'master_enrollment_id');
        			$query->addField('sltenr', 'reg_status', 'reg_status');
        			$query->addField('sltenr', 'reg_date', 'reg_date');
        			$query->condition('sltenr.course_id', $explodedCourseids[$i], '=');
        			$query->condition('sltenr.user_id', $vLMSUserId, '=');
        			expDebug::dPrintDBAPI("tp equivalenc check course 2 ", $slt_enrollment_updated);
        			$program = $query->execute()->fetchObject();
        		 	$param_array = array($program->program_id,$program->entity_type,$program->master_enrollment_id,$program->reg_status,$program->reg_date,'lrn_crs_cmp_cmp','','','','');
        		 	module_load_include('inc', 'exp_sp_administration_program', 'exp_sp_administration_program_roster');
        		 	$param = implode('$$$',$param_array);
        		 	$course_update = updateProgramRoster($param ,$response=1);
        		 	module_load_include('inc', 'exp_sp_administration_catalog', 'exp_sp_administration_catalog_roster');
        		 	$program_update = rosterProgramComplete($vLMSUserId, $program->master_enrollment_id, $program->program_id, $vLMSUserId, '', $vCompletionDate = '',$isFromAdmin=0);
        }
      }
  }
  }
  syncSolrData('Enrollment');
  syncSolrData('MasterEnrollment');
  syncSolrData('User');
  if(!$showResponse){
  	return '';
  }
  if($isAdminSide!="Y" && !$_REQUEST['apiname']) {
    print $program_msg;
  }else if($isAdminSide!="Y" && $_REQUEST['apiname']){
  	return $program_msg;
  }else if($isAdminSide == "Y" && $fromPage == "newadmin") {
    if($clcount >= 1){
      print $program_msg;
    }else {
      return $program_msg;
    }
  }else {
    return $result;
  }
  
  }catch (Exception $ex) {
    watchdog_exception('register_user_to_training_plan', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseTitle($classId){
  try{
	$selectCls = db_select('slt_course_class','cls');
	$selectCls->Join('slt_course_template', 'crs', 'crs.id=cls.course_id');
	$selectCls->addField('crs','title', 'crs_title');
	$selectCls->condition('cls.id', $classId,'=');
	return $selectCls->execute()->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getCourseTitle', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseIdFromClassId($classId){
  try{
	$selectCls = db_select('slt_course_class','cls');
	$selectCls->addField('cls','course_id', 'course_id');
	$selectCls->condition('cls.id', $classId,'=');
	return $selectCls->execute()->fetchField();
  }catch (Exception $ex) {
    watchdog_exception('getCourseIdFromClassId', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseTitleDirect($courseId){
  try{
	$selectCls = db_select('slt_course_template','crs');
	$selectCls->addField('crs','title', 'crs_title');
	$selectCls->condition('crs.id', $courseId,'=');
	$selectCls->condition('crs.status','lrn_crs_sts_del','!=');
	$result =  $selectCls->execute()->fetchAssoc();
	return $result['crs_title'];
  }catch (Exception $ex) {
    watchdog_exception('getCourseTitleDirect', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseMappingCount($objectId,$recertifyValue = '') {
  try{
  	
  if(strpos($recertifyValue, 'R')>0){
  	$recertify = explode('-',$recertifyValue);
  	$path = $recertify[0]+1;
  	$module_id = getModuleIdFromObjectId($objectId,$path);
  }
  else{
  	$module_id = getModuleIdFromObjectId($objectId,1);
  }
   
  	$selectCrsCnt = db_select('slt_module_crs_mapping','mapping');
	$selectCrsCnt->addExpression('COUNT(mapping.id)', 'crscnt');
	$selectCrsCnt->condition('mapping.program_id', $objectId,'=');
	$selectCrsCnt->condition('mapping.module_id', $module_id[0]->module_id ,'=');
	expDebug::dPrintDBAPI(' $getCourseMappingCount = ' , $selectCrsCnt);
	$totalcrsmappingcnt = $selectCrsCnt->execute()->fetchField();
	return $totalcrsmappingcnt;
  }catch (Exception $ex) {
    watchdog_exception('getCourseMappingCount', $ex);
    expertusErrorThrow($ex);
  }
}
function getTpTitleDirect($programId){
  try{
	$selectPrg = db_select('slt_program','prg');
	$selectPrg->addField('prg','title', 'prg_title');
	$selectPrg->condition('prg.id', $programId,'=');
	$result =  $selectPrg->execute()->fetchAssoc();
	return $result['prg_title'];
  }catch (Exception $ex) {
    watchdog_exception('getTpTitleDirect', $ex);
    expertusErrorThrow($ex);
  }
}

function getCourseClassCountForLP($objectId,$userId,$isAdminSide='',$recertifyValue=''){
	try{
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;

		if(strpos($recertifyValue, 'R')>0){
			$recertify = explode('-',$recertifyValue);
			$path = $recertify[0]+1;
			$modId = getModuleIdFromObjectId($objectId,$path);
		}
		else{
			$modId = getModuleIdFromObjectId($objectId,1);
		}
		
    $selectCls = db_select('slt_course_class', 'cls');
		$selectCls->leftjoin('slt_module_crs_mapping', 'crs','crs.course_id = cls.course_id');
		/*
    if($recertifyValue == 'R') {
    	$selectCls->leftjoin('slt_re_certify', 'crs','crs.course_id = cls.course_id');
    }else{
    	$selectCls->leftjoin('slt_module_crs_mapping', 'crs','crs.course_id = cls.course_id');
		} */
    // $selectCls->leftjoin('slt_course_class_session', 'ses','ses.class_id=cls.id AND ses.course_id=cls.course_id'); // 75695 - TP details page - Catalog page loads for long time 
    $selectCls->leftjoin('slt_course_content_mapper', 'content','content.class_id=cls.id AND content.course_id=cls.course_id');
    $selectCls->leftjoin('slt_profile_list_items', 'master_view','master_view.code=cls.delivery_type');
    $selectCls->leftjoin('slt_business_rule_mapping', 'sbrm','sbrm.entity_id=cls.id AND sbrm.business_rule_code=\'cre_sys_brl_rra\'');
    	if($isAdminSide != 'Y' && !empty($userId) && is_numeric($userId)) { // uncommented by Rajeshwar on Dec 23, 2016 for #0070652 
	 		// Fix for #0073700
    	if(strpos($recertifyValue, 'R')>0)
    		filterByUserAccessPrivileges($selectCls, 'cls',$userId,'','cre_sys_obt_cls','Class');		
    	else
	 filterByUserAccessPrivileges($selectCls, 'cls',$userId,'','cre_sys_obt_cls','Class','','','','','enr');
         //// 	  $classsr_Query = 'if((select count(1) from slt_group_mapping grp_mapp where grp_mapp.entity_id = cls.id AND grp_mapp.entity_type = \'cre_sys_obt_cls\')>0,(gmap.entity_id=cls.id
// 	  AND gmap.entity_type = \'cre_sys_obt_cls\'),(gmap.entity_id=cls.course_id AND gmap.entity_type = \'cre_sys_obt_crs\'))';
// 	  $selectCls->where('if(group_map.id is not null,group_map.id = (select gmap.id from slt_group_mapping gmap where ('.$classsr_Query.') and gmap.group_type=0 order by mro asc limit 1),1=1)');
	}
    if(!empty($userId)) {
      $user_id = explode(',',$userId);
     /*$selectCls->leftJoin('slt_catalog_access','catacs','catacs.entity_id=cls.id AND catacs.entity_type = \'cre_sys_obt_cls\'');
      $selectCls->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.job_role_id = (SELECT job_role FROM slt_person WHERE id=\''.$user_id[0].'\') AND mro.entity_type = \'cre_sys_obt_cls\'');*/

     // $selectCls->leftJoin('slt_group_mapping','group_map','group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\' and group_map.group_type = 0');
     // $selectCls->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');

    	// Completion status condition added by Vincent on 05 Dec, 2013 for #0026273
   	  $selectCls->leftjoin('slt_enrollment', 'enr','enr.course_id=cls.course_id AND enr.user_id IN ('.$userId.') AND enr.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\')
   		   AND IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
    }
	$selectCls->addExpression('COUNT(DISTINCT(cls.id))', 'maxcnt');
	$selectCls->addExpression('GROUP_CONCAT(DISTINCT(cls.id))', 'clsid');
	$selectCls->addExpression('GROUP_CONCAT(DISTINCT(crs.course_id))', 'crsid');
	$selectCls->condition('crs.program_id', $objectId,'=');
		$selectCls->condition('crs.module_id', $modId[0]->module_id,'=');
		
	$cond = (strpos($recertifyValue, 'R')>0) ? '(cls.status=\'lrn_cls_sts_atv\')' : "(1=1)";
	if(!empty($userId)) {
		// commented below condition for #0073700
		//$selectCls->where('IF(enr.course_id=cls.course_id,enr.class_id=cls.id,1=1)');
		$selectCls->where('IF(enr.course_id=cls.course_id AND enr.class_id=cls.id,'.$cond.',cls.status=\'lrn_cls_sts_atv\'
				AND IF(master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\', (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = cls.id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  IFNULL(CONVERT_TZ(now(), \''.$defaultTimezone.'\' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= cs.timezone)),NOW()) = 0), 1=1)
				AND (IF(master_view.attr1=\'WBT\' OR master_view.attr1=\'VOD\', content.id IS NOT NULL AND cls.status=\'lrn_cls_sts_atv\', 1=1))
				)');
		$selectCls->where('IF (sbrm.id > 0 && sbrm.id is not null, 1=1, (DATE(cls.registration_end_on) >= DATE(now()) or cls.registration_end_on is null))');
	}else{
		$selectCls->where('IF((master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\') , (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = cls.id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  NOW() = 0 AND cls.status=\'lrn_cls_sts_atv\'), 1=1)');
		$selectCls->where('IF ((master_view.attr1=\'WBT\' OR master_view.attr1=\'VOD\'), content.id IS NOT NULL AND cls.status=\'lrn_cls_sts_atv\', 1=1)');
		$selectCls->where('IF (sbrm.id > 0 && sbrm.id is not null, 1=1, (DATE(cls.registration_end_on) >= DATE(now()) or cls.registration_end_on is null))');
	}
	$selectCls->groupby('cls.course_id');
	$query = db_select($selectCls, 'clsdt');
	$query->addExpression('ifnull(MAX(clsdt.maxcnt),0)', 'cnt');
	$query->addExpression('GROUP_CONCAT(clsdt.clsid)', 'classids');
	$query->addExpression('GROUP_CONCAT(clsdt.crsid)', 'courseids');
	expDebug::dPrintDBAPI('getCourseClassCountForLP() : ' , $query);
	$maxCount = $query->execute()->fetchAll();
	$listcrsids = $maxCount[0]->courseids;
	if(!empty($listcrsids)) {
		$listcrsidsLength1 = explode(',',$listcrsids);
		$listcrsidsLength = sizeof($listcrsidsLength1);
	    $totalcrsmappingcnt =  getCourseMappingCount ($objectId,$recertifyValue);
	    if($listcrsidsLength != $totalcrsmappingcnt){
				$maxCount[0]->cnt = '-1';  // It refers No Classes under any one of the course.
		}
	}
	return $maxCount;
 	}catch (Exception $ex) {
	    watchdog_exception('getCourseClassCountForLP', $ex);
	    expertusErrorThrow($ex);
  	}
}
function getIfSingleClass($objectId,$userId = '',$is_compliance=0){
	try{
		$result_course = db_select('slt_course_class','cls');
		$result_course->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id AND ses.course_id=cls.course_id');
		if($is_compliance!=1){
			$result_course->leftJoin('slt_group_mapping','sltgmap','sltgmap.entity_id=cls.id AND sltgmap.entity_type = \'cre_sys_obt_cls\'');
			$result_course->leftJoin('slt_user_access_mapping','umap','umap.entity_id=cls.id AND umap.entity_type = \'cre_sys_obt_cls\' AND umap.user_id = \''.$userId.'\'');
		}
		$result_course->leftJoin('slt_course_content_mapper','content','content.class_id=cls.id AND content.course_id=cls.course_id');
		$result_course->leftJoin('slt_profile_list_items','master_view','master_view.code=cls.delivery_type');
		$result_course->leftJoin('slt_enrollment','enr','enr.course_id=cls.course_id and enr.user_id="'.$userId.'" and enr.comp_status="lrn_crs_cmp_cmp" and enr.reg_status="lrn_crs_reg_cnf"');
		$result_course->addField('cls','id','classids');
		$result_course->addExpression('COUNT(distinct cls.id)','maxcnt');
		$result_course->addExpression('COUNT(distinct enr.id)','cmpcnt');
		$result_course->condition('cls.course_id',$objectId,'=');
		$result_course->condition('cls.status','lrn_cls_sts_atv','=');
		if($is_compliance!=1)
			$result_course->where('if(umap.id is null,sltgmap.mro,umap.mro) = "cre_sys_inv_man"');
		$result_course->where('IF ((master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\'), (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = ses.class_id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  NOW() = 0 and cls.status=\'lrn_cls_sts_atv\'), (cls.status = \'lrn_cls_sts_atv\')) OR enr.id > 0');
		$result_course->where('IF ((master_view.attr1=\'WBT\'), content.id IS NOT NULL, 1=1)');
		$result_course->groupBy('cls.course_id');
		if($userId != ''){
			filterByUserAccessPrivileges($result_course,'cls',$userId,'','cre_sys_obt_cls','');
		}
		$maxCount = $result_course->execute()->fetchAll();
	  expDebug::dPrintDBAPI("getIfSingleClass ", $result_course);
	return $maxCount;
	}catch (Exception $ex) {
	    watchdog_exception('getIfSingleClass', $ex);
	    expertusErrorThrow($ex);
  	}
}

function getIfSingleClassCron($objectId,$userId,$is_compliance=0){
	try{
		$result_course = db_select('slt_course_class','cls');
		$result_course->leftJoin('slt_course_class_session','ses','ses.class_id=cls.id AND ses.course_id=cls.course_id');
		if($is_compliance!=1){
			$result_course->leftJoin('slt_group_mapping','sltgmap','sltgmap.entity_id=cls.id AND sltgmap.entity_type = \'cre_sys_obt_cls\'');
			$result_course->leftJoin('slt_user_access_mapping','umap','umap.entity_id=cls.id AND umap.entity_type = \'cre_sys_obt_cls\' AND umap.user_id = \''.$userId.'\' ');
		}
		//$result_course->leftJoin('slt_groups','grp','grp.id=catacs.group_id and grp.status=\'cre_sec_sts_atv\'');
		//$result_course->leftJoin('slt_mro_mapping','mro','mro.entity_id=catacs.entity_id AND FIND_IN_SET(mro.job_role_id,catacs.job_role) AND mro.entity_type = \'cre_sys_obt_cls\'');
		$result_course->leftJoin('slt_course_content_mapper','content','content.class_id=cls.id AND content.course_id=cls.course_id');
		$result_course->leftJoin('slt_profile_list_items','master_view','master_view.code=cls.delivery_type');
		$result_course->addField('cls','id','classids');
		$result_course->addExpression('COUNT(distinct cls.id)','maxcnt');
		$result_course->condition('cls.course_id',$objectId,'=');
		$result_course->condition('cls.status','lrn_cls_sts_atv','=');
		if($is_compliance!=1)
			$result_course->where('if(umap.id is null,sltgmap.mro,umap.mro) = "cre_sys_inv_man"');
		$result_course->where('IF ((master_view.attr1=\'ILT\' OR master_view.attr1=\'VC\'), (SELECT COUNT(cs.id) FROM slt_course_class_session cs WHERE cs.class_id = ses.class_id AND cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1) AND CONCAT(DATE(cs.start_date),\' \',CONCAT(cs.start_time,\':00\')) <  NOW() = 0), 1=1)');
		$result_course->where('IF ((master_view.attr1=\'WBT\'), content.id IS NOT NULL, 1=1)');
		$result_course->groupBy('cls.course_id');
		//filterByUserAccessPrivileges($result_course,'cls',$userId,'','cre_sys_obt_cls');

		expDebug::dPrintDBAPI("getIfSingleClassCron ", $result_course);
		$maxCount = $result_course->execute()->fetchAll();
		expDebug::dPrint("getIfSingleClassCron result ".print_r($maxCount,true),3);
		expDebug::dPrint("test max count ".$maxCount);
		return $maxCount;
	}catch (Exception $ex) {
		watchdog_exception('getIfSingleClassCron', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}
function getTpEnrollmentConfigValue() {
	try {
	$tp_enrollment_count = getConfigValue('tp_enrollment_max_count');
	if($tp_enrollment_count === '' || empty($tp_enrollment_count)) {
		$tp_enrollment_count = 100;
	} else {
		$tp_enrollment_count = trim($tp_enrollment_count);
	}
	return $tp_enrollment_count;
	}
	catch (Exception $ex) {
		watchdog_exception('getTpEnrollmentConfigValue', $ex);
		expertusErrorThrow($ex);
	} 
}
function getEnrollmentConfigValue() {
	try {
		$class_enrollment_count = getConfigValue('class_enrollment_max_count');
		if($class_enrollment_count === '' || empty($class_enrollment_count)) {
			$class_enrollment_count = 100;
		} else {
			$class_enrollment_count = trim($class_enrollment_count);
		}
		return $class_enrollment_count;
	}
	catch (Exception $ex) {
		watchdog_exception('getEnrollmentConfigValue', $ex);
		expertusErrorThrow($ex);
	}
}
function lp_singleclass_or_multiclass_cnt_for_course($objectId,$nodeId,$isCart,$isAdminSide,$userListIds = '',$pageandmandatory,$isAjax=true,$fromNewAdmin=false,$regDate ='',$recertify_flag = '',$isMandatory = ''){
	try{
	expDebug::dPrint("recertify flag" .$recertify_flag . "<<<". $_REQUEST['recertifyid']);
	$recertifyValue = ($isAdminSide == 'Y') ? $recertify_flag : $_REQUEST['recertifyid'];
	expDebug::dPrint('debug recertify-->'.print_r($recertifyValue,true),4);
	//$recertifyValue = $recertify[0];
	//$recertifylevel = $recertify[1];
	if($fromNewAdmin==true){	
	$module_seq = getModuleId($userListIds,$objectId);
	if(count($module_seq)>0) {
		if($module_seq[0]->object_type=="cre_sys_obt_crt" && $module_seq[0]->overall_status=="lrn_tpm_ovr_exp")
			$recertifyValue = $module_seq[0]->recertify_path.'-R';			
	} else {
				$recertifyValue = '1';
	}	
	}	 
	$val = explode('-',$pageandmandatory);
	$fromPage = $val[0];
	$MasterMandatory = $val[1];
	$tmpUserListId = explode(',', $userListIds);
    $maxCount = getCourseClassCountForLP($objectId,$tmpUserListId[0],$isAdminSide,$recertifyValue);
    expDebug::dPrint("Course class cnt -->>> ".print_r($maxCount,true));
   
    
    $totalcrsmappingcnt =  getCourseMappingCount($objectId,$recertifyValue);
    expDebug::dPrint("total crs map".print_r($totalcrsmappingcnt,1),5);
   
    
	if($totalcrsmappingcnt == 0){
     	$msgType = "nocourses";    // no courses associated with modules
	} else if(($maxCount[0]->cnt == '-1' || $maxCount[0]->cnt == 0)/*  && (count(explode(',',$maxCount[0]->courseids)) == $totalcrsmappingcnt) */){
		// for 43718: Incorrect alert message showing in TP enrollment, when one of class contains past session among multiple courses.
     	$msgType = "noclasses";     	// no classes under any one of the courses
	} else if($maxCount[0]->cnt == 1) {
	   	 $classId = $maxCount[0]->classids;
	   	 if($isCart){

	   	 if($_REQUEST['apiname'] == 'addTPToCartAPI' || $_REQUEST['apiname'] =='AddItemsToShoppingCartAPI'){
	   	 	expDebug::dPrint('test4 vincent ');
	   	   $msgType = 'singleRegister';
	   	  }
	   	  else{
			//	$msgType = callAddToCartTP('add',$nodeId,$objectId,'',$classId);
			//	changes done by Shobana for 54422: Issue in Training Plan --> Waitlist
	   	  	$availableSeats  = getObjectAvailableSeats($objectId,$userListIds,$recertifyValue);
	   	  	if($availableSeats==0) {
	   	  		$waitlist_status = getProgramWaitlistStatus($objectId,$userListIds,$isAdminSide,$recertifyValue);
	   	  	}
	   	  	expDebug::dPrint('Seat check : availableSeats -- ' .$availableSeats  , 4);
	   	  	if($availableSeats == 0 && $waitlist_status == 0) {
	   	  		$msgType  = 'seatsfull';
	   	  	}else if($availableSeats == 0 && $waitlist_status > 0 && strpos($recertifyValue, 'R')>0){
		          		$msgType = "MultiRegister";
	   	  	}else{
	   	  		$msgType = callAddToCartTP('add',$nodeId,$objectId,'',$classId);
	   	  	}
	   	  }

	   	 }
	   	 else{
	   	 	if($isAdminSide == 'Y') {
	   	 	  $aUserList = explode(',',$userListIds);
	   	 	  $ret1 = array();
	   	 	  $classArray = explode(',',$classId);
	   	 	  $tp_enrollment_count = getTpEnrollmentConfigValue();
	   	 	  $enroll_user_count = count($aUserList);
	   	 	  $classCount = count($classArray);
	   	 	  // If enrolled user count exceeding tp enrollment count converted to backend job.
	   	 	  $enrollmentCount = $enroll_user_count * $classCount;
			 if (($enrollmentCount > $tp_enrollment_count) && (empty($_REQUEST['apiname']))) {
	   	 	  	$res = tpEnrollmentRegistration('tp_enrollment',$objectId,$aUserList,$maxCount[0]->classids,$isMandatory);
	   	 	  	if($res == 'success') {
	   	 	  	  drupal_set_message(t('MSG730'));
	   	 	  	}
	   	 	  } else {
				$ret1 = array();
		      foreach($aUserList as $vUserId ){
		      	$seatAvail = 1;
		          $availableSeats  = getObjectAvailableSeats($objectId,$vUserId,$recertifyValue);
		          if($availableSeats==0)
		          	$waitlist_status = getProgramWaitlistStatus($objectId,$vUserId,$isAdminSide,$recertifyValue);
              		expDebug::dPrint('Seat check : availableSeats -- ' .$availableSeats  , 4);
              		if($availableSeats == 0 && $waitlist_status == 0) {
			            $seatAvail = 0;
			            $user_detail=db_query("select full_name from slt_person
			            		WHERE id = '".$vUserId."'");

			            $userDetail = $user_detail->fetchAll();
			            $fullName = $userDetail[0]->full_name;
			            $msg = t('ERR022');
			            $ret1[]  = $fullName." - ".$msg."</br>";
		          	}else if($availableSeats == 0 && $waitlist_status > 0 && strpos($recertifyValue, 'R')>0){
		          		$msgType = "MultiRegister";
		          	}
		        if($seatAvail){
		      	  $userId=($vUserId!=null && $vUserId!='')?$vUserId:'null';
		      	  $prereqcheck = checkCatalogPrerequisite($objectId,'cre_sys_obt_trp',$userId);
		      	  if(count($prereqcheck)>0){
			      	  $user_detail = db_select('slt_person', 'per');
			      	  $user_detail->addField('per','full_name','full_name');
			      	  $user_detail->condition('per.id',$userId);
	                  $userDetail = $user_detail->execute()->fetchAll();
	                  $fullName = $userDetail[0]->full_name;
	                  $ret1[] = $fullName." - ".t('MSG577')."</br>";
		      	  }else{
		    	  	 $ret1[] = register_user_to_training_plan($userId,$objectId, "$classId",false,$isAdminSide,true,$fromPage,$MasterMandatory,$regDate,null,$recertifyValue,$isMandatory);
		      	  }
		        }
		      }
		      if($fromNewAdmin)
		        return $ret1;
		      else
		        echo drupal_json_output($ret1);
				}
			}
	   	 	else{
		    $seatAvail = 1;
		    $availableSeats  = getObjectAvailableSeats($objectId,$userListIds,$recertifyValue);
		    if($availableSeats==0)
		     	$waitlist_status = getProgramWaitlistStatus($objectId,$userListIds,$isAdminSide,$recertifyValue);
         	expDebug::dPrint('Seat check : availableSeats -- ' .$availableSeats  , 4);
         	if($availableSeats == 0 && $waitlist_status == 0) {
			     	$msgType  = 'seatsfull';
		     }else if($availableSeats == 0 && $waitlist_status > 0 && strpos($recertifyValue, 'R')>0){
		          		$msgType = "MultiRegister";
		     	}else{
		    	 	$msgType = register_user_to_training_plan($userListIds,$objectId, "$classId",false,$isAdminSide,true,$fromPage,$MasterMandatory,$regDate,null,$recertifyValue,$isMandatory);
		      }
			  }
	   	 }
    } else {
      	$msgType = "MultiRegister";
    }
    if($isAjax)
      print $msgType;
    else
     return $msgType;
	}catch (Exception $ex) {
	    watchdog_exception('lp_singleclass_or_multiclass_cnt_for_course', $ex);
	    expertusErrorThrow($ex);
  	}
}

function lp_class_list_register($isAdminSide,$userListIds = '',$fromPage,$masterMandatory){
	try{
		$apiname = (isset($_GET['apiname']) ? $_GET['apiname'] : $_POST['apiname']);
		if($apiname != 'AddTPRosterAPI' && $apiname != 'RegisterTPAPI'){
	  $regDate = '';
      $listValues = json_decode($_POST['selectedItem']);
      $cnt=0;
  	  $classId = array();
  	  $courseId = array();
  	  foreach($listValues as $x){
  		$tpId = stripApostrophe($listValues->$cnt->tpid);
  		$courseId[] = stripApostrophe($listValues->$cnt->courseid);
  	  	$classId[] = stripApostrophe($listValues->$cnt->classid);
  	  	$recertifyId = $listValues->$cnt->recertifyid;
  		$cnt++;
  	  }
  	  $courseIdList = implode(',',$courseId);
  	  $classIdList = implode(',',$classId);
	}
	else{
		  $tpId = (isset($_GET['ProgramId']) ? $_GET['ProgramId'] : $_POST['ProgramId']);
		  $classIdList = (isset($_GET['ClassIds']) ? $_GET['ClassIds'] : $_POST['ClassIds']);
		  $regDate = (isset($_GET['RegDate']) ? $_GET['RegDate'] : $_POST['RegDate']);
		  $recertifyId = (isset($_GET['recertifyid']) ? $_GET['recertifyid'] : $_POST['recertifyid']);
	}
		// Ram Issue is Fixed For this Ticket #0034694. admin cannot Register the Priced Training plan
		$prgDetails = getTpInfoForNotifications($tpId);
		if ($prgDetails['price'] > 0 && $isAdminSide == 'Y'){
			$ret1  = print t('MSG716');
			return $ret1;
		}
    if($isAdminSide == 'Y') {
	     $aUserList = explode(',',$userListIds);
	     $ret1 = array();
	     $regReturn = array();
	     $seatAvail = 1;
	   	 $classArray = explode(',',$classIdList);
	   	 $tp_enrollment_count = getTpEnrollmentConfigValue();
	   	 $enroll_user_count = count($aUserList);
	   	 $enrClassCount = count($classArray);
	   	 $enrrCount = $enroll_user_count * $enrClassCount;
	  	 if (($enrrCount > $tp_enrollment_count) && (empty($apiname))) {
	  	 	
	  	 	if($masterMandatory == 'Y') {
	  	 		$mandValue = 1;
	  	 	}
	   	 	$tpResult = tpEnrollmentRegistration('tp_enrollment',$tpId,$aUserList,$classIdList,$mandValue);
	   	 	if($tpResult == 'success') {
	   	 		$ret1  = print t('MSG730');
	   	 	}
	   	 } else {
	      foreach($aUserList as $vUserId){
	        if($seatAvail == 1){
	          $availableSeats  = getAvailableSeatInClasses($classArray,$vUserId);
                expDebug::dPrint('Seat check : availableSeats -- ' .$availableSeats  , 4);
                if($availableSeats == 0) {
	            $seatAvail = 0;
	            $ret1  = print t('ERR022');
	            if($apiname == 'AddTPRosterAPI' || $apiname == 'RegisterTPAPI'){
	            	$ret1 = t('ERR022');
	            }
	          }
	        }
		    if($seatAvail){
	      	  $userId=($vUserId!=null && $vUserId!='')?$vUserId:'null';
	    	  $ret1 = register_user_to_training_plan($userId,$tpId, "$classIdList",true,$isAdminSide,true,$fromPage,$masterMandatory,$regDate,$cnt,$recertifyId);
		    }
				if($apiname == 'AddTPRosterAPI' || $apiname == 'RegisterTPAPI'){
	    	  $regReturn[] = $ret1;
	    	}
	      }
	   	 }
			if($apiname == 'AddTPRosterAPI' || $apiname == 'RegisterTPAPI' ){
	    	return $regReturn;
	      }
	       return $ret1;
    }else{
    	if($apiname == 'AddTPRosterAPI' || $apiname == 'RegisterTPAPI' ){
			$result = register_user_to_training_plan($userListIds,$tpId, "$classIdList",true,$isAdminSide,true,$fromPage,$masterMandatory,$regDate,null,$recertifyId);
			return $result;
		}
		register_user_to_training_plan($userListIds,$tpId, "$classIdList",true,$isAdminSide,true,$fromPage,$masterMandatory,$regDate,null,$recertifyId);
    }
	}catch (Exception $ex) {
	    watchdog_exception('lp_class_list_register', $ex);
	    expertusErrorThrow($ex);
  	}
}

function insertTPBackgroundJobDetails($erolluserCount,$tpId,$jobId,$regType) {
	try {
		$loggedUser = getSltpersonUserId();
		
		$jbName = 'tp_enrollment_background_process';
		$jobSts = 'inprogress';
		
		$update = db_update('slt_monitoring_jobs');
		$update->fields(array(
				'custom0'=>1
		));
		$update->condition('job_name',$jbName,'=');
		$update->execute();

		$insertStmt = db_insert('slt_temp_background_jobs');
		$insertStmt->fields(array(
				'job_name' => 'AR-'.$jobId,
				'created_by' => $loggedUser,
				'created_on' => now(),
				'updated_by' => $loggedUser,
				'updated_on' => now(),
				'custom1' => $erolluserCount,
				'custom2' => '',
				'custom3' => '',
				'custom4' => '',
				'status' => $jobSts,
				'program_id' => $tpId
		));
		expDebug::dPrintDBAPI("slt_temp_background_jobs check",$insertStmt,5);
		$insertStmt->execute();
	} catch(Exception $e){
		expDebug::dPrint('Error in insertTPBackgroundJobDetails'.$e,1);
		expertusLogError($e);
	}
}

function tpEnrollmentRegistration($regType,$tpId,$aUserList,$classIdList,$isMandatory) {
  try{
  	$enroll_user_count = count($aUserList);
  	$loggedUser = getSltpersonUserId();
  		$maxJobID = date('dmyHis');
  		expDebug::dPrint("max job id".$maxJobID,5);
  		insertTPBackgroundJobDetails($enroll_user_count,$tpId, $maxJobID,$regType);
  		if($regType == 'tp_enrollment') {
  			$tpDetails = getProgramObjDetails($tpId);
  			if($tpDetails[0]->object_type == "cre_sys_obt_crt") {
  				$dtype = 'Certification';
  			}
  			else if($tpDetails[0]->object_type == 'cre_sys_obt_cur') {
  				$dtype = 'Curricula';
  			}
  			else if($tpDetails[0]->object_type == 'cre_sys_obt_trn') {
  				$dtype = 'Learning Plan';
  			}
  		} else {
  			$tpDetails = getClsDetails($tpId);
  		}
		foreach($aUserList as $userids ){
			if($regType == 'class_enrollment') {
				$mand = getMandatoryOrNot($tpId,$userids);
			}
			else if($regType == 'tp_enrollment'){
				$mand = getMandatoryOrNot($tpId,$userids,1);
			}
			if($mand == 'cre_sys_inv_man' || $isMandatory == 1) {
				$mand = 'cre_sys_inv_man';
			} else {
				$mand = NULL;
			}
			$recordCount = checkExistsUnprocessedRecords($tpId,$userids);
			if($recordCount == 0) {
			$insertStmt = db_insert('slt_autoregister_temp');
				if($regType == 'tp_enrollment') {
					$insertStmt->fields(array(
						'cls_id' => $tpId,
						'cls_code' => $tpDetails[0]->code,
						'cls_title' => $tpDetails[0]->title,
						'cls_short_description' => $tpDetails[0]->short_desc,
						'price' => $tpDetails[0]->price,
						'currency_type' => $tpDetails[0]->currency_type,
						'userids' => $userids,
						'expires_in_value' => $tpDetails[0]->expires_in_value,
						'expires_in_unit' => $tpDetails[0]->expires_in_unit,
						'tp_class_ids' => $classIdList,
						'object_type' => $tpDetails[0]->object_type,
						'recertify_path'=> 1,
						'language_code' => $tpDetails[0]->lang_code,
						'delivery_type_name' => $dtype,
						'record_status' => 'PR',
						'created_on' => $tpDetails[0]->created_on,
						'published_on' => $tpDetails[0]->published_on,
						'mro_id' => $mand,
						'ascen' => ($mand == 'cre_sys_inv_man') ? '1': '0',
						'tp_enrollment_jobid' => 'AR-'.$maxJobID,
						'tp_user_id' => $loggedUser
			
				));
				} else {
					$isCompliance=getIfCourseCompliance($tpDetails[0]->course_id);
					$objType = 'Class';
					//$ascend = ($mand == 'cre_sys_inv_man') ? 1 : ($isCompliance == 1) ? 3 : Null;
					$ascend = ($mand == 'cre_sys_inv_opt') ? 3 : ($mand == 'cre_sys_inv_man') ? 1 : 2;
					if($tpDetails[0]->delivery_type == 'lrn_cls_dty_ilt')
						$delName = 'Classroom';
					else if($tpDetails[0]->delivery_type == 'lrn_cls_dty_vod')
						$delName = 'Video';
					else if($tpDetails[0]->delivery_type == 'lrn_cls_dty_wbt')
						$delName = 'Web-based';
					else if($tpDetails[0]->delivery_type == 'lrn_cls_dty_vcl')
						$delName = 'Virtual Class';
					
					$insertStmt->fields(array(
							'cls_id' => $tpId,
							'cls_code' => $tpDetails[0]->code,
							'cls_title' => $tpDetails[0]->title,
							'cls_short_description' => $tpDetails[0]->description,
							'price' => $tpDetails[0]->price,
							'currency_type' => $tpDetails[0]->currency_type,
							'userids' => $userids,
							'object_type' => $objType,
							'language_code' => $tpDetails[0]->lang_code,
							'delivery_type_code' => $tpDetails[0]->delivery_type,
							'record_status' => 'PR',
							'mro_id' => $mand,
							'is_compliance' => $isCompliance,
							'tp_user_id' => $loggedUser,
							'ascen' => $ascend,
							'crs_id' => $tpDetails[0]->course_id,
							'created_on' => $tpDetails[0]->created_on,
							'published_on' => $tpDetails[0]->published_on,
							'tp_enrollment_jobid' => 'AR-'.$maxJobID,
							'delivery_type_name' =>$delName,
							'location_id' => ($tpDetails[0]->delivery_type == 'lrn_cls_dty_ilt') ?$tpDetails[0]->location_id :NULL
								
					));
				}
			$newEnrollmentId = $insertStmt->execute();
			}
		} 
		
		$autoreglmt = getConfigValue('autoregister_limit');
		if($regType == 'tp_enrollment')
			$obj_type = $tpDetails[0]->object_type;
		else 
			$obj_type = 'Class';
		
		$sel = db_select('slt_autoregister_temp', 'autotmp');
		$sel->addExpression('count(1)','cnt');
		$sel->condition('autotmp.cls_id',$tpId);
		$sel->condition('autotmp.object_type',$obj_type);
		$sel->where('autotmp.batch_id is null');
		expDebug::dPrintDBAPI("Auto Reg count qry test- ",$sel);
		$cnt = $sel->execute()->fetchField();
		if($regType == 'tp_enrollment') {
			$explodedClassList = explode(',',$classIdList);
			$explodedClassCount = count($explodedClassList);
			$autoregLimit = round($autoreglmt/$explodedClassCount);
			$recCount = round(($enroll_user_count * $explodedClassCount ) /  $autoreglmt);
		} else {
			$autoregLimit = $autoreglmt;
			$recCount = round($enroll_user_count/$autoreglmt);
		}

		$pcnt = $recCount + 1;
		$tmpSelect = db_select('slt_enrollment','temp');
		$tmpSelect->addField('temp','dataload_by','bid');
		$tmpSelect->isNotNull('temp.dataload_by');
		$tmpSelect->where('SUBSTRING(temp.dataload_by, 1, 1) = \'A\'');
		$tmpSelect->orderBy('temp.id','desc');
		$tmpSelect->range(0,1);
		expdebug::dPrintDBAPI("GET BATCH ID - ",$tmpSelect);
		
		$btcCnt = $tmpSelect->execute()->fetchField();
			$btcCnt = explode('-',$btcCnt);
			$btId = $btcCnt[1] + 1;
		for($o=0;$o<$pcnt;$o++){
			$tmpSelect = db_select('slt_autoregister_temp','temp');
			$tmpSelect->addExpression('MAX(batch_id)','bid');
			$btcCnt = $tmpSelect->execute()->fetchField();
			if($btcCnt == $btId){
				$btId++;
			}elseif($btcCnt > $btId){
				$btId = $btcCnt + 1;
			}
			$upd = "UPDATE slt_autoregister_temp SET batch_id = :batch_id
			WHERE cls_id = :clsid AND object_type = :object_type AND batch_id IS NULL LIMIT $autoregLimit ";
			$arg = array(
					':batch_id'=>$btId,
					':clsid' => $tpId,
					':object_type'=>$obj_type,
			);
			expDebug::dPrintDBAPI("Auto Reg batch id update qry test- ",$upd,$arg);
			db_query($upd,$arg);
			db_query('COMMIT');
		}
		return "success";
		
	}
	catch (Exception $ex) {
		watchdog_exception('tpEnrollmentRegistration', $ex);
		expertusErrorThrow($ex);
	}
}
function checkExistsUnprocessedRecords($programid,$userid) {
	try {
	$select = db_select('slt_autoregister_temp','temp');
	$select->addExpression('count(1)','cnt');
	$select->condition('temp.cls_id',$programid);
	$select->condition('temp.cls_id',$programid);
	$select->condition('temp.userids',$userid);
	$select->condition('temp.record_status',array('IN'),'NOT IN');
	$cnt = $select->execute()->fetchField();
	expDebug::dPrintDBAPI("CHECK UNPROCESS",$select,5);
	return $cnt;
	}
	catch (Exception $ex) {
		watchdog_exception('checkExistsUnprocessedRecords', $ex);
		expertusErrorThrow($ex);
	}
	
}
function getCourseInfoDetails($moduleId,$userId,$isAdminSide,$recertifyValue='',$group_id='') {
  try{
  $select = db_select('slt_module', 'module');
  /*
  if(strpos($recertifyValue,'R')>0) {
    	$select->leftjoin('slt_re_certify', 'map','module.id = map.module_id');
  }else{
  	$select->leftjoin('slt_module_crs_mapping', 'map', 'module.id = map.module_id');
  }
  */
  	$select->leftjoin('slt_module_crs_mapping', 'map', 'module.id = map.module_id');

  $select->leftjoin('slt_course_template', 'crs', 'map.course_id = crs.id');
  $select->leftjoin('slt_course_class', 'cls', 'crs.id = cls.course_id');
  if($isAdminSide != 'Y' && !empty($userId) && is_numeric($userId)) {
         filterByUserAccessPrivileges($select, 'cls',$userId,'','cre_sys_obt_cls','Class','','','','','enroll');
         $clscondition=' AND enroll.class_id = cls.id';
  }
  if($isAdminSide != 'Y')
  	$select->leftjoin('slt_enrollment', 'enroll', '(enroll.course_id = crs.id'.$clscondition.') and enroll.user_id = '.$userId.' AND enroll.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\')
   		   AND IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
  $select->addExpression('count(1)', 'clscount');

  $select->addField('map', 'course_id', 		'crs_id');
  if($isAdminSide != 'Y')
        $select->addField('cls', 'id', 		'cls_id');
  $select->addField('map', 'is_required', 		'is_required');
  $select->addField('map', 'sequence', 		'sequence_no');
  $select->addField('crs', 'title', 			'crs_title');
  $select->addField('crs', 'code', 				'crs_code');
  $select->addField('crs', 'short_description', 		'crs_desc');
  $select->addField('crs', 'valid_from', 		'start_date');
  $select->addField('crs', 'valid_to', 		'end_date');
  $select->condition('module.id', $moduleId, '=');
  if(!empty($group_id))
  	$select->condition('map.group_id', $group_id, '=');
  
  if(isset($_REQUEST["mobile_request"]) && isset($_REQUEST['crs_id'])) {
  	$select->condition('map.course_id', $_REQUEST['crs_id'], '=');
  }
  //Commented for remove the enforcing child level visibility of classes under Training plan : Refer #0037979
  //filterByUserAccessPrivileges($select, 'crs','','','cre_sys_obt_crs','');
  $select->groupBy('crs.id');
  $select->orderBy('map.sequence');
  expDebug::dPrintDBAPI('query for the get course info details',$select);
  $retResult = $select->execute()->fetchAll();
  $courseDetails = array();
   foreach ($retResult as $record) {
   	$maxCount = getIfSingleClass($record->crs_id);
   	
   	
   	expDebug::dPrint("max single cnt ::".print_r($maxCount,1),5);
   	
    if($maxCount[0]->maxcnt == 1) {
        $record->single_class = $maxCount[0]->classids;
    }else{
        $record->single_class = '';
    }
     $courseDetails[] = array(
	     	 'clscount'          => $record->clscount,
	     	 'crs_id'            => $record->crs_id,
	     	 'cls_id'            => $record->cls_id,
	         'is_required'       => $record->is_required,
	         'sequence_no'		 => $record->sequence_no,
		     'crs_title'		 => $record->crs_title,
		     'crs_code'			 => $record->crs_code,
		     'crs_desc'			 => strip_tags($record->crs_desc),	//html tags are striped as the style property can make changes in the exising UI. Refer: #38907
		     'start_date'		 => $record->start_date,
		     'end_date'			 => $record->end_date,
     	     'registered_cnt'    => getRegisteredStatusForCourse($record->crs_id,$userId),
             'single_class'      => $record->single_class
     	   );
   }
   expDebug::dPrint('fetch_learning_courses_list() results ROKKKKKKK : = ' . print_r($courseDetails, true) , 4);
   return $courseDetails;
  }catch (Exception $ex) {
	 watchdog_exception('getCourseInfoDetails', $ex);
	 expertusErrorThrow($ex);
  }
}

function lp_fetch_module_list($programId,$userId,$isAdminSide,$frompage='',$is_API=0,$getUserDefaultCurrency=true){
	try{
		$recertifyValue = $_REQUEST['recertifyid'];
		expDebug::dPrint('lp_fetch_module_list() $args: = ' . print_r(func_get_args(), true) , 5);
		$select = db_select('slt_program', 'lp');
		$select->leftjoin('slt_module', 'module', 'lp.id = module.program_id');
		$select->leftjoin('slt_module_groups', 'grp', 'grp.module_id = module.id');
		$select->leftjoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id AND map.module_id = module.id AND map.group_id = grp.id ');

		if(strpos($recertifyValue,'R')>0){
			$recertify = explode('-',$recertifyValue);
    		$recertifylvl = $recertify[0]+1;
    		$module_id = getModuleIdFromObjectId($programId,$recertifylvl);
    		//$select->leftjoin('slt_re_certify', 'map','lp.id = map.program_id AND map.module_id = module.id');
		}else{
			$module_id = getModuleIdFromObjectId($programId,1);
			//$select->innerJoin('slt_module_crs_mapping', 'map', 'lp.id = map.program_id AND map.module_id = module.id');
		}
		$select->addField('module', 'id', 		'module_id');
		$select->addField('module', 'title', 		'module_title');
		$select->addField('module', 'code', 		'module_code');
		$select->addField('module', 'short_desc', 		'module_desc');
		if($is_API == 1)
		{
		 $select->addExpression('if(lp.addn_catalog_show = 1,lp.additional_info,null)','tp_additional_info');
		}
                if($_REQUEST['apiname']=='getTPdetailsAPI' || $_REQUEST['apiname']=='getTPClassesAPI' || $_REQUEST['apiname']=='getTPdetailsForEnrollmentAPI')
                {
                     $select->leftJoin('slt_tag_entity','te','((lp.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
                     $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
                     $select->addExpression("group_concat(distinct td.tagname)","tagname");
                }
		$select->addfield('grp','title','group_title');
		$select->addfield('grp','code','group_code');
		$select->addfield('grp','id','group_id');
		$select->condition('lp.id', $programId, '=');
		$select->condition('module.id', $module_id[0]->module_id, '=');
		$select->groupBy('grp.id');
		//$select->groupBy('module.id');
		$select->orderBy('grp.sequence');
		expDebug::dPrintDBAPI(" program module list ",$select);
		$retResult = $select->execute()->fetchAll();
		expDebug::dPrintDBAPI('query for the lp_fetch_module_list',$select);
		$ModuleDet = array();
		$courseInfo = array();
		$v = 0;
		expDebug::dPrint('lp_fetch_module_list() $retResult: = ' . print_r($retResult, true) , 5);
		foreach($retResult as $resModuleDetials) {
			$moduleId     = $resModuleDetials->module_id;
			$courseInfo = getCourseInfoDetails($moduleId,$userId,$isAdminSide,$recertifyValue,$resModuleDetials->group_id);
			$ModuleDet[$v]["module_details"]   = $resModuleDetials;
			if(count($courseInfo)>0) {
				for($i=0;$i<count($courseInfo);$i++) {
					expDebug::dPrint('fetch_learning_courses_list()123 results: = ' . print_r($courseInfoDet, true) , 3);
					if($is_API == 1) {
						$courseInfoDet[$i] = array('clscount'      => $courseInfo[$i]['clscount'],
								'crs_id'        => $courseInfo[$i]['crs_id'],
								'is_required'   => $courseInfo[$i]['is_required'],
								'sequence_no'   => $courseInfo[$i]['sequence_no'],
								'crs_title'     => $courseInfo[$i]['crs_title'],
								'crs_code'      => $courseInfo[$i]['crs_code'],
								'crs_desc'      => $courseInfo[$i]['crs_desc'],
								'start_date'    => $courseInfo[$i]['start_date'],
								'end_date'      => $courseInfo[$i]['end_date'],
								'registered_cnt'=> $courseInfo[$i]['registered_cnt'],
								'single_class'  => $courseInfo[$i]['single_class']
						);
					} else {
						$courseInfoDet[$i] = array('clscount'      => $courseInfo[$i]['clscount'],
								'crs_id'        => $courseInfo[$i]['crs_id'],
								'cls_id'        => $courseInfo[$i]['cls_id'],
								'is_required'   => $courseInfo[$i]['is_required'],
								'sequence_no'   => $courseInfo[$i]['sequence_no'],
								'crs_title'     => $courseInfo[$i]['crs_title'],
								'crs_code'      => $courseInfo[$i]['crs_code'],
								'crs_desc'      => $courseInfo[$i]['crs_desc'],
								'start_date'    => $courseInfo[$i]['start_date'],
								'end_date'      => $courseInfo[$i]['end_date'],
								'registered_cnt'=> $courseInfo[$i]['registered_cnt'],
								'single_class'  => $courseInfo[$i]['single_class']
						);
					}
					
					if(!isset($_REQUEST["mobile_request"])) {
					$classInfo = lp_fetch_class_list($courseInfo[$i]['crs_id'],$userId,1,$getUserDefaultCurrency,$isAdminSide,$frompage);
					expdebug::dPrint('$classInfo '. print_r($classInfo,true));
					if(count($classInfo)>0){
						for($j=0;$j<count($classInfo);$j++) {
							$classInfoDet[$j] = $classInfo[$j];
						}
					}else{
						$classInfoDet = array();
					}
					$courseInfoDet[$i]["class_details"] = $classInfoDet;
					$classInfoDet = array();
				}
				}
			} else {
				$courseInfoDet = array();
			}
			$ModuleDet[$v]["course_details"] = $courseInfoDet;
			$v++;
			$courseInfoDet=array();
		}
		expDebug::dPrintDBAPI(' fetch_learning_courses_list() SQL:  = ', $select);
		expDebug::dPrint('fetch_learning_courses_list() results: = ' . print_r($ModuleDet, true) , 3);
		if($is_API)
			return $ModuleDet;
		else
			return drupal_json_output($ModuleDet);

	}catch (Exception $ex) {
	 watchdog_exception('lp_fetch_module_list', $ex);
	 expertusErrorThrow($ex);
	}
}

function lp_fetch_class_list($courseId,$userId,$is_API=0,$getUserDefaultCurrency=true,$isAdminSide='',$frompage =''){
    try{
    	if($getUserDefaultCurrency == true) {
    		$UserDefaultCurrencyArr = getUserDefaultCurrency($is_API);
    	}
    $UserPrefCurrCode = $UserDefaultCurrencyArr['currency_code'];
    $UserPrefCurrSym = $UserDefaultCurrencyArr['currency_sym'];
    $UserPrefCurrLongCode = $UserDefaultCurrencyArr['currency_long_code'];
    
    
    // Added by Priya on Oct 30, 2013 for #0028593
    $timeZone = date_default_timezone(false);
    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
    $isMultiUser = strpos($userId, ',');

    $selectclass = db_select('slt_master_search', 'ms');
    $selectclass->leftjoin('slt_course_class', 'cl','ms.pk_id=cl.id AND ms.type=\'Class\'');
    $selectclass->leftjoin('slt_course_class_session', 'cs', 'ms.pk_id=cs.class_id and cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date, start_time LIMIT 1)');
    $selectclass->leftjoin('slt_location', 'ss','cs.location_id= ss.id');
    $selectclass->leftjoin('slt_facility', 'facl','facl.location_id= ss.id and cs.location_id = facl.location_id');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_deliverytype', 'splt_deliverytype.code=ms.delivery_type');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_status', 'splt_status.code=ms.status');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_language', 'splt_language.code=ms.lang_code');
    $selectclass->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=cs.timezone');
    $selectclass->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =ms.pk_id and if(ms.type=\'Class\',nl.entity_type = \'cre_sys_obt_cls\',nl.entity_type in (\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\'))');
    $selectclass->leftjoin('slt_country', 'sc', 'sc.country_code= ss.country');
    $selectclass->leftjoin('slt_state', 'st', 'st.country_code= ss.country and st.state_code= ss.state ');
    $selectclass->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE ms.pk_id = cont.class_id AND ms.parent_pk = cont.course_id LIMIT 1 )');
    //$selectclass->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=ms.pk_id AND IF(ms.type=\'Class\', catacs.entity_type = \'cre_sys_obt_cls\', catacs.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) ');

    //$selectclass->leftJoin('slt_group_mapping','group_map','group_map.entity_id=ms.pk_id AND IF(ms.type=\'Class\', group_map.entity_type = \'cre_sys_obt_cls\', group_map.entity_type IN (\'cre_sys_obt_trn\', \'cre_sys_obt_crt\', \'cre_sys_obt_cur\')) and group_map.group_type = 0');
   // $selectclass->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');

    //Added by Rajeshwar on Dec 23, 2016 for #0070652 
      if($isAdminSide != 'Y' && !empty($userId) && is_numeric($userId)) {
    filterByUserAccessPrivileges($selectclass, 'cl',$userId,'','cre_sys_obt_cls','Class','','','','','enr');
          $clscondition=' AND enr.class_id = cl.id';
    }
    $selectclass->leftjoin('slt_enrollment', 'enr','(enr.course_id=cl.course_id'.$clscondition.')AND enr.user_id IN ('.$userId.') AND enr.reg_status IN (\'lrn_crs_reg_cnf\',\'lrn_crs_reg_ppm\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\')
    		AND IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');

    $selectclass->addField('ms','type', 'object_type');
    $selectclass->addField('ms','parent_pk', 'crs_id');
    $selectclass->addField('ms','pk_id', 'cls_id');
    $selectclass->addField('ms','code', 'cls_code');
    $selectclass->addField('ms','name', 'cls_title');
    $selectclass->addField('ms','description', 'cls_short_description');
    $selectclass->addField('ms','delivery_type', 'delivery_type_code');
    $selectclass->addField('cl','currency_type', 'currency_type');
    $selectclass->addField('cl','custom4', 'export_compliance');
    $selectclass->addField('splt_deliverytype','name', 'delivery_type_name');
    $selectclass->addField('splt_status','name', 'status');
    $selectclass->addField('splt_language','name', 'language');
    $selectclass->addField('ss','name', 'location');
    $selectclass->addField('ss','name', 'locationName');
    $selectclass->addField('ss','addr1', 'loationAddr1');
    $selectclass->addField('ss','addr2', 'locationAddr2');
    $selectclass->addField('ss','city', 'LocationCity');
    $selectclass->addField('st','state_name', 'locationState');
    $selectclass->addField('ss','zipcode', 'locationZip');
    $selectclass->addField('ss','phone', 'locationPhone');
    $selectclass->addField('facl','id', 'facility_id');
    $selectclass->addField('facl','contact_addr1', 'loationAddr1');
    $selectclass->addField('facl','contact_addr2', 'loationAddr2');
    $selectclass->addField('facl','contact_city', 'locationCity');
    $selectclass->addField('facl','contact_state', 'locationState');
    $selectclass->addField('facl','contact_zipcode', 'locationZip');
    $selectclass->addField('facl','contact_phone', 'locationPhone');

    $selectclass->addField('nl','node_id', 'node_id');
    //$selectclass->addField('cl','price', 'price');
    $selectclass->addExpression('slf_convert_price(cl.price,cl.currency_type,\''.$UserPrefCurrCode.'\')','price');
    if($is_API)
    {
    	$selectclass->addExpression('if(cl.addn_catalog_show = 1,cl.additional_info,null)','class_additional_info');
    	$selectclass->addExpression('\''.$UserPrefCurrSym.'\'','currency_symbol');
    	$selectclass->addExpression('\''.$UserPrefCurrCode.'\'','currency');
    	$selectclass->addExpression('\''.$UserPrefCurrLongCode.'\'','currency_code');
    }
    else
   		$selectclass->addExpression('\''.$UserPrefCurrSym.'\'','currency_type');
    $selectclass->addField('cs','id', 'session_id');
    $selectclass->addField('cs','start_date', 'sess_start_date');
    $selectclass->addField('cs','start_time', 'sess_start_time');
    $selectclass->addField('cs','end_time', 'sess_end_time');
    $selectclass->addField('cs','end_date', 'sess_end_date');
    $selectclass->addField('cs','timezone', 'timezone'); // Added by Priya on Oct 30, 2013 for #0028593
    $selectclass->addField('sc','country_name', 'country_name');
    $selectclass->addField('cl','lang_code', 'language_code');
    $selectclass->addField('sc','country_code', 'country_code');
    $selectclass->addField('cl','registration_end_on', 'registration_end_on');
    $selectclass->addField('enr','course_id', 'enrcrs_id');
    $selectclass->addField('enr','class_id', 'enrclsid');
    $selectclass->addField('cl','course_id', 'clscrsid');
    $selectclass->addField('cl','id', 'clsid');
    $selectclass->condition('ms.type','class','=');
    $selectclass->condition('cl.course_id',$courseId,'=');
    $selectclass->addExpression("CONCAT( DATE_FORMAT(cs.start_date, _utf8'%Y-%m-%d'),' ',cs.start_time,':00')", 'session_end_datetime'); // Added by Priya on Oct 30, 2013 for #0028593
    $selectclass->where('IF(ms.type=\'Class\',
    ( splt_deliverytype.lang_code=\'cre_sys_lng_eng\'
    and splt_status.lang_code=\'cre_sys_lng_eng\'
    and splt_language.lang_code=\'cre_sys_lng_eng\'
    and nl.entity_type=\'cre_sys_obt_cls\'
    and IF(enr.course_id=cl.course_id AND enr.class_id=cl.id, 1=1 ,ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\'))
    		 ),1=1 )'
    );
    $selectclass->where('DATE(cl.registration_end_on) > DATE(now()) or cl.registration_end_on is null');
     //For Role Based
   // addFilterForLoggedInUserRole($selectclass, 'sepm');
   // Commented for removing the child level access in Training plan temporarily. #0037979
    /* if ($isMultiUser === false) {
      filterByUserAccessPrivileges($selectclass, '',$userId,'','','lp');
    }   */
    // Added by Vincent on Oct 29, 2013 for #0028593
    $selectclass->groupBy("cl.id having IF(enr.course_id=cl.course_id AND enr.class_id=cl.id , 1=1, session_end_datetime > IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= cs.timezone)),NOW()) or session_end_datetime is null)");
    expDebug::dPrintDBAPI(' lp_fetch_class_list SQL:  = ', $selectclass);
    $resClsDet = $selectclass->execute()->fetchAll();
	$ClassDet = Array();
	$sessionInfo = array();
	$i1 = 0;
	$n = 0;
	$v = 0;
	  foreach($resClsDet as $resClsDetials) {
	    $classId     = $resClsDetials->cls_id;
	    $resClsDetials->enrolled_id = getRegisteredOrNot($courseId,$classId,$userId);
		if(getLastCompCourseClass($courseId,$userId)->enrolled_id != ''){
	 		$resClsDetials->class_last_comp = getLastCompCourseClass($courseId,$userId);
		}else{
	 		$resClsDetials->class_last_comp = '';
		}
	    $resClsDetials->waitlist_status = getWaitlistCatalogInfo($classId);
	    $resClsDetials->availableSeats = getAvailableSeats($classId);
	    $reportee_id ='';
	    if($frompage == "MyTeam"){
	    	$reportee_id = $userId;
	    }
	    $sessionInfo = getSessionDetails($courseId,$classId,$resClsDetials->delivery_type_code,'',$reportee_id);
	    expDebug::dPrint('$sessionInfo : = ' . print_r($sessionInfo, true) , 4);
	    expDebug::dPrint('$courseId,$classId : = ' . $courseId ."---".$classId."---".count($sessionInfo)."----".$resClsDetials->delivery_type , 4);
	    $ClassDet[$v]["class_details"]   = $resClsDetials;
	    if(count($sessionInfo)>0) {
	          for($i=0;$i<count($sessionInfo);$i++) {
	          	// Added by Vincent on Oct 29, 2013 for #0028593
		          	if($resClsDetials->delivery_type_code=='lrn_cls_dty_ilt'){
		          		$sessionInfoDet[$i] = array( 'session_title' => $sessionInfo[$i]['session_title'],'ilt_session_day' => $sessionInfo[$i]['ilt_session_start_day'],
		          				'ilt_start_date' => $sessionInfo[$i]['ilt_session_start_date_format'],'ilt_start_time' =>$sessionInfo[$i]['ilt_session_start_time_format'],
		          				'ilt_start_form' => $sessionInfo[$i]['ilt_session_start_time_form'],'ilt_end_time' => $sessionInfo[$i]['ilt_session_start_end_format'],
		          				'ilt_end_form' => $sessionInfo[$i]['ilt_session_end_time_form'],'session_day' => $sessionInfo[$i]['session_start_day'],
		          				'start_date' => $sessionInfo[$i]['session_start_date_format'],'start_time' =>$sessionInfo[$i]['session_start_time_format'],
		          				'start_form' => $sessionInfo[$i]['session_start_time_form'],'end_time' => $sessionInfo[$i]['session_start_end_format'],
		          				'end_form' => $sessionInfo[$i]['session_end_time_form'], 'loc_tzcode' =>$sessionInfo[$i]['tz_code'], 
		          				 'loc_tz'  =>$sessionInfo[$i]['session_code'], 'user_tz' => $sessionInfo[$i]['user_tz'],'user_tzcode' => $sessionInfo[$i]['user_tzcode'],
		          				'session_id'  =>$sessionInfo[$i]['session_id'],
		          				'session_instructor_name' => $sessionInfo[$i]['session_instructor_name']
		          				);
		          	}else{
		          		$sessionInfoDet[$i] = array( 'session_title' => $sessionInfo[$i]['session_title'],'session_day' => $sessionInfo[$i]['session_start_day'],
		               							 'start_date' => $sessionInfo[$i]['session_start_date_format'],'start_time' =>$sessionInfo[$i]['session_start_time_format'],
		                                         'start_form' => $sessionInfo[$i]['session_start_time_form'],'end_time' => $sessionInfo[$i]['session_start_end_format'],
		                                         'end_form' => $sessionInfo[$i]['session_end_time_form'],
		          				'session_instructor_name' => $sessionInfo[$i]['session_instructor_name']
		          		);
		          	}
	            }
	        } else {
	          $sessionInfoDet = array();
	        }
	        $ClassDet[$v]["session_details"] = $sessionInfoDet;
	    $n++;
	    $v++;
	    $sessionInfoDet=array();
	  }
	  expDebug::dPrint('$ClassDet : = ' . print_r($ClassDet, true) , 4);
	  if($is_API){
	   return $ClassDet;
	  }
	  expDebug::dPrint('$sessionInfo : = ' . print_r($ClassDet, true) , 4);
	  return drupal_json_output($ClassDet);
    }catch (Exception $ex) {
	 watchdog_exception('lp_fetch_class_list', $ex);
	 expertusErrorThrow($ex);
  	}
}

function generateOpenSurveyMobileNotification($userId,$objectId1,$objectId2){
	try{
		$classArray = explode('~~',$objectId1);
		expDebug::dPrint('generateOpenSurveyMobileNotification'.print_r($classArray,1),5);
		$classid = $classArray[0];
		$getInfo1 = getClassInfoForNotifications($classid);
		$classadditionalinfo = convertNullValToNullStr($getInfo1['additional_info']);
    	$classissnoti = convertNullValToNullStr($getInfo1['addn_notification_show']);
		
		$getInfo = getPersonDetailsForNotifications($userId);
		$fullname    = '';
		$firstname   = convertNullValToNullStr(ucfirst($getInfo['first_name']));
		$notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|'.
				'classid>|' . $classArray[0]. '~|'.
				'sessionid>|' . $objectId2. '~|'.
				'class_title>|' . $classArray[1]. '~|' .
				(($classissnoti == 1) ? (($classadditionalinfo) ? ('add_notes>|'. $classadditionalinfo) : ('add_notes>|'.'-')) : '') . '~|';
		$notificationInfo['message_id']    = 'open_survey_mobile';
		$notificationInfo['message_type']  = 'Open survey from Mobile';
		$notificationInfo['send_type']     = 'pushNotification';
		$notificationInfo['lang_code']     = '';
		return $notificationInfo;
	}catch (Exception $ex) {
	 watchdog_exception('generateOpenSurveyMobileNotification', $ex);
	 expertusErrorThrow($ex);
	}
}

function generateRequestVerificationNotification($userId,$objectId1,$objectId2){
	try{
		
		expDebug::dPrint('simmmmmmm'.print_r($userId,1),5);
		expDebug::dPrint('generateOpensannn'.print_r($objectId1,1),5);
		expDebug::dPrint('newewewewe'.print_r($objectId2,1),5);
		//expDebug::dPrint('generateOpenSurveyMobileNotification'.print_r($classArray,1),5);
		//$classid = $classArray[0];
		//$getInfo1 = getClassInfoForNotifications($classid);
		//$classadditionalinfo = convertNullValToNullStr($getInfo1['additional_info']);
	//	$classissnoti = convertNullValToNullStr($getInfo1['addn_notification_show']);
        $CertificateDetailsquery = getMySkillDetails($userId,false,$objectId1,1);
        $CertificateDetails = $CertificateDetailsquery->execute()->fetchAssoc();
        expDebug::dPrint('generatedettt'.print_r($CertificateDetails,1),5);
        
        
        //$subject = "Requesting certificate verification for ";
		$getInfo = getPersonDetailsForNotifications($userId);
		
		$getVerifierInfo = getPersonDetailsForNotifications($objectId2);
		$fullname    = '';
		$fullname   = convertNullValToNullStr(ucfirst($getInfo['full_name']));
		$Verifiername   = convertNullValToNullStr(ucfirst($CertificateDetails['user_name']));
		
		$subject = t('MSG1114').' '.$fullname;
		$config           = getConfig("exp_sp");
		$configSiteUrl    = $config["admin_site_url"];
		$verify = "Verified";
		$reject = "Rejected";
		$certId = md5($objectId1);
		$insertstmt = db_insert("slt_external_certificate");
		$fields = array(
				'encrypted_id' => $certId,
				'certificate_id' => $objectId1
		);
		$insertstmt->fields($fields);
		expDebug::dPrintDBAPI(' Fetch users list query 2 =sasas ' , $insertStmt );
		$skillId = $insertstmt->execute();
	
		$validityfrom =date(" jS F Y",strtotime($CertificateDetails['valid_from']));
		$validityto = date(" jS F Y",strtotime($CertificateDetails['valid_to']));
		
		$validitydates = $validityfrom.' to '.$validityto;
		$validity = '<strong>Validity:</strong> '.$validitydates;
		if($CertificateDetails['valid_from'] == "0000-00-00"){
			$validity = '';
		}elseif($CertificateDetails['valid_to'] == "0000-00-00"){
			$validity = '<strong>Validity:</strong> from'.$validityfrom;
		}
		
		
		$VerifyUrl = trim($configSiteUrl).'/externalCertificate.php/?cretid='.$certId;
		$RejectUrl = trim($configSiteUrl).'/?q=learning/certificate/'.$objectId1.'/'.$reject;
	$link = '
    <iframe id = "verify_cert" src="'.$VerifyUrl.'" name="foo" style = "border:none; height:39px; width:84px; display: inline-block;vertical-align: middle;" >
      <form action="../mydetail.asp" method="post" target="foo">
       <input type="submit" value="Yes" name = "yes">
      </form>
    </iframe>'
  ;
		
        $compny_name = !empty($CertificateDetails['company'])?$CertificateDetails['company']:'  -';
        $cert_number = !empty($CertificateDetails['certificate_number'])?$CertificateDetails['certificate_number']:'  -';
        
        
		$verifyLink ='<a href="'.$VerifyUrl.'">Yes</a>';
		$RejectLink ='<a href="'.$RejectUrl.'">No</a>';
		$verification_duration = getConfigValue('Certificate_verification_expiry');
		$verification_duration=  !empty($verification_duration)? $verification_duration: 10;
		$filepath =  $_SERVER['DOCUMENT_ROOT'] .'/'.$CertificateDetails['certificate_img'];
		
		expDebug::dPrint('generatedetttaaa'.print_r($filepath,1),5);
		$notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|'.
				'verifier>|' . $Verifiername. '~|'.
				'certificate>|' . $CertificateDetails['skill_name']. '~|'.
				'verification_duration>|' . $verification_duration. '~|'.
				'company>|' . $compny_name. '~|'.
				'subject>|' . $subject. '~|'.
				'vreifylink>|' . $link. '~|'.
				'certificate_attachment>|' . $filepath. '~|'.
				'certificate_number>|' . $cert_number . '~|' .
				'validity>|' . $validity . '~|';
		$notificationInfo['message_id']    = 'request_cert_verification';
		$notificationInfo['message_type']  = 'ORequest for Certificate verification';
		$notificationInfo['send_type']     = 'php mailer';
		$notificationInfo['lang_code']     = '';
		return $notificationInfo;
	}catch (Exception $ex) {
	 watchdog_exception('generateRequestVerificationNotification', $ex);
	 expertusErrorThrow($ex);
	}
}



function CertificateVerificationStatus($userId,$objectId1,$objectId2){
	try{

		expDebug::dPrint('simmmmmmm'.print_r($userId,1),5);
		expDebug::dPrint('generateOpensannn'.print_r($objectId1,1),5);
		expDebug::dPrint('newewewewe'.print_r($objectId2,1),5);
		//expDebug::dPrint('generateOpenSurveyMobileNotification'.print_r($classArray,1),5);
		//$classid = $classArray[0];
		//$getInfo1 = getClassInfoForNotifications($classid);
		//$classadditionalinfo = convertNullValToNullStr($getInfo1['additional_info']);
		//	$classissnoti = convertNullValToNullStr($getInfo1['addn_notification_show']);
		$CertificateDetailsquery = getMySkillDetails($userId,false,$objectId1,1);
		$CertificateDetails = $CertificateDetailsquery->execute()->fetchAssoc();
		expDebug::dPrint('generatedettt'.print_r($CertificateDetails,1),5);
		$validityfrom =date(" jS F Y",strtotime($CertificateDetails['valid_from']));
		$validityto = date(" jS F Y",strtotime($CertificateDetails['valid_to']));
		
		$validitydates = $validityfrom.' to '.$validityto;
		$validity = '<strong>Validity:</strong> '.$validitydates;
		if($CertificateDetails['valid_from'] == "0000-00-00"){
			$validity = '';
		}elseif($CertificateDetails['valid_to'] == "0000-00-00"){
			$validity = '<strong>Validity:</strong> from'.$validityfrom;
		}

		$getInfo = getPersonDetailsForNotifications($userId);

		$getVerifierInfo = getPersonDetailsForNotifications($objectId2);
	//	$fullname    = '';
		$fullname   = convertNullValToNullStr(ucfirst($getInfo['first_name']));
		$Verifiername   = convertNullValToNullStr(ucfirst($CertificateDetails['user_name']));
		$status_verify = $CertificateDetails['verification_status'];
		$subject = t('MSG1115').' '.$status_verify;

		$verification_duration = getConfigValue('Certificate_verification_expiry');
		$verification_duration=  !empty($verification_duration)? $verification_duration: 10;
		$notificationInfo['tokens_string'] = 'first_name>|' . $fullname . '~|'.
				'verifier>|' . $Verifiername. '~|'.
				'certificate>|' . $CertificateDetails['skill_name']. '~|'.
				'company>|' . $CertificateDetails['company']. '~|'.
				'status>|' . $CertificateDetails['verification_status']. '~|'.
				'subject>|' . $subject. '~|'.
				'certificate_number>|' . $CertificateDetails['certificate_number']. '~|' .
				'validity>|' . $validity . '~|';
		$notificationInfo['message_id']    = 'cert_verification_status';
		$notificationInfo['message_type']  = 'Certificate verification status';
		$notificationInfo['send_type']     = 'php mailer';
		$notificationInfo['lang_code']     = '';
		return $notificationInfo;
	}catch (Exception $ex) {
	 watchdog_exception('CertificateVerificationStatus', $ex);
	 expertusErrorThrow($ex);
	}
}

function GenerateDeleteCertificateByLearner($userId,$objectId1,$objectId2){
    try{

        expDebug::dPrint('simmmmmmm del'.print_r($userId,1),5);
        expDebug::dPrint('generateOpensannn del'.print_r($objectId1,1),5);
        expDebug::dPrint('newewewewe del'.print_r($objectId2,1),5);
        $CertificateDetailsquery = getMySkillDetails($userId,false,$objectId1,'','','',1);
        $CertificateDetails = $CertificateDetailsquery->execute()->fetchAssoc();
        expDebug::dPrint('generatedettt'.print_r($CertificateDetails,1),5);


        //$subject = "Requesting certificate verification for ";
        $getInfo = getPersonDetailsForNotifications($userId);

        $getVerifierInfo = getPersonDetailsForNotifications($objectId2);
        $fullname   = convertNullValToNullStr(ucfirst($getInfo['full_name']));
        $Verifiername   = convertNullValToNullStr(ucfirst($CertificateDetails['user_name']));

        $subject = "$fullname has deleted the certificate submitted for verification";

        $validityfrom =date(" jS F Y",strtotime($CertificateDetails['valid_from']));
        $validityto = date(" jS F Y",strtotime($CertificateDetails['valid_to']));

        $validitydates = $validityfrom.' to '.$validityto;
        $validity = '<strong>Validity:</strong> '.$validitydates;
        if($CertificateDetails['valid_from'] == "0000-00-00"){
            $validity = '';
        }elseif($CertificateDetails['valid_to'] == "0000-00-00"){
            $validity = '<strong>Validity:</strong> from'.$validityfrom;
        }


        $notificationInfo['tokens_string'] = 'full_name>|' . $fullname . '~|'.
                'verifier>|' . $Verifiername. '~|'.
                'certificate>|' . $CertificateDetails['skill_name']. '~|'.
                'company>|' . $CertificateDetails['company']. '~|'.
                'subject>|' . $subject. '~|'.
                'certificate_number>|' . $CertificateDetails['certificate_number']. '~|' .
                'validity>|' . $validity . '~|';
        $notificationInfo['message_id']    = 'delete_cert_learner';
        $notificationInfo['message_type']  = 'Certificate in verification pending status is deleted by the learner';
        $notificationInfo['send_type']     = 'php mailer';
        $notificationInfo['lang_code']     = '';
        return $notificationInfo;
    }catch (Exception $ex) {
        watchdog_exception('generateRequestVerificationNotification', $ex);
        expertusErrorThrow($ex);
    }
}
/*
 * generateLPTerminateReminderNotifications() - gets the list of Learning Plan enrollment list which ends soon to send reminder
 */
/*function generateLPTerminateReminderNotifications($notificationType, $userId,$programId) {
  try{
  $getInfo = getLPTerminateDetailsForReminderNotifications($userId,$programId);
  //print_r($getInfo); die;
  $fullname           = '';
  $course_title       = '';
  $session_enddate    = '';
  $session_due_days   = '';
  $prg_lang_code      = '';
  $firstname         = convertNullValToNullStr(ucfirst($getInfo['first_name']));
  $course_title      = convertNullValToNullStr($getInfo['title']);
  $session_enddate   = convertNullValToNullStr($getInfo['session_end']);
  $session_due_days  = convertNullValToNullStr($getInfo['expire_days']);
  $prg_lang_code      = convertNullValToNullStr($getInfo['lang_code']);

  $notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|' .
									   'tp_title>|' . $course_title . '~|' .
									   'session_enddate>|' . $session_enddate . '~|'.
                                       'session_endtime>|' . $session_due_days . '~|';
  $notificationInfo['message_id']    = 'lp_termination_reminder';
  $notificationInfo['message_type']  = 'Learning Plan Termination Reminder';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = $prg_lang_code;
  return $notificationInfo;
  }catch (Exception $ex) {
	 watchdog_exception('generateLPTerminateReminderNotifications', $ex);
	 expertusErrorThrow($ex);
  }
}*/

/*
 * generateMobileAppDowloadNotification
 */
function generateMobileAppDowloadNotification($type){
  try{
  $firstname         = convertNullValToNullStr();
  if($type == 'Android'){
    $url = getConfigValue('android_download_url');
    $decodedUrl = urldecode($url);
  }
  else{
    $url = getConfigValue('iphone_download_url');
    $decodedUrl = urldecode($url);
  }
  $url = '<a href="'.$decodedUrl.'">'.t('Click here').'</a>';
  $notificationInfo['tokens_string'] = 'delivery_type>|' . $type . '~|'.
                                       'app_download>|' . $url . '~|';
  $notificationInfo['message_id']    = 'mobile_app_download';
  $notificationInfo['message_type']  = 'ExpertusONE Mobile Download';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = '';
  return $notificationInfo;
  }catch (Exception $ex) {
	 watchdog_exception('generateMobileAppDowloadNotification', $ex);
	 expertusErrorThrow($ex);
  }
}

function generateSuspendUserNotification($userId){
  try{
  $getInfo = getPersonDetailsForNotifications($userId);

  $fullname    = '';
  $username    = '';

  $firstname   = convertNullValToNullStr(ucfirst($getInfo['first_name']));
  $username    = convertNullValToNullStr($getInfo['user_name']);

  $notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|'.
                                       'user_name>|' . $username . '~|';
  $notificationInfo['message_id']    = 'suspend_user';
  $notificationInfo['message_type']  = 'Suspend User Notification';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = '';
  return $notificationInfo;
  }catch (Exception $ex) {
	 watchdog_exception('generateSuspendUserNotification', $ex);
	 expertusErrorThrow($ex);
  }
}

function generateActivateUserNotification($userId){
  try{
  $getInfo = getPersonDetailsForNotifications($userId);

  $fullname    = '';
  $username    = '';

  $firstname   = convertNullValToNullStr(ucfirst($getInfo['first_name']));
  $username    = convertNullValToNullStr($getInfo['user_name']);

  // #0048637 - Reset password URL added with User Actication Mail 
  $drupalUserId 		= getDrupalIdOfUser($userId);
  $account 				= user_load($drupalUserId);
  $reset_pass_url 		= '<a href="'.user_pass_reset_url($account).'" title="'.t('Link').'">'.strtolower(t('Link')).'</a>';
  $reset_pass_url 		= convertNullValToNullStr($reset_pass_url);
  
  $notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|'.
                                       'user_name>|' . $username . '~|'.
  									   'reset_pass_url>|' . $reset_pass_url . '~|';
  $notificationInfo['message_id']    = 'activate_user';
  $notificationInfo['message_type']  = 'Activate User Notification';
  $notificationInfo['send_type']     = 'php mailer';
  $notificationInfo['lang_code']     = '';
  return $notificationInfo;
  }catch (Exception $ex) {
	 watchdog_exception('generateActivateUserNotification', $ex);
	 expertusErrorThrow($ex);
  }
}

function generatePasswordWarningUserNotification($userId,$objectId1){
	try{
		$getInfo = getPersonDetailsForNotifications($userId);
		$fullname    = '';
		$firstname   = convertNullValToNullStr(ucfirst($getInfo['first_name']));
		$notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|'.
                                         'days_left>|' . $objectId1['!days_left'] . '~|'.
																				 'edit_uri>|' . $objectId1['!edit_uri'] . '~|';
		$notificationInfo['message_id']    = 'pwd_exp_warning';
		$notificationInfo['message_type']  = 'Password Warning Notification';
		$notificationInfo['send_type']     = 'php mailer';
		$notificationInfo['lang_code']     = '';
		return $notificationInfo;
	}catch (Exception $ex) {
	 watchdog_exception('generatePasswordWarningUserNotification', $ex);
	 expertusErrorThrow($ex);
	}
}

/*
 * getLPTerminateDetailsForReminderNotifications() - get all Learning plan which ends before given sequence days
 */
/*function getLPTerminateDetailsForReminderNotifications($user_id,$programId) {
  try{
  $select = db_select('slt_program','prgm');
  $select->innerJoin('slt_master_enrollment','mse','mse.program_id = prgm.id');
  $select->innerJoin('slt_person','prs','prs.id = mse.user_id');
  $select->leftJoin('slt_notification_info','info','info.notification_code = \'lp_termination_reminder\' AND  info.lang_code = \'cre_sys_lng_eng\'');
  $select->leftJoin('slt_entity_notification_mapping','map','map.notify_id = info.id AND map.entity_id=prgm.id');
  $select->addField('prs','id','personId');
  $select->addField('prs','first_name');
  $select->addField('prgm','title');
  $select->addField('prgm','id');
  $select->addField('prgm','lang_code');
  $select->addField('prgm','end_date','session_end');
  $select->addExpression("DATEDIFF(prgm.end_date,NOW())","expire_days");
  if($user_id>0) {
    $select->condition('prs.id',$user_id,'=');
    $select->condition('prgm.id',$programId,'=');
  }
  $select->condition('prgm.object_type','cre_sys_obt_trn','=');
  $select->condition('mse.overall_status','lrn_tpm_ovr_enr','=');
  $select->condition('info.status', 'cre_ntn_sts_atv');
  $select->where("IFNULL(map.status,'Y') = 'Y' AND DATEDIFF(prgm.end_date,NOW())  = IFNULL(map.notify_frequency,info.frequency_data_value)");
  expDebug::dPrintDBAPI(' getLPTerminateDetailsForReminderNotifications SQL:  = ', $select);
  $result = $select->execute();
  if($user_id>0) {
    $results = $result->fetchAssoc();
  } else {
    $results = $result->fetchAll();
  }
  return $results;
  }catch (Exception $ex) {
	 watchdog_exception('getLPTerminateDetailsForReminderNotifications', $ex);
  	if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	 expertusErrorThrow($ex);
  }
}*/

/**
 * function to check whther the course has any equivalents that are already registered by the user
 */

function checkEquivalence($userId, $courseId, $classId,$adminEnrollCall=false, $fromCart = false){
	try{
		$selectStmt = db_select('slt_common_mapping', 'map');
        $selectStmt->innerJoin('slt_enrollment','enr','(enr.reg_status IN(\'lrn_crs_reg_cnf\',\'lrn_crs_reg_rsv\',\'lrn_crs_reg_wtl\',\'lrn_crs_reg_ppm\')) 
        AND (IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)) AND (map.id2 = enr.course_id OR map.id1 = enr.course_id)');
        $selectStmt->innerJoin('slt_course_template','crs','(enr.course_id = crs.id) AND crs.status = \'lrn_crs_sts_atv\'');
        // $selectStmt->innerJoin('slt_course_template','temp','(map.id2 = temp.id OR map.id1 = temp.id) AND crs.status = \'lrn_crs_sts_atv\'');
        
        //$selectStmt->innerJoin('slt_course_template','enrcrs','(enr.course_id = enrcrs.id)');
		$selectStmt->addField('map','id2','equv_id');
        $selectStmt->addField('crs','title','equv_title');
		$selectStmt->addField('enr','reg_status','equv_status');
		$selectStmt->where( "((map.object_type = 'cre_sys_obt_crs') AND (map.type = 6) AND (map.id1 = $courseId))
				          OR ((map.prereq_type = 'cre_sys_pre_crs') AND (map.type = 6) AND (map.id2 = $courseId))" );
  		$selectStmt->condition('enr.user_id', $userId);
       // $selectStmt->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_rsv','','lrn_crs_reg_wtl','lrn_crs_reg_ppm') ,'IN');
	    // Below condition modified by Vincent on 05 Dec, 2013 for #0026273
       // $selectStmt->where('IF(enr.reg_status = \'lrn_crs_reg_cnf\', enr.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
	    $selectStmt->groupBy('enr.course_id');
        //expDebug::dPrintDBAPI('MY EQUV--->>>' , $selectStmt);
	    $result = $selectStmt->execute()->fetchAll();

	    if($adminEnrollCall==true)
	    	return $result;
	    if(count($result)>0) {
			  $result[0]->errormsg = array('msg1' => t('ERR062'),'msg2' => t('ERR063').':','msg3' => t('ERR064'));
	    }
	    $enrollCnt = complianceExpiryandRegisterCount($courseId,'',$userId);
	    if($enrollCnt){
	    	$result[0]->enrollexist = 'Y';
	    }
	    $multicls = strpos($classId, '-');
	    if($multicls === false && count($result)>0){
	      $classregcount = getCourseClassRegisteredcount($courseId,$classId,$userId);
	      $result[0]->recurringcount = $classregcount['recurring_res'];
	      $result[0]->class_cnt = $classregcount['class_cnt'];
	    }
	    
	    global $catalog_reg;
	    if ($catalog_reg == 'Course') {
	    	$tpregstatus = getRegisteredOrNotForCourseLevel($courseId,$classId,$userId,'','','','',1);
	    } else {
	    $tpregstatus = getRegisteredOrNot($courseId,$classId,$userId,'','','','',1);
	    }
	      
	    expDebug::dPrint('tp reg status '.print_r($tpregstatus,1),5);
	    if(!empty($tpregstatus) && count($tpregstatus)>0)
	    	$result[0]->tpenrollexist = (count($tpregstatus) > 0) ? 'Y' : '';

	    if($fromCart) {
	    	return $enrollCnt;
	    }
	    expDebug::dPrint("result -->> ".print_r($result,true),5);
	    
	    if($_REQUEST['mobile_request']){
	    	return $result;
	    }else{
	    	return drupal_json_output($result);
	    }
	    
	    
	}catch (Exception $ex) {
	 watchdog_exception('checkEquivalence', $ex);
	 expertusErrorThrow($ex);
  	}

}


/**
 * 0024925: Enforce equivalence in training plans
 * for this ticket we created equivalence option for training plan
 * don't delete it. Its may need for feature enhancement
 *
 */

/**
 * function to check whther the training plan has any equivalents that are already registered by the user
 * lp_singleclass_or_multiclass_cnt_for_course($objectId,$nodeId,$isCart,$isAdminSide,$userListIds = '',$pageandmandatory,$isAjax=true,$fromNewAdmin=false,$regDate ='',$recertify_flag = '')


function checkEquivalenceTP($userId, $objectId,$adminEnrollCall=false){
   //check for TP equivalence option
  $selectStmtTP = db_select('slt_common_mapping', 'map');
  $selectStmtTP->innerJoin('slt_program','program','(map.id1 = program.id OR map.id2 = program.id)');
  $selectStmtTP->innerJoin('slt_program','temp','(map.id2 = temp.id OR map.id1 = temp.id)');
  $selectStmtTP->innerJoin('slt_master_enrollment','menr','(map.id2 = menr.program_id OR map.id1 = menr.program_id)');
  $selectStmtTP->innerJoin('slt_program','sltpro','(menr.program_id = sltpro.id)');
  $selectStmtTP->addField('map','id2','equv_id');
  $selectStmtTP->addField('sltpro','title','equv_title');
  $selectStmtTP->addField('menr','overall_status','equv_status');
  $selectStmtTP->where( "((map.object_type = 'cre_sys_obt_trn') AND (map.type = 6) AND (map.id1 = $objectId))
                    OR ((map.prereq_type = 'cre_sys_pre_crs') AND (map.type = 6) AND (map.id2 = $objectId))" );
  $selectStmtTP->condition('menr.user_id', $userId);
  $selectStmtTP->condition('menr.overall_status', array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_inp','lrn_tpm_ovr_enr','lrn_tpm_ovr_att') ,'IN');
  $selectStmtTP->groupBy('menr.program_id');
  expDebug::dPrintDBAPI('MY EQUV checkEquivalenceTP--->>>' , $selectStmtTP);
  $resultTP = $selectStmtTP->execute()->fetchAll();

  if($adminEnrollCall==true)
    return $resultTP;
  return drupal_json_output($resultTP);
}
*/


/*
 * getClassInstructorInfoForNotifications() - Get class instructor information for notifications
 */
function getClassInstructorInfoForNotifications($classId) {
	try {
  $instructorInfo = 'instructor_first_name<| ~~instructor_last_name<| ~@';
  $deliveryType = getClassDeliveryType($classId, 'Short');
  expDebug::dPrint('Ilayaraja : Delivery Type-> '.$deliveryType , 4);
  if($deliveryType=='ilt'){
    $selectStmt = db_select('slt_object_role_mapping', 'rm');
    $selectStmt->leftJoin('slt_person','pers','pers.id = rm.user_id');
    // Select fields and/or add expressions
    $selectStmt->addField('pers', 'first_name', 'first_name');
    $selectStmt->addField('pers', 'last_name', 'last_name');
    // Set conditions
    $selectStmt->condition('rm.object_id', $classId);
    $selectStmt->condition('rm.object_type', 'cre_sys_obt_cls');
    // Execute query
    $result = $selectStmt->execute();
    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    if ($result->rowCount() <= 0) {
      return 'instructor_first_name<| ~~instructor_last_name<| ~@';
    }
    $instructorList  =  $result->fetchAllAssoc('id', PDO::FETCH_ASSOC);
    $instructorInfo  = '';
    foreach ($instructorList as $instructor) {
      $instructorFirstName = ($instructor['first_name'] == null)? ' ' : $instructor['first_name'];
      $instructorLastName = ($instructor['last_name'] == null)? ' ' : $instructor['last_name'];
      $instructorInfo =	$instructorInfo .
                        'instructor_first_name<|' . $instructorFirstName . '~~' .
                        'instructor_last_name<|' . $instructorLastName .
    					  '~@';
    } // end foreach
  }// end if
  return $instructorInfo;
	}catch (Exception $ex) {
	 watchdog_exception('getClassInstructorInfoForNotifications', $ex);
	 expertusErrorThrow($ex);
  	}
}

/*
 * getExpired Wbt content
 */
/* function getExpiredWbtContent() {
  try{
  $vLMSUserId = 1;
 /// $result_expiry =db_query("select map.class_id, max(map.validity_days) ,count(map.class_id) clscount, sum(if(map.validity_days is null,0,1)) validnull,
 //					enr.reg_date, DATE_ADD(enr.reg_date,INTERVAL min(map.validity_days) DAY) expire_date,enr.id,enr.user_id
// 					from slt_course_content_mapper map
//                 	left join slt_enrollment enr on enr.class_id=map.class_id
 //                  	group by map.class_id,enr.user_id
 //                   having validnull > 0 and date_format(expire_date,'%Y-%m-%d') < date_format(now(),'%Y-%m-%d')");
  $result_expiry =db_query("select map.class_id, max(map.validity_days) ,count(map.class_id) clscount, sum(if(map.validity_days is null,0,1)) validnull,
      enr.reg_date, DATE_ADD(enr.reg_date,INTERVAL min(map.validity_days) DAY) expire_date,enr.id,enr.user_id
      from slt_course_content_mapper map
      left join slt_enrollment enr on enr.class_id=map.class_id
      where enr.comp_status = 'lrn_crs_cmp_enr'
      group by map.class_id,enr.user_id
      having validnull > 0 and date_format(expire_date,'%Y-%m-%d') <= date_format(now(),'%Y-%m-%d')");

   expDebug::dPrintDBAPI('New query = ' , $result_expiry);
  if(count($result_expiry > 0)){
      foreach($result_expiry as $row_class){
       $get_classid[]= $row_class->class_id;
       $get_userid[]= $row_class->user_id;
      $select = db_update('slt_enrollment');
      $select->fields(array(
                            'comp_status'  => 'lrn_crs_cmp_inc',
                            'comp_date'    =>  now(),
                            'updated_on'    => now(),
                  			'updated_by'    => $vLMSUserId
                            ));
      $select->condition('reg_status','lrn_crs_reg_cnf','=');
      $select->condition('comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp') ,'IN');
      $select->condition('id',$row_class->id);
      expDebug::dPrint('Update the classs id ' . $row_class->class_id , 5);
      $result = $select->execute();
      expDebug::dPrintDBAPI('Update Query ' , $select);
      }

  }
     expDebug::dPrint('New query getting calass id = '.$get_classid, 5);
     expDebug::dPrint(' $itemsaaaaa = ' . print_r($get_classid, true), 5);
   if(count($get_classid) > 0 ) {
     $implode_class_id = implode(',', $get_classid);
     $implode_user_id = implode(',', $get_userid);
     $result_tp_expiry = db_query("select ms.program_id,ms.id from  slt_enrollment enr
     left join slt_master_enrollment ms on enr.master_enrollment_id = ms.id
     left join slt_module_crs_mapping map on map.program_id = ms.program_id where enr.class_id in (".$implode_class_id.") and ms.user_id in (".$implode_user_id.")
     and enr.master_enrollment_id is not null and map.is_required = 'Y' and enr.course_id=map.course_id");
      expDebug::dPrintDBAPI('New query Tp  = ' , $result_tp_expiry);
      if(count($result_tp_expiry > 0)){
            foreach($result_tp_expiry as $row_tp){
              $select = db_update('slt_master_enrollment');
              $select->fields(array(
                                    'overall_status' => 'lrn_tpm_ovr_inc',
                                    'updated_on'    => now(),
                				    'comp_date'    =>  now(),
                          		  'updated_by'    => $vLMSUserId
                                    ));
              $select->condition('overall_status',array('lrn_tpm_ovr_enr') ,'IN');
              $select->condition('id',$row_tp->id);
              expDebug::dPrint('Update the classs id ' . $row_tp->id , 5);
              $result = $select->execute();
              expDebug::dPrintDBAPI('Update Query ' , $select);
            }
      }
  }
  }catch (Exception $ex) {
	watchdog_exception('getExpiredWbtContent', $ex);
  if (isset($_GET['cron_key'])) {
		throw new Exception($ex);
	}
	expertusErrorThrow($ex);
  }
}*/
/*
 * getWaitlistCatalogInfo
 */
function getWaitlistCatalogInfo($classid,$userId = '',$From = '') {
  try{
	  $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl');
	  $no_of_confirmed = getClassEnrollmentCountForRegistrationStatuses(null, $classid, $regStatusesList,$userId);
	  $select = db_select('slt_course_class_session', 'sc');
	  $select->addField('sc','capacity_max', 'capacity_max');
	  $select->addField('sc','waitlist_count', 'waitlist_count');
	  $select->condition('sc.class_id',$classid, '=');
	  $select->range(0, 1);
	  $qry_result_session = $select->execute()->fetchObject();
	  expDebug::dPrintDBAPI("Get max and waitlist seats for class : ",$select);
	  $no_of_waitlist_seats = 0;
	  if($From == 'chkSeatCnt')
	  {
	  	$totSeatCnt = $qry_result_session->capacity_max + $qry_result_session->waitlist_count;
	  	$ava_waitlist_seats = 1;
	  	if($no_of_confirmed > 0 && $totSeatCnt > 0)
	  		$ava_waitlist_seats =  $totSeatCnt - $no_of_confirmed;
	  	return $ava_waitlist_seats;
	  
	  }
	  if ($qry_result_session->capacity_max != 0) {
	    if($qry_result_session->capacity_max <= $no_of_confirmed) {
	      $no_of_waitlist_seats = ($qry_result_session->capacity_max + $qry_result_session->waitlist_count) - $no_of_confirmed;
	    } else {
	      $no_of_waitlist_seats = ($From == 'tp') ? ($qry_result_session->capacity_max + $qry_result_session->waitlist_count) : 0;
	    }
	  }
	  if ($no_of_waitlist_seats < 0 ) {
	    $no_of_waitlist_seats = 0;
	  }
	  expDebug::dPrint("getWaitlistCatalogInfo action :".$classid."  ==> capacity_max: ".$qry_result_session->capacity_max." ,
	  waitlist_count: ".$qry_result_session->waitlist_count." , no_of_confirmed :".$no_of_confirmed.", Available seats : ".$no_of_waitlist_seats , 5);
	  return $no_of_waitlist_seats;
  }catch (Exception $ex) {
	watchdog_exception('getWaitlistCatalogInfo', $ex);
	expertusErrorThrow($ex);
  }
}
/*
 * getWaitlistPosition() - To get waitlist position of particular learner
 */
function getWaitlistPosition($classId,$courseId,$userId) {
  try{
  $selectStmt = db_select('slt_enrollment', 'enr');
  $selectStmt->addField('enr', 'user_id', 'user_id');
  $selectStmt->addField('enr', 'waitlist_priority', 'waitlist_priority');
  $selectStmt->condition('enr.class_id', $classId);
  $selectStmt->condition('enr.user_id', $userId);
  if($courseId != 0) {
    $selectStmt->condition('enr.course_id', $courseId);
  } //waitlist_priority
  $selectStmt->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl');
  $selectStmt->orderBy('reg_date', 'ASC');

  $selectStmt_qry        = $selectStmt->execute();
  $qry_waitlist_position = $selectStmt_qry->fetchAll();
  $userPosition          = 0;
  $i                     = 0;
  $userPosition = $qry_waitlist_position[0]->waitlist_priority;
  return $userPosition;
  }catch (Exception $ex) {
	watchdog_exception('getWaitlistPosition', $ex);
	expertusErrorThrow($ex);
  }
}
/**
 * Get results for Tag autocomplete.
 *
 * @param string ''
 *
 * @return
 *   Auto Complete Tag Results
 */

function getTagAutoComplete(){
  try{
  $tag = $_GET['z'];
  $select = db_select('slt_tagdefn', 'sl');
  $select->addField('sl', 'tagname');
  $select->condition('sl.tagname', '%' . db_like($tag) . '%', 'LIKE');
  $select->condition('sl.activity_count', 0 , '>');
  expDebug::dPrintDBAPI('getTagAutoComplete', $select);
  $qry_tag_autocomplete = $select->execute()->fetchAll();
  foreach ($qry_tag_autocomplete as $rec_tag_autocomplete) {
    print $rec_tag_autocomplete->tagname."\n";
  }
  }catch (Exception $ex) {
	watchdog_exception('getTagAutoComplete', $ex);
	expertusErrorThrow($ex);
  }
}

function setFiveStarRatingMobileAPI(){
	try{
		
		$entityType = $_REQUEST['entityType'];
		if($_REQUEST['entityType'] == 'Class'){
			$entityType = 'cre_sys_obt_cls';
		}
		
		$select = db_select('slt_node_learning_activity','sc');
		$select->addField('sc','node_id');
		$select->condition('sc.entity_id',$_REQUEST['classId']);
		$select->condition('sc.entity_type',$entityType);
		
		$nodeId = $select->execute()->fetchField();
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL); // general bootup for drupal modules
		$result = setFiveStarRating($nodeId,true);

		return $result;
	}catch (Exception $ex) {
		watchdog_exception('setFiveStarRating', $ex);
		expertusErrorThrow($ex);
	}
}

function setFiveStarRating($nodeId='',$isapi = false){
  try{
  	//$result= new stdClass();
	global $user;
	if($isapi){
		$userId  = $_REQUEST['user_id'];
		$user->uid = getDrupalIdOfUser($userId);
		$entity_type = $_REQUEST['entityType'];
	}else{
		$userId  = getIdOfLoggedInUser();
		$nodeId  =  $_POST['nodeId'];
		$entity_type = $_POST['entityType'];
	}
	expDebug::dPrint('on star rating userid :: '.$userId.' entity type'.$nodeId);
	  $selectStmt = db_select('votingapi_vote', 'vote');
	  $selectStmt->addField('vote', 'value', 'vote_value');
	  $selectStmt->condition('vote.entity_id', $nodeId);
	  $selectStmt->condition('vote.uid', $user->uid);
	  $selectStmt_qry        = $selectStmt->execute()->fetchAll();

	  if(count($selectStmt_qry) > 0){
	  	if($isapi){
	  		$result= new stdClass();
	  		$result->average_rating = 'AlreadyRated';
	  		$result ->classId = $_REQUEST['classId'];
	  		$result ->entity_type =$_REQUEST['entityType'];
	  		$result->vote_value = $selectStmt_qry[0]->vote_value;
	  		$result->vote_time = date('Y-m-d',$selectStmt_qry[0]->vote_time);
	  		return $result;
	  	}
	  	return drupal_json_output(array('average_rating'=>'AlreadyRated'));
	  }else{

	  	$userRatingValue = ($_POST['rating'][0] == "1" ? '20' :
						($_POST['rating'][0] == "2" ? '40' :
						($_POST['rating'][0] == "3" ? '60' :
						($_POST['rating'][0] == "4" ? '80' :
						($_POST['rating'][0] == "5" ? '100' : ''))
						))
						);

		if ($isapi) {
			$avgRating = _fivestar_cast_vote($_REQUEST['entityType'],$nodeId,$userRatingValue,'vote',$user->uid);
		} else {
			$avgRating = _fivestar_cast_vote($_POST['entityType'],$_POST['nodeId'],$userRatingValue,'vote',$user->uid);
		}

		$avgRatingTxt = array('votemsg'=>t('LBL148'),'votesmsg'=>t('LBL149'));
		
		//Added for #0068428 to take the entity_id
		$select = db_select('slt_node_learning_activity', 'atv');
		$select->addField('atv', 'entity_id', 'entity_id');
		$select->condition('atv.node_id', $nodeId);
		$select_qry = $select->execute()->fetchAll();
		$entity_id = $select_qry[0]->entity_id;
		expDebug::dPrint('on star rating userid 1111::sss '.$entity_type);
		if($entity_type == 'Class' ){
			$updateStmt = db_update('slt_course_class');
			$updateStmt->condition('id', $entity_id, '=');
			$updateStmt->fields(array('updated_on'    => now()));
			expDebug::dPrintDBAPI('QUERY DEFAULT123 sannn', $updateStmt);
			$updateStmt->execute();
		}else{
			$updateStmt = db_update('slt_program');
			$updateStmt->condition('id', $entity_id, '=');
			$updateStmt->fields(array('updated_on'    => now()));
			expDebug::dPrintDBAPI('QUERY DEFAULT123 ', $updateStmt);
			$updateStmt->execute();
		}
		syncSolrData('Course');
		syncSolrData('Class');
		syncSolrData('Program');
		
	  //insertUserPointsForEachActionsPerformed ($userId,'rating_class'); //Changed for #0068428
	  
		if($_POST['entityType']=='Class')
		insertUserPointsForEachActionsPerformed($userId,'rating_class',$entity_id,'insert',$_POST['entityType']);
		else {
			insertUserPointsForEachActionsPerformed($userId,'rating_class',$entity_id,'insert','tp');
		}
	  	if($isapi){
	  		$result= new stdClass();
			$result ->classId = $_REQUEST['classId'];
			$result ->average_rating = $avgRating['average']['value'];
			$result ->entity_type = $avgRating['average']['entity_type'];
			$result ->no_of_rating = $avgRating['count']['value'];
			$result ->vote_value = $avgRating['user']['value'];
		 	return $result;
		}else{
			return drupal_json_output(array_merge($avgRating,$avgRatingTxt));
		}
	  }
	
  }catch (Exception $ex) {
	watchdog_exception('setFiveStarRating', $ex);
	expertusErrorThrow($ex);
  }
}
//API for class registration
function classRegisterAPI($userId, $courseId, $classId){
  try{

  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrenrollment/exp_sp_launch.inc');
  $isAdminSide = 'N';
  $waitlist = 0;
  $response = false;
  $fromAPI=true;
  $regDate  = '';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $deliveryType = getClassDeliveryType($classId, 'Short');
  if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
    $waitlistNumber = waitListCheck($courseId, $classId);
    if($waitlistNumber > 0 ){
      $waitlist = 1;
    }
  }
  /***0081561 starts **/
  $isPriced=0;
  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_commerce');
  $commerceEnabled =  $select->execute()->fetchField();
  if($commerceEnabled==1){
  
  	$isPriced = function_exists('getPriceOfClass') ? (int)getPriceOfClass($courseId, $classId,'') : 0;
  }
  
  if ($isPriced > 0) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Priced training cannot be registered. Contact support team.";
  	//return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
  	return $errobj;
  }
  /***0081561 ends **/
  //Code Added to check equivalence class
  
  expDebug::dPrint(' $mobile_request123 = ' . print_r($_REQUEST['mobile_request'],true) , 4 );
  
  if($_REQUEST['Action'] == ""  && $_REQUEST['mobile_request']){
  
  	$equivalanceclass = checkEquivalence($userId, $courseId, $classId,$adminEnrollCall=false, $fromCart = false);
  
  	expDebug::dPrint(' $equivalanceclass1234 = ' . print_r($equivalanceclass,true) , 4 );
  
  	$equi_class = $equivalanceclass[0]->equv_title;
  
  expDebug::dPrint(' $equiclass1234 = ' . print_r($equi_class,true) , 4 );
  
  	if(count($equivalanceclass)>0){
  		
  		if($equivalanceclass[0]->equv_id != ""){
  		
  		$result->status = "equivalence";
  		$result->id = $equi_class;
  		return $result;
  		}else{
  			$result->status ="multiple_class";  
  			return $result;
  		}
  
  	}
  }
  
  $delivery_type = db_select('slt_course_class');
  $delivery_type->addExpression('delivery_type','delivery_type');
  $delivery_type->condition('course_id', $courseId, '=');
  $delivery_type->condition('id', $classId, '=');
  $DeliveryType = $delivery_type->execute()->fetchAll();
  expDebug::dPrintDBAPI("delivery type query  ",$delivery_type);
  expDebug::dPrint('delievery type query results ' . print_r($DeliveryType, true) , 4);
  
  if(($DeliveryType[0]->delivery_type == 'lrn_cls_dty_wbt')||($DeliveryType[0]->delivery_type == 'lrn_cls_dty_vod')){
  	$content_check = db_select('slt_course_class','scc');
  	$content_check->leftJoin('slt_course_content_mapper','scm','scm.class_id = scc.id');
  	$content_check->addExpression('scm.content_id','content_id');
  	$content_check->condition('scm.course_id', $courseId, '=');
  	$content_check->condition('scm.class_id', $classId, '=');
  	$ContentCount = $content_check->execute()->fetchAll();
  	expDebug::dPrintDBAPI("content query  ",$content_check);
  	expDebug::dPrint('content query results ' . print_r($ContentCount, true) , 4);
  
  	if(empty($ContentCount)){
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "There is no content attached to the class.";
  		return $errobj;
  	}
  }
  else{
  	$session_id = db_select('slt_course_class','scc');
  	$session_id->leftJoin('slt_course_class_session','scs','scs.class_id = scc.id AND scs.course_id = scc.course_id');
  	$session_id->addExpression('scs.id','session_id');
  	$session_id->condition('scs.course_id', $courseId, '=');
  	$session_id->condition('scs.class_id', $classId, '=');
  	$SessionCount = $session_id->execute()->fetchAll();
  	expDebug::dPrintDBAPI("session query  ",$session_id);
  	expDebug::dPrint('session query results ' . print_r($SessionCount, true) , 4);
  	if(empty($SessionCount)){
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "There are no sessions in the class.";
  		return $errobj;
  	}
  }
  //End
  
  
/*  $expired = GetIfExpired($courseId,$userId);
  // completion date check
  $PastcompDate = CheckCourseCompExpiry($courseId);
  if($expired || $PastcompDate){
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Course is expired";
  	return $errobj;
  }*/
  $checkIfDedicatedClass = is_dedicated($classId);
  if(!empty($checkIfDedicatedClass)) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Business rule set as dedicate class for training plan.";
  	return $errobj;
  }  else {
  $resp = register_user_to_course_class($userId, $courseId, $classId, $waitlist,$isAdminSide, $response,$fromAPI,$regDate);

  if($resp->status == 'Reserved' &&( $deliveryType == 'vod' || $deliveryType == 'wbt'  ))
  {
	  expDebug::dPrint('GETlaunchmodules');
	  $launchdetails = getLaunchDetail($resp->id);
	  expDebug::dPrint('$launchdetailslaunchdetailslaunchdetailslaunchdetails'.print_r($launchdetails,true),5);
	  $resp->launchdetails = $launchdetails;
  }


  expDebug::dPrint('$deliveryType$deliveryType'.$deliveryType,5);
   expDebug::dPrint('$resp$resp$resp$resp$resp'.print_r($resp,true),5);
   
  }
  return  $resp;
  }catch (Exception $ex) {
	watchdog_exception('classRegisterAPI', $ex);
	expertusErrorThrow($ex);
  }
}
//API for class Roster
function addRosterByAPI($paramsobj,$creatorId){
  try{
  $isAdminSide = 'N';
  $waitlist = 1;
  $response = false;
  $fromAPI=true;
  $paramsobj = trimInputFields($paramsobj);  // #42124 - Trim input values
  $userStr  = $paramsobj['Learnerid'];
  $courseId = $paramsobj['Courseid'];
  $classId  = $paramsobj['Classid'];
  $regDate  = $paramsobj['RegDate'];
  $isMandatory  = $paramsobj['Ismandatory'];
  $date = new DateTime($regDate);
  $regDate =  $date->format('Y-m-d');
  expDebug::dPrint(' regDate = ' .$regDate, 3);
  $aUserList = explode(',',$userStr);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $isPriced=0;
  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_commerce');
  $commerceEnabled =  $select->execute()->fetchField();
  if($commerceEnabled==1){
  
  $isPriced = function_exists('getPriceOfClass') ? (int)getPriceOfClass($courseId, $classId,'') : 0;
  }
  $validateRegDate = validateRegistrationDateOfILT($classId, $regDate);	//47622: Not able to mark a past class with a past date...
  /*-- 34887 : Disallow Priced registarion from API call --*/
  $checkIfDedicatedClass = is_dedicated($classId);
  //Added for #0069252
  $priuser = array();
  $nonpriuser = array();
  foreach($aUserList as $userId ){
  $select = db_select('slt_course_class', 'cls');
  $select->addField('cls','id','id');
  $select->condition('cls.id',$classId,'=');
  //filterByUserAccessPrivileges($select, 'cls',$userId,'','','Class');
  expDebug::dPrintDBAPI("Appended query-----> ",$select);
  $result = $select->execute()->fetchAll();
  if(!empty($result[0]->id)){
  	$priuser[] = $userId;
  }else{
  	$nonpriuser[] = $userId;
  }
  }
  expDebug::dPrint('$$Class id fetched for learner group segmentation' . print_r($result,true),5);
   $priuserList = implode(', ', $priuser);
  $nonpriuserList = implode(', ', $nonpriuser);
  
  if(empty($priuserList) || !empty($nonpriuserList)){
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "The given learner(s) ".$nonpriuserList." can't be enrolled as they don't have access to this training";
  	return $errobj;
  }
  
  $delivery_type = db_select('slt_course_class');
  $delivery_type->addExpression('delivery_type','delivery_type');
  $delivery_type->condition('course_id', $courseId, '=');
  $delivery_type->condition('id', $classId, '=');
  $DeliveryType = $delivery_type->execute()->fetchAll();
  expDebug::dPrintDBAPI("delivery type query  ",$delivery_type);
  expDebug::dPrint('delievery type query results ' . print_r($DeliveryType, true) , 4);
  
  if(($DeliveryType[0]->delivery_type == 'lrn_cls_dty_wbt')||($DeliveryType[0]->delivery_type == 'lrn_cls_dty_vod')){
  	$content_check = db_select('slt_course_class','scc');
  	$content_check->leftJoin('slt_course_content_mapper','scm','scm.class_id = scc.id');
  	$content_check->addExpression('scm.content_id','content_id');
  	$content_check->condition('scm.course_id', $courseId, '=');
  	$content_check->condition('scm.class_id', $classId, '=');
  	$ContentCount = $content_check->execute()->fetchAll();
  	expDebug::dPrintDBAPI("content query  ",$content_check);
  	expDebug::dPrint('content query results ' . print_r($ContentCount, true) , 4);
  
  	if(empty($ContentCount)){
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "There is no content attached to the class.";
  		return $errobj;
  	}
  }
  else{
  	$session_id = db_select('slt_course_class','scc');
  	$session_id->leftJoin('slt_course_class_session','scs','scs.class_id = scc.id AND scs.course_id = scc.course_id');
  	$session_id->addExpression('scs.id','session_id');
  	$session_id->condition('scs.course_id', $courseId, '=');
  	$session_id->condition('scs.class_id', $classId, '=');
  	$SessionCount = $session_id->execute()->fetchAll();
  	expDebug::dPrintDBAPI("session query  ",$session_id);
  	expDebug::dPrint('session query results ' . print_r($SessionCount, true) , 4);
  	if(empty($SessionCount)){
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "There are no sessions in the class.";
  		return $errobj;
  	}
  }
  
  if ($isPriced > 0) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Priced training cannot be registered. Contact support team.";
   //return array((object)array('status'=>'Priced training cannot be registered. Contact support team.'));
   return $errobj;
  }
  else if(!$validateRegDate){
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Incorrect date. Registration date should be the same or less than the session date.";
	return $errobj;
  }
  else if(!empty($checkIfDedicatedClass)) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Business rule set as dedicate class for training plan.";
  	return $errobj;
  }
  else {
  if(is_array($aUserList)){
    foreach($aUserList as $userId ){
    	$select = db_select('slt_person', 'per');
    	$select->addField("per", "id", "id");
    	$select->condition("per.id", $userId , "=");
    	$result_set = $select->execute()->fetchField();
    	expDebug::dPrint('$result_set'.print_r($result_set,true),5);
    	
    if(empty($result_set)){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid User ID";
    	return $errobj;
    	}
    	$expired = GetIfExpired($courseId,$userId);
    	// completion date check
 /*    	$select = db_select('slt_course_template', 'crs');
    	$select->addExpression('COUNT(1)', 'count');
    	$select->condition('crs.id', $courseId,'=');
    	$select->where('if(crs.complete_date is not null, crs.complete_date < CURDATE(),1=0)');
    	expDebug::dPrintDBAPI("completion date check >>> ",$select);
    	$invalidcompDate =$select->execute()->fetchField();
    if($expired || $invalidcompDate){ 
    		$errobj=new stdClass();
    		$errobj->isValidateError = 1;
    		$errobj->errcode = 'L_012';
    		$errobj->errormsg = "Course completion date is expired for the user.";
    		return $errobj;
    	} */
      if($userId){
        $resp[] = register_user_to_course_class($userId, $courseId, $classId, $waitlist,$isAdminSide, $response,$fromAPI,$regDate,'','','','',$isMandatory);
        //$resp[] = register_user_to_course_class($userId, $courseId, $classId, $waitlist,$isAdminSide, $response,$fromAPI,$regDate);
      }
    }
  }
  return  $resp;
  }
  }catch (Exception $ex) {
	watchdog_exception('addRosterByAPI', $ex);
	expertusErrorThrow($ex);
  }
}

//API for TP Roster
function addTPRosterByAPI($paramsobj,$creatorId){
  try{
  $paramsobj = trimInputFields($paramsobj);  // #42124 - Trim input values
  $userIdList  = $paramsobj['Learnerid'];
  $ProgramId = $paramsobj['ProgramId'];
  $ClassIds = $paramsobj['ClassIds'];
  $isMandatory  = $paramsobj['Ismandatory'];
  $isAdminSide = 'Y';
  $regDate = $paramsobj['RegDate'];
  $regDate = str_replace('/', '-', $regDate);

  $userIdList = preg_replace('/\s+/', '', $userIdList);
  $ClassIds = preg_replace('/\s+/', '', $ClassIds);

  $explodeAttrValue = explode(",",$userIdList);
  $uniqueUserIdValue = array_unique(explode(",",$userIdList));
  $userIdCount = count($uniqueUserIdValue);
  //Added for #0069252
  /*$priuser = array();
  $nonpriuser = array();
  foreach($uniqueUserIdValue as $userId ){
  $select = db_select('slt_program', 'prgm');
  $select->addField('prgm','id','id');
  $select->condition('prgm.id',$ProgramId,'=');
  //filterByUserAccessPrivileges($select, 'prgm',$userId,'','','Program');
  expDebug::dPrintDBAPI("Appended query-----> ",$select);
  $result = $select->execute()->fetchAll();
  expDebug::dPrint('$$Program id fetched for learner group segmentation' . print_r($result,true),5);
  //$registerStatus = lp_class_list_register($isAdminSide,$userIdList,'','');
   if(!empty($result[0]->id)){
  	$priuser[] = $userId;
  }else{
  	$nonpriuser[] = $userId;
  }
  }
  $priuserList = implode(', ', $priuser);
  $nonpriuserList = implode(', ', $nonpriuser);

  if(empty($priuserList) || !empty($nonpriuserList)){
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "The given learner(s) ".$nonpriuserList." can't be enrolled as they don't have access to this training";
  	return $errobj;
  }
  */
  $uniqueUserIdList = implode(',',$uniqueUserIdValue);
  foreach($uniqueUserIdValue as $k=>$v)
  {
  	$v = trim($v);
  	if(!($v > 0 && is_numeric($v)))
  	{
  		$errobj->field = 'Learnerid';
  		$errobj->isValidateError = 1;
  		$errobj->errormsg = 'Invalid Learner id';
  		return $errobj;
  	}
  }
  if($userIdCount > 0) {
  	$select = db_select('slt_person', 'sp');
  	$select->addField('sp','id');
  	$select->condition("sp.id",$uniqueUserIdValue,"IN");
  	$select->condition("sp.status","cre_usr_sts_atv","=");
  	$result = $select->execute()->fetchCol();
  	//expDebug::dPrintDBAPI('$select 123 :', $select);
  	if($userIdCount > count($result)){
  		$role_diff = array_diff($uniqueUserIdValue,$result);
  		$combined_role = implode(",",$role_diff);
  		$err_msg = "The given Learner id " . $combined_role." doesn't exists or inactive";
  		$errobj=new stdClass();
  		$errobj->field = 'Learnerid';
  		$errobj->isValidateError = 1;
  		$errobj->errormsg = $err_msg;
  		return $errobj;
  	}
  }
  //expDebug::dPrint('$ProgramId :'.$ProgramId, 5);
  if($ProgramId > 0 && is_numeric($ProgramId))
  {
  	$select = db_select("slt_program", "sp");
  	$select->addField("sp", "id");
  	$select->condition("sp.status", 'lrn_lpn_sts_atv', "=");
  	$select->condition("sp.id", $ProgramId, "=");
  	$result_set = $select->execute();
  	if(!($result_set->rowCount() > 0))
  	{
  		$errobj->field = 'ProgramId';
  		$errobj->isValidateError = 1;
  		$errobj->errormsg = 'Program id is invalid or inactive status';
  		return $errobj;
  	}
  }
  else
  {
  	$errobj->field = 'ProgramId';
  	$errobj->isValidateError = 1;
  	$errobj->errormsg = 'Invalid program id';
  	return $errobj;
  }
  if(!empty($ClassIds))
  {
  	$explodeAttrValue = explode(",",$ClassIds);
  	$uniqueClassIdValue = array_unique(explode(",",$ClassIds));
  	$classIdCount = count($uniqueClassIdValue);
  	foreach($uniqueClassIdValue as $k=>$v)
  	{
  		$v = trim($v);
  		if(!($v > 0 && is_numeric($v)))
  		{
  			$errobj->field = 'ClassIds';
  			$errobj->isValidateError = 1;
  			$errobj->errormsg = 'Invalid Class id';
  			return $errobj;
  		}
  	}
  	if($classIdCount > 0) {
  		$select = db_select('slt_course_class', 'scc');
  		$select->addField('scc','id');
  		$select->condition("scc.id",$uniqueClassIdValue,'IN');
  		$select->condition("scc.status","lrn_cls_sts_atv","=");
  		$result = $select->execute()->fetchCol();
  		if($classIdCount > count($result)){
  			$role_diff = array_diff($uniqueClassIdValue,$result);
  			$combined_role = implode(",",$role_diff);
  			$err_msg = "The given class id " . $combined_role." doesn't exists or inactive";
  			$errobj=new stdClass();
  			$errobj->field = 'ClassIds';
  			$errobj->isValidateError = 1;
  			$errobj->errormsg = $err_msg;
  			return $errobj;
  		}
  		$module_seq = getModuleId($userIdList,$ProgramId);
  		
  		if(count($module_seq)>0) {
  				
  			if($module_seq[0]->object_type=="cre_sys_obt_crt" && $module_seq[0]->overall_status=="lrn_tpm_ovr_exp")
  				$module_path = $module_seq[0]->recertify_path+1;
  			else
  				$module_path = $module_seq[0]->recertify_path;  				
  	}
  		else {
  			
  			$module_path = '1';
  }

  		$module_id = getModuleIdFromObjectId($ProgramId,$module_path);
  		
  		$select = db_select('slt_module_crs_mapping', 'map');
  		$select->innerJoin('slt_course_template','crs','crs.id=map.course_id');
  		$select->innerJoin('slt_course_class','cls','cls.course_id=crs.id');
  		$select->addExpression('group_concat(distinct(cls.course_id))', 'modcrsids'); 
		$select->condition('map.program_id', $ProgramId);
		$select->condition('map.module_id', $module_id[0]->module_id);
  		$select->condition('cls.status','lrn_cls_sts_atv','=');
   		$modcrsids = $select->execute()->fetchCol();
  		expDebug::dPrint("Module crsids::".print_r($modcrsids,1),5);
  		
  		$mod_crsids = explode(",",$modcrsids[0]);  
  		expDebug::dPrint('$$lp_singleclass_or_multiclass_cnt_for_course $$uniqueClassIdValue' . print_r($uniqueClassIdValue,true),5);
  		
  		$select2 = db_select('slt_course_class', 'cls');
  		$select2->addExpression('group_concat(distinct(cls.course_id))', 'selcrsids');
  		$select->condition('cls.status','lrn_cls_sts_atv','=');
  		$select->condition('cls.id',$uniqueClassIdValue,'IN');
  		$selcrsids = $select->execute()->fetchCol();
  		$sel_crsids = explode(",",$selcrsids[0]);
  		expDebug::dPrint("selected crsids::".print_r($sel_crsids,1),5);
 	
  		if((count($uniqueClassIdValue)!=count($mod_crsids))||(array_diff($mod_crsids, $sel_crsids)))
  		{ 
  			$err_msg = "Please check the entered class ids are available in the Program Module";
  			$errobj=new stdClass();
  			$errobj->field = 'ClassIds';
  			$errobj->isValidateError = 1;
  			$errobj->errormsg = $err_msg;
  			return $errobj;
  		}
  	}
  }

  if(!empty($regDate) && !validateDateFormat($regDate)){
  	$error -> errormsg = 'RegStatusDate Date is invalid';
  	$error -> isValidateError = true;
  	return $error;
  }
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $isPriced=0;
  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_commerce');
  $commerceEnabled =  $select->execute()->fetchField();
  if($commerceEnabled==1){
  $isPriced =  function_exists('getPriceOfClass') ? (int)getPriceOfClass($courseId, $classId, $ProgramId) : 0;
  }
  //expDebug::dPrint('$$isPriced' . $isPriced);
  /*-- 34887 : Disallow Priced registarion from API call --*/
  if ($isPriced > 0) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Priced training cannot be registered. Contact support team.";
  	return $errobj;
  } else {
  		/*-- 45670: Add TP roster api allowing to register even who have completed user --*/
  		$userList = explode(',', $userIdList);
  		$regUserList = array();
  		foreach($userList as $key => $value){
  			 $tempStatus = checkTPRegisteredStatus($userList[$key], $ProgramId);
  			 if ($tempStatus) { array_push($regUserList, $userList[$key]); }
  		}
  		if (count($regUserList)) {
  			$checkPrevRegStatus = true;
			// prepare the unregistered user from registered users.
  			$userIdList = array_diff($userList, $regUserList);
  			$userIdListString = implode(',', $userIdList);
  			$userRegString = implode(',', $regUserList);
  		} else {
  			$userIdListString = $userIdList;
  		}
  		$statusMessage = (object)array('status'=>'User '.$userRegString.' already registered to the Training Plan. User may be Inprogress / completed .');
  		if (empty($userIdList) && $checkPrevRegStatus) {
  			return array($statusMessage);
  		}
  		/*-- TP Registration block start --*/
  	//	$registerStatus  = lp_singleclass_or_multiclass_cnt_for_course($ProgramId, '', '', $isAdminSide, $userIdListString, '', false, true,$regDate);
  		$registerStatus  = lp_singleclass_or_multiclass_cnt_for_course($ProgramId, '', '', $isAdminSide, $userIdListString, '', false, true,$regDate,'',$isMandatory);
	  	if($registerStatus == 'MultiRegister'){
	    	$errCount = 0;
	    	$classIdArr = attachedCourseClassDetails($ProgramId);
	    	$courseIntersect = array_flip($classIdArr);
	    	$inputClassId = explode(',',$ClassIds);
	    	foreach ($inputClassId as $key=>$cls_id){
	      		$courseId = getCourseIdFromClassId($cls_id);
	      		if(!array_key_exists($courseId,$courseIntersect)){
	        		$errCount = 1;
	      		}
	      		unset($courseIntersect[$courseId]);
	      		if(!array_key_exists($cls_id,$classIdArr)){
	        		$errCount = 1;
	      		}
	    	}

	    	if($errCount || (count($courseIntersect) > 0)) {
	    		$errobj=new stdClass();
	    		$errobj->isValidateError = 1;
	    		$errobj->errcode = 'L_012';
	    		$errobj->errormsg = "Invalid Class List";
	    		return $errobj;
	    	}
	    	$registerStatus = lp_class_list_register($isAdminSide,$uniqueUserIdList,'','');
	  	}
	  	$registerStatus[0]->status = (strpos($registerStatus[0]->status, t('MSG809')) !== false) ? t('User').' '.$registerStatus[0]->status: $registerStatus[0]->status;
	  	if ($checkPrevRegStatus) {
	  		array_push($registerStatus, $statusMessage);
	  	}
	  	if($registerStatus[0]->status)
			return $registerStatus;
		else
			return array((object)array('status'=>$registerStatus));
		/*-- TP Registration block end --*/
	  }
  }catch (Exception $ex) {
	watchdog_exception('addTPRosterByAPI', $ex);
	expertusErrorThrow($ex);
  }
}

function checkTPRegisteredStatus($userId, $programId) {
	try {
		
		$module_seq = getModuleId($userId,$programId);
		
		if(count($module_seq)>0)  
			$module_path = $module_seq[0]->recertify_path;
	    else		
			$module_path = '1';
		
		$select = db_select('slt_master_enrollment','menr');
        $select->join('slt_person', 'per', 'per.id = menr.user_id');
        $select->addExpression('count(menr.id)', 'menrcount');
        $select->condition('per.id', $userId);
        $select->condition('menr.overall_status',array('lrn_tpm_ovr_cnf', 'lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_wtl','lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr'));
        $select->condition('menr.program_id',$programId,'=');
        $select->condition('menr.recertify_path',$module_path,'=');
        $encount = $select->execute()->fetchField();
        if ($encount) {
        	return $encount;
        } else {
        	return false;
        }
	} catch (Exception $ex) {
		watchdog_exception('attachedCourseClassDetails', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * _attachedCourseClassDetails() - To get the complete session details of all the course->class associated with the program id
 */
function attachedCourseClassDetails($programId){
  try {
    $query = db_select('slt_module_crs_mapping', 'crs_map');
    $query->leftjoin('slt_course_class', 'cls', 'crs_map.course_id = cls.course_id');
    $query->addField('cls','id','class_id');
    $query->addField('cls','course_id','course_id');
    $query->condition('crs_map.program_id', $programId);
    $query->condition('cls.status', 'lrn_cls_sts_atv');
    expDebug::dPrintDBAPI('$select Query ' , $query);
    return $query->execute()->fetchAllKeyed();
  } catch (Exception $ex) {
    watchdog_exception('attachedCourseClassDetails', $ex);
    expertusErrorThrow($ex);
  }
}









//API for TP Roster
function registerTPbyAPI($paramsobj,$creatorId){
   try{
  $paramsobj = trimInputFields($paramsobj);  // #42124 - Trim input values
   $userIdList  = $paramsobj['Learnerid'];
  $ProgramId = $paramsobj['ProgramId'];
  //$ClassIds = $paramsobj['ClassIds'];
  $RegType = $paramsobj['RegType'];
    $isAdminSide = 'Y';
  if($_REQUEST['recertifyid'])
  {
  	  $isAdminSide = 'N';
  }
  $regDate = $paramsobj['RegDate'];
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
/** 0080903 starts **/  
  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_commerce');
  $commerceEnabled =  $select->execute()->fetchField();
  if($commerceEnabled==1){
  	$courseId=0;
  	$classId=($paramsobj['ClassIds']!='') ? $paramsobj['ClassIds'] : 0;
  	$isPriced =  function_exists('getPriceOfClass') ? (int)getPriceOfClass($courseId, $classId, $ProgramId) : 0;
  	expDebug::dPrint('registerTPbyAPI $$isPriced' . $isPriced);
  	if ($isPriced > 0) {
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "Priced training cannot be registered. Contact support team.";
  		return $errobj;
  	}	
  }  
  /** 0080903 starts**/
  
  if(isset($_REQUEST['pageandmandatory'])){
			$val = explode ( '-', $_REQUEST['pageandmandatory'] );
			$fromPage = $val [0];
			$MasterMandatory = $val [1];
			$registerStatus  = lp_singleclass_or_multiclass_cnt_for_course($ProgramId, '', '', 'N', $userIdList, $_REQUEST['pageandmandatory'], false, true,$regDate);
			if($registerStatus == 'MultiRegister'){
			$registerStatus = lp_class_list_register ('N',$userIdList, $fromPage, $masterMandatory );
			}
			
			if ($registerStatus [0]->status)
				return $registerStatus;
				else
					return array (( object ) array ('status' => $registerStatus ) );
						
  			 
  }
  
  
  
  if($RegType != 'select_class')
  {
  $registerStatus  = lp_singleclass_or_multiclass_cnt_for_course($ProgramId, '', '', $isAdminSide, $userIdList, '', false, true,$regDate);
  if($registerStatus == 'MultiRegister'){
   expDebug::dPrint('$result$result$result$result$result'.print_r('qwertyuio',true),5);
    $errCount = 0;
    $classIdArr = attachedCourseClassDetailsAPI($ProgramId);
    for($i=0;$i<count($classIdArr);$i++){
	     expDebug::dPrint('$result$result$result$result$result'.print_r($classIdArr[$i]->class_id,true),4);
	     $enrolledid= getRegisteredOrNot($classIdArr[$i]->course_id,$classIdArr[$i]->class_id,$userIdList,'');
	    $classIdArr[$i]->enrolledid=$enrolledid;
    }

    expDebug::dPrint('$result$result$result$result$result'.print_r($classIdArr,true),5);
    expDebug::dPrint('$result$result$result$result$result'.print_r($tp_courses,true),5);
    $tp_courses = lp_fetch_module_list($ProgramId, $userIdList,'N','',1);
    return array((object)array('class_list'=>$classIdArr,'status'=>$registerStatus,'tp_courses'=> $tp_courses));
    }
   }else{

    $registerStatus = lp_class_list_register($isAdminSide,$userIdList,'','');



   }
    /*
    $courseIntersect = array_flip($classIdArr);
    $inputClassId = explode(',',$ClassIds);
    expDebug::dPrint('$result$result$result$result$result'.print_r($inputClassId,true),5);
    foreach ($inputClassId as $key=>$cls_id){
      $courseId = getCourseIdFromClassId($cls_id);
      if(!array_key_exists($courseId,$courseIntersect)){
        $errCount = 1;
      }
      unset($courseIntersect[$courseId]);
      if(!array_key_exists($cls_id,$classIdArr)){
        $errCount = 1;
      }
    }
    if($errCount || (count($courseIntersect) > 0)) {
      return array((object)array('status'=>'Invalid Class List'));
    }
    $registerStatus = lp_class_list_register($isAdminSide,$userIdList,'','');
    */

  if($registerStatus[0]->status)
	return $registerStatus;
  else
	return array((object)array('status'=>$registerStatus));
  
  
  }catch (Exception $ex) {
		watchdog_exception('registerTPbyAPI', $ex);
	expertusErrorThrow($ex);
  }
}




/*
 * _attachedCourseClassDetails() - To get the complete session details of all the course->class associated with the program id
 */
function attachedCourseClassDetailsAPI($programId,$userid=''){
  try {
    $query = db_select('slt_module_crs_mapping', 'crs_map');
    $query->leftjoin('slt_course_class', 'cls', 'crs_map.course_id = cls.course_id');
    if($userid!='')
    {
    	filterByUserAccessPrivileges($query, 'cls',$userid,'','cre_sys_obt_cls','Class');
    }
     $query->leftjoin('slt_course_template', 'crs', 'crs_map.course_id = crs.id');
    $query->addField('crs','title','course_title');
    $query->addField('cls','id','class_id');
    $query->addField('cls','course_id','course_id');
    $query->addField('cls','title','class_title');
    $query->addField('cls','code','class_code');
    $query->addField('cls','delivery_type','delivery_type');
    $query->addField('cls','short_description','description');
    $query->condition('crs_map.program_id', $programId);
    $query->condition('cls.status', 'lrn_cls_sts_atv');
    if(isset($_REQUEST['mobile_request'])){
    $query->distinct();
    }
    expDebug::dPrintDBAPI('$select Query123212 ' , $query);
    return $query->execute()->fetchAll();
  } catch (Exception $ex) {
    watchdog_exception('attachedCourseClassDetails', $ex);
    expertusErrorThrow($ex);
  }
}



/*
 * To get the list of user, expire in waitlist
 */
/*function getWaitlistExpiredForNotification() {
  try{
	  // Added by Vincent on Nov 01, 2013 for #0028593
	  $timeZone = date_default_timezone(false);
	  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	  $selectStmt = db_select('slt_enrollment', 'enr');
	  $selectStmt->innerJoin('slt_course_class_session','sess','sess.class_id = enr.class_id and sess.id = (select id from slt_course_class_session where class_id = sess.class_id order by start_date limit 1)');
	  $selectStmt->addField('enr','user_id','user_id');
	  $selectStmt->addField('enr','id','enroll_id');
	  $selectStmt->addField('enr','course_id','course_id');
	  $selectStmt->addField('enr','class_id','class_id');
	  $selectStmt->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl');
	  // Added by Vincent on Nov 01, 2013 for #0028593
	  $selectStmt->addField('sess','timezone','timezone');
	  $selectStmt->addExpression("CONCAT( DATE_FORMAT(sess.start_date, _utf8'%Y-%m-%d'),' ',sess.end_time,':00')", 'session_end_datetime');
	  $selectStmt->groupBy("enr.id having session_end_datetime < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= sess.timezone)),NOW())");
	  $result     = $selectStmt->execute();
	  $results    = $result->fetchAll();
	  return $results;
  }catch (Exception $ex) {
	watchdog_exception('getWaitlistExpiredForNotification', $ex);
  if (isset($_GET['cron_key'])) {
		throw new Exception($ex);
	}
	expertusErrorThrow($ex);
  }
}*/

/**
 *  Automatic waitlist cancellation Refund Amound
 *	0034090: Feedback -- Cancellation of waitlisted seats for priced class
 */
function automaticRefundForWaitlistCancel($enrId){
  $txn = db_transaction();
  try{
		$refund_flag = 0;
		module_load_include('inc', 'exp_sp_administration_order', 'exp_sp_administration_order');
		$select = db_select('slt_enrollment', 'enr');
		$select->leftjoin('slt_order', 'ord','enr.order_id = ord.id');
		$select->leftjoin('slt_node_learning_activity', 'nodelearact','enr.class_id = nodelearact.entity_id AND nodelearact.entity_type = \'cre_sys_obt_cls\' AND order_type = \'credit\'');
		$select->leftjoin('slt_course_class', 'crscls','enr.class_id = crscls.id AND enr.course_id = crscls.course_id AND crscls.price > 0');
		$select->addField('ord','uc_order_id');
		$select->addField('nodelearact','node_id');
		$select->addField('crscls','price');
		$select->condition('enr.id', $enrId, '=');
		expDebug::dPrintDBAPI('$select--->', $select);
		$waitlistDetails = $select->execute()->fetch();
		$ucOrderId = $waitlistDetails->uc_order_id;
		$price = $waitlistDetails->price;
		$nodeId = $waitlistDetails->node_id;
		expDebug::dPrint('$ucOrderId--->'.$ucOrderId);
		expDebug::dPrint('$nodeId--->'.$nodeId);
		//expDebug::dPrint('$waitlistDetails--->', print_r($waitlistDetails),5);
		//condition need here
		if(!empty($price) && $price > 0 && !empty($ucOrderId) && !empty($nodeId))
		{
			dropPolicyListItem($ucOrderId,$nodeId,$enrId);
			$refund_flag = 1;
		}
		return $refund_flag;
	}catch (Exception $ex) {
    $txn->rollback();
	  unset($txn);
	  watchdog_exception('automaticCancelWaitlistUpdate', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
  unset($txn);
}
/**
 *  Automatic waitlist cancellation update
 */
function automaticCancelWaitlistUpdate($enrId) {
  $txn = db_transaction();
  try{
  $updateCart = db_update('slt_enrollment');
  $updateCart->fields(array(
                      'waitlist_flag' => null,
  					  'waitlist_priority' => null,
  					  'reg_status' => 'lrn_crs_reg_can',
  					  'updated_on' => now(),
                      ));
  $updateCart->condition('id', $enrId, '=');
  $updateCart->execute();
  }catch (Exception $ex) {
    $txn->rollback();
	  unset($txn);
	  watchdog_exception('automaticCancelWaitlistUpdate', $ex);
	  if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
  syncSolrData('Enrollment');
  unset($txn);
}

/**
 * Conflict check for the instructor to not register the same class in which he/she conducts
 */
function getClassInstructorConflict($userId, $classId){
  try{
  $select = db_select('slt_session_instructor_details', 'sess');
  $select->addExpression('COUNT(sess.id)', 'count');
  $select->condition('sess.class_id', $classId, '=');
  $select->condition('sess.instructor_id', $userId, '=');
  expDebug::dPrintDBAPI(' SQL = ' , $select);
  $result = $select->execute()->fetchField();
  return ($result > 0)? false : true;
  }catch (Exception $ex) {
  watchdog_exception('getClassInstructorConflict', $ex);
  expertusErrorThrow($ex);
  }
}


/**
 * Conflict check for the instructor to not register the same class in which he/she conducts
 */
function getSessionandCompletionConflict($userId, $classId){
	try{
		
		$usertimezone = getUserPHPTimezoneName($userId);
		
		$select = db_select('slt_course_class_session', 'ses');
		$select->innerjoin('slt_course_template', 'crs','ses.course_id = crs.id');
		$select->innerjoin('slt_profile_list_items', 'prf','prf.code = ses.timezone');
		$select->addField('crs','complete_days');
		$select->addExpression('DATE_FORMAT(CONVERT_TZ(ses.start_date,prf.attr2,"'.$usertimezone[0]->zone_name.'"),"%Y-%m-%d")', 'convertedtimezone');
		$select->condition('crs.compliance_completed', 'hire_days', '=');
		$select->condition('ses.class_id', $classId, '=');
		$select->orderBy('ses.start_date','desc');
		$select->range(0, 1);
		expDebug::dPrintDBAPI(' SQL getSessionandCompletionConflict = ' , $select);
		$result = $select->execute()->fetchAll();
		if(count($result)>0){
		$userDetails 		= getPersonDetails($userId, array('hire_date'));	
			$userhiredate		= $userDetails['hire_date'];
		$userhiredate=	date('Y-m-d', strtotime($userhiredate. ' + '.$result[0]->complete_days .'days'));	
		if(($result[0]->convertedtimezone >= $userhiredate) && !empty($userhiredate))
				return true;
		}
		return false;
	}catch (Exception $ex) {
		watchdog_exception('getClassInstructorConflict', $ex);
		expertusErrorThrow($ex);
	}
}


/*
 * getEnrollmentDetails() - Function to fetch slt_enrollment table fields
 */
function getEnrollmentDetails($enrollmentId, $fieldList = array()) {
	try{
    if (empty($fieldList)) {
      return null;
    }
    $select = db_select('slt_enrollment', 'enr');
    $select->condition('enr.id', $enrollmentId, '=');
    // Add fields
    foreach ($fieldList as $field) {
      $select->addField('enr', $field, $field);
    }
    expDebug::dPrintDBAPI('MY ESIGN INFO ' , $select);
    $details  = $select->execute()->fetch();
    expDebug::dPrint(' $result = ' . print_r($details, true) , 3);
    return $details;
	}catch (Exception $ex) {
		watchdog_exception('getEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
  	}
}

/*
 * getMasterEnrollmentDetails() - Function to fetch slt_master_enrollment table fields
 */
function getMasterEnrollmentDetails($enrollmentid, $fieldList = array()) {
	try{
    if (empty($fieldList)) {
      return null;
    }
    $select = db_select('slt_master_enrollment', 'msenr');
    $select->condition('msenr.id', $enrollmentid, '=');
    // Add fields
    foreach ($fieldList as $field) {
      $select->addField('msenr', $field, $field);
    }
    $details  = $select->execute()->fetch();
//    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    return $details;
	}catch (Exception $ex) {
		watchdog_exception('getMasterEnrollmentDetails', $ex);
		expertusErrorThrow($ex);
  	}
}

function getRatingDetails($nodeId){
	try{
		  $selectStmt = db_select('votingapi_vote', 'vote');
		  $selectStmt->addExpression('COUNT(vote.value)', 'cnt_value');
		  $selectStmt->addField('vote','value', 'rating_value');
		  $selectStmt->condition('vote.entity_id', $nodeId);
		  $selectStmt->groupBy('vote.value');
		  $retResult = $selectStmt->execute()->fetchAll();
      $ratingDetails = array();
	  	$inc = 0;
  	  $passParams = "[";
	  foreach ($retResult as $record) {
			 $inc=$inc+1;
			 $passParams .= "{";
			 $passParams .= "'cnt_value':'".$record->cnt_value."',";
			 $passParams .= "'rating_value':'".$record->rating_value."'";
			 $passParams .= "}";

			 if($inc < count($retResult)) {
				 $passParams .= ",";
			 }
	   }
     $passParams .= "]";
     expDebug::dPrint('getRatingDetails()============= : = ' . print_r($retResult, true) , 3);
     return $passParams ;
	}catch (Exception $ex) {
		watchdog_exception('getRatingDetails', $ex);
		expertusErrorThrow($ex);
  	}
}

/* Function to check whether the password
 * given for the esignature is valid or not
 * param - one - password - returns true or false
 * */
function checkUserNameEsign($uname,$pass){
  try{
	global $user;
	unset($_SESSION['esign_validate']);
	if(module_exists('ldap_authentication') && $user->uid != 1)
	{
		$auth_conf = ldap_authentication_get_valid_conf();
	  	$authenticationMode=$auth_conf->authenticationMode;
	  	$const_var=LDAP_AUTHENTICATION_EXCLUSIVE;
	  	$const_mixed_mode=LDAP_AUTHENTICATION_MIXED;
		$ldap_auth = array();
		$ldap_auth['values']['name'] = $uname;
		$ldap_auth['values']['pass'] = $pass;
	  	if($authenticationMode == $const_var){
			    $validate_user = validateLdapPassword($auth_conf,$ldap_auth);
			    if($validate_user->uid){
			        $_SESSION['esign_validate'] = 1;
			    	return true;
			    }else{
			    	return false;
			    }
	  	}elseif($authenticationMode == $const_mixed_mode){
	  		require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
			$account = user_load($user->uid,TRUE);
		    $current_pass_failed = user_check_password($pass, $account);
		    if($current_pass_failed == false){
			    $validate_user = validateLdapPassword($auth_conf,$ldap_auth);
			    if($validate_user->uid){
			        $_SESSION['esign_validate'] = 1;
			    	return true;
			    }else{
			    	return false;
			    }
		    }else{
		        if($current_pass_failed){
	               $_SESSION['esign_validate'] = 1;
	            }
		    	 return $current_pass_failed;
		    }
	  	}
	}else{
		require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
		$account = user_load($user->uid,TRUE);
	    $current_pass_failed = user_check_password($pass, $account);
	    if($current_pass_failed){
	      $_SESSION['esign_validate'] = 1;
	    }

	    return $current_pass_failed;
	}
  }catch (Exception $ex) {
		watchdog_exception('checkUserNameEsign', $ex);
		expertusErrorThrow($ex);
  }
}

function validateLdapPassword($auth_conf,$ldap_auth){
	try{
	 $name = $ldap_auth['values']['name'];
  	 $pass = $ldap_auth['values']['pass'];
	foreach ($auth_conf->servers as $sid => $ldap_server) {
		 $ldap_server->connect();
		 $ldap_server->bind();
		 $ldap_user = $ldap_server->user_lookup($name);
		 $allow = $auth_conf->allowUser($name, $ldap_user);
		 $credentials_pass = FALSE;
      	 $credentials_pass = ($ldap_server->bind($ldap_user['dn'], $pass) == LDAP_SUCCESS);
		 if(!$credentials_pass){
		 	expDebug::dPrint('fail learning inc value for credential pass****** ');
		 	return false;
		 }else{
		 	expDebug::dPrint('else learning inc value for credential pass****** ');
		 	global $user;
		  	return $user;
		 }
	}
	}catch (Exception $ex) {
		watchdog_exception('validateLdapPassword', $ex);
		expertusErrorThrow($ex);
  	}
}

/* VERY VERY IMPORTANT FUNCTION -- DO NOT REMOVE THIS AT ANY COST!!!
 * used for validating esignature for all regions of the site.
 * Created for future use.
 * Patches of code will be added for each region where esignature is implemented.
 *
 */

function isActiveEsignRegion($regName){
	try{
		return true;
	}catch (Exception $ex) {
		watchdog_exception('isActiveEsignRegion', $ex);
		expertusErrorThrow($ex);
  }
}

/* Implemetation of esignature fieldset.
 * Common to all modules - my profile, account etc, that use drupal forms API.
 * param - 3 - form, form state and callback. Callback is different for each module.
 * output - fieldset form element with textboxes and button for login
 * */
function exp_sp_my_esign_fieldset(&$form, &$form_state,$callback) {
	try {
	global $user;
	$form['next'] = array(
		  '#type' => 'button',
		  '#value' => t('LBL141'),
      	  '#suffix' => getSuffix('action'),
  		  '#attributes' => array('tabindex' => '11'),
		  '#ajax' => array(
    		'wrapper' => 'wizard-form-wrapper',
    		'callback' => $callback,
    		'method' => 'replace',
    		'effect' => 'fade',
        	'event'=>'click',
      ),
	);


   $form['hidden_fieldset_val'] = array('#type' => 'hidden',
  	'#attributes' => array('id' => 'hidden_fieldset_val'),
	'#default_value' => '0',
   );
   $form['esign'] = array(
  	  '#type' => 'fieldset',
	  '#title' => t('E-Signature'),
  	  '#attributes' => array('class' => array('esign-fieldset-class')),
	  '#collapsible' => FALSE,
	  '#collapsed' => FALSE,
  	  '#states' => array(
      	'visible' => array(
        ':input[id="hidden_fieldset_val"]' => array('value' => "1"), //checkboxes
      	),
      ),
	);

    $form['esign']['name'] = array('#type' => 'textfield',
		'#id' => 'esign-uname',
	    '#title' => t('Username'),
	    '#maxlength' => USERNAME_MAX_LENGTH,
	    '#size' => 15,
	    '#attributes' => array('tabindex' => '1','disabled'=>array('true'),'class'=>array('readonly-text')),
	    '#default_value' => $user->name,
  	);
  	$form['esign']['pass'] = array('#type' => 'password',
	    '#title' => t('Password'),
  		'#id' => 'esign-pass',
  		'#prefix' => '<br><br>',
	    '#maxlength' => 60,
	    '#size' => 15,
	    '#attributes' => array('tabindex' => '2'),
  	);

  	$form['esign']['ebutton'] = array('#type' => 'submit',
	  	'#attributes' => array('tabindex' => '3'),
	    '#value' => t('E-Sign'),
  		'#prefix' => '<br><br>',
  		'#ajax' => array(
	        'wrapper' => 'wizard-form-wrapper',
	        'callback' => $callback,
	        'event'=>'click',
	      ),
  	);

  	return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_my_esign_fieldset', $ex);
		expertusErrorThrow($ex);
  	}
}

/*
 * render_videoHTML_in_ctools_modal() - Callback to load the video HTML in ctools modal dialog.
 *                                      Shared by exp_sp_lnrenrollment and exp_sp_lnrlearningplan.
 */
function render_videoHTML_in_ctools_modal($js, $title, $contentSubTypeCode, $path, $launchedFrom, $courseId, $classId, $lessonId, $versionId, $enrollId, $previousStatus, $suspendData = '') {
	try {
		if($js) {
			if(user_is_logged_in() == 1)
				exp_ctools_modal_display_video($title, $contentSubTypeCode, $path, $launchedFrom, $courseId, $classId, $lessonId, $versionId, $enrollId, $previousStatus, $suspendData);
			else
				die(t("ERR055"));
		} else {
			throw new Exception('exp_sp_learning.inc: render_videoHTML_in_ctools_modal() : nojs');
		}
	} catch(Exception $ex) {
		watchdog_exception('render_videoHTML_in_ctools_modal', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * exp_ctools_modal_display_video() - Prepares the video HTML and renders it.
 *                                    Shared by exp_sp_lnrenrollment and exp_sp_lnrlearningplan.
 *                                    Called by render_videoHTML_in_ctools_modal() defined above.
 */
function exp_ctools_modal_display_video($title, $contentSubTypeCode, $path, $launchedFrom, $courseId, $classId, $lessonId, $versionId, $enrollId, $previousStatus = '', $suspendData = '') {
	try {
		global $user;
		if($user->uid) {
			enableCtool();
		}
		$contentPlayerActive = contentPlayerIsActive();
		$suspendData = stripslashes($suspendData);
		expDebug::dPrint('exp_ctools_modal_display_video args ' . print_r(func_get_args(), true), 5);
		$htmlId = 'vod_' . $enrollId . '_' . $lessonId;
		// changes for Video completion based on watched progress
		// video_session_id, vod_progress_update_frequency and contentProgress are passed to all theme rendering of video players
		$config = getConfig("exp_sp");
		// vod_progress_update_frequency is set to 1 min (60 seconds) by default if exp_sp.ini varaible is not set
		$vod_progress_update_frequency = isset($config['vod_progress_update_frequency']) ? $config['vod_progress_update_frequency'] : 10;
		$enc1 = new Encrypt();
		$video_session_id = $enc1->encrypt(implode('_', array($courseId,$classId,$lessonId,$versionId,$enrollId)));
		// $_SESSION[$video_session_id]['progress'] = $contentProgress;
		// $_SESSION['suspendData'][$video_session_id] = json_encode($suspendData);

		
		$pathArr = explode('()|()', urldecode($path));
		expDebug::dPrint(' $commands 2222222222222222222222222' . print_r($pathArr, true), 4);
		
		$h5p_cont_id = null;
		$type = null;
		if($contentSubTypeCode == 1)
			$h5p_cont_id = $pathArr[3];
		else if($contentSubTypeCode == 2)
			$h5p_cont_id = $pathArr[5];
			
			$h5pUrl = implode("/", explode('()|()', urldecode($path)));
		expDebug::dPrint("h5p url:". $base_url . $h5pUrl);
		if(strpos($h5pUrl,"h5p") > 0)
		{
			$commands = array();
			$type = "h5p";
			/*$video_source = 'mp4';
			global $base_url;
			$output = theme('videojs-player', array(
							'html_id' => $htmlId,
							'video_session_id' => $video_session_id,
							'video_url' => $base_url . '/' . $h5pUrl,
							'video_source' => $video_source,
							'type' => $type,
							'enrollId'=>$enrollId,
						));
			*/
			if($launchedFrom != "PRE") {
				$commands[] = ajax_command_update_vod_score_on_modal_close($launchedFrom, $courseId, $classId, $lessonId, $versionId, $enrollId, $previousStatus, $video_session_id,$type,$h5p_cont_id);
			}
				
			$commands[] = ctools_modal_command_display($title, $output);
			//$commands[] = ctools_modal_show_loader($video_session_id, 0,0);
			expDebug::dPrint(' $commands suresh = ' . print_r($commands, true), 4);
		
			print ajax_render($commands);
			exit();
			
		}
		
		
		
		$contentProgress = 0;
		if($suspendData != '') {
			$_SESSION[$video_session_id] = json_decode(rawurldecode(rawurldecode($suspendData)), true);
			$contentProgress = (isset($_SESSION[$video_session_id]['progress']) && $_SESSION[$video_session_id]['progress'] != 100) ? $_SESSION[$video_session_id]['progress'] : 0;
			$suspendData = $_SESSION[$video_session_id];
		} else {
			$suspendData = array();
		}
		// $_SESSION[$video_session_id]['progress'] = $contentProgress != '' ? $contentProgress : 0;
		expDebug::dPrint('session on launch video $video_session_id'. $video_session_id, 2);
		expDebug::dPrint('session on launch video date ' . date('H:i:s'). print_r($_SESSION[$video_session_id], 1), 2);
		//expDebug::dPrint(' raw $title = ' . print_r($title, true), 4);
		$titleArr = explode('()|()', urldecode($title));
		$title = implode('/', $titleArr);
		//expDebug::dPrint(' $title = ' . print_r($title, true), 4);
		
		$contenthosttype = urldecode($contentSubTypeCode);
		include_once (drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
		$cdn_status = getDrupalModuleStatus(array('cloud_files'));
// 		$contentSubTypeCode = 1 for uploaded videos; 2 for url (youtube and vimeo); 3 for cdn hosted videos
		if($cdn_status == 1 && $contenthosttype != '2') {
			$contentSubTypeCode = 3;
		} else {
			$contentSubTypeCode = $contenthosttype;
		}
		
		//$pathArr = explode('()|()', urldecode($path));
		$explodeChar = ($contentPlayerActive) ? '/' : '()|()';
		$pathArr = explode($explodeChar, urldecode($path));
		expDebug::dPrint(' $pathArr = ' . print_r($pathArr, true), 4);
		
		$output = '';
		$player = 'videojs';
		if($contentSubTypeCode == '1') {
			$uploadDir = '';
			$pathArrLen = count($pathArr);
			for($i = 0; $i <= ($pathArrLen - 3); $i ++) {
				if($pathArr[$i] != '') {
					$uploadDir .= ($uploadDir == '') ? $pathArr[$i] : '/' . $pathArr[$i];
				}
			}
			
			// Changes for filename to be picked from inside the directory in contentupload
			// Earlier the player was taking in the filename to be the same as the folder name
			// Code in the admin content add/edit has been changed to make the foldername unique
			// The file name (with one of the following extensions) is randomly picked from the extracted folder
			//expDebug::dPrint(' video details --> ' . print_r($pathArr, true), 4);
			$videoFoldername = $pathArr[$pathArrLen - 2];
			
			$uploadDirectory = './sites/default/files/contentupload/' . $videoFoldername;
			expDebug::dPrint('$uploadDirectory '.print_r($uploadDirectory, 1), 5);
			$videoBasename = null;
			if($handle = opendir($uploadDirectory)) {
			    expDebug::dPrint('$uploadDirectory can be opened '.var_export($handle, 1), 5);
				while(false !== ($file = readdir($handle))) {
					$lowerCaseFileName = strtolower($file);
					if($file != '.' && $file != '..' && substr($lowerCaseFileName, - strlen('.mp4')) === '.mp4' && (strpos($lowerCaseFileName, 'mobile_') === false || strpos($lowerCaseFileName, 'mobile_') !== 0)) {
						$videoBasename = substr($file, 0, strrpos($file, '.') + 0);
						break;
					}
				}
				closedir($handle);
			}
			
			//expDebug::dPrint(' calling theme with $htmlId = ' . print_r($htmlId, true), 4);
			//expDebug::dPrint(' video session id = ' . print_r($video_session_id, 1) . 'with progress = ' . $contentProgress, 4);
			//expDebug::dPrint(' calling theme with $uploadDir = ' . print_r($uploadDir, true), 4);
			//expDebug::dPrint(' calling theme with $videoBasename = ' . $videoFoldername . "---->" . $videoBasename, 3);
			
			if(empty($videoBasename)) {
				throw new Exception('Missing video file');
			}
			
				$red5_server_host_entry = $config['red5_host_entry'] . ':5080';
				$conversion_format = array('256');
				
				include_once (drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info.inc');
				include_once (drupal_get_path('module', 'exp_sp_administration_content') . '/exp_sp_administration_content.inc');
				$module_name = array('exp_sp_stream_server');
				$stream_server_status = getDrupalModuleStatus($module_name);
			expDebug::dPrint('$stream_server_status = ' . print_r($stream_server_status, true), 4);
				
				if($stream_server_status == 1) {
					$stream_server = true;
				} else {
					$stream_server = false;
				}
				
				if($stream_server && (urlExists($red5_server_host_entry) && (streamingOrNot($uploadDirectory, $videoBasename, $conversion_format))))
				{
					$config=getConfig("exp_sp");
					$content_upload_path = $config['content_upload_path'].'/contentupload';
					$red5_server_host_entry = $config['red5_host_entry'];
					
					/*end user bandwidth detect start here */
					$kb = 512;
					expDebug::dPrint("streaming $kb Kb...<!-");
					flush();
					$time = explode(" ", microtime());
					$start = $time[0] + $time[1];
					for($x = 0; $x < $kb; $x++){
					    flush();
					}
					$time = explode(" ", microtime());
					$finish = $time[0] + $time[1];
					$deltat = $finish - $start;
					$bandwidth =  round($kb / $deltat, 0);
					$bandwidth =  round($bandwidth/1024, 0);
					expDebug::dPrint("end user bandwidth = $bandwidth");
					/*end user bandwidth detect end here*/
					
					/*file available check start here here*/
					
					
					$file_900 = $content_upload_path . '/' . $video_foldername . '/900/' . $video_basename.'.mp4';
					$file_512 = $content_upload_path . '/' . $video_foldername . '/512/' . $video_basename.'.mp4';
					$file_256 = $content_upload_path . '/' . $video_foldername . '/256/' . $video_basename.'.mp4';
					
					$file_900_logfile = $content_upload_path . '/' . $video_foldername . '/900/ffmpeg_log.txt';
					$file_512_logfile = $content_upload_path . '/' . $video_foldername . '/512/ffmpeg_log.txt';
					$file_256_logfile = $content_upload_path . '/' . $video_foldername . '/256/ffmpeg_log.txt';
					/*file available check end here*/
					
					
					/*choose a single file with bandwidth detection start here*/
					if(($bandwidth > 22000) && file_exists($file_256)  && file_exists($file_256_logfile)) {
					    $bit_rate = 256;
					} else if(($bandwidth > 19000) && file_exists($file_512) && file_exists($file_512_logfile)) {
					    $bit_rate = 512;
					}else if(($bandwidth < 19000) && file_exists($file_900) && file_exists($file_900_logfile)) {
					    $bit_rate = 900;
					}else {
					    $bit_rate = 256;
					}
					
					/*load balancing ip detect  start here*/
					$index_file_content = file_get_contents('http://'.$red5_server_host_entry.':5080/oflaDemo/');
					$find_string = 'rtmp://';
					$pos = strpos($index_file_content, $find_string);
					$ip_string = substr($index_file_content, $pos+7, 30);
					$ip_array = explode('/', $ip_string);
					$red5_server_host_entry = $ip_array[0];
					//"rtmp://$red5_server_host_entry/oflaDemo""$video_foldername . '/'.$bit_rate.'/' .$video_basename.mp4"
					$video_source = 'rtmp';
					$output = theme('videojs-player', array(
					        'html_id' => $htmlId,
					        'video_session_id' => $video_session_id,
					        'video_url' => "rtmp://$red5_server_host_entry/oflaDemo/$videoFoldername/$bit_rate/$videoBasename.mp4",
							'video_source' => $video_source
					));
				} else {
				$video_source = 'mp4';
				global $base_url;
				$output = theme('videojs-player', array(
							'html_id' => $htmlId,
							'video_session_id' => $video_session_id,
							'video_url' => $base_url . '/' . $uploadDir . '/' . $videoFoldername . '/' . $videoBasename . ".mp4",
							'video_source' => $video_source,
							'type' => ''
						));
			}
			
		} elseif($contentSubTypeCode == '3') {
			$video_source = 'mp4';
			$videoFileName = '';
			$pathArrLen = count($pathArr);
			for($i = 0; $i <= ($pathArrLen - 1); $i ++) {
				if($pathArr[$i] != '') {
					$videoFileName .= ($videoFileName == '') ? $pathArr[$i] : '/' . $pathArr[$i];
				}
			}
			$videoFileName = str_replace('sites/default/files', '', $videoFileName);
			
			//expDebug::dPrint(' CDN video name --> ' . $videoFileName, 4);
			$cdnpathUrl = (string)getCDNURL($videoFileName);
// 			expDebug::dPrint(' CDN $cdnpathUrl --> ' . $cdnpathUrl, 4);
			if($cdnpathUrl == '') {
				$cdnpath = implode('/', $pathArr);
// 				expDebug::dPrint(' $pathArr --> ' . print_r($pathArr, 1), 4);
// 				expDebug::dPrint(' $cdnpath --> ' . $cdnpath, 4);
				global $base_url;
				$video_config = array(
								'html_id' => $htmlId,
								'video_session_id' => $video_session_id,
								'video_url' => $base_url . $cdnpath,
								'video_source' => $video_source
							);
			} else {
				$video_config = array(
								'html_id' => $htmlId,
								'video_session_id' => $video_session_id,
								'video_url' => $cdnpathUrl,
								'video_source' => $video_source
							);
			}
			$output = theme('videojs-player', $video_config);
		} else {
			$video_url = implode('/', $pathArr);
			expDebug::dPrint('content type = youtube/vimeo; url = ' . $video_url, 4);
			if(strpos($video_url, 'vimeo.com') !== false) {
				$video_source = $title = 'vimeo';
			} elseif(strpos($video_url, 'youtu.be') !== false || strpos($video_url, 'youtube.com') !== false ) {
				$video_source = $title = 'youtube';
			} else {
				$video_source = $title = 'mp4';
			}
			
			$output = theme('videojs-player', array(
												'html_id' => $htmlId,
												'video_session_id' => $video_session_id,
												'video_url' => $video_url,
												'video_source' => $video_source
							));
		}
		
		$commands = array();
		// If the form has not yet been rendered, render it.
		if($launchedFrom != "PRE") {
			$commands[] = ajax_command_update_vod_score_on_modal_close($launchedFrom, $courseId, $classId, $lessonId, $versionId, $enrollId, $previousStatus, $video_session_id);
		}
		//calculate play from position
		if(is_array($suspendData)) {
			$suspend_data = $suspendData;
		}else {
			$suspend_data = json_decode(rawurldecode($suspendData));
		}
		$video_duration = isset($suspend_data['video_duration']) ? $suspend_data['video_duration'] : 0;
		if($video_duration != 0) {
			$play_from = round(($video_duration * $contentProgress)/100, 0);
		}
		else {
			$play_from = 0;
		}
		if ($contentPlayerActive) {
			return array(
						'output' => $output,
						'params' => array(
								'video_session_id' => $video_session_id,
								'play_from' => $play_from,
								'vod_progress_update_frequency' => $vod_progress_update_frequency,
						));
		}
		$commands[] = ctools_modal_command_display($title, $output);
		$commands[] = ctools_modal_show_loader($video_session_id, $play_from, $vod_progress_update_frequency,$launchedFrom);
		//expDebug::dPrint(' $commands = ' . print_r($commands, true), 4);
		
		print ajax_render($commands);
		exit();
	} catch(Exception $ex) {
		watchdog_exception('exp_ctools_modal_display_video', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * Function to get initialize with the CDN server
*/
function initializeCDN(){
	try {
		$tmpPath = $_SERVER["DOCUMENT_ROOT"].'/tmp';
		expDebug::dPrint(' InitializeCDN $tmpdir ->' . print_r($tmpPath, true) , 4);

		require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';
		expDebug::dPrint(' getCDNURL = ' . print_r($path, true) , 4);

		$rackspace_cloud_auth_url = variable_get('rackspace_cloud_auth_url');
		$rackspace_cloud_username = variable_get('rackspace_cloud_username');
		$rackspace_cloud_api_key = variable_get('rackspace_cloud_api_key');
			
		expDebug::dPrint(' InitializeCDN- $rackspace_cloud_api_key->' . print_r($rackspace_cloud_api_key, true) , 5);
		expDebug::dPrint(' InitializeCDN - $rackspace_cloud_username->' . print_r($rackspace_cloud_username, true) , 5);
		expDebug::dPrint(' InitializeCDN - $rackspace_cloud_auth_url->' . print_r($rackspace_cloud_auth_url, true) , 5);

		$client = new OpenCloud\Rackspace($rackspace_cloud_auth_url, array(
				'username' => $rackspace_cloud_username,
				'apiKey' => $rackspace_cloud_api_key,
		));

		$cacheFile = $tmpPath.'/.opencloud_token';
		expDebug::dPrint(' InitializeCDN - $cacheFile->' .$cacheFile, 5);

		// If the cache file exists, try importing it into the client
		if (file_exists($cacheFile)) {
			$data = unserialize(file_get_contents($cacheFile));
			$client->importCredentials($data);
		}

		$token = $client->getTokenObject();
		expDebug::dPrint(' InitializeCDN - $token->' .print_r($token,true), 5);

		// If no token exists, or the current token is expired, re-authenticate and save the new token to disk
		if (!$token || ($token && $token->hasExpired())) {
			$client->authenticate();
			file_put_contents($cacheFile, serialize($client->exportCredentials()));
		}

		return $client;
	}catch (Exception $ex) {
		expDebug::dPrint(' initializeCDN Exception->' . print_r($ex, true) , 5);
		watchdog_exception('initializeCDN Exception ->', $ex);
	}
}


function getCDNUrlAPI() {
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	expDebug::dPrint(' APIiiiiiii = ' .$_REQUEST["content_url"]);
	$path = getCDNURL($_REQUEST["content_url"]);
	expDebug::dPrint(' APIiiiiiii outputt = ' .$path);

	return array((object)array('play_url'=>$path));
	// 	return $result->play_url = $path;;
}


/*
 * Function to get CDN content URL
*/
function getCDNURL($path) {
	try {
		require_once 'sites/all/libraries/php-opencloud/vendor/autoload.php';
		expDebug::dPrint(' getCDNURL path = ' . print_r($path, true) , 5);

		$rackspace_cloud_region = variable_get('rackspace_cloud_region');
		$rackspace_cloud_container = variable_get('rackspace_cloud_container');
			
		expDebug::dPrint(' getCDNURL - $rackspace_cloud_container->' . print_r($rackspace_cloud_container, true) , 5);
		expDebug::dPrint(' getCDNURL - $rackspace_cloud_region->' . print_r($rackspace_cloud_region, true) , 5);
			
		$client = initializeCDN();
		//$client->authenticate();
		$object_store_service = $client->objectStoreService('cloudFiles', $rackspace_cloud_region);
		$container = $object_store_service->getContainer($rackspace_cloud_container);
		$account = $object_store_service->getAccount();
		$account->setTempUrlSecret('my_secret');
		$object = $container->getObject($path);
			
		expDebug::dPrint(' getCDNURL object = ' . print_r($object, true) , 5);
		$cdnurl = $object->getTemporaryUrl(600, 'GET')."&inline"; /* Temp URL will get expire in 10 min */
		//$cdnurl = $object->getTemporaryUrl(43200, 'GET')."&inline"; //Temp URL will get expire in 12 hrs
			
		// $cdnurl = $object->getPublicUrl();
		expDebug::dPrint(' getCDNURL - $cdnurl->' . print_r($cdnurl,1) , 5);
		return $cdnurl;


	}catch (Exception $ex) {
		expDebug::dPrint(' getCDNURL Exception->' . print_r($ex, true) , 5);
		watchdog_exception('getCDNURL Exception ->', $ex);
		return "";
	}
}

/*
 * ctools_modal_show_loader() - Creates a loader in ctools modal.
 */
function ctools_modal_show_loader($video_session_id, $play_from, $vod_progress_update_frequency,$launchedFrom) {
	try {
		expDebug::dPrint('video launch video session id ' . $video_session_id, 4);
		expDebug::dPrint('video launch video play from seconds ' . $play_from, 4);
		expDebug::dPrint('video launch video update for seconds ' . $vod_progress_update_frequency, 4);
		$enc1 = new Encrypt();
		$video_session_id_decrpt = $enc1->decrypt($video_session_id);
		$video_session_id_decrpt_arr = explode('_', $video_session_id_decrpt);
		expDebug::dPrint('video session id arr ----->' . print_r($video_session_id_decrpt_arr,1), 4);
  return array (
    'command' => 'ctoolsModalCreateLoader',
		  	'video_id' => $video_session_id,
		  	'play_from' => $play_from,
	  		'update_frequency' => $vod_progress_update_frequency,
  			'video_detail_arr' => $video_session_id_decrpt_arr,
  			'launchedFrom' => $launchedFrom
  );
	}catch (Exception $ex) {
		watchdog_exception('ctools_modal_show_loader', $ex);
		expertusErrorThrow($ex);
  	}
}
/*
 * ajax_command_update_vod_score_on_modal_close() - This is run when video is launched to trigger an update of vod score on modal close.
 *                                                  Called by exp_ctools_modal_display_video() defined above.
 */
function ajax_command_update_vod_score_on_modal_close($launchedFrom, $courseId, $classId, $lessonId,$versionId, $enrollId,  $previousStatus, $video_session_id,$type = null,$h5p_cnt_id = null) {
	try {
  return array (
    'command' => 'updateVODScoreOnCtoolsModalClose',
    'launched_from' => $launchedFrom,
    'course_id' => $courseId,
    'class_id' => $classId,
    'lesson_id' => $lessonId,
    'version_id' => $versionId,
    'enroll_id' => $enrollId,
    'prev_content_status' => $previousStatus,
    'video_session_id' => $video_session_id,
    'type'=>$type,
  	'h5p_cnt_id' => $h5p_cnt_id,
  );
	}catch (Exception $ex) {
		watchdog_exception('ajax_command_update_vod_score_on_modal_close', $ex);
		expertusErrorThrow($ex);
  	}
}

function catalogNoAccessHtml($block_html_id,$classes){
	try{
	  global $user;
		 $back_button = '';
	  // Change the back link path
	  if(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback']==TRUE){
	  	$linkPath = checkWidgetUrl('learning/catalog-search');
	  	$back_button= '<div class="back-btn-container">'.l('['.t("LBL212").']', $linkPath,array('attributes' => array('class' => 'detail-back-button','title' => t("LBL212")))).
	                    				'</div>';
	  }
		$output = '';
		$output .= '<div class="content">';
		$output .= '<div class="">';
		$output .= '<div class = "no-access">';
		if($user->uid) {
			$output .= t('LBL1156');
		} else {
		$strings = preg_split ('/ /', t('MSG027'), 3);
		$second=array_pop($strings);
		$signin=implode(" ", $strings);
		$msg =	str_replace($signin, '<div class="cls-sign-msg">'.t('LBL1156').'<a  class="use-ajax ctools-modal-ctools-login-style" id="signin-to-continue" href="/?q=ctools_ajax_sample/ajax/user_login"> '.(t('Sign In')).' </a>', t('MSG027'));
		$output .= ''.$msg.'';
		$output .= '</div>';
		}
		
		$output .= '</div>';
		$output .= '</div></div>';
	
		return $output;
	}catch (Exception $ex) {
		watchdog_exception('catalogNoAccessHtml', $ex);
		expertusErrorThrow($ex);
  	}
}

function checkPastDateOrNot($classId,$deliveryType){
	try {
    $pastSessions = 0;
    expDebug::dPrint('func get past '.print_r(func_get_args(), 1));
	  if ($deliveryType == 'lrn_cls_dty_ilt' || $deliveryType == 'lrn_cls_dty_vcl') {
	  	// Get date, start_time, end_time for all the sessions of this class. For VC also get PHP timezone.
	  	$select = db_select('slt_course_class_session', 'ses');
	  	$select->condition('ses.class_id', $classId, '=');
	  	$select->addExpression('CONCAT(date(ses.start_date), \' \', ses.start_time, \':00\')', 'start_date_time');
	  	$select->addExpression('CONCAT(date(ses.start_date), \' \', ses.end_time, \':00\')', 'end_date_time');
	    $select->leftjoin('slt_profile_list_items', 'splitz', 'ses.timezone = splitz.code AND ses.timezone IS NOT NULL  AND splitz.lang_code = \'cre_sys_lng_eng\'');
	    $select->addField('splitz', 'attr2', 'phptimezone');
	  	expDebug::dPrintDBAPI('$select', $select);
	  	$sessionsList = $select->execute()->fetchAll();
	  	expDebug::dPrint('$sessionsList = ' . print_r($sessionsList, true) , 3);
	  	expDebug::dPrint('$session->phptimezone = ' . print_r($session->phptimezone, true) , 4);
      $pastSessions = 0;
      foreach ($sessionsList as $session) {
        if ($deliveryType == 'lrn_cls_dty_ilt' && empty($session->phptimezone)) {
          $curDateTimeUX = time();
	        $sesStartDateTimeUX = strtotime($session->start_date_time);
	        expDebug::dPrint('$sesStartDateTimeUX = ' . $sesStartDateTimeUX, 4);
	        if ($sesStartDateTimeUX <= $curDateTimeUX) {
	        	$pastSessions += 1;
	        }
        }
        else{
	        // Get current datetime in the VC & ILTtimezone
	      	$sessionTZ = new DateTimeZone($session->phptimezone);
          $curDateTimeInSessionTZ = new DateTime(null, $sessionTZ);
          expDebug::dPrint('$curDateTimeInSessionTZ = ' . $curDateTimeInSessionTZ->format('Y-m-d H:i:sP'), 4);

	        // Convert session start datetime and end datetime to DateTime objects for comparing
          $sesStartDateTimeInSessionTZ = new DateTime($session->start_date_time, $sessionTZ);
	        // Compare the dates saving results in the return object
	        expDebug::dPrint('$curDateTimeInSessionTZ 2 = ' . $sesStartDateTimeInSessionTZ->format('Y-m-d H:i:sP'), 4);
	        if ($sesStartDateTimeInSessionTZ <= $curDateTimeInSessionTZ) {
	          $pastSessions += 1;
	        }
        }

      }
	  }
	  return $pastSessions;
	}catch (Exception $ex) {
		watchdog_exception('checkPastDateOrNot', $ex);
		expertusErrorThrow($ex);
  	}
}

function mostActiveUsersUpdate(){
  global $theme_key;
  if($theme_key == 'expertusoneV2'){
    $userLimit = variable_get('exp_sp_most_active_users_block_no_of_display', 5);
  }else{
    $userLimit = variable_get('exp_sp_most_active_users_block_no_of_display', 0);
  }
  try{
  db_query("truncate table slt_most_active_users");
  db_query("insert into slt_most_active_users
         SELECT  u.uid AS uid,p.full_name AS full_name, f.uri AS uri, pli.name AS job_title,e.cnt AS count_user, CONCAT(p.first_name, SPACE(1), SUBSTR(p.last_name, 1, 1)) AS user_name
		FROM slt_person p
		INNER JOIN users u ON p.user_name = u.name
          LEFT OUTER JOIN file_managed f ON u.picture = f.fid
          LEFT OUTER JOIN slt_profile_list_items pli ON p.job_title = pli.code
		INNER JOIN (
		SELECT en.user_id, COUNT(1) cnt
		FROM slt_enrollment en
		GROUP BY en.user_id ORDER BY cnt DESC
		) e ON e.user_id = p.id
          WHERE  (p.status = 'cre_usr_sts_atv')
          ORDER BY count_user DESC
          LIMIT ".$userLimit." OFFSET 0");
  }catch (Exception $e) {
	  watchdog_exception('mostActiveUsersUpdate', $e);
	  expertusErrorThrow($e);
  }
  unset($transaction);
}

function mostPopularTrainingUpdate(){
  try{
  db_query("truncate table slt_most_popular_training");
  db_query("insert into slt_most_popular_training
          SELECT sct.id AS id, sct.title AS title, sct.short_description AS short_description, snl.node_id AS node_id, sct.code AS code, 
          snl.entity_type AS entity_type, enr.cnt AS enr_count
          FROM
          slt_course_template sct
          INNER JOIN slt_node_learning_activity snl ON sct.id = snl.entity_id
          AND snl.entity_type = 'cre_sys_obt_crs'
          INNER JOIN (
			SELECT e.course_id, COUNT(1) cnt
			FROM slt_enrollment e
			WHERE e.reg_status IN ('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf')
			GROUP BY e.course_id
			ORDER BY cnt DESC ) enr ON sct.id = enr.course_id
          WHERE  (sct.status = 'lrn_crs_sts_atv')
          AND (sct.is_blended = 0)
          AND (sct.id not in( select entity_id from slt_group_mapping where entity_type = 'cre_sys_obt_crs' and group_type = 0) OR
               sct.id not in( select entity_id from slt_user_access_mapping where entity_type = 'cre_sys_obt_crs' and access_type = 0))
          GROUP BY enr.course_id
          UNION ALL SELECT sp.id AS id, sp.title AS title, sp.short_desc AS short_description, snl.node_id AS node_id, sp.code AS code, 
          snl.entity_type AS entity_type,enr.cnt AS enr_count
          FROM
          slt_program sp
          INNER JOIN slt_node_learning_activity snl ON sp.id = snl.entity_id
          AND snl.entity_type IN ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn')
          INNER JOIN ( SELECT e.program_id,COUNT(1) cnt
			FROM  slt_master_enrollment e
			WHERE e.overall_status IN ('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_exp','lrn_tpm_ovr_wtl','lrn_tpm_ovr_inc')
			GROUP BY e.program_id ORDER BY cnt DESC) enr ON sp.id = enr.program_id
          WHERE  (sp.status = 'lrn_lpn_sts_atv')
          AND (sp.id not in( select entity_id from slt_group_mapping where entity_type IN ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn') and group_type = 0 ) OR
          		 sp.id not in( select entity_id from slt_user_access_mapping where entity_type IN ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn') and access_type = 0 ))
          GROUP BY enr.program_id
          ORDER BY enr_count DESC
          LIMIT 20 OFFSET 0");
  }catch (Exception $e) {
	  watchdog_exception('mostPopularTrainingUpdate', $e);
	  expertusErrorThrow($e);
  }
  unset($transaction);
}

function highlyRatedTrainingUpdate(){
  try{
    db_query("truncate table slt_highly_rated_training");
    db_query("insert into slt_highly_rated_training
            SELECT
    		IF((vote.entity_type = 'Class'),nod_1.node_id,vote.entity_id) AS node_id,
    		AVG(CASE WHEN vote.function = 'average' THEN vote.value END) AS vote_value,
    		IF((vote.entity_type = 'Class'),'cre_sys_obt_crs',vote.entity_type) AS entity_type,
    		IF((vote.entity_type = 'Class'),crs.id,prm.id) AS id,
    		IF((vote.entity_type = 'Class'),crs.title,prm.title) AS title,
    		IF((vote.entity_type = 'Class'),crs.short_description,prm.short_desc) AS short_description,
    		IF((vote.entity_type = 'Class'),crs.code,prm.code) AS code,
    		MAX(CASE WHEN vote.function = 'count' THEN vote.value END) AS vote_count,
    		IF((vote.entity_type = 'Class'), FROM_UNIXTIME(vote.timestamp),'%Y-%c-%d %H:%i:%S') AS created_on
			FROM
			votingapi_cache vote
			LEFT OUTER
			JOIN slt_node_learning_activity nod ON nod.node_id = vote.entity_id
			LEFT OUTER
			JOIN slt_course_class cls ON cls.id = nod.entity_id AND nod.entity_type='cre_sys_obt_cls'
			LEFT OUTER
			JOIN slt_course_template crs ON crs.id = cls.course_id 
			LEFT OUTER
			JOIN slt_node_learning_activity nod_1 ON nod_1.entity_id = crs.id AND nod_1.entity_type='cre_sys_obt_crs'
			LEFT OUTER
			JOIN slt_program prm ON prm.id = nod.entity_id AND vote.entity_type IN ('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn') 
			WHERE
			(crs.status = 'lrn_crs_sts_atv') AND (vote.entity_type = 'Class') AND (vote.function IN('average','count'))
			GROUP BY cls.course_id
			ORDER BY vote_value DESC
			LIMIT 20 OFFSET 0");
  }catch (Exception $e) {
	  watchdog_exception('highlyRatedTrainingUpdate', $e);
	  expertusErrorThrow($e);
  }
  unset($transaction);
}

/*
 * updateClassAsCompleted() - selects the list of classes that need to be marked complete and updates their status to delivered or completed
 * used as a job in hook_cron..do not use for usual query selection.
 */
/*function updateClassAsCompleted(){
	//list of all classes that need to be marked complete
		// Added by Vincent on Nov 01, 2013 for #0028593
	try {
	  $timeZone = date_default_timezone(false);
	  $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
	  $selectCls = db_select('slt_course_class','cls');
	  $selectCls->leftjoin('slt_course_class_session', 'ses','cls.id=ses.class_id and ses.id = (select id from slt_course_class_session where class_id = ses.class_id order by start_date desc limit 1)');
	  $selectCls->leftjoin('slt_profile_list_items', 'splitz', 'ses.timezone = splitz.code  AND ses.timezone IS NOT NULL  AND splitz.lang_code = \'cre_sys_lng_eng\'');
	  $selectCls->addField('splitz', 'attr2', 'phptimezone');
	  $selectCls->addField('cls','id','class_id');
	  $selectCls->addExpression("CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'), ' ', ses.end_time, ':00')", 'end_date_time');
	 // Added by Vincent on Nov 01, 2013 for #0028593
	  $selectCls->addField('ses','timezone','timezone');
	  $selectCls->condition('cls.status','lrn_cls_sts_atv','=');
	  $selectCls->condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN');
	  $selectCls->groupBy("ses.class_id having end_date_time < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW())");
	  expDebug::dPrintDBAPI('$select updateClassAsCompleted', $selectCls);
	  $sessionsList = $selectCls->execute()->fetchAll();
	  expDebug::dPrint('$sessionsList updateClassAsCompleted = ' . print_r($sessionsList, true) , 3);
    $classIds = '';
	  foreach ($sessionsList as $session) {
        if (empty($session->phptimezone)) {
          $curDateTimeUX = time();
	        $sesEndtDateTimeUX = strtotime($session->end_date_time);
	        expDebug::dPrint('$sesStartDateTimeUX = ' . $sesStartDateTimeUX, 4);
	        if ($sesEndtDateTimeUX <= $curDateTimeUX) {
	        	$classIds .= ($classIds ? ',' : '').$session->class_id;
	        }
        }
        else{
	        // Get current datetime in the VC & ILTtimezone
	      	$sessionTZ = new DateTimeZone($session->phptimezone);
          $curDateTimeInSessionTZ = new DateTime(null, $sessionTZ);
          expDebug::dPrint('$curDateTimeInSessionTZ = ' . $curDateTimeInSessionTZ->format('Y-m-d H:i:sP'), 4);
	        // Convert session start datetime and end datetime to DateTime objects for comparing
          $sesEndDateTimeInSessionTZ = new DateTime($session->end_date_time, $sessionTZ);
	        // Compare the dates saving results in the return object
	        if ($sesEndDateTimeInSessionTZ <= $curDateTimeInSessionTZ) {
	          $classIds .= ($classIds ? ',' : '').$session->class_id;
	        }
        }
    }
    expDebug::dPrint('$classlist = ' . print_r($classIds, true) , 4);
    if($classIds){
      ///update all classes in the array to completed
      $txn = db_transaction();
  	  try{
    	  $updateClassSts = db_update('slt_course_class');
      	$updateClassSts->condition('id',explode(',',$classIds) ,'IN');
      	$updateClassSts->fields(array(
                          'status' => 'lrn_cls_sts_dld',
                          ));
        $updateClassSts->execute();
      }catch (Exception $ex) {
        $txn->rollback();
  	    unset($txn);
  	    watchdog_exception('updateClassAsCompleted', $ex);
	      if (isset($_GET['cron_key'])) {
					throw new Exception($ex);
				}
  	    expertusErrorThrow($ex);
    	}
    	unset($txn);
	  }
	}catch (Exception $ex) {
      watchdog_exception('updateClassAsCompleted Main function', $ex);
			if (isset($_GET['cron_key'])) {
				throw new Exception($ex);
			}
      expertusErrorThrow($ex);
   }
}*/

/**
 *  Automatic expired compliance courses to in-complete tab
 */
/*function autoComplianceIncompleteUpdate() {
 // $txn = db_transaction();
  try{
  $select = db_select('slt_course_template','crs');
  $select->innerJoin('slt_enrollment','enr','enr.course_id = crs.id and enr.is_compliance=1');
  $select->addField('crs','complete_date', 'complete_date');
  $select->addField('crs','complete_days', 'complete_days');
  $select->addField('enr','id', 'enrollid');
  $select->addField('enr','user_id', 'user_id');
  $select->addField('enr','course_id', 'course_id');
  $select->addField('enr','class_id', 'class_id');
  $select->addField('enr','reg_date', 'reg_date');
  $select->condition('crs.is_compliance', 1,'=');
  $select->condition('enr.comp_status',array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
  $select->where("if(( ifnull(crs.complete_days,'') != '') or ( ifnull(crs.complete_date,'') != ''),
        (CURDATE() >= (CASE WHEN (crs.complete_days IS NOT NULL AND crs.complete_days != '')
        THEN
        (date_format(date_add(enr.reg_date,interval crs.complete_days+1 day),'%Y-%m-%d'))
        WHEN (ifnull(crs.complete_date,'') != '')
        THEN
        (date_format(date_add(crs.complete_date,interval crs.complete_date+1 day),'%Y-%m-%d'))
        END)),1=0)");
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  $result =  $select->execute()->fetchAll();
  for($i=0;$i<count($result);$i++){
       $enrolledId = $result[$i]->enrollid;
       $completeDays = $result[$i]->complete_days;
       $completeDate = ($result[$i]->complete_date)  ? date("Y-m-d",strtotime($result[$i]->complete_date)) : '';
       $regStatusDate = date("Y-m-d",strtotime($result[$i]->reg_date));
       expDebug::dPrint('regstatusdate new = ' . print_r($regStatusDate, true) , 4);
        $currentDate = date("Y-m-d");// current date
        $dateExpiry = '';
        if($completeDays!=""){
          if($completeDays > 1) {
             $expiryDate = strtotime(date("Y-m-d", strtotime($regStatusDate)) . "+".$completeDays."days"."+1 day");
             $dateExpiry = date("Y-m-d", $expiryDate);
          } else {
            $expiryDate = strtotime(date("Y-m-d", strtotime($regStatusDate)) . "+".$completeDays."day"."+1 day");
             $dateExpiry = date("Y-m-d", $expiryDate);
          }
        }elseif($completeDate!=""){
          $expiryDate = strtotime(date("Y-m-d", strtotime($completeDate)) . "+ 1 day");
          $dateExpiry = date("Y-m-d", $expiryDate);

        }
       expDebug::dPrint('Current new date new = ' . print_r($currentDate, true) , 4 );
       expDebug::dPrint('$expiryDate new = ' . print_r($dateExpiry, true), 4);
       expDebug::dPrint('$$completeDate new = ' . print_r($completeDate, true) , 4);

       $courseId      = $result[$i]->course_id;
       $classId       = $result[$i]->class_id;
       $userId        = $result[$i]->user_id;
       $updateUserId  = 1;
       if($dateExpiry !=''){
         if($currentDate >= $dateExpiry){
           $select = db_select('slt_enrollment');
           $select->addExpression('count(1)','cnt');
           $select->condition('id', $enrolledId, '=');
           $select->condition('comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
           $inCompleteCount =  $select->execute()->fetchField();
           if($inCompleteCount){
             $updateStmt = db_update('slt_enrollment');
             // Set conditions
             $updateStmt->condition('id', $enrolledId, '='); //
             $updateStmt->condition('comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
             $updateStmt->fields(array('comp_status' => 'lrn_crs_cmp_inc','updated_by' => 1,'updated_on' => now()));
             $incomplete_updated = $updateStmt->execute();
             // compliance_class_incomplete
             insertNotification('ComplianceClassIncomplete', $courseId, $classId, $userId, $updateUserId);
           }
         }
       }
    }
  }catch (Exception $ex) {
  //    $txn->rollback();
	//  unset($txn);
	  watchdog_exception('automaticComplianceExpirytoIncomplete', $ex);
  	if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
 // unset($txn);
}*/

/**
 *  Compliance/Mandatory reminder notification for completed due date/ day
 */
/*function autoComplianceMandatoryCompletionDaysReminder($field){
  try{
  	$currentDate = date("Y-m-d");
  	if($field == 'is_compliance'){
  		$val = 1;
  	}else{
  		$val = 'Y';
  	}
	  $select = db_select('slt_enrollment','enr');
	  $select->innerJoin('slt_course_class','cls','cls.id = enr.class_id');
	  $select->innerJoin('slt_course_template','crs','crs.id = cls.course_id');
	  $select->innerJoin('slt_notification_info','notify','notify.notification_code=\'compliance_class_remind_session\'');
	 $select->addExpression('MOD(DATEDIFF(curdate(),IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),(DATE_ADD(enr.reg_date, interval notify.notify_before day)),IF(crs.complete_date IS NOT NULL,DATE_SUB(crs.complete_date,interval notify.notify_before day),
DATE_SUB(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),interval notify.notify_before day)))),notify.frequency_data_value)','resultcnt');
	  $select->addField('enr','course_id', 'course_id');
	  $select->addField('enr','class_id', 'class_id');
	  $select->addField('enr','user_id', 'user_id');
	  $select->condition('enr.'.$field, $val,'=');
	  $select->condition('cls.delivery_type', array('lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN');
	  $select->condition('enr.comp_status',array('lrn_crs_cmp_inp','lrn_crs_cmp_enr'),'IN');
	 // $select->where("curdate() > DATE_FORMAT(DATE_ADD(enr.reg_date, interval notify.notify_before day),'%Y-%m-%d')");
	 $select->where("curdate() >= IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),DATE_FORMAT(DATE_ADD(enr.reg_date, interval notify.notify_before day),'%Y-%m-%d'),
 IF(crs.complete_date IS NOT NULL,DATE_FORMAT(DATE_SUB(crs.complete_date,interval notify.notify_before day),'%Y-%m-%d'),
DATE_FORMAT(DATE_SUB(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),interval notify.notify_before day),'%Y-%m-%d')))");

	  $select->where("MOD(DATEDIFF(curdate(),IF((crs.complete_date IS NULL AND crs.complete_days IS NULL),(DATE_ADD(enr.reg_date, interval notify.notify_before day)),IF(crs.complete_date IS NOT NULL,DATE_SUB(crs.complete_date,interval notify.notify_before day),
DATE_SUB(DATE_ADD(enr.reg_date, INTERVAL crs.complete_days DAY),interval notify.notify_before day)))),notify.frequency_data_value) = 0");
	  //$select->where("MOD(DATEDIFF(curdate(),(DATE_ADD(enr.reg_date, interval notify.notify_before day))),notify.frequency_data_value) = 0");

	  $result =  $select->execute()->fetchAll();
	  expDebug::dPrintDBAPI(' Select Query for Compliance reminder notification = ' , $select);
	  expDebug::dPrint("Select Result for Compliance reminder notification ".print_r($result,true),4);
		for($i=0;$i<count($result);$i++){
			$courseId      = $result[$i]->course_id;
			$classId       = $result[$i]->class_id;
			$userId        = $result[$i]->user_id;//getIdOfLoggedInUser();
			$count 				 = $result[$i]->resultcnt;
			$updateUserId  = 1;
		  if($count == 0){
		  	if($field == 'is_compliance'){
		  		$mandatoryCompliance = 'ComplianceClassReminder';
		  	}else{
		  		$mandatoryCompliance = 'MandatoryClassReminder';
		  	}
		  	insertNotification($mandatoryCompliance, $courseId, $classId, $userId, $updateUserId);
		  }
		}
  }catch (Exception $ex) {
	  watchdog_exception('autoComplianceMandatoryCompletionDaysReminder', $ex);
 		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
	  expertusErrorThrow($ex);
  }
}*/

/**
 *  Mandatory reminder notification for completed due date/ day (TP)
 */
/*function autoMandatoryCompletionDaysReminderForTP(){
	try{
		$currentDate = date("Y-m-d");
		$select = db_select('slt_master_enrollment','enr');
		$select->innerJoin('slt_notification_info','notify','notify.notification_code=\'mandatory_class_remind_session\'');
		$select->addExpression('MOD(DATEDIFF(curdate(),(DATE_ADD(enr.reg_date, interval notify.notify_before day))),notify.frequency_data_value)','resultcnt');
		$select->addField('enr','program_id', 'program_id');
		$select->addField('enr','user_id', 'user_id');
		$select->condition('enr.mandatory', 1,'=');
		$select->condition('enr.overall_status',array('lrn_tpm_ovr_inp','lrn_tpm_ovr_enr'),'IN');
		$select->where("curdate() >= DATE_FORMAT(DATE_ADD(enr.reg_date, interval notify.notify_before day),'%Y-%m-%d')");
		$select->where("MOD(DATEDIFF(curdate(),(DATE_ADD(enr.reg_date, interval notify.notify_before day))),notify.frequency_data_value) = 0");
		$result =  $select->execute()->fetchAll();
		expDebug::dPrintDBAPI(' Select Query for Mandatory For TP reminder notification = ' , $select);
		expDebug::dPrint("Select Result for Mandatory For TPreminder notification ".print_r($result,true),4);
		for($i=0;$i<count($result);$i++){
			$program_id      = $result[$i]->program_id;
			$userId        = $result[$i]->user_id;//getIdOfLoggedInUser();
			$count 				 = $result[$i]->resultcnt;
			$updateUserId  = 1;
			if($count == 0){
				insertNotification('MandatoryTPReminder', $program_id, null, $userId, $updateUserId);
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('autoMandatoryCompletionDaysReminderForTP', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}*/

/**
 *  Compliance reminder notification for Expiry due date/ day
 */
/*function autoComplianceExpiryDaysReminder(){
	try{
		$select = db_select('slt_enrollment','enr');
		$select->leftJoin('slt_course_template','crs','crs.id=enr.course_id');
		$select->innerJoin('slt_notification_info','notify','notify.notification_code=\'compliance_expiry_remainder\'');
		$select->addField('enr','course_id', 'course_id');
		$select->addField('enr','class_id', 'class_id');
		$select->addField('enr','user_id', 'user_id');
		$select->addField('enr','id', 'enrollid');
		$select->condition('enr.is_compliance', 1,'=');
		$select->condition('enr.cmpl_expired','1','!=');
		$select->condition('enr.comp_status','lrn_crs_cmp_cmp','=');
		$select->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
				->condition('crs.validity_days','','IS NOT NULL'));
	// 	$select->where("if(crs.validity_days IS NOT NULL,(curdate() > DATE_FORMAT(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day),
	//			interval notify.notify_before day),'%Y-%m-%d')),(curdate() > DATE_FORMAT(DATE_SUB(crs.validity_date, interval notify.notify_before day),'%Y-%m-%d')))"); 

		//*$select->where("MOD(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
		//		(DATE_SUB(crs.validity_date, interval notify.notify_before day)))),notify.frequency_data_value) = 0");
		$select->where("if(crs.validity_days IS NOT NULL,(curdate() >= DATE_FORMAT(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day),
				interval notify.notify_before day),'%Y-%m-%d')),(curdate() >= DATE_FORMAT(DATE_SUB(crs.validity_date, interval notify.notify_before day),'%Y-%m-%d')))");

		$select->where("if(crs.validity_date IS NULL,
				(curdate() != DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),'%Y-%m-%d')),
				(date_format(DATE_ADD(crs.validity_date, interval 1 day),'%Y-%m-%d')!=curdate()))");

		$select->where("MOD(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day), interval notify.notify_before day)),
				(DATE_SUB(crs.validity_date, interval notify.notify_before-1 day)))),notify.frequency_data_value) = 0");
		expDebug::dPrintDBAPI('autoComplianceExpiryDaysReminder query', $select);
		$result =  $select->execute()->fetchAll();
		for($i=0;$i<count($result);$i++){
			$courseId      = $result[$i]->course_id;
			$classId       = $result[$i]->class_id;
			$userId        = $result[$i]->user_id;//getIdOfLoggedInUser();
			$enrId         = $result[$i]->enrollid;
			$updateUserId  = 1;
			expDebug::dPrint('checkign the details of the notification'.$result[$i]->course_id);
			insertNotification('ComplianceExpiryRemainder', $enrId, $classId, $userId, $updateUserId);
		}

	}catch (Exception $ex) {
		watchdog_exception('automaticComplianceExpirytoIncomplete', $ex);
		expertusErrorThrow($ex);
	}
}*/


/**
 *  Compliance reminder notification for completed due date/ day
 */
/*function autoComplianceExpiredReminder(){
	try{
		$select = db_select('slt_enrollment','enr');
		$select->leftJoin('slt_course_template','crs','crs.id = enr.course_id and crs.is_compliance=1');
		$select->addField('enr','user_id', 'user_id');
		$select->addField('enr','id', 'enrollid');
		$select->addField('enr','course_id', 'course_id');
		$select->addField('enr','class_id', 'class_id');
		$select->addExpression('max(enr.id)', 'enrollid');
		$select->addExpression('max(enr.course_id)', 'course_id');
		$select->condition('crs.is_compliance', 1,'=');
		$select->condition('enr.is_compliance', 1,'=');
		$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp','=');
		$select->condition('enr.cmpl_expired', '1','!=');
		$select->where("if(crs.validity_date IS NULL,
				(curdate() >= DATE_FORMAT(DATE_ADD(enr.comp_date, interval crs.validity_days+1 day),'%Y-%m-%d')),
				(date_format(DATE_ADD(crs.validity_date, interval 1 day),'%Y-%m-%d')=curdate()))");
		$select->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
				->condition('crs.validity_days','','IS NOT NULL'));
		$select->groupBy('enr.course_id');
		$select->groupBy('enr.user_id');
		expDebug::dPrintDBAPI(' autoComplianceExpiredReminder  ' , $select);
		$result =  $select->execute()->fetchAll();
		for($i=0;$i<count($result);$i++){
			$courseId      = $result[$i]->course_id;
			$enrollid			 = $result[$i]->enrollid;
			$classId       = $result[$i]->class_id;
			$userId        = $result[$i]->user_id;//getIdOfLoggedInUser();
			$updateUserId  = 1;
			expDebug::dPrint('Current new date re-register = '."$courseId, $classId, $userId || " . print_r($currentDate, true)." ======= ".print_r($dateExpiry, true) , 4);

			$resultCnt = complianceExpiryandRegisterCount($courseId,$enrollid,$userId);
			if($resultCnt == 0){
				$updateStmt = db_update('slt_enrollment');
				// Set conditions
				$updateStmt->condition('id',$result[$i]->enrollid,'=');
				// Set new values of fields
				$updateStmt->fields(array(
						'comp_status' => 'lrn_crs_cmp_exp',
						'cmpl_expired'	=> 1,
						'updated_on'    => now(),
						'updated_by'    => $user_id
				));
				expDebug::dPrintDBAPI(' $updateStmt = ', $updateStmt);
				$updateStmt->execute();

				//if($currentDate == $dateExpiry){
				insertNotification('ComplianceClassExpired', $enrollid, $classId, $userId, $updateUserId);
			}else{
				insertNotification('ComplianceClassExpiredAndEnrolled', $enrollid, $classId, $userId, $updateUserId);
			}

		}
	}catch (Exception $ex) {
		watchdog_exception('autoComplianceExpiredReminder', $ex);
		expertusErrorThrow($ex);
	}
}*/



/*
 * Compliance Re-registration
 *
 */
/*function complianceReregistration() {
  try{
      expDebug::dPrint('Inside of complianceReregistration',4);
      $compliance_course_class_obj = compliance_course_class();
      $result = array();
      if(count($compliance_course_class_obj['course'])>0){
	      $select = db_select('slt_enrollment','enr');
	      $select->addField('enr','user_id', 'user_id');
	      $select->addField('enr','course_id', 'course_id');
	      $select->addField('enr','class_id', 'class_id');
	      $select->condition('enr.is_compliance', 1,'=');
	      $select->condition('enr.course_id', $compliance_course_class_obj['course'],'IN');
	      $select->condition('enr.comp_status', 'lrn_crs_cmp_exp','=');
	      $select->groupBy('enr.user_id');
	      $select->groupBy('enr.course_id');
	      expDebug::dPrintDBAPI("Completed complaince course list -> ",$select);
	      $result =  $select->execute()->fetchAll();
      }
      for($i=0;$i<count($result);$i++) {
           $userId           = $result[$i]->user_id;
           $crs_id           = $result[$i]->course_id;
           $cls_id           = $result[$i]->class_id;
           $get_compliance_class_to_enroll = compliance_class_enrollment($userId,$crs_id,$cls_id,false,$compliance_course_class_obj);

           $waitlist         = false;
		       $isAdminSide      = 'N';
           if($get_compliance_class_to_enroll>0 || $get_compliance_class_to_enroll == -2) {
             $checkRegStatus = getComplianceRegistrationAllowed($userId, $crs_id);
             if($checkRegStatus == 0){
               $accessControlCheck = getAccessControlCheck($userId, $get_compliance_class_to_enroll,$crs_id);
               if($accessControlCheck[0]->cnt == 1){
                 $extraParam =  array();
                 $extraParam['regFrom'] = 'compliance_reregistration';
                 register_user_to_course_class($userId, $crs_id, $accessControlCheck[0]->clsid,$waitlist,$isAdminSide,false,false,'',$extraParam);
               }else if($accessControlCheck[0]->cnt > 1){
               	 insertNotification("MultipleComplianceClassNotification", $crs_id, $cls_id, $userId, $userId);
               }
             }
           }
      }
  }catch (Exception $ex) {
	  watchdog_exception('automaticComplianceExpirytoIncomplete', $ex);
	  expertusErrorThrow($ex);
  }
}*/

/*
 * User can register given class
 */
function getComplianceRegistrationAllowed($userId,$crs_id){
    try {
  // Select table
  $selectStmt = db_select('slt_enrollment', 'enr');
  // Set conditions
  $reg_status_condition_and = db_and();
  $reg_status_condition_and->condition('enr.user_id', $userId);
  $reg_status_condition_and->condition('enr.course_id', $crs_id);
  $reg_status_condition_or = db_or();
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_cnf');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_rsv');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_wtl');
  $reg_status_condition_or->condition('enr.reg_status', 'lrn_crs_reg_ppm');
  $reg_status_condition_and->condition($reg_status_condition_or);
  $selectStmt->condition($reg_status_condition_and);
  $selectStmt->condition('enr.comp_status' ,array('lrn_crs_cmp_cmp','lrn_crs_cmp_exp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw'), 'NOT IN');
  // Select fields
  $selectStmt->addExpression('count(enr.id)','cnt');
  expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
  // Execute query
  $result = $selectStmt->execute()->fetchField();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getEnrollmentRegistrationStatus', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Get access control for given classes
 */
function getAccessControlCheck($userId,$classId,$crsId){
  try {

    $select = db_select('slt_course_class', 'cls');

    // Select fields
    $select->addField('cls','id','clsid');
    $select->addExpression('count(cls.id)','cnt');
    filterByUserAccessPrivileges($select, 'cls',$userId,'','','Class');
    if($classId == -2) {
    	$select->condition('cls.course_id',$crsId);
    } else{
    	$select->condition('cls.id' ,$classId);
    }
    expDebug::dPrintDBAPI('select = getAccessControlCheck' , $select);
    $result = $select->execute()->fetchAll();
    expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
    return $result;
  }catch (Exception $ex) {
    watchdog_exception('getAccessControlCheck', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * To get compliance class id, in the list of already enrolled class
 */
function compliance_class_enrollment($userId,$courseId,$classId,$classExclude = false,$compliance_course_class_obj = '') {
  try{
  		if(empty($compliance_course_class_obj)){
        $compliance_course_info   = compliance_course_class();
      }else{
        $compliance_course_info = $compliance_course_class_obj;
      }
      expDebug::dPrint('Inside of compliance_class_enrollment',4);
      $result=array();
      if(count($compliance_course_info['class'][$courseId])>0){
	      $select = db_select('slt_enrollment','enr');
	      $select->addField('enr','id', 'enrollid');
	      $select->addField('enr','class_id', 'class_id');
	      $select->addField('enr','comp_date', 'comp_date');
	      $select->condition('enr.is_compliance', 1,'=');
	      $select->condition('enr.course_id', $courseId,'=');	   
	      $select->condition('enr.user_id', $userId,'=');
	      $select->condition('enr.comp_status', array('lrn_crs_cmp_exp','lrn_crs_cmp_cmp'),'IN');
	      $select->groupBy('enr.comp_status');
	      expDebug::dPrintDBAPI(' $$select = ' , $select);
	      $result =  $select->execute()->fetchAll();
	      expDebug::dPrint("Enrolled complaince course -- ".print_r($result,true),3);
      }
      $compliance_course_class  = $compliance_course_info['class'];
      $enroll_class             = array();
      $compliance_validity_date = $compliance_course_info['validity_date'][$courseId];
      $compliance_validity_days = $compliance_course_info['validity_days'][$courseId];
      if($compliance_validity_days){
        $compliance_validity_days  = $compliance_validity_days+1;
      }
      $class_delivery_type      = $compliance_course_info['delivery_type'][$courseId];
      $complianceValidityDate   = '';
      $todayDate                = date("Y-m-d");
      if($compliance_validity_date != '') {
        $cvdate = new DateTime($compliance_validity_date);
				$cvdate->add(new DateInterval('P1D'));
        $complianceValidityDate = $cvdate->format('Y-m-d');
      }
      $isClassComplianceValidity =  0;
      $to_enroll_class         = 0; // There is no enrollment
      $totResult = count($result);      
      if($totResult>0) {
          for($i=0;$i<$totResult;$i++) {
               $enrolledId      	= $result[$i]->enrollid;
               $cls_id          	= $result[$i]->class_id;
               $enrollCompDate  	= $result[$i]->comp_date;
               $delivery_type_code  = $class_delivery_type[$cls_id];
               $available_seats     = 1;
               expDebug::dPrint('complianceValidityDate' . $complianceValidityDate."    --- compliance_validity_days ---- ".$compliance_validity_days,4);
               if(empty($complianceValidityDate) && !empty($compliance_validity_days)) {
                 $isClassComplianceValidity = 1;
                 $complianceValidityDateFrm = strtotime(date("Y-m-d", strtotime($enrollCompDate)) . "  +".$compliance_validity_days." days");
                 $complianceValidityDate 	= strftime("%Y-%m-%d",$complianceValidityDateFrm);
               }

			  	    if($delivery_type_code == 'lrn_cls_dty_ilt' || $delivery_type_code == 'lrn_cls_dty_vcl'){
			          $available_seats = getAvailableSeats($cls_id);
			  	    }

               expDebug::dPrint("Compliance Re-Register :| Date Compare : ".$todayDate." == ".$complianceValidityDate." | EnrollId : ".$enrolledId." | Course Id :".$courseId." | Class Id : ".$cls_id , 4);
               if(($todayDate >= $complianceValidityDate) && ($available_seats >= 1 )) {
                   expDebug::dPrint("Compliance Re-Register inside date  check ". $todayDate .'---'. $complianceValidityDate,4);
                   if((($classExclude) && ($classId == $cls_id)) || ($delivery_type_code == 'lrn_cls_dty_wbt' || $delivery_type_code == 'lrn_cls_dty_vod') && ($totResult==1)) {
                     //$enroll_class[] = $cls_id;
                   } else {
                     $enroll_class[] = $cls_id;
                   }
               }else{
               		return 0; // No validity defined reregistration should not happen.
               }
               if($isClassComplianceValidity){
                 $complianceValidityDate   = '';
               }
          }

          $comp_class       		= $compliance_course_class[$courseId];
          expDebug::dPrint("comp_class " . print_r($comp_class,true),4);
          $notEnrolledClass 		= array_diff($comp_class,$enroll_class);
          expDebug::dPrint("enroll_class " . print_r($enroll_class,true),4);
          sort($notEnrolledClass);
          expDebug::dPrint("notEnrolledClass " . print_r($notEnrolledClass,true),4);
          $count_not_enrolled_class = count($notEnrolledClass);          
          $to_enroll_class  		= (($count_not_enrolled_class==1) ? $notEnrolledClass[0] : (($count_not_enrolled_class > 1) ? -2 : -1)); // Can enroll ==> (>0) || can't enroll ==> (<=0)
          expDebug::dPrint("to_enroll_class " . $to_enroll_class,4);
      }
      return $to_enroll_class;
  }catch (Exception $ex) {
	  watchdog_exception('automaticComplianceExpirytoIncomplete', $ex);
	  expertusErrorThrow($ex);
  }
}
/*
 * To get list of compliance class for courses
 */
function compliance_course_class() {

  try{
      $select = db_select('slt_course_template','crs');
      $select->innerJoin('slt_course_class','cls','cls.course_id = crs.id');
      $select->addField('crs','id', 'course_id');
      $select->addField('cls','id', 'class_id');
      $select->addField('crs','complete_date', 'complete_date');
      $select->addField('crs','complete_days', 'complete_days');
      $select->addField('crs','validity_date', 'validity_date');
      $select->addField('crs','validity_days', 'validity_days');
      $select->addField('cls','delivery_type', 'delivery_type');
      $select->condition(db_and()->condition('crs.is_compliance', 1, '=')
                                 ->condition(db_or()->isNull('crs.complete_date')
                                                    ->condition('crs.complete_date', NOW(), '>')
                                            )
                        );
      $select->condition('crs.status', 'lrn_crs_sts_atv','=');
      $select->condition('cls.status', 'lrn_cls_sts_atv','=');
      $price_condition_or = db_or();
      $price_condition_or->condition('cls.price', 0);
      $price_condition_or->isNull('cls.price');
      $select->condition($price_condition_or);
      expDebug::dPrintDBAPI('select = ' , $select);
      $result =  $select->execute()->fetchAll();

      $course_stack = array();
      $course_info_stack = array();

      for($i=0;$i<count($result);$i++) {
         $course_id  = $result[$i]->course_id;
         $class_id   = $result[$i]->class_id;
         $validity_date = $result[$i]->validity_date;
         $validity_days = $result[$i]->validity_days;
         if(!array_key_exists($course_id,$course_stack)) {
           $course_stack[$course_id] = array($class_id);
           $course_info_stack['course'][] = $course_id;
           $course_info_stack['class'][$course_id] = array($class_id);
           $course_info_stack['delivery_type'][$course_id] = array($class_id => $result[$i]->delivery_type);
           $course_info_stack['validity_date'][$course_id] = $validity_date;
           $course_info_stack['validity_days'][$course_id] = $validity_days;
         } else {
           $course_stack[$course_id][] = $class_id;
           $course_info_stack['class'][$course_id][] = $class_id;
           $course_info_stack['delivery_type'][$course_id] = array($class_id => $result[$i]->delivery_type);
           $course_info_stack['validity_date'][$course_id] = $validity_date;
           $course_info_stack['validity_days'][$course_id] = $validity_days;
         }
      }
      expDebug::dPrint("compliance_course_class course_info_stack" . print_r($course_info_stack,true));
      return $course_info_stack;

  }catch (Exception $ex) {
	  watchdog_exception('automaticComplianceExpirytoIncomplete', $ex);
	  expertusErrorThrow($ex);
  }
}

/*
 * To get compliance class enrollments for auto cancel when user profile has been changed.
 */
function getComplianceCourseEnrollAutoCancel($updatedEnrid,$user_id){
  try{
    	$updateStmt = db_update('slt_enrollment');
    	// Set conditions
    	$updateStmt->condition('id',$updatedEnrid,'IN');
    	$updateStmt->condition('master_enrollment_id','','IS NULL');
    	// Set new values of fields
    	$updateStmt->fields(array(
    		'mandatory' 	=> NULL,
    		'is_compliance' => NULL,
    		'updated_on'    => now(),
    		'updated_by'    => $user_id
    	));
    	expDebug::dPrintDBAPI(' $updateStmt = ', $updateStmt);
    	$updateStmt->execute();
    	syncSolrData('Enrollment');
  }catch (Exception $ex) {
    watchdog_exception('getComplianceCourseEnrollAutoCancel', $ex);
    expertusErrorThrow($ex);
   }
}

//Function for converting slp_enrollment_del
function enrollmentDelete($enrollId, $updateUId){
	$txn = db_transaction();
	$comp_sts = array(
		'lrn_crs_cmp_cmp',
		'lrn_crs_cmp_inc',
		'lrn_crs_cmp_nsw',
		'lrn_crs_cmp_inp'
	);
	try {
  $updateStmt = db_update('slt_enrollment');
  // Set conditions
  $updateStmt->condition('id', $enrollId);  
//   $updateStmt->condition('comp_status', $comp_sts,'NOT IN');
  $updateStmt->condition('reg_status', 'lrn_crs_reg_can','!=');
  $updateStmt->condition(db_or()->condition('comp_status', '', 'IS NULL')
  								->condition('comp_status', $comp_sts, 'NOT IN'));
  // Set new values of fields
  $updateStmt->fields(array(
                        'reg_status'      	=> 'lrn_crs_reg_can',
                        'comp_status'   	=> NULL,
                        'comp_date'			=> NULL,
                        'reg_status_date'	=> now(),
                        'waitlist_flag'		=> NULL,
                        'waitlist_priority' => NULL,
                        'updated_on'    	=> now(),
                        'updated_by'   		=> $updateUId,
  						'comp_on'    		=> now(),
  						'comp_by'    		=> $updateUId
                        ));
   expDebug::dPrintDBAPI(' $updateStmt = ', $updateStmt);
   $affected_rows = $updateStmt->execute();
   expDebug::dPrint('affected rows = '.$affected_rows, 5);
   return $affected_rows;
 }catch (Exception $ex) {
   	$txn->rollback();
   	watchdog_exception('enrollmentDelete', $ex);
   	throw $ex;
 }
 unset($txn);
}

/*
 *learnerProfileAPI
*/
function learnerProfileAPI($userId){
	try{
  $form=array();
  $form["form_id"]="";
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrsearch/exp_sp_lnrsearch.inc');
  include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_skill/exp_sp_my_skill.inc";
  include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_activity/exp_sp_my_activity.inc";
    include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_admin_userpoints/exp_sp_admin_userpoints.inc";
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc');
  include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_announcement/exp_sp_administration_announcement.inc');
  include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info_share.inc";
  
  global $catalog_reg;
  $learnerid = $_REQUEST["learnerid"];
  expDebug::dPrint('$learnerid : '.$learnerid,4);
  $drupalUid = getDrupalIdOfUser($learnerid);
  $account = user_load($drupalUid);

  if($_REQUEST['action'] == 'logout')
  {
  	add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_login', 'Logout', 'Logout via mobile');

	expDebug::dPrint('loggggiiinnnnngggg outtttttt');

	$query = db_merge('slt_manage_pushnotification');
	$query->key(array(
			'user_id' => $userId,
	));
	$query->fields(array(
			'logged_out' => 1,
	));

	expDebug::dPrintDBAPI('Manage notification -->>',$query);
	$query->execute();


	$query = db_delete('slt_mobiledevice_tokens');
	$query->condition('user_id',$userId);
	$query->condition('gcm_regid',$_REQUEST["device_token"]);
	expDebug::dPrintDBAPI('Manage notification -->>',$query);
	$query->execute();

  	return;
  }

  if(!empty($account->picture->uri)){
  	 $filepath  = $account->picture->uri;
  	 $profileImage = file_create_url($filepath);
  }
  expDebug::dPrint("total points count" . print_r($totalPoints,true),5);
  $is_training_admin = user_access('Administration Perm', $account);


  //ADDED FOR PAGINATING SKILLS
  if(isset($_REQUEST['mobile_version']) && isset($_REQUEST['skillspage']) && isset($_REQUEST['skillsrows']) && !empty($_REQUEST['skillspage']) && !empty($_REQUEST['skillsrows'])){
	 $getSkillDetails   = getMySkillDetails($learnerid,false);
 	 $getSkillCount   = getMySkillDetails($learnerid,true);
  	$page      = $_REQUEST['skillspage'];
  	$limit     = $_REQUEST['skillsrows'];
  
  	expDebug::dPrint('  checkconditionpage = ' . print_r($page, true) , 3);
  
  	expDebug::dPrint('  checkconditionlimit = ' . print_r($limit, true) , 3);
  
  
  	if($getSkillCount >0 ){
  		$total_pages = ceil($getSkillCount/$limit);
  	}else{
  		$total_pages = 0;
  	}
  	if($page > $total_pages){
  		$page=$total_pages;
  	}
  	$start = $limit * $page - $limit;
  	if($start < 0) {
  		$start = 0;
  	}
  
  	$getSkillDetails->range($start,$limit);
  	//$getSkillDetails->orderBy('created_on', 'DESC');
  
}else{
    $getSkillCount   = getMySkillDetails($learnerid,true,'','1','1','skill');
    $getSkillDetails   = getMySkillDetails($learnerid,false,'','1','1','skill');
    $getSkillDetails->range(0,$getSkillCount);
	
}
  
  
  
  
  
  
  
  
  
  $skillQueryResult = $getSkillDetails->execute()->fetchAll();
  
  
  //$getSkillCount   = getMySkillDetails($learnerid,true);
  $getSkillDetails   = getMySkillDetails($learnerid,false,'','','1','certificate');
  //$getSkillDetails->range(0,$getSkillCount);
  $CertificateQueryResult =  $getSkillDetails->execute()->fetchAll();
  
  
  $countActivity      = getMyProfileActivityDetails($learnerid,TRUE);
  $activityDetails	  = getMyProfileActivityDetails($learnerid,False);
  //$activityDetails->range(0,$countActivity);
  
  //Added for 0059382
  
  if(isset($_REQUEST['page']) && isset($_REQUEST['rows']) && !empty($_REQUEST['page']) && !empty($_REQUEST['rows']))
  {
  
  	$page      = $_REQUEST['page'];
  	$limit     = $_REQUEST['rows'];
  
  	expDebug::dPrint('  checkconditionpage = ' . print_r($page, true) , 3);
  
  	expDebug::dPrint('  checkconditionlimit = ' . print_r($limit, true) , 3);
  
  
  	if($countActivity >0 ){
  		$total_pages = ceil($countActivity/$limit);
  	}else{
  		$total_pages = 0;
  	}
  	if($page > $total_pages){
  		$page=$total_pages;
  	}
  	$start = $limit * $page - $limit;
  	if($start < 0) {
  		$start = 0;
  	}
  
  	$activityDetails->range($start,$limit);
  }else{
  	 
  	$activityDetails->range(0,$countActivity);
  	 
  }
  
  
  
  $activityQueryResult = $activityDetails->execute()->fetchAll();
  foreach($activityQueryResult as $key => $res){
  	$res->created_on = timeDifferenceLoggedAction($res->created_on);
  	if($res->entity_type == 'cre_sys_obt_cls' || $res->entity_type == 'Class'){
  		$delType = getClassTypeFromId($res->entity_id);
  		$activityQueryResult[$key]->deliveryType = $delType[0]->delivery_type;
  		$activityQueryResult[$key]->classTitle   = $delType[0]->title;
  	}else if($res->entity_type == 'cre_sys_obt_trn' || $res->entity_type == 'cre_sys_obt_crt' || $res->entity_type == 'cre_sys_obt_cur' || $res->entity_type == 'Certification' || $res->entity_type == 'Curricula' || $res->entity_type == 'Learning Plan'){
  	  	$delType = getTPTypeFromId($res->entity_id);
  		$activityQueryResult[$key]->classTitle   = $delType[0]->title;
  	}
  	else{
  		$activityQueryResult[$key]->deliveryType = '';
  		$activityQueryResult[$key]->classTitle = '';
  	}
  }
  $totalPoints		  = getTotalPointsGainedByUser($learnerid);
  $totalActionPoints  = getActionPointsForUser($learnerid);
  $userBadgesView     = BadgesCount($totalActionPoints);


	if($_REQUEST['action'] == 'currency_changed'){

  		//Below function will set currency details in $_REQUEST
		splitCurrencyInputAndSetInRequest();
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);

  	expDebug::dPrint('$userid'.$userid);
  	//update language parameters
  	$user_id = $_REQUEST['userid'];
  	$currency_changed = $_REQUEST['currency_type'];
  	expDebug::dPrint('$userid'.$user_id);
  	$drupalIdOfUser = getDrupalIdOfUser($user_id);
  	$updateSltperson = db_update('slt_person');
  	$updateSltperson->condition('id',$user_id);
  	$updateSltperson->fields(array('preferred_currency' => $currency_changed));
  	expDebug::dPrintDBAPI('preferred_currency ' , $updateSltperson);
  	$updateSltperson->execute();
  }

  $selectStmt = db_select('slt_person', 'prsn');
  $selectStmt->join('users', 'usr', 'prsn.user_name = usr.name');
  $selectStmt->leftJoin('slt_profile_list_items', 'prfl', 'prfl.code = prsn.time_zone');
  $selectStmt->leftJoin('slt_profile_list_items', 'prf2', 'prf2.code = prsn.preferred_language');
  $selectStmt->leftJoin('slt_profile_list_items', 'prf3', 'prf3.code = prsn.job_title');
  $selectStmt->leftJoin('slt_profile_list_items', 'prf4', 'prf4.attr1 = prsn.preferred_currency');
  $selectStmt->leftJoin('slt_location', 'my_location', 'prsn.preferred_loc_id = %alias.id');
  $selectStmt->leftJoin('slt_person_other_organization', 'porg', 'porg.user_id = prsn.id and porg.is_direct="Y"');
  $selectStmt->leftJoin('slt_organization', 'org', 'porg.organization_id = org.id and org.status != "cre_org_sts_del"');
  $selectStmt->condition('prsn.id', $learnerid);
  $selectStmt->addField('prsn', 'id', 'id');
  $selectStmt->addField('usr', 'uid', 'drupalid');
  $selectStmt->addField('prsn', 'is_instructor', 'is_instructor');
  $selectStmt->addField('prsn', 'is_manager', 'is_manager');
  $selectStmt->addExpression('CONCAT(prsn.first_name,\' \',prsn.last_name)','full_name');
  $selectStmt->addField('prsn', 'user_name', 'user_name');
  $selectStmt->addField('prsn', 'first_name', 'first_name');
  $selectStmt->addField('prsn', 'last_name', 'last_name');
  $selectStmt->addField('prsn', 'email', 'email');
  $selectStmt->addField('prsn', 'phone_no', 'phone_no');
  $selectStmt->addField('prsn', 'mobile_no', 'mobile_no');
  $selectStmt->addField('org', 'name', 'organization');
  $selectStmt->addField('prsn', 'about_me', 'about_me');
  $selectStmt->addField('prfl', 'attr1', 'time_zone');
  $selectStmt->addField('prfl', 'code', 'time_zone_code');
  $selectStmt->addField('prfl', 'attr2', 'timezone');
  $selectStmt->addField('prfl', 'name', 'timezone_name');
  $selectStmt->addField('prf2', 'name', 'person_pref_language');
  $selectStmt->addField('prf3', 'name', 'job_title');
  $selectStmt->addField('prsn','location_name', 'person_location_city');
  $selectStmt->addField('my_location','name', 'person_location_name');
  $selectStmt->addField('prsn','preferred_currency', 'preferred_currency');
  $selectStmt->addField('prf4','name', 'preferred_currency_name');

  expDebug::dPrintDBAPI('profile query',$selectStmt);


  $result =  $selectStmt->execute()->fetchAll();
  $result[0]->is_training_admin = ($is_training_admin == 1 || $is_training_admin == "1" )?"Y":"N";

  $result[0]->skillset  = $skillQueryResult;
  $result[0]->certificates  = $CertificateQueryResult;
  $result[0]->avatarimage = $profileImage;
  $is_manager = $result[0]->is_manager;
  $result[0]->is_manager = ($is_manager == Y || $is_manager == "Y" )?"Y":"N";
  $result[0]->totalpoints = $totalPoints;
  $result[0]->badges = $userBadgesView;
  $result[0]->activitydetails  = $activityQueryResult;
  $result[0]->courselevelview = $catalog_reg;
  $result[0]->iosurl = getConfigValue('iphone_download_url') ;
  $result[0]->androidurl = getConfigValue('android_download_url') ;
  $result[0]->cdn_status = getDrupalModuleStatus(array('cloud_files'));
  $result[0]->video_streaming = getDrupalModuleStatus(array('exp_sp_stream_server'));
  $result[0]->activitycount = $countActivity;
  $result[0]->skillscount = $getSkillCount;
  
  expDebug::dPrint('  checkconditionlimit sandhyaaa = ' . print_r($result, true) , 3);
  
  
  if(stripos($_SERVER['HTTP_USER_AGENT'], 'Android') ||
  		stripos($_SERVER['HTTP_USER_AGENT'], 'iPhone') ||
  		stripos($_SERVER['HTTP_USER_AGENT'], 'iPad') ){
	  db_update('users')->fields(array(
	      'access' => REQUEST_TIME,
	      'login' => REQUEST_TIME
	    ))->condition('uid', $drupalUid)->execute();
  }

  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_administration_commerce');
  $commerceEnabled =  $select->execute()->fetchField();
  $result[0]->commerce_enabled = $commerceEnabled;

  $select = db_select('system', 'sys');
  $select->addField('sys', 'status', 'status');
  $select->condition('name', 'exp_sp_fivestar');
  $ratingenabled =  $select->execute()->fetchField();
  $result[0]->rating_enabled = $ratingenabled;
  
  
  /*added for getting last modified timestamp of dl images*/
  $filename =  $_SERVER['DOCUMENT_ROOT'].'/sites/default/files/images/mobile.zip';
  $result[0]->dl_img_timestamp =  filemtime($filename);
  
  
  
  if($_REQUEST['action'] == 'login')
  {
  	add_audit_trail_entry($userId, $userId, 'cre_sys_obt_usr', 'exp_sp_login', 'Login', 'Login via Mobile');
	$query = db_merge('slt_manage_pushnotification');
	$query->key(array(
			'user_id' => $userId,
	));
	$query->fields(array(
			'logged_out' => 0,
	));

	expDebug::dPrintDBAPI('Manage notification -->>',$query);
	$query->execute();
  }else if($_REQUEST['action'] == 'lang_changed'){
  //	$_REQUEST['islang_changed']
  		try{
  			expDebug::dPrint('$userid'.$userid);
  			//update language parameters
  			$user_id = $_REQUEST['userid'];
  			$lang_web = $_REQUEST['lang_changed'];
  			expDebug::dPrint('$userid'.$user_id);
  			$prof_list_table   = db_select('slt_profile_list_items', 'prof');
  			$prof_list_table->addField('prof','attr1','attr1');
  			$prof_list_table->condition('prof.code', $lang_web);
  			$language_code = $prof_list_table->execute()->fetchField();
  			$drupalIdOfUser = getDrupalIdOfUser($user_id);
   			// update user table with universal language code
  			$updateUserLang = db_update('users');
  			$updateUserLang->condition('uid',$drupalIdOfUser);
  			$updateUserLang->fields(array('language' => $language_code));
  			$updateUserLang->execute();
 			 expDebug::dPrint('$drupalIdOfUser'.$drupalIdOfUser);
  			// update slt_person table with language code
  			$updateSltperson = db_update('slt_person');
  			$updateSltperson->condition('id',$user_id);
  			$updateSltperson->fields(array('preferred_language' => $lang_web));
  			$updateSltperson->execute();
  			}catch (Exception $ex) {
  				watchdog_exception('updateLanguage', $ex);
				  expertusErrorThrow($ex);
  }

  }
  //$clickstatus = getConfigValue('one_click_feature');
  //$result[0]->oneclick = $clickstatus;

  $select = db_select('languages', 'lan');
  $select->addField('lan', 'language', 'language');
  $select->addField('lan', 'name', 'name');
  $select->addField('lan', 'prefix', 'prefix');
  $select->condition('enabled', 1);
  $languagesEnabled =  $select->execute()->fetchAll();
  $result[0]->enabledLanguages = $languagesEnabled;

$select = db_select('slt_profile_list_items', 'splt');
  $select->addExpression('COUNT(splt.code)');
//   $select->addField('splt', 'code', 'code');
//   $select->addField('splt', 'name', 'name');
//   $select->addField('splt', 'attr1', 'label');
//   $select->addField('splt', 'attr2', 'symbol');
  $select->condition('is_active', 'Y');
  $select->condition('attr3', 'Y');
  $select->condition('splt.code', 'cre_sys_crn_%', 'LIKE');
  $enabledCurrencies =  $select->execute()->fetchField();
  $result[0]->enabled_currency = $enabledCurrencies;




//   $select = db_select('slt_notification', 'notif');
//   $select->addExpression('COUNT(notif.id)');
//   $select->condition('notif.user_id', $userId);
//   $select->condition('notif.mobile', 1);
//   $select->condition('notif.viewed', 1 , '<>');
//   $select->condition('notif.soft_delete', 1,'<>');
//   $notViewedNotif =  $select->execute()->fetchField();
//   $result[0]->notification_notviewed = $notViewedNotif;

  //code added for push notification
  
  $select2 = db_select('slt_notification', 'notif');
  $select2->addField('notif', 'id', 'id');
  //   $select->addExpression('COUNT(notif.id)');
  $select2->condition('notif.user_id', $userId);
  $select2->condition('notif.mobile', 1);
  $select2->condition('notif.viewed', 1 , '<>');
  $select2->condition('notif.soft_delete', 1,'<>');
  
  $select1 = db_select('slt_bulk_notification', 'notif');
  $select1->addField('notif', 'id', 'id');
  //   $select->addExpression('COUNT(notif.id)');
  $select1->condition('notif.user_id', $userId);
  $select1->condition('notif.mobile', 1);
  $select1->condition('notif.viewed', 1 , '<>');
  $select1->condition('notif.soft_delete', 1,'<>');
  $selectunion = $select1->union($select2,'UNION ALL');
  $select = db_select($selectunion,'noti');
  $select->addExpression('COUNT(noti.id)');
  $notViewedNotif =  $select->execute()->fetchField();
 // $result[0]->notification_notviewed = $notViewedNotif;
  
  expDebug::dPrintDBAPI('Manage_notification_vieweddd1234 -->>',$notViewedNotif);
  
    if($_REQUEST['mobile_version']){
  	$announcementListCount = fetchAnnouncementUserWidget('count',$userid);
  	//$result[0]->notification_notviewed = $notViewedNotif+$announcementListCount;
  	$result[0]->notification_notviewed = $notViewedNotif;
  }else{
  	$result[0]->notification_notviewed = $notViewedNotif;
  }
  
  
  
    $select = db_select('block', 'blk');
    $select->addField('blk', 'region', 'region');
    $select->addField('blk', 'delta', 'delta');
    $select->condition('blk.theme',"expertusoneV2");
    $selectCondition = db_or();
    $selectCondition->condition('blk.delta', 'my_skill');
    $selectCondition->condition('blk.delta', 'my_activity');
    $selectCondition->condition('blk.delta', 'most_popular');
    $selectCondition->condition('blk.delta', 'highly_rated');
    $select->condition($selectCondition);
    $EnabledBlock =  $select->execute()->fetchAll();
    
    $fetch_points_status = "select * from system where name = 'exp_sp_admin_userpoints'";
    $qry_res  = db_query($fetch_points_status);
    $fetch_result_points = $qry_res->fetchAll();
    $result[0]->points_status = $fetch_result_points[0]->status;
    $user_points = array('region'=>$fetch_result_points[0]->status,'delta'=>'user_points');
    array_push($EnabledBlock,$user_points);
    $result[0]->EnabledBlock = $EnabledBlock;
      
  

//   select count(id) from slt_notification
//   where user_id =4 and mobile =1 and viewed =0

 $priceRange = getSliderPriceRange(); 

 if(empty($priceRange['startprice']))
 {
 	$priceRange['startprice'] = 0;
 }
 
 if(empty($priceRange['endprice']))
 {
 	$priceRange['endprice'] = 0;
 }
 
  $result[0]->min_price = $priceRange['startprice'];
  $result[0]->max_price = $priceRange['endprice'];
//   $result[0]->multi_currency = true;
  
  if(isset($_REQUEST["mobile_request"])){
  	
  $select = db_select('menu_links', 'menu');
  $select->addField('menu', 'hidden', 'hidden');
  $select->condition('link_title', 'CATALOG');
  $mobile_catalog =  $select->execute()->fetchField();
  $result[0]->mobile_catalog = $mobile_catalog;
  	 
  $mobile_analytics = getConfigValue ( 'mobile_analytics' );
  $result[0]->mobile_analytics=$mobile_analytics;
  $allow_currency_change = getConfigValue ( 'allow_currency_change' );
  $default_profile_currency = getCurrencyDefDetails();
  $result[0]->default_profile_currency=$default_profile_currency;
  $result[0]->allow_currency_change=$allow_currency_change;
  $share_module_status = getShareModuleStatus ( 'api' );
  $mylearning_mobile_view = getConfigValue ('mylearning_mobile_view');
  $result[0]->mylearning_mobile_view=$mylearning_mobile_view;
  $result[0]->share_module_status = "";
  if($share_module_status)
  {
  	$result[0]->share_module_status = 1;
  }
  }
      
  return $result;
	}catch (Exception $ex) {
    watchdog_exception('learnerProfileAPI', $ex);
    expertusErrorThrow($ex);
  }
 }

function updateLearnerProfileAtrributes() {
	try {
 	$form=array();
 	$form["form_id"]="";
 	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
 	$userid = $_REQUEST['userid'];
 	$newTimeZone = $_REQUEST['timezone'];
 	$oldTimeZone = $_REQUEST['oldtimezone'];
 	expDebug::dPrint('$userid'.$userid.'$newTimeZone'.$newTimeZone.'$oldTimeZone'.$oldTimeZone);
 	$drupalUid = getDrupalIdOfUser($userid);
 	$account = user_load($drupalUid,true);
 	expDebug::dPrint(' updateLearnerProfileAtrributes $userid = '.$userid,4);
 	expDebug::dPrint(' updateLearnerProfileAtrributes $drupalUid = '.$drupalUid,4);
 	expDebug::dPrint(' updateLearnerProfileAtrributes $account = '.print_r($account,true),5);
 	if(!empty($account->picture->uri) && !empty($_REQUEST["imgdata"]))
 	{
 		try
 		{
 			$filepath  = $account->picture->uri;
	 		$profileImage = file_create_url($filepath);
		 	expDebug::dPrint(' updateLearnerProfileAtrributes = '.$filepath);
		 	expDebug::dPrint(' updateLearnerProfileAtrributes $profileImage = '.$profileImage);
		 	$data = parse_url($profileImage);
		 	$filepath = $_SERVER["DOCUMENT_ROOT"].$data["path"];
		 	drupal_chmod($filepath);
		 	replaceAvatarImage($filepath,$_REQUEST["imgdata"]);
	 		make_audit_trail_entries($fields, $userid , $oldTimeZone ,$userid, 'Updated','cre_sys_obt_usr','exp_sp_learning','updateLearnerProfileAtrributes',
	 		array('avatar_image'));
	 		return array((object)array('status'=>'success'));
 		}catch (Exception $ex) {
		    watchdog_exception('updateLearningProfileAttributes', $ex);
		    $errobj=new stdClass();
		    $errobj->isValidateError = 1;
		    $errobj->errcode = 'L_012';
		    $errobj->errormsg = "Image data/Image URL is invalid";
		    return $errobj;
		    //return array((object)array('status'=>'failure'));
		    expertusErrorThrow($ex);
		  }
 	}
 	else
 	{
 	 	$fields                = array( );
	 	$details = array();
	 	$preferredShortTimeCode = getProfileListItemByCode($newTimeZone);
	 	if(empty($preferredShortTimeCode[0]->attr2)){
	 		$errobj=new stdClass();
	 		$errobj->isValidateError = 1;
	 		$errobj->errcode = 'L_012';
	 		$errobj->errormsg = "Invalid Timezone";
	 		return $errobj;
	 	}
	 	else if($preferredShortTimeCode){
	 		$details['timezone']  = $preferredShortTimeCode[0]->attr2;
	 		$fields['time_zone']  = $newTimeZone;
	 	}
	 	$updateStmt = db_update('slt_person');
	 	$updateStmt->fields($fields);
	 	$updateStmt->condition('id', $userid);
	 	$numUpdated = $updateStmt->execute();
	 	$updatedUserAccount = user_save($account, $details);
	 	if($numUpdated && $updatedUserAccount){
	 		make_audit_trail_entries($fields, $userid , $oldTimeZone ,$userid, 'Updated','cre_sys_obt_usr','exp_sp_learning','updateLearnerProfileAtrributes',
	 		array('time_zone'));
	 		return array((object)array('status'=>'Success'));
	 	}else{
	 		$errobj=new stdClass();
	 		$errobj->isValidateError = 1;
	 		$errobj->errcode = 'L_012';
	 		$errobj->errormsg = "Error in updating Image data / Image URL";
	 		return $errobj;
	 		//return array((object)array('status'=>'Failure'));
	 	}
	}
	}catch (Exception $ex) {
    watchdog_exception('updateLearnerProfileAtrributes', $ex);
    expertusErrorThrow($ex);
  }
}
function replaceAvatarImage($filepath,$base64img){
	try{
		$base64img = preg_replace('#^data:image/[^;]+;base64,#', '', $base64img);
		$newBase = str_replace(' ', '+', $base64img);
		$binary = base64_decode($newBase);
		$file = fopen($filepath, 'wb');
		fwrite($file, $binary);
		fclose($file);
	}catch (Exception $ex) {
	    watchdog_exception('replaceAvatarImage', $ex);
	    expertusErrorThrow($ex);
  }
}
 /*
 * addHiddenFieldToAddEditForm - Use this function to add hidden field to the addedit form
 *                               These values are included in the page HTML, but are not displayed to the user.
 */
function addHiddenFieldToAddEditFormNew(&$form, $id, $defaultValue = '') {
	try{
  $form[$id] = array(
    '#type' => 'hidden',
    '#default_value' => $defaultValue,
    '#attributes' => array('id'=>$id) // Added by Vincent on 26 Mar, 2012
  );
	}catch (Exception $ex) {
    watchdog_exception('addHiddenFieldToAddEditFormNew', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * addTextFormatFieldToAddEditForm() -  Use this function to add a text format to the addedit form
 */
function addTextFormatFieldToAddEditFormNew(&$form, $id, $title = '', $defaultValue = '', $valueFormat = 'full_html', $tabIndex = null, $mandatory = false, $readOnly = false, $charLimit = 1000) {
	try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete
  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  // Add class to apply common style to all text format fields.
  $attributes['class'][] = 'addedit-edit-textarea';
  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textarea';
  }
  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }
  $attributes['data'] = $id;
  $charLimitLeft = getEditorWordCount($defaultValue);
  // Create the form element
  $form[$id] = array(
    '#type' => 'text_format',
  	'#name' => $id,
    '#format'=> $valueFormat,
    '#suffix' => '<div  class="admin-desc-character-count catalog-desc-count"><span id="char_count_'.$id.'">'.$charLimitLeft.'</span>'.' ' .t('LBL419').' '.$charLimit.' '.t('LBL420').'.</div>',
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#rows'=>'3',
    '#cols'=>'40' ,
  );
	}catch (Exception $ex) {
    watchdog_exception('addTextFormatFieldToAddEditFormNew', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * Waitlist Promotion Logic
 */
function waitlistPromotionUpdate($classId,$prevRegStatus){
  try{
  $selectClasses = db_select('slt_enrollment', 'enr');
  $selectClasses->addField('enr','id');
  $selectClasses->addField('enr','class_id');
  $selectClasses->addField('enr','course_id');
  $selectClasses->addField('enr','master_enrollment_id');
  $selectClasses->addField('enr','user_id');
  $selectClasses->addField('enr','reg_status');
  $selectClasses->addField('enr','comp_status');
  $selectClasses->condition('enr.class_id', $classId);
  $selectClasses->condition('enr.reg_status', array('lrn_crs_reg_cnf','lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_ppv'),'IN');
  $selectClasses->condition('enr.waitlist_flag', 'lrn_crs_reg_wtl','=');
  $selectClasses->orderBy('enr.waitlist_priority');
  expDebug::dPrintDBAPI('waitlistPromotionUpdate ' , $selectClasses);
  $resultClasses =$selectClasses->execute()->fetchAll();
  $inc = 0;
  if($prevRegStatus == 'lrn_crs_reg_wtl'){
    $inc = 1;
  }
  foreach($resultClasses as $record)
  {
    if($inc == 0 && ($prevRegStatus =='lrn_crs_reg_cnf' || $prevRegStatus == 'lrn_crs_reg_ppm')){
      $availSeats =  getAvailableSeats($classId);
      if($availSeats <= 0 ){
        return;
      }
      if($record->reg_status =='lrn_crs_reg_ppm' || $record->reg_status =='lrn_crs_reg_ppv'){
        $reg_status = $record->reg_status;
        $comp_status = $record->comp_status;
      }
      else if($record->reg_status =='lrn_crs_reg_wtl'){
        $reg_status = 'lrn_crs_reg_cnf';
        $comp_status = 'lrn_crs_cmp_enr';
      }
      $updateStmt = db_update('slt_enrollment');
      $updateStmt->condition('id', $record->id);
      $updateStmt->fields(array(
                          'reg_status'    => $reg_status,
                          'updated_on'    => now(),
                          'updated_by'    => 1,
                          'comp_status'   => $comp_status,
      					  'waitlist_flag'   => NULL,
      					  'waitlist_priority' => NULL
                          ));
      $updateStmt->execute();
      if(empty($record->master_enrollment_id)){
        $promoteNotification = "CALL slp_notification_ins('WaitlistPromotion', '" . $record->course_id . "', '" . $record->class_id . "', '" .
                                                                                                                     $record->user_id . "', 'Admin', '" . $record->id . "')";
  	    expDebug::dPrint("waitlistPromotionUpdate Promote Notification : ".$promoteNotification , 4);
  	    db_query($promoteNotification);
  	    addICalToNotification($record->id);
      }
      if($record->master_enrollment_id){
        //waitlist promotion update
        waitlistTPPromotionUpdate($record->master_enrollment_id,$record->user_id);
      }
      $inc = $inc + 1;
    }
    else{
      //$txn = db_transaction();
    	$statements = array('statement' => true);
      try{
      $updateStmt = db_update('slt_enrollment');
      $updateStmt->condition('id', $record->id);
      $updateStmt->fields(array(
                          'reg_status'    => $record->reg_status,
                          'updated_on'    => now(),
                          'updated_by'    => 1,
      					  'waitlist_flag'   => 'lrn_crs_reg_wtl',
      					  'waitlist_priority' => $inc
                          ));
      if(!empty($statements)) {
      	$statements[] = $updateStmt;
      } else {
      	$updateStmt->execute();
      }
      dbStatementsExecution($statements);
      //$updateStmt->execute();
      }catch (Exception $ex2) {
          //  $txn->rollback();
          //  unset($txn);
            watchdog_exception('waitlistPromotionUpdate', $ex2);
            expertusErrorThrow($ex2);
       }
       // Commit the transaction
       unset($txn);
      $inc = $inc + 1;
      
    }
  }
  syncSolrData('MasterEnrollment');
  syncSolrData('Enrollment');
  }catch (Exception $ex) {
    watchdog_exception('waitlistPromotionUpdate', $ex);
    expertusErrorThrow($ex);
  }
}

function waitlistTPPromotionUpdate($masterEnrollId,$userId){
  //Count Number of waitlist class in the same program
  try {
  $selectWaitlistCnt = db_select('slt_enrollment', 'enroll');
  $selectWaitlistCnt->addExpression('COUNT(1)', 'countenr');
  $selectWaitlistCnt->condition('enroll.master_enrollment_id', $masterEnrollId);
  $selectWaitlistCnt->condition(db_and()->
    condition(db_or()->
    condition('enroll.waitlist_flag', 'lrn_crs_reg_wtl')->
    condition('enroll.reg_status', array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_ppv'),'IN')));
  $tpWaitlistCnt = $selectWaitlistCnt->execute()->fetchField();
  expDebug::dPrintDBAPI(' $$selectWaitlistCnt = ' , $selectWaitlistCnt);
  expDebug::dPrint(' $$$tpWaitlistCnt = ' . $tpWaitlistCnt,5);
  if($tpWaitlistCnt == 0){
    //Update master enroll
    $updateMasterEnr = db_update('slt_master_enrollment');
    $updateMasterEnr->condition('id', $masterEnrollId);
    $updateMasterEnr->condition('user_id', $userId);
    $updateMasterEnr->condition('overall_status', 'lrn_tpm_ovr_wtl');
    $updateMasterEnr->fields(array(
                        'overall_status'      => 'lrn_tpm_ovr_enr',
                        'updated_on'    => now(),
    					  				'updated_by'    => 1
                        ));
    $updateMasterEnr->execute();
    //Insert Automatic Waitlist Promotion Notification
    $objectId1 			= $masterEnrollId;
    $objectId2 			= getProgramIdFromEnrollment($masterEnrollId);
    $updateUserId 		= $userId;
     $iCal = getPublishICal($masterEnrollId, 'TP');
    insertNotification('certCurrLPWaitlistPromotion', $objectId1, $objectId2, $userId, $updateUserId, $iCal);
  }
  }catch (Exception $ex) {
    watchdog_exception('waitlistTPPromotionUpdate', $ex);
    expertusErrorThrow($ex);
  }
}

function getProgramIdFromEnrollment($masterEnrId){
  try{
  $select = db_select('slt_master_enrollment','mas');
  $select->addField('mas','program_id');
  $select->condition('mas.id', $masterEnrId,'=');
  $prgId   = $select->execute()->fetchField();
  return $prgId;
  }catch (Exception $ex) {
    watchdog_exception('getProgramIdFromEnrollment', $ex);
    expertusErrorThrow($ex);
  }
}

function getAvailableSeatInClasses($classList,$userId){
	try{
  $seatAvailable = 1;
  if(count($classList)){
    foreach($classList as $classId){
      $deliveryType = getClassDeliveryType($classId, 'Short');
      expDebug::dPrint('$deliveryType = ' . print_r($deliveryType, true) , 4 );
      if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
        $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv', 'lrn_crs_reg_wtl');
        $enrollCount = getClassEnrollmentCountForRegistrationStatuses(null, $classId, $regStatusesList,$userId);

        $select = db_select('slt_course_class_session', 'sc');
        $select->addField('sc','capacity_max', 'capacity_max');
        $select->addField('sc','waitlist_count', 'waitlist_count');
        $select->condition('sc.class_id',$classId, '=');
        $select->range(0, 1);
        $qry_result_session = $select->execute()->fetchObject();
        expDebug::dPrintDBAPI("Get max and waitlist seats for class : ",$select);

        $maxSeats = $qry_result_session->capacity_max;
        $waitlistSeat =  ($qry_result_session->waitlist_count) ?  $qry_result_session->waitlist_count : 0;
        $totalSeats = $maxSeats + $waitlistSeat;
        expDebug::dPrint("seats  : Classid - ". $classId . " Max - " .$maxSeats . " waitlistSeat - " .$waitlistSeat . " total seat -" .$totalSeats ." enrollCount -" . $enrollCount ,4);
        $seatAvailable =  $totalSeats - $enrollCount;
        if($seatAvailable <= 0){
          return 0;
        }
      }
    }
  }
  return $seatAvailable;
	}catch (Exception $ex) {
    watchdog_exception('getAvailableSeatInClasses', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
    expertusErrorThrow($ex);
  }
}

/*
 * prepareCrtExpiresInDisplayString() - Prepares crt expires_in display string
 */
function prepareCrtExpiresInDisplayString($expiresInValue, $expiresInUnit) {
	try {
  $expiresInValueStr = t('Does not expire');
  if (!empty($expiresInValue)) {
  	// Determine gender
    $pluralExpiresInUnit = false;
    if ($expiresInValue != 1) {
      $pluralExpiresInUnit = true;
    }
    // Determine unit string in correct gender
    $expiresInUnitStr = $pluralExpiresInUnit? t('LBL605') : t('LBL910'); // LBL605 = days, LBL910 = day
    if ($expiresInUnit == 'months') {
      $expiresInUnitStr = $pluralExpiresInUnit? strtolower(t('LBL911')) : t('LBL913'); //LBL912 = months, LBL913 = month
    }
    else if ($expiresInUnit == 'years') {
      $expiresInUnitStr = $pluralExpiresInUnit? t('LBL915') : t('LBL916'); //LBL915 = years, LBL916 = year
    }
    // Prepare the string
    $expiresInValueStr = $expiresInValue . ' ' . $expiresInUnitStr;
  }
  return $expiresInValueStr;
	}catch (Exception $ex) {
    watchdog_exception('prepareCrtExpiresInDisplayString', $ex);
    expertusErrorThrow($ex);
  }
}

/* function to retreive taxonomy term data id */
function GetForumTopicListForDetail($type,$eId){
	try {
	require_once (drupal_get_path('module', 'exp_sp_forum').'/exp_sp_forum.inc');
	$forumCls = new ForumDetails();

	if($type == "learningplan"){
	  $forumQry = $forumCls->forumTPSearchQuery();
	  $forumQry->condition('prm.id', $eId);
	  $forumQry->condition('ttd1.vid',2,'=');
      $forumQry->condition('ttd1.description','TP','=');
      $forumQry->condition('prm.discussion',1,'<>');
      expDebug::dPrintDBAPI("Get max and waitlist seats for class : ",$select);
	}else if($type == "catalog"){
		$forumQry = $forumCls->forumClassSearchQuery();
		$forumQry->condition('crs.id', $eId);
		$forumQry->condition('ttd.vid',2,'=');
    	$forumQry->condition('ttd.description','Course','=');
    	$forumQry->condition('crs.discussion',1,'<>');
	}
	expDebug::dPrintDBAPI("FORUM DISCUSSION IN DETAIL PAGE QUERY GetForumTopicListForDetail :::::::::: ",$forumQry);
	return $forumQry->execute()->fetchAll();
	}catch (Exception $ex) {
    watchdog_exception('GetForumTopicListForDetail', $ex);
    expertusErrorThrow($ex);
  }
}

/* get contents and their active versions to record into new table slt_enrollment_content_mapping */
function getContentAndVersionForClass($classId){
  try{
  	$selectStmt = db_select('slt_content_master', 'master');
  	$selectStmt->join('slt_course_content_mapper', 'mapper', 'master.id = mapper.content_id');
  	$selectStmt->join('slt_content_version', 'version', 'master.id = version.content_master_id');
  	$selectStmt->condition('master.status', 'lrn_cnt_sts_atv');
  	$selectStmt->condition('version.status', 'lrn_cnt_sts_atv');
  	$selectStmt->condition('mapper.class_id', $classId);
  	$selectStmt->addField('version', 'content_master_id', 'content_id');
  	$selectStmt->addField('version', 'id', 'version_id');
  	expDebug::dPrintDBAPI('$selectStmt = ' , $selectStmt);
  	return $selectStmt->execute()->fetchAll();
  }catch (Exception $ex) {
    watchdog_exception('getContentAndVersionForClass', $ex);
    expertusErrorThrow($ex);
  }
}

function getSessionDates($classId){
  try{
  $select = db_select('slt_course_class_session','sess');
  $select->condition('sess.class_id', $classId,'=');
  $select->addExpression("CONCAT(DATE_FORMAT(MIN(sess.start_date),'%m-%d-%Y'), '#' ,DATE_FORMAT(MAX(sess.start_date),'%m-%d-%Y'))", 'sessStartEndDates');
  $sessDates   = $select->execute()->fetchField();
  expDebug::dPrintDBAPI("getSessionDates Qry",$select);
  return $sessDates;
  }catch (Exception $ex) {
    watchdog_exception('getSessionDates', $ex);
    expertusErrorThrow($ex);
  }
}
/*
 * To get all tags and its count for the training program
 * @param1 - $objectId
 * - classId/training program id
 * @param2 - $objectType
 * - class/cre_sys_obt_crt/cre_sys_obt_cur/cre_sys_obt_trn
 * @return
 * - array of tag names for the given parameters.
 */
function getCatalogTags($objectId, $objectType){
  try{
  $tagNames  = array();
  if($objectType=='cre_sys_obt_crt')
    $objectType  = 'Certification';
  else if($objectType=='cre_sys_obt_cur')
    $objectType  = 'Curricula';
  else if($objectType=='cre_sys_obt_trn')
    $objectType  = 'Learning Plan';

  if($objectType=='Class'){
    $courseId  = '';
    $courseTagCount  = 0;
    $select = db_select('slt_course_class', 'crscls');
    $select->condition('crscls.id', $objectId, '=');
    $select->addField('crscls', 'course_id');
    $courseId= $select->execute()->fetchField();
    if($courseId!=''){
      $countQuery = db_select('slt_tag_entity','tagent');
      $countQuery->addExpression('COUNT(tagid)', 'tagcount');
      $countQuery->condition('tagent.entity_id',$courseId,'=');
      $countQuery->condition('tagent.entity_type','Course','=');
      $courseTagCount  = $countQuery->execute()->fetchField();
    }
    if($courseTagCount>0){
      $tagQuery  = db_select('slt_tag_entity','tagent');
      $tagQuery->condition(db_or()
      ->condition(db_and()->condition('tagent.entity_type','Course','=')
      ->condition('tagent.entity_id',$courseId,'='))
      ->condition(db_and()->condition('tagent.entity_type','Class','=')
      ->condition('tagent.entity_id',$objectId,'=')));
      $tagQuery->addField('tagent','tagid');
      $tagQuery->distinct();
      $tagresult = $tagQuery->execute()->fetchAll();
    }else{
      $tagQuery  = db_select('slt_tag_entity','tagent');
      $tagQuery->condition('tagent.entity_type','Class','=');
      $tagQuery->condition('tagent.entity_id',$objectId,'=');
      $tagQuery->addField('tagent','tagid');
      $tagQuery->distinct();
      $tagresult = $tagQuery->execute()->fetchAll();
    }
  }else{
      $tagQuery  = db_select('slt_tag_entity','tagent');
      $tagQuery->condition('tagent.entity_type',$objectType,'=');
      $tagQuery->condition('tagent.entity_id',$objectId,'=');
      $tagQuery->addField('tagent','tagid');
      $tagQuery->distinct();
      $tagresult = $tagQuery->execute()->fetchAll();
  }
  $tagIds = array();
  foreach($tagresult as $key => $val){
    $tagIds[] = $val->tagid;
  }
  expDebug::dPrint('Tag Array  = ' . $tagIds, 4);
  if(!empty($tagIds)){
    $tagNameQuery = db_select('slt_tagdefn', 'tagdfn');
    $tagNameQuery->addField('tagdfn', 'tagname');
    $tagNameQuery->addField('tagdfn', 'activity_count');
    $tagNameQuery->condition('tagdfn.id', $tagIds, 'IN');
    $tagNameResult = $tagNameQuery->execute()->fetchAll();
    foreach($tagNameResult as $key => $val){
      $tagNames[] = $val->tagname;
    }
  }
  return $tagNames;
  }catch (Exception $ex) {
    watchdog_exception('getCatalogTags', $ex);
    expertusErrorThrow($ex);
  }
}//End of getCatalogTags()

/**
 * Funtion to return the Multiple Registration Status.
 *
 * @param
 *	   Class ID
 * @return
 *     Status Code
 */
function isCourseLevelMultiregistrationAllowed($entityId,$courseid,$userId) {
  try{
    $classInfo = array();
    $entityId = !empty($entityId)? explode(',',$entityId) : array();
    $classInfo['classcount'] = false;
    if(count($entityId) >= 1) {
    	$select = db_select('slt_enrollment','enroll');
    	$select->rightJoin('slt_business_rule_mapping','rules','rules.entity_id = enroll.class_id AND rules.business_rule_code = \'cre_sys_brl_rra\' AND rules.entity_type = \'cre_sys_obt_cls\'');
    	$select->leftJoin('slt_profile_list_items','prof','prof.code = rules.business_rule_code');
    	$select->addField('enroll','class_id', 'classid');
    	$select->condition('enroll.user_id',$userId,'=');
    	$select->condition('enroll.master_enrollment_id','','IS NULL');
    	$select->condition('enroll.course_id', $courseid, '=');
    	$select->condition('prof.is_active', 'Y', '=');
    	$select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl'), 'IN'));
    	expDebug::dPrintDBAPI('getEnrolledIdForCourse ', $select);
    	$classInfo['classid'] = $select->execute()->fetchField();
    	$classInfo['classcount'] = ($classInfo['classid']) ? true : false;
    }
    return $classInfo;
  }catch (Exception $ex) {
    watchdog_exception('is_multiregistration_allowed', $ex);
    expertusErrorThrow($ex);
  }
}
function getSliderPriceRange(){
	try {
	$config           = getConfig("exp_sp");
	if(isset($config["maxprice"]) && $config["maxprice"] != 0) {
		$startprice = $config["minprice"];
		$endprice =  $config["maxprice"];
	}else{
		if ($cache = cache_get('exp_static_price_detail')){
			$prices = $cache->data;
		}else{
			$prices = getStaticPriceRangeDetails();
			cache_set('exp_static_price_detail', $prices);
		}
		$minpri = explode(",",$prices['minprice']);
		$maxpri = explode(",",$prices['maxpri']);
		$cls_priceMin = $minpri[0];
		$cls_priceMax = $maxpri[0];
		$prm_priceMin = $minpri[1];
		$prm_priceMax = $maxpri[1];
		if($cls_priceMin > $prm_priceMin) {
			$startprice = $prm_priceMin;
		}else{
			$startprice = $cls_priceMin;
		}
		if($cls_priceMax > $prm_priceMax) {
			$endprice = $cls_priceMax;
		}else{
			$endprice = $prm_priceMax;
		}
	}
	$priceRng['startprice'] = $startprice;
	$priceRng['endprice'] = $endprice;
	expDebug::dPrint('price range--->>>'.print_r($priceRng,true),5);
	return $priceRng;
	}catch (Exception $ex) {
    watchdog_exception('getSliderPriceRange', $ex);
    expertusErrorThrow($ex);
  }
}

function displayNotificationDetailsAPI()
{
	$id = $_REQUEST["id"];
	$userid = $_REQUEST["userid"];
	try{
		$select = db_select('slt_notification','notify');
		$select->leftJoin('slt_notification_info', 'info', 'notify.msg_id = info.notification_code');
		$select->leftJoin('slt_notification_frame', 'frame', 'info.id = frame.notification_id');
		$select->addField('notify', 'token_str', 'token_str');
		$select->addField('frame', 'notification_template', 'notification_template');
		$select->condition('notify.id', $id, '=');
		$select->condition('notify.user_id', $userid, '=');
		$select->condition('frame.lang_code','cre_sys_lng_eng', '=');
		$results = $select->execute()->fetchAssoc();
		expDebug::dPrintDBAPI(' displayNotificationDetailsAPI $select Velu = ' ,$select);
		expDebug::dPrint(' displayNotificationDetailsAPI $results Velu = '.print_r($results,true),5);
		$output = base64_encode(generateNotificationDetailsFromToken($results['token_str'],$results['notification_template']));
		expDebug::dprint(' displayNotificationDetailsAPI $output = '.$output,4);
		if(empty($results)){
			$flag = "failure";
			$output ="Requested details not available.";
		}else{
			$flag = "success";
		}// end
		return array((object)array('id'=>$id,'status'=>$flag,'details'=>$output));
	}catch (Exception $ex) {
		watchdog_exception('displayNotificationDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}// end catch
}// end displayNotificationDetailsAPI

function generateNotificationDetailsFromToken($tokenstr,$notificationtemplate)
{
	try {
	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.module";
	expDebug::dprint(' generateNotificationDetailsFromToken $tokenstr = '.$tokenstr,4);
	expDebug::dprint(' generateNotificationDetailsFromToken $notificationtemplate = '.$notificationtemplate,4);
	$tokenSplit = array();
	if($tokenstr != null && $tokenstr != '')
	{
		$tokenArr=explode('~|',$tokenstr);
		for($i=0;$i<count($tokenArr);$i++)
		{
		$keyVal=explode('>|',$tokenArr[$i]);
		if($keyVal[0]!= 'Actions') {
		// If a date, use correct date format
		switch ($keyVal[0]) {
		case 'session_startdate':
					case 'session_enddate':
					case 'learning_dateend':
					case 'learning_datestart':
							if($keyVal[0] == 'session_startdate' && $type =='class_cancel'){
									$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); //Do not show time
									}
									else{
										$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, true, false, false); // Show time
									}
									break;
					case 'order_date':
					case 'tp_expirydate':
					case 'course_completed_date':
						$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); // Do not show time
						break;
					case 'order_refnumber': // ticket #0024165 - special characters in order comments cause mail to be incompletely rendered in outlook
						$keyVal[1] = check_plain($keyVal[1]);
						break;
		} // End switch

		$tokenSplit[$keyVal[0]]=(!empty($keyVal[1])) ? $keyVal[1] : ' ';

		if($keyVal[0]=='dtcd'){
			$dtCd=$keyVal[1];
		}
		}
		} // End for loop
	}
	$splitContent = explode('#',$notificationtemplate);
	expDebug::dPrint('exp_sp_notify.module : $splitContent = ' . print_r($splitContent, true) , 4);
	$exactTemplate = Array();
	for($i=0; $i<count($splitContent); $i++) {
		if(strlen($splitContent[$i]) > 2 && $splitContent[$i][0] == '@' &&  substr($splitContent[$i], -1) == '@') {
			$strTemplate = ereg_replace('@', '', $splitContent[$i]);
			if($strTemplate == 'group_data' || $strTemplate == 'group_discount_data' || $strTemplate == 'group_sessiondata' || $strTemplate == 'group_instructordata' || $strTemplate == 'group_tax_data') {
				$groupDataSplit = explode('~@', $tokenSplit[$strTemplate]);
				$newArrKey = Array();
				for($n = 0; $n < count($groupDataSplit); $n++) {
					$groupArr=explode('~~', $groupDataSplit[$n]);
					for($j = 0; $j < count($groupArr); $j++){
						$groupVal = explode('<|', $groupArr[$j]);
						$val1 = $groupVal[0];
						$val2 = $groupVal[1];
						expDebug::dPrint(' ($val1, $val2) = ("' . $val1 . '", "' . $val2 . '")' , 4);
						// If session_startdate or session_enddate, use the correct date format
						switch ($val1) {
							case 'session_startdate':
							case 'session_enddate':
								$val2 = dateTimeStrToUnivShort(trim($val2), true, false, false, false); // Don't show time
								break;
						}
						$newArrKey[$val1][] =  $val2;
					}
				}
				$arKey = array_keys($newArrKey);
				foreach($arKey as $valT){
					$tokenSplit[$valT] = implode("<br>", $newArrKey[$valT]); // Save new (token,value) pair in $tokenSplit
					$splitContent[$i] = $tokenSplit[$valT] ; // This does not seem getting used anywhere.
				}
			}

			if($tokenSplit[$strTemplate]) {
				if ($strTemplate !='group_data' && $strTemplate !='group_discount_data' && $strTemplate !='group_sessiondata' && $strTemplate != 'group_instructordata' && $strTemplate != 'group_tax_data'){
					$dispTemplate = $tokenSplit[$strTemplate];
				}
				else{
					$dispTemplate = '';
				}
			}else{ // $strTemplate does not have a value. Check if $strTemplate is to be processed in code.
				global $base_url;
				switch ($strTemplate) {
					case 'logo_path':
					    // $dispTemplate = '<img src="'.getContentUploadPath().'/notification_logo.png"></img>';
					    /**
					     * 0040106: The upload customer logo in the site should be used for the notification(amaran)
					     */
					    $currentDefaultThemePath = $base_url.base_path().drupal_get_path('theme',variable_get('theme_default', NULL));
						//$dispTemplate = '<img src="'.$currentDefaultThemePath.'/logo.png"></img>';
						break;
					case 'cmp_name':
						$dispTemplate = variable_get('site_name', 'ExpertusONE');
						break;
					case 'reply_to':
						$dispTemplate = $config["reply_to"];
						break;
// 					case 'site_url':
// 						$dispTemplate = '<a href="'.$base_url.'">'.variable_get('site_name', 'ExpertusONE').'</a>';
// 						break;
// 					case 'about_us':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=node/1">'.t('About Us').'</a>';
// 						break;
// 					case 'user_account':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/my-account">'.t('Account').'</a>';
// 						break;
// 					case 'my_learning':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('My Enrollment').'</a>';
// 						break;
// 					case 'req_new_pwd':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=user/password">'.t('Request new password').'</a>';
// 						break;
// 					case 'subject_name':
// 						$dispTemplate = '<a style="font-family:arial,helvetica,sans-serif" href="'.$base_url.'"></a>';
// 						break;
// 					case 'site_link':
// 						$dispTemplate = $base_url;
// 						break;
// 					case 'survey_logo_path':
// 						$dispTemplate = '<img border="0" src="'.getNotificationImagePath().'/survey_notification.png"></img>';
// 						break;
// 					case 'catalog_search':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/catalog-search">'.t('CATALOG').'</a>';
// 						break;
// 					case 'my_programs':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('LBL023').'</a>';
// 						break;
// 					case 'my_transcripts':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/enrollment-search">'.t('LBL009').'</a>';
// 						break;
// 					case 'my_certificates':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/my-profile">'.t('LBL877').'</a>';
// 						break;
// 					case 'my_team':
// 						$dispTemplate = '<a href="'.$base_url.'/?q=learning/myteam-search">'.t('My Team').'</a>';
// 						break;
					default: // Unrecognized token
						$dispTemplate = '';

				} // end switch
			}
		}
		else {
			$dispTemplate = $splitContent[$i];
		}
		expDebug::dPrint(' $dispTemplate after = ' . print_r($dispTemplate, true) , 4);
		$exactTemplate[] = $dispTemplate;
	} // loop ends
	expDebug::dPrint(' $exactTemplate = ' . print_r($exactTemplate, true) , 4);
	$notificationTemplateFinal = implode('',$exactTemplate);
	expDebug::dPrint(' $notificationTemplateFinal = ' . print_r($notificationTemplateFinal, true) , 4);
	return $notificationTemplateFinal;
	}catch (Exception $ex) {
		watchdog_exception('generateNotificationDetailsFromToken', $ex);
		expertusErrorThrow($ex);
	}
}

function getNotificationSubject($tokenstr,$notificationsubject)
{


	$tokenSplit = array();
if($tokenstr != null && $tokenstr != ''){
	$tokenArr=explode('~|',$tokenstr);
	for($i=0;$i<count($tokenArr);$i++) {
		$keyVal=explode('>|',$tokenArr[$i]);
		if($keyVal[0]!= 'Actions') {
			// If a date, use correct date format
			switch ($keyVal[0]) {

				case 'session_startdate':
				case 'session_enddate':
				case 'learning_dateend':
				case 'learning_datestart':
					if($keyVal[0] == 'session_startdate' && $type =='class_cancel'){
						$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); //Do not show time
					}
					else{
						$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, true, false, false); // Show time
					}
					break;
				case 'order_date':
				case 'tp_expirydate':
				case 'course_completed_date':
					$keyVal[1] = dateTimeStrToUnivShort(trim($keyVal[1]), true, false, false, false); // Do not show time
					break;
				case 'order_refnumber': // ticket #0024165 - special characters in order comments cause mail to be incompletely rendered in outlook
					$keyVal[1] = check_plain($keyVal[1]);
					break;
				case 'job_file_name':
					$fileName = $keyVal[1];
					break;
			} // End switch

			$tokenSplit[$keyVal[0]]=(!empty($keyVal[1])) ? $keyVal[1] : ' ';


			if($keyVal[0]=='dtcd'){
				$dtCd=$keyVal[1];
			}
		}
	} // End for loop
}


$splitSubject = explode('#',$notificationsubject);
$keywords = array();
$exactArr = Array();
for($i=0;$i<count($splitSubject);$i++) {
	if (strlen($splitSubject[$i]) > 2 && $splitSubject[$i][0] == '@' &&  substr($splitSubject[$i], -1) == '@') {
		$str = ereg_replace('@','',$splitSubject[$i]);
		$keywords[] = ereg_replace('@','',$splitSubject[$i]);
		$dispSubject = $tokenSplit[$str];
		if(!$tokenSplit[$str]){
			if($str == 'cmp_name'){
				$dispSubject = variable_get('site_name', 'ExpertusONE');
			}
			else {
				$dispSubject = '-';
			}
		}
	}else {
		$dispSubject = $splitSubject[$i];
	}
	$exactArr[] = $dispSubject;
}
$notificationsubjectFinal = implode('',$exactArr);
expDebug::dPrint('Notification subject ajayyyyyy= ' .$notificationsubjectFinal , 4);
return $notificationsubjectFinal;

}

/*
 * getClassEnrollmentIdForCourseLevel() - returns enrollment id for a class from slt_enrollment table
*                                                           if a course have only one class
*/
function getClassEnrollmentIdForCourseLevel($courseId, $classId) {
	try{
		// Select table
		$selectStmt = db_select('slt_enrollment', 'enr');
		// Set conditions
		$selectStmt->condition('enr.course_id', $courseId);
		$selectStmt->condition('enr.class_id', $classId);
		$selectStmt->condition('enr.reg_status', 'lrn_crs_reg_cnf', '=');
		// Select fields and/or add expressions
		$selectStmt->addField('enr', 'id');
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute()->fetchField();
		expDebug::dPrint(' $result $compStatusCount = ' .$result , 4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getClassEnrollmentCountForRegistrationStatuses', $ex);
		expertusErrorThrow($ex);
	}
}

function setSessionAicc($type,$aicc_sid = 0){
	if($type=="set"){
		$current_session = session_id();
		$enc1 = new Encrypt();
		$aicc_sid = $enc1->decrypt($aicc_sid);
		$select = db_select('slt_aicc_interaction','aicc');
		$select->addField('aicc','param', 'param');
		$select->condition('aicc_id',$aicc_sid,'=');
		expDebug::dPrintDBAPI('select',$select);
		$param = $select->execute()->fetchField();
		expDebug::dPrint('param -->> '.$param);
		// Delete any previous entry if any
		$delete = db_delete('slt_aicc_interaction');
		$delete->condition('aicc_id',$aicc_sid,'=');
		$delete->condition('session_id',$current_session,'=');
		$delete->execute();
		// Insert session session details
		$select = db_insert('slt_aicc_interaction');
		$select->fields(array(
				'aicc_id'=>$aicc_sid,
				'session_id'=>$current_session,
				'param' => $param,
				'connection_status'=>'N',
				'created_by'=>getIdOfLoggedInUser(),
				'created_on'=>now()
		));
		expDebug::dPrintDBAPI('Session id store while launch AICC -->',$select);
		$rtn = $select->execute();
		
		$sid = explode("-",$aicc_sid);
		expDebug::dPrint("sid test".print_r($sid,true),5);
		$select = db_select('slt_person','per');
		$select->addField('per','full_name', 'full_name');
		$select->addField('per','email', 'email');
		$select->condition('per.id',$sid[0],'=');
		expDebug::dPrintDBAPI('select',$select);
		$personDetails = $select->execute()->fetchObject();
		
		expDebug::dPrint("personDetails >>>".print_r($personDetails,true),5);
		
		$enc2 = new Encrypt();
		$aicc_sid = $enc2->encrypt($rtn);
		
		return drupal_json_output(array('session_id'=>$current_session.'-E1-'.$aicc_sid,'name'=>$personDetails->full_name,'email'=>$personDetails->email));
	}else{
		return drupal_json_output(array('session_id'=>session_id()));
	}
}
function update_learningplan_completedbydate(){
	try{
		$curr_date       = date("Y-m-d");
		$updateStmt = db_update('slt_program');
		$updateStmt->condition('object_type','cre_sys_obt_trn','=');
		$updateStmt->condition('end_date',$curr_date,'<');
		$updateStmt->fields(array('status' => 'lrn_lpn_sts_dld','updated_by' => 1,'updated_on' => now()));
		$incomplete_updated = $updateStmt->execute();
	}catch (Exception $ex) {
		watchdog_exception('update_learningplan_completedbydate', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares iCalendar event alternate html description
 */
function prepareICalendarVEventDescription($classSessionInfo, $enrLevel, $prefLang,$iCalArr=null) {
	try {
		//global $catalog_reg;
		//t('Canceled', array(), array('langcode' => $prefLang))
		$organizer = variable_get('site_name', 'ExpertusONE');
		$description = '';
                //Rajeshwar
                $lang_select=  db_select('slt_profile_list_items','lit');
                $lang_select->addField('lit','name','lang_name');
		if ($enrLevel == 'TP') {
			$description .= t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ': ' . $classSessionInfo->prg_title . '\n\n'; // Here single quote is needed for \n
			$description .= t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ' ' .
					t('LBL096', array(), array('langcode' => $prefLang)) . ': ' . $classSessionInfo->prg_code . '\n\n'; //Code
			$description .= t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ' ' .
					t('LBL038', array(), array('langcode' => $prefLang)) . ': ' . t($classSessionInfo->prg_language, array(), array('langcode' => $prefLang)) . '\n\n'; //Language
		}
		/* if ($catalog_reg == 'Course') {
		 $description .= "Course: " . $classSessionInfo->crs_title . '\n\n';
		$description .= "Course Code: " . $classSessionInfo->crs_code . '\n\n';
		$description .= "Course Language: " . $classSessionInfo->crs_language . '\n\n';
		}
		else { */
		if($iCalArr==null){
				$description .= t('Class', array(), array('langcode' => $prefLang)) . ": " .  $classSessionInfo->cls_title .  '\n\n'; //Class
				$description .= t('LBL263', array(), array('langcode' => $prefLang)) . ": " . $classSessionInfo->cls_code . '\n\n'; //Class Code
				$description .= t('Class', array(), array('langcode' => $prefLang)) . ' ' . t('LBL038', array(), array('langcode' => $prefLang)) . ": " .
						t($classSessionInfo->cls_language, array(), array('langcode' => $prefLang)) . '\n\n'; //Class, Language
				//}
				$description .= t('LBL250', array(), array('langcode' => $prefLang)) . ": " . $classSessionInfo->session_title . '\n\n'; //Session
				$description .= t('Instructor', array(), array('langcode' => $prefLang)) . ": " . $classSessionInfo->instructor_name . '\n\n'; //Instructor
		}else{
			$classInstructorInfo = $iCalArr['classInstructorInfo'];
			$classInfo = $iCalArr['classInfo'];
                        $lang_select->condition('code',$classInfo['lang_code'],'=');
                        $res=$lang_select->execute()->fetchField();
                        expDebug::dPrintDBAPI('$lang_select', $lang_select);
			$description .= t('Class', array(), array('langcode' => $prefLang)) . ": " .  $classInfo['title'] .  '\n\n'; //Class
			$description .= t('LBL263', array(), array('langcode' => $prefLang)) . ": " . $classInfo['code'] . '\n\n'; //Class Code
			$description .= t('Class', array(), array('langcode' => $prefLang)) . ' ' . t('LBL038', array(), array('langcode' => $prefLang)) . ": " .
					t($res, array(), array('langcode' => $prefLang)) . '\n\n'; //Class, Language
			//}
			$description .= t('LBL250', array(), array('langcode' => $prefLang)) . ": " . $classSessionInfo->title . '\n\n'; //Session
			$description .= t('Instructor', array(), array('langcode' => $prefLang)) . ": " . $classInstructorInfo['first_name'] . '\n\n'; //Instructor
		}
		//$description .= '\n\n' . t('The', array(), array('langcode' => $prefLang)) . " " . $organizer . " " . t('Team', array(), array('langcode' => $prefLang));
		//change by ayyappans for 37494: Issue in Ical when receving notiifcation location details not clearly showing in the body of the message
		if(isset($classSessionInfo->location_name) && $classSessionInfo->location_name != '') {
			$description .= t('Location', array(), array('langcode' => $prefLang)) . ": " . $classSessionInfo->location_name . '\n\n';
			if(!empty($classSessionInfo->addr1)) {
				$description .= $classSessionInfo->addr1 . '\n\n';
			}
			if(!empty($classSessionInfo->addr2)) {
				$description .= $classSessionInfo->addr2 . '\n\n';
			}
			$description .= $classSessionInfo->city . '\n\n';
			$description .= $classSessionInfo->state . ' ' . $classSessionInfo->zipcode . '\n\n';
			$description .= $classSessionInfo->country . '\n\n';
		}

		expDebug::dPrint('$description = ' . $description, 4);
		return $description;
	}
	catch (Exception $ex) {
		watchdog_exception('prepareICalendarVEventDescription', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares iCalendar event alternate html description
 * @TODO: translate all text to enrolled user's preferred language
 */
function prepareICalendarVEventAltDescription($classSessionInfo, $enrLevel, $prefLang,$iCalArr = null) {
	try {
		//global $catalog_reg;
		global $base_url;
                $lang_select=  db_select('slt_profile_list_items','lit');
                $lang_select->addField('lit','name','lang_name');
		$organizer = variable_get('site_name', 'ExpertusONE');
		$altDescription = '<html>' .
				'<head>' .
				'<title>' . t('LBL037', array(), array('langcode' => $prefLang)) . '</title>' . //Training
				'</head>' .
				'<body>' .
				'<table>';

		if ($enrLevel == 'TP') {
			$altDescription .=    '<tr>' .
					'<td>' . t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ':</td>' .
					'<td>' . $classSessionInfo->prg_title . '</td>' .
					'</tr>' .
					'<tr>' .
					'<td>' . t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ' ' . t('LBL096', array(), array('langcode' => $prefLang)) . ':</td>' . //Code
					'<td>' . $classSessionInfo->prg_code . '</td>' .
					'</tr>' .
					'<tr>' .
					'<td>' . t($classSessionInfo->prg_type_name, array(), array('langcode' => $prefLang)) . ' ' . t('LBL038', array(), array('langcode' => $prefLang)) . ':</td>' . //Language
					'<td>' . t($classSessionInfo->prg_language, array(), array('langcode' => $prefLang)) . '</td>' .
					'</tr>';
		}

		/*if ($catalog_reg == 'Course') {
		 $altDescription .=    '<tr>' .
		'<td>' . t('Course') . ':</td>' .
		'<td>' . $classSessionInfo->crs_title . '</td>' .
		'</tr>' .
		'<tr>' .
		'<td>' . t('Course Code') . ':</td>' .
		'<td>' . $classSessionInfo->crs_code . '</td>' .
		'</tr>' .
		'<tr>' .
		'<td>' . t('Course Language') . ':</td>' .
		'<td>' . $classSessionInfo->crs_language . '</td>' .
		'</tr>';
		}
		else {*/
		if($iCalArr==null){
				$altDescription .=    '<tr>' .
						'<td>' . t('Class', array(), array('langcode' => $prefLang)) . ':</td>' . //Class
						'<td>' . $classSessionInfo->cls_title . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('LBL263', array(), array('langcode' => $prefLang)) . ':</td>' . //Class Code
						'<td>' . $classSessionInfo->cls_code . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('Class', array(), array('langcode' => $prefLang)) . '&nbsp;' . t('LBL038', array(), array('langcode' => $prefLang)) . ':</td>' . //Language
						'<td>' . t($classSessionInfo->cls_language, array(), array('langcode' => $prefLang)) . '</td>' .
						'</tr>';
				//}
				$altDescription .=      '<tr>' .
						'<td>' . t('LBL250', array(), array('langcode' => $prefLang)) . ':</td>' . //Session
						'<td>' . $classSessionInfo->session_title . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('Instructor', array(), array('langcode' => $prefLang)) . ':</td>' . //Instructor
						'<td>' . $classSessionInfo->instructor_name . '</td>' .
						'</tr>';
		}else{
				$classInstructorInfo = $iCalArr['classInstructorInfo'];
				$classInfo = $iCalArr['classInfo'];
				$lang_select->condition('code',$classInfo['lang_code'],'=');
                                $res=$lang_select->execute()->fetchField();
                                expDebug::dPrintDBAPI('$lang_select', $lang_select);
                                expDebug::dPrint('Raj3 $res = ' . print_r($res,1), 4);
				$altDescription .=    '<tr>' .
						'<td>' . t('Class', array(), array('langcode' => $prefLang)) . ':</td>' . //Class
						'<td>' . $classInfo['cls_title'] . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('LBL263', array(), array('langcode' => $prefLang)) . ':</td>' . //Class Code
						'<td>' . $classInfo['code'] . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('Class', array(), array('langcode' => $prefLang)) . '&nbsp;' . t('LBL038', array(), array('langcode' => $prefLang)) . ':</td>' . //Language
						'<td>' . t($res, array(), array('langcode' => $prefLang)) . '</td>' .
						'</tr>';
				//}
				$altDescription .=      '<tr>' .
						'<td>' . t('LBL250', array(), array('langcode' => $prefLang)) . ':</td>' . //Session
						'<td>' . $classSessionInfo->title . '</td>' .
						'</tr>' .
						'<tr>' .
						'<td>' . t('Instructor', array(), array('langcode' => $prefLang)) . ':</td>' . //Instructor
						'<td>' . $classInstructorInfo['first_name'] . '</td>' .
						'</tr>';
		}
				/* '<tr>' .
				 '<td>' . t('LBL038', array(), array('langcode' => $prefLang)) . ':</td>' . //Language
		'<td>' . t($classSessionInfo->cls_language, array(), array('langcode' => $prefLang)) . '</td>' .
		'</tr>' . */
		//change by ayyappans for 37494: Issue in Ical when receving notiifcation location details not clearly showing in the body of the message
		if(isset($classSessionInfo->location_name) && $classSessionInfo->location_name != '') {
			$altDescription .= '<tr>' .
				'<td>' . t('Location', array(), array('langcode' => $prefLang)) . ":</td>" .
				'<td>' . $classSessionInfo->location_name . '</td>' .
				'</tr>';
			if(!empty($classSessionInfo->addr1)) {
				$altDescription .= '<tr>' .
				'<td>' . $classSessionInfo->addr1 . '</td>' .
				'</tr>';
			}
			if(!empty($classSessionInfo->addr2)) {
				$altDescription .= '<tr>' .
				'<td>' . $classSessionInfo->addr2 . '</td>' .
				'</tr>';
			}
			$altDescription .= '<tr>' .
					'<td>' . $classSessionInfo->city . '</td>' .
					'</tr>';
			$altDescription .= '<tr>' .
					'<td>' . $classSessionInfo->state . ' ' . $classSessionInfo->zipcode . '</td>' .
					'</tr>';
			$altDescription .= '<tr>' .
					'<td>' . $classSessionInfo->country . '</td>' .
					'</tr>';
		}
		$altDescription .= '</table>' .
		/* '<br/>' .
		 '<br/>' .
		t('The', array(), array('langcode' => $prefLang)) . '&nbsp;' . $organizer . '&nbsp;' . t('Team', array(), array('langcode' => $prefLang)) .
		'<br/>' .
		'<a href="' . $base_url . '">' . $organizer . '</a>' . */
		'</body>' .
		'</html>';

		expDebug::dPrint('$altDescription = ' . print_r($altDescription, true), 4);
		return $altDescription;
	}
	catch (Exception $ex) {
		watchdog_exception('prepareICalendarVEventAltDescription', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares Publish iCalendar vevents text
 */
function getPublishICalEvents($enrId, $enrLevel, $tpClassId = null) {
	try {
		//global $catalog_reg;

		$classSessionsInfoList = getClassSessionDetailsForICal($enrId, $enrLevel, $tpClassId);
		if (empty($classSessionsInfoList)) {
			return null;
		}

		// Get enrolled users preferred language (drupal code)
		$prefLang = enrolledUserPreferredLang($enrId, $enrType);

		$mastEnrId = ($enrLevel == 'TP')? $enrId : 0;

		$organizerEmail =  variable_get('site_mail', 'info@expertusone.com');
		$organizerName =  variable_get('site_name', 'ExpertusONE');
		$vEventsArr = array();
		foreach ($classSessionsInfoList as $classSessionInfo) {
			if ($enrLevel == 'TP') {
				$enrId = $classSessionInfo->enr_id;
			}

			// Generate event UID from TP enrollment id, Class enrollment id and Session id
			$eventUID = 'ExpertusONE_' . $mastEnrId . '_' . $enrId . '_' . $classSessionInfo->session_id . '_' . $classSessionInfo->seq . '@' . $_SERVER['SERVER_NAME'];

			// Prepare event description and x-alt-desc
			$description = prepareICalendarVEventDescription($classSessionInfo, $enrLevel, $prefLang);
			$altDescription = prepareICalendarVEventAltDescription($classSessionInfo, $enrLevel, $prefLang);

			// Prepare event summary
			$classTitle = $classSessionInfo->cls_title;
			//if ($catalog_reg == 'Course') {
			//	$classTitle = $classSessionInfo->crs_title;
			//}
			if ($enrLevel == 'TP') {
				$summary = $classTitle . ' - ' . $classSessionInfo->session_title . ' (' . t($classSessionInfo->cls_type_name, array(), array('langcode' => $prefLang)) . ')';
			}
			else {
				$summary = $classTitle . ((count($classSessionsInfoList) <= 1)? "" : " -  $classSessionInfo->session_title");
			}
			$utcSystemDatetime = date_format(date_now('UTC'), 'Ymd\THis\Z');
			list($utcStartDatetime, $utcEndDatetime) = prepareICalEventDateTime($classSessionInfo->start_date, $classSessionInfo->start_time,
					$classSessionInfo->end_time, $classSessionInfo->session_php_timezone);
			$vEvent  = "BEGIN:VEVENT\r\n";
			$vEvent .= "ORGANIZER;CN=" . $organizerName . ":MAILTO:" . $organizerEmail . "\r\n"; //ORGANIZER;CN=Me@ExpertusGMAIL:mailto:sunilkumarg@expertus.com
			$vEvent .= "DTSTART:" . $utcStartDatetime . "\r\n";
			$vEvent .= "DTEND:" . $utcEndDatetime . "\r\n";
			$vEvent .= "LOCATION:" . $classSessionInfo->location_name . "\r\n";
			$vEvent .= "TRANSP:OPAQUE\r\n";
			$vEvent .= "UID:" . $eventUID . "\r\n";
			$vEvent .= "X-ALT-DESC;FMTTYPE=text/html:" . $altDescription . "\r\n";
			$vEvent .= "SEQUENCE:" . $classSessionInfo->seq . "\r\n"; // revision number
			$vEvent .= "SUMMARY:" . $summary . "\r\n";
			$vEvent .= "DESCRIPTION:" . $description . "\r\n";
			$vEvent .= "CREATED:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "LAST-MODIFIED:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "DTSTAMP:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "PRIORITY:5\r\n";
			$vEvent .= "CLASS:PUBLIC\r\n";
			$vEvent .= "STATUS:CONFIRMED\r\n";
			$vEvent .= "ATTENDEE;CUTYPE=INDIVIDUAL;ROLE=REQ-PARTICIPANT;PARTSTAT=ACCEPTED:mailto:" . $classSessionInfo->lnr_email . "\r\n"; // http://webapps.stackexchange.com/questions/31747/google-calendar-auto-adding-ics-events
			$vEvent .= "X-MICROSOFT-CDO-BUSYSTATUS:BUSY\r\n";
			$vEvent .= "X-MICROSOFT-CDO-IMPORTANCE:1\r\n";
			$vEvent .= "X-MICROSOFT-CDO-INTENDEDSTATUS:BUSY\r\n";
			$vEvent .= "X-MICROSOFT-DISALLOW-COUNTER:TRUE\r\n";
			$vEvent .= "X-MS-OLK-AUTOSTARTCHECK:FALSE\r\n";
			$vEvent .= "X-MS-OLK-AUTOFILLLOCATION:FALSE\n"; // http://www.how2guru.com/archives/php-generate-calendar-file-ics/
			$vEvent .= "X-MS-OLK-CONFTYPE:0\r\n";
			$vEvent .= "BEGIN:VALARM\r\n";
			$vEvent .= "TRIGGER:-PT15M\r\n";
			$vEvent .= "ACTION:DISPLAY\r\n";
			$vEvent .= "DESCRIPTION:Reminder\r\n";
			$vEvent .= "END:VALARM\r\n";
			$vEvent .= "END:VEVENT\r\n";
			$vEventsArr[] = $vEvent;
		} // end foeach

		expDebug::dPrint('$vEventsArr = ' . print_r($vEventsArr, true), 4);
		return $vEventsArr;
	}
	catch (Exception $ex) {
		watchdog_exception('getPublishICalEvents', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares Publish iCalendar file text for an enrollment
 * iCalendar Validators:
 *   most basic:  http://severinghaus.org/projects/icv/?url=
 *   better:      http://icalvalid.cloudapp.net/Default.aspx
 *   most strict: http://arnout.engelen.eu/icalendar-validator
 * Useful links:
 * http://stackoverflow.com/questions/1823647/grouping-multiple-events-in-a-single-ics-file-icalendar-stream
 */
function getPublishICal($enrId, $enrLevel, $tpClassId = null,$iCalWholeArr=null) {
	try {
		$iCalArr = array();
		if($iCalWholeArr===null)
			$vEventsArr = getPublishICalEvents($enrId, $enrLevel, $tpClassId);
		else
			$vEventsArr = getPublishICalEventsForInstructor($iCalWholeArr);
		if (empty($vEventsArr)) {
			return null;
		}
		
			$iCal  = "BEGIN:VCALENDAR\r\n";
			$iCal .= "VERSION:2.0\r\n";
			$iCal .= "PRODID:-//ExpertusONE-" . $_SERVER['SERVER_NAME'] . "//NONSGML ExpertusONE//EN\r\n";
			$iCal .= "X-MS-OLK-FORCEINSPECTOROPEN:TRUE\r\n";
			$iCal .= "CALSCALE:GREGORIAN\r\n";
			$iCal .= "METHOD:REQUEST\r\n";	//34640: when changing the class Ical event is adding but existing event not removing. Root cause: In Google Calendar, Publish events gets cancelled only by importing the calendar in settings page. Fix: Event methos changed to Request instead of Publish
			foreach ($vEventsArr as $vEvent) {
				$iCal .= $vEvent;
			}
			$iCal .= "END:VCALENDAR\r\n";
			expDebug::dPrint('$iCal = ' . $iCal, 4);
			$iCalWithType = new StdClass();
			$iCalWithType->type = 'publish';
			$iCalWithType->ical = base64_encode($iCal);
			expDebug::dPrint('$iCalWithType = ' . print_r($iCalWithType, true), 4);
			$iCalArr[] = $iCalWithType;
		
		expDebug::dPrint('$iCalArr = ' . print_r($iCalArr, true), 4);

		$iCalObj = (object) $iCalArr;
		expDebug::dPrint('$iCalObj = ' . print_r($iCalObj, true), 4);

		return serialize($iCalObj);
	}
	catch (Exception $ex) {
		watchdog_exception('getPublishICal', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares iCalendar e-mail attachment(s) text for enrollment cancel
 */
function getCancelICal($enrId, $enrLevel, $tpClassId = null,$iCalWholeArr=null,$callFrom = null) {
	try {
		$iCalArr = array();
		if($iCalWholeArr===null)
				$vEventsArr = getCancelICalEvents($enrId, $enrLevel, $tpClassId);
		else
			  $vEventsArr = getCancelICalEventsForInstructor($iCalWholeArr,$callFrom);
		if (empty($vEventsArr)) {
			return null;
		}
		
			$iCal  = "BEGIN:VCALENDAR\r\n";
			$iCal .= "VERSION:2.0\r\n";
			$iCal .= "PRODID:-//ExpertusONE-" . $_SERVER['SERVER_NAME'] . "//NONSGML ExpertusONE//EN\r\n";
			$iCal .= "X-MS-OLK-FORCEINSPECTOROPEN:TRUE\r\n";
			$iCal .= "CALSCALE:GREGORIAN\r\n"; //
			$iCal .= "METHOD:CANCEL\r\n"; // mac mail/cal expects METHOD:PUBLISH in event cancel vcalendar, however this breaks google calendar single event
			foreach ($vEventsArr as $vEvent) {
				$iCal .= $vEvent;
			} //end foreach
			$iCal .= "END:VCALENDAR\r\n";
			expDebug::dPrint('$iCal = ' . $iCal, 4);
			$iCalWithType = new StdClass();
			$iCalWithType->type = 'cancel';
			$iCalWithType->ical = base64_encode($iCal);
			expDebug::dPrint('$iCalWithType = ' . print_r($iCalWithType, true), 4);
			$iCalArr[] = $iCalWithType;
		
		expDebug::dPrint('$iCalArr = ' . print_r($iCalArr, true), 4);

		$iCalObj = (object) $iCalArr;
		expDebug::dPrint('$iCalObj = ' . print_r($iCalObj, true), 4);

		return serialize($iCalObj);
	}
	catch (Exception $ex) {
		watchdog_exception('getCancelICal', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares iCalendar cancelled vevents text for an enrollment
 */
function getCancelICalEvents($enrId, $enrLevel, $tpClassId = null) {
	try {
		//global $catalog_reg;

		$classSessionsInfoList = getClassSessionDetailsForICal($enrId, $enrLevel, $tpClassId);
		if (empty($classSessionsInfoList)) {
			return null;
		}

		// Get enrolled users preferred language (drupal code)
		$prefLang = enrolledUserPreferredLang($enrId, $enrType);

		$mastEnrId = ($enrLevel == 'TP')? $enrId : 0;

		$organizerEmail =  variable_get('site_mail', 'info@expertusone.com');
		$organizerName =  variable_get('site_name', 'ExpertusONE');
		$vEventsArr = array();
		foreach ($classSessionsInfoList as $classSessionInfo) {
			if ($enrLevel == 'TP') {
				$enrId = $classSessionInfo->enr_id;
			}

			// Generate event UID from TP enrollment id, Class enrollment id and Session id
			$eventUID = 'ExpertusONE_' . $mastEnrId . '_' . $enrId . '_' . $classSessionInfo->session_id . '_' . ($classSessionInfo->seq - 1) . '@' . $_SERVER['SERVER_NAME'];

			// Prepare event summary
			$classTitle = $classSessionInfo->cls_title;
			//if ($catalog_reg == 'Course') {
			//	$classTitle = $classSessionInfo->crs_title;
			//}
			$summary = t('Canceled', array(), array('langcode' => $prefLang)) . ': ';
			if ($enrLevel == 'TP') {
				$summary .= $classTitle . ' - ' . $classSessionInfo->session_title . ' (' . t($classSessionInfo->cls_type_name, array(), array('langcode' => $prefLang)) . ')';
			}
			else {
				$summary .= $classTitle . ((count($classSessionsInfoList) <= 1)? "" : " -  $classSessionInfo->session_title");
			}
			$utcSystemDatetime = date_format(date_now('UTC'), 'Ymd\THis\Z');
			list($utcStartDatetime, $utcEndDatetime) = prepareICalEventDateTime($classSessionInfo->start_date, $classSessionInfo->start_time,
					$classSessionInfo->end_time, $classSessionInfo->session_php_timezone);
			$vEvent = '';
			$vEvent .= "BEGIN:VEVENT\r\n";
			$vEvent .= "DTSTART:".$utcStartDatetime."\r\n";
			$vEvent .= "DTEND:" . $utcEndDatetime . "\r\n";
			$vEvent .= "ORGANIZER;CN=" . $organizerName . ":MAILTO:" . $organizerEmail . "\r\n"; //ORGANIZER;CN=Me@ExpertusGMAIL:mailto:sunilkumarg@expertus.com
			$vEvent .= "ATTENDEE;CUTYPE=INDIVIDUAL;ROLE=REQ-PARTICIPANT;PARTSTAT=ACCEPTED:mailto:" . $classSessionInfo->lnr_email . "\r\n";
			$vEvent .= "UID:" . $eventUID . "\r\n";
			$vEvent .= "SEQUENCE:" . $classSessionInfo->seq . "\r\n"; // revision number
			$vEvent .= "PRIORITY:1\r\n"; // highest priority
			$vEvent .= "STATUS:CANCELLED\r\n";
			$vEvent .= "SUMMARY: " . $summary . "\r\n";
			$vEvent .= "TRANSP:TRANSPARENT\r\n";
			$vEvent .= "CLASS:PUBLIC\r\n";
			$vEvent .= "X-MICROSOFT-CDO-BUSYSTATUS:FREE\r\n";
			$vEvent .= "X-MICROSOFT-CDO-IMPORTANCE:2\r\n";
			$vEvent .= "X-MICROSOFT-DISALLOW-COUNTER:TRUE\r\n";
			$vEvent .= "X-MS-OLK-AUTOSTARTCHECK:FALSE\r\n";
			$vEvent .= "X-MS-OLK-AUTOFILLLOCATION:FALSE\n"; // http://www.how2guru.com/archives/php-generate-calendar-file-ics/
			$vEvent .= "X-MS-OLK-CONFTYPE:0\r\n";
			$vEvent .= "X-MS-OLK-APPTSEQTIME:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "DTSTAMP:" . $utcSystemDatetime . "\r\n";
			$vEvent .= "END:VEVENT\r\n";
			$vEventsArr[] = $vEvent;
		} // end foeach
		expDebug::dPrint('$vEventsArr = ' . print_r($vEventsArr, true), 4);

		return $vEventsArr;
	}
	catch (Exception $ex) {
		watchdog_exception('getCancelICalEvents', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Prepares session start and end date time in UTC for iCalendar VEvent
 */
function prepareICalEventDateTime($date, $startTime, $endTime, $tz) {
	try {
		expDebug::dPrint('$date = ' . print_r($date, true), 4);
		expDebug::dPrint('$startTime = ' . print_r($startTime, true), 4);
		expDebug::dPrint('$endTime = ' . print_r($endTime, true), 4);
		expDebug::dPrint('$tz = ' . print_r($tz, true), 4);

		// Build start and end date time strings
		$splitDate = explode(' ', $date);
		expDebug::dPrint('$splitDate = ' . print_r($splitDate, true), 4);
		$startDateTime = $splitDate[0] . ' ' . $startTime . ':00';
		$endDateTime = $splitDate[0] . ' ' . $endTime . ':00';
		expDebug::dPrint('$startDateTime = ' . print_r($startDateTime, true), 4);
		expDebug::dPrint('$endDateTime = ' . print_r($endDateTime, true), 4);

		// Create datetime object in class timezone
		$startDateTimeObj = new DateObject($startDateTime, $tz, 'Y-m-d H:i:s');
		$endDateTimeObj = new DateObject($endDateTime, $tz, 'Y-m-d H:i:s');

		// Change timezone to UTC
		$startDateTimeObj->setTimezone(new DateTimeZone('UTC'));
		$endDateTimeObj->setTimezone(new DateTimeZone('UTC'));

		// Format and return the date time in format suitable for iCalendar event
		$startDateTime = date_format($startDateTimeObj, 'Ymd\THis\Z');
		$endDateTime = date_format($endDateTimeObj, 'Ymd\THis\Z');
		expDebug::dPrint('$startDateTime = ' . print_r($startDateTime, true), 4);
		expDebug::dPrint('$endDateTime = ' . print_r($endDateTime, true), 4);

		return array($startDateTime, $endDateTime);
	}
	catch (Exception $ex) {
		watchdog_exception('prepareICalEventDateTime', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Fetches enrolled ILT or VC class session details, including required TP, course and class details, for iCalendar events file
 */
function getClassSessionDetailsForICal($enrId, $enrLevel, $tpClassId = null) {
	try {

		$txn = db_transaction();
		try {
			// Select table
			$selectStmt = db_select('slt_enrollment', 'enr');
			$selectStmt->forUpdate(true);

			// Join other tables.
			if ($enrLevel == 'TP') {
				$selectStmt->leftJoin('slt_master_enrollment', 'mast_enr', 'enr.master_enrollment_id = mast_enr.id');
				$selectStmt->leftJoin('slt_program', 'prg', 'mast_enr.program_id = prg.id');
				$selectStmt->leftjoin('slt_profile_list_items', 'spli_lang_prg', 'prg.lang_code = spli_lang_prg.code');
				$selectStmt->leftjoin('slt_profile_list_items', 'spli_tp_type', 'prg.object_type = spli_tp_type.code');
			}

			$selectStmt->leftJoin('slt_course_class', 'cls', 'enr.class_id = cls.id');
			$selectStmt->leftjoin('slt_profile_list_items', 'spli_lang_cls', 'cls.lang_code = spli_lang_cls.code');
			if ($enrLevel == 'TP') {
				$selectStmt->leftjoin('slt_profile_list_items', 'spli_cls_type', 'cls.delivery_type = spli_cls_type.code');
			}
			$selectStmt->leftJoin('slt_course_template', 'crs', 'enr.course_id = crs.id');
			$selectStmt->leftjoin('slt_profile_list_items', 'spli_lang_crs', 'crs.lang_code = spli_lang_crs.code');
			$joinCondition = 'enr.class_id = ses.class_id && enr.course_id = ses.course_id';
			if ($enrLevel == 'TP') {
				$joinCondition = 'cls.delivery_type in (\'lrn_cls_dty_ilt\', \'lrn_cls_dty_vcl\') && ' . $joinCondition;
			}
			$selectStmt->leftJoin('slt_course_class_session', 'ses', $joinCondition);
			$selectStmt->leftJoin('slt_session_instructor_details', 'ins_det', 'ins_det.session_id = ses.id');
			$selectStmt->leftjoin('slt_profile_list_items', 'spli_tz', 'ses.timezone is not null AND ses.timezone = spli_tz.code');
			$selectStmt->leftJoin('slt_location', 'loc', 'ses.location_id is not null AND ses.location_id = loc.id');
			$selectStmt->leftJoin('slt_person', 'inst', 'ins_det.instructor_id is not null AND ins_det.instructor_id = inst.id');
			$selectStmt->leftJoin('slt_person', 'per', 'enr.user_id = per.id');
			$selectStmt->leftJoin('slt_country', 'country', 'loc.country = country.country_code');
			$selectStmt->leftJoin('slt_state', 'state', 'country.country_code = state.country_code &&  loc.state = state.state_code');

			// Set conditions
			if ($enrLevel == 'TP') {
				$selectStmt->condition('enr.master_enrollment_id', $enrId);
				$selectStmt->condition('cls.delivery_type', array('lrn_cls_dty_ilt', 'lrn_cls_dty_vcl'), 'IN');
				if (!empty($tpClassId)) {
					$selectStmt->condition('enr.class_id', $tpClassId, '=');
				}
			}
			else {
				$selectStmt->condition('enr.id', $enrId);
			}

			// Select fields and/or add expressions
			if ($enrLevel == 'TP') {
				// Enrollement id
				$selectStmt->addField('enr', 'id', 'enr_id');
				// TP Details
				$selectStmt->addField('prg', 'title', 'prg_title');
				$selectStmt->addField('prg', 'code', 'prg_code');
				//$selectStmt->addField('prg', 'short_description', 'prg_description');
				$selectStmt->addField('spli_lang_prg', 'name', 'prg_language');
				$selectStmt->addField('spli_tp_type', 'name', 'prg_type_name');
			}

			// Class details
			$selectStmt->addField('cls', 'title', 'cls_title');
			$selectStmt->addField('cls', 'code', 'cls_code');
			//$selectStmt->addField('cls', 'short_description', 'cls_description');
			$selectStmt->addField('spli_lang_cls', 'name', 'cls_language');
			if ($enrLevel == 'TP') {
				$selectStmt->addField('spli_cls_type', 'name', 'cls_type_name');
			}

			// Course details
			$selectStmt->addField('crs', 'title', 'crs_title');
			$selectStmt->addField('crs', 'code', 'crs_code');
			//$selectStmt->addField('crs', 'short_description', 'crs_description');
			$selectStmt->addField('spli_lang_crs', 'name', 'crs_language');

			// Session details
			$selectStmt->addField('ses', 'id', 'session_id');
			$selectStmt->addField('ses', 'title', 'session_title');
			$selectStmt->addField('ses', 'start_date', 'start_date');
			$selectStmt->addField('ses', 'start_time', 'start_time');
			//$selectStmt->addField('ses', 'end_date', 'end_date');
			$selectStmt->addField('ses', 'end_time', 'end_time');
			$selectStmt->addField('spli_tz', 'attr2', 'session_php_timezone');
			$selectStmt->addField('ses', 'capacity_max', 'capacity_max');
			$selectStmt->addField('loc', 'name', 'location_name');
			$selectStmt->addField('loc', 'addr1', 'addr1');
			$selectStmt->addField('loc', 'addr2', 'addr2');
			$selectStmt->addField('loc', 'city', 'city');
			$selectStmt->addField('state', 'state_name', 'state');
			$selectStmt->addField('country', 'country_name', 'country');
			$selectStmt->addField('loc', 'zipcode', 'zipcode');
			$selectStmt->addField('inst', 'full_name', 'instructor_name');
			$selectStmt->addField('per', 'email', 'lnr_email');
			expDebug::dPrintDBAPI('$selectStmt', $selectStmt);

			//Next available ical sequence number
			$selectStmt->addField('enr', 'ical_sequence', 'seq');

			// Execute query
			$sessionsList = $selectStmt->execute()->fetchAll();
			expDebug::dPrint('$sessionList = ' . print_r($sessionsList, true));
			$updatedUserId = getSltpersonUserId();
			// Increment seq number in DB
			if (!empty($sessionsList)) {
				if ($enrLevel == 'TP') {
					// Build enrollment id list
					$enrIdList = array();
					foreach ($sessionsList as $classSessionInfo) {
						if (!in_array($classSessionInfo->enr_id, $enrIdList)) {
							$enrIdList[] = $classSessionInfo->enr_id;
						}
					}
					expDebug::dPrint('$enrIdList = ' . print_r($enrIdList, true), 4);
					$updateStmt = db_update('slt_enrollment');
					$updateStmt->condition('id', $enrIdList, 'IN');				
					$updateStmt->fields(array('updated_by' => ($updatedUserId) ? $updatedUserId : 1, 'updated_on' => now()));
					$updateStmt->expression('ical_sequence', 'ical_sequence + 1');
					expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
					$updateStmt->execute();
				}
				else {
					$updateStmt = db_update('slt_enrollment');
					$updateStmt->condition('id', $enrId, '=');					
					$updateStmt->fields(array('updated_by' => ($updatedUserId) ? $updatedUserId : 1, 'updated_on' => now()));
					$updateStmt->expression('ical_sequence', 'ical_sequence + 1');
					expDebug::dPrintDBAPI('$updateStmt', $updateStmt);
					$updateStmt->execute();
				}
			}
		}
		catch (Exception $ex) {
			$txn->rollback();
			unset($txn);
			watchdog_exception('getClassSessionDetailsForIcal txn', $ex);
			expertusErrorThrow($ex);
		}
		unset($txn);
		syncSolrData('Enrollment');

		//@TODO - To ensure location name is null string or NA for VC class

		return $sessionsList;
	}
	catch (Exception $ex) {
		watchdog_exception('getClassSessionDetailsForIcal', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Consolidates into one serialized object multiple seralized calendar objects
 */
function consolidateICals($iCalsList) {
	try {
		$iCalMerged = array();
		foreach ($iCalsList as $iCalObj) {
			if (empty($iCalObj)) {
				continue;
			}
			expdebug::dPrint('serialized $iCalObj = ' . print_r($iCalObj, true), 4);
			$iCalObj = unserialize($iCalObj);
			expdebug::dPrint('$iCalObj = ' . print_r($iCalObj, true), 4);
			$iCalArr = (array) $iCalObj;
			expdebug::dPrint('$iCalArr = ' . print_r($iCalArr, true), 4);
			foreach ($iCalArr as $iCal) {
				$iCalMerged[] = $iCal;
			} //end foreach
		} //end foreach

		if (empty($iCalMerged)) {
			return null;
		}

		$iCalMergedObj = (object) $iCalMerged;
		expDebug::dPrint('$iCalMergedObj = ' . print_r($iCalMergedObj, true), 4);

		return serialize($iCalMergedObj);
	}
	catch (Exception $ex) {
		watchdog_exception('consolidateICals', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Get enrolled user's preferred language
 */
function enrolledUserPreferredLang($enrId, $enrType) {
	try{
		if ($enrType == 'TP') {
			$selectStmt = db_select('slt_master_enrollment', 'enr');
		}
		else {
			$selectStmt = db_select('slt_enrollment', 'enr');
		}
		$selectStmt->leftJoin('slt_person', 'per', 'enr.user_id = per.id');
		$selectStmt->leftJoin('slt_profile_list_items', 'lang_spli', 'per.preferred_language = lang_spli.code && lang_spli.lang_code = \'cre_sys_lng_eng\'');
		$selectStmt->condition('enr.id', $enrId);
		$selectStmt->addField('lang_spli', 'attr1', 'drupal_lang_code');
		expDebug::dPrintDBAPI('$selectStmt = ', $selectStmt);

		// Execute query
		$prefLang = $selectStmt->execute()->fetchField();
		expDebug::dPrint('$prefLang = ' . print_r($prefLang, true), 4);

		if (empty($prefLang)) {
			return 'en-us';
		}

		return $prefLang;
	}
	catch (Exception $ex) {
		watchdog_exception('enrolledUserPreferredLang', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * conflictCheck() -  Replaces stored procedure slp_cartcheck_ins()
*/

function conflictCheck($userId, $courseId, $classId, $deliveryType = null, $doCrsConflictChk = false) {
	try {
		expDebug::dPrint('$userId : ' . $userId . ' $courseId : ' . $courseId . ' $classId : ' . $classId. ' $deliveryType : ' . $deliveryType, 4);

		// Cannot proceed if invalid User id or Class id
		if ($userId <= 0 || $classId <= 0) {
			$enrollmentStatus['enrollmentstatus'] = -4; //Invalid user or class
			$enrollmentStatus['value'] = 'Invalid user or class';
			expDebug::dPrint('$enrollmentStatus when invalid user or class : ' . print_r($enrollmentStatus, true), 4);
			return $enrollmentStatus;
		}

		if (!isActiveClass($courseId, $classId)) {
			$enrollmentStatus['enrollmentstatus'] = -6; // Invalid course or class
			$enrollmentStatus['value'] = 'Invalid course or class';
			expDebug::dPrint('$enrollmentStatus when invalid course or class : ' . print_r($enrollmentStatus,true), 4);
			return $enrollmentStatus;
		}

		// Confirm that the user is not already enrolled to the class
		$enrollmentExists = enrollmentExistsCheck ($userId, $classId, 'Class');
		expDebug::dPrint(' $enrollmentExists = ' . print_r($enrollmentExists, true) , 4);
		if ($enrollmentExists > 0) {
			if ($enrollmentExists == 2) {
				$enrollmentStatus['enrollmentstatus'] = -7; // Already reserved
			}
			else {
				$enrollmentStatus['enrollmentstatus'] = -1; // Already enrolled.
			}
			$classTitle = getClassTitle($classId);
			$enrollmentStatus['value'] = 'Title:' . $classTitle;
			expDebug::dPrint('$enrollmentStatus when user is already enrolled or has reserved the class : ' . print_r($enrollmentStatus, true), 4);
			return $enrollmentStatus;
		}

		/* // If course conflict check is to be done, confirm that the user is not already enrolled for another class in the same course
		 if ($doCrsConflictChk) {
		$courseConflict = courseConflictCheck($userId, $courseId, $classId, 'Class');
		expDebug::dPrint(' $courseConflict = ' . $courseConflict , 4);
		if ($courseConflict <> 0) {
		$enrollmentStatus['enrollmentstatus'] = -8; // Course conflict - user already enrolled for another class in the same course, etc.
		$enrollmentStatus['value'] = null;
		expDebug::dPrint('$enrollmentStatus when  Course conflict - user already enrolled for another class in the same course : ' . print_r($enrollmentStatus, true), 4);
		return $enrollmentStatus;
		}
		} */

		// Get delivery type so that we can do checks specific to ILT and VC classes
		if (empty($deliveryType)) {
			$deliveryType = getClassDeliveryType($classId, 'Short');
		}
		expDebug::dPrint('$deliveryType : ' . $deliveryType, 4);
		if ( $deliveryType == 'ilt' || $deliveryType == 'vc' ) {
			// If the user is an instructor, confirm that the user not assigned to this class as an instructor and
			// also not as an instructor to another class at an overlapping time.
			$isInstr = is_instructor($userId);
			if ($isInstr == true) {
				// Confirm that the user is not assigned to this class as an instructor
				$conflictInstructor = getClassInstructorConflict($userId, $classId);
				expDebug::dPrint('$conflictInstructor = ' . $conflictInstructor, 4);
				if ($conflictInstructor == false ) {
					$enrollmentStatus['enrollmentstatus'] = -10; //Instructor conflict - User is instructor for this class
					$enrollmentStatus['value'] = 'InstructorConflict';
					expDebug::dPrint('$enrollmentStatus when user is instructor for this class : ' . print_r($enrollmentStatus, true), 4);
					return $enrollmentStatus;
				}

				// Confirm that the user not scheduled to some other class in same session time as instructor
				$conflictInstructorSession = getSessionClassInstructorConflict($userId, $classId);
				expDebug::dPrint('$conflictsessionInstructor = ' . $conflictInstructorSession, 4);
				if ($conflictInstructorSession > 0 ) {
					$enrollmentStatus['enrollmentstatus'] = -2; //Class conflict - user is instructor of another class whose timings conflict with this class
					expDebug::dPrint('$enrollmentStatus when user is instructor of another class whose timings conflict : ' . print_r($enrollmentStatus, true), 4);
					return $enrollmentStatus;
				}

			}

			$conflictingClassId = enrollmentsConflictCheck($userId, $classId, 'Class');
			expDebug::dPrint(' $conflictingClassId : ' . print_r($conflictingClassId, true), 4 );
			if ($conflictingClassId > 0 ) {
				$enrollmentStatus['enrollmentstatus'] = -2; // Class conflict - user has another class whose timings conflict with this class
				$conflictingClassTitle = getClassTitle($conflictingClassId);
				$enrollmentStatus['value'] = 'Title::' . $conflictingClassTitle;
				expDebug::dPrint('$enrollmentStatus when user has another class whose timings conflict with this class : ' . print_r($enrollmentStatus, true), 4);
				return $enrollmentStatus;
			}

			expDebug::dPrint('Before waitListCheck.', 4);
			$seatsFull = false;
			$waitlistNumber = waitListCheck($courseId, $classId);
			expDebug::dPrint('$waitlistNumber : ' . $waitlistNumber, 4);

			if ($waitlistNumber == 0 || $waitlistNumber == -1) { // confirmed
				$enrollmentStatus['enrollmentstatus'] = 0; // Can add to cart
				$enrollmentStatus['waitlistnumber'] = $waitlistNumber;
				return $enrollmentStatus;
			}

			// waitlist - do further check against waitlist capacity, otherwise class is full.
			$classWaitlistCapacity = getClassWaitlistCapacity(null, $classId);
			expDebug::dPrint(' $classWaitlistCapacity = ' . print_r($classWaitlistCapacity, true) , 4);
			$classWaitlistCapacity = (is_null($classWaitlistCapacity))? 0 : $classWaitlistCapacity;
			if ($classWaitlistCapacity < $waitlistNumber) {
				$seatsFull = true;
			}
			$classTitle = getClassTitle($classId);
			if ($seatsFull) {
				$enrollmentStatus['enrollmentstatus'] = -5;	  // Seats full
				$enrollmentStatus['value'] = 'No seats are available';
			}
			else {
				$enrollmentStatus['enrollmentstatus'] = -3; // Waitlist
				$enrollmentStatus['value'] = 'Title::' . $classTitle . ':|:Waitlist::' . $waitlistNumber;
			}
			$enrollmentStatus['waitlistnumber'] = $waitlistNumber;
			$enrollmentStatus['classwaitlistcapacity'] = $classWaitlistCapacity;
			return  $enrollmentStatus;
			expDebug::dPrint('$enrollmentStatus Seats full or Waitlist : '.print_r($enrollmentStatus, true), 4);

		}

		// WBT and VOD
		$enrollmentStatus['enrollmentstatus'] = 0; // Can enroll/add to card
		return $enrollmentStatus;
	}
	catch (Exception $ex) {
		watchdog_exception('cartCheckIns', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}

function getClassSeatCnt($courseId, $classId){
	try {
		// Select table
		$selectStmt = db_select('slt_course_class_session', 'ses');
		// Set conditions
		if(!empty($courseId))
			$selectStmt->condition('ses.course_id', $courseId);
		$selectStmt->condition('ses.class_id', $classId);
		// Select fields
		$selectStmt->addExpression('ifnull(capacity_max, 0) + ifnull(waitlist_count, 0)', 'class_max_capacity');
		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
		if ($result->rowCount() <= 0) {
			return null;
		}
		return $result->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('getClassMaxCapacity', $ex);
		expertusErrorThrow($ex);
	}
}

function classEnrollCnt($courseId, $classId, $chk=""){
	try {
		// Select table
		$selectStmt = db_select('slt_enrollment', 'se');
		// Set conditions
		if(!empty($courseId))
			$selectStmt->condition('se.course_id', $courseId);
		$selectStmt->condition('se.class_id', $classId);
		if($chk == "confirm")
			$selectStmt->condition('se.reg_status', array('lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_wtl'), 'In');
		else
			$selectStmt->condition('se.reg_status', array('lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_rsv','lrn_crs_reg_wtl'), 'In');
		// Select fields
		$selectStmt->addExpression('count(*)', 'enroll_cnt');
		// Limit to 1 record (the first record)
		$selectStmt->range(0, 1);
		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);
		// Execute query
		$result = $selectStmt->execute();
		expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

		return $result->fetchField();
	}catch (Exception $ex) {
		watchdog_exception('getClassMaxCapacity', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 *
* courseConflictCheck() - Check whether the user has already enrolled for another class in the course. Checks also
*                         whether multiple registrations are allowed, in which case there is no conflict.
*/
function enrLnrAssignmentIns ($userId, $orderId, $courseId, $classId, $doWaitlistEnrollment, $updateUserId, $cartFlag) {
	try {
		expDebug::dPrint('$userId : ' . $userId . ' $orderId : ' . $orderId . ' $courseId : ' . $courseId . ' $classId : ' . $classId, 4);
		expDebug::dPrint('$doWaitlistEnrollment : ' . $doWaitlistEnrollment . ' $updateUserId : ' . $updateUserId . ' $cartFlag : ' . $cartFlag, 4);
		$regStatus = null;
		$completionStatus = null;
		$maxCapacityReached = false;
		$waitlistFlag = null;
		$launchable  = 1;

		$deliveryType = getClassDeliveryType($classId, 'Short');

		$result = conflictCheck($userId, $courseId, $classId,$deliveryType,false);
		expDebug::dPrint('$result conflictCheck : '.print_r($result, true), 4);
		if (($result['enrollmentstatus'] != 0 && $result['enrollmentstatus'] != -3) ||
				($result['enrollmentstatus'] == -3 && $doWaitlistEnrollment == 0)) { // permit registration to a waitlist class only when $doWaitlistEnrollment is 1
			return $result;
		}

		// set registration status and completion status
		if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
			$class_max_capacity_cnt = getClassSeatCnt($courseId, $classId);
			expDebug::dPrint('$class_max_capacity_cnt'.$class_max_capacity_cnt,3);

			$launchable  = 0;
			$isPostAssessmentAttached = isPostAssessmentAttached($classId,'class');
			if($isPostAssessmentAttached == 1){
				$launchable  = 1;
}

			if($class_max_capacity_cnt > 0) {
				$class_enroll_cnt = classEnrollCnt($courseId, $classId, "");
				expDebug::dPrint('$class_enroll_cnt123 :'.$class_enroll_cnt, 3);
				if($class_enroll_cnt >= $class_max_capacity_cnt) {
					$result['enrollmentstatus'] = -5;
					return $result;
				}
			}
			if ($result['enrollmentstatus'] == 0) {
				// setting registration status and completion status for a confirmed seat
				if ($cartFlag == 'Y') {
					$regStatus = 'lrn_crs_reg_rsv';
				}
				else {
					$regStatus = 'lrn_crs_reg_cnf';
					$completionStatus = 'lrn_crs_cmp_enr';
				}

				if ($result['waitlistnumber'] == -1) {
					$maxCapacityReached = true;
				}
			}
			else { // ($result['enrollmentstatus'] == -3)  setting registration status for waitlisted seat
				if (!empty($orderId)) {
					$regStatus = 'lrn_crs_reg_rsv';
				}
				else {
					$regStatus = 'lrn_crs_reg_wtl';
				}
				$waitlistFlag = 'lrn_crs_reg_wtl';
			}
		}
		else { //vod or wbt
			if ($cartFlag == 'Y') {
				$regStatus = 'lrn_crs_reg_rsv';
			}
			else {
				$regStatus = 'lrn_crs_reg_cnf';
				$completionStatus = 'lrn_crs_cmp_enr';
			}
		}
		$manCnt = getMandatoryOrNot($classId,$userId);
		$clsMand = ($manCnt == 'cre_sys_inv_man') ? 'Y' : NULL;
		//have checked the already user checkout process in other browser(so insert broweser data to slt_enrollment table). added by yogaraja
		$browser_session_id = session_id();


		$insertStmt = db_insert('slt_enrollment');
		$insertStmt->fields(array(
				'user_id' => $userId,
				'order_id' => $orderId,
				'class_id' => $classId,
				'course_id' => $courseId,
				'reg_status' => $regStatus,
				'recertify_path' => 1,
				'mandatory' => $clsMand,
				'browser_session'=>$browser_session_id,
				'reg_date' => now(),
				'comp_status' => $completionStatus,
				'waitlist_flag' => $waitlistFlag,
				'waitlist_priority' => (empty($result['waitlistnumber']) || $result['waitlistnumber'] <= 0) ? null : $result['waitlistnumber'],
				'created_by' => $updateUserId,
				'created_on' => now(),
				'updated_by' => $updateUserId,
				'updated_on' => now(),
				'launchable'=>$launchable
		));

		expDebug::dPrintDBAPI('$insertStmt = ' , $insertStmt);
		$newEnrollmentId = $insertStmt->execute();

		if ($maxCapacityReached) {
			insertNotification('ClassMaxCapacityReached', $courseId, $classId, $userId, $updateUserId);
		}
		if ($result['enrollmentstatus'] == 0 && $cartFlag != 'Y') {
			$iCal = null;
			if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
				$iCal = getPublishICal($newEnrollmentId, 'Class');
			}
			insertNotification('Register', $courseId, $classId, $userId, $updateUserId, $iCal);
		}
		else if (($deliveryType == 'ilt' || $deliveryType == 'vc') && $result['enrollmentstatus'] == -3) {
			insertNotification('Waitlist', $courseId, $classId, $userId, $updateUserId);
			insertNotification('WaitlistLocationAdmin', $courseId, $classId, $userId, $updateUserId); // @TODO To confirm this notification still exists
		}

		$seatsAvailableForWaitlistUsers = getWaitListAvailable($courseId, $classId);
		expDebug::dPrint(' $seatsAvailableForWaitlistUsers = ' . print_r($seatsAvailableForWaitlistUsers, true) , 4 );
		if ($regStatus == 'lrn_crs_reg_cnf') {
			if ($seatsAvailableForWaitlistUsers == 1) { // Not available
				// Notify all waitlisted users that all the seats that were available are now exhausted.
				$waitListUsers = getWaitlistUsersOfClass($courseId, $classId);
				expDebug::dPrint(' $waitListUsers = ' . print_r($waitListUsers, true), 4);
				foreach ($waitListUsers as $waitListUserId) {
					// Validate notification need to send or not
					insertNotification('WaitlistAllocated', $courseId, $classId, $waitListUserId, $updateUserId); // @TODO To confirm this with vincent wether this notification need to be Conditionally sent to user ( note : $vClassNotify Refer enrollUserToClass() )
				}
			}
		}
		if ($result['enrollmentstatus'] == -3) {
			$result['enrollmentstatus'] = 1;
			$result['value'] = 'Waitlist::' . $waitlistNumber;
		}
		if ($result['enrollmentstatus'] == 0) {
			$result['value'] = 'Registration is successful.';
		}
		syncSolrData('Enrollment');
		syncSolrData('User');
		expDebug::dPrint('$result : ' . print_r($result,true) , 4);
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('enrLnrAssignmentIns', $ex);
		expertusErrorThrow($ex);
	}
}
// this is Fixed For This Ticket #0037676
function getEnrollmentuserIds($entity_id,$class_id){

	$enruserSelect = db_select('slt_enrollment','enroll');
	$enruserSelect->addField('enroll','user_id');
	$enruserSelect->addField('enroll','id');
	$enruserSelect->addField('enroll','class_id');
	$enruserSelect->condition('enroll.course_id',$entity_id,'=');
	$enruserSelect->condition('enroll.class_id',$class_id,'=');
	// issue is Fixed for this Ticket #0036809 Clone only Enrolled ,waitlist and Inprogress  Users for ILT and VC.
	$enruserSelect->condition('enroll.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
	$survAssessCondition = db_or();
	$survAssessCondition->condition('enroll.reg_status', 'lrn_crs_reg_wtl');
	$survAssessCondition->condition('enroll.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
	$enruserSelect->condition($survAssessCondition);
	$enruserSelect->condition('enroll.master_enrollment_id', '','IS NULL');
	expDebug::dPrintDBAPI('$enruserSelect', $enruserSelect);
	$enrollResult = $enruserSelect->execute()->fetchAll();
	return $enrollResult;
}

function cloneEnrollmentNotifications($courseId,$classId,$updateUserId){
	try{
		$enrollmentDet = getEnrollmentuserIds($courseId,$classId);
		$selectIsComp = db_select('slt_course_class', 'class');
		$selectIsComp->addField('class','is_compliance');
		$selectIsComp->condition('class.id', $classId);
		$isCompliance = $selectIsComp->execute()->fetchField();
		foreach($enrollmentDet as $values){
			$userId = $values->user_id;
			$regStatus = null;
			$waitlistNumber = 0;
			//$completionStatus = null;
			$maxCapacityReached = false;
			//$allowRegistration = 0;
			$cartFlag = 'N';
			$deliveryType = getClassDeliveryType($classId, 'Short');
			if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
				$waitlistNumber = waitListCheck($courseId, $classId);
				expDebug::dPrint(' $waitlistNumber = ' . print_r($waitlistNumber, true) , 4);

				if ($waitlistNumber == 0 || $waitlistNumber == -1) { // confirmed
					if ($cartFlag == 'Y') {
						$regStatus = 'lrn_crs_reg_rsv';
					}
					else {
						$regStatus = 'lrn_crs_reg_cnf';
						//	$completionStatus = 'lrn_crs_cmp_enr';
					}

					if ($waitlistNumber == -1) {
						$maxCapacityReached = true;
					}

					$waitlistNumber = null;
				}
				else { // waitlist - do further check against waitlist capacity, otherwise class is full.
					$classWaitlistCapacity = getClassWaitlistCapacity(null, $classId);
					expDebug::dPrint(' $classWaitlistCapacity = ' . print_r($classWaitlistCapacity, true) , 4);
					$classWaitlistCapacity = (is_null($classWaitlistCapacity))? 0 : $classWaitlistCapacity;
					if ($classWaitlistCapacity >= $waitlistNumber) {
						$regStatus = 'lrn_crs_reg_wtl';
					}
					else {
						$regStatus = 'Full';
						// $allowRegistration = 1;
					}
				}
			}
			else { // delivery type == 'wbt' (or video)
				if ($cartFlag == 'Y') {
					$regStatus = 'lrn_crs_reg_rsv';
				}
				else {
					$regStatus = 'lrn_crs_reg_cnf';
					$completionStatus = 'lrn_crs_cmp_enr';
				}
				$waitlistNumber = null;
			}
			$notifyString = null;
			if (is_null($waitlistNumber)) {
				//$enrollmentStatus['id'] = 0;
				//$enrollmentStatus['value'] = 'Registration is successful.';
				$notifyString = ($isCompliance ? 'RegisterMandatoryCompliance' : 'Register');

				if(($isCompliance) && ($deliveryType == 'ilt')) {
					$notifyString = 'RegisterMandatoryCompliance';
				} else if(($isCompliance) && ($deliveryType == 'vc')) {
					$notifyString = 'RegisterVCCompliance';
				} else if(($isCompliance) && ($deliveryType == 'wbt')) {
					$notifyString = 'RegisterWBTCompliance';
				} else if(($isCompliance) && ($deliveryType == 'vod')) {
					$notifyString = 'RegisterWBTCompliance';
				}

				if ($maxCapacityReached) {
					insertNotification('ClassMaxCapacityReached', $courseId, $classId, $userId, $updateUserId);
				}
			}
			else {
				//$enrollmentStatus['id'] = 1;
				//$enrollmentStatus['value'] = 'Waitlist::' . $waitlistNumber;
				$notifyString = 'Waitlist';
			}
			// Validate notification need to send or not
			//if($vClassNotify) {
			if ($cartFlag != 'Y' || !is_null($waitlistNumber)) {
				$iCal = null;
				if ($notifyString == 'Register' || $notifyString == 'RegisterVCCompliance' || $notifyString = 'RegisterMandatoryCompliance') {
					$iCal = getPublishICal($values->id, 'Class');
				}
				insertNotification($notifyString, $courseId, $classId, $userId, $updateUserId, $iCal);
				if ($notifyString == 'Waitlist') {
					insertNotification('WaitlistLocationAdmin', $courseId, $classId, $userId, $updateUserId);
				}
			}
			//	}

			$seatsAvailableForWaitlistUsers = getWaitListAvailable($courseId, $classId);
			expDebug::dPrint(' $seatsAvailableForWaitlistUsers = ' . print_r($seatsAvailableForWaitlistUsers, true) , 4 );
			if ($regStatus == 'lrn_crs_reg_cnf') {
				if ($seatsAvailableForWaitlistUsers == 1) { // Not available
					// Notify all waitlisted users that all the seats that were available are now exhausted.
					$waitListUsers = getWaitlistUsersOfClass($courseId, $classId);
					expDebug::dPrint(' $waitListUsers = ' . print_r($waitListUsers, true), 4);
					foreach ($waitListUsers as $waitListUserId) {
						// Validate notification need to send or not
						//		if($vClassNotify) {
						insertNotification('WaitlistAllocated', $courseId, $classId, $waitListUserId, $updateUserId);
						//		}
					}
				}
			}
		}
	}
	catch (Exception $ex) {
		watchdog_exception('enrolledUserPreferredLang', $ex);
		expertusErrorThrow($ex);
	}

}

function adminCloneAssessmentScoreForClass($oldEnrIdid,$newEnrollId,$entityId,$oldClsId,$newClassId,$loggedInUserId,&$statements=array()){
	try{

		expDebug::dPrint('checking the values of incoming'.$oldEnrIdid.'$newEnrollId'.$newEnrollId.'$entityId'.$entityId.'$oldClsId'.$oldClsId.'$newClassId'.$newClassId.'$loggedInUserId'.$loggedInUserId);
		// For Clone the attendance summary for the class in attendance_summary table.
		$attSumFields = array('user_id', 'course_id', 'content_version_id', 'lesson_id', 'assessment_id', 'status', 'total_attempts',
				'last_attempt_date', 'total_time_spend', 'last_lesson_location','score', 'grade', 'launch_data', 'suspend_data',
				'cmiexit', 'aicc_data', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

		$attSumquery = db_select('slt_attendance_summary', 'att_sum');
		$attSumquery->fields('att_sum',$attSumFields);
		$attSumquery->addExpression($newClassId,'class_id');
		$attSumquery->addExpression($newEnrollId,'enrollment_id');
		$attSumquery->addExpression($loggedInUserId,'created_by');
		$attSumquery->addExpression('now()','created_on');
		$attSumquery->addExpression($loggedInUserId,'updated_by');
		$attSumquery->addExpression('now()','updated_on');
		$attSumquery->condition('att_sum.enrollment_id', $oldEnrIdid,'=');
		$attSumquery->condition('att_sum.course_id', $entityId,'=');
		$attSumquery->condition('att_sum.class_id', $oldClsId,'=');
		expDebug::dPrintDBAPI('Attendance summary clone query', $attSumquery);

		/* db_insert('slt_attendance_summary')
		->from($attSumquery)
		->execute(); */
		$insert_stat = db_insert('slt_attendance_summary')
		->from($attSumquery);
		if(!empty($statements)) {
			$statements[] = $insert_stat;
		} else {
			$insert_stat->execute();
		}

		// For Clone the Attendance Details for the class in attendance_details table.
		$attDetFields = array('user_id', 'course_id', 'content_version_id', 'lesson_id', 'assessment_id', 'status', 'attempt_number', 'attempt_date', 'time_spend',
				'score', 'grade', 'aicc_data', 'launch_data', 'suspend_data', 'cmiexit', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

		$attDetquery = db_select('slt_attendance_details', 'att_det');
		$attDetquery->fields('att_det',$attDetFields);
		$attDetquery->addExpression($newClassId,'class_id');
		$attDetquery->addExpression($newEnrollId,'enrollment_id');
		$attDetquery->addExpression($loggedInUserId,'created_by');
		$attDetquery->addExpression('now()','created_on');
		$attDetquery->addExpression($loggedInUserId,'updated_by');
		$attDetquery->addExpression('now()','updated_on');
		$attDetquery->condition('att_det.enrollment_id', $oldEnrIdid,'=');
		$attDetquery->condition('att_det.course_id', $entityId,'=');
		$attDetquery->condition('att_det.class_id', $oldClsId,'=');
		expDebug::dPrintDBAPI('Attendance details clone query', $attDetquery);

		/* db_insert('slt_attendance_details')
		->from($attDetquery)
		->execute(); */
		$insert_stat_det = db_insert('slt_attendance_details')
		->from($attDetquery);
		if(!empty($statements)) {
			$statements[] = $insert_stat_det;
		} else {
			$insert_stat_det->execute();
		}

		// For Clone the Survey Results for the class in survey_results table.
		$surResFields = array('entity_type', 'user_id', 'survey_id', 'question_type', 'question_id', 'question_txt', 'answer_txt', 'answer_status', 're_attend', 'no_of_attempts',
				'pre_status', 'score', 'isanonymous', 'custom0', 'custom1', 'custom2', 'custom3', 'custom4');

		$surResquery = db_select('slt_survey_results', 'sur_res');
		$surResquery->fields('sur_res',$surResFields);
		$surResquery->addExpression($newClassId,'entity_id');
		$surResquery->addExpression($newEnrollId,'enrollment_id');
		$surResquery->addExpression($loggedInUserId,'created_by');
		$surResquery->addExpression('now()','created_on');
		$surResquery->addExpression($loggedInUserId,'updated_by');
		$surResquery->addExpression('now()','updated_on');
		$surResquery->condition('sur_res.enrollment_id', $oldEnrIdid,'=');
		$surResquery->condition('sur_res.entity_id', $oldClsId,'=');
		expDebug::dPrintDBAPI('Survey results clone query', $surResquery);

		/* db_insert('slt_survey_results')
		->from($surResquery)
		->execute(); */
		$insert_stat_sur = db_insert('slt_survey_results')
		->from($surResquery);
		if(!empty($statements)) {
			$statements[] = $insert_stat_sur;
		} else {
			$insert_stat_sur->execute();
		}

		syncSolrData('Enrollment');
		return true;
	}
	catch (Exception $ex) {
		watchdog_exception('enrolledUserPreferredLang', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}

}
function multipleComplianceMandatoryClassNotification($courseID){
	try{
		$select = db_select('slt_course_class','cls');
		$select->leftjoin('slt_course_class_session', 'sess','sess.class_id = cls.id');
		$select->leftjoin('slt_location', 'loc','loc.id = sess.location_id');
		$select->leftjoin('slt_profile_list_items', 'prof','prof.code = cls.delivery_type');
		$select->addField('cls', 'title','ClassTittle');
		$select->addField('cls', 'code','ClassCode');
		$select->addField('prof', 'name','DeleveryType');
		$select->addField('loc', 'name','Location');
		$select->addExpression("DATE_FORMAT((sess.start_date),'%d %M %Y')", 'StartDate');
		$select->addField('cls', 'id','ClassId');
		$select->condition('cls.course_id',$courseID,'=');
		$result =  $select->execute()->fetchAll();
		expDebug::dPrintDBAPI(' Select Query for multipleComplianceMandatoryClassNotification= ' , $select);
		expDebug::dPrint("Select Result for multipleComplianceMandatoryClassNotification ".print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('multipleComplianceMandatoryClassNotification', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}
function getFrequencyDataValue($code,$date){
	try{
		$select = db_select('slt_notification_info','notify');
		$select->addExpression("if(MOD(DATEDIFF(curdate(),'$date'),notify.frequency_data_value) = 0,1,null)",'resultcnt');
		$select->condition('notify.notification_code',$code,'=');
		$select->where("curdate() >= '$date'");
		$select->where("MOD(DATEDIFF(curdate(),'$date'),notify.frequency_data_value) = 0");
		$result =  $select->execute()->fetchField();
		expDebug::dPrintDBAPI(' Select Query for getFrequencyDataValue= ' , $select);
		expDebug::dPrint("Select Result for getFrequencyDataValue ".print_r($result,true),4);
		//$result = ($result == 1) ? 1 : 0;
		//expDebug::dPrint("Select Result for gasdsadsaetFrequencyDataValue ".print_r($result,true),4);
		return $result;
	}catch (Exception $ex) {
		watchdog_exception('getFrequencyDataValue', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}
function getClassIds($courseID,$callfrom='',$ignoreStatus= false){
	try{
		$select = db_select('slt_course_class','cls');
		$select->addField('cls', 'id','classid');
		$select->condition('cls.course_id',$courseID,'=');
		if(!empty($callfrom) && $callfrom != 'all'){
			$userId = getSltpersonUserId();
			filterByUserAccessPrivileges($select, 'cls',$userId,'','cre_sys_obt_cls');
		}
        if($ignoreStatus == false){
		$select->condition('cls.status','lrn_cls_sts_atv','=');
        }
		$result =  $select->execute()->fetchAll();
		expDebug::dPrintDBAPI("Get class Id From Course Id Query ",$select);
		expDebug::dPrint("Get class Id From Course Id ".print_r($result,true),5);
		return $result;
	}	catch (Exception $ex) {
		watchdog_exception('getClassIds', $ex);
		if (isset($_GET['cron_key'])) {
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}

/*function isComplianceUpdate(){
	try{
		module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
		global $catalog_reg;
		$catalog_reg = 'Class';
		$lnrSrch = new LnrSearchCatalog();
		$lnrSrch->catalogSearchQueryClassCron('update');
		if(module_exists('exp_sp_commerce')){
			$lnrSrch->searchPriceClass('$0-$0');

		}
		$lnrSrch->setTrainingType('cre_sys_inv_com',true);
		$classQuery=$lnrSrch->select;
	//	$lnrSrch->catalogSearchQueryTPCron('update');
		//if(module_exists('exp_sp_commerce')){
			//$lnrSrch->searchPriceTP('$0-$0');
		//}
		//$lnrSrch->searchDate('', '');
		//$lnrSrch->setTrainingTypeTP('cre_sys_inv_man',true);
		//$TPQuery=$lnrSrch->select;
		//$classQuery->union($TPQuery,'UNION ALL');
		expDebug::dPrintDBAPI(' isComplianceUpdate = ', $classQuery);
		$resultSet = $classQuery->execute()->fetchAll();
		foreach ($resultSet as $result) {
			$enrollId  	= $result->eid;
			$is_compliance  = $result->is_compliance;
			$userId = $result->userids;
	//		#	0032377 : compliance class update when jobrole or ORG or UserType change
			$updateStmt = db_update('slt_enrollment');
			// Set conditions
			$updateStmt->condition('id',$enrollId,'=');
			
			// Set new values of fields
			$updateStmt->fields(array(
					'is_compliance' => $is_compliance,
					'updated_on'    => now(),
					'updated_by'    => $userId
			));
			expDebug::dPrintDBAPI(' $updateStmt = ', $updateStmt);
			$updateStmt->execute();

		}
		expDebug::dPrint("Class Query Result".print_r($resultSet,true),4);



	}catch (Exception $ex) {
		watchdog_exception('isComplianceUpdate', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}*/

//Mandatory class and program auto_registration start
/*function mandatoryComplianceAutoRegister(){
	try{
		module_load_include('inc', 'exp_sp_lnrsearch', 'exp_sp_lnrsearch');
		global $catalog_reg;
		$catalog_reg = 'Class';
		$lnrSrch = new LnrSearchCatalog();
		$lnrSrch->catalogSearchQueryClassCron();
		if(module_exists('exp_sp_commerce')){
			$lnrSrch->searchPriceClass('$0-$0');

		}
		$lnrSrch->setTrainingType('cre_sys_inv_com|cre_sys_inv_man',true);
		$classQuery=$lnrSrch->select;
		$lnrSrch->catalogSearchQueryTPCron();
		if(module_exists('exp_sp_commerce')){
			$lnrSrch->searchPriceTP('$0-$0');
		}
		//$lnrSrch->searchDate('', '');
		$lnrSrch->setTrainingTypeTP('cre_sys_inv_man',true);
		//$TPQuery=$lnrSrch->select;

		$classQuery->union($lnrSrch->select,'UNION ALL');
		$resultSet = $classQuery->execute()->fetchAll();
		expdebug::dPrintDBAPI("Class Fetch Query - ",$classQuery);
		expDebug::dPrint("Class Query Result".print_r($resultSet,true),3);
		$userGetArr = array();
		foreach ($resultSet as $result) {
			expDebug::dPrint("Class Query Result".print_r($result,true),4);
			$classId = $result->cls_id;
			$courseid = $result->crs_id;
			$groupid = $result->groupid;
			$delivery_type_name = $result->delivery_type_name;
			expDebug::dPrint("Group id - ".$groupid,4);

							$select = db_select('slt_person', 'person');
							$select->distinct();
							$select->addField('person','id','personId');
							if(!empty($groupid)){
								$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = person.id');
															$select->leftjoin('slt_groups', 'catacs', 'catacs.id='.$groupid.' AND (if(FIND_IN_SET(person.id,catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(person.id,catacs.removed_users) <= 0)
												AND( (if(catacs.country=\'All\',person.country!=\'\',(catacs.country IS NULL)) OR (catacs.country=\'\')) OR (FIND_IN_SET(person.country, catacs.country)) )
												AND( (if(catacs.department=\'All\',person.dept_code!=\'\',(catacs.department IS NULL)) OR (catacs.department=\'\')) OR (FIND_IN_SET(person.dept_code, catacs.department)) )
												AND( (if(catacs.employment_type=\'All\',person.employment_type!=\'\',(catacs.employment_type IS NULL)) OR (catacs.employment_type=\'\')) OR (FIND_IN_SET(person.employment_type, catacs.employment_type)) )
												AND( (if(catacs.is_instructor !=\'N\',person.is_instructor=\'Y\',1=1)) )AND( (if(catacs.is_manager !=\'N\',person.is_manager=\'Y\',1=1)) )
										    AND( (if(catacs.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id =person.id)>0,(catacs.job_role IS NULL)) OR (catacs.job_role=\'\')) OR (FIND_IN_SET(lpjm.job_role,catacs.job_role)>0) )
												AND( (if(catacs.language=\'All\',person.preferred_language!=\'\',(catacs.language IS NULL)) OR (catacs.language=\'\')) OR (FIND_IN_SET(person.preferred_language, catacs.language)) )
												AND( (if(catacs.org_id=\'All\',person.org_id!=\'\',(catacs.org_id IS NULL)) OR (catacs.org_id=\'\')) OR (FIND_IN_SET(person.org_id, catacs.org_id)) )
												AND( (if(catacs.state=\'All\',person.state!=\'\',(catacs.state IS NULL)) OR (catacs.state=\'\')) OR (FIND_IN_SET(person.state, catacs.state)) )
												AND( (if(catacs.user_type=\'All\',person.user_type!=\'\',(catacs.user_type IS NULL)) OR (catacs.user_type=\'\')) OR (FIND_IN_SET(person.user_type, catacs.user_type)) )
												) )');
							}
							if($delivery_type_name == 'Curricula' || $delivery_type_name == 'Certification' || $delivery_type_name == 'Learning Plan'){
								$select->leftjoin('slt_master_enrollment', 'master_enroll', "master_enroll.program_id = $classId AND master_enroll.user_id = person.id AND master_enroll.overall_status IN  ('lrn_tpm_ovr_cmp', 'lrn_tpm_ovr_enr', 'lrn_tpm_ovr_inp', 'lrn_tpm_ovr_ppv', 'lrn_tpm_ovr_ppm', 'lrn_tpm_ovr_exp', 'lrn_tpm_ovr_wtl')");
							}else{
								$select->leftjoin('slt_enrollment', 'enroll', "enroll.class_id = $classId AND enroll.course_id= $courseid AND enroll.user_id = person.id AND enroll.reg_status IN  ('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_wtl')");
							}
							$select->leftjoin('slt_autoregister_temp', 'temp', "temp.cls_id = $classId AND temp.userids = person.id");
							if($delivery_type_name == 'Curricula' || $delivery_type_name == 'Certification' || $delivery_type_name == 'Learning Plan'){
								$select->condition('master_enroll.id','','is null');
							}else{
								$select->condition('enroll.id','','is null');
							}
							$select->condition('person.id',array(1,2),'NOT IN');
							$select->condition('person.status','cre_usr_sts_atv','=');
							$select->condition('temp.id','','is null');
							if(!empty($groupid)){
							  $select->condition('catacs.id','','IS NOT NULL');
							}
							$userdetails = $select->execute()->fetchAll();
							expDebug::dPrintDBAPI("Query result for users ----->",$select);

		foreach ($userdetails as $users){
			$userid = $users->personid;
			$inQry = db_insert('slt_autoregister_temp');
			$inQry->fields(array(
					'cls_id' => $result->cls_id,
					'cls_code' => $result->cls_code,
					'cls_title' => $result->cls_title,
					'cls_short_description' => $result->cls_short_description,
					'price' => $result->price,
					'currency_type' => $result->currency_type,
					'registration_end_on' => $result->registration_end_on,
					'created_on' => $result->created_on,
					'published_on' => $result->published_on,
					'mro_id' => $result->mro_id,
					'language_code' => $result->language_code,
					'language' => $result->language,
					'userids' => $userid,
					'start_date' => $result->start_date,
					'location_id' => $result->location_id,
					'crs_id' => $result->crs_id,
					'delivery_type_code' => $result->delivery_type_code,
					'delivery_type_name' => $result->delivery_type_name,
					'export_compliance' => $result->export_compliance,
					'expires_in_value' => $result->expires_in_value,
					'expires_in_unit' => $result->expires_in_unit,
					'prm_created_on' => $result->prm_created_on,
					'object_type' => $result->object_type,
					'is_compliance' => $result->is_compliance,
					'avgvote' => $result->avgvote,
					'ascen' => $result->ascen
			));
			$logId = $inQry->execute();
		}
		}
		expDebug::dPrintDBAPI("class details fetch query -- ",$logId);

		//$inQry = db_insert('slt_autoregister_temp');
	//	$inQry->from($classQuery);
		expDebug::dPrintDBAPI("Auto register temp table insert -- ",$inQry);
	//	$inQry->execute();
		//expDebug::dPrintDBAPI("Auto register temp table insert -- ",$inQry);
	}catch (Exception $ex) {
		watchdog_exception('mandatoryComplianceAutoRegister', $ex);
		if (isset($_GET['cron_key'])) {
			expertusLogError($ex);
			throw new Exception($ex);
		}
		expertusErrorThrow($ex);
	}
}*/
function autoregisterformandatorycompliance(){
	try {
		$tid = 0;
		$rand = time();
		$tempDetails=array();
		register_shutdown_function(array(this,"autoRegisterErrorHandler"),$rand,$tid);
		$limit = getConfigValue('autoregister_limit'); // Limit
		$select = db_select('slt_autoregister_temp','temp');
		$select->addField('temp', 'id', 'id');
	  $select->addField('temp', 'object_type');
	  $select->addField('temp', 'userids','userid');
	  $select->addField('temp', 'crs_id','course_id');
	  $select->addField('temp', 'cls_id','class_id');
	  $select->addField('temp', 'mro_id','mro_id');
	  $select->addField('temp', 'is_compliance','is_compliance');
	  $select->addField('temp', 'published_on','published_on');
	  $select->addField('temp', 'delivery_type_code','delivery_type_code');
	  $select->orderBy('temp.id');
	  $select->condition('currency_type','P','!=');
	  $select->orderBy('temp.cls_id');
	  $select->orderBy('temp.crs_id');
	  $select->range(0, $limit);
	  expDebug::dPrintDBAPI("Course details to auto register -- ",$select);
	  $tempDetails = $select->execute()->fetchAll();
	  $count = count($tempDetails);
	  expDebug::dPrint("Last rec id for auto register -- >".$tempDetails[$count-1]->id,4);
	  expDebug::dPrint("Record list for auto register -- >".print_r($tempDetails,trye),4);
	  $waitlist = false;
	  $isAdminSide = 'Y';
	  $userGetArr = array();

	  if($count > 0){

	  	$update = db_update('slt_autoregister_temp');
	  	$update->fields(array(
	  			'currency_type' => 'P',
	  			'language_code' => $rand
	  	));
	  	$update->condition('id', $tempDetails[$count-1]->id, '<=');
			$update->condition('id', $tempDetails[0]->id, '>=');
	  	$update->condition('currency_type', 'P', '!=');
	  	expDebug::dPrintDBAPI("Update quey for temp table - ",$update);
	  	$update->execute();

	  	$preClsId = 0;
	  	$preCrsId = 0;
	  	$isPreReq = false;
	  	$chkCrsCompleteByDate= '';
		$registerEndDateCheck= '';
		  $maxCount = array();
	  	foreach ($tempDetails as $key=>$details){
	  		expDebug::dPrint("auto register course details -- ".print_r($details,true),5);
	  		$tid = $details->id;
	  		  $userId = $details->userid;
	  		  $crs_id = $details->course_id;
	  		  $cls_id = $details->class_id;
			  	$object_type = $details->object_type;
			  	$node_id 		= $details->node_id;
			  	$mro_id 		= $details->mro_id;
			  	$is_compliance  = $details->is_compliance;
			  	$published_on = $details->published_on;
			  	$updateUserId = 1;

			  	// One time check start, whenever the class id changes
			  	if($preClsId != $cls_id && $preCrsId != $crs_id){
			  		$preClsId = $cls_id ;
			  		$preCrsId = $crs_id ;
			  		if($object_type == 'Class'){
			  	$chkCrsCompleteByDate =  chkCrsCompleteByDate($crs_id);
			  	if($chkCrsCompleteByDate == false){
			  		$classids = getClassIds($crs_id);
			  		expDebug::dPrint("class mark complete - ids -- ".print_r($classids,true),5);
			  		foreach ($classids as $classId){
			  			$update = db_update('slt_course_class');
			  			$update->fields(array(
			  					'status' => 'lrn_cls_sts_dld'
			  			));
			  			$update->condition('id', $classId->classid, '=');
			  			expDebug::dPrintDBAPI("Class mark complete query - ",$update);
			  			$update->execute();
			  		}
			  	}
			  			$registerEndDateCheck = registerEndDateCheck($crs_id,$cls_id);
			  			$isPreReq = count(checkCatalogPrerequisite($crs_id,'cre_sys_obt_crs')) === 0 ? false : true;

			  		}else{
			  			$isPreReq = count(checkCatalogPrerequisite($cls_id,'cre_sys_obt_trp')) === 0 ? false : true;
			  		}
			  		// Moved here by Vincent on June 24, 2015 for #0048488
			  		if($object_type == 'Class'){
			  			$maxCount = getIfSingleClassCron($crs_id,$userId,$is_compliance);
			  		}
			  	}
			  	// One time check end
			  	if ($mro_id == 'cre_sys_inv_man' || ($is_compliance == 1 && $chkCrsCompleteByDate==true)) {
			  	if($object_type == 'Class'){
			  		//$maxCount = getIfSingleClassCron($crs_id,$userId);
			  		expDebug::dPrint("max count  ".print_r($maxCount,true),4);
			  		if($maxCount[0]->maxcnt == 1) {
			  				$delivery_type_code = $details->delivery_type_code;
			  				$available_seats = 1;
			  				if($delivery_type_code == 'lrn_cls_dty_ilt' || $delivery_type_code == 'lrn_cls_dty_vcl'){
			  					$available_seats = getAvailableSeats($cls_id);
			  				}
								$preRequisite = $isPreReq === true ? checkCatalogPrerequisite($crs_id,'cre_sys_obt_crs',$userId) : array() ; //if there is pre-requestie set for this course it should not register directly.
			  				if(count($preRequisite) == 0) {
			  					if($available_seats >= 1 && $registerEndDateCheck == 0){
			  		           register_user_to_course_class($userId, $crs_id, $cls_id,$waitlist,$isAdminSide,false);
			  		           }
			  		           }else{
			  		           	$published = explode(" ",$published_on);
			  		           	$getVal = getFrequencyDataValue('man_comp_multiple_class_register',$published[0]);
			  		           	if($getVal == 1)	{
			  		           		insertNotification("MultipleComplianceClassNotification", $crs_id, $cls_id, $userId, $updateUserId);
			  		           	}
			  		           }
						}	else{
			  		           	if($is_compliance == 1){
			  		           		$userCrsId = $crs_id.'_reg_'.$userId;
			  		           		if(!in_array($userCrsId,$userGetArr)){
			  		           			$getRegister = getRegisteredOrNot($crs_id,$cls_id,$userId);
			  		           			if(empty($getRegister->enrolled_id)){
			  		           				$published = explode(" ",$published_on);
			  		           				$getVal = getFrequencyDataValue('man_comp_multiple_class_register',$published[0]);
			  		           				if($getVal == 1)	{
			  		           					insertNotification("MultipleComplianceClassNotification", $crs_id, $cls_id, $userId, $updateUserId);
			  		           				}
			  		           			}
			  		           			array_push($userGetArr,$userCrsId);
			  		           		}
			  		           	}else if($mro_id == 'cre_sys_inv_man'){
			  		           		$userCrsId = $crs_id.'_reg_'.$userId;
			  		           		if(!in_array($userCrsId,$userGetArr)){
			  		           			$getRegister = getRegisteredOrNot($crs_id,$cls_id,$userId);
			  		           			if(empty($getRegister->enrolled_id)){
			  		           				$published = explode(" ",$published_on);
			  		           				$getVal = getFrequencyDataValue('man_comp_multiple_class_register',$published[0]);
			  		           				if($getVal == 1)	{
			  		           					insertNotification("MultipleMandatoryClassNotification", $crs_id, $cls_id, $userId, $updateUserId);
			  		           				}
			  		           			}
			  		           			array_push($userGetArr,$userCrsId);
			  		           		}
			  		           	}
			  		           }
			  	}else{
			  		$userCrsId = $cls_id.'_reg_'.$userId;
						if(!in_array($userCrsId,$userGetArr))		{
						$resultsMasterEnroll = getObjectRegisteredOrNot($cls_id,$userId);
						if(empty($resultsMasterEnroll->master_enrolled_id)){
							$prm_available_seats = getObjectAvailableSeats($cls_id,$userId);
							if(($prm_available_seats > 0 || $prm_available_seats == -1)){
								$maxClassCnt = getCourseClassCountForLP($cls_id,$userId,'N');
								$preRequisite = $isPreReq === true ? checkCatalogPrerequisite($cls_id,'cre_sys_obt_trp') : array() ; //if there is pre-requestie set for this Training Plan it should not register directly.
								if($maxClassCnt[0]->cnt == 1 && count($preRequisite) == 0  ) {
									$classId = $maxClassCnt[0]->classids;
			  		register_user_to_training_plan($userId,$cls_id, "$classId",false,'N',false);
			  		}else{
			  			$prm_id 		= $details->class_id;
			  			$published_on = $details->published_on;
			  			$published = explode(" ",$published_on);
			  			$getVal = getFrequencyDataValue('mandatory_tp_multiple_register',$published[0]);
			  			if($getVal == 1)	{
			  				insertNotification("MultipleMantadoryTPNotification", $prm_id, null, $userId, $updateUserId);
			  			}

			  		}
			  		}
			  		}
			  		array_push($userGetArr,$userCrsId);
			  		}
			  	}
	  	}
	  	}
	  	db_query("DELETE FROM slt_autoregister_temp  where language_code=$rand and currency_type='P'");
	  }
	  return true;
	}catch (Exception $ex) {
		expDebug::dPrint("Auto register error handler catch block",1);
		db_query("DELETE FROM slt_autoregister_temp  where language_code=$rand and currency_type='P' and id < $tid");
		expDebug::dPrint("UPdate query DELETE FROM slt_autoregister_temp  where language_code=$rand and currency_type='P' and id < $tid",2);
		$update = db_update('slt_autoregister_temp');
		$update->fields(array(
				'currency_type' => 'F',
				'language_code' => 'FF'
		));
		$update->condition('currency_type', 'P', '=');
		$update->condition('language_code', $rand, '=');
		expDebug::dPrintDBAPI("Update quey for temp table - ",$update);
		$update->execute();

		// Update monitoring jobs table to pick the process in next run
		$update = db_update('slt_monitoring_jobs');
		$update->fields(array(
		  'custom0'=>0
		));
		$update->condition('job_name','auto_register','=');
		$update->execute();

		watchdog_exception('autoregisterformandatorycompliance', $ex);
		expertusLogError($ex);
	}

}

function autoRegisterErrorHandler(&$rand,&$tid){
	expDebug::dPrint("Auto register Fatal error handler",1);
	db_query("DELETE FROM slt_autoregister_temp  where language_code=$rand and currency_type='P' and id < $tid");
	expDebug::dPrint("UPdate query DELETE FROM slt_autoregister_temp  where language_code=$rand and currency_type='P' and id < $tid",2);
	// Update temp file to process records in next run which is not processed in current batch
	$update = db_update('slt_autoregister_temp');
	$update->fields(array(
				'currency_type' => 'F',
				'language_code' => 'FF'
				));
				$update->condition('currency_type', 'P', '=');
				$update->condition('language_code', $rand, '=');
				expDebug::dPrintDBAPI("Update quey for temp table - ",$update);
				$update->execute();

	// Update monitoring jobs table to pick the process in next run
	$update = db_update('slt_monitoring_jobs');
	$update->fields(array(
	  'custom0'=>0
	));
	$update->condition('job_name','auto_register','=');
	$update->execute();
}

function getEnrDetForComplianceExpiryNotifications($enrollmentId) {
	try {
		// Select table
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->leftJoin('slt_course_template','crs','crs.id=enr.course_id');

		// Set conditions
		$selectStmt->condition('enr.id', $enrollmentId);

		// Select fields and/or add expressions
		$selectStmt->addField('enr', 'user_id', 'user_id');
		$selectStmt->addField('enr', 'course_id', 'course_id');
		$selectStmt->addField('enr', 'class_id', 'class_id');
		//$selectStmt->addField('enr', 'order_id', 'order_id');
		$selectStmt->addField('enr', 'reg_status', 'reg_status');
		$selectStmt->addExpression("if(crs.validity_date IS NULL,(DATE_FORMAT(DATE_ADD(enr.comp_date, INTERVAL crs.validity_days DAY),'%d %M %Y')), (DATE_FORMAT(crs.validity_date,'%d %M %Y')) )",'valdate');
		$selectStmt->addExpression("DATE_FORMAT(enr.comp_date,'%d %M %Y')",'compdate');
		$selectStmt->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
				->condition('crs.validity_days','','IS NOT NULL')

		);
		$selectStmt->condition('enr.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_exp'),'IN');
		/* if($notifyType == 'ComplianceExpiryRemainder'){


		} */

		expDebug::dPrintDBAPI(' $selectStmt = ' , $selectStmt);

		// Execute query
		$result = $selectStmt->execute();
		expDebug::dPrint( '$result = ' . print_r($result, true) , 3);

		if ($result->rowCount() <= 0) {
			return null;
		}

		return $result->fetchAssoc();
	}catch (Exception $ex) {
		watchdog_exception('getEnrollmentDetailsForNotifications', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Function to return completed class have recertify link or not
 * @param
 *     enroll Id
 * @return
 *     number
 */

function checkRecertifyorNot($courseId,$userId,$compStatus,$enr_id){
	try{
		$select = db_select('slt_enrollment','enr');
		$select->leftJoin('slt_course_template','crs','crs.id=enr.course_id');
		$select->innerJoin('slt_notification_info','notify','notify.notification_code=\'compliance_expiry_remainder\'');
		$select->addExpression('DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enr.comp_date, interval crs.validity_days day), interval notify.notify_before day)),
				(DATE_SUB(crs.validity_date, interval notify.notify_before day))))','cnt');
		$select->condition('enr.is_compliance', 1,'=');
		$select->condition('enr.course_id', $courseId, '=');
		$select->condition('enr.id', $enr_id, '=');
		$select->condition('enr.user_id',$userId,'=');
		if($compStatus == 'lrn_crs_cmp_cmp'){
		$select->condition('enr.cmpl_expired',1,'!=');
		}
		$select->where('if(crs.validity_date IS NOT NULL,crs.validity_date >= curdate(),1=1 )');
		$select->where('if(crs.complete_date IS NOT NULL , crs.complete_date >= curdate(),1=1 )');
		$select->condition(db_or()->condition('crs.validity_date','','IS NOT NULL')
				->condition('crs.validity_days','','IS NOT NULL'));
		$select->condition('enr.comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_exp'),'IN');

		$select->orderBy('enr.id', 'DESC');
		$select->range(0, 1);
		expDebug::dPrintDBAPI('checkRecertifyorNot', $select);
		$result =  $select->execute()->fetchField();
		expDebug::dPrint('test variable '.$result);
				return ($result >= 0 && $result != null) ? 1 : 0;
	}catch (Exception $ex) {
		watchdog_exception('getRegisteredOrNot', $ex);
		expertusErrorThrow($ex);
	}

}


/**
 * Get the count of Registered Class for Compliance.
 *
 * @param string $courseid,$classid
 *
 * @return
 *   count of the Registered Class.
 */
function getComplianceRegisteredOrNot($courseid,$classid,$userId='',$widgetId = '',$clsCnt){
	try{
		global $catalog_reg;
		if($userId=='')
			$userId = getSltpersonUserId();

		$select = db_select('slt_enrollment', 'enroll');
		$select->addField('enroll','id', 'enrolled_id');
		$select->addField('enroll','reg_status', 'enrolled_status');
		$select->addField('enroll','comp_status', 'comp_status');
		$select->addField('enroll','waitlist_flag', 'waitlist_flag');
		$select->addField('enroll','waitlist_priority', 'waitlist_priority');
		if($catalog_reg != 'Course' || ($widgetId != 'catalog-search' && $catalog_reg == 'Course')){
			$select->condition('enroll.class_id', $classid, '=');
		}
		$select->condition('enroll.course_id', $courseid, '=');
		$select->condition('enroll.user_id',$userId,'=');
		$select->leftJoin('slt_course_template','crs', 'crs.id=enroll.course_id');
		$select->innerJoin('slt_notification_info','notify', 'notify.notification_code=\'compliance_expiry_remainder\'');
		$select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm','lrn_crs_reg_wtl'), 'IN'));
		// Below condition modified by Vincent on 05 Dec, 2013 for #0026273
		$select->where('IF(enroll.reg_status = \'lrn_crs_reg_cnf\', enroll.comp_status IN(\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'),1=1)');
		expDebug::dPrint('checking the values to be printed for the sss'.print_r($clsCnt,true));
	//	if($clsCnt[0]->maxcnt == 1){
			$select->where('if(enroll.comp_status= \'lrn_crs_cmp_cmp\',
					(DATEDIFF(curdate(),if(crs.validity_days IS NOT NULL,(DATE_SUB(DATE_ADD(enroll.comp_date, interval crs.validity_days+1 day), interval notify.notify_before day)),
					(DATE_SUB(crs.validity_date, interval notify.notify_before-1 day)))) > 0),1=1)');
			$select->orderBy('enroll.id', 'DESC');
		/* }else{
			$select->orderBy('enroll.comp_status', 'ASC');
		} */
		$select->range(0, 1);
		expDebug::dPrintDBAPI('getComplianceRegisteredOrNot ', $select);
		$enrolled = $select->execute()->fetchObject();

		return $enrolled;
	}catch (Exception $ex) {
		watchdog_exception('getRegisteredOrNot', $ex);
		expertusErrorThrow($ex);
	}

}
function complianceExpiryandRegisterCount($courseId,$enrId='',$userId=''){
	$select = db_select('slt_enrollment', 'enroll');
	$select->addExpression('count(1)', 'cnt');
	$select->condition('enroll.comp_status', array('lrn_crs_cmp_enr','lrn_crs_cmp_inp','lrn_crs_cmp_att', 'lrn_crs_cmp_cmp'), 'IN');	//change by ayyappans for 40093: Alert message not showing
	$select->condition('enroll.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'), 'NOT IN');
	$select->condition('enroll.course_id', $courseId, '=');
	$select->condition('enroll.user_id',$userId,'=');
	if($enrId){
	$select->condition('enroll.id',$enrId,'!=');
		$select->condition('enroll.is_compliance',1,'=');
	}
	$result = $select->execute()->fetchField();
	expDebug::dPrintDBAPI('complianceExpiryandRegisterCount', $select);
	return $result;
}
function getEnrolledCount($courseId,$userId,$clsId=array(), $catalog_reg = 'Class'){
	expDebug::dPrint('get enroll count args '.print_r(func_get_args(), 1));
	if($catalog_reg == 'Course') {
		$enrollment = db_select('slt_enrollment', 'enr');
		$enrollment->addField('enr', 'comp_status');
		$enrollment->addField('enr', 'reg_status');
		$enrollment->addField('enr', 'class_id');
		$enrollment->condition('enr.course_id', $courseId, '=');
		$enrollment->condition('enr.user_id', $userId, '=');
		if(count($clsId) > 0){
			$enrollment->condition('enr.class_id', $clsId, 'IN');
		}
		$enrollment->orderBy('enr.id', 'desc');
// 		$enrollment->range(0, 1);
		
		$select = db_select($enrollment, 'enroll');
		$select->addField('enroll', 'comp_status', 'comp_status');
		$select->addField('enroll', 'reg_status', 'reg_status');
		$select->addField('enroll', 'class_id', 'class_id');
		
		
		
// 		$select->where('enroll.comp_status NOT IN (\'lrn_crs_cmp_inc\', \'lrn_crs_cmp_nsw\')');
// 		$select->where('enroll.reg_status NOT IN (\'lrn_crs_reg_can\', \'lrn_crs_reg_rsc\')');
		$select->groupBy('enroll.class_id');
		expDebug::dPrintDBAPI('getEnrolledCount 1 ', $select);
		$enrollments = $select->execute()->fetchAll();
		$enroll_count = 0;
		foreach ($enrollments as $enroll) {
			if($enroll->reg_status != 'lrn_crs_reg_can' && $enroll->reg_status !='lrn_crs_reg_rsc' && $enroll->comp_status != 'lrn_crs_cmp_inc' && $enroll->comp_status != 'lrn_crs_cmp_nsw') {
				$enroll_count++;
			}
		}
		expDebug::dPrint('enr result '.print_r($enroll_count, 1));
		$result = $enroll_count;
	} else {
	$select = db_select('slt_enrollment', 'enroll');
	$select->addExpression('count(1)', 'cnt');
	$select->condition('enroll.comp_status', array('lrn_crs_cmp_inc','lrn_crs_cmp_nsw'), 'NOT IN');
	$select->condition('enroll.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'), 'NOT IN');
	$select->condition('enroll.course_id', $courseId, '=');
	$select->condition('enroll.user_id',$userId,'=');
	if(count($clsId)>0){
		$select->condition('enroll.class_id',$clsId,'IN');
	}
	$result = $select->execute()->fetchField();
	}
	expDebug::dPrintDBAPI('getEnrolledCount', $select);


	return $result;
}
function getCourseCompletedCount($courseId,$userId){
	if($catalog_reg == 'Course') {
		$enrollment = db_select('slt_enrollment', 'enr');
		$enrollment->addField('enr', 'comp_status');
		$enrollment->addField('enr', 'reg_status');
		$enrollment->addField('enr', 'class_id');
		$enrollment->condition('enr.course_id', $courseId, '=');
		$enrollment->condition('enr.user_id', $userId, '=');
		if(count($clsId) > 0){
			$enrollment->condition('enr.class_id', $clsId, 'IN');
		}
		$enrollment->orderBy('enr.id', 'desc');
		// 		$enrollment->range(0, 1);
	
		$select = db_select($enrollment, 'enroll');
		$select->addField('enroll', 'comp_status', 'comp_status');
		$select->addField('enroll', 'reg_status', 'reg_status');
		$select->addField('enroll', 'class_id', 'class_id');
	
	
	
		// 		$select->where('enroll.comp_status NOT IN (\'lrn_crs_cmp_inc\', \'lrn_crs_cmp_nsw\')');
		// 		$select->where('enroll.reg_status NOT IN (\'lrn_crs_reg_can\', \'lrn_crs_reg_rsc\')');
		$select->groupBy('enroll.class_id');
		expDebug::dPrintDBAPI('getEnrolledCount 1 ', $select);
		$enrollments = $select->execute()->fetchAll();
		$enroll_count = 0;
		foreach ($enrollments as $enroll) {
			if($enroll->reg_status != 'lrn_crs_reg_can' && $enroll->reg_status !='lrn_crs_reg_rsc' && $enroll->comp_status != 'lrn_crs_cmp_inc' && $enroll->comp_status != 'lrn_crs_cmp_nsw') {
				$enroll_count++;
			}
		}
		expDebug::dPrint('enr result '.print_r($enroll_count, 1));
		$result = $enroll_count;
	} else {
	$select = db_select('slt_enrollment', 'enroll');
	$select->addExpression('count(1)', 'cnt');
	$select->condition('enroll.comp_status', 'lrn_crs_cmp_cmp', '=');
	$select->condition('enroll.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'), 'NOT IN');
	$select->condition('enroll.course_id', $courseId, '=');
	$select->condition('enroll.user_id',$userId,'=');
	$result = $select->execute()->fetchField();
	}
	
	expDebug::dPrintDBAPI('getCourseCompletedCount', $select);
	return $result;
}

function getCourseClassRegisteredcount($courseId,$classId,$userId){
  expDebug::dPrint("course Id -- ". $courseId . "--classid -- ".$classId ."--userid--" .$userId );
  $select = db_select('slt_course_class', 'cls');
  $select->leftJoin('slt_course_template','crs', 'crs.id = cls.course_id');
  $select->leftJoin('slt_course_class','c1', 'c1.course_id = crs.id');
  $select->leftJoin('slt_enrollment','enr', 'cls.id = enr.class_id ');
  $select->addExpression('group_concat(distinct(c1.id))', 'cls_id');
  $select->addExpression('group_concat(distinct(enr.class_id))', 'enroll_clsid');
  $select->condition('cls.course_id',$courseId,'=');
  $select->condition('enr.user_id',$userId,'=');
  expDebug::dPrintDBAPI('getCourseClassRegisteredcount cls count', $select);
  $cls_count = $select->execute()->fetchAll();
 
  $cls_id = explode(',',$cls_count[0]->cls_id);
  $enr_cls_id = explode(',',$cls_count[0]->enroll_clsid);
  $diff = array_diff($cls_id, $enr_cls_id);
  
  $cls_count = count($cls_id);
  if(count($diff)>0){
    $res = "N";
  }else{
    $select = db_select('slt_course_class', 'cls');
    $select->leftJoin('slt_business_rule_mapping','brl', 'brl.entity_id = cls.id and brl.entity_type = \'cre_sys_obt_cls\'');
    $select->addExpression('count(1)', 'cls');
    $select->condition('brl.business_rule_code', 'cre_sys_brl_rra', '=');
    $select->condition('cls.id', $classId, '=');
    $result = $select->execute()->fetchField();
    expDebug::dPrintDBAPI('getCourseClassRegisteredcount', $select);
    $res = ($result > 0) ? "Y": "N";
  }
  return array('recurring_res'=>$res,'class_cnt'=>$cls_count);
  
}

function getSendToMailCc($notification_sendto, $drupalUser) {
  $explodeNotificationSendTo = explode(',',$notification_sendto );
  expDebug::dPrint(' $explodeNotificationSendTo ' . print_r($explodeNotificationSendTo, true) , 4);
  foreach( $explodeNotificationSendTo as $key => $value) {
    if(trim($value) == "cre_ntn_rpt_adm") {
       $explodeNotificationSendTo[$key] =  variable_get('site_mail', 'info@expertusone.com');
      //$explodeNotificationSendTo[$key] = "NULL";
    }
    if(trim($value) == "cre_ntn_rpt_mgr") {
      $manager_id = $drupalUser['manager_id'];
      if(!empty($manager_id)) {
        $getManagerRes = getDrupalUserForNotificationInsert($manager_id);
        $managerEmailId = $getManagerRes['email'];
        $explodeNotificationSendTo[$key] =  $managerEmailId;
      }
      else {
        $explodeNotificationSendTo[$key] =  NULL;
      }
    }
    if (trim($value) == "cre_ntn_rpt_usr") {
      $explodeNotificationSendTo[$key] =  NULL;
    }
    if (trim($value) == "cre_ntn_rpt_ins") {
      $explodeNotificationSendTo[$key] =  $drupalUser['email'];
    }
  }

  $explodeNotificationSendTo = array_filter(array_unique($explodeNotificationSendTo));
  $returnCcValue = str_replace("NULL,", "", implode(",",$explodeNotificationSendTo ));
  expDebug::dPrint(' $explodeNotificationSendTo after = ' . print_r($explodeNotificationSendTo, true) , 4);
  expDebug::dPrint(' $returnCcValue = ' . print_r($returnCcValue, true) , 4);
  return $returnCcValue;
}


function getEmailId($code, $drupalUser) {
     if(trim($code) == "cre_ntn_rpt_adm") {
      $emailId =  variable_get('site_mail', 'info@expertusone.com');
      //$explodeNotificationSendTo[$key] = "NULL";
      expDebug::dPrint(' getEmaiId = ' . print_r($emailId, true) , 4);
      return $emailId;
      exit;
    }
    if(trim($code) == "cre_ntn_rpt_mgr") {
      $manager_id = $drupalUser['manager_id'];
      if(!empty($manager_id)) {
        $getManagerRes = getDrupalUserForNotificationInsert($manager_id);
        $managerEmailId = $getManagerRes['email'];
        $emailId =  $managerEmailId;
        expDebug::dPrint(' getEmaiId = ' . print_r($emailId, true) , 4);
        return $emailId;
        exit;
      }
      else {
        return "NULL";
        exit;
      }
    }
    if (trim($code) == "cre_ntn_rpt_usr") {
      $emailId =  $drupalUser['email'];
      expDebug::dPrint(' getEmaiId = ' . print_r($emailId, true) , 4);
      return $emailId;
      exit;
    }
    if (trim($code) == "cre_ntn_rpt_ins") {
      $emailId =  $drupalUser['email'];
      expDebug::dPrint(' getEmaiId = ' . print_r($emailId, true) , 4);
      return $emailId;
      exit;
    }
  }

function checkEquivalenceCourseComplete($userId, $courseId){
    try{
      $selectStmt = db_select('slt_common_mapping', 'map');
      //$selectStmt->innerJoin('slt_course_template','crs','(map.id1 = crs.id OR map.id2 = crs.id)');
      //$selectStmt->innerJoin('slt_course_template','temp','(map.id2 = temp.id OR map.id1 = temp.id)');
      $selectStmt->innerJoin('slt_enrollment','enr','(map.id2 = enr.course_id OR map.id1 = enr.course_id)'); 
      $selectStmt->innerJoin('slt_course_template','enrcrs','(enr.course_id = enrcrs.id)');
      $selectStmt->addField('map','id2','equv_id');
     //$selectStmt->addField('enrcrs','title','equv_title');
     //$selectStmt->addField('enr','reg_status','equv_status');
      $selectStmt->where( "((map.object_type = 'cre_sys_obt_crs') AND (map.type = 6) AND (map.id1 = $courseId))
          OR ((map.prereq_type = 'cre_sys_pre_crs') AND (map.type = 6) AND (map.id2 = $courseId))" );
      $selectStmt->condition('enr.user_id', $userId);
      $selectStmt->condition('enr.comp_status', array('lrn_crs_cmp_cmp') ,'IN');
      expDebug::dPrintDBAPI('MY EQUV checkEquivalenceTP--->>>' , $selectStmt);
      $selectStmt->groupBy('enr.course_id');
      $resultTP = $selectStmt->execute()->fetchAll();
      if(count($resultTP) >= 1) {
        return true;
      }
      else {
        return false;
      }
    }catch (Exception $ex) {
      watchdog_exception('checkEquivalence', $ex);
      expertusErrorThrow($ex);
    }

}

function updateUserPreference() {
	try {
	if(user_is_logged_in() ==  0){
			$result = array();
			$result['session'] ="session_out";
			print drupal_json_encode($result);
			exit;
	}
		$sltPersonId 		= getSltpersonUserId();
		$contentId 			=  $_POST['id'];
		$contentPositions 	=  $_POST['positions'];
		$action			 	=  $_POST['action'];
		$widgetId			=  $_POST['widgetId'];
		$salesForce			=  $_POST['salesforce'];
		//expDebug::dPrint('$contentId' . $contentId . '$action' . $action .'$widgetId' . $widgetId);
		// construct position array
		if (isset($contentPositions)) {
			$contentPosList 	= explode(',', $contentPositions);
			$changedOrderList 	= array_flip($contentPosList);
		}
		switch($contentId) {
			case 'highlight-list':
				if ($action == 'add') {
					$changedList = addPosition('mylearning_left', $widgetId);
					$deletedList = removeDeletedList($widgetId);
					$block_output = renderBlockContents($widgetId);
				} else if ($action == 'delete') {
					$changedList = removePostition('mylearning_left', $widgetId);
					unsetSessionForBlocks($widgetId);
					$deletedList = addDeletedList($widgetId);
				} else {
					$changedList = replacePostitions($changedOrderList, 'mylearning_left');
					$deletedList = getWidgetsDeleted(); // issue fix #52925
				}
				break;
			case 'sidebar_second-list':
				if ($action == 'add') {
					$changedList = addPosition('mylearning_right', $widgetId);
					$deletedList = removeDeletedList($widgetId);
					$block_output = renderBlockContents($widgetId);
				} else if ($action == 'delete') {
					$changedList = removePostition('mylearning_right', $widgetId);
					$deletedList = addDeletedList($widgetId);
				} else {
					$changedList = replacePostitions($changedOrderList, 'mylearning_right');
					$deletedList = getWidgetsDeleted(); // issue fix #52925
				}
				break;
		}

		if (count($changedList['mylearning_left'])<=0 && $action == 'delete') {
			return false;
			exit;
		}
		//expDebug::dPrint('changed list $updateUserPreference' . print_r($changedList, 1));
		drupal_add_js(array('mylearn_right' =>  $mylearning_right), 'setting');
		//expDebug::dPrint('deleted list' . print_r($deletedList, 1));
		// udpate in database
		db_merge('slt_person_preference')
		->key(array('user_id' => $sltPersonId))
		->updateFields(array(
				'widget_preference' => serialize($changedList),
				'widget_deleted' => $deletedList,
				'updated_by' => $sltPersonId,
				'updated_on' => now()
		))
		->insertFields(array(
				'user_id' => $sltPersonId,
				'widget_preference' => serialize($changedList),
				'widget_deleted' => $deletedList,
				'created_by' => $sltPersonId,
				'created_on' => now(),
				'updated_by' => $sltPersonId,
				'updated_on' => now()
		))
		->execute();
		if (module_exists('salesforce_integration') && $salesForce) {
			unset($changedList['mylearning_right']['announcement']); // remove the announcement block for salesforce
			unset($changedList['mylearning_right']['online_users']);
			unset($changedList['mylearning_left']['tab_instructor_desk_customized']);
		}
		$result = array();
		$result['sidebar'] = (count($changedList['mylearning_right'])<=0) ? false : true;
		$result['last_left_panel'] = (count($changedList['mylearning_left'])==1) ? true : false;
		//$result['left_sidebar'] = (count($changedList['mylearning_left'])<=0) ? false : true;
		//$result['widget_empty'] =  (count($changedList['mylearning_right'])<=0 && count($changedList['mylearning_left'])<=0) ? true : false;
		//$result['widget_deleted'] =  $deletedList;
		if ($action == 'add') {
			$result['content'] = $block_output;
		}
		$_SESSION['MyLearn_Pref'] = getUserPreference();
		print drupal_json_encode($result);
		exit;
	} catch(Exception $ex) {
		watchdog_exception('updateUserPreference', $ex);
		expertusErrorThrow($ex);
	}
}
function unsetSessionForBlocks($widget) {
	if($widget == 'tab_my_enrollment_customized') {
		unset($_SESSION['myenrolmentSearchStr']);
		unset($_SESSION['enrselectedFilters']);	
	}
	else if($widget == 'tab_my_learningplan_customized') {
		unset($_SESSION['prgselectedFilters']);
		unset($_SESSION['myprogramsSearchStr']);
	}
	else if($widget == 'tab_instructor_desk_customized') {
		unset($_SESSION['clsselectedFilters']);
		unset($_SESSION['myclassesSearchStr']);
	}
}
function getDrupalListNameBySection($section = '') {
	switch ($section) {
		case "mylearning_left":
			return "highlight-list";
			break;
		case "mylearning_right":
			return "sidebar_second-list";
			break;

	}
}

function renderBlockContents($deltaName = '') {
	$module = db_query("select module from block where delta = :delta and theme = :theme  limit 0,1", array(':delta' => $deltaName, ':theme' => 'expertusonev2'))->fetchField();
	$block = block_load($module, $deltaName);
	if ($block) {
		$render_array = _block_get_renderable_array(_block_render_blocks(array($block)));
		return render($render_array);
	}
	return false;
}
function getUserPreferenceListDeleted($salesForce = '') {
	try {
		global $user_preference;
		$loggedUserId = getSltpersonUserId();
		$isInstructorCheck		= getPersonDetails($loggedUserId, array('is_instructor')); // Instructor role check
		$defaultList  		= getUserPreference(true,$salesForce); // retirve default list
		$currentwidgetList 	= mergeChildArrays($user_preference);
		$defaultWidgetList 	= mergeChildArrays($defaultList, true);
		$deletedList		= array_diff_key($defaultWidgetList, $currentwidgetList);
		$masterEnrollCheck = masterEnrollmentRecordCountCheck();
		$personPrefCheck = personPreferenceCheck();
		$insSessionCheck = instructorSessionRecordCountCheck();
		$select = db_query("select widget_preference from slt_person_preference where user_id = :userid",array(':userid'=>$loggedUserId));
		$res = $select->fetchField();
		$found = 0;
		if (strpos($res, 'tab_my_learningplan_customized') !== false) {
			$found = 1;
		}
		$insfound = 0;
		if (strpos($res, 'tab_instructor_desk_customized') !== false) {
			$insfound = 1;
		}
		if($masterEnrollCheck == 0 && $found == 0 ) {
			$deletedList[tab_my_learningplan_customized] = 'highlight-list';
		}
		if(($insSessionCheck == 0 && $insfound == 0) && isset($isInstructorCheck['is_instructor']) && $isInstructorCheck['is_instructor'] == 'Y') {
			$deletedList[tab_instructor_desk_customized] = 'highlight-list';
		}
		if (module_exists('salesforce_integration') && $salesForce) {
			unset($deletedList['announcement']);
			unset($deletedList['online_users']);
			unset($deletedList['tab_instructor_desk_customized']);
		}
 		return $deletedList;

	} catch(Exception $ex) {
		watchdog_exception('getUserPreferenceListDeleted', $ex);
		expertusErrorThrow($ex);
	}
}

function getUserPreferenceDeletedList() {
	try {
		global $user;
		if(user_is_logged_in() ==  0){
			print "session_out";
			exit;
		}
		$salesForce = (int) $_GET['salesforce'];
		$deletedList = getUserPreferenceListDeleted($salesForce);
		if ($user->language == 'de') {
			$charLen = 12;
		}
		$listContent = '<ul id="manage-dd-list"></div>';
			if (!empty($deletedList)) {
				foreach($deletedList as $key => $value){
				$blockTitleFull = strtoupper(t('LBL287')) . ' '. strtoupper(getMyLearnBlockNamebyId($key));
				$blockTitle =  ($user->language == 'de' || $user->language == 'ru') ? titleController('DELETED-PANELS', $blockTitleFull, 28) : $blockTitleFull;
				$listContent .= '
					<li>
						<div class="dropdown-title vtip" onclick="$(&quot;body&quot;).data(&quot;learningcore&quot;).addLearnWidget(&quot;'.$value.'&quot;, &quot;'.$key.'&quot;);" title="'.$blockTitleFull.'">'.$blockTitle.'</div>
					</li>
	     			';
			}
		} else {
			$listContent .= '<li class="message"><div class="dropdown-msg">'.t('MSG701').'</div></li>';
		}
		$listContent .= '</ul>';
		print $listContent;
		exit;
	} catch(Exception $ex) {
		watchdog_exception('getUserPreferenceListDeleted', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 *  Function used to add part element to the array
 */
function addPosition($section = '', $addElement = ''){
	try {
		global $user_preference;
		$currentPostiion = max($user_preference[$section]) + 1;
		expDebug::dPrint('current postition of the element :' . $currentPostiion);
		$newElementArray = array(
			$addElement => $currentPostiion
		);
		$result = array_merge($user_preference[$section], $newElementArray);
		$user_preference[$section] = $result;
		return $user_preference;
	} catch(Exception $ex) {
		watchdog_exception('addPosition', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 *  Function used to remove part element from array
 */
function removePostition($section = '', $removeElement = '') {
	global $user_preference;
	if (isset($user_preference[$section][$removeElement])) {
		unset($user_preference[$section][$removeElement]);
		return $user_preference;
	}
	return $user_preference;
}

/**
 * Funciton used to chagne position array
 */
function replacePostitions($changedList, $section_name) {
	global $user_preference;
	foreach($user_preference[$section_name] as $key => $val) {
		if (array_key_exists($key, $changedList))
			$user_preference[$section_name][$key] =  $changedList[$key];
	}
	return $user_preference;
}



function mergeChildArrays($arrays, $section_name = '') {
	if(!is_array($arrays)) {
		return false;
	}
	$single_array = array();
	foreach($arrays as $array_key => $array_value) {
		if ($section_name) { // Section name asssigned as array value other wise weight will be default
			foreach($arrays[$array_key] as $key => $value) {
				$arrays[$array_key][$key] = getDrupalListNameBySection($array_key);
			}
		}
		$single_array += $arrays[$array_key];
	}
	return $single_array;
}

function getWidgetsDeleted($widgetId = '') {
	$sltPersonId 		= getSltpersonUserId();
	if ($widgetId) {
		$query = db_query("select FIND_IN_SET(:widget_id, widget_deleted) from slt_person_preference pref inner join slt_person per on per.id = pref.user_id
			where pref.user_id=:user_id",array(':widget_id' => $widgetId, ':user_id' => $sltPersonId));
	} else {
		$query = db_query("select widget_deleted from slt_person_preference pref inner join slt_person per on per.id = pref.user_id
			where pref.user_id=:user_id ",array(':user_id' => $sltPersonId));
	}
	$result = $query->fetchField();
	return $result;
}


function addDeletedList($widgetId = '') {
	$delList = getWidgetsDeleted();
	if ($delList == NULL) {
		$delListStr = $widgetId;
	} else {
		$delListArray = explode(',', $delList);
		array_push($delListArray, $widgetId);
		array_unique($delListArray);
		$delListStr = implode(',', $delListArray);
	}
	//expDebug::dPrint('addDeletedList' . $delListStr);
	return $delListStr;
}

function removeDeletedList($widgetId = '') {
	$delList = getWidgetsDeleted();
	if ($delList ==  NULL)
		return NULL;
	$delListArray = explode(',', $delList);
	if (($key = array_search($widgetId, $delListArray)) !== false) {
		unset($delListArray[$key]);
	}
	$delListStr = implode(',', $delListArray);
	//expDebug::dPrint('removeDeletedList' . $delListStr);
	return $delListStr;
}

/**
 * Function for getting the cron job count,
 *  if the given time exceeds the last_run_date value in table.
 * @param $jobName - Name of the cron job for which the last run value is checked.
 * @param $intervalMin - Value should be in integer. The given amount of value will be given as the minute to compare with last_run_date.
 * @return $cron_run_cnt - Value returned will be 1 if the given minutes exceed for the jobname, 0 if the given minutes not exceed for the jobname.
 * @authour - Shobana N.
 */
function checkCustomCronJob($jobName,$intervalMin){
	try{
		$select = db_select('slt_monitoring_jobs','jbrs');
		$select->addExpression('count(1)', 'jbrscount');
		$select->condition('jbrs.job_name',$jobName,'=');
		$select->where('now() > DATE_ADD(last_run_date, INTERVAL '.$intervalMin.' MINUTE)');
		expDebug::dPrintDBAPI(' $selectStmt for cron = ' , $select);
		$cron_run_cnt =  $select->execute()->fetchField();
		return $cron_run_cnt;
	}catch(Exception $e){
		expDebug::dPrint('checkCustomCronJob' . $e,1);
	}
}


function getenabledCurrencyAPI(){
	try{
		$select = db_select('slt_profile_list_items', 'splt');
		$select->addField('splt', 'code', 'code');
		$select->addField('splt', 'name', 'name');
		$select->addField('splt', 'attr1', 'label');
		$select->addField('splt', 'attr2', 'symbol');
		$select->condition('is_active', 'Y');
		$select->condition('attr3', 'Y');
		$select->condition('splt.code', 'cre_sys_crn_%', 'LIKE');
		$select->orderBy('label', 'ASC');

		$enabledCurrencies =  $select->execute()->fetchAll();
		return $enabledCurrencies;
	}catch(Exception $e){
		expDebug::dPrint('getenabledCurrencyAPI' . $e,1);
	}
}

function splitCurrencyInputAndSetInRequest(){
	try{
		$currency_code = $_REQUEST['currency_code'];
		expDebug::dPrint('Currency code = ' .$currency_code);
		
		if(!empty($currency_code) && $_REQUEST['mobile_request'] != 1)
		{
		
			$currency_input = $currency_code;
			if(strpos($currency_code,'_~') !== false)
			{
				//For mobile API validation
				$currency_array = array();
				$currency_array = explode("_~",$currency_code);
				$currency_input = $currency_array[0];
			}
		
			$selectQuery = db_select('slt_profile_list_items', 'spli');
			$selectQuery->addField('spli','attr1');
			$selectQuery->condition('spli.is_active', 'Y', '=');
			$selectQuery->condition('spli.attr3', 'Y', '=');
			$db_or = db_or();
			$db_or->condition('spli.code', $currency_input, '=');
			$db_or->condition('spli.attr1', $currency_input, '=');
			$selectQuery->condition($db_or);
		
			expDebug::dPrintDBAPI('Whats the query = ',$selectQuery);
			$currency_output = $selectQuery->execute()->fetchField();
		
			if(empty($currency_output))
			{
				return '';
			}
		}
		expDebug::dPrint('Currency code later = ' .$currency_code);
		
		
		if($_REQUEST['version'])
		{
			//Below code is for mobile app
			$_REQUEST['currency_type'] = 'USD';
			$_REQUEST['currency_sym'] = '$';
		}
		else if (strpos($currency_code,'cre_') !== false) {
			$select = db_select('slt_profile_list_items', 'splt');
			$select->addField('splt', 'attr1', 'code');
			$select->addField('splt', 'attr2', 'symbol');
			$select->condition('splt.code', $currency_code);
			$currency_output =  $select->execute()->fetchField();
			$_REQUEST['currency_type'] = $currency_output;
			//Do not add symbol here. So that it will check if the given input currency is enabled or. 
			//Added for mobile app to reduce performance issue
		}
		else
		{
			$currency_array = array();
			$currency_array = explode("_~",$currency_code);
			expDebug::dPrint('Currency code 0 = ' .$currency_array[0]);
			expDebug::dPrint('Currency code 1 = ' .$currency_array[1]);
			$_REQUEST['currency_type'] = $currency_array[0];
			$_REQUEST['currency_sym'] = $currency_array[1];
		}
		
		return 'currency valid';
		
	}catch(Exception $e){
		expDebug::dPrint('splitCurrencyInputAndSetInRequest' . $e,1);
	}
}

function getCurrencyBaseRateAPI(){
	try{

		expDebug::dPrint('Whats the input = ' .$_REQUEST['selected_currency']);
		$select = db_select('slt_currency_conversion_details','sltconv');
		$select->leftjoin('slt_profile_list_items', 'slpt','sltconv.base_type=slpt.attr1');
		$select->addField('sltconv', 'base_type', 'base_type');
		$select->addField('sltconv', 'convert_type', 'convert_type');
		$select->addField('sltconv', 'rate', 'base_rate');
		$select->condition('slpt.is_active','Y','=');
		$select->condition('sltconv.convert_type',$_REQUEST['selected_currency'],'=');

		expDebug::dPrintDBAPI(' $selectStmt for multicurrency base details = ' , $select);
		$cron_run_cnt =  $select->execute()->fetchAll();
		return $cron_run_cnt;
	}catch(Exception $e){
		expDebug::dPrint('getCurrencyBaseRateAPI' . $e,1);
	}
}

/* Multi Currency CRON API CODE */
function multicurrencyConversionUpdate(){
	if (module_exists('exp_sp_callout') && calloutIsActive() &&  calloutEntityIsActive('cme_crn_upd')) {
		expDebug::dprint('$exp_sp_callout CRON');
		$rateCallout = getCalloutEntityInfo('cme_crn_upd');
		$ratecallouturl=$rateCallout['file'];
		require_once DRUPAL_ROOT. $ratecallouturl;
		$merageallcode = getActiveCurrencyList();
		processCalloutCurrencyRateRequest('cron',$merageallcode);
	}else{
		$access_token=currencyapiAuthorization();
		$notexistcurrency = array();
		$failurecurrency = array();
		expDebug::dPrint('$multicurrencyConversionUpdate' . $access_token);
		$transaction = db_transaction();
		try{
			$baseresult = getActiveCurrencyList();
			$to_count=count($baseresult);
			expDebug::dPrint('updateCurrencyList fetchCol = ' .$to_count .' '. print_r($baseresult, true), 5);
			$to_implode=implode(",",$baseresult);
			expDebug::dPrint('updateCurrencyList fetchCol Implode = ' . print_r($to_implode, true), 5);

			foreach ($baseresult as $basetype){
				$conversionapiresult = rateConversionAPI($basetype,$baseresult,$access_token);
				$jsonresult = json_decode($conversionapiresult);
				expDebug::dPrint('updateCurrencyList fetchCol Result = ' . print_r($jsonresult, true), 5);
				$base_currency=$jsonresult->base_currency;
				$restotal=$jsonresult->total;
				$result1=$jsonresult->result;
				$api_response_status=$jsonresult->status;
				if($api_response_status=="Success"){
				if($restotal > 0){
				for ($k=0;$k<$restotal;$k++){
					$convert_type = $result1[$k]->convert_type;
					$rate = $result1[$k]->rate;
					$select = db_update('slt_currency_conversion_details');
					$select->fields(array('rate'=>$rate));
					$select->condition('base_type', $base_currency, '=');
					$select->condition('convert_type', $convert_type, '=');
					expDebug::dPrintDBAPI('multicurrencyConversionUpdate Code', $select);
					$select->execute();
				}
				$objectId = get_slt_profile_entity_id($base_currency);
				$msgsave=$base_currency.'-Update Currency conversion rate - CRON';
				add_audit_trail_entry('1', $objectId, 'cre_sys_crn', 'multicurrencyConversionUpdate', 'updateCurrencyConversionRate',$msgsave,'1','Y','Y');
				}else{
					/* Audit trail log */
					$objectId = get_slt_profile_entity_id($base_currency);
					$msgsave = $base_currency.'-Not Updated - Currency does not exist in cc server - CRON';
					add_audit_trail_entry('1', $objectId, 'cre_sys_crn', 'multicurrencyConversionUpdate', 'updateCurrencyConversionRate', $msgsave,'1','Y','Y');
					/* Audit trail log */
					$notexistcurrency[]=$base_currency;					
				}
				}else{
				// API Response fail
					$objectId = get_slt_profile_entity_id($basetype);
					$msgsave = $basetype.'-CC Server API Response fail - CRON';
					add_audit_trail_entry('1', $objectId, 'cre_sys_crn', 'multicurrencyConversionUpdate', 'updateCurrencyConversionRate', $msgsave,'1','Y','Y');
					$failurecurrency[]=$basetype;					
				}
			}
			if(count($notexistcurrency)>0) { currencyConversionStatusMail('ccmissing',$api_response_status,'empty',$notexistcurrency); } // Mail - Missing Currency
			if(count($failurecurrency)>0) { currencyConversionStatusMail('ccfail',$api_response_status,$jsonresult->message,$failurecurrency); }// Mail - API Response Fail			

		}catch (Exception $e) {
			$transaction->rollback();
			unset($transaction);
			watchdog_exception('mostActiveUsersUpdate', $e);
			expertusErrorThrow($e);
		}
		unset($transaction);
	}

}

function getActiveCurrencyList(){
	try {
		$sel = "SELECT DISTINCT base_type FROM slt_currency_conversion_details ORDER BY base_type ASC";
		$query = db_query($sel);
		$result = $query->fetchCol();
		return $result;
	} catch (Exception $e) {
		expertusLogError($e);
	}
}

function rateConversionAPI($btype,$ctype,$access_token){
	$cc_server_url=getConfigValue('cc_server_url');
	$cc_server_vendor=getConfigValue('cc_server_vendor');
	$host = getConfigValue('admin_site_url');
	$response_format = getConfigValue('cc_response_format');
	expDebug::dprint('rateConversionAPI $Entry');
	$to_implode = implode(",",$ctype);
	expDebug::dprint('$rateConversionAPI $$to_implode '.print_r($to_implode,true),5);
	$fieldsval=array(
			'from' => $btype,
			'to' => $to_implode,
			'format' => $response_format,
			'access_token' => $access_token,
			'vendor' => $cc_server_vendor,
			'host' => $host
	);
	expDebug::dprint('$rateConversionAPI $$$fieldsval '.print_r($fieldsval,true),5);
    $post_field_string = http_build_query($fieldsval, '', '&');
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $cc_server_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: application/'.$response_format.''));
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_field_string);
    curl_setopt($ch, CURLOPT_POST, true);
    $json = curl_exec($ch);
    if($response_format=='json'){
    	expDebug::dprint('$rateConversionAPI $json '.print_r($json,true),5);
    	$result_response = $json;
    }else{
    	expDebug::dprint('$rateConversionAPI $$xml '.print_r($json,true),5);
    	$xml = simplexml_load_string($json);
    	$result_response = json_encode($xml);
    }
    if(!$json) {
    	echo curl_error($ch);
    }
    curl_close ($ch);
    $insert_cc_log = db_insert('slt_currency_server_logs');
    $fields = array(
    		'vendor_id'         => $cc_server_vendor,
    		'base_type'      => $btype,
    		'request'        => serialize($fieldsval),
    		'response'        => serialize($json),
    		'created_on'        => date('Y-m-d H:i:s')
    );
    $insert_cc_log->fields($fields);
    expDebug::dPrintDBAPI('INSERT of slt_currency_server_logs record ', $insert_cc_log);
    $insert_cc_log->execute();
    return $result_response;
}

function get_slt_profile_entity_id($code){
	expDebug::dprint('get_slt_profile_entity_id'.$code);
	$selectcount = db_query("select id from slt_profile_list_items where attr1='$code' AND code LIKE 'cre_sys_crn_%'");
	return $selectcount->fetchfield();
}

function currencyapiAuthorization(){
	$site_url=getConfigValue('admin_site_url');
	$client_id=getConfigValue('oauth_client_id');
	$client_secret=getConfigValue('oauth_client_secret');
	$peerVerify = getConfigValue('peer_verify') == 0 ? FALSE : TRUE;;
	/*$sel=db_query("SELECT access_token  FROM slt_oauth_access_tokens WHERE expires > (NOW() - INTERVAL 30 MINUTE) AND scope='E1.API' AND client_id ='$client_id' order by expires DESC LIMIT 1 ");
	$token_count=$sel->rowCount();
	if($token_count > 0){
		$token = $sel->fetchField();
		expDebug::dPrint('currencyapiAuthorization $Exist = ' . print_r($token, true), 5);
	}else{	*/

		$client_details = base64_encode("$client_id:$client_secret");
		$context = stream_context_create(array(
		'http' => array(
		'method' => 'POST',
		'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
					"Authorization : Basic $client_details\r\n",
		'content' => "grant_type=client_credentials"
    		),
		    'ssl' => array('verify_peer'=>$peerVerify, 
                    'verify_peer_name'=>$peerVerify
		)
		));
		expDebug::dPrint('cliet id in currency api auth '.$client_id, 5);
		expDebug::dPrint('cliet secret in currency api auth '.$client_secret, 5);
		expDebug::dPrint('cliet details in currency api auth '.$client_details, 5);
		expDebug::dPrint('peer_verify in currency api auth '.var_export($peerVerify, 1), 5);
		$response = file_get_contents($site_url.'/apis/oauth2/Token.php', false, $context);
		expDebug::dPrint('$response from token gen '.var_export($response, 1), 5);
		$res =  json_decode($response);
		$token = $res->access_token;
		expDebug::dPrint('currencyapiAuthorization $New = ' . print_r($token, true), 5);
	//}
	return $token;
}

function currencyConversionStatusMail($type,$status,$resmessage,$currency='') {
	// API Response fail
	$to = getConfigValue('cc_report_mail');
	$currency_list = implode("," ,$currency);
	$admin_site_url=getConfigValue('admin_site_url');
	$cc_server_vendor=getConfigValue('cc_server_vendor');
	if($resmessage!=''){
		$finalresmessage = $resmessage;
	}else{
		$finalresmessage = "Currency Conversion Server API Response - Failure";
	}
	if($type=="ccfail"){
		$subject = "Currency Conversion API Response - Failure";
		$message = '<table width="50%" cellpadding="5">
							<tr><td>Vendor ID: </td><td>'.$cc_server_vendor.'</td></tr>
							<tr><td>Host: </td><td>'.$admin_site_url.'</td></tr>
							<tr><td>Status: </td><td>Failure</td></tr>
							<tr><td>Currency List: </td><td>'.$currency_list.'</td></tr>
							<tr><td>Message: </td><td>'.$finalresmessage.'</td></tr>
							</table>';
	}elseif ($type=="ccmissing"){
		$subject = "Currency Conversion API Response - Currency Not Inserted";
		$message = '<table width="50%" cellpadding="5">
							<tr><td>Vendor ID: </td><td>'.$cc_server_vendor.'</td></tr>
							<tr><td>Host: </td><td>'.$admin_site_url.'</td></tr>
							<tr><td>Status: </td><td>Failure</td></tr>
							<tr><td>Currency List: </td><td>'.$currency_list.'</td></tr>
							<tr><td>Message: </td><td>Currency does not exist in currency conversion server. </td></tr>
							</table>';
	}
	// Always set content-type when sending HTML email
	$headers = "MIME-Version: 1.0" . "\r\n";
	$headers .= "Content-type:text/html;charset=UTF-8" . "\r\n";
	$headers .= 'From: <noreply@expertusone.com>' . "\r\n";
	mail($to,$subject,$message,$headers);
}
/* Multi Currency CRON API CODE */

/* TP certificate Expiry and TP Learning plan Incomplete */
/*function change_lp_expired_status(){

  $notificationTemplateInfo = getNotificationTemplateInfo('certification_expire', 'cre_sys_lng_eng');

  $notifyFlag = ($notificationTemplateInfo['status']  == 'cre_ntn_sts_atv') ? 'Y' : 'N';
  $emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
  $ccEmail = $notificationTemplateInfo['notification_cc'];

  insertNotificationScript($emailSendTo,$notifyFlag);

  $updqry  = "UPDATE slt_master_enrollment mse
  INNER JOIN slt_program prgm ON prgm.id = mse.program_id
  SET mse.overall_status  = 'lrn_tpm_ovr_exp', mse.updated_on =NOW()
  where
  (prgm.object_type = 'cre_sys_obt_crt') AND (mse.overall_status = 'lrn_tpm_ovr_cmp') AND (mse.is_current = 'Y') AND (mse.comp_date IS NOT NULL )
  AND (prgm.expires_in_value IS NOT NULL ) AND (prgm.expires_in_unit IS NOT NULL ) AND (prgm.expires_in_value > 0)
  AND (DATEDIFF(
  IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
  IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
  DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
  CURDATE()) < 0) ";
  expDebug::dPrint('update master enrolment :: '. $updqry);
  db_query($updqry)->execute();


  $updqry1 = "UPDATE slt_skill_set a
  INNER JOIN slt_master_enrollment mse on mse.id = a.master_enrollment_id
  INNER JOIN slt_program prgm ON prgm.id = mse.program_id
  SET a.is_deleted  = 1
  where
  (prgm.object_type = 'cre_sys_obt_crt') AND (mse.overall_status = 'lrn_tpm_ovr_exp') AND (mse.is_current = 'Y') AND (mse.comp_date IS NOT NULL )
  AND (prgm.expires_in_value IS NOT NULL ) AND (prgm.expires_in_unit IS NOT NULL ) AND (prgm.expires_in_value > 0)
  AND (DATEDIFF(
  IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
  IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
  DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
  CURDATE()) < 0) ";

  expDebug::dPrint('Update skill :: '. $updqry1);

  db_query($updqry1);

  $query = "UPDATE slt_enrollment a
  INNER JOIN slt_master_enrollment mse on mse.id = a.master_enrollment_id
  INNER JOIN slt_program prgm ON prgm.id = mse.program_id
  set a.comp_status = 'lrn_crs_cmp_inc',
  a.updated_on = now(),
  a.updated_by = 0
  where prgm.object_type = 'cre_sys_obt_trn' AND prgm.end_date IS NOT NULL
  AND (IF(prgm.end_date IS NOT NULL, DATEDIFF(CURDATE(), prgm.end_date) > 0,1=1))
  AND (if(a.comp_status IS NOT NULL, a.comp_status IN ('lrn_crs_cmp_enr','lrn_crs_cmp_inp'),a.reg_status IN('lrn_crs_reg_ppm','lrn_crs_reg_ppv','lrn_crs_reg_wtl')))";


  expDebug::dPrint("Update slt_enrollment table :: ".$query);

  db_query($query);

  $qry = "UPDATE slt_master_enrollment mse
  INNER JOIN slt_program prgm ON prgm.id = mse.program_id
  SET mse.overall_status  = 'lrn_tpm_ovr_inc',
  mse.updated_on = now(),
  mse.updated_by = 0
  WHERE prgm.object_type = 'cre_sys_obt_trn' AND prgm.end_date IS NOT NULL
  AND (IF(prgm.end_date IS NOT NULL, DATEDIFF(CURDATE(), prgm.end_date) > 0,1=1))
  AND mse.overall_status IN ('lrn_tpm_ovr_enr','lrn_tpm_ovr_inp','lrn_tpm_ovr_ppm','lrn_tpm_ovr_ppv')";

  expDebug::dPrint("Update slt_master_enrollment table :: ".$qry);

  db_query($qry);

}
*/
function insertNotificationScript($emailSendTo,$notifyFlag){

  $select = db_select('slt_master_enrollment','mse');
  $select->innerJoin('slt_program','prgm','prgm.id = mse.program_id');
  $select->innerJoin('slt_profile_list_items','pfli','pfli.code =  prgm.object_type');
  $select->innerJoin('slt_person','per','per.id = mse.user_id');
  $select->innerJoin('users','usr','usr.name = per.user_name');
  $select->addExpression('\'certification_expire\'', 'msg_id');
  $select->addExpression('\'cre_sys_lng_eng\'', 'lang_code');
  $select->addExpression('\'Certification Expiry\'', 'msg_type');
  $select->addExpression("concat('delivery_type>|',ifnull(pfli.name,''),'~|course_title>|',ifnull(prgm.title,''),'~|tp_title>|',ifnull(prgm.title,''),
      '~|tp_expirydate>|',IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
      IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
      DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),'~|course_completed_date>|',mse.comp_date,
      '~|program_id>|',ifnull(prgm.id,''),'~|user_name>|',ifnull(per.user_name,''),'~|first_name>|',ifnull(per.first_name,''),
      '~|last_name>|',ifnull(per.last_name,''),'~|user_email>|',ifnull(per.email,''),'~|user_phone>|',ifnull(per.phone_no,''),'~|learning_fullname>|',
      ifnull(per.full_name,''),'~|full_name>|',ifnull(per.full_name,''))", "token_str");



  if(in_array("cre_ntn_rpt_adm", $emailSendTo)){
    $select->addField('per','id','user_id');
    $select->addExpression('null','send_to_id');
    $select->addExpression('null','send_to_name');
    $select->addExpression('null','send_to_email');
    $select->addExpression('null', 'send_to_email_cc');
    $select->addExpression('1', 'custom0');
  }
  if(in_array("cre_ntn_rpt_usr", $emailSendTo)){
    $select->addField('per','id','user_id');
    $select->addField('usr','uid','send_to_id');
    $select->addField('per','full_name','send_to_name');
    $select->addField('usr','mail','send_to_email');
    $select->addExpression('null', 'send_to_email_cc');
  }


  if(in_array("cre_ntn_rpt_mgr", $emailSendTo)){
    $select->leftJoin('slt_person','mgr','mgr.id = per.manager_id');
    $select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
    $select->addField('mgr','id','user_id');
    $select->addField('mgr1','uid','send_to_id');
    $select->addField('mgr','full_name','send_to_name');
    $select->addField('mgr1','mail','send_to_email');
    $select->addExpression('null', 'send_to_email_cc');
  }

  $select->addExpression('\'php mailer\'', 'send_type');
  $select->addExpression('\''.$notifyFlag.'\'', 'send_status');
  $select->addExpression('1', 'created_by');
  $select->addExpression('now()', 'created_on');
  $select->addExpression('1', 'updated_by');
  $select->addExpression('now()', 'updated_on');

  $select->condition('prgm.object_type', 'cre_sys_obt_crt', '=');
  $select->condition('mse.overall_status', 'lrn_tpm_ovr_cmp', '=');
  $select->condition('mse.is_current','Y','=');
  $select->isNotNull('mse.comp_date');
  $select->isNotNull('prgm.expires_in_value');
  $select->isNotNull('prgm.expires_in_unit');
  $select->condition('prgm.expires_in_value', 0, '>');
  $select->where("DATEDIFF(
      IF (prgm.expires_in_unit = 'days', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value DAY),
      IF (prgm.expires_in_unit = 'months', DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value MONTH),
      DATE_ADD(mse.comp_date, INTERVAL prgm.expires_in_value YEAR))),
      CURDATE()) < 0");

  expDebug::dPrintDBAPI('change_lp_expired_status :: ', $select);


  db_insert('slt_notification')
  ->from($select)
  ->execute();


}
function bulk_class_cancel_notification_insert($class_id,$course_id,$createUserId){
	$selectStmt = db_select('slt_course_class', 'scc');
	$selectStmt->addField('scc', 'delivery_type', 'delivery_type');
	$selectStmt->condition('scc.id', $class_id,'=');
	$result = $selectStmt->execute()->fetchAll();
	expDebug::dPrintDBAPI('slt_course_class :: yogaraja', $selectStmt);
	expDebug::dPrint(' Notification info $result = ' . print_r($result, true) , 4);
	if($result[0]->delivery_type=='lrn_cls_dty_wbt' || $result[0]->delivery_type=='lrn_cls_dty_vod')
		$notificationTemplateInfo = getNotificationTemplateInfo('class_cancel_wbt_vod', 'cre_sys_lng_eng');
	elseif ($result[0]->delivery_type=='lrn_cls_dty_ilt' || $result[0]->delivery_type=='lrn_cls_dty_vcl')
	$notificationTemplateInfo = getNotificationTemplateInfo('class_cancel', 'cre_sys_lng_eng');
	expDebug::dPrint(' Notification info string = ' . print_r($notificationTemplateInfo, true) , 4);
	if($notificationTemplateInfo['status']!= 'cre_ntn_sts_atv'){
		$updateenr = db_update('slt_enrollment');
		$updateenr->fields(array(
				'browser_session'=>null
		));
		$updateenr->condition('browser_session','bulk_cancel_notification_pending','=');
		$updateenr->condition('class_id',$class_id,'=');
		$updateenr->execute();
		
		
		$update = db_update('slt_monitoring_jobs');
		$update->fields(array(
				'custom0'=>0
		));
		$update->condition('job_name','bulk_class_cancel_notification','=');
		$update->execute();
		return;
	}
	$notifyFlag =  'N';
	$limit = 50000;
	$emailSendTo = explode(',',$notificationTemplateInfo['notification_sendto']);
	$ccEmail = $notificationTemplateInfo['notification_cc'];
	$tokenStr = generateEnrollmentCancelNotifications();
	expDebug::dPrint(' Doken string = ' . print_r($tokenStr, true) , 4);

	$select = db_select('slt_enrollment','enr');
	$select->innerJoin('slt_course_class','cls','cls.id = enr.class_id');
	$select->innerJoin('slt_course_template','crs','crs.id = enr.course_id');
	$select->innerJoin('slt_profile_list_items','pfli','pfli.code =  cls.delivery_type');
	$select->innerJoin('slt_person','per','per.id = enr.user_id');
	$select->innerJoin('users','usr','usr.name = per.user_name');
	$select->addExpression("'".$tokenStr['message_id']."'", 'msg_id');
	$select->addExpression("'".$tokenStr['message_type']."'", 'msg_type');
	$select->addExpression('IF(per.preferred_language IS NULL or per.preferred_language = \'\', \'cre_sys_lng_eng\', per.preferred_language)', 'lang_code');
	$select->addExpression("CONCAT(".$tokenStr['tokens_string'].")", "token_str");

	if(in_array("cre_ntn_rpt_adm", $emailSendTo)){
		$select->addField('per','id','user_id');
		$select->addExpression('null','send_to_id');
		$select->addExpression('null','send_to_name');
		$select->addExpression('null','send_to_email');
		$select->addExpression('null', 'send_to_email_cc');
		$select->addExpression('1', 'custom0');
	}
	if(in_array("cre_ntn_rpt_usr", $emailSendTo)){
		$select->addField('per','id','user_id');
		$select->addField('usr','uid','send_to_id');
		$select->addField('per','full_name','send_to_name');
		$select->addField('usr','mail','send_to_email');
		$select->addExpression('null', 'send_to_email_cc');
	}
	if(in_array("cre_ntn_rpt_mgr", $emailSendTo)){
		$select->leftJoin('slt_person','mgr','mgr.id = per.manager_id');
		$select->leftJoin('users','mgr1','mgr1.name = mgr.user_name');
		$select->addField('mgr','id','user_id');
		$select->addField('mgr1','uid','send_to_id');
		$select->addField('mgr','full_name','send_to_name');
		$select->addField('mgr1','mail','send_to_email');
		$select->addExpression('null', 'send_to_email_cc');
	}

	$select->addExpression('\'php mailer\'', 'send_type');
	$select->addExpression('\''.$notifyFlag.'\'', 'send_status');
	$select->addExpression('\''.$createUserId.'\'', 'updated_by');
	$select->addExpression('\''.$createUserId.'\'', 'created_by');
	$select->addExpression('now()', 'created_on');
	$select->addExpression('now()', 'updated_on');

	//$select->condition('enr.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv'), 'IN');
	//$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp' ,'<>');
	$select->condition('enr.browser_session', 'bulk_cancel_notification_pending' ,'=');
	$select->condition('enr.class_id', $class_id);
	$select->condition('enr.course_id', $course_id);
	$select->orderBy('enr.id','desc');
	$select->range(0,$limit);
	expDebug::dPrintDBAPI('change_lp_expired_status :: ', $select);


	db_insert('slt_bulk_notification')
	->from($select)
	->execute();
	expDebug::dPrintDBAPI('change_lp_expired_status :: yogaraja', $select);

	$update_query = db_update('slt_enrollment');
	$fields = array(
			'updated_by' => $createUserId,
			'browser_session' =>null,
			'updated_on' =>now(),
	);
	$update_query->fields($fields);
	$update_query->where('id in ( select * from (select id from slt_enrollment as enr where enr.class_id ='.$class_id.' and enr.course_id='.$course_id.' and enr.browser_session = \'bulk_cancel_notification_pending\' order by enr.id desc limit 0,'.$limit.') as tmp)');
	expDebug::dPrintDBAPI(' $$update_query SQL = ', $update_query);
	$update_query->execute();
	syncSolrData('Enrollment');
}
function generateEnrollmentCancelNotifications(){
	try{
		$notificationInfo = array();
		$notificationInfo['tokens_string'] = "'delivery_type>|',ifnull(cls.delivery_type,''),'~|course_title>|',ifnull(crs.title,''),'~|
		training_title>|',ifnull(crs.title,''),'~|class_title>|',ifnull(cls.title,''),'~|dt_code>|',ifnull(cls.delivery_type,''),'~|first_name>|',ifnull(per.first_name,''),
		'~|last_name>|',ifnull(per.last_name,''),'~|full_name>|',ifnull(per.full_name,''),'~|learning_fullname>|',ifnull(per.full_name,''),'~|user_name>|',ifnull(per.user_name,''),
		'~|user_email>|',ifnull(per.email,''),'~|user_phone>|',ifnull(per.phone_no,''),'~|'";

		expDebug::dPrint("Checking the generateEnrollmentDropAndCancelNotifications".print_r($notificationInfo,1));
		$notificationInfo['message_id'] = 'class_cancel';
		$notificationInfo['message_type'] = 'Class Cancel By Admin';
		return $notificationInfo;
	}catch(Exception $e){
		expDebug::dPrint("ERROR in Drop Cancel notification ".print_r($e,true),1);
		throw new Exception($e->getMessage());
	}
}
function generateWaivedEnrollmentNotification($notificationType,$objectId1,$objectId2,$userId){
	try{
		$tblname = ($objectId2 == 'class') ? 'slt_enrollment' : 'slt_master_enrollment';
		$qry = db_select($tblname,'enr');
		$qry->innerJoin('slt_person','per','per.id = enr.user_id AND per.id = '.$userId);
		if($objectId2 == 'class'){
			$qry->innerJoin('slt_course_template','crs','crs.id = enr.course_id');
			$qry->innerJoin('slt_course_class','cls','cls.course_id = crs.id');
			$qry->innerJoin('slt_profile_list_items','spli','spli.code = cls.entity_type');
			$qry->addField('crs', 'title');
			$qry->addField('cls', 'additional_info');
			$qry->addField('cls', 'addn_notification_show');
		} else{
			$qry->innerJoin('slt_program','prg','prg.id = enr.program_id');
			$qry->innerJoin('slt_profile_list_items','spli','spli.code = prg.object_type');
			$qry->addField('prg', 'title');
			$qry->addField('prg', 'additional_info');
			$qry->addField('prg', 'addn_notification_show');
		}
		$qry->addField('spli', 'name','objecttype');
		$qry->addField('per', 'first_name');
		$qry->addField('per', 'last_name');
		$qry->addField('per', 'user_name');
		$qry->addField('per', 'full_name');
		$qry->condition('enr.id',$objectId1,'=');
		$qry->condition('enr.user_id',$userId,'=');
		expDebug::dPrintDBAPI("Query for generateWaivedEnrollmentNotification",$qry);
		$getenrDet = $qry->execute()->fetchAll();
		expDebug::dPrint("Result for generateWaivedEnrollmentNotification".print_r($getenrDet,1));
		$firstname   = convertNullValToNullStr(ucfirst($getenrDet[0]->first_name));
		$lastname   = convertNullValToNullStr(ucfirst($getenrDet[0]->last_name));
		$username   = convertNullValToNullStr(ucfirst($getenrDet[0]->user_name));
		$fullname   = convertNullValToNullStr(ucfirst($getenrDet[0]->full_name));
		$additional_info1   = convertNullValToNullStr(ucfirst($getenrDet[0]->additional_info));
		$addn_notification_show1   = convertNullValToNullStr(ucfirst($getenrDet[0]->addn_notification_show));

		$notificationInfo['tokens_string'] = 'first_name>|' . $firstname . '~|'.
				'type>|' . $getenrDet[0]->objecttype. '~|'.
				'last_name>|' . $lastname. '~|'.
				'user_name>|' . $username. '~|'.
				'full_name>|' . $fullname. '~|'.
				'course_title>|' . $getenrDet[0]->title. '~|'.
				(($addn_notification_show1 == 1) ? (($additional_info1) ? ('add_notes>|'. $additional_info1) : ('add_notes>|'.'-')) : ''). '~|';

		$notificationInfo['message_id']    = $notificationType;
		$notificationInfo['message_type']  = ($notificationType == 'enrollment_waived') ? 'Erollment marked as Waived' : 'Waived Status is reverted';
		$notificationInfo['send_type']     = 'php mailer';
		$notificationInfo['lang_code']     = '';
		return $notificationInfo;

	} catch(Exception $e){
		expDebug::dPrint('Error in generateWaivedEnrollmentNotification'.$e,1);
		expertusLogError($e);
	}
}


function DeepLinkSurveyAPI($surUserId ,$surLinkType,$surObjectId,$surObjectType,$surEnrollId,$enrolledstatus,$surSurveyId){

   	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_survey/exp_sp_survey_learner/exp_sp_surveylearner.inc');
   	global $user;
	
		

		switch($surObjectType) {
			case 'Class':
				$surObjectType = 'cre_sys_obt_cls';
				break;
			case 'Curricula':
				$surObjectType = 'cre_sys_obt_cur';
				break;
			case 'Certification':
				$surObjectType = 'cre_sys_obt_crt';
				break;
			case 'Trainingplan':
			case 'LearningPlan':
				$surObjectType = 'cre_sys_obt_trn';
				break;
		}
		
		$userId = $surUserId;
		
		//Check the Class/Trainingplan has Surveys
		$selectSurvey = db_select('slt_survey_mapping','smap');
		$selectSurvey->addExpression('count(smap.survey_id)','surveymapcount');
		$selectSurvey->condition('smap.object_id',$surObjectId,'=');
		$selectSurvey->condition('smap.object_type',$surObjectType,'=');
		$resultSurvey = $selectSurvey->execute()->fetchAssoc();
		$surveymapcount = $resultSurvey['surveymapcount'];	
		
		if($surveymapcount > 0) {
			
		//Get the Class Title and Course Id
		if($surObjectType=='cre_sys_obt_cls') {
			$table_name = 'slt_course_class';
			$status = array('lrn_cls_sts_atv','lrn_cls_sts_itv','lrn_cls_sts_dld');
		} else {
			$table_name = 'slt_program';
			$status = array('lrn_lpn_sts_atv');
		}
		
		// collect course/program information from received values
		$select_title = db_select($table_name,'obj');
		$select_title->addField('obj','title','objecttitle');
		if($surObjectType=='cre_sys_obt_cls')
			$select_title->addField('obj','course_id','courseid');
		$select_title->condition('obj.id',$surObjectId,'=');
		$select_title->condition('obj.status',$status,'IN');
		$result_title = $select_title->execute()->fetchAssoc();
			
			$surveyObjectName = rawurlencode($result_title['objecttitle']);
			$surveyCourseId = $result_title['courseid'];
		
		//Check Logged User has any Enrollemnts
		if($surObjectType=='cre_sys_obt_cls') {
			$table_name2 = 'slt_enrollment';
			$field_name2 = 'comp_status';
			$status2 =  array('lrn_crs_cmp_cmp','lrn_crs_cmp_enr','lrn_crs_cmp_inp');
		} else {
			$table_name2 = 'slt_master_enrollment';
			$field_name2 = 'overall_status';
			$status2 = array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_enr','lrn_tpm_ovr_inp');
		}
		$selectEnroll = db_select($table_name2,'enr');
		$selectEnroll->addExpression('count(enr.id)','enrollcount');
		if($surObjectType=='cre_sys_obt_cls') {
			$selectEnroll->condition('enr.class_id',$surObjectId,'=');
			$selectEnroll->condition('enr.course_id',$surveyCourseId,'=');
		}
		else
			$selectEnroll->condition('enr.program_id',$surObjectId,'=');
		$selectEnroll->condition('enr.user_id',$userId,'=');
		$selectEnroll->condition('enr.'.$field_name2,$status2,'IN');
		$resultEnroll = $selectEnroll->execute()->fetchAssoc();
		
		$user_enrollcount = $resultEnroll['enrollcount'];
		
		if($user_enrollcount > 0){
		
		// survey deeplink with class mode
		if($surObjectType=='cre_sys_obt_cls') {
			
			//Check the Class has Recurring Registration
			$select_recur = db_select('slt_business_rule_mapping','recur');
			$select_recur->addExpression('count(recur.id)','rcount');
			$select_recur->condition('recur.entity_id',$surObjectId,'=');
			$select_recur->condition('recur.entity_type',$surObjectType,'=');
			$select_recur->condition('recur.business_rule_code','cre_sys_brl_rra','=');
			$result_recur = $select_recur->execute()->fetchAssoc();
			$recurring_count = $result_recur['rcount'];	
		
			//Check the Class is associated with Multiple TP's
			$select_assoc = db_select('slt_course_class','clss');
			$select_assoc->addExpression('count(clss.id)','acount');
			$select_assoc->innerJoin('slt_module_crs_mapping','crsmap','crsmap.course_id = clss.course_id');
			$select_assoc->condition('clss.id',$surObjectId,'=');
			$select_assoc->condition('clss.course_id',$result_title['courseid'],'=');
			$select_assoc->condition('clss.status',array('lrn_cls_sts_atv','lrn_cls_sts_dld'),'IN');
			$result_assoc = $select_assoc->execute()->fetchAssoc();
			
			if($result_assoc['acount']>0) {
				//Check User Enrollment
				$select_user = db_select('slt_enrollment','e');
				$select_user->addExpression('count(e.id)','ucount');
				$select_user->condition('e.user_id',$userId,'=');
				$select_user->condition('e.course_id',$result_title['courseid'],'=');	
				$select_user->condition('e.comp_status',array('lrn_crs_cmp_cmp','lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
				$select_user->isNotNull('e.master_enrollment_id');
				$result_user = $select_user->execute()->fetchAssoc();
			}
		} else {
			// need to check
		}
		
		// confirm this survey still attached to a class or program
		if($surLinkType=="single") { 
			$selectStmt = db_select('slt_survey_mapping','map');
			$selectStmt->addField('map','survey_id','surveyid');
			$selectStmt->addField('map','module_id','moduleid');
			$selectStmt->condition('map.object_id',$surObjectId,'=');
			$selectStmt->condition('map.object_type',$surObjectType,'=');
			$selectStmt->condition('map.id',$surSurveyId,'=');
			$result = $selectStmt->execute()->fetchAssoc();
		}
		
		
		//Check the Re-certification
		if($surObjectType=='cre_sys_obt_crt') {
			$selectModule = db_select('slt_module','mo');
			$selectModule->addExpression('count(mo.id)','mcount');
			$selectModule->condition('mo.program_id',$surObjectId,'=');
			$selectModule->condition('mo.status','spp_tpm_mdl_atv','=');
			$resultModule = $selectModule->execute()->fetchAssoc();
			$mcount = $resultModule['mcount'];
		}
		
		if($surObjectType=='cre_sys_obt_cls') {
			if($result_assoc['acount'] > 1 && $result_user['ucount'] >= 1) {
				// user has both program and class
				$surmultipleTP = $result_assoc['acount']; 
				
					$select_assenroll = db_select('slt_enrollment','enr');
					$select_assenroll->addField('enr','id','enrollid');
					$select_assenroll->addField('enr','comp_status','enrollstatus');
					$select_assenroll->addField('enr','reg_date','enrolldate');
					$select_assenroll->addField('clss','title','objecttitle');
					$select_assenroll->addField('clss','entity_type','objecttype');
					$select_assenroll->addExpression('""','programid');
					$select_assenroll->addExpression('""','masterenrollid');
					$select_assenroll->addExpression('""','sequence');
					$select_assenroll->addExpression('""','enforcesequence');
					$select_assenroll->innerJoin('slt_course_class','clss','clss.id = enr.class_id and clss.course_id = enr.course_id');
					$select_assenroll->isNull('enr.master_enrollment_id');
					$select_assenroll->condition('enr.class_id',$surObjectId,'=');
					$select_assenroll->condition('enr.user_id',$userId,'=');
					$select_assenroll->condition('enr.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
										
					$select_assenroll2 = db_select('slt_enrollment','enr2');
					$select_assenroll2->addField('enr2','id','enrollid');
					$select_assenroll2->addField('enr2','comp_status','enrollstatus');
					$select_assenroll2->addField('enr2','reg_date','enrolldate');
					$select_assenroll2->addField('prg','title','objecttitle');
					$select_assenroll2->addField('prg','object_type','objecttype');
					$select_assenroll2->addField('prg','id','programid');
					$select_assenroll2->addField('enr2','master_enrollment_id','masterenrollid');
					$select_assenroll2->addField('mapp','sequence','sequence');
					$select_assenroll2->addField('prg','enforce_sequence','enforcesequence');
					$select_assenroll2->innerJoin('slt_course_class','clss2','clss2.id = enr2.class_id and clss2.course_id = enr2.course_id');
					$select_assenroll2->innerJoin('slt_master_enrollment','mass','mass.id = enr2.master_enrollment_id and mass.overall_status in ("lrn_tpm_ovr_cmp","lrn_tpm_ovr_enr")');
					$select_assenroll2->innerJoin('slt_program','prg','prg.id = mass.program_id');
					$select_assenroll2->innerJoin('slt_module_crs_mapping','mapp','mapp.program_id = prg.id and mapp.course_id = clss2.course_id');
					$select_assenroll2->isNotNull('enr2.master_enrollment_id');
					$select_assenroll2->condition('enr2.class_id',$surObjectId,'=');
					$select_assenroll2->condition('enr2.user_id',$userId,'=');
					$select_assenroll2->condition('enr2.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_enr','lrn_crs_cmp_inp'),'IN');
					$select_assenroll2->orderBy('enrollid','desc');
				$select_assenroll->union($select_assenroll2);
				$enroll_res = $select_assenroll->execute();
			} else {
				//Get the Class Enrollemnts only
				$select_enroll = db_select('slt_enrollment','enr');
				$select_enroll->addField('enr','id','enrollid');
				$select_enroll->addField('enr','reg_date','enrolldate');
				$select_enroll->addField('enr','reg_status','enrollregstatus');
				$select_enroll->addField('enr','comp_status','enrollstatus');
				$select_enroll->condition('enr.class_id',$surObjectId,'=');
				$select_enroll->condition('enr.course_id',$result_title['courseid'],'=');
				$select_enroll->condition('enr.user_id',$userId,'=');
				$select_enroll->isNull('enr.master_enrollment_id');
				$select_enroll->orderBy('enr.id','desc');
				$enroll_res = $select_enroll->execute();
			}
		} else if ($mcount > 1 && $surLinkType=="single") {
			$select_master = db_select('slt_master_enrollment','mas');
			$select_master->addField('mas','id','enrollid');
			$select_master->addField('mas','overall_status','enrollstatus');
			$select_master->leftJoin('slt_module','slm','slm.program_id = mas.program_id and slm.sequence = mas.recertify_path');
			$select_master->condition('mas.program_id',$surObjectId,'=');
			$select_master->condition('mas.user_id',$userId,'=');
			$select_master->condition('slm.id',$result['moduleid'],'=');
			$select_master->condition('slm.status','spp_tpm_mdl_atv','=');
			$select_master->orderBy('mas.id','desc');
			$enroll_res = $select_master->execute();
			$recertify = 1;
		} else if($mcount > 1 && $surLinkType=="multiple"){
			//get Re-certify Enrollment Details
			$select_master = db_select('slt_master_enrollment','mas');
			$select_master->addField('mas','id','enrollid');
			$select_master->addField('spp','survey_id','surveyid');
			$select_master->addField('mas','reg_date','enrolldate');
			$select_master->addField('mas','overall_status','enrollstatus');
			$select_master->leftJoin('slt_module','slm','slm.program_id = mas.program_id and slm.sequence = mas.recertify_path');
			$select_master->innerJoin('slt_survey_mapping','spp','spp.object_id = mas.program_id and spp.module_id = slm.id');
			$select_master->condition('mas.program_id',$surObjectId,'=');
			$select_master->condition('mas.user_id',$userId,'=');
			$select_master->condition('slm.status','spp_tpm_mdl_atv','=');
			$select_master->groupBy('mas.id');
			$select_master->orderBy('mas.id','desc');
			$select_master->orderBy('spp.survey_id');
			$enroll_res = $select_master->execute();
			$recertify = 1;
		} else {
			$select_mas = db_select('slt_master_enrollment','mas');
			$select_mas->addField('mas','id','enrollid');
			$select_mas->addField('mas','overall_status','enrollstatus');
			$select_mas->condition('mas.program_id',$surObjectId,'=');
			$select_mas->condition('mas.user_id',$userId,'=');
			$select_mas->condition('mas.overall_status','lrn_tpm_ovr_cln','!=');
			$select_mas->groupBy('mas.id');
			$select_mas->orderBy('mas.id','desc');
			$enroll_res = $select_mas->execute();
		}
		if(($result_recur['rcount'] > 0) || ($result_assoc['acount'] > 1 && $result_user['ucount'] >= 1) || ($mcount > 1 && $surLinkType=="multiple")) {
			$enroll_count = $enroll_res->rowCount();
			$result_enroll = $enroll_res->fetchAll();
		} else {
			$result_enroll = $enroll_res->fetchAssoc();
		}

		$enrollid = ($surEnrollId !='null') ? $surEnrollId : $result_enroll['enrollid'];
		$enrollstatus = ($result_enroll['enrollregstatus']=='lrn_crs_reg_can') ? $result_enroll['enrollregstatus'] : $result_enroll['enrollstatus'];
		
		if($result_recur['rcount'] > 0) {
			foreach($result_enroll as $enr_val) {
				if ($enroll_count > 1) {
					if($enr_val->enrollstatus != '' && ($enr_val->enrollstatus == 'lrn_crs_cmp_cmp' || $enr_val->enrollstatus == 'lrn_crs_cmp_enr' || $enr_val->enrollstatus == 'lrn_crs_cmp_inp')) {
						$surstatus = getAssignedSurveys($userId, $surObjectId, $surObjectType, 'survey', '0', $enr_val->enrollid, 'yes');
					} else {
						$survey_id = 0;
						$enrollid = $result_enroll[0]->enrollid;
						$enrollstatus = ($result_enroll[0]->enrollregstatus=='lrn_crs_reg_can') ? $result_enroll[0]->enrollregstatus : $result_enroll[0]->enrollstatus;
					}
				} else {
					$surstatus = getAssignedSurveys($userId, $surObjectId, $surObjectType, 'survey', '0', $enr_val->enrollid, 'yes');
				}
				
				if($enr_val->enrollstatus != '' && ($enr_val->enrollstatus == 'lrn_crs_cmp_cmp' || $enr_val->enrollstatus == 'lrn_crs_cmp_enr' || $enr_val->enrollstatus == 'lrn_crs_cmp_inp')) {
					$i=0;
					foreach($surstatus as $key=>$value) {
						if($surLinkType=="single") {
							if($result['surveyid']==$surstatus[$i]->surveyid) {
								$survey_details = array('object_id'=>$surObjectId, 'object_title'=>rawurlencode($result_title['objecttitle']), 'object_type'=>$surObjectType, 'course_id'=>$result_title['courseid'],
													'survey_id'=>$surstatus[$i]->surveyid, 'survey_title'=>rawurlencode($surstatus[$i]->surveytitle), 'survey_count'=>$surstatus[$i]->surveycount,
													'enroll_id'=>$enr_val->enrollid, 'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 'enroll_status'=>$enr_val->enrollstatus);
								$survey_data[] = array($survey_details);
							}
						} else if($surLinkType=="multiple"){
							$survey_details = array('object_id'=>$surObjectId, 'object_title'=>rawurlencode($result_title['objecttitle']), 'object_type'=>$surObjectType, 'course_id'=>$result_title['courseid'],
												'survey_id'=>$surstatus[$i]->surveyid, 'survey_title'=>rawurlencode($surstatus[$i]->surveytitle), 'survey_count'=>$surstatus[$i]->surveycount,
												'enroll_id'=>$enr_val->enrollid, 'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 'enroll_status'=>$enr_val->enrollstatus);
							$survey_data[] = array($survey_details);
						}
						$i++;
					}
			  	} else {
			  		if ($enroll_count == 1) {
			  			$survey_id = $result['surveyid'];
			  			$enrollid = $result_enroll['enrollid'];
			  			$enrollstatus = ($enr_val->enrollregstatus=='lrn_crs_reg_can') ? $enr_val->enrollregstatus : $enr_val->enrollstatus;
			  		} 
			  	}
			  	if($enr_val->enrollstatus=='lrn_crs_cmp_cmp')
			  		$myenrollment_tab = 'Yes';
			}
			$_SESSION['survey_data'] = str_replace('"','\'',drupal_json_encode($survey_data));
		} else if($result_assoc['acount'] > 1 && $result_user['ucount'] >= 1) {
			foreach($result_enroll as $enr_val) {
				if ($enroll_count >= 1) {
					if($enr_val->enrollstatus != '' && ($enr_val->enrollstatus == 'lrn_crs_cmp_cmp' || $enr_val->enrollstatus == 'lrn_crs_cmp_enr' || $enr_val->enrollstatus == 'lrn_crs_cmp_inp')) {
						$surstatus = getAssignedSurveys($userId, $surObjectId, $surObjectType, 'survey', '0', $enr_val->enrollid, 'yes');
					} else {
						$survey_id = 0;
						$enrollid = $result_enroll[0]->enrollid;
						$enrollstatus = ($result_enroll[0]->enrollregstatus=='lrn_crs_reg_can') ? $result_enroll[0]->enrollregstatus : $result_enroll[0]->enrollstatus;
					}
				} 
				if($enr_val->enrollstatus != '' && ($enr_val->enrollstatus == 'lrn_crs_cmp_cmp' || $enr_val->enrollstatus == 'lrn_crs_cmp_enr' || $enr_val->enrollstatus == 'lrn_crs_cmp_inp')) {
					$i=0;
					foreach($surstatus as $key=>$value) {
						if($surLinkType=="single") {
							if($result['surveyid']==$surstatus[$i]->surveyid) {
								if($enr_val->masterenrollid != '' && $enr_val->programid != '' && $enr_val->enforcesequence == 'Y') {
									$sequence = ($enr_val->sequence)-1;
								$select_enforce = db_select('slt_enrollment','ros');
								$select_enforce->addExpression('count(ros.id)','ecount');
								$select_enforce->addField('temp','title','prevcoursetitle');
								$select_enforce->innerJoin('slt_course_template','temp','temp.id = ros.course_id');
								$subquery = db_select('slt_module_crs_mapping','crs');
								$subquery->addField('crs','course_id','crsid');
								$subquery->condition('crs.program_id',$enr_val->programid,'=');
								$subquery->condition('crs.sequence',$sequence,'=');
								$subquery->condition('crs.is_required','Y','=');
								$select_enforce->condition('ros.master_enrollment_id',$enr_val->masterenrollid,'=');
								$select_enforce->condition('ros.user_id',$userId,'=');
								$select_enforce->condition('ros.course_id',$result_title['courseid'],'!=');
								$select_enforce->condition('ros.comp_status',array('lrn_crs_cmp_enr' , 'lrn_crs_cmp_inp'),'IN');
								$select_enforce->condition('ros.course_id',$subquery,'IN');
									$result_enforce = $select_enforce->execute()->fetchAssoc();
									$enforce = $result_enforce['ecount'];
									$prev_coursetitle = rawurlencode($result_enforce['prevcoursetitle']);
								} else {
									$enforce = '0';
									$prev_coursetitle = '';
								}
								if ($enroll_count == 1) {
									if($surstatus[$i]->surveycount == 0) {
										$surObjectType = $enr_val->objecttype;
										$enrollid = ($enr_val->masterenrollid !='') ? $enr_val->masterenrollid : $enr_val->enrollid;
										$enrollstatus = $enr_val->enrollstatus;
										$survey_id = $value->surveyid;
										$survey_status = $surstatus[$i]->surveycount;
										break;
									}
								} else{
									$survey_details = array('object_id'=>$surObjectId, 'program_id'=>$enr_val->programid, 'object_title'=>rawurlencode($enr_val->objecttitle), 'object_type'=>$enr_val->objecttype,
											'survey_id'=>$surstatus[$i]->surveyid, 'survey_title'=>rawurlencode($surstatus[$i]->surveytitle), 'survey_count'=>$surstatus[$i]->surveycount, 'enroll_id'=>$enr_val->enrollid, 'master_enrollid'=>$enr_val->masterenrollid, 'prev_coursetitle'=>$prev_coursetitle, 
											'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 'enroll_status'=>$enr_val->enrollstatus, 'launch_elligible'=>$enforce);
									$survey_data[] = array($survey_details);
								}
							}
						} else if($surLinkType=="multiple"){
							if($enr_val->masterenrollid != '' && $enr_val->programid != '' && $enr_val->enforcesequence == 'Y') {
								$sequence = ($enr_val->sequence)-1;
								$select_enforce = db_select('slt_enrollment','ros');
								$select_enforce->addExpression('count(ros.id)','ecount');
								$select_enforce->addField('temp','title','prevcoursetitle');
								$select_enforce->innerJoin('slt_course_template','temp','temp.id = ros.course_id');
								$subquery = db_select('slt_module_crs_mapping','crs');
								$subquery->addField('crs','course_id','crsid');
								$subquery->condition('crs.program_id',$enr_val->programid,'=');
								$subquery->condition('crs.sequence',$sequence,'=');
								$subquery->condition('crs.is_required','Y','=');
								$select_enforce->condition('ros.master_enrollment_id',$enr_val->masterenrollid,'=');
								$select_enforce->condition('ros.user_id',$userId,'=');
								$select_enforce->condition('ros.course_id',$result_title['courseid'],'!=');
								$select_enforce->condition('ros.comp_status',array('lrn_crs_cmp_enr' , 'lrn_crs_cmp_inp'),'IN');
								$select_enforce->condition('ros.course_id',$subquery,'IN');
								$result_enforce = $select_enforce->execute()->fetchAssoc();
								$enforce = $result_enforce['ecount'];
								$prev_coursetitle = rawurlencode($result_enforce['prevcoursetitle']);
							} else {
								$enforce = '0';
								$prev_coursetitle = '';
							}
							if ($enroll_count == 1) {
								if($surstatus[$i]->surveycount == 0) {
									$surObjectType = $enr_val->objecttype;
									$enrollid = ($enr_val->masterenrollid !='') ? $enr_val->masterenrollid : $enr_val->enrollid;
									$enrollstatus = $enr_val->enrollstatus;
									$survey_id = $value->surveyid;
									$survey_status = $surstatus[$i]->surveycount;
									break;
								}
							} else{
								$survey_details = array('object_id'=>$surObjectId, 'program_id'=>$enr_val->programid, 'object_title'=>rawurlencode($enr_val->objecttitle), 'object_type'=>$enr_val->objecttype, 'survey_id'=>$value->surveyid, 'survey_title'=>rawurlencode($value->surveytitle), 
										'survey_count'=>$surstatus[$i]->surveycount, 'enroll_id'=>$enr_val->enrollid, 'master_enrollid'=>$enr_val->masterenrollid, 'prev_coursetitle'=>$prev_coursetitle, 
										'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 'enroll_status'=>$enr_val->enrollstatus, 'launch_elligible'=>$enforce);
								$survey_data[] = array($survey_details);
							}
						}
						$i++;
					}
				} 
				}			
				$_SESSION['survey_data'] = str_replace('"','\'',drupal_json_encode($survey_data));
		} else if ($mcount > 1 && $surLinkType=="multiple") {
			foreach($result_enroll as $enr_val) {
				if($enr_val->enrollstatus != '' && ($enr_val->enrollstatus == 'lrn_tpm_ovr_cmp' || $enr_val->enrollstatus == 'lrn_tpm_ovr_enr' || $enr_val->enrollstatus == 'lrn_tpm_ovr_inp')) {
					$surstatus = getAssignedSurveys($userId, $surObjectId, $surObjectType, 'survey', '0', $enr_val->enrollid, 'yes');							
					$i=0;
					foreach($surstatus as $key=>$value) {
						$survey_details = array('object_id'=>$surObjectId, 'object_title'=>rawurlencode($result_title['objecttitle']), 'object_type'=>$surObjectType,
							'course_id'=>$result_title['courseid'], 'survey_id'=>$value->surveyid, 'survey_title'=>rawurlencode($value->surveytitle), 
							'survey_count'=>$value->surveycount, 'enroll_id'=>$enr_val->enrollid, 'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 
							'enroll_status'=>$enr_val->enrollstatus);
						$survey_data[] = array($survey_details);
						$i++;
					}
				}
				if($enr_val->enrollstatus=='lrn_tpm_ovr_cmp')
					$myprogramTab = 'Yes';
			}
			$_SESSION['survey_data'] = str_replace('"','\'',drupal_json_encode($survey_data));
			
	 } else {
		$surstatus2 = getAssignedSurveys($userId, $surObjectId, $surObjectType, 'survey', '0', $enrollid, 'yes');

		$j=0;
		foreach($surstatus2 as $key=>$value) {
			expDebug::dPrint('survey completion status key ==> '.print_r($key,true),5);
			expDebug::dPrint('survey completion status value ==> '.print_r($value,true),5);
			
			$survey_details = array('object_id'=>$surObjectId, 'object_title'=>rawurlencode($result_title['objecttitle']), 'object_type'=>$objecttype,
							'course_id'=>$result_title['courseid'], 'survey_id'=>$value->surveyid, 'survey_title'=>rawurlencode($value->surveytitle), 
							'survey_count'=>$value->surveycount, 'enroll_id'=>$enr_val->enrollid, 'enroll_date'=>rawurlencode(date('d M, Y',strtotime($enr_val->enrolldate))), 
							'enroll_status'=>$enr_val->enrollstatus, 'default_content'=>$j);
// 			
			expDebug::dPrint('ewwewqewqewqeqweqwewqeqweqwewqwqe'.$j.' ==> '.print_r($survey_details,true),5);
			$survey_data[] = array($survey_details);
			
			expDebug::dPrint('survey_data ==> '.print_r($survey_data,true),5);
			expDebug::dPrint('Get surveylink survey completion status $surstatus2222 ==> ');
			
			
			
			
			if($surSurveyId!='null' || $surSurveyId!='') {
				if($surLinkType=="single") {
					if($result['surveyid'] == $value->surveyid) {
						$survey_id = $value->surveyid;
						$survey_status = $value->surveycount;
						break;
					} else {
						$survey_id = '';
					}
				} else {
				if($value->surveyid !='' && $value->surveycount == 0) {
						$survey_id = $value->surveyid;
						$survey_status = $value->surveycount;
						break;
					} else {
						$survey_id = '';
					}
				}
			}
			$j++;
		}
		if($surstatus2 == null) $survey_id = 0; 
	 }
	}
	}
	$surveylink = array(
			'survey_urlname' => $survey_attr['SurUrlName'],
			'survey_sharetype' => $surLinkType,
			'survey_objectid' => $surObjectId,
			'survey_objecttype' => $surObjectType,
			'survey_userid' => $userId,
			'survey_count' => $surveymapcount,
			'survey_objectname' => $surveyObjectName,
			'survey_courseid' => $surveyCourseId,
			'survey_enrollcount' => $user_enrollcount,
			'survey_recurringcount' => $recurring_count,
			'survey_multipleTP' => $surmultipleTP,
			'survey_recertify' => $recertify,
			'survey_myenrolltab' => $myenrollment_tab,
			'survey_myprogramstab' => $myprogramTab,
			'survey_islaunchable' => $enforce,
			'survey_enrollid' => $enrollid,
			'survey_enrollstatus' => $enrollstatus,
			'survey_id' => $survey_id,
			'survey_status' => $survey_status,
	);
	
	$output = '';
	$output->enrollment = $result_enroll;
	$output->survey_data = $survey_data;
	$output->surveylink = $surveylink;	
	return $output;	
}
//Added for #0064601
function ClassLocationDetails($classId,$sessionLocation){
	
	$select = db_select('slt_location','loc');
	$select->leftJoin('slt_course_class_session','clsses','clsses.location_id = loc.id');	
	$select->leftJoin('slt_country','cty','cty.country_code = loc.country');
	$select->leftJoin('slt_state','ste','ste.state_code = loc.state and ste.country_code =loc.country');
  	
  	$select->addExpression("concat_ws(', <br>', if(addr1 = '', null, addr1), if(addr2 = '', null, concat(addr2,',')))", "locaddr1");
	$select->addExpression("concat_ws(', ',city, ste.state_name, zipcode)", "locaddr2");
	$select->addExpression("concat('<br>',cty.country_name)", "locaddr3");		
 	$select->addField('loc','name','loc_name');
	 
    if(!empty($sessionLocation)) $select->condition('loc.name', $sessionLocation, '=');
	$select->condition('clsses.class_id', $classId, '=');  									
	//expDebug::dPrintDBAPI(" ClassLocationDetails select query ",$select,5);
	$select->distinct(); 	
	 
	$result = $select->execute()->fetchAll();
		
	if(count($result) > 0) {
		$loc_name = $result[0]->loc_name.',<br>';
		if(!empty($result[0]->locaddr1)) $locaddr1=$result[0]->locaddr1.'<br>';
		else $locaddr1 = ''; 
		$loc_address = $locaddr1.$result[0]->locaddr2.$result[0]->locaddr3; 
		
	$finalresult = '<table style="display: inline-block;vertical-align: top;margin-top: -2px;font-size: 10pt; font-family: arial,helvetica,sans-serif;"><tr><td>'.$loc_name.$loc_address.'</td></tr></table>';  
	} else  $finalresult = '';
	//expDebug::dPrint('Location Address Details = ' . print_r($result,1),5);
 	return $finalresult;
	
}

function getPushNotificationActionDetails($objectId1,$objectId2,$notificationInfo) {

	$action_detail = '';
	$notifyMobile = 0;
	$remark = '';
	$customvalue = NULL;
	$send_status ='N';

	$mobile_notify_message_array = array('register_by_admin','register_vc_by_admin','register_wbt_by_admin','class_cancel_wbt_vod','class_cancel','compliance_expiry_remainder','compliance_expired');
	if(in_array($notificationInfo['message_id'],$mobile_notify_message_array))
	{
		$classInfo = getClassInfoForNotifications($objectId2);
		//$objectId2 = classID,  objID3 = courseId , $objectId1 = enrolledId
		$action_detail =  'objID1>|'.$objectId1.'~|objID2>|'.$objectId2.'~|objID3>|'.$classInfo['course_id'].'~|objID4>|'.$classInfo['status'].'~|objID5>|'.$classInfo['delivery_type_name'];
		$notifyMobile = 1;
	}
	else if($notificationInfo['message_id'] == 'cert_curr_lp_expire_reminder')
	{
		$action_detail =  'objID1>|'.$objectId1.'~|objID2>|~|objID3>|~|objID4>|~|objID5>|';
		$notifyMobile = 1;
	}

	if($notification_message_id == 'open_survey_mobile'){
		$classArray = explode('~~',$objectId1);
		$customvalue = $classArray[0];
		$action_detail =  'objID1>|'.$classArray[0].'~|objID2>|'.$objectId2.'~|objID3>|opensurveymobile~|objID4>|opensurveymobile~|objID5>|'.$classArray[1];
		$notifyMobile = 1;
		$send_status = 'Y';
	}

	$mobile_action_details = '';
	$mobile_action_details['message_id'] = $notificationInfo['message_id'];
	$mobile_action_details['action_detail']  = $action_detail;
	$mobile_action_details['notifyMobile'] = $notifyMobile;
	$mobile_action_details['remark'] = $remark;
	$mobile_action_details['customvalue'] = $customvalue;
	$mobile_action_details['send_status'] = $send_status;
	return $mobile_action_details;

}


function fireMobilePushNotification($userId,$notificationInfo,$action_detail,$drupalUser,$userNotificationId) {

	$mobile_notify_message_array = array('register_by_admin','register_vc_by_admin','register_wbt_by_admin','class_cancel_wbt_vod','class_cancel','compliance_expiry_remainder','compliance_expired','cert_curr_lp_expire_reminder','open_survey_mobile');
	if(in_array($notificationInfo['message_id'],$mobile_notify_message_array))
	{
		$fireMobileNotification = checkForManageNotification($userId,$notificationInfo['message_id']);
		if($fireMobileNotification === true || $fireMobileNotification == true)
		{
			$list = explode('~|', $notificationInfo['tokens_string']);
			$result = array();
			$notif_type = $notificationInfo['message_id'];
			foreach ($list as $pair)
			{
				$arr = explode(">|", $pair);
				$arr[1] && ($result[$arr[0]] = $arr[1]);
			}
			$selectCrsId = db_select('slt_mobiledevice_tokens', 'gcm');
			$selectCrsId -> addField('gcm', 'gcm_regid');
			$selectCrsId ->condition('gcm.name', $drupalUser['full_name'] ,'=');
			$selectCrsId->groupBy('gcm.id');
			$registatoin_ids = $selectCrsId -> execute() -> fetchAll();
			expDebug::dPrint('Reggggg idd ='.print_r($registatoin_ids,true), 4);
			for($i=0;$i<count($registatoin_ids);$i++)
			{
				$message = "";
				$select = db_select('slt_notification_info','info');
				$select ->leftJoin('slt_notification_frame', 'frame', 'frame.notification_id = info.id');
				$select->addField('frame','notification_subject', 'notification_subject');
				$select->condition('info.notification_code',$notificationInfo["message_id"]);
				$select->condition('frame.lang_code','cre_sys_lng_eng');
				expDebug::dPrintDBAPI('narrow search sql query-->>',$select);
				$notification_subject = $select->execute()->fetchField();
				$message = getNotificationSubject($notificationInfo['tokens_string'],$notification_subject);
				expDebug::dPrint('Learning message ajay = '.$message);
				if(strpos($registatoin_ids[$i]->gcm_regid,'~~ios') !== false )
				{
					// Put your device token here (without spaces):
					$regIdArray = explode('~~', $registatoin_ids[$i]->gcm_regid);
					include_once($_SERVER["DOCUMENT_ROOT"]. '/mobile/GCM.php');
					$gcm = new GCM();
					$logged_in_user = "";
					$deviceToken = $regIdArray[0];

					// Code added for push notification
					$addId = 1;
					$new_notifyId = ($userNotificationId . $addId);
					$result = $gcm->send_notificationAPN($deviceToken,$message,$action_detail,$notif_type,$new_notifyId);
				}
				else
				{
					include_once($_SERVER["DOCUMENT_ROOT"]. '/mobile/GCM.php');
					$gcm = new GCM();
					expDebug::dPrint('Innnnnn ='.print_r($registatoin_ids[$i]->gcm_regid,true), 4);
					$message = array("message" => $message);
					$addId = 1;
					$new_notifyId = ($userNotificationId . $addId);
					$result = $gcm->send_notification($registatoin_ids[$i]->gcm_regid, $message,$action_detail,$notif_type,$new_notifyId);
				}
			}
		}
	}

}
