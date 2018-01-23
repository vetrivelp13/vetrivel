<?php
/**
* DrupalService
* 
* Page-level DocBlock is here because it is the first DocBlock
* in the file, and is immediately followed by the second
* DocBlock before any documentable element is declared
* (function, define, class, global variable, include)
* @package pagepackage
* @author Ilayaraja <ilayaraja.e@expertus.com>
* @version 1.0
* @copyright Copyright (c) 2011, ExpertUs
* @since 18-Feb-2011
*/


define('DRUPAL_ROOT', getcwd());
/**
* Included the common.inc file to get the common settings
*/
include_once './includes/common.inc';
/**
* Included the database.inc file to get the database settings
*/
include_once "./includes/database/database.inc";
/**
* Included the bootstrap.inc file to get the bootstrap settings
*/
include_once './includes/bootstrap.inc';

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
/**
* DrupalServiceHandler, It contain servies which serve all the drupal related functionalities.
*
* <p>Input parameters validation, header settings and action items validation done here.</p>
* @package DrupalServiceHandler
* @author Ilayaraja <ilayaraja.e@expertus.com>
*/
class DrupalServiceHandler {
	/**
    * @access private
    * @var string
    */
	private $actionname="Please pass a action name";
	/**
    * @access private
    * @var array
    */	
	private $paramList=array();
	/**
    * @access private
    * @var string
    */
	private $returntype="";
	/**
    * @access private
    * @var boolean
    */		
	private $enableAccess=false;
	
	/**
	* Constructor sets up {$actionname, $returntype}
    */	
	function __construct() {
		if(strlen($_GET["actionname"])>0) {
			$this->actionname=$_GET["actionname"];
			$this->parseParams($_GET);
		}
		else if(strlen($_POST["actionname"])>0) {
			$this->actionname=$_POST["actionname"];
			$this->parseParams($_POST);
		}
		else {
			$this->actionname="Please pass a action name (actionname)";
			echo "<h1>Please pass a action name</h1>";
		}

		if(strlen($_GET["returntype"])>0) {
			$this->returntype=$_GET["returntype"];
			$this->parseParams($_GET);
			$this->setReturnType($this->returntype);
		} else if(strlen($_POST["returntype"])>0) {
			$this->returntype=$_POST["returntype"];
			$this->parseParams($_POST);
			$this->setReturnType($this->returntype);
		} else {
			$this->returntype="xml";
			$this->setReturnType($this->returntype);
		}
	}

    /**
    * Validate the user and parse all input parameters which is having get type.
    * @param mixed $paramname 
    * @return mixed
    */
	private function doGet($paramname) {
		if($this->checkUser()) {
			$this->parseParams($_GET);
			return $_GET[$paramname];
		}
	}

	/**
    * Validate the user and parse all input parameters which is having get type.
    * @param mixed $paramname 
    * @return mixed
    */
	private function doPost($paramname) {
		if($this->checkUser()) {
			$this->parseParams($_POST);
			return $_POST[$paramname];
		}
	}
	
	/**
    * Get action name.
    * @param boolean $paramname 
    * @return string
    */
	function getActionname() {
		return $this->actionname;
	}
	
	/**
    * Assign the values to $paramList from $_GET/$_POST variables. 
    * @param boolean $paramname 
    * @return array
    */
	function parseParams($array) {
		foreach($array as $key => $value) {
			$this->paramList[$key]=$value;
		}
	}

	/**
    * Get a value of parameter.
    * @param boolean $paramname 
    * @return mixed
    */	
	function getParam($paramname) {
		if($this->checkUser()) {
			return $this->paramList[$paramname];
		}
	}
	
	/**
    * Get all the values which is assigned from $paramList.     
    * @return array
    */	
	function getParamList(){
		if($this->checkUser()) {
			return $this->paramList;
		}
	}
	
	/**
    * Validate authenticated user.    
    * @return mixed
    */	
	function checkUser() {
		global $user;
		if(((in_array("authenticated user",$user->roles))) || $this->enableAccess==true) {
			return true;
		} else {
			header('Content-Type: text/html');
			echo "<h1>You are not allowed to access this service</h1>";
			exit();
		}
	}
	
	/**
    * Enable service access.    
    */	
	function enableServiceAccess() {
		$this->enableAccess=true;
	}

	/**
    * Disable service access 
    */
	function disableServiceAccess() {
		$this->enableAccess=false;
	}
	
	/**
    * Setting headers based on content type specified.
    * @param boolean $type
    */
	function setReturnType($type) {
		switch ($type){
			case "js": header('Content-Type: text/javascript');
			break;
			case "xml": header('Content-Type: text/xml');
			break;
			case "html": header('Content-Type: text/html');
			break;
			case "text": header('Content-Type: text/text');
			break;
			default:header('Content-Type: text/xml');
		}
	}
	
	/**
    * Customized headers settings.
    * @param boolean $paramname 
    * @return integer
    */
	function setCustomHeader($header) {
		header($header);
	}
}
/**
* DrupalService, It contains list of action items to be processed.
*
* <p>1) It Performs file and avatar upload for users in Admin. 
*    2) In Learner, Training program can be registered/Cancelled to User group.
*    3) Drupal node can be added,updated and deleted here.
*     </p>
* @package DrupalService
* @author Ilayaraja <ilayaraja.e@expertus.com>
*/
class DrupalService {
	public function __construct(){
		
	}
	
	/**
    * Process the actions which mentioned in DrupalService package.
    * @param boolean $paramname 
    * @return integer
    */
	public function process(){
		$drupalServHdlr=new DrupalServiceHandler();
		$actionNm=$drupalServHdlr->getActionName();
		$params=$drupalServHdlr->getParamList();
		
		switch($actionNm) {
			case "isForumEnabled":
				$this->isForumEnabled($params);		
			break;
			case "addUpdateGroup":
				 $this->addUpdateGroup($params);		
			break;
			case "addUserToGroup":
				 $this->addUserToGroup($params);		
			break;
			case "removeUserFromGroup":
				 $this->removeUserFromGroup($params);		
			break;
			case  "addForumTopic":
				$this->addForumTopic($params);		
			break;
			case  "updateForumTopic":
				$this->updateForumTopic($params);		
			break;
			case "addForumComment":
				$this->addForumComment($params);
			break;							
			case "addDrupalNode":
				$this->addDrupalNode($params);	
			break;
			case "updateDrupalNode":
				$this->updateDrupalNode($params);	
			break;	 
			case "deleteDrupalNode":
				$this->deleteDrupalNode($params);	
			break;
			case "getDrupalNodeInfo":
				$this->getDrupalNode($params);
			break;
			case "subscriptionTrainingProgramToOG":
				$this->subscriptionTrainingProgramToOG($params);		
			break;
			case "unsubscriptionTrainingProgramToOG":
				$this->unsubscriptionTrainingProgramToOG($params);		
				break;			 
			case "AnnouncementFileUpload":		
				
					$source = $_GET['source'];
					$uid = $_GET['uid']; 
					$nname = $_GET['newname'];
					$vDestPath='';
					$vOldFilePath='';
					$aFileName=null;
					$vNewFileName=null;
					$vNewPath=null;
					$limits = array ('file_size'=>20*1024,'user_size'=>0) ;
					$validators = array(
					   // 'file_validate_extensions' => array($limits['extensions']),
					   // 'file_validate_image_resolution' => array($limits['resolution']),
					    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
						'file_validate_is_image'=>array()
					  );
					  $validators = array();
					$vDestPath = "sites/default/files/images/";
					//$vDestPathUp = "public://sites/default/files/images/";
					$vDestPathUp = "public://images/";
					$fup = file_save_upload($source,  $validators, $vDestPathUp, FILE_EXISTS_REPLACE);
					$vOldFileName = $_FILES['files']['name'][$source];
					$vOldFilePath = $vDestPath.$vOldFileName;
					$aFileName = explode(".",$vOldFileName);
					$vNewFileName = 'an_'.rand().'.'.$aFileName[1]; 
					$vNewPath = $vDestPath.$vNewFileName;   
									
					
					$vRenameFlag =rename($vOldFilePath,$vNewPath);
					unlink($vOldFilePath);
					$er=  drupal_get_messages('error');  

					$aImageInfo = getimagesize($vNewPath);
					
					header("Content-type: text/html; charset=UTF-8");
					ob_end_clean();
					ob_start();
					$rtn =  "{";
					$rtn .=	"error: '" . $vRenameFlag . "',\n";
					$rtn .=	"filename : '" . $vNewFileName . "',\n";
					$rtn .=	"imgwidth : '" . $aImageInfo[0] . "',\n";
					$rtn .=	"imgheight : '" . $aImageInfo[1] . "'\n";
					$rtn .= "}";
					echo $rtn;
					return $rtn;
					break;
				
			case "SurveyFileUpload":		
				
					$source = $_GET['source'];
					$uid = $_GET['uid']; 
					$nname = $_GET['newname'];

					$limits = array ('file_size'=>20*1024,'user_size'=>0) ;
					$validators = array(
					    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
						'file_validate_is_image'=>array()
					  );
					  
					$vDestPathSurvey = "sites/default/files/survey/";
					$fup = file_save_upload($source,  $validators, $vDestPathSurvey, "FILE_EXISTS_REPLACE");
					$vOldFileName = $_FILES['files']['name'][$source];
					$vOldFilePath = $vDestPathSurvey.$vOldFileName;
					
					$aFileName = explode(".",$vOldFileName);
					$vNewFileName = 'an_'.rand().'.'.$aFileName[1]; 
					$vNewPath = $vDestPathSurvey.$vNewFileName;				
					
					$vRenameFlag =rename($vOldFilePath,$vNewPath);
					unlink($vOldFilePath);
					$er=  drupal_get_messages('error');  
					
					$aImageInfo = getimagesize($vNewPath);
					
					header("Content-type: text/html; charset=UTF-8");
					ob_end_clean();
					ob_start();
					$rtn =  "{";
					$rtn .=	"error: '" . $vRenameFlag . "',\n";
					$rtn .=	"filename : '" . $vNewFileName . "',\n";
					$rtn .=	"imgwidth : '" . $aImageInfo[0] . "',\n";
					$rtn .=	"imgheight : '" . $aImageInfo[1] . "'\n";  					
					$rtn .= "}";
					echo $rtn;
					return $rtn;
					break;					
						
			case "fileUpload":			  
				global $user;
				//print_r($validators);
				//return $source;
				$source = $_GET['source'];
				$uid = $_GET['uid'];
				$nname = $_GET['newname'];
					$limits = array ('file_size'=>20*1024,'user_size'=>0) ;
					$validators = array(
					   // 'file_validate_extensions' => array($limits['extensions']),
					   // 'file_validate_image_resolution' => array($limits['resolution']),
					    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
						'file_validate_is_image'=>array()
					  );					  
					  
					$vDestPath = "sites/default/files/";
					$fup = file_save_upload($source,  $validators, $vDestPath, FILE_EXISTS_REPLACE) or die("error in file upload");	
					$user->picture = $_FILES['files']['name'][$source];
					user_save($user, array("picture"=>$user->picture ));
					$oldfile = $vDestPath.$_FILES['files']['name'][$source];
					$newfile = $oldfile;
					/*
					$oldImageInfo = getimagesize($oldfile);
					
					if($oldImageInfo[0]== 110 AND $oldImageInfo[1]== 124){
						$newfile = $vDestPath.$nname.$uid.substr($oldfile,stripos($oldfile,'.'));	
					}								
								
					if(file_exists($oldfile)){						
						if(file_exists($newfile)){
									unlink($newfile);												
							}
							$vRenameFlag=rename($oldfile,$newfile);
							unlink($oldfile);	
						}						
											*/
					$er=  drupal_get_messages('error');
					expDebug::dPrint("Error File save Upload:", 4);
					expDebug::dPrint($er, 4);
							
					$aImageInfo = getimagesize($newfile);	
					header("Content-type: text/html; charset=UTF-8");
					ob_end_clean();
					ob_start();
					$rtn =  "{";
					$rtn .=	"error: '" . $aImageInfo[0] . "',\n";
					$rtn .=	"filename : '" . $newfile . "',\n";
					$rtn .=	"imgwidth : '" . $aImageInfo[0] . "',\n";
					$rtn .=	"imgheight : '" . $aImageInfo[1] . "'\n";  					
					$rtn .= "}";
					echo $rtn;
					return $rtn;
					break;

			case "userPictureUpload":	
			  /**
        * File upload for user's picture and avatar.
        * @param $data
        *   - An associative array which contains user id, type and $_FILES value for the file upload.
        *   - User ID of the person who cancel the registration of the Training program/Course. 
        * @return
        *   - Destination file path.
        */		
							
				//print_r($validators);
				//return $source;
				$source = $_GET['source'];  
				$uid = $_GET['uid'];
				$nname = $_GET['newname'];
				//$limits = array ('file_size'=>20*1024,'user_size'=>0) ;
				$validators = array(
				  //  'file_validate_size' => array($limits['file_size'], $limits['user_size']),
					'file_validate_is_image'=>array(),
					'file_validate_image_resolution' => array('85x85'),
				  );					  
				 // to get drupal user id
				$drupal_uid=db_query("select b.uid from slt_person a,users b where a.user_name=b.name and a.id=$uid")->fetchField();

				$info = image_get_info($_FILES['files']['tmp_name'][$source]);

				//if(($info===FALSE) ||((($info['width'] > 85) ||($info['height'] > 85)) && ($source=='avatharfile')))
				if($info===FALSE)
				{
					header("Content-type: text/html; charset=UTF-8");
					ob_end_clean();
					ob_start();
					$rtn =  "{";
					$rtn .=	"error: 'true'\n";					  					
					$rtn .= "}";
					echo $rtn;
					return $rtn;
					break;
				}
				$destination = "sites/default/files/";
				$destinationUp = "public://sites/default/files/";
				/*$file = file_save_upload($source,  $validators, $destinationUp,FILE_EXISTS_REPLACE) ;//or die("error in file upload");
//			    if (file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
//			      // $form_state['values']['picture'] = $file->filepath;
//			    }
//			    
				$vOldFileName = $_FILES['files']['name'][$source];
				$vOldFilePath = $destination.$vOldFileName;
				
				$aFileName = explode(".",$vOldFileName);
				$vNewFileName = $destination.$nname. $drupal_uid .'.'.$aFileName[1];
				$vRenameFlag =rename($vOldFilePath,$vNewFileName);
				
				
				
				$selected_user = new StdClass();
				$selected_user->uid = $drupal_uid;
				$selected_user->picture = $vNewFileName;				
				
//				if($nname == 'profile_')	
//				{	
//					$slt_qry = "update slt_person set photo='%s' where id='%s'";
//				}
				if($nname == 'avatar_')
				{
					user_save($selected_user, array("picture"=>$selected_user->picture ));
					//$slt_qry = "update slt_person set avatar='%s' where id='%s'";
				}
					
				//db_result(db_query($slt_qry,$selected_user->picture, $uid));
					
				$oldfile = $vDestPath.$_FILES['files']['name'][$source];
				$newfile = $oldfile;
*/
						
				$picture    = file_save_upload($source, $validators);
        $pictureFid = '';
			$account;
	    if(!empty($picture)) {
      	  if (!$picture->status) {
      	      $account = user_load($drupal_uid);
                $info = image_get_info($picture->uri);
                $picture_directory =  file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
                // Prepare the pictures directory.
                file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY);
                $destination = file_stream_wrapper_uri_normalize($picture_directory . '/picture-' . $account->uid . '-' . REQUEST_TIME . '.' . $info['extension']);

                // Move the temporary file into the final location.
                if ($picture = file_move($picture, $destination, FILE_EXISTS_RENAME)) {
                  $picture->status = FILE_STATUS_PERMANENT;
                  $account->picture = file_save($picture);
                  file_usage_add($picture, 'user', 'user', $account->uid);
                }
             }
           $pictureFid = $account->picture->fid;
	    }

	   $userInfo = array(
                    'picture'       => $account->picture
	                );
      //updateAccount($userInfo,$user->uid);
      $u=user_save($account, $userInfo);

				$aImageInfo = getimagesize($destination);	
				header("Content-type: text/html; charset=UTF-8");
				ob_end_clean();
				ob_start();
				$rtn =  "{";
				$rtn .=	"error: '" . $aImageInfo[0] . "',\n";
				$rtn .=	"filename : '" . file_create_url($destination) . "',\n";
				$rtn .=	"imgwidth : '" . $aImageInfo[0] . "',\n";
				$rtn .=	"imgheight : '" . $aImageInfo[1] . "'\n";  					
				$rtn .= "}";
				echo $rtn;
				return $rtn;
				break;
		}
		
	}
	
	/**
    * Create a new forum.
    * @param $data
    *   - An associative array which contains title, body, language and term ID to create forum. 
    *   - term ID is the id of the container forum (Learning Forums). It is an Optional.
    * @return
    *   - A fully loaded node objects with JSON format.
    */
	public function addForumTopic($data){		
		global $user;
		$forum_new = new StdClass();								
		$forum_new->uid=$user->uid;
		$forum_new->created=time();
		$forum_new->type='forum';
		$forum_new->language=$data['forumTopicLang'];
		$forum_new->changed='';
		$forum_new->title=urldecode($data['forumTopicSubj']);
		$forum_new->teaser_js='';
		$forum_new->teaser_include=1;
		$forum_new->body=urldecode($data['forumTopicBody']);
		$forum_new->format=1;
		$forum_new->revision=0;
		$forum_new->log='';
		$forum_new->name=$user->name;
		$forum_new->date='';
		$forum_new->status=1;
		$forum_new->promote=0;
		$forum_new->sticky=0;
		$forum_new->op="Save";
		$forum_new->submit="Save";
		$forum_new->preview="Preview";
		$forum_new->form_id="forum_node_form";
		$forum_new->comment=COMMENT_NODE_READ_WRITE;
		
		$forum_new->taxonomy=array();
		$tid=$data["termId"];
		if(!isset($tid) || $tid==''){
			$tid=db_query("select tid from term_data where name = 'Learning Forums'")->fetchField();	
		}		
		$forum_new->taxonomy[1]=$tid;
		$forum_new->teaser=$data['forumTopicBody'];
		$forum_new->validated=1;		
		
		node_save($forum_new);		
		
		$result=array();
		$result['nid']=$forum_new->nid;		
		return $this->getDrupalNode($result);
					
	}
	
	/**
    * Update a forum topic.
    * @param $data 
    *   - An associative array which contains node ID of the forum.
    *   
    * @return 
    *   - A fully loaded node objects with JSON format.
    */
	public function updateForumTopic($data){
		global $user;
		$forum_upd = node_load($data['nid']);								
		$forum_upd->changed=time();
		$forum_upd->title=urldecode($data['forumTopicSubj']);
		$forum_upd->body=urldecode($data['forumTopicBody']);
		node_save($forum_upd);
		db_query("commit");
		
		$result=array();
		$result['nid']=$forum_upd->nid;
		return $this->getDrupalNode($result);
	}
	
	/**
    * Create a comment for a forum.
    * @param $data
    *   - An associative array which contains title and description of the comment.
    *   - node ID of the forum. 
    * @return
    *   - A fully loaded node objects with JSON format.
    */
	public function addForumComment($data){
		global $user;
		$comment = Array();
		$comment['nid']=$data['nid'];
		$comment['subject']=urldecode($data['title']);
		$comment['comment']=urldecode($data['desc']);
		$comment['author']=$user->name;
		$comment['format']=1;
		$comment['op']="Save";
		$comment['submit']="Save";
		$comment['preview']="Preview";
		$comment['form_build_id']="form-1f94432f3c15d879a153e9068440d8cc";
		$comment['form_token']="be1700584d79eba086aa345759d9d713";
		$comment['form_id']="comment_form";								
		$comment['uid']=$user->uid;
		comment_save($comment);
		$result=array();
		$result['nid']=$comment->nid;
		return $this->getDrupalNode($result);
	}
	
	/**
    * Not used this function.
    * 
    */
	public function addDrupalNode($data){
		
	}	
	
	/**
    * Update the node.
    * @param $data
    *   - An associative array which contains title and description of the comment.
    *   - node ID of the forum/page. 
    * @return
    *   - A fully loaded node objects with JSON format.
    */
	public function updateDrupalNode($data){
		global $user;
		$node=new StdClass();
		$node->nid=$data['nid'];
		$node->vid=$data['nid'];
		$node->body=$data['nodeBody'];
		$node->title=$data['nodeSubj'];
		$node->uid=$user->uid;		
		node_save($node);
		$node=node_load($data['nid']);
		return drupal_json_output($node);
	}
	
	/**
    * Delete a node.
    * @param $data
    *   - An associative array which contains node ID of the forum/page.
    * @return
    *   - A fully loaded $data objects with JSON format.
    */
	public function deleteDrupalNode($data){
		node_delete($data['nid']);
		return drupal_json_output($data);		
	}
	
	/**
    * Get information of node.
    * @param $data
    *   - An associative array which contains node ID of the forum/page.
    * @return
    *   - A fully loaded node objects with JSON format.
    */
	public function getDrupalNode($data){
		db_query("commit");
		$node	= node_load(array('nid'=>$data['nid']));
		//Modified by Ilayraja, changed standard time format, #2091
		$node->created		= format_date($node->created,custom,'d-F-Y, h:i A');		
		$node->changedstr	= format_date($node->changed,custom,'d-F-Y, h:i A');
		//End Modified by Ilayraja, changed standard time format, #2091

		$node->title		= rawurlencode($node->title);
		$node->body			= rawurlencode($node->body);
		$node->teaser		= urlencode($node->teaser);
		//Added by Ilayaraja, #2091
		$node->posted_on	= format_date($node->last_comment_timestamp,custom,'d-F-Y, h:i A');
		return drupal_json_output($node);		
	}
	
	/**
    * Check whether forum is enabled or not.
    * @param $data
    *   - An object which contains boolean value of isGroupForum.
    * @return
    *   - Enabled/disabled value with JSON format.
    */
	public function isForumEnabled($data){
		$isEnabled="false";		
		$check=$data->isGroupForum?function_exists('forum_menu') && function_exists('og_menu') && function_exists('og_forum_menu'):function_exists('forum_menu');
		if($check) { 
			$isEnabled="true";
		}
		$retArr=new StdClass();
		$retArr->isEnabled = $isEnabled;
		return drupal_json_output($retArr);
	}
	
	/**
    * Add/update a group.
    * @param $data
    *   - An associative array which contains title of the Training program/Course.
    *   - node ID of the Training program/Course. 
    * @return
    *   - A fully loaded node objects with JSON format.
    */
	public function addUpdateGroup($data){
		global $user;
		$group = new StdClass();
		$desc=t("User Community to hold discussions around the learning - ").$data['title'];
		if($data['nid']!=''){		
			//check if the user is in the group?
			if(!$this->isUserInGroup($data['nid'],$user->uid)){
				$this->addUserToGroup(array('groupId' => $data['nid'],'uid'=>$user->uid));
			}	
			$group=node_load($data['nid']);;
			$group->title=$data['title'];		
			$group->body=$desc;
			$group->og_description=$desc;
			$group->teaser=$desc;		
			node_save($group);
		} else {
			$group->nid='';
			$group->vid='';								
			$group->uid=$user->uid;
			$group->created=time();
			$group->type='group';
			$group->language='';
			$group->changed='';
			$group->title=$data['title'];
			$group->teaser_js='';
			$group->tease_include=1;
			$group->body=$desc;
			$group->format=2;
			$group->revision=1;
			$group->log='';
			$group->name=$user->name;
			$group->date='';
			$group->status=1;
			$group->promote='';
			$group->sticky=1;
			$group->op="Save";
			$group->submit="Save";
			$group->preview="Preview";
			$group->form_build_id="form-aaa9719efecc0ca42fdfa97da50ea8fa";
			$group->form_token="68194055d1f3dbdb23a7d71ecac4f638";
			$group->form_id="group_node_form";
			$group->comment=0;
			$group->menu=array();
			$group->menu['mlid']=0;
			$group->menu['module']="menu";
			$group->menu['hidden']=0;
			$group->menu['has_children']=0;
			$group->menu['customized']=0;
			//$menu->options=Array();
			$group->menu['expanded']=0;
			$group->menu['parent_depth_limit']=8;
			$group->menu['link_title']='';
			$group->menu['parent']="primary-links:0";
			$group->menu['weight']=0;
			$group->menu['plid']=0;
			$group->menu['menu_name']="primary-links";			
			$group->og_description=$desc;
			$group->og_selective=0;
			$group->og_register='';
			$group->og_directory=1;
			$group->theme='';
			$group->og_private=0;
			$group->notifications_content_disable=1;
			$group->teaser=$desc;
			$group->validated=1;
			node_save($group);			
		}		
		$result=array();
		$result['nid']=$group->nid;
		$node=node_load(array('nid'=>$group->nid));
		/*$tid=db_query("select tid from og_term where nid = ".$group->nid)->fetchField();
		$node->termId=$tid*/;			
		return drupal_json_output($node);		
	}
	
	/**
    * Check whether user is subscriped to the specified Training program/Course group.
    * @param $groupId,$uid
    *  
    * @return boolean  
    */
	private function isUserInGroup($groupId,$uid){
		$count=0;
		/*if($groupId!=null && $groupId!='')
			$count=db_query("select count(1) from og_uid where nid=".$groupId." and uid=".$uid)->fetchField();
     */
		return $count == 0?false:true;
	}
	
	/**
    * Short description about this function
    * @param boolean $paramname 
    * @return integer
    */
	public function addUserToGroup($data){
		/*$uid=$data["uid"];
		global $user;
		if(!isset($uid) || $uid == ''){
			$uid=$user->uid;
		}
		$isPresent=$this->isUserInGroup($data["groupId"],$uid);
		if(!$isPresent){
		//og_save_subscription($data["groupId"], $uid, array('is_active' => 1));
		}*/
		return '';
		//return drupal_json_output($user);
	}
	
	/**
    * user is unsubscriped from the specified Training program/Course group.
    * @param $data
    *   - An associative array which contains groupId of the Training program/Course.
    *   - User ID of the person who cancel the registration of the Training program/Course. 
    * @return
    *   - A fully loaded user objects with JSON format.
    */
	public function removeUserFromGroup($data){
		/*$uid=$data["uid"];
		global $user;
		if(!isset($uid) || $uid == ''){
			$uid=$user->uid;			     
		}
		$isPresent=$this->isUserInGroup($data["groupId"],$uid);
		if($isPresent){
			og_delete_subscription($data["groupId"],$uid);
		}		
		return drupal_json_output($user);*/
	    return '';
	}
	
	/**
    * user is subscriped to the specified Training program/Course group.
    * @param $data
    *   - An associative array which contains entity id and enity type.
    *   - User ID of the person who cancel the registration of the Training program/Course. 
    * @return
    *   - A fully loaded user objects with JSON format.
    */
	public function subscriptionTrainingProgramToOG($data){		
		try
  		{
			$data['groupId']=db_query("select node_id from slt_node_learning_activity where entity_type='".$data['entity_type']."' and entity_id=".$data['entity_id'])->fetchField();
			$this->addUserToGroup($data);
		}
		catch(Exception $e)
		{			
			$og_res[] = $e->getMessage();
			return drupal_json_output($og_res);
		}
	}
	
	/**
    * user is unsubscriped from the specified Training program/Course group.
    * @param $data
    *   - An associative array which contains entity id and enity type.
    *   - User ID of the person who cancel the registration of the Training program/Course. 
    * @return
    *   - A fully loaded user objects with JSON format.
    */
	public function unsubscriptionTrainingProgramToOG($data){		
		try
  		{
			$data['groupId']=db_query("select node_id from slt_node_learning_activity where entity_type='".$data['entity_type']."' and entity_id=".$data['entity_id'])->fetchField();
			$this->removeUserFromGroup($data);
		}
		catch(Exception $e)
		{			
			$og_res[] = $e->getMessage();
			return drupal_json_output($og_res);
		}
	}
}

$drupalServ=new DrupalService();
$rtn =  $drupalServ->process();

?>
