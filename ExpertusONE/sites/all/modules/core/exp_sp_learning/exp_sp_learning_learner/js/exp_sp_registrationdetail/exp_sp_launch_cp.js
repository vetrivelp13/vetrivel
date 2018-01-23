var scoobj;
(function($) {

	$.widget("ui.contentLaunch", {
	_init: function() {
	 try{
		var self = this;
		this.obj  = null;
		var scorm_api_init = null; // variable to store the scorm values when initialized 
	 }catch(e){
			// to do
		}
	},
	launchWBTContent : function(data){
	 try{
		 // console.log('launchWBTContent data');
		//  console.log(data);
//		if(isLaunched || (winobj !== undefined && !winobj.closed)) {
//			$('body').data('learningcore').enrollChangeClassErrorMessage(Drupal.t('Content'), Drupal.t('MSG754'), 1);
//			return false;
//		}
	  //console.log('In launchWBTContent');
	 // console.log(data);
		var pmType 		= data.contentType;
	    var classid 	= data.classId;
	    var courseid 	= data.courseId;
	    var stdid		= this.getLearnerId();
	    var stdname 	= this.getUserName();
	    var pmPath 		= data.LearnerLaunchURL;
	    var contentVersion;
	    var contentType;
	    var contentQuizStatus = data.contentQuizStatus;
	    var finalCallback; 
	    if(stdid == "0" || stdid == "")
	    {
	    	 self.location='?q=learning/enrollment-search';
             return;
	    }
	    if(data.LaunchFrom == 'LP'){
	    if(data.access_denied_flag ==1){
	    	//alert('loader created');
	    	$("#learningplan-tab-inner").data("contentLaunch").createLoader("cp-modalcontainer");
	    	}else{
		    $("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
		    }
	    	$("#learningplan-tab-inner").data("contentLaunch").updateFrom = 'LP';
	    	if(document.getElementById('learner-enrollment-tab-inner')) {
	    		$("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom = '';
	    	}
	    	disableWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');  // Prevent panel close while launch content
	    }
	    else if(data.LaunchFrom == 'CL'){
	    if(data.access_denied_flag ==1)
	    	 $("#lnr-catalog-search").data("contentLaunch").createLoader("cp-modalcontainer");
	    	$("#lnr-catalog-search").data("contentLaunch").updateFrom ='CL';
	    	//52687: Course after completion is not moving to completed status.Launch button is still retailed in Catalog Page
	    	//commented the bellow line bcz of above issue
//	    	if(document.getElementById('learner-enrollment-tab-inner') || document.getElementById('learningplan-tab-inner')) {
//	    		$("#lnr-catalog-search").data("contentLaunch").updateFrom ='';
//	    	}
	    }
	    else{

	       if(data.access_denied_flag ==1)
	    	 $("#learner-enrollment-tab-inner").data("contentLaunch").createLoader("cp-modalcontainer");
	    	else
		     $("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
	    	 $("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom = 'ME';
	    	
	    	if(document.getElementById('learningplan-tab-inner')) {
	    		$("#learningplan-tab-inner").data("contentLaunch").updateFrom = '';
	    	}
	    	disableWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment'); // Prevent panel close while launch content
	    }
	    this.launchParam =  {
	        'launchUrl'			: data.LearnerLaunchURL,
	        'userId' 			: stdid,
	        'username'			: stdname,
	        'courseid' 			: courseid,
	        'classid' 			: classid,
	        'lessonid' 			: data.LessonId,
	        'versionid'			: data.VersionId,
	        'enrollid' 			: data.enrollId,//this.regId,
	        'prevcontentstatus' : data.Status,
	        'LaunchFrom'		: data.LaunchFrom,
	        'aicc_sid'			: data.AICC_SID,
	         'contentId'        :data.contentId,
	    };
	    var newobj = this;
	    if(pmType.toLowerCase() == 'scorm 1.2' || pmType.toLowerCase() == 'scorm 2004') {
	    	pmType				= pmType;
	    	var pmTypeVer 		= pmType.split(' ');
	        contentType  	= pmTypeVer[0];
	        contentVersion 	= pmTypeVer[1];
	        finalCallback = '';
	    } else  {
	        contentVersion 	= '';
	        contentType 	= pmType;
	        finalCallback = newobj.updateScore;
	    }

	    var x	= {version: contentVersion,type:contentType};
	    scoobj 	= new SCORM_API_WRAPPER(x);
	    var lessonlocation=data.LessonLocation==null||data.LessonLocation==undefined ||data.LessonLocation=='null'||data.LessonLocation==''?"":data.LessonLocation;
      var launch_data=data.launch_data==null||data.launch_data==undefined ||data.launch_data=='null'||data.launch_data==''?"":data.launch_data;
      var suspend_data=data.suspend_data==null||data.suspend_data==undefined ||data.suspend_data=='null'||data.suspend_data==''?"":unescape(data.suspend_data);
      var exit=data.exit==null||data.exit==undefined ||data.exit==''?"":data.exit;
      var AICC_SID=data.AICC_SID==null||data.AICC_SID==undefined ||data.AICC_SID==''?"":data.AICC_SID;
      suspend_data = decodeURIComponent(suspend_data);
      lessonlocation = decodeURIComponent(lessonlocation);
	    // When launched from a Launch button of a multi-content dialog, get lessonlocation, launchData, suspendData, exit from Launch button data-relaunch attribute
	    var launchButton = $('#' + data.Id + '_' + data.enrollId + '_launch_button');
	    if (launchButton.length > 0) {
	      //console.log(launchButton.data('relaunch'));
	      var launchButtonData = eval(launchButton.data("relaunch"));
	      if(launchButtonData.length){
	    	  lessonlocation = launchButtonData[0].lessonlocation;
	    	  launch_data = launchButtonData[0].launchData;
	    	  suspend_data = decodeURIComponent(unescape(launchButtonData[0].suspendData));
	    	  exit = launchButtonData[0].exit;
	      }
	      //var launchButtonData = eval('(' + decodeURIComponent(launchButton.data('relaunch')) + ')');
	      //console.log(launchButtonData);
	      //console.log(typeof launchButtonData);
	    }
	    //console.log('lessonlocation = ' + lessonlocation);
	    //console.log('launch_data = ' + launch_data);
	    //console.log('suspend_data = ' + suspend_data);
	    //console.log('exit = ' + exit);
		if(checkEncodedState(suspend_data)==false){
        	//suspend_data=encodeURIComponent(suspend_data);
        	//do nothing
        }else{
        	 suspend_data = decodeURIComponent(suspend_data);
        }
		
		if(checkEncodedState(lessonlocation)==false){
        	//lessonlocation=encodeURIComponent(lessonlocation);
        	//do nothing
        }else{
        	lessonlocation = decodeURIComponent(lessonlocation);
        }
		
		    if(data.Id == undefined)
	    		var cdnurl = '?q=ajax/getcdnurl/'+data.LessonId;
			else
				var cdnurl = '?q=ajax/getcdnurl/'+data.Id;
		    $.ajax({
				url: cdnurl,
				// data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){

			//	if(data.access_denied_flag!=1){
					if(document.getElementById('learningplan-tab-inner'))
						$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
					if(document.getElementById('learner-enrollment-tab-inner'))
						$("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
					if(document.getElementById('lnr-catalog-search'))
						$("#lnr-catalog-search").data("enrollment").destroyLoader('enroll-result-container');
				//	}	
					var output = result.split('~~');
					url1      = output[0];
					cdnstatus = output[1];

					if(scoobj._type.toLowerCase() == 'scorm') {
						scorm_api_init = new Object();
						scorm_api_init.session_time = '0000:00:00.00';
						scorm_api_init.lesson_location = lessonlocation;
						scorm_api_init.launch_data = launch_data;
						scorm_api_init.suspend_data = suspend_data;
						scorm_api_init.version = scoobj._version;
						if(scoobj._version == '2004') {
							scorm_api_init.completion_status = 'not attempted';
							scorm_api_init.success_status = 'unknown';
						} else {
							scorm_api_init.lesson_status = 'not attempted';
						}
					}
					
					if(cdnstatus == 1 && data.subtype != 'h5p-presentatn'){ //Presentation need not play from CDN server. Only the video interactions in presentation will be played from CDN server.
						var data1 = {
						        url				 : url1,
						        callback		 : finalCallback,
						        idleCallback	 : newobj.updateScore, // used only in content player
						        learnerId		 : stdid,
						        learnerName		 : stdname,
						        completionStatus : 'not attempted',
						        successStatus	 : 'unknown',
						        scoreMax		 : '0',
						        scoreMin		 : '0',
						        score			 : '0',
						        location		 : lessonlocation,
						        courseid 		 : courseid,
						        classid 		 : classid,
						        lessonid 		 : data.LessonId,
						        versionid		 : data.VersionId,
						        enrollid 		 : data.enrollId,
						       
						        launch_data		 : launch_data,
						        suspend_data     : suspend_data,
						        exit			 : exit,
						        aicc_sid		 : AICC_SID,
						        MasteryScore	 : data.masteryscore,
						    };
						    scoobj.Initialize(data1);
					} else {
	    var data1 = {
	        url				 : pmPath,
	        callback		 : finalCallback,
	        idleCallback	 : newobj.updateScore, // used only in content player
	        learnerId		 : stdid,
	        learnerName		 : stdname,
	        contentQuizStatus: data.contentQuizStatus,
	        completionStatus : 'not attempted',
	        access_denied_flag : data.access_denied_flag,
	       
	        successStatus	 : 'unknown',
	        scoreMax		 : '0',
	        scoreMin		 : '0',
	        score			 : '0',
	        location		 : lessonlocation,
	        courseid 		 : courseid,
	        classid 		 : classid,
	        lessonid 		 : data.LessonId,
	        versionid		 : data.VersionId,
	        enrollid 		 : data.enrollId,
	        launch_data		 : launch_data,
	        suspend_data     : suspend_data,
	        exit			 : exit,
	        aicc_sid		 : AICC_SID,
	        MasteryScore	 : data.masteryscore,
	    };
	    // console.log('launch js----------------------------');
	    // console.log(suspend_data);
	    scoobj.Initialize(data1);
					}
				}
		    });
	 }catch(e){
			// to do
		}
	},

	updateScore : function(callback_param, callbackData){
	 try{
		 // console.log('updateScore called');
		 // console.log('callback_param : ' + callback_param);
		 /*
		  * SCORM 1.2 :
	         	result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
	         	result.status = API.LMSGetValue("cmi.core.lesson_status");
		 	SCORM 2004:
     			//SCORM 2004 will have 2 statuses - both should be validated
		         result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
		         result.status = API_1484_11.GetValue("cmi.success_status");				//quiz status
		  */
		var setObj;
		var updateFrom;
		var contentPlayerObj;
		var access_denied_flag = '';
		if(callback_param!=undefined){
		var access_denied_flag = callback_param.access_denied_flag;
		}
		

		if(document.getElementById('learningplan-tab-inner')) {
			if($("#learningplan-tab-inner").data("contentLaunch").updateFrom == 'LP'){
				setObj = $("#learningplan-tab-inner").data("contentLaunch");
				contentPlayerObj = $("#learningplan-tab-inner").data("contentPlayer");
				updateFrom = 'LP';
			}
			activateWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');
		}
		if(document.getElementById('learner-enrollment-tab-inner')) {

			if($("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom == 'ME'){
				setObj = $("#learner-enrollment-tab-inner").data("contentLaunch");
				contentPlayerObj = $("#learner-enrollment-tab-inner").data("contentPlayer");
				updateFrom = 'ME';
			}
			activateWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment');
		}
		if(document.getElementById('lnr-catalog-search')){
			if($("#lnr-catalog-search").data("contentLaunch").updateFrom == 'CL'){
				setObj = $("#lnr-catalog-search").data("contentLaunch");
				updateFrom = 'CL';
			}
			if($("#lnr-catalog-search").data("contentLaunch").updateFrom == 'ME'){

				setObj = $("#lnr-catalog-search").data("contentLaunch");
				updateFrom = 'ME';
			}
			contentPlayerObj = $("#lnr-catalog-search").data("contentPlayer");
	    }
	
	    
        var sestime					= '0';
        var courseComplitionStatus 	= '';//$('#courseComplitionStatus').val();
        var lObj	 				="'"+setObj+"'";
        var launchflag              = 0;
        var crid					= setObj.launchParam.courseid;
        var clid					= setObj.launchParam.classid;
        var lesid					= setObj.launchParam.lessonid;
        var versionid				= setObj.launchParam.versionid;
        var enrollid				= setObj.launchParam.enrollid;
        var stid 					= setObj.launchParam.userId;
        var prevcontentstatus 		= setObj.launchParam.prevcontentstatus;
        var regId					= setObj.launchParam.enrollid;
        var aicc_sid				= setObj.launchParam.aicc_sid;
        var contentType				= "scorm 1.2";
        var contentVersion			= '';
        var result					= (callback_param !== undefined && (scoobj._type.toLowerCase() == 'scorm' || scoobj._type.toLowerCase() == 'knowledge content')) ? callback_param : scoobj.Finish(); // CP need to check
        var status 					= '';
        var scmax 					= '';
        var scmin 					= '';
        var score 					= '';
        //var sestimear 				= '';
        var loc 					= '';
        var compstatus				= '';
        var launch_data		 		= '';
        var suspend_data     		= '';
        var exit			 		= '';
        var grade					= '';
        var xstatus					= '';
        if(typeof(result) != undefined && typeof(result) != 'undefined'){
	        status 					= result.status;
	        score 					= result.score;
	        sestime 				= result.sessionTime;
	        loc 					= result.location;
	        compstatus				= result.completionStatus;
	        launch_data		 		= result.launch_data;
	        suspend_data     		= result.suspend_data;
	        exit			 		= result.exit;
	        launchflag				= (callback_param !== undefined) ? result.launchflag : 0;
        }
//    alert('type'+scoobj._type);
//       alert('version'+scoobj._version);
//       alert('quiz'+status);
//       alert('conet'+compstatus);
        //console.log(scoobj._type);
		var is_scorm_api_changed = true;
        if(scoobj._type != "Knowledge Content" && scoobj._type != "Tin Can") {
        	if(prevcontentstatus !='Completed')	{
        		if(scoobj._type.toLowerCase() == 'scorm' && scoobj._version == '2004') {
        			var completion_status = compstatus.toLowerCase();
        			var success_status = status.toLowerCase();

        			if(completion_status == 'completed') {
        				xstatus='lrn_crs_cmp_cmp';
        			}
        			else if(completion_status == 'unknown' && (success_status == 'unknown' || success_status == 'passed')){
        				xstatus='lrn_crs_cmp_cmp';
        			}
        			else {
        				xstatus='lrn_crs_cmp_inp';
        			}
        		}
        		else {
        			if(status.toLowerCase() == "passed" || status.toLowerCase() == "failed" || status.toLowerCase() == "completed" || status.toLowerCase() == "unknown"){
    		        	xstatus='lrn_crs_cmp_cmp';
        			}else{
    		        	xstatus='lrn_crs_cmp_inp';
        			}
        		}

        	}
        	else{
        	xstatus='lrn_crs_cmp_cmp';
        	}
			if(scoobj._type.toLowerCase() == 'scorm') {
				if(scoobj._version == '2004') {
					// scorm_api_init = new Object();
					//comparing initialized values with the current scorm result
					if(compstatus == 'not attempted' && scorm_api_init.session_time == sestime && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data && scorm_api_init.success_status == status) {
						is_scorm_api_changed = false;
					}
				} else {
					//comparing initialized values with the current scorm result
					if(status  == 'not attempted' && scorm_api_init.session_time == sestime && scorm_api_init.lesson_location == loc && scorm_api_init.launch_data == launch_data && scorm_api_init.suspend_data == suspend_data) {
						is_scorm_api_changed = false;
					}
				}
			}
        }else if(scoobj._type == "Tin Can"){
        	xstatus='lrn_crs_cmp_inp';
        }
		else if(scoobj._type == "Knowledge Content") {
			var index = setObj.launchParam.launchUrl.lastIndexOf("/") + 1;
			var filename = setObj.launchParam.launchUrl.substr(index);
		    var extension = filename.substr( (filename.lastIndexOf('.') +1) );
		    extension = extension.toLowerCase();
		    if(extension == "pdf"){
		    	var total_page = document.getElementById('cp-frame-container').contentWindow.PDFViewerApplication.pagesCount;
	    		var current_page = document.getElementById('cp-frame-container').contentWindow.PDFViewerApplication.page;   	
				if(status.toLowerCase()  == "completed" && total_page == current_page) {
					xstatus='lrn_crs_cmp_cmp';
				}
				else {
					xstatus='lrn_crs_cmp_inp';
				}
		    	
		    }else{
		    	if(status.toLowerCase()  == "completed") {
					xstatus='lrn_crs_cmp_cmp';
				}
				else {
					xstatus='lrn_crs_cmp_inp';
				} 
		    }
		}else{	
        	xstatus='lrn_crs_cmp_cmp';
        	status="Completed";
        	var total_page = '';
    		var current_page = '';
        }
        //console.log('xstatus'+xstatus);
//        alert('xstatus'+xstatus);
        //pending values from wrapper completionStatus totalTime
        if(score == undefined || score == '') {
            score='0';
            grade='';
        };
    
        
     // ADDING FOR PRESENTATION KNC
      var h5p_cnt_id = "";
  	  var type = "";
  	  var MaxAttempt = "";
  	  var AttemptLeft = "";
  	  
  	  var source = $("#cp-frame-container").attr("src");
      if(source != null && source != undefined && ($("#cp-menucontainer").attr("subtype") != undefined && $("#cp-menucontainer").attr("subtype") == "h5p-presentatn"))
  	  {
    	  MaxAttempt   =   $("#cp-menucontainer").attr("MaxAttempt");
    	  AttemptLeft  =   $("#cp-menucontainer").attr("AttemptLeft");
  	  	  h5p_cnt_id   =   $("#cp-frame-container").contents().find(".h5p-content").data("content-id");
  	  	  type         =   "h5p";
  	  }
        
        
        $('#'+lesid+'_launch_score').html(score);
        $('#'+lesid+'launch_grade').html(grade);

        $('#'+lesid+'_launch_status').html(xstatus);

        $('#surveybutton_'+clid).css("display","block");
        $('#lanchlinks1_'+clid+'').click();
        var launchData	=	{
        		'regid'		: regId,
                'stid'		: stid,
                'classid'	: clid,
                'courseid'	: crid,
                'lessonid'	: lesid,
                'versionid' : versionid,
                'status'	: xstatus,
                'max'		: scmax,
                'min'		: scmin,
                'score'		: score,
                'grade'		: grade,
                'sestime'	: sestime,
                'location'	: loc,
                'enrollid'	: enrollid,
                'courseComplitionStatus' : courseComplitionStatus,
                'launch_data'	 : launch_data,
    	        'suspend_data'   : encodeURIComponent(suspend_data),
    	        'exit'			 : exit,
    	        'contentstatus'  : status,		//for SCORM 1.2 => lesson_status and for 2004 => success_status
    	        'compstatus'	 : compstatus,	//for SCORM 1.2 => '' and for 2004 => completion_status
				'contenttype'	 : scoobj._type,
				'contentversion' : scoobj._version,
    	        'aicc_sid'		 : aicc_sid,
				'is_scorm_api_changed' : is_scorm_api_changed,
				'launchflag' : launchflag,
				'playfrom' : 'contentplayer',
				'total_page' : total_page,
		        'current_page' : current_page,
		        'launchUrl'   : setObj.launchParam.launchUrl,
		        'h5p_cnt_id'  : h5p_cnt_id,
		        'type'        : type,
		        'MaxAttempt' : MaxAttempt,
		        'AttemptLeft' : AttemptLeft
          };
        
        if($("#cp-menucontainer").attr("subtype") == "h5p-presentatn" && source != null && source != undefined && ($("#cp-menucontainer").attr("subtype") != undefined ))
    	 {
        //0084380 and 0083873 :to support store the state when close the content window
        document.getElementById("cp-frame-container").contentWindow.H5P.storeCurrentState();
        if(launchflag == 0){
           $("#cp-frame-container").attr("src","");
        }
       }
        
        var passUrl = 'launch&regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&versionid='+versionid+'&status='+xstatus+'&max='+scmax+'&min='+scmin;
        passUrl = passUrl+'&score='+score+'&grade='+grade+'&sestime='+sestime+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseComplitionStatus;
        passUrl = passUrl+'&contentstatus='+status+'&launch_data='+launch_data+'&suspend_data='+suspend_data+'&exit='+exit+'&aicc_sid='+aicc_sid;

        var url='';
        var post_data='';
        if(scoobj._type == "AICC" || scoobj._type == "AICC Course Structure") {
        	var sid = aicc_sid;
        	passUrl="session_id="+sid+"&command=UPDATELMSDATA";
        	url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
        	post_data={"session_id":callback_param,"command":"UPDATELMSDATA",'launchflag' : launchflag};
        }else if(scoobj._type == "Tin Can"){
        	var sid = aicc_sid;
        	passUrl="Authorization="+sid+"&command=UPDATELMSDATA";
        	url = resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";
        	post_data={"Authorization=":callback_param,"command":"UPDATELMSDATA",'launchflag' : launchflag};
        }else{
        	url = resource.base_url+'/?q='+"ajax/update-launch/launch";
        	post_data = launchData;
        }
        if(launchflag == 0){
	        if(document.getElementById('launch-wizard')) {
	        	$('#launch-wizard').css("overflow","hidden");
	        	if($("#cp-modal").css('display') == 'none')
	        	setObj.createLoader('launch-wizard');
	        }
	        if(document.getElementById('launch-lp-wizard')) {
	        	$('#launch-lp-wizard').css("overflow","hidden");
	        	if($("#cp-modal").css('display') == 'none')
	        	setObj.createLoader('launch-lp-wizard');
	        }
	        if(document.getElementById('lnr-catalog-search') && ($("#cp-modal").css('display') == 'none')){
	          	$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
	          	if(document.getElementById("course-details-display-content"))
	          		$("#course-details-display-content").data("lnrcoursedetails").createLoader('class-list-loader');
	          	if(document.getElementById("tbl-paintCatalogContentResults"))
	    			$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
	        }
	    }
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			data:  post_data,
			success: function(result) {

					//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
					 if(launchflag == 1 ){ 
					// alert(34555);// cp need to check
					 	contentPlayerObj.refreshContentProgressBarLine(contentPlayerObj,result);
					 //	 $("#"+callbackData.launchObject).data('contentPlayer').playNextContent(callbackData);
					
						return;
					 }
					  if(access_denied_flag==1){
					  contentPlayerObj.refreshContentProgressBarLine(contentPlayerObj,result);
					// console.log(setObj.launchParam);
					 $('#selconetentmenulink-lContent-'+setObj.launchParam.contentId).click();
				
					 }
			
				//	 scoobj = new Object();
					 // console.log('player callback called in updatescore wbt');
					 if (callbackData != undefined && callbackData != null && !callbackData.closed) {
						 $("#"+callbackData.launchObject).data('contentPlayer').playNextContent(callbackData);
						 if(document.getElementById('lnr-catalog-search')) {
							 $("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
						 }
						 return;
					 }
					//console.log('resr'+result);
					if(document.getElementById('learner-enrollment-tab-inner')) {
					  // extra parameters passed to grid for ticket #0020086, which will update the score, status, etc in client
					  // Only do this when multi content launch dialog is present in DOM
					  if (document.getElementById('launch-content-container')) {
					    //console.log('reloading grid with lesson id and enrollment id');
						  $("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'updateFrom':updateFrom}});
						  
						  if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
								$("#paintEnrollmentResults").trigger("reloadGrid");
							}
					  }
					  else {
					    //console.log('reloading grid without lesson id and enrollment id as no launch-content-container');
						  $("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'updateFrom':updateFrom}});
						   if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
								$("#paintEnrollmentResults").trigger("reloadGrid");
							}
					  }
					  setObj.destroyLoader('launch-wizard');
					  $('#launch-wizard').css("overflow","auto");
					}
					if(document.getElementById('learningplan-tab-inner')) {
						masterEnrId = 0;
						if(document.getElementById('lp_seemore_prg_'+regId))
							masterEnrId = regId;
						else if(document.getElementById('lp_class_seemore_'+regId)){
							var parenttableId=$('#lp_class_seemore_'+regId).parents('table.enroll-show-moreclass').attr('id');
							var idArr = parenttableId.split('-');
							if(parenttableId!==undefined)
								masterEnrId = idArr[1];
						}
						 $("#paintEnrollmentLPResults").data('lastPostData', {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom});
					  // extra parameters passed to grid for ticket #0020086, which will update the status, etc in client
					  // Only do this when multi content launch dialog is present in DOM
					  if (document.getElementById('lplaunch-content-container')) {
					    //console.log('reloading LP grid with lesson id and enrollment id');
						$("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom}});
						  if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
								 $("#paintEnrollmentLPResults").trigger("reloadGrid");
							 }
					  }
					  else {
					    //console.log('reloading LP grid without lesson id and enrollment id as no launch-content-container');
						  $("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom}});
						  if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
								 $("#paintEnrollmentLPResults").trigger("reloadGrid");
							 }
					  }
					  setObj.destroyLoader('launch-lp-wizard');
					  $('#launch-lp-wizard').css("overflow","auto");
					}
					if(document.getElementById('lnr-catalog-search')) {
						if(document.getElementById('launch-wizard'))
							setObj.createLoader('launch-wizard');
		            	$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
						url = setObj.constructUrl("ajax/learningcore/register-to-launch/" + stid + '/' + clid + '/' + crid+'/'+regId+'/lrn_cls_dty_wbt/1/'+lesid);
						$.ajax({
							type: "POST",
							url: url,
							//data:  '',//Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
							success: function(result){
								if(result){
									result_arr = result.split('*~*');
									var launched_info = $.parseJSON(result_arr[1]);
									var lessonCnt = result_arr[2];
									var regId_progress = '';
									if(Drupal.settings.convertion.mylearn_version==1){
										var progress = result_arr[3];
										var launch_staus = result_arr[4];
										var regId_progress = regId+'*~*'+progress+'*~*'+launch_staus;
									}
									$("#lnr-catalog-search").data('enrollment').updateMultiContentLaunchDialog(regId,lesid,launched_info.triggering_lesson_details,launched_info.triggering_content_quiz_status);
									var html_out = '<div '+result_arr[0]+'</div>';
									if(document.getElementById('launch'+regId)){
										$('#launch'+regId).replaceWith(html_out);
										Drupal.attachBehaviors($('#launch'+regId));
									}
									if(document.getElementById('registerCls_'+clid)){
										$('#registerCls_'+clid).replaceWith(html_out);
										Drupal.attachBehaviors($('#registerCls_'+clid));
									}
									paintContentStatus(launched_info, crid, clid, '',lessonCnt,regId_progress);
								}
								$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
								if(document.getElementById('launch-content-container'))
									$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
								if(document.getElementById("tbl-paintCatalogContentResults")){
									if(document.getElementById('launch'+regId)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
									}else if(document.getElementById('registerCls_'+clid)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
									}
				        			//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
									$("#tbl-paintCatalogContentResults").trigger("reloadGrid");
				        		}
								if(document.getElementById('paintContentResults')){
									if(document.getElementById('launch'+regId)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
									}else if(document.getElementById('registerCls_'+clid)){
										$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
									}
									//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
									if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
										$("#paintContentResults").trigger("reloadGrid");
									}
								}
								if(document.getElementById('launch-wizard'))
									setObj.destroyLoader('launch-wizard');
								if(document.getElementById("course-details-display-content")){
									$('#class_content_more_'+clid).hide();
									$('#class_content_moredetail_'+clid).show();
									//$('#paindContentclsid_'+clid).click();
							    	$("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
								}else if(document.getElementById("class_detail_content")){
									$('#paindContentclsid_'+clid).click();
			                  		$("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
								}else if(document.getElementById('learning-plan-details-display-content')){
									 $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+clid);
								}
							}
					    });

					}
					// Commented status check for Tincan content #0047307 - issue 1
					//if(xstatus == "lrn_crs_cmp_cmp" && document.getElementById("my_transcript_jqgrid")) {
					if(document.getElementById("my_transcript_jqgrid")){
						$("#my_transcript_jqgrid").trigger('reloadGrid');
					}
					for (var line_item in result) {
						if (line_item == 0) {
							if (result[line_item].CompMessage == 'Attempts Over') {
								callLaunchMessageWindow(Drupal.t('LBL929'), Drupal.t('MSG915'));
							}
						}
					}
			}
	    });
	
	 	}catch(e){
			// to do
	 		// console.log(e)
		}
	  },


	  updateVODScoreOnCtoolsModalClose : function (launchedFrom, courseId, classId, lessonId, versionId, enrollId, prevContentStatus, contentProgress, callbackData) {
	  try{
		  //console.log(this);
		  // console.log('updateVODScoreOnCtoolsModalClose');
		  // console.log('expjwPlayerSettings position');
		  // console.log(expjwPlayerSettings.getPosition());
		  // console.log(jwplayer().getPosition());
      var setObj;
      var updateFrom;
      var contentPlayerObj;
      if(launchedFrom == 'LP'){
          setObj = $("#learningplan-tab-inner").data("contentLaunch");
          contentPlayerObj = $("#learningplan-tab-inner").data("contentPlayer");
          updateFrom = 'LP';
          $('#launch-lp-wizard').parent().css('display', 'block');
          activateWidgetDeleteAction('block-exp-sp-lnrlearningplan-tab-my-learningplan');
		   // create loaders
		  if(launchflag == 0){
			  if(document.getElementById('launch-lp-wizard')) { //for multi content launch my-programs tab
				setObj.createLoader('launch-lp-wizard');
				$('#launch-lp-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-lp-result-container')) {
				  setObj.createLoader('enroll-lp-result-container');
			  }
		 }
      }
	  else if(launchedFrom == 'ME'){
		  if(document.getElementById('lnr-catalog-search')){
			  setObj = $("#lnr-catalog-search").data("contentLaunch");
			  contentPlayerObj = $("#lnr-catalog-search").data("contentPlayer");
		  }
		  else{
          	setObj = $("#learner-enrollment-tab-inner").data("contentLaunch");
          	contentPlayerObj = $("#learner-enrollment-tab-inner").data("contentPlayer");
          }
          updateFrom = 'ME';
         $('#class_detail_content #launch-wizard').show();
          $('#launch-wizard').parent().css('display', 'block');
          activateWidgetDeleteAction('block-exp-sp-lnrenrollment-tab-my-enrollment');
          if(launchflag == 0){
			  if(document.getElementById('launch-wizard')) {	//for multi content launch my-enrollments tab
				if($("#cp-modal").css('display') == 'none')
					setObj.createLoader('launch-wizard');
				$('#launch-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-result-container')) {
				  // console.log("test enroll ui");
				  if($("#cp-modal").css('display') == 'none')
					  setObj.createLoader('enroll-result-container');
			  }
		 }
      }

      var crid                  = parseInt(courseId);
      var clid                  = parseInt(classId);
      var lesid                 = parseInt(lessonId);
      var versionid             = parseInt(versionId);
      var enrollid              = parseInt(enrollId);
      var stid                  = this.getLearnerId();
      var regId                 = parseInt(enrollId);

      var status          = '';
      var xstatus         = '';
      if(stid == "0" || stid == "")
	  {
	   	 self.location='?q=learning/enrollment-search';
         return;
	  }
      //0021160: Overriding the existing content status when launch the VOD
      //if(prevContentStatus != 'Completed') {
            status = 'Completed';
            xstatus='lrn_crs_cmp_cmp';
      //}

      $('#'+lesid+'_launch_status').html(xstatus); // No _launch_status in DOM

      $('#surveybutton_'+clid).css("display","block");  // No surveybutton_ in DOM
      $('#lanchlinks1_'+clid+'').click(); // No launchlinks1_ in DOM
	  var video_suspend_data = null;
	  if(typeof videoTrackerProgress != "undefined" && videoTrackerProgress != null) {
		videoTrackerProgress.progress = Math.round((videoTrackerProgress.current_position / videoTrackerProgress.video_duration) * 100);
		video_suspend_data = videoTrackerProgress;
	  }
	  if(typeof(ytPlayer) != 'undefined') {
		// window.ytPlayer.stopVideo();
		window.ytPlayer.destroy();
		clearInterval(interval);
		clearInterval(ajaxInterval);
	  }
	  var h5p_cnt_id = "";
	  var type = "";
	  var maxAttempt = "",attemptLeft = "";
	  if($("#h5pplayer").size() > 0)
	  {
	  		h5p_cnt_id = $("#h5pplayer").contents().find(".h5p-content").data("content-id");
	  		maxAttempt = $("#h5pplayer").attr("MaxAttempt");
	  		attemptLeft = $("#h5pplayer").attr("AttemptLeft");
	  		type = "h5p";
	  	//	alert(maxAttempt);
	  	//	alert(attemptLeft);
	  		
	  }
	  var launchflag = 0;
	  if(callbackData.launchflag == 1)
	  	launchflag = 1;
      var launchData  = {
          'regid'   : regId,
          'stid'    : stid,
          'classid' : clid,
          'courseid'  : crid,
          'lessonid'  : lesid,
          'versionid' : versionid,
          'enrollid'  : enrollid,
          'prev_content_status'	: prevContentStatus,
          'video_suspend_data'	: JSON.stringify(video_suspend_data),
          'playfrom'  : 'contentplayer',
          'contenttype'  : 'Video',
          'h5p_cnt_id':h5p_cnt_id,
          'type':type,
          'MaxAttempt' : maxAttempt,
          'AttemptLeft' : attemptLeft,
          'launchflag' : launchflag
        };
      
      if($("#h5pplayer").size() > 0)
	  {
      //0084380 and 0083873 : to support store the state when close the content window
      document.getElementById("h5pplayer").contentWindow.H5P.storeCurrentState();
	  if(launchflag == 0)
    	  $("#h5pplayer").remove();
	  }
      
	  
      url = resource.base_url + '/?q=ajax/update-launch/launch';
      post_data = launchData;
 	  
      var obj = this;
      $.ajax({
        type: "POST",
        url: url,
        data:  post_data,
        success: function(result){
			//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
			 if(launchflag == 1){ // cp need to check
			 	contentPlayerObj.refreshContentProgressBarLine(contentPlayerObj,result);
				return;
			 }
			 // console.log('player callback called in VOD');
			 if (callbackData != undefined && callbackData != null && !callbackData.closed) {
				 $("#"+callbackData.launchObject).data('contentPlayer').playNextContent(callbackData);
				 return;
			 }
			videoTrackerProgress = null;
			if(typeof(ytPlayer) != 'undefined') {
				ytPlayer = undefined;
				clearInterval(interval);
				clearInterval(ajaxInterval);
			}
			disposeVideoJSPlayer('all');
            for(i=0; i < result.length; i++) {
              if(result[i].ID == parseInt(lesid)) {
                $("#" + lesid + "_" + regId + "attempts_left").html(result[i].contValidateMsg);
              }
            }
			// destroy loaders
			if(launchedFrom == 'LP'){
			  if(document.getElementById('launch-lp-wizard')) { //for multi content launch my-programs tab
				setObj.destroyLoader('launch-lp-wizard');
				$('#launch-lp-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-lp-result-container')) {
				  setObj.destroyLoader('enroll-lp-result-container');
			  }
			}
			else if(launchedFrom == 'ME'){
			  if(document.getElementById('launch-wizard')) {	//for multi content launch my-enrollments tab
				setObj.destroyLoader('launch-wizard');
				$('#launch-wizard').css('overflow-x', 'hidden');
			  } else if(document.getElementById('enroll-result-container')) {
				 setObj.destroyLoader('enroll-result-container');
			  }
			}
            if(document.getElementById('lnr-catalog-search')) {
            	if(document.getElementById('launch-wizard')){
            		$('#launch-wizard').css("overflow","hidden");
            		obj.createLoader('launch-wizard');
            	}
        		$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
        		url = obj.constructUrl("ajax/learningcore/register-to-launch/" + stid + '/' + clid + '/' + crid+'/'+regId+'/lrn_cls_dty_vod/1/'+lesid);
				$.ajax({
					type: "POST",
					url: url,
					//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
					success: function(result){
						if(result){
							result_arr = result.split('*~*');
							var launched_info = $.parseJSON(result_arr[1]);
							var lessonCnt = result_arr[2];
							var regId_progress = '';
							if(Drupal.settings.convertion.mylearn_version==1){
								var progress = result_arr[3];
								var launch_staus = result_arr[4];
								var regId_progress = regId+'*~*'+progress+'*~*'+launch_staus;
							}
							$("#lnr-catalog-search").data('enrollment').updateMultiContentLaunchDialog(regId,lesid,launched_info.triggering_lesson_details,launched_info.triggering_content_quiz_status);
							var html_out = '<div '+result_arr[0]+'</div>';
							if(document.getElementById('launch'+regId)){
								$('#launch'+regId).replaceWith(html_out);
								Drupal.attachBehaviors($('#launch'+regId));
							}
							if(document.getElementById('registerCls_'+clid)){
								$('#registerCls_'+clid).replaceWith(html_out);
								Drupal.attachBehaviors($('#registerCls_'+clid));
							}
							paintContentStatus(launched_info, crid, clid, '',lessonCnt,regId_progress);
						}
						if(document.getElementById('launch-content-container'))
							$("#lnr-catalog-search").data("enrollment").scrollBarContentDialog();
						if(document.getElementById("tbl-paintCatalogContentResults")){
							if(document.getElementById('launch'+regId)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
							}else if(document.getElementById('registerCls_'+clid)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
							}
		        			//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('paintCatalogContentResults'+stid);
							$("#tbl-paintCatalogContentResults").trigger("reloadGrid");
		        		}
						if(document.getElementById('paintContentResults')){
							if(document.getElementById('launch'+regId)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('launch'+regId);
							}else if(document.getElementById('registerCls_'+clid)){
								$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('registerCls_'+clid);
							}
							//$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
							if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
								$("#paintContentResults").trigger("reloadGrid");
							}
						}
						$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
						if(document.getElementById('launch-wizard'))
							obj.destroyLoader('launch-wizard');
							$('#class_detail_content #launch-wizard').hide();
						if(document.getElementById("course-details-display-content")){
							$('#class_content_more_'+clid).hide();
							$('#class_content_moredetail_'+clid).show();
							//$('#paindContentclsid_'+clid).click();
					    	$("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
						}else if(document.getElementById("class_detail_content")){
							$('#paindContentclsid_'+clid).click();
	                  		$("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
						}else if(document.getElementById('learning-plan-details-display-content')){
							 $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+clid);
						}
					}
			    });
            }
            if(document.getElementById('learner-enrollment-tab-inner')) {
            	if (document.getElementById('launch-content-container')) {
				    //console.log('reloading grid with lesson id and enrollment id');
            		$("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'updateFrom':updateFrom}});
					if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
						$("#paintEnrollmentResults").trigger("reloadGrid");
					}
				  }
				  else {
				    //console.log('reloading grid without lesson id and enrollment id as no launch-content-container');
					  $("#paintEnrollmentResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'updateFrom':updateFrom}});
					  if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
					  	$("#paintEnrollmentResults").trigger("reloadGrid");
					  }
				  }
            }
            if(document.getElementById('learningplan-tab-inner')) {
            	masterEnrId = 0;
				if(document.getElementById('lp_seemore_prg_'+regId))
					masterEnrId = regId;
				else if(document.getElementById('lp_class_seemore_'+regId)){
					var parenttableId=$('#lp_class_seemore_'+regId).parents('table.enroll-show-moreclass').attr('id');
					var idArr = parenttableId.split('-');
					if(parenttableId!==undefined)
						masterEnrId = idArr[1];
				}
				$("#paintEnrollmentLPResults").data('lastPostData', {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom});
            	if (document.getElementById('lplaunch-content-container')) {
				    $("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom}});
					if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
						$("#paintEnrollmentLPResults").trigger("reloadGrid");
					}
				}
				else {
				   $("#paintEnrollmentLPResults").setGridParam({postData: {'lessonId': lesid, 'enrollmentId': regId,'lesId': lesid, 'enrId': regId,'masterEnrId':masterEnrId,'updateFrom':updateFrom}});
				   if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
						$("#paintEnrollmentLPResults").trigger("reloadGrid");
				   }
				}
            }

            if(xstatus == "lrn_crs_cmp_cmp" && document.getElementById("my_transcript_jqgrid")) {
              $("#my_transcript_jqgrid").trigger('reloadGrid');
            }
			for (var line_item in result) {
				if (line_item == 0) {
					if (result[line_item].CompMessage == 'Attempts Over') {
						callLaunchMessageWindow(Drupal.t('LBL929'), Drupal.t('MSG915'));
					}
				}
			}
        }
        });
	  }catch(e){
			// to do
		  // console.log(e);
	  }
    }
});

$.extend($.ui.contentLaunch.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

var obj;

$(function() {
	try{
	if(document.getElementById('learner-enrollment-tab-inner')) {
		$("#learner-enrollment-tab-inner").contentLaunch();
	}
	if(document.getElementById('learningplan-tab-inner')) {
		$("#learningplan-tab-inner" ).contentLaunch();
	}
	if(document.getElementById('lnr-catalog-search')) {
		$("#lnr-catalog-search" ).contentLaunch();
	}
	}catch(e){
		// to do
	}
});
function callLaunchMessageWindow(title, message) {
	try {
		if (title == 'registertitle') {
			title = Drupal.t('LBL721');
		}

		$('#commonMsg-wizard').remove();
		var html = '';
		html += '<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
		html += '<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		html += '<tr><td height="30"></td></tr>';
		html += '<tr>';
		html += '<td align="center"><span class="select-greyed-out-text">' + SMARTPORTAL.t(message) + '</span></td>';
		html += '</tr>';
		html += '</table>';
		html += '</div>';
		$("body").append(html);
		var closeButt = {};
		closeButt[Drupal.t('LBL123')] = function () {
			$(this).dialog('destroy');
			$(this).dialog('close');
			$('#commonMsg-wizard').remove();
		};
		$("#commonMsg-wizard").dialog({
			position: [(getWindowWidth() - 500) / 2, 100],
			bgiframe: true,
			width: 520,
			resizable: false,
			modal: true,
			title: SMARTPORTAL.t(title),
			buttons: closeButt,
			closeOnEscape: false,
			draggable: true,
			overlay: {
				opacity: 0.9,
				background: "black"
			}
		});
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if ($.browser.msie && parseInt($.browser.version, 10) == '9' && this.currTheme == "expertusoneV2" || $.browser.msie && parseInt($.browser.version, 10) == '8' && this.currTheme == "expertusoneV2") {
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass(' white-btn-bg-middle');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right', '0px');
		}
		//new dialog popUI Script
		// $('#select-class-dialog').hide();
		if (this.currTheme == "expertusoneV2") {
			//changeDialogPopUI();
			$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
			$('#select-class-equalence-dialog').find('.ui-dialog-content').addClass('commonMsg-select-class');
			changeChildDialogPopUI('select-class-equalence-dialog');
			//$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display','none');
			if ($.browser.msie && parseInt($.browser.version, 10) == '9' && this.currTheme == "expertusoneV2" || $.browser.msie && parseInt($.browser.version, 10) == '8' && this.currTheme == "expertusoneV2") {
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').prev('.white-btn-bg-left').remove();
				$('#select-class-equalence-dialog .ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').remove();
			}
		} else {
			$('#select-class-equalence-dialog').prevAll().removeAttr('select-class-equalence-dialog');
			$('#commonMsg-wizard').closest('.ui-dialog').wrap("<div id='select-class-equalence-dialog'></div>");
			changeChildDialogPopUI('select-class-equalence-dialog');
			$('#select-class-equalence-dialog').prev('.ui-widget-overlay').css('display', 'none');
		}
		$("#commonMsg-wizard").show();
		$('.ui-dialog-titlebar-close,.removebutton').click(function () {
			$("#commonMsg-wizard").remove();
			//$('#select-class-dialog').show();
			$('#select-class-dialog').next('.ui-widget-overlay').css('display', 'block');
		});
	} catch (e) {
		// to do
	}
}