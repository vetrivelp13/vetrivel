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
  global $theme_key;
  global $user;
  if(arg(0) == 'cart' && ($user->uid > 0)) {
  $current_step =  ((arg(0) == 'cart') && (arg(1) == 'checkout') && (arg(2) == 'complete')) ? 5 : '';
  $current_step =  (($current_step == '') && (arg(0) == 'cart') && (arg(1) == 'checkout') && (arg(2) == 'review')) ? 4 : $current_step;
  $current_step =  (($current_step == '') && (arg(0) == 'cart') && (arg(1) == 'checkout') && (arg(2) == 'paymethod')) ? 3 : $current_step;
  $current_step =  (($current_step == '') && (arg(0) == 'cart') && (arg(1) == 'checkout')) ? 2 : $current_step;
  $current_step =  (($current_step == '') && (arg(0) == 'cart') && (arg(1) == '')) ? 1 : $current_step;

  $stephtml =
  	'<div id="show_expertus_message">'.$messages.'</div>'.
    '<div class="shoppingcart-hdr-v2">' .
      '<div class="cart-step-region">' .
        '<div>' .
          '<span class="' . (($current_step == 1)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
            '<span>1</span>' .
          '</span>' .
          '<span class="cart-step">' . t("LBL078") . '</span>' . //View Cart
        '</div>' .
        '<div>' .
          '<span class="' . (($current_step == 2)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
            '<span>2</span>' .
          '</span>' .
            '<span class="cart-step">' .
              ((!empty($_SESSION['availableFunctionalities']->uc_discounts)) ? t('LBL957') : t('LBL555')) . //Billing & Discount Details, Billing Details
            '</span>' .
        '</div>' .
        '<div>' .
          '<span class="' . (($current_step == 3)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
            '<span>3</span>' .
          '</span>' .
          '<span class="cart-step">' . t('LBL781') . '</span>' . //Payment Details
        '</div>' .
        '<div>' .
          '<span class="' . (($current_step == 4)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
            '<span>4</span>' .
          '</span>' .
          '<span class="cart-step">' . t('LBL960') . '</span>' . //Review & Submit Order
        '</div>' .
        '<div>' .
          '<span class="' . (($current_step == 5)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
            '<span>5</span>' .
          '</span>' .
          '<span class="cart-step">' . t('LBL081') . '</span>' . //Order Confirmation
        '</div><div class="clearBoth"></div>' .
      '</div>' .
    '</div>';
    if($page['content']['system_main']['#theme'] != 'uc_empty_cart'){
      $checkoutTimerHtml .= "
      <div id='cart_chechout_timer' class='chkoutcleardiv'>
        <table align='right' >
        	<tr>
            <td>".t('LBL560')."</td> 
            <td id='checkoutTimeRemin1' style='color:#E42094;'></td>
            <td class='last'>".t('LBL561')."</td>
        	</tr>
       </table>
      </div>"; //Time left to complete your purchase, Mins
    }   
  $page_class = 'class="cart-page-wrapper"';
}
elseif(arg(0) == 'administration' && arg(1) =='order' && user_access('view all orders')){
	$current_step_mes =  ((arg(0) == 'administration') && (arg(1) == 'order') && (arg(2) == 'finish')) ? t('LBL081') : '';
	$current_step_mes =  (($current_step == '') && (arg(0) == 'administration') && (arg(1) == 'order') && (arg(2) == 'pay')) ? t('LBL781') : $current_step_mes;
	$current_step_mes =  (($current_step == '') && (arg(0) == 'administration') && (arg(1) == 'order') && (arg(2) == 'create')) ? t('User') .' ' .ucfirst(strtolower(t('LBL647'))).' '. t('LBL555')  : $current_step_mes;
	
	$stephtml =
	'<div id="show_expertus_message">'.$messages.'</div>'.
	'<div class="shoppingcart-hdr-v2">' .
	'<div class="cart-step-region admin-order-step">' .
	'<div>' .
	//'<span class="' . (($current_step == 1)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
	'<span class="shoppingcart-prevstep">' .
	//'<span>1</span>' .
	'</span>' .
	'<span class="cart-step">' . $current_step_mes.
	//((!empty($_SESSION['availableFunctionalities']->uc_discounts)) ? t('LBL1190') : t('Add User & Training Details')) . //User, Training & Discount Details, User & Training Details
	'</span>' .
	'</div>' .
	'<div>' .
	'<span class="' . (($current_step == 2)? 'shoppingcart-activestep' : 'shoppingcart-prevstep') . '">' .
	//'<span>2</span>' .
	'</span>' .
	//'<span class="cart-step">' . t('LBL1191') . '</span>' . //Enter Payment Details & Submit
	'</div>' .
	'<div class="clearBoth"></div>' .
	'</div>' .
	'</div>';
	if (!isset($_SESSION['do_admin_complete']) || !$_SESSION['do_admin_complete']){
	$checkoutTimerHtml .= "
      <div id='cart_chechout_timer' class='chkoutcleardiv'>
        <table align='right' >
        	<tr>
            <td>".t('LBL560')."</td>
            <td id='checkoutTimeRemin1' style='color:#E42094;'></td>
            <td class='last'>".t('LBL561')."</td>
        	</tr>
       </table>
      </div>"; //Time left to complete your purchase, Mins
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

<div id="page-container">
<div id="page-wrapper" <?php print $page_class;?>><div id="page">

  <div id="header"><div class="section clearfix">
    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print ucfirst(strtolower(t('HOME'))); ?>" rel="home" id="logo"><img src="<?php  /*print file_create_url($logo);*/print get_logo_path() ;?>" alt="<?php print t('Home'); ?>" /></a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"><strong>
              <a href="<?php print $front_page; ?>" title="<?php print ucfirst(strtolower(t('HOME'))); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </strong></div>
          <?php  else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print ucfirst(strtolower(t('HOME'))); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div><!-- /#name-and-slogan -->
    <?php endif; ?>
	<div class="secondary-top-menu-links">


    		<div id="system-admin-menu-container">
    		
    		<?php
              $menu = menu_navigation_links("menu-system-admin");
              expDebug::dPrint('menu_navigation_links : = ' . print_r($menu, true),4);
              print theme('links__menu-system-admin', array(
        		'links' => $menu,
        		'attributes' => array(
          		'id' => 'system-admin-menu',
          		'class' => ($logged_in ? array('links', 'inline', 'clearfix') : array('links', 'inline', 'clearfix', 'form-register-txt')),
              ),
        		'heading' => array(
          		'text' => $menu,
          		'level' => 'h2',
          		'class' => array('element-invisible'),
              ),
              ));
    ?>
	       
    		</div>


	<div class="header-top-strip">
		<div class="theme-secondary-menu-bg-left">
		<div class="theme-secondary-menu-bg-right">
		<div class="theme-secondary-menu-bg-middle">
		<div class="secondary-menu-overlay"></div>
      <?php
      if ($page['shopping_cart']):
        print render($page['shopping_cart']);
      endif;
      //echo "<pre>"; 
      //print_r($secondary_menu);
        $accountSetting= array();
      foreach($secondary_menu as $key => $val) {
        if(strtolower($val['title']) == 'learning request'){ 
          $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-learning-request-style'), 'title' => t($val['title'])));
        }else if(strtolower(t($val['title'])) == strtolower(t('Sign In')) || $val['href'] == 'ctools_ajax_sample/ajax/user_login'){ 
          $secondary_menu[$key] = array('href' => $val['href'], 'title' => t('Sign In'), 'attributes' => array('html' => TRUE, 'id'=>'signin', 'class' => array('use-ajax', 'ctools-modal-ctools-login-style'), 'onclick'=>'getClickSignIn()'));
        }else if(strtolower($val['title']) == 'site admin'){
         $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']),'alter'=>TRUE,'title' => t($val['title']),  'attributes' => array('html' => TRUE, 'title' => t($val['title'])));  
        }else if(strtolower($val['title']) == 'sign out' ){
         $secondary_menu[$key] = array('href' => $val['href'], 'title' => t($val['title']),'alter'=>TRUE,'title' => t($val['title']),  'attributes' => array('html' => TRUE, 'title' => t($val['title'])));
         $accountSetting [strtolower($val['title'])]= array (strtolower($val['title']),strtolower($val['href']));
         unset($secondary_menu[$key]);
        }else if(strtolower($val['title']) == 'account') {
          if($user->uid){
          	require_once "./getLearnerInfo.php";
          	$obj = new GetLearnerInfo();
        	$userfulname =  $obj->getValue('userfullname');
        	//if(isNotForumPage()){
        	$outhtml = t("LBL195").' '.$userfulname;
        	$secondary_menu[$key] = array('title' => '','alter'=>TRUE,'title' => $outhtml,  'attributes' => array('html' => TRUE, 'title' => $outhtml,'onclick'=>'showAccountSettings()','class' => array('my-account-arrow-new','vtip'),'id'=>('my-account-settings')));
        	$accountSetting [strtolower($val['title'])]= array (strtolower($val['title']),strtolower($val['href']));
        	//}
          }
         //unset($secondary_menu[$key]);
        }
        else if($val['title'] == 'Register'){          
          $secondary_menu[$key]['title'] = t('Register');
          if($user->uid != 0){ // Unset the Key To If it is Super admin Uid = 1
          	unset($secondary_menu[$key]);
          }
        }
      }
      //$secondary_menu['']
     // print_r($accountSetting);
      $secondary_menu[] = array('href' => 'ctools_ajax_sample/ajax/reset_password', 'title' => t('Reset Password'), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style', 'breadcrumb'), 'id' => 'resetPwdLink', 'title' => t('Reset Password')));
//      $secondary_menu[] = array('href' => 'learning/catalog-search/modal/bind', 'title' => t('Hidden Link'), 'attributes' => array('html' => TRUE, 'class' => array('use-ajax', 'ctools-modal-ctools-default-style', 'breadcrumb'), 'id' => 'dummy_link', 'title' => t('Hello world')));
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
     
    ?>    
    <?php 
    if($page['multi_language']):
	  echo '<span id="separator_multi_language" class="language_vertical-bar-separater"></span>'; 
      print render($page['multi_language']);   
    endif;
    ?>
    
    </div>
    <?php 
    if ($page['top_links']):
      print render($page['top_links']);
    endif;
    ?></div></div></div><div class="clearBoth"></div>
    </div>
    <div class="theme-navigation-menu">
    <?php 
    	$curveCls = '';
			foreach($page['header']['system_main-menu'] as $key => $menuvalue){
              if(gettype($menuvalue) == 'array'){   
              	// Changes Made For This Ticket #0040416 Previous Href is "learning/catalog-search"   
                if($menuvalue['#href'] == '<front>' && drupal_is_front_page()){
									$curveCls = 'active-header-curve';
									break;
                }
              }   
       }
    ?>
	    <div class="theme-navigation-menu-bg-left <?php echo $curveCls;?>"></div>
		<div class="theme-navigation-menu-bg-middle">
			<?php 
			 // Commented For this Ticket #0040416
			 /* foreach($page['header']['system_main-menu'] as $key => $menuvalue){
              if(strlen($menuvalue['#href'] == 0)){
                unset($page['header']['system_main-menu'][$key]); 
                break;                  
              }            
            }  */
            print render($page['header']); ?>
		</div>
	    <div class="theme-navigation-menu-bg-right"></div>
	</div>
	<?php if ($page['header_right_side']): ?>
		<div id="header-right-side" class="header-right-side">
			<?php print render($page['header_right_side']); ?>
		</div>
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
<?php 

if(arg(0) == 'admincalendar' && drupal_get_http_header("status") == '403 Forbidden'){	
	$access_denied_class = 'access-denied-page';
}?>
  <div id="main-wrapper"><div id="main" class="clearfix<?php if ($main_menu || $page['navigation']) { print ' with-navigation'; } ?>">
    <div id="content" class="column <?php global $mylearning_right; if($mylearning_right===false) print ' clsContentFull'; ?>"><div class="section">
      <?php print render($page['highlight']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php print render($title_suffix); ?>      
      <?php if ($title): ?>
	        <div class="block">
	          <div class="block-title-left <?php print $access_denied_class;?>">
	          	<div class="block-title-right">
	          		<div class="block-title-middle">
	          			<h2 class="block-title vtip" title = "<?php print $title; ?>">
	          			<?php
                         $title = titleController('EXP-SP-FOOTER-TITLE',$title,30);
                         print t($title);
                        ?>
						</h2>
						        			
	                 </div>
	            </div>
	          </div>
	          <div class="region-sidebar-widget-bg">
      <?php endif;?>
      <?php if ($tabs = render($tabs)): ?>
        <div class="tabs"><?php print $tabs; ?></div>
      <?php endif; ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content_top']); ?>
      <?php
      if($stephtml) {
        print '<div class="commerce-container-v2">'.$stephtml;
        if($checkoutTimerHtml){
		  print $checkoutTimerHtml;
        }
        $divTag ='';			  
        
        if(arg(2) != 'paymethod')$divTag = '</div>'; //Remove one unwanted div hide in the payment method  
        print render($page['content']).$divTag;
      }  
      else {
        print $messages;
        print render($page['content']);
      } ?>
      <?php if ($title): ?>
          </div>
          <div class="block-footer-left <?php print $access_denied_class;?>">
      		<div class="block-footer-right">
        		<div class="block-footer-middle">&nbsp;</div>
      		</div>
		  </div>
		</div>
        <?php endif; ?>
      <?php print render($page['content_bottom']); ?>
      <?php print $feed_icons; ?>
    </div></div><!-- /.section, /#content -->

    <?php if ($page['navigation'] || $main_menu): ?>
      <div id="navigation"><div class="section clearfix">

        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu',
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>

        <?php print render($page['navigation']); ?>

      </div></div><!-- /.section, /#navigation -->
    <?php endif; ?>

    <?php print render($page['sidebar_first']); ?>

    <?php print render($page['sidebar_second']); ?>

  </div></div><!-- /#main, /#main-wrapper -->

</div></div>
<?php
if(footerIsActive()) {
?>
		<div class="footer-container">
			<div class="footer-phone-info"><?php print t('LBL070') ?>: <?php print getConfigValue('site_phone'); ?></div>
                        <div class="footer-email-info"><?php print t('LBL061') ?>: <a href='mailto:<?php print variable_get('site_mail');?>' class="footer-email-link" > <?php print variable_get('site_mail');?></a></div>
  			<?php $versionTxt = file_get_contents('./VERSION.txt', true);?>
  			<div class="footer-version-info"><?php print t('LBL1123') ?>: <?php print substr(substr($versionTxt,0,strpos($versionTxt, " ")),1); ?></div>
  			<?php print render($page['footer']); ?>
  		</div>
<?php }else{?><div class="footer-without-content"></div><?php }?>
</div><!-- /#page, /#page-wrapper, /#page-container -->

<?php print render($page['bottom']); ?>
