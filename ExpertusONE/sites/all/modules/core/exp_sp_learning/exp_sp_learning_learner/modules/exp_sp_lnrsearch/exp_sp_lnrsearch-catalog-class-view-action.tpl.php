<?php
global $theme_key;
require_once(drupal_get_path('module', 'exp_sp_lnrenrollment').'/exp_sp_launch.inc');
$passData = "data={'NodeId':'".$results->node_id."','ClassId':'".$results->cls_id."','CourseId':'".$results->crs_id."'}";
$seatDisplay = false;
$registerEndDateCheck = registerEndDateCheck($results->crs_id,$results->cls_id);
$entityColor = ($theme_key == "expertusoneV2") ? 'learning-type-'.array_pop(explode('_',$results->delivery_type_code)).' currency-override-bold' : '' ;
//if($results->object_type =='Class') {
  global $catalog_reg;
  if($catalog_reg == 'Course') {
	$getRegister  = getRegisteredOrNotForCourseLevel($results->crs_id, $results->cls_id, $userId, '', $results->is_compliance);
	$getRegister = $getRegister[0];
  }
  else {
	$getRegister = getRegisteredOrNot($results->crs_id,$results->cls_id,$userId,'',$results->is_compliance);
  }
  $results->enrolled_id = $getRegister->enrolled_id;
  $results->comp_status = $getRegister->comp_status;
  $results->enrolled_status     = $getRegister->enrolled_status;
  $results->waitlist_flag       = $getRegister->waitlist_flag;
  $results->waitlist_priority   = $getRegister->waitlist_priority;
  $results->is_cart_added       = isCartAdded($results->node_id);
  $results->waitlist_seats  = getWaitlistCatalogInfo($results->cls_id);
  $pastDateCount                 = checkPastDateOrNot($results->cls_id,$results->delivery_type_code);
  $waitlist   = 0;
  $waitlist_register = 0;
  $waitlisted = (($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl') ) ? 1 : 0;
  if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && (($results->available_seats == 0) || ($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl'))) {
    $waitlist_register  = $results->waitlist_seats;
    $waitlist           = 1;
  }
  if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 0 ) {
    $regMsg       = (($results->comp_status == 'lrn_crs_cmp_cmp') ? t('Completed') : ((($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl')) ? t('LBL190') : t('Registered')));//." <br>".$results->enrolled_status;
    $waitlist_register  = ($regMsg !== t('LBL190')) ? 0 : $waitlist_register;
    $seatDisplay        = ($regMsg == t('LBL190')) ? true : false;
  }
  $showContentAttempts		  = false;
  expDebug::dPrint('class $results '.print_r($results, 1));
  if ($results->enrolled_id && ($results->delivery_type_code == 'lrn_cls_dty_wbt' || $results->delivery_type_code == 'lrn_cls_dty_vod' || $results->delivery_type_code =='lrn_cls_dty_wbt,lrn_cls_dty_vod')) {
  	$results->launch_info 		  = getLaunchDetail($results->enrolled_id);
  	$single_content_class 	  = (count($results->launch_info) == 1 ) ? 1 : 0;
  	if ($single_content_class) {
  		//$showContentAttempts		  = true;
  		$IsLaunchable	= $results->launch_info['0']['IsLaunchable'];
  		$attemptLeft 	= $results->launch_info['0']['AttemptLeft'];
//   		$validDays 		= $results->launch_info['0']['ValidityDays'] - $results->launch_info['0']['remDays'];
  		$validDays		= $results->launch_info['0']['daysLeft'];
  		$validDayString = ($validDays == 1) ? t('LBL910') : t('LBL605');
  		$crsClassId		= 	$results->crs_id . '_' .  $results->cls_id;
  	}
  }
//}
  expDebug::dPrint('results->delivery_type_code-----> class action'.print_r($results,true),4);
  expDebug::dPrint('ayyyappan sq'.print_r($results,true),4);
if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && ($results->multi_register == 0 || empty($results->multi_register)) ) { ?>
              <div class="top-record-div">
                  <table class="search-register-btn change-class-btn" border="0">
                  	<tr>
                  		<td
                  			<?php
                  			
													if(($results->delivery_type_code == 'lrn_cls_dty_wbt' || $results->delivery_type_code == 'lrn_cls_dty_vod') &&  ($results->comp_status =='lrn_crs_cmp_enr'|| $results->comp_status =='lrn_crs_cmp_inp')){
														$res = registeredToLaunch($userId,$results->cls_id,$results->crs_id,$results->enrolled_id,$results->delivery_type_code,'',$regMsg);
														expDebug::dPrint('OVERE ALL RESULTs---->'.print_r($res,true),4);
														print $res[0];
														$showContentAttempts = $res[1];
													}
													else{
														?>
														class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
														<?php
														print t($regMsg);
													}
											?>
                  		</td>
                  	</tr>
                  </table>
              </div>
              <?php 
              	if($results->delivery_type_code == 'lrn_cls_dty_vod') {
              		$progress = '';
              		$progress_percent = 0;
	              	foreach($results->launch_info as $launch){
	              		$suspend_data = isset($launch['SuspendData']) ? json_decode(rawurldecode($launch['SuspendData'])) : null;
	              		$progress = 0;
	              		if($suspend_data != null) {
	              			$progress = $suspend_data->progress;
	              		}
	              		$progress_percent += floatval($progress);
	              	}
	              	$progress_percent = $progress_percent / count($results->launch_info);
	              	$progress_percent_string = round($progress_percent) . '% ' . t('Completed');
              	}
              ?>
              <?php if($showContentAttempts && $results->comp_status !='lrn_crs_cmp_cmp' &&  !contentPlayerIsActive()){ ?>
				<div class="course-attempt-left"><span id="attempt_left_<?php print $crsClassId; ?>" class="attempts_left"> <?php if ($attemptLeft != 'notset' && !empty($attemptLeft)) {  print $attemptLeft . ' ' . t('LBL202'); } ?></span></div>
				<div class="course-content-validity">
					<span id="content_concat_str_<?php print $crsClassId; ?>" class="content_concat_string"><?php if ($attemptLeft != 'notset' && !empty($validDays)) { print t('LBL647'); }?></span>
					<span id="content_validity_<?php print $crsClassId; ?>" class="content_validity"><?php if ($validDays > 0) { print $validDays . ' ' .$validDayString . ' ' .t('LBL604'); }?> </span>
					<?php if (($results->delivery_type_code == 'lrn_cls_dty_vod') && (count($results->launch_info) == 1)) { ?>
						<span id="content_concat_str_<?php print $crsClassId; ?>" class="content_concat_string"><?php print t('LBL647'); ?></span>
						<span id="content_progress_<?php print $crsClassId; ?>" class="content_validity"><?php print $progress_percent_string; ?></span> 
					<?php } ?>
				</div>
		<?php } ?>
<?php } else if(((($results->object_type =='Class' && $results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")) || ($results->object_type != 'Class')) && ($results->available_seats == 0 && $results->waitlist_seats  == 0)) { ?>
          <div class="top-record-div">
            <table class="search-register-btn change-class-btn" border="0"><tr>
            		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
                  		<?php print t('LBL046'); ?>
            		</td>
            	</tr>
            </table>
          </div>

<?php }else if($registerEndDateCheck <> 0) { ?>
          <div class="top-record-div">
            <table class="search-register-btn change-class-btn" border="0">
            	<tr>
            		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
                  		<?php print t('LBL047'); ?>
            		</td>
            	</tr>
            </table>
          </div>
<?php }else if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && $pastDateCount > 0) { ?>
		<div class="top-record-div">
            <table class="search-register-btn change-class-btn" border="0">
            	<tr>
            		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
                  		<?php print t('LBL105'); ?>
            		</td>
            	</tr>
            </table>
          </div>

	<?php }else {
              $seatDisplay = true;
?>
     <div class="top-record-div">
        <table class="search-register-btn change-class-btn" border="0">
           <tr>
        <?php  if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce))  { ?>
		  <?php if($results->is_cart_added){  ?>
			<td class="action-btn-disable" id="addToCartList_<?php print $results->cls_id; ?>">
               <?php print t('LBL049');  ?>
			</td>
		<?php } else if(($results->price != '0.00') || ($results->price == '0.00' && $results->export_compliance == 'Y')){  ?>
		  <td data="<?php print $passData;?>" class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
			    id="addToCartList_<?php print $results->cls_id; ?>"
					<?php if($userId!="" && $userId>0){ ?>
						onclick="$('body').data('learningcore').callAddToCart('<?php print $results->widgetId; ?>','1','<?php print $results->cls_id; ?>','','',<?php print $waitlist;  ?>,'<?php print 'fromClasslvl'; ?>');"
							<?php }else{ ?>
								onclick="$('body').data('learningcore').callSigninWidget(this.id);"
					<?php }?>>
                        <?php print ($waitlist_register > 0) ? t("Waitlist") : t('LBL050'); ?>
                        <?php // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ // ?>
                        <?php print (!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && $results->price > 0) ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>
			</td>
		<?php } else if(($results->price == '0.00' && $results->export_compliance == 'N')  || ($waitlist_register > 0)){ ?>
		  <td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
			id="registerCls_<?php print $results->cls_id; ?>"
			  <?php if($userId!="" && $userId>0){ ?>
				onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $results->crs_id; ?>','<?php print $results->cls_id; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>','<?php print 'fromClasslvl'?>');"
				  <?php }else{ ?>
					onclick="$('body').data('learningcore').callSigninWidget(this.id);"
				  <?php }?>>
                    <?php print ($waitlist_register > 0) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf') ? t('Registered') : t('Register'));  ?>
					<?php print '';// 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ //print !empty($_SESSION['availableFunctionalities']->exp_sp_commerce) ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>
          </td>
        <?php	} ?>
	<?php } else {  ?>
          <td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>"
			id="registerCls_<?php print $results->cls_id; ?>"
			  <?php if($userId!="" && $userId>0){ ?>
				onclick="$('body').data('learningcore').callEquivalencePopup('<?php print $results->widgetId; ?>','<?php print $userId; ?>','<?php print $results->crs_id; ?>','<?php print $results->cls_id; ?>','<?php print $waitlist; ?>','<?php print $results->enrolled_status; ?>','<?php print 'fromClasslvl'; ?>');"
			  <?php }else{ ?>
				onclick="$('body').data('learningcore').callSigninWidget(this.id);"
			  <?php }?>>
                  <?php  print ($waitlist_register > 0 ) ? t("Waitlist") : (($results->enrolled_status == 'lrn_crs_reg_cnf') ? t('Registered') : t('Register')); ?>
		  </td>
    <?php } ?>
                	</tr>
                </table>
              </div>

          <?php
          }
          if( $results->object_type =='Class' && $seatDisplay == true ) {
               if($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl"){
            	    if($waitlisted == 1) {
                      $seatInfo = t('LBL125').' '.getWaitlistPosition($results->cls_id,$results->crs_id,$userId);
            	    } else if($waitlist_register > 0) {
            	      //$seatInfo = $waitlist_register." ".t('Waitlist Seats left');
            	      $seatInfo = (($waitlist_register == 1) ? ($waitlist_register." ".t('LBL126')) : ($waitlist_register." ".t('LBL127')));
            	    } else {
            	      $seatInfo = $results->available_seats;
            	      $seatInfo = (($results->available_seats == 1) ? ($results->available_seats." ".t('LBL052')) : ($results->available_seats." ".t('LBL053')));
            	    }
                 ?>
                <div class="course-available-seats changecls-available-seats">
                	<span id="myteam-catalog-seats-available-<?php print $results->cls_id; ?>">
                      <?php
				       print $seatInfo;
			          ?>
			    	</span>
                </div>
          <?php }
          }
?>