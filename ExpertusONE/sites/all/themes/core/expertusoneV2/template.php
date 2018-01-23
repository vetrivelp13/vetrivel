<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the expertusone theme.
 *
 * IMPORTANT WARNING: DO NOT MODIFY THIS FILE.
 *
 * The base expertusone theme is designed to be easily extended by its sub-themes. You
 * shouldn't modify this or any of the CSS or PHP files in the root expertusone/ folder.
 * See the online documentation for more information:
 *   http://drupal.org/node/193318
 */

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('expertusoneV2_rebuild_registry') && !defined('MAINTENANCE_MODE')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}


/**
 * Implements HOOK_theme().
 */
function expertusoneV2_theme(&$existing, $type, $theme, $path) {
  include_once './' . drupal_get_path('theme', 'expertusoneV2') . '/expertusone-internals/template.theme-registry.inc';
  return _expertusoneV2_theme($existing, $type, $theme, $path);
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $variables
 *   - title: An optional string to be used as a navigational heading to give
 *     context for breadcrumb links to screen-reader users.
 *   - title_attributes_array: Array of HTML attributes for the title. It is
 *     flattened into a string within the theme function.
 *   - breadcrumb: An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function expertusoneV2_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('expertusoneV2_breadcrumb');
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('expertusoneV2_breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $breadcrumb_separator = theme_get_setting('expertusoneV2_breadcrumb_separator');
      $trailing_separator = $title = '';
      if (theme_get_setting('expertusoneV2_breadcrumb_title')) {
        $item = menu_get_item();
        if (!empty($item['tab_parent'])) {
          // If we are on a non-default tab, use the tab's title.
          $title = check_plain($item['title']);
        }
        else {
          $title = drupal_get_title();
        }
        if ($title) {
          $trailing_separator = $breadcrumb_separator;
        }
      }
      elseif (theme_get_setting('expertusoneV2_breadcrumb_trailing')) {
        $trailing_separator = $breadcrumb_separator;
      }

      // Provide a navigational heading to give context for breadcrumb links to
      // screen-reader users.
      if (empty($variables['title'])) {
        $variables['title'] = t('You are here');
      }
      // Unless overridden by a preprocess function, make the heading invisible.
      if (!isset($variables['title_attributes_array']['class'])) {
        $variables['title_attributes_array']['class'][] = 'element-invisible';
      }
      $heading = '<h2' . drupal_attributes($variables['title_attributes_array']) . '>' . $variables['title'] . '</h2>';

      return '<div class="breadcrumb">' . $heading . implode($breadcrumb_separator, $breadcrumb) . $trailing_separator . $title . '</div>';
    }
  }
  // Otherwise, return an empty string.
  return '';
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function expertusoneV2_menu_local_tasks(&$variables) {
  $output = '';

  if ($primary = drupal_render($variables['primary'])) {
    $output .= '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $output .= '<ul class="tabs primary clearfix">' . $primary . '</ul>';
  }
  if ($secondary = drupal_render($variables['secondary'])) {
    $output .= '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $output .= '<ul class="tabs secondary clearfix">' . $secondary . '</ul>';
  }

  return $output;
}

/**
 * Override or insert variables into theme_menu_local_task().
 */
function expertusoneV2_preprocess_menu_local_task(&$variables) {
  $link =& $variables['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }
  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}

/**
 * Adds conditional CSS from the .info file.
 *
 * Copy of conditional_styles_preprocess_html().
 */
function expertusoneV2_add_conditional_styles() {
  // Make a list of base themes and the current theme.
  $themes = $GLOBALS['base_theme_info'];
  $themes[] = $GLOBALS['theme_info'];
  expDebug::dPrint("dsfdfd ".print_r($themes,true),5);
  foreach (array_keys($themes) as $key) {
    $theme_path = dirname($themes[$key]->filename) . '/';
    if (isset($themes[$key]->info['stylesheets-conditional'])) {
      foreach (array_keys($themes[$key]->info['stylesheets-conditional']) as $condition) {
        foreach (array_keys($themes[$key]->info['stylesheets-conditional'][$condition]) as $media) {
          foreach ($themes[$key]->info['stylesheets-conditional'][$condition][$media] as $stylesheet) {
            // Add each conditional stylesheet.
            drupal_add_css(
              $theme_path . $stylesheet,
              array(
                'group' => CSS_THEME,
                'browsers' => array(
                  'IE' => $condition,
                  '!IE' => FALSE,
                ),
                'every_page' => TRUE,
              )
            );
          }
        }
      }
    }
  }
}

/**
 * Override or insert variables into the html template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function expertusoneV2_preprocess_html(&$variables, $hook) {
  // If the user is silly and enables expertusone as the theme, add some styles.
  if ($GLOBALS['theme'] == 'expertusoneV2') {
    include_once './' . drupal_get_path('theme', 'expertusoneV2') . '/expertusone-internals/template.expertusoneV2.inc';
    _expertusoneV2_preprocess_html($variables, $hook);
  }
  elseif (!module_exists('conditional_styles')) {
    expertusoneV2_add_conditional_styles();
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$variables['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        $section = 'node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        $section = 'node-' . arg(2);
      }
    }
    $variables['classes_array'][] = drupal_html_class('section-' . $section);
  }
  if (theme_get_setting('expertusoneV2_wireframes')) {
    $variables['classes_array'][] = 'with-wireframes'; // Optionally add the wireframes style.
  }
  // Store the menu item since it has some useful information.
  $variables['menu_item'] = menu_get_item();
  switch ($variables['menu_item']['page_callback']) {
    case 'views_page':
      // Is this a Views page?
      $variables['classes_array'][] = 'page-views';
      break;
    case 'page_manager_page_execute':
    case 'page_manager_node_view':
    case 'page_manager_contact_site':
      // Is this a Panels page?
      $variables['classes_array'][] = 'page-panels';
      break;
  }
  $variables['jump_link_target'] = theme_get_setting('expertusoneV2_jump_link_target');
}

/**
 * Override or insert variables into the page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
function expertusoneV2_preprocess_page(&$variables, $hook) {
  // Find the title of the menu used by the secondary links.
  $secondary_links = variable_get('menu_secondary_links_source', 'user-menu');
  if ($secondary_links) {
    $menus = function_exists('menu_get_menus') ? menu_get_menus() : menu_list_system_menus();
    $variables['secondary_menu_heading'] = $menus[$secondary_links];
  }
  else {
    $variables['secondary_menu_heading'] = '';
  }
  // To remove unwanted tabs
  expertusoneV2_removetab(array('Log in','Request new password','Create new account'),$variables);
}

/**
 * Override to remove unwanted tabs.
 *
 * @param $label
 *   An array of labels to remove
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function expertusoneV2_removetab($label, &$vars) {

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

/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
function expertusoneV2_preprocess_maintenance_page(&$variables, $hook) {
  // If expertusone is the maintenance theme, add some styles.
  if ($GLOBALS['theme'] == 'expertusoneV2') {
    include_once './' . drupal_get_path('theme', 'expertusoneV2') . '/expertusone-internals/template.expertusoneV2.inc';
    _expertusoneV2_preprocess_html($variables, $hook);
  }
  elseif (!module_exists('conditional_styles')) {
    expertusoneV2_add_conditional_styles();
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function expertusoneV2_preprocess_node(&$variables, $hook) {
  // Add $unpublished variable.
  $variables['unpublished'] = (!$variables['status']) ? TRUE : FALSE;

  // Add a class for the view mode.
  if (!$variables['teaser']) {
    $variables['classes_array'][] = 'view-mode-' . $variables['view_mode'];
  }

  // Add a class to show node is authored by current user.
  if ($variables['uid'] && $variables['uid'] == $GLOBALS['user']->uid) {
    $variables['classes_array'][] = 'node-by-viewer';
  }

  $variables['title_attributes_array']['class'][] = 'node-title';
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
function expertusoneV2_preprocess_comment(&$variables, $hook) {
  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $variables['node']->type, 1) == 0) {
    $variables['title'] = '';
  }

  // Anonymous class is broken in core. See #1110650
  if ($variables['comment']->uid == 0) {
    $variables['classes_array'][] = 'comment-by-anonymous';
  }
  // Zebra striping.
  if ($variables['id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  if ($variables['id'] == $variables['node']->comment_count) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['zebra'];

  $variables['title_attributes_array']['class'][] = 'comment-title';
}

/**
 * Preprocess variables for region.tpl.php
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
function expertusoneV2_preprocess_region(&$variables, $hook) {
  // Sidebar regions get some extra classes and a common template suggestion.
  if (strpos($variables['region'], 'sidebar_') === 0) {
    $variables['classes_array'][] = 'column';
    $variables['classes_array'][] = 'sidebar';
    $variables['theme_hook_suggestions'][] = 'region__sidebar';
    // Allow a region-specific template to override expertusone's region--sidebar.
    $variables['theme_hook_suggestions'][] = 'region__' . $variables['region'];
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function expertusoneV2_preprocess_block(&$variables, $hook) {
  // Classes describing the position of the block within the region.
  if ($variables['block_id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  // The last_in_region property is set in expertusoneV2_page_alter().
  if (isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['block_zebra'];

  $variables['title_attributes_array']['class'][] = 'block-title';
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function expertusoneV2_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * Implements hook_page_alter().
 *
 * Look for the last block in the region. This is impossible to determine from
 * within a preprocess_block function.
 *
 * @param $page
 *   Nested array of renderable elements that make up the page.
 */
function expertusoneV2_page_alter(&$page) {
  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }
}

function expertusoneV2_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  //Open the footer link in a new window tab Ticket: 0016248
  if($element['#theme'] == 'menu_link__menu_footer_menu') {
         $element['#localized_options']['attributes']['target'] = '_blank';
         //expDebug::dPrint(' Footer Menu Velu = '. print_r($variables, true) , 4) ;
  }

  if($element['#original_link']['menu_name'] == 'main-menu') {
		$userId = getSltpersonUserId();
    if($element['#original_link']['link_path'] == 'learning/myteam-search') {
      if(!is_manager($userId)){
        return '';
      }
    }

  	if($element['#original_link']['link_path'] == 'reports') {
			if((!user_access('New Report Perm') && !is_manager($userId) && !is_instructor($userId))){
  		        return '';
      }
    }

    if($element['#original_link']['link_path'] == 'learning/enrollment-search') {
      // Are these two lines needed as ctools setting ctools-modal-ctools-sample-style uses scale??
      // Commenting (if we face an issue, we will uncomment with a comment why these lines are needed)
      //$element['#original_link']['localized_options']['attributes']['class'][0] = 'use-ajax';
      //$element['#original_link']['localized_options']['attributes']['class'][1] = 'ctools-modal-ctools-sample-style';

      if (empty($GLOBALS['user']->uid)) { // Anonymous user sees the login dialog when opening My Learning page
        if ($element['#original_link']['link_path'] == 'learning/enrollment-search') {
          $element['#localized_options']['attributes']['class'][] = 'use-ajax';
          $element['#localized_options']['attributes']['class'][] = 'ctools-modal-ctools-login-style';
          $element['#localized_options']['attributes']['onclick'][] ='getClickMylearn()';
          $element['#href'] = 'ctools_ajax_sample/ajax/user_login';
        }
      }

      if ($element['#below']) {
    	  $sub_menu = drupal_render($element['#below']);
      }
      $output = l(t($element['#title']), $element['#href'], $element['#localized_options']);
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
    }

    if(($element['#original_link']['link_path'] == 'learning/dummy') && ($element['#original_link']['link_title'] == 'REPORTS')) {
      if ($element['#below']) {
        $sub_menu = drupal_render($element['#below']);
      }
      $output = l(t($element['#title']), 'portalpages/authenticated-user/reports', $element['#localized_options']);
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
    }
  }

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  if ($element['#href'] == '<front>' && drupal_is_front_page()) { // Changes Made For This Ticket #0040416 => active-trail Class is missing for home menu
  	$element['#attributes']['class'][] = 'active-trail';
  }
  $output = l(t($element['#title']), $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Themes the checkout review order page.
 *
 * @param $panes
 *   An associative array for each checkout pane that has information to add to
 *   the review page.  The key is the pane's title and the value is either the
 *   data returned for that pane or an array of returned data.
 * @param $form
 *   The HTML version of the form that by default includes the 'Back' and
 *   'Submit order' buttons at the bottom of the review page.
 *
 * @return
 *   A string of HTML for the page contents.
 *
 * @ingroup themeable
 */

function expertusoneV2_preprocess_uc_cart_checkout_exp_paymethod(&$variables) {
	// for 43830: "Unknown" text showing in billing details when country has no state.
	if(isset($variables['panes']['Billing details']['render_html']) && is_array($variables['panes']['Billing details']['render_html'])) {
		foreach ($variables['panes']['Billing details']['render_html'] as &$row) {
			if (is_array($row)) {
				if($row['title'] == 'Address') {
					$row['data'] = str_replace('<br />Unknown', '', $row['data']);
				}
			}
		}
	}
}

function expertusoneV2_uc_cart_checkout_review($variables) {
  $panes = $variables['panes'];
  $form  = $variables['form'];
  drupal_add_css(drupal_get_path('module', 'uc_cart') . '/uc_cart.css');

  foreach ($panes as $paneUniqueRef => $data) { // main looping
    expDebug::dPrint('$paneUniqueRef = ' . print_r($paneUniqueRef, true), 5);
    $title = $data['title'];
    $content = $data['content'];

    expDebug::dPrint('$title = ' . print_r($title, true), 5);
    expDebug::dPrint('$content = ' . print_r($content, true), 5);

    $output_pane = '<div>';
    if(($paneUniqueRef != 'clone_uc_checkout_pane_cart') && ($paneUniqueRef != 'uc_checkout_pane_payment'))
        $output_pane .= '<div class="d-cart-review-label">' .ucwords($title) .'</div>'; // section layer

    if($paneUniqueRef=='uc_checkout_pane_payment') {
      $output_pane .= '<div style="float:right;"><table>';
      if(variable_get('uc_payment_credit_gateway', '')!='exp_paymetric'){
        $output_payment_pane = '<div class="d-cart-review-label">' .t('LBL781') .'</div>';
      }else{
        $output_payment_pane = '<div class="d-cart-review-label"></div>';
      }
    }
   if (is_array($content)) {
      foreach ($content as $row) {
          if (is_array($row)) {
          	/* 55634 : Remove the subtotal, total, tax, & discount in order checkout page*/
          	//expDebug::dPrint("Row Title 111".$row['title'].", ".t('Tax'),4);
          	if($row['title'] == t('LBL553').":" || $row['title'] == "" || $row['title'] == t('Discount')  || $row['title'] == t('Discount').":" || $row['title'] == 'Tax' || $row['title'] == t('Tax') || $row['title'] == t('LBL827').":")
          		continue;

          	if(empty($row['data']))
              $row['data'] = '&nbsp;';
            if($row['title'] == 'Address') {
            	$row['data'] = str_replace('<br />Unknown', '', $row['data']);
            }
              if($paneUniqueRef=='uc_checkout_pane_payment'){
                $total_padding_bottom = '';
                if(stristr($row['title'],'total') || stristr($row['title'],'tax'))
                  $total_padding_bottom = "padding-bottom:5px;padding-top:5px;";
                if(strstr($row['data'], "$") && strstr($row['data'], 'class="uc-price"')) {
                $output_pane .= '<tr>';
                    $output_pane .= '<td class="field-label" style="float:right;'.$total_padding_bottom.'"><b>'. $row['title'] .'</b></td>';
                    $output_pane .= '<td class="d-data-review" style="text-align:right;'.$total_padding_bottom.'">'. $row['data'] .'</td>';
                    $output_pane .= '</tr>';
                }
                else {
                  $output_payment_pane .= '<div>';
                  // if(variable_get('uc_payment_credit_gateway', '')!='exp_paymetric'){
                  $row['data'] = ($row['data'] == 'Check') ? t('Check') : $row['data'];
                  $row['data'] = ($row['data'] == 'Credit card') ? t('Credit card') : $row['data'];
                    $output_payment_pane .= '<div class="field-label">'. $row['title'] .': </div>';
                    $output_payment_pane .= '<div class="d-data-review" >'. t($row['data']) .'</div>';
                  // }else{
                    // $output_payment_pane .= '<div class="field-label"></div>';
                    // $output_payment_pane .= '<div class="d-data-review" ></div>';
                  // }
				  
                    $output_payment_pane .= '</div>';
                }
              }
              else {
                $output_pane .= '<div>'; // third layer wrapper for row-title, double dot and row data
                if($paneUniqueRef != 'uc_checkout_pane_comments')
                 $output_pane .= '<div class="field-label">'. $row['title'] .': </div>';

                $output_pane .= '<div class="d-data-review">'. $row['data'] .'</div>';
                $output_pane .= '</div>'; // end third layer wrapper
              }

        }
        else {
                  if($paneUniqueRef!='uc_checkout_pane_payment')
                    $output_pane .= '<div>'; // third layer wrapper for row-title, double dot and row
                  if($paneUniqueRef=='clone_uc_checkout_pane_cart'){
                    if($row != 'clone_cart')
                      $output_pane .= '<div>'. $row .'</div>'; // third layer for product
                  }
                  if($paneUniqueRef!='uc_checkout_pane_payment')
                    $output_pane .= '</div>'; // end third layer
        }
      }
      if($paneUniqueRef=='uc_checkout_pane_payment'){
        $output_pane .= '</table></div>';
      }
    }
    else {
          $output_pane .= '<div id="d-cart-heading">'; // third layer wrapper for row-title, double dot and row
          $output_pane .= '<div id="header-wrapper">'. $content .'</div>'; // third layer
          $output_pane .= '</div>'; // end third layer
    }
    $output_pane .= '</div>';
    $output_pane_list[] = $output_pane;
  }

  $output ='<div id="d-cart-review-wrapper-top">' .
             '<div class="d-cart-review">' .
//               check_markup(variable_get('uc_checkout_review_instructions', uc_get_message('review_instructions')), variable_get('uc_checkout_review_instructions_format', filter_fallback_format()), FALSE) .
             '</div>' .
           '</div>' .
           '<div id="d-cart-review-wrapper">' .
             '<table style="width:100%">' .
               '<tr>' .
                 '<td style="vertical-align:top;">' . $output_pane_list[0] . '</td>' .
               '</tr></table>' .
               //'<tr>' .
                 //'<td style="vertical-align:top;">' . $output_pane_list[1] . '</td>' .
                 //'<td>' . $output_pane_list[3] . '</td>'.
                 '<div class="common-style-to-review-order user">' . $output_pane_list[2] . '</div>' .
               //'</tr>' .
               //'<tr>' .
                 '<div class="common-style-to-review-order billing">'.$output_pane_list[1].'</div>' .
               //'</tr>' .
               //'<tr>' .
                 '<div class="clearBoth"></div><div class="common-style-to-review-order commerece-payment-infomation-wrapper">'.$output_payment_pane.'</div>' .
               //'</tr>' .
               //'<tr>' .
                 '<div class="common-style-to-review-order comments">'.$output_pane_list[4]. '</div>' .
               //'</tr>' .
            //'</table>' .
          '</div>' .
          '<div id="d-cart-review-wrapper-bottom">' .
            '<div id="d-review-button">'. drupal_render($form).'</div>' .
          '</div>';

  expDebug::dPrint('$output = ' . print_r($output, true), 5);
  return $output;

}


/**
 * Theme override for theme_status_messages() in includes/theme.inc to not display blank messages.
 * Returns HTML for status and/or error messages, grouped by type.
 *
 * An invisible heading identifies the messages for assistive technology.
 * Sighted users see a colored box. See http://www.w3.org/TR/WCAG-TECHS/H69.html
 * for info.
 *
 * @param $variables
 *   An associative array containing:
 *   - display: (optional) Set to 'status' or 'error' to display only messages
 *     of that type.
 */
function expertusoneV2_status_messages($variables) {

  expDebug::dPrint('template.php : expertusoneV2_status_messages() : $variables = ' . print_r($variables, true),4);
  $display = $variables['display'];
  $output = '';
  $outputval='';
  $charLengthCheck = 0;
  if($variables['char_length'] > 0){
    $charLengthCheck = 1;
    $allowedStrLength = $variables['char_length'];
  }
  $status_heading = array(
    'status' => t('Status message'),
    'error' => t('Error message'),
    'warning' => t('Warning message'),
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    // Get count of non-blank messages
    $nonBlankMsgCount = 0;
    $newMessages = array();
    $newMessages[0] = '';
    $inc = 1;
    $resultcnt=0;
    expDebug::dPrint('old message value:' . print_r($messages,true),4);
    foreach ($messages as $message) {
      $trimmedMessage = trim($message);
      if (!empty($trimmedMessage)) {
        $nonBlankMsgCount++;
      }
      if(trim($message) == t('LBL1273')){ // form field contains any script tags
      	$messages = array();
      	$messages[] = $message;
      	$nonBlankMsgCount = 1;
      	$newMessages = array();
      	break;
      }
      if(strpos(trim($message),t('ERR101'))){
        $message = trim(str_replace(t('ERR101'),'',$message));
        $newMessages[0] = empty($newMessages[0]) ? $message : $newMessages[0] . ', ' . $message;
      }
      else{
        $newMessages[$inc] = $message;
        $inc++;
      }
    }
    expDebug::dPrint('new message value:' . $newMessages[0],4);
    if(!empty($newMessages[0])){
      $pos = strrpos($newMessages[0],',');
      if($pos>0){
      		$newMessages[0] = substr_replace($newMessages[0],' '.strtolower(t('LBL647')),$pos,1);
      		$newMessages[0] = $newMessages[0] . t('ERR169');
      }else{
      		$newMessages[0] = $newMessages[0] . t('ERR101');
      	}
      $messages = $newMessages;
    }
    // Create a div for the type only if there is at least one non-blank message
    if ($nonBlankMsgCount > 0) {
      $output .= "<div id=\"message-container\"><div class=\"messages $type\">"; // class=\"messages $type\" All messages are same now
      /*if (!empty($status_heading[$type])) {
       $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
       }*/
      if ($nonBlankMsgCount > 1) {
        //$output .= " <ul class=\"messageul\">";
        $resultcnt=count($messages);
        if($resultcnt>1){
        	for($start=0;$start<$resultcnt;$start++){
        	 $outputval .='<li class="hide" style="min-width:100px;"><span>' . $messages[$start]. '</span></li>';
        	 expDebug::dPrint("ERROR MSG ::: ".$outputval,4);
        	}
        	$output .= "<span class='qtip-error-msg-display'><ul>".$outputval."</ul></span>" ;
        }
        else{
        foreach ($messages as $message) {
          $trimmedMessage = trim($message);
          if (!empty($trimmedMessage)) {
            if($charLengthCheck && $allowedStrLength){
              $strLength = strlen($message);
              if($strLength > $allowedStrLength){
                $message = substr($message, 0, $allowedStrLength) . '...';
              }
            }
            $output .= "<ul><li><span>" . $message . "</span></li></ul>";
          }
        }
        }
        //$output .= "</ul>";
      }
      else {
        foreach ($messages as $message) {
          $trimmedMessage = trim($message);
          if (!empty($trimmedMessage)) {
            if($charLengthCheck && $allowedStrLength){
              $strLength = strlen($message);
              if($strLength > $allowedStrLength){
                $message = substr($message, 0, $allowedStrLength) . '...';
              }
            }
            $output .= "<ul><li><span>" . $message . "</span></li></ul>";
            //$output .= $message;
            break;
          }
        }
      }
      /*if($resultcnt>1){
      	//$output .= "<div class='msg-min-btn'></div><div class='msg-max-btn'></div>";
      	$output .= "<div class='msg-minmax-icon'></div>";
      }*/
      $output .= '<div class="msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div></div></div>';
      $output .= '<img onload="try{$(\'body\').data(\'learningcore\').showHideMultipleLi();}catch(e){}" style="display:none;" src="sites/all/themes/core/expertusoneV2/expertusone-internals/images/close.png" height="0" width="0" />';
    }
  }
  return $output;
}


/**
 * Theme override for theme_multiselect() in multiselect.module to fix issues with user selected values getting removed from the
 * selected values list when a form validation error happens for the form that includes this element.
 * Returns HTML for a select form element.
 *
 * It is possible to group options together; to do this, change the format of
 * $options to an associative array in which the keys are group labels, and the
 * values are associative arrays in the normal $options format.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #options, #description, #extra,
 *     #multiple, #required, #name, #attributes, #size.
 *
 * @ingroup themeable
 */
function expertusoneV2_multiselect($variables) {
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : $variables = ' . print_r($variables, true));

  $element = $variables['element'];
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : $element = ' . print_r($element, true));

  // If required attribute is not set or set in false, it will be removed from list
  if(empty($element['#required']) || ($element['#required'] == FALSE)) {
    element_set_attributes($element, array('id', 'name', 'size', 'multiple', 'default_value'));
  } else {
    element_set_attributes($element, array('id', 'name', 'size', 'multiple', 'default_value', 'required'));
  }
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : after element_set_attributes() $element = ' . print_r($element, true));

  _form_set_class($element, array('form-multiselect'));
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : after _form_set_class() $element = ' . print_r($element, true));

  $options = $element['#options']; // All available options as defined by the element
  if (isset($options['none_selected']) && isset($element['#validated']) && $element['#validated'] == 1) {
  	$items = $element['#value']; // All selected options are referred to as "items".
  }
  else {
  	$items = $element['#default_value']; // Default behaviour retained if 'none_selected' is not an option.
  }
  //isset($element['#needs_validation']) && $element['#needs_validation'] == 1? $element['#value'] : $element['#default_value'];
  $element['#field_name'] = $element['#name']; // CCK calls the #name "#field_name", so let's duplicate that..
  $required = $element['#required'];

  $widget = _multiselect_build_widget_code($options, $items, $element, $required);
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : $widget = ' . print_r($widget, true));

  // Add a couple of things into the attributes.
  $element['#attributes']['class'][] = $widget['selfield'];
  $element['#attributes']['class'][] = "multiselect_sel";
  $element['#attributes']['id'] = $element['#field_name'];

  $renderedElem = $widget['prefix_pre'] .
                  $widget['prefix_options'] .
                  $widget['prefix_post'] .
                  '<div class="form-item form-type-select">' .
                    '<select' . drupal_attributes($element['#attributes']) . '>' .
                      _multiselect_html_for_box_options($widget['selected_options']) .
                    '</select>' .
                  '</div>' .
                '</div>';
  expDebug::dPrint('template.php : expertusoneV2_multiselect() : $renderedElem = ' . print_r($renderedElem, true));
  return $renderedElem;
}