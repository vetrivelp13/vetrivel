var API_1484_11;var API;var winobj;var _isFinishCalled;var KC_API=new Object();var isLaunched=false;var refresh_timer;var timeOutVar;SCORM_API_WRAPPER.prototype._version=null;SCORM_API_WRAPPER.prototype._type=null;SCORM_API_WRAPPER.prototype._callback=null;SCORM_API_WRAPPER.prototype.callback_param=null;SCORM_API_WRAPPER.prototype.winName="";SCORM_API_WRAPPER.prototype.contentTitle="";function SCORM_API_WRAPPER(a){try{this._version=a.version;this._type=a.type;this._contentTitle=Drupal.t("Content");}catch(b){}}SCORM_API_WRAPPER.prototype.Initialize=function(b){try{isLaunched=true;var a=new Date().getTime();this.winName=this._type.replace(/\ /g,"_")+"_"+a;if(this._type=="SCORM"){this.SCORMInit(b);}else{if(this._type=="Knowledge Content"){this.LaunchKnowledgeContent(b);}else{if(this._type=="Assessment"){this.LaunchAssessment(b);}else{if(this._type=="AICC"||this._type=="AICC Course Structure"){this.LaunchAICC(b);}else{if(this._type=="Tin Can"){this.LaunchTincan(b);}}}}}}catch(c){}};SCORM_API_WRAPPER.prototype.SCORMInit=function(a){try{console.log(a.contentQuizStatus);
console.log("aaaaawww",a.contentQuizStatus);if(winobj==undefined&&winobj==null){isLaunched=false;}switch(this._version){case"1.2":API=new SCORM_API_12();API.setCallBack(a.callback);if(a.contentQuizStatus==null||a.contentQuizStatus==""){a.contentQuizStatus="not attempted";}API.resetValue(a.contentQuizStatus);API.LMSInitialize("","LMS");API.LMSSetValue("cmi.core.student_id",a.learnerId);API.LMSSetValue("cmi.core.student_name",a.learnerName);API.LMSSetValue("cmi.core.lesson_mode","normal");API.LMSSetValue("cmi.core.lesson_status",a.completionStatus);API.LMSSetValue("cmi.core.lesson_time","0000:00:00.00");API.LMSSetValue("cmi.core.session_time","0000:00:00.00");if(a.suspend_data==null||a.suspend_data==""){API.LMSSetValue("cmi.core.entry","ab-initio");}else{API.LMSSetValue("cmi.core.entry","resume");}API.LMSSetValue("cmi.core.score.max",a.scoreMax);API.LMSSetValue("cmi.core.score.min",a.scoreMin);API.LMSSetValue("cmi.core.score.raw",a.score);API.LMSSetValue("cmi.student_data.mastery_score",a.MasteryScore);
API.LMSSetValue("cmi.core.total_time","0000:00:00.00");API.LMSSetValue("cmi.core.lesson_location",a.location);API.LMSSetValue("cmi.launch_data",a.launch_data);API.LMSSetValue("cmi.suspend_data",a.suspend_data);API.LMSSetValue("cmi.core.credit","credit");API.LMSSetValue("cmi.core.exit",a.exit);break;case"2004":API_1484_11=new SCORM_API_2004();API_1484_11.setCallBack(a.callback);API_1484_11.Initialize("");API_1484_11.SetValue("cmi.learner_id",a.learnerId);API_1484_11.SetValue("cmi.learner_name",a.learnerName);API_1484_11.SetValue("cmi.mode","normal");API_1484_11.SetValue("cmi.success_status",a.successStatus);API_1484_11.SetValue("cmi.completion_status",a.completionStatus);API_1484_11.SetValue("cmi.session_time","0000:00:00.00");API_1484_11.SetValue("cmi.entry","ab-initio");API_1484_11.SetValue("cmi.max",a.scoreMax);API_1484_11.SetValue("cmi.min",a.scoreMin);API_1484_11.SetValue("cmi.score.raw",a.score);API_1484_11.SetValue("cmi.total_time","0000:00:00.00");API_1484_11.SetValue("cmi.location",a.location);
API_1484_11.SetValue("cmi.launch_data",a.launch_data);API_1484_11.SetValue("cmi.suspend_data",a.suspend_data);API_1484_11.SetValue("cmi.credit","credit");API_1484_11.SetValue("cmi.exit",a.exit);break;}launchModalContentWindow(a.url,this);SCORM_API_WRAPPER.prototype.invokeLMSCommit(this._version);_isFinishCalled=false;}catch(b){}};SCORM_API_WRAPPER.prototype.ScormContentWindowCheck=function(){try{isLaunched=false;if(_isFinishCalled==false){if(API!=undefined){API.LMSFinish("");}else{API_1484_11.Terminate("");}}}catch(a){}};SCORM_API_WRAPPER.prototype.Finish=function(){try{clearTimeout(timeOutVar);var a={};_isFinishCalled=true;if(this._type=="SCORM"){switch(this._version){case"1.2":a.completionStatus=API.LMSGetValue("cmi.core.completion_status");a.status=API.LMSGetValue("cmi.core.lesson_status");a.score=API.LMSGetValue("cmi.core.score.raw");a.location=API.LMSGetValue("cmi.core.lesson_location");a.totalTime=API.LMSGetValue("cmi.core.total_time");a.sessionTime=API.LMSGetValue("cmi.core.session_time");
a.launch_data=API.LMSGetValue("cmi.launch_data");a.suspend_data=API.LMSGetValue("cmi.suspend_data");a.exit=API.LMSGetValue("cmi.core.exit");break;case"2004":a.completionStatus=API_1484_11.GetValue("cmi.completion_status");a.status=API_1484_11.GetValue("cmi.success_status");a.score=API_1484_11.GetValue("cmi.score.raw");a.location=API_1484_11.GetValue("cmi.location");a.totalTime=API_1484_11.GetValue("cmi.total_time");a.sessionTime=API_1484_11.GetValue("cmi.session_time");a.launch_data=API_1484_11.GetValue("cmi.launch_data");a.suspend_data=API_1484_11.GetValue("cmi.suspend_data");a.exit=API_1484_11.GetValue("cmi.exit");break;}}else{if(this._type=="Knowledge Content"){var b=SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat(KC_API.endTime-KC_API.startTime);a.completionStatus="Completed";a.status="Completed";a.score="0";a.location="0";a.totalTime="0";a.sessionTime=b;a.launchflag=0;}else{if(this._type=="AICC Course Structure"||this._type=="AICC"){a.completionStatus="";a.status="";
a.score="";a.location="";a.totalTime="";a.sessionTime="";a.launchflag=0;}else{if(this._type=="Tin Can"){a.completionStatus="";a.status="";a.score="";a.location="";a.totalTime="";a.sessionTime="";a.launchflag=0;}else{if(this._type=="Assessment"){}}}}}return a;}catch(c){}};SCORM_API_WRAPPER.prototype.invokeLMSCommit=function(b){try{var a={};if(!winobj.closed){switch(b){case"1.2":a.completionStatus=API.LMSGetValue("cmi.core.completion_status");a.status=API.LMSGetValue("cmi.core.lesson_status");a.score=API.LMSGetValue("cmi.core.score.raw");a.location=API.LMSGetValue("cmi.core.lesson_location");a.totalTime=API.LMSGetValue("cmi.core.total_time");a.sessionTime=API.LMSGetValue("cmi.core.session_time");a.launch_data=API.LMSGetValue("cmi.launch_data");a.suspend_data=API.LMSGetValue("cmi.suspend_data");a.exit=API.LMSGetValue("cmi.core.exit");a.launchflag=1;break;case"2004":a.completionStatus=API_1484_11.GetValue("cmi.completion_status");a.status=API_1484_11.GetValue("cmi.success_status");a.score=API_1484_11.GetValue("cmi.score.raw");
a.location=API_1484_11.GetValue("cmi.location");a.totalTime=API_1484_11.GetValue("cmi.total_time");a.sessionTime=API_1484_11.GetValue("cmi.session_time");a.launch_data=API_1484_11.GetValue("cmi.launch_data");a.suspend_data=API_1484_11.GetValue("cmi.suspend_data");a.exit=API_1484_11.GetValue("cmi.exit");a.launchflag=1;break;}if(b=="1.2"){API._callback(a);}else{API_1484_11._callback(a);}timeOutVar=setTimeout(function(){SCORM_API_WRAPPER.prototype.invokeLMSCommit(b);},Drupal.settings.content);}}catch(c){}};SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent=function(b){try{var g=this;g._type="Knowledge Content";if(winobj==undefined&&winobj==null){isLaunched=false;}SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback=b.callback;SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._idleCallback=b.callback;if(b.url!=null&&b.url.indexOf("?q=h5p/embed")>=0){var f=(screen.width/2)-(900/2);var d=(screen.height/2)-(520/2);var a=b.url.split("/");b.url=b.url+"&enrollId="+b.enrollid;launchModalContentWindow(b.url,this);
$("#modal-content").css("min-width","90%");EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("modal-content");}else{winobj=window.open(b.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");}KC_API.startTime=new Date().getTime();setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);SCORM_API_WRAPPER.prototype.refreshSession(g);}catch(c){}};SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck=function(){try{if(!winobj.closed){setTimeout("SCORM_API_WRAPPER.prototype.KnowledgeContentWindowCheck()",1000);}else{isLaunched=false;KC_API.endTime=new Date().getTime();SCORM_API_WRAPPER.prototype.LaunchKnowledgeContent._callback();}}catch(a){}};SCORM_API_WRAPPER.prototype.LaunchAssessment=function(a){try{SCORM_API_WRAPPER.prototype.LaunchAssessment._callback=a.callback;winobj=window.open(a.url,this.winName,"location=1,status=1,resizable =1,scrollbars=1,width=900,height=900");setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);
SCORM_API_WRAPPER.prototype.refreshSession();}catch(b){}};SCORM_API_WRAPPER.prototype.AssessmentWindowCheck=function(){try{if(!winobj.closed){setTimeout("SCORM_API_WRAPPER.prototype.AssessmentWindowCheck()",1000);}else{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchAssessment._callback();}}catch(a){}};SCORM_API_WRAPPER.prototype.LaunchAICC=function(c){try{if(winobj==undefined&&winobj==null){isLaunched=false;}var a=c.aicc_sid;var h="";var g=encodeURIComponent(resource.base_url+"/sites/all/commonlib/AICC_Handler.php?CMI=HACP");SCORM_API_WRAPPER.prototype.LaunchAICC._callback=c.callback;try{var b=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+a);var f=this;$.ajax({type:"POST",url:b,data:{command:"SETSESSION",aicc_id:a},success:function(e){if(c.url.indexOf("?")!=-1){h="&aicc_sid="+e.session_id+"&Aicc_url="+g;}else{h="?aicc_sid="+e.session_id+"&Aicc_url="+g;}f.callback_param=e.session_id;var i=c.url+h;launchModalContentWindow(i,f);SCORM_API_WRAPPER.prototype.refreshSession(f);
}});}catch(d){}}catch(d){}};SCORM_API_WRAPPER.prototype.LaunchTincan=function(c){try{if(winobj==undefined&&winobj==null){isLaunched=false;}var a=c.aicc_sid;var h="";var g=encodeURIComponent(resource.base_url+"/sites/all/commonlib/TinCan_Handler.php/");SCORM_API_WRAPPER.prototype.LaunchTincan._callback=c.callback;try{var b=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/set/"+a);var f=this;$.ajax({type:"POST",url:b,data:{command:"SETSESSION",content_token:a},success:function(e){var i=guid();if(c.url.indexOf("?")!=-1){h="&endpoint="+g+"&auth="+e.session_id+"&actor="+encodeURIComponent('{"name":"'+e.name+'","mbox":"'+e.email+'","objectType":"agent"}')+"&activity_id="+a;}else{h="?endpoint="+g+"&auth="+e.session_id+"&actor="+encodeURIComponent('{"name":"'+e.name+'","mbox":"'+e.email+'","objectType":"agent"}')+"&activity_id="+a;}f.callback_param=e.session_id;var j=c.url+h;launchModalContentWindow(j,f);SCORM_API_WRAPPER.prototype.refreshSession(f);}});}catch(d){}}catch(d){}};
SCORM_API_WRAPPER.prototype.AICCWindowCheck=function(){try{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchAICC._callback(this.callback_param);}catch(a){}};SCORM_API_WRAPPER.prototype.TinCanSCORMWindowCheck=function(){try{isLaunched=false;SCORM_API_WRAPPER.prototype.LaunchTincan._callback(this.callback_param);}catch(a){}};SCORM_API_WRAPPER.prototype.refreshSession=function(a){try{var d=this;var g=a._type.toLowerCase();var b="";var f={};switch(g){case"aicc":case"aicc course structure":f={command:"UPDATELMSDATA",session_id:a.callback_param,launchflag:1};b=resource.base_url+"/sites/all/commonlib/AICC_Handler.php";SCORM_API_WRAPPER.prototype.updateIdleCallBack(a,b,f);break;case"tin can":f={command:"UPDATELMSDATA",Authorization:a.callback_param,launchflag:1};b=resource.base_url+"/sites/all/commonlib/TinCan_Handler.php";SCORM_API_WRAPPER.prototype.updateIdleCallBack(a,b,f);break;default:break;}timeOutVar=setTimeout(function(){SCORM_API_WRAPPER.prototype.refreshSession(a);},Drupal.settings.content);
}catch(c){}};SCORM_API_WRAPPER.prototype.updateIdleCallBack=function(a,b,d){try{$.ajax({type:"POST",url:b,data:d,success:function(e){}});}catch(c){}};SCORM_API_WRAPPER.prototype.convertMilliSecondsToSCORMTimeFormat=function(f){var a;var c;var e;var b;var d;var g;b=f%1000;e=((f-b)/1000)%60;c=((f-b-(e*1000))/60000)%60;a=(f-b-(e*1000)-(c*60000))/3600000;g=SCORM_API_WRAPPER.prototype.ZeroPad(a,2)+":"+SCORM_API_WRAPPER.prototype.ZeroPad(c,2)+":"+SCORM_API_WRAPPER.prototype.ZeroPad(e,2);return g;};SCORM_API_WRAPPER.prototype.ZeroPad=function(e,d){var b;var c;var a;b=new String(e);c=b.length;if(c>d){b=b.substr(0,d);}else{for(a=c;a<d;a++){b="0"+b;}}return b;};function guid(){function a(){return Math.floor((1+Math.random())*65536).toString(16).substring(1);}return a()+a()+"-"+a()+"-"+a()+"-"+a()+"-"+a()+a()+a();}function launchModalContentWindow(o,g){try{var b=guid();winobj={};winobj.closed=false;var f=g._contentTitle;var c="ctools-content-launch-style";var d=$("body");if(Drupal.settings.widget!==undefined&&Drupal.settings.widget.widgetCallback==true){c=c+"-"+Drupal.settings.widget.widget_details.display_size;
d=$("#page-container").height()>d.height()?$("#page-container"):d;}if($.browser.msie&&($.browser.version=="9.0"||$.browser.version=="10.0")){Drupal.CTools.Modal.show(c,f,'<object type="application/x-shockwave-flash" class = "content-launch-iframe" width = "100%" height = "100%" frameborder="0" data="'+o+'" id="'+b+'"></object>',1);}else{Drupal.CTools.Modal.show(c,f,'<iframe class = "content-launch-iframe" width = "100%" height = "100%" frameborder="0" src="'+o+'" id="'+b+'"></iframe>',1);}$("#ctools-modal-content #modal-content").css({height:"auto",width:"auto"});var j=$("#ctools-modal-content");var p=$("#ctools-modal-content");var a=j.offset();var k={width:$("#modalBackdrop").css("width"),height:$("#modalBackdrop").css("height")};j.resizable({handles:"all",minHeight:j.height(),minWidth:j.width(),create:function(e,r){$('<div class="iframe-overlay"></div>').insertBefore("#"+b);$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe").height(j.height()-60);
},resize:function(e,r){$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe").height(r.size.height-60);$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay").show();},start:function(e,r){$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay").show();},stop:function(s,t){$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay").hide();var r=$(document).width()-p.width()-5;var u=$(document).height()-p.height()-5;p.draggable("option","containment",[5,5,r,u]);var e=$("#modalContent .ui-icon-gripsmall-diagonal-se").position();a=j.offset();if(e!=null&&a!=null){if(d.width()<=(e.left+a.left+5)){j.css(t.originalPosition);j.css(t.originalSize);}if(d.height()<=(e.top+a.top+5)){j.css(t.originalPosition);j.css(t.originalSize);}if(a.top<5){j.css(t.originalPosition);j.css(t.originalSize);}}a=j.offset();$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe").height(j.height()-60);
}});var q=function(){var e=$(document).width()-p.width()-5;var r=$(document).height()-p.height()-5;p.draggable({containment:[5,5,e,r],handle:"div.block-title-left",cursor:"move",create:function(s,t){$("#ctools-modal-content .block-title-left").css("cursor","move");},start:function(s,t){$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay").show();},stop:function(u,v){$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled #content-to-maximize .iframe-overlay").hide();var t=$(document).width()-p.width()-5;var w=$(document).height()-p.height()-5;p.draggable("option","containment",[5,5,t,w]);a=j.offset();var s=$("#modalContent .ui-icon-gripsmall-diagonal-se").position();if(s!=null&&a!=null){if(d.width()<=(s.left+a.left+5)){j.css(v.originalPosition);$("#modalBackdrop").css(k);}if(d.height()<=(s.top+a.top+5)){j.css(v.originalPosition);$("#modalBackdrop").css(k);}if(a.top<5){j.css(v.originalPosition);$("#modalBackdrop").css(k);}}}});};
q();var n=function(){p.draggable("destroy");$("#ctools-modal-content .block-title-left").css("cursor","default");};$("#modalContent.ctools-content-launch-style .popups-close .close").unbind("click");$("#modalContent.ctools-content-launch-style .popups-close .close").bind("click",function(r){r.preventDefault();EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("ctools-modal-content");$("#loaderdivctools-modal-content").css({height:"100%",width:"100%"});winobj.closed=true;clearTimeout(timeOutVar);var t=document.getElementById(b);try{if(t!==undefined&&t!=null){t.parentNode.removeChild(t);}}catch(s){}Drupal.CTools.Modal.dismiss();var u=g._type.toLowerCase();switch(u){case"scorm":g.ScormContentWindowCheck();break;case"aicc":case"aicc course structure":g.AICCWindowCheck();break;case"tin can":g.TinCanSCORMWindowCheck();break;}});var h=function(e){e.preventDefault();j.attr("data-css",JSON.stringify({width:j.width(),height:j.height(),position:j.css("position"),top:j.css("top"),left:j.css("left")}));
j.attr("data-windowtop",$(window).scrollTop());j.addClass("fullscreen-mode").css({width:"100%",top:"0px",left:"0px",bottom:"0px",position:"fixed",right:"0px",height:"100%",border:"none",margin:0,padding:0,"z-index":999999});$("#ctools-modal-content .popups-maximize").hide();$("#ctools-modal-content .popups-minimize").show();$(".ui-resizable-handle").toggle();n();$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe").height(j.height()-27);a=j.offset();};$("#ctools-modal-content .popups-maximize .maximize").bind("click",h);var m=function(e){e.preventDefault();a=j.offset();j.css(JSON.parse(j.attr("data-css")));j.removeClass("fullscreen-mode");$(window).scrollTop(j.attr("data-windowtop"));$("#ctools-modal-content .popups-maximize").show();$("#ctools-modal-content .popups-minimize").hide();$(".ui-resizable-handle").toggle();q();$("#ctools-modal-content.ctools-modal-content-fullscreen-enabled .content-launch-iframe").height(j.height()-60);};$("#ctools-modal-content .popups-minimize .minimize").bind("click",m);
var i=(function(){var e=0;return function(s,r){clearTimeout(e);e=setTimeout(s,r);};})();$(window).resize(function(){i(function(){j.offset(a);},500);});if(document.getElementById("lesson-wizard")!==undefined&&document.getElementById("lesson-wizard")!=null){if($("#lesson-wizard").parent().hasClass("ui-dialog")&&($("#modalContent").zIndex()<=$("#lesson-wizard").parent().zIndex())){$("#modalContent").css("z-index",$("#lesson-wizard").parent().zIndex()+1);}}if(document.getElementById("modalContent")!==undefined&&document.getElementById("modalContent")!=null&&$("#modalContent").hasClass("ctools-content-launch-style")){$("#modalBackdrop").css("z-index",$("#modalContent").zIndex()-1);}}catch(l){if(winobj!==undefined){winobj.closed=true;}}}