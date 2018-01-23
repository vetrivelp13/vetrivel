<?php
$shortopts  = "";
$longopts  = array(
    "document-root:",	// Required value
    "path:",    		// Required value
    "languages:",       // Optional value
    "help:",       // Optional value
);
$options = getopt($shortopts, $longopts);
echo "Given options are ".print_r($options, 1);
if(count($options) < 2 || in_array('help', $options)) {
?>
  ----------------------------------------------------------------------------------------------------------------------------------------
  import_po.php is a script to import PO files to Drupal system from the command line.
 
  It takes 3 arguments as follows
  --document-root (Mandatory paramter)is the absolute path to your drupal root directory for
	your website.
  
  --path (Mandatory paramter) can be an absolute path where the language files are available 
	or a url from which language files can be downloaded
  
  --languages (Optional parameter) is drupal understandable code of the langugae.
    One or more can be given in quotes with spaces in between if you want to 
	import specific set of langauges instead of all.
  
  Usage:
  php import_po.php --document-root "/expertusone/apache/ExpertusONE/" --path "https://expertusone.com/Languages.zip" --languages "de es"
  ----------------------------------------------------------------------------------------------------------------------------------------
<?php
} else {
	function import_po_file($file_path, $options) {
		try {
			$languages = isset($options['languages']) ? array_map('trim', explode(',', $options['languages'])) : array();
			$fileinfo = pathinfo($file_path);
			if(empty($languages) || in_array($fileinfo['filename'], $languages)) {
				$file = new stdClass;
				$file->uri = $file_path;
				$file->filename = basename($file_path);
				echo "\nTrying to import $file_path...";
				if(_locale_import_po($file, $fileinfo['filename'], LOCALE_IMPORT_OVERWRITE, 'default')) {
					echo "\nPO file import is success for $file_path...";
				} else {
					echo "\nSorry! PO file import is failure for $file_path. Please try manually.";
				}
			}
		} catch (Exception $ex) {
			var_dump($ex->getMessage());
		}
	}
	try {
		error_reporting(1);
		$document_root = $options['document-root'];
		$path = $options['path'];
		$_SERVER['DOCUMENT_ROOT'] = $document_root;
		$_SERVER['HTTP_HOST'] = 'default';
		$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
		$_SERVER['SERVER_SOFTWARE'] = NULL;
		$_SERVER['REQUEST_METHOD'] = 'GET';
		$_SERVER['QUERY_STRING'] = '';
		$_SERVER['HTTP_USER_AGENT'] = 'console';
		define('DRUPAL_ROOT', $document_root);
		include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
		include_once DRUPAL_ROOT . '/sites/all/services/Trace.php';
		if(drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL) != -1) {
			echo "\nDrupal Bootstrap is successful";
		}
		$file_type = null;
		//Download Zip file from url and unzip. Or if the given path is a location, unzip it directly.
		if(valid_url($path)) {
			echo "\nGiven path is a valid url and trying to download zip from it...";
			$file = system_retrieve_file($path, $document_root, FALSE, FILE_EXISTS_RENAME);
			if(!$file) {
				echo "\nFile download failed. Please check the given url once again...";
				exit();
			} else {
				echo "\nFile download is success. File is saved to $file";
				$file_type = 'zip';
			}
		} else {
			//path can be a directory which contains .po files or can be a file with zip extension
			$file_info = pathinfo($path);
			if(isset($file_info['extension']) && $file_info['extension'] == 'zip') {
				echo "\nTrying to open the given zip file $path";
				$file = $path;
				$file_type = 'zip';
			} else {
				echo "\nTrying to check if there are .po files in the given directory $path";
				$file_type = 'dir';
			}
		}
		if($file_type == 'zip') {
			$zip = new ZipArchive();
			$extractPath = "path_to_extract";
			if($zip->open($file) !== true) {
				echo "\nUnable to open the zip file from $file";
				exit();
			}
			for($i = 0; $i < $zip->numFiles; $i++) {
				$filename = $zip->getNameIndex($i);
				$fileinfo = pathinfo($filename);
				if(($langs = $zip->extractTo($document_root.'/po_files')) !== true) {
					echo "\nUnable to extract ".$fileinfo['basename']." to document root path";
					continue;	//Jump to next language file
				}
				$file = new stdClass;
				$file_path = $document_root . '/po_files/' . $fileinfo['dirname'] . '/' . $fileinfo['basename'];
				import_po_file($file_path, $options);
			}
			$zip->close();
		} else {
			//Import .po files from the given directory
			$files = glob("$path/*.po");
			if(empty($files)) {
				echo "\nThere are no .po files in $path";
				exit();
			}
			foreach ($files as $file_path) {
				import_po_file($file_path, $options);
			}
	
		}
		
	} catch(Exception $ex) {
		var_dump($ex->getMessage());
	}
}