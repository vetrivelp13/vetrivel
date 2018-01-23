<?php
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
require_once DRUPAL_ROOT."/sites/all/services/Trace.php";
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class DrupalActivityService {
  public function __construct(){
  	try{
    $this->truncateTerms();
    //$this->truncateOG();
    $this->truncateProduct();
    $this->truncateOrders();
    $this->truncate_slt_node_learning_activity();
  	}catch(Exception $e){
  		expDebug::dPrint($e);
  	}
  }
  public function process(){
  	try {
  		$this->createTaxonomyForUserGroup();
  		$this->assignTaxonomyGroupForumContainer();
  		//$this->createNodeForCourse();
  		$this->createNodeForClass();
  		$this->createNodeForTrainingProgram();
  	}
  	catch(Exception $e){
  		expDebug::dPrint($e);
  	}
       
  }
  private function createNodeForClass()
  {
  	try {
  		global $user;
  		$vSql = "SELECT id,course_id, title,code,price,description,delivery_type,status FROM slt_course_class";
  		$queryResult =  db_query($vSql);
  		expDebug::dPrint('count of class = '.print_r(count($queryResult)));
  		foreach  ($queryResult as $oClassInfo) {
  			/*if ($oClassInfo->price == NULL ){
  			 $vSql1 = "SELECT price FROM slt_course_template where id=".$oClassInfo->course_template_id;
  			 $queryResult1 =  db_query($vSql1);
  			 $oClassInfo1 = db_fetch_object($queryResult1);
  			 $oClassInfo->price=$oClassInfo1->price;
  			 }*/
  				
  			// echo "<br>--- ".$vClassId." - ".$vTitle."----<br>";
  			$oNode = new StdClass();
  			$oNode->uid=$user->uid;
  			$oNode->language='en';
  			$oNode->title= $oClassInfo->title;
  			$oNode->body= $oClassInfo->description;
  			$delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$oClassInfo->delivery_type."'")->fetchField();
  			$oNode->model=$delivery_type_name.'-Code-'.$oClassInfo->code;
  			$classPrice = $oClassInfo->price.'000';
  			$oNode->sell_price= $classPrice;
  			$oNode->cost=$classPrice;
  			$oNode->list_price=$classPrice;
  			$oNode->type='product';
  			$oNode->created=time();
  			$oNode->changed=time();
  			$forum_new->teaser_include=1;
  			$oNode->revision=1;
  			$node->teaser=$oClassInfo->description;
  			$oNode->format=1;
  			$oNode->log='';
  			$oNode->name=$user->uid;
  			$oNode->date='';
  			$oNode->status=($oClassInfo->status == 'lrn_cls_sts_atv') ? 1 : 0;
  			$oNode->promote=0;
  			$oNode->sticky=0;
  			$oNode->op="Save";
  			$entity_type= 'cre_sys_obt_cls';
  			// start to attach terms to node and create terms.
  			// echo "<pre>";print_r($oClassInfo); echo "</pre>";
  			// print "Taxonomy created for discount purpose<br>";
  			/*$organizations_res=db_query("select concat(name,'-',number) number_name, add1 from slt_organization where status = 'cre_org_sts_act'");
  			 foreach ($organizations_res as $organization) {
  			 $org_tid=db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent=0 and v.vid=td.vid and v.name='Catalog' and td.name = '".$organization->number_name."'")->fetchField();
  			 unset($org_taxonomy);
  			 $org_taxonomy           = array();
  			 $org_taxonomy['name']       = $organization->number_name;
  			 $org_taxonomy['description']  = $organization->add1;
  			 $org_taxonomy['vid']      = 2;
  			 $org_taxonomy['weight']     = 0;
  			 if(!empty($org_tid)) {
  			 $org_taxonomy['tid']    = $org_tid;
  			 }
  			 else
  			 {
  			 // print $organization->number_name." - Organization Taxonomy created for discount purpose<br>";
  			 taxonomy_term_save($org_taxonomy);
  			 }
  			 $org_tid  = db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent=0 and v.vid=td.vid and v.name='Catalog' and td.name = '".$organization->number_name."'")->fetchField();
  			 $oNode->taxonomy[]        = $org_tid;
  		
  			 $delivery_type_tid  = db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent='".$org_tid."' and v.vid=td.vid and v.name='Catalog' and td.description = '".$oClassInfo->delivery_type."'")->fetchField();
  			 $delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$oClassInfo->delivery_type."'")->fetchField();
  		
  			 unset($dt_taxonomy);
  			 $dt_taxonomy          = array();
  			 $dt_taxonomy['name']      = $delivery_type_name;
  			 $dt_taxonomy['description']   = $oClassInfo->delivery_type;
  			 $dt_taxonomy['vid']       = 2;
  			 $dt_taxonomy['weight']      = 0;
  			 $dt_taxonomy['parent'][$org_tid] = $org_tid;
  			 if(!empty($delivery_type_tid))  {
  			 $dt_taxonomy['tid']    = $delivery_type_tid;
  			 }
  			 else
  			 {
  			 // print $delivery_type_name." - Delivery Type Taxonomy created for discount purpose<br>";
  			 taxonomy_term_save($dt_taxonomy);
  			 }
  			 $delivery_type_tid  = db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent='".$org_tid."' and v.vid=td.vid and v.name='Catalog' and td.description = '".$oClassInfo->delivery_type."'")->fetchField();
  		
  			 $oNode->taxonomy[]  = $delivery_type_tid;
  		
  			 $class_tid=db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent='".$delivery_type_tid."' and v.vid=td.vid and v.name='Catalog' and td.description = '".$oClassInfo->id."'")->fetchField();
  		
  			 unset($class_taxonomy);
  			 $class_taxonomy         = array();
  			 $class_taxonomy['name']     = $oClassInfo->title.','.$oClassInfo->code;
  			 $class_taxonomy['description']  = $oClassInfo->id;
  			 $class_taxonomy['vid']      = 2;
  			 $class_taxonomy['weight']     = 0;
  			 $class_taxonomy['parent'][$delivery_type_tid] = $delivery_type_tid;
  			 if(!empty($class_tid))  {
  			 $class_taxonomy['tid']    = $class_tid;
  			 }
  			 else
  			 {
  			 taxonomy_term_save($class_taxonomy);
  			 }
  			 // print $oClassInfo->title." - Class - Taxonomy created for discount purpose<br>";
  			 $class_tid      = db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent='".$delivery_type_tid."' and v.vid=td.vid and v.name='Catalog' and td.description = '".$oClassInfo->id."'")->fetchField();
  			 $oNode->taxonomy[]  = $class_tid;
  			 }*/
  		
  			// end to attach terms to node and create terms.
  			node_save($oNode);
  			$vNodeId= $oNode->nid;
  			db_query("INSERT INTO slt_node_learning_activity (node_id, entity_id, entity_type, createdby, createdon, updatedby, updatedon) VALUES
  					($vNodeId, $oClassInfo->id, '$entity_type', $user->uid, now(), '', now())");
  		}
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  
  
  }
  private function truncateTerms()
  {  
  	try {
  		db_query("TRUNCATE TABLE term_node");
  		db_query("TRUNCATE TABLE term_data");
  		// db_query("TRUNCATE TABLE term_hierarchy");
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  }
  private function truncateOG()
  {
    db_query("TRUNCATE TABLE og");
    db_query("TRUNCATE TABLE og_access_post");
    db_query("TRUNCATE TABLE og_ancestry");
    db_query("TRUNCATE TABLE og_term");  
    db_query("TRUNCATE TABLE og_uid");
  }
  private function truncateProduct()
  {
  	try {
  		db_query("TRUNCATE TABLE uc_products");
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  	
  }
  private function truncateOrders()
  {
  	try {
  		db_query("TRUNCATE TABLE uc_orders");
  		db_query("TRUNCATE TABLE uc_order_products");
  		db_query("TRUNCATE TABLE uc_order_log");
  		db_query("TRUNCATE TABLE uc_order_line_items");
  		db_query("TRUNCATE TABLE uc_order_comments");
  		db_query("TRUNCATE TABLE uc_order_admin_comments");
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  	
  }
  private function truncate_slt_node_learning_activity()
  {
  	try {
  		//db_query("TRUNCATE TABLE slt_node_learning_activity");
		db_query("DELETE FROM slt_node_learning_activity WHERE entity_type IN ('cre_sys_obt_cls','cre_sys_obt_crt','cre_sys_obt_trn','cre_sys_obt_cur') ");
		db_query("DELETE FROM node WHERE type = 'product' ");
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  	
  }
  function createTaxonomyForUserGroup()
  {
  	try {
  		$og_taxonomy           = array();
  		$og_taxonomy['name']       = 'Learning Forums';
  		$og_taxonomy['description']  = 'Learning Forums';
  		$og_taxonomy['vid']      = 1;
  		$og_taxonomy['weight']     = 0;
  		taxonomy_term_save($og_taxonomy);
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
    
  }
  function assignTaxonomyGroupForumContainer()
  {
  	try {
  		variable_set('forum_default_name', 'General discussion');
  		variable_set('forum_default_container_yn', 1);
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
  // $lf_tid=db_query("select td.tid from vocabulary v, term_data td, term_hierarchy th where th.tid=td.tid and th.parent=0 and v.vid=td.vid and v.name='Forums' and td.name = 'Learning Forums'")->fetchField();
    // set Learning forum taxonomy for forum container
    
   // variable_set('forum_default_container', $lf_tid);
  }
  function createNodeForCourse()
  {
  	try {
  		global $user;
  		$entity_type = 'cre_sys_obt_crs';
  		$vSql = "SELECT id,title, description, short_description, status FROM slt_course_template";
  		$queryResult =  db_query($vSql);
  		foreach ($queryResult as $courseInfo) {
  			$oNode = new StdClass();
  			$oNode->uid=$user->uid;
  			$oNode->language='en';
  			$oNode->title= $courseInfo->title;
  			$oNode->body= $courseInfo->description;
  			$oNode->type='course';
  			$oNode->created=time();
  			$oNode->changed=time();
  			$forum_new->teaser_include=1;
  			$oNode->revision=1;
  			$node->teaser=$courseInfo->short_description;
  			$oNode->format=1;
  			$oNode->log='';
  			$oNode->name=$user->uid;
  			$oNode->date='';
  			$oNode->status = ($courseInfo->Status == 'lrn_crs_sts_atv') ? 1 : 0;
  			$oNode->promote=0;
  			$oNode->sticky=0;
  			$oNode->op="Save";
  			node_save($oNode);
  			$vNodeId=  $oNode->nid;
  			db_query("INSERT INTO slt_node_learning_activity (node_id, entity_id, entity_type, createdby, createdon, updatedby, updatedon) VALUES
  					($vNodeId, $courseInfo->id, '$entity_type', $user->uid, now(), '', now())");
  			//og_save_subscription($vNodeId, $user->uid, array('is_active' => $oNode->status));
  		}
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
    
  }
  function createNodeForTrainingProgram()
  {
  	try {
  		global $user;
//   		$entity_type = 'cre_pgn_frm_trp';
  		$vSql = "SELECT id,title, description, short_desc, status, object_type, price, code FROM slt_program";
  		$queryResult =  db_query($vSql);
  		foreach ($queryResult as $tpInfo) {
  			$oNode = new StdClass();
  			$oNode->uid=$user->uid;
  			$oNode->language='en';
  			$oNode->title= $tpInfo->title;
  			$oNode->body= $tpInfo->description;
  			$oNode->type='product';
  			$oNode->created=time();
  			$oNode->changed=time();
  			$forum_new->teaser_include=1;
  			$oNode->revision=1;
  			$node->teaser=$tpInfo->short_desc;
  			$oNode->format=1;
  			$oNode->log='';
  			$oNode->name=$user->uid;
  			$oNode->date='';
  			$oNode->status = ($inObj->Status == 'lrn_lpn_sts_atv') ? 1 : 0;
  			$oNode->promote=0;
  			$oNode->sticky=0;
  			$oNode->op="Save";
  			$entity_type = $tpInfo->object_type;
  			$delivery_type_name = db_query("select name FROM slt_profile_list_items WHERE code ='".$tpInfo->object_type."'")->fetchField();
  			$oNode->model=$delivery_type_name.'-Code-'.$tpInfo->code;
  			$tpPrice = $tpInfo->price.'000';
  			$oNode->sell_price= $tpPrice;
  			$oNode->cost=$tpPrice;
  			$oNode->list_price=$tpPrice;
  			node_save($oNode);
  			$vNodeId=  $oNode->nid;
  			db_query("INSERT INTO slt_node_learning_activity (node_id, entity_id, entity_type, createdby, createdon, updatedby, updatedon) VALUES
  					($vNodeId, $tpInfo->id, '$entity_type', $user->uid, now(), '', now())");
  			//og_save_subscription($vNodeId, $user->uid, array('is_active' => $oNode->status));
  		}
  	} catch (Exception $e) {
  		expDebug::dPrint($e);
  	}
    
  }
}
if($user->uid==1)
{
  $drupalServ=new DrupalActivityService();
  $rtn =  $drupalServ->process();
  print "Completed Successfully<br>";
}
else
{
  print "You are not Authorized to access this Service.";         
}
?>