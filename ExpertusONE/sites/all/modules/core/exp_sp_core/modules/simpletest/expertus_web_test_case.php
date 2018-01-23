<?php 

/**
 * Expertusone custom functions used in the Web testcase's
 * @author Shobana
 *
 */
//DrupalSeleniumWebTestCase
//DrupalWebTestCase
class ExpertusWebTestCase extends DrupalWebTestCase {
	protected $id = array();
	protected $paging = array();
	protected $basicParam = array(
		'_search'=>false,
		'page'=>1,		// Search page
		'rows'=>10,		// # rows to be select
		'sidx'=>'',		
		'sord'=>'desc',	// Sort order default desc
	);
	
	// general users data to test
	protected $admin;
	protected $manager;
	protected $instructor; 
	protected $learner;
	protected $accessList;

	/**
	 * Constructor for ExpertusWebTestCase.
	 */
	function __construct($test_id = NULL) {
		parent::__construct($test_id);
		$this->skipClasses[__CLASS__] = TRUE;
		$this->initUserProfiles();
	}
	
	/**
	 * Retrieves a Drupal path or an absolute path.
	 *
	 * @param $path
	 *   Drupal path or URL to load into internal browser
	 * @param $options
	 *   Options to be forwarded to url().
	 * @param $headers
	 *   An array containing additional HTTP request headers, each formatted as
	 *   "name: value".
	 * @return
	 *   The retrieved HTML string, also available as $this->drupalGetContent()
	 */
	protected function drupalGet($path, array $options = array(), array $headers = array()) {
		$options['absolute'] = TRUE;
	
		// We re-using a CURL connection here. If that connection still has certain
		// options set, it might change the GET into a POST. Make sure we clear out
		// previous options.
		$out = $this->curlExec(array(CURLOPT_HTTPGET => TRUE, CURLOPT_URL => url($path, $options), CURLOPT_NOBODY => FALSE, CURLOPT_HTTPHEADER => $headers));
		$this->refreshVariables(); // Ensure that any changes to variables in the other thread are picked up.
	
		// Replace original page output with new output from redirected page(s).
		if ($new = $this->checkForMetaRefresh()) {
			$out = $new;
		}
		if ($options['filewrite']) {
			$this->verboseCSV($out,$options['testCase']);
		}
		$this->verbose('GET request to: ' . $path .
				'<hr />Ending URL: ' . $this->getUrl() .
				'<hr />' . $out);
		return $out;
	}
	

	/**
	 * Sets verbose message in content.
	 *
	 * @param $message
	 *   The verbose message to be stored.
	 * @param $type
	 *	 The type of content that is passed eg, CSV/PDF etc
	 * 
	 */
	protected function verboseCSV($message,$type) {
		if($type == 'csv'){
			$message = mb_convert_encoding($message,'UTF-8','UTF-16LE');
		}
		if($type == 'pdf'){
			file_put_contents($this->originalFileDirectory . "/simpletest/verbose/".get_class($this).".pdf", $message, FILE_APPEND);
			$url = $this->originalFileDirectory . "/simpletest/verbose/".get_class($this).".pdf";
			require_once(DRUPAL_ROOT.'/sites/all/commonlib/pdf2text.php');
			$message = pdf2text($url);
			$message = mb_convert_encoding($message,'UTF-8','ASCII');
		}
		$this->content = $message;
	}
	
	protected function getSelectedAttribtesList(){
		try{
			$selectConfig = db_select('slt_site_configuration', 'config');
			$selectConfig->condition('config.enabled',1,'=');
			$selectConfig->addField('config', 'code');
			$selectConfig->addField('config','name');
			$selectConfig->orderBy('config.name');
			$result = $selectConfig->execute()->fetchAllKeyed();
			
			$defaultArr = array('ste_con_cnt'=>'country','ste_con_dep'=>'department','ste_con_ety'=>'employment_type','ste_con_rol'=>'role','ste_con_jbr'=>'job_role','ste_con_lng'=>'language','ste_con_org'=>'org_id','ste_con_ste'=>'state','ste_con_usr'=>'user_type');
			$enabledValues = array_intersect_key($defaultArr,$result);
			
			expDebug::dPrint("Arranged selected attributes".print_r($enabledValues,true),4);
			return $enabledValues;
		} catch(Exception $e){
				$this->exceptionHandler($e);
		}
	}
	protected function getUserAssociatedGroups($pid,$grpType = '') {
		try{
			$attrList = $this->getSelectedAttribtesList();
			expDebug::dPrint("Attributes list".print_r($attrList,1));

			$attrCount = count($attrList);
			$select = db_select('slt_groups', 'grp');
			$select->join('slt_person','per');
			$select->leftJoin('slt_group_attributes','grpatt','grpatt.group_id = grp.id');
			// Select fields and/or add expressions
			$select->distinct();
			$select->addField('grp', 'name', 'name');
			$select->addField('grp', 'id', 'id');
			$select->distinct();
			$select->condition('grp.status', 'cre_sec_sts_atv', '=');
			$select->condition('per.id', $pid, '=');
			$flg=0;
			$select1=array();
			if($attrCount > 0){
				if(!empty($attrList['ste_con_cnt'])){
					$select1[] = ("if(grp.country='All',per.country is not null,grp.country is null OR FIND_IN_SET(ifnull(per.country,''),grp.country)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_dep'])){
					$select1[] = ("if(grp.department='All',per.dept_code is not null,grp.department is null OR FIND_IN_SET(ifnull(per.dept_code,''),grp.department)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_ety'])){
					$select1[] = ("if(grp.employment_type='All',per.employment_type is not null,grp.employment_type is null OR FIND_IN_SET(ifnull(per.employment_type,''),grp.employment_type)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_rol'])){
					$select1[] = ("(CASE WHEN (grp.is_manager='Y' AND grp.is_instructor='Y') THEN
							(ifnull(per.is_manager,'N') = grp.is_manager or ifnull(per.is_instructor,'N') = grp.is_instructor)
							WHEN (grp.is_manager='Y' AND grp.is_instructor='N') THEN
							(ifnull(per.is_manager,'N') = grp.is_manager)
							WHEN (grp.is_manager='N' AND grp.is_instructor='Y') THEN
							(ifnull(per.is_instructor,'N') = grp.is_instructor)
							ELSE
							1=1
							END)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_jbr'])){
					$select->leftjoin('slt_person_jobrole_mapping','lpjm','lpjm.user_id = per.id');
					$select1[] = ("if(grp.job_role='All',(lpjm.job_role IS NOT NULL),grp.job_role is null OR FIND_IN_SET(ifnull(lpjm.job_role,''),ifnull(grp.job_role,''))>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_lng'])){
					$select1[] = ("if(grp.language='All',per.preferred_language is not null,grp.language is null OR FIND_IN_SET(ifnull(per.preferred_language,''),grp.language)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_org'])){
					$select1[] = ("if(grp.org_id='All',per.org_id is not null,grp.org_id is null OR FIND_IN_SET(per.org_id,grp.org_id)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_ste'])){
					$select1[] = ("if(grp.state='All',per.state is not null,grp.state is null OR FIND_IN_SET(ifnull(concat(per.country,'-',per.state),''),grp.state)>0)");
					$flg=1;
				}
				if(!empty($attrList['ste_con_usr'])){
					$select1[] = ("if(grp.user_type='All',per.user_type is not null,grp.user_type is null OR FIND_IN_SET(ifnull(per.user_type,''),grp.user_type)>0 )");
					$flg=1;
				}
				if(!empty($attrList['ste_con_hdt'])){
					$select1[] = ("(if(grpatt.id is not null,if(grpatt.on_or_after_start_date is not null ,DATE_FORMAT(grpatt.on_or_after_start_date,'%Y-%m-%d') <= DATE_FORMAT(per.hire_date,'%Y-%m-%d'),1=0) OR 
            		if(grpatt.on_or_before_start_date is not null, DATE_FORMAT(grpatt.on_or_before_start_date,'%Y-%m-%d') >= DATE_FORMAT(per.hire_date,'%Y-%m-%d'), 1=0) OR
					if(grpatt.between_start_date is not null ,DATE_FORMAT(per.hire_date,'%Y-%m-%d') between DATE_FORMAT(grpatt.between_start_date,'%Y-%m-%d') AND DATE_FORMAT(grpatt.between_end_date,'%Y-%m-%d'), 1=0),1=1))");
					$flg=1;
				}
			}

			if($flg==1){
				$cond = implode(' AND ',$select1);
				$select->where("((".$cond." AND ((grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0))) OR (FIND_IN_SET(per.id,grp.added_users)>0))");
			}else{
				$select->where("((grp.removed_users is null OR FIND_IN_SET(per.id,grp.removed_users)<= 0)) OR (FIND_IN_SET(per.id,grp.added_users)>0)");
			}
			$select->orderBy('name');

			if(!empty($grpType))
				$select->condition("grp.is_admin",$grpType,'=');

			expDebug::dPrintDBAPI("Query for getting the group id and name list",$select);

			$grpList = $select->execute()->fetchAll();

			expDebug::dPrint("Group id and name list for the user".print_r($grpList,true),1);
			$this->getObjectsAssociatedtoGroups('cre_sys_obt_cls',22);
			return $grpList;
		} catch(Exception $e){
				$this->exceptionHandler($e);
		}
	}
	
	protected function getUserList($uid,$limit=''){
		try{
			$userQry = db_select('slt_person','p');
			$userQry->addField('p','id','id');
			$userQry->addField('p','user_name','username');
			$userQry->addField('p','time_zone','timezone');
			if(!empty($uid)){
				if(is_array($uid))
					$userQry->condition('p.id',$uid,'IN');
				else
					$userQry->condition('p.id',$uid,'=');
			}else{
				$userQry->condition('p.id',array(1,2),'NOT IN');
			}
			if(!empty($limit))
				$userQry->range(0,$limit);
			$users = $userQry->execute()->fetchAll();
			return $users;
		} catch(Exception $e){
				$this->exceptionHandler($e);
		}
	}
	
	protected function getObjectsAssociatedtoGroups($obType , $grpId) {
		try{
			$objMap = db_select('slt_group_mapping','grpmap');
			$objMap->addField("grpmap", "id");
			$objMap->addField("grpmap", "entity_id");
			$objMap->addField("grpmap", "mro");
			$objMap->condition("grpmap.entity_type",$obType,'=');
			$objMap->condition("grpmap.group_id",$grpId,'=');
			expDebug::dPrintDBAPI("Objects associated to the groups query",$objMap);
			$res = $objMap->execute()->fetchAll();
			
			expDebug::dPrint("Result of Objects associated to the groups".print_r($res,true),4);
			return $res;
		}catch(Exception $e){
			$this->exceptionHandler($e);
		}
		
	}
	
	protected function displayTagsById($tagId){
		$dom = new DOMDocument();
    $dom->loadHTML($this->content);	
    //$xpath = new DOMXPath($dom);
    //$Node = $xpath->query('//*[@id="'.$tagId.'"]')->item(0);
    $item = $dom->getElementById($tagId);
    $item->setAttribute('style','display:block');
    $this->content = $dom->saveHTML();
	}
	
	protected function hideTagsById($tagId){
		$dom = new DOMDocument();
    $dom->loadHTML($this->content);	
    //$xpath = new DOMXPath($dom);
    //$Node = $xpath->query('//*[@id="'.$tagId.'"]')->item(0);
    $item = $dom->getElementById($tagId);
    $item->setAttribute('style','display:none');
    $this->content = $dom->saveHTML();
	}
	
	protected function getElementByAttribute($tag,$attrib,$name){
		$dom = new DOMDocument();
    $dom->loadHTML($this->content);	
    $xpath = new DOMXPath($dom);
    $Node = $xpath->query('//'.$tag.'[@'.$attrib.'="'.$name.'"]')->item(0);
    return $Node;
	}
	
	protected function updateSearchFilters(&$content, $list = array(), $search = 'div', $searchBy = 'tag', $appendTag = 'div' ){
		foreach($list as $key => $value){
  		$this->updateFiltersWithTag($content, $key, $value, $tag, $search, $searchBy, $appendTag);
  	}
  	
  	$this->content = $content;
  	$this->verbose('Request to: ' . $this->getUrl() .
                         '<hr />' . $this->content);
	}
	
	private function updateFiltersWithTag(&$content, $key, $value, $tag, $search, $searchBy, $appendTag){
  	$dom = new DOMDocument();
    $dom->loadHTML($content);	
    $xpath = new DOMXPath($dom);
    $Node = $xpath->query('//*[@id="'.$value.'"]')->item(0);
    
    // Appending Filters
    if($Node){
	    $newDom = new DOMDocument();
	    if(!empty($appendTag))
	    	$newDom->loadHTML('<'.$appendTag.'>'.$this->content[$key].'</'.$appendTag.'>');
	    else
	    	$newDom->loadHTML($this->content[$key]);
	    	
	    if($searchBy == 'tag'){
	    	$elements = $newDom->getElementsByTagName($search)->item(0);
	    }else{
	    	$elements = $newDom->getElementById($search);
	    }
	    $newNode1 = $dom->importNode($elements, TRUE);
	    $Node->appendChild($newNode1);
    }
    $content = $dom->saveHTML();
  }
  
  protected function updateSearchResults($content,$srcRc,$id){
  	$dom = new DOMDocument();
    $dom->loadHTML($content);	
    $xpath = new DOMXPath($dom);
    
  	$src = '';
    $this->cId = null;
    $this->id=array();
    foreach($srcRc['rows'] as $row){
    	$this->id[]=$row['id'];
    	$src .= '<tr id="'.$row['id'].'">';
    	if(isset($row['cell']['image']))
	    	$src .= '<td>'.$row['cell']['image'].'</td>';
	    if(isset($row['cell']['details']))
	    	$src .= '<td>'.$row['cell']['details'].'</td>';
	    if(isset($row['cell']['detail']))
	    	$src .= '<td>'.$row['cell']['detail'].'</td>';
	    if(isset($row['cell']['action']))
	    	$src .= '<td>'.$row['cell']['action'].'</td>';
	    $src .= '</tr>';
    
	    // Appending Search Details
	    if(!empty($src)){
		    $Node = $xpath->query('//*[@id="'.$id.'"]')->item(0);
		    $newDom = new DOMDocument();
		    $newDom->loadHTML($src);
		    $newNode = $dom->importNode($newDom->getElementsByTagName('tr')->item(0), TRUE);
		    $Node->appendChild($newNode); 
		    $src='';
	    }
    }
    $this->content = $dom->saveHTML();;
  	$this->verbose('Request to: ' . $this->getUrl() .
                         '<hr />' . $this->content);
  }
  
  /**
   *
   * @param  $opt
   * 	Params passed to filter the conten
   * @param  $content
   * 	Raw content in which the search filter is to be set
   * @param  $elementName
   * 	Element Group's Name for which the checkbox is to be set
   * @param  $xPathIdentifier
   * 	Identifier of the checkbox container
   * @param  $elementValues
   * 	Values for which the checkbox is to be set checked
   */
  
  public function setCheckBoxFilters($opt,&$content,$elementName,$xPathIdentifier,$elementValues){
  	$filterValues = $opt[$elementName];
  	if(isset($opt[$elementName])){
  		$filterValuesArr =	strstr($filterValues,"|") ? explode("|",$filterValues) : array();
  		if(empty($filterValuesArr)) {
  			if ($opt[$elementName]== $filterValues){
  				$elementIdArr=array_keys($elementValues,$filterValues);
  				$xPathQuery="//div[@id='".$xPathIdentifier."']/div[@id='status_".$elementIdArr[0]."']/child::label";
  				expDebug::dPrint("xPathQuery Content ".print_r($xPathQuery,true),1);
  				$key='class';
  				$values=array('narrow-search-filterset-checkbox-selected','narrow-search-filterset-item-label narrow-search-filterset-item-label-selected');
  				$this->refineSearchFilters($content,$xPathQuery,$key,$values);
  			}
  		} else{
  			foreach ($filterValuesArr as $filterVal) {
  				$elementIdArr=array_keys($elementValues,$filterVal);
  				$xPathQuery="//div[@id='".$xPathIdentifier."']/div[@id='status_".$elementIdArr[0]."']/child::label";
  				expDebug::dPrint("xPathQuery Content ".print_r($xPathQuery,true),1);
  				$key='class';
  				$values=array('narrow-search-filterset-checkbox-selected','narrow-search-filterset-item-label narrow-search-filterset-item-label-selected');
  				$this->refineSearchFilters($content,$xPathQuery,$key,$values);
  			}
  		}
  	}
  }
  /**
   * 
   * Method to set a search filter while listing an entity based on a filter criteria
   * 
   * @param $content
   * 	content in which search filter is to be set
   * @param $xPathQuery
   * 	xPath Query to select the filter element in the DOM
   * @param $key
   * 	Attribute key which need to be set
   * @param $values
   * 	Attribute value to be set, passed as as array in case of multiple items
   * 
   * 
   */  
  public function refineSearchFilters(&$content,$xPathQuery,$key,$values){
  	$dom = new DOMDocument();
  	$dom->loadHTML($content);
  	$xpath = new DOMXPath($dom);
  	$items  = $xpath->query($xPathQuery);
  	expDebug::dPrint("Refine Filter Content ".print_r($items,true),4);
  	$i=0;
  	foreach($items as $item){
  		$item->setAttribute($key,$values[$i]);
  		$i++;
  	}
  	$content = $dom->saveHTML();
  	$this->content = $content;  	
  }

  /**
   * 
   * Method to fill search keyword while listing an entity based on a filter criteria
   * @param $content
   * 	content in which search filter is to be set
   * @param $xPathQuery
   * 	xPath Query to select the filter search text element in the DOM
   * @param $value
   * 	Attribute value to be set, passed as as array in case of multiple items   * 
   *  
   */
  public function refineSearchKeyword(&$content,$xPathQuery,$value){
  	$dom = new DOMDocument();
  	$dom->loadHTML($content);
  	$xpath = new DOMXPath($dom);
  	$items  = $xpath->query($xPathQuery);
  	expDebug::dPrint("Refine Filter text ".print_r($items,true),4);
  	$i=0;
  	foreach($items as $item){
  		$item->setAttribute('value',$value[$i]);
  		$i++;
  	}
  	$content = $dom->saveHTML();
  	$this->content = $content;
  	
  }
  
  protected function addCtoolModelContent($value=array()){
  	$default = array(
  		'modelWidth' => 890,
  		'modelClass' => 'ctool-login-modal',
  		'modelTitle' => 'Course',
  		'contentWidth' => 845,
  		'contentMhight' => 335,
  		'outerHeight' => 479,
  		'outerWidth' => 890,
  		'blockHeight' => 1232,
  		'blockWidth' => 1343,
  		'modelTop' => 150,
  		'modelLeft' => 226.5,
  		'docHeight' => 1232,
  		'docWidth' => 1343
  	);
  	$modelValue = count($value) > 0 ? $value : $default;
  	$modalContent = '<div style="z-index: 1010; position: absolute; top: 150px; left: 226.5px; display: block;" id="modalContent" class="'.$modelValue['modelClass'].'">  
								<div class="ctools-modal-content ctools-sample-modal-content" style="width: '.$modelValue['modelWidth'].'px; height: auto;">    
									<table cellspacing="0" cellpadding="0" id="ctools-face-table" class="theme-ctool-popup">      
										<tbody>
											<tr> 
												<td valign="top" colspan="3" class="popups">          
													<div class="popups-container-newui">
														<div class="block-title-left">
															<div class="block-title-right">            
																<div class="modal-header popups-title block-title-middle">              
																	<span class="modal-title" id="modal-title">'.$modelValue['modelTitle'].'</span>              
																	<span class="popups-close"><a title="Close" href="#" class="close">X</a></span>              
																	<div class="clear-block"></div>            
																</div>
															</div>
														</div>            
														<div id="show_expertus_message"></div>            
														<div class="modal-scroll">
															<div class="modal-content popups-body" id="modal-content" style="width: '.$modelValue['contentWidth'].'px; height: auto; max-height: none; min-height: '.$modelValue['contentMhight'].'px; max-width: none;"></div>
														</div>        
													</div>
												</td>      
											</tr>
											<tr>
												<td>      
													<div class="block-footer-left popup-block-footer-lert"></div>       
													<div class="block-footer-middle popup-block-footer-middle"></div>       
													<div class="block-footer-right popup-block-footer-right "></div>      
												</td>
											</tr>    
										</tbody>
									</table>  
								</div>
								<input type="hidden" value="'.$modelValue['outerHeight'].'" id="modalContentOuterHeight">
								<input type="hidden" value="'.$modelValue['outerWidth'].'" id="modalContentOuterWidth">
								<input type="hidden" value="'.$modelValue['blockHeight'].'" id="modalBackdropHeight">
								<input type="hidden" value="'.$modelValue['blockWidth'].'" id="modalBackdropWidth">
								<input type="hidden" value="'.$modelValue['modelTop'].'" id="modalTop">
								<input type="hidden" value="'.$modelValue['modelLeft'].'" id="modalLeft">
								<input type="hidden" value="'.$modelValue['docHeight'].'" id="docHeight">
								<input type="hidden" value="'.$modelValue['docWidth'].'" id="docWidth">
							</div>';
  	
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$node = $dom->getElementsByTagName('body')->item(0);
  	
  	$newDom = new DOMDocument();
  	$newDom->loadHTML($modalContent);
  	$newNode = $dom->importNode($newDom->getElementsByTagName('div')->item(0), TRUE);;
  	$node->appendChild($newNode);
  	
  	$this->content = $dom->saveHTML();
  	/*$this->verbose('Request to: ' . $this->getUrl() .
                         '<hr />' . $this->content);*/
  }
  
  protected function addQtipModel($value,$addPath=''){
  	$qtipDef = array(
  		'tipWidth'=>870,
  		'tipTop'=>787,
  		'tipLeft'=>139.767,
  		'tipId'=>'qtipAttachIdqtip_addclass_visible_disp_',
  		'tipObjectId'=>'',
  		'tipTableWidht'=>373,
  		'tipContentId'=>'qtip_addclass_visible_disp_',
  		'tipEntityType'=>'cre_sys_obt_cls',
  		'tipClass'=>'qtip-parent',
  		'tipPos'=>'bottomRight'
  		
  	);
  	$qtipDef = count($value) > 0 ? $value : $qtipDef;
  	$qtipModel = '<div style="border-radius: 0px; position: absolute; width: '.$qtipDef['tipWidth'].'px; padding: 0px 0px 24px; display: block; top: '.$qtipDef['tipTop'].'px; left: '.$qtipDef['tipLeft'].'px; z-index: 6000;" class="qtip qtip-defaults qtip-active '.$qtipDef['tipClass'].' modal-qtips-close" qtip="0">
				<div style="position: absolute; height: 14px; width: 14px; margin: 0px auto 0px -33px; line-height: 0.1px; font-size: 1px; bottom: 0px; right: -31px; visibility: visible;" rel="'.$qtipDef['tipPos'].'" dir="ltr" class="qtip-tip">
					<canvas width="14" height="14"></canvas>
				</div>  
				<div style="position: relative; overflow: visible; text-align: left;" class="qtip-wrapper">    
					<div class="qtip-contentWrapper"><div class="qtip-title">
							<a style="float: right; position: relative; cursor: pointer;" class="qtip-button">
								<div onclick="closeQtyp("'.$qtipDef['tipId'].'","'.$qtipDef['tipObjectId'].'")" class="admin-bubble-close" id="'.$qtipDef['tipId'].'_close" style="visibility: visible; left: 44px;"> 
								</div></a> </div>       
					<div class="qtip-content qtip-content" style="background: transparent none repeat scroll 0% 0%; color: rgb(51, 51, 51); overflow: visible; text-align: left; padding: 5px 9px; height: auto;">
						<div id="'.$qtipDef['tipId'].'" style="min-height:75px;"><div style="margin-left:62px;min-height:75px;" id="'.$qtipDef['tipId'].'_disp">
						<span id="popup_container_'.$qtipDef['tipContentId'].'">
						<table cellspacing="0" cellpadding="0" width="'.$qtipDef['tipWidth'].'px" height="autopx" id="bubble-face-table">
						<tbody>	<tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr>
						<tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">
						<div id="paintContent'.$qtipDef['tipContentId'].'">
							<div style="" id="show_expertus_message"></div>
						</div></td><td class="bubble-cr"></td></tr>
					<tr><td class="bubble-bl"></td><td class="bubble-b">
						<table cellspacing="0" cellpadding="0" width="100%">
							<tbody><tr><td style="width:'.$qtipDef['tipTableWidht'].'px" class="bubble-blt"></td><td style="width:'.$qtipDef['tipTableWidht'].'px" class="bubble-blr"></td><td class="bubble-blm"></td></tr></tbody>
						</table></td><td class="bubble-br"></td></tr></tbody></table></span></div></div></div></div></div></div>';
  	
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$addPath = empty($addPath) ? '//body' : $addPath;
  	//$node = $dom->getElementsByTagName('body')->item(0);
  	$node  = $xpath->query($addPath)->item(0);
  	if($node){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML($qtipModel);
	  	$newNode = $dom->importNode($newDom->getElementsByTagName('div')->item(0), TRUE);;
	  	$node->appendChild($newNode);
  	}
  	$this->content = $dom->saveHTML();
  	$this->verbose('Request to: ' . $this->getUrl() .
                         '<hr />' . $this->content);
  }
  
	protected function addQtipModel2($value,$addPath='',$addHtml=''){
  	$qtipDef = array(
  		'tipWidth'=>870,
  		'tipTop'=>787,
  		'tipBottom'=>40,
  		'tipLeft'=>139.767,
  		'tipId'=>'qtipAttachIdqtip_addclass_visible_disp_',
  		'tipObjectId'=>'',
  		'tipTableWidht'=>373,
  		'tipContentId'=>'qtip_addclass_visible_disp_',
  		'tipEntityType'=>'cre_sys_obt_cls',
  		'tipClass'=>'qtip-parent',
  		'tipPos'=>'bottomRight'
  		
  	);
  	$qtipDef = count($value) > 0 ? $value : $qtipDef;
  	$qtipModel = '<div class="active-qtip-div" id="'.$qtipDef['tipId'].'_disp" style="position: absolute; left: '.$qtipDef['tipLeft'].'px; bottom: '.$qtipDef['tipBottom'].'px; z-index: 100;">
							<a onclick="$(\'#root-admin\').data(\'narrowsearch\').removeActiveQtip(\''.$qtipDef['tipId'].'\');" class="qtip-close-button"></a>
							<span id="popup_container_'.$qtipDef['tipContentId'].'">
								<table cellspacing="0" cellpadding="0" width="480px" height="autopx" class="" id="bubble-face-table">
									<tbody>
										<tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr>
										<tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">
												<div id="paintContent'.$qtipDef['tipContentId'].'">
													<div id="show_expertus_message"></div>'.$addHtml.'
												</div>
											</td><td class="bubble-cr"></td></tr>
										<tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr>
									</tbody>
								</table>
							</span>
						</div>';
  	
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$addPath = empty($addPath) ? '//body' : $addPath;
  	//$node = $dom->getElementsByTagName('body')->item(0);
  	$node  = $xpath->query($addPath)->item(0);
  	if($node){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML($qtipModel);
	  	$newNode = $dom->importNode($newDom->getElementsByTagName('div')->item(0), TRUE);;
	  	$node->appendChild($newNode);
  	}
  	
  	$this->content = $dom->saveHTML();
  	/*$this->verbose('Request to: ' . $this->getUrl() .
                         '<hr />' . $this->content);*/
  }
  
  protected function getQtipAttributes($path,$attribute){
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$node = $xpath->query($path)->item(0);
  	$str = $node->getAttribute($attribute);
  	$str = substr($str,stripos($str,'{'));
		$str = substr($str,0,stripos($str,'}')+1);
		$str = str_replace("'",'"',$str);
		$addClassObj = drupal_json_decode($str);
		//return empty($addClassObj)? $str : $addClassObj;
		return $addClassObj;
  }
  /**
   * 
   * @param $content String
   * 		DOM String of the latest content
   * @param $list Array
   * 		Mapping for a data and header
   * @param $data Array
   * 		Output of the multi-grid ajax call 
   * @param $wrapper xpath
   * 		Id of the table where the grid list to be appended
   * @return unknown_type
   */
  protected function updateGrid($content,$list,$data,$wrapper){
  	$dom = new DOMDocument();
  	$dom->loadHTML($content);
  	$xpath = new DOMXPath($dom);
  	
  	$str = '';
  	// prepare header
  	$str .= '<tr>';
  	foreach($list as $l=>$s){
  		if(strtolower($l)!='action'){
  			$str .= '<td>'.(is_array($s) ? key($s) :$s).'</td>';
  		}else{
  			for($i=0;$i<count($s);$i++){
  				$str .= '<td></td>';
  			}
  		}
  	}
  	$str .= '</tr>';
  	
  	//prepare results
  	foreach($data['rows'] as $row){
  		expDebug::dPrint("original text -- ".print_r($row['cell'],1),1);
  		$str .= '<tr>';
  		foreach($list as $l=>$s){
  			if(strtolower($l)!='action'){
  				if(is_array($s)){
  					foreach($s as $k=>$v){
  						$nstr = $this->replaceEnclosedString($v, '{#', '#}', $row['cell']);
		  				expDebug::dPrint("original text $s-- ".$v);
		  				expDebug::dPrint("computed text $s-- ".$nstr);
		  				$str .= '<td>'.$nstr.'</td>';
  					}
  					$tmp = $s[key($s)];
  				}else{
  					$str .= '<td>'.$row['cell'][$l].'</td>';
  				}
  			}else{
  				foreach($s as $k=>$v){
  					if(is_numeric($k)){
  						$str .= '<td>'.$row['cell'][$s[$i]].'</td>';
  					}else{
  						//$str .= '<td>'.str_replace('%#%',$row['cell'][$s[$k]],$v).'</td>';
  						$nstr = $this->replaceEnclosedString($v, '{#', '#}', $row['cell']);
  						expDebug::dPrint("original text $k-- ".$v);
  						expDebug::dPrint("computed text $k-- ".$nstr);
  						$str .= '<td>'.$nstr.'</td>';
  					}
  				}
  			}
  		}
  		$str .= '</tr>';
  	}
  	
  	
  	// Append content
  	$node  = $xpath->query($wrapper)->item(0);
  	if($node){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML('<div>'.$str.'</div>');
	  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);;
	  	$node->appendChild($newNode);
  	}
  	$this->content = $dom->saveHTML();
  	$this->verbose('Updated Grid : ' . $this->getUrl() .
                         '<hr />' . $this->content);
  }
  
	protected function replaceEnclosedString($str, $start='{#', $end = '#}', $data=''){
		$pos1 = strpos($str,$start) ;
		if($pos1==0) return $str;
		$pos1 = $pos1 +2;
		$pos2 = strpos($str,$end);
		$tmp = substr($str,$pos1,$pos2 - $pos1);
		$tmp2 = $start . $tmp .$end;
		$str = str_replace($tmp2,$data[$tmp],$str);
		return $this->replaceEnclosedString($str, $start, $end, $data);
	}
  /**
   * Function used to intialize user list values.
   */
  protected function initUserProfiles() {
  	$this->admin 				= new StdClass();
  	$this->admin->name 			= 'admin';
  	$this->admin->pass_raw 		= 'welcome';
  	$this->admin->role_id		= 3;
  	
  	$this->manager 				= new StdClass();
  	$this->manager->name 		= 'manager';
  	$this->manager->pass_raw 	= 'welcome';
  	$this->manager->is_manager 	= true;
  	$this->manager->role_id		= 4;
  	
  	$this->instructor 				= new StdClass();
  	$this->instructor->name 		= 'instructor';
  	$this->instructor->pass_raw 	= 'welcome';
  	$this->manager->is_instructor 	= true;
  	$this->instructor->role_id		= 5;
  	
  	$this->learner 				= new StdClass();
  	$this->learner->name 		= 'learner';
  	$this->learner->pass_raw 	= 'welcome';
  	$this->learner->role_id		= 2;
  }
  
  /**
   * Function used to login and verify user existance,
   * If user presents in the system, allow to login.
   * otherwise create a user account and login
   * 
   * Usage Information: 
   * 
   * 1. Login as a admin/manager/instructor/learner
   * $this->expertusLogin($this->admin);
   * 
   * 2. create a new account and login
   * $user = new stdClass();
   * $user->name = name of the user (mandatory)
   * $user->pass_rae = password of the user (mandatory)
   * $user->role_id = rold id to the user (mandatory)
   * $user->is_manager = true (optional)
   * $user->is_instructor = true (optional)
   * $this->expertusLogin($user);
   *  
   * @param object $user
   */
  protected function expertusLogin($user) {
  	
  	if (empty($user)) {
  		debug("User is empty");
  	}
  	else{
  	if (!empty($this->$user)) {
  		$userToLogin = $this->$user; 
  	} else {
  		$userToLogin = $user;
  	}
  	
  	$user = user_load_by_name($userToLogin->name);
  	if(!$user){
  		debug("User Not exist in DB ");
  		// User doesn't exist
  		$account = $this->expertusCreateUser($userToLogin);
  		$this->drupalLogin($account);
  	}
  	else {
  		debug("User exist in DB");
  		// User exists
  		$this->drupalLogin($userToLogin);
  	}
  	}
  	
  	
  }
  
  /**
   * Used to create user account in ExpertusONE
   * 
   * @param object $user
   * @param array $permissions
   * @return object $account
   */
  protected function expertusCreateUser($user, $permissions = array('access comments', 'access content', 'post comments', 'skip comment approval')) {
  	
  	if (empty($user->role_id)) {
  		$user->role_id = 2; // default authenticated user 
  	}
  	
  	// Create a user assigned to that role.
  	$edit = array();
  	$edit['name']   = $user->name;
  	$edit['mail']   = $edit['name'] . '@expertustest.com';
  	$edit['roles']  = array($user->role_id  => $user->role_id);
  	$edit['pass']   = $user->pass_raw;
  	$edit['status'] = 1;
  	debug($edit);
  	$account = user_save(drupal_anonymous_user(), $edit);
  	
  	// Default values
  	$default_status	  		= 'cre_usr_sts_atv';
  	$default_timezone 		= 'cre_sys_tmz_086';
  	$default_language 		= 'cre_sys_lng_eng';
  	$is_instrcutor 	 		= (isset($user->is_instructor) && $user->is_instructor) ? 'Y' : 'N';
  	$is_manager 		    = (isset($user->is_manager) && $user->is_manager) ? 'Y' : 'N';
  	
  	// expertus one user insert
  	$sql =  "insert into slt_person (first_name, last_name, user_name, status, is_manager, is_instructor,  email, time_zone, preferred_language, created_on, updated_on) values (:first_name, :last_name, :user_name, :status, :is_manager, :is_instructor, :email, :timezone, :preferred_language, :created, :updated)";
  	$result = db_query($sql, array(':first_name' => $edit['name'], ':last_name' => $edit['name'], ':user_name' => $edit['name'], ':status' => $default_status, ':email' => $edit['mail'], ':timezone' => $default_timezone, ':preferred_language' => $default_language,  ':is_manager' => $is_manager, ':is_instructor' => $is_instrcutor, ':created' => now(), ':updated'  => now()));
  
  	$this->assertTrue(!empty($account->uid), t('User created with name %name and pass %pass', array('%name' => $edit['name'], '%pass' => $edit['pass'])), t('User login'));
  	if (empty($account->uid)) {
  		return FALSE;
  	}
  
  	// Add the raw password so that we can log in as this user.
  	$account->pass_raw = $edit['pass'];
  	return $account;
  }
  
  /**
   * Get Random date time
   * @param $op (string)
   * 		date: return only the date in m-d-Y format
   * 		time: return only the time in H:i format
   * 		<any format>: specify the date time format which need to be return
   * 		default: return date and time in m-d-Y H:i format
   * @return datetime
   */
  protected function getRandomDateTime($op=''){
  	$cd = strtotime(date('Y-m-d H:i'));
  	if(empty($op)){
  		$rtnOp = 'm-d-Y H:i';
  	}else{
  		switch ($op){
  			case 'date':
  				$rtnOp = 'm-d-Y';
  				break;
  			case 'time':
  				$rtnOp = 'H:i';
  				break;
  			default:
  				$rtnOp = $op;
  				break;
  		}
  	}
  	$minval= array('00','15','30','45');
  	return date($rtnOp, mktime(rand(0,24),$minval[array_rand($minval)],0,
  			rand(date('m',$cd),12),rand(date('d',$cd)+1,30),rand(date('Y',$cd),date('Y',$cd)+1))); // Randam Date/Time
    
  }
  protected  function getDrupalUserId($loggedInUserId){
  	// Getting drupal userid
   	$objMap = db_select('users','u');
  	$objMap->addField("u", "uid");
  	$objMap->where("u.name = (select user_name from slt_person where id=".$loggedInUserId.")");
  	expDebug::dPrintDBAPI("Objects associated to the groups query",$objMap);
  	$res = $objMap->execute()->fetchAll();
  	return $res[0]->uid;
  }
  protected  function getExpUserId($drupalUserName){
  	debug($drupalUserName);
  	$objMap = db_select('slt_person','slp');
  	$objMap->addField("slp", "id");
  	$objMap->condition("slp.user_name",$drupalUserName,'=');
  	expDebug::dPrintDBAPI("Objects associated to the groups query",$objMap);
  	$res = $objMap->execute()->fetchAll();
  	return $res[0]->id;
  }
  protected  function getExistBillingAddress($user_id,$type='billing'){
  	$query = db_select('uc_orders', 'o')->distinct();
  	$alias = array();
  	$alias['first_name'] = $query->addField('o', $type . '_first_name', 'first_name');
  	$alias['last_name'] = $query->addField('o', $type . '_last_name', 'last_name');
  	$alias['phone'] = $query->addField('o', $type . '_phone', 'phone');
  	$alias['company'] = $query->addField('o', $type . '_company', 'company');
  	$alias['street1'] = $query->addField('o', $type . '_street1', 'street1');
  	$alias['street2'] = $query->addField('o', $type . '_street2', 'street2');
  	$alias['city'] = $query->addField('o', $type . '_city', 'city');
  	$alias['zone'] = $query->addField('o', $type . '_zone', 'zone');
  	$alias['postal_code'] = $query->addField('o', $type . '_postal_code', 'postal_code');
  	$alias['country'] = $query->addField('o', $type . '_country', 'country');
  	
  	// In pgsql, ORDER BY requires the field being sorted by to be in the SELECT
  	// list. But if we have the 'created' column in the SELECT list, the DISTINCT
  	// is rather useless. So we will just sort addresses alphabetically.
  	$query->condition('uid', $user_id)
  	->condition('order_status', array('payment_received','completed'), 'IN')
  	->orderBy($alias['street1']);
  	expDebug::dPrintDBAPI("Query for getting the Address",$query);
  	$result = $query->execute();
  	
  	$addresses = array();
  	while ($address = $result->fetchAssoc()) {
  		if (!empty($address['street1']) || !empty($address['postal_code'])) {
  			$addresses[] = $address;
  		}
  	}
  	
  	return $addresses;
  }
  protected function getPricedClass($user_id,$dru_user_id){
  	$select = db_select('slt_course_class', 'cls');
  	$select->addField('cls','id', 'class_id');
  	$select->addField('cls','title', 'title');
  	$select->addField('cls','price', 'price');
  	$select->addField('cls', 'course_id', 'course_id');
  	$select->condition('cls.price', '0','>');
  	$select->condition('cls.status','lrn_cls_sts_atv','=');
  	$clsdetails = $select->execute()->fetchAll();
  	expDebug::dPrint('Priced class Details----->'.print_r($clsdetails,1), 4);
  	foreach ($clsdetails as $clsidobj){
  		$select = db_select('uc_cart_products', 'ucp');
  		$select->leftJoin('slt_node_learning_activity', 'node', 'node.node_id = ucp.nid');
  		$select->addField('ucp','cart_item_id', 'id');
  		$select->condition('ucp.cart_id', $dru_user_id);
  		$select->condition('node.entity_type','cre_sys_obt_cls','=');
  		$select->condition('node.entity_id', $clsidobj->class_id);
  		expDebug::dPrintDBAPI("Check cart already added or not",$select);
  		$cartdetails = $select->execute()->fetchField();
  		expDebug::dPrint('added class Details----->'.print_r($cartdetails,1), 4);
  		if(empty($cartdetails)){
  		$select = db_select('slt_enrollment', 'enr');
  		$select->addField('enr','id', 'id');
  		$select->condition('enr.user_id', $user_id);
  		$select->condition('enr.reg_status',array('lrn_crs_reg_wtl','lrn_crs_reg_ppm','lrn_crs_reg_cnf','lrn_crs_reg_ppv','lrn_crs_reg_rsv'), 'IN');
  		$select->condition('enr.class_id', $clsidobj->class_id);
	  		expDebug::dPrintDBAPI("Check class already added or not",$select);
  		$enrdetails = $select->execute()->fetchField();
	  		expDebug::dPrint('added class Details----->'.print_r($enrdetails,1), 4);
	  		if(empty($enrdetails)){
  			return $clsidobj;
  	}
  }
  	}
  }
  protected function getNodeIdFromEntity($entity_id,$entity_type){
  	$select = db_select('slt_node_learning_activity', 'nla');
  	$select->addField('nla',	'node_id', 'node_id');
  	$select->condition('nla.entity_id',	 $entity_id);
  	$select->condition('nla.entity_type', $entity_type);
  	$select->range(0, 1);
  	$nodeId  = $select->execute()->fetchField();
  	return $nodeId;
  }
  
  /**
   * Set HTTP refer to prevent Access denied error.
   * @param string $page
   */
  protected function setReferer($page = '') {
  	$defaultPage = 'learning/enrollment-search';
  	$refererPage = ($page != '' && $page != null) ? $page : $defaultPage;
  	if(!isset($this->http_referer)) { // validate reffer value and set
  		$this->setHttpReferer($refererPage);
  	}
  }
  
  // Access control methods starts
  /**
   * Bring the access popup with empty continer
   * @param $path xpath
   * 	Access link path eg: '//a[@id="visible-course-'.$crsId.'"]'
   * @return unknown_type
   */
	protected function getAccessPopup($path){
  	$addClassObj = $this->getQtipAttributes($path,'onclick');

  	expDebug::dPrint("addClassObj ------------- ".print_r($addClassObj,true),4);
  	//Add qtip popup for class
  	$qtipDef = array(
  		'tipWidth'=>$addClassObj['wid'],
  		'tipTop'=>180,
  		'tipLeft'=>66.767,
  		'tipId'=>$addClassObj['catalogVisibleId'],
  		'tipObjectId'=>$addClassObj['entityId'],
  		'tipTableWidht'=>650,
  		'tipContentId'=>$addClassObj['popupDispId'],
  		'tipEntityType'=>'cre_sys_obt_cls',
  		'tipClass'=>'qtip-parent',
  		'tipPos'=>'bottomRight'
  		
  	);

  	$this->addQtipModel($qtipDef);
  	
  	$dom1 = new DOMDocument();
  	$dom1->loadHTML($this->content);
  	$xpath1 = new DOMXPath($dom1);
  	$mainCont = $xpath1->query('//div[@id="paintContent'.$addClassObj['popupDispId'].'"]')->item(0);
  	
  	expDebug::dPrint("Model Content ".print_r($mcont,true),1);
  	$ahtm = '<div id="paintContentVisiblePopup">
							<div style="" id="show_expertus_message"></div>
							<div id="paintContent'.$addClassObj['popupDispId'].'"></div>
						</div>';
  	
  	$dom2 = new DOMDocument();
  	$dom2->loadHTML($ahtm);
  	$newNode = $dom1->importNode($dom2->documentElement->firstChild, TRUE);;
	  $mainCont->parentNode->replaceChild($newNode,$mainCont);
  	$this->content = $dom1->saveHTML();
  	
  	/*$this->verbose('TEST Access Popup: ' . $this->getUrl() .
                         '<hr />' . $this->content);*/
  	
  	$post_field = array(
											'wrapper'=>'paintContent'.$addClassObj['popupDispId']);
	  	
  	$out = $this->drupalPostAJAX(null,array(),'Add Access','?q='.$addClassObj['url'],array(),array(),'',$post_field);
  	
  	/*$this->verbose('Access Popup: ' . $this->getUrl() .
                         '<hr />' . $this->content);*/
  	$this->assertResponse(200,'Form Repainted successfully');
  	$webError = "Website encountered an unexpected error";
  	$this->assertNoText(t($webError),'Web errors not found','catalog_access');
  	
  	// Add Multiselect box
  	$p = '//div[@id="sel-unsel-grplist"]';
  	$this->getMultiSelectContainer($p,true);
  	
  	$this->verbose('Access Popup: ' . $this->getUrl() .
                         '<hr />' . $this->content);
  }
  
  /**
   * Get the list of groups from the system and display
   * @param $path xpath
   * 		Access link path eg: '//a[@id="visible-course-'.$crsId.'"]' 
   * @param $type String
   * 		Catalog access action type eg: group/discount
   * @param $args Array
   * 		Additional arguments like page,row,type
   * @return access_list Array 
   * 		List of fetched groups
   */
  protected function listAccess($path,$type,$args=array()){
  	$args = !empty($args) ? $args : array('page'=>1,'rows'=>15,'type'=>'any');
  	
  	$addClassObj = $this->getQtipAttributes($path,'onclick');

  	$acid = $this->xpath('//input[@id="entity-val"]');
  	$uniqueId = (string) $acid[0]['value'];
  	$url = '';
  	//TODO: Need to enhance to support other types
  	switch($type){
  		case 'group':
  			$url = 'administration/catalogaccess/group/'.$uniqueId.'/0';
  			break;
  	};
  	
  	$url .= '&'.http_build_query($args);
  	expDebug::dPrint("ACCESS URL ".print_r($url,true),4);
  	
  	$out = $this->drupalPostAJAX(null,array(),'List Groups','?q='.$url,array(),array(),'',$post_field);
  
  	$this->verbose('Access Popup: ' . $this->getUrl() .
                         '<hr />' . $this->content);
  	
  	$this->assertResponse(200,'Form Repainted successfully');
  	$webError = "Website encountered an unexpected error";
  	$this->assertNoText(t($webError),'Web errors not found','catalog_access');
  	
  	// Prepare groups as list item
  	$chk='';
  	$unchk='';
  	$this->accessList = array();
  	$isChecked = false;
  	$opParam = array('cre_sys_inv_opt' => 'Optional', 'cre_sys_inv_man' => 'Mandatory', 'cre_sys_inv_rec' => 'Recommended');
  	foreach($out['data'] as $acdata){
  		if(!in_array($acdata['opt'],$this->accessList))
  			$this->accessList[$acdata['opt']] = $acdata['val'];
  			
  		if($acdata['sel']!=0){
		   	$chk .= '<li><div class="checkbox-selected">
		   						<input type="checkbox" checked = "" name="' . str_replace('"','\&quot;',$acdata['val']) . '" id="' . $acdata['opt'] . '"  opt="enabled">
		   					</div>
		   					<span id ="li-listitem" for="' . str_replace('"','\&quot;',$acdata['val']) . '" class="li-selected label-text" title="' . str_replace('"','\&quot;',$acdata['val']) . '">' . str_replace('"','\&quot;',$acdata['val']) . '</span>
		   					';
		   	if(!empty($acdata['mro'])){
		    	$chk .= '<div class="option-div"><span class="grey-pointer"></span>
			  							<span class="ui-opt-span" id="ui-opt-span">
			  									<span val="'.$acdata['mro'].'" id="option-'.$acdata['opt'].'" class="selected-opt1">'.$opParam[$acdata['mro']].'</span>
			  									<span class="dropdown background" id="dropdown-'.$acdata['opt'].'"></span>
			  							</span></div>';
		    }
		    $chk .= '</li>';
		   	$unchk .= '<li style ="display:none">
		   							<div class="checkbox-unselected">
		   								<input type="checkbox" name="' . str_replace('"','\&quot;',$acdata['val']) . '" id="' . $acdata['opt'] . '" opt="enabled">
		   							</div>
		   							<span class="label-text" for="' . str_replace('"','\&quot;',$acdata['val']) . '" title="' . str_replace('"','\&quot;',$acdata['val']) . '">' . $acdata['val'] . '
		   							</span>
		   							</li>';
		    
		   	$this->showRightPaneData($acdata['opt'],$opParam[$acdata['mro']],str_replace('"','\&quot;',$acdata['val']));
		   	$isChecked = true;
  		}else{
  			$unchk .= '<li>
		   							<div class="checkbox-unselected">
		   								<input type="checkbox" name="' . str_replace('"','\&quot;',$acdata['val']) . '" id="' . $acdata['opt'] . '" opt="enabled">
		   							</div>
		   							<span class="label-text" for="' . str_replace('"','\&quot;',$acdata['val']) . '" title="' . str_replace('"','\&quot;',$acdata['val']) . '">' . $acdata['val'] . '
		   							</span>
		   						</li>';
  		}
  	}
  	
  	// Append the prepared list into dom
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$node1  = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//div[@id="chk"]')->item(0);
  	$node2  = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//div[@id="unchk"]')->item(0);
  	$totalRows = 0;
  	if(!$node1->hasChildNodes()){
  		$chk = '<ul>' . $chk . '</ul>';
  		$unchk = '<ul>' . $unchk . '</ul>';
  	}else{
  		$node1  = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//div[@id="chk"]/ul')->item(0);
  		$node2  = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//div[@id="unchk"]/ul')->item(0);
  		$totalRows = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//li')->length;
  	}
  	$totalRows += count($out['data']);

  	// Check and enable scroll
  	if($totalRows > 8){
  		$scroll = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]/div[@class="jspContainer"]')->item(0);
  		if($scroll){
  			$attrs = explode(';',$scroll->getAttribute('style'));
	  		$attrs[]='overflow-y:scroll';
	  		$attrs[]='overflow-x:hidden';
	  		$scroll->setAttribute('style',implode(';',$attrs));
  		}
  	}
  	// Add checked list in dom
  	if($node1 && !empty($chk)){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML($chk);
	  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);;
	  	$node1->appendChild($newNode);
  	}
  	// add unchecked list in dom
  	if($node2 && !empty($unchk)){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML($unchk);
	  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);;
	  	$node2->appendChild($newNode);
  	}
  	// Enable line separator
  	if($isChecked == true){
	  	$node3 = $xpath->query('//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]//div[@class="line-separator"]')->item(0);
	  	$node3->setAttribute('style','display:block');
  	}
  	$this->content = $dom->saveHTML();
  	$this->verbose('Access Popup with option: ' . $this->getUrl() .
                         '<hr />' . $this->content);
  	return $this->accessList;
  }
  
  /**
   * Select or Unselect the list of groups from the list
   * @param $path xpath
   * 		Path to the continer 
   * 		Default: '//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]'
   * @param $action String
   * 		Action to perform 
   * 		Available options: select/unselect
   * 		Default: select
   * @param $list Array
   * 		List of groups need to be select or unselect with group id and option
   * 		eg: array('4'=>'Optional','5'=>'Mandatory')
   * @return unknown_type
   */
	protected function checkUncheckAccess($path = '', $action = 'select', $list = array()){
  	$path = !empty($path) ? $path : '//div[@id="group-control"]/div[@id="sel-unsel-grplist"]/div[@id="ui-multiselect-con"]/div[@id="container"]';
  	$opParam = array('Optional' => 'cre_sys_inv_opt','Mandatory'=>'cre_sys_inv_man','Recommended'=>'cre_sys_inv_rec');
  	foreach($list as $lst=>$op){
  		$dom = new DOMDocument();
	  	$dom->loadHTML($this->content);
	  	$xpath = new DOMXPath($dom);
	  	if($action == 'select'){ // Select a list
		  	// Fetch the list to be checked
		  	$node1  = $xpath->query($path.'//div[@id="unchk"]//input[@id="'.$lst.'"]/../..')->item(0);
		  	$nodeX = $dom->saveHTML($node1);
		  	$node1->setAttribute('style','display:none');
		  	$node2 = $xpath->query($path.'//div[@id="chk"]/ul')->item(0);
		  	// Show the selected list in the checked list div
	  		if($node2){
	  			// Append selected list to checked lists
		  		$newDom = new DOMDocument();
			  	$newDom->loadHTML($nodeX);
			  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);
			  	$node2->appendChild($newNode);
			  	$nodex  = $xpath->query($path.'//div[@id="chk"]//input[@id="'.$lst.'"]/..')->item(0);
			  	// Update class for the selected item
					$nodex->setAttribute('class','checkbox-selected');
					
					// Append option list if present
					if(!empty($op)){
						$oplist = '<div class="option-div" style="width: 129px;">
		  					<span class="grey-pointer"></span>
		  					<span class="ui-opt-span" id="ui-opt-span">
		  						<span val="'.$opParam[$op].'" id="option-'.$lst.'" class="selected-opt1">'.$op.'</span>
		  						<div class="dropdown background" id="dropdown-'.$lst.'"></div>
		  					</span>
		  				</div>';
						$node3 = $xpath->query($path.'//div[@id="chk"]//input[@id="'.$lst.'"]/../..')->item(0);
						$newDom = new DOMDocument();
				  	$newDom->loadHTML($oplist);
				  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);
				  	$node3->appendChild($newNode);
					}
					
					// Enable line separator
			  	$node3 = $xpath->query($path.'//div[@class="line-separator"]')->item(0);
			  	$node3->setAttribute('style','display:block');
	  		}
	  		$this->content = $dom->saveHTML();
	  		$xlistNode = $xpath->query($path.'//div[@id="chk"]//input[@id="'.$lst.'"]')->item(0);
  			$xlistName = $xlistNode->getAttribute('name');
	  		$this->showRightPaneData($lst,$op,$xlistName);
	  	}else{ // Unselect a list
		  	// Fetch the list to be checked
		  	$node1  = $xpath->query($path.'//div[@id="chk"]//input[@id="'.$lst.'"]/../..')->item(0);
		  	$node1->parentNode->removeChild($node1);

		  	$node2 = $xpath->query($path.'//div[@id="unchk"]//input[@id="'.$lst.'"]/../..')->item(0);
		  	$node2->setAttribute('style','display:block');
	  		
		  	$node = $xpath->query('//div[@id="sel-group-list"]//div[@class="cls-access-list-select"]//span[@id="group-names-'.$lst.'"]/..')->item(0);
		  	$node->parentNode->removeChild($node);
		  	
		  	//Set selected id list in hidden field
		  	$hNode = $xpath->query('//input[@id="hidden_idlist"]')->item(0);
		  	$hNodeVal = explode(',',$hNode->getAttribute('value'));
		  	foreach($hNodeVal as $k=>$idl){
		  		$tmp = explode('-',$idl);
		  		if($tmp[0]==$lst){
		  			unset($hNodeVal[$k]);
		  		}
		  	}
		  	$hNodeVal = array_filter($hNodeVal);
		  	$hNode->setAttribute('value',implode(',',$hNodeVal));
		  	
		  	$nodex  = $xpath->query($path.'//div[@id="chk"]/ul')->item(0);
		  	
				// Enable line separator
		  	if(!$nodex->hasChildNodes()){
			  	$node3 = $xpath->query($path.'//div[@class="line-separator"]')->item(0);
			  	$node3->setAttribute('style','display:none');
			  	
			  	// Show/hide help text messages
			  	$disp = $xpath->query('//div[@id="help-msg"]')->item(0);
			  	$disp->setAttribute('style','display:none');
			  	
			  	$hide = $xpath->query('//div[@id="sel-group-text"]')->item(0);
			  	$hide->setAttribute('style','display:inline');
			  	
			  	$hide = $xpath->query('//div[@id="sel-message"]')->item(0);
			  	$hide->setAttribute('style','display:inline');
			  	
			  	$hide = $xpath->query('//div[@id="sel-msg"]')->item(0);
			  	$hide->setAttribute('style','display:inline');
		  	}
		  	$this->content = $dom->saveHTML();
		  	/*$this->verbose('Course Save: ' . $this->getUrl() .
	                         '<hr />' . $this->content);*/
  		}
	 	}
	 	$this->verbose('Course Save: ' . $this->getUrl() .
	                         '<hr />' . $this->content);
  }
  
  /**
   * Show the selected groups in right side pane
   * @param $lst Id
   * 		Group Id
   * @param $op String
   * 		MRO	name
   * @param $xlistName String
   * 		Group name
   * @return unknown_type
   */
  private function showRightPaneData($lst,$op,$xlistName){
  	// Show the selected list in the right side pane
  	$opParam = array('Optional' => 'cre_sys_inv_opt','Mandatory'=>'cre_sys_inv_man','Recommended'=>'cre_sys_inv_rec');
	  $dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$opl = !empty($op) ? '<span id="mro-names-'.$lst.'" style="float: left; width: 100px; padding-left: 10px;"> ('.$op.')</span>' : '';
  	$htm = '<div class="cls-access-list-select">
  						<span title="'.$xlistName.'" class="vtip" id="group-names-'.$lst.'" style="width: 160px; float: left;">
  							<div class="fade-out-title-container addedit-cataog-access-group-name-fadeout-container" style="max-width: 160px;">
  								<span class="title-lengthy-text">- '.$xlistName.'<span class="fade-out-image"></span></span>
  							</div>
  						</span>'.$opl.'
  					</div>';
  	
  	$node = $xpath->query('//div[@id="sel-group-list"]')->item(0);
  	$newDom = new DOMDocument();
  	$newDom->loadHTML($htm);
  	$newNode = $dom->importNode($newDom->documentElement->firstChild, TRUE);
  	$node->appendChild($newNode);
  	
  	//Set selected id list in hidden field
  	$hNode = $xpath->query('//input[@id="hidden_idlist"]')->item(0);
  	$hNodeVal = explode(',',$hNode->getAttribute('value'));
  	$hNodeVal[] = $lst .'-' . (!empty($op) ? $opParam[$op] : '');
  	$hNodeVal = array_filter($hNodeVal);
  	$hNode->setAttribute('value',implode(',',$hNodeVal));
  	
  	// Show/hide help text messages
  	$disp = $xpath->query('//div[@id="help-msg"]')->item(0);
  	$disp->setAttribute('style','display:block');
  	
  	$hide = $xpath->query('//div[@id="sel-group-text"]')->item(0);
  	$hide->setAttribute('style','display:none');
  	
  	$hide = $xpath->query('//div[@id="sel-message"]')->item(0);
  	$hide->setAttribute('style','display:none');
  	
  	$hide = $xpath->query('//div[@id="sel-msg"]')->item(0);
  	$hide->setAttribute('style','display:none');
  	
  	$this->content = $dom->saveHTML();

  }
  
  /**
   * Summit the list to save in database
   * @param $path xpath
   * 		Access link path eg: '//a[@id="visible-course-'.$crsId.'"]' 
   * @return unknown_type
   */
	protected function submitAccess($path){
  	$addClassObj = $this->getQtipAttributes($path,'onclick');
  	// Save form
  	$param = array(
								"form_id"=>"exp_sp_administration_catalog_access_addedit_form_html",
								"_triggering_element_name"=>"op",
								"_triggering_element_value"=>"Save",
						);

		$formPath = '//div[@id="'.$addClassObj['catalogVisibleId'].'"]//div[@id="addedit-form-wrapper"]/form';
		$formIds = $this->xpath($formPath);
  	$formId = (string) $formIds[0]['id'];
  	
  	$post_field = array('submit'=>$param,	'wrapper'=>'addedit-form-wrapper');
  	$out = $this->drupalPostAJAX(null,array(),'Save','?q=system/ajax',array(),array(),$formId,$post_field);
  	$this->verbose('Course Save: ' . $this->getUrl() .
	                         '<hr />' . $this->content);
  	
  	// Check any validation error occurs 
  	$error = '';
  	$isError = $this->xpath('//div[@class="messages error"]//ul/li');
  	foreach($isError as $err){
  		$error .= (string) $err[0]->span;
  	}
  	
  	$this->assertResponse(200,'Form Repainted successfully');
  	$this->assertNoText(t($error),'Error - '.$error,'catalog_admin');
  	$webError = "Website encountered an unexpected error";
  	$this->assertNoText(t($webError),'Web errors not found','catalog_admin');
  	
  	$t1 = 'Changes made will have impact in Class';
  	$t2 = 'Details saved successfully';
  	
  	$this->assertText(t($t1),'Changes made will have impact in Class','catalog_admin');
  	$this->assertText(t($t2),'Details saved successfully','catalog_admin');
  	
  }
  
  /**
   * Add group list continer with group options into dom
   * @param $path xpath
   * 		Path to the wrapper eg: //div[@id="sel-unsel-grplist"]
   * @param $opRequire Boolean
   * 		Whether to add MRO option list or not
   * @return unknown_type
   */
  private function getMultiSelectContainer($path, $opRequire = false){
  	if($opRequire == true){
  		$opList = '<div id="optionallist" style="display: none;">
  								<ul>
  									<li id="cre_sys_inv_opt">Optional</li>
  									<li id="cre_sys_inv_man">Mandatory</li>
  									<li id="cre_sys_inv_rec">Recommended</li>
  								</ul>
  							</div>';
  	}else{
  		$opList ='';
  	}
  	
  	$multiSel = '<div class="ui-multiselect-con-class" id="ui-multiselect-con" style="width: 270px; height: 235px;">
  								<div class="autocomplete" id="autocomplete">
  									<input type="text" value="Type a group name" id="multiautocomplete" name="autocomplete" style="width: 230px; color: rgb(153, 153, 153); font-size: 10px; line-height: 21px;" autocomplete="off" class="ac_input">
  									<span><div id="autoimg"></div></span></div>
  									<div style="display:none;" class="no-rec-msg">There are no records in the system</div>
  									<div class="mcontainerclass" id="container" style="height: 170px; overflow: hidden; padding: 0px; width: 248px;">
  										<div class="jspContainer" style="width: 248px; height: 170px;">
  											<div class="jspPane" style="padding: 0px; top: 0px; width: 248px;">
  												<div id="chk"></div>'.$opList.'
  													<div style="display:none;" class="line-separator"></div>
  													<div id="unchk"></div><div class="overflowitems" id="overflowitems"></div>
  													<div style="height:60px;display:block" class="footeraccess">&nbsp;</div>
  												</div>
  											</div>
  										</div>
  									</div>';
  	
  	
  	$dom = new DOMDocument();
  	$dom->loadHTML($this->content);
  	$xpath = new DOMXPath($dom);
  	$node  = $xpath->query($path)->item(0);
  	if($node){
  		$newDom = new DOMDocument();
	  	$newDom->loadHTML($multiSel);
	  	$newNode = $dom->importNode($newDom->getElementsByTagName('div')->item(0), TRUE);;
	  	$node->appendChild($newNode);
  	}
  	$this->content = $dom->saveHTML();
  }
  // Access control methods end
  
  /**
   * Get random timezones 
   * @param $cnt Integer
   * 		Number of timezones to be return
   * 		Default: 1
   * @return Array
   * 		Return timezone as an array
   */
  protected function getTimeZone($cnt=1){
  	$tz = array(
				  	array('code'=>'cre_sys_tmz_002','name'=>'(GMT-11:00) Samoa Standard Time/Midway','attr1'=>'(GMT-11:00)','attr2'=>'Pacific/Midway','attr3'=>'webex_1'),
						array('code'=>'cre_sys_tmz_005','name'=>'(GMT-08:00) Pacific Standard Time (USA)/los_Angeles','attr1'=>'(GMT-08:00)','attr2'=>'America/Los_Angeles','attr3'=>'webex_4'),
						array('code'=>'cre_sys_tmz_007','name'=>'(GMT-07:00) Mountain Standard Time/Phoenix','attr1'=>'(GMT-07:00)','attr2'=>'America/Phoenix','attr3'=>'webex_5'),
						array('code'=>'cre_sys_tmz_009','name'=>'(GMT-07:00) Mountain Standard Time/Mazatlan','attr1'=>'(GMT-07:00)','attr2'=>'America/Mazatlan','attr3'=>'webex_5'),
						array('code'=>'cre_sys_tmz_010','name'=>'(GMT-07:00) Mountain Standard Time/Denver','attr1'=>'(GMT-07:00)','attr2'=>'America/Denver','attr3'=>'webex_6'),
						array('code'=>'cre_sys_tmz_012','name'=>'(GMT-06:00) Central Standard Time/Chicago','attr1'=>'(GMT-06:00)','attr2'=>'America/Chicago','attr3'=>'webex_7'),
						array('code'=>'cre_sys_tmz_013','name'=>'(GMT-06:00) Central Standard Time/Mexico_City','attr1'=>'(GMT-06:00)','attr2'=>'America/Mexico_City','attr3'=>'webex_8'),
						array('code'=>'cre_sys_tmz_017','name'=>'(GMT-05:00) Eastern Standard Time/New York','attr1'=>'(GMT-05:00)','attr2'=>'America/New_York','attr3'=>'webex_11'),
						array('code'=>'cre_sys_tmz_022','name'=>'(GMT-04:00) Chile Time/Santiago','attr1'=>'(GMT-04:00)','attr2'=>'America/Santiago','attr3'=>'webex_13'),
						array('code'=>'cre_sys_tmz_032','name'=>'(GMT) British Summer Time/London','attr1'=>'(GMT)','attr2'=>'Europe/London','attr3'=>'webex_21'),
						array('code'=>'cre_sys_tmz_035','name'=>'(GMT+01:00) Central European Summer Time/Brussels','attr1'=>'(GMT+01:00)','attr2'=>'Europe/Brussels','attr3'=>'webex_25'),
						array('code'=>'cre_sys_tmz_036','name'=>'(GMT+01:00) Central European Summer Time/Warsaw','attr1'=>'(GMT+01:00)','attr2'=>'Europe/Warsaw','attr3'=>'webex_25'),
						array('code'=>'cre_sys_tmz_049','name'=>'(GMT+03:00) Moscow Time/Moscow','attr1'=>'(GMT+03:00)','attr2'=>'Europe/Moscow','attr3'=>'webex_33'),
						array('code'=>'cre_sys_tmz_053','name'=>'(GMT+04:00) Gulf Standard Time/Muscat','attr1'=>'(GMT+04:00)','attr2'=>'Asia/Muscat','attr3'=>'webex_36'),
						array('code'=>'cre_sys_tmz_059','name'=>'(GMT+05:30) Indian Standard Time/Kolkata','attr1'=>'(GMT+05:30)','attr2'=>'Asia/Kolkata','attr3'=>'webex_41'),
						array('code'=>'cre_sys_tmz_060','name'=>'(GMT+05:30) Indian Standard Time/Colombo','attr1'=>'(GMT+05:30)','attr2'=>'Asia/Colombo','attr3'=>'webex_42'),
						array('code'=>'cre_sys_tmz_067','name'=>'(GMT+08:00) Hong Kong Time/Hong Kong','attr1'=>'(GMT+08:00)','attr2'=>'Asia/Hong_Kong','attr3'=>'webex_48'),
						array('code'=>'cre_sys_tmz_072','name'=>'(GMT+09:00) Japan Standard Time/Tokyo','attr1'=>'(GMT+09:00)','attr2'=>'Asia/Tokyo','attr3'=>'webex_49'),
						array('code'=>'cre_sys_tmz_075','name'=>'(GMT+09:30) Central Standard Time/Adelaide','attr1'=>'(GMT+09:30)','attr2'=>'Australia/Adelaide','attr3'=>'webex_52'),
						array('code'=>'cre_sys_tmz_083','name'=>'(GMT+12:00) New Zealand Standard Time/Auckland','attr1'=>'(GMT+12:00)','attr2'=>'Pacific/Auckland','attr3'=>'webex_60'),
						array('code'=>'cre_sys_tmz_086','name'=>'(UTC/GMT) Coordinated Universal Time / Greenwich Mean Time','attr1'=>'(GMT)','attr2'=>'UTC','attr3'=>'webex_20'),
  			);
  	$rtz = array();
  	foreach(array_rand($tz,$cnt) as $tind){
  		$rtz[] = $tz[$tind];
  	}
  	return $rtz;
  }
} // END OF CLASS ExpertusWebTestCase


/**
 * Extendable class for ExpertusONE API test
 * @author Vincent
 *
 */
class ExpertusAPITestCase extends DrupalTestCase{
	/**
	 * Api Name which under go for testing
	 * @var String
	 */
	private $apis;
	
	/**
	 * Unix timestamp, to update the test class for support
	 * multiple users to run different API test using common databse.
	 * @var String
	 */
	protected static $timestamp;
	
	/**
	 * Constructor of this class.
	 * It ges the list of available APIs in ExpertusONE
	 * @return unknown_type
	 */
	function __constructor(){
		$this->apis = parse_ini_file(DRUPAL_ROOT."/apis/dynamicapi/rest_api.config", 1);
	}
	
	/**
	 * Get the list of APIs attributes
	 * @param $apiname String
	 * 		If API name is given return the respective API's attribute.
	 *    Otherwise return all the available API's attribures.
	 * @return API's Attribute as Array
	 */
	protected function getApis($apiname=''){
		try{
			if(!empty($apiname)){
				return $this->apis[trim($apiname)];
			}
			return $this->apis;
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Proceed to test
	 * @param $url String
	 * 		Base ULR of API test
	 * @param $apiParam Array
	 * 		API's attributes 
	 * @param $tests
	 * 		Test cases
	 * @return NA
	 */
	protected function runTest($url,$apiParam,$tests){
		try{
			//expDebug::dPrint("Final Data ".print_r($apiParam,true),4);
			
			// Set endpoint URL and execute the test
			$url .= '/apis/ext/ExpertusOneAPI.php';
			$response = $this->runExec($url,$apiParam['params']);
			expDebug::dPrint("Final Response ".print_r($response,true),4);
			
			//Check Test cases with test results
			foreach($tests['result'] as $test){
				$this->assert(strpos($response, $test) !== FALSE, "Result ".htmlentities($test)." Success.", $apiParam['apiname']);
			}
			
			//Check Test cases with expected outputs
			foreach($tests['output'] as $test){
				$this->assert(strpos($response, $test) !== FALSE, "Parameter $test found in result.", $apiParam['apiname']);
			}
		}catch(Exception $e){
			throw new Exception($e->getMessage());
		}
	}
	
	/**
	 * Generate access token for testing
	 * @param $url String
	 * 		Base URL
	 * @return Access Token String
	 */
	protected function getToken($url){
		$conf = parse_ini_file(DRUPAL_ROOT."/sites/default/exp_sp.ini");
		$client_id = $conf['oauth_client_id'];
		$client_secret = $conf['oauth_client_secret'];
		$client_details = base64_encode("$client_id:$client_secret");
		$data = array("grant_type"=>"client_credentials");

		$fields_string = $this->preparePost($data);
		
		$context = stream_context_create(array(
			'http' => array(
			'method' => 'POST',
			'header' => "Content-Type: application/x-www-form-urlencoded\r\n" .
						"Authorization : Basic $client_details\r\n", //.
			'content' => $fields_string
		),
		'ssl' => array(
				'verify_peer'=>false, 
	      'verify_peer_name'=>false
			)
		));
	
		$response = file_get_contents($url.'/apis/oauth2/Token.php', false, $context);
		
		$response = json_decode($response);
		return $response->access_token;
	}
	
	/**
	 * Run the test bu using CURL
	 * @param $url
	 * 		Endpoint URL 
	 * @param $data
	 * 		Post Data for the API test call
	 * @return unknown_type
	 */
	private function runExec($url,$data){
		$post = $this->preparePost($data);
		$ch = curl_init();

		curl_setopt($ch, CURLOPT_URL,$url);
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_POSTFIELDS,$post);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE); 
		
		$response = curl_exec ($ch);
		return $response;
	}
	
	/**
	 * Prepare post field from given parameters
	 * @param $data Array
	 * 		Post file paramters with value
	 * @return Post fields String
	 */
	private function preparePost($data){ // http_build_query
		/*foreach($data as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
		return rtrim($fields_string, '&');*/
		return http_build_query($data);
	}
	
	/**
	 * (non-PHPdoc)
	 * @see sites/all/modules/core/exp_sp_core/modules/simpletest/DrupalTestCase#assert()
	 */
	protected function assert($status, $message = '', $group = 'Other', array $caller = NULL) {
    // Convert boolean status to string status.
    if (is_bool($status)) {
      $status = $status ? 'pass' : 'fail';
    }

    // Increment summary result counter.
    $this->results['#' . $status]++;

    // Get the function information about the call to the assertion method.
    if (!$caller) {
      $caller = $this->getAssertionCall();
    }

    // Creation assertion array that can be displayed while tests are running.
    $this->assertions[] = $assertion = array(
      'test_class' => 'API_TEST_'.self::$timestamp,
      'status' => $status,
      'message' => $message,
      'message_group' => $group,
      'function' => $caller['function'],
      'line' => $caller['line'],
      'file' => $caller['file'],
    );

    // Store assertion for display after the test has completed.
    Database::getConnection('default')
      ->insert('simpletest')
      ->fields($assertion)
      ->execute();

    // We do not use a ternary operator here to allow a breakpoint on
    // test failure.
    if ($status == 'pass') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  
  /**
   * Delete the test result which stored in database after completion of test
   * @return NA
   */
  protected function deleteTest(){
  	$cond = 'API_TEST_'.self::$timestamp;
  	//db_query('DELETE FROM simpletest WHERE test_class = :tclass ',array(':tclass'=>"$cond"));
  }
  
  /**
   * Show the test result in the screen after completion of test
   * @return HTML output
   */
  public static function showResults(){
  	$cond = 'API_TEST_'.self::$timestamp;
  	$results = db_query('SELECT status, message, message_group FROM simpletest WHERE test_class= :tclass ORDER BY message_group',array(':tclass'=>"$cond"))->fetchAll();
  	
   	$name = '';
   	$htmlhead ='<!doctype html>';
   	$htmlhead .='<head>';
   	print $htmlhead;
  	?>
  	<style>
  	table{
  	 border:1px solid #ccc;
  	 border-spacing: 0;
  	 border-collapse: collapse;
  	 color: #2d2d2d;
     font-family: openSansRegular,Arial;
     font-size: 12px;
     width:40%
  	}
  	table tr td,
  	.htitle th{
  	 border:1px solid #ccc;
  	 padding:3px
  	}
  	.htitle th{
  	color: #464646;
    font-family: ProximaNovaBold,Arial;
    font-size: 13px;
    text-align: left
  	}
  	.htitle{
  	  padding: 3px 10px;
  	  text-transform: uppercase;
  	  background-color:#e1e2dc
  	}
  	.pass{
  	  background-color:#b6ffb6
  	}
  	.fail{
  	  background-color:#ffc9c9
  	}
  	</style>
  	<?php 
  	$html .='</head>';
  	$html .='<body>';
  //	$html .='<div>';
  	$html .= '<table>';
  	$html .= '<thead><tr class="htitle">';
  	$html .= '<th width="35%">Description</th><th width="5%">Status</th>';
  	$html .= '</tr></thead>';
  	foreach($results as $result){
  		
  		if($name != $result->message_group){
	  		$html .= '<tr>';
	  		$html .= '<td  colspan=2 ><b>API Name:</b> '.$result->message_group.'</td>';
	  		$html .= '</tr>';
  		}
  		$name = $result->message_group;
  		//expDebug::dPrint("Get testAPIs Status -- ".print_r($result->status,true),1);
  		if($result->status=="pass"){
  		//expDebug::dPrint("Get testAPIs Status 001 -- ".print_r($result->status,true),1);
	  		$html .= '<tr class="pass">';
	  		$html .= '<td>'.$result->message.'</td>';
	  		$html .= '<td>'.$result->status.'</td>';
	  		$html .= '</tr>';
  		}else{
  		//expDebug::dPrint("Get testAPIs Status 002 -- ".print_r($result->status,true),1);
	  		$html .= '<tr class="fail">';
	  		$html .= '<td>'.$result->message.'</td>';
	  		$html .= '<td>'.$result->status.'</td>';
	  		$html .= '</tr>';
  		}
  	}
  //	$html .='</div>';
  	$html .='</body>';
  	print $html;
  	
  }
  
  protected function extendTokenValidity($token){
  	db_query('UPDATE slt_oauth_access_tokens SET expires = DATE_ADD(expires, INTERVAL 1 DAY) WHERE access_token  = :token ',array(':token'=>$token));
  }
}

?>