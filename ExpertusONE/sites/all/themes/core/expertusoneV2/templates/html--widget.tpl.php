<!DOCTYPE html /> 
<html>

<head>
  <?php // print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <!--[if IE 9]>
  	<script type="text/javascript" src="/sites/all/modules/core/exp_sp_core/js/videojs/video.js"></script>
  <![endif]-->
</head>
<body>
  <?php print $page; ?>
  <?php print $_SESSION['exp_sp_scripts']; ?>  
  <?php print $scripts; ?>
<body>
</html>
