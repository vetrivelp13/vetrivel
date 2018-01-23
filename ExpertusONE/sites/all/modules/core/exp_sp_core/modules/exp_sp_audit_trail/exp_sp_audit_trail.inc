<?php

/**
 * Audit Trail inc file
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 24-Feb-2012
 */


/*
 * exp_sp_audit_trail_add_entry() - Make an entry in slt_audit_trail table
 */
function exp_sp_audit_trail_add_entry($loggedUserId, $entityId, $entityType, $moduleName, $functionalityName, $loggedUserAction,
                                         $modifiedUserId = null, $oldValue = null, $newValue = null,
                                              $custom0 = null, $custom1 = null, $custom2 = null, $custom3 = null, $custom4 = null) {
	//expDebug::dPrint(' In add_audit_trail_entry()'.$entityType);
	
	$selectStmt = db_select('slt_profile_list_items', 'plie');
	$selectStmt->addField('plie','name', 'name');	
	$selectStmt->condition(db_or()
			->condition('plie.code', $entityType)
			->condition('plie.name', $entityType));	
	$result = $selectStmt->execute()->fetchField();	
	if($result == $entityType){
		$entityType = $entityType;
	}else{
		$entityType = $result;
	}
	

$txn = db_transaction();
try{
  	$EsignEntry = getConcatenatedUserDets('AuditTrail');
    // Select the table to insert the record in
    $insertStmt = db_insert('slt_audit_trail');
   
    // Select the fields and the values to be inserted
    $fields = array(
                    'logged_user_id' => $loggedUserId,
                    'entity_id' => $entityId,
                    'entity_type' => $entityType,
                    'module_name' => $moduleName,
                    'functionality_name' => $functionalityName,
                    'logged_user_action' => $loggedUserAction,
                    'mod_user_id' => $modifiedUserId,
                    'old_value'  =>  $oldValue,
                    'new_value' => $newValue,
    				'esign_date_time' => $EsignEntry->curr_date_time, 
    				'timezone' => $EsignEntry->time_zone_code, 
                    'created_on' => now(),
                    'custom0' => $custom0,
                    'custom1' => $custom1,
                    'custom2' => $custom2,
                    'custom3' => $custom3,
                    'custom4' => $custom4
    );
                   
    $insertStmt->fields($fields);
    
    expDebug::dPrintDBAPI(' $insertStmt obj = ' , $insertStmt);  
        
    // Execute the insert state$fieldsment. Fetch the new user's id in slt_person table. 
    $insertId = $insertStmt->execute();
    expDebug::dPrint(' $insertId = ' . print_r($insertId, true) , 3);
    return $insertId;
 }catch (Exception $ex) {
    $txn->rollback();
    unset($txn);
    watchdog_exception('exp_sp_audit_trail_add_entry', $ex);
    expertusErrorThrow($ex);
  }
  // Commit the transaction
  unset($txn);
}

?>