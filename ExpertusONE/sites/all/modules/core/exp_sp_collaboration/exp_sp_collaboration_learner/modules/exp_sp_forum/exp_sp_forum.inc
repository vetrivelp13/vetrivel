<?php
class ForumDetails{
  /**
   * An indentifier
   * @var string
   */
  public $select = null;
  public $limit;
  public $page;
  public $start = 0;
  public $total_pages;


  function __construct() {
    if(!empty($_GET['rows']))
    $this->limit     = $_GET['rows'];
    if(!empty($_GET['page']))
    $this->page      = $_GET['page'];
  }


  /**
   * Funtion to return the value of $_GET method.
   *
   * @param string $index_var
   * $index_var- String value
   *
   * @return
   *   Results of $_GET method
   */
  public function setFrmParamVal($index_var){
    try{
      return isset($_GET[$index_var]) ? $_GET[$index_var]:'';
    }catch (Exception $ex) {
      watchdog_exception('setFrmParamVal', $ex);
      expertusErrorThrow($ex);
    }
  }

  /**
   *
   */
  function frmListCalculatePagination() {
    try{
      $countSelect = $this->select;
      $list = $countSelect->execute()->fetchCol();
      
      expDebug::dPrintDBAPI("PAGINATION----BALAG-----  : ",$countSelect);
      
      $this->frm_list_count = 0;
      $this->frm_list_count = count($list);
      
      if ( $this->frm_list_count > 0 ) {
        $this->total_pages = ceil($this->frm_list_count/$this->limit);
      } else {
        $this->total_pages = 0;
      }
      if ($this->page > $this->total_pages) $this->page=$this->total_pages;
      $this->start = $this->limit*$this->page - $this->limit;
      if ($this->start < 0) {
        $this->start = 0;
      }
    }catch (Exception $ex) {
      watchdog_exception('frmListCalculatePagination', $ex);
      expertusErrorThrow($ex);
    }
  }

  /**
   *
   */  
  function forumSearchQueryPageCount(){
    try{
      $this->forumClassSearchQuery();
      $this->select->condition('ttd.vid',2,'=');
      $this->select->condition('ttd.description','Course','=');
      //$this->select->condition('crs.status','lrn_crs_sts_atv','=');
      $this->select->condition('crs.discussion',1,'!=');
      $this->select->condition('crs.status','lrn_crs_sts_atv','=');
      if(!empty($_GET['title']) && mb_strtolower($_GET['title']) != mb_strtolower(t('LBL304'))) {
        $this->select->condition('crs.title','%' . addslashes($_GET['title']) . '%','like');
      }
      $this->select->groupby('crs.id');
      //filterByUserAccessPrivileges($this->select, 'catacs','','group_map');
      filterByUserAccessPrivileges($this->select, 'crs','','','cre_sys_obt_crs','Forum');
      $crsQuery=$this->select;
      expDebug::dPrintDBAPI('$crsQuery---------> ',$crsQuery);
        
      $this->forumTPSearchQuery();
      $this->select->condition('ttd1.vid',2,'=');
      $this->select->condition('ttd1.description','TP','=');
      //$this->select->condition('prm.status','lrn_lpn_sts_atv','=');
      $this->select->condition('prm.discussion',1,'!=');
      $this->select->condition('prm.status','lrn_lpn_sts_atv','=');
      if(!empty($_GET['title']) && mb_strtolower($_GET['title']) != mb_strtolower(t('LBL304'))) {
        $this->select->condition('prm.title','%' . addslashes($_GET['title']) . '%','like');
      }  
      $this->select->groupby('prm.id');
      //filterByUserAccessPrivileges($this->select, 'catacs','','group_map');
      filterByUserAccessPrivileges($this->select, 'prm','','','','Program');
      $tpQuery=$this->select;
      
      $resultQuery = $crsQuery->union($tpQuery,'UNION ALL');
      //expDebug::dPrintDBAPI('discussion  search results ---------> ',$resultQuery);
      return $resultQuery;
    }catch (Exception $ex) {
          watchdog_exception('forumSearchQueryPageCount', $ex);
          expertusErrorThrow($ex);
    } 
  
  }
  
  /**
   *
   */  
  function forumSearchQuery($fromCal = ''){
    try{
        $this->forumClassSearchQuery($fromCal);
        $this->select->condition('ttd.vid',2,'=');
        $this->select->condition('ttd.description','Course','=');
        //$this->select->condition('crs.status','lrn_crs_sts_atv','='); 
        $this->select->condition('crs.discussion',1,'!=');   
        $this->select->condition('crs.status','lrn_crs_sts_atv','=');
        if(!empty($_GET['title']) && mb_strtolower($_GET['title']) != mb_strtolower(t('LBL304'))){
          //$this->select->condition('crs.title','%' . addslashes($_GET['title']) . '%','like')
		  $this->select->condition(db_and()->
      				condition(db_or()->
      					  condition('crs.title','%' . addslashes($_GET['title']) . '%','like')
						->condition('td.tagname','%' . addslashes($_GET['title']) . '%','like')
      			));
			
        }
        $this->select->groupby('crs.id');
        //filterByUserAccessPrivileges($this->select, 'catacs','','group_map');
        filterByUserAccessPrivileges($this->select, 'crs','','','cre_sys_obt_crs','Forum');
        if(!empty($fromCal)){
        	if($fromCal == 'list')
        		$this->select->where("((FROM_UNIXTIME(fi.last_comment_timestamp,'%Y-%m-%d') = '".$_REQUEST['startDate']."') OR (FROM_UNIXTIME(n.changed,'%Y-%m-%d') = '".$_REQUEST['startDate']."') OR (FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') = '".$_REQUEST['startDate']."'))");
        	else
        		$this->select->where("(((FROM_UNIXTIME(fi.last_comment_timestamp,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(fi.last_comment_timestamp,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')) OR ((FROM_UNIXTIME(n.changed,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(n.changed,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')) OR ((FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')))");
        	
        }
        $crsQuery=$this->select;
        
              
        $this->forumTPSearchQuery($fromCal);
        $this->select->condition('ttd1.vid',2,'=');
        $this->select->condition('ttd1.description','TP','=');
       // $this->select->condition('prm.status','lrn_lpn_sts_atv','=');
        $this->select->condition('prm.discussion',1,'!=');
        $this->select->condition('prm.status','lrn_lpn_sts_atv','=');
        if(!empty($_GET['title']) && mb_strtolower($_GET['title']) != mb_strtolower(t('LBL304'))) {
          //$this->select->condition('prm.title','%' . addslashes($_GET['title']) . '%','like');
          $this->select->condition(db_and()->
      				condition(db_or()->
      					  condition('prm.title','%' . addslashes($_GET['title']) . '%','like')
						->condition('td.tagname','%' . addslashes($_GET['title']) . '%','like')
      			));
        }
        $this->select->groupby('prm.id');
       // filterByUserAccessPrivileges($this->select, 'catacs','','group_map');
        filterByUserAccessPrivileges($this->select, 'prm','','','','Program');
        $this->frmListSortBy($this->setFrmParamVal('sortby'));
        $this->select->range($this->start, $this->limit);  
       
        
        if(!empty($fromCal)){
        	if($fromCal == 'list' || ($fromCal != 'list' && !empty($_REQUEST['startDate']) && empty($_REQUEST['endDate'])))
        		$this->select->where("((FROM_UNIXTIME(fi1.last_comment_timestamp,'%Y-%m-%d') = '".$_REQUEST['startDate']."') OR (FROM_UNIXTIME(n.changed,'%Y-%m-%d') = '".$_REQUEST['startDate']."') OR (FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') = '".$_REQUEST['startDate']."'))");
        	else
        		$this->select->where("(((FROM_UNIXTIME(fi1.last_comment_timestamp,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(fi1.last_comment_timestamp,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')) OR ((FROM_UNIXTIME(n.changed,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(n.changed,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')) OR ((FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') >= '".$_REQUEST['startDate']."') AND (FROM_UNIXTIME(cmt.changed,'%Y-%m-%d') <= '".$_REQUEST['endDate']."')))");
        }
        $tpQuery=$this->select;
        $resultQuery = $crsQuery->union($tpQuery,'UNION ALL');
        
        return $resultQuery;
      }catch (Exception $ex) {
            watchdog_exception('forumSearchQuery', $ex);
            expertusErrorThrow($ex);
      } 
    
    }
  
  /**
   *
   */  
  function forumClassSearchQuery($fromCal = ''){
    try{
      $this->select = db_select("slt_course_template","crs");
      $this->select->leftJoin('slt_course_class', 'cls', 'cls.course_id = crs.id');
      $this->select->leftJoin('taxonomy_term_data', 'ttd', 'ttd.format = crs.id');
      $this->select->leftJoin('forum_index', 'fi', 'fi.tid= ttd.tid and fi.last_comment_timestamp = (SELECT last_comment_timestamp FROM forum_index WHERE tid=ttd.tid order by last_comment_timestamp desc limit 1)');
      $this->select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
	  $this->select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
      //$this->select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=crs.id AND catacs.entity_type = \'cre_sys_obt_crs\'');        
      //$this->select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\'');
      //$this->select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
	
      
      if(empty($fromCal)){
      	$this->select->leftJoin('node', 'n', 'n.nid = fi.nid');
      	$this->select->leftJoin('slt_profile_list_items', 'splt_language', 'splt_language.code=crs.lang_code');
      	$this->select->addField('ttd','vid','vid');
      	$this->select->addField('ttd','format','format');
      	$this->select->addField('ttd','tid','tid');
      	$this->select->addField('crs','title','name');
      	$this->select->addField('crs','code','code');
      	$this->select->addField('crs','created_on','createdon');
      	$this->select->addField('crs','id','object_id');
      	$this->select->addField('crs','short_description','description');
      	$this->select->addField('splt_language','name','language_name');
      	$this->select->addField('n','uid','userid');
		//$this->select->addField('td','tagname', 'tagname');
      	$this->select->addExpression('FROM_UNIXTIME(fi.last_comment_timestamp,\'%b %d, %Y %h:%i %p\')','lct');
      	$this->select->addExpression("'cre_sys_obt_crs'", 'object_type');
      }else{
      	$this->select->leftJoin('slt_node_learning_activity', 'nla', 'nla.custom0 = \'forum\' AND nla.entity_id = crs.id and nla.entity_type = \'Course\'');
      	$this->select->leftJoin('node', 'n', 'n.nid = nla.node_id');
      	$this->select->leftJoin('comment', 'cmt', 'cmt.nid = n.nid');
      	$this->select->addField('ttd','tid','tid');
      	$this->select->addField('crs','title','title');
		//$this->select->addField('td','tagname', 'tagname');
      	$this->select->addExpression("'discussion'",'type');
      	$this->select->addExpression("CONCAT('discussion-',ttd.tid)",'id');
      	$this->select->addField('crs','created_on','createdon');
      	$this->select->addExpression('DATE_FORMAT(crs.created_on,\'%Y-%m-%d\')','start');
      	$this->select->addExpression('DATE_FORMAT(crs.created_on,\'%H:%i\')','startTime');
      }
      
      return $this->select; 
      }catch (Exception $ex) {
      	watchdog_exception('forumClassSearchQuery', $ex);
      	expertusErrorThrow($ex);
      }
  }

  /**
   *
   */  
  function forumTPSearchQuery($fromCal=''){
     try{ 
      $this->select = db_select("slt_program","prm");
      $this->select->leftJoin('taxonomy_term_data', 'ttd1', 'ttd1.format = prm.id');
      $this->select->leftJoin('forum_index', 'fi1', 'fi1.tid= ttd1.tid and fi1.last_comment_timestamp = (SELECT last_comment_timestamp FROM forum_index WHERE tid=ttd1.tid order by last_comment_timestamp desc limit 1)');
      $this->select->leftJoin('slt_tag_entity','te','((prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
      $this->select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
      //$this->select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=prm.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
      //$this->select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=prm.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');      
     // $this->select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
      if(empty($fromCal)){
	      $this->select->leftJoin('node', 'n', 'n.nid = fi1.nid');
      	$this->select->leftJoin('slt_profile_list_items', 'splt_language1', 'splt_language1.code=prm.lang_code');
      	$this->select->addField('ttd1','vid','vid');
      	$this->select->addField('ttd1','format','format');
      	$this->select->addField('ttd1','tid','tid');
      	$this->select->addField('prm','title','name');
      	$this->select->addField('prm','code','code');
      	$this->select->addField('prm','created_on','createdon');
      	$this->select->addField('prm','id','object_id');
      	$this->select->addField('prm','short_desc','description');
      	$this->select->addField('splt_language1','name','language_name');
      	$this->select->addField('n','uid','userid');
		//$this->select->addField('td','tagname', 'tagname');
      	$this->select->addExpression('FROM_UNIXTIME(fi1.last_comment_timestamp,\'%b %d, %Y %h:%i %p\')','lct');
      	$this->select->addExpression('prm.object_type', 'object_type');
      }else{
      	$this->select->leftJoin('slt_node_learning_activity', 'nla', 'nla.custom0 = \'forum\' AND nla.entity_id = prm.id and nla.entity_type = \'TP\'');
      	$this->select->leftJoin('node', 'n', 'n.nid = nla.node_id');
      	$this->select->leftJoin('comment', 'cmt', 'cmt.nid = n.nid');
      	$this->select->addField('ttd1','tid','tid');
      	$this->select->addField('prm','title','title');
		//$this->select->addField('td','tagname', 'tagname');
      	$this->select->addExpression("'discussion'",'type');
      	$this->select->addExpression("CONCAT('discussion-',ttd1.tid)",'id');
      	$this->select->addField('prm','created_on','createdon');
      	$this->select->addExpression('DATE_FORMAT(prm.created_on,\'%Y-%m-%d\')','start');
      	$this->select->addExpression('DATE_FORMAT(prm.created_on,\'%h:%i\')','startTime');
      }
      
      
      
      return $this->select; 
      }catch (Exception $ex) {
      	watchdog_exception('forumTPSearchQuery', $ex);
      	expertusErrorThrow($ex);
      }
  }
  
  /**
   *
   */
  function getFrmListResults() {
    try{
      $this->select = $this->forumSearchQueryPageCount();
      $this -> frmListCalculatePagination();
      $this->select = $this->forumSearchQuery();

      $result = $this->select->execute()->fetchAll();

      expDebug::dPrintDBAPI("FORUM LIST QUERY----BALAG-----  : ",$this->select);
      return $result;
    }catch (Exception $ex) {
      watchdog_exception('getFrmListResults', $ex);
      expertusErrorThrow($ex);
    }
  }

  /**
   *
   */
  function frmListSortBy($sortby) {
    try{
      if ($sortby == "AZ") {
        $xorderby = "name";
        $xorderbylist = "asc";
      } else if ($sortby == "ZA") {
        $xorderby = "name";
        $xorderbylist="desc";
      } else if ($sortby == "created") {
        $xorderby = "createdon";
        $xorderbylist="desc";
      } else if ($sortby == "posttime") {
        $xorderby = "lct";
        $xorderbylist="desc";
      } /*else if ($sortby == "registration") {
        $xorderby = "enrollid";
        $xorderbylist="asc";
      }*/ else{
        $xorderby = "createdon";
        $xorderbylist="desc";
      }

      $this->select->orderBy($xorderby,$xorderbylist);
    }catch (Exception $ex) {
      watchdog_exception('frmListSortBy', $ex);
      expertusErrorThrow($ex);
    }
  }


  /**
   *
   */

  function getFrmTopicListResults($tId) {
    try{

      $this->select = db_select("forum_index","fi");
      $this->select->leftJoin('field_data_body', 'fd_topic', 'fd_topic.entity_id = fi.nid');
      $this->select->leftJoin('node', 'n', 'n.nid = fi.nid');
      $this->select->leftJoin('node_comment_statistics', 'ncs', 'ncs.nid = fi.nid');
      $this->select->addField('fi','nid','nid');
      $this->select->addField('fi','title','title');
      $this->select->addField('fi','tid','tid');
      $this->select->addExpression('FROM_UNIXTIME(fi.created,\'%b %d, %Y %h:%i %p\')','created');
      $this->select->addField('fi','created','lst_cmt');
      $this->select->addField('fi','last_comment_timestamp','last_comment_timestamp');
      $this->select->addField('fi','comment_count','comment_count');
      $this->select->addField('fd_topic','body_value','description');
      $this->select->addField('n','uid','userid');
      $this->select->addField('ncs','last_comment_uid','lastcmtuid');

      $this->select->condition('fi.tid',$tId,'=');      
      if(!empty($_GET['title']) && mb_strtolower($_GET['title']) != mb_strtolower(t('LBL304'))) {
        $this->select->condition('fi.title','%' . db_like($_GET['title']) . '%','like');
      }
      $this->frmTopicListCalculatePagination();
      $this->frmTopicListSortBy($this->setFrmParamVal('sortby'));
      $this->select->range($this->start, $this->limit);

      $result = $this->select->execute()->fetchAll();

      expDebug::dPrintDBAPI("FORUM TOPIC LIST QUERY  : ",$this->select);
      return $result;
    }catch (Exception $ex) {
      watchdog_exception('getFrmListResults', $ex);
      expertusErrorThrow($ex);
    }
  }




  /**
   *
   */
  function frmTopicListCalculatePagination() {
    try{
      $countSelect = $this->select;

      $list = $countSelect->execute()->fetchCol();

      $this->frm_topic_list_count = 0;
      $this->frm_topic_list_count = count($list);

      if ( $this->frm_topic_list_count > 0 ) {
        $this->total_pages = ceil($this->frm_topic_list_count/$this->limit);
      } else {
        $this->total_pages = 0;
      }

      if ($this->page > $this->total_pages) $this->page=$this->total_pages;

      $this->start = $this->limit*$this->page - $this->limit;
      if ($this->start < 0) {
        $this->start = 0;
      }
    }catch (Exception $ex) {
      watchdog_exception('frmListCalculatePagination', $ex);
      expertusErrorThrow($ex);
    }
  }

  /**
   *
   */
  function frmTopicListSortBy($sortby) {
    try{
      if ($sortby == "AZ") {
        $xorderby = "title";
        $xorderbylist = "asc";
      } else if ($sortby == "ZA") {
        $xorderby = "title";
        $xorderbylist="desc";
      } else if ($sortby == "created") {
        $xorderby = "lst_cmt";
        $xorderbylist="desc";
      } else if ($sortby == "posttime") {
        $xorderby = "last_comment_timestamp";
        $xorderbylist="desc";
      } else{
        $xorderby = "lst_cmt";
        $xorderbylist="desc";
      }

      $this->select->orderBy($xorderby,$xorderbylist);
    }catch (Exception $ex) {
      watchdog_exception('frmListSortBy', $ex);
      expertusErrorThrow($ex);
    }
  }



}  //-------------------------ForumDetails End--------------------------



/**
 *
 */
function getCommentsList($nId,$tId){
  try{
    $select = db_query('SELECT
      cmt.cid AS cid, 
      cmt.pid AS pid, 
      cmt.nid AS nid, 
      cmt.uid AS uid, 
      cmt.subject AS subject, 
      SUM(usrpnt.total_points) AS totalPoints,
      FROM_UNIXTIME(cmt.created,\'%b %d %Y %h:%i %p\') AS created, 
      cmt.name AS name,
      cmt.thread AS thread, 
      fd_cmt.comment_body_value AS description,
      sp.id AS personid,
      sp.last_name AS lastname,
      sp.status AS status,
      sp.first_name AS firstname,
      fm.filename AS picture
      FROM 
      comment cmt
      LEFT OUTER JOIN field_data_comment_body fd_cmt ON fd_cmt.entity_id = cmt.cid
      LEFT OUTER JOIN users u ON u.uid = cmt.uid
      LEFT OUTER JOIN slt_person sp ON sp.user_name = u.name
      LEFT OUTER JOIN slt_user_points usrpnt ON usrpnt.user_id = sp.id
      LEFT OUTER JOIN file_managed fm ON fm.fid = u.picture
      WHERE  (cmt.nid = '.$nId.') group by(cmt.cid) order by SUBSTRING(thread, 1, (LENGTH(thread) - 1))');
    $result = $select->fetchAll();
    
    
    //------------------ Time Zone Conversion
    $vTimeZoneUser = date_default_timezone(false);    
    $UserPHPTimezoneName		= getUserPHPTimezoneName();
    $vUserPHPTimezonename		= $UserPHPTimezoneName[0]->zone_name;
    if(!$UserPHPTimezoneName[0]->zone_name){
      $vUserPHPTimezonename = $vTimeZoneUser;
    }

    $forumAdminAccess = forumAdminAccess();
    $c=0;
    foreach ($result as $rec_results) {
      $result[$c]->badge_code = getBadgesCodeForLeaderBoard($rec_results->totalpoints);
      if($vTimeZoneUser!=$vUserPHPTimezonename) {
        $date = new DateTime($rec_results->created, new DateTimeZone($vTimeZoneUser));
        $date->setTimezone(new DateTimeZone($vUserPHPTimezonename));
        $result[$c]->timezone = $date->format('M j Y h:i A');
      }else{
        $result[$c]->timezone = $rec_results->created;
      }
      
      //Added by Velu - this for hyperlink refer #0028796
      if($result[$c]->description != ""){
      	$result[$c]->description = $result[$c]->description;
      }
      //End
      if($result[$c]->personid != ""){
      	$result[$c]->personid = core_encrypt($result[$c]->personid);
      }
      $x = explode(" ",$result[$c]->timezone);
      $result[$c]->timezone = $x[0]." ".$x[1]." ".$x[2]." ".$x[3]." <span class='time-zone-text'>".$x[4]."</span>";
      $result[$c]->access = $forumAdminAccess;
      $c++;
    }
    
    
    
    echo drupal_json_output($result);
    expDebug::dPrintDBAPI("COMMENTS LIST QUERY  : ",$select);
  }catch (Exception $ex) {
    watchdog_exception('getCommentsList', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function getCountTopic($tId){
  try{
    $select = db_select('forum_index', 'fi');
    $select->addExpression('COUNT(fi.tid)', 'topic_count');
    $select->condition('fi.tid',$tId,'=');
    $topicCount = $select->execute()->fetchField();
    return $topicCount;
  }catch (Exception $ex) {
    watchdog_exception('getCountTopic', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function getCountPost($tId){
  try{
    $select = db_select('forum_index', 'fi');
    $select->addExpression('SUM(fi.comment_count)', 'comment_count');
    $select->condition('fi.tid',$tId,'=');
    $commentsCount = $select->execute()->fetchField();
    return $commentsCount;
  }catch (Exception $ex) {
    watchdog_exception('getCountPost', $ex);
    expertusErrorThrow($ex);
  }
}


/**
 *
 */
function getFrmDetails($tId) {
  try{
    $select = db_select("taxonomy_term_data","ttd");
    $select->addField('ttd','description','description');
    $select->addField('ttd','format','format');
    $select->condition('ttd.tid',$tId,'=');
    $result = $select->execute()->fetchAll();
   
    if($result[0]->description == "Course") {
      $select = db_select("slt_course_template","crs");
      $select->addField('crs','id','object_id');
      $select->addField('crs','title','name');
      $select->addExpression("'cre_sys_obt_crs'", 'object_type');
      $select->condition('crs.id',$result[0]->format,'=');
    }else{
      $select = db_select("slt_program","prm");
      $select->addField('prm','id','object_id');
      $select->addField('prm','title','name');
      $select->addExpression('prm.object_type', 'object_type');
      $select->condition('prm.id',$result[0]->format,'=');
    }

    $result = $select->execute()->fetchAll();
    
    expDebug::dPrintDBAPI("FORUM DETAIL QUERY  : ",$select);
    return $result;
  }catch (Exception $ex) {
    watchdog_exception('getFrmDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function deleteCommentDetails($tid,$nid,$cid) {
  try{
    comment_delete($cid);
    insertUserPointsForEachActionsPerformed(getIdOfLoggedInUser(),'add_topic_comment_reply',$cid,'delete');
  }catch (Exception $ex) {
    watchdog_exception('deleteCommentDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function deleteTopicDetails($tid,$nid){
 try{
    node_delete($nid);
    db_delete('forum')->condition('nid', $nid)->execute();
    db_delete('forum_index')->condition('nid', $nid)->execute();
    db_delete('taxonomy_index')->condition('nid', $nid)->execute();
    db_delete('field_data_body')->condition('entity_id', $nid)->execute();
      
    $select = db_select("comment","c");
    $select->addField('c','cid','cid');
    $select->condition('c.nid',$nid,'=');
    $result = $select->execute()->fetchAll();    
    foreach($result as $res) {
      comment_delete($res->cid); // Delete Comments
    }
  }catch (Exception $ex) {
    watchdog_exception('deleteTopicDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function deleteForumDetails($entityId,$entityType){
  
  print $entityId."--".$entityType;
 try{
   
   if($entityType == "cre_sys_obt_crs") {
     
      $updateData = db_update('slt_course_template');
      $updateBodyFields = array(
                        'discussion' => 1
      );
      $updateData->fields($updateBodyFields);
      $updateData->condition('id', $entityId);
      $updateData->execute();
      
   }else{
     
      $updateData = db_update('slt_program');
      $updateBodyFields = array(
                        'discussion' => 1
      );
      $updateData->fields($updateBodyFields);
      $updateData->condition('id', $entityId);
      $updateData->execute();
   }
    
  }catch (Exception $ex) {
    watchdog_exception('deleteForumDetails', $ex);
    expertusErrorThrow($ex);
  }
}

/**
 *
 */
function getTopicAutocomplete($tId) {
     $title = $_GET['z'];
  try{
    $select = db_select("forum_index","fi");
    $select->leftJoin('field_data_body', 'fd_topic', 'fd_topic.entity_id = fi.nid');
    $select->leftJoin('node', 'n', 'n.nid = fi.nid');
    $select->leftJoin('node_comment_statistics', 'ncs', 'ncs.nid = fi.nid');
    $select->addField('fi','title','title');
    $select->condition('fi.title','%' . db_like($title) . '%','like');
    $select->condition('fi.tid',$tId,'=');
    $select_autocomplete = $select->execute()->fetchAll();
    foreach ($select_autocomplete as $result_select_autocomplete) {
      print $result_select_autocomplete->title."\n";
    }
  }catch(Exception $ex) {
     watchdog_exception('getTopicAutocomplete', $ex);
    expertusErrorThrow($ex);    
  }
}

/**
 *
 */
function getForumAutocomplete(){
    $title = $_GET['z'];
  try{
    $select = db_select("slt_course_template","crs");
    $select->leftJoin('slt_course_class', 'cls', 'cls.course_id = crs.id');
    $select->leftJoin('taxonomy_term_data', 'ttd', 'ttd.format = crs.id');
	$select->leftJoin('slt_tag_entity','te','(crs.id=te.entity_id and te.entity_type=\'Course\')');
	$select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
   // $select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=crs.id AND catacs.entity_type = \'cre_sys_obt_crs\'');
   //the bellow two lines are commented bcz this two line are unwanted we check the group permission in filterByUserAccessPrivileges
   //  $select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=crs.id AND group_map.entity_type = \'cre_sys_obt_crs\'');
   // $select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
    
    $select->addField('crs','title','name');
	//$select->addField('td','tagname', 'tagname');
    $select->condition('ttd.vid',2,'=');
    $select->condition('ttd.description','Course','=');
    //$select->condition('crs.status','lrn_crs_sts_atv','=');
   // $select->condition('crs.title','%' . $title . '%','like');
	$select->condition(db_and()->
      				condition(db_or()->
      					  condition('crs.title','%' . db_like($title) . '%','like')
						->condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')
      			));
    $select->condition('crs.discussion',1,'!=');
    $select->condition('crs.status','lrn_crs_sts_atv','=');
    $select->groupBy('crs.id');
    //filterByUserAccessPrivileges($select, 'catacs','','group_map');
    filterByUserAccessPrivileges($select, 'crs','','','cre_sys_obt_crs','Forum');
    $crsQuery=$select;
    
    $select = db_select("slt_program","prm");
    $select->leftJoin('taxonomy_term_data', 'ttd1', 'ttd1.format = prm.id');
	$select->leftJoin('slt_tag_entity','te','((prm.id= te.entity_id AND (te.entity_type=\'Certification\' OR te.entity_type=\'Curricula\' OR te.entity_type=\'Learning Plan\') ))');
    $select->leftJoin('slt_tagdefn','td','te.tagid= td.id');
    //$select->leftJoin('slt_catalog_access','catacs','catacs.entity_id=prm.id AND catacs.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
    ////the bellow two lines are commented bcz this two line are unwanted we check the group permission in filterByUserAccessPrivileges
    //$select->leftJoin('slt_group_mapping','group_map','group_map.entity_id=prm.id AND group_map.entity_type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
    //$select->leftJoin('slt_groups','catacs','catacs.id=group_map.group_id and (catacs.is_admin =0) and catacs.status =\'cre_sec_sts_atv\'');
    
    $select->addField('prm','title','name');
	//$select->addField('td','tagname', 'tagname');
    $select->condition('ttd1.vid',2,'=');
    $select->condition('ttd1.description','TP','=');
    //$select->condition('prm.status','lrn_lpn_sts_atv','=');
   // $select->condition('prm.title','%' . $title . '%','like');
    $select->condition(db_and()->
      				condition(db_or()->
      					  condition('prm.title','%' . db_like($title) . '%','like')
						->condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')
      			));
    $select->condition('prm.discussion',1,'!=');
    $select->condition('prm.status','lrn_lpn_sts_atv','=');
    //filterByUserAccessPrivileges($select, 'catacs','','group_map');
    filterByUserAccessPrivileges($select, 'prm','','','','Program');
    $select->groupBy('prm.id');
    $tpQuery=$select;
    
    $select = $crsQuery->union($tpQuery,'UNION ALL');   
    $select_autocomplete = $select->execute()->fetchAll();
    expDebug::dPrintDBAPI('get forum autocomplete select query' ,$select );
    foreach ($select_autocomplete as $result_select_autocomplete) {
      print $result_select_autocomplete->name."\n";
    }
  }catch(Exception $ex) {
     watchdog_exception('getForumAutocomplete', $ex);
    expertusErrorThrow($ex);    
  }
}


/*
 * forumAdminAccess()
 */
function forumAdminAccess(){
	try{
  return user_access('Forum Perm');
  }catch (Exception $ex) {
  	watchdog_exception('forumAdminAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * 
 */
function chechIfInsHasAccess($entityId,$entityType){
  try{
  if($entityType == "cre_sys_obt_crs") {
    $select = db_query('SELECT 
              group_concat(distinct(instructor_id)) AS ins_list
              FROM 
              slt_session_instructor_details ss
              WHERE ss.course_id = '.$entityId.'');
    expDebug::dPrintDBAPI("chechIfInsHasAccess  : ",$select);
    $result = $select->fetchAll();
    return $result;
  }else{
    $select = db_query('select 
              group_concat(distinct(ss.instructor_id)) AS ins_list
              from 
              slt_module_crs_mapping mm
              LEFT OUTER JOIN slt_session_instructor_details ss ON ss.course_id = mm.course_id
              where mm.program_id = '.$entityId.'');
    expDebug::dPrintDBAPI("chechIfInsHasAccess  : ",$select);
    $result = $select->fetchAll();
    return $result;
  }
  }catch (Exception $ex) {
  	watchdog_exception('chechIfInsHasAccess', $ex);
  	expertusErrorThrow($ex);
  }
}

?>