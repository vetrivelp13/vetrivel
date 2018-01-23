<?php 

require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class SolrIndex extends SolrClient{
	// Prefix of the collection
	private $prefix;
	
	// Data import basic options
	private $baseOption = array(
		'command'=>'full-import',
		'verbose'=>'false',
		'clean'=>'true',
		'commit'=>'true',
		'optimize'=>'false',
		'name'=>'dataimport',
		'indend'=>'on',
		'wt'=>'json'
	);
	
	public function __construct(){
		// Get collection prefix from configuration
		$this->prefix = getConfigValue('collection_prefix');
		
		$this->isLog = TRUE;
		expDebug::dPrint("Prefix -- ".$this->prefix,4);
		//Call parent constructor
		parent::__construct();
	}
	
	/**
	 * Index catalog items Course & class
	 * @param $options (Array) Import options
	 * @return NA
	 */
	public function catalogIndexing($options=array()){
		try{
			// Set exact collection name (should match with solr's collection)
			$this->collName = $this->prefix . '_' . SolrClient::CatalogCore;
			
			$this->indexCourse($options);
			
			$this->indexClass($options);
			
		}catch(Exception $e){
			expDebug::dPrint("Error in Catalog Indexing -- ".print_r($e->getMessage(),true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Index User list
	 * @param $options (Array) Import options
	 * @return NA
	 */
	public function userIndexing($options=array(),$entityType='',$singleUser=''){
		try{
 		    if(!empty($singleUser)) {
		        $entityName = 'singleuser';
		        expDebug::dPrint('<---- userIndexing singleUser ---> '.$entityName);
		    } else {
			if(!empty($entityType) && $entityType != 'Login'){
				$this->dataImportType = $entityType;
				$entityName = 'UserBulkUpload';
			}else{
				$entityName = 'person';
			}
		    }
			// Set exact collection name (should match with solr's collection)
			$this->collName = $this->prefix . '_' . SolrClient::UserCore;
			
			$this->createIndex($entityName,$options);
		}catch(Exception $e){
			expDebug::dPrint("Error in User Indexing -- ".print_r($e->getMessage(),true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Index catalog items Training Program
	 * @param $options (Array) Import options
	 * @return NA
	 */
	public function trainingProgramIndexing($options=array()){
		try{
			// Set exact collection name (should match with solr's collection)
			$this->collName = $this->prefix . '_' . SolrClient::CatalogCore;
			
			$options['clean'] = 'false'; // Force clean to false
			$this->createIndex('Program',$options);
			
		}catch(Exception $e){
			expDebug::dPrint("Error in Catalog Indexing -- ".print_r($e->getMessage(),true),1);
			throw new Exception($e->getMessage());
		}
	}
	/**
	 * Index enrollments
	 * @param $options (Array) Import options
	 * @return NA
	 */
	public function enrollmentIndexing($options=array(),$entityType=''){
		try{
			if(!empty($entityType)){
				$this->dataImportType = $entityType;
				$entityName = 'BulkEnrollment';
			}else{
				$entityName = 'Enrollment';
			}
			// Set exact collection name (should match with solr's collection)
			$this->collName = $this->prefix . '_' . SolrClient::EnrollCore;
	
			$this->createIndex($entityName,$options);
	
		}catch(Exception $e){
			expDebug::dPrint("Error in Catalog Indexing -- ".print_r($e->getMessage(),true),1);
			throw new Exception($e->getMessage());
		}
	}
	/**
	 * Index catalog items Course 
	 * @param $options (Array) Import options
	 * @return NA
	 */
	private function indexCourse($options=array()){
		try{
			$this->createIndex('Course',$options);
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Index catalog items class
	 * @param $options (Array) Import options
	 * @return NA
	 */
	private function indexClass($options=array()){
		try{
			$options['clean'] = 'false'; // Force clean to false
			$this->createIndex('Class',$options);
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Index enrollment items Training Program
	 * @param $options (Array) Import options
	 * @return NA
	 */
	public function masterEnrollmentIndexing($options=array(),$entityType=''){
		try{
			if(!empty($entityType)){
				$this->dataImportType = $entityType;
				$entityName = 'BulkMasterEnrollment';
			}else{
				$entityName = 'MasterEnrollment';
			}
			// Set exact collection name (should match with solr's collection)
			$this->collName = $this->prefix . '_' . SolrClient::EnrollCore;
			
			$options['clean'] = 'false'; // Force clean to false
			$this->createIndex($entityName,$options);
	
		}catch(Exception $e){
			expDebug::dPrint("Error in User Indexing -- ".print_r($e->getMessage(),true),1);
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Initiate the indexing process
	 * @param $core
	 * @param $options
	 * @return unknown_type
	 */
	private function createIndex($entity,$options=array()){
		try{
			$options['core'] = $this->collName;
			$options['entity'] = $entity;
			
			$params = array_merge($this->baseOption,$options);
			expDebug::dPrint("Solr Data Import Param ".print_r($params,true),4);
			$response = $this->reCreateIndex($this->collName,$params);
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	public function updateIndex($entity,$data){
		
	}
}
?>