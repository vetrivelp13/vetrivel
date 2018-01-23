(function($) {
	var dWidth='';

	$.widget("ui.instructordesk", {
	_init: function() {
	 try{
		var self = this;
		dWidth = $("#instructor-result-container").width();
		this.instructortab = 'scheduled';
		this.localeArray = new Array();
		this.localeJSON = '';
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2")
			this.borderColor = "1px solid #EDEDED";
		else
			this.borderColor = "1px solid #cccccc";
		$("#instructor-tab-inner").data("instructordesk").markcomplete='';
		if(dWidth==1000) {
			$('#block-exp-sp-instructor-desk-tab-instructor-desk').addClass('enroll-disable-right-region');
			$('#block-exp-sp-instructor-desk-tab-instructor-desk').removeClass('enroll-enable-right-region');
		}
		else {
			$('#block-exp-sp-instructor-desk-tab-instructor-desk').addClass('enroll-disable-right-region');
			$('#block-exp-sp-instructor-desk-tab-instructor-desk').remove('enroll-enable-right-region');
		}
		this.renderInstructorResults();
		this.widgetObj = '$("#instructor-tab-inner").data("instructordesk")';
		this.prevMoreLPObj;
	 }catch(e){
		 // to do
	 }
	},

  hidePageControls : function(hideAll) {
	try{
    var lastDataRow = $('#paintInstructorResults tr.ui-widget-content').filter(":last");
    //console.log(lastDataRow.length);

    if (hideAll) {
     if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-instructor-desk-tab-instructor-desk .block-footer-left').show();
      $('#pager-ins').hide();
      $('#gview_paintInstructorResults').css('padding', '0');
    //  $("#page-container #paintInstructorResults").find("tr.jqgrow td").css('padding-bottom', '15px');

      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0');
      }
    }
    else {
      //console.log('hidePageControls() : hide only next/prev page control');
      $('#pager-ins').show();
      if(this.currTheme == "expertusoneV2")
      $('#block-exp-sp-instructor-desk-tab-instructor-desk .block-footer-left').hide();
      $('#pager-ins #pager-ins_center #first_pager-ins').hide();
      $('#pager-ins #pager-ins_center #prev_pager-ins').hide();
      $('#pager-ins #pager-ins_center #next_pager-ins').hide();
      $('#pager-ins #pager-ins_center #last_pager-ins').hide();
      $('#pager-ins #pager-ins_center #sp_1_pager-ins').parent().hide();
    }
	}catch(e){
		 // to do
	 }
  },

  showPageControls : function() {
	 try{
    //console.log('showPageControls() : show all control');
    $('#pager-ins').show();
    if(this.currTheme == "expertusoneV2")
    $('#block-exp-sp-instructor-desk-tab-instructor-desk .block-footer-left').hide();
    $('#pager-ins #pager-ins_center #first_pager-ins').show();
    $('#pager-ins #pager-ins_center #prev_pager-ins').show();
    $('#pager-ins #pager-ins_center #next_pager-ins').show();
    $('#pager-ins #pager-ins_center #last_pager-ins').show();
    $('#pager-ins #pager-ins_center #sp_1_pager-ins').parent().show();
	 }catch(e){
		 // to do
	 }
  },

	renderInstructorResults : function(){
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
		this.createLoader('instructor-result-container');
		var searchEnrollStr = '';
		var getRegStatus = this.instructortab;
		var obj = $("#instructor-tab-inner").data("instructordesk");
		var objStr = '$("#instructor-tab-inner").data("instructordesk")';
		var userId = this.getLearnerId();
		if(userId == "0" || userId == "")
		{
		   self.location='?q=learning/enrollment-search';
	       return;
		}
		var clsStr = Drupal.settings.myclassesSearchStr;
		if (typeof clsStr === 'undefined' || clsStr == null || clsStr == undefined){
			searchEnrollStr	= '&UserID='+userId+'&regstatuschk='+getRegStatus;
		}
		else {
			searchEnrollStr	= '&UserID='+userId+clsStr
		}
		var url = this.constructUrl("learning/instructor-classes/all/"+searchEnrollStr);
		if(Drupal.settings.mylearning_right===false){
			$("#paintInstructorResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sorttype':'text',formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintInstructorTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintInstructorActions}
				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager-ins',
				viewrecords: true,
				emptyrecords: "No Search Results Found",
				sortorder: "asc",
				toppager: false,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackInstructorLoader

			}).navGrid('#pager-ins',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}else{
			$("#paintInstructorResults").jqGrid({
				url: url,
				datatype: "json",
				mtype: 'GET',
				colNames:['','',''],
				colModel : [
				            {name:'Image',index:'learning_type_image',title: false, 'align':'center','widgetObj':objStr,'sorttype':'text',formatter:$("body").data("learningcore").paintLearningImage,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)},
				            {name:'Name',index:'cls_title',classes:'enroll-datatable-column1',title: false, 'align':'left','widgetObj':objStr,'sorttype':'text',formatter:obj.paintInstructorTitle},
				     		{name:'Action', index:'Action',classes:'enroll-datatable-column2',title: false, 'align':'right','sortable':false,'widgetObj':objStr,formatter:obj.paintInstructorActions}
				     	   ],
				rowNum: rowNumValue,
				rowList:rowListValue,
				pager: '#pager-ins',
				viewrecords: true,
				emptyrecords: "No Search Results Found",
				sortorder: "asc",
				toppager: false,
				height: 'auto',
				width: dWidth,
				loadtext: "",
				recordtext: "",
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				loadComplete:obj.callbackInstructorLoader

			}).navGrid('#pager-ins',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
//		if(Drupal.settings.mylearning_right!==false){
//			if($.browser.msie && parseInt($.browser.version, 10)=='8'){
//			$('#gview_paintInstructorResults .ui-jqgrid-btable').css({"table-layout":"auto","width":"auto"});
//			$('#gview_paintInstructorResults .ui-jqgrid-bdiv,#gview_paintInstructorResults').css({'width':'auto'});
//			$('#gview_paintInstructorResults .ui-jqgrid-bdiv,#paintContentResults .ui-widget-content, .ui-widget-content,#gbox_paintInstructorResults').css({'overflow':'hidden'});
//			$('#gbox_paintInstructorResults').css({'margin-right':'10px'});
//			$('#paintInstructorResults').css({'width':'663px'});
//			}
//		 }
		//$('#pager-ins').find('#pg_pager-ins .ui-pg-table').css("table-layout","auto");
		$('#pager-ins').find('#pg_pager-ins .ui-pg-table #pager-ins_center').removeAttr("style");
		obj.hidePageControls(true); // show in loadComplete callbackLoader()
		$('.ui-jqgrid').addClass('myenrollment-table');
		$('.jqgfirstrow td:first-child').addClass('enroll-datatable-column-img');
		$('.jqgfirstrow td:nth-child(2)').addClass('enroll-datatable-column1');
		$('.jqgfirstrow td:last-child').addClass('enroll-datatable-column2');
		$('.jqgfirstrow').hide();
		$('.ui-jqgrid-bdiv > div').css('position','static');
		$('.launch-content-status').each(function(){
		    if(parseInt($(this).css('width')) < 72){
		        $(this).css({'width':'72px','display':'inline-block','text-align':'center'});
		    }
		});
		/* to highlight the default sort order - added by Rajkumar U*/
		$('#instructor-result-container .sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#instructor-result-container .type1').addClass('sortype-high-lighter');
		}catch(e){
			 // to do
		 }
	},

	instructorSortForm : function(sort,className) {
		try{
		// High light sortype
		$('#instructor-result-container .sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#instructor-result-container .'+className).addClass('sortype-high-lighter');

		//var getEnrData 		= eval($("#sort-by-enroll").attr("data"));
		var getRegStatus 	= this.instructortab;
		var userId 			= this.getLearnerId();
    	var searchStr 		= '';
    	if(userId == "0" || userId == "")
		{
		   self.location='?q=learning/enrollment-search';
	       return;
		}
    	searchStr			= '&UserID='+userId+'&regstatuschk='+getRegStatus+'&sortBy='+sort;
    	var url 			= this.constructUrl("learning/instructor-classes/all/"+searchStr);
		$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
    	$('#paintInstructorResults').setGridParam({url: url});
        $("#paintInstructorResults").trigger("reloadGrid",[{page:1}]);
		}catch(e){
			 // to do
		 }
	},

	callbackInstructorLoader : function(data){
		try{
		
		var default_cookie = '&regstatuschk=scheduled&del_type=&tra_type=&price=&scheduled=&reg=&due=&assignedby=&location=&selectedLocID=&searchText=&sortBy=AZ';		
		//Append tr to all the row
		resetFadeOutByClass('#paintInstructorResults','content-detail-code','line-item-container','mylearning');
		if(data['rows'] != undefined && data['rows'] != null) {
				$.each(data['rows'], function(index, row) {
					// console.log(row);
					var html = '';
				    var id = row["cell"].id;
				    var dataDelTypeCode = 'lrn_cls_dty_ilt';
				  	html += '<div class="cp_seemore" id="seemore_'+id+'" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').viewClassDetails('+id+',this,\''+dataDelTypeCode+'\');">'+ Drupal.t('LBL713') +'</div>';
					html += '<div class="paindContentResults clsSeeMorePlaceholderdiv" id="paindContentResults_'+id+'"></div>';	
					$('#selclsdetailsrow-'+id).after("<div id='seemore-detail-"+id+"' class='seemore-detail'>"+html+"</div>");
				});
			}
		// $("#paintInstructorResults").find("tr:last").find("td").css('border-bottom','0px solid #EDEDED').css('padding-bottom','4px');
		$("#paintInstructorResults").find("tr:last").addClass('ui-widget-content-last-row').find("td").css('padding-bottom','4px');
		$("#instructor-tab-inner").data("instructordesk").destroyLoader('instructor-result-container');
		if(Drupal.settings.mylearning_right===false)
			$('.clsinstructor-result-container #pager-ins').width($('.clsinstructor-result-container').width()+4);
		var obj = $("#instructor-tab-inner").data("instructordesk");

    // var recs = parseInt($("#paintInstructorResults").getGridParam("records"),10);
    //console.log('callbackInstructorLoader() : recs = ' + recs);
		var recs = data['records'];
		$("#paintInstructorResults").data('totalrecords', recs);
		var showMore = $('#paintInstructorResults-show_more');
		if($('#paintInstructorResults').getGridParam("reccount") < recs) {
			showMore.show();
		} else {
			showMore.hide();
		}
		if (recs <= 0) {
			if(Drupal.settings.myclassesSearchStr === default_cookie ){
				$('#instructor-noresult-msg').show();
			}
			else{
				$('#instructor-nosearchresult-msg').show();
			}
			$('#instructor-result-container .sort-by-text').hide();
			$('#instructor-result-container .sort-by-links').hide();
		} else {
			$('#instructor-result-container .sort-by-text').show();
			$('#instructor-result-container .sort-by-links').show();
		}
    
    resetFadeOutByClass('#paintInstructorResults','content-detail-code','line-item-container','mylearning');
    
    // Show pagination only when search results span multiple pages
    var reccount = parseInt($("#paintInstructorResults").getGridParam("reccount"), 10);
    //console.log('callbackInstructorLoader() : reccount = ' + reccount);

    var hideAllPageControls = true;
    if (recs > 5) { // 10 is the least view per page option.
      hideAllPageControls = false;
      //console.log('callbackInstructorLoader() : hideAllPageControls set to false');
    }

    if (recs <= reccount) { // Covers the case when there are no recs, i.e. recs == 0
      //console.log('callbackInstructorLoader() : recs <= reccount : hide pagination controls');
      obj.hidePageControls(hideAllPageControls);
    }
    else {
      //console.log('callbackInstructorLoader() : recs > reccount : show pagination controls');
      obj.showPageControls();
    }
    $("#instructor-tab-inner").data("instructordesk").shapeTheButtonInstructor('paintInstructorResults');
		//Vtip-Display toolt tip in mouse over
		vtip();
		$('.limit-title-ins').trunk8(trunk8.ins_title);
		$('.limit-desc-ins').trunk8(trunk8.ins_desc);
		$("#paintInstructorResults").showmore({
			'grid': "#paintInstructorResults",
			'gridWrapper': '#instructor-result-container',
			'showMore': '#paintInstructorResults-show_more'
		});
		}catch(e){
			 // to do
		 }
	},

	shapeTheButtonInstructor : function(parentId){
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
		});
		}catch(e){
			 // to do
		 }
	},
	callInstructor : function(regStatus){
		try{
		this.instructortab = regStatus;
		$('#instructor-noresult-msg').hide();
		$('#instructor-nosearchresult-msg').hide();
		var userId = this.getLearnerId();
		if(userId == "0" || userId == "")
		{
		   self.location='?q=learning/enrollment-search';
	       return;
		}
    	//var getRegStatus = (regStatus) ? regStatus : 'scheduled';
    	var searchStr	= '&UserID='+userId+'&regstatuschk='+regStatus;
    	var url = this.constructUrl('learning/instructor-classes/all/'+searchStr);
    //	this.setSortTypeDataInstructor(getRegStatus);
		$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
    	$('#paintInstructorResults').setGridParam({url: url});
        $("#paintInstructorResults").trigger("reloadGrid",[{page:1}]);
       	this.highlightCourseTabInstructor(regStatus);
    	$('#instructor-result-container .sort-by-links').find('li a').removeClass('sortype-high-lighter');
       	$('#instructor-result-container .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
    	if(this.currTheme == "expertusoneV2"){
		    $('#learner-instructor .sort-by-links').find('li a').removeClass('sortype-high-lighter');
		    $('#learner-instructor .sort-by-links').find('li:first-child a').addClass('sortype-high-lighter');
        }
		}catch(e){
			 // to do
		 }
    },

    setSortTypeDataInstructor : function(getRegStatus) {
    	try{
    	var setdata = "data={'currEnrMode':'"+getRegStatus+"'}";
    	$("#sort-by-enroll").attr("data",setdata);
    	}catch(e){
   		 // to do
   	 	}
    },

    highlightCourseTabInstructor : function(highlightId){
    	try{
		var curTheme = themepath.split("/");
		var resTheme = curTheme[curTheme.length-1];
		if (resTheme == 'expertusone')
		{
			$('#instructor-maincontent_tab ul li a#'+highlightId+'_tab').parent('li').siblings('li').removeClass('selected');
			$('#instructor-maincontent_tab ul li a#'+highlightId+'_tab').parent('li').addClass('selected');
		} else
		{
			$("#instructor-maincontent_tab ul li a").each(function(){
			    $(this).removeClass('orange');
			});
			$('#instructor-maincontent_tab ul li a#'+highlightId+'_tab').addClass('orange');
		}
    	}catch(e){
   		 // to do
   	 	}
	},

	paintInstructorTitle : function(cellvalue, options, rowObject) {
		try{
		var htmlVal='';
		var wobj = eval(options.colModel.widgetObj);
		var obj1 = options.colModel.widgetObj;
		var dataId					= rowObject['id'];
		var courseTitle				= rowObject['course_title'];
		var classTitle				= rowObject['cls_title'];
		var classCode				= rowObject['cls_code'];
		var shortDescription 		= rowObject['description'];
		var descriptionFull 		= rowObject['descriptionfull'];
		var dataDelType				= rowObject['delivery_type'];
		var dataDelTypeCode			= rowObject['delivery_type_code'];
		var baseType				= rowObject['basetype'];
		var courseId 				= rowObject['course_id'];
		var session_id				= rowObject['session_id'];
		var SessStartDate 			= rowObject['session_start'];
		var sessionStartDay			= rowObject['session_start_day'];
		var SessEndDate 			= rowObject['session_end'];
		var SessMaxCapacity			= rowObject['session_max_capacity'];
		var EnrolledCount 			= rowObject['enrolled_count'];
		var language 				= rowObject['language'];
		var LocationName 			= rowObject['location_name'];
		var LocationId				= rowObject['location_id'];
		var LocationAddr1 			= rowObject['location_addr1'];
		var LocationAddr2 			= rowObject['location_addr2'];
		var LocationCity 			= rowObject['location_city'];
		var LocationState 			= rowObject['location_state'];
		var LocationCountry 		= rowObject['location_country'];
		var LocationZip 			= rowObject['location_zip'];
		var LocationPhone 			= rowObject['location_phone'];
		var startDateFormat			= rowObject['session_start_format'];
		var lnrAttach				= rowObject['show_lnr_attach'];
		var show_events				= rowObject['show_events'];
		var labelmsg 				= rowObject['labelmsg'];
		this.localeArray = labelmsg;
		var daysRemaining = '';
		var dayRemainVal  = '';
		var AttemptLeft	  = '';
		var contValidateMsg = '';
		var mro						= rowObject['mro'];
		var mro_id					= rowObject['mro_id'];
		var assigned_by				= rowObject['assigned_by'];
		//dummy variable for po translation
		var DdelType1				= Drupal.t('Classroom');
		var DdelType2				= Drupal.t('Virtual Class');
		var FullName				= rowObject['full_name'];
		var MandatoryOption			= rowObject['mandatory'];
		//var loggedUserid           =  rowObject['user_id'];
		var Managerid             =  rowObject['managerid'];
		var UpdatedBy             =  rowObject['updated_by'];
		var UpdatedByName         =  rowObject['updated_by_name'];
		var isLastRec             =  (typeof rowObject['is_last_rec'] != 'undefined' && rowObject['is_last_rec'] == 'last')? 'last' : '';

		var mroImageClass = '';
		var mroImageClassArr = new Array();
        mroImageClassArr['cre_sys_inv_man'] =  '<span class="course-mandatory-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Mandatory')+'">M</span>';
        mroImageClassArr['cre_sys_inv_opt'] =  '<span class="course-optional-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Optional')+'">O</span>';
        mroImageClassArr['cre_sys_inv_rec'] =  '<span class="course-recommended-bg vtip" id="'+classId+mro_id+'" title="'+Drupal.t('Recommended')+'">R</span>';
        mroImageClass = (mro == '' || mro == null) ? '' : mroImageClassArr[mro_id];

        if(session_id == null) {
			session_id ='';
		}

		var obj = options.colModel.widgetObj;

		var data1 = "data={'is_last_rec':'"+isLastRec+"'}";
		var html	= '';
		var inc = 0;
		var passParams ='';
		if(lnrAttach.length>0) {
			 $(lnrAttach).each(function(){
				 inc=inc+1;
				 passParams += "{";
				 passParams += "'Id':'"+$(this).attr("attachment_id")+"'";
				 passParams += ",'Title':'"+$(this).attr("reading_title")+"'";
				 passParams += ",'url':'"+$(this).attr("reading_content")+"'";
				 passParams += "}";
				 if(inc<lnrAttach.length) {
					 passParams += ",";
				 }
			 });
		}
		var attachdata = "data=["+passParams+"]";

		var data2="data={'classCode':'"+escape(classCode).replace(/"/g, '\&quot;')+"','BaseType':'"+baseType+"','isTitle':'1','title':'"+escape(classTitle)+"','rowId':'"+dataId+"','CourseId':'"+courseId+"','classId':'"+classId+"','deliverytype':'"+dataDelTypeCode+"'," +
		"'dataDelType':'"+dataDelType+"','LocationName':'"+escape(LocationName)+"','SessStartDay':'"+sessionStartDay+"','startDateFormat':'"+startDateFormat+"','SessStartDate':'"+SessStartDate+"','SessEndDate':'"+SessEndDate+"','detailTab':'false'," +"'LocationId':'"+LocationId+"','LocationAddr1':'"+escape(LocationAddr1)+"','LocationAddr2':'"+escape(LocationAddr2)+"','LocationCity':'"+escape(LocationCity)+"','LocationState':'"+escape(LocationState)+"','LocationCountry':'"+escape(LocationCountry)+"','LocationZip':'"+LocationZip+"','LocationPhone':'"+LocationPhone+"'," +
		"'FullName':'"+escape(FullName)+"','MandatoryOption':'"+MandatoryOption+"','Managerid':'"+Managerid+"','UpdatedBy':'"+UpdatedBy+"','UpdatedByName':'"+escape(UpdatedByName)+"','mro_type':'"+mro+"','assigned_by':'"+assigned_by+"',"+
		"'is_last_rec':'"+isLastRec+"'}";
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var titlelen = 0;
	 	if(this.currTheme == "expertusoneV2"){
	 		if(Drupal.settings.mylearning_right===false)
	 			titlelen = 45 ;
	 		else
	 			titlelen = 15 ;
		}else{
			titlelen = 25;
	    }

		//Enrollment Title Restricted to 20 Character
		var enrollFulltitle = unescape(classTitle);
		var baseType  = rowObject['basetype'];
		var titleclass = 'exp-sp-instructor-desk-enroll-fullsinglesession';

		html += '<div id="selclsdetailsrow-'+dataId+'"><div class="enroll-course-title">';
		html += '<div class="limit-title-row">';
		html += '<span id="class_attachment_'+dataId+'" data="'+attachdata+'"></span>';
		var style = '';
		if(wobj.currTheme == "expertusoneV2"){ style = "style=\"display:none;\"";
		html += '<a id="ins-class-accodion-'+dataId+'" href="javascript:void(0);" data = "'+data2+'" class="title_close" '+style+' onclick=\'$(\"#instructor-tab-inner\").data(\"instructordesk\").viewClassDetails('+dataId+', this,'+dataDelTypeCode+');\'>&nbsp;</a>';
		}else{
			html += '<a id="ins-class-accodion-'+dataId+'" href="javascript:void(0);" data = "'+data2+'" class="title_close" '+style+' onclick=\'$(\"#instructor-tab-inner\").data(\"instructordesk\").viewClassDetails('+dataId+', this);\'>&nbsp;</a>';
		}

		if(((baseType =="ILT") || (baseType =="VC"))) {
			var sessLength = rowObject.sessionDetails.length;
			if(sessLength>1) {
				titleclass = 'exp-sp-instructor-desk-enroll-fullmultiplesession';
			}
		}
		//	html +=  '<div class="limit-title-row">';
		html += '<span id="titleAccEn_'+dataId+'" class="item-title" ><span class="limit-title limit-title-ins enroll-class-title vtip" title="'+unescape(classTitle).replace(/"/g, '\&quot;')+'" href="javascript:void(0);">';
		html += enrollFulltitle;
		html += '</span></span>';
		html += '</div>';
		//html += '<span id="titleAccEn_'+dataId+'" data="'+data1+'"><a class="spotlight-item-title" href="'+resource.base_url+'?q=learning/class-details/'+classId+'/'+session_id+'" ><b>'+unescape(classTitle)+'</b></a></span>';
		html += '</div>';


		var startTime = rowObject['session_start'];
		var startDate = rowObject['session_start_format'];
		var classId	  = rowObject['id'];

		if(((baseType =="ILT") || (baseType =="VC")) && (startTime != null)) {
			var user_tz = rowObject.sessionDetails[0].user_tz;
			var user_tzcode = rowObject.sessionDetails[0].user_tzcode;
			var loc_tz = rowObject.sessionDetails[0].session_code;
			var loc_tzcode = rowObject.sessionDetails[0].tz_code;
			//console.log(user_tz);
			//console.log(user_tzcode);
			
			
			var sessLen = rowObject.sessionDetails.length;
			var sessLenEnd;
			if(sessLen>1) {
				sessLenEnd = sessLen-1;
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				
				var sTime =rowObject.sessionDetails[0].session_start_format;
				var sTimeForm =rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[sessLenEnd].session_end_format;
				var eTimeForm = rowObject.sessionDetails[sessLenEnd].session_end_time_form;
				var usTime = rowObject.sessionDetails[0].ilt_session_start_format;
				var usTimeForm  = rowObject.sessionDetails[0].ilt_session_start_time_form ;
				var ueTime = rowObject.sessionDetails[sessLenEnd].ilt_session_end_format;
				var ueTimeForm =rowObject.sessionDetails[sessLenEnd].ilt_session_end_time_form;
				startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';

				if(baseType =="ILT" && user_tz!=loc_tz ){
					startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+user_tz+' '+user_tzcode+'';

				}
				iltstartDate = usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+' to '+ueTime+' <span class="time-zone-text">'+ueTimeForm+'</span>';
			} else {
				/*For ticket 0028936: ILT Timezone Display for My Learning page */
				var sTime = rowObject.sessionDetails[0].session_start_format;
				var sTimeForm = rowObject.sessionDetails[0].session_start_time_form;
				var eTime = rowObject.sessionDetails[0].session_start_end_format;
				var eTimeForm = rowObject.sessionDetails[0].session_end_time_form;
				var usTime = rowObject.sessionDetails[0].ilt_session_start_format;
				var usTimeForm = rowObject.sessionDetails[0].ilt_session_start_time_form;
				var ueTime = rowObject.sessionDetails[0].ilt_session_start_end_format;
				var ueTimeForm = rowObject.sessionDetails[0].ilt_session_end_time_form;
				
				if(baseType =="ILT" && user_tz!=loc_tz ){
					startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>'+' '+user_tz+' '+user_tzcode+'';
				}
				else{
					startDate = sTime+' '+'<span class="time-zone-text">'+sTimeForm+'</span>'+' to '+eTime+' <span class="time-zone-text">'+eTimeForm+'</span>';
				}
				iltstartDate = usTime+' '+'<span class="time-zone-text">'+usTimeForm+'</span>'+' to '+ueTime+' <span class="time-zone-text">'+ueTimeForm+'</span>'+' '+loc_tz+' '+loc_tzcode+'';
			}
			var inc = 0;
			var passParams = "[";

			$(rowObject.sessionDetails).each(function(){
				inc=inc+1;
				 var sessionfacName = (($(this).attr("session_name") != null) ? ($(this).attr("session_name").replace(/'/g, "\\'")) : $(this).attr("session_name"));
				 passParams += "{";
				 passParams += "'sessionTitle':'"+(($(this).attr("session_title")) ? escape($(this).attr("session_title")) : '')+"',";
				 /*For ticket 0028936: ILT Timezone Display for My Learning page */

				 passParams +=  "'sessionDay':'"+$(this).attr("session_start_day")+"',";
				 passParams +=  "'sessionDate':'"+$(this).attr("session_start_format")+' to '+$(this).attr("session_start_end_format")+"',";
				 passParams += "'sessionSDate':'"+$(this).attr("session_start_format")+"',";
				 passParams += "'sessionEDate':'"+$(this).attr("session_start_end_format")+"',";
				 passParams += "'sessionSDayForm':'"+$(this).attr("session_start_time_form")+"',";
				 passParams += "'sessionEDayForm':'"+$(this).attr("session_end_time_form")+"',";
				 passParams += "'iltsessionDay':'"+$(this).attr("ilt_session_start_day")+"',";
				 passParams += "'iltsessionDate':'"+$(this).attr("ilt_session_start_format")+' to '+$(this).attr("ilt_session_start_end_format")+"',";
				 passParams += "'iltsessionSDate':'"+$(this).attr("ilt_session_start_format")+"',";
				 passParams += "'iltsessionEDate':'"+$(this).attr("ilt_session_start_end_format")+"',";
				 passParams += "'iltsessionSDayForm':'"+$(this).attr("ilt_session_start_time_form")+"',";
				 passParams += "'iltsessionEDayForm':'"+$(this).attr("ilt_session_end_time_form")+"',";
				 passParams += "'session_id':'"+$(this).attr("session_id")+"',";
				 passParams += "'user_tz':'"+user_tz+"',";
				 passParams += "'user_tzcode':'"+user_tzcode+"',";
				 passParams += "'loc_tz':'"+loc_tz+"',";
				 passParams += "'loc_tzcode':'"+loc_tzcode+"',";
				 passParams += "'sessionfacName':'"+sessionfacName+"',";
				 passParams += "'sessionfacAddress1':'"+escape($(this).attr("session_address1"))+"',";
				 passParams += "'sessionfacAddress2':'"+escape($(this).attr("session_address2"))+"',";
				 passParams += "'sessionfacCountry':'"+escape($(this).attr("session_country"))+"',";
				 passParams += "'sessionfacState':'"+escape($(this).attr("session_state"))+"',";
				 passParams += "'sessionfacCity':'"+escape($(this).attr("session_city"))+"',";
				 passParams += "'sessionfacZipcode':'"+escape($(this).attr("session_zipcode"))+"',";
				 passParams += "'sessionInstructorName':'"+(($(this).attr("instructorName")) ? escape($(this).attr("instructorName")) : '')+"'";

				 passParams += "}";

				 if(inc < sessLen) {
					 passParams += ",";
				 }
			});
			passParams += "]";
		/*	if(baseType =="ILT")
			html += '<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+' '+user_tz+' '+user_tzcode+'">'+titleRestrictionFadeoutImage(startDate,'exp-sp-instructordesk-timezone', 30)+'';
			else
            html += '<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+'">'+titleRestrictionFadeoutImage(startDate,'exp-sp-instructordesk-timezone', 30)+'';
			if(baseType =="ILT" && user_tz!=loc_tz ){
				html += qtip_popup_paint(dataId,iltstartDate); 
			}
		html += '</span>';*/

			var startSessDate = rowObject.sessionDetails[0].session_start;
			html += '<div id="startSessDate'+classId+'" style="display:none;">'+startSessDate+'</div>';

			if(sessLen > 1){
				for(i=0;i<sessLen;i++){
					if(i == (sessLen - 1)){
						var endSessDate = rowObject.sessionDetails[i].session_start;
					}
				}
			}else{
				endSessDate = rowObject.sessionDetails[0].session_start;
			}
			endSessDate = endSessDate.split(" ");
			endSessDate = endSessDate[0].split("-");
			endSessDate = endSessDate[1]+'-'+endSessDate[2]+'-'+endSessDate[0];
			html += '<div id="endSessDate'+classId+'" style="display:none;">'+endSessDate+'</div>';

		}
		else{
			html += '<span>&nbsp;</span>';
		}

		html += '<div class="enroll-class-title-info">';
		//html += '<div class="item-title-code">'+SMARTPORTAL.t('Code')+':<span class="vtip" title="'+classCode+'"> '+wobj.titleRestricted(classCode,15)+"</span>";
		html += '<div class="content-detail-code">';
		
		

		var statusDate = '';
		var statusName = '';


		//Pipe Sysmbol use this variable 'pipe'
		var pipe = '<span class="enroll-pipeline">|</span>';
//		if(classCode) {
//			if(Drupal.settings.mylearning_right===false)
//				//html += '<span class="vtip" title="'+unescape(classCode).replace(/"/g, '\&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-instructor-desk-class-code',40)+"</span>";
//			else
//				html += '<span class="vtip" title="'+unescape(classCode).replace(/"/g, '\&quot;')+'">'+titleRestrictionFadeoutImage(classCode,'exp-sp-instructor-desk-class-code',10)+"</span>";
//		}
		/*if(baseType) {
			var row_det_type;

			if (baseType == '' || baseType == '-') {
				return '<span>&nbsp;</span>';
			}
			else if(baseType == 'WBT'){
				row_det_type = Drupal.t("Web-based");
	        }
	        else if(baseType == 'VC'){
	        	row_det_type = Drupal.t("Virtual Class");
	        }
	        else if(baseType == 'ILT'){
	            row_det_type = Drupal.t("Classroom");
	        }
	        else if(baseType == 'VOD'){
	            row_det_type = Drupal.t("Video");
	        }


			html += row_det_type;
		}*/
		html += '<div class="item-title-code">';
		var ClassLoc = (LocationName && (LocationName != '') ? (LocationName) : '');
		if(ClassLoc) {
			html += '<div class="line-item-container float-left">';
			
			if(Drupal.settings.mylearning_right===false){
				html += /*pipe+*/'<span class="vtip" title="'+htmlEntities(ClassLoc)+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-instructor-desk-class-location',40)+'</span>';
			//html += pipe+'<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+'">'+startDate+'</span>';
			}else
				html += /*pipe+*/'<span class="vtip" title="'+htmlEntities(ClassLoc)+'">'+titleRestrictionFadeoutImage(ClassLoc,'exp-sp-instructor-desk-class-location',10)+'</span>';
			html += ' </div>';
			
		}
		/*if(ClassLoc){
		html += pipe+'<span class="vtip" title="'+this.localeArray['seats']+'">'+this.localeArray['seats']+' : <span id="seats_enrolled_'+classId+'">'+EnrolledCount+'</span>'+' '+Drupal.t('LBL419')+' '+SessMaxCapacity+' '+(Drupal.t('Enrolled')).toLowerCase()+'</span>';
		}else{
			html += '<span class="vtip" title="'+this.localeArray['seats']+'">'+this.localeArray['seats']+' : <span id="seats_enrolled_'+classId+'">'+EnrolledCount+'</span>'+' '+Drupal.t('LBL419')+' '+SessMaxCapacity+' '+(Drupal.t('Enrolled')).toLowerCase()+'</span>';
		}*/
		//html += pipe+'<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+'">'+startDate+'</span>';
		html += '<div class="line-item-container float-left">';
		
		if(baseType =="ILT")
			html += pipe+'<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+' '+user_tz+' '+user_tzcode+'">'+titleRestrictionFadeoutImage(startDate,'exp-sp-instructordesk-timezone')+'';
			else
            html += '<span class="session-time-zone vtip" data=\"data='+passParams+'\" id="session_det_popup_ins'+classId+'" title="'+sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm+'">'+titleRestrictionFadeoutImage(startDate,'exp-sp-instructordesk-timezone')+'';
		html += ' </div>';
		
		html += '</span>';
		if(baseType =="ILT" && user_tz!=loc_tz ){
			html += qtip_popup_paint(dataId,iltstartDate); 
		}
		html += ' </div>';
		html += ' </div>';
		
		var displayDesc = descriptionFull;
		var crsMoreType= dataDelTypeCode;
		html += '<div class="limit-desc-row ' +crsMoreType+'">';
		html += '<div><span class ="limit-desc limit-desc-ins vtip" id="insclassShortDesc_'+dataId+'"><span class="cls-learner-descriptions">'+displayDesc+'</span></span></div>';
		//html += '<div class="cp_seemore" id="seemore_'+dataId+'" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').viewClassDetails('+dataId+',this,\''+dataDelTypeCode+'\');">'+ Drupal.t('LBL713') +'</div>';
		//html += '<div class="paindContentResults clsSeeMorePlaceholderdiv" id="paindContentResults_'+dataId+'"></div>';
		//console.log(dataId);
		/*if(1==1){
			html += '<div class="cp_seemore" id="seemore_'+dataId+'" onclick="$(\'#learner-enrollment-tab-inner\').data(\'contentPlayer\').playContentMylearning(' + launch_data + ');">'+ Drupal.t('LBL713') +'</div>';
			html += '<div class="paindContentResults clsSeeMorePlaceholderdiv" id="paindContentResults_'+dataId+'"></div>';
		}*/
		html += '</div>';
		html += '</div></div>';

		return html;
		}catch(e){
	   		 // to do
	   	 }
	},

	viewClassDetails : function(classId,rowObj,dataDelTypeCode){
		try{
		if($(rowObj).parents("tr").find("a").attr('class')=='title_close'){
			$("#tab_instructor_desk_customized #"+classId+" #insclassShortDesc_"+classId).find("#arrow-more").click();
			$('#selEnrollAction-'+classId).click();
		    $('#ins-class-accodion-'+classId).removeClass('title_close');
		    $('#ins-class-accodion-'+classId).addClass('title_open');
		} else {
			$("#tab_instructor_desk_customized #"+classId+" #insclassShortDesc_"+classId).find("#arrow-less").click();
		    $('#ins-class-accodion-'+classId).addClass('title_close');
		    $('#ins-class-accodion-'+classId).removeClass('title_open');
		    $('#cus-accord-'+classId).remove();
		}
		//add space for data grid
		/*if(this.currTheme == 'expertusoneV2'){

		    if($("#classes-list-inst-"+classId).is(":hidden")){
		    	console.log("grid :hidden");
		      $("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0 0 7px 0');
		    }

		    if($("#classes-list-inst-"+classId).is(":visible")){
		    	console.log("grid :visible");
		       $("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0 0 7px 0');
		    }else{
		    	
		    	 if((dataDelTypeCode == 'lrn_cls_dty_vcl') && this.currTheme == 'expertusoneV2'){
		    		 	console.log("dataDelTypeCode grid :lrn_cls_dty_vcl");
		    	    $("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0');
	            }else {
	            	$("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0');
	            	}
		    }
		}*/
		var data = eval($("#ins-class-accodion-"+classId).attr("data"));
		if (document.getElementById('classes-list-inst-'+classId)) {
		  if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
			if(!document.getElementById('cus-accord-'+classId)){
				//$(rowObj).parents("tr").find("td").css("border-bottom",this.borderColor);
			}else{
				//$(rowObj).parents("tr").next().find('.instructor-learner-tab').css("border-bottom",this.borderColor);
			}
		   if(this.currTheme == 'expertusoneV2'){
			  if(!document.getElementById('cus-accord-'+classId)){
				 //  $(rowObj).parents("tr").find("td").css("border-bottom","1px solid #EDEDED");
			   }

		   }
		  }
			$('#classes-list-inst-'+classId).remove();
			$("#seemore_"+classId).html(Drupal.t('LBL713'));
		} else {
		
			$("#seemore-detail-"+classId).before("<div id = 'classes-list-inst-"+classId+"' class='inst-det-accord'></div>");
			//$(rowObj).parents("tr#seemore-detail-"+classId).find("td").css("border-bottom","none");
			var classDets  = this.paintInstructorClassSection(data,classId);
			$("#classes-list-inst-"+classId).html(classDets);
			 vtip();
			if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
			if(this.currTheme == "expertusoneV2"){
				 // $(rowObj).parents("tr").css("border-bottom","none");
			}else{
				if(!document.getElementById('cus-accord-'+classId)){
					//$(rowObj).parents("tr").next().find("td").css("border-bottom","solid 1px #cccccc");
				}
			}
			}
			if((data.is_last_rec != 'last') && this.currTheme == 'expertusoneV2'){
				if(!document.getElementById('cus-accord-'+classId)){
					//$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
					//$("#ins-class-accodion-"+classId).parents("tr").next().find("td").css('border-bottom','1px solid #EDEDED');
					//$("#instructor-result-container.clsinstructor-result-container .sessAddDet.location-row").css('padding-bottom','10px');
				}
			}

			if((data.is_last_rec != 'last') && (dataDelTypeCode == 'lrn_cls_dty_vcl') && this.currTheme == 'expertusoneV2'){
				if(!document.getElementById('cus-accord-'+classId)){
					//$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
					//$("#ins-class-accodion-"+classId).parents("tr").next().find("td").css('border-bottom','1px solid #EDEDED');
					//$(".page-learning-enrollment-search #instructor-result-container #paintInstructorResults .inst-det-accord .enroll-session-details.session-row").css('padding-bottom','7px');
				}
			}

			if( (data.is_last_rec == 'last') && (dataDelTypeCode == 'lrn_cls_dty_vcl') && (this.currTheme == 'expertusoneV2') ){
				//$("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0');
				if(!document.getElementById('cus-accord-'+classId)){
					//$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
					//$("#ins-class-accodion-"+classId).parents("tr").next().find("td").css('border-bottom','0px solid #EDEDED');
					//$("#instructor-result-container.clsinstructor-result-container .sessAddDet.location-row").css('padding-bottom','0');
					//$("#instructor-result-container.clsinstructor-result-container .instructor-learner-tab .ui-jqgrid").css('margin-top','12px');
				}
			}
			else if((data.is_last_rec == 'last') && (dataDelTypeCode == 'lrn_cls_dty_ilt') && this.currTheme == 'expertusoneV2'){
				//$("#cus-accord-"+classId).find('.instructor-learner-tab').find('#gbox_paintClassLearnersList'+classId).css('margin','0');
				if(!document.getElementById('cus-accord-'+classId)){
					//$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
					//$("#ins-class-accodion-"+classId).parents("tr").next().find("td").css('border-bottom','0px solid #EDEDED');
					//$("#instructor-result-container.clsinstructor-result-container .sessAddDet.location-row").css('padding-bottom','0');
					//$("#instructor-result-container.clsinstructor-result-container .instructor-learner-tab .ui-jqgrid").css('margin-top','12px');
				}
			}else if((data.is_last_rec != 'last') && (dataDelTypeCode == 'lrn_cls_dty_ilt') && this.currTheme == 'expertusoneV2'){
				
				if(!document.getElementById('cus-accord-'+classId)){
					//$("#instructor-result-container.clsinstructor-result-container .sessAddDet.location-row").css('padding-bottom','10px');
			}
			}

			/*if( (data.is_last_rec == 'last') && (dataDelTypeCode == 'lrn_cls_dty_vcl') && (this.currTheme == 'expertusoneV2') || (data.is_last_rec == 'last') && (dataDelTypeCode == 'lrn_cls_dty_ilt') && (this.currTheme == 'expertusoneV2') ){
				alert(dataDelTypeCode);
				if($('#classes-list-inst-'+classId).is(":hidden")){
					alert(dataDelTypeCode);
					$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
					$("#ins-class-accodion-"+classId).parents("tr").next("tr").find("td").css('border-bottom','0px solid #EDEDED').css('padding-bottom','18px');
				}
			}
			*/
			//change text for details
			$("#seemore_"+classId).html(Drupal.t('LBL3042'));
			resetFadeOutForAttributes('#classes-list-inst-'+classId,'main-item-container','line-item-container','container','myinstructor');
		}
		}catch(e){
	   		 // to do
	   	 }
		vtip();
	},

	paintInstructorClassSection : function(data,classId){
		//  console.log('In paintDetailsClassSection()');
		try{
			var ostr = '';
			var dataInfo = data;
			var shortDescription = unescape(data.shortDescription);
			var courseId = data.courseId;
			var enrollId = data.RegId;
			var clsTitle = data.title;
			var SessStartDate = data.SessStartDate;
			var SessStartDay = data.SessStartDay;
			var SessEndDate = data.SessEndDate;
			var startDateFormat = data.startDateFormat;
			var baseType = data.BaseType;
			var fullName			   = data.FullName;

			var noBorderLastRecClass = ($("#pager").is(":visible") || data.is_last_rec != 'last')? '' : 'noborder';
			var Obj = this;
			//ostr += '<td colspan = "3">';
			ostr += '<div id="inst-det-main-div" class="enroll-accordian-div ' + noBorderLastRecClass + '">';

			var dataDesc = "data={'ClassId':'"+classId+"','ClassDesc':'"+shortDescription+"'}";
			var data1 = "data={'classId':'"+classId+"','courseId':'"+courseId+"'}";

			if(document.getElementById("session_det_popup_ins"+classId)) {
				var sessionDet = eval($("#session_det_popup_ins"+classId).attr('data'));
				var inc = 1;
				var LocationName = '';
				LocationName 	= dataInfo.LocationName;
				var sesinc = 1;
				$(sessionDet).each(function(){ //sessionDate sessionDay
				if(sesinc==1)
						{
						sesinc++;
						if(LocationName!='null' && LocationName!=''){
						LocationName =   LocationName + ",";
						if(LocationName.length > 105)
							loctitle = titleRestrictionFadeoutImage(unescape(LocationName),'exp-sp-instructor-desk-loc-head',110);
						else
							loctitle = unescape(LocationName);
						ostr += '<div class="sessAddDet location-row main-item-container"> <div class="line-item-container"> <span class="location-label container">'+Drupal.t("Location")+' :</span>';
						ostr += '<div class="location-desc"><span class="name-add vtip" title="'+unescape(LocationName)+'">'+loctitle+'</span>';
						}
						//if($(this).attr("sessionfacName")!='null' && $(this).attr("sessionfacName")!='')
							//ostr += ',<br><span class="sessAddress">'+$(this).attr("sessionfacName")+'</span>';
						if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='undefined' && $(this).attr("sessionfacAddress1")!='')
							ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
						if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='undefined' && $(this).attr("sessionfacAddress2")!='')
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
						ostr += '</div> </div>';
						}
						}
						

				});
				//ostr += '<div class="enroll-session-details">'+SMARTPORTAL.t("Session Details")+':</div>';
				ostr += '<div class="enroll-session-details session-row main-item-container"><span class="session-label session-container">'+Drupal.t("LBL277")+' :</span><div class="session-desc">';
				//var inc = 1;
				//var LocationName = '';
				//LocationName 	= dataInfo.LocationName;
				$(sessionDet).each(function(){ //sessionDate sessionDay
					var sesionsH = ($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")) : "&nbsp;";
					if(Drupal.settings.mylearning_right===false)
						var sesionsHead =titleRestrictionFadeoutImage(sesionsH,'exp-sp-instructor-desk-session-head',50);
					else
						var sesionsHead = titleRestrictionFadeoutImage(sesionsH,'exp-sp-instructor-desk-session-head',18);
					ostr += '<div class="sessionDet">';
					ostr += '<div class="cls-sessionTitle-container line-item-container"><div class="lbl-sessionTitle sessionDetlbl container">'+Drupal.t("LBL107")+' :'+'</div> <div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? htmlEntities(unescape($(this).attr("sessionTitle"))) : "&nbsp;")+'">'+sesionsHead+'</div></div>';
					ostr += '<div class="cls-sessDate-container line-item-container"><div class="lbl-sessionDate sessionDetlbl container">'+Drupal.t("LBL042")+' :'+'</div> <div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
					//ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
					if(baseType == "ILT"){
						var locationsess = '<div class="sessDate">'+$(this).attr("iltsessionSDate")+" <span class='time-zone-text'>"+$(this).attr("iltsessionSDayForm")+"</span>"+' to '+$(this).attr("iltsessionEDate")+" <span class='time-zone-text'>"+$(this).attr("iltsessionEDayForm")+"</span>"+"  "+$(this).attr("loc_tz")+" "+$(this).attr("loc_tzcode")+"</div>";
						ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>"+"  "+$(this).attr("user_tz")+" "+$(this).attr("user_tzcode")+"";
 
					}
					else{
					ostr += '<div class="sessDate">'+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("sessionEDate")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span>";
					}
					if(baseType == "ILT" && $(this).attr("user_tz")!= $(this).attr("loc_tz")){
						ostr += qtip_popup_paint($(this).attr("session_id"),locationsess); 
					}
					ostr += '</div></div>';
					var sesionsInsH = ($(this).attr("sessionInstructorName") != '') ? unescape($(this).attr("sessionInstructorName")) : "&nbsp;";
					//var insHead = titleRestricted(sesionsInsH,11);
					ostr += '<div class="cls-sessionInstructor-container line-item-container"><div class="lbl-sessionInstructor sessionDetlbl container">'+Drupal.t("Instructor")+' :'+'</div> <div class="sessionInsName"><div class="sessInsName vtip" title="'+(($(this).attr("sessionInstructorName") != '') ? unescape($(this).attr("sessionInstructorName")) : "")+'">'+titleRestrictionFadeoutImage(sesionsInsH,'exp-sp-instructor-desk-session-instructor-name')+'</div></div></div>';
					ostr += '</div>';
					inc++;
				});
				ostr += '</div>';
				/*var sesinc = 1;
				$(sessionDet).each(function(){ //sessionDate sessionDay
				//alert(2);
					if(sesinc==1)
						{
						sesinc++;
						if(LocationName!='null' && LocationName!=''){
						alert(LocationName.length);
						if(LocationName.length > 115)
							loctitle = titleRestrictionFadeoutImage(LocationName,'exp-sp-instructor-desk-loc-head',110);
						else
							loctitle = unescape(LocationName);
						ostr += '<div class="sessAddDet location-row"><span class="location-label">'+Drupal.t("Location")+' :</span>';
						ostr += '<div class="location-desc"><span class="name-add">'+loctitle+',</span>';
						}
						//if($(this).attr("sessionfacName")!='null' && $(this).attr("sessionfacName")!='')
							//ostr += ',<br><span class="sessAddress">'+$(this).attr("sessionfacName")+'</span>';
						if($(this).attr("sessionfacAddress1")!='null' && $(this).attr("sessionfacAddress1")!='undefined' && $(this).attr("sessionfacAddress1")!='')
							ostr += '<span class="fac-add-a">'+unescape($(this).attr("sessionfacAddress1"))+',</span>';
						if($(this).attr("sessionfacAddress2")!='null' && $(this).attr("sessionfacAddress2")!='undefined' && $(this).attr("sessionfacAddress2")!='')
							ostr += '<span class="fac-add-b">'+unescape($(this).attr("sessionfacAddress2"))+',</span>';
						if($(this).attr("sessionfacCity")!='null' && $(this).attr("sessionfacCity")!='')
							ostr += '<span class="city-add">'+unescape($(this).attr("sessionfacCity"))+',</span>';
						if($(this).attr("sessionfacState")!='null' && $(this).attr("sessionfacState")!='')
							ostr += '<span class="state-add">'+unescape($(this).attr("sessionfacState"))+',</span>';
						if($(this).attr("sessionfacCountry")!='null' && $(this).attr("sessionfacCountry")!='')
							ostr += '<span class="country-add">'+unescape($(this).attr("sessionfacCountry"))+',</span>';
						if($(this).attr("sessionfacZipcode")!='null' && $(this).attr("sessionfacZipcode")!='')
							ostr += '<span class="zip-add">'+$(this).attr("sessionfacZipcode")+'</span>';
						}
						ostr += '</div></div>';
						

				});*/
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


			var userId = this.getLearnerId();
			var isAssigned = 0;
			var isMro = 0;
			if(userId == "0" || userId == "")
			{
			   self.location='?q=learning/enrollment-search';
		       return;
			}
			if(dataInfo.mro_type !=null && dataInfo.mro_type !='null' && dataInfo.mro_type !='') {
				isMro =1;
			}
			if(isMro == 0){
				var MandatoryValue = (dataInfo.MandatoryOption!=null && (dataInfo.MandatoryOption == 'Y') ? (dataInfo.MandatoryOption) : '');
					if(MandatoryValue) {
						//var displaymandatory="Mandatory<span class='enroll-pipeline'>|</span>";
						var displaymandatory="";
					}else{
						var displaymandatory='';
						}
					if(dataInfo.FullName !='' && dataInfo.FullName!= null){
						ostr += "<span title='"+created_by_ins_mngr_slf+"'>";
						ostr += displaymandatory+Drupal.t('LBL736')+" "+Drupal.t('by')+" : " + ((userId != dataInfo.Managerid) ? dataInfo.FullName : Drupal.t('Me'));
						ostr += "</span>";
						isAssigned = 1;
					}
			}

			//ostr += "</div>";
			ostr += '</div>';
			//ostr += '</td>';
			return ostr;
		 }catch(e){
	   		 // to do
	   	 }
		},
	getSessionInstructorDetails : function(sessData,sessionId) {
		try{
		var data= eval($(sessData).attr("data"));
		var sessionDet = '';

		if(!document.getElementById("session_det_ins"+sessionId)) {

			var sessionDet1 = '';
			$(data).each(function(){
				sessionDet1 += "<span class='enroll-session-time'>"+$(this).attr("sessionSDate")+" <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span> to "+$(this).attr("sessionEDate")+"<span class='time-zone-text'> "+$(this).attr("sessionEDayForm")+"</span> </span>";
			});
			$('#session_det_popup_ins'+sessionId).qtip({
				 content: '<div id="session_det_ins'+sessionId+'" class="tooltiptop"></div><div class="tooltipmid">'+sessionDet1+'</div><div class="tooltipbottom"></div>',
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
					background: 'none',
					color: '#333333'
				}
			});
		}
		}catch(e){
	   		 // to do
	   	 }
	},
	paintInstructorActions : function(cellval,options,rowObject){
		try{
		this.widgetObj = '$("#instructor-tab-inner").data("instructordesk")';
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
		var courseGrade				= rowObject['grade'];
		var dataId 					= rowObject['id'];
		var courseId 				= rowObject['course_id'];
		var classId 				= rowObject['id'];
		var classTitle				= rowObject['cls_title'];
		var crsTitle				= rowObject['course_title'];
		var courseTempId			= rowObject['courseid'];
		var surStatus				= rowObject['surveystatus'];
		var assessStatus			= rowObject['assessmentstatus'];
		var maxScoreValidation		= rowObject['maxscorevalue'];
		var session_id				= rowObject['session_id'];
		var launchInfo				= rowObject['launch'];
		var masterEnrollId			= rowObject['master_enrollment_id'];
		var mandatory			    = rowObject['mandatory'];
		var mro						= rowObject['mro'];
		var classStatus             = rowObject['classStatus'];
		var class_max_capacity      = rowObject['session_max_capacity'];
		var enrolled_count      = rowObject['enrolled_count'];
		$("#instructor-tab-inner").data("instructordesk").markcomplete	= rowObject['MarkComplete'];
		var labelmsg 				= rowObject['labelmsg'];
		this.localeArray = labelmsg;

		/*
		var labelmsgJSON = JSON.stringify(labelmsg);
		this.localeJSON = labelmsgJSON;
		$("#instructor-tab-inner").data("instructordesk").markcomplete	= rowObject['MarkComplete'];
		var labelmsg 				= rowObject['labelmsg'];
		this.localeArray = labelmsg;

		/*
		var labelmsgJSON = JSON.stringify(labelmsg);
		this.localeJSON = labelmsgJSON;
		*/
		var daysRemaining 			= '';
		var callLaunchUrlFn			= '';
		var contLaunch				= false; // Used to determine whether Launch link opens an accordian for multiple content class (true)
											 // or directly launches content (false)
		var IsLaunchable			= false;
		var errmsg					= '';
		var contValidateMsg			= '';
		var isMultipleCont			= false;
		var LessonLocation			= '';

		var html = '';

		 //click view button open close class Description
		   var viewcount = 0;
			$('#enroll-main-action-'+dataId +' .actionLink').live("click",function(){
				var getViewtxt=$(this).val();
				var getViewtxtUL=$(this).text();

				if(getViewtxt=="View" || getViewtxtUL=="View"){
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
				}
			});


		if(baseType=='VC'){
			 if(launchInfo['SessionType'] == "lrn_cls_vct_oth") {
			 	var presenterURL  = escape(launchInfo['PresenterLaunchURL']);
			 } else{
			 	var presenterURL  = escape(launchInfo['PresenterLaunchURL']+'/'+launchInfo['sessionId']);
			 }
			var ind=0;
			for(var i=0;i<rowObject.sessionDetails.length;i++){
				if(rowObject.sessionDetails[i].session_id == launchInfo['sessionId'] ){
					ind=i;
					break;
				}
			}
			var endSes = rowObject.sessionDetails.length - 1;
			var sTime = rowObject.sessionDetails[ind].session_start_format;
			var sTimeForm = rowObject.sessionDetails[ind].session_start_time_form;
			var stDateTimeFull = rowObject.sessionDetails[ind].session_start_time_full;
			var enDateTimeFull = rowObject.sessionDetails[ind].session_end_time_full;
			var eTime = rowObject.sessionDetails[ind].session_end_format;
			var eTimeForm = rowObject.sessionDetails[ind].session_end_time_form;
			var clEnDateFull = rowObject.sessionDetails[endSes].session_end_time_full;
			var isVC = true;
			var sesStartVC = new Date(stDateTimeFull);
			var sesEndVC   = new Date(enDateTimeFull);
			var srvDate = rowObject.sessionDetails[ind].server_date_time;
			var todayVC    = new Date(srvDate);
			var diffsession = (sesStartVC - sesEndVC);
			var timeDiffVC    = (sesStartVC - todayVC);
			var endTimeDiffVC =(sesEndVC - todayVC);
			var allowTime = (resource.allow_meeting_launch) ? (resource.allow_meeting_launch * 60000) : 3600000;
			var afterCompleteAllowTime = (resource.allow_meeting_launch_complete) ? (resource.allow_meeting_launch_complete * 60000) : 1800000;
			var diff = (diffsession+afterCompleteAllowTime);
			var label = this.localeArray['launch'];
			var message='';
			if(timeDiffVC > allowTime || (endTimeDiffVC+afterCompleteAllowTime) <= 0){
                if(endTimeDiffVC+afterCompleteAllowTime <= 0){
                        message = "MEETING_END";//Drupal.t("MSG419");
                }
                else{
                        message = Drupal.t("MSG420")+ ' ' + sTime +' <span class=enroll-session-timefrom>'+sTimeForm+'</span>';
                }
			}
			//Set Timer for auto refresh 
			if(message!="MEETING_END" && (label == Drupal.t('LBL1318') || label == Drupal.t('LBL1319'))){			
					if(todayVC < sesEndVC){
				var a = (sesEndVC-todayVC);
				var timer = (afterCompleteAllowTime+a);
				}else{
					var a = (sesEndVC-todayVC);
					var timer = afterCompleteAllowTime+a;
				}
					sessionend(a,timer);
			}				
			if(message=="MEETING_END"){
				html += "<div class='enroll-launch-full' id='enroll-main-action-"+dataId+"'>";
				html +="<span  data='"+labelmsg+"' id='selEnrollAction-"+dataId+"' onClick=\"$('#instructor-tab-inner').data('instructordesk').viewLearners('"+classId+"', this,'"+classStatus+"');\" >";
				html += "</span>";
				//html += "<label class='enroll-launch-full'><div class='admin-save-button-left-bg'></div>";
				html += "<input type='button' data='"+labelmsg+"' class='actionLink enroll-launch instructor-enr-launch clsDisabledButton' value='"+this.localeArray['roster']+"' name='"+this.localeArray['roster']+"'/>";
				//html += "<div class='dd-btn'><a data='"+labelmsg+"' class='enroll-launch-more' onClick='$(\"#instructor-tab-inner\").data(\"instructordesk\").instructorShowMoreAction(this);'></a><span class='dd-arrow'></span></div>";
				//html += '<div id="show_more_option'+dataId+'" class="enroll-action"><ul class="enroll-drop-down-list">';
				//html += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" name="'+this.localeArray['roster']+'" onclick=\'$("#instructor-tab-inner").data("instructordesk").viewLearners("'+classId+'", this,"'+classStatus+'")\' >'+this.localeArray['roster']+'</a></li>';
				//html += "</ul></div>";
				html += "</div>";
			}else{
				message = escape(message);
				html += "<div class='enroll-launch-full' id='enroll-main-action-"+dataId+"'>";
				html += "<input type='button' data='"+labelmsg+"' class='actionLink enroll-launch instructor-enr-launch' value='"+this.localeArray['launch']+"' name='"+this.localeArray['launch']+"' onClick=\"$('#instructor-tab-inner').data('instructordesk').launchPresenter('"+presenterURL+"','"+message+"',this)\" />";
				//html += "<input type='button' data='"+labelmsg+"' class='enroll-launch-more' onClick='$(\"#show_more_option"+dataId+"\").instructorShowMoreAction();' />";
				//html += "<div class='dd-btn'><a data='"+labelmsg+"' class='enroll-launch-more' onClick='$(\"#instructor-tab-inner\").data(\"instructordesk\").instructorShowMoreAction(this);'></a><span class='dd-arrow'></span></div>";
				//html += '<div id="show_more_option'+dataId+'" class="enroll-action"><ul class="enroll-drop-down-list">';
				//html += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" name="'+Drupal.t('LBL816')+'" onclick=\'$("#instructor-tab-inner").data("instructordesk").viewClassDetails("'+classId+'", this,"'+dataDelTypeCode+'");\' >'+Drupal.t('LBL81cc6')+'</a></li>';
				html += '<li class="action-enable" style="display:none;"><a  href="javascript:void(0);" id="selEnrollAction-'+dataId+'" class="actionLink" name="'+this.localeArray['roster']+'" onclick=\'$("#instructor-tab-inner").data("instructordesk").viewLearners("'+classId+'", this,"'+classStatus+'")\' >'+this.localeArray['roster']+'</a></li>';
				//html += '</ul></div>';
				html += "</div>";
			}
		}else{
			html += "<div class='enroll-launch-full' id='enroll-main-action-"+dataId+"'>";
			html +="<span  data='"+labelmsg+"' id='selEnrollAction-"+dataId+"' onClick=\"$('#instructor-tab-inner').data('instructordesk').viewLearners('"+classId+"', this,'"+classStatus+"');\" >";
			html += "</span>";
			//html += "<label class='enroll-launch-full'><div class='admin-save-button-left-bg'></div>";
			html += "<input type='button' class='actionLink enroll-launch clsDisabledButton' value='"+this.localeArray['roster']+"' name='"+this.localeArray['roster']+"'/>";
			
			//html += "<div class='dd-btn'><a data='"+labelmsg+"' class='enroll-launch-more' onClick='$(\"#instructor-tab-inner\").data(\"instructordesk\").instructorShowMoreAction(this);'></a><span class='dd-arrow'></span></div>";
			//html += '<div id="show_more_option'+dataId+'" class="enroll-action"><ul class="enroll-drop-down-list">';
			//html += '<li class="action-enable"><a href="javascript:void(0);" class="actionLink" name="'+this.localeArray['roster']+'" onclick=\'$("#instructor-tab-inner").data("instructordesk").viewLearners("'+classId+'", this,"'+classStatus+'")\' >'+this.localeArray['roster']+'</a></li>';
			//html += "</ul></div>";
			html += "</div>";
		}
		html += '<div class="progress" id="progress_'+dataId+'"></div>';
		html += '<div class="cls-seats-avail"> <div class="clsseatenrolledincompleted" id="selseatenrolledincompleted_'+dataId+'">'+enrolled_count+'/'+class_max_capacity+'</div>';
		html += '<div class="clsseatslable">'+Drupal.t("LBL106")+'</div></div>';
		if(enrolled_count>0)
			var completedPercentage = ((enrolled_count * 100)/class_max_capacity)/100;
		else
			var completedPercentage = 0;
		//$("#instructor-tab-inner").data("instructordesk").progressbarmc(dataId, completedPercentage);
		progressBarRound(dataId, completedPercentage, '','progress_',this);
		return html;
		}catch(e){
	   		 // to do
	   	 }
	},
	launchPresenter : function(launchData,message,data){
		try{
		//var msgobj = ($(data).attr("data"));
		disableWidgetDeleteAction('block-exp-sp-instructor-desk-tab-instructor-desk');	 // Prevent	 panel close while launch meet
		if(message==null || message == undefined || message == ''){
			var new_window;
			new_window = window.open(unescape(launchData),"Attachment", "width=1000,height=900,toolbar=no,location=no,directories=no,status=yes,menubar=no,scrollbars=yes,copyhistory=no,resizable=yes");
			new_window.onbeforeunload = function(){ activateWidgetDeleteAction('block-exp-sp-instructor-desk-tab-instructor-desk'); }
			refresh(new_window);
		}else{
			$("body").data("learningcore").callMessageWindow(Drupal.t('LBL670'),unescape(message));
		}
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
	getDropPolicy:function(data)
	{
		try{
			$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			var dataobj= eval($(data).attr("data"));

			var isCommerceEnabled='';
		    var assMand =0;
		    if(dataobj.Mandatory =='Y'){
		    	assMand =1;
		    }
		    var mandByRole	= 0;
		    if(dataobj.Mro =='Mandatory'){
		    	mandByRole =1;
		    }

		    if(!assMand && !mandByRole){
			    if(availableFunctionalities.exp_sp_commerce != undefined && availableFunctionalities.exp_sp_commerce != "")
			    {
			    	isCommerceEnabled = "1";
			    }
		    }
			var closeButt={};
		    $('#dropMsg-wizard').remove();
		  	html="";
			html+='<div id="dropMsg-wizard" style="display:none; padding: 13px;">';
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
		       title:Drupal.t('LBL3081') + " " + Drupal.t('Canceled'),
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
			$("#instructor-tab-inner").data("instructordesk").getDropPolicyCall(dataobj.EnrollId, dataobj.BaseType, dataobj.ClassId, isCommerceEnabled, assMand, unescape(dataobj.ClassTitle), mandByRole, dataobj.LearnerId, unescape(dataobj.FullName) ,dataobj.ClassStatus);
		}catch(e){
	   		 // to do
	   	 }
	},

	getDropPolicyCall : function(enrollId,baseType,classid,isCommerceEnabled,assMand,clstitle,mandByRole,learnerId, Name ,ClassStatus)
	{
		try{
		var userId = learnerId;
		var url = this.constructUrl("ajax/get-droppolicy/" + userId +  "/" + baseType + "/" + enrollId+"/"+classid+"/"+isCommerceEnabled+"/"+assMand+"/"+mandByRole);
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM			
			success: function(result){

				//var jsonObj=eval(result);
				$("body").append("<script>"+result+"</script>");
				var closeButt={};
				var learners = $('#username-'+learnerId+'_'+enrollId).val();
				var status=Drupal.t('Canceled');
				var cancelTitleLT = Drupal.t('Class');
				if(drop_policy_info.next_action=="drop")
				{
					$("#dropmsg-content").html('<span>'+Drupal.t('MSG822', {'@learner_name': learners, '@status': status, '@objtype': cancelTitleLT})+'</span>');
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
					if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var esignObj = {'popupDiv':'instructor-cancel-dialog','esignFor':'InstructorCancel','enrollId':enrollId,'baseType':baseType,'refundflag':refundflag,'isCommerceEnabled':isCommerceEnabled,'assMand':assMand,'learnerId':learnerId,'classid':classid};
					closeButt[Drupal.t('Yes')]=function(){  $.fn.getNewEsignPopup(esignObj); };
					}else{
					closeButt[Drupal.t('Yes')]=function(){ $("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"true",isCommerceEnabled,assMand,learnerId,classid);};
					}
					//$("#dropmsg-content").html('<span>'+SMARTPORTAL.t("Do you want to cancel <i>"+unescape(Name)+"</i> enrollment for <i>"+unescape(clstitle)+"</i>?")+'</span>');
					//obj.dropEnrollCall(enrollId, baseType);
					closeButt[SMARTPORTAL.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand); $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove(); };
					//closeButt[SMARTPORTAL.t('Yes')]=function(){ $("#learner-enrollment-tab-inner").data("enrollment").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand,ClassStatus); $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove(); };
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
				}else if(drop_policy_info.next_action=="showmsg")
				{
					$("#dropmsg-content").html('<span>'+SMARTPORTAL.t(drop_policy_info.msg)+"</span>");

					var html="";
					html+='<span><span class="dropmsgTitle">Title: </span>'+unescape(clstitle)+"</span>";
					html+="<br/><br/>";
					html+="<span>"+SMARTPORTAL.t(drop_policy_info.msg)+'</span>';
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
				}else if(drop_policy_info.next_action=="showdroppolicy")
				{
					var html="";
					/* html+='<span><span class="dropmsgTitle">Title: </span>'+unescape(clstitle)+"</span>";
					html+="<br/><br/>"; */
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
					//html+='<span>Please <a href="javascript:alert(\'Yet to implement\');">Click here</a> to see the refund policy.</span>';
					//html+="<br/><br/>";
					html+="<span>"+Drupal.t("MSG266")+'</span>';
					$("#dropmsg-content").html(html);
					closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#dropMsg-wizard').remove();};
				}
				if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
					var refundflag = (drop_policy_info.next_action=="drop") ? "" : ((drop_policy_info.next_action=="showdroppolicy") ? "true" : "");
					var esignObj = {'popupDiv':'instructor-cancel-dialog','esignFor':'InstructorCancel','enrollId':enrollId,'baseType':baseType,'refundflag':refundflag,'isCommerceEnabled':isCommerceEnabled,'assMand':assMand,'learnerId':learnerId,'classid':classid};
					if(drop_policy_info.next_action=="drop")
					{
						closeButt[Drupal.t('Yes')]=function(){  $.fn.getNewEsignPopup(esignObj);   };
					}else if(drop_policy_info.next_action=="showdroppolicy")
					{
						closeButt[Drupal.t('Yes')]=function(){  $.fn.getNewEsignPopup(esignObj); };
					}
				}else{
					if(drop_policy_info.next_action=="drop")
					{
						closeButt[Drupal.t('Yes')]=function(){  $("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"",isCommerceEnabled,assMand,ClassStatus,learnerId,classid); $("#dropmsg-content").html(Drupal.t("MSG424"));  };
					}else if(drop_policy_info.next_action=="showdroppolicy")
					{
						closeButt[Drupal.t('Yes')]=function(){ $("#instructor-tab-inner").data("instructordesk").dropEnrollCall(enrollId,baseType,"true",isCommerceEnabled,assMand,ClassStatus,learnerId,classid); $("#dropmsg-content").html(Drupal.t("MSG424")); };
					}
				}
				$("#instructor-tab-inner").data("instructordesk").destroyLoader('instructor-result-container');
				$("#dropMsg-wizard").dialog("open");
				$("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
				$('.ui-dialog').wrap("<div id='instructor-cancel-dialog'></div>");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
					 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

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
	dropEnrollCall : function(enrollId,baseType,refundflag,isCommerceEnabled,assMand,ClassStatus,learnerId, classid){
		try{
		closeButt=new Array();
		$("#dropMsg-wizard").dialog('option', 'buttons', closeButt);
		var userId = learnerId;
		isCommerceEnabled = isCommerceEnabled == ''? 0 : isCommerceEnabled;
		var url = this.constructUrl("learning/instructor/cancel-classes/" + baseType + "/" + enrollId+"/"+classid+"/"+ userId+"/"+refundflag+"/"+isCommerceEnabled);
		//var url = this.constructUrl("ajax/drop-enroll/" + userId +  "/" + baseType + "/" + enrollId+"/"+refundflag+"/"+isCommerceEnabled+"/"+assMand+"/0");
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
				$("body").append("<script>"+result+"</script>");
				if(result.indexOf('success') != -1)
				{
				 	var dialogObj=$("#dropMsg-wizard").dialog();
				 	dialogObj.dialog('destroy');
				 	dialogObj.dialog('close');
				 	$('#dropMsg-wizard').remove();

					$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp","Enrollmentpart");
					if(document.getElementById('learningplan-tab-inner')) {
						$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_enr|lrn_tpm_ovr_inp","EnrollmentLP");
					}
					//$("#instructor-tab-inner").data("instructordesk").callInstructor("scheduled");

					var data = eval($("#instructor-result-container #enroll-main-action-"+classid).parents("tr").find("a").attr("data"));
	        if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
					  $("#instructor-result-container #enroll-main-action-"+classid).parents("tr").children("td").css("border-bottom","solid 1px #cccccc");
	        }
	        
	        // Commenting the below line for 0081359: Classes disappear from My classes widget after after cancelling the enrollments
				//	$("#instructor-result-container #enroll-main-action-"+classid).parents("tr").next("tr").remove();
					// Reload the learner view
			    $("#instructor-tab-inner").data("instructordesk").viewLearners(classid, "#instructor-result-container #enroll-main-action-"+classid,ClassStatus);

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

	loadComplete : function(){
		try{
	    var recs = $("#paintEnrollmentResults").getGridParam("records");
	    if (recs == 0) {
	        $("#paintEnrollmentResults").html(Drupa.t("MSG381")+'.');
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

	/*
	 * To call the render function to show the registered class(learning)
	 * for a particular user at manager dash board
	 */
	viewLearners : function(classId,rowObj,classStatus){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var data = eval($(rowObj).parents("tr").find("a").attr("data"));

		if (document.getElementById('cus-accord-'+classId)) {
		  if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
			  if(this.currTheme == "expertusoneV2"){
				  if(!document.getElementById('classes-list-inst-'+classId)){
					//  $(rowObj).parents("tr").find("td").css("border-bottom","1px solid #EDEDED");
				  }else{
					//  $(rowObj).parents("tr").next().find("td").css("border-bottom","1px solid #EDEDED").css('padding-bottom','20px');
				  }
			  }
			  else
			  {
				  if(!document.getElementById('classes-list-inst-'+classId)){
					 // $(rowObj).parents("tr").children("td").css("border-bottom","solid 1px #cccccc");
				  }else{
					//  $(rowObj).parents("tr").next().children("td").css("border-bottom","solid 1px #cccccc");
				  }
			  }
		  }
			$('#cus-accord-'+classId).remove();
			$('#selEnrollAction-'+classId).click();
		} else {
			this.renderLearnerResults(classId, rowObj,classStatus);
		}
		}catch(e){
	   		 // to do
	   	}
	},

	/*
	 * To render the registered class(learning) for a particular user at manager dash board
	 */
	renderLearnerResults : function(classId,rowObj,classStatus) {
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
	 		if(Drupal.settings.mylearning_right===false)
	 			var enrollGwidth = 749;
	 		else
	 			var enrollGwidth = 580;
		}else{
			var enrollGwidth = 648;
	    }
	  var data = eval($(rowObj).parents("tr").find("a").attr("data"));
	  var noBorderLastRecClass = ($("#pager-ins").is(":visible") || data.is_last_rec != 'last')? '' : ' noborder';
	  var cusHTML = '<div class="loaderimg_customstyle" id="paintClassLearnersList_loader'+classId+'"></div>';
	     cusHTML += '<input type="hidden" name="hidden_idlist_'+classId+'" id="hidden_idlist_'+classId+'"><table id="paintClassLearnersList'+classId+'"></table><div class="seat-details" id="seat_details_'+classId+'"></div><div class="instructor-learner-pager" style="display:none;" id="pager2_user'+classId+'" style="width:400px;"></div>';
	     var markComp = "<label class='enroll-launch-full'><div class='admin-save-button-left-bg'></div>";
	    	 markComp += "<input type='button' class='mark-complete enroll-launch admin-save-button-middle-bg' " +
				"value='" + Drupal.t("LBL059") + "' " +
					"name='" + SMARTPORTAL.t("MarkCompletion") + "' " +
							'onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').learnerMarkCompleteWizard(\''+1+'\',\''+1+'\',\''+data.title+'\',\''+1+'\',\''+classId+'\',\''+'Name'+'\',\''+classStatus+'\');"' +
								" ><div class='admin-save-button-right-bg'></div></label>";

		/*var cusHTML = '<div class="loaderimg_customstyle" id="paintClassLearnersList_loader'+classId+'"></div>';
		cusHTML += '<table id="paintClassLearnersList'+classId+'"></table><div class="seat-details" id="seat_details_'+classId+'"></div><div class="instructor-learner-pager" style="display:none;" id="pager2_user'+classId+'" style="width:400px;"></div>';*/
		if($('#classes-list-inst-'+classId).is(":visible")){
			$('#selclsdetailsrow-'+classId).after("<div id='cus-accord-"+classId+"' class='cus-accord instructor-learner-tab" + noBorderLastRecClass + "'>"+cusHTML+"</div>");
			//$(rowObj).parents("tr").after("<tr id='cus-accord-"+classId+"' class='cus-accord " + noBorderLastRecClass + "'><td colspan='3' class='instructor-learner-tab'>"+cusHTML+"</td></tr>");
		}else{
			$('#selclsdetailsrow-'+classId).after("<div id='cus-accord-"+classId+"' class='cus-accord instructor-learner-tab" + noBorderLastRecClass + "'>"+cusHTML+"</div>");
			//$(rowObj).parents("tr").next("tr").before("<tr id='cus-accord-"+classId+"' class='cus-accord " + noBorderLastRecClass + "'><td colspan='3' class='instructor-learner-tab'>"+cusHTML+"</td></tr>");
		}

		if(this.currTheme == "expertusoneV2" &&  data.is_last_rec != 'last'){
			if(!document.getElementById('classes-list-inst-'+classId)){
				//$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
				//$(rowObj).parents("tr").next("tr").find("td").css('border-bottom','1px solid #EDEDED').css('padding-bottom','0');
			}
			else{
				//$(rowObj).parents("tr").next("tr").next("tr").find("td").css('border-bottom','1px solid #EDEDED').css('padding-bottom','0');
			}
		}
		if( (data.is_last_rec == 'last') && (data.deliverytype == 'lrn_cls_dty_vcl') && (this.currTheme == 'expertusoneV2') || (data.is_last_rec == 'last') && (data.deliverytype == 'lrn_cls_dty_ilt') && (this.currTheme == 'expertusoneV2') ){

			if(!document.getElementById('classes-list-inst-'+classId)){
				$("#ins-class-accodion-"+classId).parents("tr").find("td").css("border-bottom","none");
				$(rowObj).parents("tr").next("tr").find("td").css('border-bottom','0px solid #EDEDED').css('padding-bottom','0');
			}else{
				$(rowObj).parents("tr").next("tr").next("tr").find("td").css('border-bottom','0px solid #EDEDED').css('padding-bottom','0');
			}

		}

		if((data.is_last_rec == 'last') && (data.deliverytype == 'lrn_cls_dty_ilt') && (this.currTheme == 'expertusoneV2') ){
		  $(rowObj).parents("tr").next("tr").find("td").css('border-bottom','0px solid #EDEDED').css('padding-bottom','0');
		}

		// Commenting the below lines for 0081359: Classes disappear from My classes widget after after cancelling the enrollments

	/*	if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
			if(document.getElementById('classes-list-inst-'+classId)){
			$("#ins-class-accodion-"+classId).parents("tr").next().find("td").css("border-bottom","none");
			}
		}*/ 
		$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
		$(".loaderimg_customstyle").css('position', 'relative');
		this.createLoader('paintClassLearnersList_loader'+classId);
		//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');

		var obj = this;
		var objStr = '$("#instructor-tab-inner").data("instructordesk")';
		var pagerId	= '#pager2_user'+classId;
		var multiselectCheckbox = '<div align="center" class="checkbox-unselected"><input type="checkbox" id="multiselect-selectall-'+classId+'" class="multiselect-selectall" onclick ="checkboxSelectedUnselectedCommon(this);" /></div>';

		var tableColoumn ='';
		var colModelName = '';
		var colModelIndex = '';
		if(classStatus == 'scheduled'){
			tableColoumn = Drupal.t('LBL108');
			colModelName = 'Action';
			colModelIndex = 'action';
		}else if(classStatus == 'completed'){
			tableColoumn = multiselectCheckbox;
			colModelName = 'MultiselectCheckBox';
			colModelIndex = 'MultiselectCheckBox';
		}
            var col1Width = 190, col2Width = 178, col3Width = 100, col4Width = 70, col5Width = 110; col6Width = 60;
			if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'fr') {
				col3Width = 90, col4Width = 70, col5Width = 120; col6Width = 60;
			}else if(Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'de') {
				col1Width = 180, col2Width = 160, col3Width = 100, col4Width = 80, col5Width = 110; col6Width = 78;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'it'){
			    col2Width = 170, col3Width = 80, col4Width = 70, col5Width = 138; col6Width = 60;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ja'){
			    col3Width = 80, col4Width = 90, col5Width = 100; col6Width = 70;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ko'){
			    col1Width = 220, col2Width = 208, col3Width = 80, col4Width = 50, col5Width = 100; col6Width = 50;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'pt-pt'){
			    col1Width = 220, col2Width = 208, col3Width = 80, col4Width = 80, col5Width = 80; col6Width = 60;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'ru'){
			    col1Width = 180, col2Width = 160, col3Width = 98, col4Width = 88, col5Width = 130; col6Width = 60;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'zh-hans'){
			    col1Width = 220, col2Width = 208, col3Width = 90, col4Width = 60, col5Width = 80; col6Width = 50;
			}else if (Drupal.settings.user.language !== undefined && Drupal.settings.user.language == 'es'){
			    col3Width = 90, col4Width = 80, col5Width = 100; col6Width = 70;
			}
			
		if(classStatus != 'scheduled' && $("#instructor-tab-inner").data("instructordesk").markcomplete != 'Y'){
		console.log("not scheduled");
			var colNames1=[Drupal.t('LBL107'), Drupal.t('LBL054'),Drupal.t('LBL704'), Drupal.t('LBL102'), Drupal.t('Preassess-Score')];
			var colModel1 =[ {name:'Name',index:'name', title:false, width:235, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			                 {name:'UserName',index:'username', title:false, width:171, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			                 {name:'Enrolled-On',index:'enrolled_on', title:false, width:142,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			                 {name:'Status',index:'status', title:false, width:88,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			                 {name:'PreassessScore',index:'prescore', title:false, width:140,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
					           ];

		}else if(classStatus == 'scheduled'){
		console.log("scheduled");
			var colNames1=[Drupal.t('LBL107'), Drupal.t('LBL054'), Drupal.t('LBL704'), Drupal.t('LBL102'), Drupal.t('Preassess-Score'), tableColoumn];
			if(this.currTheme == "expertusoneV2"){
				colModel1= [ {name:'Name',index:'name', title:false, width:col1Width, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'UserName',index:'username', title:false, width:col2Width, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'Enrolled-On',index:'enrolled_on', title:false, width:col3Width,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'Status',index:'status', title:false, width:col4Width,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'PreassessScore',index:'prescore', title:false, width:col5Width,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:colModelName,index:colModelIndex, title:false, classes: "learners-for-classes", width:col6Width,resizable:false,align:"center",sortable:false,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails}
				           ];
			}
			else{
			console.log("scheduled else");
			    colModel1= [ {name:'Name',index:'name', title:false, width:170, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'UserName',index:'username', title:false, width:120, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'Enrolled-On',index:'enrolled_on', title:false, width:115,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'Status',index:'status', title:false, width:80,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'PreassessScore',index:'prescore', title:false, width:135,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:colModelName,index:colModelIndex, title:false, classes: "learners-for-classes", width:100,resizable:false,align:"center",sortable:false,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails}
			           ];
			}

		}else{
			console.log("not scheduled else");
			var colNames1=[Drupal.t('LBL107'), Drupal.t('LBL054'), Drupal.t('LBL704'), Drupal.t('LBL102'), tableColoumn];
			if(this.currTheme == "expertusoneV2"){
				colModel1= [ {name:'Name',index:'name', title:false, width:180, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'UserName',index:'username', title:false, width:148, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'Enrolled-On',index:'enrolled_on', title:false, width:115,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:'Status',index:'status', title:false, width:120,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
				             {name:colModelName,index:colModelIndex, title:false, classes: "learners-for-classes comp-chkbox", width:68,resizable:false,align:"center",sortable:false,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails}
				           ];
			}
			else{
			    colModel1= [ {name:'Name',index:'name', title:false, width:180, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'UserName',index:'username', title:false, width:125, classes:'learners-for-classes', resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'Enrolled-On',index:'enrolled_on', title:false, width:120,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:'Status',index:'status', title:false, width:80,classes:'learners-for-classes',resizable:false,sortable:true,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails},
			             {name:colModelName,index:colModelIndex, title:false, classes: "learners-for-classes", width:80,resizable:false,align:"center",sortable:false,'widgetObj':objStr,formatter:obj.paintMyLearnerDetails}
			           ];
			}

		}
		$("#paintClassLearnersList"+classId).jqGrid({
			url:this.constructUrl("learning/instructor-classes-learners/"+classId+"/&tab="+this.instructortab),
			datatype: "json",
			mtype: 'GET',
			colNames:colNames1,
			colModel:colModel1,
			rowNum:5,
			rowList:[5,10,15],
			pager: pagerId,
			viewrecords:  true,
			emptyrecords: "",
			headertitles: true ,
			sortorder: "desc",
			toppager:false,
			height: 'auto',
			sortname:'enrolled_on',
			width: enrollGwidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			loadComplete:obj.callbackMyLearnerLoader,
			gridComplete:function (){
				$("#jqgh_paintClassLearnersList"+classId+"_Action").removeClass('ui-jqgrid-sortable');
				// Sortable Remove in Multi select CheckBox
				$("#jqgh_paintClassLearnersList"+classId+"_MultiselectCheckBox").removeClass('ui-jqgrid-sortable');
				if($("#paintClassLearnersList"+classId).getDataIDs().length == 0) {
					$("#jqgh_paintClassLearnersList"+classId+"_Name").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintClassLearnersList"+classId+"_Enrolled-On").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintClassLearnersList"+classId+"_Status").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintClassLearnersList"+classId+"_Action").removeClass('ui-jqgrid-sortable');
					$("#jqgh_paintClassLearnersList"+classId+"_Name > span").css('display','none');
					$("#jqgh_paintClassLearnersList"+classId+"_Enrolled-On > span").css('display','none');
					$("#jqgh_paintClassLearnersList"+classId+"_Status > span").css('display','none');
					$("#jqgh_paintClassLearnersList"+classId+"_Action > span").css('display','none');
				}
				$("#instructor-tab-inner").data("instructordesk").destroyLoader('paintLearnerResults'+classId);
			}
		}).navGrid(pagerId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
		$("#paintClassLearnersList"+classId+'_toppager').hide();
		if(this.currTheme == "expertusoneV2"){
			$("#gview_paintClassLearnersList"+classId).find('tr td').css('padding','0px');

	/*
		$("#paintClassLearnersList"+classId).find('tr.ui-widget-content').find('td:nth-child(2)').css('text-indent','2px');
		$("#paintClassLearnersList"+classId).find('tr.ui-widget-content').find('td:nth-child(3)').css('text-indent','2px');
		$("#paintClassLearnersList"+classId).find('tr.ui-widget-content').find('td:nth-child(4)').css('text-indent','4px');
		$("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','100px');

		 if(classStatus == 'scheduled'){
			$("#paintClassLearnersList"+classId).find('tr').find('td:first-child').css('width','115px');
			$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(2)').css('width','98px');
			$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(3)').css('width','94px');
			$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(4)').css('width','68px');
			$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(5)').css('width','110px');
			$("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','72px');
			$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(5)').css('text-indent','3px').css('padding','0 4px');

			$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(2)').css('width','94px');
			if($.browser.msie && parseInt($.browser.version, 10)=='8'){
				$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(2)').css('width','97px');
			}
			$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(4)').css('width','67px');

			 if($.browser.msie && parseInt($.browser.version, 10)=='8'){
				$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(5)').css('width','103px').css('padding-right','2px');
		}
		     if(navigator.userAgent.indexOf("Chrome")>0){
			    $("#paintClassLearnersList"+classId).find('tr').find('td:first-child').css('width','130px');
				  $("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(2)').css('width','75px');
				  $("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(3)').css('width','100px');
				  $("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(4)').css('width','63px');
				  $("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(5)').css('width','105px');
				  $("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','73px');

				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:first-child').css('width','128px');
				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(2)').css('width','92px');
				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(3)').css('width','92px');
				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(4)').css('width','56px');
				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(5)').css('width','99px');
				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:last-child').css('width','74px');

				  $("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(5)').css('text-indent','-1px');
			  }
		 }else if(classStatus != 'scheduled'){
			  $("#gview_paintClassLearnersList"+classId).find('tr').find('td:nth-child(2)').css('width','106px');
			  $("#gview_paintClassLearnersList"+classId).find('tr').find('td:nth-child(3)').css('width','112px');
			  $("#gview_paintClassLearnersList"+classId).find('tr').find('td:nth-child(4)').css('width','99px');
			  $("#gview_paintClassLearnersList"+classId).find('tr').find('td:nth-child(5)').css('width','95px');
		 }
		 else if(classStatus != 'scheduled' && navigator.userAgent.indexOf("Chrome")>0){
			    $("#paintClassLearnersList"+classId).find('tr').find('td:first-child').css('width','190px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(2)').css('width','137px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(3)').css('width','146px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(4)').css('width','120px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','100px');
		 }
		*//*}else{
			if(classStatus == 'scheduled'){
				$("#paintClassLearnersList"+classId).find('tr').find('td:first-child').css('width','152px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(2)').css('width','103px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(3)').css('width','101px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(4)').css('width','70px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:nth-child(5)').css('width','106px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','87px');
				$("#gview_paintClassLearnersList"+classId).find('tr').find('th:nth-child(5)').css('text-indent','3px').css('padding','0 4px');
			}else if(classStatus !='scheduled'){
				$("#gview_paintClassLearnersList"+classId).find('tr').find('th:last-child').css('width','152px');
				$("#paintClassLearnersList"+classId).find('tr').find('td:last-child').css('width','90px');
				}*/

		}
		if($.browser.msie && $.browser.version == 8 && this.currTheme != "expertusoneV2" || $.browser.msie && $.browser.version == 9 && this.currTheme != "expertusoneV2"){
		   $("#gview_paintClassLearnersList"+classId).find('tr').find('th').css('padding-right','5px');
		   $("#gview_paintClassLearnersList"+classId).find('tr').find('th:last-child').css('padding-left','7px');
		   $("#paintClassLearnersList"+classId).css('float','left');
		}
		if(classStatus == 'completed'){
			//$("#paintClassLearnersList"+classId).append('<div class="paint-mark-completion">'+markComp+'</div>');
			$("#gview_paintClassLearnersList"+classId+" .ui-jqgrid-bdiv").after('<div class="paint-mark-completion" style="display:none;">'+markComp+'</div>');
		}
		this.paintMyclassAfterReady(classId);
		$('.ui-jqgrid-bdiv > div').css('position','');
		}catch(e){
	   		 // to do
	   	}
	},

	paintMyclassAfterReady : function(classId) {
		try{
		var html = "<div id='paintLearnerResults"+classId+"'></div>";
        $("#paintLearnerResults"+classId).remove();
        $("#gview_paintClassLearnersList"+classId).prepend(html);

		$('#prev_pager2_user'+classId).click( function(e) {
			if(!$('#prev_pager2_user'+classId).hasClass('ui-state-disabled')) {
				//$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
				//$(".loaderimg_customstyle").css('position', 'absolute');
				$("#instructor-tab-inner").data("instructordesk").createLoader('paintClassLearnersList_loader'+classId);
				//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');
				$('#loaderdivpaintLearnerResults'+classId+' td').css('border-bottom','none');
			}
		});
		$('#next_pager2_user'+classId).click( function(e) {
			if(!$('#next_pager2_user'+classId).hasClass('ui-state-disabled')) {
				//$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
				//$(".loaderimg_customstyle").css('position', 'absolute');
				$("#instructor-tab-inner").data("instructordesk").createLoader('paintLearnerResults'+classId);
				//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');
				$('#loaderdivpaintLearnerResults'+classId+' td').css('border-bottom','none');
			}
		});


		$('#pager2_user'+classId+' .ui-pg-selbox').bind('change',function() {
			$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
			$(".loaderimg_customstyle").css('position', 'absolute');
			$("#instructor-tab-inner").data("instructordesk").createLoader('paintClassLearnersList_loader'+classId);
			//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');
			//$('#loaderdivpaintLearnerResults'+classId+' td').css('border-bottom','none');
		});
		$("#pager2_user"+classId+" .ui-pg-input").keyup(function(event){
			if(event.keyCode == 13){
				//$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
				//$(".loaderimg_customstyle").css('position', 'absolute');
				$("#instructor-tab-inner").data("instructordesk").createLoader('paintClassLearnersList_loader'+classId);
				//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');
				$('#loaderdivpaintClassContentResults'+classId+' table>tbody>tr>td').css('border-bottom','none');
			}
		});

		$('#gbox_paintClassLearnersList'+classId+' .ui-jqgrid-sortable').click( function(e) {
			if($("#paintClassLearnersList"+classId).jqGrid('getGridParam', 'records') > 1 && $(this).attr('class') == 'ui-jqgrid-sortable') {
				$("#paintLearnerResults"+classId).addClass('paint-class-content-loader');
				//$("#paintClassLearnersList_loader"+classId).addClass('paintClassLearnersList-loader-height');
				//$(".loaderimg_customstyle").css('position', 'absolute');
				$("#instructor-tab-inner").data("instructordesk").createLoader('paintClassLearnersList_loader'+classId);
				//$("#paintClassLearnersList_loader"+classId).addClass('loaderimg');
				$('#loaderdivpaintLearnerResults'+classId+' td').css('border-bottom','none');				
			}
		});
		}catch(e){
	   		 // to do
	   	}
	},
	/*
	 * To paint the class information in the data grid
	 */
	paintMyLearnerDetails: function(cellvalue, options, rowObject) {
		try{
		var index  = options.colModel.index;
		var Name 			= rowObject['Name'];
		var FullName 		= rowObject['FullName'];
		var Status 			= rowObject['Status'];
		var StatusText 		= rowObject['StatusText'];
		var CompStatus 		= rowObject['CompStatus'];
		var CompStatusText 	= rowObject['CompStatusText'];
		var PreStatus		= rowObject['PreStatus'];
		var PreScoreDisp	= rowObject['PreScore'];
		var LearnerId 		= rowObject['LearnerId'];
		var BaseType 		= rowObject['BaseType'];
		var EnrollId 		= rowObject['EnrollId'];
		var MasterEnrollId  = rowObject['MasterEnrollId'];
		var ClassId 		= rowObject['ClassId'];
		var Mandatory 		= rowObject['Mandatory'];
		var Mro 			= rowObject['Mro'];
		var ClassTitle 		= escape(rowObject['ClassTitle']);
		var EnrolledDate 	= rowObject['EnrolledDate'];
		var labelmsg 		= rowObject['labelmsg'];
		var UserName	 	= rowObject['UserName'];
		var Attendance	 	= rowObject['Attendance'];
		var CompStatusCount	 	= rowObject['CompStatusCount'];
		var IsSessionStart	 	= rowObject['IsSessionStart'];
		var sessionFlag	 	= rowObject['sessionFlag'];
		var ClassStatus = rowObject['ClassStatus'];
		//Dummy declaration for translation
		var dstatus1 = Drupal.t('Enrolled');
		var dstatus2 = Drupal.t('Completed');
		var dstatus3 = Drupal.t('Incomplete');
		var dstatus4 = Drupal.t('No Show');
		var dstatus5 = Drupal.t('In progress');
		var dstatus6 = Drupal.t('Attended');
		var StatusTextTmp = StatusText;
		//alert(PreScoreDisp);
		var mroImageClass = '';
		var mroImageClassArr = new Array();
		mroImageClassArr['arrived'] =  '<span class="instructor-arrived-action vtip" id="'+ClassId+EnrollId+'" title="'+Drupal.t('LBL1122')+'">'+Drupal.t('A')+'</span>';
		mroImageClassArr['notarrived'] =  '<span class="instructor-cancel-action vtip" id="'+ClassId+EnrollId+'" title="'+Drupal.t('LBL1125')+'">'+Drupal.t('N')+'</span>';

		if(ClassStatus == 'scheduled'){
			mroImageClass = (Attendance == 'Attended') ? mroImageClassArr['arrived'] : mroImageClassArr['notarrived'];
		}else{
			mroImageClass = '';
		}

		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var tab_difference_name = ClassStatus;
		if(index  == 'name') {
			var name = '';
			var titleLen = (this.currTheme == 'expertusoneV2') ? 10 : 17;
			name += titleRestrictionFadeoutImage(Name,'exp-sp-instructor-desk-name-'.concat(tab_difference_name));
			return '<span class="vtip" title="'+htmlEntities(Name)+'">'+name+'</span>'+mroImageClass;
		} else if(index  == 'username') {
		    var uname = '';
		    var titleLeng = (this.currTheme == 'expertusoneV2') ? 10 : 15;
		    uname += titleRestrictionFadeoutImage(UserName,'exp-sp-instructor-desk-username-'.concat(tab_difference_name));
			return '<span class="vtip" title="'+htmlEntities(UserName)+'"><input type="hidden" id="username-'+LearnerId+'_'+EnrollId+'" value="'+UserName+'">'+uname;
		} else if(index  == 'enrolled_on') {
			return EnrolledDate;
		} else if(index  == 'status') {
			if(CompStatus == 'lrn_crs_cmp_nsw' || CompStatus == 'lrn_crs_cmp_cmp'){
				return  Drupal.t(CompStatusText);
			} else {
				if(Status == 'lrn_crs_reg_cnf'){
					StatusTextTmp = CompStatusText;
				}
				if(StatusTextTmp == null || StatusTextTmp == ''){ StatusTextTmp = 'Enrolled'; }
				return Drupal.t(StatusTextTmp);
			}
		}else if(index  == 'prescore') {
			if(PreStatus == 'lrn_crs_cmp_cmp' || PreStatus == 'lrn_crs_cmp_inc'){
				return  PreScoreDisp;
			} else {
				return Drupal.t('Not yet taken');
			}
		}
		else if(index  == 'MultiselectCheckBox') {
			 if(CompStatus != 'lrn_crs_cmp_cmp'){
				   if(CompStatus == 'lrn_crs_cmp_nsw' || Status == 'lrn_crs_reg_can' || CompStatus == 'lrn_crs_cmp_inc'){
					   return '<div class="checkbox-unselected"><input type="checkbox" id="multiselect-singlecheck-'+ClassId+'_'+EnrollId+'" value="'+LearnerId+'_'+EnrollId+'" class="multiselect-singlecheck" disabled ="disabled" /></div>';
				   }else{
					   return '<div class="checkbox-unselected"><input type="checkbox" id="multiselect-singlecheck-'+ClassId+'_'+EnrollId+'" value="'+LearnerId+'_'+EnrollId+'" class="multiselect-singlecheck" onclick="checkboxSelectedUnselectedCommon(this);" /></div>';
				   }
			   }else{
				   return '';
			   }
		} else {
			if(ClassStatus == 'scheduled'){
				if(CompStatus == 'lrn_crs_cmp_nsw' || CompStatus == 'lrn_crs_cmp_cmp' || Status == 'lrn_crs_reg_can' || CompStatus == 'lrn_crs_cmp_inc' ){
					return '';
				} else {
					var cancelData="data={'LearnerId':'"+LearnerId+"','BaseType':'"+BaseType+"','EnrollId':'"+EnrollId+"','ClassId':'"+ClassId+"'," +
					"'Mandatory':'"+Mandatory+"','Mro':'"+Mro+"','ClassTitle':'"+ClassTitle+"','Name':'"+escape(Name)+"','FullName':'"+escape(FullName)+"','ClassStatus':'"+escape(ClassStatus)+"'}";
					if(Attendance == 'Attended'){
						return '<label class="enroll-launch-full"><input type="button" class="actionLink enroll-launch" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').dropEnroll(this);" data="'+cancelData+'" name="Cancel" value="'+Drupal.t('LBL109')+'" href="javascript:void(0);"></label>';
					}else{
						var htm = '';
						var action = '';
						if(IsSessionStart == 1){
							htm += '<label class="enroll-launch-full"><a class="actionLink enroll-launch" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').updateAttendanceStatus(\''+ClassId+'\',\''+LearnerId+'\',\''+EnrollId+'\',\''+ClassStatus+'\');" name="Arrived" value="'+Drupal.t('LBL1122')+'">'+Drupal.t('LBL1122')+'</a></label>';
						}else{
							if(sessionFlag == 1){
								htm += '<label class="enroll-launch-full"><input type="button" class="actionLink enroll-launch" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').dropEnroll(this);" data="'+cancelData+'" name="Cancel" value="'+Drupal.t('LBL109')+'" href="javascript:void(0);"></label>';
							}else{
							action += '<li class="action-enable"><a class="actionLink" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').dropEnroll(this);" data="'+cancelData+'" name="Cancel" href="javascript:void(0);">'+labelmsg['cancel']+'</a></li>';
							htm += '<input type="button" class="actionLink enroll-launch" onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').updateAttendanceStatus(\''+ClassId+'\',\''+LearnerId+'\',\''+EnrollId+'\',\''+ClassStatus+'\');" name="Arrived" value="'+Drupal.t('LBL1122')+'">';
							htm += "<div class='dd-btn'><input type='button' data='"+labelmsg+"' class='enroll-launch-more' onClick='$(\"#instructor-tab-inner\").data(\"instructordesk\").instructorShowMoreAction(this);' /><span class='dd-arrow'></span></div>";
							htm += '<div class="enroll-action arrived-cancel-link"><ul class="enroll-drop-down-list">' + action + '</ul></div>';
							}
						}
						return htm;
					}

					//return '<a onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').dropEnroll(this);" data="'+cancelData+'" name="Cancel" href="javascript:void(0);">'+labelmsg['cancel']+'</a>';
				}
			} else {
				if(CompStatus == 'lrn_crs_cmp_nsw' || CompStatus == 'lrn_crs_cmp_cmp' || Status == 'lrn_crs_reg_can' || Status == 'lrn_crs_reg_wtl'){
					return '';
				} else {
					if($("#instructor-tab-inner").data("instructordesk").markcomplete=='Y'){
						return '<a onClick="$(\'#instructor-tab-inner\').data(\'instructordesk\').learnerMarkCompleteWizard(\''+LearnerId+'\',\''+EnrollId+'\',\''+ClassTitle+'\',\''+BaseType+'\',\''+ClassId+'\',\''+escape(Name)+'\');" data="'+cancelData+'" name="Cancel" href="javascript:void(0);">'+labelmsg['mark_completion']+'</a>';
					}else{
						return '';
					}
				}
			}

		}
		}catch(e){
	   		 // to do
	   	}
	},

	updateAttendanceStatus: function(ClassId,LearnerId,EnrollId,ClassStatus){
		try{
		var obj = this;
		obj.createLoader('instructor-result-container');
		url = obj.constructUrl("learning/instructor/update-attendance/"+ClassId+"/"+LearnerId+"/"+EnrollId);
		$.ajax({
			type: "GET",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM			
			success: function(result){
				obj.destroyLoader('instructor-result-container');
				$("#instructor-tab-inner").data("instructordesk").viewLearners(ClassId, "#instructor-result-container #enroll-main-action-"+ClassId,ClassStatus);
			}
	    });
		// $("#gbox_paintClassLearnersList"+ClassId).trigger("reloadGrid");
		}catch(e){
	   		 // to do
	   	}
	},

	/*
	 * To Mark complete  wizard the class registration(Learning) in the manager dash board
	 */
	learnerMarkCompleteWizard: function(userId,enrolledId,classTitle,learningType,classId,name,ClassStatus){
		try{
		enrolledId = $('#hidden_idlist_'+classId).val();
		//alert('enrolledId '+enrolledId);
		
		if (enrolledId == '') {
		  $('body').data('learningcore').callMessageWindow(Drupal.t('MSG404')+ ' - ' + titleRestrictionFadeoutImage(unescape(classTitle), 'exp-sp-instructor-desk-markcompletion-classtitle'), Drupal.t('ERR106'));
		  $("#select-class-equalence-dialog .exp-sp-instructor-desk-markcompletion-classtitle").parent("#ui-dialog-title-commonMsg-wizard").attr('title',Drupal.t('MSG404')+' - '+unescape(classTitle));
          $("#select-class-equalence-dialog .exp-sp-instructor-desk-markcompletion-classtitle").parent("#ui-dialog-title-commonMsg-wizard").addClass('vtip');
          vtip();
		  return;
		}

		var userId = '';
		var learningType = '';
		var name = '';

		var scoreread = '';
		var graderead = '';
		if(learningType == "Program") {
			scoreread = 'disabled=""';
			graderead = 'disabled=""';
		}
		$('#mark-complete-wizard').remove();
		var html = '';
	    html+='<div id="mark-complete-wizard" style="display:none; padding:0 13px;">';
	    html+= '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+= '<tr><td align="left" colspan="2" valign="middle" height="8"></td></tr>';
	    html+= '<tr><td class="user-attendance-label" align="left" height="25" valign="middle" width="35%"><span>'+Drupal.t('LBL723') +': </span><span class="require-text">*</span></td><td><div class="mark-complete-wizard-popup-label"><input type="radio" name="attendance_type" id="attendance_type_attended" value="attended" checked> <span>'+Drupal.t('Attended') +'</span> <input type="radio" name="attendance_type" id="attendance_type_noshow" value="noshow"> <span>'+Drupal.t('No Show') +'</span></div></td></tr>';
	    html+= '<table>';
	    html+= '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center" class="markcompletion-attendance-fields">';
	    html+= '<tr><td align="left" height="25" valign="middle" width="35%"><span>'+Drupal.t('LBL706') +': </span><span class="require-text">*</span></td><td><input type="text"  name="comp_date" id="comp_date" size="14" readonly="true"></td></tr>';
	   /* html+= '<tr><td align="left" height="25" valign="middle" width="35%"><span>'+Drupal.t('LBL707') +': </span></td><td><select name="rosterGrade" id="rosterGrade"  '+graderead+' ><option value="">'+Drupal.t("LBL674")+'</option>'+$('#mark-complete-grade').html()+'</select></td></tr>';*/
	    html+= '<tr><td align="left" height="25" valign="middle" width="35%"><span>'+Drupal.t('LBL668') +': </span></td><td><input type="text" class="validateNum" '+scoreread+' id="appScore" maxlength="4" size="8"></td></tr>';
	    html+= '</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close'); $("#mark-complete-wizard").remove(); };
		closeButt[Drupal.t('LBL725')]=function(){  
				$(this).dialog('destroy');
				$(this).dialog('close');
				var learners;
				var status;
				var cancelTitleLT = Drupal.t('Class');
				var selectedLearners = enrolledId.split(',');
				if(selectedLearners.length == 1) {
					learners = $('#username-'+enrolledId).val();
				} else {
					learners = selectedLearners.length +" "+ Drupal.t('LBL3080');
				} 
					var attendance_type = $('input[name=attendance_type]:checked').val();
					if(attendance_type == 'noshow'){
					 status = Drupal.t('No Show');
					}
					else{
				    status = Drupal.t('Completed');
					}
					$('#complete-confirmation-wizard').remove();
					html="";
					html+='<div id="complete-confirmation-wizard" style="display:none; padding: 13px;">';
				    html+='<table width="100%" class="complete-confirmation-wizard-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
				    html+='<tr><td height="25"></td></tr>';
				    html+='<tr>';
				    html+= '<td align="center" id="complete-confirmation-wizard-content"><span>'+ Drupal.t('MSG822', {'@learner_name': learners, '@status': status , '@objtype': cancelTitleLT}) +'</span></td>';
				    html+='</tr>';
				    html+='</table>';
				    html+='</div>';
				    $("body").append(html);
				    var confButton = {};
				    confButton[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#complete-confirmation-wizard').remove();};
					if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
	    			var esignObj = {'popupDiv':'instructor-markcomplete-dialog','esignFor':'InstructorMarkComplete','enrolledId':enrolledId,'classId':classId};
	   				{
					confButton[Drupal.t('Yes')] = function(){ $.fn.getNewEsignPopup(esignObj); };}
					}
					else{
					confButton[Drupal.t('Yes')] = function(){
					$("#instructor-tab-inner").data("instructordesk").learnerMarkcompleteLearning('',enrolledId,classTitle,'',classId);	
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
					$("#complete-confirmation-wizard").dialog('open');	
	    	   }; 
	    $("#mark-complete-wizard").dialog({
	        position:[(getWindowWidth()-500)/2,100],
	        bgiframe: true,
	        width:500,
	        resizable:false,
	        modal: true,
	        title:Drupal.t('MSG404')+' - '+titleRestrictionFadeoutImage(unescape(classTitle),'exp-sp-instructor-desk-markcompletion-dialog-class-title'),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:true,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog').wrap("<div id='instructor-markcomplete-dialog'></div>");
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		 this.currTheme = Drupal.settings.ajaxPageState.theme;
		  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
	/*		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right');*/
			}
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		 	if(this.currTheme == "expertusoneV2"){
		 		changeDialogPopUI();
		 }
	    $("#mark-complete-wizard").show();
	    if($('#instructor-markcomplete-dialog')!=undefined){
	    	$('#instructor-markcomplete-dialog #ui-dialog-title-mark-complete-wizard').attr('title',Drupal.t('MSG404')+' - '+unescape(classTitle));
	    	$('#instructor-markcomplete-dialog #ui-dialog-title-mark-complete-wizard').addClass('vtip');
	    	vtip();
	    }
	    $('#mark-complete-wizard').css('min-height','50px');
	    $('.ui-dialog-titlebar-close').click(function(){
	        $("#mark-complete-wizard").remove();
	    });

	    $('#attendance_type_attended').click(function(){
	    	$('.markcompletion-attendance-fields').show();
	    });
	    $('#attendance_type_noshow').click(function(){
	    	$('.markcompletion-attendance-fields').hide();
	    	//$('#instructor-markcomplete-dialog div:first').css('height','160px');
	    });

	    $('#appScore').keypress(function(evt) {
			var charCode = (evt.which) ? evt.which : event.keyCode;
			if (charCode > 31 && (charCode < 48 || charCode > 57)){
				return false;
			}
			return true;
	    });

	    //For set min date to disable past session date in ILT/VC Class. ticket: 0022513
		var sessStart = $('#startSessDate'+classId).text().split(" ");
		sessStart = sessStart[0].split("-");
		sessStart = sessStart[1]+'-'+sessStart[2]+'-'+sessStart[0];

	    $('#comp_date').datepicker({
			  duration: '',
			  showTime: false,
			  constrainInput: false,
			  stepMinutes: 5,
			  stepHours: 1,
			  time24h: true,
			  minDate:sessStart,
			  dateFormat: "mm-dd-yy",
			  buttonImage: themepath+'/expertusone-internals/images/calendar_icon.JPG',
			  buttonImageOnly: true,
			  buttonText:Drupal.t('LBL765'),
			  firstDay: 0,
			  showOn: 'both',
			  showButtonPanel: true
		});
	    //$("#comp_date").val($("#mark-complete-date").html());
	    $("#comp_date").val($('#endSessDate'+classId).text()); //For set last session date in Completion date ticket: 0023408
		}catch(e){
	   		 // to do
	   	}
	},
	/*
	 * To Mark complete the class registration(Learning) in the manager dash board
	 */
	learnerMarkcompleteLearning: function(userId,enrolledId,classTitle,learningType,classId,ClassStatus){
		try{
		var lnrId = this.getLearnerId();
		if(lnrId == "0" || lnrId == "")
		{
			self.location='?q=learning/enrollment-search';
			return;
		}
		var html = "<div id='paintClassLearnersList"+userId+"'></div>";
        $("#paintClassLearnersList"+userId).remove();
        $("#gview_paintClassLearnersList"+userId).prepend(html);
		var obj = this;
		this.createLoader("mark-complete-wizard");
		$("#loaderdivmark-complete-wizard").css({"width":"450","height":"100"});
		$("#loaderdivmark-complete-wizard td").css({"width":"450","height":"100"});
		$('#mark-complete-wizard td').css('border-bottom','none');
		var attendance_type = $('input[name=attendance_type]:checked').val();
		var status = '';
		url = obj.constructUrl("learning/instructor/learner-markcomplete/"+learningType+"/"+enrolledId+"/"+classId+"/"+userId+"/"+$("#comp_date").val()+"/"+$("#appScore").val()+"/"+attendance_type);
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM			
			success: function(result){
				result = $.trim(result);
			    results = result.split("|");
				resDate = results[0];
				result = results[1];
				username = results[2];
				if(result=='success'){
				//console.log(attendance_type);
				if(attendance_type == 'noshow'){
				 status = Drupal.t('No Show');
				}
				else{
			    status = Drupal.t('Completed');
				}
				 resultMsg = Drupal.t('MSG764', {'@user' : username , '@status' : status , '@title' : unescape(classTitle) });
				
				}else if(result == 'nochange'){
					resultMsg	= Drupal.t('MSG748');
				}else{
					resultMsg	= SMARTPORTAL.t('Sorry, Problem in mark complete the Class \''+classTitle+'\'.');
				}
				obj.learnerMarkCompleteMsg(resultMsg,userId,classId,result,resDate,ClassStatus);
			}
	    });
		}catch(e){
	   		 // to do
	   	}
	},
	/*
	 * To show the status of mark complete class in a pop-up
	 */
	learnerMarkCompleteMsg : function(data,userId,classId,result,resDate,ClassStatus){
		try{
	    var html = '';
	    html+='<div id="instructor-mark-learning-result" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+='<tr><td height="30"></td></tr>';
	    html+='<tr>';
	    html+= '<td align="center"><i>'+data+'</i></td>';
	    html+='</tr>';
	    html+='</table>';
	    html+='</div>';
	    $('#mark-complete-wizard').html(html);
	    //$('.ui-dialog-buttonset').html('');
	    $("#instructor-mark-learning-result").show();
	    if(result=='success' || result == 'nochange'){
	    	// Remove the current learner content
	      var data = eval($("#instructor-result-container #enroll-main-action-"+classId).parents("tr").find("a").attr("data"));
	      if ($("#pager-ins").is(":visible") || data.is_last_rec != 'last') {
	    	  $("#instructor-result-container #enroll-main-action-"+classId).parents("tr").children("td").css("border-bottom","solid 1px #ccc");
	      }
	       // Commenting the below line for 0081359: Classes disappear from My classes widget after after cancelling the enrollments
			//  $("#instructor-result-container #enroll-main-action-"+classId).parents("tr").next("tr").remove();
			// Reload the learner view
	    	$("#instructor-tab-inner").data("instructordesk").viewLearners(classId, "#instructor-result-container #enroll-main-action-"+classId,ClassStatus);

	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').css("display","none");
	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(".admin-save-button-left-bg").css("display","none");
	    	$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(".admin-save-button-right-bg").css("display","none");
	    	$("#complete-confirmation-wizard").remove();
		}
		}catch(e){
	   		 // to do
	   	}
	},
	instructorShowMoreAction : function(obj) {
		try{
		var position = $(obj).position();
		var posTop   = Math.round(position.top);
		$('.enroll-drop-down-list').toggle();
		$('.enroll-action').hide();
		if(this.currTheme == "expertusoneV2"){
			$(obj).parent().siblings('.enroll-action').css('position','');
			$(obj).parent().siblings('.enroll-action').css('margin-top',"19px");
			$(obj).parent().siblings('.arrived-cancel-link').css('position','absolute');
			$(obj).parent().siblings('.arrived-cancel-link').css('margin-top',"26px");
			$(obj).parent().siblings('.arrived-cancel-link').css('margin-left',"-22px");
			$(obj).parent().siblings('.enroll-action').slideToggle();
//			if(Drupal.settings.mylearning_right===false){
//
//			}else{
//				$(obj).siblings('.enroll-action').closest('tr').find('.enroll-main-list').css({
//				      "position":"absolute","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').prevAll('tr').find('.enroll-main-list').css({
//				      "position":"absolute","z-index":"1000"
//				    });
//				$(obj).siblings('.enroll-action').closest('tr').nextAll('tr').find('.enroll-main-list').css({
//				      "position":"relative","z-index":"0"
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
		}else{
		$(obj).parent().siblings('.enroll-action').css('position','absolute');
		$(obj).parent().siblings('.enroll-action').css('right','17px');
		$(obj).parent().siblings('.enroll-action.arrived-cancel-link').css('right','32px');
		$(obj).parent().siblings('.enroll-action').css('top',posTop+20);
		$(obj).parent().siblings('.enroll-action').slideToggle("");
		}
		}catch(e){
	   		 // to do
	   	}
	},

	/*
	 * Call back function after rendering class details.
	 */
	callbackMyLearnerLoader: function(response, postdata, formid){
		try{
			if(response.class_status == 'completed'){
				var notCompCount= 0;
				var i;
				// Not loading in Completed tab without Enrollments #0038102
				if(typeof response.rows != 'undefined'){
					for(i=0;i<response.rows.length;i++)
					{
					  // Issue is Fixed For Enroll , inprogress and attended status only shows Select all CheckBox
					  if(response.rows[i].cell.CompStatus == 'lrn_crs_cmp_enr' || response.rows[i].cell.CompStatus == 'lrn_crs_cmp_inp' || response.rows[i].cell.CompStatus == 'lrn_crs_cmp_att'){
							 notCompCount= notCompCount +1;
					  }
					}
				}
				if(notCompCount<=0){
					$("#jqgh_paintClassLearnersList"+response.classId+"_MultiselectCheckBox").hide();
					$("#gbox_paintClassLearnersList"+response.classId+" .paint-mark-completion").hide();
					$("#multiselect-selectall-"+response.classId).hide();
					$("#multiselect-selectall-"+response.classId).attr("disabled", true);
					//$("#multiselect-selectall-"+response.classId).removeAttr("onclick");
				}else{
					$("#jqgh_paintClassLearnersList"+response.classId+"_MultiselectCheckBox").show();
					$("#gbox_paintClassLearnersList"+response.classId+" .paint-mark-completion").show();
					$("#multiselect-selectall-"+response.classId).show();
					$("#multiselect-selectall-"+response.classId).attr("disabled", false);
					//$("#multiselect-selectall-"+response.classId).removeAttr("onclick");
				}
			}
		$("#paintClassLearnersList_loader"+response.classId).removeClass('paintClassLearnersList-loader-height');
		//$("#paintClassLearnersList_loader"+response.classId).removeClass('loaderimg');

		$("#gview_paintClassLearnersList"+response.classId+" > .ui-jqgrid-hdiv").css("display","block");
		$("#gview_paintClassLearnersList"+response.classId+" > .ui-th-column").css("text-align","right");
		$("#refresh_paintClassLearnersList"+response.classId).css("display","none");
		$("#paintClassLearnersList"+response.classId+" .ui-row-ltr:odd td").css("background-color","#F7F7F7");

		// Update the enrolled count for the class
		if(response.enrolledcount!='' && response.enrolledcount!=null){
			$('#seats_enrolled_'+response.classId).html(response.enrolledcount);
		}


                //Update the seats 
		$('#selseatenrolledincompleted_'+response.classId).html(response.enrolledcount+'/'+response.maxCapacity);
		if(response.enrolledcount>0)
		var completedPercentage = ((response.enrolledcount * 100)/response.maxCapacity)/100;
		else
		var completedPercentage = 0;
		
		var elementId = 'progress_' + response.classId;
		/*Viswanathan added for #78521 */
		var existing_percentage=$('#'+elementId+' '+'.progressbar-text').text();
		var completedPercentage_sym = Math.round(completedPercentage * 100);
		if(parseInt(existing_percentage) != completedPercentage_sym){
		$('#'+elementId).removeClass('progress-updated');
		$('#'+elementId).html('');
		//$("#instructor-tab-inner").data("instructordesk").progressbarmc(response.classId, completedPercentage);
		progressBarRound(response.classId, completedPercentage, '','progress_',this);
		}

		var recordCount = $('#paintClassLearnersList'+response.classId).jqGrid('getGridParam', 'records');
		if(recordCount > 0){
			$('#seat_details_'+response.classId).show();
			$('#seat_details_'+response.classId).html(response.seatDetails);
			//$("#gview_paintClassLearnersList"+response.classId).find('.paint-mark-completion').css('display','block');
		}
		if(recordCount > 5){
			$('#pager2_user'+response.classId).show();

			//$('#seat_details_'+response.classId).css('margin-top','-24px');

			$("#pager2_user"+response.classId+"_center .ui-pg-table td").css("border-bottom","0px none").eq(-1).hide();
			$("#pager2_user"+response.classId+"_left").hide();
			$("#pager2_user"+response.classId+"_right").hide();
			$("#pager2_user"+response.classId+"_center").css({"text-align":"right"}).children(".ui-pg-table").css("float","right");
			//$("#pager2_user"+response.classId+"_center").find(".ui-icon-seek-prev, .ui-icon-seek-next").parent().removeClass("ui-state-disabled");
			//$("#first_pager2_user"+response.classId).add("#last_pager2_user"+response.classId).hide();
			$("#last_pager2_user"+response.classId).addClass("pager-last");
			$("#first_pager2_user"+response.classId).addClass("pager-first");
			$("#next_pager2_user"+response.classId).addClass("pager-next");
			$("#prev_pager2_user"+response.classId).addClass("pager-prev");
		}
		var recs = parseInt($("#paintClassLearnersList"+response.classId).getGridParam("records"),10);
        if (recordCount == 0) {
        	$("#paintClassLearnersList"+response.classId). css('display','block');
            //var html = $("#instructor-tab-inner").data("instructordesk").localeArray['err_no_learner'];
            var html =  Drupal.t("ERR074");
            $("#paintClassLearnersList"+response.classId).html('<tr><td class="border-style-none" colspan="6"><span class="myclass-no-records-cls">'+html+'</span></td></tr>');
            $('#paintClassLearnersList'+response.classId).css('text-align','center');

            // Border issue
            $('.border-style-none').css('border','0');
            
            //space maintain
           if( $(".myclass-no-records-cls").is(":visible")){
           	   if(response.delivery_type == 'lrn_cls_dty_vcl'){
        		   $("#paintClassLearnersList"+response.classId).find(".myclass-no-records-cls").closest(".instructor-learner-tab .ui-jqgrid").css("margin-bottom","2px");
        }
        	   
        	   if(response.delivery_type == 'lrn_cls_dty_ilt'){
        		   $("#paintClassLearnersList"+response.classId).find(".myclass-no-records-cls").closest(".instructor-learner-tab .ui-jqgrid").css("margin-bottom","8px");   
        	   }
        	   $("#paintClassLearnersList"+response.classId).find(".myclass-no-records-cls").closest("tr td").css("padding-left","0");
        	  // .page-learning-enrollment-search #page-container #paintInstructorResults tr.jqgrow:last-child td .cus-accord.instructor-learner-tab .ui-jqgrid-btable td	
            }
            
        }
        $("#paintClassLearnersList"+response.classId+" tr:nth-child(odd)").addClass("jqgrow-even");
        $('#paintClassLearnersList'+response.classId+' .jqgfirstrow > td').css("border-bottom","none");
		$("#instructor-tab-inner").data("instructordesk").destroyLoader('paintClassLearnersList_loader'+response.classId);

        if(recordCount > 5){
	        $('#pager2_user'+response.classId).appendTo("#gview_paintClassLearnersList"+response.classId);
	        $('#pager2_user'+response.classId+'_center').css('border-bottom','none');
		}else if(recordCount > 0 && recordCount < 6){
	// console.log("less themn 6");
			$("#cus-accord-"+response.classId).find('#gbox_paintClassLearnersList'+response.classId).next('#seat_details_'+response.classId).css({"margin-top":"-1px","padding-bottom":"0"});	
		}
        if(recordCount > 0 && $('#paintClassLearnersList'+response.classId+' .myclass-no-records-cls').html()) {
        	//$('#paintClassLearnersList12 .myclass-no-records-cls').parents('tbody').children('tr:first').remove();
        	$('#paintClassLearnersList'+response.classId+' .myclass-no-records-cls').html(' ');
        	$('#paintClassLearnersList'+response.classId+' .myclass-no-records-cls').css('color','#FFFFFF');
        	$('#paintClassLearnersList'+response.classId+' .myclass-no-records-cls').css('height','0px');

        }
        this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	if(this.currTheme == "expertusoneV2"){
        $('.cus-accord').find('.ui-jqgrid-bdiv').css('border','1px solid #e5e5e5');
	 	}else{
	 		$("#ins-class-accodion-"+response.classId).parents("tr").find("td").css("border-bottom","none");
	 		$('.cus-accord').find('.ui-jqgrid-bdiv').css('border','1px solid #cccccc');
	 	}
		//Vtip-Display toolt tip in mouse over
		 vtip();

		 var uniqueId = response.classId;
		 $("#instructor-tab-inner").data("instructordesk").retainCheckboxSelected(uniqueId);
		// Select all check and unselect all check funtionality
			$('#multiselect-selectall-'+uniqueId).click(function(event){
				event.stopPropagation();
				var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
				if(existingList != '' || existingList != null || existingList != undefined)
				var existingListArray = existingList.split(',');
				if($(this).attr('checked') == true){
					$('#paintClassLearnersList'+uniqueId+' tr').each(function(){
						var selectedId = $(this).attr('id');
						var selectedIds = $('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).val();

						if(selectedIds != undefined && selectedIds != '' && $.inArray(selectedIds, existingListArray) == -1 && $('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('disabled') != true){
							existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
							if(existingList == ''){
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedIds);
							} else {
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedIds);
							}
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('checked', true);
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().removeClass('checkbox-unselected');
							$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().addClass('checkbox-selected');
						}
					});
				} else {
					var removeList = new Array();
					var c = 0;
					$('#paintClassLearnersList'+uniqueId+' tr').each(function(){
						var selectedId = $(this).attr('id');
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('checked', false);
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().removeClass('checkbox-selected');
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().addClass('checkbox-unselected');
						removeList[c] = selectedId;
						c++;
					});

					$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
					/*
					for(var i=0; i< existingListArray.length; i++){
						var selectedId = existingListArray[i];
						//alert('In array : ' + $.inArray(selectedId, removeList));
						if($.inArray(selectedId, removeList) == -1 ){
							existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
							//alert('existingList '+existingList);
							if(existingList == ''){
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
							} else {
								$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
							}
						}
					} */
				}

				// If all checkbox selected - multiselect-all should be checked
				//$("#instructor-tab-inner").data("instructordesk").retainSelectAllCheckbox(uniqueId);

			});

			// Alternate option for multi-select to align right
			$('#paintClassLearnersList'+uniqueId+' .multiselect-singlecheck').click(function(){

				var selectedId = $(this).val();
				var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
				var existingListArray = existingList.split(',');

				if($(this).attr('checked') == true){
					if(selectedId != '' && $.inArray(selectedId, existingListArray) == -1 ){
						if(existingList == ''){
							$('input[name="hidden_idlist_'+uniqueId+'"]').val(selectedId);
						} else {
							$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +selectedId);
						}
					}
				} else {
					if(existingList != ''){
						var existingListArray = existingList.split(',');
						var existingListLen = existingListArray.length;
						$('input[name="hidden_idlist_'+uniqueId+'"]').val('');
						for( var i = 0; i < existingListLen; i++) {
							if(selectedId != existingListArray[i]){
								existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
								if(existingList == ''){
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingListArray[i]);
								} else {
									$('input[name="hidden_idlist_'+uniqueId+'"]').val(existingList + ',' +existingListArray[i]);
								}
							}
						}
					}
				}
				// If all checkbox selected - multiselect-all should be checked
				$("#instructor-tab-inner").data("instructordesk").retainSelectAllCheckbox(uniqueId);
			});
		$('.limit-title-ins').trunk8(trunk8.ins_title);
		//$('.limit-desc-ins').trunk8(trunk8.ins_desc);
		
		//reduce the padding in last table row
		if($('#paintInstructorResults').find('tr:first').parent().children().last().hasClass("cus-accord noborder")){
			var lastId = $('#paintInstructorResults').find('tr:first').parent().children().last().attr('id');
			$("#"+lastId).find('.seat-details').css('padding-bottom',"3px");
		}
		
		}catch(e){
	   		 // to do
	   	}
	},
	retainCheckboxSelected : function(uniqueId){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var existingList = $('input[name="hidden_idlist_'+uniqueId+'"]').val();
		if(existingList != '' && existingList != undefined){
			var existingListArray = existingList.split(',');
			var existingListLen = existingListArray.length;
			var selectedId = '';
			for( var i = 0; i < existingListLen; i++) {
				selectedIdold = existingListArray[i];
				var index = selectedIdold.lastIndexOf("_");
				var selectedId = selectedIdold.substr(index+1);
				if($('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).val() != 'undefined'){
					$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).attr('checked', true);
					//if(this.currTheme == "expertusoneV2"){
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().removeClass('checkbox-unselected');
						$('#multiselect-singlecheck-'+uniqueId+'_'+selectedId).parent().addClass('checkbox-selected');
					//}

				}
			}
		}
		// If all checkbox selected - multiselect-all should be checked
		$("#instructor-tab-inner").data("instructordesk").retainSelectAllCheckbox(uniqueId);
		}catch(e){
			// to do
		}
	},
	retainSelectAllCheckbox : function(uniqueId){
		try{
		// If all checkbox selected - multiselect-all should be checked
		var multiselectVar = true;
		var removeClass = "checkbox-unselected";
		var addClass = "checkbox-selected";
		$('#paintClassLearnersList'+uniqueId+' .multiselect-singlecheck').each(function(){
			if($(this).attr('checked') == false && !$(this).is(':disabled')){
				multiselectVar = false;
				removeClass = "checkbox-selected";
				addClass = "checkbox-unselected";
		    }
		});
		$('#multiselect-selectall-'+uniqueId).attr('checked', multiselectVar);
		$('#multiselect-selectall-'+uniqueId).parent().removeClass(removeClass);
		$('#multiselect-selectall-'+uniqueId).parent().addClass(addClass);
		}catch(e){
	   		 // to do
	   	}
	}

});

$.extend($.ui.instructordesk.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
	$( "#instructor-tab-inner" ).instructordesk();
	if(document.getElementById('instructor-tab-inner')) {
		$('#first_pager-ins').click( function(e) {
			if(!$('#first_pager-ins').hasClass('ui-state-disabled')) {
				$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			}
		});

		$('#prev_pager-ins').click( function(e) {
			if(!$('#prev_pager-ins').hasClass('ui-state-disabled')) {
				$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			}
		});

		$('#next_pager-ins').click( function(e) {
			if(!$('#next_pager-ins').hasClass('ui-state-disabled')) {
				$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			}
		});

		$('#last_pager-ins').click( function(e) {
			if(!$('#last_pager-ins').hasClass('ui-state-disabled')) {
				$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			}
		});

		$('#pager-ins .ui-pg-selbox').bind('change',function() {
			$('#gview_paintInstructorResults').css('min-height','auto');
			$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			$("#instructor-tab-inner").data("instructordesk").hidePageControls(false);
		});

		$("#pager-ins .ui-pg-input").keyup(function(event){
			if(event.keyCode == 13){
				$('#gview_paintInstructorResults').css('min-height','auto');
				$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
			}
		});

		//select record count in veiw per page
		if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
			$('#instructor-result-container .page-show-prev').bind('click',function() {
				if(parseInt($("#pg_pager-ins .page_count_view").attr('id')) < 0){
					$("#pg_pager-ins .page_count_view").attr('id','0');
				}else{
					$('#gview_paintInstructorResults').css('min-height','100px');
					$("#instructor-tab-inner").data("instructordesk").hidePageControls(false);
					$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
				}
			});

			$('#instructor-result-container .page-show-next').bind('click',function() {
				if(parseInt($("#pg_pager-ins .page_count_view").attr('id')) >= parseInt($("#pg_pager-ins .page-total-view").attr('id'))){
					$("#pg_pager-ins .page_count_view").attr('id',($("#pg_pager-ins .page_count_view").attr('id')-1));
				}else{

					$('#gview_paintInstructorResults').css('min-height','100px');
					$("#instructor-tab-inner").data("instructordesk").hidePageControls(false);
					$("#instructor-tab-inner").data("instructordesk").createLoader('instructor-result-container');
				}
			});

	      }
	}
	}catch(e){
  		 // to do
  	}
});

function refresh(new_window){
	try{
		var new_window = new_window;
		if(!new_window.closed){
		setTimeout(refresh,10000,new_window);
		}else{
			session();
	}
	}catch(e){
		
	}
}
function sessionend(a,time){
	try{
		setTimeout(session,time);
	}catch(e){
		
	}
}

function session(){
	var tab = $("#instructor-tab-inner").data("instructordesk").instructortab;
	$("#paintInstructorResults").setGridParam().trigger("reloadGrid");
}
