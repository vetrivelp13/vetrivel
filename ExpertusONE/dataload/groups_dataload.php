<?php 
/*require_once "data_load_validate.php";
include_once DRUPAL_ROOT . "/includes/common.inc";
include_once DRUPAL_ROOT . "/includes/database/database.inc";
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
include_once DRUPAL_ROOT . "/sites/all/services/Encryption.php";
require_once DRUPAL_ROOT . '/sites/all/services/GlobalUtil.php';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);*/

require_once "data_load_validate.php";
include_once $_SERVER['DOCUMENT_ROOT']."/dataload/Database.php";

class groups_dataload extends dataloadValidate{
	
	private $db;
	private $temp_grp_tbl_name;
	private $jobDetail;
	private $batchrecords;
	private $tblDet;
	
	
	function __construct($from=''){
		$this->db = new DLDatabase('DL',$from);
		$this->db->update_queue = true;
	}
	
	public function extenderValidate($jobDetail=array(),$batchrecords=array()){
		try{
			
		}catch(Exception $e){
	
		}
	
	}
	
	public function validate($jobDetail=array(),$batchrecords=array()){
		 try{
			
		}catch(Exception $e){
	
		}
	}

	public function execute($jobDetail=array(),$batchrecords=array()){
		 try{
			
		}catch(Exception $e){
	
		}
	} 

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

	public function bulkValidate($jobDetail=array(),$batchrecords=array()){
		try{
			$util = new GlobalUtil();
			$config = $util->getConfig();
			$configInvalid    = $config["skip_invalid_record"];
			$grouptemptable = $jobDetail['pTable'].'_groups';
			expdebug::dPrint(' Job Details in group '.print_r($jobDetail,true),5);
			expdebug::dPrint(' batchrecords in group '.print_r($batchrecords,true),5);
			expdebug::dPrint(' $grouptemptable value is here '. $grouptemptable);
			
			$this->setJobDetails($jobDetail);
			$this->setBatchRecords($batchrecords);
			
			$this->db->callExecute("drop table if exists ".$grouptemptable);
								   $qry  = "CREATE TABLE $grouptemptable (`id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
								  `user_name` varchar(255) DEFAULT NULL,
								  `user_id` int(11) NULL,
								  `uid` int(11) unsigned NULL,
								  `user_status` varchar(20) NULL,
								  `group_name` varchar(255) NULL,
								  `editable` varchar(1) default '0',
								  `group_id` int(11) NULL,
								  `rid` int(11) NULL,
								  `group_code` varchar(20) NULL,
								  `group_type` tinyint(1) NULL,
								  `is_removed_user` tinyint(1) default '0',
								  `group_status` varchar(20) NULL,								  
								  `record_status` char(15) default 'R', 
								  `remarks` longtext default null,
								  `batch_id` int(11),
								  index temp2_user_name (user_name),
								  index temp2_user_id (user_id),
								  index temp2_group_name (group_name),
								  index temp2_group_id (group_id),
								  index temp2_batch_id (batch_id),
								  index temp2_record_status (record_status)) ENGINE=InnoDB DEFAULT CHARSET=utf8 ";
			expDebug::dPrintDBAPI("Create temprory table group --> ",$qry);
			$this->db->callExecute($qry);
			
			$this->temp_grp_tbl_name= $grouptemptable;
				
			//Group Exist check
			$qry = 'select t1.groups as groups ,t1.user_name as user_name from '.$jobDetail['sTable'].' t1 inner join '.$jobDetail['pTable'].' t2 on t1.mapping_id = t2.mapping_id
				where t2.batch_id ='.$jobDetail['batchId'];
			
			expDebug::dPrint('Group Record is here '. $qry);
			$this->db->callQuery($qry);
			$grouplist = $this->db->fetchAllResults();
			expDebug::dPrint('group list '.print_r($grouplist,true),5);
			
			foreach($grouplist as $list){
				list($fields, $args,$err) = array(array(),array(),'');
				$list->groups=trim($list->groups);
				expDebug::dPrint('The value of $list is here '.print_r($list,1),3);
				if(!empty($list->groups)){
					$dottedgroup = explode(',',$list->groups);
					$dottedgroup = array_map('trim',$dottedgroup); 
					
					$dottedgroupId = '';
					foreach($dottedgroup as $group){
						$query = "select id from slt_groups where name = :group_name and status != :del LIMIT 1";
						$this->db->callQuery($query,array(':group_name'=> addslashes(trim($group)),':del'=> 'cre_sec_sts_del'));
						$groupId = $this->db->fetchColumn();
						if(empty($groupId))
							$err .= 'Group '.addslashes(trim($group)).' does not exist.';
						else
							$dottedgroupId .= ($dottedgroupId == '') ? $groupId : ','.$groupId;
					}
				}
				expDebug::dPrint('The value of $err $err $err $err $err is here '.print_r($err,1),2);
				$fields['custom4'] = 'concat(ifnull(custom4,""), :custom4 )';
				$fields['record_status'] = "if(".$configInvalid." = 1 and ".(empty($err) ? 0 : 1)." = 1, :recst ,record_status)";
				$args[':user_name'] = addslashes($list->user_name);
				$args[':custom4'] = $err;
				$args[':recst'] = 'IN';
				$updQry = $this->db->prepareQueryUpdate($jobDetail['pTable'],$fields,array(" user_name = :user_name "));
				expDebug::dPrintDBAPI("Group exist checking validation ",$updQry,$args);
				$this->db->callExecute($updQry,$args);
			}
			
			//Insert into temp group table.
			$this->db->connect();
			$this->db->query('select t1.user_name as user_name,t1.groups as groups,t2.status as status,t2.record_status as record_status,t2.custom4 as remarks, t2.batch_id as batch_id from '.$jobDetail['sTable'].' t1 
					left join '.$jobDetail['pTable'].' t2 on t2.mapping_id = t1.mapping_id where t2.batch_id = '.$jobDetail['batchId'].' ');
			$groupDetails = $this->db->fetchAllResults();
			
			$group_map_query = 'INSERT INTO '.$grouptemptable.' ( user_name, user_status, group_name, record_status, batch_id) VALUES ';
			$val_group_str = '';
			foreach($groupDetails as $val){
				if(!empty($val->groups)){
					if(strpos($val->groups,',') !== false){
						$group_arr = explode(',',$val->groups);
						foreach ($group_arr as $group_name){
							if($val_group_str=='')
								$val_group_str .= '("'.trim($val->user_name).'", "'.trim($val->status).'", "'.trim($group_name).'", "'.$val->record_status.'", "'.$val->batch_id.'" )';
							else
								$val_group_str .= ', ("'.trim($val->user_name).'", "'.trim($val->status).'", "'.trim($group_name).'", "'.$val->record_status.'", "'.$val->batch_id.'" )';
						}
					}else{
						if($val_group_str=='')
							$val_group_str .= '("'.trim($val->user_name).'", "'.trim($val->status).'", "'.trim($val->groups).'", "'.$val->record_status.'", "'.$val->batch_id.'" )';
						else
							$val_group_str .= ', ("'.trim($val->user_name).'", "'.trim($val->status).'", "'.trim($val->groups).'", "'.$val->record_status.'", "'.$val->batch_id.'" )';
					}
				}
			}
			if(!empty($val_group_str)){
				$insert_qry = $group_map_query.' '.$val_group_str;
				expDebug::dPrint('Group $$insert_qry--->'.print_r($insert_qry,1),4);
				$this->db->callExecute($insert_qry);
			}
			
			// Existing Group Id update
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't22.group_id' => " per.id "
			);
			$condition=array(
					't22.batch_id = :batch_id ',
					't22.group_name != :nul ',
					't22.group_name IS NOT NULL '
			);
			$join=array(
					'left join slt_groups per on t22.group_name = per.name and per.status != :del'
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul'=>'',
					':del'=>'cre_sec_sts_del'
			);
			$qry1 = $this->db->prepareQueryUpdate($grouptemptable." t22",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Existing manager Id update qry ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
			//Check the user status and group is given
			$this->UserStatusCheck();
			
			//Group Access Check.
			$conditions = array(' grp.code = :code ',' FIND_IN_SET(:uid ,IFNULL(grp.userslist,0)) > 0');
			$args = array(':code'=>'grp_sup', ':uid'=> $jobDetail['uploaded_user_id']);
			$query = $this->db->prepareQuerySelect('slt_groups grp',array('COUNT(1) as cnt'),$conditions,null,null);
			expDebug::dPrintDBAPI("check user belongs to super admin group ",$query,$args);
			
			$this->db->callQuery($query,$args);
			$checkSupAdmin = $this->db->fetchColumn();
			expDebug::DPrint("super admin >>> ".$checkSupAdmin,5 );
			if($jobDetail['uploaded_user_id'] == 1 || $checkSupAdmin > 0){
					
			}else{

			//Privilege Check
			$this->UserGroupPrivilegeCheck();
			
			} 
			
			// Update remark and status from temp group to temp3
			list($fields, $arg,$join,$condition) = array(array(),array(),array(),array());
			$fields=array(
					't11.custom4' => 'concat(ifnull(t11.custom4,""),t2.remarks)',
					't11.record_status' => 't2.record_status'
			);
			$condition=array(
					't2.batch_id = :batch_id ',
					't2.remarks != :nul ',
					't2.remarks IS NOT NULL'
			);
			$join=array(
					'left join '.$grouptemptable.' t2 on t2.user_name =t11.user_name ',
			);
			$arg = array(
					':batch_id' => $jobDetail['batchId'],
					':nul'=>'',
			);
			$qry1 = $this->db->prepareQueryUpdate($jobDetail['pTable']." t11",$fields,$condition,$join);
			expDebug::dPrintDBAPI(" Update remark and status from temp group to temp3 ",$qry1,$arg);
			$this->db->callExecute($qry1,$arg);
			
		}catch(Exception $ex){
			expDebug::dPrint("PDO Error in Validating the batch records for group",1);
			throw new PDOException($ex->getMessage());
		}
  }


 private function UserStatusCheck(){
  	try{
  		
		$util = new GlobalUtil();
  		$config = $util->getConfig();
  		$configInvalid    = $config["skip_invalid_record"];
		
		$jobDetail = $this->getJobDetails();
		
  		$batchrecords = $this->getBatchRecords();  
  		
  		$grouptable = $jobDetail['pTable'].'_groups';  
		
		list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
		$selectQry['table'] = $grouptable.' bt';
		$selectQry['fields']= array(
				"bt.user_name as user_name"				 
		);
		$selectQry['joins'] = array(
				'left join slt_person per on per.user_name=bt.user_name',
				'left join slt_group_user_mapping usr_map on usr_map.group_id=bt.group_id and usr_map.user_id=per.id and usr_map.user_type != :r ' 
		);
		$selectQry['alias'] = "tmp";
		$selectQry['condition']=array( 
		
				'bt.batch_id = :batch_id ',
  				'bt.user_status != :user_atv_status',
  				'bt.group_name != :nul ',
  				'per.id != :nul ',
  				'(usr_map.id = :nul OR usr_map.id IS NULL)'
		);
		
		/*$selectQry['arg'] = array(
  				':batch_id' => $jobDetail['batchId'],  
  				':user_atv_status'=>'cre_usr_sts_atv',
  				':nul'=>''  				 
  		); */
		
		
		$select1 = array($selectQry); 
		
		
		$update['table'] = $grouptable.' bt1';
		
		$update['fields'] = array(
				'bt1.remarks' => 'concat(ifnull(bt1.remarks,""), :err )',
  				//'bt1.record_status' => ':sts'
  				'bt1.record_status' => "CASE WHEN ".$configInvalid." = 1 THEN :sts ELSE bt1.record_status END"  
		);
		$update['condition']=array(				
				"bt1.user_name = tmp.user_name"
		);
		
		$arg = array(
  				':sts'=>'IN', 
  				':err'=>'You can not add the suspended/deleted user to this group.',
  				':batch_id' => $jobDetail['batchId'],  
  				':user_atv_status'=>'cre_usr_sts_atv',
  				':nul'=>'' ,
				':r'=> 'R' 
		);
		
		$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
		expDebug::dPrintDBAPI(" Update query for User Status and Group check -  ",$updQry1,$arg);
		$this->db->callExecute($updQry1,$arg); 
  		
  	}catch (Exception $ex){
  		expDebug::dPrint("PDO Error in user status check",1);
		throw new PDOException($ex->getMessage());
  	}
  } 
  
  private function UserGroupPrivilegeCheck(){ 
  	try{
  		
  		$jobDetail = $this->getJobDetails();
  		$batchrecords = $this->getBatchRecords();
  		
  		$util = new GlobalUtil();
  		$config = $util->getConfig();
  		$configInvalid    = $config["skip_invalid_record"];
  		
  		// #0076259 - Added by ganeshbabuv on Apr 26th 2017  
							
		$sqlquery = "SELECT DISTINCT grp.id AS id, grp.name AS name, grp.code AS grp_code, grp.status AS status_code, grp.is_admin AS is_admin, grp.is_manager AS is_manager, grp.is_instructor AS is_instructor, 
  							(CASE when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (grp.created_by !=".$jobDetail['uploaded_user_id']." OR grp.updated_by !=".$jobDetail['uploaded_user_id'].")
							THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_edit),0))
							FROM slt_group_privilege p
							left join slt_groups g on p.group_id=g.id
							left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_sec' and gm.group_type=1
							where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
							AND if(grp.is_admin=1,p.page_code = 'cre_sec',p.page_code = 'cre_sec_learner') AND gm.entity_id=grp.id)
							ELSE CASE WHEN grp.created_by = ".$jobDetail['uploaded_user_id']." OR grp.updated_by = ".$jobDetail['uploaded_user_id']." OR
							((SELECT ifnull(max(p.priv_edit),0)
							FROM slt_group_privilege p
							left join slt_groups g on p.group_id=g.id
							where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
							AND if(grp.is_admin=1,p.page_code = 'cre_sec',p.page_code = 'cre_sec_learner')) > 0)
							THEN 1
							ELSE 0 END
							END ) AS sumEdit, (CASE
							when group_map.id IS NOT NULL AND catacs.status='cre_sec_sts_atv' AND (grp.created_by !=".$jobDetail['uploaded_user_id'].")
							THEN (SELECT IF(gm.id IS NULL,1,ifnull(max(p.priv_delete),0))
							FROM slt_group_privilege p
							left join slt_groups g on p.group_id=g.id
							left join slt_group_mapping gm on gm.group_id = g.id and gm.entity_type='cre_sec' and gm.group_type=1
							where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
							AND if(grp.is_admin=1,p.page_code = 'cre_sec',p.page_code = 'cre_sec_learner') AND gm.entity_id=grp.id)
							ELSE CASE WHEN grp.created_by = ".$jobDetail['uploaded_user_id']." OR ((SELECT ifnull(max(p.priv_delete),0)
							FROM slt_group_privilege p
							left join slt_groups g on p.group_id=g.id
							where FIND_IN_SET(".$jobDetail['uploaded_user_id'].",g.userslist) > 0 AND g.status = 'cre_sec_sts_atv'
							AND if(grp.is_admin=1,p.page_code = 'cre_sec',p.page_code = 'cre_sec_learner')) > 0)
							THEN 1
							ELSE 0 END
							END ) AS sumDelete
							FROM 
							slt_groups grp
							inner join slt_person per on per.id = ".$jobDetail['uploaded_user_id']."
							LEFT JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
  							LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
							LEFT OUTER JOIN slt_group_mapping group_map ON group_map.entity_id=grp.id 
							AND group_map.entity_type = 'cre_sec' and group_map.group_type = 1 AND 'cre_sec_sts_atv' = (SELECT status FROM slt_groups where id = group_map.group_id)
							LEFT OUTER JOIN slt_groups catacs ON catacs.id=group_map.group_id and (catacs.is_admin =1) 
							LEFT OUTER JOIN slt_group_attributes grpatt_2 ON catacs.id=grpatt.group_id
							LEFT OUTER JOIN slt_group_privilege priv ON priv.group_id = catacs.id and
							if(grp.is_admin = 1, priv.page_code= 'cre_sec',priv.page_code = 'cre_sec_learner')
							WHERE ( (grp.name LIKE '%%') )AND (grp.status NOT IN  ('cre_sec_sts_del')) 
							AND (if(grp.created_by =per.id  OR grp.updated_by = per.id  OR 
							FIND_IN_SET(per.id ,catacs.added_users)>0,1=1,(catacs.removed_users IS NULL OR FIND_IN_SET(per.id ,catacs.removed_users) <= 0) 
							AND (if(catacs.code = 'grp_adm',FIND_IN_SET(per.id ,catacs.added_users)>0,1=1)) 
							
							AND (if(catacs.org_id='All',per.org_id is not null,catacs.org_id is null OR FIND_IN_SET(ifnull(per.org_id,''),catacs.org_id)>0))
				  	AND (if(catacs.user_type='All',per.user_type is not null,catacs.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),catacs.user_type)>0 ))
				  	AND (if(catacs.employment_type='All',per.employment_type is not null,catacs.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),catacs.employment_type)>0))
				  	AND (if(catacs.country='All',per.country is not null,catacs.country is null OR FIND_IN_SET(ifnull(per.country,''),catacs.country)>0))
				  	AND (if(catacs.state='All',per.state is not null,catacs.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),catacs.state)>0))
				  	AND (if(catacs.department='All',per.dept_code is not null,catacs.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),catacs.department)>0))
				  	AND (if(catacs.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,catacs.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(catacs.job_role,''))>0))
				  	AND (if(catacs.language='All',per.preferred_language is not null,catacs.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),catacs.language)>0))
				    AND (CASE WHEN (catacs.is_manager='Y' AND catacs.is_instructor='Y')
				      THEN
				      (ifnull(per.is_manager,'N') = catacs.is_manager or ifnull(per.is_instructor,'N') = catacs.is_instructor)
				      WHEN (catacs.is_manager='Y' AND catacs.is_instructor='N')
				      THEN
				      (ifnull(per.is_manager,'N') = catacs.is_manager)
				      WHEN (catacs.is_manager='N' AND catacs.is_instructor='Y')
				      THEN
				      (ifnull(per.is_instructor,'N') = catacs.is_instructor)
				      ELSE
				      1=1
				      END)
					AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)  )
					AND( (group_map.group_type = 1) OR (group_map.group_id IS NULL) )AND (if((catacs.is_admin = 1),FIND_IN_SET(per.id,catacs.userslist)>0,1=1)))) 
							GROUP BY grp.id
							ORDER BY grp.created_on DESC";
							
							
  		
  		$this->db->callQuery($sqlquery);
  		$grouppriv = $this->db->fetchAllResults();
  		
  		$grouptable = $jobDetail['pTable'].'_groups';
  		
  		foreach($grouppriv as $groups){
  			$sql = "Update ".$grouptable." set editable = ".$groups->sumEdit." where group_name = :group ";
  			$arg = array(':group' => $groups->name);
  			$this->db->callExecute($sql,$arg);
  		}
  		
  		//Update privilege check in temp group table
  		list($selectQry, $select1,$update,$arg) = array(array(),array(),array(),array());
  		$selectQry['table'] = $grouptable.' temp1';
  		$selectQry['fields']= array(
  				"temp1.user_name as user_name",
  				"GROUP_CONCAT(temp1.group_name) as group_name"
  		);
  		$selectQry['alias'] = "x";
  		
  		$selectQry['condition']=array(
  				"editable = 0"
  		);
  		
  		$selectQry['others'] = ' GROUP BY temp1.user_name';
  		
  		$select1 = array($selectQry);
  		$update['table'] = $grouptable.' t2';
  		$update['fields'] = array(
  				"t2.record_status" => ":rec ",
  				"t2.remarks" => "concat(ifnull(t2.remarks,''),:err ,x.group_name, :msg )"
  		);
  		$update['condition']=array(
  				"t2.batch_id = :batch_id ",
  				"t2.user_name = x.user_name"
  		);
  		$arg = array(
  				':batch_id' => $jobDetail['batchId'],
  				':rec' => 'IN',
  				':err'=>'You do not have privilege to add the user to the ',
  				':msg'=>' groups.'
  		);
  		
  		$updQry1 = $this->db->prepareUpdateBySelect($update,$select1);
  		expDebug::dPrintDBAPI(" group User Access Check. ",$updQry1,$arg);
  		$this->db->callExecute($updQry1,$arg);
  		
  	}catch (Exception $ex){
  		expDebug::dPrint("PDO Error in Validating the batch records for group privilege",1);
		throw new PDOException($ex->getMessage());
  	}
  }

  private function updateUserGroupsIdsAndStatusInTmpTbl(){
		 try{ 
			 
			  // Update users details in temp batch tables
			   expDebug::dPrint('Starting to update the users in tmp group users batch table',5);
			  $updateUserQry = 'UPDATE 
			  						'.$this->temp_grp_tbl_name.' bt 
								JOIN  slt_person per ON per.user_name = bt.user_name
								JOIN  users usr ON usr.name = per.user_name
								SET
		  							bt.user_id = per.id,
		  							bt.uid = usr.uid,
		  							bt.user_status = per.status
								WHERE 
		  							bt.record_status!=\'IN\'';

				$this->db->connect();
				$txn = $this->db->beginTrans();
				expDebug::dPrintDBAPI('$updateUserQry =  ',$updateUserQry);
				$this->db->execute($updateUserQry);
				$this->db->commitTrans();
				$this->db->closeconnect(); 
				
				 // Update group details in temp batch tables - If already users mapped this group, not require to process this entry and set the NR status or else the default status will be kept i.e. R status
				expDebug::dPrint('Starting to update the groups in tmp group users batch table',5);
			    $updateGrpQry = 'UPDATE 
			    					'.$this->temp_grp_tbl_name.' bt 
			 						INNER JOIN  slt_groups grp ON grp.name = bt.group_name
									LEFT JOIN  role r ON r.name = grp.name
									LEFT JOIN slt_group_user_mapping grpusrmap on grpusrmap.user_id=bt.user_id and grpusrmap.group_id=grp.id and grpusrmap.user_type != \'R\' 
									SET  
			    						bt.group_id = grp.id, 
			    						bt.group_code = grp.code,
			    						bt.group_type = grp.is_admin,
			    						bt.rid = r.rid,
			    						bt.group_status = grp.status,
			    						bt.is_removed_user = if(FIND_IN_SET(bt.user_id,grp.removed_users),1,0),
										bt.record_status = if(grpusrmap.id!=\'\', \'NR\',\'R\')
									WHERE  
			    						bt.record_status!=\'IN\' 
			    						and grp.status !=\'cre_sec_sts_del\'';  
							
				$this->db->connect();
				$txn = $this->db->beginTrans();
				expDebug::dPrintDBAPI('$updateGrpQry =  ',$updateGrpQry);
				$this->db->execute($updateGrpQry);
				$this->db->commitTrans();
				$this->db->closeconnect();  
 
			
		}catch(Exception $ex){
	      expDebug::dPrint('Error in updating the group and users temp batch table  ===>'.$this->temp_grp_tbl_name.'==>'.$ex->getMessage(),1);
		}
  } 

  private function checkManagerInstuctorGroupIsExist($tblDet) {
  	try {
  		// Checking record count either manager or instructor
  		$this->db->connect();
  		$sel_qry = 'select count(1) as tot from '.$this->temp_grp_tbl_name.' bt where bt.record_status=\'R\' and bt.group_code IN (\'grp_mgr\',\'grp_ins\')';
  		expDebug::dPrintDBAPI('$sel_qry =  ',$sel_qry);
  		$this->db->query($sel_qry);
  		$resCount = $this->db->fetchColumn();
  		expDebug::dPrint('$res_r_count - '.print_r($resCount,true),4);
  		//$this->db->commitTrans();
  		$this->db->closeconnect();
  		
  		if($resCount>0 && $resCount!=''){
  			return true;
  		} else {
  			return false;
  		}
  	} catch (Exception $ex){
  		expDebug::dPrint('Error in getting count at checkManagerInstuctorCount ==>'.$ex->getMessage(),1);
  	}
  }
  
  
	public function updateManagerInstructorInPersonTable(){
			 try{  
		 	
					$this->db->connect();
					$txn = $this->db->beginTrans();   
											
				     $sel_mgr_ins_grp_lists='select grp.id as group_id, grp.code as group_code,grp.added_users as group_added_users 
				     				from slt_groups grp where grp.code in (\'grp_mgr\',\'grp_ins\') and grp.status!=\'cre_sec_sts_del\' and grp.added_users!=\'\'';  
					
					
					expDebug::dPrintDBAPI('$sel_mgr_ins_grp_lists ',$sel_mgr_ins_grp_lists);
					$this->db->query($sel_mgr_ins_grp_lists); 
					$sel_mgr_ins_grp_lists_res = $this->db->fetchAllResults();
 					expDebug::dPrint('Manager / Instrucor Group Lists - $sel_mgr_ins_grp_lists_res - '.print_r($sel_mgr_ins_grp_lists_res,true),5);

					if(count($sel_mgr_ins_grp_lists_res)>0){
						foreach($sel_mgr_ins_grp_lists_res as $sel_grp_key => $sel_grp_val){
							if($sel_grp_val->group_added_users!=''){
								 if($sel_grp_val->group_code=='grp_mgr'){ //if Manager
										 // Update manager flag in slt_person table
					  					$updateMgrQry = 'update slt_person per
														set per.is_manager=\'Y\' 
														where per.id in ('.$sel_grp_val->group_added_users.')';
										expDebug::dPrintDBAPI('$updateMgrQry =  ',$updateMgrQry);
										$this->db->execute($updateMgrQry); 
								 }
								 
								  if($sel_grp_val->group_code=='grp_ins'){ //if instructor
								  	 // Update instructor flag in slt_person table
					  					$updateInsQry = 'update slt_person per
														set per.is_instructor=\'Y\' 
														where per.id in ('.$sel_grp_val->group_added_users.')';
										expDebug::dPrintDBAPI('$updateInsQry =  ',$updateInsQry);
										$this->db->execute($updateInsQry);
								 } 
							 }
						}
					}else{
						expDebug::dPrint('No added users found in Manager and Instructor groups available to retaining',5);
					}
					
					$this->db->commitTrans();
					$this->db->closeconnect();    
				
			}catch(Exception $ex){
		      expDebug::dPrint('Error in retaining the manager and instructor group users  '.$ex->getMessage(),1);
			}
	  }




  private function checkValidGroupsOrNot(){
		 try{ 
			 
			  // Checking the record count, which are valid i.e. R record status
			    $this->db->connect();
				//$txn = $this->db->beginTrans();
			  	$sel_qry = 'select count(1) as tot from '.$this->temp_grp_tbl_name.' bt where bt.record_status=\'R\' and bt.group_id!=\'\' and bt.user_id!=\'\'';
				expDebug::dPrintDBAPI('$sel_qry =  ',$sel_qry);
				$this->db->query($sel_qry);
				$res_r_count = $this->db->fetchColumn(); 
				//$this->db->commitTrans();
				$this->db->closeconnect();
				
				if($res_r_count>0 && $res_r_count!=''){
					return true;
				}else{
					return false;
				} 
			
		}catch(Exception $ex){
	      expDebug::dPrint('Error in checking in the tmp table whether having R status records or not  ===>'.$ex->getMessage(),1);
		}
   }  
	

   public function refreshAdminGroupsUserLists($tblDet){
		 try{  
		 			
					$this->db->connect();
					$sel_adm_users='select 
										grp.id as group_id, 
										grp.name,
										grp.added_users as added_userslist,
										group_concat(distinct per.id) as userslist													
									from slt_groups grp 
									left join slt_group_user_mapping usr_map on grp.id=usr_map.group_id and usr_map.group_type=1 and usr_map.user_type != \'R\' 
							 		left join slt_person per on per.id=usr_map.user_id and per.status!=\'cre_usr_sts_del\' and per.id NOT IN (1,2)
									where grp.id!=\'\' and grp.is_admin=1 and grp.status != \'cre_sec_sts_del\' 
									group by grp.id';
					expDebug::dPrintDBAPI('$sel_adm_users -  ',$sel_adm_users);
					$this->db->query($sel_adm_users); 
					$sel_adm_users_res = $this->db->fetchAllResults(); 

					if(count($sel_adm_users_res)>0){ 
						$txn = $this->db->beginTrans();
						foreach($sel_adm_users_res as $sel_grp_key => $sel_grp_val){  
								    //Get the userslist for each group from tmp table and concatenate with existing group userslist table. This will be only applicable for admin							 
									$new_userslist='';
									$sel_grp_val->userslist=trim($sel_grp_val->userslist); 
									
									if($sel_grp_val->userslist!=''){
										$new_userslist=$sel_grp_val->userslist; 
									}
									
									if($sel_grp_val->added_userslist!='' && $new_userslist!=''){
										$new_userslist.=','.$sel_grp_val->added_userslist; 
									}else if($sel_grp_val->added_userslist!='' && $new_userslist==''){
										$new_userslist=$sel_grp_val->added_userslist; 
									}   
									
									 $new_userslist=trim($new_userslist,",");
									
									 expDebug::dPrint('Before Cleanup the duplicate from admin userslist - '.print_r($new_userslist,true),5);
									
									 $cleaned_up_users_list= implode(',', array_keys(array_flip(explode(',', $new_userslist))));
									
									 expDebug::dPrint('After Cleanup the duplicate from admin userslist - '.print_r($cleaned_up_users_list,true),5);
									 
									 $upd_grp_userslist_qry = "UPDATE slt_groups SET userslist=:userslist WHERE id = :groupid";
								 
									  $arg = array( 
												':userslist'=>$cleaned_up_users_list,
												':groupid'=>$sel_grp_val->group_id												 
										);  
								
								 expDebug::dPrintDBAPI('$upd_grp_userslist_qry = ',$upd_grp_userslist_qry, $arg);
								
								 $this->db->execute($upd_grp_userslist_qry,$arg); 
						}
						   $this->db->commitTrans();
					}else{
						expDebug::dPrint('No admin users and groups found to refresh the userslist',4);
					}
					$this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in updating the userlists in added_users at slt_groups table using temp group user batch table  ===>'.$ex->getMessage(),1);
		}
   }  


   public function refreshAdminUserRoles($tblDet){
		 try{		 			
		 		
				    // Delete the admin roles who are all admin users in slt_group_user_mapping table except admin and guest users
					$this->db->connect();
					$txn = $this->db->beginTrans(); 
					
					$del_users_role_qry = 'DELETE FROM users_roles where uid in (select distinct usr.uid as user_uid
											from slt_group_user_mapping usr_map
											inner join slt_groups grps on grps.id=usr_map.group_id and grps.is_admin=1 and usr_map.group_type=1
											inner join slt_person per on per.id=usr_map.user_id and per.id not in (1,2)
											inner join users usr on usr.name=per.user_name 
											where per.custom_dataload='.$tblDet['btc_id'].')';
					
					expDebug::dPrintDBAPI('Delete Query for users_roles table ',$del_users_role_qry);
					$this->db->execute($del_users_role_qry);
					$this->db->commitTrans();
					$this->db->closeconnect();  
		 				  
						  
				       // Insert the roles in users_roles table who are all admin users except admin and guest users
					$this->db->connect();
					$txn = $this->db->beginTrans(); 
					
					$ins_users_role_qry = 'insert into users_roles (uid,rid)
											select distinct usr.uid as user_uid,r.rid as role_id
											from slt_group_user_mapping usr_map
											inner join slt_groups grps on grps.id=usr_map.group_id and grps.is_admin=1 and usr_map.group_type=1
											inner join slt_person per on per.id=usr_map.user_id and per.id not in (1,2)
											inner join users usr on usr.name=per.user_name
											inner join role r on r.name=grps.name
											where usr_map.user_type != \'R\' 
											and per.custom_dataload='.$tblDet['btc_id'].'';
					
					expDebug::dPrintDBAPI('Insert Query for users_roles table ',$ins_users_role_qry);
					$this->db->execute($ins_users_role_qry);
					$this->db->commitTrans();
					$this->db->closeconnect();   
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in updating the users in users_roles table ===>'.$ex->getMessage(),1);
		}
   }  
   
   
   private function cleanupAdminUserRoles(){
		 try{    
					
					$this->db->connect();
					$txn = $this->db->beginTrans(); 
										
					$sel_adm_users_roles='select grp.id as group_id,grp.added_users as added_userslist,
							grp.userslist as userslist,
							r.rid as role_id,r.name as role_name
							from slt_groups grp 
							inner join role r on r.name=grp.name
							where grp.is_admin=1';
					
					expDebug::dPrintDBAPI('$sel_adm_users_roles -  ',$sel_adm_users_roles);
					
					$this->db->query($sel_adm_users_roles); 
					
					$sel_adm_users_roles_res = $this->db->fetchAllResults(); 
					
					expDebug::dPrint('$sel_adm_users_roles_res - '.print_r($sel_adm_users_roles_res,true),5);
					
					if(count($sel_adm_users_roles_res)>0){ 
						
							foreach($sel_adm_users_roles_res as $sel_grp_key => $sel_grp_val){  
								
								 
								    //Get the userslist for each group from tmp table and concatenate with existing group userslist table. This will be only applicable for admin							 
								 							 
									$new_userslist='';
									
									$sel_grp_val->userslist=trim($sel_grp_val->userslist); 
									
									if($sel_grp_val->userslist!=''){
										$new_userslist=$sel_grp_val->userslist; 
									}
									
									if($sel_grp_val->added_userslist!='' && $new_userslist!=''){
										$new_userslist.=','.$sel_grp_val->added_userslist; 
									}else if($sel_grp_val->added_userslist!='' && $new_userslist==''){
										$new_userslist=$sel_grp_val->added_userslist; 
									}   
									
									 $new_userslist=trim($new_userslist,",");
									
									 expDebug::dPrint('Before Cleanup the duplicate from admin userslist - '.print_r($new_userslist,true),5);
									
									 $cleaned_up_users_list= implode(',', array_keys(array_flip(explode(',', $new_userslist))));
									
									 expDebug::dPrint('After Cleanup the duplicate from admin userslist - '.print_r($cleaned_up_users_list,true),5); 
									 
									 
									 //Clean up the admin users roles
									 
									 $del_unneed_userroles_qry='delete from users_roles where rid='.$sel_grp_val->role_id.' and uid !=\'1\''; 
									 
									 if($cleaned_up_users_list!=''){
									     $del_unneed_userroles_qry.=' and uid in (
																		select usr.uid from slt_person per
																		inner join users usr on usr.name=per.user_name
																		where per.id not in ('.$cleaned_up_users_list.'))';	  
									 } 
									 
									 expDebug::dPrintDBAPI('Delete unneeded users roles from users_roles table - $del_unneed_userroles_qry ',$del_unneed_userroles_qry);
									 
									 $this->db->execute($del_unneed_userroles_qry);
									 //$this->db->commitTrans();
									 
									 
									 //Clean up the slt_admin_group_users , who are all not mapped with admin groups
									 $del_unneed_adm_map_qry='delete from slt_admin_group_users where group_id='.$sel_grp_val->group_id.' and user_id not in (1,2)';
									 
									 if($cleaned_up_users_list!=''){
									     $del_unneed_adm_map_qry.=' and user_id not in ('.$cleaned_up_users_list.')';	  
									 } 
									 
									 expDebug::dPrintDBAPI('Delete unneeded admin group mapping from slt_admin_group_users table - $del_unneed_adm_map_qry ',$del_unneed_adm_map_qry);
									 
									 $this->db->execute($del_unneed_adm_map_qry);
									 //$this->db->commitTrans();
				 					 
									 
								     //Clean up the slt_group_user_mapping , who are all not mapped with admin groups
									 $del_unneed_usr_map_qry='delete from slt_group_user_mapping where group_id='.$sel_grp_val->group_id.' and user_id not in (1,2) and group_type=1';
									 
									 if($cleaned_up_users_list!=''){
									     $del_unneed_usr_map_qry.=' and user_id not in ('.$cleaned_up_users_list.')';	  
									 } 
									 
									 expDebug::dPrintDBAPI('Delete unneeded user group mapping from slt_group_user_mapping table - $del_unneed_usr_map_qry ',$del_unneed_usr_map_qry);
									 
									 $this->db->execute($del_unneed_usr_map_qry);
									// $this->db->commitTrans(); 
									 
									 
									 
						   }		
					 }else{
						expDebug::dPrint('No admin roles found',5);
					 } 
					 
					 $this->db->commitTrans(); 
					 $this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in cleanupt the admin users role  ===>'.$ex->getMessage(),1);
		}
   }   


   
    public function updateMandatoryComplianceEnrollments($tblDet){
		 try{   	 
					
					$this->db->connect();
					$txn = $this->db->beginTrans();
					$userListQuery = "SELECT 
										per.id
									  FROM 
										". $tblDet['tblname'] ." tm
									  INNER JOIN slt_person per ON tm.user_name = per.user_name
									  WHERE per.`status` = 'cre_usr_sts_atv' and tm.record_status IN ('R')";
					expDebug::dPrintDBAPI('user list query -  ',$userListQuery);
					$this->db->query($userListQuery);
					$userList = $this->db->fetchAllResults();
					expDebug::dPrint('active user list - '.print_r($userList,true),4);
					
					
					if (count($userList) > 0) {
						require_once DRUPAL_ROOT . '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_people/exp_sp_administration_user/exp_sp_administration_user.inc';
						foreach ($userList as $user) {
							$upd_user_id = $user->id;
							$enrollCntQuery = "select count(1) from slt_enrollment where user_id = :user_id";
							//expDebug::dPrintDBAPI('$enrollCntQuery -  ',$enrollCntQuery, array(':user_id'=> $upd_user_id));
							$this->db->query($enrollCntQuery,array(':user_id'=> $upd_user_id));
							$enrollCnt = $this->db->fetchColumn();
							//expDebug::dPrint('$enrollCnt: ' . $enrollCnt);
							
							$msEnrollCntQuery = "select count(1) from slt_master_enrollment where user_id = :user_id";
							//expDebug::dPrintDBAPI('$msEnrollCntQuery -  ',$msEnrollCntQuery, array(':user_id'=> $upd_user_id));
							$this->db->query($msEnrollCntQuery,array(':user_id'=> $upd_user_id));
							$msEnrollCnt = $this->db->fetchColumn();
							//expDebug::dPrint('$msEnrollCnt: ' . $msEnrollCnt);
							
							if ($enrollCnt > 0 || $msEnrollCnt > 0 ) {
								
								$resultMan =  getGrpEntityIdListByUser($upd_user_id,'cre_sys_inv_man');
								$comp_status = "'lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_exp'";
								$overlAllStatus = "'lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_exp'";
								$created_by = array($upd_user_id,2);
								$created_by_str = implode(',', $created_by);
								
								$enrollUpQry = "update slt_enrollment set mandatory=null, updated_on=now() where user_id= ".$upd_user_id." AND created_by IN (". $created_by_str .") AND comp_status NOT IN (".$comp_status.") AND master_enrollment_id is null";
								expDebug::dPrintDBAPI('$enrollUpQry -  ',$enrollUpQry);
								$this->db->query($enrollUpQry);
									
								$msEnrollUpQry = "update slt_master_enrollment set mandatory=null,updated_on=now() where user_id= ".$upd_user_id." AND created_by IN (". $created_by_str .") AND overall_status NOT IN (".$overlAllStatus.")";
								expDebug::dPrintDBAPI('$msEnrollUpQry -  ',$msEnrollUpQry);
								$this->db->query($msEnrollUpQry);
								
								// Update mandatory enrollments
								if (count($resultMan) > 0) {
									
									$entityArr = array();
									foreach($resultMan as $arr){
										if($arr->entity_type == 'cre_sys_obt_cls')
											$entityArr['class'][] = $arr->entity_id;
										if($arr->entity_type == 'cre_sys_obt_cur' || $arr->entity_type == 'cre_sys_obt_trn' || $arr->entity_type == 'cre_sys_obt_crt')
											$entityArr['tp'][] = $arr->entity_id;
									}
									
									if(count($entityArr['class']) > 0) {
										$entityArrClass = implode(',', $entityArr['class']);
										$enrollUpQry1 = "update slt_enrollment set mandatory='Y',updated_on=now() where user_id = ".$upd_user_id." AND class_id IN (".$entityArrClass.") AND comp_status NOT IN (".$comp_status.") AND master_enrollment_id is null";
										expDebug::dPrintDBAPI('$enrollUpQry1 -  ',$enrollUpQry1);
										$this->db->query($enrollUpQry1);
									}
									if(count($entityArr['tp']) > 0){
										$entityArrTp = implode(',', $entityArr['tp']);
										$msEnrollUpQry1 = "update slt_master_enrollment set mandatory='1',updated_on=now() where user_id = ".$upd_user_id." AND program_id IN (".$entityArrTp.") AND overall_status NOT IN (".$overlAllStatus.")";
										expDebug::dPrintDBAPI('$msEnrollUpQry1 -  ',$msEnrollUpQry1);
										$this->db->query($msEnrollUpQry1);
									}	
									
								}	
								
								$Complainceresult =  getGrpEntityIdListByUser($upd_user_id,'cre_sys_inv_opt');
								
								$complianceAccess = compliance_access();
								foreach ($complianceAccess as $class_id){
									$class_ids[] = $class_id->class_id;
								}
									
								$comUpdateQuery = "update slt_enrollment set is_compliance=null,updated_on=now() where user_id= ".$upd_user_id." AND comp_status NOT IN (".$comp_status.") AND master_enrollment_id is null";
								if (!empty($class_ids)) {
									$class_ids_str = implode(',', $class_ids);
									$comUpdateQuery .= " AND class_id NOT IN (".$class_ids_str.")";
									expDebug::dPrintDBAPI('$comUpdateQuery -  ',$comUpdateQuery);
									$this->db->query($comUpdateQuery);
								} else {
									expDebug::dPrintDBAPI('$comUpdateQuery -  ',$comUpdateQuery);
									$this->db->query($comUpdateQuery);
								}
								
								if (count($Complainceresult) > 0) {
									$entityArr1 = array();
									foreach($Complainceresult as $arr){
										$is_compliance = checkCourseClassCompliance($arr->entity_id,$arr->entity_type);
										if($is_compliance[0]->is_compliance == 1){
											if($arr->entity_type == 'cre_sys_obt_cls')
												$entityArr1['class'][] = $arr->entity_id;
										}
									}
									if(count($entityArr1['class']) > 0){
										$class_ids_str = implode(',', $entityArr1['class']);
										$comUpdQuery1 = "update slt_enrollment set is_compliance='1',updated_on=now() where user_id = ".$upd_user_id." AND class_id IN (".$class_ids_str.") AND comp_status NOT IN (".$comp_status.") AND master_enrollment_id is null";
										expDebug::dPrintDBAPI('$comUpdQuery1 -  ',$comUpdQuery1);
										$this->db->query($comUpdQuery1);
									}
									
								}	
								
								
							} else {
								expDebug::dPrint('No users and groups found to update the Mandatory / Compliance Enrollments',5);
							}
						}
					}
					
										
					/*$sel_grp_users='select grp.id as group_id, grp.name as group_name,grp.added_users as added_userslist,group_concat(distinct per.id) as userslist													
							from slt_groups grp 
							left join slt_group_user_mapping usr_map on grp.id=usr_map.group_id and usr_map.user_type != \'R\'
							 left join slt_person per on per.id=usr_map.user_id and per.status!=\'cre_usr_sts_del\' and per.id NOT IN (1,2)
							 where grp.id!=\'\' and grp.status != \'cre_sec_sts_del\' 
							group by grp.id';
					
					expDebug::dPrintDBAPI('$sel_grp_users -  ',$sel_grp_users);
					
					$this->db->query($sel_grp_users); 
					
					$sel_grp_users_res = $this->db->fetchAllResults(); 
					
					expDebug::dPrint('$sel_grp_users_res - '.print_r($sel_grp_users_res,true),5);
					
					if(count($sel_grp_users_res)>0){
						
						    $comp_status = array('lrn_crs_cmp_cmp','lrn_crs_cmp_inc','lrn_crs_cmp_exp');
							$overlAllStatus = array('lrn_tpm_ovr_cmp','lrn_tpm_ovr_inc','lrn_tpm_ovr_exp');
						
							foreach($sel_grp_users_res as $sel_grp_key => $sel_grp_val){  
								
								 
								    //Get the userslist for each group from tmp table and concatenate with existing group userslist table. This will be only applicable for admin							 
								 							 
									$new_userslist='';
									
									$sel_grp_val->userslist=trim($sel_grp_val->userslist); 
									
									if($sel_grp_val->userslist!=''){
										$new_userslist=$sel_grp_val->userslist; 
									}
									
									if($sel_grp_val->added_userslist!='' && $new_userslist!=''){
										$new_userslist.=','.$sel_grp_val->added_userslist; 
									}else if($sel_grp_val->added_userslist!='' && $new_userslist==''){
										$new_userslist=$sel_grp_val->added_userslist; 
									}   
									
									 $new_userslist=trim($new_userslist,",");
									
									 expDebug::dPrint('Before Cleanup the duplicate from admin userslist - '.print_r($new_userslist,true),5);
									
									 $cleaned_up_users_list= implode(',', array_keys(array_flip(explode(',', $new_userslist))));
									
									 expDebug::dPrint('After Cleanup the duplicate from admin userslist - '.print_r($cleaned_up_users_list,true),5);
									 
									 $userslist = $cleaned_up_users_list;  
									
									 // Update NULL to all Mandatory enrollments if any users fall in this group  
									   
									$qry = 'UPDATE slt_enrollment e 
											INNER JOIN (
											  SELECT DISTINCT c1.course_id crs_id 
											  FROM slt_group_mapping sgm
											  INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type=\'cre_sys_obt_cls\')  
											    OR (sgm.entity_id=c1.course_id AND sgm.entity_type=\'cre_sys_obt_crs\')
											  WHERE sgm.group_id ='.$sel_grp_val->group_id.'  
											  AND sgm.mro=\'cre_sys_inv_man\'
											  AND c1.status = \'lrn_cls_sts_atv\') crs ON  crs.crs_id = e.course_id
											SET e.mandatory = NULL
											WHERE e.comp_status NOT IN (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_exp\') AND e.mandatory IS NOT NULL';
					 
									expDebug::dPrintDBAPI("Update enrollments as null to mandatory enrollments when group changes ",$qry);
									
									$this->db->execute($qry);
									
									
									 // Update NULL to all Mandatory enrollments if any users fall in this group  
									   
									$qry_tp = 'UPDATE slt_master_enrollment e 
												INNER JOIN (
												SELECT DISTINCT c1.id crs_id 
												FROM slt_group_mapping sgm
												INNER JOIN slt_program c1 ON sgm.entity_id=c1.id AND sgm.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')
												WHERE sgm.group_id = '.$sel_grp_val->group_id.' 
												AND sgm.mro=\'cre_sys_inv_man\'
												AND c1.status = \'lrn_lpn_sts_atv\') crs ON  crs.crs_id = e.program_id
												SET e.mandatory = NULL
												WHERE e.overall_status NOT IN (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_inc\',\'lrn_tpm_ovr_exp\') AND e.mandatory IS NOT NULL';
					 
									expDebug::dPrintDBAPI("Update master enrollments as null to mandatory enrollments when group changes ",$qry);
									
									$this->db->execute($qry_tp);
									
									
									// Update NULL to all Compliance enrollments if any users fall in this group  
									   
									$qry_cmpl = 'UPDATE
												slt_enrollment e 
												INNER JOIN (
												SELECT DISTINCT c1.course_id crs_id 
												FROM slt_group_mapping sgm
												INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type=\'cre_sys_obt_cls\')  
												OR (sgm.entity_id=c1.course_id AND sgm.entity_type=\'cre_sys_obt_crs\')
												WHERE sgm.group_id = '.$sel_grp_val->group_id.' 
												AND sgm.mro=\'cre_sys_inv_opt\'
												AND c1.status = \'lrn_cls_sts_atv\'
												AND c1.is_compliance = 1) crs ON  crs.crs_id = e.course_id
												SET e.is_compliance = NULL
												WHERE e.comp_status NOT IN (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_exp\') AND e.is_compliance IS NOT NULL';
					 
									expDebug::dPrintDBAPI("Update enrollments as null to compliance enrollments when group changes ",$qry_cmpl);
									
									$this->db->execute($qry_cmpl); 
									
									
									 // Update Y to all enrollments and TP as Mandatory if any users fall in this group  
						          
									 if($userslist!=''){ 
										  
										    //For Mandatory Enrollment
											$qry_mand_enr = 'UPDATE slt_enrollment e 
													INNER JOIN (
													  SELECT DISTINCT c1.course_id crs_id 
													  FROM slt_group_mapping sgm
													  INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type=\'cre_sys_obt_cls\')  
													    OR (sgm.entity_id=c1.course_id AND sgm.entity_type=\'cre_sys_obt_crs\')
													  WHERE sgm.group_id ='.$sel_grp_val->group_id.'  
													  AND sgm.mro=\'cre_sys_inv_man\'
													  AND c1.status = \'lrn_cls_sts_atv\') crs ON  crs.crs_id = e.course_id
													SET e.mandatory = \'Y\'
													WHERE e.comp_status NOT IN (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_exp\') AND e.mandatory IS NULL AND e.user_id IN ('.$userslist.')';
							 
											expDebug::dPrintDBAPI("Update enrollments as Y to mandatory enrollments when group changes ",$qry_mand_enr);
											
											$this->db->execute($qry_mand_enr); 
											
											//For Mandatory TP Enrollment
										  $qry_mand_tp_enr = 'UPDATE slt_master_enrollment e 
																INNER JOIN (
																SELECT DISTINCT c1.id crs_id 
																FROM slt_group_mapping sgm
																INNER JOIN slt_program c1 ON sgm.entity_id=c1.id AND sgm.entity_type IN (\'cre_sys_obt_cur\',\'cre_sys_obt_trn\',\'cre_sys_obt_crt\')
																WHERE sgm.group_id = '.$sel_grp_val->group_id.' 
																AND sgm.mro=\'cre_sys_inv_man\'
																AND c1.status = \'lrn_lpn_sts_atv\') crs ON  crs.crs_id = e.program_id
																SET e.mandatory = \'Y\'
																WHERE e.overall_status NOT IN (\'lrn_tpm_ovr_cmp\',\'lrn_tpm_ovr_inc\',\'lrn_tpm_ovr_exp\') AND e.mandatory IS NULL AND e.user_id IN ('.$userslist.')';
							 
											expDebug::dPrintDBAPI("Update master enrollments as Y to mandatory enrollments when group changes ",$qry_mand_tp_enr);
											
											$this->db->execute($qry_mand_tp_enr);   
											
											
											//For Compliance Enrollment 
										 	$qry_cmpl_enr = 'UPDATE
																slt_enrollment e 
																INNER JOIN (
																SELECT DISTINCT c1.course_id crs_id 
																FROM slt_group_mapping sgm
																INNER JOIN slt_course_class c1 ON (sgm.entity_id=c1.id AND sgm.entity_type=\'cre_sys_obt_cls\')  
																OR (sgm.entity_id=c1.course_id AND sgm.entity_type=\'cre_sys_obt_crs\')
																WHERE sgm.group_id = '.$sel_grp_val->group_id.' 
																AND sgm.mro=\'cre_sys_inv_opt\'
																AND c1.status = \'lrn_cls_sts_atv\'
																AND c1.is_compliance = 1) crs ON  crs.crs_id = e.course_id
																SET e.is_compliance = 1
																WHERE e.comp_status NOT IN (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inc\',\'lrn_crs_cmp_exp\') AND e.is_compliance IS NULL AND e.user_id IN ('.$userslist.')';
							 
											expDebug::dPrintDBAPI("Update enrollments as Y to compliance enrollments when group changes ",$qry_cmpl_enr);
											
											$this->db->execute($qry_cmpl_enr);  
											
									 } 
									 
									 
						   }		
					 }else{
						expDebug::dPrint('No users and groups found to update the Mandatory / Compliance Enrollments',5);
					 }
					*/
				    $this->db->commitTrans();
				    $this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in updating the Mandatory / Compliance Enrollments  ===>'.$ex->getMessage(),1);
		}
   }   
 
 
   private function updateRemovedUsersInGroups($tblDet=array()){
		 try{
		 					
		 			expDebug::dPrint('$tblDet - '.print_r($tblDet,true),5);
		 	
					$this->db->connect();
					$txn = $this->db->beginTrans();   
											
				     $sel_removed_grp_lists='select 
				     							bt.group_id as group_id, 
				     							group_concat(bt.user_id) as added_userslist, 
				     							grp.removed_users as removed_userslist 
				     						 from '.$this->temp_grp_tbl_name.' bt 
				   							 inner join slt_groups grp ON grp.id=bt.group_id
											 where 
				     						 		bt.is_removed_user=1 and bt.record_status=\'R\' 
				     						 		and bt.user_id!=\'\' and bt.group_id!=\'\'
											 group by bt.group_id';  
					
					
					expDebug::dPrintDBAPI('$sel_removed_grp_lists ',$sel_removed_grp_lists);
					
					$this->db->query($sel_removed_grp_lists); 
					
					$sel_removed_grp_lists_res = $this->db->fetchAllResults();
 					
					expDebug::dPrint('Removed Users List Group Result from tmp table - $sel_removed_grp_lists_res - '.print_r($sel_removed_grp_lists_res,true),5);
					
					if(count($sel_removed_grp_lists_res)>0){
						
						foreach($sel_removed_grp_lists_res as $sel_grp_key => $sel_grp_val){
							
							
							$grp_removed_userslist_arr=explode(',',$sel_grp_val->removed_userslist);
							$grp_added_userslist_arr=explode(',',$sel_grp_val->added_userslist); 
							$new_grp_removed_userslist_arr=array_diff($grp_removed_userslist_arr,$grp_added_userslist_arr);
							
							$new_grp_removed_userslist=implode(',',$new_grp_removed_userslist_arr);
							
							
							expDebug::dPrint('$grp_removed_userslist_arr - '.print_r($grp_removed_userslist_arr,true),5);
							expDebug::dPrint('$grp_added_userslist_arr - '.print_r($grp_added_userslist_arr,true),5);
							expDebug::dPrint('$new_grp_removed_userslist_arr - '.print_r($new_grp_removed_userslist_arr,true),5);
							expDebug::dPrint('$new_grp_removed_userslist - '.print_r($new_grp_removed_userslist,true),5); 
							
								$up_group_removed_qry = "UPDATE slt_groups SET removed_users = :removeduserslist, updated_by=:upd_by, updated_on=:upd_on";
								 
								 $arg = array(
												':removeduserslist'=>$new_grp_removed_userslist,
												':groupid'=>$sel_grp_val->group_id,
												':upd_by'=>$tblDet['user_id'],
												':upd_on'=>now()
										     );  
	
								
								 $up_group_removed_qry.=' WHERE id = :groupid';
								
								 expDebug::dPrintDBAPI('$up_group_removed_qry = ',$up_group_removed_qry);
								 expDebug::dPrint('$up_group_removed_qry  - $arg '.print_r($arg,true),5);
								
								 $this->db->execute($up_group_removed_qry,$arg);   
							
						}
					}else{
						expDebug::dPrint('No removed users found to update when adding the removed users in same group',5);
					}
					
					$this->db->commitTrans();
					$this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in updating the removed users in Group  ===>'.$ex->getMessage(),1);
		}
   }


  private function retainingAddedUsers($tblDet=array()){
		 try{
		 					
		 			expDebug::dPrint('$tblDet - '.print_r($tblDet,true),5);
		 	
					$this->db->connect();
					$txn = $this->db->beginTrans();   
											
				     $sel_grp_lists='select grp.id as group_id, grp.code as group_code, grp.status as group_status,grp.added_users as group_added_users,
				     						grp.is_admin as group_is_admin from slt_groups grp where grp.status!=\'cre_sec_sts_del\' and grp.added_users!=\'\'';  
					
					
					expDebug::dPrintDBAPI('$sel_grp_lists ',$sel_grp_lists);
					$this->db->query($sel_grp_lists); 
					$sel_grp_lists_res = $this->db->fetchAllResults();
 					expDebug::dPrint('Group Lists - $sel_grp_lists_res - '.print_r($sel_grp_lists_res,true),5);
					
					if(count($sel_grp_lists_res)>0){
						
						foreach($sel_grp_lists_res as $sel_grp_key => $sel_grp_val){
							
							
							
							if($sel_grp_val->group_added_users!=''){
								
								//Delete the user group mappings
								
								/*$del_usr_grp_qry = 'delete from slt_group_user_mapping usr_map 
												inner join slt_groups usr_grp on usr_grp.id=usr_map.group_id 
												where usr_map.user_id in ('.$sel_grp_val->group_added_users.') and usr_map.group_id='.$sel_grp_val->group_id; */
												
							     $del_usr_grp_qry = 'delete from slt_group_user_mapping 									 
												     where user_id in ('.$sel_grp_val->group_added_users.') and group_id='.$sel_grp_val->group_id;
							
								expDebug::dPrintDBAPI('delete Query for slt_group_user_mapping (learner and admin) - $del_usr_grp_qry ',$del_usr_grp_qry);
								$this->db->execute($del_usr_grp_qry);
								
								// reinsert the user group mappings
								
								$ins_usr_grp_qry = 'INSERT INTO slt_group_user_mapping (user_id, user_status, group_id, group_type, group_status, created_by, created_on, updated_by, updated_on)
													select per.id,per.status,'.$sel_grp_val->group_id.','.$sel_grp_val->group_is_admin.',\''.$sel_grp_val->group_status.'\','.$tblDet['user_id'].',now(),'.$tblDet['user_id'].',now() from slt_person per where per.id in ('.$sel_grp_val->group_added_users.')'; 
							
								expDebug::dPrintDBAPI('Insert Query for slt_group_user_mapping (learner and admin) - $del_usr_grp_qry ',$ins_usr_grp_qry);
								$this->db->execute($ins_usr_grp_qry);
								  
							    
								
								if($sel_grp_val->group_is_admin=='1'){ //if admin
								
										
										//Delete the admin group mappings
								
										/* $del_adm_grp_qry = 'delete from slt_admin_group_users adm_map 
														inner join slt_groups adm_grp on adm_grp.id=adm_map.group_id 
														where adm_map.user_id in ('.$sel_grp_val->group_added_users.') and adm_map.group_id='.$sel_grp_val->group_id;
										 */
										 
										 $del_adm_grp_qry = 'delete from slt_admin_group_users														 
														where user_id in ('.$sel_grp_val->group_added_users.') and group_id='.$sel_grp_val->group_id;
									
										expDebug::dPrintDBAPI('delete Query for slt_admin_group_users (admin) - $del_usr_grp_qry ',$del_adm_grp_qry);
										$this->db->execute($del_adm_grp_qry);
								
								
								 		$ins_admin_qry = 'INSERT INTO slt_admin_group_users (group_id, user_id, group_code, status, created_by, created_on, updated_by, updated_on) 
														  select '.$sel_grp_val->group_id.',per.id,\''.$sel_grp_val->group_code.'\',\''.$sel_grp_val->group_status.'\','.$tblDet['user_id'].',now(),'.$tblDet['user_id'].',now() from slt_person per where per.id in ('.$sel_grp_val->group_added_users.')';
															
										expDebug::dPrintDBAPI('Insert Query for slt_admin_group_users (admin group users) - $ins_admin_qry ',$ins_admin_qry);
										$this->db->execute($ins_admin_qry); 
				
									
								}	
								
							}   
							
						}
					}else{
						expDebug::dPrint('No groups available to retaining',5);
					}
					
					$this->db->commitTrans();
					$this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in retainingAddedUsers  ===>'.$ex->getMessage(),1);
		}
   }
   
 
  
   private function updateAddedUsersInGroups($tblDet=array()){
		 try{
		 					
		 			expDebug::dPrint('$tblDet - '.print_r($tblDet,true),5);
		 	
					$this->db->connect();
					$txn = $this->db->beginTrans(); 					 
					 
					$sel_added_grp_lists=' SELECT 
												bt.group_id as group_id,
												grp.is_admin as group_is_admin,
												grp.userslist as group_userslist,
												grp.added_users as group_added_userslist,
												group_concat(user_id) as bt_group_lists
											FROM  
												'.$this->temp_grp_tbl_name.'  bt 
											INNER JOIN slt_groups grp ON grp.id=bt.group_id
											WHERE 
												bt.record_status=\'R\' AND group_id!=\'\' GROUP BY group_id';  
					
					
					expDebug::dPrintDBAPI('selecting the added userslist and tmp batch table - $sel_added_grp_lists ',$sel_added_grp_lists);
					
					$this->db->query($sel_added_grp_lists); 
					
					$sel_added_grp_lists_res = $this->db->fetchAllResults();
					//expDebug::dPrintDBAPI('Added Users List Group Result - $sel_added_grp_lists_res ',$sel_added_grp_lists_res);
					
					 expDebug::dPrint('Added Users List Group Result - $sel_added_grp_lists_res - '.print_r($sel_added_grp_lists_res,true),5);
					
					if(count($sel_added_grp_lists_res)>0){
						
						foreach($sel_added_grp_lists_res as $sel_grp_key => $sel_grp_val){
							
							//Get the userslist for each group from tmp table and concatenate with existing group added_userslist table
							$new_added_userslist='';
							
							$old_added_userslist='';
							
							$sel_grp_val->group_added_userslist=trim($sel_grp_val->group_added_userslist);
							$sel_grp_val->bt_group_lists=trim($sel_grp_val->bt_group_lists); 
							
							if($sel_grp_val->group_added_userslist!=''){
								$new_added_userslist=$sel_grp_val->group_added_userslist; 
								$old_added_userslist=$sel_grp_val->group_added_userslist;
							}
							
							expDebug::dPrint('$new_added_userslist - '.print_r($new_added_userslist,true),5);
							
							if($sel_grp_val->bt_group_lists!='' && $new_added_userslist!=''){
								$new_added_userslist.=','.$sel_grp_val->bt_group_lists; 
							}else if($sel_grp_val->bt_group_lists!='' && $new_added_userslist==''){
								$new_added_userslist=$sel_grp_val->bt_group_lists; 
							}   
							
							
							$new_added_userslist=trim($new_added_userslist,",");
							
							 expDebug::dPrint('Before Cleanup the duplicate from addedusers list - '.print_r($new_added_userslist,true),5);
							
							$cleaned_up_added_users_list= implode(',', array_keys(array_flip(explode(',', $new_added_userslist))));
							
							 expDebug::dPrint('After Cleanup the duplicate from addedusers list - '.print_r($cleaned_up_added_users_list,true),5);
							 
							 
							$up_group_added_qry = "UPDATE slt_groups SET added_users = :adduserslist, updated_by=:upd_by, updated_on=:upd_on";
							 
							$arg = array(
								':adduserslist'=>$cleaned_up_added_users_list,
								':groupid'=>$sel_grp_val->group_id,
								':upd_by'=>$tblDet['user_id'],
								':upd_on'=>now()
							);  

							
							 $up_group_added_qry.=' WHERE id = :groupid';
							
							 expDebug::dPrintDBAPI('$up_group_added_qry = ',$up_group_added_qry);
							 expDebug::dPrint('$up_group_added_qry  - $arg '.print_r($arg,true),5);
							
							 $this->db->execute($up_group_added_qry,$arg); 
							 
							 
							 //Add the users in slt_groups_audit table  
							 
							 $old_added_userslist_arr=array();
							 $new_added_userslist_arr=array();
							 
							 if($old_added_userslist!=''){
							   $old_added_userslist_arr=explode(',',$old_added_userslist);
							 }
							 
							 if($cleaned_up_added_users_list!=''){
							   $new_added_userslist_arr=explode(',',$cleaned_up_added_users_list);
							 }
							 
							 $new_grp_added_userslist=array_diff($new_added_userslist_arr,$old_added_userslist_arr);
							 
							 expDebug::dPrint('$old_added_userslist -  '.print_r($old_added_userslist,true),5);
							 expDebug::dPrint('$new_added_userslist_arr 1 -  '.print_r($new_added_userslist_arr,true),5);
							 
							 
							 expDebug::dPrint('$new_grp_added_userslist Differnce before and after -  '.print_r($new_grp_added_userslist,true),5);
							 
							 
							 if(count($new_grp_added_userslist)>0){
							 	
								$ins_str='';
							    $ins_tbl_str='insert into slt_group_audit (group_id,user_id,operation,criteria_match,updated_by,updated_on) values ';
							 
							 	 foreach($new_grp_added_userslist as $grp_usr_key => $grp_usr_res){
							 	 	 $ins_str.=',('.$sel_grp_val->group_id.','.$grp_usr_res.',\'added\',\'N\','.$tblDet['user_id'].',now()'.')'; 
							 	 }
 									$ins_str=trim($ins_str,','); //trim the before comma
 									
 								expDebug::dPrint('$ins_tbl_str -  '.print_r($ins_tbl_str,true),5); 
							    expDebug::dPrint('$ins_str -  '.print_r($ins_str,true),5); 
								
								$tmp_grp_aud_ins_str=$ins_tbl_str.$ins_str; 
								 
								expDebug::dPrintDBAPI('$tmp_grp_aud_ins_str = ',$tmp_grp_aud_ins_str); 
							    $this->db->execute($tmp_grp_aud_ins_str);   
							 } 
							 
							 
							
						}
					}else{
						expDebug::dPrint('No groups found to update',5);
					}
					
					$this->db->commitTrans();
					$this->db->closeconnect();  
			
		 	}catch(Exception $ex){
	          expDebug::dPrint('Error in updating the userlists in added_users at slt_groups table using temp group user batch table  ===>'.$ex->getMessage(),1);
		}
   }  
   
  
   private function addUsersInAdminAndLearnerGroups($tblDet=array()){
		 try{
		 	
				 // Take only the requirred records (R Stauts) from tmp batch table and insert into the slt_group_user_mapping and slt_admin_group_users table according to group_type
				
				expDebug::dPrint('$tblDet - '.print_r($tblDet,true),5);
				
				// Insert learner / admin group users in  slt_group_user_mapping   
				$this->db->connect();
				$txn = $this->db->beginTrans();
				
				$ins_lnr_qry = 'INSERT INTO slt_group_user_mapping (user_id, user_status, group_id, group_type, group_status,user_type, created_by, created_on, updated_by, updated_on)
							SELECT bt.user_id,bt.user_status,bt.group_id,bt.group_type,bt.group_status,"A" AS user_type,'.$tblDet['user_id'].',now(),'.$tblDet['user_id'].',now() FROM '.$this->temp_grp_tbl_name.' bt WHERE bt.record_status=\'R\'';
							
				expDebug::dPrintDBAPI('Insert Query for slt_group_user_mapping (learner and admin) - $ins_lnr_qry ',$ins_lnr_qry);
				$this->db->execute($ins_lnr_qry);
				$this->db->commitTrans();
				$this->db->closeconnect();
				
				
				 //Insert only the admin group users in slt_admin_group_users. Take only the R status and admin group users i.e. group_type is 1
				 
				$this->db->connect();
				$txn = $this->db->beginTrans();
				
				$ins_admin_qry = 'INSERT INTO slt_admin_group_users (group_id, user_id, group_code, status, created_by, created_on, updated_by, updated_on) 
							SELECT bt.group_id,bt.user_id,bt.group_code,bt.group_status,'.$tblDet['user_id'].',now(),'.$tblDet['user_id'].',now() FROM '.$this->temp_grp_tbl_name.' bt where bt.record_status=\'R\' and bt.group_type=\'1\'';
							
				expDebug::dPrintDBAPI('Insert Query for slt_admin_group_users (admin group users) - $ins_admin_qry ',$ins_admin_qry);
				$this->db->execute($ins_admin_qry);
				$this->db->commitTrans();
				$this->db->closeconnect();  
				
				// Adding learner users in userslist in slt_groups 
				
				$this->updateAddedUsersInGroups($tblDet);   
			
			
		 	}catch(Exception $ex){
	      expDebug::dPrint('Error in updating the group and users temp batch table  ===>'.$ex->getMessage(),1);
		}
   } 
   
   
   private function removeTmpBatchTable(){
   	try{
   				 
   		$this->db->connect();
   		$txn = $this->db->beginTrans();
    
   		$del_tmp_tbl_qry='drop table if exists '.$this->temp_grp_tbl_name;
   			
   		expDebug::dPrintDBAPI('$del_tmp_tbl_qry ',$del_tmp_tbl_qry);
   		$this->db->execute($del_tmp_tbl_qry);
   		$this->db->commitTrans();
   		$this->db->closeconnect(); 
   			
   	}catch(Exception $ex){
   		expDebug::dPrint('Error in remving the removeTmpBatchTable ===>'.$ex->getMessage(),1);
   	}
   }
   
   
	public function bulkExecute($tblDet=array()){
		try{
			
			    $this->temp_grp_tbl_name=$tblDet['tblname'].'_groups';
				expDebug::dPrint('$this->temp_grp_tbl_name - '.$this->temp_grp_tbl_name,1);
				
				
				//Retaining the added users in group
				//$this->retainingAddedUsers($tblDet); // Need to check whether needed or not
				 
				//After creatting the tmp table, call the updateUserGroupsIdsAndStatus() to update the users and groups 
				$this->updateUserGroupsIdsAndStatusInTmpTbl();   
				
				//Need to proceed further or not according to Status in Group Tmp table. Count will be retrived according to R status.
				$addUserCountPresent = $this->checkValidGroupsOrNot();   
				
				if($addUserCountPresent){ //if there are valid records then proceed to add groups or else skip this process
					
					//Adding users in Group table. 
					$this->addUsersInAdminAndLearnerGroups($tblDet);  

					//Update the removed userslist according to uploading users in Group.
					$this->updateRemovedUsersInGroups($tblDet);
					
				} 	
				
				//Refresh the userslist of admin group according to feedfile users
				$this->refreshAdminGroupsUserLists($tblDet);   
				
				//Refresh the admin roles table
				$this->refreshAdminUserRoles($tblDet);  
				
				//Cleanup the admin users roles from users_roles table
				$this->cleanupAdminUserRoles();  
				
				//Update the is_instructor and is_manager group in slt_person when adding the instructor and manager group
				if ($this->checkManagerInstuctorGroupIsExist()){
					$this->updateManagerInstructorInPersonTable(); // Need
				}
				
				// Update all enrollments as Mandatory / Compliance if any users fall in this group
				$this->updateMandatoryComplianceEnrollments($tblDet);
					
					
				//Remove tmp batch table after process
				$this->removeTmpBatchTable();
			
		}catch(Exception $ex){
			expDebug::dPrint('Error in adding leaner/admin group with user ===>'.$ex->getMessage(),1);
	
		}
	}

}

?>
