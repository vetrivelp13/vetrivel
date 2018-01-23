<?php 
function exp_sp_administration_country_setting_display_form($js = NULL){
  try {
  	
  	// Default modalTheme is ExpertusCToolsModalTheme (see exp_sp_core_extend_ctools_ajax.js:Drupal.CTools.Modal.show())
	  $countrysetting_style = array(
	    'ctools-admin-country-setting-addedit-scroll-wrapper' => array(
	      'modalSize' => array(
	        'width' => 520,
	        'minHeight' => 280,
	       ),
	      'modalOptions' => array(
	        'top' => '1',
	      ),
	    )
	  );
	
	  drupal_add_js($countrysetting_style, 'setting');
	  
	  if ($js) {
	    $formIdList = array(array('id'    => 'exp_sp_administration_country_setting_form',
	                              'title' => (t('LBL039').' '.t('LBL563')))); //Country Settings
	    exp_ctools_modal_multi_ajaxonly_forms_display($formIdList, array());
	    
	  } else {
	    throw new Exception('exp_sp_administration_country_setting_form() : nojs');
	  }
  }
  catch (Exception $ex) {
    watchdog_exception('exp_sp_administration_country_setting_display_form', $ex);
    expertusErrorThrow($ex);
  }
}
function exp_sp_administration_country_setting_form($form, &$form_state, $renderMode, $arg) {
	try{
  global $theme_key;
  $prfixPgLeft   = '';
  $suffixPgRight = '';
  $filtersearch = '';
  if($theme_key == 'expertusoneV2'){
  	$prfixPgLeft   = '<div class="white-btn-bg-left"></div>';
  	$suffixPgRight = '<div class="white-btn-bg-right"></div>';
  	$cancelAttributes['class'][] = 'white-btn-bg-middle';
  	$filtersearch = '<div class="filter-search-start-date-right-bg"></div>';
  } 
  $entityId   = 1;
  $entityType = 'commerce';
  $formName = 'country-setting';
  $uniqueId = 'CountrySetting-'.$entityId.'-'.$entityType;
  $wrapperId = 'cs-grid-wrapper-'.$uniqueId;
  $customHiddenButtonId = 'equv-save-esign';
  
  $form[$formName] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="admin-commerce-country-setting-plan">'
                  .'<div id="'.$wrapperId.'" class="admin-datagrid-pagination admin-commerce-country-setting-datagrid-wrapper">',
    '#suffix' => '</div></div>',
  );
  
  $form[$formName]['hidden_idlist'] = array(
      '#type' => 'hidden',
      '#name' => 'hidden_idlist_'.$uniqueId,
      '#id' => 'datagrid-idlist-'.$uniqueId,
    );

  $paintMultiAction  = "<div id='search-list-title-keyword' class='search-list-keyword' style='display:block;'>";
  $paintMultiAction .= "<span id='search-dropdwn-list'>";
  $paintMultiAction .= "<input type='hidden' id='search_all_country_type-hidden' value='countryname' />";
  $paintMultiAction .= "<input type='hidden' id='countrylist-autocomplete_hidden' value='".t('LBL107')."' />";//Type Country name
  $paintMultiAction .= "<span id='select-list-dropdown' class='select-list-dropdown'>".t('LBL107')."</span>";//Country name
  $paintMultiAction .= "<a  id='admin-dropdown-arrow' class='select-list-dropdown-link ' onclick='$(\"body\").data(\"mulitselectdatagrid\").moreEnrollSearchHideShow();'>&nbsp;</a>";        							
  $paintMultiAction .= "<ul id='select-list-dropdown-list'>";
  $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreCountrySearchTypeText('".t('LBL107')."','countryname');\">".t('LBL107')."</li>";//Country Name
  $paintMultiAction .= "<li onclick=\"$('body').data('mulitselectdatagrid').moreCountrySearchTypeText('".t('LBL096')."','countrycode');\">".t('LBL096')."</li>";//County Code
  $paintMultiAction .= "</ul>";
  $paintMultiAction .= "</span></div>";
  
  $form[$formName]['country_multi_search_container'] = array(
  '#type' => 'markup',
  '#markup' => '<div class="admin_add_multi_search_container">',
  );
  $form[$formName]['more_add_countrylist_search'] = array(
  '#type' => 'markup',
  '#markup' => $paintMultiAction,
  );
  $acMenuPath = 'administration/commerce/setting/country/allcountry-autocomplete';
  
  $form[$formName]['add_countrylist_search_start'] = array(
  '#type' => 'markup',
  '#markup' => '<div class="admin_add_multi_auto_search">',
  );
    
  $autoCompleteAttributes['class'][] = 'admin_ac_input_mainform';
  $autoCompleteAttributes['onkeydown'][]  = '$("body").data("mulitselectdatagrid").multiSelectSearchBoxKeyDown(event)';
  addACFieldForMultiSelect($form[$formName], 'countrylist-autocomplete', $acMenuPath, '', '', '', array(),
                               array('entity_id'  =>  "'$entityId'", 'entity_type' => "'$entityType'", 'search_type' => "$('#search_all_country_type-hidden').val()"), false, false,
                                    t('LBL036').' '.t('LBL107'), $autoCompleteAttributes);
  
  $form[$formName]['add_countrylist_search_end'] = array(
  '#type' => 'markup',
  '#markup' => '</div>',
  );
  
  $searchlink = "<a title = '".t("LBL304")."' class='admin-pagination-search-go float-left' onclick='$(\"body\").data(\"mulitselectdatagrid\").searchDataGrid(\"edit\", \"CountrySetting\", $(\"#countrylist-autocomplete\").val(), $entityId,\"$entityType\", 0);'></a>";        							
  $form[$formName]['search_countrylist'] = array(
  '#type' => 'markup',
  '#markup' => $searchlink,
  );
  $form[$formName]['gridsection'] = array(
  '#type' => 'markup',
  '#markup' => $filtersearch.'<div class="clearBoth"></div></div><div class="clearBoth"></div>',
  );    
 
  $excludedCourseId =0;
  $form[$formName]['attachcourse_view_grid_load'] = array( 
    '#type' => 'markup',
    '#prefix' => '<div style="display:none"><img src="themes/seven/images/buttons.png" width="0" height="0" onload="$(\'body\').data(\'mulitselectdatagrid\').createLoader(\'datagrid-div-'.$uniqueId.'\'); $(\'body\').data(\'mulitselectdatagrid\').loadDataGrid(\'edit\', \'CountrySetting\', \'\', \''.$entityId.'\', \''.$entityType.'\', \''.$excludedCourseId.'\');" width="100" height="132" /></div>',
  ); 
  $form[$formName]['attachcourse_view_grid_markup'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="datagrid-div-'.$uniqueId.'" class="datagrid-country-setting-commerce"><table id="datagrid-container-'.$uniqueId.'" class="datagrid-container-common"></table><div id="pager-datagrid-'.$uniqueId.'" class="pager-datagrid-common"></div></div>',
  );
  $form[$formName]['lastposition'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="clearBoth"></div><div></div>',
  );  
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber admin-action-button-middle-bg white-btn-bg-middle';
  $cancelAttributes['onclick'][]         = 'Drupal.CTools.Modal.dismiss(); return false;';
  $cancelAttributes['data-wrapperid'] = array($wrapperId);
  
  $form[$formName]['cancel_countrysetting_edit_mode'] = array(
    '#type' => 'submit',
    '#value' => t('LBL123'), //Close
    '#title' => t('LBL123'),
    '#prefix' => '<div class="admin-datagrid-save-cancel-button-align-bottom"><div class="addedit-form-cancel-container-actions admin-save-button-container">'.$prfixPgLeft,
    '#suffix' => $suffixPgRight,
    '#name' => 'Cancel country setting',
    '#attributes' => $cancelAttributes,
  );
         
  return $form;
  }catch (Exception $ex) {
  	watchdog_exception('exp_sp_administration_country_setting_form', $ex);
  	expertusErrorThrow($ex);
  }
}


function loadCountrySetting($searchKeyword, $queryRequired = '', $excludedId = ''){
	try{
  $select = db_select('uc_countries', 'cty');
  $select->addField('cty', 'country_id', 'id');
  $select->addField('cty', 'country_name', 'name');
  $select->addField('cty', 'country_iso_code_3', 'code');
  $select->addField('cty', 'country_iso_code_2', 'code3');
  $select->addField('cty', 'version', 'version');
  $searchType = $_GET['searhType'];
  if($_GET['searhType']=='countrycode'){
   $default_text_title = t('LBL036').' '.t('LBL096');//t('Type code');
    if(!empty($searchKeyword) && $searchKeyword!=$default_text_title){
        $select->condition('cty.country_iso_code_3', '%' . db_like($searchKeyword) . '%', 'LIKE');
    }
  }else  {
   $default_text_title = t('LBL036').' '.t('LBL107');//t('Type name');
    if(!empty($searchKeyword) && $searchKeyword!=$default_text_title){
        $select->condition('cty.country_name', '%' . db_like($searchKeyword) . '%', 'LIKE');
    }
  }
  $select->addField('cty', 'version', 'version');
  
  //$select->orderBy('country_name','ASC');
  expDebug::dPrintDBAPI("Country Setting  " , $select);
  if(empty($queryRequired)){
    return $select->execute()->fetchAll();
  } else {
    return $select;
  }
  }catch (Exception $ex) {
  	watchdog_exception('loadCountrySetting', $ex);
  	expertusErrorThrow($ex);
  }
}


function getAutoCompleteCountryList() {
	try{
  $nameSubstr = $_GET['z'];
  $searchType = $_GET['search_type'];
  
  $select = db_select('uc_countries', 'cty');
  $select->addField('cty', 'country_id', 'id');
  if($searchType == 'countryname' || $searchType == ''){
    $select->addField('cty', 'country_name', 'name');
      if(!empty($nameSubstr)){
      $select->condition('cty.country_name', '%' . db_like($nameSubstr) . '%', 'LIKE');
      }
  }elseif($searchType == 'countrycode'){
      $select->addField('cty', 'country_iso_code_3', 'name');
     $select->condition('cty.country_iso_code_3', '%' . db_like($nameSubstr) . '%', 'LIKE');  	    
 }
  $select->orderBy('cty.country_id');
  $matchingCountry = $select->execute()->fetchAll();

  foreach ($matchingCountry as $country) {
    print $country->name. "\n"; 
  }
  }catch (Exception $ex) {
  	watchdog_exception('getAutoCompleteCountryList', $ex);
  	expertusErrorThrow($ex);
  }
}

function enableOrDisableCountry($countryCode, $countryId ){
  $txn = db_transaction();
  try {
    $status  = getCountryStatus($countryCode);
    //uc_country_enable(48);
    if($status < 0) {
      $updatedstatus = abs($status);
    }else{
      $updatedstatus = -$status;
    }
    $updateStmt = db_update('uc_countries');
    $updateStmt->condition('country_iso_code_3', $countryCode);
    $updateStmt->fields(array(
                        'version'      => $updatedstatus,                                                   
                        ));
    $updateStmt->execute();
   
    add_audit_trail_entry(getSltpersonUserId(), $countryId, 'Country Setting Admin', 'exp_sp_administration_commerce_countrysetting', 'enableOrDisableCountry', 'Updated Country Status',
		                                     null, $status, $updatedstatus);
	
	$returnStatus  = ($updatedstatus > 0)?t('LBL920'):t('LBL919');
	print $updatedstatus.'|'.$returnStatus;	                                     
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('update Country', $ex);
    throw $ex;
  }
  unset($txn);
}

function getCountryStatus($countryCode) {
	try{
  $select = db_select('uc_countries', 'ctry');
  $select->addField('ctry','version', 'version');
  $select->condition('country_iso_code_3',$countryCode);
  // Execute the query
  $countryStatus = $select->execute()->fetchField();
  return $countryStatus;  
  }catch (Exception $ex) {
  	watchdog_exception('getCountryStatus', $ex);
  	expertusErrorThrow($ex);
  }
}

?>