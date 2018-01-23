<?php
include_once $_SERVER['DOCUMENT_ROOT'] .  '/reports/ReportSyncUp.php';
/**
 * report_survey_assessment_question
 * 
 * Table Informations
 * slt_survey_questions 			- Soft Delete
 * slt_survey_groups_questions 		 	- Hard Delete
 * slt_survey_groups grp 				- No Delete
 * slt_profile_list_items				- No Delete	
 * 
 * 
 */
class report_survey_assessment_question extends ReportSyncUp {
	
	
	private $report_table = '';
	
	private $temp_table_name = '';
	
	/**
	 * Class Constructor
	 * @param object $result
	 */
	public function __construct($result){
		parent::__construct($result);
		$this->report_table = __CLASS__;
		$this->temp_table_name = 'temp_' . $this->report_table;
	}

	/**
	 * Method used to Execute Data Syncup Process
	 */
	public function reportExecute(){
		try {
			$start = date("Y-m-d H:i:s");
			// Remove deleted records in flat table
			$this->reportDelete();
			
			// Create a temp table to process insert/update records
			$this->createTempTable();
			
			// Data popuation to temp table
			$this->tempTableDataPopulate();
			
			// Update records in flat table
			$this->reportUpdate();
			
			// Insert a new records to flat table
			$this->reportInsert();
			
			$this->dropTempTable(); // drop temp table
			$end = date("Y-m-d H:i:s");
		} catch(Exception $ex) {
			if(!isset($end)) {
				$end = date("Y-m-d H:i:s");
			}
			$this->dropTempTable(); // drop temp table
			$this->write_log($this->report_table, 'temp', 'FL', $this->updSync_on, $start, $end, $ex->getMessage());
			throw new Exception("Error in reportExecute ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to remove deleted records
	 */
	protected function reportDelete() {
		try {
			$delSubQuery = $this->reportCollectIds('delete');
			$delQuery = "DELETE rprm FROM ". $this->report_table ." as rprm JOIN (".$delSubQuery.") as deleted ON rprm.RUID = deleted.RUID";
			expDebug::dPrint("reportDelete Query" . print_r($delQuery, 1), 4);
			$this->queryProcess($delQuery, $this->report_table, 'delete', $this->delSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportDelete ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to create temp table
	 */
	protected function createTempTable() {
		try {
			//clear-memcache.php
			$dropTable  = "DROP TABLE IF EXISTS " . $this->temp_table_name ."; ";
			$this->db->callQuery($dropTable);
			
			// $createTable = "CREATE TABLE ".$this->temp_table_name." LIKE ".$this->report_table."";
			$createTable = "CREATE TABLE ".$this->temp_table_name." (
							`RUID` varchar(255),
							`SurAssQuestionID` int(11) DEFAULT '0',
							`SurAssQuestion` text Default NULL,
							`SurAssQuestionType` longtext,
							`SurAssAnswerChoice` text  default NULL,
							`SurAssRightAnswer` text,
							`mandatoryOption` varchar(1) DEFAULT 'N',
							`questionScore` float(11,2) DEFAULT NULL,
							`SurAssID` int(11) DEFAULT NULL,
							`SurAssQuestionStatus` longtext,
							`SurQuestionStatus` longtext,
							`QuestionCreatedBy` text,
							`QuestionCreatedOn` datetime DEFAULT NULL,
							`QuestionUpdatedBy` text,
							`QuestionUpdatedOn` datetime DEFAULT NULL,
							`QuestionDeletedBy` text,
							`QuestionDeletedOn` datetime DEFAULT NULL,
							`SurAssGroupName` longtext,
							`SurAssGroupCode` varchar(100) DEFAULT NULL,
							`SurAssType` longtext,
							`SurAssGroupID` int(11) DEFAULT NULL,
							`SurAssSequence` int(11) DEFAULT NULL,
							`SurAssGroupQuestionID` int(11) DEFAULT NULL,
							`SurAssLanguage` longtext,
							`SurAssStatus` longtext,
							`SurAssMandatoryOption` varchar(1) DEFAULT 'N',
							`operation` varchar(10) NULL DEFAULT NULL,
							PRIMARY KEY (RUID),
							key sli_op(operation)
						) ENGINE=InnoDB DEFAULT CHARSET=utf8;";
			$this->db->callQuery($createTable);
			
			// $alterTable = "ALTER TABLE ".$this->temp_table_name." ADD COLUMN `operation` varchar(10) NULL DEFAULT NULL;";
			// $this->db->callQuery($alterTable);
			
		} catch(Exception $ex) {
			throw new Exception("Error in createTempTable ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to drop temp table
	 */
	protected function dropTempTable() {
		try {
			$dropTable  = "DROP TABLE IF EXISTS " . $this->temp_table_name ."; ";
			$this->db->callQuery($dropTable);
		} catch(Exception $ex) {
			throw new Exception("Error in dropTempTable ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to populte delta data to temp table
	 */
	protected function tempTableDataPopulate() {
		try {
			$tempDataQuery = "insert into ".$this->temp_table_name."
					SELECT
						concat_ws('-', grpqus.survey_id, grpqus.question_id, grpqus.id) AS `RUID`,
						`surq`.`id` as `SurAssQuestionID`,
						`surq`.`question_txt` as `SurAssQuestion`,
						`qustype`.`name` as `SurAssQuestionType` ,
						`surq`.`answer_choice_txt` as `SurAssAnswerChoice`,
						`surq`.`right_answer` as `SurAssRightAnswer`,
						`grpqus`.`mandatoryOption` as `mandatoryOption`,
						`grpqus`.`score` as `questionScore`,
						`grpqus`.`survey_id` as `SurAssID`,
						`qussts`.`name` AS `SurAssQuestionStatus`,
						`surq`.`status` AS `SurQuestionStatus`,
						`surq`.`created_by` AS `QuestionCreatedBy`,
						`surq`.`created_on` AS `QuestionCreatedOn`,
						`surq`.`updated_by` AS `QuestionUpdatedBy`,
						`surq`.`updated_on` AS `QuestionUpdatedOn`,
						`surq`.`deleted_by` AS `QuestionDeletedBy`,
						`surq`.`deleted_on` AS `QuestionDeletedOn`,
						`grp`.`title` as `SurAssGroupName`,
						`grp`.`code` AS `SurAssGroupCode`,
						`qustype`.`name` AS `SurAssType`,
						`grpqus`.`survey_group_id` AS `SurAssGroupID`,
						`grpqus`.`sequence` AS `SurAssSequence`,
						`grpqus`.`question_id` AS `SurAssGroupQuestionID`,
						`qustype`.`name` AS `SurAssLanguage`,
						`qustype`.`name` AS `SurAssStatus`,
						`grpqus`.`mandatoryOption` as `SurAssMandatoryOption`,
			             NULL
			        
				    FROM slt_survey_questions surq
						LEFT JOIN slt_survey_groups_questions grpqus ON grpqus.question_id = surq.id
						LEFT JOIN slt_survey_groups grp ON grp.id = grpqus.survey_group_id
						INNER JOIN slt_survey sur ON sur.id = grpqus.survey_id 
						INNER JOIN slt_profile_list_items qustype ON qustype.code = surq.question_type
						INNER JOIN slt_profile_list_items qussts ON qussts.code = surq.status
			        
					where sur.status != 'sry_det_sry_del' AND 
						  ((surq.updated_on > '".$this->updSync_on ."') OR (grp.updated_on > '".$this->updSync_on ."') OR (grpqus.updated_on > '".$this->updSync_on ."'))";
			
			expDebug::dPrint('tempTableDataPopulate query: ' . $tempDataQuery, 4);
			$this->db->callQuery($tempDataQuery);
		} catch (Exception $ex){
			throw new Exception("Error in tempTableDataPopulate ".$ex->getMessage());
		}
	}
	
	/**
	 * Method used to insert into flat table
	 */
	
	protected function reportInsert() {
		try {
			
			// truncate flat table, when script invoked with flush true
			if ($this->flush == 'true') {
				$flushTable  = "TRUNCATE TABLE " . $this->report_table ."; ";
				expDebug::dPrint('$flushTable query :' . $flushTable, 4);
				$this->db->callQuery($flushTable);
			}
			
			$query = "insert into ". $this->report_table ."
					SELECT 
						`RUID`,
						`SurAssQuestionID`,
						`SurAssQuestion`,
						`SurAssQuestionType`,
						`SurAssAnswerChoice`,
						`SurAssRightAnswer`,
						`mandatoryOption`,
						`questionScore`,
						`SurAssID`,
						`SurAssQuestionStatus`,
						`SurQuestionStatus`,
						`QuestionCreatedBy`,
						`QuestionCreatedOn`,
						`QuestionUpdatedBy`,
						`QuestionUpdatedOn`,
						`QuestionDeletedBy`,
						`QuestionDeletedOn`,
						`SurAssGroupName`,
						`SurAssGroupCode`,
						`SurAssType`,
						`SurAssGroupID`,
						`SurAssSequence`,
						`SurAssGroupQuestionID`,
						`SurAssLanguage`,
						`SurAssStatus`,
						`SurAssMandatoryOption`
			        
					FROM ". $this->temp_table_name ."
					where operation is NULL;";
			
			expDebug::dPrint("reportInsert Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'insert', $this->insSync_on);
		} catch(Exception $ex) {
			throw new Exception("Error in reportInsert ".$ex->getMessage());
		}	
	}
	
	/**
	 * Method used to update into flat table
	 */
	protected function reportUpdate() {
		try {
		$query = "update ". $this->report_table ." flat
						join ". $this->temp_table_name ." temp on flat.RUID = temp.RUID
						set
							`flat`.`SurAssQuestionID`	=	`temp`.`SurAssQuestionID`,
							`flat`.`SurAssQuestion`	=	`temp`.`SurAssQuestion`,
							`flat`.`SurAssQuestionType`	=	`temp`.`SurAssQuestionType`,
							`flat`.`SurAssAnswerChoice`	=	`temp`.`SurAssAnswerChoice`,
							`flat`.`SurAssRightAnswer`	=	`temp`.`SurAssRightAnswer`,
							`flat`.`mandatoryOption`	=	`temp`.`mandatoryOption`,
							`flat`.`questionScore`	=	`temp`.`questionScore`,
							`flat`.`SurAssID`	=	`temp`.`SurAssID`,
							`flat`.`SurAssQuestionStatus`	=	`temp`.`SurAssQuestionStatus`,
							`flat`.`SurQuestionStatus`	=	`temp`.`SurQuestionStatus`,
							`flat`.`QuestionCreatedBy`	=	`temp`.`QuestionCreatedBy`,
							`flat`.`QuestionCreatedOn`	=	`temp`.`QuestionCreatedOn`,
							`flat`.`QuestionUpdatedBy`	=	`temp`.`QuestionUpdatedBy`,
							`flat`.`QuestionUpdatedOn`	=	`temp`.`QuestionUpdatedOn`,
							`flat`.`QuestionDeletedBy`	=	`temp`.`QuestionDeletedBy`,
							`flat`.`QuestionDeletedOn`	=	`temp`.`QuestionDeletedOn`,
							`flat`.`SurAssGroupName`	=	`temp`.`SurAssGroupName`,
							`flat`.`SurAssGroupCode`	=	`temp`.`SurAssGroupCode`,
							`flat`.`SurAssType`	=	`temp`.`SurAssType`,
							`flat`.`SurAssGroupID`	=	`temp`.`SurAssGroupID`,
							`flat`.`SurAssSequence`	=	`temp`.`SurAssSequence`,
							`flat`.`SurAssGroupQuestionID`	=	`temp`.`SurAssGroupQuestionID`,
							`flat`.`SurAssLanguage`	=	`temp`.`SurAssLanguage`,
							`flat`.`SurAssStatus`	=	`temp`.`SurAssStatus`,
							`flat`.`SurAssMandatoryOption`	=	`temp`.`SurAssMandatoryOption`,  
							`temp`.`operation` =  'update'	
						where flat.RUID = temp.RUID;";
			expDebug::dPrint("reportUpdate Query" . print_r($query, 1), 4);
			$this->queryProcess($query, $this->report_table, 'update', $this->updSync_on);
		} catch(Exception $ex){
			throw new Exception("Error in reportUpdate ".$ex->getMessage());
		}		
	}
	
	/**
	 * Method used to collect delta Ids
	 * 
	 * @param string $action
	 * @return string
	 */
	protected function reportCollectIds($action) {
		try {
			$query = "SELECT 
							DISTINCT concat_ws('-', grpqus.survey_id, grpqus.question_id, grpqus.id) AS `RUID`
						FROM slt_survey sur
						LEFT JOIN slt_survey_groups_questions grpqus ON grpqus.survey_id = sur.id
						LEFT JOIN slt_survey_groups grp ON grp.id = grpqus.survey_group_id
						where sur.status = 'sry_det_sry_del' and sur.updated_on > '".$this->delSync_on ."'";
			$query .= " UNION ";
			$query .= "SELECT 
							DISTINCT concat_ws('-', del.parent1_entity_id, del.parent3_entity_id, del.entity_id) AS `RUID`
						FROM slt_survey sur
						INNER JOIN report_deleted_logs del ON del.parent1_entity_id = sur.id
						where del.deleted_on > '".$this->delSync_on ."'";
			return $query;
			
		} catch(Exception $ex){
			throw new Exception("Error in reportCollectIds ".$ex->getMessage());
		}	
	}
} 
	
	
