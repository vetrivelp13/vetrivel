<?php
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration.inc";
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

/**
 * User Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 14-Dec-2011
 */

/*
 * getUserTabContentType() - Set the result type wheather it's narrow search or some other. If narrow search then define the narrow_search.
 * 								         - If tab_content_type is narrow_search then search filters, Jggrid, sortbar, etc. will be initialised.
 */

function getUserTabContentType(){
	try{
  $jsObjectInfo['tab_content_type'] = 'narrow_search';
  return $jsObjectInfo;
  } catch (Exception $ex) {
  	watchdog_exception('getUserTabContentType', $ex);
  	expertusErrorThrow($ex);
  }
}
//#custom_attribute_0078975 - Implemented for user Feed CSV to Custom Attribute
function exp_sp_userfeed_csv()
{
    try{
    $hdr_arr = array("first_name","last_name","user_name","password","status","email","contact_no","addr1","addr2","city","country","state","zip_code","timezone","preferred_language","mobile_no","preferred_currency","org_number","manager_username","empl_type_code","employee_no","dept_code","job_title_code","job_role_code","user_type","is_manager","is_instructor","other_managers","other_org_number","sms_alert","hire_date","is_mobileadmin","webex_name","webex_pass","groups");
    if(module_exists('exp_sp_administration_customattribute'))
    {
    $sel = db_select ( "slt_custom_attr", "attr" );
    $sel->join ( "slt_custom_attr_mapping", "map", "map.cattr_id = attr.id and attr.status='cre_cattr_sts_atv' and map.entity_type = 'cre_usr' and map.entity_screen_opt = 1" );
    
    $sel->addExpression("LOWER(REPLACE(attr.cattr_name,' ','_'))", "title");
    expDebug::dPrintDBAPI ( "fetchcount------>", $sel );
    $sel1 = $sel->execute ()->fetchCol();
    expDebug::dPrint('custom attribute names---'. print_r($sel1,1),5);
    $result = array_merge($hdr_arr, $sel1); 
    }
    else {
        $result = $hdr_arr;
    }
    expDebug::dPrint('header'. print_r($result,1),5);
    getFeedFileCSV($result,'cre_usr','User_feed');
    } catch (Exception $ex) {
        watchdog_exception('exp_sp_userfeed_csv', $ex);
        expertusErrorThrow($ex);
    }
}

//#custom_attribute_0078975 - Implemented for user Feed CSV to Custom Attribute
function getFeedFileCSV($arr,$entity_type,$asFilename){
        try{
        $start = microtime(true);
        drupal_set_time_limit(3600); // 1 hour Give sufficient time for query execution and CSV generation.
        $tmpFolder = 'tmp';
        //$tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . uniqid('tmpcsv', true) . '.csv';
        $asFilename = getDownloadFileName($asFilename, "csv");
        expDebug::dPrint('$asFilename = ' . $asFilename, 4);
            
        $csvContent = '';
        $csvContent = implode ( '|', $arr );
            
        expDebug::dPrint('$csvContent = ' . $csvContent, 4);
        $tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $tmpFolder . '/' . $asFilename;
        //$tmpFileFullPath = $_SERVER['DOCUMENT_ROOT'] . '/' . $asFilename;
            
        expDebug::dPrint('$tmpFileFullPath = ' . $tmpFileFullPath, 4);
        $tmpFile = fopen($tmpFileFullPath, 'wb');
        $csvContent = chr(255) . chr(254) . mb_convert_encoding($csvContent, 'UTF-16LE', 'UTF-8');
        fwrite($tmpFile, $csvContent);
        fclose($tmpFile);
        $fileSize = filesize($tmpFileFullPath);
        expDebug::dPrint('$fileSize = ' . $fileSize, 4);
        printCSVOutputHeaders($asFilename, $fileSize);
        expPrintFileToClient($tmpFileFullPath);
        unlink($tmpFileFullPath);
        $result = $asFilename;
        expDebug::dPrint('$result= ' . $result, 4);
        } catch (Exception $ex) {
                watchdog_exception('getFeedFileCSV', $ex);
                expertusErrorThrow($ex);
                }
}

/*
 * userNarrowSearchFiltersHTML - Returns the html for narrow search filters
 */
function userNarrowSearchFiltersHTML() {
	try {
  $checkedStatus = array();
  $checkedStatus["cre_usr_sts_atv"] = TRUE;
  $allStatus = getProfileItemNamesKeyed('cre_usr_sts_');
  expDebug::dPrint(' $allStatus = ' . print_r($allStatus, true) , 4);

  //$allUserTypes = getProfileItemNamesKeyed('cre_usr_ptp_','','','','',10);
  expDebug::dPrint(' $allUserTypes = ' . print_r($allUserTypes, true) , 4);

  /*$allUserEmplTypes = getProfileItemNamesKeyed('cre_usr_etp_');
  expDebug::dPrint(' $allUserEmplTypes = ' . print_r($allUserEmplTypes, true) , 4);*/

  $filterSetforJS = array(array("type" => "checkbox", "code" => "userstatus"),
  //array("type" => "addltext","code" => "mgrusername",
	//                                        "acpath" => "administration/people/user/username-autocomplete", "defaultText" => t("LBL181")),
  array("type" => "addltext", "code" => "userorg",
	                                        "acpath" => "administration/people/user/organization-autocomplete", "defaultText" => t("LBL182")),
  array("type" => "addltext", "code" => "location",
		                                      "acpath" => "administration/people/user/location-autocomplete", "defaultText" => t("LBL310")),
  array("type"=> "addltext", "code"=>"classInstructor",
                                "acpath" => "administration/learning/instructor/instructor-autocomplete-withoutid", "defaultText"=> t("LBL259")),
 // array("type" => "checkbox", "code" => "usertype"),
  array("type" => "checkbox", "code" => "group"),
  array("type" => "checkbox", "code" => "empltype"),
  array("type" => "daterange", "code" => "hiredaterange",
		                                      "from_tooltip" => "Select From Date", "from_default_text" => t("LBL220"),
		                                      "to_tooltip" => "Select To Date", "to_default_text" => t("LBL221")),
  array("type" => "addltext", "code" => "userJobrole",
		                                      "acpath" => "administration/people/user/jobrole-autocomplete", "defaultText" => t("LBL764")),
  array("type" => "addltext", "code" => "usertype",
		                                      "acpath" => "administration/people/user/usertype-autocomplete", "defaultText" => t('LBL036').' '.t('LBL173')),
  array("type" => "daterange", "code" => "userhiredate",
  				"from_tooltip" => "Select From Date", "from_default_text" => t("LBL220"),
  				"to_tooltip" => "Select To Date", "to_default_text" => t("LBL221")),//Viswanathan added #0073397
				array("type" => "addltext", "code" => "department",
						"acpath" => "administration/people/user/dept-autocomplete", "defaultText" => t('LBL036').' '.t('LBL179')),
  );

  /*0040029: In user search screen admin should be able to filter user based on their role(amaran) */

  include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
  $start = 0;
  $limit = fetchRolesAll('COUNT');
  $fetchRoleAllList = fetchRolesAll('LISTS',$start,$limit,'', 'AZ');
  foreach($fetchRoleAllList as $key =>$value) {
  	if ($value->status_code == 'cre_sec_sts_atv') {
  		$checkBoxGroupList[$value->id] = $value->name;
  	}
  }
  expDebug::dPrint(' $getGroupDetail merged = ' . print_r($checkBoxGroupList, true) , 4);
  return theme('narrow_search_filters_holder',
  		array('rendered_filtersets_list' => array(
  				theme('narrow_search_filterset_checkboxes',
  						array('code' => 'userstatus',
  								'title' => t('LBL102'),
  								'divid' => 'status',
  								'checkboxes_list' => $allStatus)
  						// 'checked_list' => $checkedStatus)
  				),
  				/*theme('narrow_search_filterset_checkboxes',
  						array('code' => 'usertype',
  								'title' => t('LBL173'),
  								'divid' => 'user_type',
  								'checkboxes_list' => $allUserTypes)
  				),*/
  		    /*0040029: In user search screen admin should be able to filter user based on their role(amaran) */
  		        theme('narrow_search_filterset_checkboxes',
  		            array('code' => 'group',
  		                'title' => t('Group'),
  		                'divid' => 'status',
  		                'checkboxes_list' => $checkBoxGroupList )
  		        ),
  				/*theme('narrow_search_filterset_addltext',
  				 array('code' => 'mgrusername',
  				 		'title' => t('LBL180'),
  				 		'default_text' => t('LBL181'))
  				),*/
  		    theme('narrow_search_filterset_addltext',
  						array('code' => 'usertype',
  								'title' => t('LBL173'),
  								'default_text' => t('LBL036').' '.t('LBL173'))
  				),
  				//Viswanathan added #0073397
  				theme('narrow_search_filterset_daterange',
  				array('code' => 'userhiredate',
  				'dateRangeShowOption' => 'all',
  				'title' => t('LBL175'),
  				'default_text' => t('LBL036').' '.t('LBL173'))
  				),//Viswanathan added #0073397
  				theme('narrow_search_filterset_addltext',
  						array('code' => 'userJobrole',
  								'title' => t('LBL133'),
  								'default_text' => t('LBL764'))
  				),
  				theme('narrow_search_filterset_addltext',
  						array('code' => 'userorg',
  								'title' => t('Organization'),
  								'default_text' => t('LBL182'))
  				),
  				theme('narrow_search_filterset_addltext',
								array('code' => 'department',
										'title' => t('LBL179'),
										'default_text' => t('LBL036').' '.t('LBL179'))
						),
						theme('narrow_search_filterset_addltext',
  						array('code' => 'location',
  								'title' => t('LBL708'),
  								'default_text' => t('LBL310'))
  				),

  				/*theme('narrow_search_filterset_checkboxes',
  				 array('code' => 'empltype',
  				 		'title' => t('LBL174'),
  				 		'checkboxes_list' => $allUserEmplTypes)
  				),
  theme('narrow_search_filterset_daterange',
  		array('code' => 'hiredaterange',
  				'title' => t('LBL175'),
  				'dateRangeShowOption' => 'all') //dateRangeShowOption Option is all or past or future
  ),
  theme('narrow_search_filterset_addltext',
  		array('code' => 'classInstructor',
  				'title' => t('LBL086'),
  				'default_text'=> t("LBL259")
  		)
  ), */
  		),
                          'filter_set_list' => drupal_json_encode($filterSetforJS),
  ));
  } catch (Exception $ex) {
  	watchdog_exception('userNarrowSearchFiltersHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * userSearchResultsSortbarHTML - Returns the html for search results sort bar
 */
function userSearchResultsSortbarHTML() {
try{
  $sortbarLinksList = array();
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('AZ'), 'sort_type' => 'AZ', 'html_id' => 'user-sort-az');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('ZA'), 'sort_type' => 'ZA', 'html_id' => 'user-sort-za');
  $sortbarLinksList[] = array('title' => getNarrowSearchSortTitle('NewlyListed'), 'sort_type' => 'NewlyListed', 'html_id' => 'user-sort-new');

  $sortbarLinksListforJS=array('last_sort_type'=>'NewlyListed',
  					           'last_sort_type_html_id'=>'user-sort-new'
  					           );

  					           return theme('narrow_search_sortbar', array('links_list' => $sortbarLinksList,
                                              'sortbar_list'=> drupal_json_encode($sortbarLinksListforJS),
                                              'title' => t('LBL011'),
  					           )
  					           );
} catch (Exception $ex) {
	watchdog_exception('userSearchResultsSortbarHTML', $ex);
	expertusErrorThrow($ex);
}
}

/*
 * userSearchResultsActionbarHTML - Returns the html for search results action bar
 */
function userSearchResultsActionbarHTML() {
try {
  $actionbarLinksListforJS=array('export_path'=>'administration/people/user/export/',
  					             'print_path'=>'administration/people/user/print/'
  					             );
  					             return theme('narrow_search_actionbar', array('add_button_title' => t('LBL183'),
  					             		'upload_button_title' => t('upload'),
                         'show_add_button' => true,
												 'add_page_path' => base_path() . 'administration/people/user/nojs/addedit',
												 'show_print_button' => true,
												 'show_export_button' => true,
                         'actionbar_list' => drupal_json_encode($actionbarLinksListforJS),
                         'ctools_style'=>'ctools-modal-ctools-admin-user-addedit-style',
  					             'page_code' => 'cre_usr',
  					             )
  					             );
} catch (Exception $ex) {
	watchdog_exception('userSearchResultsActionbarHTML', $ex);
	expertusErrorThrow($ex);
}
}
/*
 * getUserTabContentMainHTML - Returns the html for tab content main
 */
function getUserTabContentMainHTML() {
	try {
  $tabContentTypeArr =  getUserTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];
  if($tabContentType=="narrow_search"){
    $narrowsearchTabContentListforJS=array(
                                            'search_base_path'=>'administration/people/user/search/all/',
                                            'show_top_text_filter' => true,
                                            'text_filter_ac_path' => 'administration/people/user/textfilter-autocomplete'
                                            );

                                            $tabContentTypeStr=theme('narrow_search_tab_content',array(
                                                               'narrowsearch_tab_content_list'=> drupal_json_encode($narrowsearchTabContentListforJS),
                                            )
                                            );
  }else{ /* This  is for non-narrow search */
    $tabContentTypeStr="";
  }
  return $tabContentTypeStr;
  } catch (Exception $ex) {
  	watchdog_exception('getUserTabContentMainHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserTabContent - Returns the html for narrow search block on user search page
 */
function getUserTabContent() {
	global $tTrack;
	$tTrack['start']['Admin_UserFilter'] = microtime(true);
	try {
  $tabContentTypeArr =  getUserTabContentType();
  $tabContentType=$tabContentTypeArr['tab_content_type'];

  $userDetailsResponse=array(
    'rendered_tab_content_type' => $tabContentType,
    'rendered_narrow_search_filters' => userNarrowSearchFiltersHTML(),
    'rendered_narrow_search_sortbar' => userSearchResultsSortbarHTML(),
    'rendered_narrow_search_actionbar' => userSearchResultsActionbarHTML(),
    'rendered_tab_content_main'=>getUserTabContentMainHTML(),
    'rendered_main_div' => getAdminModules('administration','people'),
  );
  $output = drupal_json_encode($userDetailsResponse);
  $tTrack['end']['Admin_UserFilter'] = (microtime(true) - $tTrack['start']['Admin_UserFilter']);
  echo $output;
  } catch (Exception $ex) {
  	watchdog_exception('getUserTabContent', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserDetailsHTML() - Renders user's details as HTML to be displayed in jqGrid.
 */
function getUserDetailsHTML($user) {
	try {
  global $theme_key;
  $detailsList = array(); //The keyword in this array is the displayed detail and value is the tooltip for it.
  $actionList = array();
  $newActionList  = array();
  $addtionalInfo = array();
  
  $solr = getConfigValue('solr_search');
  //This needs changing as when two displayed detail are same, the latter will override the former.
  /*
   if (!empty($user->username)) {
   $detailsList[] = array('detail' => $user->username, 'tooltip' => t('LBL054'));
   }
   if (!empty($user->email)) {
  	$shareTitle = titleController(' ', $user->email,15);
  	$detailsList[$user->email] = array('detail' => $shareTitle, 'tooltip' => t('LBL061').': '.$user->email);
  	}
  	*/
  /*if (!empty($user->status)) {
   $detailsList[$user->status] = array('detail' => $user->status, 'tooltip' => t('LBL102'));
   }*/
  if (!empty($user->orgname)) {
    $shareTitle = titleController('EXP-SP-ADMIN-USER-ORGNAME', sanitize_data($user->orgname),20);
    $detailsList[$user->orgname] = array('detail' => t('Organization').': '.$shareTitle, 'tooltip' => sanitize_data($user->orgname));
  }
  if($solr == 1)
  	$managerName = $user->ManagerName;
  else
  	$managerName= getUserFullname($user->manager_id);
  	
  if (!empty($managerName)) {
    $shareTitle = titleController('EXP-SP-ADMIN-USER-MANAGRNAME', sanitize_data($managerName),20);
    $detailsList[$managerName] = array('detail' => t('Manager').': '.$shareTitle, 'tooltip' => sanitize_data($managerName));
  }
  /*  changes for ticket 16539 - people admin - search - last login - displayed as 31 dec, 1969 */
  if (!empty($user->usr_login) && $user->usr_login != 0) {
    $detailsList[$user->last_login] = array('detail' => t('LBL657').': '.$user->last_login.' <span class="time-zone-text">'.$user->last_login_period.'</span> '.$user->timezone_values);
  }else{
    $detailsList[$user->last_login] = array('detail' => t('LBL658'));
  }

  if($solr == 1){
  	$delVal = 0;
  	if(!empty($user->ClassRegistration) || !empty($user->ProgramRegistration) || !empty($user->dottedmanagersid)){
  		$delVal = 1;
  	}
  	$addtionalInfo['delete_object_record'] = $delVal;
  }
  /*if (!empty($user->city)) {
   $locationCity = $user->city;
   $locationState = '';
   if (!empty($user->countrycode) && ($user->countrycode == 'US' || $user->countrycode == 'CA') && !empty($user->statecode)) {
   $locationState = $user->statecode;
   }
   elseif (!empty($user->statename)) {
   $locationState = $user->statename;
   }

   $location = $locationCity . (empty($locationState)? '': ' ' . $locationState);
   $detailsList[$location] = array('detail' => $location, 'tooltip' => t('LBL041'));
   }*/

  /*  if (!empty($user->type)) {
   $detailsList[$user->type] = array('detail' => $user->type, 'tooltip' => t('LBL243'));
   }
   */
  /*if (!empty($user->empltype)) {
   $detailsList[$user->empltype] = array('detail' => $user->empltype, 'tooltip' => t('LBL174'));
   }
   */
  $actionList[] = array('action_link_text' => t('LBL063'),
				  'action_page_path' => 'administration/people/user/nojs/addedit',
				  'action_button_params' => core_encrypt($user->id),
					  'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					  'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                    'tooltip' => t('LBL063')
  );
  $actionList[] = array('action_link_text' => t('LBL286'),
      'action_page_path' => '',
      'action_button_params' => core_encrypt($user->id),
      'action_button_program_id' => $user->id,
      'action_button_param_text' => 'User',
      'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
      'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
      'tooltip' => t('LBL286')
  );

  // Added Unlock link ticket: 0022334
  //$uid = getLastLoginTime($user->username);
  $authFloodCnt = getAuthFloodCount($user->uid);
  expDebug::dPrint(' authFloodCnt = ' . print_r($authFloodCnt, true) , 4);

  if($authFloodCnt){
   $actionList[] = array('action_link_text' => t('LBL930'),
      'action_page_path' => '',
      'action_button_params' => core_encrypt($user->uid),
      'action_button_program_id' => $user->uid,
      'action_button_param_text' => 'UnlockUser',
      'ctools_style' => 'ctools-modal-ctools-admin-course-class-addedit-style',
      'js_object_info' => array (
                                        'name' => 'narrowsearch', // provide the java script object name
                                        'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
      'tooltip' => t('LBL930'),
   		'icon_style_class'=>'unlock-locked-tab-icon'
    );
  }
	//44133: German-old-ui-Icon not changing when user is activate and suspend
  if(strtolower($user->status_code) == 'cre_usr_sts_atv'){
    $newActionList[] = array('action_link_text' => t('LBL572'),
				  		'action_page_path' => '',
				  		'action_button_params' => core_encrypt($user->id),
    					'action_button_params_text' => 'deactivate',
					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL572'),
                        'icon_style_class'=>'active-deactive-tab-icon Suspend'
                        );
  } else {
    $newActionList[] = array('action_link_text' => t('LBL573'),
				  		'action_page_path' => '',
				  		'action_button_params' => core_encrypt($user->id),
    					'action_button_params_text' => 'activate',
					    'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					    'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    	'tooltip' => t('LBL573'),
                        'icon_style_class'=>'active-deactive-tab-icon Activate'
                        );
  }

  /*$newActionList[] = array('action_link_text' => t('Privilege'),
   'action_page_path' => '',
   'action_button_params' => $user->id,
   'action_button_params_text' => 'privilege',
   'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
   'js_object_info' => array (
   'name' => 'narrowsearch', // provide the java script object name
   'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
   ),
   'tooltip' => t('User privileges')
   );*/
  if(strtolower($user->status_code) == 'cre_usr_sts_atv'){
    $newActionList[] = array('action_link_text' => t('Reset user password'),
				  'action_page_path' => '',
				  'action_button_params' => core_encrypt($user->email),
                  'action_button_user_id' => $user->id,
  				  'action_button_params_text' => 'reset_password',
					  'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					  'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
    ),
                    'tooltip' => t('Reset user password'),
                    'icon_style_class'=>'reset-password-tab-icon'
                    );
  }

  $newActionList[] = array('action_link_text' => t('LBL591'),
				  'action_page_path' => '',
				  'action_button_params' => $user->email,
                  'action_button_params_text' => 'send_message',
					  'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
					  'js_object_info' => array (
                                            'name' => 'narrowsearch', // provide the java script object name
                                            'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  ),
                    'tooltip' => t('LBL591'),
                    'icon_style_class'=>'send-message-tab-icon'
                    );


  $newActionList[] = array('action_link_text' => t('LBL275'),
  		'action_page_path' => '',
  		'action_button_params' => core_encrypt($user->id),
  		'action_button_params_text' => 'enrollments',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL275'),
  		'userId'=> $user->id,
  		'icon_style_class'=>'enrollment-tab-icon'
	);

  $newActionList[] = array('action_link_text' => t('LBL642'),
  		'action_page_path' => '',
  		'action_button_params' => core_encrypt($user->id),
  		'action_button_params_text' => 'access',
  		'ctools_style' => 'ctools-modal-ctools-admin-user-addedit-style',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  		'tooltip' => t('LBL642'),
  		'userId'=> $user->id,
  		'entityId' => $user->id,
  		'entityType' => 'cre_usr',
  		'icon_style_class'=>'access-tab-icon',
  		'postype' => 'bottomright',
  		'poslwid' => 170,
  );
  $viewuserList[] = array('action_link_text' =>'User',
  		'action_page_path' => 'administration/view-user/nojs/addedit/'.core_encrypt($user->id).'',
  		'action_button_params' => core_encrypt($user->id),
  		'action_button_program_id' => $user->id,
  		'ctools_style' => 'ctools-modal-ctools-viewscreen-wrapper',
  		'js_object_info' => array (
  				'name' => 'narrowsearch', // provide the java script object name
  				'init_id' => 'root-admin', // provide the id on which you have initialized/will be initializing the javascript object
  		),
  );
  return theme('narrow-search-results-item-details',
  		array('title' => filterSpecialCharacters(trim($user->lastname)) . ', ' . filterSpecialCharacters(trim($user->firstname)),
  				'additional_title_info' => trim($user->username),
  				'delete_info' => t('MSG357').' '.strtolower(t('User')),
  				'details_list' => $detailsList,
  				'action_list'=> $actionList,
  				'new_action_list'=> $newActionList,
  				'object_type'=>'User',
  				'sumedit' => $user->sumedit,
  				'sumdelete' => $user->sumdelete,
  				'view_list' =>$viewuserList,
  				'addtionalInfo'=>$addtionalInfo
  		)
  );

} catch (Exception $ex) {
	watchdog_exception('getUserDetailsHTML', $ex);
	expertusErrorThrow($ex);
}
}

function getAuthFloodCount($userId){
  try{
    $select = db_select('flood', 'fld');
    $select->addExpression('COUNT(1)', 'count');
    $select->condition('fld.event', 'failed_login_attempt_user', '=');
    $select->condition('fld.identifier', $userId.'-' . '%', 'LIKE');
    $result = $select->execute()->fetchField();

    expDebug::dPrintDBAPI('getWrongAuthFloodCount = ' , $select);
    expDebug::dPrint('getWrongAuthFloodCount = ' . print_r($result, true));

    return (($result > 5)? true : false);
  }catch (Exception $ex) {
    watchdog_exception('getAuthFloodCount', $ex);
    expertusErrorThrow($ex);
  }
}

/*
 * getUserActionsHTML() - Renders user's actions as HTML to be displayed in jqGrid.
 */
function getUserActionsHTML($user) {
	try {
  /*return theme('narrow-search-results-item-actions',
   array('edit_page_path' => base_path() . 'administration/people/user/nojs/addedit',
   'edit_button_action_params' => array($user->id)
   )
   );*/
  return theme('narrow-search-results-item-actions',
  array('catalog_details' => $user)
  );
  } catch (Exception $ex) {
  	watchdog_exception('getUserActionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * userSearchSelectStmtAddOrderByClause() - Function to fetch and add user selected sort order type to the select statement
 */
function userSearchSelectStmtAddOrderByClause($selectStmt, $tableAlias) {
	try {
  switch (getURLParam('sortby')) {
    case "AZ" :
      $selectStmt->orderBy($tableAlias . ".last_name", "ASC");
      $selectStmt->orderBy($tableAlias . ".first_name", "ASC");
      break;

    case "ZA" :
      $selectStmt->orderBy($tableAlias . ".last_name", "DESC");
      $selectStmt->orderBy($tableAlias . ".first_name", "DESC");
      break;

    case "NewlyListed" :
      $selectStmt->orderBy($tableAlias . ".created_on", "DESC");
      break;

    default:
      $selectStmt->orderBy($tableAlias . ".created_on", "DESC");
      break;
  }
  } catch (Exception $ex) {
  	watchdog_exception('userSearchSelectStmtAddOrderByClause', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * sendUsersListToJqGrid() - Prepares JSON response to JqGrid's request for user
 */
function sendUsersListToJqGrid() {
	global $tTrack;
	$tTrack['start']['Admin_UserList'] = microtime(true);
	try {
  expDebug::dPrint(' ' .
                        'page = "' . $_GET['page'] . '" ,' .
                        'rows = "' . $_GET['rows'] . '" ,' .
                        'sort(sidx) = "' . $_GET['sidx'] . '" ,' .
                        'order(sord) = "' . $_GET['sord'] . '" ,' .
                        'search = "' . $_GET['search'] . '" ,' .
                        'nd = "' . $_GET['nd'] . '" ,' .
                        'id = "' . $_GET['id'] . '" ,' .
                        'oper = "' . $_GET['oper'] . '" ,' .
                        'editoper = "' . $_GET['editoper'] . '" ,' .
                        'addoper = "' . $_GET['addoper'] . '" ,' .
                        'deloper = "' . $_GET['deloper'] . '" ,' .
                        'totalrows = "' . $_GET['totalrows'] . '" ,' .
                        'npage = "' . $_GET['npage'] , 4
  );

  $page = $_GET['page']; // get the requested page
  $limit = $_GET['rows'];
	$total_pages = 0;
	
	 // SOLR check starts
  	$solrSource = getConfigValue('solr_search');
	if($solrSource == 0){
	  /* $numUser = fetchUsersAll('COUNT');
	  expDebug::dPrint(' $numUser = ' . print_r($numUser, true) , 4);
	
	  $start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numUser); */
		$start = calculate_offset($page, $limit);
	  expDebug::dPrint(' $start = ' . print_r($start, true) , 4);
	
	  $userList = fetchUsersAll('LIST', $start, $limit);
	  expDebug::dPrint(' $userList = ' . print_r($userList, true) , 4);
	}else{
		require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
		try{
			$solrObj = new UserSolrSearch();
			$solrList = $solrObj->getSolrSearchResult();
			$userList = $solrList['records'];
			$numUser = $solrList['recCount'];
			$start = getOffsetOfFirstRecordInPage($page, $total_pages, $limit, $numUser);
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
		}
		expDebug::dPrint("Solr search result - Final ".print_r($userList,1),4);
	}
  

  // Prepare and send the response
  $response = new stdClass();
  $response->page = $page;
  $response->total = $total_pages;
  $response->records = $numUser;

  //$jsObjectInfo = userJSObjectInfo();
  //$response->js_object_info = $jsObjectInfo; // It should be possible to fetch have js_object_info in jqGrid methods and not
  // have to do this.
  $response->initial_sort_type_html_id = 'user-sort-new';

  //$response->filters_html = userNarrowSearchFiltersHTML($jsObjectInfo);

  $i=0;

  $sysTimezone = date_default_timezone(false);
  $vUserPHPTimezonename   = date_default_timezone();
  foreach ($userList as $user) {
  	if($solrSource == 0){ // Not using solr
	  	//	drupal last login is always based on UTC as it is nothing but $_SERVER['REQUEST_TIME']
	  	//	http://en.wikipedia.org/wiki/Unix_time
	  	//	http://stackoverflow.com/questions/17028604/how-is-phps-serverrequest-time-set
	  	//	date_default_timezone_get() will retun user specific timezone set in account preference
	    $get_last_login = getLastLoginTime($user->username);
	    $vDateTime = $get_last_login[0]->last_login;
	    $vTimeZone = $get_last_login[0]->timezone;
	    //change by ayyappans for 45925 - Last login time shows incorrectly
	    $oDateTime = new DateTime("@".$get_last_login[0]->usr_login);  // will snap to UTC because of the "@timezone" syntax
	    $oDateTime -> setTimezone(new DateTimeZone($vUserPHPTimezonename));	//logged in user's timezone
	
	    /*if(!$user->user_timezone){
	     $vUserPHPTimezonename = $sysTimezone;
	     }*/
	    // Change timevalue from user timezone to admin timezone value.
		// $oDateTime  = timeZoneConvert($vDateTime , $vTimeZone ,  $vUserPHPTimezonename);
	    // timeZoneConvert(date , fromtimezone ,  totimezone);
	    /*-- Changes for 38573: Last login time shows incorrectly --*/
	    $user->uid = $get_last_login[0]->uid;
	    $user->last_login = date_format($oDateTime,'M d, Y h:i');
	    $user->last_login_date = $get_last_login[0]->last_login_date;
	    $user->last_login_period = date_format($oDateTime,'A');
	    /* changes for ticket 16539 - people admin - search - last login - displayed as 31 dec, 1969 */
	    $user->usr_login = $get_last_login[0]->usr_login;
	
	    expDebug::dPrint('exp_sp_administration_user.inc'.print_r($get_last_login,true) , 4);
	
	    expDebug::dPrint(' $user = '. print_r($user, true) , 4);
  	}else{ // If solr in use
  		//change by ayyappans for 45925 - Last login time shows incorrectly
	    $oDateTime = new DateTime($user->Last_Login);  // will snap to UTC because of the "@timezone" syntax
	    $oDateTime -> setTimezone(new DateTimeZone($vUserPHPTimezonename));	//logged in user's timezone
	    $user->last_login = date_format($oDateTime,'M d, Y h:i');
	    $user->last_login_date = date_format($oDateTime,'Y-M-d');
	    $user->last_login_period = date_format($oDateTime,'A');
	    /* changes for ticket 16539 - people admin - search - last login - displayed as 31 dec, 1969 */
	    $user->usr_login = strtotime($user->Last_Login); 
	
	    expDebug::dPrint('exp_sp_administration_user.inc'.print_r($get_last_login,true) , 4);
	
	    expDebug::dPrint(' $user = '. print_r($user, true) , 4);
  	}
    $user->row =  $i;
    $response->rows[$i]['id']   = $user->id;

    $response->rows[$i]['cell'] = array('image' => theme('narrow-search-results-item-image', array('results' => $user)),
    									'details' => getUserDetailsHTML($user),
    // 'action' => getUserActionsHTML($user)
    );

    $i++;
  }

  expDebug::dPrint(' $response record = '. print_r($response, true) , 4);
  //drupal_add_http_header('Content-Type', 'application/json');
  $output = drupal_json_encode($response);
  $tTrack['end']['Admin_UserList'] = (microtime(true) - $tTrack['start']['Admin_UserList']);
  echo $output;
  } catch (Exception $ex) {
  	watchdog_exception('sendUsersListToJqGrid', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByUserNameCondition() - To the $selectStmt add Search by Text (search by ame of the user) condition
 *                                              if set by the user. Use can type:
 *                                              first name or,
 *                                              last name or,
 *                                              lastname, firstname (requires split and then search by first name and last name) or,
 *                                              full_name or,
 *                                              username
 */
function selectAddNarrowSearchByUserNameCondition($selectStmt, $personTableAlias, $searchText = '') {
	try {
		//53638: Home page widgets - Top search bar - Quote based string search not working
  $textFilterStr = addslashes(empty($searchText)? getURLParam('textfilter') : $searchText);
  expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);

  if (!empty($textFilterStr)) {
  	// Commented by Ramkumar For this Ticket 0032601
  	// $splitNames = $textFilterStr;
    /* $splitNames = explode(',', $textFilterStr);
    expDebug::dPrint(' $splitNames = ' . print_r($splitNames, true) , 4);
    if (count($splitNames) > 1) { // User has provided last name, first name
      $lastNameStr = trim($splitNames[0]);
      $firstNameStr = trim($splitNames[1]);

      if (!empty($firstNameStr)) {
        $selectStmt->condition($personTableAlias . '.first_name', '%' . db_like($firstNameStr) . '%', 'LIKE');
      }
      if (!empty($lastNameStr)) {
        $selectStmt->condition($personTableAlias . '.last_name', '%' . db_like($lastNameStr) . '%', 'LIKE');
      }
    } */
    //else { // user has either provided substring from first name, last name or full name
      $db_or = db_or();
      $db_or	->condition($personTableAlias . '.first_name', '%' . ($textFilterStr) . '%', 'LIKE')
      			->condition($personTableAlias . '.last_name', '%' . ($textFilterStr) . '%', 'LIKE')
      			->condition($personTableAlias . '.full_name', '%' . ($textFilterStr) . '%', 'LIKE')
      			->condition($personTableAlias . '.user_name', '%' . ($textFilterStr) . '%', 'LIKE');
      if(empty($searchText)) {
      	$db_or->condition($personTableAlias . '.email', '%' . ($textFilterStr) . '%', 'LIKE');	//for 42123: ListUserAPI does not return data if there is a hyphen in the email id
      }
      $selectStmt->condition($db_or);


   // }
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByUserNameCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * fetchUsersAll() - Fetches all users count or records for jqgrid or records for export/print after applying
 *                  the selected filters.
 */
function fetchUsersAll($op=NULL, $start=NULL, $limit=NULL, $fromApi = false, $username = '') {
	try {
  // Select slt_person table
  $select = db_select('slt_person', 'per');
//  $select->leftJoin('slt_person_other_organization', 'spoo', 'spoo.user_id = per.id');
//  $select->leftJoin('slt_organization', 'org1', 'org1.id=spoo.organization_id');
  /* $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=per.id AND group_map.entity_type = \'cre_usr\' and group_map.group_type = 1');
  $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =1) and catacs.status =\'cre_sec_sts_atv\'');
  // $select->leftJoin('users', 'usr', 'usr.name = per.user_name');
  $select->leftJoin('slt_group_privilege','priv','priv.group_id = catacs.id and priv.page_code= \'cre_usr\''); */

  $locationTextFilterStr = getURLParam('location');
  if (!empty($locationTextFilterStr) && $locationTextFilterStr != t('LBL310')) {
    $select->leftJoin('slt_state', 'state', 'per.state = state.state_code AND per.country = state.country_code');
  }
  /*
   // Left join slt_person (for manager), slt_organization, slt_state and slt_profile_list_items tables
   //$select->leftJoin('slt_person', 'mgr', 'per.manager_id = mgr.id');
   $select->leftJoin('slt_organization', 'org1', 'per.org_id = org1.id');
   //$select->leftJoin('slt_location', 'location', 'per.preferred_loc_id = location.id');
   $select->leftJoin('slt_state', 'state', 'per.state = state.state_code AND per.country = state.country_code');
   $select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=per.time_zone');
   $select->leftJoin('slt_profile_list_items', 'plistatus', 'per.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
   $select->leftJoin('slt_profile_list_items', 'plitype', 'per.user_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
   $select->leftJoin('slt_profile_list_items', 'pliempltype', 'per.employment_type = pliempltype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
   $select->leftJoin('slt_country', 'country', 'per.country = country.country_code');
   $select->leftJoin('users', 'usr', 'usr.name = per.user_name');
   */
  if($fromApi) {
  	//Added the and condution for #0059860 because of the jobrole issue.
  	$select->leftJoin('slt_profile_list_items', 'spli', '(spli.attr1 = per.preferred_currency) and spli.code like \'cre_sys_crn_%\'');
  }
  if($op == 'COUNT') {
    // Get count
    $select->addExpression('COUNT(DISTINCT(per.id))', 'count');
  }
  else {
    // Select fields/expression values to be fetched from the database.
    //$select->distinct();
    //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_edit,0)),1))', 'sumEdit');
    //$select->addExpression('(if(group_map.id is not null,sum(ifnull(priv.priv_delete,0)),1))', 'sumDelete');
    $select->distinct();
    $select->addField('per', 'id', 'id');
    $select->addField('per', 'last_name', 'lastname');
    $select->addField('per', 'first_name', 'firstname');
    $select->addField('per', 'user_name', 'username');
    $select->addField('per', 'email', 'email');
    $select->addField('per', 'manager_id', 'manager_id');
    $select->addField('per', 'status', 'status_code');
    //$select->addField('splt_timezone','attr2', 'user_timezone');
    //$select->addField('splt_timezone','attr1', 'timezone_values');
    // $select->addField('plistatus', 'name', 'status');
    //$select->addExpression('CONCAT(org1.name,\',\',GROUP_CONCAT(distinct(org2.name)))', 'orgname');
	//$select->addExpression('(select GROUP_CONCAT(org2.name) from slt_organization as org2 left join slt_person_other_organization as spoo1 on org2.id in(spoo1.organization_id) where spoo1.user_id = per.id)', 'orgname');
    //$select->addField('org1', 'name', 'orgname');
    // Added for #0059860 not fetching the address.
    $select->addField('per', 'addr1', 'addr1');
    $select->addField('per', 'addr2', 'addr2');
    $select->addField('per', 'city', 'city');
    // $select->addField('state', 'state_name', 'statename');
    $select->addField('per', 'state', 'statecode');
    $select->addField('per', 'country', 'countrycode');
    $select->addField('per', 'preferred_language', 'preferredlanguagecode');
    $select->addField('per', 'time_zone', 'timezonecode');
    $select->addField('per', 'is_mobileadmin', 'register_sms');
    if($fromApi) {
    	$select->addExpression('DATE_FORMAT(per.hire_date,\'%m/%d/%Y\')','hire_date');
//     	$select->addExpression('GROUP_CONCAT(distinct(spom.manager_id))', 'othermanagerid');
//     	$select->leftJoin('slt_person_other_manager', 'spom', 'spom.user_id = per.id and spom.is_direct=\'N\'');
    	$select->addExpression('(SELECT GROUP_CONCAT(distinct(om.manager_id)) from slt_person_other_manager om where om.user_id = per.id and om.is_direct = \'N\')', 'othermanagerid');
    	$select->addExpression('(select GROUP_CONCAT(DISTINCT(spoo2.organization_id)) from slt_person_other_organization as spoo2 where spoo2.user_id = per.id and spoo2.is_direct=\'N\')', 'otherorganisationid');
    	$select->addExpression('(select GROUP_CONCAT(org2.name) from slt_organization as org2 left join slt_person_other_organization as spoo1 on org2.id in(spoo1.organization_id) where spoo1.user_id = per.id and spoo1.is_direct=\'Y\')', 'orgname');
    	$select->addExpression('IF(per.is_instructor=\'Y\' and per.is_manager=\'Y\',\'instructor,manager\',(IF(per.is_instructor=\'Y\',\'instructor\',IF(per.is_manager=\'Y\',\'manager\',\'\'))))','roles');
    	$select->addField('per', 'zip', 'zipcode');
    	$select->addField('per', 'mobile_no', 'mobile_no');
    	$select->addField('per', 'phone_no', 'phone_no');
      	$select->addField('per', 'preferred_currency', 'currency');
		$select->addField('per', 'is_verified_add', 'is_verified_add');
		$select->addField('per', 'is_valid_add', 'is_valid_add');
		$select->addField('per', 'invalid_add_reason', 'invalid_add_reason');
     	$select->addField('spli', 'attr2', 'currency_symbol');
        $select->addField('spli', 'code', 'currency_code');
        
      //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Adding Custom Attribute Fields to Display Columns
        $ret_tmp = getcustomattributemodulestatus();
        if($ret_tmp == true){
          include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute.inc');
          $attrValues = getCustomAttributeValues('User','mandatory');
          foreach($attrValues as $ckey => $cvalue){
            //$select->addField('per', $cvalue->attributerefercolumn, strtolower(str_replace(' ','_',$cvalue->attributename)));
          	$select->addExpression("(IFNULL(IFNULL((SELECT GROUP_CONCAT(DISTINCT `opt0`.`opt_name`) FROM slt_custom_attr_options `opt0`
          			WHERE FIND_IN_SET(`opt0`.`opt_code`,`per`.`$cvalue->attributerefercolumn`)),`per`.`$cvalue->attributerefercolumn`),`per`.`$cvalue->attributerefercolumn`))",strtolower(str_replace(' ','_',$cvalue->attributename)));
          	
          }
      } //#custom_attribute_0078975 - End Check module status
      
    }
    else{
    	$select->addExpression('(select GROUP_CONCAT(org2.name) from slt_organization as org2 left join slt_person_other_organization as spoo1 on org2.id in(spoo1.organization_id) where spoo1.user_id = per.id)', 'orgname');
    	$select->addExpression('GROUP_CONCAT(distinct(spom.manager_id))', 'dottedmanagersid');
    	$select->leftJoin('slt_person_other_manager', 'spom', 'spom.user_id = per.id and spom.is_direct=\'N\'');
    	//$select->addExpression('group_concat(spoo.organization_id)', 'dottedorganizationid');
    	//$select->addExpression('(select GROUP_CONCAT(DISTINCT(spoo2.organization_id) separator \', \') from slt_person_other_organization as spoo2 where spoo2.user_id = per.id and spoo2.is_direct=\'N\')', 'dottedorganizationid');
    }
    if($fromApi) {
    	$select->addField('pl1', 'name', 'usertype');
    	$select->addField('pl2', 'name', 'empltype');
    	$select->addField('per', 'employee_no', 'employeeno');
    	//7306: Support of users to have more than 1 job role association
    	$select->addExpression('GROUP_CONCAT(distinct(pl3.name))', 'jobrole');
    	$select->addField('pl4', 'name', 'jobtitle');
    	$select->addField('pl5', 'name', 'deptcode');
    	$select->leftJoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
    	$select->leftJoin('slt_profile_list_items', 'pl1', 'pl1.code = per.user_type AND pl1.lang_code=\'cre_sys_lng_eng\'');
     	$select->leftJoin('slt_profile_list_items', 'pl2', 'pl2.code = per.employment_type AND pl2.lang_code=\'cre_sys_lng_eng\'');
    	$select->leftJoin('slt_profile_list_items', 'pl3', 'pl3.code = spjm.job_role AND pl3.lang_code=\'cre_sys_lng_eng\'');
    	$select->leftJoin('slt_profile_list_items', 'pl4', 'pl4.code = per.job_title AND pl4.lang_code=\'cre_sys_lng_eng\'');
    	$select->leftJoin('slt_profile_list_items', 'pl5', 'pl5.code = per.dept_code AND pl5.lang_code=\'cre_sys_lng_eng\'');
    }
    //$select->addField('plitype', 'name', 'type');
    //  $select->addField('pliempltype', 'name', 'empltype');
    //  $select->addField('country', 'country_name', 'country_name');
    // $select->addField('country', 'country_code', 'country_code');
    //$select->addExpression('FROM_UNIXTIME(usr.login, \'%Y-%m-%d\')', 'last_login_date');
    //$select->addExpression('FROM_UNIXTIME(usr.login, \'%p\')', 'last_login_period');
    $select->addExpression('\'People\'', 'object_type');
    $select->groupBy('per.id');
    //$select->addExpression('FROM_UNIXTIME(usr.login, \'%Y-%m-%d %H:%i\')', 'last_login'); //'Y-m-d H:i
  }

  // Delete entries should not display
  $select->condition('per.status', array('cre_usr_sts_del'), 'NOT IN');

  // Set the conditions
  $select->condition('per.user_name', array('admin','guest'),'NOT IN');
  $defaultUserStatus = isset($_GET['userstatus']) ? $_GET['userstatus'] : 'cre_usr_sts_atv';
  filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');

  selectAddNarrowSearchINCondition($select, 'per', 'status', 'userstatus');
  selectAddNarrowSearchINCondition($select, 'per', 'employment_type', 'empltype');
  
  if($fromApi){
  	if((isset($_REQUEST['usertype']) && !empty($_REQUEST['usertype']))){
  		selectAddNarrowSearchINCondition($select, 'per', 'user_type', 'usertype');
  	}
  	if((isset($_REQUEST['zipcode']) && !empty($_REQUEST['zipcode']))){
  		selectAddNarrowSearchINCondition($select, 'per', 'zip', 'zipcode');
  	}
  	if((isset($_REQUEST['phone_no']) && !empty($_REQUEST['phone_no']))){
  		selectAddNarrowSearchINCondition($select, 'per', 'phone_no', 'phone_no');
  	}
  }
  //selectAddNarrowSearchByTextCondition($select, 'mgr', 'user_name', 'mgrusername', t('LBL181'));
  selectAddNarrowSearchByMgrTextCondition($select, 'mgrusername', t('LBL181'));
  //selectAddNarrowSearchByTextCondition($select, 'org', 'name', 'userorg', t('LBL182'));
  //selectAddNarrowSearchByTextMultipleORCondition($select, 'org', array('name', 'number'), 'userorg', t('LBL182'));
  selectAddNarrowSearchByOrgTextConditionUser($select, 'userorg', t('LBL182'));
  //selectAddNarrowSearchByTextCondition($select, 'location', 'name', 'location', t('LBL114'));
  selectAddNarrowSearchByCityStateConditionUser($select, 'location', t('LBL310')); // search by class location
  //selectAddNarrowSearchByInstructorConditionUser($select, 'cls', 'classInstructor', t('Type a Instructor')); // search by instructor username
  //selectAddNarrowSearchByUserAddressCondition($select, 'per', 'city', 'country', 'state', 'feature not implemented', 'country', t('Type a location'));
  //selectAddNarrowSearchByDateRangeCondition($select, 'per', 'hire_date', 'hiredaterange');
  
  
  //Adding Custom Attrbiutes Condition #custom_attribute_0078975
  if($fromApi) {
  	$ret_tmp = getcustomattributemodulestatus();
  	if($ret_tmp == true){  	 
      foreach($_POST as $key=>$value) {
        if(strpos($key,'c_')===0 && $value !=''){
            $entityMappingColumn = getCustomAttributeMappingColumn($key,'cre_usr');
            $per_column = 'per.'.$entityMappingColumn['crefercolumn'];            
            /*$select->condition($per_column, '%' . db_like($value) . '%', 'LIKE');*/
            $custom_attr_option_code=$per_column." LIKE '%".$value."%'";
            
            //$customattr_opt_o_pos = strpos($value, ','); //for multiple option
            expDebug::dPrint(' $customattr_opt_o_pos' . print_r($customattr_opt_o_pos, true) , 5);
            //if ($customattr_opt_o_pos>0) {
            	include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_form.inc');
            	$cus_attr_opt_code_arr=explode(",",$value);
            	if(count($cus_attr_opt_code_arr)>0){
            		foreach($cus_attr_opt_code_arr as $o_key => $o_val){
            			expDebug::dPrint(' $$o_val' . print_r($o_val, true) , 5);
            			if($o_val!='')
            			{
            				$custom_attr_option_code.=" OR ".$per_column." LIKE '%".getCustomAttributeCode($o_val,'o')."%'";
            			}
            		}
            	}
            
            $select->where($custom_attr_option_code);
        }
      }
  } //#custom_attribute_0078975 - End Check module status
  }
  
  if($fromApi){
  	if(!empty($_REQUEST['roles'])){
  		$roles = explode(",",$_REQUEST['roles']);
  		expDebug::dPrint(' $form_state[values] = fetchUsersAllssssss' . print_r($roles, true) , 5);
  		if(!empty($roles)){
  			if(in_array("instructor", $roles) && in_array("manager", $roles)){
  				$select->condition(db_or()
  						->condition('per.is_instructor', 'Y', '=')
  						->condition('per.is_manager', 'Y', '='));	
  			}else if(in_array("instructor", $roles)){
  				$select->condition('per.is_instructor', 'Y', '=');
  			}else if(in_array("manager", $roles)){
  			     $select->condition('per.is_manager', 'Y', '=');
  		}
  		}
  	}
  }
  if(empty($username)) {
  	selectAddNarrowSearchByUserNameCondition($select, 'per');
  }
  else {
  	$select -> condition('per.user_name', $username, 'LIKE');
  }
  // 0047776 user type Refine Search
  if(isset($_GET['usertype']) && !empty($_GET['usertype'])){
  	$select->leftJoin('slt_profile_list_items', 'useritems', 'useritems.code = per.user_type');
  	selectAddNarrowSearchByTextCondition($select, 'useritems', 'name', 'usertype', t('LBL036').' '.t('LBL173'));
  }
  //Viswanathan added #0073397
  if(isset($_GET['userhiredate']) && !empty($_GET['userhiredate']&& $_GET['userhiredate']!='|')){
  	selectAddNarrowSearchByHiredateCondition($select, 'per', 'hire_date', 'userhiredate', t('LBL036').' '.t('LBL173'));
  }//Viswanathan added #0073397
  // 0045190 Department Refine Search
  if(isset($_GET['department']) && !empty($_GET['department'])){
  	$select->leftJoin('slt_profile_list_items', 'deptitems', 'deptitems.code = per.dept_code');
  	selectAddNarrowSearchByTextCondition($select, 'deptitems', 'name', 'department', t('LBL036').' '.t('LBL179'));
  }

  if(isset($_GET['userJobrole']) && !empty($_GET['userJobrole'])){
  		//7306: Support of users to have more than 1 job role association
  	$select->leftJoin('slt_person_jobrole_mapping', 'spjm', 'spjm.user_id = per.id');
    $select->leftJoin('slt_profile_list_items', 'items', 'items.code = spjm.job_role');
    // $select->leftJoin('slt_survey', 'tmplt', 'tmplt.id = maps.survey_id');
    //$select->leftJoin('slt_profile_list_items', 'plitype', 'per.user_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
    selectAddNarrowSearchByTextCondition($select, 'items', 'name', 'userJobrole', t('LBL764'));
    //selectAddNarrowSearchByTextMultipleORCondition($select, 'tmplt', array('title', 'code'),'classSurvey',t('LBL757'));
  }

  if(isset($_GET['group']) && !empty($_GET['group'])){
    include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
    $filterGroupId = explode('|',$_GET['group']);
    $groupNameArr = array();
    foreach($filterGroupId as $key => $grpId) {
      //getUserDetailList($grpId,$start = 0,$end = 0,$userName,$userSearchType,$count='',$fetchId ='') in exp_sp_administration_groups.inc
      $userListInGroup[$key] = getUserDetailList($grpId,0,0,'','','',1);
      $groupNameArr[] = getNameForGroupId($grpId);
      $countOfUserId = count($userListInGroup[$key]);
      if($countOfUserId != 0)
        $userIdList[] = getUserIdFromGroupUserList($userListInGroup[$key]);
    }
    $uniqueUserIdList = array_unique(explode(',',implode(',',$userIdList)));
    expDebug::dPrint(' list of user id for given group ids = ' . print_r($uniqueUserIdList, true) , 3);
    (count($groupNameArr)!= 0) ? $groupNames = implode('|', $groupNameArr) : $groupNames = '';
    (count($groupNameArr)!= 0) ? $groupNames_pdf = implode(',', $groupNameArr) : $groupNames_pdf = '';
    $select->condition('per.id', $uniqueUserIdList, 'IN');
  }



  if($op != "COUNT" && $fromApi  == false){
    // Order the records
    userSearchSelectStmtAddOrderByClause($select, 'per', "AZ");
  }

  //expDebug::dPrintDBAPI(' $select obj = ' , $select);

  if($op == "COUNT") {
  	$selectInd = $select->extend("ExpertusSelectQueryExtender");
		$selectInd->setIndex(array('per'=>'sli_per_stat'));
		expDebug::dPrintDBAPI('User Count query  = ' , $selectInd);
	  $result = $selectInd->execute()->fetchField();
    //$result = $select->execute()->fetchField();
    expDebug::dPrint(' COUNT $result = ' . print_r($result, true) , 3);

    return $result;
  }
  else if ($op == "CSV") {
    $select->addExpression('CONCAT(per.last_name, \', \', per.first_name)', 'name');
    //$select->leftJoin('slt_country', 'ct', 'per.country = ct.country_code');
    //$select->addField('ct', 'country_name', 'country');

    $colHeadersMap = array(
    t('LBL107') => 'name',
    //t('LBL056') => 'firstname',
    t('LBL054') => 'username',
    t('LBL061') => 'email',
    //t('LBL102') => 'status',
    t('Organization') => 'orgname',
    //t('LBL066') => 'city',
    //t('LBL152') => 'statename',
    //t('LBL039') => 'country',
    //t('LBL173') => 'type',
    //t('LBL174') => 'empltype'
    );

    // Applied the filter option  for CSV
    $filterArray[] = array(t('LBL3046').':'.textboxFiltersetValueToPrintableStr('textfilter'));
    $filterArray[] = array(t('LBL102').':'.checkboxFiltersetSelectionsToPrintableStr('userstatus'));
    $filterArray[] = array(t('Groups').':'.$groupNames);
    $filterArray[] = array(t('LBL173').':'.checkboxFiltersetSelectionsToPrintableStr('usertype'));
    $filterArray[] = array(t('Manager').':'.textboxFiltersetValueToPrintableStr('mgrusername', t('LBL181')));
    $filterArray[] = array(t('LBL133').':'.textboxFiltersetValueToPrintableStr('userJobrole', t('LBL764')));
    $filterArray[] = array(t('Organization').':'.textboxFiltersetValueToPrintableStr('userorg', t('LBL182')));
    $filterArray[] = array(t('LBL708').':'.textboxFiltersetValueToPrintableStr('location', t('LBL310')));
    $filterArray[] = array(ucfirst(t('LBL011')).':'.getNarrowSearchSortTitle());
		// added for performance inprovement
    $selectInd = $select->extend("ExpertusSelectQueryExtender");
		$selectInd->setIndex(array('per'=>'sli_per_stat'));
    SendNarrowSearchResultsAsCSVFile($selectInd, $colHeadersMap, 'export', $filterArray);

  }
  else if($op == "PDF") {
  	// Prepare lastname, firstname
  	$select->addExpression('CONCAT(per.last_name, \', \', per.first_name)', 'name');

/*    foreach ($pdfList as $key => $array) {
      // Prepare lastname, firstname
      $pdfList[$key]['name'] = $array['lastname'] . ', ' . $array['firstname'];

      // Prepare user type = usertype (empltype)
      if (!empty($array['type']) && !empty($array['empltype'])){
        $pdfList[$key]['usertype'] = $array['type'] . ' (' . $array['empltype'] . ')';
      }
      else if (!empty($array['type']) && empty($array['empltype'])) {
        $pdfList[$key]['usertype'] = $array['type'];
      }
      else if (empty($array['type']) && !empty($array['empltype'])) {
        $pdfList[$key]['usertype'] = $array['empltype'];
      }
      else { // both are empty
        $pdfList[$key]['usertype'] = '';
      }
    } // end foreach*/

    // % width of the columns must total to 100%
    expDebug::dPrintDBAPI('The value of $select is here ',$select);
    $colHeadersMap = array(t('LBL107') => array('name',     30),
    t('LBL054') => array('username', 20),
    //t('LBL102') => array('status',   10), When uncommenting pls ensure column widths add upto 100%
    t('LBL061') => array('email',    23),
    t('Organization') => array('orgname',  27),
    //t('LBL039') => array('country',  13), When uncommenting pls ensure column widths add upto 100%
    //t('LBL173') => array('type',     14), When uncommenting pls ensure column widths add upto 100%
    //t('LBL174') => array('empltype', 17), When uncommenting pls ensure column widths add upto 100%
    );

    $appliedFilters = array(t('LBL3046') => textboxFiltersetValueToPrintableStr('textfilter'),
    t('LBL102') => checkboxFiltersetSelectionsToPrintableStr('userstatus'),
    t('Groups') => $groupNames_pdf,
    t('LBL173') => checkboxFiltersetSelectionsToPrintableStr('usertype'),
    t('Manager') => textboxFiltersetValueToPrintableStr('mgrusername', t('LBL181')),
    t('LBL133') => textboxFiltersetValueToPrintableStr('userJobrole', t('LBL764')),
    t('Organization') => textboxFiltersetValueToPrintableStr('userorg', t('LBL182')),
    t('LBL708') => textboxFiltersetValueToPrintableStr('location', t('LBL310')),
    //t('LBL174') => checkboxFiltersetSelectionsToPrintableStr('empltype'),
    //t('LBL175') => daterangeFiltersetValueToPrintableStr('hiredaterange'),
    ucfirst(t('LBL011')) => getNarrowSearchSortTitle(),
    );

    expDebug::dPrint(' appliedFilters check by amaran = ' . print_r($appliedFilters, true) , 3);

    // Added for performance imporvment
    $selectInd = $select->extend("ExpertusSelectQueryExtender");
		$selectInd->setIndex(array('per'=>'sli_per_stat'));
    SendNarrowSearchResultsAsPDFFile($selectInd, t('User'), $appliedFilters, $colHeadersMap);

  }
  else {
  	// LIST - data for jqgrid
  	//no of total records has to be calculated before applying limit
    expDebug::dPrintDBAPI('User count query  = ' , $select->countQuery());
  	if($fromApi) 
  		$totalRecords = $select->countQuery()->execute()->fetchField(); // #45903 - list of records attached
  	add_row_id_condition($select, 'per');
  	// Added for performance improvement
//   	$selectInd = $select->extend("ExpertusSelectQueryExtender");
// 		$selectInd->setIndex(array('per'=>'sli_per_stat'));
// 		expDebug::dPrintDBAPI('User query  = ' , $selectInd);
// 	$totalRecords = $selectInd->countQuery()->execute()->fetchField(); // #45903 - list of records attached
  	//expDebug::dPrint(' $totalRecords --> 	' . print_r($totalRecords, true) , 3);
    // Limit to $limit records
    if ($limit) { // #45034 - limit value should be postive number
    	$select->range($start, $limit);
    }
    //expDebug::dPrintDBAPI(' with range $select = ' , $select);
    // Execure the query and fetch all the records
    // Added for performance
    $selectInd = $select->extend("ExpertusSelectQueryExtender");
		$selectInd->setIndex(array('per'=>'sli_per_stat'));
		expDebug::dPrintDBAPI('User query  = ' , $selectInd);
	  $result = $selectInd->execute()->fetchAll();

    //$result = $select->execute()->fetchAll();

    expDebug::dPrint(' result 	' . print_r($result, true) , 3);
	if($fromApi) {
		foreach($result as $key=>$val) {
			$groupsList = getGroupsDetails($result[$key]->id);
			$groups = '';
			foreach ($groupsList as $index=>$value) {
				$groups = empty($groups) ? $value->name : $groups.', '.$value->name;
			}
			$result[$key]->groups = $groups;
		}
	}
	if ($totalRecords && $fromApi)
		$result['totalrow'] = $totalRecords;
	return $result;
  }
  } catch (Exception $ex) {
  	watchdog_exception('fetchUsersAll', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByOrgTextCondition() - When narrowing search by user organization, it could be user's organization or one of
 *                                             user's other organization. Also, search could be performed by code.
 *
 */
function selectAddNarrowSearchByOrgTextCondition($selectStmt, $filtersetCode, $defaultText = '') {
	try {
  $orgTextFilterStr = getURLParam($filtersetCode);
  $strlen = strlen($orgTextFilterStr);
  $lastoccur = strrpos($orgTextFilterStr,'(');
  $orgnum = substr($orgTextFilterStr,$lastoccur);
  $orgname = str_replace(' '.$orgnum,'',$orgTextFilterStr);
  $orgnum = substr($orgnum,1,-1);

  expDebug::dPrint(' $orgTextFilterStr = ' . print_r($orgTextFilterStr, true) , 4);

  if (!empty($orgTextFilterStr) && $orgTextFilterStr != $defaultText) {
    $selectStmt->leftJoin('slt_organization', 'org2', '((org2.id = per.org_id) OR (FIND_IN_SET(org2.id, per.dotted_org_id) > 0))');
    // Removing the code because no more code field in the orgaznization table.	AND org3.number like \'%' . db_like($orgnum) . '%\'
    $selectStmt->where('org2.id IN (select id from slt_organization org3 ' .
                                          'where org3.name like \'%' . db_like($orgname) . '%\' )'
                                          );
                                          expDebug::dPrint(' $select = ' . print_r($selectStmt->__toString(), true) , 4);
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByOrgTextCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * selectAddNarrowSearchByOrgTextCondition() - When narrowing search by user organization, it could be user's organization or one of
 *                                             user's other organization. Also, search could be performed by code.
 *
 */
function selectAddNarrowSearchByOrgTextConditionUser($selectStmt, $filtersetCode, $defaultText = '') {
	try {
  $orgTextFilterStr = getURLParam($filtersetCode);
  $strlen = strlen($orgTextFilterStr);
  $lastoccur = strrpos($orgTextFilterStr,'(');
  $orgnum = substr($orgTextFilterStr,$lastoccur);
  $orgname = str_replace(' '.$orgnum,'',$orgTextFilterStr);
  $orgnum = substr($orgnum,1,-1);

  expDebug::dPrint(' $orgTextFilterStr = ' . print_r($orgTextFilterStr, true) , 4);
  
  if (!empty($orgTextFilterStr) && $orgTextFilterStr != $defaultText) {
      $selectStmt->leftJoin('slt_person_other_organization', 'spoo', 'spoo.user_id = per.id');
      $selectStmt->leftJoin('slt_organization', 'org1', 'org1.id=spoo.organization_id');
    //$selectStmt->leftJoin('slt_organization', 'org2', '((org2.id = per.org_id) OR (FIND_IN_SET(org2.id, per.dotted_org_id) > 0))');
    // Removing the code because no more code field in the orgaznization table.	AND org3.number like \'%' . db_like($orgnum) . '%\'
    //  $selectStmt->where('org2.id IN (select id from slt_organization org3 ' .
    //                                      'where org3.name like \'%' . db_like($orgname) . '%\' )'
    //                                        );
    $selectStmt->condition('org1.name', '%' . db_like($orgname) . '%', 'LIKE');
    expDebug::dPrint(' $select = ' . print_r($selectStmt->__toString(), true) , 4);
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByOrgTextConditionUser', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * selectAddNarrowSearchByMgrTextCondition() - When narrowing search by manager username, it could be user's manager's username or one of
 *                                             user's other managers username (dotted managers).
 *
 */
function selectAddNarrowSearchByMgrTextCondition($selectStmt, $filtersetCode, $defaultText = '') {
	try {
  $mgrTextFilterStr = getURLParam($filtersetCode);
  expDebug::dPrint(' $mgrTextFilterStr = ' . print_r($mgrTextFilterStr, true) , 4);

  if (!empty($mgrTextFilterStr) && $mgrTextFilterStr != $defaultText) {
    //$selectStmt->leftJoin('slt_person', 'mgr1', '((mgr1.id = per.manager_id) OR (FIND_IN_SET(mgr1.id, per.dotted_mngr_id) > 0))');
//   	$selectStmt->leftJoin('slt_person', 'mgr1', '((mgr1.id = per.manager_id))');
//     $selectStmt->where('mgr1.user_name like \'%' . db_like($mgrTextFilterStr) . '%\' ' );

  	$selectStmt->leftJoin('slt_person_other_manager', 'spom', 'spom.user_id = per.id');
  	$selectStmt->leftJoin('slt_person' , 'pers' , 'pers.id = spom.manager_id');
  	$selectStmt->where('pers.user_name like \'%' . db_like($mgrTextFilterStr) . '%\' ' );
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByMgrTextCondition', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * selectAddNarrowSearchByUserAddressCondition() - To the $select add Search by user address (city, state, country fields) if set by the user.
 *                                                 Location can be state name, city name or state code when country_code is 'US' or 'CA'
 *                                                 No filter yet added to invoke this function. This was the previous implementation for location
 *                                                 narrow search text filter.
 */
function selectAddNarrowSearchByUserAddressCondition($selectStmt, $tableAlias, $tableField, $countryCodeField, $stateTableAlias, $filtersetCode, $countryTableAlias, $defaultText = '') {
	try {
  $locationFilterStr = getURLParam($filtersetCode);
  expDebug::dPrint(' $locationFilterStr = ' . print_r($locationFilterStr, true) , 4);

  if (!empty($locationFilterStr) && $locationFilterStr != $defaultText) {
    $selectStmt->condition(db_or()
    ->condition($countryTableAlias . '.' . 'country_name', '%' . db_like($locationFilterStr) . '%', 'LIKE')
    ->condition($tableAlias . '.' . $tableField, '%' . db_like($locationFilterStr) . '%', 'LIKE')
    ->condition($stateTableAlias . '.state_name', '%' . db_like($locationFilterStr) . '%', 'LIKE')
    ->condition(db_and()
    ->condition($stateTableAlias . '.state_code', '%' . db_like($locationFilterStr) . '%', 'LIKE')
    ->condition(db_or()
    ->condition($tableAlias . '.' . $countryCodeField, 'US', '=')
    ->condition($tableAlias . '.' . $countryCodeField, 'CA', '=')
    )

    )
    );
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByUserAddressCondition', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserTextFilterAutoComplete() - Returns the matched user name
 */
function getUserTextFilterAutoComplete() {
	try {
		
		$solrSource = getConfigValue('solr_search');
		if($solrSource == 0){		
  			$searchText =  $_GET['z'];
			expDebug::dPrint('textinput'.$searchText);
  // Select the table
  $select = db_select('slt_person', 'per');

  // Add fields to fetch
 // $select->addField('per', 'last_name', 'lastname');
 //36609: People search is not working properly.
  $select->addExpression('DISTINCT(per.full_name)', 'firstname');
  $select->condition('per.user_name',array('admin','guest'),'NOT IN');
  $select->condition('per.status',array('cre_usr_sts_itv','cre_usr_sts_atv'),'IN');

  // Add condition
  selectAddNarrowSearchByUserNameCondition($select, 'per', $searchText); // Same logic as used to fetch records
  filterByUserAccessPrivileges($select,'per','',1,'cre_usr','autocomplete','per');
  // Order in dictionary sort order first by lastname and then by first name.
 // $select->orderBy('lastname', 'ASC');
  $select->orderBy('firstname', 'ASC');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  $matchingUserNames = $select->execute()->fetchAll();
		}
		else {
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			try{
				$solrObj = new UserSolrSearch();
				$solrList = $solrObj->getSolrSearchResultAutcomplete();
				$matchingUserNames = $solrList['records'];
				$numUser = $solrList['recCount'];
				}catch(Exception $e){
				expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
			}
			expDebug::dPrint("Solr search result - Final ".print_r($userList,1),4);
		}
  expDebug::dPrint(' $matchingUserNames = '. print_r($matchingUserNames, true) , 3);

  foreach ($matchingUserNames as $userName) {
    print rawurlencode($userName->firstname) . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getUserTextFilterAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserAddressAutoComplete() - Returns the matched city or state from slt_person and slt_state tables
 *                                 I think country needs to be added as well (TBD).
 */
function getUserAddressAutoComplete() {
	try {
  $searchText =  $_GET['z'];

  // First fetch all cities in slt_person that match $searchRext
  // Select the table
  $select = db_select('slt_person', 'per');

  // Add fields to fetch
  $select->addExpression('DISTINCT(per.city)', 'name');

  // Add condition
  $select->condition('per.city', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('per.user_name',array('admin','guest'),'NOT IN');
  // Order in dictionary sort order first by lastname and then by first name.
  //$select->orderBy('cname', 'ASC');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  $matchingCities = $select->execute()->fetchCol();
  expDebug::dPrint(' $matchingCities = '. print_r($matchingCities, true) , 3);

  // Then fetch all states in slt_person that match $searchText
  // Select the table
  $select = db_select('slt_state', 'state');

  // Add fields to fetch
  $select->addExpression('DISTINCT(state.state_name)', 'name');

  // Add condition
  $select->condition('state.state_name', '%' . db_like($searchText) . '%', 'LIKE');


  // Order in dictionary sort order first by lastname and then by first name.
  //$select->orderBy('sname', 'ASC');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  $matchingStates = $select->execute()->fetchCol();
  expDebug::dPrint(' $matchingStates = '. print_r($matchingStates, true) , 3);

  //select the country
  $select = db_select('slt_country', 'country');
  // Add fields to fetch
  $select->addExpression('DISTINCT(country.country_name)', 'name');

  // Add condition
  $select->condition('country.country_name', '%' . db_like($searchText) . '%', 'LIKE');

  $matchingCountries = $select->execute()->fetchCol();
  expDebug::dPrint(' $matchingCountries = '. print_r($matchingStates, true) , 3);
  // Merge and sort matched cities and states

  $displayNames = array_merge($matchingCities, $matchingStates, $matchingCountries);
  sort($displayNames);
  $displayNames = array_unique($displayNames);
  expDebug::dPrint(' $displayNames = '. print_r($displayNames, true) , 4);

  foreach ($displayNames as $name) {
    print $name . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getUserAddressAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getDefaultUserAddEditFormFieldValue() - Determines the default value for a form field to be shown in the form when it is painted.
 */
function getDefaultUserAddEditFormFieldValue($field, $form_state, $userDetails) {
try {
  $defaultValue = '';

  /*'id', 'first_name', 'last_name', 'middle_name', 'user_name', 'email', 'status'*/

  switch ($field) {
    case 'password' :
    case 'confirm_password' :
      $defaultValue = ''; // Reset the password/confirm password default value each time the form is rebuild
      break;

    case 'notify_user' :
      $defaultValue = empty($form_state['values'][$field])? 0 : $form_state['values'][$field];
      break;

    default:   // For all other fields
      $defaultValue = getDefaultAddEditFormFieldValue($field, $form_state, $userDetails);
      break;
  } // end switch */

  expDebug::dPrint(' $defaultValue for field ' .
  $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultUserAddEditFormFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * addPasswordEntryFieldsToAddEditForm() -  Use this function to add a password fields, Password, ConfirmPassword and SendNotification Checkbox to
 *                                     the addedit form.
 */
function addPasswordEntryFieldsToAddEditForm(&$form,
$idPasswordField, $titlePasswordField = '', $defaultValuePasswordField = '',
$idConfirmPasswordField, $titleConfirmPasswordField = '', $defaultValueConfirmPasswordField = '',
$idSendNotificationCheckbox, $titleSendNotificationCheckbox = '', $defaultValueSendNotificationCheckbox = 0,
$tabIndex = null, $passwordMandatory = false, $notificationCheckboxMandatory = false,
$readOnly = false, $maxChars = 99999, $viewWidthInChars = 60) {
 try {
  // Prepare the additional HTML attributes for the password and confirm password fields
  $passwordFieldAttributes = array();
  $passwordFieldAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $passwordFieldAttributes['class'] = array('addedit-password-' . $idPasswordField);
  $passwordFieldAttributes['data-password-idclass'] = array('addedit-password-' . $idPasswordField); // Used in the pwd field behavior to get field value
  $passwordFieldAttributes['data-cpassword-idclass'] = array('addedit-cpassword-' . $idConfirmPasswordField); // Used in the pwd field behavior to get field value

  $passwordFieldAttributes['class'][] = 'addedit-password-field'; // To add password field behavior

  if ($readOnly) {
    $passwordFieldAttributes['class'][] = 'addedit-readonly-password-field';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $passwordFieldAttributes['tabindex'] = $tabIndex;
  }

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

  $separatorSymbol = ':';
  $mandatorySymbol  = '<span class="addedit-mandatory">*</span>';

  $passwordPrefix = '<div class="addedit-pwd-fields-set">' .
                      '<div class="addedit-pwd-fields-set-lcol">' .
                        '<div class="addedit-new-field">' .
                          '<div class="addedit-new-field-title">' .
  $titlePasswordField . $separatorSymbol . ($passwordMandatory? $mandatorySymbol : '') .
                          '</div>' .
                          '<div class="addedit-new-field-value">';
  $passwordSuffix =       '</div>' .
                          '<div class="clearBoth"></div>' .
                        '</div>';
  $confirmPasswordPrefix =
                        '<div class="addedit-new-field">' .
                          '<div class="addedit-new-field-title">' .
  $titleConfirmPasswordField . $separatorSymbol . ($passwordMandatory? $mandatorySymbol : '') .
                          '</div>' .
                          '<div class="addedit-new-field-value">';
  $confirmPasswordSuffix =
                          '</div>' .
                          '<div class="clearBoth"></div>' .
                        '</div>' .
                      '</div>';
  $sendNotiCheckboxPrefix =
                      '<div class="addedit-pwd-fields-set-rcol">' .
                        '<div  class="addedit-new-field">';
  $sendNotiCheckboxSuffix =
                          '<div class="addedit-checkbox-suffixed-title">' .
  $titleSendNotificationCheckbox . ($notificationCheckboxMandatory? $mandatorySymbol : '') .
                          '</div>' .
                        '</div>' .
                      '</div>' .
                      '<div class="clearBoth"></div>' .
                    '</div>';

  // Create the password form element
  $form[$idPasswordField] = array(
    '#type' => 'password',
  //'#prefix' => addEditGetPrefix('FIELD', $titlePasswordField, $passwordMandatory),
  //'#suffix' => addEditGetSuffix('FIELD'),
    '#prefix' => $passwordPrefix,
    '#suffix' => $passwordSuffix,
    '#title' => $titlePasswordField,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValuePasswordField,
    '#disabled' => $readOnly,
    '#attributes' => $passwordFieldAttributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars
  );

  // Add the confirm password form element

  // Class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $passwordFieldAttributes['class'] = array('addedit-cpassword-' . $idConfirmPasswordField);
  $passwordFieldAttributes['data-password-idclass'] = array('addedit-password-' . $idPasswordField); // Used in the cpwd field behavior to get field value
  $passwordFieldAttributes['data-cpassword-idclass'] = array('addedit-cpassword-' . $idConfirmPasswordField); // Used in the cpwd field behavior to get field value

  $passwordFieldAttributes['class'][] = 'addedit-cpassword-field'; // Class used for adding cpassword field behavior

  if ($readOnly) {
    $passwordFieldAttributes['class'][] = 'addedit-readonly-password-field';
  }
  if (isset($tabIndex) && $tabIndex > 0) {
    $passwordFieldAttributes['tabindex'] = $tabIndex + 1;
  }

  $form[$idConfirmPasswordField] = array(
    '#type' => 'password',
    '#prefix' => $confirmPasswordPrefix,
    '#suffix' => $confirmPasswordSuffix,
    '#title' => $titleConfirmPasswordField,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValueConfirmPasswordField,
    '#disabled' => $readOnly,
    '#attributes' => $passwordFieldAttributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars
  );

  // Add the send notification checkbox

  //Create the attributes array for the send notification checkbox
  $checkboxAttributes = array();
  $checkboxAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $checkboxAttributes['class'] = array('addedit-checkbox-' . $idSendNotificationCheckbox);

  if ($readOnly) {
    $checkboxAttributes['class'][] = 'addedit-readonly-checkbox';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $checkboxAttributes['tabindex'] = $tabIndex + 2;
  }

  $form[$idSendNotificationCheckbox] = array (
    '#type' => 'checkbox',
    '#prefix' => $sendNotiCheckboxPrefix,
    '#suffix' => $sendNotiCheckboxSuffix,
    '#title' => $titleSendNotificationCheckbox,
    '#title_display' => 'invisible',
    '#default_value' =>  $defaultValueSendNotificationCheckbox,
    '#disabled' => $readOnly,
    '#attributes' => $checkboxAttributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addPasswordEntryFieldsToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * addPasswordEntryFieldsToAddEditForm() -  Use this function to add a password fields, Password, ConfirmPassword and SendNotification Checkbox to
 *                                     the addedit form.
 */
function addNotifyUserToAddEditForm(&$form, $idSendNotificationCheckbox, $titleSendNotificationCheckbox = '', $defaultValueSendNotificationCheckbox = 0,
$notificationCheckboxMandatory = false, $readOnly = false) {
 try {
  $separatorSymbol = ':';
  $mandatorySymbol  = '<span class="addedit-mandatory">*</span>';


  $sendNotiCheckboxPrefix =
  					'<div class="addedit-pwd-fields-set">' .
                      '<div class="addedit-pwd-fields-set-rcol">' .
                        '<div  class="addedit-new-field">';
  $sendNotiCheckboxSuffix =
                          '<div class="addedit-checkbox-suffixed-title">' .
  $titleSendNotificationCheckbox . ($notificationCheckboxMandatory? $mandatorySymbol : '') .
                          '</div>' .
                        '</div>' .
                      '</div>' .
                      '<div class="clearBoth"></div>' .
                    '</div>';




  // Add the send notification checkbox

  //Create the attributes array for the send notification checkbox
  $checkboxAttributes = array();
  $checkboxAttributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $checkboxAttributes['class'] = array('addedit-checkbox-' . $idSendNotificationCheckbox);

  if ($readOnly) {
    $checkboxAttributes['class'][] = 'addedit-readonly-checkbox';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $checkboxAttributes['tabindex'] = $tabIndex + 2;
  }

  $form[$idSendNotificationCheckbox] = array (
    '#type' => 'checkbox',
    '#prefix' => $sendNotiCheckboxPrefix,
    '#suffix' => $sendNotiCheckboxSuffix,
    '#title' => $titleSendNotificationCheckbox,
    '#title_display' => 'invisible',
    '#default_value' =>  $defaultValueSendNotificationCheckbox,
    '#disabled' => $readOnly,
    '#attributes' => $checkboxAttributes
  );
  } catch (Exception $ex) {
  	watchdog_exception('addNotifyUserToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function uploadUserPictureSeparately($form, &$form_state, $createUserId, $drupalUID){
  // Upload and save user's picture
  try{
  $pictureId = uploadUserPicture('picture', $drupalUID, 1);
  return $pictureId;
  } catch (Exception $ex) {
  	watchdog_exception('uploadUserPictureSeparately', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateUserAndPictureId($drupalUid, $pictureId){
  // Save the picture reference (file id) in users table against the user's record.
  try {
  $numUpdated = db_update('users')
  ->fields(array('picture' => $pictureId))
  ->condition('uid', $drupalUid)
  ->execute();
  } catch (Exception $ex) {
  	watchdog_exception('updateUserAndPictureId', $ex);
  	expertusErrorThrow($ex);
  }
}

function deleteGrpList($drupalUid,$grpId,$userId){
	try {
	$txn = db_transaction();
	$select = db_select('role','role');
	$select->join('slt_groups','grp','role.name=grp.name');
	$select->addField('role','rid','rid');
	$select->condition('grp.id',$grpId,'=');
	expDebug::dPrintDBAPI('Fetch role Id ' , $select);
	$srleid = $select->execute()->fetchField();
	//Users_role update
	$query = db_delete('users_roles');
	$query->condition('uid', $drupalUid);
	$query->condition('rid', $srleid);
	$query->execute();

	$fields = array(
	        'table_name' => 'users_roles',
	        'entity_id' => $drupalUid,
	        'entity_type' => 'group_users',
	        'module_name' => 'exp_sp_administration_user',
	        'functionality_name' => 'deleteGrpList',
	        'deleted_on' => now(),
	        'deleted_by' => getIdOfLoggedInUser(),
	        'parent1_entity_id' => $srleid,
	        'parent1_entity_type' => 'rid',
	        'parent2_entity_id' => $drupalUid,
	        'parent2_entity_type' => 'uid',
	         
	);
	 
	deleted_log_entry($fields);

	//slt_groups update
  $userDelete=db_query('UPDATE slt_groups SET userslist = TRIM(BOTH \',\' FROM REPLACE(
  		CONCAT(\',\', userslist, \',\'),CONCAT(\',\', '.$userId.', \',\'), \',\'))WHERE id = '.$grpId.'');
  expDebug::dPrintDBAPI('User delete query',$userDelete);
  $userDelete->execute();
  $is_admin = adminGrporNot($grpId);
  if($is_admin){
  	//Entry for securitymanagementAdmin
  	$select = db_delete('slt_admin_group_users');
  	$select->condition('group_id',$grpId);
  	$select->condition('user_id',$userId);
  	expDebug::dPrintDBAPI('delete userlist slt_admin_group_users' , $select);
  	$select->execute();
  }

  $loggeduserId = getSltpersonUserId();
  // Get the fields before deleting
  $select = db_select('slt_group_user_mapping', 'gumap');
  $select->addField('gumap','id','id');
  $select->addField('gumap','group_id','group_id');
  $select->addField('gumap','user_id','user_id');
  $select->addField('gumap','group_type','group_type');
  $select->condition('group_id',$grpId);
  $select->condition('user_id',$usrId);
  $gumapId =$select->execute()->fetchAll();
  
  // Added by Vincent on 24 Jan 2017, for #0071187
  $select = db_delete('slt_group_user_mapping');
  $select->condition('group_id',$grpId);
  $select->condition('user_id',$userId);
  expDebug::dPrintDBAPI('delete userlist slt_group_user_mapping' , $select);
  $select->execute();
  
  foreach($gumapId as $val){
      if($val->group_type == 1){
          $grp_type = 'Admin Group';
      }
      else if($val->group_type == 0){
          $grp_type = 'Learner Group';
      }
      // Make deleted log entry
      $fields = array(
              'table_name' => 'slt_group_user_mapping',
              'entity_id' => $val->id,
              'entity_type' => 'group_user_mapping',
              'parent1_entity_id' => $val->group_type,
              'parent1_entity_type' => $grp_type,
              'parent2_entity_id' => $val->group_id,
              'parent2_entity_type' => 'group_id',
              'parent3_entity_id' => $val->user_id,
              'parent3_entity_type' => 'user_id',
              'module_name' => 'exp_sp_administration_user',
              'functionality_name' => 'deleteGrpList',
              'api_name' => $_REQUEST['apiname'],
              'deleted_on' => now(),
              'deleted_by' => $loggeduserId              
      );
      deleted_log_entry($fields);
  }
  
  //To sync solr
	syncSolrData('User');
/*   //Manager Details
  if($roleId == 2){
  //Manager Details
  $select1 = db_select('slt_person','per');
	$select1->addField('per','id','id');
	$select1->condition('per.manager_id',$userId,'=');
	expDebug::dPrintDBAPI('Fetch Managerrole Id ' , $select1);
	$mgrId = $select1->execute()->fetchAll();
	expDebug::dPrint('$mgrId'.print_r($mgrId,true));
  foreach($mgrId as $persId){
		$updatemgr = db_update('slt_person');
		$updatemgr->condition('id',$persId->id,'=');
		$updatemgr->fields(array(
												'manager_id'=> NULL
												));
	  expDebug::dPrintDBAPI('$updatemgr', $updatemgr);
		$updatemgr->execute();
	}

	//Dotted manager details
	$select2 = db_select('slt_person','per');
	$select2->addField('per','id','id');
	$select2->where(" FIND_IN_SET($userId,ifnull(per.dotted_mngr_id,0))>0");
	expDebug::dPrintDBAPI('Fetch dottedmgrrole Id ' , $select2);
	$dottedMgrId = $select2->execute()->fetchAll();
	expDebug::dPrint('$dottedMgrId'.print_r($dottedMgrId,true));
	foreach($dottedMgrId as $perId){
		$dottedMgrdelete=db_query('UPDATE slt_person SET dotted_mngr_id = TRIM(BOTH \',\' FROM REPLACE(
	                          CONCAT(\',\', dotted_mngr_id, \',\'),CONCAT(\',\', '.$userId.', \',\'), \',\')) WHERE id = '.$perId->id.'');
	  expDebug::dPrintDBAPI('dotted manager query',$dottedMgrdelete);
	  $dottedMgrdelete->execute();
		}
  } */
  } catch (Exception $ex) {
  	watchdog_exception('deleteSecurityRole', $ex);
  	expertusErrorThrow($ex);
  }
}

function updatesGrpList($drupalUid, $grpId,$personId,$loggedInUserId){
	$txn = db_transaction();
	try {
		$select = db_select('role','role');
	  $select->join('slt_groups','srole','role.name=srole.name');
	  $select->addField('role','rid','rid');
		$select->condition('srole.id',$grpId,'=');
		expDebug::dPrintDBAPI('Fetch role Id ' , $select);
		$srleid = $select->execute()->fetchField();
		//Users role Insert

		$num_of_role = db_select('users_roles', 'roles');
		$num_of_role->condition('roles.uid',$drupalUid ,'=');
		$num_of_role->condition('roles.rid',$srleid ,'=');
		$num_of_role->addField('roles', 'rid','rid');
		$result_role_count =  $num_of_role->execute()->rowCount();
		if($result_role_count == 0) {
		$insertStmt = db_insert('users_roles');
		$insertStmt->fields(array(
                    'uid' => $drupalUid,
                    'rid' => $srleid,
                    ));
		expDebug::dPrintDBAPI('$insert user Id ' , $insertStmt);
		expDebug::dPrint(' $personId' . $personId , 4);
		$insertStmt->execute();
		}
		//Update security role
		$existingUsersList = getRoleUserListField($grpId);
//48587: Getting issue when the user instructor role is removed in administration/people/user
// 		$num_of_results = db_select('users_roles', 'roles');
// 		$num_of_results->condition('roles.uid',$drupalUid ,'=');
// 		$num_of_results->condition('roles.rid',$grpId ,'=');
// 		$num_of_results->addField('roles', 'rid','rid');
// 		$result_count =  $num_of_results->execute()->rowCount();


		if($result_role_count == 0) {
		$newuserList = ((empty($existingUsersList)) ? '' : $existingUsersList . ',') .$personId;
	   expDebug::dPrint(' $personId' . $newuserList , 4);
		expDebug::dPrint(' $existingUsersList = ' . print_r($existingUsersList, true) , 4);
		$updateUser = db_update('slt_groups');
	  $updateUser->condition('id', $grpId, '=');
	  $updateUser->fields(array(
	                        'userslist'       => $newuserList ,
	                        'updated_on'    => now(),
	                        'updated_by'    => $loggedInUserId
	                        ));
	  expDebug::dPrintDBAPI('Insert UserList for groups ', $updateUser);
	  $updateUser->execute();
	  
	  $grpDetails = getGrpDetails($grpId, array('code','is_admin','status'));
	  $grpcode = $grpDetails['code'];
	  $is_admin = $grpDetails['is_admin'];
	  $status = $grpDetails['status'];
	  if($is_admin==1){
	  	$insertStmt = db_insert('slt_admin_group_users');
	  	$fields = array(
	  			'group_id'          => $grpId,
	  			'user_id'			=> $personId,
	  			'group_code'		=> $grpcode,
	  			'status'			=> $status,
	  			'created_by'        => $loggedInUserId,
	  			'created_on'        => now(),
	  			'updated_by'        => $loggedInUserId,
	  			'updated_on'        => now()
	  	);
	  	$insertStmt->fields($fields);
	  	$insertStmt->execute();
	  }
		}
	}catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updatesSecurityRole', $ex);
    throw $ex;
  }
  unset($txn);
}

/*
 * addNewUser() - Save a new user's details in the database
 */
function addNewUser($form, &$form_state, $createUserId,$callFromAPI =0, $pictureId = '',$dtLd=0) {
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $pictureId = ' . $pictureId , 4);
  expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
  expDebug::dPrint(' $createUserId = ' . print_r($createUserId, true) , 4);

  $txn = db_transaction();

  try {

    $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
    $in_active_on          = $form_state['values']['status'] == 'cre_usr_sts_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'cre_usr_sts_itv' ? $inactivereasontext : NULL;
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_person');

    $status              = 'cre_usr_sts_del';
	if(!empty($form_state['values']['hire_date']) && $form_state['values']['hire_date'] != t('LBL112')){
		$old_dateformat = str_replace("-", "/", $form_state['values']['hire_date']);
    	$hireDate = date_format(date_create($old_dateformat), 'Y-m-d');
	} else {
		$hireDate = NULL;
	}
    if($callFromAPI || $dtLd == 1){
      $status             = $form_state['values']['status'];
    }
    $in_active_on = NULL;
    $fields =array();
    $fields['first_name']  = $form_state['values']['first_name'];
    $fields['last_name']   = $form_state['values']['last_name'];
    $fields['full_name']   = $form_state['values']['first_name'] . ' ' .$form_state['values']['last_name'];
    $fields['user_name']   = $form_state['values']['user_name'];
    $fields['city']        = $form_state['values']['city'];
    $fields['zip']         =  $form_state['values']['zip'];
	$fields['sms_alert']   = ($form_state['values']['register_sms']== '1') ? '1' : '0';
	$fields['hire_date']   = $hireDate;
    $fields['status']      = $status;
    $fields['in_active_on']= $in_active_on;
    $fields['email']       = $form_state['values']['email'];
    $fields['created_by']  = $createUserId;
    $fields['created_on']  = now();
    $fields['updated_by']  = $createUserId;
    $fields['updated_on']  = now();
    if($dtLd){
    	$fields['custom_dataload']  = (!empty($dtLd)) ? $form_state['values']['custom_dataload'] : null;
    }
	$enc1 = new Encrypt();
    if($callFromAPI){
    	$webex_module=getProfileListItemByCode('lrn_cls_vct_web');
    $role = $form_state['values']['roles'];
    if(($role == 'instructor' || $role == 'instructor,manager' || $role == 'manager,instructor') && variable_get('webex_User') == 1 && $webex_module[0]->status == 'Y'){
       	$fields['webex_name'] = (!empty($form_state['values']['webex_name']))? $form_state['values']['webex_name'] : null;
    	$fields['webex_pass'] = (!empty($form_state['values']['webex_pass']))? $enc1->encrypt($form_state['values']['webex_pass']) : null;
    }
    }else{
    	$fields['webex_name'] = (!empty($form_state['values']['webex_name']))? $form_state['values']['webex_name'] : null;
    	$fields['webex_pass'] = (!empty($form_state['values']['webex_pass']))? $enc1->encrypt($form_state['values']['webex_pass']) : null;
    }
	  /*
     $fields = array(
     'first_name'          => trim($form_state['values']['first_name']),
     'last_name'           => trim($form_state['values']['last_name']),
     'full_name'           => trim($form_state['values']['first_name']) . ' ' .
     trim($form_state['values']['last_name']),
     'user_name'           => $form_state['values']['user_name'],
     'city'			    => trim($form_state['values']['city']),
     'zip'  			    => trim($form_state['values']['zip']),
     'status'              => $status,
     'in_active_on'        => $in_active_on,
     'email'               => $form_state['values']['email'],
     // 'time_zone'           => $preferredTimeZone,
     // 'preferred_language'  => $preferredLanguage,
     'created_by'          => $createUserId,
     'created_on'          => now(),
     );
     */

    // Select the fields and the values to be inserted
    $details = array();
    //if($callFromAPI){/*commented for 25580*/
      if(!empty($form_state['values']['preferred_timezone'])&& isset($form_state['values']['preferred_timezone'])){
        $preferredShortTimeCode = getProfileListItemByCode($form_state['values']['preferred_timezone']);
        if($preferredShortTimeCode){
          $details['timezone']  = $preferredShortTimeCode[0]->attr2;
          $fields['time_zone']  =  $form_state['values']['preferred_timezone'];
        }
      }else{
      	$defaultTimezone = defaultSiteTimeZone($callFromAPI);
        $details['timezone']  = $defaultTimezone['sysTimezone'];
        $fields['time_zone']  = $defaultTimezone['time_zone'] ;
        expDebug::dPrint('$defaultTimezone  = ' . print_r($defaultTimezone,true),3);
      }
      if(!empty($form_state['values']['preferred_language']) && isset($form_state['values']['preferred_language'])){
        $preferredShortLangCode = getProfileListItemByCode($form_state['values']['preferred_language']);
        if($preferredShortLangCode[0]->attr1) {
          $fields['preferred_language']  =  $form_state['values']['preferred_language'];
          $details['language']           = $preferredShortLangCode[0]->attr1;
          expDebug::dPrint('$preferredShortLang  = ' . print_r($preferredShortLangCode,true),3);
        }
      }else{
        $siteLmsDefaultLanguage        = getSiteDefaultLanguageCode();
        $language                      = $GLOBALS['language']->language;
        $fields['preferred_language']  =  $siteLmsDefaultLanguage;
        $details['language']           =  $language;
        expDebug::dPrint('$defaultTimezone  = ' . print_r($siteLmsDefaultLanguage,true),3);
      }

      if(!empty($form_state['values']['preferred_currency']) && isset($form_state['values']['preferred_currency'])){
      		$fields['preferred_currency']  =  $form_state['values']['preferred_currency'];
      		$details['currency']           = $form_state['values']['preferred_currency'];
      		expDebug::dPrint('$preferred_currency Add New User Admin  = ' . print_r($fields['preferred_currency'],true),3);
      }else{

      	$siteLmsDefaultCurrency        = '';
      	$currency                      = '';
      	$fields['preferred_currency']  =  '';
      	$details['currency']           =  '';
      	expDebug::dPrint('$preferred_currency Add New User Admin  = Else');
      }
   //}/*commented for 25580*/
    expDebug::dPrint('$preferredShortTime = ' . print_r($preferredShortLangCode,true),3);

    //expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);

    // Execute the insert statement. Fetch the new user's id in slt_person table.
    $fields = trimInputFields($fields); // #42124 - Trim input values
    $insertStmt->fields($fields);
    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);
    $personId = $insertStmt->execute();
    expDebug::dPrint(' $personId = ' . print_r($personId, true) , 3);
    if($callFromAPI) {
    	$classCreateUser = $createUserId;
    }
    else {
    	$classCreateUser = getIdOfLoggedInUser();
    }
    // Save the new user's slt_person table id in the hidden id form field.
    $form_state['values']['id'] = $personId;
    updateGroupMappingDetails($personId, 'cre_usr', $classCreateUser,'emptyadd',0);
    // Create the user in drupal

    $details['name']     = $form_state['values']['user_name'];
    $details['pass']     = $form_state['values']['password'];
    $details['mail']     = $form_state['values']['email'];
    $details['status']   = (($status == 'cre_usr_sts_atv')? 1 : 0);

    /*$details = array(
     'name' => $form_state['values']['user_name'],
     'pass' => $form_state['values']['password'],
     'mail' => $form_state['values']['email'],
     'status' => (($status == 'cre_usr_sts_atv')? 1 : 0),
     );*/
    $details = trimInputFields($details); // #42124 - Trim input values
    $account = user_save(drupal_anonymous_user(), $details);
    expDebug::dPrint(' drupal uid = ' . print_r($account->uid, true) , 4);

    // Save the new user's Drupal users table id in the hidden drupal_uid form field.
    $form_state['values']['drupal_uid'] = $account->uid;

  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('addNewUser', $ex);
    throw $ex;
  }

  // Commit the transaction
  unset($txn);

  // If notify_user checkbox is checked, send a new_user notification to the newly added user
  //if ($form_state['values']['notify_user'] == 1) {
  //sendResetPasswordLink('register_admin_created', $form_state['values']['email']);
  //expDebug::dPrint('exp_sp_administration_user.inc : addNewUser() : sending new user notification for $personId = ' . $personId);
  //sendNewUserNotification($personId, $createUserId);
  //}

  // Upload and save user's picture
  //uploadUserPicture('picture', $form_state['values']['drupal_uid'], 1);
  // $userPic = $form_state['storage']['filepicture'];
  // uploadUserPicture($userPic, $form_state['values']['drupal_uid']);
  return $personId;
}
function resetUserPassword($user_email){
	try {
  if(isset($user_email) && !empty($user_email)){
    sendResetPasswordLink('password_reset', $user_email);
    print t('LBL580').'|'.t('LBL581');
  }
  } catch (Exception $ex) {
  	watchdog_exception('resetUserPassword', $ex);
  	expertusErrorThrow($ex);
  }
}
/**
 * Function to send reset password link to users from admin when admin creates a new user or changes password of an existing user
 */
function sendResetPasswordLink($op, $user_email) {
	try {
  global $language;
  expDebug::dPrint(' $op = '.$op, 4);
  expDebug::dPrint(' $user_email = '.$user_email , 4);
  $users = user_load_multiple(array(), array('mail' => $user_email, 'status' => '1'));
  $account = reset($users);
  expDebug::dPrint(' $account = ' . print_r($users, true) , 4);
  // Mail one time login URL and instructions using current language.
  _format_user_mail_notify($op, $account, $language);
  } catch (Exception $ex) {
  	watchdog_exception('sendResetPasswordLink', $ex);
  	expertusErrorThrow($ex);
  }
}


/**
 * getDrupalUserAvator() - Function to get drupal user avator
 * @param Drupal Id
 * @return Picture
 */
function getDrupalUserAvator($drupalId){
  /*
   $select = db_select('users', 'drupal_users');
   $select->addField('drupal_users','picture', 'person_avatar');
   $select->condition('drupal_users.uid', $drupalId);
   return $select->execute()->fetchField();
   */
	try {
  $account    = user_load($drupalId,TRUE);
  $filepath = '';
  if(!empty($account->picture)){
    $filepath  = $account->picture->uri;
  }
  return $filepath;
  } catch (Exception $ex) {
  	watchdog_exception('getDrupalUserAvator', $ex);
  	expertusErrorThrow($ex);
  }
}

function updateUserBasicDetails($form, $form_state, $updateUserId, $callFromAPI = 0, $pictureId = '',$dtLd=0) {
try {
	global $userSSOAttributes;
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  //expDebug::dPrint('exp_sp_administration_user.inc : updateUserBasicDetails() : $form_state = ' . print_r($form_state, true));
  expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
  expDebug::dPrint(' $createUserId = ' . print_r($updateUserId, true) , 4);
  expDebug::dPrint(' $_REQUEST updateUserBasicDetails= ' . print_r($_REQUEST, true) , 4);
  expDebug::dPrint(' $$userSSOAttributes $updateUserBasicDetails= ' . print_r($userSSOAttributes, true) , 4);
  $personid = $form_state['values']['id'];
  $currstatus = getPersonDetails($personid);
  expDebug::dPrint(' $$userSSOAttributes $updateUserBasicDetailsSAML= ' . print_r($currstatus, true) , 4);
  $txn = db_transaction();
  $attrChangeFlag = true;
  if(isset($form_state['values']['request_source']) && $form_state['values']['request_source'] == 'saml' && $_SESSION['saml_auth'] == 1){
  $fieldformat = array( 
  		'first_name'=>'first_name',
  		'last_name'=> 'last_name',
  		'user_name'=>'user_name',
  		'email'=>'email',
  		'addr1' => 'addr1',
    	'addr2' => 'addr2',
    	'country' => 'country',
    	'state' => 'state',
    	'city' => 'city', 
    	'zip' => 'zip', 
    	'phone_no' => 'phone_no',
  		'org_id' => 'org_id',
  		//'job_role' => 'jobrole',
  		'jobtitle' => 'job_title',
  		'empltype' => 'employment_type',
  		'employee_no'=> 'employee_no',
  		'deptcode' => 'dept_code',
  		'usertype' => 'user_type',
  		'manager_id' => 'manager_id',
  		'preferred_timezone' =>'time_zone',
  		
  );
  
  // check whether to update or not
  $attrChangeFlag = false;
  $job_role = getUserJobrole($form_state['values']['id']);
  $jr_str = array();
  expDebug::dPrint(' $keychanged = ' . print_r(count($job_role),true) , 5);
  foreach ($job_role as $jobbb=>$val){
  	array_push($jr_str,$val->job_role);
  }
  $job_st = implode(',',$jr_str);
  expDebug::dPrint(' $keychangedssss = ' . print_r($jr_str,true) , 5);
  expDebug::dPrint(' $keychangedssss = ' . $job_st , 5);
 
  if(!empty($form_state['values']['jobrole']) && $form_state['values']['jobrole'] != $job_st){
  	$attrChangeFlag = true;
  	
  }
  else {
  foreach ($form_state['values'] as $key=>$val){
  	if(array_key_exists($key,$fieldformat)){
  		if(!empty($form_state['values'][$key]) && $form_state['values'][$key] != $currstatus[$fieldformat[$key]]){
  			expDebug::dPrint(' $keychanged = ' . $key , 5);
  			expDebug::dPrint(' $$from_per_det = ' . $form_state['values'][$key] , 5);
  			expDebug::dPrint(' $$from_per_det = ' . $currstatus[$fieldformat[$key]] , 5);
  			$attrChangeFlag = true;
  			break;
  		}
  	}
  }
  
  }
  }
 else {
  	$fieldformat = array(
  		'first_name'=>'first_name',
  		'last_name'=> 'last_name',
  		'user_name'=>'user_name',
  		'email'=>'email',
  		'addr1' => 'addr1',
    	'addr2' => 'addr2',
    	'country' => 'country',
    	'state' => 'state',
    	'city' => 'city', 
    	'zip' => 'zip', 
    	'phone_no' => 'phone_no',
  		'mobile_no'=>'mobile_no',
  		'org_id' => 'org_id',
  		'jobtitle' => 'job_title',
  		'empltype' => 'employment_type',
  		'employee_no'=> 'employee_no',
  		'deptcode' => 'dept_code',
  		'usertype' => 'user_type',
  		'manager_id' => 'manager_id',
  		'preferred_timezone' =>'time_zone',
  		/** additional fields for API **/
  		// 'password'=>
  		'hire_date'=>'hire_date',
  		'webex_name'=>'webex_name',
  		'webex_pass'=>'webex_pass',
 		'preferred_language'=> 'preferred_language',
  		'preferred_currency'=>'preferred_currency',
  		'register_sms'=> 'sms_alert',
  		/** additional fields for dataload **/
  		'zip_code'=> 'zip',
  		'contact_no'=> 'phone_no',
  		'timezone' => 'time_zone',
  		'org_number'=>'org_id',
  		//'manager_username'
  		'empl_type_code' => 'employment_type',
  		'dept_code' => 'dept_code',
  		'job_title_code'=> 'job_title',
  		'user_type'=> 'user_type',
  		'is_manager' => 'is_manager',
  		'is_instructor' => 'is_instructor', 
  		'sms_alert'=> 'sms_alert',
  		'is_mobileadmin'=> 'is_mobileadmin',
  	
  	//'groups'
  	/** addtional fields for ADMIN UI edit **/
  		
  			
  		
  );
  
  // check whether to update or not
  $attrChangeFlag = false;
  
  /** UI , some fields are fetched with title instead of code **/
  if(isset($form_state['values']['form_id']) && !empty($form_state['values']['form_id']) && $form_state['values']['form_id'] == 'exp_sp_administration_user_basic_addedit_form'){
  	$fetched_name = array('empltype'=>'employment_type',
  			'deptcode'=>'dept_code',
  			'jobtitle'=>'job_title',
  			'usertype'=>'user_type');
  	foreach($fetched_name as $key=>$val){
  		$default = getProfileListItemByCode($currstatus[$fetched_name[$key]]);
  		expDebug::dPrint(' $keyvalue in admin ui edit = ' .$key , 5);
  		expDebug::dPrint(' $keyvalue in admin ui edit = ' .$fetched_name[$key] , 5);
  		expDebug::dPrint(' $keyvalue in admin ui edit = ' .$currstatus[$fetched_name[$key]] , 5);
  		expDebug::dPrint(' $keyvalue in admin ui edit = ' .$default[0]->name , 5);
  		expDebug::dPrint(' $keyvalue in admin ui edit = ' .$form_state['values'][$key] , 5);
  		if($default[0]->name != $form_state['values'][$key]){
  			$attrChangeFlag = true;
  			break;
  		}
  	}
  	unset($fieldformat['empltype']);
  	unset($fieldformat['deptcode']);
  	unset($fieldformat['jobtitle']);
  	unset($fieldformat['usertype']);
  }
  
   /** UI , some fields are fetched with title instead of code **/
  
  /** Jobrole check starts here **/
  if($attrChangeFlag == false){
  	expDebug::dPrint('Job role is checked here');
			$job_role = getUserJobrole ($personid);
			$jr_str = array ();
			foreach ($job_role as $jobbb => $val) {
				array_push ($jr_str,$val->job_role);
			}
			$job_st = implode ( ',', $jr_str );
			if ((isset( $form_state ['values'] ['jobrole'] ) && ! empty ( $form_state ['values'] ['jobrole'] ) && $form_state ['values'] ['jobrole'] != $job_st) || (isset ( $form_state ['values'] ['load_multiselect_jobrole'] ) && $form_state ['values'] ['load_multiselect_jobrole'] != $job_st) || (isset ( $form_state ['values'] ['job_role_code'] ) && $form_state ['values'] ['job_role_code'] != $job_st))
				$attrChangeFlag = true;
		}
  /** Jobrole check ends here **/
  
 /** other managers check ends here **/
  if($attrChangeFlag == false){
  	expDebug::dPrint('other manager is checked here');
  	$oth_manager = !empty($currstatus['dotted_mngr_id']) ? explode(", ", $currstatus['dotted_mngr_id']) : '';
  	expDebug::dPrint('other manager from user profile'.print_r($oth_manager,true),5);
  	$oth_mgr = ($callFromAPI==1) ? explode(",",$form_state['values']['other_managers']) : explode(",",$form_state['values']['load_multiselect_omanagers']);
  	expDebug::dPrint('other manager from formstate'.print_r($oth_mgr,true),5);
  	if(!empty($oth_manager) && $oth_mgr[0] != ''){
  		expDebug::dPrint('both formstate and field values exist');
  	$oth_mgr_ids = array();
  	foreach($oth_manager as $key=>$val){
  		$selectStmt = db_select('slt_person', 'per');
  		$selectStmt->addField('per', 'id', 'id');
  		$selectStmt->condition('per.user_name', $val);
  		expDebug::dPrintDBAPI(' $selectStmt obj aasdas=' , $selectStmt);
  		$oth_mgr_id = $selectStmt->execute()->fetchField();
  		array_push($oth_mgr_ids, $oth_mgr_id);
  	}
  	if(!check_two_array_equal($oth_mgr,$oth_mgr_ids))
  			$attrChangeFlag = true;
  	}
  	else if (($oth_mgr[0]== '' && !empty($oth_manager)) || (empty($oth_manager) && $oth_mgr[0] != ''))
  			$attrChangeFlag = true;
  }
  /** other manager check ends here **/
  
  /** other org check starts here **/
  if($attrChangeFlag == false){
  	expDebug::dPrint('other organisation is checked here');
  		$oth_org_names = !empty($currstatus['dotted_org_id']) ? explode(", ", $currstatus['dotted_org_id']) : '' ;
  		expDebug::dPrint('other organisation from user profile'.print_r($oth_org_names,true),5);
  		$oth_org_value = ($callFromAPI==1) ? $form_state['values']['other_organization'] : (isset($form_state['values']['other_org_number']) ? $form_state['values']['other_org_number'] : (isset($form_state['values']['load_multiselect_oorgs']) ? $form_state['values']['load_multiselect_oorgs'] : ''));
  		$oth_org = explode(",",$oth_org_value);
  		expDebug::dPrint('other organisation from formstate'.print_r($oth_org,true),5);
  		if(!empty($oth_org_names) && $oth_org[0] != ''){
  				$oth_org_ids = array();
  				foreach($oth_org_names as $key=>$val){
  					$selectStmt = db_select('slt_organization', 'org');
  					$selectStmt->addField('org', 'id', 'id');
  					$selectStmt->condition('org.name', $val);
  					expDebug::dPrintDBAPI(' $selectStmt obj aasdassssss=' , $selectStmt);
  					$oth_org_id = $selectStmt->execute()->fetchField();
  					array_push($oth_org_ids, $oth_org_id);
  				}
  	
  				if(!check_two_array_equal($oth_org_ids,$oth_org))
  					$attrChangeFlag = true;
  			}
  		else if (($oth_org[0] == '' && !empty($oth_org_names)) || (empty($oth_org_names) && $oth_org[0] != ''))
  			$attrChangeFlag = true;
  
 }
 /** other org check ends here **/

 /** roles check starts here **/
  if($attrChangeFlag == false && !empty($form_state['values']['roles'])){
  	expDebug::dPrint('role is checked here');
  		$roles_up = is_array($form_state['values']['roles']) ? implode(',',$form_state['values']['roles']) : $form_state['values']['roles'];
  		
  	if((strpos($roles_up,'instructor') !== false && $currstatus['is_instructor'] != 'Y') || (strpos($roles_up,'instructor') === false && $currstatus['is_instructor'] == 'Y'))
  		$attrChangeFlag = true;
  	else if((strpos($roles_up,'manager') !== false && $currstatus['is_manager'] != 'Y') || (strpos($roles_up,'manager') === false && $currstatus['is_manager'] == 'Y'))
  		$attrChangeFlag = true;
  	expDebug::dPrint('change in role is there');
  }
   /** roles check ends here **/
  
  /** manager_username field check starts here -- user dataload **/
  if($attrChangeFlag == false && isset($form_state['values']['manager_username'])){
  	expDebug::dPrint('manager user name is checked here');
  		$manager_id = $currstatus['manager_id'];
  		if((!empty($form_state['values']['manager_username']) && empty($manager_id)) || (empty($form_state['values']['manager_username']) && !empty($manager_id))){
  			$attrChangeFlag = true;
  		}
  		else {
  			$selectStmt = db_select('slt_person', 'per');
  			$selectStmt->addField('per', 'id', 'id');
  			$selectStmt->condition('per.user_name', $form_state['values']['manager_username'],'=');
  			expDebug::dPrintDBAPI(' $selectStmt obj aasdas=' , $selectStmt);
  			$mgr_id = $selectStmt->execute()->fetchField();
  			if($manager_id != $mgr_id)
  				$attrChangeFlag = true;
  		}
  }
  /** manager_username field check ends here -- user dataload **/
  
  
  /** groups -- dataload check starts here **/
  if($attrChangeFlag == false && isset($_form_state['values']['groups']) && !empty($_form_state['values']['groups'])){
  	$grp_form_value = explode(",",$_form_state['values']['groups']);
  	$selectStmt = db_select('slt_groups', 'grp');
  	$selectStmt->addField('grp', 'name', 'name');
  	$selectStmt->join('slt_group_user_mapping','grpmap','grpmap.group_id = grp.id and grpmap.user_type != "R"');
  	$selectStmt->condition('grpmap.user_id',$personid,'=');
  	$selectStmt->groupBy('grp.id');
  	expDebug::dPrintDBAPI(' $selectStmt obj aasdas grppsss=' , $selectStmt);
  	$grp_of_user = $selectStmt->execute()->fetchCol();
  	expDebug::dPrint(' $result_grpousssss '.print_r($result,true),4);
  	if(!check_two_array_equal($grp_form_value,$grp_of_user))
  		$attrChangeFlag = true;
  	
  }
  /** groups -- dataload check ends here **/
  
  /** hire date check starts here **/
  if($attrChangeFlag == false && isset($form_state['values']['hire_date'])){
  	expDebug::dPrint('Hire date is checked here');
  	if($form_state['values']['hire_date'] == 'mm-dd-yyyy')
  		$form_state['values']['hire_date'] = '';
  	if($form_state['values']['hire_date'] != $currstatus['hire_date'])
  		$attrChangeFlag = true;
  	unset($fieldformat['hire_date']);
  }
  /** other attribute check starts here **/
 if($attrChangeFlag == false) {
  foreach ($form_state['values'] as $key=>$val){
  	if(array_key_exists($key,$fieldformat)){
  		if(!empty($form_state['values'][$key]) && $form_state['values'][$key] != $currstatus[$fieldformat[$key]]){
  			expDebug::dPrint(' $keychanged = ' .$key , 5);
  			expDebug::dPrint(' $keychanged = ' .$form_state['values'][$key] , 5);
  			expDebug::dPrint(' $keychanged = ' .$currstatus[$fieldformat[$key]] , 5);
  			$attrChangeFlag = true;
  			break;
  		}
  	}
  }
  
  }
  /** other attribute check ends here **/
}
  expDebug::dPrint(' value of $attrChangeFlag ' . print_r($attrChangeFlag, true) , 4);
  if($attrChangeFlag){
  try {
    $default= db_query("SELECT id,first_name,middle_name,last_name,user_name,hire_date,email,status,time_zone,preferred_language,webex_name,webex_pass from slt_person where id=:uid",array(":uid"=>$personid))->fetchAll();
    
    $custom0 = LdapFetchUSerDetails($form_state['values']['id']);
    $customfield = $custom0[0]->custom0;
    if($custom0[0]->first_name != $form_state['values']['first_name']){
      if(strpos($customfield,'firstname') === false){
        $customfield .= ($customfield) ? ',firstname' : 'firstname';
      }
    }
    if($custom0[0]->last_name != $form_state['values']['last_name']){
      if(strpos($customfield,'lastname') === false){
        $customfield .= ($customfield) ? ',lastname' : 'lastname';
      }
    }
    //$status = 'cre_usr_sts_atv';
    $in_active_on = NULL;

    $inactivereasontext    = empty($form_state['values']['in_active_reason']) ? NULL : $form_state['values']['in_active_reason'];
    $in_active_on          = $form_state['values']['status'] == 'cre_usr_sts_itv' ? now() : NULL;
    $in_active_reason      = $form_state['values']['status'] == 'cre_usr_sts_itv' ? $inactivereasontext : NULL;
    if($callFromAPI == 0 || ($callFromAPI == 1 && isset($_REQUEST['hire_date']))){
	if(!empty($form_state['values']['hire_date']) && $form_state['values']['hire_date'] != t('LBL112')){
		$old_dateformat = str_replace("-", "/", $form_state['values']['hire_date']);
	    $hireDate = date_format(date_create($old_dateformat), 'Y-m-d H:i:s');
	} else {
		$hireDate = NULL;
	}
    }elseif($callFromAPI == 1 && (!isset($_REQUEST['hire_date']))){
    	$hireDate = $default[0]->hire_date;
    }
    // Select the table to insert the record in
    $updateStmt = db_update('slt_person');
    $fields                = array( );
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['first_name']) || isset($userSSOAttributes['FirstName']))))
    $fields['first_name']  = $form_state['values']['first_name'];
			
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['last_name']) || isset($userSSOAttributes['LastName']))))
    $fields['last_name']   = $form_state['values']['last_name'];
			
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['user_name']) || isset($userSSOAttributes['UserName']))))
    $fields['user_name']   = $form_state['values']['user_name'];
	$fields['hire_date']   = $hireDate;
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['email']) || isset($userSSOAttributes['Email']))))
				$fields['email'] = $form_state['values']['email'];
			
    $fields['in_active_on']= $in_active_on;
			
			if(! empty($form_state['values']['first_name']) && (! empty($form_state['values']['last_name']))) {
				$fields['full_name'] = $form_state['values']['first_name'] . ' ' . $form_state['values']['last_name'];
			} else {
				$fields['full_name'] = ($default['first_name'] . '' . $default['last_name']);
			}
    
    $fields['time_zone']       = $form_state['values']['preferred_timezone'];
   // $fields['preferred_language']  = $form_state['values']['preferred_language'];
    $fields['updated_by']  = $updateUserId;
    $fields['updated_on']  = now();
    $fields['custom0']     = $customfield;
    if($dtLd){
    	$fields['custom_dataload']  = (!empty($dtLd)) ? $form_state['values']['custom_dataload'] : null;
    }

	$enc1 = new Encrypt();
    if($callFromAPI){
    	$webex_module=getProfileListItemByCode('lrn_cls_vct_web');
    	$role = $form_state['values']['roles'];
    	if(($role == 'instructor' || $role == 'instructor,manager' || $role == 'manager,instructor') && variable_get('webex_User') == 1 && $webex_module[0]->status == 'Y'){
    		if(isset($_REQUEST['webex_name']))
    		$fields['webex_name'] = (!empty($form_state['values']['webex_name']))? $form_state['values']['webex_name'] : null;
    		if(isset($_REQUEST['webex_pass']))
    		$fields['webex_pass'] = (!empty($form_state['values']['webex_pass']))? $enc1->encrypt($form_state['values']['webex_pass']) : null;
    	}
    }else{
    	$fields['webex_name']  = !empty($form_state['values']['webex_name']) ? $form_state['values']['webex_name'] : null;
    	if(empty($form_state['values']['webex_pass']) && !empty($form_state['values']['webex_name']))
    	$fields['webex_pass'] = $form['webex_pass']['#default_value'];
    	else if(!empty($form_state['values']['webex_pass']) && !empty($form_state['values']['webex_name']))
    	$fields['webex_pass'] = $form_state['values']['webex_pass'];
		else if(empty($form_state['values']['webex_name']))
		$fields['webex_pass'] = null;
    }    
    /* $fields = array(
     'first_name'      => trim($form_state['values']['first_name']),
     'last_name'       => trim($form_state['values']['last_name']),
     'full_name'       => trim($form_state['values']['first_name']). ' ' .
     trim($form_state['values']['last_name']),
     //'status'          => $status,
     'in_active_on'    => $in_active_on,
     'email'           => $form_state['values']['email'],
     'time_zone'           => $preferredTimeZone,
     'preferred_language'  => $preferredLanguage,
     'updated_by'      => $createUserId,
     'updated_on'      => now(),
     'custom0'	      => $customfield
     );*/

    // Select the fields and the values to be inserted



    // Update the user in drupal
    $details = array();
    //if($callFromAPI){ /*commented for 25580*/
    //Query for timezone and preferred language update
    $selectdbstmt = db_select('slt_person', 'per');
   	$selectdbstmt->addField('per','time_zone', 'time_zone');
   	$selectdbstmt->addField('per','preferred_language', 'preferred_language');
   	$selectdbstmt->condition('per.id', $form_state['values']['id']);
   	expDebug::dPrintDBAPI(' $selectdbstmt--->> ' , $selectdbstmt);
   	$resultstmt =  $selectdbstmt->execute()->fetchAll();
   	
   	if(empty($form_state['values']['preferred_timezone']) ){
   		if(isset($form_state['values']['request_source']) && $form_state['values']['request_source'] == "saml"){
   		   unset( $fields['time_zone']);
   		}
   	}
   	expDebug::dPrint('$resultstmt---->>' . print_r($resultstmt,true),5);
   	expDebug::dPrint('$resultstmtasasas---->>' . $resultstmt[0]->time_zone,5);
      drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
      $drupalUserId = getDrupalIdOfUser($form_state['values']['id']);
      if(isset($form_state['values']['preferred_timezone'])&& !empty($form_state['values']['preferred_timezone'])){
        $preferredShortTimeCode = getProfileListItemByCode($form_state['values']['preferred_timezone']);
        if($preferredShortTimeCode){
          $details['timezone']  = $preferredShortTimeCode[0]->attr2;
          $fields['time_zone']  =  $form_state['values']['preferred_timezone'];
        }
      }else{
      	if(empty($resultstmt[0]->time_zone)){
	        $sysTimezone = date_default_timezone(FALSE);
	        $selTimeZone = db_select('slt_profile_list_items', 'prf');
	        $selTimeZone->addField('prf','code','timezone_code');
	        $selTimeZone->condition('prf.attr2', $sysTimezone);
	        $defaultTimezone = $selTimeZone->execute()->fetchField();
	        expDebug::dPrintDBAPI(' $selTimeZone obj = ' , $selTimeZone);
	        $details['timezone']  = $sysTimezone;
	        $fields['time_zone']  = $defaultTimezone ;
	        expDebug::dPrint('$defaultTimezone  = ' . print_r($defaultTimezone,true),3);
      	}
      }
// Modified 66144 for 75079
      if($callFromAPI == 0 || ($callFromAPI == 1 && isset($_REQUEST['preferred_language']))) {
			$userlanguage = trim($form_state['values']['preferred_language']);
			$preferredShortLangCode = getProfileListItemByCode($form_state['values']['preferred_language']);
				  if($preferredShortLangCode[0]->attr1) {
                    $fields['preferred_language']  =  $form_state['values']['preferred_language'];
                        $details['language']    = $preferredShortLangCode[0]->attr1;
                         expDebug::dPrint('$preferredShortLang  = ' . print_r($preferredShortLangCode,true),3);
				}else{
			      	$siteLmsDefaultLanguage        = getSiteDefaultLanguageCode();
			        $language                      = $GLOBALS['language']->language;
			        $fields['preferred_language']  =  $siteLmsDefaultLanguage;
			        $details['language']           =  $language;
			        expDebug::dPrint('$defaultTimezone  = ' . print_r($siteLmsDefaultLanguage,true),3);
		      	}
		      }

    //}/*commented for 25580*/
    // Execute the update statement.
    $fields = trimInputFields($fields); // #42124 - Trim input values
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['id']);
    $numUpdated = $updateStmt->execute();
    expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
    expDebug::dPrint(' $numUpdated = ' . print_r($numUpdated, true) , 3);

    $drupalUserId = ($callFromAPI || $dtLd) ? $drupalUserId : $form_state['values']['drupal_uid'];
    $userAccount = user_load($drupalUserId);
    expDebug::dPrint(' $userAccount = ' . print_r($userAccount, true) , 4);

    // Added by Vincent for performance imporvement on Sep 2, 2015
    clearUserSessionVariables($form_state['values']['drupal_uid']);

    // Prepare list of fields and their values for updating in drupal.

    if (!empty($form_state['values']['password'])) {
      $details['pass'] = $form_state['values']['password'];
    }
    $details['mail'] = $form_state['values']['email'];

    if(!empty($form_state['values']['status']))
    $details['status'] = (($form_state['values']['status'] == 'cre_usr_sts_atv')? 1 : 0);

    $details['name'] = $form_state['values']['user_name'];

    $updateUserId = ($updateUserId == 'Feed') ? 1 : $updateUserId;
    // Update the user in drupal
    $details = trimInputFields($details); // #42124 - Trim input values
    $updatedUserAccount = user_save($userAccount, $details);
    expDebug::dPrint(' $updatedUserAccount = ' . print_r($updatedUserAccount, true) , 4);
    expDebug::dPrint(' Fields list ' . print_r($fields, true),4);

    make_audit_trail_entries($fields, $updateUserId , $default ,$personid, 'Updated','cre_sys_obt_usr','exp_sp_administration_user','updateUserBasicDetails',
	array('first_name', 'middle_name', 'last_name', 'hire_date','email','user_name','time_zone','preferred_language','webex_name','webex_pass'));
  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('useradmin', $ex);
    throw $ex;
  }

  // Commit the transaction
  unset($txn);
  /*
   if($form_state['values']['drupal_uid']){
   // Upload and save user's picture, if one is provided
   // $_FILES['files']['name']['picture'] = $userPic;
   // $_FILES['files'] = $form_state['storage']['file_picture'];

   // $userPic = $form_state['storage']['filepicture'];

   // $userPic = $form_state['storage']['filepicture'];
   // uploadUserPicture($userPic, $form_state['values']['drupal_uid']);
   uploadUserPicture('picture', $form_state['values']['drupal_uid'], 1);
   }
   */

  $formValues     = $form_state['values'];
  $RemovePhoto = $formValues['rem_link'];

  //To delete the picture if the checkbox is checked
  if($RemovePhoto == 1){
    //if ($form_state['values']['rem_link'] == 1) {
  		$account = user_load($drupalUserId, TRUE);
  		expDebug::dPrint('debugg userr accounttt '.print_r($account,true) , 4);
  		// Reduce the file usage count
  		file_usage_delete($account->picture, 'user', 'user', $drupalUserId);

  		// Remove file reference from users table record
  		$account = user_save($account, array('picture' => 0));
  }

  // If the password is changed, update password history table.
  // Send reset_pwd notification, if the notify_user checkbox is set.
  if (!empty($form_state['values']['password'])) {
    updatePasswordPolicyHistory($drupalUserId, $updatedUserAccount->pass); // Save the encrypted password
  }

  if ($form_state['values']['notify_user'] == 1) {
    expDebug::dPrint('exp_sp_administration_user.inc : updateUserBasicDetails() : sending reset_pwd notification for slt_person id = ' .
    $form_state['values']['id']);
    sendResetPasswordLink('password_reset', $form_state['values']['email']);
  }
  if(!empty($form_state['values']['user_name']) && $default[0]->user_name != $form_state['values']['user_name']){
  	insertChangeUserNameNotification('change_username',$default,$form_state['values'],$form_state['values']['id']);
  }


  return $form_state['values']['id'];
  }
  else {  
  	if (!empty($form_state['values']['password'])) {  	
  	try{
  		/* 0089342: Can't update user's password from user profile.*/
  		$drupalUserId = ($callFromAPI || $dtLd) ? $drupalUserId : $form_state['values']['drupal_uid'];
  		$userAccount = user_load($drupalUserId);  	
  				  		
  		if (!empty($form_state['values']['password'])) {
  			$fields['pass'] = $form_state['values']['password'];
  		}
  		$fields['mail'] = $form_state['values']['email'];
  		
  		if(!empty($form_state['values']['status']))
  			$details['status'] = (($form_state['values']['status'] == 'cre_usr_sts_atv')? 1 : 0);
  		
  		$fields['name'] = $form_state['values']['user_name'];
  		
  		$updateUserId = ($updateUserId == 'Feed') ? 1 : $updateUserId;
  			// Update the user in drupal
  		$fields = trimInputFields($fields); // #42124 - Trim input values
  		$updatedUserAccount = user_save($userAccount, $fields);
  		
  			/* 0089342: Can't update user's password from user profile.*/
  		if (!empty($form_state['values']['password'])) {
  				updatePasswordPolicyHistory($drupalUserId, $updatedUserAccount->pass); // Save the encrypted password
  		}  	  	

  			
  	}catch(Exception $ex){
  		watchdog_exception('updateUserBasicDetails', $ex);
  		expertusErrorThrow($ex);
  	}
  	}
  	$no_change = 'No change required';
  	return $no_change;
  	
  }
  } catch (Exception $ex) {
  	watchdog_exception('updateUserBasicDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

function userAddEditGetPasswordPolicyRestrictionsHTML($drupalUid) {
	try {
  $passwordPolicyRestrictionsHTML = '';
  if (variable_get('password_policy_show_restrictions', 0)) {
    if (empty($drupalUid)) {
      $policy = _password_policy_load_active_policy(array(DRUPAL_ANONYMOUS_RID));
    }
    else {
      $policy = _password_policy_load_active_policy(array(DRUPAL_AUTHENTICATED_RID));
    }


    $translate = array();
    if (!empty($policy['policy'])) {
      // Some policy constraints are active.
      foreach ($policy['policy'] as $key => $value) {
        $translate['constraint_'. $key] = _password_policy_constraint_error($key, $value);
      }
    }

    // Printing out the restrictions.
    if (!empty($translate) && isset($translate)) {
      $passwordPolicyRestrictionsHTML = '<div class="addedit-password-note-cls password-note-cls">' .
                                          '<h4>' . t('LBL185') . ':</h4>' .
                                            '<ul>' .
                                              '<li>' .
      implode('</li><li>', $translate) .
                                              '</li>' .
                                            '</ul>' .
                                        '</div>';
    }
  }

  return $passwordPolicyRestrictionsHTML;
  } catch (Exception $ex) {
  	watchdog_exception('userAddEditGetPasswordPolicyRestrictionsHTML', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * userAddEditFormWithCancelAndSaveClose() - Customized closure for newAddEditFormWithCancelAndSave() to show Note in the same row as buttons
 */
function userorganizationAddEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $passwordPolicyRestrictionsHTML, $userDetails = array(),$emptyId) {
	try {
	global $theme_key;
  expDebug::dPrint(' userAddEditFormWithCancelAndSaveClose() called.');

  $customHiddenButtonId = 'last-button-account';
  $esignHiddenButt = 'esign-'.$wrapperId;
  $esignHiddenButt1= 'status-upd-esign-'.$wrapperId;

  $ajaxCallbackCancel = 'addEditCancelRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId);

  // For common styling of all addedit cancel buttons
  // $cancelAttributes['class'][] = 'addedit-edit-cancel';
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg white-btn-bg-middle';
  $cancelAttributes['tabindex'][] = '27';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  // Should be closed always - Ticket # 13936
  //if(empty($userDetails)){
  $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
  //}

  $setclass1 = ($theme_key=='expertusoneV2')?'':(empty($userDetails['id']))?'resize-save_btn':'resize-save_btn4';
  $form[$cancelButtonId] = array(
    '#type' => 'submit',
     '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions '.$setclass1.'"><div class="white-btn-bg-left"></div>',
        '#suffix' =>   '<div class="white-btn-bg-right"></div>',
    //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
    '#value' => t('LBL123'),
    '#attributes' => $cancelAttributes,
  );


  // Add the Save button
  $ajaxCallbackSave = 'addEditRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId);

  // For common styling of all addedit save buttons
  //$saveAttributes['class'][] = 'addedit-edit-save';
  $saveAttributes['class'][] = 'admin-save-button-middle-bg';
  $saveAttributes['tabindex'][] = '26';
  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $saveAttributes['data-wrapperid'] = array($wrapperId);


  $eSignatureAvailable = true;

  if(!empty($userDetails)){
    $saveAttributes['class'][] = 'edit-catalog-course-save-publish';
    $setclass = ($theme_key=='expertusoneV2')?'admin-save-button-left-bg':'';
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
      $form[$saveButtonId] = array(
    	      '#type' => 'submit',
    	      '#value' => t('LBL141'),
              '#name' => 'save',
      /*'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
       '#suffix' =>
       '<div class="admin-save-button-right-bg"></div></div>' .
       '<div class="clearBoth"></div></div>' .
       '<div class="clearBoth"></div></div>',*/
              // '#prefix' => '<div class="admin-save-pub-unpub-button-container">',
      		'#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="'.$setclass.'"></div>',
              '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
      	      '#id' =>$saveButtonId,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'customButtonId'=>$esignHiddenButt,
    		      'keypress' => true,
    		      'progress' => array(
    		        'type' => 'throbber',
    		        'message' => ''
    		        ),
    		        )
    		        );
    }
    else{
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
      $form[$saveButtonId] = array(
        '#type' => 'button',
      //'#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      /*'#suffix' =>
       '<div class="admin-save-button-right-bg"></div></div>' .
       '<div class="clearBoth"></div></div>' .
       '<div class="clearBoth"></div></div>',*/
        '#prefix' => '<div class="admin-save-pub-unpub-button-container"><div class="'.$setclass.'"></div>',
        '#suffix' => '<span id="pub-unpub-action-btn" onclick="displayPubActionList()" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn">&nbsp;</span></div>',
          '#value' => t('LBL141'),
        	'#name' => 'save',
        '#attributes' => $saveAttributes,
      );
    }
    unset($saveAttributes['tabindex']);
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
    	      '#suffix' => '</span>',
    	      '#id' =>$esignHiddenButt,
      		  '#attributes' => $saveAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
      			  'userActionStatus'=>'',
    		      'keypress' => true,
    		      'progress' => array(
    		      'type' => 'throbber',
    		      'message' => ''
    		      ),
    		      )
    		      );

    		      $form['addedit-button-group-ul-start'] = array(
          '#type' => 'markup',
          '#markup' => '<ul class="catalog-pub-add-list">',
    		      );

    		      if($userDetails['status'] == 'cre_usr_sts_atv'){
    		        $activateSuspendLabel  = t('LBL572');
    		        $userActionStatus  = 'Suspend';
    		      }
    		      else{
    		        $activateSuspendLabel  = t('LBL573');
    		        $userActionStatus  = 'Activate';
    		      }
    		      $activateSuspendButtonId = 'user-activate-suspend-button';
    		      if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
    		        $activateSuspendAttributes['onclick'] = array();
    		        $activateSuspendAttributes['onclick'][] = 'increasePopWidth();';
    		        $form[$activateSuspendButtonId] = array(
  	    '#type' => 'submit',
  	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    '#suffix' => '</li>',
  	    '#value' => $activateSuspendLabel,
        '#name' => 'usr-activate-suspend',
        '#id' =>$activateSuspendButtonId,
  	    '#attributes' => $activateSuspendAttributes,
        	'#ajax' => array(
    	      'wrapper' => $wrapperId,
    	      'callback' => $ajaxCallbackSave,
    	      'method' => 'replace',
    	      'effect' => 'none', // 'fade',
    	      'event'=>'click',
        	  'customButtonId'=>$esignHiddenButt1,
    	      'keypress' => true,
    	      'progress' => array(
    	        'type' => 'throbber',
    	        'message' => ''
    	        ),
    	        )
    	        );
    		      }else{
    		        $activateSuspendAttributes['onclick'] = array();
    		        $activateSuspendAttributes['onclick'][] = '$("#'.$esignHiddenButt1.'").click(); return false;';
    		        $form[$activateSuspendButtonId] = array(
  	    	'#type' => 'submit',
  	    	'#prefix' => '<li class="save-pub-unpub-sub-menu">',
  	    	'#suffix' => '</li>',
            '#value' => $activateSuspendLabel,
          	'#name' => 'usr-activate-suspend',
    	    '#attributes' => $activateSuspendAttributes,
    		        );
    		      }
    		      $activateSuspendAttributes['onclick'] = array();
    		      $form[$esignHiddenButt1] = array(
    	      '#type' => 'submit',
    	      '#value' => t('hidden-save'),
    	      '#name'	=> 'usr-status-upd-hidden-save',
          	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
    	      '#suffix' => '</span>',
    	      '#id' =>$esignHiddenButt1,
      		  '#attributes' => $activateSuspendAttributes,
    	      '#ajax' => array(
    		      'wrapper' => $wrapperId,
    		      'callback' => $ajaxCallbackSave,
    		      'method' => 'replace',
    		      'effect' => 'none', // 'fade',
    		      'event'=>'click',
                  'userActionStatus'=>$userActionStatus,
    		      'keypress' => true,
    		      'progress' => array(
    		      'type' => 'throbber',
    		      'message' => ''
    		      ),
    		      )
    		      );
    		      $deleteAccess = delete_object_record($userDetails['id'],'User');
    		      if(!$deleteAccess){
    		      	$deleteCheck = adminVisibilityForAddEditDelete($userDetails['id'],'cre_usr','cre_usr','slt_person');
    		      	if($deleteCheck[0]->deletevisible > 0 || !empty($deleteCheck[0]->deletecount) || $GLOBALS["user"]->uid == 1){
    		        //$displayfirstTitle = titleController(' ', $userDetails['first_name'],34);
    		        //$displaylastTitle = titleController(' ', $userDetails['last_name'],34);
    		        $displayTitle = $userDetails['last_name'] . ', ' . $userDetails['first_name'];
    		        $deleteButtonId = 'user-delete-dropdown-button';
    		        $deleteAttributes['onclick'] = array();
    		        $deleteAttributes['onclick'][] = '$("#root-admin").data("narrowsearch").displayDeleteWizard("'.t('MSG357').' '.strtolower(t('User')).' ' .$displayTitle . '","'.$userDetails['id'].'","User"); return false;';
    		        $deleteAttributes['class'][] = 'admin-delete-button-bg';
    		        $form[$deleteButtonId] = array(
    	    '#type' => 'button',
    	    '#prefix' => '<li class="save-pub-unpub-sub-menu">',
    	    '#suffix' => '</li>',
    	    '#value' => t('LBL286'),
            '#name' => 'delete',
    	    '#attributes' => $deleteAttributes,
    		        );
    		      	}
    		      }
    		      $form['addedit-button-group-ul-end'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="clearBoth"></div></div>' .
  	             '<div class="clearBoth"></div></div></ul>',
    		      );
  }
  else { // empty ($userDetails) - new record
    if(module_exists('exp_sp_esignature') && isActiveEsignRegion() && $eSignatureAvailable){
      $saveAttributes['onclick'] = array();
      //$saveAttributes['onclick'][] = 'Drupal.ajax.prototype.commands.addEditHideMessages();';
      $form[$saveButtonId] = array(
      	      '#type' => 'submit',
      	      '#value' => t('LBL141'),
                '#name' => 'save',
      	   	  '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      	       '#suffix' =>
      	               '<div class="admin-save-button-right-bg"></div></div>' .
      	               '<div class="clearBoth"></div></div>' .
      	             '<div class="clearBoth"></div></div>',
        	      '#id' =>$saveButtonId,
        		  '#attributes' => $saveAttributes,
      	      '#ajax' => array(
      		      'wrapper' => $wrapperId,
      		      'callback' => $ajaxCallbackSave,
      		      'method' => 'replace',
      		      'effect' => 'none', // 'fade',
      		      'event'=>'click',
        		  'drupalUserId' => 'drupal_uid',
                    'customButtonId'=>$esignHiddenButt,
      		      'keypress' => true,
      		      'progress' => array(
      		        'type' => 'throbber',
      		        'message' => ''
      		        ),
      		        )
      		        );
    }else{
      $saveAttributes['onclick'] = array();
      $saveAttributes['onclick'][] = '$("#'.$esignHiddenButt.'").click(); return false;';
      $form[$saveButtonId] = array(
      '#type' => 'button',
      '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
      '#suffix' =>
                 '<div class="admin-save-button-right-bg"></div></div>' .
                 '<div class="clearBoth"></div></div>' .
               '<div class="clearBoth"></div></div>',
        '#value' => t('LBL141'),
      	'#name' => 'save',
      '#attributes' => $saveAttributes,
      );
    }
    unset($saveAttributes['tabindex']);
    $saveAttributes['onclick'] = array();
    $form[$esignHiddenButt] = array(
	      '#type' => 'submit',
	      '#value' => t('hidden-save'),
	      '#name'	=> 'hidden-save',
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none">',
	      '#suffix' => '</span>',
	      '#id' =>$esignHiddenButt,
  		  '#attributes' => $saveAttributes,
	      '#ajax' => array(
		      'wrapper' => $wrapperId,
		      'callback' => $ajaxCallbackSave,
		      'method' => 'replace',
		      'effect' => 'none', // 'fade',
		      'event'=>'click',
    //'userActionStatus'=>'',
		      'keypress' => true,
		      'progress' => array(
		      'type' => 'throbber',
		      'message' => ''
		      ),
		      )
		      );

  }

  $entityId          	 = $userDetails['id'];//.',"'.$entityType
  if(!empty($emptyId)){
  	$entityId = 0;
  } else {
  	$emptyId = 0;
  }


  $entityType        	 = 'cre_usr';
  $qtipIdInit        	 = core_encrypt($entityId).'_'.$entityType;
  $qtipOptAccessObj      = "{'entityId':'".core_encrypt($entityId)."','entityType':'$entityType',
														  'url':'administration/catalogaccess/".core_encrypt($entityId)."/".$entityType."/".$emptyId."',
														  'popupDispId':'qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_".$qtipIdInit."',
														  'wid':650,'heg':'270','postype':'topleft','poslwid':'40','qdis':'ctool','linkid':'visible-user-".core_encrypt($entityId)."'}";
  $qtipOptgrpObj      = "{'entityId':'".core_encrypt($entityId)."','entityType':'$entityType','url':'administration/view/groups/".core_encrypt($entityId)."/".$entityType."','popupDispId':'qtip_visible_disp_grp_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_grp_".$qtipIdInit."','wid':300,'heg':'200','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-user-grp".core_encrypt($entityId)."','scrollid':'usr-group-list'}";
  // $qtipOptownerObj      = "{'entityId':'".core_encrypt($entityId)."','entityType':'$entityType','url':'administration/view/owner_groups/".core_encrypt($entityId)."/".$entityType."','popupDispId':'qtip_visible_disp_grp_owner_".$qtipIdInit."','catalogVisibleId':'qtipAccessqtip_visible_disp_grp_".$qtipIdInit."','wid':300,'heg':'200','postype':'topleft','poslwid':'','qdis':'ctool','linkid':'visible-user-owner-grp".core_encrypt($entityId)."','scrollid':'usr-group-list'}";

 /*  $paintMultiAction .='<div id="wrap-perm-user-container">';
  $paintMultiAction .= '<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-user-".$entityId."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
  		'</div>';
  $paintMultiAction .=  '<div class="tab-seperator"></div>';
  $paintMultiAction .= "<div id='qtip_visible_disp_grp_".$qtipIdInit."'><a id='visible-user-grp".$entityId."' onclick = \"callVisibility(".$qtipOptgrpObj.");\" class=\"tab-title\">".t('View Groups')."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD Users
  $paintMultiAction .='</div>'; */

  $form['addedit-left-col-extend-func'] = array(
  		'#type' => 'markup',
  		'#markup' => //$paintMultiAction,
  		'<div class="crs-tab-titles-container"><div class="access-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_".$qtipIdInit."'><a id='visible-user-".core_encrypt($entityId)."' onclick = \"callVisibility(".$qtipOptAccessObj.");\" class=\"tab-title\">".t('LBL642')."</a><span id='visible-popup-".core_encrypt($entityId)."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"
  		//'</div>'
  ,

  );
 $form['addedit-left-col-extend-grp-func'] = array(
  		'#type' => 'markup',
  		'#markup' => '<div class="tab-seperator"></div><div class="crs-tab-titles-container-grp"><div class="grp-tab-icon"></div>'.
  		"<div id='qtip_visible_disp_grp_".$qtipIdInit."'><a id='visible-user-grp".core_encrypt($entityId)."' onclick = \"callVisibility(".$qtipOptgrpObj.");\" class=\"tab-title\">".t('LBL816').' '.t('Groups')."</a><span id='visible-popup-".core_encrypt($entityId)."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
  		'</div>',
  );

 
 /* $form['addedit-left-col-extend-grp-owner-func'] = array(
 		'#type' => 'markup',
 		'#markup' => '<div class="tab-seperator"></div><div class="crs-tab-titles-container-grp"><div class="grp-tab-icon"></div>'.
 		"<div id='qtip_visible_disp_grp_owner_".$qtipIdInit."'><a id='visible-user-owner-grp".core_encrypt($entityId)."' onclick = \"callVisibility(".$qtipOptownerObj.");\" class=\"tab-title\">".t('View Owner').' '.t('Groups')."</a><span id='visible-popup-".core_encrypt($entityId)."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>".
 		'</div>',
 ); */
  $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
  );
  } catch (Exception $ex) {
  	watchdog_exception('userorganizationAddEditFormWithCancelAndSaveClose', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * updateUserActivateFromDelete
 */
function updateUserActivateFromDelete($userId, $drupalId,$emailId,$isNotify = 0,$formvalues,$createdUserId){
	try {
		expDebug::dPrint('results ' . print_r($formvalues, true) , 5);
  $fields['status'] = 'cre_usr_sts_atv';

  $update = db_update('slt_person');
  $update->fields($fields);
  $update->condition('id', $userId);
  $update->execute();

  $fields['status'] = 1;
  $update = db_update('users');
  $update->fields($fields);
  $update->condition('uid', $drupalId);
  $update->execute();
  if($isNotify){
    expDebug::dPrint(' updateUserActivateFromDelete() Notification id '.$emailId , 4);
    sendResetPasswordLink('register_admin_created', $emailId);
  }
  $formvalues['hire_date'] = date_format(date_create($formvalues['hire_date']), 'Y-m-d H:i:s');
  expDebug::dPrint(' form values set ' . print_r($formvalues, true) , 5);
  make_audit_trail_entries($formvalues, $createdUserId , null ,$userId, 'Inserted','cre_sys_obt_usr','exp_sp_administration_user','addNewUser',
  array('first_name', 'last_name', 'user_name','city','zip','email','hire_date'));
  } catch (Exception $ex) {
  	watchdog_exception('updateUserActivateFromDelete', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * updateUserAddressDetails - Saves/updates user's address details
 */
function updateUserAddressDetails($form, $form_state, $updateUserId ,$action) {
  expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
  expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
  expDebug::dPrint(' updateUserAddressDetails $form_state[values] = ' . print_r($form_state['values'], true) , 5);
  expDebug::dPrint(' $updateUserId = ' . print_r($updateUserId, true) , 4);
  $personid = $form_state['values']['id'];
  $callfrom = empty($form_state['values']['callfrom']) ? '' : $form_state['values']['callfrom'];
  $txn = db_transaction();

  try {
    // Select the table to insert the record in
    $updateStmt = db_update('slt_person');
    $default= db_query("SELECT id,addr1,addr2,country,state,city,zip,phone_no,mobile_no,sms_alert,is_verified_add,is_valid_add,invalid_add_reason from slt_person where id=:uid",array(":uid"=>$personid))->fetchAll();
    expDebug::dPrint(' updateUserAddressDetails velu '.print_r($default,true),5);
    $fields = array(
                    'addr1' =>(!empty($form_state['values']['addr1'])) ? trim( $form_state['values']['addr1']) : (isset($_REQUEST['addr1']) ? $_REQUEST['addr1'] : $default[0]->addr1),
                    'addr2' =>(!empty($form_state['values']['addr2'])) ?  trim($form_state['values']['addr2']) : (isset($_REQUEST['addr2']) ? $_REQUEST['addr2'] : $default[0]->addr2), //$form_state['values']['addr2'],
                    'country' =>(!empty($form_state['values']['country'])) ? trim($form_state['values']['country']) : (isset($_REQUEST['country']) ? $_REQUEST['country']:$default[0]->country),//$form_state['values']['country'],
                    'state' =>(!empty($form_state['values']['state'])) ?  trim($form_state['values']['state']) : (isset($_REQUEST['state']) ? $_REQUEST['state'] : $default[0]->state),// $form_state['values']['state'],
                    'city' => (!empty($form_state['values']['city'])) ? trim($form_state['values']['city']) : (isset($_REQUEST['city']) ? $_REQUEST['city'] : $default[0]->city),//trim($form_state['values']['city']),
                    'zip'  =>  (!empty($form_state['values']['zip'])) ? trim($form_state['values']['zip']) : (isset($_REQUEST['zip']) ? $_REQUEST['zip'] : $default[0]->zip),//trim($form_state['values']['zip']),
                    'phone_no' => (!empty($form_state['values']['phone_no'])) ? trim($form_state['values']['phone_no']) : (isset($_REQUEST['phone_no']) ? $_REQUEST['phone_no'] : $default[0]->phone_no),//$form_state['values']['phone_no'],
                    'mobile_no' => (!empty($form_state['values']['mobile_no'])) ? trim($form_state['values']['mobile_no']) : (isset($_REQUEST['mobile_no']) ? $_REQUEST['mobile_no'] : $default[0]->mobile_no),//$form_state['values']['mobile_no'],
                    'sms_alert' => empty($form_state['values']['mobile_no']) ? '' : ($callfrom == 'DL' ? $form_state['values']['sms_alert'] : $default[0]->sms_alert),
                    'is_verified_add' => (!empty($form_state['values']['is_verified_add'])) ? trim($form_state['values']['is_verified_add']) : $default[0]->is_verified_add,//$form_state['values']['is_verified_add'],
                    'is_valid_add' => (!empty($form_state['values']['is_valid_add'])) ? trim($form_state['values']['is_valid_add']) : '',
                    'invalid_add_reason' => empty($form_state['values']['invalid_add_reason']) ? '' : trim($form_state['values']['invalid_add_reason']),
                    'updated_by' => $updateUserId,
                    'updated_on' => now()
    );

    // Select the fields and the values to be inserted
    $updateStmt->fields($fields);
    $updateStmt->condition('id', $form_state['values']['id']);

    expDebug::dPrintDBAPI(' $updateStmt obj = ' , $updateStmt);
    $updateUserId = ($updateUserId == 'Feed') ? 1 : $updateUserId;
    // Execute the update statement.
    $numUpdated = $updateStmt->execute();
    make_audit_trail_entries($form_state['values'], $updateUserId , $default ,$personid, $action,'cre_sys_obt_usr','exp_sp_administration_user','updateUserAddressDetails',
    array('addr1', 'addr2', 'country', 'state', 'city', 'zip' ,'phone_no','mobile_no','sms_alert','is_verified_add','is_valid_add','invalid_add_reason'));


  }
  catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateUserAddressDetails', $ex);
    throw $ex;
  }
}


/*
 * getDefaultUserOrgAddEditFieldValue() - Determines the default value for a user org form field to be shown in the form when it is painted.
 */
function getDefaultUserOrgAddEditFieldValue($field, $form_state, $orgDetails) {
try {
  switch ($field) {
    case 'manager':
      $dbField = 'manager_id';
      expDebug::dPrint(' $orgDetails[manager_id] ' .
      print_r($orgDetails[$dbField], true) , 4);
      expDebug::dPrint(' $form_state[values][manager] ' .
      print_r($form_state['values'][$field], true) , 5);
      $defaultValue = empty($form_state['values'][$field]) ?
      (empty($orgDetails[$dbField]) ? '' : getUserAddEditManagerDisplayName($orgDetails[$dbField]))
      : $form_state['values'][$field];
      break;

    case 'org':
      $dbField = 'org_id';
      expDebug::dPrint(' $orgDetails[org_id] ' .
      print_r($orgDetails[$dbField], true) , 4);
      expDebug::dPrint(' $form_state[values][org] ' .
      print_r($form_state['values'][$field], true) , 5);
      $defaultValue = empty($form_state['values'][$field]) ?
      (empty($orgDetails[$dbField]) ? '' : getOrgName($orgDetails[$dbField]))
      : $form_state['values'][$field];
      break;

    case 'location':
      $dbField = 'preferred_loc_id';
      expDebug::dPrint(' $orgDetails[preferred_loc_id] ' .
      print_r($orgDetails[$dbField], true) , 4);
      expDebug::dPrint(' $form_state[values][location] ' .
      print_r($form_state['values'][$field], true) , 5);
      $defaultValue = empty($form_state['values'][$field]) ?
      (empty($orgDetails[$dbField]) ? '' : getLocationName($orgDetails[$dbField]))
      : $form_state['values'][$field];
      break;

    case 'omanagers':
    case 'oorgs':
     	if ($field == 'omanagers') {
        $dbField = 'dotted_mngr_id';
     	} else {  // ($field == 'oorgs')
     	  $dbField = 'dotted_org_id';
     	}
     	expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
      expDebug::dPrint(' $dbField = ' . print_r($dbField, true) , 4);
      expDebug::dPrint(' $orgDetails[$dbField] = ' .print_r($orgDetails[$dbField], true) , 4);
      expDebug::dPrint(' $form_state[values][$field] = ' .print_r($form_state['values'][$field], true) , 5);
      $defaultValue = empty($form_state['values'][$field]) ? (empty($orgDetails[$dbField]) ? '' : explode(',', $orgDetails[$dbField])): $form_state['values'][$field];
      break;

    case 'empltype' : // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['empltype']) ?
      (empty($orgDetails['employment_type']) ? '' : getProfileListItemName($orgDetails['employment_type']))
      : $form_state['values']['empltype'];
      break;

    case 'deptcode' : // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['deptcode']) ?
      (empty($orgDetails['dept_code']) ? '' : getProfileListItemName($orgDetails['dept_code']))
      : $form_state['values']['deptcode'];
      break;

    case 'jobtitle' : // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['jobtitle']) ?
      (empty($orgDetails['job_title']) ? '' : getProfileListItemName($orgDetails['job_title']))
      : $form_state['values']['jobtitle'];
      break;

    case 'jobrole' : // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['jobrole']) ?
      (empty($orgDetails['job_role']) ? '' : getProfileListItemName($orgDetails['job_role']))
      : $form_state['values']['jobrole'];
      break;

    case 'usertype' : // Cannot have underscore in autocomplete field id
      $defaultValue = empty($form_state['values']['usertype']) ?
      (empty($orgDetails['user_type']) ? '' : getProfileListItemName($orgDetails['user_type']))
      : $form_state['values']['usertype'];
      break;

    default:
      $defaultValue = getDefaultAddEditFormFieldValue($field, $form_state, $orgDetails);
      break;
  } // End switch

  if(($field == 'hire_date' || $field == 'terminated_date') && $defaultValue != ''){
    $defaultValue = date('m-d-Y', strtotime($defaultValue));
  }
  else if ($field == 'is_rehire'){
    $defaultValue = (empty($defaultValue) || $defaultValue == 'N')? 0 : 1;
  }
  else if ($field == 'is_manager'){
    $defaultValue = (empty($defaultValue) || $defaultValue == 'N')? 0 : 1;
  }
  else if ($field == 'is_instructor'){
    $defaultValue = (empty($defaultValue) || $defaultValue == 'N')? 0 : 1;
  }

  expDebug::dPrint(' $defaultValue for field ' .
  $field . ' = ' . print_r($defaultValue, true) , 4);

  return $defaultValue;
  } catch (Exception $ex) {
  	watchdog_exception('getDefaultUserOrgAddEditFieldValue', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserAddEditManagerDisplayName() - Fetch person name apprended with (username) for displaying in manager name auto-complete field
 */
function getUserAddEditManagerDisplayName($userId) {
	try {
  if (empty($userId)) {
    return '';
  }

  // Select slt_organization table
  $select = db_select('slt_person', 'per');

  // Get person full_name and user_name
  $select->addField('per', 'full_name', 'full_name');

  // Set the conditions
  $select->condition('per.id', $userId, '=');

  expDebug::dPrintDBAPI(' $select SQL = ' , $select);

  // Execure the query
  $result = $select->execute()->fetch();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  if (empty($result)) {
    return ''; // Does not exist
  }

  return $result->full_name;
  } catch (Exception $ex) {
  	watchdog_exception('getUserAddEditManagerDisplayName', $ex);
  	expertusErrorThrow($ex);
  }
}
function check_two_array_equal($a, $b) {
	return (is_array($a) && is_array($b) && array_diff($a, $b) === array_diff($b, $a));
}
/*
 * getAllManagersSelectOptionsList() -
 */
function getAllManagersSelectOptionsList($excludedUserId, $codeRequired = 1, $otherManagerList = false, $selectedMngrId,$currentUserId='') {
  // Select the table
  try {
  $select = db_select('slt_person', 'per');

  // Add fields to fetch
  $select->addField('per', 'id', 'id');
  $select->addField('per', 'first_name', 'fname');
  $select->addField('per', 'last_name', 'lname');
  $select->addField('per', 'user_name', 'uname');

  // Add conditions
  // $select->condition('per.is_manager', '1', '=');
  $select->condition('per.status', 'cre_usr_sts_atv');
  $select->condition('per.is_manager', 'Y');
  if (!empty($excludedUserId) && $excludedUserId > 0) {
    $select->condition('per.id', $excludedUserId, '!=');
  }
  if (!empty($currentUserId) && $currentUserId > 0) {
  	$select->condition('per.id', $currentUserId, '!=');
  }
		if (!empty($selectedMngrId) && $selectedMngrId > 0) {
			$select->addExpression('if(find_in_set(per.id, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedMngrId));
			$select->orderBy('sel','desc');
		}
  //fix for ticket 11298 - issue 7
  $select->condition('per.user_name', array('admin','guest'), 'NOT IN');
  filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
  // Order results by name
  $select->orderBy('fname');
  $select->orderBy('lname');

  expDebug::dPrintDBAPI(' $select obj = ' , $select);

		//$managersList = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingPersons = '. print_r($managersList, true) , 3);
		if($otherManagerList == true) {
			$idAli	= $fromAjax ? 'opt' : 'id';
			$nameAli	= $fromAjax ? 'val' : 'name';
			$seleAli	= 'sel';

			$searchText	= $_GET['z'];
			expDebug::dPrint('$output11111111 $searchText'.print_r($searchText, 1),5);
			$page		= $_GET['page'];
			$limit		= $_GET['rows'];
			$start		= $limit * $page - $limit;
			$count = $select->countQuery()->execute()->fetchField();
			if( $count >0 ){
				$total_pages = ceil($count/$limit);
    } else {
				$total_pages = 0;
    }
			if($searchText != '' && $searchText != null) {
				$select->condition('per.full_name', '%' . $searchText . '%', 'LIKE');
  }
			expDebug::dPrintDBAPI("Access getJobRoleLst countQuery ",$select);
			if($page > $total_pages)
				$page = $total_pages;
			if($start < 0) {
				$start = 0;
  }
			$select->range($start,$limit);
  $managersList = $select->execute()->fetchAll();
            $managerOptionsJs =array();
			foreach ($managersList as $key => $manager) {
				expDebug::dPrint('$output11111111'.print_r($manager, 1),5);
				$managerOptionsJs[$key] = array('opt' => $manager->id, 'val' => $manager->fname . ' ' . $manager->lname,  'sel' => $manager->sel);
			}

			$output = array();
			$output['records'] = $count;
			$output['page'] = $page ;
			$output['data'] = $managerOptionsJs;
			return $output;

		}else {
  $managerOptions = array();
  foreach ($managersList as $manager) {
    if($codeRequired){
      $managerOptions[core_encrypt($manager->id)] = $manager->fname . ' ' . $manager->lname . ' (' . $manager->uname . ')';
    } else {
      $managerOptions[core_encrypt($manager->id)] = $manager->fname . ' ' . $manager->lname;
    }
  }
			$managersList = $select->execute()->fetchAll();
  return $managerOptions;
		}
  } catch (Exception $ex) {
  	watchdog_exception('getAllManagersSelectOptionsList', $ex);
  	expertusErrorThrow($ex);
  }
}

function getOtherManagerAutoComplete($excludedMngrId, $codeRequired = 1, $fromAjax = false, $selectedMngrId) {
	try {
		expDebug::dPrint(' $excludedRoleId = '. print_r($excludedJobroleId, true) , 4) ;
		$idAli	= $fromAjax ? 'opt' : 'id';
		$nameAli	= $fromAjax ? 'val' : 'name';
		$seleAli	= 'sel';

		// Select the table
		$select = db_select('slt_person', 'per');

		// Add fields to fetch
		$select->addField('per', 'id', 'id');
		$select->addField('per', 'first_name', 'fname');
		$select->addField('per', 'last_name', 'lname');
		$select->addField('per', 'user_name', 'uname');
		$select->addField('per', 'full_name', 'fullname');
		$select->condition('per.status', 'cre_usr_sts_atv');
		if ((!empty($excludedMngrId)) && $excludedMngrId > 0) {
			$select->condition('per.id', $excludedMngrId, '<>');
		}
		$select->addExpression('if(find_in_set(per.id, :select_id) > 0 ,1 ,0)', 'sel', array('select_id'=>$selectedMngrId));
		$select->condition('per.is_manager', 'Y');
		//fix for ticket 11298 - issue 7
		$select->condition('per.user_name', array('admin','guest'), 'NOT IN');
		// Add fields to fetch

		expDebug::dPrint(' $fromAjax = '. print_r($fromAjax, true) , 4) ;
		$searchText	= $_GET['z'];
		$limit		= $_GET['limit'];

		expDebug::dPrint(' $fromAjax z = '. print_r($searchText, true) , 4) ;
		if($searchText != '' && $searchText != null) {
			$select->condition('per.full_name', '%' . db_like($searchText) . '%', 'LIKE');
		}
		filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
		// Order results by name
		$select->orderBy('fname');
		$select->orderBy('lname');


		//$count = $select->countQuery()->execute()->fetchField();
		if ($limit > 0) {
			$select->range(0, $limit);
		}
		expDebug::dPrintDBAPI("Access getJobRoleLst countQuery ",$select);
		$managersList = $select->execute()->fetchAll();

		foreach ($managersList as $key => $manager) {
			expDebug::dPrint(' $role = ' . print_r($name , true),3);
			$choiceStr = '';
			$choiceStr .=$manager->fullname;
			expDebug::dPrint(' $choiceStr = ' . print_r($choiceStr, true),4);
			print $choiceStr . "\n";
		}
		expDebug::dPrint(' $$jobrole = '. print_r($$jobrole, true) , 3);
	} catch (Exception $ex) {
		watchdog_exception('getJobRoleAutoComplete', $ex);
		expertusErrorThrow($ex);
	}
}

/*
 * getUserAdminMatchingLocationsList() - Returns matching location names with ids.
 */
function getUserAdminMatchingLocationsList($substring) {
  // Select the table
  try {
  $select = db_select('slt_location', 'loc');

  // Select the field
  $select->addField('loc', 'id');
  $select->addField('loc', 'name');

  // Add condition
  $select->condition('loc.name', '%' . db_like($substring) . '%', 'LIKE');
  expDebug::dPrintDBAPI(' $select obj = ' , $select);

  // Fetch results
  $result = $select->execute()->fetchAllKeyed();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getUserAdminMatchingLocationsList', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * getUserAdminMatchingManagersList() - Returns matching manager names with ids. Considers hidden manager_id set by autocomplete plugin
 */
function getUserAdminMatchingManagersList($pattern, $hiddenManagerId, $excludedUserId) {
 try {
  // Get list of user ids which are excluded from the search
  $excludedUserIdList = array();
  if (!empty($excludedUserId) && $excludedUserId > 0) {
    $excludedUserIdList[] = $excludedUserId;
    $excludedUserIdList = array_merge($excludedUserIdList, getAllDirectIndirectReporteesIds($excludedUserId));
  }

  // If $hiddenManagerId is present (and it is not in $excludedUserId or is inactive), get its corresponding full_name.
  // If $pattern matches its full_name, return this record
  if (!empty($hiddenManagerId) && !in_array($hiddenManagerId, $excludedUserIdList)) {
    // Select the table
    $select = db_select('slt_person', 'per');

    // Select the field
    $select->addField('per', 'id');
    $select->addField('per', 'full_name');

    // Add conditions
    $select->condition('per.id', $hiddenManagerId, '=');
    $select->condition('per.status', 'cre_usr_sts_atv','=');
    $select->condition('per.user_name', array('admin', 'guest'), 'NOT IN');
    filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
    expDebug::dPrintDBAPI(' hidden manager id $select SQL = ' , $select);
    
    // Fetch results
    $result = $select->execute()->fetchAll();
    expDebug::dPrint(' hidden manager id $result = ' . print_r($result, true) , 3);
  }

  // If a record is fetched, and its full_name matches the $pattern return the record
  if (!empty($result) && $result[0]->full_name == $pattern) {
    expDebug::dPrint(' pattern matches hidden manager_id\'s full name');
    return $result;
  }

  // $hiddenManagerId is invalid, we need to search the pattern in full_name and username fields
  // Select the table
  $select = db_select('slt_person', 'per');

  // Select the field
  $select->addField('per', 'id');
  $select->addField('per', 'full_name');

  // Add conditions
  $select->condition('per.status', 'cre_usr_sts_atv','=');
  $select->condition('per.user_name',array('admin','guest'),'NOT IN');
  if (!empty($excludedUserId) && $excludedUserId > 0) {
    $select->condition('per.id', $excludedUserIdList, 'NOT IN');
  }
  $select->condition(db_or()
  ->condition('per.full_name', '%' . db_like($pattern) . '%', 'LIKE')
  ->condition('per.user_name', '%' . db_like($pattern) . '%', 'LIKE'));
  filterByUserAccessPrivileges($select,'per','',1,'cre_usr','attach','per');
  expDebug::dPrintDBAPI(' $select SQL = ' , $select);

  // Fetch results
  $result = $select->execute()->fetchAll();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);

  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getUserAdminMatchingManagersList', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * updateUserOrgDetails - Saves/updates user's address details
 */
function updateUserOrgDetails($form, &$form_state, $updateUserId, $personid = 0 ,$action, $callFromAPI = 0) {
	try {
		global $userSSOAttributes;
		expDebug::dPrint(' $form = ' . print_r($form, true) , 5);
		expDebug::dPrint(' $form_state = ' . print_r($form_state, true) , 5);
		expDebug::dPrint(' $form_state[input] = ' . print_r($form_state['input'], true) , 5);
		expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
		expDebug::dPrint(' $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
		expDebug::dPrint(' $updateUserId = ' . print_r($updateUserId, true) , 4);
		expDebug::dPrint(' $_REQUEST updateUserOrgDetails= ' . print_r($_REQUEST, true) , 4);
		expDebug::dPrint(' $userSSOAttributes updateUserOrgDetails= ' . print_r($userSSOAttributes, true) , 4);
		//$personid= $form_state['values']['id'];
		//7306: Support of users to have more than 1 job role association
		$default= db_query("SELECT id,user_type,dept_code,job_title,employee_no,employment_type,hire_date,terminated_date,is_rehire,is_manager,is_instructor,manager_id,dotted_mngr_id ,org_id,dotted_org_id,dept_code,preferred_loc_id,preferred_language,country,state,preferred_currency,sms_alert from slt_person where id=:uid",array(":uid"=>$personid))->fetchAll();
		$txn = db_transaction();

		try {
			// Select the table to insert the record in
			$updateStmt = db_update('slt_person');

			/*if($default[0]->is_instructor == NULL)
			{
				$default[0]->is_instructor = 'N';
			}*/
			expDebug::dPrint(' $defaultdbquery = ' .print_r($default,true),4);
			// Prepare the fields list to update
			$fieldsList = array();
			$fieldsList['sms_alert'] = $form_state['values']['register_sms'];
			//$fieldsList['employment_type'] = empty($form_state['values']['empltype'])? NULL : $form_state['values']['empltype'];
			$emplType = trim($form_state['values']['empltype']);
			expDebug::dPrint(' $$emplType = ' .print_r($emplType,true),4);
			if($callFromAPI){
				if(isset($_REQUEST['empltype']) || isset($userSSOAttributes['EmplType'])) {
					$fieldsList['employment_type'] = trim($form_state['values']['empltype']);
				} else {
					unset($fieldsList['employment_type']);
				}
			}else {
				$defaultEmpTyp = getProfileListItemByCode($default[0]->employment_type);
				if(!empty($emplType)) {
				$emplTypeItem = getMatchingProfileItemNamesKeyed($emplType, 'cre_usr_etp_');
				if(($defaultEmpTyp[0]->name != $emplType)){
					if (count($emplTypeItem) == 1) {
						reset($emplTypeItem); //Safety - sets pointer to top of array
						$fieldsList['employment_type'] = key($emplTypeItem); // Returns the first key of the array (code)
					}
				}else{
					$fieldsList['employment_type'] = $default[0]->employment_type;
				}
				} else {
					$fieldsList['employment_type'] = NULL;
			}
			}



			//$fieldsList['employment_type'] = empty($form_state['values']['empltype'])? NULL : $form_state['values']['empltype'];
			// commented for 66144
			/* $fieldsList['preferred_language'] = NULL;
			$userlanguage = trim($form_state['values']['preferred_language']);
			if($callFromAPI) {
				if(isset($_REQUEST['preferred_language']) || !empty($userlanguage)) {
					$fieldsList['preferred_language'] = trim($form_state['values']['preferred_language']);
				} else {
					unset($fieldsList['preferred_language']);
				}
			}elseif (!empty($userlanguage)) {
				$select = db_select('slt_profile_list_items', 'lan');
				$select->addField('lan','name', 'name');
				$select->condition('lan.code',$userlanguage,'=');
				$language = $select->execute()->fetchField();
				$languageItem = getMatchingProfileItemNamesKeyed($language, 'cre_sys_lng_');
				if (count($languageItem) == 1) {
					reset($languageItem); //Safety - sets pointer to top of array
					$fieldsList['preferred_language'] = key($languageItem); // Returns the first key of the array (code)
				}
			} */
			expDebug::dPrint("sad dsad sadsad asdsa userlanguage".print_r($fieldsList['preferred_language'],true),5);

			//$fieldsList['dept_code'] = empty($form_state['values']['deptcode'])? NULL : $form_state['values']['deptcode'];
			$deptCode = trim($form_state['values']['deptcode']);
			expDebug::dPrint(' $$deptCode = ' .print_r($deptCode,true),4);
			if($callFromAPI) {
					if(isset($_REQUEST['deptcode']) || isset($userSSOAttributes['Department'])){
					$fieldsList['dept_code'] = $form_state['values']['deptcode'];
				} else {
						unset($fieldsList['dept_code']);
					}
					expDebug::dPrint('  $$deptCode IF = ' .print_r($deptCode,true),4);
			}else {
				$defaultDepartment = getProfileListItemByCode($default[0]->dept_code);
				expDebug::dPrint('  $$deptCode else = ' .print_r($defaultDepartment,true),4);
				if(!empty($deptCode)) {
					if(($defaultDepartment[0]->name != $deptCode)){
						$deptCodeItem = getMatchingProfileItemNamesKeyed($deptCode, 'cre_usr_dpt_');
						if (count($deptCodeItem) == 1) {
							reset($deptCodeItem); //Safety - sets pointer to top of array
							$fieldsList['dept_code'] = key($deptCodeItem); // Returns the first key of the array (code)
						}
					}else{
						$fieldsList['dept_code'] = $default[0]->dept_code;
					}
				}
				else {
					$fieldsList['dept_code'] = NULL;
				}
			}

			//$fieldsList['job_title'] = empty($form_state['values']['jobtitle'])? NULL : $form_state['values']['jobtitle'];
			$jobTitle = trim($form_state['values']['jobtitle']);
			if($callFromAPI) {
				if(isset($_REQUEST['jobtitle']) || isset($userSSOAttributes['JobTitle'])){
					$fieldsList['job_title'] = trim($form_state['values']['jobtitle']);
				}else{
					unset($fieldsList['job_title']);
				}
					
			}else{
				$defaultJbTtl = getProfileListItemByCode($default[0]->job_title);
				if(!empty($jobTitle)) {
				if(($defaultJbTtl[0]->name != $jobTitle)){
					$jobTitleItem = getMatchingProfileItemNamesKeyed($jobTitle, 'cre_usr_jtl_');
					if (count($jobTitleItem) == 1) {
						reset($jobTitleItem); //Safety - sets pointer to top of array
						$fieldsList['job_title'] = key($jobTitleItem); // Returns the first key of the array (code)
					}
				}else{
						$fieldsList['job_title'] = $default[0]->job_title;
					}
					
				}else{
						$fieldsList['job_title'] = NULL;
			}
			}

			//$fieldsList['job_role'] = empty($form_state['values']['jobrole'])? NULL : $form_state['values']['jobrole'];
			expDebug::dPrint("user language ".print_r($form_state['values'],true),5);
			//7306: Support of users to have more than 1 job role association
			if($callFromAPI  && (isset($_REQUEST['jobrole']) || isset($userSSOAttributes['JobRole']))){
				$form_state['values']['load_multiselect_jobrole'] = explode(',',$form_state['values']['jobrole']);
				$form_state['values']['load_multiselect_jobrole'] = array_unique($form_state['values']['load_multiselect_jobrole']);
			}elseif(!empty($form_state['values']['load_multiselect_jobrole'])){
				$form_state['values']['load_multiselect_jobrole'] = explode(',',$form_state['values']['load_multiselect_jobrole']);
			}elseif(!empty($form_state['values']['jobrole'])){
				$form_state['values']['load_multiselect_jobrole'] = explode(',',$form_state['values']['jobrole']);
			}

			expDebug::dPrint("load_multiselect_jobrole".print_r($form_state['values']['load_multiselect_jobrole'],true),5);
			//     if($callFromAPI && !empty($jobRole_arr)){
			//       $jobRole_arr = $jobRole;
			//     }
			//     elseif (!empty($jobRole)) {
			//     	$fieldsList['job_role'] =
			//       $jobRoleItem = getMatchingProfileItemNamesKeyed($jobRole, 'cre_usr_jrl_');
			//       if (count($jobRoleItem) == 1) {
			//         reset($jobRoleItem); //Safety - sets pointer to top of array
			//         $fieldsList['job_role'] = key($jobRoleItem); // Returns the first key of the array (code)
			//       }
			//     }

			//$fieldsList['user_type'] = empty($form_state['values']['usertype'])? NULL : $form_state['values']['usertype'];
		    $userType = trim($form_state['values']['usertype']);
		    if($callFromAPI) {
					if(isset($_REQUEST['usertype']) || isset($userSSOAttributes['UserType'])){
					$fieldsList['user_type'] = $form_state['values']['usertype'];
				}else {
						unset($fieldsList['user_type']);
					}
			}else {
				if(!empty($userType)) {
					$defaultUsrTyp = getProfileListItemByCode($default[0]->user_type);
					$userTypeItem = getMatchingProfileItemNamesKeyed($userType, 'cre_usr_ptp_');
					if(($defaultUsrTyp[0]->name != $userType)){
						if (count($userTypeItem) == 1) {
							reset($userTypeItem); //Safety - sets pointer to top of array
							$fieldsList['user_type'] = key($userTypeItem); // Returns the first key of the array (code)
						}
					}else{
						$fieldsList['user_type'] = $default[0]->user_type;
					}
				}
				else {
					$fieldsList['user_type'] = NULL;
				}
			}
			/*
			 $fieldsList['user_type'] = NULL;
			$userType = trim($form_state['values']['usertype']);
			if($callFromAPI && !empty($userType)){
			$fieldsList['user_type'] = $userType;
			}
			elseif (!empty($userType)) {
			$userTypeItem = getMatchingProfileItemNamesKeyed($userType, 'cre_usr_ptp_');
			if (count($userTypeItem) == 1) {
			reset($userTypeItem); //Safety - sets pointer to top of array
			$fieldsList['user_type'] = key($userTypeItem); // Returns the first key of the array (code)
			}
			}
			*/
// 			if($callFromAPI) {
// 				if(isset($_REQUEST['usertype'])) {
// 					$fieldsList['user_type'] = $form_state['values']['usertype'];
// 				}else {
// 					unset($fieldsList['user_type']);
// 				}
// 			}

			$fieldsList['employee_no'] = empty($form_state['values']['employee_no'])? NULL : $form_state['values']['employee_no'];
			if($callFromAPI) {
				if(isset($_REQUEST['employee_no']) || isset($userSSOAttributes['EmployeeNo'])){
					$fieldsList['employee_no'] = $form_state['values']['employee_no'];
				}else{
					unset($fieldsList['employee_no']);
				}
			}

			/*
			 * Commented Reason -> 0020185: Historical data is getting Null if we update any User profile in People Admin
			* // Hire date
			if (!empty($form_state['storage']['dates']['hire_date'])) {
			$fieldsList['hire_date'] = $form_state['storage']['dates']['hire_date'];
			}
			else {
			$fieldsList['hire_date'] = NULL;
			$form_state['values']['hire_date'] = NULL;
			}

			// Terminated date
			if (!empty($form_state['storage']['dates']['terminated_date'])) {
			$fieldsList['terminated_date'] = $form_state['storage']['dates']['terminated_date'];
			}
			else {
			$fieldsList['terminated_date'] = NULL;
			$form_state['values']['terminated_date'] = NULL;
			}*/

			// Is Rehire
			//$fieldsList['is_rehire'] = ($form_state['values']['is_rehire'] == 0)? 'N' : 'Y';
			$fieldsList['is_rehire'] = 'N';

			// Is Manager or Instructor
			/*if($callFromAPI){

			$secRole = explode(',', $form_state['values']['securityrole']);
			$fieldsList['is_manager']    = in_array(2,$secRole) ? '1' : '0';
			$fieldsList['is_instructor'] = in_array(3,$secRole) ? 'Y' : 'N';
			}
			else{

			$fieldsList['is_manager'] = isset($form_state['values']['securityrole']['2']) ? '1' : '0';
			$fieldsList['is_instructor'] = isset($form_state['values']['securityrole']['3']) ? 'Y' : 'N';
			}  */
			expDebug::dPrint(' $updateUserId' . print_r($form_state['values']['roles'], true) , 4);
			//       $fieldsList['is_manager'] = 'N'; // Commented for #0050049 by Shobana N.
			//     $fieldsList['is_instructor'] = 'N';
			if($callFromAPI){
				if(!empty($form_state['values']['roles'])){
				$roles = explode(",",$form_state['values']['roles']);
				if(count($roles) >1 ){
					$fieldsList['is_instructor'] = in_array("instructor", $roles) ? 'Y' : 'N';
					$fieldsList['is_manager'] = in_array("manager", $roles) ? 'Y' : 'N';
				}else{
					$fieldsList['is_instructor'] = ($roles[0] == "instructor") ? 'Y' : 'N';
					$fieldsList['is_manager'] = ($roles[0] == "manager") ? 'Y' : 'N';
				}
			}
				
			}
			if(!$callFromAPI){
				$fieldsList['is_manager'] = isset($form_state['values']['roles']['is_manager']) ? 'Y' : 'N';
				$fieldsList['is_instructor'] = isset($form_state['values']['roles']['is_instructor']) ? 'Y' : 'N';
			}
			$hireDate = trim($form_state['values']['hire_date']);
			if($callFromAPI) {
				if(isset($_REQUEST['hire_date'])){
				if(!empty($form_state['values']['hire_date'])) {
					$date = new DateTime($form_state['values']['hire_date']);
					$hireDate =  $date->format('Y-m-d');
					$fieldsList['hire_date'] = $hireDate;
				}
				else {
					$fieldsList['hire_date'] = NULL;
					$form_state['values']['hire_date'] = NULL;
					//unset($fieldsList['hire_date']);	
				}
			} else{
				expDebug::dPrint('Entered5 '.print_r($form_state['values']['hire_date'],1),4);
				unset($fieldsList['hire_date']);
			} 
			}elseif (!empty($hireDate) && $hireDate != t('LBL112')) {
				$hireDate = str_replace("-","/", $hireDate);
				$result= date_format(date_create($hireDate),'Y-m-d');
				$fieldsList['hire_date'] = $result;
			} else {
				$fieldsList['hire_date'] = NULL;
			}
			
			$register_sms = '';
			$fieldsList['is_mobileadmin'] = ($form_state['values']['register_sms'] == '1') ? '1' : '0';
    if($callFromAPI) {
    	if(isset($_REQUEST['register_sms'])) {
    		$fieldsList['is_mobileadmin'] = $form_state['values']['register_sms'];
    	}else {
    		unset($fieldsList['is_mobileadmin']);
    	}
    }
			expDebug::dPrint(' $fieldsList = ' . print_r($fieldsList, true) , 4);
			$managerId = '';
			// Manager
			if($callFromAPI == 0 || ($callFromAPI == 1  && (isset($_REQUEST['manager_id']) || isset($userSSOAttributes['ManagerUserName'])))) {
			if (!empty($form_state['storage']['acvalues']['manager']['id'])) {

				$fieldsList['manager_id'] = $form_state['storage']['acvalues']['manager']['id'];
				$managerId = $form_state['storage']['acvalues']['manager']['id'];
				$select = db_select('slt_groups','grp');
				$select->addField('grp','id','grpId');
				$select->condition('grp.code','grp_mgr','=');
				$grpId=$select->execute()->fetchField();
				expDebug::dPrint("group id--->".$grpId);
			//	updatesGrpList($personid,$grpId,$managerId,$updateUserId);
				$drupalUserId = getDrupalIdOfUser($managerId);
				updatesGrpList($drupalUserId,$grpId,$managerId,$updateUserId); 
				// We may have to clean up $form_state input and values for manager_id and manager fields,
				// so that they are refreshed with the latest db values
					if(empty($form_state['values']['manager_id']) 
							|| $form_state['values']['manager_id'] != $form_state['storage']['acvalues']['manager']['id'] 
							|| empty($form_state['values']['manager']) 
							|| $form_state['values']['manager'] != $form_state['storage']['acvalues']['manager']['name']) {
					unset($form_state['input']['manager']);
					unset($form_state['values']['manager']);
					unset($form_state['input']['manager_id']);
					unset($form_state['values']['manager_id']);
				}
					//Add the manager id to slt_person_other_manager table Start.
						$select = db_select('slt_person_other_manager','spom');
						$select->addField('spom', 'id','id');
						$select->condition('spom.is_direct','Y' ,'=');
						$select->condition('spom.user_id',$form_state['values']['id'] ,'=');
						$manager_Arr =$select->execute()->fetchAll();
						if(count($manager_Arr)>0){
							$upduser  = db_update('slt_person_other_manager');
							$upduser->fields(array(
									'manager_id' =>$fieldsList['manager_id'],
									'updated_by' => $updateUserId,
									'updated_on' => now(),
										
							));
							$upduser->condition('user_id', $form_state['values']['id'],'=');
							$upduser->condition('is_direct', 'Y','=');
							$upduser->execute();
						}else{
							$usrOthMan  = db_insert('slt_person_other_manager');
							$usrOthMan->fields(array(
									'user_id' => $form_state['values']['id'],
									'manager_id' => $fieldsList['manager_id'],
									'is_direct' =>'Y',
									'created_by' => $updateUserId,
									'created_on' => now(),
							));
							expDebug::dPrintDBAPI('$usrOthMan---->' , $usrOthMan);
							$usrOthMan->execute();
			}
					//Add the manager id to slt_person_other_manager table END.
					
				}else {
					$fieldsList['manager_id'] = NULL;
					
					// Get the fields before deleting
					$loggeduserId = getSltpersonUserId();
					
					$select = db_select('slt_person_other_manager','usrothman');
					$select->addField('usrothman','id','id');
					$select->addField('usrothman','user_id','user_id');
					$select->addField('usrothman','manager_id','manager_id');
					$select->condition('user_id',$form_state['values']['id']);
					$select->condition('is_direct','Y','=');
					$usrManInfo=$select->execute()->fetchAll();
					
					 $selectdelete = db_delete('slt_person_other_manager');
					 $selectdelete->condition('user_id',$form_state['values']['id']);
					 $selectdelete->condition('is_direct','Y','=');
					 //expDebug::dPrintDBAPI('delete userlistlist ' , $selectdelete);
					 $selectdelete->execute();

					 foreach($usrManInfo as $val){
					     // Make deleted log entry
					     $fields = array(
					             'table_name' => 'slt_person_other_manager',
					             'entity_id' => $val->id,
					             'entity_type' => 'user_manager',
					             'parent1_entity_id' => $val->user_id,
					             'parent1_entity_type' => 'user_id',
					             'parent2_entity_id' => $val->manager_id,
					             'parent2_entity_type' => 'per_mgr',
					             'module_name' => 'exp_sp_administration_user',
					             'functionality_name' => 'updateUserOrgDetails',
					             'api_name' => $_REQUEST['apiname'],
					             'deleted_on' => now(),
					             'deleted_by' => $loggeduserId
					     );
					     deleted_log_entry($fields);
					 }

				unset($form_state['input']['manager']);
				unset($form_state['values']['manager']);
				unset($form_state['input']['manager_id']);
				unset($form_state['values']['manager_id']);
			}
			}

			// Dotted managers
			// TODO: Sort the dotted managers by name and then save.

			if($callFromAPI ==0){
				$selectedOtherManagers =explode(',', $form_state['values']['load_multiselect_omanagers']);
			}else if($callFromAPI == 1 && isset($_REQUEST['other_managers'])){
				$selectedOtherManagers = explode(',',$form_state['values']['omanagers']);
			}
			expDebug::dPrint('selected user managers = ' . print_r($selectedOtherManagers, true) , 4);
			//66173: Need to provide other managers information of a user to API response Start
			$default_oth_manager = getUserOthManagerIds($form_state['values']['id']);
			$oth_mng_str = implode(',', $default_oth_manager);
			$default[0]->dotted_mngr_id = $oth_mng_str;
			expDebug::dPrint('Default user managers = ' . print_r($default_oth_manager, true) , 4);
			$manager_arr_diff = array_diff_assoc($selectedOtherManagers, $default_oth_manager);
			$manager_arr_diff1 = array_diff_assoc($default_oth_manager,$selectedOtherManagers);
			expDebug::dPrint('Diff user managers = ' . print_r($manager_arr_diff, true) , 4);
			expDebug::dPrint('Diff user managers1 = ' . print_r($manager_arr_diff1, true) , 4);
			if(count($default_oth_manager)<=0){
				if(count($selectedOtherManagers)>0){
					foreach ($selectedOtherManagers as $manager_id){
						if(!empty($manager_id)){
							$usrOthMan  = db_insert('slt_person_other_manager');
							$usrOthMan->fields(array(
									'user_id' => $form_state['values']['id'],
									'manager_id' =>$manager_id,
									'created_by' => $updateUserId,
									'created_on' => now(),
							));
							expDebug::dPrintDBAPI('$usrOthMan---->' , $usrOthMan);
							$usrOthMan->execute();
						}
					}
				}
				$oth_man_str_ad_tr = implode(',',$selectedOtherManagers);
				$formFieldsListOM = array('dotted_mngr_id');
				$formFieldsListOTHMAN = array('dotted_mngr_id'=>$oth_man_str_ad_tr);
				make_audit_trail_entries($formFieldsListOTHMAN, $updateUserId, $default, $form_state['values']['id'], $action, 'cre_sys_obt_usr', 'exp_sp_administration_user','updateOtherManagerDetails',$formFieldsListOM);
			}
			elseif (!empty($selectedOtherManagers) && (count($manager_arr_diff)>0 || count($manager_arr_diff1)>0)) {
				if(count($selectedOtherManagers) > 0){
				    
				    // Get the fields before deleting
				    $loggeduserId = getSltpersonUserId();
				    	
				    $select = db_select('slt_person_other_manager','usrothman');
				    $select->addField('usrothman','id','id');
				    $select->addField('usrothman','user_id','user_id');
				    $select->addField('usrothman','manager_id','manager_id');
				    $select->condition('user_id',$form_state['values']['id']);
				    $select->condition('is_direct','N','=');
				    $usrManInfo=$select->execute()->fetchAll();
				    
					$selectdelete = db_delete('slt_person_other_manager');
					$selectdelete->condition('user_id',$form_state['values']['id']);
					$selectdelete->condition('is_direct','N','=');
					expDebug::dPrintDBAPI('delete userlist1 ' , $selectdelete);
					$selectdelete->execute();
					
					foreach($usrManInfo as $val){
					    // Make deleted log entry
					    $fields = array(
					            'table_name' => 'slt_person_other_manager',
					            'entity_id' => $val->id,
					            'entity_type' => 'user_manager',
					            'parent1_entity_id' => $val->user_id,
					            'parent1_entity_type' => 'user_id',
					            'parent2_entity_id' => $val->manager_id,
					            'parent2_entity_type' => 'per_oth_mgr',
					            'module_name' => 'exp_sp_administration_user',
					            'functionality_name' => 'updateUserOrgDetails',
					            'api_name' => $_REQUEST['apiname'],
					            'deleted_on' => now(),
					            'deleted_by' => $loggeduserId
					    );
					    deleted_log_entry($fields);
					}
					
					foreach ($selectedOtherManagers as $manager_id){
						if(!empty($manager_id)){
							$usrOthMan  = db_insert('slt_person_other_manager');
							$usrOthMan->fields(array(
									'user_id' => $form_state['values']['id'],
									'manager_id' =>$manager_id,
									'created_by' => $updateUserId,
									'created_on' => now(),
							));
							expDebug::dPrintDBAPI('$usrOthMan---->' , $usrOthMan);
							$usrOthMan->execute();
						}
					}
				}
				$oth_man_str_ad_tr = implode(',',$selectedOtherManagers);
				$formFieldsListOM = array('dotted_mngr_id');
				$formFieldsListOTHMAN = array('dotted_mngr_id'=>$oth_man_str_ad_tr);
				make_audit_trail_entries($formFieldsListOTHMAN, $updateUserId, $default, $form_state['values']['id'], $action, 'cre_sys_obt_usr', 'exp_sp_administration_user','updateOtherManagerDetails',$formFieldsListOM);
			}
			//66173: Need to provide other managers information of a user to API response End
			if($callFromAPI ==0){ // We cannot modify the original list as the same is required for form rebuild
				expDebug::dPrint('selected user managers = ' . print_r($selectedOtherManagers, true) , 4);
				$noneSelectedIdx = array_search('none_selected', $selectedOtherManagers);
				if ($noneSelectedIdx !== FALSE) {
					unset($selectedOtherManagers[$noneSelectedIdx]);
				}
			}
// 			if (!empty($selectedOtherManagers)) {
// 				expDebug::dPrint('selected user managers = ' . print_r($selectedOtherManagers, true) , 4);
// 				if($callFromAPI) {
// 					expDebug::dPrint('selected user managers = ' . $callFromAPI , 4);
// 					$fieldsList['dotted_mngr_id'] =  $selectedOtherManagers;
// 				}
// 				else {
// 					$fieldsList['dotted_mngr_id'] = implode(',', $selectedOtherManagers);
// 				}
// 			}
// 			else {
// 				if($callFromAPI) {
// 					unset($fieldsList['dotted_mngr_id']);
// 				}else {
// 					$fieldsList['dotted_mngr_id'] = NULL;

// 				}
// 			}
//			expDebug::dPrint(' $fieldsList[dotted_mngr_id] = ' . print_r($fieldsList['dotted_mngr_id'], true) , 4);

			// Organization
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['org_id']) || isset($userSSOAttributes['Organization'])))) {
			if (!empty($form_state['storage']['acvalues']['org']['id'])) {

				$fieldsList['org_id'] = $form_state['storage']['acvalues']['org']['id'];

				// We may have to clean up $form_state input and values for org_id and org fields,
				// so that they are refreshed with the latest db values
					if(empty($form_state['values']['org_id']) 
							|| $form_state['values']['org_id'] != $form_state['storage']['acvalues']['org']['id'] 
							|| empty($form_state['values']['org']) 
							|| $form_state['values']['org'] != $form_state['storage']['acvalues']['org']['name']) {
					unset($form_state['input']['org']);
					unset($form_state['values']['org']);
					unset($form_state['input']['org_id']);
					unset($form_state['values']['org_id']);
				}
					// Add the organization id to slt_person_other_organization table Start.
					if(! empty($fieldsList['org_id'])) {
						$select = db_select('slt_person_other_organization', 'spoo');
						$select->addField('spoo', 'id', 'id');
						$select->condition('spoo.is_direct', 'Y', '=');
						$select->condition('spoo.user_id', $form_state['values']['id'], '=');
						$org_Arr = $select->execute()->fetchAll();
						if(count($org_Arr) > 0) {
							$upduser = db_update('slt_person_other_organization');
							$upduser->fields(array(
									'organization_id' => $fieldsList['org_id'],
									'updated_by' => $updateUserId, 
									'updated_on' => now())

							);
							$upduser->condition('user_id', $form_state['values']['id'], '=');
							$upduser->condition('is_direct', 'Y', '=');
							$upduser->execute();
						} else {
							$usrOthOrg = db_insert('slt_person_other_organization');
							$usrOthOrg->fields(array(
									'user_id' => $form_state['values']['id'], 
									'organization_id' => $fieldsList['org_id'], 
									'is_direct' => 'Y', 
									'created_by' => $updateUserId, 
									'created_on' => now()));
							expDebug::dPrintDBAPI('$usrOthMan---->', $usrOthOrg);
							$usrOthOrg->execute();
			}
					}
					// Add the organization id to slt_person_other_organization table END.
				}else {
					$fieldsList['org_id'] = NULL;
					
					// Get the fields before deleting
					$loggeduserId = getSltpersonUserId();
					$select = db_select('slt_person_other_organization','usrothorg');
					$select->addField('usrothorg','id','id');
					$select->addField('usrothorg','user_id','user_id');
					$select->addField('usrothorg','organization_id','organization_id');
					$select->condition('user_id', $form_state['values']['id']);
					$select->condition('is_direct', 'Y', '=');
					$usrOrgInfo=$select->execute()->fetchAll();
					
					$selectdelete = db_delete('slt_person_other_organization');
					$selectdelete->condition('user_id', $form_state['values']['id']);
					$selectdelete->condition('is_direct', 'Y', '=');
					// expDebug::dPrintDBAPI('delete userlistlist ' , $selectdelete);
					$selectdelete->execute();
					
					foreach ($usrOrgInfo as $val){
					    // Make deleted log entry
					    $fields = array(
					            'table_name' => 'slt_person_other_organization',
					            'entity_id' => $val->id,
					            'entity_type' => 'user_organization',
					            'parent1_entity_id' => $val->user_id,
					            'parent1_entity_type' => 'user_id',
					            'parent2_entity_id' => $val->organization_id,
					            'parent2_entity_type' => 'per_org',
					            'module_name' => 'exp_sp_administration_user',
					            'functionality_name' => 'updateUserOrgDetails',
					            'api_name' => $_REQUEST['apiname'],
					            'deleted_on' => now(),
					            'deleted_by' => $loggeduserId
					    );
					    deleted_log_entry($fields);
					}
					
				unset($form_state['input']['org']);
				unset($form_state['values']['org']);
				unset($form_state['input']['org_id']);
				unset($form_state['values']['org_id']);
			}
			}
			expDebug::dPrint(' $fieldsList[org_id] = ' . $fieldsList['org_id'] , 4);
			
			
			// Dotted organizations
			// TODO: Sort the dotted organizations by name and then save.
// 			$selectedOtherOrgs = $form_state['values']['oorgs']; // We cannot modify the original list as the same is required for form rebuild
// 			if($callFromAPI ==0){
// 				$noneSelectedIdx = array_search('none_selected', $selectedOtherOrgs);
// 				if ($noneSelectedIdx !== FALSE) {
// 					unset($selectedOtherOrgs[$noneSelectedIdx]);
// 				}
// 				$selectedOtherOrgs = $form_state['values']['load_multiselect_oorgs'];	// change by ayyappans for 33432: Issue in Users
// 			}
			if($callFromAPI ==0){
				$selectedOtherOrgs =explode(',', $form_state['values']['load_multiselect_oorgs']);
			}else if(($callFromAPI == 1 && isset($_REQUEST['other_organization']))) {
				$selectedOtherOrgs = explode(',',$form_state['values']['oorgs']);
			}
			//65348: Need to provide other organizations and other managers information of a user to API response Start
			$default_oth_organization = getUserOtherOrganizationIds($form_state['values']['id']);
			$oth_org_str = implode(',', $default_oth_organization);
			$default[0]->dotted_org_id = $oth_org_str;
			expDebug::dPrint('Default user organization = ' . print_r($default_oth_organization, true) , 4);
			$organization_arr_diff = array_diff_assoc($selectedOtherOrgs, $default_oth_organization);
			$organization_arr_diff1 = array_diff_assoc($default_oth_organization,$selectedOtherOrgs);
			expDebug::dPrint('Diff user organization = ' . print_r($organization_arr_diff, true) , 4);
			expDebug::dPrint('Diff user organization1 = ' . print_r($organization_arr_diff1, true) , 4);
			if(count($default_oth_organization)<=0){
				if(count($selectedOtherOrgs)>0){
					foreach ($selectedOtherOrgs as $organization_id){
						if(!empty($organization_id)){
							$usrOthOrg  = db_insert('slt_person_other_organization');
							$usrOthOrg->fields(array(
									'user_id' => $form_state['values']['id'],
									'organization_id' =>$organization_id,
									'created_by' => $updateUserId,
									'created_on' => now(),
							));
							expDebug::dPrintDBAPI('$$usrOthOrg---->' , $usrOthOrg);
							$usrOthOrg->execute();
			}
				}
			}
				$oth_org_str_ad_tr = implode(',',$selectedOtherOrgs);
				$formFieldsListOO = array('dotted_org_id');
				$formFieldsListOTHORG = array('dotted_org_id'=>$oth_org_str_ad_tr);
				make_audit_trail_entries($formFieldsListOTHORG, $updateUserId, $default, $form_state['values']['id'], $action, 'cre_sys_obt_usr', 'exp_sp_administration_user','updateOtherOrganizationDetails',$formFieldsListOO);
				}
			elseif (!empty($selectedOtherOrgs) && (count($organization_arr_diff)>0 || count($organization_arr_diff1)>0)) {
				if(count($selectedOtherOrgs) > 0){
				    // Get the fields before deleting
				    $loggeduserId = getSltpersonUserId();
				    $select = db_select('slt_person_other_organization','usrothorg');
				    $select->addField('usrothorg','id','id');
				    $select->addField('usrothorg','user_id','user_id');
				    $select->addField('usrothorg','organization_id','organization_id');
				    $select->condition('user_id', $form_state['values']['id']);
				    $select->condition('is_direct', 'N', '=');
				    $usrOrgInfo=$select->execute()->fetchAll();
				    
					$selectdelete = db_delete('slt_person_other_organization');
					$selectdelete->condition('user_id',$form_state['values']['id']);
					$selectdelete->condition('is_direct','N','=');
					expDebug::dPrintDBAPI('delete userlist1 ' , $selectdelete);
					$selectdelete->execute();
					
					foreach ($usrOrgInfo as $val){
					    // Make deleted log entry
					    $fields = array(
					            'table_name' => 'slt_person_other_organization',
					            'entity_id' => $val->id,
					            'entity_type' => 'user_organization',
					            'parent1_entity_id' => $val->user_id,
					            'parent1_entity_type' => 'user_id',
					            'parent2_entity_id' => $val->organization_id,
					            'parent2_entity_type' => 'per_oth_org',
					            'module_name' => 'exp_sp_administration_user',
					            'functionality_name' => 'updateUserOrgDetails',
					            'api_name' => $_REQUEST['apiname'],
					            'deleted_on' => now(),
					            'deleted_by' => $loggeduserId
					    );
					    deleted_log_entry($fields);
					}
					
					foreach ($selectedOtherOrgs as $organization_id){
						if(!empty($organization_id)){
							$usrOthOrg  = db_insert('slt_person_other_organization');
							$usrOthOrg->fields(array(
									'user_id' => $form_state['values']['id'],
									'organization_id' =>$organization_id,
									'created_by' => $updateUserId,
									'created_on' => now(),
							));
							expDebug::dPrintDBAPI('$$usrOthOrg---->' , $usrOthOrg);
							$usrOthOrg->execute();
			}
			}
				}
				$oth_org_str_ad_tr = implode(',',$selectedOtherOrgs);
				$formFieldsListOO = array('dotted_org_id');
				$formFieldsListOTHORG = array('dotted_org_id'=>$oth_org_str_ad_tr);
				make_audit_trail_entries($formFieldsListOTHORG, $updateUserId, $default, $form_state['values']['id'], $action, 'cre_sys_obt_usr', 'exp_sp_administration_user','updateOtherOrganizationDetails',$formFieldsListOO);
			}
			//65348: Need to provide other organizations and other managers information of a user to API response End

			// Location

			if (!empty($form_state['storage']['acvalues']['location'])) {
				$fieldsList['preferred_loc_id'] = $form_state['storage']['acvalues']['location']['id'];
				$form_state['values']['location'] = $form_state['storage']['acvalues']['location']['name'];
			}
			else {
				if($callFromAPI) {
					unset($fieldsList['preferred_loc_id']);
				}else {
					$fieldsList['preferred_loc_id'] = NULL;
				}
				//$fieldsList['preferred_loc_id'] = NULL;
				$form_state['values']['location'] = '';
			}

			/* Added by Sep 25, 2015  - Currency */
			$default_profile_currency = getCurrencyDefDetails();
			$fieldsList['preferred_currency'] = NULL;
			$userProfileCurrency = trim($form_state['values']['preferred_currency']);
			$usercurrency = empty($userProfileCurrency) ? $default_profile_currency : $userProfileCurrency;
			if($callFromAPI) {
				if(isset($_REQUEST['preferred_currency'])) {
					$fieldsList['preferred_currency'] = trim($form_state['values']['preferred_currency']);
				}else {
					unset($fieldsList['preferred_currency']);
				}
			}elseif (!empty($usercurrency)) {
				$fieldsList['preferred_currency'] = $usercurrency; // Returns the first key of the array (code)
			}

			// Updated by/on
			$fieldsList['updated_by'] = $updateUserId;
			$fieldsList['updated_on'] = now();

			// Select the fields and the values to be inserted
			$updateStmt->fields($fieldsList);
			$updateStmt->condition('id', $form_state['values']['id']);

			expDebug::dPrint(' $fieldsList = ' . print_r($fieldsList, true) , 4);
			expDebug::dPrintDBAPI(' $updateStmt SQL = ' , $updateStmt);

			// Execute the update statement.
			$numUpdated = $updateStmt->execute();
			
					
			//48587: Getting issue when the user instructor role is removed in administration/people/user
			if($default[0]->is_instructor =='Y' && $fieldsList['is_instructor']=='N')
			{
				$select = db_select('slt_groups','grp');
				$select->addField('grp', 'id','grpid');
				$select->condition('grp.is_instructor','Y' ,'=');
				$select->where('FIND_IN_SET('.$form_state['values']['id'].',added_users)>0');
				expDebug::dPrintDBAPI('Fetch User added  Id based user role as instructor : ' , $select);
				$addedGroupId_Arr =$select->execute()->fetchAll();
				expDebug::dPrint(' Fetch User added  Id based user role as instructor Result = ' . print_r($addedGroupId_Arr, true) , 4);
				if(count($addedGroupId_Arr)>0){
					include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
					foreach ($addedGroupId_Arr as $groupObj){
						updateRemoveUser($groupObj->grpid,$form_state['values']['id']);
					}
				}
			}

			//0048472: Once I removed manager role, still "Team" module showing.
			if($default[0]->is_manager =='Y' && $fieldsList['is_manager']=='N')
			{
				//50013: Getting issue when the user manager role is removed in administration/people/user
				$select = db_select('slt_groups','grp');
				$select->addField('grp', 'id','grpid');
				$select->condition('grp.is_manager','Y' ,'=');
				$select->where('FIND_IN_SET('.$form_state['values']['id'].',added_users)>0');
				expDebug::dPrintDBAPI('Fetch User added  Id based user role as Manager : ' , $select);
				$addedGroupId_Arr =$select->execute()->fetchAll();
				expDebug::dPrint(' Fetch User added  Id based user role as Manager Result = ' . print_r($addedGroupId_Arr, true) , 4);
				if(count($addedGroupId_Arr)>0){
					include_once(drupal_get_path('module', 'exp_sp_administration_groups') .'/exp_sp_administration_groups.inc');
					foreach ($addedGroupId_Arr as $groupObj){
						updateRemoveUser($groupObj->grpid,$form_state['values']['id']);
					}
				}
				//$personid = '';
				//60208: Other Manager is not getting removed in the reportee profile
				$userUpdate=db_query('UPDATE slt_person SET manager_id = NULL  WHERE manager_id='.$form_state['values']['id']);
				
				// Get the fields before deleting
				$loggeduserId = getSltpersonUserId();
					
				$select = db_select('slt_person_other_manager','usrothman');
				$select->addField('usrothman','id','id');
				$select->addField('usrothman','user_id','user_id');
				$select->addField('usrothman','manager_id','manager_id');
				$select->addField('usrothman','is_direct','is_direct');
				$select->condition('manager_id', $personid);
				$usrManInfo=$select->execute()->fetchAll();
				
				$deleteStmt = db_delete('slt_person_other_manager');
				$deleteStmt->condition('manager_id', $personid);
				//$deleteStmt->condition('is_direct','N','=');
				$deleteStmt->execute();
				
				foreach($usrManInfo as $val){
				    if($usrManInfo->is_direct == 'N'){
				        $mgr_type = 'per_oth_mgr';
				    }
				    else if($usrManInfo->is_direct == 'Y'){
				        $mgr_type = 'per_mgr';
				    }
				    // Make deleted log entry
				    $fields = array(
				            'table_name' => 'slt_person_other_manager',
				            'entity_id' => $val->id,
				            'entity_type' => 'user_manager',
				            'parent1_entity_id' => $val->user_id,
				            'parent1_entity_type' => 'user_id',
				            'parent2_entity_id' => $val->manager_id,
				            'parent2_entity_type' => $mgr_type,
				            'module_name' => 'exp_sp_administration_user',
				            'functionality_name' => 'updateUserOrgDetails',
				            'api_name' => $_REQUEST['apiname'],
				            'deleted_on' => now(),
				            'deleted_by' => $loggeduserId
				    );
				    deleted_log_entry($fields);
				}
				
				//expDebug::dPrint(' is_manage form value = ' . print_r($form_state['values']['roles'], true) , 4);
				
				//$userUpdate=db_query('UPDATE slt_person SET dotted_mngr_id = TRIM(BOTH \',\' FROM REPLACE(REPLACE(CONCAT(\',\',REPLACE(dotted_mngr_id, \',\', \',,\'), \',\'),\','.$personid.',\', \'\'), \',,\', \',\')) WHERE FIND_IN_SET('.$personid.', dotted_mngr_id)');
				//expDebug::dPrintDBAPI('Othermanager update query', $userUpdate);
			}
			//7306: Support of users to have more than 1 job role association
			$jobRole_arr = $form_state['values']['load_multiselect_jobrole'];
			//50180: Job roles is not capturing in audit_trail table when assigning or removing job roles to user
			$default_job_role = getUserAttributeValues($form_state['values']['id']);
			$job_role_str = implode(',', $default_job_role);
			$default[0]->job_role = $job_role_str;
			if($callFromAPI == 0 || ($callFromAPI == 1 && (isset($_REQUEST['jobrole'])|| isset($userSSOAttributes['JobRole'])))) {
					
			    // Get the fields before deleting
			    $loggeduserId = getSltpersonUserId();
			    
			    $select = db_select('slt_person_jobrole_mapping','perjrmap');
			    $select->addField('perjrmap','id','id');
			    $select->addField('perjrmap','user_id','user_id');
			    $select->addField('perjrmap','job_role','job_role');
			    $select->condition('user_id',$form_state['values']['id']);
			    $jrmapInfo=$select->execute()->fetchAll();
			    
					$selectdelete = db_delete('slt_person_jobrole_mapping');
					$selectdelete->condition('user_id',$form_state['values']['id']);
					expDebug::dPrintDBAPI('delete userlist1 ' , $selectdelete);
					$selectdelete->execute();
					
					foreach($jrmapInfo as $val){
					    // Make deleted log entry
					    $fields = array(
					            'table_name' => 'slt_person_jobrole_mapping',
					            'entity_id' => $val->id,
					    		'custom0' => $val->job_role,
					            'entity_type' => 'user_jobrole_map',
					            'parent1_entity_id' => $val->user_id,
					            'parent1_entity_type' => 'user_id',
					            'module_name' => 'exp_sp_administration_user',
					            'functionality_name' => 'updateUserOrgDetails',
					            'api_name' => $_REQUEST['apiname'],
					            'deleted_on' => now(),
					            'deleted_by' => $loggeduserId
					    );
					    deleted_log_entry($fields);
					}
					
				if(! empty($jobRole_arr)) {
				foreach ($jobRole_arr as $job_role_code){
					if(!empty($job_role_code)){
						$usrJobRole  = db_insert('slt_person_jobrole_mapping');
						$usrJobRole->fields(array(
								'user_id' => $form_state['values']['id'],
								'job_role' =>$job_role_code,
								'created_by' => $updateUserId,
								'created_on' => now(),
						));
						expDebug::dPrintDBAPI('$$usrJobRole' , $usrJobRole);
						$usrJobRole->execute();
					}
				}
			}
			}
			expDebug::dPrint('SELECTED JOB ROLE---->'.print_r($jobRole_arr,1),4);
			
			if($managerId){
				$updManager = db_update('slt_person');
				$updManager->condition('id', $managerId);
				$updManager->fields(array(
						'is_manager'      => 'Y'
				));
				$updManager->execute();
				/*role tables entry for manager*/
				/* $roleId =2;


				$select = db_select('role','role');
		  $select->join('slt_security_role','srole','role.name=srole.name');
		  $select->addField('role','rid','rid');
				$select->condition('srole.id',$roleId,'=');
				expDebug::dPrintDBAPI('Fetch role Id ' , $select);
				$srleid = $select->execute()->fetchField();

				$select1 = db_select('users','usr');
		  $select1->join('slt_person','per','per.user_name=usr.name');
		  $select1->addField('usr','uid','uid');
				$select1->condition('per.id',$managerId,'=');
				expDebug::dPrintDBAPI('Fetch role Id managerID-->' , $select1);
				$drupalUid = $select1->execute()->fetchField();

				//Users role Insert
				$selectchk = db_select('users_roles', 'urole');
				$selectchk->addExpression('count(1)','cnt');
				$selectchk->condition('urole.uid',$drupalUid,'=');
				$selectchk->condition('urole.rid',$srleid,'=');
				$count = $selectchk->execute()->fetchField();

				expDebug::dPrint('count value'.print_r($count,true));
				if($count<1){
				$insertStmt = db_insert('users_roles');
				$insertStmt->fields(array(
						'uid' => $drupalUid,
						'rid' => $srleid,
				));
				expDebug::dPrintDBAPI('$insert user Id  ManagerID-->' , $insertStmt);
				$insertStmt->execute();

				//Update security role

				$existingUsersList = getRoleUserListField($roleId);
				$newuserList = ((empty($existingUsersList)) ? '' : $existingUsersList . ',') .$managerId;
				$updateUser = db_update('slt_security_role');
				$updateUser->condition('id', $roleId, '=');
				$updateUser->fields(array(
						'userlist'       => $newuserList ,
						'updated_on'    => now(),
						'updated_by'    => $updateUserId
				));
				expDebug::dPrintDBAPI('Insert UserList for security role ', $updateUser);
				$updateUser->execute();
				} */
			}
			expDebug::dPrint("default value-----------------> ".print_r($default[0],true),5);
			$defaultUserType = $default[0]->user_type;
			$defaultJobRole  = $default[0]->job_role;
			$defaultOrgId    = $default[0]->org_id;
			$defaultEmptye	 = $default[0]->employment_type;
			$defaultDeptcode = $default[0]->dept_code;
			$defaultCountry = $default[0]->country;
			$defaultState = $default[0]->state;
			$defaultManager = $default[0]->is_manager;
			$defaultInstructor = $default[0]->is_instructor;
			$defaultLanguage  = $default[0]->preferred_language;
			$defaultCurrency  = $default[0]->preferred_currency;




			// Remove tmp space
			unset($form_state['storage']['acvalues']);
			unset($form_state['storage']['dates']);
			// Field list values for multiple audit trail entries
			$formFieldsList = array('user_type', 'job_role', 'job_title', 'employee_no', 'employment_type', 'is_manager',
				'is_instructor', 'dept_code', 'preferred_loc_id', 'manager_id', 'org_id', 'preferred_currency','sms_alert');
			/*if ($callFromAPI == 0) {
			 $formFieldsList[] = 'manager_id';
			$formFieldsList[] = 'org_id';
			}*/
			$updateUserId = ($updateUserId == 'Feed') ? 1 : $updateUserId;
			expDebug::dPrint(' end $form_state[input] = ' . print_r($form_state['input'], true) , 5);
			expDebug::dPrint(' end $form_state[values] = ' . print_r($form_state['values'], true) , 5);
			expDebug::dPrint(' end $form_state[storage] = ' . print_r($form_state['storage'], true) , 5);
			make_audit_trail_entries($fieldsList, $updateUserId, $default, $personid, $action, 'cre_sys_obt_usr', 'exp_sp_administration_user',
					'updateUserOrgDetails', $formFieldsList);
			expDebug::dPrint("default value-----------------> ".print_r($default[0],true),5);
			expDebug::dPrint("fefault jo defaultEmptye ".print_r($fieldsList,true),5);
			expDebug::dPrint("fefault jo fieldsList['employment_type'] ".print_r($fieldsList['employment_type'],true),5);
			// To sync solr data
			syncSolrData('MasterEnrollment');
			
		}
		catch (Exception $ex) {
			$txn->rollback();
			watchdog_exception('useradmin', $ex);
			throw $ex;
		}
		unset($txn);
		/*
		 *	0032377 : Starting auto update for compliance course registration
		 *  83856: SSO Login - Separate Mandatory/compliance registration and Update process into background jobs - Below code commented
		*/

		/*$attributes = getEnabledAttrForVisibility();// list of attributes in site configuration
		expDebug::dPrint("fefault jo defaultEmptye ".print_r($fieldsList,true),5);
		expDebug::dPrint("sadsdsdsdsadas".print_r($attributes,true),5);
		foreach ($attributes as $key=>$attribute){
			if($attribute == 'department'){
				$attr = 'dept_code';
			}
			switch($attribute){
				case 'department' :
					$attributes[$key] = 'dept_code';
					break;
				case 'language' :
					$attributes[$key] = 'preferred_language';
					break;
				default :
					$attributes[$key]  = $attribute;
					break;
			}
		}
		$UpdEnrollid = getUpdatedEnrollmentsIdRevised($attributes,$form_state['values']['id'],$default[0],$fieldsList);
 		$UpdManEnrollid = getUpdatedEnrollmentsIdForMandatory($attribute,$form_state['values']['id'],$default[0]->$attr,$fieldsList[$attribute]);
 		if(!empty($UpdEnrollid) || !empty($UpdManEnrollid)){
 			getComplianceCourseEnrollAutoCancel($UpdEnrollid,$form_state['values']['id']);
 		}
		
		
		if($attribute != 'role' && $attribute != 'hire_date'){
		expDebug::dPrint('Role attribute check default  ' . $attribute . ' value : ' . $default[0]->$attr . 'new value: ' . $fieldsList[$attr], 4);		
	  	if($default[0]->$attr != $fieldsList[$attr]){
	  		$UpdEnrollid = getUpdatedEnrollmentsId($attribute,$form_state['values']['id'],$default[0]->$attr,$fieldsList[$attribute]);
	  		$UpdManEnrollid = getUpdatedEnrollmentsIdForMandatory($attribute,$form_state['values']['id'],$default[0]->$attr,$fieldsList[$attribute]);
	  		if(!empty($UpdEnrollid) || !empty($UpdManEnrollid)){
	  			getComplianceCourseEnrollAutoCancel($UpdEnrollid,$form_state['values']['id']);
	  		}
	  	}
			}else if($attribute == 'role'){
				if($defaultManager != $fieldsList['is_manager']){
					$UpdEnrollid = getUpdatedEnrollmentsId('is_manager',$form_state['values']['id'],$defaultManager,$fieldsList['is_manager']);
					$UpdManEnrollid = getUpdatedEnrollmentsIdForMandatory('is_manager',$form_state['values']['id'],$defaultManager,$fieldsList['is_manager']);
					if(!empty($UpdEnrollid) || !empty($UpdManEnrollid))
						getComplianceCourseEnrollAutoCancel($UpdEnrollid,$form_state['values']['id']);
				}
				if($defaultInstructor != $fieldsList['is_instructor']){
					$UpdEnrollid = getUpdatedEnrollmentsId('is_instructor',$form_state['values']['id'],$defaultInstructor,$fieldsList['is_instructor']);
					$UpdManEnrollid = getUpdatedEnrollmentsIdForMandatory('is_instructor',$form_state['values']['id'],$defaultInstructor,$fieldsList['is_instructor']);
					if(!empty($UpdEnrollid) || !empty($UpdManEnrollid))
						getComplianceCourseEnrollAutoCancel($UpdEnrollid,$form_state['values']['id']);
				}
			}else if ($attribute == 'hire_date') {
// 				$UpdEnrollid = getUpdatedEnrollmentsId('hire_date',$form_state['values']['hire_date'],$default[0]->$attr,$fieldsList[$attribute]);
// 				$UpdManEnrollid = getUpdatedEnrollmentsIdForMandatory('hire_date',$form_state['values']['hire_date'],$default[0]->$attr,$fieldsList[$attribute]);
// 				if(!empty($UpdEnrollid) || !empty($UpdManEnrollid))
// 					getComplianceCourseEnrollAutoCancel($UpdEnrollid,$form_state['values']['id']);
			}

		//}*/

		if (!empty($selectedOtherManagers)) {
			expDebug::dPrint('selected user managers = ' . print_r($selectedOtherManagers, true) , 4);
			$otherMngrIdChanged = 0;
			$otherMngrId = explode(',',$default[0]->dotted_mngr_id);
			foreach($selectedOtherManagers as $key => $value){
				$defaultMngrName[] = db_query("SELECT full_name from slt_person where id=:uid",array(":uid"=>$value))->fetchField();
				if(!(in_array($value,$otherMngrId))){
					$otherMngrIdChanged = 1;
				}
			}
			$newManagerName = implode(',',$defaultMngrName);
		}

		if (!empty($selectedOtherOrgs)) {
			$otherOrgIdChanged = 0;
			$otherOrgId = explode(',',$default[0]->dotted_org_id);
			$selectedOtherOrgs = explode(',',$selectedOtherOrgs);
			foreach($selectedOtherOrgs as $key => $value){
				$defaultOrgName[] = db_query("SELECT name from slt_organization where id=:uid",array(":uid"=>$value))->fetchField();
				if(!(in_array($value,$otherOrgId))){
					$otherOrgIdChanged =1;
				}
			}
			$newOrgName = implode(',',$defaultOrgName);
		}
		
		if($action == 'Updated' && $form_state['values']['request_source'] != 'saml' ){
			expDebug::dPrint('updating existing enrollments in user update');
			/*
			expDebug::dPrint('normal user update mandatory/compliance part begins', 4);
			$upd_user_id = $form_state['values']['id'];
			$results = getGrpEntityIdListByUser($upd_user_id,'cre_sys_inv_man');
			$comp_status = array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_exp');
			$overlAllStatus = array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_exp');
			$created_by = array($upd_user_id,2);
			$entityArr = array();
			foreach($results as $arr){
				if($arr->entity_type == 'cre_sys_obt_cls')
					$entityArr['class'][] = $arr->entity_id;
				if($arr->entity_type == 'cre_sys_obt_cur' || $arr->entity_type == 'cre_sys_obt_trn' || $arr->entity_type == 'cre_sys_obt_crt')
					$entityArr['tp'][] = $arr->entity_id;
			}
       
			db_query("update slt_enrollment set mandatory=null,updated_on=now() where user_id=:user_id AND created_by IN (:created_by) AND comp_status NOT IN (:comp_status) AND master_enrollment_id is null", array(":user_id"=>$upd_user_id,":comp_status"=>$comp_status,":created_by"=>$created_by));
			db_query("update slt_master_enrollment set mandatory=null,updated_on=now() where user_id=:user_id AND created_by IN (:created_by) AND overall_status NOT IN (:overall_status)", array(":user_id"=>$upd_user_id,":overall_status"=>$overlAllStatus,":created_by"=>$created_by));
			if(count($entityArr['class']) > 0)
				db_query("update slt_enrollment set mandatory='Y',updated_on=now() where user_id IN (:user_id) AND class_id IN (:class_id) AND comp_status NOT IN (:comp_status) AND master_enrollment_id is null", array(":user_id"=>$upd_user_id, ":class_id"=>$entityArr['class'],":comp_status"=>$comp_status));
			if(count($entityArr['tp']) > 0)
				db_query("update slt_master_enrollment set mandatory='1',updated_on=now() where user_id IN (:user_id) AND program_id IN (:program_id) AND overall_status NOT IN (:overall_status)", array(":user_id"=>$upd_user_id, ":program_id"=>$entityArr['tp'],":overall_status"=>$overlAllStatus));
					
			//Complince update 
			$Complainceresult = getGrpEntityIdListByUser($upd_user_id,'cre_sys_inv_opt');	
			$entityArr1 = array();					
			foreach($Complainceresult as $arr){			
				$is_compliance = checkCourseClassCompliance($arr->entity_id,$arr->entity_type);
				if($is_compliance[0]->is_compliance == 1){
					if($arr->entity_type == 'cre_sys_obt_cls')
						$entityArr1['class'][] = $arr->entity_id;
					}
				}				
			$complianceAccess = compliance_access();			
			foreach ($complianceAccess as $class_id){
				$class_ids[] = $class_id->class_id;
			}
			$comUpdateQuery = "update slt_enrollment set is_compliance=null,updated_on=now() where user_id=:user_id AND comp_status NOT IN (:comp_status) AND master_enrollment_id is null";
			if (!empty($class_ids)) {
				$comUpdateQuery .= " AND class_id NOT IN (:class_id)";
				db_query($comUpdateQuery, array(":user_id"=>$upd_user_id,":comp_status"=>$comp_status, ":class_id"=>$class_ids,));
			} else {
				db_query($comUpdateQuery, array(":user_id"=>$upd_user_id,":comp_status"=>$comp_status));
			}
			if(count($entityArr1['class']) > 0){	
			 	db_query("update slt_enrollment set is_compliance='1',updated_on=now() where user_id IN (:user_id) AND class_id IN (:class_id) AND comp_status NOT IN (:comp_status) AND master_enrollment_id is null", array(":user_id"=>$upd_user_id, ":class_id"=>$entityArr1['class'],":comp_status"=>$comp_status));
			}
			*/
		}
		$defaultdbname = db_query("SELECT manager_id from slt_person where id=:uid",array(":uid"=>$form_state['values']['id']))->fetchField();
		if(!empty($form_state['values']['org_id']) && $default[0]->org_id != $form_state['values']['org_id']){
			insertNotificationForUser('notify_129',$form_state['values']['id'],$form_state['values']['org']);
		}
		if(!empty($defaultdbname) && $default[0]->manager_id != $form_state['values']['manager_id']){
			insertNotificationForUser('notify_130',$form_state['values']['id'],$form_state['values']['manager'], $defaultdbname);
		}
		if($otherMngrIdChanged){
			insertNotificationForUser('assign_dotted_manager',$form_state['values']['id'],$newManagerName);
		}
		if($otherOrgIdChanged){
			insertNotificationForUser('assign_dotted_organization',$form_state['values']['id'],$newOrgName);
		}
		syncSolrData('User');
		syncSolrData('Enrollment');
		// To sync solr data
		syncSolrData('MasterEnrollment');
		
	} catch (Exception $ex) {
		watchdog_exception('updateUserOrgDetails', $ex);
		expertusErrorThrow($ex);
	}
}
function checkInstructorAssignedSession($instructorId) {
	try {
		$timeZone = date_default_timezone(false);
		$defaultTimezone = ($timeZone == 'Asia/Kolkata') ? 'Asia/Calcutta' : $timeZone;
		$select = db_select('slt_course_class', 'cat');
		$select->addField('cat','id', 'cls_id');
		$select->addField('ses','timezone', 'ses_timezone');
	//	$select->addField('ses','timezone', 'ses_timezone');
		//$enrollQuery->addField('ses_ins','is_presenter', 'is_presenter');
		$select->addExpression("DATE_FORMAT(`ses`.`start_date`,_utf8'%d-%b-%Y')", 'session_start');
		$select->addExpression("CONCAT( DATE_FORMAT(ses.start_date, _utf8'%Y-%m-%d'),' ',ses.end_time,':00')", 'session_end_datetime');
		$select->leftjoin('slt_course_template', 'crs', 'crs.id = cat.course_id');
		$select->leftjoin('slt_course_class_session', 'ses', "ses.class_id = cat.id ");
		$select->leftjoin('slt_session_instructor_details', 'ses_ins', "ses_ins.class_id = cat.id ");
		$select->leftjoin('slt_location', 'loc', 'ses.location_id = loc.id');
		$select->where("ses.id = if((select count(1) from slt_session_instructor_details as ses_ins1 where ses_ins1.class_id=cat.id and ses_ins1.instructor_id =".$instructorId.")>0,
				(select id from slt_course_class_session where class_id=cat.id order by start_date desc, end_time desc limit 1), 0)");
		$select->condition('cat.status', array('lrn_cls_sts_can','lrn_cls_sts_del'),'NOT IN');
		$select->where("date(ses.start_date) 
>= IFNULL(CONVERT_TZ(date(now()), 'Asia/Calcutta' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),date(NOW()))");
		//$select->groupBy("ses.class_id having date(ses.start_date) >= IFNULL(CONVERT_TZ(date(now()), '".$defaultTimezone."' ,(select pro1.attr2 from slt_profile_list_items pro1 where pro1.code= ses.timezone)),NOW()"); 
		expDebug::dPrintDBAPI(' session instructor = ' ,$select);
		$result = $select->execute()->fetchAll();
		$recCount = count($result);
		return $recCount;
	} catch (Exception $ex) {
		watchdog_exception('checkInstructorAssignedSession', $ex);
		expertusErrorThrow($ex);
	}
}
function getOrganizationNameDetails($orgId, $codeRequired = 1){
	try {
  $orgDetails = getOrganizationDetails($orgId, array('name', 'number'));
  if(!empty($orgDetails)){
    if($codeRequired){
      return $orgDetails['name'].' ('.$orgDetails['number']. ')';
    } else {
      return $orgDetails['name'];
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('getOrganizationNameDetails', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * UserAddrEditFormWithCancelAndSaveClose() - Closure for newAddEditFormWithCancelAndSave() omplemented specially for handling e-signature save
 */
function UserAddrEditFormWithCancelAndSaveClose(&$form, $wrapperId = 'addedit-form-wrapper', $saveButtonId = 'save-button', $cancelButtonId = 'cancel-button', $expandPopup = FALSE, $saveTabIndex = '', $entityId = '') {
	try {
  expDebug::dPrint(' addEditFormWithCancelAndSaveClose() called.');

  // Remove the temporary storage for API's to exchange information during form creation
  unset($form['addedit_tmp_storage']);

  if (isset($saveTabIndex) && $saveTabIndex > 0) {
    $saveAttributes['tabindex'] = $saveTabIndex;
    $cancelAttributes['tabindex'] = $saveTabIndex+1;
  }

  // Add the Cancel button
  $ajaxCallbackCancel = 'addEditCancelRepaintForm';

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $cancelAttributes['class'] = array('addedit-edit-' . $cancelButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // For common styling of all addedit cancel buttons
  //$cancelAttributes['class'][] = 'addedit-edit-cancel';

  // Using this class to applying bg color to the save button
  $cancelAttributes['class'][] = 'admin-action-button-middle-bg';

  // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
  $cancelAttributes['class'][] = 'addedit-form-expertusone-throbber';
  // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
  $cancelAttributes['data-wrapperid'] = array($wrapperId);

  // A common class for all addedit form cancel button, for styling to appear as a link.
  $cancelAttributes['class'][] = 'exp-addedit-form-cancel-button';

  if(empty($entityId)){
    $cancelAttributes['onclick'] = 'Drupal.CTools.Modal.dismiss();return false;';
  }

  $form[$cancelButtonId] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="addedit-form-cancel-and-save-actions-row">' .
                   '<div class="addedit-form-cancel-container-actions ">',
  //'#suffix' =>   '<div class="admin-save-button-right-bg"></div>',
    '#value' => t('Close'),
    '#attributes' => $cancelAttributes,
    '#ajax' => array(
      'wrapper' => $wrapperId,
      'callback' => $ajaxCallbackCancel,
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

        // Add the Save button
        $ajaxCallbackSave = 'addEditRepaintForm';

        // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
        // each time the form is re-built.
        $saveAttributes['class'] = array('addedit-edit-' . $saveButtonId); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

        // For common styling of all addedit save buttons
        //$saveAttributes['class'][] = 'addedit-edit-save';

        // Using this class to applying bg color to the save button
        $saveAttributes['class'][] = 'admin-save-button-middle-bg';



        //Increase the width of the popup while clicking on save button
        if ($wrapperId != 'resource-addedit-form' && $wrapperId != 'organization-addedit-form' && $wrapperId != 'user-org-addedit-form'  && $wrapperId != 'user-addr-addedit-form' && $expandPopup) {
          $saveAttributes['onclick'][] = 'increasePopWidth();';
        }

        // A common class for all addedit form submit buttons and ajax enabled fields for showing expertusone throbber.
        $saveAttributes['class'][] = 'addedit-form-expertusone-throbber';
        // Communicate the wrapper to javascript for showing the expertusone throbber via HTML data tag
        $saveAttributes['data-wrapperid'] = array($wrapperId);

        if(module_exists('exp_sp_esignature') && isActiveEsignRegion()){
          $saveAttributes['onclick'][] = '$("body").data("learningcore").getEsignPopup({"popupDiv":"ctools-face-table","esignFor":"UserAddrAdmin"});return false;';

          $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' =>
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' => t('LBL141'),
	    '#attributes' => $saveAttributes,
          /* '#ajax' => array(
           'wrapper' => $wrapperId,
           'callback' => $ajaxCallbackSave,
           'method' => 'replace',
           'effect' => 'none', //	 'fade',
           'event'=>'click',
           'keypress' => true,
           'progress' => array(
           'type' => 'throbber',
           'message' => ''
           ),
           )*/
          );
        }else{
          // Using this onclick function to hide message div
          //$saveAttributes['onclick'][] = 'hideMessageInfo();';
          $form[$saveButtonId] = array(
	    '#type' => 'submit',
	    '#prefix' => '<div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div>',
	    '#suffix' =>
	               '<div class="admin-save-button-right-bg"></div></div>' .
	               '<div class="clearBoth"></div></div>' .
	             '<div class="clearBoth"></div></div>',
	    '#value' => t('LBL141'),
	    '#attributes' => $saveAttributes,
	    '#ajax' => array(
	      'wrapper' => $wrapperId,
	      'callback' => 'addEditRepaintForm',
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

        //$saveAttributes['onclick'] = 'hideMessageInfo();';
        $saveAttributes['class'] = array('common-esign-save-addr');//IMPORTANT!!!!!!! do not use this class anywhere else -- only for esign click ------- IMPORTANT!!!!!!!
        $form['common-esign-save-addr'] = array(
	      '#type' => 'submit',
	      '#value' => t('LBL141'),
      	  '#prefix' => '<span id = "esign-admin-div" style="display:none;">',
	      '#suffix' => '</span>',

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

		        $form['addedit-left-col-end'] = array(
    '#type' => 'markup',
    '#markup' => '</div><div class="clearBoth"></div>',
		        );
		        } catch (Exception $ex) {
		        	watchdog_exception('UserAddrEditFormWithCancelAndSaveClose', $ex);
		        	expertusErrorThrow($ex);
		        }
}

function addNewUserByRestAPI( &$form_state,$createuserid=""){
	try {
            //drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	//Added by Subin. Ticket #59698
	$default_profile_currency = getCurrencyDefDetails();
	$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
  	expDebug::dPrint(' addNewUserByRestAPI() drupal_user_id '.$form_state['values']['drupal_uid'] , 4);
  	if(isset($_POST['roles']) && $form_state['values']['roles'] == ''){
  	  $form_state['values']['roles'] = 'N';
  	}

/*  if (!empty($form_state['values']['first_name']) && (strlen($form_state['values']['first_name'])) > 50) {
  	$name_length = strlen($form_state['values']['first_name']);
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "First Name cannot be longer than 50 characters but is currently ".$name_length." characters long.";
  	return $errobj;
  }
  if (!empty($form_state['values']['last_name']) && (strlen($form_state['values']['last_name'])) > 50) {
  	$name_length = strlen($form_state['values']['last_name']);
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Last Name cannot be longer than 50 characters but is currently ".$name_length." characters long.";
  	return $errobj;
  } */

  if (existsUserEmail($form_state['values']['email'])) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Email id already exists.";
  	return $errobj;
    //return array((object)array('Id'=>'Email id already exists.'));
  }
  if (!validateEmail($form_state['values']['email'])) {
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "Email id is not valid.";
  	return $errobj;
    //return array((object)array('Id'=>'Email id is not valid.'));
  }
  if(!empty($form_state['values']['status'])){
  	$roleErrmsg = checkValidUserAtrribute($form_state['values']['status'],'cre_usr_sts','status');
  	if($roleErrmsg !== TRUE){
  		return $roleErrmsg;
  	}
  }

  if (!empty($form_state['values']['user_name'])) {
  	if (strpos($form_state['values']['user_name'], ' ') !== FALSE) { // There should be no space in username (there should be other checks as well TBC)
  		$errobj=new stdClass();
	  	$errobj->isValidateError = 1;
	  	$errobj->errcode = 'L_012';
	  	$errobj->errormsg = "Enter the username without space.";
	  	return $errobj;
  		//return array((object)array('Id'=>'Enter the username without space.'));
  	} else if(existsUserName($form_state['values']['user_name'], $form_state['values']['drupal_uid'])) { // Confirm user name has not already been used
  		$errobj=new stdClass();
  		$errobj->isValidateError = 1;
  		$errobj->errcode = 'L_012';
  		$errobj->errormsg = "UserName already exists.";
  		return $errobj;
  		//return array((object)array('Id'=>'UserName already exists.'));
  	}
  }

  if(!empty($form_state['values']['manager_id'])){
  	$numCheck = is_numeric($form_state['values']['manager_id']);
  	if($numCheck)
  		$checkIdValid = validatePersonID($form_state['values']['manager_id']);
 		if(empty($checkIdValid) || $checkIdValid == '' || empty($numCheck)){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Manager Id.";
    	return $errobj;
      //return array((object)array('Id'=>'Invalid Manager Id.'));
    }
  }
  if(!empty($form_state['values']['other_managers'])){
    $omanagers      = explode(',',$form_state['values']['other_managers']);
    $countmangers   = count($omanagers);
    for($i=0;$i<$countmangers;$i++){
    	$ismannum = is_numeric($omanagers[$i]);
  		if($ismannum)
      	$mang     = is_manager($omanagers[$i]);
      $is_mang  = $form_state['values']['manager_id'];
      if($mang == false || (!empty($is_mang) && $is_mang == $omanagers[$i])) {
      	$errobj=new stdClass();
	    	$errobj->isValidateError = 1;
	    	$errobj->errcode = 'L_012';
	    	$errobj->errormsg = "Invalid Other Manager Id.";
	    	return $errobj;
        //return array((object)array('Id'=>'Invalid Other Manager Id.'));
      }
    }
    $form_state['values']['other_managers'] = implode(',',array_unique($omanagers));
  }
  if(!empty($form_state['values']['org_id'])){
    $orgcheck = is_organization($form_state['values']['org_id']);
    if($orgcheck == false){
    	$errobj=new stdClass();
    	$errobj->isValidateError = 1;
    	$errobj->errcode = 'L_012';
    	$errobj->errormsg = "Invalid Organization Id.";
    	return $errobj;
      //return array((object)array('Id'=>'Invalid Organization Id.'));
    }
  }
  if(!empty($form_state['values']['other_organization'])){
    $other_org      = explode(',',$form_state['values']['other_organization']);
    $countorg   = count($other_org);
    for($i=0;$i<$countorg;$i++){
      $org     = is_organization($other_org[$i]);
      $is_org  = $form_state['values']['org_id'];
      if($org == false || (!empty($form_state['values']['org_id']) && $is_org == $other_org[$i])) {
      	$errobj=new stdClass();
      	$errobj->isValidateError = 1;
      	$errobj->errcode = 'L_012';
      	$errobj->errormsg = "Invalid Other Organization Id.";
      	return $errobj;
        //return array((object)array('Id'=>'Invalid Other Organization Id.'));
      }
    }
    $form_state['values']['other_organization'] = implode(',',array_unique($other_org));
  }
	if(!empty($form_state['values']['jobrole'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['jobrole'],'cre_usr_jrl_','job role');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
	}
	if(!empty($form_state['values']['usertype'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['usertype'],'cre_usr_ptp_','user type');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
	}
	if(!empty($form_state['values']['jobtitle'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['jobtitle'],'cre_usr_jtl_','job title');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
	}
	if(!empty($form_state['values']['empltype'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['empltype'],'cre_usr_etp_','employment type');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
	}
	if(!empty($form_state['values']['deptcode'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['deptcode'],'cre_usr_dpt_','department');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
	}
	if(!empty($form_state['values']['preferred_language'])) {
		$roleErrmsg = checkValidUserAtrribute($form_state['values']['preferred_language'],'cre_sys_lng_','preferred language');
		if($roleErrmsg !== TRUE){
			return $roleErrmsg;
		}
	}
	$preferred_currency = $form_state['values']['preferred_currency'];
	if(!empty($preferred_currency)) {
		$selectQuery = db_select('slt_profile_list_items', 'spli');
		$selectQuery->addField('spli','attr1');
		$selectQuery->condition('spli.is_active', 'Y', '=');
		$selectQuery->condition('spli.attr3', 'Y', '=');
		$selectQuery->condition('spli.code', $preferred_currency, '=');
		$preferred_currency = $selectQuery->execute()->fetchField();
		if(empty($preferred_currency))
		{
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->field = 'currency_type';
			$errobj->errormsg = "The given preferred currency is invalid or inactive status";
			return $errobj;
		}
		else
			$form_state['values']['preferred_currency'] = $preferred_currency;
	}else{
            $form_state['values']['preferred_currency'] = $default_profile_currency;
	}
	if(!empty($form_state['values']['preferred_timezone'])) {
		$roleErrmsg = checkValidUserAtrribute($form_state['values']['preferred_timezone'],'cre_sys_tmz_','preferred timezone');
		if($roleErrmsg !== TRUE){
			return $roleErrmsg;
		}
	}
	if(!empty($form_state['values']['employee_no'])){
		$roleErrmsg = existsUserEmpid($form_state['values']['employee_no'],$form_state['values']['id'],'api');
		if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
		}
	}
	if(!empty($form_state['values']['country'])){
		$roleErrmsg = is_valid_country($form_state['values']['country']);
		if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
		}
	}
	if(!empty($form_state['values']['state'])){
		if(empty($form_state['values']['country'])){
			$countryErrmsg=new stdClass();
			$countryErrmsg->isValidateError = 1;
			$countryErrmsg->errormsg = "Country field should not be empty";
			return $countryErrmsg;
		}
		$roleErrmsg = is_valid_state($form_state['values']['country'],$form_state['values']['state']);
		if($roleErrmsg !== TRUE){
			return $roleErrmsg;
		}
	}
	if(!empty($form_state['values']['roles'])) {
		//$role = array_unique(explode(',',$form_state['values']['roles']));
		$role = $form_state['values']['roles'];
		if($role != 'manager' && $role !='instructor'
					&& $role != 'instructor,manager' && $role != 'manager,instructor' && $role != 'N'){
			$roleErrmsg=new stdClass();
			$roleErrmsg->isValidateError = 1;
			$roleErrmsg->errormsg = "The given role is invalid";
			return $roleErrmsg;
		}
	}
	if(!empty($form_state['values']['roles'])) {
			$role = $form_state['values']['roles'];
			if($role =='instructor' || $role == 'instructor,manager' || $role == 'manager,instructor'){
	if(!empty($form_state['values']['webex_name']) && empty($form_state['values']['webex_pass'])){
		$webexnameErrmsg=new stdClass();
		$webexnameErrmsg->isValidateError = 1;
		$webexnameErrmsg->errormsg = "WebEx pass field should not be empty";
		return $webexnameErrmsg;
	}
	if(empty($form_state['values']['webex_name']) && !empty($form_state['values']['webex_pass'])){
		$webexpassErrmsg=new stdClass();
		$webexpassErrmsg->isValidateError = 1;
		$webexpassErrmsg->errormsg = "WebEx name field should not be empty";
		return $webexpassErrmsg;
}
}}
	if(!empty($form_state['values']['hire_date'])) {
		if(!validateHireDateFormat($form_state['values']['hire_date'])){
			$hireDateErrmsg=new stdClass();
			$hireDateErrmsg->isValidateError = 1;
			$hireDateErrmsg->errormsg = "Please enter a valid date in this mm/dd/yyyy format.";
			return $hireDateErrmsg;
		}
	}

	if(!empty($form_state['values']['hire_date'])) {
		if(!validateDateMonthYear($form_state['values']['hire_date'],'m/d/Y',now(),5)){
			$hireDateErrmsg=new stdClass();
			$hireDateErrmsg->isValidateError = 1;
			$hireDateErrmsg->errormsg = "Hire Date should be within range of 5 years in future from current date.";
			return $hireDateErrmsg;
		}
	}
	

	 //Added by Subin ->> User Address Validation
	 $verifyGivenAddress = getAddressVerified($form_state);
	 $is_verified_add = $verifyGivenAddress['is_verified_add'];
	 $is_valid_add = $verifyGivenAddress['is_valid_add'];
	 $invalid_add_reason = $verifyGivenAddress['invalid_add_reason'];
	 //generate form_state values for the is_verified_add, is_valid_add, & invalid_add_reason
	  $form_state['values']['is_verified_add'] = $is_verified_add;
	  $form_state['values']['is_valid_add'] = $is_valid_add;
	  $form_state['values']['invalid_add_reason'] = $invalid_add_reason;
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $form=array();
  $form["form_id"]="";
  $form_state['storage']['acvalues']['manager']['id'] = $form_state['values']['manager_id'];
  $form_state['storage']['acvalues']['org']['id'] = $form_state['values']['org_id'];
  $form_state['values']['omanagers'] = $form_state['values']['other_managers'];
  $form_state['values']['oorgs'] = $form_state['values']['other_organization'];
  if(empty($form_state['values']['preferred_language']))
  		$form_state['values']['preferred_language']        = NULL; /*!empty($form_state['values']['preferred_language']) ? $form_state['values']['preferred_language'] :  getSiteDefaultLanguageCode(); */
 
 //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
	  $ret_tmp = getcustomattributemodulestatus();
	  if($ret_tmp == true){
      include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
      $custom_attr_error = validateCustomAttribute($form_state,'cre_usr');
        expDebug::dPrint('user page custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
      if($custom_attr_error != '') {
        $customErrmsg=new stdClass();
        $customErrmsg->isValidateError = 1;
        $customErrmsg->errormsg = $custom_attr_error;
        return $customErrmsg;
      }
  } //#custom_attribute_0078975 - End Check module status
  
  //36820: Irrelevant escape character is getting inserted on processing through API
  $form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
  $userId =  addNewUser($form,$form_state,$createuserid,1);
  if($userId){
    if($form_state['values']['status'] == 'cre_usr_sts_atv' && empty($form_state['values']['password']) && ($form_state['values']['sendnotification'] != 'N')){
      expDebug::dPrint('addNewUserByRestAPI() Notification id '.$form_state['values']['email'] , 4);
      sendResetPasswordLink('register_admin_created', $form_state['values']['email']);
    }
    updateUserAddressDetails($form, $form_state, $createuserid ,'Inserted');
    updateUserOrgDetails($form, $form_state, $createuserid, $userId, 'Inserted', 1);
    expDebug::dPrint(' addNewUserByRestAPI created user:' . $userId ,4);
    expDebug::dPrint("SEcurity Role :: !!!".$form_state['values']['securityrole']."!!!");
    // change by ayyappans for 42239: Users roles table not updated when creation user from Create User rest api
    $newGrpList = getGroupsDetails($userId, '' ,1,1,0);
    $drupalUserId = getDrupalIdOfUser($userId);
    $$newGrpListarr = array();
    $delUserGrp = 'Delete'; // While populat flat table the existing entry should be delete once/very first call
    foreach($newGrpList as $key => $value)
    {
    	if(isset($value->name))
    	{
    		$newGrpListarr[] = $value->id;
    	}
    }
    if(count($newGrpListarr) > 0){
    	foreach($newGrpListarr as $grpId){
    		updatesGrpList($drupalUserId, $grpId,$userId,$createuserid);
    		//Added by Vincent on 24 Jan, 2017 for #0071187
    		populateGroupUsersInFlatTable($grpId,$userId,$delUserGrp); // For Admin groups
    		$delUserGrp = '';
    	}
    	// To reset stored active session details (implemented for performance)
    	// resetStoredSessionsForActiveUsers();
    }
    //Added by Vincent on 24 Jan, 2017 for #0071187
    populateGroupUsersInFlatTable(null,$userId,$delUserGrp); // For learner groups
    expDebug::dPrint('$drupalUserId'.$drupalUserId.'$userId'.$userId);
    expDebug::dPrint('grps'.print_r($newGrpListarr, 1), 4);
    
    $form_state['values']['hire_date'] = str_replace("-", "/", $form_state['values']['hire_date']);
    $form_state['values']['hire_date'] = date_format(date_create($form_state['values']['hire_date']), 'Y-m-d H:i:s');
    expDebug::dPrint('form results' . print_r($formvalues, true) , 5);
    
    //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Save Custom Attributes
	    $ret_tmp = getcustomattributemodulestatus();
	    if($ret_tmp == true){
        include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
        saveCustomAttributesInAddEditForm($form, $form_state, $userId, 'cre_usr', $createuserid,'Inserted',1);
    } //#custom_attribute_0078975 - End Check module status
    
    make_audit_trail_entries($form_state['values'],$createuserid,null,$userId,'Inserted','cre_sys_obt_usr','exp_sp_administration_user','addNewUserByRestAPI', array('first_name', 'last_name', 'user_name','city','zip','email','hire_date'));
    //For Insert Role Start
    /* if(!empty($form_state['values']['securityrole'])){
	    $chkRolesExist = is_securityrole($form_state['values']['securityrole']);
	    expDebug::dPrint('addNewUserByRestAPI Velu = '.print_r($chkRolesExist,true),5);
	    if($chkRolesExist == false) {
	          return array((object)array('Id'=>'Invalid Security Role Id.'));
	    }else{
	          $drupalUid = getDrupalIdOfUser($userId);
	          $securityrole = explode(',',$form_state['values']['securityrole']);
	          $loggedInUserId = 1;//getIdOfLoggedInUser();
	          expDebug::dPrint(' Security API Add $drupalUid =  ' . $drupalUid , 5);
	          expDebug::dPrint(' Security API Add $securityrole = ' . print_r($securityrole, true) , 5);
	          expDebug::dPrint(' Security API Add $loggedInUserId = ' . $loggedInUserId , 5);
	          if(count($securityrole) > 0){
	          	foreach($securityrole as $roleId){
	          		updatesSecurityRole($drupalUid,$roleId,$userId,$loggedInUserId);
	          	}
	          }
	    }
    } */
    //For Insert Role End

    return array((object)array('Id'=>$userId, 'is_verified_add'=>$is_verified_add, 'is_valid_add'=>$is_valid_add, 'invalid_add_reason'=>$invalid_add_reason));
  }
  else{
  	$errobj=new stdClass();
  	$errobj->isValidateError = 1;
  	$errobj->errcode = 'L_012';
  	$errobj->errormsg = "User not created";
  	return $errobj;
   // return array((object)array('Id'=>'Failure'));
  }
  } catch (Exception $ex) {
  	watchdog_exception('addNewUserByRestAPI', $ex);
  	expertusErrorThrow($ex);
  }
}


function updateUserByRestAPI(&$form_state, $updateUserId="") {
	try {
        $default_profile_currency = getCurrencyDefDetails();
        
		if(isset($_POST['roles']) && $form_state['values']['roles'] == ''){
			$form_state['values']['roles'] = 'N';
		}
		$form_state['values'] = trimInputFields($form_state['values']); // #42124 - Trim input values
		//36820: Irrelevant escape character is getting inserted on processing through API
		$form_state['values'] = stripslashes_deep($form_state['values']); // Strip the slashes
		$userId = $form_state['values']['id'];
		$form_state['values']['drupal_uid'] = getDrupalIdOfUser($userId);
		expDebug::dPrint(' $form_state[values][drupal_uid] = ' . print_r($form_state['values']['drupal_uid'], true) , 5);
		if($form_state['values']['drupal_uid']==0)
		{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Invalid id.";
			return $errobj;
			}
		expDebug::dPrint(' $form_state[values] = ' . print_r($form_state['values'], true) , 5);
		if (existsUserEmail($form_state['values']['email'], getDrupalIdOfUser($form_state['values']['id']))) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Email id already exists.";
			return $errobj;
			//return array((object)array('Id'=>'Email id already exists.'));
		}
	/*	if (!empty($form_state['values']['first_name']) && (strlen($form_state['values']['first_name'])) > 50) {
			$name_length = strlen($form_state['values']['first_name']);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "First Name cannot be longer than 50 characters but is currently ".$name_length." characters long.";
			return $errobj;
		}
		if (!empty($form_state['values']['last_name']) && (strlen($form_state['values']['last_name'])) > 50) {
			$name_length = strlen($form_state['values']['last_name']);
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Last Name cannot be longer than 50 characters but is currently ".$name_length." characters long.";
			return $errobj;
		} */
		if (!validateEmail($form_state['values']['email'])) {
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = "Email id is not valid.";
			return $errobj;
			//return array((object)array('Id'=>'Email id is not valid.'));
		}
		if(!empty($form_state['values']['status'])){
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['status'],'cre_usr_sts','status');
			if($roleErrmsg !== TRUE){
				return $roleErrmsg;
			}
		}
		if (!empty($form_state['values']['user_name'])) {
			if (strpos($form_state['values']['user_name'], ' ') !== FALSE) { // There should be no space in username (there should be other checks as well TBC)
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Enter the username without space.";
				return $errobj;
				//return array((object)array('Id'=>'Enter the username without space.'));
			} else if (existsUserName($form_state['values']['user_name'], $form_state['values']['drupal_uid'])) { // Confirm user name has not already been used
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "UserName already exists.";
				return $errobj;
				//return array((object)array('Id'=>'UserName already exists.'));
			}
		}
		if(!empty($form_state['values']['manager_id'])){
			$numCheck = is_numeric($form_state['values']['manager_id']);
			if($numCheck)
				$checkIdValid = validatePersonID($form_state['values']['manager_id']);
				if(empty($checkIdValid) || $checkIdValid == '' || empty($numCheck) || ($form_state['values']['id']== $form_state['values']['manager_id'])){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid Manager Id.";
				return $errobj;
				//return array((object)array('Id'=>'Invalid Manager Id.'));
			}
		}
		if(!empty($form_state['values']['other_managers'])){
			$omanagers      = explode(',',$form_state['values']['other_managers']);
			$countmangers   = count($omanagers);
			for($i=0;$i<$countmangers;$i++){
				$ismannum = is_numeric($omanagers[$i]);
				if($ismannum)
					$mang     = is_manager($omanagers[$i]);
				$is_mang  = $form_state['values']['manager_id'];
				if($mang == false || empty($ismannum) || (!empty($is_mang) && $is_mang == $omanagers[$i])) {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Other Manager Id.";
					return $errobj;
					//return array((object)array('Id'=>'Invalid Other Manager Id.'));
				}
			}
			$form_state['values']['other_managers'] = implode(',',array_unique($omanagers));
		}
		if(!empty($form_state['values']['org_id'])){
			$orgcheck = is_organization($form_state['values']['org_id']);
			if($orgcheck == false){
				$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->errormsg = "Invalid Organization Id.";
				return $errobj;
				//return array((object)array('Id'=>'Invalid Organization Id.'));
			}
		}
		if(!empty($form_state['values']['other_organization'])){
			$other_org      = explode(',',$form_state['values']['other_organization']);
			$countorg   = count($other_org);
			for($i=0;$i<$countorg;$i++){
				$org     = is_organization($other_org[$i]);
				$is_org  = $form_state['values']['org_id'];
				if($org == false || (!empty($form_state['values']['org_id']) && $is_org == $other_org[$i])) {
					$errobj=new stdClass();
					$errobj->isValidateError = 1;
					$errobj->errcode = 'L_012';
					$errobj->errormsg = "Invalid Other Organization Id.";
					return $errobj;
					//return array((object)array('Id'=>'Invalid Other Organization Id.'));
				}
			}
			$form_state['values']['other_organization'] = implode(',',array_unique($other_org));
		}
		if(!empty($form_state['values']['jobrole'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['jobrole'],'cre_usr_jrl_','job role');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['usertype'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['usertype'],'cre_usr_ptp_','user type');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['jobtitle'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['jobtitle'],'cre_usr_jtl_','job title');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['empltype'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['empltype'],'cre_usr_etp_','employment type');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['preferred_timezone'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['preferred_timezone'],'cre_sys_tmz_','preferred timezone');
			if($roleErrmsg !== TRUE){
				return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['preferred_language'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['preferred_language'],'cre_sys_lng_','preferred language');
			if($roleErrmsg !== TRUE){
				return $roleErrmsg;
			}
		}
		$preferred_currency = $form_state['values']['preferred_currency'];
		if(!empty($preferred_currency)) {
			$selectQuery = db_select('slt_profile_list_items', 'spli');
			$selectQuery->addField('spli','attr1');
			$selectQuery->condition('spli.is_active', 'Y', '=');
			$selectQuery->condition('spli.attr3', 'Y', '=');
			$selectQuery->condition('spli.code', $preferred_currency, '=');
			$preferred_currency = $selectQuery->execute()->fetchField();
			if(empty($preferred_currency))
			{
				$errobj->isValidateError = 1;
				$errobj->errcode = 'L_012';
				$errobj->field = 'currency_type';
				$errobj->errormsg = "The given preferred currency is invalid or inactive status";
				return $errobj;
			}
			else
				$form_state['values']['preferred_currency'] = $preferred_currency;
		}else{
                        $updUserId =$form_state['values']['id'];
                        $updUserInfo = getUserInformations($updUserId);
                        $updUserCurrency = $updUserInfo['preferred_currency'];
                        if(!empty($updUserCurrency)){
                            $form_state['values']['preferred_currency'] = $updUserCurrency;
                        }else{
                            $form_state['values']['preferred_currency'] = $default_profile_currency;
                        }
		}
		if(!empty($form_state['values']['deptcode'])) {
			$roleErrmsg = checkValidUserAtrribute($form_state['values']['deptcode'],'cre_usr_dpt_','department');
			if($roleErrmsg !== TRUE){
			  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['employee_no'])){
			$roleErrmsg = existsUserEmpid($form_state['values']['employee_no'],$form_state['values']['id'],'api');
			if($roleErrmsg !== TRUE){
				  return $roleErrmsg;
				}
		}
		if(!empty($form_state['values']['country'])){
			$roleErrmsg = is_valid_country($form_state['values']['country']);
			if($roleErrmsg !== TRUE){
				  return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['state'])){
			if(empty($form_state['values']['country'])){
				$countryErrmsg=new stdClass();
				$countryErrmsg->isValidateError = 1;
				$countryErrmsg->errormsg = "Country field should not be empty";
				return $countryErrmsg;
			}
			$roleErrmsg = is_valid_state($form_state['values']['country'],$form_state['values']['state']);
			if($roleErrmsg !== TRUE){
				return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['roles'])) {
		//	$role = array_unique(explode(',',$form_state['values']['roles']));
			$role = $form_state['values']['roles'];
			if($role != 'manager' && $role !='instructor'
					&& $role != 'instructor,manager' && $role != 'manager,instructor' && $role != 'N'){
				$roleErrmsg=new stdClass();
				$roleErrmsg->isValidateError = 1;
				$roleErrmsg->errormsg = "The given role is invalid";
				return $roleErrmsg;
			}
		}
		if(!empty($form_state['values']['roles'])) {
			$role = $form_state['values']['roles'];
			if($role =='instructor' || $role == 'instructor,manager' && $role == 'manager,instructor'){
		if(!empty($form_state['values']['webex_name']) && empty($form_state['values']['webex_pass'])){
			$webexnameErrmsg=new stdClass();
			$webexnameErrmsg->isValidateError = 1;
			$webexnameErrmsg->errormsg = "WebEx pass field should not be empty";
			return $webexnameErrmsg;
		}
		if(empty($form_state['values']['webex_name']) && !empty($form_state['values']['webex_pass'])){
			$webexpassErrmsg=new stdClass();
			$webexpassErrmsg->isValidateError = 1;
			$webexpassErrmsg->errormsg = "WebEx name field should not be empty";
			return $webexpassErrmsg;
		}
		}}


		if(!empty($form_state['values']['roles'])){
			$roles = explode(",",$form_state['values']['roles']);
			if(count($roles) >1 ){
				$fieldsList['is_instructor'] = in_array("instructor", $roles) ? 'Y' : 'N';
			}else{
				$fieldsList['is_instructor'] = ($roles[0] == "instructor") ? 'Y' : 'N';
			}
			if(($fieldsList['is_instructor'] == 'N') || ($form_state['values']['status'] == 'cre_usr_sts_itv')){
				$checkSessionInsAPI = checkInstructorAssignedSession($form_state['values']['id']);
				if($checkSessionInsAPI > 0) {
					$roleErrmsg=new stdClass();
					$roleErrmsg->isValidateError = 1;
					$roleErrmsg->errormsg = "Instructor has active associations and cannot be modified now";
					return $roleErrmsg;
				}
			}
		}
		if(!empty($form_state['values']['hire_date'])) {
			if(!validateHireDateFormat($form_state['values']['hire_date'])){
				$hireDateErrmsg=new stdClass();
				$hireDateErrmsg->isValidateError = 1;
				$hireDateErrmsg->errormsg = "Please enter a valid date in this mm/dd/yyyy format.";
				return $hireDateErrmsg;
			}
		}
		
		if(!empty($form_state['values']['hire_date'])) {
			if(!validateDateMonthYear($form_state['values']['hire_date'],'m/d/Y',now(),5)){
				$hireDateErrmsg=new stdClass();
				$hireDateErrmsg->isValidateError = 1;
				$hireDateErrmsg->errormsg = "Hire Date should be within range of 5 years in future from current date.";
				return $hireDateErrmsg;
			}
		}
		//Added by Subin ->> User Address Validation
		$verifyGivenAddress = getAddressVerified($form_state);
		$is_verified_add = $verifyGivenAddress['is_verified_add'];
		$is_valid_add = $verifyGivenAddress['is_valid_add'];
		$invalid_add_reason = $verifyGivenAddress['invalid_add_reason'];
		//generate form_state values for the is_verified_add, is_valid_add, & invalid_add_reason
		$form_state['values']['is_verified_add'] = $is_verified_add;
		$form_state['values']['is_valid_add'] = $is_valid_add;
		$form_state['values']['invalid_add_reason'] = $invalid_add_reason;

		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		$form_state['storage']['acvalues']['manager']['id'] = $form_state['values']['manager_id'];
		$form_state['storage']['acvalues']['org']['id'] = $form_state['values']['org_id'];
		$form_state['values']['omanagers'] = $form_state['values']['other_managers'];
		$form_state['values']['oorgs'] = $form_state['values']['other_organization'];
		if(empty($form_state['values']['preferred_timezone'])){ 
			if(isset($form_state['values']['request_source']) && $form_state['values']['request_source'] == "saml"){
			   // Do nothing 
			}else{
			   	$defaultTimezone = defaultSiteTimeZone();
    	        $form_state['values']['preferred_timezone']  = $defaultTimezone['time_zone'] ;
			}
		}
        //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status/Custom Attrbiutes Field Validation
        $ret_tmp = getcustomattributemodulestatus();
        if($ret_tmp == true){
            include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'exp_sp_administration_customattribute.inc');
            $custom_attr_error = validateCustomAttribute($form_state,'cre_usr');
            expDebug::dPrint('user page custom attribute Validation Result--> '.print_r($custom_attr_error,true), 5);
            if($custom_attr_error != '') {
                $customErrmsg=new stdClass();
                $customErrmsg->isValidateError = 1;
                $customErrmsg->errormsg = $custom_attr_error;
                return $customErrmsg;
		}
        } //#custom_attribute_0078975 - End Check module status
        
		$defUserdetails = getUserAccessPriviledges($form_state['values']['id']);

			$newStatus = $form_state['values']['status'];
			$currstatus = getPersonDetails($userId,array('status'));
			if((!empty($newStatus)) && ($newStatus != $currstatus['status'])){
				activateOrDeactivatePeople($userId,$form_state['values']['status'],false);
			}
		$userId =  updateUserBasicDetails($form, $form_state, $updateUserId, 1);
		expDebug::dPrint('$defUserdetails12333'.$userId, 4);
		if ($userId && $userId!='No change required') {
			updateUserAddressDetails($form, $form_state, $updateUserId ,'Updated');
			expDebug::dPrint('form state update roles'.print_r($form_state['values'], 1));
			updateUserOrgDetails($form, $form_state, $updateUserId, $form_state['values']['id'], 'Updated', 1);
			expDebug::dPrint(' updateUserByRestAPI updated user : ' . $userId , 4);
			expDebug::dPrint("SEcurity Role :: !!!".$form_state['values']['securityrole']."!!!");
			$attributes = getEnabledAttrForVisibility();

			//change by ayyappans for 42239: Users roles table not updated when creation user from Create User rest api
			$fieldformat = array(
					'user_type' => 'usertype',
					'org_id' => 'org_id',
					'job_role' => 'jobrole',
					'employment_type' => 'empltype',
					'department' => 'deptcode',
					'country' => 'country',
					'state' => 'state',
					'language' => 'preferred_language'
			);
			$attrChangeflag = false;
			expDebug::dPrint('$updateUserId '.$form_state['values']['id'], 4);
			expDebug::dPrint('$drupalUserId '.$form_state['values']['drupal_uid'], 4);
			expDebug::dPrint('$attributes'.print_r($attributes, 1), 4);
			expDebug::dPrint('$attrChangeflag'.$attrChangeflag, 4);
			expDebug::dPrint('$form_state'.print_r($form_state['values'], 1), 4);
			expDebug::dPrint('$defUserdetails'.print_r($defUserdetails, 1), 4);

			foreach($attributes as $key => $attr){
				expDebug::dPrint('arg1'.$defUserdetails->$attr, 5);
				expDebug::dPrint('arg2'.$form_state['values'][$fieldformat[$attr]], 5);
				if($attr != 'role'){
					if($defUserdetails->$attr != $form_state['values'][$fieldformat[$attr]]){
						$attrChangeflag = true;//set the flag as true when users attributes updated
						break;
					}
				}
			}
			$exroleList = getExistingGrpList($form_state['values']['id']);
			$newGrpList = getGroupsDetails($form_state['values']['id'],'',1,'',0);
			expDebug::dPrint('$exroleList'.print_r($exroleList, 1), 5);
			expDebug::dPrint('$$newGrpList'.print_r($newGrpList, 1), 5);

			if($attrChangeflag){
				expDebug::dPrint('TEST INSIDE---->> ');
				/*$exroleList = getExistingGrpList($form_state['values']['id']);
				$newGrpList = getGroupsDetails($form_state['values']['id'],'',1,'',0);
				expDebug::dPrint('$exroleList'.print_r($exroleList, 1), 5);
				expDebug::dPrint('$$newGrpList'.print_r($newGrpList, 1), 5);*/
				$ExistingRoleList = array();
				$newGrpListarr = array();
				foreach($exroleList as $key => $value)
				{
					if(isset($value->name))
					{
						$ExistingRoleList[] = $value->id;
					}
				}
				foreach($newGrpList as $key => $value)
				{
					if(isset($value->name))
					{
						$newGrpListarr[] = $value->id;
					}
				}
				expDebug::dPrint('$$newGrpListarr'.print_r($newGrpListarr, 1), 5);
				expDebug::dPrint('$$$ExistingRoleList'.print_r($ExistingRoleList, 1), 5);
				$delUserGrp = 'Delete'; // While populat flat table the existing entry should be delete once/very first call
				foreach($newGrpListarr as $grpId){
					if(($keyval = array_search($grpId, $ExistingRoleList)) !== false) {
						unset($ExistingRoleList[$keyval]);
					}else{
						updatesGrpList($form_state['values']['drupal_uid'], $grpId, $form_state['values']['id'], $updateUserId);
					}
					//Added by Vincent on 24 Jan, 2017 for #0071187
					populateGroupUsersInFlatTable($grpId,$form_state['values']['id'],$delUserGrp); // For Admin groups
					$delUserGrp = '';
				}
				if(!empty($ExistingRoleList)){
					foreach($ExistingRoleList as $grpId){
						deleteGrpList($form_state['values']['drupal_uid'], $grpId, $form_state['values']['id']);
					}
				}
				//Added by Vincent on 24 Jan, 2017 for #0071187
				populateGroupUsersInFlatTable(null,$form_state['values']['id'],$delUserGrp); // For learner groups
				// To reset stored active session details (implemented for performance)
  			// resetStoredSessionsForActiveUsers();
			}

            //if(module_exists('exp_sp_administration_customattribute')){   //#custom_attribute_0078975 - Check module status
            $ret_tmp = getcustomattributemodulestatus();
            if($ret_tmp == true){ 
                include_once(drupal_get_path('module', 'exp_sp_administration_customattribute') .'/exp_sp_administration_customattribute_renderinforms.inc');
                saveCustomAttributesInAddEditForm($form, $form_state, $userId, 'cre_usr', $updateUserId, 'Updated',1);
            } //#custom_attribute_0078975 - End Check module status
            
			//For Update Role Start
			/* if(!empty($form_state['values']['securityrole'])){

			$chkRolesExist = is_securityrole($form_state['values']['securityrole']);
			if($chkRolesExist == false) {
			return array((object)array('Id'=>'Invalid Security Role Id.'));
			}else{
			$exroleList = getSecurityDefDetails($userId,array('id'));
			$ExistingRoleList = array();
			foreach($exroleList as $key => $value){
			if(isset($value->name)){
			$ExistingRoleList[] = $value->id;
			}
			}
			$drupalUid = getDrupalIdOfUser($userId);
			$securityrole = explode(',',$form_state['values']['securityrole']);
			$loggedInUserId = 1;//getIdOfLoggedInUser();
			expDebug::dPrint(' Security API Edit $drupalUid = ' . $drupalUid , 5);
			expDebug::dPrint(' Security API Edit $securityrole = ' . print_r($securityrole, true) , 5);
			expDebug::dPrint(' Security API Edit $loggedInUserId = ' . $loggedInUserId , 5);
			expDebug::dPrint(' Security API Edit $drupalUid = ' . print_r($ExistingRoleList, true) , 5);
			foreach($securityrole as $roleId){
			if(($keyval = array_search($roleId, $ExistingRoleList)) !== false){
			unset($ExistingRoleList[$keyval]);
			}else{
			updatesSecurityRole($drupalUid,$roleId,$userId,$loggedInUserId);
			}
			}
			if(!empty($ExistingRoleList)){
			foreach($ExistingRoleList as $role_id){
			deleteSecurityRole($drupalUid,$role_id,$userId);
			}
			}
			}
			} */
			//For Update Role End
			return array((object)array('Id'=>$userId, 'is_verified_add'=>$is_verified_add, 'is_valid_add'=>$is_valid_add, 'invalid_add_reason'=>$invalid_add_reason));
		}
		else if($userId == 'No change required' && $_SESSION['saml_auth'] == 1){
			expDebug::dPrint('No change from  saml authentication');
			return 0;
		}
		else if($userId == 'No change required' && isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			expDebug::dPrint('No change from  UpdateUser API');
			return array((object)array('Id'=>$form_state['values']['id'], 'is_verified_add'=>$is_verified_add, 'is_valid_add'=>$is_valid_add, 'invalid_add_reason'=>$invalid_add_reason));
		}
		else{
			$errobj=new stdClass();
			$errobj->isValidateError = 1;
			$errobj->errcode = 'L_012';
			$errobj->errormsg = 'User details not updated';
			return $errobj;
			//return array((object)array('Id'=>'Failure'));
		}
	} catch (Exception $ex) {
		watchdog_exception('updateUserByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function validatePersonID($managerID) {
	try {
	$select = db_select('slt_person', 'per');
	$select->addField('per', 'id');
	$select->addField('per', 'full_name');
	$select->condition('per.id', $managerID, '=');
	$select->condition('per.status', 'cre_usr_sts_atv','=');
	$select->condition('per.user_name', array('admin', 'guest'), 'NOT IN');
	$result = $select->execute()->fetch();
	return $result->id;
	} catch (Exception $ex) {
  	watchdog_exception('validatePersonID', $ex);
  	expertusErrorThrow($ex);
  }
}
function listUserByRestAPI( $start, $limit, $userid="")
{
	try {
	  $form=array();
	  $form["form_id"]="";
	  $limit = (isset($limit) && $limit > 500) ? 500 : $limit;
	  
		// SOLR check starts
	  $solrSource = getConfigValue('solr_search');
		if($solrSource == 0){
			drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		  return fetchUsersAll('',$start,$limit, true);
		}else{
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			try{
				$solrObj = new UserSolrSearch();
				$solrList = $solrObj->getSolrSearchResult();
				return $solrList;
			}catch(Exception $e){
				expDebug::dPrint("Error in Solr Search for Users - ".print_r($e->getMessage(),true),1);
			}
		}
  } catch (Exception $ex) {
  	watchdog_exception('listUserByRestAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * @author ayyappans for 40036: New RestAPI - getUserDetails
 */
function getUserDetailsByRestAPI($name, $type) {
	try {
		$name = trim($name);
		$typeLower = strtolower($type);
		switch ($typeLower) {
			case 'user':
				//validation for user name
				$count = checkUserNameExists($name, array('name' => array('admin', 'guest')));
				if($count == 0) {
					$error -> isValidateError = true;
					$error -> errormsg = 'No user found';
					return $error;
				}
				$response = fetchUsersAll('', 0, 1, true, $name);
				$object_type = 'User';
				break;

			case 'organization':
				include_once(drupal_get_path('module', 'exp_sp_administration_organization') .'/exp_sp_administration_organization.inc');
				//change by ayyappans for 41066: Need an API to get Org details when Org Name is passed
				$orgList = fetchOrganizationsAll('', null, null, $name);
				$response = array();

				foreach ($orgList as $organization) {
					$org = new stdClass();
					$org->org_id			= $organization->id;
					$org->org_name			= $organization->name;
					$org->org_description	= $organization->description;
					$org->org_type 			= $organization->type;
					$org->org_status_code 	= $organization->status_code;
					$org->org_status 		= $organization->status;
					$response[] 			= $org;
				}
				$object_type = 'Organization';
				break;

			case 'job_role': case 'job_title': case 'department':
				$codePrefix = array('job_role' => 'cre_usr_jrl_', 'job_title' => 'cre_usr_jtl_', 'department' => 'cre_usr_dpt_');
				$codeName	= array('job_role' => 'Job Role', 'job_title' => 'Job Title', 'department' => 'Department');
				$fields		= array('id' => 'object_id', 'name' => 'object_name', 'code' => 'object_code');
				$response = getMatchingProfileItemNamesKeyed($name, $codePrefix[$typeLower], 'cre_sys_lng_eng', true, '', true, $fields);
				$object_type = $codeName[$typeLower];
				break;

			default:
				$error -> isValidateError = true;
				$error -> errormsg = "type can be user or organization or job_role or job_title or department";
				return $error;
				break;
		}
		if(count($response) == 0) {
			$error= new stdClass();
			$error -> isValidateError = true;
			$error -> errormsg = "No $object_type found";
			return $error;
		}
		$response[0]->object_type = $object_type;
		return $response;

	} catch (Exception $ex) {
		watchdog_exception('getUserDetailsByRestAPI', $ex);
		expertusErrorThrow($ex);
	}
}
function checkUserNameExists($username, $not_in = array()) {
	try{
		$select = db_select('users', 'users');
		$select->addExpression('COUNT(*)', 'count');
		$select->condition('users.name', $username, 'LIKE');
		if (!empty($not_in)) {
			foreach ($not_in as $column => $set) {
				$select->condition('users.'.$column, $set, 'NOT IN');
			}
		}
		$result =  $select->execute()->fetchField();
		return (($result > 0)? true : false);
	}catch (Exception $ex) {
		watchdog_exception('checkUserNameExists', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * selectAddNarrowSearchByInstructorConditionUser() - To the $selectStmt add Search by Instructor username when set by the user
 */
function selectAddNarrowSearchByInstructorConditionUser($selectStmt, $clsTableAlias, $filtersetCode, $defaultText = '') {
	try {
  $textFilterStr = getURLParam($filtersetCode);
  expDebug::dPrint(' $textFilterStr = ' . print_r($textFilterStr, true) , 4);

  if (!empty($textFilterStr) && $textFilterStr != $defaultText){
    $selectStmt->condition('per.is_instructor', 'Y');
    $selectStmt->condition('per.full_name', '%' . db_like($textFilterStr) . '%', 'LIKE');
  }
  } catch (Exception $ex) {
  	watchdog_exception('selectAddNarrowSearchByInstructorConditionUser', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * Get the Location name in auto-complet
 */
function getFacilityNameAutoComplete(){
try {
  $searchText =  $_GET['z'];
  $excludedCourseId = $_GET['excluded_course_id'];
  $idRequired = isset($_GET['id_required']) ? $_GET['id_required'] : 1;

  // Select the table
  $select = db_select('slt_location', 'location');
  // Add fields to fetchgetTpCourseAutoComplete
  $select->addField('location', 'id', 'id');
  $select->addField('location', 'name', 'name');

  // Add condition
  $select->condition('location.name', '%' . db_like($searchText) . '%', 'LIKE');
  $select->condition('location.status', 'lrn_res_loc_atv','=');

  if (!empty($excludedCourseId)) {
    $excludedCourseId = urldecode($excludedCourseId);
    $excludedCourseIdArr = explode(',', $excludedCourseId);
    $select->condition('location.id', $excludedCourseIdArr, 'NOT IN');
  }

  // Order in dictionary sort order
  $select->orderBy('name');
  $matchingLocationNames = $select->execute()->fetchAll();
  expDebug::dPrint(' $matchingLocationNames = '. print_r($matchingLocationNames, true) , 3);

  foreach ($matchingLocationNames as $locationName) {
    if($idRequired == 0){
      print $locationName->name . "\n";
    } else {
      print $locationName->id."|".$locationName->name ."\n";

    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('getFacilityNameAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}


function ACFieldSetToAddEditFormUser(&$form, $fieldsetName, $id, $acMenuPath, $title = '', $defaultValue = '', $tabIndex = null, $dependsUponFields = array(), $addlDependencies = array(), $mandatory = false, $readOnly = false, $preTextValue = '') {
try {
  $attributes = array();
  $autocompleteWithId=false;
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.

  // Add a classes to apply common style to all autocomplete fields
  $attributes['class'][] = 'addedit-edit-ac-textfield';
  $attributes['class'][] = 'ac_input';


  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  if ($readOnly) {
    $attributes['readonly'] = 'readonly';
    $attributes['class'][] = 'addedit-readonly-textfield'; // Add another class
  } else {
    $extraParams = '';
    foreach ($dependsUponFields as $dependancy) {
      $extraParams .= (($extraParams == '')? '' : ', ') .
      $dependancy .
                              ': function() { return jQuery(".addedit-edit-' .
      $dependancy . '").val(); }';
    }

    foreach ($addlDependencies as $paramName => $paramValue) {
      if($paramName=='id'){ // Added by vincent on 26 Mar, 2012 to handle result id in autocomplete
      		$autocompleteWithId=true;
      }
      $extraParams .= (($extraParams == '')? '' : ', ') .
      $paramName . ': ' . $paramValue;
    }

    expDebug::dPrint(' $extraParams = '. print_r($extraParams, true) , 4);

    if($preTextValue == 'Type a Course title' ) {

      $readingTypeClass = 'input-field-grey';
      $defaultValue = $preTextValue;
      $attributes['class'][] = $readingTypeClass; // Add another class
      $attributes['onblur'][] = 'textfieldTitleBlur(this, "'.$preTextValue.'");';
      $attributes['onfocus'][] = 'textfieldTitleClick(this, "'.$preTextValue.'");';

    }
    if($autocompleteWithId==true){ // Added by vincent on 26 Mar, 2012 to show text in UI when use autocomplete with id
      $attributes['onfocus'][] = '(function ($) {' .
      		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                  'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                      '{' .
                                                        'minChars: 3, ' .
                                                        'max: 50, ' .
                                                        'autoFill: true, ' .
                                                        'mustMatch: false, ' .
                                                        'matchContains: false, ' .
      																									'formatItem:function(pmRec){return pmRec[1]},'.
                                                        'inputClass: "ac_input", ' .
                                                        'loadingClass: "ac_loading", ' .
                                                        'extraParams: {' . $extraParams . '}' .
                                                       '}' .
                                                    ');' .
                                            '}' .
                                 ')(jQuery);';
    }else{
      $attributes['onfocus'][] = '(function ($) {' .
       		                        'jQuery(".addedit-edit-' . $id . '").unautocomplete();' .
                                  'jQuery(".addedit-edit-' . $id . '").autocomplete("/?q=' . $acMenuPath . '", ' .
                                                      '{' .
                                                        'minChars: 3, ' .
                                                        'max: 50, ' .
                                                        'autoFill: true, ' .
                                                        'mustMatch: false, ' .
                                                        'matchContains: false, ' .
                                                        'inputClass: "ac_input", ' .
                                                        'loadingClass: "ac_loading", ' .
                                                        'extraParams: {' . $extraParams . '}' .
                                                       '}' .
                                                    ');' .
                                            '}' .
                                 ')(jQuery);';
    }
  }

  $form[$fieldsetName.'_fieldset'][$fieldsetName][$fieldsetName.'_name_'.$id] = array(
        '#type' => 'textfield',
        '#prefix' => addEditGetPrefix('FIELD', $title),
        '#suffix' => addEditGetSuffix('FIELD'),
        '#title' => $title.(($mandatory)? '<span class="addedit-mandatory">*</span>':''),
        '#title_display' => 'invisible',
        '#default_value' => $defaultValue,
        '#attributes' => $attributes,
  		'#size' => 59,
  );
  } catch (Exception $ex) {
  	watchdog_exception('ACFieldSetToAddEditFormUser', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * function to activate and deactivate the user
 */
function activateOrDeactivatePeople($personId,$status='',$printResult=true,$callfrom = ''){
  $updatedUserId = ($callfrom == 'Feed') ? 'Feed' : getSltpersonUserId();
  $drupalUserId = getDrupalIdOfUser($personId);
  $txn = db_transaction();
  try {
  	$fields = array();
	$is_instructor = db_query("select is_instructor from slt_person where id=:id",array(":id"=>$personId))->fetchField();
	$checkSessionIns = ($is_instructor=='Y') ? checkInstructorAssignedSession($personId) : '0';
	expDebug::dPrint('inside active inst : '.$checkSessionIns);
	if($checkSessionIns > 0){
			print ('activeinstr');
			die;
	}else{
    if($status==''){
      $status  = getPeopleStatus($personId);
      $updateStmt = db_update('slt_person');
      $personStatus  = ($status == 'cre_usr_sts_atv') ? "cre_usr_sts_itv" : "cre_usr_sts_atv";
    }else{
      $updateStmt = db_update('slt_person');
      //$personStatus  = ($status == 'Activate' || $status == 'cre_usr_sts_atv') ? "cre_usr_sts_atv" : "cre_usr_sts_itv";

     if($status == 'Activate' || $status == 'cre_usr_sts_atv'){
        $personStatus = 'cre_usr_sts_atv';
      }else if($status == 'cre_usr_sts_del'){
      	$personDetails = getPersonDetails($personId, array('email', 'user_name'));
        $personStatus = 'cre_usr_sts_del';
        $fields['email']     = $personDetails['email'] . '-deleted-' . $personId;
        $fields['user_name'] = $personDetails['user_name'] . '-deleted-' . $personId;
      }else{
      	$personStatus = 'cre_usr_sts_itv';
      }
    }
    $fields['status']      = $personStatus;
    $fields['updated_on']    = now();
    $fields['updated_by']    = $updatedUserId;
    $updateStmt->condition('id', $personId);
    $updateStmt->fields($fields);
    expDebug::dPrintDBAPI('update string ',$updateStmt);
    $updateStmt->execute();
    $updatedUserId = ($updatedUserId == 'Feed') ? 1 : $updatedUserId;
    add_audit_trail_entry($updatedUserId, $personId, 'cre_sys_obt_usr', 'exp_sp_administration_user', 'activateOrDeactivatePeople', 'Updated User Status', null, $status, $personStatus);
    $userStatus  = ($personStatus=='cre_usr_sts_itv' || $personStatus=='cre_usr_sts_del' ) ? "0" : "1";
    $fields = array();
    if($personStatus=='cre_usr_sts_del'){
    	$fields['mail']   = $personDetails['email'] . '-deleted-' . $personId;
    	$fields['name']   = $personDetails['user_name'] . '-deleted-' . $personId;
    }
    $fields['status'] = $userStatus;
    $updateStmt = db_update('users');
    $updateStmt->condition('uid', $drupalUserId);
    $updateStmt->fields($fields);
    $updateStmt->execute();

    //Added by Vincent on 3 Feb, 2017 for #0071969.
	populateGroupUsersInFlatTable(null,$personId,'UserStatusChange');
	
	if($personStatus=='cre_usr_sts_itv'){
		$loggeduserId = getSltpersonUserId();
		// Get the fields before deleting
		$select = db_select('slt_user_access_mapping', 'uamap');
		$select->addField('uamap','id','id');
		$select->addField('uamap','entity_id','entity_id');
		$select->addField('uamap','entity_type','entity_type');
		$select->addField('uamap','user_id','user_id');
		$select->condition('user_id',$personId);
		$uamapId =$select->execute()->fetchAll();
		 
		$select = db_delete('slt_user_access_mapping');
		$select->condition('user_id',$personId);
		expDebug::dPrintDBAPI('delete user from slt_user_access_mapping' , $select);
		$select->execute();
	
		foreach ($uamapId as $val){
			// Make deleted log entry
			$fields = array(
					'table_name' => 'slt_user_access_mapping',
					'entity_id' => $val->id,
					'entity_type' => 'user_access',
					'parent1_entity_id' => $val->entity_id,
					'parent1_entity_type' => $val->entity_type,
					'parent2_entity_id' => $val->user_id,
					'parent2_entity_type' => 'user_id',
					'module_name' => 'exp_sp_administration_user',
					'functionality_name' => 'activateOrDeactivatePeople',
					'api_name' => $_REQUEST['apiname'],
					'deleted_on' => now(),
					'deleted_by' => $loggeduserId
			);
			deleted_log_entry($fields);
		}
		 
	}
		
    if($personStatus == 'cre_usr_sts_atv'){
      insertNotification('ActivateUser', '', '', $personId, $updatedUserId);
    }else{
      insertNotification('SuspendUser', '', '', $personId, $updatedUserId);
    }

    if($printResult){
      print   ($personStatus == 'cre_usr_sts_atv') ? t('LBL572') : t('LBL573');
    }
    }
  }catch (Exception $ex) {
    $txn->rollback();
    watchdog_exception('activateOrDeactivatePeople', $ex);
    throw $ex;
  }
  unset($txn);
}
/*
 * fucntion to retrive the current status of user
 */
function getPeopleStatus($userId){
	try {
  $select = db_select('slt_person', 'per');
  $select->addField('per','status', 'status');
  $select->condition('per.id',$userId);
  // Execute the query
  $perStatus = $select->execute()->fetchField();
  return $perStatus;
  } catch (Exception $ex) {
  	watchdog_exception('getPeopleStatus', $ex);
  	expertusErrorThrow($ex);
  }
}
function getUserFullname($userId){
	try {
  $select = db_select('slt_person', 'per');
  $select->addExpression('CONCAT(per.last_name, \', \', per.first_name)','user_firstlast_name');
  //$select->addField('per','first_name','user_firstname');
  $select->condition('per.id',$userId);
  // Execute the query
  $perName = $select->execute()->fetchField();
  return $perName;
  } catch (Exception $ex) {
  	watchdog_exception('getUserFullname', $ex);
  	expertusErrorThrow($ex);
  }
}

function delete_profile_images ($form, &$form_state) {
try {
  $userId = $form_state['triggering_element']['#ajax']['user_id'];
  $drupalUserId = getDrupalIdOfUser($userId);
  if(!empty($drupalUserId)){
    $account = user_load($drupalUserId, TRUE);
    file_usage_delete($account->picture, 'user', 'user', $drupalUserId);
    // Remove file reference from users table record
    $account = user_save($account, array('picture' => 0));
    expDebug::dPrint('User id id value' .$userId.'drupal'.$drupalUserId , 4);
    $form_state['storage']['edited_entity'] = TRUE; // To trigger refresh of jqgrid on modal close.
    $form_state['display_class'] = TRUE;
    $form_state['rebuild'] = TRUE;
    // If any module has implemented hook_user_picture_updated invoke it
    foreach (module_implements('user_picture_updated') as $module) {
      $function = $module . '_user_picture_updated';
      expDebug::dPrint('$function = ' . $function, 4);
      $function($drupalUserId);
    }
  }
  } catch (Exception $ex) {
  	watchdog_exception('delete_profile_images', $ex);
  	expertusErrorThrow($ex);
  }
}

function getLastLoginTime($username){
	try {
  $select = db_select('users', 'usr');
  /* changes for ticket 16539 - people admin - search - last login - displayed as 31 dec, 1969 */
  $select->addField('usr','uid', 'uid');
  $select->addField('usr','timezone', 'timezone');
  $select->addExpression('usr.login', 'usr_login');
  $select->addExpression('FROM_UNIXTIME(usr.login, \'%Y-%m-%d\')', 'last_login_date');
  $select->addExpression('FROM_UNIXTIME(usr.login, \'%p\')', 'last_login_period');
  $select->addExpression('FROM_UNIXTIME(usr.login, \'%Y-%m-%d %H:%i\')', 'last_login'); //'Y-m-d H:i
  $select->condition('usr.name',$username);
  // Execute the query
  $getLoginTime = $select->execute()->fetchAll();//$select->execute()->fetchField();
  expDebug::dPrint(' getlogin time = ' . print_r($getLoginTime, true) , 3);
  return $getLoginTime;
  } catch (Exception $ex) {
  	watchdog_exception('getLastLoginTime', $ex);
  	expertusErrorThrow($ex);
  }
}





/*
 * getAdminUserJobTitleAutoComplete() - Returns the matched User Job Titles
 */
function getAdminUserJobTitleAutoComplete() {
	try {
  $itemsList = getMatchingProfileItemNamesKeyed($_GET['z'], 'cre_usr_jtl_');
  foreach ($itemsList as $code => $name) {
    print $name . "\n";
  }
  } catch (Exception $ex) {
  	watchdog_exception('getAdminUserJobTitleAutoComplete', $ex);
  	expertusErrorThrow($ex);
  }
}




function existsUserEmpid($empid,$userid=null,$api='') {
  try{
    $select = db_select('slt_person', 'pers');
    $select->addExpression('COUNT(*)', 'count');
    $select->condition('pers.employee_no', $empid, '=');
    $select->condition('pers.status','cre_usr_sts_del','!=');
    if (!empty($userid) && $userid > 0) {
      $select->condition('pers.id', $userid, '!=');
    }
    $result = $select->execute()->fetchField();
        if($api == 'api'){
	    if(($result > 0)){
	    	$errobj=new stdClass();
				$errobj->isValidateError = 1;
				$errobj->errormsg = "Employee id must be Unique";
				return $errobj;
	    }else{
	    	return TRUE;
	    }
		}
		return (($result > 0)? true : false);
  }catch (Exception $ex) {
    watchdog_exception('existsUserEmpid', $ex);
    expertusErrorThrow($ex);
  }
}

function is_organization($orgId){
  try{
    $select = db_select('slt_organization', 'org');
    $select->addExpression('count(org.id)','cnt');
    $select->condition('org.id', $orgId);
   	$select->condition('org.status','cre_org_sts_act','=');
    //$select->condition(db_or()->condition('per.manager_id', $userId)->condition($andcondition));
    //$select->condition(db_or()->condition('per.manager_id', $userId)->condition($andcondition)->where('FIND_IN_SET( '.$userId.', per.dotted_mngr_id )'));
    expDebug::dPrintDBAPI(' is_organization check = ' , $select);
    $result = $select->execute()->fetchField();
    return ($result > 0) ? true : false;
  }catch (Exception $ex) {
    watchdog_exception('is_organization', $ex);
    expertusErrorThrow($ex);
  }
}

function insertNotificationForUser($notificationType,$userId,$notificationName,$managerId) {
	try{
	if($notificationType=='notify_130'){
    			$query = "CALL slp_lnr_notification_ins('".$notificationType."','".$userId."'";
    			$query .= ",'".$managerId."'";
    			$query .= ",'".addslashes($notificationName)."'";
    			$query .= ",null,'".$userId."');";
		    }
		    elseif($notificationType=='notify_129'){
    			$query = "CALL slp_lnr_notification_ins('".$notificationType."','".$userId."'";
    			$query .= ",null";
    			$query .= ",'".addslashes($notificationName)."'";
    			$query .= ",null,'".$userId."');";
		    }
	       elseif($notificationType=='assign_dotted_manager'){
    			$query = "CALL slp_lnr_notification_ins('".$notificationType."','".$userId."'";
    			$query .= ",null";
    			$query .= ",'".addslashes($notificationName)."'";
    			$query .= ",null,'".$userId."');";
		    }
	        elseif($notificationType=='assign_dotted_organization'){
    			$query = "CALL slp_lnr_notification_ins('".$notificationType."','".$userId."'";
    			$query .= ",null";
    			$query .= ",'".addslashes($notificationName)."'";
    			$query .= ",null,'".$userId."');";
		    }
		    else{
    			$query = "CALL slp_lnr_notification_ins('".$notificationType."','".$userId."'";
    			$query .= ($inObj->OrderId!=null && $inObj->OrderId!='')?",".$inObj->OrderId :",null";//altered query to pass integer value for reading id when empty or null or any value is passed too
    			$query .= ",'".$inObj->Email."','".$inObj->Password."','".$userId."');";
		    }
			$dao = new AbstractDAO();
            $dao->connect();
            $dao->beginTrans();
            expDebug::dPrint("insertNotificationForUser ".$query);
            $dao->execute($query);
            $dao->commitTrans();
            $dao->closeconnect();
            return '';
	}catch (Exception $ex) {
    watchdog_exception('insertNotificationForUser', $ex);
    expertusErrorThrow($ex);
  }
}

/* Function not used anywhere and also table slt_security_role not in use now. - #0046888
 * function is_securityrole($role){
  try{
    $roles = explode(",", $role);
    $select = db_select('slt_security_role', 'role');
    $select->addExpression('count(role.id)','cnt');
    $select->condition('role.id', $roles , 'IN');
    expDebug::dPrintDBAPI(' is_securityrole check = ' , $select);
    $result = $select->execute()->fetchField();
    return ($result == count($roles)) ? true : false;
  }catch (Exception $ex) {
    watchdog_exception('is_securityrole', $ex);
    expertusErrorThrow($ex);
  }
}*/
function insertChangeUserNameNotification($notificationCode, $notifyResult, $notifyDetails, $userId){
	try {
	$notifyOverrideFlag = null;
	$overrideContent = '';
	$overrideInfo = getEntityNotificationOverrideInfo($userId, 'cre_sys_obt_usr', $notificationCode, 'Email');
	expDebug::dPrint(' insertChangeUserNameNotification $overrideInfo = ' . print_r($overrideInfo, true) , 4);
	if($overrideInfo != null) {
		$notifyOverrideFlag = $overrideInfo['notify_flag'];
		$overrideContent = $overrideInfo['override_content'];
	}
	$notificationTemplateInfo = getNotificationTemplateInfo($notificationCode, 'cre_sys_lng_eng');
	$notifyFlag = null;

	// Both 'Y' and 'cre_ntn_sts_atv' are active statuses
	if ($notificationTemplateInfo['status'] == 'cre_ntn_sts_atv') {
		$notifyFlag = 'Y';
	}
	if ($notifyOverrideFlag == 'cre_ntn_sts_atv') {  // Added
		$notifyOverrideFlag = 'Y';
	}
	$frameString = true;
	if ($notifyFlag != 'Y' || ($notifyOverrideFlag != null && $notifyOverrideFlag != 'Y')) {
		if($notifyFlag != 'Y'){
			// If the notification is disabled status in template, do not generate notification
			expDebug::dPrint('Notification is disabled');
			$frameString = false;
		}else{
			//If the notification status has been overridden and set to disabled, do not generate notification.
			expDebug::dPrint(' Notification is overridded and disabled');
			$frameString = false;
		}
	}
	if($frameString){
		$notificationInfo = null;
		$notificationInfo['tokens_string'] =         'first_name>|' . $notifyDetails['first_name'] . '~|' .
				'exist_username>|' . $notifyResult[0]->user_name . '~|' .
				'user_name>|' . $notifyDetails['user_name'] ;

		$notificationInfo['message_id']    = $notificationCode;
		$notificationInfo['message_type']  = 'Username Updated by Admin';
		$notificationInfo['send_type']     = 'php mailer';
		$notificationInfo['lang_code']     = '';
		callCommonNotificationInsert($notificationCode,$notificationInfo,$notificationTemplateInfo,$userId,$overrideContent);
	}
	} catch (Exception $ex) {
		watchdog_exception('insertChangeUserNameNotification', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function to fetch compliance enroll id for access.
 * */
function getUpdatedEnrollmentsId($fieldType,$userId,$default,$new) {
	try{
		$select = db_select('slt_group_mapping','cat');
		$select->leftJoin('slt_groups','grp','grp.id = cat.group_id');
		$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->leftJoin('slt_enrollment','enr','((cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') OR (cat.entity_id = enr.course_id and cat.entity_type = \'cre_sys_obt_crs\') OR
  						 (cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') ) and enr.user_id='.$userId.' and enr.reg_status = \'lrn_crs_reg_cnf\'');
		$select->leftJoin('slt_person','per','per.id=enr.user_id');
		$select->addExpression('GROUP_CONCAT(DISTINCT(enr.id))', 'enroll_id');
		if ($fieldType == 'hire_date') {
			$select->where('(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)) AND
							(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\') < DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') OR DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\') > DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=2)  )');
		} else {
		$select->where('find_in_set(\''.$default.'\',grp.'.$fieldType.') >0 and find_in_set(\''.$new.'\',grp.'.$fieldType.') = 0');
		}
	
		$select->where('find_in_set(enr.created_by,per.dotted_mngr_id) <0');
		$select->condition('enr.created_by','per.manager_id','!=');
		expDebug::dPrintDBAPI('$select query---->', $select);
		$result = $select->execute()->fetchField();
		$enrollId = explode(",", $result);
		expDebug::dPrint('result variable-->>'.print_r($enrollId,true),5);
		return $enrollId;
	}catch (Exception $ex) {
		watchdog_exception('getUpadeEnrollidForJobrole', $ex);
		expertusErrorThrow($ex);
	}
}

function getUpdatedEnrollmentsIdRevised($fieldType,$userId,$default,$new) {
	try{
		$condition = '';
		$orQuery = false;
		expDebug::dPrint('values in $fieldType: ' . print_r($fieldType,1),4);
		expDebug::dPrint('values in $default: ' . print_r($default,1),4);
		expDebug::dPrint('values in $new: ' . print_r($new,1),4);
		$select = db_select('slt_group_mapping','cat');
		$select->leftJoin('slt_groups','grp','grp.id = cat.group_id');
		$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->leftJoin('slt_enrollment','enr','((cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') OR (cat.entity_id = enr.course_id and cat.entity_type = \'cre_sys_obt_crs\') OR
  						 (cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') ) and enr.user_id='.$userId.' and enr.reg_status = \'lrn_crs_reg_cnf\'');
		$select->leftJoin('slt_person','per','per.id=enr.user_id');
		$select->addExpression('GROUP_CONCAT(DISTINCT(enr.id))', 'enroll_id');
		
		
 		if (isset($fieldType['ste_con_cnt']) && $default->country != $new['country']) {
 			$condition .= '(find_in_set(\''.$default->country .'\',grp.country) >0 and find_in_set(\''.$new['country'].'\',grp.country) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_dep']) && $default->dept_code != $new['dept_code'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->dept_code .'\',grp.department) >0 and find_in_set(\''.$new['dept_code'].'\',grp.department) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_ety']) && $default->employment_type != $new['employment_type']){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->employment_type.'\',grp.employment_type) >0 and find_in_set(\''.$new['employment_type'].'\',grp.employment_type) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_rol'])){ // differernt
 			
 			if($default->is_manager != $new['is_manager']){
 				$condition .= ($orQuery == true) ?  ' OR ' : '';
 				$condition .= '(find_in_set(\''.$default->is_manager.'\',grp.is_manager) >0 and find_in_set(\''.$new['is_manager'].'\',grp.is_manager) = 0)';
 				$orQuery = true;
 			}
 			
 			if($default->is_instructor != $new['is_instructor']){
 				$condition .= ($orQuery == true) ?  ' OR ' : '';
 				$condition .= '(find_in_set(\''.$default->is_instructor.'\',grp.is_instructor) >0 and find_in_set(\''.$new['is_instructor'].'\',grp.is_instructor) = 0)';
 				$orQuery = true;
 			}
 		}
 		
 		if (isset($fieldType['ste_con_jbr']) && $default->job_role != $new['job_role'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->job_role.'\',grp.job_role) >0 and find_in_set(\''.$new['job_role'].'\',grp.job_role) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_lng']) && $default->preferred_language != $new['preferred_language'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->preferred_language.'\',grp.language) >0 and find_in_set(\''.$new['preferred_language'].'\',grp.language) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_org']) && $default->org_id != $new['org_id'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->org_id.'\',grp.org_id) >0 and find_in_set(\''.$new['org_id'].'\',grp.org_id) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_ste']) && $default->state != $new['state'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->state.'\',grp.state) >0 and find_in_set(\''.$new['state'].'\',grp.state) = 0)';
 			$orQuery = true;
 		}
 		
 		if (isset($fieldType['ste_con_usr']) && $default->user_type != $new['user_type'] ){
 			$condition .= ($orQuery == true) ?  ' OR ' : '';
 			$condition .= '(find_in_set(\''.$default->user_type.'\',grp.user_type) >0 and find_in_set(\''.$new['user_type'].'\',grp.user_type) = 0)';
 			$orQuery = true;
 		}
 		
		if (isset($fieldType['ste_con_hdt']) && $default->hire_date != $new['hire_date']) {
			$condition .= ($orQuery == true) ?  ' OR ' : '';
			$condition .= '(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$default->hire_date.'\',\'%Y-%m-%d\'),1=0) OR
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$default->hire_date.'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$default->hire_date.'\',\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)) AND
							(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$new['hire_date'].'\',\'%Y-%m-%d\'),1=0) OR
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$new['hire_date'].'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$new['hire_date'].'\',\'%Y-%m-%d\') < DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') OR DATE_FORMAT(\''.$new['hire_date'].'\',\'%Y-%m-%d\') > DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=2)  )';
		} 
		
		expDebug::dPrint('final condtion formed: ' . $condition, 4);
		$select->where($condition);
		$select->where('find_in_set(enr.created_by,per.dotted_mngr_id) <0');
		$select->condition('enr.created_by','per.manager_id','!=');
		expDebug::dPrintDBAPI('$select query revised---->', $select);
		// $result = $select->execute()->fetchField();
		$enrollId = explode(",", $result);
		expDebug::dPrint('result variable-->>'.print_r($enrollId,true),5);
		return $enrollId;
	}catch (Exception $ex) {
		watchdog_exception('getUpadeEnrollidForJobrole', $ex);
		expertusErrorThrow($ex);
	}
}

function exp_sp_administration_viewuser_addedit_form($js = NULL, $entityId){
	try{
		//0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
		expDebug::dPrint('$entityId -->'.$entityId);
    	global $theme_key;
		   $css_theme_optional = array('type' => 'file', 'group' => CSS_THEME);
      if($theme_key == 'expertusoneV2') {
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view_v2.css', $css_theme_optional);
      }else{
      	drupal_add_css(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_view.css', $css_theme_optional);
      }
		$catalog_style = array(
				'ctools-viewscreen-wrapper' => array(
						'modalSize' => array(
								'width' => 755,
								'minHeight' => 550,
								'Height' => auto,
						),
						'modalTheme' => 'ExpertusCToolsAdminCourseModalTheme',
				)
		);

		drupal_add_js($catalog_style, 'setting');

		if ($js) {
			// Prepare the arguments list to be passed to $formState['build_info']['args'] of each form
			$argsList = array();
			$argsList[] = $entityId;

			// Prepare list of forms to be displayed in the modal
			$formIdList = array(
					array('id' => 'exp_sp_administration_view_userdetails_addedit_form', 'title' => (t('LBL816'))) //view
			);
			expDebug::dPrint('$form $classdetails---->555555'.print_r($formIdList,true),5);
			view_form_display($formIdList, $argsList);
		}
		else {
			throw new Exception('exp_sp_administration_catalog.module: exp_sp_administration_viewclass_addedit_form() : nojs');
		}
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_viewuser_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}


function exp_sp_administration_view_userdetails_addedit_form($form, &$form_state,$renderMode, $userId){
	try {
		include_once(drupal_get_path('module', 'exp_sp_coursedetail') .'/exp_sp_coursedetail.inc');
		expDebug::dPrint('$form 1111111t---->'.print_r($form,true),5);
		expDebug::dPrint('$form_state input---->'.print_r($form_state['input'],true),5);
		expDebug::dPrint('$form_state values---->'.print_r($form_state['values'],true),5);
		expDebug::dPrint('$entityId---->'.$userId);

		$wrapperId = 'viewuser-detail-wrapper';

		$form = newAddEditFormWithCancelAndSave($wrapperId);

		$userdetails = getUserDetailsForView($userId);
		expDebug::dPrint('$orgdetails-->'.print_r($userdetails,true),5);

		$form['narrow_userview'] = array(
				'#markup'       => theme('narrow_userview', array('results' => $userdetails,'type' => 'user')),
		);

		return $form;
	}
	catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_view_userdetails_addedit_form', $ex);
		expertusErrorThrow($ex);
	}
}

function getUserDetailsForView($userId){
	try{
		$select = db_select('slt_person', 'per');
		$select->leftJoin('slt_person', 'per1', 'per.manager_id = per1.id');
		$select->leftJoin('slt_organization', 'org1', 'per.org_id = org1.id');
		//$select->leftJoin('slt_organization', 'org2', 'find_in_set(org2.id,per.dotted_org_id) >0');
		$select->leftJoin('slt_location', 'location', 'per.preferred_loc_id = location.id');
		$select->leftJoin('slt_state', 'state', 'per.state = state.state_code AND per.country = state.country_code');
		$select->leftJoin('slt_country', 'country', 'per.country = country.country_code');
		$select->leftJoin('slt_profile_list_items', 'slpilang', 'per.preferred_language = slpilang.code');
		//$select->leftJoin('slt_profile_list_items', 'slpi', 'slpi.code in(CONCAT(spjm.job_role))');
		$select->leftjoin('slt_profile_list_items', 'splt_timezone', 'splt_timezone.code=per.time_zone');
		$select->leftJoin('slt_profile_list_items', 'plistatus', 'per.job_title = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('slt_profile_list_items', 'plitype', 'per.user_type = plitype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('slt_profile_list_items', 'pliempltype', 'per.employment_type = pliempltype.code AND plitype.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('slt_profile_list_items', 'plidepart', 'per.dept_code = plidepart.code AND plidepart.lang_code = \'cre_sys_lng_eng\'');
		$select->leftJoin('users', 'usr', 'usr.name = per.user_name');
		$select->addField('per','id','id');
		$select->addField('usr','uid','uid');
		$select->addField('per','first_name','first_name');
		$select->addField('per','last_name','last_name');
		$select->addField('per','user_name','user_name');
		$select->addField('per','email','email');
		$select->addField('per','addr1','addr1');
		$select->addField('per','addr2','addr2');
		$select->addField('per','city','city');
		$select->addField('per','zip','zip');
		$select->addField('per','is_instructor','is_instructor');
		$select->addField('per','webex_name','webex_name');
		$select->addField('per','is_manager','is_manager');
		$select->addField('slpilang','name','language');
		$select->addField('per','phone_no','contact');
		$select->addField('per','mobile_no','mobile');
		$select->addField('per','preferred_currency','preferred_currency');
		$select->addField('org1','name','organization');
		$select->addField('per','employee_no','employee_no');
		$select->addField('per1','full_name','manager');
		$select->addField('location','name','location');
		$select->addField('state','state_name','state');
		$select->addField('country','country_name','country');
		//$select->addField('slpi','name','jobrole');
		$select->addField('splt_timezone','name','timezone');
		$select->addField('plistatus','name','jobtitle');
		$select->addField('plitype','name','usertype');
		$select->addField('pliempltype','name','employment');
		$select->addField('plidepart','name','department');
		$select->addExpression('DATE_FORMAT(per.hire_date,\'%m-%d-%Y\')','hire_date');
		$select->addExpression('(select group_CONCAT(per2.full_name  separator \', \') from slt_person as per2 left join slt_person_other_manager as spom on per2.id in(spom.manager_id) where spom.user_id = '.$userId.' and spom.is_direct=\'N\')', 'dottedmanager');
		$select->addExpression('(select group_CONCAT(org2.name  separator \', \') from slt_organization as org2 left join slt_person_other_organization as spoo on org2.id in(spoo.organization_id) where spoo.user_id = '.$userId.' and spoo.is_direct=\'N\')', 'dottedorg');
		//$select->addExpression('GROUP_CONCAT(DISTINCT(org2.name))', 'dottedorg');
		$select->addExpression('(select group_CONCAT(si.name  separator \', \') from slt_profile_list_items as si left join slt_person_jobrole_mapping as  spjm on si.code in(spjm.job_role) where spjm.user_id = '.$userId.')', 'jobrole');
		$select->groupBy('per.id');
		$select->condition('per.id',$userId,'=');
		filterByUserAccessPrivileges($select, 'per','',1,'cre_usr','','per');
		expDebug::dPrintDBAPI('$select sabeena=',$select);
		$result = $select->execute()->fetchAll();
		expDebug::dPrint('v$result$result$result->'.print_r($result,true),4);
		return $result;

	}catch (Exception $ex) {
		watchdog_exception('getUserDetailsForView', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_grp_view($entityId, $entityType) {

	try {

		$form_state = array(
				'no_redirect' => TRUE, // To confirm whether necessary
				'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
				'entity_id' => $entityId,
				'entity_type' => $entityType,
				//'empty_id' => $emptyId,
		);

		$formId = 'exp_sp_administration_grp_view_html';
		$builtForm = drupal_build_form($formId, $form_state);
		$renderedForm = drupal_render($builtForm);

		$renderedTabContentHTML = $renderedForm;//$builtForm;

		$drupalSettings = getDrupalSettings();

		$commerceResponse = array(
				'drupal_settings' => $drupalSettings,
				'render_content_main' => $renderedTabContentHTML,

		);

		return drupal_json_output($commerceResponse);
		exit;

	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_grp_view', $ex);
		expertusErrorThrow($ex);
	}


}
/*
function exp_sp_administration_grp_owner_view($entityId, $entityType){
	try{
		$form_state = array(
				'no_redirect' => TRUE, // To confirm whether necessary
				'build_info' => array('args' => array('ajax')), // The form builder function expects the first argument to be ajax or nojs.
				'entity_id' => core_decrypt($entityId),
				'entity_type' => $entityType,
				//'empty_id' => $emptyId,
		);

		$formId = 'exp_sp_administration_grp_owner_view_html';
		$builtForm = drupal_build_form($formId, $form_state);
		$renderedForm = drupal_render($builtForm);

		$renderedTabContentHTML = $renderedForm;//$builtForm;

		$drupalSettings = getDrupalSettings();

		$commerceResponse = array(
				'drupal_settings' => $drupalSettings,
				'render_content_main' => $renderedTabContentHTML,

		);
			
		return drupal_json_output($commerceResponse);
		exit;

	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_grp_owner_view_html', $ex);
		expertusErrorThrow($ex);
	}
}
function exp_sp_administration_grp_owner_view_html($form, &$form_state, $renderMode){
	try{
		$pid = $form_state['entity_id'];
		$entityType = $form_state['entity_type'];
		$grpDetails = getOwnergroups($pid);
		expDebug::dPrint(' exp_sp_administration_grp_view  '.print_r($grpDetails,true),4);
		foreach($grpDetails as $grp){
			$var .= '<div><span id="group-names-'.$grp->id.'" class="usr-grp-name vtip" title="'.$grp->groupname.'">'.'- '.titleController(' ',$grp->groupname,40).'</span></div>';
		}
		if(empty($var)){
			$var = '<div class = "usr-grp-msg"><span id="no-groups" class = "info-grp-msg">'.t("No").' '.strtolower(t("Groups")).' '.strtolower(t("LBL736")).'</span></div>';
			$form['grp-view-user'] = array(
					'#type' => 'markup',
					'#markup' => $var,
			);
		}else{
			$form['grp-view-user'] = array(
					'#type' => 'markup',
					'#markup' => '<div id="usr-group-list"><span class="info-text">'.t("MSG769").'</span>'.$var.'</div>',
			);
		}
		return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_grp_view', $ex);
		expertusErrorThrow($ex);
	}
}
*/
function exp_sp_administration_grp_view_html($form, &$form_state, $renderMode){
	try{
		$pid = $form_state['entity_id'];
		$entityType = $form_state['entity_type'];
		$grpDetails = getGroupsDetails($pid,$entityType,'','view_users_group',0);
		expDebug::dPrint(' exp_sp_administration_grp_view  '.print_r($grpDetails,true),4);
		foreach($grpDetails as $grp){
			$var .= '<div><span id="group-names-'.$grp->id.'" class="usr-grp-name vtip" title="'.sanitize_data($grp->name).'">'.'- '.titleController('EXP-SP-ADMIN-USER-GRPNAME',sanitize_data($grp->name),40).'</span></div>';
		}
		if(empty($var)){
			$var = '<div class = "usr-grp-msg"><span id="no-groups" class = "info-grp-msg">'.t("No").' '.strtolower(t("Groups")).' '.strtolower(t("LBL736")).'</span></div>';
			$form['grp-view-user'] = array(
					'#type' => 'markup',
					'#markup' => $var,
			);
		}else{
			$form['grp-view-user'] = array(
					'#type' => 'markup',
					'#markup' => '<div id="usr-group-list"><span class="info-text">'.t("MSG715").'</span>'.$var.'</div>',
			);
		}
		return $form;
	}catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_grp_view', $ex);
		expertusErrorThrow($ex);
	}
}

function getExistingGrpList($perId){
	try{

		// Select table
		$select = db_select('slt_groups', 'grp');
		$select->join('slt_person','per');
		// Select fields and/or add expressions
		$select->addField('grp', 'name', 'name');
		$select->addField('grp', 'id', 'id');
		$select->where("(FIND_IN_SET(per.id,grp.userslist)>0) AND (grp.added_users is null OR FIND_IN_SET(per.id,grp.added_users)<=0)");
		// Set conditions
		$select->condition('per.id', $perId, '=');
		$select->condition('grp.code', array('grp_adm', 'grp_sup'), 'NOT IN');
		$select->condition('grp.is_admin', 1, '=');
		expDebug::dPrintDBAPI(' $$$select obj =' , $select);
		$result = $select->execute()->fetchAll();
		return $result;

	}catch (Exception $ex) {
		watchdog_exception('getSecurityDefDetails', $ex);
		expertusErrorThrow($ex);
	}
}
/*
 * function to fetch mandatory enroll id for access.
* */
function getUpdatedEnrollmentsIdForMandatory($fieldType,$userId,$default,$new) {
	try{
		$select = db_select('slt_group_mapping','cat');
		$select->leftJoin('slt_groups','grp','grp.id = cat.group_id');
		$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->leftJoin('slt_enrollment','enr','((cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') OR (cat.entity_id = enr.course_id and cat.entity_type = \'cre_sys_obt_crs\') OR
				(cat.entity_id = enr.class_id and cat.entity_type = \'cre_sys_obt_cls\') ) and enr.user_id='.$userId.' and enr.reg_status = \'lrn_crs_reg_cnf\' and cat.mro=\'cre_sys_inv_man\'');
		//$select->leftJoin('slt_mro_mapping','mro','(mro.entity_id = cat.entity_id and mro.entity_type = cat.entity_type and mro.mro_flag= \'cre_sys_inv_man\')');
		$select->addExpression('GROUP_CONCAT(DISTINCT(enr.id))', 'enroll_id');
		if ($fieldType == 'hire_date') {
			$select->where('(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$default.'\',\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)) AND
							(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\') < DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') OR DATE_FORMAT(\''.$new.'\',\'%Y-%m-%d\') > DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=2)  )');
		} else {
		$select->where('find_in_set(\''.$default.'\',grp.'.$fieldType.') >0 and find_in_set(\''.$new.'\',grp.'.$fieldType.') = 0');
		}
		expDebug::dPrintDBAPI('getUpdatedEnrollmentsIdForMandatory', $select);
		$result = $select->execute()->fetchField();
		$enrollId = explode(",", $result);
		expDebug::dPrint('result getUpdatedEnrollmentsIdForMandatory'.print_r($enrollId,true),5);
		return $enrollId;
	}catch (Exception $ex) {
		watchdog_exception('getUpdatedEnrollmentsIdForMandatory', $ex);
		expertusErrorThrow($ex);
	}
}

function getUserIdFromGroupUserList($selOpt) {
  expDebug::dPrint(' entered into groupDetailUserListForAPI '.print_r($selOpt, true),4);
  $optional_array = array();
  foreach ($selOpt as $row) {
    if (isset($row->user_id)) {
      //$optional_array[] = 'Full Name :'.$row->fname.''.'ID:'.$row->user_id.''.',';
      $optional_array[] = $row->user_id;
    }
  }
  return implode(',',$optional_array);
}
function getTeamResourceCount($manager_id){
	$selectStmt = db_select('slt_person', 'sp');
	// Set conditions
	$selectStmt->condition('sp.manager_id', $manager_id);
	$selectStmt->condition('sp.status', 'cre_usr_sts_atv');
	// Select fields and/or add expressions
	$selectStmt->addExpression('COUNT(sp.id)', 'count');
	expDebug::dPrintDBAPI('$selectmanager resource user = ', $selectStmt);
	// Execute query
	$numResource = $selectStmt->execute()->fetchField();
	return $numResource;
}

function getGrpEntityIdListByUser($usrId,$mro=''){
	//$userName = $_GET['z'];
	try {
		$select = db_select('slt_person','per');
		$select->join('slt_groups','grp');
		$select->join('slt_group_mapping','grp_map', 'grp_map.group_id=grp.id');
		$select->leftjoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$select->join('users','usr','usr.name = per.user_name');
		$select->addField('grp_map','entity_id','entity_id');
		$select->addField('grp_map','entity_type','entity_type');
		$select->condition('per.id',$usrId,'=');
		$select->condition('grp_map.mro',$mro,'=');
			
		$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
		$select->where("(((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
				AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
				AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
				AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
				AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
				AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
				AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
				AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
				AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
				THEN
				(ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
				WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
				THEN
				(ifnull(per.is_manager,'N') = grp.is_manager)
				WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
				THEN
				(ifnull(per.is_instructor,'N') = grp.is_instructor)
				ELSE
				1=1
				END)
				AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
				AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))");
		$select->groupBy("grp_map.id");

		expDebug::dPrintDBAPI('list of users in role' ,   $select);
		$result = $select->execute()->fetchAll();

		expDebug::dPrint(' Group ID $result = ' . print_r($result, true) ,3);
		return $result;
	}catch(Exception $ex) {
		watchdog_exception('getGrpEntityIdListByUser', $ex);
		expertusErrorThrow($ex);
	}
}
function compliance_access(){
	try{
		$select = db_select('slt_course_template','crs');
		$select->addField('cls','id','class_id');
		$select->leftjoin('slt_course_class','cls','crs.id = cls.course_id');
		$select->leftjoin('slt_group_mapping','map','map.entity_id = crs.id and map.entity_type = \'cre_sys_obt_crs\'');
		$select->leftjoin('slt_group_mapping','map1','map1.entity_id = cls.id and map1.entity_type = \'cre_sys_obt_cls\'');
		$select->condition('crs.is_compliance',1,'=');
		$select->isNull('map.id');
		$select->isNull('map1.id');
		$result = $select->execute()->fetchAll();
		return $result;
	}catch(Exception $ex) {
		watchdog_exception('getGrpEntityIdListByUser', $ex);
		expertusErrorThrow($ex);
	}
}
//Added by vetrivel.P for #0070900
function exp_sp_administration_upload(){
	try {
		if(isset($_POST["user_upload_done_btn"])) {
			
		expDebug::dPrint('The value of $_FILES is here '.print_r($_FILES,1),5);
			$file = $_FILES['files']['tmp_name'];
			$filename = $_FILES['files']['name'];
			//expDebug::dPrint('button name '. $enrbut);
			//expDebug::dPrint('File Name '. $filename);
			// Checking the File extension is CSV and proceed if it is TRUE.
			$file_name = $_FILES['files']['name'];
			$extensions         = array('.csv',);
			$extension          = strrchr($file_name, '.');
			
			if (in_array(strtolower($extension), $extensions))    {
				$file_format = true;
			}else{
				$message = t('ERR171').' '.t('ERR206').'.'.' '.t('MSG824');
				return $message;
			}
			$dataload_path = getConfigValue('dataload_file_path');
			if($dataload_path == '') {
				$message = t('ERR057');
				expDebug::dPrint("dataload_file_path not set in ex_sp.ini", 4);
				return $message;
			}
			else if ($file_format) {
				$fileext = pathinfo($filename, PATHINFO_EXTENSION);
				$file_name = substr_replace($filename, '_'.microtime(false), strpos($filename,'.'));
			 	$file_path = str_replace(' ', '_', $dataload_path . "/" . $file_name.'.'.$fileext);
				if(!move_uploaded_file($file,$file_path)) {
					expDebug::dPrint("enrollment upload file move failed", 4);
					$message = t('ERR057');
					return $message;
				}
				
				if (($handle = fopen($file_path, "r")) !== FALSE) {
					$data = fgetcsv($handle, 1000, ",");
					$hdr_col_arr = explode("|", $data[0]);
					$hdr_col_arr = array_map('strtolower', $hdr_col_arr);
					$hdr_col_arr = array_map('trim', $hdr_col_arr);
                                    
					$hdr_arr = array("first_name","last_name","user_name","password","status","email","contact_no","addr1","addr2","city","country","state","zip_code","timezone","preferred_language","mobile_no","preferred_currency","org_number","manager_username","empl_type_code","employee_no","dept_code","job_title_code","job_role_code","user_type","is_manager","is_instructor","other_managers","other_org_number","sms_alert","hire_date","is_mobileadmin","webex_name","webex_pass","groups");
                    
                    if(count($hdr_col_arr) >= 35 && module_exists('exp_sp_administration_customattribute')) //#custom_attribute_0078975 - Check module status
                    {
                        
                        $sel = db_select ( "slt_custom_attr", "attr" );
                        $sel->join ( "slt_custom_attr_mapping", "map", "map.cattr_id = attr.id and attr.status='cre_cattr_sts_atv' and map.entity_type = 'cre_usr' and map.entity_screen_opt = 1" );
                        $sel->addExpression("LOWER(REPLACE(attr.cattr_name,' ','_'))", "title");
                        expDebug::dPrintDBAPI ( "fetchcount------>", $sel );
                        $sel1 = $sel->execute()->fetchCol();
                        $result1 = array_merge($hdr_arr, $sel1);    
                        $result=array_diff($hdr_col_arr, $result1);
                        expDebug::dPrint("csv header data :".print_r($result, 1), 5);
                
                        if(count($result) > 0)
                        {
                            $result = implode(",", $result);
                            expDebug::dPrint("csv header data :".print_r($result, 1), 1);
                            $message = t('Invalid column name');
                            return $message;
                        }
                    }
                    else if(count($hdr_col_arr) == 35 && !module_exists('exp_sp_administration_customattribute'))
                        
					{
						$result=array_diff($hdr_col_arr, $hdr_arr);
						expDebug::dPrint("csv header data :".print_r($result, 1), 5);
				
						if(count($result) > 0)
						{
							$result = implode(",", $result);
							expDebug::dPrint("csv header data :".print_r($result, 1), 1);
							$message = t('Invalid column name');
							return $message;
						}
					}                   
					else {
						$message = t('Invalid column name');
						return $message;
					}
					fclose($handle);
				}
				$startDate = date_format(date_now(date_default_timezone(FALSE)),'Y-m-d H:i');
				$userId = getIdOfLoggedInUser();
				$userDet = getDrupalUserForNotificationInsert($userId);
				$insertFilename = str_replace(' ', '_', $file_name.'.'.$fileext);
				//File size conversion
				$unit = intval(log($_FILES['files']['size'], 1024));
				$units = array('B', 'KB', 'MB', 'GB');
				$fileSize = round($_FILES['files']['size'] / pow(1024, $unit),2). $units[$unit];
				$fields=array(
						'entity_type' => 'cre_usr',
						'file_name' => $insertFilename,
						'file_size' => $fileSize,
						'process_type' => 'bulk',
						'user_id' => $userId,
						'mail_to' => '',
						//'starts_when' => $startDate->format('Y-m-d H:i'),
						'starts_when' => $startDate,
						'status' => 'NS',
						'custom0' => 'user_file'
				);
				// Insert into Dataload Jobs table.
				$insertStmt = db_insert('slt_dataload_jobs');
				$insertStmt->fields($fields);
				expDebug::dPrintDBAPI('$insert Dataload Id ' , $insertStmt);
				$insertId = $insertStmt->execute();
				make_audit_trail_entries($fields, $userId, null, 'User','Uploaded','cre_usr','exp_sp_administration_user','exp_sp_administration_upload',
						array('file_name','file_size','process_type','user_id','mail_to','starts_when','status', 'custom0'));
				// Update the Job code and Job id.
				$jobId = 'job'.$insertId;
				$jobcode = str_replace(' ', '_', 'User Create').'_'.$jobId;
				$updateDtld = db_update('slt_dataload_jobs');
				$updateDtld->condition('id', $insertId, '=');
				$updateDtld->fields(array(
						'job_id'       => $jobId ,
						'job_code'    => $jobcode
				));
				expDebug::dPrintDBAPI('Update the Job id and code ', $updateDtld);
				$updateDtld->execute();
				$updateDt = array();
				$updateDt[job_id]=$jobId;
				$updateDt[job_code]=$jobcode;
				make_audit_trail_entries($updateDt, $userId, null, 'User' ,'Updated','cre_usr','exp_sp_administration_user','exp_sp_administration_upload',
						array('job_id', 'job_code'));
				
				include_once DRUPAL_ROOT."/dataload/dataloadDirectExec.php";
				$data = array(
						'document_root'=>DRUPAL_ROOT,
						'remote_addr'=>'127.0.0.1',
						'id'=>$insertId,
						'operation'=>'Instant'
				);
				$obj =  new dataloadDirect();
				$enRt = $obj->start($data);
				
				if($enRt == 'Successfully Completed'){
					
					$select = db_select('slt_dataload_jobs','job');
					$select->addField('job','total_records','total_records');
					$select->addField('job','status','status');
					$select->addField('job','failure_records','failure_records');
					$select->condition('job.id',$insertId,'=');
					$result = $select->execute()->fetchAll();
					
					$totalrecord = $result[0]->total_records;
					$failurerecords = $result[0]->failure_records;
					$status = $result[0]->status;
					$sucess = $totalrecord - $failurerecords;
					if($status == 'FL'){
						$message = t('MSG826', array('@failurerecords'=>$totalrecord));
					}else{
						if($failurerecords == 0 && $sucess != 0){
							$message = t('MSG825', array('@sucess'=>$sucess));
						}elseif($sucess == 0 && $failurerecords !=0){
							$message = t('MSG826', array('@failurerecords'=>$failurerecords));
						}else{
							$message = t('MSG827', array('@sucess'=>$sucess,'@failurerecords'=>$failurerecords));
						}
					}
					return $message;	
				
				}else{
					$message = t('MSG828');
					return $message;
				}
			}
		}
	} catch (Exception $ex) {
		watchdog_exception('exp_sp_administration_upload', $ex);
		expertusErrorThrow($ex);
	}
}
//End for #0070900 by vetrivel.P
?>