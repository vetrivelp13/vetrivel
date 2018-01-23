(function($) {
	var dWidth='';

$.widget("ui.enrollment", {
	_init: function() {
		try{
			var DrupalLocale = {'failed' : Drupal.t('failed'),
					'passed' : Drupal.t('passed'),
					'unknown' : Drupal.t('unknown'),
					'incomplete': Drupal.t('Incomplete'),
					'completed': Drupal.t('Completed')
					};
		var self = this;
		this.enrUserId = this.getLearnerId();
		dWidth = $("#enroll-result-container").width();
		if(dWidth==1000) {
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').addClass('enroll-disable-right-region');
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').removeClass('enroll-enable-right-region');
		}
		else {
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').addClass('enroll-enable-right-region');
			$('#block-exp-sp-lnrenrollment-tab-my-enrollment').remove('enroll-enable-right-region');
		}
		this.renderEnrollResults();
		this.prevObj;
		this.prevMoreObj;
		if(document.getElementById('learner-enrollment-tab-inner'))
		this.widgetObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
		else
			this.widgetObj = '$("#lnr-catalog-search").data("enrollment")';
		this.qtipListArr = new Array();
		}catch(e){
			// to do
		}
	},

  hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintEnrollmentResults tr.ui-widget-content').filter(":last");
    if (hideAll) {
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').show();
      $('#pager').hide();
      $('#gview_paintEnrollmentResults').css('padding', '0');

      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').hide();
     // $('#gview_paintEnrollmentResults').css('padding-bottom', '12px');
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #prev_pager').hide();
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #last_pager').hide();
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
	}catch(e){
		// to do
	}
  },

  showPageControls : function() {
	try{
	var lastDataRow = $('#paintEnrollmentResults tr.ui-widget-content').filter(":last");
	lastDataRow.children('td').css('border', '0 none');
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    $('#block-exp-sp-lnrenrollment-tab-my-enrollment .block-footer-left').hide();
    $('#gview_paintEnrollmentResults').css('padding-bottom', '0px');
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #prev_pager').show();
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #last_pager').show();
    $('#pager #pager_center #sp_1_pager').parent().show();
	}catch(e){
		// to do
	}
  },

	renderEnrollResults : function(){
		try{
	   	this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var dWidth = 663;
		}else{
			var dWidth = 667;
	    }

	  	if(document.getElementById('learner-enrollment-tab-inner')){
	  	if (Drupal.settings.last_left_panel==true) {
	  		rowNumValue = 20;
	  		rowListValue = [20,30,40];
	  	} else {
	  		rowNumValue = 10;
	  		rowListValue = [10,15,20];
	  	}
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
		var searchEnrollStr = '';
		var getRegStatus = 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
		var obj = $("#learner-enrollment-tab-inner").data("enrollment");
		var objStr = '$("#learner-enrollment-tab-inner").data("enrollment")';

	  	}else{
	  		$("#lnr-catalog-search").data("enrollment").createLoader('enroll-result-container');
			var searchEnrollStr = '';
			var getRegStatus = 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
			var obj = $("#lnr-catalog-search").data("enrollment");
			var objStr = '$("#lnr-catalog-search").data("enrollment")';
	  	}

		var userId = this.enrUserId;
		var enrStr = Drupal.settings.myenrolmentSearchStr;
		if (typeof enrStr === 'undefined' || enrStr == null || enrStr == undefined){
			searchEnrollStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus+'&sortBy=AZ';
		}
		else {
			searchEnrollStr	= '&UserID='+userId+enrStr
		}
		var url = this.constructUrl("learning/enrollment-search/all/"+searchEnrollStr);
		if(Drupal.settings.mylearning_right===false){
			$("#paintEnrollmentResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sortable':false,formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintEnrollmentTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintActions}

				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager',
				rowTotal: 0,
				viewrecords: true,
				emptyrecords: "No Search Results Found",
				sortorder: "asc",
				toppager:true,
				height: 'auto',
				autowidth: true,
				shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackLoader

			}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
		$("#paintEnrollmentResults").jqGrid({
			url: url,
			datatype: "json",
			mtype: 'GET',
			colNames:['','',''],
			colModel : [
			            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sortable':false,formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
			            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintEnrollmentTitle},
			     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintActions}

			     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
			pager: '#pager',
			rowTotal: 0,
			viewrecords: true,
			emptyrecords: "No Search Results Found",
			sortorder: "asc",
			toppager:true,
			height: 'auto',
			width: dWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadComplete:obj.callbackLoader

		}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
		obj.hidePageControls(true); // show in loadComplete callbackLoader()
		$('.ui-jqgrid').addClass('myenrollment-table');
		$('.jqgfirstrow td:first-child').addClass('enroll-datatable-column-img');
		$('.jqgfirstrow td:nth-child(2)').addClass('enroll-datatable-column1');
		$('.jqgfirstrow td:last-child').addClass('enroll-datatable-column2');
		$('#paintEnrollmentResults .jqgfirstrow').hide();
		$('.ui-jqgrid-bdiv > div').css('position','static');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		$('#pager').find('#pg_pager .ui-pg-table').css("table-layout","auto");
		$('#pager').find('#pg_pager .ui-pg-table #pager_center').removeAttr("style");
		this.setSortTypeData('lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att');
		/* to highlight the default sort order - added by Rajkumar U*/
		$('#enroll-result-container .sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-result-container .add-high-lighter').addClass('sortype-high-lighter');
		$('.iframe-mylearning #innerWidgetTagEnroll #pager').css("width","948px");
		$('.iframe-mylearning #innerWidgetTagEnroll #pager-lp').css("width","948px");
		if(this.currTheme != "expertusoneV2"){
		  $('.iframe-mylearning #innerWidgetTagEnroll .enroll-action').css("right","321px");
		}

		}catch(e){
			// to do
		}
	},

	enrollSortForm : function(sort,className) {
		try{
		// High light sortype
		$('#enroll-result-container .sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#enroll-result-container .'+className).addClass('sortype-high-lighter');

		var getEnrData 		= eval($("#sort-by-enroll").attr("data"));
		var currEnrMode 	= getEnrData.currEnrMode;
		var userId 			= this.enrUserId;
    	var getRegStatus 	= '';
    	var searchStr 		= '';
    	searchStr			= '&UserID='+userId+'&regstatuschk='+currEnrMode+'&sortBy='+sort;
    	var url 			= this.constructUrl('learning/enrollment-search/all/'+searchStr)
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
    	$('#paintEnrollmentResults').setGridParam({url: url});
        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
		}catch(e){
			// to do
		}
	},

	updateMultiContentLaunchDialog : function(enrollmentId, lessonId, lessonInfo, contentQuizStatusConsolidated) { // function added for mantis ticket #0020086
    // Update lessonlocation data in the launch button of the lesson
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
      status = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.Status)+'" class="vtip">'+titleRestrictionFadeoutImage(Drupal.t('LBL102') +': ' + Drupal.t(lessonInfo.Status),'exp-sp-lnrenrollment-status',200)+'</span>';
    }
    var lesDiv = lessonId + "_" + enrollmentId + "_launch_button";
    if(document.getElementById(lesDiv) != null){
      $("#" + lessonId + "_" + enrollmentId + "_launch_button").data('relaunch', relaunchInfo);
      $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_status").html(status);
    }
    // Update lesson score
    var lesnscore = '';
    var pipe1 = '<span class="enroll-pipeline">|</span>';
    if(lessonInfo.LesScore !=null && lessonInfo.LesScore!=undefined && lessonInfo.LesScore != ''&& lessonInfo.LesScore != '0.00') {
    	lesnscore = '<div class="line-item-container float-left">'+pipe1 + '<span title="'+Drupal.t('LBL668')+': '+lessonInfo.LesScore+'" class="vtip">'+ titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + lessonInfo.LesScore,'exp-sp-lnrenrollment-score',200)+'</span></div>';
    }
    var contentQuizStatus = '';
    if(lessonInfo.contentQuizStatus != null && lessonInfo.contentQuizStatus != undefined && lessonInfo.contentQuizStatus != '') {
    	contentQuizStatus = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus) + '">'+  titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(lessonInfo.contentQuizStatus),'exp-sp-lnrenrollment-sucstatus')+ '</span>';
    }
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonScore").html(lesnscore);
    $("#" + lessonId + "_" + lessonInfo.ContentId + "_" + enrollmentId + "_lessonQuizStatus").html(contentQuizStatus);
    // Update content status and score
    var contentStatus = Drupal.t("MSG511");
    var Dstatus = Drupal.t("In progress");
	var Dstatus1 = Drupal.t("Incomplete");
	var Dstatus2 = Drupal.t("Completed");

  
    if (lessonInfo.ContentStatus != '') {
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.ContentStatus)+'" class="vtip">'+titleRestrictionFadeoutImage( Drupal.t('LBL102') +' : ' + Drupal.t(lessonInfo.ContentStatus),'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }else{
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.ContentStatus)+'" class="vtip">'+ titleRestrictionFadeoutImage( Drupal.t('LBL102') +' : ' + contentStatus,'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }
    if(lessonInfo.LaunchType == 'VOD' && lessonInfo.Status != ''){
    	contentStatus = '<div class="line-item-container float-left"><span title="'+Drupal.t('LBL102')+': '+Drupal.t(lessonInfo.Status)+'" class="vtip">'+ titleRestrictionFadeoutImage(Drupal.t('LBL102') +' : ' + Drupal.t(lessonInfo.Status),'exp-sp-lnrenrollment-status',200)+'</span></div>';
    }
    var score = '';
    var pipe = '<span class="enroll-pipeline">|</span>';
    if(lessonInfo.ContScore !=null && lessonInfo.ContScore!=undefined && lessonInfo.ContScore != '' && lessonInfo.ContScore != '0.00') {
      score = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + lessonInfo.ContScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + lessonInfo.ContScore,'exp-sp-lnrenrollment-score',200)+'</span></div>';
    }
    var attemptsLeft = (lessonInfo.AttemptLeft == 'notset')? '' : lessonInfo.AttemptLeft;
    if (attemptsLeft !== '') {
      attemptsLeft = '<div class="line-item-container float-left">'+pipe +'<span class="vtip" title="'+Drupal.t('LBL202')+' : '+' ' + attemptsLeft+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL202')+' : ' +' ' + attemptsLeft,'exp-sp-lnrenrollment-attemptsleft',200)+'</span></div>';
    }
    var versionNo = lessonInfo.VersionNum;
    if (versionNo !== '') {
      versionNo = '<div class="line-item-container float-left">'+pipe +'<span class="vtip" title="'+Drupal.t('LBL1123')+' : '+' ' + versionNo+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL1123')+' : ' +' ' + versionNo,'exp-sp-lnrenrollment-version',200)+'</span></div>';
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
    	validityDay = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604')+' : ' + remValidityDays + ' ' + daysLBL+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-lnrenrollment-validitydays',200)+'</span></div>';
    }
    contentQuizStatus = lessonInfo.contentQuizStatus;
    var data = eval($('#launch'+enrollmentId).attr('data'));
    //console.log('checck:: ' +data);
    contentQuizStatus = (data !== undefined ? widgetObj.getConsolidatedQuizStatus(lessonInfo.ContentId, data) : (contentQuizStatusConsolidated !== undefined ? contentQuizStatusConsolidated : ''));
    if(document.getElementById('lnr-catalog-search')){
    	var success_status = lessonInfo.contentQuizStatus;
		if (success_status == "failed" || success_status == 'incomplete' || success_status == '') {
			contentQuizStatus = success_status;
		}
		else if((lessonInfo.contentQuizStatus=='completed' || lessonInfo.contentQuizStatus=='Completed') && (lessonInfo.ContentType == 'Knowledge Content' || lessonInfo.ContentType == 'Tin Can'))
    		contentQuizStatus = Drupal.t('passed');
    }
    if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
		  contentQuizStatus = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
	  }
	var percentCompleted = '';
	if(lessonInfo.LaunchType == 'VOD') {
		var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "" && lessonInfo.SuspendData != undefined) ? JSON.parse(unescape(lessonInfo.SuspendData)) : null;
		var progress = suspend_data != null ? suspend_data['progress'] : 0;
		progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
		percentCompleted = '<div class="line-item-container float-left">'+pipe1 + '<span class="vtip" title="'+ progress+'% '+Drupal.t('Completed') + '">'+ titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
	}
    $("#lesson_status_"+lessonInfo.ContentId).html(contentStatus + score + contentQuizStatus + attemptsLeft +validityDay + percentCompleted + versionNo);
    // When no more attempts left, disable the Launch button
    if (lessonInfo.AttemptLeft == 0) {
      // Single lesson content
      $("#lesson-launch-" + lessonInfo.ContentId + " .launch_button_lbl").removeClass('enroll-launch-full').addClass('enroll-launch-empty');
      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeClass('actionLink');
      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").removeAttr('onclick');

      // More than 1 lesson in content
      $("#" + lessonInfo.ContentId + "LessonSubGrid .launch_button_lbl").removeClass('enroll-launch-full').addClass('enroll-launch-empty');
      $("#" + lessonInfo.ContentId + "LessonSubGrid .enroll-launch").removeClass('actionLink');
      $("#" + lessonInfo.ContentId + "LessonSubGrid .enroll-launch").removeAttr('onclick');

      $("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() { // for VOD content type
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
	if(lessonInfo.LaunchType == 'VOD') {
		//update launch button progress
//		console.log($("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch"));
		$("#lesson-launch-" + lessonInfo.ContentId + " .enroll-launch").each(function() {
			var launchButtonId = $(this).attr('id');
			var href = $("#" + launchButtonId).attr('href');
			href_suspend = href.substr(0, href.lastIndexOf("/"));
			href = href_suspend.substr(0, href_suspend.lastIndexOf("/"));
			var suspend_data = (lessonInfo.SuspendData != null && lessonInfo.SuspendData != "" && lessonInfo.SuspendData != undefined) ? JSON.parse(unescape(lessonInfo.SuspendData)) : null;
			// var progress = suspend_data != null ? suspend_data['progress'] : 0;
			//console.log(progress);
			if(lessonInfo.AttemptLeft != 0) {	//update href attribute if attempts left to launch
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
			}
		});
	}
		
	}catch(e){
		// to do
		// console.log(e);
	}
	},

	callbackLoader : function(data, postdata, formid, updateShowMore){
	 try{
	     var default_cookie = '&regstatuschk=lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';
		 if(document.getElementById('learner-enrollment-tab-inner'))
			var obj = $("#learner-enrollment-tab-inner").data("enrollment");
		 else
			 var obj = $("#lnr-catalog-search").data("enrollment");
		obj.destroyLoader('enroll-result-container');
		if(Drupal.settings.mylearning_right===false)
			$('.clsenroll-result-container #pager').width($('.clsenroll-result-container').width()+4);
		// Delete enrollmentId and lessonId from jqGrid postData object when present. Added for mantis ticket #0020086
		var postData = $('#paintEnrollmentResults').getGridParam("postData");
		delete postData.enrollmentId;
		delete postData.lessonId;

		if (document.getElementById('launch-content-container') &&
		      data.triggering_enrollment_id && data.triggering_lesson && data.triggering_lesson_details) { // added for mantis ticket #0020086
		  obj.updateMultiContentLaunchDialog(data.triggering_enrollment_id, data.triggering_lesson, data.triggering_lesson_details, data.triggering_content_quiz_status);
		}

//		var recs = parseInt($("#paintEnrollmentResults").getGridParam("records"),10);
//		$("#paintEnrollmentResults")[0].p.rowTotal = recs;
		$("#paintEnrollmentResults")[0].p.rowTotal = data['records'];
		var recs = data['records'];
		updateShowMore = ((updateShowMore == undefined || updateShowMore == null) ? true : updateShowMore);
		if (updateShowMore) {
			$("#paintEnrollmentResults").data('totalrecords', recs);
			var showMore = $('#paintEnrollmentResults-show_more');
			if ($('#paintEnrollmentResults').getGridParam("reccount") < recs) {
				showMore.show();
			} else {
				showMore.hide();
			}
		}
		//Added by ganeshbabuv to avoid the issue for not showing the enrollment message if there are no enrollments and comes from salesforce app - Ref:SF Cookieless Option (0054508),Sep 30th 2015
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {		    
			setTimeout(function(){
				if (recs <= 0){ 
					if(Drupal.settings.myenrolmentSearchStr === default_cookie){
						$('#enroll-noresult-msg').show();
					}
					else{
						$('#enroll-nosearchresult-msg').show();
					}
					$('#enroll-result-container .sort-by-text').hide();
					$('#enroll-result-container .sort-by-links').hide();
				} else {
					$('#enroll-result-container .sort-by-text').show();
					$('#enroll-result-container .sort-by-links').show();
				}
			},100);
			
		}else{  
			if (recs <= 0) {
				if(Drupal.settings.myenrolmentSearchStr === default_cookie){
					$('#enroll-noresult-msg').show();
				}
				else{
					$('#enroll-nosearchresult-msg').show();
				}
				$('#enroll-result-container .sort-by-text').hide();
				$('#enroll-result-container .sort-by-links').hide();
			} else {
				$('#enroll-result-container .sort-by-text').show();
				$('#enroll-result-container .sort-by-links').show();
			}
		}	
		 

    // Show pagination only when search results span multiple pages
    var reccount = parseInt($("#paintEnrollmentResults").getGridParam("reccount"), 10);
    var hideAllPageControls = true;
    if (recs > 10) { // 10 is the least view per page option.
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
        	var nodeId = $(this).parents("form").siblings("input:hidden").val();
        	var entity_type = "Class";
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
	    				$("body").data("learningcore").fiveStarCheck(nodeId,'enroll-node-');
	    				var average_stars = (result.average.value * 5) / 100;
		    			average_stars = average_stars.toFixed(1);
		    			var voteText = (result.count.value == 1) ? result.votemsg : result.votesmsg;
		    			var avgRating = '<span class="total-votes"> (<span>'+result.count.value+'</span> '+voteText+')</span>';
		    			$(fivestarClick).parent("div").next("div.description").children("div.fivestar-summary").html(avgRating);
	    			}
    			}
    	    });
		});
		$("#learner-enrollment-tab-inner").data("enrollment").shapeTheButton('paintEnrollmentResults');
		$('#enroll-result-container .ui-jqgrid-hdiv').css('display','none');
		$('#paintEnrollmentResults .jqgfirstrow').css('display','none');
		//Vtip-Display toolt tip in mouse over
		
		
		
		if (Drupal.settings.salesforce.type == "canvas" || Drupal.settings.salesforce.type == "iframe") {			
			if (Drupal.settings.mylearning_right===false) {
				var currentWidth = 942;
				$("#paintEnrollmentResults").jqGrid('setGridWidth', currentWidth);
				// var currentPage = $('.ui-pg-input').val();
				// $("#paintEnrollmentResults").setGridParam({page: currentPage}).trigger('reloadGrid');
			}
		} 
		//alert(navigator.userAgent.indexOf('Edge/'));
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if(typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1" && (navigator.userAgent.indexOf('Edge/')!= -1)) {		
			 RefreshFadeoutForSF('#paintEnrollmentResults','item-title-code','line-item-container','mylearning');
			 Refreshtrunk8('myenrollments','limit-title-enr');
		}else{ 
			resetFadeOutByClass('#paintEnrollmentResults','item-title-code','line-item-container','mylearning');
			$('.limit-title-enr').trunk8(trunk8.myenroll_title);
			$('.limit-desc-enr').trunk8(trunk8.myenroll_desc);
		}
		//$('.cp-menulist .limit-title').trunk8(trunk8.contstrip_title);
		vtip();
		$("#paintEnrollmentResults").showmore({
			'grid': "#paintEnrollmentResults",
			'gridWrapper': '#enroll-result-container',
			'showMore': '#paintEnrollmentResults-show_more'
		});
	 }catch(e){
			// to do		
		}
	},
	



	shapeTheButton : function(parentId){
		try{
		$('#'+parentId).find(".enroll-main-list").each(function() {
			var avlBtn = 0;
			avlBtn += ($(this).children(".enroll-launch-gray").length > 0)? 1: 0;
			avlBtn += ($(this).children(".enroll-launch-more-gray").length > 0)? 2: 0;

			switch(avlBtn) {
				case 2:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch").wrap("<label class='enroll-launch-full'></label>");
				case 3:
					$(this).children(".enroll-launch-more-gray").hide().siblings(".enroll-launch-gray").wrap("<label class='enroll-launch-empty'></label>");
			}
			resetFadeOutByClass('#launch-wizard #paintEnrollmentResults','item-title-code','line-item-container','mylearning');
		});
		}catch(e){
			// to do
		}
	},

	paintLPSearchIcons : function (cellvalue, options, rowObject) {
		try{
		var type = 'cre_sys_obt_cls';
		var deliverytypecode = rowObject['delivery_type_code'];
	    var html		= '';
		var iconType='';
		if(type == "cre_sys_obt_cls"){

			if(deliverytypecode == 'lrn_cls_dty_ilt' ) {
				iconType = "iltIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_wbt' ) {
				iconType = "wbtIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_vcl' ) {
				iconType = "vclIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}else if(deliverytypecode == 'lrn_cls_dty_vod' ) {
				iconType = "vodIcon";
				html += '<div class='+iconType+'>';
				html += '<div class="Crs_IconTitle"></div>';
				html += '<div class="Crs_IconTxt"></div>';
				html += '</div>';
			}
		}else if(type == "cre_sys_obt_crt"){
			iconType = "Cert_Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';
		}else if(type == "cre_sys_obt_cur"){
			iconType = "Curr_Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';

		}else if(type == "cre_sys_obt_trp"){
			iconType = "TP Icon";
			html += '<div class='+iconType+'>';
			html += '<div class="Crs_IconTitle"></div>';
			html += '<div class="Crs_IconTxt"></div>';
			html += '</div>';
		}

		return html;
		}catch(e){
			// to do
		}
	},

	setEnrStateInMsg : function(regActive) {
		try{
		switch (regActive) {
		case 'Enrollmentpart' :
			$('#enrollment-state').html((Drupal.t('Enrolled')).toLowerCase());
			break;
		case 'EnrollCompleted':
			$('#enrollment-state').html(Drupal.t('Completed'));
			break;
		case 'EnrollInCompleted':
			$('#enrollment-state').html((Drupal.t('Incomplete')).toLowerCase());
			break;
		case 'EnrollExpired':
			$('#enrollment-state').html((Drupal.t('Expired')).toLowerCase());
			break;
		case 'EnrollCanceled':
			$('#enrollment-state').html(Drupal.t('Canceled'));
			break;
		case 'EnrollPayments' :
			$('#enrollment-state').html((Drupal.t('Pending')).toLowerCase());
			break;
		}
		}catch(e){
			// to do
		}
	},

	callEnrollment : function(regStatus, regActive){
		try{
		$('#enroll-noresult-msg').hide();
		$('#enroll-nosearchresult-msg').hide();
		this.setEnrStateInMsg(regActive);
		var userId = this.enrUserId;
    	var getRegStatus = (regStatus) ? regStatus : 'lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att';
    	var searchStr = '';
    	searchStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus;
    	var url = this.constructUrl('learning/enrollment-search/all/'+searchStr);
    	this.setSortTypeData(getRegStatus);
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
    	$('#paintEnrollmentResults').setGridParam({url: url});
        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
       	this.highlightCourseTab(regActive);
       	$('#enroll-result-container .sort-by-links').find('li a').removeClass('sortype-high-lighter');
       	$('#enroll-result-container .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
       	if(this.currTheme == "expertusoneV2"){
           $('#learner-enrollment .sort-by-links').find('li a').removeClass('sortype-high-lighter');
           $('#learner-enrollment .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
        }
		}catch(e){
			// to do
		}
    },

    setSortTypeData : function(getRegStatus) {
    	try{
    	var setdata = "data={'currEnrMode':'"+getRegStatus+"'}";
    	$("#sort-by-enroll").attr("data",setdata);
    	}catch(e){
    		// to do
    	}
    },

	highlightCourseTab : function(highlightId){
		try{
		var curTheme = themepath.split("/");
		var resTheme = curTheme[curTheme.length-1];
		if (resTheme == 'expertusoneV2')
		{
			$('#learner-maincontent_tab3 ul li a#'+highlightId).parent('li').siblings('li').removeClass('selected');
			$('#learner-maincontent_tab3 ul li a#'+highlightId).parent('li').addClass('selected');
		} else
		{
			$("#learner-maincontent_tab3 ul li a").each(function(){
			    $(this).removeClass('orange');
			});
			$('#learner-maincontent_tab3 ul li a#'+highlightId).addClass('orange');
		}
		}catch(e){
			// to do
		}
	},

	paintEnrollmentTitle : function(cellvalue, options, rowObject) {
		try{
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		var htmlVal='';
		var wobj = eval(options.colModel.widgetObj);
		var obj1 = options.colModel.widgetObj;
		var baseType				= rowObject['basetype'];
		var dateValidTo				= rowObject['valid_to'];
		var dataDelType				= rowObject['delivery_type'];
		var dataDelTypeCode			= rowObject['delivery_type_code'];
		var dataRegStatus			= rowObject['reg_status'];
		var dataRegStatusCode		= rowObject['reg_status_code'];
		var RegStatusCode			= rowObject['reg_status_code'];
		var courseComplitionStatus	= rowObject['reg_status'];
		var RegStatusDate          =  rowObject['reg_status_date'];
		var courseScore				= rowObject['score'];
		var quizStatus				= rowObject['quiz_status'];
		var dataId 					= rowObject['id'];
		var courseId 				= rowObject['course_id'];
		var classId 				= rowObject['class_id'];
		var classTitle				= rowObject['cls_title'];
		var classCode				= rowObject['cls_code'];
		var courseTitle				= rowObject['course_title'];
		var courseTempId			= rowObject['courseid'];
		var session_id				= rowObject['session_id'];
		var SessStartDate 			= rowObject['session_start'];
		var sessionStartDay			= rowObject['session_start_day'];
		var SessEndDate 			= rowObject['session_end'];
		var shortDescription 		= encodeURIComponent(rowObject['description']);
		var descriptionFull			= encodeURIComponent(rowObject['descriptionfull']);
		var language 				= rowObject['language'];
		var usertimezonecode = rowObject['usertimezonecode'];
		var session_code = rowObject['session_code'];
		var user_tzcode =  rowObject['user_tzcode'];
		var LocationName 			= rowObject['location_name'];
		var LocationId				= rowObject['location_id'];
		var LocationAddr1 			= rowObject['location_addr1'];
		var LocationAddr2 			= rowObject['location_addr2'];
		var LocationCity 			= rowObject['location_city'];
		var LocationState 			= rowObject['location_state'];
		var LocationCountry 		= rowObject['location_country'];
		var LocationZip 			= rowObject['location_zip'];
		var LocationPhone 			= rowObject['location_phone'];
		var regDate					= rowObject['reg_date'];
		var remDays			        = rowObject['remDays'];
		var compDate				= rowObject['comp_date'];
		var updateDate				= rowObject['updated_on'];
		var startDateFormat			= rowObject['session_start_format'];
		var lnrAttach				= rowObject['show_lnr_attach'];
		var show_events				= rowObject['show_events'];
		var LessonLocation			= rowObject['LessonLocation'];
		var launch_data				= rowObject['LaunchData'];
		var suspend_data			= rowObject['SuspendData'];
		var exit					= rowObject['CmiExit'];
		var labelmsg 				= rowObject['labelmsg'];
		var updated_by_ins_mngr_slf	= rowObject['updated_by_ins_mngr_slf'];
		var created_by_ins_mngr_slf	= rowObject['created_by_ins_mngr_slf'];
		var compliance_compl_days 	= rowObject['compliance_complete_days'];
		var compliance_compl_date	= rowObject['compliance_complete_date'];
		var compliance_val_days 	= rowObject['compliance_validity_days'];
		var compliance_val_date		= rowObject['compliance_validity_date'];
		var compliance_expire		= rowObject['compliance_expire'];
		var usrId					= rowObject['user_id'];
		var compliance_expired_on_validity = rowObject['compliance_expired_on_validity'];

		var daysRemaining = '';
		var dayRemainVal  = '';
		var daysLeft = '';
		var AttemptLeft	  = '';
		var ValidityDays	  = '';
		var contValidateMsg = '';
		var classScore = '';
		var mro						= rowObject['mro'];
		var mro_id						= rowObject['mro_id'];
		var assigned_by				= unescape(rowObject['assigned_by']).replace(/"/g, '&quot;');

		var FullName			= rowObject['full_name'].replace(/'/g, "\\\'");
		var MandatoryOption			= rowObject['mandatory'];
		var IsCompliance			= rowObject['is_compliance'];
		var Managerid             =  rowObject['managerid'];
		var UpdatedBy             =  rowObject['updated_by'];
		var CreatedBy             =  rowObject['created_by'];
		var UpdatedByName         =  rowObject['updated_by_name'].replace(/'/g, "\\\'");
		var waitlist_priority     =  rowObject['waitlist_priority'];
		var isLastRec             =  (typeof rowObject['is_last_rec'] != 'undefined' && rowObject['is_last_rec'] == 'last')? 'last' : '';
		var diffDays = remDays;
		var complianceCompDate	= (compliance_compl_days) ? compliance_compl_days : compliance_compl_date;
		var is_exempted		= rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'].replace(/'/g, "\\\'");;
		var exempted_on = rowObject['exempted_on'];
		var VersionNo = '';
		var mroImageClass = '';
		var mroImageClassArr = new Array();
		mroImageClassArr['cre_sys_inv_com'] =  '<span class="course-compliance-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Compliance')+'">'+Drupal.t('LBL743')+'</span>';
		mroImageClassArr['cre_sys_inv_man'] =  '<span class="course-mandatory-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Mandatory')+'">'+Drupal.t('M')+'</span>';
        mroImageClassArr['cre_sys_inv_opt'] =  '<span id="'+classId+mro_id+'" title="'+Drupal.t('Optional')+'">'+'</span>';
        mroImageClassArr['cre_sys_inv_rec'] =  '<span class="course-recommended-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Recommended')+'">'+Drupal.t('LBL746')+'</span>';
        
		if(IsCompliance == 1){
        	mroImageClass = mroImageClassArr['cre_sys_inv_com'];
        }
        else if(MandatoryOption == 'Y'){
        	mroImageClass = mroImageClassArr['cre_sys_inv_man'];
        } else {
        	mroImageClass = (mro == '' || mro == null) ? '' : mroImageClassArr[mro_id];
        }

		if(session_id == null) {
			session_id ='';
		}
		if (courseScore && courseScore != 0 && courseScore != '0' && dataRegStatusCode=="lrn_crs_cmp_cmp") {
		    classScore = Drupal.t("LBL668") +' : ' + courseScore;
		}
		if (quizStatus && quizStatus != "" && dataRegStatusCode == "lrn_crs_cmp_cmp") {
			quizStatus = Drupal.t("LBL1284") +' : ' + Drupal.t(quizStatus);
		}
		var extendedClass = '';
		switch (baseType) {
			case "WBT":
			case "VOD":
				extendedClass = 'webvod_cls_code';
				break;
			case "ILT":
			case "VC":
				extendedClass = 'iltvc_cls_code';
				break;
			default: 
				extendedClass = ''
				break;	
		}
		if((baseType=="WBT" || baseType=="VOD") && ((dataRegStatusCode=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") || (dataRegStatusCode=="lrn_crs_cmp_inp"))) {
			  var i=0;
				var allCntId = [];
				for(j=0; j < rowObject.launch.length; j++){
				  allCntId[j] = rowObject.launch[j].ContentId;
				}
				uniqueCntId = $.unique(allCntId);
				if(uniqueCntId.length == 1 && rowObject.launch[0].ValidityDays !=''){
					var remValidityDays = (rowObject.launch[0].daysLeft !== undefined && rowObject.launch[0].daysLeft != null) ? rowObject.launch[0].daysLeft : '';
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
				if(uniqueCntId.length == 1 && rowObject.launch[0].AttemptLeft !='' && rowObject.launch[0].AttemptLeft !='notset'){
					contValidateMsg			= rowObject.launch[0].contValidateMsg;
					AttemptLeft = Drupal.t("LBL202")+' : '+rowObject.launch[0].AttemptLeft;
				}
				if(uniqueCntId.length == 1)
					VersionNo = Drupal.t("LBL1123")+' : '+rowObject.launch[0].VersionNum;
					
			  	if(dateValidTo!='' && dateValidTo!=null){
				  var daystocount = new Date(dateValidTo);
				  var srvDate = rowObject.launch[0].server_date_time;
				  today=new Date(srvDate);
				  var oneday=1000*60*60*24;
				  daysRemaining = (Math.ceil((daystocount.getTime()-today.getTime())/(oneday)));
				  if(daysRemaining<0){
					  dayRemainVal  = Drupal.t("Expired");
				  }else {
					  dayRemainVal = Drupal.t("LBL677")+': '+(daysRemaining);
				  }
			  	}
			  	daysLeft = (rowObject.launch.length > 0 && rowObject.launch[0].daysLeft != undefined && rowObject.launch[0].daysLeft != '' ) ? rowObject.launch[0].daysLeft : '';
			  //	daysLeft = rowObject.launch[0].daysLeft;
		}
		var obj = options.colModel.widgetObj;
		var LocationNameArg = ((LocationName != null) ? ((unescape(LocationName).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationName);
		var LocationAdd1Arg = ((LocationAddr1!= null) ? ((unescape(LocationAddr1).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr1);
		var LocationAdd2Arg = ((LocationAddr2 != null) ? ((unescape(LocationAddr2).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationAddr2);
		var LocationcityArg = ((LocationCity != null) ? ((unescape(LocationCity).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : LocationCity);
		var data1="data={'RegId':'"+dataId+"','classCode':'"+escape(classCode)+"','BaseType':'"+baseType+"','shortDescription':'"+escape(shortDescription)+"','isTitle':'1','title':'"+escape(classTitle)+"','rowId':'"+dataId+"','CourseId':'"+courseId+"'," +
				"'show_events':'"+show_events+"','coursetempid':'"+courseTempId+"','regstatus':'"+dataRegStatusCode+"','regstatusDate':'"+RegStatusDate+"','regStatusCode':'"+RegStatusCode+"','classId':'"+classId+"','deliverytype':'"+dataDelTypeCode+"','daysRemaining':'"+daysRemaining+"'," +
				"'dataDelType':'"+dataDelType+"','SessStartDay':'"+sessionStartDay+"','startDateFormat':'"+startDateFormat+"','SessStartDate':'"+SessStartDate+"','SessEndDate':'"+SessEndDate+"','courseComplitionStatus':'"+courseComplitionStatus+"','courseScore':'"+courseScore+"','detailTab':'false'," +
				"'regDate':'"+regDate+"','compDate':'"+compDate+"','updateDate':'"+updateDate+"'," +
				"'isCompliance':'"+IsCompliance+"',"+"'remDays':'"+remDays+"',"+
				"'complianceCompleteDays' :'"+compliance_compl_days+"',"+"'complianceCompleteDate' :'"+compliance_compl_date+"',"+
				"'complianceValidityDays' :'"+compliance_val_days+"',"+"'complianceValidityDate' :'"+compliance_val_date+"',"+
				"'complianceExpire' : '"+compliance_expire+"',"+
					"'session_id' : '"+session_id+"',"+
					"'usertimezonecode' : '"+usertimezonecode+"',"+
					 "'user_tzcode' : '"+user_tzcode+"',"+
				"'complianceExpiredOnValidity' : '"+compliance_expired_on_validity+"',"+
				"'contValidateMsg':'"+AttemptLeft+"','dayRemainVal':'"+dayRemainVal+"','daysLeft':'"+daysLeft+"',"+"'LessonLocation':'"+LessonLocation+"',"+
				"'launch_data':'"+launch_data+"','suspend_data':'"+suspend_data+"','exit':'"+exit+"',"+
				"'LocationName':'"+LocationNameArg+"','LocationId':'"+LocationId+"','LocationAddr1':'"+LocationAdd1Arg+"','LocationAddr2':'"+LocationAdd2Arg+"','LocationCity':'"+LocationcityArg+"','LocationState':'"+LocationState+"','LocationCountry':'"+LocationCountry+"','LocationZip':'"+LocationZip+"','LocationPhone':'"+LocationPhone+"'," +
				"'FullName':'"+FullName+"','MandatoryOption':'"+MandatoryOption+"','Managerid':'"+Managerid+"','UpdatedBy':'"+UpdatedBy+"','CreatedBy':'"+CreatedBy+"','UpdatedByName':'"+UpdatedByName+"','mro_type':'"+mro+"','assigned_by':'"+assigned_by+"','updated_by_ins_mngr_slf':'"+updated_by_ins_mngr_slf+"','created_by_ins_mngr_slf':'"+created_by_ins_mngr_slf+"',"+
				"'is_last_rec':'"+isLastRec+"',"+"'courseTitle':'"+escape(courseTitle)+"',"+"'is_exempted':'"+is_exempted+"',"+"'exempted_by':'"+exempted_by+"',"+"'exempted_on':'"+exempted_on+"',"+"'version_no':'"+VersionNo+"'}";

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
		var classSwitch = (baseType=="WBT" || baseType=="VOD") ? 'item-title-wbt' : 'item-title';

		//Enrollment Title Restricted to 20 Character
		var enrollFulltitle = unescape(classTitle);
		var isMRO = '';
		if(mroImageClass != '') {
			isMRO = ' enroll-course-title-mro';
		}
		html += '<div class="limit-title-row enroll-course-title'+isMRO+'">';
		html += '<span id="class_attachment_'+dataId+'" data="'+attachdata+'"></span>';
		var style = '';
		var sessLength = rowObject.sessionDetails.length;
			if(sessLength>1) {
				var titleClass = 'exp-sp-lnrenrollment-enrollmulses';
			}else{
				var titleClass = 'exp-sp-lnrenrollment-enrollsinses';
			}
		if(this.currTheme == "expertusoneV2"){
		   style = "style=\"display:none;\"";
		  }
		html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="title_close"  '+style+'  onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>&nbsp;</a>';
		html += '<span id="titleAccEn_'+dataId+'" class="'+classSwitch+' vtip" ><span class="limit-title limit-title-enr enroll-class-title vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
		html += '<span class="'+titleClass+'"/>';
		//if(baseType=="WBT" || baseType=="VOD"){
		//	if(Drupal.settings.mylearning_right===false)
				html += enrollFulltitle;
		//	else
			//	html += enrollFulltitle;
		//}
	/*	else{
			var sessLength = rowObject.sessionDetails.length;
			if(sessLength>1) {
				var titleClass = 'exp-sp-lnrenrollment-enrollmulses';
			}else{
				var titleClass = 'exp-sp-lnrenrollment-enrollsinses';
			}
			if(Drupal.settings.mylearning_right===false)
				html += titleRestrictionFadeoutImage(enrollFulltitle,titleClass,47);
			else
				html += titleRestrictionFadeoutImage(enrollFulltitle,titleClass,15);
		}*/
		if(is_exempted != 1)
			html += '</span>'+mroImageClass+'</span>';
		
		html += '</div>';

		var startTime = rowObject['session_start'];
		var startDate = rowObject['session_start_format'];
		var baseType  = rowObject['basetype'];
		var classId	  = rowObject['class_id'];
	 //click view button open close TP Description
		var viewcount = 0;
		$('#class-accodion-'+dataId).live("click",function(){
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


        
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			var usertimezonecode = rowObject['usertimezonecode'];
			var session_code = rowObject.sessionDetails[0].session_code;
			var user_tzcode = rowObject['user_tzcode'];
			
			if(sessLen>1) {
			
				sessLenEnd = sessLen-1;
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				var sTime =  rowObject.sessionDetails[0].session_start_format;
				var sTimeForm =  rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
				var eTimeForm =rowObject.sessionDetails[sessLenEnd].session_end_time_form;
				var ecode = rowObject.sessionDetails[0].tz_code;
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';

				if(baseType =="ILT"){
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
				}
				var uTime = rowObject.sessionDetails[0].ilt_session_start_format ;
			    var uTimeForm = rowObject.sessionDetails[0].ilt_session_start_time_form;
			    var rTime = rowObject.sessionDetails[sessLenEnd].ilt_session_end_format;
			    var rTimeForm = rowObject.sessionDetails[sessLenEnd].ilt_session_end_time_form;
			    startDateForTitleloc = uTime+' '+'<span class="time-zone-text">'+uTimeForm+'</span>'+' to '+rTime+' <span class="time-zone-text">'+rTimeForm+'</span>'+' '+session_code+' '+ecode;
			} else {
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				var sTime =  rowObject.sessionDetails[0].session_start_format;
				var sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[0].session_start_end_format;
				var eTimeForm = rowObject.sessionDetails[0].session_end_time_form;
				var ecode = rowObject.sessionDetails[0].tz_code;
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';

				if(baseType =="ILT"){
				startDateForTitle = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+usertimezonecode+' '+user_tzcode;
				}
				var uTime = rowObject.sessionDetails[0].ilt_session_start_format;
			    var uTimeForm = rowObject.sessionDetails[0].ilt_session_start_time_form;
			    var rTime = rowObject.sessionDetails[0].ilt_session_start_end_format;
			    var rTimeForm = rowObject.sessionDetails[0].ilt_session_end_time_form;
			    startDateForTitleloc = uTime+' '+'<span class="time-zone-text">'+uTimeForm+'</span>'+' to '+rTime+' <span class="time-zone-text">'+rTimeForm+'</span>'+' '+session_code+' '+ecode;
			}
			var inc = 0;
			var passParams = "[";
        
			$(rowObject.sessionDetails).each(function(){
				inc=inc+1;

				var sessionfacName = (($(this).attr("session_name") != null) ? ((unescape($(this).attr('session_name')).replace(/"/g, '&quot;')).replace(/'/g, "\\'")) : $(this).attr("session_name"));
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
				 passParams +=  "'sessionEDate':'"+$(this).attr("session_start_end_format")+"',";
				 passParams += "'sessionSDayForm':'"+$(this).attr("session_start_time_form")+"',";
				 passParams += "'sessionEDayForm':'"+$(this).attr("session_end_time_form")+"',";
              // if(baseType =="ILT"){
                passParams += "'sessiontimezone':'"+$(this).attr("sess_timezone")+"'," ;
                 passParams += "'usertimezone':'"+$(this).attr("user_timezone")+"'," ;
               passParams += "'sessionId':'"+$(this).attr("session_id")+"'," ;
                passParams += "'sessionDayloc':'"+$(this).attr("ilt_session_start_day")+"'," ;
              passParams += "'sessionDateloc':'"+$(this).attr("ilt_session_start_format")+' to '+$(this).attr("ilt_session_start_end_format")+"'," ;
                 passParams += "'sessionSDateloc':'"+$(this).attr("ilt_session_start_format")+"'," ;
                passParams += "'sessionEDateloc':'"+$(this).attr("ilt_session_start_end_format")+"'," ;
                passParams += "'sessionSDayFormloc':'"+$(this).attr("ilt_session_start_time_form")+"'," ;
                passParams += "'sessionEDayFormloc':'"+$(this).attr("ilt_session_end_time_form")+"'," ;
                passParams += "'usertimezonecode':'"+usertimezonecode+"'," ;
               passParams += "'session_code':'"+$(this).attr("session_code")+"'," ;
               passParams += "'user_tzcode':'"+$(this).attr("user_tzcode")+"'," ;
                passParams += "'tz_code':'"+$(this).attr("tz_code")+"'," ;
               // }
                 
                 
                 
				 //passParams += "'sessionfacName':'"+$(this).attr("session_name")+"',";
				 passParams += "'sInsName':'"+sInsName+"',";
				 passParams += "'sessionfacName':'"+sessionfacName+"',";
				 passParams += "'sessionfacAddress1':'"+sessionfacAddress1+"',";
				 passParams += "'sessionfacAddress2':'"+sessionfacAddress2+"',";
				 passParams += "'sessionfacCountry':'"+escape($(this).attr("session_country"))+"',";
				 passParams += "'sessionfacState':'"+escape($(this).attr("session_state"))+"',";
				 passParams += "'sessionfacCity':'"+sessionfacCity+"',";
				 passParams += "'sessionfacZipcode':'"+$(this).attr("session_zipcode")+"'";
				 passParams += "}";

				 if(inc < sessLen) { 
					 passParams += ",";
				 }
			});
			passParams += "]";
			/*if(baseType =="ILT"){
			html += '<div id=sessdetails><span class="session-time-zone" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDateForTitle,'exp-sp-lnrenrollment-timezone', 30) +'</span>';
       }
			else{
			html += '<div id=sessdetails><span class="session-time-zone" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+startDateForTitle+'</span>';

			}*/// alert(rowObject.sessionDetails[0].sess_timezone);
          //alert(rowObject.sessionDetails[0].user_timezone);
            /*if(baseType =="ILT" && rowObject.sessionDetails[0].sess_timezone!=rowObject.sessionDetails[0].user_timezone){
            	html += qtip_popup_paint(classId,startDateForTitleloc,sessLen); 
            }*/
		//else{
			//html += '<span>&nbsp;</span>';
		//}
		}
		//else{
		//	html += '<span>&nbsp;</span>';
		//}
		html += '<div class="enroll-class-title-info">';
		html += '<div class="item-title-code '+ extendedClass +'">';

		var statusDate = '';
		var statusName = '';

		if(RegStatusCode == 'lrn_crs_cmp_enr') {
		/*	if((complianceCompDate != '' && complianceCompDate != null)){
				statusDate = complianceCompDate;
				statusName = Drupal.t('LBL234');

			}else{*/

				statusDate = regDate;
				statusName = Drupal.t('LBL704');
			//}
		}else if(RegStatusCode == 'lrn_crs_cmp_cmp') {
			statusDate = compDate;
			statusName = Drupal.t('LBL027');
		}else if(RegStatusCode == 'lrn_crs_cmp_inc') {
			statusDate = compDate;
			statusName = Drupal.t('LBL1193');
		}else if(RegStatusCode == 'lrn_crs_reg_can') {
			statusDate = updateDate;
			statusName = Drupal.t('LBL026');
		}else if(RegStatusCode == 'lrn_crs_reg_ppm') {
			statusDate = regDate;
			statusName = Drupal.t('LBL704');
		}else if(RegStatusCode == 'lrn_crs_cmp_nsw') {
		    statusDate = compDate;
		    statusName = Drupal.t('LBL704');
	    }else{
			statusDate = regDate;
			statusName = Drupal.t('LBL704');
		}
		//Pipe Sysmbol use this variable 'pipe'
		var pipe = '<span class="enroll-pipeline">|</span>';
		
		html += '<div class="line-item-container float-left" ><span class="vtip" title="' + statusName + ': ' + statusDate + '">' + titleRestrictionFadeoutImage(statusName + ' : ' + statusDate,'exp-sp-lnrenrollment-status',200)+'</span></div>';
		
		if((RegStatusCode == 'lrn_crs_cmp_enr' && complianceCompDate != '' && complianceCompDate != null)){
                statusDate = complianceCompDate;
                statusName = Drupal.t('LBL234');
                html += '<div class="line-item-container float-left">'+pipe;
                html += '<span class="vtip" title="' + statusName + ': ' + statusDate + '">' + titleRestrictionFadeoutImage(statusName + ' : ' + statusDate,'exp-sp-lnrenrollment-status', 50) + '</span></div>';
            }
//		html += pipe;
//		if(Drupal.settings.mylearning_right===false)
//			html += '<span class="vtip" title="' + Drupal.t('LBL096') + ': ' + unescape(classCode).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(classCode,'exp-sp-lnrenrollment-classCode', 50) + '</span>'; // Code
//		else
//			html += '<span class="vtip" title="' + Drupal.t('LBL096') + ': ' + unescape(classCode).replace(/"/g, '\&quot;') + '">' + titleRestrictionFadeoutImage(classCode,'exp-sp-lnrenrollment-classCode', 5) + '</span>'; // Code
		if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_cmp' || RegStatusCode == 'lrn_crs_cmp_inp') {
		var progress = '';
		/* Viswanathan uncommented the below code to display tracking percentage*/
			var progressPercent = 0;
			if(baseType == 'VOD') {
				 for(j=0; j < rowObject.launch.length; j++){
                     var suspendData = (rowObject.launch[j].SuspendData != undefined && rowObject.launch[j].SuspendData != null && rowObject.launch[j].SuspendData != '') ? JSON.parse(unescape(rowObject.launch[j].SuspendData)) : null;
                     progressPercent = progressPercent + (suspendData != null && !isNaN(parseFloat(suspendData['progress'])) ? parseFloat(suspendData['progress']) : 0);
				 }
				 progress += Math.round(progressPercent/rowObject.launch.length) + '% '+Drupal.t('Completed');
				 html += ((progress != '')) ? '<div class="line-item-container float-left">'+(pipe) : '';
				 html += ((progress != '') ? '<span class="vtip" title="'+progress+'">'+(titleRestrictionFadeoutImage(progress,'exp-sp-lnrenrollment-percentCompletion'))+'</span></div>' : '');
			}/* Viswanathan uncommented the below code to display tracking percentage*/
				 }
		var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
		 var clslocation = 'clsLocationSingle';
		  var clssession = 'clsSessionSingle';
		  if(sessLen>1){
			  clslocation = 'clsLocationMultiple';
			  clssession = 'clsSessionMultiple';
			}
	
		var newdelType;
		if(dataDelTypeCode=='lrn_cls_dty_vcl' && RegStatusCode == 'lrn_crs_cmp_att') {
                  newdelType = Drupal.t('Attended');
                  html += pipe+newdelType;
				}		
		var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
		if(ClassLoc && (RegStatusCode != 'lrn_crs_cmp_cmp') ) {
		
		  if(Drupal.settings.mylearning_right===false){
			 
				html += '<div class="line-item-container float-left" >'+pipe+'<span class="vtip '+clslocation+'" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrenrollment-classloc',20)+'</span></div>';
			    //html += pipe+'<span class="session-time-zone '+clssession+'" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+');\'>'+startDateForTitle+'</span>';
				
			}else
				html += pipe+'<span class="vtip" title="'+unescape(ClassLoc).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-lnrenrollment-classloc',9)+'</span>';
		}
		if(passParams && (baseType =="ILT" || baseType =="VC")&& (RegStatusCode != 'lrn_crs_cmp_cmp') ){
			html += '<div class="line-item-container float-left" >'+pipe+'<span class="session-time-zone '+clssession+'" data=\"data='+passParams+'\" id="session_det_popup'+classId+'" onmouseover=\'$("#learner-enrollment-tab-inner").data("enrollment").getSessionDetails(this,'+classId+',"'+baseType+'");\'>'+titleRestrictionFadeoutImage(startDateForTitle,'exp-sp-lnrenrollment-timezone')+'</span></div>';
		if(baseType =="ILT" && rowObject.sessionDetails[0].sess_timezone!=rowObject.sessionDetails[0].user_timezone)
        	html += qtip_popup_paint(classId,startDateForTitleloc,sessLen); 
		}
		//#73112	
		if(RegStatusCode == 'lrn_crs_cmp_cmp'){			
			if(IsCompliance == 1) {	
				if(compliance_val_date != null && compliance_val_date != ''){
					html += '<div class="line-item-container float-left">'+pipe;
					if(compliance_expire == true){
						html += "<span class='comp-label vtip' title='+Drupal.t('LBL028')+' : ' +compliance_val_date+'>"+titleRestrictionFadeoutImage(Drupal.t("LBL028")+" : " +compliance_val_date,'exp-sp-lnrenrollment-validitydays')+"</span>";
					} else if(compliance_expire == false) {
						html += "<span class='comp-label vtip' title='"+Drupal.t('LBL735')+' : ' +compliance_val_date+"'>"+titleRestrictionFadeoutImage(Drupal.t('LBL735')+" : " +compliance_val_date,'exp-sp-lnrenrollment-validitydays')+"</span>";
					}
					html +='</div>';
				}
			}
		}
		//#73112		
		if(dataRegStatusCode=="lrn_crs_reg_wtl" && waitlist_priority !=''){
		  var wtlCount = SMARTPORTAL.t(rowObject['labelmsg']['msg5']) + ' : '+ waitlist_priority;
		  html += '<div class="line-item-container float-left" >'+pipe+'<span>'+titleRestrictionFadeoutImage(wtlCount,'exp-sp-lnrenrollment-waitlist',200)+'</span></div>';
		}
		if(dataRegStatusCode == "lrn_crs_cmp_cmp" || dataRegStatusCode == "lrn_crs_cmp_inp"  || dataRegStatusCode == "lrn_crs_cmp_enr"){
			//html += pipe;
			html += "<span id='compTabDetails"+dataId+"' class='compTabDetails'>";
			html += ((AttemptLeft != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+AttemptLeft+'">'+titleRestrictionFadeoutImage(AttemptLeft,'exp-sp-lnrenrollment-attemptsleft',200))+'</span></div>' : '');
			//html += ((ValidityDays != '' && AttemptLeft != '') ? (pipe) : '');
			html += ((ValidityDays != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+ValidityDays+'">'+titleRestrictionFadeoutImage(ValidityDays,'exp-sp-lnrenrollment-validitydays',200))+'</span></div>' : '');
		//	html += ((( ValidityDays != '' || AttemptLeft != '' )) ? (pipe) : '');
			//html += ((classScore != '') ? (classScore) : '');
		//	html += ((VersionNo != '' && ( ValidityDays != '' || AttemptLeft != '' )) ? (pipe) : '');
			html += ((VersionNo != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+VersionNo+'">'+titleRestrictionFadeoutImage(VersionNo,'exp-sp-lnrenrollment-version',200))+'</span></div>' : '');
			if(dataRegStatusCode == "lrn_crs_cmp_cmp") {
				html += ((classScore != '') ? '<div class="line-item-container float-left">'+(pipe+'<span class="vtip" title="'+classScore+'">'+titleRestrictionFadeoutImage(classScore,'exp-sp-lnrenrollment-score',200))+'</span></div>' : '');
				html += ((quizStatus != '' && ( ValidityDays != '' || AttemptLeft != '' || classScore != '' || progress != '')) ? '<div class="line-item-container float-left">'+(pipe) : '');
				html += ((quizStatus != '' && ( ValidityDays == '' && AttemptLeft == '' && classScore == '' && progress == '')) ? pipe : '');
				html += ((quizStatus != '') ? '<span class="vtip" title="'+quizStatus+'">'+titleRestrictionFadeoutImage(quizStatus,'exp-sp-lnrenrollment-quizstatus')+'</span></div>' : '');
			}
			html += "</span>";
		}else{
			html += ((AttemptLeft != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(AttemptLeft,'exp-sp-lnrenrollment-attemptsleft',200))+'</div>' : '');
			html += ((ValidityDays != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(ValidityDays,'exp-sp-lnrenrollment-validitydays',200))+'</div>' : '');
			html += ((classScore != '') ? '<div class="line-item-container float-left">'+(pipe+titleRestrictionFadeoutImage(classScore,'exp-sp-lnrenrollment-score',200))+'</div>' : '');
		}
		html += ' </div>';
		//var displayDesc = addExpanColapse(decodeURIComponent(shortDescription),decodeURIComponent(descriptionFull),dataDelTypeCode,'ME',dataId);
		var displayDesc =decodeURIComponent(shortDescription) && decodeURIComponent(descriptionFull);
		var crsMoreType= dataDelTypeCode;
		html += '<div class="limit-desc-row ' +crsMoreType+'">';
		html += '<span class="limit-desc limit-desc-enr vtip" id="classShortDesc_'+dataId+'"><span class="cls-learner-descriptions">'+displayDesc+'</span></span>';
		html += '</div>';
		html += '<div class="cp_seemore" id="seemore_'+dataId+'" onclick="seeMoreMyLearning('+dataId+',\'myenrollmentclass\');">'+ Drupal.t('LBL713') +'</div>';
		html += '</div>';

		return html;
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

	getSessionDetails : function(sessData,sessionId,baseType) {
		try{
		var data= eval($(sessData).attr("data"));
		var sessionDet = '';
    
		if(!document.getElementById("session_det"+sessionId)) {
			var sessionDet1 = '';
			$(data).each(function(){
			if(baseType == "ILT")
				sessionDet1 += "<span class='qtip_session enroll-session-time time-zone-ilt'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> "+"  "+$(this).attr("usertimezonecode")+" "+$(this).attr("user_tzcode")+"</span>";
			else
			sessionDet1 += "<span class='qtip_session enroll-session-time'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> </span>";
			
			});
			$('#session_det_popup'+sessionId).qtip({
				// content: '<div id="session_det'+sessionId+'" class="tooltiptop"></div><div class="tooltipmid">'+sessionDet1+'</div><div class="tooltipbottom"></div>',
				 content: '<div id="session_det'+sessionId+'" class="qtip_session_top tooltiptop"></div><div class="qtip_session_mid tooltipmid">'+sessionDet1+'</div><div class="qtip_session_bottom tooltipbottom"></div>',
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
				position: {
				    corner: {
				       target: 'bottomLeft',
				       tooltip: 'topLeft'
				    }
				},
				style: {
					width: 325,
					background: 'none',
					border:'0',
					color: '#333333'
				}
			});
			
		}
		}catch(e){
			// to do
		}
	},

	getClassDetails : function(catdiv,accordionLink,parentObj){
		try{
		var data= eval(accordionLink.attr("data"));
		$('#openclose_enr_'+data.rowId).removeClass('title_open');
		$('#openclose_enr_'+data.rowId).addClass('title_close');
		var ostr = '';
        $('#ClassDetailsMainDiv').remove();
        ostr += parentObj.paintDetailsClassSection(data);
		catdiv.html(ostr);
		catdiv.css('display','block');
		$('.dt-child-row-En > td').attr('colSpan','6');
		$('.enrollment-class-table tr td.enroll-class-schedule').css('padding','0');
		}catch(e){
			// to do
		}
	},

	getLocationDetails : function(data) {
		try{
		var data= eval($(data).attr("data"));
		var enrollId	   = data.enrollId;
		var LocationName   = data.LocationName;
		var LocationId	   = data.LocationId;
		var locAddr1 	   = unescape(data.LocationAddr1);
		var locAddr2 	   = unescape(data.LocationAddr2);
		var locCity		   = unescape(data.LocationCity);
		var locState 	   = unescape(data.LocationState);
		var locCountryName = unescape(data.LocationCountry);
		var locZip 		   = data.LocationZip;
		var locPhone       = data.LocationPhone;

		var inQtip 		= $.inArray(enrollId,this.qtipListArr);
		if((inQtip == -1)) {
			this.qtipListArr.push(enrollId);
			var locValue = '';
			locValue += "<span class='enroll-location-details'>"+LocationName+"</span>";
			if(locAddr1 !='' && locAddr1 != null) {
				locValue += "<span class='enroll-location-details'>"+locAddr1+"</span>";
			}
			if(locAddr2 !='' && locAddr2 != null) {
				locValue += "<span class='enroll-location-details'>"+locAddr2+"</span>";
			}
			if(locCity !='' && locCity != null) {
				locValue += "<span class='enroll-location-details'>"+locCity+"</span>";
				if (locState == '' && locState != null) {
					locValue += "<br />";
				}
			}
			if(locState !='' && locState != null) {
				if (locCity != '' && locCity != null) {
					locValue += ", ";
				}
				locValue += "<span class='enroll-location-details'>"+locState+"</span>";
			}
			if(locZip !='' && locZip != null){
				locValue += "<span class='enroll-location-details'>"+locZip+"</span>";
			}
			if(locCountryName !='' && locCountryName != null){
				locValue += "<span class='enroll-location-details'>"+locCountryName+"</span>";
			}

			$('#paint-location'+enrollId).qtip({
				 content: locValue,
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
				position: {
				    corner: {
				       target: 'bottomRight',
				       tooltip: 'topLeft'
				    }
				},
				style: {
					width: 250,
					background: '#dfe5eb',
					border: {
						radius: 10,
						width: 5,
						color: '#dfe5eb'
					},
					padding:5,
					tip: true, // Give it a speech bubble tip with automatic corner detection
					name: 'cream', // Style it according to the preset 'cream' style
					color: '#333333'
				}
			});
		}
		}catch(e){
			// to do
		}
	},

	paintDetailsClassSection : function(data){
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
		var sessionId = data.session_id;
		var user_tzcode = data.user_tzcode;
		var startDateFormat = data.startDateFormat;
		var updated_by_ins_mngr_slf = data.updated_by_ins_mngr_slf;
		var created_by_ins_mngr_slf = data.created_by_ins_mngr_slf;
		var baseType = data.BaseType;
		var regStatusCode = data.regStatusCode;
		var regstatusDate = data.regstatusDate;
		var show_events   = data.show_events;
		var dayRemainVal = data.dayRemainVal;
		var complianceCompleteDays = data.complianceCompleteDays;
		var complianceCompleteDate = data.complianceCompleteDate;
		var complianceValidityDays = data.complianceValidityDays;
		var complianceValidityDate = data.complianceValidityDate;
		var complianceExpire	   = data.complianceExpire;
		var complianceExpiredOnValidity = data.complianceExpiredOnValidity;
		var isCompliance		   = data.isCompliance;
		var fullName			   = data.FullName;
		var courseTitle 		= data.courseTitle;
		var noBorderLastRecClass = ($("#pager").is(":visible") || data.is_last_rec != 'last')? '' : 'noborder';
		var Obj = $("#learner-enrollment-tab-inner").data("enrollment");
		var userId = this.enrUserId;
		var RegStatusCode		= data.regStatusCode;
		var regDate				= data.regDate;
		var compDate			= data.compDate;
		var updateDate			= data.updateDate;
		var dateComnplete    = '';
		var dateEnrolled    = '';
		var isExtempted = data.is_exempted;
		var extemptedBy = data.exempted_by;
		var extemptedOn = data.exempted_on;

		eval(complianceExpire);


		// Class details main div
		ostr += '<td colspan="3"><div id="ClassDetailsMainDiv" class="enroll-accordian-div ' + noBorderLastRecClass + '"> <div class="session-details-course-row-innertbl main-item-container">';

		//Session & Location detail section -- start
		/*if(document.getElementById("session_det_popup"+classId)) {
			var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			ostr += '<div class="enroll-session-details session-row"><span class="session-label">'+Drupal.t("LBL670")+' :</span><div class="session-desc">';
			var inc = 1;
			var LocationName = '';
			LocationName 	= dataInfo.LocationName;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
		if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
		else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				//ostr += '<div class="sessionDet"><div class="sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				ostr += '<div class="sessionDet"><div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				ostr += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
				//ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				ostr += '<div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				inc++;
			});
			ostr += '</div></div>';
			var sesinc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					if(LocationName!='null' && LocationName!=''){
					ostr += '<div class="sessAddDet location-row"><span class="location-label">'+Drupal.t("Location")+' :</span>';
					ostr += '<div class="location-desc"><span class="name-add">'+LocationName+',</span>';
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
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+',</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					if(LocationName!='null' && LocationName!=''){
		ostr += '</div>';
					}
					}

			});
		}*/

		// Coursee Title section -- start
		ostr += '<div class="enroll-session-details course-row line-item-container">';
		//if(Drupal.settings.mylearning_right===false)
			ostr += '<span class="wbt-course-title course-label container">'+ Drupal.t("Course")+' '+Drupal.t("LBL083")+ ' : </span><div class="course-desc"><span class="vtip" title="'+unescape(htmlEntities(courseTitle))+'">'+titleRestrictionFadeoutImage(unescape(htmlEntities(courseTitle)),'exp-sp-lnrenrollment-wbt-course-title',70)+'</span></div>';  // course title
//		else
//			ostr += '<span class="wbt-course-title course-label container">'+ Drupal.t("Course")+' '+Drupal.t("LBL083")+ ' : </span><div class="course-desc"><span class="vtip" title="'+saniztizeJSData(courseTitle)+'">'+titleRestrictionFadeoutImage(saniztizeJSData(courseTitle),'exp-sp-lnrenrollment-wbt-course-title',35)  +'</span></div>';  // course title
		ostr += '</div>';
		// Coursee Title section -- end
		var dataDesc = "data={'ClassId':'"+classId+"','ClassDesc':'"+shortDescription+"'}";
		var data1 = "data={'classId':'"+classId+"','courseId':'"+courseId+"','regStatusCode':'"+regStatusCode+"'}";

		//Enrollment detail section -- start


			if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_inp' || RegStatusCode =='lrn_crs_cmp_att'){
				if(isExtempted == 1){
					ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('Waived') +" " + Drupal.t("by") +" : " +  "</span><div class='enr-status-cls status-desc'>" +  extemptedBy +" "+Drupal.t("LBL945")+" " +extemptedOn+'</div></div>';
				}else{
					if(isCompliance == 1) {
			        	if(dataInfo.CreatedBy == 0) {
			        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025') +" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';

			        	}else {
			        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025') +" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			        	}
			        	//0041143: For this ticket ,this part is important
			        	/*if(complianceExpire == 'true'){
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t("LBL028")+" : " +"</span>"+complianceExpiredOnValidity;
						} else if(complianceExpire == 'false') {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL735')+" : " +"</span>"+complianceExpiredOnValidity;
						} else {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL234')+" : " +"</span>"+complianceExpiredOnValidity;
						}*/

			        }else {
			        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName :dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
				}
			        
			}else if(RegStatusCode == 'lrn_crs_cmp_cmp'){
				if(isCompliance == 1) {
					/*if(complianceValidityDate != null && complianceValidityDate != ''){
						//	0043140: Modified by Priya.
						if(complianceExpire == 'true'){
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t("LBL028")+" : " +"</span>"+complianceValidityDate;
						} else if(complianceExpire == 'false') {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL735')+" : " +"</span>"+complianceValidityDate;
						} /*else {
							ostr += "<div class='page-learning-enrollment-compliance'><span class='comp-label container'>"+Drupal.t('LBL234')+" : " +"</span>"+complianceCompleteDays;
						}*/
					//}
					if(dataInfo.CreatedBy == 0) {
						ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';
					} else {
						ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
					//ostr += "<span title='"+updated_by_ins_mngr_slf+"'>";
					ostr += "<div class='status-row line-item-container vtip' title='"+updated_by_ins_mngr_slf+"'><span class='status-label container"+((userId != dataInfo.UpdatedBy) ? ' markcompletestatus' : ' completedby')+"'>"+((userId != dataInfo.UpdatedBy) ? Drupal.t('LBL747')+' '+Drupal.t('by') : Drupal.t('LBL681'))+" : " + "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +compDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
					//ostr += "</span>";
					ostr += "</div>";
				}else{
					ostr += "<div class='status-row line-item-container'><span class='status-label container"+((userId != dataInfo.UpdatedBy) ? ' markcompletestatus' : ' completedby')+"'>"+((userId != dataInfo.UpdatedBy) ? Drupal.t('LBL747')+' '+Drupal.t('by') : Drupal.t('LBL681'))+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +compDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.UpdatedByName #0073510
					ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				}
			}else if(RegStatusCode == 'lrn_crs_cmp_inc' || RegStatusCode == 'lrn_crs_cmp_nsw'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL1193')+" : " +  "</span><span class='enr-status-cls status-desc'>" +compDate+'</span></div>';
				if(isCompliance == 1) {
		        	if(dataInfo.CreatedBy == 0) {
		        		ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" +  Drupal.t('LBL432') +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';
		        	}else {
			        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
					}
		        }else {
		        	ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
		        }
			}else if(RegStatusCode == 'lrn_crs_reg_can'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL680')+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.UpdatedBy) ? dataInfo.UpdatedByName : dataInfo.UpdatedByName) +" "+Drupal.t("LBL945")+" " +regstatusDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><div class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</div></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			}else if(RegStatusCode == 'lrn_crs_reg_ppm' || RegStatusCode == 'lrn_crs_reg_wtl'){
				ostr += "<div class='status-row line-item-container'><span class='status-label container'>"+ Drupal.t('LBL025')+" : " +  "</span><span class='enr-status-cls status-desc'>" + ((userId != dataInfo.Managerid) ? dataInfo.FullName : dataInfo.FullName) +" "+Drupal.t("LBL945")+" " +regDate+'</span></div>';//Viswanthan replaced LBL3046 to datainfo.fullName #0073510
			}
		/*if(dayRemainVal != ''){
		ostr += '<div class="days-remain"><span>'+dayRemainVal+'</span></div>';
		}*/

			//Attachment detail section -- start
			/*var attachData = eval($("#class_attachment_"+enrollId).attr("data"));
			if(attachData.length > 0) {
				ostr += '<div class="enroll-attach attach-row"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+' :'+'</span><span class="attach-info">'+ Drupal.t("LBL232")+'</span></div>';
				ostr += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
				var len = attachData.length;
				var inc = 1;
					$(attachData).each(function(){
							--Issue fix for the ticket - 32781 --
							var attachment_url = unescape(saniztizeJSData($(this).attr('Title')));
						//	if(attachment_url.length > 60) attachment_url = attachment_url.substring(0,60)+"...";
						attachment_url = titleRestrictionFadeoutImage(attachment_url,'exp-sp-lnrenrollment-attachment-name');
							ostr += "<li class='vtip' title='"+unescape(saniztizeJSData($(this).attr('Title')))+"'>"+"<a href='javascript:void(0);' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+attachment_url+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
							inc++;
					});
				ostr += "</ul></div></div>";
			}*/
			//Attachment detail section -- end	

		//Session & Location detail section -- start
		if(document.getElementById("session_det_popup"+classId)) {
			var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			var inc = 1;
			var LocationName = '';
			LocationName 	= dataInfo.LocationName;
			/*$(sessionDet).each(function(){ //sessionDate sessionDay
			var usertimezonecodes= $(this).attr("sessionTitle");
				var locationsess = '<div class="sessDate">'+$(this).attr("sessionSDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayFormloc")+"</span>"+' to '+$(this).attr("sessionEDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayFormloc")+'</span>'+$(this).attr("session_code")+' '+$(this).attr("tz_code")+'</div>' ;
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				ostr += '<div class="sessionDet"><div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div>';
				ostr += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
				if(baseType == "ILT")
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span>'+' '+$(this).attr("usertimezonecode")+' '+user_tzcode;
				else
				ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
	
	
				if(baseType == "ILT" && $(this).attr("sessiontimezone")!= $(this).attr("usertimezone") ){
					ostr += qtip_popup_paint($(this).attr("sessionId"),locationsess,'',1); 
              }
          
            ostr += '</div></div>';
				inc++;
			});*/
			
			var sesinc = 1;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
					{
					sesinc++;
					if(LocationName!='null' && LocationName!=''){
					ostr += '<div class="sessAddDet location-row line-item-container"><span class="location-label container">'+Drupal.t("Location")+' :</span>';
					LocationName =   LocationName + ",";
					if(LocationName.length > 110)
							loctitle = titleRestrictionFadeoutImage(unescape(LocationName),'exp-sp-myenroll-loc',110);
						else
							loctitle = unescape(LocationName);
					ostr += '<div class="location-desc"><span class="name-add"> <span class="vtip" title="'+unescape(htmlEntities(LocationName))+'">'+loctitle+'</span> </span>';
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
						ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+',</span>';
					if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
						ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span></div>';
					if(LocationName!='null' && LocationName!=''){
					ostr += '</div>';
					}
					}

			});
			
			//var sessionDet = eval($("#session_det_popup"+classId).attr('data'));
			ostr += '</div><div class="enroll-session-details session-row main-item-container"><span class="session-label session-container">'+Drupal.t("LBL277")+' :</span><div class="session-desc ">';
			//var inc = 1;
			//var LocationName = '';
			//LocationName 	= dataInfo.LocationName;
			$(sessionDet).each(function(){ //sessionDate sessionDay
				var usertimezonecodes= $(this).attr("sessionTitle");
				var locationsess = '<div class="sessDate">'+$(this).attr("sessionSDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayFormloc")+"</span>"+' to '+$(this).attr("sessionEDateloc")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayFormloc")+'</span>'+$(this).attr("session_code")+' '+$(this).attr("tz_code")+'</div>' ;
				var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
				var sesionInsName = ($(this).attr("sInsName") != '') ? unescape($(this).attr("sInsName")) : "&nbsp;";
				if(Drupal.settings.mylearning_right===false)
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',50);
				else
					var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-lnrenrollment-sesions-head',24);
				    var sesInstName = titleRestrictionFadeoutImage(sesionInsName,'exp-sp-lnrenrollment-instructor-names');
				ostr += '<div class="sessionDet">';
				ostr += '<div class="cls-sessionTitle-container line-item-container"><div class="lbl-sessionTitle sessionDetlbl container">'+Drupal.t("LBL107")+' :'+'</div> <div class="vtip sessName" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div></div>';
				ostr += '<div class="cls-sessDate-container line-item-container"><div class="lbl-sessionDate sessionDetlbl container">'+Drupal.t("LBL042")+' :'+'</div> <div class="session-day-date-container"> <div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
			//	ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div></div>';
				if(baseType == "ILT")
					ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span>'+' '+$(this).attr("usertimezonecode")+' '+user_tzcode+'</div>';
					else
					ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+'</span></div>';
					if(baseType == "ILT" && $(this).attr("sessiontimezone")!= $(this).attr("usertimezone") ){
						ostr += qtip_popup_paint($(this).attr("sessionId"),locationsess,'',1); 
						//ostr += '</div>';
	                }
				ostr += '</div></div>';
				ostr += '<div class="cls-sessionInstructor-container line-item-container"><div class="lbl-sessionInstructor sessionDetlbl container">'+Drupal.t("Instructor")+' :'+'</div> <div class="vtip sessionInstructor-val" title="'+(($(this).attr("sInsName") != '') ? htmlEntities(unescape($(this).attr("sInsName"))) : "")+'">'+sesInstName+'</div></div>';
				ostr += '</div>';
				inc++;
			});
			ostr += '</div></div>';
			
		}

		//Attachment detail section -- start
		var attachData = eval($("#class_attachment_"+enrollId).attr("data"));
		if(attachData.length > 0) {
			ostr += '<div class="enroll-attach attach-row"><div class="wbtClass-attachment-title attach-label"><span class="attach-title">'+Drupal.t("LBL231")+' :'+'</span><div class="attach-info">'+ Drupal.t("LBL232")+'</div></div>';
			ostr += "<div class='attach-desc'><ul class='enroll-attach-listitems'>";
			var len = attachData.length;
			var inc = 1;
				$(attachData).each(function(){
						/*--Issue fix for the ticket - 32781 --*/
						var attachment_url = unescape(saniztizeJSData($(this).attr('Title')));
					//	if(attachment_url.length > 60) attachment_url = attachment_url.substring(0,60)+"...";
					attachment_url = titleRestrictionFadeoutImage(attachment_url,'exp-sp-lnrenrollment-attachment-name');
						ostr += "<li class='vtip' title='"+unescape(saniztizeJSData($(this).attr('Title')))+"'>"+"<a href='javascript:void(0);' name='Attachment' onclick='openAttachmentCommon(\""+$(this).attr('url')+"\")'>"+attachment_url+"</a>"+((len == inc) ? "" : "<span class='enroll-pipeline'>|</span>")+"</li>";
						inc++;
				});
			ostr += "</ul></div></div>";
		}
		//Attachment detail section -- end

		//Session & Location detail section -- end

	

		ostr += '</div></td>';
		return ostr;
		}catch(e){
			// to do
		}
	},

	getClassOption : function(enrollId,regStatusCode) {
	 try{
		if($('#class-option-'+enrollId).html() == ''){
		    this.createLoader('ClassDetailsMainDiv');
			var url = this.constructUrl("ajax/class-option/" + enrollId);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader('ClassDetailsMainDiv');
					$("#class-option-"+enrollId).slideDown(1000);
					obj.classOptionPaint(result,enrollId,regStatusCode);
					$('#show-alternative-events_'+enrollId).css('display','none');
					$('#hide-alternative-events_'+enrollId).css('display','block');
				}
		    });
		}
		else{
			$("#class-option-"+enrollId).slideDown(1000);
			$('#show-alternative-events_'+enrollId).css('display','none');
			$('#hide-alternative-events_'+enrollId).css('display','block');
		}
		}catch(e){
			// to do
		}
	},

	getClassOptionHide : function(enrollId) {
	 try{
		$("#class-option-"+enrollId).slideUp(1000);
		$('#hide-alternative-events_'+enrollId).css('display','none');
		$('#show-alternative-events_'+enrollId).css('display','block');
	 }catch(e){
			// to do
		}
	},

	classOptionPaint : function(data,enrollId,regStatusCode) {
	 try{
		var res_length = data.length;
		var rhtml = "";
		var c;
		var courseId;
		var classId;
		var classCode;
		var ScheduledClassId;
		var NodeId;
		var StartDate;
		var EndDate;
		var clsTitle;
		var DeliveryType;

		var clsAddEven;
		if(res_length > 0 ) {
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-moreevent' cellspacing='0' width='100%'>";
			for(c=0; c < data.length ; c++){
				courseId 	= data[c]['courseid'];
				classId 	= data[c]['classid'];
				classCode 	= unescape(data[c]['code']);
				//ScheduledClassId = data[c]['ScheduledClassId'];
				NodeId 		= data[c]['nodeid'];
				StartDate 	= data[c]['startdateformat'];
				StartTime 	= data[c]['starttime'];
				EndTime 	= data[c]['endtime'];
				DeliveryType 	 = data[c]['deliverytype'];
				clsTitle 	= data[c]['name'];

				StartDate = (StartDate != null)? StartDate : '';
				StartTime = (StartTime != null)? StartTime : '-';
				EndTime   = (EndTime != null)? EndTime : '-';
				var curDay ='-';
				if(StartDate != ''){
					var myDays = ["Sun","Mon","Tue","Wed","Thu","Fri","Sat","Sun"];
					var myDate = new Date(eval('"'+StartDate+'"'));
					curDay = myDays[myDate.getDay()];
				}
				else{
					StartDate = '-';
				}
				clsAddEven = (c % 2 == 0)? 'odd' : 'even';
				var isSwitchLink = '';
				if(regStatusCode == 'lrn_crs_cmp_enr' || regStatusCode == 'lrn_crs_cmp_inp'){
					isSwitchLink = "<a href='javascript:void(0);' class='actionLink' name='Select' onclick='"+this.widgetObj+".switchEnroll("+classId+","+enrollId+",\""+clsTitle+"\");' >"+SMARTPORTAL.t("Select")+"</a>";
				}
		 			rhtml += '<tr class="'+clsAddEven+'">';
					rhtml += '<td class="enrollment-class-name">'+clsTitle+'</td>';
					rhtml += '<td class="enrollment-class-day">'+curDay+'</td>';
					rhtml += '<td class="enrollment-class-date">'+StartDate +'</td>';
					rhtml += '<td class="enrollment-class-start">'+StartTime+'</td>';
					rhtml += '<td class="enrollment-class-end">'+EndTime+'</td>';
					rhtml += '<td class="enroll-show-select">'+isSwitchLink+'</td>';
					rhtml += '</tr>';
		  }
		  rhtml += '</table>';
		}
		else{
			rhtml += "No Events";
		}
		$("#class-option-"+enrollId).html(rhtml);
		$('.enroll-show-moreevent tr:last').css('border-bottom','0px');
	 }catch(e){
			// to do
		}
	},

	switchEnroll : function(classId,enrollId,classTitle){
		try{
		if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != ""){
			this.confirmPricedEvent(classId,classTitle);
		}
		else{
			this.createLoader('class-option-'+enrollId);
			var url = this.constructUrl("ajax/switch-enroll/" + classId + "/" + enrollId);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					obj.destroyLoader('class-option-'+enrollId);
					obj.callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
				}
		    });
		}
		}catch(e){
			// to do
		}
	},

	confirmPricedEvent : function(cid,vTitle){
		try{
		 $('#div-confirm-price').remove();
		var message  = "<div id='div-confirm-price' >";
			message += "<div class='dialogInner'>" + Drupal.t('MSG399')+"<br />"+Drupal.t('MSG400')+"</div>";
			message += "</div>";

		$('body').append(message);
		var closeButt={};
		closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#div-confirm-price').remove();};
		$("#div-confirm-price").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:vTitle,
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
		$("#div-confirm-price").show();
		$('.ui-dialog-titlebar-close').click(function(){
			$("#div-confirm-price").remove();
		});
		}catch(e){
			// to do
		}
	},

	getLaunchDetails : function(enrollId) {
		try{
		var passdata = $('#launch'+enrollId).attr('data');
		var data = eval($('#launch'+enrollId).attr('data'));
		var html = '<div id="launch-content-container">';
		html += '<table id="paintEnrollmentResults" cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-launch-table">';
		var lesCnt = 0;
		var obj = this.widgetObj;
		var widgetObj = this;
		var contentArr = new Array();
		$(data).each(function(){
			$(this).each(function(){
				var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				var launchFrom = '';
				if(this.LaunchFrom =='CL')
					var launchFrom = $(this).attr('LaunchFrom');
				var contValidateMsg = $(this).attr('contValidateMsg');
				var contentType = $(this).attr('launchType');
		          var contentId = $(this).attr('ContentId');
		          var VersionId = $(this).attr('VersionId');
				  var lcnt = $(this).attr('Lessoncnt');
				  var pipe = '<span class="enroll-pipeline">|</span>';

				  var AvailableScore = ($(this).attr('ContScore') == '') ? '' : $(this).attr('ContScore');
				  var contentQuizStatus = '';
				  /*if(lcnt > 1) {
					  contentQuizStatus = widgetObj.getConsolidatedQuizStatus(data);
				  }
				  else {
					  contentQuizStatus = ($(this).attr('contentQuizStatus') == '') ? '' : $(this).attr('contentQuizStatus');
				  }*/
				  var launchInfo = data;
				  var contentQuizStatus = widgetObj.getConsolidatedQuizStatus(contentId, launchInfo);
//				  var contentQuizStatus = ($(this).attr('contentQuizStatus') == '') ? '' : $(this).attr('contentQuizStatus');
				  var score ='';
				  if(AvailableScore !=null && AvailableScore !=undefined && AvailableScore != '' && AvailableScore != '0.00') {
					      var score = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + AvailableScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') +': ' + AvailableScore ,'exp-sp-lnrenrollment-score')+'</span></div>';
				  }
				  if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
					  contentQuizStatus = '<div class="line-item-container float-left">'+pipe + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-sucstatus') + '</span></div>';
				  }
				  var attemptsLeft = '';
				  var attemptsLeft = ($(this).attr('AttemptLeft') == 'notset')? '' : $(this).attr('AttemptLeft');
				  if (attemptsLeft !== '') {
					  attemptsLeft = '<div class="line-item-container float-left" >'+pipe +'<span class="vtip" title="'+Drupal.t('LBL202')+' : '+attemptsLeft+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL202') +' : '+' ' + attemptsLeft,'exp-sp-lnrenrollment-attemptsleft')+'</span></div>';
				  }

				  var validityDays = ($(this).attr('ValidityDays') == '')? '' : $(this).attr('ValidityDays');
				  var diffDays = $(this).attr('remDays');

				  if (validityDays !== '') {
					  var remValidityDays = ($(this).attr('daysLeft') !== undefined && $(this).attr('daysLeft')  != null && $(this).attr('daysLeft')  !== 'null') ? $(this).attr('daysLeft')  : '';
//					  var remValidityDays = validityDays - diffDays;
					  if(remValidityDays <= 0){
					  var daysLBL = Drupal.t("Expired");//Expired
					  remValidityDays = '';// To avoid result as Validity: 0 Expired
					  }else if(remValidityDays == 1){
					  var daysLBL = Drupal.t("LBL910");//day
					  }else if(remValidityDays > 1){
					  var daysLBL = Drupal.t("LBL605");//days
					  }
					  validityDays = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+Drupal.t('LBL604')+' : '+remValidityDays+' '+daysLBL+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL604') +' : ' + remValidityDays + ' ' + daysLBL,'exp-sp-lnrenrollment-validitydays')+'</span></div>';
				  }
				
				var percentCompleted = '';
				if(contentType == 'VOD') {
					var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != '') ? JSON.parse(unescape($(this).attr('suspend_data'))) : null;
					var progress = suspend_data != null ? suspend_data['progress'] : 0;
					progress = isNaN(parseFloat(progress)) ? 0 : Math.round(progress);
					//console.log(progress);
					percentCompleted = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+ progress+'% '+Drupal.t('Completed') + '">'+titleRestrictionFadeoutImage(progress+'% '+Drupal.t('Completed'),'exp-sp-lnrenrollment-percentCompletion',200) + '</span></div>';
				}
	
				var versionNo = $(this).attr('VersionNum');
				  if (versionNo !== '') {
					  versionNo = '<div class="line-item-container float-left" >'+pipe +'<span class="vtip" title="'+Drupal.t('LBL1123')+' : '+versionNo+'">' +titleRestrictionFadeoutImage(Drupal.t('LBL1123') +' : '+' ' + versionNo,'exp-sp-lnrenrollment-versionNo')+'</span></div>';
				  }
	
				  var Dstatus = Drupal.t('In progress');
				  var Dstatus1 = Drupal.t('Incomplete');
				  var Dstatus2 = Drupal.t('Completed');
				  if ($.inArray(contentId, contentArr) == -1) {
					  contentArr.push(contentId);
					  var lessonId = $(this).attr('Id');
					  var contentStatus = ($(this).attr('ContentStatus') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('ContentStatus'));
				  if(contentType == 'VOD') {
						  contentStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : $(this).attr('Status');
				  }

					  html += '<tr id="lesson-launch-' + contentId + '">';
					  html +=   '<td class="enroll-launch-column1">';
					  html +=     '<div class="enroll-course-title">';
					  html +=       '<span id="lesson_title_' + contentId + '"></span>';
					  if (lcnt > 1) {
					    html +=     '<a id="lesson-accodion-' + contentId + '" href="javascript:void(0);"' +
					                    ' data="' + passdata + '"' +
					                      ' class="title_close"' +
					                        ' onclick=\'' + obj + '.addAccordionInLessonView(this.className, "0 0", "0 -61px", "dt-child-row-En", ' +
					                                                                          obj + '.getLessonViewDetails, this, ' + obj + ', true);\'>' +
					                  '&nbsp;' +
					                '</a>';
					  }
					  
					  if(lcnt > 1){
						  html +=       '<span id="titleAccEn_'+contentId+'" class="item-title multilesswidth" >';
					  }else{
						  html +=       '<span id="titleAccEn_'+contentId+'" class="item-title" >';
					  }
					 
					  html +=         '<span class="enroll-class-title"><span class="vtip"' +
					                       ' title="'+unescape($(this).attr('ContentTitle')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
					  html +=           titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('ContentTitle'))),'exp-sp-lnrenrollment-content-title',45)+'</span></br>';
					  html +=           '<div class="item-title-code individual-content">';
					  html +=             '<span id="lesson_status_'+contentId+'"><div class="line-item-container float-left" ><span title="'+Drupal.t('LBL102')+': '+Drupal.t(contentStatus)+'" class="vtip">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+': '+ Drupal.t(contentStatus),'exp-sp-lnrenrollment-contentstatus',200)+'</span></div>' + score + contentQuizStatus + attemptsLeft + validityDays + percentCompleted +versionNo;
					  html +=           '</span></div>';
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
							var suspend_data = ($(this).attr('suspend_data') != null && $(this).attr('suspend_data') != "") ? JSON.parse(unescape($(this).attr('suspend_data'))) : null;
							// var progress = suspend_data != null ? suspend_data['progress'] : 0;
							//console.log(progress);
						    callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
						    callLaunchUrlFn += title + "/";
						    callLaunchUrlFn += escape($(this).attr('contentSubTypeCode')) + "/";
						    callLaunchUrlFn += escape($(this).attr('url1').replace(/\//g, '()|()')) + '/';
						    callLaunchUrlFn += "ME" + "/";
						    callLaunchUrlFn += $(this).attr('courseId') + "/";
						    callLaunchUrlFn += $(this).attr('classId') + "/";
						    callLaunchUrlFn += $(this).attr('Id') + "/";
						    callLaunchUrlFn += $(this).attr('VersionId') + "/";
						    callLaunchUrlFn += $(this).attr('enrollId') + "/";
							callLaunchUrlFn += escape($(this).attr('StatusCode')) + "/";
						    // callLaunchUrlFn += progress + "/";
						    callLaunchUrlFn += $(this).attr('suspend_data')+ "\"";
							//console.log('callLaunchUrlFn');
			            }
			            else{
			            	var params = "";
			            	if(launchFrom == 'CL')
			            		params = "{'ContentId':'"+contentId+"','LaunchFrom':'"+launchFrom+"','Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			                      "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"'," +
			                      "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"' }";
			            	else
			            	params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			                      "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"'," +
			                      "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
			            	if(launchFrom == 'CL')
			            		callLaunchUrlFn = "onclick=\"$('#lnr-catalog-search').data('contentLaunch').launchWBTContent("+params+")\"";
			            	else
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
	    		var dataparam = 'data="'+params+'"';
	            var ajaxCss = ((contentType == 'VOD') && (IsLaunchable))? ' use-ajax ctools-modal-ctools-video-style ' : ' ';
					    html +=   '<div class="enroll-main-list">';
					    html +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
	                            ' class="' + ((IsLaunchable)? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
					   	html +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
	                             ' class="' + ((IsLaunchable)? 'actionLink' : '') + ajaxCss + ' enroll-launch"' +
	                             ' data-relaunch="' + relaunchData + '"' +
	                                 ' value="' + Drupal.t('LBL199') + '" '+((IsLaunchable)? callLaunchUrlFn : '') + ((launchFrom == 'CL')? dataparam : '')+' >';
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
			// console.log(e);
		}
	},

	getLessonViewDetails : function(catdiv,accordionLink,parentObj){
		try{
		return '';
		}catch(e){
			// to do
		}
	},


	addAccordionInLessonView : function(openCloseClass, openpos, closepos, childClass, callback, obj, parentObj, isRemove) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var currTrStr = $(obj).parent().parent().parent().attr("id");
		currTrStr = currTrStr.split('-');
		var currTr = currTrStr[2];
		if(!document.getElementById(currTr + "LessonSubGrid")) {
			$("#lesson-launch-"+currTr).after("<tr id='"+currTr+"LessonSubGrid'></tr>");
			$("#"+currTr+"LessonSubGrid").show();
			$("#lesson-accodion-"+currTr).removeClass("title_close");
			$("#lesson-accodion-"+currTr).addClass("title_open");
			$("#lesson-launch-"+currTr).css('border-bottom','none 0px');
			$("#"+currTr+"LessonSubGrid").slideDown(300);
			var data = eval($("#lesson-accodion-"+currTr).attr("data"));
			var lessonDetSec = this.paintLessonSection(data, currTr);
			$("#"+currTr+"LessonSubGrid").html(lessonDetSec);
		}
		else {
			var clickStyle = $("#"+currTr+"LessonSubGrid").css("display");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"LessonSubGrid").show();
    			$("#"+currTr+"LessonSubGrid").slideDown(300);
    			$("#lesson-accodion-"+currTr).removeClass("title_close");
				$("#lesson-accodion-"+currTr).addClass("title_open");
				$("#lesson-launch-"+currTr).css('border-bottom','none 0px');
    		}
    		else {
    			$("#"+currTr+"LessonSubGrid").hide();
    			$("#"+currTr+"LessonSubGrid").slideUp(300);
    			$("#lesson-accodion-"+currTr).removeClass("title_open");
				$("#lesson-accodion-"+currTr).addClass("title_close");
				$("#lesson-launch-"+currTr).removeClass("ui-widget-content");
				$("#lesson-launch-"+currTr).css('border-bottom','1px solid #cccccc');
    		}
		}
		if(this.currTheme == 'expertusoneV2'){
			$('.enrollment-launch-table tr:last-child').css('border','0px');
		}

		$('.launch-lesson-view').each(function(){
		  $(this).parents("tr").children("td").css({'padding-top' : '0px',"border-top":"0"});
		  $('.enrollment-launch-table').find('tbody tr:visible:last').last().css('border','0px');
		  $('.enrollment-launch-table').find("tbody tr td tr:visible:last-child").css('border','0px');
		});

		//SCrollbar for content dialogbox
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
		else
			$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		resetFadeOutByClass('.launch-lesson-view','item-title-code','line-item-container','MyEnrollments');
		vtip();
		}catch(e){
			// to do
		}
	},

	paintLessonSection : function(data,paramContId){
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
			  if(paramContId == contentId){
				  lesCnt++;
				  var params = "";
				  var IsLaunchable 	= eval($(this).attr('IsLaunchable'));
				  var launchFrom = '';
				  if(this.LaunchFrom =='CL')
					  var launchFrom = $(this).attr('LaunchFrom');
				  var contValidateMsg = $(this).attr('contValidateMsg');
				  var contentType = $(this).attr('launchType');
				  var VersionId = $(this).attr('VersionId');
				  var lessonId = $(this).attr('Id');
				  if(launchFrom == 'CL')
					  params = "{'LaunchFrom':'"+launchFrom+"','Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
			            "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',"+
			            "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
				  else
				  params = "{'Id':'"+$(this).attr('Id')+"','enrollId':'"+$(this).attr('enrollId')+"','VersionId':'"+$(this).attr('VersionId')+"','url1':'"+$(this).attr('url1')+"'," +
				            "'courseId':'"+$(this).attr('courseId')+"','classId':'"+$(this).attr('classId')+"','url2':'"+$(this).attr('url2')+"','AICC_SID':'"+$(this).attr('AICC_SID')+"',"+
				            "'ErrMsg':'"+$(this).attr('ErrMsg')+"','contentType':'"+$(this).attr('contentType')+"','Status':'"+$(this).attr('Status')+"'}";
				  if(launchFrom == 'CL')
	            		callLaunchUrlFn = "onclick=\"$('#lnr-catalog-search').data('contentLaunch').launchWBTContent("+params+")\"";
	            	else
				  callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
				  var lessonLocation = $(this).attr('LessonLocation');
          var launchData = $(this).attr('launch_data');
          var suspendData = $(this).attr('suspend_data');
          var exit = $(this).attr('exit');
          lessonLocation = lessonLocation == null || lessonLocation == 'null'? '' : lessonLocation;
          launchData = launchData == null || launchData == 'null'? '' : launchData;
          suspendData = (suspendData == null || suspendData == 'null' || suspendData == undefined)? '' : suspendData;
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
		          var lessonStatus = ($(this).attr('Status') == '') ? Drupal.t('MSG511') : Drupal.t($(this).attr('Status'));
		          var contScore = ($(this).attr('LesScore') != '') ? $(this).attr('LesScore') : '';
		          var contentQuizStatus = ($(this).attr('contentQuizStatus') != '') ? $(this).attr('contentQuizStatus') : '';
		          if(contentQuizStatus != null && contentQuizStatus != undefined && contentQuizStatus != '') {
					  contentQuizStatus = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+ Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus) + '">'+ titleRestrictionFadeoutImage(Drupal.t('LBL1284') + ' : ' + Drupal.t(contentQuizStatus),'exp-sp-lnrenrollment-sucstatus') + '</span></div>';
				  }
		          if(contScore == '') {
		        	  var lessonScore = '';
		          } else {
		        	  var lessonScore = '<div class="line-item-container float-left" >'+pipe + '<span class="vtip" title="'+Drupal.t('LBL668')+': ' + contScore+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL668') + ': ' + contScore,'exp-sp-lnrenrollment-score') + '</span></div>';
		          }
				  ostr += '<tr class="enroll-lesson-section"><td class="enroll-launch-column1">';
				  ostr += '<div class="enroll-course-title-lesson">';
				  ostr += '<span id="titleAccEn_'+lessonId+'" class="item-title" ><span class="enroll-course-title vtip" title="'+Drupal.t('LBL854')+' '+lesCnt+' : '+unescape($(this).attr('Title')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				  ostr += Drupal.t('LBL854')+' '+lesCnt+': ' +titleRestrictionFadeoutImage(decodeURIComponent($(this).attr('Title')),'exp-sp-lnrenrollment-enroll-course-title',35);
				  ostr += '</span></span>';
				  ostr += '</div>';
				  ostr += '<div class="enroll-class-title-info">';
				  ostr += '<div class="item-title-code individual-lesson">';
				  // fix for mantis ticket #0020086. Added id attribute for status span
				  ostr += '<div class="line-item-container float-left"><span id ="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_status" class="vtip" title="'+Drupal.t('LBL102')+': ' +Drupal.t(lessonStatus)+'">'+titleRestrictionFadeoutImage(Drupal.t('LBL102')+' : '+Drupal.t(lessonStatus),'exp-sp-lnrenrollment-status') + '</span></div>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonScore">'+ lessonScore +'</span>';
				  ostr += '<span id="' + $(this).attr('Id') + '_' + $(this).attr('ContentId') + '_' + $(this).attr('enrollId') + '_lessonQuizStatus">'+ contentQuizStatus +'</span>';
				  ostr += ' </span>';
				  ostr += ' </div>';
				  ostr += '</div>';
				  ostr += '</td>';
				  // fix for mantis ticket #0020086. Added id attribute for button
				  ostr += '<td class="enroll-launch-column2">';
				  ostr +=   '<div class="enroll-main-list">';
				  ostr +=     '<label id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button_lbl"' +
				                  ' class="'+((IsLaunchable) ? "enroll-launch-full" : "enroll-launch-empty") + ' launch_button_lbl">';
				  ostr +=       '<input type="button" id="' + $(this).attr('Id') + '_' + $(this).attr('enrollId') + '_launch_button"' +
				                   ' class="' + ((IsLaunchable) ? 'actionLink' : '') + ' enroll-launch"' +
                             ' data-relaunch="' + relaunchData + '"' +
				                       ' value="' + Drupal.t('LBL199') + '" '+((IsLaunchable)? callLaunchUrlFn : '') + '>';
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

	addAccordionToTable1 : function(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableView(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		}catch(e){
			// to do
		}
		vtip();
	},

	addAccordionToTableView : function(classId,openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var currTr = $('#class-accodion-'+classId).parent().parent().parent().attr("id");
		var data = eval($("#class-accodion-"+currTr).attr("data"));
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr+" td .enroll-class-title-info").after("<div id='"+currTr+"SubGrid' class='clsDetailsDivNormal'></div>");
			$("#"+currTr+"SubGrid").css("display","block");
			//$("#compTabDetails"+classId).show();// Completed tab css("display","block");
			if(this.currTheme != 'expertusoneV2') {
				$("#class-accodion-"+currTr).removeClass("title_close");
				$("#class-accodion-"+currTr).addClass("title_open");
			}
			//$("#"+currTr).find("td").css('border-bottom','none 0px');
			//$("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','none 0px');
			//$("#class-accodion-"+currTr).parents("tr").children("td").css('border-right','none 0px');
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"SubGrid").css("display","block");
			$("#"+currTr+"SubGrid").slideDown(1000);
			$("#"+currTr+"SubGrid").css("display","block");
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display");
			//$("#compTabDetails"+classId).hide();// Completed tab css("display","none");
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			//$("#compTabDetails"+classId).show();// Completed tab css("display","block");
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#class-accodion-"+currTr).removeClass("title_close");
    				$("#class-accodion-"+currTr).addClass("title_open");
    			}
				  $("#"+currTr).removeClass("ui-widget-content");
				  if ($("#pager").is(":visible") || data.is_last_rec != 'last') {
		//		    $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','none 0px');
				  }
				} else {
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$('#seemore_'+currTr).css('display', 'block');
    			$("#"+currTr+"SubGrid").slideUp(1000);
    			if(this.currTheme != 'expertusoneV2') {
    				$("#class-accodion-"+currTr).removeClass("title_open");
    				$("#class-accodion-"+currTr).addClass("title_close");
    			}
				  $("#"+currTr).removeClass("ui-widget-content");
				  $("#"+currTr).addClass("ui-widget-content");
				  if ($("#pager").is(":visible") || data.is_last_rec != 'last') {
					  if(this.currTheme == 'expertusoneV2') {
						  $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','solid 1px #ededed');
					  }else
					  {
						  $("#class-accodion-"+currTr).parents("tr").children("td").css('border-bottom','solid 1px #ccc');
					  }
				  }
    		}
		}
		$("#"+currTr+"SubGrid").find("td").css('border-bottom','none 0px');
		var classDetSec = this.paintDetailsClassSection(data);
		if(document.getElementById("seemore_"+currTr))
			$("#"+currTr+"SubGrid").html('<table><tr>'+classDetSec+'</tr></table>');
		else
		$("#"+currTr+"SubGrid").html(classDetSec);
		if((this.currTheme == 'expertusoneV2') && (data.is_last_rec != 'last')){
			//$("#"+currTr+"SubGrid").find("td").css('border-bottom','1px solid #EDEDED');
			$("#"+currTr+"SubGrid").html('<table><tr>'+classDetSec+'</tr></table>');
		}
		}catch(e){
			// console.log(e);
			// to do
		}
	},

	addAccordionToTable2 : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		this.addAccordionToTable(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		$('.dt-child-row-En > td').attr('colSpan','6');
		Drupal.attachBehaviors();
		}catch(e){
			// to do
		}
	},

	addAccordionToTable_override:function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
        if(openCloseClass!=undefined && childClass!=undefined && callback!=undefined) {
            var mainObj=$(obj).parent().get(0).nodeName.toLowerCase()=="td"?$(obj).parent():$(obj).parent().parent();
            $('.'+openCloseClass).each(function(){
                $(this).css("background-position",closepos);
            });

            $(".ui-state-hover").click(function(){
            	var clickId = $(this).attr("id");
            	if(document.getElementById("child"+clickId)) {
            		var clickStyle = $("#child"+clickId).css("display");
            		if((clickStyle == "none") || (clickStyle == 'undefined') || (clickStyle == 'table-row')) {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","block");
            		} else if(clickStyle == "block") {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","none");
            		}else {
            			$("#child"+clickId).slideDown(1000);
            			$("#child"+clickId).css("display","none");
            		}
            	}
            });

        	var cols = $(obj).parent('td').siblings().length;
        	if(String(mainObj.parent().next().attr("class")).indexOf(childClass+" dt-acc-row")<0) {
                var newObj=mainObj.parent().after("<tr class='"+childClass+" dt-acc-row' id='tempChild'><td colspan='"+cols+"'><div class='"+childClass+"-data' style='display:none;width=100%;'></div></td></tr>").next(0).find("div."+childClass+"-data");
                if(callback!=undefined) {
                    callback(newObj,$(obj),parentObj);
                }
                $(obj).children().css("background-position",openpos);
            }
        } else {
            throw("addAccordionTable : Parameters not specified");
        }
		}catch(e){
			// to do
		}
    },

	paintActions : function(cellval,options,rowObject){
	 try{
		var baseType				= rowObject['basetype'];
		var dateValidTo				= rowObject['valid_to'];
		var dataDelType				= rowObject['delivery_type'];
		var dataDelTypeCode			= rowObject['delivery_type_code'];
		var dataRegStatus			= rowObject['reg_status'];
		var dataRegStatusCode		= rowObject['reg_status_code'];
		var RegStatusCode			= rowObject['reg_status_code'];
		var courseRegStatusCode		= rowObject['reg_status_code'];
		var courseComplitionStatus	= rowObject['reg_status'];
		var courseScore				= rowObject['score'];
		var courseQuizStatus		= rowObject['quiz_status'];
		//var courseGrade				= rowObject['grade'];
		var dataId 					= unescape(rowObject['id']);
		var courseId 				= rowObject['course_id'];
		var classId 				= rowObject['class_id'];
		var classTitle				= rowObject['cls_title'];
		var classStatus				= rowObject['cls_status'];
		var dedClass				= rowObject['dedicated_class_flag'];
		var crsTitle				= rowObject['course_title'];
		var courseTempId			= rowObject['courseid'];
		var surStatus				= rowObject['surveystatus'];
		var assessStatus			= rowObject['assessmentstatus'];
		var preassessStatus			= rowObject['preassessmentstatus'];
		//var attemptsleft			= rowObject['attemptleft'];
		var maxScoreValidationpre	= rowObject['maxscorevaluepre'];
		var maxScoreValidationpost	= rowObject['maxscorevaluepost'];
		var session_id				= rowObject['session_id'];
		var usertimezonecode        = rowObject['usertimezonecode'];
		var launchInfo				= rowObject['launch'];
		var masterEnrollId			= rowObject['master_enrollment_id'];
		var mandatory			    = rowObject['mandatory'];
		var IsCompliance			= rowObject['is_compliance'];
		var recertifyCompliance    	= rowObject['recertify_compliance'];
		var mro						= rowObject['mro'];
		var nodeId 					= rowObject['node_id'];
		var usrId					= rowObject['user_id'];
		var isChangeClass 			= rowObject['switch_events'];
		var dataIdEncrypted			= rowObject['id_encrypted'];
		var classIdEncrypted		= rowObject['class_id_encrypted'];
		//36601: Invalid Error Message in "Change Class" function
		var isPricedClass			= (rowObject['show_price']==0)?'':rowObject['show_price'];
		var ecommerce_enabled_status= Drupal.settings.exp_sp_lnrenrollment.ecommerce_enabled;//36658: Unable to choose another class using "Change Class" function.
		var daysRemaining 			= '';
		var callLaunchUrlFn			= '';
		var contLaunch				= false; // Used to determine whether Launch link opens an accordian for multiple content class (true)
											 // or directly launches content (false)
		var IsLaunchable			= false;
		var remDays			        = rowObject['remDays'];
		var errmsg					= '';
		var contValidateMsg			= '';
		var isMultipleCont			= false;
		var LessonLocation			= '';
		var launch_data				= '';
		var suspend_data			= '';
		var exit					= '';
		var VersionId 				= '';
		var AICC_SID 				= '';
		var AttemptLeft	  = '';
		var dayRemainVal  = '';
		var VersionId = '';
		var launch_data				= rowObject['LaunchData'];
		var starWidgetHtml = rowObject['star_widget'];
		var vcMoreActionFlag		= true;
		var is_exempted = rowObject['is_exempted'];
		var exempted_by = rowObject['exempted_by'].replace(/'/g, "\\\'");
		var exempted_on = rowObject['exempted_on'];
		var hideShareLink = rowObject['hide_share'];
		if(IsCompliance){
			mandatory = 'Y';
		}

		// Assessment Validation - Max score and questions score total should be equal.
		// At least one assessment should match this condition
		if(assessStatus == 'TRUE' && maxScoreValidationpost == 0)
			assessStatus = false;
		if(preassessStatus == 'TRUE' && maxScoreValidationpre == 0)
			preassessStatus = false;

		var startTime = rowObject['session_start'];
		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			var sTime,sTimeForm,eTime,eTimeForm,nTime,stDateFull,enDateFull,srvDate;
			if(sessLen>1) {
				if(baseType =="ILT"){
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					viewButton = 'viewDetails';
					html += "<a type='button' title='"+Drupal.t("Reregister")+"' class='actionLink enroll-launch mypgm-recertify vtip'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+titleRestrictionFadeoutImage(Drupal.t("Reregister"),'exp-sp-mylearning-menulist')+"</a>";

					completeAction = '<li class="action-enable"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';

					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';

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
					var endSes = rowObject.sessionDetails.length - 1;
					sTime = rowObject.sessionDetails[ind].session_start_format;
					sTimeForm = rowObject.sessionDetails[ind].session_start_time_form;
					eTime = rowObject.sessionDetails[ind].session_end_format;
					eTimeForm = rowObject.sessionDetails[ind].session_end_time_form;
					nTime = rowObject.sessionDetails[ind].session_end_format;
					eDateTime = rowObject.sessionDetails[ind].session_end_format;
					stDateFull = rowObject.sessionDetails[ind].session_start_time_full;
					enDateFull = rowObject.sessionDetails[ind].session_end_time_full;
					clEnDateFull = rowObject.sessionDetails[endSes].session_end_time_full;
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
				clEnDateFull = rowObject.sessionDetails[0].session_end_time_full;
			}
			var srvDate = rowObject.sessionDetails[0].server_date_time;
			var type = rowObject.sessionDetails[0].type;
			var sesStartIlt = new Date(stDateFull);
			var sesEndVC   = new Date(enDateFull);
			var todayDate    = new Date(srvDate);
			var timeDiffIlt    = (sesStartIlt - todayDate);
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
			// If the content is launchable, prepare the launch URL call function string in variable callLaunchUrlFn
			if(IsLaunchable) {
				var id 					= launchInfo[0]["ID"];
				var url1 				= launchInfo[0]["LearnerLaunchURL"];
				var contentTitle       	= launchInfo[0]["Title"];
				var url2 				= launchInfo[0]["PresenterLaunchURL"];
				var status 				= launchInfo[0]["Status"];
				var statusCode			= launchInfo[0]["StatusCode"];
				var launchBaseType		= launchInfo[0]["baseType"];
				var cntType				= launchInfo[0]["ContentType"];
				var cntSubTypeCode 		= launchInfo[0]["ContentSubTypeCode"];
				var MasteryScore 		= launchInfo[0]["masteryscore"];
				var CntStatus			= launchInfo[0]["contentQuizStatus"];
				if (baseType == "VOD")
					var suspendData = (launchInfo[0]["SuspendData"] != null && launchInfo[0]["SuspendData"] != "" && launchInfo[0]["SuspendData"] != undefined) ? JSON.parse(unescape(launchInfo[0]["SuspendData"])) : null;
				var inc = 0;
				if(launchInfo.length > 1) {  // Class has multiple contents, launch URL call function will launch the accordian
					contLaunch = true; // Launch link opens an accordian
					var overrideIsLaunchable = false; // Variable used to determine WBT class launchability from the launchability of each content

					// Prepare content launch data for rendering the content launch links in the accordian
					var passParams = "[";
					$(launchInfo).each(function(){
						 inc=inc+1;
						 passParams += "[{";
						 passParams += "'Id':'"+$(this).attr("ID")+"'";
						 passParams += ",'ContentId':'"+$(this).attr("ContentId")+"'";
						 passParams += ",'VersionId':'"+$(this).attr("VersionId")+"'";
						 passParams += ",'VersionNum':'"+$(this).attr("VersionNum")+"'";
						 passParams += ",'enrollId':'"+dataId+"'";
						 passParams += ",'Title':'"+$(this).attr("Title")+"'";
						 passParams += ",'ContentTitle':'"+escape($(this).attr("Code"))+"'";
						 passParams += ",'url1':'"+$(this).attr("LearnerLaunchURL")+"'";
						 passParams += ",'courseId':'"+courseId+"'";
						 passParams += ",'IsLaunchable':'"+$(this).attr("IsLaunchable")+"'";
						 passParams += ",'AttemptLeft':'"+$(this).attr("AttemptLeft")+"'";
						 passParams += ",'ValidityDays':'"+$(this).attr("ValidityDays")+"'";
						 passParams += ",'remDays':'"+$(this).attr("remDays")+"'";
						 passParams += ",'contValidateMsg':'"+$(this).attr("contValidateMsg")+"'";
						 passParams += ",'daysLeft':'"+$(this).attr("daysLeft")+"'";
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
						 passParams += ",'suspend_data':'"+$(this).attr("SuspendData")+"'";
						 passParams += ",'exit':'"+$(this).attr("CmiExit")+"'";
						 passParams += ",'AICC_SID':'"+$(this).attr("AICC_SID")+"'";
						 passParams += ",'MasteryScore':'"+$(this).attr("masteryscore")+"'";
						 passParams += ",'contentQuizStatus':'"+$(this).attr("contentQuizStatus")+"'";
						 passParams += ",'ContentCompletionStatus':'"+$(this).attr("ContentCompletionStatus")+"'";
						 passParams += ",'StatusCode':'"+$(this).attr("StatusCode")+"'";
						 passParams += "}]";

						 if(inc < launchInfo.length) {
							 passParams += ",";
						 }

						 // Determine launchability of WBT class and VOD class from the launchability of each included content
						 if (baseType == "WBT" || baseType == "VOD") {
							 overrideIsLaunchable = overrideIsLaunchable || $(this).attr("IsLaunchable");
						 }
					});
					passParams += "]";

					// Launchability of a multiple content WBT class or a multiple content VOD class is determined from the launchability of each
					// included content
					if (baseType == "WBT" || baseType == "VOD") {
						IsLaunchable = overrideIsLaunchable;
					}

					var obj = options.colModel.widgetObj;
					callLaunchUrlFn = 'onclick=\''+obj+'.launchMultiContent('+dataId+',this);\'';


				} else { // Class has single content
					contLaunch = false; // Launch link directly launches the content
					if((baseType == "WBT") && (cntType != null) && (cntType != undefined) && (cntType != '')) {
						var params = "{'Id':'"+id+"','enrollId':'"+dataId+"','VersionId':'"+VersionId+"','url1':'"+url1+"','courseId':'"+courseId+"','classId':'"+classId+"','url2':'"+url2+"','ErrMsg':'"+errmsg+"','contentType':'"+cntType+"','Status':'"+status+
						"','LessonLocation':'"+LessonLocation+"','launch_data':'"+launch_data+"','contentQuizStatus':'"+CntStatus+"','suspend_data':'"+suspend_data+"','exit':'"+exit+"','AICC_SID':'"+AICC_SID+"','MasteryScore':'"+MasteryScore+"'}";
						callLaunchUrlFn = "onclick=\"$('#learner-enrollment-tab-inner').data('contentLaunch').launchWBTContent("+params+")\"";
					}
					else if (baseType == "VOD") {
					   //debugger;
						// The title for video is the content title
						var title = contentTitle;
						title = encodeURIComponent(title.replace(/\//g, '()|()'));						
						
						callLaunchUrlFn = "href=\"/?q=learning/nojs/play_video/";
						callLaunchUrlFn += title + "/";
						callLaunchUrlFn += escape(cntSubTypeCode) + "/";
						callLaunchUrlFn += (url1) ? escape(url1.replace(/\//g, '()|()')) + '/' : '' ;
						callLaunchUrlFn += "ME" + "/";
						callLaunchUrlFn += courseId + "/";
						callLaunchUrlFn += classId + "/";
						callLaunchUrlFn += id + "/";
						callLaunchUrlFn += VersionId + "/";
						callLaunchUrlFn += dataId + "/";
						callLaunchUrlFn += escape(statusCode) + "/";
						
						// var progress = suspendData != null ? suspendData['progress'] : 0;
						//console.log('progress--->'+progress);
						// callLaunchUrlFn += progress + "/";
						callLaunchUrlFn += launchInfo[0]["SuspendData"] + "\"";
						//console.log($(this));
					}
					else if(baseType == "WBT" || baseType == "VC") {
						callLaunchUrlFn	= url1;
					}
				}
			}
		}
		// Calculate days remaining for the WBT and VOD content, to disallow cancellation of an expired WBT or VOD class.
		if((baseType=="WBT" || baseType == "VOD") &&
			 ((courseComplitionStatus=="lrn_crs_cmp_cmp") || (dataRegStatusCode=="lrn_crs_cmp_enr") ||
					 			(dataRegStatusCode=="lrn_crs_cmp_inp"))) {
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

		var obj = options.colModel.widgetObj;
		var objEval = eval(options.colModel.widgetObj);
		var callDialogObj = '$("body").data("learningcore")';
		var data1;
		var contdata;
		var multiContentData;
		if(contLaunch) {
			// Multiple contents in class, prepare data for the accordian
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'" + options.rowId + "','CourseId':'" + courseId + "','session_id':'"+session_id+
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode +
							"','regstatus':'" + dataRegStatusCode + "','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode +
							"','daysRemaining':'" + daysRemaining + "','courseComplitionStatus':'" + courseComplitionStatus +
							"','courseScore':'" + courseScore + "','courseQuizStatus':'" + courseQuizStatus + "','detailTab':'true','assMand':'" + mandatory + "','mro':'" 
							+ mro+"','is_exempted':'" + is_exempted + "','exempted_on':'" + exempted_on + "','exempted_by':'" + exempted_by + "'}";
			contdata = "data=["+passParams+"]";
			multiContentData = passParams;
		} else {
			// Prepare data for content launch link
			data1 = "data={'RegId':'" + dataId + "','BaseType':'" + baseType + "','clstitle':'" + escape(classTitle) +
							"','title':'" + escape(crsTitle) + "','rowId':'" + options.rowId + "','CourseId':'" + courseId +  "','session_id':'"+session_id+
							"','coursetempid':'" + courseTempId + "','regStatusCode':'" + RegStatusCode + "','regstatus':'" + dataRegStatusCode +
							"','classid':'" + classId + "','deliverytype':'" + dataDelTypeCode + "','daysRemaining':'" + daysRemaining +
							"','courseComplitionStatus':'" + courseComplitionStatus + "','courseScore':'" + courseScore +
							"','courseQuizStatus':'" + courseQuizStatus +
							"','detailTab':'true','assMand':'" + mandatory + "','mro':'" + mro+"','is_exempted':'" + is_exempted + "','exempted_on':'" + exempted_on + "','exempted_by':'" + exempted_by + "'}";
		}

		// Prepare the Action HTML to be returned.
		var html = '';
		var setDropButton = true; // Allow cancellation (drop) of class when setDropButton is set to true.
		if((baseType == 'WBT' || baseType == 'VOD') && daysRemaining < 0){
			setDropButton = false; // Disallow cancellation of class if the WBT class has expired.
		}

		html += "<div class='enroll-main-list' id='enroll-main-action-"+dataId+"'>";
		// Prepare Action button HTML for My Enrollments 'Enrolled' tab
		/**
		 * RegStatusCode lrn_crs_cmp_cmp added to the block as the assessment links are not shown in completed tab
		 * */
		if(RegStatusCode == 'lrn_crs_cmp_enr' ||  RegStatusCode == 'lrn_crs_cmp_inp' || RegStatusCode == 'lrn_crs_cmp_att' || RegStatusCode == 'lrn_crs_cmp_cmp'){ //  RegStatusCode == 'lrn_crs_cmp_cmp' ||
			var isILT = false;
			var isWBT = false;
			var isVC = false;
			var isVOD = false;
			var wbtButtonAction = null; // Variable later used to determined what links to show in the More menu.
			var vodButtonAction = null;
			var iltButtonAction = null;
			var viewButton = null;
			var iltShareButton = null;
			var wbtShareButton = null;
			var iltChangeButton = null;
			var iltCompleteCertificateButton = null;
			var vcjoinButton = null;
			var no_primary_html_button='0';
			var no_drop_down_option='0';

			if (dataDelTypeCode == 'lrn_cls_dty_wbt') { // Show correct button for the WBT class
			     isWBT = true;
				// If is launchable, show the Launch button
				if (IsLaunchable) {
					var launchLabelVal  = Drupal.t("LBL199");//(launchInfo.length > 1) ? Drupal.t('LBL816') : Drupal.t("LBL199");
					wbtButtonAction = 'Launch';
					html += "<input type='button' class='actionLink enroll-launch' " +
									"data=\"" + multiContentData + "\" " +
										"value='" + launchLabelVal + "' " +
											"name='" + SMARTPORTAL.t("Launch") + "' " +
												"id='launch" + dataId + "' " +
													callLaunchUrlFn +
														" >";
				}
				else {
					//wbtButtonAction = 'View';
					var wbtShareButton = 'wbtShareButton';
					if(hideShareLink == 0){
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
							'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
							'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
				   }else{
				   	  no_primary_html_button='1';
				   }
					
					//viewButton = 'viewDetails';
					//html += '<a  id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				}
			} else if (dataDelTypeCode == 'lrn_cls_dty_vod') {
						 isVOD = true;
				        // If is launchable, show the Launch button
				        if (IsLaunchable) {
				          vodButtonAction = 'Launch';
				          if (launchInfo.length > 1) {
				            html += "<input type='button' class='actionLink enroll-launch' " +
				                  "data=\"" + multiContentData + "\" " +  "data-data1=\"" + data1 + "\" " +
				                    "value='" + Drupal.t("LBL199") + "' " +
				                      "name='" + SMARTPORTAL.t("Launch") + "' " +
				                        "id='launch" + dataId + "' " +
				                          callLaunchUrlFn +
				                            " >";
				          } else {
				            html += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
				                          "title='" + Drupal.t("LBL199") + "' " +
				                            "id='launch" + dataId + "' " +
				                              callLaunchUrlFn +
				                                " >" +
				                                  Drupal.t("LBL199") + "</a>";
				          }
				        }
				        else {
				          //viewButton = 'viewDetails';
				          //html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				        	var wbtShareButton = 'wbtShareButton';
				        	if(hideShareLink == 0){
								html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
								'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
								'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
							}else{
							   	  no_primary_html_button='1';
							}
				        }
			// Show enabled/disabled Launch button for the VCL class
			} else if (dataDelTypeCode == 'lrn_cls_dty_vcl') {
					  isVC = true;
					  if(RegStatusCode == 'lrn_crs_cmp_cmp') {
						  	var title = 'Session Details';
							var message = Drupal.t("MSG419");
							html += "<a class='actionLink enroll-launch vtip " +
		        					"' data=\"" + contdata + "\" " +
		        					"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
		        					" id='launch" + dataId + "' href='javascript:void(0);' " +
													"onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
														" >"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
					  }
					  else {
						  	var sesStartVC = new Date(stDateFull);
						  	var sesEndVC   = new Date(enDateFull);
							var todayVC    = new Date(srvDate);
							var timeDiffVC    = (sesStartVC - todayVC);
							var endTimeDiffVC =(sesEndVC - todayVC);
							var allowTime = (resource.allow_meeting_launch) ? (resource.allow_meeting_launch * 60000) : 3600000;
							//Launch appears only in the configured time . If not configured allow_meeting_launch then system take default as 1 hour from session start
							var afterCompleteAllowTime = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete * 60000) : 1800000;
							//Launch appears only in the configured time. If not configured allow_meeting_launch then system take default as half hour from session end
							
							if(timeDiffVC <= allowTime && (endTimeDiffVC+afterCompleteAllowTime) >=0){
									var a = (sesEndVC-todayVC);
									var timer = (afterCompleteAllowTime+a);
									if(RegStatusCode == 'lrn_crs_cmp_att' && type == 'lrn_cls_vct_web'){
													var new_window;
													new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink enroll-launch " +
													"' data=\"" + contdata + "\" " +
															"' name='" + SMARTPORTAL.t("Launch") + "' " +
																" id='launch" + dataId + "' href='javascript:void(0);' " +
																		//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")'>"+ Drupal.t("Re-Join") + "</a>";
																"onclick='"+new_window+";'>"+ Drupal.t("LBL1319") + "</a>";
												refresh_time(timer);
									}else{
												var new_window;
												new_window = "window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")";
												html += "<a class='actionLink enroll-launch vtip " +
												"' data=\"" + contdata + "\" " +
														"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
															" id='launch" + dataId + "' href='javascript:void(0);' " +
																	//" onclick='window.open(\""+((IsLaunchable) ? callLaunchUrlFn : '')+"\", \"Attachment\", \"width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes\")'>"+ Drupal.t("Join") + "</a>";
												"onclick='"+new_window+";refresh_enrolled("+new_window+");'>"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
										}
							}else{
								var title ,message;
								title = Drupal.t('LBL670');
								if(endTimeDiffVC+afterCompleteAllowTime < 0){
									message = Drupal.t("MSG419");
								}
								else{
									message = Drupal.t("MSG420").replace("'", "&#39;")+ ' ' + sTime +' <span class=enroll-session-timefrom>'+sTimeForm+'</span>';
								}
								html += "<a class='actionLink enroll-launch vtip " +
				        		"' data=\"" + contdata + "\" " +
				        				"' name='" + SMARTPORTAL.t("Launch") + "' " + " title='"+Drupal.t("LBL880")+"' " +
				        					" id='launch" + dataId + "' href='javascript:void(0);' " +
															"onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
																" >"+ titleRestrictionFadeoutImage(Drupal.t("LBL880"),'exp-sp-mylearning-menulist') + "</a>";
							}
					  }
			}else {
				// For ILT classes, the button is always Cancel on Enrolled tab
				isILT = true;
				viewButton = 'viewDetails';
				if(timeDiffIlt >= 0 && hideShareLink == 0){
					iltShareButton = 'iltShareDetails';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
					'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
			    }
				else if(RegStatusCode == 'lrn_crs_cmp_enr' && isPricedClass && ecommerce_enabled_status != '' && ecommerce_enabled_status !=null) {
					iltChangeButton = 'iltChangeButton';
					regMsg = Drupal.t('MSG692');
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Change Class" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class") ,'exp-sp-mylearning-menulist')+ '</a>';
				} else if(RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw' && isChangeClass && (isPricedClass == null || isPricedClass == '' || isPricedClass == undefined)){
					iltChangeButton = 'iltChangeButton';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch vtip" data=\"' + data1 +
					'\" name="Change Class" title="'+Drupal.t("LBL943") + ' ' +Drupal.t("Class")+'" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
					titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),'exp-sp-mylearning-menulist') + '</a>';
				}
				else{
					if(RegStatusCode == 'lrn_crs_cmp_cmp') {
						iltCompleteCertificateButton = 'iltCompleteCertificateButton';
					   var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
				   	   if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" class="actionLink enroll-launch" data=\"' + data1 +
							'\" name="Certificate"  onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+dataIdEncrypted+'\',\''+classIdEncrypted+'\',\''+objEval.enrUserId+'\',\'CLASS\',\'div_my_transcript\',800,900);" >' + Drupal.t("LBL205") + '</a>';
					   }else{
						   html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip'  data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")' title='"+Drupal.t("LBL205")+"'>" + titleRestrictionFadeoutImage(Drupal.t("LBL205") ,'exp-sp-mylearning-menulist') + "</a>";
					   }
					}else{
						if(hideShareLink == 0){
							html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" title="'+Drupal.t("Share")+'" class="actionLink enroll-launch clsDisabledButton vtip" data=\"' + data1 +
							'\" name="Refer" >' + titleRestrictionFadeoutImage(Drupal.t("Share"),'exp-sp-mylearning-menulist') + '</a>';
							//viewButton = 'viewDetails';
							//html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
						}else{
							no_primary_html_button='1'; 
						}
					}
				}
				//html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
			}
			
			// Prepare the More menu items HTML in variable enrollAction
			var enrollAction = '';		
			// Show Cancel link
		 
			if( RegStatusCode == 'lrn_crs_cmp_enr'){
				if (isWBT || isVOD ) {
					if(setDropButton == true) {
						
						if(no_primary_html_button=='1'){ //make CANCEL button as primary
							
							html+="<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a>";
						    no_primary_html_button='0'; // Reset to 0					
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a></li>";
						} 
											
											
					}
				}else if(isILT){
					if(setDropButton == true) {
					  iltButtonAction = 'Cancel';
					  if(timeDiffIlt >= 0) {
					  	
						  	if(no_primary_html_button=='1'){ //make CANCEL button as primary
						  		html += "<a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" onclick='" + obj + ".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' >" +Drupal.t("LBL109") + "</a>";
							    no_primary_html_button='0'; // Reset to 0				
						  	}else{
						  		enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" onclick='" + obj + ".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' >" +Drupal.t("LBL109") + "</a></li>";
						  	}
						  
								
								
					  }
					}

				}else if(isVC && timeDiffVC >= 0 && vcMoreActionFlag==true){
					if(setDropButton == true) {
						enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='Cancel' onclick='" + obj + ".dropEnroll(this);' >" +Drupal.t("LBL109") + "</a></li>";
					}
				}
			}
			
		 
			// Show Survey link
			var surObj		= '$("#block-take-survey").data("surveylearner")';
			if (surStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner) /*&&
					((isWBT && wbtButtonAction == 'Launch') || (isVOD && vodButtonAction == 'Launch') || isILT || isVC )*/) {
						
						
				if(no_primary_html_button=='1'){ //make SURVEY button as primary
					
					html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ",\"NULL\",\"enroll-result-container\");' >" +
											Drupal.t("Survey") + "</a>";
					
					no_primary_html_button='0';
					
				}else{
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ",\"NULL\",\"enroll-result-container\");' >" +
											Drupal.t("Survey") + "</a></li>";
				}		
				
			}

	        //Show Assessment link
			//For ILT and VC the Assessment Link will enable after the Class Session time is complete.

			//if (assessStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
				if(isILT || isVC){
					if(isVC){
						var sessEndDate =clEnDateFull;
						var meeting_complete = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete) : 30;
						var sesEndDateTime = new Date(sessEndDate);
						sesEndDateTime.setMinutes(sesEndDateTime.getMinutes()+parseInt(meeting_complete));
						var sesEndTimeStamp	= sesEndDateTime.getTime();
						var todayTimeStamp	= todayDate.getTime();
					}else{
						var sessEndDate = enDateFull ;
						var sesEndDateTime = new Date(sessEndDate);
						var sesEndTimeStamp	= sesEndDateTime.getTime();
						var todayTimeStamp	= todayDate.getTime();
					}
					if(id==null || id==undefined || id=='undefined')
			        	id='0';
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId;
					if(preassessStatus == 'TRUE'){
					enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
						Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>"+ "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					if(todayTimeStamp > sesEndTimeStamp){
						enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
								Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
					 }
					}
				}else {
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId+"###"+VersionId;
					
					if(preassessStatus == 'TRUE'){
						
						if(no_primary_html_button=='1'){ //make PRE ASSESSMENT button as primary
							
							html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
											"\" name='takesurvey' onclick='" +
												surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																		escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
													titleRestrictionFadeoutImage(Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>",'exp-sp-mylearning-menulist') + "</a>";
													
							no_primary_html_button='0';
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
											"\" name='takesurvey' onclick='" +
												surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																		escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1,\"enroll-result-container\");' >" +
													Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
						} 													
												
					}
					
					if(assessStatus == 'TRUE'){
						
						if(no_primary_html_button=='1'){ //make POST ASSESSMENT button as primary
							
							html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" + data1 +
							"\" name='takesurvey' onclick='" +
								surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
														escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
									titleRestrictionFadeoutImage(Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>",'exp-sp-mylearning-menulist') + "</a>";
									
							no_primary_html_button='0';
							
						}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
							"\" name='takesurvey' onclick='" +
								surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
														escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0,\"enroll-result-container\");' >" +
									Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
						} 
							
				     }
			 }
			//}

			//Show refer link
				//change by ayyappans for 41930: Survey & Share link gone once Vod/WBT is Launched
				//Share link should be hidden only when session of VC or ILT classes is invalid
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				/*if(((isWBT && (wbtButtonAction == 'Launch' || wbtButtonAction == 'Survey')) ||
					      (isVOD && (vodButtonAction == 'Launch' || vodButtonAction == 'Survey'))) && classStatus == 'lrn_cls_sts_atv'){*/
				if((isWBT || isVOD) && classStatus == 'lrn_cls_sts_atv' && dedClass != 'Y'&& (hideShareLink == 0) && wbtShareButton==null){  
					enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data="'
												+ data1 + '" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\'' +
												classId + '\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +
												Drupal.t("Share") + '</a></li>';
				}
			}
			else {
				// Show Refer link
				/*if (((isWBT && (wbtButtonAction == 'Launch')) ||
				       (isVOD && (vodButtonAction == 'Launch')) ||
				         ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv') {*/
				if ((isWBT || isVOD || ((isILT || isVC) && timeDiffIlt >= 0)) && classStatus == 'lrn_cls_sts_atv' && iltShareButton == null && dedClass != 'Y' && (hideShareLink == 0) && wbtShareButton==null) {
					enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data=\"' + data1 +
									'\" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\''+ classId +
									'\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' + Drupal.t("Share") + '</a></li>';
				}
			}
			
			//36658: Unable to choose another class using "Change Class" function.
			if(RegStatusCode == 'lrn_crs_cmp_enr' && isPricedClass && ecommerce_enabled_status != '' && ecommerce_enabled_status !=null && iltChangeButton == null) {
				regMsg = Drupal.t('MSG692');
				
				if(no_primary_html_button=='1'){ //make CHANGE CLASS button as primary  
					 html += '<a href="javascript:void(0)" class="actionLink enroll-launch vtip" '+
					'name="Change Class" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a>';
						
					no_primary_html_button='0';
					
				}else{ 
					enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" '+
					'name="Change Class" onclick="$(\'body\').data(\'learningcore\').enrollChangeClassErrorMessage(' + '\'registertitle\''  + ',' + '\'' +regMsg +'\'' + ');" >' +
					Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a></li>';
					
				} 
				
				
			} else if((RegStatusCode == 'lrn_crs_cmp_enr' || RegStatusCode == 'lrn_crs_cmp_nsw' && isChangeClass && (isPricedClass == null || isPricedClass == '' || isPricedClass == undefined)) && iltChangeButton == null){
				
				if(no_primary_html_button=='1'){ //make CHANGE CLASS button as primary				 
					html += '<a href="javascript:void(0)" class="actionLink enroll-launch" '+
						'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
						titleRestrictionFadeoutImage(Drupal.t("LBL943") + ' ' +Drupal.t("Class"),"exp-sp-mylearning-menulist") + '</a>';
						no_primary_html_button='0';
						no_drop_down_option='1'; 
				}else{
					enrollAction += '<li class="action-enable"><a href="javascript:void(0)" class="actionLink" '+
						'name="Change Class" onclick="$(\'#learningplan-tab-inner\').data(\'learningplan\').showSwitchClass(' + usrId + ',' + classId + ',' + courseId + ',' + dataId + ',' + '\'Enroll\'' +');" >' +
						Drupal.t("LBL943") + ' ' +Drupal.t("Class") + '</a></li>';
				}
				
			}
			
			/**
			 * 39005: In completed tab "certificate" link and rating option not showing
			 * change by: ayyappas
			 * */
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				// Add the Print Certificate More menu item to HTML in completeAction for Completed items


				/*enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/


				  /*
				   * Start # 0041917 -  Qa link is shown in salesforce app
				   * Added By : Ganesh Babu V, Dec 9th 2014 5:00 PM
				   * Description: Check the print certificate whether it triggers from salesforce or expertusone. callback changed according to trigger
				   * Ticket : #0041917 -  Qa link is shown in salesforce app
				   */


				/*enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";*/

					
				   var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
				   if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1") {
					   if(iltCompleteCertificateButton==null)
					    enrollAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data=\"' + data1 +
						'\" name="Certificate"  onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+dataIdEncrypted+'\',\''+classIdEncrypted+'\',\''+objEval.enrUserId+'\',\'CLASS\',\'div_my_transcript\',800,900);" >' + Drupal.t("LBL205") + '</a></li>';
				   }else if(iltCompleteCertificateButton==null){
				   	
					   	if(no_primary_html_button=='1'){ //make CERTIFICATE button as primary
					   		
					   		html += "<a href='javascript:void(0);' class='actionLink enroll-launch vtip' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + titleRestrictionFadeoutImage(Drupal.t("LBL205"),'exp-sp-mylearning-menulist') + "</a>";
							  
					   		no_primary_html_button='0';
					   		no_drop_down_option='1'; 
					   		
					   	}else{
					   		  enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
							  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataIdEncrypted +
							  "&classid=" + classIdEncrypted + "&userid=" + objEval.enrUserId +
							  "\", \"PrintCertificate\"" +
							  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
							  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";
					   	}
	
				   }


				   /* End # 0041917 -  Qa link is shown in salesforce app */


				if(recertifyCompliance > 0){
					if(classStatus == 'lrn_cls_sts_atv') {
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					
					if(no_primary_html_button=='1'){ //make RECERTIFICATE button as primary
						
							html += "<a href='javascript:void(0);' class='actionLink'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+Drupal.t("Reregister")+"</a>";
										
							no_primary_html_button='0';
					   		no_drop_down_option='1'; 
										
					}else{
							enrollAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink'" +
						"data=\"" + data1 + "\" " +
							"value='" + Drupal.t("Reregister") + "' " +
								"name='" + Drupal.t("Reregister") + "' " +
										"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
											" >"+Drupal.t("Reregister")+"</a></li>";
					}
					
										
										
					}
				}
			}
			
			if(viewButton != 'viewDetails'){
				if(no_primary_html_button=='1'){
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="clsDisabledButton actionLink enroll-launch vtip" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
					no_primary_html_button='0';
				}else{
				  enrollAction += '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';	
				}
				
			}
			// Add html to show the More menu-button/menu (enabled or disabled as appropriate)
			if(enrollAction !='' && ((no_drop_down_option!='1' && RegStatusCode != 'lrn_crs_cmp_inp') || (hideShareLink == 0 && RegStatusCode == 'lrn_crs_cmp_inp'))) { 
					html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'></a></div>";
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + enrollAction + '</ul></div>'; 
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			}
			// add rating option to completed items
			if(RegStatusCode == 'lrn_crs_cmp_cmp') {
				html += "<div id='enroll-node-"+nodeId+"'>";
				html += "<input type='hidden' id = 'lnrenroll-node' value = '"+nodeId+"'>";
				html += starWidgetHtml;
				html += "<div>";
			}
		}

		// Prepare Action button HTML for My Enrollments 'Completed' tab
		//Commented For Code Refactoring Unwanted Code To Remove in Future
		/*else if(RegStatusCode == 'lrn_crs_cmp_cmp') {
	      var isILT = false;
	      var isWBT = false;
	      var isVC = false;
	      var isVOD = false;
	      var wbtButtonAction = null; // Variables later used to determined what links to show in the More menu.
	      var vodButtonAction = null;

			if (dataDelTypeCode == 'lrn_cls_dty_wbt') { // Show correct button for the WBT class
			  isWBT = true;
				// If is launchable, show the Launch button
				if (IsLaunchable) {
					var launchLabelVal  = Drupal.t("LBL199");//(launchInfo.length > 1) ? Drupal.t('LBL816') : Drupal.t("LBL199");
					wbtButtonAction = 'Launch';
					html += "<input type='button' class='actionLink enroll-launch' " +
					"data=\"" + multiContentData + "\" " +
						"value='" + launchLabelVal + "' " +
						"id='launch" + dataId + "' " +
							"name='" + SMARTPORTAL.t("Launch") + "' " +
									callLaunchUrlFn +
										" >";
				}
				else {
					viewButton = 'viewDetails';
					html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
				}
			}
			else if (dataDelTypeCode == 'lrn_cls_dty_vod') {
	       isVOD = true;
	        // If is launchable, show the Launch button
	        if (IsLaunchable) {
	          vodButtonAction = 'Launch';
	          if (launchInfo.length > 1) {
	            html += "<input type='button' class='actionLink enroll-launch' " +
	            "data=\"" + multiContentData + "\" " +  "data-data1=\"" + data1 + "\" " +
	                "value='" + Drupal.t("LBL199") + "' " +
	                "id='launch" + dataId + "' " +
	                  "name='" + Drupal.t("LBL199") + "' " +
	                     callLaunchUrlFn +
	                       " >";
	          } else {
              html += "<a class='actionLink enroll-launch use-ajax ctools-modal-ctools-video-style' " +
                          "title='" + Drupal.t("LBL199") + "' " +
                            callLaunchUrlFn +
                              " >" +
                                Drupal.t("LBL199") +
                                   "</a>";
	          }
	        }
	        // Otherwise show the Survey button if survey is present and surveylearner module button is enabled
	        // Otherwise show the Refer button
	        else {
	          //vodButtonAction = 'Refer';
	        	viewButton = 'viewDetails';
	        	html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';
	        }
			}
			else if (dataDelTypeCode == 'lrn_cls_dty_vcl') {
			  isVC = true;
				var title = 'Session Details';
				var message = Drupal.t("MSG419");
				html += "<input type='button' class='actionLink enroll-launch " +
					"' data=\"" + contdata + "\" value='" + Drupal.t("LBL880") +
					"' onclick='" + callDialogObj + ".callMessageWindow(\"" + title + "\",\"" + message + "\");'" +
					"' >";
			} else {
				// For ILT classes, the button is always Refer on Completed tab
				isILT = true;
				viewButton = 'viewDetails';
				html += '<a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="action-link enroll-launch" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a>';

			}

			// Prepare the More menu items HTML in completeAction
			var completeAction = '';
			if(viewButton != 'viewDetails') {
				completeAction += '<li class="action-enable"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
			}
			var surObj		= '$("#block-take-survey").data("surveylearner")';
			if (surStatus == 'TRUE'  && (availableFunctionalities.exp_sp_surveylearner) &&
					((isWBT && wbtButtonAction == 'Launch') || (isVOD && vodButtonAction == 'Launch') || isILT || isVC) ) {

				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" + surObj + ".callTakeSurveyToClass(" +
									classId + ",\"" + escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"survey\",\"\"," + dataId + ");' >" + Drupal.t("Survey") + "</a></li>";
			}

			// Commented - Assessment not available after class completion
			if(((isWBT && (wbtButtonAction == 'Launch' || wbtButtonAction == 'Survey')) ||
				      (isVOD && (vodButtonAction == 'Launch' || vodButtonAction == 'Survey'))) && classStatus == 'lrn_cls_sts_atv'){
					completeAction += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" data="'
											+ data1 + '" name="Refer" onclick="$(\'body\').data(\'refercourse\').getReferDetails(\'' +
											classId + '\',\'cre_sys_obt_cls\',\'enroll-result-container\');" >' +
											Drupal.t("Share") + '</a></li>';
			}

			if(recertifyCompliance > 0){
				var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink'" +
				"data=\"" + data1 + "\" " +
					"value='" + Drupal.t("Reregister") + "' " +
						"name='" + Drupal.t("Reregister") + "' " +
								"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
									" >"+Drupal.t("Reregister")+"</a></li>";
			}

			//Show Assessment link
			//For ILT and VC the Assessment Link will enable after the Class Session time is complete.

			//if (assessStatus == 'TRUE' && (availableFunctionalities.exp_sp_surveylearner)) {
				if(isILT || isVC){
					var sessEndDate = isVC ? clEnDateFull : enDateFull ;
					var sesEndDateTime = new Date(sessEndDate);
					var sesEndTimeStamp	= sesEndDateTime.getTime();
					var todayTimeStamp	= todayDate.getTime();
					if(id==null || id==undefined || id=='undefined')
			        	id='0';
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId;
					if(preassessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1);' >" +
						Drupal.t("Assessment")+ "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>"+ "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					if(todayTimeStamp > sesEndTimeStamp){
						completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
									"\" name='takesurvey' onclick='" +
										surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0);' >" +
								Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
					 }
					}
				}else {
					var paramsUpdScore 		= id+"###"+dataId+"###"+courseId+"###"+classId+"###"+VersionId;
					if(preassessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
										"\" name='takesurvey' onclick='" +
											surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
																	escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",1);' >" +
												Drupal.t("Assessment") +  "<span class='pre-assessment-container'>" + " (" + Drupal.t('LBL1253')+ ")" + "</span>" + "</a></li>";
					}
					if(assessStatus == 'TRUE'){
					completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" + data1 +
					"\" name='takesurvey' onclick='" +
						surObj + ".callTakeSurveyToClass(" + classId + ",\"" +
												escape(classTitle) + "\",\"cre_sys_obt_cls\",  \"assessment\", \""+paramsUpdScore+ "\"," + dataId + ",0);' >" +
							Drupal.t("Assessment") +  "<span class='post-assessment-container'>" + " (" + Drupal.t('LBL871')+ ")" + "</span>" + "</a></li>";
				}
			}

			// Add the Print Certificate More menu item to HTML in completeAction
			if(RegStatusCode == 'lrn_crs_cmp_cmp'){
				completeAction += "<li class='action-enable'><a href='javascript:void(0);' class='actionLink' data=\"" +
								  data1 + "\" name='Certificate' onclick='window.open(\"printcertificate.php?enrollid=" + dataId +
								  "&classid=" + classId + "&userid=" + objEval.enrUserId +
								  "\", \"PrintCertificate\"" +
								  ", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, " +
								  "scrollbars=yes, copyhistory=yes, resizable=yes\")'>" + Drupal.t("LBL205") + "</a></li>";
			}

			// Add the Rating More menu item to HTML in completeAction
			//completeAction += "<li><a href='javascript:void(0);' class='actionLink' data=\""+data1+"\" name='Rating' onclick='"+obj+".testAction(this);' >"+SMARTPORTAL.t("Rating")+"</a></li>";

			// Add html to show the more menu (enabled or disabled as appropriate)
			if(completeAction !=''){
				html += "<a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a>";
				html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
			}
			else{
				html += "<a class='enroll-launch-more-gray'></a>";
			}

			html += "<div id='enroll-node-"+nodeId+"'>";
			html += "<input type='hidden' id = 'lnrenroll-node' value = '"+nodeId+"'>";
			html += starWidgetHtml;
			html += "<div>";
		}*/else if( RegStatusCode == 'lrn_crs_cmp_exp' ){
					var recertifyObj = '$("#learner-enrollment-tab-inner").data("enrollment")';
					viewButton = 'viewDetails';
					if(classStatus == 'lrn_cls_sts_atv' && recertifyCompliance > 0) {
					html += "<label class='enroll-launch-full'><a type='button' title='"+Drupal.t("Reregister")+"' class='actionLink enroll-launch mypgm-recertify vtip'" +
					"data=\"" + data1 + "\" " +
						"value='" + Drupal.t("Reregister") + "' " +
							"name='" + Drupal.t("Reregister") + "' " +
									"onclick='" + recertifyObj + ".showCompliancePopup(" +usrId+ ","+ classId + ","+ courseId + ","+ dataId + ");'" +
										" >"+titleRestrictionFadeoutImage(Drupal.t("Reregister"),'exp-sp-mylearning-menulist')+"</a></label>";
		
									//	completeAction = '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
		
				//	html += "<div class='dd-btn'><span class='dd-arrow'></span><a class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'> </a></div>";
				//	html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + completeAction + '</ul></div>';
				}
					else {
				html += '<label class="enroll-launch-full">'+'<span style="display:none;" id="class-accodion-'+dataId+'" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'></span><a href="javascript:void(0);"  class="action-link enroll-launch clsDisabledButton" >'+Drupal.t('LBL816')+'</a>'+'</label>';
					}
		}
		// Prepare Action button HTML for My Enrollments 'Pending' tab
		else if (RegStatusCode == 'lrn_crs_reg_ppm' || RegStatusCode == 'lrn_crs_reg_wtl' ) {
				if(setDropButton == true) {
					html += "<label class=\"enroll-launch-full\"><input type='button' class='actionLink enroll-launch' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" onclick='"+obj+".dropEnroll(this);' name='"+SMARTPORTAL.t("Cancel")+"' ></label>";
					//html += "<div  class='dd-btn'><span class='dd-arrow'></span><a style=\"display:none;\" class='enroll-launch-more' title='" + Drupal.t("LBL713") +"' onclick='" + obj + ".showMoreAction(this)' onblur='" + obj + ".hideMoreAction(this)'></a></div>";
					var	pendingAction = '<li class="action-enable" style="display:none;"><a id="class-accodion-'+dataId+'" href="javascript:void(0);" data="'+data1+'" class="actionLink" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'>'+Drupal.t('LBL816')+'</a></li>';
					html += '<div class="enroll-action"><ul class="enroll-drop-down-list">' + pendingAction + '</ul></div>';
				}
				else{
					html += "<input type='button' class='actionLink enroll-launch-gray' value='"+Drupal.t("LBL109")+"' data=\""+data1+"\" name='"+SMARTPORTAL.t("Cancel")+"' >";
				}
		}
		if(RegStatusCode == 'lrn_crs_cmp_inc' || RegStatusCode =='lrn_crs_reg_can' || RegStatusCode =='lrn_crs_cmp_nsw') {
				html += '<label class="enroll-launch-full">'+'<span style="display:none;" id="class-accodion-'+dataId+'" onclick=\''+obj+'.addAccordionToTable1('+dataId+',this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getClassDetails,this,'+obj+',true); \'></span><a href="javascript:void(0);"  class="action-link enroll-launch clsDisabledButton" >'+Drupal.t('LBL816')+'</a>'+'</label>';
		}
		html += "</div>";
		return html;
	 }catch(e){
			// to do
		}
	},

	launchMultiContent : function(enrollId,obj){
		try{
		var closeButt={};
	    $('#launch-wizard').remove();
	  	html="";
		html+='<div id="launch-wizard" class="launch-wizard-content" style="display:none; padding: 0px;">';
	    html+= this.getLaunchDetails(enrollId);
	    html+='</div>';
	    $("body").append(html);
	    Drupal.attachBehaviors();
	    $('.enrollment-launch-table tr:last').css('border-bottom','0px');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		var iframeWidth = 0;
		//var iframeDisWidth = Drupal.settings.widget.widget_details.display_size;
		if(Drupal.settings.widget.widgetCallback == true)
			var iframeWidth = $( document ).width();
		if(iframeWidth < 775 && iframeWidth!=0){
			var cpopwidth = 600;
		}else
			var cpopwidth = 675;

	    $("#launch-wizard").dialog({
	        position:[(getWindowWidth()-700)/2,100],
	        bgiframe: true,
	        width:cpopwidth,
	        resizable:false,
	        modal: true,
	        title: Drupal.t("Content"),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         },
	         zIndex: 999 //0034567: UI issue in while launching Video -> Fix: z-index added to the modal dialog
	    });

	    $("#launch-wizard").show();

		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
			}
	  	if(this.currTheme == "expertusoneV2"){
  		   $('#select-class-onclick-dialog').prevAll().removeAttr('select-class-onclick-dialog');
	 	   $('#launch-wizard').closest('.ui-dialog').wrap("<div id='select-class-onclick-dialog'></div>");
	       $('#select-class-onclick-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		   changeChildDialogPopUI('select-class-onclick-dialog');
		   $('#select-class-onclick-dialog').prev('.ui-widget-overlay').css('display','none');
		   $('#select-class-dialog').prev('.ui-widget-overlay').css('display','none');
		   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
		   }
		   $('#select-class-onclick-dialog #launch-wizard').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
		   if(iframeWidth < 775 && iframeWidth!=0)
			   $('#select-class-onclick-dialog .ui-widget').css('left','10px');
	  	}

		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
	        $("#launch-wizard").remove();
	        $('#select-class-onclick-dialog').next('.ui-widget-overlay').css('display','block');
	        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#select-class-onclick-dialog").remove();
	    });
		resetFadeOutByClass('#launch-wizard','item-title-code','line-item-container','mylearning');
	    //SCrollbar for content dialogbox
		
		
		if(document.getElementById('learner-enrollment-tab-inner'))
	    	$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
	    $('#paintEnrollmentResults tr:first-child td .title_close').trigger("click");
		}catch(e){
			// to do
		}
		vtip();
	},
	
	getLaunchSurveyDetails : function(data) {
		try{
		var data = eval(data);
		var html = '<div id="launch-content-container">';
		html += '<table id="paintEnrollmentResults" cellpadding="0" cellspacing="0" width="100%" border="0" class="enrollment-launch-table">';
		$(data).each(function(){
			$(this).each(function(){
				var launchFrom = '';
					var surveyId = $(this).attr('survey_id');
					var surveyTitle = $(this).attr('survey_title');
					var enrollId = $(this).attr('enroll_id');
					var enrollDate = decodeURIComponent(unescape($(this).attr('enroll_date')));
					var enrollStatus = $(this).attr('enroll_status');
					var objectId = $(this).attr('object_id');
					var objectTitle = $(this).attr('object_title');
					var objectType = $(this).attr('object_type');
					var surveyCount = $(this).attr('survey_count');
					var launchElligible = ''; 
					var prevCourseTitle = ''; 
					var masterEnrollId = ''; 
					var clickparams = '';
					var buttonValue = '';
					var buttonclass = '';
					var deliverytype = '';
					var programId = '';
					
					if(surveyCount==0) {
						if(Drupal.settings.mylearning.user.survey_multipleTP > 0) {
							launchElligible = $(this).attr('launch_elligible'); 
							prevCourseTitle = $(this).attr('prev_coursetitle'); 
							masterEnrollId = $(this).attr('master_enrollid');
							programId = $(this).attr('program_id');
							
							clickparams = "$('.ui-dialog-titlebar-close').click();";
							if (launchElligible !== '' && launchElligible == 0) {
								
								if(masterEnrollId=='')
									clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-result-container');";
								else {
									if(objectType == 'cre_sys_obt_crt') { clickparams += "$('#mod-accodion-"+programId+"').click();";}
									clickparams += "$('#lp_seemore_prg_"+masterEnrollId+"').click();";
									clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+Drupal.settings.mylearning.user.survey_objectname+"','"+Drupal.settings.mylearning.user.survey_objecttype+"','survey','',"+enrollId+",'NULL','enroll-lp-result-container');";
								}
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
							}
							else {
								clickparams += "$('body').data('learningcore').callMessageWindow('Survey','"+Drupal.t('You need to complete')+" "+decodeURIComponent(unescape(prevCourseTitle))+" "+Drupal.t('Course')+"')";
							}
						} else {
							if(Drupal.settings.mylearning.user.survey_objecttype=='cre_sys_obt_cls'){	
								clickparams = "$('.ui-dialog-titlebar-close').click();";
								clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-result-container');";
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
								clickparams += "setTimeout(function(){$('.survey-main-holder .ui-dialog-buttonset .removebutton').css('display','none');}, 200);";
							} else{
								clickparams = "$('.ui-dialog-titlebar-close').click();";
								clickparams += "$('#block-take-survey').data('surveylearner').callTakeSurveyToClass("+objectId+",'"+objectTitle+"','"+objectType+"','survey','',"+enrollId+",'NULL','enroll-lp-result-container');";
								clickparams += "$('#block-take-survey').data('surveylearner').moveNext("+surveyId+");";
								clickparams += "setTimeout(function(){$('.survey-main-holder .ui-dialog-buttonset .removebutton').css('display','none');}, 200);";
							}
						}
						buttonValue = Drupal.t('LBL199');
						buttonclass = 'enroll-launch';
					} else {
						buttonValue = Drupal.t('Completed');
						buttonclass = 'enroll-completed';
					}
					
					if(Drupal.settings.mylearning.user.survey_recertify==1) {
						deliverytype = Drupal.t('Certification');
					} else {
						deliverytype = Drupal.t('Class');
					}
				    
				  html += '<tr id="lesson-launch-' + enrollId + '">';
				  html +=   '<td class="enroll-launch-column1">';
				  html +=     '<div class="enroll-course-title">';
				  html +=       '<span id="lesson_title_' + enrollId + '"></span>';
				  html +=       '<span id="titleAccEn_'+enrollId+'" class="item-title" >';
					html +=         '<span class="enroll-class-title"><span class="vtip"' +
					                       ' title="'+unescape($(this).attr('survey_title')).replace(/"/g, '&quot;')+'" href="javascript:void(0);">'+
					                       ' '+titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('survey_title'))),'exp-sp-lnrenrollment-content-title',150);+'';
					  html += '</span></br>';
				  html +=           '<div class="item-title-code individual-content">';
				  if(Drupal.settings.mylearning.user.survey_multipleTP > 0) {
						 html +=   '<span id="lesson_status_'+enrollId+'"><span title="'+Drupal.t('LBL704')+': '+enrollDate+'" class="vtip">'+Drupal.t('LBL704')+': '+ enrollDate+'</span><span class="narrow-search-results-item-detail-pipe-line sharelink-edit-delete-icons">|</span></span>';
						 html +=   '<span class="assoc_object_title" id="lesson_status_'+enrollId+'"><span class="vtip content-subtitle"' +
						  ' title="'+unescape($(this).attr('object_title')).replace(/"/g, '&quot;')+'">'+
						  ' '+Drupal.t('LBL083')+': '+titleRestrictionFadeoutImage(decodeURIComponent(unescape($(this).attr('object_title'))),'exp-sp-lnrenrollment-content-title',45);+'';
						  html +=   '</span></span>';
					  } else {
						  html +=  '<span id="lesson_status_'+enrollId+'"><span title="'+deliverytype+' '+Drupal.t('LBL704')+': '+enrollDate+'" class="vtip">'+deliverytype+' '+Drupal.t('LBL704')+': '+ enrollDate+'</span></span>';
				  }
				  html +=           '</div>';
				  html +=         '</span>';
				  html +=       '</span>';
				  html +=     '</div>';
				  html +=   '</td>';
				  html +=   '<td class="enroll-launch-column2">';
				  html +=   '<div class="enroll-main-list">';
					html +=     '<label id="' + enrollId + '_' + surveyId + '_launch_button_lbl"' +
					    ' class="enroll-launch-full launch_button_lbl">';
					html +=       '<input type="button" id="' + enrollId + '_' + surveyId + '_launch_button"' +
					     ' class="actionLink '+buttonclass+'"' +
					     'onclick="'+clickparams+'"'+
					     ' value="' + buttonValue+'"' +
					     ' >';
					html +=     '</label>';
					html +=   '</div>';
					html += '</td></tr>';
				
			});
		});

	   html +='</table>';
	   html +='</div>';

	   return html;
		}catch(e){
			// to do
			// console.log(e);
		}
	},
	
	launchMultiSurvey : function(data){
		try{
		var closeButt={};
	    $('#launch-wizard').remove();
	  	html="";
		html+='<div id="launch-wizard" class="launch-wizard-content" style="display:none; padding: 0px;">';
	    html+= this.getLaunchSurveyDetails(data);
	    html+='</div>';
	    $("body").append(html);
	    Drupal.attachBehaviors();
	    $('.enrollment-launch-table tr:last').css('border-bottom','0px');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		var iframeWidth = 0;
		if(Drupal.settings.widget.widgetCallback == true)
			var iframeWidth = $( document ).width();
		if(iframeWidth < 775 && iframeWidth!=0){
			var cpopwidth = 600;
		}else
			var cpopwidth = 675;

	    $("#launch-wizard").dialog({
	        position:[(getWindowWidth()-700)/2,100],
	        bgiframe: true,
	        width:cpopwidth,
	        resizable:false,
	        modal: true,
	        title: Drupal.t("Survey For")+'&nbsp;'+titleRestrictionFadeoutImage(decodeURIComponent(unescape(Drupal.settings.mylearning.user.survey_objectname)),'exp-sp-lnrenrollment-content-title',45),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         },
	         zIndex: 999 //0034567: UI issue in while launching Video -> Fix: z-index added to the modal dialog
	    });
	    
	    $('.assoc_object_title .fade-out-title-container').css('margin-bottom','-6px');
		$('.assoc_object_title .content-subtitle').css({'overflow':'hidden', 'width': '342px', 'white-space':'nowrap', 'position':'absolute'});	
		$('.assoc_object_title .content-subtitle .fade-out-title-container').css('max-width','312px');
		$('.enroll-class-title .fade-out-title-container .title-lengthy-text').css({'white-space':'normal','line-height':'16px','margin-top':'2px'});
		$('.assoc_object_title .content-subtitle .fade-out-title-container .title-lengthy-text').css({'white-space':'nowrap','margin-bottom':'8px'});
		$('.launch-wizard-content #paintEnrollmentResults .enroll-course-title .enroll-class-title .exp-sp-lnrenrollment-content-title').css('margin-bottom','-12px');
		$('#launch-wizard #paintEnrollmentResults .enroll-course-title .enroll-class-title .item-title-code').css('padding-top','0');
		$('#launch-wizard .enroll-launch-full').css({'width':'auto', 'margin-left':'3px'});
		$('#launch-wizard .enroll-launch-full .enroll-launch').css({'border-radius':'12px', 'height':'24px', 'padding':'2px 0'});
	    $("#launch-wizard").show();
	   
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
			}
	  	if(this.currTheme == "expertusoneV2"){
  		   $('#select-class-onclick-dialog').prevAll().removeAttr('select-class-onclick-dialog');
	 	   $('#launch-wizard').closest('.ui-dialog').wrap("<div id='select-class-onclick-dialog'></div>");
	       $('#select-class-onclick-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
		   changeChildDialogPopUI('select-class-onclick-dialog');
		   $('#select-class-onclick-dialog').prev('.ui-widget-overlay').css('display','none');
		   $('#select-class-dialog').prev('.ui-widget-overlay').css('display','none');
		   if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
		   $('#select-class-onclick-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
		   }
		   $('#select-class-onclick-dialog #launch-wizard').after('<div class="popupBotLeftCorner ui-helper-clearfix"><div class="popupBotRightCorner"><div class="popupBotmiddle"></div></div></div>');
		   if(iframeWidth < 775 && iframeWidth!=0)
			   $('#select-class-onclick-dialog .ui-widget').css('left','10px');
	  	}

		$('.ui-dialog-titlebar-close,.removebutton').click(function(){
	        $("#launch-wizard").remove();
	        $('#select-class-onclick-dialog').next('.ui-widget-overlay').css('display','block');
	        $('#select-class-dialog').next('.ui-widget-overlay').css('display','block');
	        $("#select-class-onclick-dialog").remove();
	    });
		
		$('#select-class-onclick-dialog .ui-dialog .ui-dialog-titlebar .fade-out-title-container').css({'float':'right','margin-bottom':'-9px'});
		$('#select-class-onclick-dialog .ui-dialog .ui-dialog-titlebar .fade-out-title-container .fade-out-image').css('background','rgba(0, 0, 0, 0) linear-gradient(to right, transparent, #27245e) repeat scroll 0 0');

	    //SCrollbar for content dialogbox
		if(document.getElementById('learner-enrollment-tab-inner'))
	    	$("#learner-enrollment-tab-inner").data("enrollment").scrollBarContentDialog();
		/*if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();*/
	    $('#paintEnrollmentResults tr:first-child td .title_close').trigger("click");
		}catch(e){
			// to do
		}
		vtip();
	},

	//SCROLLBAR IMPLEMENTATION FOR CONTENT SCREEN
	scrollBarContentDialog : function(){
		try{
		var contentDialogHeight = $("#launch-content-container").height();
		if(contentDialogHeight > 550) {
			$(".launch-wizard-content").css('height',550);
			$(".launch-wizard-content").css('overflow','auto');
		} else if(contentDialogHeight <= 550) {
			$(".launch-wizard-content").css('height','auto').css('min-height','70px');
		}
		}catch(e){
			// to do
		}
	},

	showMoreAction : function(obj) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	var position = $(obj).position();
		var posLeft  = Math.round(position.left);
		var posTop   = Math.round(position.top);
		$('.enroll-drop-down-list').toggle();
		$('.enroll-action').hide();
		if(this.currTheme == "expertusoneV2"){			
			if (navigator.userAgent.indexOf("Chrome")>0) {
				$(obj).parent().siblings('.enroll-action').css('position','absolute');
				$(obj).parent().siblings('.enroll-action').css('margin-top',"21px");
			//	$(obj).parent().siblings('.enroll-action').css('right','2px');
				$(obj).parent().siblings('.enroll-action').css('top','2px');
			//	$(obj).parent().siblings('.enroll-action').css('width','90px');
				$(obj).parent().siblings('.enroll-action').css('z-index','-1');
				$(obj).parent().siblings('.enroll-action').slideToggle();
			}else{
				$(obj).parent().siblings('.enroll-action').css('position','');
				$(obj).parent().siblings('.enroll-action').css('margin-top',"21px");
				$(obj).parent().siblings('.enroll-action').slideToggle();
			}
//			if(Drupal.settings.mylearning_right===false){
//
//				$(obj).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
//			      "position":"absolute","z-index":"1000"
//			    });
//				$(obj).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
//				      "position":"absolute","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
//				      "position":"relative","z-index":"0"
//				    });
//				$(obj).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
//				      "position":"relative","z-index":"0"
//				    });
//			}else{
//				$(obj).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
//				      "position":"absolute","margin-right":"0","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
//				      "position":"absolute","margin-right":"0","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
//				      "position":"relative","margin-right":"6px","z-index":"0"
//				    });
//				$(obj).siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
//				      "position":"relative","margin-right":"6px","z-index":"0"
//				    });
//			}
			$(obj).parent().siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
			      "z-index":"1000"
			    });
			$(obj).parent().siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
			      "z-index":"1000"
			    });
			$(obj).parent().siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
			      "z-index":"0"
			    });
			$(obj).parent().siblings('.enroll-action').parents('.course-detail-section-innerRecord').closest('tr').nextAll('tr').find('.enroll-lp-main-list,.enroll-main-list').css({
			      "z-index":"0"
			    });
			}else{
			$(obj).parent().siblings('.enroll-action').css('position','absolute');
			$(obj).parent().siblings('.enroll-action').css('right','16px');
			$(obj).parent().siblings('.enroll-action').css('top',posTop+20);
			$(obj).parent().siblings('.enroll-action').slideToggle("");
			$(obj).parent().siblings('.iframe-mylearning #innerWidgetTagEnroll .enroll-action').css('right','321px'); // salesforce
		}
		}catch(e){
			// to do
		}
	},

	hideMoreAction : function(obj) {
		try{
			this.prevMoreObj = '';
		}catch(e){
			// to do
		}
	},
	getDropPolicy:function(data, loaderDiv)
	{
		try{
			$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			var dataobj= eval($(data).attr("data"));

			var isCommerceEnabled='';
		    var assMand =0;
		    
		    if(dataobj.assMand =='Y' && dataobj.is_exempted != 1){assMand =1;}  
		  
		    var mandByRole	= 0;
		    if(dataobj.mro =='Mandatory' && dataobj.is_exempted != 1){mandByRole =1;}

		    if(!assMand && !mandByRole){
			    if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "")
			    {
			    	isCommerceEnabled = "1";
			    }
		    }
			var closeButt={};
		    $('#dropMsg-wizard').remove();
		  	html="";
			html+='<div id="dropMsg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" class="dropMsg-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
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
		        title: Drupal.t("LBL109"),
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:true,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		    $("#dropMsg-wizard").show();

			$('.ui-dialog-titlebar-close').click(function(){
		        $("#dropMsg-wizard").remove();
		    });
			$("#learner-enrollment-tab-inner").data("enrollment").getDropPolicyCall(dataobj.RegId,dataobj.BaseType,dataobj.classid,isCommerceEnabled,assMand,dataobj.clstitle,mandByRole);
		}catch(e){
			// to do
		}
	},



	getDropPolicyCall : function(enrollId,baseType,classid,isCommerceEnabled,assMand,clstitle,mandByRole)
	{
		try{
		var userId = this.enrUserId;
		var url = this.constructUrl("ajax/get-droppolicy/" + userId +  "/" + baseType + "/" + enrollId+"/"+classid+"/"+isCommerceEnabled+"/"+assMand+"/"+mandByRole);
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
					$("#dropmsg-content").html('<span>'+Drupal.t("MSG263")+'</span><br /><i>"'+unescape(clstitle)+'"</i>');
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand); $("#dropmsg-content").html(Drupal.t('MSG424'));};
				}else if(drop_policy_info.next_action=="showmsg")
				{
					$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_info.msg)+"</span>");

					var html="";
					html+='<span><b>'+Drupal.t('LBL083')+' : </b>'+unescape(clstitle)+"</span>";
					html+="<br/><br/>";
					html+="<span>"+SMARTPORTAL.t(drop_policy_info.msg)+'</span>';
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
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
					  html+='<span>' + Drupal.t('MSG402')+' ' + drop_policy_info.currency_type +' '+ drop_policy_info.deducted_amount +' '+Drupal.t('MSG401')+"</span>";
					  html+="<br/><br/>";
					  html+='<span class="dropmsgTitle">'+Drupal.t('LBL1165')+': </span>'+drop_policy_info.currency_type+" "+drop_policy_info.refund_amt;
					  html+="<br/><br/>";
					  html+="<span>"+Drupal.t("MSG266")+'</span>';
					}
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
					closeButt[Drupal.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"true",isCommerceEnabled,assMand);  $("#dropmsg-content").html(Drupal.t("MSG424")); };
				}
				 $("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
				 $("#dropMsg-wizard").dialog("open");
				 $("#dropMsg-wizard").dialog('option', 'buttons', closeButt);

				 //Append div script
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end()
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

	dropEnroll : function(data){
		try{
		this.getDropPolicy(data);
		}catch(e){
			// to do
		}
	},

	dropEnrollCall : function(enrollId,baseType,refundflag,isCommerceEnabled,assMand){  
		try{
		closeButt=new Array();
		$("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
		var userId = this.enrUserId;
		var url = this.constructUrl("ajax/drop-enroll/" + userId +  "/" + baseType + "/" + enrollId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/0");
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				if(drop_policy_result.status=="success")
				{
				 	var dialogObj=$("#dropMsg-wizard").dialog();
				 	dialogObj.dialog('destroy');
				 	dialogObj.dialog('close');
				 	$('#dropMsg-wizard').remove();
				 	var enrStr = Drupal.settings.myenrolmentSearchStr;
					if (typeof enrStr == 'undefined' || enrStr == null || enrStr == undefined){
						$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp|lrn_crs_cmp_att","Enrollmentpart");
					}else {
				    	$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment(enrStr);
				    }
					if(document.getElementById('learningplan-tab-inner')) {
						$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_enr|lrn_tpm_ovr_inp","EnrollmentLP");
					}
					// Reload Instructor view
					if(document.getElementById('instructor-tab-inner')) {
						var insSelTab = ($("#scheduled_tab").parent().hasClass('selected') === true) ? "scheduled" : "delivered";
						$("#instructor-tab-inner").data("instructordesk").callInstructor(insSelTab);
					}
			    	// Reload My-Calendar after enrollment dropped
			    	$("#catalog-admin-cal").data("mycalendar").reloadMyCalendar();
				}
				else {
					$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_result.msg)+"</span>");
				}
			}
	    });
		}catch(e){
			// to do
		}
	},

		showCompliancePopup : function(usrId,classId,courseId,dataId){
		var popupObj = $("#learningplan-tab-inner").data("learningplan");
		var obj = $('body').data('learningcore');
		var url = this.constructUrl("ajax/compliance-class-cnt/"+usrId + "/" + classId +"/" + courseId+"/" + dataId);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '',  //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				returnMsg = $.trim(result);
				if(returnMsg == 'MultiRegister') {
					popupObj.showSwitchClass(usrId,classId,courseId,dataId,"compliance");
				}else if(returnMsg == 'noclasses' || returnMsg == 'pricedclass'){
					regMsg = (returnMsg == 'noclasses') ? Drupal.t('MSG379') : Drupal.t('Only priced class is available.Register from Catalog page.');
					obj.enrollChangeClassErrorMessage('compliance register', regMsg);//, '', usrId, courseId, classId, '','');
				}else{
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
			        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
				}
			}
	    });
	},

	testAction : function(obj){
		try{
		}catch(e){
			// to do
		}

	},
	loadComplete : function(){
	 try{
	    var recs = $("#paintEnrollmentResults").getGridParam("records");
	    if (recs == 0) {
	        $("#paintEnrollmentResults").html(Drupal.t("MSG381")+'.');
	    }
	 }catch(e){
			// to do
		}
	},

	showHide : function (strOne,strTwo) {
		try{
		$('#'+strTwo).toggle();
			var classShowHide = $('#'+strOne).hasClass('clsShow');
			if(classShowHide){
				$('#'+strOne).removeClass('clsShow');
				$('#'+strOne).addClass('clsHide');
			}else{
				$('#'+strOne).removeClass('clsHide');
				$('#'+strOne).addClass('clsShow');
			}
		}catch(e){
			// to do
		}
	},


	getConsolidatedQuizStatus : function(contentId, launchInfo) {
	  //console.log(launchInfo);
		var quizStatus = "";
		var lessonCount = 0;
		$(launchInfo).each(function(i, launch)  {
			var contentInfo = launch[0];
			if(contentInfo.ContentId == contentId) {	//if multiple contents attached to single enrollment, separate status should be considered
				lessonCount ++;
				var successStatus = contentInfo.contentQuizStatus;
				//console.log('hi' +contentInfo);
				if (successStatus == "failed" || successStatus == 'incomplete' || successStatus.trim() == '') {
					quizStatus = successStatus;
					return false;
				}else if( contentInfo.ContentCompletionStatus == 'completed' || (contentInfo.ContentCompletionStatus == '' && contentInfo.contentType!="Tin Can")){			
					quizStatus = "passed";
				} else {
					quizStatus = "";
				}
			}
		});
		return quizStatus;
	},

	capitalizeFirstLetter : function(string) {
	    return string.charAt(0).toUpperCase() + string.slice(1);
	}
	
});

$.extend($.ui.enrollment.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);



$(function() {
	try{
		$( "#learner-enrollment-tab-inner" ).enrollment();
		if(document.getElementById('learner-enrollment-tab-inner')) {
			$('#first_pager').click( function(e) {
				try{
				if(!$('#first_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#prev_pager').click( function(e) {
				try{
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#next_pager').click( function(e) {
				try{
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#last_pager').click( function(e) {
				try{
				if(!$('#last_pager').hasClass('ui-state-disabled')) {
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
				}catch(e){
					// to do
				}
			});

			$('#pager .ui-pg-selbox').bind('change',function() {
				try{
				$('#gview_paintEnrollmentResults').css('min-height','auto');
				$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);
				}catch(e){
					// to do
				}
			});

			$("#pager .ui-pg-input").keyup(function(event){
				if(event.keyCode == 13){
					$('#gview_paintEnrollmentResults').css('min-height','auto');
					$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
				}
			});
			//select record count in veiw per page
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				$('#enroll-result-container .page-show-prev').bind('click',function() {
					if(parseInt($("#pg_pager .page_count_view").attr('id')) < 0){
						$("#pg_pager .page_count_view").attr('id','0');
					}else{
						$('#gview_paintEnrollmentResults').css('min-height','100px');
						$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
						$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);

					}
				});
				$('#enroll-result-container .page-show-next').bind('click',function() {
					if(parseInt($("#pg_pager .page_count_view").attr('id')) >= parseInt($("#pg_pager .page-total-view").attr('id'))){
						$("#pg_pager .page_count_view").attr('id',($("#pg_pager .page_count_view").attr('id')-1));
					}else{
						$('#gview_paintEnrollmentResults').css('min-height','100px');
						$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
						$("#learner-enrollment-tab-inner").data("enrollment").hidePageControls(false);

					}
				});

		      }


		}
		if(document.getElementById('lnr-catalog-search'))
			$("#lnr-catalog-search").enrollment();
	}catch(e){
		// to do
	}
});

function refresh_enrolled(window,timer){
	if(!window.closed){
	var window = window;
		setTimeout(refresh_enrolled,10000,window);
	}else{
		reload_page();
		refresh_time(timer);
	}
	
}

function refresh_time(timer){
	setTimeout(reload_page,timer);
}

function reload_page(){
	if(document.getElementById('loaderdivenroll-result-container')){
		}else{
		$("#paintEnrollmentResults").setGridParam().trigger("reloadGrid");
}	
}
