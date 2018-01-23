<?php

/**
 * @file
 * Describe hooks provided by the exp_sp_session_timeout module.
 */

/**
 * Prevent exp_sp_session_timeout logging a user out.
 *
 * This allows other modules to indicate that a page should not be included
 * in the exp_sp_session_timeout checks. This works in the same way as not ticking the
 * enforce on admin pages option for exp_sp_session_timeout which stops a user being logged
 * out of admin pages.
 *
 * @return bool
 *   Return TRUE if you do not want the user to be logged out.
 *   Return FALSE (or nothing) if you want to leave the exp_sp_session_timeout
 *   process alone.
 */
function hook_exp_sp_session_timeout_prevent() {
  // Don't include exp_sp_session_timeout JS checks on ajax callbacks.
  if (in_array(arg(0), array('ajax', 'exp_sp_session_timeout_ahah_logout', 'exp_sp_session_timeout_ahah_set_last'))) {
    return TRUE;
  }
}

/**
 * Keep a login alive whilst the user is on a particular page.
 *
 * @return bool
 *   By returning TRUE from this function the JS which talks to exp_sp_session_timeout
 *   module is included in the current page request and peridoically dials
 *   back to the server to keep the login alive.
 *   Return FALSE (or nothing) to just use the standard behaviour.
 */
function hook_exp_sp_session_timeout_refresh_only() {
  // Check to see if an open admin page will keep
  // login alive.
  if (arg(0) == 'admin' && !variable_get('exp_sp_session_timeout_enforce_admin', FALSE)) {
    return TRUE;
  }
}

/**
 * Let others act when session is extended.
 *
 * Use case: Some applications might be embedding the some other
 * applications via iframe which also requires to extend its sessions.
 */
 function hook_auto_logout_session_reset($user) {
   $myOtherIframeApplication->resetSession($user->uid);
 }

/**
 * Let other modules modify the timeout value.
 */
function hook_exp_sp_session_timeout_timeout_alter(&$timeout) {
  $timeout = 1800;
}
