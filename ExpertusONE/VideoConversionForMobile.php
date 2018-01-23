<?php //Trunk
ini_set('max_execution_time', 60*60); //3600 seconds = 60 minutes
define('DRUPAL_ROOT', getcwd());
include_once "./includes/common.inc";
include_once "./includes/database/database.inc";
include_once './includes/bootstrap.inc';
include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/exp_sp_core.inc";

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

class VideoConversionForMobile {

  public function process(){    
    $this->VideoConversionForMobile_MigrationScript();
  
  }
  
  function needsToBeConverted($src)
  {
  
  	//$src= $fileUrl;//."/".$tokens[count($tokens)-1].".mp4";
  	
  	$obj = new stdClass();
  	if (file_exists($src))
  	{
  		
  		$tokens = split("/",$src);
  		$onlySrcFileName = $tokens[count($tokens)-1];
  		$dest = $src;
  		//generate mobile video file  name;
  		$dest = str_replace("/".$onlySrcFileName,"/mobile_".$onlySrcFileName, $dest);
  		if (file_exists($dest))
  		{
  			$srcFileName = "";
  			$mobileVideoFileName = "";
  				
  			$obj->is_conversion_needed = false;
  		}else
  		{
  			$obj->is_conversion_needed = true;
  			$obj->srcFileName = $src;
  			$obj->mobileVideoFileName = $dest;
  		}
  	}
  	expDebug::dPrint("ffmpeg: filename ".$src, 4);
  	return $obj;
  }
 
  function doVideoConversionForMobile($filename)
  {
  	$conversion_needed = getConfigValue("convert_video_for_mobile_access");
  	
  	//expDebug::dPrint("conversion_needed >>> $conversion_needed" , 4);
  	if(isset($conversion_needed) && $conversion_needed == "1")
  	{
  		include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_video_conversion_for_mobile.inc";
  		$tmp_arr = split("/",$filename);
  		$srcDir = "";
  		for($i=1;$i<count($tmp_arr)-1;$i++)
  		{
  			$srcDir.="/".$tmp_arr[$i];
  		}
  		$filenameAlone = get_video_filename_from_fs($srcDir);
  		$filename = $srcDir."/".$filenameAlone;
  		$is_installed = ffmpeg_installed();
	  	if($is_installed != true)
	  	{
	  		print "Video conversion module is not installed in this server.<br>";
	  		exit();
	  	}
	  	
	  	$stdObj = $this->needsToBeConverted($filename);
	  	
	  	if($stdObj->is_conversion_needed == true)
	  	{
	  		expDebug::dPrint("ffmpeg: conversion needed for mobile php".$stdObj->srcFileName, 4);
	  		//convertVideoForMobileAccessForMP4($stdObj->srcFileName,$stdObj->mobileVideoFileName );
	  	}
  	}
  }
  function VideoConversionForMobile_MigrationScript()  {
  	include_once $_SERVER["DOCUMENT_ROOT"]."/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_manage/exp_sp_administration_content/exp_sp_video_conversion_for_mobile.inc";
  	$is_installed = ffmpeg_installed();
  	if($is_installed != true)
  	{
  		print "Video conversion module is not installed in this server.<br>";
  		exit();
  	}
  	global $user;
    $txn = db_transaction();
    
    
    try {
    	$sql = "select ver.id,ver.content_master_id,ver.content_sub_type, les.launchurl from slt_content_version ver ";
    	$sql.= " left join slt_content_lesson les on les.content_version_id = ver.id ";
    	$sql.= " where ver.content_sub_type = 'lrn_cnt_typ_vod' and ver.hosted_type = 1";
    	
        
        $queryResult =  db_query($sql);
        
        foreach ($queryResult as $contentInfo) {
            $stdObj = $this->needsToBeConverted($contentInfo->launchurl);
            expDebug::dPrint("ffmpeg: contentInfo->launchurl ".$contentInfo->launchurl, 4);
            if($stdObj->is_conversion_needed == true)
            {
            	expDebug::dPrint("ffmpeg: conversion needed ".$stdObj->srcFileName, 4);
            	convertVideoForMobileAccessForMP4($stdObj->srcFileName,$stdObj->mobileVideoFileName );
            }
        } //for Loop END
    
    } catch(Exception $e) {
	   $txn->rollback();
       unset($txn);
       watchdog_exception('VideoConversionForMobile', $ex);
       expertusErrorThrow($ex);
	}
	unset($txn);
	
  } //createNodeForCourseForum() END
  
  
} //DrupalActivityService END


/*if($user->uid==1) {
  
  $drupalServ=new VideoConversionForMobile();
  $rtn =  $drupalServ->process();
  print "Completed Successfully<br>";
  
} else {*/
	if($user->uid != 0)
	{
		$drupalServ=new VideoConversionForMobile();
		$fileName = $_SESSION["VODUploaded"];
		expDebug::dPrint("VODUploaded from SESSION:".$fileName , 4);
		if($fileName)
		{
			unset($_SESSION["VODUploaded"]);
			$drupalServ->doVideoConversionForMobile($fileName);
		}
	}else 
	{
	  print "You are not Authorized to access this Service.";
	}
           
/*}*/

?>