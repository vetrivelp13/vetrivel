function editSurveyListDetails(surveyId, surveyQuestionId){
	try{
	var questionVisibility = $('#add-new-question').is(':visible');
	/*
	$('.question-wrapper-lst').hide();	
	$('.edit-class-list').css('background-color','#FFFFFF');
	//$('.edit-survey-list').find('.admin-add-button-container').css('display','block');	
	$("#catalog-survey-question-addedit-form-details .admin-add-button-container").css("display","block");
	
	if(surveyQuestionId>0) {
		$('#add-new-question').hide();
		$('#edit-class-wrapper-'+surveyQuestionId).find('#question-wrapper-container').show();
		$('#edit-survey-list-'+surveyQuestionId).css('background','#d7ecf9');
	} else {
		if(questionVisibility == false) {
			$('#add-new-question').show();
		} else {
			$('#add-new-question').hide();				
		}

		if($('#add-edit-class-norecords')) {
			if($('#add-new-question').is(':visible')) {
				$('#add-edit-class-norecords').hide();
			} else {
				$('#add-edit-class-norecords').show();
			}			
		} 
	}
	
	//$('#edit-survey-list-'+surveyQuestionId).find('.admin-add-button-container').css('display','none');
	$('#edit-survey-list-'+surveyQuestionId+' .admin-add-button-container').css('display','none');
	
	if($('#edit-catalog-survey-assesment-basic-cancel')) {
		$('#edit-catalog-survey-assesment-basic-cancel').click();
	}
	$('.messages').remove();
	*/
	}catch(e){
		// To Do
	}
}

function editSurveyAssesment() {
	try{
	if($('#question-wrapper-container')) {
		$('.question-wrapper-lst').hide();	
		$('.edit-class-list').css('background-color','#FFFFFF');
		$('.edit-survey-list').find('.admin-add-button-container').css('display','block');
	}
	}catch(e){
		// To Do
	}
}

function remove_messages(){
	try{
	$('.messages').remove();
	}catch(e){
		// To Do
	}
}

function showSurvType() {
	try{
	//var survType = $("#edit-survey-type").val();
	var survType = $('#survey_type').val();
	//var survType = $('input[name="survey_type"]').val();
	//survey_type
	if(survType == 'sry_det_typ_ass') {
		/*$("#edit-min-marks").attr("disabled",false);
		$("#edit-max-marks").attr("disabled",false);
		*/
		//$("min-marks").attr("disabled",false);
		//$("max-marks").attr("disabled",false);
		// min_marks
		$("#min_max_mark_container").show();
		/*$('input[name="max_marks"]').removeClass("addedit-readonly-textfield");
		$('input[name="max_marks"]').attr("disabled",false);
		
		$('input[name="min_marks"]').removeClass("addedit-readonly-textfield");
		$('input[name="min_marks"]').attr("disabled",false);*/
		
	} else {
		/*$("#edit-min-marks").attr("disabled",true);
		$("#edit-max-marks").attr("disabled",true);
		*/
		//$("min-marks").attr("disabled",true);
		//$("max-marks").attr("disabled",true);
		$("#min_max_mark_container").hide();
		/*$('input[name="max_marks"]').addClass("addedit-readonly-textfield");
		$('input[name="max_marks"]').attr("disabled",true);
		//$('input[name="max_marks"]').val('');
		
		$('input[name="min_marks"]').addClass("addedit-readonly-textfield");
		$('input[name="min_marks"]').attr("disabled",true);*/
		//$('input[name="min_marks"]').val('');
	}
	}catch(e){
		// To Do
	}
}
function attachQuestionDelete(callId) {
	try{
		var wSize = 300;
		var html = '';
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
	    html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+Drupal.t("MSG772")+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	   	closeButt[Drupal.t('Yes')]= function(){$('#delete-msg-wizard').remove();$("#"+callId).click();};
    	var drupalTitle = Drupal.t("LBL286");
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:wSize,
	        resizable:false,
	        modal: true,
	        title:Drupal.t(drupalTitle),//"title",
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
	    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='unique-delete-class'></div>");

	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');

	    $("#delete-msg-wizard").show();

		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
	    	changeDialogPopUI();
	    }
	    if($('div.qtip-defaults').length > 0) {
	    	var prevZindex = $('.qtip-defaults').css('z-index');
	    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
	    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
	    }
	}catch(e){
		// To Do
	}
}
function onLoadChangeSurAssGreyout() {
	try{
	var ac_name = $('#surassattchedquestions-autocomplete').val();
	var sur_type = $('#hiddensurassgptype').val();
	$('.ac_results').css('display','none');
	if(sur_type){
	  $('#search_all_surass_type-hidden').val(sur_type);
	}  
	if(sur_type == 'surassgrpname'){
		$('#select-list-surass-dropdown').text(Drupal.t('Group'));
		$('#select-list-surass-dropdown').val('surassgrpname');		
	}
	if(sur_type == 'surassgrpname' && ac_name== Drupal.t('LBL036')+' '+Drupal.t('Group')){
	    $('#surassattchedquestions-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('Group'));	
	}
	if(sur_type == 'surassgrpname' && ac_name!= Drupal.t('LBL036')+' '+Drupal.t('Group')){
	    $('#surassattchedquestions-autocomplete_hidden').val(Drupal.t('LBL036')+' '+Drupal.t('Group'));	
	}else{
		$('#surassattchedquestions-autocomplete_hidden').val(Drupal.t('LBL324'));
	}
	if(ac_name!= Drupal.t('LBL324') && sur_type=='surassqus'){
		$('#surassattchedquestions-autocomplete').removeClass('input-field-grey');
	}else if(ac_name!= Drupal.t('LBL036')+' '+Drupal.t('Group') && sur_type=='surassgrpname'){
		$('#surassattchedquestions-autocomplete').removeClass('input-field-grey');
		
	}
	}catch(e){
		// To Do
	}
}
function getSurAssSearchType(){
	try{
	var gptype= $('#search_all_surass_type-hidden').val();
	$('#hiddensurassgptype').val(gptype);
	}catch(e){
		// To Do
	}
}

$(document).keypress(function(e) {
	try{
	var val = $('.edit-attachquestion-list').attr('class');
    if(val == 'edit-attachquestion-list' && e.which == 13) {
        return false;
    }
	}catch(e){
		// To Do
	}
});
