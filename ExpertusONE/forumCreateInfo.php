<?php //Trunk
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class DrupalActivityService {

  public function process(){    
    $this->createNodeForCourseForum();
    $this->createNodeForTPForum();    
  }
 
  function createNodeForCourseForum()  {
    global $user;
    
    $txn = db_transaction();
    
    try {
        $vSql = "SELECT id,title, description, short_description, status FROM slt_course_template where status ='lrn_crs_sts_atv'";
        $queryResult =  db_query($vSql);
        
        foreach ($queryResult as $courseInfo) {
          
            $selectNodeForum = db_select("slt_node_learning_activity","entity");
            $selectNodeForum->innerJoin('node','n','entity.node_id=n.nid');
            $selectNodeForum->addField('n','nid','nid');
            $selectNodeForum->condition('entity.entity_id',$courseInfo->id,'=');
            $selectNodeForum->condition('entity.entity_type','Course');
            $selectNodeForum->condition('entity.custom0', 'forum');
            $crsList = $selectNodeForum->execute()->fetchAll();
            
            if($crsList[0]->nid == '') {
                $forumNode 	= new StdClass(); 
                $forumNode->uid       =  $user->uid;
                $forumNode->language  = 'en';
                $forumNode->title     = str_replace("''","'",$courseInfo->title);
                $forumNode->body      = $courseInfo->description;
                $forumNode->type      = 'forum';
                $forumNode->created   = time();
                $forumNode->changed   = time();
                $forumNode->teaser_include = 1;
                $forumNode->revision  = 1;
                $forumNode->teaser     = $courseInfo->description;
                $forumNode->format    = 1;
                $forumNode->log       = '';
                $forumNode->name      = $user->uid;
                $forumNode->date      = '';
                $forumNode->status    = ($courseInfo->Status == 'lrn_crs_sts_atv') ? 1 : 0;
                $forumNode->promote   = 0;
                $forumNode->sticky    = 0;
                $forumNode->op        = "Save";
            
                node_save($forumNode);    
                
                $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
                $forumTerms               = new stdClass;
                $forumTerms->name         = $courseInfo->title;
                $forumTerms->vid          = $forumvid;
                $forumTerms->description  = "Course";//$description;
                $forumTerms->format       = $courseInfo->id; //courseid          
            
                taxonomy_term_save($forumTerms);
                $forumNodeId = $forumNode->nid;
                  
                //to insert record into the slt_node_learning_activity
                $nodeInsertForum = db_insert('slt_node_learning_activity');
                $nodeFieldsForum = array(
                                 'node_id' => $forumNodeId,
                                 'entity_id' => $courseInfo->id,  // courseid
                                 'entity_type' => "Course",
                                 'createdby' => $user->uid,
                                 'createdon' => now(),
                                 'custom0' => 'forum'
                       );
                $nodeInsertForum->fields($nodeFieldsForum);
                $nodeInsertForum->execute();
            }
        
        } //for Loop END
    
    } catch(Exception $e) {
	   $txn->rollback();
       unset($txn);
       watchdog_exception('createNodeForCourseForum', $ex);
       expertusErrorThrow($ex);
	}
	unset($txn);
	
  } //createNodeForCourseForum() END
  
  function createNodeForTPForum()  {
    global $user;

    $txn = db_transaction();
    
    try {
        $vSql = "SELECT id,title, description, short_desc, status FROM slt_program where status ='lrn_lpn_sts_atv'";
        $queryResult =  db_query($vSql);
        foreach ($queryResult as $tpInfo) {
    
          $selectNodeForum = db_select("slt_node_learning_activity","entity");
          $selectNodeForum->innerJoin('node','n','entity.node_id=n.nid');
          $selectNodeForum->addField('n','nid','nid');
          $selectNodeForum->condition('entity.entity_id',$tpInfo->id,'=');
          $selectNodeForum->condition('entity.entity_type','TP');
          $selectNodeForum->condition('entity.custom0', 'forum');
          $tpList = $selectNodeForum->execute()->fetchAll();
            
            if($tpList[0]->nid == '') {
                $forumNode = new StdClass();  
                $forumNode->uid=$user->uid;
                $forumNode->language='en';
                $forumNode->title= str_replace("''","'",$tpInfo->title);
                $forumNode->body= $tpInfo->short_desc;
                $forumNode->type='forum';
                $forumNode->created=time();
                $forumNode->changed=time();
                $forumNode->teaser_include=1;
                $forumNode->revision=1;  
                $forumNode->format=1;
                $forumNode->log='';
                $forumNode->name=$user->uid;
                $forumNode->date='';
                $forumNode->status= ($tpInfo->status == 'lrn_lpn_sts_atv') ? 1 : 0;
                $forumNode->promote=0;
                $forumNode->sticky=0;
                $forumNode->op="Save";
                  
                // Getting forum  vocabulary id
                $forumvid = db_query("select vid FROM taxonomy_vocabulary WHERE machine_name = 'forums'")->fetchField();
               
                $forumTerms               = new stdClass;
                $forumTerms->name         = str_replace("''","'",$tpInfo->title);
                $forumTerms->vid          = $forumvid;
                $forumTerms->description  = "TP";//$form_state['values']['short_desc'];
                $forumTerms->format       = $tpInfo->id;
                
                taxonomy_term_save($forumTerms);
                
                node_save($forumNode);
                
                $forumNodeId = $forumNode->nid;
                
                $nodeInsertForum = db_insert('slt_node_learning_activity');
                $nodeFieldsForum = array(
                            'node_id' => $forumNodeId,
                            'entity_id' => $tpInfo->id,  // class id
            	            'entity_type' => 'TP',//$type,
            		        'createdby' => $user->uid,
                            'createdon' => now(),
              				'custom0' => 'forum'
                      );
                $nodeInsertForum->fields($nodeFieldsForum);
                $nodeInsertForum->execute();
            }
            
        } //for Loop END
    
    } catch(Exception $e) {
	   $txn->rollback();
       unset($txn);
       watchdog_exception('createNodeForTPForum', $ex);
       expertusErrorThrow($ex);
	}
	unset($txn);
  } //createNodeForTPForum() END
  
  
} //DrupalActivityService END


if($user->uid==1) {
  
  $drupalServ=new DrupalActivityService();
  $rtn =  $drupalServ->process();
  print "Completed Successfully<br>";
  
} else {
  
  print "You are not Authorized to access this Service.";
           
}

?>