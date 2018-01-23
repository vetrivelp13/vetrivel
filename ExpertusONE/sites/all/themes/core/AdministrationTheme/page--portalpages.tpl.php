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
if(arg(0) == 'cart') {
  $current_step =  ((arg(0) == 'cart') and (arg(1) == 'checkout') and (arg(2) == 'complete')) ? 4 : '';
  $current_step =  (($current_step == '') and (arg(0) == 'cart') and (arg(1) == 'checkout') and (arg(2) == 'review')) ? 3 : $current_step;
  $current_step =  (($current_step == '') and (arg(0) == 'cart') and (arg(1) == 'checkout')) ? 2 : $current_step; 
  $current_step =  (($current_step == '') and (arg(0) == 'cart') and (arg(1) == '')) ? 1 : $current_step;
  $stepTwoTitle = getOrderTotal() ? t('LBL547') : t('LBL079');
  $stepFourTitle = getOrderTotal() ? t('LBL546') : t('LBL081');
  $stephtml = '<div class="shoppingcart-hdr">
  <div class="cart-step-region">
    <div><span class="'.(($current_step == 1) ?'shoppingcart-activestep':'shoppingcart-prevstep').'"><span>1</span></span><span class="cart-step">'.t("LBL078").'</span></div>
    <div><span class="'.(($current_step == 2) ?'shoppingcart-activestep':'shoppingcart-prevstep').'"><span>2</span></span><span class="cart-step">'.t($stepTwoTitle).'</span></div>
    <div><span class="'.(($current_step == 3) ?'shoppingcart-activestep':'shoppingcart-prevstep').'"><span>3</span></span><span class="cart-step">'.t("LBL080").'</span></div>
    <div><span class="'.(($current_step == 4) ?'shoppingcart-activestep':'shoppingcart-prevstep').'"><span>4</span></span><span class="cart-step">'.t($stepFourTitle).'</span></div>
  </div>
  </div>';
  if($page['content']['system_main']['#theme'] != 'uc_empty_cart'){
    $stephtml .= "
    <div id='cart_chechout_timer' class='chkoutcleardiv'>
      <table align='right' >
      	<tr>
          <td>".t('LBL560')."</td> 
          <td id='checkoutTimeRemin1'></td>
          <td class='last'>".t('LBL561')."</td>
      	</tr>
     </table>
    </div>";
  }
  $page_class = 'class="cart-page-wrapper"';
}
else if((arg(0) == 'user') && (arg(1) == 'password')) {
  $stephtml = '';
  $page_class = 'class="user-password-page-wrapper"';
}
else {
  $stephtml = '';
  $page_class = '';
}
?>

<div id="wrap" <?php print $page_class;?>><div id="page">

  <div id="header" class="header-main">
	     <div id='head_div' class="header-div-top">
	     	<div id="page_logo_wrapper">
	    		 <?php if ($logo): ?>
			        <a href="<?php print $front_page; ?>" title="<?php print ucfirst(strtolower(t('HOME'))); ?>" rel="home" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
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
      //$secondary_menu['']
     // print_r($accountSetting);
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
	<!-- Main -->
	
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
  	<div id="mainbody" class="content-main-holder">
        <?php print $messages; ?>
                <div class='clearLeft mainContainers'>
	                <?php if($page['left']): /* Left Side Content*/?>
	                <div id='left-side' class='front-leftSide clearLeft mainColumns'><?php print render($page['left']); ?></div>
	                <?php endif;?>
                </div>
                <?php if ($logged_in) {?>
                <div class='mainContainers'>
	                <div id='main-content' class='<?php print $page['right']?'front':'others';?>-mainContent mainColumns'>
	                <?php if ($tabs = render($tabs)): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
	                <?php if($logged_in): print render($page['help']); endif;?>
	                <?php if($logged_in): print '<div id="content-content">'.render($page['content']).'</div>'; endif;?>
	                </div>
				</div>
				<?php } else {?>
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
			<?php 	}?>
				<?php if($page['right']): /*Right Side Content*/?>
                <div class='mainContainers'>                
                	<div id='right-side' class='mainColumns'><?php print render($page['right']);?></div>
                </div>
                <?php endif; ?>
                <?php // print $feed_icons; ?>
	</div>
  	<!-- End Main -->
  	<div class="footer-main">
		<?php if($page['footer']): print render($page['footer']); endif; ?>
	</div>
</div>
</div>