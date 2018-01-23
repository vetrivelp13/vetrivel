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
;

/* Note:  As of jQuery 1.7, the .live() method is deprecated.
 * Use .on() to attach event handlers in jquery 1.7+ versions.  
 */ 
$(document).ready(function(){
try{
	$('#access-control .movable').live('click', function(event){	
		try{
		if($(this).hasClass('selected-list')){
			$(this).removeClass('selected-list');
		} else {
			$(this).addClass('selected-list');
/*			$('#access-control .movable').mouseover(function(){
	    		$(this).addClass('selected-list');
	    	}); */
		}
		}catch(e){
			// to do
		}
	});
			
	$('body').click(function (event) {
		try{
		if(event.target.className == 'move-right' || event.target.className == 'move-left') {
		}
		else {
			$('#access-control .movable').each(function(){
					//$(this).removeClass('selected-list');
			});
		}		
		$('#access-select-list-class-dropdown-list').css("display","");
		if(event.target.className != 'access-select-list-class-dropdown-link'){
			 $('#access-select-list-class-dropdown-list').hide();
			
		}
		$('#ssel-unsel-usrlist-wrapper #select-list-dropdown-list').show();
		if(event.target.className != 'access-select-list-user-dropdown-link'){
			$('#sel-unsel-usrlist-wrapper #select-list-dropdown-list').hide();
		}
		}catch(e){
			// to do
		}
	}); 
	// Organization
	$('#access-control #organization .move-right').live('click', function(){
		try{
		$('#access-control #organization-available .selected-list').each(function(){
			storeOrgRolesValues('org', 'add', $(this).attr('id'));
			var orgName = $(this).children('span').attr('data');
			var orgNameRestricted = $(this).children('span').html();
			$('#organization-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+orgName+'" title="'+orgName+'">'+orgNameRestricted+'</span></li>');
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #organization .move-left').live('click', function(){
		try{
		$('#access-control #organization-selected .selected-list').each(function(){
			storeOrgRolesValues('org', 'remove', $(this).attr('id'));
			var orgName = $(this).children('span').attr('data');
			var orgNameRestricted = $(this).children('span').html();
			$('#organization-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+orgName+'" title="'+orgName+'">'+orgNameRestricted+'</span></li>');
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #organization-available .item-list li.movable').live('dblclick', function(){
		try{
		$('#vtip').remove();
		storeOrgRolesValues('org', 'add', $(this).attr('id'));
		var orgName = $(this).children('span').attr('data');
		var orgNameRestricted = $(this).children('span').html();
		$('#organization-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+orgName+'" title="'+orgName+'">'+orgNameRestricted+'</span></li>');
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});
	$('#access-control #organization-selected .item-list li.movable').live('dblclick',function(){	
		try{
		$('#vtip').remove();
		storeOrgRolesValues('org', 'remove', $(this).attr('id'));
		var orgName = $(this).children('span').attr('data');
		var orgNameRestricted = $(this).children('span').html();
		$('#organization-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+orgName+'" title="'+orgName+'">'+orgNameRestricted+'</span></li>');
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});

	// Job roles
	$('#access-control #jobroles .move-right').live('click', function(){
		try{
		$('#access-control #jobroles-available .selected-list').each(function(){
			var roleName,roleNameRestricted,mroRestricted;
			var entityType=$('#entity_type').val();
			var is_compliance = $('#is_compliance').val();
			if((entityType=='cre_sys_obt_cls' && is_compliance=='0') || entityType=='cre_sys_obt_trn' || entityType=='cre_sys_obt_crt' || entityType=='cre_sys_obt_cur'){
				var mro = $('#mro-controls').val();
				var default_mro = $('#default-mro-control').val();
				storeOrgRolesValues('jobroles', 'add', $(this).attr('id')+'-'+default_mro);
				roleName = $(this).children().html();
				roleNameRestricted = titleRestricted(roleName, '13');
				mroRestricted = titleRestricted(mro, '5');
				$('#jobroles-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="admin-access-org-name vtip" data="'+roleName+'" title="'+roleName+'">'+roleNameRestricted+'</span>'+mro+'</li>');
			}else{
				storeOrgRolesValues('jobroles', 'add', $(this).attr('id'));
				roleName = $(this).children().html();
				roleNameRestricted = titleRestricted(roleName, '40');
				$('#jobroles-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+roleName+'" title="'+roleName+'">'+roleNameRestricted+'</span></li>');
			}
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #jobroles .move-left').live('click', function(){
		try{
		$('#access-control #jobroles-selected .selected-list').each(function(){
			storeOrgRolesValues('jobroles', 'remove', $(this).attr('id'));
			$(this).find('.mro-controls').remove();
			var roleName = $(this).children('span').attr('data');
			$('#jobroles-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+roleName+'" title="'+roleName+'">'+roleName+'</span></li>');
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #jobroles-available .item-list li.movable').live('dblclick', function(){
		try{
		$('#vtip').remove();
		var roleName,roleNameRestricted,mroRestricted;
		var entityType=$('#entity_type').val();
		var is_compliance = $('#is_compliance').val();
		if((entityType=='cre_sys_obt_cls' && is_compliance=='0') || entityType=='cre_sys_obt_trn' || entityType=='cre_sys_obt_crt' || entityType=='cre_sys_obt_cur'){
			var mro = $('#mro-controls').val();
			var default_mro = $('#default-mro-control').val();
			storeOrgRolesValues('jobroles', 'add', $(this).attr('id')+'-'+default_mro);
			roleName = $(this).children().html();
			roleNameRestricted = titleRestricted(roleName, '13');
			mroRestricted = titleRestricted(mro, '5');
			$('#jobroles-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="admin-access-org-name vtip" data="'+roleName+'" title="'+roleName+'">'+roleNameRestricted+'</span>'+mro+'</li>');
		}else{
			storeOrgRolesValues('jobroles', 'add', $(this).attr('id'));
			roleName = $(this).children().html();
			roleNameRestricted = titleRestricted(roleName, '40');
			$('#jobroles-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+roleName+'" title="'+roleName+'">'+roleNameRestricted+'</span></li>');
		}
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});
	$('#access-control #jobroles-selected .item-list li.movable').live('dblclick', function(){	
		try{
		$('#vtip').remove();
		storeOrgRolesValues('jobroles', 'remove', $(this).attr('id'));
		$(this).find('.mro-controls').remove();
		var roleName = $(this).children('span').attr('data');
		$('#jobroles-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+roleName+'" title="'+roleName+'">'+roleName+'</span></li>');
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});
	$('#jobroles-selected .sub-menu li').live('click', function(){
		try{
		var selectedMRO = $(this).html();
		var id = $(this).closest('.movable').attr('id');
		var mro = $(this).attr('data');
		$(this).parent().prev('span').prev('span').html(selectedMRO);
		$(this).parent().css('display', 'none');
		storeOrgRolesValues('jobroles', 'modify', id, mro);
		return false;
		}catch(e){
			// to do
		}
	});
	$('#access-control .mro-selection').live('click', function(){
		try{
		var posX = $(this).position().left;
		var posY = $(this).position().top;
		if($(this).next().css('display') == 'block'){
			$(this).next().css('display', 'none');
		} else {
			$(this).next().css('display', 'block');
			if (navigator.userAgent.indexOf("Chrome")>0 || $.browser.msie && $.browser.version >= 8)
			{
			$(this).next().css('top', posX);
			$(this).next().css('top', posY+17);
			}
			
		}
		}catch(e){
			// to do
		}
	});
	
	// UserType
	$('#access-control #usertype .move-right').live('click', function(){
		try{
		$('#access-control #usertype-available .selected-list').each(function(){
			storeOrgRolesValues('usertype', 'add', $(this).attr('id'));
			var utName = $(this).children('span').attr('data');
			var utNameRestricted = $(this).children('span').html();
			$('#usertype-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+utName+'" title="'+utName+'">'+utNameRestricted+'</span></li>');
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #usertype .move-left').live('click', function(){
		try{
		$('#access-control #usertype-selected .selected-list').each(function(){
			storeOrgRolesValues('usertype', 'remove', $(this).attr('id'));
			var utName = $(this).children('span').attr('data');
			var utNameRestricted = $(this).children('span').html();
			$('#usertype-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+utName+'" title="'+utName+'">'+utNameRestricted+'</span></li>');
			$(this).remove();
			vtip();
		});
		}catch(e){
			// to do
		}
	});
	$('#access-control #usertype-available .item-list li.movable').live('dblclick', function(){
		try{
		$('#vtip').remove();
		storeOrgRolesValues('usertype', 'add', $(this).attr('id'));
		var utName = $(this).children('span').attr('data');
		var utNameRestricted = $(this).children('span').html();
		$('#usertype-selected .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+utName+'" title="'+utName+'">'+utNameRestricted+'</span></li>');
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});
	$('#access-control #usertype-selected .item-list li.movable').live('dblclick',function(){	
		try{
		$('#vtip').remove();
		storeOrgRolesValues('usertype', 'remove', $(this).attr('id'));
		var utName = $(this).children('span').attr('data');
		var utNameRestricted = $(this).children('span').html();
		$('#usertype-available .item-list').append('<li class="movable" id="'+$(this).attr('id')+'"><span class="vtip" data="'+utName+'" title="'+utName+'">'+utNameRestricted+'</span></li>');
		$(this).remove();
		vtip();
		}catch(e){
			// to do
		}
	});
}catch(e){
	// to do
}
});

function storeOrgRolesValues(fieldType, type, id, mro){
	try{
	var fieldName = '#'+fieldType+'_selected_id';
	var selectedList = $(fieldName).val();
	var selectedListArray = new Array();
	var selectedListLength = 0;
	if(type == 'add'){
		if(selectedList != ''){
			selectedListArray = selectedList.split(",");
			selectedListArray.push(id);
			selectedList = selectedListArray.join(",");
			$(fieldName).val(selectedList);
		} else {
			$(fieldName).val(id);
		}
	} else if(type == 'remove'){
		selectedListArray = selectedList.split(",");
		selectedListArray.splice( $.inArray(id, selectedListArray), 1 );
		selectedList = selectedListArray.join(",")
		$(fieldName).val(selectedList);
	} else {
		selectedListArray = selectedList.split(",");
		selectedListLength = selectedListArray.length;
		var mroArray = new Array();
		for(var c=0; c<selectedListLength; c++){
			mroArray = selectedListArray[c].split("-");
			if(mroArray[0] == id){
				selectedListArray[c] = id+"-"+mro;
			}
		}
		$(fieldName).val(selectedListArray.join(","));
	}
	}catch(e){
		// to do
	}
}

function getOrganizationList(org_name,isMore,fromPage){
	try{
	if(org_name.length>2 || org_name.length==0){
		var selectedIds = $('#org_selected_id').val();
		org_name = org_name.length==0?'-':org_name;
		selectedIds = selectedIds.length==0?'-':selectedIds;
		var pagenate = $('#org_pagenation').val().split('#');
		var total;
		var passPage;
		if(isMore=='more'){
			passPage = pagenate[1];
		}else{
			passPage = 0;
		}
		if(fromPage == 'discounts'){
 		  url = "/?q=administration/commerce/discounts/getorglist" + '/' + org_name + '/' + selectedIds + "/" + passPage;
		}
		else{
		  url = "/?q=administration/learning/access/getorglist" + '/' + org_name + '/' + selectedIds + "/" + passPage;
		}
		$('.admin-access-jobrole-field1 .admin-access-jobrole-field').addClass('ac_loading');
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				if(result!=undefined && result!=null && result!=''){
					if(isMore=='more'){
						$('#org_more').remove();
						$('#organization-available > ul').append(result);
						$('#org_pagenation').val(pagenate[0]+"#"+(Number(pagenate[1])+1));
						pagenate = $('#org_pagenation').val().split('#');
						total = 15*(Number(pagenate[1]));
					}else{
						$('#organization-available > ul').html(result);
						$('#org_pagenation').val(pagenate[0]+"#1");
						pagenate = $('#org_pagenation').val().split('#');
						total = 15*1;
					}
				}
					
				$('.admin-access-jobrole-field1 .admin-access-jobrole-field').removeClass('ac_loading');
				if(Number(pagenate[0])<=total){
					$('#org_more').remove();
				}
			}
		});
	}
	}catch(e){
		// to do
	}
}

function getJobRoles(role_name,isMore,fromPage){
	try{
	if(role_name.length>2 || role_name.length==0){
		var selectedIds = $('#jobroles_selected_id').val();
		role_name = role_name.length==0?'-':role_name;
		selectedIds = selectedIds.length==0?'-':selectedIds;
		var pagenate = $('#jobrole_pagenation').val().split('#');
		var total;
		var passPage;
		if(isMore=='more'){
			passPage = pagenate[1];
		}else{
			passPage = 0;
		}
		if(fromPage == 'discounts'){
			url = "/?q=administration/commerce/discounts/getjobroles" + '/' + role_name + '/' + selectedIds + "/" + passPage;
		}
		else{
			url = "/?q=administration/learning/access/getjobroles" + '/' + role_name + '/' + selectedIds + "/" + passPage;
		}
		$('.admin-access-jobrole-field2 .admin-access-jobrole-field').addClass('ac_loading'); 
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				if(result!=undefined && result!=null && result!=''){
					if(isMore=='more'){
						$('#jobrole_more').remove();
						$('#jobroles-available > ul').append(result);
						$('#jobrole_pagenation').val(pagenate[0]+"#"+(Number(pagenate[1])+1));
						pagenate = $('#jobrole_pagenation').val().split('#');
						total = 15*(Number(pagenate[1]));
					}else{
						$('#jobroles-available > ul').html(result);
						$('#jobrole_pagenation').val(pagenate[0]+"#1");
						pagenate = $('#jobrole_pagenation').val().split('#');
						total = 15*1;
					}
				}

				$('.admin-access-jobrole-field2 .admin-access-jobrole-field').removeClass('ac_loading');
				if(Number(pagenate[0])<=total){
					$('#jobrole_more').remove();
				}
			}
		});
	}
	}catch(e){
		// to do
	}
}

function getUserTypes(user_type_name,isMore,fromPage){
	try{
	if(user_type_name.length>2 || user_type_name.length==0){
		var selectedIds = $('#usertype_selected_id').val();
		user_type_name = user_type_name.length==0?'':user_type_name;
		selectedIds = selectedIds.length==0?'':selectedIds;
		var pagenate = $('#usertype_pagenation').val().split('#');
		var total;
		var passPage;
		if(isMore=='more'){
			passPage = pagenate[1];
		}else{
			passPage = 0;
		}
		if(fromPage == 'discounts'){
			url = "/?q=administration/commerce/discounts/getusertype" + '/' + user_type_name+ '/' + selectedIds + "/" + passPage;
		}
		else{
			url = "/?q=administration/learning/access/getusertype" + '/' + user_type_name+ '/' + selectedIds + "/" + passPage;
		}
		$('.admin-access-jobrole-field3 .admin-access-jobrole-field').addClass('ac_loading');
		$.ajax({
			type: "POST",
			url: url,
			data:  '',
			success: function(result){
				if(result!=undefined && result!=null && result!=''){
					if(isMore=='more'){
						$('#usertype_more').remove();
						$('#usertype-available > ul').append(result);
						$('#usertype_pagenation').val(pagenate[0]+"#"+(Number(pagenate[1])+1));
						pagenate = $('#usertype_pagenation').val().split('#');
						total = 15*(Number(pagenate[1]));
					}else{
						$('#usertype-available > ul').html(result);
						$('#usertype_pagenation').val(pagenate[0]+"#1");
						pagenate = $('#usertype_pagenation').val().split('#');
						total = 15*1;
					}
				}

				$('.admin-access-jobrole-field3 .admin-access-jobrole-field').removeClass('ac_loading');
				if(Number(pagenate[0])<=total){
					$('#usertype_more').remove();
				}
			}
		});
	}
	}catch(e){
		// to do
	}
}

function updateAccessOnKeyDown(evt,obj){
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 13) {           	    	
        evt.preventDefault();
        evt.stopPropagation();
        return false;           	    	
    }
	}catch(e){
		// to do
	}
}

function selectKeyDownAccess(id, evt, type){
	/* 	
		Down arrow - 40
		Up arrow - 38
	*/
	try{
	evt = evt || window.event;
    var charCode = evt.keyCode || evt.which;
    if (charCode == 40) { 
    	$('#'+type+' #'+id).next().focus();           	    	
    	$('#'+type+' #'+id).next().addClass('selected-list');
    }	
    if (charCode == 38) { 
    	$('#'+type+' #'+id).removeClass('selected-list');           	    	
    	$('#'+type+' #'+id).prev().focus();
    }	
    evt.preventDefault();
    evt.stopPropagation();
    return false;  
	}catch(e){
		// to do
	}
}
(function($) {
	$.widget("ui.accessgroup", {
	callMultiSelect: function(id,uniq,mro,entType,emptyId){
		var autoUrl = this.constructUrl("administration/catalogaccess/group/autocomplete&type="+entType+"&uniqueId="+uniq);
		var grpUrl;
		var obj = this;
		var autoHelpTxt = Drupal.t('LBL1270');
		if(typeof(entType) == 'undefined') entType = '';
		if(entType =='Discount'){
			grpurl = this.constructUrl("administration/catalogaccess/discount/group/"+uniq);
		}else{
			grpurl = this.constructUrl("administration/catalogaccess/group/"+uniq+"/"+emptyId+"&type="+entType);
		}
		var xWidth = 270;
		if(document.getElementById('sel-unsel-usrlist') != null)
			xWidth = 305;
		if(id == 'sel-unsel-usrlist'){
			grpurl = this.constructUrl("administration/catalogaccess/user/"+uniq+"/"+emptyId+"&type="+entType);
			autoUrl = this.constructUrl("administration/catalogaccess/user/autocomplete&type="+entType+"&uniqueId="+uniq);
			autoHelpTxt = Drupal.t('LBL036') + ' ' + Drupal.t('LBL054');
			$('#sel-unsel-grplist').hide();
			$('#sel-unsel-usrlist').show();
			
			if($('#user-list-control').length > 0){
				$('#group-list-control').find('span:last')
	    		.removeClass('down-tip-arrow')
	    		.addClass('right-tip-arrow');
	    	
		    	$('#user-list-control').find('span:last')
				.removeClass('right-tip-arrow')
				.addClass('down-tip-arrow');
			}
		}else{
			
			$('#sel-unsel-grplist').show();
			$('#sel-unsel-usrlist').hide();
			
			if($('#user-list-control').length > 0){
				$('#user-list-control').find('span:last')
	    		.removeClass('down-tip-arrow')
	    		.addClass('right-tip-arrow');
	    	
		    	$('#group-list-control').find('span:last')
				.removeClass('right-tip-arrow')
				.addClass('down-tip-arrow');
			}
		}
		if($('#'+id + ' #ui-multiselect-con').size()>0){
			return true;
		}
		var res = '';
		if($('#hidden_oplist').val() != null) {
			res = JSON.parse($('#hidden_oplist').val());
		}
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		//console.log($(this));
		if(this.currTheme == 'expertusoneV2'){
			$('#'+id).expMultiSelectDropdownExtend({
				//data :x,
				width: xWidth,
				height: 235,
				listmodel :{id:'opt',title:'val',dispTitle:'dispval',selected:'sel'},
				sublistmodel :{enable:'dropdown',defaultval:'mro'},
				optlistmodel :{id :'opt',title:'val'},
				optional: {optionlist:res,enable:mro},
				url: grpurl,
				searchfilter:{url:autoUrl,enable:true},
				helpText: {autocomplete:autoHelpTxt,checkAll:'Check All',uncheckAll:'Uncheck All'},
				dataType:"json",
				rownum:15,
				checkAll :false,
				uncheckall :false,
				titlelength : 30,
				widget: this,
				onselect:this.onselectfn,
				onunselect:this.onunselectfn,
				ondropdown:this.ondropdownselfn
				
			});
		}else{
			$('#'+id).expmultiselectDropdown({
				//data :x,
				width: xWidth,
				height: 235,
				listmodel :{id:'opt',title:'val',dispTitle:'dispval',selected:'sel'},
				sublistmodel :{enable:'dropdown',defaultval:'mro'},
				optlistmodel :{id :'opt',title:'val'},
				optional: {optionlist:res,enable:mro},
				url: grpurl,
				searchfilter:{url:autoUrl,enable:true},
				helpText: {autocomplete:autoHelpTxt,checkAll:'Check All',uncheckAll:'Uncheck All'},
				dataType:"json",
				rownum:15,
				checkAll :false,
				uncheckall :false,
				titlelength : 27,
				widget: this,
				onselect:this.onselectfn,
				onunselect:this.onunselectfn,
				ondropdown:this.ondropdownselfn
				
			});
			$('#chk ul li').each(function(){
				if($(this).hasClass('learner-group-container')) {
					$(this).css("display","table-row");
				}
			});
		}
		//if(id == 'sel-unsel-usrlist'){
			//$('#sel-unsel-usrlist #multiautocomplete').css('width','180px');
			setTimeout(function(){
				$('#'+id+ ' #chk ul li .access-fullname .fade-out-image-access').remove();
				//var lid = (id == 'sel-unsel-usrlist') ? 'sel-user-list' : 'sel-group-list';
				$('#sel-user-list .cls-access-list-select').each(function(){
					var id = $(this).find('span:first').attr('id');
					id = id.replace('group-names-','');
					obj.resetRighPane('sel-user-list',id);
				});
			},100)
		//}
		this.callScroll();
		vtip();
	},
	
	callScroll: function(){
		var aHight = $('#group-control').height()-60;
		var gHight = $('#sel-group-list').height();
		var uHight = $('#sel-user-list').height();
		var sHight = (gHight !=0 && uHight !=0) ? (gHight + uHight + 101) : (gHight + uHight+23);
		if(sHight>=aHight){
			if($('#group-scroll-wrapper').find('.jspContainer').length>0){
				$('#group-scroll-wrapper').jScrollPane('reinitialise');
			}else{
				$('#group-scroll-wrapper').jScrollPane({});
			}
		}else{
			if($('#group-scroll-wrapper').find('.jspContainer').length>0){
				$('#group-scroll-wrapper').jScrollPane('destroy');
				//$('#group-scroll-wrapper').html($('#group-scroll-wrapper .jspPane').html());
			}
		}
	},
	
	onselectfn: function(uniq,data,ob){
		var id = $(uniq).attr('id');
		var lid = (id == 'sel-unsel-usrlist') ? 'sel-user-list' : 'sel-group-list';
		var inid = (id == 'sel-unsel-usrlist') ? 'hidden_selection_usr' : 'hidden_selection'; 
		var inids = (id == 'sel-unsel-usrlist') ? 'hidden_idlist_usr' : 'hidden_idlist';
        var str=$('input[id="'+inid+'"]').attr('value');
    	var selected_list='';
        if(str.length==0)
        {
            if($('#'+data.value).attr('opt')==='enable')
            selected_list=data.value+'-cre_sys_inv_opt';
            else
            selected_list=data.value+'-';
        }else{
	        if($('#'+data.value).attr('opt')==='enable')
	            selected_list=','+data.value+'-cre_sys_inv_opt';
	        else
	            selected_list=','+data.value+'-';
        }
		var selListItem = $('#'+id).expmultiselectDropdown('getValue','selected');
		var selList = $('#'+lid).html();
		selList = titleRestrictionFadeoutImage("-"+" "+data.title, "addedit-cataog-access-group-name-fadeout-container");
		var mroList = 'mro-names-'+data.value;
		var grpname = 'group-names-'+data.value;
		if(document.getElementById(grpname)==null){
			$('#'+lid).append('<div class="cls-access-list-select"><span id="'+grpname+'" class="vtip" title ="'+data.title+'"></span><span id="'+mroList+'"></span></div>');
		}
		$('#group-names-'+data.value).html(selList);
		if($(".contentauthorclone").size() == 0)
			ob.sortList(lid);
		if(selListItem.length != '') {
			if(id == 'sel-unsel-usrlist')
				$('#help-msg-usr').css('display','block') +'<br>';
			if(id == 'sel-unsel-grplist')
				$('#help-msg-grp').css('display','block') +'<br>';
			$('#sel-group-text').css('display','none');
			$('#sel-message').css('display','none');
			$('#sel-msg').css('display','none');
		}
		if(id == 'sel-unsel-usrlist') {
			$('#'+id+ ' #chk ul li .access-username').remove();
			$('#'+id+ ' #chk ul li .access-fullname').css('min-width','130px');
		}
	  //Add selected list code in hidden variable
		var i ='';
		$.each(selListItem,function(){
			i += i==''?(this.value+"-"+this.selectedOption.optId) : ","+(this.value+"-"+this.selectedOption.optId);
			});
		$('input[id="'+inids+'"]').attr('value',i);
        str=str+selected_list;
        $('input[id="'+inid+'"]').attr('value',str);
		//$('#group-scroll-wrapper').jScrollPane({});
        
        ob.dispSeparator();
		ob.callScroll();
		vtip();
	},
	
	sortList: function(lid){
		var mylist = $('#'+lid); 
		var l = mylist.children('div').get();
		l.sort(function(a,b){
			return $(a).text().toUpperCase().localeCompare($(b).text().toUpperCase());
		});
		$.each(l, function(idx, itm) { $('#'+lid).append(itm); });
	},
	
	onunselectfn : function(uniq,data, ob){
		var id = $(uniq).attr('id');
		var lid = (id == 'sel-unsel-usrlist') ? 'sel-user-list' : 'sel-group-list';
		var inid = (id == 'sel-unsel-usrlist') ? 'hidden_selection_usr' : 'hidden_selection';
		var inids = (id == 'sel-unsel-usrlist') ? 'hidden_idlist_usr' : 'hidden_idlist';
		var str=$('input[id="'+inid+'"]').attr('value');
	    var unsel=data.value+'-';
	    var pattern1='(^|,)'+unsel+'(cre_sys_inv_rec|cre_sys_inv_man|cre_sys_inv_opt|)';
	    var regex1=new RegExp(pattern1);
	    str=str.replace(regex1,'');
	    if(str.charAt(0)==',')
	       str=str.substring(1,str.length);
		var selList =$('#'+lid).html();
		selList = selList.split("-");
		selList.splice(selList.indexOf(data.title),1);
		var regExp = new RegExp(",","g");
		selList = selList.toString().replace(regExp, '-');
		$('#group-names-'+data.value).html('');
		$('#mro-names-'+data.value).html('');
		$('#group-names-'+data.value).parent().remove();
		$('input[id="'+inid+'"]').attr('value','');
		var selListItem = $('#'+id).expmultiselectDropdown('getValue','selected');
		var i ='';
		$.each(selListItem,function(){
		      i += i==''?(this.value+"-"+this.selectedOption.optId) : ","+(this.value+"-"+this.selectedOption.optId);
			});
		 $('input[id="'+inids+'"]').attr('value',i);
		 str = (str == undefined || str == 'undefined') ? '' : str;
         $('input[id="'+inid+'"]').attr('value',str);
         ob.dispSeparator(id);
		 ob.callScroll();
	},
	
	dispSeparator: function(id){
		/*var groupSelect = $('#sel-unsel-grplist').data('expmultiselectDropdown') != undefined ? $('#sel-unsel-grplist').expmultiselectDropdown('getValue','selected') : [];
		var userSelect = $('#sel-unsel-usrlist').data('expmultiselectDropdown') != undefined ?$('#sel-unsel-usrlist').expmultiselectDropdown('getValue','selected') : [];*/
		var groupSelect = $('#sel-group-list').children();
		var userSelect = $('#sel-user-list').children();
		if(groupSelect.length > 0 && userSelect.length > 0){
			$('#right-separator').css('display','block');
		}else{
			$('#right-separator').css('display','none');
		}
		if(groupSelect.length == 0 && userSelect.length == 0){
			$('#sel-group-text').css('display','inline');
			$('#sel-message').css('display','inline');
			$('#sel-msg').css('display','inline');
		}else{
			$('#sel-group-text').css('display','none');
			$('#sel-message').css('display','none');
			$('#sel-msg').css('display','none');
		}
		if(groupSelect.length == 0)
			$('#help-msg-grp').css('display','none');
		if(userSelect.length == 0)
			$('#help-msg-usr').css('display','none');
	},
	
	ondropdownselfn : function(uniq,data,ob){
		var id = $(uniq).attr('id');
		var lid = (id == 'sel-unsel-usrlist') ? 'sel-user-list' : 'sel-group-list';
		var inid = (id == 'sel-unsel-usrlist') ? 'hidden_selection_usr' : 'hidden_selection';
		var inids = (id == 'sel-unsel-usrlist') ? 'hidden_idlist_usr' : 'hidden_idlist';
        var str=$('input[id="'+inid+'"]').attr('value');
        var optChanged=','+data.value+'-'+data.selectedOption.optId;
        var unsel=data.value+'-';
        var pattern1='(^|,)'+unsel+'(cre_sys_inv_rec|cre_sys_inv_man|cre_sys_inv_opt|)';
        var regex1=new RegExp(pattern1);
        str=str.replace(regex1,optChanged);
        if(str.charAt(0)==',')
           str=str.substring(1,str.length);
		var selList = $('#'+lid).html();
		selList = " "+"<span>("+data.selectedOption.optval +")</span>";		
		var selectedValue = data.selectedOption.optId;

		//$('#group-names-'+data.value).append("<span id='"+mroList+"'></span><br>");
		$('#mro-names-'+data.value).html('');
		$('#mro-names-'+data.value).html(selList);
		if(selectedValue == 'cre_sys_inv_opt'){
			$('#mro-names-'+data.value).html("");
		}
		else{
			$('#mro-names-'+data.value).html(selList);
		}
		
		ob.resetRighPane(lid,data.value);
		
		var hid = $('input[id="'+inids+'"]').val();
        hid = hid+'-'+data.selectedOption.optId;
        
        var selListItem = $('#'+id).expmultiselectDropdown('getValue');
   
        var i = '';
		$.each(selListItem,function(){
			i += i==''?(this.value+"-"+this.selectedOption.optId) : ","+(this.value+"-"+this.selectedOption.optId);
		});
		$('input[id="'+inids+'"]').attr('value',i);
        $('input[id="'+inid+'"]').attr('value',str);

	},
	
	resetRighPane:function(lid,value){
		var Width = $('#'+lid).width();
		var mWidth = $('#mro-names-'+value+' span').width()+2;
		var tWidth = $('#group-names-'+value+' .fade-out-title-container .title-lengthy-text').width();
		var aWidth = Width - mWidth;
		var fWidth = (tWidth > aWidth) ? (aWidth - 5) : tWidth;

		$('#group-names-'+value).css('float','left');
		$('#group-names-'+value+' .fade-out-title-container.addedit-cataog-access-group-name-fadeout-container').css('max-width',fWidth+'px');
		$('#group-names-'+value+' .fade-out-title-container.addedit-cataog-access-group-name-fadeout-container span .fade-out-image').remove();
		if(tWidth > aWidth){
			$('#group-names-'+value+' .fade-out-title-container.addedit-cataog-access-group-name-fadeout-container span').append('<span class="fade-out-image"></span>');
			$('#mro-names-'+value).css('width',mWidth+'px').css('padding-left','0px');
		}else{
			$('#mro-names-'+value).css('width',mWidth+'px').css('padding-left','10px');
		}
		$('#mro-names-'+value).css('float','left');
	},
	moreAccessSearchTypeText : function(dCode,dText) {
	    $('#access-select-list-class-dropdown-list').hide();
	    $('#access-select-list-class-dropdown').text(dCode);
	    $('#access-search_all_classs_type-hidden').val(dText);
	    var displayText;
	    if(dText=='clstit'){
	 	   displayText = Drupal.t('LBL766');     
	    }else if (dText=='clsstatus') {
	 	   displayText =Drupal.t('LBL036') + ' ' + Drupal.t('Class')+ ' ' + dCode;
	    }else{
	 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
	    }
	    
		 var searchType = $('#access-select-list-class-dropdown').html();
		 var x = $('#sel-unsel-grplist').data('expmultiselectDropdown');
		 if(x.options.searchfilter.url.indexOf("Admin")>=0){
		      x.options.searchfilter.url = x.options.searchfilter.url.replace('Admin',searchType);
		      x.options.url = x.options.url.replace('Admin',searchType);
		 }
		 else if(x.options.searchfilter.url.indexOf("Learner")>=0){
		      x.options.searchfilter.url = x.options.searchfilter.url.replace('Learner',searchType);
		      x.options.url =  x.options.url.replace('Learner',searchType);
		 }
		 else if(x.options.searchfilter.url.indexOf("Any")>=0){
			 x.options.searchfilter.url = x.options.searchfilter.url.replace('Any',searchType);
			 x.options.url = x.options.url.replace('Any',searchType);
		 }
		 $('#sel-unsel-grplist').expmultiselectDropdown('autocompleteInit');
	},
	moreUserAccessSearchTypeText : function(dCode,dText) {
	    $('#select-list-dropdown-list').hide();
	    var privVal = $('#search_all_enroll_type-hidden').val();
	    $('#select-list-dropdown').text(dCode);
	    $('#search_all_enroll_type-hidden').val(dText);
	    var displayText = Drupal.t('LBL036') + ' ' + dCode;
	    $('#sel-unsel-usrlist #multiautocomplete').val(displayText);
		 //var searchType = $('#select-list-dropdown').html();
		 var x = $('#sel-unsel-usrlist').data('expmultiselectDropdown');
		 if(x.options.searchfilter.url.indexOf(privVal)>=0){
		      x.options.searchfilter.url = x.options.searchfilter.url.replace(privVal,dText);
		      x.options.url = x.options.url.replace(privVal,dText);
		      x.options.helpText.autocomplete = displayText;
		 }
		 
		 $('#sel-unsel-usrlist').expmultiselectDropdown('autocompleteInit');
	},
	 moreAccessSearchHideShow : function (id) {			
			 $('#access-select-list-class-dropdown-list').slideToggle();
			 $('#access-select-list-class-dropdown-list li:last').css('border-bottom','0px none');		
     },
     moreUserAccessSearchHideShow : function (id) {			
		 $('#select-list-dropdown-list').slideToggle();
		 $('#select-list-dropdown-list li:last').css('border-bottom','0px none');		
     },
	accessClose: function(){
		$('#group-control').remove();
	}
});
	$.extend($.ui.accessgroup.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery);

$(function() {
	try{
		$("#root-admin").accessgroup();	
	}catch(e){
			// To Do
	}
});
(function($) {
	$.widget("custom.multiselectclone",  {
		 // default options
		options: {
		checkAll: false,
		uncheckAll: false,
		helpText: {autocomplete: 'Search', checkAll:'Check All',uncheckAll:'Uncheck All'},
		targetClass: '',
		onunselectAll: '',
		onselectAll: '',
		theme: ''
		},
		_create: function(){
			  var options = this.options,
			  	obj = this,
				html ='',
				chkAll = '',
				chk= '<ul></ul>',
				unchk = '<ul></ul>',
				unchkAll = '',
				cHight=options.height - 30;
		 	 this.elem = this.element;
		 	 this.elemid = $(this.elem).attr('id');
		 	 
		 	 if($(".contentauthorclone").size() > 0)
		 	 {
		 	 		html +='<div id ="ui-multiselect-con"><div id = "container"><div id ="chk"></div></div></div>';
		 	 		
		 	 		$(obj.element).append(html);
		    		$(obj.element).find('#ui-multiselect-con').addClass(options.classes).css({'width':options.width,'height':options.height});
		    		$(obj.element).find('#container').css({'height':cHight});
		    		$(obj.element).find('.label-text').each(function(){
		 				obj._labelBind(this);
		 			});
	    			obj._dataLoad();
	    			//obj._binding();
		 	 }
		 	 else
		 	 {
		 	 
		 	 if(options.checkAll == true) {
		 		 if(options.theme == "expertusoneV2") {
		 			chkAll +='<div><ul><li style="display:none;" class="select-all-wrapper"><label class="clone-label-list" for="checkall" title="'+options.helpText.checkAll+'">'+options.helpText.checkAll+'</label><div class="form-item form-type-checkbox form-item-checkbox-attributes-access"><div class="checkbox-unselected"><input class="form-checkbox" type="checkbox" name="checkall" id="selectcheckall"><span class="field-suffix"></span></div></div></li></ul></div>';
		 		 }
		 		 else {
		 			 chkAll +='<div><ul><li style="display:none;" class="select-all-wrapper"><input class="form-checkbox" type="checkbox" name="checkall" id="selectcheckall"><label class="clone-label-list" for="checkall">'+options.helpText.checkAll+'</label></li></ul></div>';
		 		 }
		 	 }
		 	 
			 if(options.uncheckAll == true) {
				 if(options.theme == "expertusoneV2") {
					 unchkAll +='<div><ul><li style="display:none;" class="select-all-wrapper"><label class="clone-label-list" for="uncheckall" title="'+options.helpText.uncheckAll+'">'+options.helpText.uncheckAll+'</label><div class="form-item form-type-checkbox form-item-checkbox-attributes-access"><div class="checkbox-unselected"><input class="form-checkbox" type="checkbox" name="uncheckall" id="selectuncheckall"><span class="field-suffix"></span></div></div></li></ul></div>';
			 	}
				 else {
					 unchkAll +='<div><ul><li style="display:none;" class="select-all-wrapper"><input class="form-checkbox" type="checkbox" name="uncheckall" id="selectuncheckall"><label class="clone-label-list" for="uncheckall">'+options.helpText.uncheckAll+'</label></li></ul></div>';
				 }

			 }
		 	html +='<div id ="ui-multiselect-con"><div id = "container">'+unchkAll+'<div id ="chk">'+chk+'</div><div class="line-separator" style="display:none;"/>'+chkAll+'<div id ="unchk">'+unchk+'</div></div></div>';
		    $(obj.element).append(html);
		    $(obj.element).find('#ui-multiselect-con')
		    	.addClass(options.classes)
				.css({'width':options.width,'height':options.height});
		    $(obj.element).find('#container')
				.css({'height':cHight});
		    $(obj.element).find('.label-text').each(function(){
		 		obj._labelBind(this);
		 	});
	    	obj._dataLoad();
	    	obj._binding();
	    	}
	    	if($(".contentauthorclone").size() == 0)
	    		obj._sortlist();
	    	$(obj.element).jScrollPane();
		},
		_dataLoad : function(){
			var obj = this;
			
			if($(".contentauthorclone").size() > 0)
			{
				$.each($(obj.element).find('.'+obj.options.targetClass), function() {
					var checkbox = $(this).find('input').eq(0);
					$(obj.element).find('#chk ul').append($(this));
				});
			}
			else
			{
			
			//load checked and unchecked checkboxes to the respective divs
			$.each($(obj.element).find('.'+obj.options.targetClass), function() {
				var checkbox = $(this).find('input').eq(0);
				if(checkbox.attr('checked')) {
					$(obj.element).find('#chk ul').append($(this));
				}
				else {
					$(obj.element).find('#unchk ul').append($(this));
				}
				obj._clickfuntion($(obj.element).find('input[id="'+$(this).find('input').attr('id')+'"]'));
			});
		     // Show/Hide "Select All" Option
		    $(obj.element).find('#selectcheckall').parents('.select-all-wrapper')
		     	[ (this.options.checkAll == true && $('#'+this.elemid+' #unchk ul li').filter(function() {
			 	    return $(this).css('display') !== 'none';
			 	}).length > 1) ? 'show' : 'hide' ]();
		     
		    // Show/Hide "Unselect All" Option with checked attribute
		 	if(this.options.uncheckAll == true && $('#'+this.elemid+' #chk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length > 1) {
		 		$(obj.element).find('#selectuncheckall').attr('checked','checked');
		 		
		 		$(obj.element).find('#selectuncheckall').parents('.select-all-wrapper').show();
		 	}else{
		 		$(obj.element).find('#selectuncheckall').removeAttr('checked');
		 		$(obj.element).find('#selectuncheckall').parents('.select-all-wrapper').hide();
		 	}
		 	if(obj.options.theme == "expertusoneV2") {
				checkboxSelectedUnselectedCommon($(obj.element).find('#selectuncheckall'));
			}
			
			if($(obj.element).find('#unchk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length <= 0 || $(obj.element).find('#chk ul li').filter(function() {
		 	    return $(this).css('display') !== 'none';
		 	}).length <= 0)
				 $(obj.element).find('.line-separator').hide();
		    else
		    	 $(obj.element).find('.line-separator').show();
		    }
		},
		_moveItems : function(ele){
			var id = ele.attr('id'),
				obj = this;
			var checked = $(obj.elem).find('#chk ul');
			var unchecked = $(obj.elem).find('#unchk ul');
			
			$.each(ele, function() {
				if($(this).attr('checked')) {
					$(this).closest("."+obj.options.targetClass).appendTo(checked);
				}
				else {
					$(this).closest("."+obj.options.targetClass).appendTo(unchecked);
				}
			});
		
			checked.find('input[id="'+id+'"]').attr('checked','checked');
			
			// Show "Unselect All" option if there are 2 or more list items are available
			var lst = checked.find('li').length;
			if(this.options.uncheckAll == true){
				if(lst > 1){
					$(obj.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parents('.select-all-wrapper')
						.css('display', 'block');
				}else {
					$(obj.elem).find('input[id="selectuncheckall"]')
						.attr('checked','checked')
						.parents('.select-all-wrapper')
						.css('display', 'none');
				}
				if(obj.options.theme == "expertusoneV2") {
					checkboxSelectedUnselectedCommon($(obj.element).find('#selectuncheckall'));
				}
			}
			
			// Hide "Select All" option if there are less than 2 list items are available
			if($(unchecked).find('li').length <= 1) {
				$(this.elem).find('input[id="selectcheckall"]')
				.removeAttr('checked')
				.parents('.select-all-wrapper')
				.css('display', 'none');
			} else {
				$(this.elem).find('input[id="selectcheckall"]')
				.removeAttr('checked')
				.parents('.select-all-wrapper')
				.css('display', 'block');
			}
			
			// Show or hide the separator line 
			if($('#'+this.elemid+' #unchk ul li').filter(function() {
				    return $(this).css('display') !== 'none';
				}).length <= 0 || $('#'+this.elemid+' #chk ul li').filter(function() {
				    return $(this).css('display') !== 'none';
				}).length <= 0)
				 $(this.elem).find('.line-separator').hide();
			 else
				 $(this.elem).find('.line-separator').show();
			if($(".contentauthorclone").size() == 0)
				obj._sortlist();
			$(obj.element).data('jsp').reinitialise();
		},
		_clickfuntion : function(ele){
			var obj = this;
			$(ele).bind('click',function(){
				obj._moveItems($(ele));
			});	
		},
		_binding : function() {
			var obj = this;
			if(obj.options.checkAll == true){
				$(obj.element).find('#selectcheckall').unbind('click')
					.click(function () {
						obj._selectAll($(this));
						if(obj.options.theme =="expertusoneV2") {
							checkboxSelectedUnselectedCommon(this);
						}
					});
			}
			if(obj.options.uncheckAll == true){
				$(obj.element).find('#selectuncheckall').unbind('click')
					.click(function () {
						obj._selectAll($(this));
						if(obj.options.theme =="expertusoneV2") {
							checkboxSelectedUnselectedCommon(this);
						}
					});
			}
		},
		_selectAll: function(ele) {
			var obj = this;
			/*	clicked target = select All*/
			var target;
			if(ele.attr('id') == 'selectcheckall') {
				target = $(obj.element).find('#unchk ul input');
				target.each(function(){
					if(!$(this).attr('disabled')) {
						$(this).attr('checked', 'checked');
						if(obj.options.theme == "expertusoneV2") {
							checkboxSelectedUnselectedCommon(this);
						}
					}
				});
				this._moveItems(target);
			}
			else {
				target = $(obj.element).find('#chk ul input');
				target.each(function(){
					$(this).removeAttr('checked');
					if(obj.options.theme == "expertusoneV2") {
						checkboxSelectedUnselectedCommon(this);
					}
				});
				this._moveItems(target);
			}
			
		},
		_sortlist :function(){
			var mylist = $(this.elem).find('#chk ul'); 
			var srt = this.options.sort;
			var listitems = mylist.children('li').get();
			listitems.sort(function(a, b) {
				if(srt == 'desc')
					return $(b).text().toUpperCase().localeCompare($(a).text().toUpperCase());
				else
					return $(a).text().toUpperCase().localeCompare($(b).text().toUpperCase());
			});
			$.each(listitems, function(idx, itm) { mylist.append(itm); });

		}
	});
})(jQuery);

(function($) {
	/**	this function will be called on Drupal ajax completion*/
	$.fn.enableMultiCheckClone = function(classId) {
	
	if($(".contentauthorclone").size() > 0)
	{
		$('#clonescrolldiv'+classId).multiselectclone({
			checkAll: false, 
			uncheckAll: false,   
			targetClass: 'clone-list-class',
			helpText: {autocomplete: 'Search', checkAll: Drupal.t('Check')+' '+Drupal.t('LBL1039'), uncheckAll: Drupal.t('LBL1272')+' '+Drupal.t('LBL1039')},
			theme: Drupal.settings.ajaxPageState.theme
		});
	}
	else
	{
		$('#clonescrolldiv'+classId).multiselectclone({
			checkAll: true, 
			uncheckAll: true,   
			targetClass: 'clone-list-class',
			helpText: {autocomplete: 'Search', checkAll: Drupal.t('Check')+' '+Drupal.t('LBL1039'), uncheckAll: Drupal.t('LBL1272')+' '+Drupal.t('LBL1039')},
			theme: Drupal.settings.ajaxPageState.theme
		});
	} 
	};
})(jQuery);
;
/*! Copyright (c) 2013 Brandon Aaron (http://brandon.aaron.sh)
 * Licensed under the MIT License (LICENSE.txt).
 *
 * Version: 3.1.9
 *
 * Requires: jQuery 1.2.2+
 */

(function (factory) {
	if ( typeof define === 'function' && define.amd ) {
        // AMD. Register as an anonymous module.
        define(['jquery'], factory);
    } else if (typeof exports === 'object') {
        // Node/CommonJS style for Browserify
        module.exports = factory;
    } else {
        // Browser globals
        factory(jQuery);
    }
}(function ($) {

    var toFix  = ['wheel', 'mousewheel', 'DOMMouseScroll', 'MozMousePixelScroll'],
        toBind = ( 'onwheel' in document || document.documentMode >= 9 ) ?
                    ['wheel'] : ['mousewheel', 'DomMouseScroll', 'MozMousePixelScroll'],
        slice  = Array.prototype.slice,
        nullLowestDeltaTimeout, lowestDelta;

    if ( $.event.fixHooks ) {
        for ( var i = toFix.length; i; ) {
            $.event.fixHooks[ toFix[--i] ] = $.event.mouseHooks;
        }
    }

    var special = $.event.special.mousewheel = {
        version: '3.1.9',

        setup: function() {
            if ( this.addEventListener ) {
                for ( var i = toBind.length; i; ) {
                    this.addEventListener( toBind[--i], handler, false );
                }
            } else {
                this.onmousewheel = handler;
            }
            // Store the line height and page height for this particular element
            $.data(this, 'mousewheel-line-height', special.getLineHeight(this));
            $.data(this, 'mousewheel-page-height', special.getPageHeight(this));
        },

        teardown: function() {
            if ( this.removeEventListener ) {
                for ( var i = toBind.length; i; ) {
                    this.removeEventListener( toBind[--i], handler, false );
                }
            } else {
                this.onmousewheel = null;
            }
        },

        getLineHeight: function(elem) {
            return parseInt($(elem)['offsetParent' in $.fn ? 'offsetParent' : 'parent']().css('fontSize'), 10);
        },

        getPageHeight: function(elem) {
            return $(elem).height();
        },

        settings: {
            adjustOldDeltas: true
        }
    };

    $.fn.extend({
        mousewheel: function(fn) {
            return fn ? this.bind('mousewheel', fn) : this.trigger('mousewheel');
        },

        unmousewheel: function(fn) {
            return this.unbind('mousewheel', fn);
        }
    });


    function handler(event) {
        var orgEvent   = event || window.event,
            args       = slice.call(arguments, 1),
            delta      = 0,
            deltaX     = 0,
            deltaY     = 0,
            absDelta   = 0;
        event = $.event.fix(orgEvent);
        event.type = 'mousewheel';

        // Old school scrollwheel delta
        if ( 'detail'      in orgEvent ) { deltaY = orgEvent.detail * -1;      }
        if ( 'wheelDelta'  in orgEvent ) { deltaY = orgEvent.wheelDelta;       }
        if ( 'wheelDeltaY' in orgEvent ) { deltaY = orgEvent.wheelDeltaY;      }
        if ( 'wheelDeltaX' in orgEvent ) { deltaX = orgEvent.wheelDeltaX * -1; }

        // Firefox < 17 horizontal scrolling related to DOMMouseScroll event
        if ( 'axis' in orgEvent && orgEvent.axis === orgEvent.HORIZONTAL_AXIS ) {
            deltaX = deltaY * -1;
            deltaY = 0;
        }

        // Set delta to be deltaY or deltaX if deltaY is 0 for backwards compatabilitiy
        delta = deltaY === 0 ? deltaX : deltaY;

        // New school wheel delta (wheel event)
        if ( 'deltaY' in orgEvent ) {
            deltaY = orgEvent.deltaY * -1;
            delta  = deltaY;
        }
        if ( 'deltaX' in orgEvent ) {
            deltaX = orgEvent.deltaX;
            if ( deltaY === 0 ) { delta  = deltaX * -1; }
        }

        // No change actually happened, no reason to go any further
        if ( deltaY === 0 && deltaX === 0 ) { return; }

        // Need to convert lines and pages to pixels if we aren't already in pixels
        // There are three delta modes:
        //   * deltaMode 0 is by pixels, nothing to do
        //   * deltaMode 1 is by lines
        //   * deltaMode 2 is by pages
        if ( orgEvent.deltaMode === 1 ) {
            var lineHeight = $.data(this, 'mousewheel-line-height');
            delta  *= lineHeight;
            deltaY *= lineHeight;
            deltaX *= lineHeight;
        } else if ( orgEvent.deltaMode === 2 ) {
            var pageHeight = $.data(this, 'mousewheel-page-height');
            delta  *= pageHeight;
            deltaY *= pageHeight;
            deltaX *= pageHeight;
        }

        // Store lowest absolute delta to normalize the delta values
        absDelta = Math.max( Math.abs(deltaY), Math.abs(deltaX) );

        if ( !lowestDelta || absDelta < lowestDelta ) {
            lowestDelta = absDelta;

            // Adjust older deltas if necessary
            if ( shouldAdjustOldDeltas(orgEvent, absDelta) ) {
                lowestDelta /= 40;
            }
        }

        // Adjust older deltas if necessary
        if ( shouldAdjustOldDeltas(orgEvent, absDelta) ) {
            // Divide all the things by 40!
            delta  /= 40;
            deltaX /= 40;
            deltaY /= 40;
        }

        // Get a whole, normalized value for the deltas
        delta  = Math[ delta  >= 1 ? 'floor' : 'ceil' ](delta  / lowestDelta);
        deltaX = Math[ deltaX >= 1 ? 'floor' : 'ceil' ](deltaX / lowestDelta);
        deltaY = Math[ deltaY >= 1 ? 'floor' : 'ceil' ](deltaY / lowestDelta);

        // Add information to the event object
        event.deltaX = deltaX;
        event.deltaY = deltaY;
        event.deltaFactor = lowestDelta;
        // Go ahead and set deltaMode to 0 since we converted to pixels
        // Although this is a little odd since we overwrite the deltaX/Y
        // properties with normalized deltas.
        event.deltaMode = 0;

        // Add event and delta to the front of the arguments
        args.unshift(event, delta, deltaX, deltaY);

        // Clearout lowestDelta after sometime to better
        // handle multiple device types that give different
        // a different lowestDelta
        // Ex: trackpad = 3 and mouse wheel = 120
        if (nullLowestDeltaTimeout) { clearTimeout(nullLowestDeltaTimeout); }
        nullLowestDeltaTimeout = setTimeout(nullLowestDelta, 200);

        return ($.event.dispatch || $.event.handle).apply(this, args);
    }

    function nullLowestDelta() {
        lowestDelta = null;
    }

    function shouldAdjustOldDeltas(orgEvent, absDelta) {
        // If this is an older event and the delta is divisable by 120,
        // then we are assuming that the browser is treating this as an
        // older mouse wheel event and that we should divide the deltas
        // by 40 to try and get a more usable deltaFactor.
        // Side note, this actually impacts the reported scroll distance
        // in older browsers and can cause scrolling to be slower than native.
        // Turn this off by setting $.event.special.mousewheel.settings.adjustOldDeltas to false.
        return special.settings.adjustOldDeltas && orgEvent.type === 'mousewheel' && absDelta % 120 === 0;
    }

}));
;
