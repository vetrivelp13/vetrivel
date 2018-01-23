(function($) {

$.widget("ui.adminlearningcore", {
	_init: function() {
		try{
		var self = this;
		}catch(e){
			// to do
		}
	},

	multiClassQTip : function(entityId,uniqueId,recertifyValue){
		try{
		var userIdList = $('[name=hidden_idlist_'+uniqueId+']').val(); 
		if(userIdList == '') {
			var errMsg = new Array();
			errMsg[0] = Drupal.t('ERR106');
			var message_call = expertus_error_message(errMsg,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();			
		}else{
		  this.multiClassQTipAfter(entityId, uniqueId,userIdList,recertifyValue);
		}
		}catch(e){
			// to do
		}
	},
	
	multiClassQTipAfter : function(entityId,uniqueId,userIdList,recertifyValue){
		try{
		if(typeof(recertifyValue) == 'undefined') recertifyValue = '';
		$('#multi-class-container').show();
		$('.bottom-qtip-tip-up,.page-administration-learning-program .bottom-qtip-tip-up-visible').css('z-index','99');
		this.getModuleListForTPAdminSide(entityId, 'Y',userIdList,'adminside',recertifyValue);
		hideSearchTP();
		}catch(e){
			// to do
		}
	},	
	getModuleListForTPAdminSide : function( prgId, isAdminSide,userListIfAdminSide,fromPage,recertifyValue) {		
		try{
			this.createLoader('multi-select-class-wrapper');
			if(isAdminSide == 'Y') {
				$(".error").css('display','none');				
			}
			
			var url = this.constructUrl("ajax/trainingplan/module-list/"+prgId+ "/" + userListIfAdminSide+ "/" + isAdminSide+ "/" +fromPage);		
			var obj = this;
			var params = {"recertifyid":recertifyValue};
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){				
				
						obj.paintTPModuleDetailsAdminSide(result,prgId,isAdminSide,userListIfAdminSide,fromPage);
						
						if(isAdminSide == 'Y') {
							$(".error").css('display','none');
						}
						obj.destroyLoader('multi-select-class-wrapper');
											 
				}
		    });
		}catch(e){
			// to do
		}
	},
	
	paintTPModuleDetailsAdminSide : function(result,prgId,isAdminSide,userIdList,fromPage){
	 try{
		var rhtml = "";
		var c;
		var moduleTitle;
		var moduleId;
		var moduleCode;
		var moduleDesc;
		
		var courseList = '';
		var courseId;		
		
		var obj = $("body").data("adminlearningcore");
		var objStr = '$("body").data("adminlearningcore")';
		if(result.length > 0 ) {
			rhtml += "<table style='width:100%;'>";
			//rhtml += "<tr><td> <div id='successMsgDisplay' class='messages status' style='display:none;'></div> <div id='errMsgDisplay' class='messages error' style='display:none;'></div></td></tr>";
			rhtml += "<tr><td><div class='module-level'>";
			
			modId = result[0]['module_details']['module_id'];
			modTitle = result[0]['module_details']['module_title'];
			modCode = result[0]['module_details']['module_code'];
			/*Module level */
			//var grpIds = new Array();
			//for(c=0; c < result.length ; c++){
			//	grpIds[c] = result[c]['module_details']['group_id'];
			//}
			//var param1 = "data={'grpIds':'"+grpIds+"'}"; 
			rhtml += '<a class="set-height-column-left title_open" id="lp-module-level-accodion-'+modId+'" href="javascript:void(0);"  onclick=\''+objStr+'.accordionForModuleLevelViewAdminSide('+modId+');\'>&nbsp;</a>';
			rhtml += '<div class="set-height-column-right"><span class="item-title"><span class="lp-reg-class-title" title="'+unescape(modTitle)+'" href="javascript:void(0);">';
			rhtml += titleRestrictionFadeoutImage(modTitle,'tp-view-module-title');
			rhtml += '</span></span>';
			rhtml += '</div>';
			
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse module-container-seperation' cellspacing='0'>";
			for(c=0; c < result.length ; c++){

				/*moduleId = result[c]['module_details']['module_id'];
				moduleTitle = result[c]['module_details']['module_title'];
				moduleCode = result[c]['module_details']['module_code'];
				moduleDesc = result[c]['module_details']['module_desc'];*/	
				moduleId = result[c]['module_details']['group_id'];
				moduleTitle = result[c]['module_details']['group_title'];
				moduleCode = result[c]['module_details']['group_code'];
				moduleDesc = result[c]['module_details']['module_desc'];				
				
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
							 //cls_det += "'enrolled_id':'"+x['class_details']['enrolled_id']+"',";
							 
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
						
						rhtml += '<tr style="display:none;"><td><input type="hidden" id="classListIds-'+courseId+'" name="classListIds" value="'+regValue+'"></td></tr>';
						
						
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
				rhtml += '<td style="border-bottom:0px;">';
				if(moduleId == null || moduleId == undefined || moduleId == ''){
					rhtml += '<div class="set-height-column-left" id="lp-crs-accodion-module-'+moduleId+'" data="'+param+'"></div>';
				}else{
				rhtml += '<a class="set-height-column-left title_close" id="lp-crs-accodion-module-'+moduleId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForModuleViewAdminSide('+moduleId+');\'>&nbsp;</a>';
				rhtml += '<div class="set-height-column-right"><span class="item-title"><span class="lp-reg-class-title" title="'+unescape(moduleTitle)+'" href="javascript:void(0);">';
				rhtml += titleRestrictionFadeoutImage(moduleTitle,'tp-view-module-title');
				rhtml += '</span></span>';
	
				rhtml += '</div>';
				}
				rhtml += '</td>';
				rhtml += '</tr>';		

		  }

			
			 rhtml += '<tr><td><input type="hidden" id="courseListIds" name="courseListIds" value="'+courseList+'"></td></tr>';
			 
	    	 rhtml += '<tr><td><input type="hidden" id="isadminsidefield" name="isadminsidefield" value="'+isAdminSide+'"></td></tr>';
	    	 
			 rhtml += '<tr><td><input type="hidden" id="userListIds" name="userListIds" value="'+userIdList+'"></td></tr>';
				
			 
		  rhtml += '</table>';
		  rhtml += '</div></td></tr>';  
		}
		else{
			rhtml += "<tr><td class='no-item-found'>"+SMARTPORTAL.t('No modules associated with this.')+"</td></tr>";
		}
		
		 rhtml += "</table>";
		
		 $("#multi-container-pop-up").html(rhtml);
		 
		 $('#multi-container-pop-up').jScrollPane();
		 $('.enroll-show-morecourse .set-height td .title_close').trigger("click");
		  var grpId = result[0]['module_details']['group_code'];
		 if(grpId == null || grpId == undefined || grpId == ''){
			 this.accordionForModuleViewAdminSide(grpId,fromPage);
			 $('#lp-crs-accodion-module-'+grpId).removeClass('title_open');
		 }
		 for(c=0; c < result.length ; c++){
			moduleId = result[c]['module_details']['group_id'];
			var data = eval($("#lp-crs-accodion-module-"+moduleId).attr("data"));
			var classDetSec = this.paintTPCourseDetailsAdminSide(data);
			//$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
			$("#lnr-tainingplan-register .course-level .set-height-column-right").css('border-bottom','0px none');
			$(".class-details-info tr:last-child .course-delivery-info").css('border-bottom','0px none');
			$('#multi-container-pop-up').jScrollPane();
		 }
	 	}catch(e){
			// to do
		}
	 	vtip();
	},
	accordionForModuleLevelViewAdminSide : function(moduleId) {
		try{
			//var data = eval($("#lp-module-level-accodion-"+moduleId).attr("data"));
			//console.log(data.grpIds);
			//var userArr = data.grpIds.split(',');
	
			if($("#lp-module-level-accodion-"+moduleId).hasClass('title_close')){
				$("#lp-module-level-accodion-"+moduleId).removeClass("title_close");
				$("#lp-module-level-accodion-"+moduleId).addClass("title_open");
				$('.enroll-show-morecourse').show();
				$('.enroll-show-morecourse .set-height td .title_close').trigger("click");
			}else{
				$("#lp-module-level-accodion-"+moduleId).removeClass("title_open");
				$("#lp-module-level-accodion-"+moduleId).addClass("title_close");
				$('.enroll-show-morecourse').hide();
				$('.enroll-show-morecourse .set-height td .title_open').trigger("click");
			}
			//for(c=0; c < userArr.length ; c++){
			    //console.log(userArr[c])
			//	$('#lp-crs-accodion-module-'+userArr[c]).click();
			//}
			//$('.enroll-show-morecourse').show();
	
		}catch(e){
			// to do
		}
	},
	accordionForModuleViewAdminSide : function(moduleId) {
	 try{	
		var currTr = moduleId;
		if(!document.getElementById(currTr+"ModuleSubGrid")) {
			$("#lp-crs-module-"+currTr).after("<tr id='"+currTr+"ModuleSubGrid'><td colspan='4'><div id='module-details-"+currTr+"' ></div></td></tr>");
			$("#"+currTr+"ModuleSubGrid").show();//css("display","block");
			$("#lp-crs-accodion-module-"+currTr).removeClass("title_close");
			$("#lp-crs-accodion-module-"+currTr).addClass("title_open");
			//$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ModuleSubGrid").slideDown(1000);
			$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
		} else {
			var clickStyle = $("#"+currTr+"ModuleSubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"ModuleSubGrid").show();//css("display","block");
    			$("#"+currTr+"ModuleSubGrid").slideDown(1000);
    			$("#lp-crs-accodion-module-"+currTr).removeClass("title_close");
				$("#lp-crs-accodion-module-"+currTr).addClass("title_open");
				//$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
    		} else {
    			$("#"+currTr+"ModuleSubGrid").hide();//css("display","none");
    			$("#"+currTr+"ModuleSubGrid").slideUp(1000);
    			$("#lp-crs-accodion-module-"+currTr).removeClass("title_open");
				$("#lp-crs-accodion-module-"+currTr).addClass("title_close");
				//$("#lp-crs-module-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-module-"+currTr).addClass("ui-widget-content");
				//$("#lp-crs-module-"+currTr).find(".set-height-column-right").css("border-bottom","1px solid #cccccc");  //commented for accordion ui issues in multiple qtip popup
				//$("#lp-crs-module-"+currTr).find(".set-height-column-right").last().css("border-bottom","0px");
    		}
		}		
		setTimeout(function() {
			try {
				resetFadeOutByClass('#multi-class-container','item-title-code','line-item-container','select');
			} catch (e) {
				// TODO: handle exception
				console.log(e, e.stack);
			}
			
		}, 1000);
		/*var data = eval($("#lp-crs-accodion-module-"+currTr).attr("data"));		
		
		var classDetSec = this.paintTPCourseDetailsAdminSide(data);
		$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
		$('#multi-container-pop-up').jScrollPane();*/
	 }catch(e){
			// to do
		}
	},
	

	paintTPCourseDetailsAdminSide : function(data){
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
		
		var inc = 0;
		
		var obj = $("body").data("adminlearningcore");
		var objStr = '$("body").data("adminlearningcore")';
		rhtml += "<table cellpadding='0' cellspacing='0' style='width:100%'>";
		if(crslength > 0 ) {

			rhtml += "<tr><td><div class='course-level'>";
			rhtml += "<table border='0' cellpadding='0' class='enroll-show-morecourse' cellspacing='0'>";
			for(c=0; c < crslength ; c++){
				var rhtmlcls = obj.paintTPClassDetailsAdminSide(data.courseDetails[c]['class_details']);
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
	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-crs-course-'+courseId+'">'; 
				rhtml += '<td>';
				rhtml += '<a class="set-height-column-left title_open" id="lp-crs-accodion-course-'+courseId+'" href="javascript:void(0);" data="'+param+'" onclick=\''+objStr+'.accordionForCourseViewAdminSide('+courseId+');\'>&nbsp;</a>';
				rhtml += '<div class="set-height-column-right"><span class="item-title"><span class="lp-reg-class-title vtip" title="'+unescape(courseTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				rhtml += titleRestrictionFadeoutImage(unescape(courseTitle),'tp-view-course-title');
				rhtml += '</span></span>';
				rhtml += '<div class="course-info item-title-code">';
				rhtml += '<div class="line-item-container float-left"><span class="vtip" title="'+unescape(courseCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(courseCode),'tp-view-course-code')+'</span></div>';
				rhtml += '<div class="line-item-container float-left"><span class="pipeline float-left">|</span>';
				var mro_access = ((isRequired =='Y')?''+Drupal.t("Mandatory")+'':''+Drupal.t("Optional")+'');
				rhtml += '<span class="vtip" title="'+mro_access+'">'+titleRestrictionFadeoutImage(mro_access,'tp-view-course-access')+'</span></div>';
				//rhtml += '<span class="vtip" title="'+mro_access+'">'+mro_access+'</span></div>';
				//if(data.isAdminSide != 'Y') {
				var reg_or_not = ((registeredCnt < 1)?''+Drupal.t("Not Registered")+'':''+Drupal.t("Registered")+'');
					rhtml += '<div class="line-item-container float-left"><span class="pipeline float-left">|</span>';
					rhtml += '<span class="vtip" title="'+reg_or_not+'">'+titleRestrictionFadeoutImage(reg_or_not,'tp-view-course-reg')+'</span></div>';
			//	rhtml += '<span class="vtip" title="'+reg_or_not+'">'+reg_or_not+'</span></div>';
				//}
				rhtml += '</div>';
				
				rhtml += '<div class="course-desc-info limit-desc-row">';
				rhtml += '<span class="limit-desc limit-desc-mulcls crs-desc vtip" title="'+unescape(courseDesc)+'">'+unescape(courseDesc)+'</span>';				
				rhtml += '</div></div>';
				rhtml += '</td>';
				rhtml += '</tr><tr id='+courseId+'CourseSubGrid ><td colspan="4"><div id="course-details-'+courseId+'">'+rhtmlcls+'</div></td></tr>';			

		  }		
		    
		  rhtml += '</table>';
		  rhtml += '</div></td></tr>';  
		}
		else{
			rhtml += "<tr><td class='no-item-found'>"+Drupal.t('ERR054')+"</td></tr>";
		}
		rhtml += "</table>";
		
		$("#module-details-"+data.moduleId).html(rhtml);
		
		$('#multi-container-pop-up').jScrollPane();
		vtip();
		$('.lp-reg-class-title-iltvc').trunk8(trunk8.admin_class_detail_title);
		$('.lp-reg-class-title').trunk8(trunk8.admin_class_detail_title);
		
	 }catch(e){
			// to do
		}	
	},
	
	accordionForCourseViewAdminSide : function(courseId) {
		try{
		var currTr = courseId;
		if(!document.getElementById(currTr+"CourseSubGrid")) {
			$("#lp-crs-course-"+currTr).after("<tr id='"+currTr+"CourseSubGrid'><td colspan='4'><div id='course-details-"+currTr+"' ></div></td></tr>");
			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
			$("#lp-crs-accodion-course-"+currTr).removeClass("title_close");
			$("#lp-crs-accodion-course-"+currTr).addClass("title_open");
			//$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"CourseSubGrid").slideDown(1000);
			$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
		} else {
			var clickStyle = $("#"+currTr+"CourseSubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"CourseSubGrid").show();//css("display","block");
    			$("#"+currTr+"CourseSubGrid").slideDown(1000);
    			$("#lp-crs-accodion-course-"+currTr).removeClass("title_close");
				$("#lp-crs-accodion-course-"+currTr).addClass("title_open");
				//$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");
				$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","0px");
    		} else {
    			$("#"+currTr+"CourseSubGrid").hide();//css("display","none");
    			$("#"+currTr+"CourseSubGrid").slideUp(1000);
    			$("#lp-crs-accodion-course-"+currTr).removeClass("title_open");
				$("#lp-crs-accodion-course-"+currTr).addClass("title_close");
				//$("#lp-crs-course-"+currTr).removeClass("ui-widget-content");  //commented for accordion ui issues in multiple qtip popup
				$("#lp-crs-course-"+currTr).addClass("ui-widget-content");
				$("#lp-crs-course-"+currTr).find(".set-height-column-right").css("border-bottom","1px solid #cccccc");
    		}
		}
		
		
		var data = eval($("#lp-crs-accodion-course-"+currTr).attr("data"));
		
		
		var classDetSec = this.getClassListForCourseAdminSide(data);
		$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');

		$('#multi-container-pop-up').jScrollPane();
		vtip();		
		}catch(e){
			// to do
		}
	},
	
	getClassListForCourseAdminSide : function(data){		
	 try{	
		var prgId = data.PrgId;		
		var courseId = data.CourseId;
		var userId = data.UserId;
		
		if($("#course-details-"+courseId).html() == ''){
			this.createLoader("course-details-"+courseId);
			var url = this.constructUrl("ajax/trainingplan/class-list/" + courseId + "/" + userId);		
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){

					obj.paintTPClassDetailsAdminSide(result,data);
					$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
					obj.destroyLoader("course-details-"+courseId);
					
						/*  Commented For Multiple class selection #0038560 
						 * for(c=0; c < result.length ; c++){								
							var enrolledId = result[c]['class_details']['enrolled_id'];								
							if(enrolledId.enrolled_id ) {							
								if($('input[name="'+courseId+'-clsregister"]:radio').is(":checked") == true)
								{
									$('input[name="'+courseId+'-clsregister"]:radio').attr('disabled',true);
								}					
							}
						} */
					
				}
		    });
		}
	 }catch(e){
			// to do
		}
	},
	
	paintTPClassDetailsAdminSide : function(result,data){
	 try{	
		var isadminsidefield = $('#isadminsidefield').val();
		
		var obj = $("body").data("adminlearningcore");
		var objStr = '$("body").data("adminlearningcore")';
		var rhtml ='';
		var userArr = ($('#userListIds').val()).split(',');
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
				var sessionStartDate = result[c]['sess_start_date'];
				var enrolledId = result[c]['enrolled_id'];
				var waitList = result[c]['waitlist_status'];	
				var availableSeats = result[c]['availableSeats'];	
				
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
							 
				var checkedValue = "";
				
				if(isadminsidefield == 'Y' && userArr.length == 1) { 
					if(enrolledId.enrolled_id) {
						if(enrolledId.comp_status == 'lrn_crs_cmp_cmp') {
							checkedValue = "checked";
						}
					}
				}
				
				if(result.length == 1) {
					checkedValue = "checked";
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
							
							startDate = sDay +' '+ sTime+' '+sTimeForm+' to '+eDay+ ' ' +eTime+' '+eTimeForm;
						}else {					

							var eTime = result[c]['session_details'][0]['end_time'];
							var eTimeForm = result[c]['session_details'][0]['end_form'];
							
							startDate = sDay +' '+ sTime+' '+sTimeForm+' to '+eTime+' '+eTimeForm;
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
	
			    var param = "data={'classId':'"+classId+"','classCode':'"+escape(classCode)+"','classDesc':'"+escape(classDesc)+"','language':'"+language+"'," +
			    		"'sessionId':'"+sessionId+"','deliveryTypeCode':'"+deliveryTypeCode+"','locationName':'"+escape(locationName)+"','locationAddr1':'"+locationAddr1+"','locationAddr2':'"+locationAddr2+"'," +
	    				"'locationCity':'"+locationCity+"','locationState':'"+locationState+"','countryName':'"+countryName+"','locationZip':'"+locationZip+"'," +
   						"'sessionDetails':"+passParams+"}";
   						
				if(last_comp_enrolledId!=''){
					if(last_comp_enrolledId == enrolledId.enrolled_id){
						checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"' : checkedValue;
					}else{
						checkedValue = '';
					}
				}else{
					checkedValue = (waitList==0 && availableSeats==0)?checkedValue+' disabled="disabled"' : checkedValue;
				}
	 			rhtml += '<tr class="set-height ui-widget-content" id ="lp-class-'+classId+'">'; 
	 			if(deliveryTypeCode =='lrn_cls_dty_vcl' || deliveryTypeCode =='lrn_cls_dty_ilt')  {
				rhtml += '<td class="course-sub-class-detail"><table border="0" width="100%" cellpadding="0" cellspacing="0"><tr><td valign="top" class="course-detail-col1">';
				
				rhtml += '<a id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="title_close" onclick=\''+objStr+'.accordionForClassViewAdminSide('+classId+');\'>&nbsp;</a>';
				rhtml += '<div class="iltvc-select-class limit-title-row"><span class="lp-reg-class-title-iltvc limit-title  vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				rhtml += classTitle;
				rhtml += '</span></div></td>';
				
				rhtml += '<td valign="top" class="course-detail-col2"><span>'+titleRestrictionFadeoutImage(startDate,'tp-view-class-sessiondate')+'</span></td>';

				//rhtml += '<input class="rButtonclass" type="radio" id="'+courseId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'">';
				rhtml += '<td valign="top" class="course-detail-col3"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></td>';
				}
				else {
				rhtml += '<td class="course-sub-class-detail"><table border="0" width="100%" cellpadding="0" cellspacing="0"><tr><td valign="top" colspan="2" class="course-detail-col1-new limit-title-row">';				
				rhtml += '<a id="lp-class-accodion-'+classId+'" href="javascript:void(0);" data="'+param+'" class="title_close" onclick=\''+objStr+'.accordionForClassViewAdminSide('+classId+');\'>&nbsp;</a>';
				rhtml += '<div class="tp-reg-select-class limit-title-row"><span class="lp-reg-class-title limit-title vtip" title="'+unescape(classTitle).replace(/"/g, '&quot;')+'" href="javascript:void(0);">';
				rhtml += classTitle;
				rhtml += '</span></div></td>';				
				//rhtml += '<td valign="top" class="course-detail-col2"><span>'+startDate+'</span></td>';
				//rhtml += '<input class="rButtonclass" type="radio" id="'+courseId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'">';
				
				rhtml += '<td valign="top" class="course-detail-col3"><input '+checkedValue+' type="radio" id="'+courseId+'-'+classId+'-clsregister" name="'+courseId+'-clsregister" value="'+classId+'"></td>';
				}
				rhtml += '</tr></table>';

				
				rhtml += '<div class="course-delivery-info item-title-code">';
				rhtml += '<div class="line-item-container float-left"><span class="vtip" title="'+unescape(classCode).replace(/"/g, '&quot;')+'">'+titleRestrictionFadeoutImage(unescape(classCode),'tp-view-class-code')+'</span></div>';
			//	rhtml += '<span class="pipeline">|</span>';
				
				var row_det_type;
				if (deliveryTypeCode == '' || deliveryTypeCode == '-') {
				return '<span>&nbsp;</span>';				
				}
				else if(deliveryTypeCode=='lrn_cls_dty_wbt'){
					row_det_type = Drupal.t("Web-based");
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_vcl'){
		        	row_det_type = Drupal.t("Virtual Class");
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_ilt'){
		        	row_det_type = Drupal.t("Classroom");
		        }
		        else if(deliveryTypeCode=='lrn_cls_dty_vod'){
		        	row_det_type = Drupal.t("Video");
		        }
				rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span><span class="vtip" title="'+unescape(row_det_type)+'">'+titleRestrictionFadeoutImage(unescape(row_det_type),'tp-view-class-code')+'</span></div>';
				
				if(deliveryTypeCode=='lrn_cls_dty_ilt') {
					rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span>';
					rhtml += '<span class="vtip" title="'+unescape(locationName)+'">'+titleRestrictionFadeoutImage(unescape(locationName),'tp-view-class-code')+'</span></div>';
				}
				
				if(deliveryTypeCode=='lrn_cls_dty_ilt' || deliveryTypeCode=='lrn_cls_dty_vcl') {
					if(waitList > 0) {
						rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span>';
						if(waitList==1)
							rhtml += '<span class="vtip" title="'+waitList+" "+Drupal.t("LBL126")+'">'+titleRestrictionFadeoutImage(waitList+" "+Drupal.t("LBL126"),'tp-view-class-code')+'</span></div>';
						else
							rhtml += '<span>'+waitList+" "+Drupal.t("LBL127")+'</span>';
					}else if(waitList == 0 && availableSeats==0){
						rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span>';
						rhtml += '<span class="vtip" title="'+Drupal.t("LBL106")+" "+Drupal.t("LBL046")+'">'+titleRestrictionFadeoutImage(Drupal.t("LBL106")+" "+Drupal.t("LBL046"),'tp-view-class-code')+'</span></div>';
					}else{
						rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span>';
						if(availableSeats==1)
							rhtml += '<span class="vtip" title="'+availableSeats+" "+Drupal.t("LBL052")+'">'+titleRestrictionFadeoutImage(availableSeats+" "+Drupal.t("LBL052"),'tp-view-class-code')+'</span></div>';
						else
							rhtml += '<span class="vtip" title="'+availableSeats+" "+Drupal.t("LBL053")+'">'+titleRestrictionFadeoutImage(availableSeats+" "+Drupal.t("LBL053"),'tp-view-class-code')+'</span>';
					}
				}
				if(enrolledId.comp_status == 'lrn_crs_cmp_cmp' || enrolledId.comp_status == 'lrn_crs_cmp_enr') {
					rhtml += '<div class="line-item-container float-left"><span class="pipeline">|</span>';
				    var regClassStatus = (enrolledId.comp_status == 'lrn_crs_cmp_cmp') ? Drupal.t("Completed") : Drupal.t("Registered");
				    rhtml += '<span class="vtip" title="'+regClassStatus+'">'+titleRestrictionFadeoutImage(regClassStatus,'tp-view-class-code')+'</span>';
				}
				rhtml += '</div>';
				rhtml += '</td>';
				rhtml += '</tr>';
		  }
			
		}else{
			
			rhtml += '<tr>';
			rhtml += '<td class="no-item-found">'+Drupal.t("ERR061")+'</td>';
			rhtml += '</tr>';
		}
	  rhtml += '</table>';
	  rhtml += '</div>';
	  return rhtml;
	  //$("#course-details-"+data.CourseId).html(rhtml);
	 // $('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
	 // $('#multi-container-pop-up').jScrollPane();
	 }catch(e){
			// to do
		}
	 vtip();
	},	
	
	accordionForClassViewAdminSide : function(classId) {
		try{
		var currTr = classId;
		if(!document.getElementById(currTr+"ClassSubGrid")) {
			$("#lp-class-"+currTr).after("<tr id='"+currTr+"ClassSubGrid'><td colspan='4'><div id='class-details-"+currTr+"' class='class-level'></div></td></tr>");
			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
			$("#lp-class-accodion-"+currTr).removeClass("title_close");
			$("#lp-class-accodion-"+currTr).addClass("title_open");
			//$("#lp-class-"+currTr).removeClass("ui-widget-content");
			$("#"+currTr+"ClassSubGrid").slideDown(1000);
			$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","0px");
			$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
		} else {
			var clickStyle = $("#"+currTr+"ClassSubGrid").css("display"); 
    		if((clickStyle == "none") || (clickStyle == 'undefined')) {
    			$("#"+currTr+"ClassSubGrid").show();//css("display","block");
    			$("#"+currTr+"ClassSubGrid").slideDown(1000);
    			$("#lp-class-accodion-"+currTr).removeClass("title_close");
				$("#lp-class-accodion-"+currTr).addClass("title_open");
				//$("#lp-class-"+currTr).removeClass("ui-widget-content");
				$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","0px");
				$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
    		} else {
    			$("#"+currTr+"ClassSubGrid").hide();//css("display","none");
    			$("#"+currTr+"ClassSubGrid").slideUp(1000);
    			$("#lp-class-accodion-"+currTr).removeClass("title_open");
				$("#lp-class-accodion-"+currTr).addClass("title_close");
				//$("#lp-class-"+currTr).removeClass("ui-widget-content"); //commented for accordion ui issues in multiple qtip popup
				//$("#lp-class-"+currTr).addClass("ui-widget-content");
				//$("#lp-class-"+currTr).find(".course-delivery-info").css("border-bottom","1px solid #cccccc");
				$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
    		}
		}
		
		
		var data = eval($("#lp-class-accodion-"+currTr).attr("data"));
		
		var classDetSec = this.paintTPClassSessionDetailsAdminSide(data);
		$('.class-details-info > tbody > tr:last > td').find('.course-delivery-info').css('border-bottom','0px none');
		$('.class-details-info > tbody > tr.set-height:last td').find('.course-delivery-info').css('border-bottom','0px');
		$('.class-details-info > tbody > tr:last > td').find('.cls-description').css('border-bottom','0px none');

		$('#multi-container-pop-up').jScrollPane();
		vtip();
		}catch(e){
			// to do
		}
	},	
	
	paintTPClassSessionDetailsAdminSide : function(data){
	 try{	
		var oHtml = '';
		oHtml += "<table cellpadding='2' class='sub-course-class' cellspacing='2' width='100%'>";
		if(data.classCode) {
			oHtml += "<tr><td class=''><div class=\"assn-lrn-left-sec\">"+Drupal.t("LBL263")+":</div> <div class=\"assn-lrn-right-sec\">"+unescape(data.classCode)+"</div></td></tr>";
		}
		oHtml += "<tr><td><div class=\"assn-lrn-left-sec\">"+Drupal.t("LBL229")+":</div><div class=\"assn-lrn-right-sec\">"+unescape(data.classDesc)+"</div></td></tr><tr><td class='cls-description'>";
		if(data.sessionDetails.length>0) {
			oHtml += '<div class="enroll-session-details"><div class=\"assn-lrn-left-sec-ses\">'+Drupal.t("LBL670")+':</div></div>';
			var inc = 1;
			$(data.sessionDetails).each(function(){ 
				if($(this).attr("sessionTitle")) {
					var sesionsH = ($(this).attr("sessionTitle") != '') ? $(this).attr("sessionTitle") : "&nbsp;"; 
					var sesionsHead = sesionsH;
					
					oHtml += '<div class="sessionDet"><div class="sessName vtip" title="'+(($(this).attr("sessionTitle") != '') ? unescape($(this).attr("sessionTitle")).replace(/"/g, '&quot;') : "&nbsp;")+'">'+titleRestrictionFadeoutImage(unescape(sesionsHead),'assignlearn-tp-cls-session-title')+'</div>';
					oHtml += '<div class="sessDay">'+$(this).attr("sessionDay")+'</div>';
					oHtml += '<div class="sessDate">'+$(this).attr("sessionSDate")+ " " +$(this).attr("start_time")+ " <span class='time-zone-text'>"+$(this).attr("sessionSDayForm")+"</span>"+' to '+$(this).attr("end_time")+" <span class='time-zone-text'>"+$(this).attr("sessionEDayForm")+"</span></div></div>";
					inc++;
				}
			});
		}
		if(data.language) {
			oHtml += "<div class='cls-lang'><div class=\"assn-lrn-left-sec\">"+Drupal.t("LBL038")+": </div><div class=\"assn-lrn-right-sec\">"+Drupal.t(data.language)+"</div></div>";
		}
		
		if(data.deliveryTypeCode=='lrn_cls_dty_ilt') {
			
				var LocationName 	= unescape(data.locationName); 
				if(LocationName != '') {
					oHtml +='<table border="0" cellpadding="0" class="enroll-loc-details" cellspacing="0"><tr><td>';
					// <tr><td class="enroll-location-head" valign="top"><div class="enroll-loc-head"><div class="assn-lrn-left-sec">'+Drupal.t("Location")+':</div></div></td><td></td></tr>
					oHtml += "<div class=\"assn-lrn-left-sec\">"+Drupal.t("Location")+":</div><div class=\"assn-lrn-right-sec\"><div class='enroll-location-text'>"+unescape(LocationName)+"</div>";
					if(data.locationAddr1 !='' && data.locationAddr1 != null && data.locationAddr1 != 'null') {
						oHtml += "<div class='enroll-location-text'>"+unescape(data.locationAddr1)+"</div>";
					}
					if(data.locationAddr2 !='' && data.locationAddr2 != null && data.locationAddr2 != 'null') {
						oHtml += "<div class='enroll-location-text'>"+unescape(data.locationAddr2)+"</div>";
					}
					if(data.locationCity !='' && data.locationCity != null && data.locationCity != 'null') {
						oHtml += "<div class='enroll-location-text'>"+unescape(data.locationCity);
						if (data.locationState == '' && data.locationState != null && data.locationState != 'null') {
							oHtml += "<br />";
						}
					}
					if(data.locationState !='' && data.locationState != null && data.locationState != 'null') {
						if (data.locationCity != '' && data.locationCity != null && data.locationCity != 'null') {
						    oHtml += " , ";
						}
						oHtml += "<div class='enroll-location-text'>"+data.locationState+"</div>";
					}
					
					oHtml += "<div class='enroll-location-text'>"+data.countryName;
					if(data.locationZip !='' && data.locationZip != null && data.locationZip != 'null'){
						oHtml += " - "+data.locationZip;
					}
					oHtml += "</div></div>";
					oHtml += "</td></tr></table>";
				}
		}
	
		oHtml += "</td></tr></table>"; 
		
		$("#class-details-"+data.classId).html(oHtml);
		
		$('#multi-container-pop-up').jScrollPane();
	 }catch(e){
			// to do
		}
	},
	
	adminOrderMultiClassQTip : function(entityId,uniqueId,userIdList){
		try{
		$('#multi-class-container').remove();
		$('#select-order-dialog .ui-dialog').css('top','300px');
		var multiClassContainer = "<div id='multi-class-container' style='display:none;'>" +
		'<table width="750px" height="auto" cellspacing="0" cellpadding="0" class="tp-bubble-popup-multi-register" id="bubble-face-table">' +
			'<tbody>' +
				'<tr>' +
			      	'<td class="bubble-tl"></td>' + 
						'<td class="bubble-t"></td>' +
						'<td class="bubble-tr"></td>' +
				'</tr>' +
		        '<tr>' +
				    '<td class="bubble-cl"></td>' +
				    '<td valign="top" class="bubble-c"><div id="show_expertus_tp_message"></div><div id="multi-select-class-wrapper" class="multi-select-class-wrapper">' +
				    '<a onclick="$(\'#multi-class-container\').hide();$(\'.bottom-qtip-tip-up\').css(\'z-index\',\'100\');var x=$(\'#multi-container-pop-up\').data(\'jsp\');x.destroy();$(\'.grid-refresh\').click();" class="bubble-qtip-button-close"></a>' +
				    '<div id="multi-container-pop-up"  class="multi-class-container-cls scroll-pane"></div></div>' +
				    '<div class="addedit-form-cancel-container-actions"><span class="white-btn-bg-left"></span><span class="admin-action-button-middle-bg white-btn-bg-middle" onclick="$(\'#multi-class-container\').hide();$(\'.bottom-qtip-tip-up\').css(\'z-index\',\'100\');var x=$(\'#multi-container-pop-up\').data(\'jsp\');x.destroy();$(\'.grid-refresh\').click();">Close</span><span class="white-btn-bg-right"></span><div class="admin-save-button-left-bg"></div><input type="button" value="Done" class="admin-save-button-middle-bg" onClick="$(\'body\').data(\'adminlearningcore\').registerTPClassDetailsAdminSide(\''+ entityId +'\',\'admin_order\'); return false;"><div class="admin-save-button-right-bg"></div></div>' +
				    
				    '</td>' +
				    '<td class="bubble-cr"></td>' +
			    '</tr>' +
			    '<tr>' +
			      	'<td class="bubble-bl"></td>' +
			      	'<td class="bubble-b">' +
			        	'<table width="100%" cellspacing="0" cellpadding="0">' +
			          		'<tbody>' +
			            		'<tr>' +
					              '<td class="bubble-blt" style="width:325px;"></td>' +
					              '<td class="bubble-blr" style="width:325px;"></td>' +
					              '<td class="bubble-blm"><div rel="bottomRight" dir="ltr" class="qtip-tip"></div></td>' +
			            		'</tr>' +
			          		'</tbody>' +
			        	'</table>' +
			      	'</td>' +
			      '<td class="bubble-br"></td>' +
			    '</tr>' +
			'</tbody>' +
		'</table></div>';
		$('#'+uniqueId).html(multiClassContainer);
		$('#multi-class-container').show();
		$('.bottom-qtip-tip-up').css('z-index','99');
		$('div.ui-jqgrid-bdiv').eq(0).css('position','');
 		$('div.ui-jqgrid-bdiv div:first-child').css('position','');
 		var position = $('#'+uniqueId).position();
 		var topPos = -400;
 		topPos = topPos + position.top;
 		$('div#multi-class-container').css('top',topPos+'px');
 		this.createLoader("multi-container-pop-up");
		this.getModuleListForTPAdminSide(entityId, 'Y',userIdList,'adminside','');
		hideSearchTP();
		}catch(e){
			// to do
		}
	},
	
	registerTPClassDetailsAdminSide : function(prgId,fromPage,recertifyValue){
		try{
		if(typeof(recertifyValue)== 'undefined') recertifyValue = '';
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
		var errMsg = new Array();
		var message_call;
		
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
				
				if(registeredNsingleCourseClassIds != '') {
					selectedClass = registeredNsingleCourseClassIds;
				}
			
				
				params += '"'+c+'":'+'{';				
				params += '"tpid":"'+prgId+'",';
				params += '"courseid":"'+courseId+'",';
				params += '"classid":"'+selectedClass+'",';
				params += '"recertifyid":"'+recertifyValue+'"';
				
				params += '}';
				
				classIds +=  selectedClass;
				if(inc < courseListIds.length) {
					params += ',';
					classIds += ',';	
				 }

			}
		
			params += '}';

			if(selectedClass1 == "NULL") {
				errMsg[0] = Drupal.t("ERR066");
				message_call = expertus_error_message(errMsg,'error');
				$('#show_expertus_tp_message').html(message_call);
				$('#show_expertus_tp_message').show();
			}else{
					
				var MasterMandatory=$('#assignPrg_checkbox_'+prgId+':checked').val();
				if(MasterMandatory==undefined){
					var multiRegister=$('#multi_register_mandatory').val();
					if(multiRegister=='Y')
					MasterMandatory='Y';
					else
						MasterMandatory='N';
				}
				//console.log('mandatory check',MasterMandatory);
				if(fromPage == 'admin_order'){
					adminMultiTpOrderSelection(classIds);
				}				
				else{
					this.createLoader("multi-container-pop-up");					
				    var url = this.constructUrl("ajax/trainingplan/class-list-register/"+isAdminSide+ "/" + userListIds+ "/" + fromPage+ "/" + MasterMandatory);		
					var obj = this;
					$.ajax({
						type: "POST",
						url: url,
						data:  params,
						datatype: 'text',
						success: function(result){
							/*
							 * Success call function get modified by Vincent for #0071323 on Jan 12, 2017
							 * Earlier the multi-class select popup did not close automatically after the
							 * process successfully completed. This will be changed to close the popup 
							 * if process is successful and reloaded the grid.
							 * 
							 * Also earlier the Registered message was not shown because of the wrong condition
							 * that is also fixed by changing the condition.
							 */
							result = $.trim(result);
							$("#search_attach_tp_crs").click();
						    //if(result == 'Registered'){ 
							if(result.indexOf(Drupal.t('MSG809')) >= 0){
								    $('#multi-class-container').remove();
									$('.grid-refresh').click();
									errMsg[0] = result;
									message_call = expertus_error_message(errMsg,'error');
									$('#show_expertus_message').html(message_call);
									$("#show_expertus_message").show();
								}
							
							if(result.indexOf('Registered') >= 0){
							   $('#multi-class-container').remove();
						    	/*resultMsg = Drupal.t("MSG248");
						    	$("#successMsgDisplay").html(resultMsg);
								$("#successMsgDisplay").css('display','block');
								$("#errMsgDisplay").css('display','none');*/
								$('.grid-refresh').click();
								errMsg[0] = "Registered";
								message_call = expertus_error_message(errMsg,'error');
								$('#show_expertus_message').html(message_call);
								$("#show_expertus_message").show();
								
							}else{
								errMsg[0] = result;
								message_call = expertus_error_message(errMsg,'error');
								if(result == Drupal.t('MSG730')){
									$('#multi-class-container').remove();
									$('.grid-refresh').click();
									$('#show_expertus_message').html(message_call);
									$("#show_expertus_message").show();
								}else{
									$('#show_expertus_tp_message').html(message_call);
									$('#show_expertus_tp_message').show();
								}
							}
						    obj.destroyLoader("multi-container-pop-up");
						//  $('#atach-course-hidden').click();
						    searchCourseTP(prgId,false);
						}
				    });
				}
				
			}
		$('.page-administration-learning-program .bottom-qtip-tip-up-visible').css('z-index','99');	
		}catch(e){
			// to do
		}
	}
	
});

$.extend($.ui.adminlearningcore.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true}});

})(jQuery);


$(function() {
	$("body").adminlearningcore();
});
$(function() {
	try{
		$(".page-administration-learning").sessioninstructor();
	}catch(e){
			// To Do
	}
});
function hideSearchTP() {
	try{
	if ($.browser.msie) {
		if($.browser.version == 7){	
        var chkmulOpen=$('#multi-class-container').css('display');
           if(chkmulOpen=="block") {
        	   $('.admin_add_multi_search_container').hide();
        	   $('#admin-data-grid .ui-jqgrid-toppager .ui-pager-control .ui-pg-table .ui-pg-table').hide();
        	 }   
		}
	}
	}catch(e){
		// to do
	}
}
$('.enrolltp-user-multiregister').live("click",function(){
	try{
		$('#multi_register_mandatory').val('Y');
		}catch(e){
			console.log(e,e.stack);
		}
});

$('#multi-select-class-wrapper .bubble-qtip-button-close').live("click",function() {
	try{
	if ($.browser.msie) {
		if($.browser.version == 7){	
			$('.admin_add_multi_search_container').show();
			$('#admin-data-grid .ui-jqgrid-toppager .ui-pager-control .ui-pg-table .ui-pg-table').show();
	           }   
		}
	}catch(e){
		// to do
	}
});

function copytoclipboard(elementId, buttonId, entity_type) {
	var copyBtn = document.querySelector('#'+buttonId);
	
	copyBtn.addEventListener('click', function(event) {
		var copyText = document.querySelector('#'+elementId);
		copyText.select();
		
		try {
			var successful = document.execCommand('copy');
			copyText.blur();
			if(entity_type == 'cre_sys_obt_cls') {
				$('#sharelink_disp_div #show_expertus_message').hide();
				$('.qtip_survey_sharelink #show_expertus_message').hide();
				$('.qtip-contentWrapper #show_expertus_message').html(expertus_error_message([Drupal.t('MSG836')]));
			} else
				$('#show_expertus_message').html(expertus_error_message([Drupal.t('MSG836')]));
		} catch (err) {
			console.log('Oops, unable to copy');
		}
	});
}