<?php 
require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solrclient.php';

class UserSolrSearch extends SolrClient{
	
	function __construct(){
		// Get collection prefix from configuration
		$prefix = getConfigValue('collection_prefix');
	
		// Set exact collection name (should match with solr's collection)
		$this->collName = $prefix . '_' . SolrClient::UserCore;
	
		//Call parent constructor
		parent::__construct();
	}
	
	/**
	 * Fetch user list from Solr. Main method of user search.
	 * @return (Object) Result set
	 */
	
	public function getSolrSearchResult(){
		try{
			// Set field mapping for class level search
			$fieldList = array(
					'id:id',
					'lastname:LastName',
					'firstname:FirstName',
					'username:Username',
					'email:Email',
					'manager_id:ManagerID',
					'status_code:Status',
					'addr1:Address1',
					'addr2:Address2',
					'city:City',
					'statecode:State',
					'countrycode:Country',
					'preferredlanguagecode:PreferredLanguage',
					'timezonecode:TimeZoneCode',
					'register_sms:RegisterSMS',
					'orgname:Organization',
					'dottedorg:DottedOrganizationName',
					'dottedmanagersid:DottedManagerID',
					'uid:UID',
					'ManagerName',
					'ClassRegistration',
					'ProgramRegistration',
					'Last_Login',
					'Privilege',
					'CreatedBy',
					'UpdatedBy',
					'HireDate',
			);
			
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				$apiFields = array(
					'phone_no:PhoneNumber',
					'mobile_no:MobileNumber',
					'hire_date:HireDate',
					'otherorganisationid:DottedOrganizationID',
					'othermanagerid:DottedManagerID',
					'roles:UserRoles',
					'zipcode:ZipCode',
					'mobile_no:ContactNumber',
					'currency:PreferredCurrency',
					'is_verified_add:AddressVerified',
					'is_valid_add:AddressValid',
					'invalid_add_reason:InvalidAddressReason',
					'currency_symbol:CurrencySymbol',
					'currency_code:CurrencyCode',
					'usertype:UserType',
					'empltype:EmploymentType',
					'employeeno:EmployeeNo',
					'jobrole:JobRole',
					'jobtitle:JobTitle',
					'deptcode:Department',
					'statecode:StateCode',
					'countrycode:CountryCode',
				);
				// Field names for API and WEB are different hence removing for api
				unset($fieldList['dottedmanagersid']);
				unset($fieldList['HireDate']); 
				unset($fieldList['statecode']);
				unset($fieldList['countrycode']);
				$fieldList = array_merge($fieldList,$apiFields);
			}
				
			// Setting required filters
			$filter = array();
			$this->setAdminAccessFilters($filter,'user');
			$this->setSearchFilters($filter);
				
			$results = $this->getUserDetails($filter,$fieldList,'GET');
				
			return $results;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	
	}
	
	public function getSolrSearchResultAutcomplete(){
		// Set field mapping for class level search
		try{
			$fieldList = array (
					'firstname:FullName' 
			);
			$filter = array ();
			$this->setAdminAccessFilters($filter,'user');
			$this->setSearchFilterAutoComplete($filter);
			
			$results = $this->getUserDetailsAutoComplete($filter,$fieldList,'GET');
			
			return $results;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	/**
	 * Setting up filters which entered in the UI
	 * @param $filter (Array)
	 * @return NA
	 */
	
	
	private function setSearchFilters(&$filter){
		
		$this->setOrgFilter($filter);
		$this->setDeptFilter($filter);
		$this->setUsrStatusFilter($filter);
		$this->setJobRoleFilter($filter);
		$this->setUsrTypeFilter($filter);
		$this->setLocFilter($filter);
		$this->setGroupFilter($filter);
		$this->setTextSearchFilter($filter);
		$this->setManagerFilter($filter);
		$this->setHireDateFilter($filter);
		$this->setRoleFilter($filter);
		$this->setEmplyTypeFilter($filter);
		
		// ExpertusAdmin should not be displayed
		$remove = '-id:1';
		array_push($filter,urlencode($remove));
	}
	
	private function setOrgFilter(&$filter){
		if($_REQUEST['userorg']){
			$org   = preg_replace("/[^a-zA-z]/", ' ', $_REQUEST['userorg']);
			$type = explode(' ',$org);
			foreach($type as $key=>$val){
				if($key==0)
					$orgfilter = '(Organization:*'.$val.'* OR DottedOrganizationName:*'.$val.'*)';
				else
					$orgfilter .= ' AND (Organization:*'.$val.'* OR DottedOrganizationName:*'.$val.'*)';
			}
			array_push($filter,urlencode($orgfilter));
		}
	}
	
	private function setDeptFilter(&$filter){
		if($_REQUEST['department']){
			$type = explode(' ',preg_replace("/[^a-zA-z]/", ' ', $_REQUEST['department']));
			foreach($type as $key=>$val){
				if($key==0)
					$departmentfilter = 'Department:*'.$val.'*';
				else
					$departmentfilter .= ' AND Department:*'.$val.'*';
			}
			array_push($filter,urlencode($departmentfilter));
		}
	}
	
	private function setUsrStatusFilter(&$filter){
		if($_REQUEST['userstatus']){
			$tatus = explode('|',$_REQUEST['userstatus']);
			foreach($tatus as $key=>$val){
				if($key==0)
					$statusfilter = 'Status:(*'.trim($val).'*';
				else
					$statusfilter .= ' OR *'.trim($val).'*';
			}
			$statusfilter   .= ')';
			array_push($filter,urlencode($statusfilter));
		}
	}
	
	private function setJobRoleFilter(&$filter){
		if($_REQUEST['userJobrole']){
			expDebug::dPrint('Joberoless'.print_r($_REQUEST['userJobrole'],true),4);
			$userJobrolefilter = 'JobRole:"*`'.str_replace(' ', '?', $_REQUEST['userJobrole']).'`*"';
			array_push($filter,urlencode($userJobrolefilter));
		}
	}
	
	private function setUsrTypeFilter(&$filter){
		if($_REQUEST['usertype']){
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname']))
				$search = 'UserTypeCode';
			else 
				$search = 'UserType';
			$usertypefilter = ''.$search.':"*'.str_replace(' ','?',$_REQUEST['usertype']).'*"';
			array_push($filter,urlencode($usertypefilter));
		}
	}
	
	private function setLocFilter(&$filter){
		if($_REQUEST['location']){
			$type = explode(' ',$_REQUEST['location']);
			foreach($type as $key=>$val){
				if($key==0)
								$locationfilter = '(City:*'.$val.'* OR State:*'.$val.'*)';
				else
								$locationfilter .= ' AND (City:*'.$val.'* OR State:*'.$val.'*)';
			}
			array_push($filter,urlencode($locationfilter));
		}
		if($_REQUEST['zipcode']){
			$zipfilter = 'ZipCode:*'.$_REQUEST['zipcode'].'*';
			array_push($filter,urlencode($zipfilter));
		}
		if($_REQUEST['phone_no']){
			$zipfilter = 'ContactNumber:*'.$_REQUEST['zipcode'].'*';
			array_push($filter,urlencode($zipfilter));
		}
	}
	
	private function setGroupFilter(&$filter){
		if($_REQUEST['group']){
			$group = explode('|',$_REQUEST['group']);
			foreach($group as $key=>$val){
				if($key==0){
					$grpfilter = 'AdminGrpId:'.trim($val).' OR LearnerGrpId:'.trim($val).'';
				}
				else{
					$grpfilter .= 'OR AdminGrpId:'.trim($val).' OR LearnerGrpId:'.trim($val).'';
				}
			}
			array_push($filter,urlencode($grpfilter));
		}
	}
	
	private function setTextSearchFilter(&$filter){
		if($_REQUEST['textfilter']){
			$_REQUEST['textfilter'] = str_replace('"', '\"', $_REQUEST['textfilter']);
			if(strpos($_REQUEST['textfilter'],"'") !== false){
				$type = explode("'",$_REQUEST['textfilter']);
				foreach($type as $key=>$val){
					if($key==0)
						$textfilter = '(UsernameSearch:"'.$val.'" OR Username:"'.$val.'")';
					else
						$textfilter .= ' AND (UsernameSearch:"'.$val.'" OR Username:"'.$val.'")';
				}
			}
			else 
				$textfilter = 'UsernameSearch:"'.$_REQUEST['textfilter'].'" OR Username:"'.$_REQUEST['textfilter'].'"';
			array_push($filter,urlencode($textfilter));
		}
	}
	
	private function setManagerFilter(&$filter){
		if($_REQUEST['mgrusername']){
			if(strpos($_REQUEST['mgrusername'],' ') !== false || strpos($_REQUEST['mgrusername'],'@') !== false  )
				$mgr = 'ManagerUserName:"*'.str_replace(' ','?',$_REQUEST['mgrusername']).'*"';
			else
				$mgr = 'ManagerUserName:*'.$_REQUEST['mgrusername'].'*';
			array_push($filter, urlencode($mgr));
			}
	}
	
	private function setHireDateFilter(&$filter){
	  //userhiredate	
			$hiredate = explode('|',$_REQUEST['userhiredate']);
			if($hiredate[0] || $hiredate[1]){
				expDebug::dPrint('Joberoless1'.print_r($hiredate,true),4);
			if ($hiredate [0]) {
				$start_date = date_format ( date_create_from_format ( 'm-d-Y', $hiredate [0] ), 'Y-m-d' );
				$start_date .= 'T00:00:00Z';
			} else
				$start_date = '*';
			if ($hiredate [1]) {
				$end_date = date_format ( date_create_from_format ( 'm-d-Y', $hiredate [1] ), 'Y-m-d' );
				$end_date .= 'T00:00:00Z';
			} else
				$end_date = '*';
			$date_range = '' . $start_date . ' TO ' . $end_date . '';
			$datefilter = 'HireDate:[' . $date_range . ']';
			array_push ($filter, urlencode($datefilter));
		}
	}
	
	private function setRoleFilter(&$filter){
		if($_REQUEST['roles']){
			$roles = explode(',',$_REQUEST['roles']);
			foreach ( $roles as $key => $val ) {
				if ($key == 0)
					$typefilter = 'UserRoles:*'.trim(ucfirst($val)).'*';
				else
					$typefilter .= ' OR UserRoles *'.trim(ucfirst($val)).'*';
		}
		array_push ($filter, urlencode($typefilter));
		}
	}
	
	private function setEmplyTypeFilter(&$filter){
		if($_REQUEST['empltype']){
			$empfilter = 'EmploymentTypeCode:'.$_REQUEST['empltype'].'';
			array_push($filter,urlencode($empfilter));
		}
	}
	/**
	 * Retrive search results from Solr
	 * @param $filter (Array) Search filters
	 * @param $fieldList (Array) Required fields
	 * @param $method (String) GET/POST
	 * @return (Object) Result set
	 */
	private function getUserDetails($filter,$fieldList,$method){
		global $Solr_User;
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$resBase = array(
				'id'=>null,
				'lastname'=>null,
				'firstname'=>null,
				'username'=>null,
				'email'=>null,
				'manager_id'=>null,
				'status_code'=>null,
				'addr1'=>null,
				'addr2'=>null,
				'city'=>null,
				'statecode'=>null,
				'countrycode'=>null,
				'preferredlanguagecode'=>null,
				'timezonecode'=>null,
				'register_sms'=>null,
				'orgname'=>null,
				'dottedmanagersid'=>null,
				'object_type'=>'People',
				'sumedit'=>null,
				'sumdelete'=>null,
		);
		
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				$apiFields = array(
					'phone_no'=>null,
					'mobile_no'=>null,
					'hire_date'=>null,
					'otherorganisationid'=>null,
					'othermanagerid'=>null,
					'roles'=>null,
					'zipcode'=>null,
					'mobile_no'=>null,
					'currency'=>null,
					'is_verified_add'=>null,
					'is_valid_add'=>null,
					'invalid_add_reason'=>null,
					'currency_symbol'=>null,
					'currency_code'=>null,
					'usertype'=>null,
					'empltype'=>null,
					'employeeno'=>null,
					'jobrole'=>null,
					'jobtitle'=>null,
					'deptcode'=>null,
				);
				unset($resBase['dottedmanagersid']); // Field names for API and WEB are different hence removeing for api
				unset($resBase['object_type']);
				unset($resBase['sumedit']);
				unset($resBase['sumdelete']);
				$resBase = array_merge($resBase,$apiFields);
				$dispCols=explode(',',$_REQUEST['display_cols']);
				foreach($fieldList as $field=>$val){
					$acFld = explode(':',$val);
					if(!in_array($acFld[0],$dispCols)){
						unset($fieldList[$field]);
						unset($resBase[$acFld[0]]);
					}
				}
			}
			
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
	
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
	
		// Append solr additional params
		// SortBy filter
		if($_REQUEST['sortby'] && $_REQUEST['sortby']!='NewlyListed')
			$sortby = $_REQUEST['sortby'] == 'AZ'?'LastNameSrt+asc,FirstNameSrt+asc':'LastNameSrt+desc,FirstNameSrt+desc';
		else
			$sortby = 'UpdatedOn+desc,UID+asc';
		
		$rows = 10;
		if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
			$sortby = 'UID+asc';
			$start = ($_REQUEST['rows']*$_REQUEST['limit'])-$_REQUEST['limit'];
			$rows = !empty($_REQUEST['limit']) ? $_REQUEST['limit'] : $rows;
		}else{
			$start = ($_REQUEST['page']*$_REQUEST['rows'])-$_REQUEST['rows'];
			$rows = !empty($_REQUEST['rows']) ? $_REQUEST['rows'] : $rows;
		}
		$start = $start < 0 ? 0 : $start;
		$data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
		try{
			$srcRst = $this->getData($this->collName,$data,$method);
	
			if(isset($_REQUEST['apiname']) && !empty($_REQUEST['apiname'])){
				return $this->apiResponseHandler($srcRst,'userlist',$resBase);
			}

			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
		
			// Logged in users details
			if(empty($Solr_User)){
				$results = $this->getLoggedUserDetails();
			}else{
				$results = $Solr_User;
			}
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$this->modifyResultSet($tdoc,$results);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst2,true),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	
	private function getUserDetailsAutoComplete($filter,$fieldList,$method){
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		$resBase = array(
				'firstname'=>NULL
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		$rows = 1000;
		$start = 0;
		$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows;
		
		try{
			$srcRst = $this->getData($this->collName,$data,$method);
			expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst,true),4);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst2,true),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	private function setSearchFilterAutoComplete(&$filter){
		expDebug::dPrint("setSearchFilterAutoComplete ".$_REQUEST['z'],4);
		$searchCurText = str_replace('"', '\"', $_REQUEST['z']);
		if(strpos($searchCurText,"'") !== false){
			$type = explode("'",$searchCurText);
			foreach($type as $key=>$val){
				if($key==0)
					$search = '(UsernameSearch:"'.$val.'" OR Username:"'.$val.'")';
				else
					$search .= ' AND (UsernameSearch:"'.$val.'" OR Username:"'.$val.'")';
			}
		}
		else {
			$search = 'UsernameSearch:"'.$searchCurText.'" OR Username:"'.$searchCurText.'"';
		}
		array_push ($filter, urlencode($search));
	}
	private function modifyResultSet(&$tdoc,$results){
		if($tdoc['dottedorg'])
			$this->restOrg($tdoc);
		// Set Edit & Delete privilege
		$this->setPrivileges($tdoc,$results,'cre_usr');
	}
    
	
	private function restOrg(&$tdoc){
		$dot_org = implode(',',array_unique(explode(',',$tdoc['dottedorg'])));
		$tdoc['orgname'] .= ','.$dot_org;
		unset($tdoc['dottedorg']);
	}
	
	
	// solr-- Group Add User begins here
	public function getSolrSearchResultGroupAddUsers($entityId,$searchType){
	    try{
	        // Set field mapping for class level search
	        $fieldList = array(
	                'user_id:id',
	                'uname:Username',
	                'fname:FullName',
	                'Privilege',
	                'CreatedBy',
	                'UpdatedBy'
	        );
	
	        // Setting required filters
	        $filter = array();
	        expDebug::dPrint ('&*()_%#$1234');
	        $this->setAdminAccessFilters($filter,'user');
	        $this->setSearchFiltersGroupAddUsers($filter,$searchType,$entityId);
	        $results = $this->getUserDetailsGroupAddUsers($filter,$fieldList,'GET');
	
	        return $results;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	       
	private function setSearchFiltersGroupAddUsers(&$filter, $searchType,$entityId){
	    $this->setNameSearchFilter($filter, $searchType);
	    // ExpertusAdmin should not be displayed
	    $remove = '-id:1';
	    array_push($filter,urlencode($remove));	     
	    
	    $this->excludeGroupId($filter,$entityId);
	}
	
	private function excludeGroupId(&$filter,$entityId){
		$textfilter = '-LearnerGrpId:'.$entityId.' OR -AdminGrpIdsList:'.$entityId;
	//	$textfilter = '-LearnerGrpId:'.$entityId.' OR -AdminGrpId:'.$entityId;
    expDebug::dPrint('text filter value 1234'.print_r($textfilter,true),4);
    array_push($filter,urlencode($textfilter));
	}
	
	private function setNameSearchFilter(&$filter, $searchType){
	    if($_REQUEST['q']){
	        expDebug::dPrint('q filter value'.print_r($_REQUEST['q'],true),4);
	        $data = explode('/',$_REQUEST['q']);
	
	        expDebug::dPrint('search type value'.print_r($searchType,true),4);
	        expDebug::dPrint('data filter value'.print_r($data,true),4);
	        $textfiltervalue = $data[4];
	        expDebug::dPrint('text filter value'.print_r($textfiltervalue,true),4);
	        if(!empty($textfiltervalue)){
	            if($textfiltervalue != 'Type a Username' && $textfiltervalue != 'Type Full Name'){
	                if($searchType == 'usrtit' || $searchType == 'usrtitle'){
	
	                    if(strpos($textfiltervalue,"'") !== false){
	                        $type = explode("'",$textfiltervalue);
	                        expDebug::dPrint('type valueeee'.print_r($type,true),4);
	                        foreach($type as $key=>$val){
	                            if($key==0)
	                                $textfilter = '( Username:"'.$val.'")';
	                            else
	                                $textfilter .= ' AND ( Username:"'.$val.'")';
	                        }
	                    }
	                    else
	                        $textfilter = ' Username:"'.$textfiltervalue.'"';
	                }
	                else{
	
	                    if(strpos($textfiltervalue,"'") !== false){
	                        $type = explode("'",$textfiltervalue);
	                        foreach($type as $key=>$val){
	                            if($key==0)
	                                $textfilter = '( FullName:"'.$val.'")';
	                            else
	                                $textfilter .= ' AND (FullName:"'.$val.'")';
	                        }
	                    }
	                    else
	                        $textfilter = 'FullName:"'.$textfiltervalue.'"';
	
	
	                }
	            }
	            expDebug::dPrint('text filter value 1234'.print_r($textfilter,true),4);
	            array_push($filter,urlencode($textfilter));
	        }
	    }
	}
	
	private function getUserDetailsGroupAddUsers($filter,$fieldList,$method,$call=''){
		global $Solr_User;
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'user_id'=>null,
	            'uname'=>null,
	            'fname'=>null,
	            'sumEdit'=>null,
	            'sumDelete'=>null
	    );
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	
	    expDebug::dPrint("Data after filters applying".print_r($data,true),4);
	    // Append solr additional params
	    // SortBy filter
	        $sidx = $_REQUEST['sidx'];
	        $sord = $_REQUEST['sord'];
	        if(!empty($sidx)) {
	            if($sidx == 'full_name') {
	                $sort = 'FullNameSrt+'.$sord;
	            }
	            else if($sidx == 'user_name') {
	                $sort = 'UsernameSrt+'.$sord;
	            }
	           
	        } else {
	            $sort = 'FullNameSrt+desc';
	        }
	
	    if($call == 'autocomplete'){
	        $rows = 1000;
	        $start = 0;
	    }
	    else{
	        $rows = ($_REQUEST['rows']) ? $_REQUEST['rows'] : 5;
	        $start = ($_REQUEST['page'] * $_REQUEST['rows']) - $_REQUEST['rows'];
	        $start = $start < 0 ? 0 : $start;
	    }
        $data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
	    try{
	        $srcRst = $this->getData($this->collName,$data,$method);
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
			    // Logged in users details
					if(empty($Solr_User)){
						$results = $this->getLoggedUserDetails();
					}else{
						$results = $Solr_User;
					}
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            if($call != 'autocomplete')
	                $this->modifyResultSet($tdoc,$results);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	        expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst2,true),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	}
	public function getSolrSearchResultGroupAddUsersAutoComplete($searchType,$entityId){
	    try{
	        // Set field mapping for class level search
	        $fieldList = array(
	                'user_id:id',
	                'uname:Username',
	                'fname:FullName',
	
	        );
	        expDebug::dPrint('searchType type12'.print_r($searchType,true),4);
	         
	        // Setting required filters
	        $filter = array();
	        $this->setAdminAccessFilters($filter,'user');
	        $this->setSearchFilterGroupAddUsersAutoComplete($filter,$searchType);
	        $this->excludeGroupId($filter,$entityId);
	        $results = $this->getUserDetailsGroupAddUsers($filter,$fieldList,'GET','autocomplete');
	
	        return $results;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}

	private function setSearchFilterGroupAddUsersAutoComplete(&$filter,$searchType){
	    expDebug::dPrint("setSearchFilterAutoComplete ".$_REQUEST['z'],4);
	    if($_REQUEST['z']){
	        $searchCurText = str_replace('"', '\"', $_REQUEST['z']);
	        expDebug::dPrint('searchType type function in'.print_r($searchType,true),4);
	
	        if($searchType == 'usrtit' || $searchType == 'usrtitle'){
	            if(strpos($searchCurText,"'") !== false){
	                $type = explode("'",$searchCurText);
	                foreach($type as $key=>$val){
	                    if($key==0)
	                        $search = '( Username:"'.$val.'")';
	                    else
	                        $search .= ' AND (Username:"'.$val.'")';
	                }
	            }
	            else {
	                $search = ' Username:"'.$searchCurText.'"';
	            }
	        }
	        else{
	            if(strpos($searchCurText,"'") !== false){
	                $type = explode("'",$searchCurText);
	                foreach($type as $key=>$val){
	                    if($key==0)
	                        $search = '(FullName:"'.$val.'")';
	                    else
	                        $search .= ' AND ( FullName:"'.$val.'")';
	                }
	            }
	            else {
	                $search = ' FullName:"'.$searchCurText.'"';
	            }
	        }
	        expDebug::dPrint('search value before push'.print_r($search,true),4);
	        array_push ($filter, urlencode($search));
	    }
	}
	/* ** Group Add user ends here ** */
	
	// group added users begins here
	public function getSolrSearchResultGroupAddedUsers($entityId){
	    try{
	        // Set field mapping for class level search
	        $fieldList = array(
	                'user_id:id',
	                'uname:Username',
	                'status:StatusName',
	                'fname:FullName',
	        );
	         
	        // Setting required filters
	        $filter = array();
	        $this->setAdminAccessFilters($filter,'user');
	        //$this->setAccessFilters($filter);
	        $this->setSearchFiltersGroupAddedUsers($filter,$entityId);
	        $results = $this->getUserDetailsGroupAddedUsers($filter,$fieldList,'GET');
	
	        return $results;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	
	private function setSearchFiltersGroupAddedUsers(&$filter,$entityId){
	    expDebug::dPrint('entityId valueee'.print_r($entityId,true),4);
	    $this->setFilterGroupsAddedUser($filter,$entityId);
	    $this->setNameSearchFilter($filter, $searchType);
	    // ExpertusAdmin should not be displayed
	    $remove = '-id:1';
	    array_push($filter,urlencode($remove));
	}
	private function setFilterGroupsAddedUser(&$filter,$entityId){
	    $textfilter = 'LearnerGrpId:'.$entityId.' OR AdminGrpIdsList:'.$entityId;
	    array_push($filter,urlencode($textfilter));
	}
	private function getUserDetailsGroupAddedUsers(&$filter,$fieldList,$method,$from=''){
	    // List of required fields to be set as NULL,
	    // because Solr will not keep any null fields
	    $resBase = array(
	            'user_id'=>null,
	            'uname'=>null,
	            'status'=>null,
	            'fname'=>null,
	
	    );
	     
	    // Append field list
	    $data = 'fl=' . implode(',',$fieldList);
	
	    // Append filters
	    $data .= '&fq=' . implode('&fq=',$filter);
	
	    // Append solr additional params
	    // SortBy filter
	    if($_REQUEST['sord'] == 'asc')
	        $sortby = 'FullNameSrt+asc';
	    else if($_REQUEST['sord'] == 'desc')
	        $sortby = 'FullNameSrt+desc';
	    else
	        $sortby = 'FullNameSrt+desc';
	
	    if($from == 'autocomplete'){
	        $rows = 1000;
	        $start = 0;
	    }
	    else{
	        $rows = ($_REQUEST['rows']) ? $_REQUEST['rows'] : 5;
	        $start = ($_REQUEST['page']*$_REQUEST['rows'])-$_REQUEST['rows'];
	        $start = $start < 0 ? 0 : $start;
	    }
	
	    $data .= '&sort='.$sortby.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
	    try{
	        $srcRst = $this->getData($this->collName,$data,$method);
	        $srcRst1 = $srcRst['response']['docs'];
	        $srcRst2 = array();
	        $srcRst2['recCount'] = $srcRst['response']['numFound'];
	        foreach($srcRst1 as $doc){
	            $tdoc  = array_merge($resBase, $doc);
	            $srcRst2['records'][] = $this->arrayToObject($tdoc);
	        }
	        expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst2,true),4);
	        return $srcRst2;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	}
	
	// solr group added users autocomplete begins here
	public function getSolrSearchResultGroupAddedUsersAutoComplete($searchType,$entityId){
	    try{
	        // Set field mapping for class level search
	        $fieldList = array(
	                'user_id:id',
	                'uname:Username',
	                'fname:FullName',
	
	        );
	        expDebug::dPrint('searchType type12'.print_r($searchType,true),4);
	        // Setting required filters
	        $filter = array();
	        $this->setAdminAccessFilters($filter,'user');
	        $this->setFilterGroupsAddedUser($filter,$entityId);
	        $this->setSearchFilterGroupAddUsersAutoComplete($filter,$searchType);
	        $results = $this->getUserDetailsGroupAddUsers($filter,$fieldList,'GET','autocomplete');
	
	        return $results;
	    }catch(Exception $e){
	        expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
	    }
	
	}
	// solr group added users autocomplete ends here
	/** Filter in Enrollment screen -- TP & Class Enrollments **/
	public function getSolrSearchResultClassEnrollments($val_filter,$from,$entityId,$enrType){
		// Set field mapping for class level search
		if($from == 'enrollments'){
			$fieldList = array(
					'User_id:id'
			);
		}
		else { // Have to handle for path
			$searchType = $_REQUEST['search_type'];
			if($searchType == 'user'){
				$fieldList = array(
						'name:Username'
				);
			}
			if($searchType == 'fullname'){
				$fieldList = array(
						'name:FullName'
				);
			}
			if($searchType == 'org'){
				$fieldList = array(
						'name:Organization'
				);
			}
			if($searchType == 'jobrole'){
				$fieldList = array(
						'name:JobRole'
				);
			}
			if($searchType == 'usertype'){
				$fieldList = array(
						'name:UserType'
				);
			}
			if($searchType == 'manager'){
				$fieldList = array(
						'name:ManagerName'
				);
			}
			if($searchType == 'group'){
				$fieldList = array(
						'name:LearnerGrpName'
				);
			}
			//array_merge($fieldList1,$fieldList);
		}		
		$filter = array();
		array_push ($filter, urlencode($val_filter));
		/** fetch user id based on enroll ids **/
		
		$results = $this->getUserDetailsOfEnrolled($filter,$fieldList,'GET');
 		return $results;
		
	}
	
	private function getUserDetailsOfEnrolled($filter,$fieldList,$method){
		$resBase = array(
				'User_id'=>NULL,
				'name'=>NULL
		);
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
		$rows = 1000;
		$start = 0;
		$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows;
	
		try{
			$srcRst = $this->getData($this->collName,$data,$method);
			expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst,true),4);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$tdoc = $this->removeTilt($tdoc);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("tOTAL in Solr Catalog Search buncgh-- ".print_r($srcRst2,true),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	//solr Enroll User begins here
	public function getSolrSearchResultEnrollUser($searchKeyword,$searchType,$entityId,$enrPage){
		try{
			// Set field mapping for class level search
			$fieldList = array(
					'id:id',
					'user_name:Username',
					'full_name:FullName',
					'manager_name:ManagerName',
					'organization_name:Organization',
	
			);
			// Setting required filters
			$filter = array();
			$this->setAdminAccessFilters($filter,'user');
			//$this->setAccessFilters($filter);
            $this->setSearchFiltersEnrollUser($filter,$searchKeyword,$searchType,$entityId,$enrPage);
			$results = $this->getUserDetailsEnrollUser($filter,$fieldList,'GET');	
			return $results;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	private function setAccessFilters(&$filter){
		global $Solr_User;
		$filter = array();
		if(empty($Solr_User)){
			// Setting group filters
			require_once $_SERVER['DOCUMENT_ROOT'].'/sites/all/modules/core/exp_sp_solr/exp_sp_solr_user.php';
			$userObj = new UserSolrSearch();
			$userDetails = $userObj->getLoggedUserDetails();
		}else{
			$userDetails = $Solr_User;
		}
		$grpStr = '';
		$uid = $userDetails[0]->id;
		$user_hire = $userDetails[0]->HireDate;
		$user_name = $userDetails[0]->username;
		expDebug::dPrint ( 'Learner group for userss' . print_r ( $userDetails, true ), 4 );
		if($userDetails[0]->learner_groups != ''){
			$userAdminGrp = explode(',',$userDetails[0]->learner_groups);
			expDebug::dPrint ( 'Learner group for usersssssssss' . print_r ( $userDetails, true ), 4 );
			foreach($userAdminGrp as $key=>$val){
				if($key==0)
					$grpStr = 'LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
				else
					$grpStr .= ' OR LnrGroupName:"*'.trim(str_replace(' ','?',$val)).'*"';
			}
			$grpStr .= ' OR (LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
		}
	
		else
			$grpStr = '(LnrGroupName:"*Open_Entity*" AND LnrAccessUserName:"*Open_Entity*")';
	
		expDebug::dPrint ( 'grpStr for groupsss' . print_r ( $grpStr, true ), 4 );
		if($user_name)
			$grpStr .= ' OR LnrAccessUserName:"*`'.$user_name.'`*"';
	
		expDebug::dPrint ( 'grpStr for groupsss12345' . print_r ( $grpStr, true ), 4 );
		$filter[] = urlencode($grpStr);
		if($user_hire){
			$hire_date_filter = $this->setHireDateFilter($user_hire);
			array_push($filter, urlencode($hire_date_filter));
		}
	}
	private function setSearchFiltersEnrollUser(&$filter,$searchKeyword,$searchType,$entityId,$enrPage){
	
		expDebug::dPrint('searchKeyword valueee'.print_r($searchKeyword,true),5);
 		if($searchType == 'user')
			$set = 'Username:"';
		else if($searchType == 'fullname')
			$set = 'FullName:"';
		if($searchType == 'org')
			$set = 'Organization:"';
		if($searchType == 'jobrole')
			$set = 'JobRole:"';
		if($searchType == 'usertype')
			$set = 'UserType:"';
		if($searchType == 'manager')
			$set = 'ManagerName:"';
		if($searchType == 'group')
			$set = 'LearnerGrpName:"';
		
	    $typeNameStr = t('LBL036').' '.t('LBL107');
	    $orgStr = t('LBL036').' '.t('LBL137');
	    $jobroleStr = t('LBL036').' '.t('LBL133').' '.t('LBL137');
	    $userTypeStr = t('LBL036').' '.t('LBL173');
	    $mangStr = t('LBL036').' '.t('LBL134');
 		//need to handle for multi language
		if($searchKeyword != "" && $searchKeyword != t('LBL181') && $searchKeyword != $typeNameStr && $searchKeyword != $orgStr && $searchKeyword != $jobroleStr && $searchKeyword != $userTypeStr && $searchKeyword !=$mangStr && $searchKeyword !=t('LBL1270') ){
 			expDebug::dPrint("Call to excludedID auto complete function  sdfsdgfsdg".$set,5 );
			$this->setFilterUser($filter,$set,$searchKeyword);
		}
 		$this->setExcludedIDUserFilter($filter,$entityId,$enrPage);
		// ExpertusAdmin should not be displayed
		$remove = '-id:1';
		array_push($filter,urlencode($remove));
		$remove1 = 'Status:cre_usr_sts_atv';
		array_push($filter,urlencode($remove1));
 	}
	
	
	private function setExcludedIDUserFilter(&$filter,$entityId,$enrPage){
		expDebug::dPrint("Call to excludedID entitytyty ".$entityId,5);
		if(!empty($enrPage)) {
			if($enrPage == 'class')
				$excludeduser = '-EnrClassIds:'.$entityId;
			else if($enrPage == 'tp')
				$excludeduser = '-EnrProgramIds:'.$entityId;
			array_push($filter,urlencode($excludeduser));
		}
		
	}
	private function setFilterUser(&$filter, $set, $searchKeyword){
			if(strpos($searchKeyword,"'") !== false){
				$type = explode("'",$searchKeyword);
				foreach($type as $key=>$val){
					if($key==0)
						$textfilter = $set.$val.'"';
					else
						$textfilter .= ' AND '.$set.$val.'"';
				}
			}
			else
				$textfilter = $set.$searchKeyword.'"';
		
	
		//return $text;
		expDebug::dPrint('text filter value 1234'.$textfilter,5);
		array_push($filter,urlencode($textfilter));
	}
	
	private function getUserDetailsEnrollUser($filter,$fieldList,$method,$from=''){
 		global $Solr_User;
		// List of required fields to be set as NULL,
		// because Solr will not keep any null fields
		if($from == 'autocomplete'){
			$resBase = array(
					'name'=>null,
			);
		}
		else{
			$resBase = array(
					'id'=>null,
					'user_name'=>null,
					'full_name'=>null,
					'manager_name'=>null,
					'organization_name'=>null
			);
		}
		// Append field list
		$data = 'fl=' . implode(',',$fieldList);
	
		// Append filters
		$data .= '&fq=' . implode('&fq=',$filter);
	
		// Append solr additional params
		// SortBy filter
		$sidx = $_REQUEST['sidx'];
		$sord = $_REQUEST['sord'];
		if(!empty($sidx)) {
			if($sidx == 'user_name') {
				$sort = 'UsernameSrt+'.$sord;
			}
			if($sidx == 'full_name') {
				$sort = 'FullNameSrt+'.$sord;
			}
			else if($sidx == 'manager_name') {
				$sort = 'ManagerNameSrt+'.$sord;
			}
			else if($sidx == 'organization_name') {
				$sort = 'OrganizationSrt+'.$sord;
			}
		} else {
			$sort = 'FullNameSrt+desc';
		}
		
		if($from == 'autocomplete'){
			$rows = 1000;
			$start = 0;
		}
		else{
			$rows = ($_REQUEST['rows']) ? $_REQUEST['rows'] : 10;
			$start = ($_REQUEST['page']*$_REQUEST['rows'])-$_REQUEST['rows'];
			$start = $start < 0 ? 0 : $start;
		}
			
		$data .= '&sort='.$sort.'&indent=on&q=*:*&start='.$start.'&rows='.$rows;
		//$data .= '&indent=on&q=*:*&start='.$start.'&rows='.$rows;
		try{
			$srcRst = $this->getData($this->collName,$data,$method);
			$srcRst1 = $srcRst['response']['docs'];
			$srcRst2 = array();
			$srcRst2['recCount'] = $srcRst['response']['numFound'];
			// Logged in users details
			if(empty($Solr_User)){
				$results = $this->getLoggedUserDetails();
			}else{
				$results = $Solr_User;
			}
			foreach($srcRst1 as $doc){
				$tdoc  = array_merge($resBase, $doc);
				$this->modifyResultSet($tdoc,$results);
				$srcRst2['records'][] = $this->arrayToObject($tdoc);
			}
			expDebug::dPrint("tOTAL in Solr Catalog Search -- ".print_r($srcRst2,true),4);
			return $srcRst2;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	
	// solr -- Enroll User ends here
	
	
	// Solr Enroll User AutoComplete begins here //
	public function getSolrSearchResultEnrollUserAutoComplete($searchType,$nameSubstr,$entityId,$enrPage){
		try{
			expDebug::dPrint ( 'search typeeeeeeee' . print_r ($searchType, true ), 5 );
			// Set field mapping for class level search
			if($searchType == 'user'){
				$fieldList = array(
						'name:Username',
				);
			}
			if($searchType == 'fullname'){
				$fieldList = array(
						'name:FullName',
				);
			}
			if($searchType == 'org'){
				$fieldList = array(
						'name:Organization',
				);
			}
			if($searchType == 'jobrole'){
				$fieldList = array(
						'name:JobRole',
				);
			}
			if($searchType == 'usertype'){
				$fieldList = array(
						'name:UserType',
				);
			}
			if($searchType == 'manager'){
				$fieldList = array(
						'name:ManagerName',
				);
			}
			if($searchType == 'group'){
				$fieldList = array(
						'name:LearnerGrpName',
				);
			}
	
	
			// Setting required filters
			$filter = array();
			$this->setAdminAccessFilters($filter,'user');
			//$this->setAccessFilters($filter);
			$this->setSearchFiltersEnrollUser($filter,$nameSubstr,$searchType,$entityId,$enrPage);
			$results = $this->getUserDetailsEnrollUser($filter,$fieldList,'GET','autocomplete');	
			return $results;
		}catch(Exception $e){
			expDebug::dPrint("Error in Solr Catalog Search -- ".$e->getMessage(),1);
		}
	}
	}
?>