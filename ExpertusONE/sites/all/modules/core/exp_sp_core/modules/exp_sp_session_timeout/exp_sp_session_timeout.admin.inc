<?php

/**
 * @file
 * Contains all admin pages, settings, and validate.
 */

/**
 * Settings form for menu callback
 */
function exp_sp_session_timeout_settings() {
  $form = array();
  $timeout = variable_get('exp_sp_session_timeout_timeout', 1800);

  if (module_exists('jstimer')) {
    if (!module_exists('jst_timer')) {
      drupal_set_message(t('The "Widget: timer" module must also be enabled for the dynamic countdown to work in the automated logout block.'), 'error');
    }

    if (variable_get('jstimer_js_load_option', 0) != 1) {
      drupal_set_message(t("The Javascript timer module's 'Javascript load options' setting should be set to 'Every page' for the dynamic countdown to work in the automated logout block."), 'error');
    }
  }

  $form['exp_sp_session_timeout_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout value in seconds'),
    '#default_value' => $timeout,
    '#size' => 8,
    '#weight' => -10,
    '#description' => t('The length of inactivity time, in seconds, before automated log out.  Must be 60 seconds or greater. Will not be used if role timeout is activated.'),
  );

  $form['exp_sp_session_timeout_max_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Max timeout setting'),
    '#default_value' => variable_get('exp_sp_session_timeout_max_timeout', '172800'),
    '#size' => 10,
    '#maxlength' => 12,
    '#weight' => -8,
    '#description' => t('The maximum logout threshold time that can be set by users who have the permission to set user level timeouts.'),
  );

  $form['exp_sp_session_timeout_padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout padding'),
    '#default_value' => variable_get('exp_sp_session_timeout_padding', 10),
    '#size' => 8,
    '#weight' => -6,
    '#description' => t('How many seconds to give a user to respond to the logout dialog before ending their session.'),
  );

  $form['exp_sp_session_timeout_role_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Role Timeout'),
    '#default_value' => variable_get('exp_sp_session_timeout_role_logout', FALSE),
    '#weight' => -4,
    '#description' => t('Enable each role to have its own timeout threshold, a refresh maybe required for changes to take effect. Any role not ticked will use the default timeout value. Any role can have a value of 0 which means that they will never be logged out.'),
  );

  $form['exp_sp_session_timeout_redirect_url']  = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URL at logout'),
    '#default_value' => variable_get('exp_sp_session_timeout_redirect_url', 'user/login'),
    '#size' => 40,
    '#description' => t('Send users to this internal page when they are logged out.'),
  );

  $form['exp_sp_session_timeout_no_dialog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not display the logout dialog'),
    '#default_value' => variable_get('exp_sp_session_timeout_no_dialog', FALSE),
    '#description' => t('Enable this if you want users to logout right away and skip displaying the logout dialog.'),
  );

  $form['exp_sp_session_timeout_use_alt_logout_method'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use alternate logout method'),
    '#default_value' => variable_get('exp_sp_session_timeout_use_alt_logout_method', FALSE),
    '#description' => t('Normally when auto logout is triggered, it is done via an AJAX service call. Sites that use an SSO provider, such as CAS, are likely to see this request fail with the error "Origin is not allowed by Access-Control-Allow-Origin". The alternate approach is to have the auto logout trigger a page redirect to initiate the logout process instead.'),
  ); 

  $form['exp_sp_session_timeout_message']  = array(
    '#type' => 'textarea',
    '#title' => t('Message to display in the logout dialog'),
    '#default_value' => variable_get('exp_sp_session_timeout_message', 'Your session is about to expire. Do you want to reset it?'),
    '#size' => 40,
    '#description' => t('This message must be plain text as it might appear in a JavaScript confirm dialog.'),
  );

  $form['exp_sp_session_timeout_inactivity_message']  = array(
    '#type' => 'textarea',
    '#title' => t('Message to display to the user after they are logged out.'),
    '#default_value' => variable_get('exp_sp_session_timeout_inactivity_message', 'You have been logged out due to inactivity.'),
    '#size' => 40,
    '#description' => t('This message is displayed after the user was logged out due to inactivity. You can leave this blank to show no message to the user.'),
  );

  $form['exp_sp_session_timeout_use_watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable watchdog Automated Logout logging'),
    '#default_value' => variable_get('exp_sp_session_timeout_use_watchdog', ''),
    '#description' => t('Enable logging of automatically logged out users'),
  );

  $form['exp_sp_session_timeout_enforce_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enforce auto logout on admin pages'),
    '#default_value' => variable_get('exp_sp_session_timeout_enforce_admin', FALSE),
    '#description' => t('If checked, then users will be automatically logged out when administering the site.'),
  );

  if (module_exists('jstimer') && module_exists('jst_timer')) {
    $form['exp_sp_session_timeout_jstimer_format']  = array(
      '#type' => 'textfield',
      '#title' => t('Autologout block time format'),
      '#default_value' => variable_get('exp_sp_session_timeout_jstimer_format', '%hours%:%mins%:%secs%'),
      '#description' => t('Change the display of the dynamic timer.  Available replacement values are: %day%, %month%, %year%, %dow%, %moy%, %years%, %ydays%, %days%, %hours%, %mins%, and %secs%.'),
    );
  }

  $form['table'] = array(
    '#type' => 'item',
    '#weight' => -2,
    '#title' => t('If Enabled every user in role will be logged out based on that roles timeout, unless the user has an individual timeout set.'),
    '#theme' => 'exp_sp_session_timeout_render_table',
  );

  foreach (user_roles(TRUE) as $key => $role) {

    $form['table']['exp_sp_session_timeout_roles']['exp_sp_session_timeout_role_' . $key . '_timeout'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('exp_sp_session_timeout_role_' . $key . '_timeout', $timeout),
      '#size' => 8,
      '#theme' => 'textfield',
    );

  }

  foreach (user_roles(TRUE) as $key => $role) {
    $form['table']['exp_sp_session_timeout_roles']['exp_sp_session_timeout_role_' . $key] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('exp_sp_session_timeout_role_' . $key, FALSE),
      '#theme' => 'checkbox',
    );
  }

  return system_settings_form($form);
}

/**
 * Settings validation.
 */
function exp_sp_session_timeout_settings_validate($form, &$form_state) {
  $max_timeout = $form_state['values']['exp_sp_session_timeout_max_timeout'];

  // Validate timeouts for each role.
  foreach (user_roles(TRUE) as $key => $role) {
    if (empty($form_state['values']['exp_sp_session_timeout_role_' . $key])) {
      // Don't validate role timeouts for non enabled roles.
      continue;
    }

    $timeout = $form_state['values']['exp_sp_session_timeout_role_' . $key . '_timeout'];
    $validate = exp_sp_session_timeout_timeout_validate($timeout, $max_timeout);

    if (!$validate) {
      form_set_error('exp_sp_session_timeout_role_' . $key . '_timeout', t('%role role timeout must be an integer greater than 60, less then %max or 0 to disable exp_sp_session_timeout for that role.', array('%role' => $role, '%max' => $max_timeout)));
    }
  }

  $timeout = $form_state['values']['exp_sp_session_timeout_timeout'];

  // Validate timeout.
  if (!is_numeric($timeout) || ((int) $timeout != $timeout) || $timeout < 60 || $timeout > $max_timeout) {
    form_set_error('exp_sp_session_timeout_timeout', t('The timeout must be an integer greater than 60 and less then %max.', array('%max' => $max_timeout)));
  }
}
