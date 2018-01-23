(function(b){var a="";b.widget("ui.adminprogramroster",{_init:function(){try{var c=this;this.rosterObj='$("body").data("adminprogramroster")';}catch(d){}},launchRoster:function(f,c,i){try{var h="";var d="";this.uniqueId=f+"_"+h+"_"+d;this.programId=f;this.courseId=h;this.classId=d;this.entityType=c;this.rosterTitle=i;this.renderRosterPopup();}catch(g){}},renderRosterPopup:function(){try{b("#program_roster_container").remove();var h="";var l="Roster - "+this.rosterTitle;h+='<div id="program_roster_container" class="admin-roster-wrapper"></div>';b("body").append(h);b("#program_roster_container").dialog({position:[(getWindowWidth()-900)/2,100],bgiframe:true,width:760,resizable:true,modal:true,title:SMARTPORTAL.t(l),closeOnEscape:false,draggable:true,overlay:{opacity:0.5,background:"black"}});this.createLoader("program_roster_container");this.regStatusList=this.buildDropDownList("lrn_crs_reg_");this.compStatusList=this.buildDropDownList("lrn_crs_cmp_");var m=this.buildFormContent();document.getElementById("program_roster_container").innerHTML=m;
b("#registration_status_date").datepicker({showOn:"button",buttonImage:"sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG",buttonImageOnly:true});b("#completion_date").datepicker({showOn:"button",buttonImage:"sites/all/themes/core/AdministrationTheme/images/calendar_icon.JPG",buttonImageOnly:true});this.destroyLoader("program_roster_container");b("#program_roster_container").show();b(".ui-dialog-titlebar-close").click(function(){b("#program_roster_container").remove();});var k=null;var f=null;var i=null;var g=null;var d=null;var c=null;this.rosterSearchResult(k,f,i,g,d,c);}catch(j){}},rosterSearchResult:function(m,g,j,i,f,c){try{this.createLoader("program-roster-container-div");var h=this;var l='$("#program-roster-result-container").data("adminprogramroster")';var d=m+"$$$"+g+"$$$"+j+"$$$"+i+"$$$"+f+"$$$"+c;b("#program-roster-result-container").jqGrid({url:this.constructUrl("administration/learning/program/roster/"+this.programId+"/"+this.entityType+"/"+d),datatype:"json",mtype:"GET",colNames:["Full Name","Username","Status"],colModel:[{name:"FullName",index:"FullName",title:true,width:225,widgetObj:l,formatter:h.paintRosterResults},{name:"Username",index:"Username",title:true,width:175,widgetObj:l,formatter:h.paintRosterResults},{name:"CompletionStatus",index:"CompletionStatus",title:true,width:125,widgetObj:l,formatter:h.paintRosterResults}],rowNum:10,rowList:[10,25,50],pager:"#pager-roster",viewrecords:true,multiselect:true,emptyrecords:"",sortorder:"desc",toppager:false,width:723,height:"auto",loadtext:"",recordtext:"",pgtext:"{0} "+Drupal.t("LBL981")+" {1}",loadui:false,loadComplete:h.callbackRosterLoader}).navGrid("#pager-roster",{add:false,edit:false,del:false,search:false,refreshtitle:true});
b("#program-roster-result-container tr:last").css("border-bottom","0px");b("#program-roster-result-container").jqGrid("filterToolbar",{searchOnEnter:false});}catch(k){}},paintRosterResults:function(c,f,h){try{var d=f.colModel.index;if(d=="Username"){return h.Username==null?"":h.Username;}else{if(d=="FullName"){return h.FullName==null?"":h.FullName;}else{if(d=="RegistrationStatus"){return h.RegistrationStatus==null?"":h.RegistrationStatus;}else{if(d=="CompletionStatus"){return h.CompletionStatus==null?"":h.CompletionStatus;}else{if(d=="Score"){return h.Score==null?"":h.Score;}else{if(d=="Grade"){return h.Grade==null?"":h.Grade;}}}}}}}catch(g){}},callbackRosterLoader:function(d){try{var c=d.events_filter;if(b("#program_roster_loaded").val()==0){b("#count-statistics").html(d.count_result);b("#program_roster_loaded").val(1);}b("body").data("adminprogramroster").destroyLoader("program-roster-container-div");if(b("#program-roster-result-container").getGridParam("records")<=0&&c==0){b("#roster-noresult-msg").show();
b("#program_roster_container").dialog({height:250});b("#roster-container").hide();}else{b("#roster-noresult-msg").hide();}b("#gbox_program-roster-result-container #prev_pager-roster").click(function(){if(b(this).hasClass("ui-state-disabled")==false){b("body").data("adminprogramroster").createLoader("program-roster-container-div");}});b("#gbox_program-roster-result-container #next_pager-roster").not(".ui-state-disabled").click(function(){if(b(this).hasClass("ui-state-disabled")==false){b("body").data("adminprogramroster").createLoader("program-roster-container-div");}});b("#gbox_program-roster-result-container .ui-jqgrid-sortable").click(function(){b("body").data("adminprogramroster").createLoader("program-roster-container-div");});b("#gbox_program-roster-result-container .ui-pg-selbox").bind("change",function(){b("body").data("adminprogramroster").createLoader("program-roster-container-div");});b("#program-roster-result-container tr:last").css("border-bottom","0px");}catch(f){}},searchRoster:function(){try{var f=b("#username_search").val()==""?null:b("#username_search").val();
var c=b("#fullname_search").val()==""?null:b("#fullname_search").val();var j=b("#registration_status_search").val()==""?null:b("#registration_status_search").val();var i=b("#completion_status_search").val()==""?null:b("#completion_status_search").val();var g=b("#score_search").val()==""?null:b("#score_search").val();var d=b("#grade_search").val()==""?null:b("#grade_search").val();this.rosterSearchResult(f,c,j,i,g,d);}catch(h){}},formReset:function(){try{b(":input","#program-roster-form-id").not(":button, :submit, :reset, :hidden").val("").removeAttr("checked").removeAttr("selected");}catch(c){}},buildFormContent:function(){try{var c="";c+='<div id="program-roster-form">';c+='<input type="hidden" id="program_roster_loaded" value="0">';c+='<form id="program-roster-form-id" name="roster-form">';c+='<span class="admin-roster-col1"><label>Registration Status:</label><select name="registration_status" id="registration_status" onchange="$(\'body\').data(\'adminprogramroster\').fillDate(\'lrn_crs_reg_\', this.value)"><option value="">Select</option>'+this.regStatusList+"</select></span>";
c+='<span class="admin-roster-col1"><label>Registration Date:</label><input readonly="readonly" size="15" type="text" name="registration_status_date" id="registration_status_date"/></span><br/>';c+='<span class="admin-roster-col1"><label>Completion Status:</label><select name="completion_status" id="completion_status" onchange="$(\'body\').data(\'adminprogramroster\').fillDate(\'lrn_crs_cmp_\', this.value)"><option value="">Select</option>'+this.compStatusList+"</select></span>";c+='<span class="admin-roster-col1"><label>Completion Date:</label><input readonly="readonly" size="15" type="text" name="completion_date" id="completion_date"/></span><br/>';c+='<div id="addedit-form-cancel-and-save-actions-row">';c+='<div class="addedit-form-cancel-container-actions">';c+='<input type="button" class="admin-action-button-middle-bg" value="Cancel" onclick="$(\'#program_roster_container\').remove();" />';c+='<div class="admin-save-button-container">';c+='<div class="admin-save-button-left-bg"></div>';
c+='<input type="button" class="admin-save-button-middle-bg" value="Apply" onclick="$(\'body\').data(\'adminprogramroster\').rosterSubmit();" />';c+='<div class="admin-save-button-right-bg"></div>';c+="</div>";c+="</div>";c+="</div>";c+="</form><br/>";c+="</div>";c+='<div id="roster-status-update" class="roster-status"></div>';c+='<div id="roster-noresult-msg">No user(s) has registered for this program.</div>';c+='<div id="roster-container"><div class="program-label-div">User(s) list:</div><div id="count-statistics" class="count-statistics"></div><div id="program-roster-container-div"><table id="program-roster-result-container"></table><div id="pager-roster"></div></div>';c+="</div></div>";return c;}catch(d){}},rosterSubmit:function(){try{this.createLoader("program_roster_container");var n=b("#program-roster-result-container").jqGrid("getGridParam","selarrrow");if(n==""){this.destroyLoader("program_roster_container");return false;}var k=b("#registration_status").val();var d=b("#registration_status_date").val();
var h=b("#completion_status").val();var o=b("#completion_date").val();var c="";var u="";var j="";var s="";b("#program_roster_loaded").val(0);d=d.replace("/","-");d=d.replace("/","-");o=o.replace("/","-");o=o.replace("/","-");var w=this.programId+"$$$"+this.entityType+"$$$"+n+"$$$"+k+"$$$"+d+"$$$"+h+"$$$"+o+"$$$"+c+"$$$"+u+"$$$"+j+"$$$"+s;var g='<div class="program-label-div">Status:</div><table><tr class="header"><td>Username</td><td>Status</td></tr>';var i="</table>";var p="";var f=decodeURI("/?q=administration/learning/program/roster/update/"+w);b.ajax({url:f,async:false,success:function(e){p=g+b.trim(e)+i;b("#roster-status-update").html("");b("#roster-status-update").html(p);b("#roster-status-update").show();}});var m=null;var x=null;var t=null;var r=null;var u=null;var c=null;var l=m+"$$$"+x+"$$$"+t+"$$$"+r+"$$$"+u+"$$$"+c;var q=decodeURI("/?q=administration/learning/program/roster/"+this.programId+"/"+this.entityType+"/"+l);this.destroyLoader("program_roster_container");this.createLoader("program-roster-container-div");
b("#program-roster-result-container").setGridParam({url:q});b("#program-roster-result-container").trigger("reloadGrid",[{page:1}]);this.formReset();}catch(v){}},buildDropDownList:function(d){try{var c="";b.ajax({url:decodeURI("/?q=administration/learning/program/roster/load-drop-down/"+d),async:false,success:function(g){g=b.trim(g);dataWithIndexArray=g.split("###");if(d=="lrn_crs_reg_"){for(var e=0;e<dataWithIndexArray.length;e++){dataArray=dataWithIndexArray[e].split("$$$");if(dataArray[0]!="lrn_crs_reg_ppm"&&dataArray[0]!="lrn_crs_reg_rsv"&&dataArray[0]!="lrn_crs_reg_rsc"&&dataArray[0]!="lrn_crs_reg_wtl"){c+="<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";}}}else{if(d=="lrn_crs_cmp_"){for(var e=0;e<dataWithIndexArray.length;e++){dataArray=dataWithIndexArray[e].split("$$$");if(dataArray[0]!="lrn_crs_cmp_inp"&&dataArray[0]!="lrn_crs_cmp_inc"&&dataArray[0]!="lrn_crs_cmp_nsw"){c+="<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";}}}else{for(var e=0;e<dataWithIndexArray.length;
e++){dataArray=dataWithIndexArray[e].split("$$$");c+="<option value='"+dataArray[0]+"'>"+dataArray[1]+"</option>";}}}}});return c;}catch(f){}},fillDate:function(g,f){try{var c=new Date();var d=(c.getMonth()+1)+"/"+c.getDate()+"/"+c.getFullYear();if(g=="lrn_crs_reg_"){if(f==""){b("#registration_status_date").val("");}else{b("#registration_status_date").val(d);}}else{if(f=="lrn_crs_cmp_cmp"){b("#completion_date").val(d);}else{b("#completion_date").val("");}}}catch(h){}}});b.extend(b.ui.adminprogramroster.prototype,EXPERTUS_SMARTPORTAL_AbstractManager,EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);})(jQuery);$(function(){$("body").adminprogramroster();});