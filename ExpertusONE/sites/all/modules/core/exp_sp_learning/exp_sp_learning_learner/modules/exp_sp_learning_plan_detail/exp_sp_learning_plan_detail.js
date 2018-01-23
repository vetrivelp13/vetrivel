(function($) {
var crstitle = '';
$.widget("ui.lnrplandetails", {
	lastClassIdAndSessionId: -1,
	_init: function() {
		try{
			this.learningActionItem();
			this.renderCoursesList();
			this.prevObj;
			//this.user_id = this.getLearnerId();
		}catch(e){
			 // to do
		 }
	},
	

	displayClassDetailField: function(fieldName, value) {
		try{
		if (value == null || value == '') {
			$('#' + fieldName).html('-');
		}
		else {
			$('#' + fieldName).html(value);
		}
		}catch(e){
			 // to do
		 }
	},


	displayClassDetailLocationField: function(locFieldDiv, locFieldName, value) {
		try{
		if (value == null || value == '') {
			$('#' + locFieldDiv).hide();
		}
		else {
			$('#' + locFieldName).html(value);
			$('#' + locFieldDiv).show();
		}
		}catch(e){
			 // to do
		 }
	},
	
	displayClassDetails: function(CourseId,classIdAndSessionId, catdiv) {
		try{
		var obj = this;
		var objStr = '$("#learning-plan-details-display-content").data("lnrplandetails")'; 
		var url = this.constructUrl("learning/learning-class-details/" + classIdAndSessionId);
		//if($('#'+CourseId+'-SubGrid').html().length < 55){
			this.createLoader('class-details-displayloader-'+CourseId);
			var rtnResult;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				dataType: "html",
				success: function(result) {
					obj.destroyLoader('class-details-displayloader-'+CourseId);
					var str='';
					str +="<td colspan='7' class='course-class-subgrid'>"+result+"</td>";
					$("#"+classIdAndSessionId+"SubGrid").html(str);
					//catdiv.html(result);
					//rtnResult = result;
				}
		    });
		//}
		//return rtnResult;
		}catch(e){
			 // to do
		 }
	},
	
	paintName: function(cellvalue, options, rowObject) {
		try{
		var obj = options.colModel.widgetObj;
		var html		= '';

		if (rowObject['name'] == null || rowObject['name'] == '') {
			html += '-';
		}
		else {
			var CourseId = rowObject['CourseId'];
			var ClassId = rowObject['ClassId'];
			var SessId  = rowObject['SessonId']; 
 			var data1 = "data={'ClsId':'"+ClassId+"','SessId':'"+SessId+"'}";
			
 			var course_title =titleRestrictionFadeoutImage(rowObject['name'],'view-course-title');
 			//Embed widget related work (Create flexible grid)
 		 	if(Drupal.settings.widget.widgetCallback==true){
 		 		course_title =class_course_tp_title_controler_for_widget(rowObject['name']);
 			}
			html += '<div class="enroll-course-title">';
			html += '<span id="class_attachment_'+ClassId+'"></span>';
			html += '<a id="class-accodion-'+ClassId+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+obj+'.addAccordionToTable1(this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getCrsClassDetails,this,'+obj+','+CourseId+',true);\'>&nbsp;&nbsp;</a>';
			html += '<span id="titleAccEn_'+ClassId+'" class="item-title" ><span class="enroll-class-title vtip" title="'+rowObject['name']+'" href="javascript:void(0);"> '+course_title+'</span></span>';
			html += '</div>';
		}
		$("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-details-displayloader-'+CourseId);
 		return html;
		}catch(e){
			 // to do
		 }
	},
	addAccordionToTable1 : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,CourseId,isRemove) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableView(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,CourseId,isRemove);
/*		if(this.prevObj != undefined && this.prevObj != obj)
		{
			$(this.prevObj).parents('tr.ui-widget-content').children('td').css('border-top','none');
			//$(this.prevObj).parents('tr.ui-widget-content').children('td').css('padding-bottom','8px');
			//$(this.prevObj).parents('tr.ui-widget-content').children('td').children('.enroll-course-title').css('padding-left','6px');
			$(this.prevObj).parents('tr.ui-widget-content').children('td').css('border-bottom','solid 1px #ccc');
			$(this.prevObj).removeClass('title_open').addClass('title_close');			
		}else if(this.prevObj == obj){
			$(obj).parents('tr.ui-widget-content').children('td').css('border-top','none');
			//$(obj).parents('tr.ui-widget-content').children('td').css('padding-bottom','8px');
			//$(obj).parents('tr.ui-widget-content').children('td').children('.enroll-course-title').css('padding-left','6px');
			$(obj).parents('tr.ui-widget-content').children('td').css('border-bottom','solid 1px #cccccc');
			$(obj).removeClass('title_open').addClass('title_close');
			this.prevObj = '';
			return;
		}		
		this.prevObj = obj;
		$(obj).parents('tr.ui-widget-content').children('td').css('border-bottom','none');
		$(obj).parents('tr.ui-widget-content').children('td').css('padding-bottom','0px');
		//$(obj).parents('tr.ui-widget-content').children('td').children('.enroll-course-title').css('padding-left','2px');
		$(obj).parents('tr.ui-widget-content').children('td').css('border-top','solid 1px #cccccc');
		$(obj).parents('tr.ui-widget-content').next().children('td').css('border-bottom','solid 1px #cccccc');
		$(obj).parents('tr.ui-widget-content').next().children('td').css('padding-bottom','10px');
		$(obj).removeClass('title_close').addClass('title_open');*/
		}catch(e){
			 // to do
		 }
	},
	addAccordionToTableView : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,CourseId,isRemove) {
		try{
		var currTr = $(obj).parent().parent().parent().attr("id");
		var curAccor = currTr.split("-");
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr).after("<tr id='"+currTr+"SubGrid' class='learning-class-session-detail'><td colspan='7' class='course-class-subgrid'></td></tr>");
			$("#"+currTr+"SubGrid").show();//css("display","block");
			$("#class-accodion-"+curAccor[0]).removeClass("title_close");
			$("#class-accodion-"+curAccor[0]).addClass("title_open");
			//$("#ClassDetailsMainDiv").css('border-bottom','1px solid #cccccc');
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"SubGrid").slideDown(1000);
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
    			$("#class-accodion-"+curAccor[0]).removeClass("title_close");
				$("#class-accodion-"+curAccor[0]).addClass("title_open");
				$("#"+currTr).removeClass("ui-widget-content");
    		} else {
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$("#"+currTr+"SubGrid").slideUp(1000);
    			$("#class-accodion-"+curAccor[0]).removeClass("title_open");
				$("#class-accodion-"+curAccor[0]).addClass("title_close");
				$("#"+currTr).removeClass("ui-widget-content");
				$("#"+currTr).addClass("ui-widget-content");
    		}
		}
		//var curAccor = currTr.split("-");

		var data = eval($("#class-accodion-"+curAccor[0]).attr("data"));
		var Class_Id = data.ClsId;
		var Sess_Id  = data.SessId;
		var ClsSessId = Class_Id+'-'+((Sess_Id != 'null') ? (Sess_Id) : '');
		var classDetSec = this.displayClassDetails(CourseId,ClsSessId, obj);
		$('.dt-child-row-En > td').attr('colSpan','7');
		}catch(e){
			 // to do
		 }
	},
	addAccordionToTable2 : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove) {
		try{
		this.addAccordionToTable(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove);
		$('.dt-child-row-En > td').attr('colSpan','7');
		}catch(e){
			 // to do
		 }
	},
	
	getCrsClassDetails : function(catdiv,accordionLink,parentObj){
		try{
		parentObj = eval(parentObj); 
		var data= eval(accordionLink.attr("data")); 
		var Class_Id = data.ClsId; 
		var Sess_Id  = data.SessId;
		var ClsSessId = Class_Id+'-'+Sess_Id;
 		$('#openclose_enr_'+Class_Id).removeClass('title_open'); 
		$('#openclose_enr_'+Class_Id).addClass('title_close'); 
		var ostr = '';
        ostr += parentObj.displayClassDetails(CourseId,ClsSessId, catdiv);
		catdiv.css('display','block');
		$('.dt-child-row-En > td').attr('colSpan','7');
		}catch(e){
			 // to do
		 }
	},
/*	paintClassCode: function (cellvalue, options, rowObject) {
		if (rowObject['ClassCode'] == '' || rowObject['ClassCode'] == '-') {
			return '<span>&nbsp;</span>';
		}
		return rowObject['ClassCode']; 
	},
*/	paintDeliveryType: function(cellvalue, options, rowObject) {
	 try{
		var row_del_type;
		if (rowObject['type'] == '' || rowObject['type'] == '-') {
			return '<span>&nbsp;</span>';
		}
		
		if(rowObject['type'] == 'WBT'){
            row_del_type = Drupal.t("Web-based");
        }
        else if(rowObject['type'] == 'VC'){
            row_del_type = Drupal.t("Virtual Class");
        }
        else if(rowObject['type'] == 'ILT'){
            row_del_type = Drupal.t("Classroom");
        }
        else if(rowObject['type'] == 'VOD'){
            row_del_type = Drupal.t("Video");
        }
        return row_del_type;
	 }catch(e){
		 // to do
	 }
	},
	
	paintLocation: function(cellvalue, options, rowObject) {
		try{
		if (rowObject['location'] == '' || rowObject['location'] == '-') {
			return '<span>&nbsp;</span>';
		}
		
		return  rowObject['location'];
		}catch(e){
			 // to do
		 }
	},
	paintDate: function(cellvalue, options, rowObject) {
		try{
		if (rowObject['date'] == '' || rowObject['date'] == '-') {
			return '<span>&nbsp;</span>';
		}
		//Modified by Vincent on Oct 29, 2013 for #0028593
		return rowObject['date'];
		}catch(e){
			 // to do
		 }
	},

	paintStartDate: function(cellvalue, options, rowObject) {
		try{
		var custTime = '';
		if (rowObject['start'] == '' || rowObject['start'] == '-') {
			custTime = '<span>&nbsp;</span>';
		}
		else {
			//Modified by Vincent on Oct 29, 2013 for #0028593
			var format_time = rowObject['start'].split(" ");
			custTime = format_time[0] + ' ' + '<span class="time-slab">'+ format_time[1]+'</span>';
			custTime = '<span>'+ custTime +'</span>';
		}
		
		return custTime;
		}catch(e){
			 // to do
		 }
	},
	
	paintEndDate: function(cellvalue, options, rowObject) {
		try{
		var type = rowObject['type'];
		var custTime = '';
		if(type == 'ILT' || type == 'VC'){
			var CourseId = rowObject['CourseId'];
			var ClassId = rowObject['ClassId'];
			var SessId  = rowObject['SessonId'];
			
			var session_timezone = rowObject['session_timezone'];
			var user_timezone =  rowObject['user_timezone'];
			var session_code = rowObject['session_code'];
			var short_code = rowObject['short_code'];
			
			var format_time = rowObject['ilt_start'].split(" ");
			custTime = format_time[0] + ' ' + '<span class="time-slab">' + format_time[1] + '</span>';

	        var format_time_end = rowObject['ilt_end'].split(" ");
			custTimeend = format_time_end[0]+' '+'<span class="time-slab">'+format_time_end[1]+'</span>';
			
			
			var startDateForTitle= rowObject['ilt_date']+' '+custTime+' to '+custTimeend+' '+session_code+' '+short_code;
		var custTime = '';
		
		if (rowObject['end'] == '' || rowObject['end'] == '-') {
			custTime = '<span>&nbsp;</span>';
		}
		else if(rowObject['end'] != ''){
			//Modified by Vincent on Oct 29, 2013 for #0028593
			var format_time = rowObject['end'].split(" ");
			custTime = format_time[0]+' '+'<span class="time-slab">'+format_time[1]+'</span>';
			custTime = '<span>'+ custTime +'</span>';
		}
		if(type == 'ILT' && session_timezone != user_timezone ){
			custTime = '<span>'+ custTime +'</span>'+' '+qtip_popup_paint(ClassId,startDateForTitle); 	
		}
		}
		return custTime;
		}catch(e){
			 // to do
		 }
	},
	
/*	paintAction: function(cellvalue, options, rowObject) {
		return rowObject['action'];
	},
*/
	learningActionItem: function() {
		try{
		Drupal.attachBehaviors();
		$("body").data("learningcore").disableFiveStarOnVoting();
		var userId = this.getLearnerId();
		/*if(userId == "" || userId == "0"){
			
		    $('.prgdet-node .fivestar-click').removeClass('fivestar-click');	    
	        $('.prgdet-node .fivestar-widget').unbind('mouseover');
	        $('.prgdet-node .fivestar-widget .star a').unbind('click');
	        $('.prgdet-node .fivestar-widget .star a').css('cursor','auto');
		}
		
		var obj = this;
		this.createLoader('learning-plan-details-data');*/
		var progrmId = $("#programId").val();
		var objStr = '$("#learning-plan-details-display-content").data("lnrplandetails")'; 
		/*var url = this.constructUrl("learning/learning-action-item/" + progrmId);
		var rtnResult;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			dataType: "html",
			success: function(result) {
				obj.destroyLoader('learning-plan-details-data');
				$("#learning-plan-action").html(result);*/
				//after login auto register or add to cart related work start
				var lnrplanobj = $("#learning-plan-details-display-content").data("lnrplandetails");
				var user_id =lnrplanobj.getLearnerId();
				//user_id = lnrplanobj.user_id;
				//console.log(user_id);
				if(user_id != 0 && user_id !='' && user_id != null && user_id != undefined){
					var user_selected_class_id = $.cookie("user_selected_class_id");
					if(user_selected_class_id != null && user_selected_class_id !=undefined){
						 var onclickprop = $('#'+user_selected_class_id).attr("onclick");
						 if(onclickprop != null &&  onclickprop !=''  && onclickprop!=undefined)
							 $('#'+user_selected_class_id).click();
						 else
							 $("body").data("learningcore").callMessageWindow(Drupal.t('LBL721'),Drupal.t('MSG430'));
						 $.cookie("user_selected_class_id",'',{expires: -300});
						 $.cookie("user_selected_url", '',{ expires: -300 });
					}
					var user_selected_page_number = $.cookie("user_selected_page_number");
					if(user_selected_page_number != null && user_selected_page_number !=undefined)
						 $.cookie("user_selected_page_number",'',{expires: -300});
					var user_selected_row_number = $.cookie("user_selected_row_number");
					if(user_selected_row_number != null && user_selected_row_number !=undefined)
						$.cookie("user_selected_row_number",'',{expires: -300});
				}
				else{
					$('#signin').click();
				}
				//after login auto register or add to cart related work end
			/*}
	    });*/
//				75333: Added discussion is not shown under TP details page.				
//			    if(availableFunctionalities.exp_sp_forum){
//					/* call to function in forum js to render forum topics */
//					$('#forum-topic-list-display').data('forumlistdisplay').renderForumTopicResults($("#tdataTpId").val());
//				}
		}catch(e){
			 // to do
		 }
	},
	
	renderGroupList: function(moduletitle) {
		try{
		if($('.learning-module-row-'+moduletitle).length > 0){
	 		$('.learning-module-row-'+moduletitle).toggle();
	 	}else{
	 		$('.learning-module-subrow-').toggle();
	 	}
		//Accordion effect
		var modId = $('#lp-learning-module-accordion-'+moduletitle);
 		var clsName = $('#lp-learning-module-accordion-'+moduletitle).attr('class');		 
		if(clsName=='title_close'){
			modId.removeClass('title_close');
			modId.addClass('title_open');
			
		}else if(clsName=='title_open') {
			modId.removeClass('title_open');
			modId.addClass('title_close');
		}
		}catch(e){
			 // to do
		 }
	},
	
	
	renderCourseList: function(grouptitle) {
		try{ 
		$('.learning-module-subrow-'+grouptitle).toggle();
		//Accordion effect
		var modId = $('#learning-module-accordion-'+grouptitle);
		var clsName = $('#learning-module-accordion-'+grouptitle).attr('class');
		if(clsName=='title_close'){
			modId.removeClass('title_close');
			modId.addClass('title_open');
		}else if(clsName=='title_open') {
			modId.removeClass('title_open');
			modId.addClass('title_close');
		}
		}catch(e){
			 // to do
		 }
	},
	
	renderCoursesList: function(reload) {
		try {
			var obj = this;
			this.createLoader('course-list-loaderimg');
			var gridWidth = '100%';
			var objStr = '$("#learning-plan-details-display-content").data("lnrplandetails")';
			var programId = $('#programId').val();
			if(typeof reload != 'undefined' && reload == true) {
				$('#paint-courses-list').trigger("reloadGrid",[{page:1}]);
			}
			var url = this.constructUrl("learning/learning-detailed-courses-list/" + programId);
			//Embed widget related work (flexiple grid related work)
			if(Drupal.settings.widget.widgetCallback==true){
				$('#paint-courses-list').jqGrid({ 
					url: url,
					datatype: "json", 
					mtype: 'GET',
					colNames: [''],
					colModel: [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}],
					rowNum:10, // A large number as we want to show all classes
					rowList:[10,20,30], 
					viewrecords: true, 
					emptyrecords: "",
					sortorder: "asc", 
					toppager:false,
					height: 'auto',
					headertitles:true,
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",
					loadui:false,
					loadComplete:obj.coursecallbackLoader
				});
			}else{
				$('#paint-courses-list').jqGrid({ 
					url: url,
					datatype: "json", 
					mtype: 'GET',
					colNames: [''],
					colModel: [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}],
					rowNum:10, // A large number as we want to show all classes
					rowList:[10,20,30], 
					viewrecords: true, 
					emptyrecords: "",
					sortorder: "asc", 
					toppager:false,
					height: 'auto',
					autowidth: true,
	                shrinkToFit: true,
					loadtext: "",
					recordtext: "",
					loadui:false,
					loadComplete:obj.coursecallbackLoader
				}); //.navGrid('#pager', {add:false,edit:false,del:false,search:false,refreshtitle:true});
			}
			
			if(availableFunctionalities.exp_sp_forum){
			 	/* call to function in forum.js to render the forum topics */
				var contId = $("#tdataTpId").val();
				var frm = "TP";
				var newObj = {id:contId,src:frm};
			 	$('#forum-topic-list-display').data('forumlistdisplay').renderForumTopicResults(newObj);
		 	 }
			
		} catch(e) {
			// to do

		}
	},
	
	coursecallbackLoader : function(response, postdata, formid) {
		try {
			// New UI Code
			$("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('course-list-loaderimg');
			  $("#gview_paint-courses-list .ui-jqgrid-hdiv").hide();
			  $('#paint-courses-list .lmt-lrp-crs-title').trunk8(trunk8.class_detail_title);
			  $('#paint-courses-list .lmt-lrp-crs-desc ').trunk8(trunk8.lp_detail_course_desc);
			  $('.lmt-cls-desc-add').trunk8(trunk8.class_detail_add_desc);
			  if(Drupal.settings.widget.widgetCallback==true){
			   resetFadeOutByClass('.learningplan-content-wrapper','content-detail-container','line-item-container','course_details');
			  }
			  // Show more section starts here
			  var recs = response['records'];
			  var showMore = $('#paintlrpcrslist-show_more');
			  $("#paint-courses-list").data('totalrecords', recs);
			  if($('#paint-courses-list').getGridParam("reccount") < recs) {
				  showMore.show();
			  } else {
				  showMore.hide();
			  }       
		      $("#paint-classes-list").find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');	
			  if (response.group_ids.length) { // show one group title
		    	  $(response.group_ids).each(function(i,val){
		    		  if ($('#lp_grp_title_'+val).length) {
		    			  $('.lp_grp_title_'+val).hide();
		    			  $('#lp_grp_title_'+val).show();
		    		  }
		    	  })
		      }
			  Drupal.attachBehaviors();
			  $("body").data("learningcore").disableFiveStarOnVoting();
			  vtip();
			  $("#searchLrnplanCourseListPaint").showmore({
			  	// showAlways: true,
			  	'grid': "#paint-courses-list",
			  	'gridWrapper': '#searchLrnplanCourseListPaint',
			  	'showMore': '#paintlrpcrslist-show_more'
			  });
		} catch(e) {
			// to do
		}
	},
	
	renderClassesList: function(courseId, programId) {
		try{
		crstitle = courseId;
		if($("#paint-classes-list-"+crstitle).html()==''){
			this.createLoader('class-list-loaderimg-'+crstitle);
		}
		
		if ($("#paint-classes-list-"+crstitle).html() != "") {
			$("#learning-course-subrow-"+crstitle).slideDown();
			$("#course_content_less_"+crstitle).show();
			$("#course_content_more_"+crstitle).hide();
		}
		//$('#learning-course-subrow-'+courseId).toggle();
		//Accordion effect
		var crsId = $('#learning-course-accordion-'+courseId);
		var crsName = $('#learning-course-accordion-'+courseId).attr('class');
		if(crsName=='title_close'){
			crsId.removeClass('title_close');
			crsId.addClass('title_open');
		}else if(crsName=='title_open') {
			crsId.removeClass('title_open');
			crsId.addClass('title_close');
		}
		var obj = this;
		var gridWidth = '100%';
		var objStr = '$("#learning-plan-details-display-content").data("lnrplandetails")';
		//var url = this.constructUrl("learning/learning-classes-list/" + courseId);
		var url = this.constructUrl("learning/learning-detailed-classes-list/" + courseId);
		//Embed widget related work (flexiple grid related work)
		if(Drupal.settings.widget.widgetCallback==true){
			$('#paint-classes-list-'+courseId).jqGrid({ 
				url: url,
				postData:{programId : programId},
				datatype: "json", 
				mtype: 'GET',
				/*colNames:[Drupal.t('Class')+' '+Drupal.t('LBL107'), Drupal.t('LBL036'), Drupal.t('LBL042') , Drupal.t('LBL251'), Drupal.t('LBL252') , Drupal.t('Location')],
				colModel:[
				           {name: Drupal.t('Class Name'), index:'class.title', width:250,'align':'left', title:false,'widgetObj':objStr, formatter:obj.paintName},
				           //{name: SMARTPORTAL.t('Class Code'), index:'ClassCode', width:110,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintClassCode},
				           {name: Drupal.t('LBL036'), index:'DeliveryType', width:120,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintDeliveryType},
				           {name: Drupal.t('Date'), index:'start_date', width:90,'align':'left','sorttype':'text',title:false,'widgetObj':objStr, formatter:obj.paintDate},
				           {name: Drupal.t('Start'), index:'StartDate', width:90,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintStartDate}, 
				           {name: Drupal.t('End'), index:'EndDate', width:90,'align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintEndDate},
				           {name: Drupal.t('Location'), index:'Location', width:140,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintLocation}],*/
				colNames: [''],
				colModel: [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}],
				rowNum:10, // A large number as we want to show all classes
				rowList:[10,20,30], 
				viewrecords: true, 
				emptyrecords: "",
				sortorder: "asc", 
				toppager:false,
				height: 'auto',
				headertitles:true,
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoader
			});
		}else{
			$('#paint-classes-list-'+courseId).jqGrid({
				url: url,
				postData:{programId : programId},
				datatype: "json", 
				mtype: 'GET',
				/*colNames:[Drupal.t('Class')+' '+Drupal.t('LBL107'), Drupal.t('LBL036'), Drupal.t('LBL042') , Drupal.t('LBL251'), Drupal.t('LBL252') , Drupal.t('Location')],
				colModel:[
				           {name: Drupal.t('Class Name'), index:'class.title', width:290,'align':'left', title:false,'widgetObj':objStr, formatter:obj.paintName},
				           //{name: SMARTPORTAL.t('Class Code'), index:'ClassCode', width:110,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintClassCode},
				           {name: Drupal.t('LBL036'), index:'DeliveryType', width:120,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintDeliveryType},
				           {name: Drupal.t('Date'), index:'start_date', width:90,'align':'left','sorttype':'text',title:false,'widgetObj':objStr, formatter:obj.paintDate},
				           {name: Drupal.t('Start'), index:'StartDate', width:90,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintStartDate}, 
				           {name: Drupal.t('End'), index:'EndDate', width:90,'align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintEndDate},
				           {name: Drupal.t('Location'), index:'Location', width:180,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintLocation}],*/
				colNames: [''],
				colModel: [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}],
				rowNum:10, // A large number as we want to show all classes
				rowList:[10,20,30], 
				viewrecords: true, 
				emptyrecords: "",
				sortorder: "asc", 
				toppager:false,
				height: 'auto',
				autowidth: true,
                shrinkToFit: true,
				loadtext: "",
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoader
			}); //.navGrid('#pager', {add:false,edit:false,del:false,search:false,refreshtitle:true});
		}
		$('.ui-jqgrid').css('margin','0px');
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
		    if($.browser.version >='8'){
		    	 $('.ui-widget-content').find('tr td:first-child').css("padding","0 8px 0 0");
		     }
		}
		
		$("#course-list-"+courseId+" .content-detail-container .content-description .fade-desc").find("#arrow-more").first().click();
		/*var isAtLeastIE11 = !!(navigator.userAgent.match(/Trident/) && !navigator.userAgent.match(/MSIE/))? true : false;
		if($.browser.msie && parseInt($.browser.version, 10)>='9' || isAtLeastIE11==1 ){
		     $('#paint-classes-list-'+crstitle).find('tr td:first-child').css("width","auto");
		     $('#gview_paint-classes-list-'+crstitle).find(".ui-jqgrid-htable").find('tr th:first-child').css("width","254px");
	    }*/
		//$('.ui-jqgrid-htable').css('display','block');
		}catch(e){
			 // to do
		 }
	},
	
	hideClassesList: function(courseId) {
		try {
			var courseId = (courseId != undefined) ? courseId : 0;
			if (courseId > 0) {
				$("#learning-course-subrow-"+courseId).slideUp();
				$("#course_content_more_"+courseId).show(300);
				$("#course_content_less_"+courseId).hide();
				$("#course-list-"+courseId+" .content-detail-container .content-description").find("#arrow-less").click();
			}
		} catch(e) {
			
		}
	},
	 
	callbackLoader : function(response, postdata, formid) {
		try{
		$("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-list-loaderimg-'+crstitle);
	   //Now, display class details of the first class in the displayed list.
	   crstitle = (response.course_id !== undefined) ? response.course_id : crstitle;
	   var dataIDs = $('#paint-classes-list-'+crstitle).jqGrid('getDataIDs');
	   if (dataIDs.length <= 0) {
		   $('#gview_paint-classes-list-'+crstitle).children(".ui-state-default").hide();
		   $('#paint-classes-list-'+crstitle).html('<tr class="no-item-found-row" ><td align="center">'+'<div class="no-records">'+Drupal.t("ERR073")+'</div>'+'</td></tr>');
		   $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-list-loaderimg-'+crstitle);
	   }
	   else {
		   var firstRowId = dataIDs[0];
		   $('#paint-classes-list-'+crstitle).jqGrid('setSelection',firstRowId);
		   if(Drupal.settings.convertion.mylearn_version==1){
			   $.each(response.rows, function (i, list){
				   if(list['class']['action_variables']['showprogress']===true || list['class']['action_variables']['showTpprogress'])
				   	progressBarRound(list['class']['action_variables']['enrolled_id'],list['class']['action_variables']['progress'], 'enr_progress','progress_',$("#lnr-catalog-search").data("contentPlayer"));
					//$("#lnr-catalog-search").data("contentPlayer").progressbardetail(list['class']['action_variables']['enrolled_id'],list['class']['action_variables']['progress'], 'enr_progress','progress_');					
				});
		   }
	   }
	   $("#paint-classes-list-"+crstitle).find("tr").each(function(){
		   $(this).removeClass("ui-state-highlight");
		});
	   
	   setTimeout(function(){ $("body").data("learningcore").disableFiveStarOnVoting(); }, 250);
	// New codes started here
	    $("#gview_paint-classes-list-"+crstitle+" .ui-jqgrid-hdiv").hide();
	    $("#course_content_more_"+crstitle).hide();
	    $("#course_content_less_"+crstitle).show();
	    $('#gview_paint-classes-list-'+crstitle+' .lmt-lrp-crs-cls-title').trunk8(trunk8.class_detail_title);
		$('#gview_paint-classes-list-'+crstitle+' .lmt-lrp-crs-cls-desc').trunk8(trunk8.lp_detail_course_desc);

	// While click on jqrow the details will open
	   /*$("#paint-classes-list .jqgrow a[id$='#class-accodion-']").click(function(event){
		   event.stopPropagation();
	   });
	   */
	   /*$("#paint-classes-list .jqgrow").click(function(){
		   var curAnchor = "#class-accodion-" + parseInt($(this).attr("id"));		   
		   $(curAnchor).click(function(event){
			   event.stopPropagation();
		   }).trigger("click");			   
		});*/
		
		// Show more section starts here
		// $("#searchLrnplanClassListPaint-"+crstitle).showmore();
		
		var recs = response['records'];
		$("#paint-classes-list-"+crstitle).data('totalrecords', recs);
		if($("#paint-classes-list-"+crstitle).getGridParam("reccount") < recs) {
			 $('#paintlrpclslist-show_more-'+crstitle).show();
		} else {
			 $('#paintlrpclslist-show_more-'+crstitle).hide();
        }       
        $("#paint-classes-list-"+crstitle).find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');	
      
        resetFadeOutByClass('#learning-plan-details-data .search-lrnplan-class-list-paint','content-detail-code','line-item-container','course_details');
    
		Drupal.attachBehaviors();
		//Vtip-Display toolt tip in mouse over
		vtip();
		// console.log('show more here');
		$("#searchLrnplanClassListPaint-"+crstitle).showmore({
			// showAlways: true,
			'grid': "#paint-classes-list-"+crstitle,
			'gridWrapper': "#searchLrnplanClassListPaint-"+crstitle,
			'showMore': '#paintlrpclslist-show_more-'+crstitle
		});
		}catch(e){
			 // to do
			 //console.log(e, e.stack);
		 }
	},
	
	paintClassRow : function (cellvalue, options, rowObject) {	
		try{
			return rowObject;
		}catch(e){
			// to do
		}
	},
	
	showCourseDetailMore: function(course_id) {
		try {
			var obj = this;
			var courseId  = course_id;
			var objStr = '$("#learning-plan-details-display-content").data("lnrplandetails")';
			var url = this.constructUrl("learning/learning-classes-details-list/" + courseId);
			if($("#paint-classes-list-"+crstitle).html()==''){
				this.createLoader('class-list-loaderimg-'+crstitle);
			}
			
			$.ajax({
				type: "POST",
				url: url,
				// data:  '', 
				dataType: "html",
				success: function(result) {
					obj.destroyLoader('learning-plan-details-data');
					$("#course_content_moredetail_"+course_id + " .course_class_lists").html(result);
					$("#course_content_moredetail_"+course_id ).slideDown();
					$('.limit-title').trunk8(trunk8.class_detail_title);
					$('.limit-desc').trunk8(trunk8.class_detail_desc);
					$('#course_content_more_' + courseId).hide();
				}
		    });
			
			
		} catch(e) {
			// to do
			console.log(e);
		}
	},
	
	showCourseDetailLess: function(course_id) {
		try {
			var courseId = (course_id != undefined) ? course_id : 0;
			
			if (courseId > 0) {
				$('#course_content_moredetail_' + courseId).slideUp();
				$('#course_content_more_' + courseId).show();
				
			}
		} catch(e) {
			
		}
	}
 	
});

$.extend($.ui.lnrplandetails.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
		$("#learning-plan-details-display-content" ).lnrplandetails();
		// $("#searchLrnplanCourseListPaint").showmore();
		$('.lmt-lrp-title').trunk8(trunk8.class_detail_title);
		$('.lmt-lrp-desc').trunk8(trunk8.class_detail_desc);
	}catch(e){
		 // to do
	 }
});


function openAttachment(url){
	try{
		var woption = "width=800,height=900,toolbar=no,location=yes,status=yes,menubar=no,scrollbars=yes,resizable=1";
		window.open(url,"_blank",woption);
	}catch(e){
		 // to do
	 }
}
