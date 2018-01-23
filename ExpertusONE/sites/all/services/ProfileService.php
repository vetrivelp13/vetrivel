<?php
/**
 * @class or PHP Name		: Profile  Service
 * @author(s)				: Product Team
 * Version         			: 1.0
 * Date						: 13/10/2009
 * PHP Version          		: 5.2.6
 * Description     			: Web Service which is used to do CRUD operations of the Catalog table in SmartPortal LMS.
 */

include "../dao/ProfileDAO.php";
include_once "ServiceBase.php";
class ProfileService extends ServiceBase{


	function __construct(){

	}

	public function insertProfileRequest($requestData){
		try{
//			expDebug::debugOff();
			expDebug::dPrint("getProfileRequest started");
			$dao = new ProfileDAO();
			expDebug::dPrint("level ".$requestData->level);
			$id = $dao->insertProfile($requestData);
			return $id;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}


	public function getProfileRequest($requestData){
		try{
			expDebug::dPrint("getProfileRequest started");
			expDebug::dPrint("dummy ".$requestData->dummy);
			$dao = new ProfileDAO();
			expDebug::dPrint("count ".count($requestData));
			$outData = $dao->getTree();
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function renameProfile($requestData){
		try{
			expDebug::dPrint("renameProfile starts...");
			$dao = new ProfileDAO();
			$dao->renameProfile($requestData);
			return;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}

	public function deleteProfile($requestData){
		try{
			expDebug::dPrint("deleteProfile starts...");
			$dao = new ProfileDAO();
			$dao->deleteProfile($requestData);
			return;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}

	public function getProfileDets($requestData){
		try{
			expDebug::dPrint("getProfileDets starts...");
			$dao = new ProfileDAO();
			$results = $dao->getProfileDets($requestData);
			expDebug::dPrint("getProfileDets count ".count($results));			
			$outData = $this->toXML('GetProfileDetsResponse',$results,null);
			
			$result1=$dao->getProfileLangCode($results[0]->lang_code);
			$outData1 = $this->toXML("GetLanguageCodeIdResponse",$result1,null);

			$result2=$dao->getProfileCode($requestData);
			$outData2 = $this->toXML("GetParentCodeResponse",$result2,null);
			$finalOutData=$outData.$outData1.$outData2;
			
			$outData = new SoapVar($finalOutData, XSD_ANYXML, null, null, null);
			return $outData;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}

	public function updateProfile($requestData){
		try{
			expDebug::dPrint("updateProfile starts...");
			$dao = new ProfileDAO();
			$results = $dao->updateProfile($requestData);
			//$outData = $this->toXML('GetProfileDetsResponse','Result',$results);
			//$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			//return $outData;
			return $results;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getProfileByType($requestData){
			expDebug::dPrint("getProfileByType starts...");
		try{
			$dao = new ProfileDAO();
			$results = $dao->getProfileByType($requestData);
			$lastId="";
			$str ="";$xmlStr="";
			for($i=0;$i<count($results);$i++){
				if($lastId!=$results[$i]->profiletype_id){
					if(!empty($str))
						$str .= "</items></profile>";
					$xmlStr .= $str;
					$str = "<profile id=\"".$results[$i]->profiletype_id."\">";
					$str .= "<name>".$results[$i]->profiletype_name."</name>";
					$str .= "<items>";
				}
				$str .=  "<item id=\"". $results[$i]->profilevalue_id ."\">".$results[$i]->profilevalue_name."</item>";
				$lastId = $results[$i]->profiletype_id;
			}
			if(!empty($str))
				$str .= "</items></profile>";
			$xmlStr .= $str;
			$xmlStr.="<GetProfileByTypeResponse>".$xmlStr."</GetProfileByTypeResponse>";
			$xmlStr = new SoapVar($xmlStr, XSD_ANYXML, null, null, null);
			expDebug::dPrint("getProfileByType response ".print_r($xmlStr));
			return $xmlStr;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function searchProfile($requestData){
		expDebug::dPrint("searchProfile starts.. ");
		try{
			$dao = new ProfileDAO();
			$results = $dao->searchProfile($requestData);
			$action = array(new stdClass());
			$action[0]->action="Edit";
			$action[0]->script="displayFetchResourceLocationDetails";
			$action[0]->type="Ajax";
			$action[0]->params="id";
			$outData = $this->toXML('SearchProfileResponse',$results,$action);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	public function getProfileSubTree($requestData){
		expDebug::dPrint("getProfileSubTree starts.. ");
		try{
			$dao = new ProfileDAO();
			$results = $dao->getProfileSubTree($requestData);
			$mapAttr = $dao->getMapAttribute($requestData->map_attr);
			//if($requestData->op=="edit"){
			$selectedNodes = $dao->getSelectedNodes($requestData->id1, $requestData->parent_id);
			//}
			$results = "<SubTree>". $results . "</SubTree>";
			$results .= $mapAttr;
			$results .= $selectedNodes;			
			$outData = new SoapVar($results, XSD_ANYXML, null, null, null);
			return $outData;
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	public function saveProfileMapping($requestData){
		expDebug::dPrint("saveProfileMapping starts.. ");
		$dao = new ProfileDAO();
		$results = $dao->saveProfileMapping($requestData);
		expDebug::dPrint("saveProfileMapping done.. ");
		
		/*$outData = new SoapVar($results, XSD_ANYXML, null, null, null);
		return $outData;*/
	}
	
	public function getProfileAsync($reqData){
		expDebug::dPrint("getProfileAsync starts.. ");
		try{
			$dao = new ProfileDAO();
			$results = $dao->getProfileAsync($reqData);
			$outData = new SoapVar($results, XSD_ANYXML, null, null, null);			
			expDebug::dPrint("getProfileAsync done.. ".print_r($outData));
			return $outData;
		}catch(Exception $e){
				throw new SoapFault("SPLMS",$e->getMessage());
			}
	}
	
	public function getProfileCode($requestData){
		try{
			expDebug::dPrint("getProfileCodeRequest started");
			$dao = new ProfileDAO();
			$results = $dao->getProfileCode($requestData);
			$outData = $this->toXML('GetProfileCodeResponse',$results,null);
			$outData = new SoapVar($outData, XSD_ANYXML, null, null, null);
			return $outData;

		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
}


$server = new SoapServer("../modules/core/exp_sp_core/wsdl/profileservice.wsdl");
$server->setClass("ProfileService");
$server->handle();
?>