<?php
/**
* @class or PHP Name		: Autocomplete Service
* @author(s)				: Product Team
* Version         			: 1.0
* Date						: 29/09/2009
* PHP Version          		: 5.2.6
* Description     			: This service is used to get Autocomplete search in SmartPortal LMS.
*/
//include_once "../dao/CatalogDAO.php";
//include_once "../dao/UserDAO.php";
//include_once "../dao/OrganizationDAO.php";
//include_once "../dao/CertMasterDAO.php";
//include_once "../dao/ContentMasterDAO.php";
//include_once  "../dao/ResourceDAO.php";
//include_once  "../dao/TagsDAO.php";
//include_once  "../dao/TagsAdminDAO.php";
include_once  "../dao/learner/LnrSearchDAO.php";
//include_once  "../dao/ReportConfigDAO.php";
//include_once "../dao/AnnouncementMasterDAO.php";
//include_once "../dao/SecurityManagementServiceDAO.php";
//include_once "../dao/ReportDAO.php";
//include_once "../dao/SurveyDAO.php";
//include_once "../dao/SurveyQuestionDAO.php";
//include_once "../dao/ReportThemeDAO.php";
include_once "../dao/NotificationDAO.php";
//include_once  "../dao/learner/LnrTrainingProgramDAO.php";
//include_once "../dao/TrainingProgramDAO.php";

include_once "ServiceBase.php";



class Autocomplete extends ServiceBase{
	function __construct(){$this->process();}

	function process(){
		try{
		$searchtype=isset($_GET['type']) ? $_GET['type'] : '';
		$searchsubtype=isset($_GET['subtype']) ? $_GET['subtype'] : '';
		$searchautotype=isset($_GET['autotype']) ? $_GET['autotype'] : '';
		$searchkey=isset($_GET['z']) ? $_GET['z'] : '';
		$location=isset($_GET['id']) ? $_GET['id'] : '';
		$iscluetip=isset($_GET['isClueTip']) ? $_GET['isClueTip'] : '';
		$subval=isset($_GET['subval']) ? $_GET['subval'] : '';
		$sublvl=isset($_GET['sublvl']) ? $_GET['sublvl'] : '';
		$userId=isset($_GET['userId']) ? $_GET['userId'] : '';
		$status=isset($_GET['status']) ? $_GET['status'] : '';
		$entityType=isset($_GET['entityType']) ? $_GET['entityType'] : '';
		expDebug::dPrint('$searchtype '.$searchtype.' $searchsubtype '.$searchsubtype.' $searchkey '.$searchkey.' $iscluetip '.$iscluetip.' $subval '.$subval.' $userId '.$userId.' $status '.$status.' $entityType '.$entityType);
		switch($searchtype) {
			case 'folks':

				$dao = new TagsDAO();
				$vText = $dao->getTagsAutoCompleteDetails($searchkey, $searchtype, $searchsubtype);
                print $vText;
				break;
			case 'tags':

				$dao = new TagsAdminDAO();
				$vText = $dao->getTagsAdminAutoCompleteDetails($searchkey, $searchtype, $searchsubtype);
                print $vText;
				break;
			case 'tag':

				$dao = new TagsAdminDAO();
				$results = $dao->getTagAdminAutoCompleteDetails($searchkey, $searchtype, $searchsubtype);
        //        print $vText;
				if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							$val='';
							$type='';
							foreach($results[$i] as $key=>$value){
								if(strtolower($key)==strtolower('tagname')){
									$val.=$value;
								}
							}
							//print $val." - <b>".$type."</b>\n";
							print $val."\n";
						}
				    }
				break;

			case 'ResourceClassRoom':
				$locid = $_GET["locationId"];
				$classid=isset($_GET['classroomId'])?$_GET['classroomId']:'';
				//print $locid;
				//die;
				$dao = new ResourceDAO();
				//$oResults = $dao->getResourceClassroomAutoCompleteDetails($searchtype, $searchsubtype, $searchkey);
				$oResults = $dao->getResourceClassroomAutoCompleteDetails($locid,$searchkey);
				 	//print_r($oResults)."ssds";
		           if($oResults != null){
					for($i=0;$i<sizeOf($oResults);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						//print $oResults[$i]->Cid."|".$oResults[$i]->FName.'-'.$oResults[$i]->CName."\n";
						//print $oResults[$i]->Cid."|".$oResults[$i]->CName."|".$oResults[$i]->MaxCapacity."\n";
						print $oResults[$i]->Cid."|".$oResults[$i]->CName."\n";
					}
		           }
					break;
				/*if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$resultText = $this->toXML('ClueTip',$oResults,'');
				 }else {
				$resultText = $dao->getClassAutoCompleteTextFormat($oResults,$searchsubtype);
				 }
				print $resultText;
				break;	*/

			case 'ResourceFacility':

				    $locid = $_GET["locationId"];
					$dao = new ResourceDAO();
					$oResults = $dao->getResourceFacilityAutoCompleteDetails($locid,$searchkey);
					if($oResults != null){
					for($i=0;$i<sizeOf($oResults);$i++){
						print $oResults[$i]->FId."|".$oResults[$i]->FName."\n";
					}
					}
				 break;	
					
            case 'ResourceFacilityDropDown':
				    $locid = $_GET["locationId"];
				    $retXml = "";
					$dao = new ResourceDAO();
					$oResults = $dao->getResourceFacilityDropDownDetails($locid);
					if($oResults != null){
					  
					  $retXml .= "<ResourceFacility>";
					  $fc=0;
					  for($i=0;$i<sizeOf($oResults);$i++){					    
						//print $oResults[$i]->FId."|".$oResults[$i]->FName."\n";
						if($fc!=$oResults[$i]->FId){
						    if($fc !=0)
						      $retXml .= "</Facility>";
    					    $retXml .= "<Facility>";						
    			            $retXml .= "<FacilityId>".  $oResults[$i]->FId ."</FacilityId>";
    			            $retXml .= "<FacilityName>".  $oResults[$i]->FName ."</FacilityName>";
    			            $fc=$oResults[$i]->FId;
						}
			            $retXml .= "<ClassRoom>";						
			            $retXml .= "<ClassRoomId>".  $oResults[$i]->CId ."</ClassRoomId>";
			            $retXml .= "<ClassRoomName>".  $oResults[$i]->CName ."</ClassRoomName>";
			            $retXml .= "</ClassRoom>";
			            
  				    }
  				    $retXml .= "</Facility>";					  
					$retXml .= "</ResourceFacility>";
					  
					  print $retXml;
				    }
				 break;
				 
				 	
			case 'ResourceLocationSearch':

				$dao = new ResourceDAO();
				
				$oResults = $dao->getResourceLocationAutoCompleteDetails($searchtype,$searchsubtype, $searchkey );
				//	print $searchkey."ddd";
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$resultText = $this->toXML('ClueTip',$oResults,'');
				 }else {
				//$resultText = $dao->getAutoCompleteTextFormatTitle($oResults,$searchsubtype);
				$resultText = "";
					for($i=0;$i<count($oResults);$i++){
						if($searchsubtype == 'loc_name')
							$resultText .= $oResults[$i]->Title . " - <b>". $oResults[$i]->Type . "</b> \n ";
						else
							$resultText .= $oResults[$i]->Code . "\n ";
					}
				 }
			print $resultText;
			 break;

			 case 'ResourceLocationSearchLnrView':
				$searchtype="ResourceLocation";
				$dao = new ResourceDAO();
				$oResults = $dao->getResourceLocationAutoCompleteDetails($searchtype,$searchsubtype, $searchkey );
				//	print $searchkey."ddd";
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$resultText = $this->toXML('ClueTip',$oResults,'');
				 }else {
				//$resultText = $dao->getAutoCompleteTextFormatTitle($oResults,$searchsubtype);
				$resultText = "";
					for($i=0;$i<count($oResults);$i++){
						if($searchsubtype == 'loc_name')
							$resultText .= $oResults[$i]->Title . " - <b>". $oResults[$i]->Type . "</b> \n ";
						else
							$resultText .= $oResults[$i]->Code . "\n ";
					}
				 }
			print $resultText;
			 break;
			 
			 
			case 'ResourceLocation':

				$dao = new ResourceDAO();
				$oResults = $dao->getResourceLocationAutoCompleteDetails($searchtype,$searchsubtype, $searchkey );
				//	print $searchkey."ddd";
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$resultText = $this->toXML('ClueTip',$oResults,'');
				 }else {
				$resultText = $dao->getAutoCompleteTextFormat($oResults,$searchsubtype);
				 }
				print $resultText;

				break;

			case "course_lnr" :
				$param->course = true; //Follows through
				$dao= new CatalogDAO();
				if($searchsubtype == 'code'){
					$param->code=$searchkey;
				} elseif($searchsubtype == 'tag') {
					$param->tag=$searchkey;
				}else{
					$param->title=$searchkey;
				}
				$searchtype='course';
				$results=$dao->findAutocompleteLnr($param,$searchtype,$searchautotype);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					echo $outData;
				} else {
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							print $results[$i]->Title."\n";
						}
				    }
				}
				break;
			case "objecttype_lnr_search_autocomplate":
				$dao= new LnrSearchDAO();
				$param->course = true;
				$param->title=$searchkey;
				$searchtype='class';
				$results=$dao->findAutocompleteLnrSearch($param,$searchtype);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					echo $outData;
				} else {
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							print $results[$i]->Title."\n";
						}
				    }
				}
			break;
			case "course" :
				$param->course = true; //Follows through

			case "Catalog" :
			case "catalog"  :
				expDebug::dPrint("Catalog dao autocomplete is calling");
				$dao= new CatalogDAO();
				if($searchsubtype == 'code'){
					$param->code=str_replace("'","''",$searchkey);
				} elseif($searchsubtype == 'tag') {
					$param->tag=str_replace("'","''",$searchkey);
				}else{
					$param->title=str_replace("'","''",$searchkey);
				}
				$results=$dao->findAutocomplete($param,$searchtype);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
					echo $outData;
				} else {
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							$val='';
							$type='';
							$dt='';
							foreach($results[$i] as $key=>$value){
								if(strtolower($key)==strtolower($searchsubtype)){
									$val.=$value;
								}
								if(strtolower($key)==strtolower('Type')){
									$type.=$value;
								}
								if(strtolower($key)==strtolower('DT')){
									$dt.=$value;
								}
							}
							$show=$val." - <b>".$type."</b>".($dt != null && $dt != '' ? " - <b>".$dt."</b>" : "");
							//print $val." - <b>".$type."</b>|".$val."\n";
							$typeval= (str_word_count($type)>=1)?' - <b>'.$type.'</b>':'';
							print $val.$typeval."\n";
							//print $val." - <b>".$type."</b>\n";
						}
				    }
				}
				break;
			case "class" :
				$dao= new CatalogDAO();
				if($searchsubtype == 'code'){
					$param->code=$searchkey;
				} else {
					$param->title=$searchkey;
				}
				$results=$dao->findAutocompleteItem($param);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
					echo $outData;
				} else {
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							$val='';
							$type='';
							$dt='';
							foreach($results[$i] as $key=>$value){
								if(strtolower($key)==strtolower($searchsubtype)){
									$val.=$value;
								}
								if(strtolower($key)==strtolower('Type')){
									$type.=$value;
								}
								if(strtolower($key)==strtolower('DT')){
									$dt.=$value;
								}
							}
							$show=$val." - <b>".$type."</b>".($dt != null && $dt != '' ? " - <b>".$dt."</b>" : "");
							//print $val." - <b>".$type."</b>|".$val."\n";
							print $val." - <b>".$type."</b>\n";
						}
				    }
				}
				break;

			case "Certification"    :
			case "Curricula"   		:

				$dao= new CertMasterDAO();
				$oResults  = $dao->findCertAutocompleteDetails($searchtype,$searchsubtype,$searchkey);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$resultText = $this->toXML('ClueTip',$oResults,'');
				 }else {
				$resultText = $dao->getAutoCompleteTextFormat($oResults,$searchsubtype);
				 }
				expDebug::dPrint("cert master result ".$resultText);
				print $resultText;

			 break;

			case "ContentMaster" :
				$dao= new ContentMasterDAO();
				$oResults =$dao->findContentAutocompleteDetails($searchtype,$searchsubtype,$searchkey);
				$resultText = $dao->getAutoCompleteTextFormat($oResults,$searchsubtype);
				print $resultText;
				break;
			case "instructor" :
				$dao= new CatalogDAO();
				if($searchsubtype == 'id'){
					$param->id=$searchkey;
				} else {
					$param->name=$searchkey;
				}
				$results=$dao->findInstructorAutocomplete($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						print $results[$i]->Id."|".$results[$i]->Name."\n";
					}
			    }
				break;
			case "owner" :
				$dao= new CatalogDAO();
				if($searchsubtype == 'id'){
					$param->id=$searchkey;
				} else {
					$param->name=$searchkey;
				}
				$results=$dao->findOwnerDetails($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						print $results[$i]->Id."|".$results[$i]->Name."\n";
					}
			    }
				break;
			case "user" :
				$dao= new UserDAO();
				if($searchsubtype == 'username'){
					$param->username=str_replace("'","''",$searchkey);
				} 
		        elseif($searchsubtype == 'department'){
					$param->department=$searchkey;
				}elseif($searchsubtype == 'managerusername'){
					$param->managerusername=str_replace("'","''",$searchkey);
				}
				else {
					$param->name=str_replace("'","''",$searchkey);
				}
				$param->Type='AutoComplete';
				$results=$dao->getUser($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
					     print $results[$i]->Name."\n";

					}
			    }
				break;
			case "manager" :
				$dao= new UserDAO();
				if($searchsubtype == 'username'){
					$param->username=$searchkey;
				} else {
					$param->name=$searchkey;
				}
				$param->Type='AutoComplete';
				$results=$dao->getUser($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						print $results[$i]->Id."|".$results[$i]->Name."\n";
					}
			    }
				break;
			case "managerlist":
			    $param->userId  = $userId;
			    $param->status  = $status;
			    $param->name = addslashes($searchkey);
				$param->searchsubtype = $searchsubtype;
				
			    $dao= new UserDAO();
				$results=$dao->getManagerList($param);
				if($results != null){
				    $managerResultCount = sizeOf($results);
    				for($i=0;$i<$managerResultCount;$i++){
    					print $results[$i]->Id."|".$results[$i]->Person."\n";
    				}
				}
			    break;
			case "organizationlist":
				$dao= new UserDAO();
				$param->userId  = $userId;
				$param->status  = $status;
				$param->name = $searchkey;
				$param->searchsubtype = $searchsubtype;

				$results=$dao->getOrganizationList($param);
				if($results != null){
				    $orgResultCount = sizeOf($results);
    				for($i=0;$i<$orgResultCount;$i++){
    					print $results[$i]->Id."|".$results[$i]->Name."\n";
    				}
				}
			    break;
		  case "lnrsearch" :
				$dao = new LnrSearchDAO();
				$param = new StdClass();
				$param->title=$searchkey;
				if($searchsubtype == 'all'){
					$param->types='Course|Certification|Curricula|forum|blog|group|Announcement';
				} else if($searchsubtype == 'formal'){
					$param->types='Course|Certification|Curricula';
				} else if($searchsubtype == 'informal'){
					$param->types='forum|blog|group|Announcement';
				}
				$results=$dao->findAutocomplete($param,$searchtype);
				if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							$val='';
							$type='';
							$dt='';
							foreach($results[$i] as $key=>$value){

								if(strtolower($key)==strtolower('Type')){
									$type.=$value;
								}
								if(strtolower($key)==strtolower('Title')){
									$val.=$value;
								}
							}
							//print $val." - <b>".$type."</b>|".$val."\n";
							print $val." - <b>".$type."</b>\n";
						}
				    }
				break;

		case "AnnouncementMaster" :
				$dao= new AnnouncementMasterDAO();
				if($searchsubtype == 'an_title'){
					$param->an_title=$searchkey;
				} else {
					$param->an_code=$searchkey;
				}
				$param->Type='AutoComplete';
				$results=$dao->SearchAnnouncement($param);
				if($results != null)
				{
					for($i=0;$i<sizeOf($results);$i++)
					{
						if($searchsubtype == 'an_title'){
					    	print $results[$i]->Title."\n";
						}
						else
						{
						  print $results[$i]->Code."\n";
						}
					}
			    }
				break;

		case "SecurityMgmt" :
				$dao= new SecurityManagementServiceDAO();
				if($searchsubtype == 'security_title'){
					$param->name=$searchkey;
				} else {
					$param->value=$searchkey;
				}
				$param->Type='AutoComplete';
				$results=$dao->SearchSecurityMgmtAdmin($param);
				if($results != null)
				{
					for($i=0;$i<sizeOf($results);$i++)
					{
						if($searchsubtype == 'security_title'){
					    	print $results[$i]->Title."\n";
						}
						else
						{
						  print $results[$i]->Code."\n";
						}
					}
			    }
				break;


		case "organization" :
				$dao= new OrganizationDAO();
				if($searchsubtype == 'number'){
					$param->number=$searchkey;
				} else {
					$param->name=$searchkey;
				}
				$param->Type='AutoComplete';
				$results=$dao->getOrganization($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						print $results[$i]->Name."\n";
					}
			    }
				break;
			case "suborganization" :
				$dao= new OrganizationDAO();
				if($searchsubtype == 'number'){
					$param->number=$searchkey;
				} else {
					$param->name=$searchkey;
				}
				$param->Type='AutoComplete';
				$results=$dao->getOrganization($param);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						//foreach($results[$i] as $key=>$value){
						//	if($key=='Name'){
						//		print $value."\n";
						//	}
					//	}
						print $results[$i]->Id."|".$results[$i]->Name."\n";
					}
			    }
				break;

			case "report":

				if($searchsubtype == 'code'){
					$param->SearchKey->name="code";
				} else {
					$param->SearchKey->name="name";
				}
				$param->SearchKey->value=$searchkey;
				$dao = new ReportConfigDAO();
				$results = $dao->searchReport($param);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
					echo $outData;
				} else {
					$resultText = "";
					for($i=0;$i<count($results);$i++){
						if($searchsubtype == 'code')
							//$resultText .= $results[$i]->name . " - ". $results[$i]->code . " | "	. $results[$i]->code ;
							$resultText .=  $results[$i]->code . "\n ";
						else
							//$resultText .= $results[$i]->name . " - ". $results[$i]->code . " | "	. $results[$i]->name ;
							$resultText .= $results[$i]->name . " - <b>". $results[$i]->code . "</b> \n ";
					}
					print $resultText;
				}
				break;



				case "Profile" :
				$dao= new ProfileDAO();
				$results =$dao->fetchProfileListValues($searchsubtype,$subval,$searchkey,$sublvl);
				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						print $results[$i]->name."\n";
					}
			    }
				break;

				case "CountryList" :
				$cntry='null';
				$state='null';
				$city='null';
				$dao= new ProfileDAO();
				$results= null;
				//get the searchsubtype
				if($searchsubtype == 'Country'){
					$cntry=$searchkey;
					$results =$dao->fetchCntryList($cntry);
				} else if($searchsubtype == 'State'){
					$cntry=$subval;
					$state=$searchkey;
					$results =$dao->fetchStateList($cntry,$state);
				} else if($searchsubtype == 'City'){
					$indx=stripos($subval,"|");
					$cntry=substr($subval,0,$indx);
					$state=substr($subval,$indx+1);
					$city=$searchkey;
					$results =$dao->fetchCityList($cntry,$state,$city);
				}

				if($results != null){
					for($i=0;$i<sizeOf($results);$i++){
						if($searchsubtype == 'Country'){
							print $results[$i]->country_name."\n";
							expDebug::dPrint('Country list: '.serialize($results));
						} else if($searchsubtype == 'State'){
							print $results[$i]->state_name."\n";
						} else if($searchsubtype == 'City'){
							print ucwords($results[$i]->city)."\n";
						}

					}
			    }
				break;

				case 'reporttheme':

				if($searchsubtype == 'code'){
					$param->SearchKey->name="code";
				} else {
					$param->SearchKey->name="name";
				}
				$param->SearchKey->value=$searchkey;
				$dao = new ReportThemeDAO();
				$results = $dao->searchTheme($param);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					echo $outData;
				} else {
					$resultText = "";
					for($i=0;$i<count($results);$i++){
						if($searchsubtype == 'code')
							$resultText .=  $results[$i]->code . "\n ";
						else
							$resultText .= $results[$i]->name . "\n ";
					}
					print $resultText;
				}
				break;


				case "reportconfig":
						$dao = new ReportDAO();
						$search_txt = $_GET["z"];
						$call_proc = $_GET["call_proc"];
						$result = $dao->getReportAutocomplete($search_txt,$call_proc);
						//print_r($result);
						for($i=0;$i<sizeOf($result);$i++){
							foreach($result[$i] as $key=>$value){
								if($value != ''){
									print $value."\n";

								}
							}
						}
						/*foreach($result as $key=>$value){
						//for($i=0;$i<sizeOf($results);$i++){
						print $value; echo "<br>";

						}*/
						//print_r($result);
					break;

			case "survey" :
				$dao= new SurveyDAO();
				if($searchsubtype == 'code'){
					$param->code=$searchkey;
				} else{
					$param->title=$searchkey;
				}
				$results=$dao->findAutocompletesurvey($param,$searchtype,$entityType);
				if($iscluetip != null && $iscluetip=='true'){
					header("Content-type: text/xml");
					$outData = $this->toXML('ClueTip',$results,'');
					//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
					echo $outData;
				} else {
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							$val='';
							$type='';
							$dt='';
							foreach($results[$i] as $key=>$value){
								if(strtolower($key)==strtolower($searchsubtype)){
									$val.=$value;
								}
								if(strtolower($key)==strtolower('Type')){
									$type.=$value;
								}
							}
							$show=$val." - <b>".$type."</b>";

							print $val."\n";
						}
				    }
				}
				break;

			    case 'surveyquestions':

					$dao = new SurveyQuestionDAO();
					$aResults = $dao->getSurveyQuestionAutoCompleteDetails($searchkey, $searchtype, $searchsubtype);
					$resultText = $dao->getAutoCompleteTextFormat($aResults,$searchsubtype);
					print $resultText;
					break;

				case "trainingprogram" :
					$dao= new TrainingProgramDAO();
					if($searchsubtype == 'tag'){
						$param->tag=$searchkey;
					}else if($searchsubtype == 'code'){
						$param->code=$searchkey;
					} else{
						$param->title=$searchkey;
					}
					$param->callfrom = $entityType;
					$results=$dao->findAutocompleteprogram($param,$searchtype);
					if($iscluetip != null && $iscluetip=='true'){
						header("Content-type: text/xml");
						$outData = $this->toXML('ClueTip',$results,'');
						//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
						echo $outData;
					} else {
						if($results != null){
							for($i=0;$i<sizeOf($results);$i++){
								$val='';
								$type='';
								$dt='';
								foreach($results[$i] as $key=>$value){
									if(strtolower($key)==strtolower($searchsubtype)){
										$val.=$value;
									}
									if(strtolower($key)==strtolower('Type')){
										$type.=$value;
									}
								}
								$show=$val." - <b>".$type."</b>";

								print $val."\n";
							}
					    }
					}
				break;

				case 'notificationmaster':

					if($searchsubtype == 'notification_code'){
						$param->SearchKey->name="notification_code";
					} else {
						$param->SearchKey->name="notification_title";
					}
					$param->SearchKey->value=$searchkey;
					$dao = new NotificationDAO();
					$results = $dao->searchNotification($param);
					if($iscluetip != null && $iscluetip=='true'){
						header("Content-type: text/xml");
						$outData = $this->toXML('ClueTip',$results,'');
						echo $outData;
					} else {
						$resultText = "";
						for($i=0;$i<count($results);$i++){
							if($searchsubtype == 'noti_code')
								$resultText .=  $results[$i]->notification_code . "\n ";
							else
								$resultText .= $results[$i]->notification_title . "\n ";
						}
						print $resultText;
					}
				break;

				case "training_program" :
				$param->course = true; //Follows through
				$dao= new LnrTrainingProgramDAO();
				if($searchsubtype == 'Title'){
					$param->title=$searchkey;
				}
				$searchtype='course';
				$results=$dao->getTPAutocompleteSearch($param,$searchtype,$searchautotype);
					if($results != null){
						for($i=0;$i<sizeOf($results);$i++){
							print $results[$i]->title."\n";
						}
				    }
				break;

		}


		exit();
	}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
}


$obj= new Autocomplete();