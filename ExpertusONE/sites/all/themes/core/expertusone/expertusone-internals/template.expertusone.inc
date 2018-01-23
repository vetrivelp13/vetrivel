<?php
/**
 * @file
 * Add stylesheets that are only needed when expertusone is the enabled theme.
 *
 * Don't do something this dumb in your sub-theme. Stylesheets should be added
 * using your sub-theme's .info file. If you desire styles that apply
 * conditionally, you can conditionally add a "body class" in the
 * preprocess_page function. For an example, see how wireframe styling is
 * handled in expertusone_preprocess_html() and wireframes.css.
 */

/**
 * If the user is silly and enables expertusone as the theme, manually add some stylesheets.
 */
function _expertusone_preprocess_html(&$variables, $hook) {
  $directory = drupal_get_path('theme', 'expertusone') . '/expertusone-internals/css/';

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
  if (theme_get_setting('expertusone_layout') == 'expertusone-columns-liquid') {
    // Replace the layout-fixed.css file.
    $stylesheets[2] = 'layout-liquid.css';
  }
  foreach ($stylesheets as $stylesheet) {
    drupal_add_css($directory . $stylesheet, array('group' => CSS_THEME, 'every_page' => TRUE));
  }

  // Add print styles.
  drupal_add_css($directory . 'print.css', array('media' => 'print', 'group' => CSS_THEME, 'every_page' => TRUE));

  // Add conditional stylesheets.
  //drupal_add_css($directory . 'ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 7', '!IE' => FALSE), 'every_page' => TRUE));
  //drupal_add_css($directory . 'ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lte IE 6', '!IE' => FALSE), 'every_page' => TRUE));
  drupal_add_css($directory . 'ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'weight'=>1200));
  drupal_add_css($directory . 'ie9.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight'=>1200));
  $ie10Browser = (ereg('MSIE 10',$_SERVER['HTTP_USER_AGENT'])) ? true : false;
  if($ie10Browser == 1){
  	drupal_add_css($directory . 'ie10.css', array('type' => 'file', 'group' => CSS_THEME, 'weight' => 1200));
  }
  
  global $user;
  global $theme_key;
  $userLanguage = ($user->uid) ? $user->language : $_SESSION['language'];
  $langDir = drupal_get_path('theme', $theme_key) . '/expertusone-internals/lang-css/' . $userLanguage . '/' ;
  if(file_exists($langDir . 'ie8_'.$userLanguage.'.css')){
  	$ieEight = $langDir . 'ie8_'.$userLanguage.'.css';
  	drupal_add_css($ieEight, array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'weight'=>1200));
  }
  if(file_exists($langDir . 'ie9_'.$userLanguage.'.css')){
  	$ieNine  = $langDir . 'ie9_'.$userLanguage.'.css';
  	drupal_add_css($ieNine,  array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'weight'=>1200));
  }
  if(file_exists($langDir . 'ie10_'.$userLanguage.'.css') && $ie10Browser == 1){
  	$ieTen  = $langDir . 'ie10_'.$userLanguage.'.css';
  	drupal_add_css($ieTen,  array('type' => 'file', 'group' => CSS_THEME, 'weight'=>1200));
  }
}
