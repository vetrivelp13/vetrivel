<?php 
require_once "data_load_validate.php";
include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";

class organization_dataload extends dataloadValidate{
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
		try{
		//	expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
		//	expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
			
			foreach($batchrecords as $rec){
				try{
				
					$fields = array();
					$args = array();
					$error = '';
					if(!empty($rec->name)){
						if (strpos($rec->name, '+') !== FALSE)
							$error .= 'Organization name cannot contain +. ';
						if($jobDetail['batchopt'] == 'insert'){
							//$org = db_query("select count(1) from ".$jobDetail['pTable']." where name = '".addslashes($rec->name)."'")->fetchField();
							$qry = "select count(1) from ".$jobDetail['pTable']." where name = :recname ";
							$this->db->callQuery($qry,array(':recname'=>addslashes($rec->name)));
							$org = $this->db->fetchColumn();
							$error .= ($org > 1) ? 'Duplicate Organization Name. ' : '';
						}
					}
					if($rec->status != 'Activate' && $rec->status != 'Suspended' && $rec->status != 'Deleted'){
						$error .= 'Organization status can be only Activate or Suspended or Deleted. ';
					}else{
						$fields['status'] = ":status ";
						$args[':status']= ($rec->status == 'Activate')? 'cre_org_sts_act' : (($rec->status == 'Suspended') ? 'cre_org_sts_itv' : 'cre_org_sts_del'); 
					}
					if($rec->type != 'Internal' && $rec->type != 'External'){
						$error .= 'Organization status can be only Internal or External. ';
					}else{
						$fields['type'] = ':type ';
						$args[':type'] = ($rec->type == 'Internal')? 'cre_org_typ_int' : 'cre_org_typ_ext';
					}
					if(!empty($rec->parent_org_id)){
						if($rec->name == $rec->parent_org_id){
							$error .= 'An Organization cannot be a parent for itself. ';
						}else{
							//$orgId = db_query("select if(status = 'cre_org_sts_act',id,'inactive') as id from slt_organization where name = '".addslashes($rec->parent_org_id)."' Limit 1")->fetchField();//check org already exists in organization.
							$qry = "select if(status = :status ,id,:inact) as id from slt_organization where name = :name Limit 1"; //check org already exists in organization.
							$this->db->callQuery($qry,array(':status'=>'cre_org_sts_act', ':inact'=>'inactive', ':name'=>addslashes($rec->parent_org_id)));
							$orgId = $this->db->fetchColumn();
							if(empty($orgId)){
								//$orgId = db_query("select if(record_status='IN','invalid',mapping_id) from ".$jobDetail['pTable']." where name = '".addslashes($rec->parent_org_id)."' Limit 1")->fetchField();
								$qry = "select if(record_status= :op ,:inval,mapping_id) from ".$jobDetail['pTable']." where name = :name Limit 1"; 
								$this->db->callQuery($qry,array(':op'=>'IN', ':inval'=>'invalid', ':name'=>addslashes($rec->parent_org_id)));
								$orgId = $this->db->fetchColumn();
								if(empty($orgId))
									$error .= 'Given Parent Organization does not exist in both LMS and feed. ';
								else if($orgId == 'invalid'){
									$error .= 'Given Parent Organization does not exist in LMS,  exist in feed, but is not a valid Organization (check error details of the Organization for more details). ';
								}else{
									$fields['operation'] = ':ops';
									$args[':ops'] = 'upsert';
								}
							}else if($orgId == 'inactive'){
								$error .= 'Given Parent Organization exist in LMS but is not active. ';
							}else{
									$fields['parent_org_id'] = ":porg";
									$args[':porg'] = $orgId;
							}
						}
					}
					if(empty($error)){
						if(!empty($rec->number)){
							$query = $this->db->prepareQueryUpdate($jobDetail['pTable'],$fields,array('number = :num'));
							$args[':num'] = $rec->number;
							expDebug::dPrintDBAPI(' $updateStmt for organization ',$query,$args);
							$this->db->callExecute($query,$args);
						}
					}else{
						
						$query = $this->db->prepareQueryUpdate($jobDetail['pTable'],array('record_status' => ':recst'),array('number = :num'));
						$args = array(':recst'=>'IN',':num'=>$rec->number);
						expDebug::dPrintDBAPI(' $updateStmt for organization ',$query,$args);
						$this->db->callExecute($query,$args);
					
						//db_query("update ".$jobDetail['sTable']." set record_status = 'IN', remarks = '".$error."' where mapping_id = (select mapping_id from ".$jobDetail['pTable']." where number = '".addslashes($rec->number)."')");
						$fields = array(
							'record_status' => ':recst',
							'remarks' => ':remark'
						);
						$join = array(
							'left join '.$jobDetail['pTable'].' t2 on t1.mapping_id = t2.mapping_id'
						);
						$condition = array(
							't2.number' => ':recno'
						);
						$args = array(
							':recst' => 'IN',
							':remark' => $error,
							':recno' => addslashes($rec->number)
						);
						$query = $this->db->prepareQueryUpdate($jobDetail['sTable']." t1",$fields,$condition,$join);
						expDebug::dPrintDBAPI(' $updateStmt for organization ',$query,$args);
						$this->db->callExecute($query,$args);
					}
				}catch(Exception $e){
					// TODO update record as error/invalid and continue
				}
			}
			
		}catch(Exception $e){
			// TODO: throw exception 
		}
	} 
	
	public function bulkValidate($jobDetail=array(),$batchrecords=array()){
		try{
			$util = new GlobalUtil();
			$config = $util->getConfig();
			$configInvalid    = $config["skip_invalid_record"];
			
			expdebug::dPrint(' Job Details bulk '.print_r($jobDetail,true),5);
			expdebug::dPrint(' batchrecords bulk '.print_r($batchrecords,true),5);
			expdebug::dPrint(' $configInvalid '. $configInvalid);
			
			if($jobDetail['batchopt'] == 'update'){
				list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
				$fields=array(
						't2.primary_id' => 'org.id'
				);
				$condition=array(
						't2.batch_id = :batch ',
						't2.operation = :op ',
						't2.primary_id is null'
				);
				$join=array(
						'left join slt_organization org on org.number = t2.number'
				);
				$args=array(
						':batch' => $jobDetail['batchId'],
						':op' => 'update'
				);
			
				$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
				expDebug::dPrintDBAPI("Update primary id".$qry,$args);
				$this->db->callExecute($qry,$args);
			
			}
			
			// Check special character '+' in organization name
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => ":recst "
			);
			$condition = array(
				't22.batch_id = :batch ',
				't22.name like :org '
			);
			$args = array(
					':recst' => 'IN',
					':msg' => 'Organization name cannot contain +. ',
					':batch' => $jobDetail['batchId'],
					':org' => "%+%"
			);
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Spcial char + check in org name ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			
			// Duplicate name check
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => ":recst "
			);
			$join = array(
				" right join slt_organization org on t22.name = org.name" 
			);
			$condition = array(
				't22.batch_id = :batch ',
				't22.number != ifnull(org.number,:nul )'
			);
			$args = array(
					':nul'  => '',
					':recst' => 'IN',
					':msg' => 'Duplicate Organization Name. ',
					':batch' => $jobDetail['batchId']
			);

			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Duplicate name check in org name ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			// Status validation
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => ":recst "
			);
			$args = array(
				':recst' => 'IN',
				':msg' => 'Organization status can be only Activate or Suspended or Deleted. '
			);
			$join = array(
				" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id" 
			);
			$condition = array(
				't22.batch_id = :batch ',
				't1.'.$batchrecords['status'].' not in ( :stat1 , :stat2 , :stat3 )'
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'Activate';
			$args[':stat2'] = 'Suspended';
			$args[':stat3'] = 'Deleted';
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Status validation ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Status Update
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
				
			$fields = array(
					"t22.status" => "CASE
					WHEN t1.".$batchrecords['status']." = :a THEN :stat1
					WHEN t1.".$batchrecords['status']." = :s THEN :stat2
					WHEN t1.".$batchrecords['status']." = :d THEN :stat3
					ELSE t22.status =''
					END"
			);
			$join = array(
					" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id"
			);
			$condition = array(
					't22.batch_id = :batch '
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'cre_org_sts_act';
			$args[':stat2'] = 'cre_org_sts_itv';
			$args[':stat3'] = 'cre_org_sts_del';
			$args[':a'] = 'Activate';
			$args[':s'] = 'Suspended';
			$args[':d'] = 'Deleted';
				
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Status Update  ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			
			// Organization type check
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => ":recst "
			);
			$args = array(
				':recst' => 'IN',
				':msg' => 'Organization type can be only Internal or External. '
			);
			$join = array(
				" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id" 
			);
			$condition = array(
				't22.batch_id = :batch ',
				't1.'.$batchrecords['type'].' not in ( :stat1 , :stat2 )'
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'Internal';
			$args[':stat2'] = 'External';
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Organization type check ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Type Update
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
					"t22.type" => "CASE
					WHEN t1.".$batchrecords['type']." = :int THEN :stat1
					WHEN t1.".$batchrecords['type']." = :ext THEN :stat2
					ELSE t22.type =''
					END"
			);
			$join = array(
					" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id"
			);
			$condition = array(
					't22.batch_id = :batch '
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':stat1'] = 'cre_org_typ_int';
			$args[':stat2'] = 'cre_org_typ_ext';
			$args[':int'] = 'Internal';
			$args[':ext'] = 'External';
			
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Type Update  ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			// Parant org name validation

			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => "if(".$configInvalid." = 1,:recst , t22.record_status)"
			);
			$args = array(
				':msg' => 'An Organization cannot be a parent for itself. ',
				':recst' => 'IN'
			);
			$join = array(
				" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id" 
			);
			$condition = array(
				't22.batch_id = :batch ',
				't1.'.$batchrecords['parent_org_id'].' = t1.'.$batchrecords['name']
			);
			$args[':batch'] = $jobDetail['batchId'];

			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Parant org name validation ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Parent Organization does not exists in LMS and Feed
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
				"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
				"t22.record_status" => "if(".$configInvalid." = 1,:recst , t22.record_status)"
			);
			$args = array(
				':msg' => 'Given Parent Organization does not exist in both LMS or feed. ',
				':recst' => 'IN'
			);
			$join = array(
				" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id",
				" left join ".$jobDetail['sTable']." t11 on t1.".$batchrecords['parent_org_id']." = t11.".$batchrecords['name'],
				'left join slt_organization org on org.name = t1.'.$batchrecords['parent_org_id']
			);
			$condition = array(
				't22.batch_id = :batch ',
				't1.'.$batchrecords['parent_org_id'].' != :nul ',
				't1.'.$batchrecords['parent_org_id'].' IS NOT NULL',
				't11.'.$batchrecords['name'].' IS NULL',
				'org.name IS NULL'
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':nul']  = '';
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Parant org exists validation ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			// Parent Organization exists in LMS or feed but not active
			
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
				
			$fields = array(
					"t22.custom4" => "concat(ifnull(t22.custom4,''), :msg )",
					"t22.record_status" => "if(".$configInvalid." = 1,:recst , t22.record_status)"
			);
			$args = array(
					':recst' => 'IN',
					':msg' => 'Given Parent Organization exist in LMS or Feed but is not active or not a valid Organization (check error details of the Organization for more details) . '
			);
			$join = array(
				" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id",
				" left join ".$jobDetail['sTable']." t11 on t1.".$batchrecords['parent_org_id']." = t11.".$batchrecords['name'],
				" left join ".$jobDetail['pTable']." t2 on t1.".$batchrecords['parent_org_id']." = t2.name",
				" left join slt_organization org on org.name = t1.".$batchrecords['parent_org_id']
			);
			$condition = array(
					't22.batch_id = :batch ',
					't1.'.$batchrecords['parent_org_id'].' != :nul ',
					't1.'.$batchrecords['parent_org_id'].' IS NOT NULL',
					'(if(t11.'.$batchrecords['name'].' is not null, t11.'.$batchrecords['status'].' != :actv , org.name is not null and org.status != :atv )
							or (t2.record_status = :recst or t11.record_status = :recst ))'
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':atv'] = 'cre_org_sts_act';
			$args[':actv'] = 'Activate';
			$args[':nul']  = '';
			$args[':recst'] = 'IN';
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Parent Organization exists in LMS not active ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			
		//Parent id update
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			
			$fields = array(
					"t22.parent_org_id" => "org.id"
			);
			$join = array(
					" left join ".$jobDetail['sTable']." t1 on t22.mapping_id = t1.mapping_id",
					" left join slt_organization org on t1.".$batchrecords['parent_org_id']." = org.name"
			);
			$condition = array(
					't22.batch_id = :batch ',
					'org.status = :status '
			);
			$args[':batch'] = $jobDetail['batchId'];
			$args[':status'] = 'cre_org_sts_act';
			
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Parent id update ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Upsert record Update
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.operation' => " :op "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['parent_org_id'].' != :nul ',
			 		't1.'.$batchrecords['parent_org_id'].' IS NOT NULL',
			 		'(t11.record_status = :r or t11.record_status != :recst )',
			 		't11.'.$batchrecords['status'].' = :atv ',
			 		'(t22.record_status = :r or t22.record_status != :recst ) ',
			 		'org.id is null',
			 		't11.'.$batchrecords['name'].' is not null'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_organization org on org.name = t1.'.$batchrecords['parent_org_id'],
			 		'left join '.$jobDetail['sTable'].' t11 on t1.'.$batchrecords['parent_org_id'].' = t11.'.$batchrecords['name'],
			 		'left join '.$jobDetail['pTable'].' t2 on t1.'.$batchrecords['parent_org_id'].' = t2.name'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':op' => 'upsert',
			 		':atv' => 'Activate',
			 		':recst' => 'IN',
			 		':r' => 'R'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Parent organization id update qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			 //Update temp table 1
			 list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
					't11.remarks' => 'concat(ifnull(t11.remarks,""),t2.custom4)',
					't11.record_status' => 't2.record_status'
				);
			 $condition=array(
			 		't2.batch_id = :batch_id '
			 );
			 $join=array(
			 		'left join '.$jobDetail['pTable'].' t2 on t2.mapping_id =t11.mapping_id',
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId']
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t11",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Source table qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
				 
			$qry = "update ".$jobDetail['pTable']." set custom4 = null where batch_id = ".$jobDetail['batchId'];
			$this->db->callExecute($qry);
		
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
		include_once($_SERVER['DOCUMENT_ROOT'].'/'.drupal_get_path('module', 'exp_sp_administration_organization') .'/exp_sp_administration_organization.inc');
		try{
			$form_state['values']['name'] = $updVal->name;
			$form_state['values']['number'] = $updVal->number;
			$form_state['values']['type'] = $updVal->type;
			$form_state['values']['status'] = $updVal->status;
			$form_state['values']['description']['value'] = $updVal->description;
			$form_state['values']['contact'] = $updVal->contact_name;
			$form_state['values']['cost_center'] = $updVal->cost_center;
			$form_state['storage']['acvalues']['parentorg']['id'] = $updVal->parent_org_id;
			$form_state['values']['custom_dataload'] = $updVal->batch_id;
			
			if($updVal->operation =='insert' || $updVal->operation == 'upsert'){
				$orgId = addNewOrganization($form,$form_state,'Feed','','DAPI');
			}else{
				$form_state['values']['id'] = $updVal->primary_id;
				$orgId = updateOrganizationDetails($form,$form_state,'Feed','','DAPI');
				if(($orgId[0]->id) > 0){
					activateOrDeactivateOrg($orgId[0]->id,$form_state['values']['status'],false,1);
				}
			}
			return $orgId;
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Organization Execution in API ".$e->getMessage(),1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			throw new Exception("Error in batch Execution".$e->getMessage());
		}
	}
	
	public function bulkExecute($tblDet=array()){
		$this->db->connect();
		$txn = $this->db->beginTrans();
		try{
			$orgVocabularyNumber = $this->getOrgVocabularyNumber();
			
			if($tblDet['oper'] == 'insert' || $tblDet['oper'] == 'upsert'){
				
				//Fields to insert.
				$fields = array();
				$selFields = array();
				$createdBy = array('created_on','created_by','updated_on','updated_by','custom_dataload');
				$createdByArgs = array('now()', ':by ','now()', ':by ',':cust ');
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col '));
				
				expDebug::dPrintDBAPI("Get Org mapping columns -- ", $query, array(':entid' => $tblDet['ent_id'],':col'=>'%custom%'));
				$this->db->query($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%'));
				
				$columnDetails = $this->db->fetchAllResults();
				foreach($columnDetails as $col){
					$fields[] = $col->base_column;
					$selFields[] = 'org.'.$col->base_column;
				}
				
				
				$tbl = $tblDet['tblname'];
				$insert = array();
				$insert['table'] = 'slt_organization';
				$insert['fields'] = array_merge($fields,$createdBy);
				$select = array();
				$select['table'] = "$tbl org";
				$select['fields']= array_merge($selFields,$createdByArgs);
				
				$select['condition'] = array(
						'org.batch_id = :btid ',
						'org.record_status = :r ',
						'(org.operation = :opr OR org.operation = :oprsts )'
				);
				$arg=array(
						':btid' => $tblDet['btc_id'],
						':opr' => $tblDet['oper'],
						':oprsts' => 'upsert',
						':r' => 'R',
						':on' => 'now',
						':by'=>'Feed',
						':cust'=>$tblDet['btc_id']
				);
				
				$query = $this->db->prepareInsertBySelect($insert,$select);
				expDebug::dPrintDBAPI("Organization Insert query -- ",$query,$arg);
				$this->db->execute($query,$arg);
				
				$tempTbl = $tblDet['tblname'];
				$insert = array();
				$insert['table'] = 'taxonomy_term_data';
				$insert['fields'] = array('vid', 'name', 'description');
				$select = array();
				$select['table'] = "$tempTbl term_data";
				$select['fields']= array(':vid', ':nme ', 'org.id');
				$select['joins'] = array(
						' left join slt_organization org on org.number=term_data.number',
				);
				$select['condition'] = array(
						'term_data.batch_id = :btid ',
						'term_data.record_status = :r ',
						'(term_data.operation = :opr OR term_data.operation = :oprsts )'
				);
				$arg1=array(
						':vid'=> $orgVocabularyNumber,
						':nme' => "if(term_data.status='cre_org_sts_act',CONCAT(term_data.name,'-'),CONCAT(term_data.name,'-',' (InActive)'))",
						':btid' => $tblDet['btc_id'],
						':opr' => $tblDet['oper'],
						':oprsts' => 'upsert',
						':r' => 'R'
				);
				
				$query1 = $this->db->prepareInsertBySelect($insert,$select);
				expDebug::dPrintDBAPI("taxonomy_term_data Insert query -- ",$query1,$arg1);
				$this->db->execute($query1,$arg1);
				
				$tempTbl = $tblDet['tblname'];
				$insert = array();
				$insert['table'] = 'taxonomy_term_hierarchy';
				$insert['fields'] = array('tid');
				$select = array();
				$select['table'] = "taxonomy_term_data ttd";
				$select['fields']= array('ttd.tid');
				$select['joins'] = array(
						' left join slt_organization org on org.id=ttd.description and ttd.description is not null',
						' left join '.$tempTbl.' term_data on org.number=term_data.number'
				);
				$select['condition'] = array(
						'term_data.batch_id = :btid ',
						'ttd.vid = :vid ',
						'term_data.record_status = :r ',
						'(term_data.operation = :opr OR term_data.operation = :oprsts )'
				);
				$th_arg=array(
						':vid'=> $orgVocabularyNumber,
						':btid' => $tblDet['btc_id'],
						':opr' => $tblDet['oper'],
						':oprsts' => 'upsert',
						':r' => 'R'
				);
				
				$th_query = $this->db->prepareInsertBySelect($insert,$select);
				expDebug::dPrintDBAPI("taxonomy_term_hierarchy Insert query -- ",$th_query,$th_arg);
				$this->db->execute($th_query,$th_arg);
				
			}else{
				
				//Field to be updated
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col ','map.key_column != :unq '));
				
				$this->db->query($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%',':unq'=>'unique'));
				
				$columnDetails = $this->db->fetchAllResults();
				foreach($columnDetails as $col){
					$fields['org.'.$col->base_column] = 'x.'.$col->base_column;
				}
				$fields['org.updated_on'] ='now()';
				$fields['org.updated_by']= ':uid ';
				$fields['org.custom_dataload'] = ':cust ';
				
				$condition = array('org.number=x.number','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ');
				$join = array(" left join ".$tblDet['tblname']." x on x.number=org.number");
				$args = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':uid'=>1,':cust'=>$tblDet['btc_id'],':r'=>'R');
				$orgQry = $this->db->prepareQueryUpdate('slt_organization org ',$fields,$condition,$join);
				
				expDebug::dPrintDBAPI('Organization bulk update ',$orgQry,$args);
				$this->db->execute($orgQry,$args);
				
				$field = array('term_data.name' => "if(x.status= :sts ,CONCAT(x.name,'-'),CONCAT(x.name,'-', :inact ))" ,'term_data.weight' => "if(x.status= :sts ,0,1)");
				$condition = array('term_data.vid= :vid ','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ','term_data.description=org.id');
				$join = array(" left join ".$tblDet['tblname']." x on x.primary_id = term_data.description"," left join slt_organization org on org.number=x.number");
				$args = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':vid' => $orgVocabularyNumber,':sts' => 'cre_org_sts_act',':inact' => '  (InActive)',':r' => 'R');
				$ttdQry = $this->db->prepareQueryUpdate('taxonomy_term_data term_data ',$field,$condition,$join);
				
				expDebug::dPrintDBAPI('Taxonomy term data bulk update ',$ttdQry,$args);
				$this->db->execute($ttdQry,$args);
				
			}
			
			$this->db->commitTrans();
			$this->db->closeconnect();
		}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in Executing the batch records for organization",1);
			$this->db->rollbackTrans();
			$this->db->closeconnect();
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			$txn->rollback();
			expDebug::dPrint('Error in the batch execution for bulk upload'.$e->getMessage(),1);
		}

	}
	
	private function getOrgVocabularyNumber(){
		$qry = "SELECT ifnull(value,'') from variable where name = 'organization_nav_vocabulary' ";
		$this->db->query($qry);
		$res = $this->db->fetchColumn();
		expDebug::dPrint('Unserialized results for OrgVocabularyNumber'.print_r(unserialize($res),true),4);
		return !empty($res)? unserialize($res) : '4';
	}
	
	private function getCodeForCustomProfile($profileTag) {
  // This should be obtained from slt_profile_tagging_defn
  // select code from slt_profile_tagging_defn where profile_cat = 'Custom Profile' and profile_tag = $profileTag
  // However, this is master data which is unlikely to change.
  try{
  switch ($profileTag) {
    case 'Organization':
      return 12;
      break;
  }
  }catch (Exception $ex) {
    watchdog_exception('getCodeForCustomProfile', $ex);
    expertusErrorThrow($ex);
  }
}
	
}

?>