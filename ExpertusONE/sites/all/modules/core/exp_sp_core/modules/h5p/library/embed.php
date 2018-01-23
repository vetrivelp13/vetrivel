<!doctype html>
<html lang="<?php print $lang; ?>" class="h5p-iframe">
<head>
  <meta charset="utf-8">
  <title><?php print $content['title']; ?></title>
  <style>
  .loadercontent
  {
  background:url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/spacer.gif") repeat;
  opacity:.8;
  position:absolute;
  z-index:98;
  filter:Alpha(Opacity=80);
  text-align:center
}
.loaderimg {
  background:url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/loader.gif") no-repeat 50% 50%;
  height:60px;
  text-align:center;
  width:60px;
  z-index:99;
  vertical-align:middle;
  margin:0 auto
}
  </style>
	 <link rel ="stylesheet" href="/sites/all/libraries/h5plibraries/FontAwesome-4.5/h5p-font-awesome.min.css"></link> 
  	
   <?php if( $content["library"]["name"] == "H5P.InteractiveVideo") 
   {
   ?>
  	<link rel ="stylesheet" href="/sites/all/libraries/h5plibraries/H5P.DragNBar-1.2/styles/context-menu.css"></link>
  	<link rel ="stylesheet" href="/sites/all/libraries/h5plibraries/H5P.InteractiveVideo-1.9/styles/interactive-video.css"></link>
	<?php } else { ?>
  	<link rel ="stylesheet" href="/sites/all/libraries/h5plibraries/H5P.CoursePresentation-1.9/styles/cp.css"></link>
	<?php } ?>
  </head>
  
     <?php if( $content["library"]["name"] == "H5P.InteractiveVideo") 
   {
   ?>

   <script>
    var jsdata = window.parent.$("#previewvideojsfiles").attr("value");
    var scr= document.createElement("script");        
	scr.innerHTML = jsdata;
	document.getElementsByTagName("head")[0].appendChild(scr);
	
	
	var cssdata = window.parent.$("#previewvideocssfiles").attr("value");
    var sty= document.createElement("style");        
	sty.innerHTML = cssdata;
	document.getElementsByTagName("head")[0].appendChild(sty);
	
   </script>
	<?php } else { ?>
   <script>
    var jsdata = window.parent.$("#previewpresentationjsfiles").attr("value");
    var scr= document.createElement("script");        
	scr.innerHTML = jsdata;
	document.getElementsByTagName("head")[0].appendChild(scr);
	
	
	var cssdata = window.parent.$("#previewpresentationcssfiles").attr("value");
    var sty= document.createElement("style");        
	sty.innerHTML = cssdata;
	document.getElementsByTagName("head")[0].appendChild(sty);
	
   </script>
   	<?php } ?>



  



	


  <script>var h5ptincantoken = "<?php print $content['h5ptoken']; ?>"</script>
  <script>var user_fullname = "<?php print $content["user_fullname"]; ?>"</script>
  
  <script>var tincan_registration_id = "<?php print $content["registration"]; ?>"</script> 
  <script>var className = "<?php print $content["contentTitle"]; ?>"</script> 
  
  <?php 		expDebug::dPrint('content detailssss12333333'. $content["registration"] , 4);
  expDebug::dPrint('content detailssss123333322223331'. print_r($content,true), 4);
	 ?>

<body>

<div id='loader' style="position:absolute;top:0px;left:0px;z-index:10007;"></div>
  <div class="h5p-content" data-content-id="<?php print $content['id']; ?>">
  </div>
  <script>
    H5PIntegration = <?php print json_encode($integration); ?>;
    <?php 	
    expDebug::dPrint('content integration11'. print_r($integration,true), 4);
  	 ?>
  </script>
  <script>
  function createLoaderNew(id)
  {
  	//alert("in loader function")
  		   var height = 480;
  		   var width = "100%";
  		   
  		   H5P.jQuery("#"+id).prepend("<div id='loader' class='loadercontent' style='z-index:10007;height:480px;width:100%;margin-top:-125px;'></div>");
  		   H5P.jQuery("#loader").html('<table border="0" style="width: 100%; height: 100%;"><tr><td width="53%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>');
   }

  </script>
</body>
</html>
