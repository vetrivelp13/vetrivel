(function($) {
	var dWidth='';

$.widget("ui.learningplan", {
	_init: function() {
		try{
		var self = this;
		var DrupalLocaleLP = {'failed' : Drupal.t('failed'),
				'passed' : Drupal.t('passed'),
				'unknown' : Drupal.t('unknown'),
				'incomplete': Drupal.t('Incomplete'),
				'completed': Drupal.t('Completed')
				};
		this.lpUserId = this.getLearnerId();
		$.ui.learningplan.parentAssessStatus = new Array();
		dWidth = $("#enroll-lp-result-container").width();
		if(dWidth==1000) {
			$('#block-exp-sp-lnrlearningplan-tab-my-learningplan').addClass('enroll-disable-right-region');
			$('#block-exp-sp-lnrlearningplan-tab-my-learningplan').removeClass('enroll-enable-right-region');
		}
		else {
			$('#block-exp-sp-lnrlearningplan-tab-my-learningplan').addClass('enroll-enable-right-region');
			$('#block-exp-sp-lnrlearningplan-tab-my-learningplan').remove('enroll-enable-right-region');
		}
		this.renderLearningPlanResults();
		this.widgetObj = '$("#learningplan-tab-inner").data("learningplan")';
		this.prevMoreLPObj;
		this.searchStrValue = '';
		this.changeClsEnrollId = '';
		}catch(e){
			// to do
		}
	},

  hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintEnrollmentLPResults tr.ui-widget-content').filter(":last");
    if (hideAll) {
    	if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrlearningplan-tab-my-learningplan .block-footer-left').show();
      $('#pager-lp').hide();
      $('#gview_paintEnrollmentLPResults').css('padding', '0');
      //$("#page-container #paintEnrollmentLPResults").find("tr.jqgrow td").css('padding-bottom', '15px');
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        lastDataRow.children('td').css('border', '0');
      }
    }
    else {
      var lastDataRow = $('#paintEnrollmentLPResults tr.ui-widget-content').filter(":last");
      lastDataRow.children('td').css('border', '0');
      $('#pager-lp').show();
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrlearningplan-tab-my-learningplan .block-footer-left').hide();
      $('#pager-lp #pager-lp_center #next_pager-lp').hide();
      $('#pager-lp #pager-lp_center #prev_pager-lp').hide();
      $('#pager-lp #pager-lp_center #first_pager-lp').hide();
      $('#pager-lp #pager-lp_center #last_pager-lp').hide();
      $('#pager-lp #pager-lp_center #sp_1_pager-lp').parent().hide();
    }
	}catch(e){
		// to do
	}
  },

  showPageControls : function() {
	try{
	var lastDataRow = $('#paintEnrollmentLPResults tr.ui-widget-content').filter(":last");
	lastDataRow.children('td').css('border', '0');	
    $('#pager-lp').show();
    if(this.currTheme == "expertusoneV2")
    $('#block-exp-sp-lnrlearningplan-tab-my-learningplan .block-footer-left').hide();
    $('#pager-lp #pager-lp_center #next_pager-lp').show();
    $('#pager-lp #pager-lp_center #prev_pager-lp').show();
    $('#pager-lp #pager-lp_center #first_pager-lp').show();
    $('#pager-lp #pager-lp_center #last_pager-lp').show();
    $('#pager-lp #pager-lp_center #sp_1_pager-lp').parent().show();
	}catch(e){
		// to do
	}
  },

	renderLearningPlanResults : function(){
		try{
	  	this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var dWidth = 663;
		}else{
			var dWidth = 667;
	    }
	  	if (Drupal.settings.last_left_panel==true) {
	  		rowNumValue = 20;
	  		rowListValue = [20,30,40];
	  	} else {
	  		rowNumValue = 10;
	  		rowListValue = [10,15,20];
	  	}
	  	$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
		var searchEnrollStr = '';
		var getRegStatus = 'lrn_tpm_ovr_enr|lrn_tpm_ovr_inp';
		var obj = $("#learningplan-tab-inner").data("learningplan");
		var objStr = '$("#learningplan-tab-inner").data("learningplan")';
		//var obj = this;

		var userId = this.lpUserId;
		
		var prgStr = Drupal.settings.myprogramsSearchStr;
		if (typeof prgStr === 'undefined' || prgStr == null || prgStr == undefined){
			searchEnrollStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus;
		}
		else {
			searchEnrollStr	= '&UserID='+userId+prgStr
		}
		var url = this.constructUrl("learning/learningplan-search/all/"+searchEnrollStr);
		if(Drupal.settings.mylearning_right===false){
			$("#paintEnrollmentLPResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sorttype':'text',formatter:$("body").data("learningcore").paintLearningImage,hidden : ((this.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'prg_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintLearningPlanTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintLPActions}
				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager-lp',
				viewrecords: true,
				emptyrecords: Drupal.t("MSG381"),
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackLPLoader

			}).navGrid('#pager-lp',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
			$("#paintEnrollmentLPResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sorttype':'text',formatter:$("body").data("learningcore").paintLearningImage,hidden : ((this.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'prg_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintLearningPlanTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintLPActions}
				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager-lp',
				viewrecords: true,
				emptyrecords: Drupal.t("MSG381"),
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				width: dWidth,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackLPLoader

			}).navGrid('#pager-lp',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
		/*0038401: UI Issue -- Horizontal scrollbar is displayed when My Programs is getting loaded
		 * change by: ayyappans
		 * Fix: table-layout: a for the jqgrid table*/
		//$('#gview_paintEnrollmentLPResults .ui-jqgrid-btable').css('table-layout','auto');
		$('#pager-lp').find('#pg_pager-lp .ui-pg-table #pager-lp_center').removeAttr("style");
		obj.hidePageControls(true); // show in loadComplete callbackLoader()
		$('.ui-jqgrid').addClass('myenrollment-table');
		$('.jqgfirstrow td:first-child').addClass('enroll-datatable-column-img');
		$('.jqgfirstrow td:nth-child(2)').addClass('enroll-datatable-column1');
		$('.jqgfirstrow td:last-child').addClass('enroll-datatable-column2');
		$('.ui-jqgrid-bdiv > div').css('position','static');
		$('#pager-lp').find('#pg_pager-lp .ui-pg-table').css("table-layout","auto");
		this.setSortTypeLPData('lrn_tpm_ovr_enr|lrn_tpm_ovr_inp');
		/* to highlight the default sort order - added by Rajkumar U*/
		$('#enroll-lp-result-container .sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-lp-result-container .add-high-lighter').addClass('sortype-high-lighter');
		}catch(e){
			// to do
		}
	},

  updateLPMultiContentLaunchDialog : function(enrollmentId, lessonId, lessonInfo, contentQuizStatusConsolidated) {
	try{
	var widgetObj = this;
    lessonInfo.LessonLocation = lessonInfo.LessonLocation == null || lessonInfo.LessonLocation == 'null'? '' : lessonInfo.LessonLocation;
    lessonInfo.LaunchData = lessonInfo.LaunchData == null || lessonInfo.LaunchData == 'null'? '' : lessonInfo.LaunchData;
    lessonInfo.SuspendData = lessonInfo.SuspendData == null || lessonInfo.SuspendData == 'null'? '' : lessonInfo.SuspendData;
    lessonInfo.CmiExit = lessonInfo.CmiExit == null || lessonInfo.CmiExit == 'null'? '' : lessonInfo.CmiExit;
    var relaunchInfo = [{
         lessonlocation: lessonInfo.LessonLocation
         , launchData: lessonInfo.LaunchData
         , suspendData: lessonInfo.SuspendData
         , exit: lessonInfo.CmiExit
    }];
    // Update lesson status
    var status = Drupal.t('MSG511');
    if (lessonInfo.Status != '') {
      status = Drupal.t(lessonInfo.Status);
    }
    var pipe1 = '<span class="enroll-pipeline">|</span>';
    var LessonStatus = '<div class = "line-item-container float-left">'+pipe1+'<span class="vtip" title="'+Drupal.t('LBL102')+' : ' + status+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+' : ' + status,'exp-sp-myprogram-multilessonstatus')+'</div></span>';
    var lesDiv = lessonId + "_" + enrollmentId + "_launch_button";
    if(document.getElementById(lesDiv) != null){
      $("#" + lessonId + "_" + enrollmentId + "_launch_button").data('relaunch', relaunchInfo);
      $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_status").html(LessonStatus);
    }
    // Update lesson score
    var lesnscore = '';
    
    if(lessonInfo.LesScore !=null && lessonInfo.LesScore!=undefined && lessonInfo.LesScore != '' && lessonInfo.LesScore != '0.00') {
  	    var lessonScore = '<div class = "line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+Drupal.t('LBL668') + ' : ' + lessonInfo.LesScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') + ' : ' + lessonInfo.LesScore,'exp-sp-myprogram-multilessonscore')+'</span></div>';

    }
    var contentQuizStatus = '';
    if(lessonInfo.contentQuizStatus != null && lessonInfo.contentQuizStatus != undefined && lessonInfo.contentQuizStatus != '') {
		  contentQuizStatus = '<div class = "line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus),'exp-sp-myprogram-quizstatus')+'</span></div>';

    }
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonScore").html(lesnscore);
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonQuizStatus").html(contentQuizStatus);
    // Update content status and score
    var contentStatus = '<div class = "line-item-container float-left"><span class="vtip" title="'+Drupal.t('MSG511')+'">'+titleRestrictionFadeoutImage(Drupal.t('MSG511'),'exp-sp-myprogram-contentstatus')+'</span></div>';;
    
    var Dstatus = Drupal.t('In progress');
	var Dstatus1 = Drupal.t('Incomplete');
	var Dstatus2 = Drupal.t('Completed');
    if (lessonInfo.ContentStatus != '') {
    	contentStatus = '<div class = "line-item-container float-left"><span class="vtip" title="'+Drupal.t(lessonInfo.ContentStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t(lessonInfo.ContentStatus),'exp-sp-myprogram-contentstatus')+'</span></div>';
    }
    if(lessonInfo.LaunchType == 'VOD' && lessonInfo.Status != ''){
    	contentStatus = '<div class = "line-item-container float-left"><span class="vtip" title="'+Drupal.t(lessonInfo.Status)+'">'+titleRestrictionFadeoutImage(Drupal.t(lessonInfo.Status),'exp-sp-myprogram-contentstatus')+'</span></div>';
    }
    var score = '';
    var pipe = '<span class="enroll-pipeline">|</span>';
    if(lessonInfo.ContScore !=null && lessonInfo.ContScore!=undefined && lessonInfo.ContScore != '' && lessonInfo.ContScore != '0.00' ) {
      score ='<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668') + ' : ' + lessonInfo.ContScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') + ' : ' + lessonInfo.ContScore,'exp-sp-myprogram-multilessonscore')+'</span></div>';
    }
    var attemptsLeft = (lessonInfo.AttemptLeft == 'notset')? '' : lessonInfo.AttemptLeft;
    if (attemptsLeft !== '') {
        attemptsLeft = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL202')+' : ' + attemptsLeft+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL202')+' : ' + attemptsLeft,'exp-sp-myprogram-attemptsleft')+'</span></div>';

    }
    
    var attemptsLeft = (lessonInfo.AttemptLeft == 'notset')? '' : lessonInfo.AttemptLeft;
    if (attemptsLeft !== '') {
        attemptsLeft = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL202')+' : ' + attemptsLeft+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL202')+' : ' + attemptsLeft,'exp-sp-myprogram-attemptsleft')+'</span></div>';

    }
    
    var validityDay = (lessonInfo.ValidityDays == '')? '' : lessonInfo.ValidityDays;
    var diffDays =  lessonInfo.remDays;

    if (validityDay !== '') {
//    	var remValidityDays = validityDay - diffDays;
    	var remValidityDays = (lessonInfo.daysLeft !== undefined && lessonInfo.daysLeft != null) ? lessonInfo.daysLeft : '';
		  if(remValidityDays <= 0){
		  var daysLBL = Drupal.t("Expired");//Expired
		  remValidityDays = '';// To avoid result as Validity: 0 Expired
		  }else if(remValidityDays == 1){
		  var daysLBL = Drupal.t("LBL910");//day
		  }else if(remValidityDays > 1){
		  var daysLBL = Drupal.t("LBL605");//days
		  }
		  validityDay = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-myprogram-validitydays')+'</span></div>';;

    }
    contentQuizStatus = contentQuizStatusConsolidated;	//overall success/quiz status of content
    if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
    	contentQuizStatus =  '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL1284')+' : ' + Drupal.t(contentQuizStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL1284')+' : ' + Drupal.t(contentQuizStatus),'exp-sp-myprogram-quizstatus')+'</span></div>';;

    }
	var percentCompleted = '';
	if(lessonInfo.LaunchType == 'VOD') {
		//console.log(lessonInfo.SuspendData);
		var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "") ? JSON.parse(unescape(unescape(lessonInfo.SuspendData))) : null;
		//console.log(suspend_data);
		var progress = suspend_data != null ? suspend_data['progress'] : 0;
		//console.log(progress);
		progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
		percentCompleted =  '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+progress+'% '+Drupal.t('Completed')+'">'+titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-myprogram-compPercent')+'</span></div>';

	}

	  $("#lesson_status_" + lessonInfo.ContentId).html(contentStatus + score  + contentQuizStatus +  attemptsLeft + validityDay + percentCompleted);
    // When no more attempts left, disable the Launch button
    if (lessonInfo.AttemptLeft == 0) {
      // Single lesson content
      $("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .launch_button_lbl").removeClass('enroll-launch-full');
      $("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .launch_button_lbl").addClass('enroll-launch-empty');
      $("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeClass('actionLink');
      $("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeAttr('onclick');
      // More than 1 lesson in content
      $("#launch-lp-wizard #" + lessonInfo.ContentId + "LPLessonSubGrid .launch_button_lbl").removeClass('enroll-launch-full');
      $("#launch-lp-wizard #" + lessonInfo.ContentId + "LPLessonSubGrid .launch_button_lbl").addClass('enroll-launch-empty');
      $("#launch-lp-wizard #" + lessonInfo.ContentId + "LPLessonSubGrid .enroll-launch").removeClass('actionLink');
      $("#launch-lp-wizard #" + lessonInfo.ContentId + "LPLessonSubGrid .enroll-launch").removeAttr('onclick');
      $("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() { // for VOD content type
        var launchButtonId = $(this).attr('id');
        if (Drupal.ajax[launchButtonId]) {
          if (Drupal.ajax[launchButtonId].element_settings.event) {
            $("#" + launchButtonId).unbind(Drupal.ajax[launchButtonId].element_settings.event);
          }
          if (Drupal.ajax[launchButtonId].element_settings.keypress) {
            $("#" + launchButtonId).unbind('keypress');
          }
          delete Drupal.ajax[launchButtonId];
          $("#" + launchButtonId).attr('href', '#');
          $("#" + launchButtonId).click(function() {return false;});
          $("#" + launchButtonId).removeClass('use-ajax');
          $("#" + launchButtonId).removeClass('ctools-modal-ctools-video-style');
          $("#" + launchButtonId).removeClass('ajax-processed');
        }
      });
    }
	if(lessonInfo.LaunchType == 'VOD' && lessonInfo.AttemptLeft != 0) {
		//update launch button progress
		//console.log($("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .enroll-launch"));
		$("#launch-lp-wizard #lp-lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() {
			var launchButtonId = $(this).attr('id');
			var href = $("#" + launchButtonId).attr('href');
			href_suspend = href.substr(0, href.lastIndexOf("/"));
			href = href_suspend.substr(0, href_suspend.lastIndexOf("/"));
			// var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "") ? JSON.parse(unescape(lessonInfo.SuspendData)) : null;
			// var progress = suspend_data != null ? suspend_data['progress'] : 0;
			// if (Drupal.ajax[launchButtonId].element_settings.url) {
				// Drupal.ajax[launchButtonId].element_settings.url = href + '/' + progress;
			// }
			if(Drupal.ajax[launchButtonId].element_settings.url) {
				var url = Drupal.ajax[launchButtonId].element_settings.url.substr(0, Drupal.ajax[launchButtonId].element_settings.url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].element_settings.url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			if(Drupal.ajax[launchButtonId].url){
				var url = Drupal.ajax[launchButtonId].url.substr(0, Drupal.ajax[launchButtonId].url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			if(Drupal.ajax[launchButtonId].options.url) {
				var url = Drupal.ajax[launchButtonId].options.url.substr(0, Drupal.ajax[launchButtonId].options.url.lastIndexOf("/"));
				var url = url.substr(0,url.lastIndexOf("/"));
				Drupal.ajax[launchButtonId].options.url = url + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData;
			}
			$("#" + launchButtonId).attr('href', href + '/' + lessonInfo.StatusCode + '/' + lessonInfo.SuspendData);
		});
		}
	}catch(e){
		// to do
		//console.log(e);
	}
  },
	callbackLPLoader : function(data, postdata, formid, updateShowMore){
	 try{
	  var obj = $("#learningplan-tab-inner").data("learningplan");
	  var default_cookie = '&regstatuschk=lrn_tpm_ovr_enr|lrn_tpm_ovr_inp&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';
	  obj.destroyLoader('enroll-lp-result-container');
	  if(Drupal.settings.mylearning_right===false)
		$('.clsenroll-lp-result-container #pager-lp').width($('.clsenroll-lp-result-container').width()+4);
    // Delete enrollmentId and lessonId from jqGrid postData object when present. Added for mantis ticket #0020086
    var postData = $('#paintEnrollmentLPResults').getGridParam("postData");	
    if (postData !== undefined) { // When add my program panel under deleted panel, default tab not selected - my learn customization
        delete postData.enrollmentId;
        delete postData.lessonId;
        delete postData.enrId;
		delete postData.lesId;
		delete postData.objecttype;
    }
    if (document.getElementById('lplaunch-content-container') &&
          data.triggering_enrollment_id && data.triggering_lesson && data.triggering_lesson_details) { // added for mantis ticket #0020086
      obj.updateLPMultiContentLaunchDialog(data.triggering_enrollment_id, data.triggering_lesson, data.triggering_lesson_details, data.triggering_content_quiz_status);
    }
	// var recs = parseInt($("#paintEnrollmentLPResults").getGridParam("records"),10);
	var recs = data['records'];
	updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
	if (updateShowMore) {
	$("#paintEnrollmentLPResults").data('totalrecords', recs);
	var showMore = $('#paintEnrollmentLPResults-show_more');
	if($('#paintEnrollmentLPResults').getGridParam("reccount") < recs) {
		showMore.show();
	} else {
		showMore.hide();
	}
	}
		
		//Added by ganeshbabuv to avoid the issue for not showing the enrollment message if there are no enrollments and comes from salesforce app - Ref:SF Cookieless Option (0054508),Sep 30th 2015
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {
			setTimeout(function(){			  
			    if (recs <= 0) {
					if(Drupal.settings.myprogramsSearchStr === default_cookie){
						$('#enroll-lp-noresult-msg').show();
					}
					else{
						$('#enroll-lp-nosearchresult-msg').show();
					}
					$('#enroll-lp-result-container .sort-by-text').hide();
					$('#enroll-lp-result-container .sort-by-links').hide();
				} else {
					$('#enroll-lp-result-container .sort-by-text').show();
					$('#enroll-lp-result-container .sort-by-links').show();
				}				
			  
			},150);
		
		}else{	
			if (recs <= 0) {
				if(Drupal.settings.myprogramsSearchStr === default_cookie){
						$('#enroll-lp-noresult-msg').show();
					}
					else{
						$('#enroll-lp-nosearchresult-msg').show();
					}
				$('#enroll-lp-result-container .sort-by-text').hide();
				$('#enroll-lp-result-container .sort-by-links').hide();
			} else {
				$('#enroll-lp-result-container .sort-by-text').show();
				$('#enroll-lp-result-container .sort-by-links').show();
			}
		}
		
    // Show pagination only when search results span multiple pages
    var reccount = parseInt($("#paintEnrollmentLPResults").getGridParam("reccount"), 10);
    var hideAllPageControls = true;
    if (recs > 5) { // 10 is the least view per page option.
      hideAllPageControls = false;
    }
    if (recs <= reccount) { // Covers the case when there are no recs, i.e. recs == 0
      obj.hidePageControls(hideAllPageControls);
    }
    else {
      obj.showPageControls();
    }
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();

        $('.fivestar-click').click(function() {
        	var fivestarClick = this;
        	var fiveStarClass = $(this).attr('class');
        	var pattern = /[0-9]+/;
        	var rating = fiveStarClass.match(pattern);
        	var nodeId = $(this).parents("form").siblings(".prgenroll-node").val();
        	var entity_type = $(this).parents("form").siblings(".prgenroll-type").val();
        	var param = {'rating':rating,'nodeId':nodeId,'entityType':entity_type};
    		url = obj.constructUrl("learning/five-star-submit");
    		$.ajax({
    			type: "POST",
    			url: url,
    			data:  param,
    			success: function(result){
	    			if(result.average_rating == "AlreadyRated"){
	    				return false;
	    			}else{
	    				$("body").data("learningcore").fiveStarCheck(nodeId,'prg-node-');
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? result.votemsg : result.votesmsg;
		    			var avgRating = '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);
	    			}
    			}
    	    });
		});
		$("#learningplan-tab-inner").data("learningplan").shapeLPButton('#paintEnrollmentLPResults');
		$('#enroll-lp-result-container .ui-jqgrid-hdiv').css('display','none');
		$('#paintEnrollmentLPResults .jqgfirstrow').css('display','none');
		//Vtip-Display toolt tip in mouse over
		
		
		 	if (Drupal.settings.salesforce.type == "canvas" || Drupal.settings.salesforce.type == "iframe") {
				if (Drupal.settings.mylearning_right===false) {
					var currentWidth = 942;
					$("#paintEnrollmentLPResults").jqGrid('setGridWidth', currentWidth);
					//$("#paintEnrollmentLPResults").setGridParam({page: 1}).trigger('reloadGrid');
				}
			}
			var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');
			if(typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1" && (navigator.userAgent.indexOf('Edge/')!= -1)) {		
			     RefreshFadeoutForSF('#paintEnrollmentLPResults','item-title-code','line-item-container','MyPrograms');
			     Refreshtrunk8('myprograms','limit-title-lp');
		    }else{ 
			    resetFadeOutByClass('#paintEnrollmentLPResults','item-title-code','line-item-container','MyPrograms');
				$('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
			    $('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
		    }
		
			vtip();
			$("#paintEnrollmentLPResults").showmore({
				'grid': "#paintEnrollmentLPResults",
				'gridWrapper': '#enroll-lp-result-container',
				'showMore': '#paintEnrollmentLPResults-show_more'
			});
	 }catch(e){
			// to do
		}
	},
	shapeLPButton : function(parentId){
		try{
		$(parentId).find(".enroll-lp-main-list, .enroll-main-list").each(function() {
			var avlBtn = 0;
			avlBtn += ($(this).children(".enroll-launch-gray").length > 0)? 1: 0;
			avlBtn += ($(this).children(".enroll-launch-more-gray").length > 0)? 2: 0;

			switch(avlBtn) {
				case 2:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch").wrap("<label class='enroll-launch-full'></label>");
				case 3:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch-gray").wrap("<label class='enroll-launch-empty'></label>");
			}
		});
		}catch(e){
			// to do
		}
	},
	callLearningPlan : function(regStatus, regActive){
		try{
		$('#enroll-lp-noresult-msg').hide();
		$('#enroll-lp-nosearchresult-msg').hide();
		this.setEnrStateLPMsg(regActive);
		var userId = this.lpUserId;
    	var getRegStatus = (regStatus) ? regStatus : '';
    	var searchStr = '';
    	searchStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus;
    	var url = this.constructUrl('learning/learningplan-search/all/'+searchStr);
    	//this.setSortTypeLPData(getRegStatus);
		$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
    	$('#paintEnrollmentLPResults').setGridParam({url: url});
        $("#paintEnrollmentLPResults").trigger("reloadGrid",[{page:1}]);
       	this.highlightLPTab(regActive);
       	$('#enroll-lp-result-container .sort-by-links').find('li a').removeClass('sortype-high-lighter');
       	$('#enroll-lp-result-container .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
       	if(this.currTheme == "expertusoneV2"){
		    $('#learner-learningplan .sort-by-links').find('li a').removeClass('sortype-high-lighter');
		    $('#learner-learningplan .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
        }
		}catch(e){
			// to do
		}
    },
	setEnrStateLPMsg : function(regActive) {
		try{
		switch (regActive) {
		case 'EnrollmentLP' :
			$('#enrollment-lp-state').html((Drupal.t('Enrolled')).toLowerCase());
			break;
		case 'EnrollLPCompleted':
			$('#enrollment-lp-state').html(Drupal.t('Completed'));
			break;
		case 'EnrollLPInCompleted':
			$('#enrollment-lp-state').html((Drupal.t('Incomplete')).toLowerCase());
			break;
		case 'EnrollExpired':
			$('#enrollment-lp-state').html((Drupal.t('Expired')).toLowerCase());
			break;
		case 'EnrollLPCanceled' :
			$('#enrollment-lp-state').html(Drupal.t('Canceled'));
			break;
		case 'EnrollPending' :
			$('#enrollment-lp-state').html((Drupal.t('Pending')).toLowerCase());
			break;
		}
		}catch(e){
			// to do
		}
	},
	setSortTypeLPData : function(getRegStatus) {
		try{
    	var setdata = "data={'currEnrMode':'"+getRegStatus+"'}";
    	$("#sort-by-lp-enroll").attr("data",setdata);
		}catch(e){
			// to do
		}
    },
	highlightLPTab : function(highlightId){
		try{
		var curTheme = themepath.split("/");
		var resTheme = curTheme[curTheme.length-1];
		if (resTheme == 'expertusoneV2')
		{
			$('#learningplan-maincontent ul li a#'+highlightId).parent('li').siblings('li').removeClass('selected');
			$('#learningplan-maincontent ul li a#'+highlightId).parent('li').addClass('selected');
		} else
		{
			$("#learningplan-maincontent ul li a").each(function(){
			    $(this).removeClass('orange');
			});
			$('#learningplan-maincontent ul li a#'+highlightId).addClass('orange');
		}
		}catch(e){
			// to do
		}
	},
	displayTip : function(elementid, messagecontent){
		try{
		if(!document.getElementById("tooltip"+elementid)) {
			$('#'+elementid).qtip({
				 content: '<div id="tooltip'+elementid+'" class="tooltiptop"></div><div class="tooltipmid">'+messagecontent+'</div><div class="tooltipbottom"></div>',
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
				position: { adjust: { x: -75, y: 0 } },
				style: {
					width: 325,
					background: 'none',
					'font-size' : 12,
					color: '#333333'
				}
			});
		}
		}catch(e){
			// to do
		}
	},
	paintLearningPlanTitle : function(cellvalue, options, rowObject) {
		try{
		var wobj = eval(options.colModel.widgetObj);
		var obj = options.colModel.widgetObj;
		var master_enroll_id 		= rowObject['master_enroll_id'];
		var user_id 				= rowObject['user_id'];
		var overall_status 			= rowObject['overall_status'];
		var reg_date				= rowObject['reg_date'];
		var cancel_date				= rowObject['cancel_date'];
		var comp_date				= rowObject['comp_date'];
		var update_date				= rowObject['update_date'];
		var expired_date				= rowObject['expired_date'];
		var percentage_complete		= rowObject['percentage_complete'];
		var prg_id					= rowObject['prg_id'];
		var dataId					= rowObject['id'];
		var base_type 				= rowObject['base_type'];
		var prg_title				= rowObject['prg_title'];
		var prg_code 				= rowObject['prg_code'];
		var prg_desc				= rowObject['prg_desc'];
		var prg_shortdesc 			= encodeURIComponent(rowObject['prg_shortdesc']);
		var prg_fulldesc 			= encodeURIComponent(rowObject['prg_fulldescription']);
		var prg_start_date 			= rowObject['prg_start_date'];
		var prg_end_date			= rowObject['prg_end_date'];
		var prg_status 				= rowObject['prg_status'];
		var prg_object_type_code 	= rowObject['prg_object_type'];
		var prg_object_type 		= rowObject['prg_type'];
		var recertify_path			= rowObject['recertify_path'];

		var enr_comp_status			= rowObject['enr_comp_status'];
		var isRequiredCount 		= enr_comp_status['req_count'];
		var compCount 				= enr_comp_status['comp_count'];
		var enrollCount 			= enr_comp_status['enr_count'];
		var inpCount 				= enr_comp_status['inp_count'];
		var incCount 				= enr_comp_status['inc_count'];
		var completedPercentage 	= enr_comp_status['comp_percentage'];

		var master_mandatory        = rowObject['master_mandatory'];
		var FullName				= rowObject['full_name'];
		var managerid				= rowObject['managerid'];
		var UpdatedBy             	=  rowObject['updated_by'];
		var UpdatedByName         	=  rowObject['updated_by_name'];
		var created_by_ins_mngr_slf =  rowObject['created_by_ins_mngr_slf'];
		var updated_by_ins_mngr_slf =  rowObject['updated_by_ins_mngr_slf'];
		var lnrAttach				= rowObject['show_lnr_lp_attach'];
		var mro						= rowObject['mro'];
		var mro_name				= rowObject['mro_name'];
		var assigned_by				= unescape(rowObject['assigned_by']).replace(/"/g, '&quot;');
		var labelmsg 				= rowObject['labelmsg'];
		var isLastRec             	=  (typeof rowObject['is_last_rec'] != 'undefined' && rowObject['is_last_rec'] == 'last')? 'last' : '';
		
		var is_exempted		= rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'];
		var exempted_on = rowObject['exempted_on'];
		var mroImageClass = '';
		var mroImageClassArr = new Array();
        mroImageClassArr['cre_sys_inv_man'] =  '<span class="course-mandatory-bg vtip" id="'+prg_id+mro+'" title="'+Drupal.t('Mandatory')+'">'+Drupal.t('M')+'</span>';
        //mroImageClassArr['cre_sys_inv_opt'] =  '<span class="course-optional-bg" id="'+prg_id+mro+'" onmouseover="$(\'#learningplan-tab-inner\').data(\'learningplan\').displayTip(\''+prg_id+mro+'\',\''+Drupal.t('Optional')+'\')">'+'</span>';
        mroImageClassArr['cre_sys_inv_opt'] = '';
        mroImageClassArr['cre_sys_inv_rec'] =  '<span class="course-recommended-bg vtip" title="'+Drupal.t('Recommended')+'">'+Drupal.t('LBL746')+'</span>';
        if(master_mandatory == 'Y' || master_mandatory == '1'){
        	mroImageClass = mroImageClassArr['cre_sys_inv_man'];
        } else {
        	mroImageClass = (mro == '' || mro == null) ? '' : mroImageClassArr[mro];
        }
		var data1="data={'MasterEnrollId':'"+master_enroll_id+"','PrgId':'"+prg_id+"','Desc':'"+escape(prg_shortdesc)+"','Recertify':'"+recertify_path+"','FullName':'"+escape(FullName)+"'," +
		"'MasMandatory':'"+master_mandatory+"','managerid':'"+managerid+"','UpdatedBy':'"+UpdatedBy+"','UpdatedByName':'"+escape(UpdatedByName)+"','overall_status':'"+overall_status+"'," +
		"'mro_type':'"+mro_name+"','assigned_by':'"+assigned_by+"','updated_by_ins_mngr_slf':'"+updated_by_ins_mngr_slf+"','created_by_ins_mngr_slf':'"+created_by_ins_mngr_slf+"'," +
		"'regDate':'"+reg_date+"','compDate':'"+comp_date+"','updateDate':'"+update_date+"'," +
		"'is_last_rec':'"+isLastRec+"',"+"'is_exempted':'"+is_exempted+"',"+"'exempted_by':'"+exempted_by+"',"+"'exempted_on':'"+exempted_on+"'}";
		var html	= '';

		var inc = 0;
		var passParams ='';
		if(lnrAttach.length>0) {
			 $(lnrAttach).each(function(){
				 inc=inc+1;
				 passParams += "{";
				 passParams += "'Id':'"+$(this).attr("attachment_id")+"'";
				 /*--Issue fix for the ticket - 32781 --*/
				 passParams += ",'Title':'"+escape($(this).attr("reading_title"))+"'";
				 passParams += ",'url':'"+encodeURI($(this).attr("reading_content")).replace(/'/g, '%27')+"'";
				 passParams += "}";
				 if(inc<lnrAttach.length) {
					 passParams += ",";
				 }
			 });
		}
		var attachdata = "data=["+passParams+"]";
		//Enrollment Title Restricted to 20 Character
		var prgFullTitle = prg_title;
		var isMRO = '';
		if(mroImageClass != '') {
			isMRO = ' enroll-lp-course-title-mro';
		}
		html += '<div class="enroll-lp-course-title'+isMRO+'">';
		html +=  '<div class="limit-title-row">';
		html += '<span id="lp_attachment_'+master_enroll_id+'" data="'+attachdata+'"></span>';
		if(wobj.currTheme != 'expertusoneV2') {
			html += '<a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>&nbsp;</a>';
		}
		html += '<span id="titlePrgEn_'+master_enroll_id+'" class="item-title ' + overall_status + '" ><span class="limit-title limit-title-lp enroll-lp-class-title vtip" title="'+prgFullTitle+'" href="javascript:void(0);">';
		var fadout_image_title = 'exp_sp_lnrmyprogram_title_'+overall_status;
		//if(Drupal.settings.mylearning_right===false)
		//	html += titleRestrictionFadeoutImage(prgFullTitle,fadout_image_title,55);
		//else
			html += prgFullTitle;
		
		if(is_exempted != 1)
			html += '</span>'+mroImageClass+'</span>';
		
		html += '</div>';
		html += '</div>';
		
		/*// progress bar disabled - new progress bar implementation
		var progressBar ='';
		if(overall_status == 'lrn_tpm_ovr_enr' || overall_status == 'lrn_tpm_ovr_cmp' || overall_status == 'lrn_tpm_ovr_inp') {
			progressBar +='<div class="ui-progressbar">'+
						  '<div class="ui-progress-curve">&nbsp;</div>'+
						  '<div class="progress-status">'+completedPercentage+'% '+Drupal.t('Completed')+'</div>'+
		   		  		  '<div style="width: '+completedPercentage+'%;" class="ui-progressbar-value">&nbsp;</div>'+
						  '</div>';
		}
		html += '<span class="session-time-zone">'+progressBar+'</span>'; */
		
		html += '<div class="enroll-tp-title-info">';
		html += '<div class="item-title-code">';
		var statusDate = '';
		var statusName = '';
		if(overall_status == 'lrn_tpm_ovr_cln') {
			statusDate = cancel_date;
			statusName = SMARTPORTAL.t(rowObject['labelmsg']['msg2']);
		}
		else if(overall_status == 'lrn_tpm_ovr_cmp') {
			statusDate = comp_date;
			statusName = SMARTPORTAL.t(rowObject['labelmsg']['msg3']);
		}else if(overall_status == 'lrn_tpm_ovr_inc') {
			statusDate = comp_date;
			statusName = Drupal.t('LBL1193');
		}
		else if(overall_status == 'lrn_tpm_ovr_exp') {
			statusDate = expired_date;
			statusName = SMARTPORTAL.t(rowObject['labelmsg']['msg4']);
		}
		else{
			statusDate = reg_date;
			statusName = SMARTPORTAL.t(rowObject['labelmsg']['msg1']);
		}
		//Pipe Sysmbol use this variable 'pipe'
		var pipe = '<span class="enroll-pipeline">|</span>';
		var complete_by = false;
		if(overall_status != 'lrn_tpm_ovr_enr' && overall_status != 'lrn_tpm_ovr_inp' && overall_status !='lrn_tpm_ovr_cmp' && overall_status != 'lrn_tpm_ovr_inc' && overall_status != 'lrn_tpm_ovr_exp'  && overall_status != 'lrn_tpm_ovr_ppv' && overall_status != 'lrn_tpm_ovr_ppm') {
		  html += statusName+' : '+statusDate;
		 // html += pipe ;
		}
//		if(prg_code) {
//			if(Drupal.settings.mylearning_right===false)
//				html +='<span class="vtip" title="'+Drupal.t('LBL096')+" : "+htmlEntities(prg_code)+'">'+titleRestrictionFadeoutImage(prg_code,'exp-sp-lnrmyprogram-prg-code-right-false',45)+"</span>";
//			else
//				html +='<span class="vtip" title="'+Drupal.t('LBL096')+" : "+htmlEntities(prg_code)+'">'+titleRestrictionFadeoutImage(prg_code,'exp-sp-lnrmyprogram-prg-code',5)+"</span>";
//		}
		var inCompleteCount = (inpCount+enrollCount);
		var recertstr = "";
		if(overall_status == 'lrn_tpm_ovr_cmp' && prg_object_type == 'Recertification' && recertify_path > 1) { 
			html +=  "Recertified on : "+comp_date;						
		} else if(overall_status == 'lrn_tpm_ovr_enr' || overall_status == 'lrn_tpm_ovr_cmp' || overall_status == 'lrn_tpm_ovr_inp') {
			//html +=  Drupal.t('Completed')+' : '+ compCount + pipe+ Drupal.t('In progress')+' : '+ inCompleteCount + pipe+ Drupal.t('Incomplete')+' : '+ incCount;
			html += "<div class = 'line-item-container float-left'><span class ='vtip' title = '"+Drupal.t('Completed')+" : "+ compCount+"'>"+titleRestrictionFadeoutImage(Drupal.t('Completed')+" : "+ compCount,'exp-sp-myprogram-lineitem')+"</span></div>";
			html += "<div class = 'line-item-container float-left'><span class ='vtip' title = '"+Drupal.t('In progress')+' : '+ inCompleteCount+"'>"+titleRestrictionFadeoutImage(pipe+ Drupal.t('In progress')+' : '+ inCompleteCount,'exp-sp-myprogram-lineitem')+"</span></div>";
			html += "<div class = 'line-item-container float-left'><span class ='vtip' title = '"+Drupal.t('Incomplete')+' : '+ incCount+"'>"+titleRestrictionFadeoutImage(pipe+ Drupal.t('Incomplete')+' : '+ incCount,'exp-sp-myprogram-lineitem')+"</div>";

			complete_by = true;
		}else if(overall_status == 'lrn_tpm_ovr_inc'){
			html += "<div class = 'line-item-container float-left'>"+Drupal.t('LBL1193')+ ' : ' +comp_date+"</div>";
		}
		if(overall_status == 'lrn_tpm_ovr_cmp' && (prg_object_type == 'Certification' || prg_object_type == 'Recertification') && expired_date != '') {
			var msglbl = rowObject['is_current'] == 'Y' ? Drupal.t('LBL735') : Drupal.t('LBL028');
			html += pipe+ msglbl+' : '+ expired_date ; 
			complete_by = true;
		}
		if(prg_object_type_code == 'cre_sys_obt_trn') {
			if(Drupal.settings.mylearning_right===false) {
				//0073410: My Learning Changes - For a Canceled Learning plan -"Canceled On : Mar 06, 2017Complete By : Mar 31, 2017" is displayed
				if(complete_by == true && overall_status != 'lrn_tpm_ovr_cln'){
					html += "<span id='completeBy"+prg_id+"' class='completeByLengthly'>";
					html += pipe;
					html += Drupal.t("LBL234")+' : ' +prg_end_date;
					html += "</span>";
				}
			}
			else {
				if(overall_status != 'lrn_tpm_ovr_cln'){
					html += "<div id='completeBy"+prg_id+"' class='completeBy' style='display:none;'>";
					html += Drupal.t("LBL234")+' : ' +prg_end_date;
					html += "</div>";
				}
			}
		}
		html += ' </div>';
	    if(prg_shortdesc != null && prg_shortdesc != 'null') {
	    	var prg_description_icon = decodeURIComponent(prg_shortdesc) && decodeURIComponent(prg_fulldesc);
	    	var tptype = prg_object_type_code;
		   	html += '<div class="learningplan-desc-div">';
			html += '<div class="limit-desc-row '+tptype+'">';
			html += '<div class="recordDiv FindTrngTxt"><span class="limit-desc limit-desc-lp vtip" id="LPShortDesc_prg_'+master_enroll_id+'"><span class="cls-learner-descriptions">'+prg_description_icon+'</span></span></div>';
			html += '</div>';
	   }

			html += '<div class="lp_seemore" id="lp_seemore_prg_'+master_enroll_id+'" onclick="seeMoreMyLearning('+master_enroll_id+',\'myprogramtp\');">'+ Drupal.t('LBL713') +'</div>';
		html += '</div>';

		return html;
		}catch(e){
			// to do
			// console.log(e);
		}
	},
	paintLPActions : function(cellval,options,rowObject){
		try{
		var master_enroll_id 		= rowObject['master_enroll_id'];
		var user_id 				= rowObject['user_id'];
		var overall_status 			= rowObject['overall_status'];
		var reg_date				= rowObject['reg_date'];
		var cancel_date				= rowObject['cancel_date'];
		var comp_date				= rowObject['comp_date'];
		var update_date				= rowObject['update_date'];
		var percentage_complete		= rowObject['percentage_complete'];
		var prg_id					= rowObject['prg_id'];
		var base_type 				= rowObject['base_type'];
		var prg_title				= rowObject['prg_title'];
		var prg_code 				= rowObject['prg_code'];
		var prg_desc				= rowObject['prg_desc'];
		var prg_shortdesc 			= rowObject['prg_shortdesc'];
		var prg_start_date 			= rowObject['prg_start_date'];
		var prg_end_date			= rowObject['prg_end_date'];
		var prg_status 				= rowObject['prg_status'];
		var prg_object_type 		= rowObject['prg_object_type'];
		var prg_type 				= rowObject['prg_type'];
		var master_mandatory        = rowObject['master_mandatory'];
		var full_name               = rowObject['full_name'];
		var managerid               = rowObject['managerid'];
		var mro						= rowObject['mro'];
		var surStatus				= rowObject['survey_status'];
		var assessStatus			= rowObject['assessment_status'];
		var preassessStatus			= rowObject['preassessment_status'];
		//var attemptsleft			= rowObject['attemptleft'];
		//var maxScoreValidation		= rowObject['max_score_validation'];
		var maxScoreValidationpre	= rowObject['maxscorevaluepre'];
		var maxScoreValidationpost	= rowObject['maxscorevaluepost'];
		var inpClassCount			= rowObject['inp_class_count'];
		var star_widget				= rowObject['star_widget'];
		var nodeId					= rowObject['node_id'];
		var recertify_path			= rowObject['recertify_path'];
		var UpdatedBy            	= rowObject['updated_by'];
		var UpdatedByName        	= rowObject['updated_by_name'];
		var created_by_ins_mngr_slf =  rowObject['created_by_ins_mngr_slf'];
		var updated_by_ins_mngr_slf =  rowObject['updated_by_ins_mngr_slf'];
		var isLastRec             =  (typeof rowObject['is_last_rec'] != 'undefined' && rowObject['is_last_rec'] == 'last')? 'last' : '';
		var mro_name				= rowObject['mro_name'];
		var assigned_by				= unescape(rowObject['assigned_by']).replace(/"/g, '&quot;');
		var baseType				= rowObject['basetype'];
		var startTime 				= rowObject['session_start'];
		var is_current 				= rowObject['is_current'];
		var master_enroll_id_encrypted		= rowObject['master_enroll_id_encrypted'];
		var prg_id_encrypted 				= rowObject['prg_id_encrypted'];
		var recertify_before_certify_expired = rowObject['recertify_before_certify_expired'];
		
		var is_exempted		= rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'];
		var exempted_on = rowObject['exempted_on'];
		var hideShare = rowObject['hide_share'];
		var expired_on	= rowObject['expired_on'];
		var dataId	= 'prg_' + master_enroll_id;
		var completedPercentage 	= rowObject['enr_comp_status']['comp_percentage'];
		var sharefullbutton = null;
		var cancelAction = '';
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			var sTime,sTimeForm,eTime,eTimeForm,nTime,stDateFull,enDateFull,srvDate;
			if(sessLen>1) {
				if(baseType =="ILT"){
					sessLenEnd = sessLen-1;
					sTime = rowObject.sessionDetails[0].session_start_format;
					sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
					eTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
					eTimeForm = rowObject.sessionDetails[sessLenEnd].session_end_time_form;
					nTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
					eDateTime = rowObject.sessionDetails[0].session_end_format;
					stDateFull = rowObject.sessionDetails[0].session_start_time_full;
					enDateFull = rowObject.sessionDetails[sessLenEnd].session_end_time_full;
				}else{
					var ind=0;
					for(var i=0;i<rowObject.sessionDetails.length;i++){
						if(rowObject.sessionDetails[i].session_id == launchInfo[0]['ID'] ){
							ind=i;
							break;
						}
						vcMoreActionFlag=false;
					}
					if(vcMoreActionFlag == false && ind == 0){
						ind = rowObject.sessionDetails.length - 1;
					}
					sTime = rowObject.sessionDetails[ind].session_start_format;
					sTimeForm = rowObject.sessionDetails[ind].session_start_time_form;
					eTime = rowObject.sessionDetails[ind].session_end_format;
					eTimeForm = rowObject.sessionDetails[ind].session_end_time_form;
					nTime = rowObject.sessionDetails[ind].session_end_format;
					eDateTime = rowObject.sessionDetails[ind].session_end_format;
					stDateFull = rowObject.sessionDetails[ind].session_start_time_full;
					enDateFull = rowObject.sessionDetails[ind].session_end_time_full;
				}
			} else {
				sTime = rowObject.sessionDetails[0].session_start_format;
				sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
				eTime = rowObject.sessionDetails[0].session_start_end_format;
				eDateTime = rowObject.sessionDetails[0].session_end_format;
				eTimeForm = rowObject.sessionDetails[0].session_end_time_form;
				nTime = rowObject.sessionDetails[0].session_end_format;
				stDateFull = rowObject.sessionDetails[0].session_start_time_full;
				enDateFull = rowObject.sessionDetails[0].session_end_time_full;
			}
		}
		// Assessment Validation - Max score and questions score total should be equal.
		// At least one assessment should match this condition
		if(assessStatus == 0 && maxScoreValidationpost == 0)
			assessStatus = false;
		// learning plan Assessment Pre Or Post assement has marks are not set right
		if(assessStatus != 0 && maxScoreValidationpost == 0)
			 assessStatus = false;
		if(preassessStatus != 0  && maxScoreValidationpre == 0)
			 preassessStatus =false;

		$.ui.learningplan.parentAssessStatus[prg_id] = assessStatus;

		var obj = options.colModel.widgetObj;
		var objEval = eval(options.colModel.widgetObj);

		var surObj		= '$("#block-take-survey").data("surveylearner")';
		var shareactionbuttom = null;
		var mainRecertifyButton = '';
		var certificateButton = '';

		// Prepare the Action HTML to be returned.
		var html = '';
		html += "<div class='enroll-lp-main-list' id='enroll-lp-main-action-"+master_enroll_id+"'>";

		var data1="data={'RegId':'" + master_enroll_id + "','MasterEnrollId':'"+master_enroll_id+"','PrgId':'"+prg_id+"','Desc':'"+escape(prg_shortdesc)+"','Recertify':'"+recertify_path+"','FullName':'"+escape(full_name)+"'," +
		"'MasMandatory':'"+master_mandatory+"','managerid':'"+managerid+"','UpdatedBy':'"+UpdatedBy+"','UpdatedByName':'"+escape(UpdatedByName)+"','overall_status':'"+overall_status+"'," +
		"'mro_type':'"+mro_name+"','assigned_by':'"+assigned_by+"','updated_by_ins_mngr_slf':'"+updated_by_ins_mngr_slf+"','created_by_ins_mngr_slf':'"+created_by_ins_mngr_slf+"'," +
		"'is_last_rec':'"+isLastRec+ "','BaseType':'" + base_type + "','MasterMandatory':'" + master_mandatory + "','TpTitle':'" + escape(prg_title) +  "','mro':'" + mro + "','regDate':'" + reg_date + "','compDate':'" + comp_date + "','updateDate':'" + update_date + "',"+"'is_exempted':'"+is_exempted+"',"+"'exempted_by':'"+exempted_by+"',"+"'exempted_on':'"+exempted_on+"',"+"'expired_on':'"+expired_on+"',"+"'Object_type':'"+prg_object_type+"',"+"'Overall_status':'"+overall_status+"'}";
 		
	    var data2="data={'MasterEnrollId':'"+master_enroll_id+"','PrgId':'"+prg_id+"','overall_status':'"+overall_status+"'}"; 
		 
		var paramsUpdScore 		= prg_id+"###"+master_enroll_id;
		// Prepare Action button HTML for My Programs 'Enrolled' tab
		if(overall_status == 'lrn_tpm_ovr_enr' || overall_status == 'lrn_tpm_ovr_inp' || overall_status == 'lrn_tpm_ovr_cmp') {
		  //html += '<a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink enroll-launch" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a>';
			if(overall_status == 'lrn_tpm_ovr_cmp' && prg_object_type == 'cre_sys_obt_crt' && recertify_path <= 1 && (recertify_before_certify_expired != 1) && is_current == 'N'){
				html += '<label class="enroll-launch-full">';
				sharefullbutton = 1;
			}
             if(hideShare == 0 ){
		  	html += '<a href="javascript:void(0)" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
			'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ prg_id +
			'\',\'' +prg_object_type+ '\',\'enroll-lp-result-container\',\''+ prg_type +'\');" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
                        	
                        }else if(surStatus != 0 && (availableFunctionalities.exp_sp_surveylearner)){
                            html +="<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" +
													escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"survey\", \"\"," + master_enroll_id + ",\"NULL\",\"enroll-lp-result-container\");' >" +
											titleRestrictionFadeoutImage(Drupal.t("Survey"),'exp-sp-mylearning-menulist') + "</a>";
                        }else if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1){
                            html +="<a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink enroll-launch vtip' " +
							"data=\"" + data2 + "\" " +
								"value='" + Drupal.t("View History") + "' " +
									"name='" + Drupal.t("View History") + "' " +
 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
								">" + titleRestrictionFadeoutImage(Drupal.t("View History"),'exp-sp-mylearning-menulist') + "</a>";
                        }else if(overall_status == 'lrn_tpm_ovr_cmp' && recertify_before_certify_expired == 1 && prg_object_type == 'cre_sys_obt_crt'){
                        	mainRecertifyButton = 1;
                        	var recertifyObj = '$("body").data("learningcore")';
            				var recertifyFrom = 'N';
            				var loaderDiv = 'enroll-lp-result-container';
                        	html += "<a type='button' class='actionLink enroll-launch mypgm-recertify vtip'" +
        					"data=\"" + data1 + "\" " +
        						"value='" + Drupal.t("LBL429") + "' " +
        							"name='" + Drupal.t("LBL429") + "' " +
        							"title='" + Drupal.t("LBL429") + "' " +
        									"onclick='" + recertifyObj + ".callPopupOrNot(\"" +loaderDiv+ "\","+ prg_id + ","+null+",\"" + recertifyFrom +"\","+null+",\""+recertify_path+"-R\");'" +
        										" >"+titleRestrictionFadeoutImage(Drupal.t("LBL429"),'exp-sp-mylearning-menulist')+"</a>";
                        }else if(overall_status == 'lrn_tpm_ovr_cmp' && is_current == 'Y') {
                        	certificateButton = 1;
                        	sharefullbutton = 1;
                        	html += "<a href='javascript:void(0)' class='actionLink enroll-launch-full vtip' data=\"" +
                  		  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + master_enroll_id_encrypted +
                  		  "&certifyfrom=LP&classid=" + prg_id_encrypted + "&userid=" + objEval.lpUserId +
                  		  "\", \"PrintCertificate\"" +
                  		  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
                  		  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + titleRestrictionFadeoutImage(Drupal.t("LBL205"),'exp-sp-mylearning-menulist') + "</a>";
                        }else{
                          var classfull = (assessStatus != 0  && (availableFunctionalities.exp_sp_surveylearner)) ? "enroll-launch" : "enroll-launch-full";
                            html +="<a href='javascript:void(0);' class='actionLink "+classfull+"' data=\"" + data1 +
							"\" name='Cancel' onclick='" + obj + ".dropTrainingPrg(this);' >" +titleRestrictionFadeoutImage(Drupal.t("LBL109"),'exp-sp-mylearning-menulist') + "</a>";
                        
                        sharefullbutton=(assessStatus != 0  && (availableFunctionalities.exp_sp_surveylearner)) ? 0 : 1;
                        cancelAction = 1;
                        }
                        shareactionbuttom = true;
		  if(overall_status == 'lrn_tpm_ovr_cmp' && prg_object_type == 'cre_sys_obt_crt' && recertify_path <= 1 && (recertify_before_certify_expired != 1) && is_current == 'N')
				html += '</label>';
	
		  //click view button open close TP Description
		   var viewcount = 0;
			$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
				viewcount++;
				curObj = $(this).closest("tr").find(".item-long-desc");
				shortObj = $(this).closest("tr").find(".item-short-desc");
				elipsis = $(this).closest("tr").find(".item-elipsis");
				desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
				var isEven = function(viewcount) {
			        return (viewcount % 2 === 0) ? true : false;
			    };
			    if (isEven(viewcount) === false) {
					$(curObj).show();
					$(elipsis).hide();
					$(shortObj).hide();
					$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
					$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
				}else if (isEven(viewcount) === true) {
					$(curObj).hide();
					$(elipsis).show();
					$(shortObj).show();
					$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
					$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

				}
			});
		  var enrollAction = '';
			// Prepare the More menu items HTML in variable enrollAction
		
			// code Chnaged for this Ticket #0034693
			// Allow Re-Certification Before Certification Expired #0012673
		    if(overall_status == 'lrn_tpm_ovr_cmp' && is_current == 'Y' && recertify_before_certify_expired == 1 && prg_object_type == 'cre_sys_obt_crt' && mainRecertifyButton !==1){
				var recertifyObj = '$("body").data("learningcore")';
				var recertifyFrom = 'N';
				var loaderDiv = 'enroll-lp-result-container';
				if(prg_status == 'lrn_lpn_sts_atv'){
				enrollAction += "<li class='action-enable'><a href='javascript:void(0)' class='actionLink vtip' " +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("LBL429") + "' " +
							"name='" + Drupal.t("LBL429") + "' " +
							"title='" + Drupal.t("LBL429") + "' " +
								"onclick='" + recertifyObj + ".callPopupOrNot(\"" +loaderDiv+ "\","+ prg_id + ","+null+",\"" + recertifyFrom +"\","+null+",\""+recertify_path+"-R\");'" +
									" >" + Drupal.t("LBL429") + "</a></li>";
							}
			}// Recerify Changes end #0012673
		//console.log('recertfy history' + recertify_path);
		if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1 && (hideShare == 0 ||  assessStatus != 0 || preassessStatus != 0 || surStatus != 0)){ 
			//	var recertifyObj = '$("body").data("learningcore")';
			//	var recertifyFrom = 'N';
			//	var loaderDiv = 'enroll-lp-result-container';
               //if(hideShare == 0){
				enrollAction += "<li class='action-enable'><a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink' " +
					"data=\"" + data2 + "\" " +
						"value='" + Drupal.t("View History") + "' " +
							"name='" + Drupal.t("View History") + "' " +
 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
								">" + Drupal.t("View History") + "</a></li>";
				//}
				/*
				else{
												   enrollAction +=""; 
												}*/
				
                            }
		
		    // Cp actions commented out
		
		    if (surStatus != 0 && (availableFunctionalities.exp_sp_surveylearner)) {
                        if(hideShare == 0){
				enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" +
													escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"survey\", \"\"," + master_enroll_id + ",\"NULL\",\"enroll-lp-result-container\");' >" +
											Drupal.t("Survey") + "</a></li>"; 
                               }else{
                               enrollAction +='';
                           }
			}
			
			if (assessStatus != 0  && (availableFunctionalities.exp_sp_surveylearner)) {
				var preassObj='';
				var postassObj='';
				var assObj='';
				var callDialogObj = '$("body").data("learningcore")';
				preassObj =surObj+".callTakeSurveyToClass(" + prg_id + ",\"" +
				escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"assessment\", \"" +paramsUpdScore+ "\"," + master_enroll_id + ",1,\"enroll-lp-result-container\");' >";
				if(inpClassCount>0){
					var title = Drupal.t("Assessment");
					var message = escape(Drupal.t("MSG688"));
					assObj=callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");' >";
				}else{
					postassObj =surObj+".callTakeSurveyToClass(" + prg_id + ",\"" +
					escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"assessment\", \"" +paramsUpdScore+ "\"," + master_enroll_id + ",0,\"enroll-lp-result-container\");' >";
				}
				var preassPopup = preassObj;
				var postassPopup = (inpClassCount>0) ? assObj : postassObj;
				if(preassessStatus != 0) { // if pre-assement is there only allowing showing pre-assesment link, Fixed For this Ticket #0036120
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
										"\" name='takesurvey' onclick='" +preassPopup +Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
				}
				if (assessStatus != 0) {
				enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					                   "\" name='takesurvey' onclick='" +postassPopup +Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
			}
			}
			
			if(prg_status == 'lrn_lpn_sts_atv' && shareactionbuttom === null){
				enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" data=\"' + data1 +
				'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ prg_id +
				'\',\'' +prg_object_type+ '\',\'enroll-lp-result-container\',\''+ prg_type +'\');" >' + Drupal.t("Share") + '</a></li>';
			}
			if (overall_status == 'lrn_tpm_ovr_enr' && cancelAction == ''){
			enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
			"\" name='Cancel' onclick='" + obj + ".dropTrainingPrg(this);' >" +Drupal.t("LBL109") + "</a></li>";
			}
			if(sharefullbutton==1){
				enrollAction += '<a style="display:none;" id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a>';
			}else
				enrollAction += '<li class="action-enable" style="display:none;"><a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a></li>';
//				enrollAction += '<li class="action-enable" style="display:none;"><a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a></li>';
			  //click view button open close TP Description
			   var viewcount = 0;
				$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
					viewcount++;
					curObj = $(this).closest("tr").find(".item-long-desc");
					shortObj = $(this).closest("tr").find(".item-short-desc");
					elipsis = $(this).closest("tr").find(".item-elipsis");
					desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
					var isEven = function(viewcount) {
				        return (viewcount % 2 === 0) ? true : false;
				    };
				    if (isEven(viewcount) === false) {
						$(curObj).show();
						$(elipsis).hide();
						$(shortObj).hide();
						$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
					}else if (isEven(viewcount) === true) {
						$(curObj).hide();
						$(elipsis).show();
						$(shortObj).show();
						$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

					}
				});
			// Add the Print Certificate More menu item to HTML in completeAction
			if(overall_status == 'lrn_tpm_ovr_cmp' && is_current == 'Y'){
				/*enrollAction += "<li class='action-enable'><a href='javascript:void(0)' class='actionLink' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + master_enroll_id +
							  "&certifyfrom=LP&classid=" + prg_id + "&userid=" + objEval.lpUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/

				 /*
				   * Start # 0041917 -  Qa link is shown in salesforce app
				   * Added By : Ganesh Babu V, Dec 9th 2014 5:00 PM
				   * Description: Check the print certificate whether it triggers from salesforce or expertusone. callback changed according to trigger
				   * Ticket : #0041917 -  Qa link is shown in salesforce app
				   */

				/* enrollAction += "<li class='action-enable'><a href='javascript:void(0)' class='actionLink' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + master_enroll_id +
							  "&certifyfrom=LP&classid=" + prg_id + "&userid=" + objEval.lpUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/



				   if($("#salesforce_my_transcripts").length) {
					    enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data=\"' + data1 +
						'\" name="Certificate"  onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+master_enroll_id_encrypted+'\',\''+prg_id_encrypted+'\','+objEval.lpUserId+',\'LP\',\'div_my_transcript\',800,900);" >' + Drupal.t("LBL205") + '</a></li>';
				   }else{
					   if(certificateButton != 1) {
					   enrollAction += "<li class='action-enable'><a href='javascript:void(0)' class='actionLink' data=\"" +
						  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + master_enroll_id_encrypted +
						  "&certifyfrom=LP&classid=" + prg_id_encrypted + "&userid=" + objEval.lpUserId +
						  "\", \"PrintCertificate\"" +
						  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
						  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";
				   }
				   }


				   /* End # 0041917 -  Qa link is shown in salesforce app */
			}
			// Add html to show the More menu-button/menu (enabled or disabled as appropriate)
			if(enrollAction !='') {
				if(sharefullbutton==1)
					html += '<div class="enroll-action">' + enrollAction + '</div>';
				else{
					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreLPAction(this)' onblur='" + obj + ".hideMoreLPAction(this)' ></a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + enrollAction + '</ul></div>';
				}
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			}

			if (overall_status == 'lrn_tpm_ovr_cmp'){
				html += "<div id='prg-node-"+nodeId+"' class='rating-block'>";
				html += "<input type='hidden' class= 'prgenroll-node' value = '"+nodeId+"'>";
				html += "<input type = 'hidden' class = 'prgenroll-type' value = '"+prg_object_type+"'>";
				html += star_widget+"</div>";
			}
			setTimeout(function() {
				progressBarRound(dataId, completedPercentage, '','progress_',$("#learningplan-tab-inner").data("learningplan"));
				//$("#learningplan-tab-inner").data("learningplan").progressbarlp(dataId, completedPercentage);
			}, 200);
		}
		// Prepare Action button HTML for My Programs 'Completed' tab
		else if(overall_status == 'lrn_tpm_ovr_inc'){
			// incompleted Tab doesnt need a Survey and Share Button, Fixed For this ticket #0039752 and We can Remove this Code's In Code Refactoring
			// var tpButtonAction = null; // Variable used to determined what buttons to show in the drop-down.
			var completeAction = '';
			/* if (surStatus !=0 && (availableFunctionalities.exp_sp_surveylearner) && overall_status != 'lrn_tpm_ovr_inc') {
					tpButtonAction = 'Survey';
					html += '<a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink enroll-launch" onclick=\''+obj+'.addAccordionToTableLP1(this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a>';
					  //click view button open close TP Description
					   var viewcount = 0;
						$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
							viewcount++;
							curObj = $(this).closest("tr").find(".item-long-desc");
							shortObj = $(this).closest("tr").find(".item-short-desc");
							elipsis = $(this).closest("tr").find(".item-elipsis");
							desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
							var isEven = function(viewcount) {
						        return (viewcount % 2 === 0) ? true : false;
						    };
						    if (isEven(viewcount) === false) {
								$(curObj).show();
								$(elipsis).hide();
								$(shortObj).hide();
								$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
								$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
							}else if (isEven(viewcount) === true) {
								$(curObj).hide();
								$(elipsis).show();
								$(shortObj).show();
								$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
								$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

							}
						});
					completeAction += "<li class='action-enable'><a type='button' class='actionLink' " +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Survey") + "' " +
							"name='" + SMARTPORTAL.t("Survey") + "' " +
									"onclick='" + surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" + escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"survey\",\"\"," + master_enroll_id + ");'" +
										" >"+Drupal.t("Survey")+"</a></li>";
				}

			else if (assessStatus !=0 && (availableFunctionalities.exp_sp_surveylearner)) {
				tpButtonAction = 'Assessment';
				html += "<input type='button' class='actionLink enroll-launch' " +
							"data=\"" + data1 + "\" " +
								"value='" + SMARTPORTAL.t("Assessment") + "' " +
									"name='" + SMARTPORTAL.t("Assessment") + "' " +
											"onclick='" + surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" + escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"assessment\");'" +
												" >";alert(1);
			}
			*/
				// Otherwise show the Refer button
			//else {
				//tpButtonAction = 'Refer';
			 	var viewhistory = false;
				if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1){
					//	var recertifyObj = '$("body").data("learningcore")';
					//	var recertifyFrom = 'N';
					//	var loaderDiv = 'enroll-lp-result-container';
					 viewhistory = true;
					html += "<li class='action-enable'><a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink enroll-launch-full' " +
							"data=\"" + data2 + "\" " +
								"value='" + Drupal.t("View History") + "' " +
									"name='" + Drupal.t("View History") + "' " +
		 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
										">" + Drupal.t("View History") + "</a></li>";
				}
				if(viewhistory)
					html += '<label class="enroll-launch-full" style="display:none">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
				else
					html += '<label class="enroll-launch-full">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
				
				//html += '<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink enroll-launch clsDisabledButton" >'+Drupal.t("LBL816")+'</a>';
				  //click view button open close TP Description
				   var viewcount = 0;
					$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
						viewcount++;
						curObj = $(this).closest("tr").find(".item-long-desc");
						shortObj = $(this).closest("tr").find(".item-short-desc");
						elipsis = $(this).closest("tr").find(".item-elipsis");
						desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
						var isEven = function(viewcount) {
					        return (viewcount % 2 === 0) ? true : false;
					    };
					    if (isEven(viewcount) === false) {
							$(curObj).show();
							$(elipsis).hide();
							$(shortObj).hide();
							$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
							$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
						}else if (isEven(viewcount) === true) {
							$(curObj).hide();
							$(elipsis).show();
							$(shortObj).show();
							$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
							$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

						}
					});

				/*if(prg_status == 'lrn_lpn_sts_atv'){
				completeAction += "<li class='action-enable'><a type='button' class='actionLink' " +
				"data=\"" + data1 + "\" " +
					"value='" + Drupal.t("Share") + "' " +
						"name='" + SMARTPORTAL.t("Refer") + "' " +
								'onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ prg_id +
									'\',\'' +prg_object_type+ '\',\'enroll-lp-result-container\',\''+ prg_type +'\');" ' +
										" >"+Drupal.t("Share")+"</a></li>";
				}
			}
			// Prepare the More menu items HTML in completeAction
			if(tpButtonAction != 'Refer'){
				completeAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" data="'
										+ data1 + '" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\'' +
										prg_id + '\',\'' +prg_object_type+ '\',\'enroll-lp-result-container\',\''+ prg_type +'\');" >' +
										Drupal.t("Share") + '</a></li>';
			}*/

			// Add the Rating More menu item to HTML in completeAction
			// Add html to show the more menu (enabled or disabled as appropriate)
			/*// Incomplete - options disabled		
			if(completeAction !=''){
				html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreLPAction(this)' onblur='" + obj + ".hideMoreLPAction(this)'></a></div>";
				html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			} */
			//html += "<a class='enroll-launch-more-gray'></a>";
		}
		// Prepare Action button HTML for My Programs 'Pending' tab
		else if (overall_status == 'lrn_tpm_ovr_ppm' || overall_status == 'lrn_tpm_ovr_ppv' || overall_status == 'lrn_tpm_ovr_wtl' ) {
			var completeAction = '';
			var viewhistory = false;
			if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1){
					viewhistory = true;
					completeAction += "<li class='action-enable'><a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink' " +
						"data=\"" + data2 + "\" " +
							"value='" + Drupal.t("View History") + "' " +
								"name='" + Drupal.t("View History") + "' " +
	 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
									">" + Drupal.t("View History") + "</a></li>";
				}
			
			if(viewhistory){
			    html += "<a type='button' class='actionLink enroll-launch' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" onclick='"+obj+".dropTrainingPrg(this);' name='"+SMARTPORTAL.t("Cancel")+"' >"+Drupal.t("LBL109")+"</a>";
			}else{
			    html += "<a type='button' class='actionLink enroll-launch-full' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" onclick='"+obj+".dropTrainingPrg(this);' name='"+SMARTPORTAL.t("Cancel")+"' >"+Drupal.t("LBL109")+"</a>";
			}    
				  //click view button open close TP Description
				   var viewcount = 0;
					$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
						viewcount++;
						curObj = $(this).closest("tr").find(".item-long-desc");
						shortObj = $(this).closest("tr").find(".item-short-desc");
						elipsis = $(this).closest("tr").find(".item-elipsis");
						desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
						var isEven = function(viewcount) {
					        return (viewcount % 2 === 0) ? true : false;
					    };
					    if (isEven(viewcount) === false) {
							$(curObj).show();
							$(elipsis).hide();
							$(shortObj).hide();
							$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
							$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
						}else if (isEven(viewcount) === true) {
							$(curObj).hide();
							$(elipsis).show();
							$(shortObj).show();
							$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
							$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

						}
					});
			    completeAction += "<li class='action-enable' style=\"display:none;\">"+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" >'+Drupal.t("LBL816")+'</a>'+"</li>";
			    /*if(completeAction !=''){
					html += "<div class='dd-btn'><span class='dd-arrow'></span><a  title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreLPAction(this)' onblur='" + obj + ".hideMoreLPAction(this)'></a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
				}else{
					html += "<a class='enroll-launch-more-gray'></a>";
				}*/
				if(viewhistory){
					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreLPAction(this)' onblur='" + obj + ".hideMoreLPAction(this)'></a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
				}
				else{
			    html += completeAction + "<a class='enroll-launch-more-gray'></a>";
		}
			   // html += completeAction + "<a class='enroll-launch-more-gray'></a>";
		}
		// Prepare Action button HTML for My Programs 'Expired' tab
		else if(overall_status == 'lrn_tpm_ovr_exp'){
			var completeAction = '';
			var historyAvail = '';
			if (prg_object_type == 'cre_sys_obt_crt') {
				var recertifyObj = '$("body").data("learningcore")';
				var recertifyFrom = 'N';
				var loaderDiv = 'enroll-lp-result-container';
				if(prg_status == 'lrn_lpn_sts_atv' && recertify_before_certify_expired==1){
					html += "<a type='button' class='actionLink enroll-launch mypgm-recertify vtip'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("LBL429") + "' " +
							"name='" + Drupal.t("LBL429") + "' " +
							"title='" + Drupal.t("LBL429") + "' " +
									"onclick='" + recertifyObj + ".callPopupOrNot(\"" +loaderDiv+ "\","+ prg_id + ","+null+",\"" + recertifyFrom +"\","+null+",\""+recertify_path+"-R\");'" +
										" >"+titleRestrictionFadeoutImage(Drupal.t("LBL429"),'exp-sp-mylearning-menulist')+"</a>";
					html += '<li class="action-enable" style="display:none"> <a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink " onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a> </li>';
				} else if (prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1){
				//historyAvail = true;
				   html += "<a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink enroll-launch mypgm-recertify vtip' " +
						"data=\"" + data2 + "\" " +
							"value='" + Drupal.t("View History") + "' " +
								"name='" + Drupal.t("View History") + "' " +
								"title='" + Drupal.t("View History") + "' " +
	 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
									">" + titleRestrictionFadeoutImage(Drupal.t("View History"),'exp-sp-mylearning-menulist') + "</a>";
														html += '<li class="action-enable" style="display:none"> <a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink " onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'>'+Drupal.t("LBL816")+'</a> </li>';
									
				} else {
				//html += '<label class="enroll-launch-full">'+'<a id="prg-accodion-prg_'+master_enroll_id+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \' class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl">'+Drupal.t("LBL816")+'</a>'+'</label>';
				html += '<label class="enroll-launch-full">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
				}
				
			if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1 && prg_status == 'lrn_lpn_sts_atv' && recertify_before_certify_expired==1){
					historyAvail = true;
					completeAction += "<li class='action-enable'><a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink' " +
						"data=\"" + data2 + "\" " +
							"value='" + Drupal.t("View History") + "' " +
								"name='" + Drupal.t("View History") + "' " +
	 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
									">" + titleRestrictionFadeoutImage(Drupal.t("View History"),'exp-sp-mylearning-menulist')  + "</a></li>";
				}
			}
			  //click view button open close TP Description
			   var viewcount = 0;
				$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
					viewcount++;
					curObj = $(this).closest("tr").find(".item-long-desc");
					shortObj = $(this).closest("tr").find(".item-short-desc");
					elipsis = $(this).closest("tr").find(".item-elipsis");
					desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
					var isEven = function(viewcount) {
				        return (viewcount % 2 === 0) ? true : false;
				    };
				    if (isEven(viewcount) === false) {
						$(curObj).show();
						$(elipsis).hide();
						$(shortObj).hide();
						$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
					}else if (isEven(viewcount) === true) {
						$(curObj).hide();
						$(elipsis).show();
						$(shortObj).show();
						$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

					}
				});
 			// Add html to show the more menu (enabled or disabled as appropriate)
			// Expried content - disable arrow options
			if(historyAvail){
				html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreLPAction(this)' onblur='" + obj + ".hideMoreLPAction(this)'></a></div>";
				html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
			}
			else{
				if (prg_object_type == 'cre_sys_obt_crt')
					html += completeAction + "<a class='enroll-launch-more-gray'></a>";	
			} 
			//	html += completeAction + "<a class='enroll-launch-more-gray'></a>";	
		}
		else if(overall_status == 'lrn_tpm_ovr_cln'){
			 var viewhistory = false;
			if(prg_object_type == 'cre_sys_obt_crt' && recertify_path > 1){
				//	var recertifyObj = '$("body").data("learningcore")';
				//	var recertifyFrom = 'N';
				//	var loaderDiv = 'enroll-lp-result-container';
				 viewhistory = true;
				html += "<li class='action-enable'><a id=\"mod-accodion-"+prg_id+"\"  href='javascript:void(0)' class='actionLink enroll-launch-full' " +
						"data=\"" + data2 + "\" " +
							"value='" + Drupal.t("View History") + "' " +
								"name='" + Drupal.t("View History") + "' " +
	 								'onclick=\''+obj+'.addAccordionToTableModule('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \''+
									">" + titleRestrictionFadeoutImage(Drupal.t("View History"),'exp-sp-mylearning-menulist') + "</a></li>";
			}
			if(viewhistory)
				html += '<label class="enroll-launch-full" style="display:none">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
			else
				html += '<label class="enroll-launch-full">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
			
			
			//html += '<label class="enroll-launch-full">'+'<span id="prg-accodion-prg_'+master_enroll_id+'" data="'+data1+'" onclick=\''+obj+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getPrgDetails,this,'+obj+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
			
			
			  //click view button open close TP Description
			   var viewcount = 0;
				$('#prg-accodion-prg_'+master_enroll_id).live("click",function(){
					viewcount++;
					curObj = $(this).closest("tr").find(".item-long-desc");
					shortObj = $(this).closest("tr").find(".item-short-desc");
					elipsis = $(this).closest("tr").find(".item-elipsis");
					desType=$(this).closest("tr").find(".more-text").children("a").attr('class');
					var isEven = function(viewcount) {
				        return (viewcount % 2 === 0) ? true : false;
				    };
				    if (isEven(viewcount) === false) {
						$(curObj).show();
						$(elipsis).hide();
						$(shortObj).hide();
						$(this).closest("tr").find(".more-text").children(".show-short-text").removeClass("show-short-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-full-text");
					}else if (isEven(viewcount) === true) {
						$(curObj).hide();
						$(elipsis).show();
						$(shortObj).show();
						$(this).closest("tr").find(".more-text").children(".show-full-text").removeClass("show-full-text");
						$(this).closest("tr").find(".more-text").children("a").addClass("show-short-text");

					}
				});
		}
		if (overall_status == 'lrn_tpm_ovr_enr' || overall_status == 'lrn_tpm_ovr_inp' || overall_status == 'lrn_tpm_ovr_cmp') {
			html += '<div class="progress" id="progress_'+dataId+'"></div>';	
		}	
		html += "</div>";
		return html;
		}catch(e){
			// to do
			// console.log(e);
		}
	},
	addAccordionToTableLP1 : function(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableLPView(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		}catch(e){
			// to do
		}
		vtip();
	},
	addAccordionToTableLPView : function(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var currTr = $(obj).parents("tr").attr("id");
		var data = eval($("#prg-accodion-"+currTr).attr("data"));
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#completeBy"+prg_id).show();
			$("#"+currTr).after("<tr id='"+currTr+"SubGrid' data-rowid='"+currTr+"' class='jqgcrow ui-row-"+currTr+"'><td></td><td colspan='2'><div class='course-detail-section-innerRecord' id='course-detail-section-"+data.MasterEnrollId+"'></div></td></tr>");
			var noBorderLastRecClass = '';
			if (!$("#pager-lp").is(":visible") && data.is_last_rec == 'last') {
				noBorderLastRecClass = 'noborder';
			}
			$("#"+currTr+"SubGrid").after("<tr class='ui-widget-content empty-border " + noBorderLastRecClass + "' id='"+currTr+"LpEmptyRow'><td colspan='3' style='padding:0px 0 0 0;'></td></tr>");
			$("#"+currTr+"SubGrid").show();//css("display","block");
			if(this.currTheme != 'expertusoneV2') {
			$("#prg-accodion-"+currTr).removeClass("title_close");
			$("#prg-accodion-"+currTr).addClass("title_open");
			}
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"SubGrid").slideDown(1000);
			if($("#"+currTr+"ModuleGrid").is(":visible")){
				//console.log("#"+currTr+"ModuleGrid" + ": ModuleGrid is visible");
				$("#"+currTr+"ModuleGrid").prev("tr.empty-border").find("td").css("border","0");
			}
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display");
			$("#completeBy"+prg_id).hide();
			if((clickStyle == "none") || (clickStyle == 'undefined')) {
	    	  $("#completeBy"+prg_id).show();
	    	  $("#"+currTr+"SubGrid").show();//css("display","block");
	    	  $("#"+currTr+"SubGrid").slideDown(1000);
	    	  if(this.currTheme != 'expertusoneV2') {
	    		  $("#prg-accodion-"+currTr).removeClass("title_close");
	    		  $("#prg-accodion-"+currTr).addClass("title_open");
	    	  }
			  $("#"+currTr).removeClass("ui-widget-content");
			  $("#"+currTr+"LpEmptyRow").addClass("ui-widget-content empty-border");
    		} else {
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$("#"+currTr+"SubGrid").slideUp(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#prg-accodion-"+currTr).removeClass("title_open");
    				$("#prg-accodion-"+currTr).addClass("title_close");
    			}
				$("#"+currTr).removeClass("ui-widget-content");
				$("#"+currTr).addClass("ui-widget-content");
				$("#"+currTr+"LpEmptyRow").removeClass("ui-widget-content empty-border");
    		}
		}
		
		var prgDetSec = '';
		var classDetSec = this.getLPClassDetail(data);
		}catch(e){
			// to do
		}
	},
	addAccordionToTableModule : function(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableLPModuleView(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		}catch(e){
			// to do
		}
		vtip();
	},
	
	addAccordionToTableLPModuleView : function(prg_id,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{		
		var currTr = $(obj).parents("tr").attr("id");
		var data = eval($("#mod-accodion-"+prg_id).attr("data"));		
		var PrgId = data.PrgId;

		//this.getLPModuleDetail(data);
		if(!document.getElementById(currTr+"ModuleGrid")) {
			//$("#completeBy"+prg_id).show();
			var afterElement = ($("#"+currTr+"SubGrid").length) ? "#lp_seeless_tr_"+currTr : "#"+currTr; // "#" +currTr+"SubGrid"  
			$(afterElement).after("<tr id='"+currTr+"ModuleGrid' data-rowid='"+currTr+"' class='jqgcrow ui-row-"+currTr+"'><td colspan='3'><div class='Module-detail-section-innerRecord' id='Module-detail-section-"+data.PrgId+"'></div></td></tr>");
			var noBorderLastRecClass = '';
			if (!$("#pager-lp").is(":visible") && data.is_last_rec == 'last') {
				noBorderLastRecClass = 'noborder';
			}
			//$("#"+currTr+"ModuleGrid").after("<tr class='ui-widget-content empty-border " + noBorderLastRecClass + "' id='"+currTr+"LpEmptyRow'><td colspan='3' style='padding:0px 0 0 0;'></td></tr>");
			$("#"+currTr+"ModuleGrid").show();//css("display","block");
			if(this.currTheme != 'expertusoneV2') {
			$("#prg-accodion-"+currTr).removeClass("title_close");
			$("#prg-accodion-"+currTr).addClass("title_open");
			}
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ModuleGrid").slideDown(1000);
		} else {
			var clickStyle = $("#"+currTr+"ModuleGrid").css("display");
			$("#completeBy"+prg_id).hide();
      if((clickStyle == "none") || (clickStyle == 'undefined')) {
    	  $("#completeBy"+prg_id).show();
    	  $("#"+currTr+"ModuleGrid").show();//css("display","block");
    	  $("#"+currTr+"ModuleGrid").slideDown(1000);
    	  if(this.currTheme != 'expertusoneV2') {
    		  $("#prg-accodion-"+currTr).removeClass("title_close");
    		  $("#prg-accodion-"+currTr).addClass("title_open");
    	  }
		  $("#"+currTr).removeClass("ui-widget-content");
		 // $("#"+currTr+"LpEmptyRow").addClass("ui-widget-content empty-border");
    		} else {
    			$("#"+currTr+"ModuleGrid").hide();//css("display","none");
    			$("#"+currTr+"ModuleGrid").slideUp(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#prg-accodion-"+currTr).removeClass("title_open");
    				$("#prg-accodion-"+currTr).addClass("title_close");
    			}
				$("#"+currTr).removeClass("ui-widget-content");
				$("#"+currTr).addClass("ui-widget-content");
				$("#"+currTr+"SubGrid").prev("tr.ui-state-highlight").find("td").css("border","0");
				if($("#"+currTr+"ModuleGrid").is(":hidden")){
					$("#"+currTr+"ModuleGrid").prev("tr.empty-border").find("td").css("border-bottom","1px solid #ededed");
				}
				//$("#"+currTr+"LpEmptyRow").removeClass("ui-widget-content empty-border");
    		}
		} 
		var prgDetSec = '';
			//this.getLPModuleDetail(data);
		var classDetSec = this.getLPModuleDetail(data);
		}catch(e){
			// to do
		}
	},
	  
	getLPModuleDetail : function(data){
		try{
			var prgId = data.PrgId;			
			var userId = this.lpUserId;
			searchEnrollStr	= '&UserID='+userId;
 			this.createLoader('enroll-lp-result-container');
			//$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
			//var url = this.constructUrl("ajax/lp-modulelist/" + masterEnrollId + "/" + prgId + "/" + recertifyPath + "/" +object_type + "/" + overall_status);
			var obj = this;
			$.ajax({
				type: "POST",
				url: '?q=ajax/lp-modulelist/'+prgId+searchEnrollStr,				
				success: function(result){
					obj.destroyLoader('enroll-lp-result-container');	
					obj.paintModuleDetails(result,data); 
					$('.module-level .limit-title').trunk8(trunk8.myprogramModule_title);
					$('.module-level .limit-desc').trunk8(trunk8.myprogramModule_desc);
				} 
		    });
 
		}catch(e){
			// to do
		}
	},
	
	paintModuleDetails : function(result) {
		try{
			var userId = this.lpUserId;
			var rhtml="",objStr="",img="",cert="",cert_on="";
			 
			var pipe = '<span class="enroll-pipeline">|</span>';
	 		rhtml += "<div class='module-level'>";
			rhtml += "<table border='0' cellpadding='0' class='module-show-morecourse' cellspacing='0'>";	  	
			var objEval = '$("#learningplan-tab-inner").data("learningplan")';
		 	
			for(c=0; c < result.length ; c++){
				var master_enroll_id 		= result[c]['master_enroll_id'];
				var user_id 				= result[c]['user_id'];
				var overall_status 			= result[c]['overall_status'];
		 		var reg_date				= result[c]['reg_date'];		 		
				var cancel_date				= result[c]['cancel_date'];
				var comp_date				= result[c]['comp_date'];
				var update_date				= result[c]['update_date'];
				var percentage_complete		= result[c]['percentage_complete'];
				var prg_id					= result[c]['prg_id'];
				var base_type 				= result[c]['base_type'];
				var prg_title				= result[c]['prg_title'];
				var prg_code 				= result[c]['prg_code'];
				var prg_desc				= result[c]['prg_desc'];
				var prg_shortdesc 			= result[c]['prg_shortdesc'];
				var prg_fulldesc 			= encodeURIComponent(result[c]['prg_fulldescription']);
				var prg_start_date 			= result[c]['prg_start_date'];
				var prg_end_date			= result[c]['prg_end_date'];
				var prg_status 				= result[c]['prg_status'];				
				var prg_object_type 		= result[c]['prg_object_type'];
				var prg_type 				= result[c]['prg_type'];
				var master_mandatory        = result[c]['master_mandatory'];
				var full_name               = result[c]['full_name'];
				var managerid               = result[c]['managerid'];
				var mro						= result[c]['mro'];
				var surStatus				= result[c]['survey_status'];
				var assessStatus			= result[c]['assessment_status']; 
				var preassessStatus			= result[c]['preassessment_status']; 
				var maxScoreValidationpre	= result[c]['maxscorevaluepre'];
				var maxScoreValidationpost	= result[c]['maxscorevaluepost'];
				var inpClassCount			= result[c]['inp_class_count'];
				var star_widget				= result[c]['star_widget'];
				var nodeId					= result[c]['node_id'];
				var recertify_path			= result[c]['recertify_path'];
				var UpdatedBy            	= result[c]['updated_by'];
				var UpdatedByName        	= result[c]['updated_by_name'];
				var created_by_ins_mngr_slf =  result[c]['created_by_ins_mngr_slf'];
				var updated_by_ins_mngr_slf =  result[c]['updated_by_ins_mngr_slf'];
				var isLastRec             =  (typeof result[c]['is_last_rec'] != 'undefined' && result[c]['is_last_rec'] == 'last')? 'last' : '';
				var mro_name				= result[c]['mro_name'];
				var assigned_by				= unescape(result[c]['assigned_by']).replace(/"/g, '&quot;');
				var baseType				= result[c]['basetype'];
				var startTime 				= result[c]['session_start'];
				var is_current 				= result[c]['is_current'];
				var master_enroll_id_encrypted		= result[c]['master_enroll_id_encrypted'];
				var prg_id_encrypted 				= result[c]['prg_id_encrypted'];
				var recertify_before_certify_expired = result[c]['recertify_before_certify_expired'];			
				var is_exempted		= result[c]['is_exempted'];
				var exempted_by = result[c]['exempted_by'];
				var exempted_on = result[c]['exempted_on'];
				var exp_on = result[c]['expired_date'];				
				var img = result[c]['learning_type_image'];
				var prg_object_type_code 	= result[c]['prg_object_type'];
				var lnrAttach						= result[c]['show_lnr_lp_attach'];
	 			var expires_on_lbl                  = result[c]['expires_on_lbl'];
				var IsHasAssessment = false;
				var IsHasSurvey = false;
				
				if(recertify_path > 1) {
					cert =  "Recertification";
					cert_on = "Recertified";						
				} else {
					cert =  "Certification";
					cert_on = "Certified";
				}
			 
				var passParams ='';
				if(lnrAttach.length>0) {
					 $(lnrAttach).each(function(){
						 inc=inc+1;
						 passParams += "{";
						 passParams += "'Id':'"+$(this).attr("attachment_id")+"'";
						 /*--Issue fix for the ticket - 32781 --*/
						 passParams += ",'Title':'"+escape($(this).attr("reading_title"))+"'";
						 passParams += ",'url':'"+encodeURI($(this).attr("reading_content")).replace(/'/g, '%27')+"'";
						 passParams += "}";
						 if(inc<lnrAttach.length) {
							 passParams += ",";
						 }
					 });
				}
				var attachdata = "data=["+passParams+"]";
			
			var surObj 	= '$("#block-take-survey").data("surveylearner")';
			var preassObj='';
			var paramsUpdScore 		= prg_id+"###"+master_enroll_id;
 			//Added to display the pre assessment for the TP which is re-certified(in the module path).
			preassObj =surObj+".callTakeSurveyToClass(" + prg_id + ",\"" +
					escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"assessment\", \"" +paramsUpdScore+ "\"," + master_enroll_id + ",1,\"enroll-lp-result-container\");' >";
			
			postassObj =surObj+".callTakeSurveyToClass(" + prg_id + ",\"" +
					escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"assessment\", \"" +paramsUpdScore+ "\"," + master_enroll_id + ",0,\"enroll-lp-result-container\");' >";
			 
				
				var data1="data={'RegId':'" + master_enroll_id + "','MasterEnrollId':'"+master_enroll_id+"','PrgId':'"+prg_id+"','Desc':'"+escape(prg_shortdesc)+"','Recertify':'"+recertify_path+"','FullName':'"+escape(full_name)+"'," +
				"'MasMandatory':'"+master_mandatory+"','managerid':'"+managerid+"','UpdatedBy':'"+UpdatedBy+"','UpdatedByName':'"+escape(UpdatedByName)+"','overall_status':'"+overall_status+"'," +
				"'mro_type':'"+mro_name+"','assigned_by':'"+assigned_by+"','updated_by_ins_mngr_slf':'"+updated_by_ins_mngr_slf+"','created_by_ins_mngr_slf':'"+created_by_ins_mngr_slf+"'," +
				"'is_last_rec':'"+isLastRec+ "','BaseType':'" + base_type + "','MasterMandatory':'" + master_mandatory + "','TpTitle':'" + escape(prg_title) +  "','mro':'" + mro + "','regDate':'" + reg_date + "','compDate':'" + comp_date + "','updateDate':'" + update_date + "',"+"'is_exempted':'"+is_exempted+"',"+"'exempted_by':'"+exempted_by+"',"+"'exempted_on':'"+exempted_on+"',"+"'Object_type':'"+prg_object_type+"',"+"'Overall_status':'"+overall_status+"'}";
				
				var enrollAction = ''; 			
			
			 	var moduleId = 'mod_'+master_enroll_id;
			 	rhtml += '<tr class="jqgcrow ui-row-ltr ui-widget-content ui-row-'+moduleId+'" id ="'+moduleId+'" data-rowid="'+moduleId+'">';				
	 			rhtml += '<td><div title="' +cert + '" class="'+ img +' vtip"></div></td><td>';	 
	 			rhtml += '<div class="enroll-lp-course-title"><span id="lp_attachment_'+master_enroll_id+'" data="'+attachdata+'"><span id="titlePrgEn_'+master_enroll_id+'" class="item-title limit-title-row ' + overall_status + '" ><span class="limit-title limit-title-lp enroll-lp-class-title vtip" title="'+prg_title+'" href="javascript:void(0);">'+prg_title+'</span></span></div>';
	 			rhtml += '<div class="enroll-tp-title-info"><div class="item-title-code">';
	 			rhtml += '<span title="Code : test_curr" class="vtip">';
	 			var expmsglbl = expires_on_lbl == 'yes' ? Drupal.t('LBL735') : Drupal.t('LBL028');
	 			if(overall_status == 'lrn_tpm_ovr_cmp')
	 				rhtml += '</span>'+cert+pipe+Drupal.t("LBL704")+' : '+reg_date+pipe+cert_on+' on  : '+comp_date+pipe+ expmsglbl+' : '+exp_on+'';
	 			else if(overall_status == 'lrn_tpm_ovr_cln')
	 				rhtml += '</span>'+cert+pipe+Drupal.t("LBL704")+' : '+reg_date +pipe+ Drupal.t('LBL026')+' : '+cancel_date;
	 			else if(overall_status == 'lrn_tpm_ovr_inc')
	 				rhtml += '</span>'+cert+pipe+Drupal.t("LBL704")+' : '+reg_date +pipe+ Drupal.t('LBL1193')+' : '+comp_date;
	 				
	 			if(prg_shortdesc != null && prg_shortdesc != 'null') {
	 			    	var prg_description_icon = decodeURIComponent(prg_shortdesc) && decodeURIComponent(prg_fulldesc);
	 			    	var tptype = prg_object_type_code;
	 			    	rhtml += '<div class="learningplan-desc-div">';
	 			    	rhtml += '<div class="limit-desc-row '+tptype+'">';
	 			    	rhtml += '<div class="recordDiv FindTrngTxt"><span class="limit-desc limit-desc-lp vtip" id="LPShortDesc_mod_'+master_enroll_id+'"><span class="cls-learner-descriptions">'+prg_description_icon+'</span></span></div>';
	 			    	rhtml += '</div>';
	 			}
	 			var dataId = 'mod_' + master_enroll_id;

	 			rhtml += '<div class="lp_seemore" id="lp_seemore_prg_'+master_enroll_id+'" onclick="seeMoreMyLearning('+master_enroll_id+',\'tprecertify\');">'+ Drupal.t('LBL713') +'</div>';
	 
	 			rhtml +='<td><div id="enroll-lp-main-action-'+master_enroll_id+'"" class="enroll-lp-main-list">';
	 			/*assessStatus = 0;
	 			surStatus = 0;*/
	 			//Changed to display the pre assessment for the TP which is re-certified.
	 			rhtml += '<label class="enroll-launch-full vtip" style="display:none;">'+'<span id="prg-accodion-mod_'+master_enroll_id+'" data="'+data1+'" onclick=\''+objEval+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getPrgDetails,this,'+objEval+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
	 		 	if (preassessStatus != 0 && overall_status != "lrn_tpm_ovr_cln") { 
	 				var className = (surStatus != 0 && (availableFunctionalities.exp_sp_surveylearner)) ? 'enroll-launch' : 'enroll-launch-full';
	 				rhtml += "<a href='javascript:void(0);' title='"+Drupal.t("Assessment") + ' ' +Drupal.t("LBL1253")+"' class='actionLink "+className+"' data=\"" + data1 +
					                   "\" name='takesurvey' onclick='" +preassObj +titleRestrictionFadeoutImage(Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" ,'exp-sp-mylearning-menulist')+ "</a></li>"; 
		      	}  
	 		 	// console.log('overall_status::'+overall_status);
	 		 	if (assessStatus != 0 && overall_status != "lrn_tpm_ovr_cln" && overall_status != "lrn_tpm_ovr_inc") { 
	 		 		if(preassessStatus == 0){
	 		 			var className = (surStatus != 0 && (availableFunctionalities.exp_sp_surveylearner)) ? 'enroll-launch' : 'enroll-launch-full';
	 		 			rhtml += "<a href='javascript:void(0);' title='"+Drupal.t("Assessment") + ' ' +Drupal.t("LBL871")+"' class='actionLink "+className+"' data=\"" + data1 +
		                   "\" name='takesurvey' onclick='" +postassObj +titleRestrictionFadeoutImage(Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" ,'exp-sp-mylearning-menulist')+ "</a></li>";
	 		 		}else{
	 		 		
	 		 			enrollAction += "<li class='action-enable'><a href='javascript:void(0);' title='"+Drupal.t("Assessment") + ' ' +Drupal.t("LBL871")+"' class='actionLink' data=\"" + data1 +
		                   "\" name='takesurvey' onclick='" +postassObj +Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>"+ "</a></li>";	
	 		 		}
	 				 
	 			}
	 		 	
	 			if (surStatus != 0 && (availableFunctionalities.exp_sp_surveylearner) && overall_status != "lrn_tpm_ovr_cln" && overall_status != "lrn_tpm_ovr_inc") {
	 				//var className = (assessStatus != 0) ? 'enroll-launch' : 'enroll-launch-full';
	 				if(assessStatus != 0){
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
										"\" name='takesurvey' onclick='" +
											surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" +
														escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"survey\", \"\"," + master_enroll_id + ",\"NULL\",\"enroll-lp-result-container\");' >" +
												Drupal.t("Survey") + "</a></li>";
	 				}else{
	 					rhtml += "<a href='javascript:void(0);' class='actionLink enroll-launch-full' data=\"" + data1 +
						"\" name='takesurvey' onclick='" +
							surObj + ".callTakeSurveyToClass(" + prg_id + ",\"" +
										escape(prg_title) + "\", \"" +prg_object_type+ "\",  \"survey\", \"\"," + master_enroll_id + ",\"NULL\",\"enroll-lp-result-container\");' >" +
								Drupal.t("Survey") + "</a>";
	 				}
				}
	 			if((assessStatus == 0 && surStatus == 0) || overall_status == "lrn_tpm_ovr_cln" || overall_status == "lrn_tpm_ovr_inc"){//Added for if the tp have survey and it is in canceled status.
					rhtml += '<label class="enroll-launch-full">'+'<span id="prg-accodion-mod_'+master_enroll_id+'" data="'+data1+'" onclick=\''+objEval+'.addAccordionToTableLP1('+prg_id+',this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getPrgDetails,this,'+objEval+',true); \'></span><a  href="javascript:void(0);" class="actionLink clsDisabledButton enroll-launch enroll-launch-full launch_button_lbl" >'+Drupal.t("LBL816")+'</a>'+'</label>';
				}
	 		
	 		     if(enrollAction !='') { 
	 		    	rhtml += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + objEval + ".showMoreLPAction(this)' onblur='" + objEval + ".hideMoreLPAction(this)' ></a></div>";
	 		    	rhtml += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + enrollAction + '</ul></div>';
	 		    	enrollAction = '';
				}
	 			rhtml +=  '</td></tr>';
	 			
	 			
		  }
		  rhtml += '</table>';
		  rhtml += '</div>';
 		 
			//rhtml = "<tr class='ui-widget-content jqgrow ui-row-ltr' role='row' tabindex='-1' id='prg_8'><td aria-describedby='paintEnrollmentLPResults_Image' style='text-align: center; padding-bottom: 15px; border: 0px none;' role='gridcell'><div class='cert-icon vtip' title='Certification'></div></td><td aria-describedby='paintEnrollmentLPResults_Name' class='enroll-datatable-column1' style='text-align: left; padding-bottom: 15px; border: 0px none;' role='gridcell'><div class='enroll-lp-course-title'><span data='data=[]' id='lp_attachment_8'></span><span class='item-title lrn_tpm_ovr_enr' id='titlePrgEn_8'><span href='javascript:void(0);' title='test_lnr_crs_tp' class='enroll-lp-class-title vtip'><div class='fade-out-title-container exp_sp_lnrmyprogram_title_lrn_tpm_ovr_enr'><span class='title-lengthy-text'>test_lnr_crs_tp</span></div></span></span></div><span class='session-time-zone'><div class='ui-progressbar'><div class='ui-progress-curve'>&nbsp;</div><div class='progress-status'>0% Completed</div><div class='ui-progressbar-value' style='width: 0%;'>&nbsp;</div></div></span><div class='enroll-tp-title-info'><div class='item-title-code'><span title='Code : test_lnr_crs_tp' class='vtip'><div class='fade-out-title-container exp-sp-lnrmyprogram-prg-code'><span class='title-lengthy-text'>test_lnr_crs_tp</span></div></span><span class='enroll-pipeline'>|</span>Completed : 0<span class='enroll-pipeline'>|</span>In progress : 2<span class='enroll-pipeline'>|</span>Incomplete : 0 </div><div class='learningplan-desc-div'><div class='recordDiv FindTrngTxt'><span id='LPShortDesc_prg_8'><span class='item-short-desc'>test_lnr_crs_tp</span></span></div></div></div></td><td aria-describedby='paintEnrollmentLPResults_Action' class='enroll-datatable-column2' style='text-align: right; padding-bottom: 15px; border: 0px none;' role='gridcell'><div id='enroll-lp-main-action-8' class='enroll-lp-main-list'><a onclick='$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;).addAccordionToTableLP1(154,this.className,&quot;0 0&quot;,&quot;0 -61px&quot;,&quot;dt-child-row-En&quot;,$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;).getPrgDetails,this,$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;),true); ' class='actionLink enroll-launch' data='data={'RegId':'8','MasterEnrollId':'8','PrgId':'154','Desc':'test_lnr_crs_tp','Recertify':'2','FullName':'gayathri%20k','MasMandatory':'0','managerid':'30','UpdatedBy':'30','UpdatedByName':'gayathri%20k','overall_status':'lrn_tpm_ovr_enr','mro_type':'','assigned_by':'','updated_by_ins_mngr_slf':'Me','created_by_ins_mngr_slf':'Me','is_last_rec':'last','BaseType':'TP','MasterMandatory':'0','TpTitle':'test_lnr_crs_tp','mro':'null','regDate':'Aug 10, 2016','compDate':'Aug 10, 2016','updateDate':'Aug 10, 2016','is_exempted':'','exempted_by':'','exempted_on':'','Object_type':'cre_sys_obt_crt','Overall_status':'lrn_tpm_ovr_enr'}' href='javascript:void(0);' id='prg-accodion-prg_8'>View</a><div class='dd-btn'><span class='dd-arrow'></span><a onblur='$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;).hideMoreLPAction(this)' onclick='$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;).showMoreLPAction(this)' title='More' class='enroll-launch-more'></a></div><div class='enroll-action'><ul class='enroll-drop-down-list'><li class='action-enable'><a onclick='$('body').data('refercourse').getReferDetails('154','cre_sys_obt_crt','enroll-lp-result-container','Certification');' name='Refer' data='data={'RegId':'8','MasterEnrollId':'8','PrgId':'154','Desc':'test_lnr_crs_tp','Recertify':'2','FullName':'gayathri%20k','MasMandatory':'0','managerid':'30','UpdatedBy':'30','UpdatedByName':'gayathri%20k','overall_status':'lrn_tpm_ovr_enr','mro_type':'','assigned_by':'','updated_by_ins_mngr_slf':'Me','created_by_ins_mngr_slf':'Me','is_last_rec':'last','BaseType':'TP','MasterMandatory':'0','TpTitle':'test_lnr_crs_tp','mro':'null','regDate':'Aug 10, 2016','compDate':'Aug 10, 2016','updateDate':'Aug 10, 2016','is_exempted':'','exempted_by':'','exempted_on':'','Object_type':'cre_sys_obt_crt','Overall_status':'lrn_tpm_ovr_enr'}' class='actionLink' href='javascript:void(0)'>Share</a></li><li class='action-enable'><a onclick='$(&quot;#learningplan-tab-inner&quot;).data(&quot;learningplan&quot;).dropTrainingPrg(this);' name='Cancel' data='data={'RegId':'8','MasterEnrollId':'8','PrgId':'154','Desc':'test_lnr_crs_tp','Recertify':'2','FullName':'gayathri%20k','MasMandatory':'0','managerid':'30','UpdatedBy':'30','UpdatedByName':'gayathri%20k','overall_status':'lrn_tpm_ovr_enr','mro_type':'','assigned_by':'','updated_by_ins_mngr_slf':'Me','created_by_ins_mngr_slf':'Me','is_last_rec':'last','BaseType':'TP','MasterMandatory':'0','TpTitle':'test_lnr_crs_tp','mro':'null','regDate':'Aug 10, 2016','compDate':'Aug 10, 2016','updateDate':'Aug 10, 2016','is_exempted':'','exempted_by':'','exempted_on':'','Object_type':'cre_sys_obt_crt','Overall_status':'lrn_tpm_ovr_enr'}' class='actionLink' href='javascript:void(0);'>Cancel</a></li></ul></div></div></td></tr>";
			//Append result
			$("#Module-detail-section-"+prg_id).html(rhtml);
			$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');					
		
			
		}catch(e){
			// to do
		}
	},
	
	paintDetailsPrgSection : function(data){
		try{
		var masterEnrollId  = data.MasterEnrollId;
		var prgId = data.PrgId;
		var recertifyPath = data.Recertify;
		this.createLoader('enroll-lp-result-container');
		var url = this.constructUrl("ajax/lp-course/" + masterEnrollId + "/" + prgId + "/" + recertifyPath);
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				obj.destroyLoader('enroll-lp-result-container');
				obj.paintCourseDetails(result,data);
			}
	    });
	    $('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
		$('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
		}catch(e){
			// to do
		}
	},
	enrollLPSortForm : function(sort,className) {
		// High light sortype
		try{
		$('#enroll-lp-result-container .sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-lp-result-container .'+className).addClass('sortype-high-lighter');

		var getEnrData 		= eval($("#sort-by-lp-enroll").attr("data"));
		var currEnrMode 	= getEnrData.currEnrMode;
		var userId 			= this.lpUserId;
    	var getRegStatus 	= '';
    	var searchStr 		= '';
    	searchStr			= '&UserID='+userId+'&regstatuschk='+currEnrMode+'&sortBy='+sort;
    	var url 			= this.constructUrl('learning/learningplan-search/all/'+searchStr);
		$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
    	$('#paintEnrollmentLPResults').setGridParam({url: url});
        $("#paintEnrollmentLPResults").trigger("reloadGrid",[{page:1}]);
		}catch(e){
			// to do
		}
	},
	showMoreLPAction : function(obj) {
		try{
		var position = $(obj).position();
		var posLeft  = Math.round(position.left);
		var posTop   = Math.round(position.top);
		$('.enroll-drop-down-list').toggle();
		$('.enroll-action').hide();
		if(this.currTheme == "expertusoneV2"){
			$(obj).parent().siblings('.enroll-action').css('position','');
			$(obj).parent().siblings('.enroll-action').css('margin-top',"19px");
			$(obj).parent().siblings('.enroll-action').slideToggle();
			if(Drupal.settings.mylearning_right===false){
				$(obj).parent().siblings('.enroll-action').closest('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "z-index":"1000"
				    });
				$(obj).parent().siblings('.enroll-action').closest('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "z-index":"1000"
				    });
				$(obj).parent().siblings('.progress').css({
				      "z-index":"-2"
				    });
				$(obj).parent().siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "z-index":"0"
				    });
				$(obj).parent().siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "z-index":"0"
				    });
			}else{
				$(obj).parent().siblings('.enroll-action').closest('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "position":"absolute","margin-right":"0","z-index":"1000"
				    });
				$(obj).parent().siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "position":"absolute","margin-right":"0","z-index":"1000"
				    });
				$(obj).parent().siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "position":"relative","margin-right":"6px","z-index":"0"
				    });
				$(obj).parent().siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
				      "position":"relative","margin-right":"6px","z-index":"0"
				    });
			}
			}else{
			$(obj).parent().siblings('.enroll-action').css('position','absolute');
			/* $(obj).siblings('.enroll-action').css('left',posLeft-32); */
			$(obj).parent().siblings('.enroll-action').css('right','16px');
			$(obj).parent().siblings('.enroll-action').css('top',posTop+20);
			$(obj).parent().siblings('.enroll-action').slideToggle("");
			this.prevMoreLPObj = obj;
			$(obj).parent().siblings('.iframe-mylearning #innerWidgetTagEnroll .enroll-action').css('right','321px'); // salesforce
         }
		}catch(e){
			// to do
		}
			},
	hideMoreLPAction : function(obj) {
		try{
			this.prevMoreLPObj = '';
		}catch(e){
			// to do
		}
	},
	paintCourseDetails : function(result,data){
		try{
		var masterEnrollId  = data.MasterEnrollId;
		var prgId = data.PrgId;
		var shortDescription = unescape(data.Desc);
		var res_length = result.length;
		var rhtml = "";
		var c;
		var courseId;
		var courseCode;
		var compStatus;
		var courseTitle;
		var courseAddEven;
		var isRequired;
		var obj = $("#learningplan-tab-inner").data("learningplan");
		var objStr = '$("#learningplan-tab-inner").data("learningplan")';
		if(res_length > 0 ) {
			if(shortDescription != null && shortDescription != 'null') {
				rhtml += '<div class="learningplan-desc-div">';
				if((shortDescription.length)>250) {
					var shortDescTxt = this.spiltString(shortDescription, 250);
					rhtml += '<div class="recordDiv FindTrngTxt"><span id="LPShortDesc_'+prgId+'">'+shortDescTxt+'</span></div>';
				}else {
					rhtml += '<div class="recordDiv FindTrngTxt"><span>'+shortDescription+'</span></div>';
				}
				rhtml += '</div>';
			}
			rhtml += "<div class='course-level'>";
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse' cellspacing='0'>";
			rhtml += '<tr>';
			rhtml += '<th class="enrollment-course-name">Course Name</th>';
			rhtml += '<th class="enrollment-course-code">Code</th>';
			rhtml += '<th class="enrollment-course-type">Mandatory/Optional</th>';
			rhtml += '<th class="enrollment-course-status">Status</th>';
			rhtml += '</tr>';
			for(c=0; c < result.length ; c++){
				courseId 	= result[c]['crs_id'];
				courseCode 	= result[c]['crs_code'];
				compStatusCode 	= result[c]['comp_status'];
				compStatus 	= result[c]['complition_status'];
				courseTitle = result[c]['crs_title'];
				isRequired  = result[c]['is_required'];
				courseAddEven = (c % 2 == 0)? 'odd' : 'even';
				var param="data={'MasterEnrollId':'"+masterEnrollId+"','PrgId':'"+prgId+"','CourseId':'"+courseId+"'}";
	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-crs-'+prgId+'-course-'+courseId+'">';
				rhtml += '<td>';
				rhtml += '<a id="lp-crs-accodion-'+prgId+'-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" class="title_close" onclick=\''+objStr+'.addAccordionToTableCourseView(this.className,"0 0","0 -61px","dt-child-row-En",'+objStr+'.getLPCourseViewDetails,this,'+objStr+',true);\'>&nbsp;</a>';
				rhtml += '<span class="item-title" ><span class="enroll-lp-class-title vtip" title="'+unescape(courseTitle)+'" href="javascript:void(0);">';
				//rhtml += titleRestricted(courseTitle,27);
				rhtml += titleRestrictionFadeoutImage(courseTitle,'exp-sp-lnrmyprogram-course-title');
				rhtml += '</span></span>';
				rhtml += '</td>';
				rhtml += '<td class="enroll-lp-class-title vtip" title="'+htmlEntities(courseCode)+'">'+titleRestrictionFadeoutImage(courseCode,'exp-sp-lnrmyprogram-course-code')+'</td>';
				rhtml += '<td class="enroll-lp-class-title">'+((isRequired =='Y')?'Mandatory':'Optional')+'</td>';
				rhtml += '<td class="enroll-lp-class-title">'+compStatus +'</td>';
				rhtml += '</tr>';
		  }
		  rhtml += '</table>';
		  rhtml += '</div>';
		}
		else{
			rhtml += "<div>No Events</div>";
		}
		$("#course-detail-section-"+masterEnrollId).html(rhtml);
		}catch(e){
			// to do
		}
	},
	addAccordionToTableCourseView : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var currTrStr = $(obj).parent().parent().attr("id");
		currTrStr = currTrStr.split('-');
		var currTr = currTrStr[2]+'-course-'+currTrStr[4];
		if(!document.getElementById(currTr+"CourseSubGrid")) {
			$("#lp-crs-"+currTr).after("<tr id='"+currTr+"CourseSubGrid'><td colspan='4'><div id='enroll-"+currTr+"' class='class-level'></div></td></tr>");
			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
			$("#lp-crs-accodion-"+currTr).removeClass("title_close");
			$("#lp-crs-accodion-"+currTr).addClass("title_open");
			$("#lp-crs-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"CourseSubGrid").slideDown(1000);
		} else {
			var clickStyle = $("#"+currTr+"CourseSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
    			$("#"+currTr+"CourseSubGrid").slideDown(1000);
    			$("#lp-crs-accodion-"+currTr).removeClass("title_close");
				$("#lp-crs-accodion-"+currTr).addClass("title_open");
				$("#lp-crs-"+currTr).removeClass("ui-widget-content");
    		} else {
    			$("#"+currTr+"CourseSubGrid").hide();//css("display","none");
    			$("#"+currTr+"CourseSubGrid").slideUp(1000);
    			$("#lp-crs-accodion-"+currTr).removeClass("title_open");
				$("#lp-crs-accodion-"+currTr).addClass("title_close");
				$("#lp-crs-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-"+currTr).addClass("ui-widget-content");
    		}
		}


		var data = eval($("#lp-crs-accodion-"+currTr).attr("data"));
		var classDetSec = this.getLPClassDetail(data);
		}catch(e){
			// to do
		}
	},
	getLPClassDetail : function(data){
		try{
		var masterEnrollId  = data.MasterEnrollId;
		var prgId = data.PrgId;
		var recertifyPath = data.Recertify;
		var object_type = data.Object_type;
		var overall_status = data.Overall_status;
 		
		if($('#course-detail-section-'+masterEnrollId).html() == ''){
			//this.createLoader('enroll-lp-result-container');
			var url = this.constructUrl("ajax/lp-class/" + masterEnrollId + "/" + prgId + "/" + recertifyPath + "/" +object_type + "/" + overall_status);
			var obj = this;
			
			//$('#paindLPContentResults_prg_'+masterEnrollId).css('display', 'block');
			
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.paintLPClassDetails(result,data);
					if(document.getElementById('tab_my_learningplan') !== undefined) {
						if($("#block-take-survey" ).data('surveylearner') === undefined) {
							$('<div id="block-take-survey"></div>').insertAfter("#learningplan-tab-inner");
							Drupal.attachBehaviors('block-take-survey');
							$("#block-take-survey").surveylearner();
						}
						if(document.getElementById('learner-enrollment-tab-inner') === undefined || $('#learner-enrollment-tab-inner').data('contentLaunch') === undefined) {
							$('<div id="learner-enrollment-tab-inner"></div>').insertAfter("#learningplan-tab-inner");
							$("#learner-enrollment-tab-inner").enrollment();
							$("#learner-enrollment-tab-inner" ).contentLaunch();
						}
						$('#lp_seemore_prg_'+masterEnrollId).css('display','none');
						$('#lp_seeless_prg_'+masterEnrollId).css('display','block');
						obj.destroyLoader('enroll-lp-result-container');
						$('.lp-class-title-details .limit-title').trunk8(trunk8.myprogramClass_title);
						$('.lp-class-title-details .limit-desc').trunk8(trunk8.myprogramClass_desc);
					}
				}
			
		    });
		}
		
		$('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
		//72790: More and description V should expand all the details instead of only doing what we do today.
		//$('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
		}catch(e){
			// to do
		}
	},
	paintLPClassDetails : function(result,data){
		try{
		var res_length = result.length;
		var masterEnrollId  = data.MasterEnrollId;
		var prgId = data.PrgId;
		var shortDescription = unescape(data.Desc);
		var created_by_ins_mngr_slf = unescape(data.created_by_ins_mngr_slf);
		var updated_by_ins_mngr_slf = unescape(data.updated_by_ins_mngr_slf);
		var fullName = unescape(data.FullName);
		var updatedByName = unescape(data.UpdatedByName);
		var overallStatus = data.overall_status;
		var updatedBy = data.UpdatedBy;
		var managerId = data.managerid;
		var regsteredDate  = data.regDate;
		var completedDate  = data.compDate;
		var expired_on = data.expired_on;
		var html = "";
		var ostr = '';
		var c;
		var obj = $("#learningplan-tab-inner").data("learningplan");
		var objStr = '$("#learningplan-tab-inner").data("learningplan")';
		var userId = this.lpUserId;
		if(res_length > 0 ) {
			var isAssigned = 0;
			var isMro = 0;
			if(data.mro_type !=null && data.mro_type !='null' && data.mro_type !='' && data.mro_type !='Mandatory' && data.mro_type !='Recommended' && data.mro_type !='Optional') {
				html += "<div class='item-assigned-code'>";
				var isAssigned = 0;
				var MROValue = (data.mro_type!=null  ? (data.mro_type) : '');
						if(MROValue) {
							var displaymro='';
						}else{
							var displaymro='';
							}
					if(data.assigned_by !='' && data.assigned_by!= null  && userId != data.managerid ){
						html += "<span title='"+Drupal.t('LBL133')+"'>";
						html += displaymro+ Drupal.t("LBL736") +" "+Drupal.t('by')+ " : " + "<span class='enr-status-cls'>" + data.assigned_by + '</span>';
						html += "</span>";
						isAssigned = 1;
					}
					if(data.overall_status =='lrn_tpm_ovr_cln'){
						if(isAssigned==1){
							html += "<span class='enroll-pipeline'>|</span>";
						}
						html += "<span title='"+updated_by_ins_mngr_slf+"'>";
						html += Drupal.t('LBL680')+" : " + "<span class='enr-status-cls'>" + ((userId != data.UpdatedBy) ? updatedByName : updatedByName) + '</span>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
						html += "</span>";
					}
					if(data.overall_status =='lrn_tpm_ovr_cmp'){
						if(isAssigned==1){
							html += "<span class='enroll-pipeline'>|</span>";
						}
						html += "<span title='"+updated_by_ins_mngr_slf+"'>";
						html += Drupal.t('LBL681')+" : " +  "<span class='enr-status-cls'>" +  ((userId != data.UpdatedBy) ? updatedByName : updatedByName) + '</span>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
						html += "</span>";
					}
				html += "</div>";
			}
			html += '</div>';
			//51563: When displaying the classes under a training plan, the group name of the class is not displayed
			var thClass = '';
			var module_code_arr = new Array();
			var group_name_arr = new Array();
			
			var group_name_count = [];
			for(k=0; k < res_length; k++){
				if(module_code_arr.length<=0){
					thClass = 'clsEmptyTh'
					module_code_arr[result[k]['module_code']]= result[k]['module_name'];
				}
				
				
				if(group_name_arr.length<=0){
					thClass = 'clsEmptyTh';
					if(result[k]['groupname'] in group_name_arr) {
						group_name_count[result[k]['groupname']] = group_name_count[result[k]['groupname']] + 1;
					} else {
						group_name_count[result[k]['groupname']] = 1;
					}
					group_name_arr[result[k]['groupname']] = result[k]['groupname'];
				}
				
			}
			html += '<div class="see-more-tp-details">';
			html += '<div class="course-level">';
			html += '<table class="enroll-show-moreclass" width="100%" border="0" cellpadding="0" cellspacing="0">';
			html += '<tr>';
			html += '<th colspan="3" class="'+thClass+'"></th>';
			html += '</tr>';
			/*var module_name = titleRestrictionFadeoutImage(result[0]['module_name'],'exp-sp-lnrmyprogram-grp-name',90);				
			  html += '<tr class="clsGrpTr"><td colspan="4"><div class="clsGrpModuleName"><span class="vtip" title="'+result[0]['module_name']+'">'+module_name+'</span></div><div><hr></div></td></tr>'; */
			var g = 1;
			var group_name_flag = false;
			for (var key in group_name_arr){ 
			var group_name = titleRestrictionFadeoutImage(group_name_arr[key],'exp-sp-lnrmyprogram-grp-name',90);				
			if(group_name!='') {
				group_name_flag = true;
				html += '<tr class="clsGrpTr"><td colspan="3"><div class="clsGrpModuleName"><span class="vtip" title="'+group_name_arr[key]+'">'+group_name+'</span></div><div><hr></div></td></tr>';
			}
			  //console.log(result.toSource());
			  for(c=0; c < result.length ; c++){
			  //console.log('grp name ' + result[c]['groupname'])			 
			 		if(key == result[c]['groupname'] || (result[c]['groupname'] == 'null' || result[c]['groupname'] == null || result[c]['groupname'] == '')){
			 			group_name_count[key] = group_name_count[key] - 1;
			 			
						this.currTheme = Drupal.settings.ajaxPageState.theme;
						var baseType				= result[c]['basetype'];
						var dateValidTo				= result[c]['valid_to'];
						var dataDelType				= result[c]['delivery_type'];
						var dataDelTypeCode			= result[c]['delivery_type_code'];
						var dataDelTypeImage		= result[c]['delivery_type_image'];
						var dataRegStatus			= result[c]['reg_status'];
						var dataRegStatusCode		= result[c]['reg_status_code'];
						var RegStatusCode			= result[c]['reg_status_code'];
						var courseComplitionStatus	= result[c]['reg_status'];
						var courseScore				= result[c]['score'];
						var courseGrade				= result[c]['grade'];
						var dataId 					= result[c]['id'];
						var courseId 				= result[c]['course_id'];
						var classId 				= result[c]['class_id'];
						var usrId					= result[c]['user_id'];
						var classTitle				= result[c]['cls_title'];
						var classCode				= result[c]['cls_code'];
						var courseTitle				= result[c]['course_title'];
						var courseCode				= result[c]['course_code'];
						var courseTempId			= result[c]['courseid'];
						var session_id				= result[c]['session_id'];
						var SessStartDate 			= result[c]['session_start'];
						var sessionStartDay			= result[c]['session_start_day'];
						var SessEndDate 			= result[c]['session_end'];
						var shortDescription 		= result[c]['description'];
						var language 				= result[c]['language'];
						var usertimezonecode 		= result[c]['usertimezonecode'];
						
						var LocationName 			= result[c]['location_name'];
						var LocationId				= result[c]['location_id'];
						var LocationAddr1 			= result[c]['location_addr1'];
						var LocationAddr2 			= result[c]['location_addr2'];
						var LocationCity 			= result[c]['location_city'];
						var LocationState 			= result[c]['location_state'];
						var LocationCountry 		= result[c]['location_country'];
						var LocationZip 			= result[c]['location_zip'];
						var LocationPhone 			= result[c]['location_phone'];
						var regDate					= result[c]['reg_date'];
						var canceledDate            = result[c]['reg_status_date'];
						var compDate				= result[c]['comp_date'];
						var updateDate				= result[c]['updated_on'];
						var startDateFormat			= result[c]['session_start_format'];
						var lnrAttach				= result[c]['show_lnr_attach'];
						var show_events				= result[c]['show_events'];
						var LessonLocation			= result[c]['LessonLocation'];
						var launch_data				= result[c]['LaunchData'];
						var suspend_data			= result[c]['SuspendData'];
						var exit					= result[c]['CmiExit'];
						var isRequired				= result[c]['isRequired'];
						var moduleTitle				= result[c]['module_name'];
						var moduleCode				= result[c]['module_code'];
						var courseQuizStatus		= result[c]['course_quiz_status'];
						var daysRemaining = '';
						var dayRemainVal  = '';
						var AttemptLeft	  = '';
						var ValidityDays  = '';
						var contValidateMsg = '';
						var classScore = '';
						var daysLeft = '';
						var percentCompleted = '';	// will be used only for VOD contents
						var VersionNo = '';

						var sequenceNo  = result[c]['seqNo'];
						var waitlistPriority     =  result[c]['waitlist_priority'];
						var labelMsg = result[c]['labelmsg'];
						var hideShare = result[c]['hideShare'];
						if(this.currTheme == "expertusoneV2"){
							/*if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["es", "fr", "ru"]) != -1) {
								html += '<tr class="ui-widget-content set-height" id="lp-cl-'+result[c]['id']+'"><td style="width:80%" id="enr-moreclass-columnone" class="lp-class-title-details">';
							}else{
								html += '<tr class="ui-widget-content set-height" id="lp-cl-'+result[c]['id']+'"><td style="width:87%" id="enr-moreclass-columnone" class="lp-class-title-details">';
							}*/
							html += '<tr class="ui-widget-content set-height '+baseType+'" id="lp-cl-'+result[c]['id']+'">';
							html += '<td class="lp-class-delivery-type"><div class="delivery-type-icon ' + dataDelTypeImage + ' vtip" title="' + dataDelType + '" style="border-right: 0px none;"></div></td>';	// dellivery type icon
							html += '<td id="enr-moreclass-columnone" class="lp-class-title-details">';
						}else{
							html += '<tr class="set-height '+baseType+'" id="lp-cl-'+result[c]['id']+'"><td style="width:529px;">';
						}
						if(session_id == null) {
							session_id ='';
						}
						if((baseType=="WBT" || baseType == "VOD") && ((dataRegStatusCode=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") || (dataRegStatusCode=="lrn_crs_cmp_inp") || (dataRegStatusCode=="lrn_crs_cmp_inc"))) {
							  var i=0;
								var allCntId = [];
								for(j=0; j<result[c].launch.length; j++){
								  allCntId[j] = result[c].launch[j].ContentId;
								}
								var uniqueCntId = $.unique(allCntId);
								if(uniqueCntId.length == 1 && result[c].launch[0].ValidityDays !=''){
								  var diffDays = result[c].launch[0].remDays;
//									var remValidityDays = result[c].launch[0].ValidityDays - diffDays;
									var remValidityDays = (result[c].launch[0].daysLeft !== undefined && result[c].launch[0].daysLeft != null) ? result[c].launch[0].daysLeft : '';
									if(remValidityDays <= 0){
										var daysLBL = Drupal.t("Expired");//Expired
										remValidityDays = '';// To avoid result as Validity: 0 Expired
									}else if(remValidityDays == 1){
										var daysLBL = Drupal.t("LBL910");//day
									}else if(remValidityDays > 1){
										var daysLBL = Drupal.t("LBL605");//days
									}
									ValidityDays = Drupal.t("LBL604")+' : '+remValidityDays+' '+daysLBL;
								}
								if(uniqueCntId.length == 1 && result[c].launch[0].AttemptLeft !='' && result[c].launch[0].AttemptLeft !='notset'){
									contValidateMsg			= result[c].launch[0].contValidateMsg;
									AttemptLeft	= Drupal.t("LBL202")+' : '+result[c].launch[0].AttemptLeft;
								}
								if(uniqueCntId.length == 1 && result[c].launch[0].VersionNum !='' && result[c].launch[0].VersionNum !='notset'){
									VersionNo	= Drupal.t("LBL1123")+' : '+result[c].launch[0].VersionNum;
								}
								if (courseScore && courseScore != '0.00' && dataRegStatusCode=="lrn_crs_cmp_cmp") {
								    classScore = Drupal.t("LBL668") +' : ' + courseScore;
								  }
								var courseQuizStatusString = '';
								if(courseQuizStatus && courseQuizStatus != '' && dataRegStatusCode == "lrn_crs_cmp_cmp") {
									courseQuizStatusString = Drupal.t("LBL1284") +' : ' + Drupal.t(courseQuizStatus);
								}
							  	if(dateValidTo!='' && dateValidTo!=null){
								  var daystocount = new Date(dateValidTo);
								  var srvDate = result[c].launch[0].server_date_time;
								  today=new Date(srvDate);
								  var oneday=1000*60*60*24;
								  daysRemaining = (Math.ceil((daystocount.getTime()-today.getTime())/(oneday)));
								  if(daysRemaining<0){
									  dayRemainVal  = Drupal.t("Expired");
								  }else {
									  dayRemainVal = Drupal.t("LBL677")+': '+(daysRemaining);
								  }
							  	}
							  	daysLeft = (result[c].launch.length > 0 && result[c].launch[0].daysLeft != undefined && result[c].launch[0].daysLeft != '' ) ? result[c].launch[0].daysLeft : '';
							  	//daysLeft = result[c].launch[0].daysLeft;
								var progressPercent = 0;
								if(baseType == 'VOD') {
									//if(result[c].launch.length > 0){
									if(courseComplitionStatus == "Completed"){
										percentCompleted = '100% '+Drupal.t('Completed');
										progressPercent = 100;
									}else{
										for(j=0; j < result[c].launch.length; j++){
											var suspendData = (result[c].launch[j].SuspendData != undefined && result[c].launch[j].SuspendData != null && result[c].launch[j].SuspendData != '') ? JSON.parse(unescape(result[c].launch[j].SuspendData)) : null;
											//progressPercent = progressPercent + parseFloat((suspendData != null ? suspendData['progress'] : 0));
											progressPercent = progressPercent + (suspendData != null && !isNaN(parseFloat(suspendData['progress'])) ? parseFloat(suspendData['progress']) : 0);
										}
										percentCompleted = Math.round(progressPercent/result[c].launch.length) + '% '+Drupal.t('Completed');
									}
									//}else{
									//	percentCompleted = '0% '+Drupal.t('Completed');
									//}
									
								}
						}
						var LocationAdd1Arg = ((LocationAddr1!= null) ? ((unescape(LocationAddr1).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr1);
						var LocationAdd2Arg = ((LocationAddr2 != null) ? ((unescape(LocationAddr2).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr2);
						var LocationcityArg = ((LocationCity != null) ? ((unescape(LocationCity).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationCity);
						var data1="data={'RegId':'"+dataId+"','classCode':'"+escape(classCode)+"','BaseType':'"+baseType+"','shortDescription':'"+escape(shortDescription)+"','isTitle':'1','title':'"+escape(classTitle)+"','rowId':'"+dataId+"','CourseId':'"+courseId+"'," +
								"'show_events':'"+show_events+"','coursetempid':'"+courseTempId+"','regstatus':'"+dataRegStatusCode+"','regStatusCode':'"+RegStatusCode+"','classId':'"+classId+"','deliverytype':'"+dataDelTypeCode+"','daysRemaining':'"+daysRemaining+"'," +
								"'dataDelType':'"+dataDelType+"','SessStartDay':'"+sessionStartDay+"','startDateFormat':'"+startDateFormat+"','SessStartDate':'"+SessStartDate+"','SessEndDate':'"+SessEndDate+"','courseComplitionStatus':'"+courseComplitionStatus+"','courseScore':'"+courseScore+"','courseGrade':'"+courseGrade+"','detailTab':'false'," +
								"'regDate':'"+regDate+"','compDate':'"+compDate+"','updateDate':'"+updateDate+"','courseTitle':'"+escape(courseTitle)+"','courseCode':'"+escape(courseCode)+"'," +
								"'contValidateMsg':'"+AttemptLeft+"','dayRemainVal':'"+dayRemainVal+"','daysLeft':'"+daysLeft+"',"+"'LessonLocation':'"+LessonLocation+"','moduleCode':'"+escape(moduleCode)+"',"+
								"'launch_data':'"+launch_data+"','suspend_data':'"+escape(suspend_data)+"','exit':'"+exit+"',"+
								"'usertimezonecode' : '"+usertimezonecode+"',"+
								"'LocationName':'"+escape(LocationName)+"','LocationId':'"+LocationId+"','LocationAddr1':'"+LocationAdd1Arg+"','LocationAddr2':'"+LocationAdd2Arg+"','LocationCity':'"+LocationcityArg+"','LocationState':'"+LocationState+"','LocationCountry':'"+escape(LocationCountry)+"','LocationZip':'"+LocationZip+"','LocationPhone':'"+LocationPhone+"'}";
						var inc = 0;
						var passParams ='';
						if(lnrAttach.length>0) {
							 $(lnrAttach).each(function(){
								 inc=inc+1;
								 passParams += "{";
								 passParams += "'Id':'"+$(this).attr("attachment_id")+"'";
								 /*--Issue fix for the ticket - 32781 --*/
								 passParams += ",'Title':'"+escape($(this).attr("reading_title")).replace(/"/g, '&quot;')+"'";
								 passParams += ",'url':'"+encodeURI($(this).attr("reading_content")).replace(/'/g, '%27')+"'";
								 passParams += "}";
								 if(inc<lnrAttach.length) {
									 passParams += ",";
								 }
							 });
						}
						var attachdata = "data=["+passParams+"]";
						var classSwitch = (baseType=="WBT" || baseType=="VOD") ? 'item-title-wbt' : 'item-title';
						//Enrollment Title Restricted to 20 Character
						var enrollFulltitle = htmlEntities(classTitle);
		    classTitle1 = classTitle.replace(/\\/g, '');
						html += '<div class="enroll-lp-course-title">';
						html += '<span id="class_attachment_lp_'+dataId+'" data="'+attachdata+'"></span>';
						if(this.currTheme != 'expertusoneV2') {
							html += '<a id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+objStr+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objStr+'.getLPClassViewDetails,this,'+objStr+',true);\'>&nbsp;</a>';
						}
			html +=  '<div class="limit-title-row">';			
			html += '<span id="titleLPAccEn_'+dataId+'" class="'+classSwitch+'" ><span class="limit-title limit-title-cls enroll-lp-class-title vtip" title="'+htmlEntities(classTitle1)+'" href="javascript:void(0);">';
						//if(baseType=="WBT" || baseType=="VOD"){
						//	if(Drupal.settings.mylearning_right===false)
								html += enrollFulltitle.replace(/\\/g, '');
						//	else
						//		html += titleRestrictionFadeoutImage(enrollFulltitle.replace(/\\/g, ''),'exp-sp-lnrmyprogram-enroll-fulltitle',50);//+"--"+sequenceNo;
						//}
						//else{
						//	if(Drupal.settings.mylearning_right===false)
						//		html += titleRestrictionFadeoutImage(enrollFulltitle.replace(/\\/g, ''),'exp-sp-lnrmyprogram-enroll-fulltitle',55);
						//	else
						//		html += titleRestrictionFadeoutImage(enrollFulltitle.replace(/\\/g, ''),'exp-sp-lnrmyprogram-enroll-fulltitle',20);
						//}
						html += '</span></span>';
						html += '</div>';
						html += '</div>';
						var startTime = result[c]['session_start'];
						var startDate = result[c]['session_start_format'];
						var baseType  = result[c]['basetype'];
						var classId	  = result[c]['class_id'];
						if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
							var sessLen = result[c].sessionDetails.length;
							var session_code = result[c].sessionDetails[0].session_code;
							var sessLenEnd;
							var user_tzcode = result[c].sessionDetails[0].user_tzcode;
							var tz_code = result[c].sessionDetails[0].tz_code;
							//alert(6);
						//alert('dis-->>'+result[c].sessionDetails[0].tz_code);
          //alert(user_tzcode);
							if(sessLen>1) {
								sessLenEnd = sessLen-1;
								/*For ticket 0028936: ILT Timezone Display for My Learning page */
								var sTime = result[c].sessionDetails[0].session_start_format;
								var sTimeForm = result[c].sessionDetails[0].session_start_time_form;
								var eTime = result[c].sessionDetails[sessLenEnd].session_end_format;
								var eTimeForm = result[c].sessionDetails[sessLenEnd].session_end_time_form;
								startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
								if(baseType =="ILT"){
								startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
								}
								var usTime = result[c].sessionDetails[0].ilt_session_start_format;
								var usTimeForm = result[c].sessionDetails[0].ilt_session_start_time_form;
								var ueTime = result[c].sessionDetails[sessLenEnd].ilt_session_end_format;
								var ueTimeForm = result[c].sessionDetails[sessLenEnd].ilt_session_end_time_form;
								SessionstartDate = usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+' to '+ueTime+' <span class="time-zone-text">'+ueTimeForm+'</span>'+' '+session_code+' '+tz_code;
								
							} else {
								/*For ticket 0028936: ILT Timezone Display for My Learning page */
								var sTime = result[c].sessionDetails[0].session_start_format;
								var sTimeForm = result[c].sessionDetails[0].session_start_time_form;
								var eTime = result[c].sessionDetails[0].session_start_end_format;
								var eTimeForm = result[c].sessionDetails[0].session_end_time_form;
								startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
								if(baseType =="ILT"){
								startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
							}
							    var usTime = result[c].sessionDetails[0].ilt_session_start_format ;
							    var usTimeForm = result[c].sessionDetails[0].ilt_session_start_time_form;
							    var ueTime = result[c].sessionDetails[0].ilt_session_start_end_format;
							    var ueTimeForm = result[c].sessionDetails[0].ilt_session_end_time_form;
							 SessionstartDate = usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+' to '+ueTime+' <span class="time-zone-text">'+ueTimeForm+'</span>'+' '+session_code+' '+tz_code;
							//console.log(SessionstartDate);
							}
							var inc = 0;
							var passParams = "[";

							$(result[c].sessionDetails).each(function(){
								inc=inc+1;
								var sessionfacAddress1 = (($(this).attr("session_address1") != null) ? ((unescape($(this).attr('session_address1')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_address1"));
								var sessionfacAddress2 = (($(this).attr("session_address2") != null) ? ((unescape($(this).attr('session_address2')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_address2"));
								var sessionfacCity = (($(this).attr("session_city") != null) ? ((unescape($(this).attr('session_city')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_city"));
								var sInsName = (($(this).attr("session_instructor_name") ===null) ? '':((unescape($(this).attr('session_instructor_name')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")));
								 passParams += "{";
								 passParams += "'sessionTitle':'"+(($(this).attr("session_title")) ? escape($(this).attr("session_title")) : '')+"',";

								 /*For ticket 0028936: ILT Timezone Display for My Learning page */
								 passParams += "'sessionDay':'"+$(this).attr("session_start_day")+"',";
								 passParams += "'sessionDate':'"+$(this).attr("session_start_format")+' to '+$(this).attr("session_start_end_format")+"',";
								  passParams += "'sessionSDate':'"+$(this).attr("session_start_format")+"',";
								 passParams += "'sessionEDate':'"+$(this).attr("session_start_end_format")+"',";
								 passParams += "'sessionSDayForm':'"+$(this).attr("session_start_time_form")+"',";
								 passParams += "'sessionEDayForm':'"+$(this).attr("session_end_time_form")+"',";
								 passParams += "'sInsName':'"+sInsName+"',";
                             // if(baseType =="ILT"){
                                 passParams += "'sessionDayloc':'"+$(this).attr("ilt_session_start_day")+"',"  ;
                                 passParams +=  "'sessionDateloc':'"+$(this).attr("ilt_session_start_format")+' to '+$(this).attr("ilt_session_start_end_format")+"'," ;
                                 passParams +=  "'sessionSDateloc':'"+$(this).attr("ilt_session_start_format")+"',"  ;
                                 passParams +=  "'sessionEDateloc':'"+$(this).attr("ilt_session_start_end_format")+"'," ;
                                 passParams += "'sessionSDayFormloc':'"+$(this).attr("ilt_session_start_time_form")+"'," ;
                                 passParams +=  "'sessionEDayFormloc':'"+$(this).attr("ilt_session_end_time_form")+"'," ;
                                 passParams +=  "'sessionId':'"+$(this).attr("session_id")+"'," ;
                                 passParams +=  "'sessiontimezone':'"+$(this).attr("sess_timezone")+"'," ;
                                 passParams +=  "'usertimezone':'"+$(this).attr("user_timezone")+"'," ;
                                 passParams += "'usertimezonecode':'"+usertimezonecode+"'," ;
                                 passParams +=  "'session_code':'"+$(this).attr("session_code")+"'," ;
                                 passParams +=  "'user_tzcode':'"+$(this).attr("user_tzcode")+"'," ;
                                 passParams +=  "'tz_code':'"+$(this).attr("tz_code")+"'," ;
                                // }

								 passParams += "'sessionfacAddress1':'"+sessionfacAddress1+"',";
								 passParams += "'sessionfacAddress2':'"+sessionfacAddress2+"',";
								 passParams += "'sessionfacCountry':'"+escape($(this).attr("session_country"))+"',";
								 passParams += "'sessionfacState':'"+escape($(this).attr("session_state"))+"',";
								 passParams += "'sessionfacCity':'"+escape(sessionfacCity)+"',";
								 passParams += "'sessionfacZipcode':'"+$(this).attr("session_zipcode")+"'";
								 passParams += "}";
								 if(inc < sessLen) {
									 passParams += ",";
								 }
							});
							passParams += "]";
						//	html += '<div id="LPtz"><span class="session-time-zone" data=\"data='+passParams+'\" id="session_det_popup_lp'+classId+'" onmouseover=\'$("#learningplan-tab-inner").data("learningplan").getLPSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDate,'exp-sp-lnrenrollment-timezone', 30)+'</span>';
						}
						//alert(baseType);
        /*   if(baseType =="ILT" && result[c].sessionDetails[0].sess_timezone!=result[c].sessionDetails[0].user_timezone){
           var qtipId = dataId+"-tp-"+classId;
           html += qtip_popup_paint(qtipId,SessionstartDate,sessLen); 
		html += '</div>';	
						}*/
						html += '<div class="enroll-class-title-info ">';
						html += '<div class="item-title-code">';
						//Pipe Sysmbol use this variable 'pipe'
						var pipe = '<span class="enroll-pipeline">|</span>';
						var enrollClassInfo = '';
						/*var newdelType;
						if(dataDelType) {
							if(dataDelTypeCode=='lrn_cls_dty_ilt'){
								newdelType = Drupal.t('Classroom');
							}else if(dataDelTypeCode=='lrn_cls_dty_wbt'){
								newdelType = Drupal.t('Web-based');
							}else if(dataDelTypeCode=='lrn_cls_dty_vcl'){
								newdelType = Drupal.t('Virtual Class');
							}else{
								newdelType = Drupal.t('Video');
							}
							enrollClassInfo += newdelType;
						}*/
						var attemLeft_flag = false;
						var validity_flag = false;
						var enrollClassInfoDet= '';
						if(AttemptLeft!=''){
							//html += "<div class = 'line-item-container float-left'>"+Drupal.t('Completed')+" : "+ compCount+"</div>";
                            enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + AttemptLeft,'exp-sp-myprogram-lineitem');
                            
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class= 'vtip' title = '"+AttemptLeft+"'>"+enrollClassInfoDet+"</span></div>";
							attemLeft_flag = true;
						}
						if((ValidityDays!='' && attemLeft_flag==false) || (ValidityDays!='' && Drupal.settings.mylearning_right===false)){
							//enrollClassInfo += (enrollClassInfo!= '' ? pipe : '') + ValidityDays;
							enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + ValidityDays,'exp-sp-myprogram-lineitem');
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class= 'vtip' title = '"+ValidityDays+"'>"+enrollClassInfoDet+"</span></div>";
							attemLeft_flag = true;
							validity_flag = true;
						}
//						console.log('percentCompleted = ', percentCompleted);
						if(baseType == 'VOD' && percentCompleted!='') {
							enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + percentCompleted,'exp-sp-myprogram-lineitem');
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class= 'vtip' title = '"+percentCompleted+"'>"+enrollClassInfoDet+"</span></div>";

						}
//						if(Drupal.settings.mylearning_right===false)
//							enrollClassInfo += (enrollClassInfo!= '' ? pipe : '') + '<span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',50)+'</span>';
//						else
//							enrollClassInfo += (enrollClassInfo!= '' ? pipe : '') + '<span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',10)+'</span>';

						var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
						if(ClassLoc && (RegStatusCode != 'lrn_crs_cmp_cmp') ) {
							if(Drupal.settings.mylearning_right===false){
							 enrollClassInfoDet = (enrollClassInfo!= '' ? pipe : '') + '<span class="vtip" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrmyprogram-class-location',30)+'</span>';
							 enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";
							}else{
							 enrollClassInfoDet = (enrollClassInfo!= '' ? pipe : '') + '<span class="vtip" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrmyprogram-class-location',10)+'</span>';
							 enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";
							}
						}

						if(RegStatusCode == "lrn_crs_reg_wtl" && waitlistPriority != '') {
							enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') +  '<span>' + labelMsg['msg5'] + ' ' + waitlistPriority,'exp-sp-myprogram-lineitem') + '</span>';
							enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";
						}
						isRequired = (isRequired =='Y')?'Mandatory':'Optional';
						enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + Drupal.t(isRequired),'exp-sp-myprogram-lineitem');
						enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+Drupal.t(isRequired)+"'>"+enrollClassInfoDet+"</span></div>";

						enrollClassInfoDet = (courseComplitionStatus == "Completed") ? titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + Drupal.t("Completed"),'exp-sp-myprogram-lineitem') : titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + Drupal.t(courseComplitionStatus),'exp-sp-myprogram-lineitem');
						enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+Drupal.t(courseComplitionStatus)+"'>"+enrollClassInfoDet+"</span></div>";

						if(VersionNo!=''){
							enrollClassInfoDet = titleRestrictionFadeoutImage((enrollClassInfo!= '' ? pipe : '') + VersionNo,'exp-sp-myprogram-lineitem');
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+VersionNo+"'>"+enrollClassInfoDet+"</span></div>";

							}
						if(passParams && (baseType =="ILT" || baseType =="VC")&& (RegStatusCode != 'lrn_crs_cmp_cmp') ){
							
							enrollClassInfoDet = (enrollClassInfo!= '' ? pipe : '') +  '<span data=\"data='+passParams+'\" id="session_det_popup_lp'+classId+'" onmouseover=\'$("#learningplan-tab-inner").data("learningplan").getLPSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDate,'exp-sp-lnrenrollment-timezone')+'</span>';
							enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";

						}

		
						if(RegStatusCode=='lrn_crs_cmp_cmp' || RegStatusCode=='lrn_crs_cmp_enr' || RegStatusCode=='lrn_crs_cmp_inp' ){
							enrollClassInfoDet = "<span id='compLpDetails"+dataId+"' class='compLpDetails'>";
							enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";

							//html += ((AttemptLeft != '') ? (AttemptLeft) : '');
						//	html += ((ValidityDays != '' && AttemptLeft != '') ? (pipe) : '');
						//	html += ((ValidityDays != '' && validity_flag==false) ? (ValidityDays) : '');
						//	html += ((classScore != '' && ( ValidityDays != '' && validity_flag==false)) ? (pipe) : '');
							//html += ((classScore != '') ? (classScore) : '');
						if(dataRegStatusCode == "lrn_crs_cmp_cmp") {
							
							enrollClassInfoDet = titleRestrictionFadeoutImage(((classScore != '') ? (pipe+classScore) : ''),'exp-sp-myprogram-lineitem');
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+classScore+"'>"+enrollClassInfoDet+"</span></div>";
						    enrollClassInfoDet = ((courseQuizStatusString != '' && ( (ValidityDays != '' && validity_flag==false) || classScore != ''  || classScore=='')) ? (pipe) : '');
							enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";
							enrollClassInfoDet = ((courseQuizStatusString != '') ? (titleRestrictionFadeoutImage(courseQuizStatusString,'exp-sp-myprogram-lineitem')) : '');
							enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+courseQuizStatusString+"'>"+enrollClassInfoDet+"</span></div>";
						}
							
						enrollClassInfo += ' </span>';
					}else{
						enrollClassInfoDet =  "<span id='compLpDetails"+dataId+"' class='compLpDetails'>";
						enrollClassInfo += "<div class = 'line-item-container float-left'>"+enrollClassInfoDet+"</div>";
						//enrollClassInfo += ((AttemptLeft != '') ? (pipe+AttemptLeft) : '');
						enrollClassInfoDet =  titleRestrictionFadeoutImage(((ValidityDays != '' && validity_flag==false) ? (ValidityDays) : ''),'exp-sp-myprogram-lineitem');
						enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+ValidityDays+"'>"+enrollClassInfoDet+"</span></div>";
						enrollClassInfoDet =  titleRestrictionFadeoutImage(((classScore != '') ? (pipe+classScore) : ''),'exp-sp-myprogram-lineitem');
						enrollClassInfo += "<div class = 'line-item-container float-left'><span class='vtip' title = '"+classScore+"'>"+enrollClassInfoDet+"</span></div>";
							
						enrollClassInfoDet = ' </span>';
						}
						html += enrollClassInfo; 
							 if(baseType =="ILT" && result[c].sessionDetails[0].sess_timezone!=result[c].sessionDetails[0].user_timezone &&  (RegStatusCode != 'lrn_crs_cmp_cmp')){
					           var qtipId = dataId+"-tp-"+classId;
					           html += qtip_popup_paint(qtipId,SessionstartDate,sessLen); 
				//			html += '</div>';
						 }					
						html += ' </div>';
						html += '</div>';
						if(shortDescription != null) {
							var tptype = baseType;
							html += '<div class="limit-desc-row '+tptype+'"><div class="recordDiv FindTrngTxt"><span class="limit-desc limit-desc-cls vtip" id="classShortDesc_'+dataId+'" class="training-desc"><span class="cls-learner-descriptions">'+shortDescription+'</span></span></div></div>';
						}
						html += '</td>';
						if(Drupal.settings.user.language !== undefined && $.inArray(Drupal.settings.user.language, ["es", "fr", "ru"]) != -1) {
							html += '<td id="enr-moreclass-columntwo" class="class-launch-action" style="width:13%">';
						}else{
							html += '<td id="enr-moreclass-columntwo" class="class-launch-action" style="width:13%">';
						}
						//html += '<td id="enr-moreclass-columntwo">';
						result[c]['progressPercent'] = progressPercent;
						result[c]['overall_status'] = data.overall_status;
						html += this.paintLPClassActions(result[c],prgId,data1);
						html += '</td>';
						html += '</tr>';
					  html += '<tr id="subgrid-class_menu_detail_'+dataId+'" class="subgrid-class_menu_detail" style="display:none">';
					  html += '<td></td><td colspan="2" class="clsTPClassMenu" style="padding-bottom:0;"><div class="paindContentResults clsSeeMorePlaceholderdiv" id="paindContentResults_'+dataId+'"></div></td>';
					  html += '</tr>';
					  var lastRowInGroup = '';
						if(parseInt(Object.keys(group_name_arr).length) !== parseInt(g) && group_name_count[key]==0 && group_name_flag===true)
							 lastRowInGroup = ' lp_class_seemore_seeless_last clsLastGroupnameTr';
						 else if(group_name_count[key] == 0)
				 			lastRowInGroup = ' lp_class_seemore_seeless_last';
						html += '<tr id="lp_class_seemore_seeless_'+dataId+'" class="lp_class_seemore_seeless'+lastRowInGroup+'"><td class="empty-td"></td><td class="lp-cls-separator">';
						html += '<div class="lp_class_seemore" id="lp_class_seemore_'+dataId+'" onclick="seeMoreMyLearning('+dataId+',\'myprogramclass\');">'+ Drupal.t('LBL713') +'</div>';
						html += '<div class="lp_class_seeless" id="lp_class_seeless_'+dataId+'" onclick="seeLessMyLearning('+dataId+',\'myprogramclass\');" style="display:none">'+ Drupal.t('LBL3042') +'</div>';
						html += '</td><td class="empty-td"></td></tr>';
			  	}
			}
			}
		  html += '</table>';
		  html += '</div>';
		   html += '<div class ="main-item-container">';
			if(overallStatus == 'lrn_tpm_ovr_enr' || overallStatus == 'lrn_tpm_ovr_inp'  || overallStatus == 'lrn_tpm_ovr_ppm'){
				html += '<div class="item-assigned-code status-row line-item-container">';
				if(data.is_exempted == 1){
					html += '<span class="status-label container">'+ Drupal.t('Waived') +" " + Drupal.t("by") +' : '+'</span>';
					html += '<span class="enr-statusupdate-user-cls">' + data.exempted_by+" "+Drupal.t("LBL945")+" ";
					html += '<span class="enr-status-cls">' + data.exempted_on + '</span></span>';
				}else{
					html += '<span class="status-label container">'+ Drupal.t('LBL025') +' : '+'</span>';
					html += '<span class="enr-statusupdate-user-cls">' + ((userId != managerId) ? fullName : fullName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					html += '<span class="enr-status-cls">' + regsteredDate + '</span></span>';
				}
				
				html += '</div>';
			}else if(overallStatus == 'lrn_tpm_ovr_cmp'){
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += "<span class='status-label container"+((userId != updatedBy) ? ' markcompletestatus' : '')+"'>"+((userId != updatedBy) ? Drupal.t('LBL747')+' '+Drupal.t('by') : Drupal.t('LBL681')) +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != updatedBy) ? updatedByName : updatedByName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.UpdateByName #0073510
				html += '<span class="enr-status-cls">' + completedDate + '</span></span>';
				html += '</div>';
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL025') +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != managerId) ? fullName : fullName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				html += '<span class="enr-status-cls">' + regsteredDate + '</span></span>';
				html += '</div>';
			}else if(overallStatus == 'lrn_tpm_ovr_inc'){
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL1194') +' : '+'</span>';
				if(updatedBy == 0){
					html += '<span class="enr-statusupdate-user-cls">' + Drupal.t('LBL432')+" "+Drupal.t("LBL945")+" ";
				}else{
					html += '<span class="enr-statusupdate-user-cls">' + ((userId != updatedBy) ? updatedByName : updatedByName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.UpdateByName #0073510
				}
				html += '<span class="enr-status-cls">' + updateDate + '</span></span>';
				html += '</div>';
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL025') +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != managerId) ? fullName : fullName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				html += '<span class="enr-status-cls">' + regsteredDate + '</span></span>';
				html += '</div>';
			}else if(overallStatus == 'lrn_tpm_ovr_cln'){
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL680') +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != updatedBy) ? updatedByName : updatedByName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.UpdateByName #0073510
				html += '<span class="enr-status-cls">' + canceledDate + '</span></span>';
				html += '</div>';
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL025') +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != managerId) ? fullName : fullName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				html += '<span class="enr-status-cls">' + regsteredDate + '</span></span>';
				html += '</div>';
			}else if(overallStatus == 'lrn_tpm_ovr_exp'){
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += '<span class="status-label container">'+ Drupal.t('LBL025') +' : '+'</span>';
				html += '<span class="enr-statusupdate-user-cls">' + ((userId != managerId) ? fullName : fullName)+" "+Drupal.t("LBL945")+" ";//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				html += '<span class="enr-status-cls">' + regsteredDate + '</span></span>';
				html += '</div>';
				html += '<div class="item-assigned-code status-row line-item-container">';
				html += "<span class='status-label container"+"'>"+Drupal.t('LBL028')+' : '+'</span>';
				html += '<span class="enr-status-cls enr-statusupdate-expired-cls">' + expired_on + '</span>';
				html += '</div>';
				
			}
		   html += '</div>';
			var attachLPData = eval($("#lp_attachment_"+masterEnrollId).attr("data"));
			if(attachLPData.length > 0) {
				html += '<div class="enroll-attach attach-row"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+' :'+'</span><div class="attach-info">'+ Drupal.t("LBL232") +'</div></div>';
				html += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
				var len = attachLPData.length;
				var inc = 1;
					$(attachLPData).each(function(){
							/*--Issue fix for the ticket - 32781 --*/
							html += "<li>"+"<a href='javascript:void(0)' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+titleRestrictionFadeoutImage(unescape(saniztizeJSData($(this).attr('Title'))),'exp-sp-lnrmyprogram-attachment-name')+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
							inc++;
					});
					html += "</ul></div></div>";
			}
		}
		else{
			html += "<div>No Events</div>";
		}
		$("#course-detail-section-"+masterEnrollId).html(html);
		Drupal.attachBehaviors();
		$('.enroll-show-moreclass tr').eq(-1).children('td').css('border-bottom','0px');
		//51563: When displaying the classes under a training plan, the group name of the class is not displayed
		$( ".clsGrpTr" ).prev().find('td').css( "border-bottom", "0px" );
		$( ".clsGrpTr" ).prev().find('td').css( "border-width", "0px" );
		$("#learningplan-tab-inner").data("learningplan").shapeLPButton('.enroll-show-moreclass');
		resetFadeOutByClass('.enroll-show-moreclass','item-title-code','line-item-container','MyPrograms');	

		}catch(e){
			// to do
			// console.log(e, e.stack);
		}
		vtip();
		$('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
	},

	paintLPClassActions : function(result,prgId,dataAccor){
		try{
			//called for each class paint
		var obj = $("#learningplan-tab-inner").data("learningplan");
		var objEval = '$("#learningplan-tab-inner").data("learningplan")';
		var master_enrollment_id    = result['master_enrollment_id'];
		var baseType				= result['basetype'];
		var dateValidTo				= result['valid_to'];
		var dataDelType				= result['delivery_type'];
		var dataDelTypeCode			= result['delivery_type_code'];
		var dataRegStatus			= result['reg_status'];
		var dataRegStatusCode		= result['reg_status_code'];
		var RegStatusCode			= result['reg_status_code'];
		var courseRegStatusCode		= result['reg_status_code'];
		var courseComplitionStatus	= result['reg_status'];
		var courseScore				= result['score'];
		var courseGrade				= result['grade'];
		var dataId 					= result['id'];
		var courseId 				= result['course_id'];
		var classId 				= result['class_id'];
		var classTitle				= result['cls_title'];
		var usrId					= result['user_id'];
		var classStatus				= result['cls_status'];
		var dedClass				= result['dedicated_class_flag'];
		var crsTitle				= result['course_title'];
		var courseTempId			= result['courseid'];
		var surStatus				= result['surveystatus'];
		var session_id				= result['session_id'];
		var launchInfo				= result['launch'];
		var daysRemaining 			= '';
		var callLaunchUrlFn			= '';
		var contLaunch				= false; // Used to determine whether Launch link opens an accordian for multiple content class (true)
											 // or directly launches content (false)
		var IsLaunchable			= false;
		var errmsg					= '';
		var contValidateMsg			= '';
		var isMultipleCont			= false;
		var LessonLocation			= '';
		var launch_data				= '';
		var suspend_data			= '';
		var exit					= '';
		var ManagerFullName			= result['full_name'];
		var MasterMandatory        	= result['master_mandatory'];
		var compStatus 				= result['comp_status'];
		var surStatus				= result['surveystatus'];
		var assessStatus			= result['assessmentstatus'];
		var preassessStatus			= result['preassessmentstatus'];
		
		//var attemptsleft			= result['attemptleft'];
		// var maxScoreValidation		= result['maxscorevalue'];
		var maxScoreValidationpre	= result['maxscorevaluepre'];
		var maxScoreValidationpost	= result['maxscorevaluepost'];
		var VersionId = '';
		var isSwitchAvail   		= result['show_events'];
		var userID					= result['user_id'];
		var AICC_SID 				= '';
		var MasteryScore 			= '';
		var referButtonAction		= '';
		var prevCompStatus			= result['prev_comp_status'];
		var prevClassTitle			= result['prev_class_title'];
		var courseQuizStatus		= result['course_quiz_status'];
		var hideShare               = result['hideShare'];
		var progress			    = result['progress'];
		//var progress			    = result['progressPercent'];
		
//		console.log('result here', result);
		// Assessment Validation - Max score and questions score total should be equal.
		// At least one assessment should match this condition
		// If training plan has assessment, classes shouldn't have the assessments
		//if((assessStatus != 0 && maxScoreValidation == 0) || $.ui.learningplan.parentAssessStatus[prgId] > 0 || compStatus == 'lrn_crs_cmp_inc' || compStatus == 'lrn_crs_cmp_cmp')
		// Condition for check parent assessment is removed by Vincent
		if((assessStatus != 0 && maxScoreValidationpost == 0) || compStatus == 'lrn_crs_cmp_inc')
			assessStatus = false;
		// Inside Learning plan => class Pre Or Post assement has marks are not set right
		if(assessStatus != 0 && maxScoreValidationpost == 0)
			assessStatus = false;
		if(preassessStatus != 0 && maxScoreValidationpre == 0)
			preassessStatus = false;

		var callDialogObj = '$("body").data("learningcore")';
		
		var startTime = result['session_start'];
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = result.sessionDetails.length,eTimeForm,stDateFull,enDateFull;
			var sessLenEnd,sTime,sTimeForm,eTime,eDateTime;
			if(sessLen>1) {
				sessLenEnd = sessLen-1;
				sTime     = result.sessionDetails[0].session_start_format;
				sTimeForm = result.sessionDetails[0].session_start_time_form;
				eTime 	  = result.sessionDetails[sessLenEnd].session_end_format;
				eTimeForm = result.sessionDetails[sessLenEnd].session_end_time_form;
				stDateFull = result.sessionDetails[0].session_start_time_full;
				enDateFull = result.sessionDetails[sessLenEnd].session_end_time_full;
			} else {
				sTime 	  = result.sessionDetails[0].session_start_format;
				sTimeForm = result.sessionDetails[0].session_start_time_form;
				eTime 	  = result.sessionDetails[0].session_start_end_format;
				eDateTime = result.sessionDetails[0].session_end_format;
				eTimeForm = result.sessionDetails[0].session_end_time_form;
				stDateFull = result.sessionDetails[0].session_start_time_full;
				enDateFull = result.sessionDetails[0].session_end_time_full;
			}
			var srvDate = result.sessionDetails[0].server_date_time;
			var sesStartIlt = new Date(stDateFull);
			var todayDate    = new Date(srvDate);
			var timeDiffIlt    = (sesStartIlt - todayDate);
			var type = result.sessionDetails[0].type;
		}

		if(launchInfo.length) {
			IsLaunchable 			= (launchInfo.length>1) ? true : launchInfo[0]["IsLaunchable"];//alert("IsLaunchable : "+IsLaunchable);
			isMultipleCont			= (launchInfo.length>1) ? true : false;
			errmsg 					= launchInfo[0]["NonLaunchableMessage"];
			contValidateMsg			= launchInfo[0]["contValidateMsg"];
			LessonLocation			= launchInfo[0]['LessonLocation'];
			launch_data				= launchInfo[0]['LaunchData'];
			suspend_data			= launchInfo[0]['SuspendData'];
			exit					= launchInfo[0]['CmiExit'];
			VersionId 				= launchInfo[0]['VersionId'];
			AICC_SID 				= launchInfo[0]['AICC_SID'];
			MasteryScore 			= launchInfo[0]["masteryscore"];
			// If the content is launchable, prepare the launch URL call function string in variable callLaunchUrlFn
			if(IsLaunchable) {
				var id 					= launchInfo[0]["ID"];
				var url1 				= launchInfo[0]["LearnerLaunchURL"];
				var contentTitle		= launchInfo[0]["Title"];
				var url2 				= launchInfo[0]["PresenterLaunchURL"];
				var status 				= launchInfo[0]["Status"];
				var launchBaseType		= launchInfo[0]["baseType"];
				var cntType				= launchInfo[0]["ContentType"];
				var cntSubTypeCode		= launchInfo[0]["ContentSubTypeCode"];
				if(baseType == 'VOD') {
					var statusCode 		= launchInfo[0]["StatusCode"];
					// var suspendData		= (launchInfo[0]["SuspendData"] != null && launchInfo[0]["SuspendData"] != "") ? JSON.parse(unescape(unescape(launchInfo[0]["SuspendData"]))) : null;
				}
				var inc = 0;
				if(launchInfo.length > 1) {  // Class has multiple contents, launch URL call function will launch the accordian
					contLaunch = true; // Launch link opens an accordian
					var overrideIsLaunchable = false; // Variable used to determine WBT and VOD class launchability from the launchability of each content
					// Prepare content launch data for rendering the content launch links in the accordian
					var passParams = "[";
					$(launchInfo).each(function(){
						 inc=inc+1;
						 passParams += "[{";
						 passParams += "'Id':'"+$(this).attr("ID")+"'";
						 passParams += ",'ContentId':'"+$(this).attr("ContentId")+"'";
						 passParams += ",'VersionId':'"+$(this).attr("VersionId")+"'";
						 passParams += ",'VersionNo':'"+$(this).attr("VersionNum")+"'";
						 passParams += ",'enrollId':'"+dataId+"'";
						 passParams += ",'Title':'"+$(this).attr("Title")+"'";
						 passParams += ",'ContentTitle':'"+escape($(this).attr("Code"))+"'";
						 passParams += ",'url1':'"+$(this).attr("LearnerLaunchURL")+"'";
						 passParams += ",'courseId':'"+courseId+"'";
						 passParams += ",'IsLaunchable':'"+$(this).attr("IsLaunchable")+"'";
						 passParams += ",'AttemptLeft':'"+$(this).attr("AttemptLeft")+"'";
						 passParams += ",'ValidityDays':'"+$(this).attr("ValidityDays")+"'";
						 passParams += ",'remDays':'"+$(this).attr("remDays")+"'";
						 passParams += ",'daysLeft':'"+$(this).attr("daysLeft")+"'";
						 passParams += ",'contValidateMsg':'"+$(this).attr("contValidateMsg")+"'";
						 passParams += ",'classId':'"+classId+"'";
						 passParams += ",'url2':'"+$(this).attr("PresenterLaunchURL")+"'";
						 passParams += ",'ErrMsg':''";
						 passParams += ",'contentType':'"+$(this).attr("ContentType")+"'";
						 passParams += ",'contentSubTypeCode':'"+$(this).attr("ContentSubTypeCode")+"'";
						 passParams += ",'launchType':'"+$(this).attr("LaunchType")+"'";
						 passParams += ",'ClsScore':'"+$(this).attr("ClsScore")+"'";
						 passParams += ",'LesScore':'"+$(this).attr("LesScore")+"'";
						 passParams += ",'ContScore':'"+$(this).attr("ContScore")+"'";
						 passParams += ",'Status':'"+$(this).attr("Status")+"'";
						 passParams += ",'ContentStatus':'"+$(this).attr("ContentStatus")+"'";
						 passParams += ",'Lessoncnt':'"+$(this).attr("Lessoncnt")+"'";
						 passParams += ",'LessonLocation':'"+$(this).attr("LessonLocation")+"'";
						 passParams += ",'launch_data':'"+$(this).attr("LaunchData")+"'";
						 passParams += ",'suspend_data':'"+escape($(this).attr("SuspendData"))+"'";
						 passParams += ",'exit':'"+$(this).attr("CmiExit")+"'";
						 passParams += ",'AICC_SID':'"+$(this).attr("AICC_SID")+"'";
						 passParams += ",'MasteryScore':'"+$(this).attr("masteryscore")+"'";
						 passParams += ",'contentQuizStatus':'"+$(this).attr("contentQuizStatus")+"'";
						 passParams += ",'ContentCompletionStatus':'"+$(this).attr("ContentCompletionStatus")+"'";
						 if(baseType == "VOD") {
							passParams += ",'StatusCode':'"+$(this).attr("StatusCode")+"'";
						 }
						 passParams += "}]";
						 if(inc<launchInfo.length) {
							 passParams += ",";
						 }
						 // Determine launchability of WBT and VOD class from the launchability of each included content
						 if (baseType == "WBT" || baseType == "VOD") {
							 overrideIsLaunchable = overrideIsLaunchable || $(this).attr("IsLaunchable");
						 }
					});
					passParams += "]";
					// passParams = "";
					// Launchability of a multiple content WBT and VOD class is determined from the launchability of each included content
					if (baseType == "WBT" || baseType == "VOD") {
						IsLaunchable = overrideIsLaunchable;
					}
					//callLaunchUrlFn = 'onclick=\''+objEval+'.addAccordionLaunchView("open_close","0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPLaunchAccordion,this,'+objEval+',true);\'';
					callLaunchUrlFn = 'onclick=\''+objEval+'.launchLPMultiContent('+dataId+',this);\'';
				} else { // Class has single content
					contLaunch = false; // Launch link directly launches the content
					if((baseType == "WBT") && (cntType != null) && (cntType != undefined) && (cntType != '')) {
						var params 		= "{'Id':'"+id+"','enrollId':'"+dataId+"','VersionId':'"+VersionId+"','url1':'"+url1+"','courseId':'"+courseId+"','classId':'"+classId+
						                   "','url2':'"+url2+"','ErrMsg':'"+errmsg+"','contentType':'"+cntType+"','Status':'"+status+
						                     "','LessonLocation':'"+LessonLocation+"','LaunchFrom':'LP','launch_data':'"+launch_data+"','contentQuizStatus':'"+courseQuizStatus+
						                         "','suspend_data':'"+suspend_data+"','exit':'"+exit+"','AICC_SID':'"+AICC_SID+"','MasteryScore':'"+MasteryScore+"'}";
						callLaunchUrlFn = "onclick=\"$('#learningplan-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";

				  }
          else if (baseType == "VOD") {
            // The title for video is the content title.
            var title = contentTitle;
            title = encodeURIComponent(title.replace(/\//g, '()|()'));
			// var progress = suspendData != null ? suspendData['progress'] : 0;
						//console.log($(this));
			//console.log(progress);
            callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
            callLaunchUrlFn += title + "/";
            callLaunchUrlFn += escape(cntSubTypeCode) + "/";
            callLaunchUrlFn += escape(url1.replace(/\//g, '()|()')) + '/';
            callLaunchUrlFn += "LP" + "/";
            callLaunchUrlFn += courseId + "/";
            callLaunchUrlFn += classId + "/";
            callLaunchUrlFn += id + "/";
            callLaunchUrlFn += VersionId + "/";
            callLaunchUrlFn += dataId + "/";
            callLaunchUrlFn += escape(statusCode) + "/";
			// callLaunchUrlFn += progress + "/";
			callLaunchUrlFn += launchInfo[0]["SuspendData"]+ "\"";
          }
          else if(baseType == "WBT" || baseType == "VC") {
						callLaunchUrlFn	= url1;
				  }
				}
			}
		}
		
		
		
		// Calculate days remaining for the WBT and VOD content, to disallow cancellation of an expired WBT classe.
		if((baseType=="WBT" || baseType == "VOD") &&
			 ((courseComplitionStatus=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") ||
					 			(dataRegStatusCode=="lrn_crs_cmp_inp") || (dataRegStatusCode=="lrn_crs_cmp_inc"))) {
			var i=0;
			if(dateValidTo!='' && dateValidTo != null){
				  var daystocount = new Date(dateValidTo);
				  //daystocount.setDate(daystocount.getDate()+30)
				  var srvDate = launchInfo[0].server_date_time;
				  today=new Date(srvDate);
				  var oneday=1000*60*60*24;
				  daysRemaining = (Math.ceil((daystocount.getTime()-today.getTime())/(oneday)));
			}
		}
        var vClassId = classId;
        if(id==null || id==undefined || id=='undefined')
        	id='0';
        var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId+"###"+VersionId;

		var data1;
		var contdata;
		var multiContentData;
		if(contLaunch) {
			//" + options.rowId + "
			// Multiple contents in class, prepare data for the accordian
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'','CourseId':'" + courseId +
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode +
							"','regstatus':'" + dataRegStatusCode + "','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode +
							"','daysRemaining':'" + daysRemaining + "','courseComplitionStatus':'" + courseComplitionStatus +
							"','courseScore':'" + courseScore + "','courseGrade':'" + courseGrade+"','detailTab':'true'}";
			contdata = "data=["+passParams+"]";
			multiContentData = passParams;
		} else {
			// Prepare data for content launch link
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'','CourseId':'" + courseId +
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode + "','regstatus':'" + dataRegStatusCode +
							"','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode + "','daysRemaining':'" + daysRemaining +
							"','courseComplitionStatus':'" + courseComplitionStatus + "','courseScore':'" + courseScore +
							"','courseGrade':'" + courseGrade + "','detailTab':'true'}";
		}

		// Prepare the Action HTML to be returned.
		var html = '';
		var setDropButton = true; // Allow cancellation (drop) of class when setDropButton is set to true.
		if((baseType == 'WBT' || baseType == 'VOD')  && daysRemaining < 0){
			setDropButton = false; // Disallow cancellation of class if the WBT or VOD class has expired.
		}
        
		html += "<div class='enroll-main-list enroll-main-action-lp-multiclasss' id='enroll-main-action-lp-"+dataId+"'>";
		var getEnrData 		= eval($("#sort-by-lp-enroll").attr("data"));
		var overallStatus 	= result['overall_status']; //getEnrData.currEnrMode;
		var inc_can_cmp = 0;
		if((overallStatus == 'lrn_tpm_ovr_inc' || overallStatus == 'lrn_tpm_ovr_cln') && RegStatusCode == 'lrn_crs_cmp_cmp')
			var inc_can_cmp = 1;
		if(compStatus == "lrn_crs_cmp_inc"){
			html += '<span id="lp-class-accodion-'+dataId+'" data="'+dataAccor+'" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'></span><a  href="javascript:void(0);"  class="actionLink enroll-launch-full clsDisabledButton" >'+Drupal.t("LBL816")+'</a>';
		}
		else if(overallStatus == 'lrn_tpm_ovr_enr' || overallStatus == 'lrn_tpm_ovr_inp' || overallStatus == 'lrn_tpm_ovr_cmp' || inc_can_cmp == 1){
					// Prepare Action button HTML for My Enrollments 'Enrolled' tab
					if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_inp' || RegStatusCode == 'lrn_crs_cmp_inc' || RegStatusCode == 'lrn_crs_cmp_cmp'||RegStatusCode == 'lrn_crs_cmp_nsw' ||RegStatusCode == 'lrn_crs_cmp_att'){
							      var isILT = false;
							      var isWBT = false;
							      var isVC = false;
							      var isVOD = false;
							      var wbtButtonAction = null; // Variable later used to determined what links to show in the More menu.
							      var vodButtonAction = null;
							      var iltButtonAction = null;
							      var viewButton = null;
							      var ILTChangeButton = null;
							      var ILTShareButton = null;
							      var ILTviewButton = null;
							      var ILTSurveyButton = null;
							      var ILTPreAssButton = null;
							      var ILTPostAssButton = null;
							      var vcjoinButton = null;
							      var enrollAction = '';
							      var preassValue ='';
							      var postassValue ='';
							      var no_primary_html_button='0';
								  var no_drop_down_option='0';			
							      var surObj = '$("#block-take-survey").data("surveylearner")';
									if(prevCompStatus != 'notallow'){
										preassValue = surObj + ".callTakeSurveyToClass(" + classId + ",\"" + escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ", 1,\"enroll-lp-result-container\");' >";
										postassValue= surObj + ".callTakeSurveyToClass(" + classId + ",\"" + escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ", 0,\"enroll-lp-result-container\");' >";
									}else{
										var titleLength = classTitle.length;
										if(this.currTheme == 'expertusoneV2' && titleLength > 50) {
											titleLength = 50;
										}
										callpopupValue = callDialogObj + ".callMessageWindow(\"" + classTitle + "\",\"" + Drupal.t("You need to complete") +" " +escape(prevClassTitle)+" " + Drupal.t("Course") + "\","+titleLength+");' >" ;
									}
									var preassPopup = (prevCompStatus != 'notallow')? preassValue : callpopupValue;
									var postassPopup = (prevCompStatus != 'notallow')? postassValue : callpopupValue;
									var DedClass = (dedClass == 'Y');
										if (dataDelTypeCode == 'lrn_cls_dty_wbt') { // Show correct button for the WBT class
												  isWBT = true;
												
													// If is launchable, show the Launch button
												  if (IsLaunchable) {
														var launchLabelVal  = Drupal.t("LBL199");
														if(prevCompStatus != 'notallow'){
															wbtButtonAction = 'Launch';	
															if(DedClass){
															   classlaunch = (DedClass && (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)) ? ' enroll-launch' : ' enroll-launch-full';
															}else{ 
															       if((hideShare==0) || (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)){
															         classlaunch = ' enroll-launch';
															       } 
															       else{
															        classlaunch = ' enroll-launch-full';
															       } 
															}						
															html += "<input type='button' class='actionLink "+classlaunch+" ' " +
															//html += "<input type='button' class='actionLink enroll-launch' " +
																		"data=\"" + multiContentData + "\" " +
																			"value='" + launchLabelVal + "' " +
																				"name='" + SMARTPORTAL.t("Launch") + "' " +
																					"id='launch-lp-class-" + dataId + "' " +
																						callLaunchUrlFn +
																							" >";
														}else{
															var titleLength = classTitle.length;
															if(this.currTheme == 'expertusoneV2' && titleLength > 50) {
																titleLength = 50;
															}
															html += "<input type='button' class='actionLink enroll-launch' " +
																		"data=\"" + multiContentData + "\" " +
																			"value='" + launchLabelVal + "' " +
																				"name='" + "Launch" + "' " +
																					"id='launch-lp-class-" + dataId + "' " +
																					"onclick='" +
																                    callDialogObj + ".callMessageWindow(\"" + classTitle + "\",\"" + Drupal.t("You need to complete") +" " +escape(prevClassTitle) +" "+ Drupal.t("Course") + "\", "+titleLength+");' >";
														}
													} else{ //	0073403: My Learning - When content player is off,view button is breaking inclass level under tp
									//		           		if(surStatus == 'TRUE' || assessStatus == 'TRUE' || classStatus == 'lrn_cls_sts_atv'){
									//									viewButton = 'viewSurAssShareDetails';
									//									classlaunch = 'enroll-launch';
									//									
									//						}
									//						else{
									//									viewButton = 'viewDetails';
									//									classlaunch = 'enroll-launch-full';
									//									
									//							}
									//		        	   html += '<a id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="actionLink '+classlaunch+'" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
														  ILTShareButton = true;
														  var classlaunch = ' enroll-launch-full';
													    	if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)
																classlaunch = ' enroll-launch';
															else if(preassessStatus == 'TRUE') {
																classlaunch = ' enroll-launch';
															}
															else if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
																classlaunch = ' enroll-launch';
															}															
															if(hideShare == 0){ 
															  	 html += '<a href="javascript:void(0)" title="'+Drupal.t("Share")+'" class="vtip actionLink '+classlaunch+'" data=\"' + data1 +
																'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
																'\',\'cre_sys_obt_cls\',\'enroll-lp-result-container\');" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
															}else{
																no_primary_html_button='1';
																viewButton = 'yes_require_view_button';
															}  
															
										        	}
										}else if (dataDelTypeCode == 'lrn_cls_dty_vod') { // Show correct button for the VOD class
											          isVOD = true;
											          // If is launchable, show the Launch button
											          if (IsLaunchable) {
											         	 if(prevCompStatus != 'notallow'){
											         		  vodButtonAction = 'Launch';
											         //		  console.log('launch'+launchInfo.length);
											         		  if (launchInfo.length > 1) {      
											          			       if(DedClass){
																		   classlaunch = (DedClass && (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)) ? ' enroll-launch' : ' enroll-launch-full';
																		}else{ 
																		       if((hideShare==0) || (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)){
																		         classlaunch = ' enroll-launch';
																		       } 
																		       else{
																		        classlaunch = ' enroll-launch-full';
																		       } 
																		}
											                                 html += "<input type='button' class='actionLink"+classlaunch+" ' " +
											         			  			"data=\"" + multiContentData + "\" " +
											         			  			"value='" +Drupal.t("LBL199") + "' " +
											         			  			"name='" + SMARTPORTAL.t("Launch") + "' " +
											         			  			"id='launch-lp-class-" + dataId + "' " +
											         			  			callLaunchUrlFn +
											         			  			" >";
											         		  } else {
											               		   if(DedClass){
																		   classlaunch = (DedClass && (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)) ? ' enroll-launch' : ' enroll-launch-full';
																		}else{ 
																		       if((hideShare==0) || (surStatus == 'TRUE')||(assessStatus == 'TRUE')||(preassessStatus == 'TRUE') || ((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)){
																		         classlaunch = ' enroll-launch';
																		       } 
																		       else{
																		        classlaunch = ' enroll-launch-full';
																		       } 
																		}				
											
											         			  html += "<a class='actionLink"+classlaunch+" use-ajax ctools-modal-ctools-video-style' " +
											         			  "title='" + Drupal.t("LBL199") + "' " +
											         			  "id='launch-lp-class-" + dataId + "' " +
											         			  callLaunchUrlFn +
											         			  " >" +
											                       Drupal.t("LBL199") + "</a>";
											         		  }
											         	 }else{
											         		var titleLength = classTitle.length;
															if(this.currTheme == 'expertusoneV2' && titleLength > 50) {
																titleLength = 50;
															}
											         		  html += "<a class='actionLink enroll-launch ctools-modal-ctools-video-style' " +
											                   "title='" + Drupal.t("LBL199") + "' " +
											                     "id='launch-lp-class-" + dataId + "' " +
											                     "onclick='" +
											                     callDialogObj + ".callMessageWindow(\"" + classTitle + "\",\"" + Drupal.t("You need to complete") +" " +prevClassTitle +" "+ Drupal.t("Course") + "\", "+titleLength+");' >" +
											                           Drupal.t("LBL199") + "</a>";
											
											         	  }
											           }
											         // Otherwise show Survey button if survey is present and surveylearner module is enabled
											          else{ //	0073403: My Learning - When content player is off,view button is breaking inclass level under tp
											        	  ILTShareButton = true;
											        	  var classlaunch = ' enroll-launch-full';
													    	if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)
																classlaunch = ' enroll-launch';
															else if(preassessStatus == 'TRUE') {
																classlaunch = ' enroll-launch';
															}
															else if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
																classlaunch = ' enroll-launch';
															}
															if(hideShare == 0){ 
															  	 html += '<a href="javascript:void(0)" title="'+Drupal.t("Share")+'" class="vtip actionLink '+classlaunch+'" data=\"' + data1 +
															'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
															'\',\'cre_sys_obt_cls\',\'enroll-lp-result-container\');" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
															}else{
																no_primary_html_button='1';
																viewButton = 'yes_require_view_button';
															}
											//        	  viewButton = 'viewDetails';
											//        	  html += '<a id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="actionLink enroll-launch" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
											       	   }
						        }else if (dataDelTypeCode == 'lrn_cls_dty_vcl') { // Show enabled/disabled Launch button for the VCL class
											var isVC = true;
						
											var sessTimeFormat = sTime+':00 ' + sTimeForm;
											var sessEndTimeFormat = eDateTime+':00 ' + eTimeForm;
											var sesStartVC = new Date(stDateFull);
											var sesEndVC   = new Date(enDateFull);
											var todayVC    = new Date(srvDate);
											var timeDiffVC    = (sesStartVC - todayVC);
											var endTimeDiffVC =(sesEndVC - todayVC);
						
											var sessEndDate =enDateFull;
											var meeting_complete = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete) : 30;
											var sesEndDateTime = new Date(sessEndDate);
											sesEndDateTime.setMinutes(sesEndDateTime.getMinutes()+parseInt(meeting_complete));
											var sesEndTimeStamp	= sesEndDateTime.getTime();
											var todayTimeStamp	= todayDate.getTime();
											
											var allowTime = (resource.allow_meeting_launch) ? (resource.allow_meeting_launch * 60000) : 3600000;
											//Launch appears only in the configured time  and after half an hour to session end. If not configured allow_meeting_launch then system take default as 1 hour from session start
											var afterCompleteAllowTime = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete * 60000) : 1800000;
											//Launch appears only in the configured time. If not configured allow_meeting_launch then system take default as half hour from session end
											var classvclaunch = ' enroll-launch-full';
											if (((isILT || isVC) && timeDiffIlt >= 0) && classStatus == 'lrn_cls_sts_atv' && dedClass != 'Y') 
												classvclaunch = ' enroll-launch';
											else if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)
												classvclaunch = ' enroll-launch';
											else if(preassessStatus == 'TRUE') {
												classvclaunch = ' enroll-launch';
											}
											else if (assessStatus == 'TRUE') {
												if(todayTimeStamp > sesEndTimeStamp)
													classvclaunch = ' enroll-launch';
											}
											else if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) && isVC) {
												classvclaunch = ' enroll-launch';
											}
						
											if(timeDiffVC <= allowTime && (endTimeDiffVC+afterCompleteAllowTime) >=0){
												var a = (sesEndVC-todayVC);
												var timer = (afterCompleteAllowTime+a);
												if(RegStatusCode == 'lrn_crs_cmp_att' && type == 'lrn_cls_vct_web'){
													var new_window;
													new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink "+classvclaunch+" ' data=\"" + contdata + "\" " +
															"' name='" + SMARTPORTAL.t("Launch") + "' " +
																" id='launch" + dataId + "' href='javascript:void(0);' " +
																		//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")'>"+ Drupal.t("Re-Join") + "</a>";
																"onclick='"+new_window+";'>"+ Drupal.t("LBL1319") + "</a>";
												refresh_timelp(timer);
												}else{
												var new_window;
												classvclaunch = (DedClass) ? ' enroll-launch-full' : ((hideShare == 0)||(preassessStatus == 'TRUE')||(surStatus == 'TRUE')|| classStatus == 'lrn_cls_sts_atv' )? ' enroll-launch' : ' enroll-launch-full';
												new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink vtip "+classvclaunch+" ' data=\"" + contdata + "\" " +
															"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
																" id='launch" + dataId + "' href='javascript:void(0);' " +
																		//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=yes,location=yes,directories=yes,status=yes,menubar=yes,scrollbars=yes,copyhistory=yes,resizable=no\")'>"+ Drupal.t("LBL880") + "</a>";
																"onclick='"+new_window+";refresh_enrolledlp("+new_window+");'>"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
											/*	html += "<a class='actionLink "+classvclaunch+" ' data=\"" + contdata + "\" " +
															"' name='" + SMARTPORTAL.t("Launch") + "' " +
																" id='launch" + dataId + "' href='javascript:void(0);' " +
																		" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=yes,location=yes,directories=yes,status=yes,menubar=yes,scrollbars=yes,copyhistory=yes,resizable=no\")'>"+ Drupal.t("LBL880") + "</a>"; */
												}
											}
											else{
												var title ,message;
												title =Drupal.t("LBL670");//Session Details
												if(endTimeDiffVC+afterCompleteAllowTime < 0){
													message = Drupal.t("MSG419");
												}
												else{
													message = Drupal.t("MSG420") +' '+ sessTimeFormat;
												}
												var vcchangeClass = 0;
												if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail)
													vcchangeClass =1;
												classvclaunch = (DedClass && vcchangeClass==0 && preassessStatus != 'TRUE' && surStatus != 'TRUE') ? ' enroll-launch-full' : ((hideShare == 0 && classStatus == 'lrn_cls_sts_atv' && timeDiffIlt >= 0)||(preassessStatus == 'TRUE')||(surStatus == 'TRUE') || vcchangeClass==1)? ' enroll-launch' : ' enroll-launch-full';
												if (assessStatus == 'TRUE') {
												if(todayTimeStamp > sesEndTimeStamp)
													classvclaunch = ' enroll-launch';
													}
												html += "<a class='actionLink vtip "+classvclaunch+" ' data=\"" + contdata + "\" " +
								        				"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
								        					" id='launch" + dataId + "' href='javascript:void(0);' " +
																			"onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
																				" >"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
											}
								}
								else {
										  isILT = true;
										  var todayDate    = new Date(srvDate);
										  var todayTimeStamp	= todayDate.getTime();
										  var sesEndDateTime = new Date(enDateFull);
										  var sesEndTimeStamp	= sesEndDateTime.getTime();
										  
										  var changeClassIsThere='0'; // To check that is it he chageclass are there or not
										if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail && ILTChangeButton===null){
											changeClassIsThere='1';
										}
										  
										  if ((isWBT || isVOD || ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv' && dedClass != 'Y') {
										    	ILTShareButton = true;
										    	var classlaunch = ' enroll-launch-full';
										     	var classlaunchNew='enroll-launch-full';
										    	if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail){
										    		classlaunch = ' enroll-launch';
													classlaunchNew='enroll-launch';
												}
												else if(preassessStatus == 'TRUE') {
													classlaunch = ' enroll-launch';
													classlaunchNew='enroll-launch';
												}
												else if (assessStatus == 'TRUE' && todayTimeStamp > sesEndTimeStamp){
														classlaunch = ' enroll-launch';
														classlaunchNew='enroll-launch';
												}
												else if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
													classlaunch = ' enroll-launch';
													classlaunchNew='enroll-launch-full';
												}
												if(hideShare==0){  
											    	
													html += '<a href="javascript:void(0)" title="'+Drupal.t("Share")+'" class="vtip actionLink '+classlaunch+'" data=\"' + data1 +
													'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
													'\',\'cre_sys_obt_cls\',\'enroll-lp-result-container\');" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
												
												}else if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail){
												    	ILTChangeButton = true;
												    	
												    	if((surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner))||(assessStatus == 'TRUE' && todayTimeStamp > sesEndTimeStamp)||(preassessStatus == 'TRUE')){
												    		var classlaunch = ' enroll-launch';
												    	}else{
												    		var classlaunch = ' enroll-launch-full';
												    	}
												    	
												    	html += '<a href="javascript:void(0)" class="actionLink vtip  '+classlaunch+' " '+
															'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + userID + ',' + classId + ',' + courseId + ',' + dataId + ', \'Tp\');" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" >' +
															titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),'exp-sp-mylearning-menulist') + '</a>';
													    no_primary_html_button='0';
												    }else{	
												 	//if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)){ 	
													 /*	html += "<a href='javascript:void(0);' class='actionLink vtip "+classlaunchNew+"' data=\"" + data1 +
															"\" name='" + SMARTPORTAL.t("Survey") + "' onclick='" +
															surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																									escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																Drupal.t("Survey") + "</a>";*/
													 //} 
													 
													no_primary_html_button='1';
													viewButton = 'yes_require_view_button';
												 }					 
										    }
										    //to get survey option
										    else if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail && surStatus != 'TRUE'){
										    	ILTChangeButton = true;
										    	var classlaunch = ' enroll-launch-full';
										    	if(preassessStatus == 'TRUE') {
													classlaunch = ' enroll-launch';
												}
												else if (assessStatus == 'TRUE' && todayTimeStamp > sesEndTimeStamp){
														classlaunch = ' enroll-launch';
												}
												else if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
													classlaunch = ' enroll-launch';
												}
											    html += '<a href="javascript:void(0)" class="actionLink vtip  '+classlaunch+' " '+
													'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + userID + ',' + classId + ',' + courseId + ',' + dataId + ', \'Tp\');" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" >' +
													titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),'exp-sp-mylearning-menulist') + '</a>';
										    }else{
										    //	console.log('isSwitchAvail::'+isSwitchAvail);
										    	if ( surStatus != 'TRUE' && preassessStatus != 'TRUE' && assessStatus != 'TRUE' && changeClassIsThere != '1'){
										    		ILTviewButton = true;
										    	//	alert('disabled view');
										    		html += '<span id="lp-class-accodion-'+dataId+'" data="'+dataAccor+'" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'></span><a  href="javascript:void(0);"  class="actionLink enroll-launch-full clsDisabledButton" >'+Drupal.t("LBL816")+'</a>';
										    	
										    	//ilt test future pressessment coming on top-
										    	}else if(preassessStatus == 'TRUE'  && surStatus != 'TRUE'){
						 						  ILTPreAssButton = true;
										    		var classlaunch = ' enroll-launch-full';
											    	if(surStatus == 'TRUE'  && (availableFunctionalities.exp_sp_surveylearner)) {
														classlaunch = ' enroll-launch';
													}
													else if (assessStatus == 'TRUE' && todayTimeStamp > sesEndTimeStamp){
															classlaunch = ' enroll-launch';
													}
											    	html += "<a href='javascript:void(0);' class='actionLink vtip "+classlaunch+" ' data=\"" + data1 +
													"\" name='takesurvey' title='"+Drupal.t("Assessment") + ' ' +Drupal.t("LBL1253")+"' onclick='" +preassPopup
													+titleRestrictionFadeoutImage(Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" ,'exp-sp-mylearning-menulist')+ "</a>";
										    	//-!past ilt ded
										    	}else if (assessStatus == 'TRUE'  && surStatus != 'TRUE'  && todayTimeStamp > sesEndTimeStamp){
										  	      var ILTPostAssButton = true;
										    		var classlaunch = ' enroll-launch-full';
											    	if(preassessStatus == 'TRUE') {
														classlaunch = ' enroll-launch';
													}else if(surStatus == 'TRUE'  && (availableFunctionalities.exp_sp_surveylearner)) {
														classlaunch = ' enroll-launch';
													}
											    	if(todayTimeStamp > sesEndTimeStamp){
											    		html += "<a href='javascript:void(0);' class='actionLink vtip "+classlaunch+" ' data=\"" + data1 +
																"\" name='takesurvey' title='"+Drupal.t("Assessment") + ' ' +Drupal.t("LBL871")+"' onclick='" +postassPopup
																+titleRestrictionFadeoutImage(Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" ,'exp-sp-mylearning-menulist')+ "</a>";
														}
												//--for dedicated class issues
										    	}else if(surStatus == 'TRUE'  && (availableFunctionalities.exp_sp_surveylearner) && !isSwitchAvail && (dedClass != 'Y') ){
										    		var ILTSurveyButton = true;
										    		var classlaunch = ' enroll-launch-full';
											    	if(preassessStatus == 'TRUE') {
														classlaunch = ' enroll-launch';
													}
													else if (assessStatus == 'TRUE' && todayTimeStamp > sesEndTimeStamp){
															classlaunch = ' enroll-launch';
													}
										    		iltButtonAction = 'Survey';
													var surObj = '$("#block-take-survey").data("surveylearner")';
													/*html += "<a href='javascript:void(0);' class='actionLink "+classlaunch+"' data=\"" + data1 +
													"\" name='" + SMARTPORTAL.t("Survey") + "' onclick='" +
													surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
															escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
														Drupal.t("Survey") + "</a>";*/
										    	}
										    }
										    
										    
										    if ( surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) && ILTSurveyButton == null && (dedClass == 'Y')){
													iltButtonAction = 'Survey';
													var surObj = '$("#block-take-survey").data("surveylearner")';
													
													//-for ilt ded class
												//	console.log(timeDiffIlt);
													if((isILT || isVC)  && timeDiffIlt < 0)			// Past session										
													no_primary_html_button ='1';
												//	console.log('no_primary_html_button::'+no_primary_html_button);
													  	if(no_primary_html_button=='1')            { //make SURVEY button as primary
										   		
													   		if(changeClassIsThere=='1' || assessStatus=='TRUE' || preassessStatus == 'TRUE'){ //if there is change class option or post assessment
															    	var classLaunchNew='enroll-launch';
														    }else{
														    		var classLaunchNew='enroll-launch-full';
															}  
															//-for @past ilt ded break 
															
															if(isSwitchAvail && surStatus == 'TRUE' && timeDiffIlt < 0){
															var classLaunchNew='enroll-launch';														    	
															}
														//	alert(1);
																html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" '  data=\"" + data1 +
																"\" name='" + SMARTPORTAL.t("Survey") + "' onclick='" +
																surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																										escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																	Drupal.t("Survey") + "</a>";
																	
															      no_primary_html_button='0';		
														
														 	    
														}else{
														
													if(isILT || isVC){	
														if(changeClassIsThere=='1'  || preassessStatus == 'TRUE'){ //if there is change class option or post assessment
															    	var classLaunchNew='enroll-launch';
														    }else{
														    		var classLaunchNew='enroll-launch-full';
															} 
														
																
															//	alert(2);
																html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" '  data=\"" + data1 +
																"\" name='" + SMARTPORTAL.t("Survey") + "' onclick='" +
																surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																										escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																	Drupal.t("Survey") + "</a>";
														} else 
														
														enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
															"\" name='" + SMARTPORTAL.t("Survey") + "' onclick='" +
															surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																									escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																Drupal.t("Survey") + "</a></li>";
															//	alert(6);
														
														
														}
											}					
										    iltViewButton = 'viewDetails';
										    
											// Add html to show the More menu-button/menu (enabled or disabled as appropriate)
										    
										    if(ILTviewButton===null && iltButtonAction==null)
										    	html += '<a  style="display:none;" id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="actionLink " onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
										    else if(iltButtonAction=='Survey'){
										    	html += '<li class=\'action-enable\' style="display:none;"><a   id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="actionLink " onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a></li>';
										    }			    
										    
								  }	
								  
								 	/*	var changeClassIsThere='0'; // To check that is it he chageclass are there or not
										if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail && ILTChangeButton===null){
											changeClassIsThere='1';
										}*/
										
										
										
										var surObj		= '$("#block-take-survey").data("surveylearner")';
										//main issue fix-78331
										if((isILT || isVC) && surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) && timeDiffIlt < 0 && (dedClass != 'Y')) {
											no_primary_html_button='1';
											ILTShareButton = true;
										}
										if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) &&
													((isVOD && vodButtonAction == 'Launch') || (isWBT && wbtButtonAction == 'Launch') || isVC || ILTShareButton) ) {
										//   	console.log('hideShare::'+no_primary_html_button);
										   	if(no_primary_html_button=='1'){ //make SURVEY button as primary
										   	//	console.log('hideShare::'+hideShare);
											   		if(changeClassIsThere=='1' || (assessStatus=='TRUE' && timeDiffIlt < 0) || preassessStatus == 'TRUE' || ((isVOD || isWBT) && hideShare == 0 ) || ((isILT || isVC) && timeDiffIlt > 0 && hideShare == 0 )){ //if there is change class option or post assessment
													    	var classLaunchNew='enroll-launch';
												    }else{
												    		var classLaunchNew='enroll-launch-full';
													}  	    
											   		// alert(3);
											   		html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" '  data=\"" + data1 +
																	"\" name='takesurvey' onclick='" +
																		surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																									escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																			Drupal.t("Survey") + "</a>";
																			
													no_primary_html_button='0';																		
																		
										   	}else{
										   			enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
																"\" name='takesurvey' onclick='" +
																	surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																								escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\", \"\"," + dataId + ",\"NULL\",\"enroll-lp-result-container\");' >" +
																		Drupal.t("Survey") + "</a></li>";
																	//	alert(4);
										   	}				
											
										}
													if(baseType=='ILT' || baseType=="VC"){
													var todayDate    = new Date(srvDate);
													var todayTimeStamp	= todayDate.getTime();
													if(baseType=="VC"){
														var meeting_complete = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete) : 30;
														var sesEndDateTime = new Date(enDateFull);
														sesEndDateTime.setMinutes(sesEndDateTime.getMinutes()+parseInt(meeting_complete));
														var sesEndTimeStamp	= sesEndDateTime.getTime();
													}else{
														var sesEndDateTime = new Date(enDateFull);
														var sesEndTimeStamp	= sesEndDateTime.getTime();
													}
												//	console.log('preassessStatus::'+preassessStatus);
												//	console.log('ILTPreAssButton::'+ILTPreAssButton);
													if(preassessStatus == 'TRUE' && ILTPreAssButton === null) { 
												//		console.log('assessment:' +no_primary_html_button);
															if(no_primary_html_button=='1'){ //make PRE ASSESSMENT button as primary
															
															    if(changeClassIsThere=='1' || (assessStatus=='TRUE' && timeDiffIlt < 0)){ //if there is change class option
															    	var classLaunchNew='enroll-launch';
															    }else{
															    	var classLaunchNew='enroll-launch-full';
															    }
															    
																html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" '  data=\"" + data1 +
																"\" name='takesurvey' onclick='" +preassPopup
																+Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a>";
																
																no_primary_html_button='0';
															}else{
															
														if(changeClassIsThere!='1'  && (assessStatus != 'TRUE' && timeDiffIlt < 0)  && surStatus != 'TRUE' && hideShare == 1)
															
															html += "<a href='javascript:void(0);' class='actionLink vtip enroll-launch-full '  data=\"" + data1 +
																"\" name='takesurvey' onclick='" +preassPopup
																+Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a>";
														else	
															
																	enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
																"\" name='takesurvey' onclick='" +preassPopup
																+Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
															}
															
															
															
																					
													}
												//	console.log('assessStatus::'+assessStatus);
													if (assessStatus == 'TRUE' && ILTPostAssButton == null) {
														if(todayTimeStamp > sesEndTimeStamp){
														
													//	console.log('postassessment:'+no_primary_html_button);
															if(no_primary_html_button=='1'){ //make POST ASSESSMENT button as primary 
															
															    if(changeClassIsThere=='1'){ //if there is change class option 
															    	var classLaunchNew='enroll-launch';
															    }else{
															    	var classLaunchNew='enroll-launch-full';
															    }
															    
															    html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" ' data=\"" + data1 +
																"\" name='takesurvey' onclick='" +postassPopup
																+Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a>"; 
																 
																no_primary_html_button='0';
																
															}else{
															
															
														
															if(changeClassIsThere!='1'  && preassessStatus != 'TRUE' && surStatus != 'TRUE'  && hideShare == 1) //if there is change class option or post assessment
													    	
																html += "<a href='javascript:void(0);' class='actionLink vtip enroll-launch-full ' data=\"" + data1 +
																"\" name='takesurvey' onclick='" +postassPopup
																+Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a>"; 
															
															
															
															else
															
															
															
																enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
																"\" name='takesurvey' onclick='" +postassPopup
																+Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
															} 
																
																
														}
													}
											}else{ 
														
													if(preassessStatus == 'TRUE') {
														if(no_primary_html_button=='1'){ //make PRE ASSESSMENT button as primary 
															
															if(changeClassIsThere=='1' || assessStatus=='TRUE'){ //if there is change class option or post assessment
															    	var classLaunchNew='enroll-launch';
														    }else{
														    	var classLaunchNew='enroll-launch-full';
														    } 
															    
															html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" ' data=\"" + data1 +
																"\" name='takesurvey' onclick='" + preassPopup
																 +Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a>";
															no_primary_html_button='0';
														}else{
															enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
																"\" name='takesurvey' onclick='" + preassPopup
																 +Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
														}	 
													}
													
													if (assessStatus == 'TRUE') {
														
														if(no_primary_html_button=='1'){ //make POST ASSESSMENT button as primary 
															
															    if(changeClassIsThere=='1'){ //if there is change class option
															    	var classLaunchNew='enroll-launch';
															    }else{
															    	var classLaunchNew='enroll-launch-full';
															    }
															    
																html += "<a href='javascript:void(0);' class='actionLink vtip "+classLaunchNew+" '  data=\"" + data1 +
															"\" name='takesurvey' onclick='" +postassPopup
															    +titleRestrictionFadeoutImage(Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>",'exp-sp-mylearning-menulist') + "</a>";
																no_primary_html_button='0';
														}else{
																enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
															"\" name='takesurvey' onclick='" +postassPopup
															    +Drupal.t("Assessment")+ "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
														} 
												    }
												    
												    
											} 
						
										//}
										/*if ( surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) && baseType=="VC"){
											iltButtonAction = 'Survey';
											var surObj = '$("#block-take-survey").data("surveylearner")';
						
											enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='" + SMARTPORTAL.t("LBL203") + "' onclick='" +
											surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																					escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\");' >" +
												Drupal.t("LBL203") + "</a></li>";
						
											html += "<input type='button' class='actionLink enroll-launch' " +
														"data=\"" + data1 + "\" " +
														"value='" + Drupal.t("LBL203") + "' " +
															"name='" + SMARTPORTAL.t("Survey") + "' " +
																"id='survey" + dataId + "' " +
																	"onclick='" + surObj + ".callTakeSurveyToClass(" + classId + ",\"" + escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\");'" +
																		" >";
										}*/
										if ((isWBT || isVOD || ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv' && dedClass != 'Y' && ILTShareButton===null) { 
											if(hideShare == 0){
						                                     enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" data=\"' + data1 +
															'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
															'\',\'cre_sys_obt_cls\',\'enroll-lp-result-container\');" >' + Drupal.t("Share") + '</a></li>';
						                   }else {
					                          var linav = false;
											  enrollAction += '';
					                       }
										}
										
										if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw') && isSwitchAvail && ILTChangeButton===null){ //RegStatusCode == 'lrn_crs_cmp_ppm' ||
											enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" '+
												'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + userID + ',' + classId + ',' + courseId + ',' + dataId + ', \'Tp\');" >' +
												Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a></li>';
										}
									/*	if(!IsLaunchable){
											html += '<a id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="actionLink '+classlaunch+'"  onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
										}*/
										// Prepare the More menu items HTML in variable enrollAction
									   if(viewButton != 'viewDetails') {
											if(enrollAction!=''){
												var linav = true;
												enrollAction += '<li class="action-enable" style="display:none"><a id="lp-class-accodion-'+dataId+'"  href="javascript:void(0);" data="'+dataAccor+'" class="actionLink" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a></li>';
											}
											else{
												var linav = false;
												if(no_primary_html_button=='1'){ //Make View as primary button if not set
													//html += '<a id="lp-class-accodion-'+dataId+'"  href="javascript:void(0);" data="'+dataAccor+'" class="actionLink enroll-launch clsDisabledButton vtip" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
													//html += '<a id="lp-class-accodion-'+dataId+'"  href="javascript:void(0);" data="'+dataAccor+'" class="actionLink enroll-launch vtip">'+Drupal.t("LBL816")+'</a>';
													html += '<span id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="enroll-hide-launch" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'></span>';
													html += '<label class="enroll-launch-full"><a class="actionLink enroll-launch clsDisabledButton" href="javascript:void(0);">'+Drupal.t("LBL816")+'</a></label>';
													no_primary_html_button='0';
												//	alert('disabled view1');
												}else{
													enrollAction += '<a id="lp-class-accodion-'+dataId+'"  href="javascript:void(0);" data="'+dataAccor+'" class="actionLink" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'>'+Drupal.t("LBL816")+'</a>';
												}
												
											}
										}
										
										// Add html to show the More menu-button/menu (enabled or disabled as appropriate)
										if(enrollAction !='') {
											if(linav === true)
											html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + objEval + ".showMoreLPAction(this)' onblur='" + objEval + ".hideMoreLPAction(this)' ></a></div>";
											html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + enrollAction + '</ul></div>';
										}
										else{
											html += "<a class='enroll-launch-more-gray'></a>";
										}
					}
		}
		else if(overallStatus == 'lrn_tpm_ovr_ppv' || overallStatus == 'lrn_tpm_ovr_ppm' || overallStatus == 'lrn_tpm_ovr_wtl'){
			if((RegStatusCode == 'lrn_crs_reg_wtl') && isSwitchAvail){ // || RegStatusCode == 'lrn_crs_reg_ppm'
				html += "<label class='enroll-launch-full'><input type='button' class='actionLink enroll-launch' " +
				"value='" + Drupal.t('LBL943') + ".."+ "' title='" + Drupal.t('LBL943') + " " + Drupal.t('Class') +"' " +
					"name='" + Drupal.t('LBL943') + " " + Drupal.t('Class') + "' " +
						"id='changeclass" + dataId + "' " +
							'onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + userID + ',' + classId + ',' + courseId + ',' + dataId + ', \'Tp\');" ></label>';
			}else {
				html += '<label class="enroll-launch-full">'+'<span id="lp-class-accodion-'+dataId+'"  data="'+dataAccor+'" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'></span><a href="javascript:void(0);" class="actionLink enroll-launch clsDisabledButton" >'+Drupal.t("LBL816")+'</a></label>';
			}
		}else if(overallStatus == 'lrn_tpm_ovr_exp' || ((overallStatus == 'lrn_tpm_ovr_inc'|| overallStatus == 'lrn_tpm_ovr_cln') && RegStatusCode != 'lrn_crs_cmp_cmp')){
				html += '<span id="lp-class-accodion-'+dataId+'" href="javascript:void(0);" data="'+dataAccor+'" class="enroll-hide-launch" onclick=\''+objEval+'.addAccordionInClassView(this.className,"0 0","0 -61px","dt-child-row-En",'+objEval+'.getLPClassViewDetails,this,'+objEval+',true);\'></span>';
				html += '<label class="enroll-launch-full"><a class="actionLink enroll-launch clsDisabledButton" href="javascript:void(0);">'+Drupal.t("LBL816")+'</a></label>';
		}
		html += "</div>";
		return html;
		}catch(e){
			// to do
		}
	},

	launchLPMultiContent : function(enrollId,obj){
		try{
		var closeButt={};
		
		
	    $('#launch-lp-wizard').remove();
	  	html="";
		html+='<div id="launch-lp-wizard" class="launch-lp-wizard-content" style="display:none; padding: 0px;">';
	    html+= this.getLPLaunchDetails(enrollId);
	    html+='</div>';
	    $("body").append(html);
	    Drupal.attachBehaviors();
	    if(this.currTheme == 'expertusoneV2'){
	    	$('.enrollment-lplaunch-table tr:last-child').css('border-bottom','0px');
	    }
	    $('.enrollment-lplaunch-table tr:last').css('border-bottom','0px');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});

	    $("#launch-lp-wizard").dialog({
	        position:[(getWindowWidth()-700)/2,100],
	        bgiframe: true,
	        width:675,
	        resizable:false,
	        modal: true,
	        title: Drupal.t("Content"),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });


	    $("#launch-lp-wizard").show();
	    resetFadeOutByClass('#lplaunch-content-container #paintEnrollmentLPResults','item-title-code','line-item-container','MyPrograms');
	    
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
	  		changeDialogPopUI();
	  	}
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#launch-lp-wizard").remove();
	    });

		//SCrollbar for content dialogbox
		$("#learningplan-tab-inner").data("learningplan").scrollBarContentDialog();
		$('#paintEnrollmentLPResults tr:first-child td .title_close').trigger("click");
		}catch(e){
			// to do
		}
		vtip();
	},

	//SCROLLBAR IMPLEMENTATION FOR CONTENT SCREEN
	scrollBarContentDialog : function(){
		try{
		var contentDialogHeight = $("#launch-lp-wizard").height();
		if(contentDialogHeight > 550) {
			$(".launch-lp-wizard-content").css('height',550);
		} else if(contentDialogHeight <= 550) {
			$(".launch-lp-wizard-content").css('height','auto').css('min-height','70px');
		}
		}catch(e){
			// to do
		}
	},

	getLPSessionDetails : function(sessData,sessionId,baseType) {
		try{
		var data= eval($(sessData).attr("data"));
		var sessionDet = '';

		 $(".qtip-contentWrapper").css('border','0px none');  // border none added Fro this ticket #0046435
		if(!document.getElementById("session_det_lp"+sessionId)) {

			var sessionDet1 = '';
			$(data).each(function(){
if(baseType == "ILT")
				sessionDet1 += "<span class='qtip_session enroll-session-time time-zone-ilt'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> "+"  "+$(this).attr("usertimezonecode")+" "+$(this).attr("user_tzcode")+"</span>";
			else
			sessionDet1 += "<span class='qtip_session enroll-session-time'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> </span>";			});

			$('#session_det_popup_lp'+sessionId).qtip({
 content: '<div id="session_det_lp'+sessionId+'" class="qtip_session_top tooltiptop"></div><div id="session_det'+baseType+'"  class="qtip_session_mid tooltipmid">'+sessionDet1+'</div><div class="qtip_session_bottom tooltipbottom"></div>',			     show:{
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
				position: {
				    corner: {
				       target: 'bottomLeft',
				       tooltip: 'topLeft'
				    }
				},
				style: {
					width: 325,
	//				height: 150,
					background: 'none',
					color: '#333333'
				}
			});
		}
		}catch(e){
			// to do
		}
	},

	getLPClassViewDetails : function(catdiv,accordionLink,parentObj){
		try{
		return '';
		}catch(e){
			// to do
		}
	},

	getLPCourseViewDetails : function(catdiv,accordionLink,parentObj){
		try{
			return '';
		}catch(e){
				// to do
		}
	},

	getLPLaunchAccordion : function(catdiv,accordionLink,parentObj){
		try{
			return '';
		}catch(e){
				// to do
		}
	},

	addAccordionInClassView : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var currTrStr = $(obj).parents("tr").attr("id");
		currTrStr = currTrStr.split('-');
		var currTr = currTrStr[2];
		$("#"+currTr+"launch-subgrid").remove();
		if(!document.getElementById(currTr+"ClassSubGrid")) {
			//$("#compLpDetails"+currTr).show();// Completed tab css("display","block");
			$("#subgrid-class_menu_detail_"+currTr).after("<tr id='"+currTr+"ClassSubGrid' class='classSubGrid-LPDetailsTD-container' style='display: table-row;'></tr>");
			$("#subgrid-class_menu_detail_"+currTr).css('display', 'table-row');
			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
			if(this.currTheme != 'expertusoneV2') {
			$("#lp-class-accodion-"+currTr).removeClass("title_close");
			$("#lp-class-accodion-"+currTr).addClass("title_open");
			}
			// 0037346: Code Re-Factoring 3. My Programs > Enrolled > List sub titles : Jerks as classname of tr "ui-widget-content" is missing while expand.
			$("#lp-cl-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ClassSubGrid").slideDown(1000);
		} else {
			var clickStyle = $("#"+currTr+"ClassSubGrid").css("display");
			//$("#compLpDetails"+currTr).hide();// Completed tab css("display","none");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			//$("#compLpDetails"+currTr).show();// Completed tab css("display","block");
    			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
    			$("#"+currTr+"ClassSubGrid").slideDown(1000);
    			if(this.currTheme != 'expertusoneV2') {
    			$("#lp-class-accodion-"+currTr).removeClass("title_close");
				$("#lp-class-accodion-"+currTr).addClass("title_open");
    			}
				$("#lp-cl-"+currTr).removeClass("ui-widget-content");
    		} else {
    			$("#"+currTr+"ClassSubGrid").hide();//css("display","none");
    			$("#"+currTr+"ClassSubGrid").slideUp(1000);
    			if(this.currTheme != 'expertusoneV2') {
    			$("#lp-class-accodion-"+currTr).removeClass("title_open");
				$("#lp-class-accodion-"+currTr).addClass("title_close");
    			}
				$("#lp-cl-"+currTr).removeClass("ui-widget-content");
				$("#lp-cl-"+currTr).addClass("ui-widget-content");
    		}
		}

		var data = eval($("#lp-class-accodion-"+currTr).attr("data"));
		var classDetSec = this.paintDetailsLPClassSection(data);
		$("#"+currTr+"ClassSubGrid").html(classDetSec);
		$('.enroll-show-moreclass>tbody>tr:last').find('#'+currTr+'LPDetailsMainDiv').css('border','0px');
		//72790: More and description V should expand all the details instead of only doing what we do today.
		//$('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
		
		}catch(e){
			// to do
		}
		vtip();
	},

	paintDetailsLPClassSection : function(data){
		try{
		var ostr = '';
		var dataInfo = data;
		var shortDescription = unescape(data.shortDescription);
		var classId  = data.classId;
		var courseId = data.courseId;
		var enrollId = data.RegId;
		var clsTitle = data.title;
		var SessStartDate = data.SessStartDate;
		var SessStartDay = data.SessStartDay;
		var SessEndDate = data.SessEndDate;
		var startDateFormat = data.startDateFormat;
		var baseType = data.BaseType;
		var regStatusCode = data.regStatusCode;
		var show_events   = data.show_events;
		var dayRemainVal = data.dayRemainVal;
		var classCode = unescape(data.classCode);
		var courseTitle   = unescape(data.courseTitle);
		var courseCode    = unescape(data.courseCode);
		if(data.moduleTitle)
			var moduleTitle   = unescape(data.moduleTitle);
		var moduleCode    = unescape(data.moduleCode);
		var updateDate = data.updateDate;
		var compDate = data.compDate;
		var regDate = data.regDate;
		var deliverytype = data.deliverytype;
		ostr += '<td></td><td class="LPDetailsTD"><div id="'+enrollId+'LPDetailsMainDiv" class="enroll-accordian-div">';
		/*if(document.getElementById("session_det_popup_lp"+classId)) {
			var sessionDet = eval($("#session_det_popup_lp"+classId).attr('data'));
			ostr += '<div class="enroll-session-details session-row"><span class="session-label">'+Drupal.t("LBL670")+' :</span><div class="session-desc">';
			var inc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay

				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead =titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrmyprogram-session-title',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrmyprogram-session-title',20);
				//ostr += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;")+'">'+sesionsHead+'</div>';
				//ostr += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")).replace(/"/g, '&quot;') : "&nbsp;")+'">'+unescape(sesionsHead)+'</div>';
				ostr += '<div class="sessionDet"><div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
				ostr += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
				//ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
				ostr += '<div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")).replace(/"/g, '&quot;') : "&nbsp;")+'">'+unescape(sesionsHead)+'</div>';
				inc++;
          });
			ostr += '</div></div>';
			if (baseType == 'ILT') {
			var sesinc = 1;
			var LocationName = '';
			LocationName 	= unescape(dataInfo.LocationName);
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					ostr += '<div class="sessAddDet location-row line-item-container"><span class="location-label container">'+Drupal.t("Location")+' :</span>';
					if(LocationName!='null' && LocationName!=''){
						LocationName =   LocationName + ",";
						if(LocationName.length > 105)
							loctitle = titleRestrictionFadeoutImage(unescape(LocationName),'exp-sp-instructor-desk-loc-head',110);
						else
							loctitle = unescape(LocationName);
						ostr += '<div class="location-desc"><span class="name-add vtip" title =" '+unescape(LocationName)+' ">'+loctitle+'</span>';
						}
					if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='')
						ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
					if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='')
						ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
					if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
						ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
					if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
						ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
					if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+'</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					ostr += '</div>';
					}
				});
			}
		}*/
		
		var dataDesc = "data={'ClassId':'"+classId+"','ClassDesc':'"+shortDescription+"'}";
		var data1 = "data={'classId':'"+classId+"','courseId':'"+courseId+"','regStatusCode':'"+regStatusCode+"'}";
		ostr += '<div class="course-title-code main-item-container">';
		//Pipe Sysmbol use this variable 'pipe'
		var pipe = '<span class="enroll-pipeline">|</span>';

		var statusDate = '';
		var statusName = '';

		if(regStatusCode == 'lrn_crs_reg_can' || regStatusCode == 'lrn_crs_cmp_nsw') {
			statusDate = updateDate;
			statusName = Drupal.t('LBL026');
		}
		else if(regStatusCode == 'lrn_crs_cmp_cmp') {
			statusDate = compDate;
			statusName = Drupal.t('LBL027');
		}else if(regStatusCode == 'lrn_crs_cmp_inc'){
			statusDate = compDate;
			statusName = Drupal.t('LBL1193');
		}
		else{
			statusDate = regDate;
			statusName = Drupal.t('LBL704');
		}
//		if(Drupal.settings.mylearning_right===false)
//			html += pipe+'<span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',50)+'</span>';
//		else
//			html += pipe+'<span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',10)+'</span>';
		statusNameDate = '<span class="mypgm-group-title container">'+statusName +' : </span>' + '<span class="enr-status-cls">' + statusDate+ '</span>';
		var titStatusNameDate = statusName+' : '+ statusDate;
		ostr += '<div class="traning-plan-cls-titlist line-item-container vtip" title="'+titStatusNameDate+'">'+statusNameDate+"</div>";
		if(Drupal.settings.mylearning_right===false){
			/*Viswanathan uncommented for #0073514*/
			if(classCode)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"><span class="mypgm-group-title container">'+Drupal.t('LBL263')+' : </span><span class="vtip mypgm-group-title-val" title="'+classCode+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',50)+'</span></div>';/*Viswanathan uncommented for #0073514*/
			if(courseTitle)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"> <span class="mypgm-group-title container">'+Drupal.t('Course')+' '+Drupal.t('LBL083')+' : </span><span class="vtip mypgm-group-title-val" title="'+courseTitle.replace(/\\/g, '')+'">'+titleRestrictionFadeoutImage(courseTitle.replace(/\\/g, ''),'exp-sp-lnrmyprogram-traning-plan-cls-titlist',120)+"</span></div>";
			if(courseCode)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"> <span class="mypgm-group-title container">'+Drupal.t('Course')+' '+Drupal.t('LBL096')+' : </span><span class="vtip mypgm-group-title-val" title="'+courseCode+'">'+titleRestrictionFadeoutImage(courseCode,'exp-sp-lnrmyprogram-traning-plan-cls-titlist',120)+"</span></div>";
			if(moduleTitle!=undefined && moduleTitle!=null)
				ostr +='<div class="traning-plan-cls-grp-title"><span class="mypgm-group-title container">'+Drupal.t('Group')+' : </span><span class="vtip mypgm-group-title-val" title="'+moduleTitle.replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(moduleTitle,'exp-sp-lnrmyprogram-traning-plan-cls-grp-title',100)+"</span></div>";
		}else{
			if(classCode)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"><span class="mypgm-group-title container">'+Drupal.t('LBL263')+' : </span><span class="vtip mypgm-group-title-val" title="'+classCode+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-lnrmyprogram-class-code',10)+'</span></div>';
			if(courseTitle)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"> <span class="mypgm-group-title container">'+Drupal.t('Course')+' '+Drupal.t('LBL083')+' : </span><span class="vtip mypgm-group-title-val" title="'+courseTitle.replace(/\\/g, '')+'">'+titleRestrictionFadeoutImage(courseTitle.replace(/\\/g, ''),'exp-sp-lnrmyprogram-traning-plan-cls-titlist',90)+"</span></div>";
			if(courseCode)
				ostr += '<div class="traning-plan-cls-titlist line-item-container"> <span class="mypgm-group-title container">'+Drupal.t('Course')+' '+Drupal.t('LBL096')+' : </span><span class="vtip mypgm-group-title-val" title="'+courseCode+'">'+titleRestrictionFadeoutImage(courseCode,'exp-sp-lnrmyprogram-traning-plan-cls-titlist',40)+"</span></div>";
			if(moduleTitle!=undefined && moduleTitle!=null)
				ostr +='<div class="traning-plan-cls-grp-title"><span class="mypgm-group-title container">'+Drupal.t('Group')+' : </span><span class="vtip mypgm-group-title-val" title="'+moduleTitle.replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(moduleTitle,'exp-sp-lnrmyprogram-traning-plan-cls-grp-title',40)+"</span></div>";
		}
		/*ostr += ' </div>';*/
		/*if(dayRemainVal != ''){
			ostr += '<div class="days-remain"><span>'+dayRemainVal+'</span></div>';
	    }*/

		if(document.getElementById("session_det_popup_lp"+classId)) {
			var sessionDet = eval($("#session_det_popup_lp"+classId).attr('data'));
			if (baseType == 'ILT') {
				var sesinc = 1;
				var LocationName = '';
				LocationName 	= unescape(dataInfo.LocationName);
				$(sessionDet).each(function(){ //sessionDate sessionDay
					if(sesinc==1)
						{
						sesinc++;
						ostr += '<div class="sessAddDet location-row line-item-container"><span class="location-label container">'+Drupal.t("Location")+' :</span>';
						
						if(LocationName!='null' && LocationName!=''){
						LocationName =   LocationName + ",";
						if(LocationName.length > 80)
							loctitle = titleRestrictionFadeoutImage(unescape(LocationName),'exp-sp-instructor-desk-loc-head',110);
						else
							loctitle = unescape(LocationName);
						ostr += '<div class="location-desc"><span class="name-add vtip" title =" '+unescape(LocationName)+' ">'+loctitle+'</span>';
						}
						
						if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='')
							ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
						if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='')
							ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
						if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
							ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
						if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
							ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
						if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
							ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+'</span>';
						if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
							ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
						ostr += '</div>';
						}
					});
			}
			//var sessionDet = eval($("#session_det_popup_lp"+classId).attr('data'));
			ostr += ' </div>';
			ostr += '<div class="enroll-session-details session-row main-item-container"> <span class="session-label session-container">'+Drupal.t("LBL277")+' :</span><div class="session-desc">';
			var inc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay
			var tz_code = $(this).attr("tz_code") ;
             	var locationsess = '<div class="sessDate">'+$(this).attr("sessionSDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayFormloc")+"</span>"+' to '+$(this).attr("sessionEDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayFormloc")+'</span>'+$(this).attr("session_code")+' '+tz_code+'</div>';

				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				var sesionInsName = ($(this).attr("sInsName") != '') ? unescape($(this).attr("sInsName")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead =titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrmyprogram-session-title',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrmyprogram-session-title',20);
				    var sesInstName = titleRestrictionFadeoutImage(sesionInsName,'exp-sp-lnrenrollment-instructor-names');
				ostr += '<div class="sessionDet">';	
				ostr += '<div class="cls-sessionTitle-container line-item-container"><div class="lbl-sessionTitle sessionDetlbl container">'+Drupal.t("LBL107")+' :'+'</div> <div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;")+'">'+sesionsHead+'</div></div>';
				ostr += '<div class="cls-sessDate-container line-item-container"><div class="lbl-sessionDate sessionDetlbl container">'+Drupal.t("LBL042")+' :'+'</div> <div class="session-day-date-container"><div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
						if(baseType == "ILT")
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>"+' '+$(this).attr("usertimezonecode")+' '+$(this).attr("user_tzcode")+"</div>";
		        else
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div>";
			if(baseType == "ILT" && $(this).attr("sessiontimezone")!= $(this).attr("usertimezone")){
		       var qtipId = enrollId+"-tpdet-"+$(this).attr("sessionId");
				ostr += qtip_popup_paint(qtipId,locationsess,'',1); 
				//ostr += '</div>';
			}
			ostr += '</div></div>';
			ostr += '<div class="cls-sessionInstructor-container line-item-container"><div class="lbl-sessionInstructor sessionDetlbl container">'+Drupal.t("Instructor")+' :'+'</div> <div class="vtip sessionInstructor-val" title="'+(($(this).attr("sInsName") != '') ? htmlEntities(unescape($(this).attr("sInsName"))) : "")+'">'+sesInstName+'</div></div>';
			ostr += '</div>';
				inc++;
          });
			ostr += '</div></div>';
			/*if (baseType == 'ILT') {
			var sesinc = 1;
			var LocationName = '';
			LocationName 	= unescape(dataInfo.LocationName);
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					ostr += '<div class="sessAddDet location-row line-item-container"><span class="location-label container">'+Drupal.t("Location")+' :</span>';
					if(LocationName!='null' && LocationName!='')
						ostr += '<div class="location-desc"><span class="name-add">'+unescape(LocationName)+',</span>';
					if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='')
						ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
					if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='')
						ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
					if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
						ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
					if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
						ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
					if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+'</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					ostr += '</div></div>';
					}
				});
			}*/
		}
		var endTime   = '-';
		var startTime = '-';
		if(baseType != "ILT"){
			startDateFormat = '-';
			SessStartDay	= '-';
		}
		if((baseType == "ILT") && (SessEndDate != null || SessEndDate !='')) {
			var endFormatTime = SessEndDate.split(" ");
			endTime = endFormatTime[1]+' '+endFormatTime[2];
		}
		if((baseType == "ILT") && (SessStartDate != null || SessStartDate !='')) {
			var startFormatTime = SessStartDate.split(" ");
			startTime = startFormatTime[1]+' '+startFormatTime[2];
		}
		var attachData = eval($("#class_attachment_lp_"+enrollId).attr("data"));
			if(attachData.length > 0) {
				ostr += '<div class="enroll-attach attach-row-tpClass"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+':'+'</span><div class="attach-info">'+ Drupal .t("LBL232") +'</div></div>';
				ostr += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
				var len = attachData.length;
				var inc = 1;
					$(attachData).each(function(){
							/*--Issue fix for the ticket - 32781 --*/
							ostr += "<li>"+"<a href='javascript:void(0)' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+titleRestrictionFadeoutImage(unescape(saniztizeJSData($(this).attr('Title'))),'exp-sp-lnrmyprogram-courseclass-attachment-name')+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
							inc++;
					});
				ostr += "</ul></div></div>";
			}

		ostr += '</div></td><td></td>';
		resetFadeOutByClass('.enroll-show-moreclass','item-title-code','line-item-container','MyPrograms');
		return ostr;
		}catch(e){
			// to do
		}
	},
	addAccordionLaunchView : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var currTrStr = $(obj).parent().parent().parent().attr("id");
		currTrStr = currTrStr.split('-');
		var currTr = currTrStr[3];
		if(!document.getElementById(currTr+"LPLessonSubGrid")) {
			$("#lp-lesson-launch-"+currTr).after("<tr id='"+currTr+"LPLessonSubGrid'></tr>");
			$("#"+currTr+"LPLessonSubGrid").show();//css("display","block");
			$("#lp-lesson-accodion-"+currTr).removeClass("title_close");
			$("#lp-lesson-accodion-"+currTr).addClass("title_open");
			$("#lp-lesson-launch-"+currTr).css('border-bottom','none 0px');
			$("#"+currTr+"LPLessonSubGrid").slideDown(1000);
			var data = eval($("#lp-lesson-accodion-"+currTr).attr("data"));
			var lessonDetSec = this.paintLPLessonSection(data,currTr);
			$("#"+currTr+"LPLessonSubGrid").html(lessonDetSec);
		} else {
			var clickStyle = $("#"+currTr+"LPLessonSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"LPLessonSubGrid").show();//css("display","block");
    			$("#"+currTr+"LPLessonSubGrid").slideDown(1000);
    			$("#lp-lesson-accodion-"+currTr).removeClass("title_close");
				$("#lp-lesson-accodion-"+currTr).addClass("title_open");
				$("#lp-lesson-launch-"+currTr).css('border-bottom','none 0px');
    		} else {
    			$("#"+currTr+"LPLessonSubGrid").hide();//css("display","none");
    			$("#"+currTr+"LPLessonSubGrid").slideUp(1000);
    			$("#lp-lesson-accodion-"+currTr).removeClass("title_open");
				$("#lp-lesson-accodion-"+currTr).addClass("title_close");
				$("#lp-lesson-launch-"+currTr).removeClass("ui-widget-content");
				$("#lp-lesson-launch-"+currTr).css('border-bottom','1px solid #cccccc');
    		}
		}

		$('.launch-lesson-view').each(function(){
		  $(this).parents("tr").children("td").css({'padding-top' : '0px'});
		  $('.enrollment-lplaunch-table').find('tbody tr:visible:last').last().css('border','0px');
          $('.enrollment-lplaunch-table').find("tbody tr td tr:visible:last-child").css('border','0px');
		});
		resetFadeOutByClass('.launch-lesson-view','item-title-code','line-item-container','MyPrograms');
		vtip();
		}catch(e){
			// to do
		}
	},
	paintLPLessonSection : function(data,paramContId){
		try{
		var data = eval(data);
		var lesCnt = 0;
		var obj = this.widgetObj;
		var ostr = '';
		ostr += '<td colspan="2"><div class="enroll-accordian-div">';
		ostr += '<table class="launch-lesson-view" cellpadding="0" cellspacing="0" width="100%" border="0">';
		$(data).each(function(){
			$(this).each(function(){
			  var contentId = $(this).attr('ContentId');
			  var VersionId = $(this).attr('VersionId');
			  if(paramContId == contentId){
				  lesCnt++;
				  var params = "";
				  var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				  var contValidateMsg = $(this).attr('contValidateMsg');
				  var contentType = $(this).attr('launchType');
				  var lessonId = $(this).attr('Id');
				  params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"','courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',}";
				  callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
          var lessonLocation = $(this).attr('LessonLocation');
          var launchData = $(this).attr('launch_data');
          var suspendData = $(this).attr('suspend_data');
          var exit = $(this).attr('exit');
          lessonLocation = lessonLocation == null || lessonLocation == 'null'? '' : lessonLocation;
          launchData = launchData == null || launchData == 'null'? '' : launchData;
          suspendData = suspendData == null || suspendData == 'null'? '' : suspendData;
          exit = exit == null || exit == 'null'? '' : exit;
          var relaunchInfo ='';
          relaunchInfo += "{";
          relaunchInfo += "'lessonlocation':'"+lessonLocation+"'";
          relaunchInfo += ",'launchData':'"+launchData+"'";
          relaunchInfo += ",'suspendData':'"+suspendData+"'";
          relaunchInfo += ",'exit':'"+exit+"'";
          relaunchInfo += "}";
  		  var relaunchData = "["+relaunchInfo+"]";
  		  var pipe = '<span class="enroll-pipeline">|</span>';
  		  var contScore = ($(this).attr('LesScore') != '') ? $(this).attr('LesScore') : '';
  		  var contentQuizStatus = ($(this).attr('contentQuizStatus') != '') ? $(this).attr('contentQuizStatus') : '';
  		  if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
			  contentQuizStatus = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-myprogram-multilessonscore')+'</span></div>';
		  }
  		  if(contScore == '') {
      	    var lessonScore = '';
          } else {
      	    var lessonScore = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668') + ' : ' + contScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') + ' : ' + contScore,'exp-sp-myprogram-multilessonscore')+'</span></div>';;
          }
				  ostr += '<tr class="enroll-lesson-section"><td class="enroll-launch-column1">';
				  ostr += '<div class="enroll-course-title">';
				  ostr += '<span id="titleAccEn_'+lessonId+'" class="item-title" ><span class="enroll-class-title vtip" title="'+Drupal.t('LBL854')+' '+lesCnt+' : '+unescape($(this).attr('Title')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				  ostr += '<span class="clsLessLabel">'+Drupal.t('LBL854')+' '+lesCnt+' :  </span><span class="clsLessTitle">' +titleRestrictionFadeoutImage(decodeURIComponent($(this).attr('Title')),'exp-sp-lnrmyprogram-enroll-class-title')+'</span>';
				  ostr += '</span></span>';
				  ostr += '</div>';
				  ostr += '<div class="enroll-class-title-info">';
				  ostr += '<div class="item-title-code">';
				  ostr += '<div class="line-item-container float-left"><span class="vtip" title="'+Drupal.t('LBL036')+' : '+Drupal.t('LBL854')+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL036')+' : '+Drupal.t('LBL854'),'exp-sp-myprogram-multilessType')+'</span></div>';
				  var lessonStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('Status'));
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_status"><div class = "line-item-container float-left">'+pipe+'<span class="vtip" title="'+Drupal.t('LBL102')+' : ' + Drupal.t(lessonStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+' : ' + Drupal.t(lessonStatus),'exp-sp-myprogram-multilessonstatus')+'</span></div></span>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonScore">'+ lessonScore +'</span>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonQuizStatus">'+ contentQuizStatus +'</span>';
				  ostr += ' </div>';
				  ostr += '</div>';
				  ostr += '</td>';
				  ostr += '<td class="enroll-launch-column2">';
				  ostr +=   '<div class="enroll-main-list">';
				  ostr +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
				                  ' class="'+((IsLaunchable) ? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
				  ostr +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
				                  ' class="' + ((IsLaunchable)? 'actionLink' : '') + ' enroll-launch"' +
				                    ' data-relaunch="' + relaunchData + '"' +
				                      ' value="' + Drupal.t('LBL199') + '" ' + ((IsLaunchable)? callLaunchUrlFn : '') + '>';
				  ostr +=     '</label>';
				  ostr +=   '</div>';
				  ostr += '</td></tr>';
			  }
			});
		});
		ostr += '</table></div"></td>';
		return ostr;
		}catch(e){
			// to do
		}
	},

	getLPLaunchDetails_ : function(data) {
		try{
		var html = '<td colspan="2"><table cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-class-table">';
		html += '<tr><th class="enrollment-class-name">'+SMARTPORTAL.t('Title')+'</th><th class="enrollment-class-name">'+SMARTPORTAL.t('Action')+'</th></tr>';

		$(data).each(function(){
			$(this).each(function(){
				var params 		 	= "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"','courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"','LaunchFrom':'LP','AICC_SID':'"+$(this).attr('AICC_SID')+"'}";
				var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				var contValidateMsg = $(this).attr('contValidateMsg');
				callLaunchUrlFn = "onclick=\"$('#learningplan-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
				html += '<tr><td>'+decodeURIComponent($(this).attr('Title'))+'</td><td><div class"enroll-main-list"><input type="button" class="actionLink '+((IsLaunchable) ? 'enroll-launch' : 'enroll-launch-gray')+'"  value="Launch" '+((IsLaunchable) ? callLaunchUrlFn : '')+'>';
				html += (contValidateMsg != '') ? '<span id="'+$(this).attr('Id')+"_"+$(this).attr('enrollId')+'attempts_left" class="lp-launch-content-status">'+contValidateMsg+'</span>' : '';
				html += '</div></td></tr>';
			});
		});

		html +='</table></td>';
		return html;
		}catch(e){
			// to do
		}
	},

	getLPLaunchDetails : function(enrollId) {
		try{
			// called when launch is clicked for multi-content
		var passdata = $('#launch-lp-class-'+enrollId).attr('data');
		var data = eval($('#launch-lp-class-'+enrollId).attr('data'));
		var html = '<div id="lplaunch-content-container">';
		html += '<table id="paintEnrollmentLPResults" cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-lplaunch-table">';
		var lesCnt = 0;
		var obj = this.widgetObj;
		var widgetObject = this;
		var contentArr = new Array();
		$(data).each(function(){
			$(this).each(function(){
				var params = "";
				var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				var contValidateMsg = $(this).attr('contValidateMsg');
				var contentType = $(this).attr('launchType');
				var contentId = $(this).attr('ContentId');
				var ValidityDays = $(this).attr('ValidityDays');
				var lcnt = $(this).attr('Lessoncnt');
				var pipe = '<span class="enroll-pipeline">|</span>';
				var AvailableScore = ($(this).attr('ContScore') == '') ? '' : $(this).attr('ContScore');
				var percentCompleted = '';	// will be used only for VOD contents
				var VersionNo = '';
				  var score ='';
				  if(AvailableScore !=null && AvailableScore !=undefined && AvailableScore != '' && AvailableScore != '0.00') {
						var score = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+' : ' + AvailableScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668')+' : ' + AvailableScore,'exp-sp-myprogram-score')+'</span></div>';
				  }
		        var VersionNo = ($(this).attr('VersionNo') == 'notset')? '' : $(this).attr('VersionNo');
		        if (VersionNo != '') {
		          VersionNo = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL1123')+' : ' + VersionNo+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL1123')+' : ' + VersionNo,'exp-sp-myprogram-versionNo')+'</span></div>';
		        }
		        
		        var attemptsLeft = ($(this).attr('AttemptLeft') == 'notset')? '' : $(this).attr('AttemptLeft');
		        if (attemptsLeft !== '') {
		          attemptsLeft = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL202')+' : ' + attemptsLeft+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL202')+' : ' + attemptsLeft,'exp-sp-myprogram-attemptsleft')+'</span></div>';
		        }

				  var validityDays = ($(this).attr('ValidityDays') == '')? '' : $(this).attr('ValidityDays');
				  var diffDays =  $(this).attr('remDays');

				  if (validityDays !== '') {
//					  var remValidityDays = validityDays - diffDays;
					  var remValidityDays = ($(this).attr('daysLeft') !== undefined && $(this).attr('daysLeft') != null) ? $(this).attr('daysLeft') : '';
					  if(remValidityDays <= 0){
					  var daysLBL = Drupal.t("Expired");//Expired
					  remValidityDays = '';// To avoid result as Validity: 0 Expired
					  }else if(remValidityDays == 1){
					  var daysLBL = Drupal.t("LBL910");//day
					  }else if(remValidityDays > 1){
					  var daysLBL = Drupal.t("LBL605");//days
					  }
					validityDays = '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-myprogram-validitydays')+'</span></div>';;
				  }

				if(contentType == 'VOD') {
					var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != '') ? JSON.parse(unescape(unescape($(this).attr('suspend_data')))) : null;
					//console.log(suspend_data);
					var progress = suspend_data != null ? suspend_data['progress'] : 0;
					progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
					percentCompleted =  '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+progress+'% '+Drupal.t('Completed')+'">'+titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-myprogram-compPercent')+'</span></div>';
				}
				if($.inArray(contentId,contentArr) == -1){
					contentArr.push(contentId);
					var launchInfo = data;
					var contentQuizStatus = widgetObject.getConsolidatedQuizStatusLP(contentId, launchInfo);
					var contentQuizStatusString = '';
			        if (contentQuizStatus !== '') {
			        	contentQuizStatusString =  '<div class = "line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL1284')+' : ' + Drupal.t(contentQuizStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL1284')+' : ' + Drupal.t(contentQuizStatus),'exp-sp-myprogram-quizstatus')+'</span></div>';;
			        }
					var lessonId = $(this).attr('Id');
					var Dstatus = Drupal.t('In progress');
					var Dstatus1 = Drupal.t('Incomplete');
					var Dstatus2 = Drupal.t('Completed');
					var contentStatus = ($(this).attr('ContentStatus') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('ContentStatus'));
					if(contentType == 'VOD') {
					  contentStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : $(this).attr('Status');
					}
					//contentStatus =  '<div class = "line-item-container float-left" <span class="vtip" title="'+Drupal.t('LBL102')+' : ' + contentStatus+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+' : ' + contentStatus,'exp-sp-myprogram-attemptsleft')+'</span></div>';
					html += '<tr id="lp-lesson-launch-' + contentId + '">';
					html += '<td class="enroll-launch-column1">';
					html += '<div class="enroll-course-title">';
					html += '<span id="lp-lesson_title_' + contentId + '"></span>';
					if (lcnt > 1) {
						html += '<a id="lp-lesson-accodion-'+contentId+'" href="javascript:void(0);" data="'+passdata+'" class="title_close" onclick=\''+obj+'.addAccordionLaunchView(this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getLPLaunchAccordion,this,'+obj+',true);\'>&nbsp;</a>';
					}
					html += '<span id="titleAccEn_'+contentId+'" class="item-title" >';
					html += '<span class="enroll-class-title vtip"' +
					              ' title="'+unescape($(this).attr('ContentTitle')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
					html +=   titleRestrictionFadeoutImage(unescape(decodeURIComponent($(this).attr('ContentTitle'))),'exp-sp-lnrmyprogram-content-title')+'</span><br>';
					html +=   '<div class="item-title-code">';
					html +=  '<span id="lesson_status_'+contentId+'"><div class="line-item-container float-left" ><span title="'+Drupal.t('LBL102')+': '+contentStatus+'" class="vtip">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+': '+ contentStatus,'exp-sp-myprogram-contentstatus',200)+'</span></div>' + score + contentQuizStatusString + attemptsLeft + validityDays + percentCompleted + VersionNo+'</span></div></span>';	
					html +=           '</div>';
					html +=         '</span>';
					html +=       '</span>';
					html +=     '</div>';
					html +=   '</td>';
					html +=   '<td class="enroll-launch-column2">';
					if (lcnt == 1 || contentType == 'VOD') {
			            var IsLaunchable  = eval($(this).attr('IsLaunchable'));
			            var contValidateMsg = $(this).attr('contValidateMsg');
			            var contentType = $(this).attr('launchType');
			            var lessonId = $(this).attr('Id');
			            if (contentType == 'VOD'){
				        	var title = $(this).attr('Title');
						    title = encodeURIComponent(title.replace(/\//g, '()|()'));
							//console.log($(this));
							//console.log($(this).attr('suspend_data'));
							//console.log(unescape($(this).attr('suspend_data')));
							// console.log(JSON.parse(unescape($(this).attr('suspend_data'))));
							// var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != "") ? JSON.parse(unescape(unescape($(this).attr('suspend_data')))) : null;
							// var progress = suspend_data != null ? suspend_data['progress'] : 0;
							//console.log(progress);
						    callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
						    callLaunchUrlFn += title + "/";
						    callLaunchUrlFn += escape($(this).attr('contentSubTypeCode')) + "/";
						    callLaunchUrlFn += escape($(this).attr('url1').replace(/\//g, '()|()')) + '/';
						    callLaunchUrlFn += "LP" + "/";
						    callLaunchUrlFn += $(this).attr('courseId') + "/";
						    callLaunchUrlFn += $(this).attr('classId') + "/";
						    callLaunchUrlFn += $(this).attr('Id') + "/";
						    callLaunchUrlFn += $(this).attr('VersionId') + "/";
						    callLaunchUrlFn += $(this).attr('enrollId') + "/";
						    callLaunchUrlFn += escape($(this).attr('StatusCode')) + "/";
						    // callLaunchUrlFn += progress + "/";
						    callLaunchUrlFn += $(this).attr('suspend_data')+ "\"";
			            }
			            else{
			            	var params = "";
			            	params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			                      "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',"+
			                      "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
			            	callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
			            }
			            var lessonLocation = $(this).attr('LessonLocation');
			            var launchData = $(this).attr('launch_data');
			            var suspendData = $(this).attr('suspend_data');
			            var exit = $(this).attr('exit');
			            lessonLocation = lessonLocation == null || lessonLocation == 'null'? '' : lessonLocation;
			            launchData = launchData == null || launchData == 'null'? '' : launchData;
			            suspendData = suspendData == null || suspendData == 'null'? '' : suspendData;
			            exit = exit == null || exit == 'null'? '' : exit;
			            var relaunchInfo ='';
			            relaunchInfo += "{";
			            relaunchInfo += "'lessonlocation':'"+lessonLocation+"'";
			            relaunchInfo += ",'launchData':'"+launchData+"'";
			            relaunchInfo += ",'suspendData':'"+suspendData+"'";
			            relaunchInfo += ",'exit':'"+exit+"'";
			            relaunchInfo += "}";
			    		var relaunchData = "["+relaunchInfo+"]";
			            var ajaxCss = ((contentType == 'VOD') && (IsLaunchable))? ' use-ajax ctools-modal-ctools-video-style ' : ' ';
					    html +=   '<div class="enroll-main-list">';
					    html +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
	                            ' class="'+((IsLaunchable) ? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
					   	html +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
	                             ' class="' + ((IsLaunchable) ? 'actionLink' : '') + ajaxCss + ' enroll-launch"' +
	                               ' data-relaunch="' + relaunchData + '"' +
	                                 ' value="' + Drupal.t('LBL199') + '" '+((IsLaunchable)? callLaunchUrlFn : '') + '>';
					    html +=     '</label>';
					    html +=   '</div>';
					  }
					  html += '</td></tr>';
				}
			});
		});

	   html +='</table>';
	   html +='</div>';
       return html;
		}catch(e){
			// to do
			//console.log(e);
		}
	},

	getLPDropPolicy:function(data)
	{
		try{
		$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
		var data= eval($(data).attr("data"));
		var isCommerceEnabled='';
	    $('#lp-dropmsg-wizard').remove();
	    var html = '';
	    var MasterMandatory=0;
	    if(data.MasterMandatory == 1){
	    	MasterMandatory =1;
	    }
	    var manByRole=0;
	    if(data.mro =='cre_sys_inv_man' && (data.is_exempted != 1 )){
	    	manByRole =1;
	    }

	    if(!MasterMandatory && !manByRole){
		   if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != ""){
			   isCommerceEnabled = "1";
		   }
	    }
	    html+='<div id="lp-dropmsg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
	    html+= '<td align="center" id="dropmsg-lp-content"><span>'+Drupal.t("LBL416")+'</span></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};

	    $("#lp-dropmsg-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        autoOpen: false,
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        // title:titleRestricted(unescape(data.TpTitle),30), // Commented for this Ticket #0042320
	        title:Drupal.t("LBL109"),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });


	    $("#lp-dropmsg-wizard").show();
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#lp-dropmsg-wizard").remove();
	    });
		$("#learningplan-tab-inner").data("learningplan").getLPDropPolicyCall(data.RegId,'tp',data.PrgId,isCommerceEnabled,MasterMandatory,data.TpTitle,manByRole);
		}catch(e){
			// to do
		}
	},

	getLPDropPolicyCall : function(enrollId,baseType,prgId,isCommerceEnabled,assMand,clstitle,manByRole)
	{
		try{
		var userId = this.lpUserId;
		var url = this.constructUrl("ajax/get-droppolicy/" + userId +  "/" + baseType + "/" + enrollId+"/"+prgId+"/"+isCommerceEnabled+"/"+assMand+"/"+manByRole);
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				var closeButt={};
				if(drop_policy_info.next_action=="drop")
				{
					$("#dropmsg-lp-content").html('<span>'+Drupal.t("MSG263")+'</span><br /><i>"'+unescape(clstitle)+'"</i>');
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropmsg-lp-content').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learningplan-tab-inner").data("learningplan").dropLPCall(enrollId,baseType,prgId,"",isCommerceEnabled,assMand); $("#dropmsg-lp-content").html(Drupal.t("MSG424"));};

				}else if(drop_policy_info.next_action=="showmsg")
				{
					$("#dropmsg-lp-content").html('<span>'+SMARTPORTAL.t(drop_policy_info.msg)+"</span>");

					var html="";
					html+='<span><b>'+Drupal.t("LBL083")+' : </b>'+unescape(clstitle)+"</span>";//<b>Title: </b>
					html+="<br/><br/>";
					html+="<span>"+SMARTPORTAL.t(drop_policy_info.msg)+'</span>';
					$("#dropmsg-lp-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropmsg-lp-content').remove();};
				}else if(drop_policy_info.next_action=="showdroppolicy")
				{
					var html="";
					if(drop_policy_info.deducted_amount == 0 && drop_policy_info.price > 0){
						html+="<span>"+Drupal.t("MSG264")+"<br/> </span>";
						html+="<br/><br/>";
						html+="<span>"+Drupal.t("MSG266")+'</span>';
					}else if (drop_policy_info.price <= 0 && drop_policy_info.refund_amt == 0) {
						html+='<span>'+Drupal.t("MSG263")+'</span><br /><i>"'+unescape(clstitle)+'"</i>';
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
					  html+='<span>'+Drupal.t('MSG402')+" "+drop_policy_info.currency_type+' '+drop_policy_info.deducted_amount+" "+Drupal.t('MSG401')+"<br/> </span>";
					  html+="<br/>";
					  html+='<span class="dropmsgTitle">'+Drupal.t('LBL1165')+': </span>'+drop_policy_info.currency_type+" "+drop_policy_info.refund_amt;
					  html+="<br/><br/>";
					  html+="<span>"+Drupal.t("MSG266")+'</span>';
					}
					$("#dropmsg-lp-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#lp-dropmsg-wizard').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learningplan-tab-inner").data("learningplan").dropLPCall(enrollId,baseType,prgId,"true",isCommerceEnabled,assMand);  $("#dropmsg-lp-content").html(Drupal.t("MSG424")); };

				}
				$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
			    $("#lp-dropmsg-wizard").dialog("open");
				$("#lp-dropmsg-wizard").dialog('option', 'buttons', closeButt);

				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			     $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
				 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
				 //Append div script
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
	dropTrainingPrg : function(data){
		try{
		this.getLPDropPolicy(data);
		}catch(e){
			// to do
		}
	},
	dropLPCall : function(masterEnrollId,baseType,prgId,refundflag,isCommerceEnabled,assMand){
		try{
		closeButt=new Array();
		$("#lp-dropmsg-wizard").dialog('option', 'buttons', closeButt);
		var url = this.constructUrl("ajax/drop-lp/" + masterEnrollId + "/" + prgId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/0");
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				if(drop_policy_result.status=="success")
				{
					var dialogObj=$("#lp-dropmsg-wizard").dialog();
					/*Filter reset issue 71523- Added by maheswari*/
					var status='';
					var statusName='status_filter_myprograms';
					var tab='myprograms';
					if($('#learner-'+tab).find("input[name="+statusName+"]:checked").length > 0 ) {
						$('#learner-'+tab).find("input[name="+statusName+"]:checked").each(function() {
						status+=status!=''?'|':'';
						status+=$(this).val();
						});
					}
					/*Filter reset issue 71523*/
				 	dialogObj.dialog('destroy');
				 	dialogObj.dialog('close');
				 	$('#lp-dropmsg-wizard').remove();
				 	$("#learningplan-tab-inner").data("learningplan").callLearningPlan(status);
					//$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_enr|lrn_tpm_ovr_inp","EnrollmentLP");
					if(document.getElementById('learner-enrollment-tab-inner')) {
						if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollCompleted"){
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_cmp","EnrollCompleted");
					 	}else if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollInCompleted"){
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_inc|lrn_crs_cmp_nsw","EnrollInCompleted");
					 	}else if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollExpired"){
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_exp","EnrollExpired");
					 	}else if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollCanceled"){
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_can","EnrollCanceled");
					 	}else if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollPayments"){
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_reg_ppm|lrn_crs_reg_wtl","EnrollPayments");
					 	}else{
					 		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
						}
						//$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
					}
				}
				else {
					$("#dropmsg-lp-content").html('<span>'+SMARTPORTAL.t(drop_policy_result.msg)+"</span>");
				}
			}
	    });
		}catch(e){
			// to do
		}
	},
	showSwitchClass : function(userId,changeClassId,courseId,enrollId,myEnrChangeCls){
		try{
		if(typeof(myEnrChangeCls)==='undefined') myEnrChangeCls = '';
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var chgClassWidth = 770 ;
		}else{
			var chgClassWidth = 688;
	    }
		$("#learningplan-tab-inner").data("learningplan").defaults.catStart = true;
		this.changeClsEnrollId = enrollId;
		this.renderClassPopup(userId);
		if(navigator.userAgent.indexOf("Safari")!=-1){
		$('#paintCatalogContentResults'+userId).css('min-height','50px').css('overflow','visible').css('height','auto').css("background-color","#fff");
		}else{
		$('#paintCatalogContentResults'+userId).css('min-height','auto').css('overflow','visible').css('height','auto').css("background-color","#fff");
		}
		this.createLoader('paintCatalogContentResults'+userId);
		$('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');

		var nAgt = navigator.userAgent;
		var verOffset ;
		var DetailsW = 465;
		var actionW = 120;
		
		var obj = this;
		var objStr = '$("#learningplan-tab-inner").data("learningplan")';
		$("#tbl-paintCatalogContentResults").jqGrid({
			url:this.constructUrl("ajax/lp-change-class/catalog-search/"+userId+"/"+changeClassId+ "/" +courseId + "/" + myEnrChangeCls),
			datatype: "json",
			mtype: 'GET',
			colNames:['Icons','Details','Action'],
			colModel:[ {name:'Icons',index:'Icons', title:false, width:75,'widgetObj':objStr,formatter:obj.paintLPSearchIcons},
			           {name:'Details',index:'Details', title:false, width:DetailsW,'widgetObj':objStr,formatter:obj.paintLPSearchResults},
			           {name:'Action',index:'Action', title:false, width:actionW,'widgetObj':objStr,formatter:obj.paintLPSearchResultsAction}],
			rowNum:5,
			rowList:[5,10,15],
			pager: '#pager1',
			viewrecords:  true,
			emptyrecords: "",
			sortorder: "desc",
			toppager:true,
			height: 'auto',
			autowidth: true,
            shrinkToFit: true,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			loadComplete:obj.callbackCatalogLoader,
			gridComplete:obj.callbackGridComplete
		}).navGrid('#pager1',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		this.paintCatalogAfterReady(userId);
		}catch(e){
			// to do
		}
	},
	/*
	 * To show the pop-up with the rendered classes
	 */
	renderClassPopup : function(id) {
		try{
		var obj	= this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
			var chgclassqtipWidth = 770 ;
		}else{
			var chgclassqtipWidth = 712;
	    }
		var html = "<div id='paintCatalogContentResults"+id+"'></div>";
        $("#paintCatalogContentResults"+id).remove();
        $('body').append(html);
        var nHtml = "<div id='show_expertus_message'></div><div id='paintCatalogHeader'></div><div id='catalog-no-records' style='display: none'></div><table id='tbl-paintCatalogContentResults'></table><div id='paintCatalogFooter'><div id='pager1' style='display:none;'></div></div>";
        $('#paintCatalogContentResults'+id).html(nHtml);
        var closeButt={};
        $("#paintCatalogContentResults"+id).dialog({
        	autoResize: true,
        	position:[(getWindowWidth()-chgclassqtipWidth)/2,100],
            bgiframe: true,
            width:chgclassqtipWidth,
            resizable:false,
            draggable:false,
            closeOnEscape: false,
            modal: true,
            title:SMARTPORTAL.t('<span class="myteam-header-text">' + Drupal.t("LBL353") +'</span><div id="search-cat-keyword"></div>'),
            buttons: closeButt,
            close: function(){
                $("#paintCatalogContentResults"+id).remove();
                $("#my-team-dialog").remove();
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
	paintLPSearchIcons : function (cellvalue, options, rowObject) {
		try{
			return rowObject['image'];
		}catch(e){
			// to do
		}
	},
	paintLPSearchResults : function(cellvalue, options, rowObject) {
		try{
			return rowObject['details'];
		}catch(e){
			// to do
		}
	},
	paintLPSearchResultsAction : function (cellvalue, options, rowObject) {
		try{
			return rowObject['action'];
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
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:first-child').addClass('assign-learn-first-row');
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:nth-child(2)').addClass('assign-learn-second-row');
		$('#my-team-dialog').find('#tbl-paintCatalogContentResults > tbody > tr > td:last-child').addClass('assign-learn-last-row');
	  var recs = parseInt($("#tbl-paintCatalogContentResults").getGridParam("records"),10);
      if (recs == 0) {
        $('#catalog-no-records'). css('display','block');
        var html = Drupal.t("MSG381")+'.';
        $("#catalog-no-records").html(html);
      } else {
        $('#catalog-no-records'). css('display','none');
        $("#catalog-no-records").html("");
        $('#paintCatalogContentResults'+response.userId).css("background-color","");
      }
      var obj = $("#learningplan-tab-inner").data("learningplan");

	  // Show pagination only when search results span multiple pages
	  var hideAllPageControls = true;
	  if (recs > reccount) { // 5 is the least view per page option.
	    hideAllPageControls = false;
	    $('#my-team-dialog .popupBotLeftCorner').hide();
	    $('#select-class-dialog .popupBotLeftCorner').hide();
	  }

      var reccount = parseInt($("#tbl-paintCatalogContentResults").getGridParam("reccount"), 10);
      if (recs <= reccount) {
        obj.hideCatalogPageControls(hideAllPageControls);
        $('#my-team-dialog .popupBotLeftCorner').show();
        $('#select-class-dialog .popupBotLeftCorner').show();
      }
      else {
        obj.showCatalogPageControls();
        $('#my-team-dialog .popupBotLeftCorner').hide();
        $('#select-class-dialog .popupBotLeftCorner').hide();
      }

      if($("#learningplan-tab-inner").data("learningplan").defaults.catStart) {
		$("#paintCatalogHeader").html(response.header);
		$("#myteam-find-trng-sort-display").show();
		$("#learningplan-tab-inner").data("learningplan").defaults.catStart = false;
      }
      var curHeight = parseInt($(".ui-dialog").eq(-1).css("height")) + parseInt($(".ui-dialog").eq(-1).css("top"));
      var curOlay = parseInt($(".ui-widget-overlay").eq(-1).css("height"));
	  if(curOlay < curHeight) {
	    $(".ui-widget-overlay").eq(-1).css("height", (curHeight + 50) + "px");
	  }
	  $("#tbl-paintCatalogContentResults tr.ui-widget-content:last td").css('border','0px none');
	  $("#learningplan-tab-inner").data("learningplan").destroyLoader('paintCatalogContentResults'+response.userId);
	 
	  resetFadeOutByClass('#my-team-dialog','find-training-list-course','line-item-container','changeclass');
	  vtip();
	  $('.limit-title-cls').trunk8(trunk8.mylearnmulcls_title);
	  $('.limit-desc-cls').trunk8(trunk8.mylearnmulcls_desc);
	  scrollBar_Refresh('learning');
	 }catch(e){
			// to do
	  }
	},
	callbackGridComplete: function(response, postdata, formid){
		try{
			$('.limit-title-cls').trunk8(trunk8.mylearnmulcls_title);
			$('.limit-desc-cls').trunk8(trunk8.mylearnmulcls_desc);
			scrollBar_Refresh('learning');
		}catch(e){
			// to do
		}
	},
	hideCatalogPageControls : function(hideAll) {
	 try{
	  var lastDataRow = $('#tbl-paintCatalogContentResults tr.ui-widget-content').filter(":last");
	  if (hideAll) {
		//$('#pager-lp').hide();
		$('#pager1').hide();
	    if (lastDataRow.length != 0) {
	      lastDataRow.children('td').css('border', '0 none');
	    }
	  }
	  else {
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
	 * action to be performed while clicking on pagination at Change Classes pop-up
	 */
	paintCatalogAfterReady : function(userId) {
		try{
		if(document.getElementById('my-team-dialog')) {
			$('#first_pager1').click( function(e) {
				if(!$('#first_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#prev_pager1').click( function(e) {
				if(!$('#prev_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#next_pager1').click( function(e) {
				if(!$('#next_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#last_pager1').click( function(e) {
				if(!$('#last_pager1').hasClass('ui-state-disabled')) {
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
				}
			});
			$('#pager1 .ui-pg-selbox').bind('change',function() {
				$('#tbl-paintCatalogContentResults').hide();
				$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
				$("#learningplan-tab-inner").data("learningplan").hideCatalogPageControls(false);
				$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
			});
			$("#pager1 .ui-pg-input").keyup(function(event){
				if(event.keyCode == 13){
					$('#tbl-paintCatalogContentResults').hide();
					$('#loaderdivpaintCatalogContentResults'+userId).css('min-height','250px');
					$("#learningplan-tab-inner").data("learningplan").createLoader('paintCatalogContentResults'+userId);
				}
			});
		}
		}catch(e){
			// to do
		 }
	},
	/*
	 * To list all matched classes on class/code autocomplete on class search at pop-up
	 * 'Change Classes' section
	 */
	paintCatkeywordAutocomplete : function(userId,changeClassId,courseId,myenrollChangeClass){
		try{
		var obj	= this;
		if(typeof(myenrollChangeClass)==='undefined') myenrollChangeClass = '';
		var delType	= $('#myteam-cat-delivery-type-hidden').val();
		url = obj.constructUrl("ajax/lp-change-class/catalog-autocomplete/" + delType + '/' + userId + '/' + changeClassId + '/' + courseId + '/' + myenrollChangeClass + '/');
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
	deliveryHideShow : function () {
		try{
		$('#myteam-cat-delivery-type-list').slideToggle();
		$('#myteam-cat-delivery-type-list li:last').css('border-bottom','0px none');
		}catch(e){
			// to do
		 }
	},
	deliveryTypeText : function(dCode,dText,userId,changeClassId,courseId,myenrollChangeClass) {
		try{
		$('#myteam-cat-delivery-type-list').hide();
		$('#myteam-cat-delivery-type').text(dText);
		$('#myteam-cat-delivery-type-hidden').val(dCode);
		$("#learningplan-tab-inner").data("learningplan").paintCatkeywordAutocomplete(userId,changeClassId,courseId,myenrollChangeClass);
		}catch(e){
			// to do
		 }
	},
	/*
	 * To highlight the default text when there is no text, in search filters
	 */
	highlightedText : function(event, ID,textType) {
		try{
			var crrTheme = Drupal.settings.ajaxPageState.theme;
			var fontStyle = (crrTheme == 'expertusoneV2')?'normal':'italic';
			if(event.type == "blur"){
	   			if($("#"+ID).val() != textType) {
	        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
	    		}
	    		if($("#"+ID).val() == '') {
	        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','10px').css('fontStyle',fontStyle);
	    		}
			}
			else if(event.type == "focus"){
	   			if($("#"+ID).val() != textType) {
	        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
	    		}
	    		if($("#"+ID).val() == textType) {
	        		$("#"+ID).val('').css('color','#333333').css('fontSize','11px').css('fontStyle',fontStyle);
	    		}
			}
			else if (event.type == "change"){
	   			if($("#"+ID).val() != textType) {
	        		$("#"+ID).css('color','#333333').css('fontSize','11px').css('fontStyle','normal');
	    		}
	    		if($("#"+ID).val() == '') {
	        		$("#"+ID).val(textType).css('color','#999999').css('fontSize','11px').css('fontStyle',fontStyle);
	    		}
			}
		}catch(e){
			// to do
		 }
	},
	/*
	 * To call class search widget when enter class keywords on pop-up, 'Change Classes' section
	 */
	catkeywordEnterKey : function(userId,changeClassId,courseId,myEnrollChangeCls){
		try{
		 obj = this;
		 $("#srch_criteria_catkeyword").keyup(function(event){
			 if(event.keyCode == 13){
				 obj.catalogSearchAction('','',userId,changeClassId,courseId,myEnrollChangeCls);
			 }
		 });
		}catch(e){
			// to do
		 }
	},
	changeClassEnroll : function(userId, courseId, classId, rowObj, wailist, myEnrollChangeCls){
		try{
		if(typeof(myEnrollChangeCls)==='undefined') myEnrollChangeCls = '';
		var obj = this;
		$("#class-detail-loader-"+classId).remove();
		this.createLoader('paintCatalogContentResults'+userId);
		var changeClsEnrollId = this.changeClsEnrollId;
		var url = obj.constructUrl("ajax/learning/change-class/" + userId + '/' + classId + '/' + changeClsEnrollId+'/'+wailist+'/'+myEnrollChangeCls);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
			   obj.destroyLoader('paintCatalogContentResults'+userId);
			   if(result.status == 'success'){
				 $('#show_expertus_message').html('');
				 $('#show_expertus_message').hide();
			     $("#paintCatalogContentResults"+userId).remove();
                 $("#my-team-dialog").remove();
			     if(myEnrollChangeCls == 'Enroll'){
			    	 $("#paintEnrollmentResults").trigger("reloadGrid");
			     }if(myEnrollChangeCls == 'compliance'){
			    	 // Expired Tab Reload issue #0046138
			    	 $("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_exp","EnrollExpired")
			     }else{
			    	 $("#paintEnrollmentLPResults").trigger("reloadGrid");
			     }
			   }
			   else{
				   var message_call = expertus_error_message(result.msg,'error');
					$('#show_expertus_message').show();
					$('#show_expertus_message').html(message_call);
			   }
			}
		});
		}catch(e){
			// to do
		 }
	},
	renderCallAssignClass : function(userId, courseId, classId, rowObj, wailist){
		try{
		var obj = this;
		$("#class-detail-loader-"+classId).remove();
		var cRow = $(rowObj).parents("tr").parents("tr").before("<tr><td colspan='3'><div id='class-detail-loader-"+classId+"'></div></td></tr>");
		this.createLoader("class-detail-loader-"+classId);
		$('.disp-msg').hide();
		var mandatory=$('#assignClass_checkbox_'+classId+':checked').val();
		var url = obj.constructUrl("ajax/learning/myteam-search/assign-class/" + userId + '/' + courseId + '/' + classId + '/' + mandatory+'/'+wailist);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
			result = $.trim(result);
			   if(userId!=0)

			   //if(result == 'Assigned') {
			   if((result == 'Assigned') || (result == 'Waitlisted')) {

				   var showAction = ((result == 'Assigned') ? Drupal.t("Registered") : ((result == 'Waitlisted') ? Drupal.t("LBL190") : ''));
				   if(result == 'Assigned') {
					 $("#assignClass_"+classId).removeClass("action-btn");
				   } else if(result == 'Waitlisted') {
					   $("#assignClass_"+classId).removeClass("action-btn-waitlist");
				   }

				   obj.callAvailableSeats(classId,wailist,userId);

				   $("#assignClass_"+classId).removeAttr('onclick');
				   //$("#assignClass_"+classId).removeClass("action-btn");
				   $("#assignClass_"+classId).addClass("action-btn-disable");
				   //$("#assignClass_"+classId).html(SMARTPORTAL.t("Registered"));
				   $("#assignClass_"+classId).html(showAction);
				   $('#assignClass_checkbox_'+classId).attr("disabled", true);
			   }else{
				   $('#assignClassMsg_'+classId).html(result);
				   $('#assignClassMsg_'+classId).show();
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
	 * Action to performed during class search in the pop-up
	 */
	catalogSearchAction : function(sortbytxt,className,userId,changeClassId,courseId,myEnrollChangeCls) {
		try{
		var searchStr = '';
		/*-------Title-------*/
		var title 	  = $('#srch_criteria_catkeyword').val();
		if(title == Drupal.t("LBL545"))
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
		$("#learningplan-tab-inner").data("learningplan").hideCatalogPageControls(true);
		$('#tbl-paintCatalogContentResults').hide();
        this.createLoader('paintCatalogContentResults'+userId);
        $('#loaderdivpaintCatalogContentResults'+userId).css('height','250px');

		$('#tbl-paintCatalogContentResults').setGridParam({url: this.constructUrl("ajax/lp-change-class/catalog-search/"+userId+"/"+changeClassId+ "/" +courseId+ "/" + myEnrollChangeCls +'/'+searchStr)});
	    $("#tbl-paintCatalogContentResults").trigger("reloadGrid",[{page:1}]);
	    // have added the code for when change the sort by in class popup added by yogaraja
	    $('.'+className).parents('li').siblings().find('a').removeClass('sortype-high-lighter');
		if(className!=''){
			$('.'+className).addClass('sortype-high-lighter');
		}else{
			$('#paintCatalogHeader .catalog-type1').addClass('sortype-high-lighter');
		}
		}catch(e){
			// to do
		 }
	},

	getConsolidatedQuizStatusLP : function(contentId, launchInfo) {
		var quizStatus = "";
		var lessonCount = 0;
		$(launchInfo).each(function(i, launch)  {
			var contentInfo = launch[0];
			if(contentInfo.ContentId == contentId) {	//if multiple contents attached to single enrollment, separate status should be considered
				lessonCount ++;
				var successStatus = contentInfo.contentQuizStatus;
				if (successStatus == "failed" || successStatus == 'incomplete' || successStatus.trim() == '') {
					quizStatus = successStatus;
					return false;
				} else if( contentInfo.ContentCompletionStatus == 'completed' || contentInfo.ContentCompletionStatus == ''){
					quizStatus = "passed";
				} else {
					quizStatus = "";
				}
			}
		});
		return quizStatus;
	}
});

$.extend($.ui.learningplan.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true,catStart:true}});

})(jQuery);
$(function() {
	try{
	$( "#learningplan-tab-inner" ).learningplan();
	if(document.getElementById('learningplan-tab-inner')) {
		$('#first_pager-lp').click( function(e) {
			try{

			if(!$('#first_pager-lp').hasClass('ui-state-disabled')) {
				$('#gview_paintEnrollmentLPResults').css('min-height','100px');
				$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			}
			}catch(e){
				// to do
			 }
		});
		$('#prev_pager-lp').click( function(e) {
			try{
			if(!$('#prev_pager-lp').hasClass('ui-state-disabled')) {
				$('#gview_paintEnrollmentLPResults').css('min-height','auto');
				$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			}
			}catch(e){
				// to do
			 }
		});
		$('#next_pager-lp').click( function(e) {
			try{
			if(!$('#next_pager-lp').hasClass('ui-state-disabled')) {
				$('#gview_paintEnrollmentLPResults').css('min-height','auto');
				$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			}
			}catch(e){
				// to do
			 }
		});
		$('#last_pager-lp').click( function(e) {
			try{
			if(!$('#last_pager-lp').hasClass('ui-state-disabled')) {
				$('#gview_paintEnrollmentLPResults').css('min-height','auto');
				$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			}
			}catch(e){
				// to do
			 }
		});
		$('#pager-lp .ui-pg-selbox').bind('change',function() {
			try{
			$('#gview_paintEnrollmentLPResults').css('min-height','auto');
			$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			$("#learningplan-tab-inner").data("learningplan").hidePageControls(false);
			}catch(e){
				// to do
			 }
		});
		$("#pager-lp .ui-pg-input").keyup(function(event){
			try{
			if(event.keyCode == 13){
				$('#gview_paintEnrollmentLPResults').css('min-height','auto');
				$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
			}
			}catch(e){
				// to do
			 }
		});
		//select record count in veiw per page
		if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
			$('#enroll-lp-result-container .page-show-prev').bind('click',function() {
				try{
				if(parseInt($("#pg_pager-lp .page_count_view").attr('id')) < 0){
					$("#pg_pager-lp .page_count_view").attr('id','0');
				}else{
					$('#gview_paintEnrollmentLPResults').css('min-height','100px');
					$("#learningplan-tab-inner").data("learningplan").hidePageControls(false);
					$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
				}
				}catch(e){
					// to do
				 }
			});
			$('#enroll-lp-result-container .page-show-next').bind('click',function() {
				try{
				if(parseInt($("#pg_pager-lp .page_count_view").attr('id')) >= parseInt($("#pg_pager-lp .page-total-view").attr('id'))){
					$("#pg_pager-lp .page_count_view").attr('id',($("#pg_pager-lp .page_count_view").attr('id')-1));
				}else{
					$('#gview_paintEnrollmentLPResults').css('min-height','100px');
					$("#learningplan-tab-inner").data("learningplan").hidePageControls(false);
					$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
				}
				}catch(e){
					// to do
				 }
			});

	      }
	}
	}catch(e){
		// to do
	 }
});
function refresh_enrolledlp(window){
//	console.log('inside refresh enrolled');
	if(!window.closed){
		var window = window;
		setTimeout(refresh_enrolledlp,10000,window);
	}else{
	//	console.log('closed');
		reload_page();
	}
}

function refresh_timelp(timer){
	setTimeout(reload_page,timer);
}

function reload_pagelp(){
	if(document.getElementById('loaderdivenroll-lp-result-container')){
		//console.log('inside loader div');
	}else{
		//console.log('inside no loader div');
	$("#paintEnrollmentLPResults").setGridParam().trigger("reloadGrid");
}	
}