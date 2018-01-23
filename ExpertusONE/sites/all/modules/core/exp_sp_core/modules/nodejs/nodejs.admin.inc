<?php
/**
 * @file
 * Admin page callbacks for the Node.js module.
 */

/**
 * Form builder function for module settings.
 */
function nodejs_settings() {
  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node.js server'),
  );
  $form['server']['nodejs_server_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Protocol used by Node.js server'),
    '#default_value' => variable_get('nodejs_server_scheme', 'http'),
    '#options' => array('http' => t('http'), 'https' => t('https')),
    '#description' => t('The protocol used to communicate with the Node.js server.'),
  );
  $form['server']['nodejs_server_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Node.js server host'),
    '#default_value' => variable_get('nodejs_server_host', 'localhost'),
    '#size' => 40,
    '#required' => TRUE,
    '#description' => t('The hostname of the Node.js server.'),
  );
  $form['server']['nodejs_server_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Node.js server port'),
    '#default_value' => variable_get('nodejs_server_port', '8080'),
    '#size' => 10,
    '#required' => TRUE,
    '#description' => t('The port of the Node.js server.'),
  );

  $form['server']['nodejs_service_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Service Key',
    '#description' => 'An arbitrary string used as a secret between the node.js server and the Drupal site.',
    '#default_value' => variable_get('nodejs_service_key', ''),
  );
  
  $form['server']['nodejs_flash_policy_port'] = array(
    '#type' => 'textfield',
    '#title' => 'Flash Policy Server Port',
    '#required' => TRUE,
    '#description' => 'TCP port of the flash policy server. ' .
                      'Flash policy server is auto-started by node server to listen at this port. ' .
                      'Socket.IO client will check this port to see if flashsocket connections are allowed.',
    '#default_value' => variable_get('nodejs_flash_policy_port', 10843),
  );
  
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard.
                    Example paths are %blog for the blog page and %blog-wildcard for every personal blog.
                    %front is the front page.",
                    array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));
  $form['server']['nodejs_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages on which to enable nodejs'),
    '#default_value' => variable_get('nodejs_pages', '*'),
    '#required' => TRUE,
    '#description' => $description,
  );
  
  // EXPERTUS added fieldset reconnect_settings and fields in the fieldset
  $form['reconnect_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reconnect settings'),
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE
  );
  
	$reconnectAttemptOptions = drupal_map_assoc(array(0, 1, 2, 5, 10, 15, 20, 25, 30, 40, 50, 75, 100, 150, 200));
  $reconnectAttemptOptions[0] = t('don\'t reconnect');
  $form['reconnect_settings']['nodejs_exp_reconnect_attempts'] = array(
    '#type' => 'select',
    '#title' => t('Reconnect attempts'),
    '#default_value' => variable_get('nodejs_exp_reconnect_attempts', 20),
    '#options' => $reconnectAttemptOptions,
    '#description' => t('On connection failure, number of reconnect attempts to be made.')
  );
  
  $form['reconnect_settings']['nodejs_exp_min_initial_reconnect_delay'] = array(
    '#type' => 'select',
    '#title' => t('Minimum initial reconnect delay'),
    '#default_value' => variable_get('nodejs_exp_min_initial_reconnect_delay', 500),
    '#options' => getNodejsDelayOptions(500, 5000) + getNodejsDelayOptions(10000, 30000, 5000),
    '#description' => t('On connection failure, first reconnect attempt is delayed by atleast this much time.')
  );
  
  $form['reconnect_settings']['nodejs_exp_max_initial_reconnect_delay'] = array(
    '#type' => 'select',
    '#title' => t('Maximum initial reconnect delay'),
    '#default_value' => variable_get('nodejs_exp_max_initial_reconnect_delay', 5000),
    '#options' => getNodejsDelayOptions(1000, 10000, 1000) + getNodejsDelayOptions(15000, 30000, 5000) + getNodejsDelayOptions(40000, 60000, 10000),
    '#description' => t('On connection failure, first reconnect attempt is not delayed beyond this much time.<br>' .
                        'A random delay within minimum and maximum initial reconnect delays is used as the first reconnect delay.')
  );

  $form['reconnect_settings']['nodejs_exp_max_reconnect_delay'] = array(
    '#type' => 'select',
    '#title' => t('Maximum reconnect delay'),
    '#default_value' => variable_get('nodejs_exp_max_reconnect_delay', 120000),
    '#options' => getNodejsDelayOptions(30000, 300000, 30000),
    '#description' => t('On connection failure, reconnect attempt is not to be delayed beyond this time. This excludes page unload delay.<br>' .
                        'When the first reconnect attempt fails, subsequent reconnect attempts are delayed by 2 times the previous delay. ' .
                        'If the new reconnect delay exceeds maximum reconnect delay, a new random delay within minimum and maximum initial reconnect delays is used as the reconnect delay.')
  );
  
  $form['reconnect_settings']['nodejs_exp_page_unload_delay'] = array(
    '#type' => 'select',
    '#title' => t('Page unload delay'),
    '#default_value' => variable_get('nodejs_exp_page_unload_delay', 10000),
    '#options' => getNodejsDelayOptions(5000, 15000, 1000) + getNodejsDelayOptions(20000, 30000, 5000) + getNodejsDelayOptions(40000, 60000, 10000),
    '#description' => t('The worst case time a page takes to unload. This setting is used to further delay the first reconnect attempt so that reconnect is not attempted when a page is being removed or refreshed.')
  );
  
  $form['reconnect_settings']['nodejs_exp_auth_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Authetication timeout'),
    '#default_value' => variable_get('nodejs_exp_auth_timeout', 10000),
    '#options' => getNodejsDelayOptions(5000, 15000, 1000) + getNodejsDelayOptions(20000, 30000, 5000) + getNodejsDelayOptions(40000, 60000, 10000),
    '#description' => t('The worst case time a client should expect the response (granted or denied) for an authorization request sent to Drupal backend via nodejs server. Otherwise, the socket.io connection to the nodejs server is closed and a reconnect attempt is made by the client.')
  );
  
  return system_settings_form($form);
}

/*
 * nodejs_settings_validate() -  EXPERTUS added function - Validates Expertus added settings
 */
function nodejs_settings_validate($form, &$form_state) {
	expDebug::dPrint('$form_state[values] = ' . print_r($form_state['values'], true), 4);
	if ($form_state['values']['nodejs_exp_max_initial_reconnect_delay'] <=
	                              $form_state['values']['nodejs_exp_min_initial_reconnect_delay']) {
	  form_set_error('nodejs_exp_max_initial_reconnect_delay',
	                     t('Maximum initial reconnect delay cannot be less than or equal to minimum initial reconnect delay.'));
	}
	
	if ($form_state['values']['nodejs_exp_max_reconnect_delay'] <
	                              $form_state['values']['nodejs_exp_max_initial_reconnect_delay']) {
    form_set_error('nodejs_exp_max_reconnect_delay', t('Maximum reconnect delay cannot be less than maximum initial reconnect delay.'));
	}
}

/*
 * getNodejsDelayOptions() - EXPERTUS added function - Prepare delay options for various reconnect delays
 */
function getNodejsDelayOptions($lower, $upper, $increment = 500) {
	$options = array();
	while ($lower <= $upper) {
		$options[] = $lower;
		$lower += $increment;
	}
	
	return drupal_map_assoc($options, 'format_interval_millisec');
}

/*
 * format_interval_millisec() - EXPERTUS added function - Format a time interval specified in milliseconds
 */

function format_interval_millisec($timestamp) {
  $output = '';
  
  if ($timestamp >= 60000) {
  	$output .= format_plural(floor($timestamp / 60000), '1 min', '@count min', array());
  	$timestamp %= 60000;
  }
  
  if ($timestamp > 0) {
    //$output .= ($output? ' ' : '') . number_format($timestamp / 1000, 1) . ' ' . t('sec');
    $output .= ($output? ' ' : '') . round($timestamp / 1000, 1) . ' ' . t('sec');
  }

  return $output ? $output : t('0 sec', array(), array('langcode' => $langcode));
}
