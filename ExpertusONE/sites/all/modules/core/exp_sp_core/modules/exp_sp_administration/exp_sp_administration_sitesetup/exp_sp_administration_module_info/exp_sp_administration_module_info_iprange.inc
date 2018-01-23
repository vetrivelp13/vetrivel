<?php

/**
 * IP Ranges Admin inc file
 *
 * @package pagepackage
 * @author Prasanth P
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 13-Oct-2015
 */

/**
 * Show exp_sp_pwdpolicy_addedit_form in ctools popup
 */
function exp_sp_administration_iprange_display_form($js = NULL) {
	
	try {
		global $user;
    $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
    drupal_add_js(drupal_get_path('module', 'exp_sp_administration_module_info') . '/exp_sp_administration_module_info_iprange.js',
                                                 $js_module_optional);
	
    if ($user->language == 'pt-pt') {
    	$defaultWidth = 100;
    } 
    
    // Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $iprange_style = array( 
	    'ctools-admin-iprange-style' => array(
	      'modalSize' => array(
			'width' => 475,
	         'minHeight' => 180,
	      	 'minWidth' =>$defaultWidth,	
	      ),
	    )
	  );
	
	  drupal_add_js($iprange_style, 'setting');
	
	  if ($js) {
	    // Prepare list of forms to be displayed in the modal
	    $formIdList = array(
	      array('id' => 'exp_sp_administration_iprange_form',
	      		'title' => (t('LBL1302'))), //Network Access
	    );	
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	  }
	  else {
	    throw new Exception('exp_sp_administration_module_info_iprange.inc: exp_sp_administration_iprange_display_form() : nojs');
	  }
	}
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_iprange_display_form', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 * Build the form for password policy admin
 */
function exp_sp_administration_iprange_form($form, &$form_state) {
	try{
		global $theme_key;
		$formWrapperId = 'iprange-admin-form';
		
		$categorizedIPS=getCategorizedIPS();
		$ipCount = count($categorizedIPS);
		$loggedUserId = getSltpersonUserId();
		$popupAddSessionentityType    = 'iprange';
		
		$formName = 'iprange_detail';
		$form_state['form_unique_name'] = 'iprange_detail'; //$formName;
		if(count($form[$formName])){
			unset($form[$formName]);
		}
		if($ipCount==0){
			$popupAddiprangevisibPopupId  = 'qtip_visible_disp_addsession_iprange';
			expDebug::dPrint('$form = ' . print_r($formName, true), 5);
			$popupAddSession = "{'entityId':'0','entityType':'addip','url':'administration/sitesetup/moduleinfo/ipranges/addedit/0','popupDispId':'".$popupAddiprangevisibPopupId."','catalogVisibleId':'1_".$popupAddiprangevisibPopupId."','wBubble':'400','hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent'}";
			$form[$formName] = array(
					'#type' => 'markup',
					'#prefix' => '<div id="admin-data-grid"><div id="iprange-details-fieldset-wrapper-iprange" class="admin-tab-list-row iprange-details-fieldset-wrapper"><div id="'.$formWrapperId.'" class="admin-datagrid-pagination"><div class="admin-datagrid-iprange-with-addanother classroom-session">',
					'#suffix' => '</div></div></div></div>',
			);
			$form[$formName]['add_ip_details'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="admin-datagrid-add-btn-wrapper-container_ip"><div class="add_iprange_popup"><div class="add-iprange-popup-box" id="'.$popupAddiprangevisibPopupId.'"><a onclick =$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;"><span class="admin-save-button-left-bg"></span><span class="admin-save-button-middle-bg">'.t('LBL1300').'</span><span class="admin-save-button-right-bg"></span><div class="clearBoth"></div></a></div></div>',
			);		 
			$form[$formName]['add_iprange_msg'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="admin-empty-text-msg" id="expertus-no-online-users-msg-iprange">'.t('LBL1301').'</div></div>',
			);
		}//No IP's Configured
		else {
			
			$entityId = core_encrypt($loggedUserId);
			$entityType = "ipr_sys_obt_cls";
			$uniqueId = "iprange-".$entityId."-".$entityType;
			$excludedCourseId= $loggedUserId;
			
			$form[$formName] = array(
					'#type' => 'markup',
					'#prefix' => '<div id="admin-data-grid"><div id="iprange-details-fieldset-wrapper-iprange1" class="admin-tab-list-row iprange-details-fieldset-wrapper"><div id="'.$formWrapperId.'" class="admin-datagrid-pagination"><div class="admin-datagrid-iprange-with-addanother classroom-session">',
					'#suffix' => '</div></div></div></div>',
			);
			
			$form[$formName]['iprange_view_grid_markup'] = array(
					'#type' => 'markup',
					'#markup' => '<div id="datagrid-div-'.$uniqueId.'"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
			);
			
			$displayModeDataGrid = 'view_only';
			
			$form[$formName]['iprange_view_grid_load'] = array(
					'#type' => 'markup',
					'#prefix' => '<div style="display:none;"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\''.$displayModeDataGrid.'\', \'iprange\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div></div>',
			);
			
			$popupentityId      = $class_id;
			$popupAddSessionentityType    = 'iprange';
			$popupAddSessionIdInit        	 = $popupentityId.'_'.$popupAddSessionentityType;
			$popupAddiprangevisibPopupId  = 'qtip_visible_disp_addsession_iprange';
			
			$popupAddIprange = "{'entityId':'0','entityType':'addip','url':'administration/sitesetup/moduleinfo/ipranges/addedit/0','popupDispId':'".$popupAddiprangevisibPopupId."','catalogVisibleId':'2_".$popupAddiprangevisibPopupId."','wBubble':'400','hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent'}";			
			
				$form[$formName]['ip_ranges'] = array(
						'#type' => 'markup',
						'#markup' => '<div class="iprange-add-another-popup-container"><div class="addedit-form-cancel-container-actions-ip"><div class="add_session_popup" id="'.$popupAddiprangevisibPopupId.'" ></div><a class="add-custombtn-symbol" onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddIprange.'); return false;">'.t('LBL386').'</a></div></div>',
				);
			// }	
		}//IP's Configured
		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_iprange_form', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * IP Range module whitelist/blacklist IP's configuration page
 */
function exp_sp_administration_iprange_addedit_form($rangeType,$ip_rangeId) {
	try {
		expDebug::dPrint('E1>>>>>>>>>>>>ip_rangeId'. print_r($rangeType."<..............>".$ip_rangeId, true), 5);
		$form_state = array(
   		'no_redirect'  => TRUE,
    	'build_info'   => array('args' => array('ajax')),
   		'ip_rangeId'    => $ip_rangeId
		);

		$formId                 = 'add_edit_new_ip_html';
		$builtForm              = drupal_build_form($formId, $form_state);
		$renderedTabContentHTML = drupal_render($builtForm);
		$drupalSettings         = getDrupalSettings();

		$commerceResponse = array(
        'drupal_settings'     => $drupalSettings,
        'render_content_main' => $renderedTabContentHTML
		);

		echo drupal_json_output($commerceResponse);
		exit;
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_iprange_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}
function add_edit_new_ip_html_validate($form, &$form_state){
try{
	expDebug::dPrint('Deleted IP Range record >>'.print_r($form_state,true),5);
	$form_state["storage"]["form_validate"] = true;
	$strt_ip=trimInput($form_state['values']['start_ip']);
	$end_ip=trimInput($form_state['values']['end_ip']);
	$iprangeID=$form_state['values']['iprangeID'];
	
	$strt_ipp = (int) ip2long($strt_ip);
	$end_ipp = (int)  ip2long($end_ip);
	if(empty($strt_ip) || $strt_ip== t('LBL1303')){
		form_set_error('start_ip',t('LBL1304').' '.t('ERR101'));
		$form_state["storage"]["form_validate"] = false;
	}else if(!filter_var($strt_ip, FILTER_VALIDATE_IP)){
		form_set_error('start_ip',t('LBL1304').' '.t('ERR250'));
		$form_state["storage"]["form_validate"] = false;
	}
	if((!empty($end_ip)) && (!filter_var($end_ip, FILTER_VALIDATE_IP))){
		form_set_error('end_ip',t('LBL1305').' '.t('ERR250'));
		$form_state["storage"]["form_validate"] = false;
	}
	if($strt_ip == $end_ip)
	{
		form_set_error('start_ip',t('LBL1311'));
		$form_state["storage"]["form_validate"] = false;
	}
	if((!empty($strt_ip)) && (!empty($end_ip)))
	{
		if($strt_ipp > $end_ipp)
		{
			form_set_error('start_ip',t('LBL1312'));
			$form_state["storage"]["form_validate"] = false;
		}
	}	
	if(empty($iprangeID) || !empty($iprangeID))
	{
	    if((!empty($strt_ip)))
	    {
		$existIP=getexistip($strt_ip,$end_ip,$iprangeID);
		   if(!empty($existIP)){
			form_set_error('start_ip',t('LBL1313'));
			$form_state["storage"]["form_validate"] = false;
		   }
		expDebug::dPrint('add_edit_new_ip_html_validate'.$countexistip,4);
		
	    }
	}
	} catch (Exception $ex) {
		watchdog_exception('add_edit_new_ip_html_validate', $ex);
		expertusErrorThrow($ex);
	}
}	
function add_edit_new_ip_html_submit($form, &$form_state){
	
	try {
		if(!empty($form_state['ip_rangeId']) && $form_state['ip_rangeId']!=0){
			updateIPRangeDetails($form_state);
		}else if(empty($form_state['ip_rangeId']) || $form_state['ip_rangeId']==0){
			
			insertIPRangeDetails($form_state);
		}
		$form_state['rebuild']  = TRUE;
	} catch (Exception $ex) {
		watchdog_exception('add_edit_new_ip_html_submit', $ex);
		expertusErrorThrow($ex);
	}
	
}
function insertIPRangeDetails($form_state){
	expDebug::dPrint('form state in ip insert'.print_r($form_state, 1));
    $loggedInUserId  = getIdOfLoggedInUser();
	$ip=trimInput($form_state['values']['start_ip']);
	$ip=trimInput($form_state['values']['end_ip'])==''?$ip:$ip.'-'.trimInput($form_state['values']['end_ip']);
	$cType=t('LBL1307');
	if(trimInput($form_state['values']['ip_category'])=='0'){
		$cType='whitelist';
	}else if(trimInput($form_state['values']['ip_category'])=='1'){
		$cType='blacklist';
	}
	$select="select ip,type from ip_ranges where (inet_aton((SUBSTRING_INDEX(ip, '-', 1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')) OR (inet_aton((SUBSTRING_INDEX(ip, '-', -1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId'));";
	if($cType=='whitelist')
	{
		$ip_1=trimInput($form_state['values']['start_ip']);
		$ip_2=trimInput($form_state['values']['end_ip']);
		if(!empty($ip_2))
		{
		   $select="delete from flood where (inet_aton((SUBSTRING_INDEX(identifier, '-', -1))) BETWEEN inet_aton('$ip_1') AND inet_aton('$ip_2')) OR (identifier BETWEEN inet_aton('$ip_1') AND inet_aton('$ip_2')) and event in ('failed_login_attempt_ip','failed_login_attempt_user')";
		}
		else
		{
			$select="delete from flood where ( ((SUBSTRING_INDEX(identifier, '-', -1))='$ip_1') OR (identifier = '$ip_1')) and event in ('failed_login_attempt_ip','failed_login_attempt_user')";
		}
		$result=db_query($select);
		expDebug::dPrint('getexistip $result->' .print_r($result,true),4);
	}
		
	$insertStmt = db_insert('ip_ranges');
	$fields=array(
			'ip' => $ip,
			'type' => $cType
	);
	$fields = trimInputFields($fields);
	$insertStmt ->fields($fields);
	$ipRangeId = $insertStmt->execute();

	make_audit_trail_entries($fields, $loggedInUserId, null, $ipRangeId, 'Inserted', 'Ip-Ranges' , 'exp_sp_administration_module_info_iprange', 'insertIPRangeDetails', array('ip', 'type'),null);

}
/**
* Fetch whitelist/blacklist IP's configured
*/
function exp_sp_administration_iprange_delete($ip_rangeId){
	try{
			$deleteStmt = db_delete('ip_ranges');
			$deleteStmt->condition('bid', $ip_rangeId,'=');
			$numUpdated = $deleteStmt->execute();
			$categorizedIPS=getCategorizedIPS();
			expDebug::dPrint('Deleted IP Range record >>'.$numUpdated,5);
		}catch (Exception $ex) {
			watchdog_exception('exp_sp_administration_iprange_delete', $ex);
			expertusErrorThrow($ex);
		}
	}
function exp_sp_administration_iprange_details(){
	try {
		expDebug::dPrint('E1 loadCtoolDataGridType'.$mode.'---'.$type.'---'.$searchKeyword.'---'.$entityId.'---'.$entityType.'---'.$excludedId,5);
		$page      = (int)$_GET['page'];
		$limit     = $_GET['rows'];
		$sidxVal      = stripApostrophe(stripRegStatus($_GET['sidx']));
		if((!empty($limit) && !is_numeric($limit)) || !empty($page) && !is_numeric($page)){
			throw new Exception('Non Numeric Values');
		}
	
		if(!empty($sidxVal)){
			$sidx = escape_string($sidxVal);
			$_GET['sidx'] = $sidx;
		}
	
		$sord      = $_GET['sord'];
		if(!empty($sord)){
			if((stripos('desc',$sord) !== false)){
				$sord = 'desc';
			}else{
				$sord = 'asc';
			}
			$_GET['sord'] = $sord;
		}
		$tab       = $_GET['tab'];

			$uniqueId  =  "$type-".$entityId."-$entityType";

		$queryRequired = 1;
		$fetchFields = array();
	
		$mode = empty($mode) ? 'view' : $mode;
		// Added by Vincent on 02, Jan 2014 for #0029687: Refresh on Admin Enrollments pages
		// Below code is handeld in exp_sp_administration_order.inc (in loadOrderAddProduct())
		// Update both file if any changes required.
		$crPage = explode('~',$_COOKIE['current_page']);
		if($mode=='view' && !empty($page) && $page==1){
			$isUpdate = explode('#',$crPage[2]);
			if($isUpdate[1]==1){
				$page= ($isUpdate[0])!=0?$isUpdate[0]:1;
			}
		}
		$countAvailable = 0;
		$count = 0;
	
		$listquery = getCategorizedIPQuery();
	
		$count = count($listquery->execute()->fetchAll());
			
		/* Paging Related */
		$start = $limit * $page - $limit;
		$listquery->range($start, $limit);
		$queryResult = $listquery->execute();
	
		if( $count >0 ){
			$total_pages = ceil($count/$limit);
		}else{
			$total_pages = 0;
		}
	
		if($page > $total_pages)
			$page = $total_pages;
		if($start < 0) {
			$start = 0;
		}
	
		$srch_result_response_roster->total   = $total_pages;
		$srch_result_response_roster->records = $count;
		$srch_result_response_roster->page    = $page;
		$srch_result_response_roster->unique_id    = $uniqueId;
		$srch_result_response_roster->msg    = '';
	
		$_SESSION['jq_count'] = array();
		$i=0;
	
	
		expDebug::dPrint('inside sessions data table');
		$i = 0;
		
		foreach ($queryResult as $rec_search_results)
		{
			$bid = $rec_search_results->bid;
			$ip      = $rec_search_results->ip;
			/* TO DO Changes */
			$popupAddiprangevisibPopupId  = 'qtip_visible_disp_addsession_iprange';
			$popupAddSession = "{'entityId':'0','entityType':'addip','url':'administration/sitesetup/moduleinfo/ipranges/addedit/0','popupDispId':'".$popupAddiprangevisibPopupId."','catalogVisibleId':'1_".$popupAddiprangevisibPopupId."','wBubble':'400','hBubble':'auto','tipPosition':'tipfaceMiddleRight','qtipClass':'admin-qtip-access-parent'}";			
			/* TO DO Changes */

			$titleLenght = 20;

	
	
			$stDate = explode(" ",$rec_search_results->start_date);
// 			$pipLine = ($isEnrolled > 0 || $theme_key == "expertusoneV2") ? '<span class="narrow-search-results-item-detail-pipe-line session-edit-delete-icons">|</span>' : '<span class="narrow-search-results-item-detail-pipe-line session-edit-delete-icons">|</span>';
			$pipLine = '<span class="narrow-search-results-item-detail-pipe-line iprange-edit-delete-icons">|</span>';
// 			$editText = ($theme_key == "expertusoneV2") ? ' ' : t('LBL063');
			$srch_result_response_roster->rows[$i]['id']   = $rec_search_results->bid;
			$srch_result_response_roster->rows[$i]['cell'] = array(
// 					'id'				 => $rec_search_results->bid,
					'ip_address' 			=> $rec_search_results->ip,
					'ip_category' 	  	 => '<span class="vtip" title="'.sanitize_data($rec_search_results->type).'">'.$rec_search_results->type.'</span>',
// 					'Edit'   			 => '<div style="position:relative;">'.(($deliveryType == 'lrn_cls_dty_vcl')? $pipLine :'').'<a class="session-added-edit-link-text" onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;">'.$editText.'</a>'.$pipLine.'<div class="add_session_popup" id="'.$popupAddiprangevisibPopupId.'" ></div></div>',
					//'Remove'   		 => $rec_search_results->session_id,
					'Edit'   			 => $rec_search_results->bid,//'<div style="position:relative;">'.$pipLine.'<a class="session-added-edit-link-texti onclick="$(\'#root-admin\').data(\'narrowsearch\').getQtipDiv('.$popupAddSession.'); return false;">'.$editText.'</a>'.$pipLine.'<div class="add_session_popup" id="'.$popupAddiprangevisibPopupId.'" ></div></div>',
// 					'ShowDelete'		 => ($isEnrolled > 0) ? $rec_search_results->session_id : 'hide',
					'Remove'             =>	$rec_search_results->bid
			);
			$i++;
		}
		
	
			
	
		echo drupal_json_encode($srch_result_response_roster);
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_iprange_details', $ex);
		expertusErrorThrow($ex);
	}
	
}
function updateIPRangeDetails($form_state){
	$loggedInUserId  = getIdOfLoggedInUser();
	$ip=trimInput($form_state['values']['start_ip']);
	$ip=trimInput($form_state['values']['end_ip'])==''?$ip:$ip.'-'.trimInput($form_state['values']['end_ip']);
	$cType='whitelist';
	if(trimInput($form_state['values']['ip_category'])=='0'){
		$cType='whitelist';
	}else if(trimInput($form_state['values']['ip_category'])=='1'){
		$cType='blacklist';
	}
	$form_state['values']['end_ip'];
	$updateStmt = db_update('ip_ranges');
	$fields = array(
			'ip'          => $ip,
			'type'		  => $cType
	);
	expDebug::dPrint('inside ip_range form State : '.print_r($form_state['values'],true) , 5);
	$fields = trimInputFields($fields);
	$updateStmt->fields($fields);
	$updateStmt->condition('bid', $form_state['ip_rangeId']);
	$numUpdated = $updateStmt->execute();

	$default = getCategorizedIPS($form_state['ip_rangeId']);
	expDebug::dPrint('make_audit_trail_entries>>>>>>>>Updated1' . print_r($numUpdated, true), 5);
	make_audit_trail_entries($fields, $loggedInUserId, $default, $numUpdated, 'Updated', 'Ip-Ranges' , 'exp_sp_administration_module_info_iprange', 'updateIPRangeDetails', array('ip', 'type'));
	expDebug::dPrint('make_audit_trail_entries>>>>>>>>Updated2' . print_r($numUpdated, true), 5);
}
function add_edit_new_ip_html($form, &$form_state) {
	try {
		$form = array();
		$categorizedIPS = array();
		$ip_rangeId = $form_state['ip_rangeId'];
		$bid="";
		$ipArray = array();
		$startIP="";
		$endIP="";
		$cTYPE="";
		if(!empty($ip_rangeId)){
			$categorizedIPS=getCategorizedIPS($ip_rangeId);
			expDebug::dPrint('E1>>>>>>>>>>>>>>>>>>>>>>>>categorizedIPS' . print_r($categorizedIPS, true), 5);
			if(count($categorizedIPS)>0){
				$bid=$categorizedIPS[0]->bid;
				$ipArray=explode("-", $categorizedIPS[0]->ip);
				$startIP=$ipArray[0];
				$endIP=$ipArray[1];
				expDebug::dPrint('E1>>>>>>>>>>>>>>>>>>>>>>>>startIP' . print_r($startIP, true), 5);
				expDebug::dPrint('E1>>>>>>>>>>>>>>>>>>>>>>>>endIP' . print_r($endIP, true), 5);
				if(!empty($categorizedIPS[0]->type)){
					$cTYPE=strtolower($categorizedIPS[0]->type)=="whitelist"?'0':'1';
				}
			}
		}
		
		$loggedUserId = getSltpersonUserId();
		global $theme_key;
		$uniqueId = 'add-iprange-'.$loggedUserId;

		$wrapperId = 'iprange-detail-wrapper-'.$uniqueId;
		$formName = "ipranges";

		$form['#prefix'] = '</form>'. '<div id="iprange_detail_container_sample">';
		$form['#suffix'] = '</div>';


		$form[$formName]['iprange_details']['iprange_heading_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<div id="add-iprange-details"><div id="meeting-type-ipranges"><div id="show_ip_detail_message"></div><table cellpadding="0" cellspacing="0" class="admin-addedit-iprange-details-wrapper" border="0">',
		);

		addHiddenValueToAddEditForm($form, 'iprangeID', $bid);
		$preTextValue=t('LBL1303');
		$titleAttributes['class'] = array();
		$attributes['class'] = array();
		$endip_attributes['class'][] = 'iprange-form-text'; // Add another class
		$attributes['class'][] = 'iprange-form-text'; // Add another class
		if(empty($startIP) && $preTextValue == t('LBL1303')) {
			$readingTypeClass = 'input-field-grey';
			$defaultValue = $preTextValue;
			$attributes['class'][] = $readingTypeClass; // Add another class
			$attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'",\'\',1);';
			$attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'");';
		
		}else{
			$defaultValue = $startIP;
		}
		$form[$formName]['iprange_details']['sd_first_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
		$form[$formName]['iprange_details']['start_ip'] = array(
				'#type' => 'textfield',
				'#title' => t('LBL1304').':<span class="addedit-mandatory">*</span>',
				'#prefix' => '<td colspan="2" class="admin-addedit-class-strtip-name">',
				'#suffix' => '</td>',
				'#default_value' => $defaultValue,
				'#size' => 31,
				'#maxlength' => 40,
				'#attributes' => $attributes,
		);

		$form[$formName]['iprange_details']['sd_first_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);

		$form[$formName]['iprange_details']['sd_second_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);

		$form[$formName]['iprange_details']['end_ip'] = array(
				'#type' => 'textfield',
				'#title' => t('LBL1305').':',
				'#prefix' => '<td colspan="2" class="admin-addedit-class-strtip-name">',
				'#suffix' => '</td>',
				'#default_value' => $endIP,
				'#size' => 31,
				'#maxlength' => 40,
				'#attributes' => $endip_attributes,
		);

		$form[$formName]['iprange_details']['sd_second_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);


		$form[$formName]['iprange_details']['sd_third_row_start'] = array(
				'#type' => 'markup',
				'#markup' => '<tr>',
		);
			//Using the below veriable we can find current loaded theme name
// 			global $theme_key;

			if($theme_key == 'expertusoneV2') {
				$newuiPrefix = '<div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon">';
				$newuiSuffix = '</div></div>';
			} else {
				$newuiPrefix = '';
				$newuiSuffix = '';
			}

			$timezoneAttributes['class'] = array();
			$timezoneAttributes['class'][] = 'addedit-edit-select ctrDropDown123';
			$timezoneAttributes['onblur'][] = 'this.className="addedit-edit-select ctrDropDown123";';
			$timezoneAttributes['onmousedown'][] = 'this.className="addedit-edit-select ctrDropDownClick123"';
			$timezoneAttributes['onchange'][] = 'this.className="addedit-edit-select ctrDropDown123"';
            $timezoneAttributes['onclick'][] = 'this.className="addedit-edit-select ctrDropDown123"';
			$categories['0']= t('LBL1307');
			$categories['1']= t('LBL1308');
			$form[$formName]['iprange_details']['ip_category'] = array(
					'#id' => 'ip_category',
					'#type' => 'select',
					'#prefix' => '<td class="admin-addedit-class-ip_category"><div class="crt-paint-ip_category"><div class="form-item-ip_category-list"><label>'.t('LBL1306').':</label></div>'.$newuiPrefix,
					'#suffix' => $newuiSuffix.'</div></td>',
					'#options'=> $categories,
					'#default_value' => $cTYPE,
					'#maxlength' => 12,
					'#attributes' => $timezoneAttributes,//array('Class' => 'addedit-edit-select'),
			);

		

		$form[$formName]['iprange_details']['sd_third_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</tr>',
		);

		$form[$formName]['iprange_heading_row_end'] = array(
				'#type' => 'markup',
				'#markup' => '</table></div></div>',
		);

		$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
		$cancelAttributes['data-wrapperid'] = array($wrapperId);
		$cancelAttributes['onclick'][]         = '$(".active-qtip-div").remove();$(\'#meeting-title\').html(\'\');return false;';
		$form[$formName]['cancel_content_edit_mode'] = array(
				'#type' => 'submit',
				'#value' => t('LBL109'),
				'#title' => t('LBL109'),
				'#prefix' =>  '<div class="addedit-form-cancel-container-actions admin-session-save-button admin-save-button-container1 resize-save_btn"><div class="white-btn-bg-left"></div>',
				'#suffix' =>  '<div class="white-btn-bg-right"></div>',
				'#name' => 'Cancel IPRange',
				'#submit' => array('cancelIPRangeDetails'),
				'#attributes' => $cancelAttributes,
		);


		$buttonName  = 'set-iprange';
		$setAttributes['class'][] = 'addedit-form-expertusone-throbber admin-save-button-middle-bg';
		$setAttributes['data-wrapperid'] = array($wrapperId);
		$form[$formName]['set_iprange_edit_mode'] = array(
				'#type' => 'submit',
				'#prefix' => '<div class="admin-save-button-container1"><div class="admin-save-button-left-bg"></div>',
				'#suffix' => '<div class="admin-save-button-right-bg"></div></div></div>',
				'#title' => t('LBL569'),
				'#value' => t('LBL569'),
				'#name' => $buttonName,
				'#attributes' => $setAttributes,
				'#ajax' => array(
						'wrapper' => 'iprange-admin-form',
						'callback' => 'add_ip_range_callback',
						'entity_plugin'=>'iprange',
						//'customButtonId'=>$customHiddenButtonId,
						'method' => 'replace',
						'effect' => 'none', // 'fade',
						'event'=>'click',
						'keypress' => true,
						'progress' => array(
								'type' => 'throbber',
								'message' => ''
						),
				),
		);

		return $form;
	} catch (Exception $ex) {
		watchdog_exception('add_edit_new_ip_html', $ex);
		expertusErrorThrow($ex);
	}
}
function add_ip_range_callback(&$form, &$form_state){
	try {
		
		$commands = array();
		if($form_state["storage"]["form_validate"] == true){
			$min_form_state       = array(
					'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
					're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
					'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
					'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
			);
			$form_state                                = $min_form_state;
		  // $commands[] = exp_ctools_modal_adjust();
			$builtForm            = drupal_build_form('exp_sp_administration_iprange_form', $form_state);
			$commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
		}
		
		
		$statusMsgHTML = theme('status_messages');
		
		expDebug::dPrint(' $statusMsgHTML nnan = ' . print_r($statusMsgHTML, true) , 5);
		
		if (!empty($statusMsgHTML)) {
			// Show the messages
			$commands[] = ajax_command_html(' #bubble-face-table #show_ip_detail_message', $statusMsgHTML);//ajax_command_prepend(NULL, $statusMsgHTML);
			// Set the trigger to remove the messages after a while
			if (!empty($form_state['triggering_element']['#ajax']['wrapper'])) {
				$commands[] = addedit_hide_messages($form_state['triggering_element']['#ajax']['wrapper']);
			}
			else {
				$commands[] = addedit_hide_messages();
			}
		}
				
		return array('#type' => 'ajax', '#commands' => $commands);
		
		
	} catch (Exception $ex) {
		watchdog_exception('add_ip_range_callback', $ex);
		expertusErrorThrow($ex);
	}
}
/**
 * IP range query
 */
function getCategorizedIPQuery(){
	try {
	$select = db_select('ip_ranges','ranges');
	$select->addField('ranges','bid','bid');
	$select->addField('ranges','ip','ip');
	$select->addField('ranges','type','type');
	$select->orderBy('type', 'ASC');
	expDebug::dPrintDBAPI('getCategorizedIPQuery $select->',$select);
	return $select;
	}
	catch(Exception $ex)
	{
		watchdog_exception('getCategorizedIPQuery', $ex);
		expertusErrorThrow($ex);
	}
}
function getexistip($ip_startId,$ip_endId,$iprangeID)
{
	try {
		$result="";
		expDebug::dPrint('getexistip $ip_startId-> ' .print_r($ip_startId,true),4);
		expDebug::dPrint('getexistip $ip_endId-> ' .print_r($ip_endId,true),4);
		$details = array();

		/* if(!empty($ip_startId) && (!empty($ip_endId))){
			$select="select ip,type from ip_ranges where (SUBSTRING_INDEX(ip, '-', 1) BETWEEN '$ip_startId' AND '$ip_endId') OR (SUBSTRING_INDEX(ip, '-', -1) BETWEEN '$ip_startId' AND '$ip_endId')
			OR (SUBSTRING_INDEX(ip, '-', 1) <= '$ip_startId') OR (SUBSTRING_INDEX(ip, '-', -1) < '$ip_startId')";
		}
		if (!empty($ip_startId) && empty($ip_endId)){
			$select="select ip,type from ip_ranges where (SUBSTRING_INDEX(ip, '-', 1) = '$ip_startId') or (SUBSTRING_INDEX(ip, '-', -1) = '$ip_startId') ";
		} */
		
		if(!empty($iprangeID))
		{
			if(!empty($ip_endId))
			{
				$select1="select ip,type from ip_ranges where bid not in('$iprangeID') and ((inet_aton((SUBSTRING_INDEX(ip, '-', 1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')) OR (inet_aton((SUBSTRING_INDEX(ip, '-', -1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')));";
				$result=db_query($select1);
				$number_of_rows = $result->rowCount();
				expDebug::dPrint('select ip rangrowcount'. $number_of_rows);
				    if($number_of_rows == '0')
				    {
				    	$select="select ip,type from ip_ranges where bid!='$iprangeID' and (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', -1)) >= inet_aton('$ip_startId'))";
				    	expDebug::dPrint('select ip rang11'. $select);
				    }
					else
					{
						$select="select ip,type from ip_ranges where bid!='$iprangeID' and ((inet_aton((SUBSTRING_INDEX(ip, '-', 1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')) OR (inet_aton((SUBSTRING_INDEX(ip, '-', -1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')));";
						expDebug::dPrint('select ip rang12'. $select);
					}
			}
			else
			{
				$select="select ip,type from ip_ranges where bid!='$iprangeID' and (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', -1)) >= inet_aton('$ip_startId'))";
				expDebug::dPrint('select ip rang13'. $select);
			}	
		}
		else
		{
			if(!empty($ip_endId))
			{
				
				$select1="select ip,type from ip_ranges where ((inet_aton((SUBSTRING_INDEX(ip, '-', 1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')) OR (inet_aton((SUBSTRING_INDEX(ip, '-', -1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')));";
				$result=db_query($select1);
				$number_of_rows = $result->rowCount();
				expDebug::dPrint('select ip rangrowcount'. $number_of_rows);
				if($number_of_rows == '0')
				{
					$select="select ip,type from ip_ranges where ((inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', -1)) >= inet_aton('$ip_startId')) OR (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) >= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')))";
					expDebug::dPrint('select ip rang14'. $select);
				}
				else
				{
					$select="select ip,type from ip_ranges where ((inet_aton((SUBSTRING_INDEX(ip, '-', 1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')) OR (inet_aton((SUBSTRING_INDEX(ip, '-', -1))) BETWEEN inet_aton('$ip_startId') AND inet_aton('$ip_endId')));";
					expDebug::dPrint('select ip rang15'. $select);
				}
					
			}
			else 
			{
				$select="select ip,type from ip_ranges where ((inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', -1)) >= inet_aton('$ip_startId')) OR (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) >= inet_aton('$ip_startId')) and (inet_aton(SUBSTRING_INDEX(ip, '-', 1)) <= inet_aton('$ip_startId')))";
				expDebug::dPrint('select ip rang16'. $select);
			}
		}
		expDebug::dPrint('select ip rang'. $select);
		$result=db_query($select);

		foreach($result as $record){
			$details['ip'] = $record->ip;
			$details['type'] = $record->type;
			break;
		}

		return $details;
	 }
	 catch(Exception $ex)
	 {
	 	watchdog_exception('getexistip', $ex);
	 	expertusErrorThrow($ex);
	 }
	
}
/**
 * Fetch whitelist/blacklist IP's configured
 */
function getCategorizedIPS($ip_rangeId){
	try{
		
		$result="";
		$select=getCategorizedIPQuery();
		if(!empty($ip_rangeId)){
			$select->condition('ranges.bid',$ip_rangeId,'=');
			return $result = $select->execute()->fetchAll();
		}else{
			return $result = $select->execute()->fetchAll();
		}
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getCategorizedIPS', $ex);
		expertusErrorThrow($ex);
	}
}
?>