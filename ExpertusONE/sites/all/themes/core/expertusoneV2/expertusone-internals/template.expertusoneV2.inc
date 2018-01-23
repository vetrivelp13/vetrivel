<?php
/**
 * @file
 * Add stylesheets that are only needed when expertusone is the enabled theme.
 *
 * Don't do something this dumb in your sub-theme. Stylesheets should be added
 * using your sub-theme's .info file. If you desire styles that apply
 * conditionally, you can conditionally add a "body class" in the
 * preprocess_page function. For an example, see how wireframe styling is
 * handled in expertusoneV2_preprocess_html() and wireframes.css.
 */

/**
 * If the user is silly and enables expertusone as the theme, manually add some stylesheets.
 */
function _expertusoneV2_preprocess_html(&$variables, $hook) {
  $directory = drupal_get_path('theme', 'expertusoneV2') . '/expertusone-internals/css/';

  // Add expertusone's stylesheets manually instead of via its .info file. We do not
  // want the stylesheets to be inherited from expertusone since it becomes impossible
  // to re-order the stylesheets in the sub-theme.
  /*$stylesheets = array(
    'html-reset.css',
    'wireframes.css',
    'layout-fixed.css',
    'page-backgrounds.css',
    'tabs.css',
    'pages.css',
    'blocks.css',
    'navigation.css',
    'views-styles.css',
    'nodes.css',
    'comments.css',
    'forms.css',
    'fields.css',
  );*/
  $stylesheets = array(
    'layout-fixed.css'
  );
  if (theme_get_setting('expertusoneV2_layout') == 'expertusone-columns-liquid') {
    // Replace the layout-fixed.css file.
    $stylesheets[2] = 'layout-liquid.css';
  }
  foreach ($stylesheets as $stylesheet) {
    drupal_add_css($directory . $stylesheet, array('group' => CSS_THEME, 'every_page' => TRUE, 'browsers' => array('IE' => 'lte IE 10', '!IE' => true), 'preprocess' => true));  // #57721 - IE 9 support for large css file includes  
  }

  // Add print styles.
  drupal_add_css($directory . 'print.css', array('media' => 'print', 'group' => CSS_THEME, 'every_page' => TRUE));

  // Add conditional stylesheets.
  //drupal_add_css($directory . 'ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'every_page' => TRUE));
 // drupal_add_css($directory . 'ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'every_page' => TRUE));
  drupal_add_css($directory . 'ie8.css', array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'weight'=>1200));
  drupal_add_css($directory . 'ie9.css', array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight' => 1200));
  $ie10Browser = (preg_match('/MSIE 10/',$_SERVER['HTTP_USER_AGENT'])) ? true : false;
  $ie11Browser = (preg_match('/Trident\/7.0; rv:11.0/',$_SERVER['HTTP_USER_AGENT'])) ? true : false;
  $msedgeBrowser=(preg_match('/Edge/i',$_SERVER['HTTP_USER_AGENT'])) ? true : false;
  //expDebug::dPrint('IE10 variable check' . $ie10Browser, 1);
  if($ie10Browser == 1){
  	drupal_add_css($directory . 'ie10.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  }
  if($ie11Browser == 1){
  	drupal_add_css($directory . 'ie11.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  }
  if($msedgeBrowser == 1){
  	drupal_add_css($directory . 'ms_edge.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  }
  global $user;
  global $theme_key;
  $userLanguage = ($user->uid) ? $user->language : $_SESSION['language'];
  $langDir = drupal_get_path('theme', $theme_key) . '/expertusone-internals/lang-css/' . $userLanguage . '/' ;
  if(file_exists($langDir . 'ie8_'.$userLanguage.'.css')){
  	$ieEight = $langDir . 'ie8_'.$userLanguage.'.css';
  	drupal_add_css($ieEight, array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'weight'=>1301));
  }
  if(file_exists($langDir . 'ie9_'.$userLanguage.'.css')){
  	$ieNine  = $langDir . 'ie9_'.$userLanguage.'.css';
  	drupal_add_css($ieNine,  array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight'=>1301));
  }
  if(file_exists($langDir . 'ie10_'.$userLanguage.'.css') && $ie10Browser == 1){
  	$ieTen  = $langDir . 'ie10_'.$userLanguage.'.css';
  	drupal_add_css($ieTen,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1301));
  }
  if(file_exists($langDir . 'ie11_'.$userLanguage.'.css') && $ie11Browser == 1){
  	$ieEleven  = $langDir . 'ie11_'.$userLanguage.'.css';
  	drupal_add_css($ieEleven,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1301));
  }
  if(file_exists($langDir . 'ms_edge_'.$userLanguage.'.css') && $msedgeBrowser == 1){
  	$msedge  = $langDir . 'ms_edge_'.$userLanguage.'.css';
  	drupal_add_css($msedge,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1301));
  }
  
 
  /*--- include admin calendar realted files ----*/ 
  if (module_exists('exp_sp_admincalendar') && arg(0) == 'admincalendar') {
  	$adminCalCssDir  = drupal_get_path("module", "exp_sp_admincalendar") . '/css/';
  	$adminCalLangDir = drupal_get_path("module", "exp_sp_admincalendar") . '/lang-css/' . $userLanguage . '/';
  	
  	// IE Based Css
  	drupal_add_css($adminCalCssDir . 'ie9_ac.css', array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight' => 1200));
  	if($ie10Browser == 1){
  		drupal_add_css($adminCalCssDir . 'ie10_ac.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  	}
  	if($ie11Browser == 1){
  		drupal_add_css($adminCalCssDir . 'ie11_ac.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  	}
  	
  	// Language file inclusion
  	if(file_exists($adminCalLangDir . 'lang_ac_'.$userLanguage.'.css')){
  		$adminCalLangcss = $adminCalLangDir . 'lang_ac_'.$userLanguage.'.css';
  		drupal_add_css($adminCalLangcss, array('group' => CSS_THEME, 'every_page' => TRUE));
  	}
  	if(file_exists($adminCalLangDir . 'ie9_ac_'.$userLanguage.'.css')){
  		$ieNine  = $adminCalLangDir . 'ie9_ac_'.$userLanguage.'.css';
  		drupal_add_css($ieNine,  array('type' => 'file', 'group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight'=>1301));
  	}
  	if(file_exists($adminCalLangDir . 'ie10_ac_'.$userLanguage.'.css') && $ie10Browser == 1){
  		$ieTen  = $adminCalLangDir . 'ie10_ac_'.$userLanguage.'.css';
  		drupal_add_css($ieTen,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1301));
  	}
  	if(file_exists($adminCalLangDir . 'ie11_ac_'.$userLanguage.'.css') && $ie11Browser == 1){
  		$ieEleven  = $adminCalLangDir . 'ie11_ac_'.$userLanguage.'.css';
  		drupal_add_css($ieEleven,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1301));
  	}
  	
  }
}
