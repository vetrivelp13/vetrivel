<?php
/**
 * User Point Admin inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 04-Nov-2011
 */

/*
 * getPointDetailsForUserPoint() - For getting User Point details settup by admin
 */

function getPointDetailsForUserPoint() {
	try {
  $select = db_select('slt_master_points','points');
  $select->addField('points','code');
  $select->addField('points','name');
  $select->addField('points','points');
  $result= $select->execute()->fetchAll();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getPointDetailsForUserPoint', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getUserPointsByCode() - For getting User Point values settup by admin
 * function for getting the points for each action performed - It is called from the common function for inserting the values into the user point table
 */

function getUserPointsByCode($code) {
	try {
  $select = db_select('slt_master_points','smp');
  $select->addField('smp','points');
  $select->condition('smp.code','%'.$code.'%','LIKE');
  $result = $select->execute()->fetchField();
  expDebug::dPrintDBAPI('query for points from master table',$select);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getUserPointsByCode', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * getBadgeDetailsForUserPoint: update the points for each badges
 */
function getBadgeDetailsForUserPoint() {
	try {
  $select = db_select('slt_badge_points','badge');
  $select->addField('badge','badge_code');
  $select->addField('badge','badge_points');
  $select->addField('badge','badge_title');
  expDebug::dPrintDBAPI('getBadgeDetailsForUserPoint select query',$select);
  $result= $select->execute()->fetchAll();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getBadgeDetailsForUserPoint', $ex);
  	expertusErrorThrow($ex);
  }
}
function getUserPointActionBadgeCode($totalPoints,$code) {
	try {
  $select = db_select('slt_badge_points','badge');
  $select->addField('badge','badge_code');
  $select->condition('badge.badge_code',$code,'=');
  $select->condition('badge.badge_points',$totalPoints,'<=');
  $result= $select->execute()->fetchField();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getUserPointActionBadgeCode', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * function for getting the existing details of user points.
 */
function getDetailsToDeleteUserPoints($code,$entityId,$userId,$entityType) {
	try {
  $query = db_select('slt_user_points','usr');
  $query->addField('usr','id');
  $query->condition('usr.action_code',$code,'=');
  $query->condition('usr.entity_id',$entityId,'=');
  $query->condition('usr.user_id',$userId,'=');
  $query->condition('usr.operation_flag','insert','=');
  if($entityType != 'others') {
    $query->condition('usr.entity_type',$entityType,'=');
  }
  $result = $query->execute()->fetchField();
  expDebug::dPrintDBAPI('query result for user point deletion',$query);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getDetailsToDeleteUserPoints', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * function for getting points for each seperate action to displaying that in Points panel of profile page
 * getting total points for the user
 */
function getActionPointsForUser($userId,$actionCode='') {
  try{
  $selectStmt = db_select('slt_user_points','points');
  $selectStmt->addField('points','action_code');
  $selectStmt->addExpression('SUM(points.total_points)','actionpoints');
  $selectStmt->condition('points.user_id',$userId,'=');
  $selectStmt->condition('points.operation_flag','insert','=');
  $selectStmt->groupBy('points.action_code');
  expDebug::dPrintDBAPI('getSeperatePointsForAction', $selectStmt);
  $result = $selectStmt->execute()->fetchAllKeyed();
  return $result;
  }
  catch(Exception $ex) {
    watchdog_exception('getActionPointsForUser', $ex);
    expertusErrorThrow('getActionPointsForUser',$ex);
  }
}

/*
 * getTotalUserPoints : to get the total badges awarded to the user
 * Added for showing user points details
 */
function getTotalUserBadges() {
	try {
  $selectStmt = db_select('slt_badge_points','bdgpnt');
  $selectStmt->addField('bdgpnt','badge_code');
  $selectStmt->addField('bdgpnt','badge_points');
  //$selectStmt->condition('bdgpnt.badge_points',$totalPoints,'<=');
  $result = $selectStmt->execute()->fetchAllKeyed();
  expDebug::dPrintDBAPI('getTotalUserBadge',$selectStmt);
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getTotalUserBadges', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * getBadgesCodeForTotalPoints : for getting getBadgesCodeForTotalPoints
 */
function getTotalPointsGainedByUser($perId,$action_code='') {
	try {
  $selectStmt = db_select('slt_user_points','points');
  $selectStmt->addExpression('SUM(points.total_points)','actionpoints');
  $selectStmt->condition('points.user_id',$perId,'=');
  $sharemodule = getShareModuleStatus('profile');
  if($sharemodule==false){
  $selectStmt->condition('points.action_code','sharing_class','!=');
  }
  $selectStmt->condition('points.operation_flag','insert','=');
  $result = $selectStmt->execute()->fetchField();
  $result = ($result)? $result : 0;
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getTotalPointsGainedByUser', $ex);
  	expertusErrorThrow($ex);
  }
}
function BadgesCount($resultTotal) {
  try {
  $sharemodule=getShareModuleStatus('profile');	
  $regPoints = ($resultTotal['register_class']) ? $resultTotal['register_class'] : 0;
  $compPoints = ($resultTotal['complete_class_training']) ? $resultTotal['complete_class_training'] : 0;
  $topicCommentPoints = ($resultTotal['add_topic_comment_reply']) ? $resultTotal['add_topic_comment_reply'] : 0;
  $ratePoints = ($resultTotal['rating_class']) ? $resultTotal['rating_class'] : 0;
  $sharePoints = ($sharemodule == true ? ($resultTotal['sharing_class'] ? $resultTotal['sharing_class'] : 0) : 0);

  $keyedValues = getTotalUserBadges();
  
  $totalCnt = 0;
  $totalCnt = ($keyedValues['cre_sys_upt_bg1'] <= $regPoints) ? ($totalCnt+1) : $totalCnt;
  $totalCnt = ($keyedValues['cre_sys_upt_bg2'] <= $sharePoints) ? ($totalCnt+1) : $totalCnt;
  $totalCnt = ($keyedValues['cre_sys_upt_bg3'] <= $topicCommentPoints) ? ($totalCnt+1) : $totalCnt;
  $totalCnt = ($keyedValues['cre_sys_upt_bg4'] <= $compPoints) ? ($totalCnt+1) : $totalCnt;
  $totalCnt = ($keyedValues['cre_sys_upt_bg5'] <= $ratePoints) ? ($totalCnt+1) : $totalCnt;
  return $totalCnt;
  } catch (Exception $ex) {
  	watchdog_exception('BadgesCount', $ex);
  	expertusErrorThrow($ex);
  }
}
/*
 * updateMasterPoints: update the points for each action
 */
function updateMasterPoints($form, $form_state, $loggedInUserId, $callFromAPI =0) {
  expDebug::dPrint('form state values contains code and points'.print_r($form_state['values'],true),5);
  $txn = db_transaction();
  try{
    $oldPoints = db_select('slt_master_points','mstpnt');
    $oldPoints->addField('mstpnt','code');
    $oldPoints->addField('mstpnt','points');
    $oldPointsResult = $oldPoints->execute()->fetchAllKeyed();
    
    $oldBadgesPoint = db_select('slt_badge_points','bdg_pnt');
    $oldBadgesPoint->addField('bdg_pnt','badge_code');
    $oldBadgesPoint->addField('bdg_pnt','badge_points');
    $oldBadgesPointResult = $oldBadgesPoint->execute()->fetchAllKeyed();
    
    foreach($oldPointsResult as $key => $value){
      if($form_state['values'][$key] != $value) {
        $fields = array(
                'points'     	     => ($form_state['values'][$key]) ? $form_state['values'][$key] : 0,
                'updated_by'	     => $loggedInUserId,  
                'updated_on'         => now(),
        );
        $update = db_update('slt_master_points');
        $update->fields($fields);
        $update->condition('code',$key);
        $update->execute();
      }
    }
    foreach($oldBadgesPointResult as $key1 => $value1){
      if($form_state['values'][$key1] != $value1) {
        $fields1 = array(
                'badge_points'     	 => ($form_state['values'][$key1]) ? $form_state['values'][$key1] : 0,
                'updated_by'	     => $loggedInUserId,  
                'updated_on'         => now(),
        );
        $updatestmt = db_update('slt_badge_points');
        $updatestmt->fields($fields1);
        $updatestmt->condition('badge_code',$key1);
        $updatestmt->execute();
      }
    }
  }
  catch(Exception $ex) {
    $txn->rollback();
    watchdog_exception('updateMasterPoints', $ex);
  }

}

/*
 * addUserPointsTextFieldToAddEditForm() -  Use this function to add a text field to the addedit form
 */
function addUserPointsTextFieldToAddEditForm(&$form, $id, $title = '', $defaultValue = '', $tabIndex = null, $mandatory = false, $readOnly = false, $maxChars = 99, $viewWidthInChars = 60, $description = false) {
	try {
  // Prepare the additional HTML attributes for the textfield
  $attributes = array();
  $attributes['autocomplete'] = 'off'; // Prevent browser autocomplete

  // We need to add a class selector for any jQuery as we cannot use id which gets appended by an incrementing number
  // each time the form is re-built.
  $attributes['class'] = array('addedit-edit-' . $id); // This class must be prefixed with 'addedit-edit-' followed by the form field id.
  // Add class to apply common style to all text fields.
  $attributes['class'][] = 'addedit-edit-textfield';
  $attributes['onkeydown'][] = 'return onlyNumbers(event)';

  /*if($defaultValue == 0){
    $attributes['class'][] = 'addedit-empty-points-textfield';
    $attributes['class'][] = 'add-'.$id.'-points';
    $attributes['onblur'][] = "userPointsOnBlur(this,'".$id."');";
    $attributes['onfocus'][] = "userPointsOnFocus(this,'".$id."');";
  }
*/
  if ($readOnly) {
    $attributes['class'][] = 'addedit-readonly-textfield';
  }

  if (isset($tabIndex) && $tabIndex > 0) {
    $attributes['tabindex'] = $tabIndex;
  }

  // If the view width is larger than max allowed characters, make view width equal to the max allowed characters
  if ($viewWidthInChars > $maxChars) {
    $viewWidthInChars = $maxChars;
  }

  $prefix = '';
  $suffix = '';
  if (empty($form['addedit_tmp_storage']['two_column'])) {
    $prefix = addEditGetPrefix('FIELD', $title, $mandatory);
    $suffix = addEditGetSuffix('FIELD');
  }
  else {
    if ($form['addedit_tmp_storage']['two_column']['current_column'] == 0) {
      $prefix = addEditGetPrefix('TWOCOL_FIRSTCOL_FIELD', $title, $mandatory);
      $suffix = addEditGetSuffix('TWOCOL_FIRSTCOL_FIELD');
      $form['addedit_tmp_storage']['two_column']['current_column'] = 1; // increment current column
    } else if ($form['addedit_tmp_storage']['two_column']['current_column'] == 1) {
      $prefix = addEditGetPrefix('TWOCOL_SECONDCOL_FIELD', $title, $mandatory);
      $suffix = addEditGetSuffix('TWOCOL_SECONDCOL_FIELD');
      $form['addedit_tmp_storage']['two_column']['current_column'] = 2; // increment current column
    } else {
      throw new Exception('exp_sp_admin_userpoints.inc : addUserPointsTextFieldToAddEditForm() : Attempt to add a third field to a two column row.');
    }
  }

  // Create the form element
  $form[$id] = array(
    '#type' => 'textfield',
    '#prefix' => $prefix,
    '#suffix' => $suffix,
    '#title' => $title,
    '#title_display' => 'invisible',
    '#default_value' => $defaultValue,
    '#disabled' => $readOnly,
    '#attributes' => $attributes,
    '#size' => $viewWidthInChars,
    '#maxlength' => $maxChars,
    '#description' => $description
  );
  } catch (Exception $ex) {
  	watchdog_exception('addUserPointsTextFieldToAddEditForm', $ex);
  	expertusErrorThrow($ex);
  }
}

function fetchLeaderBoardUsersList($username)
{
	try{
	$flag =  block_role_permission_check('exp_sp_learning_spotlight','leaderboard');
	expDebug::dPrint('fetchLeaderBoardUsersList user access for block'.print_r($flag,true),4);
	 drupal_save_session(FALSE);
	 if($flag) {
	 	$rs=fetchLeaderBoardUsersListRS(rawurldecode($username));
	 	echo drupal_json_encode($rs);
	 }else {
	 	$rs='access denied';
	 	echo drupal_json_encode($rs);
	 }
	}catch(Exception $ex) {
	  	watchdog_exception('fetchLeaderBoardUsersList', $ex);
	  	expertusErrorThrow($ex);
  	}
}
/*
 * fetch_catalog_users_list() - function to fetch requested max count of online user's for a page in jqgrid
 */
function fetchLeaderBoardUsersListRS($username,$callfromAPI = 0) {
  
  try{
      global $theme_key;
      
      $sharemodule = getShareModuleStatus('profile');
	  $sord = 'ASC'; // Sort is always ascending by full_name
	
	  expDebug::dPrint(' $start = ' . print_r($start, true),4);
	  expDebug::dPrint('entered into fetchLeaderBoardUsersList'.print_r($theme_key,true),4);
	  //sub query to get top point holders from slt_user_points
	  $selectToppers = db_select('slt_user_points', 'points');
	  $selectToppers->addField('points', 'user_id', 'user_id');
	  $selectToppers->addExpression('SUM(points.total_points)', 'total_points');
	  
	  if($sharemodule==false){                                                            // Added this check for 79724
	  	$selectToppers->condition('points.action_code','sharing_class','!=');
	  }
	  // $selectToppers->where('operation_flag = \'insert\' AND total_points != 0');
	  
	  // $selectToppers->having('total_points > 0');
	  $selectToppers->groupBy('points.user_id');
	  // expDebug::dPrintDBAPI('select toppers from users points ', $selectToppers);
	  // Select sessions table
	  $select = db_select('slt_person', 'p');
	      
	  // $select->leftJoin('slt_user_points', 'points', 'p.id = points.user_id');
	  $select->addJoin('INNER', $selectToppers, 'userpoints', 'p.id = userpoints.user_id and userpoints.total_points != 0');
	  $select->leftJoin('users', 'u', 'p.user_name = u.name');
	  // $select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
	  $select->leftJoin('file_managed', 'pic', 'u.picture = pic.fid');
	  
	  // $select->addExpression('SUM(points.total_points)', 'totalpoints');
	  $select->addField('u', 'uid', 'uid');
	  $select->addField('p', 'id', 'personid');
	  $select->addField('p', 'full_name', 'full_name');
	  $select->addField('p', 'user_name', 'username');
	  $select->addField('pic', 'uri', 'uri');
	  $select->addField('userpoints', 'total_points', 'totalpoints');
	  filterByUserAccessPrivileges($select, 'p','',1,'cre_usr','','p');
	  // Set the conditions
	  $select->condition('u.status', 1, '=');
	  $select->condition('p.status', array('cre_usr_sts_atv'), 'IN');
	  $select->condition('p.user_name', array('admin', 'guest'), 'NOT IN');
	  // $select->groupBy('points.user_id');
	  $select->orderBy('totalpoints','DESC');
	  // $selectToppers->orderBy('total_points','DESC');
	  //Select a specific user
	  if($username != "null" && $username != t('LBL181')){
	  	$select->condition(db_or()
  	                     ->condition('p.user_name', '%' . db_like($username) . '%', 'LIKE')
  	                     ->condition('p.full_name', '%' . db_like($username) . '%', 'LIKE')
  	                  );
	  }else {
	    $select->having('totalpoints > 0');
	  }
	  
	  $countUsers = variable_get('exp_sp_admin_userpoints_block_max_list_count', 5);
	  
	  // Limit to $limit records
	  $select->range(0, $countUsers); 
	  
	  // Execure the query and fetch all the records
	  $UsersList = $select->execute()->fetchAll();
	  expDebug::dPrint(' $items = ' . print_r($UsersList, true),3);
	  if($callfromAPI){
	    return $UsersList;
	  }
	  expDebug::dPrint('rawurldecode($username) user name value:'.print_r($UsersList,true),5);
	  $i=0;
	  foreach ($UsersList as $leaderBoardUser) {
	    expDebug::dPrint(' $onlineUser = '. print_r($leaderBoardUser, true),3);
	    
	    $response->rows[$i]['id']=$leaderBoardUser->uid;
	    $response->rows[$i]['personid']=$leaderBoardUser->personid; 
	    $response->rows[$i]['username'] = $leaderBoardUser->username;
	    $response->rows[$i]['cell']['totalpoints'] = $leaderBoardUser->totalpoints;
	    $response->rows[$i]['cell']['badge_code'] = getBadgesCodeForLeaderBoard($leaderBoardUser->totalpoints);
	    $response->rows[$i]['cell']['id'] = $leaderBoardUser->personid;
	    $response->rows[$i]['cell']['personid'] = core_encrypt($leaderBoardUser->personid);
	    $response->rows[$i]['cell']['css'] = 'exp-user-leaderboard-rec';
	    
	    // Get screen_name from full_name
	  	$exp_ufn = explode(" ",$leaderBoardUser->full_name);
	 	if(count($exp_ufn) > 2){
	 	  $response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".mb_substr($exp_ufn[2], 0, 1);
	 	}
	 	else {
	 	  $response->rows[$i]['cell']['screen_name'] = $exp_ufn[0]." ".mb_substr($exp_ufn[1], 0, 1);
	 	}
	 	
	 	// Create the image url from uri
	 	if($theme_key == 'expertusoneV2'){ 
			 	if (!empty($leaderBoardUser->uri)) {
			      $response->rows[$i]['cell']['image_url'] = file_create_url($leaderBoardUser->uri);
			 	}
			 	else {
			      $response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/expertusonev2_default_user.png');
			    }
	 	} else{
	         if (!empty($leaderBoardUser->uri)) {
		      $response->rows[$i]['cell']['image_url'] = file_create_url($leaderBoardUser->uri);
		 	}
		 	else {
		      $response->rows[$i]['cell']['image_url'] = file_create_url('sites/default/files/pictures/default_user.png');
		    }
          }
	 	
	    $i++;
	  }
	  if (count($UsersList) > 0) {
	    $response->rows[0]['cell']['css'] .= ' exp-leaderboard-first-rec';
	    $response->rows[count($UsersList) - 1]['cell']['css'] .= ' exp-leaderboard-last-rec';
	  }
	  if(!empty($response))
		   return $response;
	  else 
	      return "null";
  }catch(Exception $ex) {	
	  watchdog_exception('fetchLeaderBoardUsersListRS', $ex);
	  expertusErrorThrow($ex);
  }
}

?>