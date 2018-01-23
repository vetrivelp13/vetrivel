(function($) {
$.widget("ui.lnrmyteamsearch", {
	_init: function() {
		try{
		var self = this;
		//var searchStr = this.searchActionCheck();
		var searchStr = '';
		this.searchStrValue = searchStr;
		this.renderSearchResults(searchStr);	
		}catch(e){
			// to do
		}
	},
	/*
	 * To render the user profile information for the given user id
	 */
	showUserProfile: function(userId,userName){
		try{
		var obj = this;
		$('#user-detail-loader-'+userId).show();
		url = obj.constructUrl("learning/myteam-profile-view/"+userId);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				//$("#paintMyclassContentResults"+userId).trigger("reloadGrid",[{page:1}]);
				result = $.trim(result);
				obj.renderUserPopup(result,userName);
				$('#user-detail-loader-'+userId).hide();
			}
	    });
		}catch(e){
			// to do
		}
	},
	/*
	 * To show the user profile information in a pop-up
	 */
	renderUserPopup : function(data,userName){
		try{
	    $('#myteam-show-user-result').remove();
	    var html = '';
	    var dlgTitle = Drupal.t('LBL762')+' '+ userName;
	    html+='<div id="myteam-show-user-result" class="myteam-user-profile-cls">'+data+'</div>';
	    $("body").append(html);
	    $("#myteam-show-user-result").dialog({
	        position:[(getWindowWidth()-600)/2,100],
	        bgiframe: true,
	        width:600,
	        resizable:false,
	        modal: true,
	        title: SMARTPORTAL.t(dlgTitle),
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.5,
	           background: "black"
	         }
	    });	
	    //$('.ui-dialog').css('border','solid 10px #5F5F5F');
	    $("#myteam-show-user-result").show();	
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#myteam-show-user-result").remove();
	    });
		}catch(e){
			// to do
		}
	},
	

	rejectWizard : function(userId,enrolledId,classTitle,rowObj,learningType,classId,trid)
	{
		try{
			$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintMyclassContentResults'+userId);
			var isCommerceEnabled='';
		    var assMand =0;
		    var mandByRole	= 0;
		    if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "")
		    {
		    	isCommerceEnabled = "1";
		    }
			var closeButt={};
		    $('#dropMsg-wizard').remove();
		  	html="";
			html+='<div id="dropMsg-wizard" style="display:none; padding: 2px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		    html+='<tr><td height="30"></td></tr>';
		    html+='<tr>';
		    html+= '<td align="center" id="dropmsg-content"><span>'+Drupal.t("LBL416")+'</span></td>';
		    html+='</tr>';	
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    $("#dropMsg-wizard").dialog({
		        position:[(getWindowWidth()-500)/2,100],
		        autoOpen: false,
		        bgiframe: true,
		        width:500,
		        resizable:false,
		        modal: true,
		        title: Drupal.t('LBL3081') + " " + Drupal.t('Canceled'),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		    
		    $('.ui-dialog').wrap("<div id='my-team-reject-dialog'></div>");
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		    
		    $("#dropMsg-wizard").show();
 
			$('.ui-dialog-titlebar-close').click(function(){
		        $("#dropMsg-wizard").remove();
		        $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);
		    });

			$("#lnr-myteam-search").data("lnrmyteamsearch").getDropPolicyCall(userId,enrolledId,learningType,classId,isCommerceEnabled,assMand,classTitle,mandByRole,trid);
		}catch(e){
			// to do
		}
	},
	
	getDropPolicyCall : function(userId,enrollId,baseType,classid,isCommerceEnabled,assMand,clstitle,mandByRole,trid)
	{
		try{
		var url = this.constructUrl("ajax/get-droppolicy/" + userId +  "/" + baseType + "/" + enrollId+"/"+classid+"/"+isCommerceEnabled+"/"+assMand+"/"+mandByRole);		
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){				
				$("body").append("<script>"+result+"</script>");
				var closeButt={};				
				var learners=$("#gview_paintContentResults").find('#complete-confirmation-wizard_'+userId).val();
				var status=Drupal.t('Canceled');				
				if(baseType == "tp") {
					var cancelTitleLT = Drupal.t("Training Plan");
				} else {
					var cancelTitleLT = Drupal.t("Class");
				}				
				if(drop_policy_info.next_action=="drop")
				{ 
					$("#dropmsg-content").html('<span>'+Drupal.t('MSG822', {'@learner_name': learners, '@status': status, '@objtype': cancelTitleLT})+'</span>');
					closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();$("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);};
					if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'my-team-reject-dialog','esignFor':'ManagerReject','userId':userId,'enrolledId':enrollId,'classTitle':clstitle,'baseType':baseType,'classId':classid,'trid':trid,'isCommerceEnabled':isCommerceEnabled,'assMand':assMand,'refundFlag':''};
				    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
				    }else{
						closeButt[Drupal.t('Yes')]=function(){ $("#lnr-myteam-search").data("lnrmyteamsearch").dropEnrollCall(userId,classid,enrollId,baseType,"",isCommerceEnabled,assMand,clstitle,trid); $("#dropmsg-content").html(Drupal.t('MSG424'));};
				    }
				}else if(drop_policy_info.next_action=="showmsg")
				{
				$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_info.msg)+"</span>");
					
					var html="";
					html+='<span><b>'+Drupal.t('LBL083')+' : </b>'+(HtmlEncode(decodeURIComponent(clstitle)))+"</span>";
					html+="<br/><br/>";
					html+="<span>"+SMARTPORTAL.t(drop_policy_info.msg)+'</span>';	
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();$("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);};
				}else if(drop_policy_info.next_action=="showdroppolicy")
				{
					var html="";
		
					if(drop_policy_info.deducted_amount == 0){
					  html+="<span>"+Drupal.t("MSG265")+"<br/> </span>";
					  html+="<br/><br/>";
					  html+="<span>"+Drupal.t("MSG266")+'</span>';
					}
					else{
						if(drop_policy_info.refund_amt != null && drop_policy_info.refund_amt != undefined && drop_policy_info.refund_amt != "" && drop_policy_info.refund_amt != 0){
							drop_policy_info.refund_amt = drop_policy_info.refund_amt.toFixed(2);
						}
						else
							drop_policy_info.refund_amt = '0.00';
						if(drop_policy_info.deducted_amount != null && drop_policy_info.deducted_amount != undefined && drop_policy_info.deducted_amount != "" && drop_policy_info.deducted_amount != 0){
							drop_policy_info.deducted_amount = drop_policy_info.deducted_amount.toFixed(2);
						}
						else
							drop_policy_info.deducted_amount = '0.00';
					  html+='<span>' + Drupal.t('MSG402')+' ' + drop_policy_info.currency_type+' '+ drop_policy_info.deducted_amount +' '+Drupal.t('MSG401')+"</span>";
					  html+="<br/><br/>";
					  html+='<span class="dropmsgTitle">'+Drupal.t('LBL1165')+': </span>'+drop_policy_info.currency_type+" "+drop_policy_info.refund_amt;
					  html+="<br/><br/>";
					  html+="<span>"+Drupal.t("MSG266")+'</span>';
					}
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();$("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);};
					if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
				    	var esignObj = {'popupDiv':'my-team-reject-dialog','esignFor':'ManagerReject','userId':userId,'enrolledId':enrollId,'classTitle':clstitle,'learningType':baseType,'classId':classid,'trid':trid,'isCommerceEnabled':isCommerceEnabled,'assMand':assMand,'refundFlag':'true'};
				    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
				    }else{
						closeButt[Drupal.t('Yes')]=function(){ $("#lnr-myteam-search").data("lnrmyteamsearch").dropEnrollCall(userId,classid,enrollId,baseType,"true",isCommerceEnabled,assMand,clstitle,trid); $("#dropmsg-content").html(Drupal.t('MSG424'));};
				    }
				}
				 $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('paintMyclassContentResults'+userId);
				 $("#dropMsg-wizard").dialog('open');
				 $("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
				 
				 //Append div script
				  this.currTheme = Drupal.settings.ajaxPageState.theme;
			/*	  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('close-btn-lnrenrollment white-btn-bg-middle');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
					}*/
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	 
				 
				 this.currTheme = Drupal.settings.ajaxPageState.theme;
				 if(this.currTheme == "expertusoneV2"){
				 changeDialogPopUI();
				 }
			}
	    });
		}catch(e){
			// to do
		}
	},
	
dropEnrollCall : function(userId,classId,enrollId,baseType,refundflag,isCommerceEnabled,assMand,classTitle,trid){
		try{
		closeButt=new Array();
		$("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
		if(baseType == 'tp'){
		  var url = this.constructUrl("ajax/drop-lp/" + enrollId + "/" + classId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/1");
		}
		else{
		  var url = this.constructUrl("ajax/drop-enroll/" + userId +  "/" + baseType + "/" + enrollId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/1");
		}
		$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				//obj.searchAction();
				//window.location.reload();
				$("#paintMyclassContentResults"+userId).trigger("reloadGrid",[{page:1}]);
				$("body").append("<script>"+result+"</script>");
				var status =  drop_policy_result.status;
				var resultMsg;
				if(status == 'success'){
					var resultMsgStatus = Drupal.t('MSG416');
					resultMsg	= resultMsgStatus + ' - ' +(HtmlEncode(decodeURIComponent(classTitle)))+ '.';
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset div.admin-save-button-left-bg').remove();
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset div.admin-save-button-right-bg').remove();
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').css("margin-right","-14px");
					$("#dropMsg-wizard").remove();	
					$("#dropmsg-content").remove();
				}else if(status == 'cannotcancel'){
					resultMsg	= Drupal.t('MSG406');
				}else{
					//resultMsg	= SMARTPORTAL.t('Sorry, Problem in rejecting the Class \''+classTitle+'\'.');
					resultMsg	= Drupal.t('ERR144')+' '(HtmlEncode(decodeURIComponent(classTitle)));
				}
				$("#dropmsg-content").html('<span>'+resultMsg+"</span>");
				$("#dropMsg-wizard").remove();
				$("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('searchRecordsPaint');
			}
	    });
		}catch(e){
			// to do
		}
	},
	
	rejectWizard_old: function(userId,enrolledId,classTitle,rowObj,learningType,classId,trid){
		try{
		unescape(classTitle);
		if(learningType == "Program") {
			var cancelTitleLT = Drupal.t("Training Plan");
		} else {
			var cancelTitleLT = Drupal.t("Class");
		}
		$('#reject-wizard').remove();
		var html = '';
	    html+='<div id="reject-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
	    html+= '<td align="center" class="commanTitleAll"><span>'+Drupal.t("MSG415")+' '+cancelTitleLT+',</span></td>';
	    html+='</tr>';
	    html+='<tr>';
	    html+= '<td align="center"><span>"'+classTitle+'"</span></td>';
	    html+='</tr>';
	    html+='<tr><td height="25"></td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#reject-wizard').remove(); $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);};
	    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'my-team-reject-dialog','esignFor':'ManagerReject','userId':userId,'enrolledId':enrolledId,'classTitle':classTitle,'rowObj':rowObj,'learningType':learningType,'classId':classId,'trid':trid};
	    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj); };
	    }else{
	    	closeButt[Drupal.t('Yes')]=function(){ $("#lnr-myteam-search").data("lnrmyteamsearch").cancelLearning(userId,enrolledId,classTitle,rowObj,learningType,classId,trid);};
	    }
	    $("#reject-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('MSG405'),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog').wrap("<div id='my-team-reject-dialog'></div>");
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    $("#reject-wizard").show();	
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
	    changeDialogPopUI();
	 	}
	    $('.ui-dialog-titlebar-close').click(function(){
	        $("#reject-wizard").remove();
	        /* ----- Changes to refresh grid on close ---- ticket 16260*/
	        $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);
	    });
		}catch(e){
			// to do
		}
	},
	/*
	 * To Cancel the class registration(Learning) in the manager dash board
	 */
	cancelLearning: function(userId,enrolledId,classTitle,rowObj,learningType,classId,trid){
		try{
		var html = "<div id='paintClassContentResults"+userId+"'></div>";
        $("#paintClassContentResults"+userId).remove();
        $("#gview_paintMyclassContentResults"+userId).prepend(html);
        var obj = this;
		this.createLoader("reject-wizard");
		$("#loaderdivreject-wizard").css({"width":"450","height":"70"})
		$("#loaderdivreject-wizard td").css({"width":"450","height":"80"})
		$('#reject-wizard td').css('border-bottom','none');
		url = obj.constructUrl("ajax/learning/myteam-search/cancel/"+learningType+"/"+enrolledId+"/"+classId+"/"+userId);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				result = $.trim(result);
				result = result.split("|");
				resDate = result[0];
				result = result[1];
				if(result=='success'){
					resultMsgStatus = Drupal.t('MSG416');
					resultMsg	= resultMsgStatus + ' - ' +HtmlEncode(decodeURIComponent(classTitle))+ '.';
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset div.admin-save-button-left-bg').remove();
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset div.admin-save-button-right-bg').remove();
					$('#my-team-reject-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').css("margin-right","-14px");
				}else if(result=='cannotcancel'){
					resultMsg	= Drupal.t('MSG406');
				}else{
					//resultMsg	= SMARTPORTAL.t('Sorry, Problem in rejecting the Class \''+classTitle+'\'.');
					resultMsg	= Drupal.t('ERR144')+' '+HtmlEncode(decodeURIComponent(classTitle));
				}
				obj.rejectMsg(resultMsg,userId,classId,result,trid,resDate);
				
			}
	    });
		}catch(e){
			// to do
		}
	},
	/*
	 * To show the status of cancel class in a pop-up
	 */
	rejectMsg : function(data,userId,classId,result,trid,resDate){
		try{
	    var html = '';
	    html+='<div id="myteam-cancel-learning-result" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
	    html+= '<td align="center" class="commanTitleAll"><i>'+data+'</i></td>';
	    html+='</tr>';
	    html+='<tr><td height="25"></td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $('#reject-wizard').html(html);
	    //$('.ui-dialog-buttonset').html('');
	    $("#myteam-cancel-learning-result").show();
	    if(result=='success'){
			//$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-course-btn' ).css("display","none");
	    	 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').css("display","none");
			$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-status' ).html(Drupal.t("Canceled"));
			$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-Date' ).html(resDate);
		}
		}catch(e){
			// to do
		}
	},
	/*
	 * To Mark complete  wizard the class registration(Learning) in the manager dash board
	 */
	markCompleteWizard: function(userId,enrolledId,classTitle,rowObj,learningType,classId,trid){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){ 
			var calShowOn = 'both';
			var markComwidth=280;
			var markcomtitle=Drupal.t('LBL3081') + " " +Drupal.t('Completed');
		}else
		{
			var calShowOn = 'button';
			var markComwidth=500;
			var markcomtitle=Drupal.t('MSG404')+' - '+titleRestrictionFadeoutImage(classTitle,'exp-sp-myteam-class-title');
		}
		var scoreread = '';
		var graderead = '';
		if(learningType == "Program") {
			scoreread = 'disabled=""';
			graderead = 'disabled=""';
		}		
		if(learningType == "Program") {
			var cancelTitleLT = Drupal.t("Training Plan");
		} else {
			var cancelTitleLT = Drupal.t("Class");
		}
		$('#mark-complete-wizard').remove();
		var html = '';
	    html+='<div id="mark-complete-wizard" style="display:none; padding: 0px; overflow:hidden">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="15" colspan="2"></td></tr>';
	    if(this.currTheme == "expertusoneV2"){ 
	    html+= '<tr><td width="52%" class="commanTitleAll mark-complete-lbl"><span>'+Drupal.t('LBL706') +':</span> <span class="require-text">*</span></td><td class="mark-complete-calender"><input type="text" name="comp_date" id="comp_date" size="14" readonly="true"></td></tr>';
	  /*html+= '<tr><td width="35%"><span>'+Drupal.t('LBL707') +' : </span></td><td><select name="rosterGrade" id="rosterGrade"  '+graderead+' ><option value="">Select</option>'+$('#mark-complete-grade').html()+'</select></td></tr>';*/
	    html+= '<tr><td width="52%" class="commanTitleAll mark-complete-lbl"><span>'+Drupal.t('LBL668') +':</span></td><td class="mark-complete-score"><input type="text" class="validateNum" '+scoreread+' id="appScore" maxlength="4" size="14"></td></tr>';	
	    }
	    else
	    {
	     html+= '<tr><td width="35%" class="commanTitleAll mark-complete-lbl"><span>'+Drupal.t('LBL706') +' : </span><span class="require-text">*</span></td><td class="mark-complete-calender"><input type="text" name="comp_date" id="comp_date" size="14" readonly="true"></td></tr>';
	   	 html+= '<tr><td width="35%" class="commanTitleAll mark-complete-lbl"><span>'+Drupal.t('LBL668') +' : </span></td><td class="mark-complete-score"><input type="text" class="validateNum" '+scoreread+' id="appScore" maxlength="4" size="8"></td></tr>';	
	    }
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close'); $("#mark-complete-wizard").remove(); $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId); };
				closeButt[Drupal.t('LBL725')]=function() {
				$(this).dialog('destroy');
				$(this).dialog('close');
				var learners;
				var selectedLearners = enrolledId.split(',');
				if(selectedLearners.length == 1) {
					learners = $("#gview_paintContentResults").find('#complete-confirmation-wizard_'+userId).val();
				} 	else {
					learners = selectedLearners.length +" "+ Drupal.t('LBL3080');
				}
				var status = Drupal.t('Completed');
				$('#complete-confirmation-wizard').remove();
			  	html="";
				html+='<div id="complete-confirmation-wizard" style="display:none; padding: 10px;">';
			    html+='<table width="100%" class="complete-confirmation-wizard-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
			    html+='<tr><td height="30"></td></tr>';
			    html+='<tr>';
			    html+= '<td align="center" id="complete-confirmation-wizard-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status': status, '@objtype': cancelTitleLT}) +'</span></td>';
			    html+='</tr>';
			    html+='</table>';
			    html+='</div>';
			    $("body").append(html);
			    var confButton = {};
				confButton[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#complete-confirmation-wizard').remove(); $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId); };
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    		var esignObj = {'popupDiv':'my-team-markcomplete-dialog','esignFor':'ManagerMarkComplete','userId':userId,'enrolledId':enrolledId,'classTitle':classTitle,'rowObj':rowObj,'learningType':learningType,'classId':classId,'trid':trid};
				{
				confButton[Drupal.t('Yes')] = function(){ $.fn.getNewEsignPopup(esignObj); };}
				}
				else{
				confButton[Drupal.t('Yes')] = function(){
							$("#lnr-myteam-search").data("lnrmyteamsearch").markcompleteLearning(userId,enrolledId,classTitle,rowObj,learningType,classId,trid);		
				};
				}
				$("#complete-confirmation-wizard").dialog({
			        position:[(getWindowWidth()-500)/2,100],
			        autoOpen: false,
			        bgiframe: true,
			        width: 500,
			        resizable: false,
			        modal: true,
			        title: Drupal.t('LBL3081') + " " +status,
			        buttons: confButton,
			        closeOnEscape: false,
			        draggable: false,
			        overlay: {
			           opacity: 0.9,
			           background: "black"
			         }
			    });				
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			    					
				if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
					changeDialogPopUI();
				} 
				$("#complete-confirmation-wizard").show();
				$("#complete-confirmation-wizard").dialog('open');};
				$('.ui-dialog-titlebar-close').click(function(){
					        $("#complete-confirmation-wizard").remove();
					        $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);
					    });
	    $("#mark-complete-wizard").dialog({
	        position:[(getWindowWidth()-markComwidth)/2,100],
	        bgiframe: true,
	        width:markComwidth,
	        resizable:false,
	        modal: true,
	        title:markcomtitle,
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    
	    $('.ui-dialog').wrap("<div id='my-team-markcomplete-dialog'></div>");
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		 this.currTheme = Drupal.settings.ajaxPageState.theme;
		  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			/* $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right');*/
			}
	    $("#mark-complete-wizard").show();
	    if($('#my-team-markcomplete-dialog')!=undefined && this.currTheme != "expertusoneV2"){
	    	$('#my-team-markcomplete-dialog #ui-dialog-title-mark-complete-wizard').attr('title',Drupal.t('MSG404')+' - '+classTitle);
	    	$('#my-team-markcomplete-dialog #ui-dialog-title-mark-complete-wizard').addClass('vtip');
	    	vtip();
	    }
	    if(this.currTheme == "expertusoneV2"){
		 	$('#my-team-dialog').find('.ui-dialog-content').addClass('assign-learning-team');	
		    changeDialogPopUI();
		   }
	    $('.ui-dialog-titlebar-close').click(function(){
	        $("#mark-complete-wizard").remove();
	        $("#lnr-myteam-search").data("lnrmyteamsearch").reloadDataGridMyTeam(userId);
	    });
	    
	    //For set min date to disable past session date in ILT/VC Class. ticket: 0022513
	    var sesEndDate = new Array();
	    var sessStartEndDates = $('#sessStartEndDates_'+classId).text();
	    sesEndDate = sessStartEndDates.split("#");
	    var sess = sesEndDate[0];//first session's date should be used to validate completion date
		var minDate;
		var today = new Date();/* fix for ticket  0029563*/
		/*$forMonth = (today.getMonth()  < 10) ? ('0'+(today.getMonth() + 1)) : today.getMonth();*/
		
		/* fix for ticket  #0040074 Oct.getMonth() is 9 , Nov.getMonth() is 10 it wont Print as 10 as Month Because Nov is 11 in Month Setup*/
		/* Note : getMonth() Jan as 0 , Feb as 1 , Dec as 11 */
		$forMonth = (today.getMonth()  < 9) ? ('0'+(today.getMonth() + 1)) : today.getMonth()+1;
		
		var formatdate = ($forMonth + "-" + today.getDate() + "-" + today.getFullYear());
			if(sesEndDate != null && sesEndDate != '' && sesEndDate != undefined){
				//$('#comp_date').datepicker({minDate:sess, dateFormat: "mm-dd-yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
				$("#comp_date").val(sess);
				formatdate = sess;
			}
			else{
			    //$('#comp_date').datepicker({minDate:formatdate,dateFormat: "mm-dd-yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
			    $("#comp_date").val(formatdate);
			} 
			var isAtLeastIe11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
			if($.browser.msie && $.browser.version > 7 || isAtLeastIe11 ){
				var obj = this;
				setTimeout(function(){
					obj.callDatePicker(formatdate);
				},500);
			}else{
				$('#comp_date').datepicker({minDate:formatdate,dateFormat: "mm-dd-yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
			}
		}catch(e){
			// to do
		}
	},/* fix for ticket  0029563*/
	/*
	 * To Mark complete the class registration(Learning) in the manager dash board
	 */
	callDatePicker: function(minDate){
		try{
		$('#comp_date').datepicker({minDate:minDate, dateFormat: "mm-dd-yy", showOn: "both", buttonImage: themepath+"/expertusone-internals/images/calendar_icon.JPG", buttonImageOnly: true });
		}catch(e){
			// to do
		}
	},
	
	markcompleteLearning: function(userId,enrolledId,classTitle,rowObj,learningType,classId,trid){
		try{
		var html = "<div id='paintClassContentResults"+userId+"'></div>";
        $("#paintClassContentResults"+userId).remove();
        $("#gview_paintMyclassContentResults"+userId).prepend(html);
        $("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
		var obj = this;
		this.createLoader("complete-confirmation-wizard");
		$("#loaderdivmark-complete-wizard").css({"width":"450","height":"100"});
		$("#loaderdivmark-complete-wizard td").css({"width":"450","height":"100"});
		$('#complete-confirmation-wizard td').css({"height":"38","border-bottom":"none"});
		var score = ($("#appScore").val()) ? $("#appScore").val() : 'nil';
		var lnrId = this.getLearnerId();
		if(lnrId == "0" || lnrId == "")
		{
		self.location='?q=learning/enrollment-search';
		return;
		}
		
		url = obj.constructUrl("ajax/myteam/markcomplete/"+learningType+"/"+enrolledId+"/"+classId+"/"+userId+"/"+$("#comp_date").val()+"/"+score);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				$("#paintMyclassContentResults"+userId).trigger("reloadGrid",[{page:1}]);
				result = $.trim(result);
				result = result.split("|");
				resDate = result[0];
				result = result[1];
				if(result=='success'){
					resultMsg	= Drupal.t('LBL747')+' - '+(HtmlEncode(decodeURIComponent(classTitle)));
				}else if(result == 'nochange'){
					resultMsg	= Drupal.t('MSG748');
				}else{
					resultMsg	= SMARTPORTAL.t('Sorry, Problem in mark complete the Class \''+(HtmlEncode(decodeURIComponent(classTitle)))+'\'.');
				}
				obj.markCompleteMsg(resultMsg,userId,classId,result,resDate,trid);
			}
	    });
		}catch(e){
			// to do
		}
	},
	/*
	 * To show the status of mark complete class in a pop-up
	 */
	markCompleteMsg : function(data,userId,classId,result,resDate,trid){
		try{
	    var html = '';
	    html+='<div id="myteam-mark-learning-result" style="display:none; padding: 0px;">';
	    html+='<table width="100%" class="complete-confirmation-wizard-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
	    html+= '<td align="center" title="'+htmlEntities(data)+'" class="commanTitleAll vtip"><i>'+titleRestrictionFadeoutImage(HtmlEncode(data),'my-team-module-class-title')+'</i></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $('#mark-complete-wizard').html(html);
	    //$('.ui-dialog-buttonset').html('');
	    $("#myteam-mark-learning-result").show();
	    if(result=='success' || result=='nochange'){
			//$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-course-btn' ).css("display","none");
	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').prev().css("display","none");
	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').css("display","none");
	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').next().css("display","none");
			$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-status' ).html("Completed");
			$('#paintMyclassContentResults'+userId+' #'+trid+' .myteam-learning-Date' ).html(resDate);
			$("#complete-confirmation-wizard").remove();
			$("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('searchRecordsPaint');
		}
	    vtip();
		}catch(e){
			// to do
		}
	},
	/*
	 * To call the render function to show the registered class(learning) 
	 * for a particular user at manager dash board
	 */
	viewLearning : function(userId,rowObj){
	 try{	
	  //console.log(rowObj);
	  var isLastRec = $("#viewLearningCls_" + userId).data('is_last_rec');
	  //console.log('data-is_last_rec = '+ isLastRec);
		if ($(rowObj).parents("tr").next("tr").hasClass("cus-accord")) { // destroy accordion
		  if ($("#pager").is(":visible") || isLastRec != 'last') {
			  if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
			  $(rowObj).parents("tr").children("td").css("border-bottom","solid 1px #ededed");
			  }else{
			 $(rowObj).parents("tr").children("td").css("border-bottom","dotted 1px #ccc");
	  	  }
		  }
			$(rowObj).parents("tr").next("tr").remove();
		} else {
			$('#user-detail-'+userId).show();
			/*var html = "<div id='paintUserContentResults"+userId+"'></div>";
	        $("#paintUserContentResults"+userId).remove();
	        $(rowObj).append(html);
	        this.createLoader("paintUserContentResults"+userId);*/
			this.renderClassResults(userId,rowObj);
		}
	 }catch(e){
			// to do
		}
	},
	
	reloadDataGridMyTeam : function(userId) {
		try{
		$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
		$("#paintMyclassContentResults"+userId).setGridParam({url: this.constructUrl("learning/myteam-search/class-details/"+userId)});
	    $("#paintMyclassContentResults"+userId).trigger("reloadGrid",[{page:1}]);
		}catch(e){
			// to do
		}
	},
	/*
	 * To render the registered class(learning) for a particular user at manager dash board
	 */
	renderClassResults : function(userId,rowObj) {
	 try{
	  var isLastRec = $("#viewLearningCls_" + userId).data('is_last_rec');
	  //console.log('isLastRec = ' + isLastRec);
	  var noBorderLastRecClass = ($("#pager").is(":visible") || isLastRec != 'last')? '' : 'noborder';
	  //console.log('noBorderLastRecClass = ' + noBorderLastRecClass);
		var cusHTML = '<table style="display:none;" id="paintMyclassContentResults'+userId+'"></table><div style="display:none;" id="pager2_user'+userId+'"></div>';
		var cRow = $(rowObj).parents("tr").after("<tr class='cus-accord " + noBorderLastRecClass + "'><td colspan='3'>"+cusHTML+"</td></tr>");
		
		if ($("#pager").is(":visible") || isLastRec != 'last') {
		  $(rowObj).parents("tr").children("td").css("border-bottom","none 0px");
		}
		//console.log("rowCreated");
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var teamGinWidth = 688;	
		}else{
			var teamGinWidth = 760;
	    }
		var dateTitle	= Drupal.t('LBL042')+'<p class="usr-enroll-grid-header-grey-cls">(' + Drupal.t('LBL699') + ')</p>';
		var objStr = '$("#lnr-myteam-search").data("lnrmyteamsearch")';
		var pagerId	= '#pager2_user'+userId;
		$("#paintMyclassContentResults"+userId).jqGrid({
			url:this.constructUrl("learning/myteam-search/class-details/"+userId),
			datatype: "json",
			mtype: 'GET',
			colNames:[Drupal.t('LBL107'),Drupal.t('LBL036'),Drupal.t('LBL102'),dateTitle ,Drupal.t('LBL108')],
			colModel:[ {name:'Name',index:'class_title', title:false, width:150,resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintLPMyclassDetails},
			           {name:'Type',index:'type', title:false, width:85,resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintLPMyclassDetails},
			           {name:'Status',index:'status', title:false, width:180,resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintLPMyclassDetails},
			           {name:'Date',index:'completed_on', title:false, width:80,height:40, resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintLPMyclassDetails},
			           {name:'Action-For',index:'action', title:false, classes: "course-status-action", width:85,resizable:false,sortable:false,'widgetObj':objStr,formatter:obj.paintLPMyclassDetails}],
			rowNum:5,
			rowList:[5,10,15],
			pager: pagerId,
			viewrecords:  true,
			emptyrecords: "",
			sortorder: "desc",
			toppager:true,
			height: 'auto',
			sortname:'enrolled_on',
			width: teamGinWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			onSortCol:function (){
				$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
			},
			loadComplete:obj.callbackMyclassLoader,
			gridComplete:function (){
				if($("#paintMyclassContentResults"+userId).getDataIDs().length == 0) {
					$("#jqgh_paintMyclassContentResults"+userId+"_Name").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Enrolled-On").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Type").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Status").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Date").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Action").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintMyclassContentResults"+userId+"_Name > span").css('display','none');
					$("#jqgh_paintMyclassContentResults"+userId+"_Enrolled-On > span").css('display','none');
					$("#jqgh_paintMyclassContentResults"+userId+"_Type > span").css('display','none');
					$("#jqgh_paintMyclassContentResults"+userId+"_Status > span").css('display','none');
					$("#jqgh_paintMyclassContentResults"+userId+"_Date > span").css('display','none');
					$("#jqgh_paintMyclassContentResults"+userId+"_Action > ").css('height','38px');
		
				}
				$("#jqgh_paintMyclassContentResults"+userId+"_Date").addClass("enroll-date-sortalign");
				this.currTheme = Drupal.settings.ajaxPageState.theme;
			 	if(this.currTheme == "expertusoneV2"){
			 	$("#paintMyclassContentResults"+userId+"_Name").css('width','253px');
			 	$("#jqgh_paintMyclassContentResults"+userId+"_Name").css('text-indent','11px');
				//("#paintMyclassContentResults"+userId+"_Enrolled-On").css('width','98px');
				$("#paintMyclassContentResults"+userId+"_Type").css('width','98px');
				$("#paintMyclassContentResults"+userId+"_Status").css('width','118px');
				$("#paintMyclassContentResults"+userId+"_Date").css('width','101px');
				$("#paintMyclassContentResults"+userId+"_Action-For").css('width','90px');
				/*table td alignment*/
				$("#paintMyclassContentResults"+userId).find("tr td:first-child").css('width','245px').css('text-indent','0').css('padding-left','10px');
				$("#paintMyclassContentResults"+userId).find("tr td:nth-child(2)").css('width','100px');
				$("#paintMyclassContentResults"+userId).find("tr td:nth-child(3)").css('width','120px');
				$("#paintMyclassContentResults"+userId).find("tr td:nth-child(4)").css('width','100px');
				$("#paintMyclassContentResults"+userId).find("tr td:last-child").css('width','94px');
				
				$("#gview_paintMyclassContentResults"+userId).find(" .ui-jqgrid-htable .ui-jqgrid-sortable").css('height','26px');
				$("#jqgh_paintMyclassContentResults"+userId+"_Action").css('height','26px');
			 	}else
			 	{
			 		var lang = Drupal.settings.user.language;				 		
			 	$("#paintMyclassContentResults"+userId).find("tr td:last-child").css('width','95px').css('padding-left','13px');	
			 	//$("#paintMyclassContentResults"+userId).find("tr th:nth-child(1)").css('width','');						 			
			 	if(lang == 'de' || lang == 'pt-pt'){
			 		 $("#lnr-myteam-search .myteam-learning-course-btn .full-button-without-action").css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat scroll -2px -351px rgba(0, 0, 0, 0)');			 		
				 }		
										
				$("#gview_paintMyclassContentResults"+userId).find(" .ui-jqgrid-htable .ui-jqgrid-sortable").css('height','28px');
				$("#jqgh_paintMyclassContentResults"+userId+"_Action").css('height','28px');
				
				
			 	}
			 	/* fix for ticket 0033364 */
				if($.browser.msie && parseInt($.browser.version, 10)=='9'|| $.browser.msie && parseInt($.browser.version, 10)=='8'){
				   $("#paintMyclassContentResults"+userId).css('float','left');
				}
			}
		}).navGrid(pagerId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
		$("#paintMyclassContentResults"+userId+'_toppager').hide();
		$("#jqgh_paintMyclassContentResults"+userId+"_Action-For").removeClass('ui-jqgrid-sortable');
		this.paintMyclassAfterReady(userId);
		$('.ui-jqgrid-bdiv > div').css('position','');
	 }catch(e){
			// to do
		}
	},
	/*
	 * Call back function after rendering class details.
	 */
	callbackMyclassLoader: function(response, postdata, formid){
		try{
		$("#paintMyclassContentResults"+response.userId).show();
		$("#gview_paintMyclassContentResults"+response.userId+" > .ui-jqgrid-hdiv").css("display","block");
		$("#gview_paintMyclassContentResults"+response.userId+" > .ui-th-column").css("text-align","right");
		$("#refresh_paintMyclassContentResults"+response.userId).css("display","none");
		
		$("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('searchRecordsPaint');

		// Ticket #6747 - Hide the pagination, if the record count in the view learning table is equal to or less than
		var recordCount = $('#paintMyclassContentResults'+response.userId).jqGrid('getGridParam', 'records');
		if(recordCount > 5){ 
			$('#pager2_user'+response.userId).show();

			$("#pager2_user"+response.userId+"_center .ui-pg-table td").css("border-bottom","0px none").eq(-1).hide();
			$("#pager2_user"+response.userId+"_left").hide();
			$("#pager2_user"+response.userId+"_right").hide();
			$("#pager2_user"+response.userId+"_center").css({"text-align":"right"}).children(".ui-pg-table").css("float","right");
			//$("#pager2_user"+response.userId+"_center").find(".ui-icon-seek-prev, .ui-icon-seek-next").parent().removeClass("ui-state-disabled");
			//$("#first_pager2_user"+response.userId).add("#last_pager2_user"+response.userId).hide();
			$("#first_pager2_user"+response.userId).addClass("pager-first");
			$("#last_pager2_user"+response.userId).addClass("pager-last");
			$("#next_pager2_user"+response.userId).addClass("pager-next");
			$("#prev_pager2_user"+response.userId).addClass("pager-prev");
			$('.ui-state-disabled').css('width','');
		}
		
		var recs = parseInt($("#paintMyclassContentResults"+response.userId).getGridParam("records"),10);
        if (recs == 0) {
        	$("#paintMyclassContentResults"+response.userId). css('display','block');
            var html = Drupal.t("MSG583");
            $("#paintMyclassContentResults"+response.userId).html('<tr><td class="border-style-none"><span class="myclass-no-records-cls">'+html+'</span></td></tr>');
            $('#paintMyclassContentResults'+response.userId).css('text-align','center');
                    //Ticket #6747 - Hide the pagination, if the record count in the view learning table is equal to or less than
            // Border issue
            $('.border-style-none').css('border','0');
        }
        $("#paintMyclassContentResults"+response.userId+" tr:nth-child(odd)").addClass("jqgrow-even");
        $('#paintMyclassContentResults'+response.userId+' .jqgfirstrow > td').css("border-bottom","none");
        // $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('paintUserClassContentResults'+response.userId);
        $('#user-detail-'+response.userId).hide();
        $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('paintClassContentResults'+response.userId);
        $("#paintClassContentResults"+response.userId).removeClass('paint-class-content-loader');
        /* 0009247: My team issues-----IE-7  */
       // $('#gview_paintMyclassContentResults'+response.userId+' div').remove('.loader-for-ie-fix');
        // Ticket #6747 - Hide the pagination, if the record count in the view learning table is equal to or less than
        if(recordCount > 5){ 
	        $('#pager2_user'+response.userId).appendTo("#gview_paintMyclassContentResults"+response.userId);
	        $('#pager2_user'+response.userId+'_center').css('border-bottom','none');
		}
        this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
        $('.cus-accord').find('.ui-jqgrid-bdiv').css('border','1px solid #E5E5E5').css('border-width','0 1px 1px 1px');
	 	}else
	 	{
	 	$('.cus-accord').find('.ui-jqgrid-bdiv').css('border','1px solid #cccccc');	
	 	}
	    /*if($.browser.msie && $.browser.version == 8) {
	 		$("#paintMyclassContentResults"+response.userId+"_Name").css('width','366px');
	 		$("#paintMyclassContentResults"+response.userId+"_Date").css('width','99px');
	 		$("#paintMyclassContentResults"+response.userId).find("tr:first-child td:first-child").css('width','366px');
	 	}*/

		//Vtip-Display toolt tip in mouse over
		 vtip();		 
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint the class information in the data grid
	 */
	paintLPMyclassDetails: function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		
	    var mroImageClass = '';
		var mroImageClassArr = new Array();
		mroImageClassArr['cre_sys_inv_com'] =  '<span class="mro-myteam-class course-compliance-bg vtip" title="'+Drupal.t("Compliance")+'">C</span>';
	   	mroImageClassArr['cre_sys_inv_man'] =  '<span class="mro-myteam-class course-mandatory-bg vtip" title="'+Drupal.t("Mandatory")+'">M</span>';
	   	// Commented For Optional icon is Showing in View Learning. For this Ticket #0039033
        // mroImageClassArr['cre_sys_inv_opt'] =  '<span class="mro-myteam-class  vtip" title="'+Drupal.t("Optional")+'">O</span>';
	    mroImageClassArr['cre_sys_inv_opt'] =  '';
        mroImageClassArr['cre_sys_inv_rec'] =  '<span class="mro-myteam-class course-recommended-bg vtip" title="'+Drupal.t("Recommended")+'">R</span>';
        
        var mroSt = rowObject['mro_status'];
 		var IsCompliance = rowObject['is_compliance'];
 		var exempted_sts = rowObject['exempted_sts'];
        if(mroSt == '' || mroSt == null || mroSt == 'null'){
        	mroImageClass = '';
        } else {        	
        	mroImageClass = mroImageClassArr[mroSt];
        }
		/* Changes for mandatory to be displayed for classes assigned by the manager after checking "mandatory" checkbox in assign learning
         * ticket 16261
         * */
        if(rowObject['mand_status'] == "1" || rowObject['mand_status'] == "Y"){
        	if(exempted_sts) {
        		mroImageClass = '';
        	}else {
        		mroImageClass = mroImageClassArr['cre_sys_inv_man'];
        	}
    	}else {
    		mroImageClass = ''; // Fixed For this Ticket #0046390
    	}
    	if(IsCompliance){
        	if(exempted_sts) {
        		mroImageClass = '';
        	}else {
        		mroImageClass = mroImageClassArr['cre_sys_inv_com'];
        	}
        }
		if(index  == 'class_title') {
			return "<div><span style='float:left;'>"+rowObject['class_title']+'</span>'+mroImageClass+'</div>';
		} else if(index  == 'enrolled_on') {
			return rowObject['enrolled_on'];
		} else if(index  == 'completed_on') {
			return rowObject['completed_on'];
		} else if(index  == 'type') {
			var row_del_type;
			if(rowObject['type'] == 'Web-based'){
	            row_del_type = Drupal.t("Web-based");
	        }
	        else if(rowObject['type'] == 'Virtual Class'){
	            row_del_type = Drupal.t("Virtual Class");
	        }
	        else if(rowObject['type'] == 'Classroom'){
	            row_del_type = Drupal.t("Classroom");
	        }
	        else if(rowObject['type'] == 'Video'){
	            row_del_type = Drupal.t("Video");
	        }
	        else if(rowObject['type'] == 'Certification'){
	            row_del_type = Drupal.t("Certification");
	        }
	        else if(rowObject['type'] == 'Curricula'){
	            row_del_type = Drupal.t("Curricula");
	        }
	        else if(rowObject['type'] == 'Learning Plan'){
	            row_del_type = Drupal.t("Learning Plan");
	        }
			return '<div><span class="vtip" title="'+row_del_type+'">'+titleRestrictionFadeoutImage(row_del_type,'exp-sp-myteam-row-del-type')+'</span></div>';
		} else if(index  == 'status') {
			if(rowObject['type'] == 'Learning Plan' || rowObject['type'] == 'Curricula' || rowObject['type'] == 'Certification'){
				if(exempted_sts && (rowObject['comp_status']=='lrn_tpm_ovr_enr' || rowObject['comp_status']=='lrn_tpm_ovr_inp')) {
	        		return Drupal.t('Waived')
	        	}else {
	        		return rowObject['status'];
	        	}
			}else{
	        	if(exempted_sts && (rowObject['comp_status']=='lrn_crs_cmp_enr' || rowObject['comp_status']=='lrn_crs_cmp_inp')) {
	        		return Drupal.t('Waived')
	        	}else {
	        		return rowObject['status'];
	        	}
			}
		} else {
			return rowObject['action'];
		}	
		}catch(e){
			// to do
		}
	},
	/*
	 * To show the drop-down when we click on 'View Learning' arrow
	 */
	showMoreAction : function(obj) {
		try{
		var position = $(obj).position();
		var posLeft  = Math.round(position.left);
		var posTop   = Math.round(position.top);
		$('.myteam-more-action').hide();
		//if(this.prevMoreObj != obj || this.prevMoreObj == undefined){
			$(obj).siblings('.myteam-more-action').css('position','absolute');
			$(obj).siblings('.myteam-more-action').css('left',posLeft-97);
			$(obj).siblings('.myteam-more-action').css('top',posTop+21);
			$(obj).siblings('.myteam-more-action').css('width','113');
			$(obj).siblings('.myteam-more-action').slideToggle("");
			this.prevMoreObj = obj;
/*		}else{
			this.prevMoreObj = '';
		}
*/	
		}catch(e){
			// to do
		}
	},
	/*
	 * To hide the drop-down when we click on 'View Learning' arrow
	 */
	hideMoreAction : function(obj) {
		try{
/*		$('body').click(function (event) {
			if(event.target.className!='myteam-learning-arrow') {
				$('.myteam-more-action').hide();
			}
		});
*/		this.prevMoreObj = '';
		}catch(e){
			// to do
		}
	},

  hideCatalogPageControls : function(hideAll) {
	try{
    var lastDataRow = $('#tbl-paintCatalogContentResults tr.ui-widget-content').filter(":last");
    //console.log(lastDataRow.length);
    
    if (hideAll) {
      $('#pager1').hide();
      
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hideCatalogPageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      //console.log('hideCatalogPageControls() : hide only next/prev page control');
      $('#pager1').show();
      $('#pager1 #pager1_center #first_pager1').hide();
      $('#pager1 #pager1_center #last_pager1').hide(); 
      $('#pager1 #pager1_center #next_pager1').hide();
      $('#pager1 #pager1_center #prev_pager1').hide();              
      $('#pager1 #pager1_center #sp_1_pager1').parent().hide();
    }
	}catch(e){
		// to do
	}
  },
  
  showCatalogPageControls : function() {
	try{   
    //console.log('showCatalogPageControls() : show all control');
    $('#pager1').show();
    $('#pager1 #pager1_center #first_pager1').show();
    $('#pager1 #pager1_center #last_pager1').show();
    $('#pager1 #pager1_center #next_pager1').show();
    $('#pager1 #pager1_center #prev_pager1').show();              
    $('#pager1 #pager1_center #sp_1_pager1').parent().show();
	}catch(e){
		// to do
	}
  },
	
	/*
	 * To render all classes to show it on pop-up 
	 */
	showLearningCatalog : function(userId,userName,rowObj){
	 try{	
	  this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var assignLWidth = 750 ;	
		}else{
			var assignLWidth = 688;
	    }
		$("#lnr-myteam-search").data("lnrmyteamsearch").defaults.catStart = true;
		this.render_popup(userId,userName,rowObj);
		$('#paintCatalogContentResults'+userId).css('min-height','auto');
		this.createLoader('paintCatalogContentResults'+userId);
		$('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
		
		var nAgt = navigator.userAgent;
		var verOffset ;
		var DetailsW = 350;		

		$('.page-learning-myteam-search').addClass('assign-learning');				
		var obj = this;
		var objStr = '$("#lnr-myteam-search").data("lnrmyteamsearch")';
		$("#tbl-paintCatalogContentResults").jqGrid({
			url:this.constructUrl("learning/myteam-search/catalog/"+userId),
			datatype: "json",
			mtype: 'GET',
			colNames:['Icons','Details','Action'],
			colModel:[ {name:'Icons',index:'Icons', title:false, width:70,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
			           {name:'Details',index:'Details', title:false, width:DetailsW,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
			           {name:'Action',index:'Action', title:false, width:165,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
			rowNum:5,
			rowList:[5,10,15],
			pager: '#pager1',
			viewrecords:  true,
			emptyrecords: "",
			sortorder: "desc",
			toppager:true,
			height: 'auto',
			width: assignLWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			loadComplete:obj.callbackCatalogLoader,
			gridComplete:obj.callbackGridComplete
		}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		this.paintCatalogAfterReady(userId);
		if(this.currTheme == "expertusone"){
		$("#my-team-dialog #tbl-paintCatalogContentResults").css("width","675px");
		}
	 }catch(e){
			// to do
	 }
	},
	
	/*
	 * Call back after rendering the classes in the pop-up
	 */
	callbackCatalogLoader: function(response, postdata, formid){
	 try{
		$("#tbl-paintCatalogContentResults").show();
		//add class for first and  last row cell
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:first-child').addClass('assign-learn-first-row');
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:nth-child(2)').addClass('assign-learn-second-row');
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:last-child').addClass('assign-learn-last-row');
		var recs = parseInt($("#tbl-paintCatalogContentResults").getGridParam("records"),10);
	  //console.log('callbackCatalogLoader() : recs = ' + recs);
    if (recs == 0) {
      $('#catalog-no-records'). css('display','block');
      var html = Drupal.t("MSG381")+'.';
      $("#catalog-no-records").html(html);            
    } else {
      $('#catalog-no-records'). css('display','none');
      $("#catalog-no-records").html("");
    }
    
    var obj = $("#lnr-myteam-search").data("lnrmyteamsearch");

    // Show pagination only when search results span multiple pages
    var hideAllPageControls = true;
    if (recs > 5) { // 5 is the least view per page option.
      hideAllPageControls = false;
      obj.showCatalogPageControls();
      $('#my-team-dialog .popupBotLeftCorner').hide();
      //console.log('callbackCatalogLoader() : hideAllPageControls set to false');
    }else{
    	obj.hideCatalogPageControls(hideAllPageControls);
    	$('#my-team-dialog .popupBotLeftCorner').show();
    }

    if($("#lnr-myteam-search").data("lnrmyteamsearch").defaults.catStart) {
			$("#paintCatalogHeader").html(response.header);
			$("#myteam-find-trng-sort-display").show();
			$("#lnr-myteam-search").data("lnrmyteamsearch").defaults.catStart = false;
    }

    var curHeight = parseInt($(".ui-dialog").eq(-1).css("height")) + parseInt($(".ui-dialog").eq(-1).css("top"));
    var curOlay = parseInt($(".ui-widget-overlay").eq(-1).css("height"));
    if(curOlay < curHeight) {
      $(".ui-widget-overlay").eq(-1).css("height", (curHeight + 50) + "px");
    }
        
	  $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('paintCatalogContentResults'+response.userId);
	  $('#my-team-dialog #tbl-paintCatalogContentResults').find('tr:last-child td').css('border','0px'); 
	//Vtip-Display toolt tip in mouse over
	    vtip();
	    resetFadeOutByClass('','sub-attributes','line2-attribute','MyTeam');
	    $('.limit-title').trunk8(trunk8.myteam_title);
		$('.limit-desc').trunk8(trunk8.myteam_desc);
		scrollBar_Refresh('myteam');
	 }catch(e){
			// to do
	 }
	},
	
	callbackGridComplete: function(response, postdata, formid){
		// manager assign mandatory enrollment to user, adjust title width in the assign learn grid
		/*$('#my-team-dialog .course-container-list .top-record-div-left').each(function(index, elem) {
		  var manObj = $(this).find('.catalog-course-role-access-bg');
		  if (manObj.attr('class') !== undefined )
		  {
			var fadeoutContainer = $(this).find('.fade-out-title-container');
			fadeoutContainer.removeClass('exp-sp-myteam-catalog-search-class-title-fadeout-container');
			fadeoutContainer.addClass('myteam-mandatory-course-title-fadeout-container');
			if (fadeoutContainer.width() > 326) {
			  $(this).find('.fade-out-title-container .title-lengthy-text').append('<span class="fade-out-image"></span>');
			}
		  }
		});*/		
		//scrollBar_Refresh('myteam');
		 $('.limit-title').trunk8(trunk8.myteamname_title);
		scrollBar_Refresh('myteam');
	},
	
	/*
	 * To show the pop-up with the rendered classes
	 */
	render_popup : function(id,userName,rowObj) {
		try{
		var obj	= this;
		 this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
				var assignLqtipWidth = 750 ;	
			}else{
				var assignLqtipWidth = 712;
		    }
		var html = "<div id='paintCatalogContentResults"+id+"'></div>";
        $("#paintCatalogContentResults"+id).remove();
        $('body').append(html);
        var nHtml = "<div id='show_expertus_message'></div><div id='paintCatalogHeader'></div><div id='catalog-no-records' style='display: none'></div><div class='course-container-list'><table id='tbl-paintCatalogContentResults'></table></div><div id='paintCatalogFooter'><div id='pager1' style='display:none;'></div></div>";
        $('#paintCatalogContentResults'+id).html(nHtml);
        var closeButt={};
        var assignToTxt = Drupal.t("LBL085");
        $("#paintCatalogContentResults"+id).dialog({
        	autoResize: true,
        	position:[(getWindowWidth()-assignLqtipWidth)/2,100],
            bgiframe: true,
            width:assignLqtipWidth,
            resizable:false,
            draggable:false,
            closeOnEscape: false,
            modal: true,
            title:SMARTPORTAL.t('<span class="myteam-header-text">' + assignToTxt +'</span><div id="search-cat-keyword"></div>'),
            buttons: closeButt,
            close: function(){
                $("#paintCatalogContentResults"+id).remove();
                $("#my-team-dialog").remove();
                obj.viewLearning(id,rowObj);
                $('.ac_results').remove();	//change by ayyappan for 40055: Team view assign learning pop up if searching any class name and once pop up closed means but still shows class name
                $(".page-learning-myteam-search").removeClass("assign-learning");
                if($('.location-session-detail-clone').size()>0){
                	$('.location-session-detail-clone').remove();
                }
            },
            overlay: {
 	           opacity: 0.5,
	           background: "#000000"
             },
            open: function(){
            	 $(".ui-widget-overlay").eq(-1).css("width",getWindowWidth()-1+"px");
             }
        });
        //$('.ui-dialog').css('border','solid 10px #5F5F5F');
        $('.ui-dialog').wrap("<div id='my-team-dialog'></div>");
        $("#paintCatalogContentResults"+id).css("position","");
        this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
	 	$('#my-team-dialog').find('.ui-dialog-content').addClass('assign-learning-team');	
	 	$('#my-team-dialog').find('.ui-dialog').css("overflow","visible");
	    changeDialogPopUI();
	   }
		}catch(e){
			// to do
		}
    },
    /*
     * To paint class icon on data grid
     */
	paintLPSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint the class details section on data grid
	 */
	paintLPSearchResults : function(cellvalue, options, rowObject) {
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint class action on data grid
	 */
	paintLPSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},
	
	/*
	 * Action to performed during class search in the pop-up
	 */
	catalogSearchAction : function(sortbytxt,className,userId) {
		try{
			//remove existing expertus message
		$('#show_expertus_message').html('');
		var searchStr = '';
		/*-------Title-------*/
		var title 	  = $('#srch_criteria_catkeyword').val();
		if(title == (Drupal.t('LBL970') +' '+ Drupal.t('LBL983')))
			title='';
		else		
			$('#cat-title-clr').css('display','block');
		
		/*-------Delivery Type-------*/
		var dl_type 	  = $('#myteam-cat-delivery-type-hidden').val();
		if(dl_type == Drupal.t("LBL428"))
			dl_type='';
		else		
			$('#cat-title-clr').css('display','block');	
		
		/*-------Sort By-------*/
		var sortby = sortbytxt;
		searchStr	= '&title='+encodeURIComponent(title)+'&dl_type='+encodeURIComponent(dl_type)+'&sortby='+sortby;
		this.searchStrValue = searchStr;

	  $("#lnr-myteam-search").data("lnrmyteamsearch").hideCatalogPageControls(true);
		//$('#tbl-paintCatalogContentResults').hide();
		
    this.createLoader('paintCatalogContentResults'+userId);
    $('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');
    	
		$('#tbl-paintCatalogContentResults').setGridParam({url: this.constructUrl('learning/myteam-search/catalog/'+userId+'/'+searchStr)});
	    $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:1}]);
	    //Highlight sort type 
		$('#paintCatalogHeader .sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		if(className!=''){
			$('.'+className).addClass('sortype-high-lighter');
		}else{
			$('#paintCatalogHeader .catalog-type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},
	/*
	 * Actions to be performed during user search at manager dash board
	 */
	searchAction : function(sortbytxt,className) {
		try{
		var searchStr = '';		
		var cy_type = '';
		$('.country-others').each(function(){
			if($(this).is(':checked')){
				cy_type+=cy_type!=''?'|':'';
				cy_type+=$(this).val();
			}
		});

		/*-------Title-------*/
		var title 	  = $('#search_searchtext').val();
			if(title == Drupal.t("LBL304").toUpperCase())
				title='';
		
		/*-------Location-------*/
		var location 	= $('#srch_criteria_location').val();
			if(location == Drupal.t('LBL114'))
				location='';
			else
				$('#location-clr').css('display','block');

		/*-------Username-------*/
		var username 	= $('#srch_criteria_username').val();
			if(username == Drupal.t('LBL181'))
				username='';
			else
				$('#username-clr').css('display','block');

		/*-------Sort By-------*/
		var sortby = sortbytxt;
		
		var reporttype = '';
		$('.report-others').each(function(){
			if($(this).is(':checked')){
				reporttype+=reporttype!=''?'|':'';
				reporttype+=$(this).val();	
			}

		});
		/*------Header Search------*/
		var headersearch 	= $('#myteam_searchtext').val();
		if(headersearch.toLowerCase() == (Drupal.t('LBL304').toLowerCase()))
			headersearch='';
		
		// Note: exp_sp_myteam.inc:setSearchParam() is expecting @full_name which is not passed below instead of @title.
		// However, currently the top search filter shown in my team page is for catalog search.
		// Remove the username filter in the narrow search filter - ticket no : 28622
		//searchStr	= '&title='+encodeURIComponent(title)+'&reporttype='+reporttype+'&location='+location+'&username='+username+'&cy_type='+cy_type+'&sortby='+sortby+'&searchusername='+headersearch;
		searchStr	= '&title='+encodeURIComponent(title)+'&reporttype='+reporttype+'&location='+location+'&cy_type='+cy_type+'&sortby='+sortby+'&searchusername='+headersearch;
		this.searchStrValue = searchStr;
		
		// Close all open accordions
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
		$('#paintContentResults tr.cus-accord:not(.noborder)').prev('tr').children("td").css("border-bottom","1px solid #EDEDED");
	 	}
	 	else
	 	{
		$('#paintContentResults tr.cus-accord:not(.noborder)').prev('tr').children("td").css("border-bottom","dotted 1px #ccc");
	 	}
		$('#paintContentResults tr.cus-accord').remove();
		
    this.createLoader('searchRecordsPaint');

		$('#paintContentResults').setGridParam({url: this.constructUrl('learning/myteam-search/search/all/'+searchStr)});
	    $("#paintContentResults").trigger("reloadGrid",[{page:1}]);
	    
	    this.checkboxValidation();
	    //Highlight sort type 
		$('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		if(!className){
			$('.type1').addClass('sortype-high-lighter');
		}else{
			$('.'+className).addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		}
	},
	/*
	 * To clear the search filter on class search widget on pop-up
	 */
	clearCatalogField : function (userId) {
		try{
		$('#srch_criteria_catkeyword').val('Type title or code').css('color','#999999').css('fontSize','11px');
		if(this.currTheme != "expertusoneV2"){
			$('#srch_criteria_catkeyword').css('fontStyle','italic');
		}
		$('#myteam-cat-delivery-type-hidden').val('Any');
		$('#cat-title-clr').css('display','none');
		this.catalogSearchAction('','',userId);
		//$('#srch_criteria_catkeyword').flushCache();
		}catch(e){
			// to do
		}
	},
	/*
	 * To clear the search filters on user search widget at manager dash board
	 */
	clearField : function (txt) {
		try{
		if(txt=='Location'){
			$('#srch_criteria_location').val(Drupal.t("LBL114")).css('color','#999999').css('fontSize','11px');
			if(this.currTheme != "expertusoneV2"){
				$('#srch_criteria_location').css('fontStyle','italic');
			}
			$('#location-clr').css('display','none');
		}else if(txt=='Username'){
			$('#srch_criteria_username').val(Drupal.t("LBL181")).css('color','#999999').css('fontSize','11px');
			if(this.currTheme != "expertusoneV2"){
				$('#srch_criteria_username').css('fontStyle','italic');
			}
			$('#username-clr').css('display','none');
		}else if(txt=='catalogKeyword'){
			$('#srch_criteria_catkeyword').val((Drupal.t('LBL970')+' '+ Drupal.t('LBL983'))).css('color','#999999').css('fontSize','11px');
			if(this.currTheme != "expertusoneV2"){
				$('#srch_criteria_username').css('fontStyle','italic');
			}
			$('#catkeyword-clr').css('display','none');
		}
		this.searchAction();
		}catch(e){
			// to do
		}
	},
	/*
	 * To show and hide the sub filter info of narrow search section
	 */
	showHide : function (strOne,strTwo) {
		try{
		$('#'+strTwo).toggle();
		var classShowHide = $('#'+strOne).hasClass('cls-show');
		if(classShowHide){
			$('#'+strOne).removeClass('cls-show');
			$('#'+strOne).addClass('cls-hide');
		}else{
			$('#'+strOne).removeClass('cls-hide');
			$('#'+strOne).addClass('cls-show');
		}
		$('#srch_criteria_catkeyword').flushCache();
		}catch(e){
			// to do
		}
	},
	/*
	 * Checkbox validation on narrow search section
	 */
	checkboxValidation : function() {
		try{
		$('#searchopts-content').find('input[type=checkbox]').each(function() {
			if($(this).is(':checked')){			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-blue');
					$(this).parent().next('label').addClass('highlight-light-gray');
					$(this).parent().removeClass('checkbox-unselected');				
					$(this).parent().addClass('checkbox-selected');
				}
			} else {			
				if($(this).val() != 'All'){
					$(this).parent().next('label').removeClass('highlight-light-gray');
					$(this).parent().next('label').addClass('highlight-light-blue');
					$(this).parent().removeClass('checkbox-selected');				
					$(this).parent().addClass('checkbox-unselected');
				}
			}
		});	
		}catch(e){
			// to do
		}
	},
	/*
	 * To show more information on search filter at narrow search section
	 */
	moreListDisplay : function(recLen,dispId) {
		try{
		for(i=0;i<=recLen;i++){
			$('#'+dispId+'_'+i).css("display","block");
		}
		$('#'+dispId+'_more').css("display","none");
		$('#'+dispId+'_short').css("display","block");
		}catch(e){
			// to do
		}
	},
	/*
	 * To show short information on search filter at narrow search section
	 */
	shortListDisplay : function(recLen,dispId){
		try{
		for(i=0;i<=recLen;i++){
			if(i<=4){
				$('#'+dispId+'_'+i).css("display","block");
				}
			else {
				$('#'+dispId+'_'+i).css("display","none");
				}
			}
		$('#'+dispId+'_short').css("display","none");
		$('#'+dispId+'_more').css("display","block");
		}catch(e){
			// to do
		}
	},
	
  hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintContentResults tr.ui-widget-content').filter(":last");
    //console.log(lastDataRow.length);
    
    if (hideAll) {
     if(this.currTheme == "expertusoneV2")
     $('#paintContent .block-footer-left').show();  
      $('#pager').hide();
      
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      //console.log('hidePageControls() : hide only next/prev page control');
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
     $('#paintContent .block-footer-left').hide(); 
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #last_pager').hide(); 
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #prev_pager').hide();              
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
	}catch(e){
		// to do
	}
  },
  
  showPageControls : function() {
	 try{  
    //console.log('showPageControls() : show all control');
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    $('#paintContent .block-footer-left').hide(); 
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #last_pager').show();
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #prev_pager').show();              
    $('#pager #pager_center #sp_1_pager').parent().show();
	 }catch(e){
			// to do
	}
  },

	/*
	 * To render current user's reportees information on manager dash board
	 */
	renderSearchResults : function(searchStr){	
	 try{	
	  this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var teamGWidth = 726;
			var iconWidth = 87;
			var detailWidth = 390;
			var actionWidth = 250;
		}else{
			var lang = Drupal.settings.user.language;
			if(lang == 'it') {
				var teamGWidth = 764;
				var iconWidth = 35;
				var detailWidth = 519;
				var actionWidth = 200;
				
			}else {
				var teamGWidth = 764;
				var iconWidth = 35;
				var detailWidth = 589;
				var actionWidth = 130;
			}
			
	    }
		this.createLoader('lnr-myteam-search');
		var obj = this;
		var urlStr = (searchStr != '') ? ('&title='+encodeURIComponent(searchStr)) : '';
		var objStr = '$("#lnr-myteam-search").data("lnrmyteamsearch")';
		$("#paintContentResults").jqGrid({
			url:obj.constructUrl("learning/myteam-search/search/all/"+urlStr),
			datatype: "json",
			mtype: 'GET',
			colNames:['Icons','Details','Action'],
			colModel:[ {name:'Icons',index:'Icons', title:false, width:iconWidth,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
			           {name:'Details',index:'Details', title:false, width:detailWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
			           {name:'Action',index:'Action', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
			rowNum:10,
			rowList:[10,20,30],
			pager: '#pager',
			viewrecords:  true,
			emptyrecords: "",
			toppager:true,
			height: 'auto',
			width: teamGWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			loadComplete:obj.callbackLoader
		}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		$('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('.type1').addClass('sortype-high-lighter');
	 	}catch(e){
			// to do
		}
	},
	/*
	 * Call back after rendering the user information.
	 */
	callbackLoader : function(response, postdata, formid){
		try{
		    $('#paintContentResults').show();
		    var recs = parseInt($("#paintContentResults").getGridParam("records"),10);
		    //console.log('callbackLoader() : recs = ' + recs);
        if (recs == 0) {
          if(($("#lnr-myteam-search").data("lnrmyteamsearch").searchStrValue == '') ||
        	             (($("#lnr-myteam-search").data("lnrmyteamsearch").searchAction("AZ","type1"))==null) ||
        	                  (($("#lnr-myteam-search").data("lnrmyteamsearch").searchAction("ZA","type2"))==null) ||
        	                         (($("#lnr-myteam-search").data("lnrmyteamsearch").searchAction("Time","type3"))==null)) {
                var html = Drupal.t("MSG413");
                $("#gview_paintContentResults"). css('display','none');
        	}
        	else {
                var html = Drupal.t("MSG381")+'.';
        	}
        	$('#no-records').css('display','block').css('padding','10px').css('color','#777777').css('textAlign','center').css('lineHeight','40px');
          $("#no-records").html(html);       
        }
        else {
        	$("#gview_paintContentResults"). css('display','block');
        	$('#no-records'). css('display','none');
        	$("#no-records").html("");
        }
        
        var obj = $("#lnr-myteam-search").data("lnrmyteamsearch");

        // Show pagination only when search results span multiple pages
        var reccount = parseInt($("#paintContentResults").getGridParam("reccount"), 10);
        var hideAllPageControls = true;
        if (recs > 10) { // 10 is the least view per page option.
           hideAllPageControls = false;
         
          //console.log('callbackLoader() : hideAllPageControls set to false');
        }
        
        //console.log('callbackLoader() : reccount = ' + reccount);
        if (recs <= reccount) {
          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
          obj.hidePageControls(hideAllPageControls);
         
        }
        else {
          //console.log('callbackLoader() : recs > reccount : show pagination controls');
           obj.showPageControls();
          
        }       
        
	      //$("#dummy_link").trigger("click");
        $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('lnr-myteam-search');
        $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('searchRecordsPaint');
		    if($("#lnr-myteam-search").data("lnrmyteamsearch").defaults.start) {
			    $("#search-filter-content").html(response.filter);
			    $("#find-trng-sort-display").show();
			    $("#lnr-myteam-search").data("lnrmyteamsearch").defaults.start = false;
		    } else {
			    $("#lnr-myteam-search").data("lnrmyteamsearch").unselectFilter(response);
		    }
		  $("#search-link-content").html(response.links);
		    //Vtip-Display toolt tip in mouse over
		    vtip();	
		    $('.limit-title').trunk8(trunk8.myteamname_title);
	        $('#lnr-myteam-search #paintCountry').jScrollPane({}); 
		}catch(e){
			// to do
		}
	},

	unselectFilter : function(response) {
		try{
			
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint the user avatar on user data grid
	 */
	paintLPSearchIcons : function (cellvalue, options, rowObject) {	
		try{
		return rowObject['image'];
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint the user information on user data grid
	 */
	paintLPSearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},
	/*
	 * To paint the user action on user data grid
	 */
	paintLPSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},	
	/*
	 * To call the search widget when select any location from filter section
	 */
	locationEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_location").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction();
			 }
		 });
		}catch(e){
			// to do
		}
	},
	/*
	 * To list all matched location for location autocomplete on filter section
	 */
	paintLocationAutocomplete : function(){
		try{
		$('#srch_criteria_location').autocomplete(
			"/?q=learning/myteam-search/location-autocomplete",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},
	/*
	 * To call the search widget when select any user name from filter section
	 */
	usernameEnterKey : function(){
		try{
		 obj = this;
		 $("#srch_criteria_username").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.searchAction();
			 }
		 });
		}catch(e){
			// to do
		}
	},
	/*
	 * To list all matched username for username autocomplete on filter section
	 */
	paintUsernameAutocomplete : function(){
		try{
		$('#srch_criteria_username').autocomplete(
			"/?q=learning/myteam-search/username-autocomplete",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},
	/*
	 * To call class search widget when enter class keywords on pop-up, 'Assign Learning' section
	 */
	catkeywordEnterKey : function(userId){
		try{
		 obj = this;
		 $("#srch_criteria_catkeyword").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.catalogSearchAction('','',userId);
			 }
		 });
		 
		}catch(e){
			// to do
		}
	},
	/*
	 * To list all matched classes on class/code autocomplete on class search at pop-up
	 * 'Assign Learning' section
	 */
	paintCatkeywordAutocomplete : function(userId){
		try{
		var obj	= this;
		var delType	= $('#myteam-cat-delivery-type-hidden').val();
		url = obj.constructUrl("learning/myteam-search/catalog-autocomplete/" + delType  +'/' + userId);
		$('#srch_criteria_catkeyword').autocomplete(
			url,{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass :"ac_input",
			loadingClass :"ac_loading"
		});
		}catch(e){
			// to do
		}
	},
	/*
	 * To highlight the default text when there is no text, in search filters
	 */
	highlightedText : function(ID,lang) {
	 try{	
		 if(lang == 'it' || lang == 'ru'){
			var font_size = '10px';			
		 }else{
			var font_size = '12px';
		 }
	  var defaultText = $('#' + ID).data('default-text');
		var obj = this;
		$("#"+ID).blur(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(defaultText).css('color','#999999').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
		$("#"+ID).focus(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == defaultText) {
        		$(this).val('').css('color','#333333').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
		$("#"+ID).change(function(){
   			if($("#"+ID).val() != defaultText) {
        		$(this).css('color','#333333').css('fontSize','13px').css('fontStyle','normal');
    		}
    		if($("#"+ID).val() == '') {
        		$(this).val(defaultText).css('color','#999999').css('fontSize',font_size);
        		if(obj.currTheme != "expertusoneV2"){
        			$(this).css('fontStyle','italic');
        		}
    		}
		});
	 }catch(e){
			// to do
	 }
	},	
	/*
	 * To assign a class to particualr user at manager dash board
	 * objType - Ticket no: 57382
	 */
	callEquivPopupForMyteam : function(userId, courseId, classId, rowObj, wailist, objType) {	
		try{
		$('#show_expertus_message').html('');
		var obj = this;
				url = obj.constructUrl("ajax/learningcore/check-equivalence/" + userId + '/' + courseId + '/' + classId);
				var IN_FLIGHT = 'requestinflight';
				var $ajaxcall;
				$ajaxcall = $( this );
               if( ! $ajaxcall.data( IN_FLIGHT ) ){
					$ajaxcall.data( IN_FLIGHT, true );
					$.ajax({
						type: "POST",
						url: url,
						data:  '',
						success: function(result){
							$ajaxcall.data( IN_FLIGHT, true );
							if(result.length > 0){
								obj.displayMyteamEquvPopup(userId, courseId, classId,rowObj,wailist,result,objType);
							}else{					
								obj.callAssignClass(userId, courseId, classId,rowObj,wailist,objType);
								
							}					
						},
					    complete: function()
					    {
							$ajaxcall.data( IN_FLIGHT, false );
					    }
					});
              }
		}catch(e){
			// to do
		}
	},
	
	
	displayMyteamEquvPopup : function(userId, courseId, classId,rowObj,wailist,result,objType){
		try{
	    $('#equvMsg-wizard').remove();
	    var html = '';
	    var title = Drupal.t('LBL247');
	    html+='<div id="equvMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	   // html+='<tr>';		
	   if( result[0].enrollexist != 'Y') {
	    if(result.length == 1)
	    	html+='<tr><td align="center" class="light-gray-color">'+Drupal.t('ERR192')+':'+'</td></tr>';
	    else
	    	html+='<tr><td align="center" class="light-gray-color">'+Drupal.t('ERR193')+':'+'</td></tr>';	

		for(k=0;k < result.length ; k++){
			html+= '<tr><td align="center"><i>"'+unescape(result[k]['equv_title'])+'"</i></td></tr>';
		}
	   }else if( result[0].tpenrollexist == 'Y') {
			html+='<tr><td align="center" class="light-gray-color">'+Drupal.t('MSG535')+''+'</td></tr>'; 
			title = Drupal.t('LBL721');
		}else{
		   html+='<tr><td align="center" class="light-gray-color">'+Drupal.t('LBL1271')+''+'</td></tr>'; 
		   title = Drupal.t('LBL721');
	   }
	   if( result[0].tpenrollexist != 'Y') {
		 html+= '<tr><td align="center" class="light-gray-color"><span>'+Drupal.t('ERR194')+'</span></td></tr>';	
	   }	
		
	    //html+='</tr>';	
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	     closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove();$('#my-team-dialog').remove();};
	     if( result[0].tpenrollexist != 'Y') {
	       closeButt[Drupal.t('Yes')]=function(){ $("#lnr-myteam-search").data("lnrmyteamsearch").callAssignClass(userId, courseId, classId,rowObj,wailist,objType); $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove();$('#my-team-dialog').show(); };
	     }
	    $("#equvMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:title,
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    $('#my-team-dialog').hide();
	    if(Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
		    $('#equvMsg-wizard').closest('.ui-dialog').wrap("<div id='myteam-equalence-dialog'></div>");
	    	$('#myteam-equalence-dialog').find('.ui-dialog-content').addClass('assign-equalence-team');
		  	changeChildDialogPopUI('myteam-equalence-dialog');
		}
	    $("#equvMsg-wizard").show();
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#equvMsg-wizard").remove();
	        $('#my-team-dialog').show();
	    });
		}catch(e){
			// to do
		}
	},
	
	callAssignClass : function(userId, courseId, classId, rowObj, wailist, objType) {
		try{
		 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		    	var esignObj = {'popupDiv':'my-team-dialog',
		    			'esignFor':'ManagerAssign','userId':userId, 'courseId':courseId,'classId': classId,'rowObj': rowObj,'wailist': wailist};
		    	 $.fn.getNewEsignPopup(esignObj); 		    	
		 }else{
			 $("#lnr-myteam-search").data("lnrmyteamsearch").renderCallAssignClass(userId, courseId, classId, rowObj, wailist, objType);			 
		 }
		}catch(e){
			// to do
		}
	},
	
	
	renderCallAssignClass : function(userId, courseId, classId, rowObj, wailist, objType){
		try{

		var obj = this;
		$("#class-detail-loader-"+classId).remove();
		var cRow = $(rowObj).parents("tr").parents("tr").before("<tr><td colspan='3'><div id='class-detail-loader-"+classId+"'></div></td></tr>");
		this.createLoader("class-detail-loader-"+classId);
		$('.disp-msg').hide();
		var mandatory=$('#assignClass_checkbox_'+classId+':checked').val();
		if(mandatory==undefined){
			mandatory='N';
		}
		url = obj.constructUrl("ajax/learning/myteam-search/assign-class/" + userId + '/' + courseId + '/' + classId + '/' + mandatory+'/'+wailist);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
	
			result = $.trim(result);				
			   if(userId!=0)

			   //if(result == 'Assigned') {
			   if((result == 'Assigned') || (result == 'Waitlisted')) {

				   var showAction = ((result == 'Assigned') ? Drupal.t("Registered") : ((result == 'Waitlisted') ? Drupal.t("LBL190") : ''));
				   if(result == 'Assigned') {
					 $("#assignClass_"+objType+"_"+classId).removeClass("action-btn");
				   } else if(result == 'Waitlisted') {
					   $("#assignClass_"+objType+"_"+classId).removeClass("action-btn-waitlist");
				   }
				   
				   obj.callAvailableSeats(classId,wailist,userId);
				   
				   $("#assignClass_"+objType+"_"+classId).removeAttr('onclick');
				   //$("#assignClass_"+classId).removeClass("action-btn");
				   $("#assignClass_"+objType+"_"+classId).addClass("action-btn-disable");
				   if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
					   $("#assignClass_"+objType+"_"+classId).html('<div class="white-btn-bg-left"></div><div class="white-btn-bg-middle">'+showAction+'</div><div class="white-btn-bg-right"></div>');
				   }else{
				   //$("#assignClass_"+classId).html(SMARTPORTAL.t("Registered"));
					   $("#assignClass_"+objType+"_"+classId).html(showAction);
				   }
				   $('#assignClass_checkbox_'+classId).attr("disabled", true);
			   }else{
			  
				   var resultErr = new Array();
				   resultErr[0]= result;
				   var message_call = expertus_error_message(resultErr,'error');
				  
				    $("#assignClass_"+objType+"_"+classId).html(SMARTPORTAL.t("Registered"));
				     $("#assignClass_"+objType+"_"+classId).addClass("action-btn-disable");
				   if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
					   $("#assignClass_"+objType+"_"+classId).html('<div class="white-btn-bg-left"></div><div class="white-btn-bg-middle">'+SMARTPORTAL.t("Registered")+'</div><div class="white-btn-bg-right"></div>');
				   }else{
				   //$("#assignClass_"+classId).html(SMARTPORTAL.t("Registered"));
					   $("#assignClass_"+objType+"_"+classId).html(showAction);
				   }
					$('#show_expertus_message').html(message_call);
					$('#show_expertus_message').show();
				   $('#assignClass_checkbox_'+classId).attr("checked", false);
			   }
			   $("#lnr-myteam-search").data("lnrmyteamsearch").destroyLoader('class-detail-loader-'+classId);
			}
		});
		}catch(e){
			// to do
		}
	},
	/*
	 * To assign a training program to particualr user at manager dash board
	 */
	callObjectAssignClass : function(userId, prgId, assignObject) {
		try{
		if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    	var esignObj = {'popupDiv':'my-team-dialog',
	    			'esignFor':'ManagerAssignProgram','userId' : userId,'prgId': prgId,'assignObject' : assignObject};
	    	 $.fn.getNewEsignPopup(esignObj); 	    	
		}else{
			$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallObjectAssignClass(userId, prgId, assignObject);			 
		}
		}catch(e){
			// to do
		}
	},	
	
	renderCallObjectAssignClass : function(userId, prgId, assignObject){
		try{
		var obj = this;
		loaderObj = 'class-detail-'+prgId;
		$('#'+loaderObj).show();
		$('.disp-msg').hide();
		var MasterMandatory=$('#assignPrg_checkbox_'+prgId+':checked').val();
		if(MasterMandatory==undefined){
			MasterMandatory='N';
		}
		var LMSNodeId = 0;
		var isCart = 0;
		var isAdminSide = 'N';
		var fromPage = 'MyTeam';
		var pageandmandatory = fromPage + "-" +MasterMandatory;
		
		if(!document.getElementById(prgId+"_cus-accord")) {
			//url = obj.constructUrl("ajax/learning/myteam-search/assign-object-class/" + userId + '/' + prgId + '/' + MasterMandatory +'/ee');
			  url = obj.constructUrl("ajax/trainingplan/class-cnt-for-course/"+prgId + "/" + LMSNodeId +"/" + isCart+"/" + isAdminSide+"/" + userId+"/" + pageandmandatory);
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
				result = $.trim(result);	
			   if(userId!=0)
					   obj.callAvailableSeats(prgId,0,userId); // waitlist functionality check parameter added
				   if(result == "MultiRegister"){
					   obj.renderAccordion(userId,'paintClassesSelectList'+prgId,assignObject);
				   }else if(result == 'Registered') {
					   $("#assignClass_"+prgId).removeAttr('onclick');
					   $("#assignClass_"+prgId).removeClass("action-btn");
					   $("#assignClass_"+prgId).addClass("action-btn-disable");
					   //$("#assignClass_"+prgId).html(SMARTPORTAL.t("Registered"));
					   if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
						   $("#assignClass_"+prgId).html('<div class="white-btn-bg-left"></div><div class="white-btn-bg-middle">'+Drupal.t('Registered')+'</div><div class="white-btn-bg-right"></div>');
					   }
					   var currentPage = $('.ui-pg-input').val();
					   $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:currentPage}]);
					   $('#assignPrg_checkbox_'+prgId).attr("disabled", true);
				   }else{
					   if(result == 'noclasses') {
						   result = Drupal.t("ERR061");
					   }
					   var resultErr = new Array();
					   resultErr[0]= result;
					   var message_call = expertus_error_message(resultErr,'error');
						$('#show_expertus_message').html(message_call);
						$('#show_expertus_message').show();
					   $('#assignPrg_checkbox_'+prgId).attr("checked", false);
					   
				   }
				   $('#'+loaderObj).hide();
				}
		    });
		}else{
			obj.renderAccordion(userId,'paintClassesSelectList'+prgId,assignObject);
		}
		}catch(e){
			// to do
		}
	},	
	
	/*
	 * #28817: In team, the status message of the prerequisite class is showing as 'Assign'	
	 * change by: ayyappans 
	 *
	 */
	renderTpPrequisteClass : function(userId, prgId, type,assignObject){
		try{
			var obj = this;
			loaderObj = 'assignClass_'+prgId;
			$('#'+loaderObj).show();
			$('.disp-msg').hide();
			var MasterMandatory=$('#assignPrg_checkbox_'+prgId+':checked').val();
			if(MasterMandatory==undefined){
				MasterMandatory='N';
			}

			var fromPage = 'MyTeam';
			obj.createLoader('paintCatalogContentResults'+userId);
			if($('#tbl-paintCatalogContentResults').find('.'+prgId+'-sub-result').length == 0) {
				url = obj.constructUrl("ajax/Prequiste/"+userId + "/" + prgId +"/" + type+"/" + fromPage);
				$.ajax({
					type: "POST",
					url: url,
					data:  '',
					success: function(result){
						$.each(result.rows, function() {
							if($(this.cell.preReqTpDetails).find('.error-string').length != 0) {
								var errorMessage = new Array();
								errorMessage[0] = $(this.cell.preReqTpDetails).find('.prereq-enrolled-txt').text();
								$('#show_expertus_message').html(expertus_error_message(errorMessage, 'error'));
								$('#show_expertus_message').show();
							}
							else {
								$(assignObject).parents("tr").parents("tr").after("<tr id='"+this.id+"' parelem='"+prgId+"' class='cus-accord "+prgId+"-sub-result'><td></td><td colspan='2'>"+this.cell.preReqTpDetails+"</td></tr>");
								$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
							}
						});
						obj.destroyLoader('paintCatalogContentResults'+userId);
					}
				});
			}else{
				$('#tbl-paintCatalogContentResults').find('.'+prgId+'-sub-result').remove();
				$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
				obj.destroyLoader('paintCatalogContentResults'+userId);
			}
		}catch(e){
			// to do
		}
	},
	
	getPrereqListForTeam : function(id,divId,catalogType,preLevel, userId, fromPage, assignObject) {
		try{
			obj = this;
			obj.createLoader('paintCatalogContentResults'+userId);
			var url = this.constructUrl("ajax/PrerequisiteFetchlist/"+userId+"/"+catalogType+"/"+id+"/"+preLevel+"/"+fromPage+"/"+divId);
			var assignId =  $(assignObject).parents("tr").parents("tr").attr("id");
			if($('#tbl-paintCatalogContentResults').find('.'+assignId+'-sub-result').length == 0) {
				$.ajax({
					type: "GET",
					url: url,
					success: function(result){
						var target = $(assignObject).parents("tr").parents("tr");
						var parentClass = target.attr('parelem');
						target.children('td').css('border-bottom', 'none');
						$.each(result.rows, function() {
							var currentRow = $("<tr id='"+this.id+"' parelem='"+parentClass+"' class='cus-accord "+assignId+"-sub-result "+parentClass+"-sub-result'><td></td><td style='padding-left: 15px;' colspan='2'>"+this.cell.preReqTpDetails+"</td></tr>");
							target.after(currentRow);
							target = currentRow;
							var crsId = this.cell.crsId;
							if(document.getElementById('cls-'+crsId)) {
								if($('input[name="cls-'+crsId+'"]:radio').is(":checked") == true)	{
									$('input[name="cls-'+crsId+'"]:radio').attr('disabled',true);
								}
							}
							$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
						});
						target.children('td').css('border-bottom', '1px solid #EDEDED');
						obj.destroyLoader('paintCatalogContentResults'+userId);
					}
				});
			}
			else {
				$('#tbl-paintCatalogContentResults').find('.'+assignId+'-sub-result').slideToggle(function() {
					$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
					obj.destroyLoader('paintCatalogContentResults'+userId);
				});
			}
			if($(assignObject).is('.title_close')) {
				$(assignObject).removeClass('title_close').addClass('title_open');
			}
			else {
				$(assignObject).removeClass('title_open').addClass('title_close');
			}
		}catch(e){
			// to do
		}
	},
	getPrereqCatalogRegisterForTeam : function(tpCrsIdv,courseId,str, userId) {
		try{
			$('#show_expertus_message').html('');
			var tpCrsId = tpCrsIdv.split('-');
			var tpCrsIdLen = tpCrsId.length;
			var inc = 0;
			var selectedClass,clsaction;
			var clsActionType;


			if(tpCrsIdLen == 1) {
				selectedClass 	= $('input[name="cls-'+tpCrsId[0]+'"]:radio:checked').val();
				if(selectedClass != undefined) {
					clsaction 		= $('#clsaction-'+selectedClass).val();
					var clsactionV 	= clsaction.split("-");
					clsActionType 	= clsactionV[0];
					if(clsActionType == "cart") {
						//show message priced offering can not be registerd
						var MsgErr = [Drupal.t('MSG716')];
						$('#show_expertus_message').html(expertus_error_message(MsgErr,'error'));
						$('#show_expertus_message').show();
					} else {
						this.prereqCatalogRegisterForTeam(tpCrsId[0],selectedClass, userId);
					}
				}
			} else {
				for(var n = 0; n < tpCrsId.length; n++) {			
					tpCrsIdV 		= tpCrsId[n];
					courseId		= tpCrsIdV;
					selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();
					if(selectedClass != undefined) {
						if($("#cls-"+courseId).attr("disabled") == false) {
							clsaction 		= $('#clsaction-'+courseId).val();
							var clsactionV 	= clsaction.split("-");
							clsActionType 	= clsactionV[0];
							if(clsActionType == "cart") {
								//show message priced offering can not be registerd
								var MsgErr = [Drupal.t('MSG716')];
								$('#show_expertus_message').html(expertus_error_message(MsgErr,'error'));
								$('#show_expertus_message').show();
							} else {
								this.prereqCatalogRegisterForTeam(courseId,selectedClass, userId);
							}
							inc=inc+1;
						}
					}
	
				}
			}
	
			if(((tpCrsIdLen == 1) && (selectedClass == undefined)) || ((tpCrsIdLen > 1) && (inc == 0))) {
				var MsgErr = new Array();
				MsgErr[0] = str;
				var message_call = expertus_error_message(MsgErr,'error');
				$('#show_expertus_message').html(message_call);
				$('#show_expertus_message').show();
			} 
		}catch(e){
			// to do
		}
	},
	getTpPrereqCatalogRegisterForTeam : function(tpCrsId,objectId,str, userId) {
		try{
			$('#show_expertus_message').html('');
			var tpCrsId = tpCrsId.split('-');

			var tpCrsIdV, selectedClass,inc = 0,v = 0;
			var checkedClass = new Array();
			var tpCrsIdLen	 = tpCrsId.length;
			var tpAction 	= $('#tpaction-'+objectId).val();
			var tpActionV 	= tpAction.split("-");
			tpActionType 	= tpActionV[0];
			tpNodeId		= tpActionV[1];
			var tpSelectedClass = '';

			var params = 'selectedItem={';
			for(var n = 0; n < tpCrsIdLen ; n++) {			
				tpCrsIdV 		= tpCrsId[n];
				inc=inc+1;
				selectedClass 	= $('input[name="cls-'+tpCrsIdV+'"]:radio:checked').val();

				if(selectedClass != undefined) {
					checkedClass[n]	= selectedClass;
					tpSelectedClass	= selectedClass+tpSelectedClass;
				}

				if(selectedClass == undefined || selectedClass == 'undefined') {				
					selectedClass='NULL';
					break;
				}
				params += '"'+v+'":'+'{';				
				params += '"tpid":"'+objectId+'",';
				params += '"courseid":"'+tpCrsIdV+'",';
				params += '"classid":"'+selectedClass+'"';
				params += '}';

				if(inc < tpCrsIdLen) {
					params += ',';							
					tpSelectedClass = ','+tpSelectedClass;
				}
				v = v+1;

			}

			params += '}';

			if(tpCrsIdLen != checkedClass.length) {
				var errMsgPre = new Array();
				errMsgPre[0] = str;
				var message_call = expertus_error_message(errMsgPre,'error');
				$('#show_expertus_message').html(message_call);
				$('#show_expertus_message').show();
			} else {
				if(tpActionType == "cart") {
					//show message priced offering can not be registerd
					var MsgErr = [Drupal.t('MSG716')];
					$('#show_expertus_message').html(expertus_error_message(MsgErr,'error'));
					$('#show_expertus_message').show();
				} else {
					var isAdminSide = 'N'; 
					var fromPage = 'MyTeam'; 
					var MasterMandatory = 'N'; 
					var url = this.constructUrl("ajax/trainingplan/class-list-register/"+isAdminSide+ "/" + userId+ "/" + fromPage+ "/" + MasterMandatory);		
					var obj = this;
					$.ajax({
						type: "POST",
						url: url,
						data:  params,
						datatype: 'text',
						success: function(result){
							result = $.trim(result);
							var resultErr = new Array();
							if(result == 'Registered') {
								resultErr[0] = Drupal.t("Registered Successfully");
								var message_call = expertus_error_message(resultErr,'error');
								$('#show_expertus_message').html(message_call);
								$('#show_expertus_message').show();

							} else {
								resultErr[0] = result; 
								var message_call = expertus_error_message(resultErr,'error');
								$('#show_expertus_message').html(message_call);
								$('#show_expertus_message').show();
							}

						}
					});
				}
			} 
		}catch(e){
			// to do
		}
	},
	prereqCatalogRegisterForTeam : function(courseId,classId, userId) {
		try{
			var obj = this;
			var isAdminSide = 'N';
			var waitlist = 1;
			//this.createLoader(loaderObj);
			obj.createLoader('paintCatalogContentResults'+userId);
			url = obj.constructUrl("ajax/learningcore/register/" + userId + '/' + courseId + '/' + classId+'/'+waitlist+'/'+isAdminSide);
			$.ajax({
				type: "POST",
				url: url,
				data:  {'isManager':'Y'},
				success: function(result) {
					result = $.trim(result);
					obj.destroyLoader('paintCatalogContentResults'+userId);
					var preMsg = new Array();
					if(result == 'Registered') {
						preMsg[0] = Drupal.t("Registered Successfully");
						$('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);

						if(document.getElementById('clsStatus-'+courseId)) {
							$("#clsStatus-"+courseId).html(SMARTPORTAL.t("Registered"));
						}

					}else if(result == 'Waitlisted'){ // Waitlist Message Ticket no :  0021486
						preMsg[0] = Drupal.t("Waitlisted Successfully");
						$('input[name="cls-'+courseId+'"]:radio').attr('disabled',true);
						if(document.getElementById('clsStatus-'+courseId)) {
							$("#clsStatus-"+courseId).html(SMARTPORTAL.t("Waitlisted"));
						}
					} 
					else{
						preMsg[0] = result;
					}
					var message_call = expertus_error_message(preMsg, 'error');
					$('#show_expertus_message').html(message_call).show();
				}
			});
		}catch(e){
			logMyErrors(e);
			// to do
		}
	},
	/*
	 * To get the available seats for the particular class
	 */
	 callAvailableSeats : function(classId,waitlist,userId) {
		 try{
		 var obj = this;
			url = obj.constructUrl("ajax/learning/myteam-search/available-seats/" + classId+'/'+waitlist+'/'+userId);
			
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){				   
				   $("#myteam-catalog-seats-available-"+classId).html(result);
				}
		    });
		 }catch(e){
				// to do
		 }
	},
	/*
	 * default actions to be performed in user list page at manager dash board
	 */
	paintAfterReady : function() {		
		try{
		if(document.getElementById('lnr-myteam-search')) {
			$('#prev_pager').click( function(e) {
				if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
				{					
					$('.sort-by-links').hide(); //hide sortby option
				}
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','60px');
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
				}
			});
			$('#next_pager').click( function(e) {
				if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
				{					
					$('.sort-by-links').hide(); //hide sortby option
				}
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','60px');
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
				}
			});
			$('.ui-pg-selbox').bind('change',function() {
				$('#paintContentResults').hide();
				$('#gview_paintContentResults').css('min-height','60px');
				$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
        $("#lnr-myteam-search").data("lnrmyteamsearch").hidePageControls(false);
			});			
			$(".ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#paintContentResults').hide();
					$('#gview_paintContentResults').css('min-height','60px');
				  $("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('searchRecordsPaint');
				}
			});
		}
		}catch(e){
			// to do
		}
	},
	/*
	 * action to be performed while clicking on pagination at assign learning pop-up
	 */
	paintCatalogAfterReady : function(userId) {		
		try{
		if(document.getElementById('my-team-dialog')) {
			$('#first_pager1').click( function(e) {
				
				if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
				{
					$('#myteam-cat-delivery-type-list').hide(); //hide any dropdown
					$('.sort-by-links').hide(); //hide sortby option
				}
					if(!$('#first_pager1').hasClass('ui-state-disabled')) {
						//$('#tbl-paintCatalogContentResults').hide();
						$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
						$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
					}
					$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
					
				});
			$('#prev_pager1').click( function(e) {
				
			if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			{
				$('#myteam-cat-delivery-type-list').hide(); //hide any dropdown
				$('.sort-by-links').hide(); //hide sortby option
			}
				if(!$('#prev_pager1').hasClass('ui-state-disabled')) {
					//$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
				}
				$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
			});
			$('#next_pager1').click( function(e) {
				if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
				{
					$('#myteam-cat-delivery-type-list').hide(); //hide any dropdown
					$('.sort-by-links').hide(); //hide sortby option
				}
				if(!$('#next_pager1').hasClass('ui-state-disabled')) {
					//$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
				}
				$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
			});
			$('#last_pager1').click( function(e) {
				
				if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
				{
					$('#myteam-cat-delivery-type-list').hide(); //hide any dropdown
					$('.sort-by-links').hide(); //hide sortby option
				}
					if(!$('#last_pager1').hasClass('ui-state-disabled')) {
						//$('#tbl-paintCatalogContentResults').hide();
						$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
						$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
					}
					$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
					
				});
			$('#pager1 .ui-pg-selbox').bind('change',function() {
				//$('#tbl-paintCatalogContentResults').hide();
				$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
				$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
		        $("#lnr-myteam-search").data("lnrmyteamsearch").hideCatalogPageControls(false);
		        $("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
			});	
			
			$("#pager1 .ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					//$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
				  $("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
				  $("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
				}
			});
			// expertusoneV2 theme pagination control
			if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			{
				$('.course-container-list #pager1 .page-show-next').click( function(e) {
					var pageCount = parseInt($(".course-container-list #pg_pager1 .page_count_view").attr('id'));
					var totCount = parseInt($(".course-container-list #pager1 .page_count_view").html());
					var page_total_view = parseInt($(".course-container-list #pg_pager1 .page-total-view").attr('id'));
					//console.log(pageCount);
					//console.log(page_total_view);
					var pageCount1 = pageCount +1;
					if(page_total_view == pageCount1){
						//console.log(111);
						$("#lnr-myteam-search").data("lnrmyteamsearch").afterBeforePageCount(userId);
					}
					if(pageCount >=page_total_view){
						$(".course-container-list #pg_pager1 .page_count_view").attr('id',(pageCount-1));
					}
					if(totCount==15 && $(".course-container-list #pager1 .page-show-next").hasClass("hide-pointer")){
						return false;
					}
					$("#lnr-myteam-search").data("lnrmyteamsearch").afterBeforePageCount(userId);
				});
			}
			if( Drupal.settings.ajaxPageState.theme == "expertusoneV2")
			{
				$('.course-container-list #pager1 .page-show-prev').click( function(e) {
					
					var pageCount = parseInt($(".course-container-list #pg_pager1 .page_count_view").attr('id'));
					//console.log(parseInt($(".course-container-list #pg_pager1 .page_count_view").attr('id')));
					if( pageCount <= 0){
						if(pageCount == 0){
							$("#lnr-myteam-search").data("lnrmyteamsearch").afterBeforePageCount(userId);
						}
						$(".course-container-list #pg_pager1 .page_count_view").attr('id','0');
					}
					if(parseInt($(".course-container-list #pg_pager1 .page_count_view").attr('id'))==0 && $(".course-container-list #pager1 .page-show-prev").hasClass("hide-pointer")){
						return false;
					}
					$("#lnr-myteam-search").data("lnrmyteamsearch").afterBeforePageCount(userId);
				});
			}
		}
		}catch(e){
			// to do
		}
	},
	afterBeforePageCount : function(userId){
		$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
		$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintCatalogContentResults'+userId);
        $("#lnr-myteam-search").data("lnrmyteamsearch").hideCatalogPageControls(false);
        $("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
	},
	/*
	 * actions to be performed while clicking on pagination under view learning page
	 */
	paintMyclassAfterReady : function(userId) {	
		try{
		if(document.getElementById('lnr-myteam-search')) {
			var html = "<div id='paintClassContentResults"+userId+"' class='paint-class-content-loader'></div>";
	        $("#paintClassContentResults"+userId).remove();
	        $("#gview_paintMyclassContentResults"+userId).prepend(html);
			
			$('#prev_pager2_user'+userId).click( function(e) {
				if(!$('#prev_pager2_user'+userId).hasClass('ui-state-disabled')) {
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
					$('#loaderdivpaintClassContentResults'+userId+' td').css('border-bottom','none');
				}
			});
			$('#next_pager2_user'+userId).click( function(e) {
				if(!$('#next_pager2_user'+userId).hasClass('ui-state-disabled')) {
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
					$('#loaderdivpaintClassContentResults'+userId+' td').css('border-bottom','none');
				}
			});

			$('#pager2_user'+userId+' .ui-pg-selbox').bind('change',function() {
				
				$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
				$('#loaderdivpaintClassContentResults'+userId+' td').css('border-bottom','none');
			});			
			$("#pager2_user"+userId+" .ui-pg-input").keyup(function(event){
				if(event.keyCode == 13){
					$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
					$('#loaderdivpaintClassContentResults'+userId+' table>tbody>tr>td').css('border-bottom','none');
				}
			});
			$('#gbox_paintMyclassContentResults'+userId+' .ui-jqgrid-sortable').click( function(e) {
				if($("#paintMyclassContentResults"+userId).jqGrid('getGridParam', 'records') > 1) {
					$("#paintClassContentResults"+userId).addClass('paint-class-content-loader');
					//$("#lnr-myteam-search").data("lnrmyteamsearch").createLoader('paintClassContentResults'+userId);
					$('#loaderdivpaintClassContentResults'+userId+' td').css('border-bottom','none');
				}
			});
		}
		}catch(e){
			// to do
		}
	},//End of paintMyclassAfterReady()
	/*
	 * Display list of delivery Types
	 */
	deliveryHideShow : function () {
		try{
			$('#myteam-cat-delivery-type-list').slideToggle();
			$('#myteam-cat-delivery-type-list li:last').css('border-bottom','0px none');
		}catch(e){
			// to do
		}
	},

	deliveryTypeText : function(dCode,dText,userId) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		$('#myteam-cat-delivery-type-list').hide();
		$('#myteam-cat-delivery-type').text(dText);
		$('#myteam-cat-delivery-type-hidden').val(dCode);
		$("#lnr-myteam-search").data("lnrmyteamsearch").catalogSearchAction("","",userId);
		$("#lnr-myteam-search").data("lnrmyteamsearch").paintCatkeywordAutocomplete(userId);
		$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
		}catch(e){
			// to do
		}
	},
	
	
/*	callForCoursesList : function(userId,divId,prgId){
		var isAdminSide = 'N';
		var url = this.constructUrl("ajax/trainingplan/course-list/"+prgId+ "/" + isAdminSide);		
		var obj =  this;
		var loaderObj = 'paintClassesSelectList'+prgId;
		this.createLoader(loaderObj);
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){										
					obj.paintMyTeamTPCourseList(userId,result,prgId,divId);
				    obj.destroyLoader(loaderObj);					 
			}
	    });
	},*/
	
	renderAccordion : function(userId,divId,rowObj){	
		try{
		var assignId =  $(rowObj).parents("tr").parents("tr").attr("id");
		if(!document.getElementById(assignId+"_cus-accord")) {
			var cusHTML = '<table id="'+divId+'" class = "lp-course-class-list-accord"></table>';
			$(rowObj).parents("tr").parents("tr").after("<tr id='"+assignId+"_cus-accord' class='cus-accord'><td colspan='3'>"+cusHTML+"</td></tr>");
			$(rowObj).parents("tr").parents("tr").children("td").css("border-bottom","none 0px");
			
			//this.callForCoursesList(userId,divId,assignId);			
			$("body").data("learningcore").getModuleListForTP('', assignId,'','N',userId,'MyTeam');
			$("#"+assignId+"_cus-accord").show();//css("display","block");
			$("#"+assignId+"_cus-accord").slideDown(1000);
		} else {
			var clickStyle = $("#"+assignId+"_cus-accord").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+assignId+"_cus-accord").show();//css("display","block");
    			$(rowObj).parents("tr").parents("tr").children("td").css("border-bottom","none 0px");
    			$("#"+assignId+"_cus-accord").slideDown(1000);
    		} else {
    			$("#"+assignId+"_cus-accord").hide();//css("display","none");
    			$("#"+assignId+"_cus-accord").slideUp(1000);
    			if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
    				$(rowObj).parents("tr").parents("tr").children("td").css("border-bottom","1px solid #ccc");
    			} else {
    				$(rowObj).parents("tr").parents("tr").children("td").css("border-bottom","1px dotted #ccc");
    			}
    			
    		}
    		
    	}
		/*-- #34270 scroll pane for the dynamic contents in team assign learn popup --*/
			$("#lnr-myteam-search").data("lnrmyteamsearch").refreshScrollBar();
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},
	showMoreTeamAction : function(obj,user_id) {
		try{
			var cur_obj_dis = 0;
			if($('#more-'+user_id).css('display')=='block'){
				cur_obj_dis = 1;
			}
			$('.team-more-action').hide();
			if(cur_obj_dis==0){
				$('#more-'+user_id).css('display','block')
			}else{
				$('#more-'+user_id).css('display','none')
			}
		}catch(e){
			// to do
		}
			},
	hideMoreTeamAction : function(obj) {
		try{
			this.prevMoreLPObj = '';
		}catch(e){
			// to do
		}
	},
	refreshScrollBar: function() {
		try {
			/*-- #34270 - scroll pane for the dynamic contents in team assign learn popup --*/
			// $("#my-team-dialog #gview_tbl-paintCatalogContentResults").jScrollPane();
			$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
		} catch(e) {
			// to do
		}
	}
	/*
	paintMyTeamTPCourseList : function(userId,result,prgId,divId){
		var res_length = result.length;
		var rhtml = "";
		var c;
		var courseId;
		var courseCode;
		var compStatus;
		var courseTitle;
		var isRequired;
		var registeredCnt;
		var courseList = '';
		var inc = 0;
		
		var obj = this;
		var objStr = '$("#lnr-myteam-search").data("lnrmyteamsearch")';
		if(res_length > 0 ) {
			rhtml += "<tr><td><div id='errMsgDisplay' class='messages error'></div></td></tr>";
			rhtml += "<tr><td><div class='course-level'>";
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse' cellspacing='0'>";
			for(c=0; c < result.length ; c++){
				inc = inc + 1;
				courseId 	= result[c]['course_details']['crs_id'];
				courseCode 	= result[c]['course_details']['crs_code'];
				courseTitle = result[c]['course_details']['crs_title'];
				courseDesc = result[c]['course_details']['crs_desc'];
				isRequired = result[c]['course_details']['is_required'];				
				registeredCnt = result[c]['course_details']['registered_cnt'];
				
				courseList += courseId;	
				
				if(inc < result.length) {
					courseList += ',';	
				 }
				
				
				
				var param="data={'PrgId':'"+prgId+"','CourseId':'"+courseId+"','UserId':'"+userId+"'}";
	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-crs-course-'+courseId+'">'; 
				rhtml += '<td>';
				
				rhtml += '<a class="set-height-column-left title_close " id="lp-crs-accodion-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForClassesView(this);\'>&nbsp;</a>';
				
				rhtml += '<div class="set-height-column-right"><span class="item-title" ><span class="lp-reg-class-title vtip" title="'+unescape(courseTitle)+'" href="javascript:void(0);">';
				rhtml += titleRestricted(courseTitle,30);
				rhtml += '</span></span>';
				rhtml += '<div class="course-info item-title-code">';
				rhtml += '<span class="vtip" title="'+unescape(courseCode)+'">'+titleRestricted(courseCode,10)+'</span>';
				rhtml += '<span class="pipeline">|</span>';
				rhtml += '<span>'+((isRequired =='Y')?''+SMARTPORTAL.t("Mandatory")+'':''+SMARTPORTAL.t("Optional")+'')+'</span>';
				rhtml += '<span class="pipeline">|</span>';
				rhtml += '<span>'+((registeredCnt < 1)?''+SMARTPORTAL.t("Not Registered")+'':''+SMARTPORTAL.t("Registered")+'')+'</span>';
				rhtml += '</div>';
				
				rhtml += '<div class="course-desc-info">';
				rhtml += '<span>'+courseDesc+'</span>';				
				rhtml += '</div></div>';
				rhtml += '</td>';
				rhtml += '</tr>';	
				
		  }
			
			    rhtml += '<tr><td><input type="hidden" id="courseListIds" name="courseListIds" value="'+courseList+'"></td></tr>';					    
			    var fnClick = objStr+'.registerTPObjectDetails("'+prgId+'","'+userId+'")';
				rhtml += "<tr><td><div class="admin-save-button-container"><div class="admin-save-button-left-bg"></div><input type='button' class ='assign-btn admin-save-button-middle-bg' id='assign_btn_"+prgId+"' value = 'Done' onclick='"+fnClick+"'/><div class="admin-save-button-right-bg"></div></div></td></tr>";
			    
		  rhtml += '</table>';
		  rhtml += '</div></td></tr>';  
		}
		else{
			rhtml += "<tr><td class='no-item-found'>"+SMARTPORTAL.t('There are no course(s) under this program.')+"</td></tr>";
		}
		

		$("#"+divId).html(rhtml);
	},
	
	*/
	
	/*
	accordionForClassesView : function(rowObj){
			var data = eval($(rowObj).attr("data"));
			var courseId = data.CourseId;
			
			if(!document.getElementById(courseId+"CourseSubGrid")) {
				$("#lp-crs-course-"+courseId).after("<tr id='"+courseId+"CourseSubGrid'><td colspan='4'><div id='course-details-"+courseId+"' ></div></td></tr>");
				
				$("#"+courseId+"CourseSubGrid").show();//css("display","block");
				$("#lp-crs-accodion-course-"+courseId).removeClass("title_close");
				$("#lp-crs-accodion-course-"+courseId).addClass("title_open");
				$("#lp-crs-course-"+courseId).removeClass("ui-widget-content");
				$("#"+courseId+"CourseSubGrid").slideDown(1000);
				$("#lp-crs-course-"+courseId).find(".set-height-column-right").css("border-bottom","0px");
				this.renderClassesGrid(rowObj);
			} else {
				var clickStyle = $("#"+courseId+"CourseSubGrid").css("display"); 
	    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
	    			$("#"+courseId+"CourseSubGrid").show();//css("display","block");
	    			$("#"+courseId+"CourseSubGrid").slideDown(1000);
	    			$("#lp-crs-accodion-course-"+courseId).removeClass("title_close");
					$("#lp-crs-accodion-course-"+courseId).addClass("title_open");
					$("#lp-crs-course-"+courseId).removeClass("ui-widget-content");
					$("#lp-crs-course-"+courseId).find(".set-height-column-right").css("border-bottom","0px");
	    		} else {
	    			$("#"+courseId+"CourseSubGrid").hide();//css("display","none");
	    			$("#"+courseId+"CourseSubGrid").slideUp(1000);
	    			$("#lp-crs-accodion-course-"+courseId).removeClass("title_open");
					$("#lp-crs-accodion-course-"+courseId).addClass("title_close");
					$("#lp-crs-course-"+courseId).removeClass("ui-widget-content");
					$("#lp-crs-course-"+courseId).addClass("ui-widget-content");
					$("#lp-crs-course-"+courseId).find(".set-height-column-right").css("border-bottom","1px solid #cccccc");
	    		}
			}	
	},
	
*/
	/*
	renderClassesGrid : function(rowObj) {
		var data = eval($(rowObj).attr("data"));
		var prgId = data.PrgId;		
		var courseId = data.CourseId;
			this.createLoader("course-details-"+courseId);
			var url = this.constructUrl("ajax/trainingplan/class-list/" + courseId);		
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
	
					obj.paintTrainingPrgClassDetails(result,data);
					$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
					obj.destroyLoader("course-details-"+courseId);
					
						for(c=0; c < result.length ; c++){								
							var enrolledId = result[c]['class_details']['enrolled_id'];								
							if(enrolledId.enrolled_id ) {							
								if($('input[name="'+courseId+'-clsregister"]:radio').is(":checked") == true)
								{
									$('input[name="'+courseId+'-clsregister"]:radio').attr('disabled',true);
								}					
							}
						}
					
				}
		    });
		
	},
	
	*/
	/*
	paintTrainingPrgClassDetails : function(result,data){

		var obj = this;
		var objStr = '$("#lnr-myteam-search").data("lnrmyteamsearch")';
		var rhtml ='';
		
		rhtml += "<div class='class-level'>";
		rhtml += "<table border='0' cellpadding='0' class='class-details-info' cellspacing='0'>";

		if(result.length > 0) {
			
			for(c=0; c < result.length ; c++){
				
				var courseId 	= result[c]['class_details']['crs_id'];
				
				var classId 	= result[c]['class_details']['cls_id'];
				var classCode 	= result[c]['class_details']['cls_code'];
				var classTitle 	= result[c]['class_details']['cls_title'];
				var classDesc 	= result[c]['class_details']['cls_short_description'];
				var deliveryTypeCode 	= result[c]['class_details']['delivery_type_code'];
				var deliveryTypeName 	= result[c]['class_details']['delivery_type_name'];				
				var language 	= result[c]['class_details']['language'];
				var sessionId   = result[c]['class_details']['session_id'];
				var locationName = result[c]['class_details']['location'];
				var loationAddr1 = result[c]['class_details']['loationaddr1'];
				var loationAddr2 = result[c]['class_details']['loationaddr2'];
				var locationCity = result[c]['class_details']['locationcity'];
				var locationState = result[c]['class_details']['locationstate'];
				var locationZip = result[c]['class_details']['locationzip'];
				
				var sessionStartDate = result[c]['class_details']['sess_start_date'];
				
				var enrolledId = result[c]['class_details']['enrolled_id'];
				
				
				if(enrolledId.enrolled_id || result.length == 1) {
					var checkedValue = "checked";
				}else{
					var checkedValue = "";
				}

				
				var startDate = '';
				if(result[c]['session_details'].length > 0) {
				
					var sDay = result[c]['session_details'][0]['start_date'];
					var sTime = result[c]['session_details'][0]['start_time'];
					var sTimeForm = result[c]['session_details'][0]['start_form'];
					 
						if(result[c]['session_details'].length > 1) {
							sessLenEnd = result[c]['session_details'].length-1;							
							var eDay = result[c]['session_details'][sessLenEnd]['start_date'];
							var eTime = result[c]['session_details'][sessLenEnd]['end_time'];
							var eTimeForm = result[c]['session_details'][sessLenEnd]['end_form'];
							
							startDate = sDay +' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eDay+ ' ' +eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
						}else {					

							var eTime = result[c]['session_details'][0]['end_time'];
							var eTimeForm = result[c]['session_details'][0]['end_form'];
							
							startDate = sDay +' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
						}
							
						
					
					var inc = 0;
					var passParams = "[";
					
					var sessLen = result[c]['session_details'].length;
					
					$(result[c]['session_details']).each(function(){
						inc=inc+1;
						 passParams += "{";
						 passParams += "'sessionTitle':'"+(($(this).attr("session_title")) ? $(this).attr("session_title") : '')+"',";
						 passParams += "'sessionDay':'"+$(this).attr("session_day")+"',";			
						 passParams += "'sessionSDate':'"+$(this).attr("start_date")+"',";
						 passParams += "'start_time':'"+$(this).attr("start_time")+"',";
						 passParams += "'end_time':'"+$(this).attr("end_time")+"',";
						 passParams += "'sessionSDayForm':'"+$(this).attr("start_form")+"',";
						 passParams += "'sessionEDayForm':'"+$(this).attr("end_form")+"'";
						 passParams += "}";
						 
						 if(inc < sessLen) {
							 passParams += ",";							
						 }	
								  
					});
					passParams += "]";
				
				}else{
					var passParams="''";
				}
	
			    var param = "data={'classId':'"+classId+"','classCode':'"+classCode+"','classDesc':'"+escape(classDesc)+"','language':'"+language+"'," +
			    		"'sessionId':'"+sessionId+"','deliveryTypeCode':'"+deliveryTypeCode+"','locationName':'"+locationName+"','loationAddr1':'"+loationAddr1+"','loationAddr2':'"+loationAddr2+"'," +
	    				"'locationCity':'"+locationCity+"','locationState':'"+locationState+"','locationZip':'"+locationZip+"'," +
   						"'sessionDetails':"+passParams+"}";

			    
	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-class-'+classId+'">'; 
				rhtml += '<td class="course-sub-class-detail"><table border="0" width="100%" cellpadding="0" cellspacing="0"><tr><td valign="top" class="course-detail-col1">';
				
				rhtml += '<a id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="title_close " onclick=\''+objStr+'.ViewClasses('+classId+');\'>&nbsp;</a>';
				rhtml += '<span><span class="lp-reg-class-title vtip" title="'+unescape(classTitle)+'" href="javascript:void(0);">';
				rhtml += titleRestricted(classTitle,30);
				rhtml += '</span></span></td>';
				
				rhtml += '<td valign="top" class="course-detail-col2"><span>'+startDate+'</span></td>';

				//rhtml += '<input class="rButtonclass" type="radio" id="'+courseId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'">';
				
				rhtml += '<td valign="top" class="course-detail-col3"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></td>';
				
				rhtml += '</tr></table>';

				
				rhtml += '<div class="course-delivery-info item-title-code">';
				rhtml += '<span class="vtip" title="'+unescape(classCode)+'">'+titleRestricted(classCode,10)+'</span>';
				rhtml += '<span class="pipeline">|</span>';
				rhtml += '<span>'+deliveryTypeName+'</span>';
				
				if(deliveryTypeCode=='lrn_cls_dty_ilt') {
					rhtml += '<span class="pipeline">|</span>';
					rhtml += '<span>'+locationName+'</span>';
				}
				rhtml += '</div>';
				rhtml += '</td>';
				rhtml += '</tr>';
		  }
			
		}else{
			
			rhtml += '<tr>';
			rhtml += '<td class="no-item-found">'+SMARTPORTAL.t("There are no classes under this course")+'</td>';
			rhtml += '</tr>';
		}
	  rhtml += '</table>';
	  rhtml += '</div>';
	  
	  $("#course-details-"+data.CourseId).html(rhtml);
	  $('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
	},
	
	*/
	/*
	ViewClasses : function(classId) {		
		var currTr = classId;
		if($("#lp-class-accodion-"+currTr).attr("class").indexOf('title_close') == 0 ){
				$("#lp-class-accodion-"+currTr).removeClass("title_close");
			$("#lp-class-accodion-"+currTr).addClass("title_open");
			  			
		}else{
			$("#lp-class-accodion-"+currTr).removeClass("title_open");
			$("#lp-class-accodion-"+currTr).addClass("title_close");  
		    			
		}
    		
	},
		*/
	
	/*

	registerTPObjectDetails : function(prgId,userId){		
		var courseList = $('#courseListIds').val();
		var courseListIds = courseList.split(",");
		
		var courseId;
		var selectedClass;
		var inc = 0;
		var classIds = '';
		var params = 'selectedItem={';
		
		for(c=0; c < courseListIds.length ; c++){
			
			inc=inc+1;
			courseId 	= courseListIds[c];	
		
			selectedClass = $('input[name="'+courseId+'-clsregister"]:radio:checked').val();
		
			if(selectedClass == undefined || selectedClass == 'undefined') {				
				selectedClass='NULL';
				break;
			}
			params += '"'+c+'":'+'{';	
			params += '"uid":"'+userId+'",';
			params += '"tpid":"'+prgId+'",';
			params += '"courseid":"'+courseId+'",';
			params += '"classid":"'+selectedClass+'"';
			params += '}';
			classIds +=  selectedClass;
			if(inc < courseListIds.length) {
				params += ',';
				classIds += ',';	
			 }
			
		}		
			
			params += '}';
			if(selectedClass == "NULL") {
				 $("#errMsgDisplay").html("Please select one class in each course");
				 $(".error").css('display','block');
			}else{
				var obj = this;
				var MasterMandatory=$('#assignPrg_checkbox_'+prgId+':checked').val();
				if(MasterMandatory==undefined){
					MasterMandatory='N';
				}
				url = obj.constructUrl("ajax/learning/myteam-search/assign-object-class/" + userId + '/' + prgId + '/' + MasterMandatory +'/'+'forcereg');
				$.ajax({
					type: "POST",
					url: url,
					data:  params,
					success: function(result){
					result = $.trim(result);	
						   obj.callAvailableSeats(prgId);
					   if(result == 'Registered') {
						   $("#paintClassesSelectList"+prgId).remove();
						   $("#assignClass_"+prgId).removeAttr('onclick');
						   $("#assignClass_"+prgId).removeClass("action-btn");
						   $("#assignClass_"+prgId).addClass("action-btn-disable");
						   $("#assignClass_"+prgId).html("Registered");							   
						   var currentPage = $('.ui-pg-input').val();
						   $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:currentPage}]);
					   }else{
						   $("#errMsgDisplay").html(result);
						   $(".error").css('display','block');					   
					   }
					   //$('#'+loaderObj).hide();
					}
			    });
								
			}			
	}
	*/

});

$('body').click(function (event) {
	if(event.target.className!='team-launch-more') {
		$('.team-more-action').hide();
	}
});

$.extend($.ui.lnrmyteamsearch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});

})(jQuery);

$(function() {
	try{
	$( "#lnr-myteam-search" ).lnrmyteamsearch();
/*	$('#myteam-cat-delivery-type').event(function(){
		alert("Event");
	});
*/
	}catch(e){
	// to do
	}
});

// Hide the drop down option values
$('body').bind('click', function(event) {
	try{
	if(event.target.id != 'admin-dropdown-arrow'){
		$('#myteam-cat-delivery-type-list').hide();
	}
	}catch(e){
		// to do
	}
});

//For set min date to disable past session date in ILT/VC Class. ticket: 0022513
function formattedDateComp(date) {
	try{
	if(navigator.userAgent.toLowerCase().indexOf('msie')>=0){
		if(date!=null && date!=undefined && date!=''){
			date = date.replace(' ','-').replace(' ','-');
		}else{
			date = new Date().toString();
		}
	}else{
		if(date==null || date==undefined || date==''){
			date = new Date().toString();
		}
	}
  var d = new Date(date),
      month = '' + (d.getMonth() + 1),
      day = '' + d.getDate(),
      year = d.getFullYear();
  if (month.length < 2) month = '0' + month;
  if (day.length < 2) day = '0' + day;
  return [month,day,year].join('-');
	}catch(e){
		// to do
	}
}

$(function() {
	try{
	$("#myteam_searchtext").keyup(function(event){
		try{
		  if(event.keyCode == 13){			  
			  $("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();
		   }
		}catch(e){
			// to do
		}
		});

	$('#myteam-search-txt').click(function() {
		try{
		$("#lnr-myteam-search").data("lnrmyteamsearch").searchAction();
		}catch(e){
			// to do
		}
	});
	
	if($('#myteam_searchtext')) {
		if ( $( ".page-myapproval-myteam-certificate" ).length > 0) {
		
		$('#myteam_searchtext').autocomplete(
					"/?q=learning/myteam-myapproval/certificate-autocomplete",{
					minChars :3,
					max :50, 
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
			});
		}
		else {
		
		$('#myteam_searchtext').autocomplete(
					"/?q=learning/myteam-search/username-autocomplete",{
					minChars :3,
					max :50, 
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
			});
		}
			
	}
	
	//markcomplete calender open default :ticket 0029563 item1
	/*$('.mark-complete-calender #comp_date').live("click",function(){
		$('#ui-datepicker-div').css('display','block');
	});
	$('.myteam-learning-course-btn .myteam-more-drop-down-list li').live("click",function(){
		
		//alert('call');
		var disCalend=$('#my-team-markcomplete-dialog').css('display');
		//var disCalendinner=$('.page-learning-myteam-search #ui-datepicker-div').css('display');
		setTimeout(function(){
		if($.browser.msie && $.browser.version > 7 && disCalend=='block'){
			//alert('enable');
			$('.page-learning-myteam-search').find('#ui-datepicker-div, .ui-datepicker').css('display','none');
			$('#my-team-markcomplete-dialog').next().next('#ui-datepicker-div, .ui-datepicker').css('display','none');
		}
		
		},500);
	});	
		*/
	}catch(e){
		// to do
	}
});
