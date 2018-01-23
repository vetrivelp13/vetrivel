<div class="top-record-div-left">
	<?php
	
	if(isset($tpresults1)) {
	// Training program level prerequisite - Shows list of cert/curr/lp/course assigned to TP
		$parentId		= arg(2);

		$prereqtype 	= (!empty($tpresults1->prereqtype) ? $tpresults1->prereqtype : 'course');//.'l1';
		$passParams 	= "{'prereqType':'".$prereqtype."'}";//premsenrstatus premsenrstatus preMsEnrStatus
		$preMsEnrStatus = ($prereqtype == 'course') ? $tpresults1->preenrstatus : $tpresults1->preenrstatus;
		$enrollStatus 	= (($tpresults1->enrollstatus == 'N') ? t("Not Registered") : t("Registered"));
		$enrollStatus 	= (($regstatus == 'N') ? t("Not Registered") : $regstatus);//t("Registered"));
		$enrollStatus	= ($enrollStatus == "Enrolled") ? t("Registered") : $enrollStatus;
		$enrollPrice	= ($tpresults1->price == 0) ? "0.00" : $tpresults1->price;
		$enrollRegType	= (module_exists("exp_sp_commerce") && ($tpresults1->price > 0)) ? "cart" : "register";
		
		$header_details->delivery_type_code = $prereqtype;
		$header_details->icon_class = getTypeImageClass($prereqtype);
		$header_details->icon_title = $tpresults1->pretype;
		$header_details->data_title = $tpresults1->pretitle;

		$header_details->attributes['precode']->tooltip = $tpresults1->precode;
		$header_details->attributes['precode']->data = titleController('EXP-SP-PREREQUISITE-TP-CODE',$tpresults1->precode, 10);
		
		$header_details->attributes['pretype']->tooltip = t($tpresults1->pretype);
		$header_details->attributes['pretype']->data = t($tpresults1->pretype);
		
		$header_details->attributes['enroll_status']->tooltip = $enrollStatus;
		$header_details->attributes['enroll_status']->data = $enrollStatus;
		
		/* code for checking if equivalence is completed or registered */
		if(!empty($tpresults1->equvstatus) && $tpresults1->equvstatus != "Zero") {
			$header_details->attributes['equvstatus']->tooltip = $tpresults1->equvstatus;
			$header_details->attributes['equvstatus']->data = $tpresults1->equvstatus;
		}
		$header_details->attributes['tpaction']->data = "<input type='hidden' id='tpaction-".$tpresults1->preid."' name='tpaction-".$tpresults1->preid."' value='".$enrollRegType.'-'.$tpresults1->node_id."'>";
		$header_details->attributes['tpaction']->hidden = true;
		
		/* // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ */
		/* if($enrollPrice=='0.00')
			$dispData .= (($tpresults1->pretype != 'Course' ) ? ((module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>") : '')) : '')."</div></td></tr>";
		else
			$dispData .= (($tpresults1->pretype != 'Course' ) ? ((module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>$ ".$enrollPrice) : '')) : '')."</div></td></tr>"; */
		if($enrollPrice != '0.00' && module_exists("exp_sp_commerce") && $tpresults1->pretype != 'Course') {
			$header_details->attributes['enroll_price']->data = "$ ".$enrollPrice;
		}
		$header_details->data_desc = $tpresults1->shortdesc;
		$header_details->pre_req_show_more = "<div class='show_more_less class-content-more padbt5' id='preTitle".$prereqtype.$tpresults1->preid."l1' data=\"data=".$passParams."\" onclick=\"$('body').data('prerequisite').getTpPrereqCatalog('".$tpresults1->preid."','l1','".$prereqtype."','l1',".$parentId.")\" href=\"javascript:void(0);\">".t('LBL713')."</div>";
		$preReqTpDetails = theme("content_header", array("result"=>$header_details, 'context' => ['callfrom' => 'tp_prereq']));
		
		print $preReqTpDetails;
		
	} else if(isset($tpresults2)) {
			// shows list of courses - Bring into list of courses respect to the TP
			// print_r($register_action);
		if(isset($tpresults2->buttonArr)) {
			
			if($tpresults2->totRec > 0) {
				if($parentRegStatus == 'N') {
					if($tpresults2->regOption == 'tp') {
						$dispData .= "<table border='0' cellpadding='0'  cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;'><span style='float:right;' class='action-btn'>";
						$dispData .= "<table border='0' class='search-register-btn' align='right'><tbody><tr>";
						$dispData .= "<td style='border-bottom:0px;' onclick=\"$('body').data('prerequisite').getTpPrereqCatalogRegister('" . $tpCrsId . "','" . $program_id . "','" . t('ERR066') . "')\" class='prereq-action-container' class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>" . t("Register") . "</span><div class='admin-save-button-right-bg'></div></div></td>";
						$dispData .= "</tr></table></span></td></tr></table>";
					} else {
						$userId = getSltpersonUserId();
						$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;padding-top:0px;'><span style='float:right;'>";
						// <td style='border-bottom:solid 0px #ccc;'><span style='float:right;'>".$tpclassresults['class_details']->crs_id."<input type='button' class='action-btn' value='Done' onclick=\"$('body').data('learningcore').callRegisterClass('lnr-catalog-search',".$userId.",".$tpCrsId.",".$program_id.")\"></span></td></tr></table>";
						$dispData .= "<table border='0' class='search-register-btn' align='right'><tbody><tr>";
						$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('body').data('prerequisite').getPrereqCatalogRegister('" . $tpCrsId . "','" . $program_id . "','" . t('ERR067') . "')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>" . t("Register") . "</span><div class='admin-save-button-right-bg'></div></div></td>";
						$dispData .= "</tr></table></span></td></tr></table>";
					}
				}
			} else {
				$dispData .= t("ERR054");
			}
		} else {
			$prereqtype = 'course';
			$passParam = 'l1-' . arg(2);
			$parentId = arg(2);
			$mandatory = ""; // ($tpresults2->mandatory =='N') ? t('Optional') : t('Mandatory');
			$passParams = "{'prereqType':'" . $prereqtype . "'}";
			$enrollStatus = (($regstatus == 'N') ? t("Not Registered") : $regstatus); // t("Registered"));
			$enrollStatus = ($enrollStatus == "Enrolled") ? t("Registered") : t($enrollStatus);
			
			// below check implemented by Vincent for #0027337
			// TODO: Need to fix this issue in IE9 and remove the below check
			$ie9Browser = (ereg('MSIE 9', $_SERVER['HTTP_USER_AGENT'])) ? true : false;
			if($ie9Browser == 1) {
				$class = '';
			} else {
				$class = 'vtip';
			}
			
			$ntpLevel = (isset($tpresults2->node_id) ? "l3" : "l2");
			$showLevel = "l2";
			
			$header_details->delivery_type_code = $prereqtype;
			$header_details->icon_class = getTypeImageClass($prereqtype);
			$header_details->icon_title = $tpresults1->pretype;
			$header_details->data_title = $tpresults2->pretitle;
			
			$header_details->attributes['precode']->tooltip = sanitize_data($tpresults2->precode);
			$header_details->attributes['precode']->data = titleController('EXP-SP-PREREQUISITE-COURSE-CODE', $tpresults2->precode, 10);
			
			$header_details->attributes['enroll_status']->tooltip = $enrollStatus;
			$header_details->attributes['enroll_status']->data = $enrollStatus;
			$header_details->attributes['enroll_status']->id = 'clsStatus-' . $tpresults2->id;
			
			/* code for checking if equivalence is completed or registered */
			if(! empty($tpresults2->equvstatus) && $tpresults2->equvstatus != "Zero") {
				$header_details->attributes['equvstatus']->id = 'crsEquvStatus-' . $tpresults2->id;
				$header_details->attributes['equvstatus']->data = $tpresults2->equvstatus;
				$header_details->attributes['equvstatus']->tooltip = $tpresults2->equvstatus;
			}
			$header_details->data_desc = $tpresults2->short_description;
			$header_details->pre_req_show_more = "<div class='show_more_less class-content-more padbt5' id='preTitle" . $prereqtype . $tpresults2->id . "l2' data=\"data=" . $passParams . "\" onclick=\"$('body').data('prerequisite').getTpPrereqCatalog('" . $tpresults2->id . "','" . $ntpLevel . "','" . $prereqtype . "','" . $showLevel . "'," . $parentId . ")\" href=\"javascript:void(0);\">" . t('LBL713') . "</div>";
			$header_details->content_row_class = ' tp-prereq-select-course-level-container';
		}
		expDebug::dPrint('$header_details '.print_r($header_details, 1), 1);
		if(isset($header_details)) {
			$dispData = theme("content_header", array("result" => $header_details, 'context' => ['callfrom' => 'tp_prereq']));
		}
		print $dispData;
	} else if(isset($tpclassresults)) {
 		// To list the class details 
 		//print_r($register_action);
		if($totalRec > 0) {
			if(isset($tpclassresults["buttonArr"])) {
				if($tpclassresults["totRec"] > 0) {//print_r($tpclassresults); print_r($registered_count);
					//echo "Parent regStatus : ".$parentRegStatus; print_r($tpclassresults);
					if($parentRegStatus == "N") {
						if($tpclassresults["regOption"] == "tp") {
							$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;'><span style='float:right;'>";
							//<span style='float:right;'>".$tpclassresults['class_details']->crs_id."<input type='button' class='action-btn' value='Done' onclick=\"$('body').data('prerequisite').getTpPrereqCatalogRegister('".$tpCrsId."','".$program_id."')\"></span></td></tr></table>";
							$dispData .= "<table border='0' class='search-register-btn' align='right'><tbody><tr>";
							$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('body').data('prerequisite').getTpPrereqCatalogRegister('".$tpCrsId."','".$program_id."','".t('ERR066')."')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("Register")."</span><div class='admin-save-button-right-bg'></div></div></td>";
							$dispData .= "</tr></table></span></td></tr></table>";
						} else {
							$userId 		= getSltpersonUserId();
							$getBtnAction 	= $tpclassresults["button_action"];
							$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;padding-top:0px;'><span style='float:right;'>";
							//<td style='border-bottom:solid 0px #ccc;'><span style='float:right;'>".$tpclassresults['class_details']->crs_id."<input type='button' class='action-btn' value='Done' onclick=\"$('body').data('learningcore').callRegisterClass('lnr-catalog-search',".$userId.",".$tpCrsId.",".$program_id.")\"></span></td></tr></table>";
							$dispData .= "<table border='0' class='search-register-btn' align='right'><tbody><tr>";
							$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('body').data('prerequisite').getPrereqCatalogRegister('".$tpCrsId."','".$program_id."','".t('ERR067')."')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("Register")."</span><div class='admin-save-button-right-bg'></div></div><div id='seatleftcount$tpCrsId' class='overall-seatleft-count'></div></td>";
							$dispData .= "</tr></table></span></td></tr></table>";
						}
					}
				} else {
					$dispData = t("ERR061");
				}			
			} else {
				$location 		= $tpclassresults['class_details']->location;
				//$sessionDate 	= '<div class="line-item-container float-left" >'.(count($tpclassresults['session_details'])>0) ? $tpclassresults['session_details'][0]['start_date'].' '.$tpclassresults['session_details'][0]['start_time'].' <span class="time-zone-text">'.$tpclassresults['session_details'][0]['start_form'].'</span> '.t('to').' '.$tpclassresults['session_details'][0]['end_time'].' <span class="time-zone-text">'.$tpclassresults['session_details'][0]['end_form'].'</span></div> ' : '';
				$sessionDateDetail=(count($tpclassresults['session_details'])>0) ? $tpclassresults['session_details'][0]['start_date'].' '.$tpclassresults['session_details'][0]['start_time'].' <span class="meridian-time time-zone-text">'.$tpclassresults['session_details'][0]['start_form'].'</span>'.' to '.$tpclassresults['session_details'][0]['end_time'].' <span class="meridian-time time-zone-text">'.$tpclassresults['session_details'][0]['end_form'].'</span>': '';
				//$regstatus 	= $tpclassresults['class_details']->regstatus;
				$enrollStatus 	= (($regstatus == 'N') ? t("Not Registered") : $regstatus);//t("Registered"));
				$enrollStatus	= ($enrollStatus == "Enrolled") ? t("Registered") : $enrollStatus;
				$cls_id 		= $tpclassresults['class_details']->cls_id;				
				$crs_id 		= $tpclassresults['class_details']->crs_id;
				
				if(($totalRec == 1) || (($totalRec == 2) && ($tpLevel == 'l1'))) {
					$checkedValue = "";//"checked";				
				}else{
					$checkedValue = "";
				}
            $waitlist   = 0;
            $getRegister                         = getRegisteredOrNot($tpclassresults['class_details']->crs_id, $tpclassresults['class_details']->cls_id);
            $tpclassresults->enrolled_id         = $getRegister->enrolled_id;
            $tpclassresults->comp_status         = $getRegister->comp_status;
            $tpclassresults->enrolled_status     = $getRegister->enrolled_status;
            $tpclassresults->waitlist_flag       = $getRegister->waitlist_flag;
            $tpclassresults->waitlist_priority   = $getRegister->waitlist_priority;
            
            if(getLastCompCourseClass($tpclassresults['class_details']->crs_id)->enrolled_id != ''){
	 			$tpclass_last_comp = getLastCompCourseClass($tpclassresults['class_details']->crs_id);
			}else{
	 			$tpclass_last_comp = '';
			}
		
            $waitlisted = (($tpclassresults->enrolled_status == 'lrn_crs_reg_wtl') || ($tpclassresults->waitlist_flag == 'lrn_crs_reg_wtl') ) ? 1 : 0;
            if(($tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_ilt" || $tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_vcl") && (($tpclassresults['class_details']->available_seats == 0) || ($tpclassresults->enrolled_status == 'lrn_crs_reg_wtl') || ($tpclassresults['class_details']->waitlist_flag == 'lrn_crs_reg_wtl'))) {
              $waitlist_register  = getWaitlistCatalogInfo($tpclassresults['class_details']->cls_id);//$tpclassresults->waitlist_seats;
              $waitlist           = 1; 
            } else {
              $waitlist_register = 0;
              $waitlist          = 0;
            }
            if($tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_ilt" || $tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_vcl"){
                 if($waitlisted == 1) {
                     $seatInfo = t('LBL125').' '.getWaitlistPosition($tpclassresults['class_details']->cls_id, $tpclassresults['class_details']->crs_id, $userId);
                    }else if($waitlist_register > 0) {
                  	 $seatInfo = (($waitlist_register == 1) ? ($waitlist_register." ".t('LBL126')) : ($waitlist_register." ".t('LBL127')));
                   }else {
                  	  $seatInfo = $tpclassresults['class_details']->available_seats;
                  	  $seatInfo = (($tpclassresults['class_details']->available_seats == 1) ? ($tpclassresults['class_details']->available_seats." ".t('LBL052')) : ($tpclassresults['class_details']->available_seats." ".t('LBL053')));
                 }
              }
			if($getRegister->enrolled_id == $tpclass_last_comp->enrolled_id){
				$tpcheckedValue ='checked=checked';
			}else{
				$tpcheckedValue ="";
			}
				
				//following attributes are common to all delivery types of class
				$header_details->delivery_type_code = $tpclassresults['class_details']->delivery_type_code;
				$header_details->icon_class = getTypeImageClass($tpclassresults['class_details']->delivery_type_code);
				$header_details->icon_title = $tpclassresults['class_details']->delivery_type_name;
				$header_details->data_title = sanitize_data($tpclassresults['class_details']->cls_title);
			
// 				$header_details->pre_req_show_more = "<div class='show_more_less class-content-more padbt5' id='preTitle".$prereqtype.$tpresults1->preid."l1' data=\"data=".$passParams."\" href=\"javascript:void(0);\">" . t('LBL713') . "</div>";
				$header_details->attributes['cls_code']->tooltip = sanitize_data($tpclassresults['class_details']->cls_code);
				$header_details->attributes['cls_code']->data = titleController('EXP-SP-PREREQUISITE-CLASS-CODE', sanitize_data($tpclassresults['class_details']->cls_code), 10);
				
				$header_details->attributes['clsaction']->data = "<input type='hidden' name='clsaction-".$cls_id."' id='clsaction-".$cls_id."' value='".$register_action.'-'.$tpclassresults['class_details']->node_id."' >";
				$header_details->attributes['clsaction']->hidden = true;
				
				expDebug::dPrint('$tpclassresults class_details'.print_r($tpclassresults['class_details'], 1), 1);
				 if($tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_ilt" || $tpclassresults['class_details']->delivery_type_code == "lrn_cls_dty_vcl"){
					$header_details->attributes['session_date']->tooltip = strip_tags($sessionDateDetail);
					$header_details->attributes['session_date']->data =titleController('EXP-SP-PREREQUISITE-CLASS-SESSION-DATE', $sessionDateDetail, 10); 
				}
				
				
				
				$header_details->attributes['delivery_type_name']->tooltip = t($tpclassresults['class_details']->delivery_type_name);
				$header_details->attributes['delivery_type_name']->data = titleController('EXP-SP-PREREQUISITE-LOCATION', sanitize_data(t($tpclassresults['class_details']->delivery_type_name)), 10);  
				
				if(!empty($location)) {
					$header_details->attributes['location']->tooltip = sanitize_data($location);
					$header_details->attributes['location']->data = titleController('EXP-SP-PREREQUISITE-LOCATION', sanitize_data($location));
				}
				
				
				/* // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ */
				if($tpclassresults['class_details']->price > 0 && module_exists("exp_sp_commerce")){
					$header_details->attributes['price']->tooltip = "$ ".$tpclassresults['class_details']->price;
					$header_details->attributes['price']->data = "$ ".$tpclassresults['class_details']->price;
					
				}
				$header_details->data_desc = $tpclassresults['class_details']->cls_short_description;
				$header_details->action_variables = "<input type='radio'  value='" . $cls_id . "' " . (($regstatus != 'N') ? $tpcheckedValue : $checkedValue) . " name='cls-" . $crs_id . "' id='cls-" . $crs_id . "' onclick=\"$('body').data('prerequisite').getPreqClassId('" . $cls_id . "','" . $crs_id . "')\"></span><span id='hidden-seatleft-cls-" . $cls_id . "' style='display:none;'>" . $seatInfo;
				$header_details->content_detail_container_class = ' tp-prereq-select-class-level-detail';
				$header_details->content_action_container_class = ' tp-prereq-select-class-level-action';
				$header_details->content_icon_container_class = ' tp-prereq-select-class-level-icon';
				$header_details->content_row_class = ' tp-prereq-select-class-level-container';

				$dispData = theme("content_header", array("result" => $header_details, 'context' => ['callfrom' => 'tp_prereq']));
				
				//following section renders extra attributes like session, language and location info about the classes
				if(!empty($tpclassresults['class_details']->language)) {
					$language_info = '<div class="session-language-warpper"><div class="sub-section-title padbt5">'.t("LBL038").'</div><div class="session-location-block"><div class="location-address padbt10">'.t($tpclassresults['class_details']->language).'</div></div></div>';
				}
				expDebug::dPrint('$$language_info '.print_r($language_info, 1), 1);
				$sessionDetail = '';
				$sessionCnt = count($tpclassresults['session_details']);
				if($sessionCnt>0) {
					$sessionDetail = '<div class="sub-section-title">' . t("LBL277") . '</div>';
					for($i = 0; $i < $sessionCnt; $i++) {
						expDebug::dPrint('session details '.print_r($tpclassresults['session_details'], 1), 1);
						$paddingCls ='';
						if($i == ($sessionCnt-1) && empty($tpclassresults['class_details']->locationname))
							$paddingCls ='padbt10';
						$sessionDetail .= '<div class="session-detail-block padbt5 sessionDet '.$paddingCls.'">';
						$sessionName = '<div class="session-name session-row">';
						$sessionName .= '<span class="sess-attr-name">'.t("LBL107").': </span>';
						$sessionName .= '<div class="sessName vtip" title="' . sanitize_data($tpclassresults['session_details'][$i]["session_title"]) . '">' . titleController('ASSIGNLEARN-TP-CLS-SESSION-TITLE', sanitize_data($tpclassresults['session_details'][$i]["session_title"])) . '</div>';
						$sessionName .= '</div>';
						
						$sessionDateInfo = '<div class="session-date session-row">';
						$sessionDateInfo .= '<span class="sess-attr-name">'.t("LBL042").'</span>';
						$sessionDateInfo .= '<span title="" class="sess-attr-val">';
						$sessionDateInfo .= $tpclassresults['session_details'][$i]["session_day"];
						$sessionDate = $tpclassresults['session_details'][$i]['start_date'] . ' ' . $tpclassresults['session_details'][$i]['start_time'] . ' ' . '<span class="meridian-time">'.$tpclassresults['session_details'][$i]['start_form'] .'</span>' . ' ' . t('to') . ' ' . $tpclassresults['session_details'][$i]['end_time'] . ' ' . '<span class="meridian-time">'.$tpclassresults['session_details'][$i]['end_form'].'</span>';
						$sessionDateInfo .= ' ' . $sessionDate;
						$sessionDateInfo .= '</span>';
						
						$sessionInstructor = '<div class="session-instructor session-row">';
						$sessionInstructor .= '<span class="sess-attr-name">'.t('Instructor').': </span>';
						$sessionInstructor .= '<span class="sess-attr-val vtip" title="'.$tpclassresults['session_details'][$i]['session_instructor_name'].'">';
						$sessionInstructor .= titleController('CLASS-DETAIL-SESSION-INSTRUCTOR', $tpclassresults['session_details'][$i]['session_instructor_name']);
						$sessionInstructor .= '</span></div>';
						
						$sessionDetail .= $sessionName;
						$sessionDetail .= $sessionDateInfo;
						$sessionDetail .= $sessionInstructor;
						$sessionDetail .= '</div></div>';
					}
				}
				expDebug::dPrint('$sessionDetail '.print_r($sessionDetail, 1), 1);
				if(!empty($tpclassresults['class_details']->locationname)) {
					$locationInfo = '<div class="session-location-warpper">';
					expDebug::dPrint('$class_details location '.print_r($tpclassresults['class_details'], 1), 1);
					$locationInfo .= '<div class="sub-section-title padbt5">'.t("Location").': </div>';
					$locationInfo .= '<div class="session-location-block"><div class="location-address padbt10">';
					$locationInfo .= sanitize_data($tpclassresults['class_details']->locationname);
					$locationInfo .= (!empty($tpclassresults['class_details']->loationaddr1) ? ", ".sanitize_data($tpclassresults['class_details']->loationaddr1) : '');
					$locationInfo .= (!empty($tpclassresults['class_details']->locationaddr2) ? ", ".sanitize_data($tpclassresults['class_details']->locationaddr2) : '');
					$locationInfo .= (!empty($tpclassresults['class_details']->locationcity) ? ", ".sanitize_data($tpclassresults['class_details']->locationcity) : '');
					$locationInfo .= (!empty($tpclassresults['class_details']->locationstatename) ? ", ".$tpclassresults['class_details']->locationstatename : '');
					$locationInfo .= (!empty($tpclassresults['class_details']->locationzip) ? " - ".$tpclassresults['class_details']->locationzip : '');
					$locationInfo .= (!empty($tpclassresults['class_details']->locationcountryname) ? ", ".$tpclassresults['class_details']->locationcountryname : '');
// 					$locationInfo .= 'test11, ';
// 					$locationInfo .= 'test11, ';
// 							$locationInfo .= 'test11, ';;
// 							$locationInfo .= 'test11, ';
				}
				expDebug::dPrint('$$locationInfo '.print_r($locationInfo, 1), 1);
				
				$dispData .= '<div class="class-content-wrapper class-content-content-wrapper border-box-cnt-tp"><div class="content-list">';
				$dispData .= $language_info;
				$dispData .= $sessionDetail;
				$dispData .= $locationInfo;
				$dispData .= '</div></div>';
			}
		} else {
			$dispData .= t("ERR061");
		}
		
		print $dispData;

	} else if(isset($tpenrolled)) {
		$dispData = "<div class='prereq-enrolled-txt' id='PrerequisteErrorMsg'>";
		$dispData .= t("MSG408");
		$dispData .= "</div>";
		
		print $dispData;		
	}
	
	
	 ?>
</div>