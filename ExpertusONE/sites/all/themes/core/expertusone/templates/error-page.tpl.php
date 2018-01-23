<?php
/**
 * @file
 * Zen theme's implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $secondary_menu_heading: The title of the menu used by the secondary links.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 * - $page['bottom']: Items to appear at the bottom of the page below the footer.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see zen_preprocess_page()
 * @see template_process()
 */
	// include_once $_SERVER["DOCUMENT_ROOT"].'/sites/all/themes/core/expertusoneV2/expertusone-internals/css/layout-fixed.css';
  global $theme_key;
  global $user;

?>
<?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
<div id="page-container">
<div id="page-wrapper" <?php print $page_class;?>><div id="page">

  <div id="header"><div class="section clearfix">
    <?php if ($logo): ?>
      <a style="margin-top:20px;" href="<?php print $front_page; ?>" title="<?php print ucfirst(strtolower(t('HOME'))); ?>" rel="home" id="logo"><img src="<?php print file_create_url($logo); ?>" alt="<?php print t('Home'); ?>" /></a>
    <?php endif; ?>

  </div></div><!-- /.section, /#header -->
  
   <div id="account_setting" style="display:none;">
  <div class="dropdownadd-dd-list-arrow" style="right:33px"></div>
  <a  class="qtip-close-button account-close-popup"></a>
  <table cellspacing="0" cellpadding="0" id="bubble-face-table">
  <tbody>
    <tr>
      <td class="bubble-tl"></td>
      <td class="bubble-t"></td>
      <td class="bubble-tr"></td>
    </tr>
    <tr>
      <td class="bubble-cl"></td>
      <td valign="top" class="bubble-c">
  <?php 
  // $loggedUserId = getSltpersonUserId();
  // $field = array('email','full_name');
  // $details  = getPersonDetails($loggedUserId,$field);
      
  ?>  
     <!--  <div class="full_name"><?php // print $details['full_name'];?></div>
      <div class="email"><?php //print $details['email'];?>
      </div> -->
      <div class="links header-popup"><a href="?q=<?php print $accountSetting['account']['1'];?>"> <?php print t("LBL825");?></a>
      </div>
      <div class="links header-popup"><a href="?q=<?php print $accountSetting['sign out']['1'];?>"> <?php print t(ucwords($accountSetting['sign out']['0']));?></a>
      </div>
      </td>
      <td class="bubble-cr"></td>
    </tr>
    <tr>
      <td class="bubble-bl"></td>
      <td class="bubble-blt"></td>
      <td class="bubble-br"></td>
    </tr>
  </tbody>
</table>
</div>

  <div id="main-wrapper"><div id="main" class="clearfix<?php if ($main_menu || $page['navigation']) { print ' with-navigation'; } ?>">

    <div id="content" class="column"><div class="section">
      <?php print render($page['highlight']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php print render($title_suffix); ?>      
      <?php if ($title): ?>
	        <div class="block">
	          <div class="block-title-left">
	          	<div class="block-title-right">
	          		<div class="block-title-middle">
	          			<h2 class="block-title">
						<?php
	          				print t($title);
	          			?>
						</h2>
						        			
	                 </div>
	            </div>
	          </div>
	          <div class="region-sidebar-widget-bg" style="padding-top:5px;">
      <?php endif;?>
      <span class="saml-msg">
      <?php
        print $content;
       ?>
       </span>
      <?php if ($title): ?>
          </div>
          <div class="block-footer-left">
      		<div class="block-footer-right">
        		<div class="block-footer-middle">&nbsp;</div>
      		</div>
		  </div>
		</div>
        <?php endif; ?>
    </div></div><!-- /.section, /#content -->

    



  </div></div><!-- /#main, /#main-wrapper -->

</div></div>
<?php print render($page['footer']); ?>
</div><!-- /#page, /#page-wrapper, /#page-container -->

<?php print render($page['bottom']); ?>
