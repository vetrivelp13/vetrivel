<?php 
require_once "data_load_validate.php";

class profile_list_item_dataload extends dataloadValidate{
	private $db;
	
	function __construct(){
		$this->db = new DLDatabase();
		$this->db->update_queue = true;
	}
	
	public function extenderValidate($jobDetail=array(),$batchrecords=array()){
		try{
	
		}catch(Exception $e){
	
		}
	
	}
	
	public function validate($jobDetail=array(),$batchrecords=array()){
		
		expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
		expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
		//Get parent id and list id
		$select = db_select("slt_profile_list_items",'pl');
		$select->addField('pl', 'lang_code');
		$select->addField('pl','id','parent_id');
		$select->addField('pl','list_id','list_id');
		$select->condition('code',$jobDetail['oType'],'=');
		expDebug::dPrintDBAPI(' $update ', $select);
		$profInfo = $select->execute()->fetchAll();
		
		foreach($batchrecords as $rec){
			try{
				$fields = array();
				$error = '';
				if(!empty($rec->code)){
					if (strpos($rec->code, ' ') !== FALSE){
						$error .= $jobDetail['entity'].' Code cannot contain space. ';}
					else if (strpos($rec->code, '?') !== FALSE){
						$error .= $jobDetail['entity'].' Code cannot contain question mark. ';}
					else if (strpos($rec->code, '-') !== FALSE){
						$error .= $jobDetail['entity'].' Code cannot contain hyphen. ';}
					else if (strpos($rec->code, '\'') !== FALSE){
						$error .= $jobDetail['entity'].' Code cannot contain single quote. ';}
					else if (strpos($rec->code, ';') !== FALSE){
						$error .= $jobDetail['entity'].' Code cannot contain semi colon or colon. ';}
					else {
						$fields['code'] = $jobDetail['oType'].'_'.$rec->code;}
				}
				if(!empty($rec->name)){
					$attr = db_query("select count(1) from ".$jobDetail['pTable']." where name = '".addslashes($rec->name)."'")->fetchField();
					if (strpos($rec->name, '+') !== FALSE)
						$error .= $jobDetail['entity'].' Name cannot contain +.';
					else if($attr > 1)
						$error .= 'Duplicate '.$jobDetail['entity'].' Name. ';
					else 
						$fields['name'] = $rec->name;
				}
				expDebug::dPrint(' $fields  '. print_r($fields,true),5);
				//expDebug::dPrint(' $error '. $error);
				if(empty($error)){
					if(!empty($rec->code)){
						$fields['lang_code']  = $profInfo[0]->lang_code;
						$fields['list_id']    =  $profInfo[0]->list_id;
						$fields['parent_id']  =  $profInfo[0]->parent_id;
						$updateStmt = db_update($jobDetail['pTable']);
						$updateStmt->fields($fields);
						$updateStmt->condition('code',$rec->code,'=');
						expDebug::dPrintDBAPI(' $updateStmt for profileList ', $updateStmt);
						$updateStmt->execute();
					}
				}else{
					$updateStmt = db_update($jobDetail['pTable']);
					$updateStmt->fields(array(
							'record_status' => 'IN'
					));
					$updateStmt->condition('code',$rec->code,'=');
					expDebug::dPrintDBAPI(' $updateStmt for error ', $updateStmt);
					$updateStmt->execute();
						
					db_query("update ".$jobDetail['sTable']." set record_status = 'IN', remarks = '".$error."' where mapping_id = (select mapping_id from ".$jobDetail['pTable']." where code = '".addslashes($rec->code)."')");
				}
				
			}catch(Exception $ex){
				$txn->rollback();
				unset($txn);
				throw new Exception("Error in batch validation".$ex->getMessage());
			}
		}
	} 
	
	public function bulkValidate($jobDetail=array(),$batchrecords=array()){
		try{
			
			expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
			expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
			
			// Validation for Code with quotes, characters like -?+:; and spaces.
			list($fields,$arg,$condition) = array(array(),array(),array());
			
			$fields=array(
					't2.custom4' => "concat(ifnull(t2.custom4,''), :err )",
					't2.record_status' => " :recst "
			);
			$arg = array(
					':err' => ' Code cannot contain quotes, characters like -?+:; and spaces. ', 
					':regex' => "[-?+\':;]",
					':recst' => 'IN',
					':btid' => $jobDetail['batchId']);
			$condition = array(
					"t2.batch_id = :btid ", 
					"t2.code !=''", 
					"t2.code IS NOT NULL", 
					"(t2.code REGEXP (:regex ) OR t2.code like '% %')");
			$upQry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition);
			expDebug::dPrintDBAPI(" Profile list code validation with quotes, characters like -?+:; and spaces",$upQry,$arg);
			$this->db->callExecute($upQry,$arg);
			 
			// Check special character '+' in profile list item name
			list($fields, $args,$condition) = array(array(),array(),array());
			$fields = array(
					"t2.custom4" => "concat(ifnull(t2.custom4,''), :msg )",
					"t2.record_status" => ":recst "
			);
			$condition = array(
					't2.batch_id = :batch ',
					't2.name like :reg '
			);
			$args = array(
					':recst' => 'IN',
					':msg' => $jobDetail['entity'].' name cannot contain +. ',
					':batch' => $jobDetail['batchId'],
					':reg' => "%+%"
			);
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition);
			expDebug::dPrintDBAPI("Spcial char + check in profile name ",$qry,$args);
			$this->db->callExecute($qry,$args);
			//if(!empty($batchrecords['is_active'])){
			// Status validation
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
				
			$fields = array(
					"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )"
			);
			$args = array(
					':msg' => 'Status can be only Active or Inactive. '
			);
			$join = array(
					" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id"
			);
			$condition = array(
					't22.batch_id = :batch ',
					't1.'.$batchrecords['is_active'].' not in ( :stat1 , :stat2 )'
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'Active';
			$args[':stat2'] = 'Inactive';
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Status validation ",$qry,$args);
			$this->db->callExecute($qry,$args);
				
			//Status Update
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
					"t22.is_active" => "CASE
					WHEN t1.".$batchrecords['is_active']." = :a THEN :stat1
					WHEN t1.".$batchrecords['is_active']." = :s THEN :stat2
					ELSE t22.is_active =''
					END"
			);
			$join = array(
					" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id"
			);
			$condition = array(
					't22.batch_id = :batch '
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'Y';
			$args[':stat2'] = 'N';
			$args[':a'] = 'Active';
			$args[':s'] = 'Inactive';
			
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Status Update  ",$qry,$args);
			$this->db->callExecute($qry,$args);
			//}
			// Updating lang_code,list_id and parent_id fields in temp table 2.
			list($fields,$arg,$condition,$join) = array(array(),array(),array(),array());
				
			$fields=array(
					't2.lang_code' => 'spli.lang_code',
					't2.list_id' => 'spli.list_id',
					't2.parent_id' => 'spli.id'
			);
			$arg = array(
					':code' => $jobDetail['oType'],
					':btid' => $jobDetail['batchId'],
					':r' => 'R');
			$condition = array(
					"t2.batch_id = :btid ",
					"t2.record_status = :r ");
			$join = array(" LEFT JOIN slt_profile_list_items spli on spli.code = :code ");
			$upQry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Updating lang_code,list_id and parent_id fields",$upQry,$arg);
			$this->db->callExecute($upQry,$arg);
			
			
		
			// Updating remarks and record_status fields in temp table 1 and Code,custom4 as null in temp table 2.
			list($fields,$arg,$condition,$join) = array(array(),array(),array(),array());
			
			$fields=array(
					't2.code' => "CONCAT( :type ,'_',t2.code)",
					't1.remarks' => "concat(ifnull(t1.remarks,''),t2.custom4)",
					't1.record_status' => 't2.record_status',
					't2.custom4' => "null"
			);
			$arg = array(':btid' => $jobDetail['batchId'],':type'=> $jobDetail['oType']);
			$condition = array(
					"t2.batch_id = :btid ",
					"t1.mapping_id = t2.mapping_id");
			$join = array(" LEFT JOIN ".$jobDetail['pTable']." t2 on t2.mapping_id =t1.mapping_id");
			$upQry = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Profile list code remarks and record_status fields updation",$upQry,$arg);
			$this->db->callExecute($upQry,$arg);
			
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Organization Validation ".$e->getMessage(),1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $ex){
			expDebug::dPrint("Exception Error in Organization Validation ".$ex->getMessage(),1);
		}
	}
	
	public function execute($updVal){
		/** TODO: Need User Id to update
		 * Using 1 as user id for checking */
		$i = 0;
		$j = 0;
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/common.inc';
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/database/database.inc';
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		$form=array();
		$form["form_id"]="";
		include_once($_SERVER['DOCUMENT_ROOT'].'/'.drupal_get_path('module', 'exp_sp_administration_setup') .'/exp_sp_administration_setup.inc');
		try{
			$form_state['values']['custom_dataload'] = $updVal->batch_id;
			if($updVal->operation =='insert' || $updVal->operation == 'upsert'){
				$form_state['values']['name'] = $updVal->name;
				$form_state['list_code'] = $updVal->code;
				$form_state['list_id'] = $updVal->list_id;
				$form_state['parent_id'] = $updVal->parent_id;
				$form_state['is_active'] = $updVal->is_active;
				$listId = addNewListValues($form,$form_state,'Feed','','DAPI');
			}else{
				expDebug::dPrint('updateListValueDetailsByDataLoadAPI'.print_r($updVal,true));
				$form_state['list_id'] = $updVal->primary_id;
				$form_state['values']['name'] = $updVal->name;
				$form_state['list_code'] = $updVal->code;
				$form_state['is_active'] = $updVal->is_active;
				$listId = updateListValues($form,$form_state,'Feed','','DAPI');
			}
			return $listId; 
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Profile list Execution API ".$e->getMessage(),1);
			throw new PDOException($ex->getMessage());
		}catch(Execption $e){
			throw new Exception("Error in batch Execution".$e->getMessage());
		}
	}
	
	public function bulkExecute($tblDet=array()){
		try{
			if($tblDet['oper'] == 'insert' || $tblDet['oper'] == 'upsert'){
				/* TODO : TO BE REMOVED
				 * $profFields = array('code', 'lang_code', 'name', 'list_id', 'parent_id', 'is_active');
					
				$profQry = db_select($tblDet['tblname'], 'prof');
				$profQry->fields('prof',$profFields);
				$profQry->addExpression('now()','created_on');
				$profQry->addExpression(1,'created_by');
				$profQry->addExpression('now()','updated_on');
				$profQry->addExpression(1,'updated_by');
				$profQry->condition('prof.batch_id', $tblDet['btc_id'],'=');
				$profQry->condition('prof.record_status', '','IS NULL');
				$profQry->condition(db_or()->condition('prof.operation', $tblDet['oper'],'=')
																	 ->condition('prof.operation', 'upsert','='));
				expDebug::dPrintDBAPI('profile list items query', $persQry);
					
				db_insert('slt_profile_list_items')
				->from($profQry)
				->execute();
				 */
				
				//Fields to insert.
				$fields = array();
				$selFields = array();
				$createdBy = array('lang_code', 'list_id', 'parent_id','created_on','created_by','updated_on','updated_by','custom_dataload');
				$createdByArgs = array('prof.lang_code', 'prof.list_id', 'prof.parent_id','now()', ':by ','now()', ':by ',':cust ');
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col '));
				
				$this->db->callQuery($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%'));
				
				$columnDetails = $this->db->fetchAllResults();
				foreach($columnDetails as $col){
					$fields[] = $col->base_column;
					$selFields[] = 'prof.'.$col->base_column;
				}
				
				$tbl = $tblDet['tblname'];
				$insert = array();
				$insert['table'] = 'slt_profile_list_items';
				$insert['fields'] = array_merge($fields,$createdBy); 
				/* array('code', 'lang_code', 'name', 'list_id', 'parent_id', 'is_active','created_on','created_by','updated_on','updated_by'); */
				$select = array();
				$select['table'] = "$tbl prof";
				$select['fields']= array_merge($selFields,$createdByArgs); 
				/* array('prof.code', 'prof.lang_code', 'prof.name', 'prof.list_id', 'prof.parent_id', 'prof.is_active',':on ', ':by ',':on ', ':by '); */
				
				$select['condition'] = array(
						'prof.batch_id = :btid ',
						'prof.record_status = :r ',
						'(prof.operation = :opr OR prof.operation = :oprsts )'
				);
				$arg=array(
						':btid' => $tblDet['btc_id'],
						':opr' => $tblDet['oper'],
						':oprsts' => 'upsert',
						':r' => 'R',
						':by'=>1,
						':cust' =>$tblDet['btc_id']
				);
				
				$query = $this->db->prepareInsertBySelect($insert,$select);
				$this->db->callExecute($query,$arg);
				
				
			}else{
				/* TODO : TO BE REMOVED
				 * $profQry = "update slt_profile_list_items prof,
				(select code, lang_code, name, list_id,parent_id,is_active
				from ".$tblDet['tblname']."
				where batch_id =".$tblDet['btc_id']." and operation='update')x
				set prof.code = x.code,prof.lang_code = x.lang_code, prof.name = x.name,prof.list_id = x.list_id
				,prof.parent_id = x.parent_id,prof.list_id = x.list_id,updated_on=now(),updated_by=1
				where prof.code=x.code and x.batch_id=".$tblDet['btc_id']." and record_status IS NULL and operation='update'";
				expDebug::dPrint('profile list bulk update '.$profQry);
				db_query($profQry); */
				
				
				//Field to be updated
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col ','map.key_column != :unq '));
				
				$this->db->callQuery($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%',':unq'=>'unique'));
				
				$columnDetails = $this->db->fetchAllResults();
				foreach($columnDetails as $col){
					$fields['prof.'.$col->base_column] = 'x.'.$col->base_column;
				}
				$fields['prof.updated_on'] ='now()';
				$fields['prof.updated_by']= ':uid ';
				$fields['prof.custom_dataload']= ':cust ';
				
				$condition = array('prof.code=x.code','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ');
				$join = array(" left join ".$tblDet['tblname']." x on x.code = prof.code");
				$args = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':uid'=>1,':cust'=>$tblDet['btc_id'],':r'=>'R');
				$profQry = $this->db->prepareQueryUpdate('slt_profile_list_items prof ',$fields,$condition,$join);
				
				expDebug::dPrintDBAPI('profile list bulk update ',$profQry,$args);
				$this->db->callExecute($profQry,$args);
				
				
				/* TODO : TO BE REMOVED 
				 * $profQry = "update slt_profile_list_items prof
				left join ".$tblDet['tblname']." x on x.batch_id = :btid and x.operation= :oper
				set prof.code = x.code,prof.lang_code = x.lang_code, prof.name = x.name,prof.list_id = x.list_id
				,prof.parent_id = x.parent_id,prof.list_id = x.list_id,updated_on=now(),updated_by=1
				where prof.code=x.code and x.batch_id=".$tblDet['btc_id']." and record_status IS NULL and operation='update'";
				expDebug::dPrint('profile list bulk update '.$profQry);
				$this->db->callExecute($profQry,array(':btid'=>$tblDet['btc_id'],':oper'=>'update')); */
				
			}
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Profile list Execution ".$e->getMessage(),1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			expDebug::dPrint('Error in the batch execution for bulk upload'.$e->getMessage(),1);
		}

	}
}

?>