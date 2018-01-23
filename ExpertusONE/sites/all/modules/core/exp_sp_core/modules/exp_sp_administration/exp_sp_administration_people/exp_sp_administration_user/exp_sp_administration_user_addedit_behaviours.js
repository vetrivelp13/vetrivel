( function($) {
  try{
  function initAddEditPasswordField(currField) {
    try{
  // Set the on blur, on focus and on change behaviours
  $(currField).blur(function(event) {
	try{
    var pwdIdClass = $(this).data('password-idclass');
    var cPwdIdClass = $(this).data('cpassword-idclass');
    var thisIsPwdField = $(this).hasClass(pwdIdClass) ? true : false;
    var pwdVal = $('.' + pwdIdClass).val();
    var cPwdVal = $('.' + cPwdIdClass).val();
    //alert('blur called for ' + (thisIsPwdField? 'pwd' : 'cpwd') + ' field with pwd = ' + pwdVal + ', cpwd = ' + cPwdVal);
    
    if(pwdVal!="" && cPwdVal!=""){
    	if(pwdVal!=cPwdVal){
	    	  $('#password-mismatch').show();
	    	  $(".addedit-password-field").addClass("error");
	    	  $(".addedit-cpassword-field").addClass("error");
    	}
    	else{
    		  $('#password-mismatch').hide();
    	      $(".addedit-password-field").removeClass("error");
    	      $(".addedit-cpassword-field").removeClass("error");
    	}
    	Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
    }
	}catch(e){
		// To Do
	}
  });

  
/*  $(currField).focus(function(event) {
    var pwdIdClass = $(this).data('password-idclass');
    var cPwdIdClass = $(this).data('cpassword-idclass');
    var thisIsPwdField = $(this).hasClass(pwdIdClass) ? true : false;
    var pwdVal = $('.' + pwdIdClass).val();
    var cPwdVal = $('.' + cPwdIdClass).val();
    //alert('focus called for ' + (thisIsPwdField? 'pwd' : 'cpwd') + ' field with pwd = ' + pwdVal + ', cpwd = ' + cPwdVal);
  });

  $(currField).change(function(event) {
    var pwdIdClass = $(this).data('password-idclass');
    var cPwdIdClass = $(this).data('cpassword-idclass');
    var thisIsPwdField = $(this).hasClass(pwdIdClass) ? true : false;
    var pwdVal = $('.' + pwdIdClass).val();
    var cPwdVal = $('.' + cPwdIdClass).val();
    //alert('change called for ' + (thisIsPwdField? 'pwd' : 'cpwd') + ' field with pwd = ' + pwdVal + ', cpwd = ' + cPwdVal);
  }); */
    }catch(e){
		// To Do
	}
}
  
  Drupal.behaviors.passwordFields =  {
    attach: function (context, settings) {
    try{
      $('.addedit-password-field:not(.addedit-password-field-initialized)').addClass('addedit-password-field-initialized').each(function () {
        //alert('In Drupal.behaviors.passwordFields pwd attach()');
        initAddEditPasswordField(this);
      });
      
      $('.addedit-cpassword-field:not(.addedit-cpassword-field-initialized)').addClass('addedit-cpassword-field-initialized').each(function () {
        //alert('In Drupal.behaviors.passwordFields cpwd attach()');
        initAddEditPasswordField(this);
      });
    }catch(e){
		// To Do
	}
    }
  };
  }catch(e){
		// To Do
	}
})(jQuery);

function checkusernameadmin(obj){
	try{
	objname=obj.value;
	objid=obj.id;
    if(objname != null && objname != undefined && objname != ""){//administration/people/user/usernamecheck/addedit-edit-user_name addedit-edit-textfield form-text
    var url1=resource.base_host+"/?q=administration/people/user/usernamecheck/"+objname;
	$.ajax({
			type: "GET",
		    url : url1,
		    complete : function(xmlHttpRequest, textStatus) {
			    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
			        return;
			    }

			    var responseText = xmlHttpRequest.responseText;
			    if(responseText==1){
			    	$("#username-mismatch").show();
			    	$(".addedit-edit-user_name").addClass("error");
			    	
			    }else{
			    	$("#username-mismatch").hide();
			    	//$("#erruser").removeClass("error");
			    	$(".addedit-edit-user_name").removeClass("error");
			   		
			    }
			    Drupal.ajax.prototype.commands.CtoolsModalAdjust(null, null, null);
		    }
		    
	   });
}
	}catch(e){
		// To Do
	}
}
//48472: Once I removed manager role, still "Team" module showing.
var already_is_manager_validation_checked;
function checkManagerUncheck(){
	 var selected = $("#edit-roles option:selected");
     var is_manager = false;
     already_is_manager_validation_checked = false;
     selected.each(function () {
         if($(this).val()=='is_manager'){         
        	 is_manager = true;
        	 already_is_manager_validation_checked = false;
         }
     });
     //console.log(already_is_manager_validation_checked);
     if(is_manager === false && already_is_manager_validation_checked === false){
    	 var title = Drupal.t('LBL082')+' '+Drupal.t('Manager')+' '+Drupal.t('LBL579');
    	 var message = Drupal.t('MSG729');
    	 try{
 		    $('#commonMsg-wizard').remove();
 		    $("#edit-roles").multiselect("refresh");
 		    var html = '';	    
 		    html+='<div id="commonMsg-wizard" style="display:none; padding: 0px;">';
 		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
 		    if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
 		   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+message+'</td></tr>';
 		    } else {
 		     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+message+'</td></tr>';
 		    }
 		    html+='</table>';
 		    html+='</div>';
 		    $("body").append(html);
 		    var closeButt={};
 		    closeButt[Drupal.t('LBL109')]=function(){
 		    	$("#edit-roles").multiselect("widget").find(":checkbox[value='is_manager']").attr("checked","checked");
 		    	$("#edit-roles option[value='is_manager']").attr("selected", 1);
 		    	$("#edit-roles").multiselect("refresh");
 		    	already_is_manager_validation_checked = false;
 		    	$(this).dialog('destroy');
 		    	$(this).dialog('close');
 		    	$('#commonMsg-wizard').remove();
 		    };
         	closeButt[Drupal.t('Yes')]=function(){already_is_manager_validation_checked = true;$(this).dialog('destroy');$(this).dialog('close');$("#commonMsg-wizard").remove();}; 
 		    $("#commonMsg-wizard").dialog({
 		        position:[(getWindowWidth()-400)/2,200],
 		        bgiframe: true,
 		        width:300,
 		        resizable:false,
 		        modal: true,
 		        title:title,
 		        buttons:closeButt,
 		        closeOnEscape: false,
 		        draggable:false,
 		        overlay:
 		         {
 		           opacity: 0.9,
 		           background: "black"
 		         }
 		    });
 		    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+message+" remove-reports'></div>");
 		    $('#delete-object-dialog.remove-reports').find('.ui-dialog').css("z-index","10100");
 		    $('#delete-object-dialog.remove-reports').next('.ui-widget-overlay').css("z-index","1030");
 		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
 		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
 		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
 		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
 		    $("#commonMsg-wizard").show();
 		    if($('#delete-object-dialog.remove-reports').is(':visible')) {
                $(".ui-multiselect-menu.edit-roles").hide();
            }
 			$('.ui-dialog-titlebar-close').click(function(){
 				$("#edit-roles").multiselect("widget").find(":checkbox[value='is_manager']").attr("checked","checked");
 		    	$("#edit-roles option[value='is_manager']").attr("selected", 1);
 		    	$("#edit-roles").multiselect("refresh");
 		    	already_is_manager_validation_checked = false;
 		        $("#commonMsg-wizard").remove();
 		    });
 			if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
 		    	changeDialogPopUI();
 		    }
 	 }catch(e){
 			// to do
 		}
     }
}
// change by ayyappans for 33432: Issue in Users
(function($) {
	$.widget("ui.peopleuser", {
		callMultiSelect: function(userId, ele){
			//alert(userId)
			var oc = $('#attr_open').val();
			var obj = this;
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			$('.multiselect-custom-dropdown-results').css('display','none');
			$('.ui-multiselect-menu').css('display','none');
			$('#avil_'+ele).css('display','block');
			$('#attr_open').val(ele);
			$('#avil_'+oc).prev().find('span:last')
			.removeClass('down-tip-arrow')
			.addClass('right-tip-arrow');

			$('#avil_'+ele).prev().find('span:last')
			.removeClass('right-tip-arrow')
			.addClass('down-tip-arrow');
			var orgList = $('#load_multiselect_oorgs').val();
			//Construct data load and autocomplete urls
			userId = userId == '' ? undefined : userId;
			if(ele == 'jobrole'){
				var jobroleList = $('#load_multiselect_jobrole').val();
				var url = this.constructUrl("administration/people/user/jobrole/"+userId+"/"+jobroleList);
				var autourl = this.constructUrl("administration/people/user/jobrole-autocomplete/"+ele);
				var list_height = 200;
				var titlelength = 28;
				var list_width = 250;
			}else if(ele == 'omanagers') {
				var otherMangerList = $('#load_multiselect_omanagers').val();
				var url = this.constructUrl("administration/people/user/othermanagers/"+userId+"/"+otherMangerList);
				var autourl = this.constructUrl("administration/people/user/othermanagers-autocomplete/"+ele);
				var list_height = 140;
				var titlelength = 15;
				var list_width = 188;
			}else if(ele == 'session_instructor') {
				if(($( ".set-wbubble-left" ).hasClass("bottom-qtip-tip-up")) && ($("td").hasClass('admin-addedit-class-instructor_expmeeting'))) {
					$('.bottom-qtip-tip-up').css('z-index','99');
				}
				var insList = $('#load_multiselect_session_instructor').val();
				var url = this.constructUrl("administration/session/allinstructor/"+userId+"/"+insList);
				var autourl = this.constructUrl("administration/session/allinstructor-autocomplete/"+ele);
				var list_height = 120;
				var titlelength = 8;
				var list_width = 160;
			}else{
				var url = this.constructUrl("administration/people/user/org/"+userId+"/"+orgList);
				var autourl = this.constructUrl("administration/people/user/org-autocomplete/"+ele);
				var list_height = 120;
				var titlelength = 15;
				var list_width = 188;
			}
			var option;
			option = {
					url: url,
					dataType:"json",
					titlelength: titlelength,
					searchfilter:{url:autourl,enable:true},
					rownum:20,
					width:list_width,
					height: list_height,
					onselect:this.onselectfn,
					onunselect:this.onunselectfn,
					helpText: {autocomplete:Drupal.t('Search for refine'),checkAll:'Select All',uncheckAll:'Unselect All'},
					widget:this,
					afterload: function() {
						$('#avil_'+ele).find('.label-text').addClass('vtip');
						vtip();
						$('#avil_'+ele).find('.label-text').each(function() {
							$('#avil_'+ele).data('expmultiselectDropdown')._labelBind($(this));
						});
						$('#autoimg').bind('click',function(){
							if(ele=='jobrole')
								$('#avil_'+ele).data('expmultiselectDropdown').options.url = obj.constructUrl("administration/people/user/jobrole/"+userId+"/"+$('#load_multiselect_jobrole').val());
							else if(ele=='omanagers'){
								$('#avil_'+ele).data('expmultiselectDropdown').options.url = obj.constructUrl("administration/people/user/othermanagers/"+userId+"/"+$('#load_multiselect_omanagers').val());
							}
							else if(ele=='session_instructor'){
								$('#avil_'+ele).data('expmultiselectDropdown').options.url = obj.constructUrl("administration/session/allinstructor/"+userId+"/"+$('#load_multiselect_session_instructor').val());
							}
							else {
								$('#avil_'+ele).data('expmultiselectDropdown').options.url = obj.constructUrl("administration/people/user/org/"+userId+"/"+$('#load_multiselect_oorgs').val());
							}
						});
					}
			};
			$('#avil_'+ele).expmultiselectDropdown(option);
			$('#avil_'+ele).css('display','block');
			//change by ayyappans for 41736: Issue in user creation page
			$('#avil_'+ele).find('.footeraccess').hide();
			$('#avil_'+ele).data('expmultiselectDropdown')._callScroll();
		},
		onselectfn: function(ele,data,obj) {
			var eid = $(ele).attr('id').replace('avil_','');
			var dt=new Array();
			var v = $('#load_multiselect_'+eid).val();
			if(v!=''){
				dt=v.split(',');
			}
			dt.push(data.value);
			$('#load_multiselect_'+eid).val(dt.join(','));
			$("#root-admin").data('peopleuser').updateSelectedText(eid, dt.length);
			$(ele).find('#chk .label-text').addClass('vtip');
			vtip();
			$(ele).find('.label-text').each(function() {
				$(ele).data('expmultiselectDropdown')._labelBind($(this));
			});
			if(document.getElementById("edit-session-presenter")){
				//$('input[id="change_instructor"]').attr('value',2);
				var x = document.getElementById("edit-session-presenter");
				var option = document.createElement("option");
				option.text = data.title;
				option.value = data.value;
				option.title = data.title;
				x.add(option);
			}
			$('#avil_'+eid+' #container').jScrollPane({});
		},
		onunselectfn: function(ele,data,obj) {
			var eid = $(ele).attr('id').replace('avil_','');
			var dt=new Array();
			var v = $('#load_multiselect_'+eid).val();
			if(v!=''){
				dt=v.split(',');
			}
			var index = $.inArray(data.value, dt);
			if(index>=0)
				dt.splice(index, 1);
			$('#load_multiselect_'+eid).val(dt.join(','));
			$("#root-admin").data('peopleuser').updateSelectedText(eid, dt.length);
			if(document.getElementById("edit-session-presenter")){
				//$('input[id="change_instructor"]').attr('value',2);
				var ind = $("#edit-session-presenter option[value='"+data.value+"']").index();
				var x = document.getElementById("edit-session-presenter");
				x.remove(ind);
			}
			$('#avil_'+eid+' #container').jScrollPane({});
		},
		updateSelectedText: function(eid, count) {
			var selectedText;
			//	for 41196: Can't able to save the page,system gets hang for a while
			if(count === undefined) {
				var dt=new Array();
				var v = $('#load_multiselect_'+eid).val();
				if(v!=''){
					dt=v.split(',');
				}
				count = dt.length;
			}
			if(count > 0) {
				selectedText = count + ' '+Drupal.t('LBL646');
			}
			else {
				selectedText = Drupal.t('LBL674');
			}
			$('#menu_'+eid).find('.selected-list-text').text(selectedText);
		}
	});
	$.extend($.ui.peopleuser.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function() {
	try{
		$("#root-admin").peopleuser();
	}catch(e){
			// To Do
	}
});
$(function() {
	try {
		$('body').live('click', function(event) {
			if($(event.target).is('.multiselect-custom-dropdown') || ($(event.target).parents('div.multiselect-custom-dropdown-results').length && $('.multiselect-custom-dropdown-results').is(':visible'))) {
				//change for 42102: Unable to move to any other modules while clicking any modules link from the user profile page
				return false;
			}
			else if (!$(event.target).parents('.multiselect-custom-dropdown-results, .multiselect-custom-dropdown').length) {
				$('.multiselect-custom-dropdown-results').hide();
				$('.bottom-qtip-tip-up').css('z-index','100');
				/*if($('#change_instructor').val() != undefined && $('#change_instructor').val() != null && $('#change_instructor').val() == 2){
					$('#change_instructor').val(1);
					selectDropdownOnclick();
				} */
			}
		});
	}
	catch (e) {
		// TODO: handle exception
	}
});
function restrictMultiselectTitle(multiselectName, length) {
	/*$('div.ui-multiselect-menu.'+multiselectName).find('.ui-multiselect-checkboxes label.ui-corner-all span').each(function() {
		var labelText = titleRestricted($(this).text(), length);
		$(this).text(labelText);
	});*/
	$('div.ui-multiselect-menu.'+multiselectName).find('.ui-multiselect-checkboxes label.ui-corner-all span').each(function() {
		var labelText = titleRestrictionFadeoutImage($(this).text(), length);
		$(this).html(labelText);
	});
	$('.fade-out-title-container-unprocessed').each(function() {
		// console.log('unprocessed'+$(this).width());
		// console.log('span'+$(this).find('.title-lengthy-text').width());
		if($(this).width() >= $(this).find('.title-lengthy-text').width() && $(this).find('.title-lengthy-text').width() != 0) {
			$(this).find('.fade-out-image').remove();
		}
		$(this).removeClass('fade-out-title-container-unprocessed');
	});
	vtip();
}

//alert("drupal behaiour")


function checkInstructor(a){
	if(a == 'Y'){
		$('#two-col-row-webexname_webexpass').css('display','block');
	}else if(a == 'N'){
		$('#two-col-row-webexname_webexpass').css('display','none');
	}
	if($("#ui-multiselect-edit-roles-option-0").has("aria-selected")){
	if($("#ui-multiselect-edit-roles-option-0").attr("aria-selected") == "true"){
		$('#two-col-row-webexname_webexpass').css('display','block');
    }else if(($("#ui-multiselect-edit-roles-option-0").attr("aria-selected") == "true") && !(x)) {
    	$('#two-col-row-webexname_webexpass').css('display','block');
    }else if(($("#ui-multiselect-edit-roles-option-0").attr("aria-selected") == "false") && (($("#ui-multiselect-edit-roles-option-1").attr("aria-selected") != "true"))){
    	$('#two-col-row-webexname_webexpass').css('display','none');
  	} else if(($("#ui-multiselect-edit-roles-option-0").attr("aria-selected") == "false") && (($("#ui-multiselect-edit-roles-option-1").attr("aria-selected") != "false"))){
  		$('#two-col-row-webexname_webexpass').css('display','none');
  	}}else{
  		$('#two-col-row-webexname_webexpass').css('display','none');
  	}
}
