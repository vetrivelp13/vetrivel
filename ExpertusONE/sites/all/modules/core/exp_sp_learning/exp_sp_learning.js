(function($) {

$.widget("ui.learningcore", {
	qtipLoadSet : new Array(),
	qtipLenth : 0,
	_init: function() {
		try{
			var self = this;
			/*
		if($('#block-system-main-menu a[href="/?q=learning/catalog-search"]').hasClass('active-trail') == true){
			$('#block-system-main-menu').parents('div').siblings('.theme-navigation-menu-bg-left').addClass('active-header-curve');
		}else{
			$('#block-system-main-menu').parents('div').siblings('.theme-navigation-menu-bg-left').removeClass('active-header-curve');
		}
			 */
			//change by ayyappans for 39837: Theme is not getting applied for 'Catalog' header menu in Catalog -> Class details page
			var pageUrl = window.location.search;
			var catalogMenu = $('#block-system-main-menu a[href="/?q=learning/catalog-search"]');
			var forumMenu = $('#block-system-main-menu a[href="/?q=learning/forum-list"]');
			var firstMenuSel=$('.theme-navigation-menu #block-system-main-menu').find('.menu li:first-child');
			var lastMenuSel=$('.theme-navigation-menu #block-system-main-menu').find('.menu li:last-child');
			if(firstMenuSel.is('.active-trail')){
			$('.theme-navigation-menu-bg-left').addClass('active-header-curve');
			}else {
			$('.theme-navigation-menu-bg-left').removeClass('active-header-curve');
			}
			if(lastMenuSel.is('.active-trail')){
				$('.theme-navigation-menu-bg-right').addClass('active-header-curve');
				lastMenuSel.css("border-right","0");
			}else {
				$('.theme-navigation-menu-bg-right').removeClass('active-header-curve');
			}
			if((pageUrl.indexOf("learning/course-details") > -1 || pageUrl.indexOf("learning/class-details") > -1 || pageUrl.indexOf("learning/learning-plan-details") > -1) && !catalogMenu.hasClass('active-trail')) {
				catalogMenu.addClass('active active-trail').parent().addClass('active active-trail');
				if(catalogMenu.parent().is(':first-child')) {
					$('.theme-navigation-menu-bg-left').addClass('active-header-curve');
				}
				else {
					$('.theme-navigation-menu-bg-left').removeClass('active-header-curve');
				}
			}
			else if(pageUrl.indexOf("learning/forum-topic-list") > -1 && !forumMenu.hasClass('active-trail')) {
				forumMenu.addClass('active active-trail').parent().addClass('active active-trail');
			}
		}catch(e){
			// to do
		}
	},


	 callAvailableSeats : function(userId, courseId, classId, waitlist) {
		 try{
		 var obj = this;
			url = obj.constructUrl("ajax/learningcore/availableseats/" + classId+'/'+waitlist+'/'+userId);
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
				   $("#seats_available_"+classId).html(result);
				}
		    });
		 }catch(e){
				// to do
			}
	},

	callObjectAvailableSeats : function(objectId) {
		try{
		var obj = this;
		url = obj.constructUrl("ajax/learningcore/objectavailableseats/" + objectId);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '',//Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
			   $("#object-seats_available_"+objectId).html(result);
			}
	    });
		}catch(e){
			// to do
		}
	},

	callSigninWidget : function(clickedid){
		try{
			//after login auto register or add to cart related work start
			//console.log(clickedid);
			var current_url_arr = window.getUrlVars();
			var url_query_string = current_url_arr['q'];
			var pagenumber = 	$('.ui-pg-input').val();
			var rownumber = 	$("#paintContentResults").jqGrid('getGridParam', 'rowNum');
			//console.log(rownumber);
			//document.cookie="user_selected_class_id="+clickedid+"; expires="+expireTime;
			$.cookie("user_selected_class_id", clickedid,{ expires: 300 });
			$.cookie("user_selected_url",url_query_string ,{ expires: 300 });
			//console.log(pagenumber);
			if(pagenumber!=null && pagenumber != undefined)
				$.cookie("user_selected_page_number", pagenumber,{ expires: 300 });
			if(rownumber!=null && rownumber != undefined)
				$.cookie("user_selected_row_number", rownumber,{ expires: 300 });
			//after login auto register or add to cart related work start
		 //$(".ctools-modal-ctools-login-style").click();
			$("#signin").click();
		}catch(e){
			// console.log(e);
		}
	},

	callRegisterClass : function(widgetId,userId, courseId, classId, waitList, idValue) {
		try{
		var currtheme = Drupal.settings.ajaxPageState.theme;
		if(typeof(idValue)==='undefined') idValue = '';
		 var obj = this;
		 var loaderObj = widgetId;
		 var userId = this.getLearnerId();

			if(!userId) {
				this.callMessageWindow('registertitle',"Please sign in to proceed.");
			}else{
					this.createLoader(loaderObj);
					var isAdminSide = 'Nlnrsearch';
					url = obj.constructUrl("ajax/learningcore/register/" + userId + '/' + courseId + '/' + classId+'/'+waitList+'/'+isAdminSide);
					$.ajax({
						type: "POST",
						url: url,
						// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
						success: function(result){
							result_arr = result.split('~~');
							result = $.trim(result_arr[0]);
							if(result_arr[1]!='' && result_arr[1]!=undefined && (result_arr[3] == 'vod' || result_arr[3]=='wbt') && (document.getElementById('lnr-prerequisite-container')==undefined || document.getElementById('lnr-prerequisite-container')=='')){
								if(Drupal.settings.convertion.mylearn_version==1)
									var launchInfo_arr = result_arr[1];
								else
								 var launchInfo_arr = $.parseJSON(result_arr[1]);
								 var del_type = result_arr[3];
								 var enrolled_id = result_arr[2];
								 var action_status = result;
								 var pre_ass_status = result_arr[4];
								 //console.log(pre_ass_status);
								 var query_str = window.location.href.slice(window.location.href.indexOf('?') + 1);
								 var recurring = $('#reccuring_'+classId).attr('value');
								 if ((query_str.indexOf("learning/course-details") >= 0 || query_str.indexOf("learning/class-details") >= 0) && (result == 'Registered' || result == Drupal.t('Registered')) && recurring !=1){
									   location.reload();
									   return;
								 }
								  else if((result == 'Registered' || result == Drupal.t('Registered')) && Drupal.settings.convertion.mylearn_version == 1){
									   var enrolled_id = result_arr[2];
									   $('.progress_'+classId).attr('id','progress_'+enrolled_id);
									   //76575: Class Details - UI issue when registering to recurring Classes
									   $('.progress_'+classId).html('');
									   progressBarRound(enrolled_id,0, 'enr_progress','progress_',$("#lnr-catalog-search").data("contentPlayer"));
									   //$("#lnr-catalog-search").data("contentPlayer").progressbardetail(enrolled_id,0, 'enr_progress','progress_');
									   $('#progress_'+enrolled_id).show();
								 }
								 registeredToLaunch(launchInfo_arr,userId,classId,courseId,del_type,enrolled_id,action_status,pre_ass_status);
							}else{

						   if(userId!=0)

						   //alert('result : '+result);
						   if((result == 'Registered') || (result == Drupal.t('Registered')) || (result == 'Waitlisted') || (result == Drupal.t('LBL190'))) {
							 //  if(idValue != 'fromClasslvl') {
								   obj.callAvailableSeats(userId, courseId, classId, waitList);
							  // }

 							   var showAction = (result) ? result  : '';
							   if(result == Drupal.t('Registered')) {
								 $("#registerCls_"+classId).removeClass("action-btn");
								 $("#registerCls_"+classId).html(Drupal.t('Registered'));
							   } else if(result == Drupal.t('LBL190')) {
								   $("#registerCls_"+classId).removeClass("action-btn-waitlist");
							   }

							   $("#registerCls_"+classId).removeAttr('onclick');
							   //$("#registerCls_"+classId).removeClass("action-btn");
							   $("#registerCls_"+classId).addClass("action-btn-disable");
							   if(idValue == 'fromClasslvl') {
								   if(currtheme == 'expertusoneV2'){
									   $("#select-class-dialog #registerCls_"+classId).html('<div>'+showAction+'</div>');
									   $("#myteam-catalog-seats-available-"+classId).html('');
									   $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
								   	}
									if (widgetId=='lnr-catalog-search') {
										var currentPage = $('.ui-pg-input').val();
										if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
											$("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
										}
									   }
							   }else{
								   $("#registerCls_"+classId).html(showAction);
							   }
							   //$("#registerCls_"+classId).html(showAction);

							   if(document.getElementById("preReqClsStatus"+courseId)) {
								   $("#preReqClsStatus"+courseId).html(Drupal.t('Enrolled'));
								   $("#registerClsPreReq_"+classId).removeAttr('onclick');
								   $("#registerClsPreReq_"+classId).removeClass("action-btn");
								   $("#registerClsPreReq_"+classId).addClass("action-btn-disable");
								   $("#registerClsPreReq_"+classId).html(showAction);
							   }
							   var query_str = window.location.href.slice(window.location.href.indexOf('?') + 1);
							   var recurring = $('#reccuring_'+classId).attr('value');
							   if ((query_str.indexOf("learning/course-details") >= 0 || query_str.indexOf("learning/class-details") >= 0) && (result == 'Registered' || result == Drupal.t('Registered')) && recurring !=1){
								   location.reload();
								   return;
							   }
							   else if((result == 'Registered' || result == Drupal.t('Registered')) && Drupal.settings.convertion.mylearn_version == 1){
								   var enrolled_id = result_arr[2];
								   $('.progress_'+classId).attr('id','progress_'+enrolled_id);
								   //76575: Class Details - UI issue when registering to recurring Classes
								   $('.progress_'+classId).html('');
								   	progressBarRound(enrolled_id,0, 'enr_progress','progress_',$("#lnr-catalog-search").data("contentPlayer"));
									//$("#lnr-catalog-search").data("contentPlayer").progressbardetail(enrolled_id,0, 'enr_progress','progress_');
								   $('#content-available-seats_'+classId).hide();
								   $('#progress_'+enrolled_id).show();
							   }
						   } else {
							    var commClassId;
							       //obj.callMessageWindow('registertitle',result);
							   if(resource['catalog_reg'] == 'Class' || (resource['catalog_reg'] == 'Course' && idValue != 'fromClasslvl') ){
								   obj.callMessageWindow('registertitle',result);
							   }else{
								   var errResult = new Array();
								   errResult[0] = result;
								   var err_msg = expertus_error_message(errResult,'error');
									$('#show_expertus_message').html(err_msg);
									$('#show_expertus_message').show();
							   	}
							   		if(result.indexOf(Drupal.t('MSG311')) >= 0){
							   			if(idValue == 'fromCourselvl') {
							   				commClassId = courseId;
							   			}else{
							   				commClassId = classId;
							   			//$("#registerCls_"+classId).html(Drupal.t('LBL032'));
								   			var tmp = $("#registerCls_"+commClassId).html();
								   			var tmp1 = $.trim(tmp).split('<div>');
								   			var str1='';
								   			if(tmp1.length>1){
								   				/*-- Issue fix for #38288 --*/
								   				if($.browser.msie && parseInt($.browser.version, 10)=='9') {
								   					var divLen = tmp1.length-2;
								   				} else {
								   					var divLen = tmp1.length-1;
								   				}
								   				for(var i=0;i<divLen;i++){
								   					if(tmp1[i].trim()!='')
								   						str1 += "<div>"+tmp1[i];
								   				}
								   				str1 += "<div>"+Drupal.t('Registered')+"</div>";
								   			}else{
								   				str1 = Drupal.t('Registered');
								   			}
							   			}
							   			$("#registerCls_"+commClassId).html(str1);
									    var regStatus = 'lrn_crs_reg_cnf';
									    var stringAttr = $("#registerCls_"+commClassId).attr("onClick");
									    var contains = stringAttr.indexOf('lrn_crs_reg_cnf');
									    if(contains == -1){
									    	if(idValue == 'fromCourselvl') {
									    		var setNewAttr = '$("body").data("learningcore").callEquivalencePopup(\''+widgetId+'\',\''+userId+'\',\''+courseId+'\',\''+classId+'\',\''+waitList+'\',\''+regStatus+'\',\''+idValue+'\')';
									    	}else {
									    		var setNewAttr = '$("body").data("learningcore").callEquivalencePopup(\''+widgetId+'\',\''+userId+'\',\''+courseId+'\',\''+classId+'\',\''+waitList+'\',\''+regStatus+'\')';
									    	}
										    $("#registerCls_"+commClassId).attr("onClick", setNewAttr);
									    }
									    if((idValue == 'fromCourselvl' || idValue == 'fromClasslvl') && widgetId=='lnr-catalog-search'){
									    	var currentPage = $('.ui-pg-input').val();
											if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
												$("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
											}
									    }
							   	}
						   }
						}
						   obj.destroyLoader(loaderObj);

						}
				    });
			}
		}catch(e){
			// to do
			//console.log(e)
		}
	},

	registerConfirmation : function(title, message ,widgetId, userId, courseId, classId, waitList,selectValues,compClassPopup,courseLevelCnt){
		try{
		if(typeof(selectValues)==='undefined') selectValues = '';
		if(typeof(compClassPopup)==='undefined') compClassPopup = '';
		if(title == 'registertitle'){
			title = Drupal.t('LBL721');
		}

	    $('#commonMsg-wizard').remove();
	    var html = '';
	    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(message)+'</span></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
    	if(selectValues=='fromCourselvl' && (compClassPopup == 'Y' || courseLevelCnt == 'Y')) {
    		closeButt[Drupal.t('Yes')]=function(){
    		$(this).dialog('destroy');
    		$(this).dialog('close');
    		$('#equvMsg-wizard').remove();
    		$('#select-class-equalence-dialog').remove();
    		$("#lnr-catalog-search").data("lnrcatalogsearch").showSelectClass(userId, courseId); };
    	} else {
    	closeButt[Drupal.t('Yes')]=function(){ $("body").data("learningcore").callRegisterClass(widgetId,userId, courseId, classId, waitList,selectValues); $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove(); };
    	}
    	$("#commonMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title: SMARTPORTAL.t(title),
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

		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	 }
	 	if(this.currTheme == "expertusoneV2"){
	 		//changeDialogPopUI();
	 		 $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-equalence-dialog');
			 //$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
	    $("#commonMsg-wizard").show();
		$('.ui-dialog-titlebar-close, .removebutton').click(function(){
			$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#commonMsg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},

/*
 * 0024925: Enforce equivalence in training plans
 * for this ticket we created equivalence option for training plan
 * don't delete it. Its may need for feature enhancement
 *

 */
 /*

	registerConfirmationTP : function(title, message ,widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId,lengthTP){
		try{
		if(typeof(addToCart)==='undefined') addToCart = '';
		if(typeof(isAdminSide)==='undefined') isAdminSide = 'N';
		if(typeof(userListIfAdminSide)==='undefined') userListIfAdminSide = '';
		if(typeof(recertifyId)==='undefined') recertifyId = '';
		//alert('title:'+title+'|'+'message:'+message+'|'+widgetId+'|'+prgId+'|'+addToCart+'|'+isAdminSide+'|'+userListIfAdminSide+'|'+recertifyId);
		if(title == 'registertitle'){
			title = Drupal.t('LBL721');
		}

	    $('#commonMsg-wizard').remove();
	    var html = '';
	    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(message)+'</span></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
    	if(lengthTP > 0 ) {
    		closeButt[Drupal.t('Yes')]=function(){ $("body").data("learningcore").callPopupOrNotTP(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId); $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove(); };
    	} else {
    		closeButt[Drupal.t('Yes')]=function(){ $("body").data("learningcore").callPopupOrNotTP(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId); $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove(); };
    	}
    	$("#commonMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title: SMARTPORTAL.t(title),
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

		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	 }
	 	if(this.currTheme == "expertusoneV2"){
	 		//changeDialogPopUI();
	 		 $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-equalence-dialog');
			   $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
	    $("#commonMsg-wizard").show();
		$('.ui-dialog-titlebar-close, .removebutton').click(function(){
			$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#commonMsg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},

*/
    enrollChangeClassErrorMessage : function(title, message, restoreOverlay){
	 try{
		if(title == 'registertitle'){
			title = Drupal.t('LBL721');
		}

	    $('#commonMsg-wizard').remove();
	    var html = '';
	    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
    	html+= '<td align="center"><span class="select-greyed-out-text">'+SMARTPORTAL.t(message)+'</span></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
    	//closeButt[Drupal.t('LBL342')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove(); };
	    $("#commonMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title: SMARTPORTAL.t(title),
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
	    //$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
		 }
		if(this.currTheme == "expertusoneV2"){
		 	   $('#select-class-enrollChange-dialog').prevAll().removeAttr('select-class-enrollChange-dialog');
		 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-enrollChange-dialog'></div>");
		       $('#select-class-enrollChange-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			   changeChildDialogPopUI('select-class-enrollChange-dialog');
			   if(typeof restoreOverlay == "undefined" || restoreOverlay == 0) {
			   $('#select-class-enrollChange-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-enrollChange-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-enrollChange-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-enrollChange-dialog').prevAll().removeAttr('select-class-enrollChange-dialog');
			 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-enrollChange-dialog'></div>");
				changeChildDialogPopUI('select-class-enrollChange-dialog');
				$('#select-class-enrollChange-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
	    $("#commonMsg-wizard").show();
		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
			$('#my-team-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#commonMsg-wizard").remove();
	    });
	 }catch(e){
			// to do
		}
	},


	callAddToCart : function(widgetId,act,ObjectId,addFrom,class_ids,waitlist, catalogType) {
		try{
		var loaderObj = widgetId ;
		if(typeof(catalogType)==='undefined') catalogType = '';
		var currtheme = Drupal.settings.ajaxPageState.theme;

		 if(addFrom == 'Cart'){
			var classData 	= eval($('#object-addToCartList_'+ObjectId).attr("data"));
		 }else{
			 var classData 	= eval($('#addToCartList_'+ObjectId).attr("data"));
		 }
			var LMSNodeId 	= classData.NodeId;
			var classId	 	= addFrom == 'Cart' ? classData.TpId : classData.ClassId;
			var courseId	= classData.CourseId;

			var obj = this;
			this.createLoader(loaderObj);
			if(addFrom == 'Cart'){
				//var class_ids = '';
				if(class_ids ==''){
					class_ids = 0;
				}
				url =  this.constructUrl("ajax/cart/tpproduct/add/" + LMSNodeId + "/" + classId + "/" + 'null' + "/" + class_ids);
			}
			else{
				url =  this.constructUrl("ajax/cart/product/add/" + LMSNodeId + "/" + classId + "/" + courseId+'/'+waitlist);
			}

			$.ajax({
				type: "POST",
				url: url,
				// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader(loaderObj);
					var cart_msg = result.cart_msg;
					if((result.cart_msg == Drupal.t('MSG247')) || (result.cart_msg == 'waitlisted')) {
						if(addFrom == 'Cart'){
							$("#tainingplan-register"+ObjectId).remove();
						}
						var showBtnTxt = cart_msg == 'CartAdded' ? 'Added to Cart' : 'Waitlisted';
						var showBtnTxt = Drupal.t('LBL049');
						var objDivId = (addFrom == 'Cart')? 'object-addToCartList_' : 'addToCartList_';

						if(cart_msg == Drupal.t('MSG247')) {
						  $("#"+objDivId+classId).removeClass("action-btn-disable");
						} else if(cart_msg == 'waitlisted') {
						  $("#"+objDivId+classId).removeClass("action-btn-waitlist");
						}
						$("#"+objDivId+classId).removeAttr('onclick');
						$("#"+objDivId+classId).removeClass("action-btn");
						$("#"+objDivId+classId).addClass("action-btn-disable");
						$("#addToCartList_"+classId).html(Drupal.t('LBL049'));
						//$("#"+objDivId+classId).html("Added to Cart");

						if(catalogType == 'fromClasslvl') {
							   if(currtheme == 'expertusoneV2'){
								   $("#select-class-dialog #"+objDivId+classId).html(showBtnTxt);
								   $("#myteam-catalog-seats-available-"+classId).html('');
							   if (widgetId=='lnr-catalog-search') {
									   var currentPage = $('.ui-pg-input').val();
									   if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
											$("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
										}
								   }
							   }
						   }else{
							   $("#"+objDivId+classId).html(showBtnTxt);
						   }

					}
					else{
						if(addFrom == 'Cart'){
							var errorMessages = new array();
							errorMessages[0]=result.cart_msg;
							var message_call = expertus_error_message(errorMessages,'error');
							$('#show_expertus_message').html(message_call);
							$('#show_expertus_message').show();
						}
						else{
							obj.callMessageWindow('registertitle',result.cart_msg);
						}
					}
					if (parseInt(result.total_product) > 0) {
						$("#ShoppingCartItemsId").addClass("filled");
						$("#ShoppingCartItemsId").html(result.total_product);
					} else {
						$("#ShoppingCartItemsId").removeClass("filled");
						$("#ShoppingCartItemsId").html('');
					}
				}
		    });
		}catch(e){
			// to do
			// console.log(e);
		}
	},

/*	callObjectAddToCart : function(widgetId,act,objectId) {
		alert("Under Construction");
	},*/

	//Common function for calling dialog window. Param : title, message
	callMessageWindow : function(title,message, restrictTitle){
	 try{
		if(title == 'registertitle'){
			title = Drupal.t('LBL721');
		}

	    $('#commonMsg-wizard').remove();
	    var html = '';
	    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
    	html+= '<td align="center"><span class="select-greyed-out-text">'+unescape(SMARTPORTAL.t(message))+'</span></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
    	closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#commonMsg-wizard').remove();};
	    $("#commonMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:520,
	        resizable:false,
	        modal: true,
	        title: SMARTPORTAL.t(title),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    if(restrictTitle !== undefined) {
	    	$('#ui-dialog-title-commonMsg-wizard').attr('title', unescape(title)).text(titleRestricted(unescape(title), restrictTitle));
	    }
	   	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
			}
	    /*new dialog popUI Script*/
		 // $('#select-class-dialog').hide();
	 	if(this.currTheme == "expertusoneV2"){
	     //changeDialogPopUI();
	 	   $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
	 	   $('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
	       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		   changeChildDialogPopUI('select-class-equalence-dialog');
		   $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
		   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
		   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
		   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
		   }
		}
	 	else {
	 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
			changeChildDialogPopUI('select-class-equalence-dialog');
			$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
		   }
	    $("#commonMsg-wizard").show();
		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
			if(opener!=null && opener!=undefined && opener.location.href.indexOf("admincalendar") >= 0){
				window.close();
			}
			$("#commonMsg-wizard").remove();
	        //$('#select-class-dialog').show();
	        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	    });
	 }catch(e){
			// to do
		}
	},


	registerTPClassDetails : function(widgetId,prgId,addToCart,fromPage,recertifyId){
	 try{
		if(typeof(recertifyId) == 'undefined') recertifyId = '';
		var courseList = $('#courseListIds').val();
		var courseListIds = courseList.split(",");

		var isAdminSide = $('#isadminsidefield').val();
		var userListIds = $('#userListIds').val();

		var courseId;
		var selectedClass = '';
		var inc = 0;
		var classIds = '';
		var selectedClass1;
		var registeredNsingleCourseClassIds;

		var params = 'selectedItem={';

			for(c=0; c < courseListIds.length ; c++){

				inc=inc+1;
				courseId 	= courseListIds[c];

				registeredNsingleCourseClassIds = $("#classListIds-"+courseId).val();

				selectedClass = $('input[name="'+courseId+'-clsregister"]:radio:checked').val();

					if(registeredNsingleCourseClassIds == '' && (selectedClass == undefined || selectedClass == 'undefined') ) {
							selectedClass1='NULL';
							break;
					}

				//if(registeredNsingleCourseClassIds != '' && recertifyId !='R') { // Fixed For this Issue #0040101 First time Recertifying registeredNsingleCourseClassIds has Value.
					// selectedClass = registeredNsingleCourseClassIds;
				//} Commented by rajeshwar for #71732


				params += '"'+c+'":'+'{';
				params += '"tpid":"'+prgId+'",';
				params += '"courseid":"'+courseId+'",';
				params += '"classid":"'+selectedClass+'",';
				params += '"recertifyid":"'+recertifyId+'"';
				params += '}';

				classIds +=  selectedClass;
				if(inc < courseListIds.length) {
					params += ',';
					classIds += ',';
				 }

			}

			params += '}';
			
		 /* Start : #0069220 Added by ganesh for implement cookieless option in Salesforce on Oct 6th 2016 10:40 AM -  Passing session variables to ajax callback */
		 			var sf_exp_ses_value = $('.salesforce-widget #widget').attr('data-exp-sess-id');  
					if (typeof sf_exp_ses_value !== typeof undefined && sf_exp_ses_value !== false) {
						 if(params!=''){
						    params = params+'&exp_sess_id='+sf_exp_ses_value; 
						 }else{
						      params = 'exp_sess_id='+sf_exp_ses_value;
						 }
					 }					 
		   /* End : #0069220 Added by ganesh for implement cookieless option in Salesforce */   
		   

			if(selectedClass1 == "NULL") {
				var errMsg = new Array();
				errMsg[0] = Drupal.t("ERR066");
				var message_call = expertus_error_message(errMsg,'error');
				$('#show_expertus_message').html(message_call);
				$('#show_expertus_message').show();

				/* $("#errMsgDisplay").html(Drupal.t("ERR149"));
				 $(".error").css('display','block');*/
			}else{
				if(addToCart != 'Cart'){

					var MasterMandatory=$('#assignPrg_checkbox_'+prgId+':checked').val();
					if(MasterMandatory==undefined){
						MasterMandatory='N';
					}
					this.createLoader('tainingplan-register'+prgId);
					//alert(MasterMandatory)
					var url = this.constructUrl("ajax/trainingplan/class-list-register/"+isAdminSide+ "/" + userListIds+ "/" + fromPage+ "/" + MasterMandatory);
					var obj = this;
					$.ajax({
						type: "POST",
						url: url,
						data:  params,
						datatype: 'text',
						success: function(result){

							if(isAdminSide =="Y") {
								result = eval(result);
							}else{
								result = $.trim(result);
							}

							if(isAdminSide =="Y") {
								 $("#tainingplan-register"+prgId).remove();
								 obj.displayRegistrationStatus(result);
							}else if(result == 'Registered' || result == 'Waitlisted') {

							   obj.callObjectAvailableSeats(prgId);
							   if(fromPage != 'MyTeam'){
								   $("#tainingplan-register"+prgId).remove();
								   $("#object-registerCls_"+prgId).removeAttr('onclick');
								   if(result == 'Waitlisted') {
									   $("#object-registerCls_"+prgId).removeClass("action-btn-waitlist");
									   $("#object-registerCls_"+prgId).html(Drupal.t('LBL190'));
								   }else{
									   $("#object-registerCls_"+prgId).removeClass("action-btn");
									   $("#object-registerCls_"+prgId).html(Drupal.t('Registered'));
								   }
								   $("#object-registerCls_"+prgId).addClass("action-btn-disable");

								   if (widgetId=='lnr-catalog-search') {
									   var currentPage = $('.ui-pg-input').val();
									   if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
											$("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
										}
								   }
								   if(document.getElementById("learningplan-tab-inner")){
									   if(widgetId == 'enroll-lp-result-container'){ // Expired Tab Reload issue #0046138
										   $("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_exp","EnrollExpired");
									   }
								   }
								   if(document.getElementById("learning-plan-details-display-content"))
									   location.reload();
							   }else{
								   $("#paintClassesSelectList"+prgId).remove();
								   $("#assignClass_"+prgId).removeAttr('onclick');
								   $("#assignClass_"+prgId).removeClass("action-btn");
								   $("#assignClass_"+prgId).addClass("action-btn-disable");
								   if(result == 'Waitlisted') {
									   $("#assignClass_"+prgId).html(Drupal.t('LBL190'));
								   }else{
									   $("#assignClass_"+prgId).html(Drupal.t('Registered'));
								   }
								   var currentPage = $('.ui-pg-input').val();
								   $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:currentPage}]);
							   }
						   }else{
							   var errResult = new Array();
							   errResult[0] = result;
							   var err_msg = expertus_error_message(errResult,'error');
								$('#show_expertus_message').html(err_msg);
								$('#show_expertus_message').show();
						   }
						   obj.destroyLoader('tainingplan-register'+prgId);
						}
				    });
				}
				else{
					var waitlist = 0; // waitlist function
					this.callAddToCart(widgetId,'', prgId,addToCart,classIds,waitlist);
				}
			}
	 }catch(e){
			// to do
		}
	},


	displayRegistrationStatus : function(result) {
	 try{
		var rhtml='';
		rhtml += "<table border='1' cellpadding='5' style='width: 100%;'>";
		rhtml += "<tr style='background-color:#EFEFEF'>";
		rhtml += "<td style='width: 150px;font-weight:bold;font-size:15px;'>User Name</td>";
		rhtml += "<td style='width: 150px;font-weight:bold;font-size:15px;'>Status</td>";
		rhtml += "</tr>";
		for(c=0; c < result.length ; c++){

			rhtml += "<tr>";
			rhtml += "<td>"+result[c]['username']+"</td>";
			rhtml += "<td>"+result[c]['status']+"</td>";
			rhtml += "</tr>";

		}
		rhtml += "</table>";

		$("#displayRegisteredUserStatus").html(rhtml);
	 }catch(e){
			// to do
		}
	},


	myteamAccordin : function(widgetId,prgId,addToCart) {
		try{
		//alert(1)
		}catch(e){
			// to do
		}
	},

	renderPopup : function(widgetId,prgId,addToCart,fromPage,recertifyId) {
		try{
		var html = "<div id='tainingplan-register"+prgId+"' style='height:250px;width:600px;overflow:visible;padding:0px;'></div>";
		$("#tainingplan-register"+prgId).remove();
		$('body').append(html);

		var nHtml = "<div id='show_expertus_message'></div><table id='lnr-tainingplan-register' class='lnr-tainingplan-register learningplan-content-wrapper'></table>";
		$('#tainingplan-register'+prgId).html(nHtml);

		var closeButt = {};
		closeButt[Drupal.t('LBL569')]=function(){
			eval('$("body").data("learningcore").registerTPClassDetails(\''+widgetId+'\',\''+prgId+'\',\''+addToCart+'\',\''+fromPage+'\',\''+recertifyId+'\')');


		};
		var prewidth = 780;
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
		    var iwidth = Math.max(document.documentElement.clientWidth, window.innerWidth || 0);
		    prewidth = iwidth - 40;
		}
		$("#tainingplan-register"+prgId).dialog({
			bgiframe: true,
			width:prewidth,
			resizable:false,
			draggable:false,
			closeOnEscape: false,
			modal: true,
			title:Drupal.t('LBL716'),
			buttons: closeButt,
			close: function(){
				$("#tainingplan-register"+prgId).remove();
			},
			overlay: {
			   opacity: 0.5,
			   background: '#000000'
			 }
		});
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="admin-save-button-left-bg"></div>');
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("admin-save-button-middle-bg");
		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="admin-save-button-right-bg"></div>');

		 this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
		    changeDialogPopUI();
		 	}
		
		//$('.ui-dialog-buttonpane .ui-dialog-buttonset button').addClass("removebutton");
		//$('#tainingplan-register'+prgId).parent().css('border','4px solid gray');
		}catch(e){
			// to do
		}
	},


	callPopupOrNot : function(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId) {
		try{
		/**
 		* 0024925: Enforce equivalence in training plans
 		* for this ticket we created equivalence option for training plan
 		* don't delete it. Its may need for feature enhancement
 		*
 		*/
		/*
			var obj = this;
			var userId = this.getLearnerId();
			if(!userId) {
				this.callMessageWindow('registertitle',"Please sign in to proceed.");
			}
			obj.callEquivalencePopupTP(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId);
		}catch(e){
			// to do
		}
	},

	callPopupOrNotTP : function(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId) {
		try{
			var obj = this;
			var userId = this.getLearnerId();

			//obj.callEquivalencePopupTP(widgetId,userId, prgId);
		*/
		if(typeof(recertifyId) == 'undefined') recertifyId='';
		var userId = this.getLearnerId();
		var LMSNodeId = 0;
		var returnMsg;
		var userIdList = this.getLearnerId();
		var loaderId;
		
		if(!userId) {
			this.callMessageWindow('registertitle',"Please sign in to proceed.");
		}else{
			if(userListIfAdminSide != null && userListIfAdminSide != '') {
				userIdList = userListIfAdminSide;
			}

				var isCart =(addToCart =='Cart') ? 1 : 0;
				if(addToCart == 'Cart'){
					var tpData 	= eval($('#object-addToCartList_'+prgId).attr("data"));
					LMSNodeId = tpData.NodeId;
				 }
				var MasterMandatory = 'N';
				var fromPage = 'catalogpopup';
				if(document.getElementById(widgetId)!=null){ // Loader display on widgetId which passed
					loaderId = widgetId;
					this.createLoader(widgetId);
				}else{
					if(document.getElementById('tbl-paintCatalogContentResults')!= null){ // Check for team page or not
						loaderId = 'tbl-paintCatalogContentResults';
						this.createLoader('tbl-paintCatalogContentResults');
					}else{ // default loader
						loaderId = 'paintContent';
						this.createLoader('paintContent');
					}
				}
				/*if(fromPage != 'MyTeam'){
					this.createLoader('paintContent');
				}else{
					this.createLoader('tbl-paintCatalogContentResults');
				}*/
				var pageandmandatory = fromPage + "-" +MasterMandatory;
				var params = {"recertifyid":recertifyId};
				var url = this.constructUrl("ajax/trainingplan/class-cnt-for-course/"+prgId + "/" + LMSNodeId +"/" + isCart+"/" + isAdminSide+"/" + userIdList+"/" + pageandmandatory);
				var obj = this;
				$.ajax({
					type: "POST",
					url: url,
					data:  params,
					success: function(result){

						returnMsg =(addToCart !='Cart' || ($.type(result) === "string")) ? result : result.cart_msg;
					    returnMsg = $.trim(returnMsg);

							if(returnMsg == 'MultiRegister') {
						        obj.getModuleListForTP(widgetId, prgId,addToCart,isAdminSide,userIdList,'catalogpopup',recertifyId);

							} else if(returnMsg == 'Registered' || returnMsg == 'Waitlisted') {
									obj.callObjectAvailableSeats(prgId);
								  /* $("#object-registerCls_"+prgId).removeAttr('onclick');
								   $("#object-registerCls_"+prgId).removeClass("action-btn");
								   $("#object-registerCls_"+prgId).addClass("action-btn-disable");
								   $("#object-registerCls_"+prgId).html("Registered");

								   if (widgetId=='lnr-catalog-search') {
									   var currentPage = $('.ui-pg-input').val();
									   $("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
								   }
								   */

									if(fromPage != 'MyTeam'){
										   $("#object-registerCls_"+prgId).removeAttr('onclick');
										   if(returnMsg == Drupal.t('LBL190')) {
											   $("#object-registerCls_"+prgId).removeClass("action-btn-waitlist");
											   $("#object-registerCls_"+prgId).html(Drupal.t('LBL190'));
										   }else{
											   $("#object-registerCls_"+prgId).removeClass("action-btn");
											   $("#object-registerCls_"+prgId).html(Drupal.t('Registered'));
										   }
										   $("#object-registerCls_"+prgId).addClass("action-btn-disable");

										   if (widgetId=='lnr-catalog-search') {
											   var currentPage = $('.ui-pg-input').val();
											   if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
													$("#paintContentResults").trigger("reloadGrid",[{page:currentPage}]);
												}
										   }else if(widgetId == 'enroll-lp-result-container' && recertifyId.indexOf('R')>=0) {
											   var currentPage = $('.ui-pg-input').val();
											   $("#paintEnrollmentLPResults").trigger("reloadGrid",[{page:currentPage}]);
										   }
										   if(document.getElementById("learning-plan-details-display-content"))
											   location.reload();
									   }else{
										   $("#assignClass_"+prgId).removeAttr('onclick');
										   $("#assignClass_"+prgId).removeClass("action-btn");
										   $("#assignClass_"+prgId).addClass("action-btn-disable");
										   if(returnMsg == Drupal.t('LBL190')) {
											   $("#assignClass_"+prgId).html(Drupal.t('LBL190'));
										   }else{
											   $("#assignClass_"+prgId).html(Drupal.t('Registered'));
										   }
										   var currentPage = $('.ui-pg-input').val();
										   $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:currentPage}]);
									   }

							} else if(returnMsg == 'CartAdded') {
								$("#object-addToCartList_"+prgId).removeAttr('onclick');
								$("#object-addToCartList_"+prgId).removeClass("action-btn");
								$("#object-addToCartList_"+prgId).addClass("action-btn-disable");
								$("#object-addToCartList_"+prgId).html(Drupal.t('LBL049'));
								if (parseInt(result.total_product) > 0) {
									$("#ShoppingCartItemsId").addClass("filled");
									$("#ShoppingCartItemsId").html(result.total_product);
								} else {
									$("#ShoppingCartItemsId").removeClass("filled");
									$("#ShoppingCartItemsId").html('');
								}
							} else {

								if(isAdminSide != 'Y'){
										if(returnMsg == 'nocourses'){
											//returnMsg = SMARTPORTAL.t('No courses has been associated');
											returnMsg = Drupal.t('MSG268');
										} else if(returnMsg == 'noclasses'){
											//returnMsg = SMARTPORTAL.t('No classes under one of the associated course(s)');
											returnMsg = Drupal.t('MSG379');
										} else if(returnMsg == 'seatsfull'){
											//returnMsg = SMARTPORTAL.t('No seats available for this Program');
											returnMsg = Drupal.t('MSG380');
										}

										obj.callMessageWindow('registertitle',returnMsg);

								}else{

									if(returnMsg == 'nocourses' || returnMsg == 'noclasses' || returnMsg == 'seatsfull'){
										if(returnMsg == 'nocourses'){
											//returnMsg = SMARTPORTAL.t('No courses has been associated');
											returnMsg = Drupal.t('MSG268');
										}else if(returnMsg == 'noclasses') {
											returnMsg = SMARTPORTAL.t('No classes under one of the associated course(s)');

										}else if(returnMsg == 'seatsfull'){
											//returnMsg = SMARTPORTAL.t('No seats available for this Program');
											returnMsg = Drupal.t('MSG380');
										}
										obj.callMessageWindow('registertitle',returnMsg);
									}else{
										returnMsg = eval(result);
										obj.displayRegistrationStatus(returnMsg);
									}

								}
							}
							obj.destroyLoader(loaderId);
					}
			    });
		}
		}catch(e){
			// to do
			//console.log(e);
		}
	},


	getModuleListForTP : function(widgetId, prgId, addToCart,isAdminSide,userListIfAdminSide,fromPage,recertifyId) {
		try{
		if(fromPage == 'MyTeam') {
			this.myteamAccordin(widgetId,prgId,addToCart);
			this.createLoader('paintClassesSelectList'+prgId);
		}else{
			this.renderPopup(widgetId,prgId,addToCart,fromPage,recertifyId);
			this.createLoader('tainingplan-register'+prgId);
		}

			if(isAdminSide == 'Y') {
				$(".error").css('display','none');
			}

            var url = this.constructUrl("ajax/trainingplan/module-list/"+prgId+ "/" + userListIfAdminSide+ "/" +isAdminSide+ "/"+fromPage);			
			var obj = this;
			var params = {"recertifyid":recertifyId};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
						//obj.paintTPCourseDetails(result,prgId,isAdminSide,userIdList);

						obj.paintTPModuleDetails(widgetId,addToCart,result,prgId,isAdminSide,userListIfAdminSide,fromPage);
						//$(".lnr-tainingplan-register").jScrollPane();
						$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
						if(isAdminSide == 'Y') {
							$(".error").css('display','none');
						}
						/*for(c=0; c < result.length ; c++){
								obj.accordionForModuleView(result[c]['module_details']['module_id']);
						}*/
						if(fromPage == 'MyTeam') {
							obj.destroyLoader('paintClassesSelectList'+prgId);
						}else{
							obj.destroyLoader('tainingplan-register'+prgId);
						}
						/*-- #34270 - scroll pane for the dynamic contents in team assign learn popup --*/
						if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
							$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
						}
						if(fromPage == 'MyTeam') {
							resetFadeOutByClass('.tp-register-select-class-crs-title', 'item-title-code', 'line-item-container', 'myteam');
							resetFadeOutByClass('.course-sub-class-detail', 'item-title-code', 'line-item-container', 'myteam');
						}
						if(fromPage == 'catalogpopup'){
							resetFadeOutByClass('.page-learning-catalog-search .ui-widget-content .content-detail-container ', 'content-detail-code', 'line-item-container', 'catalogpopup');
						}
				}
		    });
			
		}catch(e){
			// to do
		}
	},

	paintTPModuleDetails : function(widgetId,addToCart,result,prgId,isAdminSide,userIdList,fromPage){
	 try{
		
		var rhtml = "";
		var c;
		var moduleTitle;
		var moduleId;
		var moduleCode;
		var moduleDesc;

		var courseList = '';
		var courseId;
		this.currTheme = Drupal.settings.ajaxPageState.theme;

		var obj = $("body").data("learningcore");
		var objStr = '$("body").data("learningcore")';
		if(result.length > 0 ) {
			//rhtml += "<tr><td><div id='show_expertus_message'></div></td></tr>";
			rhtml += "<tr><td><div class='main-module-level'>";
			rhtml += "<tr><td><div class='module-level'>";
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse' cellspacing='0'>";
			for(c=0; c < result.length ; c++){

				moduleId = result[c]['module_details']['group_code'];
				moduleTitle = result[c]['module_details']['group_title'];
				moduleCode = result[c]['module_details']['module_code'];
				moduleDesc = result[c]['module_details']['module_desc'];
				//modTitle = result[c]['module_details']['module_title'];
				//modId = result[c]['module_details']['module_id'];
				if(result[c]['course_details'].length > 0) {
					var z = 0;
					var inc5 = 0;
					var passParams = "[";

					var crsDetailsLenth = result[c]['course_details'].length;

					 if(courseList != '') {
						 courseList += ',';
					 }

					$(result[c]['course_details']).each(function(){

						 courseId = result[c]['course_details'][inc5]['crs_id'];
						 courseList += courseId;
						 var cls_det='[';
						 var clscnt = 0;
						 var clsLength = result[c]['course_details'][inc5]['class_details'].length;
						 //alert(result[c]['course_details'][inc5]['class_details'].toSource())
						 $(result[c]['course_details'][inc5]['class_details']).each(function(){
							 var x = this;
							 cls_det += '{';
							 cls_det += "'cls_id':'"+x["class_details"]["cls_id"]+"',";
							 cls_det += "'crs_id':'"+x["class_details"]["crs_id"]+"',";
							 cls_det +=	"'cls_code':'"+escape(x['class_details']['cls_code'])+"',";
							 cls_det += "'cls_title':'"+escape(x['class_details']['cls_title'])+"',";
							 cls_det += "'cls_short_description':'"+escape(x['class_details']['cls_short_description'])+"',";
							 cls_det += "'delivery_type_code':'"+x['class_details']['delivery_type_code']+"',";
							 cls_det += "'delivery_type_name':'"+x['class_details']['delivery_type_name']+"',";
							 cls_det += "'language':'"+x['class_details']['language']+"',";
							 cls_det += "'session_id':'"+x['class_details']['session_id']+"',";
							 cls_det += "'country_name':'"+x['class_details']['country_name']+"',";
							 cls_det += "'location':'"+escape(x['class_details']['location'])+"',";
							 cls_det += "'loationaddr1':'"+escape(x['class_details']['loationaddr1'])+"',";
							 cls_det += "'locationaddr2':'"+escape(x['class_details']['locationaddr2'])+"',";
							 cls_det += "'locationcity':'"+escape(x['class_details']['locationcity'])+"',";
							 cls_det += "'locationstate':'"+x['class_details']['locationstate']+"',";
							 cls_det += "'locationzip':'"+x['class_details']['locationzip']+"',";
							 cls_det += "'sess_start_date':'"+x['class_details']['sess_start_date']+"',";
					
							 var enrolled_det = '[';
							 if(x['class_details']['enrolled_id']) {
							 	enrolled_det += '{';
							 	enrolled_det += "'comp_status':'"+x['class_details']['enrolled_id'].comp_status+"',";
							 	enrolled_det += "'enrolled_id':'"+x['class_details']['enrolled_id'].enrolled_id+"',";
							 	enrolled_det += "'enrolled_status':'"+x['class_details']['enrolled_id'].enrolled_status+"',";
							 	enrolled_det += "'waitlist_flag':'"+x['class_details']['enrolled_id'].waitlist_flag+"',";
							 	enrolled_det += "'waitlist_priority':'"+x['class_details']['enrolled_id'].waitlist_priority+"'";						 								 	
							 	enrolled_det += '}';
							 }
							 enrolled_det += ']';
							 cls_det += "'enrolled_id':"+enrolled_det+",";
							 var enrolled_last_comp_class = '[';
							 if(x['class_details']['class_last_comp']) {
							 	enrolled_last_comp_class += '{';
							 	enrolled_last_comp_class += "'comp_status':'"+x['class_details']['class_last_comp'].comp_status+"',";
							 	enrolled_last_comp_class += "'enrolled_id':'"+x['class_details']['class_last_comp'].enrolled_id+"',";
							 	enrolled_last_comp_class += "'comp_on':'"+x['class_details']['class_last_comp'].comp_on+"'";						 								 	
							 	enrolled_last_comp_class += '}';
							 }
							 enrolled_last_comp_class += ']';
							 
							 cls_det += "'enrolled_last_comp_class':"+enrolled_last_comp_class+",";
							 cls_det += "'waitlist_status':'"+x['class_details']['waitlist_status']+"',";
							 var sesslis = '[';
							 if(x['session_details'].length > 0) {
								 var inc = 0;
								 var sessLen = x['session_details'].length;
								 $(x['session_details']).each(function(){
									inc=inc+1;
									sesslis += '{';
									sesslis += "'session_title':'"+(escape($(this).attr("session_title")) ? escape($(this).attr("session_title")) : '')+"',";
								       sesslis += "'session_day':'"+$(this).attr("session_day")+"',";
								       sesslis += "'start_date':'"+$(this).attr("start_date")+"',";
								       sesslis += "'start_time':'"+$(this).attr("start_time")+"',";
								       sesslis += "'end_time':'"+$(this).attr("end_time")+"',";
								       sesslis += "'start_form':'"+$(this).attr("start_form")+"',";
								       sesslis += "'sessionInstructorName':'"+escape($(this).attr("session_instructor_name"))+"',";
                                if(x['class_details']['delivery_type_code']=='lrn_cls_dty_ilt'){
                                    sesslis += "'ilt_session_day':'"+$(this).attr("ilt_session_day")+"',";
									sesslis += "'ilt_start_date':'"+$(this).attr("ilt_start_date")+"',";
									sesslis += "'ilt_start_time':'"+$(this).attr("ilt_start_time")+"',";
									sesslis += "'ilt_end_time':'"+$(this).attr("ilt_end_time")+"',";
									sesslis += "'ilt_start_form':'"+$(this).attr("ilt_start_form")+"',";
									sesslis += "'ilt_end_form':'"+$(this).attr("ilt_end_form")+"',";
									sesslis += "'loc_tzcode':'"+$(this).attr("loc_tzcode")+"',";
									sesslis += "'loc_tz':'"+$(this).attr("loc_tz")+"',";
									sesslis += "'user_tz':'"+$(this).attr("user_tz")+"',";
									sesslis += "'session_id':'"+$(this).attr("session_id")+"',";
									sesslis += "'user_tzcode':'"+$(this).attr("user_tzcode")+"',";
                                                                 }
								    	sesslis += "'end_form':'"+$(this).attr("end_form")+"'";
									sesslis += '}';
									 if(inc < sessLen) {
										 sesslis += ",";
									 }

								 });
							 }
							 sesslis += ']';
							 cls_det += "'session_details':"+sesslis+",";
							 cls_det += "'availableSeats':'"+x['class_details']['availableSeats']+"'";
							 cls_det += '}';
							 clscnt++;
							 if(clscnt < clsLength)
								 cls_det += ',';

						 });
						 cls_det += ']';
						 //alert(cls_det)
						 z = z + 1;
						 if(z < crsDetailsLenth) {
								courseList += ',';
						 }


						 passParams += "{";
						 passParams += "'crs_id':'"+result[c]['course_details'][inc5]['crs_id']+"',";
						 passParams += "'crs_code':'"+escape(result[c]['course_details'][inc5]['crs_code'])+"',";
						 passParams += "'crs_title':'"+escape(result[c]['course_details'][inc5]['crs_title'])+"',";
						 passParams += "'crs_desc':'"+escape(result[c]['course_details'][inc5]['crs_desc'])+"',";
						 passParams += "'is_required':'"+result[c]['course_details'][inc5]['is_required']+"',";
						 passParams += "'registered_cnt':'"+result[c]['course_details'][inc5]['registered_cnt']+"',";
						 passParams += "'single_class':'"+result[c]['course_details'][inc5]['single_class']+"',";
						 passParams += "'sequence_no':'"+result[c]['course_details'][inc5]['sequence_no']+"',";
						 passParams += "'start_date':'"+result[c]['course_details'][inc5]['start_date']+"',";
						 passParams += "'end_date':'"+result[c]['course_details'][inc5]['end_date']+"',";
						 passParams += "'clscount':'"+result[c]['course_details'][inc5]['clscount']+"',";
						 passParams += "'class_details':"+cls_det;
						 passParams += "}";


						 if(result[c]['course_details'][inc5]['registered_cnt'] == 1 && isAdminSide != 'Y') {

							var regValue =  result[c]['course_details'][inc5]['cls_id'];

						 }else if (result[c]['course_details'][inc5]['clscount'] == 1 ){
							var regValue =  result[c]['course_details'][inc5]['single_class'];

						 }else{
							var regValue = '';
						 }

						rhtml += '<input type="hidden" id="classListIds-'+courseId+'" name="classListIds" value="'+regValue+'">';


						 inc5=inc5+1;
						 if(inc5 < crsDetailsLenth) {
							 passParams += ",";
						 }

					});
					passParams += "]";


				}else{
					var passParams="''";
				}

			    var param = "data={'prgId':'"+prgId+"'," +
			    "'moduleId':'"+moduleId+"'," +
			    "'isAdminSide':'"+isAdminSide+"'," +
			    		"'userIdList':'"+userIdList+"'," +
   						"'courseDetails':"+passParams+"}";

				//var param = "data={'courseDetails':"+passParams+"}";


	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-crs-module-'+moduleId+'">';
				rhtml += '<td>';
				// Added condition for 0072664
				if(moduleId == null || moduleId == undefined || moduleId == ''){
					rhtml += '<div class="set-height-column-left" id="lp-crs-accodion-module-'+moduleId+'" data="'+param+'"></div>';
				}else{
					if(fromPage == "MyTeam") {
						rhtml += '<a class="set-height-column-left title_close" id="lp-crs-accodion-module-'+moduleId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForModuleView("'+moduleId+'","'+fromPage+'");\'>&nbsp;</a>';
						rhtml += '<div class="tp-register-select-module-title"><div class="set-height-column-right"><span class="item-title limit-title-row"><span class="limit-title limit-title-mulcls-module-title lp-reg-class-title vtip " title="'+unescape(moduleTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
					} else {
						rhtml += '<a style="display: none;" class="set-height-column-left title_close" id="lp-crs-accodion-module-'+moduleId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForModuleView("'+moduleId+'","'+fromPage+'");\'>&nbsp;</a>';
						// rhtml += '<div class="tp-register-select-module-title"><div class="set-height-column-right">';
						rhtml += '<div class="lp_group_title"><fieldset><legend align="left">';
						rhtml += '<span class="item-title limit-title-row"><span class="limit-title limit-title-mulcls-module-title lp-reg-class-title vtip " title="'+unescape(moduleTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
					}
				/*if(fromPage == "MyTeam") {
					if(this.currTheme == "expertusoneV2"){
						rhtml += titleRestrictionFadeoutImage(moduleTitle,'tp-view-module-title');
						//rhtml += titleRestricted(moduleTitle,80);
					}else{
						rhtml += titleRestricted(moduleTitle,70);
					}
				}else{
					rhtml += titleRestrictionFadeoutImage(moduleTitle,'tp-view-module-title');
				}*/
					rhtml += titleRestrictionFadeoutImage(moduleTitle,'tp-view-module-title', 20);
				rhtml += '</span></span>';
				//rhtml += '<div class="course-info item-title-code">';
				//rhtml += '<span title="'+unescape(moduleCode)+'">'+titleRestricted(moduleCode,10)+'</span>';
				//rhtml += '<span class="pipeline">|</span>';
				//rhtml += '</div>';

				//rhtml += '<div class="course-desc-info">';
				//rhtml += '<span>'+moduleDesc+'</span>';
				//rhtml += '</div>';
					if(fromPage == "MyTeam") {
						rhtml += '</div></div>';
					} else {
						rhtml += '</legend></fieldset></div>';
					}
				}
				rhtml += '</td>';
				rhtml += '</tr>';

		  }


			 rhtml += '<tr><td><input type="hidden" id="courseListIds" name="courseListIds" value="'+courseList+'"></td></tr>';

	    	 rhtml += '<tr><td><input type="hidden" id="isadminsidefield" name="isadminsidefield" value="'+isAdminSide+'"></td></tr>';

			 rhtml += '<tr><td><input type="hidden" id="userListIds" name="userListIds" value="'+userIdList+'"></td></tr>';

			 if(fromPage == "MyTeam") {
				 var fnClick = objStr+'.registerTPClassDetails("'+widgetId+'","'+prgId+'","'+addToCart+'","'+fromPage+'")';
				// var fnClick = '$("#lnr-myteam-search").data("lnrmyteamsearch").registerTPObjectDetails("'+prgId+'","'+userIdList+'")';
				 rhtml += "<tr><td><div class='admin-save-button-container'><div class='admin-save-button-left-bg'></div><input type='button' class ='assign-btn admin-save-button-middle-bg' id='assign_btn_"+prgId+"' value = '"+Drupal.t('LBL569')+"' onclick='"+fnClick+"'><div class='admin-save-button-right-bg'></div></div></td></tr>";
			 }


		  rhtml += '</table>';
		  rhtml += '</div></td></tr>';
		}
		else{
			rhtml += "<tr><td class='no-item-found'>"+SMARTPORTAL.t('No modules associated with this.')+"</td></tr>";
		}
		 if(fromPage == "MyTeam") {
			 $("#paintClassesSelectList"+prgId).html(rhtml);
		 }else{
			 $("#lnr-tainingplan-register").html(rhtml);
		 }
		 //$(".lnr-tainingplan-register").jScrollPane();
		 $("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		 $('.enroll-show-morecourse .set-height td .title_close').trigger("click");

		// Added condition for 0072664
		 var grpId = result[0]['module_details']['group_code'];
		 if(grpId == null || grpId == undefined || grpId == ''){
			 this.accordionForModuleView(grpId,fromPage);
			 $('#lp-crs-accodion-module-'+grpId).removeClass('title_open');
		 }
		 
		 for(c=0; c < result.length ; c++){

			moduleId = result[c]['module_details']['group_code'];
			var data = eval($("#lp-crs-accodion-module-"+moduleId).attr("data"));
			var classDetSec = this.paintTPCourseDetails(data,fromPage);
			/*-- #34270 scroll pane for the dynamic contents in team assign learn popup --*/
			if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
				$("#my-team-dialog .class-details-info tr:last-child .course-delivery-info").css('border-bottom','0px none');
				$("#my-team-dialog .course-level .set-height-column-right").css('border-bottom','0px none');
				$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
			}else{
				$("#lnr-tainingplan-register .class-details-info tr:last-child .course-delivery-info").css('border-bottom','0px none');
				$("#lnr-tainingplan-register .course-level .set-height-column-right").css('border-bottom','0px none');
			}
			$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
			//$(".lnr-tainingplan-register").jScrollPane();
		 }
		 vtip();
		 resetFadeOutByClass('#lnr-tainingplan-register','content-detail-code','line-item-container','select');		 
	 	 $('.limit-title-mulcls-title').trunk8(trunk8.tpmod_title);
		 $('.limit-desc-mulcls').trunk8(trunk8.tpmod_desc);
		 $('.limit-title-mulcls-module-title').trunk8(trunk8.tpgroup_title);
		 //Overlay container display full bg height
		 $('#tainingplan-register'+prgId).parent(".ui-dialog").next('.ui-widget-overlay').css({"height":"100%", "position":"fixed"})
		 scrollBar_Refresh('tp-select-class');
	 }catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},

	accordionForModuleView : function(moduleId,fromPage) {
	 try{
		var currTr = moduleId;

		if(!document.getElementById(currTr+"ModuleSubGrid")) {
			$("#lp-crs-module-"+currTr).after("<tr id='"+currTr+"ModuleSubGrid'><td colspan='4'><div id='module-details-"+currTr+"' ></div></td></tr>");
			$("#"+currTr+"ModuleSubGrid").show();//css("display","block");
			//$("#"+currTr+"ModuleSubGrid").css("visibility", 'visible');
			$("#lp-crs-accodion-module-"+currTr).removeClass("title_close");
			$("#lp-crs-accodion-module-"+currTr).addClass("title_open");
			$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ModuleSubGrid").slideDown(1000);
			$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
		} else {
			var clickStyle = $("#"+currTr+"ModuleSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"ModuleSubGrid").show();//css("display","block");
    			//$("#"+currTr+"ModuleSubGrid").css("visibility", 'visible');
    			$("#"+currTr+"ModuleSubGrid").slideDown(1000);
    			$("#lp-crs-accodion-module-"+currTr).removeClass("title_close");
				$("#lp-crs-accodion-module-"+currTr).addClass("title_open");
				$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
    		} else {
    			$("#"+currTr+"ModuleSubGrid").hide();//css("display","none");
    			//$("#"+currTr+"ModuleSubGrid").css("visibility", 'hidden');
    			$("#"+currTr+"ModuleSubGrid").slideUp(1000);
    			$("#lp-crs-accodion-module-"+currTr).removeClass("title_open");
				$("#lp-crs-accodion-module-"+currTr).addClass("title_close");
				$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-module-"+currTr).addClass("ui-widget-content");
				//$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","1px solid #cccccc"); commented for border line within select class in recertify popup

    		}
		}


		//var data = eval($("#lp-crs-accodion-module-"+currTr).attr("data"));


		//var classDetSec = this.paintTPCourseDetails(data,fromPage);
		$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
		/*-- #34270 scroll pane for the dynamic contents in team assign learn popup --*/
		if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
			$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
		}
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		//$(".lnr-tainingplan-register").jScrollPane();
	 }catch(e){
			// to do
		}
	},


	paintTPCourseDetails : function(data, fromPage){
	 try{
		var crslength = data.courseDetails.length;
		var rhtml = "";
		var c;
		var courseId;
		var courseCode;
		var compStatus;
		var courseTitle;
		var isRequired;
		var registeredCnt;
		var courseList = '';
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var inc = 0;

		var obj = $("body").data("learningcore");
		var objStr = '$("body").data("learningcore")';
		rhtml += "<table cellpadding='0' cellspacing='0' style='width:100%'>";
		if(crslength > 0 ) {

			rhtml += "<tr><td><div class='course-level'>";
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse' cellspacing='0'>";
			for(c=0; c < crslength ; c++){
				var rhtmlcls = obj.paintTPClassDetails(data.courseDetails[c]['class_details'], '', fromPage);
				courseId 	= data.courseDetails[c]['crs_id'];
				courseCode 	= data.courseDetails[c]['crs_code'];
				courseTitle = data.courseDetails[c]['crs_title'];
				courseDesc = data.courseDetails[c]['crs_desc'];
				isRequired = data.courseDetails[c]['is_required'];
				registeredCnt = data.courseDetails[c]['registered_cnt'];

				courseList += courseId;

				inc = inc + 1;
				if(inc < crslength) {
					courseList += ',';
				 }

				var param="data={'PrgId':'"+data.prgId+"','CourseId':'"+courseId+"','UserId':'"+data.userIdList+"'}";
	 			rhtml += '<tr class="set-height" id ="lp-crs-course-'+courseId+'">';
				rhtml += '<td>';
				if(fromPage == "MyTeam") {
					rhtml += '<a class="set-height-column-left title_open" id="lp-crs-accodion-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForCourseView('+courseId+', "MyTeam");\'>&nbsp;</a>';
					rhtml += '<div class="tp-register-select-class-crs-title"><div class="set-height-column-right"><span class="item-title limit-title-row"><span class="limit-title limit-title-mulcls-title lp-reg-class-title vtip" title="'+unescape(courseTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				} else {
					// rhtml += '<a class="set-height-column-left title_open" id="lp-crs-accodion-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForCourseView('+courseId+');\'>&nbsp;</a>';
					rhtml += '<div class="content-row-container cre_sys_obt_crs cls-course-title-code" id="content-row-container-'+courseId+'">';
					rhtml += '<div class="content-icon-container"><div title="' + Drupal.t("Course") + '" class="content-icon crs-icon vtip"></div></div>';
					// rhtml += '';
					// rhtml += '';
					// rhtml += '';
					rhtml += '<div class="content-detail-container"><div class="tp-register-select-class-crs-title"><div class="set-height-column-right"><span class="item-title limit-title-row content-title"><span class="limit-title limit-title-mulcls-title lp-reg-class-title vtip" title="'+unescape(courseTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				}
				if(fromPage == "MyTeam") {
					if(this.currTheme == "expertusoneV2"){
						rhtml += unescape(courseTitle);
						//rhtml += titleRestricted(unescape(courseTitle),80);
					}else{
						rhtml += unescape(courseTitle);
					}
				}else {
					rhtml += unescape(courseTitle);
				}
				rhtml += '</span></span>';
				if(fromPage != "MyTeam") {
					rhtml += '</div></div>';
				}
				rhtml += '<div class="course-info item-title-code">';
				rhtml += '<div class="line-item-container float-left" ><span title="'+unescape(courseCode).replace(/"/g, '&quot;')+'" class="vtip">'+titleRestrictionFadeoutImage(unescape(courseCode),'tp-view-course-code')+'</span></div>';
				rhtml += '<div class="line-item-container float-left" ><span class="pipeline">|</span>';
				rhtml += '<span>'+((isRequired =='Y')?''+Drupal.t("Mandatory")+'':''+Drupal.t("Optional")+'')+'</span></div>';
				if(data.isAdminSide != 'Y') {
					rhtml += '<div class="line-item-container float-left" ><span class="pipeline">|</span>';
					rhtml += '<span>'+((registeredCnt < 1)?''+Drupal.t("Not Registered")+'':''+Drupal.t("Registered")+'')+'</span></div>';
				}
				rhtml += '</div>';

				rhtml += '<div class="course-desc-info limit-desc-row">';
				rhtml += '<span class="limit-desc limit-desc-mulcls crs_desc vtip">'+unescape(courseDesc)+'</span>';
				if(fromPage == "MyTeam") {
					rhtml += '</div></div>';
					rhtml += '</td>';
					rhtml += '</tr><tr id='+courseId+'CourseSubGrid ><td colspan="4"><div id="course-details-'+courseId+'">'+rhtmlcls+'</div></td></tr>';
				} else {
					rhtml += '</div></div></div>';	//close tag for <div content-row-container
					rhtml += '<div class="course_list_content" id="' + courseId + 'CourseSubGrid"><div class="course-content-content-wrapper"><div id="course-details-'+courseId+'">'+rhtmlcls+'</div></div></div>';
				rhtml += '<div id="course_content_more_"'+courseId+' class="select-class-course-content-more course-content-more padbt5"><div class="show_more_less" id="lp-crs-accodion-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForCourseView('+courseId+');\'>'+Drupal.t('LBL3042')+'</div></div>';
					if(c!=(crslength-1))
						rhtml += '<div class="clsAfterMoreDiv">&nbsp;</div>';
					rhtml += '</td>';
					// console.log(rhtml);
					rhtml += '</tr>';
					// rhtml += '</tr><tr id='+courseId+'CourseSubGrid ><td colspan="4"><div id="course-details-'+courseId+'">'+rhtmlcls+'</div></td></tr>';
				}
				// rhtml += '</td>';
				// console.log(rhtml);
				rhtml += '</tr>';
				// rhtml += '</tr><tr id='+courseId+'CourseSubGrid ><td colspan="4"><div id="course-details-'+courseId+'">'+rhtmlcls+'</div></td></tr>';

		  }

		  rhtml += '</table>';
		  rhtml += '</div></td></tr>';
		}
		else{
			rhtml += "<tr><td class='no-item-found'>"+Drupal.t('There are no course(s) under this program.')+"</td></tr>";
		}
		rhtml += "</table>";

		$("#module-details-"+data.moduleId).html(rhtml);
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		//$(".lnr-tainingplan-register").jScrollPane();
		//reinitialise jscrollane on description show/hide event
		$(".lnr-tainingplan-register .course-desc-info .more-icon-sec").click(function() {
			//$('.lnr-tainingplan-register').jScrollPane().data().jsp.reinitialise();
			$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		});
		$('.limit-title-mulcls-title').trunk8(trunk8.tpmod_title);
		$('.limit-desc-mulcls').trunk8(trunk8.tpmod_desc);
		$('.lmt-lrp-crs-cls-title').trunk8(trunk8.class_detail_title);
		$('.lmt-lrp-crs-cls-desc').trunk8(trunk8.lp_detail_course_desc);
		resetFadeOutByClass('#lnr-tainingplan-register .cls-course-title-code','course-info','line-item-container','select');
		//resetFadeOutByClass('#lnr-tainingplan-register .class_list_container','cls-class-title-code','line-item-container','select');
		vtip();
		scrollBar_Refresh('tp-select-class');
	 }catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},

	accordionForCourseView : function(courseId, fromPage) {
	 try{
		var currTr = courseId;

		if(!document.getElementById(currTr+"CourseSubGrid")) {
			if(fromPage == "MyTeam") {
				$("#lp-crs-course-"+currTr).after("<tr id='"+currTr+"CourseSubGrid'><td colspan='4'><div id='course-details-"+currTr+"' ></div></td></tr>");
				$("#lp-crs-accodion-course-"+currTr).removeClass("title_close");
				$("#lp-crs-accodion-course-"+currTr).addClass("title_open");
			} else {
				$("#lp-crs-course-"+currTr).find('#content-row-container-'+courseId).after('<div class="course_list_content" id="' + courseId + 'CourseSubGrid"><div class="course-content-content-wrapper"><div id="course-details-'+courseId+'"></div></div></div>');
				$("#lp-crs-accodion-course-"+currTr).text(Drupal.t("LBL3042"));
			}
			
			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
			$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"CourseSubGrid").slideDown(1000);
			$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
		} else {
			var clickStyle = $("#"+currTr+"CourseSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
    			$("#"+currTr+"CourseSubGrid").slideDown(1000);
				if(fromPage == "MyTeam") {
					$("#lp-crs-accodion-course-"+currTr).removeClass("title_close");
					$("#lp-crs-accodion-course-"+currTr).addClass("title_open");
				} else {
					$("#lp-crs-accodion-course-"+currTr).text(Drupal.t("LBL3042"));
				}
				$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
    		} else {
    			$("#"+currTr+"CourseSubGrid").hide();//css("display","none");
    			$("#"+currTr+"CourseSubGrid").slideUp(1000);
				if(fromPage == "MyTeam") {
					$("#lp-crs-accodion-course-"+currTr).removeClass("title_open");
					$("#lp-crs-accodion-course-"+currTr).addClass("title_close");
				} else {
					$("#lp-crs-accodion-course-"+currTr).text(Drupal.t("LBL713"));
				}
				$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-course-"+currTr).addClass("ui-widget-content");
				$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","0px solid #cccccc");
    			$('.set-height-column-right > tbody > tr:last > td').find('.set-height-column-right').css('border-bottom','0px none');
    		}
		}


		var data = eval($("#lp-crs-accodion-course-"+currTr).attr("data"));


		var classDetSec = this.getClassListForCourse(data, fromPage);
		//$(".lnr-tainingplan-register").jScrollPane();
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
	 	$('.limit-title-mulcls-title').trunk8(trunk8.tpmod_title);
		$('.limit-desc-mulcls').trunk8(trunk8.tpmod_desc);
		if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
			$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
		}
		if($('.lnr-tainingplan-register').length > 0) {
			scrollBar_Refresh('tp-select-class');
		}
	 }catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},

	getClassListForCourse : function(data, fromPage){
	 try{
		var prgId = data.PrgId;
		var courseId = data.CourseId;
		var userId = data.UserId;
		//alert(data.toSource())

		if($("#course-details-"+courseId).html() == ''){
			this.createLoader("course-details-"+courseId);
			var url = this.constructUrl("ajax/trainingplan/class-list/" + courseId + "/" + userId);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '',//Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM 
				success: function(result){

					obj.paintTPClassDetails(result,data,fromPage);
					$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
					obj.destroyLoader("course-details-"+courseId);
					/* /*  Commented For Multiple class selection #0038560
					 * for(c=0; c < result.length ; c++){
							var enrolledId = result[c]['class_details']['enrolled_id'];
							if(enrolledId.enrolled_id ) {
								if($('input[name="'+courseId+'-clsregister"]:radio').is(":checked") == true)
								{
									$('input[name="'+courseId+'-clsregister"]:radio').attr('disabled',true);
								}
							}
						} */

						/*-- #34270 scroll pane for the dynamic contents in team assign learn popup --*/
						if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
							$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
						}

				}
		    });
		}
	 }catch(e){
			// to do
		}
	},

	paintTPClassDetails : function(result,data,fromPage){
	 try{
		var isadminsidefield = $('#isadminsidefield').val();
		// console.log('paintTPClassDetails result', result);
		var obj = $("body").data("learningcore");
		var objStr = '$("body").data("learningcore")';
		var rhtml ='';

		rhtml += "<div class='class-level'>";
		rhtml += "<table border='0' cellpadding='0' class='class-details-info' cellspacing='0'>";
		if(result.length > 0) {

			for(c=0; c < result.length ; c++){
				var courseId 	= result[c]['crs_id'];
				var classId 	= result[c]['cls_id'];
				var classCode 	= unescape(result[c]['cls_code']);
				var classTitle 	= unescape(result[c]['cls_title']);
				var classDesc 	= unescape(result[c]['cls_short_description']);
				var deliveryTypeCode 	= result[c]['delivery_type_code'];
				var deliveryTypeName 	= result[c]['delivery_type_name'];
				var language 	= result[c]['language'];
				var sessionId   = result[c]['session_id'];
				var countryName = result[c]['country_name'];
				var locationName = result[c]['location'];
				var locationAddr1 = result[c]['loationaddr1'];
				var locationAddr2 = result[c]['locationaddr2'];
				var locationCity = result[c]['locationcity'];
				var locationState = result[c]['locationstate'];
				var locationZip = result[c]['locationzip'];
				Drupal.t('Web-based');
				Drupal.t('Virtual Class');
				Drupal.t('Video');
				Drupal.t('Classroom');

				var sessionStartDate = result[c]['sess_start_date'];
				if(result[c]['enrolled_id'].length > 0){
					var enrolledId = result[c]['enrolled_id'][0];
				}else{
					var enrolledId = '';
				}
				
				if(result[c]['enrolled_last_comp_class'].length > 0){
					var last_comp_enrolledId = result[c]['enrolled_last_comp_class'][0].enrolled_id;
				}else{
					var last_comp_enrolledId = '';
				}
				
				var waitList = result[c]['waitlist_status'];
				var availableSeats = result[c]['availableSeats'];


				var checkedValue = "";

				if(isadminsidefield != 'Y') {
					if(enrolledId.comp_status == 'lrn_crs_cmp_cmp') {
						checkedValue = "checked";
					}
				}

				if(result.length == 1) {
					checkedValue = "checked";
				}


				var startDate = '';
				var startDateVtip = '';
				var sessioncount = 1; // 072871: Catalog - Select Class pop up - Time zone pop up window is breaking
				if(result[c]['session_details'].length > 0) {
					var sDay = result[c]['session_details'][0]['start_date'];
					var sTime = result[c]['session_details'][0]['start_time'];
					var sTimeForm = result[c]['session_details'][0]['start_form'];
                    
						if(result[c]['session_details'].length > 1) {
							sessLenEnd = result[c]['session_details'].length-1;
							var eDay = result[c]['session_details'][sessLenEnd]['start_date'];
							var eTime = result[c]['session_details'][sessLenEnd]['end_time'];
							var eTimeForm = result[c]['session_details'][sessLenEnd]['end_form'];

							startDate = sDay +' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +eDay+ ' ' +eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
							startDateVtip =  sDay +' '+ sTime+' '+sTimeForm+' ' +Drupal.t('LBL621')+ ' ' +eDay+ ' ' +eTime+' '+eTimeForm;
						}else {

							var eTime = result[c]['session_details'][0]['end_time'];
							var eTimeForm = result[c]['session_details'][0]['end_form'];

							startDate = sDay +' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
							startDateVtip =  sDay +' '+ sTime+' '+sTimeForm+' ' +Drupal.t('LBL621')+ ' ' +eTime+' '+eTimeForm;
						}

                     if(result[c]['delivery_type_code']=='lrn_cls_dty_ilt'){
                    	 var sDay = result[c]['session_details'][0]['start_date'];
     					var sTime = result[c]['session_details'][0]['start_time'];
     					var sTimeForm = result[c]['session_details'][0]['start_form'];
     					var usDay = result[c]['session_details'][0]['ilt_start_date'];
     					var usTime = result[c]['session_details'][0]['ilt_start_time'];
     					var usTimeForm = result[c]['session_details'][0]['ilt_start_form'];
     					var loc_tz = result[c]['session_details'][0]['loc_tz'];
     					var loc_tzcode = result[c]['session_details'][0]['loc_tzcode'];
     					var user_tz = result[c]['session_details'][0]['user_tz'];
     					var user_tzcode = result[c]['session_details'][0]['user_tzcode'];
     						if(result[c]['session_details'].length > 1) {
     							sessLenEnd = result[c]['session_details'].length-1;
     							var eDay = result[c]['session_details'][sessLenEnd]['start_date'];
     							var eTime = result[c]['session_details'][sessLenEnd]['end_time'];
     							var eTimeForm = result[c]['session_details'][sessLenEnd]['end_form'];
     							var uDay = result[c]['session_details'][sessLenEnd]['ilt_start_date'];
     							var uTime = result[c]['session_details'][sessLenEnd]['ilt_end_time'];
     							var uTimeForm = result[c]['session_details'][sessLenEnd]['ilt_end_form'];
                                
     							startDate = sDay+' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +eDay+ ' ' +eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+ ' ' +user_tz+' '+user_tzcode+'';
     							startDateVtip =  sDay +' '+ sTime+' '+sTimeForm+' ' +Drupal.t('LBL621')+ ' ' +eDay+ ' ' +eTime+' '+eTimeForm+ ' ' +user_tz+' '+user_tzcode;
     							sessstartDate = usDay +' '+ usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +uDay+ ' ' +uTime+' <span class="time-zone-text">'+uTimeForm+'</span>'+ ' ' +loc_tz+' '+loc_tzcode+'';
     							sessioncount = 2;
     						}else {

     							var eTime = result[c]['session_details'][0]['end_time'];
     							var eTimeForm = result[c]['session_details'][0]['end_form'];
     							var uTime = result[c]['session_details'][0]['ilt_end_time'];
     							var uTimeForm = result[c]['session_details'][0]['ilt_end_form'];

     							startDate = sDay +' '+ sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+ ' ' +user_tz+' '+user_tzcode+'';
     							startDateVtip =  sDay +' '+ sTime+' '+sTimeForm+' ' +Drupal.t('LBL621')+ ' ' +eTime+' '+eTimeForm+ ' ' +user_tz+' '+user_tzcode;
     							sessstartDate = usDay +' '+ usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+ ' ' +Drupal.t('LBL621')+ ' ' +uTime+' <span class="time-zone-text">'+uTimeForm+'</span>'+ ' ' +loc_tz+' '+loc_tzcode+'';
     							
     						}
                     }

					var inc = 0;
					var passParams = "[";

					var sessLen = result[c]['session_details'].length;
					// console.log('session_details', result[c]['session_details']);
					$(result[c]['session_details']).each(function(){
						// console.log('session_details', $(this));
						inc=inc+1;
						passParams += "{";
						 passParams += "'sessionTitle':'"+(($(this).attr("session_title")) ? $(this).attr("session_title") : '')+"',";
						 passParams += "'sessionDay':'"+$(this).attr("session_day")+"',";
						 passParams += "'sessionSDate':'"+$(this).attr("start_date")+"',";
						 passParams += "'start_time':'"+$(this).attr("start_time")+"',";
						 passParams += "'end_time':'"+$(this).attr("end_time")+"',";
						 passParams += "'sessionSDayForm':'"+$(this).attr("start_form")+"',";
						 if(deliveryTypeCode == 'lrn_cls_dty_ilt'){
							 passParams += "'session_id':'"+$(this).attr("session_id")+"',";
							 passParams += "'iltsessionDay':'"+$(this).attr("ilt_session_day")+"',";
							 passParams += "'iltsessionSDate':'"+$(this).attr("ilt_start_date")+"',";
							 passParams += "'iltstart_time':'"+$(this).attr("ilt_start_time")+"',";
							 passParams += "'iltend_time':'"+$(this).attr("ilt_end_time")+"',";
							 passParams += "'iltsessionSDayForm':'"+$(this).attr("ilt_start_form")+"',";
							 passParams += "'loc_tz':'"+$(this).attr("loc_tz")+"',";
							 passParams += "'loc_tzcode':'"+$(this).attr("loc_tzcode")+"',";
							 passParams += "'user_tz':'"+$(this).attr("user_tz")+"',";
							 passParams += "'user_tzcode':'"+$(this).attr("user_tzcode")+"',";
							 passParams += "'iltsessionEDayForm':'"+$(this).attr("ilt_end_form")+"',";
						}
						 passParams += "'sessionEDayForm':'"+$(this).attr("end_form")+"',";
						 passParams += "'sessionInstructorName':'"+$(this).attr("sessionInstructorName")+"'";
						 passParams += "}";
						 if(inc < sessLen) {
							 passParams += ",";
						 }

					});
					passParams += "]";

				}else{
					var passParams="''";
				}
				var LocationNameArg = ((locationName != null) ? ((unescape(locationName).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : locationName);
				var LocationAdd1Arg = ((locationAddr1!= null) ? ((unescape(locationAddr1).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : locationAddr1);
				var LocationAdd2Arg = ((locationAddr2 != null) ? ((unescape(locationAddr2).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : locationAddr2);
				var LocationcityArg = ((locationCity != null) ? ((unescape(locationCity).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : locationCity);
				var param = "data={'classId':'"+classId+"','classCode':'"+escape(classCode)+"','classDesc':'"+escape(classDesc)+"','language':'"+language+"'," +
			    		"'sessionId':'"+sessionId+"','deliveryTypeCode':'"+deliveryTypeCode+"','countryName':'"+countryName+"','locationName':'"+LocationNameArg+"','locationAddr1':'"+LocationAdd1Arg+"','locationAddr2':'"+LocationAdd2Arg+"'," +
	    				"'locationCity':'"+LocationcityArg+"','locationState':'"+locationState+"','locationZip':'"+locationZip+"'," +
   						"'sessionDetails':"+passParams+"}";


	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-class-'+classId+'">';
	 			if(last_comp_enrolledId!=''){
					if(last_comp_enrolledId == enrolledId.enrolled_id){
						checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"': checkedValue;
					}else{
						checkedValue = '';
					}
				}else{
					checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"': checkedValue;
				}
				if(fromPage == "MyTeam") {
					if(deliveryTypeCode =='lrn_cls_dty_vcl' || deliveryTypeCode =='lrn_cls_dty_ilt')  {
						rhtml += '<td class="course-sub-class-detail"><table border="0" width="100%" cellpadding="0" cellspacing="0"><tr><td valign="top" class="course-detail-col1">';
						rhtml += '<a id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="title_close" onclick=\''+objStr+'.accordionForClassView('+classId+', "'+fromPage+'");\'>&nbsp;</a>';
						rhtml += '<div class="tp-register-iltvcl-select-class limit-title-row"><span class="lp-reg-class-title limit-title limit-title-mulcls-title vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
						rhtml += classTitle; /*titleRestricted(classTitle,30);*/
						rhtml += '</span></div></td>';
						if(deliveryTypeCode =='lrn_cls_dty_ilt' && loc_tz!=user_tz ){
						rhtml += '<td valign="top" class="course-detail-col2 vtip" title="'+sDay+' '+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+' '+user_tz+' '+user_tzcode+'"><div><span style="display: inline-block;">'+titleRestrictionFadeoutImage(startDate,'exp-sp-lnrenrollment-timezone', 20)+'';
						}
						else{
							rhtml += '<td valign="top" class="course-detail-col2"><span>'+startDate+'';
						}
						rhtml += '</span>'; 
						if(deliveryTypeCode =='lrn_cls_dty_ilt' && loc_tz!=user_tz ){
							rhtml += qtip_popup_paint(classId,sessstartDate,sessioncount); 
						}
						rhtml += '</div></td>';

						//rhtml += '<input class="rButtonclass" type="radio" id="'+courseId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'">';
						//checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"': checkedValue;
						rhtml += '<td valign="top" class="course-detail-col3"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></td>';
					} else {
						rhtml += '<td class="course-sub-class-detail"><table border="0" width="100%" cellpadding="0" cellspacing="0"><tr><td valign="top" colspan="2" class="course-detail-col1-new">';
						rhtml += '<a id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="title_close" onclick=\''+objStr+'.accordionForClassView('+classId+', "'+fromPage+'");\'>&nbsp;</a>';
						rhtml += '<div class="tp-register-select-class limit-title-row"><span class="lp-reg-class-title limit-title limit-title-mulcls-title vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
						rhtml += classTitle; /*titleRestricted(classTitle,30);*/
						rhtml += '</span></div></td>';
						//rhtml += '<td valign="top" class="course-detail-col2"><span>'+startDate+'</span></td>';
						//rhtml += '<input class="rButtonclass" type="radio" id="'+courseId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'">';
						//checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"': checkedValue;
						rhtml += '<td valign="top" class="course-detail-col3"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></td>';
					}
					rhtml += '</tr></table>';
					rhtml += '<div class="course-delivery-info item-title-code cls-class-title-code">';
					//rhtml += '<span title="'+unescape(classCode)+'" class="vtip">'+titleRestrictionFadeoutImage(unescape(classCode),'tp-view-class-code')+'</span>';
					rhtml += '<div class="line-item-container float-left myteam"><span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(classCode),'tp-view-class-code')+'</span></div>';
					rhtml += '<div class="line-item-container float-left myteam">';
					rhtml += '<span class="pipeline">|</span>';
				} else {
					rhtml += '<td class="course-sub-class-detail"><div class="course-class-content-wrapper"><div class="class-list padtp5" id="class-container-loader-'+classId+'"><div class="class_list_container"><div class="content-row-container id="content-row-container-class-'+classId+'" '+deliveryTypeCode+'">';
				}
				var row_det_type;
				var iconClass;
				if (deliveryTypeCode == '' || deliveryTypeCode == '-') {
					return '<span>&nbsp;</span>';
				}
				else if(deliveryTypeCode=='lrn_cls_dty_wbt'){
					row_det_type = Drupal.t("Web-based");
					iconClass = 'wbt-icon';
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_vcl'){
		        	row_det_type = Drupal.t("Virtual Class");
					iconClass = 'vcl-icon';
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_ilt'){
		        	row_det_type = Drupal.t("Classroom");
					iconClass = 'ilt-icon';
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_vod'){
		        	row_det_type = Drupal.t("Video");
					iconClass = 'vod-icon';
		        }
				if(fromPage == "MyTeam") {
					rhtml += '<span>'+row_det_type+'</span>';
					rhtml += '</div>';
					if(deliveryTypeCode=='lrn_cls_dty_ilt') {
						rhtml += '<div class="line-item-container float-left myteam">';
						rhtml += '<span class="pipeline">|</span>';
						rhtml += '<span>'+unescape(locationName)+'</span>';
						rhtml += '</div>';
					}
					if(deliveryTypeCode=='lrn_cls_dty_ilt' || deliveryTypeCode=='lrn_cls_dty_vcl') {
						if(waitList > 0) {
							rhtml += '<div class="line-item-container float-left myteam">';
							rhtml += '<span class="pipeline">|</span>';
							if(waitList==1)
								rhtml += '<span>'+waitList+" "+Drupal.t("LBL126")+'</span>';
							else
								rhtml += '<span>'+waitList+" "+Drupal.t("LBL127")+'</span>';
							rhtml += '</div>';
						}else if(waitList == 0 && availableSeats==0){
							rhtml += '<div class="line-item-container float-left myteam">';
							rhtml += '<span class="pipeline">|</span>';
							rhtml += '<span>'+Drupal.t("LBL106")+" "+Drupal.t("LBL046")+'</span>';
							rhtml += '</div>';
						}else{
							rhtml += '<div class="line-item-container float-left myteam">';
							rhtml += '<span class="pipeline">|</span>';
							if(availableSeats==1)
								rhtml += '<span>'+availableSeats+" "+Drupal.t("LBL052")+'</span>';
							else
								rhtml += '<span>'+availableSeats+" "+Drupal.t("LBL053")+'</span>';
							rhtml += '</div>';
						}
					}
					if(enrolledId.comp_status == 'lrn_crs_cmp_cmp' || enrolledId.comp_status == 'lrn_crs_cmp_enr') {
						rhtml += '<div class="line-item-container float-left myteam">';
						rhtml += '<span class="pipeline">|</span>';
						var regClassStatus = (enrolledId.comp_status == 'lrn_crs_cmp_cmp') ? Drupal.t("Completed") : Drupal.t("Registered");
						rhtml += '<span>'+regClassStatus+'</span>';
						rhtml += '</div>';
					}
					rhtml += '</div>';
					rhtml += '</td>';
				} else {	// else of fromPage == "MyTeam"
					var classIcon = '<div class="content-icon-container"><div title="'+row_det_type+'" class="content-icon '+iconClass+' vtip"></div></div>';
					// console.log('classIcon', classIcon);
					rhtml += classIcon;
					var classDetail = '<div class="content-detail-container"><div class="content-title vtip limit-title-row"><span title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" class="limit-title lmt-lrp-crs-cls-title vtip">'+classTitle+'</span></div>';
					// console.log('classDetail', classDetail);
					var classInfo = '<div class="course-delivery-info item-title-code content-detail-code">';
					classInfo += '<div class="line-item-container float-left" ><span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(classCode),'tp-view-class-code')+'</span></div>';
					// classInfo += '<span class="pipeline">|</span>';
					// classInfo += '<span>'+row_det_type+'</span>';
					if(deliveryTypeCode=='lrn_cls_dty_ilt') {
						classInfo += '<div class="line-item-container float-left" >';
						classInfo += '<span class="pipeline">|</span>';
						classInfo += '<span class="vtip" title="'+unescape(locationName).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(locationName),'tp-view-location-selectbox')+'</span>';
						classInfo += '</div>';
					}
					if(deliveryTypeCode=='lrn_cls_dty_ilt' || deliveryTypeCode=='lrn_cls_dty_vcl') {
						if(waitList > 0) {
							classInfo += '<div class="line-item-container float-left" >';
							classInfo += '<span class="pipeline">|</span>';
							if(waitList==1) {
								classInfo += '<span>'+waitList+" "+Drupal.t("LBL126")+'</span>';
							} else {
								classInfo += '<span>'+waitList+" "+Drupal.t("LBL127")+'</span>';
							}
							classInfo += '</div>';
						} else if(waitList == 0 && availableSeats==0) {
							classInfo += '<div class="line-item-container float-left" >';
							classInfo += '<span class="pipeline">|</span>';
							classInfo += '<span>'+Drupal.t("LBL106")+" "+Drupal.t("LBL046")+'</span>';
							classInfo += '</div>';
						} else {
							classInfo += '<div class="line-item-container float-left" >';
							classInfo += '<span class="pipeline">|</span>';
							var seats_left_dis = availableSeats+" "+Drupal.t("LBL052");
							if(availableSeats==1) {
								classInfo += '<span class="vtip" title="'+unescape(availableSeats).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(seats_left_dis),'tp-view-class-seatsleft')+'</span>';
							} else {
								classInfo += '<span class="vtip" title="'+unescape(availableSeats).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(seats_left_dis),'tp-view-class-seatsleft')+'</span>';
							}
							classInfo += '</div>';
						}
					}
					if(enrolledId.comp_status == 'lrn_crs_cmp_cmp' || enrolledId.comp_status == 'lrn_crs_cmp_enr') {
						classInfo += '<div class="line-item-container float-left" >';
						classInfo += '<span class="pipeline">|</span>';
						var regClassStatus = (enrolledId.comp_status == 'lrn_crs_cmp_cmp') ? Drupal.t("Completed") : Drupal.t("Registered");
						classInfo += '<span>'+regClassStatus+'</span>';
						classInfo += '</div>';
					}
					var sessionInfo = '';
					var classRegisterStatus = '';
					if(deliveryTypeCode =='lrn_cls_dty_vcl' || deliveryTypeCode =='lrn_cls_dty_ilt') {
						if(deliveryTypeCode =='lrn_cls_dty_ilt' && loc_tz!=user_tz ) {
							sessionInfo += '<div class="float-left" >';
							sessionInfo += '<span valign="top" class="vakku course-detail-col2 clsWithTz vtip" title="'+sDay+' '+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+' '+user_tz+' '+user_tzcode+'"><div><span style="display: inline-block;">'+titleRestrictionFadeoutImage(startDate,'exp-sp-lnrenrollment-timezone', 20)+'';
							sessionInfo += '</span>';
							sessionInfo += qtip_popup_paint(classId,sessstartDate,sessioncount);					
							sessionInfo += '</div></span>';	
							sessionInfo += '</div>';
						} else {
							sessionInfo += '<div class="float-left" >';
							sessionInfo += '<span valign="top" class="vakku course-detail-col2 vtip" title="'+startDateVtip+'">'+titleRestrictionFadeoutImage(startDate,'exp-sp-lnrenrollment-timezone', 20)+'';
							sessionInfo += '</span>'; 		
							sessionInfo += '</div>';
						}
						classRegisterStatus += '<div valign="top" class="content-action-container"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></div>';
					} else {
						classRegisterStatus += '<div valign="top" class="content-action-container"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></div>';
					}
					if(sessionInfo != '') {
						classInfo += '<div class="line-item-container float-left" >';
						classInfo += '<span class="pipeline">|</span>';
						classInfo += sessionInfo;
						classInfo += '</div>';
					}
					classInfo += '</div>';
					classDetail += classInfo;
					classDetail += '<div class="limit-desc-row content-description padbt5 '+deliveryTypeCode+'"><div class="limit-desc lmt-lrp-crs-cls-desc vtip">'+classDesc+'</div></div>';
					classDetail += '</div>';
					rhtml += classDetail;
					rhtml += classRegisterStatus;
					rhtml += '</div>';
					var showMoreLess = '<div class="class_list_content"><div class="class-content-content-wrapper"><div id="class_content_more_'+classId+'" class="class-content-more padbt5"><div id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="show_more_less" onclick=\''+objStr+'.accordionForClassView('+classId+');\'>'+Drupal.t("LBL713")+'</div></div></div></div>';
					var moreClassInfo = '<div class="class-content-wrapper class-content-content-wrapper border-box-cnt-tp '+deliveryTypeCode+'"><div class="content-list"><div id="'+classId+'ClassSubGrid" style="display: none;"><div id="class-details-'+classId+'" class="class-level"></div></div></div></div>';
					rhtml += moreClassInfo;
					rhtml += showMoreLess;
					rhtml += '</div></div></div></td>';				
					rhtml += '</div></td>';
				}
				rhtml += '</tr>';
			}

		} else {
			rhtml += '<tr>';
			rhtml += '<td class="no-item-found">'+SMARTPORTAL.t("There are no classes under this course")+'</td>';
			rhtml += '</tr>';
		}
	  rhtml += '</table>';
	  rhtml += '</div>';
	 // alert('final output '+ rhtml)
	  return rhtml;
	  vtip();
	  //$("#course-details-"+data.CourseId).html(rhtml);
	  //$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
	 } catch(e) {
			// to do
			// console.log(e, e.stack);
		}
	},


	accordionForClassView : function(classId, fromPage) {
		try{
		var currTr = classId;

		if(!document.getElementById(currTr+"ClassSubGrid")) {
			if(fromPage == "MyTeam") {
				$("#lp-class-"+currTr).after("<tr id='"+currTr+"ClassSubGrid'><td colspan='4'><div id='class-details-"+currTr+"' class='class-level'></div></td></tr>");
				$("#lp-class-accodion-"+currTr).removeClass("title_close");
				$("#lp-class-accodion-"+currTr).addClass("title_open");
				$("#"+currTr+"ClassSubGrid").find(".class-level").css("border-bottom","1px solid #cccccc");
			} else {
				$("#lp-class-"+currTr).find('#content-row-container-class-'+currTr).after("<div class='class-content-wrapper border-box-cnt-tp'><div class='content-list'><div id='"+currTr+"ClassSubGrid'><div id='class-details-"+currTr+"' class='class-level'></div></div></div></div>");
				$("#lp-class-accodion-"+currTr).text(Drupal.t("LBL3042"));
			}
			
			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
			$("#lp-class-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ClassSubGrid").slideDown(1000);
			$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","0px");
			//$("#"+currTr+"ClassSubGrid").find(".class-level").css("margin-left","16px");
			$('.class-details-info > tbody > tr:last > td').find('.class-level').css('border-bottom','0px none');
		} else {
			var clickStyle = $("#"+currTr+"ClassSubGrid").css("display");
			var getWidth=$(".course-class-content-wrapper").find(".content-row-container").width();
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
    			$("#"+currTr+"ClassSubGrid").slideDown(1000);
				if(fromPage == "MyTeam") {
					$("#lp-class-accodion-"+currTr).removeClass("title_close");
					$("#lp-class-accodion-"+currTr).addClass("title_open");
					$("#"+currTr+"ClassSubGrid").find(".class-level").css("border-bottom","1px solid #cccccc");
				} else {
					$("#lp-class-accodion-"+currTr).text(Drupal.t("LBL3042"));
					
				}
				$("#lp-class-"+currTr).removeClass("ui-widget-content");
				$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","0px");
				//$("#"+currTr+"ClassSubGrid").find(".class-level").css("margin-left","16px");
				$('.class-details-info > tbody > tr:last > td').find('.class-level').css('border-bottom','0px none');
				//$("#lp-class-"+currTr).find(".course-class-content-wrapper .content-detail-container").removeClass("expand-content-detail-container");
				//$("#lp-class-"+currTr).find(".course-class-content-wrapper .content-detail-container").addClass("expand-content-detail-container");
				if(Drupal.settings.widget.widgetCallback==true){
				$("#lp-class-accodion-"+currTr).parents("tr").find(".course-class-content-wrapper .class_list_container > div").css("width",getWidth);
				}
    		} else {
    			$("#"+currTr+"ClassSubGrid").hide();//css("display","none");
    			$("#"+currTr+"ClassSubGrid").slideUp(1000);
				if(fromPage == "MyTeam") {
					$("#lp-class-accodion-"+currTr).removeClass("title_open");
					$("#lp-class-accodion-"+currTr).addClass("title_close");
				} else {
					$("#lp-class-accodion-"+currTr).text(Drupal.t("LBL713"));
				}
				$("#lp-class-"+currTr).removeClass("ui-widget-content");
				$("#lp-class-"+currTr).addClass("ui-widget-content");
				if(fromPage == "MyTeam") {
					$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","1px solid #cccccc");
					$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
				}
				//$("#lnr-tainingplan-register .course-class-content-wrapper .content-row-container").find(".content-detail-container").removeClass("expand-content-detail-container");
    		    //$("#lnr-tainingplan-register .course-class-content-wrapper .content-row-container").find(".content-detail-container").addClass("collapse-content-detail-container");
    		    //$("#lp-class-"+currTr).find(".course-class-content-wrapper .content-detail-container").removeClass("expand-content-detail-container");
    		   // $("#lp-class-accodion-"+currTr).parents("tr").find(".course-class-content-wrapper .content-detail-container").removeAttr("style");
				if(Drupal.settings.widget.widgetCallback==true){
    		     $("#lp-class-accodion-"+currTr).parents("tr").find(".course-class-content-wrapper .class_list_container > div").css("width",getWidth);
				}
    			}
		}


		var data = eval($("#lp-class-accodion-"+currTr).attr("data"));

		var classDetSec = this.paintTPClassSessionDetails(data, fromPage);
		if(fromPage == "MyTeam") {
			$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
			$('.class-details-info > tbody > tr.set-height:last td').find('.course-delivery-info').css('border-bottom','0px');
			$('.class-details-info > tbody > tr:last > td').find('.cls-description').css('border-bottom','0px none');
		}
		/*-- #34270 scroll pane for the dynamic contents in team assign learn popup --*/
		if($("#my-team-dialog #gview_tbl-paintCatalogContentResults").length) {
			$("body").data("learningcore").refreshJScrollPane("#my-team-dialog #gview_tbl-paintCatalogContentResults");
		}
		$('.lmt-lrp-crs-cls-title').trunk8(trunk8.class_detail_title);
		$('.lmt-lrp-crs-cls-desc').trunk8(trunk8.lp_detail_course_desc);
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		
		//$(".lnr-tainingplan-register").jScrollPane();
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
	},


	paintTPClassSessionDetails : function(data, fromPage){
		try{
			// console.log(data);
		var oHtml = '';
		if(fromPage == "MyTeam") {
			oHtml += "<table cellpadding='2' class='sub-course-class' cellspacing='2' width='100%'>";
		} else {
			oHtml += '<div class="class-content-wrapper border-box-cnt-tp">';
		}
		if(data.classCode && fromPage == "MyTeam") {
			oHtml += "<tr><td class=''><div class='viewlrn-cls-lbl'>"+Drupal.t("LBL263")+":</div><div class='viewlrn-cls-lbl-val vtip' title='"+unescape(data.classCode)+"'> "+titleRestrictionFadeoutImage(unescape(data.classCode),'assignlearn-tp-cls-title')+"</div></td></tr>";
		}
		if(fromPage == "MyTeam") {
			oHtml += "<tr><td><div class='viewlrn-cls-lbl'>"+Drupal.t("LBL229")+":</div><div class='viewlrn-cls-lbl-val'> "+unescape(data.classDesc)+"</div></td></tr> <tr><td class='cls-description'>";
		} else {
			oHtml += '<div class="content-list"><div class="session-details-warpper">';
		}
		if(data.sessionDetails.length>0) {
			if(fromPage == "MyTeam") {
				oHtml += '<div class="enroll-session-details viewlrn-cls-lbl">'+Drupal.t("LBL670")+':</div>';
			} else {
				oHtml += '<div class="sub-section-title padbt5">'+Drupal.t("LBL277")+'</div>';
			}
			var inc = 1;
			$(data.sessionDetails).each(function(){
				if($(this).attr("sessionTitle")) {
					var sesionsH = ($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;";
					var sesionsHead = sesionsH;
					if(fromPage == "MyTeam") {
						oHtml += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")).replace(/"/g, '&quot;') : "&nbsp;")+'">'+titleRestrictionFadeoutImage(unescape(sesionsHead),'assignlearn-tp-cls-session-title')+'</div>';
						oHtml += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
						if(data.deliveryTypeCode == 'lrn_cls_dty_ilt') {
							oHtml += '<div class="sessDate">'+$(this).attr("sessionSDate")+ " " +$(this).attr("start_time")+ " <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("end_time")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>"+$(this).attr("user_tz")+" "+$(this).attr("user_tzcode")+"";
							var locdetails ='<div class="locsessDate">'+$(this).attr("iltsessionSDate")+ " " +$(this).attr("iltstart_time")+ " <span class='time-zone-text'>"+$(this).attr("iltsessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("iltend_time")+" <span class='time-zone-text'>"+$(this).attr("iltsessionEDayForm")+"</span>"+$(this).attr("loc_tz")+" "+$(this).attr("loc_tzcode")+"</div>";
							// console.log($(this).attr("user_tz"));
							// console.log($(this).attr("loc_tz"));
							if($(this).attr("user_tz")!= $(this).attr("loc_tz")){
								oHtml += qtip_popup_paint($(this).attr("session_id"),locdetails); 
							}
							oHtml +='</div></div>';
					   }
					   else{
						oHtml += '<div class="sessDate">'+$(this).attr("sessionSDate")+ " " +$(this).attr("start_time")+ " <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("end_time")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
					   }
					} else {	// end of if(fromPage == "MyTeam")
					var sessionDetail = '<div class="session-detail-block padbt5 sessionDet">';
					var sessionName = '<div class="session-name session-row">';
					sessionName += '<span class="sess-attr-name">'+Drupal.t("LBL107")+': </span>';
					sessionName += '<div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")).replace(/"/g, '&quot;') : "&nbsp;")+'">'+titleRestrictionFadeoutImage(unescape(sesionsHead),'assignlearn-tp-cls-session-title')+'</div>';
					sessionName += '</div>';
					var sessionDate = '<div class="session-date session-row">';
					sessionDate += '<span class="sess-attr-name">'+Drupal.t("LBL042")+'</span>';
					sessionDate += '<span title="" class="sess-attr-val">';

					
					sessionDate += $(this).attr("sessionDay") + ' ';
			       if(data.deliveryTypeCode == 'lrn_cls_dty_ilt'){
					sessionDate += $(this).attr("sessionSDate")+ " " +$(this).attr("start_time")+ " <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("end_time")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>"+$(this).attr("user_tz")+" "+$(this).attr("user_tzcode")+"";
                    var locdetails ='<div class="locsessDate">'+$(this).attr("iltsessionSDate")+ " " +$(this).attr("iltstart_time")+ " <span class='time-zone-text'>"+$(this).attr("iltsessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("iltend_time")+" <span class='time-zone-text'>"+$(this).attr("iltsessionEDayForm")+"</span>"+$(this).attr("loc_tz")+" "+$(this).attr("loc_tzcode")+"</div>";
					// console.log($(this).attr("user_tz"));
					// console.log($(this).attr("loc_tz"));   

					if($(this).attr("user_tz")!= $(this).attr("loc_tz")){
						sessionDate += qtip_popup_paint($(this).attr("session_id"), locdetails); 
					}
					// sessionDate +='</div></div>';
			       }
			       else{
					sessionDate += $(this).attr("sessionSDate")+ " " +$(this).attr("start_time")+ " <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' ' + Drupal.t('LBL621') + ' ' +$(this).attr("end_time")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>";
			       }
				   sessionDate += '</span></div>';
				   sessionDetail += sessionName;
				   sessionDetail += sessionDate;
				  
				   var sessionInstructor = '';
				   if($(this).attr("sessionInstructorName") != null) {
						var instrName = ($(this).attr("sessionInstructorName") == 'null' ? '' : $(this).attr("sessionInstructorName"));
						sessionInstructor += '<div class="session-instructor session-row">';
						sessionInstructor += '<span class="sess-attr-name">'+Drupal.t('Instructor')+': </span>';
						sessionInstructor += '<span class="sess-attr-val vtip" title="'+unescape(instrName).replace(/"/g, '&quot;')+'">';
						// sessionInstructor += '<div class="fade-out-title-container class-detail-session-instructor-fadeout-container" style=""><span class="title-lengthy-text" style="">'+$(this).attr("sessionInstructorName")+'</span></div>';
						sessionInstructor += titleRestrictionFadeoutImage(unescape(instrName).replace(/"/g, '&quot;'), 'class-detail-session-instructor-fadeout-container');
						sessionInstructor += '</span></div>';
					
					}
					sessionDetail += sessionInstructor;
					sessionDetail += '</div>';
					oHtml += sessionDetail;
					// console.log('sessionInstructor', sessionInstructor);
					}
					inc++;
				}
			});
		}
		if(data.language) {
			if(fromPage == "MyTeam") {
				oHtml += "<div class='cls-lang'><div class='viewlrn-cls-lbl'>"+Drupal.t("LBL038")+":</div><div class='viewlrn-cls-lbl-val'> "+Drupal.t(data.language)+"</div></div>";
			} else {
				var locationInfo = '<div class="session-location-warpper"><div class="sub-section-title padbt5">'+Drupal.t("LBL038")+'</div><div class="session-location-block "><div class="location-address padbt10">'+Drupal.t(data.language)+'</div></div></div>';
				oHtml += locationInfo;
			}
		}

		if(data.deliveryTypeCode=='lrn_cls_dty_ilt') {
			var LocationName 	= data.locationName;
			if(LocationName != '') {
				if(fromPage == "MyTeam") {
					oHtml +='<table class="enroll-loc-details"><tr><td class="enroll-location-head" valign="top"><div class="enroll-loc-head viewlrn-cls-lbl">'+Drupal.t("Location")+':</div><div class="viewlrn-cls-lbl-val">';
					oHtml += "<div class='enroll-location-text'>"+unescape(LocationName)+"</div>";
					if(data.locationAddr1 !='' && data.locationAddr1 != null && data.locationAddr1 != 'null') {
						oHtml += "<div class='enroll-location-text'>"+data.locationAddr1+"</div>";
					}
					if(data.locationAddr2 !='' && data.locationAddr2 != null && data.locationAddr2 != 'null') {
						oHtml += "<div class='enroll-location-text'>"+data.locationAddr2+"</div>";
					}
					if(data.locationCity !='' && data.locationCity != null && data.locationCity != 'null') {
						oHtml += "<div class='enroll-location-text'>"+data.locationCity;
						if (data.locationState == '' && data.locationState != null && data.locationState != 'null') {
							oHtml += "<br />";
						}
					}
					if(data.locationState !='' && data.locationState != null && data.locationState != 'null') {
						if (data.locationCity != '' && data.locationCity != null && data.locationCity != 'null') {
						   oHtml += ", ";
						}
						oHtml += "<div class='enroll-location-text'>"+data.locationState+"</div>";
					}
					oHtml += "<div class='enroll-location-text'>"+data.countryName;
					if(data.locationZip !='' && data.locationZip != null && data.locationZip != 'null'){
						oHtml += " - "+data.locationZip;
					}
					oHtml += "</div>";
					oHtml += "</td></tr></table>";
				} else {
					var locationInfo = '';
					locationInfo += '<div class="session-location-warpper">';
					locationInfo += '<div class="sub-section-title padbt5">'+Drupal.t("Location")+': </div>';
					locationInfo += '<div class="session-location-block"><div class="location-address padbt10">';
					locationInfo += unescape(LocationName);
					if(data.locationAddr1 !='' && data.locationAddr1 != null && data.locationAddr1 != 'null') {
						locationInfo += ", "+data.locationAddr1;
					}
					if(data.locationAddr2 !='' && data.locationAddr2 != null && data.locationAddr2 != 'null') {
						locationInfo += ", "+data.locationAddr2;
					}
					if(data.locationCity !='' && data.locationCity != null && data.locationCity != 'null') {
						locationInfo += ", "+data.locationCity;
					}
					if(data.locationState !='' && data.locationState != null && data.locationState != 'null') {
						locationInfo += ", "+data.locationState;
					}
					if(data.locationZip !='' && data.locationZip != null && data.locationZip != 'null'){
						locationInfo += " - "+data.locationZip;
					}
					locationInfo += ", "+data.countryName;
					locationInfo += "</div></div></div>";
					// console.log('locationInfo', locationInfo);
					oHtml += locationInfo;
				}
			}
		}
		if(fromPage == "MyTeam") {
			oHtml += "</td></tr></table>";
		} else {
			oHtml += "</div></div></div>";
		}
		$("#class-details-"+data.classId).html(oHtml);
		$("body").data("learningcore").refreshJScrollPane(".lnr-tainingplan-register");
		//$(".lnr-tainingplan-register").jScrollPane();
		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
		vtip();
	},

	displayEquvPopup : function(widgetId,userId, courseId, classId,result,type,selectValues){
		try{
	    $('#equvMsg-wizard').remove();
	    var html = '';
	    html+='<div id="equvMsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	   // html+='<tr>';
	    if(result.length == 1)
		html+='<td align="center" class="light-gray-color">'+Drupal.t(result[0]['errormsg']['msg1'])+'</td></tr>';
	    else
	    html+='<td align="center" class="light-gray-color">'+Drupal.t(result[0]['errormsg']['msg2'])+'</td></tr>';

		for(k=0;k < result.length ; k++){
			html+= '<tr><td align="center"><i>"'+unescape(result[k]['equv_title'])+'"</i></td></tr>';
		}


		html+= '<tr><td align="center" class="light-gray-color"><span>'+Drupal.t(result[0]['errormsg']['msg3'])+'</span></td></tr>';
	    //html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove();};
	    closeButt[Drupal.t('Yes')]=function(){ $("body").data("learningcore").callRegisterClass(widgetId,userId, courseId, classId,type,selectValues); $(this).dialog('destroy');$(this).dialog('close');$('#equvMsg-wizard').remove(); };

	    $("#equvMsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('LBL247'),
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
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ($.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    }
	    $("#equvMsg-wizard").show();
	 	if(this.currTheme == "expertusoneV2"){
	 		//changeDialogPopUI();
	 		 $('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
		 	   $('#equvMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
		       $('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		       $('#select-class-equalence-dialog #equvMsg-wizard').css('height','270px');
			   $('#select-class-equalence-dialog #equvMsg-wizard').jScrollPane();
			   changeChildDialogPopUI('select-class-equalence-dialog');
			   $('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
			   $('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			   }
			}
		 	else {
		 		$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			 	$('#equvMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
				changeChildDialogPopUI('select-class-equalence-dialog');
				$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			   }
	    $("#equvMsg-wizard").show();
		$('.ui-dialog-titlebar-close, .removebutton').click(function(){
			$('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#equvMsg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	},

	callEquivalencePopup : function(widgetId,userId, courseId, classId, waitList, enrolledStatus,selectValues,courseLevelCnt){
		try{
		if(typeof(selectValues)==='undefined') selectValues = '';
		if(typeof(courseLevelCnt)==='undefined') courseLevelCnt = '';
		var obj = this;
		var loaderObj = widgetId;
		var loaderFlg = true;
		this.createLoader(loaderObj);
		var userId = this.getLearnerId();
		if(!userId) {
			this.callMessageWindow('registertitle',"Please sign in to proceed.");
		}else{
				url = obj.constructUrl("ajax/learningcore/check-equivalence/" + userId + '/' + courseId + '/' + classId);
				var IN_FLIGHT = 'requestinflight';
				var $ajaxcall;
				$ajaxcall = $( this );
               if( ! $ajaxcall.data( IN_FLIGHT ) ){
					$ajaxcall.data( IN_FLIGHT, true );
					$.ajax({
						type: "POST",
						url: url,
						//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
						success: function(result){
							var	compClassPopup = '';
							var regtestcount = (result.length >0 && result[0].recurringcount) ? result[0].recurringcount : 'N';
							var class_cnt = (result.length >0 && result[0].class_cnt) ? result[0].class_cnt : 0;
							var isCompliance = (result.length >0 && result[0].isCompliance) ? result[0].isCompliance : '';
							var comp_inc = (result.length >0 && result[0].comp_inc) ? result[0].comp_inc : '';
							
							
							if(result.length >0 && result[0].enrollexist){
								compClassPopup = 'Y';
							}
							var tpcompclass = (result.length > 0 && result[0].tpenrollexist) ? 'Y' : '';

							$ajaxcall.data( IN_FLIGHT, true );
							if(result.length > 0 && result[0].enrollexist != 'Y' && result[0].tpenrollexist != 'Y'){
								obj.displayEquvPopup(widgetId,userId, courseId, classId,result,waitList,selectValues);
							}else{
								if((enrolledStatus == 'lrn_crs_reg_cnf' && courseLevelCnt != 'Y' && regtestcount == "Y") || (enrolledStatus=='' && isCompliance==1 && comp_inc=='lrn_crs_cmp_inc')){
									if(class_cnt > 1 && (selectValues != 'fromClasslvl' && selectValues  != '')){
										$("#lnr-catalog-search").data("lnrcatalogsearch").showSelectClass(userId, courseId); 	
									}
									else{
										regMsg = Drupal.t('MSG535') +'<br />'+ Drupal.t('MSG536');
										obj.registerConfirmation('registertitle', regMsg, widgetId, userId, courseId, classId, waitList,selectValues,compClassPopup);
									}
								}else if(tpcompclass == 'Y'){
										if(result.length > 0 && result[0].recurringcount != '' &&  result[0].recurringcount != 'undefined' && result[0].recurringcount != undefined && result[0].recurringcount == "Y"){
									    	regMsg = Drupal.t('MSG535') +'<br />'+ Drupal.t('MSG536');
											obj.registerConfirmation('registertitle', regMsg, widgetId, userId, courseId, classId, waitList,selectValues,compClassPopup);
                                        }else{
											status_msg = Drupal.t('MSG535');
                                        	obj.callMessageWindow(Drupal.t('LBL721'),status_msg);
                                        }
								}else if((compClassPopup == 'Y' || courseLevelCnt == 'Y') && selectValues != 'fromClasslvl'){
										regMsg = Drupal.t('LBL1267') +'<br />'+ Drupal.t('LBL1268');
										obj.registerConfirmation('registertitle', regMsg, widgetId, userId, courseId, classId, waitList,selectValues,compClassPopup,courseLevelCnt);
								}else{
									obj.destroyLoader(loaderObj);
									loaderFlg = false;
									obj.callRegisterClass(widgetId,userId, courseId, classId, waitList,selectValues);
									//$("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:1}]);
								}
							}
							if(loaderFlg==true)
								obj.destroyLoader(loaderObj);
						},
					    complete: function()
					    {
							$ajaxcall.data( IN_FLIGHT, false );
					    }
					});
              }
		}
		scrollBar_Refresh('catalog');
		}catch(e){
			// to do
			//console.log(e);
		}
	},
 /*
 * 0024925: Enforce equivalence in training plans
 * for this ticket we created equivalence option for training plan
 * don't delete it. Its may need for feature enhancement
 *
 */
 /*

	callEquivalencePopupTP : function(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId){
		try{

			var obj = this;
			var loaderObj = widgetId;
			var userId = this.getLearnerId();
			if(userId=='') {

				this.callMessageWindow('registertitle',"Please sign in to proceed.");
			}else{
				url = obj.constructUrl("ajax/learningcore/check-equivalence-tp/"+userId+'/'+prgId);
				//alert(url);
				var IN_FLIGHT = 'requestinflight';
				var $ajaxcall;
				$ajaxcall = $( this );
               if( ! $ajaxcall.data( IN_FLIGHT ) ){
					$ajaxcall.data( IN_FLIGHT, true );
					$.ajax({
						type: "POST",
						url: url,
						//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
						success: function(resultTP){
							var	compClassPopup = '';
							 if(resultTP.length >0 ){
								compClassPopup = 'Y';
								var title ='';
								//alert(JSON.stringify(resultTP));
								for(i=0; i<resultTP.length; i++) {
									title = title+resultTP[i].equv_title+"<br />";

								}
							}
							 regMsg = Drupal.t("MSG703TP")+'<br />'+title+Drupal.t("MSG734TP");
							$ajaxcall.data( IN_FLIGHT, true );
							if(resultTP.length > 0){
									var lengthTP = resultTP.length;
									obj.registerConfirmationTP('registertitle', regMsg,widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId,lengthTP);
								}else{
									obj.callPopupOrNotTP(widgetId, prgId,addToCart,isAdminSide,userListIfAdminSide,recertifyId);
								}
						},
					    complete: function()
					    {
							$ajaxcall.data( IN_FLIGHT, false );
					    }
					});
              }
			}
		}catch(e){
				// to do
			}
	},*/


	getRatingDetails : function(ratingdata,nodeid,average) {
		try{
		var total_average = average/20;

		var data= eval($(ratingdata).attr("data"));
		var ratingDetails = '';

		if(!document.getElementById("rating_details_popup-"+nodeid)) {


			ratingDetails += "<span class='voting-details rating-header'>Rating : "+total_average+" out of 5 stars</span>";

			var ratedArr =new Array();

		    $(data).each(function(){
		    	var rate = $(this).attr("rating_value");
				var rate_value = $(this).attr("rating_value")/20;
				ratedArr[rate_value] = $(this).attr("cnt_value");

				//var widthcnt = $(this).attr("cnt_value") * 10;
				//ratingDetails += "<span class='voting-details'> "+rate_value+" star <span class='voting-flow'><span class='voting-rating' style='width:"+widthcnt+"%'></span></span><span>("+$(this).attr("cnt_value")+")</span></span>";

			});

			for(i = 5;i >= 1;i--){
				var rateValue = (ratedArr[i]!=undefined?ratedArr[i]:0);
				var selectedCntClass = (ratedArr[i]!=undefined?'selected-cnt':'');
				var widthcnt = (ratedArr[i]!=undefined?(ratedArr[i]*5):0);
				ratingDetails += "<span class='voting-details'> "+i+" star <span class='voting-flow'><span class='voting-rating' style='width:"+widthcnt+"%'></span></span><span class='"+selectedCntClass+"'>("+rateValue+")</span></span>";
			}


			$('#rating-popup-'+nodeid).qtip({
				 content: '<div id="rating_details_popup-'+nodeid+'" class="tooltiptop"></div><div class="tooltipmid">'+ratingDetails+'</div><div class="tooltipbottom"></div>',
			     show:{
					when:{
						event:'mouseover'
					},
					effect:'slide'
				 },
				 hide: {
					when:{
						event:'mouseout'
					},
					effect:'slide'
				},
/*				position: {
				    corner: {
				       target: 'bottomLeft',
				       tooltip: 'topLeft'
				    }
				},*/
				style: {
					width: 325,
					background: 'none',
					color: '#333333'
				},
				position: { adjust: { x: -98, y: -5 } }
			});
		}
		}catch(e){
			// to do
		}
	},

	disableFiveStarOnVoting : function(){
	 try{
		var divId = document.getElementById('fivestar-already-rated');
		if(divId != null && divId != "" && divId != undefined){
			$('#fivestar-already-rated .fivestar-click').removeClass('fivestar-click');
		    $('#fivestar-already-rated .fivestar-widget').unbind('mouseover');
		    $('#fivestar-already-rated .fivestar-widget .star a').unbind('click');
		    $('#fivestar-already-rated .fivestar-widget .star a').css('cursor','default');
		}
	 }catch(e){
			// to do
		}
	},

	fiveStarCheck : function(node_id,divId){
	 try{
		Drupal.attachBehaviors();
	    $('#'+divId+node_id+' '+'.fivestar-click').removeClass('fivestar-click');
	    $('#'+divId+node_id+' '+'.fivestar-widget').unbind('mouseover');
	    $('#'+divId+node_id+' '+'.fivestar-widget .star a').unbind('click');
	    $('#'+divId+node_id+' '+'.fivestar-widget .star a').css('cursor','default');
	 }catch(e){
			// to do
		}
	},

	/* Common function to evaluate the typed username and pass word in esignature
		and if true, to proceed with the remaining functionality
	*/
	evaluateLogin : function(){
		//drop call -- accord,enrollId,baseType,refundflag,isCommerceEnabled,assMand,learnerId,classid
		//mark call --- accord,userId,enrolledId,classTitle,learningType,classId
		//$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand,learnerId,classid); $("#dropmsg-content").html("Cancelling the enrollment. Please wait...");
		try{
		var obj = this;
		if(this.mandatoryCheck(this.esignObj.accord)){
			var uname = $('#esign-uname').val();
			var pass = $('#esign-pass').val();
			url = this.constructUrl("ajax/learningcore/evaluate-login");
			var params = {"uname":uname,"pass":pass};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
					if(result == true){
						$('#esign-pass-error').css('display','none');
						$('.esign-msg-close-btn').css('display','none');
						if(obj.esignObj.esignFor == "InstructorCancel"){
							$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(obj.esignObj.enrollId,obj.esignObj.baseType,
									obj.esignObj.refundflag,obj.esignObj.isCommerceEnabled,
									obj.esignObj.assMand,obj.esignObj.learnerId,obj.esignObj.classid);
							$("#dropmsg-content").html(Drupal.t("MSG424"));
						}else if(obj.esignObj.esignFor == "InstructorMarkComplete"){
							$('#esign-pass-error').css('display','none');
							$("#instructor-tab-inner").data("instructordesk").learnerMarkcompleteLearning(obj.esignObj.userId,
									obj.esignObj.enrolledId,obj.esignObj.classTitle,obj.esignObj.learningType,obj.esignObj.classId);
							$('#'+obj.esignObj.fieldset).remove();
						}else if(obj.esignObj.esignFor == "ManagerReject"){
							$("#lnr-myteam-search").data("lnrmyteamsearch").dropEnrollCall(obj.esignObj.userId, obj.esignObj.classId,
									obj.esignObj.enrolledId, obj.esignObj.learningType, obj.esignObj.refundFlag, obj.esignObj.isCommerceEnabled, obj.esignObj.assMand, obj.esignObj.classTitle, obj.esignObj.trid); $("#dropmsg-content").html(Drupal.t('MSG424'));
							$('#'+obj.esignObj.fieldset).remove();
						}else if(obj.esignObj.esignFor == "ManagerMarkComplete"){
							$("#lnr-myteam-search").data("lnrmyteamsearch").markcompleteLearning(obj.esignObj.userId,obj.esignObj.enrolledId,obj.esignObj.classTitle,
									obj.esignObj.rowObj,obj.esignObj.learningType,obj.esignObj.classId,obj.esignObj.trid);
							$('#'+obj.esignObj.fieldset).remove();
						}else if(obj.esignObj.esignFor == "ManagerAssign"){
							$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallAssignClass(obj.esignObj.userId, obj.esignObj.courseId,
									obj.esignObj.classId, obj.esignObj.rowObj, obj.esignObj.wailist);
							$('#'+obj.esignObj.fieldset).remove();
						}else if(obj.esignObj.esignFor == "ManagerAssignProgram"){
							$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallObjectAssignClass(obj.esignObj.userId,
									obj.esignObj.prgId, obj.esignObj.assignObject);
							$('#'+obj.esignObj.fieldset).remove();
						}
					}else{
						$('#esign-pass-error').css('display','block');
						$('.esign-msg-close-btn').css('display','block');
				    	$("#esign-pass").addClass("error");
					}
				}
		    });
		}else{
			$('.esign-msg-close-btn').css('display','none');
			$('#esign-pass-error').css('display','none');
		}
		}catch(e){
			// to do
		}
	},
	/* Common function to paint the esign fieldset when clicking on
		yes button or any okay button in the popups for Instructor Class
		cancel, instructor mark complete, manger reject class and manager mark complete.
	*/
	esign_accord_toggle : function(esignObj){
	 try{
		this.esignObj = esignObj;
		//$('#'+accord).toggle();
		var objStr = '$("body").data("learningcore")';
		var clickStyle = $("#"+esignObj.fieldset).css("display");
		if(clickStyle == "none") {
			var html = "";
			html +='<table cellpadding="4" cellspacing="0" border="0" valign="center">';
			html +='<tr><td class="field-title">'+SMARTPORTAL.t('Username')+'</td><td class="light-gray-color"><input id="esign-uname" type ="text" value="'+esignObj.uname+'" readonly></td></tr>';
			html +='<tr><td class="field-title">'+SMARTPORTAL.t('Password')+'</td><td class="light-gray-color"><input id="esign-pass" type ="password" name = "'+SMARTPORTAL.t('Password')+'" class="mandatory"><div id="esign-pass-error" style="display:none; color:#ff0000;font-size: 0.9em;">Please enter valid password.</div></td></tr>';
			//html +='<tr><td></td><td></td></tr>';
			html +='<tr><td></td><td class="tdButtonHolder" style="padding-right: 29px; text-align: right;"><a class="esign-close" href="javascript:void(0)" onclick=\''+objStr+'.disableButtonOnEsign();\'>'+SMARTPORTAL.t('Close')+'</a><input type ="button" class="form-submit" value ="'+SMARTPORTAL.t("E-Sign")+'"  onclick =\''+objStr+'.evaluateLogin();\'></td></tr>';
			html +='</table>';
			html +='</div>';
			$('#'+esignObj.accord).html(html);
			if(esignObj.fieldset == "esign-fieldset")
			{
				$('#'+esignObj.fieldset).prev('div').children('button').attr('disabled',"disabled");
				$('#'+esignObj.fieldset).prev('div').fadeTo('slow',.3);
			}
			if(esignObj.esignFor == "ManagerAssignProgram" || esignObj.esignFor == "ManagerAssign"){
				$('#tbl-paintCatalogContentResults > tbody > tr:last .myteam-esign-fieldset').addClass("myteam-esign-fieldset-last");
			}
			$('#'+esignObj.fieldset).show();//css("display","block");
			$('#'+esignObj.fieldset).slideDown(1000);

		} else {
   			$('#'+esignObj.fieldset).hide();//css("display","none");
			$('#'+esignObj.fieldset).slideUp(1000);
		}
	 }catch(e){
			// to do
		}
	},

	disableButtonOnEsign : function(){
	 try{
		$('#'+this.esignObj.fieldset).hide();//css("display","none");
		$('#'+this.esignObj.fieldset).slideUp(1000);
		$('#'+this.esignObj.fieldset).prev('div').children('button').removeAttr('disabled');
		$('#'+this.esignObj.fieldset).prev('div').css('opacity','');
	 }catch(e){
			// to do
		}
	},

	getEsignPopup : function(esignObj){
	 try{
    	var obj = this;
    	url = this.constructUrl("ajax/learningcore/getConcatenatedUserDets");
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(details){

				obj.esignObj = esignObj;
			    $('#esign-wizard').remove();
			    var html = "";
			    html += '<div id = "esign-wizard" class="esign-wizard-class">';
			    html += '<div id="esign-pass-error"><span class="esign-span-error">'+details[0].password_valid_txt+'</span></div>';
			    html += '<div id="esign-mandatory-error"><span class="esign-span-error">'+details[0].password_req_txt+'</span></div>';
			    html += '<div class="esign-msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div>';
			    html += '<div id="esign-message">'+details[0].esign_static_message+'</div>';
			    html += '<table cellpadding="4" cellspacing="0" border="0" valign="center">';
			    html += '<tr><td colspan="3" class="esign-labels">'+details[0].esign_date_txt+' : '+details[0].curr_date_time+'&nbsp;';
				html += details[0].time_zone+'</td></tr>';
				html += '<tr><td class="esign-labels">'+details[0].esign_name_txt+' : '+details[0].full_name+'</td></tr>';
				html += '</table>';

				html +='<table cellpadding="4" cellspacing="0" border="0" valign="center">';
				html +='<tr><td colspan="2" class="esign-title1">'+details[0].esign_uname_txt+' :</td>';
				html +='<td class="esign-textbox1"><input id="esign-uname" tabindex="1001" type ="text" value="'+details[0].user_name+'" maxlength = "60" size ="32" readonly></td>';
				html +='<td></td></tr>';
				html +='<tr><td colspan="2" class="esign-title2">'+details[0].esign_pass_txt+' :<span class="require-text">*</span></td>';
				html +='<td class="esign-textbox2"><input id="esign-pass" tabindex="1002" type ="password" name = "'+SMARTPORTAL.t('Password')+'" maxlength = "60" size ="32"></td>';
				html +='<td></td></tr>';
				html +='</table>';
				html +='</div>';
			    $("body").append(html);
			    var closeButt={};
			    closeButt[details[0].esign_cancel_txt]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#esign-wizard').remove();
			    if(esignObj){
			    	$("#"+obj.esignObj.popupDiv).show();
			    }
			    };
			    closeButt[details[0].esign_sign_txt]=function(){ $("body").data("learningcore").getNewEvaluateLogin();  };

			    $("#esign-wizard").dialog({
			        position:[(getWindowWidth()-500)/2,200],
			        bgiframe: true,
			        width:'auto',
			        resizable:false,
			        modal: true,
			        title: details[0].esign_title,
			        buttons:closeButt,
			        closeOnEscape: false,
			        draggable:true,
			        overlay:
			         {
			           opacity: 0.9,
			           background: "black"
			         }
			    });
			    $('#esign-wizard').parent('.ui-dialog').wrap("<div id='esign-wizard-dialog'></div>");

				$('#esign-pass').focus();
				$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr('tabindex','1003');
				$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('tabindex','1004');
				$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('id','esign-sign-button');

			    $('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
				$('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
				$('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			    $('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("esign-button-wizard").end();
			    $('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("esign-close-button").end();

			    if(esignObj){
			    	$("#"+obj.esignObj.popupDiv).hide();
			    }
			    $("#esign-wizard").show();
			    this.currTheme = Drupal.settings.ajaxPageState.theme;
			 	if(this.currTheme == "expertusoneV2"){
			    changeChildDialogPopUI('esign-wizard-dialog');
			 	}

				$('.ui-dialog-titlebar-close').click(function(){
			        $("#esign-wizard").remove();
			        if(esignObj){
			        	$("#"+obj.esignObj.popupDiv).show();
			        }
			    });
				$("#esign-pass").keyup(function(event){
				    if(event.keyCode == 13){
				        $("#esign-sign-button").click();
				    }
				});

				//Chrome browser dialog box UI fixes
				var getScreenWidth = $(window).width()/4;
				$("#esign-wizard-dialog").find('.ui-dialog').css('left',Math.round(getScreenWidth)+'px');

			}
		});


	 }catch(e){
			// to do
		}
	},


	getEvaluateLogin : function(){
		//drop call -- accord,enrollId,baseType,refundflag,isCommerceEnabled,assMand,learnerId,classid
		//mark call --- accord,userId,enrolledId,classTitle,learningType,classId
		//$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand,learnerId,classid); $("#dropmsg-content").html("Cancelling the enrollment. Please wait...");
		try{
		var obj = this;
		if($('#esign-pass').val() != ''){
			var uname = $('#esign-uname').val();
			var pass = $('#esign-pass').val();
			url = this.constructUrl("ajax/learningcore/evaluate-login");
			var params = {"uname":uname,"pass":pass};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
					if(result == true){
						$('#esign-pass-error').css('display','none');
						$('.esign-msg-close-btn').css('display','none');
						$('#esign-wizard').dialog('destroy');$('#esign-wizard').dialog('close');$('#esign-wizard').remove();
							if(obj.esignObj){
								if(obj.esignObj.esignFor == "InstructorCancel"){
								$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(obj.esignObj.enrollId,obj.esignObj.baseType,
										obj.esignObj.refundflag,obj.esignObj.isCommerceEnabled,
										obj.esignObj.assMand,obj.esignObj.learnerId,obj.esignObj.classid);
								//$("#dropMsg-wizard").show();
								$("#dropmsg-content").html(Drupal.t("MSG424"));
								}else if(obj.esignObj.esignFor == "InstructorMarkComplete"){
									$('#esign-pass-error').css('display','none');
									$("#instructor-tab-inner").data("instructordesk").learnerMarkcompleteLearning(obj.esignObj.userId,
											obj.esignObj.enrolledId,obj.esignObj.classTitle,obj.esignObj.learningType,obj.esignObj.classId);
									//$('#mark-complete-wizard').show();
								}else if(obj.esignObj.esignFor == "ManagerReject"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").dropEnrollCall(obj.esignObj.userId, obj.esignObj.classId,
											obj.esignObj.enrolledId, obj.esignObj.learningType, obj.esignObj.refundFlag, obj.esignObj.isCommerceEnabled, obj.esignObj.assMand, obj.esignObj.classTitle, obj.esignObj.trid); $("#dropmsg-content").html(Drupal.t('MSG424'));
									//$('#reject-wizard').show();
								}else if(obj.esignObj.esignFor == "ManagerMarkComplete"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").markcompleteLearning(obj.esignObj.userId,obj.esignObj.enrolledId,obj.esignObj.classTitle,
											obj.esignObj.rowObj,obj.esignObj.learningType,obj.esignObj.classId,obj.esignObj.trid);
									//$("#mark-complete-wizard").show();
								}else if(obj.esignObj.esignFor == "ManagerAssign"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallAssignClass(obj.esignObj.userId, obj.esignObj.courseId,
											obj.esignObj.classId, obj.esignObj.rowObj, obj.esignObj.wailist);

								}else if(obj.esignObj.esignFor == "ManagerAssignProgram"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallObjectAssignClass(obj.esignObj.userId,
											obj.esignObj.prgId, obj.esignObj.assignObject);

								}else if(obj.esignObj.esignFor == "UserAdmin" || obj.esignObj.esignFor == "CatalogClassAdmin"){
									$('#last-button-account').click();
								}else if(obj.esignObj.esignFor == "EsignAdmin"){
									$('.common-esign-save').click();
								}else if(obj.esignObj.esignFor == "UserAddrAdmin"){
									$('.common-esign-save-addr').click();
								}else if(obj.esignObj.esignFor == "AddAdmin"){
									$("#"+obj.esignObj.esignButtId).click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_unpublish"){
									$("#edit-catalog-course-save-unpublish").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_publish"){
									var data = saveInteractionsH5PWrapper(this);
									if(data == false)
									{
										return false;
									}
									$("#INTERACTIONDATA").val(data);
									$("#content_save_pub_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit"){
										var data = saveInteractionsH5PWrapper(this);
										if(data == false)
										{
											return false;
										}
										$("#INTERACTIONDATA").val(data);
										$("#content_save_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new"){
										$("#content_save_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new_publish"){
									$("#content_save_pub_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new_unpublish"){
										$("#edit-catalog-course-save-unpublish").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorPresentationInteractions"){
									saveInteractionsH5PWrapper(null,obj.esignObj.paramForPublish);
								}								
								
								
								$("#"+obj.esignObj.popupDiv).show();
							}else{
								$('#last-button-account').click();
							}
					}else{
						$('#esign-mandatory-error').css('display','none');
						$('.esign-msg-close-btn').css('display','none');
						$('#esign-pass-error').css('display','block');
						$('#esign-pass').val('');
				    	$("#esign-pass").addClass("esign-pass-txt-error");
					}
				}
		    });
		}else{
			$('#esign-pass-error').css('display','none');
			$('#esign-mandatory-error').css('display','block');
			$('.esign-msg-close-btn').css('display','block');
			$("#esign-pass").addClass("esign-pass-txt-error");
		}
		}catch(e){
			// to do
		}
	},

	getNewEvaluateLogin : function(){
		//drop call -- accord,enrollId,baseType,refundflag,isCommerceEnabled,assMand,learnerId,classid
		//mark call --- accord,userId,enrolledId,classTitle,learningType,classId
		//$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand,learnerId,classid); $("#dropmsg-content").html("Cancelling the enrollment. Please wait...");
		try{
		var obj = new Object;
		obj.esignObj =$.fn.getNewEsignPopup.esignObj;
		if($('#esign-pass').val() != ''){
			var uname = $('#esign-uname').val();
			var pass = $('#esign-pass').val();
			url = this.constructUrl("ajax/learningcore/evaluate-login");
			var params = {"uname":uname,"pass":pass};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
					if(result == true){
						$('#esign-pass-error').css('display','none');
						$('.esign-msg-close-btn').css('display','block');
						$('#esign-wizard').dialog('destroy');$('#esign-wizard').dialog('close');$('#esign-wizard').remove();
							if(obj.esignObj){
								
								if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_unpublish"){
									$("#edit-catalog-course-save-unpublish").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_publish"){
									var data = saveInteractionsH5PWrapper(this);
									if(data == false)
									{
										return false;
									}
									$("#INTERACTIONDATA").val(data);
									$("#content_save_pub_btn").click();
									
								}
								if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit"){
										var data = saveInteractionsH5PWrapper(this);
										if(data == false)
										{
											return false;
										}
										$("#INTERACTIONDATA").val(data);
										$("#content_save_btn").click();
								}
								if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new"){
										$("#content_save_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new_publish"){
									$("#content_save_pub_btn").click();
								}
								else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new_unpublish"){
										$("#edit-catalog-course-save-unpublish").click();
								}
								if(obj.esignObj.esignFor == "contentAuthorPresentationInteractions"){
									saveInteractionsH5PWrapper(null,obj.esignObj.paramForPublish);
									
								}								
								
								
								if(obj.esignObj.esignFor == "InstructorCancel"){
								$("#instructor-tab-inner").data("instructordesk").dropEnrollCall(obj.esignObj.enrollId,obj.esignObj.baseType,
										obj.esignObj.refundflag,obj.esignObj.isCommerceEnabled,
										obj.esignObj.assMand,obj.esignObj.learnerId,obj.esignObj.classid);
								//$("#dropMsg-wizard").show();
								$("#dropmsg-content").html(Drupal.t("MSG424"));
								}else if(obj.esignObj.esignFor == "InstructorMarkComplete"){
									$('#esign-pass-error').css('display','none');
									$("#instructor-tab-inner").data("instructordesk").learnerMarkcompleteLearning(obj.esignObj.userId,
											obj.esignObj.enrolledId,obj.esignObj.classTitle,obj.esignObj.learningType,obj.esignObj.classId);
									//$('#mark-complete-wizard').show();
								}else if(obj.esignObj.esignFor == "ManagerReject"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").dropEnrollCall(obj.esignObj.userId, obj.esignObj.classId,
											obj.esignObj.enrolledId, obj.esignObj.learningType, obj.esignObj.refundFlag, obj.esignObj.isCommerceEnabled, obj.esignObj.assMand, obj.esignObj.classTitle, obj.esignObj.trid); $("#dropmsg-content").html(Drupal.t('MSG424'));
									//$('#reject-wizard').show();
								}else if(obj.esignObj.esignFor == "ManagerMarkComplete"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").markcompleteLearning(obj.esignObj.userId,obj.esignObj.enrolledId,obj.esignObj.classTitle,
											obj.esignObj.rowObj,obj.esignObj.learningType,obj.esignObj.classId,obj.esignObj.trid);
									//$("#mark-complete-wizard").show();
								}else if(obj.esignObj.esignFor == "ManagerAssign"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallAssignClass(obj.esignObj.userId, obj.esignObj.courseId,
											obj.esignObj.classId, obj.esignObj.rowObj, obj.esignObj.wailist);

								}else if(obj.esignObj.esignFor == "ManagerAssignProgram"){
									$("#lnr-myteam-search").data("lnrmyteamsearch").renderCallObjectAssignClass(obj.esignObj.userId,
											obj.esignObj.prgId, obj.esignObj.assignObject);

								}else if(obj.esignObj.esignFor == "DeleteFunctionality"){
									$("body").data("mulitselectdatagrid").deleteEntity(obj.esignObj.type,obj.esignObj.entityId,obj.esignObj.entityType,obj.esignObj.id);
								}else if(obj.esignObj.esignFor == "AddKeywordToTemplate"){
									$("body").data("mulitselectdatagrid").addKeywordtoTemplate(obj.esignObj.type,obj.esignObj.entityId,obj.esignObj.entityType,obj.esignObj.id,obj.esignObj.kwdName);
								}else if(obj.esignObj.esignFor == "UserAdmin" || obj.esignObj.esignFor == "CatalogClassAdmin"){
									$('#last-button-account').click();
								}else if(obj.esignObj.esignFor == "EsignAdmin"){
									$('.common-esign-save').click();
								}else if(obj.esignObj.esignFor == "UserAddrAdmin"){
									$('.common-esign-save-addr').click();
								}else if(obj.esignObj.esignFor == "AddAdmin"){
									$("#"+obj.esignObj.esignButtId).click();
								}
								else if(obj.esignObj.esignFor == "attachCrsDelete"){
								      $("#"+obj.esignObj.objectId).click();
								}
								else if(obj.esignObj.esignFor == "addScreenDetails"){ //Added by Ganesh for Custom Attribute #custom_attribute_0078975
								     $("#hidden-screen-button").click();
								}
								else if(obj.esignObj.esignFor == "confirmationForMdlDel"){
								     deleteModuleCall(obj.esignObj.objectId, obj.esignObj.moduleId,obj.esignObj.objectType);
								}
								else if(obj.esignObj.esignFor == "DeleteAdminObject"){
									$("#root-admin").data("narrowsearch").callDeleteProcess(obj.esignObj.objectId,obj.esignObj.objectType);
								}else if(obj.esignObj.esignFor == "PublishUnpublishCatalog"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishCat(obj.esignObj.catalogId,obj.esignObj.catalogType,obj.esignObj.actionStatus,obj.esignObj.rowObj);
								}else if(obj.esignObj.esignFor == "displaymessagewizard"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishCat(obj.esignObj.catalogId,obj.esignObj.catalogType,obj.esignObj.actionStatus,obj.esignObj.rowObj);
									$("input[name = 'saveandshow']").click();
								}else if(obj.esignObj.esignFor == "PublishUnpublishProgram"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishPrg(obj.esignObj.programId,obj.esignObj.rowObj);
								}else if(obj.esignObj.esignFor == "ActivateDeactivateResource"){
									$("#root-admin").data("narrowsearch").activateAndDeactivateRsc(obj.esignObj.resourceId,obj.esignObj.resourceType,obj.esignObj.rowObj);
								}else if(obj.esignObj.esignFor == "ActivateDeactivateObject"){
									$("#root-admin").data("narrowsearch").activateAndDeactivateObj(obj.esignObj.objectId, obj.esignObj.objectType, obj.esignObj.rowObj);
								}else if(obj.esignObj.esignFor == "publishAndUnpublishSurveyAssessment"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishSur(obj.esignObj.surveyId, obj.esignObj.surveyType, obj.esignObj.rowObj);
								}else if(obj.esignObj.esignFor == "publishAndUnpublishSurveyAssessmentQuestions"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishQus(obj.esignObj.questionId, obj.esignObj.rowObj);
                               }else if(obj.esignObj.esignFor == "publishAndUnpublishContent"){
                            	   $("#root-admin").data("narrowsearch").publishAndUnpublishCnt(obj.esignObj.contentId, obj.esignObj.rowObj);
                              }else if(obj.esignObj.esignFor == "publishAndUnpublishVideo"){
                            	   $("#root-admin").data("narrowsearch").publishAndUnpublishCntVideo(obj.esignObj.contentId, obj.esignObj.rowObj);
                              }else if(obj.esignObj.esignFor == "publishAndUnpublishPresentation"){
                            	   $("#root-admin").data("narrowsearch").publishAndUnpublishCntPresentation(obj.esignObj.contentId, obj.esignObj.rowObj);
                              }else if(obj.esignObj.esignFor == "publishAndUnpublishBanner"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishBan(obj.esignObj.bannerId, obj.esignObj.bannerType, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishLocation"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishLoc(obj.esignObj.locationId, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishCustom"){//#custom_attribute_0078975
									$("#root-admin").data("narrowsearch").publishAndUnpublishCus(obj.esignObj.CustomId, obj.esignObj.rowObj);}
							  else if(obj.esignObj.esignFor == "publishAndUnpublishNotification"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishNot(obj.esignObj.notificationId, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishModule"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishModuleDialog(obj.esignObj.moduleName, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishDiscount"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishDiscountDialog(obj.esignObj.discountId, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishPayment"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishPaymentDialog(obj.esignObj.paymentId, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "publishAndUnpublishOrder"){
									$("#root-admin").data("narrowsearch").publishAndUnpublishOrderDialog(obj.esignObj.orderId, obj.esignObj.orderStatus, obj.esignObj.rowObj);
							  }else if(obj.esignObj.esignFor == "addReportPublishAndUnpublish"){
							  $("body").data("reportsearch").addReportPublishAndUnpublish(obj.esignObj.statusType);
							  }else if(obj.esignObj.esignFor == "displayWarningWizard"){
								  callInsertProcess(obj.esignObj.dupId,'esign');
							  }else if(obj.esignObj.esignFor == "displayDeleteWizardForum"){
								  switch (obj.esignObj.forumObjType) {
							    	case 'forum':
							    		$("#forum-list-display").data("forumlistdisplay").deleteForum(obj.esignObj.objectId,obj.esignObj.objectType);
							    		break;
							    	case 'topic':
							    		$("#forum-topic-list-display").data("forumlistdisplay").deleteTopics(obj.esignObj.objectId,obj.esignObj.nodeId);
								    	break;
							    	case 'comment':
							    		$("#forum-topic-list-display").data("forumlistdisplay").deleteComments(obj.esignObj.objectId,obj.esignObj.nodeId,obj.esignObj.commentId);
							    		break;
							    }
							  }else if(obj.esignObj.esignFor == "DeleteAdminObjectSettingsList"){
								  $('#deleteAddedList-'+obj.esignObj.objectId).click();
							  }

								$("div#"+obj.esignObj.popupDiv).show();
							}else{
								$('#last-button-account').click();
							}
					}else{
						$('#esign-mandatory-error').css('display','none');
						$('.esign-msg-close-btn').css('display','block');
						$('#esign-pass-error').css('display','block');
						$('#esign-pass').val('');
				    	$("#esign-pass").addClass("esign-pass-txt-error");
					}
				}
		    });
		}else{
			$('#esign-pass-error').css('display','none');
			$('#esign-mandatory-error').css('display','block');
			$('.esign-msg-close-btn').css('display','block');
			$("#esign-pass").addClass("esign-pass-txt-error");
		}
		}catch(e){
			// to do
		}
	},

	deleteDrupalUsers : function(drupalUserId){
		try{
		if(drupalUserId != ""){
			url = resource.base_url+'/?q='+"administration/learning-core/delete-drupal-users";
			var params = {"drupalUserId":drupalUserId};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
					//success
				}
			});
		}
		}catch(e){
			// to do
		}
	},

//Dupliacate for QTIP div function is used in the leaner side under User catalog Block
	getLeanerQtipDiv : function(qtipObj,obj) {
	 try{
		var popupId 	= qtipObj.popupDispId;
		this.qtipLenth = this.qtipLenth+1;
		var entityType = qtipObj.entityType;
		//var courseId = qtipObj.courseId!=undefined?qtipObj.courseId:'';
		var url 			  = resource.base_host+'?q='+qtipObj.url; // resource.base_host+'?q=administration/catalogaccess/33/cre_sys_obt_crs';
		var qtipClass 		  = qtipObj.qtipClass;
		var wBubble 		  = qtipObj.wBubble;//700; //Bubble Popup Width
		var hBubble 		  = qtipObj.hBubble;//300; //Bubble Popup Height
		var setwBubble        = wBubble-5;
		var setBtmLeft        = (setwBubble-104)/2;
		var qtipLeftPos       = (wBubble > 700) ? 400 : (setBtmLeft + 20);
		var tipPos = qtipObj.tipPosition;
		var catalogVisibleId = qtipObj.catalogVisibleId;
		var mLeft = 0;
		var tipPosition 	 = qtipObj.tipPosition;
		var topElements,bottomElements;
		var setTip			 = $(obj).position();
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader(popupId);
		$.ajax({
			 type: "GET",
   	         url: url,
   	         // data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
   	         success: function(data){
				$(".active-qtip-div").remove();
				var paintHtml = topElements+"<div id='paintContent"+popupId+"'>"+data.render_content_main+"</div>"+bottomElements;
				var contentDiv = qtipObj.catalogVisibleId+"_disp";
				$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
				if(qtipClass == 'display-message-positioning'){
					$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div "+qtipClass+"'></div>");
				}
				else{
					$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
				}
				$("#"+contentDiv).html(paintHtml);

				$(".bottom-qtip-tip").css('bottom','0px').css('position','absolute').css('z-index','101');
				$("#"+contentDiv).css('position','absolute').css('left','-'+qtipLeftPos+'px').css('bottom','40px').css('z-index','100');


				var p = $("#"+popupId);
				var position = p.position();
				var divHeight = $("#"+contentDiv).height();
				var parentTopPos = Math.round(position.top) + 10;
				var parentLeftPos = Math.round(position.left);
				this.currTheme = Drupal.settings.ajaxPageState.theme;
			 	if(this.currTheme == "expertusoneV2"){
					 if(tipPosition == 'tipfaceTopMiddle'){
						$('#wizarad-myprofile-skills .ui-jqgrid-bdiv').css({'overflow':'visible'});
						$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
						$("#"+popupId+" .bottom-qtip-tip-up").css({'top':+Math.round(setTip.top)+10+'px','left':Math.round(setTip.left)-21+'px','position':'absolute'});
						$(".bottom-qtip-tip").remove();
						$("#"+contentDiv).css({'position':'absolute','left':Math.round(setTip.left)-270+'px','top':Math.round(setTip.top)+38+'px','z-index':'100'});
						$("#learner-myskill-details .block-title-middle #skill-qtipid .active-qtip-div").css({'top':'86px'});
						$("#learner-myskill-details .block-title-middle #skill-qtipid .bottom-qtip-tip-up").css({'top':'57px','left':'567px'});
					}if(tipPosition == 'tipfaceTopMiddleFaceUp'){
						$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
						$("#"+popupId+" .bottom-qtip-tip-up").css({'top':+Math.round(setTip.top)+10+'px','left':Math.round(setTip.left)-21+'px','position':'absolute'});
						$(".bottom-qtip-tip").remove();
						$("#"+contentDiv).css({'position':'absolute','left':Math.round(setTip.left)-90+'px','top':Math.round(setTip.top)+30+'px','z-index':'100'});

					}
			 	}else {
			 		 if(tipPosition == 'tipfaceTopMiddle'){
							$('#wizarad-myprofile-skills .ui-jqgrid-bdiv').css({'overflow':'visible'});
							$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
							$("#"+popupId+" .bottom-qtip-tip-up").css({'top':+Math.round(setTip.top)+10+'px','left':Math.round(setTip.left)-21+'px','position':'absolute'});
							$(".bottom-qtip-tip").remove();
							$("#"+contentDiv).css({'position':'absolute','left':Math.round(setTip.left)-295+'px','top':Math.round(setTip.top)+38+'px','z-index':'100'});
							$("#learner-myskill-details .block-title-middle #skill-qtipid .active-qtip-div").css({'top':'47px','left':'326px'});
							$("#learner-myskill-details .block-title-middle #skill-qtipid .bottom-qtip-tip-up").css({'top':'18px','left':'628px'});
						}if(tipPosition == 'tipfaceTopMiddleFaceUp'){
							$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
							$("#"+popupId+" .bottom-qtip-tip-up").css({'top':+Math.round(setTip.top)+10+'px','left':Math.round(setTip.left)-21+'px','position':'absolute'});
							$(".bottom-qtip-tip").remove();
							$("#"+contentDiv).css({'position':'absolute','left':Math.round(setTip.left)-90+'px','top':Math.round(setTip.top)+30+'px','z-index':'100'});

						}
			 	}
			 	if(tipPosition == 'tipfaceMiddleRight'){
			 		var tipFrom = qtipObj.tipFrom;
			 		var boxRight = '151px';
			 		var tipRight = '103px';
			 		if(tipFrom == 'addUserOrder'){
			 			boxRight = '130px';
			 			tipRight = '81px';
			 		}
			 		else if(tipFrom == 'addProductOrder'){
			 			boxRight = '165px';
			 			tipRight = '116px';
			 		}
					$("#"+popupId).append("<div class='bottom-qtip-tip-right active-qtip-div set-wbubble-left'></div>");
					$(".bottom-qtip-tip-right").css({'top':+parentTopPos-30+'px','position':'absolute','right': tipRight});
					$(".bottom-qtip-tip").remove();
					$("#"+contentDiv).css({'position':'absolute','right':boxRight,'left':'auto','top':+parentTopPos-125+'px','bottom':'0px','z-index':'100'});
				}
				if($("#"+popupId+" .tab-title").width() > 0 ){
					 var labelWidth = ($("#"+popupId+" .tab-title").width())/2;
					 var bubbleWidth = 32;
					 var setbubblePosition = labelWidth - bubbleWidth;
					 $(".set-wbubble-left").css('left', setbubblePosition +'px');
			    }

				//Set buble tool tip arrow position
				tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
				topElements = '<a class="qtip-close-button" onclick="$(\'.active-qtip-div\').remove();"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
				bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
				$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");

				var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message'></div>"+data.render_content_main+"</div>"+bottomElements;
				var contentDiv = qtipObj.catalogVisibleId+"_disp";
				EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader(popupId);
				$("#"+contentDiv).html(paintHtml);
				if(data.drupal_settings) {
				   $.extend(true, Drupal.settings, data.drupal_settings);
				}
					Drupal.attachBehaviors();
	            }
 			});
	 }catch(e){
			// to do
		}
	},

	paintOtherUserProfile: function(qtipObj){
		try{
		$('.ui-dialog').remove();
		var popupId 	= qtipObj.popupDispId;
		var url 			  = resource.base_host+'?q='+qtipObj.url;
		var contentDiv = qtipObj.catalogVisibleId+"_disp";
		var loaderDivId = qtipObj.dataloaderid;
		var dataIntId = qtipObj.dataIntId;
		var stdid		= this.getLearnerId();
		if(stdid == "0" || stdid == "")
		{
		    self.location='?q=learning/enrollment-search';
	        return;
		}
		$('#paintContent'+popupId).remove(); //0079970: Unable to close the user profile pop up
		this.createLoader(loaderDivId);
		var themeKey = Drupal.settings.ajaxPageState.theme;
		var abstractObj = this;
		$.ajax({
			 type: "GET",
  	         url: url,
  	        // data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
  	         success: function(data){
  	        	 //0033772: Code Re-Factoring - Home page - Remove unwanted JavaScript
			if(Drupal.settings.activeModulesList.myactivity == 'Y')
  	        	abstractObj.include("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_activity/exp_sp_my_activity.js");
			if(Drupal.settings.activeModulesList.myskill == 'Y')
  	        	abstractObj.include("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_skill/exp_sp_my_skill.js");
  	        	if(themeKey == "expertusoneV2"){
	  	        	abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_activity/exp_sp_my_activity_v2.css");
	  	        	abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_skill/exp_sp_my_skill_v2.css");
	  	        	abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_profile/exp_sp_my_profile_v2.css");
  	        	}else{
  	        		abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_activity/exp_sp_my_activity.css");
  	  	        	abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_skill/exp_sp_my_skill.css");
  	  	        	abstractObj.includecss("/sites/all/modules/core/exp_sp_core/modules/exp_sp_my_profile/exp_sp_my_profile.css");
  	        	}
				$("#"+contentDiv).dialog({
					bgiframe: true,
					width:535,
					resizable:false,
					draggable:false,
					closeOnEscape: false,
					modal: true,
					close: function(){
						$('.ui-dialog').remove();
						$('#paintContent'+popupId).remove();
						/*if(document.getElementById('wizarad-myprofile-activity')){
							$('#wizarad-myprofile-activity').myprofileactivity();
							$("#wizarad-myprofile-activity").data('myprofileactivity').destroyLoader('wizarad-myprofile-activity');
						}else if(document.getElementById('wizarad-myprofile-skills')) {
							$("#wizarad-myprofile-skills").myprofileskills();
							$("#wizarad-myprofile-skills").data('myprofileskills').destroyLoader('wizarad-myprofile-skills');
						}*/
					},
					title: Drupal.t('LBL852'),
					overlay:
					{
					   opacity: 0.5,
					   background: '#000000'
					 }

				});
				/*if(qtipObj.dataIntId == 'announcement'){
					$('.ui-widget-overlay').css('z-index','1000');
				}else{
					$('.ui-widget-overlay').css('z-index','99');
				}*/
				$('.ui-widget-overlay').css('z-index','1000');
				var paintHtml = "<div id='paintContent"+popupId+"'>"+data+"</div>";
				$('.ui-dialog #'+contentDiv).html(paintHtml);
				var uiHeightSet = '';
				uiHeightSet = $('.ui-dialog:visible').css('top');
				uiHeightSet = Math.round(uiHeightSet.substring(uiHeightSet.length-2,0)) - 120;
				$('.ui-dialog').css('top',uiHeightSet+'px');
				var profileDialogHeight = $("#"+contentDiv).height();
				$(".ui-dialog #"+contentDiv).css('overflow-y','scroll').css('height',440);
				this.currTheme = Drupal.settings.ajaxPageState.theme;
			 	if(this.currTheme == "expertusoneV2"){
			 	$(".ui-dialog #"+contentDiv).addClass('user-popout-container');
			 	
			 	$('#menuaddskill .addskillcerti').css('display','none');
				$('.user-popout-container').find('.block').css("margin-top","10px");
				changeDialogPopUI();
				$('.expertusV2PopupContainer').find('.ui-dialog-content').css("padding","0px 13px");

			 	}
				$("#"+loaderDivId).data(dataIntId).destroyLoader(loaderDivId);
				/* #0039834 Ie The Skills and Profile Activity Loader are Gets Generated and It is not Destroyed So Issue is Identifed For only My-profile Page */
				if(Drupal.settings.activeModulesList.myactivity == 'Y'){
					  if ($.browser.msie && loaderDivId == 'catalog-users') {
						$("#wizarad-myprofile-activity").data('myprofileactivity').destroyLoader('wizarad-myprofile-activity');
					  }
				}
				if(Drupal.settings.activeModulesList.myskill == 'Y'){
					if ($.browser.msie && loaderDivId == 'catalog-users') {
						$("#wizarad-myprofile-skills").data('myprofileskills').destroyLoader('wizarad-myprofile-skills');
				    }
				}
				vtip();
				
				setTimeout(function () {
					    $(".user-popout-container .profile-name-inline .limit-title").trunk8(trunk8.profile_title);
						$(".user-popout-container #userprofile-part .limit-desc").trunk8(trunk8.profile_desc);
			    }, 100);
				
			}

		});
		//$(".user-popout-container .profile-name-inline .limit-title").trunk8(trunk8.profile_title);
		//$(".user-popout-container #userprofile-part .limit-desc").trunk8(trunk8.profile_desc);
		}catch(e){
			// to do
		}
	},
	showHideMultipleLi: function() {
		try{
		var n = 1;
		var $list = $('.messages ul');
		$list.find('li').addClass('visibility').removeClass('hide');
		var totalWidth = 0, actualWidth = 0;
		var divId = (document.getElementById('show_expertus_message')!= null)? '#show_expertus_message' :'#show_expertus_tp_message';
		var divwidth= $(divId).parent().css('width');
		divwidth = parseInt(divwidth.substring(0,divwidth.length-2));
		$list.children().each(function() {
			totalWidth = $(this).width();
			if(totalWidth > actualWidth){
				if(totalWidth >= (divwidth-40)){
					actualWidth = divwidth-40;
				}else{
					actualWidth = totalWidth;
				}
			}

		});
		$list.find('li').removeClass('visibility').addClass('hide');
		$list.find('li:lt(' + n + ')').removeClass('hide');
		//$('.messages ul li').css('width',(actualWidth+5)+'px');
		$list.find('.hide').slideDown('slow', function(){
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2") {
			$('.msg-minmax-icon').css('background','url("/sites/all/themes/core/expertusoneV2/expertusone-internals/images/ExpertusIcons.png") no-repeat -118px -25px');
		    }else{
			$('.msg-minmax-icon').css('background','url("/sites/all/themes/core/expertusone/expertusone-internals/images/ExpertusIcons.png") no-repeat -118px -25px');
		    }
		});
		$('#message-container').css('visibility','visible');
		}catch(e){
			// to do
		}
	},
	/*selectedOptionTypeDisplay : function(sortbytxt,className,toggleClassName) {
		if (sortbytxt!=null && sortbytxt!=undefined) this.sortbyValue = sortbytxt;
		if(this.sortbyValue!=''){
			if(this.sortbyValue == 'AZ'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL017'));
			}
			else if(this.sortbyValue == 'Time'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL044'));
			}
			else if(this.sortbyValue == 'ClassStartDate'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL045'));
			}
			else if(this.sortbyValue == 'Mandatory'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL022'));
			}
			else if(this.sortbyValue == 'ZA'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL018'));
			}
			else if(this.sortbyValue == 'dateOld'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL019'));
			}
			else if(this.sortbyValue == 'dateNew'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL020'));
			}
			else if(this.sortbyValue == 'type'){
				$('.'+className).text('');
				$('.'+className).text(Drupal.t('LBL021'));
			}
		} else {
			$('.'+className).text('');
		}
		$("body").data("learningcore").sortTypeToggle(toggleClassName);
	},*/

	closeErrorMessage: function() {
		try{
		$('#message-container').remove();
		$('#esign-mandatory-error').css('display','none');
		$('#esign-pass-error').css('display','none');
		$('.esign-msg-close-btn').css('display','none');
		}catch(e){
			// to do
		}
	},

	paintLearningImage : function(cellvalue, options, rowObject) {
		try{
		// console.log('cellvalue=', cellvalue, 'rowObject-', rowObject);
		var html = '';
		var clsImage 				= rowObject['learning_type_image'];
		var dataDelType				= rowObject['learning_type'];
		html += '<div title="' +Drupal.t(dataDelType) + '" class="'+ clsImage +' vtip"></div>';
		return html;
		}catch(e){
			// to do
		}
	},

	deleteLearnWidget : function(contentId, widgetId ){
		try{
			var loaderElement = ($(".salesforce-widget").length) ?  "widget" :  "main";
			this.createLoader(loaderElement); // show loader
			$('.loadercontent').css('z-index','2000');
			var params = {
				id: contentId,
				widgetId: widgetId,
				action: "delete",
				salesforce: ($(".salesforce-widget").length) ? 1: 0
			};
			$.ajax({
				type: "POST",
				url: '/?q=user-preference/update',
				data:  params,
				dataType:  'json',
				success: function(result){
					if (result.session != undefined && result.session == 'session_out') {
						self.location='?q=learning/enrollment-search';
						return;
					}
					$("#" + contentId + " #" + widgetId).remove();
					Drupal.settings.last_left_panel = result.last_left_panel;
					//52001: Change class pop not showing when we click on change class link in action link button in My Enrollment panel
					if(widgetId == 'tab_my_learningplan_customized'){
						$( "#page-container" ).append( '<div id="learningplan-tab-inner" class="dummyelement_tab_my_learningplan" style="display:none;"></div>' );
						Drupal.attachBehaviors('learningplan-tab-inner');
						$( "#learningplan-tab-inner" ).learningplan();
					}
					if(widgetId == 'tab_my_enrollment_customized'){
						$(".page-learning-enrollment-search #page-container" ).append( '<div id="learner-enrollment-tab-inner" class="dummyelement_tab_my_enrollment" style="display:none;"></div>' );
						Drupal.attachBehaviors('learner-enrollment-tab-inner');
						$("#learner-enrollment-tab-inner").enrollment();
						$("#learner-enrollment-tab-inner" ).contentLaunch();
					}
					$('#delete-msg-wizard').remove();
					if (result.sidebar == false) {
						$("body").data("learningcore").removeSideBarSection();
					}
					if (result.last_left_panel == true) {
						$("#highlight-list li:first").find(".delete-widget").css("display","none");
						$("#highlight-list li:first").find(".delete-widget-disabled").css("display","block");
						// Retrive 20 rows, when it was a last panel in left - 51857
						var gridId = $("#highlight-list li:first").find(".ui-jqgrid-btable").attr('id');
						$("#"+gridId).setGridParam({rowNum: 20, rowList: [20,30,40], page:1}).trigger('reloadGrid');
						$("#highlight-list li:first").find(".page_count_view").text(20);
					}
					$("body").data("learningcore").destroyLoader(loaderElement);
				}
			});
		}catch(e){
			// to do
		}

	},

	showWidgetList : function(){
		try {
			var params = { salesforce: ($(".salesforce-widget").length) ? 1: 0 };
			$.ajax({
				type: "GET",
				url: '/?q=user-preference/list',
				data:  params,
				success: function(result){
					if (result == 'session_out') {
						self.location='?q=learning/enrollment-search';
						return;
					}
				  if ($("#user-preference-settings #manage-dd-list").length) {
					  $("#user-preference-settings #manage-dd-list").replaceWith(result);
				  } else {
					  $("#user-preference-settings").append(result);
				  }
				//  $('#manage-dd-list').css('visibility','visible');
				  //Vtip-Display toolt tip in mouse over
				  vtip();
				}
			});
		}catch(e){
			// to do
		}
	},

	addLearnWidget : function(contentId, widgetId ){
		try {
			var loaderElement = ($(".salesforce-widget").length) ?  "widget" :  "main";
			this.createLoader(loaderElement); // show loader
			var params = {
				id: contentId,
				widgetId: widgetId,
				action: "add",
				salesforce: ($(".salesforce-widget").length) ? 1: 0
			};
			$.ajax({
				type: "POST",
				url: '/?q=user-preference/update',
				data:  params,
				dataType: 'json',
				success: function(result){
					//52001: Change class pop not showing when we click on change class link in action link button in My Enrollment panel
					if(widgetId == 'tab_my_learningplan_customized'){
						$(".dummyelement_tab_my_learningplan" ).remove();
					}
					if(widgetId == 'tab_my_enrollment_customized'){
				    	$(".dummyelement_tab_my_enrollment" ).remove();
					}
				  if ($(".region-highlight").length == 0) {
					$("#main-wrapper #content .section").append('<div class="region region-highlight"><ul class="sortable-list" id="highlight-list"></ul></div>');
					createSortableList(".region-highlight .sortable-list");
				  }

				  if (result.sidebar == true && $(".region-sidebar-second").length == 0) {
						$("body").data("learningcore").addSideBarSection();
				  }
				  Drupal.settings.last_left_panel = result.last_left_panel;
				  $("#"+contentId).append(result.content);
				  $('#manage-dd-list').css('visibility','hidden');
				  switch(widgetId) {
				  	case "tab_my_enrollment_customized":
				  		enrFilters = [];
				  		enrFilters.push({filterVal : 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att', attrVal : 'Enrolled', type : 'Enrollmentpart',name: 'status_filter_myenrollment'});
				  		Drupal.settings.myenrolmentSearchStr = '&regstatuschk=lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ'				 
				  		$("#learner-enrollment-tab-inner").contentLaunch();
				  		$("#block-take-survey").surveylearner();
				  		if(Drupal.settings.convertion.mylearn_version==1) {
				  			$("#learner-enrollment-tab-inner").contentPlayer();
				  		}
						statusFilterSubmit('myenrollment','AZ');
						$("#learner-enrollment-tab-inner").enrollment();
				  		break;
				  	case "tab_my_learningplan_customized":
				  		prgFilters = [];
				  		prgFilters.push({filterVal : 'lrn_tpm_ovr_enr|lrn_tpm_ovr_inp', attrVal : 'Enrolled', type : 'EnrollmentLP',name: 'status_filter_myprograms'});
					    Drupal.settings.myprogramsSearchStr = '&regstatuschk=lrn_tpm_ovr_enr|lrn_tpm_ovr_inp&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';
				  		statusFilterSubmit('myprograms','AZ'); 
				  		$("#learningplan-tab-inner").contentLaunch();
				  		$("#block-take-survey").surveylearner();
				  		if(Drupal.settings.convertion.mylearn_version==1) {
				  			$("#learningplan-tab-inner").contentPlayer();
				  		}
				  		$("#learningplan-tab-inner").learningplan();
						break;
				  	case "tab_instructor_desk_customized":
				  		clsFilters = [];
				  		clsFilters.push({filterVal : 'scheduled', attrVal : 'Scheduled', type : 'Scheduled',name: 'status_filter_myclasses'});
				  		Drupal.settings.myclassesSearchStr = '&regstatuschk=scheduled&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';
					  	statusFilterSubmit('myclasses','AZ');
					  	$("#instructor-tab-inner").instructordesk();	
						break;
				  	case "announcement":
				  		$("#div_announcement").announcement();
						break;
				  	case "tab_my_calendar":
				  		$("#catalog-admin-cal").mycalendar();
						break;
				  	case "mytranscript":
						$("#div_my_transcript").mytranscript();
						break;
				  	case "online_users":
				  		$("#expertus-online-users").lnrwhoisonline();
						break;
				  }
				 	if (result.last_left_panel == false) {
				 		$("#highlight-list .delete-widget-disabled").css('display','none');
						$("#highlight-list .delete-widget").css('display','block');
						var gridId = $("#highlight-list li:first").find(".ui-jqgrid-btable").attr('id');
						var rowsView = $("#"+gridId).jqGrid('getGridParam', 'rowNum'); // Check the rows, based on the retrive new results
						if (rowsView > 10 ) {
							$("#"+gridId).setGridParam({ rowNum:10, rowList:[10,15,20], page: 1 }).trigger("reloadGrid");
							$("#highlight-list li:first").find(".page_count_view").text(10);
						}
					}
					attachHandlerToFilterIcon();
					$("body").data("learningcore").destroyLoader(loaderElement);
				}
			});
		}catch(e){
			// to do
		}
	},

	removeSideBarSection : function() {
		try {
			Drupal.settings.mylearning_right = false;
			$(".region-sidebar-second").remove();

			$("#content").addClass("clsContentFull");
			var currentPage = $('.ui-pg-input').val();
			var currentWidth = ($(".salesforce-widget").length) ? $("#block-system-main-menu").width() : $("#header").width();
			if ($("#tab_my_enrollment").length) { // enrollment
			  $("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			  $("#enroll-result-container").addClass("clsenroll-result-container");
			  if (Drupal.settings.last_left_panel) {
				  $("#paintEnrollmentResults").jqGrid('setGridParam', { rowNum:20, rowList: [20,30,40], page:1}); // set new grid params, when it was a last panel in left
				  $("#highlight-list li:first").find(".page_count_view").text(20);
			  } else {
				  $("#paintEnrollmentResults").jqGrid('setGridParam', { page:1});
			  }
			  $("#paintEnrollmentResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintEnrollmentResults").trigger("reloadGrid");
			}

			if ($("#tab_my_learningplan").length) { // lp
			  $("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			  $("#enroll-lp-result-container").addClass("clsenroll-lp-result-container");
			  if (Drupal.settings.last_left_panel) {
				  $("#paintEnrollmentLPResults").jqGrid('setGridParam', { rowNum:20, rowList: [20,30,40], page:1});  // set new grid params, when it was a last panel in left
				  $("#highlight-list li:first").find(".page_count_view").text(20);
			  } else {
				  $("#paintEnrollmentLPResults").jqGrid('setGridParam', { page:1});
			  }
			  $("#paintEnrollmentLPResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintEnrollmentLPResults").trigger("reloadGrid");
			}

			if ($("#tab_instructor_desk").length) { // my classes
			  $("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			  $("#instructor-result-container").addClass("clsinstructor-result-container");
			  if (Drupal.settings.last_left_panel) {
				  $("#paintInstructorResults").jqGrid('setGridParam', { rowNum:20, rowList: [20,30,40], page:1});  // set new grid params, when it was a last panel in left
				  $("#highlight-list li:first").find(".page_count_view").text(20);
			  } else {
				  $("#paintInstructorResults").jqGrid('setGridParam', { page:1});
			  }
			  $("#paintInstructorResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintInstructorResults").trigger("reloadGrid");
			}

			$(".enroll-bottom-curve").addClass("clsRightNoData");
		}catch(e){
			// to do
		}
	},

	addSideBarSection : function() {
		try {
			Drupal.settings.mylearning_right = true;

			if ($(".region-sidebar-second").length == 0) {
				$("#main-wrapper #main").append('<div class="region region-sidebar-second column sidebar"><ul class="sortable-list" id="sidebar_second-list"></ul></div>');
				createSortableList(".region-sidebar-second .sortable-list");
			}

			$("#content").removeClass("clsContentFull");
			var currentPage = $('.ui-pg-input').val();

			if ($("#tab_my_enrollment").length) { // enrollment
			  var currentWidth = $("#enroll-result-container").width();
			  $("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			  $("#enroll-result-container").removeClass("clsenroll-result-container");
			  $("#paintEnrollmentResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintEnrollmentResults").trigger("reloadGrid",[{page:currentPage}]);

			}

			if ($("#tab_my_learningplan").length) { // lp
			  var currentWidth = 663;
			  $("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			  $("#enroll-lp-result-container").removeClass("clsenroll-lp-result-container");
			  $("#paintEnrollmentLPResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintEnrollmentLPResults").trigger("reloadGrid",[{page:currentPage}]);

			}

			if ($("#tab_instructor_desk").length) { // my classes
			  var currentWidth = $("#instructor-result-container").width();
			  $("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			  $("#instructor-result-container").removeClass("clsinstructor-result-container");
			  $("#paintInstructorResults").jqGrid('setGridWidth', currentWidth);
			  $("#paintInstructorResults").trigger("reloadGrid",[{page:currentPage}]);

			}

			$(".enroll-bottom-curve").removeClass("clsRightNoData");
		}catch(e){
			// to do
		}
	},

	displayDeleteWizardLearner : function(title,objectId,objectType,wSize){
		try {
			var uniqueClassPopup = '';
			var currTheme = Drupal.settings.ajaxPageState.theme;
			var wSize = (wSize) ? wSize : 300;
			var closeButAction = '';

			closeButAction = function(){ $("body").data("learningcore").deleteLearnWidget(objectType,objectId); };
			$('#delete-msg-wizard').remove();
		    var html = '';
		    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
			if(currTheme == 'expertusoneV2'){
		    		html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+''+'?'+'</td></tr>';
		    } else {
		    		html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+''+'?'+'</td></tr>';
		    }
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    var closeButt={};
		    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
		    closeButt[Drupal.t('Yes')]= closeButAction;

		    $("#delete-msg-wizard").dialog({
		        position:[(getWindowWidth()-400)/2,200],
		        bgiframe: true,
		        width:wSize,
		        minHeight:"auto",
		        resizable:false,
		        modal: true,
		        title: Drupal.t('MSG943'),//"title",
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:false,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

		    $("#delete-msg-wizard").show();

			$('.ui-dialog-titlebar-close').click(function(){
		        $("#delete-msg-wizard").remove();
		    });
			if(currTheme == 'expertusoneV2'){
		    	changeDialogPopUI();
		    }
			$("#delete-msg-wizard .commanTitleAll").css('padding','30px 24px');
			/*-- 37211: Unable to delete a class in FF version 32.0 --*/
		    if($('div.qtip-defaults').length > 0) {
		    	var prevZindex = $('.qtip-defaults').css('z-index');
		    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
		    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
		    }
		}catch(e){
			// to do
		}
	},
	refreshJScrollPane: function(selector) {
		try {
			if($(selector).data('jsp') !== undefined) {
				$(selector).data('jsp').reinitialise();
				setTimeout(function () {
					if($(selector+" .jspContainer").find('.jspVerticalBar').length==0){
						$(selector+' .jspPane').css('top','0px');
					}
				},100);
			} else {
				$(selector).jScrollPane({ autoReinitialise: true });
			}
		} catch(e) {
			// window.console.log(e, e.stack);
		}
	},
	refreshLastAccessedLearningRow: function(grid, gridRow) {
		try {
			var rowFound = false;
			// console.log('refreshLastAccessedLearningRow', console.trace());
			var grid = $(grid);
			if(gridRow === undefined) {
				var gridRow = grid.jqGrid('getGridParam', 'lastAccessedRow');
			}
			if(gridRow !== undefined && gridRow != null && typeof gridRow.id != 'undefined') {
				// console.log(gridRow, gridRow.id);
				var enrId = gridRow.id;
				// console.log('refreshLastAccessedLearningRow enr_id', enrId);
				var options = {data: {enr_id: enrId,page:1,rows:1}};
				grid.jqGrid('updateRowByRowId', options);
				rowFound = true;	// return true to stop the grid reload
				grid.jqGrid('setGridParam', {lastAccessedRow: null});	// set it to null to avoid grid reload confusions
			} else {
				// grid.trigger("reloadGrid",[{page:1}]);
				rowFound = false;	// return false so that reload of grid happens as no last accessed grid row found
			}
		} catch(e) {
			rowFound = false;
			// window.console.log(e, e.stack);
		}
		// window.console.log(rowFound);
		return rowFound;
	}
});

$.extend($.ui.learningcore.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true}});

})(jQuery);

$(function() {
	try{
	$("body").learningcore();

  //below code to load showHideMultipleLi() for messgae display
  var wrul=window.location;
	wrul=wrul.toString();
	var tmp=$('#message-container').html();
	if((tmp != null) &&(tmp != '')){
		if(wrul.indexOf("cart")>=0 || wrul.indexOf("administration/order")){
			$('body').data('learningcore').showHideMultipleLi();
		}
	}


  // Command to update score on video ctools modal close
  Drupal.ajax.prototype.commands.updateVODScoreOnCtoolsModalClose = function(ajax, response, status) {
    if (typeof Drupal.CTools.Modal.closeCommand.updateVODScore === 'undefined') {
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.launched_from = ' + response.launched_from);
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.course_id = ' + response.course_id);
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.class_id = ' + response.class_id);
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.lesson_id = ' + response.lesson_id);
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.enroll_id = ' + response.enroll_id);
      //console.log('exp_sp_learning.js : updateVODScoreOnCtoolsModalClose() - response.prev_content_status = ' + response.prev_content_status);
      Drupal.CTools.Modal.closeCommand.updateVODScore =
        (function (launchedFrom, courseId, classId, lessonId, versionId,enrollId, prevContentStatus, videoSessionId) {
          var jsObject = 'contentLaunch';
          var objectSelector = '';
           if (launchedFrom == 'LP') {
             var objectSelector = '#learningplan-tab-inner';
           }
           else if (launchedFrom == 'ME') {
        	   if(document.getElementById('lnr-catalog-search'))
        		   var objectSelector = '#lnr-catalog-search';
        	   else
             		var objectSelector = '#learner-enrollment-tab-inner';
           }

           return { cmd: function () {
        	   			  $( "#launch-content-container" ).parent().parent('.ui-widget-content').css( "visibility", "visible" );
        	   			  $("#lplaunch-content-container" ).parent().parent('.ui-widget-content').css( "visibility", "visible" );
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - jsObject = ' + jsObject);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - launchedFrom = ' + launchedFrom);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - courseId = ' + courseId);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - classId = ' + classId);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - lessonId = ' + lessonId);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - enrollId = ' + enrollId);
                           //console.log('exp_sp_learning.js : updateVODScore.cmd() - prevContentStatus = ' + prevContentStatus);

                           $(objectSelector).data(jsObject).updateVODScoreOnCtoolsModalClose(
                               launchedFrom,
                               courseId,
                               classId,
                               lessonId,
                               versionId,
                               enrollId,
                               prevContentStatus,
							   videoSessionId,
							   response.type,
							   response.h5p_cnt_id
							   );
                         }
                   };
        })(response.launched_from, response.course_id, response.class_id, response.lesson_id,response.version_id, response.enroll_id, response.prev_content_status, response.video_session_id);
    }
  };

  Drupal.ajax.prototype.commands.ctoolsModalCreateLoader = function(ajax, response, status) {
    EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('modal-content');
    $('.ctool-video-modal .ctools-modal-content .ctools-sample-modal-content ctools-face-table').css('margin', '0 auto');
    $('.ctool-video-modal .loaderimg').css('margin', '0 auto');
    $( "#launch-content-container" ).parent().parent('.ui-widget-content').css( "visibility", "hidden" );
    $("#lplaunch-content-container" ).parent().parent('.ui-widget-content').css( "visibility", "hidden" );
	seekForFirstTime = false;
	if(videojs !== undefined && response.video_id !== undefined) {
	disposeVideoJSPlayer(response.video_id);
	videojs(response.video_id, {}, function(){
	  // Player (this) is initialized and ready.
	  var vodPlayer = this;
	  vodPlayer.on('mouseout', function(){ 
		  this.controls(false);
		});

		vodPlayer.on('mouseover', function(){
		  this.controls(true);
		});
	  // $('#'+response.video_id).css('visibility', 'visible');
	  // $('#'+response.video_id).find('video').css('visibility', 'visible');
	  winobj = {closed: false};
	  SCORM_API_WRAPPER.prototype.refreshSession();
	  if(response.play_from !== undefined) {
	    vodPlayer.currentTime(response.play_from);
	    vodPlayer.play();
		seekForFirstTime = true;
	  }
	  vodPlayer.on('loadedmetadata', function() {
		if(seekForFirstTime && response.play_from > this.currentTime()) {
		  this.currentTime(response.play_from);
		  seekForFirstTime = false;
		}
	  });
      if(response.launchedFrom == 'LP'){
          setObj = $("#learningplan-tab-inner").data("contentLaunch");
      }
	  else if(response.launchedFrom == 'ME'){
		  if(document.getElementById('lnr-catalog-search'))
			  setObj = $("#lnr-catalog-search").data("contentLaunch");
		  else
          	setObj = $("#learner-enrollment-tab-inner").data("contentLaunch");
      }
	  updateStatusToDB = setInterval(function(){updateVideoProgressToDB(response,setObj); },Drupal.settings.content);
	  vodPlayer.on('durationchange', function() {
		this.controlBar.progressControl.seekBar.loadProgressBar.update();
	  });
	  $('#loaderdivmodal-content').remove();
	  ajaxInterval = vodPlayer.setInterval(function() {
	    updateVideoProgress(vodPlayer);
	  }, 1000, vodPlayer);
      
	  var updateFreq = (response.update_frequency !== undefined ? (response.update_frequency * 1000) : 10000);
	  saveInterval = setInterval(function() {
	    saveVideoProgress(vodPlayer, false);
	  }, updateFreq, vodPlayer);
      
	  vodPlayer.on('ended', function(){
	    vodPlayer = this;
		if(vodPlayer.techName_ == 'Youtube') {
			vodPlayer.pause();
			vodPlayer.play();
		}
	    var progress	= 100; // when player cursor moved at the end.
	    videoTrackerProgress = {
	      session_id: response.video_id,
	      current_position: this.currentTime(),
	      progress: progress,
	      video_duration: this.duration(),
	      additional_data: {video_session_id: response.video_id}
	    };
	    saveVideoProgress(vodPlayer, true);
	    updateVideoProgressToDB(response,setObj);
	    clearInterval(saveInterval);
	    clearInterval(ajaxInterval);
	    clearInterval(updateStatusToDB);
	  });
	});
	}
  };


  Drupal.behaviors.expInitJWPlayer =  {
    attach: function (context, settings) {
      //alert('In exp_sp_learning.js : Drupal.behaviors.expInitJWPlayer attach()');
      $('.exp-jwplayer:not(.exp-jwplayer-initialized)').addClass('exp-jwplayer-initialized').each(function () {
        //alert($(this).data('exp-jwplayer-settings'));
        var jwPlayerSettings = eval('(' + $(this).data('exp-jwplayer-settings') + ')');
        jwPlayerSettings.events = {
        		onReady : function() {
        			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('modal-content');/*this.play();*/
        			// console.log(this);
//        			this.seek(100);
        			expjwPlayerSettings = this;
        		},
        		onError : function(e) {
        			$('.exp-jwplayer-container').hide();
        			$('.no-video-found').show();
        		},
        		onComplete: function() {
					// console.log("video completed!");
				}
        };
        var initDivId = $(this).attr('id');
        //alert ('In exp_sp_learning.js : Drupal.behaviors.expInitJWPlayer attach() : id = ' + initDivId);
        if (typeof jwplayer === "undefined") {
          //alert("In exp_sp_learning.js : jwplayer is undefined");
          $.getScript("/sites/all/modules/core/exp_sp_core/js/jwplayer/jwplayer.js",
              function() {
                jwplayer(initDivId).setup(jwPlayerSettings);
              }
          );
        }
        else {
          //alert("In exp_sp_learning.js : jwplayer is already defined");
          jwplayer(initDivId).setup(jwPlayerSettings);
        }
        if(document.getElementById('launch-lp-wizard')){
        	$('#launch-lp-wizard').parent().css('display', 'none');
        }
        else if(document.getElementById('launch-wizard')){
        	$('#launch-wizard').parent().css('display', 'none');
        }
      });
    }
  };

  Drupal.behaviors.expInitJWPlayerYouTube =  {
    attach: function (context, settings) {
      //alert('In exp_sp_learning.js : Drupal.behaviors.expInitJWPlayerYouTube attach()');
      $('.exp-jwplayer-yt:not(.exp-jwplayer-yt-initialized)').addClass('exp-jwplayer-yt-initialized').each(function () {
        //alert($(this).data('exp-jwplayer-yt-settings'));
        var jwPlayerSettings = eval('(' + $(this).data('exp-jwplayer-yt-settings') + ')');
        jwPlayerSettings.events = {
        		onReady : function() {
        			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('modal-content');/*this.play();*/
					// console.log(this);
       			// this.seek(100);
					expjwPlayerSettings = this;
        			},
    			onError : function(e) {
    				// console.log("video error!");
        		},
        		onComplete: function() {
					// console.log("video completed!");
				}
        };
        var initDivId = $(this).attr('id');
        //alert ('In exp_sp_learning.js : Drupal.behaviors.expInitJWPlayerYouTube attach() : id = ' + initDivId);
        if (typeof jwplayer === "undefined") {
          //alert("In exp_sp_learning.js : jwplayer is undefined");
          $.getScript("/sites/all/modules/core/exp_sp_core/js/jwplayer/jwplayer.js",
              function() {
                jwplayer(initDivId).setup(jwPlayerSettings);
              }
          );
        }
        else {
          //alert("In exp_sp_learning.js : jwplayer is already defined");
          jwplayer(initDivId).setup(jwPlayerSettings);
        }
      });
    }
  };
	}catch(e){
		// to do
		// console.log(e);
	}
});

(function($) {
	$.fn.getNewEsignPopup = function(esignObj){
	try{
	var obj = new Object;//this;
	if(typeof(esignObj) == "string"){
		$.fn.getNewEsignPopup.esignObj = $.parseJSON(esignObj);
	}else{
		$.fn.getNewEsignPopup.esignObj = esignObj;
	}
	url = resource.base_url+'/?q='+"ajax/learningcore/getConcatenatedUserDets";
	$.ajax({
		type: "POST",
		url: url,
		//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
		success: function(details){
			obj.esignObj = $.fn.getNewEsignPopup.esignObj;
		    $('#esign-wizard , #esign-wizard-dialog').remove(); // esign-wizard-dialog Removed For this Issue #0038827
		    var html = "";
		    html += '<div id = "esign-wizard" class="esign-wizard-class">';
		    html += '<div id="esign-pass-error"><span class="esign-span-error">'+details[0].password_valid_txt+'</span></div>';
		    html += '<div id="esign-mandatory-error"><span class="esign-span-error">'+details[0].password_req_txt+'</span></div>';
		    html += '<div class="esign-msg-close-btn" onclick="$(\'body\').data(\'learningcore\').closeErrorMessage();"></div>';
		    html += '<div id="esign-message">'+details[0].esign_static_message+'</div>';
		    html += '<table class="esign-table" cellpadding="3" cellspacing="0" border="0" valign="center">';
		   // html += '<tr><td><table cellpadding="4" cellspacing="0" border="0" valign="center">';
		    html += '<tr><td class="esign-labels esign-field-label">'+details[0].esign_date_txt+' : </td><td class="esign-label-values esign-label-values-container">'+details[0].curr_date_time+'&nbsp;';
			html += details[0].time_zone+'</td></tr>';
			html += '<tr><td class="esign-field-label esign-field-label-name">'+details[0].esign_name_txt+' : </td><td class="esign-label-values esign-field-label-name">'+details[0].full_name+'</td></tr>';
			//html += '</table>';

			//html +='<table cellpadding="4" cellspacing="0" border="0" valign="center">';
			html +='<tr><td class="esign-field-label">'+details[0].esign_uname_txt+' :</td>';
			html +='<td class="esign-label-values"><input id="esign-uname" tabindex="1001" type ="text" disabled="true" class="readonly-text" value="'+details[0].user_name+'" maxlength = "60" size ="32" readonly></td>';
			html +='</tr>';
			html +='<tr><td class="esign-field-label">'+details[0].esign_pass_txt+' :<span class="require-text">*</span></td>';
			html +='<td class="esign-label-values"><input id="esign-pass" tabindex="1002" type ="password" name = "'+SMARTPORTAL.t('Password')+'" maxlength = "60" size ="32"></td>';
			html +='</tr>';
			html +='</table>';
			//html +='</td></tr></table>';
			html +='</div>';
		    $("body").append(html);
		    var closeButt={};
		    closeButt[details[0].esign_cancel_txt]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#esign-wizard').remove();
		    if(esignObj){
		    	if(obj.esignObj.esignFor == "AddAdmin"){
		    		$("body").data("learningcore").deleteDrupalUsers(obj.esignObj.drupalUserId);
				}else if(obj.esignObj.esignFor == "contentAuthorPresentationInteractions") 
				{
					//$("#h5p_presentation_frame").contents().find("#h5p-content-node-form").attr("esigndisplayed","off");
					$(".h5p-editor-iframe").attr("esigndisplayed","off");
				}else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit" || obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_publish" || obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_unpublish"){
							$(".h5p-editor-iframe").attr("esigndisplayed","off");
					}
				else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new" || obj.esignObj.esignFor == "contentAuthorAddInteractions_new_publish" || obj.esignObj.esignFor == "contentAuthorAddInteractions_new_unpublish"){
							$("#exp-sp-administration-contentauthor-addedit-form").attr("esigndisplayed","off");
					}
				else if(obj.esignObj.esignFor == "attachCrsDelete"){
				
				}
		    	$("div#"+obj.esignObj.popupDiv).show();
		    }
		    };
		    closeButt[details[0].esign_sign_txt]=function(){ $("body").data("learningcore").getNewEvaluateLogin();  };

		    $("#esign-wizard").dialog({
		        position:[(getWindowWidth()-400)/2,200],
		        bgiframe: true,
		        width:348,
		        resizable:false,
		        modal: true,
		        title: details[0].esign_title,
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });

		    $('#esign-wizard').parent('.ui-dialog').wrap("<div id='esign-wizard-dialog'></div>");

			 setTimeout(function () {
		    	  $('#esign-pass').focus();
		    	}, 10);

			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr('tabindex','1003');
			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('tabindex','1004');
			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('id','esign-sign-button');

			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			$('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
		    $('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg esign-admin-save-button-left-bg"></div>');
			$('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
			$('#esign-wizard-dialog  .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		    $('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("esign-button-wizard").end();
		    $('#esign-wizard-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("esign-close-button").end();

		    if(esignObj){
		    	$("div#"+obj.esignObj.popupDiv).hide();
		    }
		    $("#esign-wizard").show();
		    this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
		 	changeChildDialogPopUI('esign-wizard-dialog');
		    }
		 	//when add/attach content E-Sign pop up gets hidden behind the Add content pop up #0030278 And z-index Increased For this Ticket #0040188 and #0040178
		 	$('#esign-wizard').parent('.ui-dialog').parent('#esign-wizard-dialog').prev('#esign-wizard-dialog').remove();
		    $("#esign-wizard-dialog").find('.ui-dialog').css('z-index','60011');
		    $("#esign-wizard-dialog").next('.ui-widget-overlay').css({'z-index':'60010','position':'fixed'});

		   $('.ui-dialog-titlebar-close').click(function(){
		        $("#esign-wizard").remove();
		        if(esignObj){
				    if(obj.esignObj.esignFor == "AddAdmin"){
				    	$("body").data("learningcore").deleteDrupalUsers(obj.esignObj.drupalUserId);
				    }else if(obj.esignObj.esignFor == "contentAuthorPresentationInteractions") 
					{
						//$("#h5p_presentation_frame").contents().find("#h5p-content-node-form").attr("esigndisplayed","off");
						$(".h5p-editor-iframe").attr("esigndisplayed","off");
						
					}
				    else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_edit"  || obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_publish" || obj.esignObj.esignFor == "contentAuthorAddInteractions_edit_unpublish"){
				    	$(".h5p-editor-iframe").attr("esigndisplayed","off");
					}
				    else if(obj.esignObj.esignFor == "contentAuthorAddInteractions_new" || obj.esignObj.esignFor == "contentAuthorAddInteractions_new_publish" || obj.esignObj.esignFor == "contentAuthorAddInteractions_new_unpublish"){
							$("#exp-sp-administration-contentauthor-addedit-form").attr("esigndisplayed","off");
					}
				    
		        	$("div#"+obj.esignObj.popupDiv).show();
		        }
		    });
			$("#esign-pass").keyup(function(event){
			    if(event.keyCode == 13){
			        $("#esign-sign-button").click();
			    }
			});

			//Chrome browser dialog box UI fixes
			//var getScreenWidth = $(window).width()/4;
			//$("#esign-wizard-dialog").find('.ui-dialog').css('left',Math.round(getScreenWidth)+'px');

		}
	});
	return false;
	}catch(e){
		// to do
	}
};
})(jQuery);
function addslashes(str) {
	  return (str + '')
	    .replace(/[\\"']/g, '\\$&')
	    .replace(/\u0000/g, '\\0');
	}
function paintContentStatus(launchInfo, courseId, classId, elementId,lessonCnt,enroll_progress) {
	try{
//	console.log(Drupal.settings.convertion.mylearn_version);
	 if(Drupal.settings.convertion.mylearn_version==0){
	  //  console.log(launchInfo); 
	    var crsClassId      = courseId + '_' + classId
	    var attemptDiv      = 'attempt_left_' + crsClassId;
	    var validityDiv     = 'content_validity_' + crsClassId;
	    var progressDiv     = 'content_progress_' + crsClassId;
//	    console.log('attemptDiv: ' +attemptDiv);
//	    console.log('validityDiv: ' +validityDiv);
	    if (launchInfo.triggering_lesson_details != undefined) {  // content update from launch.js
	    	var isMultiContent  = (lessonCnt > 1) ? true : false;
	        var attemptLeft     = launchInfo.triggering_lesson_details.AttemptLeft;
	        var ValidityDays    = launchInfo.triggering_lesson_details.ValidityDays;
	        var remDays    		= launchInfo.triggering_lesson_details.remDays;
	        var IsLaunchable    = launchInfo.triggering_lesson_details.IsLaunchable;
	        var LaunchType      = launchInfo.triggering_lesson_details.LaunchType;
	        var SuspendData     = launchInfo.triggering_lesson_details.SuspendData;
	    } else {
	        var isMultiContent  = (launchInfo.length > 1) ? true : false;
	        var attemptLeft     = launchInfo[0].AttemptLeft;
	        var ValidityDays    = launchInfo[0].ValidityDays;
	        var remDays    		= launchInfo[0].remDays;
	        var IsLaunchable    = launchInfo[0].IsLaunchable;
	        var LaunchType      = launchInfo[0].LaunchType;      
	        var SuspendData     = launchInfo[0].SuspendData;     
	        
	    }
//	    console.log('attemptLeft' + attemptLeft);
//	    console.log('ValidityDays' + ValidityDays);
//	    console.log('remDays' + remDays);
	    if (attemptLeft != 'notset'){
	       var attemptHtml      = parseInt(attemptLeft) + ' ' + Drupal.t('LBL202');
	       if(document.getElementById(attemptDiv)) {
	           $('#'+attemptDiv).html(attemptHtml);
	       } else {
	           var attemptFullHtml = '<div class="course-attempt-left"><span id="attempt_left_' + crsClassId + '" class="attempts_left">' + attemptHtml + '</span></div>';
	           if (elementId != undefined && IsLaunchable && !isMultiContent) {
	                $('#'+elementId).parents('.top-record-div').append(attemptFullHtml);
	           }
	       }
	    }
	    var remValidityDays = ValidityDays - remDays;
	    if (ValidityDays != '' && remValidityDays !== undefined) {
	    	remValidityDays = parseInt(remValidityDays);
	       var dayString = (remValidityDays == 1) ? Drupal.t('LBL910') : Drupal.t('LBL605');
	       var ValidityHtml     =  parseInt(remValidityDays) + ' ' + dayString + ' ' + Drupal.t('LBL604');
	       if(document.getElementById(validityDiv)) {
	           $('#'+validityDiv).html(ValidityHtml);
	       } else {
	           var validityFullHtml = '<div class="course-content-validity yyyyy">';
	           if (attemptLeft != 'notset') {
	                validityFullHtml += '<span id="content_concat_str_' + crsClassId + '" class="content_concat_string">' + Drupal.t('LBL647') + '</span>';
	           }
	           validityFullHtml += '<span id="content_validity_' + crsClassId + '" class="content_validity">' + ValidityHtml + '</span></div>'; 
	           if (elementId != undefined && IsLaunchable && !isMultiContent) {
	                $('#'+elementId).parents('.top-record-div').append(validityFullHtml);
	           }
	       }       
	    }
	           
	       if(document.getElementById(progressDiv)) {
				var suspend_data = (SuspendData != null && SuspendData != "" && SuspendData != undefined) ? JSON.parse(unescape(SuspendData)) : null;
				var progress = (suspend_data != null && suspend_data != undefined) ? suspend_data['progress'] : 0;
				var progress = (progress == null || isNaN(progress)) ? 0 : progress;
				var progress_data = parseInt(progress) + '% ' + Drupal.t('Completed');
				$('#'+progressDiv).html(progress_data);
	       }else{
	       
	          if(LaunchType == "VOD" && !isMultiContent){       
		            progressFullHtml = '<div class="course-content-validity">';     
		            if ((ValidityDays != '' && remValidityDays !== undefined) || attemptLeft != 'notset') {   
		           	  progressFullHtml += '<span id="content_concat_str_' + crsClassId + '" class="content_concat_string">' + Drupal.t('LBL647') + '</span>';
		          	}
		        	var suspend_data = (SuspendData != null && SuspendData != "" && SuspendData != undefined) ? JSON.parse(unescape(SuspendData)) : null;
					var progress = (suspend_data != null && suspend_data != undefined) ? suspend_data['progress']  : 0;
					progress = isNaN(progress) ? 0 : progress;
					progressFullHtml += '<span id="content_progress_' + crsClassId + '" class="content_validity">' + progress + '% ' + Drupal.t('Completed')+ '</span></div>'; 
	           if (elementId != undefined && IsLaunchable && !isMultiContent) {
	                	$('#'+elementId).parents('.top-record-div').append(progressFullHtml);
	           }
	       }
	    }
	       if(document.getElementById("learning-plan-details-display-content")){
				 var urlpro = resource.base_url+'/?q='+'ajax/get-progress/'+enroll_progress;
				 $.ajax({
						type: "POST",
						url: urlpro,
						data:  {updateFrom: 'TP'},
						success: function(result){
							var res_arr = result.split('*~*');
							progressBarRound(res_arr[2],res_arr[1], 'enr_progress','prm_progress_',$("#lnr-catalog-search").data("contentPlayer"));
							//progressbarlpdetail(res_arr[2],res_arr[1], 'enr_progress','prm_progress_');
							//change the action button label
							var pro_class = $('#prm_progress_'+res_arr[2]).attr('class');
							var cls_arr = pro_class.split(' ');
							var int_cls_arr = cls_arr[1].split('_');
							if(res_arr[3] == 'lrn_tpm_ovr_cmp'){
								$('#object-registerCls_'+int_cls_arr[2]).html('<div>'+Drupal.t("Completed")+'</div>');
							}
							if(res_arr[3] == 'lrn_tpm_ovr_inc'){
								$('#object-registerCls_'+int_cls_arr[2]).html('<div>'+Drupal.t("Registered")+'</div>');
							}
						}
				 });
			}
	       if($("#select-class-dialog #gview_tbl-paintCatalogContentResults").length) { 
				$("body").data("learningcore").refreshJScrollPane("#select-class-dialog #gview_tbl-paintCatalogContentResults");
		   }
	}else{
		var reg_pro_arr = enroll_progress.split('*~*');
		var progress = 0;
		var enroll_id = reg_pro_arr[0];
		if(reg_pro_arr[1]!='' && reg_pro_arr[1] !=undefined){
			if(reg_pro_arr[1]> 0 && reg_pro_arr[1]<=1){
			   progress = reg_pro_arr[1];
			 }
		}
		var launch_status = reg_pro_arr[2];
		if(document.getElementById("paindContentclsid_"+classId)){
			$('#paindContentclsid_'+classId).attr('alt',enroll_id);
			var oldprogressid = $('.progress_'+classId).attr('id');
			var opid_arr = oldprogressid.split('_');
			if(opid_arr[1]==undefined || typeof opid_arr[1] == 'undefined')
				$('.progress_'+classId).attr('id', 'progress_'+enroll_id);
			else if(opid_arr[1]!=enroll_id){
				$('.progress_'+classId).html('');
				$('.progress_'+classId).attr('id', 'progress_'+enroll_id);					
			}
			$('.paindContentclsid_'+classId).attr('id', 'paindContentResults_'+enroll_id);
			setTimeout(function(){
				if(launch_status==1 || launch_status==undefined || typeof launch_status == 'undefined'){
					if(document.getElementById("course-details-display-content")){
						$('#class_content_more_'+classId).hide();
						$('#class_content_moredetail_'+classId).show();
						$('#paindContentclsid_'+classId).click();
					}else if(document.getElementById("class_detail_content")){
						$('#paindContentclsid_'+classId).click();
					}else if(document.getElementById("learning-plan-details-display-content")){
						$('#paindContentclsid_'+classId).click();
					}
				}
				if(launch_status==0){
					$('#class_content_moredetail_'+classId+' .class-content-wrapper').remove();
				}
				else
					$('.progress').show();
				if(document.getElementById("learning-plan-details-display-content")){
					 var urlpro = resource.base_url+'/?q='+'ajax/get-progress/'+enroll_id;
					 $.ajax({
							type: "POST",
							url: urlpro,
							data:  {updateFrom: 'TP'},
							success: function(result){
								var res_arr = result.split('*~*');
								progressBarRound(res_arr[2],res_arr[1], 'enr_progress','prm_progress_',$("#lnr-catalog-search").data("contentPlayer"));
								//change the action button label
								var pro_class = $('#prm_progress_'+res_arr[2]).attr('class');
								var cls_arr = pro_class.split(' ');
								var int_cls_arr = cls_arr[1].split('_');
								if(res_arr[3] == 'lrn_tpm_ovr_cmp'){
									$('#object-registerCls_'+int_cls_arr[2]).html('<div>'+Drupal.t("Completed")+'</div>');
								}
								if(res_arr[3] == 'lrn_tpm_ovr_inc'){
									$('#object-registerCls_'+int_cls_arr[2]).html('<div>'+Drupal.t("Registered")+'</div>');
								}
								$("#learning-plan-details-display-content").data("lnrplandetails").renderCoursesList(1);	
							}
					 });
				}
				progressBarRound(enroll_id,progress, 'enr_progress','progress_',$("#lnr-catalog-search").data("contentPlayer"));
				//$("#lnr-catalog-search").data("contentPlayer").progressbardetail(enroll_id,progress, 'enr_progress','progress_');
			},100);
		}
	}
	 if(('#selCompleteBy_'+classId).length>0){
		 $('#selCompleteBy_'+classId).show();
	 }
	}catch(e){
		// To Do
	}
}
function registeredToLaunch(launchInfo_arr,userId,classId,courseId,del_type,enrolled_id,action_status,pre_ass_status)
{
	try{
	if(userId!=0)
	   if(action_status == 'Registered' || action_status == Drupal.t('Registered')){
		   if(document.getElementById("registerCls_"+classId))
			 $("#registerCls_"+classId).html('');
			 var launchLen = launchInfo_arr.length;
			 if(launchLen == 1){
				 if(Drupal.settings.convertion.mylearn_version==1){
					 var launchInfo =  eval(launchInfo_arr[0]);
					 var launch_data = {
						masterEnrollId : 0,
						enrollId :	enrolled_id,
						courseId : courseId, 
						classId  : classId,
						userId : userId,
						LaunchFrom : 'CL',
						defaultContent : '',
						//classTitle:launchInfo["Title"],
						pagefrom : 'launch'
					};
						launch_data = objectToString(launch_data);
						var callLaunchUrlFn = "$('#lnr-catalog-search').data('contentPlayer').playContent(" + launch_data + ");";
						var onclickFun = "$('#lnr-catalog-search').data('contentPlayer').playContent(" + launch_data + ");";
				 }else{
				 	var data ='';
					for(i=0;i<launchLen;i++){
						var launchInfo =  eval(launchInfo_arr[0]);
						if (del_type == 'vod'){
				        	var title = launchInfo["Title"];
						    title = encodeURIComponent(title.replace(/\//g, '()|()'));
						    callLaunchUrlFn = "/?q=learning/nojs/play_video/";
						    callLaunchUrlFn += title + "/";
						    callLaunchUrlFn += escape(launchInfo["ContentSubTypeCode"]) + "/";
						    callLaunchUrlFn += escape(launchInfo["LearnerLaunchURL"].replace(/\//g, '()|()')) + '/';
						    callLaunchUrlFn += "ME" + "/";
						    callLaunchUrlFn += courseId + "/";
						    callLaunchUrlFn += classId + "/";
						    callLaunchUrlFn += launchInfo["ID"] + "/";
						    callLaunchUrlFn += launchInfo["VersionId"] + "/";
						    callLaunchUrlFn += enrolled_id + "/";
						    callLaunchUrlFn += escape(launchInfo["StatusCode"]);
			            }else{
			            	var onclickFun = "$('#lnr-catalog-search').data('contentLaunch').launchWBTContent({'LaunchFrom':'CL','Id':'"+launchInfo["ID"]+"','enrollId':'"+enrolled_id+"','VersionId':'"+launchInfo["VersionId"]+"','VersionNum':'"+launchInfo["VersionNum"]+"','url1':'"+launchInfo['LearnerLaunchURL']+"','courseId':'"+courseId+"','classId':'"+classId+"','url2':'"+launchInfo["PresenterLaunchURL"]+"','ErrMsg':''	,'contentType':'"+launchInfo["ContentType"]+"','Status':'"+launchInfo["Status"]+"','LessonLocation':'"+launchInfo["LessonLocation"]+"','launch_data':'"+launchInfo["LaunchData"]+"','suspend_data':'"+launchInfo["SuspendData"]+"','exit':'"+launchInfo["CmiExit"]+"','AICC_SID':'"+launchInfo["AICC_SID"]+"','MasteryScore':'"+launchInfo["masteryscore"]+"'});";
			            }
					    break;
					 }
				 }
			}else{
				 if(Drupal.settings.convertion.mylearn_version==1){
					 var launch_data = {
						masterEnrollId : 0,
						enrollId :	enrolled_id,
						courseId : courseId, 
						classId  : classId,
						userId : userId,
						LaunchFrom : 'CL',
						defaultContent : '',
					//	classTitle:'test',
						pagefrom : 'launch'
					};
						launch_data = objectToString(launch_data);
						var onclickFun = "$('#lnr-catalog-search').data('contentPlayer').playContent(" + launch_data + ");";
				 }else{
					var onclickFun ="$('#lnr-catalog-search').data('enrollment').launchMultiContent("+enrolled_id+",this);";
					var data ="[";
					for(i=0;i<launchLen;i++){
						var launchInfo =  launchInfo_arr[i];
						data +="[{'LaunchFrom':'CL','Id':'"+launchInfo["ID"]+"','ContentId':'"+launchInfo["ContentId"]+"','VersionId':'"+launchInfo["VersionId"]+"','VersionNum':'"+launchInfo["VersionNum"]+"','enrollId':'"+enrolled_id+"','Title':'"+addslashes(launchInfo["Title"])+"','ContentTitle':'"+addslashes(launchInfo["Code"])+"','url1':'"+launchInfo["LearnerLaunchURL"]+"','courseId':'"+courseId+"','IsLaunchable':'"+launchInfo["IsLaunchable"]+"','AttemptLeft':'"+launchInfo["AttemptLeft"]+"','ValidityDays':'"+launchInfo["ValidityDays"]+"','remDays':'"+launchInfo["remDays"]+"','daysLeft':'"+launchInfo["daysLeft"]+"','contValidateMsg':'"+launchInfo["contValidateMsg"]+"','classId':'"+classId+"','url2':'"+launchInfo["PresenterLaunchURL"]+"','ErrMsg':'','contentType':'"+launchInfo["ContentType"]+"','contentSubTypeCode':'"+launchInfo["ContentSubTypeCode"]+"','launchType':'"+launchInfo["LaunchType"]+"','ClsScore':'"+launchInfo["ClsScore"]+"','LesScore':'"+launchInfo["LesScore"]+"','ContScore':'"+launchInfo["ContScore"]+"','Status':'"+launchInfo["Status"]+"','ContentStatus':'"+launchInfo["ContentStatus"]+"','Lessoncnt':'"+launchInfo["Lessoncnt"]+"','LessonLocation':'"+launchInfo["LessonLocation"]+"','launch_data':'"+launchInfo["LaunchData"]+"','suspend_data':'"+launchInfo["SuspendData"]+"','exit':'"+launchInfo["CmiExit"]+"','AICC_SID':'"+launchInfo["AICC_SID"]+"','MasteryScore':'"+launchInfo["masteryscore"]+"','contentQuizStatus':'"+launchInfo["contentQuizStatus"]+"','ContentCompletionStatus':'"+launchInfo["ContentCompletionStatus"]+"','StatusCode':'"+launchInfo["StatusCode"]+"'}],";
					}
					data +="]";
			}
			}
			 //$("#registerCls_"+classId).attr('onclick',null);
			 if(document.getElementById("registerCls_"+classId)){
				 $("#registerCls_"+classId).removeAttr('onclick');
				 if(del_type == 'vod' && launchLen == 1){
					// $("#registerCls_"+classId).css({'line-height':'48px','width':'113px'});
					/* $("#registerCls_"+classId).css('position','absolute');
					 $("#registerCls_"+classId).css('height','40px');
					 $("#registerCls_"+classId).css('padding-top','15px');*/
					 if(parseInt(pre_ass_status) == 1){
						 $("#registerCls_"+classId).attr("alt", callLaunchUrlFn);
						 document.getElementById("registerCls_"+classId).setAttribute('onclick',"$('#lnr-catalog-search').data('lnrcatalogsearch').preAssMsgBox("+enrolled_id+",'sinVod');");
						 $("#registerCls_"+classId).html(Drupal.t('LBL199'));
						 $("#registerCls_"+classId).append('<a href="'+callLaunchUrlFn+'" id="dummylaunch'+enrolled_id+'" style="display:none;" class="enroll-launch use-ajax ctools-modal-ctools-video-style">dummy</a>');
					 }else{
						 $("#registerCls_"+classId).addClass(' enroll-launch use-ajax ctools-modal-ctools-video-style');
						 $("#registerCls_"+classId).attr("href", callLaunchUrlFn);
						 $("#registerCls_"+classId).html(Drupal.t('LBL199'));
					 }
					 Drupal.attachBehaviors($("#registerCls_"+classId));
					 var elementId = "registerCls_"+classId;
					 paintContentStatus(launchInfo_arr, courseId, classId, elementId,'',enrolled_id);
				 }else{
					 //console.log("registerCls_"+classId)
					 if( parseInt(pre_ass_status) == 1 && Drupal.settings.convertion.mylearn_version==0){
						 var alertfun = "$('#lnr-catalog-search').data('lnrcatalogsearch').preAssMsgBox("+enrolled_id+");";
						 var funCom = onclickFun+alertfun;
						 //$("#registerCls_"+classId).attr("onclick", alertfun);
						 document.getElementById("registerCls_"+classId).setAttribute('onclick',alertfun);
						 document.getElementById("registerCls_"+classId).setAttribute('alt',onclickFun);
					 }
					 else{
						 document.getElementById("registerCls_"+classId).setAttribute('onclick',onclickFun);
					 }
					 $("#registerCls_"+classId).attr("data",data);
					 $("#registerCls_"+classId).html(Drupal.t('LBL199'));
					 Drupal.attachBehaviors($("#registerCls_"+classId));
					 var elementId = "registerCls_"+classId;
                     paintContentStatus(launchInfo_arr, courseId, classId, elementId,'',enrolled_id);
				 }
				 $("#registerCls_"+classId).attr('id', 'launch'+enrolled_id);
			 }
            //Code commented as per vincent concern ontime only we give the launch offer.
			 if(document.getElementById('tbl-paintCatalogContentResults')){
					$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
					if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
						$("#paintContentResults").trigger("reloadGrid");
					}
			 }
	   }
	}catch(e){
		// To Do
	}
}

function remove_qtip(val){
	$(".active-qtip-div").remove();
}

function showUserPreferenceSettings(){
	try{
	
	$('#manage-dd-list').toggle();
	$("body").data("learningcore").showWidgetList();
	if($('#block-exp-sp-learning-tab-user-customization').hasClass('active')){
		$('#block-exp-sp-learning-tab-user-customization').removeClass('active');
	}else{
		$('#block-exp-sp-learning-tab-user-customization').addClass('active');
	}
	if(!$('#block-exp-sp-learning-tab-user-customization').hasClass('active')){
	     $('#block-exp-sp-learning-tab-user-customization').removeClass('active');
	     $('#block-exp-sp-learning-tab-user-customization').height("");
	     }
	}catch(e){
			// To Do
	}
}
function closeUserPreference() {
	$('#manage-dd-list').css('visibility','hidden');
}
function disableWidgetDeleteAction(elementId) {
	try {
		if (document.getElementById(elementId))
			$("#"+elementId).find('.widget-delete-button').addClass('inactiveLink');
	} catch (e){

	}
}
function activateWidgetDeleteAction(elementId) {
	try {
		if (document.getElementById(elementId))
			$("#"+elementId).find('.widget-delete-button').removeClass('inactiveLink');
	} catch (e){

	}
}
function createSortableList(element) {
	try {
		$(element).sortable({
	        axis: 'y',
	        //containment: 'parent',
	        connectWith: element,
	        forceHelperSize: true,
	        forcePlaceholderSize: true,
	        handle: '.block-title',
	        cancel : '.filter-block',
	        opacity: 0.9,
	        zIndex: 2000,
	        //tolerance: 'pointer',
	        placeholder: 'placeholder',
	        helper: "original",
	        sort: function(event, ui) {
	        	if(Drupal.settings.mylearning_right===false){
	        		$(".ui-sortable-helper #block-exp-sp-lnrenrollment-tab-my-enrollment-customized," +
	        			".ui-sortable-helper #block-exp-sp-lnrlearningplan-tab-my-learningplan-customized," +
	        			".ui-sortable-helper #block-exp-sp-instructor-desk-tab-instructor-desk-customized").css('width', '942px');
	           	} else {
	        		$(".ui-sortable-helper #block-exp-sp-lnrenrollment-tab-my-enrollment-customized," +
	            		".ui-sortable-helper #block-exp-sp-lnrlearningplan-tab-my-learningplan-customized," +
	            		".ui-sortable-helper #block-exp-sp-instructor-desk-tab-instructor-desk-customized").css('width', '658px');
	        	}
	        },
			start: function(event, ui) {
	        },
	        stop: function(e, ui) {
	        	$("#block-exp-sp-lnrenrollment-tab-my-enrollment-customized, #block-exp-sp-lnrlearningplan-tab-my-learningplan-customized, #block-exp-sp-instructor-desk-tab-instructor-desk-customized").removeAttr('style');
	        },
	        update: function(event, ui){
	            var changedList = this.id;
	            var order = $(this).sortable('toArray');
	            var positions = order.join(',');
	        	var params = {id: changedList, positions: positions};
	            $.ajax({
					type: "POST",
					url: '/?q=user-preference/update',
					data:  params,
					dataType: 'json',
					success: function(result){
						if (result.session != undefined && result.session == 'session_out') {
							self.location='?q=learning/enrollment-search';
							return;
						}
					}
				});
	        }
	  	});
	}catch(e){
		// to do
	}
}
function getVideoSuspendData() {
	videoTrackerProgress = {
	session_id: session_id,
	current_position: ytPlayer.getCurrentTime(),
	progress: progress,
	video_duration: ytPlayer.getDuration(),
	additional_data: {video_session_id: session_id},
	content_type : 'youtube'
	};  
}
$('body').bind('click', function(event) {
	 try{
		  if(event.target.id != 'user-preference-settings'){
			  $('#manage-dd-list').css('display','none');
		  }
	 }catch(e){
			// to do
}
});
$(document).ready(function() {
	try{
	  $("#cart-form-pane #cart-form-products .sticky-table tr th:last-child").css("border-left"," 0px");
	  /*-- My learning page widget customization --*/
	  if (Drupal.settings.salesforce.type != "iframe") {
		  createSortableList(".region-sidebar-second .sortable-list");
		  createSortableList(".region-highlight .sortable-list");
	  }
	  //52001: Change class pop not showing when we click on change class link in action link button in My Enrollment panel
	  if(document.getElementById('tab_my_learningplan')==null || document.getElementById('tab_my_learningplan')==undefined){
		  //52687: Course after completion is not moving to completed status.Launch button is still retailed in Catalog Page.
		    // added the parent class bcz of above issue.
			$(".page-learning-enrollment-search #page-container" ).append( '<div id="learningplan-tab-inner" class="dummyelement_tab_my_learningplan" style="display:none;"></div>' );
			Drupal.attachBehaviors('learningplan-tab-inner');
			$( "#learningplan-tab-inner" ).learningplan();
		}
	  if(document.getElementById('tab_my_enrollment')==null || document.getElementById('tab_my_enrollment')==undefined){
			$(".page-learning-enrollment-search #page-container" ).append( '<div id="learner-enrollment-tab-inner" class="dummyelement_tab_my_enrollment" style="display:none;"></div>' );
			Drupal.attachBehaviors('learner-enrollment-tab-inner');
			$("#learner-enrollment-tab-inner").enrollment();
			$("#learner-enrollment-tab-inner").contentLaunch();
		}
	  if(document.getElementById('tab_my_learningplan') !== undefined && $("#block-take-survey" ).data('surveylearner') === undefined) {
		  $('<div id="block-take-survey"></div>').insertAfter("#learningplan-tab-inner");
		  Drupal.attachBehaviors('block-take-survey');
		  $("#block-take-survey").surveylearner();
	  }
	if(Drupal.settings.mylearning.user.survey_urlname == 'surveylink') {
		setTimeout(loadSurveyLink,2000);
	}

	  /*-- My learning page widget customization --*/
	}catch(e){
		// to do
	}
});

function addClearIcon(inputElem){
	try {
		var input = inputElem.attr('id');
		var width = inputElem.width();
		inputElem.width(width - 18);
		width = inputElem.width();
		$('<div class="eol-search-clearance"><span id="searchclear" onmousedown="clearInput(this, \''+input+'\')"></span></div>').insertAfter(inputElem)
		.css({
			'margin-left': width+'px',
			'display': 'block'
		});
	} catch(e) {
		// window.console.log(e, e.stack);
	}
}

function removeClearIcon(inputElem) {
	try {
		if(inputElem.siblings('.eol-search-clearance').length > 0) {
			var width = inputElem.width();
			inputElem.width(width + 18);
			width = inputElem.width();
			inputElem.siblings('.eol-search-clearance').remove();
		}
	} catch(e) {
		
		// window.console.log(e, e.stack);
	}
}

function clearInput(_this, input) {
	try {
		var inputElem = $('#'+input);
		inputElem.val('');
		removeClearIcon(inputElem);
	} catch(e) {
		// window.console.log(e, e.stack);
	}
}

$("#search_searchtext, #forum_searchtext, #reports_searchtext, #myteam_searchtext, #narrow-search-calendar-filter").focus(function () {
	try {
		var inputElem = $(this);
		inputElem.val() == '' || addClearIcon(inputElem);
	} catch(e) {
		// window.console.log(e, e.stack);
	}
})
.blur(function(e) {
	try {
		var inputElem = $(this);
		removeClearIcon(inputElem)
	} catch(e) {
		// window.console.log(e, e.stack);
	}
}).keyup(function () {
	var inputElem = $(this);
	if(inputElem.siblings('.eol-search-clearance').length < 1) {
		inputElem.val() == '' || addClearIcon(inputElem);
	} else {
		inputElem.val() == '' && removeClearIcon(inputElem);
	}
});

function locationdetails(qtipObj,seestime){
 
	// convert the string to object
	if(typeof qtipObj == 'string'){
		var qtipObj =  unserialize(qtipObj);
	}
	$('.qtip-popup-visible').html('').hide();
		
	var cls_id = qtipObj.entityId;
	var popupId 	= qtipObj.popupDispId;
	var entId = qtipObj.entityId;
	var qTipOptions = {
		wid : qtipObj.wid,
		heg : qtipObj.heg,
		top:qtipObj.top,
		entityId : qtipObj.entityId,
		popupDispId : qtipObj.popupDispId,
		postype : qtipObj.postype,
		poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid,
		posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
		disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
		linkid	: qtipObj.linkid,
		dispDown : (qtipObj.dispDown == undefined) ? '': qtipObj.dispDown,
		beforeShow: function() {
			$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html($('#selSessionId-'+entId).html())
		}
	};
	
	if(qtipObj.entityId.indexOf("changeclass") != -1 
		|| qtipObj.entityId.indexOf("assign-learning-") != -1 
		|| qtipObj.entityId.indexOf("course-details-") != -1) {
		qTipOptions.beforeShow = function() {
			$("#wrapper-visible-popup-"+entId).remove();
			$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html($('#selSessionId-'+entId).html());
			$('#visible-popup-'+entId).css({'visibility': 'hidden'});
		};
		qTipOptions.afterShow = function() {
			setTimeout(function() {
				var offset = $('#'+qtipObj.linkid).offset();
				var newSpan = $('#visible-popup-'+entId).clone()
				.appendTo('body')
				.offset(offset)
				.zIndex(10010)
				.wrap('<div class="location-session-detail-clone" id="wrapper-visible-popup-'+entId+'"></div>')
				.css({'visibility': 'visible'});
				}, 10);
		};
	}
	$('#' + qtipObj.popupDispId).qtipPopup(qTipOptions);
	$('body').bind('click', function(event) {
		if((event.target.id != 'manage-location-time-'+cls_id+'' || event.target.id != 'manage-location-cal-'+cls_id+'')
				&& document.getElementById('paintContentlocation-session-details_'+cls_id) != null ){
			$('.qtip-popup-visible').html('').hide();
		}
	});
}	
function closeQtip(popupId,entId,onClsFn){
    try{
            if(typeof(onClsFn) == 'undefined') onClsFn='';
            if(popupId == '')
                    $('.qtip-popup-visible').html('').hide();
            else
                    $('#'+popupId+' #visible-popup-'+entId).html('').hide();
            
            if(typeof(onClsFn) == 'function') {
                    onClsFn();
            }
    }catch(e){
            
    }
}

function showClassDetailMore(classId,dType,cStatus) {
	try {
		var classId = (classId != undefined) ? classId : 0;
		//console.log("class id - "+classId);
		//console.log("type - "+dType);
		//console.log("status - "+cStatus);
		if (classId > 0) {
			if(document.getElementById('block-exp-sp-coursedetail-course-details'))
				$("#gbox_paint-classes-list #"+classId+" .content-description").find("#arrow-more").click();
			if(document.getElementById('block-exp-sp-learning-plan-detail-learning-details'))
				$(".learning-classes-details #"+classId+" .content-description").find("#arrow-more").click();
			$('#class_content_moredetail_' + classId).slideDown();
			$('#class_content_more_' + classId).hide();
			var enr_id = $('#paindContentclsid_classId').attr('alt');
			if((dType=='lrn_cls_dty_wbt'||dType=='lrn_cls_dty_vod') && ((cStatus=='lrn_crs_cmp_enr' ||  cStatus == 'lrn_crs_cmp_inp' || cStatus == 'lrn_crs_cmp_att') || (cStatus=='' && enr_id>0))){
				$('#class_content_moredetail_'+classId+' .class-content-wrapper').addClass('padbt10');
				if($('#class_content_moredetail_'+classId+' .class-content-wrapper').length >0){
					$('#paindContentclsid_'+classId).click();
				}
			}
			resetInstructorFadeout('#'+classId);
            $('.lmt-cls-desc-add').trunk8(trunk8.class_detail_add_desc);
		}
	} catch(e) {

	}
}

function resetInstructorFadeout(selector,callFrom){
	selector = (selector == null || selector == undefined) ? '' : selector;
	$(selector + ' .content-instructor-block').each(function(){
		var insCnt = $(this).find('.content-instructor-row').size();
		if(insCnt == 1 || (callFrom == 'class' && insCnt == 2)){ // Reset the max-width for instructors
			var w1 = $(this).width();
			w1 = (callFrom == 'class' && insCnt == 2) ? Math.ceil(w1/insCnt) : w1;
			var w2 = $(this).find('.avatar-image').width();
			var w3 = (w1-w2) - 20;
			var pw = (callFrom == 'class' && insCnt == 2) ? 50 : 100; 
			$(this).find('.content-instructor-row').css('width',pw+'%');
			$(this).find('.content-instructor-row .content-instructor-detail').css('max-width',w3+'px');
			$(this).find('.content-instructor-row .content-instructor-detail .class-detail-ins-details-name-fadeout-container').css('max-width',w3+'px');
			$(this).find('.content-instructor-row .content-instructor-detail .class-detail-ins-details-job-fadeout-container').css('max-width',w3+'px');
		}
		$(this).find('.content-instructor-detail .class-detail-ins-details-name-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
		$(this).find('.content-instructor-detail .class-detail-ins-details-job-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
	});

	$(selector + ' .session-details-warpper .session-instructor').each(function(){
		var insCnt = $(this).find('.class-detail-session-instructor-fadeout-container').size();
		var mWidth = $(this).find('.sess-attr-val').width();
		if(insCnt > 1){
			mWidth = Math.ceil(mWidth/insCnt)-10; // subtract the with by 10 for comma and a space between two names
			mWidth = mWidth <= 60 ? 58 : mWidth;
		}
		$(this).find('.class-detail-session-instructor-fadeout-container').css('max-width',mWidth+"px");
		$(this).find('.class-detail-session-instructor-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
	});
	
	$(selector + ' .session-details-warpper .session-name').each(function(){
		var insCnt = $(this).find('.class-detail-session-title-fadeout-container').size();
		var mWidth = $(this).find('.sess-attr-val').width();
		if(insCnt > 1){
			mWidth = Math.ceil(mWidth/insCnt)-10; // subtract the with by 10 for comma and a space between two names
			mWidth = mWidth <= 60 ? 58 : mWidth;
		}
		$(this).find('.class-detail-session-title-fadeout-container').css('max-width',mWidth+"px");
		$(this).find('.class-detail-session-title-fadeout-container').each(function() {
			if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
				$(this).find('.fade-out-image').remove();
			}
		});
	});
}

function showClassDetailLess(classId) {
	try {
		var classId = (classId != undefined) ? classId : 0;

		if (classId > 0) {
			if(document.getElementById('block-exp-sp-coursedetail-course-details'))
				$("#gbox_paint-classes-list #"+classId+" .content-description").find("#arrow-less").click();
			if(document.getElementById('block-exp-sp-learning-plan-detail-learning-details'))
				$(".learning-classes-details #"+classId+" .content-description").find("#arrow-less").click();
			$('#class_content_moredetail_' + classId).slideUp();
			setTimeout(function(){$('#class_content_more_' + classId).show();},300)
		}
	} catch(e) {
		
	}
}

function updateVideoProgress(vodPlayer) {
	if(vodPlayer === undefined || vodPlayer.duration() == 0) {
		return;
	}

	var percentage 	= (vodPlayer.currentTime() / vodPlayer.duration()) * 100;
	var progress 	= isNaN(percentage) == false ? percentage : 0;

	videoTrackerProgress = {
				session_id: vodPlayer.id_,
				current_position: vodPlayer.currentTime(),
				progress: progress,
				video_duration: vodPlayer.duration(),
				additional_data: {video_session_id: vodPlayer.id_}
			};
}
function updateVideoProgressToDB(response,setObj){
	if($('#video-container').length) {
		var launchedFrom 	= response.launchedFrom;
		var courseId 		= response.video_detail_arr[0]
		var	classId			= response.video_detail_arr[1]
		var	lessonId		= response.video_detail_arr[2]
		var	versionId		= response.video_detail_arr[3]
		var	enrollId		= response.video_detail_arr[4]
		var prevContentStatus = '';
		var launchflag      = 1;
		setObj.updateVODScoreOnCtoolsModalClose(launchedFrom, courseId, classId, lessonId, versionId, enrollId, prevContentStatus, launchflag); 
	}
}
                
function saveVideoProgress(vodPlayer, ended) {
	try {
		if((ended || (videojs(vodPlayer.id_) !== undefined && !vodPlayer.paused())) && vodPlayer.duration() != 0) {
			$.ajax({
				url:"?q=ajax/update-launch/vod",
				type: 'POST',
				data: videoTrackerProgress,
				success: function() {
					// console.log("Call is successful.");
				},
				error: function() {
					// console.log("Call is unsuccessful.");
				},
				complete: function() {
					// console.log("Server update complete.");
        }
      });
    }
	}
	catch(e) {
		// console.log(e);
	}
}
function loadSurveyLink(){
	$.cookie('SurUrlName', null);
	$.cookie('Surpath', null);
	var json_obj = Drupal.settings.mylearning.survey_data;
	var msg_string='';
	msg_string = (Drupal.settings.mylearning.user.survey_objecttype == 'cre_sys_obt_cls') ? Drupal.t('Class') : Drupal.t('Training Plan');
	
	var widgetname;
	if(Drupal.settings.mylearning.user.survey_objecttype == 'cre_sys_obt_cls' && Drupal.settings.mylearning.user.survey_dedicatetpcount < 1)
		widgetname = Drupal.t('MSG858');
	else
		widgetname = Drupal.t('MSG859');
	
	if(Drupal.settings.mylearning.user.survey_userwidget == 0) {
	if(Drupal.settings.mylearning.user.survey_count > 0) {
		if(Drupal.settings.mylearning.user.survey_enrollcount > 0) {
	
	if((Drupal.settings.mylearning.user.survey_recurringcount > 0) || ((Drupal.settings.mylearning.user.survey_recertify == 1) && (Drupal.settings.mylearning.user.survey_sharetype=='multiple')) || (Drupal.settings.mylearning.user.survey_multipleTP >= 1 && Drupal.settings.mylearning.user.survey_enrollcount > 1)) {
		 if(json_obj !== 'null') {
			 if(Drupal.settings.mylearning.user.survey_myenrolltab == 'Yes' || Drupal.settings.mylearning.user.survey_multipleTP >= 1) {
				 if($('#learning_filter_myenrollment_EnrollCompleted').parent().hasClass('checkbox-unselected')) {
					setTimeout(function(){
						$('#myenrollment-learner-menu #learning_filter_myenrollment_EnrollCompleted').click(); 
						selectedMenuFilter('myenrollment','EnrollCompleted','lrn_crs_cmp_cmp');
					}, 10);
					setTimeout(function(){statusFilterSubmit('myenrollment');}, 60);
				 }
			 } 
			 if(Drupal.settings.mylearning.user.survey_myprogramstab == 'Yes' || Drupal.settings.mylearning.user.survey_multipleTP >= 1) {
				 if($('#learning_filter_myprograms_EnrollLPCompleted').parent().hasClass('checkbox-unselected')){
				 	setTimeout(function(){
				 		$('#myprograms-learner-menu #learning_filter_myprograms_EnrollLPCompleted').click();
				 		selectedMenuFilter('myprograms','EnrollLPCompleted','lrn_tpm_ovr_cmp');
				 	}, 150);
				 	setTimeout(function(){statusFilterSubmit('myprograms');}, 200);
				 }
			 }
			 setTimeout(function(){
				 $('#learner-enrollment-tab-inner').data('enrollment').launchMultiSurvey(json_obj);
			 }, 1500);
			 	return;
		 } else
				 return;
		}
			

		if(Drupal.settings.mylearning.user.survey_sharetype == 'single' && Drupal.settings.mylearning.user.survey_enrollstatus == null ) {
			$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Not Registered')+' '+Drupal.t('LBL102'));
		} else if (Drupal.settings.mylearning.user.survey_id == '' || Drupal.settings.mylearning.user.survey_id == null) {
			$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG850'));
		} else {

		switch(Drupal.settings.mylearning.user.survey_enrollstatus) {
			case 'lrn_crs_cmp_inc': 
			case 'lrn_tpm_ovr_inc':	
			case 'lrn_crs_cmp_nsw':
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Incomplete')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_reg_can': 
			case 'lrn_tpm_ovr_cln':
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Canceled')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_reg_rsv':
			case 'lrn_tpm_ovr_rsv':
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('LBL942')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_reg_rsc':	
			case 'lrn_tpm_ovr_rsc':
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('LBL942')+Drupal.t('Canceled')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_cmp_exp': 
			case 'lrn_tpm_ovr_exp':	
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Expired')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_tpm_ovr_wtl':
			case 'lrn_crs_reg_wtl':
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('LBL138')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_reg_ppm': 
			case 'lrn_crs_reg_ppv':
			case 'lrn_tpm_ovr_ppv':
			case 'lrn_tpm_ovr_ppm': 
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Pending')+' '+Drupal.t('LBL102'));
				break;
			case 'lrn_crs_cmp_cmp':
			case 'lrn_crs_cmp_enr':
			case 'lrn_crs_cmp_inp':
			case 'lrn_tpm_ovr_cmp':
			case 'lrn_tpm_ovr_enr':
			case 'lrn_tpm_ovr_inp':
				if(Drupal.settings.mylearning.user.survey_status == 0 || (Drupal.settings.convertion.mylearn_version==1 && Drupal.settings.mylearning.user.survey_completion !== 'ALL')) {
					if(Drupal.settings.convertion.mylearn_version==1) { 
						if(Drupal.settings.mylearning.user.survey_objecttype=='cre_sys_obt_cls' && Drupal.settings.mylearning.user.survey_dedicatetpcount < 1) {
							 if(Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_crs_cmp_cmp' && ($('#learning_filter_myenrollment_EnrollCompleted').parent().hasClass('checkbox-unselected'))) {
										$('#myenrollment-learner-menu #learning_filter_myenrollment_EnrollCompleted').click(); 
										selectedMenuFilter('myenrollment','EnrollCompleted','lrn_crs_cmp_cmp');
									statusFilterSubmit('myenrollment');
							 }	
							 setTimeout(function(){callContentPlayer('');}, 1800);
						} else {
							if((Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_crs_cmp_cmp' || Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_tpm_ovr_cmp') && ($('#learning_filter_myprograms_EnrollLPCompleted').parent().hasClass('checkbox-unselected'))){
								setTimeout(function(){
							 		$('#myprograms-learner-menu #learning_filter_myprograms_EnrollLPCompleted').click();
							 		selectedMenuFilter('myprograms','EnrollLPCompleted','lrn_tpm_ovr_cmp');
							 	}, 10);
							 	setTimeout(function(){statusFilterSubmit('myprograms');}, 60);
							 }
							setTimeout(function(){callContentPlayer('');}, 1800);
							}
							} else {
						if(Drupal.settings.mylearning.user.survey_objecttype=='cre_sys_obt_cls' && Drupal.settings.mylearning.user.survey_dedicatetpcount < 1) {
							if(Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_crs_cmp_cmp' && ($('#learning_filter_myenrollment_EnrollCompleted').parent().hasClass('checkbox-unselected'))) {
								setTimeout(function(){
									$('#myenrollment-learner-menu #learning_filter_myenrollment_EnrollCompleted').click(); 
									selectedMenuFilter('myenrollment','EnrollCompleted','lrn_crs_cmp_cmp');
								}, 10);
								setTimeout(function(){statusFilterSubmit('myenrollment');}, 60);
							}
							setTimeout(function(){
								$("#block-take-survey").data("surveylearner").callTakeSurveyToClass(Drupal.settings.mylearning.user.survey_objectid,""+decodeURIComponent(Drupal.settings.mylearning.user.survey_objectname)+"",""+Drupal.settings.mylearning.user.survey_objecttype+"","survey","",Drupal.settings.mylearning.user.survey_enrollid,"NULL","enroll-result-container");
							}, 600);
							setTimeout(function(){
								$("#block-take-survey").data("surveylearner").moveNext(Drupal.settings.mylearning.user.survey_id);
							}, 650);
						} else {
							if((Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_crs_cmp_cmp' || Drupal.settings.mylearning.user.survey_enrollstatus == 'lrn_tpm_ovr_cmp') && ($('#learning_filter_myprograms_EnrollLPCompleted').parent().hasClass('checkbox-unselected'))){
								setTimeout(function(){
							 		$('#myprograms-learner-menu #learning_filter_myprograms_EnrollLPCompleted').click();
							 		selectedMenuFilter('myprograms','EnrollLPCompleted','lrn_tpm_ovr_cmp');
							 	}, 10);
							 	setTimeout(function(){statusFilterSubmit('myprograms');}, 60);
							}
							setTimeout(function(){
								$("#block-take-survey").data("surveylearner").callTakeSurveyToClass(Drupal.settings.mylearning.user.survey_objectid,""+decodeURIComponent(Drupal.settings.mylearning.user.survey_objectname)+"",""+Drupal.settings.mylearning.user.survey_objecttype+"","survey","",Drupal.settings.mylearning.user.survey_enrollid,"NULL","enroll-lp-result-container");
							}, 600);
							setTimeout(function(){
								$("#block-take-survey").data("surveylearner").moveNext(Drupal.settings.mylearning.user.survey_id);
							}, 650);
						}	
					}
			} else {
				$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG394'));
			}
		}
		}
	} else {
		$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG849')+' '+msg_string+' '+Drupal.t('MSG847')+' '+Drupal.t('Not Registered')+' '+Drupal.t('LBL102'));
	}
			
	} else {
		$("body").data("learningcore").callMessageWindow('Survey',Drupal.t('MSG850'));
	}
	
	} else {
		$("body").data("learningcore").callMessageWindow('Survey',widgetname);
	}
	$('#select-class-equalence-dialog .ui-dialog').css({'width':'auto', 'min-width':'394px'});
	$('#select-class-equalence-dialog .ui-dialog #commonMsg-wizard tr td:first').height('27');
	$('.ui-dialog #commonMsg-wizard').css('min-height','63px');
	$('.ui-dialog #commonMsg-wizard span.select-greyed-out-text').css({'width': 'auto', 'padding': '0px 16px 16px'});
	$('.ui-dialog .ui-dialog-buttonpane .ui-dialog-buttonset').css('min-width','auto');
	$('.ui-dialog .ui-dialog-buttonpane .ui-dialog-buttonset .removebutton').css('margin-right','2px');
}


function RefreshFadeoutForSF(selector,divcont,linecont,fromPage){
 	   if($('.item-title-code').find('.title-lengthy-text').width()>0) {
        	resetFadeOutByClass(selector,divcont,linecont,fromPage);     
        }else{
           setTimeout(function(){RefreshFadeoutForSF(selector,divcont,linecont,fromPage);}, 100);
        }
}

function  Refreshtrunk8( fromPage, lcont) {
	   if($('.limit-title-row').find('.'+lcont).height()>0 ) {
		      if(fromPage=='myenrollments'){
		           $('.limit-title-enr').trunk8(trunk8.myenroll_title);
				   $('.limit-desc-enr').trunk8(trunk8.myenroll_desc);
			  }else if(fromPage=='myprograms'){
				   $('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
				   $('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
			  }
	    }else{
	           setTimeout(function(){Refreshtrunk8(fromPage,lcont );}, 100);
	    } 
}
function clickAndDisable(link) {
    // disable subsequent clicks
    link.onclick = function(event) {
       event.preventDefault();
    }
  }  