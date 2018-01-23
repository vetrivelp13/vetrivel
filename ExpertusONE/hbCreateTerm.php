<?php

include_once($_SERVER['DOCUMENT_ROOT']. '/expertusone_auth.php');
$init_time = microtime(true);
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
require_once DRUPAL_ROOT."/sites/all/services/Trace.php";
ini_set('max_execution_time', 0);
expDebug::dPrint("Includes done, time taken -- ".($init_time - microtime(true)),1);
class DrupalActivityService {

  public function __construct(){
    $this->truncateTerms();
  }

  public function process() {
   // $this->createOrgVocabTerms();
    $this->createBaseVocabTerms();
    $this->createDtVocabTerms();
    //$this->createCourseVocabTerms();
    $this->createTPVocabTerms();
  }
   
  public function truncateTerms() {
    db_query("TRUNCATE TABLE taxonomy_index");
    db_query("TRUNCATE TABLE taxonomy_term_data");
    //db_query("TRUNCATE TABLE taxonomy_vocabulary");
    db_query("TRUNCATE TABLE taxonomy_term_hierarchy");
  }

  public function createOrgVocabTerms() {
  	$org_time = microtime(true);
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'organization_nav_vocabulary'")->fetchField();
    if(empty($vid)) {
      // Create vocabulary
      $orgVocabulary = array(
       'name' => t('Organization'),
       'module' => 'taxonomy',
     	 'machine_name' => 'organization_nav_vocabulary',
      );
      $this->createVocabulary($orgVocabulary);
    }
    // Create "All Organization" terms
    $orgTermsAll = array(
     'name' => 'Any',
     'vid' => $vid,
     'description' => '',
     'parent' => '',
     'var_set' => 'organization_all_tid',   	 
    );
    $this->createVocabTerms($orgTermsAll);
    $oraganization_all_tid = db_query("select tid FROM taxonomy_term_data WHERE name ='Any' and vid = ".$vid)->fetchField();    
    // Create list of organization terms
    $organizations_res = db_query("select concat(name,'-',number) name_number, id from slt_organization where status != 'cre_org_sts_del'");        
    foreach($organizations_res as $organization) {
       $orgTerm = array(
         'name' => $organization->name_number,
         'vid' => $vid,
         'description' => $organization->id,
         'parent' => $oraganization_all_tid,
         'var_set' => '',   	 
       );
       
      $this->createVocabTerms($orgTerm);
    }
    expDebug::dPrint("Terms created for organization, time taken -- ".($org_time - microtime(true)),1);
  }
  public function createBaseVocabTerms(){
  	$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'organization_nav_vocabulary'")->fetchField();
    if(empty($vid)) {
      // Create vocabulary
      $orgVocabulary = array(
       'name' => t('Organization'),
       'module' => 'taxonomy',
     	 'machine_name' => 'organization_nav_vocabulary',
      );
      $this->createVocabulary($orgVocabulary);
    }
    // Create "All Organization" terms
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'organization_nav_vocabulary'")->fetchField();
    $orgTermsAll = array(
     'name' => 'Any',
     'vid' => $vid,
     'description' => '',
     'parent' => '',
     'var_set' => 'organization_all_tid',   	 
    );
    $this->createVocabTerms($orgTermsAll);
    $orgTermsAll = array(
     'name' => 'Root Org-XXXXXXXX1',
     'vid' => $vid,
     'description' => '',
     'parent' => '',
     'var_set' => '',   	 
    );
    $this->createVocabTerms($orgTermsAll);
  	$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
  	if(empty($vid)) {
      // Create vocabulary
      $tpVocabulary = array(
       'name' => t('Catalog'),
       'module' => 'taxonomy',
       'machine_name' => 'catalog',
      );
      $this->createVocabulary($tpVocabulary);
    }
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
  	$dtTermsAll = array(
       'name' => '--All Delivery types--',
       'vid' => $vid,
       'description' => '',
       'parent' => '',
       'var_set' => '',   	 
      );
      $this->createVocabTerms($dtTermsAll);
      
  	$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
    if(empty($vid)) {
      // Create vocabulary
      $tpVocabulary = array(
       'name' => t('Tp Catalog'),
       'module' => 'taxonomy',
       'machine_name' => 'tp_catalog_nav_vocabulary',
      );
      $this->createVocabulary($tpVocabulary);
    } 
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
    if($vid) {
       // Create "All Organization" terms
      $tpTermsAll = array(
       'name' => '--All Tp--',
       'vid' => $vid,
       'description' => '',
       'parent' => '',
       'var_set' => '',   	 
      );
      $this->createVocabTerms($tpTermsAll);
   }
 }
  
  public function createDtVocabTerms() {
  	$dt_time = microtime(true);
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'catalog'")->fetchField();
    if($vid) {
       // Create "All Organization" terms
      /*$dtTermsAll = array(
       'name' => '--All Delivery types--',
       'vid' => $vid,
       'description' => '',
       'parent' => '',
       'var_set' => '',   	 
      );
      $this->createVocabTerms($dtTermsAll);*/
      $delivery_type_all_tid = db_query("select tid FROM taxonomy_term_data WHERE name ='--All Delivery types--' and vid = ".$vid)->fetchField();
      
      $select = db_select('slt_course_class', 'scc');
      $select->join('slt_node_learning_activity','snla','scc.id=snla.entity_id');
      $select->addField('snla','node_id');
      $select->addField('scc','id');
      $select->addField('scc','course_id');
      $select->addField('scc','title');
      $select->addField('scc','code');
      $select->addField('scc','price');
      $select->addField('scc','description');
      $select->addField('scc','delivery_type');
      $select->addField('scc','status');
      $select->condition('scc.status', 'lrn_cls_sts_del', '!=');
      $select->condition('snla.entity_type', 'cre_sys_obt_cls');
      expDebug::dPrintDBAPI("Create terms for class ",$select,array(),1);
      $classResult = $select->execute()->fetchAll();
      //print_r($classResult);
      foreach($classResult as $oClassInfo) {
        $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$oClassInfo->delivery_type."'")->fetchField();
        $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$oClassInfo->delivery_type."' and vid = ".$vid)->fetchField();      
        //echo $delivery_type_name.'-Code-'.$oClassInfo->node_id.'<br>';
        $oNode = node_load($oClassInfo->node_id);  	
        //print_r($oNode);
        $oNode->model=$delivery_type_name.'-Code-'.$oClassInfo->code;  		 
        $oNode->status=($oClassInfo->status == 'lrn_cls_sts_atv') ? 1 : 0;
        $oNode->op="Save";
        $dtTerms = array(
           'name' => $delivery_type_name,
           'vid' => $vid,
           'description' => $oClassInfo->delivery_type,
           'parent' => $delivery_type_all_tid   	 
         );
         $oNode->taxonomy_catalog['und'] = array();
         $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_all_tid);       
         if(empty($delivery_type_tid)) {
           $oNode->taxonomy_catalog['und'][] = array('tid' => $this->createVocabTerms($dtTerms));
         }
         else {
           $oNode->taxonomy_catalog['und'][] = array('tid' => $delivery_type_tid);
         }
        $delivery_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$oClassInfo->delivery_type."' and vid = ".$vid)->fetchField();
        $dtTerms1 = array(
           'name' => $oClassInfo->title.' : '.$oClassInfo->code,
           'vid' => $vid,
           'description' => $oClassInfo->id,
           'parent' => $delivery_type_tid,//$parent_tid,
           'var_set' => '',   	 
         );
        $oNode->taxonomy_catalog['und'][] = array('tid' => $this->createVocabTerms($dtTerms1));        
        //print_r($oNode);
        // end to attach terms to node and create terms.
    	node_save($oNode);
        
      }
    }
    expDebug::dPrint("Terms created for delivery types, time taken -- ".($dt_time - microtime(true)),1);
  }
  
public function createTPVocabTerms() {
	$tp_time = microtime(true);
	/*$vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
    if(empty($vid)) {
      // Create vocabulary
      $tpVocabulary = array(
       'name' => t('Tp Catalog'),
       'module' => 'taxonomy',
       'machine_name' => 'tp_catalog_nav_vocabulary',
      );
      $this->createVocabulary($tpVocabulary);
    } */
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'tp_catalog_nav_vocabulary'")->fetchField();
    if($vid) {
     /*  // Create "All Organization" terms
      $tpTermsAll = array(
       'name' => '--All Tp--',
       'vid' => $vid,
       'description' => '',
       'parent' => '',
       'var_set' => '',   	 
      );
      $this->createVocabTerms($tpTermsAll);*/
      $prg_type_all_tid = db_query("select tid FROM taxonomy_term_data WHERE name ='--All Tp--' and vid = ".$vid)->fetchField();
      
      $select = db_select('slt_program', 'prg');
      $select->join('slt_node_learning_activity','snla','prg.id=snla.entity_id');
      $select->addField('snla','node_id');
      $select->addField('prg','id');
      $select->addField('prg','title');
      $select->addField('prg','code');
      $select->addField('prg','price');
      $select->addField('prg','description');
      $select->addField('prg','object_type');
      $select->addField('prg','status');
      $select->condition('prg.status', 'lrn_lpn_sts_del', '!=');
      $select->condition('snla.entity_type', array('cre_sys_obt_cur', 'cre_sys_obt_trn','cre_sys_obt_crt'), 'IN');
      expDebug::dPrintDBAPI("Create terms for TP ",$select,array(),1);
      $classResult = $select->execute()->fetchAll();
      //print_r($classResult);
      foreach($classResult as $oClassInfo) {
        $prg_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$oClassInfo->object_type."'")->fetchField();
        $prg_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$oClassInfo->object_type."' and vid = ".$vid)->fetchField();      
        //echo $prg_type_name.'-Code-'.$oClassInfo->node_id.'<br>';
        $oNode = node_load($oClassInfo->node_id);  	
        //print_r($oNode);
        $oNode->model=$prg_type_name.'-Code-'.$oClassInfo->code;  		 
        $oNode->status=($oClassInfo->status == 'lrn_lpn_sts_atv') ? 1 : 0;
        $oNode->op="Save";
        $dtTerms = array(
           'name' => $prg_type_name,
           'vid' => $vid,
           'description' => $oClassInfo->object_type,
           'parent' => $prg_type_all_tid   	 
         );
         $oNode->taxonomy_catalog['und'] = array();
         $oNode->taxonomy_catalog['und'][] = array('tid' => $prg_type_all_tid);       
         if(empty($prg_type_tid)) {
           $oNode->taxonomy_catalog['und'][] = array('tid' => $this->createVocabTerms($dtTerms));
         }
         else {
           $oNode->taxonomy_catalog['und'][] = array('tid' => $prg_type_tid);
         }
        $prg_type_tid = db_query("select tid FROM taxonomy_term_data WHERE description ='".$oClassInfo->object_type."' and vid = ".$vid)->fetchField();
        $dtTerms1 = array(
           'name' => $oClassInfo->title.' : '.$oClassInfo->code,
           'vid' => $vid,
           'description' => $oClassInfo->id,
           'parent' => $prg_type_tid,//$parent_tid,
           'var_set' => '',   	 
         );
        $oNode->taxonomy_catalog['und'][] = array('tid' => $this->createVocabTerms($dtTerms1));        
        //print_r($oNode);
        // end to attach terms to node and create terms.
    	node_save($oNode);
        
      }
    }
    expDebug::dPrint("Terms created for delivery types, time taken -- ".($tp_time - microtime(true)),1);
  }
  
  public function createCourseVocabTerms() {
    $vid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'course_catalog_nav_vocabulary'")->fetchField();
    if(empty($vid)) {
      // Create vocabulary
      $dtVocabulary = array(
       'name' => t('Course Catalog'),
       'module' => 'Taxonomy',
     	 'machine_name' => 'course_catalog_nav_vocabulary',
      );
      $this->createVocabulary($dtVocabulary);
    }    
    // Create list of organization terms
    $course_res = db_query("select title, id from slt_course_template where status != 'lrn_crs_sts_del'");    
    foreach($course_res as $course) {
       $orgTermsAll = array(
         'name' => $course->title,
         'vid' => $vid,
         'description' => $course->id,
         //'parent' => variable_get('organization_all_tid', 0),
         'var_set' => '',   	 
       );
       
      $this->createVocabTerms($orgTermsAll);
    }
    
  }  

  // Vocabulary
  private function createVocabulary($vocabTxt) {
    $vocabulary               = new stdClass;
    $vocabulary->name         = $vocabTxt["name"];
    $vocabulary->module       = $vocabTxt["module"];
    $vocabulary->machine_name = $vocabTxt["machine_name"];
    $vocabulary->nodes        = array();

    taxonomy_vocabulary_save($vocabulary);
     
    $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = :machine_name",array(':machine_name' => $vocabTxt["machine_name"]))->fetchField();
    variable_set($vocabTxt["machine_name"], $vid);
  }

  // Terms
  private function createVocabTerms($termsTxt) {
    $terms               = new stdClass;
    $terms->name         = $termsTxt["name"];
    $terms->vid          = $termsTxt["vid"];
    if(!empty($termsTxt["description"])) {
      $terms->description  = $termsTxt["description"];
    }
    if(!empty($termsTxt["parent"])) {
      $terms->parent       = $termsTxt["parent"];
    }    
    taxonomy_term_save($terms);
    if(!empty($termsTxt['var_set'])) {
      variable_set($termsTxt['var_set'], $terms->tid);
    } 
    return $terms->tid;
  }
}
$process_time = microtime(true);
if($user->uid == 1) {
  $drupalServ  = new DrupalActivityService();
  $rtn         =  $drupalServ->process();
  print "Terms are created successfully for Oragnization, Delivery Type and Classes.";
  expDebug::dPrint("Process completed , time taken to complete all process -- ".($process_time - microtime(true)),1);
  expDebug::dPrint("Process completed , overall time taken -- ".($init_time - microtime(true)),1);
}
else
{
  print "You are not Authorized to access this Service.";
}
?>