<?php

/*
 * render_mobile_app_in_ctools_modal() - Callback to load the mobile application HTML in ctools modal dialog.
 *                                      
 */
function render_mobileapp_ctools_modal($type) {
	try{
  expDebug::dPrint(' render_mobileapp_ctools_modal = ' . print_r($type, true) , 4 );
  if ($type) {
    exp_ctools_modal_display_mobile($type);
  }
  else {
    throw new Exception('exp_sp_mobile_app.inc: render_mobileapp_ctools_modal() : Android or Iphone');
  }
  }catch (Exception $ex) {
  	watchdog_exception('render_mobileapp_ctools_modal', $ex);
  	expertusErrorThrow($ex);
  }
}


/*
 * exp_ctools_modal_display_video() - Prepares the video HTML and renders it.
 *                                    Shared by exp_sp_lnrenrollment and exp_sp_lnrlearningplan.
 *                                    Called by render_videoHTML_in_ctools_modal() defined above.
 */
function exp_ctools_modal_display_mobile($type) {
	global $user;  
	try{
  // Enable CTool related library functions.
  if($user->uid) {
  	enableCtool();
  }
  $output = theme('mobile_app_popup', array('type' => $type));
  
  $commands = array();
  // Multi language Support
  if($type =='android'){
  	$type = t('Android');
  }else if($type =='iphone'){
  	$type = t('iPhone / iPad');
  }
  // If the form has not yet been rendered, render it.
  $commands[] = ctools_modal_command_display($type, $output);
  //$commands[] = ctools_modal_show_mobile_loader();
  expDebug::dPrint(' $commands = ' . print_r($commands, true) , 4);
  
  print ajax_render($commands);
  exit; 
  }catch (Exception $ex) {
  	watchdog_exception('exp_ctools_modal_display_mobile', $ex);
  	expertusErrorThrow($ex);
  }
}

/*
 * ctools_modal_show_loader() - Creates a loader in ctools modal.
 */
function ctools_modal_show_mobile_loader() {
	try{
  return array (
    'command' => 'ctoolsModalCreateLoader',
  );
  }catch (Exception $ex) {
  	watchdog_exception('ctools_modal_show_mobile_loader', $ex);
  	expertusErrorThrow($ex);
  }
}

/**
 * 
 * @param Application type $type
 * @param Send Email-id $email
 */
function sendMobileAppLink($type,$email){
	try{
  global $user;
  if($user->uid){
    $userId = getUserIdFromDrupalUid($user->uid);
  }else{
    $userId = '';
  }
  if (!validateEmail($email)) {
    $notifyId = 0;
  }
  else{
    $notifyId = insertNotification('sendMobileAppLink', $type, $email, $userId, '');
  }
  $mailReturn  = array(
    'notify_id'      => $notifyId         
  );                 
  echo drupal_json_output($mailReturn);
  }catch (Exception $ex) {
  	watchdog_exception('sendMobileAppLink', $ex);
  	expertusErrorThrow($ex);
  }
}


?>