<?php

/*
 * addEditRepaintFormMulti() - 
 */


function catalog_promote_action($js = NULL, $entityId, $entityType){
  try {
  $js_module_optional = array('type' => 'file', 'group' => JS_DEFAULT);
  drupal_add_js(drupal_get_path('module', 'exp_sp_administration') .'/exp_sp_administration_addedit_behaviours.js', $js_module_optional);
 
  
  expDebug::dPrint('catalog_promote_action_details  $entityId : '.$entityId.'| $entityType :'.$entityType, 4);
  
  $info = array();
  
  $info['activity_id'] = $entityId;
  $info['entity_type'] = $entityType;
  
  $getPromoInfo = select_promote_entity($info);
  
  expDebug::dPrint('catalog_promote_action_details  $$getPromoInfo : '.print_r($getPromoInfo,true) , 4);
  
  if(!empty($getPromoInfo['promote_id'])) {
    $promote_status = $getPromoInfo['promote_status'];
    
    $updateInfo = array();
    $updateInfo['content_id']  = $getPromoInfo['promote_id'];
    $updateInfo['entity_id']   = $entityId;
    $updateInfo['entity_type'] = $entityType;
    $updateInfo['status']      = ($promote_status == 1) ? 0 : 1;
    //$updateInfo['status']      = 0 ;
    
    expDebug::dPrint('catalog_promote_action_details  $$$updateInfo : '.print_r($updateInfo,true),4);
    /*
    'content_id'        => $formInfo['content_id'],                  
                'activity_id'       => $formInfo['entity_id'],
                'entity_type'     	=> $formInfo['entity_type'],
    */
    update_promote($updateInfo);
  } else {
    $content_id = select_promote();
    $content_id = $content_id+1;
    $promoInfo = array();
    $promoInfo['content_id']  = $content_id;
    $promoInfo['entity_id']   = $entityId;
    $promoInfo['entity_type'] = $entityType;
    $promoInfo['status']      =  1;
    
    insert_promote($promoInfo);
  }
  
  
  ctools_include('ajax');

  
  
  
  $commands = array();
  $commands[] = ajax_command_refresh_narrow_search_results('immediate');
  //$commands[] = ajax_command_remove("tr.ajax-sample-row-$row");
  //$commands[] = ajax_command_restripe("table.ajax-sample-table");
  //return array('#type' => 'ajax', '#commands' => $commands);
  //return array('#type' => 'ajax', '#commands' => $commands);
  print ajax_render($commands);
  exit;
  } catch (Exception $ex) {
  	watchdog_exception('catalog_promote_action', $ex);
  	expertusErrorThrow($ex);
  }
}

function insert_promote($formInfo) {
 try {
  global $user;
  
  $userid  = $user->uid;  
  $txn     = db_transaction();
  
  /*
  insert into slt_promoted_highly_rated(
		content_id,
		activity_id,
		entity_type,
		status,
		created_by,
		createdon
		) 
	values(
		xcontentid,
		xactivityid,
		xentitytype,
		xstatus,
		xcreatedby,
		now()
		);
		*/

  try {
    $insertStmt = db_insert('slt_promoted_highly_rated');
    $fields     = array(
                'content_id'        => $formInfo['content_id'],                  
                'activity_id'       => $formInfo['entity_id'],
                'entity_type'     	=> $formInfo['entity_type'],
    						'status'     	      => '1',
                'created_by'        => $userid,
                'createdon'         => now(),  
    						'updated_by'        => $userid,
    						'updatedon'         => now(),
              );

    $insertStmt->fields($fields);
    $crsId 	  = $insertStmt->execute();
    
  } catch (Exception $ex) {
    
    $txn->rollback();
    watchdog_exception('CourseAdmin', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
  } catch (Exception $ex) {
  	watchdog_exception('insert_promote', $ex);
  	expertusErrorThrow($ex);
  }
}

function update_promote($formInfo) {
 try {
  global $user;
  
  $userid  = $user->uid;  
  $txn     = db_transaction();
  
  /*
  update slt_promoted_highly_rated 
	set content_id=xcontentid,
	    activity_id=xactivityid,
	    entity_type=xentitytype,
	    status=xstatus,
			updated_by=xupdatedby,
	    updatedon=now() 
	where entity_type=xentitytype and activity_id=xactivityid;
		*/

  try {
    $updateStmt = db_update('slt_promoted_highly_rated');
    $fields     = array(
                /*'content_id'        => $formInfo['content_id'],                  
                'activity_id'       => $formInfo['entity_id'],
                'entity_type'     	=> $formInfo['entity_type'],*/
    			'status'     	    => $formInfo['status'],
                'updated_by'        => $userid,
                'updatedon'         => now(),                          
              );

    $updateStmt = db_update('slt_promoted_highly_rated');  
    $updateStmt->fields($fields);
    $updateStmt->condition('content_id', $formInfo['content_id']);
    $updateStmt->condition('activity_id', $formInfo['entity_id']);
    $updateStmt->condition('entity_type', $formInfo['entity_type']);
    
    // Execute the update statement.
    $numUpdated = $updateStmt->execute();              
    
  } catch (Exception $ex) {
    
    $txn->rollback();
    watchdog_exception('CourseAdmin', $ex);
    throw $ex;
  }
  
  // Commit the transaction
  unset($txn);
  } catch (Exception $ex) {
  	watchdog_exception('update_promote', $ex);
  	expertusErrorThrow($ex);
  }
}


function select_promote_entity($info){
	try {
  $select = db_select("slt_promoted_highly_rated","promo");
  $select->addField('promo','content_id','content_id');
  $select->addField('promo','activity_id','activity_id');
  $select->addField('promo','entity_type','entity_type');      
  $select->addField('promo','status','status');
  $select->condition('promo.activity_id',$info["activity_id"],'=');
  $select->condition('promo.entity_type',$info["entity_type"],'=');	
  $crsList = $select->execute()->fetchAll();
  //$oNode->nid=isset($crsList[0]->nid)?$crsList[0]->nid:'';
  //$oNode->vid=isset($crsList[0]->vid)?$crsList[0]->vid:'';
  
  $promote_id     = !empty($crsList[0]->content_id) ? $crsList[0]->content_id : '';
  $promote_status = !empty($crsList[0]->status) ? $crsList[0]->status : '';
  
  $returnArr = array('promote_id' => $promote_id, 'promote_status' => $promote_status );
            
  return $returnArr;     
  } catch (Exception $ex) {
  	watchdog_exception('select_promote_entity', $ex);
  	expertusErrorThrow($ex);
  }
}


function select_promote(){
	try {
  $select = db_select("slt_promoted_highly_rated","promo");
  $select->addField('promo','content_id','content_id');
  $select->addField('promo','activity_id','activity_id');
  $select->addField('promo','entity_type','entity_type');      
  $select->addField('promo','status','status');
//  $select->condition('promo.activity_id',$info["activity_id"],'=');
//  $select->condition('promo.entity_type',$info["entity_type"],'=');	
  $select->orderBy('promo.content_id','desc');
  $crsList = $select->execute()->fetchAll();
  //$oNode->nid=isset($crsList[0]->nid)?$crsList[0]->nid:'';
  //$oNode->vid=isset($crsList[0]->vid)?$crsList[0]->vid:'';
  
  $promote_id     = !empty($crsList[0]->content_id) ? $crsList[0]->content_id :0;
  //$promote_status = !empty($crsList[0]->status) ? $crsList[0]->status : '';
  
  //$returnArr = array('promote_id' => $promote_id, 'promote_status' => $promote_status );
            
  return $promote_id;   
  } catch (Exception $ex) {
  	watchdog_exception('select_promote', $ex);
  	expertusErrorThrow($ex);
  }  
}






?>