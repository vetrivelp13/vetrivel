<?php 

/**
 * MyteamCatalogSearch
 *
 * Catalog Search, which is used to search the Class,Certification and Curricula
 * and have option to find various types of Delivery Type, Language, Country and Location
 *
 * @package pagepackage
 * @author Product Team
 * @version 1.0
 * @copyright Copyright (c) 2011, ExpertUs
 * @since 07-Nov-2011
 */
class MyteamCatalogSearch{
  /**
   * An indentifier
   * @var string
   */
  private $select = null;
  private $user_role = null;
  public $limit;
  public $page;
  public $start = 0;
  public $total_pages;
  public $userId;

  public $delivery_type_name_distinct = array();
  public $language_distinct = array();
  public $country_name_distinct = array();
  public $full_search = array();

  function __construct() {
  	try{
    if(!empty($_GET['rows']))
    $this->limit     = $_GET['rows'];
    if(!empty($_GET['page']))
    $this->page      = $_GET['page'];
    } catch (Exception $ex) {
    	watchdog_exception('__construct', $ex);
    	expertusErrorThrow($ex);
    }
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
  public function myteamCatalogSetParamVal($index_var){
  	try{
    return isset($_GET[$index_var]) ? $_GET[$index_var]:'';
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSetParamVal', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function myteamCatalogSearchTitle($title) {
  	try{
    if (!empty($title)){
      $this->select->condition(db_and()->
      condition(db_or()->
      condition('ms.name', '%' . db_like($title) . '%', 'LIKE')->
      condition('ms.code', '%' . db_like($title) . '%', 'LIKE')->
      condition('ms.description', '%' . db_like($title) . '%', 'LIKE')->
	  condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')));
    }
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSearchTitle', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function myteamCatalogSearchType($dl_type,$ob_type,$titlesearch) {
  	try{
    if (!empty($dl_type) && !empty($ob_type)){
      $deliverytype = explode("|",$dl_type);
      $objecttype = explode("|",$ob_type);
      $this->select->condition(db_and()->
      condition(db_or()->
      condition('ms.delivery_type',$deliverytype,'IN')->
      condition('ms.type',$objecttype,'IN')));   
    } else if (!empty($dl_type) && $ob_type=='') {
      $deliverytype = explode("|",$dl_type);
      $this->select->condition(db_and()->
      condition(db_or()->
      condition('ms.delivery_type',$deliverytype,'IN')->
      condition('ms.type',$deliverytype,'IN')));   
      //$this->select->condition(db_and()->condition('ms.delivery_type',$deliverytype,'IN'));
      
    } else if ($dl_type=='' && !empty($ob_type)){
      $objecttype = explode("|",$ob_type);
      $this->select->condition(db_and()->condition('ms.type',$objecttype,'IN'));
      
    } else {
        $this->select->condition(db_and()->
        condition(db_or()->
        condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_wbt','lrn_cls_dty_vcl','lrn_cls_dty_vod'),'IN')->
        condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN')
      ));
    }
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSearchType', $ex);
    	expertusErrorThrow($ex);
    }
  }
  function myteamCatalogSearchDate() {
  	try{
  	$time = Date("H:i:s");
  	$toDay = Date("Y-m-d");
    $startdate = Date("Y-m-d");
    $this->select->where('IF(ms.type=\'Class\',
    ( 
    ( ms.delivery_type=\'lrn_cls_dty_ilt\' and cs.start_date >= \''.$startdate.'\' and 
    	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1) ) 
    or 
    ( ms.delivery_type=\'lrn_cls_dty_vcl\' and cs.start_date >= \''.$startdate.'\' and 
    	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)  ) 
    or 
    ( ms.delivery_type=\'lrn_cls_dty_wbt\' and content.content_id IS NOT NULL ) 
    )
    or
    ( ms.delivery_type=\'lrn_cls_dty_vod\' and content.content_id IS NOT NULL ),
    1=1)');
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSearchDate', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  /* --code added for assign learnign filter in mobile- Language|Mrotype|Location|Tag --*/
  function myteamCatalogSearchLanguage($lg_type,$titlesearch) {
  	try{
  		expDebug::dPrint("coming inside  myteamCatalogSearchLanguage     ".$lg_type , 4);
  
  		if ($lg_type!=''){
  			$langtype = explode("|",$lg_type);
  			$this->select->condition(db_and()->
  					condition(db_or()->
  							condition('cl.lang_code',$langtype,'IN')->
  							condition('prm.lang_code',$langtype,'IN')
  							));
  		}
  	} catch (Exception $ex) {
  		watchdog_exception('myteamCatalogSearchLanguage', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  function myteamCatalogSearchMrotype($mro_type,$titlesearch) {
  	try{
  		expDebug::dPrint("coming inside myteamCatalogSearchMrotype     ".$mro_type , 4);
  
  		if ($mro_type!=''){
  			$mrotype = explode("|",$mro_type);
  			$clsTblName = 'cl';
  
  			if(in_array('cre_sys_inv_com',$mrotype)){
  				$this->select->where('(group_map.mro IN (\''.implode('\',\'',$mrotype).'\')
	      	    AND (catacs.status = \'cre_sec_sts_atv\'))
	      	    OR ('.$clsTblName.'.is_compliance = 1 and (
             if((select count(1) from slt_groups gp,slt_group_mapping gm where gm.group_id=gp.id and (gm.entity_type = \'cre_sys_obt_crs\' OR gm.entity_type = \'cre_sys_obt_cls\') and gm.entity_id = '.$clsTblName.'.id and gp.status = \'cre_sec_sts_atv\')>0,
             catacs.id is not null, catacs.id is null)
             ))');
  			}
  			else{
  				$this->select->condition(db_and()->condition('group_map.mro',$mrotype,'IN'));
  			}
  		}
  	} catch (Exception $ex) {
  		watchdog_exception('myteamCatalogSearchLanguage', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  
  function myteamCatalogsearchLocation($location) {
  	try{
  		expDebug::dPrint("comming inside  myteamCatalogsearchLocation ".$location , 4);
  		if (!empty($location)){
  			$location = explode("|",$location);
  			$locationArray = db_or();
  			for($i=0;$i<count($location);$i++)
  			{
  				$locationArray->condition('ss.name', '%' . db_like($location[$i]) . '%', 'LIKE');
  			}
  			$this->select->condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl'),'IN'); //ms.delivery_type
  			$this->select->condition(db_and()->condition($locationArray));
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchLocation', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  function myteamCatalogsearchRating($rating_type,$type) {
  	try{
  
  		expDebug::dPrint("comming inside  myteamCatalogsearchRating     ".$voteValue , 4);
  
  		if (!empty($rating_type)){
  			$voteValue = explode("|",$rating_type);
  			$voteValuenew = explode("|",$rating_type);
  			$length = sizeof($voteValue);
  			if($length == 1) {
  				$this->select->condition(db_and()->
  						condition(db_or()->
  								condition(db_and()->
  										condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
  										where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue)))) )->
  
  								condition(db_and()->
  										condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
  										where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValuenew) - 19),':maxval'=>(max($voteValuenew)))) )
  								));
  
  				//   				if($type == 'Class') {
  				//   					$this->select->condition(db_and()->
  				//   							condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
  				//   							where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue))))
  				//   							);
  				//   				}else{
  				//   					$this->select->condition(db_and()->
  					//   							condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
  					//   							where("vote.value BETWEEN  :minval and :maxval",array(':minval'=>(min($voteValue) - 19),':maxval'=>(max($voteValue))))
  					//   							);
  					//   				}
  
  				}else{
  					$str = '';
  					for($i=0; $i < $length; $i++){
  						$str .= "(vote.value BETWEEN ".($voteValue[$i]-19)." and ".($voteValue[$i]).")";
  						if($i != $length-1) {
  							$str .= ' OR ';
  						}
  					}
  
  					$this->select->condition(db_and()->
  							condition(db_or()->
  									condition(db_and()->
  											condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
  											where("$str") )->
  
  									condition(db_and()->
  											condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
  											where("$str") )
  									));
  
  					//   				if($type == 'Class') {
  					//   					$this->select->condition(db_and()->
  					//   							condition('ms.delivery_type',array('lrn_cls_dty_ilt','lrn_cls_dty_vcl','lrn_cls_dty_wbt','lrn_cls_dty_vod'),'IN')->
  					//   							where("$str")
  					//   							);
  					//   				}else{
  					//   					$this->select->condition(db_and()->
  					//   							condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN') ->
  					//   							where("$str")
  					//   							);
  					//   				}
  				}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchRating', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  function myteamCatalogsearchTag($tag) {
  	try{
  		expDebug::dPrint('TAGS VALUE---->'.$tag,4);
  		expDebug::dPrint('TAGS VALUE---->'.rawurldecode($tag),4);
  		if (!empty($tag)){
  			if(isset($_REQUEST['mobile_request'])){
  				$tags = explode ( "|", $tag );
  				$this->select->condition ( db_and ()->condition ( 'td.tagname', $tags, 'IN' ) );
  			}else{
  				//$this->select->condition(db_and()->condition('td.tagname', rawurldecode($tag), '='));
  				$this->select->where('td.tagname=:tagname', array(':tagname' => rawurldecode($tag)));
  			}
  		}
  	}catch (Exception $ex) {
  		watchdog_exception('searchTag', $ex);
  		expertusErrorThrow($ex);
  	}
  }
  
  
  function myteamCatalogSetSearchParam($titleSrch=false){
  	try{
    if($titleSrch)
      $this->myteamCatalogSearchTitle($this->myteamCatalogSetParamVal('title'));    
    $this->myteamCatalogSearchType($this->myteamCatalogSetParamVal('dl_type'),$this->myteamCatalogSetParamVal('ob_type'),$this->myteamCatalogSetParamVal('title'));
    if(isset($_REQUEST['apiname']) && $_REQUEST['mobile_version']) {
    	//code added for assign learning filters in mobile ( Language|Mrotype|Location|Tag )
    	$this->myteamCatalogSearchLanguage($this->myteamCatalogSetParamVal('lg_type'),$this->myteamCatalogSetParamVal('title'));
    	$this->myteamCatalogSearchMrotype($_GET['mro_type'],$this->myteamCatalogSetParamVal('title'));
    	$this->myteamCatalogsearchLocation($this->myteamCatalogSetParamVal('location', false));
    	$this->myteamCatalogsearchTag($_GET['tag']);
    	if(!empty($_SESSION['availableFunctionalities']->exp_sp_fivestar)) {
    		$this->myteamCatalogsearchRating($this->myteamCatalogSetParamVal('rating_type'),'Class');
    		expDebug::dPrint(' $selectedFilters = ' . print_r($this->searchRating, true) , 4);
    	}
    }
    $this->myteamCatalogSearchDate();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSetSearchParam', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function myteamCatalogCalculatePagination() {
  	try{
    $this->qry_search_count = 0;
    $this->qry_search_count = count($this->full_search);
    if ( $this->qry_search_count >0 ) {
      $this->total_pages = ceil($this->qry_search_count/$this->limit);
    } else {
      $this->total_pages = 0;
    }

    if ($this->page > $this->total_pages) $this->page=$this->total_pages;
    $this->start = $this->limit*$this->page - $this->limit;
    if ($this->start < 0) {
      $this->start = 0;
    }
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogCalculatePagination', $ex);
    	expertusErrorThrow($ex);
    }
  }

  
  
  function myteamCatalogSearchResultsQry(){
    //echo $this->userId;
    try{
    $this->select = srchCatalogQuery($this->userId);
    if($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['action'] == 'team')
    {
    	$this->select->leftjoin('slt_enrollment', 'enroll', 'enroll.class_id=ms.pk_id AND enroll.user_id = '.$this->userId.' AND enroll.id =(select max(id) from slt_enrollment e where e.class_id = ms.pk_id and e.user_id = '.$this->userId.')');
    	$this->select->leftjoin('slt_master_enrollment', 'menr', 'menr.program_id=ms.pk_id AND menr.user_id = '.$this->userId.' AND menr.id =(select max(id) from slt_master_enrollment me where me.program_id = ms.pk_id and me.user_id = '.$this->userId.')');
    }   
    $this->select->leftJoin('slt_person','prs','prs.id = '.$this->userId.'');
    $this->select->addField('ms','type', 'object_type');
    $this->select->addField('ms','parent_pk', 'crs_id');
    $this->select->addField('ms','pk_id', 'cls_id');
    $this->select->addField('ms','code', 'cls_code');
    $this->select->addField('ms','name', 'cls_title');
    $this->select->addField('ms','description', 'cls_short_description');
    $this->select->addField('ms','delivery_type', 'delivery_type_code');
    $this->select->addField('cl','currency_type', 'currency_type');
    $this->select->addField('cl','custom4', 'export_compliance');
    $this->select->addField('splt_deliverytype','name', 'delivery_type_name');
    $this->select->addField('splt_status','name', 'status');
    $this->select->addField('splt_language','name', 'language');
    $this->select->addField('ss','name', 'location');
    $this->select->addField('nl','node_id', 'node_id');
    $this->select->addField('cl','price', 'price');
    $this->select->addField('cs','id', 'session_id');
    $this->select->addField('cs','start_date', 'sess_start_date');
    $this->select->addField('cs','start_time', 'sess_start_time');
    $this->select->addField('cs','end_time', 'sess_end_time');    
    $this->select->addField('cs','end_date', 'sess_end_date');
    $this->select->addField('sc','country_name', 'country_name');
    $this->select->addField('cl','lang_code', 'language_code');
    $this->select->addField('sc','country_code', 'country_code');
    $this->select->addField('cl','registration_end_on', 'registration_end_on');
    
    $this->select->addField('prm','expires_in_value', 'expires_in_value');
    $this->select->addField('prm','expires_in_unit', 'expires_in_unit');
    $this->select->addField('prm','price', 'prm_price');
    $this->select->addField('prm','currency_type', 'prm_currency_type');
    $this->select->addField('prm','end_date', 'prm_end_date');
    $this->select->addField('prm','created_on', 'prm_created_on');
    $this->select->addField('group_map','mro', 'mro_id');
   // $this->select->addField('splt_mro','name', 'mro');
    $this->select->addField('cl','is_compliance', 'is_compliance');
	//$this->select->addField('td','tagname', 'tagname');
     
    $this->select->condition('1','1','=');
    $this->select->distinct('te.entity_id');
    
    if($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['action'] == 'team')
    {
    	$this->select->addExpression('(CASE WHEN ms.type = \'Class\' THEN (CASE WHEN enroll.reg_status is null AND ms.type = \'Class\' THEN 1 ELSE (CASE WHEN (enroll.reg_status=\'lrn_crs_reg_can\') THEN 1 ELSE  (CASE WHEN (enroll.reg_status!=\'lrn_crs_reg_can\'
    			AND enroll.comp_status not in (\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',\'lrn_crs_cmp_att\'))
    			THEN 1 ELSE 0 END) END )  END) ELSE (CASE WHEN menr.overall_status is null THEN 1 ELSE
    			(CASE WHEN (menr.overall_status IN (\'lrn_tpm_ovr_cln\',\'lrn_tpm_ovr_inc\')) THEN 1 ELSE 0 END) END) END)','showlist');
    }

    $this->select->where('IF(ms.type=\'Class\',
    ( splt_deliverytype.lang_code=\'cre_sys_lng_eng\' 
    and splt_status.lang_code=\'cre_sys_lng_eng\' 
    and splt_language.lang_code=\'cre_sys_lng_eng\' 
    and nl.entity_type=\'cre_sys_obt_cls\' 
    and ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\') ),1=1 )'
    );
    
    $this->select->where('IF(ms.type!=\'Class\',
    (
    ms.status in (\'lrn_lpn_sts_atv\') AND map.program_id=ms.pk_id),1=1 )'
    );
    $this->select->where('IF(ms.type!=\'Class\',
    		(
    		prm.price is null OR prm.price = 0),cl.price is null OR cl.price = 0 )'
    );
    $this->select->where('cl.dedicated_class_flag = \'N\' or cl.dedicated_class_flag is null');
    $this->select->where ("if(prs.hire_date IS NOT NULL AND prs.hire_date != '' AND crs.complete_days IS NOT NULL AND crs.complete_days != '' AND crs.compliance_completed = 'hire_days', CURDATE() <= (date_format(date_add(prs.hire_date,interval crs.complete_days day),'%Y-%m-%d')),
						 if(crs.complete_date IS NOT NULL AND crs.complete_date != '', CURDATE() <= (date_format(crs.complete_date,'%Y-%m-%d')), 1=1))");
    $this->select->groupBy('cls_id');
    filterByUserAccessPrivileges($this->select, '',$this->userId,'','','Myteam');
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSearchResultsQry', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function myteamCatalogSetSearchResults(){
  	try{
    $this -> myteamCatalogSearchResultsQry();
    $this -> myteamCatalogSetSearchParam(true);
    $this -> myteamCatalogSetSearchFilter();
    $this -> myteamCatalogCalculatePagination();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSetSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  function myteamCatalogSetFullSearchResults(){
  	try{
  		expDebug::dPrint('ashuuuuuu1234 = ');
    $this -> myteamCatalogSearchResultsQry();    
    $this -> myteamCatalogSetSearchParam();
    $this -> myteamCatalogSetSearchFilter();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSetFullSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function myteamCatalogGetSearchResults() {
  	try{
  		expDebug::dPrint('ashuuuuuu12345678 = ');
    $this -> myteamCatalogSetSearchResults();
    return $this -> myteamCatalogGetSearchList();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogGetSearchResults', $ex);
    	expertusErrorThrow($ex);
    }
  }

  function myteamCatalogSearchSortBy($sortby) {
  	try{
    if ($sortby == "AZ") {
      $xorderby = "ms.name";
      $xorderbylist = "asc";
    } else if ($sortby == "ZA") {
      $xorderby = "ms.name";
      $xorderbylist="desc";
    } else if ($sortby == "Time") {
      $xorderby="ms.created_on";
      $xorderbylist="desc";
    } else if ($sortby == "ClassStartDate") {
      $xorderby="ifnull(cs.start_date,'4000-12-30') , cs.start_time";
      $xorderbylist="asc";
    } else {
      $xorderby="ms.name";
      $xorderbylist="desc";
    }
    
    
  	if($_REQUEST['apiname'] == 'CatalogSearchAPI' && $_REQUEST['action'] == 'team')
    {
    	if($_REQUEST['mobile_request']){
    		$this->select->orderBy($xorderby,$xorderbylist);
    	}else{
    		$this->select->orderBy('showlist','desc');
    	}
    	
    }else
    {
    	$this->select->orderBy($xorderby,$xorderbylist);
    }
    
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSearchSortBy', $ex);
    	expertusErrorThrow($ex);
    }
  }

  /**
   * Funtion to return the Search Results.
   *
   * @param string ''
   *
   * @return
   *   Results of Search Data
   */
  function myteamCatalogGetSearchList(){
  	try{
    $this->myteamCatalogSearchSortBy($this->myteamCatalogSetParamVal('sortby'));
    $this->select->range($this->start, $this->limit);
		//print $this->select;
		expDebug::dPrint("Myteam search queryyyy : ".print_r($this->select,true) , 4);
		expDebug::dPrintDBAPI("Myteam search queryyyy enddd ",$this->select);
    return $this->select->execute()->fetchAll();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogGetSearchList', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
  /**
   * Funtion to set the Filter Critria
   *
   * @param string ''
   *
   * @return
   * Set Filter Values
   */
  function myteamCatalogSetSearchFilter(){
  	try{
    $full_Select = $this->select;
    $this->full_search = $full_Select->execute()->fetchAll();
    } catch (Exception $ex) {
    	watchdog_exception('myteamCatalogSetSearchFilter', $ex);
    	expertusErrorThrow($ex);
    }
  }
  
} //Class MyteamCatalogSearch End


/**
 * Funtion to return the list of criteria like Delivery Type etc.
 *
 * @param
 *
 * @return
 *   Results of Delivery types
 */
function getMyteamDeliverTypes(){
	try{
  $select = db_select('slt_profile_list_items','splt_parent');
  $select->join('slt_profile_list_items','splt_child','splt_parent.id = splt_child.parent_id');
  $select->addField('splt_parent','id', 'p_id');
  $select->addField('splt_child','name', 'c_name');
  $select->addField('splt_child','code', 'c_code');
  $select->condition('splt_parent.code', '%' . db_like('lrn_cls_dty') . '%', 'LIKE');
  $select->condition(db_and()->condition('splt_parent.lang_code','cre_sys_lng_eng','='));
  $select->condition(db_and()->condition('splt_child.lang_code','cre_sys_lng_eng','='));
  $select->condition(db_and()->condition('splt_parent.is_active','Y','='));
  $select->condition(db_and()->condition('splt_child.is_active','Y','='));
  $result1 = $select->execute()->fetchAll();
  
  $result2 = getMyteamLearningObjectType();
  return array_merge($result1,$result2);
  } catch (Exception $ex) {
  	watchdog_exception('getMyteamDeliverTypes', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Funtion to return the list of Learning Objects criteria like Cert,curricula and learning plan etc.
 *
 * @param
 *
 * @return
 *   Results of Learning Object Types
 */
function getMyteamLearningObjectType(){
	try{
  $select = db_select('slt_profile_list_items','splt');
  $select->addField('splt','id', 'p_id');
  $select->addField('splt','name', 'c_name');
  $select->addField('splt','code', 'c_code');
  
  $select->condition(db_and()->
      condition(db_or()->
      condition('splt.code', 'cre_sys_obt_crt' , '=')->
      condition('splt.code', 'cre_sys_obt_cur' , '=')->
      condition('splt.code', 'cre_sys_obt_trn' , '=')));
        
  $select->condition(db_and()->condition('splt.lang_code','cre_sys_lng_eng','='));

  $select->condition(db_and()->condition('splt.is_active','Y','='));

  $result = $select->execute()->fetchAll();
  return $result;
  } catch (Exception $ex) {
  	watchdog_exception('getMyteamLearningObjectType', $ex);
  	expertusErrorThrow($ex);
  }
}

function myteamClassRegisteredOrNot($courseid,$classid,$userid){
	try{
  $userId = $userid;
  $select = db_select('slt_enrollment', 'enroll');
  $select->addExpression('COUNT(id)', 'registered_class');
  $select->condition('enroll.class_id', $classid, '=');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));
  $no_of_registered_class = $select->execute()->fetchField();
  if ($no_of_registered_class >= 1 ) {
    $registeredOption = 1;   // Class Registered
  }else{
    $registeredOption = 0;   // Class Not Registered
  }
  return $registeredOption;
  } catch (Exception $ex) {
  	watchdog_exception('myteamClassRegisteredOrNot', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Get the mandatory value from  enrollment tabel
 */

function myteamFetchEnrollment($courseid,$classid,$userid){
	try{
  $userId = $userid;
  $select = db_select('slt_enrollment', 'enroll');
  $select->condition('enroll.class_id', $classid, '=');
  $select->condition('enroll.course_id', $courseid, '=');
  $select->condition('enroll.user_id',$userId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));
  
  $select->addField('enroll', 'mandatory', 'mandatory');
  expDebug::dPrintDBAPI("Class Mandatory Query:",$select);
  $mandatory_value = $select->execute()->fetchField();
  expDebug::dPrint("Class Mandatory Value:".$mandatory_value , 3);
  expDebug::dPrint("Courseid,classid,userid Value:".$courseid.','.$classid.','.$userid , 4);
  return  $mandatory_value;  
  } catch (Exception $ex) {
  	watchdog_exception('myteamFetchEnrollment', $ex);
  	expertusErrorThrow($ex);
  }
}


function myteamObjectClassRegisteredOrNot($objectid,$userId){
	try{
  $select = db_select('slt_master_enrollment', 'master_enroll');
  $select -> join('slt_enrollment', 'enroll','enroll.master_enrollment_id=master_enroll.id');  
  $select->addExpression('COUNT(enroll.id)', 'registered_class');
  $select->condition('master_enroll.program_id', $objectid, '=');
  $select->condition('master_enroll.user_id',$userId,'=');
  $select->condition(db_and()->condition('enroll.reg_status',array('lrn_crs_reg_cnf','lrn_crs_reg_ppm'), 'IN'));
  $no_of_registered_class = $select->execute()->fetchField();
  if ($no_of_registered_class >= 1 ) {
    $registeredOption = 1;   // Class Registereds
  }else{
    $registeredOption = 0;   // Class Not Registered
  }
  return $registeredOption;
  } catch (Exception $ex) {
  	watchdog_exception('myteamObjectClassRegisteredOrNot', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Get the mandatory value from msater enrollment tabel
 */

function myteamFetchProgram($objectid,$userId){
	try{
 	 $select = db_select('slt_master_enrollment', 'master_enroll');
 	 $select->condition('master_enroll.program_id', $objectid, '=');
 	 $select->condition('master_enroll.user_id',$userId,'=');
 	 $select->condition(db_and()->condition('master_enroll.overall_status',array('lrn_tpm_ovr_enr','lrn_tpm_ovr_cmp'), 'IN'));
   $select->addField('master_enroll', 'mandatory', 'mandatory');
   
   $Master_Mandatory= $select->execute()->fetchField();
   expDebug::dPrint("Learning Plan Register - slp_program_enrollment_ins  ==>".$Master_Mandatory , 3);
   return $Master_Mandatory;
  } catch (Exception $ex) {
  	watchdog_exception('myteamFetchProgram', $ex);
  	expertusErrorThrow($ex);
  }
}//array('lrn_tpm_ovr_enr','lrn_tpm_ovr_cmp')

/**
 * Assign course-class to user.
 */

function assignUserToCourseClassAPI($paramsobj,$userid){
	
	include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
	$paramsobj=array_to_object($paramsobj);
	
	if($_REQUEST['Action'] == ""  && $_REQUEST['mobile_request'] && $_REQUEST['Register'] != ''){
	
		$equivalanceclass = checkEquivalence($paramsobj->user_id, $paramsobj->crs_id, $paramsobj->cls_id,$adminEnrollCall=false, $fromCart = false);
	
		expDebug::dPrint(' $equivalanceclass1234 = ' . print_r($equivalanceclass,true) , 4 );
	
		$equi_class = $equivalanceclass[0]->equv_title;
	
		expDebug::dPrint(' $equiclass1234 = ' . print_r($equi_class,true) , 4 );
	
	if(count($equivalanceclass)>0){
			if($class_cnt == '1'){
			$result->msgType = "equivalence";
			$result->msgString = $equi_class;
			return $result;
			}else{
				$result->msgType ="multiple_class";
				$result->msgString = 'multiple';
				return $result;
				
			}
	
		}else{
	
			$result->msgType = "No";
			return $result;
		}
	}
	
	$crsId = $paramsobj->crs_id;
	
	if($crsId){
		$hiredays_details=db_query("SELECT per.id AS id
				FROM
				slt_course_template crs
				INNER JOIN slt_person per
				WHERE  (crs.id = $crsId) AND (per.id = $paramsobj->user_id) AND (per.hire_date IS NOT NULL ) AND (crs.compliance_completed = 'hire_days') AND ((CURDATE() >= date_format(date_add(per.hire_date, interval crs.complete_days+1 day),'%Y-%m-%d'))) ");
		$hiredaysDetails = $hiredays_details->fetchAll();
		if($hiredaysDetails[0]->id == $paramsobj->user_id){
			$result->msgType = 'hire_date';
			
			return $result;
			
		}
	}
	
	
	$result = assignUserToCourseClass($paramsobj->user_id, $paramsobj->crs_id, $paramsobj->cls_id, $paramsobj->mandatory, $paramsobj->waitlist_seats);
	return $result;
}

function assignUserToCourseClass($userId, $courseId, $classId, $mandatory, $waitlist){
	try{
		
		//0060386: Check if user has access privilege to the class when manager tries to assign a class using assignUserToCourseClassAPI
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_core/exp_sp_core.inc');
		
		$select = db_select('slt_course_class','cls');
		$select->addField('cls','id', 'cls_id');
		$select->condition('cls.status','lrn_cls_sts_atv','=');
		$select->condition('cls.id',$classId,'=');
		
		filterByUserAccessPrivileges($select, 'cls',$userId,'','','Class');
		
		expDebug::dPrintDBAPI('narrow search sql query-->>',$select);
		$result = $select->execute()->fetchAll();
		
		if(count($result) < 1)
		{
			$result = array();
			 
			$result[0]->msgType = "privilege";
			$result[0]->msgString = "User does not have privilege for this class";
			 
			return $result;
		}
		
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');

		
  global $user;
  
  $vDrupalUserId      = getSltpersonUserId();
  $vUserName          = $user->name;
  
 	 if($_REQUEST['apiname'])
  	{
	  	$uid = getDrupalIdOfUser($vDrupalUserId);
	  	$selectStmt = db_select('users', 'usr');
	  	$selectStmt->condition('usr.uid',$uid);
	  	$selectStmt->addField('usr', 'name', 'name');
	  	$global_user_values = $selectStmt->execute()->fetchAll();
	  	$vUserName = $global_user_values[0]->name;
 	 }
  
  
  if($vDrupalUserId > 0){
  	
  	$aResp = CreateNewOrder($userId, 'lrn_crs_reg_rsv', $vDrupalUserId);
    $vOrderId = $aResp['OrderId'];
  
    
    $aReq = array('LMSUserId'=>$userId,'OrderId'=>$vOrderId,'DrupalUserId'=>$vDrupalUserId,'UserName'=>$vUserName,
          'CourseId'=>$courseId,'ClassId'=>$classId,'ForceReg'=>$waitlist,'MandatoryMang'=>$mandatory);  
    
    list($aCatalogRegStatus, $enrId) = getCatalogAssignStatus($aReq);
    $cart_msg=$aCatalogRegStatus;
    if($cart_msg =='Waitlist'){
      $param = '{"classid" : "'.$cClassId.'","courseid" : "'.$cCourseId.'","orderid" : "'.$cOrderId.'","nid" : "'.$nodeid.'","userid" : "'.$lmsUserId.'"}';
      //$setCart_msg = '<a id="set_shoppingCart_Waitlist" href=\'javascript:void(0);\' onclick=\'callShoppingCartDetails("LnrShoppingCartDetails","setShoppingCartWaitlist",'.$param.');\'>'.t('Click to Join Waitlist').'</a>';
      $setCart_msg = t('Waitlisted');
	  }
	  else if ($cart_msg =='InstructorConflict'){
      $setCart_msg = t('ERR147');
    }
    else if($cart_msg =='AlreadyEnrolled'){
      $setCart_msg = t('ERR020');
    }
    else if($cart_msg =='Conflict'){
      $setCart_msg = t('ERR021');
    }
    else if($cart_msg =='AlreadyReserved'){
      $setCart_msg = t('ERR057');
    }
    else if($cart_msg =='Invalid'){
      $setCart_msg = t('ERR133');
    }
    else if($cart_msg =='Full'){
      $setCart_msg = t('ERR022');
    }
    else if($cart_msg =='InvalidUser'){
      $setCart_msg = t('Invalid user.');
    }
    else if($cart_msg =='Reserved'){
      $multiRegAllowed = is_multiregistration_allowed($classId);
      if($multiRegAllowed == 0) {
        $setCart_msg = t('Assigned'); //Your registration was successful
      } else {
        $setCart_msg = t('LBL3005').'<br /><div style="text-align: left; font-size: .8em; padding-left: 15px;">'. t('MSG745').'</div>';
      }
       //#2421 - Class notification assignment implemented
       
        $vLMSUserId         = getSltpersonUserId();
        $notificationType 	= "classAssign";
        $objectId1 			= $courseId;
        $objectId2 			= $classId;
        $userId 			= $userId;
        $updateUserId 		= $vLMSUserId;
        
        $deliveryType = getClassDeliveryType($classId, 'Short');
        expDebug::dPrint('$deliveryType = '.$deliveryType, 4);
        $iCal = null;
        if (!empty($enrId) && ($deliveryType == 'ilt' || $deliveryType == 'vc')) {
          $iCal = getPublishICal($enrId, 'Class');
        }
        insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId, $iCal);
        add_audit_trail_entry($updateUserId, $classId, 'cre_sys_obt_cls', 'exp_sp_myteam', 'Assign Learning', 'Enroll Learner to Class', $userId);
        syncSolrData('Class');
    }
    else if($cart_msg =='CourseConflict'){
      $setCart_msg = t('MSG026');
    }else if($cart_msg =='PastRegistration'){
      $setCart_msg = t('ERR053'); //Registration is closed.
    }else if($cart_msg =='Waitlisted') {
	    $setCart_msg = t('Waitlisted'); //waitListCheck($courseId, $classId);
	}else if($cart_msg =='CompletionDateConflict') {
	    $setCart_msg = t('MSG863').' '.t('MSG868'); //waitListCheck($courseId, $classId);
	}
  }
  if($_REQUEST['apiname'])
  {
  	$result = array();
  	
  	$result[0]->msgType = $cart_msg;
  	$result[0]->msgString = $setCart_msg;
  	
  	return $result;
  }
  
  if($vDrupalUserId)
    print $setCart_msg;
  else
    print t('MSG027');
  } catch (Exception $ex) {
  	watchdog_exception('assignUserToCourseClass', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * Register user to Learning Plan.
 */
/*
function assingUserToObjectClass($userId, $objectId, $masterMandatory,$type){
  $vLMSUserId = getSltpersonUserId();
  if(!$vLMSUserId) {    
    $program_msg = t('MSG027');    
  }else{
      if($type != 'forcereg'){
		      $entity_status = array();
		      $result_course=db_query("SELECT b.id AS classid FROM slt_module_crs_mapping a,slt_course_class b 
		      WHERE a.program_id = '".$objectId."' and a.course_id = b.course_id and b.status = 'lrn_cls_sts_atv'");
		      
		      $maxCount = getCourseClassCountForLP($objectId,$userId);
		          if($maxCount[0]->cnt == 1){ 
		                foreach($result_course as $row_class){
		                	  $getConflictCheck = enrollmentsConflictCheck($userId,$row_class->classid,'Class'); 
		                	  if($getConflictCheck > 0) {
		                      $entity_status[] = $row_class->classid;
		                	  }
		                }
		                if(count($entity_status) > 0) {      
		                  $program_msg = "User has a conflicting class assignment.";      
		                }else{ 
		           
		                  $classIds =  $maxCount[0]->classids;
		                  $dao = new AbstractDAO();
		                  $qry = "CALL slp_program_enrollment_ins($userId,$objectId,'".$classIds."','No',$vLMSUserId,@outval1,@outval2,@outval3,@outval4,@outval5,@outval6)";
		                  $dao->connect();
		                  expDebug::dPrint("Learning Plan Register - slp_program_enrollment_ins  ==>".$qry);
		                  $dao->execute($qry);  
		                  $qry = "SELECT @outval1 AS status,@outval2 AS mastrEnrId";
		                  $res = $dao->query($qry);
		                  $result=$dao->fetchResult();
		                  $dao->closeconnect();      
		                  $program_msg = $result->status;
		                  $mastrEnrId = $result->mastrEnrId;
		                 
		                  $updateStmt = db_update('slt_master_enrollment');
		
		                  // Set conditions
		                  $updateStmt->condition('user_id', $userId, '=');
		                  $updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_cmp'),'IN');
		                  $updateStmt->condition('id', $mastrEnrId,'=');
		                  
		                  if($masterMandatory=='N'){
		                    $masterMandatory= null;
		                  }
		                  // Set new values of fields
		                  $updateStmt->fields(array('mandatory' => $masterMandatory));
		                  $mandatory_updated = $updateStmt->execute();
		                             
		                  expDebug::dPrint('exp_sp_myteam-catalog-search.inc : Mastermandatory()  : $orderId = ' . print_r($mandatory_updated, true));
		                  
		                  //#2421 - Cert/Curr/lp notification assignment implemented
		                  $notificationType 	= "certCurrLPAssigned";
		                  $objectId1 			= $mastrEnrId;
		                  $objectId2 			= $objectId;
		                  $updateUserId 		= $vLMSUserId;
		                  insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId);
		                }
		        }else{
		                $program_msg = "MultiRegister";
		        }   
      }else{
      		$listValues = json_decode($_POST['selectedItem']);
    		$cnt=0;    		
    		$classId = array();
			$courseId = array();
	
			foreach($listValues as $x){		
				$tpId = $listValues->$cnt->tpid;
				$courseId[] = $listValues->$cnt->courseid;
	    		$classId[] = $listValues->$cnt->classid;
				$cnt++;
			}
	
			$courseIdList = implode(',',$courseId);
			$classIdList = implode(',',$classId);
			
	      	$entity_id = array();
	  	  	$listIds = explode(',',$classIdList);
	
	  	  	for($i=0;$i<count($listIds);$i++){
	      	
	          $getConflictCheck = enrollmentsConflictCheck($userId,$listIds[$i],'Class'); 
	
	      	  if($getConflictCheck > 0) {
	            $entity_id[] = $listIds[$i];
	      	  }	      	
	      	}	
	      	if(count($entity_id) > 0) {
	      		$program_msg = "User has a conflicting class assignment.";  
	      	}else{
	      		  $dao = new AbstractDAO();
                  $qry = "CALL slp_program_enrollment_ins($userId,$objectId,'".$classIdList."','No',$vLMSUserId,@outval1,@outval2,@outval3,@outval4,@outval5,@outval6)";
                  $dao->connect();
                  expDebug::dPrint("Learning Plan Register - slp_program_enrollment_ins  ==>".$qry);
                  $dao->execute($qry);  
                  $qry = "SELECT @outval1 AS status,@outval2 AS mastrEnrId";
                  $res = $dao->query($qry);
                  $result=$dao->fetchResult();
                  $dao->closeconnect();      
                  $program_msg = $result->status;
                  $mastrEnrId = $result->mastrEnrId;
                 
                  $updateStmt = db_update('slt_master_enrollment');

                  // Set conditions
                  $updateStmt->condition('user_id', $userId, '=');
                  $updateStmt->condition('overall_status', array('lrn_tpm_ovr_enr','lrn_tpm_ovr_cmp'),'IN');
                  $updateStmt->condition('id', $mastrEnrId,'=');
                  
                  if($masterMandatory=='N'){
                    $masterMandatory= null;
                  }
                  // Set new values of fields
                  $updateStmt->fields(array('mandatory' => $masterMandatory));
                  $mandatory_updated = $updateStmt->execute();
                             
                  expDebug::dPrint('exp_sp_myteam-catalog-search.inc : Mastermandatory()  : $orderId = ' . print_r($mandatory_updated, true));
                  
                  //#2421 - Cert/Curr/lp notification assignment implemented
                  $notificationType 	= "certCurrLPAssigned";
                  $objectId1 			= $mastrEnrId;
                  $objectId2 			= $objectId;
                  $updateUserId 		= $vLMSUserId;
                  insertNotification($notificationType, $objectId1, $objectId2, $userId, $updateUserId);
	      	}
      }  
  }  
  print $program_msg;
}

*/
function getCatalogAssignStatus($pmReq){
  expDebug::dPrint(' In getCatalogRegistrationStatus() with $pmReq = ' . print_r($pmReq, true) , 4);
  try {
    $dao = new AbstractDAO();
    $vDrupalUserId  = $pmReq['DrupalUserId'];
    $vLMSUserId     = $pmReq['LMSUserId'];
     
    $vCourseId  = $pmReq['CourseId'];
    $vClassId   = $pmReq['ClassId'];
    $vUserName  = $pmReq['UserName'];
    $vOrderId   = $pmReq['OrderId'];
    $MandatoryMang  = $pmReq['MandatoryMang'];
     
    $vShoppingCartFlag='N';
    $vForceWaitingReg=$pmReq['ForceReg'];

    enrollmentStatusChangeUpdate($vLMSUserId, $vCourseId, $vClassId, $vDrupalUserId);
    $vClassNotify = false;
    $enrollmentStatus = enrollUserToClass($vLMSUserId, $vOrderId, $vUserName, $vCourseId, $vClassId, $vForceWaitingReg, $vDrupalUserId, $vShoppingCartFlag, $vClassNotify, $MandatoryMang);
    expDebug::dPrint(' $enrollmentStatus = ' . print_r($enrollmentStatus, true) , 4);
    
    $vRegStatus='';
    if($enrollmentStatus['id'] == '0'  ){
      $vRegStatus='Reserved';
    }else if($enrollmentStatus['id'] =='-1'  ){
      $vRegStatus='AlreadyEnrolled';
    }else if($enrollmentStatus['id'] =='-2'  ){
      $vRegStatus='Conflict';
    }else if($enrollmentStatus['id'] =='-3'  ){
      $vRegStatus ='Waitlist';
    }else if($enrollmentStatus['id'] =='-4'  ){
      $vRegStatus ='InvalidUser';
    }else if($enrollmentStatus['id'] =='-5'  ){
      $vRegStatus ='Full';
    }else if($enrollmentStatus['id'] =='-6'  ){
      $vRegStatus ='Invalid';
    }else if($enrollmentStatus['id'] =='-7'  ){
      $vRegStatus ='AlreadyReserved';
    }else if($enrollmentStatus['id'] =='-8'  ){
      $vRegStatus ='CourseConflict';
    }else if($enrollmentStatus['id'] =='-9'  ){
      $vRegStatus ='PastRegistration';
    }else if($enrollmentStatus['id'] =='-10'  ){
      $vRegStatus ='InstructorConflict';
    }else if($enrollmentStatus['id'] =='-11'  ){
      $vRegStatus ='CompletionDateConflict';
    }else if($enrollmentStatus['id'] =='1'  ){
      $vRegStatus ='Waitlisted';
    }
    //$loginID=$dao->getLoggedInUserId();
    insertOrderItem($vOrderId, $vCourseId, $vClassId, $vLMSUserId, null, '', $vDrupalUserId, '', '', '', '', '');
    return array($vRegStatus, empty($enrollmentStatus['enrollid'])? null : $enrollmentStatus['enrollid']);
  } catch (Exception $e){
    expDebug::dPrint("Error Occured...when get enroll status", 1);
    expDebug::dPrint($e, 1);
    exit();
  }
}
/**
 * Get results for Course-Class title autocomplete.
 *
 * @param string ''
  *
 * @return
 *   Auto Complete Course-Class title Results
 */
function getMyteamCatalogAutoComplete($delType,$userId){
	try{
  /*$title = $_GET['z'];
  $select = db_select('slt_master_search', 'ms');
  $select->leftjoin('slt_course_class', 'cl','ms.pk_id=cl.id AND ms.type=\'Class\'');
  $select->leftjoin('slt_course_class_session', 'cs', 'ms.pk_id=cs.class_id and cs.id=(SELECT id FROM  slt_course_class_session WHERE class_id=cs.class_id ORDER BY start_date LIMIT 1)');
  $select->leftjoin('slt_location', 'ss','cs.location_id= ss.id');
  $select->leftjoin('slt_node_learning_activity', 'nl',	'nl.entity_id =ms.pk_id and nl.entity_type in (\'cre_sys_obt_cls\',\'cre_sys_obt_trn\',\'cre_sys_obt_cur\',\'cre_sys_obt_crt\')');
  $select->leftjoin('slt_country', 'sc', 'sc.country_code= ss.country');
  $select->leftjoin('slt_course_content_mapper', 'content', '  content.id=(SELECT id FROM slt_course_content_mapper cont WHERE ms.pk_id = cont.class_id AND ms.parent_pk = cont.course_id LIMIT 1 )');
  //$select->leftjoin('slt_entity_profile_mapping', 'sepm', 'sepm.entity_id=ms.pk_id AND sepm.entity_type IN (\'27\',\'28\',\'31\',\'32\')');
  $select->leftjoin('slt_program', 'prm', 'prm.id=ms.pk_id AND ms.type IN (\'cre_sys_obt_crt\',\'cre_sys_obt_cur\',\'cre_sys_obt_trn\')');
  $select->addExpression('DISTINCT(ms.name)', 'clstitle');  
  $select->condition(db_and()->
  condition(db_or()->
  condition('ms.name', '%' . db_like($title) . '%', 'LIKE')->
  condition('ms.code', '%' . db_like($title) . '%', 'LIKE')->
  condition('ms.description', '%' . db_like($title) . '%', 'LIKE')));
  $select->condition(db_and()->
  condition('ms.status',array('lrn_cls_sts_atv','lrn_lpn_sts_atv'),'IN'));
  
  //addFilterForLoggedInUserRole($select, 'sepm', $userId);
         
  $startdate = Date("Y-m-d");
  $time = Date("H:i:s");
  $toDay = Date("Y-m-d");
  if($delType=='lrn_cls_dty_ilt'){
    $select->condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_ilt', '=')->
    condition('cs.start_date', $startdate, '>=')->
    where('IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)')
    );
  }else if($delType=='lrn_cls_dty_vcl'){
    $select->condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_vcl', '=')->
    condition('cs.start_date', $startdate, '>=')->
    where('IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)')
    );
  }else if($delType=='lrn_cls_dty_wbt'){ 
    $select->condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_wbt', '=')->
    where('content.content_id IS NOT NULL')
    );
  }else if($delType=='lrn_cls_dty_vod'){ 
    $select->condition(db_and()->
    //condition('cl.delivery_type', 'lrn_cls_dty_wbt', '=')->
    condition('cl.delivery_type', 'lrn_cls_dty_vod', '=')
    );
  }else if($delType=='cre_sys_obt_trn'){ 
    $select->condition(db_and()->
    condition('ms.type', 'cre_sys_obt_trn', '=')
    );
  }else if($delType=='cre_sys_obt_cur'){ 
    $select->condition(db_and()->
    condition('ms.type', 'cre_sys_obt_cur', '=')
    );
  }else if($delType=='cre_sys_obt_crt'){ 
    $select->condition(db_and()->
    condition('ms.type', 'cre_sys_obt_crt', '=')
    );
  }else{
    $select->condition(db_and()->
    condition(db_or()->
    
    condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_ilt', '=')->
    condition('cs.start_date', $startdate, '>=')->
    where('IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)'))->
    
    condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_vcl', '=')->
    condition('cs.start_date', $startdate, '>=')->
    where('IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)'))->
    
    condition(db_and()->
    condition('cl.delivery_type', 'lrn_cls_dty_wbt', '=')->
    where('content.content_id IS NOT NULL'))->
  
    //condition('cl.delivery_type', 'lrn_cls_dty_wbt', '=')->
    condition('cl.delivery_type', 'lrn_cls_dty_vod', '=')->
    
   
    condition('ms.type', array('cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN')
     ));
  }    
  $qry_srch_autocomplete = $select->execute()->fetchAll();
  foreach ($qry_srch_autocomplete as $rec_srch_autocomplete) {
    print $rec_srch_autocomplete->clstitle."\n";
  }*/

    $title = $_GET['z'];
    $srcSource = getConfigValue('solr_search');
    //solr off----
    if($srcSource == 0){
    	$select = srchCatalogQuery($userId);
	    $select->leftJoin('slt_person','prs','prs.id = '.$userId.'');
    	$select->addExpression('DISTINCT(ms.name)', 'clstitle');  
    
	    $select->condition(db_and()->
    	condition(db_or()->
	    condition('ms.name', '%' . db_like($title) . '%', 'LIKE')->
    	condition('ms.code', '%' . db_like($title) . '%', 'LIKE')->
	    condition('ms.description', '%' . db_like($title) . '%', 'LIKE')->
    	condition('td.tagname', '%' . db_like($title) . '%', 'LIKE')
		));
  
	    if(!empty($_SESSION['availableFunctionalities']->exp_sp_learning_plan_detail)) {
    		$select->condition('ms.type', array('Class','cre_sys_obt_crt','cre_sys_obt_cur','cre_sys_obt_trn'), 'IN');	
    	}else{
    		$select->condition('ms.type', 'Class', '=');
	    }
    
    $select->where('IF(ms.type=\'Class\',
    ( splt_deliverytype.lang_code=\'cre_sys_lng_eng\' 
    and splt_status.lang_code=\'cre_sys_lng_eng\' 
    and splt_language.lang_code=\'cre_sys_lng_eng\' 
    and nl.entity_type=\'cre_sys_obt_cls\' 
    and ms.status in (\'lrn_cls_sts_atv\',\'lrn_crs_sts_atv\') ),1=1 )'
    );
    
    $select->where('IF(ms.type!=\'Class\',
    (
    ms.status in (\'lrn_lpn_sts_atv\') AND map.program_id=ms.pk_id ),1=1 )' //AND map.program_id=ms.pk_id
    );
    //38403: Autocomplete issue in Assign Learning popup (added by yogaraja added the priced class condition)
    $select->where('IF(ms.type!=\'Class\',
    		(
    		prm.price is null OR prm.price = 0),cl.price is null OR cl.price = 0 )'
    );
    //38403: Autocomplete issue in Assign Learning popup (added by yogaraja added the priced class condition)
    $select->where('cl.dedicated_class_flag = \'N\' or cl.dedicated_class_flag is null');
    // 0075246: Class able to register in catalog page for the following scenario
    $select->where ("if(prs.hire_date IS NOT NULL AND prs.hire_date != '' AND crs.complete_days IS NOT NULL AND crs.complete_days != '' AND crs.compliance_completed = 'hire_days', CURDATE() <= (date_format(date_add(prs.hire_date,interval crs.complete_days day),'%Y-%m-%d')),
						 if(crs.complete_date IS NOT NULL AND crs.complete_date != '', CURDATE() <= (date_format(crs.complete_date,'%Y-%m-%d')), 1=1))");
    //addFilterForLoggedInUserRole($select, 'sepm');
    //38403: Autocomplete issue in Assign Learning popup (added by yogaraja added the priced class condition)
    filterByUserAccessPrivileges($select, '',$userId,'','','Myteam');
    //38403: Autocomplete issue in Assign Learning popup (added by yogaraja added the priced class condition)
         
    $startdate = Date("Y-m-d");
    $time = Date("H:i:s");
    $toDay = Date("Y-m-d");

    
    expDebug::dPrint('Before delivery type check');
    if($delType=='lrn_cls_dty_ilt'){
      expDebug::dPrint('Comes in ILT');
      $select->where('( ms.delivery_type=\'lrn_cls_dty_ilt\' and cs.start_date >= \''.$startdate.'\' and 
        		IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1) )');
    }else if($delType=='lrn_cls_dty_vcl'){
      expDebug::dPrint('Comes in VCL');
      $select->where('( ms.delivery_type=\'lrn_cls_dty_vcl\' and cs.start_date >= \''.$startdate.'\' and 
        		IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1) )');
    }else if($delType=='lrn_cls_dty_wbt'){ 
      expDebug::dPrint('Comes in WBT');
      $select->where('( ms.delivery_type=\'lrn_cls_dty_wbt\' and content.content_id IS NOT NULL )');
    }else if($delType=='lrn_cls_dty_vod'){
      expDebug::dPrint('Comes in VOD'); 
      $select->where('(  ms.delivery_type=\'lrn_cls_dty_vod\' and content.content_id IS NOT NULL )');
    }else if($delType=='cre_sys_obt_trn'){
      expDebug::dPrint('Comes in TRN'); 
      $select->where('(  ms.type=\'cre_sys_obt_trn\' )');
    }else if($delType=='cre_sys_obt_cur'){
      expDebug::dPrint('Comes in CUR'); 
      $select->where('(  ms.type=\'cre_sys_obt_cur\' )');
    }else if($delType=='cre_sys_obt_crt'){
      expDebug::dPrint('Comes in CRT'); 
      $select->where('(  ms.type=\'cre_sys_obt_crt\' )');
    }else{
      expDebug::dPrint('Comes in ILT');
      $select->where('IF(ms.type=\'Class\',
        ( 
        ( ms.delivery_type=\'lrn_cls_dty_ilt\' and cs.start_date >= \''.$startdate.'\' and 
        	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1) ) 
        or 
        ( ms.delivery_type=\'lrn_cls_dty_vcl\' and cs.start_date >= \''.$startdate.'\' and 
        	IF(cs.start_date =\''.$toDay.'\', cs.start_time >= \''.$time.'\',1=1)  ) 
        or 
        ( ms.delivery_type=\'lrn_cls_dty_wbt\' and content.content_id IS NOT NULL ) 
        )
        or
        ( ms.delivery_type=\'lrn_cls_dty_vod\' and content.content_id IS NOT NULL ),
        1=1)');
    }
    $qry_srch_autocomplete = $select->execute()->fetchAll();
    expDebug::dPrint('Heloooooooo');
    
    expDebug::dPrint('auto complete fields'. print_r($qry_srch_autocomplete,true),4);
    //print $select;
    expDebug::dPrintDBAPI('Auto complete query', $select);
    }
    else{
        
        require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_catalog.php';
        try{
            expDebug::dPrint("Solr Search Testing auto complete 1 ",1);
            $solrObj = new CatalogSolrSearch();
            
            expDebug::dPrint("Call solr my autocomplete my team",1);
            $search_results = $solrObj->getSolrSearchResultMyTeamAutoComplete();
            
           
            expDebug::dPrint('Result set from solr main result'.print_r($search_results,true),5);
            $qry_srch_autocomplete = $search_results['records'];
            expDebug::dPrint('Result set from solr'.print_r($qry_srch_autocomplete,true),5);
            $count = $search_results['recCount'];
            expDebug::dPrint('Result set count'.print_r($count,true),5);
         
        }catch(Exception $e){
            expDebug::dPrint("Error in Solr Search -- ".print_r($e->getMessage(),true),1);
        }
        expDebug::dPrint("Solr search result - Final ".print_r($search_results,1),4);
    }
    expDebug::dPrint("Solr search resdsdfdfffffffffffffsult - Final ".print_r($qry_srch_autocomplete,1),4);
    foreach ($qry_srch_autocomplete as $rec_srch_autocomplete) {
        $result = ($rec_srch_autocomplete->clstitle!= '') ? $rec_srch_autocomplete->clstitle : $rec_srch_autocomplete->cls_title;
        print $result."\n";
    }
    } catch (Exception $ex) {
    	watchdog_exception('getMyteamCatalogAutoComplete', $ex);
    	expertusErrorThrow($ex);
    }
    
}

function getDashboardDetailsAPI(){
	try{

		expDebug::dPrint('whatsssssssssssssssssssssss =', 4);
		
		
		if($_REQUEST["action"] == '' || $_REQUEST["action"] == 'chartView')
		{
		$user_id = $_REQUEST["userid"];

		$selectStmt = db_select('slt_person', 'ct');
		$selectStmt->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=ct.id and per_oth_mng.is_direct=\'N\'');
		$selectStmt->addField('ct', 'manager_id', 'manager_id');
		$selectStmt->addExpression('COUNT(ct.id)', 'report_count');

		$db_or_condition = db_or();
		$db_or_condition->condition('ct.manager_id',$user_id);
		$db_or_condition->condition('per_oth_mng.manager_id',$user_id);
		//$db_or_condition->where('FIND_IN_SET('.$user_id.', ct.dotted_mngr_id)');
		$selectStmt->condition($db_or_condition);

		$selectStmt->groupBy('manager_id');
		$reportResult =  $selectStmt->execute()->fetchAll();

		expDebug::dPrint('whats the dashboard output ='.print_r($reportResult,true) , 4);

		//Below query is for getting enrolled courses count
		$select = db_select('slt_enrollment', 'enr');
		$select->leftJoin('slt_person', 'per','enr.user_id = per.id');
		$select->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
		$select->addExpression('if(find_in_set(\'lrn_crs_cmp_cmp\',group_concat(enr.comp_status)) > 0,\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_enr\')','comp_status');
		$select->addExpression('if(find_in_set(\'lrn_crs_cmp_cmp\',group_concat(enr.comp_status)) > 0,1,1=1)','status_count');
		$select->addField('per', 'id', 'user_id');
		$select->addField('per', 'full_name', 'full_name');
		$select->addField('enr', 'course_id', 'crs_id');
		$select->condition('enr.reg_status', 'lrn_crs_reg_cnf');

		$db_or = db_or();
		$db_or->condition('per.manager_id',$user_id);
		$db_or->condition('per_oth_mng.manager_id',$user_id);
		//$db_or->where('FIND_IN_SET('.$user_id.', per.dotted_mngr_id)');
		$select->condition($db_or);

		$or = db_or();
		$or->condition('enr.is_compliance', '1');
		$or->condition('enr.mandatory', 'Y');
		$select->condition($or);

		$select->isNull('enr.master_enrollment_id');
		
		$select->groupBy('per.id');
		$select->groupBy('enr.course_id');
	
		expDebug::dPrintDBAPI("select overall compliance query :",$select);
		
		$complianceResult =  $select->execute()->fetchAll();

		expDebug::dPrint('Enrollment output ='.print_r($complianceResult,true) , 4);
		
		//Below query is for getting details of not enrolled courses as well as enrolled courses count
		$selectCompliance1 = db_select('slt_course_template', 'crs');
		$selectCompliance1->join('slt_person', 'per');
		$selectCompliance1->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
		$selectCompliance1->leftJoin('slt_course_class','cls','(cls.course_id=crs.id AND (cls.status = \'lrn_cls_sts_atv\'))');
		$selectCompliance1->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
		$selectCompliance1->leftJoin('slt_group_mapping', 'group_map','(group_map.entity_type = \'cre_sys_obt_crs\' AND group_map.entity_id=crs.id) OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\') AND group_map.group_type = 0');
		$selectCompliance1->leftJoin('slt_groups', 'grp',' grp.id=group_map.group_id and grp.is_admin =0 and grp.status = \'cre_sec_sts_atv\'');
		$selectCompliance1->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$selectCompliance1->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
		$selectCompliance1->addField('per', 'id', 'user_id');
		$selectCompliance1->addField('per', 'full_name', 'full_name');
		$selectCompliance1->addExpression('GROUP_CONCAT(distinct(crs.id))', 'crs_id');
		//$selectCompliance->fields('crs', array('id', 'id'));
		//$selectCompliance->addField('crs', 'id', 'crs_id');
		$or = db_or();
		$or->condition('crs.is_compliance', '1');
		//$or->condition('group_map.mro', 'cre_sys_inv_man');
		$selectCompliance1->condition($or);

		$db_or = db_or();
		$db_or->condition('per.manager_id',$user_id);
		$db_or->condition('per_oth_mng.manager_id',$user_id);
		//$db_or->where('FIND_IN_SET('.$user_id.', per.dotted_mngr_id)');
		$selectCompliance1->condition($db_or);

		$selectCompliance1->condition('crs.status', 'lrn_crs_sts_atv');

		$selectCompliance1->where("((((FIND_IN_SET(per.id,grp.added_users)>0) OR ((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		    AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		    AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		    AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		    AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		    AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
		    AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
		    AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		    AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
		    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager)
		    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_instructor,'N') = grp.is_instructor)
		    ELSE
		    1=1
		    END)
			AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))				
		    AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))))");

		
		expDebug::dPrintDBAPI("select compliance after privilege1234  ", $selectCompliance1);
		$selectCompliance1->groupBy('per.id');
		
		$catalogOutputResult =  $selectCompliance1->execute()->fetchAll();
		
		expDebug::dPrint('whats the dashboard output output result1234 ='.print_r($catalogOutputResult,true) , 4);

		$selectCompliance = db_select('slt_course_template', 'crs');
		$selectCompliance->join('slt_person', 'per');
		$selectCompliance->leftJoin('slt_person_other_manager', 'per_oth_mng', 'per_oth_mng.user_id=per.id and per_oth_mng.is_direct=\'N\'');
		$selectCompliance->leftJoin('slt_course_class','cls','(cls.course_id=crs.id AND (cls.status = \'lrn_cls_sts_atv\'))');
		$selectCompliance->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
		$selectCompliance->leftJoin('slt_group_mapping', 'group_map','(group_map.entity_type = \'cre_sys_obt_crs\' AND group_map.entity_id=crs.id) OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\') AND group_map.group_type = 0');
		$selectCompliance->leftJoin('slt_groups', 'grp',' grp.id=group_map.group_id and grp.is_admin =0 and grp.status = \'cre_sec_sts_atv\'');
		$selectCompliance->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
		$selectCompliance->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
		$selectCompliance->addField('per', 'id', 'user_id');
		$selectCompliance->addField('per', 'full_name', 'full_name');
		$selectCompliance->addExpression('GROUP_CONCAT(distinct(crs.id))', 'crs_id');
		//$selectCompliance->fields('crs', array('id', 'id'));
		//$selectCompliance->addField('crs', 'id', 'crs_id');
		$or = db_or();
		//$or->condition('crs.is_compliance', '1');
		$or->condition('group_map.mro', 'cre_sys_inv_man');
		$selectCompliance->condition($or);

		$db_or = db_or();
		$db_or->condition('per.manager_id',$user_id);
		$db_or->condition('per_oth_mng.manager_id',$user_id);
		//$db_or->where('FIND_IN_SET('.$user_id.', per.dotted_mngr_id)');
		$selectCompliance->condition($db_or);

		$selectCompliance->condition('crs.status', 'lrn_crs_sts_atv');

		$selectCompliance->where("((((FIND_IN_SET(per.id,grp.added_users)>0) OR ((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		    AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		    AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		    AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		    AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		    AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
		    AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
		    AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		    AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
		    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager)
		    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_instructor,'N') = grp.is_instructor)
		    ELSE
		    1=1
		    END)
			AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))	
		    AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))))");

		expDebug::dPrintDBAPI("select compliance after privilege  ", $selectCompliance);
		$selectCompliance->groupBy('per.id');
		
		$catalogOutputResult1 =  $selectCompliance->execute()->fetchAll();
		
		expDebug::dPrint('whats the dashboard output output result1234 ='.print_r($catalogOutputResult1,true) , 4);
		
		if(count($catalogOutputResult) > 0){
		
		for($i=0;$i<count($catalogOutputResult);$i++)
		{
		$catalogOutputResult[$i]->crs_id .= ','.$catalogOutputResult1[$i]->crs_id;
		
		
		}
		}else{
			
			$catalogOutputResult = $catalogOutputResult1;
			
		}
		
		expDebug::dPrint('whats the dashboard output output result ='.print_r($catalogOutputResult,true) , 4);
		
		
		
		
		
		$overAllResult = array();
		$overAllResult = $reportResult;
		for($i=0;$i<count($catalogOutputResult);$i++)
		{				
			$insertValue = '';
			$courseArray = array_filter(explode(",",$catalogOutputResult[$i]->crs_id));
			$overallCount = count($courseArray);
				
			
			$enrolledCount = 0 ;
			$completedCount = 0 ;
			for($j=0;$j<count($complianceResult);$j++)
			{
				if($catalogOutputResult[$i]->user_id == $complianceResult[$j]->user_id)
				{
					if($complianceResult[$j]->comp_status == 'lrn_crs_cmp_cmp')
					{
						$completedCount = $completedCount + 1;
					}else {
						$enrolledCount = $enrolledCount+ 1;
					}
					
					if (in_array($complianceResult[$j]->crs_id, $courseArray)) {
						//Do nothing
					}
					else
					{
						$overallCount++;
					}
				}
			}
			
			
			$insertValue->user_id =  $catalogOutputResult[$i]->user_id;
			$insertValue->full_name =  $catalogOutputResult[$i]->full_name;
			$insertValue->enrolled_count =  $enrolledCount;
			$insertValue->completed_count =  $completedCount;
			$insertValue->overall_count =  $overallCount;

			array_push($overAllResult,$insertValue);
		}
		
		expDebug::dPrint('whats the dashboard output ='.print_r($overAllResult,true) , 4);
		return $overAllResult;
		}
		else
		{
			$user_id = $_REQUEST["subordinate_id"];
			
			$crs_status = $_REQUEST["crs_status"];
			
			if(!empty($crs_status))
			{
			$select = db_select('slt_course_template', 'crs');
			$select->leftJoin('slt_enrollment', 'enr','enr.course_id = crs.id');
			$select->leftJoin('slt_person', 'per','enr.user_id = per.id');
			$select->addExpression('if(find_in_set(\'lrn_crs_cmp_cmp\',group_concat(enr.comp_status)) > 0,\'lrn_crs_cmp_cmp\',\'lrn_crs_cmp_enr\')','comp_status');
			$select->addField('per', 'id', 'user_id');
			$select->addField('per', 'full_name', 'full_name');
			$select->addField('per', 'email', 'email_id');
			$select->addField('crs', 'title', 'crs_title');
			$select->addField('crs', 'code', 'crs_code');
			$select->addField('crs', 'id', 'crs_id');
			$select->addField('crs', 'short_description', 'crs_short_description');
			$select->condition('per.id', $user_id);
			//$select->condition('crs.status', 'lrn_crs_sts_atv');
			$or = db_or();
			$or->condition('enr.is_compliance', '1');
			$or->condition('enr.mandatory', 'Y');
			$select->condition($or);
			$select->isNull('enr.master_enrollment_id');
			$select->condition('enr.reg_status', 'lrn_crs_reg_cnf');
			$select->groupBy('per.id');
			$select->groupBy('enr.course_id');			
			$selectFinal = db_select($select,'acc');
			$selectFinal->addField('acc', 'user_id', 'user_id');
			$selectFinal->addField('acc', 'full_name', 'full_name');
			$selectFinal->addField('acc', 'email_id', 'email_id');
			$selectFinal->addField('acc', 'crs_title', 'crs_title');
			$selectFinal->addField('acc', 'crs_code', 'crs_code');
			$selectFinal->addField('acc', 'crs_id', 'crs_id');
			$selectFinal->addField('acc', 'crs_short_description', 'crs_short_description');
			$selectFinal->where("acc.comp_status = '$crs_status'");
			expDebug::dPrintDBAPI("select overall compliance query innnnn :",$selectFinal);
				
			$complianceResult =  $selectFinal->execute()->fetchAll();
			}
			else
			{
				$selectCompliance = db_select('slt_course_template', 'crs');
				$selectCompliance->join('slt_person', 'per');
				$selectCompliance->leftJoin('slt_course_class','cls','(cls.course_id=crs.id AND (cls.status = \'lrn_cls_sts_atv\'))');
				$selectCompliance->leftJoin('slt_profile_list_items','splt_language','splt_language.code=cls.lang_code');
				$selectCompliance->leftJoin('slt_group_mapping', 'group_map','(group_map.entity_type = \'cre_sys_obt_crs\' AND group_map.entity_id=crs.id) OR (group_map.entity_id=cls.id AND group_map.entity_type = \'cre_sys_obt_cls\') AND group_map.group_type = 0');
				$selectCompliance->leftJoin('slt_groups', 'grp',' grp.id=group_map.group_id and grp.is_admin =0 and grp.status = \'cre_sec_sts_atv\'');
				$selectCompliance->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
				$selectCompliance->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id ');
				$selectCompliance->addField('per', 'id', 'user_id');
				$selectCompliance->addField('crs', 'id', 'crs_id');
				$selectCompliance->addField('per', 'full_name', 'full_name');
				$selectCompliance->addField('per', 'email', 'email_id');
				$selectCompliance->addField('crs', 'title', 'crs_title');
				$selectCompliance->addField('crs', 'code', 'crs_code');
				$selectCompliance->addField('crs', 'short_description', 'crs_short_description');
				$or = db_or();
				$or->condition('crs.is_compliance', '1');
				$or->condition('group_map.mro', 'cre_sys_inv_man');
				$selectCompliance->condition($or);
				$selectCompliance->condition('per.id',$user_id);
				$selectCompliance->condition('crs.status', 'lrn_crs_sts_atv');
								
				$selectCompliance->where("((((FIND_IN_SET(per.id,grp.added_users)>0) OR ((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		    AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		    AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		    AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		    AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		    AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
		    AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)))
		    AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
		    AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
		    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
		    THEN
		    (ifnull(per.is_manager,'N') = grp.is_manager)
		    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
		    THEN
		    (ifnull(per.is_instructor,'N') = grp.is_instructor)
		    ELSE
		    1=1
		    END)
			AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))			
		    AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))))");
				
				$selectCompliance->groupBy('per.id');
				$selectCompliance->groupBy('crs_id');
				
				$selectFinal = db_select($selectCompliance,'acc');
				$selectFinal->addField('acc', 'crs_id', 'crs_id');
				$selectFinal->addField('acc', 'full_name', 'full_name');
				$selectFinal->addField('acc', 'email_id', 'email_id');
				$selectFinal->addField('acc', 'crs_title', 'crs_title');
				$selectFinal->addField('acc', 'crs_code', 'crs_code');
				$selectFinal->addField('acc', 'crs_short_description', 'crs_short_description');
				$selectFinal->addField('acc', 'user_id', 'user_id');
				$selectFinal->where("acc.crs_id not in (select enr.course_id as id from slt_enrollment enr
						where enr.user_id = '$user_id' and (enr.is_compliance = 1 or enr.mandatory = 'Y') and master_enrollment_id is null and enr.reg_status = 'lrn_crs_reg_cnf'
						group by enr.course_id )");
				$selectFinal->groupBy('acc.user_id');
				$selectFinal->groupBy('acc.crs_id');
				expDebug::dPrintDBAPI("select overall compliance query :",$selectFinal);
				
				$complianceResult =  $selectFinal->execute()->fetchAll();
				
			}
						
			expDebug::dPrint('whats the dashboard output ='.print_r($complianceResult,true) , 4);	

			return $complianceResult;
			
		}

	}catch (Exception $ex) {
		watchdog_exception('getDashboardDetailsAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function getAnalyticsReportDataAPI($userid,$startDate='',$endDate='',$compliance = '',$action = '',$cls_status = ''){
	try{
		$query2 ="SELECT DISTINCT grp.name AS name, grp.id AS id
FROM
slt_groups grp
INNER JOIN slt_person per
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
				    AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 0)  UNION ALL SELECT grp.name AS name, grp.id AS id
	FROM
	slt_groups grp
	INNER JOIN slt_person per
	WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1)
	ORDER BY name ASC";
		
		
		//expDebug::dPrintDBAPI("testtttttttqueryyyyyy1232123:",$abc);
		
		$checkStatus = db_query($query2)->fetchAll();

		expDebug::dPrint('whatsthedashboardoutput1234 ='.print_r(($checkStatus),true) , 4);
		
		$check_group = count($checkStatus);
		if($action == 'count')
		{
				if ($_REQUEST ['type'] == 'tp') {
				
				$comp_sts = array(
						'lrn_tpm_ovr_rsc',
						'lrn_tpm_ovr_cln',
						'lrn_tpm_ovr_ppm',
						'lrn_tpm_ovr_exp',
						'lrn_tpm_ovr_rsv'
				);
				
				$overallEnrollments = db_select ( 'slt_master_enrollment', 'enroll' );
				$overallEnrollments->addExpression ( 'COUNT(id)', 'overall_count' );
				$overallEnrollments->condition ( 'enroll.user_id', $userid );
				$overallEnrollments->condition ( 'enroll.overall_status',$comp_sts ,'NOT IN' );
				$overallEnrollments->where ( 'enroll.id = (select max(id) from slt_master_enrollment where user_id=' . $userid . ' and program_id=enroll.program_id)' );
				if ($compliance == 1) {
					$overallEnrollments->condition ( 'enroll.mandatory', '1' );
				}
				expDebug::dPrintDBAPI ( "testttttttt queryyyyyy:", $overallEnrollments );
				$myResultCount = $overallEnrollments->execute ()->fetchAll ();
				expDebug::dPrint ( "Overall enrollments count outputttt: " . print_r ( $myResultCount, true ), 1 );
				
				//USER TP ENROLLMENT
				
				$comp_sts = array(
						'lrn_tpm_ovr_rsc',
						'lrn_tpm_ovr_cln',
						'lrn_tpm_ovr_ppm',
						'lrn_tpm_ovr_rsv'
				);
				
				$select = db_select ( 'slt_master_enrollment', 'enr' );
				$select->addExpression ( 'if(enr.overall_status=\'\',\'\',enr.overall_status)', 'my_comp_status' );
				$select->addExpression ( 'COUNT(overall_status)', 'my_comp_status_count' );
				$select->condition ( 'enr.user_id', $userid );
				$select->condition ( 'enr.overall_status',$comp_sts ,'NOT IN' );
				if ($compliance == 1) {
					$select->condition ( 'enr.mandatory', '1' );
				}
				if (! empty ( $startDate ) && ! empty ( $endDate )) {
					$startDate = str_replace ( '-', '/', $startDate ); // 2014/11/25;
					$endDate = str_replace ( '-', '/', $endDate ); // 2014/11/25;
					$startDate = $startDate . ' 00:00:00';
					$endDate = $endDate . ' 23:59:59';
					$db_or = db_or ();
					$db_or->condition ( 'enr.reg_date', array (
							$startDate,
							$endDate
					), 'BETWEEN' );
					$db_or->condition ( 'enr.comp_date', array (
							$startDate,
							$endDate
					), 'BETWEEN' );
					$select->condition ( $db_or );
				}
				$select->groupBy ( 'my_comp_status' );
				expDebug::dPrintDBAPI ( "USER TP ENROLLMENT", $select );
				$myResult = $select->execute ()->fetchAll ();
				expDebug::dPrint ( "My exnrollments outputttt: " . print_r ( $myResult, true ), 4 );
				
				//USER TP ENROLLMENT
				
				//OTHERS AVERAGE TP ENROLLMENT
				
				
				$query = db_select ( 'slt_master_enrollment', 'e1' );
				$query->innerjoin ( 'slt_master_enrollment', 'e2', 'e1.program_id=e2.program_id ' );
				$query->addField ( 'e2', 'user_id', 'user_id' );
				$query->addExpression ( 'if(e2.overall_status=\'\',\'\',e2.overall_status)', 'comp_status' );
				$query->addExpression ( 'COUNT(1)', 'count' );
				$query->condition ( 'e1.user_id', $userid );
				$query->condition ( 'e2.user_id', $userid, '!=' );
				
				
				//data segmentation for tp-count
				if($check_group > 0){
				
					$query->where("(e2.user_id in (select user_id from (
				
SELECT DISTINCT per.id AS user_id, per.user_name AS uname, pli.name AS status, per.full_name AS fname
FROM
slt_person per
INNER JOIN slt_groups grp
LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
INNER JOIN slt_profile_list_items pli ON per.status=pli.code
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
WHERE  (per.id NOT IN  (1, 2))
AND (pli.name = 'Active')
AND (grp.id in ( select id from (
  
SELECT DISTINCT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM
slt_groups grp
INNER JOIN slt_person per
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
LEFT OUTER JOIN slt_group_attributes grpatt ON grp.id = grpatt.group_id
WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND( (grp.status = 'cre_sec_sts_atv') OR (grp.status = 'cre_sec_sts_itv') )
                    AND (per.id = ".$userid.")
                    AND (grp.is_admin = 0)
                    UNION ALL
SELECT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM
slt_groups grp
INNER JOIN slt_person per
WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1)
ORDER BY name ASC
) ggg))
				
AND (per.status != 'cre_usr_sts_del')
AND ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
         AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
         AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
         AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
         AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
         AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
         AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)))
         AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
         AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
              WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager)
              WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_instructor,'N') = grp.is_instructor)
              ELSE
              1=1 END)
         AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
                 if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
                    if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
         AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))
        OR (FIND_IN_SET(per.id,grp.added_users)>0)
				
        ))))
				
		) sss
        ) )"
							);
				}
				
				
				
				$select->condition ( 'e1.overall_status',$comp_sts ,'NOT IN' );
				$select->condition ( 'e2.overall_status',$comp_sts ,'NOT IN' );
				if ($compliance == 1) {
					$query->condition ( 'e2.mandatory', '1' );
				}
				if (! empty ( $startDate ) && ! empty ( $endDate )) {
					$startDate = str_replace ( '-', '/', $startDate ); // 2014/11/25;
					$endDate = str_replace ( '-', '/', $endDate ); // 2014/11/25;
					$startDate = $startDate . ' 00:00:00';
					$endDate = $endDate . ' 23:59:59';
					$db_or = db_or ();
					$db_or->condition ( 'e2.reg_date', array (
							$startDate,
							$endDate
					), 'BETWEEN' );
					$db_or->condition ( 'e2.comp_date', array (
							$startDate,
							$endDate
					), 'BETWEEN' );
					$query->condition ( $db_or );
				}
				
				$query->groupBy ( 'user_id' );
				$query->groupBy ( 'comp_status' );
				$subQuery = db_select ( $query, 'x' );
				$subQuery->addField ( 'x', 'comp_status', 'avg_comp_status' );
				$subQuery->addExpression ( 'avg(x.count)', 'avg_comp_status_count' );
				$subQuery->groupBy ( 'x.comp_status' );
				expDebug::dPrintDBAPI ( "testttttttt queryyyyyy:", $subQuery );
				$result = $subQuery->execute ()->fetchAll ();
				expDebug::dPrint ( "Other exnrollments outputttt: " . print_r ( $result, true ), 4 );
				
				//OTHERS AVERAGE TP ENROLLMENT
				
				
				$overAllResult = array ();
				$defaultValue = array (
						"lrn_tpm_ovr_cmp",
						"lrn_tpm_ovr_enr",
						"lrn_tpm_ovr_inc",
						"lrn_tpm_ovr_exp"
						
				);
				for($i = 0; $i < count ( $defaultValue ); $i ++) {
					for($j = 0; $j < count ( $myResult ); $j ++) {
						if ($defaultValue [$i] == $myResult [$j]->my_comp_status) {
							$overAllResult [$i]->my_comp_status = $myResult [$j]->my_comp_status;
							$overAllResult [$i]->my_comp_status_count = $myResult [$j]->my_comp_status_count;
							$overAllResult [$i]->overall_count = $myResultCount [0]->overall_count;
						}
					}
						
					for($k = 0; $k < count ( $result ); $k ++) {
						if ($defaultValue [$i] == $result [$k]->avg_comp_status) {
							$overAllResult [$i]->avg_comp_status = $result [$k]->avg_comp_status;
							$overAllResult [$i]->avg_comp_status_count = $result [$k]->avg_comp_status_count;
							$overAllResult [$i]->overall_count = $myResultCount [0]->overall_count;
						}
					}
				}
				expDebug::dPrint ( "Overalllll exnrollments outputttt: " . print_r ( $overAllResult, true ), 4 );
			} else {
			
		
					
				
			
		$overallEnrollments = db_select('slt_enrollment', 'enroll');
		$overallEnrollments->addExpression('COUNT(reg_status)', 'overall_count');
		$overallEnrollments->condition('enroll.user_id', $userid);
		$overallEnrollments->condition('enroll.reg_status', 'lrn_crs_reg_cnf');
		$overallEnrollments->where('enroll.id = (select max(id) from slt_enrollment where user_id='.$userid.' and class_id=enroll.class_id and course_id=enroll.course_id)');
		if($compliance == 1)
		{
			$or = db_or();
			$or->condition('enroll.is_compliance', '1');
			$or->condition('enroll.mandatory', 'Y');
			$overallEnrollments->condition($or);
		}
		
		$myResultCount = $overallEnrollments->execute()->fetchAll();
		
		expDebug::dPrint("Overall enrollments count outputttt: ".print_r($myResultCount,true) , 4);
		
		$select = db_select('slt_enrollment', 'enr');
		$select->addExpression('if(enr.comp_status=\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',enr.comp_status)','my_comp_status');
		$select->addExpression('COUNT(comp_status)', 'my_comp_status_count');
		$select->condition('enr.user_id', $userid);
		$select->condition('enr.reg_status', 'lrn_crs_reg_cnf');
		$select->where('enr.id = (select max(id) from slt_enrollment where user_id='.$userid.' and class_id=enr.class_id and course_id=enr.course_id)');
		if($compliance == 1)
		{
			$or_query = db_or();
			$or_query->condition('enr.is_compliance', '1');
			$or_query->condition('enr.mandatory', 'Y');
			$select->condition($or_query);
		}
		if(!empty($startDate) && !empty($endDate))
		{
			$startDate = str_replace('-', '/', $startDate); //2014/11/25;
			$endDate = str_replace('-', '/', $endDate); //2014/11/25;
			$startDate = $startDate.' 00:00:00';
			$endDate = $endDate.' 23:59:59';
			$db_or = db_or();
			$db_or->condition('enr.reg_date', array($startDate, $endDate), 'BETWEEN');
			$db_or->condition('enr.comp_date', array($startDate, $endDate), 'BETWEEN');
			$select->condition($db_or);
		}
		$select->groupBy('my_comp_status');
		expDebug::dPrintDBAPI("testttttttt queryyyyyy:",$select);
		$myResult = $select->execute()->fetchAll();
		expDebug::dPrint("My exnrollments outputttt: ".print_r($myResult,true) , 4);

		


		//new query for others enrollment
		$date_filter = '';
		$comp_mand = "";
		$users_in_group = "";
		
		if($compliance == 1)
		{
			$comp_mand = 'AND ((e2.is_compliance = 1) OR (e2.mandatory = "Y") )';
		}
		
		if(!empty($startDate) && !empty($endDate))
		{
			$startDate = str_replace('-', '/', $startDate); //2014/11/25;
			$endDate = str_replace('-', '/', $endDate); //2014/11/25;
			$startDate = $startDate.' 00:00:00';
			$endDate = $endDate.' 23:59:59';
			$date_filter = 'AND( (e2.reg_date BETWEEN "'.$startDate.'" AND "'.$endDate.'") OR (e2.comp_date BETWEEN "'.$startDate.'" AND "'.$endDate.'") )';
		}
		
		//Code added for data segmentation
		if($check_group > 0){
		
			$users_in_group = "AND (e2.user_id in (select user_id from (
					
SELECT DISTINCT per.id AS user_id, per.user_name AS uname, pli.name AS status, per.full_name AS fname
FROM
slt_person per
INNER JOIN slt_groups grp
LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
INNER JOIN slt_profile_list_items pli ON per.status=pli.code
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
WHERE  (per.id NOT IN  (1, 2))
AND (pli.name = 'Active')
AND (grp.id in ( select id from (
                 
SELECT DISTINCT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM 
slt_groups grp
INNER JOIN slt_person per
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
LEFT OUTER JOIN slt_group_attributes grpatt ON grp.id = grpatt.group_id
WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)     
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND( (grp.status = 'cre_sec_sts_atv') OR (grp.status = 'cre_sec_sts_itv') )
                    AND (per.id = ".$userid.") 
                    AND (grp.is_admin = 0)
                    UNION ALL 
SELECT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM 
slt_groups grp
INNER JOIN slt_person per
WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1) 
ORDER BY name ASC
) ggg))

AND (per.status != 'cre_usr_sts_del')
AND ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
         AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
         AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
         AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
         AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
         AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
         AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)))
         AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
         AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
              WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager)
              WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_instructor,'N') = grp.is_instructor)
              ELSE
              1=1 END)
         AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
                 if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
                    if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))			
         AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))
        OR (FIND_IN_SET(per.id,grp.added_users)>0)
        
        ))))

		) sss
        ) )";
		}
		
		
		
		
		
		
		
		$Query = db_query('SELECT final_enr.comp_status as avg_comp_status , Count(*)/Count(DISTINCT final_enr.user_id) as avg_comp_status_count
		FROM
		(
				select e2.id,e2.user_id ,if(e2.comp_status = \'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',e2.comp_status) AS comp_status
				FROM `slt_enrollment` e2
		
				WHERE e2.`class_id` in ( SELECT e1.class_id as class_id  FROM  slt_enrollment e1 WHERE (e1.user_id =   "'.$userid.'")
		
						AND (reg_status = \'lrn_crs_reg_cnf\' ))
				AND (e2.reg_status = \'lrn_crs_reg_cnf\')
				AND e2.master_enrollment_id is null
				AND e2.id = (SELECT MAX(id) FROM slt_enrollment WHERE `class_id` = e2.`class_id` AND user_id = e2.user_id ORDER BY id desc limit 1)
				AND (e2.user_id !=  "'.$userid.'"  )
				'.$date_filter.'
				'.$comp_mand.'
				'.$users_in_group.'
				) final_enr
				GROUP BY  final_enr.comp_status');
		
		
		
		expDebug::dPrintDBAPI("testttttttt queryyyyyy others enrollment:",$Query);
		$result = $Query->fetchAll();
		expDebug::dPrint("Other exnrollments outputttt: ".print_r($result,true) , 4);
		
		
		
		
		
		
		
		
		
		
		/*
		// old query
		$query =  db_select('slt_enrollment', 'e1');
		$query->innerjoin('slt_enrollment', 'e2', 'e1.class_id=e2.class_id AND e1.course_id = e2.course_id');
		$query->addField('e2', 'user_id', 'user_id');
		$query->addExpression('if(e2.comp_status=\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',e2.comp_status)','comp_status');
		$query->addExpression('COUNT(1)', 'count');
		$query->condition('e1.user_id', $userid);
		$query->condition('e2.user_id', $userid , '!=');
		
		
		//Code added for data segmentation
		
		if($check_group > 0){
		
		$query->where(" (e2.user_id in (select user_id from (SELECT DISTINCT per.id AS user_id, per.user_name AS uname, pli.name AS status, per.full_name AS fname
		FROM
		slt_person per
		INNER JOIN slt_groups grp
		INNER JOIN slt_profile_list_items pli ON per.status=pli.code
		LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
		LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
		WHERE  (per.id NOT IN  (1, 2)) AND (grp.id in (select id from ( SELECT DISTINCT grp.name AS name, grp.id AS id
		FROM
		slt_groups grp
		INNER JOIN slt_person per
		LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
		WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
		AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
		AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
		AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
		AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
		AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
		AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
		AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
		
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
		AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,UNIX_TIMESTAMP(grpatt.on_or_after_start_date) <= UNIX_TIMESTAMP(per.hire_date),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, UNIX_TIMESTAMP(grpatt.on_or_before_start_date) >= UNIX_TIMESTAMP(per.hire_date), 1=0) OR
					if(grpatt.between_start_date is not null ,UNIX_TIMESTAMP(per.hire_date) between UNIX_TIMESTAMP(grpatt.between_start_date) AND UNIX_TIMESTAMP(grpatt.between_end_date), 1=0),1=1))
		AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 0)  UNION ALL SELECT grp.name AS name, grp.id AS id
		FROM
		slt_groups grp
		INNER JOIN slt_person per
		LEFT OUTER JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
		WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1)
		ORDER BY name ASC
		) ggg)) AND (per.status != 'cre_usr_sts_del') AND ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
  		AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
  		AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
 		AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
  		AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
  		AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
  		AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)))
 		AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
  		AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
		
                        THEN
                        (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                        WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                        THEN
                        (ifnull(per.is_manager,'N') = grp.is_manager)
                        WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                        THEN
                        (ifnull(per.is_instructor,'N') = grp.is_instructor)
                        ELSE
                        1=1 END)
		AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,UNIX_TIMESTAMP(grpatt.on_or_after_start_date) <= UNIX_TIMESTAMP(per.hire_date),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, UNIX_TIMESTAMP(grpatt.on_or_before_start_date) >= UNIX_TIMESTAMP(per.hire_date), 1=0) OR
					if(grpatt.between_start_date is not null ,UNIX_TIMESTAMP(per.hire_date) between UNIX_TIMESTAMP(grpatt.between_start_date) AND UNIX_TIMESTAMP(grpatt.between_end_date), 1=0),1=1))		
  AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) ) sss
		
) )
		
		
" );
		}
		
		
		$query->condition('e1.reg_status','lrn_crs_reg_cnf');
		$query->condition('e2.reg_status', 'lrn_crs_reg_cnf');
		$query->where('e2.id = (select max(e3.id) from slt_enrollment e3 where e3.class_id=e2.class_id AND e3.course_id = e2.course_id and e3.user_id=e2.user_id)');
		if($compliance == 1)
		{
			$or_query = db_or();
			$or_query->condition('e2.is_compliance', '1');
			$or_query->condition('e2.mandatory', 'Y');
			$query->condition($or_query);
		}
		if(!empty($startDate) && !empty($endDate))
		{
			$startDate = str_replace('-', '/', $startDate); //2014/11/25;
			$endDate = str_replace('-', '/', $endDate); //2014/11/25;
			$startDate = $startDate.' 00:00:00';
			$endDate = $endDate.' 23:59:59';
			$db_or = db_or();
			$db_or->condition('e2.reg_date', array($startDate, $endDate), 'BETWEEN');
			$db_or->condition('e2.comp_date', array($startDate, $endDate), 'BETWEEN');
			$query->condition($db_or);
		}
		$query->groupBy('user_id');
		$query->groupBy('comp_status');
		$subQuery =  db_select($query,'x');
		$subQuery->addField('x', 'comp_status', 'avg_comp_status');
		$subQuery->addExpression('avg(x.count)','avg_comp_status_count');
		$subQuery->groupBy('x.comp_status');
		expDebug::dPrintDBAPI("testttttttt queryyyyyy:",$subQuery);
		$result = $subQuery->execute()->fetchAll();
		expDebug::dPrint("Other exnrollments outputttt: ".print_r($result,true) , 4);
				
		*/




        $overAllResult = array();
		$defaultValue = array("lrn_crs_cmp_cmp", "lrn_crs_cmp_enr", "lrn_crs_cmp_inc");
		for($i=0;$i<count($defaultValue);$i++)
		{
			for($j=0;$j<count($myResult);$j++)
			{
				if($defaultValue[$i] == $myResult[$j]->my_comp_status)
				{
					$overAllResult[$i]->my_comp_status = $myResult[$j]->my_comp_status;
					$overAllResult[$i]->my_comp_status_count = $myResult[$j]->my_comp_status_count;
					$overAllResult[$i]->overall_count = $myResultCount[0]->overall_count;
				}
			}

			for($k=0;$k<count($result);$k++)
			{
				if($defaultValue[$i] == $result[$k]->avg_comp_status)
				{
					$overAllResult[$i]->avg_comp_status = $result[$k]->avg_comp_status;
					$overAllResult[$i]->avg_comp_status_count = $result[$k]->avg_comp_status_count;
					$overAllResult[$i]->overall_count = $myResultCount[0]->overall_count;
				}
			}
		}
		expDebug::dPrint("Overalllll exnrollments outputttt: ".print_r($overAllResult,true) , 4);
		}
		} 
		else {
			
			if($_REQUEST ['type'] == 'tp'){
				


				$query =  db_select('slt_master_enrollment', 'e1');
				$query->innerjoin('slt_master_enrollment', 'e2', 'e1.program_id=e2.program_id');
				$query->innerjoin('slt_program', 'crs', 'crs.id=e2.program_id');
				$query->addField('e2', 'program_id', 'class_id');
				$query->addField('e2', 'program_id', 'course_id');
				$query->addField('e2', 'mandatory', 'mro');
				$query->addField('crs', 'title', 'cls_title');
				$query->addField('crs', 'code', 'cls_code');
				$query->addField('crs', 'short_desc', 'description');
				$query->addExpression('avg(e2.score)','avg_score');
				$query->addField('crs', 'object_type', 'delivery_type_code');
				$query->addExpression('if(e2.overall_status=\'\',\'\',e2.overall_status)','comp_status');
				$query->addExpression('COUNT(1)', 'count');
				$query->condition('e1.user_id', $userid);
				$query->condition('e2.user_id', $userid , '!=');
				
				//Data segmentation for TP - class list
				if($check_group > 0){
				
					$query->where("(e2.user_id in (select user_id from (
				
SELECT DISTINCT per.id AS user_id, per.user_name AS uname, pli.name AS status, per.full_name AS fname
FROM
slt_person per
INNER JOIN slt_groups grp
LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
INNER JOIN slt_profile_list_items pli ON per.status=pli.code
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
WHERE  (per.id NOT IN  (1, 2))
AND (pli.name = 'Active')
AND (grp.id in ( select id from (
   
SELECT DISTINCT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM
slt_groups grp
INNER JOIN slt_person per
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
LEFT OUTER JOIN slt_group_attributes grpatt ON grp.id = grpatt.group_id
WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND( (grp.status = 'cre_sec_sts_atv') OR (grp.status = 'cre_sec_sts_itv') )
                    AND (per.id = ".$userid.")
                    AND (grp.is_admin = 0)
                    UNION ALL
SELECT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM
slt_groups grp
INNER JOIN slt_person per
WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1)
ORDER BY name ASC
) ggg))
				
AND (per.status != 'cre_usr_sts_del')
AND ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
         AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
         AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
         AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
         AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
         AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
         AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)))
         AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
         AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
              WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager)
              WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_instructor,'N') = grp.is_instructor)
              ELSE
              1=1 END)
         AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
                 if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
                    if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))
         AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))
        OR (FIND_IN_SET(per.id,grp.added_users)>0)
				
        ))))
				
		) sss
        ) )"
							);
				}
				
					
				if(strstr($cls_status, '|'))
				{
					$query->condition('e2.overall_status', 'lrn_tpm_ovr_enr');
				}
				else {
					$query->condition('(e2.overall_status', $cls_status);
				}
				if($compliance == 1)
				{
					$query->condition('e2.mandatory', '1');
				}
				if(!empty($startDate) && !empty($endDate))
				{
					$startDate = str_replace('-', '/', $startDate); //2014/11/25;
					$endDate = str_replace('-', '/', $endDate); //2014/11/25;
					$startDate = $startDate.' 00:00:00';
					$endDate = $endDate.' 23:59:59';
					$db_or = db_or();
					$db_or->condition('e2.reg_date', array($startDate, $endDate), 'BETWEEN');
					$db_or->condition('e2.comp_date', array($startDate, $endDate), 'BETWEEN');
					$query->condition($db_or);
				}
					
				$query->groupBy('class_id');
				$query->groupBy('course_id');
				$query->groupBy('comp_status');
					
				expDebug::dPrintDBAPI("testttttttt queryyyyyy full data:",$query);
				$overAllResult = $query->execute()->fetchAll();
					
					
				
				
				
			}else{
			
			
			$query =  db_select('slt_enrollment', 'e1');
			$query->innerjoin('slt_enrollment', 'e2', 'e1.class_id=e2.class_id AND e1.course_id = e2.course_id');
			$query->innerjoin('slt_course_class', 'crs', 'crs.id=e2.class_id AND crs.course_id = e2.course_id');
			$query->addField('e2', 'class_id', 'class_id');
			$query->addField('e2', 'course_id', 'course_id');
			$query->addField('e2', 'is_compliance', 'is_compliance');
			$query->addField('e2', 'mandatory', 'mro');
			$query->addField('crs', 'title', 'cls_title');
			$query->addField('crs', 'code', 'cls_code');
			$query->addField('crs', 'short_description', 'description');
			$query->addExpression('avg(e2.score)','avg_score');
			$query->addField('crs', 'delivery_type', 'delivery_type_code');
			$query->addExpression('if(e2.comp_status=\'lrn_crs_cmp_inp\',\'lrn_crs_cmp_enr\',e2.comp_status)','comp_status');
			$query->addExpression('COUNT(1)', 'count');
			$query->condition('e1.user_id', $userid);
			$query->condition('e2.user_id', $userid , '!=');
			
			
			//Code added for data segmentation
			
			expDebug::dPrint('ashutosh12343456 ='.print_r(($check_group),true) , 4);
			
			if($check_group > 0){
				
				$query->where("(e2.user_id in (select user_id from (
					
SELECT DISTINCT per.id AS user_id, per.user_name AS uname, pli.name AS status, per.full_name AS fname
FROM
slt_person per
INNER JOIN slt_groups grp
LEFT JOIN slt_group_attributes grpatt ON grpatt.group_id = grp.id
INNER JOIN slt_profile_list_items pli ON per.status=pli.code
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
WHERE  (per.id NOT IN  (1, 2))
AND (pli.name = 'Active')
AND (grp.id in ( select id from (
                 
SELECT DISTINCT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM 
slt_groups grp
INNER JOIN slt_person per
LEFT OUTER JOIN slt_person_jobrole_mapping lpjm ON lpjm.user_id = per.id
LEFT OUTER JOIN slt_group_attributes grpatt ON grp.id = grpatt.group_id
WHERE  ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
					AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
					AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
					AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
					AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
					AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
					AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where jobmap.user_id = per.id)>0,grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0))
					AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
			        AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
                                                    THEN
                                                    (ifnull(per.is_manager,'N') = grp.is_manager)
                                                    WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
                                                    THEN
                                                    (ifnull(per.is_instructor,'N') = grp.is_instructor)
                                                    ELSE
                                                    1=1
                                                    END)
					AND (
					if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1)     
					)
					AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0))))) AND( (grp.status = 'cre_sec_sts_atv') OR (grp.status = 'cre_sec_sts_itv') )
                    AND (per.id = ".$userid.") 
                    AND (grp.is_admin = 0)
                    UNION ALL 
SELECT grp.name AS name, grp.id AS id, grp.code AS grp_code
FROM 
slt_groups grp
INNER JOIN slt_person per
WHERE  ((((FIND_IN_SET(per.id,grp.added_users)>0 OR FIND_IN_SET(per.id,grp.userslist)>0) and (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)))) AND (grp.status = 'cre_sec_sts_atv') AND (per.id = ".$userid.") AND (grp.is_admin = 1) 
ORDER BY name ASC
) ggg))

AND (per.status != 'cre_usr_sts_del')
AND ((((((if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0))
         AND (if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 ))
         AND (if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0))
         AND (if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0))
         AND (if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0))
         AND (if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0))
         AND (if(grp.job_role='All',(select count(1) from slt_person_jobrole_mapping as jobmap where per.id= jobmap.user_id)>0,grp.job_role IS NULL OR (FIND_IN_SET(ifnull(lpjm.job_role,''),grp.job_role)>0)))
         AND (if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0))
         AND (CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
              WHEN (grp.is_manager='Y' AND grp.is_instructor='N')
              THEN
              (ifnull(per.is_manager,'N') = grp.is_manager)
              WHEN (grp.is_manager='N' AND grp.is_instructor='Y')
              THEN
              (ifnull(per.is_instructor,'N') = grp.is_instructor)
              ELSE
              1=1 END)
         AND (if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR
                 if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
                    if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))			
         AND (grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))
        OR (FIND_IN_SET(per.id,grp.added_users)>0)
        
        ))))

		) sss
        ) )" 
						);
			}	
			
			
			if(strstr($cls_status, '|'))
			{
				$orQuery = db_or();
				$orQuery->condition('e2.comp_status', 'lrn_crs_cmp_enr');
				$orQuery->condition('e2.comp_status', 'lrn_crs_cmp_inp');
				$query->condition($orQuery);
			}
			else {
				$query->condition('(e2.comp_status', $cls_status);
			}
			$query->condition('e1.reg_status','lrn_crs_reg_cnf');
			$query->condition('e2.reg_status','lrn_crs_reg_cnf');
			$query->where('e2.id = (select max(e3.id) from slt_enrollment e3 where e3.class_id=e2.class_id AND e3.course_id = e2.course_id and e3.user_id=e2.user_id)');
			if($compliance == 1)
			{
				$or_query = db_or();
				$or_query->condition('e2.is_compliance', '1');
				$or_query->condition('e2.mandatory', 'Y');
				$query->condition($or_query);
			}
			if(!empty($startDate) && !empty($endDate))
			{
				$startDate = str_replace('-', '/', $startDate); //2014/11/25;
				$endDate = str_replace('-', '/', $endDate); //2014/11/25;
				$startDate = $startDate.' 00:00:00';
				$endDate = $endDate.' 23:59:59';
				$db_or = db_or();
				$db_or->condition('e2.reg_date', array($startDate, $endDate), 'BETWEEN');
				$db_or->condition('e2.comp_date', array($startDate, $endDate), 'BETWEEN');
				$query->condition($db_or);
			}
			
			$query->groupBy('class_id');
			$query->groupBy('course_id');
			$query->groupBy('comp_status');
			
			expDebug::dPrintDBAPI("testttttttt queryyyyyy full data:",$query);
			$overAllResult = $query->execute()->fetchAll();
			
			
		}
		}
		return $overAllResult;
	} catch (Exception $ex) {
		watchdog_exception('getAnalyticsReportDataAPI', $ex);
		expertusErrorThrow($ex);
	}
}


function getMyteamCatalogResultsAPI()
{
	try{

		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. '/sites/all/modules/core/exp_sp_learning/exp_sp_learning_learner/modules/exp_sp_lnrsearch/exp_sp_lnrsearch.inc');
		
		$id = $_REQUEST["userid"];
		$delivery_type_code = $_REQUEST["delivery_type_code"];
		$isAdminRole = $_REQUEST['admin_role'];
		
		expDebug::dPrint('$isAdminRole = '.$isAdminRole);
		
		
		if ($_REQUEST ['type'] == 'tp') {
			
			if (strstr ( $_REQUEST ['action'], 'count' )) {
				
				$select = db_select ( 'slt_person', 'per' );
				$select->leftJoin ( 'slt_master_enrollment', 'enr', 'enr.user_id = per.id' );
				$select->leftJoin ( 'slt_program', 'cls', 'cls.id = enr.program_id' );
				$select->addField ( 'enr', 'overall_status', 'comp_status' );
				$select->addField ( 'enr', 'overall_status', 'reg_status' );
				$select->addExpression ( 'COUNT(enr.overall_status)', 'count' );
				if ($isAdminRole == "false") {
					$select->condition ( 'per.manager_id', $id );
				}
				$select->condition ( 'enr.overall_status', array (
						'lrn_tpm_ovr_rsc',
						'lrn_tpm_ovr_rsv'
						), 'NOT IN' );
				$select->groupBy ( 'comp_status' );
			} else if (strstr ( $_REQUEST ['action'], 'userList' )) {
				$select = db_select ( 'slt_person', 'per' );
				$select->leftJoin ( 'slt_master_enrollment', 'enr', 'enr.user_id = per.id' );
				$select->leftJoin ( 'slt_program', 'cls', 'cls.id = enr.program_id' );
				$select->leftJoin ( 'slt_profile_list_items', 'pro', 'per.job_title = pro.code' );
				$select->leftJoin ( 'users', 'u', 'per.user_name = u.name' );
				$select->leftJoin ( 'file_managed', 'file', 'u.picture = file.fid' );
				$select->addField ( 'enr', 'overall_status', 'comp_status' );
				$select->addField ( 'enr', 'overall_status', 'reg_status' );
// 				$select->addField ( 'enr', 'is_compliance', 'is_compliance' );
				$select->addField ( 'enr', 'mandatory', 'mandatory' );
				$select->addField ( 'per', 'id', 'subordinate_id' );
				$select->addField ( 'per', 'full_name', 'full_name' );
				$select->addField ( 'per', 'user_name', 'user_name' );
				$select->addField ( 'per', 'phone_no', 'phone_no' );
				$select->addField ( 'per', 'mobile_no', 'mobile_no' );
				$select->addField ( 'per', 'email', 'email' );
				$select->addField ( 'pro', 'name', 'job_title' );
				$select->addField ( 'file', 'uri', 'user_picture' );
				$select->addExpression ( 'COUNT(enr.overall_status)', 'count' );
				$select->addExpression ( 'COUNT(enr.overall_status)', 'reg_count' );
				if ($isAdminRole == "false") {
					$select->condition ( 'per.manager_id', $id );
				}
				$select->condition ( 'enr.overall_status', array (
						'lrn_tpm_ovr_rsv',
						'lrn_tpm_ovr_rsc' 
				), 'NOT IN' );
				$select->groupBy ( 'user_name' );
			} else {
				$select = db_select ( 'slt_person', 'per' );
				$select->leftJoin ( 'slt_master_enrollment', 'enr', 'enr.user_id = per.id' );
				$select->leftJoin ( 'slt_program', 'cls', 'cls.id = enr.program_id' );
				$select->leftJoin ( 'slt_profile_list_items', 'pro', 'per.job_title = pro.code' );
				$select->leftJoin ( 'slt_profile_list_items', 'profile', 'enr.overall_status = profile.code' );
				$select->leftJoin ( 'slt_profile_list_items', 'prof', 'cls.object_type = prof.code' );
				$select->leftJoin ( 'users', 'u', 'per.user_name = u.name' );
				$select->leftJoin ( 'file_managed', 'file', 'u.picture = file.fid' );
				$select->addField ( 'enr', 'program_id', 'class_id' );
				$select->addField ( 'enr', 'mandatory', 'mro_status' );
				$select->addField ( 'enr', 'id', 'enrolled_id' );
				$select->addField ( 'cls', 'title', 'cls_title' );
				if (! $_REQUEST ['export']) {
					$select->addField ( 'cls', 'code', 'cls_code' );
					$select->addField ( 'cls', 'short_desc', 'cls_short_description' );
				}
				$select->addField ( 'cls', 'object_type', 'delivery_type_code' );
				$select->addField ( 'enr', 'overall_status', 'comp_status' );
				$select->addField ( 'enr', 'overall_status', 'reg_status' );
// 				$select->addField ( 'enr', 'is_compliance', 'is_compliance' );
				$select->addField ( 'enr', 'mandatory', 'mandatory' );
				$select->addField ( 'per', 'id', 'subordinate_id' );
				$select->addField ( 'per', 'full_name', 'full_name' );
				$select->addField ( 'per', 'user_name', 'user_name' );
				$select->addField ( 'per', 'phone_no', 'phone_no' );
				$select->addField ( 'per', 'mobile_no', 'mobile_no' );
				$select->addField ( 'per', 'email', 'email' );
				$select->addField ( 'pro', 'name', 'job_title' );
				$select->addField ( 'file', 'uri', 'user_picture' );
				$select->addField ( 'profile', 'name', 'status' );
				$select->addField ( 'prof', 'name', 'delivery_type_name' );
				if ($_REQUEST ['subOrdinateId']) {
					$select->condition ( 'per.id', $_REQUEST ['subOrdinateId'] );
				}
				
				if ($isAdminRole == "false") {
					$select->condition ( 'per.manager_id', $id );
				}
				$select->condition ( 'enr.overall_status', array (
						'lrn_tpm_ovr_rsv',
						'lrn_tpm_ovr_rsc' 
				), 'NOT IN' );
			}
			
			
			if ($_REQUEST ['action'] == 'countCompliance') {
// 				$select->condition ( 'enr.is_compliance', 1 );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
			} else if ($_REQUEST ['action'] == 'countMandatory') {
				$select->condition ( 'enr.mandatory', '1' );
// 				$select->isNull ( 'enr.is_compliance' );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
			} else if ($_REQUEST ['action'] == 'countGeneral') {
				// $select->addExpression('COUNT(enr.comp_status)', 'count');
			} else if ($_REQUEST ['action'] == 'Compliance') {
// 				$select->condition ( 'enr.is_compliance', 1 );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
				
				if (! $_REQUEST ['export']) {
					if ($_REQUEST ['cls_status'] == 'lrn_tpm_ovr_cmp') {
						$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_cmp' );
					} else {
						$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_cmp', '!=' );
					}
				}
			} else if ($_REQUEST ['action'] == 'Mandatory') {
				$select->condition ( 'enr.mandatory', '1' );
				//$select->isNull ( 'enr.is_compliance' );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
				
				if (! $_REQUEST ['export']) {
					$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
				}
			} else if ($_REQUEST ['action'] == 'userListCompliance') {
// 				$select->condition ( 'enr.is_compliance', 1 );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
				
				if (! $_REQUEST ['export']) {
					if ($_REQUEST ['cls_status'] == 'lrn_tpm_ovr_cmp') {
						$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_cmp' );
					} else {
						$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_cmp', '!=' );
					}
				}
			} else if ($_REQUEST ['action'] == 'userListMandatory') {
				$select->condition ( 'enr.mandatory', '1' );
				//0083564
// 				$select->isNull ( 'enr.is_compliance' );
				$select->condition ( 'enr.overall_status', 'lrn_tpm_ovr_can', '!=' );
				
				if (! $_REQUEST ['export']) {
					$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
				}
			} else if ($_REQUEST ['action'] == 'userListGenaral') {
				if (! $_REQUEST ['export']) {
					if ($_REQUEST ['cls_status'] == 'lrn_tpm_ovr_can') {
						$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
					} else {
						$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
					}
				}
			} else {
				if (! $_REQUEST ['export']) {
					if ($_REQUEST ['cls_status'] == 'lrn_tpm_ovr_can') {
						$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
					} else {
						$select->condition ( 'enr.overall_status', $_REQUEST ['cls_status'] );
					}
				}
			}
			
// 			$select->isNull ( 'enr.master_enrollment_id' );
			
			$num_rows = $select->countQuery ()->execute ()->fetchField ();
			
			if ($_REQUEST ['paginate'] || strstr ( $_REQUEST ['action'], 'user' )) {
				$start = 0;
				
				if (! empty ( $_REQUEST ['start'] )) {
					expDebug::dPrint ( "innnnnnnn" );
					$start = $_REQUEST ['start'];
				}
				
				$limit = $_REQUEST ['limit'];
				$start = $start * $limit;
				
				$select->range ( $start, $limit );
			}
			
			expDebug::dPrintDBAPI ( "getMyteamCatalogResultsAPI total records:", $select );
			
			$result = $select->execute ()->fetchAll ();
			
			expDebug::dPrint ( "Myteam search queryyyy outputttttttt: " . print_r ( $result, true ), 4 );
			
			if ($num_rows) {
				$result ['totalrow'] = $num_rows;
			}
			
// 			if ($_REQUEST ['paginate']) {
// 				foreach ( $result as $resClsDetials ) {
// 					$getNodeId = getNodeIdFromEntity ( $resClsDetials->class_id, 'cre_sys_obt_cls' );
// 					$resClsDetials->rating = getEntityRatingInfo ( $getNodeId, 'Class' );
// 				}
// 			}
			
			return $result;
			
			
			
		}else{
			if (strstr ( $_REQUEST ['action'], 'count' )) {
				

				//NEW QUERY FOR PERFORMANCE ENHANCEMENT
				$select = db_select('slt_enrollment', 'enr');
				$select->addField('enr', 'comp_status', 'comp_status');
				$select->addField('enr', 'reg_status', 'reg_status');
				$select->addExpression('COUNT(enr.reg_status)', 'count');
					
				if($isAdminRole == "false")
				{
					$select->leftJoin('slt_person', 'per','per.id = enr.user_id');
					$select->condition('per.manager_id', $id);
				}
					
				$select->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc') ,'NOT IN');
				$select->groupBy('comp_status');
				


			/*
			$select = db_select('slt_person', 'per');
			$select->leftJoin('slt_enrollment', 'enr','enr.user_id = per.id');
			$select->leftJoin('slt_course_class', 'cls','cls.id = enr.class_id');
			$select->addField('enr', 'comp_status', 'comp_status');
			$select->addField('enr', 'reg_status', 'reg_status');
			$select->addExpression('COUNT(enr.reg_status)', 'count');
			
			if($isAdminRole == "false")
			{
			$select->condition('per.manager_id', $id);
			}
			
			$select->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc') ,'NOT IN');
			$select->groupBy('comp_status');
			*/

		}
		else if(strstr($_REQUEST['action'], 'userList'))
		{
			$select = db_select('slt_person', 'per');
			$select->leftJoin('slt_enrollment', 'enr','enr.user_id = per.id');
			$select->leftJoin('slt_course_class', 'cls','cls.id = enr.class_id');
			$select->leftJoin('slt_profile_list_items', 'pro', 'per.job_title = pro.code');
			$select->leftJoin('users', 'u', 'per.user_name = u.name');
			$select->leftJoin('file_managed', 'file', 'u.picture = file.fid');
			$select->addField('enr', 'comp_status', 'comp_status');
			$select->addField('enr', 'reg_status', 'reg_status');
			$select->addField('enr', 'is_compliance', 'is_compliance');
			$select->addField('enr', 'mandatory', 'mandatory');
			$select->addField('per', 'id', 'subordinate_id');
			$select->addField('per', 'full_name', 'full_name');
			$select->addField('per', 'user_name', 'user_name');
			$select->addField('per', 'phone_no', 'phone_no');
			$select->addField('per', 'mobile_no', 'mobile_no');
			$select->addField('per', 'email', 'email');
			$select->addField('pro', 'name', 'job_title');
			$select->addField('file', 'uri', 'user_picture');
			$select->addExpression('COUNT(enr.comp_status)','count');
			$select->addExpression('COUNT(enr.reg_status)', 'reg_count');
			if($isAdminRole == "false")
			{
				$select->condition('per.manager_id', $id);
			}
			$select->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc') ,'NOT IN');
			$select->groupBy('user_name');
		}
		else {
			$select = db_select('slt_person', 'per');
			$select->leftJoin('slt_enrollment', 'enr','enr.user_id = per.id');
			$select->leftJoin('slt_course_class', 'cls','cls.id = enr.class_id');
			$select->leftJoin('slt_profile_list_items', 'pro', 'per.job_title = pro.code');
			$select->leftJoin('slt_profile_list_items', 'profile', 'enr.comp_status = profile.code');
			$select->leftJoin('slt_profile_list_items', 'prof', 'cls.delivery_type = prof.code');
			$select->leftJoin('users', 'u', 'per.user_name = u.name');
			$select->leftJoin('file_managed', 'file', 'u.picture = file.fid');
			$select->addField('enr', 'class_id', 'class_id');
			$select->addField('enr', 'mandatory', 'mro_status');
			$select->addField('enr', 'id', 'enrolled_id');
			$select->addField('cls', 'title', 'cls_title');
			if(!$_REQUEST['export'])
			{
			$select->addField('cls', 'code', 'cls_code');
			$select->addField('cls', 'short_description', 'cls_short_description');
			}
			$select->addField('cls', 'delivery_type', 'delivery_type_code');
			$select->addField('enr', 'comp_status', 'comp_status');
			$select->addField('enr', 'reg_status', 'reg_status');
			$select->addField('enr', 'is_compliance', 'is_compliance');
			$select->addField('enr', 'mandatory', 'mandatory');
			$select->addField('per', 'id', 'subordinate_id');
			$select->addField('per', 'full_name', 'full_name');
			$select->addField('per', 'user_name', 'user_name');
			$select->addField('per', 'phone_no', 'phone_no');
			$select->addField('per', 'mobile_no', 'mobile_no');
			$select->addField('per', 'email', 'email');
			$select->addField('pro', 'name', 'job_title');
			$select->addField('file', 'uri', 'user_picture');
			$select->addField('profile', 'name', 'status');
			$select->addField('prof', 'name', 'delivery_type_name');
			if($_REQUEST['subOrdinateId'])
			{
			$select->condition('per.id', $_REQUEST['subOrdinateId']);
			}

			if($isAdminRole == "false")
			{
				$select->condition('per.manager_id', $id);
			}
			$select->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc') ,'NOT IN');
		}
		



		if($_REQUEST['action'] == 'countCompliance')
		{
			$select->condition('enr.is_compliance', 1);
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
		}
		else if($_REQUEST['action'] == 'countMandatory' )
		{
			$select->condition('enr.mandatory', 'Y');
			$select->isNull('enr.is_compliance');
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
		}
		else if($_REQUEST['action'] == 'countGeneral' )
		{
			//$select->addExpression('COUNT(enr.comp_status)', 'count');
		}
		else if($_REQUEST['action'] == 'Compliance')
		{
			$select->condition('enr.is_compliance', 1);
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
				
			if(!$_REQUEST['export'])
			{
				if($_REQUEST['cls_status'] == 'lrn_crs_cmp_cmp')
				{
					$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp');
				}
				else
				{
					$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp' ,'!=');
				}
			}
		}
		else if($_REQUEST['action'] == 'Mandatory')
		{
			$select->condition('enr.mandatory', 'Y');
			$select->isNull('enr.is_compliance');
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
				
			if(!$_REQUEST['export'])
			{
				$select->condition('enr.comp_status', $_REQUEST['cls_status']);
			}
		}
		else if($_REQUEST['action'] == 'userListCompliance')
		{
			$select->condition('enr.is_compliance', 1);
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
		
		if(!$_REQUEST['export'])
			{
				if($_REQUEST['cls_status'] == 'lrn_crs_cmp_cmp')
				{
					$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp');
				}
				else
				{
					$select->condition('enr.comp_status', 'lrn_crs_cmp_cmp' ,'!=');
				}
			}
		}
		else if($_REQUEST['action'] == 'userListMandatory')
		{
			$select->condition('enr.mandatory', 'Y');
			$select->isNull('enr.is_compliance');
			$select->condition('enr.reg_status', 'lrn_crs_reg_can', '!=');
		
			if(!$_REQUEST['export'])
			{
				$select->condition('enr.comp_status', $_REQUEST['cls_status']);
			}
		}
		else if($_REQUEST['action'] == 'userListGenaral')
		{
			if(!$_REQUEST['export'])
			{
			if($_REQUEST['cls_status'] == 'lrn_crs_reg_can')
					{
						$select->condition('enr.reg_status', $_REQUEST['cls_status']);
					}
					else
					{
						$select->condition('enr.comp_status', $_REQUEST['cls_status']);
					}
			}
		}
		else
		{
				if(!$_REQUEST['export'])
				{
					if($_REQUEST['cls_status'] == 'lrn_crs_reg_can')
					{
						$select->condition('enr.reg_status', $_REQUEST['cls_status']);
					}
					else
					{
						$select->condition('enr.comp_status', $_REQUEST['cls_status']);
					}
				}
			}

			$select->isNull('enr.master_enrollment_id');
		
			$num_rows=$select->countQuery()->execute()->fetchField();
			
			
			if($_REQUEST['paginate'] || strstr($_REQUEST['action'], 'user'))
			{				
				$start = 0;
				
				if(!empty($_REQUEST['start']))
				{
					expDebug::dPrint("innnnnnnn");
					 		$start      = $_REQUEST['start'];
				}
				
			$limit = $_REQUEST['limit'];
			$start = $start * $limit;
			
			$select->range($start,$limit);
			}
			
			expDebug::dPrintDBAPI("getMyteamCatalogResultsAPI total records:",$select);
			
		$result = $select->execute()->fetchAll();
		
		expDebug::dPrint("Myteam search queryyyy outputttttttt: ".print_r($result,true) , 4);
		
			if($num_rows){
				$result['totalrow'] = $num_rows;
			}

			if($_REQUEST['paginate'])
			{
				foreach($result as $resClsDetials)
				{
					$getNodeId   = getNodeIdFromEntity($resClsDetials->class_id,'cre_sys_obt_cls');
					$resClsDetials->rating = getEntityRatingInfo($getNodeId,'Class');
				}
			}
			
			return $result;
				

		//Below commented code is future reference. Please do not remove it (API)

		// 	$limit = 10;
		// 	$start = 0;
		// 	if(!empty($_REQUEST['rows']))
			// 		$limit     = $_REQUEST['rows'];
			// 	if(!empty($_REQUEST['start']))
				// 		$start      = $_REQUEST['start'];

				// 	$manager_id = $_REQUEST["userid"];
				// 	$delivery_type_code = $_REQUEST["delivery_type_code"];
				// 	$select = db_select('slt_person', 'per');
				// 	$select->leftJoin('slt_enrollment', 'enr','enr.user_id = per.id');
				// 	$select->leftJoin('slt_course_class', 'cls','cls.id = enr.class_id');
				// 	$select->addField('enr', 'class_id', 'class_id');
				// 	$select->addField('cls', 'title', 'cls_title');
				// 	$select->addField('cls', 'code', 'cls_code');
				// 	$select->addField('cls', 'short_description', 'cls_short_description');
				// 	$select->addField('cls', 'delivery_type', 'delivery_type_code');
				// 	$select->condition('per.manager_id', $manager_id);
				// 	$select->condition('enr.reg_status', array('lrn_crs_reg_rsv','lrn_crs_reg_rsc') ,'NOT IN');
				// 	if($_REQUEST["class_title"])
					// 	{
				// 		$cls_title =  $_REQUEST["class_title"];
				// 		$select->condition('cls.title', '%' . db_like($cls_title) . '%', 'LIKE');
				// 	}
				// 	if($_REQUEST["delivery_type_code"])
					// 	{
				// 		$delivery_type_code = $_REQUEST["delivery_type_code"];
				// 		$dtype = explode("|",$delivery_type_code);
				// 		$select->condition('cls.delivery_type', $dtype, 'IN');
				// 	}
				// 	if($_REQUEST["classlangtype"])
					// 	{
				// 		$classlangtype = $_REQUEST["classlangtype"];
				// 		$langtype = explode("|",$classlangtype);
				// 		$select->condition('cls.lang_code', $langtype, 'IN');
				// 	}
				// 	$select->groupBy('enr.class_id');
				// 	$select->orderBy('cls.title',$_REQUEST["sord"]);
				// 	$num_rows=$select->countQuery()->execute()->fetchField();
				// 	$select->range($start,$limit);
				// 	expDebug::dPrintDBAPI("getMyteamCatalogResultsAPI total records:",$num_rows);

				// 	$result = $select->execute()->fetchAll();
				// 	if($num_rows){
				// 		$result['totalrow'] = $num_rows;
				// 	}
				// 	return $result;
				}
	} catch (Exception $ex) {
		watchdog_exception('getMyteamCatalogResultsAPI', $ex);
		expertusErrorThrow($ex);
	}
}



?>
