<?php expDebug::dPrint('videojs player learning tpl file : $html_id = ' . $html_id , 5);?>
<?php expDebug::dPrint('videojs player learning tpl file : $video_session_id = ' . $video_session_id , 5);?>
<?php
	expDebug::dPrint('videojs player learning tpl file : $video_url = ' . $video_url, 5);
	// parse url and check path
	$parsed_url = parse_url($video_url);
	if(!isset($parsed_url['scheme']) && strpos($video_url, '/') !== 0) {
		$video_url = '//' . $video_url;
		expDebug::dPrint('videojs player learning tpl file : $video_url corrected after parse = ' . $video_url, 5);
	}
	
?>
<?php expDebug::dPrint('videojs player learning tpl file : $video_source = ' . $video_source , 5);?>
<?php expDebug::dPrint('videojs player learning tpl file : $is_preview = ' . $is_preview , 5);?>

<?php
	$width = 640;
	$height = 380;
	if(! empty($is_preview)) {
		$width = $width / 2;
		$height = $height / 2;
	} else {
		if(isset($_SESSION['widgetCallback']) && $_SESSION['widgetCallback'] == TRUE) {
			if($_SESSION['widget']['display_width'] < 775) {
				$width = 550;
				$height = 310;
			} else if($_SESSION['widget']['display_width'] > 775 && $_SESSION['widget']['display_width'] < 900) {
				$width = 650;
				$height = 420;
			} else if($_SESSION['widget']['display_width'] > 900) {
				$width = 855;
				$height = 510;
			}
		} else {
			$width = 855;
			$height = 510;
		}
		if (contentPlayerIsActive()) { // content player basic resolution
			$width = (isset($_COOKIE['vdo_wdt'])) ? $_COOKIE['vdo_wdt']: 960;
			$height = (isset($_COOKIE['vdo_hgt'])) ? $_COOKIE['vdo_hgt']: 470;
		}
	}
?>

<?php
if($type == 'h5p')
{
		if(! empty($is_preview)) 
		{
			$height = 220; //280
			$width = 320;
			$nodeIdArr = explode("h5p/embed/",$video_url);
			$video_url = "/?q=h5p/embed/".$nodeIdArr[1];

			$video_url = $video_url."&preview=true";
		}
		else
		{
			$height = 540;
			$nodeIdArr = explode("h5p/embed/",$video_url);
			$video_url = "/?q=h5p/embed/".$nodeIdArr[1];
			$video_url = $video_url."&enrollId=".$enrollId;
			//$video_url = "/?q=node/".$nodeIdArr[1]."/view/&enrollId=".$enrollId;
		}
?>
<!--  removed margin-right:20px; from below check with Abhishek -->
<div  class="video-js-box" style="width:<?php print $width;?>px; height:<?php print $height;?>px; max-height:504px;overflow:hidden">
	<iframe allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"   src='<?php print $video_url;?>'   width="<?php print $width;?>px" height="<?php print $height;?>px"  border='0' margin='0' padding='0'></iframe>
</div>
<?php
}
else {
	

?>
	
<div id="video-container" class="video-js-box" width="640" height="380">
<video
    id = "<?php print $video_session_id;?>"
    class = "video-js vjs-default-skin vjs-big-play-centered"
    width = <?php print $width; ?>
    height = <?php print $height; ?>
    controls
    data-setup = '{
    				"techOrder": ["vimeo", "youtube", "html5", "flash"],
    				"sources": [{ "type": "<?php print ($video_source == 'rtmp' ? 'rtmp/mp4' : 'video/'.$video_source);?>",
    				"src": "<?php print $video_url;?>"}],
    				"autoplay": false
    			}'>
  </video>
</div>
<script>
	videojs.options.flash.swf = "<?php print $base_url . '/' . drupal_get_path('module', 'exp_sp_core') . '/js/videojs/video-js.swf';?>"
</script>


<?php
}
?>


