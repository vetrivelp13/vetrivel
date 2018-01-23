<?php
global $theme_key;
expDebug::dPrint('Reult set'.print_r($results,true),5);
$passData = "data={'NodeId':'".$results->node_id."','ClassId':'".$results->cls_id."','CourseId':'".$results->crs_id."'}";
$seatDisplay = false;
$registerEndDateCheck = registerEndDateCheck($results->crs_id,$results->cls_id);
$mandatoryValue = 'Y';
if($results->object_type =='Class') {
  //$results->enrolled_id = myteamClassRegisteredOrNot($results->crs_id,$results->cls_id,$userId);
	//$exmpId = getRegisteredEnrollmentExmeptedorNot('class',$userId,$results->crs_id,$results->cls_id);
  $results->mandatory_value = myteamFetchEnrollment($results->crs_id,$results->cls_id,$userId);
  $idvalue='id="assignClass_checkbox_'.$results->cls_id.'"';
  $assignmsg='<div class="disp-msg" id="assignClassMsg_'.$results->cls_id.'"></div>';
  $esignAccord = "<fieldset id='esign-fieldset-cls".$results->cls_id."' class='instructor-cancel-confirm-popup myteam-esign-fieldset' style ='display:none;'><legend><strong>E-Signature</strong></legend><div id='esign-accordion-cls".$results->cls_id."' class='myteam-esign-accord'></div></fieldset>";

  $getRegister = getRegisteredOrNot($results->crs_id,$results->cls_id,$userId);
  $results->enrolled_id = $getRegister->enrolled_id;
  $results->comp_status = $getRegister->comp_status;
  $results->enrolled_status     = $getRegister->enrolled_status;
  $results->waitlist_flag       = $getRegister->waitlist_flag;
  $results->waitlist_priority   = $getRegister->waitlist_priority;
  $results->is_cart_added       = isCartAdded($results->node_id);
  
  
  $results->waitlist_seats  = getWaitlistCatalogInfo($results->cls_id);
  $exmpId = getRegisteredEnrollmentExmeptedorNot('class',$userId,$results->crs_id,$results->cls_id,$results->enrolled_id);
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
    $regMsg       = (($results->comp_status == 'lrn_crs_cmp_cmp') ? t('Completed') : ((($results->enrolled_status == 'lrn_crs_reg_wtl') || ($results->waitlist_flag == 'lrn_crs_reg_wtl')) ? t('LBL190') : (!empty($exmpId)) ? t('Waived') : t('Registered')));//." <br>".$results->enrolled_status;
    $waitlist_register  = ($regMsg !== 'Waitlisted') ? 0 : $waitlist_register; 
    $seatDisplay        = ($regMsg == 'Waitlisted') ? true : false; 
  }  
  $statusArray = array('lrn_crs_cmp_enr','lrn_crs_cmp_inp');
  
  if(($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 1 ) {
  	$selectStmt = db_select('slt_enrollment', 'enr');
  	
  	
  	$selectStmt->condition('enr.user_id', $userId);
  	 
  	$selectStmt->condition('enr.class_id', $results->cls_id, '=');
  	
  	$selectStmt->condition('enr.reg_status', 'lrn_crs_reg_cnf');
  	$selectStmt->isNull('enr.master_enrollment_id');
  	$selectStmt->addField('enr', 'comp_status', 'regStatus');
  	$selectStmt->orderBy('enr.id', 'DESC');
  	$selectStmt->range(0, 1);
  	expDebug::dPrintDBAPI(' $selectStmtgetEnrollmentRegistrationStatus sannn= ' , $selectStmt);
  	// Execute query
  	$compStatusresult = $selectStmt->execute()->fetchField();
  	
  	expDebug::dPrint(' $regStatus aaa= ' . print_r($result, true) , 4);
  	$enrolledInp = array("lrn_crs_cmp_enr","lrn_crs_cmp_inp");
  	expDebug::dPrint('Reult set came hereeee'.print_r($results->comp_status,true),5);
  	if(in_array($compStatusresult,$statusArray)){
  	$regMsg       =  t('Registered');//." <br>".$results->enrolled_status;
  	}
  	
  }
  $pastDateCount   = checkPastDateOrNot($results->cls_id,$results->delivery_type_code);
  expDebug::dPrint("dgsdfgsd-->>".$results->mandatory_value);
  $mandatoryValue = ($results->is_compliance == 1) ? 'N' : 'Y';
}else{
  //$results->enrolled_id = myteamObjectClassRegisteredOrNot($results->cls_id,$userId);
  $results->mandatory_value = myteamFetchProgram($results->cls_id,$userId);
  //$exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$userId,$results->cls_id);
  $idvalue='id="assignPrg_checkbox_'.$results->cls_id.'"';
  $assignmsg='<div class="disp-msg" id="assignPrgMsg_'.$results->cls_id.'"></div>';
  $esignAccord = "<fieldset id='esign-fieldset-prg".$results->cls_id."' class='instructor-cancel-confirm-popup myteam-esign-fieldset' style ='display:none;'><legend><strong>E-Signature</strong></legend><div id='esign-accordion-prg".$results->cls_id."' class= 'myteam-esign-accord'></div></fieldset>";

  $resultsMasterEnroll = getObjectRegisteredOrNot($results->cls_id,$userId);
  $exmpId = getRegisteredEnrollmentExmeptedorNot('tp',$userId,$results->cls_id,'',$resultsMasterEnroll->master_enrolled_id);
  $results->available_seats = getObjectAvailableSeats($results->cls_id,$userId);
  $results->waitlist_status = -1;
  if($results->available_seats==0)
  	$results->waitlist_status = getProgramWaitlistStatus($results->cls_id,$userId);
  
  if(($results->waitlist_status > 0) && ($resultsMasterEnroll->master_enrolled_id == '' || $resultsMasterEnroll->master_enrolled_id == null || $resultsMasterEnroll->master_enrolled_id == 0)) {
    $waitlist_register  = $results->waitlist_status;
    $waitlist           = 1;  
  } else {
    $waitlist_register = 0;
    $waitlist          = 0;
  }
  
  if($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_wtl') {
  	$waitlist_st = 1;
  } else {
  	$waitlist_st = 0;
  }
  
  $results->enrolled_id = $resultsMasterEnroll->master_enrolled_id;
  $results->waitlist_seats = $waitlist_register;
  $regMsg = ($resultsMasterEnroll->master_enrolled_status == 'lrn_tpm_ovr_cmp') ? t('Completed') : (($waitlist_st > 0) ? t('LBL190') :(!empty($exmpId)) ? t('Waived') : t('Registered'));
}

$isexpired = 0;
if(!empty(GetIfExpired($results->crs_id,$userId)) && ($results->enrolled_id == '' || $results->enrolled_id == null || $results->enrolled_id == 0)){
	$isexpired = 1;
	$regMsg =  t('LBL253');
}

       if( (($results->enrolled_id != '' || $results->enrolled_id != null || $results->enrolled_id != 0) && $results->multi_register == 0) || $isexpired ||($results->multi_register == 1 && in_array($compStatusresult,$statusArray)) ) { ?>
              <div class="top-record-div">
                  <table class="search-register-btn" border="0">
                  	<tr>
                  	<td class="search-register-mandatory-list"><div class="title-mandatory-clr"><?php print t('Mandatory'); ?></div>
                  	<?php if($theme_key == 'expertusoneV2') {
                  				if($results->mandatory_value=="Y" || $results->mandatory_value=="1"){
                  	?>
                  	<label class="checkbox-selected"><input type="checkbox" <?php print $idvalue;?>  checked="checked"  value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
                  	<?php } else{?>
                  	<label class="checkbox-unselected"><input type="checkbox" <?php print $idvalue;?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
                  	<?php } } else{?>
                  	<input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y"){?> checked="checked" <?php }?> value= "<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox"/></td>
                  	<?php }?>
                  		<td class="action-btn-disable"
                  			id="assignClass_<?php print $results->cls_id; ?>">
                  	<?php if($theme_key == 'expertusoneV2') {?>
                  			<div class="white-btn-bg-left"></div>
                  			<div class="white-btn-bg-middle">	 <?php print t($regMsg); ?></div>
                  			<div class="white-btn-bg-right"></div>
                   	<?php }else{?>	
                   	         <?php print t($regMsg); ?>
                   	<?php }?>	
                  		</td>
                  	</tr>
                  </table>
              </div>
<?php } else if(($results->object_type == "cre_sys_obt_trn" || $results->object_type == "cre_sys_obt_crt" || $results->object_type == "cre_sys_obt_cur") && $results->available_seats == 0 && $results->waitlist_status  == 0) { expDebug::dPrint('Training plan looppp'.print_r($results,1),5);?>
          <div class="top-record-div">
            <table class="search-register-btn" border="0"><tr>
            <td class="search-register-mandatory-list"><div class="title-mandatory-clr"><?php print t('Mandatory'); ?></div>
            <?php if($theme_key == 'expertusoneV2') {
            	     if($results->mandatory_value=="Y" || $results->mandatory_value=="1"){
            	?>
            <label class="checkbox-selected"><input type="checkbox" <?php print $idvalue;?>  checked="checked"  value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
                  	<?php } else{?>
                  	<label class="checkbox-unselected"><input type="checkbox" <?php print $idvalue;?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
                  	<?php } } else{?>
                <input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y"){?> checked="checked" <?php }?> value="Y" DISABLED class="mandatory-checkbox"></td>
                <?php }?>
            		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
            	<?php if($theme_key == 'expertusoneV2') {?>
            		<div class="white-btn-bg-left"></div>
            		<div class="white-btn-bg-middle">	<?php print t('LBL046'); ?> </div>
            		<div class="white-btn-bg-right"></div>
            	<?php }else{?>	
            	     <?php print t('LBL046'); ?>
            	<?php }?>
            		</td>
            	</tr>
            </table>
          </div>
<?php } else if(((($results->object_type =='Class' && $results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")) || ($results->object_type != 'Class')) && ($results->available_seats == 0 && $results->waitlist_seats  == 0)) { ?>
          <div class="top-record-div">
            <table class="search-register-btn" border="0"><tr>
            <td class="search-register-mandatory-list"><div class="title-mandatory-clr"><?php print t('Mandatory'); ?></div>
            <?php if($theme_key == 'expertusoneV2') {
            	if($results->mandatory_value=="Y" || $results->mandatory_value=="1"){
            		?>
            	            <label class="checkbox-selected"><input type="checkbox" <?php print $idvalue;?>  checked="checked"  value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
            	                  	<?php } else{?>
            	                  	<label class="checkbox-unselected"><input type="checkbox" <?php print $idvalue;?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" ></label></td>
            	                  	<?php } } else{?>
            	<input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y"){?> checked="checked" <?php }?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox"></td>
            <?php } ?>
            <td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
            	<?php if($theme_key == 'expertusoneV2') {?>	
            	<div class="white-btn-bg-left"></div>
            	<div class="white-btn-bg-middle">	<?php print t('LBL046'); ?></div>
            	<div class="white-btn-bg-right"></div>
            	  <?php }else{?>
            	  <?php print t('LBL046'); ?>
            	  <?php }?>
            		</td>
            	</tr>
            </table>
          </div>

<?php }else if($registerEndDateCheck <> 0) { ?>
          <div class="top-record-div">
            <table class="search-register-btn" border="0">
            	<tr>
            	<td class="search-register-mandatory-list"><div class="title-mandatory-clr"><?php print t('Mandatory'); ?></div>
            	<?php if($theme_key == 'expertusoneV2') {?>
            		<label class="checkbox-unselected"><input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y"){?> checked="checked" <?php }?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox" onclick="checkboxSelectedUnselectedCommon(this);"></label></td>
            	<?php }else {?>
                  	<input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y"){?> checked="checked" <?php }?> value="<?php print $mandatoryValue;?>" DISABLED class="mandatory-checkbox"></td>
                <?php }?>
            		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>">
            		<?php if($theme_key == 'expertusoneV2') {?>
            		<div class="white-btn-bg-left"></div>
            		<div class="white-btn-bg-middle"><?php print t('LBL047'); ?> </div>
            		<div class="white-btn-bg-right"></div>
            		  <?php }else{?>
            		  <?php print t('LBL047'); ?> 
            		  <?php }?>
            		</td>
            	</tr>
            </table>
          </div>
<?php }else { 
              $seatDisplay = true;
?>
              <div class="top-record-div">
                <table class="search-register-btn" border="0">
                	<tr>
                	<td class="search-register-mandatory-list"><div class="title-mandatory-clr"><?php print t('Mandatory'); ?></div>
                	<?php if($theme_key == 'expertusoneV2') {?>
                		<label class="checkbox-unselected"><input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y" || $results->mro=='Mandatory' || $results->is_compliance){?> checked="checked" DISABLED <?php }?> value="<?php print $mandatoryValue;?>" class="mandatory-checkbox" onclick="checkboxSelectedUnselectedCommon(this);"></label></td>
                	<?php }else {?>
                		<input type="checkbox" <?php print $idvalue;?> <?php if($results->mandatory_value=="Y" || $results->mro=='Mandatory' || $results->is_compliance){?> checked="checked" DISABLED <?php }?> value="<?php print $mandatoryValue;?>" class="mandatory-checkbox"></td>
                	<?php }?>
                	
                	<?php if($theme_key == 'expertusoneV2') {?>
                		<?php if($results->object_type =='Class') {
                			     if($pastDateCount > 0 && ($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")) {
                			     	$seatDisplay = false;
                			     ?>
                			     		<td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>" >
		                					<div class="white-btn-bg-left"></div><div class="white-btn-bg-middle"><?php print  t('LBL105'); //print "Delivered : ?></div><div class="white-btn-bg-right"></div>
		                				</td>	     
                				<?php } else {?>
                				<?php $preRequisite = checkCatalogPrerequisite($results->crs_id,'cre_sys_obt_crs', $userId); ?>
                				<?php	if((count($preRequisite)>0)) { ?>
												<td class="<?php print 'action-btn'; ?>" id="assignClass_<?php print $results->crs_id; ?>"
												 	onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").renderTpPrequisteClass("<?php print $userId; ?>","<?php print $results->crs_id; ?>","class",this);'>
                					<div class="admin-save-button-left-bg"></div>
                					<div class="admin-save-button-middle-bg prereq-action"><?php print t('LBL230'); //Prerequisite ?></div>
                					<div class="admin-save-button-right-bg"></div>
                				</td>
                				<?php }else{?>		
				                		<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>" id="assignClass_<?php print $results->object_type.'_'.$results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").callEquivPopupForMyteam("<?php print $userId; ?>","<?php print $results->crs_id; ?>","<?php print $results->cls_id; ?>",this,<?php print $waitlist; ?>,"<?php print $results->object_type; ?>");'>
				                			<div class="admin-save-button-left-bg"></div><div class="admin-save-button-middle-bg"><?php print ($waitlist_register > 0) ? t('Waitlist') : t('LBL253'); //print "Wlst : ".$waitlist_register."| clsId : ".$results->cls_id; ?></div><div class="admin-save-button-right-bg"></div>
				                			
				                		</td>
                				<?php }  } 
                		 	}else{?>
                		 		<?php $preRequisite = checkCatalogPrerequisite($results->cls_id, 'cre_sys_obt_trp', $userId); ?>
                		 		<?php	if(count($preRequisite)>0) { ?>
									<td class="<?php print 'action-btn'; ?>" id="assignClass_<?php print $results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").renderTpPrequisteClass("<?php print $userId; ?>","<?php print $results->cls_id; ?>","initPrereq",this);'>
                					<div class="admin-save-button-left-bg"></div>
                					<div class="admin-save-button-middle-bg prereq-action"><?php print t('LBL230'); //Prerequisite ?></div>
                					<div class="admin-save-button-right-bg"></div>
                					</td>
                				<?php }else{?>
                		 	
                		<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>" id="assignClass_<?php print $results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").callObjectAssignClass("<?php print $userId; ?>","<?php print $results->cls_id; ?>",this);'>
                			<div class="admin-save-button-left-bg"></div><div class="admin-save-button-middle-bg"><?php print ($waitlist_register > 0) ? t('Waitlist') : t('LBL253'); ?></div><div class="admin-save-button-right-bg"></div>
                		</td>
						<?php }}?> 
					<?php }else{?>
					     <?php if($results->object_type =='Class') {
					     	      if($pastDateCount > 0 && ($results->delivery_type_code == "lrn_cls_dty_ilt" || $results->delivery_type_code == "lrn_cls_dty_vcl")){
					     	      	 $seatDisplay = false;
					     	      ?>
					     	      <td class="action-btn-disable" id="assignClass_<?php print $results->cls_id; ?>" >
                						<?php print  t('LBL105'); //print "Delivered : ?>
								<?php }else{ ?>
								<?php $preRequisite = checkCatalogPrerequisite($results->crs_id,'cre_sys_obt_crs', $userId); ?>
								<?php if(count($preRequisite)>0) {?>
									<td class="<?php print 'action-btn'; ?>" id="assignClass_<?php print $results->crs_id; ?>"
												 	onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").renderTpPrequisteClass("<?php print $userId; ?>","<?php print $results->crs_id; ?>","class",this);'>
                					<?php print t('LBL230'); //Prerequisite ?>
                				</td>
								<?php } else {?>
                		<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>" id="assignClass_<?php print $results->object_type.'_'.$results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").callEquivPopupForMyteam("<?php print $userId; ?>","<?php print $results->crs_id; ?>","<?php print $results->cls_id; ?>",this,<?php print $waitlist; ?>,"<?php print $results->object_type; ?>");'>
                			<?php print ($waitlist_register > 0) ? t('Waitlist') : t('LBL253'); //print "Wlst : ".$waitlist_register."| clsId : ".$results->cls_id; ?>
                			
                		</td>
                		<?php 	}  }
					     	      
					     }else{?>
					     <?php $preRequisite = checkCatalogPrerequisite($results->cls_id, 'cre_sys_obt_trp', $userId); ?>
					     <?php if(count($preRequisite)>0) {?>
					     	<td class="<?php print 'action-btn'; ?>" id="assignClass_<?php print $results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").renderTpPrequisteClass("<?php print $userId; ?>","<?php print $results->cls_id; ?>","initPrereq",this);'>
                				<?php print t('LBL230'); //Prerequisite ?>
                			</td>
					     <?php } else {?>
                		<td class="<?php print ($waitlist_register > 0) ? 'action-btn-waitlist' : 'action-btn'; ?>" id="assignClass_<?php print $results->cls_id; ?>" onclick='$("#lnr-myteam-search").data("lnrmyteamsearch").callObjectAssignClass("<?php print $userId; ?>","<?php print $results->cls_id; ?>",this);'>
                			<?php print ($waitlist_register > 0) ? t('Waitlist') : t('LBL253'); ?>
                		</td>
						<?php } }?> 
					
					<?php }?>	
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
                <div class="course-available-seats">
                	<span id="myteam-catalog-seats-available-<?php print $results->cls_id; ?>">
                      <?php
				       print $seatInfo;
			          ?>
			    	</span>
                </div>
          <?php } 
          }
?>
<?php print $esignAccord;
	  print $assignmsg;
?>

