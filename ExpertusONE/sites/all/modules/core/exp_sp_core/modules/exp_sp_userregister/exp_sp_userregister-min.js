var ajaxCall=false;var ajaxResponse=null;function reloadCaptchaImg(a){try{$("#hiddencaptxt").val(a);}catch(b){}}function confirmationPopup(b){try{this.currTheme=Drupal.settings.ajaxPageState.theme;var d="Address you have entered is invalid. Do you still want to continue with the entered address?";$("#addrval-confirmation-wizard").remove();var c="";c+='<div id="addrval-confirmation-wizard" style="display:none; padding: 13px;">';c+='<table width="100%" class="autologout-confirmation-table" cellpadding="0" cellspacing="0" border="0" valign="center">';c+='<tr><td height="10"></td></tr>';c+="<tr>";c+='<td align="center" id="addrval-confirmation-content"><span>'+Drupal.t("MSG830")+"</span></td>";c+="</tr>";c+="</table>";c+="</div>";$("body").append(c);var a={};a[Drupal.t("No")]=function(){$(this).dialog("destroy");$(this).dialog("close");return false;};a[Drupal.t("Yes")]=function(){$("#addrval-status").val(0);if(b==1){$("#esign-account-save").click();}$(this).dialog("destroy");$(this).dialog("close");
return true;};$("#addrval-confirmation-wizard").dialog({position:[(getWindowWidth()-500)/2,100],autoOpen:false,bgiframe:true,width:365,resizable:false,modal:true,title:Drupal.t("MSG831"),buttons:a,dialogClass:"exp_sp_session_timeout-dialog",closeOnEscape:false,draggable:false,zIndex:10005,overlay:{opacity:0.9,background:"black"}});$(".ui-dialog").wrap("<div id='addrval-confirmation-wizard-dialog'></div>");$(".ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)").addClass("removebutton").end();changeDialogPopUI();$("#addrval-confirmation-wizard").show();$("#addrval-confirmation-wizard").dialog("open");}catch(f){}}function createLoader(c){var e="loaderdiv"+c;if(document.getElementById(e)==null&&document.getElementById(c)!=null){var d=document.createElement("div");d.id=e;var a;if(navigator.appName=="Microsoft Internet Explorer"){a=document.getElementById(c).offsetHeight;}else{a=document.getElementById(c).clientHeight;}var b=document.getElementById(c).offsetWidth;d.innerHTML='<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+a+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+a+'px">&nbsp;</td></tr></table>';
$($("#"+c)).prepend(d);$("#"+e).addClass("loadercontent");$("#"+e).width(b);$("#"+e).height(a);$("#"+e).css("z-index",1003);}}function checkusername(c){try{objname=c.value;objid=c.id;if(objname!=null&&objname!=undefined&&objname!=""){var b=resource.base_host+"/?q=learning/register/usernamecheck/"+objname;$.ajax({type:"GET",url:b,complete:function(f,h){if((parseInt(f.status)!=0)&&(parseInt(f.status/100)!=2)){return;}var e=f.responseText;if(e==1){var g=new Array();g[0]=Drupal.t("ERR016");var d=expertus_error_message(g,"error");$("#show_expertus_message").html(d);$("#show_expertus_message").show();}else{$("#erruser").removeClass("error");$("#"+objid).removeClass("error");}}});}}catch(a){}}$(document).ready(function(){try{var a=$(".password-note-cls").find("ul li").length;if(a>1){$(".password-note-cls").parents("tr").find("td:first-child").css({"vertical-align":"top","padding-top":"8px"});}if(Drupal.settings.widget.widgetCallback==true){$(".reg-link-back a").click(function(){location.href=document.referrer;
});}lenghtNameFadeout();}catch(b){}});function pad(b,a){var c=""+b;while(c.length<a){c="0"+c;}return c;}function userregistercheckedall(){try{$(document).ready(function(){$("#usr_select").change(function(){$(".attach-group-list").prop("checked",$(this).prop("checked"));if($("#usr_select").attr("checked")){$(".usr-muliselect").removeClass("checkbox-unselected").addClass("checkbox-selected");$(".multichk").removeClass("checkbox-unselected").addClass("checkbox-selected");}else{$(".usr-muliselect").removeClass("checkbox-selected").addClass("checkbox-unselected");$(".multichk").removeClass("checkbox-selected").addClass("checkbox-unselected");}});});}catch(a){}}function checkUserLoggedIn(){$.ajax({url:resource.base_host+"/?q=learning/active_login",type:"GET",beforeSend:function(){$("#message-container").remove();ajaxCall=true;ajaxResponse=undefined;},error:function(c,a,b){ajaxResponse="success";ajaxCall=false;$("#ajax-userregister").find(":submit").click();},success:function(b,d,c){ajaxCall=false;
ajaxResponse="error";var a=expertus_error_message(new Array(b),"error");$("#show_expertus_message").html(a);$("#show_expertus_message").show();$("#ajax-userregister").find(":submit").click();}});}Drupal.behaviors.UserRegister={attach:function(a,b){Drupal.ajax.prototype.beforeSubmit=function(e,d,c){if(d.attr("id")=="ajax-userregister"){createLoader("ajax-userregister");if(ajaxResponse=="success"){ajaxResponse=undefined;return true;}else{if(ajaxResponse=="error"){ajaxResponse=undefined;this.ajaxing=false;return false;}}checkUserLoggedIn();this.ajaxing=false;$("#loaderdivajax-userregister").remove();return false;}};}};function checkCountryDisableuserreg(){if($("#userreg_Country").is(":checked")){$("#userreg_State").attr("disabled",false);}else{$("#userreg_State").parent().removeClass("checkbox-selected");$("#userreg_State").parent().addClass("checkbox-unselected");$("#userreg_State").attr("checked","");$("#userreg_State").attr("disabled",true);}}function stateDisabledMessageuserreg(){if(!($("#userreg_Country").is(":checked"))){var c=Drupal.t("LBL674");
var a=Drupal.t("LBL039");error=c+" "+a;var b=expertus_error_message(new Array(error),"error");$("#show_expertus_message").show();$("#show_expertus_message").html(b);}}