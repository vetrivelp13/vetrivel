<?php
/**
 * Fetches a user information by user id.
 *
 * @param $user_id
 *   The ID of a user.
 *
 * @return
 *   A fully-loaded user object if a user is exist.
 *   otherwise.
 * 
 */
function getUserProfile($drupal_user_id){
  try{
  // Select drupal users table
  $select =  db_select('users', 'u');

  // Left join the file_managed table to get the picture name and picture uri
  $select->leftjoin('file_managed','fm', 'u.picture = fm.fid');
  
  // Left join the slt_person table
  $select->leftJoin('slt_person','p','u.name = p.user_name');

  // Left join the slt_profile_list_items, slt_person table (to get manager's name), slt_organization to slt_person table
  $select->leftJoin('slt_profile_list_items', 'pli', ' p.time_zone = pli.code');
  $select->leftJoin('slt_person', 'mgr', 'p.manager_id = mgr.id');
  $select->leftJoin('slt_organization','org', 'p.org_id = org.id');
  $select->leftJoin('slt_location','loc', 'p.preferred_loc_id = loc.id');

  // Fetch these fields from the users table
  $select->addField('u','uid');

  // Fetch these fields from the slt_person table
  $select->addField('p','id');
  $select->addField('p','full_name');
  $select->addField('p','user_name');
  $select->addField('loc','name','location_name');
  $select->addField('p','email');
  
  // Fetch these fields from the slt_organization table
  $select->addField('org','name');
  
  // Fetch these fields from the slt_profile_list_items table
  $select->addField('pli','name','timezonename');
  
  // Fetch these fields from the file_managed table
  $select->addField('fm','uri');
  
  // Set the condition
  $select->condition('u.uid', $drupal_user_id);
  expDebug::dPrintDBAPI(' $select obj = ' , $select);
  
  // Execute the query and fetch the result
  $result = $select->execute()->fetchObject();
  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  
  if(!empty($result->uri)) {
    $result->filename = file_create_url($result->uri);     
  }
  else {
    $result->filename = file_create_url('sites/default/files/pictures/default_user.png');
  }
  unset($result->uri); // Remove uri from the result as not needed anymore.

  expDebug::dPrint(' $result = ' . print_r($result, true) , 3);
  return $result;
  }catch (Exception $ex) {
    watchdog_exception('getUserProfile', $ex);
    expertusErrorThrow($ex);
  } 
}

function getUserJobtitle($user_id) {
  try{
  $select = db_select('users', 'u');
  // Left join the slt_person table.
  $select->leftJoin('slt_person', 'p', 'u.name= p.user_name');
  // Left join slt_profile_list_items table to get job title name
  $select->leftJoin('slt_profile_list_items', 'pli', 'p.job_title = pli.code');
  // Select fields/expression values to be fetched from the database.
  $select->addField('pli', 'name', 'job_title');
  $select->condition('u.uid', $user_id);
  $result = $select->execute()->fetchObject();
  return $result->job_title;
  }catch (Exception $ex) {
    watchdog_exception('getUserJobtitle', $ex);
    expertusErrorThrow($ex);
  } 
  
}

?>