<?php
  global $theme_key;
  $cancelPoliciesList = getCancellationPolicyDetails();
  expDebug::dPrint('exp_sp_admin_com_cancellation_policy.tpl.php : $cancelPoliciesList = ' . print_r($cancelPoliciesList, true), 5);

  if (empty($cancelPoliciesList)) { //No cancel policy configured in the system
    $cancelPolicyTypesList = getCancellationPolicyEntityTypes(false);
?>
<div id='commerce-cancellation-policy-screen-wrapper' class='commerce-cancellation-policy-screen-wrapper no-cancel-policy'>
  <div id="show_expertus_message"></div>
  <div class="no-cancel-policy-add-button-div">
    <div id="add-cancel-policy" class="add-cancel-policy-popup cancel-policy-popup-wrapper"></div>
    <div class="add-cancel-policy-button-wrapper">
	    <ul class="add-cancel-policy-type-list-ul">
	      <span class="admin-save-button-left-bg"></span>
<?php
    foreach ($cancelPolicyTypesList as $key => $cancelPolicyType) {

      if (in_array($cancelPolicyType->code, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt'))) {
        $qtipWidth = 314;
      }
      else {
        $qtipWidth = 248;
      }

      $qtipObj = "{" .
                 "  'holderId': 'add-cancel-policy'" .
                 ", 'id': 'add-cancel-policy-qtip'" .
                 ", 'placement': 'above-or-below'" .
                 "}";
      
    	if ($cancelPolicyType->code == 'lrn_cls_dty_wbt') {
    	  $linkLabelAsButton = t('LBL1128') . '&nbsp;' . t('LBL1129') . '&nbsp;'; //Add Policy For, Web Based
    	  $linkLabelAsMoreItem = t('LBL1129'); //Web Based
    	}
    	else {
    		$linkLabelAsButton = t('LBL1128') . '&nbsp;' . $cancelPolicyType->name . '&nbsp;'; //Add Policy For
    		$linkLabelAsMoreItem = $cancelPolicyType->name;
    	}
      if ($key == 0) {
      	$linkLabel = $linkLabelAsButton;
      	$cssClass = 'admin-save-button-middle-bg';
      }
      else {
        $linkLabel = $linkLabelAsMoreItem;
        $cssClass = '';
      }
?>

<?php
      if ($key == 1 ) {
?>
        <span class="add-cancel-policy-more-list dropdownadd-dd-list">
          <div class="dropdownadd-dd-list-arrow"></div>
<?php
      } //end if
?>        
          <li id="add-<?php print $cancelPolicyType->code;?>-cancel-policy"
                class="<?php print $cssClass;?> add-cancel-policy-type-list-item"
                  data-link-label-as-button="<?php print $linkLabelAsButton;?>"
                    data-link-label-as-more-item="<?php print $linkLabelAsMoreItem;?>">
            <a id="add-<?php print $cancelPolicyType->code;?>-cancel-policy-link" class="addedit-form-expertusone-throbber use-ajax"
                 style="cursor:pointer; text-decoration:none; color: inherit; float:none;"
                   href="?q=/ajax/administration/cancelpolicy/addedit/<?php print $cancelPolicyType->code;?>/0"
                     data-qtip-obj="<?php print $qtipObj;?>" data-wrapperid="commerce-cancellation-policy-screen-wrapper">
              <?php print $linkLabel;?>
            </a>
            
          </li>
<?php
    } //end foreach
?>
        </span>
      </ul>
      <span id="add-cancel-policy-more-button" class="pub-unpub-add-action-wrapper pub-unbpub-more-btn "></span>
    </div>
  </div>
  <div id="no-cancel-policies-msg" class="admin-empty-text-msg">
      <?php print t('MSG667'); //Click on Add Policy to add a cancellation policy.?>
    </div>
</div>
<?php
  } //end if empty($cancel_policies_list)
  else {
?>
<div id='commerce-cancellation-policy-screen-wrapper' class='commerce-cancellation-policy-screen-wrapper  has-cancel-policies'>
  <div id="show_expertus_message"></div>
  <div id="cancel-policy-display-div" class="cancel-policy-display-div">
    <table class="cancel-policy-display-table">
      <thead>
        <tr class="cancel-policy-display-table-row cancel-policy-display-table-header-row">
          <th class="training-type">
            <?php print t('LBL037'); //Training ?>
          </th>
          <th class="deduct-percent">
            <?php print t('LBL1130'); //Percentage To Deduct ?>
          </th>
          <th colspan="2" class="deduct-days-plus-actions" >
            <?php print t('LBL1131'); //Days To Start ?>
          </th>
        </tr>
      </thead>
      <tbody>
<?php
    $odd = 1;
    $cancelPolicyTypesList = getCancellationPolicyEntityTypes(true);
    foreach ($cancelPoliciesList as $cancelPolicy) {
    	$cancelPolicyTypeDetails = $cancelPolicyTypesList[$cancelPolicy->delivery_type];
    	$cancelPolicyTypesList[$cancelPolicy->delivery_type]->rendered = true;
   	
    	$daysBefore = $cancelPolicy->days;
    	if (!in_array($cancelPolicy->delivery_type, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt'))) {
    		$daysBefore = ucfirst(t('LBL1126')); //Always
    	}
    	
    	if ($cancelPolicyTypeDetails->code == 'lrn_cls_dty_wbt') {
    	  $cancelPolicyTitle = t('LBL1129'); //Web Based
    	}
    	else {
    		$cancelPolicyTitle = $cancelPolicyTypeDetails->name;
    	}
    	
      if (in_array($cancelPolicy->delivery_type, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt'))) {
        $qtipWidth = 314;
      }
      else {
        $qtipWidth = 248;
      }
      
      $deleteURL = '?q=ajax/administration/cancelpolicy/delete/' . $cancelPolicy->id;
      
      $qtipObj = "{" .
                 "  'holderId': 'edit-cancel-policy-" . $cancelPolicy->id . "'" .
                 ", 'id': 'edit-cancel-policy-" . $cancelPolicy->id . "-qtip'" .
                 ", 'placement': 'left-or-right'" .
                 "}";
?>
        <tr class="cancel-policy-display-table-row cancel-policy-display-table-body-row<?php print ($odd == 1)? ' odd' : ' even'; ?>">
          <td class="training-type">
            <span>
              <?php print $cancelPolicyTitle; ?>
            </span>
          </td>
          <td class="deduct-percent">
            <span>
              <?php print $cancelPolicy->refund_percentage; ?>
            </span>
          </td>
          <td class="deduct-days">
            <span>
              <?php print $daysBefore; ?>
            </span>
          </td>
          <td class="actions">
            <div class="cancel-policy-actions-wrapper">
              <div id="cancel-policy-edit-wrapper-<?php print $cancelPolicy->id;?>" class="cancel-policy-edit-wrapper">
                <div id="edit-cancel-policy-<?php print $cancelPolicy->id;?>" class="edit-cancel-policy-popup cancel-policy-popup-wrapper"></div>
                <a id="edit-cancel-policy-<?php print $cancelPolicy->id;?>-link" href="?q=/ajax/administration/cancelpolicy/addedit/<?php print $cancelPolicyTypeDetails->code;?>/<?php print $cancelPolicy->id;?>"
                     style="text-decoration:none; float:none;"
                       class="addedit-form-expertusone-throbber cancel-policy-edit-action cancel-policy-action use-ajax"
                         data-qtip-obj="<?php print $qtipObj;?>" data-wrapperid="commerce-cancellation-policy-screen-wrapper">
                  <?php if($theme_key != 'expertusoneV2') {?>   
                  <span><?php print t('LBL063');//Edit?></span>
                  <?php } ?>
                </a>
              </div>
              <div class="tab-seperator"></div>
              <div id="cancel-policy-delete-wrapper-<?php print $cancelPolicy->id;?>" class="cancel-policy-delete-wrapper">
                <a href="<?php print $deleteURL;?>"
                     class="use-ajax addedit-form-expertusone-throbber cancel-policy-delete-action cancel-policy-action"
                       data-wrapperid="commerce-cancellation-policy-screen-wrapper">
                  &nbsp;
                </a>
              </div>
            </div>
          </td>
        </tr>
<?php
      $odd = 1 - $odd; //toggle odd
    }
?>
      </tbody>
    </table>
    <div class="add-another-cancel-policy-type-list">
      <ul class="add-another-cancel-policy-type-list-ul dropdown-dd-list">
<?php
    expDebug::dPrint('$cancelPolicyTypesList = ' . print_r($cancelPolicyTypesList, true), 5);
    foreach ($cancelPolicyTypesList as $key => $cancelPolicyType) {
    	expDebug::dPrint('$key = ' . print_r($key, true), 5);
    	if (!in_array($key, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt')) && $cancelPolicyType->rendered) {
    		continue; //skip
    	}
    	
    	expDebug::dPrint('Generating link for $key = ' . print_r($key, true), 5);
    	
    	if (in_array($key, array('lrn_cls_dty_vcl', 'lrn_cls_dty_ilt'))) {
    		$qtipWidth = 314;
    	}
    	else {
    		$qtipWidth = 248;
    	}
      
      $qtipObj = "{" .
                 "  'holderId': 'add-another-cancel-policy'" .
                 ", 'id': 'add-cancel-policy-qtip'" .
                 ", 'placement': 'above-or-below'" .
                 ", 'showForId': 'add-another-cancel-policy-label'" .
                 "}";

      if ($cancelPolicyType->code == 'lrn_cls_dty_wbt') {
        $displayLabelWhenQtipOpen = t('LBL1128') . '&nbsp;' . t('LBL1129') . '&nbsp;'; //Add Policy For, Web Based
        $linkLabel = t('LBL1129'); //Web Based
      }
      else {
        $displayLabelWhenQtipOpen = t('LBL1128') . '&nbsp;' . $cancelPolicyType->name . '&nbsp;'; //Add Policy For
        $linkLabel = $cancelPolicyType->name;
      }
?>
        <li id="add-another-<?php print strtolower($cancelPolicyType->attr1);?>-cancel-policy"
                class="add-another-cancel-policy-type-list-item" style="text-align:center;">
          <a id="add-another-<?php print strtolower($cancelPolicyType->attr1);?>-cancel-policy-link" class="addedit-form-expertusone-throbber use-ajax"
               style="cursor:pointer; text-decoration:none; color: inherit; float:none;"
                 href="?q=/ajax/administration/cancelpolicy/addedit/<?php print $cancelPolicyType->code;?>/0"
                   data-qtip-obj="<?php print $qtipObj;?>" data-label-when-qtip-open="<?php print $displayLabelWhenQtipOpen;?>"
                      data-wrapperid="commerce-cancellation-policy-screen-wrapper">
            <?php print $linkLabel;?>
          </a>
        </li>
<?php
    } //end foreach
?>
      </ul>
      <div class="add-another-cancel-policy-openup-arrow"></div>
    </div>
    
    <?php
      $addAnother = ($theme_key == 'expertusoneV2') ? 'add-custombtn-symbol' : ' ';   
    ?>
    <div class="cancel-policy-add-another-button-div">
      <div id="add-another-cancel-policy" class="add-another-cancel-policy-popup cancel-policy-popup-wrapper"></div>
      <span id='add-another-cancel-policy-label' class="<?php echo $addAnother; ?> add-another-cancel-policy-label"
                 data-default-label="<?php print t('LBL1133'); //Add Another Policy ?>">
        <?php print t('LBL1133'); //Add Another Policy ?>
      </span>
      <a class="add-another-cancel-policy-arrow" id="arrow-icon-cancel-policy">&nbsp;</a>
    </div>
  </div>
</div>
<?php
  }
?>