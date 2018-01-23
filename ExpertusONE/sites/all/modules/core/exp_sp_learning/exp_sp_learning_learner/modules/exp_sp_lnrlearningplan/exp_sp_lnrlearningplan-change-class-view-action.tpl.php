<?php
global $theme_key;

$passData = "data={'NodeId':'".$results->node_id."','ClassId':'".$results->cls_id."','CourseId':'".$results->crs_id."'}";
$seatDisplay = false;
$registerEndDateCheck = registerEndDateCheck($results->crs_id,$results->cls_id);
//36658: Unable to choose another class using "Change Class" function.
$registerClassPrice = getClassPrice(arg(4));
if($results->object_type =='Class') {
  $getRegister = getRegisteredOrNot($results->crs_id,$results->cls_id,$userId);
  $results->enrolled_id = $getRegister->enrolled_id;
  $results->comp_status = $getRegister->comp_status;
  $results->enrolled_status     = $getRegister->enrolled_status;
  $results->waitlist_flag       = $getRegister->waitlist_flag;
  $results->waitlist_priority   = $getRegister->waitlist_priority;
  $results->is_cart_added       = isCartAdded($results->node_id);
  
  $waived_status 				=$result->waived_status;

  $results->waitlist_seats  = getWaitlistCatalogInfo($results->cls_id);
  $waitlist   = 0;
  $waitlisted = (($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl') ) ? 1 : 0;
  if(($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl") && (($results->available_seats == 0) || ($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl'))) {
    $waitlist_register  = $results->waitlist_seats;
    $waitlist           = 1;
  } else {
    $waitlist_register = 0;
    $waitlist          = 0;
  }
  if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 0 ) {
    $regMsg       = (($results->comp_status == 'lrn_crs_cmp_cmp') ? t('Completed') : ((($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl')) ? t('LBL190') : t('Registered')));//." <br>".$results->enrolled_status;
    $waitlist_register  = ($regMsg !== 'Waitlisted') ? 0 : $waitlist_register;
    $seatDisplay        = ($regMsg == 'Waitlisted') ? true : false;
  }
}
if($myEnrollChangeCls == 'Enroll' || $myEnrollChangeCls == 'compliance'){
  $entityColor = ($theme_key == "expertusoneV2") ? 'learning-type-'.array_pop(explode('_',$results->delivery_type_code)).' currency-override-bold' : '' ;
  $actionClass = 'action-course-btn';
  $actionDisableClass = 'action-course-btn-disable';
  $actionWaitlistClass = 'action-course-btn-waitlist';
}else {
  $actionClass = 'action-btn';
  $actionDisableClass = 'action-course-btn-disable';
  $actionWaitlistClass = 'action-btn-waitlist';
}
if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 0 ) { ?>
              <div class="top-record-div">
                  <table class="search-register-btn change-class-btn" border="0">
                  	<tr>
                  		<td class="<?php print $actionDisableClass;?>"
                  			id="assignClass_<?php print $results->cls_id; ?>">
                  	  <?php if($theme_key == 'expertusoneV2') {?>
                  			<!-- <div class="admin-save-button-left-bg"></div>
                  			<div class="admin-save-button-middle-bg"> --> <?php print t($regMsg); ?><!--   </div>
                  			<div class="admin-save-button-right-bg"></div> -->
                  		<?php }else{?>
                  		<?php print t($regMsg); ?>
                  		<?php }?>
                  		</td>
                  	</tr>
                  </table>
              </div>
<?php } else if(((($results->object_type =='Class' && $results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")) || ($results->object_type != 'Class')) && ($results->available_seats == 0 && $results->waitlist_seats  == 0)) { ?>
          <div class="top-record-div">
            <table class="search-register-btn change-class-btn" border="0"><tr>
            		<td class="<?php print $actionDisableClass;?>" id="assignClass_<?php print $results->cls_id; ?>">
            		 <?php if($theme_key == 'expertusoneV2') {?>
                  			<!-- <div class="admin-save-button-left-bg"></div>
                  			<div class="admin-save-button-middle-bg"> --> <?php print t('LBL046'); ?> <!-- </div>
                  			<div class="admin-save-button-right-bg"></div> -->
                  		<?php }else{?>
                  		<?php print t('LBL046'); ?>
                  		<?php }?>
            		</td>
            	</tr>
            </table>
          </div>

<?php }else if($registerEndDateCheck <> 0) { ?>
          <div class="top-record-div">
            <table class="search-register-btn change-class-btn" border="0">
            	<tr>
            		<td class="<?php print $actionDisableClass;?>" id="assignClass_<?php print $results->cls_id; ?>">
            		 <?php if($theme_key == 'expertusoneV2') {?>
                  			<!-- <div class="admin-save-button-left-bg"></div>
                  			<div class="admin-save-button-middle-bg"> --> <?php print t('LBL047'); ?> <!-- </div>
                  			<div class="admin-save-button-right-bg"></div> -->
                  		<?php }else{?>
                  		<?php print t('LBL047'); ?>
                  		<?php }?>
            		</td>
            	</tr>
            </table>
          </div>
<?php }else {
	$seatDisplay = true;
              if( $myEnrollChangeCls == 'Enroll' || $myEnrollChangeCls == 'compliance'){
              	//36658: Unable to choose another class using "Change Class" function.
  							    if(!empty($_SESSION['availableFunctionalities']->exp_sp_commerce) && ($registerClassPrice != '0.00' || $results->price != '0.00')){  //|| ($results->price == '0.00' && $results->export_compliance == 'Y') ?>
  								<div class="top-record-div">
                                   <table class="search-register-btn change-class-btn" border="0">
                                   <tr>
  										<td data="<?php print $passData;?>" class="<?php print ($waitlist_register > 0) ? $actionWaitlistClass : $actionClass; ?>"
  										id="addToCartList_<?php print $classID; ?>"
  										onclick="$('body').data('learningcore').enrollChangeClassErrorMessage('<?php print 'registertitle'; ?>','<?php print t('MSG692'); ?>');"
  										>
  											<div><?php print ($waitlist_register > 0) ? t("LBL138") : t('LBL050'); ?></div>
  											<?php print !empty($_SESSION['availableFunctionalities']->exp_sp_commerce) ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>
  										</td>
  									</tr>
  								</table>
  							</div>

  							<?php } else { ?>
  							    <div class="top-record-div">
                                   <table class="search-register-btn change-class-btn" border="0">
                                     <tr>
  										<td class="<?php print ($waitlist_register > 0) ? $actionWaitlistClass : $actionClass; ?>"
  											id="registerCls_<?php  print $classID;?>"
  											onclick='$("#learningplan-tab-inner").data("learningplan").changeClassEnroll("<?php print $userId; ?>","<?php print $results->crs_id; ?>","<?php print $results->cls_id; ?>",this,<?php print $waitlist; ?>,"<?php print $myEnrollChangeCls; ?>");'
  											>
  											<div><?php print ($waitlist_register > 0) ? t("LBL138") : (($results->enrolled_status == 'lrn_crs_reg_cnf') ? t('Registered') : t('Register'));  ?></div>
  											<?php print '';// 41115: When commerce is enabled and e is not set then we should be showing as Register without the 0$ //print !empty($_SESSION['availableFunctionalities']->exp_sp_commerce) ? ("<div class='".$entityColor."'>".$results->currency_type ." ".$results->price."</div>") : '';?>
  											</td>
  									    </tr>
  								    </table>

  						     <?php	} ?>

  						<?php }  else if($myEnrollChangeCls != 'Enroll') {

             
?>
              <div class="top-record-div">
                <table class="search-register-btn change-class-btn" border="0">
                	<tr>
                		<td class="<?php print ($waitlist_register > 0) ? $actionWaitlistClass : $actionClass; ?>" id="assignClass_<?php print $results->cls_id; ?>" onclick='$("#learningplan-tab-inner").data("learningplan").changeClassEnroll("<?php print $userId; ?>","<?php print $results->crs_id; ?>","<?php print $results->cls_id; ?>",this,<?php print $waitlist; ?>,"<?php print 'Tp'; ?>");'>
                		<?php if($theme_key == 'expertusoneV2') {?>
                  			<!-- <div class="admin-save-button-left-bg"></div>
                  			<div class="admin-save-button-middle-bg">	 -->
                        <?php print ($waitlist_register > 0) ? t('Waitlist') : t('Register'); //print "Wlst : ".$waitlist_register."| clsId : ".$results->cls_id; ?>
                		<!-- </div>
                		<div class="admin-save-button-right-bg"></div> -->
                		<?php }else{?>
                  	     <?php print ($waitlist_register > 0) ? t('Waitlist') : t('Register'); //print "Wlst : ".$waitlist_register."| clsId : ".$results->cls_id; ?>
                  		<?php }?>
                		</td>
                	</tr>
                </table>
              </div>

          <?php
          }
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