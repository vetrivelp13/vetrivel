<?php
global $theme_key, $user;
$inLst = isset($info_list)?$info_list:'';
expDebug::dPrint('exp_sp_administration-narrow-search-results-item-details.tpl.php In narrow search result item details tpl file : $info_list = ' . print_r($inLst, true),4); ?>
<div id='<?php print $html_id; ?>' class="narrow-search-results-item-details <?php if($sumedit == 0  && $GLOBALS["user"]->uid != 1){print "narrow_search_privilege";}?>">
  <div>
      <span>
        <?php
				expDebug::dPrint('$view_list :  action_list = ' . print_r($view_list, true),4);
				expDebug::dPrint('$action_list :  object_type = ' . print_r($object_type, true),4);

        $titleWithoutinfo = '\"'.$title.'\"';

        if(!empty($additional_title_info)){
           $title = $title.' ('.$additional_title_info.')';
        }
        /*$length = !empty($additional_title_info) ? 85 : 34;

        if ($view_list[0]['object_type'] == "Course" || $view_list[0]['object_type'] == "Class" || $view_list[0]['action_link_text'] == "trainingplan") {
       	   $length = !empty($additional_title_info) ? 85 : 60;
        }
        elseif ($view_list[0]['action_link_text'] == "User") {
       	   $length = 74;
        }
        elseif ($view_list[0]['action_link_text'] == "Organization" || $view_list[0]['action_link_text'] == "Groups") {
       	   $length = 86;
        }
        elseif ($view_list[0]['action_link_text'] == "Surass" || $view_list[0]['action_link_text'] == "SurAssQues") {
       	   $length = 70;
        }
        elseif ($view_list[0]['action_link_text'] == "Content") {
       	   $length = 60;
        }
        elseif ($view_list[0]['action_link_text'] == "Banner") {
       	   $length = 60;
        }
        elseif ($view_list[0]['action_link_text'] == "Announcement") {
           $length = 54;
        }
        elseif ($object_type == 'location') {
       		$length = 70;
        }
        elseif ($object_type == 'notification') {
       		$length = 72;
        }
        elseif($object_type =='discount') {
       		$length = 74;
        }
        elseif($view_list[0]['action_link_text'] == "Order") {
       	   $length = 60;
        }
        elseif ($view_list[0]['action_link_text'] == "Content Author") {
        	$length = 60;
		    expDebug::dPrint('Content Author123456');
        	        }
        else {
       	   $length = !empty($additional_title_info) ? 85 : 72;
        }*/

		    expDebug::dPrint('searchType :  action_list = ' . print_r($truncate_keyword.'||'.$searchType, true),4);
		    //42710: survey-Irrelavant details are displayed
		    // This file called many time, this is the common file. but some where they pass title encoded so that i decode it and controle the title after that encoded.
		   // $title = html_entity_decode($title);// 	0049207:Modified by joola vasavi
        ?>
        
       <?php $displayTitle = $title;
        $title = filterSpecialCharacters($title); ?>
        <?php //$shareTitle = titleController(' ', $title,30);	//commented unwanted call to title conroller method
              //$shareTitleWithoutInfo = titleController('SHARE_TITLE', $titleWithoutinfo); ?>
        <?php /* $displayTitle = $title;*/ ?>
        <?php if (empty($title_link_path)): ?>


		  <?php //if (!empty($additional_title_info)):
        // Multi language Support
        if($title == 'Department'){
        	$title = t('LBL179');
        	$displayTitle = t('LBL179');
        }else if($title == 'Employment Type'){
        	$title = t('LBL174');
        	$displayTitle = t('LBL174');
        }else if($title == 'Job Role'){
        	$title = t('LBL133');
        	$displayTitle = t('LBL133');
        }else if($title == 'Job Title'){
        	$title = t('LBL073');
        	$displayTitle = t('LBL073');
        }else if($title == 'User Type'){
        	$title = t('LBL173');
        	$displayTitle = t('LBL173');
        }else if($title == 'Administer'){
        	$title = ucfirst(strtolower(t('ADMINISTER')));
        	$displayTitle = ucfirst(strtolower(t('ADMINISTER')));
         }
         if ($object_type == 'location') {
         	$title = htmlentities($title, ENT_QUOTES, "UTF-8");
         }
         if($view_list[0]['action_link_text'] == "Content") {
         	$disp_title = t($title);
         }
         if($view_list[0]['action_link_text'] == "Content Author") {
         	$disp_title = t($title);
         }
         else {
         	$disp_title = html_entity_decode(t($title));
         }
        ?>
        <div class="limit-title-row titlePadBot">
		  <span class="vtip narrow-search-results-additional-title-info" title="<?php print html_entity_decode(t($title), ENT_QUOTES, 'UTF-8'); ?>">

		 <?php //foreach ($action_list as $view){
		  	 if ($view_class[0]['object_type'] == "Class"){ ?>
		 		 			<a class="use-ajax narrow-search-results-item-title ctools-modal-ctools-viewscreen-wrapper"  href="/?q=<?php print $view_class[0]['action_page_path']; ?>" data-wrapperid="paint-narrow-search-results"><span class="limit-title"> <?php print ($displayTitle); ?> </span>
		   	  		</a>
		   	   <?php } else if($view_list[0]['object_type'] == "Course" || $view_list[0]['action_link_text'] == "Content" || $view_list[0]['action_link_text'] == "SurAssQues" || $view_list[0]['action_link_text'] == "Surass" || $view_list[0]['action_link_text'] == "trainingplan"
		   	   		|| $view_list[0]['action_link_text'] == "Organization" || $view_list[0]['action_link_text'] == "User" || $view_list[0]['action_link_text'] == "Groups" || $view_list[0]['action_link_text'] == "Banner" ||  $view_list[0]['action_link_text'] == "Announcement" ||  $view_list[0]['action_link_text'] == "Notification" || $view_list[0]['action_link_text'] == "Content Author"){ ?>
		   	   	<a class="use-ajax narrow-search-results-item-title ctools-modal-ctools-viewscreen-wrapper"  href="/?q=<?php print $view_list[0]['action_page_path']; ?>" data-wrapperid="paint-narrow-search-results"> <span class="limit-title"> <?php print t($displayTitle);?> </span>
		   	   			   	  		</a>
		   	  <?php }
		   	  else{  ?> <span class="limit-title">
		   	  			<?php print t($displayTitle);
					}
				 ?> </span>
		  <?php //if(isset($additional_title_info) && !empty($additional_title_info)){?> <!-- ( --> <?php //print $additional_title_info; ?><!--  ) --> <?php //}?>
		 </span>
		 </div>
		  <?php //else :?>
		  <?php //print $displayTitle; ?>
		  <?php //endif; ?>


  <?php if (!empty($action_list)): ?>
  <?php $countvalues=count($action_list);?>
    <?php $showPipe = false; ?>
    <div class="narrow-search-results-item-action-list">
    <?php //expDebug::dPrint('In narrow search result item details tpl file :  $action_list = ' . print_r($action_list, true),4); ?>
      <?php foreach ($action_list as $item): ?>
        <?php if ($showPipe): ?>
          <span class="narrow-search-results-item-detail-pipe-line">|</span>
        <?php endif; if(isset($item['tooltip'])):?>
        <span>
		    <?php endif;
		      if (!empty($item['action_page_path'])) {
		      		   $action_page_path = $item['action_page_path'];
		        	   $action_page_path .= '/' .  $item['action_button_params'];
		        	   $action_page_path .= !empty($item['action_button_params_child']) ? ('/' .  $item['action_button_params_child']) : '';
		      		}
		    ?>
		     <?php

		            $resultCnt ='';
		            if ($item['action_link_text'] == t('LBL286')) {
		            	$solr = getConfigValue('solr_search');
									if($solr == 1 && isset($addtionalInfo)){
										$resultCnt = $addtionalInfo['delete_object_record'];
									}else{
		                $resultCnt = delete_object_record($item['action_button_params'],$item['action_button_param_text']);
									}

		            }

		     ?>
		    <?php  if($theme_key == 'expertusoneV2') {
                     if($item['action_link_text']==t('LBL063')) { // Edit
                     	//print $sumedit;
	                     	if(isset($sumedit) && $GLOBALS["user"]->uid != 1){
		                     	if($sumedit > 0){
		                        $enableClass = 'enable-edit-icon';
		                     	}else{
		                        $disableClass = 'disable-edit-icon';
		                     	}
	                     	}else{
	                     		if($title == "Currency" && !module_exists('exp_sp_commerce'))
	                     		{
	                     			$enableClass = 'disable-edit-icon';
	                     			$item['action_page_path'] = "";
	                     			$item['tooltip'] = "Disabled";
	                     		}
	                     		else
	                     			$enableClass = 'enable-edit-icon';
	                     	}
                     } elseif($item['action_link_text'] == t('LBL930')) { //Unlock
                         $enableClass = 'enable-unlock-icon';
                         $disableClass = 'disable-unlock-icon';
                     } elseif($item['action_link_text'] == t('LBL286')) { //Delete
                         $enableClass = 'enable-delete-icon';
                         $disableClass = 'disable-delete-icon';
                     } elseif($item['action_link_text'] == t('LBL1021')) { //Setting
                         $enableClass = 'enable-edit-icon';
                           if (!empty($item['action_payment_setting']) && ($item['action_payment_setting'] == 2)){
                               $enableClass = 'setting-disable-edit-icon';
                           }
                     }else if ($item['action_link_text']==t('LBL1023')) {//Configure
                           $enableClass = 'enable-edit-icon';
                           if (!empty($item['action_module_status']) && ($item['action_module_status']==2)){
                               $enableClass = 'setting-disable-edit-icon';
                           }
                     }
                    } else {
                         $enableClass = '';
                         $disableClass = '';
                    }
                    if(isset($sumedit) && $GLOBALS["user"]->uid != 1){
	                    if($sumedit > 0){
	                    	$disableeditlink =  '';
	                    }else{
	                    	$disableeditlink =  'disable-edit-link'; //disable-edit-link edit
	                    }
                    }
                    if(isset($sumdelete) && $GLOBALS["user"]->uid != 1){
	                    if($sumdelete > 0){
	                    	$disabledeletelink = '';

	                    }else{
	                    	$disabledeletelink =  'disable-delete-link';//delete
	                    }
                    }

                    if((arg(2) == 'moduleinfo' && $theme_key == 'expertusoneV2') && ($displayTitle == t('Commerce') || $displayTitle == t('LBL855') || $displayTitle == t('Rating') || $displayTitle == trim(t('LBL1247')) || $displayTitle == t('LBL1248'))){
                    	$class= "";
                    }else {
                    	$class= "narrow-search-results-item-action-list-btn ";
                    }


            ?>
    <div id="qtipEditPayment_<?php print $item['action_button_params'];?>" class="visible-ctools-commerece-popup" >
    <?php $item['tooltip'] = ($item['tooltip'] == 'Setting') ? t('LBL1021') : $item['tooltip']; // Multi lang Commerece Setting Page Title #0043782 ?>
    <span class="vtip" title="<?php print $item['tooltip'];?>">
			<a id="visible-ctools-<?php print $item['action_button_params'];?>" class="<?php print $class;?> addedit-form-expertusone-throbber
			          <?php print $enableClass; ?>
			          <?php if($item['action_link_text']==t('LBL063')) { print $disableeditlink; print " ".$disableClass;}?>
			          <?php if($item['action_link_text']==t('LBL286')) { print $disabledeletelink; }?>
			          <?php if(($resultCnt > 0) || ( !empty($item['action_payment_setting']) && ($item['action_payment_setting'] == 2)) || (!empty($item['action_module_status']) && ($item['action_module_status']==2)) ){
			              print 'disable-delete-link';
			          }
			          ?>
			          <?php if ((!isset($item['action_payment_status'])) && !empty($item['action_page_path']) &&
			                     (empty($item['action_module_status']) || ($item['action_module_status'] != 2)))
			              print $item['ctools_style'].' use-ajax';
			          ?>"
				<?php  if(($item['action_link_text']==t('LBL063') && $sumedit > 0 || $GLOBALS["user"]->uid == 1 || arg(1) == 'commerce' || arg(2) == 'setup' || !isset($sumedit))
										&& (arg(3) != 'PaymentMethod' && arg(3) != 'PaymentGatewayMethod') && (arg(2)!='discounts')) { 

// issue is Fixed For Commerce payment Method-> Credit and Checkout is Unable To Edit ?>
						<?php if (!empty($item['action_page_path'])): ?>
						onclick='$(".qtip-popup-visible").html("").hide(); $(".active-qtip-div").remove();$("#root-admin").data("narrowsearch").setPositionToCtoolPop("<?php print $action_page_path; ?>");orderEdit()'; href='/?q=<?php print $action_page_path; ?>'  data-wrapperid="paint-narrow-search-results"
						<?php endif; ?>
		  <?php } else if(($item['action_link_text']==t('LBL063')) && (arg(2) == 'discounts')) { 			  	
		if(($sumedit > 0 || $GLOBALS["user"]->uid == 1)){  ?>
		onclick='$(".qtip-popup-visible").html("").hide(); $(".active-qtip-div").remove();$("#root-admin").data("narrowsearch").setPositionToCtoolPop("<?php print $action_page_path; ?>");orderEdit()'; href='/?q=<?php print $action_page_path; ?>'  data-wrapperid="paint-narrow-search-results"
		<?php } else { ?>
		onclick='return false;'  href='javascript:void(0);' 
		<?php } ?>			  
			   <?php }  else if ($item['action_link_text']==t('LBL1023')) {?>
			    <?php if (empty($item['action_module_status']) || ($item['action_module_status'] != 2)) { ?>
			      <?php if (!empty($item['action_page_path'])): ?>  onclick='$(".active-qtip-div").remove();$("#root-admin").data("narrowsearch").setPositionToCtoolPop("<?php print $action_page_path; ?>")'; href='/?q=<?php print $action_page_path; ?>'  data-wrapperid="paint-narrow-search-results" <?php endif; ?>
			    <?php } else { ?>
			        onclick='return false;'
				  <?php } ?>
				<?php } else if ($item['action_link_text']==t('LBL1021')) {
                  $popupentityId      = $item['action_button_params'];//$rec_search_results->id;
                  $popupAddPaymententityType    = 'payment';
                  $popupAddPaymentIdInit        	 = $popupentityId.'_'.$popupAddPaymententityType;
                  $popupAddPaymentvisibPopupId  = 'qtip_visible_disp_addpayment_'.$popupAddPaymentIdInit;

                  $widthBubble = 534;
                  $poslwid=125;
                  if ($item['action_button_params'] == 'credit') {
                  	if ($user->language == 'it') {
                  		$widthBubble = 805;
                  	} else {
                  		$widthBubble = 705;
                  	}
                  	$poslwid=150;
                  }
                  if ($item['action_button_params'] == 'cybersource' || $item['action_button_params'] == 'paymetric' ) {
                    $widthBubble = 600;
                    $poslwid=150;
                  }

                  $popupAddPayment    = "{'entityId':'".$popupentityId."','entityType':'".$popupAddPaymententityType."',
												                  'url':'administration/commerce/setting/payment/".$item['action_button_params']."',
												                  'popupDispId':'qtipEditPayment_".$popupentityId."',
												                  'catalogVisibleId':'qtipEditPaymentIdqtip_visible_disp_".$popupAddPaymentIdInit."',
												                  'wid':" . $widthBubble . ",'heg':'100','postype':'bottomleft','poslwid':" . $poslwid . ",'qdis':'',
												                  'linkid':'visible-ctools-".$popupentityId."','dispDown':'Y'}";

                  $onclick = 'onclick="callVisibility('.$popupAddPayment.');"';
                  expDebug::dPrint("popup1".$onclick,5);
				?>
			    <?php if($item['action_payment_setting'] == 1) {

			      if (!empty($item['action_page_path'])): ?>  <?php print $onclick;?>  data-wrapperid="paint-narrow-search-results" <?php endif; ?>
			    <?php } else { ?>
			        onclick='return false;'
				  <?php } ?>
				<?php }else {?>
						<?php if (!empty($item['action_page_path']) && ($sumedit > 0)  || $GLOBALS["user"]->uid == 1 && arg(1) != 'commerce'){
						?>  href='/?q=<?php print $action_page_path; ?>'  data-wrapperid="paint-narrow-search-results" <?php
						}else{ ?>
						 href='javascript:void(0);'
				<?php } }?>
				<?php if($item['action_link_text'] == t('LBL930')) { ?>
				onclick='$("#root-admin").data("narrowsearch").displayDeleteWizard("<?php print htmlspecialchars(t('MSG578'), ENT_QUOTES).' '.check_plain(str_replace("\\\\", "", escape_string($titleWithoutinfo)));?>","<?php print $item['action_button_params']; ?>", "<?php print $item['action_button_param_text']; ?>",300);'
				<?php } ?>
				<?php if (empty($item['action_page_path'])  && $sumdelete > 0  || $GLOBALS["user"]->uid == 1 || arg(1) == 'commerce'  || !isset($sumedit)){ ?>
				        <?php  if ($item['action_link_text'] == t('LBL286'))  {
				            if($resultCnt == 0 && ($item['action_button_param_text'] == "Course" || $item['action_button_param_text'] == "Class" ||
				                                     $item['action_button_param_text'] == "TP" || $item['action_button_param_text'] == "User" ||
				                                       $item['action_button_param_text'] == "Organization" || $item['action_button_param_text'] == "SurAss" ||
                                                         $item['action_button_param_text'] == "Content" || $item['action_button_param_text'] == "SurAssQues" || $item['action_button_param_text'] == "location" || $item['action_button_param_text'] == "banner" || $item['action_button_param_text'] == "Custom" || $item['action_button_param_text'] == "Tax" || 
$item['action_button_param_text'] == ("Certificate" || "notification_template"))) {//#custom_attribute_0078975
				            	$delete_info=empty($delete_info)?'':str_replace('\'','\u0027',$delete_info);
				            	if($item['action_button_param_text'] == "Content")
				            		$msg_str = $delete_info.' '.check_plain(str_replace("\\\\", "", escape_string($titleWithoutinfo))).'?<br/>'.t('LBL1281');
				            	else
				            		$msg_str = $delete_info.' '.check_plain(str_replace("\\\\", "", escape_string($titleWithoutinfo)));
				          ?>
				     			onclick='$("#root-admin").data("narrowsearch").displayDeleteWizard("<?php print $msg_str;?>","<?php print $item['action_button_params']; ?>", "<?php print $item['action_button_param_text']; ?>",300);'
    				    <?php }
    				    }else{
    				    	if (!empty($item['action_button_params'])) { ?>
         				       onclick='$("<?php print '#' . $item['js_object_info']['init_id']; ?>").data("<?php print $item['js_object_info']['name']; ?>").editItem(<?php print $item['action_button_params']; ?>);'
         				<?php }
						}
				} ?>>


					<?php if($theme_key != 'expertusoneV2' && arg(2) != 'moduleinfo'):
				        print $item['action_link_text'];

				      endif;
				      if($theme_key != 'expertusoneV2' && arg(2) == 'moduleinfo' && $displayTitle != t('Commerce') && $displayTitle != t('LBL855') && $displayTitle != t('Rating') && $displayTitle != trim(t('LBL1247')) && $displayTitle != t('LBL1248')){
				      	print $item['action_link_text'];
				      }

				?>
	        </a>
	        </span>
	     <span id="visible-popup-<?php print $item['action_button_params'];?>" class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
	  </div>
        </span>
        <?php if ($item['action_link_text']==t('LBL1021')) {?>
	          		<div class="add_session_popup" id="<?php print $popupAddPaymentvisibPopupId;?>" ></div>
	       <?php  }?>
        <?php $showPipe = true; ?>
    <?php endforeach; ?>
    </div>
  <?php endif; if(isset($promote_action['tooltip'])):?>

            <span class="vtip" title="<?php print  $promote_action['tooltip']; ?>">
      		    <?php endif;
      		      if (!empty($promote_action['action_page_path'])) {
      		      		   $promote_action_page_path = $promote_action['action_page_path'];
      		        	   $promote_action_page_path .= '/' .  $promote_action['action_button_params'];
      		        	   $promote_action_page_path .= !empty($promote_action['action_button_params_child']) ? ('/' .  $promote_action['action_button_params_child']) : '';
      		      		}
      		      		$actImg = isset($promote_action['action_link_img'])?$promote_action['action_link_img']:'';
      		    ?>
      			<a class="<?php print $actImg;?> <?php if (!empty($promote_action['action_page_path'])) print $promote_action['ctools_style'].' use-ajax'; ?>"
      				<?php if (!empty($promote_action['action_page_path'])): ?> href='/?q=<?php print $promote_action_page_path; ?>'  data-wrapperid="paint-narrow-search-results" <?php endif; ?>
      				>
      				<?php $actTxt = isset($promote_action['action_link_text'])?$promote_action['action_link_text']:'';
      				print $actTxt; ?>
      	        </a>
            </span>


        <?php else: ?>
          <span style="float:left">
          <?php print l($displayTitle, $title_link_path,
                          array('attributes' => array('class' => 'narrow-search-results-item-title', 'title' => $title)));?>
          </span>
          <div style="clear:both" /></div>
        <?php endif; ?>
      </span>
      <div class="clearBoth"></div>
  </div>
  <?php  //echo "<pre>"; print_r($details_list);echo "</pre>";?>
  <?php if (!empty($details_list)): ?>
  <?php $countvalues=count($details_list);?>
    <?php $showPipe = false; ?>
    <div class="narrow-search-results-item-detail attrPadBWithDesc">
      <?php $index = 0;
      foreach ($details_list as $item): $index++;?>
      <?php // check made for admin and Learner Group Multilanguage support
	       if($item['detail'] == 'Admin group'){
	       	$item['detail'] = t('Admin').' '.t('Group');
	      }else if($item['detail'] == 'Learner group'){
	      	$item['detail'] = t('Learner').' '.t('Group');
	      }
      ?>
        <?php if ($showPipe): 
        	   if($view_list[0]['action_link_text'] != "Content Author") //do not display for content author h5pcustomize
        	   {
        	?>
          <span class="narrow-search-results-item-detail-pipe-line">|</span>
        <?php }
        endif; if(isset($item['tooltip'])){
					 if($item['tooltip']==t('LBL096')){
                     $charres = (!empty($item['charres'])) ? $item['charres'] : 15; ?>
              <span class="vtip" title="<?php print t($item['tooltip']).' : '.t($item['detail']); ?>"><?php print titleController('ADMIN-NARROW-SEARCH-CODE',t($item[detail]),$charres); ?></span>
         <?php }else{
         	$explodeString  = explode(":",$item['tooltip']);
         	$uniString = '';
			if($explodeString[0] == t('LBL038')) {
         		$uniString = "NARROW-SEARCH-LANG";
         	}
         	/*if($explodeString[0] == t('LBL036')) {
         		$uniString = "NARROW-SEARCH-TYPE";
         	}*/
         	if($explodeString[0] == t('LBL854')) {
         		$uniString = "NARROW-SEARCH-LESSON";
         	}
         	if($explodeString[0] == t('LBL244')) {
         		$uniString = "NARROW-SEARCH-ORGANIZATION-TYPE";
         	}
         	if($explodeString[0] == t('LBL039')) {
         		$uniString = "NARROW-SEARCH-COUNTRY";
         	}
         	if($explodeString[0] == t('LBL918')) {
         		$uniString = "NARROW-SEARCH-RECIPIENT-NAME";
         	}
         	if($explodeString[0] == t('Equipment')) {
         		$uniString = "NARROW-SEARCH-EQUIPMENT";
         	}
         	if($explodeString[0] == t('LBL096')) {
         		$uniString = "NARROW-SEARCH-COMMON-CODE";
         	}
         	if(($explodeString[0] == t('LBL091')) || ($explodeString[0] == t('LBL093'))) {
         		$tip_string = $item['detail'];
         	}
         	else {
         		$tip_string = $item['tooltip'];
         	}
         	if(isset($uniString) && !empty($uniString)) {
         		if($view_list[0]['action_link_text'] != "Content Author") //do not display for content author
				{
			?>
         	 <span class="vtip" title="<?php print t($tip_string); ?>"><?php  print titleController($uniString,t($item['detail']),20); ?></span> 
        	<?php }
				 } else { if(isset($explodeString[0]) && $explodeString[0] != t('LBL036')) {
        		expDebug::dPrint('vadivel check delivery type :  action_list = ' . print_r(isset($explodeString[0]) && $explodeString[0], true),5);
        		if($view_list[0]['object_type'] != "Class"){
        		?>
        	<span class="vtip" title="<?php print t($tip_string); ?>"><?php  print $item['detail']; ?></span>
        	<?php 
        		}
        	} } }
        }else {?>
        <span><?php print $item['detail']; ?></span>
        <?php }?>
        <?php if(isset($explodeString[0]) && $explodeString[0] == t('LBL036') && $index == 1 || $view_list[0]['object_type'] == "Class" && isset($explodeString[0]) && $explodeString[0] != t('LBL036')) {
        	$showPipe = false;
        } else {
        	$showPipe = true;
        }?>
    <?php endforeach; ?>
	<?php if($delivery_type_code == "lrn_cls_dty_ilt" || $delivery_type_code == "lrn_cls_dty_vcl") {
	  if(count($session_details) > 0) {
		$sDay = $session_details[0]['session_start_date_format'];
		$sTime = $session_details[0]['session_start_time_format'];
		$sTimeForm = $session_details[0]['session_start_time_form'];
		if(count($session_details) > 1) {
		  $sessLenEnd = count($session_details)-1;
		  $eDay = $session_details[$sessLenEnd]['session_start_date_format'];
		  $eTime = $session_details[$sessLenEnd]['session_start_end_format'];
		  $eTimeForm = $session_details[$sessLenEnd]['session_end_time_form'];
		  $sessionDate = $sDay .' '. $sTime .' '. $sTimeForm.' to '.$eDay. ' ' .$eTime.' '.$eTimeForm;
		}else{
		  $eTime = $session_details[0]['session_start_end_format'];
		  $eTimeForm = $session_details[0]['session_end_time_form'];
		  $sessionDate = $sDay .' '. $sTime.' '.$sTimeForm.' to '.$eTime.' '.$eTimeForm;
		  $sessionTimeZone = $session_details[0]['sess_timezone'];
		  $sessionFullTimeZone = $session_details[0]['sess_fulltimezone'];
		}
	}
	if(!empty($sessionDate) && !empty($sessionFullTimeZone)) {
	  $dateWithZone = $sessionDate.' ('.$sessionFullTimeZone.')';
	}else if($sessionDate){
	 $dateWithZone = $sessionDate;
	}
	?>
	<?php if (!empty($dateWithZone)): ?>
	<span class="narrow-search-results-item-detail-pipe-line">|</span>
	<span class="vtip" title="<?php print $dateWithZone;?>"><?php ($theme_key == 'expertusoneV2') ? print titleController('ADMIN-NARROW-SEARCH-DATEWITHZONE',$dateWithZone,35) : print titleController('ADMIN-NARROW-SEARCH-DATEWITHZONE',$dateWithZone,45);?> </span>
	<?php endif; ?>
	<?php }?>
    </div>
  <?php endif; ?>

  <?php if (!empty($description)): ?>
  	<div class="limit-desc-row <?php echo $more_type ?>">
    <div class="narrow-search-results-item-description">
     <span class="limit-desc vtip">
      <span class="cls-admin-descriptions"> <?php print $description;  ?> </span>
    </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($entity_multi_action)): ?>
    <?php
    //expDebug::dPrint(' $entity_multi_action velu '.print_r($entity_multi_action,true),5);

      $emptyId           = 0;
      /*--- URL encryption applied to the enabled entities (exp_sp_security). This codes to be changed as default (When all enabled) in the future ---*/
      global $secure_entities;
      if (in_array($entity_multi_action->entityType, $secure_entities)) {
      	$entityId          = core_encrypt($entity_multi_action->entityId);
      	$entitycourseId    = core_encrypt($entity_multi_action->courseId);
      	$tagsentityType    = $entity_multi_action->entityTypeName;
      	$tagsentityId      = $entityId;
      	$qtipTagsIdInit    = $entityId.'_'.$tagsentityType;
      	$entityType        = $entity_multi_action->entityType;	
      	$qtipIdInit        = $entityId.'_'.$entityType;
      } else {
      	$entityId          = $entity_multi_action->entityId;
      	$entitycourseId    = $entity_multi_action->courseId;
      	$tagsentityType    = $entity_multi_action->entityTypeName;
      	$tagsentityId      = $entityId;
      	$qtipTagsIdInit    = $entityId.'_'.$tagsentityType;
      	$entityType        = $entity_multi_action->entityType;
      	
      	
       if($view_list[0]['action_link_text'] == 'Content Author')
        {      	      	
      	 if(strpos($view_list[0]['action_page_path'],'administration/contentauthor/presentation/') !== false)
      		$entityType     =   'cre_sys_obt_cnt_aut_pre';
      	else
      		$entityType     =   'cre_sys_obt_cnt_aut';
      	
        } 
      
      	$qtipIdInit        = $entityId.'_'.$entityType;
      }

      if($entity_multi_action->object_type == 'Content' || $entity_multi_action->object_type == 'location' || ($entity_multi_action->object_type == 'Content Author')){
      	$entityId          = $entity_multi_action->id;
      	$tagsentityType    = $entity_multi_action->object_type;
      }if($entity_multi_action->object_type == 'Order'){
      	$entityId          = $entity_multi_action->slt_order_id;
      }
      expDebug::dPrint(' $tagsentityType '.print_r($tagsentityType,true),5);

      $tagsvisibPopupId  = 'qtip_visible_disp_tags_'.$qtipTagsIdInit;
      if($theme_key == 'expertusoneV2') {
      	$qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'".$tagsentityType."','url':'administration/catalog-tags/ajax/".$entityId."/".$tagsentityType."/".$emptyId."','popupDispId':'narrow_search_".$tagsvisibPopupId."','catalogVisibleId':'narrow_search_renderTagsId".$qtipTagsIdInit."','wid':400,'heg':'150','postype':'middle','poslwid':'1','qdis':'','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
      } else {
      	$qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'".$tagsentityType."','url':'administration/catalog-tags/ajax/".$entityId."/".$tagsentityType."/".$emptyId."','popupDispId':'narrow_search_".$tagsvisibPopupId."','catalogVisibleId':'narrow_search_renderTagsId".$qtipTagsIdInit."','wid':400,'heg':'140','postype':'middle','poslwid':'','qdis':'','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
      }
      //$qtipTagsOptObj    = "{'entityId':".$entityId.",'entityType':'".$tagsentityType."','url':'administration/catalog-tags/ajax/".$entityId."/".$tagsentityType."/".$emptyId."','popupDispId':'narrow_search_".$tagsvisibPopupId."','catalogVisibleId':'narrow_search_renderTagsId".$qtipTagsIdInit."','wBubble':300,'hBubble':'auto'}";
/*      if($theme_key == 'expertusoneV2') {
        $attachmentLabel = t('');
        $tagsLabel = t('');
        $accessLabel = t('');
        $previewLabel = t('');
        $permissionsLabel = t('');
        $userLabel = t('');
        $enrollmentLabel = t('');
      } else {
*/      $attachmentLabel = t('LBL231');
        $tagsLabel = t('LBL191'); // Tags
        $accessLabel = t('LBL642');
        $previewLabel = t("LBL694");
        $permissionsLabel = t('LBL1035');
        $userLabel = t('Users');
        // $ownerLabel = t('Owner');
        $enrollmentLabel = t('LBL275');
        $createCopyLabel = t('Create copy');
        $sendmessageLabel = t('LBL1282');

        /*      }*/



      if($entityType == 'cre_sys_obt_trp'){
        $entityTypeAccess  = $entity_multi_action->objectType;
      }else if($entityType == 'Order'){
        $entityTypeAccess  = $entity_multi_action->object_type;
      }else{
        $entityTypeAccess  = $entityType;
      }

      //$qtipOptAccessObj      = "{'entityId':".$entityId.",'entityType':'".$entityType."','url':'administration/catalogaccess/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'narrow_search_qtip_visible_disp_".$qtipIdInit."','catalogVisibleId':'narrow_search_qtipAccessqtip_visible_disp_".$qtipIdInit."','wBubble':750,'hBubble':'auto','tipPosition':'bottomCorner','qtipClass':'admin-qtip-access-parent'}";
 			if($entityType == 'cre_sys_obt_cls' || $entityType == 'cre_sys_obt_trp'){
      	$postype = 'bottomright';
      }else{
      	$postype = 'middle';
      }
      if($entityType == 'cre_sys_obt_crs' || $entityType == 'cre_sys_obt_cls' || $entityType == 'cre_sys_obt_trp'){
      	$popWidth = 685;
      }else{
      	$popWidth = 645;
      }
      $dummy_entity_id       = empty($entityId) ? ereg_replace(" ", "",$emptyId) : $entityId;
      if($theme_key == 'expertusoneV2') {
      	$qtipOptAccessObj      = "{'entityId':'".$entityId."',
      														 'entityType':'".$entityTypeAccess."',
      														 'url':'administration/catalogaccess/".$entityId."/".$entityTypeAccess."/".$emptyId."',
      														 'popupDispId':'narrow_search_qtip_visible_disp_".$qtipIdInit."',
      														 'catalogVisibleId':'narrow_search_qtipAccessqtip_visible_disp_".$qtipIdInit."',
      														 'wid':'".$popWidth."',
      														 'heg':'300',
      														 'postype':'".$postype."',
      														 'poslwid':'50',
      														 'onClsFn':'$(\'#root-admin\').data(\'accessgroup\').accessClose',
      														 'linkid':'access-visibility-".$entityId."'}";


      	$qtipOptShareObj      = "{'entityId':'".$entityId."',
      														 'entityType':'".$entityTypeAccess."',
      														 'url':'administration/contentauthor/content-share/".$entityId."/".$entityTypeAccess."/".$emptyId."',
      														 'popupDispId':'narrow_search_qtip_share_disp_".$qtipIdInit."',
      														 'catalogVisibleId':'narrow_search_qtipShareqtip_visible_disp_".$qtipIdInit."',
      														 'wid':645,
      														 'heg':'300',
      														 'postype':'".$postype."',
      														 'poslwid':'50',
      														 'linkid':'share-visibility-".$entityId."'}";



      	$qtipOptAttachmentObj  = "{'entityId':'".$dummy_entity_id."','entityType':'".$entityType."',
														      	'url':'administration/catalog-attachment/".$dummy_entity_id."/".$entityType."',
														      	'popupDispId':'narrow_search_qtip_attachment_disp_".$qtipIdInit."',
														      	'catalogVisibleId':'narrow_search_qtip_attachment_disp_".$qtipIdInit."',
														      	'wid':570,'heg':'270','postype':'bottomleft','poslwid':'','qdis':'', 'dispDown':'Y',
														      	'linkid':'visible-tp-attachment-".$entityId."','scrollid':'scrolldiv'}";
      } else {
      	$qtipOptAccessObj      = "{'entityId':'".$entityId."',
      														 'entityType':'".$entityTypeAccess."',
      														 'url':'administration/catalogaccess/".$entityId."/".$entityTypeAccess."/".$emptyId."',
      														 'popupDispId':'narrow_search_qtip_visible_disp_".$qtipIdInit."',
      														 'catalogVisibleId':'narrow_search_qtipAccessqtip_visible_disp_".$qtipIdInit."',
      														 'wid':'".$popWidth."',
      														 'heg':'300',
      														 'postype':'".$postype."',
      														 'poslwid':'',
      														 'onClsFn':'$(\'#root-admin\').data(\'accessgroup\').accessClose',
      														 'linkid':'access-visibility-".$entityId."'}";
      	$qtipOptAttachmentObj  = "{'entityId':'".$dummy_entity_id."','entityType':'".$entityType."',
														      	'url':'administration/catalog-attachment/".$dummy_entity_id."/".$entityType."',
														      	'popupDispId':'narrow_search_qtip_attachment_disp_".$qtipIdInit."',
														      	'catalogVisibleId':'narrow_search_qtip_attachment_disp_".$qtipIdInit."',
														      	'wid':570,'heg':'185','postype':'bottomleft','poslwid':'','qdis':'', 'dispDown':'Y',
														      	'linkid':'visible-tp-attachment-".$entityId."','scrollid':'scrolldiv'}";
      }


      $visibCertificatePopupId  = 'qtip_visible_certificatedisp_'.$entityId.'_'.$entityType;
      $qtipCertOptObj           = "{'entityId':".$entityId.",'entityType':'".$entityType."','url':'administration/print-certificate/".$entityId."/".$entityType."/".$emptyId."','popupDispId':'narrow_search_".$visibCertificatePopupId."','catalogVisibleId':'narrow_search_renderPrintCerId_".$qtipIdInit."','wBubble':376,'hBubble':'auto'}";

      if($entityType == 'cre_sys_obt_trp'){
        $enrollPopupId       = 'qtip_visible_enrolldisp_'.$entityId.'_'.$entityType;
        $qtipEnrollOptObj    = "{'entityId':'".$entityId."','entityType':'".$entityType."',
													        'url':'administration/training-plan-enroll/".$entityId."/".$entity_multi_action->objectType."',
													        'popupDispId':'narrow_search_enroll".$enrollPopupId."','catalogVisibleId':'narrow_search_render_catalog_enroll_".$qtipIdInit."',
													        'qtipClass':'display-message-positioning',
													        'wid':680,'heg':'440','postype':'middle','poslwid':'50','qdis':'',
																	'linkid':'visible-tp-enroll-".$entityId."'}";
      }else if($entityType == 'cre_sec'){
      	if($entity_multi_action->is_admin == '1'){
      	    $privilegePopupMode    = ((isset($grp_code) && $grp_code == "grp_sup") ? "view" : "search");
      	    expDebug::dPrint('priv mode '.$privilegePopupMode.' grp code = '.$grp_code);
      		$enrollPopupId      = 'qtip_visible_perm_disp_'.$entityId.'_'.$entityType;
  	    	$qtipuserpopupobj  = "{'entityId':'".$entityId."','entityType':'cre_sec',
  	    	'url':'people/roles/add-permissions/".$privilegePopupMode."/".$entityId."/".$entityType."/narrow_search_permission".$enrollPopupId."',
  	    	'popupDispId':'narrow_search_permission".$enrollPopupId."','catalogVisibleId':'qtipAttachIdqtip_addlistpermissions_visible_disp_".$qtipIdInit."',
  	    	'wid':550,'heg':'270','postype':'middle','poslwid':'1','qdis':'','linkid':'visible-priv-".$entityId."','scrollid':'admin-add-scroll'}";

  	    	// $ownerPopupId      = 'narrow_search_usersqtip_owner_disp_'.$entityId.'_'.$entityType;
  	    	// $qtipOptownerpopup = "{'entityId':".$entityId.",'entityType':'cre_sec','url':'administration/people/group/owner-addedit/".$entityId."/".$entityType."','popupDispId':'".$ownerPopupId."',
  	    	// 'catalogVisibleId':'qtipAttachIdqtip_addpermissions_visible_disp_".$qtipIdInit."','wid':520,'heg':'270','postype':'bottomleft','poslwid':'','qdis':'','linkid':'owner-priv-group-".$entityId."'}";
      	}
      	$adduserVisibpopupId ='qtip_visible_adduser_disp_'.$entityId.'_'.$entityType;
        $qtipUsersClassObj     = "{'entityId':'".$entityId."','entityType':'cre_sec','url':'administration/people/groups/add-users/".$entityId."/".$entityType."','popupDispId':'narrow_search_users".$adduserVisibpopupId."','catalogVisibleId':'qtipAttachIdqtip_addroletousers_visible_disp_".$qtipIdInit."','wid':520,'heg':'270','postype':'bottomleft','poslwid':'','qdis':'','linkid':'visible-priv-users-".$entityId."'}";
      }else{
        $enrollPopupId       = 'qtip_visible_enrolldisp_'.$entityId.'_'.$entityType;
        //$class_code			 =  htmlentities($class_code,ENT_QUOTES,'UTF-8'); // encode class code into html characters
        $qtipEnrollOptObj    = "{'entityId':'".$entityId."','entityType':'".$entityType."',
													        'url':'administration/catalog-class-enroll/".$entityId."/".$entitycourseId."/".$entity_multi_action->deliveryType."',
													        'popupDispId':'narrow_search_enroll".$enrollPopupId."','catalogVisibleId':'narrow_search_render_catalog_enroll_".$qtipIdInit."',
													        'qtipClass':'display-message-positioning',
													        'wid':680,'heg':'440','postype':'middle','poslwid':'50','qdis':'',
																	'linkid':'visible-enroll-".$entityId."'}";
      }

      $paintMultiAction  = "<div class='narrow-search-multi-action-container'>";
      if($entity_multi_action->object_type_code == "cre_sys_obt_cnt"){
      	$lessonList = getLessonList($entity_multi_action->version_id);
      	$popupHolderId = 'search-video-preview-popup-container-' . $entity_multi_action->version_id;
      	//Commented for #0068080 
      	$onclick = getOnclick($lessonList, $entity_multi_action->version_id, $popupHolderId);
      	$tab_disable = "";
      	$content_priview = 'content-preview-icon';

      	$lessonList = getLessonList($entity_multi_action->version_id);

      	if(count($lessonList) > 1){
      		expDebug::dPrint("inside cdn");
      		$lessonListObj = json_encode($lessonList);
					$onclick = 'onclick = \'$("#root-admin").data("narrowsearch").getPreviewContent('.$entity_multi_action->id.','.$entity_multi_action->version_id.','.$lessonListObj.');return false;\'';
      		$paintMultiAction .= '<div class="'.$content_priview.'" title="'.t('LBL694').'"></div><div><a title = "'.t("LBL694").'" class=\'preview-content-version vtip tab-title\' '.$onclick.'>'.$previewLabel.'</a></div>';

      	}else if(count($lessonList) == 1){
      		//expDebug::dPrint("inside cdn111:".$view_list[0]['action_link_text']);
      		if($lessonList[0]->contentypecode != "lrn_cnt_typ_vod"){
				$previewCls = "preview-content-version";
			}else{
				if(!empty($onclick) && $view_list[0]['action_link_text'] != 'Content Author')
					$previewCls = "preview-content-version actionLink enroll-launch use-ajax";
				else
					$previewCls = "preview-content-version";
			}
			expDebug::dPrint("soun test".$entity_multi_action->version_id,5);
			expDebug::dPrint("soun test1".$onclick,5);
			expDebug::dPrint("soun test2". $entity_multi_action->id,5);
			$paintMultiAction .= '<div style="float:left;" class="video-preview-popup-container" id="' . $popupHolderId . '"></div>' .
			                     '<div class="'.$content_priview.' vtip" title="' . t('LBL694') . '"></div>'. //Preview
			                     '<div>' .
			                       '<span class = "vtip" title = "'.t("LBL694").'">' . //Preview
			                         '<a class="'.$previewCls.' tab-title '.$tab_disable.'"  id="search-preview-' . $entity_multi_action->id . '-' . $entity_multi_action->version_id . '"' . $onclick. '>'.$previewLabel.'</a>' .
			                       '</span>' .
			                     '</div>';
      	}
      	if($sumedit > 0  || $GLOBALS["user"]->uid == 1){
      		$tagOnclick = "onclick =\"callVisibility(".$qtipTagsOptObj."); return false;\"";
      		$tab_disable = "";
      		$tags_tab_icon = 'tags-tab-icon';
      	}else{
      		$tagOnclick = '';
      		$tab_disable = 'tab-disable';
      		$tags_tab_icon = 'tags-disable-tab-icon';
      	}
		if($view_list[0]['action_link_text'] != "Content Author") //do not display for content author
		{
	
      	$paintMultiAction .= "<div class=\"tab-seperator\"></div>".
      											 "<div class='".$tags_tab_icon." vtip' title='".$tagsLabel."'></div>". // Tags
      											 "<div id='narrow_search_".$tagsvisibPopupId."'>".
      											   "<a id='visible-tags-".$entityId."' class=\"tab-title $tab_disable\" $tagOnclick>".$tagsLabel."</a>".
      											 "<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>";
		}
      }elseif($entityType == 'cre_sec'){
      	if($entity_multi_action->is_admin == '1'){
      		if($GLOBALS["user"]->uid == 1 || $sumedit > 0){
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='permission-tab-icon vtip' title='".t('LBL1035')."'></div><div id='narrow_search_permission".$enrollPopupId."' style='position:relative;'><a id='visible-priv-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipuserpopupobj."); return false;\">".$permissionsLabel."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD permissions
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='users-tab-icon vtip' title='".t('Users')."'></div><div id='narrow_search_users".$adduserVisibpopupId."' style='position:relative;'><a id='visible-priv-users-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipUsersClassObj."); return false;\">".$userLabel."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD Users
      			// $paintMultiAction .="<div class=\"tab-seperator\"></div><div class='owner-tab-icon vtip' title='".t('Owner')."'></div><div id='".$ownerPopupId."' class='owner-narrow-search' style='position:relative;'><a id='owner-priv-group-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptownerpopup."); return false;\">".$ownerLabel."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD owners
      		}else{
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='permission-disable-icon vtip' title='".t('LBL1035')."'></div><div id='narrow_search_permission".$enrollPopupId."' style='position:relative;'><a id='visible-priv-".$entityId."' class=\"tab-title tab-disable\">".$permissionsLabel."</a><span id='visible-popup-".$entityId."' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD permissions
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='active-deactive-disable-enroll-icon vtip' title='".t('Users')."'></div><div id='narrow_search_users".$adduserVisibpopupId."' style='position:relative;'><a class=\"tab-title tab-disable\" >".$userLabel."</a></div>"; //ADD Users
      			// $paintMultiAction .="<div class=\"tab-seperator\"></div><div class='deactive-owner-tab-icon vtip' title='".t('Owner')."'></div><div id='".$ownerPopupId."' style='position:relative;'><a class=\"tab-title tab-disable\" >".$ownerLabel."</a></div>"; //ADD Owners
      		}
      	} else{
      		if($GLOBALS["user"]->uid == 1 || $sumedit > 0){
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='users-tab-icon vtip' title='".t('Users')."'></div><div id='narrow_search_users".$adduserVisibpopupId."' style='position:relative;'><a id='visible-priv-users-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipUsersClassObj."); return false;\">".$userLabel."</a><span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span></div>"; //ADD Users
      		}else{
      			$paintMultiAction .="<div class=\"tab-seperator\"></div><div class='active-deactive-disable-enroll-icon vtip' title='".t('Users')."'></div><div id='narrow_search_users".$adduserVisibpopupId."' style='position:relative;'><a class=\"tab-title tab-disable\" >".$userLabel."</a></div>"; //ADD Users
      		}
      	}
      }
      else if($entity_multi_action->object_type == "Order_x"){ // Need to remove Gaja
      	$orderId = $entity_multi_action->order_id;
      	$entityType = 'Order';
      	$qtipOrderInit    = $orderId.'_'.$entityType;
        $orderPopupId  = 'qtip_visible_disp_order_'.$qtipOrderInit;
      	$qtipOrderObj    = "{'entityId':".$orderId.",'entityType':'".$entityType."','url':'administration/orderupdate/ajax/".$orderId."','popupDispId':'narrow_search_".$orderPopupId."','catalogVisibleId':'narrow_search_renderOrderId".$qtipOrderInit."','wBubble':250,'hBubble':'auto'}";
      	$paintMultiAction .= "<div class='orderchange-tab-icon vtip' title='".t('LBL943'). ' ' . t('LBL102') ."'></div><div id='narrow_search_".$orderPopupId."'>";
      	if($entity_multi_action->order_status != 'in_checkout' && $entity_multi_action->order_status != 'processing' && $entity_multi_action->order_status != 'canceled'){
      		$paintMultiAction .= "<a class=\"tab-title\" onclick =\"$('#root-admin').data('narrowsearch').getQtipDiv(".$qtipOrderObj."); return false;\">".t('LBL943'). ' ' . t('LBL102') ."</a>";
      	}
      	else{
      		$paintMultiAction .= "<a class=\"tab-title disable-publish-link\" >".t('LBL943'). ' ' . t('LBL102') ."</a>";
      	}
      	$paintMultiAction .= "</div>";
      }
      else{
      	if($entity_multi_action->object_type != "location" && $entity_multi_action->object_type != 'Order'){
      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
      	$paintMultiAction .= "<div class='attachment-tab-icon vtip' title='".t('LBL231')."'></div><div id='narrow_search_qtip_attachment_disp_".$qtipIdInit."'>

      	<a id='visible-tp-attachment-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptAttachmentObj.");\">".$attachmentLabel."</a>
      	<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
      	</div>";
      	}else{
      		$paintMultiAction .= "<div class='attachment-disable-tab-icon vtip' title='".t('LBL231')."'></div><div id='narrow_search_qtip_attachment_disp_".$qtipIdInit."'>

      		<a class=\"tab-title tab-disable\" >".$attachmentLabel."</a>

      		</div>";
      	}
      	}
      	if($entity_multi_action->object_type != "location" && $entity_multi_action->object_type != 'Order'){
      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
	      $paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='tags-tab-icon vtip' title='".t('LBL191')."'></div><div id='narrow_search_".$tagsvisibPopupId."'>
	      <a id='visible-tags-".$entityId."' class=\"tab-title\" onclick =\"callVisibility(".$qtipTagsOptObj."); return false;\">".$tagsLabel."</a>
	      <span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
	      </div>";
      	}else{
      		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='tags-disable-tab-icon vtip' title='".t('LBL191')."'></div><div id='narrow_search_".$tagsvisibPopupId."'>
      		<a class=\"tab-title tab-disable\" >".$tagsLabel."</a>
      		</div>";
      	}
      	}
	      /*if($entityType == 'cre_sys_obt_crs'){
	       $paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='certificate-tab-icon vtip' title='".t('LBL205')."'></div><div id='narrow_search_".$visibCertificatePopupId."'><a class=\"tab-title\" onclick =\"$('#root-admin').data('narrowsearch').getQtipDiv(".$qtipCertOptObj."); return false;\">".t('LBL205')."</a></div>";
	      }*/

	      if($entityType == 'cre_sys_obt_cls'){
	      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
	       $paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='enrollment-tab-icon vtip' title='".t('LBL275')."'></div><div id='narrow_search_enroll".$enrollPopupId."'>
		       <a id='visible-enroll-".$entityId."' class=\"tab-title\" onclick =\"callVisibility(".$qtipEnrollOptObj."); return false;\">".$enrollmentLabel."</a>
		       <span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
		       </div>";
	      	}else{
	      	$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='enrollment-disable-tab-icon vtip' title='".t('LBL275')."'></div><div id='narrow_search_enroll".$enrollPopupId."'>
	      		<a class=\"tab-title tab-disable\">".$enrollmentLabel."</a>
	      		</div>";
	      	}
	      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
	      		$disabecopy = 'copy-tab-icon';
	      	}else{
	      		$disabecopy = 'disable-copy-tab-icon';
	      	}

					if(module_exists("exp_sp_administration_clone") || $view_list[0]['action_link_text'] == "Content Author")
					{
						$createcopyobject = "Class";
						if( $view_list[0]['action_link_text'] == "Content Author")
							$createcopyobject = "contentauthor";
							
						if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
							$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='copy-tab-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a id='manage-dd-arrow' class=\"use-ajax\" onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");'  href='?q=administration/clone/popup/".$entitycourseId."/".$entityId."/".$createcopyobject."'><span class=\"tab-title grey-btn-bg-right-create-copy \"><span class=\"clone-title-container\">".$createCopyLabel."</span></span></a>";
							$paintMultiAction .= "<ul id='ul-clone-list-".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><a class='clone-popup-close' href='javascript:void(0);'></a><div id='manage-clone-list".$entityId."'></div>";
							$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
						}else{
							$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='disable-copy-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a class=\"tab-title tab-disable\" >".$createCopyLabel."</a></div>";
							$paintMultiAction .= "<div><a id='manage-dd-arrow' class='disable-arrow-icon' 'href='javascript:void(0);'>&nbsp;&nbsp;</a>";
							//$paintMultiAction .= "<ul id='manage-clone-list".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><li>";
							//$paintMultiAction .= "<a class='dropdown-title use-ajax course-class-addedit-scroll-wrapper' href='?q=administration/clone/class/enrollment/".$entitycourseId."/".$entityId."/Class/0'>".t('with enrollment')."</a></li><li>";
							//$paintMultiAction .= "<a class='dropdown-title ctools-modal-ctools-admin-course-class-addedit-scroll-wrapper use-ajax' onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");' href='?q=administration/clone/".$entitycourseId."/".$entityId."/Class/0'>".t('Without Enrollment')."</a></li>";
							$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
						}
					}
	      }else if($entityType == 'cre_sys_obt_trp'){
	      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
		       $paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='enrollment-tab-icon vtip' title='".t('LBL275')."'></div><div id='narrow_search_enroll".$enrollPopupId."'>
		       <a id='visible-tp-enroll-".$entityId."' class=\"tab-title\" onclick =\"callVisibility(".$qtipEnrollOptObj."); return false;\">".$enrollmentLabel."</a>
		       <span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
		       </div>"; //TP Enrollments
	      	}else{
	      		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='enrollment-disable-tab-icon vtip' title='".t('LBL275')."'></div><div id='narrow_search_enroll".$enrollPopupId."'>
	      		<a class=\"tab-title tab-disable\">".$enrollmentLabel."</a>
	      		</div>"; //TP Enrollments
	      	}
	       if(module_exists("exp_sp_administration_clone") || $view_list[0]['action_link_text'] == "Content Author")
	       {
	       	$createcopyobject = "TP";
			if( $view_list[0]['action_link_text'] == "Content Author")
				$createcopyobject = "contentauthor";
	       	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
	       		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='copy-tab-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a id='manage-dd-arrow' class=\"use-ajax\" onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");'  href='?q=administration/clone/popup/0/".$entityId."/".$createcopyobject."'><span class=\"tab-title grey-btn-bg-right-create-copy \"><span class=\"clone-title-container\">".$createCopyLabel."</span></span></a>";
	       		$paintMultiAction .= "<ul id='ul-clone-list-".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><a class='clone-popup-close' href='javascript:void(0);'></a><div id='manage-clone-list".$entityId."'></div>";
	      		$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
	       	}else{
	       		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='disable-copy-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a class=\"tab-title tab-disable\"  \">".$createCopyLabel."</a></div>";
	       		$paintMultiAction .= "<div><a id='manage-dd-arrow' class='disable-arrow-icon' 'href='javascript:void(0);' >&nbsp;&nbsp;</a>";
	       		//$paintMultiAction .= "<ul id='manage-clone-list".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><li>";
	       		//$paintMultiAction .= "<a class='dropdown-title ctools-modal-ctools-admin-course-class-addedit-scroll-style use-ajax' onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");' href='?q=administration/clone/".$entityId."//Tp/0'>".t('Without Enrollment')."</a></li>";
	       		$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
	       	}
	       }
	      }
      }

      if($view_list[0]['action_link_text'] == "Content Author")
      {
      	$createcopyobject = "TP";
      	if( $view_list[0]['action_link_text'] == "Content Author")
      		$createcopyobject = "contentauthor";
      	if($sumedit > 0 || $GLOBALS["user"]->uid == 1){
      		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='copy-tab-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a id='manage-dd-arrow' class=\"use-ajax\" onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");'  href='?q=administration/clone/popup/0/".$entityId."/".$createcopyobject."'><span class=\"tab-title grey-btn-bg-right-create-copy\">".$createCopyLabel."</span></a>";
      		$paintMultiAction .= "<ul id='ul-clone-list-".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><a class='clone-popup-close' href='javascript:void(0);'></a><div id='manage-clone-list".$entityId."'></div>";
      		$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
      	}else{
      		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='disable-copy-icon vtip' title='".$createCopyLabel."'></div><div id='narrow_search_enroll".$qtipIdInit."'><a class=\"tab-title tab-disable\"  \">".$createCopyLabel."</a></div>";
      		$paintMultiAction .= "<div><a id='manage-dd-arrow' class='disable-arrow-icon' 'href='javascript:void(0);' >&nbsp;&nbsp;</a>";
      		//$paintMultiAction .= "<ul id='manage-clone-list".$entityId."' class='manage-clone-list'><div class='manage-dd-list-arrow'></div><li>";
      		//$paintMultiAction .= "<a class='dropdown-title ctools-modal-ctools-admin-course-class-addedit-scroll-style use-ajax' onclick='$(\"#root-admin\").data(\"narrowsearch\").createLoader(\"paint-narrow-search-results\");' href='?q=administration/clone/".$entityId."//Tp/0'>".t('Without Enrollment')."</a></li>";
      		$paintMultiAction .= "<div class='clearBoth'></div></ul></div>";
      	}
      
    /*  	if($sumedit > 0 || $GLOBALS["user"]->uid == 1)
      	{
      		if($entity_multi_action->status_code == "lrn_cnt_sts_itv")
      		{
      			//if not published, inactive the share
      			$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='disable-share-tab-icon vtip' title='".t('Share')."'></div><div id='narrow_search_qtip_share_disp_".$qtipIdInit."'>
      			<a id='share-visibility-".$entityId."' class=\"tab-title tab-disable\" onclick = \"javascript:void(0);\">".t("Share")."</a>
      			<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
      			</div>";
      		}
      		else
      		{
      			$paintMultiAction .= "<div class='tab-seperator'></div><div class='share-tab-icon vtip' title='".t('Share')."'></div><div id='narrow_search_qtip_share_disp_".$qtipIdInit."'>
      			<a id='share-visibility-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptShareObj.");\">".t("Share")."</a>
      			<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
      			</div>";
      		}
      	}
      	else
      	{
      		$paintMultiAction .= "<div class=\"tab-seperator\"></div><div class='disable-share-tab-icon vtip' title='".t('Share')."'></div><div id='narrow_search_qtip_share_disp_".$qtipIdInit."'>
      		<a id='share-visibility-".$entityId."' class=\"tab-title tab-disable\" onclick = \"javascript:void(0);\">".t("Share")."</a>
      		<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
      		</div>";
      		 
      	} */
      
      
      	 
      }
      
      
      if($entity_multi_action->object_type != "location" ){
      if($sumedit > 0 || $GLOBALS["user"]->uid == 1 || !isset($sumedit)){
      	if($entity_multi_action->object_type != 'Order'){
      		$paintMultiAction .= "<div class='tab-seperator'></div>";
      	}
     	$paintMultiAction .= "<div class='access-tab-icon vtip' title='".t('LBL642')."'></div><div id='narrow_search_qtip_visible_disp_".$qtipIdInit."'>
     	<a id='access-visibility-".$entityId."' class=\"tab-title\" onclick = \"callVisibility(".$qtipOptAccessObj.");\">".$accessLabel."</a>
     	<span id='visible-popup-".$entityId."' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
     	</div>";
      }else{
      	$paintMultiAction .= "<div class='tab-seperator'></div><div class='access-disable-tab-icon vtip' title='".t('LBL642')."'></div><div id='narrow_search_qtip_visible_disp_".$qtipIdInit."'>
      	<a class=\"tab-title tab-disable\" >".$accessLabel."</a>
      	</div>";
      }
      }

		$paintMultiAction .= "</div>";
		
      if($object_type != 'Grp') { print $paintMultiAction;}
    ?>
  <?php endif; ?>
  <?php

  ?>
  <?php if (!empty($new_action_list)): ?>
  <?php $countvalues=count($new_action_list);?>
    <div class='narrow-search-results-item-action-list-user narrow-search-results-item-action-list'>
    <?php //expDebug::dPrint('In narrow search result item details tpl file :  $action_list = ' . print_r($action_list, true),4); ?>
      <?php

      $newActionCount = count($new_action_list);
      $newActionLoop = 1;
      foreach ($new_action_list as $item):
      if(empty($item['action_button_params'])){
      	$item['icon_style_class'] = 'active-deactive-disable-msg-icon';
      }
      if($sumedit <= 0 && $GLOBALS["user"]->uid != 1 && arg(1) != 'commerce' && isset($sumedit)){
      	switch($item['action_link_text']){
      		case t('LBL572') : // Suspend
      			$item['icon_style_class'] = 'active-deactive-disable-suspend-icon';
      		break;
      		case t('LBL573'): // Activate
      			$item['icon_style_class'] = 'active-deactive-disable-activate-icon';
      		break;
      		case t('Reset user password') : // Reset user password
      			$item['icon_style_class'] = 'active-deactive-disable-reset-icon';
      		break;
       	/*	Ticket No: 0068084 
       	    case t('LBL591') : // Send message to user
      			$item['icon_style_class'] = 'active-deactive-disable-msg-icon';
      		break;      	  
         	case 'Enrollments' :
      			$item['icon_style_class'] = 'active-deactive-disable-enroll-icon';
      		break; */
      		case t('LBL642') : // Access
      			$item['icon_style_class'] = 'active-deactive-disable-access-icon';
      		break;
      	}
      } else if($sumedit == 0 && $GLOBALS["user"]->uid != 1 && arg(1) == 'commerce' && $item['action_link_text'] == "Access") {  // Ticket : 0068210      	 
      	$item['icon_style_class'] = 'active-deactive-disable-access-icon';      		 
      } else{
      	$item['icon_style_class'] = $item['icon_style_class'];
      }
      if($object_type == 'Grp'){//Disable suspend/activate icon when background job is running
          $checkStatus = array ('new','inprogress');
          $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$entityId);
          if($back_gd_running){
              if($item['action_link_text'] == t('LBL572')){
                  $item['icon_style_class'] = 'active-deactive-disable-suspend-icon';
              }
              else if($item['action_link_text'] == t('LBL573')){
                  $item['icon_style_class'] = 'active-deactive-disable-activate-icon';
              }
          }
      }
      //44133: German-old-ui-Icon not changing when user is activate and suspend
      ?>
      	<div class="narrow-search-multi-action-container">
        	<div id='action_icon_tooltip_<?php print $item['action_button_params']; ?>' class="<?php print $item['icon_style_class'];?>  vtip" title="<?php print $item['tooltip'];?>"></div>
        	<?php if($action_list[0]['action_button_params_module'] == 'assessment' || $action_list[0]['action_button_params_module']=='survey') { ?>
        	<div class="survey-assment-access">
        	<?php }else{?>
        			<div>
              <?php }
              if (!empty($item['action_page_path'])) {
        		   $action_page_path = $item['action_page_path'];
          	   $action_page_path .= '/' .  $item['action_button_params'];
          	   $action_page_path .= !empty($item['action_button_params_child']) ? ('/' .  $item['action_button_params_child']) : '';
              }

        		$disableClass = '';
        		if($item['action_button_params_module'] == "assessment") {
        		    $maxAssessmentScore = maxScoreValidate($item['action_button_params']);
        		    //print "---".$maxAssessmentScore;
            		if($maxAssessmentScore != 0) {
            		  $disableClass = 'disable-publish-link';
            		}else{
            		  $disableClass = '';
            		}
        		}
        		if($object_type == 'Grp'){//Disable suspend/activate lbl when background job is running
        		    $checkStatus = array ('new','inprogress');
        		    $back_gd_running = fetchBackgroundListForGroups($checkStatus,'inlist',$entityId);
        		    if($back_gd_running){
        		        $disableClass = 'disable-publish-link';
        		    }
        		}

      		?>
            
            
            <?php if($item['action_button_params_text'] == 'show_in_screen' && module_exists('exp_sp_administration_customattribute')){ 

                                expDebug::dPrint('details    >>= ' . print_r($item, true),4);
                                 
                                $entityId= $item['entityId'];
                                $entityType = $item['entityType'];
                                $qtipIdInit        = $entityId.'_'.$entityType;
                                $entityTypeAccess  = $entityType;
                                $postype = $item['postype'];
                                $poslwid = $item['poslwid'];
                                $actionparams = $item['action_button_params'];                               
                                
                                ?>
                                <?php if($entityType == 'cre_sys_obt_cattr'){
                                    $popWidth = 280;
                                }?>



                                        <div id='qtip_showinscreen_disp_<?php print $item['entityId']; ?>'>
                                        <?php $qtipOptShowInScreentObj   = "{'entityId':'".$entityId."',
                                                                                                    'entityType':'".$entityType."',
                                                                                                    'url':'administration/manage/customattribute/shortcut/showinscreen/".$actionparams."',
                                                                                                    'popupDispId':'qtip_showinscreen_disp_".$entityId."',
                                                                                                    'catalogVisibleId':'qtip_showinscreen_disp_".$qtipIdInit."',
                                                                                                    'wid':'".$popWidth."','heg':'210','postype':'topleft','poslwid':'','qdis':'',
                                                                                                    'linkid':'showinscreen-".$entityId."'}"; ?>
            <?php   }?> 
            
            
      		<?php
      		 if (empty($item['action_page_path']) && $item['action_button_params_text'] == 'access'){
							$emptyId = 0;
							$entityId= core_encrypt($item['entityId']);
							$entityType = $item['entityType'];
							$qtipIdInit        = $entityId.'_'.$entityType;
							$entityTypeAccess  = $entityType;
							$postype = $item['postype'];
							$poslwid = $item['poslwid'];

							$qtipOptAccessObj      = "{'entityId':'".$entityId."',
																				 'entityType':'".$entityTypeAccess."',
																				 'url':'administration/catalogaccess/".$entityId."/".$entityTypeAccess."/".$emptyId."',
																				 'popupDispId':'narrow_search_qtip_visible_disp_".$qtipIdInit."',
																				 'catalogVisibleId':'narrow_search_qtipAccessqtip_visible_disp_".$qtipIdInit."',
																				 'wid':660,
																				 'heg':'300',
																				 'postype':'".$postype."',
																				 'poslwid':'".$poslwid."',
																				 'onClsFn':'$(\'#root-admin\').data(\'accessgroup\').accessClose',
																				 'linkid':'access-visibility-".$entityId."'}";
/* Ticket No: 0046992 */
$emptyId           = microtime(false);
$qtipTagsIdInit    = $entityId.'_'.$entityTypeAccess;
$tagsvisibPopupId  = 'qtip_visible_disp_tags_'.$qtipTagsIdInit;

if($theme_key == 'expertusoneV2') {
	$qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'".$entityTypeAccess."','url':'administration/catalog-tags/ajax/".$entityId."/".$entityTypeAccess."/".$emptyId."','popupDispId':'narrow_search_".$tagsvisibPopupId."','catalogVisibleId':'narrow_search_renderTagsId".$qtipTagsIdInit."','wid':400,'heg':'150','postype':'middle','poslwid':'1','qdis':'','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
} else {
	$qtipTagsOptObj    = "{'entityId':'".$entityId."','entityType':'".$entityTypeAccess."','url':'administration/catalog-tags/ajax/".$entityId."/".$entityTypeAccess."/".$emptyId."','popupDispId':'narrow_search_".$tagsvisibPopupId."','catalogVisibleId':'narrow_search_renderTagsId".$qtipTagsIdInit."','wid':400,'heg':'140','postype':'middle','poslwid':'','qdis':'','linkid':'visible-tags-".$entityId."','scrollid':'tag-scroll-id'}";
}
/* Ticket No: 0046992 */
      //}?>
      	<div id="narrow_search_qtip_visible_disp_<?php print $qtipIdInit;?>">
      	<?php }?>

      	<?php if($sumedit > 0 || !isset($sumedit) || $GLOBALS["user"]->uid == 1 || $item['action_button_params_text'] == 'enrollments' || $item['action_button_params_text'] == 'send_message' || $item['action_button_params_text'] =='show_in_screen'){?>
      	<?php if($item['action_button_params_text'] == 'enrollments'){  ?>
						<div id='qtip_enrollment_disp_<?php print core_encrypt($item['userId']); ?>'>
						<?php $qtipOptEnrollmentObj   = "{'entityId':'".core_encrypt($item['userId'])."',
																					'entityType':'".$item['action_button_params_text']."',
																					'url':'administration/catalogaccess/".core_encrypt($item['userId'])."',
																					'popupDispId':'qtip_enrollment_disp_".core_encrypt($item['userId'])."',
																					'catalogVisibleId':'narrow_search_qtipAccessqtip_visible_disp_".$qtipIdInit."',
																					'wid':670,'heg':'200','postype':'bottomright','poslwid':'','qdis':'',
																					'linkid':'user-enrollments-".core_encrypt($item['userId'])."'}"; ?>
					<?php	}?>
					<a
						class="tab-title <?php print $disableClass; ?>
						<?php if (!empty($item['action_page_path'])) print $item['ctools_style'].' use-ajax'; print empty($item['action_button_params']) ? 'disable-delete-link' : '';
  			if($item['action_button_params_text'] == 'deactivate' && $item['action_org_users_count'] != 0 && $sumedit <= 0){ print 'disable-suspend'; }
  			if($item['action_button_params_text'] == 'orgusers' && $item['action_org_users_count'] == 0 /* && $sumedit <= 0 */){ print 'disable-suspend'; }?>"
  			<?php if (!empty($item['action_page_path'])): ?>
						href='/?q=<?php print $action_page_path; ?>'
						data-wrapperid="paint-narrow-search-results" <?php endif; ?>
						<?php if (empty($item['action_page_path'])){  ?>
      <?php if($item['action_button_params_text'] == 'deactivate' || $item['action_button_params_text'] == 'activate'){ ?>
						id='suspend_activate_<?php print $item['action_button_params']; ?>'
						<?php if($disableClass == '') {?>
						<?php if($item['action_org_users_count'] == 0  ){ if($object_type == 'Grp'){
						if($grp_code != 'grp_sup') {?>
						onclick='$("#root-admin").data("narrowsearch").publishAndUnpublishGroupDialog("<?php print $item['action_button_params']; ?>", "<?php print $object_type; ?>", this,"<?php print $item['is_admin']; ?>","<?php print $item['owner_cnt']; ?>");'
						<?php } } else { ?>
						onclick='$("#root-admin").data("narrowsearch").activateAndDeactivateObject("<?php print $item['action_button_params']; ?>", "<?php print $object_type; ?>", this);'
						
						<?php }}} ?>
						<?php } else if($item['action_button_params_text'] == 'send_message'){?>
						<?php print !empty($item['action_button_params']) ? (" href='mailto:".$item['action_button_params']."'") : ''; ?>
						<?php }else if($item['action_button_params_text'] == 'reset_password'){ ?>
						id='reset_password'
						onclick='$("#root-admin").data("narrowsearch").resetUserPassword("<?php print $item['action_button_params']; ?>", "<?php print $item['action_button_user_id']; ?>", this);'
						<?php }else if($item['action_button_params_text'] == 'published'){ ?>
						id='status-published'
						<?php }else if($item['action_button_params_text'] == 'enrollments'){

							?>
						id="user-enrollments-<?php print core_encrypt($item['userId']);?>"
						onclick="$('body').data('mulitselectdatagrid').loadUserEnrollmentGrid(<?php print $qtipOptEnrollmentObj?>);"
                                   
                            <?php }else if($item['action_button_params_text'] == 'show_in_screen' && module_exists('exp_sp_administration_customattribute')){?> 
                        id='showinscreen-<?php print $entityId;?>'
                        onclick="callVisibility(<?php print $qtipOptShowInScreentObj?>);"
						<?php }else if($item['action_button_params_text'] == 'orgusers'){ ?>
						id='user-orgusers'
						<?php if($item['action_org_users_count'] != 0 ) {?>
						onclick='$("body").data("mulitselectdatagrid").loadOrgUsersListGrid("<?php print $item['orgid'];?>");'
						<?php } ?>
						<?php }else if($item['action_button_params_text'] == 'access'){ ?>
						id='access-visibility-<?php print $entityId;?>'
						onclick="callVisibility(<?php print $qtipOptAccessObj?>);"
						<?php } else { ?>
						onclick='$("<?php print '#' . $item['js_object_info']['init_id']; ?>").data("<?php print $item['js_object_info']['name']; ?>").editItem(<?php print $item['action_button_params']; ?>);'
						<?php }
				 } ?>> <?php print $item['action_link_text']; ?>
					</a>
					<?php if($item['action_button_params_text'] == 'enrollments'){  ?>
						<span id="visible-popup-<?php print core_encrypt($item['userId']);?>" class="qtip-popup-visible" style='display:none; position:absolute; left:0px; top:0px;'></span>
						</div>
                                
                                                    <?php } else if($item['action_button_params_text'] == 'show_in_screen' && module_exists('exp_sp_administration_customattribute')){ ?>
                                <span id="visible-popup-<?php print $item['entityId'];?>" class="qtip-popup-visible" style='display:none; position:absolute; left:0px; top:0px;'></span>
                                </div>
					<?php	}?>
					<?php }else{
                        
						?>
						<a
						class="tab-title tab-disable tab-title-float-left <?php print $disableClass; ?>
						<?php if (!empty($item['action_page_path'])) print $item['ctools_style'].' use-ajax'; print empty($item['action_button_params']) ? 'disable-delete-link' : '';
  			if($item['action_button_params_text'] == 'deactivate' && $item['action_org_users_count'] != 0 ){ print 'disable-suspend'; }
  			if($item['action_button_params_text'] == 'orgusers' && $item['action_org_users_count'] == 0 ){ print 'disable-suspend'; }?>"
  			<?php if (!empty($item['action_page_path'])): ?>
						data-wrapperid="paint-narrow-search-results" <?php endif; ?>
						<?php if (empty($item['action_page_path'])){ ?>
      <?php if($item['action_button_params_text'] == 'deactivate' || $item['action_button_params_text'] == 'activate'){ ?>
						id='suspend_activate_<?php print $item['action_button_params']; ?>'
						<?php } else if($item['action_button_params_text'] == 'send_message'){?>
						<?php //print !empty($item['action_button_params']) ? (" href='mailto:".$item['action_button_params']."'") : ''; ?>
						<?php }else if($item['action_button_params_text'] == 'reset_password'){ ?>
						id='reset_password'
						<?php }else if($item['action_button_params_text'] == 'published'){ ?>
						id='status-published'
						<?php }else if($item['action_button_params_text'] == 'enrollments'){ ?>
						id='user-enrollments'
						<?php }else if($item['action_button_params_text'] == 'orgusers'){ ?>
						id='user-orgusers'
						<?php if($item['action_org_users_count'] != 0 ) {?>
						<?php } ?>
						<?php }else if($item['action_button_params_text'] == 'access'){ ?>
						id='access-visibility'
						<?php } else {?>

						<?php }
				 } ?>> <?php print $item['action_link_text']; ?>
					</a>
						<?php
					}?>
					<?php if (empty($item['action_page_path']) && $item['action_button_params_text'] == 'access'){?>
					<span id="visible-popup-<?php print $entityId;?>" class="qtip-popup-visible" style="display:none; position:absolute; left:0px; top:0px;"></span>
      		</div>
      		<?php }?>
          </div>
         	 <?php /* Ticket No: 0046992 */         
          if($entityTypeAccess == 'sry_det_typ_ass_qus' || $entityTypeAccess == 'sry_det_typ_sry_qus'){     
          	      
          	$tagsLabel = t('LBL191'); // Tags
          if($sumedit > 0 || $GLOBALS["user"]->uid == 1){ ?>
          <div class="tab-seperator"></div><div title="<?php print $tagsLabel; ?>" class="tags-tab-icon vtip"></div>
          <div id="narrow_search_qtip_visible_disp_tags_<?php print $qtipTagsIdInit;?>">
	      <a onclick="callVisibility(<?php print $qtipTagsOptObj; ?>); return false;" class="tab-title" id="visible-tags-<?php print $entityId;?>"><?php print $tagsLabel; ?></a>
	      <span style="display:none; position:absolute; left:0px; top:0px;" class="qtip-popup-visible" id="visible-popup-<?php print $entityId;?>"></span>
	      </div>
          <?php } else{ /* Ticket no : 0068042 */
          		?><div class="tab-seperator"></div><div class='tags-disable-tab-icon vtip' title="<?php print $tagsLabel; ?>"></div><div id='narrow_search_<?php print $qtipTagsIdInit?>'>
          		<a class="tab-title tab-disable" ><?php print $tagsLabel; ?></a>
          		</div><?php 
          	}          	 
          }	/* Ticket No: 0046992 */	 ?>  

          <?php if($entity_multi_action->object_type == "location"){ if($sumedit > 0 || $GLOBALS["user"]->uid == 1 || !isset($sumedit)){ ?>
          <div class='tab-seperator'></div><div class='access-tab-icon vtip' title='<?php print t('LBL642');?>'></div>

          <div><div id='narrow_search_qtip_visible_disp_<?php print $qtipIdInit;?>'>
     	<a id='access-visibility-<?php print $entityId;?>' class="tab-title" onclick = "callVisibility(<?php print $qtipOptAccessObj;?>);"><?php print $accessLabel;?></a>
     	<span id='visible-popup-<?php print $entityId;?>' class='qtip-popup-visible' style='display:none; position:absolute; left:0px; top:0px;'></span>
     	</div></div>
     	<?php }else{?>

     	<div class='tab-seperator'></div><div class='access-disable-tab-icon vtip' title='<?php print t('LBL642');?>'></div>

     	<div><div id='narrow_search_qtip_visible_disp_<?php print $qtipIdInit;?>'>
      	<a class="tab-title tab-disable" ><?php print $accessLabel;?></a>
      	</div></div>
     	<?php } }?>
          <?php if($newActionLoop < $newActionCount){?>
         	<div class="tab-seperator"></div>
          <?php }
         	  $newActionLoop++;
           ?>
        </div>
        <?php if($object_type == 'Grp') { print $paintMultiAction;}?>

	    <!-- Added by Velu start -->
	    	    <?php if($item['action_button_params_text'] == 'orgusers'){?>
	    <div id="admin-data-grid" style="position:absolute;">
	    <div id="datagrid-div-<?php print $item['orgid'];?>-orgusers" class='orgusers' style='display:none'>
  		    <div class="user-enrollment-div-cls orgusers" id="org-users-main-div-<?php print $item['orgid'];?>" style="display:none;position:absolute; left:0px;">
  		    <a class="qtip-close-button" onclick="$('#org-users-main-div-<?php print $item['orgid'];?>').hide();"></a>
  		    <div class='bottom-qtip-tip active-qtip-div'></div>
  		    	<table cellspacing="0" cellpadding="0" id="bubble-face-table">
  		    		<tbody>
  		    			<tr>
  		    				<td class="bubble-tl"></td>
  		    				<td class="bubble-t"></td>
  		    				<td class="bubble-tr"></td>
  		    			</tr>
  		    			<tr>
  		    				<td class="bubble-cl"></td>
  		    				<td valign="top" class="bubble-c">
								<div class= "filter-search-start-date-left-bg enroll-search-wrapper-div"></div>
								<span style="display: block;" class="narrow-text-container-cls user-enroll-input-auto-cls" id="user-<?php print $item['orgid'];?>-orgusers-search-box">
									<input onblur="textfieldTitleBlur(this,'<?php print t('LBL1230');?>');" onfocus="textfieldTitleClick(this, '<?php print t('LBL1230');?>');" type="text" autocomplete="off" alt="<?php print t('Users').' '.t('LBL304');?>" maxlength="75" size="59" value="<?php print t('LBL1230');?>" class="ac_input narrow-search-ac-text-overlap header-search-text-filter input-field-grey" id="org-users-autocomplete-<?php print $item['orgid'];?>">
  									<a onclick="$('body').data('mulitselectdatagrid').searchOrgUsersDataGrid(<?php print $item['orgid'];?>);" id="search-user-orgusers-<?php print $item['orgid'];?>" title="<?php print t('Users').' '.t('LBL304');?>" class="narrow-text-search">&nbsp;</a>
								</span>
								<div class= "filter-search-start-date-right-bg enroll-search-wrapper-div"></div>
								<div id='exportcontainer' class='exportcontainer' ><a onclick="$('body').data('mulitselectdatagrid').callOrgUsersExportProcess('<?php print $item['orgid'];?>');" class='enrollments-exports-icon' title="<?php print t('LBL309');?>"></a></div>
								<div id="datagrid-div-inside-grid-<?php print $item['orgid'];?>-orgusers" class="user-enroll-grid-div">
									<table id="datagrid-container-<?php print $item['orgid'];?>-orgusers" class="datagrid-container-common"></table>
									<div id="pager-datagrid-<?php print $item['orgid'];?>-orgusers" class="pager-datagrid-common"></div>
									<div id="datagrid-div-<?php print $item['orgid'];?>-orgusers">
    									<div class="user-enrollment-bottom-row-cls">
    									    <span class="user-enrollment-count-pipe-cls" id="user-enrollments-status-count-div-<?php print $item['userId'];?>"></span>
    										<span><a class="user-enroll-close-link" onclick="$('#org-users-main-div-<?php print $item['orgid'];?>').hide();"><?php print t('LBL123');?></a></span>
    									</div>
									</div>
								</div>
  		    				</td>
  		    				<td class="bubble-cr"></td>
  		    			</tr>
              		    <tr>
              		    	<td class="bubble-bl"></td>
              		    	<td class="bubble-blt"></td>
              		    	<td class="bubble-br"></td>
              		    </tr>
          		  </tbody>
          		</table>
  		    </div>
  		</div>
	    </div>
	    <?php } ?>
	    <!-- Added by Velu end -->

    <?php endforeach; ?>
    </div>
  <?php endif; ?>

<!--
  <?php //if (!empty($action_list)): ?>
  <?php //$countvalues=count($action_list);?>
    <?php //$showPipe = false; ?>
    <div class="narrow-search-results-item-action-list">
    <?php //expDebug::dPrint('In narrow search result item details tpl file :  $action_list = ' . print_r($action_list, true),4); ?>
      <?php //foreach ($action_list as $item): ?>
        <?php //if ($showPipe): ?>
          <span class="narrow-search-results-item-detail-pipe-line">|</span>
        <?php //endif; ?>
        <span class="vtip" title="<?php //print  $item['tooltip']; ?>">
		    <?php
/*		      if (!empty($item['action_page_path'])) {
		      		   $action_page_path = $item['action_page_path'];
		        	   $action_page_path .= '/' .  $item['action_button_params'];
		        	   $action_page_path .= !empty($item['action_button_params_child']) ? ('/' .  $item['action_button_params_child']) : '';
		      		}*/
		    ?>
			<a class="narrow-search-results-item-action-list-btn addedit-form-expertusone-throbber  <?php //if (!empty($item['action_page_path'])) print $item['ctools_style'].' use-ajax'; ?>"
				<?php //if (!empty($item['action_page_path'])): ?> href='/?q=<?php //print $action_page_path; ?>'  data-wrapperid="paint-narrow-search-results" <?php //endif; ?>
				<?php //if (empty($item['action_page_path'])){ ?>

				<?php //if ($item['action_link_text'] == 'Roster') {?>
					<?php //if(isset($item['entity_type']) && ( $item['entity_type'] == 'cre_sys_obt_trn' || $item['entity_type'] == 'cre_sys_obt_crt' || $item['entity_type'] == 'cre_sys_obt_cur' ) ){ ?>
						onclick='$("body").data("adminprogramroster").launchRoster("<?php //print $item['action_button_params']; ?>", "<?php //print $item['entity_type']; ?>", "<?php //print $displayTitle; ?>");'
					<?php //} else { ?>
						onclick='$("body").data("admincatalogroster").launchRoster("<?php //print $item['action_button_course_id']; ?>","<?php //print $item['action_button_class_id']; ?>", "<?php //print $displayTitle; ?>");');'
					<?php //} ?>
				<?php //} else if ($item['action_link_text'] == 'Share') {?>
				onclick='$("body").data("refercourse").getReferDetails("<?php //print $item['action_button_params']; ?>","<?php //print $item['entity_type'];?>","paint-narrow-search-results","<?php //print $shareTitle; ?>");'
				<?php //} else if ($item['action_link_text'] == "Launch Report") { ?>
				onclick='$("<?php //print '#' . $item['js_object_info']['init_id']; ?>").data("<?php //print $item['js_object_info']['name']; ?>").luanchReport(<?php //print $item['action_button_params']; ?>);'
				<?php
				//} else { ?>
				onclick='$("<?php //print '#' . $item['js_object_info']['init_id']; ?>").data("<?php //print $item['js_object_info']['name']; ?>").editItem(<?php //print $item['action_button_params']; ?>);'
				<?php //}?>

				<?php //} ?>>
				<?php //print $item['action_link_text']; ?>
	        </a>
        </span>
        <?php //$showPipe = true; ?>
    <?php //endforeach; ?>
    </div>
  <?php //endif; ?> -->

<div class="clearBoth"></div>
</div>
<?php
$linkText = isset($item['action_link_text'])?$item['action_link_text']:'';
expDebug::dPrint("action_listaction_list -- ".print_r($action_list,true),4);
if ($linkText == "Launch Report") {
	 echo "<div id='narrow-report-launch-id-$item[action_button_params]' class='narrow-report-launch-class'></div>";
}
?>
