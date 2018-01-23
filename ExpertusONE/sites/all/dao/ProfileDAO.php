<?php
/**
 * @class or PHP Name		: Catalog DAO
 * @author(s)				: Sangeetha - Product Team
 * Version         			: 1.0
 * Date						: 09/09/2009
 * PHP Version          		: 5.2.6
 * Description     			: DAO Class to do basic CRUD operations in catalog table.
 */

include_once "AbstractDAO.php";

class ProfileDAO extends AbstractDAO{
 
	private $strIds = "";
	private $id;
	public function __construct(){
		parent::__construct();
	}

	public function insertProfile($inObj){
		try{
			/*if($inObj->root!="R")
				$inObj->level++;*/
			$vUserId = $this->getLoggedInUserId();
			$inObj->code = str_replace(" ", "_", $inObj->code);
			$inObj->code = ereg_replace("[^A-Za-z0-9\_]", "", $inObj->code); 
			if($inObj->level==1){
				$qualifier = $this->getQualifier($inObj->name);
				$qry="call slp_profilelist_ins('".$inObj->code."','".$inObj->language_code."','".$inObj->name."','".$inObj->type."','".$inObj->description."','".$inObj->attr1."','".$inObj->attr2."','".$inObj->attr3."','".$inObj->attr4."','".$inObj->is_active."','".$vUserId."', '','','','','". $qualifier  ."',@insval)";
			}else if ($inObj->level==2){
				$qry="call slp_profilelist_items_ins('".$inObj->code."','".$inObj->language_code."','".$inObj->name."','".substr($inObj->parent_id,1)."',NULL,'".$inObj->attr1."','".$inObj->attr2."','".$inObj->attr3."','".$inObj->attr4."','".$inObj->is_active ."','".$vUserId."','','','','','',@insval)"; //substr to remove the "F" prefix
			}else {
				//expDebug::dPrint(strval(stripos($inObj->parent_id,'F')));
				$inObj->parent_id=(strval(stripos($inObj->parent_id,'F'))!="" && stripos($inObj->parent_id,'F')>=0)?"NULL":"'".$inObj->parent_id."'";
				$qry="call slp_profilelist_items_ins('".$inObj->code."','".$inObj->language_code."','".$inObj->name."','".substr($inObj->list_id,1)."',".$inObj->parent_id.",'".$inObj->attr1."','".$inObj->attr2."','".$inObj->attr3."','".$inObj->attr4."','".$inObj->is_active ."','".$vUserId."','','','','','',@insval)";
			}
			
			expDebug::dPrint('Profile DAO insert Query --->'.$qry);
			$this->connect();
			$this->beginTrans();
			$res = $this->query($qry);
			$this->commitTrans();
			
			expDebug::dPrint("committed..");
			
			//$res = $this->query("select last_insert_id() as id");
			$res = $this->query("select @insval as id");
			$result= $this->fetchAllResults();
			
			expDebug::dPrint('Profile DAO insert id --->'.$result[0]->id);
			if($inObj->level==1)
				return "F".$result[0]->id;
			else
				return $result[0]->id;
			$this->closeconnect();
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getQualifier($name){
		try{
			$qualifier = strtolower(substr($name,0,2));
//			$query = "SELECT COUNT(LCASE(SUBSTR(NAME,1,2))) AS qfy_count FROM slt_profile_list WHERE LCASE(SUBSTR(NAME,1,2))='". $qualifier ."'"; /* listid is not null and  */
			$query = "call slp_profilelist_sel('count(lcase(substr(name,1,2))) AS qfy_count',\"lcase(substr(name,1,2))='". $qualifier ."'\")"; /* listid is not null and  */
			expDebug::dPrint("getQualifier ". $query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			expDebug::dPrint("count getQualifier ". count($results));
			if($results[0]->qfy_count > 0){
				$qualifier = $this->getUniqueQualifier($qualifier, $results[0]->qfy_count+1);
			}
			return $qualifier;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}
	
	private function getUniqueQualifier($qualifier, $count){
		try {
	//		$query = "SELECT NAME FROM slt_profile_list WHERE custom4='". $qualifier . $count ."'";
			$query = "call slp_profilelist_sel('name',\" custom4='". $qualifier . $count ."' \")";
			expDebug::dPrint("getUniqueQualifier ". $query);
			$this->connect();
			$this->query($query);
			$res = $this->fetchAllResults();
			if(count($res)!=0){
				$qualifier = $this->getUniqueQualifier($qualifier, $count+1);
			}else{
				$this->closeconnect();	
				return $qualifier.$count;
			}
			$this->closeconnect();
			return $qualifier;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}
	
	public function getProfileSubTree($inObj){
		try {
			$finalRes = "";
	//		$qry = "SELECT id, `name`, custom4 FROM slt_profile_list WHERE `id` IN (". $inObj->type  .") AND is_active='Y' ORDER BY id";
			$qry = "call slp_profilelist_sel(\"id, name\",\" `id` IN (". $inObj->type  .") AND is_active='Y' ORDER BY id \")";
			expDebug::dPrint($qry);
			$this->connect();
			$this->query($qry);
			$res = $this->fetchAllResults();
			$this->closeconnect();		
			$str="";
			for($i=0;$i<count($res);$i++){
				$str = "<profile><root>";
				$crVal=$res[$i]->name;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal_convert = htmlspecialchars($crVal);
					//$crVal = $crVal;
				}
				$str .= "<item parent_id=\"0\" id=\"F". $res[$i]->id ."\" qulfy=\"". $res[$i]->custom4 ."\"><content><name>".$crVal_convert."</name></content></item>";
	//			$qry = "SELECT id, name FROM slt_profile_list_items WHERE listid='".$res[$i]->id ."' AND parentid is NULL AND is_active='Y' ORDER BY id";
				$qry = "call slp_profilelist_items_sel( \"id, name\", \"list_id='".$res[$i]->id ."' AND parent_id is NULL AND is_active='Y' ORDER BY id\")";
				expDebug::dPrint("getProfileSubTree qu ".$qry);
				$this->connect();
				$this->query($qry);
				$subRes = $this->fetchAllResults();
				$this->closeconnect();
				$strIds="";
				expDebug::dPrint("getProfileSubTree qu ".count($subRes));
				for($j=0;$j<count($subRes);$j++){
					$crVal=$subRes[$j]->name;
					$isutfstr = mb_detect_encoding($crVal,"ASCII");
					if (is_string($isutfstr)){
						$crVal = htmlspecialchars($crVal);
						//$crVal = $crVal;
					}
					$str .= "<item parent_id=\"F". $res[$i]->id ."\" id=\"". $subRes[$j]->id ."\"><content><name>".$crVal."</name></content></item>";
					$strIds .= "'".$subRes[$j]->id."',";
					expDebug::dPrint("id ".$subRes[$j]->id);
				}
				//expDebug::dPrint("strIds ".$strIds);
				if(count($subRes)!=0){
					$subTr = $this->getSubTree(substr($strIds,0,-1),true);
					$str .= $subTr;
				}
				expDebug::dPrint("ret str ".$subTr);
				$str .= "</root></profile>";
				$finalRes .= $str;
				expDebug::dPrint("getProfileSubTree ");
				expDebug::dPrint($str);
				$str ="";
			}
			$this->closeconnect();
			return $finalRes;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}	

	
	public function getTree(){
		try {
			$finalRes = "";
	//		$qry = "SELECT slt_profile_list.id AS nodeid, slt_profile_list.name AS nodename,  slt_profile_list_items.id AS itemid, slt_profile_list_items.name AS itemname FROM slt_profile_list LEFT OUTER JOIN slt_profile_list_items ON slt_profile_list.id = slt_profile_list_items.listid ORDER BY nodeid, itemid ";
			$qry = "select * from slv_profile_tree";
			$this->connect();
			$this->query($qry);
			$res = $this->fetchAllResults();
			$lastid=-1;
			$str="";
			$Ids="";
			expDebug::dPrint("len ".count($res));
			for($i=0;$i<count($res);$i++){
				// construct level 0 and 1 nodes and store in $finalRes array
				if($res[$i]->nodeid!=$lastid){
					if(!empty($str)){
						$this->strIds = "";
						$strId = $this->getSubTree(substr($Ids,0,-1));
						expDebug::dPrint("strId ".$strId);
						//$finalRes[] = $str . $strId;
						$finalRes .= $str . $strId;
						$Ids="";
						//			expDebug::dPrint("str ".$str . $strId);
					}
					//"F" prefixed for level 1 nodes to prevent id clash with lower nodes
					$crVal=$res[$i]->nodename;
					$isutfstr = mb_detect_encoding($crVal,"ASCII");
					if (is_string($isutfstr)){
						$crVal = htmlspecialchars($crVal);
						//$crVal = $crVal;
					}
					$str = '<item parent_id="R" id="F'. $res[$i]->nodeid .'"><content><name>'.$crVal.'</name></content></item>';
				}
				if(!is_null($res[$i]->itemid) && !empty($res[$i]->itemid) ){
					$crVal=$res[$i]->itemname;
					$isutfstr = mb_detect_encoding($crVal,"ASCII");
					if (is_string($isutfstr)){
						$crVal = htmlspecialchars($crVal);
						//$crVal = $crVal;
					}
					$str .= '<item parent_id="F'. $res[$i]->nodeid .'" id="'. $res[$i]->itemid .'"><content><name>'.$crVal.'</name></content></item>';	
				}
				if(!is_null($res[$i]->itemid) && !empty($res[$i]->itemid) )
				$str .= '<item parent_id="F'. $res[$i]->nodeid .'" id="'. $res[$i]->itemid .'"><content><name>'.htmlspecialchars($res[$i]->itemname).'</name></content></item>';
				$Ids .= "'".$res[$i]->itemid."',";
				$lastid = $res[$i]->nodeid;
			}
			if(count($res)>0){
				if(!empty($str)){
					$this->strIds = "";
					$strId = $this->getSubTree(substr($Ids,0,-1));
					expDebug::dPrint("strId ".$strId);
					//$finalRes[] = $str . $strId;
					$finalRes .= $str . $strId;
					$Ids="";
					//			expDebug::dPrint("str ".$str . $strId);
				}
				//"F" prefixed for level 1 nodes to prevent id clash with lower nodes
				$crVal=$res[$i]->nodename;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal = htmlspecialchars($crVal);
					//$crVal = $crVal;
				}
				$str = '<item parent_id="R" id="F'. $res[$i]->nodeid .'"><content><name>'.$crVal.'</name></content></item>';
			}
			if(!is_null($res[$i]->itemid) && !empty($res[$i]->itemid) ){
				$crVal=$res[$i]->itemname;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal = htmlspecialchars($crVal);
					//$crVal = $crVal;
				}
				$str .= '<item parent_id="F'. $res[$i]->nodeid .'" id="'. $res[$i]->itemid .'"><content><name>'.$crVal.'</name></content></item>';	
			}
			/*if(!empty($str))
				$finalRes .= $str;*/
			//expDebug::dPrint("list ".count($finalRes));
			$this->closeconnect();
			return '<root><item parent_id="0" id="R"><content><name>Lookup</name></content></item>'.$finalRes.'</root>';
			
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}

	}



	public function getSubTree($Ids, $isActive=false){
		try {
			//$query = "SELECT id, name, IF (listid IS NULL, parentid, listid) AS parentid FROM slt_profile_list_items WHERE parentid IN (".$Ids .") order by id";
			if($isActive)
	//			$query = "SELECT id, name, parentid FROM slt_profile_list_items WHERE parentid IN (".$Ids .") AND is_active='Y' ORDER BY parentid, id"; //listid IS NULL AND 
				$query = "call slp_profilelist_items_sel('id, name, parent_id', \"parent_id IN (".$Ids .") AND is_active='Y' ORDER BY parent_id, id\")"; //listid IS NULL AND
			else
	//			$query = "SELECT id, name, parentid FROM slt_profile_list_items WHERE parentid IN (".$Ids .") ORDER BY parentid, id"; //listid IS NULL AND 
				$query = "call slp_profilelist_items_sel('id, name, parent_id',\"parent_id IN (".$Ids .") ORDER BY parent_id, id \")"; //listid IS NULL AND
			expDebug::dPrint("query inside getSubTree Function ProfileDAO ".$query);
			$this->connect();
			$this->query($query);
			$res = $this->fetchAllResults();
			$idstr="";
			$strIds="";
			expDebug::dPrint("res ".count($res));
			//if(count($res)==0)
			//			return;
			for($i=0;$i<count($res);$i++){
				$crVal=$res[$i]->name;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal = htmlspecialchars($crVal);
					//$crVal = $crVal;
				}
				$strIds .= '<item parent_id="'. $res[$i]->parent_id .'" id="'. $res[$i]->id .'"><content><name>'.$crVal.'</name></content></item>';
				$idstr .= "'".$res[$i]->id ."',";
			}
			//		expDebug::dPrint("str ".$this->strIds);
	
	
			if(empty($idstr) || $idstr=="" || $idstr=="'',")
				return "";
			$this->closeconnect();
			return $strIds . $this->getSubTree(substr($idstr,0,-1));
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}


	public function renameProfile($requestData){
		try{
			if($requestData->level==1){
				$qry = "call slp_profilelist_ren('".substr($requestData->id,1)."', '". $requestData->newname."')";
			}else{
				$qry = "call slp_profilelist_items_ren('".$requestData->id."', '". $requestData->newname."')";
			}
			expDebug::dPrint('Profile DAO renameProfile Query --->'.$qry);
			$this->connect();
			$this->beginTrans();
			$this->query($qry);
			
			//$result = $this->fetchAllResults();
			$this->commitTrans();
			$this->closeconnect();
			//return $result;
			return;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function deleteProfile($requestData){
		try{
			
			$ids = $this->getSubTreeIds("'".$requestData->id."'", $requestData->level);
			$ids = substr($ids,0,-1);
			
			$this->connect();
			$this->beginTrans();
			if($requestData->level==1){
				if(!empty($ids)){
					$query = "call slp_profilelist_items_del(\"".$ids."\")" ;
					expDebug::dPrint("level = 1 1". $query);
					$this->query($query);
				}
				$query = "call slp_profilelist_del('".substr($requestData->id,1)."')" ;
				expDebug::dPrint("level = 1 2". $query);
				$this->query($query);
			}else{
				if(!empty($ids))
					$delIds = "'". $requestData->id ."',".$ids;
				else
					$delIds = "'". $requestData->id ."'";
				$query = "call slp_profilelist_items_del(\"".$delIds."\")" ;
				expDebug::dPrint("level != 1". $query);
				$this->query($query);
			}
			$this->commitTrans();
			$this->closeconnect();
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getSubTreeIds($Ids, $level){
		try {
			$idstr ="";
			/*if(empty($Ids))
				return "";*/
			expDebug::dPrint("getSubTreeIds ".$level);
			if($level==1) // will be true its level 1 node and only on first call
	//			$query = "SELECT id FROM slt_profile_list_items WHERE listid IN ('".substr(trim($Ids),2) .") ORDER BY id";
				$query = "call slp_profilelist_items_sel('id',\"list_id IN ('".substr(trim($Ids),2) .") ORDER BY id\")";
			else
	//			$query = "SELECT id FROM slt_profile_list_items WHERE parentid IN (".$Ids .") ORDER BY id"; //listid IS NULL AND
				$query = "call slp_profilelist_items_sel('id',\"parent_id IN (".$Ids .") ORDER BY id\")"; //listid IS NULL AND
			
				
			expDebug::dPrint("del ids ".$query);
			$this->connect();
			$this->query($query);
			$result = $this->fetchAllResults();
			$this->closeconnect();
	
			if(count($result)==0)
				return $idstr;
			for($i=0;$i<count($result);$i++){
				$idstr .= "'".$result[$i]->id ."',";
			}
			$idstr = substr($idstr,0,-1);
			return $idstr.",".$this->getSubTreeIds($idstr, 0);
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}	
	}

	public function getProfileDets($inObj){
		try{
			
			if($inObj->level==1)
				$query = "CALL slp_profilelist_sel('*','id=\"".substr($inObj->id,1) ."\"')";
			else
			$query = "CALL slp_profilelist_items_sel('*','id=\"".$inObj->id ."\"')";
			//$query = "CALL slp_profilelist_items_sel('*','id=\"".$inObj->id ."\"')";
			
			expDebug::dPrint("getProfileDets ".$query);
			$this->connect();
			//$this->beginTrans();
			$this->query($query);
			//$this->commitTrans();
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
//			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getProfileLangCode($inObjLangCode){
		try{
			$query = "CALL slp_profilelangcode_sel('code=\"".$inObjLangCode."\"')";
			expDebug::dPrint("getProfileLangcode ".$query);
			$this->connect();
			$this->query($query);
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
		
	}
	
	public function getProfileCode($inObj){
		try{
			$query = "CALL slp_get_profile_details('".$inObj->id."',@out)";
			
			expDebug::dPrint("getProfileCode ".$query);
			$this->connect();
			$this->query($query);
			
			//$this->commitTrans();
			expDebug::dPrint("committed..");			
			//$res = $this->query("select @out as parent_list");
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function updateProfile($inObj){
		try{
		/*	if($inObj->root!="R")
				$inObj->level++;*/
			$vUserId = $this->getLoggedInUserId();
			$inObj->code = str_replace(" ", "_", $inObj->code);
			$inObj->code = ereg_replace("[^A-Za-z0-9\_]", "", $inObj->code); 
			 		
			if($inObj->level==1)
				$query = "CALL slp_profilelist_upd('".substr($inObj->id,1) ."','".$inObj->code."','".$inObj->language_code."','".$inObj->name ."','".$inObj->type."','".$inObj->description."','".$inObj->attr1."','".$inObj->attr2."','".$inObj->attr3."','".$inObj->attr4."','".$inObj->is_active ."','".$vUserId."',@insval)";
			else
				$query = "CALL slp_profilelist_items_upd('".$inObj->id ."','".$inObj->code."','".$inObj->language_code."','".$inObj->name ."','".$inObj->attr1."','".$inObj->attr2."','".$inObj->attr3."','".$inObj->attr4."','".$inObj->is_active ."','".$vUserId."',@insval)";
			expDebug::dPrint("updateProfile ".$query);
			$this->connect();
			$this->beginTrans();
			$this->query($query);
			$this->commitTrans();
			//$result = $this->fetchAllResults();

			$this->closeconnect();
			//return $result;
			return;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function getProfileByType($inObj){
		try{
			$query = "CALL slp_profilelist_sel_typ('".$inObj->type ."')";
			expDebug::dPrint("getProfileByType ".$query);
			$this->connect();
			//$this->beginTrans();
			$this->query($query);
			//$this->commitTrans();
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function searchProfile($inObj){
		try{
			$query = "CALL slp_profilelist_sel_ser('".$inObj->SearchKey->name."','". $inObj->SearchKey->value ."')";
			expDebug::dPrint("searchProfile".$query);
			$this->connect();
//			$this->beginTrans();
			$this->query($query);
//			$this->commitTrans();
			$result = $this->fetchAllResults();
			$this->closeconnect();
			return $result;
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	public function saveProfileMapping($requestData){
		try{
			//	expDebug::dPrint("count ".count($requestData->rowset));
			$this->connect();
			$this->beginTrans();
			$query = "CALL slp_taggingmaster_del('". $requestData->id1 ."')";
			expDebug::dPrint("saveProfile".$query);
			$this->query($query);
			for($i=0;$i<count($requestData->rowset);$i++){
				if(count($requestData->rowset)==1)
					$rowset = $requestData->rowset;
				else
					$rowset = $requestData->rowset[$i];
				//		expDebug::dPrint("count map ".$i." ".count($rowset->mapping));
				for($j=0;$j<count($rowset->tags->mapping);$j++){
					$tags="";
					if(count($rowset->tags->mapping)==1)
						$tags = $rowset->tags->mapping;
					else
						$tags = $rowset->tags->mapping[$j];
					
					$query ="CALL slp_taggingmaster_ins('". $requestData->id1 ."','". $requestData->parent_id ."','".$tags."','". $rowset->attr->type ."','". $rowset->attr->value ."','". $i ."','','','','')";
					expDebug::dPrint("query ".$i." ".$query);
					$this->query($query);
				}
			}
			$this->commitTrans();
			$this->closeconnect();
		}catch(Exception $e){
			$this->rollbackTrans();
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getMapAttribute($mapAttr){
		try{
			$query = "CALL slp_list_items_sel('". $mapAttr ."', NULL)";
			expDebug::dPrint("query ".$query);
			$this->connect();
			$this->query($query);
			$res = $this->fetchAllResults();
			$outStr1="";
			for($i=0;$i<count($res);$i++){
				$outStr1 .= "<mapAttr>";
				$outStr1 .= "<id>".$res[$i]->Id."</id>";
				$outStr1 .= "<name>".$res[$i]->Name."</name>";
				$outStr1 .= "</mapAttr>";
			}
			$outStr1 = "<MapAttribute>". $outStr1 ."</MapAttribute>";
			expDebug::dPrint("DAO mapAttrData ".$outStr1);
			$this->closeconnect();
			return $outStr1;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getSelectedNodes($id1, $parentId){
		try{
//			$query = "SELECT id, tags, `value`, `type`, custom0 FROM slt_tagging_master WHERE id1='". $id1 ."'";
//			$query = "call slp_taggingmaster_sel('id, tags, `value`, `type`, custom0',\"id1='". $id1 ."'\")";
			$whereClause = "id1='". $id1 ."'";
			if( isset($parentId) && !empty($parentId) )
				$whereClause .= " AND parent_id='". $parentId ."'";
			$whereClause .= " ORDER BY custom0, id";
			$query = "call slp_taggingmaster_sel('id, tags, `value`, `type`, custom0',\"".$whereClause."\")";
			expDebug::dPrint("getSelectedNodes ".$query);
			$this->connect();
			$this->query($query);
			$results = $this->fetchAllResults();
			$selNodes = array();
			$lastCustom = "";
			$idx=0;
			$type = array();
			$value = array();
			if(count($results)==0)
				return "";
			for($i=0;$i<count($results);$i++){
				if($lastCustom!="" && $lastCustom != $results[$i]->custom0){
					$idx++;
					$type[]=$results[$i]->type;
					$value[]=$results[$i]->value;
				}
				if($lastCustom==""){
					$type[]=$results[$i]->type;
					$value[]=$results[$i]->value;
				}
				$tokens = explode(",", $results[$i]->tags);
				expDebug::dPrint("tokens count ".count($tokens));
				for($j=0;$j<count($tokens);$j++){
					$subTokens = explode(":",$tokens[$j]);
					//if(count($selNodes[$idx][$subTokens[0]])==0 || !array_search($subTokens[1],$selNodes[$idx][$subTokens[0]]))
					$flg=false;
					for($k =0;$k<count($selNodes[$idx][$subTokens[0]]);$k++){
						if($selNodes[$idx][$subTokens[0]][$k]==$subTokens[1]){
							$flg = true;
							break;
						}
					}
					if(!$flg)
						$selNodes[$idx][$subTokens[0]][]=$subTokens[1];
					
				}
				$lastCustom = $results[$i]->custom0;
				
			}
			expDebug::dPrint("selnodes count ".count($selNodes));
			for($i=0;$i<count($selNodes);$i++){
				expDebug::dPrint("selnodes i count ".count($selNodes[$i]));
				$selNodesXml .= "<row>"; 
				foreach($selNodes[$i] as $key=>$valueArr){
					$selNodesXml .= "<nodes qulfy=\"".$key ."\">";
					$nodes = "";
					for($j=0;$j<count($valueArr);$j++){
						$nodes .= $valueArr[$j].",";
					}
					$selNodesXml .= substr($nodes,0,-1);
					$selNodesXml .= "</nodes>";
				}
				$selNodesXml .= "<type>".$type[$i]."</type>";
				$selNodesXml .= "<value>".$value[$i]."</value>";
				$selNodesXml .= "</row>";
			}
			$this->closeconnect();
			$selNodesXml = "<selNodes>" .$selNodesXml ."</selNodes>";
			return $selNodesXml;
		}catch(Exception $e){
			
		}
	}
	

	private function getId($name,$level){
		try{
			if($level==1){
//				$query = "select id from slt_profile_list where name = '". $name ."'";
				$query = "call slp_profilelist_sel('id', \"name = '". $name ."' \")" ;
				expDebug::dPrint("getId ".$query); 
				$this->connect();
				$this->query($query);
				$res = $this->fetchAllResults();
				if(count($res)==0){
					throw new SoapFault("SPLMS","Invalid Name ".$name);
				}			
				$this->closeconnect();
				return "F".$res[0]->id;
			}else{
				if($level==1){
					//$query = "select b.id as id, b.name as name from slt_profile_list a, slt_profile_list_items b where a.id = b.listid and parentid is null and name = '". $name ."'";
					$query = "CALL slp_profilelist_name('".$name."')";
					expDebug::dPrint("getId ".$query); 
					$this->connect();
					$this->query($query);
					$res = $this->fetchAllResults();
					if(count($res)==0){
						throw new SoapFault("SPLMS","Invalid Name ".$name);
					}
					$this->closeconnect();
					return $res[0]->id;
				}else{
					return $this->getListItemId($name,$level);
				}
			}
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());	
		}
	}
	
	private function getListItemId($name,$level){
		try{
//		$query = "select id from slt_profile_list_items where name='".$name."'";
			$query = "call slp_profilelist_items_sel('id', \"name='".ServiceBase::stripRegStatus($name)."'\")";
			expDebug::dPrint("getListItemId ".$query);
			$this->connect();
			$this->query($query);
			$res = $this->fetchAllResults();
			$lvl;
			$id;
			if(count($res)==0)
				throw new SoapFault("SPLMS","Invalid Name ".$name);
			if(count($res)>1){
				for($i=0;$i<count($res);$i++){
					$lvl = $this->getLevel($res[$i]->id,2);
					if($lvl == $level){
						if(isset($id)){
							throw new SoapFault("SPLMS","Duplicate entry in the same level ".$name);	
						}
						else
							$id = $res[$i]->id;
					}
				}
			}else{
				$lvl = $this->getLevel($res[0]->id,2);
				expDebug::dPrint("getlevel ".$lvl);
				if($lvl != $level){
					throw new SoapFault("SPLMS","Level mismatch ".$name);
				}
				else
					$id = $res[0]->id;
			}
			
			$this->closeconnect();
			return $id;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	private function getLevel($id, $level){
		try{
			$lvl;
//			$query = "select parentid from slt_profile_list_items where id='".$id."'";
			$query = "call slp_profilelist_items_sel('parent_id',\"id='".$id."'\")";
			expDebug::dPrint("getlevel ".$query);
			$this->connect();
			$this->query($query);
			$res = $this->fetchAllResults();
			if(count($res)>0){
				$this->closeconnect();
				if(!isset($res[0]->parentid))	
					return $level;
				$lvl = $this->getLevel($res[0]->parentid,$level+1);	
			}
			return $lvl;
			
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}		
	}

	public function getProfileAsync($reqData){
		try{
			
			$iniId = $reqData->id;
			expDebug::dPrint("getProfileAsync level".$reqData->level);
			if($reqData->root!="R" && $reqData->id==0 ){
				$reqData->id = $this->getId($reqData->root, $reqData->level);
				expDebug::dPrint("getProfileAsync id ".$reqData->id);
			}			
			
			if($reqData->level==0){
				$qry = "CALL slp_profilelist_stats('L0','0')";
			}elseif($reqData->level==1){
				$qry = "CALL slp_profilelist_stats('L1','".substr($reqData->id,1)."')";
			}else{
				$qry = "CALL slp_profilelist_stats('L2','".$reqData->id."')";
			}	

			expDebug::dPrint("getProfileAsync ".$qry);
			$this->connect();
			$this->query($qry);
			$results = $this->fetchAllResults();
			$retXml="<root>";
			if($reqData->level==0) // && $reqData->root=="R"
				$retXml .='<item parent_id="0" id="R" state="open"><content><name>Lookup Master</name></content></item>';
			else if($iniId=="0"){
				$crVal=$reqData->root;
				$isutfstr = mb_detect_encoding($crVal,"ASCII");
				if (is_string($isutfstr)){
					$crVal = htmlspecialchars($crVal);
					//$crVal = $crVal;
				}
				$retXml .='<item parent_id="0" id="'.$reqData->id.'" state="open" ><content><name>'.$crVal.'</name></content></item>';
			}
				
				
			for($i=0;$i<count($results);$i++){
				$retXml .= '<item ';
				if($reqData->level==0){
					$retXml .= ' id="F'. $results[$i]->id .'"';
					$retXml .= ' parent_id="R"';
					$retXml .= ' attr3="'. $results[$i]->attr3 .'"';
					$retXml .= ' attr4="'. $results[$i]->attr4 .'"';
				}
				else if($iniId=="0"){
					$retXml .= ' parent_id="'.$reqData->id.'"';
					$retXml .= ' id="'. $results[$i]->id .'"';
					$retXml .= $this->loadStats($results[$i]);
					$retXml .= ' attr3="'. $results[$i]->attr3 .'"';
					$retXml .= ' attr4="'. $results[$i]->attr4 .'"';
				}
				else{
					$retXml .= ' id="'. $results[$i]->id .'"';
					$retXml .= $this->loadStats($results[$i]);
					$retXml .= ' attr3="'. $results[$i]->attr3 .'"';
					$retXml .= ' attr4="'. $results[$i]->attr4 .'"';
				}
				
				if($results[$i]->children>0)
					$retXml .= ' state="closed">';
				else
					$retXml .= '>';	
				$retXml .= '<content>';
				$retXml .= '<name>'.htmlspecialchars($results[$i]->name).'</name>';
				$retXml .= '<desc>Description</desc>';
				$retXml .= "</content>";
				$retXml .= "</item>";
			}
			$retXml .= "</root>";
//			expDebug::dPrint("root ".$retXml);
			$this->closeconnect();
			return $retXml;
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function getProfileAsyncByReST($reqData){
		try{
			
			$iniId = $reqData->id;
			expDebug::dPrint("getProfileAsync level".$reqData->level);
			if($reqData->root!="R" && $reqData->id==0 ){
				$reqData->id = $this->getId($reqData->root, $reqData->level);
				expDebug::dPrint("getProfileAsync id ".$reqData->id);
			}			
			
			if($reqData->level==0){
				$qry = "CALL slp_profilelist_stats('L0','0')";
			}elseif($reqData->level==1){
				$qry = "CALL slp_profilelist_stats('L1','".substr($reqData->id,1)."')";
			}else{
				$qry = "CALL slp_profilelist_stats('L2','".$reqData->id."')";
			}	

			expDebug::dPrint("getProfileAsync ".$qry);
			$this->connect();
			$this->query($qry);
			$results = $this->fetchAllResults();
			return $results;
			$this->closeconnect();
		}catch(Exception $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	private function loadStats($result){
		try {
			$retXml = ' stats1="'.$result->stats1.'"';
			$retXml .= ' stats1_desc="'.$result->stats1_desc.'"';
			$retXml .= ' stats2="'.$result->stats2.'"';
			$retXml .= ' stats2_desc="'.$result->stats2_desc.'"';
			$retXml .= ' stats3="'.$result->stats3.'"';
			$retXml .= ' stats3_desc="'.$result->stats3_desc.'"';
			$retXml .= ' stats4="'.$result->stats4.'"';
			$retXml .= ' stats4_desc="'.$result->stats4_desc.'"';
			return $retXml;
		}catch(Excepion $e){			
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}

	
	public function filterNodesOnAttr($attr){
		try{
			$this->connect();
			$query = "call slp_profilelist_attr_sel('". $attr."')";
			expDebug::dPrint("Filter___".$query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Excepion $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function fetchProfileListValues($listname,$parents,$searchkey,$sublvl){
		try{
			$this->connect();
			$query = "call slp_profilelist_val_sel('". $listname."',";
			$query .= isset($parents) ? "'".$parents."'," : "null,";
			$query .= "'".$searchkey."',";
			$query .= "'".$sublvl."')";
			expDebug::dPrint($query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Excepion $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function fetchCntryList($cntry){
		try{
			$this->connect();
			$query = "call slp_cntrylist_sel('". $cntry."')";
			expDebug::dPrint($query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Excepion $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
	
	public function fetchStateList($cntry,$state){
		try{
			$this->connect();
			$query = "call slp_statelist_sel('". $cntry."', '".$state."')";
			expDebug::dPrint($query);
			$this->query($query);
			$results = $this->fetchAllResults();
			$this->closeconnect();
			return $results;
		}catch(Excepion $e){
			$this->closeconnect();
			throw new SoapFault("SPLMS",$e->getMessage());
		}
	}
}

/*
function callProfileValuesAPI($level,$id,$root){
	$obj=new ProfileDAO();
	if($level!="" && $level=="001")
				$id="F".$id;
	$paramsobj=new stdclass();
	$paramsobj->level = $level;
	$paramsobj->id = $id;
	$paramsobj->root = $root;
	$result=$obj->getProfileAsyncByReST($paramsobj);
	return $result;
}
*/
function callProfileValuesAPI($paramsobj){
	$paramsobj=array_to_object($paramsobj);
	expDebug::dPrint("test params");
	expDebug::dPrint($paramsobj);
	
	$obj=new ProfileDAO();
	if($paramsobj->level!="" && $paramsobj->level=="001")
				$paramsobj->id="F".$paramsobj->id;
	
	$result=$obj->getProfileAsyncByReST($paramsobj);
	return $result;
}
?>