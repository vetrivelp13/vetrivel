<?php
$vDirName=time();

print "<pre>";
//print_r($_FILES);

function parseCrsFileData($pmArray, $pmKey){
			
	if(is_array($pmArray)){
		foreach($pmArray as $vRec){
		$aRow= explode("=",$vRec);
			if(trim($aRow[0])==	$pmKey){
				return $aRow[1];	
			}
		}		
	}			
}
  
function parseAuFileData($pmArray, $pmKey){
			
	if(is_array($pmArray)){
		foreach($pmArray as $vRec){
			$vStrPos = strpos($vRec,$pmKey); 
			if($vStrPos){				  
				$vStr = substr($vRec,$vStrPos,strlen($vRec));
				$aStr = explode('"',$vStr);
				return $aStr[0];
			}
			
			
		}		
	}			
}

 
$aUploadedCrsFileData = file($_FILES['file_crs']['tmp_name'],FILE_IGNORE_NEW_LINES);
$aUploadedAuFileData = file($_FILES['file_au']['tmp_name'],FILE_IGNORE_NEW_LINES);
$vLaunchURL = parseAuFileData($aUploadedAuFileData, 'http:'); 

$vCourseTitle =  parseCrsFileData($aUploadedCrsFileData,'Course_Title');
$vCourseTitle = rtrim(ltrim($vCourseTitle));

$vRawCourseTotal =  parseCrsFileData($aUploadedCrsFileData,'Total_AUs');
$vCourseTotal = rtrim(ltrim($vRawCourseTotal));
 
		
?>

<script>

  parent.$('#cm_title').val('<?php  print $vCourseTitle?>'); 
  parent.$('#cm_total_lesson').val(escape('<?php  print $vCourseTotal?>'));
  parent.$('#hid_contentmaster_launchurl').val('<?php  print $vLaunchURL?>');
  parent.$('#cm_url').val('<?php  print $vLaunchURL?>');
   
  parent.$('#msgDiv').css('display','block');
  parent.$('#msgDiv').html('Validated');
   
</script>