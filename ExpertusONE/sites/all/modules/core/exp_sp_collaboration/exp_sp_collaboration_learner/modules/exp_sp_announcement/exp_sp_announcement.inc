<?php
/*
 * getAnnouncements() - Fetch the top 5 announcements to be displayed to the learner
 */
function getAnnouncements() {	
	
  try {
  	// Get user's preferred language
  	$userPrefLang = getUserPreferredLanguage();
  	expDebug::dPrint('$userPrefLang = ' . $userPrefLang, 4);
  	
	  // Select announcement master table
	  $select = db_select('slt_announcement_master', 'ann');
	  
	  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=ann.id AND catacs.entity_type = \'cre_sys_obt_anm\' ');
	  //$select->leftjoin('slt_group_mapping', 'group_map', 'group_map.entity_id=ann.id AND group_map.entity_type = \'cre_sys_obt_anm\' ');
	  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
	  // Select these specific fields for the output.
	  $select->distinct();
	  $select->addField('ann', 'id');
	  $select->addField('ann', 'title');
	  $select->addField('ann', 'shortdesc');
	  $select->addField('ann', 'banner_thumbnail');    
	  $select->addField('ann', 'banner_large');
	
	  // Filter active banner type announcements falling within the admin specified time frame.
	  $select->condition('ann.ann_type', 'cbn_anm_typ_ban');
	  $select->condition('ann.status', 'cbn_anm_sts_atv');
	  
	  if (!empty($userPrefLang)) {
	    $select->condition('ann.lang_code', $userPrefLang, '=');
	  }

	  $currentDate = date("Y-m-d");
	  expDebug::dPrint('$currentDate = ' . $currentDate, 4);

	  // banner display type - display always
    $displayType1 = db_and()
                    ->isNull('ann.date_activate')
                    ->isNull('ann.date_deactivate');
    // banner display type - display always from a particular date
	  $displayType2 = db_and()
	                  ->isNotNull('ann.date_activate')
	                  ->isNull('ann.date_deactivate')
	                  ->condition('ann.date_activate', date($currentDate), '<=');
	  // banner display type - display always until a particular date
	  $displayType3 = db_and()
	                  ->isNull('ann.date_activate')
	                  ->isNotNull('ann.date_deactivate')
	                  ->condition('ann.date_deactivate', date($currentDate), '>=');
	  // banner display type - display from a particular date until a particular date
    $displayType4 = db_and()
                    ->isNotNull('ann.date_activate')
                    ->isNotNull('ann.date_deactivate')
                    ->condition('ann.date_activate', date($currentDate), '<=')
                    ->condition('ann.date_deactivate', date($currentDate), '>=');
                    
	  $select->condition(db_or()->condition($displayType1)
	                            ->condition($displayType2)
	                            ->condition($displayType3)
	                            ->condition($displayType4));
  
	  // Further filter by role
	  //addFilterForLoggedInUserRole($select, 'sepm');
	  //filterByUserAccessPrivileges($select, 'ann','','group_map');
	  //41373: Banners-Banners visible based on Access privileges is not working (Entity type given worng so that not work)
	  filterByUserAccessPrivileges($select, 'ann','','','cbn_anm_typ_ban','Announcement');
  	$loggedinUserID = getIdOfLoggedInUser();
	  if($loggedinUserID <= 0){
	  	$select->condition('group_map.id','','IS NULL');
	  }
	
	  // Order by display sequence number
	  $select->orderBy('ann.banner_seq_num', 'ASC');
	  $select->orderBy('ann.created_on', 'ASC');//changes made for ticket no:19698
	
	  // Fetch only as many announcement records as have been configured by admin 
	  $exp_sp_announcement_block_no_of_display =5;// variable_get('exp_sp_announcement_block_no_of_display', 5); // Fetch max 5 anm, if variable is not set
	  $select->range(0, $exp_sp_announcement_block_no_of_display);
	
	  expDebug::dPrintDBAPI('$select' , $select);
	  
	  $announceRes = $select->execute()->fetchAll();
	  expDebug::dPrint('$announceRes = ' . print_r($announceRes, true));
	  
	  $exp_sp_announcement_block_no_of_display = (count($announceRes) <= $exp_sp_announcement_block_no_of_display) ? count($announceRes) : $exp_sp_announcement_block_no_of_display;
	  $block_seconds_pager = variable_get('exp_sp_announcement_block_seconds_pager', 8000);
	  $block_seconds_pager = ($block_seconds_pager)?$block_seconds_pager:8000;
	  $vars = array('totalRecords' => count($announceRes),
					'exp_sp_announcement_block_seconds_pager' =>$block_seconds_pager,
					'exp_sp_announcement_block_no_of_display' =>$exp_sp_announcement_block_no_of_display,
	               );
	  expDebug::dPrint(' $vars = ' . print_r($vars, true),4);
	  drupal_add_js(array('exp_sp_announcement' => $vars), 'setting');
	    
	  return $announceRes;
  }catch(Exception $ex) {	
  	watchdog_exception('getAnnouncements', $ex);
  	expertusErrorThrow($ex);
  }
}
// new function is created to get the count of list of banners 
function getListOfBannersAPI(){
  try{
	$result = getBannersAPI();
	$count = getBannersAPI('COUNT');
	$result['totalrow'] = $count ;
 	return $result;
  }catch(Exception $ex) {	
  	watchdog_exception('getBannersAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

function getBannersAPI($count = '')
{	
  try{
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		// Select announcement master table
	  $select = db_select('slt_announcement_master', 'ann');
	  $select->leftJoin('slt_profile_list_items', 'plilang', 'ann.lang_code = plilang.code AND plilang.lang_code = \'cre_sys_lng_eng\'');
	  $select->leftJoin('slt_profile_list_items', 'plistatus', 'ann.status = plistatus.code AND plistatus.lang_code = \'cre_sys_lng_eng\'');
	  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=ann.id AND catacs.entity_type = \'cre_sys_obt_anm\' ');	  
	  //$select->leftjoin('slt_group_mapping', 'group_map', 'group_map.entity_id=ann.id AND group_map.entity_type = \'cre_sys_obt_anm\' ');
	  //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
	  
	  // Left join slt_entity_profile_mapping table (for filtering by role)
	  //$select->leftJoin('slt_entity_profile_mapping', 'sepm', 'ann.id = %alias.entity_id AND %alias.entity_type = \'05\'');
	  $select->distinct();
	  if(!empty($count)){
          $select->addExpression('count(1)','cnt');
      }else{                                                                            // see table slt_profile_tagging_defn for defn of entity type 5
	  // Select these specific fields for the output.
	  $select->addField('ann', 'id');
	  $select->addField('ann', 'title');
	  $select->addField('ann', 'shortdesc');
	  $select->addField('ann', 'banner_thumbnail');
	  $select->addField('ann', 'banner_large');
	  //change by ayyappans for 41589: API Review - Data Missing in list banner API
	  $select->addField('ann', 'date_activate', 'display_from');
	  $select->addField('ann', 'date_deactivate', 'display_to');
	  $select->addField('ann', 'banner_seq_num', 'banner_sequence');
	  $select->addField('plilang', 'name', 'language');
	  $select->addField('plistatus', 'name', 'status');
      }
	  // Filter active banner type announcements falling within the admin specified time frame.
	  $select->condition('ann.ann_type', 'cbn_anm_typ_ban');
	  $select->condition('ann.status', 'cbn_anm_sts_atv');
	  $currentDateTime = now();
	  $andcondition = db_and()->condition('ann.date_activate', $currentDateTime, '<=')->condition('ann.date_deactivate', $currentDateTime, '>=');
	  $select->condition(db_or()
	    ->isnull('ann.date_activate')->isnull('ann.date_deactivate')
	    ->condition($andcondition)
	    );
	 // Further filter by role
	  //addFilterForLoggedInUserRole($select, 'sepm');
	  //filterByUserAccessPrivileges($select, 'catacs','','group_map');
	  filterByUserAccessPrivileges($select, 'ann','','','cre_sys_obt_anm','Announcement');
	   // Order by display sequence number
	  $select->orderBy('ann.banner_seq_num', 'ASC');
	  if(!empty($count))
	  	$announceRes = $select->execute()->fetchField();
	  else
	  	$announceRes = $select->execute()->fetchAll();
	  if(is_array($announceRes))
	  foreach ($announceRes as &$banner) {
	  	$banner->display_from = empty($banner->display_from) ? '' :date('m-d-Y', strtotime($banner->display_from));
	  	$banner->display_to = empty($banner->display_to) ? '' :date('m-d-Y', strtotime($banner->display_to));
	  }
	  return $announceRes;
  }catch(Exception $ex) {	
  	watchdog_exception('getBannersAPI', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getMylearningAnnouncements() - Fetch the top announcements to be displayed to the my learning page
 */
function getMylearningAnnouncements() {	
  try {

	  global $user;
	  global $base_url;
	  
	  $page = $_GET['page']; // get the requested page
	  $limit = $_GET['rows'];
	  $sord = 'ASC'; // Sort is always ascending by full_name	
	  // Select announcement master table
	  $select = db_select('slt_announcement_master', 'ann');
	  //$select->leftjoin('slt_catalog_access', 'catacs', 'catacs.entity_id=ann.id AND catacs.entity_type = \'cre_sys_obt_anm\' ');	 
	  //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=ann.id AND group_map.entity_type = \'cre_sys_obt_anm\' ');
	// $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
	  
	  // Select these specific fields for the output.
	  $select->addField('ann', 'id');
	  $select->addField('ann', 'title');
	  $select->addField('ann', 'shortdesc');
	  $select->addField('ann', 'banner_thumbnail');    
	
	  // Filter active banner type announcements falling within the admin specified time frame.
	  $select->condition('ann.ann_type', 'cbn_anm_typ_ban');
	  $select->condition('ann.status', 'cbn_anm_sts_atv');
	  $currentDateTime = now();
	  $select->condition('ann.date_activate', $currentDateTime, '<=');
	  $select->condition('ann.date_deactivate', $currentDateTime, '>=');
	
	  // Further filter by role
	  //addFilterForLoggedInUserRole($select, 'sepm');
	 // filterByUserAccessPrivileges($select, 'catacs','','group_map');
	  filterByUserAccessPrivileges($select, 'ann','','','cre_sys_obt_anm','Announcement');
	  // Order by display sequence number
	  $select->orderBy('ann.banner_seq_num', 'ASC');
	  
	  $learnerAnnounce = $select->execute()->fetchAll();
	  expDebug::dPrint('$mytranscript : = ' . print_r($mytranscript, true),4);
	  $myLearningAnnouncementCount = count($learnerAnnounce);
	  
	  if($myLearningAnnouncementCount > 0) {
	    $total_pages = ceil($myLearningAnnouncementCount / $limit);
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
	  // Fetch only as many announcement records as have been configured by admin 
	  $select->range($start, $limit);
	
	  $myLearningAnnounceList = $select->execute()->fetchAll();
	  
	  $response->page = $page;
	  $response->total = $total_pages;
	  $response->records = $myLearningAnnouncementCount;
	  $i=0;
	  foreach ($myLearningAnnounceList as $currentRecord) {
	    $response->rows[$i]['id'] = $currentRecord->id; 
	    $response->rows[$i]['cell']['title'] = titleController('EXP-SP-ANNOUNCEMENT-TITLE',$currentRecord->title,30);
	    $response->rows[$i]['cell']['full_title'] = $currentRecord->title;
	    $response->rows[$i]['cell']['shortdesc'] = descController($title,$currentRecord->shortdesc);
	    $i++;
	  }
	  expDebug::dPrint('$getMylearningAnnouncements : = ' . print_r(drupal_json_encode($response), true),4);
	  echo drupal_json_encode($response);
  }catch(Exception $ex) {	
  	watchdog_exception('getMylearningAnnouncements', $ex);
  	expertusErrorThrow($ex);
  }
  
}

?>