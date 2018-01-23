<?php 

function parseCrsFileData($pmArray){
	try {		
	if(is_array($pmArray)){
		$rtnArray = array();
		$group='';
		foreach($pmArray as $vRec){
			if(stripos($vRec,'[')!== false){
				$group=str_replace('[','',$vRec);
				$group=str_replace(']','',$group);
				$group = strtolower(rtrim(ltrim($group)));
			}else{
				$tmp=explode("=",$vRec);
				$rtnArray[$group][strtolower(rtrim(ltrim($tmp[0])))] = ltrim($tmp[1]);
			}
		}
		return $rtnArray;
	}	
	} catch (Exception $ex) {
		watchdog_exception('parseCrsFileData', $ex);
		expertusErrorThrow($ex);
	}		
}

function parseAuFileData($pmArray){
	try {
	if(is_array($pmArray)){
		$line=0;
		$pos=0;
		$rtnAry = array();
		$tmpAry = array();
		foreach($pmArray as $vRec){
			$vRec=str_replace(', "',',"',$vRec);
			$tmp=explode('",',$vRec);
			for($cnt=0;$cnt<sizeOf($tmp);$cnt++){
				$tmp[$cnt]=str_replace('"','',$tmp[$cnt]);
				if($line==0){
					$tmpAry[$cnt] = strtolower($tmp[$cnt]);
				}else{
					$rtnAry[$line-1][$tmpAry[$cnt]] = $tmp[$cnt];
				}
			}
			$line++;
		}		
		expDebug::dPrint("parseAuFileData Array --->>> ".print_r($rtnAry,true),4);
		return $rtnAry;
	}			
	} catch (Exception $ex) {
		watchdog_exception('parseAuFileData', $ex);
		expertusErrorThrow($ex);
	}
}

function ParseAICC($inObj){
	//Changes done by Vincent on Dec 19, 2013 for #0030276 
try {	
$aUploadedCrsFileData = parseCrsFileData(file($inObj->crsContentURLPath,FILE_IGNORE_NEW_LINES));
$aUploadedDesFileData = parseAuFileData(file($inObj->desContentURLPath,FILE_IGNORE_NEW_LINES));
$aUploadedAuFileData = parseAuFileData(file($inObj->auContentURLPath,FILE_IGNORE_NEW_LINES));


$data[0]=new StdClass();
$data[0]->TotlaLesson = $aUploadedCrsFileData['course']['total_aus'];
$data[0]->CourseTitle = $aUploadedCrsFileData['course']['course_title'];
expDebug::dPrint("Parsing AICC Start --- ");
expDebug::dPrint("AU Array -->".print_r($aUploadedAuFileData,true) , 4);
expDebug::dPrint("CRS Array -->".print_r($aUploadedCrsFileData,true) , 4);
expDebug::dPrint("DES Array -->".print_r($aUploadedDesFileData,true) , 4);
expDebug::dPrint("Total Lessons : ".$data[0]->TotlaLesson , 4);
if($data[0]->TotlaLesson==1){
	$weblaunch=$aUploadedAuFileData[0]["web_launch"]; // parseAuFileData($aUploadedAuFileData, 'Web_Launch', 1); 
	$data[1]=new StdClass();
	$data[1]->LaunchURL = $aUploadedAuFileData[0]["file_name"]; // parseAuFileData($aUploadedAuFileData, 'file_name', 1); 
	if(isset($weblaunch) && !empty($weblaunch))
	{
		$data[1]->LaunchURL=$data[1]->LaunchURL.'?'.$weblaunch;
	}
  $data[1]->MasteryScore = $aUploadedAuFileData[0]["mastery_score"]; // parseAuFileData($aUploadedAuFileData, 'mastery_score', 1); 
	$data[1]->Title = $aUploadedDesFileData[0]["title"]; // parseAuFileData($aUploadedDesFileData,'title',1);
	$other_info = '{';
	$other_info .= '"Type":"'.$aUploadedAuFileData[0]["type"].'"';
	$other_info .= ',"Command_Line":"'.$aUploadedAuFileData[0]["command_line"].'"';
	$other_info .= ',"Max_Score":"'.$aUploadedAuFileData[0]["max_score"].'"';
	$other_info .= ',"Max_Time_Allowed":"'.$aUploadedAuFileData[0]["max_time_allowed"].'"';
	$other_info .= ',"Time_Limit_Action":"'.$aUploadedAuFileData[0]["time_limit_action"].'"';
	$other_info .= ',"System_Vendor":"'.$aUploadedAuFileData[0]["system_vendor"].'"';
	$other_info .= ',"Core_Vendor":"'.$aUploadedAuFileData[0]["core_vendor"].'"';
	$other_info .= ',"AU_Password":"'.$aUploadedAuFileData[0]["au_password"].'"';
	$other_info .= '}';
	$data[1]->datafromlms = $other_info;
	expDebug::dPrint("LaunchURL - ".$data[1]->LaunchURL , 4);
	expDebug::dPrint("MasteryScore - ".$data[1]->MasteryScore , 4);
	expDebug::dPrint("Title - ".$data[1]->Title , 4);
	expDebug::dPrint("weblaunch - ".$weblaunch , 4);
	
}else{
	for($x=1;$x<=$data[0]->TotlaLesson;$x++){
		$weblaunch=$aUploadedAuFileData[$x-1]["web_launch"]; // parseAuFileData($aUploadedAuFileData, 'Web_Launch', $x);
		$data[$x]=new StdClass();
		$data[$x]->LaunchURL = $aUploadedAuFileData[$x-1]["file_name"]; // parseAuFileData($aUploadedAuFileData, 'file_name', $x); 
		if(isset($weblaunch) && !empty($weblaunch))
		{
			$data[$x]->LaunchURL=$data[$x]->LaunchURL.'?'.$weblaunch;
		}
  	$data[$x]->MasteryScore = $aUploadedAuFileData[$x-1]["mastery_score"]; // parseAuFileData($aUploadedAuFileData, 'mastery_score', $x); 
		$data[$x]->Title = $aUploadedDesFileData[$x-1]["title"]; // parseAuFileData($aUploadedDesFileData,'title',$x);
		
		$other_info = '{';
		$other_info .= '"Max_Score":"'.$aUploadedAuFileData[$x-1]["max_score"].'"';
		$other_info .= ',"Max_Time_Allowed":"'.$aUploadedAuFileData[$x-1]["max_time_allowed"].'"';
		$other_info .= ',"Time_Limit_Action":"'.$aUploadedAuFileData[$x-1]["time_limit_action"].'"';
		$other_info .= ',"System_Vendor":"'.$aUploadedAuFileData[$x-1]["system_vendor"].'"';
		$other_info .= ',"Core_Vendor":"'.$aUploadedAuFileData[$x-1]["core_vendor"].'"';
		$other_info .= ',"AU_Password":"'.$aUploadedAuFileData[$x-1]["au_password"].'"';
		$other_info .= '}';
		$data[$x]->datafromlms = $other_info;
	}
}
expDebug::dPrint($data , 4);
return $data;
} catch (Exception $ex) {
	watchdog_exception('ParseAICC', $ex);
	expertusErrorThrow($ex);
}
}
?>