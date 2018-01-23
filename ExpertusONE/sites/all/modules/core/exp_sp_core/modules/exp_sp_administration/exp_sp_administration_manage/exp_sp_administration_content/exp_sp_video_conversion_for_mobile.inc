<?php 
function convertVideoForMobileAccessForMP4($srcFileName,$mobileVideoFileName,$ffmpeg_home){
	try {
		$output = array();
		$return_var = "";
		$conversion_needed = getConfigValue("convert_video_for_mobile_access");
		if(isset($conversion_needed) && $conversion_needed == "1")
		{
			$mobileVideoFileName = !empty($mobileVideoFileName) ? $mobileVideoFileName : $srcFileName;
			$fast = "-crf 22 -threads 0 ";//coder 0 flags -loop cmp +chroma partitions -parti8x8-parti4x4-partp8x8-partb8x8 me_method dia subq 0 me_range 16 g 250 keyint_min 25 sc_threshold 0 i_qfactor 0.71 b_strategy 0 qcomp 0.6 qmin 10 qmax 51 qdiff 4 bf 0 refs 1 directpred 1 trellis 0 flags2 -bpyramid-mixed_refs-wpred-dct8x8+fastpskip-mbtree wpredp 0 aq_mode 0 ";

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 ".$fast." -b:v 512k -f	mp4  ".$mobileVideoFileName;


			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 -b:v 512k -f	mp4  ".$mobileVideoFileName;

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline  -b:v 512k -f	mp4  ".$mobileVideoFileName;

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264  -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 -b:v 512k -f	mp4  ".$mobileVideoFileName;

			$cmd = $ffmpeg_home."/ffmpeg -i ".$srcFileName." -vcodec libx264 -vprofile baseline  -b:v 128k -f mp4 -y  ".$mobileVideoFileName;

			expDebug::dPrint("final ffmpeg outputhello:".$cmd, 4);


			$output = shell_exec($cmd);
			//exec(escapeshellcmd($cmd),$output,$return_var); //print_r($output);echo $cmd;exit();
			//$output = shell_exec($cmd);
		}
	} catch (Exception $ex) {
		watchdog_exception('convertVideoForMobileAccessForMP4', $ex);
		expertusErrorThrow($ex);
	}
}

function convertVideoForMobileAccessForWEBM_TO_MP4($srcFileName,$mobileVideoFileName,$ffmpeg_home){
	try {
		$output = array();
		$return_var = "";
		
			$mobileVideoFileName = !empty($mobileVideoFileName) ? $mobileVideoFileName : $srcFileName;
			$fast = "-crf 22 -threads 0 ";//coder 0 flags -loop cmp +chroma partitions -parti8x8-parti4x4-partp8x8-partb8x8 me_method dia subq 0 me_range 16 g 250 keyint_min 25 sc_threshold 0 i_qfactor 0.71 b_strategy 0 qcomp 0.6 qmin 10 qmax 51 qdiff 4 bf 0 refs 1 directpred 1 trellis 0 flags2 -bpyramid-mixed_refs-wpred-dct8x8+fastpskip-mbtree wpredp 0 aq_mode 0 ";

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 ".$fast." -b:v 512k -f	mp4  ".$mobileVideoFileName;


			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 -b:v 512k -f	mp4  ".$mobileVideoFileName;

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264 -acodec libmp3lame -vprofile baseline  -b:v 512k -f	mp4  ".$mobileVideoFileName;

			//$cmd = "ffmpeg -i ".$srcFileName." -y -vcodec libx264  -vprofile baseline -level 13 -maxrate 768000 -bufsize 3000000 -b:v 512k -f	mp4  ".$mobileVideoFileName;

			
				
			//$cmd = $ffmpeg_home."/ffmpeg -i ".$srcFileName." -vcodec libx264 -vprofile baseline  -b:v 128k -f mp4 -y  ".$mobileVideoFileName;
			$cmd = $ffmpeg_home."/ffmpeg -i ".$srcFileName." -strict -2 -f mp4 ".$mobileVideoFileName;
			expDebug::dPrint("final ffmpeg outputhello:".$cmd, 4);


			$output = shell_exec($cmd);
			//exec(escapeshellcmd($cmd),$output,$return_var); //print_r($output);echo $cmd;exit();
			//$output = shell_exec($cmd);
	
	} catch (Exception $ex) {
		watchdog_exception('convertVideoForMobileAccessForWEBM_TO_MP4', $ex);
		expertusErrorThrow($ex);
	}
}

function ffmpeg_installed($ffmpeg_home)
{
	try {
	// init version 
	$version = ''; // or NULL or '0.0.0' whatever //

	// ffmpeg command
	$command = $ffmpeg_home.'/ffmpeg -version';

	// grab the ffmpeg details
	$output = shell_exec($command);
	expDebug::dPrint("ffmpeg output :".$output , 4);
	// filter the output string with Regex
	/*if (preg_match('/^[a-zA-z\s]+(?<version>[0-9\.]+)+\,/im', $output, $matches)) {
		if (isset($matches['version'])) {
	$version = $matches['version'];
	}
	}
	expDebug::dPrint("ffmpeg version :".$version , 4); */
	$pos = strpos($output, "version");
	if ( $pos > 0)
		return true;
	else
		return false;
	//return ($text) ? $output : $version;
	} catch (Exception $ex) {
		watchdog_exception('ffmpeg_installed', $ex);
		expertusErrorThrow($ex);
	}
}



function get_video_filename_from_fs($fileDir)
{
	try {
	$videoBasename = "";
	if ($handle = opendir($fileDir)) {
		while (false !== ($file = readdir($handle)))        {
	    $lowerCaseFileName = strtolower($file);
	    if ($file != '.' && $file != '..' && substr($lowerCaseFileName, -strlen('.mp4')) === '.mp4' &&
	        		                                    (strpos($lowerCaseFileName, 'mobile_') === false || strpos($lowerCaseFileName, 'mobile_') !== 0))	{
				$videoBasename = $file;// substr($file, 0, strrpos($file, '.') + 0);
				break;
			}
		}
		closedir($handle);
	}
	return $videoBasename;
	} catch (Exception $ex) {
		watchdog_exception('get_video_filename_from_fs', $ex);
		expertusErrorThrow($ex);
	}
}


?>
