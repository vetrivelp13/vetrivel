var isPopupOpen=0;function selContentHostedType(c){try{var a=$("#edit-content-type").val();if((a=="lrn_cnt_typ_knc")||(a=="lrn_cnt_typ_vod")){if($("#edit-hosted-type-file").attr("checked")){$("#content_url_control").hide();$("#upload_browse_only").show();$("#content_browse_control").show();}else{$("#content_url_control").show();$("#content_browse_control").hide();}}else{$("#upload_browse_only").show();$("#upload_browse_and_url").hide();$("#edit-content-data").attr("size","35");}}catch(b){}}function toEditContentLessonTitle(f,a){try{var d="blur";if(a=="label"){if($.trim($("#content_title"+f).val())==""){$("#content_title"+f).val($("#content_title_label_"+f).attr("title"));}$("#content_title_label_"+f).hide();$("#content_title_label_edit_"+f).show();$("#content_title"+f).focus();d="click";}else{$("#content_title_label_edit_"+f).hide();if($.trim($("#content_title"+f).val())!=""){$("#content_title_label_"+f).html($("#content_title"+f).val());}$("#content_title_label_"+f).show();}var b="content_title"+f;
$("#"+b).addClass("edit-attachment-txtbox");$("#"+b).removeClass("non-editable-txt");$("#edit-lesson-title-container").find(".attachment-txt-box").each(function(){var e=$(this).attr("id");if((e==b)&&(d=="click")){$("#"+e).removeClass("edit-attachment-txtbox");$("#"+e).addClass("edit-attachment-txtbox");$("#"+e).removeClass("non-editable-txt");}else{$("#"+e).removeClass("edit-attachment-txtbox");$("#"+e).removeClass("non-editable-txt");$("#"+e).addClass("non-editable-txt");}});}catch(c){}}function contentTypeSelect(){try{var a=$("#edit-content-type").val();if((a=="lrn_cnt_typ_knc")||(a=="lrn_cnt_typ_vod")){$("#edit-hosted-type-file").attr("checked",true);$("#content_browse_control").show();$("#content_url_control").hide();$("#edit-content-data").attr("size","17");$("#upload_browse_and_url").show();if(a=="lrn_cnt_typ_vod"){$("#upload_msg_part").show();}else{$("#upload_msg_part").hide();}}else{$("#content_browse_control").show();$("#upload_browse_only").show();$("#upload_browse_and_url").hide();
$("#edit-content-data").attr("size","35");$("#upload_msg_part").hide();}}catch(b){}}function toEditLessonKeyDown(b){try{b=b||window.event;var a=b.keyCode||b.which;if(a==13){$("#content_save_btn").click();b.preventDefault();return false;}}catch(c){}}function sizeOfUploadButton(a){try{if($.browser.msie){if(a=="lrn_cnt_typ_knc"||a=="lrn_cnt_typ_vod"){$("#exp-sp-administration-content-addedit-form #content_browse_control div .form-file").attr("size","26");}else{$("#exp-sp-administration-content-addedit-form #content_browse_control div .form-file").attr("size","46");}}}catch(b){}}function setVersionDefault(d,b){try{$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");var a=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/manage/content/check-version-transfer-conflict/"+d+"/"+b);$.ajax({type:"POST",url:a,data:"",success:function(e){var h=JSON.parse(e);if(h.newitems!=null){pubContentVersion(d,b,h.newitems);$("body").data("mulitselectdatagrid").destroyLoader("paint-class-search-results-datagrid");
}else{if(h.inprogress!=null){$("#select-class-equalence-dialog").remove();$("body").data("learningcore").callMessageWindow(Drupal.t("Content"),Drupal.t("MSG995"));var g=$("#modalContent").css("z-index");$("#select-class-equalence-dialog .ui-widget-content").css("z-index",g+10);$(".ui-widget-overlay").css("z-index",g+1);$("body").data("mulitselectdatagrid").destroyLoader("paint-class-search-results-datagrid");return;}else{var f=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/set-default-version/"+d+"/"+b);$.ajax({type:"POST",url:f,data:"",success:function(i){var j=$("#admin-version-list-"+d+"-pagination_toppager .ui-pg-input").val();$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");$("#admin-version-list-"+d+"-pagination").trigger("reloadGrid",[{page:j}]);}});}}}});}catch(c){}}function pubContentVersion(a,b,d){try{var j=300;var h=Drupal.t("MSG993")+'"? <br/>'+Drupal.t("MSG994");$("#version-conf-wizard").remove();
var f="";f+='<div id="version-conf-wizard" style="display:none; padding: 0px;">';f+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';f+='<tr><td class="delete-msg-txtcolor" color="#333333" style="padding: 30px 15px; text-align:left;">'+h+"</td></tr>";f+="</table>";f+="</div>";$("body").append(f);var i={};i[Drupal.t("LBL109")]=function(){$(this).dialog("destroy");$(this).dialog("close");$("#version-conf-wizard").remove();};i[Drupal.t("Yes")]=function(){$("#version-conf-wizard").remove();var e=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/cancel-version-transfer/"+a+"/"+b+"/"+d);$.ajax({type:"POST",url:e,data:"",success:function(k){var l=$("#admin-version-list-"+a+"-pagination_toppager .ui-pg-input").val();$("#admin-version-list-"+a+"-pagination").trigger("reloadGrid",[{page:l}]);$("body").data("mulitselectdatagrid").destroyLoader("paint-class-search-results-datagrid");}});};$("#version-conf-wizard").dialog({position:[(getWindowWidth()-400)/2,200],bgiframe:true,width:j,resizable:false,modal:true,title:Drupal.t("LBL3110"),buttons:i,closeOnEscape:false,draggable:false,overlay:{opacity:0.9,background:"black"}});
$(".ui-dialog").wrap("<div id='conf-object-dialog'></div>");var c=$("#modalContent").css("z-index");$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)").addClass("removebutton").end();$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").before('<div class="admin-save-button-left-bg"></div>');$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").addClass("admin-save-button-middle-bg").end();$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").after('<div class="admin-save-button-right-bg"></div>');this.currTheme=Drupal.settings.ajaxPageState.theme;if(this.currTheme=="expertusoneV2"){changeDialogPopUI();$(".expertusV2PopupContainer").wrapAll('<div class="reports-delete-popup" />');}$("#version-conf-wizard").show();$("#conf-object-dialog .ui-widget-content").css("z-index",c+10);$(".ui-widget-overlay").css("z-index",c+1);$(".ui-dialog-titlebar-close").click(function(){$("#version-conf-wizard").remove();});}catch(g){}}function deleteContentVersion(a,b,h){try{var j=300;
var g=Drupal.t("MSG357")+' "'+h+'"? <br/>'+Drupal.t("LBL1281");$("#delete-msg-wizard").remove();var d="";d+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';d+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';d+='<tr><td class="delete-msg-txtcolor" color="#333333" style="padding: 30px 15px; text-align:left;">'+g+"</td></tr>";d+="</table>";d+="</div>";$("body").append(d);var i={};i[Drupal.t("LBL109")]=function(){$(this).dialog("destroy");$(this).dialog("close");$("#delete-msg-wizard").remove();};i[Drupal.t("Yes")]=function(){$("#delete-msg-wizard").remove();$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");var e=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("ajax/administration/manage/content/delete-version/"+b);$.ajax({type:"POST",url:e,data:"",success:function(k){var l=$("#admin-version-list-"+a+"-pagination_toppager .ui-pg-input").val();$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
$("#admin-version-list-"+a+"-pagination").trigger("reloadGrid",[{page:l}]);}});};$("#delete-msg-wizard").dialog({position:[(getWindowWidth()-400)/2,200],bgiframe:true,width:j,resizable:false,modal:true,title:Drupal.t("LBL286"),buttons:i,closeOnEscape:false,draggable:false,overlay:{opacity:0.9,background:"black"}});$(".ui-dialog").wrap("<div id='delete-object-dialog'></div>");var c=$("#modalContent").css("z-index");$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)").addClass("removebutton").end();$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").before('<div class="admin-save-button-left-bg"></div>');$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").addClass("admin-save-button-middle-bg").end();$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").after('<div class="admin-save-button-right-bg"></div>');this.currTheme=Drupal.settings.ajaxPageState.theme;if(this.currTheme=="expertusoneV2"){changeDialogPopUI();$(".expertusV2PopupContainer").wrapAll('<div class="reports-delete-popup" />');
}$("#delete-msg-wizard").show();$("#delete-object-dialog .ui-widget-content").css("z-index",c+10);$(".ui-widget-overlay").css("z-index",c+1);$(".ui-dialog-titlebar-close").click(function(){$("#delete-msg-wizard").remove();});}catch(f){}}function contentVersionListDisplay(d){try{var b="#admin-version-list-"+d+"-pagination_toppager";var a='$("#root_admin").data("narrowsearch")';$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");$("#admin-version-list-"+d+"-pagination").jqGrid({url:EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/manage/content/version-list/"+d),datatype:"json",mtype:"GET",colModel:[{name:"Detail",index:"detail",title:false,width:500,widgetObj:a,sortable:false,formatter:displayVersionDetail}],rowNum:5,rowList:[5,10,15],pager:b,viewrecords:true,multiselect:false,emptyrecords:"",toppager:false,height:"auto",width:650,loadtext:"",recordtext:"",pgtext:"{0} "+Drupal.t("LBL981")+" {1}",loadui:false,loadComplete:callbackVersionListDataGrid}).navGrid("#pager-datagrid-"+d,{add:false,edit:false,del:false,search:false,refreshtitle:true});
$("#admin-version-list-"+d+"-pagination_toppager").hide();}catch(c){}}function callbackVersionListDataGrid(b,k,j){try{var a=5;var c=$("#root-admin").data("narrowsearch");$("#admin-version-list-"+b.contentId+"-pagination").show();$("#gview_admin-version-list-"+b.contentId+"-pagination").find(".ui-jqgrid-bdiv > div:first").css("position","static");if(Drupal.settings.ajaxPageState.theme=="expertusoneV2"){$("#gview_admin-version-list-"+b.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable div.edit-class-list table.edit-content-version-list-container").css("border-bottom","0").css("padding-bottom","0").css("display","block");}else{$("#gview_admin-version-list-"+b.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable div.edit-class-list table.edit-content-version-list-container").css("border-bottom","1px dotted #CCCCCC").css("padding-bottom","5px").css("display","block");}$("#gview_admin-version-list-"+b.contentId+"-pagination").find(".ui-jqgrid-bdiv > div table.ui-jqgrid-btable tbody tr:last-child table.edit-content-version-list-container").css("border","none");
var g=document.getElementById("modal-content");var f=$("#admin-version-list-"+b.contentId+"-pagination").jqGrid("getGridParam","records");if(f==0){$("#admin-version-list-"+b.contentId+"-pagination").css("display","block");var h=Drupal.t("MSG381")+".";$("#admin-version-list-"+b.contentId+"-pagination").html('<tr><td class="border-style-none" width="840px"><div id="add-edit-class-norecords" class="no-records-msg">'+h+"</div></td></tr>");$("#admin-version-list-"+b.contentId+"-pagination").css("text-align","center");$(".border-style-none").css("border","0");}else{if(f>a){$("#admin-version-list-"+b.contentId+"-pagination_toppager").show();$("#add-edit-class-norecords").css("display","none");}else{$("#admin-version-list-"+b.contentId+"-pagination_toppager").hide();$("#add-edit-class-norecords").css("display","none");}}initGridVersionPagination(b.contentId);$("body").data("mulitselectdatagrid").destroyLoader("paint-class-search-results-datagrid");$("#admin-version-list-pagination-"+b.contentId+" tr").click(function(e){e.stopPropagation();
});$(".edit-class-list-container").last().css("border","0px none");var d=$(document).height();$("#modalBackdrop").css("height",d+"px");vtip();Drupal.ajax.prototype.commands.CtoolsModalAdjust();}catch(i){}}function initGridVersionPagination(b){try{$("#prev_admin-version-list-"+b+"-pagination_toppager").click(function(c){if(!$("#prev_admin-version-list-"+b+"-pagination_toppager").hasClass("ui-state-disabled")){$("#admin-version-list-"+b+"-pagination").hide();$("#gview_admin-version-list-"+b+"-pagination").css("min-height","50px");$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");}});$("#next_admin-version-list-"+b+"-pagination_toppager").click(function(c){if(!$("#next_admin-version-list-"+b+"-pagination_toppager").hasClass("ui-state-disabled")){$("#admin-version-list-"+b+"-pagination").hide();$("#gview_admin-version-list-"+b+"-pagination").css("min-height","50px");$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");
}});$(".ui-pg-input").keyup(function(c){if(c.keyCode==13){$("#admin-version-list-"+b+"-pagination").hide();$("#gview_admin-version-list-"+b+"-pagination").css("min-height","50px");$("body").data("mulitselectdatagrid").createLoader("paint-class-search-results-datagrid");}});}catch(a){}}function displayVersionDetail(a,b,d){try{return d.detail;}catch(c){}}function loadDataGrid(j,o,g,a,d,c,f){try{var p=(j=="edit"||j=="view_only")?true:false;j=(j=="view_only")?"view":j;var i=(j=="edit")?true:false;var q=(j=="edit")?false:true;var m="";var h=$("body").data("mulitselectdatagrid");h.mode=j;h.type=o;h.entityId=a;h.entityType=d;h.uniqueId=o+"-"+a+"-"+d;uniqueId=o+"-"+a+"-"+d;var b=" ";b='<input type="checkbox" id="multiselect-selectall-'+uniqueId+'" class="multiselect-selectall" onclick ="checkboxSelectedUnselectedCommon(this);" />';var n=$("#search_all_moveuser_type-hidden").val();this.currTheme=Drupal.settings.ajaxPageState.theme;if(this.currTheme=="expertusoneV2"&&navigator.userAgent.indexOf("Chrome")>0){var l=480;
}else{var l=500;}$("#datagrid-container-"+uniqueId).jqGrid({url:EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/content/grid-moveusers/"+j+"/"+o+"/"+encodeURIComponent(g)+"/"+a+"/"+d+"/"+c),datatype:"json",mtype:"GET",postData:{searhType:n},colNames:[Drupal.t("LBL054"),Drupal.t("LBL691"),Drupal.t("LBL262"),Drupal.t("LBL102"),b],colModel:[{name:"UserName",index:"UserName",title:false,width:70,widgetObj:m,formatter:h.displayGridValues,sortable:true},{name:"FullName",index:"FullName",title:false,width:90,widgetObj:m,formatter:h.displayGridValues,sortable:true},{name:"ClassTitle",index:"ClassTitle",title:false,width:90,widgetObj:m,formatter:h.displayGridValues,sortable:true},{name:"Status",index:"Status",title:false,width:70,widgetObj:m,formatter:h.displayGridValues,sortable:true},{name:"MultiselectCheck",index:"MultiselectCheck",title:true,width:17,widgetObj:m,formatter:h.displayGridValues,sortable:false,hidden:q}],rowNum:6,rowList:[10,25,50],pager:"#pager-datagrid-"+uniqueId,viewrecords:true,multiselect:false,emptyrecords:"",sortorder:"desc",toppager:true,botpager:false,height:"auto",width:l,loadtext:"",recordtext:"",pgtext:"{0} of {1}",loadui:false,loadComplete:h.callbackDataGrid}).navGrid("#pager-datagrid-"+uniqueId,{add:false,edit:false,del:false,search:false,refreshtitle:true});
$("#datagrid-container-"+uniqueId+"_toppager_center .search-pagination-right").find("span:first-child").remove();if(Drupal.settings.ajaxPageState.theme=="expertusoneV2"){$("#datagrid-container-"+uniqueId+"_MultiselectCheck").find("div").css("margin-left","2px");}if(navigator.userAgent.indexOf("Chrome")>0){$("#gview_datagrid-container-"+uniqueId+" .ui-jqgrid-bdiv #datagrid-container-"+uniqueId+" tr.jqgfirstrow td:nth-child(4)").css("width","92px");}}catch(k){}}function searchVersionDataGrid(g,k,d,a,c,b){try{var i=k+"-"+a+"-"+c;$("body").data("mulitselectdatagrid").createLoader("datagrid-div-"+i);d=d.replace(/\//g,"|");var f=EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("administration/content/grid-moveusers/"+g+"/"+k+"/"+encodeURIComponent(d)+"/"+a+"/"+c+"/"+b);var j=$("#search_all_moveuser_type-hidden").val();$("#datagrid-container-"+i).setGridParam({url:f,postData:{searhType:j}});$("#datagrid-container-"+i).trigger("reloadGrid",[{page:1}]);}catch(h){}}function moreBrowseSearchHideShow(){try{$("#select-list-dropdown-list").slideToggle();
$("#select-list-dropdown-list li:last").css("border-bottom","0px none");}catch(a){}}function moreBrowseSearchTypeText(c,a){try{$("#select-list-dropdown").text(c);$("#hosted_type").val(a);}catch(b){}}function convertVideoForMobileAccess(){try{$.ajax({url:"/VideoConversionForMobile.php"});}catch(a){}}$("#content-addedit-form").live("click",function(a){if($("#select-list-dropdown-list").css("display","block")){if(!$(a.target).hasClass("select-list-dropdown-link")){$("#select-list-dropdown-list").css("display","none");}}});$("#edit-content-data").live("click",function(){try{$(this).change(function(){$("#BrowserVisible").find("#FileField").focus();var b=this.files;if(b&&b.length){$("#BrowserVisible").find("#FileField").val(b[0].name);}else{$("#BrowserVisible").find("#FileField").val($(this).val().replace("C:\\fakepath\\",""));}});}catch(a){}});$("body").click(function(a){try{if(a.target.id!="pub-unpub-action-btn"){$(".catalog-pub-add-list,.catalog-pub-add-list-move").hide();}if(a.target.id!="admin-dropdown-arrow"){$("#select-moveuser-dropdown-list dropdown-link-font ").hide();
}if(a.target.id!="transfer-dd-arrow"){$("#transfer-dd-list").css("visibility","hidden");}}catch(b){}});function setPosition(c,g,d){try{var a=$("#"+g).offset().top;var f=$(c).offset().top;$("#"+g).html(actionLink+" "+d);$(c).css("position","relative");$(c).css("top",a-f+"px");}catch(b){}}function contentShowHideDropDown(){try{var a=$("#transfer-dd-list").css("visibility");if(a=="hidden"){$("#transfer-dd-list").css("visibility","visible");}else{$("#transfer-dd-list").css("visibility","hidden");}}catch(b){}}function transUserList(){try{if(isPopupOpen!=1){contentShowHideDropDown();var b=$("#transfer-dd-list").width();if(b<"50"){$("#transfer-dd-list").css("left","108px");$(".transfer-dd-list-arrow").css("left",b/2+"px");}else{if(b>"100"&&b<"160"){$("#transfer-dd-list").css("left","auto");$(".transfer-dd-list-arrow").css("left",b/2+"px");}else{if(b>"160"&&b<="200"){$("#transfer-dd-list").css("left","35px");$(".transfer-dd-list-arrow").css("left",b/2+"px");}}}}return false;}catch(a){}}(function($){Drupal.ajax.prototype.commands.expVideoPreviewQtip=function(ajax,response,status){try{$(".active-qtip-div").remove();
var qtipObjJSON=$("#"+ajax.element.id).attr("data");var qtipObj=eval("("+qtipObjJSON+")");var popupHolderId=qtipObj.holderId;var popupId=qtipObj.id;var renderedHeight=(typeof qtipObj.renderedHeight=="undefined")?230:qtipObj.renderedHeight;$("#"+popupHolderId).html(response.html);var qtipPlacement="above";var triggerPos=$("#"+ajax.element.id).offset();var availableSpaceAbove=triggerPos.top-$(window).scrollTop();if(availableSpaceAbove<350){qtipPlacement="below";}var qtipArrowPos={};var qtipBoxPos={};var triggerWidth=$("#"+ajax.element.id).width();var triggerHeight=$("#"+ajax.element.id).height();if(qtipPlacement=="above"){$("#"+popupHolderId).append('<div id="arrow-'+popupId+'" class="bottom-qtip-tip active-qtip-div qtip-arrow" style="display:none"></div>');var downArrowImageWidth=$("#"+popupHolderId+" .bottom-qtip-tip").width();var downArrowLeftAdjust=(downArrowImageWidth-triggerWidth)/2;qtipArrowPos.left=triggerPos.left-downArrowLeftAdjust;var downArrowImageHeight=$("#"+popupHolderId+" .bottom-qtip-tip").height();
qtipArrowPos.top=triggerPos.top-(downArrowImageHeight-15);qtipBoxPos.left=triggerPos.left-downArrowLeftAdjust-60;if($(".page-administration-contentauthor-video").size()>0){qtipBoxPos.top=qtipArrowPos.top-renderedHeight+12-26;}else{qtipBoxPos.top=qtipArrowPos.top-renderedHeight+12;}}else{$("#"+popupHolderId).append('<div id="arrow-'+popupId+'"class="bottom-qtip-tip-up active-qtip-div qtip-arrow" style="display:none"></div>');var upArrowImageWidth=$("#"+popupHolderId+" .bottom-qtip-tip-up").width();var upArrowLeftAdjust=((upArrowImageWidth-triggerWidth)/2);qtipArrowPos.left=triggerPos.left-upArrowLeftAdjust;var upArrowImageHeight=$("#"+popupHolderId+" .bottom-qtip-tip-up").height();qtipArrowPos.top=triggerPos.top+triggerHeight-5;qtipBoxPos.left=triggerPos.left-upArrowLeftAdjust-60;qtipBoxPos.top=qtipArrowPos.top+upArrowImageHeight-24;}$("#arrow-"+popupId).css({position:"absolute","z-index":"1003"}).show().offset(qtipArrowPos);$("#"+popupId).css({position:"absolute","z-index":"1002"}).show().offset(qtipBoxPos);
Drupal.attachBehaviors();}catch(e){}};Drupal.ajax.prototype.commands.initializeVideoJSPlayer=function(ajax,response,status){try{disposeVideoJSPlayer(response.video_id);if(videojs!==undefined&&response.video_id!==undefined){videojs(response.video_id,{},function(){var vodPlayer=this;vodPlayer.play();vodPlayer.on("mouseout",function(){this.controls(false);});vodPlayer.on("mouseover",function(){this.controls(true);});});}}catch(e){}};jQuery(".version-content-transfer-user-txt").live("click",function(){$("#transfer-dd-arrow").click();});$(".video-preview-popup .qtip-close-button").live("click",function(){disposeVideoJSPlayer("all");});})(jQuery);Drupal.ajax.prototype.commands.displayContentConfirmationmessagewizard=function(k,c,d){try{this.currTheme=Drupal.settings.ajaxPageState.theme;var l=c.cId;var a=c.cvId;var f=c.trgElmt;var i="ContentMoveUsers-"+l+"-"+a;$("#contentmoveuser-msg-wizard").remove();$('input[name="hidden_move_content_users_'+l+"-"+a+'"]').attr("value",0);var g="";g+='<div id="contentmoveuser-msg-wizard" style="display:none; padding: 0px;">';
g+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';if($("#multiselect-selectall-ContentMoveUsers-"+l+"-"+a).attr("checked")||f=="Move_All"){var j=Drupal.t("MSG776")+" "+Drupal.t("MSG777")+". "+Drupal.t("MSG728");}else{var j=Drupal.t("MSG778")+" "+Drupal.t("MSG777")+"?";}g+='<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+j+"</td></tr>";g+="</table>";g+="</div>";$("body").append(g);var m={};m[Drupal.t("LBL109")]=function(){$('input[name="hidden_completedvalue_'+i+'"]').attr("value",0);$("#contentmoveuser-msg-wizard").remove();$.fn.refreshVersionList(l);};if(availableFunctionalities.exp_sp_esignature!=undefined&&availableFunctionalities.exp_sp_esignature!=""){var b={popupDiv:"catalog-publish-unpublis-dialog",esignFor:"displayContentConfirmationmessagewizard",catalogId:dupId,catalogType:"Class",actionStatus:"lrn_cls_sts_atv",rowObj:this};m[Drupal.t("Yes")]=function(){$.fn.getNewEsignPopup(b);$(this).dialog("destroy");$(this).dialog("close");$("#contentmoveuser-msg-wizard").remove();
$.fn.refreshVersionList(l);};}else{m[Drupal.t("Yes")]=function(){$('input[name="hidden_move_content_users_'+l+"-"+a+'"]').attr("value",2);$(this).dialog("destroy");$(this).dialog("close");$("#contentmoveuser-msg-wizard").remove();$('input[name="hidden_completedvalue_'+i+'"]').attr("value",0);$('input[name = "'+f+'"]').click();$.fn.refreshVersionList(l);$.fn.refreshMoveUsersQtip(l,a);};}$("#contentmoveuser-msg-wizard").dialog({position:[(getWindowWidth()-400)/2,200],bgiframe:true,width:550,resizable:false,modal:true,title:Drupal.t("LBL947"),buttons:m,closeOnEscape:false,draggable:false,zIndex:10005,overlay:{opacity:0.9,background:"black"}});$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)").addClass("removebutton").end();$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").before('<div class="admin-save-button-left-bg"></div>');$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").addClass("admin-save-button-middle-bg");$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)").after('<div class="admin-save-button-right-bg"></div>');
if($.browser.msie&&parseInt($.browser.version,10)=="9"&&this.currTheme=="expertusoneV2"||$.browser.msie&&parseInt($.browser.version,10)=="8"&&this.currTheme=="expertusoneV2"){$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)").next(".white-btn-bg-right").css("margin-right","0px");}$("#contentmoveuser-msg-wizard").show();$(".ui-dialog-titlebar-close").click(function(){$("#contentmoveuser-msg-wizard").remove();});changeDialogPopUI();}catch(h){}};function setCompleteUserValue(a){$('input[name="hidden_completedvalue_'+a+'"]').attr("value",0);$.each($("#datagrid-container-"+a).find(".checkbox-selected"),function(){if(($(this).find(".completed_sts").attr("checked"))==true){$('input[name="hidden_completedvalue_'+a+'"]').attr("value",1);}});}