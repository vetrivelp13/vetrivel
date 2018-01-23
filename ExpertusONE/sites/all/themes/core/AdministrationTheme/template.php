<?php
// $Id: template.php,v 1.3 2011-04-06 12:47:38 balaji.rajendran Exp $

function AdministrationTheme_preprocess_page(&$vars,$hook) {
  // Remove undesired local task tabs from order page.  
   /*AdministrationTheme_removetab('Add an attribute', $vars);
   AdministrationTheme_removetab('Add a predicate', $vars);
   AdministrationTheme_removetab('Convert configurations', $vars);
   AdministrationTheme_removetab('Add a tax rate', $vars);
   AdministrationTheme_removetab('Overview', $vars);
   AdministrationTheme_removetab('Edit menu', $vars);*/
   
 //if(strstr(request_uri(),'admin/store/orders/')){
    //AdministrationTheme_removetab('View', $vars);
   // AdministrationTheme_removetab('Edit', $vars);
  //}
  //AdministrationTheme_removetab(array('Log in','Request new password','Create new account'),$vars);  
    AdministrationTheme_removetab(array('Add discount'),$vars);
    AdministrationTheme_removetab(array('Edit Discount Rule'),$vars);
  
}
/**
 * Override to remove unwanted tabs.
 *
 * @param $label
 *   An array of labels to remove
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function AdministrationTheme_removetab($label, &$vars) {
  // Remove from primary tabs
  $i = 0;
  if (is_array($vars['tabs']['#primary'])) {
    foreach ($vars['tabs']['#primary'] as $primary_tab) {
      if (in_array($primary_tab['#link']['title'],$label)) {
        unset($vars['tabs']['#primary'][$i]);
      }
      ++$i;
    }
  }

  // Remove from secundary tabs
  $i = 0;
  if (is_array($vars['tabs']['#secondary'])) {
    foreach ($vars['tabs']['#secundary'] as $secundary_tab) {
      //if ($secundary_tab['#link']['title'] == $label) {
      if (in_array($secundary_tab['#link']['title'],$label)) {
        unset($vars['tabs']['#secundary'][$i]);
      }
      ++$i;
    }
  }
}
function AdministrationTheme_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  
  if($element['#original_link']['link_path'] == 'learning/myteam-search') {
    if(!is_manager(getSltpersonUserId())){
      return '';
    }
  }
  if($element['#original_link']['link_path'] == 'reports') {
       $userId = getSltpersonUserId();
      if(!user_access('New Report Perm') && !is_manager($userId) && !is_instructor($userId)){
        return '';
      }
    }
  
  if($element['#original_link']['menu_name'] == 'navigation') {
  	
  	 if($element['#original_link']['link_path'] == 'portalpages/sp_administrator/store-admin/ajax/cancel-policy') {
  	  $element['#original_link']['localized_options']['attributes']['id'] = 'cancel-policy';
      $element['#original_link']['localized_options']['attributes']['class'][0] = 'use-ajax';
      $element['#original_link']['localized_options']['attributes']['class'][1] = 'ctools-modal-ctools-admin-drop-policy-addedit-style';  
     }
  	
  	 if ($element['#below']) {
    	$sub_menu = drupal_render($element['#below']);
      }
      //$output = l($element['#title'], $element['#href'], $element['#localized_options']);
      $output = l(t($element['#title']), $element['#href'], $element['#original_link']['localized_options']);
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
  	
  }
  
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l(t($element['#title']), $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
/*
// Remove undesired local task tabs.
// Related to yourthemename_removetab() in yourthemename_preprocess_page().
function AdministrationTheme_removetab($label, &$vars) {
  $tabs = explode("\n", $vars['tabs']);
  $vars['tabs'] = '';

  foreach ($tabs as $tab) {
    if (strpos($tab, '>' . $label . '<') === FALSE) {
      $vars['tabs'] .= $tab . "\n";
    }
  }
  $vars['tabs'] = trim($vars['tabs']);
}*/