(function($) {

$.widget("ui.lnrcoursedetails", {
	
	lastClassIdAndSessionId: -1,
	
	_init: function() {
		try{
			//alert(this.options.name);
		this.renderClassesList();
		this.prevObj;
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
	
	displayClassDetails: function(classIdAndSessionId, catdiv) {
		try{
		this.createLoader('gbox_paint-classes-list');
		var obj = this;
		var objStr = '$("#course-details-display-content").data("lnrcoursedetails")'; 
		var url = this.constructUrl("learning/fetch-class-details/" + classIdAndSessionId); 
		var rtnResult;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			dataType: "html",
			success: function(result) {
				obj.destroyLoader('gbox_paint-classes-list');
				var str='';
				str +="<td colspan='7' class='course-class-subgrid'>"+result+"</td>";
				$("#"+classIdAndSessionId+"SubGrid").html(str);
				vtip();
				//catdiv.html(result);
				//rtnResult = result;
			}
	    });
		//return rtnResult;
		}catch(e){
			// to do
		}
	},
	displayClassViewDetails : function(classIdAndSessionId, catdiv) {
		try{
			$("#paint-classes-list").height(40);
			this.createLoader('gbox_paint-classes-list');
			var obj = this;
			var objStr = '$("#course-details-display-content").data("lnrcoursedetails")'; 
			var url = this.constructUrl("administration/view-classdetails/" + classIdAndSessionId); 
			var rtnResult;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				dataType: "html",
				success: function(result) {
					obj.destroyLoader('gbox_paint-classes-list');
					var str='';
					str +="<td colspan='7' class='course-class-subgrid'>"+result+"</td>";
					$("#"+classIdAndSessionId+"SubGrid").html(str);
					$(".limit-title").trunk8(trunk8.admin_title);
					$("#view-scroll-wrapper").jScrollPane({});
					}
		    });
			//return rtnResult;
			
			}catch(e){
				// to do
			}
		},
	
	paintName: function(cellvalue, options, rowObject) {
		try{
		var obj = options.colModel.widgetObj;
		var path = options.colModel.name;
		var html		= '';
		var mroclass = '';
		var mroName = '';
		if (rowObject['name'] == null || rowObject['name'] == '') {
			html += '-';
		}
		else {
			var ClassId = rowObject['ClassId'];
			var SessId  = rowObject['SessonId']; 
			var manClass = rowObject['mro_id'];
 			var data1 = "data={'ClsId':'"+ClassId+"','SessId':'"+SessId+"'}";
 			//if(rowObject['exemptedid'] == 0){
 				if(manClass == 'cre_sys_inv_man') {
 	 				mroclass = 'course-mandatory-bg';
 	 				mroName = 'M';
 	 			}
// 	 			else if(manClass == 'cre_sys_inv_opt') {
// 					mroclass = 'course-optional-bg'; 
// 	 				mroName = 'O';
// 	 			}
 	 			else if(manClass == 'cre_sys_inv_rec') {
 	 				mroclass = 'course-optional-bg'; 
 	 				mroName = 'R';
 	 			}
 			//}
 			
 			
 			//var title = (path == "ViewCourseDetail") ? titleRestricted(rowObject['name'],25) : titleRestricted(rowObject['name'],25);
 			var course_title =titleRestrictionFadeoutImage(rowObject['name'],'course-names');
 			//Embed widget related work (Create flexible grid)
 		 	if(Drupal.settings.widget.widgetCallback==true){
 		 		course_title =class_course_tp_title_controler_for_widget(rowObject['name']);
 			}
			html += '<div class="enroll-course-title">';
			html += '<span id="class_attachment_'+ClassId+'"></span>';
			html += '<a id="class-accodion-'+ClassId+'" style ="cursor: pointer;" data="'+data1+'" class="title_close" onclick=\''+obj+'.addAccordionToTable1(this.className,"0 0","0 -61px","dt-child-row-En",'+obj+'.getCrsClassDetails,this,'+obj+',true,"'+path+'"); \'>&nbsp;&nbsp;</a>';
			html += '<span id="titleAccEn_'+ClassId+'" class="item-title" ><span class="enroll-class-title vtip" title="'+rowObject['name']+'" href="javascript:void(0);"> '+ course_title +'</span><span class='+mroclass+'>'+mroName+'</span></span>';
			html += '</div>';
 		}
 		return html;
 		$(".limit-title").trunk8(trunk8.admin_title);
		}catch(e){
			// to do
		}
	},
	addAccordionToTable1 : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove,path) {
		try{
		var className =  openCloseClass;
		this.addAccordionToTableView(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove, path);
		//$(".limit-title").trunk8(trunk8.admin_title);
		//$("#view-scroll-wrapper").jScrollPane({});
		}catch(e){
			// to do
		}
	},
	addAccordionToTableView : function(openCloseClass,openpos,closepos,childClass,callback,obj,parentObj,isRemove,path) {
		try{
		var currTr = $(obj).parent().parent().parent().attr("id");
		var curAccor = currTr.split("-");
		var is_chrome = navigator.userAgent.indexOf('Chrome') > -1;
		var is_safari = navigator.userAgent.indexOf("Safari") > -1;
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr).after("<tr id='"+currTr+"SubGrid'><td colspan='7' class='course-class-subgrid'></td></tr>");
			$("#"+currTr+"SubGrid").show();//css("display","block");
			$("#class-accodion-"+curAccor[0]).removeClass("title_close");
			$("#class-accodion-"+curAccor[0]).addClass("title_open");
			$("#manage-location-time-"+curAccor[0]).removeClass("det_close");
			$("#manage-location-time-"+curAccor[0]).addClass("det_open");
			//$("#ClassDetailsMainDiv").css('border-bottom','1px solid #cccccc');
			$("#paint-classes-list").find("tr.ui-widget-content").css('border-top','1px solid #ededed');
			$("#"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"SubGrid").slideDown(1000);
			if($('#paint-classes-list tr td div.enroll-course-title a.title_open').length != 0) {
				if ((is_chrome)&&(is_safari)) { is_safari = false; }
				if (is_safari) { 
					$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','4px');
					$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','2px');
				}
				else {
					$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','4px');
					$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','6px');
				}
				} else {
					$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','2px');
					$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','2px');
				} 
		//	$(".limit-title").trunk8(trunk8.admin_title);
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"SubGrid").show();//css("display","block");
    			$("#"+currTr+"SubGrid").slideDown(1000);
    			$("#class-accodion-"+curAccor[0]).removeClass("title_close");
				$("#class-accodion-"+curAccor[0]).addClass("title_open");
				$("#"+currTr).removeClass("ui-widget-content");
				if ((is_chrome)&&(is_safari)) { is_safari = false; }
				if (is_safari) { 
					$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','4px');
					$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','2px');
				}
				else {
					$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','4px');
					$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','6px');
				}
    		} else {
				$("#paint-classes-list tr.jqgrow td:nth-child(5)").css('padding-left','2px');
				$("#paint-classes-list tr.jqgrow td:nth-child(6)").css('padding-left','2px'); 
    			$("#"+currTr+"SubGrid").hide();//css("display","none");
    			$("#"+currTr+"SubGrid").slideUp(1000);
    			$("#class-accodion-"+curAccor[0]).removeClass("title_open");
				$("#class-accodion-"+curAccor[0]).addClass("title_close");
				$("#"+currTr).removeClass("ui-widget-content");
				$("#"+currTr).addClass("ui-widget-content");
    		}
    		//$(".limit-title").trunk8(trunk8.admin_title);
		}
		//var curAccor = currTr.split("-");

		var data = eval($("#class-accodion-"+curAccor[0]).attr("data"));
		var Class_Id = data.ClsId;
		var Sess_Id  = data.SessId;
		var ClsSessId = Class_Id+'-'+((Sess_Id != 'null') ? (Sess_Id) : '');
		if(path == "ViewCourseDetail") {
		  //var ClsSessId = Class_Id;
		  var ClassDetsec = 	 this.displayClassViewDetails(ClsSessId, obj);
		} else {
		  var classDetSec = this.displayClassDetails(ClsSessId, obj);
		}
		$('.dt-child-row-En > td').attr('colSpan','7');
		//$(".limit-title").trunk8(trunk8.admin_title);
		$("#view-scroll-wrapper").jScrollPane({});
		//$(".limit-title").trunk8(trunk8.admin_title);
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
        ostr += parentObj.displayClassDetails(ClsSessId, catdiv);
		catdiv.css('display','block');
		$('.dt-child-row-En > td').attr('colSpan','7');
		}catch(e){
			// to do
		}
	},
	
	paintDeliveryType: function(cellvalue, options, rowObject) {
		try{
		var row_del_type;
		if (rowObject['type'] == '' || rowObject['type'] == '-') {
			return '<span>&nbsp;</span>';
		}
		else if(rowObject['type'] == 'WBT'){
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
	    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())){
		  class_loc = 'delivery-type';
		}
	    else if (navigator.userAgent.indexOf('Safari') != -1) {
	      class_loc = 'class-delivery-type';
	    } 
	    else { 
	     class_loc = 'delivery-type';
	    }
		return '<span class="vtip" title="'+row_del_type+'">'+titleRestrictionFadeoutImage(row_del_type,class_loc)+'</span>';	    
		
		}catch(e){
			// to do
		}
	},
	
	paintLocation: function(cellvalue, options, rowObject) {
		try{
		if (rowObject['location'] == '' || rowObject['location'] == '-') {
			return '<span>&nbsp;</span>';
		}
		if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())){
		  class_loc = 'location-names';
		}
	    else if (navigator.userAgent.indexOf('Safari') != -1) {
	      class_loc = 'class-location-names';
	    } 
	    else { 
	     class_loc = 'location-names';
	    }
		return '<span class="vtip" title="'+htmlEntities(rowObject['location'])+'">'+titleRestrictionFadeoutImage(rowObject['location'],class_loc)+'</span>';		
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
			custTime = format_time[0] + ' ' + '<span class="time-slab">' + format_time[1] + '</span>';
			custTime = '<span>'+ custTime +'</span>';
		}
		
		return custTime;
		}catch(e){
			// to do
		}
	},
	
	paintEndDate: function(cellvalue, options, rowObject) {
		try{
		 
			var custTime = '';
			var type = rowObject['type'];
			if(type == 'ILT' || type == 'VC') {	
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
					var sessionQtip = qtip_popup_paint('course-details-'+ClassId,startDateForTitle); 
					custTime = '<span>'+ custTime + ' ' +sessionQtip;
				}
			}
			return custTime;
		}catch(e){
			// to do
		}
	},
	
	paintAction: function(cellvalue, options, rowObject) {
		try{
		return rowObject['action'];
		}catch(e){
			// to do
		}
	},
	
	renderClassesList: function() {
		try{
		this.createLoader('class-list-loader');
		var courseId = $('#courseId').val();
		var obj = this;
		var objStr = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.options.name == 'ViewCourseDetail'){
			if(this.currTheme == "expertusoneV2"){
			  var gridWidth 		= 710;
			} else{
			  var gridWidth 		= 700;
			}
		} else{
		   var gridWidth 		= '100%';
		}
		var objStr;
		objStr = '$("#course-details-display-content").data("lnrcoursedetails")';
		objStr.callFrom =  'ViewCourseDetail';
		if(this.options.name == 'ViewCourseDetail'){
		  var colNames1 =[Drupal.t('LBL107'), Drupal.t('LBL042'), Drupal.t('LBL251'), Drupal.t('LBL252'), Drupal.t('LBL036'), Drupal.t('Location')];
		  var colModel1 = [
			           {name: Drupal.t('LBL107'), index:'class.title', width:210,'align':'left', title:false,'widgetObj':objStr, formatter:obj.paintName,'name': 'ViewCourseDetail'},
			           {name: Drupal.t('LBL042'), index:'start_date', width:100,'align':'left','sorttype':'text',title:false,'widgetObj':objStr, formatter:obj.paintDate},
			           {name: Drupal.t('LBL251'), index:'StartDate', width:80,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintStartDate}, 
			           {name: Drupal.t('LBL252'), index:'EndDate', width:80,'align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintEndDate},
			           {name: Drupal.t('LBL036'), index:'DeliveryType', width:99,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintDeliveryType}, 
			           {name: Drupal.t('Location'), index:'Location', width:130,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintLocation},
					  ];
		}else{
		  /*var colNames1 = [Drupal.t('LBL107'), Drupal.t('LBL042'), Drupal.t('LBL251'), Drupal.t('LBL252'), Drupal.t('LBL036'), Drupal.t('Location'),''];
		  var colModel1 = [{name: Drupal.t('LBL107'), index:'class.title', width:252,'align':'left', title:false,'widgetObj':objStr, formatter:obj.paintName},
						           {name: Drupal.t('LBL042'), index:'start_date', width:110,'align':'left','sorttype':'text',title:false,'widgetObj':objStr, formatter:obj.paintDate},
						           {name: Drupal.t('LBL251'), index:'StartDate', width:90,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintStartDate}, 
						           {name: Drupal.t('LBL252'), index:'EndDate', width:90,'align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintEndDate},
						           {name: Drupal.t('LBL036'), index:'DeliveryType', width:120,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintDeliveryType}, 
						           {name: Drupal.t('Location'), index:'Location', width:140,'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintLocation},
								   {name:'', index:'Action', width:130,'align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintAction} ];*/
		  var colNames1 = [''];
		  var colModel1 = [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}];
		}
		if(this.options.name == 'ViewCourseDetail'){
			var url = this.constructUrl("learning/fetch-classes-list-view/" + courseId+'/admin');
		}else{
			//var url = this.constructUrl("learning/fetch-classes-list/" + courseId+'/learner');
			var url = this.constructUrl("learning/fetch-detailed-class-list/" + courseId+'/learner');
		}
		//Embed widget related work (Create flexible grid)
		if(Drupal.settings.widget.widgetCallback==true){
			if(this.options.name == 'ViewCourseDetail'){
				  colModel1 = [
					           {name: Drupal.t('LBL107'), index:'class.title', width:150,fixed: true, 'align':'left', title:false,'widgetObj':objStr, formatter:obj.paintName,'name': 'ViewCourseDetail'},
					           {name: Drupal.t('LBL042'), index:'start_date', 'align':'left','sorttype':'text',title:false,'widgetObj':objStr, formatter:obj.paintDate},
					           {name: Drupal.t('LBL251'), index:'StartDate', 'align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintStartDate}, 
					           {name: Drupal.t('LBL252'), index:'EndDate','align':'left', title:false, 'sortable':false,'widgetObj':objStr, formatter:obj.paintEndDate},
					           {name: Drupal.t('LBL036'), index:'DeliveryType','align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintDeliveryType}, 
					           {name: Drupal.t('Location'), index:'Location','align':'left', title:false,'sortable':false, 'widgetObj':objStr, formatter:obj.paintLocation},
							  ];
				}else{
				  var colNames1 = [''];
				  var colModel1 = [{name:'Cell',index:'Cell', title:false, width:gridWidth,'widgetObj':objStr,formatter:obj.paintClassRow}];
				}
			$("#paint-classes-list").jqGrid({ 
				url: url,
				datatype: "json", 
				mtype: 'GET',
				
				colNames:colNames1,
				colModel:colModel1, 
				rowNum:10, // A large number as we want to show all classes
				rowList:[10,20,30], 
				viewrecords: true, 
				emptyrecords: "",
				sortorder: "asc", 
				toppager:false,
				height: 'auto',
				loadtext: "",
				autowidth: true,
				headertitles:true,
                shrinkToFit: true,
				recordtext: "",
				loadui:false,
				loadComplete:obj.callbackLoader
			});
		}else{
			if(this.options.name == 'ViewCourseDetail'){
				$("#paint-classes-list").jqGrid({ 
					url: url,
					datatype: "json", 
					mtype: 'GET',
					
					colNames:colNames1,
					colModel:colModel1, 
					rowNum:10, // A large number as we want to show all classes
					rowList:[10,20,30], 
					viewrecords: true, 
					emptyrecords: "",
					sortorder: "asc", 
					toppager:false,
					height: 'auto',
					loadtext: "",
					width:gridWidth,
					recordtext: "",
					loadui:false,
					loadComplete:obj.callbackLoader
				});
			}else{
				$("#paint-classes-list").jqGrid({ 
					url: url,
					datatype: "json", 
					mtype: 'GET',
					
					colNames:colNames1,
					colModel:colModel1, 
					rowNum:10, // A large number as we want to show all classes
					rowList:[10,20,30], 
					viewrecords: true, 
					emptyrecords: "",
					sortorder: "asc", 
					toppager:false,
					height: 'auto',
					loadtext: "",
					autowidth: true,
					shrinkToFit: true,
					recordtext: "",
					loadui:false,
					loadComplete:obj.callbackLoader
				}); //.navGrid('#pager', {add:false,edit:false,del:false,search:false,refreshtitle:true});
			}
		}
		$('.ui-jqgrid').css('margin','0px');
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
		     if (navigator.userAgent.indexOf("Chrome")>0) {
		        $('#paint-classes-list').find('tr td:nth-child(6)').css("width","105px");
		     }
		   var lang = Drupal.settings.user.language;		  
		    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())){
		        $('#paint-classes-list tr:first').height(0);
		    }
		    else if (navigator.userAgent.toLowerCase().indexOf('safari') != -1 ) {
		    	$('#paint-classes-list tr:first').height(0);
			    $('.ui-jqgrid-htable').find('tr th:nth-child(5)').css("width","96px");
			    $('.ui-jqgrid-btable').find('tr td:nth-child(5)').css("width","96px");
			 }
		}

		if(availableFunctionalities.exp_sp_forum){
		 	/* call to function in forum.js to render the forum topics */
			var contId = $("#tdataCrsId").val();
			var frm = "Course";
			var newObj = {id:contId,src:frm};
		 	$('#forum-topic-list-display').data('forumlistdisplay').renderForumTopicResults(newObj);
	 	 }
	//	$(".limit-title").trunk8(trunk8.admin_title);
		}catch(e){
			// to do
		}
	},
	 
	callbackLoader : function(response, postdata, formid){
		try{
			// Set number of classes in statistics section of the course detail
			$('#course_detail_content .right-section .stats-cls-cnt .stats-val').text(response.records);
			$("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
			   // Now, display class details of the first class in the displayed list.
			   var dataIDs = $('#paint-classes-list').jqGrid('getDataIDs');
			   
			   //$("tr.jqgrow:odd td").css("background-color", "#DBEEFA");
			   
			   if (dataIDs.length <= 0) {
				   $(".ui-jqgrid-htable").hide();
				   $('#paint-classes-list').html('<div class=" no-records">'+Drupal.t("ERR061")+'</div>');
			   }
			   else {
				   var firstRowId = dataIDs[0];
				   $('#paint-classes-list').jqGrid('setSelection',firstRowId);
			   }
			   
			   $("#paint-classes-list").find("tr").each(function(){
					$(this).removeClass("ui-state-highlight");
				});
			   
			//widget ie 10 related work
			if(Drupal.settings.widget.widgetCallback==true){
	           if($.browser.msie && parseInt($.browser.version, 10)=='10'){
	             $("#course-details-display-content .ui-jqgrid .ui-jqgrid-bdiv").css("height","auto");  
	           }
			}

	   // code added by yogaraja
	   //0037426: Code Re-Factoring 2. Learning > Courses / Classes > Course Detail popup > Enrollments popup - In the certificate table list, when expand collapse a list title, the scroll behaves properly. But when clicking the sort arrow on the table head, the scroll doesn't disappear even if the list title is in collapsed state.
	   if(document.getElementById("viewcourse-detail-wrapper")!= null  && typeof(document.getElementById("viewcourse-detail-wrapper"))!= "undefined")
	   {
		   $("#view-scroll-wrapper").jScrollPane({});
	   } 
	   setTimeout(function(){
			if(Drupal.settings.convertion.mylearn_version==1 && !document.getElementById("viewcourse-detail-wrapper")){
				$.each(response.rows, function (i, list){
					if(list['class']['action_variables']['showprogress']===true)
						progressBarRound(list['class']['action_variables']['enrolled_id'],list['class']['action_variables']['progress'], 'enr_progress','progress_',$("#lnr-catalog-search").data("contentPlayer"));
						//$("#lnr-catalog-search").data("contentPlayer").progressbardetail(list['class']['action_variables']['enrolled_id'],list['class']['action_variables']['progress'], 'enr_progress','progress_');
					});
			 }
		},100) ;
	   
	// While click on jqrow the details will open
		//Vtip-Display toolt tip in mouse over
		// vtip();
	//	 $(".limit-title").trunk8(trunk8.admin_title);	
		//after login auto register or add to cart related work start
		 var courseobj = $("#course-details-display-content").data("lnrcoursedetails");
		 var user_id =courseobj.getLearnerId();
		// console.log(user_id);
		if(user_id !='' && user_id != null && user_id!=undefined && user_id!=0){
			 var user_selected_class_id = $.cookie("user_selected_class_id");
			 if(user_selected_class_id != null && user_selected_class_id !=undefined){
				 var onclickprop = $('#'+user_selected_class_id).attr("onclick");
				 if(onclickprop != null &&  onclickprop !=''  && onclickprop!=undefined)
					 $('#'+user_selected_class_id).click();
				 else
					 $("body").data("learningcore").callMessageWindow(Drupal.t('LBL721'),Drupal.t('ERR047'));
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
		else {
			$('#signin').click();
		}
		
		// Show more section starts here
		var recs = response['records'];
		var showMore = $('#paintcrsclslist-show_more');
		$("#paint-classes-list").data('totalrecords', recs);
		if($('#paint-classes-list').getGridParam("reccount") < recs) {
			showMore.show();
		} else {
			showMore.hide();
        }     
        $("#paint-classes-list").find('tr.ui-widget-content').filter(":last").addClass('ui-widget-content-last-row');	

		Drupal.attachBehaviors(); 
		$("body").data("learningcore").disableFiveStarOnVoting();
	   // New codes started here
	    $("#gbox_paint-classes-list .ui-jqgrid-hdiv").hide();
	    $('.lmt-crs-cls-title').trunk8(trunk8.class_detail_title);
		$('.lmt-crs-cls-desc').trunk8(trunk8.class_detail_desc);
		$('.lmt-cls-desc-add').trunk8(trunk8.class_detail_add_desc);
		if(Drupal.settings.widget.widgetCallback==true){
		resetFadeOutByClass('#course_detail_content #searchCourseClassListPaint','content-detail-code','line-item-container','course_details');
		}
		vtip('.course-content-wrapper');
		$("#searchCourseClassListPaint").showmore({
			// showAlways: true,
			'grid': "#paint-classes-list",
			'gridWrapper': "#searchCourseClassListPaint",
			'showMore': '#paintcrsclslist-show_more'
		});
		}catch(e){
			// to do
			console.log(e)
		}
	},
	
	paintClassRow : function (cellvalue, options, rowObject) {	
		try{
			return rowObject;
		}catch(e){
			// to do
		}
	},
 	
});

$.extend($.ui.lnrcoursedetails.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
	
		$( "#course-details-display-content" ).lnrcoursedetails();
		// $( "#searchCourseClassListPaint" ).showmore();
		$('.lmt-crs-title').trunk8(trunk8.class_detail_title);
		$('.lmt-crs-desc').trunk8(trunk8.class_detail_desc);
		if(Drupal.settings.widget.widgetCallback==true){
		//resetFadeOutByClass('#course-details-display-content','class-misc-wrapper','prerequisite-block','course_details');
		}
		
	}catch(e){
		// to do
	}
});
Drupal.ajax.prototype.commands.callCourseDetailWidget = function(ajax, response, status) {
    try {
    	$( "#course-details-display-content" ).lnrcoursedetails({"name":"ViewCourseDetail"});
    //	$(".limit-title").trunk8(trunk8.admin_title);
    }
    catch(e){
      //Nothing to do
    }
  };
function openAttachment(url){
	try{
		var woption = "width=800,height=900,toolbar=no,location=yes,status=yes,menubar=no,scrollbars=yes,resizable=1";
		window.open(url,"_blank",woption);
	}catch(e){
		// to do
	}
}