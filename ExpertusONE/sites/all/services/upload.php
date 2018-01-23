<?php

include "../dao/inc_ScormParser.php";
include "Trace.php";
include "GlobalUtil.php";

// Load the Zip class
require_once('ZipFolder.php');
expDebug::dPrint("upload.php called...");
if (is_uploaded_file($_FILES['Filedata']['tmp_name'])){
	$uploadDirectory="../../default/files/contentupload/";
	$uploadFile=$uploadDirectory.basename($_FILES['Filedata']['name']);
	$fileName = basename($_FILES['Filedata']['name'],".zip");
	copy($_FILES['Filedata']['tmp_name'], $uploadFile);
}


expDebug::dPrint("uploadFile >>> $uploadFile");
expDebug::dPrint("fileName >>> $fileName");
// Create a class object
$folder = new ZipFolder();

// Set the load path of the compressed archive
//$folder->setLoadPath("D:/wamp/www/test/zipfolder/");

// Set the zip file to be uncompressed
$folder->setFile($uploadFile);

// Set the name of the folder where the zip file will be uncompressed
$folder->setFolder($uploadDirectory.$fileName);


// Set the save path of the uncompressed archive
//$folder->setSavePath("D:/wamp/www/test/zipfolder/");
// Once the parameters have been set we will uncompress the zip file
$folder->openZip();

// After the zip folder has been uncompressed we will delete it
//$folder->eraseZip();

$getFoldername  ="contentupload/";

$getFilename = basename($_FILES["Filedata"]["name"] ,".zip");
$setNewDirectory = $getFoldername.$getFilename;

$setFileExit = '../../default/files/contentupload/'.$getFilename.'';

$realPath='';

expDebug::dPrint("setNewDirectory >>> $setNewDirectory");

$gutil = new GlobalUtil();
$geturlpath = $gutil->getConfig();
$pathFromini = $geturlpath["content_upload_path"];
$urlpath  = $pathFromini.'/'.$setNewDirectory;

$val = $_REQUEST['contentvalue'];
$aval = explode(',',$val);
$vRowIndex = $aval[0];
$isKnowledgeContent =$aval[1];
$contentType = $aval[2];

expDebug::dPrint("urlpath >>> $urlpath");

if($contentType!="AICC" && $contentType!="AICC_TP"){
	if($isKnowledgeContent !='yes' && $contentType != 'lrn_cnt_typ_vod')
	{

		if(file_exists($setFileExit.'/imsmanifest.xml'))
		{
			$realPath = $urlpath.'/imsmanifest.xml';
		}
		else
		{
			$isSubDirectory = get_dirs($setFileExit);
			$realPath = $urlpath.'/'.$isSubDirectory.'/imsmanifest.xml';
		}

		$inObj = new stdClass();
		$inObj->ContentURLPath = $realPath;

		$inputValues = ContentMasterScormValidate($inObj);
	}
	else{
		$inputValues = ContentMasterKnowledgeContentValidate($urlpath);
	}
}else{
	include "../dao/inc_AICC_Parser.php";

	$files = glob("$setFileExit/{*.AU,*.au}",GLOB_BRACE);
	$aubasename;
	if(sizeOf($files)>0){
		$aubasename = basename($files[0]);
		$realPath_au = $urlpath.'/'.basename($files[0]);
		$files1 = glob("$setFileExit/{*.CRS,*.crs}",GLOB_BRACE);
		$realPath_crs = $urlpath.'/'.basename($files1[0]);
		$files2 = glob("$setFileExit/{*.CST,*.cst}",GLOB_BRACE);
		$realPath_cst = $urlpath.'/'.basename($files2[0]);
		$files3 = glob("$setFileExit/{*.DES,*.des}",GLOB_BRACE);
		$realPath_des = $urlpath.'/'.basename($files3[0]);

	}else{
		$isSubDirectory = get_dirs_aicc($setFileExit);
		$realPath = $setFileExit.'/'.$isSubDirectory;
		expDebug::dPrint("Real Path ".$realPath);
		$urlpath = $urlpath.'/'.$isSubDirectory;
		$files = glob("$realPath/{*.AU,*.au}",GLOB_BRACE);
		$aubasename = basename($files[0]);
		$realPath_au = $urlpath.'/'.basename($files[0]);
		$files1 = glob("$realPath/{*.CRS,*.crs}",GLOB_BRACE);
		$realPath_crs = $urlpath.'/'.basename($files1[0]);
		$files2 = glob("$realPath/{*.CST,*.cst}",GLOB_BRACE);
		$realPath_cst = $urlpath.'/'.basename($files2[0]);
		$files3 = glob("$realPath/{*.DES,*.des}",GLOB_BRACE);
		$realPath_des = $urlpath.'/'.basename($files3[0]);
	}
	$inObj = new stdClass();
	$inObj->auContentURLPath = $realPath_au;
	$inObj->crsContentURLPath = $realPath_crs;
	$inObj->cstContentURLPath = $realPath_cst;
	$inObj->desContentURLPath = $realPath_des;
	$inObj->baseFileName=$aubasename;
	$inObj->hostedType=$contentType=="AICC"?"EXP_HOSTED":"NON_EXP_HOSTED";

	$inputValues = ContentAICCValidate($inObj);
}
expDebug::dPrint("inputValues >>> ");
expDebug::dPrint($inputValues);

function get_dirs($dir){
	// global $dirs;
	$getDirectoryName='';
	// if (!isset($dirs)){$dirs = '';}
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$opendirectory = @opendir($dir.$file);
				while ($filedir = @readdir($opendirectory)) {
					if (preg_match("/imsmanifest.xml/",$filedir)) {
						$getDirectoryName = $file;
					}
				}
			}
		}
		@closedir($handle);
	}
	return $getDirectoryName;
}

function get_dirs_aicc($dir){
	// global $dirs;
	$getDirectoryName='';
	// if (!isset($dirs)){$dirs = '';}
	if(substr($dir,-1) !== '//'){$dir .= '//';}
	if ($handle = @opendir($dir)){
		while (false !== ($file = @readdir($handle))){
			if (filetype($dir.$file) === 'dir' && $file != "." && $file != ".."){
				clearstatcache();
				$opendirectory = @opendir($dir.$file);
				$f=glob("$dir/$file/{*.AU,*.au}",GLOB_BRACE);
				if(sizeOf($f)>0)
					$getDirectoryName = $file;
			}
		}
		@closedir($handle);
	}
	return $getDirectoryName;
}

function ContentMasterScormValidate($inObj) {
	$aResult = ParseScormIMSManifestFile($inObj->ContentURLPath);
	$aResult = array_values($aResult);
	$aContentInfo='';

	$val = $_REQUEST['contentvalue'];
	$aval = explode(',',$val);
	$vRowIndex = $aval[0];

	$outData="<Content>";
	$outData.="<RowIndex>$vRowIndex</RowIndex>";
	$outData .= "<totallesson>".count($aResult)."</totallesson>";

	/**
	 * $contentPath - Added by Vincent to get the relative path for the upload content
	 * for issue #5576 - On 07-Nov-2011
	 */
	$contentPath = substr_replace($inObj->ContentURLPath,'',0,strpos($inObj->ContentURLPath,'/sites'));
	for($vArrCount=0; $vArrCount < count($aResult); $vArrCount++) {

		$aContentInfo[$vArrCount]->title          = $aResult[$vArrCount]['title'];
		$aContentInfo[$vArrCount]->masteryscore   = $aResult[$vArrCount]['masteryscore'];
		$aContentInfo[$vArrCount]->href           = $aResult[$vArrCount]['href'];
		$aContentInfo[$vArrCount]->launchurl      = str_replace('imsmanifest.xml',$aResult[$vArrCount]['href'],$contentPath);
		$aContentInfo[$vArrCount]->datafromlms    = $aResult[$vArrCount]['datafromlms'];

		$outData .= "<LessonItem>";
		$outData .= "<title>".$aContentInfo[$vArrCount]->title."</title>";
		$outData .= "<masteryscore>".$aContentInfo[$vArrCount]->masteryscore."</masteryscore>";
		$outData .= "<launchurl>".$aContentInfo[$vArrCount]->launchurl."</launchurl>";
		$outData .= "<datafromlms>".$aContentInfo[$vArrCount]->datafromlms."</datafromlms>";
		$outData .= "</LessonItem>";
	}
	$outData .= "</Content>";
	return $outData;
}

function ContentAICCValidate($inObj){
	$aResult = ParseAICC($inObj);
	$aContentInfo='';

	$val = $_REQUEST['contentvalue'];
	$aval = explode(',',$val);
	$vRowIndex = $aval[0];
	
	$outData="<Content>";
	$outData.="<RowIndex>$vRowIndex</RowIndex>";
	$outData .= "<totallesson>".$aResult[0]->TotlaLesson."</totallesson>";
	$outData .= "<coursetitle>".$aResult[0]->CourseTitle."</coursetitle>";

	/**
	 * $contentPath - Added by Vincent to get the relative path for the upload content
	 * for issue #5576 - On 07-Nov-2011
	 */
	$contentPath = substr_replace($inObj->auContentURLPath,'',0,strpos($inObj->auContentURLPath,'/sites'));
	expDebug::dPrint($contentPath);
	for($vArrCount=1; $vArrCount < count($aResult); $vArrCount++) {

		$aContentInfo[$vArrCount]->title          = $aResult[$vArrCount]->Title;
		$aContentInfo[$vArrCount]->masteryscore   = $aResult[$vArrCount]->MasteryScore;
		//$aContentInfo[$vArrCount]->href           = $aResult[$vArrCount]['href'];
		if($inObj->hostedType=="EXP_HOSTED")
			$aContentInfo[$vArrCount]->launchurl      = str_replace($inObj->baseFileName,$aResult[$vArrCount]->LaunchURL,$contentPath);
		else
			$aContentInfo[$vArrCount]->launchurl      = $aResult[$vArrCount]->LaunchURL;
		//$aContentInfo[$vArrCount]->datafromlms    = $aResult[$vArrCount]['datafromlms'];
		expDebug::dPrint($aContentInfo[$vArrCount]->launchurl);
		$outData .= "<LessonItem>";
		$outData .= "<title>".$aContentInfo[$vArrCount]->title."</title>";
		$outData .= "<masteryscore>".$aContentInfo[$vArrCount]->masteryscore."</masteryscore>";
		$outData .= "<launchurl>".urlencode($aContentInfo[$vArrCount]->launchurl)."</launchurl>";
	//	$outData .= "<datafromlms>".$aContentInfo[$vArrCount]->datafromlms."</datafromlms>";
		$outData .= "</LessonItem>";
	}
	$outData .= "</Content>";
	return $outData;
}
 
function ContentMasterKnowledgeContentValidate($urlpath) {
	$val = $_REQUEST['contentvalue'];
	$aval = explode(',',$val);
	$vRowIndex = $aval[0];
	/**
	 * $urlpath - Added by Vincent to get the relative path for the upload content
	 * for issue #5576 - On 07-Nov-2011
	 */
	$urlpath = substr_replace($urlpath,'',0,strpos($urlpath,'/sites'));
	$outData="<Content>";
	$outData.="<RowIndex>$vRowIndex</RowIndex>";
	$outData.="<launchurlbase>".$urlpath."</launchurlbase>";
	$outData.= "</Content>";
	return $outData;
}

print $inputValues;
// print "&flashvar=$printVal";

?>