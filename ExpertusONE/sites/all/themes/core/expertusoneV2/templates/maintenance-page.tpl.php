<?php
/**
 * @file
 * Zen theme's implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript">
  $(function(){
	  function loadHeight(){
	  var viewportHeight = $(window).height();
	  $('#page-container').css('height',viewportHeight);
	  }
	  loadHeight();
	  //call function to window resize
	  $(window).resize(function() {
	    loadHeight();
	  });
  });
  
  </script>
</head>
<body class="<?php print $classes; ?>">
 <div id="page-container">
  <div id="page-wrapper"><div id="page">

    <div id="header"><div class="section clearfix">

      <?php if ($logo): ?>
        <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
      <?php endif; ?>

      <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan">
          <?php if ($site_name): ?>
            <div id="site-name"><strong>
              <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
              <span><?php print $site_name; ?></span>
              </a>
            </strong></div>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div><!-- /#name-and-slogan -->
      <?php endif; ?>

      <?php print $header; ?>

    </div></div><!-- /.section, /#header -->

    <div id="main-wrapper"><div id="main" class="clearfix<?php if ($navigation) { print ' with-navigation'; } ?>">

      <div id="content" class="column"><div class="section">

        <?php print $highlighted; ?>  
    <div class="block sitemaintenance-bottom-spacer">
      <div class="block-title-left">
	          	<div class="block-title-right">
	          		<div class="block-title-middle">
	        		
        <?php if ($title): ?>
          <h2 class="block-title"><?php print t('Site maintenance'); ?></h2>
        <?php endif; ?>
       
                   </div>
	            </div>
             </div>    
	<div class="region-sidebar-widget-bg">             
        <div class="sitemaintanance-msg-result">
       <?php print $messages; ?>
       <?php print t('MSG693') ; ?>
       </div>
    </div>
      <div class="block-footer-left">
      		<div class="block-footer-right">
        		<div class="block-footer-middle">&nbsp;</div>
      		</div>
		  </div>
      </div>
      </div></div><!-- /.section, /#content -->

      <?php if ($navigation): ?>
        <div id="navigation"><div class="section clearfix">

          <?php print $navigation; ?>

        </div></div><!-- /.section, /#navigation -->
      <?php endif; ?>

      <?php print $sidebar_first; ?>

      <?php print $sidebar_second; ?>

    </div></div><!-- /#main, /#main-wrapper -->

    <?php print $footer; ?>

  </div></div></div><!-- /#page, /#page-wrapper -->

  <?php print $bottom; ?>

</body>
</html>
