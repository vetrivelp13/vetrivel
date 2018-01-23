var API_1484_11;
var API;
var winobj;
var _isFinishCalled;
var KC_API = new Object();
var isLaunched = false;
var refresh_timer;
var timeOutVar;
SCORM_API_WRAPPER.prototype._version = null;
SCORM_API_WRAPPER.prototype._type = null;
SCORM_API_WRAPPER.prototype._callback = null;
SCORM_API_WRAPPER.prototype.callback_param = null;
SCORM_API_WRAPPER.prototype.winName = '';
SCORM_API_WRAPPER.prototype.contentTitle = '';

function SCORM_API_WRAPPER(data){
	try{
    this._version = data.version;
    this._type = data.type;
    this._contentTitle = Drupal.t('Content');
	}catch(e){
		// to do
	}
};

SCORM_API_WRAPPER.prototype.Initialize = function(data){
	try{
		isLaunched = true;
		var dateStr =  new Date().getTime();
		//this.winName = this._type+'_'+ dateStr;
		this.winName = this._type.replace(/\ /g, "_")+'_'+ dateStr;

    if(this._type == "SCORM"){
        this.SCORMInit(data);
    }else if(this._type == "Knowledge Content"){
        this.LaunchKnowledgeContent(data);
    }else if (this._type == "Assessment") { // Code for My Know How integration
    	this.LaunchAssessment(data);
    }else if (this._type == "AICC" || this._type=="AICC Course Structure"){ // AICC integration
    	this.LaunchAICC(data);
    }else if(this._type == "Tin Can"){
    	this.LaunchTincan(data);
    }
	}catch(e){
		// to do
	}
    
};

SCORM_API_WRAPPER.prototype.SCORMInit = function(data){
	try{
	console.log(data.contentQuizStatus);
	console.log('aaaaawww',data.contentQuizStatus);
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
    switch(this._version){
        case "1.2" :
            API  = new SCORM_API_12();
            API.setCallBack(data.callback);
            if(data.contentQuizStatus == null || data.contentQuizStatus == ""){ data.contentQuizStatus='not attempted'}
            API.resetValue(data.contentQuizStatus);
            API.LMSInitialize("",'LMS');
            API.LMSSetValue("cmi.core.student_id",data.learnerId);
            API.LMSSetValue("cmi.core.student_name",data.learnerName);
            //possible values: browse,normal,resumes
            API.LMSSetValue("cmi.core.lesson_mode","normal");
            //possible values:passed,completed,failed,incomplete,browsed,not attempted
            API.LMSSetValue("cmi.core.lesson_status",data.completionStatus);
            API.LMSSetValue("cmi.core.lesson_time","0000:00:00.00");
            API.LMSSetValue("cmi.core.session_time","0000:00:00.00");
            //possible values: ab-initio,"",resume
            if(data.suspend_data == null ||data.suspend_data == "" ){            	
                API.LMSSetValue("cmi.core.entry","ab-initio");
            }else{
                	API.LMSSetValue("cmi.core.entry","resume");
            }
            //API.LMSSetValue("cmi.core.entry","ab-initio");
            API.LMSSetValue("cmi.core.score.max",data.scoreMax);
            API.LMSSetValue("cmi.core.score.min",data.scoreMin);
            API.LMSSetValue("cmi.core.score.raw",data.score);
            API.LMSSetValue("cmi.student_data.mastery_score",data.MasteryScore);
            API.LMSSetValue("cmi.core.total_time","0000:00:00.00");
            API.LMSSetValue("cmi.core.lesson_location",data.location);
            //"cmi.launch_data" contains any information that is to be provided to a SCO upon the start of any session of that SCO including a learner�s initial visit.
            //This value is initialized by the LMS to be equal to the information in
            //the <adlcp:datafromlms> element in the manifest for the particular SCO.
            API.LMSSetValue("cmi.launch_data",data.launch_data);
            API.LMSSetValue("cmi.suspend_data",data.suspend_data);
            API.LMSSetValue("cmi.core.credit","credit");
            API.LMSSetValue("cmi.core.exit",data.exit);
            break;
        case "2004":
            API_1484_11  = new SCORM_API_2004();
            API_1484_11.setCallBack(data.callback);

            API_1484_11.Initialize("");
            API_1484_11.SetValue("cmi.learner_id",data.learnerId);
            API_1484_11.SetValue("cmi.learner_name",data.learnerName);
            //possible values: browse,normal,resumes
            API_1484_11.SetValue("cmi.mode","normal");
            //possible values:passed,completed,failed,incomplete,browsed,not attempted
            API_1484_11.SetValue("cmi.success_status",data.successStatus);
            API_1484_11.SetValue("cmi.completion_status",data.completionStatus);
            API_1484_11.SetValue("cmi.session_time","0000:00:00.00");
            //possible values: ab-initio,"",resume
            API_1484_11.SetValue("cmi.entry","ab-initio");
            API_1484_11.SetValue("cmi.max",data.scoreMax);
            API_1484_11.SetValue("cmi.min",data.scoreMin);
            API_1484_11.SetValue("cmi.score.raw",data.score);
            API_1484_11.SetValue("cmi.total_time","0000:00:00.00");
            API_1484_11.SetValue("cmi.location",data.location);
            //"cmi.launch_data" contains any information that is to be provided to a SCO upon the start of any session of that SCO including a learner�s initial visit.
            //This value is initialized by the LMS to be equal to the information in
            //the <adlcp:datafromlms> element in the manifest for the particular SCO.
            API_1484_11.SetValue("cmi.launch_data",data.launch_data);
            API_1484_11.SetValue("cmi.suspend_data",data.suspend_data);
            API_1484_11.SetValue("cmi.credit","credit");
            API_1484_11.SetValue("cmi.exit",data.exit);
            break;
    }
	// Changes done for content launch in modal window -- start
	// pass callback function as argument which needs to be called when modal window is closed
	launchModalContentWindow(data.url, this);
	SCORM_API_WRAPPER.prototype.invokeLMSCommit(this._version);
    _isFinishCalled=false
	}catch(e){
		// to do
	}
};


SCORM_API_WRAPPER.prototype.ScormContentWindowCheck = function(){
	try{
    // if(!winobj.closed)
        // setTimeout("SCORM_API_WRAPPER.prototype.ScormContentWindowCheck()",1000);
    // else{
		isLaunched = false;
    	if(_isFinishCalled==false){
    		if(API != undefined)
    			API.LMSFinish('');
    		else
    			API_1484_11.Terminate('');
    	}
    // }
	}catch(e){
		// to do
	}
};

SCORM_API_WRAPPER.prototype.Finish = function(){
	try{
	clearTimeout(timeOutVar);
    var result = {};
    _isFinishCalled=true;
    if(this._type=="SCORM"){
        switch(this._version){
            case "1.2" :
                result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
                result.status = API.LMSGetValue("cmi.core.lesson_status");
                result.score = API.LMSGetValue("cmi.core.score.raw");
                result.location = API.LMSGetValue("cmi.core.lesson_location");
                result.totalTime = API.LMSGetValue("cmi.core.total_time");
                result.sessionTime = API.LMSGetValue("cmi.core.session_time");
                result.launch_data = API.LMSGetValue("cmi.launch_data");
                result.suspend_data = API.LMSGetValue("cmi.suspend_data");
                result.exit = API.LMSGetValue("cmi.core.exit");
                break;
            case "2004" :
            	//SCORM 2004 will have 2 statuses - both should be validated 
                result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
                result.status = API_1484_11.GetValue("cmi.success_status");					//quiz status
                result.score = API_1484_11.GetValue("cmi.score.raw");
                result.location = API_1484_11.GetValue("cmi.location");
                result.totalTime = API_1484_11.GetValue("cmi.total_time");
                result.sessionTime = API_1484_11.GetValue("cmi.session_time");
                result.launch_data = API_1484_11.GetValue("cmi.launch_data");
                result.suspend_data = API_1484_11.GetValue("cmi.suspend_data");
                result.exit = API_1484_11.GetValue("cmi.exit");
                break;
        }
    }else if(this._type == "Knowledge Content"){
		var timeSpend = SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime - KC_API.startTime);
		result.completionStatus	= "Completed";
        result.status			= "Completed";
        result.score			= "0";
        result.location			= "0";
        result.totalTime		= "0";
        result.sessionTime		= timeSpend;
        result.launchflag 		= 0;
    }else if(this._type == "AICC Course Structure" || this._type=="AICC"){ 
    	result.completionStatus="";
        result.status="";
        result.score="";
        result.location="";
        result.totalTime="";
        result.sessionTime="";
        result.launchflag = 0;
    }else if(this._type == "Tin Can"){ 
    	result.completionStatus="";
        result.status="";
        result.score="";
        result.location="";
        result.totalTime="";
        result.sessionTime="";
        result.launchflag = 0;
    }else if(this._type == "Assessment"){ // Code start for My Know How integration
    	// Do nothing.
    }
    return result;
	}catch(e){
		// to do
	}
};

/**
 * function used to initiate the LMSCommit at every interval of 5 mins
 * content lessons. 
 * @return
 */
SCORM_API_WRAPPER.prototype.invokeLMSCommit = function(version){
	try{
		var result = {};
		if(!winobj.closed){
			switch(version){
			case "1.2" :
				result.completionStatus = API.LMSGetValue("cmi.core.completion_status");
                result.status = API.LMSGetValue("cmi.core.lesson_status");
                result.score = API.LMSGetValue("cmi.core.score.raw");
                result.location = API.LMSGetValue("cmi.core.lesson_location");
                result.totalTime = API.LMSGetValue("cmi.core.total_time");
                result.sessionTime = API.LMSGetValue("cmi.core.session_time");
                result.launch_data = API.LMSGetValue("cmi.launch_data");
                result.suspend_data = API.LMSGetValue("cmi.suspend_data");
                result.exit = API.LMSGetValue("cmi.core.exit");
                result.launchflag = 1;
                break;
			case "2004" :
				result.completionStatus = API_1484_11.GetValue("cmi.completion_status");	//lesson status
                result.status = API_1484_11.GetValue("cmi.success_status");					//quiz status
                result.score = API_1484_11.GetValue("cmi.score.raw");
                result.location = API_1484_11.GetValue("cmi.location");
                result.totalTime = API_1484_11.GetValue("cmi.total_time");
                result.sessionTime = API_1484_11.GetValue("cmi.session_time");
                result.launch_data = API_1484_11.GetValue("cmi.launch_data");
                result.suspend_data = API_1484_11.GetValue("cmi.suspend_data");
                result.exit = API_1484_11.GetValue("cmi.exit");
                result.launchflag = 1;
                break;
			}
			if(version == "1.2"){
				API._callback(result);				
			}else {
				API_1484_11._callback(result);			
			}
			//setTimeout(SCORM_API_WRAPPER.prototype.invokeLMSCommit,(Drupal.settings.content), version);  // request for once in certain interval configured in exp_sp.ini
			timeOutVar = setTimeout(function(){
				SCORM_API_WRAPPER.prototype.invokeLMSCommit(version);
			},Drupal.settings.content);
	  }
	}catch(e){
		
	}
}

SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent = function(data){
  try{
  	var obj = this;
	obj._type = 'Knowledge Content';
  	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
	SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback = data.callback;
	SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback = data.callback;
	if(data.url != null && data.url.indexOf("?q=h5p/embed") >= 0) 
    {
    	var left = (screen.width/2)-(900/2);
  		var top = (screen.height/2)-(520/2); 
  		var newurl = data.url.split("/");
		//to support tincan change url
  		//data.url = "?q=node/"+newurl[2]+"/view&enrollId="+data.enrollid;
  		data.url = data.url+"&enrollId="+data.enrollid;
  		//modal-content
  		
  		
  		launchModalContentWindow(data.url, this);
  		$("#modal-content").css("min-width","90%");
  		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("modal-content");
  		//winobj = window.open(data.url+"&enrollId="+data.enrollid,this.winName,"location=1,status=0,resizable =1,scrollbars=1,width=900,height=520,top="+top+",left="+left);
   		//API = window.open(data.url,"kcwindow","location=1,status=1,resizable =1,scrollbars=1,width=900,height=520,top="+top+",left="+left);
    }
   else
    {
   		winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
    }
	
	
	
	
	
  // winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
   KC_API.startTime		= new Date().getTime();
   setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
   SCORM_API_WRAPPER.prototype.refreshSession(obj);
  }catch(e){
	  // to do
  }
};

SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck=function(){
	try{
    if(!winobj.closed){
        setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
    }else{
		isLaunched = false;
		KC_API.endTime		= new Date().getTime();
        SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback();
    }
	}catch(e){
		// to do
	}
};

//Code start for My Know How integration

SCORM_API_WRAPPER.prototype.LaunchAssessment = function(data){
	try{
	   //alert("In SCORM_API_WRAPPER.prototype.LaunchAssessment()");
	   SCORM_API_WRAPPER.prototype.LaunchAssessment._callback = data.callback;
	   winobj = window.open(data.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
	   setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
	   SCORM_API_WRAPPER.prototype.refreshSession();
	}catch(e){
		// to do
	} 
	};

SCORM_API_WRAPPER.prototype.AssessmentWindowCheck=function(){
	try{
	//alert("In SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()");
    if(!winobj.closed){
        setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
    }else{
		isLaunched = false;
        SCORM_API_WRAPPER.prototype.LaunchAssessment._callback();
    }
	}catch(e){
		// to do
	}
};
// Code end for My Know How integration

//AICC integration
SCORM_API_WRAPPER.prototype.LaunchAICC=function(data){
	try{
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}  
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(resource.base_url+"/sites/all/commonlib/AICC_Handler.php?CMI=HACP");
	SCORM_API_WRAPPER.prototype.LaunchAICC._callback = data.callback;

	try{	
		var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
		var obj = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSION", "aicc_id": sid },
				success: function(result){
					if(data.url.indexOf("?") != -1){
						qrystr = "&aicc_sid="+result.session_id+"&Aicc_url="+callBackURL;
					}else{
						qrystr = "?aicc_sid="+result.session_id+"&Aicc_url="+callBackURL;
					}
					obj.callback_param = result.session_id;
					var pmPath = data.url + qrystr ;
					launchModalContentWindow(pmPath, obj);
					// winobj = window.open(pmPath,this.winName,"location=1,status=1,scrollbars=1,resizable=yes,width=900,height=900");
					//winobj.document.write('<iframe width="100%" height="100%" src="'+pmPath+'" frameborder="0" allowfullscreen></iframe>'); //allowfullscreen
				    // setTimeout("SCORM_API_WRAPPER.prototype.AICCWindowCheck()",1000);
				    SCORM_API_WRAPPER.prototype.refreshSession(obj);
				}
	    });
	  }catch(e){
		  // to do
	  }
	}catch(e){
		// to do
	}
};
/**
 * Tincan integeration.
 */
SCORM_API_WRAPPER.prototype.LaunchTincan=function(data){
	try{
	if(winobj == undefined && winobj == null) {
	  isLaunched = false;
	}
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(resource.base_url+"/sites/all/commonlib/TinCan_Handler.php/");
	SCORM_API_WRAPPER.prototype.LaunchTincan._callback = data.callback;
	//alert('call back -->> '+ data.toSource())
	try{	
		var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
		var obj = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSION", "content_token": sid },
				success: function(result){
				//alert(result.toSource());
					var uuid = guid();
					if(data.url.indexOf("?") != -1){
						//alert('if-');
						qrystr = "&endpoint="+callBackURL+"&auth="+result.session_id+'&actor='+encodeURIComponent('{"name":"'+result.name+'","mbox":"'+result.email+'","objectType":"agent"}')+'&activity_id='+sid;
					}else{
						//alert('else-');
						qrystr = "?endpoint="+callBackURL+"&auth="+result.session_id+'&actor='+encodeURIComponent('{"name":"'+result.name+'","mbox":"'+result.email+'","objectType":"agent"}')+'&activity_id='+sid;
					}
					obj.callback_param = result.session_id;
					var pmPath = data.url + qrystr ;
					
					// winobj = window.open(pmPath,this.winName,"location=1,status=1,scrollbars=1,resizable=yes,width=900,height=900");
					launchModalContentWindow(pmPath, obj);
					//winobj.document.write('<iframe width="100%" height="100%" src="'+pmPath+'" frameborder="0" allowfullscreen></iframe>'); //allowfullscreen
				    // setTimeout("SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck()",1000);
				    SCORM_API_WRAPPER.prototype.refreshSession(obj);
				}
	    });
	  }catch(e){
		  // to do
		  //console.log(e);
	  }
	}catch(e){
		// to do
		//console.log(e);
	}
};
SCORM_API_WRAPPER.prototype.AICCWindowCheck = function(){
	try{
		// if(!winobj.closed){
			// setTimeout("SCORM_API_WRAPPER.prototype.AICCWindowCheck()",1000);
		// }else{
			isLaunched = false;
	    	SCORM_API_WRAPPER.prototype.LaunchAICC._callback(this.callback_param);
		// }
	}catch(e){
		// to do
	}
}

SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck = function(){
	try{
		// if(!winobj.closed){
			// console.log('TinCanSCORMWindowCheck');
			// setTimeout("SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck()",1000);
		// }else{
			isLaunched = false;
			//alert('window check');
	    	SCORM_API_WRAPPER.prototype.LaunchTincan._callback(this.callback_param);
		// }
	}catch(e){
		// to do
	}
}

/**
 * function used to keep the session alive when the learner reading the 
 * content lessons. 
 * @return
 */
SCORM_API_WRAPPER.prototype.refreshSession = function(launchObj){
	try{
		//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
		var obj = this;
		var contentType = launchObj._type.toLowerCase();
		var url ='';
		var params = {};
		switch(contentType) {
			case "aicc":
			case "aicc course structure":
				//console.log('aicc block');
				params = {
					'command' : 'UPDATELMSDATA',
					'session_id' : launchObj.callback_param,
					'launchflag' : 1
				}
				url = resource.base_url+"/sites/all/commonlib/AICC_Handler.php";
				SCORM_API_WRAPPER.prototype.updateIdleCallBack(launchObj, url, params);
				break;
			case 'tin can':
				params = {
					'command' : 'UPDATELMSDATA',
					'Authorization' : launchObj.callback_param,
					'launchflag' : 1
				}
				url = resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";
				SCORM_API_WRAPPER.prototype.updateIdleCallBack(launchObj, url, params);
				//console.log('tincan block');
				break;
			/* case 'knowledge content':
				var result = {};
				KC_API.endTime		= new Date().getTime();
				var timeSpend = SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime - KC_API.startTime);
				result.completionStatus	= "Completed";
		        result.status			= "Completed";
		        result.score			= "0";
		        result.location			= "0";
		        result.totalTime		= "0";
		        result.sessionTime		= timeSpend;
		        result.launchflag 		= 1;
		        SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback(result);
				break */
			default:
				//console.log('default block');
				break;
		}
		timeOutVar = setTimeout(function(){SCORM_API_WRAPPER.prototype.refreshSession(launchObj);},Drupal.settings.content);
		/*if(!winobj.closed){
			var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/get/0");
			$.ajax({
	    		   type: "POST",
		           url: url,
		           cache: false,
		           success: function(result){}
		        });
			setTimeout("SCORM_API_WRAPPER.prototype.refreshSession()",(1000*60*15));  // request for once in 15 min
		} */
	}catch(e){
		// to do
	}	
}

//73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
SCORM_API_WRAPPER.prototype.updateIdleCallBack = function(launchObj, url, params) {
	try {
		//console.log('params', params);
		$.ajax({
			type: "POST",
			url: url,
			data:  params,
			success: function(result){
				/* var contentPlayerObj;
				if(document.getElementById('learningplan-tab-inner')) {
					if($("#learningplan-tab-inner").data("contentLaunch").updateFrom == 'LP'){
						contentPlayerObj = $("#learningplan-tab-inner").data("contentPlayer");
					}
				}
				if(document.getElementById('learner-enrollment-tab-inner')) {
					if($("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom == 'ME'){
						contentPlayerObj = $("#learner-enrollment-tab-inner").data("contentPlayer");
					}
				}
				if(document.getElementById('lnr-catalog-search')){
					contentPlayerObj = $("#lnr-catalog-search").data("contentPlayer");
			    }
				contentPlayerObj.refreshContentProgressBarLine(contentPlayerObj,result); */
			}
		});
	} catch(e){
		//console.log(e);
	}
}

/**
 * function used to convert milliseconds to SCORM session_time format
 * @return
 */
SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat = function(intTotalMilliseconds){
	var intHours;
	var intMinutes;
	var intSeconds;
	var intMilliseconds;
	var intHundredths;
	var strCMITimeSpan;
	
	//extract time parts
	intMilliseconds = intTotalMilliseconds % 1000;

	intSeconds = ((intTotalMilliseconds - intMilliseconds) / 1000) % 60;

	intMinutes = ((intTotalMilliseconds - intMilliseconds - (intSeconds * 1000)) / 60000) % 60;

	intHours = (intTotalMilliseconds - intMilliseconds - (intSeconds * 1000) - (intMinutes * 60000)) / 3600000;
	
	//put in padding 0's and concatinate to get the proper format
	strCMITimeSpan = SCORM_API_WRAPPER.prototype.ZeroPad(intHours, 2) + ":" + SCORM_API_WRAPPER.prototype.ZeroPad(intMinutes, 2) + ":" + SCORM_API_WRAPPER.prototype.ZeroPad(intSeconds, 2);
	
	return strCMITimeSpan;

}

SCORM_API_WRAPPER.prototype.ZeroPad = function(intNum, intNumDigits){

	var strTemp;
	var intLen;
	var i;
	
	strTemp = new String(intNum);
	intLen = strTemp.length;
	
	if (intLen > intNumDigits){
		strTemp = strTemp.substr(0,intNumDigits);
	}
	else{
		for (i=intLen; i<intNumDigits; i++){
			strTemp = "0" + strTemp;
		}
	}
	
	return strTemp;
}

function guid() {
	  function s4() {
	    return Math.floor((1 + Math.random()) * 0x10000)
	      .toString(16)
	      .substring(1);
	  }
	  return s4() + s4() + '-' + s4() + '-' + s4() + '-' +
	    s4() + '-' + s4() + s4() + s4();
}
function launchModalContentWindow(contentURL, object) {
	try {
	// https://pacoup.com/2011/06/12/list-of-true-169-resolutions/
		var iframeID = guid();
		winobj = {};
		winobj.closed = false;
		var modalTitle = object._contentTitle;
		var style = 'ctools-content-launch-style';
		var modalWindowContainer = $('body');
		if(Drupal.settings.widget !== undefined && Drupal.settings.widget.widgetCallback == true) {
			style = style + '-' + Drupal.settings.widget.widget_details.display_size;
			modalWindowContainer = $('#page-container').height() > modalWindowContainer.height() ? $('#page-container') : modalWindowContainer;
		}
		
		if ($.browser.msie && ($.browser.version=="9.0"|| $.browser.version=="10.0")) {
			Drupal.CTools.Modal.show(style, modalTitle, '<object type="application/x-shockwave-flash" class = "content-launch-iframe" width = "100%" height = "100%" frameborder="0" data="'+contentURL+'" id="'+iframeID+'"></object>', 1);
		}else{
			Drupal.CTools.Modal.show(style, modalTitle, '<iframe class = "content-launch-iframe" width = "100%" height = "100%" frameborder="0" src="'+contentURL+'" id="'+iframeID+'"></iframe>', 1);
		}
		$('#ctools-modal-content #modal-content').css({'height': 'auto', 'width': 'auto'});
		var modalWindow = $('#ctools-modal-content');
		var modalWindowToDrag = $('#ctools-modal-content');
		var modalWindowPosition = modalWindow.offset();
		var modalWindowBackDropCss = {'width': $('#modalBackdrop').css('width'), 'height': $('#modalBackdrop').css('height')};
		
		modalWindow.resizable({
			handles: 'all',
			// containment: 'document',
			minHeight: modalWindow.height(),
			minWidth: modalWindow.width(),
			create: function(event, ui) {
				$('<div class="iframe-overlay"></div>').insertBefore('#'+iframeID);
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe').height(modalWindow.height() - 60);
			},
			resize: function(event, ui) {
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe').height(ui.size.height - 60);
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay').show();
			},
			start: function(event, ui) {
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay').show();
			},
			stop: function(event, ui) {
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay').hide();
				// reset the containment when modal window is resized
				var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
				var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
				modalWindowToDrag.draggable('option', 'containment', [5, 5, draggableX2, draggableY2]);
				// resize the height/width to original if its greater than available container (i.e. body)
				var resizePosition = $('#modalContent .ui-icon-gripsmall-diagonal-se').position();
				modalWindowPosition = modalWindow.offset();
				if(resizePosition != null && modalWindowPosition != null) {
					if(modalWindowContainer.width() <= (resizePosition.left + modalWindowPosition.left + 5)) {
						modalWindow.css(ui.originalPosition);
						modalWindow.css(ui.originalSize);
					}
					if(modalWindowContainer.height() <= (resizePosition.top + modalWindowPosition.top + 5)) {
						modalWindow.css(ui.originalPosition);
						modalWindow.css(ui.originalSize);
					}
					// not to allow modal window to have negative value for top position
					if(modalWindowPosition.top < 5) {
						modalWindow.css(ui.originalPosition);
						modalWindow.css(ui.originalSize);
					}
				}
				modalWindowPosition = modalWindow.offset();
				$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe').height(modalWindow.height() - 60);
			}
		});
		
		var enableDrag = function() {
			var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
			var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
			modalWindowToDrag.draggable({
				containment: [5, 5, draggableX2, draggableY2],	//boundaries till which dragging is available
				handle: 'div.block-title-left',
				cursor: 'move',
				create: function(event, ui) {
					$('#ctools-modal-content .block-title-left').css('cursor', 'move');
				},
				start: function(event, ui) {
					$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay').show();
				},
				stop: function(event, ui) {
					$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay').hide();
					var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
					var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
					modalWindowToDrag.draggable('option', 'containment', [5, 5, draggableX2, draggableY2]);
					modalWindowPosition = modalWindow.offset();
					var resizePosition = $('#modalContent .ui-icon-gripsmall-diagonal-se').position();
					if(resizePosition != null && modalWindowPosition != null) {
						if(modalWindowContainer.width() <= (resizePosition.left + modalWindowPosition.left + 5)) {
							modalWindow.css(ui.originalPosition);
							$('#modalBackdrop').css(modalWindowBackDropCss);
						}
						if(modalWindowContainer.height() <= (resizePosition.top + modalWindowPosition.top + 5)) {
							modalWindow.css(ui.originalPosition);
							$('#modalBackdrop').css(modalWindowBackDropCss);
						}
						// not to allow modal window to have negative value for top position
						if(modalWindowPosition.top < 5) {
							modalWindow.css(ui.originalPosition);
							$('#modalBackdrop').css(modalWindowBackDropCss);
						}
					}
				}
			});
		}
		enableDrag();
		var disableDrag = function() {
			modalWindowToDrag.draggable("destroy");
			$('#ctools-modal-content .block-title-left').css('cursor', 'default');
		}
		// bind callbacks for close button
		$('#modalContent.ctools-content-launch-style .popups-close .close').unbind('click');
		$('#modalContent.ctools-content-launch-style .popups-close .close').bind('click', function(event) {
			event.preventDefault();
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('ctools-modal-content');
			$('#loaderdivctools-modal-content').css({'height': '100%', 'width': '100%'});
			winobj.closed = true;
			clearTimeout(timeOutVar);
			// document.getElementById(iframeID).remove();
			var frame = document.getElementById(iframeID);
			try {
				//following line needs to be wrapped inside a try-catch to suppress warning in Chrome
				if(frame !== undefined && frame != null) frame.parentNode.removeChild(frame);
			} catch(e) {
				//console.log(e);
			}
			Drupal.CTools.Modal.dismiss();
			var contentType = object._type.toLowerCase();
			switch(contentType) {
				case "scorm":
					object.ScormContentWindowCheck();
				break;
				case "aicc":
				case "aicc course structure":
					object.AICCWindowCheck();
				break;
				case 'tin can':
					object.TinCanSCORMWindowCheck();
				break;
				
			}
		});
		
		// Bind a click for maximizing the modalContent
		var maximize = function(event) {
			event.preventDefault();
			modalWindow.attr('data-css', JSON.stringify({'width': modalWindow.width(),
										  'height': modalWindow.height(),
										  'position': modalWindow.css('position'),
										  'top': modalWindow.css('top'),
										  'left': modalWindow.css('left')
										  }));
			modalWindow.attr('data-windowtop', $(window).scrollTop());
			modalWindow.addClass('fullscreen-mode')
			.css({
				'width': '100%',
				'top': '0px',
				'left': '0px',
				'bottom': '0px',
				'position': 'fixed',
				'right': '0px',
				'height': '100%',
				'border': 'none',
				'margin': 0,
				'padding': 0,
				'z-index': 999999
				});
			// Show the minimize button; hide the maximize button when full screen mode is on
			$('#ctools-modal-content .popups-maximize').hide();
			$('#ctools-modal-content .popups-minimize').show();
			//disable resize option
			$('.ui-resizable-handle').toggle();
			//disable drag option
			disableDrag();
			$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe').height(modalWindow.height() - 27);
			modalWindowPosition = modalWindow.offset();
		};
		$('#ctools-modal-content .popups-maximize .maximize').bind('click', maximize);
		
		var minimize = function(event) {
			event.preventDefault();
			modalWindowPosition = modalWindow.offset();
			modalWindow.css(JSON.parse(modalWindow.attr('data-css')));
			modalWindow.removeClass('fullscreen-mode');
			$(window).scrollTop(modalWindow.attr('data-windowtop'));
			// Show the maximize button; hide the minimize button when full screen is left
			$('#ctools-modal-content .popups-maximize').show();
			$('#ctools-modal-content .popups-minimize').hide();
			//enable resize option
			$('.ui-resizable-handle').toggle();
			//enable drag option
			enableDrag();
			$('#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe').height(modalWindow.height() - 60);
		}
		$('#ctools-modal-content .popups-minimize .minimize').bind('click', minimize);
		
		// adjust the position of the modal popup when the browser window is minimized/maximized
		var delay = (function(){
			var timer = 0;
			return function(callback, ms){
				clearTimeout (timer);
				timer = setTimeout(callback, ms);
			};
		})();
		$(window).resize(function() {
			delay(function(){
				modalWindow.offset(modalWindowPosition);
			}, 500);
		});
		// ui-dialog of multiple lessons aicc has z-index higher than the modal content popup
		if(document.getElementById('lesson-wizard') !== undefined && document.getElementById('lesson-wizard') != null) {
			if($('#lesson-wizard').parent().hasClass('ui-dialog') && ($('#modalContent').zIndex() <= $('#lesson-wizard').parent().zIndex())) {
				$('#modalContent').css('z-index', $('#lesson-wizard').parent().zIndex()+1);
			}
		}
		// ui-widget-overlay of select class and multiple content launch dialogs have z-index higher than the modal content popup
		if(document.getElementById('modalContent') !== undefined && document.getElementById('modalContent') != null && $('#modalContent').hasClass('ctools-content-launch-style')) {
			$('#modalBackdrop').css('z-index', $('#modalContent').zIndex() - 1);
		}
	} catch(e) {
		if(winobj !== undefined) {
			winobj.closed = true;
		}
		// console.log(e.stack);
		// console.log(e);
	}	
}