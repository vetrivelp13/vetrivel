<?php 
require_once "data_load_validate.php";
include_once DRUPAL_ROOT . "/includes/common.inc";
include_once DRUPAL_ROOT . "/includes/database/database.inc";
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
include_once DRUPAL_ROOT . "/sites/all/services/Encryption.php";
require_once DRUPAL_ROOT . '/sites/all/services/GlobalUtil.php';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
class users_dataload extends dataloadValidate{
	
	private $db;
	private $jobDetail;
	private $batchrecords;
	private $tblDet;
	
	function __construct($from=''){
		$this->db = new DLDatabase('DL',$from);
		$this->db->update_queue = true;
	}
	
	public function extenderValidate($jobDetail=array(),$batchrecords=array()){
		try{
			
			expDebug::dPrint('Job details '.print_r($jobDetail,true),4);
			expDebug::dPrint('Batch records '.print_r($batchrecords,true),4);
				
			$this->setJobDetails($jobDetail);
			$this->setBatchRecords($batchrecords);
				
			//Alter tables
			$this->alterTables();
			
		}catch(Exception $e){
	
		}
	
	}
	
	public function validate($jobDetail=array(),$batchrecords=array()){
		try{
		expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
		expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
		foreach($batchrecords as $rec){
			expDebug::dPrint('%%%%%%%%%%%%%%%checking the validate time starting--->'.microtime(true),4);
			$fields = array();
			$error = '';
			$op = '';
			expDebug::dPrint('test rec ' .print_r($jobDetail['batchopt'],true));
			if(!validateEmail($rec->email)){
				$error .= 'User email is not in valid format.\r';
			}
			else{
				$usrId = db_query("select count(1) from slt_person where email = '".addslashes($rec->email)."'")->fetchField();
				if(empty($usrId) && $jobDetail['batchopt'] == 'insert'){
					$usrId = db_query("select count(1) from ".$jobDetail['sTable']." where email = '".addslashes($rec->email)."'")->fetchField();
					if($usrId > 1){
						$error .= 'Duplicate email id.\r';
					}else{
						$fields['email'] = $rec->email;
					}
				}else if((!empty($usrId) && $jobDetail['batchopt'] == 'insert') || ($usrId > 1 && $jobDetail['batchopt'] == 'update')) {
					$error .= 'Duplicate email id.\r';
				}else{
					$fields['email'] = $rec->email;
				}
			}
			if(!empty($rec->user_name) && $jobDetail['batchopt'] == 'insert'){
				$usrId = db_query("select count(1) from ".$jobDetail['pTable']." where user_name = '".addslashes($rec->user_name)."'")->fetchField();
				$error .= ($usrId > 1) ? 'Duplicate User Name.\r' : '';
			}
			if($rec->status != 'Active' && $rec->status != 'Suspended' && $rec->status != 'Deleted'){
				$error .= 'User status can be only Active or Suspended or Deleted.\r';
			}else{
				$fields['status'] = ($rec->status == 'Active')? 'cre_usr_sts_atv' : (($rec->status == 'Suspended') ? 'cre_usr_sts_itv' : 'cre_usr_sts_del');
			}
		 	if(!empty($rec->org_id)){
				$orgId = db_query("select id from slt_organization where name = '".addslashes($rec->org_id)."' and status = 'cre_org_sts_act' Limit 1")->fetchField();
				if(empty($orgId))
					$error .= 'Organization specified for the user does not exist in LMS.\r' ;
				else
					$fields['org_id'] = $orgId;
			}
			if(!empty($rec->manager_id)){
				if($rec->user_name == $rec->manager_id){
					$error .= 'An User cannot be a manager for himself.\r';
				}else{
					$mgrdetails = db_query("select if(status = 'cre_usr_sts_atv',id,'inactive') as id,is_manager from slt_person where user_name = '".addslashes($rec->manager_id)."'")->fetchAll();//check manager already exists in person.
					$mgrId = $mgrdetails['id'];
					if(empty($mgrId)){
						$mgrId = db_query("select if(record_status='IN','invalid',mapping_id) from ".$jobDetail['pTable']." where user_name = '".addslashes($rec->manager_id)."'");
						if(empty($mgrId))
							$error .= 'Given manager does not exist as an user in both LMS and feed.\r';
						else if($mgrId == 'invalid'){
							$error .= 'Given manager does not exist as an user in LMS,  exist in feed, but is not a valid user (check error details of the manager user for more details).\r';
						}else{
							//$fields['manager_id'] = $mgrId;
							$fields['operation'] = 'upsert';
						} 
					}else if($mgrId == 'inactive'){
						$error .= 'Given manager exist as an user in LMS but is not active.\r';
					}else{
						if($mgrdetails['is_manager'] == 'Y')
							$fields['manager_id'] = $mgrId;
						else
							$error .= 'Given manager exist as an user in LMS but is not manager.\r';
					}
				}
			} 
			if(!empty($rec->employment_type)){
				$empId = db_query("select id from slt_profile_list_items where code = '".addslashes($rec->employment_type)."' and is_active = 'Y'")->fetchField();
				if(empty($empId))
					$error .= 'Employee Type specified for the user does not exist in LMS.\r' ;
				else
					$fields['employment_type'] = $rec->employment_type;
			}
			if(!empty($rec->dept_code)){
				$depId = db_query("select id from slt_profile_list_items where code = '".addslashes($rec->dept_code)."' and is_active = 'Y'")->fetchField();
				if(empty($depId))
					$error .= 'Department code specified for the user does not exist in LMS.\r' ;
				else
					$fields['dept_code'] = $rec->dept_code;
			}
			if(!empty($rec->job_title)){
				$jtlId = db_query("select id from slt_profile_list_items where code = '".addslashes($rec->job_title)."' and is_active = 'Y'")->fetchField();
				if(empty($jtlId))
					$error .= 'Job Title Code specified for the user does not exist in LMS.\r' ;
				else
					$fields['job_title'] = $rec->job_title;
			}
			if(!empty($rec->job_role)){
				expDebug::dPrint('JOB ROLE YOGA--->'.print_r($rec->job_role,1),4);
				$job_arr = explode(',',$rec->job_role);
				$job_role_str = '';
				foreach($job_arr as $job_role){
					$jbrId = db_query("select id from slt_profile_list_items where code = '".addslashes($job_role)."' and is_active = 'Y'")->fetchField();
					if(empty($jbrId))
						$error .= 'Job Role Code specified for the user does not exist in LMS.\r' ;
					else{
						if(empty($job_role_str))
						 	$job_role_str = $job_role;
						else
							$job_role_str .= ','.$job_role;
					}
				}
				if(!empty($job_role_str))
					$fields['job_role'] = $job_role_str;
			}
			if(!empty($rec->user_type)){
				$ustId = db_query("select id from slt_profile_list_items where name = '".addslashes($rec->user_type)."' and is_active = 'Y'")->fetchField();
				if(empty($ustId))
					$error .= 'User Type specified for the user does not exist in LMS.\r' ;
				else
					$fields['user_type'] = $rec->user_type;
			}
			if(!empty($rec->time_zone)){
				$tmezne = db_query("select code from slt_profile_list_items where name = '".addslashes($rec->time_zone)."' and is_active = 'Y'")->fetchField();
				if(empty($tmezne))
					$error .= 'Preferred Time Zone Name specified for the user does not exist in LMS.\r' ;
				else
					$fields['time_zone'] = $tmezne;
			}
			if(!empty($rec->preferred_language)){
				$lang = db_query("select code from slt_profile_list_items where name = '".addslashes($rec->preferred_language)."' and is_active = 'Y'")->fetchField();
				if(empty($lang))
					$error .= 'Preferred Language name specified for the user does not exist in LMS.\r' ;
				else
					$fields['preferred_language'] = $lang;
			}
			if(!empty($rec->state)){
				$stcode = db_query("select state_code from slt_state where state_code ='".addslashes($rec->state)."' Limit 1")->fetchField();
				if(empty($stcode))
					$error .= 'State Code specified for the user does not exist in LMS.\r' ;
				else
					$fields['state'] = $stcode;
			}
			if(!empty($rec->country)){
				$con = db_query("select country_code from slt_country where country_code = '".addslashes($rec->country)."' Limit 1")->fetchField();
				if(empty($con))
					$error .= 'Country Code specified for the user does not exist in LMS.\r' ;
				else
					$fields['country'] = $con;
			}
			if($rec->is_instructor == 'Y' || $rec->is_instructor == 'N'){
				$fields['is_instructor'] = $rec->is_instructor;
			}else{
				$error .= 'is_instructor field can only be Y or N.\r';
			}
			if($rec->is_manager == 'Y' || $rec->is_manager == 'N'){
				$fields['is_manager'] = $rec->is_manager;
			}else{
				$error .= 'is_manager field can only be N or Y.\r';
			}
			if(!empty($rec->dotted_mngr_id)){
				$dottedmgr = explode(',',$rec->dotted_mngr_id);
				$dottedmgrId = '';
				$err = '' ;
				foreach($dottedmgr as $mgr){
					if($rec->user_name == $mgr){
						$err .= 'An User '.$mgr.' cannot be a dotted manager for himself.\r';
					}else{
						$mgrdetails = db_query("select if(status = 'cre_usr_sts_atv',id,'inactive') as id,is_manager from slt_person where user_name = '".addslashes($mgr)."'")->fetchAll();//check manager already exists in person.
						$mgrId = $mgrdetails[0]->id;
						if(empty($mgrId)){
							$mgrId = db_query("select if(record_status='IN','invalid',mapping_id) from ".$jobDetail['pTable']." where user_name = '".addslashes($mgr)."' LIMIT 1")->fetchField();
							if(empty($mgrId))
								$err .= 'Given dotted manager '.addslashes($mgr).' does not exist as an user in both LMS and feed.\r';
							else if($mgrId == 'invalid'){
								$err .= 'Given dotted manager '.addslashes($mgr).'  does not exist as an user in LMS,  exist in feed, but is not a valid user (check error details of the manager user for more details).\r';
							}else{
								$fields['operation'] = 'upsert';
							}
						}else if($mgrId == 'inactive'){
							$err .= 'Given dotted manager '.addslashes($mgr).' exist as an user in LMS but is not active.\r';
						}else{
							if($mgrdetails[0]->is_manager == 'Y')
								$dottedmgrId .=  ($dottedmgrId == '') ? $mgrId : ','.$mgrId;
							else
								$err .= 'Given dotted manager '.addslashes($mgr).' exist as an user in LMS but is not manager.\r';
						}
					}
				}
				if(empty($err)){
					$fields['dotted_mngr_id'] = $dottedmgrId;
				}else{
					$error .= $err;
				}
			}
			if(!empty($rec->dotted_org_id)){
				$dottedorg = explode(',',$rec->dotted_org_id);
				$dottedorgId = '';
				$err = '' ;
				foreach($dottedorg as $org){
					$orgId = db_query("select id from slt_organization where name = '".addslashes($org)."' and status = 'cre_org_sts_act' LIMIT 1")->fetchField();
					if(empty($orgId))
						$err .= 'Dotted Organization '.addslashes($org).' specified for the user does not exist in LMS.\r' ;
					else
						$dottedorgId .= ($dottedorgId == '') ? $orgId : ','.$orgId;
				}
				if(empty($err)){
					$fields['dotted_org_id'] = $dottedorgId;
				}else{
					$error .= $err;
				}
			}
			if(empty($error)){
				if(!empty($rec->user_name)){
					$updateStmt = db_update($jobDetail['pTable']);
					$updateStmt->expression('full_name', 'concat(first_name,\' \',last_name)');
					$updateStmt->fields($fields);
					$updateStmt->condition('user_name',addslashes($rec->user_name),'=');
					expDebug::dPrintDBAPI(' $updateStmt for compliance ', $updateStmt);
					$updateStmt->execute();
				}
			}else{
				$updateStmt = db_update($jobDetail['pTable']);
				$updateStmt->fields(array(
						'record_status' => 'IN'
						));
				$updateStmt->condition('user_name',$rec->user_name,'=');
				expDebug::dPrintDBAPI(' $updateStmt for error ', $updateStmt);
				$updateStmt->execute();
				
				db_query("update ".$jobDetail['sTable']." set record_status = 'IN', remarks = '".$error."' where mapping_id = (select mapping_id from ".$jobDetail['pTable']." where user_name = '".addslashes($rec->user_name)."')");
			}
			expDebug::dPrint('%%%%%%%%%%%%%%%checking the validate time ending--->'.microtime(true),4);
		}
		
		}catch(Exception $e){
			// TODO: throw exception 
		}
	}   
	
	private function checkFeedHasGroups($jobDetail) {
		try {
			$this->db->connect();
			$table = (empty($jobDetail['pTable'])) ? $jobDetail['tblname'] : $jobDetail['pTable'];
			$qry = 'select count(1) from '. $table . ' where groups != \'\'';
			expDebug::dPrintDBAPI('checkFeedHasGroups query =  ', $qry);
			$this->db->query($qry);
			$resCount = $this->db->fetchColumn();
			$this->db->closeconnect();
			
			if($resCount>0 && $resCount!=''){
				return true;
			} else {
				return false;
			}	
		} catch(Exception $ex) {
			expDebug::dPrint("ERROR in generateNewUserCreation notification ".print_r($e,true),1);
		}
	}
	
	//Changed for #70900 by vetrivel.P in this function for some of the entity like employee_type,user_type,etc., changed to code instread of name
	public function bulkValidate($jobDetail=array(),$batchrecords=array()){
		try{
			$util = new GlobalUtil();
			$config = $util->getConfig();
			$configInvalid    = $config["skip_invalid_record"];
			expdebug::dPrint(' Job Details '.print_r($jobDetail,true),5);
			expdebug::dPrint(' batchrecords '.print_r($batchrecords,true),5);
			$this->setJobDetails($jobDetail);
			$this->setBatchRecords($batchrecords);
			expdebug::dPrint(' $configInvalid '. $configInvalid);
			  
			
			if($jobDetail['batchopt'] == 'update'){
				list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
				
				$fields=array(
					't2.primary_id' => 'per.id'
				);
				$condition=array(
					't2.batch_id = :batch ',
					't2.operation = :op ',
					't2.primary_id is null'
				);
				$join=array(
					'left join slt_person per on per.user_name = t2.user_name'
				);
				$args=array(
					':batch' => $jobDetail['batchId'],
					':op' => 'update'
				);
				
				$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
				expDebug::dPrintDBAPI("Update primary id",$qry,$args);
				$this->db->callExecute($qry,$args);
				
			}
			
			//Email Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			
			 $fields=array(
				 	't22.custom4' => "CASE WHEN t1.".$batchrecords['email']." NOT REGEXP (:regex ) THEN :err END",
				 	't22.record_status' => "CASE WHEN t1.".$batchrecords['email']." NOT REGEXP (:regex ) THEN :recst ELSE t22.record_status END",
				  't22.email' => "CASE WHEN t1.".$batchrecords['email']." REGEXP (:regex ) THEN t1.".$batchrecords['email']." ELSE NULL END"
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['email'].' != :mid ',
			 		't1.'.$batchrecords['email'].' IS NOT NULL'
			 );
			 $join=array(
			 		' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':regex' => '^[A-Z0-9._%-]+@[A-Z0-9.-]+\.[A-Z]{2,4}$',
			 		':mid'=>'',
			 		':err' => 'User email is not in valid format. ',
			 		':recst' => 'IN');
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" User email check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			 //Duplicate Email id Vaildation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => ":recst "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['email'].' != :nul ',
			 		't1.'.$batchrecords['email'].' IS NOT NULL',
			 		'per.user_name != t22.user_name'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_person per on per.email = t1.'.$batchrecords['email']
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'Duplicate email id. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Duplicate Email id Vaildation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			  
			//Duplicate Email within a Feed 
      		$flds = array('t22.record_status' => ':record_status ',
										't22.custom4'  => 'CONCAT(IFNULL(t22.custom4, ""), :err )');
			$params =  array(':record_status' => 'IN ',':err' => 'Duplicate email id.  ');
			$joins = array(' join (SELECT mapping_id as mapping_id,'.$batchrecords['email'].' as email 
															FROM '.$jobDetail['sTable'].' 
															GROUP BY '.$batchrecords['email'].' 
															HAVING count(1) > 1) t1 
															on t22.Mapping_id = t1.mapping_id ');
															
			$query1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$flds,array(),$joins);
			expDebug::dPrintDBAPI("Duplicate Email within a Feed  ",$query1,$params);
			$this->db->callExecute($query1,$params);                                                                                                                
                                                                                                                       
			 //User Name Validation
			 list($fields,$arg,$condition) = array(array(),array(),array());
			 $fields = array(
			 		"t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
			 		"t22.record_status" => ":recst "
			 );
			 $condition = array(
			 		"t22.batch_id = :batch_id ",
			 		"t22.user_name like '% %'"
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':recst' => 'IN',
			 		':err' => 'User name cannot have spaces. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			 expDebug::dPrintDBAPI(" User Name Validation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			  //User Name Limit  Validation - #0076555 - Added by ganeshbauv on Apr 26th 2017 11 AM 
			 list($fields,$arg,$condition) = array(array(),array(),array());
			 $fields = array(
			 		"t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
			 		"t22.record_status" => ":recst "
			 );
			 $condition = array(
			 		"t22.batch_id = :batch_id ", 
			 		"t22.user_name IS NOT NULL",
			 		't22.user_name != :nul ',
			 		"LENGTH(t22.user_name) > 60"
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':recst' => 'IN',
			 		':nul'=>'',
			 		':err' => 'User name should be less than 60 characters.'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			 expDebug::dPrintDBAPI(" User Name Limit Validation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
             /* start */ //#custom_attribute_0078975
             if(module_exists('exp_sp_administration_customattribute'))
             {        
             list($fields,$arg,$condition) = array(array(),array(),array());
             $checkcus = db_query("select count(1) as count from slt_custom_attr_mapping map1 join slt_custom_attr cattr on cattr.id = map1.cattr_id where map1.entity_screen_opt = 1 and map1.entity_type = 'cre_usr' and cattr.status='cre_cattr_sts_atv' ");
             
             $checkcus= $checkcus->fetchField();
             expDebug::dPrint("checkcus" . print_r($checkcus,1),5);
             if($checkcus > 0)
             {  
                $qrysel11 = "select group_concat(c1.entity_ref_tbl_col) as entity_ref_tbl_col from slt_custom_attr_mapping c1 join slt_custom_attr c2 on c2.id = c1.cattr_id 
                        where c2.status = 'cre_cattr_sts_atv' and c1.entity_type = 'cre_usr' and c1.entity_screen_opt = 1 order by c1.entity_ref_tbl_col asc";
                $this->db->callQuery($qrysel11);
                $cuscoldet = $this->db->fetchAllResults();
                expDebug::dPrint(' $usrId 2222>>>'.print_r($cuscoldet,1), 5);
                $cuscoldet1 = explode(',',$cuscoldet[0]->entity_ref_tbl_col);
                foreach($cuscoldet1 as $valuesplit)
                {
                    expDebug::dPrint(' valuesplit'.print_r($valuesplit,1), 5);
                    $batchrecords[''.$valuesplit.'']=strtolower(str_replace(' ','_',$batchrecords[''.$valuesplit.'']));
                    $qrysel1 = 'select cusattr.id,cusattr.cattr_name,cusattr.cattr_type,cusattr.status,cusattr.cattr_datatype,cusattr.cattr_length,cusattr.cattr_multiple_opt,map1.entity_ref_tbl_col,
                            GROUP_CONCAT(attr_opt.opt_name) as cattr_opt_name,GROUP_CONCAT(attr_opt.opt_code) as cattr_opt_codes,
                            if(cusattr.cattr_type in ("cattr_type_txtarea","cattr_type_txtbox"),0,count(attr_opt.opt_code)) as cattr_opt_count
                            from slt_custom_attr_mapping map1 join slt_custom_attr cusattr on map1.cattr_id = cusattr.id
                            left Join slt_custom_attr_options attr_opt on attr_opt.cattr_id=cusattr.id
                            where LOWER(REPLACE(cusattr.cattr_name," ","_")) = "'.$batchrecords[''.$valuesplit.''].'" 
                            and cusattr.status = "cre_cattr_sts_atv" and map1.entity_type = "cre_usr" and map1.entity_screen_opt = 1 order by map1.entity_ref_tbl_col asc';
                
                $this->db->callQuery($qrysel1);
                $cusdetails = $this->db->fetchAllResults();
                expDebug::dPrint('cusdetails'.print_r($cusdetails,1), 5);
                 
                $opt_name_arr=explode(",",$cusdetails[0]->cattr_opt_name);
                $opt_code_arr=explode(",",$cusdetails[0]->cattr_opt_codes);
                 
                $opt_arr = array_combine($opt_name_arr, $opt_code_arr);
                expDebug::dPrint('option array >>'.print_r($opt_arr,1), 5);
                 
                // custom attribute length validation
                if($cusdetails[0]->cattr_datatype == 'cattr_dtype_text' || $cusdetails[0]->cattr_type == 'cattr_type_txtarea')
                {   
                $fields = array(
                        "t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
                        "t22.record_status" => ":recst "
                );
                $condition = array(
                        "t22.batch_id = :batch_id ",
                        "t22.$valuesplit IS NOT NULL",
                        "t22.$valuesplit != :nul ",
                        "LENGTH(t22.$valuesplit) > :len" 
                                     
                );
                $arg = array(
                        ':batch_id' => $jobDetail['batchId'],
                        ':recst' => 'IN',
                        ':nul'=>'',
                        ':err' => ''.$batchrecords[''.$valuesplit.''].' should not exceed '.$cusdetails[0]->cattr_length.' characters. ',
                        ':len'=> $cusdetails[0]->cattr_length
                );
                $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
                expDebug::dPrintDBAPI(" Custom attribute Limit Validation ",$qry1,$arg);
                $this->db->callExecute($qry1,$arg);
                }
                
                // custom attribute number type validation
                if($cusdetails[0]->cattr_datatype == 'cattr_dtype_numbers')
                {
                $flds1 = array(
                        "t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
                        "t22.record_status" => ":recst "
                );
                $joincus1=array(
                        'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
                        
                );
                $conditioncus1 = array(
                        "t22.batch_id = :batch_id ",
                        "t22.$valuesplit IS NOT NULL",
                        "t22.$valuesplit != :nul ",
                        "t1.".$batchrecords[''.$valuesplit.'']." = t22.$valuesplit",
                        "t22.$valuesplit NOT REGEXP '^[0-9]*[.]?[0-9]+$' OR (substr(t22.$valuesplit,-1) = '.')"
                );
                $arg1 = array(
                        ':batch_id' => $jobDetail['batchId'],
                        ':recst' => 'IN',
                        ':nul'=>'',
                        ':err' => ''.$batchrecords[''.$valuesplit.''].' has a non-numeric value. '
                );
                $qry2 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$flds1,$conditioncus1,$joincus1);
                expDebug::dPrintDBAPI(" Custom attribute Number Type Validation ",$qry2,$arg1);
                $this->db->callExecute($qry2,$arg1);
                                
                //Check no of characters  - accepts maximum @count digits.
                $flds3 = array(
                        "t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
                        "t22.record_status" => ":recst "
                );
                $conditioncus3 = array(
                        "t22.batch_id = :batch_id ",
                        "t22.$valuesplit IS NOT NULL",
                        "t22.$valuesplit != :nul ",
                        "LENGTH(REPLACE(t22.$valuesplit,'.','')) > :len"
                );
                $arg3 = array(
                        ':batch_id' => $jobDetail['batchId'],
                        ':recst' => 'IN',
                        ':nul'=>'',
                        ':err' => ''.$batchrecords[''.$valuesplit.''].' accepts maximum '.$cusdetails[0]->cattr_length.' digits. ',
                        ':len'=> $cusdetails[0]->cattr_length
                );
                $qry4 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$flds3,$conditioncus3);
                expDebug::dPrintDBAPI(" Custom attribute Number before decimal Validation ",$qry4,$arg3);
                $this->db->callExecute($qry4,$arg3);
                }
                
             // checkbox radio dropdown
             if(($cusdetails[0]->cattr_type == 'cattr_type_checkbox') || ($cusdetails[0]->cattr_type == 'cattr_type_dropdown') || ($cusdetails[0]->cattr_type == 'cattr_type_radio'))
             {
                $qry5 = "select t1.".$batchrecords[''.$valuesplit.'']." as column1,
                                                        t1.".$batchrecords['user_name']." as user_name
                                                        from ".$jobDetail['sTable']." t1
                                                        inner join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
                                                        where
                                                        t2.batch_id =".$jobDetail['batchId']." AND
                                                        (t1.".$batchrecords[''.$valuesplit.'']." !='' AND t1.".$batchrecords[''.$valuesplit.'']." is not null)";
                expDebug::dPrint(' check '. $qry5);
                $this->db->callQuery($qry5);
                $dlist = $this->db->fetchAllResults();
                expDebug::dPrint('check 1 '.print_r($dlist,true),5);
                
                foreach($dlist as $rec1){
                    //list($fields, $args,$err) = array(array(),array(),'');
                    $actualOptions = explode(',',$cusdetails[0]->cattr_opt_name);
                    expDebug::dPrint('Validation actual options --> '.print_r($actualOptions,true), 5);
                    $inputOptions = explode(',',$rec1->column1);
                    $inputOptionsCount = count($inputOptions);
                    expDebug::dPrint('Validation input options --> '.print_r($inputOptions,true), 5);
                    expDebug::dPrint('Validation input options count --> '.print_r($inputOptionsCount,true), 5);
                    $matchOptionsCount = count(array_intersect($actualOptions, array_map('trim',$inputOptions)));
                    expDebug::dPrint('Validation match options result --> '.print_r($matchOptionsCount,true), 5);
                    if($cusdetails[0]->cattr_type == 'cattr_type_radio' || ($cusdetails[0]->cattr_type == 'cattr_type_dropdown' && ($cusdetails[0]->cattr_multiple_opt == '' || $cusdetails[0]->cattr_multiple_opt == 0)))
                    {
                        if($inputOptionsCount > 1) {
                    $field4= array(
                            "t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
                            "t22.record_status" => ":recst "
                    );
                    $join4=array(
                            ' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
                    );
                    $conditioncus4 = array(
                            "t22.user_name = :user_name ",
                            "t22.batch_id = :batch_id ",
                            "t22.$valuesplit IS NOT NULL",
                            "t22.$valuesplit != :nul "
                                        );
                    $arg4 = array(
                            ':user_name' => $rec1->user_name,
                            ':batch_id' => $jobDetail['batchId'],
                            ':recst' => 'IN',
                            //':err' => ''.$batchrecords[''.$valuesplit.''].' accepts only one option.',
                            ':err' => 'Incorrect value for '.$batchrecords[''.$valuesplit.''].'. ',
                            ':nul'=>''
                    );
                    $qry6 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$field4,$conditioncus4,$join4);
                    expDebug::dPrintDBAPI(" Custom attribute option Validation ",$qry6,$arg4);
                    $this->db->callExecute($qry6,$arg4);                            
                        }
                    }
                        
                    if($inputOptionsCount != $matchOptionsCount){
                        $field5= array(
                                "t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
                                "t22.record_status" => ":recst "
                        );
                        $join5=array(
                                ' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
                        );
                        $conditioncus5 = array(
                                "t22.user_name = :user_name ",
                                "t22.batch_id = :batch_id ",
                                "t22.$valuesplit IS NOT NULL",
                                "t22.$valuesplit != :nul "
                        );
                        $arg5 = array(
                                ':user_name' => $rec1->user_name,
                                ':batch_id' => $jobDetail['batchId'],
                                ':recst' => 'IN',
                                //':err' => ''.$batchrecords[''.$valuesplit.''].' has invalid option.',
                                ':err' => 'Incorrect value for '.$batchrecords[''.$valuesplit.''].'. ',
                                ':nul'=>''
                        );
                        $qry10 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$field5,$conditioncus5,$join5);
                        expDebug::dPrintDBAPI(" Custom attribute invalid option Validation ",$qry10,$arg5);
                        $this->db->callExecute($qry10,$arg5);
                    }
                    $tmp_feed_opt_arr= explode(',',$rec1->column1);
                    $tmp_feed_opt_arr = array_filter(array_map('trim', $tmp_feed_opt_arr));
                    expDebug::dPrint('$tmp_feed_opt_arr to store ='.print_r($tmp_feed_opt_arr,true),5);
                     $opt_to_store='';
                     $tmp_opt_arr=array();
                     if(count($tmp_feed_opt_arr)>0){
                        foreach($tmp_feed_opt_arr as $tmp_opt_key => $tmp_opt_val){
                            $tmp_opt_arr[]=$opt_arr[$tmp_opt_val];
                        }
                        $opt_to_store=implode(",",$tmp_opt_arr);
                     }
                    
                     expDebug::dPrint('$opt_to_store ='.print_r($opt_to_store,true),5);
                    expDebug::dPrint('check 2 '.print_r($rec1->column1,true),5);
                    //$qry11 = "UPDATE ".$jobDetail['pTable']." SET  ".$valuesplit."= '$rec1->column1'  WHERE  user_name =  '$rec1->user_name' and record_status != 'IN' "; 
                    $qry11 = "UPDATE ".$jobDetail['pTable']." SET  ".$valuesplit."= '".$opt_to_store."'  WHERE  user_name =  '$rec1->user_name' and record_status != 'IN' ";
                    expDebug::dPrint('check $qry11 '.$qry11);
                    $this->db->callExecute($qry11,$arg);
                }                                            
               }
              }
             }
            }
			//Status Validate
			
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 
			$fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err )",
				 	't22.record_status' => ":recst "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['status'].' NOT IN ( :actv , :susp , :del )'
			 );
			 $join=array(
			 		' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'User status can be only Active or Suspended or Deleted. ',
			 		':recst' => 'IN',
			 		':actv' => 'Active',
			 		':susp' => 'Suspended',
			 		':del' => 'Deleted'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" User status check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			 //Hire Date Validation - Added by ganeshbabuv on Apr 25th 2017 for #0075944
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
 			 $fields=array(
			 		't22.hire_date'=> "DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y-%m-%d')",
					//'t22.custom4' => "CASE WHEN DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y-%m-%d') is NULL AND t1.".$batchrecords['hire_date']." != :nul THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.custom4' => "CASE 					
										WHEN DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y-%m-%d') is NULL AND t1.".$batchrecords['hire_date']." != :nul THEN concat(ifnull(t22.custom4,''), :err )
										WHEN DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y')>date_format(date_add(now(),INTERVAL 5 Year),'%Y') AND t1.".$batchrecords['hire_date']." != :nul THEN concat(ifnull(t22.custom4,''), :err_future )
										ELSE t22.custom4 END",
					't22.record_status' => "CASE 
											WHEN DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y-%m-%d') is NULL AND t1.".$batchrecords['hire_date']." != :nul AND ".$configInvalid." = 1 THEN :recst 
											WHEN DATE_FORMAT(t1.".$batchrecords['hire_date'].",'%Y')>date_format(date_add(now(),INTERVAL 5 Year),'%Y') AND t1.".$batchrecords['hire_date']." != :nul AND ".$configInvalid." = 1 THEN :recst
											ELSE t22.record_status END"
			 );
			 $condition=array(
					"t22.batch_id = :batch_id"
			 );
			 $join=array(
					' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
					':batch_id' => $jobDetail['batchId'],
					':err' => 'Invalid Hire date',
					//':err_past' => 'Invalid Hire date,which should be below 50 years from current year',
					':err_future' => 'Invalid Hire date,which should not be above 5 years from current year',
					':recst' => 'IN',
			 		':nul'=>''
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Hire date Validation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//User Type Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
	 	 	$fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.user_type' => 'if(spli.id = 0 ,null,spli.code)'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['user_type'].' != :nul ',
			 		't1.'.$batchrecords['user_type'].' IS NOT NULL'
			 );
			 $join=array(
			 		' left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		' left join slt_profile_list_items spli on t1.'.$batchrecords['user_type'].' = spli.code and spli.is_active = :is_active and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'User Type specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':is_active' => 'Y',
			 		':code' => "cre_usr_ptp_%"
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("User Type check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
	 	 	
	 	 	/*
	 	 	//Job Role Valiadtion
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.job_role' => 'if(spli.id = 0 ,null,(select group_concat(code) from slt_profile_list_items where find_in_set(code,replace(concat(\'cre_usr_jrl_\', t1.'.$batchrecords['job_role'].'), \',\',\',cre_usr_jrl_\'))))'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['job_role'].' != :nul ',
			 		't1.'.$batchrecords['job_role'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on find_in_set(spli.code, replace(concat(\'cre_usr_jrl_\', t1.'.$batchrecords['job_role'].'), \',\',\',cre_usr_jrl_\')) and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Job Role specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':code' => "cre_usr_jrl_%"
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Job Role qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			*/
			 
			//Job title Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.job_title' => 'if(spli.id = 0 ,null,spli.code)'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['job_title'].' != :nul',
			 		't1.'.$batchrecords['job_title'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on t1.'.$batchrecords['job_title'].' = spli.code and spli.is_active = :is_active and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Job Title specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':is_active' => 'Y',
			 		':code' => '%cre_usr_jtl_%'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Job Title qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Employee Type Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.employment_type' => 'if(spli.id = 0 ,null,spli.code)'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['employment_type'].' != :nul',
			 		't1.'.$batchrecords['employment_type'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on t1.'.$batchrecords['employment_type'].' = spli.code and spli.is_active = :is_active and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Employee Type specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':is_active' => 'Y',
			 		':code' => '%cre_usr_etp_%'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Employee Type qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			 
			 #0076380 - Added for ganeshbabuv Apr 25th 2017 for avoiding duplicate employee id in Feed and DB
			 //Duplicate Employee ID within a Feed 
      		$flds = array('t22.record_status' => ':record_status ',
										't22.custom4'  => 'CONCAT(IFNULL(t22.custom4, ""), :err )');
			$params =  array(':record_status' => 'IN ',':err' => 'Employee id is not unique within Feed.  '); 															
		  	$joins = array(' join (SELECT mapping_id as mapping_id,'.$batchrecords['employee_no'].' as employee_no 
															FROM '.$jobDetail['sTable'].' 
															WHERE '.$batchrecords['employee_no'].'!=\'\'
															GROUP BY '.$batchrecords['employee_no'].' 
															HAVING count(1) > 1) t1 
															on t22.'.$batchrecords['employee_no'].' = t1.'.$batchrecords['employee_no']);
			
															
			$query1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$flds,array(),$joins);
			expDebug::dPrintDBAPI("Duplicate Employee id Vaildation within a Feed  ",$query1,$params);
			$this->db->callExecute($query1,$params);    
			
			 //Employee ID validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => ":recst "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		//'t1.'.$batchrecords['employee_no'].' != :nul ',
			 		//'t1.'.$batchrecords['employee_no'].' IS NOT NULL',
			 		'per.employee_no = t22.employee_no',
			 		'per.status!=:del_code'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		//'left join slt_person per on per.email = t1.'.$batchrecords['email']
			 		'left join slt_person per on per.user_name != t1.'.$batchrecords['user_name'] . ' AND per.employee_no IS NOT NULL AND per.employee_no != \'\'' 
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		//':nul'=>'',
			 		':recst' => 'IN',
			 		':del_code' => 'cre_usr_sts_del', 
			 		':err' => 'Employee id is not unique. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Duplicate Employee id Vaildation ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			  
			 
			 
			//Department Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.dept_code' => 'if(spli.id = 0 ,null,spli.code)'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id',
			 		't1.'.$batchrecords['dept_code'].' != :nul ',
			 		't1.'.$batchrecords['dept_code'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on t1.'.$batchrecords['dept_code'].' = spli.code and spli.is_active = :is_active and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Department specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':is_active' => 'Y',
			 		':code' => '%cre_usr_dpt_%'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Department qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Time zone validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(spli.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.time_zone' => 'spli.code',
			 		't22.custom2' => 'spli.attr2'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id',
			 		't1.'.$batchrecords['time_zone'].' != :nul ',
			 		't1.'.$batchrecords['time_zone'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on t1.'.$batchrecords['time_zone'].' = spli.name and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Preferred Time Zone Name specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':code' => '%cre_sys_tmz_%'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Preferred Time Zone qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Default Timezone Update for null columns 
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$timezone  = date_default_timezone_get(); //$defaultTimezone['sysTimezone'];
			//$timezone  = 'Asia/Kolkata';
			$fields=array(
					't22.custom2' => ':tz ',
					't22.time_zone' => 'spli.code'
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.time_zone IS NULL',
					't22.custom2 IS NULL',
					't22.primary_id is NULL'
			);
			$join=array(
					'JOIN slt_profile_list_items spli ON spli.attr2 = :tz AND spli.code like :code '
			);
			$arg=array(
					':batch_id' => $jobDetail['batchId'],
					':tz' => $timezone,
			 		':code' => 'cre_sys_tmz_%'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Defatult Time Zone qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
                        
			//Update Existing is_instructor Column
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't2.is_instructor' => 'per.is_instructor'
			);
			$condition=array(
					't2.batch_id = :batch ',
					't2.primary_id is NOT NULL',
					't1.is_instructor =:nul OR t1.is_instructor IS NULL'
			);
			$join=array(
					'left join slt_person per on per.user_name = t2.user_name',
					'left join '.$jobDetail['sTable'].' t1 on t2.user_name = t1.user_name'
			);
			$args=array(
					':batch' => $jobDetail['batchId'],
					':nul' => ''
			);
			
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Update existing is_instructor column for existing user ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Update Existing is_manager Column
			list($fields, $args,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't2.is_manager' => 'per.is_manager'
			);
			$condition=array(
					't2.batch_id = :batch ',
					't2.primary_id is NOT NULL',
					't1.is_manager =:nul OR t1.is_manager IS NULL'
			);
			$join=array(
					'left join slt_person per on per.user_name = t2.user_name',
					'left join '.$jobDetail['sTable'].' t1 on t2.user_name = t1.user_name'
			);
			$args=array(
					':batch' => $jobDetail['batchId'],
					':nul' => ''
			);
				
			$qry = $this->db->prepareQueryUpdate($jobDetail['pTable']." t2",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Update existing is_manager column for existing user ",$qry,$args);
			$this->db->callExecute($qry,$args);
			
			//Preferred Language Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "CASE WHEN spli.code IS NULL THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.preferred_language' => 'spli.code',
			 		't22.custom1' => 'spli.attr1'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['preferred_language'].' != :nul ',
			 		't1.'.$batchrecords['preferred_language'].' IS NOT NULL '
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_profile_list_items spli on t1.'.$batchrecords['preferred_language'].' = spli.name and spli.is_active = :is_active and spli.code like :code '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Preferred Language name specified for the user does not exist in LMS. ',
			 		':nul' => '',
			 		':recst' => 'IN',
			 		':is_active' => 'Y',
			 		':code' => '%cre_sys_lng_%'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Preferred Language qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Preferred Currency Validation
             list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
             $fields=array(
                     't22.custom4' => "CASE WHEN spli.attr1 IS NULL THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
                     't22.record_status' => "CASE WHEN ifnull(spli.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
                     't22.preferred_currency' => 'spli.attr1'
             );
             $condition=array(
                     't22.batch_id = :batch_id ',
                     't1.'.$batchrecords['preferred_currency'].' != :nul ',
                     't1.'.$batchrecords['preferred_currency'].' IS NOT NULL '
             );
             $join=array(
                     'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
                     'left join slt_profile_list_items spli on t1.'.$batchrecords['preferred_currency'].' = spli.attr1 and spli.code like :code and spli.is_active = :is_active and spli.attr3 = :attr3 '
             );
             $arg = array(
                     ':batch_id' => $jobDetail['batchId'],
                     ':err' => 'Preferred Currency Code specified for the user does not exist in LMS. ',
                     ':nul' => '',
                     ':recst' => 'IN',
                     ':code' => '%cre_sys_crn_%',
                     ':is_active' => 'Y',
                     ':attr3' => 'Y',
             );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Preferred Currency qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			//Default Language update
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$language =  $this->getDefaultLanguage();
			$fields=array(
					't22.custom1' => ':lang ',
					't22.preferred_language' => 'spli.code'
			);
			$condition=array(
					't22.batch_id = :batch_id ',
			 		'(t22.preferred_language = :nul or t22.preferred_language IS NULL)',
					't22.custom1 IS NULL',
					't22.primary_id is NULL'
			);
			$join=array(
					'join slt_profile_list_items spli on spli.attr1 = :lang and spli.is_active = :is_active and spli.code like :code '
			);
			$arg=array(
					':batch_id' => $jobDetail['batchId'],
					':lang' => $language,
					':nul' => '',
					':is_active' => 'Y',
			 		':code' => '%cre_sys_lng_%'
			);
			
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("Default Language update ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			 
			//Country Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(st.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN ifnull(st.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.country' => 'if(st.id = 0 ,null, st.country_code )'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['country'].' != :nul ',
			 		't1.'.$batchrecords['country'].' IS NOT NULL '
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_country st on t1.'.$batchrecords['country'].' = st.country_name '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'Country Name specified for the user does not exist in LMS. ',
			 		':recst' => 'IN',
			 		':nul' => ''
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("Country Code qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			//State Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(st.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN ifnull(st.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.state' => 'if(st.id = 0 ,null, st.state_code)'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['state'].' != :nul ',
			 		't1.'.$batchrecords['state'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_state st on t1.'.$batchrecords['state'].' = st.state_name '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'State Name specified for the user does not exist in LMS. ',
			 		':recst' => 'IN',
			 		':nul' => ''
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("State Code qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			 //State within Country Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
			 		't22.custom4' => "CASE WHEN ifnull(st.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(st.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END"
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['state'].' != :nul ',
			 		't1.'.$batchrecords['country'].' != :nul ',
			 		't1.'.$batchrecords['country'].' IS NOT NULL',
			 		't1.'.$batchrecords['state'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_country cty ON t1.'.$batchrecords['country'].' = cty.country_name',
					'left join slt_state st ON cty.country_code = st.country_code AND t1.'.$batchrecords['state'].' = st.state_name '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':err' => 'State Name not exist in the Country specified for the user. ',
			 		':recst' => 'IN',
			 		':nul' => ''
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI("State within Country qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
                         
			//Instructor Validation
			
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "CASE WHEN t1.".$batchrecords['is_instructor']." NOT IN ( :y , :n )  THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN t1.".$batchrecords['is_instructor']." NOT IN ( :y , :n ) AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
				  't22.is_instructor' => "CASE WHEN t1.".$batchrecords['is_instructor']." IN ( :y , :n ) THEN t1.".$batchrecords['is_instructor']." ELSE NULL END"
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['is_instructor'].' != :nul ',
			 		't1.'.$batchrecords['is_instructor'].' IS NOT NULL '
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':y' => 'Y',
			 		':n' => 'N',
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'is_instructor field can only be Y or N. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Instructor check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
                         
                        //Instructor has active association and cannot be modified now.
			 $qry = "select per.id as user_id from ".$jobDetail['pTable']." t2
                                 inner join slt_person per on per.user_name = t2.user_name 
                                 where per.is_instructor = 'Y' and (t2.is_instructor = 'N' or t2.is_instructor = '')";
                        
                            expDebug::dPrint('Instructor detais to check-->>>'. $qry);
                            $this->db->callQuery($qry);
                            $instructor = $this->db->fetchAllResults();
                            
                        expDebug::dPrint("Instructor result--->>> " .print_r($instructor,true),5);
                        if(!empty($instructor)){
                        	
                            foreach($instructor as $ins){
                                $userId = $ins->user_id;
                                $checkSessionIns = checkInstructorAssignedSession($userId);
                                if($checkSessionIns > 0) {
                                    $ins_id[] = $userId;
                                }
                            }
                        
                        $failureUserId  = implode(',',$ins_id);
                        list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
                        $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err )",
			 		't22.record_status' => ":recsts"
			 );
             if(!empty($failureUserId)){
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		"t1.id IN ( $failureUserId )"
			 );
                         $join=array(
			 		'inner join slt_person t1 on t1.user_name = t22.user_name'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
                                        ':recsts' => 'IN',
			 		':err' => 'Instructor has active association and cannot be modified now.\r'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Instructor has active association check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
             }
                        }else{
                            //No Validation required
                        }
			
            //Manager Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN t1.".$batchrecords['is_manager']." NOT IN ( :y , :n )  THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN t1.".$batchrecords['is_manager']." NOT IN ( :y , :n ) AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
				  't22.is_manager' => "CASE WHEN t1.".$batchrecords['is_manager']." IN ( :y , :n ) THEN t1.".$batchrecords['is_manager']." ELSE :n  END"
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['is_manager'].' != :nul ',
			 		't1.'.$batchrecords['is_manager'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':y' => 'Y',
			 		':n' => 'N',
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'is_manager field can only be N or Y.\r'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Is Manager check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Organization name Validation
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "CASE WHEN ifnull(org.id,0) = 0 THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
			 		't22.record_status' => "CASE WHEN ifnull(org.id,0) = 0 AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
			 		't22.org_id' => 'if(org.id = 0 ,null, org.id )'
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['org_id'].' != :nul ',
			 		't1.'.$batchrecords['org_id'].' IS NOT NULL'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_organization org on  t1.org_number = org.number and org.status = "cre_org_sts_act" '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'Organization specified for the user does not exist or is inactive in LMS. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Organization check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			//Manager name validation	
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
					't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL',
			 		"t1.".$batchrecords['user_name']." = t1.".$batchrecords['manager_id']
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'An User cannot be a manager for himself. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Manager name check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			 
			 //Manager and Other manager name Conflict
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
			 		't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['dotted_mngr_id'].' != :nul ',
			 		't1.'.$batchrecords['dotted_mngr_id'].' IS NOT NULL',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL AND FIND_IN_SET(t1.'.$batchrecords['manager_id'].',t1.'.$batchrecords['dotted_mngr_id'].')'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'Given Manager cannot be an Other Manager for the User. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Manager  name check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			 //Organization and Other Organization name Conflict
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
			 		't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['org_id'].' != :nul ',
			 		't1.'.$batchrecords['dotted_org_id'].' != :nul ',
			 		't1.'.$batchrecords['dotted_org_id'].' IS NOT NULL',
			 		't1.'.$batchrecords['org_id'].' IS NOT NULL AND FIND_IN_SET(t1.'.$batchrecords['org_id'].',t1.'.$batchrecords['dotted_org_id'].')'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'Given Organization cannot be an Other Organization for the User. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Manager  name check qry1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);

			 // Manager Exists check
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL ',
			 		'per.id IS NULL',
			 		't11.'.$batchrecords['user_name'].' IS NULL '
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join '.$jobDetail['sTable'].' t11 on t1.'.$batchrecords['manager_id'].' = t11.'.$batchrecords['user_name'],
			 		'left join slt_person per on t1.'.$batchrecords['manager_id'].' = per.user_name'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':recst' => 'IN',
			 		':err' => 'Given manager does not exist as an user in both LMS or feed. '
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Manager exists check qry2 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			
			 // Manager is active or not
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
			 		't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL ',
			 		'(((per.status !=  :actv  AND per.is_manager = :y ) 
					OR (t11.'.$batchrecords['status'].' !=  :atv AND  t11.'.$batchrecords['user_name'].' IS NOT NULL AND t11.'.$batchrecords['is_manager'].' =  :y ))
					OR((per.is_manager != :y  AND ifnull(per.id,0) != 0 ) OR (t11.'.$batchrecords['user_name'].' IS NOT NULL AND t11.'.$batchrecords['is_manager'].' !=  :y ))
  				OR(t2.record_status = :recst or t11.record_status = :recst ))'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id ',
			 		'left join '.$jobDetail['sTable'].' t11 on t1.'.$batchrecords['manager_id'].' = t11.'.$batchrecords['user_name'],
			 		'left join slt_person per on t1.'.$batchrecords['manager_id'].' = per.user_name ',
			 		'left join '.$jobDetail['pTable'].' t2 on t1.'.$batchrecords['manager_id'].' = t2.user_name '
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':err' => 'Given manager exist as an user in LMS or feed but is not active or not a manager. ',
			 		':actv' => 'cre_usr_sts_atv',
			 		':atv'=>'Active',
			 		':recst' => 'IN',
			 		':y' => 'Y'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Manager is active or not qry ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			// Existing manager Id update
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
				 	't22.manager_id' => " per.id "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL ',
			 		'per.status = :atvt ',
			 		'per.is_manager = :y '
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join slt_person per on t1.'.$batchrecords['manager_id'].' = per.user_name'
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':atvt' => 'cre_usr_sts_atv',
			 		':y' => 'Y'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Existing manager Id update qry ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			// Upsert record update
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
			 		't22.operation' => " :op "
			 );
			 $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['manager_id'].' != :nul ',
			 		't1.'.$batchrecords['manager_id'].' IS NOT NULL ',
			 		't11.'.$batchrecords['is_manager'].' = :y ',
			 		't11.'.$batchrecords['status'].' = :atv ',
			 		't11.'.$batchrecords['user_name'].' is not null',
			 		't11.record_status is null or t11.record_status != :rec ',
			 		't22.manager_id is null',
			 		'per.user_name is null'
			 );
			 $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id',
			 		'left join '.$jobDetail['sTable'].' t11 on t1.'.$batchrecords['manager_id'].' = t11.'.$batchrecords['user_name'],
			 		'left join slt_person per on t1.'.$batchrecords['manager_id'].' = per.user_name'
			 		
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':op' => 'upsert',
			 		':atv' => 'Active',
			 		':y' => 'Y',
			 		':rec' => 'IN'
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Upsert record update qry ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);
			 
			 //Status update
			 list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			 $fields=array(
				 	't22.status' => " CASE 
				      WHEN t1.status= :a THEN :atv 
				      WHEN t1.status= :i THEN :itv 
				      WHEN t1.status= :d THEN :del 
				      ELSE t22.status = :nul
				      END "
			  );
			  $condition=array(
			 		't22.batch_id = :batch_id ',
			 		't1.'.$batchrecords['status'].' != :nul ',
			 		't1.'.$batchrecords['status'].' IS NOT NULL ',
			  		 );
			  $join=array(
			 		'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
			  );
			  $arg = array(
			 		':batch_id' => $jobDetail['batchId'],
			 		':nul'=>'',
			 		':a' => 'Active',
			 		':atv' => 'cre_usr_sts_atv',
			 		':i' => 'Suspended',
			 		':itv' => 'cre_usr_sts_itv',
			 		':d' => 'Deleted',
			 		':del' => 'cre_usr_sts_del'
			  );
			  $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			  expDebug::dPrintDBAPI(" Status update qry ",$qry1,$arg);
			  $this->db->callExecute($qry1,$arg);
			
			  			
			//Dotted manager and Dotted Organization Validate
			$qry = "select t1.".$batchrecords['dotted_mngr_id']." as dotted_mngr_id,t1.".$batchrecords['dotted_org_id']." as dotted_org_id, t1.".$batchrecords['job_role']." as job_role,
														t1.".$batchrecords['user_name']." as user_name
														from ".$jobDetail['sTable']." t1
														inner join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
														where
														t2.batch_id =".$jobDetail['batchId']." AND 
														((t1.".$batchrecords['dotted_mngr_id']." !='' AND t1.".$batchrecords['dotted_mngr_id']." is not null) OR (t1.".$batchrecords['dotted_org_id']." !='' AND t1.".$batchrecords['dotted_org_id']." is not null) OR (t1.".$batchrecords['job_role']." !='' AND t1.".$batchrecords['job_role']." is not null))";
			
			expDebug::dPrint(' Dotted Manager and Dotted organization and Job Role '. $qry);
			$this->db->callQuery($qry);
			$dottedlist = $this->db->fetchAllResults();
			expDebug::dPrint('dotted manager and organization list '.print_r($dottedlist,true),5);
			
			$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
			$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
			$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
				
			expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
				
			$this->db->callQuery($query,$args);
			$checkSupAdmin = $this->db->fetchColumn();
			expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
			
			foreach($dottedlist as $rec){
				list($fields, $args,$err) = array(array(),array(),'');
				//$error = '';
				$rec->dotted_mngr_id=trim($rec->dotted_mngr_id);
				$rec->dotted_mngr_id = implode(',',array_unique(explode(',', $rec->dotted_mngr_id)));
				if(!empty($rec->dotted_mngr_id)){
					$dottedmgr = explode(',',$rec->dotted_mngr_id);
					$dottedmgr = array_map('trim',$dottedmgr);
					$dottedmgrId = '';
					foreach($dottedmgr as $mgr){
						if($rec->user_name == $mgr){
							$err .= 'An User '.$mgr.' cannot be a dotted manager for himself. ';
						}else{
							$query = "select if(per.status = 'cre_usr_sts_atv',per.id,'inactive') as id,per.is_manager, if(t1.user_name = :user_name , 'exist','nonexist') as userexist, t1.is_manager as tmgr from slt_person per
												left join ".$jobDetail['sTable']." t1 on t1.user_name = per.user_name
												where per.user_name = :user_name ";
							$this->db->callQuery($query,array(':user_name'=>addslashes($mgr)));
							$mgrdetails = $this->db->fetchAllResults();
							expDebug::dPrint("Manager details result".print_r($mgrdetails,true),4);
							$mgrId = !empty($mgrdetails[0]->id)  ? $mgrdetails[0]->id : '';
							if(empty($mgrId)){
								$query = "select if((t1.record_status=:record_status  or t2.record_status = :record_status ) ,:value  ,t1.mapping_id) 
													from ".$jobDetail['sTable']." t1 
													left join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
													where t1.user_name =  :user_name   LIMIT 1";
								expDebug::dPrintDBAPI(" mgrdetails 111 qry ",$query,array(':record_status'=> 'IN',':value'=>'invalid',':user_name'=>addslashes($mgr)));
								$this->db->callQuery($query,array(':record_status'=> 'IN',':value'=>'invalid',':user_name'=>addslashes($mgr)));
								$mgrId = $this->db->fetchColumn();	
								if($mgrId == 'invalid'){
									expDebug::dPrint(' Test 1111 ');
									$err .= 'Given dotted manager '.addslashes($mgr).'  does not exist as an user in LMS,  exist in feed, but is not a valid user (check error details of the manager user for more details). ';
								}else if(!empty($mgrId)){
									$query = "select if(t1.status != :sts ,:value , t1.is_manager ) from ".$jobDetail['sTable']." t1 
														left join ".$jobDetail['pTable']." t2 on t1.mapping_id = t2.mapping_id
														where t1.user_name =  :user_name ";
									$this->db->callQuery($query,array(':value' => 'inactive',':user_name'=>addslashes($mgr),':sts' => 'Active'));
									$sts = $this->db->fetchColumn();
									expDebug::dPrint(' Test 222 '.$sts,4);
									if($sts == 'inactive'){
										$err .= 'Given other manager '.addslashes($mgr).' exist as an user in Feed but is not active. ';
									}else if($sts != 'Y'){
										$err .= 'Given other manager '.addslashes($mgr).' exist as an user in Feed but is not manager. ';
									}
									$fields['operation'] = ':operation ';
									$args[':operation'] = 'upsert';
								}else{
									expDebug::dPrint(' Test 333 ');
									$err .= 'Given other manager '.addslashes($mgr).' does not exist as an user in both LMS and feed. ';
								}
							}else if($mgrId == 'inactive'){
								if($mgrdetails[0]->userexist == 'exist')
									$err .= 'Given other manager '.addslashes($mgr).' exist as an user in LMS and Feed but is not active. ';
								else
									$err .= 'Given other manager '.addslashes($mgr).' exist as an user in LMS but is not active. ';
							}else{
								if($mgrdetails[0]->is_manager == 'Y' && (($mgrdetails[0]->userexist == 'exist' && $mgrdetails[0]->tmgr == 'Y') || ($mgrdetails[0]->userexist == 'nonexist') )){
									$dottedmgrId .=  ($dottedmgrId == '') ? $mgrId : ','.$mgrId;
								}else if($mgrdetails[0]->is_manager == 'Y' && ($mgrdetails[0]->userexist == 'exist' && $mgrdetails[0]->tmgr != 'Y')){
									$err .= 'Given other manager '.addslashes($mgr).' exist as an user in LMS and Feed, but is not manager in Feed. ';
								}else{	
									$err .= 'Given other manager '.addslashes($mgr).' exist as an user in LMS but is not manager. ';
								}
							}  
							
					} 
							//Ganesh
							$err_mgr_access='';
							if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
								
							}else{
							    $err_mgr_access=$this->validateDottedManagerAccess($mgr,$rec->user_name,$jobDetail['uploaded_user_id']);
							}
							if($err_mgr_access!=''){
								$err .= $err_mgr_access;
							}
							
 				}//End for
 					expDebug::dPrint('$mgrdetails $err '.$err. '$dotted manager list'.$dottedmgrId);
					$fields['dotted_mngr_id'] = ':dotted_mngr_id ';
					$args[':dotted_mngr_id'] = $dottedmgrId;
				} 
				$rec->dotted_org_id=trim($rec->dotted_org_id);
				$rec->dotted_org_id = implode(',',array_unique(explode(',', $rec->dotted_org_id)));
				if(!empty($rec->dotted_org_id)){
					$dottedorg = explode(',',$rec->dotted_org_id);
					$dottedorg = array_map('trim',$dottedorg);
					$dottedorgId = '';
					foreach($dottedorg as $org){
						$query = "select id from slt_organization where number = :org_name  and status = :status  LIMIT 1";
						$this->db->callQuery($query,array(':org_name'=> addslashes($org),':status'=>'cre_org_sts_act'));
						$orgId = $this->db->fetchColumn();
						if(empty($orgId))
							$err .= 'Other Organization '.addslashes($org).' specified for the user does not exist in LMS. ' ; // Changed by ganeshbabuv on May 10th 2017 for #0077341
						else{ //Ganesh
							
							$err_org_access = '';
							
							if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
							
							}else{							
								$err_org_access=$this->validateDottedOrgAccess($org,$rec->user_name,$jobDetail['uploaded_user_id']);
							}
							
						    if($err_org_access!=''){
						    	$err .= $err_org_access;
						    }else{
								$dottedorgId .= ($dottedorgId == '') ? $orgId : ','.$orgId;
							} 
							
						}
					}
					$fields['dotted_org_id'] = ':dotted_org_id ';
					$args[':dotted_org_id'] = $dottedorgId;
					
				}
				$rec->job_role=trim($rec->job_role);
				$rec->job_role = implode(',',array_unique(explode(',', $rec->job_role)));
				if(!empty($rec->job_role)){
					$jobrole = explode(',',$rec->job_role);
					$jobrole = array_map('trim',$jobrole);
					$jobroleId = '';
					foreach($jobrole as $job){  //#0076229- Added by ganeshbabuv on Apr 26th 2017 for avoiding to place the job title instead of jobrole
						$query = "select code from slt_profile_list_items where code = :job_role and is_active = 'Y' and code like 'cre_usr_jrl%' LIMIT 1";
						$this->db->callQuery($query,array(':job_role'=> addslashes($job)));
						$jobId = $this->db->fetchColumn();
						if(empty($jobId))
							$err .= 'Job Role specified for the user does not exist in LMS. ' ;
						else{ //Ganesh 
							  
							$err_jr_access = '';
							
							if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
									
							}else{
								$err_jr_access=$this->validateDottedJobRoleAccess($jobId,$rec->user_name,$jobDetail['uploaded_user_id']);
							}
							 
						    if($err_jr_access!=''){
						    	$err .= $err_jr_access;
						    }else{
								$jobroleId .= ($jobroleId == '') ? $jobId : ','.$jobId;
							} 
							
						}
					}
					
					$fields['job_role'] = ':job_role ';
					$args[':job_role'] = $jobroleId;
						
				}
				
				$fields['custom4'] = 'concat(ifnull(custom4,""), :custom4 )';
				$fields['record_status'] = "if(".$configInvalid." = 1 and ".(empty($err) ? 0 : 1)." = 1, :recst ,record_status)"; 
				$args[':user_name'] = addslashes($rec->user_name);
				$args[':custom4'] = $err;
				$args[':recst'] = 'IN';
				$updQry = $this->db->prepareQueryUpdate($jobDetail['pTable'],$fields,array(" user_name = :user_name "));
				expDebug::dPrintDBAPI("bulkValidate else part ",$updQry,$args);
				$this->db->callExecute($updQry,$args);

			}
			
			//Manager and Other manager name Conflict in LMS
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
					't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.dotted_mngr_id != :nul ',
					't22.dotted_mngr_id IS NOT NULL',
					't22.primary_id IS NOT NULL',
					't22.primary_id != :nul ',
					'FIND_IN_SET(per.manager_id,t22.dotted_mngr_id)'
			);
			$join=array(
					'inner join slt_person per on per.user_name =t22.user_name',
					'inner join slt_person mng on mng.id =per.manager_id'
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul'=>'',
					':recst' => 'IN',
					':err' => 'User Manager cannot be an Other Manager for himself. '
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Manager rgsfgdfgdfg name check qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//Organization and Other organization name Conflict in LMS
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
					't22.record_status' => "if(".$configInvalid." = 1, :recst ,t22.record_status) "
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.dotted_org_id != :nul ',
					't22.dotted_org_id IS NOT NULL',
					't22.primary_id IS NOT NULL',
					't22.primary_id != :nul ',
					'FIND_IN_SET(per.org_id,t22.dotted_org_id)'
			);
			$join=array(
					'inner join slt_person per on per.user_name =t22.user_name',
					'inner join slt_organization org on org.id =per.org_id'
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul'=>'',
					':recst' => 'IN',
					':err' => 'User Organization cannot be an Other Organization for himself. '
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Manager rgsfgdfgdfg name check qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
						
			//SMS alert Validation
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "CASE WHEN t1.".$batchrecords['sms_alert']." NOT IN ( :y , :n )  THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN t1.".$batchrecords['sms_alert']." NOT IN ( :y , :n ) AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
					't22.sms_alert' => "CASE WHEN t1.".$batchrecords['sms_alert']." IN ( :y , :n ) THEN t1.".$batchrecords['sms_alert']." ELSE null  END"
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't1.'.$batchrecords['sms_alert'].' != :nul ',
					't1.'.$batchrecords['sms_alert'].' IS NOT NULL'
			);
			$join=array(
					'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':y' => 'Y',
					':n' => 'N',
					':nul'=>'',
					':recst' => 'IN',
					':err' => 'SMS alert field can only be Y or N. '
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" SMS alert Validation ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//SMS column change by vetrivel.P for #73139
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.sms_alert' => "CASE WHEN t1.".$batchrecords['sms_alert']." = :y THEN 1 ELSE 0  END"
			);
			$condition=array(
					't22.batch_id = :batch_id '
			);
			$join=array(
					'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':y' => 'Y'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("sms_alert change Code qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//WEBEX column change by vetrivel.P for #73139
			$qry = "select id as id from slt_profile_list_items where code = 'lrn_cls_vct_web' and is_active = 'Y'";
			expDebug::dPrint('Webex detail '. $qry);
			$this->db->callQuery($qry);
			$webex = $this->db->fetchAllResults();
			if(!empty($webex) && variable_get('webex_User') == 1){
				
				list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
				$fields=array(
				 		't22.webex_name' => "CASE WHEN t1.".$batchrecords['webex_name']." != :y THEN t1.webex_name ELSE NULL END"
				);
				$condition=array(
						't22.batch_id = :batch_id '
				);
				$join=array(
						'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
				);
				$arg = array(
						':batch_id' => $jobDetail['batchId'],
						':y'=>''
				);
				$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
				expDebug::dPrintDBAPI("Webex value qry1 ",$qry1,$arg);
				$this->db->callExecute($qry1,$arg);
				
				list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
				$fields=array(
						't22.webex_pass' => "CASE WHEN t1.".$batchrecords['webex_pass']." != :y THEN t1.webex_pass ELSE NULL END"
				);
				$condition=array(
						't22.batch_id = :batch_id '
				);
				$join=array(
						'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
				);
				$arg = array(
						':batch_id' => $jobDetail['batchId'],
						':y'=>''
				);
				$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
				expDebug::dPrintDBAPI("Webex value qry1 ",$qry1,$arg);
				$this->db->callExecute($qry1,$arg);
				
				$this->db->connect();
				//Added to encrypt the password if it is given in feed file. by vetrivel.P
				$query1 = $this->db->prepareQuerySelect($jobDetail['pTable'],array('webex_pass','user_name'),array('1=1'));
				$this->db->query($query1);
				$pass1 = $this->db->fetchAllResults();
				
				foreach($pass1 as $password1){
					if (!empty($password1->webex_pass)) {
						include_once DRUPAL_ROOT . "/sites/all/services/Encryption.php";
						$enc1 = new Encrypt();
						$encrypt = $enc1->encrypt($password1->webex_pass);
						$result = "update ".$jobDetail['pTable']." set webex_pass = :pass where user_name = :name ";
						$arg = array(
								':name'=>$password1->user_name,
								':pass'=>$encrypt
						);
						$this->db->callExecute($result,$arg);
					}
				}
			}elseif(!empty($webex) && variable_get('webex_User') != 1){ //Need to reject if the webex module is enabled at organization level.
				list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
				$fields=array(
						't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
						't22.record_status' => "if(".$configInvalid." = 1,:recst ,t22.record_status) "
				);
				$condition=array(
						't22.batch_id = :batch_id ',
						't1.webex_name != :nul and t1.webex_name is not null',
						't1.webex_pass != :nul and t1.webex_pass is not null'
				);
				$join=array(
						'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
				);
				$arg = array(
						':batch_id' => $jobDetail['batchId'],
						':nul'=>'',
						':recst' => 'IN',
						':err' => 'Webex credentials cannot be entered as Webex module is configured at the organization level. '
				);
				$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
				expDebug::dPrintDBAPI(" Webex module is in Organization state. ",$qry1,$arg);
				$this->db->callExecute($qry1,$arg);
			}elseif(empty($webex)){ //Need to reject if the webex module is in disabled status.
				list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
				$fields=array(
						't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
						't22.record_status' => "if(".$configInvalid." = 1,:recst ,t22.record_status) "
				);
				$condition=array(
						't22.batch_id = :batch_id ',
						't1.webex_name != :nul and t1.webex_name is not null',
						't1.webex_pass != :nul and t1.webex_pass is not null'
				);
				$join=array(
						'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
				);
				$arg = array(
						':batch_id' => $jobDetail['batchId'],
						':nul'=>'',
						':recst' => 'IN',
						':err' => 'Webex credentials cannot be entered as Webex module is disabled. '
				);
				$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
				expDebug::dPrintDBAPI(" Webex module is in Disable state. ",$qry1,$arg);
				$this->db->callExecute($qry1,$arg);
			}
			//Need to reject if webex module is enabled and is_instractor is N.
			if(!empty($webex)){
				list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
				$fields=array(
						't22.custom4' => "concat(ifnull(t22.custom4,''), :err ) ",
						't22.record_status' => "if(".$configInvalid." = 1,:recst ,t22.record_status) "
				);
				$condition=array(
						't22.batch_id = :batch_id ',
						'(t1.webex_name != :nul and t1.webex_name is not null)',
						't22.is_instructor != :y ',
						'(t1.webex_pass != :nul and t1.webex_pass is not null)'
				);
				$join=array(
						'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
				);
				$arg = array(
						':batch_id' => $jobDetail['batchId'],
						':nul'=>'',
						':recst' => 'IN',
						':y' => 'Y',
						':err' => 'Webex credentials cannot be entered for users without Instructor role. '
				);
				$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
				expDebug::dPrintDBAPI("User dont have instractor role ",$qry1,$arg);
				$this->db->callExecute($qry1,$arg);
			}
			//Mobil Admin column change by vetrivel.P for #73139
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "CASE WHEN t1.".$batchrecords['is_mobileadmin']." NOT IN ( :y , :n )  THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN t1.".$batchrecords['is_mobileadmin']." NOT IN ( :y , :n ) AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
					't22.is_mobileadmin' => "CASE WHEN t1.".$batchrecords['is_mobileadmin']." IN ( :y , :n ) THEN t1.".$batchrecords['is_mobileadmin']." ELSE null  END"
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't1.'.$batchrecords['is_mobileadmin'].' != :nul ',
					't1.'.$batchrecords['is_mobileadmin'].' IS NOT NULL'
			);
			$join=array(
					'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':y' => 'Y',
					':n' => 'N',
					':nul'=>'',
					':recst' => 'IN',
					':err' => 'IsMobilAdmin field can only be Y or N. '
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("is_mobileadmin change Code qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//Mobil Admin column change by vetrivel.P for #73139
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.is_mobileadmin' => "CASE WHEN t1.".$batchrecords['is_mobileadmin']." = :y THEN 1 ELSE 0 END"
			);
			$condition=array(
					't22.batch_id = :batch_id '
			);
			$join=array(
					'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':y' => 'Y'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI("is_mobileadmin change Code qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			
			//Chat signed in Validation
			/* list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.custom4' => "CASE WHEN t1.".$batchrecords['chat_signed_in']." NOT IN ( :y , :n )  THEN concat(ifnull(t22.custom4,''), :err ) ELSE t22.custom4 END",
					't22.record_status' => "CASE WHEN t1.".$batchrecords['chat_signed_in']." NOT IN ( :y , :n ) AND ".$configInvalid." = 1 THEN :recst ELSE t22.record_status END",
					't22.chat_signed_in' => "CASE WHEN t1.".$batchrecords['chat_signed_in']." IN ( :y , :n ) THEN t1.".$batchrecords['chat_signed_in']." ELSE :n  END"
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't1.'.$batchrecords['chat_signed_in'].' != :nul ',
					't1.'.$batchrecords['chat_signed_in'].' IS NOT NULL'
			);
			$join=array(
					'left join '.$jobDetail['sTable'].' t1 on t1.mapping_id =t22.mapping_id'
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':y' => '1',
					':n' => '0',
					':nul'=>'',
					':recst' => 'IN',
					':err' => 'Chat signed in field can only be 0 or 1.\r'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Chat signed in Validation ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg); */
			
			//Full name update
			$qry = "update ".$jobDetail['pTable']." set full_name = REPLACE(concat(first_name,' ',last_name), '\'', '''') where batch_id =".$jobDetail['batchId'];
			expDebug::dPrint(' Full name update '. $qry);
			$this->db->callExecute($qry);
                        
                        //Added by Subin #70899 - User address Verification
                        require_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_sitesetup/exp_sp_administration_module_info/exp_sp_administration_module_info.inc';
                        require_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_core/modules/exp_sp_usps_integration/exp_sp_usps_integration.module';
                        $isUSPSModuleEnable = getDrupalModuleStatus(array('exp_sp_usps_integration'));
                        
                        $is_verified_add = 'No';
                        $is_valid_add = '';
                        $invalid_add_reason = '';
                        $updCountry ='';
                        if($isUSPSModuleEnable == 1){
                            $updCountry = "and country!='US'";
                        }
                        $updAddVal = "update ".$jobDetail['pTable']."  set is_verified_add = '".$is_verified_add."' , is_valid_add= '".$is_valid_add."' , invalid_add_reason= '".$invalid_add_reason."' where record_status='R' ".$updCountry;
                        expDebug::dPrintDBAPI("Query for slt person bulk validate fileds fetch ", $updAddVal);
                        $this->db->callExecute($updAddVal);
                        if($isUSPSModuleEnable == 1){
                                $this->db->connect();
                                $this->db->beginTrans();

                                $arg=array(':bthid' => $jobDetail['batchId'],':sts'=>'R',':cntry'=>'US');
                                $query = $this->db->prepareQuerySelect($jobDetail['pTable'],array('user_name','addr1','addr2','city','state','country','zip'),array('country= :cntry ','record_status= :sts ','batch_id= :bthid '));
                                expDebug::dPrintDBAPI("Query for slt person bulk validate fileds fetch ", $query,$arg);
                                $this->db->query($query,$arg);
                                $getSqlResult = $this->db->fetchAllResults();

                                         foreach ( $getSqlResult as $VResult ) {
                                                        $address_info = array(
                                                                        'apt' => trim($VResult->addr1),
                                                                        'address' => trim($VResult->addr2),
                                                                        'city' => trim($VResult->city),
                                                                        'state' => trim($VResult->state),
                                                                        'zip5' => trim($VResult->zip),
                                                        );
                                                        if($address_info['apt']=='' && $address_info['address']=='' && $address_info['city']=='' && $address_info['state']=='' && $address_info['zip5']==''){
                                                            $addressVerify = '';
                                                        }else{
                                                        $addressVerify = exp_sp_usps_integration_verify_address($address_info);
                                                        }
                                                        $response = end($addressVerify);
                                                        $is_verified_add = (!empty($response)) ? 'Yes' : 'No';
                                                        $isuspsError = $response['Address']['Error']['Description'];
                                                        if(empty($isuspsError) && !empty($response)){
                                                            $resCity = strtolower($response['Address']['City']);
                                                            $inpCity = strtolower($address_info['city']);
                                                            $isuspsError = ($resCity == $inpCity) ? '' : 'Invalid City';
                                                        }
                                                        $is_valid_add = (!empty($isuspsError)) ? 'Invalid' : (empty($response) ? '': 'Valid');
                                                        $invalid_add_reason = (!empty($isuspsError)) ? $isuspsError : null;

                                                        $tmpUpd = "update ".$jobDetail['pTable']."  set is_verified_add = '".$is_verified_add."' , is_valid_add= '".$is_valid_add."' , invalid_add_reason= '".$invalid_add_reason."' where user_name = '".$VResult->user_name."'";
                                                        $this->db->callExecute($tmpUpd);
                                         }
                                 }
                                 $this->db->connect();
                                 $this->db->beginTrans();
                                 //Added to encrypt the password if it is given in feed file. by vetrivel.P
                                 $this->db->query('select custom0 as pass, user_name as name, email as email from '.$jobDetail['pTable'].' where 1=1');
                                 $pass = $this->db->fetchAllResults();
                                 foreach($pass as $password){
                                 	if (!empty($password->pass)) {
                                 		require_once DRUPAL_ROOT . '/includes/password.inc';
                                 		$encrypt = user_hash_password(trim($password->pass));
                                 		$result = "update ".$jobDetail['pTable']." set custom0 = :pass where user_name = :name";
                                 		$arg = array(
                                 				':name'=>$password->name,
                                 				':pass'=>$encrypt
                                 		);
                                 		$this->db->callExecute($result,$arg);
                                 	}
                                 }
                        
            //Access Check for all the entity. 
			$this->userAccessCheck(); 
			
			// #0076069 - Added by ganesh for Apr 25th 2017 for Check the user privilesges for uploading user whether he has access or not for each users in batch table 
			$this->userPrivilegeCheck();
                                 
			//vetrivel - Adding / removing the groups from tmp table
			if ($this->checkFeedHasGroups($jobDetail)) {
			require_once('groups_dataload.php');
				$grp_obj = new groups_dataload();
			$grp_obj->bulkValidate($jobDetail,$batchrecords);
			}
			                     
			// Update notifyuser column for notification update in temp2 table
			$this->updateNofifyType();
			                     
            // Update remark and status from temp2 to temp1
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't11.remarks' => 'concat(ifnull(t11.remarks,""),t2.custom4)',
					't11.record_status' => 't2.record_status'
			);
			$condition=array(
			 		't2.batch_id = :batch_id '
			 );
			 $join=array(
			 		'left join '.$jobDetail['pTable'].' t2 on t2.mapping_id =t11.mapping_id ',
			 );
			 $arg = array(
			 		':batch_id' => $jobDetail['batchId']
			 );
			 $qry1 = $this->db->prepareQueryUpdate($jobDetail['sTable']." t11",$fields,$condition,$join);
			 expDebug::dPrintDBAPI(" Update remark and status from temp2 to temp1 ",$qry1,$arg);
			 $this->db->callExecute($qry1,$arg);

			// Update custom 4 column as null 
			$qry = "update ".$jobDetail['pTable']." set custom4 = null where batch_id = ".$jobDetail['batchId'];
			expDebug::dPrint(' Update custom 4 column as null '. $qry);
			$this->db->callExecute($qry); 
			
		}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in Validating the batch records for users",1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
				// TODO: throw exception
		}
	}
	
	/*
	 * Returns the default timezone
	 */
	function defaultTimeZone(){
		try{
		  $sysTimezone = date_default_timezone_get();
		  /*$selTimeZone = db_select('slt_profile_list_items', 'prf');
		  $selTimeZone->addField('prf','code','timezone_code');
		  $selTimeZone->condition('prf.attr2', $sysTimezone);
		  expDebug::dPrintDBAPI(' $selTimeZone obj = ' , $selTimeZone);
		  $defaultTimezone['time_zone'] = $selTimeZone->execute()->fetchField();
		  $defaultTimezone['sysTimezone'] = $sysTimezone;
		  expDebug::dPrint('param change==>>'.print_r($defaultTimezone,true),4);
		  return $defaultTimezone;*/
		  
		  $qry = "SELECT code FROM slt_profile_list_items WHERE attr2 = :tz ";
		  $this->db->query($qry,array(':tz'=>$sysTimezone));
		  $defaultTimezone['time_zone'] = $this->db->fetchColumn();
		  $defaultTimezone['sysTimezone'] = $sysTimezone;
		  expDebug::dPrint('param change==>>'.print_r($defaultTimezone,true),4);
		  return $defaultTimezone;
		  
		}catch(Exception $ex) {
	    	watchdog_exception('defaultSiteTimeZone', $ex);
	    	expertusErrorThrow($ex);
	  	}	
	}	 
 
	private function validateDottedJobRoleAccess($job_role_code,$user_name,$uploading_user_id,$skip_existing_user='1'){
					
		try{
		
				list($field, $condition,$join,$arg) = array(array(),array(),array(),array());
				
				$field= array(
							"upload_usr.user_name as user_name",
							"GROUP_CONCAT(grpmp.group_id) as group_id",
							"GROUP_CONCAT(grps.userslist) AS userlist "
					); 
					
				if($skip_existing_user=="1"){
					
						$condition=array(
								   "grpmp.entity_id IS NOT NULL 
									and per.code = :job_role_code 
									and grps.status='cre_sec_sts_atv'
									and usr_jr.id IS NULL 
									and (per.created_by != :uploading_user_id 
									and per.updated_by != :uploading_user_id)"
						);
						
						$join = array(
									'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_jrl\'',
									'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
									'LEFT JOIN slt_person usr ON usr.user_name = :user_name',
									'LEFT JOIN slt_person_jobrole_mapping usr_jr on usr_jr.user_id = usr.id and usr_jr.job_role=:job_role_code',
									'LEFT JOIN slt_person upload_usr ON upload_usr.id = :uploading_user_id',
									
						);
						
						
						$arg = array(':job_role_code' => $job_role_code,
									':uploading_user_id' => $uploading_user_id,
									':user_name' => $user_name
					    );
				
				}else{
					
						$condition=array(
								   "grpmp.entity_id IS NOT NULL 
									and per.code = :job_role_code 
									and grps.status='cre_sec_sts_atv' 
									and (per.created_by != :uploading_user_id 
									and per.updated_by != :uploading_user_id)"
						);
						
						$join = array(
									'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_jrl\'',
									'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id', 
									'LEFT JOIN slt_person upload_usr ON upload_usr.id = :uploading_user_id',
									
						);
						
						
						$arg = array(':job_role_code' => $job_role_code,
									':uploading_user_id' => $uploading_user_id								 
					    );
				
				}
				
				$other = 'GROUP BY upload_usr.user_name HAVING FIND_IN_SET( :uploading_user_id,IFNULL(userlist,0)) <=0';  
				 
				$jr_access_qry = $this->db->prepareQuerySelect('slt_profile_list_items per',$field,$condition,$join,$other);
				expDebug::dPrintDBAPI('JobRole access query Check - $jr_access_qry',$jr_access_qry,$arg);
				$this->db->callQuery($jr_access_qry,$arg);
				$jr_access_qry_res = $this->db->fetchAllResults();
				expDebug::dPrint('JobRole access query Check result - $jr_access_qry_res - '.print_r($jr_access_qry_res,1),5); 
				 
				$err_str='';
				if(count($jr_access_qry_res)>0){
					$tmp_user_name=trim($jr_access_qry_res[0]->user_name);
					if($tmp_user_name!=''){
						$err_str='Access to this Job Role '.$job_role_code.' is restricted.';
					}
				}
	
			   expDebug::dPrint('$err_str - '.print_r($err_str,1),5); 
		       return $err_str;
		   }catch (Exception $ex){
				expDebug::dPrint("PDO Error in validateDottedJobRoleAccess ",1);
				throw new PDOException($ex->getMessage());
		  }
	}
	
	
	private function validateDottedManagerAccess($manager_user_name,$user_name,$uploading_user_id,$skip_existing_user='1'){
		
			 try{
			
						list($field, $condition,$join,$arg) = array(array(),array(),array(),array());
						
						$field= array(
									"upload_usr.user_name as user_name",
									"GROUP_CONCAT(grpmp.group_id) as group_id",
									"GROUP_CONCAT(grps.userslist) AS userlist "
							); 
							
						if($skip_existing_user=="1"){
							
								$condition=array(
										   "grpmp.entity_id IS NOT NULL 
											and mgr_per.user_name = :manager_user_name 
											and grps.status='cre_sec_sts_atv'
											and usr_oth_mgr.id IS NULL 
											and (mgr_per.created_by != :uploading_user_id 
											and mgr_per.updated_by != :uploading_user_id)"
								);
								
								$join = array(
											'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = mgr_per.id AND grpmp.entity_type = \'cre_usr\'', 
											'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id', 
											'LEFT JOIN slt_person usr ON usr.user_name =  :user_name', 
											'LEFT JOIN slt_person_other_manager usr_oth_mgr on usr_oth_mgr.user_id = usr.id and usr_oth_mgr.manager_id=mgr_per.id',
											'LEFT JOIN slt_person upload_usr ON upload_usr.id =  :uploading_user_id',
								);
								
								
								$arg = array(':manager_user_name' => $manager_user_name,
											':uploading_user_id' => $uploading_user_id,
											':user_name' => $user_name								 
							    );
						
						}else{
							
								 $condition=array(
										   "grpmp.entity_id IS NOT NULL 
											and mgr_per.user_name = :manager_user_name 
											and grps.status='cre_sec_sts_atv' 
											and (mgr_per.created_by != :uploading_user_id 
											and mgr_per.updated_by != :uploading_user_id)"
								);
								
								$join = array(
											'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = mgr_per.id AND grpmp.entity_type = \'cre_usr\'', 
											'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',  
											'LEFT JOIN slt_person upload_usr ON upload_usr.id =  :uploading_user_id',
								);
								
								
								$arg = array(':manager_user_name' => $manager_user_name,
											':uploading_user_id' => $uploading_user_id
							    );
						
						}
						
						$other = 'GROUP BY upload_usr.user_name HAVING FIND_IN_SET( :uploading_user_id,IFNULL(userlist,0)) <=0';  
						 
						$mgr_access_qry = $this->db->prepareQuerySelect('slt_person mgr_per',$field,$condition,$join,$other);
						
						expDebug::dPrintDBAPI('Dotted Manager access query Check - $mgr_access_qry',$mgr_access_qry,$arg);
						$this->db->callQuery($mgr_access_qry,$arg);
						$mgr_access_qry_res = $this->db->fetchAllResults();
						expDebug::dPrint('Dotted Manager access query Check result - $mgr_access_qry_res - '.print_r($mgr_access_qry_res,1),5); 
						 
						$err_str='';
						if(count($mgr_access_qry_res)>0){
							$tmp_user_name=trim($mgr_access_qry_res[0]->user_name);
							if($tmp_user_name!=''){
								$err_str='Access to this Other Manager '.$manager_user_name.' is restricted.';
							}
						}
			
					   expDebug::dPrint('$err_str - '.print_r($err_str,1),5); 
				       return $err_str;
		       }catch (Exception $ex){
					expDebug::dPrint("PDO Error in validateDottedManagerAccess",1);
					throw new PDOException($ex->getMessage());
				}
	}
	
	private function validateDottedOrgAccess($org_code,$user_name,$uploading_user_id,$skip_existing_user='1'){
		
			 try{
			
						list($field, $condition,$join,$arg) = array(array(),array(),array(),array());
						
						$field= array(
									"upload_usr.user_name as user_name",
									"GROUP_CONCAT(grpmp.group_id) as group_id",
									"GROUP_CONCAT(grps.userslist) AS userlist "
							); 
							
						if($skip_existing_user=="1"){
							
								$condition=array(
										   "grpmp.entity_id IS NOT NULL 
											and org.number = :org_code 
											and grps.status='cre_sec_sts_atv'
											and usr_oth_org.id IS NULL 
											and (org.created_by != :uploading_user_id 
											and org.updated_by != :uploading_user_id)"
								);
								
								$join = array(
											'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = org.id AND grpmp.entity_type = \'cre_org\'', 
											'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id', 
											'LEFT JOIN slt_person usr ON usr.user_name =  :user_name', 
											'LEFT JOIN slt_person_other_organization usr_oth_org on usr_oth_org.user_id = usr.id and usr_oth_org.organization_id=org.id',
											'LEFT JOIN slt_person upload_usr ON upload_usr.id =  :uploading_user_id'										 
								);
								
								
								$arg = array(':org_code' => $org_code,
											':uploading_user_id' => $uploading_user_id,
											':user_name' => $user_name
							    );
						
						}else{
							
								 $condition=array(
										   "grpmp.entity_id IS NOT NULL 
											and org.number = :org_code 
											and grps.status='cre_sec_sts_atv' 
											and (org.created_by != :uploading_user_id 
											and org.updated_by != :uploading_user_id)"
								);
								
								$join = array(
											'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = org.id AND grpmp.entity_type = \'cre_org\'', 
											'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
											'LEFT JOIN slt_person upload_usr ON upload_usr.id =  :uploading_user_id'										 
								);
								
								
								$arg = array(':org_code' => $org_code,
											':uploading_user_id' => $uploading_user_id
							    );
						
						}
						
						$other = 'GROUP BY upload_usr.user_name HAVING FIND_IN_SET( :uploading_user_id,IFNULL(userlist,0)) <=0';  
						 
						$org_access_qry = $this->db->prepareQuerySelect('slt_organization org',$field,$condition,$join,$other);
						
						expDebug::dPrintDBAPI('Dotted Org access query Check - $mgr_access_qry',$org_access_qry,$arg);
						$this->db->callQuery($org_access_qry,$arg);
						$org_access_qry_res = $this->db->fetchAllResults();
						expDebug::dPrint('Dotted Org access query Check result - $org_access_qry_res - '.print_r($org_access_qry_res,1),5); 
						 
						$err_str='';
						if(count($org_access_qry_res)>0){
							$tmp_user_name=trim($org_access_qry_res[0]->user_name);
							if($tmp_user_name!=''){
								$err_str='Access to this Other Organization '.$org_code.' is restricted.';
							}
						}
			
					   expDebug::dPrint('$err_str - '.print_r($err_str,1),5); 
				       return $err_str;
		       }catch (Exception $ex){
					expDebug::dPrint("PDO Error in validateDottedOrgAccess",1);
					throw new PDOException($ex->getMessage());
				}
	}
	
		
		
	 private function getDefaultLanguage(){
	               $qry = "SELECT ifnull(value,'') from variable where name = 'language_default' ";
	               $this->db->callQuery($qry);
	               $res = $this->db->fetchColumn();
	               return !empty($res)? unserialize($res)->language : 'en-us';
	       }

	public function execute($updVal){
		/** TODO: Need User Id to update
		 * Using 1 as user id for checking */
		expDebug::dPrint('%%%%%%%%%%%%%%%checking the execute time starting--->'.microtime(true),4);
		expDebug::dPrint("MAXIMUM EXECUTION TME LIMIT WITH IN USER ENTITY FILE".print_r($updVal,true));
		$i = 0;
		$j = 0;
		$form_state = array();
		expDebug::dPrint("ASDFGHJKL".$_SERVER['DOCUMENT_ROOT']);
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/bootstrap.inc';
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/common.inc';
		include_once $_SERVER['DOCUMENT_ROOT'].'/includes/database/database.inc';
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER['DOCUMENT_ROOT'].'/'.drupal_get_path('module', 'exp_sp_administration_user') .'/exp_sp_administration_user.inc');
		try{
			$form_state['values']['first_name'] = $updVal->first_name;
			$form_state['values']['last_name'] = $updVal->last_name;
			$form_state['values']['user_name'] = $updVal->user_name;
			$form_state['values']['password'] = $updVal->custom0;
			$form_state['values']['email'] = $updVal->email;
			$form_state['values']['status'] = $updVal->status;
			$form_state['values']['addr1'] = $updVal->addr1;
			$form_state['values']['addr2'] = $updVal->addr2;
			$form_state['values']['country'] = $updVal->country;
			$form_state['values']['state'] = $updVal->state;
			$form_state['values']['city'] = $updVal->city;
			$form_state['values']['zip'] = $updVal->zip;
			$form_state['values']['phone_no'] = $updVal->phone_no;
			$form_state['values']['mobile_no'] = $updVal->mobile_no;
			$form_state['values']['sms_alert'] = $updVal->sms_alert;
			$form_state['values']['manager_id'] = $updVal->manager_id;
			$form_state['values']['org_id'] = $updVal->org_id;
			$form_state['values']['usertype'] = $updVal->user_type;
			$form_state['values']['jobrole'] = $updVal->job_role;
			$form_state['values']['jobtitle'] = $updVal->job_title;
			$form_state['values']['empltype'] = $updVal->employment_type;
			$form_state['values']['employee_no'] = $updVal->employee_no;
			$form_state['values']['deptcode'] = $updVal->dept_code;
			$form_state['values']['other_managers'] = $updVal->dotted_mngr_id;
			$form_state['values']['other_organization'] = $updVal->dotted_org_id;
			$form_state['values']['preferred_timezone'] = $updVal->time_zone;
			$form_state['values']['preferred_language'] = $updVal->preferred_language;
			$form_state['values']['preferred_currency'] = $updVal->preferred_currency;
			$form_state['values']['hire_date'] = $updVal->hire_date;
			$form_state['values']['custom_dataload'] = $updVal->batch_id;
			$form_state['values']['callfrom'] = 'DL';
			$createuserid = 'Feed';
			$updateUserId = 'Feed';
			
			$form=array();
			$form["form_id"]="";
			$form_state['storage']['acvalues']['manager']['id'] = $updVal->manager_id;
			$form_state['storage']['acvalues']['org']['id'] = $updVal->org_id;
			$form_state['values']['omanagers'] = $updVal->dotted_mngr_id;
			$form_state['values']['oorgs'] = $updVal->dotted_org_id;

			if($updVal->operation =='insert' || $updVal->operation == 'upsert'){
				//36820: Irrelevant escape character is getting inserted on processing through API
				$form_state['values'] = array_map('stripslashes', $form_state['values']);
				$form_state['values']['roles']['is_manager'] = $updVal->is_manager;
				$form_state['values']['roles']['is_instructor'] = $updVal->is_instructor;
				try{
				$userId =  addNewUser($form,$form_state,$createuserid,0,'',1);
				}catch(Exception $ex){
					expDebug::dPrint('Error in add user'.$ex->getMessage());
				}
				if($userId){
					if($form_state['values']['status'] == 'cre_usr_sts_atv' && empty($form_state['values']['password'])){
						expDebug::dPrint(' addNewUserByRestAPI() Notification id '.$form_state['values']['email'] , 4);
						sendResetPasswordLink('register_admin_created', $form_state['values']['email']);
					}
					updateUserAddressDetails($form, $form_state, $updateUserId ,'Inserted');
					updateUserOrgDetails($form, $form_state, $updateUserId, $userId, 'Inserted', 1);
					expDebug::dPrint(' DAta load addNewUserByRestAPI created user:' . $userId ,4);
					return array((object)array('Id'=>$userId,'status'=>'success'));
				}
				else{
					return array((object)array('Id'=>'Failure','status'=>'Failure','remarks'=>''));
				}
			}else{
				$form_state['values']['id'] = $updVal->primary_id;
				$form_state['values']['roles']['is_manager'] = $updVal->is_manager;
				$form_state['values']['roles']['is_instructor'] = $updVal->is_instructor;
				try{
					$userId =  updateUserBasicDetails($form, $form_state, $updateUserId, 0,'',1);
				}catch(Exception $ex){
					throw new Exception("Error in batch Execution".$ex->getMessage());
					expDebug::dPrint('Error in add user'.$ex->getMessage());
				}
				if ($userId && $userId!='No change required') {
					$newStatus = $form_state['values']['status'];
					$currstatus = getPersonDetails($userId,array('status'));
				
					expDebug::dPrint('newstatus ='.$newStatus);
					expDebug::dPrint('currstatus ='.$currstatus['status']);
				
					if((!empty($newStatus)) && ($newStatus != $currstatus['status'])){
						activateOrDeactivatePeople($userId,$form_state['values']['status'],false,'Feed');
					}
				
					updateUserAddressDetails($form, $form_state, $updateUserId ,'Updated');
					updateUserOrgDetails($form, $form_state, $updateUserId, $form_state['values']['id'], 'Updated', 1);
					expDebug::dPrint(' updateUserByRestAPI updated user:' . $userId , 4);
					expDebug::dPrint("SEcurity Role :: !!!".$form_state['values']['securityrole']."!!!");
					
					//For Update Role End
					return array((object)array('Id'=>$userId,'status'=>'success'));
				}
				else if($userId =='No change required'){
					return array((object)array('Id'=>$form_state['values']['id'],'status'=>'success'));
				}
				else{
					return array((object)array('Id'=>'Failure','status'=>'Failure','remarks'=>''));
				}
			}
		try {
			// To sync solr
			syncSolrData('User');
		} catch(Exception $e) {
			// do nothing
		}
			expDebug::dPrint('%%%%%%%%%%%%%%%checking the execute time ending--->'.microtime(true),4);
		}catch (PDOException $e) {
			expDebug::dPrint("PDO Error in Job Execution of API".$e->getMessage(),1);
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			expDebug::dPrint('Error in the batch execution for each records'.$ex->getMessage());
			//throw new Exception("Error in batch Execution".$ex->getMessage());
		}
	} 
	public function updateJobrole($tblDet=array()){
		try{
			if($tblDet['oper'] == 'insert' || $tblDet['oper'] == 'update'){
				$this->db->connect();
				$this->db->beginTrans();
				try{
					//Fields to insert.
					$fields = array();
					$selFields = array();				
					$query = 'select id,manager_id,org_id,job_role,dotted_mngr_id,dotted_org_id,created_on,updated_on,created_by,updated_by from slt_person where custom_dataload ='.$tblDet['btc_id'].' AND (job_role is not null OR dotted_mngr_id is not null OR dotted_org_id is not null) AND (job_role !="" OR dotted_mngr_id!="" OR dotted_org_id!="")';
					expDebug::dPrint('JOBROLE--->$query'.print_r($query,1),4);
					$this->db->query('select id,manager_id,org_id,job_role,dotted_mngr_id,dotted_org_id,created_on,updated_on,created_by,updated_by from slt_person where custom_dataload ='.$tblDet['btc_id'].' AND (job_role is not null OR dotted_mngr_id is not null OR dotted_org_id is not null) AND (job_role !="" OR dotted_mngr_id!="" OR dotted_org_id!="")' );
					$joroleDetails = $this->db->fetchAllResults();
					$job_map_query = 'INSERT INTO slt_person_jobrole_mapping ( user_id, job_role, created_by, created_on, updated_by, updated_on) VALUES ';
					$mang_map_query = 'INSERT INTO slt_person_other_manager ( user_id, manager_id,is_direct, created_by, created_on, updated_by, updated_on) VALUES ';
					$org_map_query = 'INSERT INTO slt_person_other_organization ( user_id, organization_id, is_direct,created_by, created_on, updated_by, updated_on) VALUES ';
					$dot_mang_map_query = 'INSERT INTO slt_person_other_manager ( user_id, manager_id, created_by, created_on, updated_by, updated_on) VALUES ';
					$dot_org_map_query = 'INSERT INTO slt_person_other_organization ( user_id, organization_id, created_by, created_on, updated_by, updated_on) VALUES ';
					$val_str = '';
					$val_mngr_str = '';
					$val_org_str = '';
					$val_dot_mngr_str = '';
					$val_dot_org_str = '';
					$user_org_str = '';
					$user_str = '';
					$user_manr_str = '';
					$user_dot_manr_str = '';
					$user_dot_org_str = '';
					expDebug::dPrint('JOBROLE--->'.print_r($joroleDetails,1),4);
				
					foreach($joroleDetails as $val){
						if(!empty($val->job_role)){
							if(strpos($val->job_role,',') !== false){
								$job_role_arr = explode(',',$val->job_role);
								$job_role_arr = array_map('trim',$job_role_arr);
								foreach ($job_role_arr as $jobrole){
									if($val_str=='')
										$val_str .= '('.$val->id.', "'.$jobrole.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
									else
										$val_str .= ', ('.$val->id.', "'.$jobrole.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
								}
							}else{
								if($val_str=='')
										$val_str .= '('.$val->id.', "'.$val->job_role.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
									else
										$val_str .= ', ('.$val->id.', "'.$val->job_role.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
							}
							if(empty($user_str))
								$user_str .= $val->id;
							else
								$user_str .= ','.$val->id;
						}
						
						if(!empty($val->manager_id)){
							if($val_mngr_str==''){
								$val_mngr_str .= '('.$val->id.', "'.$val->manager_id.'", "Y", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
							}else{
								$val_mngr_str .= ', ('.$val->id.', "'.$val->manager_id.'", "Y", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
						}
							$user_manr_str = $val->id;
						}
						if(!empty($val->org_id)){
							if($val_org_str==''){
								$val_org_str .= '('.$val->id.', "'.$val->org_id.'", "Y", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
							}else{
								$val_org_str .= ', ('.$val->id.', "'.$val->org_id.'", "Y", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
						}
							$user_org_str = $val->id;
						}
						
						$val->dotted_mngr_id=trim($val->dotted_mngr_id);
						if(!empty($val->dotted_mngr_id)){
							if(strpos($val->dotted_mngr_id,',') !== false){
								$dotted_mngr_arr = explode(',',$val->dotted_mngr_id);
								$dotted_mngr_arr = array_map('trim',$dotted_mngr_arr);
								foreach ($dotted_mngr_arr as $dotted_mngr){
									if($val_dot_mngr_str=='')
										$val_dot_mngr_str .= '('.$val->id.', "'.$dotted_mngr.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
									else
										$val_dot_mngr_str .= ', ('.$val->id.', "'.$dotted_mngr.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
								}
							}else{
								if($val_dot_mngr_str=='')
									$val_dot_mngr_str .= '('.$val->id.', "'.$val->dotted_mngr_id.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
								else
									$val_dot_mngr_str .= ', ('.$val->id.', "'.$val->dotted_mngr_id.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
							}
							if(empty($user_dot_manr_str))
								$user_dot_manr_str .= $val->id;
							else
								$user_dot_manr_str .= ','.$val->id;
						}
						$val->dotted_org_id=trim($val->dotted_org_id);
						if(!empty($val->dotted_org_id)){
							if(strpos($val->dotted_org_id,',') !== false){
								$dotted_org_arr = explode(',',$val->dotted_org_id);
								$dotted_org_arr = array_map('trim',$dotted_org_arr);
								foreach ($dotted_org_arr as $dotted_org){
									if($val_dot_org_str=='')
										$val_dot_org_str .= '('.$val->id.', "'.$dotted_org.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
									else
										$val_dot_org_str .= ', ('.$val->id.', "'.$dotted_org.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
								}
							}else{
								if($val_dot_org_str=='')
									$val_dot_org_str .= '('.$val->id.', "'.$val->dotted_org_id.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
								else
									$val_dot_org_str .= ', ('.$val->id.', "'.$val->dotted_org_id.'", "'.$val->created_by.'", now(), '.$val->updated_by.', now() )';
							}
							if(empty($user_dot_org_str))
								$user_dot_org_str .= $val->id;
							else
								$user_dot_org_str .= ','.$val->id;
						}
					}
					if(!empty($user_str)){
						$delete_qry = 'delete from slt_person_jobrole_mapping where find_in_set(user_id,"'.$user_str.'")';
						expDebug::dPrint('JOBROLE $delete_qry--->'.print_r($delete_qry,1),4);
						$this->db->callExecute('delete from slt_person_jobrole_mapping where find_in_set(user_id,"'.$user_str.'")');
						$insert_qry = $job_map_query.' '.$val_str;
						expDebug::dPrint('JOBROLE $$insert_qry--->'.print_r($insert_qry,1),4);
						$this->db->callExecute($insert_qry);
						$this->db->callExecute('update slt_person set job_role= null where find_in_set(id,"'.$user_str.'")');
					}
					if(!empty($val_mngr_str)){
						$delete_qry = 'delete from slt_person_other_manager where is_direct=\'Y\' and user_id="'.$user_manr_str.'"';
						expDebug::dPrint('other Manager $delete_qry--->'.print_r($delete_qry,1),4);
						$this->db->callExecute('delete from slt_person_other_manager where is_direct=\'Y\' and user_id="'.$user_manr_str.'"');
						$insert_qry = $mang_map_query.' '.$val_mngr_str;
						expDebug::dPrint('Manager $$insert_qry--->'.print_r($insert_qry,1),4);
						$this->db->callExecute($insert_qry);
					}
					if(!empty($val_org_str)){
						$delete_qry = 'delete from slt_person_other_organization where is_direct=\'Y\' and user_id="'.$user_org_str.'"';
						expDebug::dPrint('other Manager $delete_qry--->'.print_r($delete_qry,1),4);
						$this->db->callExecute('delete from slt_person_other_organization where is_direct=\'Y\' and user_id="'.$user_org_str.'"');
						$insert_qry = $org_map_query.' '.$val_org_str;
						expDebug::dPrint('ORG $$insert_qry--->'.print_r($insert_qry,1),4);
						$this->db->callExecute($insert_qry);
					}
					if(!empty($user_dot_manr_str)){
						$delete_qry = 'delete from slt_person_other_manager where is_direct=\'N\' and find_in_set(user_id,"'.$user_dot_manr_str.'")';
						expDebug::dPrint('other Manager $delete_qry--->'.print_r($delete_qry,1),4);
						$this->db->callExecute('delete from slt_person_other_manager where is_direct=\'N\' and find_in_set(user_id,"'.$user_dot_manr_str.'")');
						$insert_qry = $dot_mang_map_query.' '.$val_dot_mngr_str;
						expDebug::dPrint('other Manage $$insert_qry--->'.print_r($insert_qry,1),4);
						$this->db->callExecute($insert_qry);
						$this->db->callExecute('update slt_person set dotted_mngr_id= null where find_in_set(id,"'.$user_dot_manr_str.'")');
					}
					if(!empty($user_dot_org_str)){
						$delete_qry = 'delete from slt_person_other_organization where is_direct=\'N\' and find_in_set(user_id,"'.$user_dot_org_str.'")';
						expDebug::dPrint('other Manager $delete_qry--->'.print_r($delete_qry,1),4);
						$this->db->callExecute('delete from slt_person_other_organization where is_direct=\'N\' and find_in_set(user_id,"'.$user_dot_org_str.'")');
						$insert_qry = $dot_org_map_query.' '.$val_dot_org_str;
						expDebug::dPrint('other ORG $$insert_qry--->'.print_r($insert_qry,1),4);
						$this->db->callExecute($insert_qry);
						$this->db->callExecute('update slt_person set dotted_org_id= null where find_in_set(id,"'.$user_dot_org_str.'")');
					}
					//$this->db->commitTrans();
					$this->db->closeconnect();
				}catch(Exception $e){
					$this->db->rollbackTrans();
					$this->db->closeconnect();
				}
			}
		}catch(Exception $ex){
				expDebug::dPrint('Error in users insert in users Job Role Mapping table===>'.$ex->getMessage(),1);
		}
	}
	public function bulkExecute($tblDet=array()){
		
		try{
			expDebug::dPrint('BULK EXCECUTE DETAILS--->'.print_r($tblDet,1),4);
			$this->setTableDetails($tblDet);
			
			//Need to reset the manager profile if change from Y to N.
			$this->updateManagerProfile($tblDet);
			
			if($tblDet['oper'] == 'insert' || $tblDet['oper'] == 'upsert'){
				$this->db->connect();
				$this->db->beginTrans();
				try{
					
				//Fields to insert.
				$fields = array();
				$selFields = array();
				$createdBy = array('created_on','created_by','updated_on','updated_by','custom_dataload','dataload_by','is_verified_add','is_valid_add','invalid_add_reason');
				$createdByArgs = array('now()', ':by ','now()', ':updby ',':cust ',':dlby ','pers.is_verified_add ','pers.is_valid_add ','pers.invalid_add_reason ');
				$arg=array(':entid' => $tblDet['ent_id'],':col'=>'%custom%', ':web'=>'%webex_%');
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col ','map.base_column not like :web'));
                expDebug::dPrintDBAPI("Query for slt person bulk execute fileds fetch insert", $query,$arg);
                
                if(module_exists('exp_sp_administration_customattribute'))
                {
                /* Start for Custom Attribute */ //#custom_attribute_0078975
                    
                $argcustom=array(':entid' => $tblDet['ent_id'], ':enttype'=>'cre_usr' ,':col'=>'%custom%', ':web'=>'%webex_%'); //#custom_attribute_0078975
                $joins1 = array(" right Join slt_custom_attr cattr on cattr.id = map1.cattr_id and cattr.status='cre_cattr_sts_atv' ");

                $query1 = $this->db->prepareQuerySelect('slt_custom_attr_mapping map1',array('map1.entity_ref_tbl_col as base_column'),array('map1.entity_type= :enttype ','map1.entity_screen_opt = 1'),$joins1);
                
                expDebug::dPrintDBAPI("Query for slt person bulk execute fileds fetch insert>> ", $query1,$argcustom);
                
                $selcusattr = "$query UNION ($query1)";
                
                $this->db->query($selcusattr,$argcustom);
                /* end for Custom Attribute */
                                }
                else {
				$this->db->query($query,$arg);
                }
                    $columnDetails = $this->db->fetchAllResults();
                    expDebug::dPrint('$columnDetails'.print_r($columnDetails,1),5);
				
				foreach($columnDetails as $col){
					$fields[] = $col->base_column;
					$selFields[] = 'pers.'.$col->base_column;
				}
				
				$tbl = $tblDet['tblname'];
				$insert = array();
				$insert['table'] = 'slt_person';
				$insert['fields'] = array_merge($fields,$createdBy);

				$select = array();
				$select['table'] = "$tbl pers";
				$select['fields']= array_merge($selFields,$createdByArgs);
					
				$select['condition'] = array(
						'pers.batch_id = :btid ',
						'pers.record_status = :r ',
						'(pers.operation = :opr OR pers.operation = :oprsts )'
				);
				$arg=array(
						':btid' => $tblDet['btc_id'],
						':opr' => $tblDet['oper'],
						':oprsts' => 'upsert',
						':r' => 'R',
						':by'=>$tblDet['user_id'],
						':updby' => $tblDet['user_id'],
						':cust' => $tblDet['btc_id'],
						':dlby' => $tblDet['loadby'],
				);
                                
				$query = $this->db->prepareInsertBySelect($insert,$select);
				expDebug::dPrintDBAPI("Query for slt person bulk execute", $query,$arg);
				$this->db->execute($query,$arg);
				$this->db->commitTrans();
				$this->db->closeconnect();
				}catch(Exception $e){
					$this->db->rollbackTrans();
					$this->db->closeconnect();
				}
				$this->db->connect();
				$this->db->beginTrans();
				try{
					
					$sel = "select max(p1.uid) from users p1;";
					$this->db->query($sel);
					$res = $this->db->fetchColumn();
					
					$sesVar = "SET @s = null;";
					$this->db->query($sesVar);
					
					$selQuery = "insert into users(uid,name,pass,mail,language,timezone,status)
					SELECT  @s := ifnull(@s, :uid  )+1 as uid,user_name as name,custom0 as pass,email as mail,custom1 as language,custom2 as timezone,if(status='cre_usr_sts_atv',1,0) as status
					FROM ".$tblDet['tblname']." where 
					batch_id= :btid and record_status = :r and (operation= :oper or operation= :upsr )";
					
					$args=array(
							':btid'=>$tblDet['btc_id'],
							':oper'=>'insert',
							':upsr'=>'upsert',
							':r'=>'R',
							':uid' => $res);
					expDebug::dPrintDBAPI('Query for User Bulk execute',$selQuery,$args);
					$this->db->execute($selQuery,$args);
					$this->db->commitTrans();
					$this->db->closeconnect();
					$this->updatewebex($tblDet);
					
				}catch(Exception $ex){
					$this->db->rollbackTrans();
					$this->db->closeconnect();
					expDebug::dPrint('Error in users insert in users table===>'.$ex->getMessage(),1);
				}
				$this->db->connect();
				$this->db->beginTrans();
				try{	
							
					$selQueryy = 'insert into slt_mapped_entities(entity_id,entity_type,group_id,owner_id,created_by,created_on,updated_by,updated_on)
                    select id,"cre_usr",0,updated_by,created_by,created_on,updated_by,updated_on 
                    FROM slt_person where 
					status != "cre_usr_sts_del" AND 
                    custom_dataload ='.$tblDet['btc_id'].'';
					expDebug::dPrintDBAPI('Insert query for mapped entities',$selQueryy);
					$this->db->execute($selQueryy);
					$this->db->commitTrans();
					$this->db->closeconnect();
				}catch(Exception $ex){
					$this->db->rollbackTrans();
					$this->db->closeconnect();
					expDebug::dPrint('Error in users insert in mapped entities table===>'.$ex->getMessage(),1);
				}
				
			}else{
				
                //Field to be updated #custom_attribute_0078975
                $arg=array(':entid' => $tblDet['ent_id'], ':col'=>'%custom%', ':unq'=>'unique');
                
				$query = $this->db->prepareQuerySelect('slt_dataload_table_mapping map',array('map.base_column'),array('map.entity_id= :entid ','map.base_column not like :col ','map.key_column != :unq '));
                expDebug::dPrintDBAPI("Query for slt person bulk execute fields fetch update", $query,$arg);
                
				$this->db->connect();
                if(module_exists('exp_sp_administration_customattribute'))
                {
                // fields for custom attributes #custom_attribute_0078975
                $argcustom=array(':entid' => $tblDet['ent_id'], ':enttype'=>'cre_usr',':col'=>'%custom%', ':unq'=>'unique'); //#custom_attribute_0078975
				
                $joins1 = array(" right Join slt_custom_attr cattr on cattr.id = map1.cattr_id and cattr.status='cre_cattr_sts_atv' ");
			
                $query1 = $this->db->prepareQuerySelect('slt_custom_attr_mapping map1',array('map1.entity_ref_tbl_col as base_column'),array('map1.entity_type= :enttype ','map1.entity_screen_opt = 1'),$joins1);
                
                expDebug::dPrintDBAPI("Query for slt person bulk execute fileds fetch update>> ", $query1,$argcustom);
                
                $selcusattr = "$query UNION ($query1)";
                
                $this->db->query($selcusattr,$argcustom);
                }
                else {
                    $this->db->query($query,array(':entid' => $tblDet['ent_id'],':col'=>'%custom%',':unq'=>'unique'));
                }
				$columnDetails = $this->db->fetchAllResults();
				$this->db->closeconnect();
				foreach($columnDetails as $col){
					//$fields['pers.'.$col->base_column] = 'x.'.$col->base_column;
					$fields['pers.'.$col->base_column] = "if(x.$col->base_column != '', x.$col->base_column, pers.$col->base_column )";	
	
				}
				$fields['pers.updated_on'] ='now()';
				$fields['pers.updated_by']= ':uid';
				$fields['pers.custom_dataload']= ':cust';
                                $fields['pers.is_verified_add']= 'x.is_verified_add';
                                $fields['pers.is_valid_add']= 'x.is_valid_add';
                                $fields['pers.invalid_add_reason']= 'x.invalid_add_reason';
		
				$condition = array('pers.user_name=x.user_name','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ');
				$join = array(" left join ".$tblDet['tblname']." x on x.user_name = pers.user_name");
				$args = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':uid'=>$tblDet['user_id'],':cust'=>$tblDet['btc_id'],':r'=>'R');
				$persQry = $this->db->prepareQueryUpdate('slt_person pers ',$fields,$condition,$join);
				$this->db->connect();
				expDebug::dPrintDBAPI('person bulk update  ',$persQry,$args);
				$this->db->execute($persQry,$args);
				$this->db->closeconnect();
								
				$fields_webex['pers.webex_name'] ='if(x.webex_name != "", x.webex_name, pers.webex_name)';
				$fields_webex['pers.webex_pass']= 'if(x.webex_pass != "", x.webex_pass, pers.webex_pass)';
				$condition1 = array('pers.user_name=x.user_name','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ', 'pers.is_instructor = "Y"');
				$arg = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':r'=>'R');
				$joins = array(" inner join ".$tblDet['tblname']." x on x.user_name = pers.user_name");
				$perswebexQry = $this->db->prepareQueryUpdate('slt_person pers ',$fields_webex,$condition1,$joins);
				expDebug::dPrintDBAPI('person bulk update  ',$perswebexQry,$arg);
				$this->db->connect();
				$this->db->execute($perswebexQry,$arg);
				$this->db->closeconnect();
				
				/* TODO: TO BE REMOVED 
				 * "(select custom0 as pass, email as mail, custom1 as timezone, custom2 as language,user_name as name,batch_id,record_status,operation
				from ".$tblDet['tblname']."
				where batch_id =".$tblDet['btc_id']." and operation='update')x"; */
				
				$field = array('usr.pass' => 'if(x.custom0 !=\'\' , x.custom0, usr.pass)','usr.mail' => 'x.email','usr.status' => 'if(x.status = \'cre_usr_sts_atv\', 1, 0 )','usr.language' => 'if(x.custom1!=\'\' , x.custom1, usr.language)', 'usr.timezone' => 'if(x.custom2!=\'\' , x.custom2, usr.timezone)');
				$condition = array('usr.name=x.user_name','x.batch_id= :btid ','x.record_status = :r ','x.operation= :oper ');
				$join = array(" left join ".$tblDet['tblname']." x on x.user_name = usr.name");
				$args = array(':btid'=>$tblDet['btc_id'],':oper'=>'update',':r'=>'R');
				$usrQry = $this->db->prepareQueryUpdate('users usr ',$field,$condition,$join);
				$this->db->connect();
				expDebug::dPrintDBAPI('User bulk update ',$persQry,$args);
				$this->db->execute($usrQry,$args);
				$this->db->closeconnect();

				$mapqry = 'delete ent
									from slt_mapped_entities ent
									INNER join slt_person per ON ent.entity_id = per.id
									INNER join '.$tblDet['tblname'].' x on x.user_name = per.user_name
									where x.status = "cre_usr_sts_del" and x.batch_id = '.$tblDet['btc_id'].'
									and x.record_status = \'R\' and x.operation= \'update\'
									AND ent.entity_type = "cre_usr" ';
				$this->db->connect();
				expDebug::dPrintDBAPI('mapped entities bulk delete query ',$mapqry);
				$this->db->execute($mapqry);
				$this->db->closeconnect();
				

			}
			//Code for Multiple Job Role insert to slt_person_jobrole_mapping table
			$this->updateDeletedUser($tblDet); //Added by vetrivel for delete the user.
			$this->updateJobrole($tblDet); 
			$this->populateFlateTableData($tblDet);
			
			//Ganesh - Adding / removing the groups from tmp table 
			if ($this->checkFeedHasGroups($tblDet)) {
			require_once('groups_dataload.php'); 
				$grp_obj = new groups_dataload();
			$grp_obj->bulkExecute($tblDet); 
			} else {
				require_once('groups_dataload.php');
				$grp_obj = new groups_dataload();
				$grp_obj->refreshAdminGroupsUserLists($tblDet);
				$grp_obj->refreshAdminUserRoles($tblDet);
				$grp_obj->updateManagerInstructorInPersonTable();
				$grp_obj->updateMandatoryComplianceEnrollments($tblDet);
			}
			
			//Drupal uid update
			$this->drupalUidUpdate($tblDet);
			
			// Notification part
			$this->insertNotification();
			try{
				//To sync solr
				syncSolrData('User','','Bulk');
			}catch(Exception $e){
				//do nothing
			}
		}catch (PDOException $ex) {
			expDebug::dPrint("PDO Error in Executing the batch records for users",1);
			$this->db->rollbackTrans();
			$this->db->closeconnect();
			
			throw new PDOException($ex->getMessage());
		}catch(Exception $e){
			expDebug::dPrint('Error in the batch execution for bulk upload'.$e->getMessage(),1);
		}
		 
	}
	
	// Added by Vincent on 24 Jan, 2017 for #0071187 
	private function populateFlateTableData($tblDet){
		try{
			
			// Delete record in flat table slt_group_user_mapping before populate (if any)
			
			$qry = 'DELETE gmap FROM slt_group_user_mapping gmap 
					INNER JOIN slt_person per ON per.id=gmap.user_id
					WHERE per.custom_dataload ='.$tblDet['btc_id'].'
					';
			$this->db->connect();
			expDebug::dPrintDBAPI('Delete Query for slt_group_user_mapping ',$qry);
			$this->db->execute($qry);
			$this->db->closeconnect();
			
			// Populate flat table slt_group_user_mapping for learner groups
			$qry = '
				INSERT INTO slt_group_user_mapping (user_id, user_status, group_id, group_type, group_status,user_type, created_by, created_on, updated_by, updated_on) 
				SELECT DISTINCT per.id AS user_id, per.status AS user_status, grp.id AS group_id, grp.is_admin AS group_type, grp.status AS group_status,IF(FIND_IN_SET(per.id, grp.added_users)>0,"A","M") AS user_type, grp.created_by AS created_by, grp.created_on AS created_on, grp.updated_by AS updated_by, grp.updated_on AS updated_on
				FROM 
				slt_groups grp
				INNER JOIN slt_person per
				LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
				LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
				WHERE  
				per.id NOT IN (1,2)
				AND per.custom_dataload ='.$tblDet['btc_id'].'
				AND grp.status != "cre_sec_sts_del"
				  AND per.status != \'cre_usr_sts_del\'
				  AND grp.is_admin = 0
				    AND (((if(grp.org_id=\'All\',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
				  	AND (if(grp.user_type=\'All\',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,\'\'),grp.user_type)>0 ))
				  	AND (if(grp.employment_type=\'All\',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,\'\'),grp.employment_type)>0))
				  	AND (if(grp.country=\'All\',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,\'\'),grp.country)>0))
				  	AND (if(grp.state=\'All\',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,\'-\',per.state),\'\'),grp.state)>0))
				  	AND (if(grp.department=\'All\',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,\'\'),grp.department)>0))
				  	AND (if(grp.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,\'\'),ifnull(grp.job_role,\'\'))>0))
				  	AND (if(grp.language=\'All\',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,\'\'),grp.language)>0))
				    AND (CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager or ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager)
				      WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      ELSE
				      1=1
				      END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)  )	
				  	AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)) 
			';
			$this->db->connect();
			expDebug::dPrintDBAPI('Insert Query for slt_group_user_mapping (learner) ',$qry);
			$this->db->execute($qry);
			$this->db->closeconnect();
			
			
			// Populate flat table slt_group_user_mapping for admin groups
			
			$qry = '
				INSERT INTO slt_group_user_mapping (user_id, user_status, group_id, group_type, group_status,user_type, created_by, created_on, updated_by, updated_on) 
				SELECT DISTINCT per.id AS user_id, per.status AS user_status, grp.id AS group_id, grp.is_admin AS group_type, grp.status AS group_status, IF(FIND_IN_SET(per.id, grp.added_users)>0,"A","M") AS user_type,grp.created_by AS created_by, grp.created_on AS created_on, grp.updated_by AS updated_by, grp.updated_on AS updated_on
				FROM 
				slt_groups grp
				INNER JOIN slt_person per
				LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
				LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
				WHERE  
				per.id NOT IN (1,2)
				AND per.custom_dataload ='.$tblDet['btc_id'].'
				AND grp.status != "cre_sec_sts_del"
				  AND per.status != \'cre_usr_sts_del\'
				  AND grp.is_admin = 1
				    AND (((if(grp.org_id=\'All\',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
				  	AND (if(grp.user_type=\'All\',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,\'\'),grp.user_type)>0 ))
				  	AND (if(grp.employment_type=\'All\',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,\'\'),grp.employment_type)>0))
				  	AND (if(grp.country=\'All\',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,\'\'),grp.country)>0))
				  	AND (if(grp.state=\'All\',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,\'-\',per.state),\'\'),grp.state)>0))
				  	AND (if(grp.department=\'All\',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,\'\'),grp.department)>0))
				  	AND (if(grp.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,\'\'),ifnull(grp.job_role,\'\'))>0))
				  	AND (if(grp.language=\'All\',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,\'\'),grp.language)>0))
				    AND (CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager or ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager)
				      WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      ELSE
				      1=1
				      END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1))	
				  	AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)) 
				  	AND ((grp.org_id is not null or
					grp.user_type is not null or
					grp.employment_type is not null or
					grp.country is not null or
					grp.state is not null or
					grp.department is not null or
					grp.job_role is not null or
					grp.language is not null or
					grp.is_manager != \'N\' or
					grp.is_instructor !=\'N\' or
					grpatt.id is not null or	
					(FIND_IN_SET(per.id,grp.added_users)>0) ))
			';
			$this->db->connect();
			expDebug::dPrintDBAPI('Insert Query for slt_group_user_mapping (admin) ',$qry);
			$this->db->execute($qry);
			$this->db->closeconnect();
			
			
			// Delete record in flat table slt_group_user_mapping before populate (if any)
			
			$qry = 'DELETE gmap FROM slt_admin_group_users gmap 
					INNER JOIN slt_person per ON per.id=gmap.user_id
					WHERE per.custom_dataload ='.$tblDet['btc_id'].'
					';
			$this->db->connect();
			expDebug::dPrintDBAPI('Delete Query for slt_admin_group_users ',$qry);
			$this->db->execute($qry);
			$this->db->closeconnect();
			
			// Populate flat table slt_admin_group_users for admin groups alone
			
			$qry = '
				INSERT INTO slt_admin_group_users (group_id, user_id, group_code, status, created_by, created_on, updated_by, updated_on) 
				SELECT DISTINCT grp.id AS group_id, per.id AS user_id, grp.code AS group_code, grp.status AS status, grp.created_by AS created_by, grp.created_on AS created_on, grp.updated_by AS updated_by, grp.updated_on AS updated_on
				FROM 
				slt_groups grp
				INNER JOIN slt_person per
				LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
				LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id 
				WHERE  
				per.id NOT IN (1,2)
				AND per.custom_dataload ='.$tblDet['btc_id'].'
				AND grp.status != "cre_sec_sts_del"
				  AND per.status != \'cre_usr_sts_del\'
				  AND grp.is_admin = 1
				    AND (((if(grp.org_id=\'All\',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
				  	AND (if(grp.user_type=\'All\',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,\'\'),grp.user_type)>0 ))
				  	AND (if(grp.employment_type=\'All\',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,\'\'),grp.employment_type)>0))
				  	AND (if(grp.country=\'All\',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,\'\'),grp.country)>0))
				  	AND (if(grp.state=\'All\',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,\'-\',per.state),\'\'),grp.state)>0))
				  	AND (if(grp.department=\'All\',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,\'\'),grp.department)>0))
				  	AND (if(grp.job_role=\'All\',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,\'\'),ifnull(grp.job_role,\'\'))>0))
				  	AND (if(grp.language=\'All\',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,\'\'),grp.language)>0))
				    AND (CASE WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager or ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      WHEN (grp.is_manager=\'Y\' AND grp.is_instructor=\'N\')
				      THEN
				      (ifnull(per.is_manager,\'N\') = grp.is_manager)
				      WHEN (grp.is_manager=\'N\' AND grp.is_instructor=\'Y\')
				      THEN
				      (ifnull(per.is_instructor,\'N\') = grp.is_instructor)
				      ELSE
				      1=1
				      END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,\'%Y-%m-%d\') <= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,\'%Y-%m-%d\') >= DATE_FORMAT(per.hire_date,\'%Y-%m-%d\'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,\'%Y-%m-%d\') between DATE_FORMAT(grpatt.between_start_date,\'%Y-%m-%d\') AND DATE_FORMAT(grpatt.between_end_date,\'%Y-%m-%d\'), 1=0),1=1)  )	
				  	AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)) 
				  	AND ((grp.org_id is not null or
					grp.user_type is not null or
					grp.employment_type is not null or
					grp.country is not null or
					grp.state is not null or
					grp.department is not null or
					grp.job_role is not null or
					grp.language is not null or
					grp.is_manager != \'N\' or
					grp.is_instructor !=\'N\'or
					grpatt.id is not null or	
					(FIND_IN_SET(per.id,grp.added_users)>0) )) 
			';
			
			// AND grp.code NOT IN (\'grp_ins\',\'grp_mgr\')
			
			$this->db->connect();
			expDebug::dPrintDBAPI('Insert Query for slt_admin_group_users (admin) ',$qry);
			$this->db->execute($qry);
			$this->db->closeconnect();
			
		}catch(Exception $ex){
			//$this->db->rollbackTrans();
			//$this->db->closeconnect();
			expDebug::dPrint('Error in users insert in mapped entities table===>'.$ex->getMessage(),1);
		}
	}
	//Added by vetrivel For #0073629 Checking data segmentation for user dataload.
	private function setBatchRecords($batchrecords){
		$this->batchrecords = $batchrecords;
	}
	
	private function getBatchRecords(){
		return $this->batchrecords;
	}
	
	protected function setJobDetails($jobDetail){
		$this->jobDetail = $jobDetail;
	}
	
	private function getJobDetails(){
		return $this->jobDetail;
	}
	protected function setTableDetails($tblDet){
		$this->tblDet = $tblDet;
	}
	
	private function getTableDetails(){
		return $this->tblDet;
	}
	
	
	// #0076069 - Added by ganesh for Apr 25th 2017 for Check the user privilesges for uploading user whether he has access or not for each users in batch table
	private function userPrivilegeCheck(){
		try{
			
		$jobDetail = $this->getJobDetails();

		// check user belongs to super admin group
		$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
		$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
		$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
		
		expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
		
		$this->db->callQuery($query,$args);
		$checkSupAdmin = $this->db->fetchColumn();
		expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
		if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
		
		}else{
			   
			    $batch_table=$jobDetail['pTable'];
				
				expDebug::dPrint('$batch_table='.$batch_table,5);
				
				
				$util = new GlobalUtil();
		  		$config = $util->getConfig();
		  		$configInvalid    = $config["skip_invalid_record"];  
						
						 $access_qry = "UPDATE ".$batch_table." bt1,

								(SELECT DISTINCT per.id AS id, 
								bt.user_name as user_name,
								bt.status as user_status,
								(CASE when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (per.created_by !=".$jobDetail['uploaded_user_id']." OR per.updated_by != ".$jobDetail['uploaded_user_id'].")
															THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_edit),0))
															FROM slt_group_privilege p
															left join slt_groups g on p.group_id=g.id
															left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_usr' and gm.group_type=1
															where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
															AND if(g.is_admin=1,p.page_code = 'cre_usr',p.page_code = 'cre_usr') AND gm.entity_id=per.id)
															ELSE CASE WHEN per.created_by = ".$jobDetail['uploaded_user_id']." OR per.updated_by = ".$jobDetail['uploaded_user_id']." OR
															((SELECT ifnull(max(p.priv_edit),0)
															FROM slt_group_privilege p
															left join slt_groups g on p.group_id=g.id
															where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
															AND if(g.is_admin=1,p.page_code = 'cre_usr',p.page_code = 'cre_usr')) > 0)
															THEN 1
															ELSE 0 END
															END ) AS sumEdit, (CASE
															when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (per.created_by != ".$jobDetail['uploaded_user_id'].")
															THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_delete),0))
															FROM slt_group_privilege p
															left join slt_groups g on p.group_id=g.id
															left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_usr' and gm.group_type=1
															where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
															AND if(g.is_admin=1,p.page_code = 'cre_usr',p.page_code = 'cre_usr') AND gm.entity_id=per.id)
															ELSE CASE WHEN per.created_by = ".$jobDetail['uploaded_user_id']." OR ((SELECT ifnull(max(p.priv_delete),0)
															FROM slt_group_privilege p
															left join slt_groups g on p.group_id=g.id
															where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
															AND if(g.is_admin=1,p.page_code = 'cre_usr',p.page_code = 'cre_usr')) > 0)
															THEN 1
															ELSE 0 END
															END ) AS sumDelete
								FROM 
								slt_person per FORCE INDEX (sli_per_stat)
								INNER JOIN ".$batch_table." bt on bt.user_name=per.user_name
								LEFT OUTER JOIN slt_person up_per ON up_per.id = per.id and up_per.id=".$jobDetail['uploaded_user_id']."
								LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = up_per.id
								LEFT OUTER JOIN slt_person_other_manager spom ON spom.user_id = per.id and spom.is_direct='N'
								LEFT OUTER JOIN slt_group_mapping group_map ON group_map.entity_id=per.id AND group_map.entity_type = 'cre_usr' and group_map.group_type = 1 AND 'cre_sec_sts_atv' = (SELECT status FROM slt_groups where id = group_map.group_id)
								LEFT OUTER JOIN slt_groups catacs ON catacs.id=group_map.group_id and (catacs.is_admin =1)
								LEFT OUTER JOIN slt_group_attributes grpatt ON catacs.id=grpatt.group_id
								LEFT OUTER JOIN slt_group_privilege priv ON priv.group_id = catacs.id and priv.page_code= 'cre_usr'
								WHERE  (per.status NOT IN  (\"cre_usr_sts_del\")) AND (per.user_name NOT IN  (\"admin\", \"guest\")) 
								AND (if(per.created_by =".$jobDetail['uploaded_user_id']." OR per.updated_by = ".$jobDetail['uploaded_user_id']." OR FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.removed_users) <= 0) 
								
								AND (if(catacs.code = 'grp_adm',FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.added_users)>0,1=1)) 
							
							
								AND (if(catacs.org_id='All',up_per.org_id is not null,catacs.org_id is null OR FIND_IN_SET(ifnull(up_per.org_id,''),catacs.org_id)>0))
							  	AND (if(catacs.user_type='All',up_per.user_type is not null,catacs.user_type is null OR FIND_IN_SET(ifnull(up_per.user_type,''),catacs.user_type)>0 ))
							  	AND (if(catacs.employment_type='All',up_per.employment_type is not null,catacs.employment_type is null OR FIND_IN_SET(ifnull(up_per.employment_type,''),catacs.employment_type)>0))
							  	AND (if(catacs.country='All',up_per.country is not null,catacs.country is null OR FIND_IN_SET(ifnull(up_per.country,''),catacs.country)>0))
							  	AND (if(catacs.state='All',up_per.state is not null,catacs.state is null OR FIND_IN_SET(ifnull(concat(up_per.country,'-',up_per.state),''),catacs.state)>0))
							  	AND (if(catacs.department='All',up_per.dept_code is not null,catacs.department is null OR FIND_IN_SET(ifnull(up_per.dept_code,''),catacs.department)>0))
							  	AND (if(catacs.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = up_per.id)>0,catacs.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(catacs.job_role,''))>0))
							  	AND (if(catacs.language='All',up_per.preferred_language is not null,catacs.language is null OR FIND_IN_SET(ifnull(up_per.preferred_language,''),catacs.language)>0))
							    AND (CASE WHEN (catacs.is_manager='Y' AND catacs.is_instructor='Y')
							      THEN
							      (ifnull(up_per.is_manager,'N') = catacs.is_manager or ifnull(up_per.is_instructor,'N') = catacs.is_instructor)
							      WHEN (catacs.is_manager='Y' AND catacs.is_instructor='N')
							      THEN
							      (ifnull(up_per.is_manager,'N') = catacs.is_manager)
							      WHEN (catacs.is_manager='N' AND catacs.is_instructor='Y')
							      THEN
							      (ifnull(up_per.is_instructor,'N') = catacs.is_instructor)
							      ELSE
							      1=1
							      END)
								AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(up_per.hire_date,'%Y-%m-%d'),1=0) OR 
			            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(up_per.hire_date,'%Y-%m-%d'), 1=0) OR
								if(grpatt.between_start_date is not null ,DATE_FORMAT(up_per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)  )								
								
								AND( (group_map.group_type = 1) OR (group_map.group_id IS NULL) ) 
								AND (if((catacs.is_admin = 1),FIND_IN_SET(".$jobDetail['uploaded_user_id'].",catacs.userslist)>0,1=1)))) 
								GROUP BY per.id) x
							
							SET bt1.custom4=if((x.sumEdit!='1' and ".$configInvalid."=1),concat(ifnull(bt1.custom4,''),:err_edit),if((x.sumDelete!='1' and x.user_status='cre_usr_sts_del' and ".$configInvalid."=1),concat(ifnull(bt1.custom4,''),:err_del), bt1.custom4)), 
							bt1.record_status=if((x.sumEdit!='1' and ".$configInvalid."=1),'IN',if((x.sumDelete!='1' and x.user_status='cre_usr_sts_del' and ".$configInvalid."=1),'IN', bt1.record_status)) 
							WHERE bt1.user_name=x.user_name"; 
							
							

								 
				  $arg = array(
								':err_edit'=>'You do not have privilege to edit this user',
								':err_del'=>'You do not have privilege to delete this user'							 
						     );   
				
				 expDebug::dPrintDBAPI('$access_qry = ',$access_qry);
				 expDebug::dPrint('$access_qry - $arg '.print_r($arg,true),5);
				
				 $this->db->callExecute($access_qry,$arg);  
				 
		}
		
		//Reject if user having active enrollment
		list($fields,$arg,$condition,$join) = array(array(),array(),array(),array());
		 $fields = array(
		 		"t22.custom4" => "concat(ifnull(t22.custom4,''), :err )",
		 		"t22.record_status" => ":recst "
		 );
		 $condition = array(
		 		't22.batch_id = :batch_id ', 
		 		't22.primary_id IS NOT NULL',
		 		't22.primary_id != :nul ',
		 		't22.status = :del ',
		 		't22.record_status != :recst ',
		 		'enr.comp_status = :enr '
		 );
		 $join = array(
		 		'left join slt_enrollment enr on enr.user_id = t22.primary_id'
		 );
		 $arg = array(
		 		':batch_id' => $jobDetail['batchId'],
		 		':recst' => 'IN',
		 		':nul'=>'',
		 		':del'=>'cre_usr_sts_del',
		 		':enr'=>'lrn_crs_cmp_enr',
		 		':err' => 'Cannot Delete the user since User having Active Enrollment.'
		 );
		 $qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
		 expDebug::dPrintDBAPI(" User Name Limit Validation ",$qry1,$arg);
		 $this->db->callExecute($qry1,$arg);
		
		}catch(Exception $e){
			expDebug::dPrint("ERROR in userPrivilegeCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
}

private function updateDeletedUser($tblDet){
	try{
		
		//Update deleted user in users table
		list($fields,$arg,$condition,$join) = array(array(),array(),array(),array());
		$fields = array(
				"usr.name" => "concat(usr.name,'-deleted-',per.id)",
				"usr.mail" => "concat(usr.mail,'-deleted-',per.id)"
		);
		$condition = array(
				't22.batch_id = :batch_id ',
				't22.status = :del ',
				't22.record_status = :res '
		);
		$join = array(
				'left join '.$tblDet['tblname'].' t22 on t22.user_name = usr.name',
				'inner join slt_person per on per.user_name = t22.user_name'
		);
		$arg = array(
				':batch_id' => $tblDet['btc_id'],
				':del'=>'cre_usr_sts_del',
				':res'=>'R'
		);
		$qry1 = $this->db->prepareQueryUpdate("users usr",$fields,$condition,$join);
		expDebug::dPrintDBAPI(" User updateDeletedUser users table ",$qry1,$arg);
		$this->db->callExecute($qry1,$arg);
		
		//Update deleted user in slt_person table
		list($fields,$arg,$condition,$join) = array(array(),array(),array(),array());
		$fields = array(
				"per.user_name" => "concat(per.user_name,'-deleted-',per.id)",
				"per.email" => "concat(per.email,'-deleted-',per.id)"
		);
		$condition = array(
				't22.batch_id = :batch_id ',
				't22.status = :del ',
				't22.record_status = :res '
		);
		$join = array(
				'left join '.$tblDet['tblname'].' t22 on t22.user_name = per.user_name'
		);
		$arg = array(
				':batch_id' => $tblDet['btc_id'],
				':del'=>'cre_usr_sts_del',
				':res'=>'R'
		);
		$qry1 = $this->db->prepareQueryUpdate("slt_person per",$fields,$condition,$join);
		expDebug::dPrintDBAPI(" User updateDeletedUser slt_person table ",$qry1,$arg);
		$this->db->callExecute($qry1,$arg);
			
	}catch(Exception $e){
		expDebug::dPrint("ERROR in updateDeletedUser ".print_r($e,true),1);
		throw new Exception($e->getMessage());
	}
}
	
	private function userAccessCheck(){
		try{
			
		$jobDetail = $this->getJobDetails();

		// check user belongs to super admin group
		$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
		$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
		$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
		
		expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
		
		$this->db->callQuery($query,$args);
		$checkSupAdmin = $this->db->fetchColumn();
		expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
		if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
		
		}else{
				//Organization Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
							"temp1.user_name as user_name",
							"GROUP_CONCAT(grpmp.group_id) as group_id",
							"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
							'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = temp1.org_id AND grpmp.entity_type = \'cre_org\'',
							'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
							'LEFT JOIN slt_organization per ON per.id = temp1.org_id',
							'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
							"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.org_id IS NULL THEN 1 = 1 ELSE usr.org_id != temp1.org_id END",
							"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
							"t2.record_status" => ":rec ",
							"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
							"t2.batch_id = :batch_id ",
							"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
							':batch_id' => $jobDetail['batchId'],
							':rec' => 'IN',
							':err' => 'Access to this Organization is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" Organization Access check ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
				
				//Department Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_profile_list_items per ON per.code = temp1.dept_code',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_dpt\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
						'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.dept_code IS NULL THEN 1 = 1 ELSE usr.dept_code != temp1.dept_code END",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this Department is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" Department Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
				
				//Job Title Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_profile_list_items per ON per.code = temp1.job_title',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_jtl\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
						'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.job_title IS NULL THEN 1 = 1 ELSE usr.job_title != temp1.job_title END",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this Job Title is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" Job Title Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
				
				//Employment Type Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_profile_list_items per ON per.code = temp1.employment_type',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_etp\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
						'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.employment_type IS NULL THEN 1 = 1 ELSE usr.employment_type != temp1.employment_type END",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this Employment Type is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" Employment Type Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
				
				//User Type Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_profile_list_items per ON per.code = temp1.user_type',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr_ptp\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
						'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.user_type IS NULL THEN 1 = 1 ELSE usr.user_type != temp1.user_type END",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this User Type is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" User Type Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
				
				//User Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_person per ON per.user_name = temp1.user_name',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv'",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this User is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" User Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
		
				//Manager User Access Check.
				list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
				$selectQry['table'] = $jobDetail['pTable'].' temp1';
				$selectQry['fields']= array(
						"temp1.user_name as user_name",
						"GROUP_CONCAT(grpmp.group_id) as group_id",
						"GROUP_CONCAT(grps.userslist) AS userlist"
				);
				$selectQry['joins'] = array(
						'LEFT JOIN slt_person per ON per.id = temp1.manager_id',
						'LEFT JOIN slt_group_mapping grpmp ON grpmp.entity_id = per.id AND grpmp.entity_type = \'cre_usr\'',
						'LEFT JOIN slt_groups grps ON grps.id = grpmp.group_id',
						'LEFT JOIN slt_person usr ON usr.user_name = temp1.user_name'
				);
				$selectQry['alias'] = "x";
				
				$selectQry['condition']=array(
						"grpmp.entity_id IS NOT NULL and grps.status='cre_sec_sts_atv' AND CASE WHEN usr.manager_id IS NULL THEN 1 = 1 ELSE usr.manager_id != temp1.manager_id END",
						"per.is_manager ='Y' and per.status = 'cre_usr_sts_atv'",
						"(per.created_by!=".$jobDetail['uploaded_user_id']." AND per.updated_by!=".$jobDetail['uploaded_user_id'].")"
				);
				
				$selectQry['others'] = ' GROUP BY temp1.user_name HAVING FIND_IN_SET('.$jobDetail['uploaded_user_id'].',IFNULL(userlist,0)) <=0 ';
				
				$select1 = array($selectQry);
				$update['table'] = $jobDetail['pTable'].' t2';
				$update['fields'] = array(
						"t2.record_status" => ":rec ",
						"t2.custom4" => "concat(ifnull(t2.custom4,''),:err )"
				);
				$update['condition']=array(
						"t2.batch_id = :batch_id ",
						"t2.user_name = x.user_name"
				);
				$arg = array(':batch_id' => $jobDetail['batchId'],
						':batch_id' => $jobDetail['batchId'],
						':rec' => 'IN',
						':err' => 'Access to this Manager User is restricted. '
				);
				
				$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
				expDebug::dPrintDBAPI(" Manager User Access Check. ",$updQry1,$arg);
				$this->db->callExecute($updQry1,$arg);
		
		}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in userAccessCheck ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function updateManagerProfile($tblDet){
		try{
	
			//Manager remove from user profile
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					'manusr.manager_id' => 'NULL'
			);
			$condition=array(
					't22.is_manager = :n ',
					'per.is_manager = :y ',
					't22.primary_id IS NOT NULL',
					't22.primary_id != :nul ',
					'manusr.manager_id = per.id'
			);
			$join=array(
					'inner join '.$tblDet['tblname'].' t22 ON t22.batch_id = :batch_id',
					'inner join slt_person per on per.user_name = t22.user_name'
			);
			$arg = array(
					':batch_id' => $tblDet['btc_id'],
					':nul'=>'',
					':n'=>'N',
					':y'=>'Y'
			);
			$qry1 = $this->db->prepareQueryUpdate("slt_person manusr",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" updateManagerProefile qry1 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			$sql = 'DELETE man from slt_person_other_manager man
					inner join '.$tblDet['tblname'].' t22 ON t22.batch_id = '.$tblDet['btc_id'].'
					inner join slt_person per ON per.user_name = t22.user_name 
					WHERE t22.is_manager = \'N\' and per.is_manager = \'Y\' and t22.primary_id IS NOT NULL and t22.primary_id != \'\' and man.manager_id = per.id';
			
			$this->db->connect();
			expDebug::dPrintDBAPI('updateManagerProefile entities bulk delete query ',$sql);
			$this->db->execute($sql);
			$this->db->closeconnect();
			
		}catch (Exception $e){
			expDebug::dPrint("ERROR in updateManagerProfile tables ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	private function updatewebex($tblDet){
		
		try{
			//Drupal uid manager detail updation for temp table.
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					'p.webex_name' => 't22.webex_name',
					'p.webex_pass' => 't22.webex_pass',
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					'p.is_instructor = "Y"'
			);
			$join=array(
					'inner join '.$tblDet['tblname'].' t22 on p.user_name = t22.user_name',
			);
			$arg = array(
					':batch_id' => $tblDet['btc_id']
			);
			$qry1 = $this->db->prepareQueryUpdate("slt_person p",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" updatewebex ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
		
		}catch (Exception $e){
			expDebug::dPrint("ERROR in drupalUidUpdate tables ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
		
	}
	private function drupalUidUpdate($tblDet){
		try{
			//Drupal uid manager detail updation for temp table.
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.drup_uid' => "u.uid ",
					't22.manager_email' => "p.email",
					't22.mgr_uid' => "p.id",
					't22.mgr_drup_uid' => "mu.uid",
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					'u.name = t22.user_name'
			);
			$join=array(
					'inner join users u on u.name = t22.user_name',
					'inner join slt_person p on p.id = t22.manager_id',
					'inner join users mu on mu.name = p.user_name'
			);
			$arg = array(
					':batch_id' => $tblDet['btc_id']
			);
			$qry1 = $this->db->prepareQueryUpdate($tblDet['tblname']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" drupalUidUpdate Updation ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
	
		}catch (Exception $e){
			expDebug::dPrint("ERROR in drupalUidUpdate tables ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function alterTables(){
		try{
			$jobDetail = $this->getJobDetails();
			//alter column to add class_id in temp1 table
			$atrQry1 = ' ALTER TABLE '.$jobDetail['pTable']."
										 ADD column user_email varchar(255) default null,
										 ADD column drup_uid int(10) default null,
										 ADD column manager_email varchar(255) default null,
										 ADD column mgr_uid int(10) default null,
										 ADD column mgr_drup_uid int(10) default null,
										 ADD KEY map (mapping_id) ";
	
			expDebug::dPrintDBAPI('Alter temp table 1 query ',$atrQry1);
			$this->db->callExecute($atrQry1);
	
		}catch(Exception $e){
			expDebug::dPrint("ERROR in alter tables ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function insertNotification(){
		try{
			$tblDet = $this->getTableDetails();
			$util = new GlobalUtil();
			$config = $util->getConfig();
			$notifyParam    = $config["send_notification_bulk_user"];
			$upldby = $tblDet['user_id'];
			if($notifyParam == 1){
					
				$colNm = 't2.notifyuser as notifystring,t2.user_name as user_name,t2.email as email,t2.drup_uid as uid ';
				$grpBy = 'group by t2.notifyuser,t2.user_name ';
				$unionQry = 'SELECT '.$colNm.' from '.$tblDet['tblname'].' t2 LEFT JOIN slt_person per on per.user_name = t2.user_name WHERE t2.batch_id =  :btid  and t2.record_status =  :r '. $grpBy;
				$args=array(
						':btid' => $tblDet['btc_id'],
						':r' => 'R'
				);
				$this->db->callExecute($unionQry,$args);
				expDebug::dPrintDBAPI('Query for generating records for Notification insert',$unionQry,$args);
				$notDetails = $this->db->fetchAllResults();
					
				$args = array();
				$conditons = array();
				$fields = array(
						'ninfo.id',
						'ninfo.notification_code',
						'ninfo.lang_code',
						'ninfo.locale_code',
						'ninfo.profile_code',
						'ninfo.notification_sendto',
						'ninfo.entity_type',
						'ninfo.notification_cc',
						'ninfo.notification_admin',
						'ninfo.notification_title',
						'ninfo.notification_description',
						'ninfo.notification_type',
						'ninfo.frequency_data_type',
						'ninfo.frequency_data_value',
						'ninfo.status',
						'group_concat("\'",frame.lang_code,"\'") as frame_lang'
				);
				$join = array('left join slt_notification_frame frame on frame.notification_id = ninfo.id');
				$conditons[] = ' ninfo.lang_code = :langcode ';
				$conditons[] = " ninfo.notification_code IN ('notify_130','notify_132','notify_129','assign_dotted_manager','assign_dotted_organization','suspend_user','activate_user') ";
				$args[':langcode']= 'cre_sys_lng_eng';
				$others = "group by ninfo.id";
					
				// Prepare query
				$query = $this->db->prepareQuerySelect('slt_notification_info ninfo',$fields,$conditons,$join,$others);
				expDebug::dPrintDBAPI("Get Notification details",$query,$args);
				$this->db->callQuery($query,$args);
				$NotInfoDetails = $this->db->fetchAllResults();
					
				foreach($NotInfoDetails as $key => $value){
					$notDetArr[$value->notification_code] = $value;
				}
					
				foreach ($notDetails as $res => $val ){
					expDebug::dPrint("Fetch the result of notification".print_r($val,1),4);
					$username = $val->user_name;
					$email = $val->email;
					$uid = $val->uid;
					$notifyString = explode(',', trim($val->notifystring,","));
	
					expDebug::dPrint("Passed Notify String ".print_r($notifyString,1),4);
					foreach ($notifyString as $notify){
	
						if($notify == 'newusercreatedbyadmin'){
							$this->userNotificationInsert('notify_132', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'suspenduser'){
							$this->userNotificationInsert('suspend_user', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'activeuser'){
							$this->userNotificationInsert('activate_user', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'assignmanager'){
							$this->userNotificationInsert('notify_130', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'assignorg'){
							$this->userNotificationInsert('notify_129', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'assignothermanager'){
							$this->userNotificationInsert('assign_dotted_manager', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}elseif ($notify == 'assignotherorg'){
							$this->userNotificationInsert('assign_dotted_organization', $username,$email,$uid,$upldby,$notDetArr,$notify);
						}
					}
				}
			}
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insert notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function updateNofifyType(){
		try{
			$jobDetail = $this->getJobDetails();
	
			//New user notification
			list($fields,$args,$condition) = array(array(),array(),array());
			$fields=array(
					"t22.notifyuser" => "CONCAT(
										 if((t22.status = :atv and (t22.custom0 = :nul or t22.custom0 is NULL)), :newuser ,''),
										 if((t22.status = :atv and ((t22.manager_id!=:nul or t22.manager_id is not null))), :manager ,''),
										 if((t22.status = :atv and ((t22.dotted_mngr_id!=:nul or t22.dotted_mngr_id is not null))), :otmanager ,''),
										 if((t22.status = :atv and ((t22.dotted_org_id!=:nul or t22.dotted_org_id is not null))), :otorg ,''),
										 if((t22.status = :atv and ((t22.org_id!=:nul or t22.org_id is not null))), :org ,''))",
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.operation = :ins ',
					't22.record_status = :r ',
					't22.primary_id = :nul or t22.primary_id is NULL '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul' => '',
					':atv' => 'cre_usr_sts_atv',
					':sus' => 'cre_usr_sts_itv',
					':newuser' => 'newusercreatedbyadmin,',
					':manager' => 'assignmanager,',
					':otmanager' => 'assignothermanager,',
					':otorg' => 'assignotherorg,',
					':org' => 'assignorg,',
					':ins' => 'insert',
					':r' => 'R'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition);
			expDebug::dPrintDBAPI(" Update temp2 table qry1 for notifyuser",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
	
			//Existing user notification
			list($fields,$args,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					"t22.notifyuser" => "CONCAT(
										 if((t22.status = :sus and p.status!=:sus ), :suspend ,''),
										 if((t22.status = :atv and p.status!=:atv ), :active ,''),
										 if((t22.status = :atv and ((t22.manager_id!=:nul or t22.manager_id is not null) and if(!(p.manager_id is null or p.manager_id = :nul ),t22.manager_id != p.manager_id,1))), :manager ,''),
										 if((t22.status = :atv and ((t22.org_id!=:nul or t22.org_id is not null) and if(!(p.org_id is null or p.org_id = :nul ),t22.org_id != p.org_id,1))), :org ,''))",
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.operation = :upd ',
					't22.record_status = :r ',
					't22.primary_id != :nul or t22.primary_id is not NULL '
			);
			$join=array(
					'inner join slt_person p on p.user_name = t22.user_name '
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul' => '',
					':atv' => 'cre_usr_sts_atv',
					':sus' => 'cre_usr_sts_itv',
					':manager' => 'assignmanager,',
					':org' => 'assignorg,',
					':suspend' => 'suspenduser,',
					':active' => 'activeuser,',
					':upd' => 'update',
					':r' => 'R'
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Update temp2 table qry1 for notifyuser",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			// Other mng and Other org nitification check
			$query = "select group_concat(org.id) as otorg, group_concat(mng.id) as otmng, per.dotted_mngr_id as dotter_mng, per.dotted_org_id as dotter_org, per.user_name as user_name 
					from ".$jobDetail['pTable']." per
					left join slt_person_other_organization otorg on otorg.user_id = per.primary_id and otorg.is_direct = 'N'
					left join slt_person_other_manager otmng on otmng.user_id = per.primary_id and otmng.is_direct = 'N'
					left join slt_organization org on org.id = otorg.organization_id
					left join slt_person mng on mng.id = otmng.manager_id 
							where per.primary_id != '' or per.primary_id is not null and ((per.dotted_mngr_id != '' or per.dotted_mngr_id is not null) 
					or (per.dotted_org_id != '' or per.dotted_org_id is not null)) and per.status = :atv and per.record_status = :r and per.operation = :upd group by per.user_name  ";

			$this->db->callQuery($query,array(':r'=>'R', ':upd'=>'update', ':atv'=>'cre_usr_sts_atv'));
			$otherorgDetail = $this->db->fetchAllResults();
			
			foreach ($otherorgDetail as $otherDetail){
				
				$otmng = explode(',',$otherDetail->otmng);
				$dotter_mng = explode(',',$otherDetail->dotter_mng);
				
				$otorg = explode(',',$otherDetail->otorg);
				$dotter_org = explode(',',$otherDetail->dotter_org);
				
				$manager_arr_diff = array_diff($otmng, $dotter_mng);
				$manager_arr_diff1 = array_diff($dotter_mng,$otmng);

				$org_arr_diff = array_diff($otorg, $dotter_org);
				$org_arr_diff1 = array_diff($dotter_org,$otorg);
					
				if((count($manager_arr_diff)>0 || count($manager_arr_diff1)>0)){
					$qry1 = "update ".$jobDetail['pTable']." set notifyuser = concat(ifnull(notifyuser,''), :otmanager ) where user_name = :user and record_status = :r and operation = :upd ";
					$args=array(
							':otmanager' => 'assignothermanager,',
							':r' => 'R',
							':user' => $otherDetail->user_name,
							':upd' => 'update'
					);
					expDebug::dPrintDBAPI(" Update temp2 table qry1 for notifyuser",$qry1,$args);
					$this->db->callExecute($qry1,$args);
				}
				if((count($org_arr_diff)>0 || count($org_arr_diff1)>0)){
					$qry1 = "update ".$jobDetail['pTable']." set notifyuser = concat(ifnull(notifyuser,''), :otorg ) where user_name = :user and record_status = :r and operation = :upd ";
					$args=array(
							':otorg' => 'assignotherorg,',
							':r' => 'R',
							':user' => $otherDetail->user_name,
							':upd' => 'update'
					);
					expDebug::dPrintDBAPI(" Update temp2 table qry1 for notifyuser",$qry1,$args);
					$this->db->callExecute($qry1,$args);
				}	
			}
			
		}catch(Exception $e){
			expDebug::dPrint("ERROR in notify type update ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	protected function userNotificationInsert($notStr,$username,$email,$uid,$updByUsrId,$notDetArr,$notifyString){
		try{
	
			$notifyFlag = null;
			$emailSendTo = null;
			$ccEmail = null;
			if ($notDetArr[$notStr] != null) {
				$notifyFlag = $notDetArr[$notStr]->status;
				$emailSendTo = $notDetArr[$notStr]->notification_sendto;
				$ccEmail = $notDetArr[$notStr]->notification_cc;
	
				if ($notifyFlag == 'cre_ntn_sts_itv') {
					expDebug::dPrint('Notification is disabled',4);
					return;
				}
				if(!empty($ccEmail)){
					$mailIdWithCc = $emailSendTo.",".$ccEmail;
				}else{
					$mailIdWithCc =  $emailSendTo;
				}
				$explodeMailIdWithCc = array_filter(array_unique(explode(",", $mailIdWithCc )));
			}
	
			$notTokenStr = $this->generateNotificationsTokenString($notStr,$username,$uid,$email,$moduleseq);
			$tblDet = $this->getTableDetails();
			$notVal = $notDetArr[$notStr];
			$emailSendTo = explode(',', $notVal->notification_sendto);
	
			if ((in_array("cre_ntn_rpt_usr", $emailSendTo)) && (!empty($explodeMailIdWithCc))) {
				list($fields, $args) = array(array(),array());
				$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'p.id', 'u.uid',
						'p.full_name', 'temp.email', ':nullval ', ':sendtype ', ':sndsts ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
				$args = array(
						':msg_id' => $notTokenStr['message_id'],
						':lngcode' => 'cre_sys_lng_eng',
						':msgtype' => $notTokenStr['message_type'],
						':sendtype' => 'php mailer',
						':sndsts' => 'N',
						':nullval' => NULL,
						':updusr' => $tblDet['user_id'],
						':atv' => 'cre_usr_sts_atv',
						':itv' => 'cre_usr_sts_itv',
						':recsts' => 'R',
						':btid' => $tblDet['btc_id'],
						':loadby' => $tblDet['loadby'],
						':name' => $username
				);
				$this->insertUserNotification($fields,$args,$notifyString);
			}
			if ((in_array("cre_ntn_rpt_mgr", $emailSendTo)) && (!empty($explodeMailIdWithCc))) {
				list($fields, $args) = array(array(),array());
				$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'temp.mgr_uid', 'temp.mgr_drup_uid',
						'p.full_name', 'temp.manager_email', ':nullval ', ':sendtype ', ':sndsts ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
				$args = array(
						':msg_id' => $notTokenStr['message_id'],
						':lngcode' => 'cre_sys_lng_eng',
						':msgtype' => $notTokenStr['message_type'],
						':sendtype' => 'php mailer',
						':sndsts' => 'N',
						':nullval' => NULL,
						':updusr' => $tblDet['user_id'],
						':atv' => 'cre_usr_sts_atv',
						':itv' => 'cre_usr_sts_itv',
						':recsts' => 'R',
						':btid' => $tblDet['btc_id'],
						':loadby' => $tblDet['loadby'],
						':name' => $username
				);
				$this->insertUserNotification($fields,$args,$notifyString);
			}
			if(!empty($ccEmail) && (!empty($explodeMailIdWithCc))) {
				list($fields, $args) = array(array(),array());
				$fields = array(':msg_id ', "if(p.preferred_language IN (".$notVal->frame_lang."), p.preferred_language, :lngcode )", ':msgtype ', "CONCAT(".$notTokenStr['tokens_string'].")", 'p.id', 'u.uid',
						'p.full_name', ':sndtoml ', ':nullval ', ':sendtype ', ':sndsts ', ':updusr ', 'now()', ':updusr ', 'now()', ':nullval ',':loadby ');
				$args = array(
						':msg_id' => $notTokenStr['message_id'],
						':lngcode' => 'cre_sys_lng_eng',
						':msgtype' => $notTokenStr['message_type'],
						':sendtype' => 'php mailer',
						':sndsts' => 'N',
						':nullval' => NULL,
						':updusr' => $tblDet['user_id'],
						':sndtoml' => $ccEmail,
						':atv' => 'cre_usr_sts_atv',
						':itv' => 'cre_usr_sts_itv',
						':recsts' => 'R',
						':btid' => $tblDet['btc_id'],
						':loadby' => $tblDet['loadby'],
						':name' => $username
				);
				$this->insertUserNotification($fields,$args,$notifyString);
			}
	
		}catch(Exception $e){
			expDebug::dPrint("ERROR in userNotificationInsert ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function insertUserNotification($fields,$args,$notStr){
		try{
			$tblDet = $this->getTableDetails();
			$tbl = $tblDet['tblname'];
			$insert = array();
			$insert['table'] = 'slt_bulk_notification';
			$insert['fields'] = array(
					'msg_id',
					'lang_code',
					'msg_type',
					'token_str',
					'user_id',
					'send_to_id',
					'send_to_name',
					'send_to_email',
					'send_to_email_cc',
					'send_type',
					'send_status',
					'created_by',
					'created_on',
					'updated_by',
					'updated_on',
					'custom0',
					'dataload_by');
				$select = array();
				
				$select['fields']= $fields;
				
				$select['table'] = $tblDet['tblname']." temp";
				
				$select['joins'] = array('INNER JOIN slt_person p on p.user_name = temp.user_name',
						'LEFT JOIN users u on u.name = temp.user_name',
						'LEFT JOIN slt_person man on man.id = temp.manager_id',
						'LEFT JOIN slt_organization org on org.id = temp.org_id',
				);
				$select['condition'] = array(
						'temp.record_status IN (:recsts )',
						'p.status IN (:atv ,:itv ) ',
						'temp.batch_id =  :btid ',
						'temp.user_name =:name '
				);
	
				$arg= $args;
				
				$query = $this->db->prepareInsertBySelect($insert,$select);
				expDebug::dPrintDBAPI("Query for slt_bulk_notification bulk execute ", $query,$arg);
				$this->db->callExecute($query,$arg);
				
		}catch(Exception $e){
			expDebug::dPrint("ERROR in insertEnrollmentNotification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateNotificationsTokenString($notStr,$username,$uid,$email,$moduleseq){
		try{
			expDebug::dPrint("values with in generateNotificationsTokenString ".print_r($notStr,1),4);
				
			if($notStr == 'notify_132'){
				$tokenStr = $this->generateNewUserCreation($notStr,$email,$uid);
			}elseif($notStr == 'suspend_user'){
				$tokenStr = $this->generateSuspendedUser($notStr,$email,$uid);
			}elseif($notStr == 'activate_user'){
				$tokenStr = $this->generateActivatedUser($notStr,$email,$uid);
			}elseif($notStr == 'notify_130'){
				$tokenStr = $this->generateManagerAssignUser($notStr,$email,$uid);
			}elseif($notStr == 'notify_129'){
				$tokenStr = $this->generateOrganizationAssignUser($notStr,$email,$uid);
			}elseif($notStr == 'assign_dotted_manager'){
				$tokenStr = $this->generateOtherManagerAssignUser($notStr,$email,$uid);
			}elseif($notStr == 'assign_dotted_organization'){
				$tokenStr = $this->generateOtherOrganizationAssignUser($notStr,$email,$uid);
			}
			return $tokenStr;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateNotificationTokenString ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateOtherOrganizationAssignUser($notStr,$email,$uid){
		try{
			$query = "select group_concat(org.name) as otorg from slt_person per 
					left join slt_person_other_organization otorg on otorg.user_id = per.id and otorg.is_direct = 'N' 
					left join slt_organization org on org.id = otorg.organization_id where per.email = :mail ";
			
			$this->db->callQuery($query,array(':mail'=>$email));
			$otherorgDetail = $this->db->fetchAllResults();
			$orglist = $otherorgDetail[0]->otorg;
			
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' ,
					'dotted_org_names>|', '$orglist' ,'~|' ,
					'last_name>|',ifnull(p.last_name,''),'~|' ,
					'user_email>|',ifnull(p.email,''),'~|' ,
					'user_phone>|',ifnull(p.phone_no,''),'~|' ,
					'learning_fullname>|',ifnull(p.full_name,''),'~|' ,
					'full_name>|',ifnull(p.full_name,''),'~|' ,
					'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'assign_dotted_organization';
			$notificationInfo['message_type'] = 'Assigned Other Organization';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateOtherOrganizationAssignUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateOtherManagerAssignUser($notStr,$email,$uid){
		try{
			$query = "select group_concat(mng.full_name) as otmng from slt_person per
					left join slt_person_other_manager otmng on otmng.user_id = per.id and otmng.is_direct = 'N'
					left join slt_person mng on mng.id = otmng.manager_id where per.email = '".$email."'";
			
			$this->db->callQuery($query,array(':mail'=>$email));
			$othermngDetail = $this->db->fetchAllResults();
			$mnglist = $othermngDetail[0]->otmng;
			
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' ,
					'dotted_manager_names>|', '$mnglist' ,'~|' ,
					'last_name>|',ifnull(p.last_name,''),'~|' ,
					'user_email>|',ifnull(p.email,''),'~|' ,
					'user_phone>|',ifnull(p.phone_no,''),'~|' ,
					'learning_fullname>|',ifnull(p.full_name,''),'~|' ,
					'full_name>|',ifnull(p.full_name,''),'~|' ,
					'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'assign_dotted_manager';
			$notificationInfo['message_type'] = 'Assigned Other Manager';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateOtherManagerAssignUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateOrganizationAssignUser($notStr,$email,$uid){
		try{
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' ,
					'org_name>|',ifnull(org.name,''),'~|' ,
					'last_name>|',ifnull(p.last_name,''),'~|' ,
					'user_email>|',ifnull(p.email,''),'~|' ,
					'user_phone>|',ifnull(p.phone_no,''),'~|' ,
					'learning_fullname>|',ifnull(p.full_name,''),'~|' ,
					'full_name>|',ifnull(p.full_name,''),'~|' ,
					'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'notify_129';
			$notificationInfo['message_type'] = 'Assign Organization to User by Admin';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateOrganizationAssignUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateManagerAssignUser($notStr,$email,$uid){
		try{
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' ,
					'manager_name>|',ifnull(man.full_name,''),'~|' ,
					'last_name>|',ifnull(p.last_name,''),'~|' ,
					'user_email>|',ifnull(p.email,''),'~|' ,
					'user_phone>|',ifnull(p.phone_no,''),'~|' ,
					'learning_fullname>|',ifnull(p.full_name,''),'~|' ,
					'full_name>|',ifnull(p.full_name,''),'~|' ,
					'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'notify_130';
			$notificationInfo['message_type'] = 'Assign Manager to User by Admin';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateManagerAssignUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateActivatedUser($notStr,$email,$uid){
		try{
			$account = user_load($uid);
			$notifyResult = '<a href="'.user_pass_reset_url($account).'" title="'.t('Link').'">'.strtolower(t('Link')).'</a>';
			
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' , 
					'reset_pass_url>|', '$notifyResult' ,'~|' ,
					'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'activate_user';
			$notificationInfo['message_type'] = 'Activate User Notification';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateActivatedUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateSuspendedUser($notStr,$email,$uid){
		try{
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' , 'first_name>|',ifnull(p.first_name,'')";
	
			$notificationInfo['message_id'] = 'suspend_user';
			$notificationInfo['message_type'] = 'Suspend User Notification';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateSuspendedUser notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	private function generateNewUserCreation($notStr,$email,$uid){
		try{
			$users = user_load_multiple(array(), array('mail' => $email, 'status' => '1'));
			$account = reset($users);
			expDebug::dPrint(' $account = ' . print_r($users, true) , 4);
			$params['account'] = $account;
			
			$notifyResult->one_time_url = str_replace('/apis/ext','',user_pass_reset_url($params['account']));
			
			expDebug::dPrint('The value of $notifyResult is here vetrivel '.print_r($notifyResult,1),3);
			
			$notificationInfo = array();
			$notificationInfo['tokens_string'] = "'user_name>|',ifnull(p.user_name,''),'~|' ,
					'first_name>|',ifnull(p.first_name,''),'~|' ,
					'one_time_url>|', '$notifyResult->one_time_url' ,'~|' ,
					'full_name>|',ifnull(p.full_name,'')";
			
			$notificationInfo['message_id'] = 'notify_132';
			$notificationInfo['message_type'] = 'New User Creation';
			return $notificationInfo;
		}catch(Exception $e){
			expDebug::dPrint("ERROR in generateNewUserCreation notification ".print_r($e,true),1);
			throw new Exception($e->getMessage());
		}
	}

}
		
?>