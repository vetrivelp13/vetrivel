<?php
function exp_sp_administration_currency_display_addedit_form($js = NULL) {
	try {
		  global $theme_key;
			$js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
			drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);

		// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
		$currency_style = array(
				'ctools-admin-currency-style' => array(

						'modalSize' => array(
													  'width' => 300,
														'class' =>'currency-module',
								),
								)
		);
		drupal_add_js($currency_style, 'setting');

		if ($js) {
			$argsList = array();

			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_currency_addedit_form',
							  'title' => t('LBL101')) //Currency
			);
			exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_currency_display_addedit_form');
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_currency_display_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_currency_addedit_form($form, &$form_state){
	try{
		global $theme_key,$user;
		$wrapperId = 'currency-detail-wrapper';
		$form = newAddEditFormWithCancelAndSave($wrapperId);

		//$currencyList = getCurrencyAttributes();
		$currencybasetype=getCurrencyBaseType();
	 	$form[$formName]['currency_table_start'] = array(
        '#type' => 'markup',
     	);
	 	$prefixBgLeft='';$suffixBgRight='';
	 	if($theme_key == 'expertusoneV2'){
	 		$style_label = 'padding: 0 4px;';
	 		$style_th="padding: 0 0 0 3px";
	 		$prefixBgLeft   = '<div class="white-btn-bg-left"></div>';
	 		$suffixBgRight = '<div class="white-btn-bg-right"></div>';
	 	}else{
	 		$style = 'padding-left: 0';
	 	}
	 	$addButtonId = 'currency-add-button';
	 	$currencycodearray=array();
		$currencybasetype_cnt = count($currencybasetype);
		expDebug::dPrint('$$$currencybasetype = ' . print_r($currencybasetype, true),4);
		expDebug::dPrint('$$$currencybasetype_cnt = ' . print_r($currencybasetype_cnt, true),4);
		if($currencybasetype_cnt>0){
	 	$tablelist='<div id="tableholder"><div id="multi_currency_setup_div"><table width="100px" class="currency-table-heading" cellpadding="4" cellspacing="0" border="0"><th width="20%" style="min-width: 60px; line-height: 25px; padding: 0px 8px;'.$style_label.'" class="currency-title-heading currency-header heading-background-class-title">'.t('LBL101').'</th>';
	 	$symbol_arr = array();
		 	foreach($currencybasetype as $key => $val){
		 		$attr2sym=getCurrencySymbol($val->base_type);
		 		$tablelist.='<th width="" style="'.$style_th.'" class="row-'.$val->base_type.' currency-title-heading currency-header heading-background-class-title">'.$val->base_type.' <span class="currency-override-bold">'.$attr2sym['attr2'].'</span></th>';
		 		$currencycodearray[]=$val->base_type;
		 		$symbol_arr[$val->base_type] = $attr2sym['attr2'];
		 	}
		 	$tablelist.='<th width="5%" style="padding-left: 0px; text-align:center;" class="currency-title-heading currency-header heading-background-class-title"></th>';
	 	}else{
	 		$tablelist = "<div id=\"tableholder\"><div id=\"multi_currency_setup_div\"><div width=\"130px\" height=\"100px\" class=\"currency-table-heading admin-empty-text-msg\" style=\"border:0px;\">".t('MSG746')."</div>";
	 	}
     $form[$formName]['currency_fieldset'] = array(
        '#type' => 'markup',
        '#prefix' => $tablelist,
        '#suffix' => '</div></div>',
     );

    $i=0;
    $j=0;
    $tablebodyrow='';
    expDebug::dPrint('$$currencycodearray = ' . print_r($currencycodearray, true));
    $fetchtype="rfetch";
    foreach($currencybasetype as $key1 => $val1){
    	$tdattr2sym=getCurrencySymbol($val1->base_type);
    	expDebug::dPrint('$currencyList = simbu= '.$i);
    	$class='odd-list-class';
    	if($j==1){
    		$class='even-list-class';
    		$j=0;
    	}else{
    		$j=1;
    	}
    	$tablebodyrow.='<tr class="rowtbody row-'.$val1->base_type.' '.$class.'"><td class="check-attributes"><span class="vtip" title="'.sanitize_data(t($val1->base_type)).'"> 1 '.$val1->base_type.' <span class="currency-override-bold">'.$tdattr2sym['attr2'].'</span></span></td>';
    	for($s=$i;$s<$i+1;$s++){
    		for($k=0;$k<count($currencycodearray);$k++){
    			$rate = checkCurrencyCombination($currencycodearray[$s],$currencycodearray[$k],$fetchtype);
    			if(!empty($rate)){
    				$vtiprate=$rate;
    			}else{
    				$vtiprate=sprintf ("%.5f",$rate);
    			}
    			expDebug::dPrint('$currencyList = $tablebodyrow ' . $rate);    			
    			$tablebodyrow.='<td class="row-'.$currencycodearray[$k].' check-attributes"><span class="vtip" title="'.sanitize_data(t($vtiprate)).'"> '.sprintf ("%.5f",$rate).'</span></td>';
    		}
    	}
    	$tablebodyrow.='<td class="row-'.$val1->base_type.' Checked-column">';
    	$check_currency_item = check_currency_item($val1->base_type);
    	expDebug::dPrint('$$check_currency_item ' . $check_currency_item);
    	if($check_currency_item > 0){
    		$tablebodyrow.='<span class="cremove"><a href="javascript:void(0)" class="narrow-search-results-item-action-list-btn enable-delete-icon disable-delete-link">'.t("LBL286").'</a></span>';
    	}else{
    		$tablebodyrow.='<span class="cremove"><a href="javascript:void(0)" class="narrow-search-results-item-action-list-btn enable-delete-icon vtip" title="'.t("LBL286").'" onclick= "removeCurrencyPopup(\'' . $val1->base_type . '\',\'' . $tdattr2sym['attr2']. '\')">'.t("LBL286").'</a></span>';
    	}

    	$tablebodyrow.='</td></tr>';
    	$i++;
    }

    expDebug::dPrint('$currencyList = $tablebodyrow ' . print_r($tablebodyrow, true));
    $form[$formName]['currency_table_row'] = array(
    		'#type' => 'markup',
    		'#prefix' => $tablebodyrow,
    );
    if($currencybasetype_cnt>0){
    $form[$formName]['currency_table_end'] = array(
        '#type' => 'markup',
        '#suffix' => '</table></div>',

     );
    }
    $qtipOptAccessObj      = "{'url':'administration/sitesetup/config/currency/add/nojs','entityId':0,
	              'entityType':'addcurrency',
	              'popupDispId':'qtip_currency_disp_add_currency',
	              'wid':325,
	              'heg':'100',
	              'postype':'topleft',
	              'poslwid':'230',
	              'linkid':'visible-group-add-currency',
	              'scrollid':'admin-add-scroll'}";

    $paintMultiAction .='<div class="addedit-form-cancel-and-save-actions-row"';
    if($currencybasetype_cnt<=0){
    	$paintMultiAction .=' style="width:95%;"';
    }
    if($user->language=='en-us'){
    	$vtip_eng_class="en_vtip_currency";
    }else{
    	$vtip_eng_class="";
    }
    $paintMultiAction .='><div class="addedit-form-cancel-container-actions"><div class="admin-save-button-container" id="qtip_currency_disp_add_currency"><div class="admin-save-button-left-bg"></div>';
    $paintMultiAction .= "<a id='visible-group-add-currency' class='tab-title addedit-edit-group-save-button admin-currency-save-button-middle-bg ".$vtip_eng_class." addedit-form-expertusone-throbber form-submit' title='".t('LBL287')." ".t('LBL101')."' onclick = \"callVisibility(".$qtipOptAccessObj.");\">".t('LBL287')." ".t('LBL101')."</a><span id='visible-popup-0' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>".
    		'<div class="admin-save-button-right-bg"></div></div><div class="clearBoth"></div></div><div class="clearBoth"></div></div>';


    $form[$formName]['currency_button'] = array(
    		'#type' => 'markup',
    		'#markup' => $paintMultiAction,
    );

    $hidewrapperId         = 'currency-detail-wrapper';
    $hideCloseButton = 'add_currency_cancel_button';
    $hideCloseAttributes['class'] = array('addedit-edit-' . $hideCloseButton);
    $hideCloseAttributes['class'][] = 'addedit-form-expertusone-throbber';
    $hideCloseAttributes['data-wrapperid'] = array($hidewrapperId);
    //$hideCloseAttributes['onclick'][] = 'remove_messages("class"); return false;';
    $classIdQtip = !empty($classId)?$classId:0;
    $hideCloseAttributes['onclick'][] = 'remove_messages("class");';

    $form[$hideCloseButton] = array(
    		'#type' => 'submit',
    		'#id' => $hideCloseButton,
    		'#prefix' => '<div style="display:none">',
    		'#suffix' => '</div>',
    		'#value' => '',
    		'#name' => 'hiddencancel',
    		'#attributes' => $hideCloseAttributes,
    		'#ajax' => array(
    				'wrapper' => $hidewrapperId,
    				'callback' => 'addCurrencyCancelRepaintForm',
    				'method' => 'replace',
    				'effect' => 'none',
    				'event'=>'click',
    				'keypress' => true,
    				'progress' => array(
    						'type' => 'throbber',
    						'message' => ''
    				),
    		)
    );

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_currency_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function addCurrencyCancelRepaintForm($form, &$form_state) {
	try {
		$storageCode = $form_state['storage']['success-status'];
		expDebug::dPrint('addCurrencyCancelRepaintForm storageCode' . $storageCode);
		$min_form_state       = array(
				'ajax'         => TRUE, // As I have merged code from ctools_modal_form_render() in this function, this may no longer be needed. Retaining as in ctools_modal_form_wrapper(), and because form_state is passed around by reference.
				're_render'    => TRUE, // Doesn't seem of being used by ctools or drupal forms API. Retaining as in ctools_modal_form_wrapper()
				'no_redirect'  => TRUE, // Prevent any redirection, required forms are built in ajax callback
				'build_info'   => array('args' => array('ajax')) // The the form builder function expects the first argument to be ajax or nojs.
		);
		$form_state = $min_form_state;
		$commands = array();
		if ($storageCode == 1) {
			$commands[] = exp_ctools_modal_adjust();
			$commands[] = ajax_command_invoke(NULL, 'dynamicWidthHeightAdd');
			$builtForm            = drupal_build_form('exp_sp_administration_currency_addedit_form', $form_state);
			$commands[]           = ajax_command_replace(NULL, drupal_render($builtForm));
			$statusMsgHTML = theme('status_messages');
			expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
			if (!empty($statusMsgHTML)) {
				// Show the messages
				$commands[] = ajax_command_html('#modalContent #show_expertus_message', $statusMsgHTML);
			}
		} else {
			$statusMsgHTML = theme('status_messages');
			expDebug::dPrint(' $statusMsgHTML = ' . print_r($statusMsgHTML, true) , 4);
			if (!empty($statusMsgHTML)) {
				// Show the messages
				$commands[] = ajax_command_html('#bubble-face-table #show_expertus_message', $statusMsgHTML);
			}
		}
		return array('#type' => 'ajax', '#commands' => $commands);

	} catch (Exception $ex) {
		watchdog_exception('addCurrencyCancelRepaintForm', $ex);
		expertusErrorThrow($ex);
	}

}
/**
 * Fetch Currency attriutes from slt_currency_conversion_details table
 */
function getCurrencyAttributes(){
	try{
		$select = db_select('slt_currency_conversion_details','currency');
		$select->addField('currency','id','id');
		$select->addField('currency','base_type','base_type');
		$select->addField('currency','convert_type','convert_type');
		$select->addField('currency','rate','rate');
		$select->orderBy('base_type', 'ASC');
		$select->orderBy('convert_type', 'ASC');
		expDebug::dPrintDBAPI('getCurrencyAttributes $select->',$select);
		$result = $select->execute()->fetchAll();
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getCurrencyAttributes', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Fetch Currency Base Type from slt_currency_conversion_details table
 */
function getCurrencyBaseType(){
	try{
		$select=db_query("SELECT DISTINCT currency.base_type AS base_type FROM slt_currency_conversion_details currency LEFT JOIN slt_profile_list_items profList on (profList.attr1=base_type) where profList.is_active='Y' AND profList.attr3='Y' order by base_type ASC");
		$result = $select->fetchAll();
		return $result;
	}
	catch (Exception $ex) {
		watchdog_exception('getCurrencyBaseType', $ex);
		expertusErrorThrow($ex);
	}
}



/* Add Currency */
function exp_sp_administration_addcurrency_display_addedit_form($js = NULL) {
	try {
		expDebug::dPrint('exp_sp_administration_addcurrency_display_addedit_form');
	 	$form_state = array(
	    'no_redirect'  => TRUE,
	    'build_info'   => array('args' => array('ajax')),
	  );
	  $formId                 = 'add_edit_admin_currency_list';
	  $builtForm              = drupal_build_form($formId, $form_state);
	  $renderedTabContentHTML = drupal_render($builtForm);
	  $drupalSettings         = getDrupalSettings();

	  $response       = array(
	    'drupal_settings'     => $drupalSettings,
	    'render_content_main' => $renderedTabContentHTML,
	  );


	  echo drupal_json_output($response);
	  exit;
  } catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_addcurrency_display_addedit_form', $ex);
  	expertusErrorThrow($ex);
  }
}

function add_edit_admin_currency_list($form,&$form_state){
	try {
		$loggedUserId = getSltpersonUserId();
		global $theme_key;
		$uniqueId = 'add-currency-'.$loggedUserId;
		$formName= 'add-currency';

		// Form wrapper id
        $formWrapperId = 'currency-detail-wrapper';
		$dynamicwrapperid="currency-addedit-form".$loggedUserId;
		$popuprenderPermId = "popup_container_qtip_currency_disp_".$loggedUserId;
		$popupDisplayId				 = "qtipCurrencyqtip_visible_disp_".$loggedUserId;
		$form = currencyNewAddEditFormWithCancelAndSave($dynamicwrapperid);

		//All available profilecurrencyList
		$profilecurrencyList  = fetchCurrencyList($loggedUserId);
		//expDebug::dprint('Currency list '.print_r($profilecurrencyList,true),5);

		$form[$formName] = array(
				'#type' => 'markup',
				'#prefix' => '<div id="admin-add-currecny" class="add-currency-class admin-add-currecny" >',//<div id="'.$wrapperId.'" class="admin-datagrid-pagination '.$setminHight.'">',
				'#suffix' => '</div>',
		);

		if(!empty($profilecurrencyList)){

			expDebug::dprint('$profilecurrencyList NOT Empty ');
			$form[$formName]['profile-currency_fieldset'] = array(
					'#type' => 'markup',					//
					'#prefix' => '<table width="283px" cellpadding="4" cellspacing="0" border="0" id="permission-table"><tr class="currency-header-tr">
	     		<td width="240px" class="adduser-title-heading user-list-header adduser-title-heading-currency certificate-column currency-theader-first">'.t('LBL101').'</td>	     		
	     		<td width="40px" class="adduser-title-heading user-list-header adduser-title-heading-currency certificate-column"><div class="currency-muliselect checkbox-unselected">
    					<input type="checkbox" id="currency_select" class="multiselect-selectall" onclick="Currencycheckedall(this);" value="" /></div></td>',
					'#suffix' => '</tr></table>',
			);
			$form[$formName]['profile-currency-add-start'] = array(
					'#type' => 'markup',
					'#prefix' => '<div id="admin-add-scroll" class="admin-add-class">',

			);
			$form[$formName]['profile_currency_td_start'] = array(
					'#type' => 'markup',
					'#prefix' =>'<table width="280px" cellpadding="4" cellspacing="0" border="0" id="currency-td-table"><tr class="currency-header-td">',
					'#suffix' => '</tr>',
			);

			expDebug::dprint('$profilecurrencyList NOT Empty -1 ');

			$i=0;
			$j=0;
			$k=0;


			foreach($profilecurrencyList as $key => $val){
				expDebug::dprint('$profilecurrencyList NOT Empty $i= '.$i);
				$disstyle = ($theme_key=='expertusoneV2')? 'display:none;cursor:default;' : 'cursor:default;';
				$checkedsubattr = '';

				$class='odd-list-class';
				if($j==1){
					$class='even-list-class';
					$j=0;
				}else{
					$j=1;
				}
				
					$k++;
					$markup = '<tr width="100%" class="'.$class.'">
	      			<td width="250px" class="user-list-detail certificate-column">
	      				<span class="currency-col-container vtip" title="'.sanitize_data(t($val->currency_name)).'"><span class="currency-code">'.$val->currency_code.'</span><span class="currency-symbol">'.$val->currency_symbol.'</span><span class="currency-name">'.$val->currency_name.'</span></span></td>		      	

	      			<td width="50px" class="Checked-column"><div class="multichk checkbox-unselected">
	      					<input type="checkbox" id = "add_currency_'.$val->currency_code.'" class = "attach-group-list" onclick="checkboxSelectedUnselectedCurrency(this);" name="attach_attributes'.$uniqueId.'[]" value="'.$val->currency_code.'"/></div></td>';

					$form[$formName]['profile_currency_table_row_head'.$i] = array(
							'#type' => 'markup',
							'#prefix' => $markup,
							'#suffix' => '</tr>',
					);

				$i++;
			}
			expDebug::dprint('$profilecurrencyList NOT Empty OUT');

			$form[$formName]['profile_currency_td_end'] = array(
					'#type' => 'markup',
					'#suffix' => '</table>',
			);
			$form[$formName]['profile-currency-add-end'] = array(
					'#type' => 'markup',
					'#suffix' => '</div>',
			);
			expDebug::dprint('$profilecurrencyList NOT Empty OVER');
			expAdmincurrencyAddEditFormClose($form, $formWrapperId, 3);
		} else {

			$form[$formName]['no-records-msg'] = array(
					'#type' => 'markup',
					'#markup' => '<div class="no-records-msg">'.t('MSG746').'</div>'
			);
		}
		// Add action buttons and close the form

		return $form;
	}catch (Exception $ex) {
	  	watchdog_exception('add_edit_admin_currency_list', $ex);
	  	expertusErrorThrow($ex);
  }
	}

	/**
	 * Adds close and save buttons to the cancellation policy addedit form and closes the form
	 */
	function expAdmincurrencyAddEditFormClose(&$form, $wrapperId, $tabIndex) {
		try {
			// Button ids
			global $theme_key,$user;
			$saveButtonId = 'addedit-currency-save-button';
			$cancelButtonId = 'addedit-currency-cancel-button';
			expDebug::dprint('expAdmincurrencyAddEditFormClose');

			// Add the Cancel button
			// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
			// each time the form is re-built.
			$cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

			// Using this class to applying bg color to the save button
			$cancelAttributes['class'][] = 'admin-action-button-middle-bg';

			// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
			$cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
			// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
			$cancelAttributes['data-wrapperid'] = array($wrapperId);

			// A common class for all addedit form cancel button, for styling to appear as a link.
			$cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

			$cancelAttributes['onclick'][] = '$("#visible-popup-0").html(""); return false;';
			$cancelAttributes['tabindex'][] = $tabIndex + 1;
			$prfixPgLeft = '';
			$suffixPgRight = '';
			if($theme_key == 'expertusoneV2'){
				$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
				$suffixPgRight = '<div class="white-btn-bg-right"></div>';
				$cancelAttributes['class'][] = 'white-btn-bg-middle';
			}
			$form[$cancelButtonId] = array(
					'#type' => 'button',
					'#prefix' => '<div style="clear:both;"></div>' .
					'<div>' .
					'<div class="pop-addedit-form-cancel-container-actions">'. $prfixPgLeft,
					'#suffix' => $suffixPgRight,
					'#value' => t('LBL123'), //Close
					'#name' => $cancelButtonId,
					'#attributes' => $cancelAttributes
			);

			// Add the Save button
			$ajaxCallbackSave = 'addCurrencyCancelRepaintForm';

			// We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
			// each time the form is re-built.
			$saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

			// Using this class to applying bg color to the save button
			if($user->language=='en-us'){
				$saveAttributes['class'][] = 'en_vtip_currency';
			}
			$saveAttributes['class'][] = 'admin-currency-save-button-middle-bg';
			
			$saveAttributes['tabindex'][] = $tabIndex;
			// A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
			$saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
			// Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
			$saveAttributes['data-wrapperid'] = array($wrapperId);
			//$saveAttributes['onclick'][] = 'simbu()';
			$form[$saveButtonId] = array(
					'#type' => 'submit',
					'#prefix' => '<div class="admin-save-button-container">' .
					'<div class="admin-save-button-left-bg"></div>',
					'#suffix' =>   '<div class="admin-save-button-right-bg"></div>' .
					'</div>' .
					'<div class="clearBoth"></div>' .
					'</div>' . // see #prefix of close button above
					'<div class="clearBoth"></div>' .
					'</div>', // see #prefix of close button above
					'#value' => t('LBL287'), // Save
					'#name' => $saveButtonId,
					'#attributes' => $saveAttributes,
					'#ajax' => array(
							'wrapper' => $wrapperId,
							'callback' => $ajaxCallbackSave,
							'method' => 'replace',
							'effect' => 'none', // 'fade',
							'event'=>'click',
							'keypress' => true,
							'progress' => array(
									'type' => 'throbber',
									'message' => ''
							),
					)
			);
		}
		catch (Exception $ex) {
			watchdog_exception('expAdmincurrencyAddEditFormClose', $ex);
			expertusErrorThrow($ex);
		}
	}


	/*
	 * currencyNewAddEditFormWithCancelAndSave() - Use this function to create your addedit form having Cancel and Save buttons at the bottom
	*/
	function currencyNewAddEditFormWithCancelAndSave($wrapperId = 'addedit-form-wrapper', $miscAction = array()) {
		try {
			$form = array();
			$form['#prefix'] = '</form>'  .'<div id="' . $wrapperId . '" class="addedit-form-wrapper">';
			$form['#suffix'] = '</div>';

			// Add a class to the form so that we can apply custom style (margin, padding, etc).
			$form['#attributes'] = array('class' => array('addedit-form'));
			$form['#attributes']['autocomplete'] = 'off';
			$form['#tree'] = TRUE; // Show full path of values (hierarchical values)

			$form['addedit-left-col-start'] = array(
					'#type' => 'markup',
					'#markup' => '<div>',
			);


			return $form;
		} catch (Exception $ex) {
			watchdog_exception('currencyNewAddEditFormWithCancelAndSave', $ex);
			expertusErrorThrow($ex);
		}
	}

function fetchCurrencyList($cId){
		try {
			expDebug::dPrint('fetch Profile Currency',4);
			$select = db_query("SELECT
		  lst.name currency_name,
		  lst.attr1 currency_code,
		  lst.attr2 currency_symbol,
		  lst.attr3 status
		from
		  slt_profile_list_items lst
		where
			lst.code LIKE 'cre_sys_crn_%'
			and lst.is_active = 'Y'
			and lst.attr3 != 'Y'
		    order by lst.attr1"
			);
			expDebug::dPrintDBAPI('list of Profile Currency' ,   $select);
			$result = $select->fetchAll();
			return $result;
		}catch(Exception $ex){
			watchdog_exception('fetchCurrencyList', $ex);
			expertusErrorThrow($ex);
		}
	}

	function add_edit_admin_currency_list_validate($form, &$form_state) {
		$form_state['storage']['success-status']= true;
		$selectedCurrencyList = $form_state['input']['attach_attributesadd-currency-1'];
		if (count($selectedCurrencyList) < 1) {
			form_set_error('profile_currency_table_row_head0', t('ERR106'));
			$form_state['storage']['success-status']= false;
		}
	}
	
	function add_edit_admin_currency_list_submit ($form, &$form_state){
		try {
			expDebug::dPrint(' $add_edit_admin_currency_list_submit '.$loggedUserId,4);
			updateCurrencyList($form_state);
			$form_state['rebuild'] = TRUE;
		} catch (Exception $ex) {
			watchdog_exception('add_edit_admin_currency_list_submit', $ex);
			expertusErrorThrow($ex);
		}

	}

	/*
	 * updateCurrencyList() - Updated the updateCurrencyList
	*/
	function updateCurrencyList($form_state){		
		try{
			$loggedUserId = getSltpersonUserId();
			$selectedCurrencyList = $form_state['input']['attach_attributesadd-currency-1'];
			$existingcode=fetchExistingCurrencyCode();
			expDebug::dprint('$existingcode '.print_r($existingcode,true),5);
			$merageallcode = array_unique(array_merge($existingcode,$selectedCurrencyList));
			expDebug::dprint('DISTINCT lst.base_type '.print_r($merageallcode,true),5);
			$notexistcurrency = array();
			$failurecurrency = array();
			$success_currency_code = array();

			if (module_exists('exp_sp_callout') && calloutIsActive() &&  calloutEntityIsActive('cme_crn_upd')) {
				expDebug::dprint('$exp_sp_callout Application');
				$rateCallout = getCalloutEntityInfo('cme_crn_upd');
				$ratecallouturl=$rateCallout['file'];
				require_once DRUPAL_ROOT. $ratecallouturl;
				processCalloutCurrencyRateRequest('application',$merageallcode);
			}else{
				$access_token=currencyapiAuthorization();
				$total_currency_cnt=count($merageallcode);
				/* Insert Currency combination in slt_currency_conversion_details table */
			for($i=0;$i<$total_currency_cnt;$i++){
				for($j=0;$j<$total_currency_cnt;$j++){
					$codematchexist=checkCurrencyCombination($merageallcode[$i],$merageallcode[$j],'rcount');
					expDebug::dprint('$codematchexist sss '.$merageallcode[$i]."---".$merageallcode[$j]);
					expDebug::dprint('$codematchexist '.$codematchexist);
					if($codematchexist=='0'){
					$insertstmt = db_insert('slt_currency_conversion_details');
					$fields = array(
							'base_type'         => $merageallcode[$i],
							'convert_type'      => $merageallcode[$j],
							'created_by'        => $loggedUserId,
							'created_on'        => date('Y-m-d H:i:s'),
							'updated_by'        => $loggedUserId,
							'updated_on'        => date('Y-m-d H:i:s')
					);
					$insertstmt->fields($fields);
					expDebug::dPrintDBAPI('list of slt_currency_conversion_details record ', $insertstmt);
					$insertstmt->execute();
				}
				}
			} /* Insert Currency combination in slt_currency_conversion_details table */
			
			/* Currency Rate Update */
			$sel = "SELECT DISTINCT base_type FROM slt_currency_conversion_details ORDER BY base_type ASC";
			$query = db_query($sel);
			$baseresult = $query->fetchCol();
			$to_count=count($baseresult);
			expDebug::dPrint('updateCurrencyList fetchCol = ' .$to_count .' '. print_r($baseresult, true), 5);
			$to_implode=implode(",",$baseresult);
			expDebug::dPrint('updateCurrencyList fetchCol Implode = ' . print_r($to_implode, true), 5);
			
			foreach ($baseresult as $basetype){
				$conversionapiresult = rateConversionAPI($basetype,$baseresult,$access_token);
				$jsonresult = json_decode($conversionapiresult);
				expDebug::dPrint('updateCurrencyList fetchCol Result = ' . print_r($jsonresult, true), 5);
				$base_currency=$jsonresult->base_currency;
				expDebug::dPrint('updateCurrencyList fetchCol $basetype = ' . $basetype.'---'.$base_currency, 5);
				$api_response_status=$jsonresult->status;
				if($api_response_status=="Success"){				
				$restotal=$jsonresult->total;	
				$result1=$jsonresult->result;
				if($restotal > 0){
				for ($k=0;$k<$restotal;$k++){
					$convert_type = $result1[$k]->convert_type;
					$rate = $result1[$k]->rate;
					$select = db_update('slt_currency_conversion_details');
					$select->fields(array('rate'=>$rate,'updated_by'=>$loggedUserId,'updated_on'=>date('Y-m-d H:i:s')));
					$select->condition('base_type', $base_currency, '=');
					$select->condition('convert_type', $convert_type, '=');
					expDebug::dPrintDBAPI('multicurrencyConversionUpdate Code', $select);
					$select->execute();
				}
				/* Update slt_profile_list_items table attr3 value */
				$select = db_update('slt_profile_list_items');
				$select->fields(array('attr3'=>'Y'));
				$select->condition('attr1', $base_currency, '=');
				expDebug::dPrintDBAPI('list of updateCurrencyList Code', $select);
				$select->execute();		
				$success_currency_code[] = $base_currency;	
				/* Update slt_profile_list_items table attr3 value */
				}else{
					// API Response Currency does not exist
					expDebug::dPrint('updateCurrencyList $$$selectedCurrencyList = ' . print_r($selectedCurrencyList, true), 5);
					if(in_array($base_currency, $selectedCurrencyList)){
						expDebug::dPrint('updateCurrencyList fetchCol DEL Query = ' ."delete FROM slt_currency_conversion_details where base_type LIKE '".$base_currency."' OR convert_type LIKE '".$base_currency."'", 5);
						db_query("delete FROM slt_currency_conversion_details where base_type LIKE '".$base_currency."' OR convert_type LIKE '".$base_currency."'");
						/* Audit trail log */
						$objectId = get_slt_profile_entity_id($base_currency);
						$msgsave = $base_currency.'-Currency does not exist in cc server - Application';
						add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_administration_config_currency', 'updateCurrencyList', $msgsave,$loggedUserId,'N','N');
						/* Audit trail log */
						$notexistcurrency[]=$base_currency;		 
					}			
					//drupal_set_message('Currency Not added-'.$base_currency);
				}
			}else{				
				// API Response fail
				expDebug::dPrint('updateCurrencyList $API Response fail = ' . $basetype.'---'.$base_currency, 5);
				if(in_array($basetype, $selectedCurrencyList)){
					expDebug::dPrint('updateCurrencyList API Response fail DEL Query = ' ."delete FROM slt_currency_conversion_details where base_type LIKE '".$basetype."' OR convert_type LIKE '".$basetype."'", 5);
					db_query("delete FROM slt_currency_conversion_details where base_type LIKE '".$basetype."' OR convert_type LIKE '".$basetype."'");
					$objectId = get_slt_profile_entity_id($basetype);
					$msgsave = $basetype.'-CC Server API Response fail - Application';
					add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_administration_config_currency', 'updateCurrencyList', $msgsave,$loggedUserId,'N','N');
				}				
				$failurecurrency[]=$basetype;					
				expDebug::dPrint('updateCurrencyList $jsonresult->message ccfail '. $jsonresult->message);
				
			}
			}
			/* Currency Rate Update*/		
			/* Update Audit trail log */
			expDebug::dPrint('updateCurrencyList $$$success_currency_code = ' . print_r($success_currency_code, true), 5);
			foreach ($success_currency_code as $chkedcurrency){
				expDebug::dPrint('updateCurrencyList $chkedcurrency = ' . $chkedcurrency, 5);	
				$existingchkcode=fetchExistingCurrencyCode();
				expDebug::dPrint('updateCurrencyList $$existingchkcode = ' . print_r($existingchkcode, true), 5);
				if (in_array($chkedcurrency, $selectedCurrencyList)){
					$objectId = get_slt_profile_entity_id($chkedcurrency);
					$msgsave=$chkedcurrency.'-Insert Currency conversion rate - Application';
					add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_administration_config_currency', 'updateCurrencyList', $msgsave,$loggedUserId,'N','Y');
			    }
			}
			expDebug::dPrint('currencyConversionStatusMail $$notexistcurrency = ' . count($notexistcurrency).'---'.count($failurecurrency), 5);
			if(count($notexistcurrency)>0) { currencyConversionStatusMail('ccmissing',$api_response_status,'empty',$notexistcurrency); } // Mail - Missing Currency
			if(count($failurecurrency)>0) { currencyConversionStatusMail('ccfail',$api_response_status,$jsonresult->message,$failurecurrency); }// Mail - API Response Fail					
			}

		} catch (Exception $ex) {
			watchdog_exception('updateCurrencyList', $ex);
			throw $ex;
		}
	}
	/*
	 * Fetch Existing Currency Code form slt_currency_conversion_details table
	 */
	function fetchExistingCurrencyCode(){
		$select = db_query("SELECT DISTINCT lst.base_type from slt_currency_conversion_details lst");
		expDebug::dprint('rowCount lst.base_type '.$select->rowCount());
		$existingcode = array();
		while ($n = $select->fetchAssoc()) {
			$existingcode[] = $n['base_type'];
		}
		return $existingcode;
	}
	/*
	 * Check if currency base_type and convert_type exist or not in slt_currency_conversion_details table
	 */
  function checkCurrencyCombination($basetype,$converttype,$type){
    $codematchexist = db_query("SELECT rate FROM slt_currency_conversion_details where base_type='$basetype' AND convert_type='$converttype'");
    expDebug::dprint('checkCurrencyCombination return '.$type);
    if($type=='rcount'){
    	expDebug::dprint('checkCurrencyCombination return rcount '.$type);
    	return $codematchexist->rowCount();
    }elseif($type=='rfetch'){
    	expDebug::dprint('checkCurrencyCombination return rfetch '.$type);
    	$n = $codematchexist->fetchAssoc();
  		$retval = $n['rate'];
  		return $retval;
    }
  }

  
  function check_currency_item($code){
  	expDebug::dprint('exp_sp_administration_removecurrency'.$code);
  	$selectcount = db_query("select ((SELECT count(*) FROM slt_course_class WHERE status IN ('lrn_cls_sts_atv','lrn_cls_sts_itv') AND currency_type ='$code') + (SELECT count(*) FROM slt_program WHERE status IN ('lrn_lpn_sts_atv','lrn_lpn_sts_itv') AND currency_type ='$code') + (SELECT count(*) FROM slt_person WHERE preferred_currency ='$code')) as cnt");
  	return $selectcount->fetchfield();
  }


  function exp_sp_administration_removecurrency($code){
  	$loggedUserId = getSltpersonUserId();
  	expDebug::dprint('exp_sp_administration_removecurrency'.$code);
  	$select = "DELETE FROM slt_currency_conversion_details where '$code' IN(base_type,convert_type)";
  	if(db_query($select)){
  		$select1 = db_update('slt_profile_list_items');
  		$select1->fields(array('attr3'=>''));
  		$select1->condition('attr1', $code, '=');
  		$select1->condition('code', 'cre_sys_crn_%','LIKE');
  		expDebug::dPrintDBAPI('exp_sp_administration_removecurrency of updateCurrencyList Code', $select1);
  		$select1->execute();
  		$objectId = get_slt_profile_entity_id($code);
  		add_audit_trail_entry($loggedUserId, $objectId, 'cre_sys_crn', 'exp_sp_administration_config_currency', 'exp_sp_administration_removecurrency', 'Remove Currency',$loggedUserId,'Y','N');
  		echo "success";
  	}else{
  		echo "failed";
  	}
  }

?>