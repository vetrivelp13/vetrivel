<?php 
// $Id: page-admin-store.tpl.php,v 1.4 2011-04-07 09:37:35 muthusamys Exp $

/**
 * @file page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the theme is located in, e.g. themes/garland or
 *   themes/garland/minelli.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $body_classes: A set of CSS classes for the BODY tag. This contains flags
 *   indicating the current layout (multiple columns, single column), the current
 *   path, whether the user is logged in, and so on.
 * - $body_classes_array: An array of the body classes. This is easier to
 *   manipulate then the string in $body_classes.
 * - $node: Full node object. Contains data that may not be safe. This is only
 *   available if the current page is on the node's primary url.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing primary navigation links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing secondary navigation links for
 *   the site, if they have been configured.
 *
 * Page content (in order of occurrance in the default page.tpl.php):
 * - $left: The HTML for the left sidebar.
 *
 * - $breadcrumb: The breadcrumb trail for the current page.
 * - $title: The page title, for use in the actual HTML content.
 * - $help: Dynamic help text, mostly for admin pages.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the view
 *   and edit tabs when displaying a node).
 *
 * - $content: The main content of the current Drupal page.
 *
 * - $right: The HTML for the right sidebar.
 *
 * Footer/closing data:
 * - $feed_icons: A string of all feed icons for the current page.
 * - $footer_message: The footer message as defined in the admin settings.
 * - $footer : The footer region.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<div id="wrap">
<div id="page">

	<div id="header" class="header-main">
	     <div id='head_div' class="header-div-top">
	     	<div id="page_logo_wrapper">
	    		 <?php if ($logo): ?>
			        <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
			      <?php endif; ?>
		     </div>
		        <div class="header-top-strip_oldtheme"> 
		     <div class="secondary-top-menu-links"></div>
    <?php

                if ($page['shopping_cart']):
                print render($page['shopping_cart']);
                endif;
                //echo "<pre>"; 
      //print_r($secondary_menu);
        $accountSetting= array();
      foreach($secondary_menu as $key => $val) {
                  if(strtolower($val['title']) == 'learning request') 
                    $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style'), 'title' => t($val['title'])));
                  if(strtolower($val['title']) == 'sign in') 
                    $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style'), 'title' => t($val['title'])));
                  if(strtolower($val['title']) == 'reset password') 
                    $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']), 'attributes' => array('html' => TRUE, 'id'=>'resetPwdLink', 'class' => array('use-ajax', 'ctools-modal-ctools-default-style'), 'title' => t($val['title'])));
                    
                  if(strtolower($val['title']) != 'reset password' && strtolower($val['title']) != 'sign in' && strtolower($val['title']) != 'learning request') {
                  	$secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']), 'attributes' => array('title' => t($val['title'])));
                  }
                  if(strtolower($val['title']) == 'sign out' ){
                   $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']),'alter'=>TRUE,'title' => t($val['title']),  'attributes' => array('html' => TRUE, 'title' => t($val['title'])));
                   $accountSetting [strtolower($val['title'])]= array (strtolower($val['title']),strtolower($val['href']));
                   unset($secondary_menu[$key]);
                }
               if(strtolower($val['title']) == 'account') {
                  if($user->uid){
                  	require_once "./getLearnerInfo.php";
                  	$obj = new GetLearnerInfo();
                	$userfulname =  $obj->getValue('userfullname');
                	//if(isNotForumPage()){
                	$outhtml = t("LBL195").' '.$userfulname;
                	$secondary_menu[$key] = array('title' => '','alter'=>TRUE,'title' => $outhtml,  'attributes' => array('html' => TRUE, 'title' => $outhtml,'onclick'=>'showAccountSettings()','class' => array('my-account-arrow-new'),'id'=>('my-account-settings')));
                	$accountSetting [strtolower($val['title'])]= array (strtolower($val['title']),strtolower($val['href']));
                	//}
          }
         //unset($secondary_menu[$key]);
        }
                }
     $secondary_menu[] = array('href' => 'ctools_ajax_sample/ajax/reset_password', 'title' => t('Reset Password'), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style', 'breadcrumb'), 'id' => 'resetPwdLink', 'title' => t('Reset Password')));
      $secondary_menu[] = array('href' => 'learning/catalog-search/modal/bind', 'title' => t('Hidden Link'), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style', 'breadcrumb'), 'id' => 'dummy_link', 'title' => t('Hello world')));
      print theme('links__system_secondary_menu', array(
       'links' => $secondary_menu,
       'attributes' => array(
          'id' => 'secondary-menu',
          'class' => ($logged_in ? array('links', 'inline', 'clearfix') : array('links', 'inline', 'clearfix', 'form-register-txt')),
        ),
        'heading' => array(
          'text' => $secondary_menu_heading,
          'level' => 'h2',
          'class' => array('element-invisible'),
        ),
        
      ));
      
                
    if($page['multi_language']):
	  echo '<span id="separator_multi_language" class="language_vertical-bar-separater"></span>'; 
      print render($page['multi_language']);   
    endif;
                
    ?>          
    			 </div>
		 	    <div id="page_top_links">
		 	    	<?php if(empty($user->uid)) {?>
		     			<div id="page_top_links">
      						<div class="topinfo-display topname-display"></div>
      						<div class="top_head"></div>
      					</div>
      			     <?php }?>				      
	    			<?php if($page['header']): print render($page['header']); endif; ?>
	    		</div>
	    	 
	     </div>
	     <div class="splitter">&nbsp;</div>
	     <div id="horiz-menu" class="header-div-nav"><?php print render($page['top_menu']); ?></div>
  	</div>
<?php 
    $accountRightCSS = ($page['shopping_cart']) ? 'right: 46px' : 'right: 4px';  
    ?>
  <div id="account_setting" style="display:none;">
  <div class="bottom-qtip-tip-up" style="<?php print $accountRightCSS ?>"></div>
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
   $loggedUserId = getSltpersonUserId();
   $field = array('email','full_name');
   $details  = getPersonDetails($loggedUserId,$field);
      
  ?>  
     <!--  <div class="full_name"><?php // print $details['full_name'];?></div>
      <div class="email"><?php //print $details['email'];?>
      </div> -->
      <div class="links"><a href="?q=<?php print $accountSetting['account']['1'];?>"> <?php print t("LBL825");?></a>
      </div>
      <div class="links"><a href="?q=<?php print $accountSetting['sign out']['1'];?>"> <?php print t(ucwords($accountSetting['sign out']['0']));?></a>
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
  	<!-- Main -->
  	<div id="mainbody" class="content-main-holder">        
                <div class='clearLeft mainContainers'>
	                <?php if($page['left']): /* Left Side Content*/?>
	                <div id='left-side' class='front-leftSide clearLeft mainColumns'><?php print render($page['left']); ?></div>
	                <?php endif;?>
                </div>
                <?php if ($logged_in) {?>
                <div class='mainContainers'>
	                <div id='main-content' class='<?php print $page['right']?'front':'others';?>-mainContent mainColumns'>
		                <div class="uc-admin-store">
			                <div class="front-block-header block-header sp_content_header"><h2><?php print $title;?></h2>
				                <div id="contentGuide" class="contentGuide"><img key="Search Course" src="sites/all/themes/core/AdministrationTheme/images/exp_sp_icon_20x20_Help.gif" class="userGuide" alt="How To" title="How To">
				                </div>
			                </div>
			                <div class="uc-admin-store-region">
			                     <?php print $messages; ?>
				                <?php if ($tabs = render($tabs)): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
				                <?php if($logged_in): print render($page['help']); endif;?>
				                <?php if($logged_in): print '<div id="content-content">'.render($page['content']).'</div>'; endif;?>
			                </div>
		            	</div>
	                </div>
				</div>
				<?php }else{?>
				<div class="block">
	               <div class="block-title-left">
	          	      <div class="block-title-right">
	          		     <div class="block-title-middle">
	          			  <h2 class="block-title"> <?php print t($title);?></h2>
	                     </div>
	                   </div>
	                </div>
	                <div class="region-sidebar-widget-bg"> <?php print render($page['content'])?></div>
                 <div class="block-footer-left">
      		         <div class="block-footer-right">
        		      <div class="block-footer-middle">&nbsp;</div>
      		        </div>
		         </div>
		    </div>
				<?php }?>
				<?php if($page['right']): /*Right Side Content*/?>
                <div class='mainContainers'>                
                	<div id='right-side' class='mainColumns'><?php print render($page['right']);?></div>
                </div>
                <?php endif; ?>
                <?php // print $feed_icons;  ?>
	</div>
  	<!-- End Main -->
	<div class="footer-main">
		<?php if($page['footer']): print render($page['footer']); endif; ?>
	</div>
</div>
</div>