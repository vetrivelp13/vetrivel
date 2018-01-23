<div class="top-record-div-left for-team">
	<?php
	global $theme_key;
// 	$theme_key == "expertusoneV2";
	expDebug::dPrint('ayyappan test $$prereqtype'.print_r($prereqtype, true), 4);

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

		$dispData  = "<table id='myteam-prerequisite' class='prereq-course-class-list-accord' cellpadding='1' cellspacing='2' width='100%' border='0'>";
		$dispData .= "<tr class='ui-widget-contents'><td>";
		$dispData .= "<a class='title_close ' id='preTitle".$prereqtype.$tpresults1->preid."l1' data=\"data=".$passParams."\" onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqListForTeam('".$tpresults1->preid."','l1','".$prereqtype."','l1','".$parentId."','".'MyTeam'."', this)\" href=\"javascript:void(0);\">&nbsp;</a>";
		$dispData .= "<span class='vtip course-title' title='".sanitize_data($tpresults1->pretitle)."'>".subStringController(sanitize_data($tpresults1->pretitle),85).((strlen($tpresults1->pretitle) > 85) ? "..." : "")."</span></td></tr>";
		$dispData .= "<tr class='ui-widget-contents'><td><div class='ui-widget-contents-list'><span class='item-title-code vtip' title='".sanitize_data($tpresults1->precode)."'>".subStringController(sanitize_data($tpresults1->precode),10)."</span><span class='pipe-line'>|</span>".t($tpresults1->pretype)."<span class='pipe-line'>|</span>".$enrollStatus;
		/* code for checking if equivalence is completed or registered */
		if(!empty($tpresults1->equvstatus) && $tpresults1->equvstatus != "Zero")
			$dispData 	.= "<span class='pipe-line'>|</span>".$tpresults1->equvstatus;

		$dispData .= "<input type='hidden' id='tpaction-".$tpresults1->preid."' name='tpaction-".$tpresults1->preid."' value='".$enrollRegType.'-'.$tpresults1->node_id."'>";
		/* // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ */
		if($enrollPrice=='0.00')
			$dispData .= (($tpresults1->pretype != 'Course' ) ? ((module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>") : '')) : '')."</div></td></tr>";
		else
			$dispData .= (($tpresults1->pretype != 'Course' ) ? ((module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>$ ".$enrollPrice) : '')) : '')."</div></td></tr>";
		$dispData .= "<tr class='ui-widget-contents'><td><span class='course-desc-info'>".descController('COURSE DETAILS',$tpresults1->shortdesc)."</span></td></tr>";
		$dispData .= "</table>";

		print $dispData;

	} else if(isset($tpresults2)) {
		//shows list of courses - Bring into list of courses respect to the TP
		//print_r($register_action);
		if(isset($tpresults2->buttonArr)) {
				
			if($tpresults2->totRec > 0) {
				if($parentRegStatus == 'N') {
					if($tpresults2->regOption == 'tp') {
						$dispData .= "<table border='0' cellpadding='0'  cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;'><span style='float:right;' class='action-btn'>";
						$dispData .= "<table border='0' class='search-register-btn prereq-register' align='right'><tbody><tr>";
						if($theme_key == "expertusoneV2") {
							$dispData .= "<td style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getTpPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR066')."','".$Myteamuserid."')\" class='prereq-action-container' class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("LBL253")."</span><div class='admin-save-button-right-bg'></div></div></td>";
						}
						else {
							$dispData .= "<td style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getTpPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR066')."','".$Myteamuserid."')\" class='prereq-action-container action-btn' class='border:0px solid;'>".t("LBL253")."</td>";
						}
						$dispData .= "</tr></table></span></td></tr></table>";
					} else {
						$userId = getSltpersonUserId();
						$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;padding-top:0px;'>";
						$dispData .= "<table border='0' class='search-register-btn prereq-register' align='right'><tbody><tr>";
						if($theme_key == "expertusoneV2") {
							$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR067')."','".$Myteamuserid."')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("LBL253")."</span><div class='admin-save-button-right-bg'></div></div></td>";
						}
						else {
							$dispData .= "<td class='prereq-action-container action-btn' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR067')."','".$Myteamuserid."')\"  class='border:0px solid;'>".t("LBL253")."</td>";
						}
						$dispData .= "</tr></table></td></tr></table>";
					}
				}
			} else {
				$dispData .= t("ERR054");
			}
		} else {
			$prereqtype 	= 'course';
			$passParam  	= 'l1-'.arg(2);
			$parentId   	= arg(2);
			$mandatory  	= "";//($tpresults2->mandatory =='N') ? t('Optional') : t('Mandatory');
			$passParams 	= "{'prereqType':'".$prereqtype."'}";
			$enrollStatus 	= (($regstatus == 'N') ? t("Not Registered") : $regstatus);//t("Registered"));
			$enrollStatus	= ($enrollStatus == "Enrolled") ? t("Registered") : $enrollStatus;
				
			// below check implemented by Vincent for #0027337
			// TODO: Need to fix this issue in IE9 and remove the below check
			$ie9Browser = (ereg('MSIE 9',$_SERVER['HTTP_USER_AGENT'])) ? true : false;
			if($ie9Browser == 1){
				$class = '';
			}else{
				$class = 'vtip';
			}
				
			$ntpLevel 		= (isset($tpresults2->node_id) ? "l3" : "l2");
			$showLevel		= "l2";
			$dispData 	 = "<table id='myteam-prerequisite' class='prereq-course-class-list-accord' cellpadding='0' cellspacing='0' width='100%' border='0'>";
			$dispData 	.= "<tr class='ui-widget-contents'><td>";
			//$dispData .= "<a class='title_close' id='preTitle".$prereqtype.$tpresults2->id."l2' data=\"data=".$passParams."\" onclick=\"$('body').data('prerequisite').getTpPrereqCatalog('".$tpresults2->id."','','".$prereqtype."','l2')\" href=\"javascript:void(0);\">&nbsp;</a>";
			$dispData 	.= "<a class='title_close ' id='preTitle".$prereqtype.$tpresults2->id."l2' data=\"data=".$passParams."\" onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqListForTeam('".$tpresults2->id."','".$ntpLevel."','".$prereqtype."','".$showLevel."','".$parentId."','".'MyTeam'."', this)\" href=\"javascript:void(0);\">&nbsp;</a>";
			$dispData 	.= "<span class='course-title ".$class."' title='".sanitize_data($tpresults2->pretitle)."'>".subStringController(sanitize_data($tpresults2->pretitle),85).((strlen($tpresults2->pretitle) > 85) ? "..." : "")."</span></td></tr>";
			$dispData 	.= "<tr class='ui-widget-contents'><td><div class='ui-widget-contents-list'><span class='item-title-code ".$class."' title='".sanitize_data($tpresults2->precode)."'>".subStringController(sanitize_data($tpresults2->precode),10)."</span><span class='pipe-line'>|</span><span id='clsStatus-".$tpresults2->id."'>".$enrollStatus."</span>";
			/* code for checking if equivalence is completed or registered */
			if(!empty($tpresults2->equvstatus) && $tpresults2->equvstatus != "Zero")
				$dispData 	.= "<span class='pipe-line'>|</span><span id='crsEquvStatus-".$tpresults2->id."'>".$tpresults2->equvstatus."</span>";
				
			$dispData 	.= "</div></td></tr>";
			$dispData 	.= "<tr class='ui-widget-contents'><td><span class='course-desc-info'>".descController('COURSE DETAILS',$tpresults2->short_description)."</span></td></tr>";
			$dispData 	.= "</table>";
				
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
							$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;'>";
							$dispData .= "<table border='0' class='search-register-btn prereq-register' align='right'><tbody><tr>";
							if($theme_key == "expertusoneV2") {
								$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getTpPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR066')."','".$Myteamuserid."')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("LBL253")."</span><div class='admin-save-button-right-bg'></div></div></td>";
							}
							else {
								$dispData .= "<td class='prereq-action-container action-btn' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getTpPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR066')."','".$Myteamuserid."')\"  class='border:0px solid;'>".t("LBL253")."</td>";
							}
							$dispData .= "</tr></table></td></tr></table>";
						} else {
							$userId 		= getSltpersonUserId();
							$getBtnAction 	= $tpclassresults["button_action"];
							$dispData = "<table border='0' cellpadding='0' cellspacing='0' width='100%' id='actionBtn'><tr><td style='border-bottom:solid 0px #ccc;padding-top:0px;'>";
							$dispData .= "<table border='0' class='search-register-btn prereq-register' align='right'><tbody><tr>";
							if($theme_key == "expertusoneV2") {
								$dispData .= "<td class='prereq-action-container' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR067')."','".$Myteamuserid."')\"  class='border:0px solid;'><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><span class='prerequisite-done-btn admin-save-button-middle-bg'>".t("LBL253")."</span><div class='admin-save-button-right-bg'></div></div><div id='seatleftcount$tpCrsId' class='overall-seatleft-count'></div></td>";
							}
							else {
								$dispData .= "<td class='prereq-action-container action-btn' style='border-bottom:0px;' onclick=\"$('#lnr-myteam-search').data('lnrmyteamsearch').getPrereqCatalogRegisterForTeam('".$tpCrsId."','".$program_id."','".t('ERR067')."','".$Myteamuserid."')\"  class='border:0px solid;'>".t("LBL253")."</td>";
							}
							$dispData .= "</tr></table></td></tr></table>";
						}
					}
				} else {
					$dispData = "<div class='error-string'>".t("ERR061")."</div>";
				}
			} else {
				$location 		= $tpclassresults['class_details']->location;
				$sessionDate 	= (count($tpclassresults['session_details'])>0) ? $tpclassresults['session_details'][0]['start_date'].' '.$tpclassresults['session_details'][0]['start_time'].' <span class="time-zone-text">'.$tpclassresults['session_details'][0]['start_form'].'</span> '.t('to').' '.$tpclassresults['session_details'][0]['end_time'].' <span class="time-zone-text">'.$tpclassresults['session_details'][0]['end_form'].'</span> ' : '';
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
				$getRegister                         = getRegisteredOrNot($tpclassresults['class_details']->crs_id, $tpclassresults['class_details']->cls_id, $Myteamuserid);
				$tpclassresults->enrolled_id         = $getRegister->enrolled_id;
				$tpclassresults->comp_status         = $getRegister->comp_status;
				$tpclassresults->enrolled_status     = $getRegister->enrolled_status;
				$tpclassresults->waitlist_flag       = $getRegister->waitlist_flag;
				$tpclassresults->waitlist_priority   = $getRegister->waitlist_priority;
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
				$dispData  = "<table id='myteam-prerequisite' class='prereq-course-class-list-accord' cellpadding='0' cellspacing='0' width='100%' border='0'>";
				$dispData .= "<tr class='ui-widget-contents'><td width='45%'>";
				$dispData .= "<a class='cls-title-open' id='preTitle".$prereqtype.$tpresults1->preid."l1' data=\"data=".$passParams."\" href=\"javascript:void(0);\">&nbsp;</a>";
				$dispData .= "<span class='vtip class-title' title='".sanitize_data($tpclassresults['class_details']->cls_title)."'>".subStringController(sanitize_data($tpclassresults['class_details']->cls_title),30).((strlen($tpclassresults['class_details']->cls_title)>30) ? "..." : "")."</span></td><td width='45%'>".$sessionDate."</td>";
				$dispData .= "<td width='10%'><span class='radio-btn'>";
				$dispData .= "<input type='hidden' name='clsaction-".$cls_id."' id='clsaction-".$cls_id."' value='".$register_action.'-'.$tpclassresults['class_details']->node_id."' >";
				//$dispData .= $tpclassresults['class_details']->node_id;
				$dispData .= "<input type='radio'  value='".$cls_id."' ".(($regstatus != 'N') ? ('checked=checked') : $checkedValue)." name='cls-".$crs_id."' id='cls-".$crs_id."' onclick=\"$('body').data('prerequisite').getPreqClassId('".$cls_id."','".$crs_id."')\"></span><span id='hidden-seatleft-cls-".$cls_id."' style='display:none;'>".$seatInfo."</span></td></tr>";

				$dispData .= "<tr class='ui-widget-contents'><td><div class='ui-widget-contents-list'><span class='item-title-code'>".subStringController(sanitize_data($tpclassresults['class_details']->cls_code),10)."<span class='pipe-line'>|</span>";
				$dispData .= t($tpclassresults['class_details']->delivery_type_name).(!empty($location) ? ("<span class='pipe-line'>|</span>".$location) : '')."</span>";
				/* // 41115: When commerce is enabled and price is not set then we should be showing as Register without the 0$ */
				if($tpclassresults['class_details']->price > 0){
					$dispData .= (module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>$ ".$tpclassresults['class_details']->price) : '')."</div></td></tr>";
				}else{
					$dispData .= (module_exists("exp_sp_commerce") ? ("<span class='pipe-line'>|</span>") : '')."</div></td></tr>";
				}
				$dispData .= "<tr class='ui-widget-contents'><td colspan='3'><span class='course-desc-info'>".descController('COURSE DETAILS',$tpclassresults['class_details']->cls_short_description)."</span></td></tr>";

				$dispData .= !empty($tpclassresults['class_details']->language) ? "<tr class='ui-widget-contents'><td colspan='3'><span class='class-lang'>".t("LBL038").": <span class='class-pre-label'>".t($tpclassresults['class_details']->language)."</span></span></td></tr>" : '';

				if(count($tpclassresults['session_details'])>0) {
					$dispData .= "<tr class='ui-widget-contents'><td colspan='3'>";
					$dispData .= "<table id='myteam-prerequisite' class='prereq-course-class-list-accord-session' cellpadding='0' cellspacing='0' width='100%' border='0'  class='class-level'>";
					$dispData .= "<tr class='ui-widget-contents'><td colspan='3'><span class='class-pre-label'>".t("LBL670").":</span></td></tr>";
						
					for($i=0;$i<count($tpclassresults['session_details']);$i++) {
						$sessionDate = $tpclassresults['session_details'][$i]['start_date'].' '.$tpclassresults['session_details'][$i]['start_time'].' '.$tpclassresults['session_details'][$i]['start_form'].' '.t('to').' '.$tpclassresults['session_details'][$i]['end_time'].' '.$tpclassresults['session_details'][$i]['end_form'];
						$dispData .= "<tr>";
						$dispData .=  "<td><span class='class-pre-label vtip' title='".sanitize_data($tpclassresults['session_details'][$i]["session_title"])."'>".subStringController(sanitize_data($tpclassresults['session_details'][$i]["session_title"]),10)."</span></td>";
						$dispData .=  "<td><span class='class-pre-label'>".$tpclassresults['session_details'][$i]["session_day"]."</span></td>";
						$dispData .=  "<td><span class='class-pre-label'>".$sessionDate."</span></td>";
						$dispData .= "</tr>";
					}
						
					$dispData .= "</table></td></tr>";
				}

				if(!empty($tpclassresults['class_details']->locationname)) {
					$dispData .= "<tr class='ui-widget-contents'><td colspan='3'>";
					$dispData .= "<table id='myteam-prerequisite' class='prereq-course-class-list-accord-location' cellpadding='0' cellspacing='0' width='100%' border='0' class='class-level'>";
					$dispData .= "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".t("Location").":</span></td></tr>";
					$dispData .= "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".sanitize_data($tpclassresults['class_details']->locationname).",</span></td></tr>";
					$dispData .= !empty($tpclassresults['class_details']->loationaddr1) ? "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".sanitize_data($tpclassresults['class_details']->loationaddr1).",</span></td></tr>" : '';
					$dispData .= !empty($tpclassresults['class_details']->loationaddr2) ? "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".sanitize_data($tpclassresults['class_details']->loationaddr2).",</span></td></tr>" : '';
					$dispData .= !empty($tpclassresults['class_details']->loationcity) ? "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".sanitize_data($tpclassresults['class_details']->loationcity).",</span></td></tr>" : '';
					$dispData .= !empty($tpclassresults['class_details']->loationstate) ? "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".$tpclassresults['class_details']->loationstate.",</span></td></tr>" : '';
					$dispData .= !empty($tpclassresults['class_details']->country_name) ? "<tr class='ui-widget-contents'><td><span class='class-pre-label'>".$tpclassresults['class_details']->country_name.".</span></td></tr>" : '';
					$dispData .= "</table></td></tr>";
				}
				$dispData .= "</table>";
			}
		} else {
			$dispData = "<div class='error-string'>".t("ERR061")."</div>";
		}

		print $dispData;

	} else if(isset($tpenrolled)) {
		$dispData = "<div class='prereq-enrolled-txt error-string'>";
		$dispData .= t("MSG408");
		$dispData .= "</div>";

		print $dispData;
	}
	?>
</div>