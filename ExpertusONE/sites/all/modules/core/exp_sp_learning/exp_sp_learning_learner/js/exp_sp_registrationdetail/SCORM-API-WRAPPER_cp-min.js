var API_1484_11;var API;var winobj;var _isFinishCalled;var KC_API=new Object();var isLaunched=false;var contentPlayer={closed:true};var timeOutVar;SCORM_API_WRAPPER.prototype._version=null;SCORM_API_WRAPPER.prototype._type=null;SCORM_API_WRAPPER.prototype._callback=null;SCORM_API_WRAPPER.prototype.callback_param=null;SCORM_API_WRAPPER.prototype.winName="";function SCORM_API_WRAPPER(a){try{this._version=a.version;this._type=a.type;}catch(b){}}SCORM_API_WRAPPER.prototype.Initialize=function(b){try{isLaunched=true;var a=new Date().getTime();this.winName=this._type+"_"+a;if(this._type=="SCORM"){this.SCORMInit(b);}else{if(this._type=="Knowledge Content"){if(b.access_denied_flag==1){this.LaunchKnowledgeContentInWindow(b);}else{this.LaunchKnowledgeContent(b);}}else{if(this._type=="Assessment"){this.LaunchAssessment(b);}else{if(this._type=="AICC"||this._type=="AICC Course Structure"){this.LaunchAICC(b);}else{if(this._type=="Tin Can"){this.LaunchTincan(b);}}}}}}catch(c){}};SCORM_API_WRAPPER.prototype.SCORMInit=function(a){try{switch(this._version){case"1.2":API=new SCORM_API_12();
API.setCallBack(a.callback);API.setIdleCallBack(a.idleCallback);if(a.contentQuizStatus==null||a.contentQuizStatus==""){a.contentQuizStatus="not attempted";}API.resetValue(a.contentQuizStatus);API.LMSInitialize("","LMS");API.LMSSetValue("cmi.core.student_id",a.learnerId);API.LMSSetValue("cmi.core.student_name",a.learnerName);API.LMSSetValue("cmi.core.lesson_mode","normal");API.LMSSetValue("cmi.core.lesson_status",a.completionStatus);API.LMSSetValue("cmi.core.lesson_time","0000:00:00.00");API.LMSSetValue("cmi.core.session_time","0000:00:00.00");if(a.suspend_data==null||a.suspend_data==""){API.LMSSetValue("cmi.core.entry","ab-initio");}else{API.LMSSetValue("cmi.core.entry","resume");}API.LMSSetValue("cmi.core.score.max",a.scoreMax);API.LMSSetValue("cmi.core.score.min",a.scoreMin);API.LMSSetValue("cmi.core.score.raw",a.score);API.LMSSetValue("cmi.student_data.mastery_score",a.MasteryScore);API.LMSSetValue("cmi.core.total_time","0000:00:00.00");API.LMSSetValue("cmi.core.lesson_location",a.location);
API.LMSSetValue("cmi.launch_data",a.launch_data);API.LMSSetValue("cmi.suspend_data",a.suspend_data);API.LMSSetValue("cmi.core.credit","credit");API.LMSSetValue("cmi.core.exit",a.exit);break;case"2004":API_1484_11=new SCORM_API_2004();API_1484_11.setCallBack(a.callback);API_1484_11.setIdleCallBack(a.idleCallback);API_1484_11.Initialize("");API_1484_11.SetValue("cmi.learner_id",a.learnerId);API_1484_11.SetValue("cmi.learner_name",a.learnerName);API_1484_11.SetValue("cmi.mode","normal");API_1484_11.SetValue("cmi.success_status",a.successStatus);API_1484_11.SetValue("cmi.completion_status",a.completionStatus);API_1484_11.SetValue("cmi.session_time","0000:00:00.00");API_1484_11.SetValue("cmi.entry","ab-initio");API_1484_11.SetValue("cmi.max",a.scoreMax);API_1484_11.SetValue("cmi.min",a.scoreMin);API_1484_11.SetValue("cmi.score.raw",a.score);API_1484_11.SetValue("cmi.total_time","0000:00:00.00");API_1484_11.SetValue("cmi.location",a.location);API_1484_11.SetValue("cmi.launch_data",a.launch_data);
API_1484_11.SetValue("cmi.suspend_data",a.suspend_data);API_1484_11.SetValue("cmi.credit","credit");API_1484_11.SetValue("cmi.exit",a.exit);break;}launchContentPlayer(this,a.url);SCORM_API_WRAPPER.prototype.invokeLMSCommit(this._version);_isFinishCalled=false;}catch(b){}};SCORM_API_WRAPPER.prototype.ScormContentWindowCheck=function(){try{isLaunched=false;if(_isFinishCalled==false){if(API!=undefined){API.LMSFinish("");}else{API_1484_11.Terminate("");}}}catch(a){}};SCORM_API_WRAPPER.prototype.Finish=function(){try{clearTimeout(timeOutVar);var a={};_isFinishCalled=true;if(this._type=="SCORM"){switch(this._version){case"1.2":a.completionStatus=API.LMSGetValue("cmi.core.completion_status");a.status=API.LMSGetValue("cmi.core.lesson_status");a.score=API.LMSGetValue("cmi.core.score.raw");a.location=API.LMSGetValue("cmi.core.lesson_location");a.totalTime=API.LMSGetValue("cmi.core.total_time");a.sessionTime=API.LMSGetValue("cmi.core.session_time");a.launch_data=API.LMSGetValue("cmi.launch_data");
a.suspend_data=API.LMSGetValue("cmi.suspend_data");a.exit=API.LMSGetValue("cmi.core.exit");break;case"2004":a.completionStatus=API_1484_11.GetValue("cmi.completion_status");a.status=API_1484_11.GetValue("cmi.success_status");a.score=API_1484_11.GetValue("cmi.score.raw");a.location=API_1484_11.GetValue("cmi.location");a.totalTime=API_1484_11.GetValue("cmi.total_time");a.sessionTime=API_1484_11.GetValue("cmi.session_time");a.launch_data=API_1484_11.GetValue("cmi.launch_data");a.suspend_data=API_1484_11.GetValue("cmi.suspend_data");a.exit=API_1484_11.GetValue("cmi.exit");break;}}else{if(this._type=="Knowledge Content"){var b=SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime-KC_API.startTime);a.completionStatus="Completed";a.status="Completed";a.score="0";a.location="0";a.totalTime="0";a.sessionTime=b;a.launchflag=0;}else{if(this._type=="AICC Course Structure"||this._type=="AICC"){a.completionStatus="";a.status="";a.score="";a.location="";a.totalTime="";a.sessionTime="";
a.launchflag=0;}else{if(this._type=="Tin Can"){a.completionStatus="";a.status="";a.score="";a.location="";a.totalTime="";a.sessionTime="";a.launchflag=0;}else{if(this._type=="Assessment"){}}}}}return a;}catch(c){}};SCORM_API_WRAPPER.prototype.invokeLMSCommit=function(b){try{var a={};if(!winobj.closed){switch(b){case"1.2":a.completionStatus=API.LMSGetValue("cmi.core.completion_status");a.status=API.LMSGetValue("cmi.core.lesson_status");a.score=API.LMSGetValue("cmi.core.score.raw");a.location=API.LMSGetValue("cmi.core.lesson_location");a.totalTime=API.LMSGetValue("cmi.core.total_time");a.sessionTime=API.LMSGetValue("cmi.core.session_time");a.launch_data=API.LMSGetValue("cmi.launch_data");a.suspend_data=API.LMSGetValue("cmi.suspend_data");a.exit=API.LMSGetValue("cmi.core.exit");a.launchflag=1;break;case"2004":a.completionStatus=API_1484_11.GetValue("cmi.completion_status");a.status=API_1484_11.GetValue("cmi.success_status");a.score=API_1484_11.GetValue("cmi.score.raw");a.location=API_1484_11.GetValue("cmi.location");
a.totalTime=API_1484_11.GetValue("cmi.total_time");a.sessionTime=API_1484_11.GetValue("cmi.session_time");a.launch_data=API_1484_11.GetValue("cmi.launch_data");a.suspend_data=API_1484_11.GetValue("cmi.suspend_data");a.exit=API_1484_11.GetValue("cmi.exit");a.launchflag=1;break;}if(b=="1.2"){API._idleCallback(a);}else{API_1484_11._idleCallback(a);}timeOutVar=setTimeout(function(){SCORM_API_WRAPPER.prototype.invokeLMSCommit(b);},Drupal.settings.content);}}catch(c){}};SCORM_API_WRAPPER.prototype.LaunchKnowledgeContentInWindow=function(c){try{var h=this;h._type="Knowledge Content";if(winobj==undefined&&winobj==null){isLaunched=false;}console.log(h);SCORM_API_WRAPPER.prototype.LaunchKnowledgeContentInWindow._callback=c.callback;SCORM_API_WRAPPER.prototype.LaunchKnowledgeContentInWindow._idleCallback=c.callback;if(c.url!=null&&c.url.indexOf("?q=h5p/embed")>=0){var g=(screen.width/2)-(900/2);var f=(screen.height/2)-(520/2);var b=c.url.split("/");winobj=window.open(c.url+"&enrollId="+c.enrollid,this.winName,"location=1,status=0,resizable =1,scrollbars=1,width=900,height=520,top="+f+",left="+g);
}else{winobj=window.open(c.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");}var a={};KC_API.endTime=new Date().getTime();a.completionStatus="completed";a.status="completed";a.score="0";a.location="0";a.totalTime="0";a.sessionTime="";a.access_denied_flag=1;console.log(a);SCORM_API_WRAPPER.prototype.LaunchKnowledgeContentInWindow._callback(a);if(document.getElementById("learningplan-tab-inner")){$("#learningplan-tab-inner").data("contentLaunch").destroyLoader("cp-modalcontainer");}if(document.getElementById("learner-enrollment-tab-inner")){$("#learner-enrollment-tab-inner").data("contentLaunch").destroyLoader("cp-modalcontainer");}if(document.getElementById("lnr-catalog-search")){$("#lnr-catalog-search").data("contentLaunch").destroyLoader("cp-modalcontainer");}$("#lnr-catalog-search").data("enrollment").destroyLoader("enroll-result-container");}catch(d){console.log(d);}};SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheckForLoadDenied=function(){try{if(!winobj.closed){setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheckForLoadDenied()",1000);
}else{isLaunched=false;var a={};KC_API.endTime=new Date().getTime();var b=SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime-KC_API.startTime);console.log(b);a.completionStatus="completed";a.status="completed";a.score="0";a.location="0";a.totalTime="0";a.sessionTime=b;a.access_denied_flag=1;console.log(a);console.log(SCORM_API_WRAPPER);SCORM_API_WRAPPER.prototype.LaunchKnowledgeContentInWindow._callback(a);if(document.getElementById("learningplan-tab-inner")){$("#learningplan-tab-inner").data("contentLaunch").destroyLoader("cp-modalcontainer");}if(document.getElementById("learner-enrollment-tab-inner")){$("#learner-enrollment-tab-inner").data("contentLaunch").destroyLoader("cp-modalcontainer");}if(document.getElementById("lnr-catalog-search")){$("#lnr-catalog-search").data("contentLaunch").destroyLoader("cp-modalcontainer");}$("#lnr-catalog-search").data("enrollment").destroyLoader("enroll-result-container");}}catch(c){}};SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent=function(f){try{var h=this;
h._type="Knowledge Content";SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback=f.callback;SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback=f.callback;if(f.url.indexOf("http://")>-1||f.url.indexOf("https://")>-1){$("#cp-frame-container").attr("src",f.url);}else{var b=f.url.split(".").pop();if(b.toLowerCase()=="pdf"){var a=window.location;var d=a.protocol+"//"+a.host+"/"+a.pathname.split("/")[1];localStorage.removeItem("database");$("#cp-frame-container").attr("src",d+"sites/all/libraries/pdfviewer/viewer.html?file="+f.url);if(f.suspend_data==""||f.suspend_data==undefined||f.suspend_data==null){setTimeout(function(){document.getElementById("cp-frame-container").contentWindow.PDFViewerApplication.page=1;},1000);}else{var c=JSON.parse(unescape(f.suspend_data));if(c.current_page==""||c.current_page==undefined||c.current_page==null){setTimeout(function(){document.getElementById("cp-frame-container").contentWindow.PDFViewerApplication.page=1;},1000);}else{setTimeout(function(){document.getElementById("cp-frame-container").contentWindow.PDFViewerApplication.page=c.current_page;
},1000);}}}else{$("#cp-frame-container").attr("src",f.url);if(f.url.indexOf("?q=h5p/embed")<0){$("#cp-frame-container").contents().find("body").append('<div style="font-family:openSansRegular, Arial; font-size:12px; color:#aaa; text-align:center;" class="cont-status-msg">'+Drupal.t("MSG797")+"</div>");}}}KC_API.startTime=new Date().getTime();SCORM_API_WRAPPER.prototype.refreshSession(h);}catch(g){}};SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck=function(){try{if(!contentPlayer.closed){KnowledgeContentWindowCheck=setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);}else{isLaunched=false;clearTimeout(KnowledgeContentWindowCheck);KC_API.endTime=new Date().getTime();SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback();}}catch(a){}};SCORM_API_WRAPPER.prototype.LaunchAssessment=function(a){try{SCORM_API_WRAPPER.prototype.LaunchAssessment._callback=a.callback;winobj=window.open(a.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");
setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);SCORM_API_WRAPPER.prototype.refreshSession();}catch(b){}};SCORM_API_WRAPPER.prototype.AssessmentWindowCheck=function(){try{if(!winobj.closed){setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);}else{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchAssessment._callback();}}catch(a){}};SCORM_API_WRAPPER.prototype.LaunchAICC=function(c){try{var a=c.aicc_sid;var h="";var g=encodeURIComponent(resource.base_url+"/sites/all/commonlib/AICC_Handler.php?CMI=HACP");SCORM_API_WRAPPER.prototype.LaunchAICC._callback=c.callback;try{var b=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+a);var f=this;$.ajax({type:"POST",url:b,data:{command:"SETSESSION",aicc_id:a},success:function(e){if(c.url.indexOf("?")!=-1){h="&aicc_sid="+e.session_id+"&Aicc_url="+g;}else{h="?aicc_sid="+e.session_id+"&Aicc_url="+g;}f.callback_param=e.session_id;var i=c.url+h;launchContentPlayer(f,i);SCORM_API_WRAPPER.prototype.refreshSession(f);
}});}catch(d){}}catch(d){}};SCORM_API_WRAPPER.prototype.LaunchTincan=function(c){try{var a=c.aicc_sid;var h="";var g=encodeURIComponent(resource.base_url+"/sites/all/commonlib/TinCan_Handler.php/");SCORM_API_WRAPPER.prototype.LaunchTincan._callback=c.callback;try{var b=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+a);var f=this;$.ajax({type:"POST",url:b,data:{command:"SETSESSION",content_token:a},success:function(e){var i=guid();if(c.url.indexOf("?")!=-1){h="&endpoint="+g+"&auth="+e.session_id+'&actor={"name":"'+e.name+'","mbox":"'+e.email+'","objectType":"agent"}&activity_id='+a;}else{h="?endpoint="+g+"&auth="+e.session_id+'&actor={"name":"'+e.name+'","mbox":"'+e.email+'","objectType":"agent"}&activity_id='+a;}f.callback_param=e.session_id;var j=c.url+h;launchContentPlayer(f,j);SCORM_API_WRAPPER.prototype.refreshSession(f);}});}catch(d){}}catch(d){}};SCORM_API_WRAPPER.prototype.AICCWindowCheck=function(){try{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchAICC._callback(this.callback_param);
}catch(a){}};SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck=function(){try{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchTincan._callback(this.callback_param);}catch(a){}};SCORM_API_WRAPPER.prototype.refreshSession=function(c){try{var g=this;var i=c._type.toLowerCase();var d="";var h={};switch(i){case"aicc":case"aicc course structure":h={command:"UPDATELMSDATA",session_id:c.callback_param,launchflag:1};d=resource.base_url+"/sites/all/commonlib/AICC_Handler.php";SCORM_API_WRAPPER.prototype.updateIdleCallBack(c,d,h);break;case"tin can":h={command:"UPDATELMSDATA",Authorization:c.callback_param,launchflag:1};d=resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";SCORM_API_WRAPPER.prototype.updateIdleCallBack(c,d,h);break;case"knowledge content":var a={};KC_API.endTime=new Date().getTime();var b=SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime-KC_API.startTime);a.completionStatus="InProgress";a.status="InProgress";a.score="0";a.location="0";a.totalTime="0";
a.sessionTime=b;a.launchflag=1;SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback(a);break;default:break;}timeOutVar=setTimeout(function(){SCORM_API_WRAPPER.prototype.refreshSession(c);},Drupal.settings.content);}catch(f){}};SCORM_API_WRAPPER.prototype.updateIdleCallBack=function(a,b,d){try{$.ajax({type:"POST",url:b,data:d,success:function(e){var f;if(document.getElementById("learningplan-tab-inner")){if($("#learningplan-tab-inner").data("contentLaunch").updateFrom=="LP"){f=$("#learningplan-tab-inner").data("contentPlayer");}}if(document.getElementById("learner-enrollment-tab-inner")){if($("#learner-enrollment-tab-inner").data("contentLaunch").updateFrom=="ME"){f=$("#learner-enrollment-tab-inner").data("contentPlayer");}}if(document.getElementById("lnr-catalog-search")){f=$("#lnr-catalog-search").data("contentPlayer");}f.refreshContentProgressBarLine(f,e);}});}catch(c){}};SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat=function(f){var a;var c;var e;var b;var d;
var g;b=f%1000;e=((f-b)/1000)%60;c=((f-b-(e*1000))/60000)%60;a=(f-b-(e*1000)-(c*60000))/3600000;g=SCORM_API_WRAPPER.prototype.ZeroPad(a,2)+":"+SCORM_API_WRAPPER.prototype.ZeroPad(c,2)+":"+SCORM_API_WRAPPER.prototype.ZeroPad(e,2);return g;};SCORM_API_WRAPPER.prototype.ZeroPad=function(e,d){var b;var c;var a;b=new String(e);c=b.length;if(c>d){b=b.substr(0,d);}else{for(a=c;a<d;a++){b="0"+b;}}return b;};function launchContentPlayer(a,c){try{winobj={};winobj.closed=false;$("#cp-frame-container").attr("src",c);$("#cp-modalcontainer .cp-modal-close, #cp-modalcontainer .cp-menu-action, #cp-modalcontainer .cp-content-menu-details").bind("click",function(d){closeCallHandler(d,a);});}catch(b){}}function closeCallHandler(b,a){try{b.preventDefault();EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("ctools-modal-content");winobj.closed=true;var d=document.getElementById("cp-frame-container");try{if(a._type=="AICC"||a._type=="AICC Course Structure"||a._type=="Tin Can"||a._type=="SCORM"){if(d!==undefined&&d!=null){d.parentNode.removeChild(d);
}}}catch(c){}$("#cp-modalcontainer .cp-modal-close, #cp-modalcontainer .cp-menu-action, #cp-modalcontainer .cp-content-menu-details").unbind("click");}catch(c){}}function guid(){function a(){return Math.floor((1+Math.random())*65536).toString(16).substring(1);}return a()+a()+"-"+a()+"-"+a()+"-"+a()+"-"+a()+a()+a();}