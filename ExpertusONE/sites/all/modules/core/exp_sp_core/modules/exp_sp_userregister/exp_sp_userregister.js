var ajaxCall = false;
var ajaxResponse = null;
function reloadCaptchaImg(capNum){
	try{
	$('#hiddencaptxt').val(capNum);
	}catch(e){
		// to do
	}
};

function confirmationPopup(accntSet){
        try{
                this.currTheme = Drupal.settings.ajaxPageState.theme; var message = 'Address you have entered is invalid. Do you still want to continue with the entered address?';
            $('#addrval-confirmation-wizard').remove();
                var html = '';
                html += '<div id="addrval-confirmation-wizard" style="display:none; padding: 13px;">';
                html += '<table width="100%" class="autologout-confirmation-table" cellpadding="0" cellspacing="0" border="0" valign="center">';
                html += '<tr><td height="10"></td></tr>';
                html += '<tr>';
                html += '<td align="center" id="addrval-confirmation-content"><span>'+ Drupal.t('MSG830') +'</span></td>';
                html += '</tr>';
                html +='</table>';
                html +='</div>';
                $("body").append(html);
                var confButton = {};
                confButton[Drupal.t('No')] = function() {
                                $(this).dialog('destroy');
                                $(this).dialog('close');
                                return false;
                };
                confButton[Drupal.t('Yes')] = function(){
                                $('#addrval-status').val(0);
                             // $('#ajax-userregister').submit();
                             if(accntSet == 1){
                               $("#esign-account-save").click();
                             }
                                $(this).dialog('destroy');
                                $(this).dialog('close');
                                                                                                                                return true;
                }
                            
                            $("#addrval-confirmation-wizard").dialog({
                                position:[(getWindowWidth()-500)/2,100],
                                autoOpen: false,
                                bgiframe: true,
                                width: 365,
                                resizable: false,
                                modal: true,
                                title: Drupal.t('MSG831'),
                                buttons: confButton,
                                dialogClass: 'exp_sp_session_timeout-dialog',
                                closeOnEscape: false,
                                draggable: false,
                                zIndex : 10005,
                                overlay: {
                                   opacity: 0.9,
                                   background: "black"
                                }
                            });
                $('.ui-dialog').wrap("<div id='addrval-confirmation-wizard-dialog'></div>");
                $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();

                changeDialogPopUI();
                $("#addrval-confirmation-wizard").show();
                $("#addrval-confirmation-wizard").dialog('open')
               // return;
            } catch(e) {
            //console.log(e.stack);
            //console.log(e);
        }
}

function createLoader(resultPanel){
    var divid= "loaderdiv"+resultPanel;
    if(document.getElementById(divid)==null && document.getElementById(resultPanel)!=null){
        var divobj=document.createElement('div');
        divobj.id=divid;
        var height;
        if(navigator.appName=="Microsoft Internet Explorer") {
            height = document.getElementById(resultPanel).offsetHeight;
        } else {
            height = document.getElementById(resultPanel).clientHeight;
        }
        var width = document.getElementById(resultPanel).offsetWidth;
        divobj.innerHTML='<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>';
        $($("#"+resultPanel)).prepend(divobj);
        $('#'+divid).addClass("loadercontent");
        $('#'+divid).width(width);
        $('#'+divid).height(height);
        $('#'+divid).css("z-index",1003);
}  }

function checkusername(obj){
	try{
	objname=obj.value;
	objid=obj.id;
    if(objname != null && objname != undefined && objname != ""){
    var url1=resource.base_host+"/?q=learning/register/usernamecheck/"+objname;
	$.ajax({
			type: "GET",
		    url : url1,
		    complete : function(xmlHttpRequest, textStatus) {
			    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
			        return;
			    }

			    var responseText = xmlHttpRequest.responseText;
			    if(responseText==1){
			    	var errorMessages = new Array();
			    	errorMessages[0] = Drupal.t('ERR016');
					var message_call = expertus_error_message(errorMessages,'error');
					$('#show_expertus_message').html(message_call);
					$('#show_expertus_message').show();
			    }else{
			    	$("#erruser").removeClass("error");
			   		$("#"+objid).removeClass("error");
			    }
		    }
		    
	   });
}
	}catch(e){
		// to do
	}
}

//User register alignment script
$(document).ready(function(){
	try{    
		var pwdCondLen=$('.password-note-cls').find('ul li').length;
		if(pwdCondLen > 1)
		{
			$('.password-note-cls').parents('tr').find('td:first-child').css({"vertical-align":"top","padding-top":"8px"});
		}
		if(Drupal.settings.widget.widgetCallback==true){		
			$('.reg-link-back a').click(function() { location.href = document.referrer;});
		}
		/*var country = '';
		country = Drupal.settings.register.country_name; //value get from userregister module
		var offset = new Date().getTimezoneOffset();
		offset = 'GMT'+((offset<0? '+':'-')+ // Note the reversed sign!
				pad(parseInt(Math.abs(offset/60)), 2)+':'+																																																			
				pad(Math.abs(offset%60), 2));
		var idValue = $('#edit-tzone option:contains("'+offset+'")');

		var mySelect= [];
		if (typeof idValue[0].value !== "undefined" && country != '') {		
			$(idValue).each(function( index ) {
				if ($( this ).text().indexOf(country) >= 0) {
					var offsetValue = idValue[index].value;		      
					mySelect.push(offsetValue);
				} 
			});
		}
		if(mySelect == '' || mySelect == ' undefined') {
			var selectOne = '';
		} else {
			var selectOne = mySelect.shift();
		}
		$('#edit-tzone').val(selectOne);*/
		lenghtNameFadeout();
	}catch(e){
		// to do
	}
});

function pad(number, length){
    var str = "" + number
    while (str.length < length) {
        str = '0'+str
    }
    return str
}
function userregistercheckedall(){
	try{
		
		$(document).ready(function(){ 
			
		    $("#usr_select").change(function(){
		    	
		    	$(".attach-group-list").prop('checked', $(this).prop("checked"));
		    	
		    	if($('#usr_select').attr('checked')) {
		    		
		    		$('.usr-muliselect').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    		$('.multichk').removeClass('checkbox-unselected').addClass('checkbox-selected');
		    	} else {
		    		$('.usr-muliselect').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    		$('.multichk').removeClass('checkbox-selected').addClass('checkbox-unselected');
		    	}
		    
		    });
		   
	    });	
	}
		
	catch(e){
			
	}
}

function checkUserLoggedIn() {
	$.ajax({
		url: resource.base_host+"/?q=learning/active_login",
		type: 'GET',
		beforeSend: function() {
			$('#message-container').remove();
			ajaxCall = true;
			ajaxResponse = undefined;
		},
		error: function(xhr, testStatus, error) {
			ajaxResponse = "success";
			ajaxCall = false;
			$('#ajax-userregister').find(':submit').click();
		},
		success: function(data, textStatus, xhr) {
			ajaxCall = false;
			ajaxResponse = "error";
			
//			var errorMessages = new Array(data);
//	    	errorMessages[0] = Drupal.t('user has active session');
			var message_call = expertus_error_message(new Array(data),'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
			$('#ajax-userregister').find(':submit').click();
		}
	});
}

Drupal.behaviors.UserRegister = {
		attach: function (context, settings) {
			// Overwrite beforeSubmit                     
			Drupal.ajax.prototype.beforeSubmit = function (form_values, element, options) {
				if(element.attr('id') == 'ajax-userregister') {
                                    createLoader('ajax-userregister');
                                       
					if(ajaxResponse == "success") {
						ajaxResponse = undefined;
						return true;
					} else if(ajaxResponse == "error") {
						ajaxResponse = undefined;
						this.ajaxing = false;
						return false;
					}
					checkUserLoggedIn();
					this.ajaxing = false;
                                        $('#loaderdivajax-userregister').remove();
					return false;
				}
			};
		}
                
};

function checkCountryDisableuserreg(){
	if($('#userreg_Country').is(':checked')){
	 $('#userreg_State').attr('disabled', false);
  }else {
	 $('#userreg_State').parent().removeClass('checkbox-selected');
	 $('#userreg_State').parent().addClass('checkbox-unselected');
	 $('#userreg_State').attr('checked','');
	 $('#userreg_State').attr('disabled',true);
  }
}
function stateDisabledMessageuserreg(){
 if(!($('#userreg_Country').is(':checked'))){
	 var err1 = Drupal.t('LBL674');  
	 var err2 = Drupal.t('LBL039');
	 error=err1+' '+err2;
	 var message_call = expertus_error_message(new Array(error),'error');
	 $('#show_expertus_message').show();
	 $('#show_expertus_message').html(message_call);
  }
}
