<?php

/**
 * @file
 * Credit menu items.
 */

/**
 * Prints the contents of the CVV information popup window.
 */
function uc_credit_cvv_info() {
  global $theme_key;
  	if($theme_key == 'expertusoneV2') {
  		//NEWUI THEME STYLE SHEET
  		$output =  "<LINK href='".drupal_get_path('module', 'exp_sp_administration') ."/exp_sp_administration_v2.css' rel='stylesheet' type='text/css'>";
  	} else {
		$output =  "<LINK href='".drupal_get_path('module', 'exp_sp_administration') ."/exp_sp_administration.css' rel='stylesheet' type='text/css'>";
  	}
  // multi language UTF Support For Chinese Language 
  $output .=  '<meta http-equiv="Content-Type" content="text/html; charset=utf-8">';
  $output .= '<b>' . t('LBL1150') . '</b><p style="clear:both;">' . t('LBL1151') . '</p>';
  // ##29248 - Language based content title changed
  $cc_types = array('LBL1094' => 'visa', 'LBL1095' => 'mastercard', 'LBL1096' => 'discover');
  foreach ($cc_types as $key => $type) {
    if (variable_get('uc_credit_' . $type, TRUE)) {
      $valid_types[] = t($key);
    }
  }
  if (count($valid_types) > 0) {
    $output .= '<br /><b>' . implode(', ', $valid_types) . ':</b><p><img src="'
             . base_path() . drupal_get_path('module', 'uc_credit')
              . '/images/visa_cvv.jpg" align="left" />'
            . t('LBL1152') . '</p>';
  }

  if (variable_get('uc_credit_amex', TRUE)) {
    $output .= '<br /><p style="clear:both;"><b>' . t('LBL1097') . ':</b><p><img src="'
             . base_path() . drupal_get_path('module', 'uc_credit')
              . '/images/amex_cvv.jpg" align="left" />'
             . t('LBL1153') . '</p>';
  }

  $output .= '<p style="clear:both;"><div align="right"><div style="margin-right:10px;" class="addedit-form-cancel-container-actions"><div class="admin-save-button-left-bg white-btn-bg-left"></div><input type="button" style="cursor:pointer" class="admin-save-button-middle-bg white-btn-bg-middle" onclick="self.close();" '
            . 'value="' . t('LBL123') . '" /><div class="admin-save-button-right-bg white-btn-bg-right"></div></div></div></p>';

  print $output;
  exit();
}
