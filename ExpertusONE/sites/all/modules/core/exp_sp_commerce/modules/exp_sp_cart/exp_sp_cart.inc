<?php
//have checked the already user checkout process in other browser for class. added by yogaraja
function checkAlreadyReserved($userId,$classId,$courseId)
{
	try {
		$browser_session_id = session_id();
		expDebug::dPrint('$userId: '.$userId.'-'.$classId.'-'.$courseId.'-'.$browser_session_id, 4);
		$select = db_select('slt_enrollment','enr');
		$select->leftJoin("slt_business_rule_mapping",'rm','rm.entity_id = enr.class_id');
		$select->addField('enr','id');
		$select->condition('enr.user_id', $userId);
		$select->condition('enr.class_id', $classId);
		$select->condition('enr.course_id', $courseId);
		$select->condition('enr.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsc'),'NOT IN');
	  $select->condition('enr.comp_status', array('lrn_crs_cmp_inc'),'NOT IN');
		$select->condition('enr.browser_session', $browser_session_id,'!=');
		$select->isNull("rm.id");
		expDebug::dPrintDBAPI('$select yoga: ',$select, 4);
		$result = $select->execute()->fetchField();
		expDebug::dPrint('$result: '.$result, 4);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('checkAlreadyReserved', $ex);
		expertusErrorThrow($ex);
	}
}
//get paymention already initiated with other browser status for tp. added by yogaraja
function checkAlreadyReservedForTp($userId,$programId)
{
	try {
		$browser_session_id = session_id();
		$select = db_select('slt_master_enrollment');
		$select->addField('slt_master_enrollment','id');
		$select->condition('user_id', $userId);
		$select->condition('program_id', $programId);
		$select->condition('overall_status', array('lrn_tpm_ovr_rsc','lrn_tpm_ovr_cln','lrn_tpm_ovr_inc'),'NOT IN');
		$select->condition('browser_session', $browser_session_id,'!=');
		expDebug::dPrintDBAPI('$select TP: ',$select, 4);
		$result = $select->execute()->fetchField();
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('checkAlreadyReservedForTp', $ex);
		expertusErrorThrow($ex);
	}
}

function checkOrderEntrrolmentStatus($Order_Id)
{
	try {
		$select = db_select('slt_enrollment');
		$select->addField('slt_enrollment','id');
		$select->condition('reg_status', 'lrn_crs_reg_rsv','=');
		$select->condition('order_id', $Order_Id);
		expDebug::dPrintDBAPI('$select yoga: ',$select, 1);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('$result: '.print_r($result,1), 1);
		return $result;
	} catch (Exception $ex) {
		watchdog_exception('checkAlreadyReservedForTp', $ex);
		expertusErrorThrow($ex);
	}
}
//Get Node Information
function getCatalogInfofromNodeId($vNodeId){
  try{
    $dao = new AbstractDAO();
    expDebug::dPrint('$vNodeId : '.$vNodeId, 4);
    $qry = "call slp_getcourse_class_fromnode_sel($vNodeId)";
    expDebug::dPrint("getCatalogInfofromNodeId get LMS Node Info". $qry,4);
    $dao->connect();
    $res = $dao->query($qry);
    $result=$dao->fetchResult();
    $dao->closeconnect();
      expDebug::dPrint("getCatalogInfofromNodeId : ".print_r($result, true));

    return $result;
  } catch (Exception $e){
  expDebug::dPrint("Error Occured...when get Catalog details");
  expDebug::dPrint($e);
  exit();
  }
}


function getListOfclassesinCartAPI($userID,$user_id,$action){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		//Added by Subin. Ticket #59698
		$updUserInfo = getPersonDetails($userID,array('preferred_currency'));
        $updUserCurrency = $updUserInfo['preferred_currency'];
        $isCommerceEnabled = module_exists('exp_sp_commerce');
        $currecyChangeRestrict = restrict_user_currency_select();
        
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		if($isCommerceEnabled && $currecyChangeRestrict == TRUE){
		
                 //Added by Subin. Ticket #59698
		if(!empty($updUserCurrency) && (!empty($_REQUEST['currency_type'])) && ($updUserCurrency != $_REQUEST['currency_type'])){
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "Currency code should be same as user's profile currency.";
		return $errobj;
		}
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
				
		if ($_REQUEST['currency_type'] && $_REQUEST['currency_sym']) {
			$_SESSION['shop_cart_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_sym'] = $_REQUEST['currency_sym'];
		} else if ($_REQUEST['currency_type']) {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		} else {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		}

		$uid = getDrupalIdOfUser($userID);
        $vOrderId=0;
        $lmsUserId = getSltpersonUserId();
        $form= array();
        $form_state = array();

        $selectStmt = db_select('uc_cart_products', 'ct');
        // Set conditions
        $selectStmt->condition('ct.cart_id',$uid);
        // Select state name field for returning
        $selectStmt->addField('ct', 'nid');
        // Execute query
        $totalRecords = $selectStmt->countQuery()->execute()->fetchField();
        expDebug::dPrintDBAPI('select cart', $selectStmt->countQuery());
        expDebug::dPrint('$totalRecords '.$totalRecords);
        $node_id = $selectStmt->execute()->fetchAll();
       	$result['totalrow'] = $totalRecords;

        if($action == 'get_count')
		{
            if(count($node_id) > 0)
            {
                 $i=0;
        	foreach($node_id as $key => $valuues){
			        $valuues->nid;
		            $cart = getCatalogInfoFromNodeIdToMobileApi($valuues->nid);
		            $result[$i] = $cart[0];
		            $result[$i]->order_id = $vOrderId;
		            $i++;
            }
            
            $selectStmt = db_select('variable', 'va');
            $selectStmt->condition('va.name','uc_payment_credit_gateway');
            $selectStmt->addField('va', 'value');
            expDebug::dPrintDBAPI('select cart', $selectStmt);
            $payment_gateway_result = $selectStmt->execute()->fetchAll();
            $payment_gateway = unserialize($payment_gateway_result[0]->value);
            expDebug::dPrint('$payment_gateway$payment_gateway$payment_gateway$payment_gateway' . $payment_gateway, 5);
            $result[0]->payment_gateway = $payment_gateway;
            
            
            return $result;
            }else
            {
                $result = array();
            }
            return $result;
        }
        if($action != 'delete')
        {
        //for creating new slt order id
            $vOrderId = uc_deleted_classes_cart_pane_output($form, $form_state,'',$uid,'api');
        }
        if(count($node_id) > 0)
        {
        $i=0;
        foreach($node_id as $key => $valuues){
	        $valuues->nid;
            $cart = getCatalogInfoFromNodeIdToMobileApi($valuues->nid);
            $result[$i] = $cart[0];
            $result[$i]->order_id = $vOrderId;
            $i++;
            }
            $selectStmt = db_select('variable', 'va');
            $selectStmt->condition('va.name','uc_payment_credit_gateway');
            $selectStmt->addField('va', 'value');
            expDebug::dPrintDBAPI('select cart', $selectStmt);
            $payment_gateway_result = $selectStmt->execute()->fetchAll();
            $payment_gateway = unserialize($payment_gateway_result[0]->value);
            expDebug::dPrint('$payment_gateway$payment_gateway$payment_gateway$payment_gateway' . $payment_gateway, 5);
            $result[0]->payment_gateway = $payment_gateway;
            $checkEnabled = variable_get('uc_payment_method_other_checkout');
            expDebug::dPrint('$checkEnabled$checkEnabled$checkEnabled$checkEnabled' . $checkEnabled, 5);
            
            $order = array('credit', 'check','po','other');// rule indicating new key order
            $paymetTypeList = array();
            foreach($order as $index) {
            	$paymetTypeList[$index] = variable_get('uc_payment_method_'.$index.'_checkout');
            	expDebug::dPrint('$checkEnabled$checkEnabled$checkEnabled$checkEnabled'.$index. $paymetTypeList[$index], 5);
            }
            $result[0]->enabledPaymentMethod = $paymetTypeList;
            
            return $result;
		}
		else
		{
            $result = array();
            return $result;
		}
	}catch (Exception $ex) {
		watchdog_exception('getListOfclassesinCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/*

function getCatalogInfoFromNodeIdToMobileApi($node_id){
	try{


foreach($node_id as $key => $valuues){
	$nodeId[$key] = $valuues->nid;
}

		$select = db_select('slt_course_class','cls');
		$select->addField('cls', 'id' , 'ClassId');
		$select->addField('cls', 'course_id', 'CourseId');
		$select->addField('cls', 'title', 'ClassTitle');
		$select->addField('cls', 'code', 'class_code');
		$select->addField('cls', 'short_description', 'description');
		$select->addField('cls', 'delivery_type', 'DeliveryTypeId');
		$select->addField('cls', 'price', 'Price');
		$select->addField('items', 'name', 'DeliveryTypeName');
		$select->addField('loc', 'name', 'locname');
		$select->addField('loc', 'city', 'loccity');
		$select->addField('loc', 'country', 'loccountry');
		$select->leftJoin('slt_course_class_session', 'ccls', 'ccls.class_id = cls.id');
		$select->leftJoin('slt_location', 'loc', 'loc.id = ccls.location_id');
		$select->leftJoin('slt_node_learning_activity', 'la', 'la.entity_id = cls.id');
		$select->leftJoin('slt_profile_list_items', 'items', 'items.code = cls.delivery_type');
		$select->condition('la.node_id',$nodeId,is_array($nodeId)?'IN': '=');
		$select->groupBy('cls.id');

		$sessionFlag = $select->execute()->fetchAll();

		return  $sessionFlag;

	}catch (Exception $ex) {
		watchdog_exception('addClassToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}


*/



function getCatalogInfoFromNodeIdToMobileApi($node_id){
	try{

	expDebug::dPrint('resultttttttttt lastttt entity_data ' . print_r($node_id, true), 5);
	$sessionFlag = '';
	//	$sessionFlag1 = array();
	//	$sessionFlag2 = array();


	     $select = db_select('slt_node_learning_activity','nl');
	     $select->addField('nl', 'entity_id' , 'entity_id');
	     $select->addField('nl', 'entity_type' , 'entity_type');
	     $select->condition('nl.node_id',$node_id);
	     expDebug::dPrintDBAPI(' selectselectselect = ' ,$select);

	     $entity_data = $select->execute()->fetchAll();
	     expDebug::dPrint('resultttttttttt lastttt entity_data ' . print_r($entity_data[0]->entity_type, true), 5);

	     	if($entity_data[0]->entity_type == 'cre_sys_obt_cls')
	     	{
	     	$select = db_select('slt_course_class','cls');
			$select->addField('cls', 'id' , 'ClassId');
			$select->addField('cls', 'course_id', 'CourseId');
			$select->addField('cls', 'title', 'ClassTitle');
			$select->addField('cls', 'code', 'class_code');
			$select->addField('cls', 'short_description', 'description');
			$select->addField('cls', 'delivery_type', 'DeliveryTypeId');
			$select->addExpression("slf_convert_price(cls.price,cls.currency_type,'".$_REQUEST['currency_type']."')", 'Price');
			$select->addField('items', 'name', 'DeliveryTypeName');
			$select->addField('loc', 'name', 'locname');
			$select->addField('loc', 'city', 'loccity');
			$select->addField('loc', 'country', 'loccountry');
			$select->leftJoin('slt_course_class_session', 'ccls', 'ccls.class_id = cls.id');
			$select->leftJoin('slt_location', 'loc', 'loc.id = ccls.location_id');
			$select->leftJoin('slt_node_learning_activity', 'la', 'la.entity_id = cls.id');
			$select->leftJoin('slt_profile_list_items', 'items', 'items.code = cls.delivery_type');
			$select->condition('cls.id',$entity_data[0]->entity_id);
			$select->groupBy('cls.id');
			$result = $select->execute()->fetchAll();
			//	array_push($sessionFlag1,$result1);
	     	}
	     	else
	     	{
	     	$select = db_select('slt_program','tp');
	     	$select->addField('tp', 'id', 'ClassId');
			$select->addField('tp', 'title', 'ClassTitle');
			$select->addField('tp', 'code', 'class_code');
			$select->addField('tp', 'short_desc', 'description');
			$select->addExpression("slf_convert_price(tp.price,tp.currency_type,'".$_REQUEST['currency_type']."')", 'Price');
			$select->addField('tp', 'object_type', 'DeliveryTypeName');
			$select->condition('tp.id',$entity_data[0]->entity_id);
			$result = $select->execute()->fetchAll();
			    // array_push($sessionFlag2,$result2);
	     	}
	     	//$sessionFlag = array_merge($sessionFlag1,$sessionFlag2);
	     	expDebug::dPrint('resultttttttttt ' . print_r($result, true), 5);

	expDebug::dPrintDBAPI(' selectselectselect = ' ,$select);
	expDebug::dPrint('resultttttttttt lastttt ' . print_r($result, true), 5);

	return  $result;

	}catch (Exception $ex) {

		watchdog_exception('addClassToCartAPI', $ex);

		expertusErrorThrow($ex);

	}

}


//Cart Catalog Registration Status
function getCartCatalogRegistrationStatus($pmReq){
  try{
    $dao = new AbstractDAO();
    $vDrupalUserId  = $pmReq['DrupalUserId'];
    $vLMSUserId     = $pmReq['LMSUserId'];
    $updateUserId   = $pmReq['updateUserId'];

    $vCourseId  = $pmReq['CourseId'];
    $vClassId   = $pmReq['ClassId'];
    $vUserName  = addslashes($pmReq['UserName']);
    $vOrderId   = $pmReq['OrderId'];

    $cartFrom   = $pmReq['CartFrom'];

    $vShoppingCartFlag='Y';
    $vForceWaitingReg=$pmReq['ForceReg'];

    $qryUpd = "CALL slp_enrollstatuschange_upd('$vLMSUserId',$vOrderId,'$vUserName','$vCourseId','$vClassId',$updateUserId) ";
    $dao->connect();
   //$dao->beginTrans();
    expDebug::dPrint("Cart Catalog Enrollment Reserved Status Change DAO  ==>".$qryUpd,1);
    $dao->execute($qryUpd);
   //$dao->commitTrans();
   $dao->closeconnect();
   $result = enrLnrAssignmentIns($vLMSUserId,$vOrderId,$vCourseId,$vClassId,$vForceWaitingReg,$updateUserId,$vShoppingCartFlag);
    $vRegStatus='';
    expDebug::dPrint("status param DAO  ". $result['enrollmentstatus'],4);
    if($result['enrollmentstatus'] =='0'  ){
      $vRegStatus='Reserved';
    }else if($result['enrollmentstatus'] =='-1'  ){
      $vRegStatus='AlreadyEnrolled';
    }else if($result['enrollmentstatus'] =='-2'  ){
      $vRegStatus='Conflict';
    }else if($result['enrollmentstatus'] =='-3'  ){
      $vRegStatus ='Waitlist';
    }else if($result['enrollmentstatus'] =='-4'  ){
      $vRegStatus ='InvalidUser';
    }else if($result['enrollmentstatus'] =='-5'  ){
      $vRegStatus ='Full';
    }else if($result['enrollmentstatus'] =='-6'  ){
      $vRegStatus ='Invalid';
    }else if($result['enrollmentstatus'] =='-7'  ){
      $vRegStatus ='AlreadyReserved';
    }else if($result['enrollmentstatus'] =='-8'  ){
      $vRegStatus ='CourseConflict';
    }else if($result['enrollmentstatus'] =='-10' ){
      $vRegStatus = 'InstructorConflict';
    }
    $isOrderItemIns = 0;
    if($cartFrom == 'adminorder' && ($result['enrollmentstatus'] == '0' || $result['enrollmentstatus'] == '-3' || $result['enrollmentstatus'] == '1')){
    	$isOrderItemIns = 1;
    }
    else if($cartFrom != 'adminorder'){
    	$isOrderItemIns = 1;
    }
    if($isOrderItemIns){

    	//$user_pre_currency = empty($_SESSION['user_preferred_currency_code']) ? NULL : $_SESSION['user_preferred_currency_code'];
    	$user_pre_currency = (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))?$_SESSION['admin_shop_cart_currency_code']:$_SESSION['user_preferred_currency_code'];

			/*
    	$vInsertOrderItem ="call slp_order_items_ins('$vOrderId','$vCourseId','$vClassId','$vLMSUserId',null,'','$updateUserId','','','','','','$user_pre_currency')";
    	expDebug::dPrint("Insert order Item Dao ==>".$vInsertOrderItem,4);
	    $dao->connect();
	   // $dao->beginTrans();
	    $dao->execute($vInsertOrderItem);
	   // $dao->commitTrans();
	    $dao->closeconnect();
			*/
			 
			insertOrderItem($vOrderId, $vCourseId, $vClassId, $vLMSUserId, 'null','', $updateUserId, '', '', '', '', '',$user_pre_currency);
			 
    }
    if($cartFrom == 'adminorder'){
    	foreach ($_SESSION['cartform']['clone_uc_cart_review_table']['#items'] as $item) {
	    	$cClassId 	= $item->classId;
	  		$cCourseId 	= $item->courseId;
	  		$ucOrderId 	= $item->order_id;
	  		$nodeid  		= $item->nid;
	  		$Object_Type = $item->delivery_type_code;
	  		$order_product_id = $item->order_product_id;
	    	updateSltOrederItems($cCourseId,$cClassId,$Object_Type,$vOrderId,$vLMSUserId,$order_product_id,$nodeid,$ucOrderId,$cartFrom);
    	}
    }
    expDebug::dPrint('Already Added cart details---->'.print_r($_SESSION['cartform']['clone_uc_cart_review_table']['#items'],1),4);
    //return array('RegStatus' => $vRegStatus);
    return $vRegStatus;
  } catch (Exception $e){
	expDebug::dPrint("Error Occured...when get enroll status", 1);
	expDebug::dPrint($e, 1);
	exit();
  }
}

//Cart TP Registration Status
function getCartTPRegistrationStatus($pmReq){
  try{
    $dao = new AbstractDAO();
    $vDrupalUserId  = $pmReq['DrupalUserId'];
    $vLMSUserId     = $pmReq['LMSUserId'];
    $updateUserId   = $pmReq['updateUserId'];

    $vCourseId   = $pmReq['CourseId'];
    $vProgramId  = $pmReq['ClassId'];
    $vUserName   = $pmReq['UserName'];
    $vOrderId    = $pmReq['OrderId'];
    $Object_Type = $pmReq['Object_Type'];
    $NodeId      = $pmReq['NodeId'];

    $cartFrom   = $pmReq['CartFrom'];

    $vShoppingCartFlag='Y';
    $vForceWaitingReg=$pmReq['ForceReg'];

    //ORM code for stored procedure slp_program_enrollment_ins
    //Check for Training Program already enrolled
//ADDED INCOMP STATUS FOR #0080950 After checked in slf_check_program_user_enrolled()
    $selectEnrollCnt = db_select('slt_master_enrollment','mas');
    $selectEnrollCnt->addExpression('COUNT(1)', 'count');
    $selectEnrollCnt->condition('mas.program_id', $vProgramId,'=');
    $selectEnrollCnt->condition('mas.user_id', $vLMSUserId,'=');
    $selectEnrollCnt->condition('mas.overall_status', array('lrn_tpm_ovr_cln','lrn_tpm_ovr_rsc','lrn_tpm_ovr_inc','lrn_tpm_ovr_rsv'),'NOT IN');
    $isEnrolled  = $selectEnrollCnt->execute()->fetchField();

    $overall_status = 'lrn_tpm_ovr_rsv';
    $regStatus = 'lrn_crs_reg_rsv';
    if($vShoppingCartFlag == 'Y'){
      $regStatus  = 'lrn_crs_reg_rsv';
      $compStatus = null;
    }
    else{
	  $regStatus  = 'lrn_crs_reg_cnf';
	  $compStatus = 'lrn_crs_cmp_enr';
    }
    $vRegStatus='';
    expDebug::dPrint(' getCartTPRegistrationStatus() enrollcount -->'.$isEnrolled,3);
    if(!$isEnrolled){
      $transaction = db_transaction();
      try{
      	$manCnt = getMandatoryOrNot($vProgramId,$vLMSUserId,1);
      	$browser_session_id = session_id();
        $insertStmt = db_insert('slt_master_enrollment');
        $insertStmt->fields(array(
        'program_id' => $vProgramId,
        'order_id' 	 => $vOrderId,
        'user_id' => $vLMSUserId,
        'overall_status' => $overall_status,
        'recertify_path' => 1,
        'mandatory' => ($manCnt == 'cre_sys_inv_man') ? '1' : NULL,
        'browser_session' => $browser_session_id,
        'reg_date' => now(),
        'percentage_complete' => 0,
        'created_by' => $updateUserId,
        'created_on' => now(),
        'updated_by' => $updateUserId,
        'updated_on' => now(),
        'launchable'=> 0
        ));

        $masterEnrollmentId = $insertStmt->execute();
        expDebug::dPrint(' master_enrollment_id = ' . print_r($masterEnrollmentId, true),3);

        //Select classlist from node
        if($cartFrom != 'adminorder'){
	        $selectCart = db_select('uc_cart_products','ucp');
	        $selectCart->addField('ucp','class_ids');
	        $selectCart->condition('ucp.nid', $NodeId, '=');
	        $selectCart->condition('ucp.cart_id', $vDrupalUserId, '=');
	        $selectCart->range(0,1);
	        $class_ids  = $selectCart->execute()->fetchField();
        }
        else{
        	$class_ids  = $pmReq['ListedClassIds'];
        }
        expDebug::dPrint(' Class Ids = ' .$class_ids,3);
        
        //Get course mapped for the certain training program from slt_module_crs_mapping table
        
        $module_id = getModuleIdFromObjectId($vProgramId,1);
        $course_list = moduleCourseMapping($vProgramId,$module_id[0]->module_id);

        $masterEnrWaitlist = 0;
        $launchable  = 1;
        foreach ($course_list as $record) {

          $course_id = $record->course_id;
          $selectClass = db_select('slt_course_class','cls');
          $selectClass->addField('cls','id');
          $selectClass->addField('cls','delivery_type');
          $selectClass->addField('cls','is_compliance');
          $selectClass->condition('cls.course_id', $course_id, '=');
          $selectClass->condition('cls.status', 'lrn_cls_sts_atv', '=');
          if($class_ids){
            $selectClass->where('cls.id IN('.$class_ids.')');
          }
          $selectClass->orderBy('cls.created_on', 'ASC');
          $selectClass->range(0,1);
          $result_set  = $selectClass->execute()->fetchAll();

          $class_id = $result_set[0]->id;
          $delivery_type = $result_set[0]->delivery_type;

          expDebug::dPrint('$$delivery_type ------------- : '.$delivery_type,3);

          if($delivery_type == 'lrn_cls_dty_ilt' || $delivery_type == 'lrn_cls_dty_vcl'){

          	//Added by Vetrivel.P for #0072173
          	$select = db_select('slt_course_class','cls');
          	$select->join('slt_course_class_session','sess','sess.class_id = cls.id');
          	$select->join('slt_profile_list_items','spf','sess.timezone = spf.code');
          	$select->addField('sess','start_date');
          	$select->addField('sess','start_time');
          	$select->addExpression('spf.attr2','course_timezone');
          	$select->condition('cls.course_id', $course_id, '=');
          	$select->condition('cls.status', 'lrn_cls_sts_atv', '=');
          	$select->condition('cls.id', $class_id ,'=');
          	$result = $select->execute()->fetchAll();
          	
          	expDebug::dPrint("result->course_timezone  => ".print_r($result,1),4);
          	
          	$start_date = $result[0]->start_date;
          	$course_timezone = $result[0]->course_timezone;
          	$start_time = $result[0]->start_time;
          	
          	$reson = toGetSessionExpired($course_timezone,$start_date,$start_time);
          	
          	if($reson == 'classSessionExpired'){
          		return $reson;
          	}
          	
          	expDebug::dPrint('The value of $resonis here vetrivel '.print_r($reson,1),4);
          	
          	//Ended by Vetrivel.P for #0072173
          	
          	
          	$launchable  = 0;
          	$isPostAssessmentAttached = isPostAssessmentAttached($class_id,'class');
          	if($isPostAssessmentAttached == 1){
          		$launchable  = 1;
          	}
          	
          	 $waitlistSeat  = getWaitlistCatalogInfo($class_id);
	         expDebug::dPrint('$getWaitlistCatalogInfo111---------------- : '.$waitlistSeat,4);
	         $availableSeatCnt  = getWaitlistCatalogInfo($class_id, '', 'chkSeatCnt');
	         expDebug::dPrint('$getWaitlistCatalogInfo222---------------- : '.$availableSeatCnt,4);
	         if($availableSeatCnt <= 0){
	         	 return 'Full';
	         }
	         else{
	         	 $getWaitlistPosition = waitListCheckProgram($course_id,$class_id);
	             expDebug::dPrint('$getWaitlistPosition------------------- : '.$getWaitlistPosition,4);
		         $waitlist_priority = ($waitlistSeat > 0) ? $getWaitlistPosition : NULL;
		         $waitlist_flag = ($waitlistSeat > 0) ? 'lrn_crs_reg_wtl' : NULL;
	         }

          } else {
          	$waitlist_priority = NULL;
          	$waitlist_flag = NULL;
          }
/*         if($getWaitlistPosition > 0) {
         	$masterEnrWaitlist = 1;
         } */

          if($class_id){
           /* //Get total enrollment in same class
            $selectEnroll = db_select('slt_enrollment','enr');
            $selectEnroll->addField('enr','id');
            $selectEnroll->condition('enr.course_id', $course_id, '=');
            $selectEnroll->condition('enr.class_id', $class_id, '=');
            $selectEnroll->condition('enr.user_id', $vLMSUserId, '=');
            $selectEnroll->condition('enr.reg_status', array('lrn_crs_reg_can','lrn_crs_reg_rsv','lrn_crs_reg_rsc'),'NOT IN');
        //May need to check master_enrollment_id for get more tp register
            $selectEnroll->range(0,1);
            $enrollId  = $selectEnroll->execute()->fetchField();

            if(!$enrollId){*/
              //Insert into slt_enrollment table
          	$browser_session_id = session_id();
          	$manCnt = getMandatoryOrNot($class_id,$vLMSUserId);
              $insertStmt = db_insert('slt_enrollment');
              $insertStmt->fields(array(
              	'master_enrollment_id' => $masterEnrollmentId,
                'user_id' => $vLMSUserId,
                'order_id' => $vOrderId,
                'class_id' => $class_id,
                'course_id' => $course_id,
                'reg_status' => $regStatus,
              	'browser_session' => $browser_session_id,
                'waitlist_flag' => $waitlist_flag,
              	'waitlist_priority' => $waitlist_priority,
                'recertify_path' => 1,
                'is_compliance' => $result_set[0]->is_compliance,
              	'mandatory' => ($manCnt == 'cre_sys_inv_man') ? 'Y' : NULL,
                'reg_date' => now(),
                'comp_status' => $compStatus,
                'created_by' => $updateUserId,
                'created_on' => now(),
              	'updated_by' => $updateUserId,
                'updated_on' => now(),
              	'launchable'=>$launchable
              ));
              $enrollmentId = $insertStmt->execute();
            /*}
            else{
              //update the enrollment if the recertify course is already enrolled
              $updateEnr = db_update('slt_enrollment');
              $updateEnr->fields(array(
                                  'master_enrollment_id' => $masterEnrollmentId,
                                  'reg_from_prg'    => 'N',
              					  'updated_by'    => $vLMSUserId,
              					  'updated_on' => now()
                                  ));
              $updateEnr->condition('id', $enrollId);
              $updateEnr->where('reg_from_prg IS NULL');
              $updateEnr->execute();
            }*/

            // If ilt or vc class, send notification if max capacity is reached
            $deliveryType = getClassDeliveryType($class_id, 'Short');
            if ($deliveryType == 'ilt' || $deliveryType == 'vc') {
              $regStatusesList = array('lrn_crs_reg_cnf', 'lrn_crs_reg_ppm', 'lrn_crs_reg_rsv');
              $classEnrolledCount = getClassEnrollmentCountForRegistrationStatuses($course_id, $class_id, $regStatusesList);
              expDebug::dPrint(' $classEnrolledCount = ' . print_r($classEnrolledCount, true),4);

              $classMaxCapacity = getClassMaxCapacity($course_id, $class_id);
              expDebug::dPrint(' $classMaxCapacity = ' . print_r($classMaxCapacity, true),4);

              if (!empty($classMaxCapacity) && $classEnrolledCount == $classMaxCapacity) {
              	insertNotification('ClassMaxCapacityReached', $course_id, $class_id, $vLMSUserId, $updateUserId);
              }
            }

            //Insert order_items
            insertOrderItem($vOrderId, $course_id, $class_id, $vLMSUserId, $vProgramId, $Object_Type, $updateUserId, '', '', '', '', '',$cartFrom);
          }

        }

        
        expDebug::dPrint(' master_enrollment_id111 = ' . print_r($masterEnrollmentId, true),3);
        
        $isProgramLaunchable = isProgramLaunchable($masterEnrollmentId);
        if($isProgramLaunchable){
        	$updateStmt = db_update('slt_master_enrollment');
        	$updateStmt->condition('id', $masterEnrollmentId,'=');
        	$updateStmt->fields(array('launchable' => 1,'updated_on'=>now()));
        	expDebug::dPrintDBAPI(' update isProgramLaunchable master enrolment ' , $updateStmt);
        	$master_enroll_upd = $updateStmt->execute();
        }
        
        
        //lrn_tpm_ovr_rsv
      /* if($masterEnrWaitlist == 1) {
        	$updateEnr = db_update('slt_master_enrollment');
              $updateEnr->fields(array(
                                  'overall_status' => 'lrn_tpm_ovr_wtl'
                                  ));
              $updateEnr->condition('id', $masterEnrollmentId);
              $updateEnr->execute();
        }*/
        $vRegStatus='Reserved';
      }catch (Exception $e) {
         $transaction->rollback();
         throw $e;
      }

    }
    else{
      $vRegStatus='AlreadyEnrolled';
    }
    
    // To sync solr data
    syncSolrData('Enrollment');
    syncSolrData('MasterEnrollment');
    syncSolrData('User');
    return $vRegStatus;
  } catch (Exception $e){
	expDebug::dPrint("Error Occured...when get enroll status", 1);
	expDebug::dPrint($e, 1);
	exit();
  }
}

//Get Cart order Total
function getOrderTotal(){
	try{
  $order_subtotal = 0;
  $order          = new stdClass();
  $order->products = uc_cart_get_contents();
  if (is_array($order->products) && !empty($order->products)) {
	foreach ($order->products as $product) {
	  $order_subtotal += $product->price * $product->qty;
	}
  }
  return $order_subtotal;
	}catch (Exception $ex) {
  	watchdog_exception('getOrderTotal', $ex);
  	expertusErrorThrow($ex);
  }
}

//Get Location Based Payment Type
function getLocationBasedPaymentType(){
 try {
	$select = db_select('slt_profile_list_items', 'pli');
	$select->condition('pli.code', 'cme_pmt_typ');
	$select->addField('pli', 'id');
	expDebug::dPrintDBAPI(' $selectStmt = ' ,$select);
	$parentId = $select->execute()->fetchField();
	if($parentId){
		$selectStmt = db_select('slt_profile_list_items', 'pli');
		// Set conditions
		$selectStmt->condition('pli.parent_id', $parentId);
		$selectStmt->condition('pli.is_active', 'Y');
		$selectStmt->addField('pli', 'id', 'id');
		$selectStmt->addField('pli', 'name', 'name');
		$result = $selectStmt->execute()->fetchAll();
		return $result;
  }
  else
  	return false;
  }catch (Exception $ex) {
  	watchdog_exception('getLocationBasedPaymentType', $ex);
  	expertusErrorThrow($ex);
  }
}

//Get Payment instruction message
function getPaymentInstructionMessage ($payment_type){
  try{
  	$result = '';
    $payIns = getProfileListItemByCode($payment_type);
    if($payIns){
    	$result  = $payIns[0]->attr2;
    }
    return $result;
  }catch(Exception $exception){
  expDebug::dPrint("Exception Occurs when get payment instruction", 4);
  }
}

/* Grabs the items in a shopping cart for a user.*/
function expertus_cart_add_get_contents(&$item){
 try{
  $resultDt = getCatalogInfofromNodeId($item->nid);

  if(count($resultDt)==0){
    $resultDt->CourseId = '';
    $resultDt->ClassId  = '';
    $resultDt->DeliveryTypeName       = '';
    $resultDt->DeliveryTypeId   = '';
    $resultDt->LocId       = '';
    $resultDt->LocName = '';
  }
  $item->courseId = $resultDt->CourseId;
  $item->title = $resultDt->ClassTitle; // Added For adding Present Name To the Product
  $item->ClassCode = $resultDt->ClassCode; // Code added For this Ticket #0036764
  $item->classId = $resultDt->ClassId;
  $item->delivery_type = t($resultDt->DeliveryTypeName);
  $item->delivery_type_code = $resultDt->DeliveryTypeId;
  $item->location_id = $resultDt->LocId;
  $item->location = $resultDt->LocName==''?'N/A':$resultDt->LocName;
  if($item->content_from == 'admin_update'){
	  //Get order status
	  $sltOrderId  = getSltLMSOrderId($item->order_id);
	  $enrId = '';
	  $compStatus = 0;
	  if($resultDt->DeliveryTypeId == 'cre_sys_obt_trn' || $resultDt->DeliveryTypeId == 'cre_sys_obt_cur' || $resultDt->DeliveryTypeId == 'cre_sys_obt_crt'){
	  	$enrId = getMasterEnrIdFromOId($resultDt->ClassId,$sltOrderId);
	  	$compStatus = getCancelStatusFromEnrId($enrId,'tp');
	  }
	  else{
	  	$enrId = getEnrIdFromOId($resultDt->ClassId,$sltOrderId);
	  	$compStatus = getCancelStatusFromEnrId($enrId,'class');
	  }
	  $orderStatus = getSltOrderItemDetails($sltOrderId, $resultDt->ClassId, $resultDt->DeliveryTypeId);
	  if($compStatus){
	  	$item->order_status = 'na';
	  }
	  else{
	  	$item->order_status = $orderStatus[0]->order_status;
	  }
	  $item->enr_id = $enrId;
	  $refund_amount = '';
	  $taxAmt = 0;
	  $discountAmt = 0;
	  if($item->order_status == 'cme_pmt_sts_rjt' && $orderStatus[0]->refund_amount > 0){
		  $refundAmt = ($orderStatus[0]->refund_amount) ? ($orderStatus[0]->refund_amount) : 0;
		  $taxAmt    = ($orderStatus[0]->tax_amount) ? ($orderStatus[0]->tax_amount) : 0;
		  $discountAmt  = getDropListDiscountAmount($item->nid,$item->order_id);
		  if($refundAmt){
		  	$refund_amount = $refundAmt - $taxAmt;
		  	$refund_amount = $refund_amount + $discountAmt;
		  }
	  }else if($item->order_status == 'cme_pmt_sts_rjt' && $orderStatus[0]->refund_amount <= 0){
	  	$refund_amount = 0;
	  }
	  $item->refund_amount   = $refund_amount;
	  $item->tax_amount 	   = $taxAmt;
	  $item->discount_amount = $discountAmt;
  }
  }catch (Exception $ex) {
  	watchdog_exception('expertus_cart_add_get_contents', $ex);
  	expertusErrorThrow($ex);
  }
}

function getSltOrderItemDetails($sltOrderId, $entityId, $entityType){
 try{
	$select = db_select('slt_order_items', 'oi');
	$select->addField('oi','id', 'slt_ord_item_id');
	$select->addField('oi','order_status');
	$select->addField('oi','refund_amount');
	$select->addField('oi','tax_amount');
	$select->condition('oi.order_id', $sltOrderId);
	if($entityType == 'cre_sys_obt_trn' || $entityType == 'cre_sys_obt_cur' || $entityType == 'cre_sys_obt_crt'){
		$select->condition('oi.program_id', $entityId);
	}
	else{
		$select->condition('oi.class_id', $entityId);
	}
	$select->range(0, 1);
	expDebug::dPrintDBAPI(' $$select SQL = ' , $select);
	$orderStatus = $select->execute()->fetchAll();
	return $orderStatus;
	}catch (Exception $ex) {
		watchdog_exception('getSltOrderItemDetails', $ex);
		expertusErrorThrow($ex);
	}
}

function getMasterEnrIdFromOId($entityId,$lmsOrderId){
	try{
		$select = db_select('slt_enrollment', 'enr');
		$select->addField('enr', 'master_enrollment_id');
		$select->condition('enr.order_id', $lmsOrderId);
		$select->condition('enr.reg_status', array('lrn_crs_reg_rsc','lrn_crs_reg_rsv'), 'NOT IN');
		$select->condition('enr.master_enrollment_id', '','IS NOT NULL');
		expDebug::dPrintDBAPI(' $$select SQL = ' , $select);
		$results = $select->execute()->fetchAll();
		foreach($results as $masEnrId){
			$selectStmt = db_select('slt_master_enrollment', 'menr');
			$selectStmt->condition('menr.id', $masEnrId->master_enrollment_id);
			$selectStmt->condition('menr.program_id', $entityId);
			$selectStmt->addExpression('COUNT(1)', 'count');
			expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
			$cnt = $selectStmt->execute()->fetchField();
			if($cnt){
				return $masEnrId->master_enrollment_id;
			}
		}
	}catch (Exception $ex) {
		watchdog_exception('getMasterEnrIdFromOId', $ex);
		expertusErrorThrow($ex);
	}
}

function getEnrIdFromOId($entityId,$lmsOrderId){
	try{
		$select = db_select('slt_enrollment', 'enr');
		$select->addField('enr', 'id');
		$select->condition('enr.order_id', $lmsOrderId);
		$select->condition('enr.class_id', $entityId);
		$select->condition('enr.reg_status', array('lrn_crs_reg_rsc','lrn_crs_reg_rsv'), 'NOT IN');
		$select->range(0, 1);
		expDebug::dPrintDBAPI(' $$select SQL = ' , $select);
		$results = $select->execute()->fetchField();
		return $results;
		}catch (Exception $ex) {
			watchdog_exception('getEnrIdFromOId', $ex);
			expertusErrorThrow($ex);
		}
}

function getCancelStatusFromEnrId($enrId,$entityType){
	try{
		if(!$enrId){
			return 0;
		}
		$selectStmt = db_select('slt_enrollment', 'enr');
		$selectStmt->addExpression('COUNT(1)', 'count');
		if($entityType == 'class'){
			$selectStmt->condition('enr.id', $enrId);
		}
		else{
			$selectStmt->condition('enr.master_enrollment_id', $enrId);
		}
		$selectStmt->condition('enr.comp_status', array('lrn_crs_cmp_cmp','lrn_crs_cmp_inp','lrn_crs_cmp_inc','lrn_crs_cmp_nsw','lrn_crs_cmp_att'), 'IN');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' , $selectStmt);
		$compCount = $selectStmt->execute()->fetchField();
		return $compCount;
	}catch (Exception $ex) {
		watchdog_exception('getCancelStatusFromSltOrderId', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * Get the product billing information.
 */
function expertus_checkout_pane_userorginfo($op, $order, $form = NULL, &$form_state = NULL,$callFrom = '') {
	try{
  //slp_purchaseaccountinfo_sel
	$fieldStatus = ($callFrom == 'pay') ? 'disabled' : '';
  if(!$order->uid && arg(0) == 'administration'){
  	$userId = 0;
  }
  else if(!$order->uid){
  	$userId = getSltPersonUserId();
  }
  else{
  	$userId  = getUserIdFromDrupalUid($order->uid);
  }
  $resultDt = getPersonDetails($userId, array('id', 'first_name', 'last_name','full_name', 'middle_name', 'user_name', 'email', 'status', 'in_active_reason', 'addr1', 'addr2', 'country', 'state', 'city',
  										'zip', 'phone_no','org_id'));

  if(empty($resultDt['id'])){
  	$resultDt['user_name']  = '';
    $resultDt['first_name'] = '';
    $resultDt['last_name']  = '';
    $resultDt['full_name']  = '';
    $resultDt['addr1']      = '';
    $resultDt['addr2']      = '';
    $resultDt['city']       = '';
    $resultDt['state']      = '';
    $resultDt['zip']        = '';
    $resultDt['country']    = '';
    $resultDt['phone_no']   = '';
    $resultDt['org_id']     = '';
    $org_name               = '';
    $org_country            = '';
    $resultDt['email']      = '';
  }
  else{
    $resultDt['state']   = getStateName($resultDt['state'], $resultDt['country']);
    $resultDt['country'] = getCountryName($resultDt['country']);
  }
  if($resultDt['org_id']){
    $org_details = getOrganizationDetails($resultDt['org_id'], array('name','country'));
    $org_name    = $org_details['name'];
    $org_country = $org_details['country'];
  }
  switch ($op) {
  	case 'view':
      $description = '<div class="cart_head user-info">'.t('LBL556').'</div>';
       $user_full_name = ucwords($resultDt['full_name']);
       $user_first_name = ucwords($resultDt['first_name']);
       $user_last_name = ucwords($resultDt['last_name']);
       $contents['org_user_first_name'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL056') . ':</label><span>' . $user_first_name .'</span></div>',
        );
       $contents['org_user_last_name'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL058') . ':</label><span>' . $user_last_name .'</span></div>',
        );
        $contents['org_address1'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL064') . ':</label><span>'. $resultDt['addr1'] .'</span></div>',
        );
        $contents['org_address2'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL065') . ':</label><span>' . $resultDt['addr2'] .'</span></div>',
        );
        $contents['org_city'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL066') .':</label><span>' .  ucfirst($resultDt['city']) .'</span></div>',
        );
        $contents['org_state'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL067') . ':</label><span>' . $resultDt['state'] .'</span></div>',
        );
        $contents['org_country'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL039') . ':</label><span>' . $resultDt['country'] .'</span></div>',
        );
        $contents['org_postal'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL562') . ':</label><span>' . $resultDt['zip'] .'</span></div>',
        );
        $contents['org_user_phone'] = array(
          '#markup' => '<div class="user-information-container-form"><label>'. t('LBL070') . ':</label><span>' . $resultDt['phone_no'].'</span></div>',
        );
        $contents['org_id'] =  array(
          '#value' => $org_country,
        );

	  return array('description' => $description, 'contents' => $contents);

    case 'process':
      $order->org_user_name	= $resultDt['full_name'];
      $order->org_first_name = $resultDt['first_name'];
      $order->org_last_name = $resultDt['last_name'];
      $order->org_name_text = $org_name;
      $order->org_address1 	= $resultDt['addr1'];
      $order->org_address2 	= $resultDt['addr2'];
      $order->org_city 		= $resultDt['city'];
      $order->org_state		= $resultDt['state'];
      $order->org_postal	= $resultDt['zip'];
      $order->org_country 	= $resultDt['country'];
      $order->org_user_phone= $resultDt['phone_no'];
      return TRUE;

    case 'review':
      $user_full_name = ucwords($resultDt['full_name']);
      $user_first_name = ucwords($resultDt['first_name']);
      $user_last_name = ucwords($resultDt['last_name']);
		  $address = array();
		  $address = uc_address_format(
		    $user_first_name,
		    $user_last_name,
		    '',
		    $resultDt['addr1'],
		    $resultDt['addr2'],
		    ucfirst($resultDt['city']),
		    $resultDt['state'],
		    $resultDt['zip'],
		    $resultDt['country'],
		    true,
		    'Array'
		  );
		  $review[] = array('title' => t('LBL107'), 'data' => $address[0]); //Name
		  if(!empty($address[1])){
		  	$address1 = preg_replace('`^<br( /)?>`', '' , $address[1]);
		  	$review[] = array('title' => t('LBL727'), 'data' => $address1); //Address
		  }
		  if(!empty($address[2])){
		  	$address2 = preg_replace('`^<br( /)?>`', '' , $address[2]);
		  	$review[] = array('title' => t('LBL562'), 'data' => $address[2]); //Postal Code
		  }
		  if (uc_address_field_enabled('phone') && !empty($resultDt['phone_no'])) {
		  	$review[] = array('title' => t('LBL070'), 'data' => $resultDt['phone_no']);
		  }

		  return $review;

    case 'shortview':
    	$description = '<div class="cart_head user-info">'.t('LBL850').'</div>';
    	$user_first_name = ucwords($resultDt['first_name']);
     	$user_last_name = ucwords($resultDt['last_name']);
    	$user_name = ucwords($resultDt['user_name']);
    	$email = $resultDt['email'];
    	$contents['org_user_name'] = array(
    			'#markup' => '<div class="user-information-container-form"><input type="hidden" name="form-user-id" id="form-user-id">
    										<span class ="edit-panes-full-name"><label for="edit-panes-full-name"><span class="uc-userinfo-label">'. t('LBL056') .':</span><span title="This field is required." class="user-info-mandatory">*</span></label>
 												<span><input type="text" class="form-text" name="first_name" id="edit-panes-productorg-first-name" value = '.$user_first_name.'  '.$fieldStatus.'></span></span></div>',
    	);
    	//<label for="edit-panes-user-name"><span class="uc-userinfo-label">'. t('User Name') .':</span> </label>
    	//<span><input type="text" class="form-text" name="user_name" id="edit-panes-productorg-user-name" value = '.$user_name.'  '.$fieldStatus.'></span>
    	$contents['org_user_last_name'] = array(
    			'#markup' => '<div class="user-information-container-form"><label for="edit-panes-last-name"><span class="uc-userinfo-label">'. t('LBL058') .':</span><span title="This field is required." class="user-info-mandatory">*</span></label>
 												<span><input type="text" class="form-text" name="last_name" id="edit-panes-productorg-last-name" value = '.$user_last_name.'  '.$fieldStatus.'></span>
    										</div>',
    	);
    	//<span class ="edit-panes-email"><label for="edit-panes-email"><span class="uc-userinfo-label">'. t('LBL061') .':</span> </label>
    	//<span><input type="text" class="form-text" name="email" id="edit-panes-productorg-email" value = '.$email.'  '.$fieldStatus.'></span></span>

    	$contents['user_name'] = array(
    			'#markup' => '<div class="user-information-container-form"><input type="hidden" name="form-user-id" id="form-user-id">
    										<span class ="edit-panes-full-name"><label for="edit-panes-full-name"><span class="uc-userinfo-label">'. t('LBL054') .':</span><span title="This field is required." class="user-info-mandatory">*</span></label>
 												<span><input type="text" class="form-text" name="user_name" id="edit-panes-productorg-username" value = '.$user_name.'  '.$fieldStatus.'></span></span></div>',
    	);

    	$contents['org_user_email'] = array(
    			'#markup' => '<div class="user-information-container-form"><label for="edit-panes-last-name"><span class="uc-userinfo-label">'. t('LBL061') .':</span><span title="This field is required." class="user-info-mandatory">*</span></label>
 												<span><input type="text" class="form-text" name="email" id="edit-panes-productorg-email" value = '.$email.'  '.$fieldStatus.'></span>
    										</div>',
    	);

    	return array('description' => $description, 'contents' => $contents);
		  /*if (variable_get('uc_order_capitalize_addresses', TRUE)) {
			  foreach($address as $addr){
			    	$addressReturned[] = drupal_strtoupper($addr);
			  }
		  }*/


  }
  }catch (Exception $ex) {
  	watchdog_exception('expertus_checkout_pane_userorginfo', $ex);
  	expertusErrorThrow($ex);
  }
}

function moduleCourseMapping($prgId,$module_id){
	try{
  $select = db_select('slt_module_crs_mapping','map');
  $select->addField('map','course_id');
  $select->addField('map','is_required');
  $select->condition('map.program_id', $prgId,'=');
  $select->condition('map.module_id', $module_id,'=');
  $result  = $select->execute()->fetchAll();
  return $result;
  }catch (Exception $ex) {
  	watchdog_exception('moduleCourseMapping', $ex);
  	expertusErrorThrow($ex);
  }
}
function getClassSessionValidate($orderId){
	try{
		$loggedusr = getSltpersonUserId();
		$resultarr =array();
		$qry = db_select('slt_enrollment','enr');
		$qry->addField('enr','course_id');
		$qry->addField('enr','class_id');
		$qry->addField('enr','master_enrollment_id');
		$qry->condition('enr.order_id',$orderId,'=');
		expDebug::dPrintDBAPI(' $selectStmt SQL = ' ,$qry);
		$result  = $qry->execute()->fetchAll();
		expdebug::dPrint("list of class id ".print_r($result,true),5);
		$classIds = array();
		foreach ($result as $res) {
			$classIds[]= $res->class_id;
		}
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$selectCls = db_select('slt_course_class','cls');
		$selectCls->leftjoin('slt_course_class_session', 'ses','cls.id=ses.class_id and ses.id = (select id from slt_course_class_session where class_id = ses.class_id order by start_date ASC LIMIT 1)');
		$selectCls->leftjoin('slt_profile_list_items', 'splitz', 'ses.timezone = splitz.code  AND ses.timezone IS NOT NULL  AND splitz.lang_code = \'cre_sys_lng_eng\'');
		$selectCls->addField('splitz', 'attr2', 'phptimezone');
		$selectCls->addField('cls','id','class_id');
		$selectCls->addExpression("CONCAT(DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'), ' ', ses.start_time, ':00')", 'start_date_time');
		$selectCls->addField('ses','timezone','timezone');
		$selectCls->condition('cls.status','lrn_cls_sts_atv','=');
		$selectCls->condition('cls.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN');
		$selectCls->condition('cls.id',$classIds ,'IN');
		$selectCls->groupBy("ses.class_id having start_date_time < IFNULL(CONVERT_TZ(now(), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW())");
		expDebug::dPrintDBAPI('$select updateClassAsCompleted', $selectCls);
		$sessionsList = $selectCls->execute()->fetchAll();
		$conflict_cls = array();
	 	foreach ($sessionsList as $session) {
	        if (empty($session->phptimezone)) {
	          	$curDateTimeUX = time();
		        $sesEndtDateTimeUX = strtotime($session->start_date_time);
		        expDebug::dPrint('$sesStartDateTimeUX = ' . $sesStartDateTimeUX, 4);
		        if ($sesEndtDateTimeUX <= $curDateTimeUX) {
		        	$conflict_cls[] = $session->class_id;
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
		          $conflict_cls[] = $session->class_id;
		      }
	        }
	    }
	    if(count($conflict_cls) > 0){
		    //Get program Title
		    $selectCls = db_select('slt_enrollment','enr');
			$selectCls->leftjoin('slt_master_enrollment', 'menr','enr.master_enrollment_id = menr.id');
			$selectCls->leftjoin('slt_program', 'prg', 'prg.id = menr.program_id');
			$selectCls->leftjoin('slt_course_class', 'cls', 'cls.id = enr.class_id');
			$selectCls->addField('enr', 'master_enrollment_id', 'master_enrollment_id');
			$selectCls->addExpression('IF(enr.master_enrollment_id IS NULL ,cls.title,prg.title)','program_title');
			
			$selectCls->condition('enr.order_id',$orderId,'=');
			$selectCls->condition('enr.class_id',$conflict_cls,'IN');
			expDebug::dPrintDBAPI('$select updateClassAsCompleted', $selectCls);
			$prgarr = $selectCls->execute()->fetchAll();
		   // expDebug::dPrint("program list array ".print_r($prgarr,true),5);
		    $prgList = '';
		    $clsList = '';
			foreach ($prgarr as $prg) {
				if(!empty($prg->master_enrollment_id))
					$prgList .= ($prgList ? ',' : '').$prg->program_title ;
				else
					$clsList .= ($clsList ? ',' : '').$prg->program_title ;
			}
		    if(!empty($prgList))
		    	$resultarr['invalid_session_tp'] = $prgList.' - '.t('ERR108');
		    if(!empty($clsList))
		    	$resultarr['invalid_session_cls'] = $clsList.' - '.t('MSG499');
	    }
	    //seat available check
	   /* $seat_full = getAvailableSeatInClasses($classIds,$loggedusr,'cart');
	    expDebug::dPrint("program list seat full ".print_r($seat_full,true),5);
	    if(count($seat_full)>0){
	    	 //Get program Title
		    $selectCls = db_select('slt_enrollment','enr');
			$selectCls->leftjoin('slt_master_enrollment', 'menr','enr.master_enrollment_id = menr.id');
			$selectCls->leftjoin('slt_program', 'prg', 'prg.id = menr.program_id');
			$selectCls->addField('prg', 'title', 'program_title');
			//$selectCls->addField('cls','id','class_id');
			
			$selectCls->condition('enr.order_id',$orderId,'=');
			$selectCls->condition('enr.class_id',$seat_full,'IN');
			expDebug::dPrintDBAPI('$select updateClassAsCompleted', $selectCls);
			$prgarr = $selectCls->execute()->fetchAll();
		   // expDebug::dPrint("program list array ".print_r($prgarr,true),5);
		    $prgList = '';
			foreach ($prgarr as $prg) {
			//	expdebug::dPrint("list of class id inside loop ".print_r($res->class_id,true),5);
				$prgList .= ($prgList ? ',' : '').$prg->program_title ;
			}
			//$prgList = implode(',',$prgarr);
		   // expDebug::dPrint("program list array after ".print_r($prgList,true),5);
	    	 $resultarr['seat_full'] = $prgList.'-'.t('ERR022');
	    }*/
	    
	    
		return $resultarr;
	}catch (Exception $ex) {
  		watchdog_exception('moduleCourseMapping', $ex);
  		expertusErrorThrow($ex);
  }
}
//Added by vetrivel for #0072173
function toGetSessionExpired($course_timezone,$start_date,$start_time){
    
    $timeZone = date_default_timezone(false);
    $defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
    $sessionTZ = new DateTimeZone($defaultTimezone);
    $toDay = new DateTime(null,$sessionTZ) ;
    $toDayDT = $toDay->format('Y-m-d H:i');
    expDebug::dPrint("today  => ".$toDayDT,4);
    expDebug::dPrint("result->course_timezone  => ".$course_timezone,4);
    $date = new DateTime($start_date);
    $course_date = $date->format('Y-m-d');
    $course_date_time = $course_date.' '.$start_time;
    expDebug::dPrint("course  => ".$course_date_time,4);
    $courseTimeZone = timeZoneConvert($course_date_time,$course_timezone,$defaultTimezone);
    expDebug::dPrint($courseTimeZone, 4);
    expDebug::dPrint("course TIME => ".$courseTimeZone->date,4);
    $startDate = date_format($courseTimeZone,'Y-m-d H:i');
    expDebug::dPrint("default  => ".$startDate,4);
    
    if(strtotime($toDayDT) >= strtotime($startDate)){
        return 'classSessionExpired';
    }
    
}


?>