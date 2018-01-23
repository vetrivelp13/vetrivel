<?php
	/**
	* @class or PHP Name		: Profileconfig DAO
	* @author(s)				: Product Team
	* Version         			: 1.0
	* Date						: 10/12/2009
	* PHP Version          		: 5.2.6
	* Description     			: DAO Class to do basic CRUD operations in Profileconfig table.
	*/

include_once "AbstractDAO.php";
include_once "ProfileDAO.php";

class ProfileConfigDAO extends AbstractDAO{

	public function __construct(){
		parent::__construct();
	}

	public function getInitialData($inObj=null){
		try{
			$language = 'cre_sys_lng_eng';

			expDebug::dPrint("LANGUAGE VALUE -->".$language);

			$profileDAO = new ProfileDAO();

			$this->connect();
			$qury = "CALL slp_list_items_sel('Entity', NULL);";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			expDebug::dPrint("cnt ".count($results));
			$ret = "<Entity>";
			$ret .= $this->retXml($results);
			$ret .= "</Entity>";
			$this->closeconnect();

			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_prf_ctg','".$language."');";
//			$query = "SELECT id as Id, name as Name FROM slt_profile_list_items WHERE listid ='27' AND parentid IS null";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			expDebug::dPrint("cnt ".count($results));
			$ret .= "<Category>";
			$ret .= $this->retXml($results);
			$ret .= "</Category>";
			$this->closeconnect();
			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_prf_tag','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<Functions>";
			$ret .= $this->retXml($results);
			$ret .= "</Functions>";
			$this->closeconnect();
			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_prf_dty','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<DataType>";
			$ret .= $this->retXml($results);
			$ret .= "</DataType>";
			$this->closeconnect();
			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_prf_pui','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<UI>";
			$ret .= $this->retXml($results);
			$ret .= "</UI>";
			$this->closeconnect();
						
			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_sys_lng','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData  Language-->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<Languages>";
			$ret .= $this->retXml($results);
			$ret .= "</Languages>";
			$this->closeconnect();

			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_sys_inv','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<Validate>";
			$ret .= $this->retXml($results);
			$ret .= "</Validate>";
			$this->closeconnect();
			$this->connect();
			$results =  $profileDAO->filterNodesOnAttr("H");
			$ret .= "<Hierarchy>";
			$ret .= $this->retXml($results);
			$ret .= "</Hierarchy>";
			$this->closeconnect();

			$this->connect();
			$results =  $profileDAO->filterNodesOnAttr("SM");
			$ret .= "<Select>";
			$ret .= $this->retXml($results);
			$ret .= "</Select>";
			$this->closeconnect();

			$this->connect();
			$results =  $profileDAO->filterNodesOnAttr("BL");
			$ret .= "<BoolTypes>";
			$ret .= $this->retXml($results);
			$ret .= "</BoolTypes>";
			$this->closeconnect();


			return $ret;
		}catch(Exception $e){
			$this->closeconnect();
			expDebug::dPrint("Excep ".$e->getMessage());
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getDataTypes(){
		try{
			$language = 'cre_sys_lng_eng';
			$profileDAO = new ProfileDAO();

			$this->connect();
			$qury = "CALL slp_list_items_sel('cre_sys_odt','".$language."');";
			expDebug::dPrint("ProfileConfigDAO getInitialData -->".$qury);
			$this->query($qury);
			$results = $this->fetchAllResults();
			$ret .= "<DataType>";
			$ret .= $this->retXml($results);
			$ret .= "</DataType>";
			$this->closeconnect();

			return $ret;
		}catch(Exception $e){
			$this->closeconnect();
			expDebug::dPrint("Excep ".$e->getMessage());
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	private function retXml($results){
		try {
			$ret = "";
			for($i=0;$i<count($results);$i++){
				$ret .= "<Item>";
				foreach($results[$i] as $key=>$value){
					$ret .= "<".$key.">".$value."</".$key.">";
				}
				$ret .= "</Item>";
			}
			expDebug::dPrint("ret ".$ret);
			return $ret;
		}catch(Exception $e){
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}

	private function loadProfileConfig($catId){
		try{
			$language = 'cre_sys_lng_eng';
			
			$this->connect();
			$this->query("SELECT slf_get_profile_id('".$catId."','".$language."') as catid");
			$result= $this->fetchAllResults();
			$catId = $result[0]->catid;
			$this->closeconnect();
			
			$this->connect();
			$query = "call slp_profileconfig_sel('id,code,lang_code,profile_field_id,data_type,ui,cols, profile_value_id, type ','category_id=\'". $catId ."\'' )";
			expDebug::dPrint("loadProfileConfig query ".$query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$retXml = "<StoredValues>";
			for($i=0;$i<count($results);$i++){
				$retXml .= "<field>";
				$retXml .= "<id>".$results[$i]->id."</id>";
				$retXml .= '<name id="'. $results[$i]->profile_field_id.'">'. $this->lookUp($results[$i]->profile_field_id).'</name>';
				$dataType = $this->lookUp($results[$i]->data_type);
				$retXml .= '<data_type id="'.$results[$i]->data_type.'">'.$dataType.'</data_type>';
				if($results[$i]->type=="L"){
					$retXml .= '<profile_value_id id="F'.$results[$i]->profile_value_id.'">'.$results[$i]->profile_value_id.'</profile_value_id>';
					$retXml .= '<profile_value_name id="F'.$results[$i]->profile_value_id.'">'.$this->lookUpRootNode($results[$i]->profile_value_id).'</profile_value_name>';
				}
				else if($results[$i]->type=="LI"){
					$retXml .= '<profile_value_id id="'.$results[$i]->profile_value_id.'">'.$results[$i]->profile_value_id.'</profile_value_id>';
					$retXml .= '<profile_value_name id="'.$results[$i]->profile_value_id.'">'.$this->lookUp($results[$i]->profile_value_id).'</profile_value_name>';
				}

				$retXml .= '<ui id="'.$results[$i]->ui.'">'.$this->lookUp($results[$i]->ui).'</ui>';
				$retXml .= '<cols id="'. $results[$i]->cols.'">'.$results[$i]->cols.'</cols>';
				$retXml .= '</field>';
			}
			$retXml .= "</StoredValues>";
			$this->closeconnect();
			return $retXml;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function loadProfileFields($reqData){
		try{
			$language = 'cre_sys_lng_eng';
			
			$this->connect();
			$this->query("SELECT slf_get_profile_id('".$reqData->id."','".$language."') as parentid");
			$result= $this->fetchAllResults();
			$parentid = $result[0]->parentid;
			$this->closeconnect();
			
			$this->connect();
			$query = "call slp_profilelist_items_sel('*','parent_id =\"". $parentid ."\"')";
			expDebug::dPrint("ProfileConfig DAO loadProfileFields query ".$query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret = "<Items>".$this->retXml($results)."</Items>";
			$this->closeconnect();

			$ret .= $this->loadProfileConfig($reqData->id);

			return $ret;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function saveDataMap($reqData){
		try{
			$query = "";
			$this->connect();
			$this->beginTrans();
			$vUserId 			= 	$this->getLoggedInUserId();
			for($i=0;$i<count($reqData->field);$i++){
				$query = "CALL slp_profileconfig_ins('".$reqData->entity."','".$reqData->category."','".$reqData->field[$i]->field_id."','".$reqData->field[$i]->data_type."',".($reqData->field[$i]->hier_id==0?'NULL':"'".$reqData->field[$i]->hier_id."'").",'".$reqData->field[$i]->ui."','".$reqData->field[$i]->column."','".$reqData->field[$i]->function."','',$vUserId)" ;
				expDebug::dPrint("ProfileconfigDAO saveDataMap() query ".$query);
				$this->query($query);
			}
			$this->commitTrans();
			$this->closeconnect();
			return "";
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}


	public function lookUpRootNode($id){
		try{
			$query = "call slp_profilelist_sel('name','code=\"". $id."\"')";
			expDebug::dPrint("lookUpRootNode query ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret="";
			if(count($results)==0){
				throw new SoapFault("SPLMS","1Invalid Id ".$id);
			}else
				$ret = $results[0]->name;
			$this->closeconnect();
			return $ret;

		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}


	public function lookUp($id){
		try{
			//$id = trim($id,' " ');
			$query = "call slp_profilelist_items_sel('name','id=\"". $id."\"')";
			expDebug::dPrint("Profile Config DAO lookUp ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret="";			
			
			if(count($results)==0){				
			$this->closeconnect();		
			$this->connect();			
			$query = "call slp_profilelist_items_sel('name','code=\"". $id."\"')";
			expDebug::dPrint("Profile Config DAO lookUp 2 ".$query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret = "";			
			}
			
			if(count($results)==0){
				throw new SoapFault("SPLMS","2Invalid Id ".$id);
			}else
				$ret = $results[0]->name;
			$this->closeconnect();
			return $ret;

		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getAvailCols($reqData, $profileFieldId=''){
		try{
			$selectCols = "cols";
			$whereClause = "category_id='". $reqData->id ."'";

			if(!empty($profileFieldId))
				$whereClause .= " and profile_field_id<>'".$profileFieldId."'";

			$whereClause .= " order by cols";
			$query = "call slp_profileconfig_sel('". $selectCols ."',\"". $whereClause ."\")";
			expDebug::dPrint("getAvailCols2 ".$query);

			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret="";
			$arrCols = array();
			for($i=0;$i<count($results);$i++){
				$arrCols[] = $results[$i]->cols;
			}
			for($i=1;$i<=26;$i++){
				if(!in_array($i,$arrCols))
					$ret .= "<col>".$i."</col>";
			}

			$ret = "<columns>".$ret."</columns>";
			$this->closeconnect();
			return $ret;

		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function saveProfileField($reqData){
		try{
			$language = 'cre_sys_lng_eng';
			
			$query = "call slp_profilelist_sel('id','name = \"Category\"')";
			expDebug::dPrint("saveProfileField_Select ".$query);
			$this->connect();
			$this->query($query);
			$result= $this->fetchAllResults();
			$catId = $result[0]->id;
			$vUserId 			= 	$this->getLoggedInUserId();
			
			$this->closeconnect();
			$this->connect();
			$this->query("SELECT slf_get_profile_id('".$reqData->category_id."','".$language."') as catid");
			$result= $this->fetchAllResults();
			$category_id = $result[0]->catid;

			if(!isset($reqData->config_id) || empty($reqData->config_id)){
				
				$query = "call slp_profilelist_items_ins('".$reqData->code."','".$reqData->language_code."','".$reqData->field_name."','".$catId."',$category_id,'".$reqData->attr1."','".$reqData->attr2."','".$reqData->attr3."','".$reqData->attr4."','Y','".$vUserId."','','','','','',@out)";
				expDebug::dPrint("saveProfileField___Insert ".$query);
				$this->closeconnect();
				$this->connect();
				$this->query($query);
				//$this->query("select last_insert_id() as id");
				$this->query("select @out as id");
				$result= $this->fetchAllResults();
				$id = $result[0]->id;
				if($id =='-1')
				{	
					$setID = '<ResponseCode>'.$id.'</ResponseCode>';
					return $setID;
				}
				$this->closeconnect();
				$type;
				$profileValueId;
				if(!empty($reqData->hier_id))
					$profileValueId = $reqData->hier_id;
				else if(!empty($reqData->sel_id))
					$profileValueId = $reqData->sel_id;
				else if(!empty($reqData->bool_id))
					$profileValueId = $reqData->bool_id;
				if(isset($profileValueId)){
					if(substr($profileValueId,0,1)=="F"){
						$type = "L";
						$profileValueId = substr($profileValueId,1);
					}
					else
					 	$type = "LI";
				}

				expDebug::dPrint("sel_id2 ".$reqData->sel_id);

				$query = "CALL slp_profileconfig_ins('".$reqData->code."','".$reqData->language_code."','".$reqData->entity_id."','".$category_id."','".$id."','".$reqData->data_type_id."',".(isset($type)?"'".$type."'":'NULL').",".($profileValueId!=0?"'".$profileValueId."'":'NULL').",'". $reqData->display_on ."','".$reqData->ui_id."',".($reqData->display_on=="R"?'NULL':"'".$reqData->column."'").",'".$reqData->function."','',$vUserId)" ;
				expDebug::dPrint("saveProfileField ".$query);
				$this->connect();
				$this->query($query);
				$this->closeconnect();

			}else{
				$query = "call slp_profilelist_items_upd('".$reqData->profile_field_id."','".$reqData->code."','".$reqData->language_code."','".$reqData->field_name."','".$reqData->attr1."','".$reqData->attr2."','".$reqData->attr3."','".$reqData->attr4."','Y','".$vUserId."',@out)";
				$this->closeconnect();
				$this->connect();
				$this->query($query);

				$this->closeconnect();

				$type;
			$type;
				$profileValueId;
				if(!empty($reqData->hier_id))
					$profileValueId = $reqData->hier_id;
				else if(!empty($reqData->sel_id))
					$profileValueId = $reqData->sel_id;
				else if(!empty($reqData->bool_id))
					$profileValueId = $reqData->bool_id;
				if(isset($profileValueId)){
					if(substr($profileValueId,0,1)=="F"){
						$type = "L";
						$profileValueId = substr($profileValueId,1);
					}
					else
					 	$type = "LI";
				}
				expDebug::dPrint("sel id ".$reqData->sel_id);
				expDebug::dPrint("type ".$type);
				$query = "CALL slp_profileconfig_upd('". $reqData->config_id ."','".$reqData->code."','".$reqData->language_code."','".$reqData->entity_id."','".$category_id."','".$reqData->profile_field_id."','".$reqData->data_type_id."',".(isset($type)?"'".$type."'":'NULL').",".($profileValueId!=0?"'".$profileValueId."'":'NULL').",'". $reqData->display_on ."','".$reqData->ui_id."',".($reqData->display_on=="R"?'NULL':"'".$reqData->column."'").",'".$reqData->function."','',$vUserId)" ;
				expDebug::dPrint("saveProfileField ".$query);
				$this->connect();
				$this->query($query);
				$this->closeconnect();


			}
			return "";
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function loadProfileField($reqData){
		try{
			$query = "CALL slp_profileconfig_sel('id,code, lang_code, entity_id, category_id, profile_field_id, type, profile_value_id, display_col, data_type, ui, cols, profile_tag_code functions, flags', 'id = ".$reqData->id ."')";

			expDebug::dPrint("query ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$ret ="";
			$profileId;
			if(count($results)==0)
				throw new SoapFault("SPLMS","3Invalid Id ".$reqData->id);
			else{
				$ret .= "<id>".$reqData->id."</id>";
				$ret .= "<profile_field_id>".$results[0]->profile_field_id."</profile_field_id>";
				$profileId = $results[0]->profile_field_id;
				$ret .= "<data_type_id>".$results[0]->data_type."</data_type_id>";
				$dataType = $this->lookUp($results[0]->data_type);
				$ret .= "<data_type>".$dataType."</data_type>";

				if($results[0]->type=="L")
					$ret .= "<profile_value_id>F".$results[0]->profile_value_id."</profile_value_id>";
				else if($results[0]->type=="LI")
					$ret .= "<profile_value_id>".$results[0]->profile_value_id."</profile_value_id>";
					
				$ret .= "<code>".$results[0]->code."</code>";
				$ret .= "<lang_code>".$results[0]->lang_code."</lang_code>";
				$ret .= "<ui_id>".$results[0]->ui."</ui_id>";
				$ret .= "<cols>".$results[0]->cols."</cols>";
				$ret .= "<functions>".$results[0]->functions."</functions>";
				$ret .= "<display_on>".$results[0]->display_col."</display_on>";

				$obj = new stdClass();
				$obj->id = $results[0]->category_id;
				$ret .= $this->getAvailCols($obj,$profileId);
			}

			$this->closeconnect();

			$query = "CALL slp_profilelist_items_sel('*','id=\"". $results[0]->profile_field_id ."\"')";
			expDebug::dPrint("saveProfileList___sel ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();

			if(count($results)==0)
				throw new SoapFault("SPLMS","4Invalid Id ".$profileId);
			else{
				$ret .= "<profile_field_name>".$results[0]->name."</profile_field_name>";
				$ret .= "<attr1>".$results[0]->attr1."</attr1>";
				$ret .= "<attr2>".$results[0]->attr2."</attr2>";
				$ret .= "<attr3>".$results[0]->attr3."</attr3>";
				$ret .= "<attr4>".$results[0]->attr4."</attr4>";
			}
			$ret = "<LoadProfileFieldResponse>" . $ret . "</LoadProfileFieldResponse>";
			return $ret;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}

	public function deleteProfileField($requestData){
		try{
			$query = "CALL slp_profileconfig_del('". $requestData->id ."')";
			expDebug::dPrint("deleteProfileField ".$query);
			$this->connect();
			$this->beginTrans();
			$this->query($query);
			$this->commitTrans();
			$this->closeconnect();
			return "";
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

}
?>