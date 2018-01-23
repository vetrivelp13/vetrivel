<?php
/**
 *  File used to convert documents to PDF formats 
 *  
 *  
 * Coulumn used in the convertion table
 *  
 * custom0 - Task status
 * custom1 - Start Time
 * custom2 - End Time
 * 
 */
if (in_array($argv, array('--help')) || $argc != 5) {
	?>
  This is a script to run cron from the command line.
 
  It takes 2 arguments, which must both be specified:
  
  --limit is the file size(in KB), a filter to be applied for the execution. 
  limit defaults to 3000  
  
  --document-root is the path to your drupal root directory for
  your website.
 
  Usage:
  php converter.php --limit 3000 --document-root '/path/to/drupal/root'
  
  Status:
  ---------------------------------------------------------------------
  N - New Conversion Task
  C - Current / Failure Task
  S - Skipped Task (PDF and Image extenstion Skipped)
 
<?php
} else {
  // Loop through arguments and extract the cron key and drupal root path.
  for ($i = 1; $i < $argc; $i++) {
    switch ($argv[$i]) {
      case '--limit':
        $i++;
        $limit = $argv[$i];
        break;
      case '--document-root':
        $i++;
        $path = $argv[$i];
        break;
    }
  }

	//set_time_limit(0);
	// Sets script name
	if (empty($limit)) {
		$limit = 3000;
	}
	
	
	$_SERVER['SCRIPT_NAME'] = 'converter.php';
	
	// Values as copied from drupal.sh
	$_SERVER['HTTP_HOST'] = 'default';
	$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
	$_SERVER['SERVER_SOFTWARE'] = NULL;
	$_SERVER['REQUEST_METHOD'] = 'GET';
	$_SERVER['QUERY_STRING'] = '';
	$_SERVER['HTTP_USER_AGENT'] = 'console';
	$_SERVER['DOCUMENT_ROOT'] = $path;
	
	define('DRUPAL_ROOT', $path);
	include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
	drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
	
	expDebug::dPrint('Filesize Limit: ' . $limit);
	
	/**
	 * Update applicable records to N status
	 */
	$updateStmt = db_update('slt_content_version');
	$updateStmt->fields(array('custom0' => 'N'));
	$updateStmt->condition('content_sub_type', 'lrn_cnt_typ_knc','=');
	$updateStmt->condition('hosted_type', '1','='); // only select upload type
	$updateStmt->condition('status', 'lrn_cnt_sts_del','!=');
	$updateStmt->isNull('custom0');
	expDebug::dPrintDBAPI('update content version query ', $updateStmt);
	$updateStmt->execute();
	
	/**
	 * select applicable records to convert
	 */
	$selectStmt = db_select('slt_content_version','ver');
	$selectStmt->addField('ver','id', 'id');
	$selectStmt->addField('ver','content_master_id', 'content_master_id');
	$selectStmt->addField('ver','title', 'title');
	$selectStmt->addField('ver','file_path', 'file_path');
	$selectStmt->addField('ver','file_size', 'file_size');
	$selectStmt->condition('ver.status', 'lrn_cnt_sts_del','!=');
	$selectStmt->condition('ver.content_sub_type', 'lrn_cnt_typ_knc','=');
	$selectStmt->condition('ver.hosted_type', '1','='); // only select upload type 
	$selectStmt->condition('ver.custom0', 'N','=');
	expDebug::dPrintDBAPI('select content version query ', $selectStmt);
	$result = $selectStmt->execute()->fetchAll();
	
	expDebug::dPrint('content to process' . print_r($result, 1));
	$config=getConfig("exp_sp");
	$jod_converter_url = $config['jod_converter_url'];
	$content_upload_path = $config['content_upload_path'];
	$image_ext =  array('gif','png' ,'jpg', 'jpeg','pdf');
	foreach($result as $file_details){
		$dir = DRUPAL_ROOT . substr($file_details->file_path,1);
		
		if (is_dir($dir)){
			if ($dh = opendir($dir)){
				while (($file = readdir($dh)) !== false){
					if($file!='.' && $file!='..' && !empty($file)){
						$source_file = $file_details->file_path.'/'.$file;
						//echo "filename:" . $file_details->file_path.'/'.$file . "<br>";
						expDebug::dPrint('file information: ' . print_r($file, 1));
						//find expention of upload file
						$file_arr  = explode(".", $file);
						$file_arr_cnt = count($file_arr);
						$file_name = '';
						for($i=0;$i<($file_arr_cnt-1);$i++){
							if($file_name=='')
								$file_name=$file_arr[$i];
							else
								$file_name=$file_name.'.'.$file_arr[$i];
						}
						$ext = strtolower(end(($file_arr)));
						$fileSize = fileSizeConvertToKiloBytes($file_details->file_size);
						if(!in_array($ext, $image_ext) && $fileSize <= $limit){
							$uri_arr = explode('/',$file_details->file_path);
							$cnt = count($uri_arr);
							$designation_folder = $uri_arr[$cnt-1];
							//create convertion folder name
							$source_path = $content_upload_path.'/contentupload/'.$designation_folder.'/'.$file;
							$filetype = _mime_content_type($source_path);
							$designation_folder = $content_upload_path.'/contentupload/convertioncontent/'.$designation_folder;
							$oldmask = umask(0);
							mkdir($designation_folder, 0777,true);
							umask($oldmask);
							chown($designation_folder, $user_name);
							$designation_path = $designation_folder.'/'.$file_name.'.pdf';
							chown($designation_path, $user_name);
							if(is_dir($designation_folder)){
								$updateQuery = db_query("update slt_content_version set custom0 = 'C', custom1 = now() where id = :id",  array(":id" => $file_details->id)); // update task as current
								if($ext!='pdf' && $ext!='png' && $ext!='gif' && $ext!='jpeg' && $ext!='jpg'){
									$comment = 'wget '.$jod_converter_url.' \    --post-file='.$source_path.' \    --header="Content-Type: '.$filetype.'" \    --header="Accept: application/pdf" \    --output-document='.$designation_path;			
									expDebug::dPrint(' jod convetion comment->' . print_r($comment, true) , 4);
									exec($comment,$output);
									$updateQuery = db_query("update slt_content_version set custom0 = 'Y', custom2 = now() where id = :id", array(":id" => $file_details->id)); // update task as success
								}
							}
						} else {
							if (in_array($ext, $image_ext)) {
								$updateQuery = db_query("update slt_content_version set custom0 = 'S', custom1 = now() where id = :id",  array(":id" => $file_details->id)); // update current task as skipped
							}	
						}
		
					}
				}
				closedir($dh);
			}
		}
	}
	echo 'completed';
}

function fileSizeConvertToKiloBytes($fileSize) {
	$outValue = 0;
	if (empty($fileSize)) {
		return $outValue;
	}
	$fileParts = explode(" ", $fileSize);
	$unitValue = $fileParts[0];
	$unitText  = strtolower($fileParts[1]);
	$roundValue = 1000;
	switch($unitText) {
		case 'kb':
			$outValue = $unitValue;
			break;
		case 'mb':
			$outValue = $unitValue * $roundValue;
			break;
		case 'gb':
			$outValue = $unitValue * $roundValue * $roundValue; 
			break;
		case 'tb':
			$outValue = $unitValue * $roundValue * $roundValue * $roundValue;
			break;
		default:
			$outValue = 0;
			break;	
	}
	return $outValue;
}
// echo '<pre>';print_r($result);echo '</pre>';
// //expDebug::dPrint("FILES DATAS----------> ".print_r($result,true),1);
// exit;


// $it = new RecursiveDirectoryIterator("sites/default/files/contentupload/");
// $display = Array ( 'jpeg', 'jpg','txt','doc', 'docx','ppt','pptx','pps','ppsx','xls','xlsx','png','gif');
// foreach(new RecursiveIteratorIterator($it) as $file)
// {
//     if (in_array(strtolower(array_pop(explode('.', $file))), $display))
//         echo $file . "<br/> \n";
// }
?>