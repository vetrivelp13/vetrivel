//alert(22);
var API_1484_11;
var API;
//var scoobj;
var scormwindow;
SCORM_API_WRAPPER.prototype._version = null;
SCORM_API_WRAPPER.prototype._type = null;
SCORM_API_WRAPPER.prototype._callback = null;


function SCORM_API_WRAPPER(data){
    this._version = data.version;
    this._type = data.type;
    this._callback = data.callback;
};

SCORM_API_WRAPPER.prototype.Initialize = function(data){ 
    if(this._type == "SCORM" || unescape(this._type) == "SCORM 2004" ){
        this.SCORMInit(data);
    }else if(this._type == "Knowledge Content"){
        this.LaunchKnowledgeContent(data);
    }else if (this._type == "Assessment") { // Code start for My Know How integration
    	this.LaunchAssessment(data);
    }else if (this._type == "AICC" || this._type=="AICC Course Structure"){ // AICC integration
        this.LaunchAICC(data);
    }else if(this._type == "Tin Can" || this._type == "tin can" ){
    	this.LaunchTincan(data);
    }
    // Code end for My Know How integration
};

SCORM_API_WRAPPER.prototype.SCORMInit = function(data){

    switch(this._version){
        case "1.2" :
            API  = new SCORM_API_12();
            API.setCallBack(data.callback);
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
            API.LMSSetValue("cmi.core.entry","ab-initio");
            API.LMSSetValue("cmi.core.score.max",data.scoreMax);
            API.LMSSetValue("cmi.core.score.min",data.scoreMin);
            API.LMSSetValue("cmi.core.score.raw",data.score);
            API.LMSSetValue("cmi.student_data.mastery_score",data.masteryscore);
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
            API_1484_11.SetValue("cmi.success_status",data.status);
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
//    alert('ScormInit');
//    window.open('http://www.google.com');
    $("#msg").empty();
    data.url = decodeURI(data.url);
    
	
var d = new Date();
var n = d.getTime();
var av = (n -data.version)/1000;

if(data.show_button == '1' && data.version == "" ){
alert("You are not authorized to view this content. ");
return;
}

if(av >10 && data.version !="" ){
alert("You are not authorized to view this content. ");
//$("body").append("<object id='mobileObject' width='100%' style='height: 100%;left:0; top:120px;'   data='nonononono'></object>");
return;
}
    
	
var questionmark_content = "";
if (data.url.indexOf('questionmark') > -1)
{
questionmark_content = true;
}

if(data.show_button != '1'){

if(questionmark_content)
{
   if(navigator.userAgent.match('iPhone')){
    $("body").append("<iframe id='mobileObject' width='100%' height='100%'  style='position:absolute;left:0;top:0px;'   src='"+data.url+"'></iframe>");
    }else{
    $("body").append("<iframe id='mobileObject' width='100%' height='100%'  style='position:absolute;left:0;top:0px;'   src='"+data.url+"'></iframe>"); 
    }	
}
else{
   if(navigator.userAgent.match('iPhone')){
    $("body").append("<object id='mobileObject' width='100%'  style='height: 100%;position:absolute;left:0;top:0px;'   data='"+data.url+"'></object>");
    }else{
    $("body").append("<object id='mobileObject' width='100%'  style='height: 100%;position:absolute;left:0;top:0px'   data='"+data.url+"'></object>"); 
    }
}

    $("body").append("<button type='button' style='font-size:45px; background:transparent; height:75px; width:75px; padding:0px; border:0px; position:absolute; top:-100; right: 0;' display:block !important; id='closebtn' onClick='closeWindow();'><img src='../apis/core/close_button_blue.png' style='height:75px; width:75px;'/></button>");
}else
{

if(questionmark_content)
{
$("body").append("<iframe id='mobileObject' width='100%' height='100%' style='left:0; top:120px;margin-top:180px;'   src='"+data.url+"'></iframe>");   
}
else{
$("body").append("<object id='mobileObject' width='100%' style='height: 100%;left:0; top:120px;margin-top:180px;'   data='"+data.url+"'></object>");   
}

$("body").append("<div style='background-color: #393939;border: 2px solid #393939; height: 80px;position: absolute; top:0;left:0;right:0;width: 99%;'><span onClick='closeWindow()'; style='display:block; display: block; width: 100px;background-color: #007AFF;border: 1px solid #007AFF;color:#FFFFFF;height: 55px;text-align: center;margin-top: 12px;margin-right: 10px;float: right;border-radius: 5px;'><span style='display:block;margin-top: 15px;font-size: 18px;'>Done</span></span></div>");
}
   
   	window.addEventListener("orientationchange", function() {
   		window.setTimeout(function(){
   	     	// Announce the new orientation number
   	     	if(window.innerHeight > window.innerWidth){
   	     		$("#mobileObject").css('height','100%');
   				// $("#mobileObject").css('margin-left','0px');
   	     	}
   	     	else
   	     		{
   	     		$("#mobileObject").css('height','97%');
   				// $("#mobileObject").css('margin-left','0px');
   	     		}
   	     },300);
   }, false);
    
    //style='font-family:\"pictos\"; D 
    //<img src='closeButton.png' style='height:100%; width:100%'/>
    //scormwindow=window.open(data.url,"scormwindow","location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
    //scormwindow=window.open(data.url,"mywin","location=0,toolbar=0,status=0,menubar=0");
};

SCORM_API_WRAPPER.prototype.Finish = function(){
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
                result.suspendData = API.LMSGetValue("cmi.suspend_data");
                result.exit = API.LMSGetValue("cmi.core.exit");
                break;
            case "2004" :
                result.completionStatus = API_1484_11.GetValue("cmi.completion_status");
                result.status = API_1484_11.GetValue("cmi.success_status");
                result.score = API_1484_11.GetValue("cmi.score.raw");
                result.location = API_1484_11.GetValue("cmi.location");
                result.totalTime = API_1484_11.GetValue("cmi.total_time");
                result.sessionTime = API_1484_11.GetValue("cmi.session_time");
                result.launch_data = API_1484_11.GetValue("cmi.launch_data");
                result.suspendData = API_1484_11.GetValue("cmi.suspend_data");
                result.exit = API_1484_11.GetValue("cmi.exit");
                break;
        }
    }else if(this._type == "Knowledge Content"){
        result.completionStatus="Completed"; 
        result.status="Completed";
        result.score="0";
        result.location="0";
        result.totalTime="0";
        result.sessionTime="0";
    }else if(this._type == "AICC Course Structure" || this._type=="AICC"){ 
    	result.completionStatus="";
        result.status="";
        result.score="";
        result.location="";
        result.totalTime="";
        result.sessionTime="";
    }else if(this._type == "Assessment"){ // Code start for My Know How integration
    	// Do nothing.
    }
 // Code end for My Know How integration
    
    
    return result;
};


SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent = function(data){
   SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback = data.callback;
   API = window.open(data.url,"kcwindow","location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
   setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
   
};

SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck=function(){
    if(!API.closed)
        setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);
    else
        SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback();
};

//Code start for My Know How integration

SCORM_API_WRAPPER.prototype.LaunchAssessment = function(data){
	   //alert("In SCORM_API_WRAPPER.prototype.LaunchAssessment()");
	   SCORM_API_WRAPPER.prototype.LaunchAssessment._callback = data.callback;
	   API = window.open(data.url,"asmwindow","location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
	   setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
	   
	};

SCORM_API_WRAPPER.prototype.AssessmentWindowCheck=function(){
	//alert("In SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()");
    if(!API.closed)
        setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
    else
        SCORM_API_WRAPPER.prototype.LaunchAssessment._callback();
};
// Code end for My Know How integration

//AICC integration
SCORM_API_WRAPPER.prototype.LaunchAICC=function(data){
	try{
	var aicc_id =data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(data.host+"/sites/all/commonlib/AICC_Handler.php?CMI=HACP&aicc_id="+aicc_id+"&access_token="+data.access_token);
	
	SCORM_API_WRAPPER.prototype.LaunchAICC._callback = data.callback;
	try{	
	//	var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
	//var url=data.host+"/?q=aicc/launch/set/"+sid+"&aicc_id="+sid+"&command=SETSESSION";
	var url = data.host+"/sites/all/commonlib/AICC_Handler.php";
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSIONMOBILE", "aicc_id": aicc_id },
				success: function(result){
				var session_id = result
					if(data.url.indexOf("?") != -1){
						qrystr = "&aicc_sid="+session_id+"&Aicc_url="+callBackURL;
					}else{
						qrystr = "?aicc_sid="+session_id+"&Aicc_url="+callBackURL;
					}
					
				scoobj._callback = session_id;

					var pmPath = data.url + qrystr ;
	 $("#msg").empty();
	 $("body").append("<object id='mobileObject' width='100%' style='height: 100%;position:absolute;left:0;top:0'  data='"+pmPath+"' ></object>");
     $("body").append("<button type='button' style='font-size:45px; background:transparent; height:75px; width:75px; padding:0px; border:0px; position:absolute; top:-100px; right: 0;' id='closebtn' onClick='closeWindow();'><img src='../apis/core/close_button_blue.png'  style='height:75px; width:75px;'/></button>");
	
     window.addEventListener("orientationchange", function() {
     	// Announce the new orientation number
     	window.setTimeout(function(){
     	if(window.innerHeight > window.innerWidth){
     		$("#mobileObject").css('height','100%');
     	}
     	else
     		{
     		$("#mobileObject").css('height','97%');
     		}
     	},300);
     }, false);
//      
				}
	    });
	  }catch(e){
		  // to do
	  }
	}catch(e){
		// to do
	}
};


SCORM_API_WRAPPER.prototype.AICCWindowCheck=function(){
	if(!aiccwinobj.closed)
		setTimeout("SCORM_API_WRAPPER.prototype.AICCWindowCheck()",1000);
    else
    	SCORM_API_WRAPPER.prototype.LaunchAICC._callback();
};

SCORM_API_WRAPPER.prototype.LaunchTincan=function(data){
	try{
	var aicc_id = data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var sid = data.aicc_sid; //data.learnerId+"-"+data.courseid+"-"+data.classid+"-"+data.lessonid+"-"+data.versionid+"-"+data.enrollid+"-"+data.location;
	var qrystr ="";
	var callBackURL = encodeURIComponent(data.host+"/sites/all/commonlib/TinCan_Handler.php?aicc_id="+aicc_id+"&access_token="+data.access_token);
	SCORM_API_WRAPPER.prototype.LaunchTincan._callback = data.callback;
	this.aicc_id = aicc_id;
	//alert('call back -->> '+ data.toSource())
	try{	
		//var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+sid);
		var url = data.host+"/sites/all/commonlib/TinCan_Handler.php";
		var obj = SCORM_API_WRAPPER.prototype;
		$.ajax({
				type: "POST",
				url: url,
				data:  { "command": "SETSESSIONMOBILE", "content_token": aicc_id },
				success: function(result){
				//alert(result.toSource());
					if(data.url.indexOf("?") != -1){
						//alert('if-');
						qrystr = "&endpoint="+callBackURL+"&auth="+result+"&Authorization="+result+"&access_token="+data.access_token;
					}else{
						//alert('else-');
						qrystr = "?endpoint="+callBackURL+"&auth="+result+"&Authorization="+result+"&access_token="+data.access_token;
					}
					obj.callback_param = result;
					var pmPath = data.url + qrystr ;
					
					 $("#msg").empty();
					 $("body").append("<object id='mobileObject' width='100%' style='height: 100%;position:absolute;left:0;top:0' data='"+pmPath+"' ></object>");
				     $("body").append("<button type='button' style='font-size:45px; background:transparent; height:75px; width:75px; padding:0px; border:0px; position:absolute; top:-100; right: 0;' id='closebtn' onClick='closeWindow();'><img src='../apis/core/close_button_blue.png'  style='height:75px; width:75px;'/></button>");

				     
				     window.addEventListener("orientationchange", function() {
				     	// Announce the new orientation number
     	window.setTimeout(function(){
				     	if(window.innerHeight > window.innerWidth){
				     		$("#mobileObject").css('height','100%');
				     	}
				     	else
				     		{
				     		$("#mobileObject").css('height','97%');
				     		}
     	},300);
				     }, false);
					//alert(pmPath);
//					winobj = window.open(pmPath,"myaiccwindow","location=1,status=1,scrollbars=1,resizable=yes,width=900,height=900");
//					//winobj.document.write('<iframe width="100%" height="100%" src="'+pmPath+'" frameborder="0" allowfullscreen></iframe>'); //allowfullscreen
//				    setTimeout("SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck()",1000);
//				    SCORM_API_WRAPPER.prototype.refreshSession();
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


