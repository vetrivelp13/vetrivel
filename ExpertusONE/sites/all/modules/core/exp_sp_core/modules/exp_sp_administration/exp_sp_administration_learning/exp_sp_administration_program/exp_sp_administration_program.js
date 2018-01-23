var tpTabDoubleClick = false;
var tpTabDoubleClickData = null;
( function($) {
  Drupal.behaviors.trainingPlanAdminFieldsBehavior = {
		  attach: function(content, settings) {
		try{
        $('.addedit-edit-expires-in-value:not(.addaddedit-edit-expires-in-value-processed)').addClass('addaddedit-edit-expires-in-value-processed').each(function () {
          var defaultText = $(this).data('default-text');
          if ($(this).val() == null || $(this).val() == undefined ||
                 $(this).val() == '' || $(this).val() == defaultText) {
              $(this).val(defaultText);
              $(this).removeClass('narrow-search-filterset-daterange-nonempty');
              $(this).addClass('narrow-search-filterset-daterange-empty');
          }
          
		      $(this).focus(function(event) {
			      if($(this).val() == defaultText) {
			        $(this).val('');
			        $(this).removeClass('narrow-search-filterset-daterange-empty');
			        $(this).addClass('narrow-search-filterset-daterange-nonempty');
			      }
			    });
		      
		      $(this).blur(function(event) {
            if($(this).val() == defaultText || $(this).val() == ''){
              $(this).val(defaultText);
              $(this).removeClass('narrow-search-filterset-daterange-nonempty');
              $(this).addClass('narrow-search-filterset-daterange-empty');
            }
          });
		      
		      $(this).change(function(event) { // Not sure whether change is doing anything.
            if($(this).val() == '' || $(this).val() == defaultText){
              $(this).val(defaultText);
              $(this).removeClass('narrow-search-filterset-daterange-nonempty');
              $(this).addClass('narrow-search-filterset-daterange-empty');
            } else {
              $(this).removeClass('narrow-search-filterset-daterange-empty');
              $(this).addClass('narrow-search-filterset-daterange-nonempty');              
            }
          });
		    });
        
        $('.addedit-edit-tpattachinggroupname:not(.addedit-edit-tpattachinggroupname-processed)').addClass('addedit-edit-tpattachinggroupname-processed').each(function () {
            var defaultGroupText = 'Enter Group Name';
            if ($('.addedit-edit-tpattachinggroupname').val() == null || $('#edit-tpattachinggroupname').val() == undefined ||
                   $('#edit-tpattachinggroupname').val() == '' || $('#edit-tpattachinggroupname').val() == defaultGroupText) {
                $('#edit-tpattachinggroupname').val(defaultGroupText);
                $('#edit-tpattachinggroupname').removeClass('narrow-search-filterset-daterange-nonempty');
                $('#edit-tpattachinggroupname').addClass('narrow-search-filterset-daterange-empty');
            }
            
  		      $('.addedit-edit-tpattachinggroupname').focus(function(event) {
  			      if($('.addedit-edit-tpattachinggroupname').val() == defaultGroupText) {
  			        $('.addedit-edit-tpattachinggroupname').val('');
  			        $('.addedit-edit-tpattachinggroupname').removeClass('narrow-search-filterset-daterange-empty');
  			        $('.addedit-edit-tpattachinggroupname').addClass('narrow-search-filterset-daterange-nonempty');
  			      }
  			    });
  		      
  		      $('.addedit-edit-tpattachinggroupname').blur(function(event) {
              if($('.addedit-edit-tpattachinggroupname').val() == defaultGroupText || $(this).val() == ''){
                $('.addedit-edit-tpattachinggroupname').val(defaultGroupText);
                $('.addedit-edit-tpattachinggroupname').removeClass('narrow-search-filterset-daterange-nonempty');
                $('.addedit-edit-tpattachinggroupname').addClass('narrow-search-filterset-daterange-empty');
              }
            });
  		      
  		      $('.addedit-edit-tpattachinggroupname').change(function(event) { // Not sure whether change is doing anything.
              if($('.addedit-edit-tpattachinggroupname').val() == '' || $('.addedit-edit-tpattachinggroupname').val() == defaultGroupText){
                $('.addedit-edit-tpattachinggroupname').val(defaultGroupText);
                $('.addedit-edit-tpattachinggroupname').removeClass('narrow-search-filterset-daterange-nonempty');
                $('.addedit-edit-tpattachinggroupname').addClass('narrow-search-filterset-daterange-empty');
              } else {
                $('.addedit-edit-tpattachinggroupname').removeClass('narrow-search-filterset-daterange-empty');
                $('.addedit-edit-tpattachinggroupname').addClass('narrow-search-filterset-daterange-nonempty');              
              }
            });
  		    });
        	add_module_toggle();
        	
		}catch(e){
			// to do
		}
      } // end attach
  };
  
})(jQuery);


function add_module_toggle(){
	// hide/show add module 
	var isCls = $('#page-container-tabs-prg .module-course-lists:last .nonlist').size();
	if(isCls == 0){
		$('#add-module-list').css('display','block');
	}else{
		$('#add-module-list').css('display','none');
	}
	
	//Update tab counts
	$('#tab_count').val($('#program_attach_tabs .admin-module-navigation .tp-module-tab').size());
}

function editGroupDetails(uniqueId){
	try{
	var groupVisibility = $('#program-group-course-addedit-form-'+uniqueId).is(':visible');
	$('#showform').css('display', 'none');
	$('#view-tp-list-wrapper .addedit-form-wrapper').hide();
	$('#program-trainingplan-addedit-form-details').find('.edit-class-list').css({'background' : '#ffffff'});
	$('.edit-class-list').find('.admin-edit-button').css('display','block');
	if(groupVisibility == true){
		$('#program-group-course-addedit-form-'+uniqueId).hide();
	} else {
		$('#program-group-course-addedit-form-'+uniqueId).show();
		$('#delete_all_'+uniqueId).find('.edit-class-list').css({'background-color' : '#D7ECF9'});
		$('#delete_all_'+uniqueId).find('.admin-edit-button').css('display','none');
	}
	$('#edit-newtheme-cancel-link').click();
	}catch(e){
		// to do
	}
}

function showCourseForm(){
	try{
	var classVisibility = $('#showform').is(':visible');
	$('#showform').css('display', 'block');
	$('#view-tp-list-wrapper .addedit-form-wrapper').hide();
	$('#program-course-attaching-from-addedit-form .addedit-form-wrapper').hide();
	$('#program-trainingplan-addedit-form-details').find('.edit-class-list').css({'background' : '#ffffff'});
	$('.edit-class-list').find('.admin-edit-button').css('display','block');
	if(classVisibility == true){
		//$('#showform').hide();
		$('#showform').css('display', 'none');
		
	} else {
		//$('#showform').show();
		$('#add-course-form-details').css('display', 'block');
		$('#showform').css('display', 'block');
	}
	$('#edit-newtheme-cancel-link').click();
	}catch(e){
		// to do
	}
}

function remove_success_messages(){
	try{
	$('#view-tp-list-wrapper .messages').remove();
	}catch(e){
		// to do
	}
}

function remove_error_messages(){
	try{
	$('#view-tp-list-wrapper .error').remove();
	$('#view-tp-list-wrapper .messages').remove();
	$('#no-courses-list').hide();
	}catch(e){
		// to do
	}
} 
function delete_operation(){
	try{
	$('#view-tp-list-wrapper .messages').remove();
	$('#view-tp-list-wrapper .error').remove();
	$('#showform').css('display', 'none');
	}catch(e){
		// to do
	}
}

function onLoadChangeGreyout() {
	try{
	var tp_type = $('#hiddengptype').val();
	$('#search_all_course_type-hidden').val(tp_type);
	$('.ac_results').css('display','none');
	if ($('#tpattchedcoursename-autocomplete').val() == '') {
    if (tp_type == 'grpnametype') {
      $('#tpattchedcoursename-autocomplete').val(Drupal.t('LBL036')+' '+Drupal.t('Group'));
    }
    else if (tp_type =='crstit') {
      $('#tpattchedcoursename-autocomplete').val(Drupal.t('LBL088')+' '+Drupal.t('LBL083'));
    }
    else if (tp_type =='crscode') {
        $('#tpattchedcoursename-autocomplete').val(Drupal.t('LBL036')+' '+Drupal.t('LBL096'));
      }
	}
	if($('#hiddengpval').val() == Drupal.t('LBL036') + ' ' + Drupal.t('Group') || $('#hiddengptype').val() == "grpnametype"){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036') + ' ' + Drupal.t('Group'));
	}
	var ac_name = $('#tp-admin-crs-search-textsearch').val();	
	if(ac_name == Drupal.t('LBL088')+' '+Drupal.t('LBL083')|| ac_name == Drupal.t('LBL036')+' '+Drupal.t('Group') ||  ac_name == Drupal.t('LBL036')+' '+Drupal.t('LBL096') ){
		$('#tp-admin-crs-search-textsearch').addClass('input-field-grey');
	}else{
		$('#tp-admin-crs-search-textsearch').removeClass('input-field-grey');
	}
	/*if(tp_type == 'grpnametype'){
		$('#select-list-course-dropdown').text(Drupal.t('Group'));
	} else if (tp_type =='crscode'){
		$('#select-list-course-dropdown').text(Drupal.t('LBL096'));
	}

	if(tp_type =='crstit' && ac_name!= Drupal.t('LBL088')+' '+Drupal.t('LBL083')){
		$('#tp-admin-crs-search-textsearch').removeClass('input-field-grey');
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL088')+' '+Drupal.t('LBL083'));
	}else if(tp_type =='grpnametype' && ac_name!= Drupal.t('LBL036')+' '+Drupal.t('Group')){
		$('#tp-admin-crs-search-textsearch').removeClass('input-field-grey');
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('Group'));
	}
	else if(tp_type =='crscode' && ac_name!= Drupal.t('LBL036')+' '+Drupal.t('LBL096')){
		$('#tp-admin-crs-search-textsearch').removeClass('input-field-grey');
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('LBL096'));
	}
	  
	if(tp_type == 'grpnametype' && ac_name== Drupal.t('LBL036')+' '+Drupal.t('Group')){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('Group'));
	}else if(tp_type =='crscode' && ac_name!= Drupal.t('LBL036')+' '+Drupal.t('LBL096')){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('LBL096'));
	}*/
	}catch(e){
		// to do
	}
}
function getTpSearchType(){
	try{
	var gptype= $('#search_all_course_type-hidden').val();
	$('#hiddengptype').val(gptype);
	}catch(e){
		// to do
	}
}
function cancelGroupDetails (uniqueId){
	try{
	var hidGroupName	= $('#hidden_group_name_'+uniqueId).val();
	var hidSequenceNo	= $('#hidden_sequence_no_'+uniqueId).val();
	var hidIsRequired	= $('#hidden_is_req_'+uniqueId).val();
	
	$('#edit-attachgroupname-'+uniqueId).val(hidGroupName);
	$('#edit-sequenceno-'+uniqueId).val(hidSequenceNo);
	if(hidIsRequired==1)
		$('#edit-is-required-'+uniqueId).attr('checked','checked');
	else
		$('#edit-is-required-'+uniqueId).removeAttr('checked');
	
	$('#view-tp-list-wrapper .messages').remove();
	var groupVisibility = $('#program-group-course-addedit-form-'+uniqueId).is(':visible');
	$('#showform').css('display', 'none');
	$('#view-tp-list-wrapper .addedit-form-wrapper').hide();
	$('#program-trainingplan-addedit-form-details').find('.edit-class-list').css({'background' : '#ffffff'});
	$('.edit-class-list').find('.admin-edit-button').css('display','block');
	if(groupVisibility == true){
		$('#program-group-course-addedit-form-'+uniqueId).hide();
	} else {
		$('#program-group-course-addedit-form-'+uniqueId).show();
		$('#delete_all_'+uniqueId).find('.edit-class-list').css({'background-color' : '#D7ECF9'});
	}
	$('#edit-newtheme-cancel-link').click();
	}catch(e){
		// to do
	}
}
function editTPDetailsView(){
	try{
	$('#program-trainingplan-addedit-form-details').find('.edit-class-list').css({'background' : '#ffffff'});
	$('.edit-class-list').find('.admin-edit-button').css('display','block');
	$('#view-tp-list-wrapper .addedit-form-wrapper').hide();
	$('#add-course-form-details').hide();
	}catch(e){
		// to do
	}
}
function attachToggle(toggleClassName){
	try{
		closeQtip('','');
		$('#add-group-popup').hide();
		$('.'+toggleClassName).toggle();
		if(toggleClassName == 'text-filter-box-class')
			$('#search-dropdwn-list-course').show();
		onLoadChangeGreyout();
		}catch(e){
			// to do
		}
}
function courseGroupSave(prgId){
	
	var dbvalue = $('#group_topic_title').val();
	var str = $('#program_attach_tabs .ui-state-active').attr('id');
	var currmod = str.split('-');
	
	url = '?q=administration/learning/program/group-save/'+prgId+'/'+currmod[2]+'/'+dbvalue;
	//url = obj.constructUrl('administration/learning/program/group-save/'+prgId+'/'+modId+'/'+dbvalue);
	$.ajax({
		url : url, //obj.constructUrl('administration/learning/program/group-save/'+prgId+'/'+modId+'/'+dbvalue),
		async: false,
		success: function(data) {
			$('#atach-course-hidden').click();
		}
	});
	
}
function searchCourseTP(prgid,showSearch){
	try{
		//console.log('tp custom function' + $('#lb-users-autocomplete').css('display'))
	this.currTheme = Drupal.settings.ajaxPageState.theme;
	if(showSearch == undefined)
		showSearch = true;
	if($('#tp-admin-text-filter-box').css('display') == "none" && showSearch == true){
		$('#tp-admin-text-filter-box').css('display','block');	
		$('#search-dropdwn-list-course').css('display','block');	
	}else{
		var str = $('#program_attach_tabs .ui-state-active').attr('id');
		var currmod = str.split('-');
		$('#tp-admin-text-filter-box').css('display','none');	
		$('#search-dropdwn-list-course').css('display','none');
		var InpVal = $('#tp-admin-crs-search-textsearch').val();
		$('#hiddengpval').val(InpVal);
	//	$('#hiddengptype').val("grpnametype");
		$('#mapped_module_id').val(currmod[2]);
		//Added for #0072459
		$('#hiddengptype-'+currmod[2]).val($('#search_all_course_type-hidden').val());
		$('#hiddengpval-'+currmod[2]).val(InpVal);
		
		$('#atach-course-hidden').click();
			
	}
	

	}catch(e){
		// to do
	}
}

function moduleTabDblClick(data,event){
	tpTabDoubleClick = true;
	$('#root-admin').data('narrowsearch').getInlineEditAttachedCourse(data,event);
}

function moduleTabclick(id){
	setTimeout(function(){
		if(tpTabDoubleClick == false){
			$('input[id="mapped_module_id"]').attr('value',id);
			$('.attach-grp-links').css('display', 'none');
			$('.attach-module-link').css('display', 'none');
			closeQtip('','');
			var prgId =$('input[name="mapped_program_id"]').val();
			/*Disable attach course button if the module has enrollments*/
			
			//Added for #0072459
			var chtype = $('#hiddengptype-'+id).val() == '' ? 'crstit' : $('#hiddengptype-'+id).val();
			$('#search_all_course_type-hidden').val(chtype);
			var name= Drupal.t('LBL083'); 
			switch(chtype){
				case "crstit": name= Drupal.t('LBL083'); break;
				case "grpnametype": name = Drupal.t('Group'); break;
				case "crscode": name = Drupal.t('LBL096'); break;
			};
			moreCourseSearchTypeText(name,chtype);
			if($('#hiddengpval-'+id).val() != '')
				$('#tp-admin-crs-search-textsearch').val($('#hiddengpval-'+id).val());
			$('#tp-admin-text-filter-box').hide();
			// End  #0072459
			url = '?q=administration/module/enroll-cnt/'+prgId+'/'+id+'/ajax';
			$.ajax({
				url : url,
				async: false,
			success: function(result) {			
			        if(result > 0){
						$('.clsdisbleattachcourse').show();
						$('.clsenableattachcourse').hide();
						//$('#list-options-tp').hide();
					}else{
						$('.clsdisbleattachcourse').hide();
						$('.clsenableattachcourse').show();
						$('#add-new-coursetoTp-'+prgId).show();
						$('#list-options-tp').show();
					}
				}
			});
		}
	},100);
	//moreCourseSearchTypeText(Drupal.t('LBL083'),'crstit');
	
}

function scorllModTabPrev(){
	try{
		if($('#program_attach_tabs .first-reached').size() > 0){
			return '';
		}
		var addMore = false;
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('page-container-tabs-prg');
		var liCount = $('#program_attach_tabs .admin-module-navigation .ui-corner-top').size();
		var activeList = $('#program_attach_tabs .visible-main-tab').size();
		var activeLiLast = $( "#program_attach_tabs .visible-main-tab:first" ).index();
		var firstTabActiv = $('#program_attach_tabs .admin-module-navigation li:eq(1)').attr('class').indexOf('visible-main-tab') != -1 ? true : false;
		//$('#program_attach_tabs .admin-module-navigation .visible-main-tab:first').index()
		var totalMod = $("#total-modules").val() ; 
		if(liCount > activeList && firstTabActiv == false){
			$('#program_attach_tabs ul li:eq('+(activeLiLast-1)+')').removeClass('hidden-main-tab');
			$('#program_attach_tabs ul li:eq('+(activeLiLast-1)+')').addClass('visible-main-tab');
			$('#program_attach_tabs .visible-main-tab:last').removeClass('visible-main-tab').addClass('hidden-main-tab');
			//$('#program_attach_tabs .visible-main-tab:last .attachedcourse-name-container a').click();
			if($('#program_attach_tabs .admin-module-navigation li:eq(1)').attr('class').indexOf('visible-main-tab') != -1 && liCount == totalMod)
				$( ".first-arrow" ).addClass('first-reached ui-state-disabled');
		}else{
			if(liCount == activeList && firstTabActiv == false && liCount < totalMod){
				$('#program_attach_tabs ul li:eq(1)').removeClass('hidden-main-tab');
				$('#program_attach_tabs ul li:eq(1)').addClass('visible-main-tab');
				$('#program_attach_tabs .visible-main-tab:last').removeClass('visible-main-tab').addClass('hidden-main-tab');
				//$('#program_attach_tabs .visible-main-tab:last .attachedcourse-name-container a').click();
			}else if(liCount < totalMod){
				EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('attach_course_dt');
				addMore = true;
				$('#tab-left-arrow').click();
			}
		}
		if(addMore == false)
			changeArrowStatus('prev');
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('page-container-tabs-prg');
	}catch(e){
			// To Do
		}
}

function scorllModTabNext(){
	try{
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader('page-container-tabs-prg');
		var liCount = $('#program_attach_tabs .admin-module-navigation .tp-module-tab').size();
		var activeLiLast = $('#program_attach_tabs .visible-main-tab:last').index();
		var lastMod = $('#program_attach_tabs .admin-module-navigation .tp-module-tab:last');
		/*var remcnt = ($('#add-module-list').length) > 0 ? 4 : 2;
		if(activeLiLast==(liCount-remcnt)){
			return '';
		}*/
		if(activeLiLast==lastMod){
			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('page-container-tabs-prg');
			return '';
		}
		if(liCount>activeLiLast){
			$('#program_attach_tabs ul li:eq('+(activeLiLast+1)+')').removeClass('hidden-main-tab');
			$('#program_attach_tabs ul li:eq('+(activeLiLast+1)+')').addClass('visible-main-tab');
			$('#program_attach_tabs .visible-main-tab:first').removeClass('visible-main-tab').addClass('hidden-main-tab');
			$( ".first-arrow" ).removeClass('first-reached');
		}
		changeArrowStatus('next');
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('page-container-tabs-prg');
	}catch(e){
			// To Do
		}
}

function changeArrowStatus(callFrom){	
	
	if(parseInt($("#total-modules").val())<=4){
		$('.first-arrow').css('display','none');
		$('.last-arrow').css('display','none');
	}else{
		$('.first-arrow').css('display','block');
		$('.last-arrow').css('display','block');
	}
	var prevIdx = $('#program_attach_tabs .ui-state-active').index();
	$('#program_attach_tabs .last-arrow a').removeClass('clspatharraowenabled');
	
	if($('#program_attach_tabs .first-arrow').next("li").hasClass('visible-main-tab'))
		$('#program_attach_tabs .first-arrow').addClass('ui-state-disabled');
	else
		$('#program_attach_tabs .first-arrow').removeClass('ui-state-disabled');
	
	if($('#program_attach_tabs .last-arrow').prev("li").hasClass('visible-main-tab')){
		$('#ctools-modal-content #program_attach_tabs .admin-module-navigation li.last-arrow').css('opacity','0.35');
		$('#program_attach_tabs .last-arrow').addClass('ui-state-disabled');
	}else{
		$('#ctools-modal-content #program_attach_tabs .admin-module-navigation li.last-arrow').css('opacity','1');
		$('#program_attach_tabs .last-arrow').removeClass('ui-state-disabled');	
	}
	var selected = null;
	$('#program_attach_tabs .visible-main-tab').each(function(){
		if($(this).attr('class').indexOf('ui-state-active') != -1){
			selected = $(this).index();
		}
	});
	
	if(selected == null){
		if(callFrom=='prev'){
			prevIdx = prevIdx - 1;
			if($('#program_attach_tabs li:eq('+prevIdx+')').hasClass('visible-main-tab')){
				selected = prevIdx;
			}else{
				selected = $('#program_attach_tabs .visible-main-tab:last').index();
			}
		}else if(callFrom=='next'){
			prevIdx = prevIdx + 1;
			if($('#program_attach_tabs li:eq('+prevIdx+')').hasClass('visible-main-tab')){
				selected = prevIdx;
			}else{
				selected = $('#program_attach_tabs .visible-main-tab:first').index();
			}
		}
		//$('#program_attach_tabs .visible-main-tab:first a').click();
		
	}/*else{
		$('#program_attach_tabs .visible-main-tab li:eq('+selected+') a').click();
	}*/
	$('#program_attach_tabs li:eq('+selected+') a').click();
	var liCount2 = $('#program_attach_tabs .admin-module-navigation .ui-corner-top').size();
	if(liCount2 != $("#total-modules").val()){
		$( ".first-arrow" ).removeClass('ui-state-disabled');
	}
}
$(document).ready(function() {
	//changeArrowStatus();
});

function resetMainTab(){
	try{
	var selected = $('#program_attach_tabs .selected').index();
	var lastLi = $('#program_attach_tabs .visible-main-tab:last').index();
	if($('#program_attach_tabs .selected').css('display')=='none'){
		for(var i=lastLi;i<selected;i++){
			scorllModTabNext();
		}
	}
	}catch(e){
			// To Do
		}
}
function onloadsearch(){
	var prg_id = $('input[name=mapped_program_id]').val();
	var str = $('#program_attach_tabs .ui-state-active').attr('id');
	var currmod = str.split('-');
	$('#search_leaderboard-users_txt').autocomplete(
			"/?q=administration/learning/program/course-autocomplete/"+prg_id+"/"+currmod[2]+"/",{
			extraParams : {'exclude_logged_user' : 1,'search_by_username' : 1},
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false
	});
}

function loadCustomPopupLeft(prgId,url){
	try{
	var obj = this;
	var param = '';
	//obj.createLoader('report_criteria_form');
	//url = obj.constructUrl("administration/learning/program/add-module/"+prgId);
	url = '?q='+url + '/' + $('#tab_count').val() + "/" + $('#attach_course_dt form input[name="form_build_id"]').val();
	$.ajax({
		 type: "GET",
         url: url,
         data:  '',
		success: function(data){
			//if(data.result == 'updated'){
				var html = '';
				var columnName = '';
				html+='<div class="qtip-tip-point-left"></div>';
				html+='<table cellspacing="0" cellpadding="0" id="bubble-face-table" class="bubble-table-container">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-tl"></td>';
				html+='<td class="bubble-t"></td>';
				html+='<td class="bubble-tr"><a onclick="closeQtip(\'\',\'\');" class="qtip-close-button-visible" id="admin-bubble-close"></a></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-cl"></td>';
				html+='<td valign="top" class="bubble-c">';
				html += '<div class="module-scroll-pane-container">';
				html += '<div id="show_expertus_message"></div>';
				html+= data.render_content_main;
				html+='<td class="bubble-cr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-bl"></td>';
				html+='<td class="bubble-b">';
				html+='<table width="100%" cellspacing="0" cellpadding="0">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-blt"></td>';
				html+='<td class="bubble-blr"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				html+='</td>';
				html+='<td class="bubble-br"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				$('#add_new_module_popup').html(html);
				$('#add_new_module_popup').show();
				
				/*$('#report_add_narrow_filter_popup .add_button').click(function(){
					var criteriaData = $(this).prev().attr('data');
					var criteriaDataArray = criteriaData.split('$$');
					obj.addNarrowFilter(criteriaDataArray[0], criteriaDataArray[1]);
				});*/
				
			//}
			//obj.destroyLoader('report_criteria_form');
			$('.module-scroll-pane').jScrollPane();
				$.extend(true, Drupal.settings, data.drupal_settings);
				Drupal.attachBehaviors();
					vtip();
		}
	}); 	
	}catch(e){
 		//Nothing to do
	}
}
function confirmationForMdlDel(prgId, module_id,module_title,entityType){
	//this.currTheme = Drupal.settings.ajaxPageState.theme;
	var uniqueClassPopup = '';
	var esignForType = '';
	esignForType = 'confirmationForMdlDel';
	 $('#delete-msg-wizard').remove();
	    var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    //if(this.currTheme == 'expertusoneV2'){
	    
	  /*  var args = {};	   
	    args['!name'] = '"'+module_title+'"';*/
	    html+= '<tr><td style="padding: 10px 24px;" class="commanTitleAll">'+Drupal.t("MSG357")+' "'+module_title+'"?</td></tr>';
	    /*} else {
	     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+Drupal.t("MSG747")+' "'+ codeval +' <span class="currency-override-bold">'+ symbol +'</span>"?'+'</td></tr>';
	    }*/
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);

	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};

		 if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		var esignObj = {'popupDiv':'delete-object-dialog','esignFor':esignForType,'objectId':prgId,'moduleId': module_id,'objectType':entityType};
		closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);  };
		}else{
		closeButt[Drupal.t('Yes')]=function(){
		deleteModuleCall(prgId, module_id,entityType);
		}
		}
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:300,
	        resizable:false,
	        minHeight:"auto",
	        modal: true,
	        title:Drupal.t('LBL749'),
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        zIndex : 10005,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	    $('.ui-dialog').wrap("<div id='tp-delete-module-dialog'></div>");
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $(".removebutton").text(Drupal.t("No"));
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		$('.admin-save-button-middle-bg').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		//if(this.currTheme == 'expertusoneV2'){
	    	changeDialogPopUI();
	   // }
}
function deleteModuleCall(prgId, module_id,entityType){
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	var timer = 100;
	var list = {};
	var deletedId = $('#page-container-tabs-prg #module-list-'+module_id).attr('id');
	var deletedIdx = $('#page-container-tabs-prg #module-list-'+module_id).index();
	var selectedId = $('#program_attach_tabs .ui-state-active').attr('id');
	var selectedIdx = $('#program_attach_tabs .ui-state-active').index();
	var listCnt = $('#program_attach_tabs .admin-module-navigation .tp-module-tab').size();
	// Since the index will calculate based on the li count, we need to reduce one value if left side arrow is available
	listCnt = $('.first-arrow').size() > 0 ? listCnt : listCnt - 1;
	var modCnt = $("#total-modules").val();
	var i=0;
	$('#program_attach_tabs .admin-module-navigation .visible-main-tab').each(function(){
		list[i] = {};
		list[i]['id'] = $(this).attr('id')
		list[i]['idx'] = $(this).index();
		list[i]['sel'] = $(this).attr('class').indexOf('ui-state-active') != -1 ? 1 : 0;
		i++;
	});
	i--;
	// Add more module (if any) while delete an existing module 
	if(listCnt<=modCnt && $('.first-arrow').size() > 0 && $('#program_attach_tabs .admin-module-navigation .visible-main-tab:first').index() == 1){
		scorllModTabPrev();
		timer = 1500;
	}

	setTimeout(function(){
		url = '?q=administration/learning/program/delete-module/'+prgId+'/'+module_id+'/'+entityType;
		$.ajax({
			url : url, 
			async: false,
			success: function(data) {
				
				var showIdx;
				var selectIdx;
				if(timer == 1500){ // New modules are added
					// Hide all tabs
					$('#program_attach_tabs .admin-module-navigation .visible-main-tab').removeClass('visible-main-tab').addClass('hidden-main-tab');
					
					listCnt = $('#program_attach_tabs .admin-module-navigation .tp-module-tab').size(); // reset total module tabs
					$.each(list, function(k,v){
						var newIdx = $('#'+v.id).index();
						if(v.id != deletedId){ // re-show the existing shown module except deleted
							$('#'+v.id).removeClass('hidden-main-tab').addClass('visible-main-tab');
						}else{
							deletedIdx =  newIdx// reset deleted index with new index value
						}
						if(v.sel==1)
							selectedIdx = newIdx; // reset selected index with new index value
						
						v.idx = newIdx; // reset old index with new index with new index value
					})
				}
				//console.log("deletedIdx - "+deletedIdx+" total count - "+listCnt+" selected idx - "+selectedIdx);

				// Define active tab
				if(deletedIdx == listCnt){ // last module deleted
					selectIdx = listCnt - 1; // select previous tab of the deleted tab
				}else{
					// if the active tab is deleted then the new tab replacing that position should be a active tab
					// otherwise move one position forward to keep the existing active tab 
					selectIdx = (deletedId == selectedId) ? selectedIdx : selectedIdx - 1; 
				}
				// Since the index will calculate based on the li count, we need to reduce one value if left side arrow is available
				selectIdx = $('.first-arrow').size() > 0 ? selectIdx - 1 : selectIdx;
				
				// Define which tab should visible as a replacement of deleted tab
				if(list[i].idx < listCnt){
					showIdx = list[i].idx + 1; // add one module from right 
				}else{
					showIdx = list[0].idx - 1; // add one module from left 
				}
				// reset to 1 if the computed value become 0
				showIdx = showIdx <= 0 ? 1 : showIdx;
				// set visible class to the newly shown tab
				$('#program_attach_tabs .admin-module-navigation .ui-corner-top:eq('+(showIdx-1)+')').removeClass('hidden-main-tab').addClass('visible-main-tab');
				
					//console.log("added tab - "+showIdx+" Selected tab - "+selectIdx);
				// Remove deleted tab and course list sections
				$('#page-container-tabs-prg #module-list-'+module_id).remove();
				$('#page-container-tabs-prg #module-course-list-'+module_id).remove();
				
				$( "#page-container-tabs-prg" ).tabs('destroy');
				$( "#page-container-tabs-prg" ).tabs({
					selected: selectIdx
				});
				
				$("#total-modules").val(parseInt($("#total-modules").val())-1);
				changeArrowStatus();
				obj.destroyLoader('page-container-tabs-prg');
			}
		});
		add_module_toggle();
	},timer);
	
}

function loadCustomPopupRight(prgId,url,container){
	try{
	var obj = this;
	var param = '';
	var str = $('#program_attach_tabs .ui-state-active').attr('id');
	var currmod = str.split('-');
	url = '?q='+url + '/' + currmod[2];
		$.ajax({
			 type: "GET",
	         url: url,
	         data:  '',
			success: function(data){
				var html = '';
				html+='<div class="qtip-tip-point-right"></div>';
				html+='<table cellspacing="0" cellpadding="0" id="bubble-face-table" class="bubble-table-container" style="z-index:200">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-tl"></td>';
				html+='<td class="bubble-t"></td>';
				html+='<td class="bubble-tr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-cl"></td>';
				html+='<td valign="top" class="bubble-c">';
				html += '<div id="show_expertus_message"></div>';
				html+= data.render_content_main;
				html+='</td>';
				html+='<td class="bubble-cr"></td>';
				html+='</tr>';
				html+='<tr>';
				html+='<td class="bubble-bl"></td>';
				html+='<td class="bubble-b">';
				html+='<table width="100%" cellspacing="0" cellpadding="0">';
				html+='<tbody>';
				html+='<tr>';
				html+='<td class="bubble-blt"></td>';
				html+='<td class="bubble-blr"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				html+='</td>';
				html+='<td class="bubble-br"></td>';
				html+='</tr>';
				html+='</tbody>';
				html+='</table>';
				$('#'+container).html(html);
				$('#'+container).show();
				$("#tp-list-search-container").find("#objectTypeSearch").css("z-index","0");
				Drupal.attachBehaviors();
			}
		}); 	
	}catch(e){
 		//Nothing to do
	}
}
(function($) {
	$.fn.resetEditGroupDetails = function(uniqueId) {
		try{
		var hidGroupName	= $('#hidden_group_name_'+uniqueId).val();
		var hidSequenceNo	= $('#hidden_sequence_no_'+uniqueId).val();
		var hidIsRequired	= $('#hidden_is_req_'+uniqueId).val();
		$('#edit-attachgroupname-'+uniqueId).val(hidGroupName);
		$('#edit-sequenceno-'+uniqueId).val(hidSequenceNo);
		if(hidIsRequired==1)
			$('#edit-is-required-'+uniqueId).attr('checked','checked');
		else
			$('#edit-is-required-'+uniqueId).removeAttr('checked');
		}catch(e){
			// to do
		}
	};
	$.fn.updateEditGroupDetails = function(uniqueId) {
		try{
		var updatehidGroupName	= $('#edit-attachgroupname-'+uniqueId).val();
		var updatehidSequenceNo	= $('#edit-sequenceno-'+uniqueId).val();
		var updatehidIsRequired	= $('#edit-is-required-'+uniqueId).val();		
		$('#hidden_group_name_'+uniqueId).val(updatehidGroupName);
		$('#hidden_sequence_no_'+uniqueId).val(updatehidSequenceNo);
		if($('#edit-is-required-'+uniqueId).is(':checked') == true){
			$('#hidden_is_req_'+uniqueId).val(1);
		} else {
			$('#hidden_is_req_'+uniqueId).val(0);
		}
		}catch(e){
			// to do
		}
	};
		
	
	
})(jQuery);

function changeTrpDeliveryType(deliveryType){
	try{
	var selectId = $(deliveryType).attr('id');
	var delTypeCode = $('#'+selectId).val();
	if(delTypeCode == 'cre_sys_obt_trn' || delTypeCode == 'cre_sys_obt_crt'){
		$('#admin-trp-delivery-data-part').show();
		if(delTypeCode == 'cre_sys_obt_crt'){
			$('#admin-trp-delivery-data-part-expires').show();
			$('#admin-trp-delivery-data-part-compdate').hide();
		}else{
			$('#admin-trp-delivery-data-part-expires').hide();
			$('#admin-trp-delivery-data-part-compdate').show();
		}
	}
	else{
		$('#admin-trp-delivery-data-part').hide();
	}
	}catch(e){
		// to do
	}
}

function moreCourseSearchTypeText(dCode,dText) {
	//alert("test")
    //console.log('moreCourseSearchTypeText() called');
    $('#select-list-course-dropdown').text(dCode);
    $('#search_all_course_type-hidden').val(dText);
    var gptype= $('#search_all_course_type-hidden').val();
  //  var InpVal = $('#tp-admin-crs-search-textsearch').val();
	$('#hiddengpval').val(dCode);
	//$('#hiddengptype').val(gptype);
	$('#hiddengptype').val(dText);
    var displayText;
    if(gptype=='crstit'){
 	   displayText = Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083');
    }else{
 	   displayText =Drupal.t('LBL036') + ' ' + dCode;
    }
    //var displayText = Drupal.t('LBL036') + ' ' + dCode;
    
	$('#tp-admin-crs-search-textsearch').val(displayText);
	$('#tpattchedcoursename-autocomplete_hidden').val(displayText);
	$('#tp-admin-crs-search-textsearch').addClass('input-field-grey');
	//$('#atach-course-hidden').click();

  }
function onblurInputSearch(obj){
	if($('#hiddengpval').val() == Drupal.t('LBL036') + ' ' + Drupal.t('Group') || $('#hiddengptype').val() == "grpnametype"){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036') + ' ' + Drupal.t('Group'));
	}
	if($('#hiddengpval').val() == Drupal.t('LBL036') + ' ' + Drupal.t('LBL096') || $('#hiddengptype').val() == "crscode"){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036') + ' ' + Drupal.t('LBL096'));
	}
	if(obj.value == "" ){
		obj.value = $('#tpattchedcoursename-autocomplete_hidden').val();
		obj.style.color = "#cccccc";
		obj.style.fontStyle= "normal";
	}
	if(obj.value != "" && obj.value != Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083') 
			&& obj.value != Drupal.t('LBL036') + ' ' + Drupal.t('Group') && obj.value != Drupal.t('LBL036') + ' ' + Drupal.t('LBL096') ){
		obj.style.color = "#333333";
		obj.style.fontStyle= "normal";
	}
	if(obj.value == Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083') 
			|| obj.value == Drupal.t('LBL036') + ' ' + Drupal.t('Group') 
			|| obj.value == Drupal.t('LBL036') + ' ' + Drupal.t('LBL096')){
		obj.style.fontStyle= "normal";
	}
}
function onfocusInputSearch(obj){
	if($('#hiddengpval').val() == Drupal.t('LBL036') + ' ' + Drupal.t('Group')){
		$('#tpattchedcoursename-autocomplete_hidden').val(Drupal.t('LBL036') + ' ' + Drupal.t('Group'));
	}
	if(obj.value == Drupal.t('LBL088')+ ' '+ Drupal.t('LBL083') 
			|| obj.value == Drupal.t('LBL036') + ' ' + Drupal.t('Group') 
			|| obj.value == Drupal.t('LBL036') + ' ' + Drupal.t('LBL096')){
		obj.value = "";
		obj.style.color = "#333333";
		obj.style.fontStyle= "normal";
	}
}
function updInput(id){
	 /*if ( $('#add_module_'+id).hasClass('selected') ) {
            $('#add_module_'+id).removeClass('selected');
            $('input[id="module_sel"]').attr('value','');
     }else {*/
            $('#add-module-table tr.selected').removeClass('selected');
            $('#add_module_'+id).addClass('selected');
            $('input[id="module_sel"]').attr('value',id);
      // }
	//$('input[id="module_sel"]').attr('value',id);
}
function sequenceDragAndDropPrg(containerId, type, dragId,prgId,mdlId){
	try{
	if(type == 'attach_course'){
	  $("#"+containerId).sortable({
		handle: '.dragndrop-selectable-item',
		cursor: 'crosshair',
		connectWith: "ul",
		dropOnEmpty: true
	  });
	}else{
		$("#"+containerId).sortable({
			handle: '.dragndrop-selectable-item',
			cursor: 'crosshair',
			connectWith: "div"
		  });
	}
	$("#"+containerId).droppable({
		drop: function( event, ui ) {
//			console.log('drop executed' + event);
//			console.log('drop executed' + ui);
//			console.log("container id " + containerId);
//			console.log("type id " + type);
//			console.log("drag id " + dragId);
//			console.log("prg id " + prgId);
//			console.log("mdl id " + mdlId);
			var sequenceOrderArray = new Array() ;
			var sequenceOrderArray1 = new Array();
			var j = 0;
			var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
			obj.createLoader(dragId);
			setTimeout(function(){
			if($("#"+dragId).find( "li.drag-grp-head").length > 0){
				$("#"+dragId).find( "li.drag-grp-head").each(function(){
					var grpId = $(this).attr('id');
	        		var grpName = $(this).attr('data');
	        		var Ids = grpId.split('-');
					var i = 0;
				        sequenceOrderArray1 = new Array();
						$('#'+grpId).find( "ul li.draggable-list").each(function(){
							var crs = $(this).attr('id');
							var crsId = crs.split('-');
	            			sequenceOrderArray1[i] = crsId[0];
							i++;
						});
				        sequenceOrderArray[j] = {};
				        sequenceOrderArray[j].GroupId = grpName;
				        sequenceOrderArray[j].GroupCode = Ids[2];
				        sequenceOrderArray[j].CrsId=sequenceOrderArray1;
				        j++;
				        // Added for 0072674
				        // Enable Group delete when there is no courses on it
				        if($(this).find("li ul li").length == 0 && $(this).find(".grp-delete").length > 0){
				        	if($(this).find(".grp-delete").attr('class').indexOf('recertify-grp-delete-disable') != -1){
								var action = $(this).find(".grp-delete").attr('param');
								//var param = action.substring(action.indexOf('"')+1,action.length-2);
								$(this).find(".grp-delete").attr('class','attach-course-delete grp-delete recertify-grp-delete');
								$(this).find(".grp-delete").replaceWith($(this).find(".grp-delete")[0].outerHTML.replace('param=','onclick='));
								$(this).find(".grp-delete").bind('click',function(){action});
				        	}
						}
				        // Disable Group delete if any courses added to it
						if($(this).find("li ul li").length > 0 && $(this).find(".grp-delete").length > 0){
							if($(this).find(".grp-delete").attr('class').indexOf('recertify-grp-delete-disable')){
								$(this).find(".grp-delete").attr('class','attach-course-delete grp-delete recertify-grp-delete-disable');
								$(this).find(".grp-delete").replaceWith($(this).find(".grp-delete")[0].outerHTML.replace('onclick=','param='));
								$(this).find(".grp-delete").unbind('click');
							}
						}
				});
			}else if($("#"+dragId).find("li.draggable-list").length > 0){
				var i = 0;
				var sequenceOrderArray1 = new Array();
				$("#"+dragId).find( "li.draggable-list").each(function(){
					var crs = $(this).attr('id');
					var crsId = crs.split('-');
	    			sequenceOrderArray1[i] = crsId[0];
					i++;
				});
				sequenceOrderArray[j] = {};
		        sequenceOrderArray[j].GroupId = 0;
		        sequenceOrderArray[j].GroupCode = "";
		        sequenceOrderArray[j].CrsId=sequenceOrderArray1;
			}
			
			obj.createLoader(dragId);
			var str = EXPERTUS.SMARTPORTAL.AbstractManager.convertJsonToString(sequenceOrderArray);
			var i=1;
			url = "?q=administration/sequence-drag-drop-prg/"+str+"/"+prgId+"/"+mdlId;
			$.ajax({
				type : "POST",
				url : url,
				async: false,
				success: function(data) {
					obj.destroyLoader(dragId);
					var sequenceArray = sequenceOrderArray.filter(function(v){return v!==''});
					var list_values = ($('#'+containerId +' li.draggable-list').length)-1;
					var arrLen = sequenceArray.length;
					if(list_values == arrLen ) {
						var i = 0;
						for(i=0; i<arrLen; i++) {
							$("#"+containerId +" li[id="+ sequenceArray[i] +"]").removeClass('odd-even-row-highlighter');
							if (i%2 == 1) {
								$("#"+containerId +" li[id="+ sequenceArray[i] +"]").addClass('odd-even-row-highlighter');
							}
						}
					}

				}
			});
			//obj.destroyLoader(containerId);
			},500);
			//foreach for group and courses count
			
			}
	});

	}catch(e){
			// To Do
		}
}
function fillSurveyPath(uniqueId,type){
	if(type == 'roster'){
		var overallPath = $('select#certifcatepath option:selected').val();
		var overallName = $('select#certifcatepath option:selected').attr("name");
		$('input[name="selected_enroll_path"]').val(overallPath);
		$('#recertifyandsave_'+uniqueId).click();
	}else{
		var overallPath = $('select#certifcatepath option:selected').val();
		$('input[name="hidden_certificate_path_'+uniqueId+'"]').val(overallPath);
		$('#add_new_'+uniqueId).click();
	}
}
function renderCourseList(moduleId){
	try {
		$('#course-table-'+moduleId).toggle();
		var modId = $('#detail-code-expand-collapse-'+moduleId);
		var clsName = $('#detail-code-expand-collapse-'+moduleId).attr('class');		 
		if(clsName=='title_close'){
			modId.removeClass('title_close');
			modId.addClass('title_open');
		}else if(clsName=='title_open') {
			modId.removeClass('title_open');
			modId.addClass('title_close');
		}
		if($('#view-scroll-wrapper').length > 0 && $('#view-scroll-wrapper').data('jsp') !== undefined) {
			$('#view-scroll-wrapper').data('jsp').reinitialise();
		}
	} catch(e) {
		// window.console.log(e, e.stack);
	}
}
function certificatePathQTip(uniqueId,type){
	//console.log('test ' + '#registration_date_container_'+uniqueId); 
	if(type == 'survey'){
		$('#survey_tp_container_'+uniqueId).show();
	}else if(type == 'roster'){
		$('#visible-roster-path-'+uniqueId).click();
	}else{
		$('#assessment_detail_container_'+uniqueId).show();
	}

}

(function($) {
	$.fn.validateDeliveryPart = function(objectType) {
    try{
	if(objectType=='cre_sys_obt_crt'){
		$('#admin-trp-delivery-data-part').show();
		$('#admin-trp-delivery-data-part-compdate').hide();
		$('#admin-trp-delivery-data-part-expires').show();
	}else if(objectType=='cre_sys_obt_trn'){
		$('#admin-trp-delivery-data-part').show();
		$('#admin-trp-delivery-data-part-compdate').show();
		$('#admin-trp-delivery-data-part-expires').hide();
	}
	//return false;
    }catch(e){
		// to do
	}
};
})(jQuery);
(function($) {
	try{
	$.fn.setNotifyContainerWidth = function() {
	vtip();
	$('.catalog-course-basic-addedit-form-container').css('padding-left','15px');
	$('.catalog-course-basic-addedit-form-container').css('width','625px');
	$('.addedit-form-cancel-container-actions').css('margin-right','12px');
	
};
//if($.browser.msie && parseInt($.browser.version, 10)=='9'|| $.browser.msie && parseInt($.browser.version, 10)=='10'){
	
/*$(".certifcatepath-custom-select-arrow").live("click",function(){
	console.log("load select click");
	$(this).parent(".certifcatepath-custom-select").children("#certifcatepath").change(function(){});
});*/
//}
	}catch(e){
		// to do
	}
})(jQuery);

$('body').click(function (event) {
	try{
	if(event.target.id!='pub-unpub-action-btn') {
		$('.catalog-pub-add-list').hide();
	} 
	var target = $(event.target).parents('#tp-admin-text-filter-box');
	if(target.length == 0 && event.target.id != 'advanced_serach'){
		$('.text-filter-box-class').hide();
	}
	}catch(e){
		// to do
	}
});
$('#tp-admin-crs-search-textsearch').live("keypress", function(e) {
	var str = $('#program_attach_tabs .ui-state-active').attr('id');
	var currmod = str.split('-');
	var prgId =$('input[name="mapped_program_id"]').val();
	var InpVal = $('#hiddengptype').val();
	$('#tp-admin-crs-search-textsearch').autocomplete(
			"/?q=administration/learning/program/course-autocomplete/"+prgId+"/"+currmod[2]+"/&search_type="+InpVal+"",{
			minChars :3,
			max :50, 
			autoFill :true,
			mustMatch :false,
			matchContains :false,
			inputClass : "ac_input",
			loadingClass: "ac_loading"	
			});
});

(function($) {
	$.fn.postPagination = function(element_settings,type,selector,data) {
		try{
			if(type == 'tab_paginate'){ // Add more tabs when pagination
				// Copy the inner html of the dummy div which contains the new tab tags
				var thtm = $('#dummy-tab-content').html();
				// Get the last index of the newly appended tab
				var lastAddIdx = $('#dummy-tab-content .tp-module-tab:last').index();
				// Remove the dummy div with its contents
				$('#dummy-tab-content').remove();
				// Append the copied content after the left arrow
				// Since we are showing the last module at the beginning the pagination should be in reverse order   
				$('#exp_sp_administration_program_attaching_courses #program_attach_tabs ul li:eq(0)').after(thtm);
				
				// Similar to above doing for course list div
				var chtm = $('#dummy-course-content').html();
				$('#dummy-course-content').remove();
				$('#page-container-tabs-prg #exp_sp_administration_program_attaching_courses').after(chtm);
				
				// Get the latest count of the tabs
				var liCount2 = $('#program_attach_tabs .admin-module-navigation .tp-module-tab').size();
				// Remove all the visible classes, since we need to re-arrange the tabs
				$('#program_attach_tabs .visible-main-tab').removeClass('visible-main-tab').addClass('hidden-main-tab');
				
				// Arrange the visible tabs. To keep the default tab's pagination behavior (move tabs one by one) 
				// the show-able starting tab should be the last tab of the newly added tabs   
				// In case if the tab should move by set (all 4 tabs) the loop start value should be 0
				for(var i=lastAddIdx; i<(lastAddIdx + 4);i++ ){
					$('#program_attach_tabs ul li:eq('+(i+1)+')').removeClass('hidden-main-tab').addClass('visible-main-tab');
				}
				// Find the privious selected tab
				var prevIdx = $('#program_attach_tabs .ui-state-active').index();
				if(!$('#program_attach_tabs li:eq('+prevIdx+')').hasClass('visible-main-tab')){
					prevIdx = prevIdx - 1;
				}
				// Reset tab control with latest values
				$( "#page-container-tabs-prg" ).tabs('destroy');
				$( "#page-container-tabs-prg" ).tabs({
					selected: prevIdx - 1
				});
				var str = $('#program_attach_tabs .ui-state-active').attr('id');
				var currmod = str.split('-');
				moduleTabclick(currmod[2]);
				// Add Drupal behavior for the added tabs
				$.each(element_settings,function(k, v){
					$('#'+k).removeClass('ajax-processed');
					Drupal.attachBehaviors('#'+k,v);
				});
				EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader('attach_course_dt');
			}else if(type == 'add_tabs'){ // Add new tab when clicking add module
				// Take the inner html of the dummy div
				var thtm = $('#dummy-tab-content').html();
				// Check if already pagination arrows are present or not
				var is_paginate = $('#attach_course_dt .first-arrow').size();
				// Remove dummy div
				$('#dummy-tab-content').remove();
				// Add new module tab at the end (before right arrow)
				$('#exp_sp_administration_program_attaching_courses #program_attach_tabs ul .ui-corner-top:last').after(thtm);

				// While adding 5th module on the fly the pagination arrow should be added in both the end
				// if there is no arrow added earlier
				if(is_paginate > 0 && $('.first-arrow').size() <= 1){
					var page_html = $('.first-arrow')[0].outerHTML;
					$('#exp_sp_administration_program_attaching_courses #program_attach_tabs ul .first-arrow').remove();
					$('#exp_sp_administration_program_attaching_courses #program_attach_tabs ul .ui-corner-top:first').before(page_html);
					$('.first-arrow').removeClass('first-reached');
				}else if(is_paginate > 0){
					// Adding 5th module and delete it, then again add module 5 (on the fly) 
					// the arrows will be added twice. In such as case removing the extra arrows in both the end
					if($('.last-arrow').size() == 2){
						$('.first-arrow:last').remove();
						$('.last-arrow:last').remove();
					}
					$('.first-arrow').removeClass('first-reached');
				}
				
				// Similarly adding course list div
				var chtm = $('#dummy-course-content').html();
				$('#dummy-course-content').remove();
				$('#page-container-tabs-prg .module-course-lists:last').after(chtm);
				
				// Get the latest tab counts
				var liCount2 = $('#program_attach_tabs .admin-module-navigation .tp-module-tab').size();
				// Remove all visible classes
				$('#program_attach_tabs .visible-main-tab').removeClass('visible-main-tab').addClass('hidden-main-tab');
				// Add visible class for the last 4 tabs, since the newly added tab should be visible and active
				for(var i=4; i>0;i-- ){
					var j = $('.first-arrow').size() > 0 ? (liCount2-i)+1 : (liCount2-i);
					$('#program_attach_tabs ul li:eq('+(j)+')').removeClass('hidden-main-tab').addClass('visible-main-tab');
				}
				activeLiLast = $( "#program_attach_tabs .visible-main-tab:last" ).index();
				activeLiLast = $('.first-arrow').size() > 0 ? activeLiLast : activeLiLast + 1;
				// Reset tabs
				$( "#page-container-tabs-prg" ).tabs('destroy');
				$( "#page-container-tabs-prg" ).tabs({
					selected: activeLiLast-1
				});
				var str = $('#program_attach_tabs .ui-state-active').attr('id');
				var currmod = str.split('-');
				moduleTabclick(currmod[2]);
				
				// Add Drupal behaviours to the neely added tab
				$('#page-container-tabs-prg .infinite-loader-container input').removeClass('ajax-processed');
				$.extend(true, Drupal.settings.ajax, element_settings);
				$.each(element_settings,function(k, v){
					$('#'+k).removeClass('ajax-processed');
					Drupal.attachBehaviors('#'+k,v);
				});
				// Reset total module count
				$("#total-modules").val(parseInt($("#total-modules").val())+1);
			}else if(type == 'attachgrp'){ // Search courses using the text search box
				$(selector).html(data)
				$.each(element_settings,function(k, v){
					$('#'+k).removeClass('ajax-processed');
					$('#'+k).unbind('click');
					Drupal.detachBehaviors('#'+k,v);
					Drupal.attachBehaviors('#'+k,v);
				});
			}else if(type == 'show_more'){ // List courses by clicking show more button
				data = $.trim(data.replace(/[\t\n]+/g,' '));
				var selectarr = selector.split('-'); // Get Module Id
				var currmid = selectarr[1];
				if($(data).find('.drag-grp-head').length > 0){ 
					$(data).find('.drag-grp-head').each(function(){ // Loop groups
						var id = $(this).attr('id');
					  	if($('#module-course-list-'+currmid+' #dragndrop-'+currmid+' #'+id).length > 0){
					  		// If the group already placed in screen then update only the new courses in the existing groups
					  		$('#module-course-list-'+currmid+' #dragndrop-'+currmid+' #'+id+' ul .drag-crs-head:last').after($(this).find('ul').html());
					    }else{
					    	// If it is a new group update the group at the end of existing group list
					    	$('#module-course-list-'+currmid+' #dragndrop-'+currmid+' .drag-grp-head:last').after($(this)[0].outerHTML);
					    }
					});
				}else{
					// If there is no group associated to the module
					var append = $(data+ " #"+selector).html();
					$("#"+selector).append(append);
				}
				
				$.each(element_settings,function(k, v){
					$('#'+k).removeClass('ajax-processed');
					Drupal.detachBehaviors('#'+k,v);
					Drupal.attachBehaviors('#'+k,v);
				});
			}else if(type == 'show_more_scroll'){
				// Update the show more button with the current state
				$("#"+selector).html(data);
			}
			
			changeArrowStatus();
			add_module_toggle();
		}catch(e){
  			// To Do
  		}
	};
})(jQuery);
//}