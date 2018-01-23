<?php
/**
 * @file
 * Cart menu items.
 */

/**
 * Displays the cart view page.
 *
 * Show the products in the cart with a form to adjust cart contents or go to
 * checkout.
 */

function uc_cart_view() {
  // Failsafe so that this function only works when called with no arguments.
  // This prevents the accidental wiping of the cart_order session variable.
  if (func_num_args() > 0) {
    return MENU_NOT_FOUND;
  }

  // Clear the cart order session variable if it exists.
  if (!empty($_SESSION['cart_order'])) {
    unset($_SESSION['cart_order']);
  }
  unset($_SESSION['admin_discount']);
  unset($_SESSION['discount']);
  unset($_SESSION['admin_total_discount']);
  unset($_SESSION['total_discount']);

  // Load the array of shopping cart items.
  $items = uc_cart_get_contents();

  // Display the empty cart page if there are no items in the cart.
  if (empty($items)) {
    return array(
      '#theme' => 'uc_empty_cart',
    );
  }

  $build = array();
  // Load through the cart panes...
  foreach (uc_cart_cart_pane_list($items) as $id => $pane) {
    // If the pane is enabled...
    if ($pane['enabled']) {
      // Add its output to the cart view.
      $build[$id] = $pane['body'];
    }
  }

  // Add a custom cart breadcrumb if specified.
  if (($text = variable_get('uc_cart_breadcrumb_text', '')) !== '') {
    $link = l($text, variable_get('uc_cart_breadcrumb_url', '<front>'));
    drupal_set_breadcrumb(array($link));
  }

  return $build;
}

/**
 * Displays the cart checkout page built of checkout panes from enabled modules.
 */
function uc_cart_checkout() {
  global $user;

  $items = uc_cart_get_contents();
  expdebug::dPrint('$items = ' . print_r($items, true), 5);

  if (count($items) == 0 || !variable_get('uc_checkout_enabled', TRUE)) {
    drupal_goto('cart');
  }

  if (($min = variable_get('uc_minimum_subtotal', 0)) > 0) {
    $subtotal = 0;
    if (is_array($items) && count($items) > 0) {
      foreach ($items as $item) {
        $data = module_invoke($item->module, 'uc_cart_display', $item);
        if (!empty($data)) {
          $subtotal += $data['#total'];
        }
      }
    }

    if ($subtotal < $min) {
      drupal_set_message(variable_get('uc_minimum_subtotal_text', t('The minimum order subtotal for checkout is @min.',
                                              array('@min' => uc_currency_format($min)))), 'error');
      drupal_goto('cart');
    }
  }

  // Send anonymous users to login page when anonymous checkout is disabled.
  if (!$user->uid && !variable_get('uc_checkout_anonymous', TRUE)) {
    drupal_set_message(t('You must login before you can proceed to checkout.'));
    if (variable_get('user_register', 1) != 0) {
      drupal_set_message(t('If you do not have an account yet, you should <a href="!url">register now</a>.',
                                                  array('!url' => url('user/register', array('query' => drupal_get_destination())))));
    }
    drupal_goto('user', array('query' => drupal_get_destination()));
  }

  $list = _uc_line_item_list();
  expDebug::dPrint('$list = ' . print_r($list, true), 5);

  foreach ($list as $line_item) {
    if (isset($line_item['callback']) && function_exists($line_item['callback'])) {
      $line_item['callback']('cart-preview', $items); // None of the expertus used line items seem to have cart-preview implemented.
                                                      // subtotal list item has 'load' op, whereas total list item has 'display' op.
                                                      // uc_discounts list item does not have a callback.
    }
  }

  drupal_add_js(drupal_get_path('module', 'uc_cart') . '/uc_cart.js');
  $build = drupal_get_form('uc_cart_checkout_form');

  return $build;
}

/**
 * The checkout form built up from the enabled checkout panes.
 *
 * @see uc_cart_checkout_form_validate()
 * @see uc_cart_checkout_form_review()
 * @see uc_cart_checkout_review()
 * @see theme_uc_cart_checkout_form()
 * @ingroup forms
 */
function uc_cart_checkout_form($form, &$form_state) {
	expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);
	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
	expDebug::dPrint('$_SESSION = ' . print_r($_SESSION, true), 5);
	expDebug::dPrint('$_SERVER = ' . print_r($_SERVER, true), 5);


  global $user;

  if (isset($form_state['storage']['order'])) {
    $order = $form_state['storage']['order'];
  }
  else {
    if (isset($_SESSION['cart_order'])) {
      $order = uc_order_load($_SESSION['cart_order']);
    }
    else {
      $order = NULL;
    }
  }

  // Check the referer URI to clear order details and prevent identity theft.
  if (uc_referer_check(array('cart/checkout', 'cart/checkout/paymethod'))) { // same page, or back from paymethod
    if ($order == FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
      unset($_SESSION['cart_order']);
      $order = NULL;
    }
    elseif (uc_order_status_data($order->order_status, 'state') != 'in_checkout' || ($user->uid > 0 && $user->uid != $order->uid)) {
      $order = NULL;
    }
  }
  else {
    unset($_SESSION['cart_order']);
    $order = NULL;
  }

  if (is_null($order)) {
    $order = new UcOrder();
    $order->products = uc_cart_get_contents();
  }

  expDebug::dPrint('$order = ' . print_r($order, true), 5);

  $form['panes'] = array('#tree' => TRUE);
  $panes = _uc_checkout_pane_list();
  expDebug::dPrint('$panes = ' . print_r($panes, true), 5);

  // If the cart isn't shippable, remove panes with shippable == TRUE.
  if (!uc_cart_is_shippable() && variable_get('uc_cart_delivery_not_shippable', TRUE)) {
    $panes = uc_cart_filter_checkout_panes($panes, array('shippable' => TRUE));
  }

  expDebug::dPrint('$panes after shippable panes removed = ' . print_r($panes, true), 5);

  foreach ($panes as $id => $pane) {
    if ($pane['enabled']) {
      $pane['prev'] = _uc_cart_checkout_prev_pane($panes, $id);
      $pane['next'] = _uc_cart_checkout_next_pane($panes, $id);

      if (!isset($pane['collapsed'])) {
        $collapsed = ($pane['prev'] === FALSE || empty($displayed[$pane['prev']])) ? FALSE : TRUE;
      }
      if (isset($form_state['expanded_panes']) && in_array($id, $form_state['expanded_panes'])) {
        $collapsed = FALSE;
      }

      $return = $pane['callback']('view', $order, $form, $form_state);

      // Add the pane if any display data is returned from the callback.
      if (is_array($return) && (!empty($return['description']) || !empty($return['contents']))) {
        // Create the fieldset for the pane.
        $form['panes'][$id] = array(
          '#type' => 'fieldset',
          '#title' => $pane['title'],
          '#description' => !empty($return['description']) ? $return['description'] : NULL,
          '#collapsible' => $pane['collapsible'],
          '#collapsed' => variable_get('uc_use_next_buttons', FALSE) ? $collapsed : FALSE,
          '#id' => $id . '-pane',
          '#theme' => isset($return['theme']) ? $return['theme'] : NULL,
        );

        // Add the contents of the fieldset if any were returned.
        if (!empty($return['contents'])) {
          $form['panes'][$id] = array_merge($form['panes'][$id], $return['contents']);
          expDebug::dPrint('$form[panes][$id] = ' . print_r($form['panes'][$id], true), 5);
        }

        // Add the 'Next' button if necessary.
        if ((!isset($return['next-button']) || $return['next-button'] !== FALSE) && $pane['next'] !== FALSE &&
            variable_get('uc_use_next_buttons', FALSE) != FALSE) {
          $opt = variable_get('uc_collapse_current_pane', FALSE) ? $id : 'false';
          $form['panes'][$id]['next'] = array(
            '#type' => 'button',
            '#value' => t('Next'),
            '#weight' => 20,
            '#attributes' => array('onclick' => "return uc_cart_next_button_click(this, '" . $pane['next'] . "', '" . $opt . "');"),
            '#prefix' => '<div class="next-button">',
            '#suffix' => '</div>',
          );
        }

        // Log that this pane was actually displayed.
        $displayed[$id] = TRUE;
      }
    }
  }
  unset($form_state['expanded_panes']);

  $contents = uc_cart_get_contents();

  $form['cart_contents'] = array(
    '#type' => 'hidden',
    '#value' => serialize($contents),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('LBL109'),
    '#validate' => array(), // Disable validation to prevent a new order from being created.
    '#limit_validation_errors' => array(),
    '#submit' => array('uc_cart_checkout_form_cancel'),
  );
  $form['actions']['continue'] = array(
    '#type' => 'submit',
    '#value' => t('LBL986'),//Enter Payment Details
  );

  unset($_SESSION['do_exp_paymethod']);

  return $form;
}

/**
 * Adds markup and styling to the checkout panes.
 *
 * @see uc_cart_checkout_form()
 * @ingroup themeable
 */
function theme_uc_cart_checkout_form($variables) {
  $form = $variables['form'];

  drupal_add_css(drupal_get_path('module', 'uc_cart') . '/uc_cart.css');

  $output = '<div id="checkout-instructions">' . filter_xss_admin(variable_get('uc_checkout_instructions', '')) . '</div>';

  foreach (element_children($form['panes']) as $pane_id) {
    if (function_exists(($func = _uc_checkout_pane_data($pane_id, 'callback')))) {
    	expDebug::dPrint('pane theme function for pane ' . $pane_id . ' is ' . $func, 5);
      $result = $func('theme', $form['panes'][$pane_id], NULL);
      if (!empty($result)) {
        $output .= $result;
        $form['panes'][$pane_id] = array();
      }
      else {
        $output .= drupal_render($form['panes'][$pane_id]);
      }
    }
    else {
      $output .= drupal_render($form['panes'][$pane_id]);
    }
  }

  $output .= '<div style="clear:both"></div>' .
             '<div id="checkout-form-bottom">' . drupal_render_children($form) . '</div>';

  return $output;
}

/**
 * Form validation for uc_cart_checkout_form().
 *
 * @see uc_cart_checkout_form()
 * @see uc_cart_checkout_form_submit()
 */
function uc_cart_checkout_form_validate($form, &$form_state) {
	unset($_SESSION['checkout_validation_failed']);

  global $user;
  if (empty($form_state['storage']['order'])) {
  	if(isset($_SESSION['cart_order'])){
  		$order = uc_order_load($_SESSION['cart_order']);
  	}else{
  		$order = uc_order_new($user->uid);
  		$form_state['storage']['order'] = $order;
  	}
  }
  else {
  	$order = $form_state['storage']['order'];
  }

  $_SESSION['cart_order'] = $order->order_id;

  db_delete('uc_order_products')
    ->condition('order_id', $order->order_id)
    ->execute();

  $order->products = unserialize($form_state['values']['cart_contents']);

  $order->order_total = uc_order_get_total($order, TRUE);

  // Validate/process the cart panes.  A FALSE value results in failed checkout.
  $form_state['checkout_valid'] = TRUE;
  foreach (element_children($form_state['values']['panes']) as $pane_id) {
    $func = _uc_checkout_pane_data($pane_id, 'callback');
    if (is_string($func) && function_exists($func)) {
      $isvalid = $func('process', $order, $form, $form_state);
      if ($isvalid === FALSE) {
        $form_state['expanded_panes'][] = $pane_id;
        $form_state['checkout_valid'] = FALSE;
      }
    }
  }
  expertus_xss_validate_checkout($form, $form_state);
  if (!empty($form_state['input']['panes']['billing']['billing_postal_code']) && preg_match('#[^a-zA-Z0-9]#', $form_state['input']['panes']['billing']['billing_postal_code'])) {
  	form_set_error('panes][billing][billing_postal_code',t('ERR071').' '.t('LBL562'));
  }
  $order->line_items = uc_order_load_line_items($order, TRUE);
  $order->line_items = array_merge($order->line_items, uc_order_load_line_items($order, FALSE));
  usort($order->line_items, 'uc_weight_sort');

  $formErrors = form_get_errors();
  expDebug::dPrint('$formErrors = ' . print_r($formErrors, true), 5);
  if ($form_state['checkout_valid'] == FALSE || !empty($formErrors)) {
    $_SESSION['checkout_validation_failed'] = TRUE; // is read by exp_sp_cybersource_tax.module hook_uc_order()
  }

  expDebug::dPrint('calling uc_order_save for $order = ' . print_r($order, true), 5);
  $order->currency_type = (isset($_SESSION['admin_shop_cart_currency_code']) && !empty($_SESSION['admin_shop_cart_currency_code']))?$_SESSION['admin_shop_cart_currency_code']:$_SESSION['shop_cart_currency_code']; //multi currency work
  uc_order_save($order);
  $form_state['storage']['order'] = uc_order_load($order->order_id, TRUE);
  if($form_state['input']['op'] == 'Continue' && $form_state['storage']['order']->order_total == 0){
  	$function = 'uc_discounts_uc_order';
  	if (function_exists($function)) {
  		expDebug::dPrint('admin_order_payment calling function ' . $function, 5);
  		// $order must be passed by reference.
  		$result = $function('submit', $order, NULL);
  	}
  }
}


/**
 * Form submission handler for uc_cart_checkout_form().
 *
 * @see uc_cart_checkout_form()
 * @see uc_cart_checkout_form_validate()
 */
function uc_cart_checkout_form_submit($form, &$form_state) {
	//	expDebug::dPrint('checking the order for m state in  form submit'.print_r($form_state,true));
	if($form_state['input']['op'] == 'Continue' && $form_state['storage']['order']->order_total == 0){
		$_SESSION['do_complete'] = TRUE;
		drupal_goto('cart/checkout/complete');
	}else{
		if ($form_state['checkout_valid'] === FALSE) {
			$url = 'cart/checkout';
		}
		else {
			$url = 'cart/checkout/paymethod';
			$_SESSION['do_exp_paymethod'] = TRUE;
		}

		unset($form_state['checkout_valid']);

		$form_state['redirect'] = $url;
	}
}

/**
 * @see uc_checkout_form()
 */
function uc_cart_checkout_form_cancel($form, &$form_state) {
  if (isset($_SESSION['cart_order']) && intval($_SESSION['cart_order']) > 0) {
    uc_order_comment_save($_SESSION['cart_order'], 0, t('Customer cancelled this order from the checkout form.'));
    unset($_SESSION['cart_order']);
  }

  $form_state['redirect'] = 'cart';
}

/**
 * Allows a customer to review their order before finally submitting it.
 *
 * @see uc_cart_checkout_form()
 */
function uc_cart_checkout_review() {
  drupal_add_js(drupal_get_path('module', 'uc_cart') . '/uc_cart.js');

  if (empty($_SESSION['do_review'])) {
    drupal_goto('cart/checkout/paymethod');
  }

  $order = uc_order_load($_SESSION['cart_order']);

  if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    unset($_SESSION['cart_order']);
//     unset($_SESSION['cartform']);
//     unset($_COOKIE['admin_shop_cart_currency_code']);
//     unset($_COOKIE['admin_shop_cart_currency_sym']);
//     setcookie('admin_shop_cart_currency_code', null, -1, '/');
//     setcookie('admin_shop_cart_currency_sym', null, -1, '/');
//     unset($_SESSION['admin_shop_cart_currency_code']);
//     unset($_SESSION['admin_shop_cart_currency_sym']);
    unset($_SESSION['shop_cart_currency_code']);
    unset($_SESSION['shop_cart_currency_sym']);
    drupal_goto('cart');
  }

  $panes = _uc_checkout_pane_list();
  expDebug::dPrint('$panes = ' . print_r($panes, true), 5);

  // If the cart isn't shippable, bypass panes with shippable == TRUE.
  if (!uc_cart_is_shippable() && variable_get('uc_cart_delivery_not_shippable', TRUE)) {
    $panes = uc_cart_filter_checkout_panes($panes, array('shippable' => TRUE));
  }

  foreach ($panes as $pane) {
    if ($pane['enabled']) {
      $func = $pane['callback'];
      if (function_exists($func)) {
        $return = $func('review', $order, NULL);
        if (!is_null($return)) {
          $data[$func]['title'] = $pane['title'];
          $data[$func]['content'] = $return;
        }
      }
    }
  }

  $build = array(
    '#theme' => 'uc_cart_checkout_review',
    '#panes' => $data,
    '#form' => drupal_get_form('uc_cart_checkout_review_form', $order),
  );

  return $build;
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
function theme_uc_cart_checkout_review($variables) {
  $panes = $variables['panes'];
  $form = $variables['form'];

  drupal_add_css(drupal_get_path('module', 'uc_cart') . '/uc_cart.css');

  $output = '<div id="review-instructions">' . filter_xss_admin(variable_get('uc_checkout_review_instructions', uc_get_message('review_instructions'))) . '</div>';

  $output .= '<table class="order-review-table">';

  foreach ($panes as $paneUniqueRef => $data) {
    $title = $data['title'];
    $content = $data['content'];

    $output .= '<tr class="pane-title-row"><td colspan="2">' . $title
              . '</td></tr>';
    if (is_array($content)) {
      foreach ($content as $row) {
        if (is_array($row)) {
          if (isset($row['border'])) {
            $border = ' class="row-border-' . $row['border'] . '"';
          }
          else {
            $border = '';
          }
          $output .= '<tr valign="top"' . $border . '><td class="title-col" '
                    . 'nowrap>' . $row['title'] . ':</td><td class="data-col">'
                   . $row['data'] . '</td></tr>';
        }
        else {
          $output .= '<tr valign="top"><td colspan="2">' . $row . '</td></tr>';
        }
      }
    }
    else {
      $output .= '<tr valign="top"><td colspan="2">' . $content . '</td></tr>';
    }
  }

  $output .= '<tr class="review-button-row"><td colspan="2">' . drupal_render($form)
            . '</td></tr></table>';

  return $output;
}

/**
 * Gives customers the option to finish checkout or go revise their information.
 *
 * @see uc_cart_checkout_review_form_back()
 * @see uc_cart_checkout_review_form_submit()
 * @ingroup forms
 */
function uc_cart_checkout_review_form($form, &$form_state, $order) {
	expDebug::dPrint('$order = ' . print_r($order, true), 5);

  $form_state['uc_order'] = $order;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array('uc_cart_checkout_review_form_back'),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit order'),
  );

  expDebug::dPrint('$form = ' . print_r($form, true), 5);
  return $form;
}

/**
 * Returns the customer to the checkout page to edit their information.
 *
 * @see uc_cart_checkout_review_form()
 */
function uc_cart_checkout_review_form_back($form, &$form_state) {
  unset($_SESSION['do_review']);
  $_SESSION['do_exp_paymethod'] = TRUE;
  $form_state['redirect'] = 'cart/checkout/paymethod';
}

function purchaseClassAPI($user_id,$userid,$order_id,$payment_method,$po_number,$cc_number,$cc_exp_month,$cc_exp_year,$cc_cvv,$currency_code=''){
	try{
		drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
		include_once($_SERVER["DOCUMENT_ROOT"]. 'sites/all/modules/core/exp_sp_learning/exp_sp_learning.inc');
		
	//Below function will set currency details in $_REQUEST
		$currencyDetails = splitCurrencyInputAndSetInRequest();
		
		if(empty($currencyDetails))
		{
		$errobj=new stdClass();
		$errobj->isValidateError = 1;
		$errobj->errcode = 'L_012';
		$errobj->field = 'currency_type';
		$errobj->errormsg = "The given Currency type is invalid or inactive status";
		return $errobj;
		}
		expDebug::dPrint('currency_type = ' .$_REQUEST['currency_type']);
		expDebug::dPrint('currency_sym  = ' .$_REQUEST['currency_sym']);
		
		if ($_REQUEST['currency_type'] && $_REQUEST['currency_sym']) {
			$_SESSION['shop_cart_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_code'] = $_REQUEST['currency_type'];
			$_SESSION['user_preferred_currency_sym'] = $_REQUEST['currency_sym'];
		} else if ($_REQUEST['currency_type']) {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		} else {
			$getDefaultCurrency = getDefaultCurrency();
			$UserPrefCurrCode = $getDefaultCurrency['preferred_currency'];
			$UserPrefCurrSym = $getDefaultCurrency['attr2'];

			$_SESSION['shop_cart_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_code'] = $UserPrefCurrCode;
			$_SESSION['user_preferred_currency_sym'] = $UserPrefCurrSym;
		}


		$uid = getDrupalIdOfUser($user_id);

		$selectStmt = db_select('users', 'usr');

  		$selectStmt->condition('usr.uid',$uid);

 	 	 $selectStmt->addField('usr', 'name', 'name');
 		 $selectStmt->addField('usr', 'pass', 'pass');
 	 	 $selectStmt->addField('usr', 'mail', 'mail');
 	 	 $selectStmt->addField('usr', 'theme', 'theme');
 		 $selectStmt->addField('usr', 'signature', 'signature');
 		 $selectStmt->addField('usr', 'signature_format', 'signature_format');
 		 $selectStmt->addField('usr', 'created', 'created');
 		 $selectStmt->addField('usr', 'access', 'access');
 		 $selectStmt->addField('usr', 'login', 'login');
	 	 $selectStmt->addField('usr', 'status', 'status');
	 	 $selectStmt->addField('usr', 'timezone', 'timezone');
	 	 $selectStmt->addField('usr', 'language', 'language');
	 	 $selectStmt->addField('usr', 'picture', 'picture');
	 	 $selectStmt->addField('usr', 'init', 'init');
	 	 $selectStmt->addField('usr', 'data', 'data');

	 	 $global_user_values = $selectStmt->execute()->fetchAll();

	 	 global $user;

	 	 $user->uid = $uid;
	 	 $user->name = $global_user_values[0]->name;
	 	 $user->mail = $global_user_values[0]->mail;
	 	 $user->theme = $global_user_values[0]->theme;
	 	 $user->signature = $global_user_values[0]->signature;
	 	 $user->signature_format = $global_user_values[0]->signature_format;
	 	 $user->created = $global_user_values[0]->created;
	 	 $user->access = $global_user_values[0]->access;
	 	 $user->login = $global_user_values[0]->login;
	 	 $user->status = $global_user_values[0]->status;
	 	 $user->timezone = $global_user_values[0]->timezone;
	 	 $user->picture = $global_user_values[0]->picture;
	 	 $user->init = $global_user_values[0]->init;
	 	 $user->data = $global_user_values[0]->data;


	 	 if($_REQUEST['session_discount'])
	 	 {
	 	 	$_SESSION['discount'] = json_decode($_REQUEST['session_discount'], true);
	 	 	//                      $_SESSION['discount'] =  (array) $_SESSION['discount'];
	 	 }
	 	 
	 	 expDebug::dPrint(' session_discount array output = ' . print_r($_SESSION['discount'], true),3);
	 	 
 	 	expDebug::dPrint(' global_user_values = ' . print_r($global_user_values, true),3);
		
		/* code added for ilt & vc  session validation */
		
		$slt_payment_order_id = getSltOrderId($order_id);
  		$prgInvalid = getClassSessionValidate($slt_payment_order_id);
  		expDebug::dPrint("session invalid check >>>> ". print_r($prgInvalid,true),5);
		if(count($prgInvalid) > 0){
		$error = '';
		foreach($prgInvalid as $key=>$val){
			expDebug::dPrint("value ".$val);
			$error.=   $val .'-~-';
	  	}
		expDebug::dPrint("session invalid check >>>> ".$error);
		
		$result-> status =  "invalidsession";
		$result-> msg =  $error;
		return $result;
		}
		/* code added for ilt & vc  session validation */

		if($payment_method == "invoice")
        {
        	$payment_method = 'other';
        }

		$fields = array(
				'payment_method' => $payment_method,
		);
		$updStmt = db_update('uc_orders');
		$updStmt->fields($fields);
		$updStmt->condition('order_id', $order_id);
		expDebug::dPrintDBAPI('$updStmt', $updStmt);
		$updStmt->execute();

		$form = array();
   		 $form_state = array();

        if($payment_method == "po")
        {
	        $po_data = array();
	        $po_data["po_number"] = $po_number;
	        $order = uc_order_load($order_id);

	        $order->payment_details['po_number'] = $po_number;
	        uc_po_uc_order('submit',$order,'');
        }else if($payment_method == "Zero Cost")
        {
        	$_SESSION['do_complete'] = TRUE;
        	$_SESSION['cart_order'] = $order_id;
        	expDebug::dPrint('Order id ajay = '.$order_id);
        	$result_uc = uc_cart_checkout_complete('api');
//         	return $result_uc;
        }
        else if($payment_method == "credit")
        {
        	$enc = new Encrypt();

        	$cc_number = $enc->getAESDecryptedValue($cc_number);
        	$cc_exp_month = $enc->getAESDecryptedValue($cc_exp_month);
        	$cc_exp_year = $enc->getAESDecryptedValue($cc_exp_year);
        	$cc_cvv = $enc->getAESDecryptedValue($cc_cvv);

		     $form_state['values']['panes']['payment']['payment_method'] = "credit";
		     $form_state['values']['panes']['payment']['details']['cc_number'] = $cc_number;
		     $form_state['values']['panes']['payment']['details']['cc_exp_month'] = $cc_exp_month;
		     $form_state['values']['panes']['payment']['details']['cc_exp_year'] = $cc_exp_year;
		     $form_state['values']['panes']['payment']['details']['cc_cvv'] = $cc_cvv;
		     $form_state['values']['panes']['payment']['details']['cc_token'] = $_REQUEST['cc_token'];
		     $form_state['values']['panes']['comments'] ='';
		     $form_state['values']['panes']['uc_termsofservice_agreement_checkout']['tos_agree_terms']['agreed'] = agreed;
		     $form_state['values']['cancel']='Cancel';
		     $form_state['values']['continue']='Continue';
		     $form_state['values']['back']='Back';
		     $form_state['values']['form_build_id']='';
		     $form_state['values']['form_token']='';
		     $form_state['values']['form_id']='uc_cart_checkout_exp_paymethod_form';
		     $form_state['values']['op']='Continue';
		     
		     $_SESSION["cc_token"] = $_REQUEST['cc_token'];
		     $_SESSION['cc_date_exp'] = $_REQUEST['cc_date_exp'];
		      

	        $isvalid = uc_cart_checkout_exp_paymethod_form_validate($form, $form_state,$order_id,'api');

	        expDebug::dPrint('am valid  '.$isvalid);

        if($isvalid)
    	{
	    	expDebug::dPrint('am valid  ');
	        $cc_number = substr($cc_number, -4);

	       expDebug::dPrint('after valid u see= ' . print_r($_SESSION['sescrd'], true), 5);

	        $cc_data = array();
	        $cc_data["cc_number"] = $cc_number;
	        $cc_data["cc_exp_month"] = $cc_exp_month;
	        $cc_data["cc_exp_year"] = $cc_exp_year;

	        //Below session cache is hardcoded because it is used further in cybersource request.

	      $_SESSION['cart_order'] = $order_id;

	        _uc_credit_save_cc_data_to_order($cc_data,$order_id);

	        uc_credit_cache('save', $_SESSION['sescrd']);
	        expDebug::dPrint('am valid fine we are in ');
	        $result_payment = uc_cart_checkout_review_form_submit($form,$formstate,$order_id,'api');

	        expDebug::dPrint(' $rowCount = ' . print_r($result_payment, true),3);


	        if($result_payment != 'failure')
	        {
	        	$result-> status =  'success';
	        }
	        else {
	        	$result-> status =  'unprocessed';
	        }
	 		 return $result;
        }
        else
        {
	 		$result-> status =  'failure';
			 return $result;
        }
        }

        if($payment_method != "credit")
        {
	        $result_payment = uc_cart_checkout_review_form_submit($form,$formstate,$order_id,'api');

	        if($result_payment != 'failure')
	        {
			 $result-> status = 'success';
	        }
	        else {
	        	$result-> status =  'unprocessed';
	        }
	        return $result;
         }

	}catch (Exception $ex) {
		watchdog_exception('addClassToCartAPI', $ex);
		expertusErrorThrow($ex);
	}
}

/**
 * Final checks to make sure the order can be completed.
 *
 * @see uc_cart_checkout_review_form()
 */
function uc_cart_checkout_review_form_submit($form, &$form_state,$order_id='',$isAPI = '') {
  expDebug::dPrint('$_SESSION[cart_order] = ' . $_SESSION['cart_order'], 5);
  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);

  // Invoke hook_uc_order($op = 'submit') to test to make sure the order can
  // be completed... used for auto payment in uc_credit.module.


  if($isAPI != 'api'){
 		 	$order = uc_order_load($_SESSION['cart_order']);
  	}else{
  		$order = uc_order_load($order_id);
  	}
  expDebug::dPrint('$order$order = ' . print_r($order, true), 5);

  $error = FALSE;

  // Invoke it on a per-module basis instead of all at once.
  foreach (module_implements('uc_order') as $module) {
    $function = $module . '_uc_order';
    if (function_exists($function)) {
    	expDebug::dPrint('calling function12121212 ' . $function, 5);
      // $order must be passed by reference.

      $result = $function('submit', $order, NULL);
      expDebug::dPrint('$result = ' . print_r($result, true), 5);
      $msg_type = 'status';
      if ($result[0]['pass'] === FALSE) {
        $error = TRUE;
        $msg_type = 'error';
      }
      if (!empty($result[0]['message'])) {
      	expDebug::dPrint('Setting message and type = ' . $result[0]['message'] .  ' --> ' . $msg_type, 5);
        drupal_set_message($result[0]['message'], $msg_type);
      }

      expDebug::dPrint('$error = ' . ($error? 'TRUE' : 'FALSE'), 5);
      // Stop invoking the hooks if there was an error.
      if ($error) {
      	$sltOrderId  = db_query("SELECT id FROM slt_order WHERE uc_order_id = '".$order->order_id."' limit 1")->fetchField();
      	$lmsUserId = getSltpersonUserId();
      	$insertStmt = db_insert('slt_payment_log');
      	$fields = array(
      			'order_id'    => $sltOrderId,
      			'type'        => 'CC',
      			'paytime'     => now(),
      			'requestxml'  => 'not available',
      			'responsexml' => $result[0]['message'],
      			'created_by'  => $lmsUserId,
      			'created_on'  => now(),
      			'updated_by'  => $lmsUserId,
      			'updated_on'  => now()
      	);
      	$insertStmt->fields($fields);
      	// Execute the insert statement. Fetch the new org id.
      	$logId = $insertStmt->execute();
        break;
      }
    }
  }

  if ($error && $result[0]['cart_process'] != 'unprocessed') {
  	if($isAPI == 'api'){
  		return 'failure';
  	}
  	$form_state['redirect'] = 'cart/checkout/review';
  }else if($result[0]['cart_process'] == 'unprocessed'){
  	if($isAPI == 'api'){
  		return 'failure';
  	}
  	$form_state['redirect'] = 'cart/checkout/review';
  	//$form_state['redirect'] = '';  	
  }
  else {
    unset($_SESSION['do_review']);
    $_SESSION['do_complete'] = TRUE;

    if($isAPI != 'api'){
 		 	$form_state['redirect'] = 'cart/checkout/complete';
  	}
  	else
  	{
  	$_SESSION['cart_order'] = $order_id;
  	$result_uc = uc_cart_checkout_complete('api');
  	return $result_uc;
  	}

  }
}

/**
 * Completes the sale and finishes checkout.
 */
function uc_cart_checkout_complete($isAPI = '') {

expDebug::dPrint('ajayyy3333' );
  if ((!isset($_SESSION['do_complete']) || !$_SESSION['do_complete']) && !isset($_SESSION['callout_response'])) {
    drupal_goto('cart');
  }

  $order = uc_order_load(intval($_SESSION['cart_order']));

  if (empty($order)) {
    // Display messages to customers and the administrator if the order was lost.
    drupal_set_message(t("We're sorry.  An error occurred while processing your order that prevents us from completing it at this time. Please contact us and we will resolve the issue as soon as possible."), 'error');
    watchdog('uc_cart', 'An empty order made it to checkout! Cart order ID: @cart_order', array('@cart_order' => $_SESSION['cart_order']), WATCHDOG_ERROR);
    drupal_goto('cart');
  }

  $build = uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));

  // Add a comment to let sales team know this came in through the site.
  uc_order_comment_save($order->order_id, 0, t('Order created through website.'), 'admin');

  $page = variable_get('uc_cart_checkout_complete_page', '');
  if (!empty($page)) {
    drupal_goto($page);
  }

  return $build;
}

/**
 * Displays the cart checkout page with payment methods.
 */
function uc_cart_checkout_exp_paymethod() {

	drupal_add_js(drupal_get_path('module', 'uc_cart') . '/uc_cart.js');

  if (empty($_SESSION['do_exp_paymethod'])) {
    drupal_goto('cart/checkout');
  }

  $order = uc_order_load($_SESSION['cart_order']);

  if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    unset($_SESSION['cart_order']);
//     unset($_SESSION['admin_shop_cart_currency_code']);
//     unset($_SESSION['admin_shop_cart_currency_sym']);
    unset($_SESSION['shop_cart_currency_code']);
    unset($_SESSION['shop_cart_currency_sym']);
    drupal_goto('cart');
  }

  $panes = _uc_checkout_pane_list();
  expDebug::dPrint('$panes = ' . print_r($panes, true), 5);

  // If the cart isn't shippable, bypass panes with shippable == TRUE.
  if (!uc_cart_is_shippable() && variable_get('uc_cart_delivery_not_shippable', TRUE)) {
    $panes = uc_cart_filter_checkout_panes($panes, array('shippable' => TRUE));
  }
  expDebug::dPrint('$panes after removing panes with shippable set to TRUE = ' . print_r($panes, true), 5);

  foreach ($panes as $id => $pane) {
    if ($id != 'payment' && $id != 'comments' && $id != 'uc_termsofservice_agreement_checkout' && $pane['enabled']) {
      $func = $pane['callback'];
      if (function_exists($func)) {
        $return = $func('review', $order, NULL);
        if (!is_null($return)) {
          $data[$pane['title']] = array('pane_callback' => $pane['callback'], 'render_html' => $return);
        }
      }
    }
  }

  $build = array(
    '#theme' => 'uc_cart_checkout_exp_paymethod',
    '#panes' => $data,
    '#form' => drupal_get_form('uc_cart_checkout_exp_paymethod_form', $panes, $order),
  );

  return $build;
}

/**
 * The checkout form built up from the enabled checkout panes including pay methods pane.
 *
 * @see uc_cart_checkout_exp_paymethod_form_validate()
 * @see uc_cart_checkout_exp_paymethod_form_review()
 * @see uc_cart_checkout_exp_paymethod_review()
 * @see theme_uc_cart_checkout_exp_paymethod_form()
 * @ingroup forms
 */
function uc_cart_checkout_exp_paymethod_form($form, &$form_state, $panes, $order) {
	expDebug::dPrint('$panes = ' . print_r($panes, true), 5);
	expDebug::dPrint('$order = ' . print_r($order, true), 5);
  global $user;

  $form['panes'] = array('#tree' => TRUE);

  foreach ($panes as $id => $pane) {
  	expDebug::dPrint('$pane = ' . print_r($pane, true), 5);

    if (($id == 'payment' || $id == 'comments' || $id == 'uc_termsofservice_agreement_checkout') && $pane['enabled']) {
      $pane['prev'] = _uc_cart_checkout_prev_pane($panes, $id);
      $pane['next'] = _uc_cart_checkout_next_pane($panes, $id);

      if (!isset($pane['collapsed'])) {
        $collapsed = ($pane['prev'] === FALSE || empty($displayed[$pane['prev']])) ? FALSE : TRUE;
      }
      if (isset($form_state['expanded_panes']) && in_array($id, $form_state['expanded_panes'])) {
        $collapsed = FALSE;
      }

      $return = $pane['callback']('view', $order, $form, $form_state);
      expDebug::dPrint('view $return = ' . print_r($return, true), 5);

      // Add the pane if any display data is returned from the callback.
      if (is_array($return) && (!empty($return['description']) || !empty($return['contents']))) {
        // Create the fieldset for the pane.
        $form['panes'][$id] = array(
          '#type' => 'fieldset',
          '#title' => $pane['title'],
          '#description' => !empty($return['description']) ? $return['description'] : NULL,
          '#collapsible' => $pane['collapsible'],
          '#collapsed' => variable_get('uc_use_next_buttons', FALSE) ? $collapsed : FALSE,
          '#id' => $id . '-pane',
          '#theme' => isset($return['theme']) ? $return['theme'] : NULL,
        );

        // Add the contents of the fieldset if any were returned.
        if (!empty($return['contents'])) {
          $form['panes'][$id] = array_merge($form['panes'][$id], $return['contents']);
        }

        // Add the 'Next' button if necessary.
        if ((!isset($return['next-button']) || $return['next-button'] !== FALSE) && $pane['next'] !== FALSE &&
            variable_get('uc_use_next_buttons', FALSE) != FALSE) {
          $opt = variable_get('uc_collapse_current_pane', FALSE) ? $id : 'false';
          $form['panes'][$id]['next'] = array(
            '#type' => 'button',
            '#value' => t('Next'),
            '#weight' => 20,
            '#attributes' => array('onclick' => "return uc_cart_next_button_click(this, '" . $pane['next'] . "', '" . $opt . "');"),
            '#prefix' => '<div class="next-button">',
            '#suffix' => '</div>',
          );
        }

        // Log that this pane was actually displayed.
        $displayed[$id] = TRUE;
      }
    }
  }
  unset($form_state['expanded_panes']);

  $contents = uc_cart_get_contents();

  $form['cart_contents'] = array(
    '#type' => 'hidden',
    '#value' => serialize($contents),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#validate' => array(), // Disable validation to prevent a new order from being created.
    '#limit_validation_errors' => array(),
    '#submit' => array('uc_cart_checkout_exp_paymethod_form_cancel'),
  );

  $form['actions']['continue'] = array(
    '#type' => 'submit',
    '#value' => t('Review order'),
  );

  unset($_SESSION['do_review']);

  return $form;
}

/**
 * Adds markup and styling to the panes in uc_cart_checkout_exp_paymethod_form.
 *
 * @see uc_cart_checkout_exp_paymethod_form()
 * @ingroup themeable
 */
function theme_uc_cart_checkout_exp_paymethod_form($variables) {
	expDebug::dPrint('$variables = ' . print_r($variables, true), 5);
  $form = $variables['form'];

  drupal_add_css(drupal_get_path('module', 'uc_cart') . '/uc_cart.css');

  $output = '<div id="checkout-instructions">' . filter_xss_admin(variable_get('uc_checkout_instructions', '')) . '</div>';

  foreach (element_children($form['panes']) as $pane_id) {
    if (function_exists(($func = _uc_checkout_pane_data($pane_id, 'callback')))) {
      $result = $func('theme', $form['panes'][$pane_id], NULL);
      if (!empty($result)) {
        $output .= $result;
        $form['panes'][$pane_id] = array();
      }
      else {
        $output .= drupal_render($form['panes'][$pane_id]);
      }
    }
    else {
      $output .= drupal_render($form['panes'][$pane_id]);
    }
  }

  $output .= '<div id="checkout-form-bottom">' . drupal_render_children($form) . '</div>';

  return $output;
}

/**
 * Form validation for uc_cart_checkout_exp_paymethod_form().
 *
 * @see uc_cart_checkout_exp_paymethod_form()
 * @see uc_cart_checkout_exp_paymethod_form_submit()
 */
function uc_cart_checkout_exp_paymethod_form_validate($form, &$form_state,$order_id,$isAPI = '') {

  expDebug::dPrint('$_SESSION[cart_order] = ' . $_SESSION['cart_order'], 5);

  expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 5);
  expDebug::dPrint('$form_state[storage] = ' . print_r($form_state['storage'], true), 5);

  if($isAPI != 'api'){
  if (empty($form_state['storage']['order'])) {
    $order = uc_order_load($_SESSION['cart_order']);
    $form_state['storage']['order'] = $order;
  }
  else {
    $order = $form_state['storage']['order'];
  }
  }



 else{
  		$order = uc_order_load($order_id);
  	}

  expdebug::dPrint('$order = ' . print_r($order, true), 5);

  // Validate/process the cart panes.  A FALSE value results in failed checkout.
  $form_state['checkout_exp_paymethod_valid'] = TRUE;
  foreach (element_children($form_state['values']['panes']) as $pane_id) {
  	expDebug::dPrint('$pane_id = ' . print_r($pane_id, true), 5);
    $func = _uc_checkout_pane_data($pane_id, 'callback');
    expDebug::dPrint('$func = ' . print_r($func, true), 5);
    if (is_string($func) && function_exists($func)) {
      $isvalid = $func('process', $order, $form, $form_state);
      if ($isvalid === FALSE) {
      	expDebug::dPrint('$isvalid is FALSE', 5);
        $form_state['expanded_panes'][] = $pane_id;
        $form_state['checkout_exp_paymethod_valid'] = FALSE;
      }
    }
  }

  expdebug::dPrint('$order after process = ' . print_r($order, true), 5);

  usort($order->line_items, 'uc_weight_sort');
  expdebug::dPrint('$order after usort of line items = ' . print_r($order, true), 5);

  uc_order_save($order);
  expdebug::dPrint('$order after save = ' . print_r($order, true), 5);

  $form_state['storage']['order'] = uc_order_load($order->order_id, TRUE);

  expDebug::dPrint('$form_state[checkout_exp_paymethod_valid] = ' . ($form_state['checkout_exp_paymethod_valid'] === FALSE?'FALSE' : 'TRUE'), 5);

  if($isAPI == 'api'){
  return $form_state['checkout_exp_paymethod_valid'] === FALSE?FALSE : TRUE;
  }
}

/**
 * Form submission handler for uc_cart_checkout_exp_paymethod_form().
 *
 * @see uc_cart_checkout_exp_paymethod_form()
 * @see uc_cart_checkout_exp_paymethod_form_validate()
 */
function uc_cart_checkout_exp_paymethod_form_submit($form, &$form_state) {
  if ($form_state['checkout_exp_paymethod_valid'] === FALSE) {
    $url = 'cart/checkout/paymethod';
  }
  else {
  	// Check the callout featute enabled or not
  	if (module_exists('exp_sp_callout') && calloutIsActive() &&  calloutEntityIsActive('cme_pmt') && calloutEntityMethod('cme_pmt') == 'api') {
  		$callOutInfo = getCalloutEntityInfo('cme_pmt');
  		require_once DRUPAL_ROOT.$callOutInfo['file'];
  		require_once DRUPAL_ROOT. '/sites/all/modules/core/exp_sp_core/modules/exp_sp_callout/exp_sp_callout.inc';

  		$modOrder = organizeOrderDetails($form_state['storage']['order']);
  		$result = processCalloutPaymentRequest($modOrder);
  		completeCalloutPaymentResponse($result);

  	}
  	 else {
    $url = 'cart/checkout/review';
    unset($_SESSION['do_exp_paymethod']);
    $_SESSION['do_review'] = TRUE;
  }

  }

  unset($form_state['checkout_exp_paymethod_valid']);

  $form_state['redirect'] = $url;
}

/**
 * @see uc_cart_checkout_exp_paymethod_form()
 */
function uc_cart_checkout_exp_paymethod_form_cancel($form, &$form_state) {
  if (isset($_SESSION['cart_order']) && intval($_SESSION['cart_order']) > 0) {
    uc_order_comment_save($_SESSION['cart_order'], 0, t('Customer cancelled this order from the checkout paymethod form.'));
    unset($_SESSION['cart_order']);
  }

  $form_state['redirect'] = 'cart';
}

function theme_uc_cart_checkout_exp_paymethod($variables) {
  $panes = $variables['panes'];
  $form = $variables['form'];

  drupal_add_css(drupal_get_path('module', 'uc_cart') . '/uc_cart.css');
  $output = '';
  foreach ($panes as $title => $data) {
  	expDebug::dPrint('$title = ' . print_r($title, true), 4);
  	expDebug::dPrint('$data = ' . print_r($data, true), 4);
  	if (strpos($title,'Expertus') !== false) {
  	  $title =  "";
  	}
  	$pieces = explode(" ", $title);
    $pieces =  strtolower($pieces[0]);
    if($data['pane_callback'] == 'clone_uc_checkout_pane_cart') {
    	$output .= '<table class="order-review-table">';
    } else if($data['pane_callback'] == 'uc_checkout_pane_billing' && $data['pane_render'] == 'exp_sp_admin_order') {
    	$output .= '<div class="payment-user-details-wrapper"><div class="payment-billing-details-info">';
    } else if($data['pane_callback'] == 'uc_checkout_pane_billing') {
    	$output .= '<div class="payment-user-details-wrapper"><span class="cls-hide" onclick="paymentBillingHideShowBehaviors(this)"></span><div class="payment-billing-details-info">';
    } else {
    	$output .= '<div class="payment-user-details-info">';
    }
    $output .= '<table class="order-review-table">';
    if($pieces != 'expertus') {
	    $output .= '<tr class="pane-title-row"><td colspan="2">' . $title
	              . '</td></tr>';
    }
    if (is_array($data['render_html'])) {
      foreach ($data['render_html'] as $row) {
      	$title_variable = implode(',', explode("<br />", $row['data']));
        if (is_array($row)) {
          if (isset($row['border'])) {
            $border = ' class="row-border-' . $row['border'] . '"';
          }
          else {
            $border = '';
          }
          if($row['title'] == t('LBL107'))
          {
          	if($data['pane_callback'] == 'uc_checkout_pane_billing') {
          	  $uniqName = "CART-ORDER-BILLING-NAME";
          	}
          	if($data['pane_callback'] == 'expertus_checkout_pane_userorginfo'){
          	  $uniqName = "CART-ORDER-USER-NAME";
          	}
          	$output .= '<tr class="paint-payment-details-list" valign="top"' . $border . '><td class="title-col" '
          	. 'nowrap>' . $row['title'] . ':</td><td class="data-col vtip" title="' . $title_variable . '">'
          	. titleController($uniqName,$row['data'],'20') . '</td></tr>';
          }
          else
          {
          	$output .= '<tr class="paint-payment-details-list" valign="top"' . $border . '><td class="title-col" '
          	. 'nowrap>' . $row['title'] . ':</td><td class="data-col " title="' .$title_variable. '">'
          	. $row['data'] . '</td></tr>';
          }

        }
        else {
          $output .= '<tr valign="top"><td colspan="2">' . $row . '</td></tr>';
        }
      }
    }
    else {
      $output .= '<tr valign="top"><td colspan="2">' . $data['render_html'] . '</td></tr>';
    }
    if($data['pane_callback'] == 'clone_uc_checkout_pane_cart') {
    	$output .= '</table><div style="clear:both"></div>';
    } else if($data['pane_callback'] == 'uc_checkout_pane_billing') {
		$output .= '</table></div>';
    } else {
    	$output .= '</table></div><div class="clearBoth"></div></div>';
    }
  }

  /*$output .= '<tr class="review-button-row">' .
               '<td colspan="2">' . drupal_render($form) . '</td>' .
             '</tr>' .
           '</table>';*/
  if($form){
  	$output .= drupal_render($form) . '</div>';
  }

  return $output;
}



