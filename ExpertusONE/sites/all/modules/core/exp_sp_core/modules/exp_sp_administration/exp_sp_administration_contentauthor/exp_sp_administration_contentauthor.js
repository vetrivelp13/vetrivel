var PRESENTATION_ACTION_PARAMS = {"PRESENTATION_SAVE_AND_PUBLISH" : "","PRESENTATION_UNPUBLISH":"","PRESENTATION_DELETE":"","PRESENTATION_PUBLISHED":""};
var VIDEO_ACTION_PARAMS = {"VIDEO_SAVE" : ""};
VIDEO_ACTION_PARAMS.VIDEO_SAVE = $("#VIDEO_SAVE").val();

function putCustomLabelForContentAuthorWrapper(isNew)
{
		//refresh the list when click on close icon
	//$(".page-administration-contentauthor-video #modalContent").css('top','191px');
		$(".popups-close .close").click(function(){
			if($(this).attr("refreshRequired") == 1)
			{
//				$("#root-admin").data("narrowsearch").refreshGrid();
			}
		}); 
		 var recordType = $("#recordtype").val();
		if(recordType == "new"){
			$("#edit-video").parent().addClass("uploadiconbgshow");
			$("#edit-retry").parent().addClass("retryUnChecked");
			$("#edit-show-solution").parent().addClass("ShowSolnUnChecked");
			$("#two-col-row-url_uploadvideo").find(".form-item-files-video").addClass("newTempRecord");
		    $("#two-col-row-url_uploadvideo").find(".addedit-twocol-firstcol .addedit-twocol-firstcol .addedit-firstcol-field-value #edit-url").attr("placeholder", Drupal.t('MSG891'));		
		}
		else{
			//ADDING CLASS FOR TEMP DATA ALLIGNMENT ISSUE
			$("#two-col-row-url_uploadvideo").find(".form-item-files-video").addClass("uploadiconbgshow");
			$("#two-col-row-url_uploadvideo").find(".form-item-files-video").addClass("oldTempRecord");
			$("#two-col-row-url_uploadvideo").css({"padding-bottom":"5px"});
		}
		
		
		
		var text1 = '<span class="customlabelfileuploadhelp upload-msg-style">'+Drupal.t('MSG807')+ '</span>';
		if($(".customlabelfileuploadhelp").size() == 0)
			$("#contentauthor-addedit-form-container").find(".addedit-new-field-value").find(".form-item-files-video").find("#edit-video").css({"color":"transparent","max-width": "65px"}).after(text1);
		
		
		if(recordType == "embed")
		{
		    $("#two-col-row-url_uploadvideo").find(".addedit-twocol-firstcol .addedit-twocol-firstcol .addedit-firstcol-field-value #edit-url").attr("placeholder", "Enter https URL for YouTube videos");
				disableFileField();
			
		}
		if(recordType == "upload")
		{
				disableURLField();
			
		}
		
		
		//Disable file field if url is having value
		$("#edit-url").keypress(function(event) {
			disableFileField();
		});
		$("#edit-url").keyup(function(event) {
			if(recordType != "new"){
				$("#recordtype").val("embed"); 
			}
			
			if($("#edit-url").val()== "" && recordType == "new"){
				$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-mandatory").show();
				$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field .addedit-mandatory").hide();
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-title").css("color","#000");
			
				
			$(".uploadiconbgshowFaded").unbind('.myclick');
			
			
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").removeClass('uploadiconbgshowFaded');
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").css("pointer-events","auto");
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").addClass('uploadiconbgshow');
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file .customlabelfileuploadhelp").css("color","#000");

			
			
			}
		});

		//detect paste then disable file field
		$("#edit-url").bind('paste', function() {
			disableFileField();
			$("#iframe_editor_wrapper_container").attr("videochanged","true");
		});
		
		
		
		//detect cut then enable file field when new record
		$("#edit-url").bind('cut', function() {
		
         if(recordType != "new"){
				$("#recordtype").val("embed"); 
			}
			
         else{
		  setTimeout(function(){
			if($("#edit-url").val()== "" && recordType == "new"){
				$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-mandatory").show();
				$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field .addedit-mandatory").hide();
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-title").css("color","#000");
			
				
			$(".uploadiconbgshowFaded").unbind('.myclick');
			
			
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").removeClass('uploadiconbgshowFaded');
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").css("pointer-events","auto");
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").addClass('uploadiconbgshow');
			$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file .customlabelfileuploadhelp").css("color","#000");

			
			
			}
		  },0);
         }
		});
		

		if($.trim($("#videofilenamedisplay").html()) != null && $.trim($("#videofilenamedisplay").html()) != "")
		{	
			$("#edit-video").hide();
			$(".customlabelfileuploadhelp").hide();
			var clickEvt = "onclick='showeditvideo();'";
			if($("#edit-video").attr("disabled") == true || $("#edit-video").attr("disabled") == "disabled" || $("#edit-catalog-course-save-unpublish").size() == 1 )
			{
				clickEvt = "";
				$("#edit-video").parent().append("<div id='tmpdata'>"+$("#videofilenamedisplay").html()+"&nbsp;<a class='disable-delete-link' href='javascript:void(0);' "+clickEvt+" >x</a></div>");

			}
			else
			$("#edit-video").parent().append("<div id='tmpdata'>"+$("#videofilenamedisplay").html()+"&nbsp;<a href='javascript:void(0);' "+clickEvt+" >x</a></div>");
				
			$("#edit-video").parent().addClass("uploadiconbghide");
			$(".edit-edit-btn-prerequest").removeClass("ajax-processed");
			$(".addedit-form-cancel-container-actions").show();
			
			//disable url field
			$("#edit-url").attr("disabled","disabled");		
			$("#edit-url").addClass("addedit-readonly-textfield");
			$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").hide();
			var file_name=$("#tmpdata .title-lengthy-text").html();
			$('#tmpdata').addClass('vtip');
			$('#tmpdata').attr('title', file_name);		
			
			
			//Retain fadeout if char > 30 otherwise remove it
			if($("#tmpdata .title-lengthy-text").html().length < 30)
			{
				$("#tmpdata").find(".fade-out-image").remove();
			}
			
		}
		if($("#edit-url").attr('value') != "" && $("#edit-catalog-course-save-unpublish").size() == 1)
		{
			
			$("#edit-url").attr("disabled","disabled");		
			$("#edit-url").addClass("addedit-readonly-textfield");
		//	$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").hide(); // needs to be commented for #0086292
		}
		
		
		
		if($("#edit-video").attr("type") != "text")
		{	   
		   	$("#edit-video").click(function(){
				$(this).val("");
			});
		}
			
		$("#edit-video").change(function(){
			
			var filename = $("#edit-video").val();
			if(filename != "" || filename != undefined)
			{
				filename = filename.replace("\\","/");
				filename = filename.replace("\\","/");
			
				filename = filename.split("/");
				$(".customlabelfileupload").show();
				$("#VIDEO_DELETE").val("FALSE");
				
				
				$(".customlabelfileupload").hide();
				$(".customlabelfileupload").next().remove();
				$(".customlabelfileuploadhelp").hide();
				
				var lengthytitle = '';
				if(filename[filename.length-1].length > 30)
					lengthytitle = '<div class="fade-out-title-container  video-file-title-fadeout-container"><span class="title-lengthy-text">'+filename[filename.length-1]+'</span><span class="fade-out-image"></span></div>';
				else
					lengthytitle = filename[filename.length-1];
					
			   var clickEvt = "onclick='showeditvideo();'";
				if($("#edit-video").attr("disabled") == true || $("#edit-video").attr("disabled") == "disabled")
					clickEvt = "";
				else
					clickEvt = "<a href='javascript:void(0);' "+clickEvt+" >x</a>";
				$("#edit-video").parent().append("<div id='tmpdata'>"+lengthytitle+"&nbsp;"+clickEvt+"</div>");
				$("#edit-video").css("display","none");
				$("#edit-video").parent().addClass("uploadiconbghide");
				$("#edit-video").parent().removeClass("uploadiconbgshow");
				var file_name=$("#tmpdata .title-lengthy-text").html();
				$('#tmpdata').addClass('vtip');
				$('#tmpdata').attr('title', file_name);	
				vtip();
				//disable url field
				$("#edit-url").attr("disabled","disabled");
				$("#edit-url").addClass("addedit-readonly-textfield");
				$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").hide();
			}
		});
		
	
		initRetryShowSolEvents();
		
		
		$('#retrycontainer').find(".addnewtitle_field_show_retry").find("img.info-enr-upload").attr('title', Drupal.t('MSG805'));
		$('#showsolutioncontainer').find(".addnewtitle_field_show_retry").find("img.info-enr-upload").attr('title', Drupal.t('MSG806'));
		vtip();		
		

		$("#edit-edit-btn-details").click(function(){	
			if($("#two-col-row-url_uploadvideo").find(".addedit-twocol-firstcol .addedit-twocol-firstcol .addedit-firstcol-field-value #edit-url").attr('value') != ""){
				 disableFileField();
			}
			else if($("#recordtype").val() == "upload")
				 disableURLField();
				
		});
	
	
		
	/*	 Adding for Esign support - start	
		var actionType =  "";
	//	if(recordType != "new"){
		if(recordType != "new"){
			actionType = "contentAuthorAddInteractions_edit";
		}
		else if(recordType == "new")
			actionType = "contentAuthorAddInteractions_new";
	  	  
		
		
	 	  if($('#iframe_editor_wrapper').attr("esigndisplayed") == "on")
			{
				//alert(12345678);
				$('#iframe_editor_wrapper').attr("esigndisplayed","off");
			}
			else if($(".h5p-editor-iframe").attr("esigndisplayed") == "on")
			{
				$('.h5p-editor-iframe').attr("esigndisplayed","off");
			}
		    if($("#exp-sp-administration-contentauthor-addedit-form").attr("esigndisplayed") == "on"){
		    	//alert("not inside the esign conditions");
		    	$('#exp-sp-administration-contentauthor-addedit-form').attr("esigndisplayed","off");
		    }
		    
		    	
			else
			{
				
				if(availableFunctionalities != undefined && availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "")
				{
					//alert("inside the esign conditions")
					//var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType,'paramForPublish':paramForPublish};
					var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType};
					window.parent.$.fn.getNewEsignPopup(esignObj);
					if(actionType == "contentAuthorAddInteractions_edit"){
	 				$('#h5p-editor-iframe').attr("esigndisplayed","on");
					}
	 			else if(actionType == "contentAuthorAddInteractions_new")
	 				$('#exp-sp-administration-contentauthor-addedit-form').attr("esigndisplayed","on");	 			
	 			
	         	return false;
				}
			}  */
	  	  
	
		
   /* Adding for Esign support - end*/
		
		
		/*if(recordType == "new")
			{
			
			}*/
		
		
		
		
		
		
	
    		
	 // if(recordType != "new")	
		clonedActionsForVideo();	
}

function hideUploadVideoControlAndDisplayLabelWrapper()
{
	//$(".page-administration-contentauthor-video #modalContent").css('top','191px');
	/*alert($("#VIDEO_SAVE").val());
	if($("#VIDEO_SAVE").val() == "TRUE"){
		alert("not the first time load");
	}*/
	
	/*$("#content_save_btn").click(function(event){	
		var recordType = $("#recordtype").val();
		
		var data = "";
		
		if(recordType != "new"){
		
			data = saveInteractionsH5PWrapper(this);
			if(data == false)
			{
				return false;
			}
			$("#INTERACTIONDATA").val(data);
		}
//		$("#iframe_editor_wrapper_container").show();
	});
	$("#content_save_pub_btn").click(function(){
		var data = ""; 
		if(recordType != "new"){	
			data = saveInteractionsH5PWrapper(this);
			if(data == false){
				return false;
			}
			$("#INTERACTIONDATA").val(data);
		}
//		$("#iframe_editor_wrapper_container").show();
	});
	*/
//	$("#iframe_editor_wrapper_container").show();
	
	var disableField = function() {
		if($("#two-col-row-url_uploadvideo").find(".addedit-twocol-firstcol .addedit-twocol-firstcol .addedit-firstcol-field-value #edit-url").attr('value') != ""){
			 disableFileField();
		}
		else if($("#recordtype").val() == "upload")
			disableURLField();
	}
	disableField();
	$("#content_save_btn").click(function(){	
		disableField();
	});
	
	
		if($("#h5purl1").size() == 0)
		{
			$("body").append("<div id='h5purl1' style='display:none;'>"+$("#h5purl").html()+"</div>");
			$("body").append("<div id='h5pjsoncontent' style='display:none;'>"+$("#jsoncontent").val()+"</div>");
		}
		else
		{
			$("#h5purl1").html($("#h5purl").html());
			$("#h5pjsoncontent").html($("#jsoncontent").val());
		}
		
		var text1 = '<span class="customlabelfileuploadhelp upload-msg-style">'+Drupal.t('MSG807')+ '</span>';
		if($("#tmpdata").size() == 0 && $(".customlabelfileuploadhelp").size() == 0)
		{
			$("#tmpdata").after(text1);
			$("#tmpdata").parent().find("br").remove();
		}
		else if($(".customlabelfileuploadhelp").size() == 0)
			$("#contentauthor-addedit-form-container").find(".addedit-new-field-value").find(".form-item-files-video").find("#edit-video").css({"color":"transparent","max-width": "65px"}).after(text1);
		
		$( ".edit-edit-btn-details" ).unbind( "click" );
		$(".edit-edit-btn-details").removeClass("ajax-processed");
		$( ".edit-edit-btn-details" ).click(function( event ) {
			  $( ".edit-edit-btn-details" ).parent().parent().parent().parent().find(".selected").removeClass("selected");
			  $( ".edit-edit-btn-details" ).addClass("selected");
			 
			 $("#h5pdetails").show();
			 $("#h5pdetails1").hide();
			 $(".addedit-form-cancel-and-save-actions-row").show();
			 $(".addedit-form-cancel-container-actions").show();
			 $("#iframe_editor_wrapper_container").hide();
			 
			 $("#contentauthor-addedit-form-container").css("height","auto");			 
			 //if any error msges
			 $('body').data('learningcore').closeErrorMessage();
			 return false;
		});
		$( ".edit-edit-btn-prerequest" ).unbind( "click" );
		$(".edit-edit-btn-prerequest").removeClass("ajax-processed");
		vtip();
		$( ".edit-edit-btn-prerequest" ).click(function( event ) {
			  loadLazyH5P();
			  var url = $("#h5purl1").html();
			  
			  $( ".edit-edit-btn-prerequest" ).parent().parent().parent().parent().find(".selected").removeClass("selected");
			  $( ".edit-edit-btn-prerequest" ).addClass("selected");
			  console.log("h5peditorloaded:"+$(".edit-edit-btn-prerequest").hasClass("h5peditorloaded"));
			  if(!$(".edit-edit-btn-prerequest").hasClass("h5peditorloaded"))
			  {
			  	if($(".h5p-editor-iframe").contents().find(".h5peditor-dragnbar").size() == 0)
					EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("exp-sp-administration-contentauthor-addedit-form");
			  }
			  else
			  {
			  	EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
				$("#iframe_editor_wrapper").show();
				$("#iframe_editor_wrapper").css("visibility","visible");
     			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
    			$("#iframe_editor_wrapper_container").show();
    			EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("contentauthor-addedit-form-container");
     			$("#iframe_editor").show();
    			$("#h5pe1loading").hide();
			  	
			  	
			  }
			  $("#h5pdetails").hide();
			  $("#h5pdetails1").hide();
			  //if any error msges
			 $('body').data('learningcore').closeErrorMessage();
			  $("#iframe_editor_wrapper_container").show();
			  $(".addedit-form-cancel-and-save-actions-row").hide();
			  $("#iframe_editor_wrapper #edit-actions").remove();
		//	 $("#contentauthor-addedit-form-container").css("height","463px");
			  
		//	  $("#iframe_editor_wrapper").append('<div class="form-actions form-wrapper" id="edit-actions"><input type="button"  onclick="closeInteractionPopup(this)" name="op" value="'+Drupal.t("LBL123")+'" class="form-submit"></input>&nbsp;&nbsp;<input type="button"  id="saveInteractionsH5P" onclick="saveInteractionsH5PWrapper(this)" name="op" value="'+Drupal.t("LBL569")+'" class="form-submit"></input></div>');
			
			 
			 event.stopImmediatePropagation();
			  event.stopPropagation();
			  event.preventDefault();
			  return false;
		});
		
		$( ".edit-edit-btn-equivalence" ).unbind( "click" );
		$(".edit-edit-btn-equivalence").removeClass("ajax-processed");
		$( ".edit-edit-btn-equivalence" ).click(function( event ) {
					 //if any error msges
			 $('body').data('learningcore').closeErrorMessage();
		
			  var url = $("#h5purl1").html();
			  url = url.replace("edit","view");
			  $( ".edit-edit-btn-equivalence" ).parent().parent().parent().parent().find(".selected").removeClass("selected");
			  $( ".edit-edit-btn-equivalence" ).addClass("selected");
		      $(".addedit-form-cancel-container-actions").hide();			  
			  var summarydefaultdata = JSON.parse(decodeURIComponent($("#h5pjsoncontent").html()));
			  summarydefaultdata.feedbacksummary = replaceAllStr(summarydefaultdata.feedbacksummary,"+"," ");
			  var passingscore = summarydefaultdata.passingscore;
			  
			  if(passingscore == null || passingscore == undefined || passingscore == 'undefined')
			  	passingscore = "";

			var data = "";
			var summariesStr = "";
			var displayAtValue = "";
			displayAtValue = summarydefaultdata.displayAt;
			if(displayAtValue == "" || displayAtValue == undefined)
				displayAtValue = "3";
			if(summarydefaultdata.summaries != null && summarydefaultdata.summaries != "")
			{
				
				for(var i = 0; i < summarydefaultdata.summaries.length; i++)
					if(summariesStr == "")
						summariesStr = summarydefaultdata.summaries[i].summary;
					else
						summariesStr = summariesStr+"\n"+summarydefaultdata.summaries[i].summary;
			}
			if(summariesStr == undefined )
				summariesStr = "";
			else
				summariesStr = replaceAllStr(summariesStr.toString(),"+"," ");
			
			var htmlData = "";
			htmlData += '<div class="addedit-new-field">';
			htmlData += '<div class="addedit-new-field-title">Summary:</div>';
			
			htmlData += '<div class="addedit-new-field-value">';
			htmlData += '<div class="form-item form-type-textfield form-item-title">';
 			htmlData += '<textarea autocomplete="off" onkeypress="summaryMandatoryFields()" class="addedit-edit-summary addedit-edit-textarea form-textarea adjust-modalbackdrop-onresize" data="summary" id="edit-summary" name="summary[value]" cols="40" rows=7" style="display: block;" aria-hidden="true">'+summariesStr+'</textarea>';
 			htmlData += '</div>';

 			htmlData += "<div style='width:420px;height:30px;color:#999'>Write each statement separate by comma.Use an empty line to separate sets of statements.</div>";

			htmlData += '</div><div class="clearBoth"></div></div>';
			


			htmlData += '<div class="addedit-new-field">';
			htmlData += '<div class="addedit-new-field-title">Display At:<span class="addedit-mandatory summarymandatoryfields" style="display:none;">*</span></div>';
			htmlData += '<div class="addedit-new-field-value">';
			htmlData += '<div class="form-item form-type-textfield form-item-title">';
 			htmlData += '<input autocomplete="off" class="addedit-edit-displayat addedit-edit-textfield form-text" tabindex="1" type="text" id="edit-displayat" name="displayat" value="'+displayAtValue+'" size="60" maxlength="100">';
 			htmlData += '</div>';
 			htmlData += "<div style='width:420px;height:30px;color:#999'>Display summary before number of seconds end this video.</div>";
 			
			htmlData += '</div><div class="clearBoth"></div></div>';
			


			var feedbackSummaryDefaultValue = "You got @score of @total statements (@percent %) correct on your first try.";	
			if(summarydefaultdata.feedbacksummary != null && summarydefaultdata.feedbacksummary != "")
					feedbackSummaryDefaultValue = summarydefaultdata.feedbacksummary;
			htmlData += '<div class="addedit-new-field" style="display:none;">';
			htmlData += '<div class="addedit-new-field-title">Feedback summary:<span class="addedit-mandatory">*</span></div>';
			htmlData += '<div class="addedit-new-field-value">';
			htmlData += '<div class="form-item form-type-textfield form-item-title">';
 			htmlData += '<input autocomplete="off" class="addedit-edit-summary addedit-edit-textfield form-text" tabindex="1" type="text" id="edit-summaryfeedback" name="summaryfeedback" value="'+feedbackSummaryDefaultValue+'" size="60" maxlength="100">';
 			htmlData += '</div>';
 			htmlData += "<div style='width:400px;height:30px;color:#999'>Available variables: @score, @total, @percent. Example: You got @score of @total statements (@percent %) correct.</div>";

			htmlData += '</div><div class="clearBoth"></div></div>';
			
			htmlData += '<div class="addedit-new-field">';
			htmlData += '<div class="addedit-new-field-title">Passing Score (%):</div>'; //<span class="addedit-mandatory" >*</span>
			htmlData += '<div class="addedit-new-field-value">';
			htmlData += '<div class="form-item form-type-textfield form-item-title">';
 			htmlData += '<input autocomplete="off" class="addedit-edit-passingscore  form-text" tabindex="1" type="text" id="edit-passingscore" name="passingscore" value="'+passingscore+'" size="5" maxlength="5">';
 			htmlData += '</div>';
 			htmlData += "<div style='width:400px;height:30px;color:#999'></div>";

			htmlData += '</div><div class="clearBoth"></div></div>';

			
			htmlData += '<div class="addedit-form-cancel-and-save-actions-row">';
  
   			htmlData +='<div class="addedit-form-cancel-container-actions">';
     		htmlData +=' <div class="white-btn-bg-left"></div>';
      		htmlData +='<input class="addedit-edit-contentauthor-basic-cancel admin-action-button-middle-bg addedit-form-expertusone-throbber exp-addedit-form-cancel-button white-btn-bg-middle form-submit" data-wrapperid="contentauthor-addedit-form" onclick="Drupal.CTools.Modal.dismiss(); return false;" tabindex="6" type="submit" id="edit-contentauthor-basic-cancel" name="Close" value="Close"/>';
      		htmlData +='<div class="white-btn-bg-right"></div>';

			htmlData += '<div class="admin-save-pub-unpub-button-container">';
			htmlData += '<div class="admin-save-button-left-bg"></div>';
			htmlData += '<input class="addedit-edit-newtheme-save-button addedit-form-expertusone-throbber admin-save-button-middle-bg  form-submit ajax-processed" tabindex="12" data-wrapperid="program-addedit-form" type="button" onClick="saveH5PSummaryWrapper();" id="newtheme-save-button" name="save" value="Save">';
			htmlData += '<div class="admin-save-button-right-bg"></div>';
			htmlData += '</div>'; 



      		htmlData +='<div class="clearBoth"></div>';
   			htmlData +='</div>';
   			htmlData +='<div class="clearBoth"></div>';
			htmlData +='</div>';


			$("#h5pdetails").hide();
			$("#h5pdetails1").html(htmlData);
			$("#h5pdetails1").show();
			$("#iframe_editor_wrapper_container").hide();

			  event.stopImmediatePropagation();
			  event.stopPropagation();
			  event.preventDefault();
			  return false;
		});
		
		
		if($(".video-file-title-fadeout-container").size() > 0)
		{
			$(".customlabelfileuploadhelp").hide();
		}
		else
		$(".customlabelfileuploadhelp").show();
		
		initRetryShowSolEvents();
		
		
		
		//if there are no error messages
		var recordType = $("#recordtype").val();
		var vid_stream = $("#stream").val();
		if(recordType == "newlycreated")
		{
	
			$( ".edit-edit-btn-prerequest" ).click();
			$( "#contentauthor-addedit-form-container" ).find("#h5pdetails").css("");
			$( "#contentauthor-addedit-form-container" ).find("#h5pdetails").css({"display":"block"});
			$( "#contentauthor-addedit-form-container" ).find(".addedit-form-cancel-and-save-actions-row").css({"display":"block"});
			$("#recordtype").val("");
			
			var status = "";
			if(vid_stream == "true")
				status = Drupal.t('MSG700');
			else if(vid_stream == "false")
				status = Drupal.t('LBL272') + ' ' + Drupal.t('MSG600') +  '.' ;
			
			var htm ="";
			htm += '<div id="message-container" >';
			htm +=  '<div class="messages status">';
			htm +=   '<ul>';
			htm +=    '<li class=""><span>'+status+'</span></li>';
			htm +=   '</ul>';
			htm +=  '<div class="msg-close-btn" onclick="h5pCloseErrorMessage()"></div>';
			htm += '</div>';
			htm += '</div>';
			
			
			$("#show_expertus_message").find('img').before(htm);
			
		//	$("#show_expertus_message").html(Drupal.t('MSG700'));
		//	$( "#contentauthor-addedit-form-container" ).find(".addedit-form-cancel-and-save-actions-row").css({"display":"none"});
		}
		else if($("#show_expertus_message").html() == "")
		{
			var recordType = $("#recordtype").val();
	//		$( "#contentauthor-addedit-form-container" ).find(".addedit-form-cancel-and-save-actions-row").first().addClass("oldScreen123");
			$( "#contentauthor-addedit-form-container" ).find(".addedit-form-cancel-and-save-actions-row").css({"display":"none"});
			loadLazyH5P();
			
		//	$( "#contentauthor-addedit-form-container" ).find(".addedit-form-cancel-and-save-actions-row").css({"display":"none"});
		}
		else if($("#iframe_editor_wrapper_container").attr("videochanged") =="true" && $("#show_expertus_message").find(".status").size() > 0) //if there is no error
		{
			loadLazyH5P();
			$("#iframe_editor_wrapper_container").show();
		}
		//83771
		var pScore = $("#edit-passingscore").val().trim();
		if(isNaN(pScore)){			
			pScore=$("#edit-passingscore").val();
		}else{
			if(pScore!='')
			var pScore = parseFloat(pScore,10);
			else
				pScore='';
		}
		
		$("#edit-passingscore").val(pScore);
		  	
	
}

function putCustomLabelForContentAuthorWrapperOld()
{	
		//ADDING FOR THE OR ICON
		var orIcon = '<span class="orIcon" style="margin-left:12px"><img src="/sites/all/themes/core/expertusoneV2/expertusone-internals/images/or_icon.png"></span>';
		$("#two-col-row-url_uploadvideo").find(".addedit-twocol-secondcol").before(orIcon);
		
		
	//For Show Solution	
		if ($('#edit-show-solution-disabled').is(":checked"))
		{
			$('#edit-show-solution-disabled').parent().addClass("DisableIsSelected");
		}
		if ($('#edit-show-solution-enabled').is(":checked"))
		{
			$('#edit-show-solution-enabled').parent().addClass("EnableIsSelected");	
		}	
		$(".form-item-show-solution input").live('click', function(){
			var id = $(this).attr("id");
			if(id == "edit-show-solution-disabled"){
			 $(this).closest( ".form-item-show-solution" ).addClass("DisableIsSelected");
				 $(this).parent().siblings( ".form-item-show-solution" ).removeClass("EnableIsSelected"); 
			}
			else{
			$(this).closest( ".form-item-show-solution" ).addClass("EnableIsSelected");	
			$(this).parent().siblings( ".form-item-show-solution" ).removeClass("DisableIsSelected"); 
			}
		}); 
		
	// For Retry
		if ($('#edit-retry-disabled').is(":checked"))
		{
			$('#edit-retry-disabled').parent().addClass("DisableRetryIsSelected");
		}
		if ($('#edit-retry-enabled').is(":checked"))
		{
			$('#edit-retry-enabled').parent().addClass("EnableRetryIsSelected");	
		}
		$(".form-item-retry input").live('click', function(){
			var id = $(this).attr("id");
			if(id == "edit-retry-disabled"){
			 $(this).closest( ".form-item-retry" ).addClass("DisableRetryIsSelected");
			 $(this).parent().siblings( ".form-item-retry" ).removeClass("EnableRetryIsSelected"); 
			}
			else{
			$(this).closest( ".form-item-retry" ).addClass("EnableRetryIsSelected");	
			$(this).parent().siblings( ".form-item-retry" ).removeClass("DisableRetryIsSelected"); 
			}
		}); 
		
		
		
		
		
		
		var text1 = '<br/><span class="customlabelfileuploadhelp upload-msg-style">'+Drupal.t('MSG807')+ '</span>';
		if($("#tmpdata").size() == 0  && $(".customlabelfileuploadhelp").size()  == 0)
		{
			$("#tmpdata").after(text1);
			$("#tmpdata").parent().find("br").remove();
		}
		else if( $(".customlabelfileuploadhelp").size()  == 0)
			$("#contentauthor-addedit-form-container").find(".addedit-new-field-value").find(".form-item-files-video").find("#edit-video").css({"color":"transparent","max-width": "65px"}).after(text1);
	   	
	   	
		var text2 = '<span class="customlabelvideoembed embed-msg-style" style="float:left;clear:both;">('+Drupal.t('MSG781')+ ')</span>';
		$("#contentauthor-addedit-form-container").find(".addedit-new-field-value").find(".form-item-video").find("#edit-video").css({"color":""}).after(text2);
	
	


		
		var text1 = '<span class="customlabelimageuploadhelp upload-msg-style">'+Drupal.t('MSG779')+ '</span>';
	   	if($("#tmpdata1").size() ==0 && $(".customlabelimageuploadhelp").size() == 0)
	   		$("#edit-poster-image").css({"color":"transparent","max-width": "65px"}).after(text1);
	   	else if($(".customlabelimageuploadhelp").size() == 0)
	   		$("#tmpdata1").after(text1);
	   		
	   	if($("#edit-poster-image").css("display") == "none")
	   	{
	   		$(".customlabelimageuploadhelp").hide();
	   	}
		if($("#edit-video").attr("type") != "text")
		{	   
		   	$("#edit-video").click(function(){
				$(this).val("");
			});
			
		$("#edit-video").change(function(){
			var filename = $("#edit-video").val();
			if(filename != "" || filename != undefined)
			{
				filename = filename.replace("\\","/");
				filename = filename.replace("\\","/");
			
				filename = filename.split("/");
				$(".customlabelfileupload").show();
				$("#VIDEO_DELETE").val("FALSE");
				
				
				$(".customlabelfileupload").hide();
				$(".customlabelfileupload").next().remove();
				$(".customlabelfileuploadhelp").hide();
				
				var lengthytitle = '';
				if(filename[filename.length-1].length > 30)
					lengthytitle = '<div class="fade-out-title-container  video-file-title-fadeout-container"><span class="title-lengthy-text">'+filename[filename.length-1]+'</span><span class="fade-out-image"></span></div>';
				else
					lengthytitle = filename[filename.length-1];
					
			   var clickEvt = "onclick='showeditvideo();'";
				if($("#edit-video").attr("disabled") == true || $("#edit-video").attr("disabled") == "disabled" )
					clickEvt = "";
				else
					clickEvt = "<a href='javascript:void(0);' "+clickEvt+" >x</a>";
				$("#edit-video").parent().append("<div id='tmpdata'>"+lengthytitle+"&nbsp;"+clickEvt+"</div>");
				$("#edit-video").css("display","none");
				$("#edit-video").parent().addClass("uploadiconbghide");
				$("#edit-video").parent().removeClass("uploadiconbgshow");
				var file_name=$("#tmpdata .title-lengthy-text").html();
				$('#tmpdata').addClass('vtip');
				$('#tmpdata').attr('title', file_name);	
				vtip();
			}
		});
		}
		$("#edit-poster-image").click(function(){
			$(this).val("");
		});
		
		$("#edit-poster-image").change(function(){
			var filename = $("#edit-poster-image").val();
			if(filename != "" || filename != undefined)
			{
				filename = filename.replace("\\","/");
				filename = filename.replace("\\","/");
			
				filename = filename.split("/");
				$(".customlabelimageupload").show();
				$(".customlabelimageupload").html(filename[filename.length-1]);
				$("#POSTER_DELETE").val("FALSE");
				
				$(".customlabelimageupload").hide();
				$(".customlabelimageuploadhelp").hide();
				
				var lengthytitle  = '';
				if(filename[filename.length-1].length > 30)
					lengthytitle = '<div class="fade-out-title-container  poster-image-title-fadeout-container"><span class="title-lengthy-text">'+filename[filename.length-1]+'</span><span class="fade-out-image"></span></div>';
				else
					lengthytitle = 		filename[filename.length-1];
					
			   var clickEvt = "onclick='showeditposter();'";
				$("#edit-poster-image").parent().append("<div id='tmpdata1'>"+lengthytitle+"&nbsp;<a href='javascript:void(0);' "+clickEvt+" >x</a></div>");
				$("#edit-poster-image").css("display","none");
				$("#edit-poster-image").parent().addClass("uploadiconbghide");
				$("#edit-poster-image").parent().removeClass("uploadiconbgshow");
				
			}
		});
			
		if($.trim($("#posterfilenamedisplay").html())  == "")
		{
			$("#edit-poster-image").parent().removeClass("uploadiconbghide");
			$("#edit-poster-image").parent().addClass("uploadiconbgshow").css("width","200px");
			$("<span class='customlabelimageupload'>No file chosen</span>").insertAfter("#edit-poster-image");
			
		}
		if($.trim($("#videofilenamedisplay").html()) == "")
		{
			$("#edit-video").parent().removeClass("uploadiconbghide");
			$("#edit-video").parent().addClass("uploadiconbgshow");
			
		}
		
			showHelpIconsForContentAuthor();
			vtip();		
}



function showeditvideo()
{
	
 //if($(".interactionsexists").size() > 0 && $("#INTERACTIONDATA").attr("value") != "FALSE") // Added for 0084022 : Until we click save, even though interactions are present, 'X' should not fade out.
// 	return ""; //do not remove video and do not provide option to attach new video
 
 //else
	 if($(".interactionsexists").size() > 0 ){
	removeInteractionMessage('','deleteVideo');
	return "";
 }

  $("#tmpdata").remove();
 $("#edit-video").show();
 $("#edit-video").parent().removeClass("uploadiconbghide");
 $("#edit-video").parent().addClass("uploadiconbgshow");
 $("#edit-video").parent().css("width","200px");
$("#edit-video").removeAttr("disabled");


 $(".customlabelfileuploadhelp").html(Drupal.t('MSG807'));
 $(".customlabelfileuploadhelp").show();
 $(".customlabelfileupload").show();
 $("#VIDEO_DELETE").val("TRUE");
 $("#videofilenamedisplay").html("");
 var recordType = $("#recordtype").val();

 

 $("#iframe_editor_wrapper_container").attr("videochanged","true");
 $("#iframe_editor_wrapper_container").hide();

 //Enable url field
 if(recordType == "new")
 {
	$("#edit-video").val("");
 	$("#edit-url").attr("disabled","");		
 	$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").show();
	$("#edit-url").removeClass("addedit-readonly-textfield");
 }
 else{
	 
	 $("#recordtype").val("upload"); 
	 $("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").hide(); 
 }

 
}

//Content Author changes
function showcreatemenu_contentauthor()
{
	$("#createmenu_contentauthor").show();
}





function disableFileField()
{
	//alert("in func");
	$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-mandatory").hide();
	$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field .addedit-mandatory").show();
	$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").removeClass('uploadiconbgshow');
	$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").addClass('uploadiconbgshowFaded');
	$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file").css("pointer-events","none");
	
	if( $(".uploadiconbgshowFaded").size() != 0 )
    {
		$(".uploadiconbgshowFaded").bind("click.myclick", function(event) {
			  event.preventDefault();
			});
    }
	
	$("#two-col-row-url_uploadvideo .addedit-twocol-secondcol .addedit-new-field .addedit-new-field-value .form-type-file .customlabelfileuploadhelp").css("color","#ddd");

}

function disableURLField()
{

	$("#edit-url").attr("disabled","disabled");		
	$("#edit-url").addClass("addedit-readonly-textfield");
	$("#two-col-row-url_uploadvideo .addedit-twocol-firstcol .addedit-new-field-title .addedit-mandatory").hide();
	var file_name=$("#tmpdata .title-lengthy-text").html();
	$('#tmpdata').addClass('vtip');
	$('#tmpdata').attr('title', file_name);		
}



function loadLazyH5P()
{
	//$(".addedit-form-cancel-and-save-actions-row").show();
var url = $("#h5purl1").html();
if($("#iframe_editor_wrapper").size()  == 0 || $("#iframe_editor_wrapper_container").attr("videochanged") =="true")
{
	$("#iframe_editor_wrapper_container").remove();
   // var ifrm = '<div id="iframe_editor_wrapper_container" style="display:none;"><div id="iframe_editor_wrapper" style="visibility:hidden;" ><div class="h5p-editor" id="iframe_editor" style="border:0px;display:none;" src="'+url+'" width="600px" height="520px"></div><div id="h5pe1loading" style="height:300px;display:block;"></div></div></div>';
   // var ifrm = '<div id="iframe_editor_wrapper_container" style="height:1px !important;"><div id="iframe_editor_wrapper" style="visibility:hidden;" ><div class="h5p-editor" id="iframe_editor" style="border:0px;display:none;" src="'+url+'" width="600px" height="520px"></div><div id="h5pe1loading" style="height:300px;display:block;"></div></div></div>';
   var ifrm = '<div id="iframe_editor_wrapper_container" style="height:1px;"><div id="iframe_editor_wrapper" style="visibility:hidden;" ><div class="h5p-editor" id="iframe_editor" style="border:0px;display:none;" src="'+url+'" width="600px" height="520px"></div><div id="h5pe1loading" style="height:300px;display:block;"></div></div></div>';

    ifrm += '<div id="h5pdetails1" style="display:none"></div>';
// $("#contentauthor-addedit-form-container").append(ifrm);
// $("#contentauthor-addedit-form #h5pdetails").after(ifrm);
$("#contentauthor-addedit-form ").after(ifrm);

ns.init();
if(!window.parent.parent.$(".edit-edit-btn-details").hasClass("selected"))
if(!$(".edit-edit-btn-prerequest").hasClass("h5peditorloaded"))
EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("exp-sp-administration-contentauthor-addedit-form");

} 


}



function bindPresentationCloseAction(id,actionexecuted,h5pId)
{
		/*if(actionexecuted != null && actionexecuted == 1) 
			$("#root-admin").data("narrowsearch").refreshGrid();
			*/
		
		
		if(id == null || id == undefined || id == "")  //record not created
		{
				//delete the h5p node record which is not mapped with content master
				//alert("delete:"+h5pId);
				$.ajax({
				url:'?q=ajax/administration/contentauthor/presentation/presentationCleanup',
				type:'POST',
				data:'h5p_id='+h5pId,
				success:function(data)
				{
					//alert(data);
				},
				failure:function(err)
				{
				},
				error:function(err)
				{
				}
			});
		}
}
function initializePresentationObjectsWrapper()
	{
	$(".popups-close .close").click(function(){
	 	bindPresentationCloseAction($(this).attr("entityId"),$(this).attr("actionexecuted"),$(this).attr("h5pId"));
	});
	if(document.getElementById("entityId") != null)
		$(".popups-close .close").attr("entityId",document.getElementById("entityId").value);
	if(document.getElementById("h5pId") != null)
		$(".popups-close .close").attr("h5pId",document.getElementById("h5pId").value);
		
	 // ADDING FOR NEW PRESENTATION UI 
		$("#retry").click(function(){
			if($("#retry:checked").size() == 1){
				$("#retry").val("Enabled");
				$("#retry").parent().addClass("retryChecked");
				$("#retry").parent().removeClass("retryUnChecked");
			}
			else{
				$("#retry").val("Disabled");
				$("#retry").parent().removeClass("retryChecked");
				$("#retry").parent().addClass("retryUnChecked");
				
			}
		});
		
		$("#show_solution").click(function(){
			if($("#show_solution:checked").size() == 1){
				$("#show_solution").val("Enabled");
				$("#show_solution").parent().addClass("ShowSolnChecked");
				$("#show_solution").parent().removeClass("ShowSolnUnChecked");
			}
			else{
				$("#show_solution").val("Disabled");
				$("#show_solution").parent().removeClass("ShowSolnChecked");
				$("#show_solution").parent().addClass("ShowSolnUnChecked");
			}
		});
		
		
		
	
	$("#exp-sp-administration-contentauthor-presentation-addedit-form").find(".clearBoth").css("clear","none");
	
	
	
	PRESENTATION_ACTION_PARAMS.PRESENTATION_SAVE_AND_PUBLISH = $("#PRESENTATION_SAVE_AND_PUBLISH").val();
	PRESENTATION_ACTION_PARAMS.PRESENTATION_UNPUBLISH		 = $("#PRESENTATION_UNPUBLISH").val();
	PRESENTATION_ACTION_PARAMS.PRESENTATION_DELETE			 = $("#PRESENTATION_DELETE").val();
	PRESENTATION_ACTION_PARAMS.SLT_CONTENT_MASTER_ID 		 = $("#entityId").val();
	PRESENTATION_ACTION_PARAMS.PRESENTATION_PUBLISHED        = $("#PRESENTATION_PUBLISHED").val();
	PRESENTATION_ACTION_PARAMS.PRESENTATION_NAME             = $("#edit-title").prop("value");
	$(".addedit-form-cancel-container-actions").hide();
	$(".addedit-edit-contentauthor-basic-save").removeClass("ajax-processed");
	$(".addedit-edit-contentauthor-basic-save").removeClass("form-submit");
	$(".addedit-edit-contentauthor-basic-save").removeClass("addedit-form-expertusone-throbber");
	$(".addedit-form-cancel-container-actions").html("");
   var htm = new EJS({url: '/sites/all/modules/core/exp_sp_core/modules/exp_sp_administration/exp_sp_administration_contentauthor/presentation_actions.ejs?ts='+new Date().getTime()}).render(PRESENTATION_ACTION_PARAMS);
 
	
	$(".addedit-form-cancel-container-actions").append(htm);
	
	
	

	
	$(document).ready(function(){
	$(".addedit-form-cancel-container-actions").css("margin-right","10px");
	ns.init();
	
//	$(".h5p-background-selector").find(".h5p-bg-selector-content").find(".h5p-color-selector").addClass("heloooo");
//alert(111);
	});
	
	vtip();
}




function initRetryShowSolEvents()
{
	
		$("#edit-retry").click(function(){
			if($("#edit-retry:checked").size() == 1){
				$("#edit-retry").val("Enabled");
				$("#edit-retry").parent().parent().addClass("retryChecked");
				$("#edit-retry").parent().parent().removeClass("retryUnChecked");
			}
			else{
				$("#edit-retry").val("Disabled");
				$("#edit-retry").parent().parent().removeClass("retryChecked");
				$("#edit-retry").parent().parent().addClass("retryUnChecked");
				
			}
		});
$("#edit-show-solution").click(function(){
			if($("#edit-show-solution:checked").size() == 1){
				$("#edit-show-solution").val("Enabled");
				$("#edit-show-solution").parent().parent().addClass("ShowSolnChecked");
				$("#edit-show-solution").parent().parent().removeClass("ShowSolnUnChecked");
			}
			else{
				$("#edit-show-solution").val("Disabled");
				$("#edit-show-solution").parent().parent().removeClass("ShowSolnChecked");
				$("#edit-show-solution").parent().parent().addClass("ShowSolnUnChecked");
			}
		});
}



function presentationClose()
{
	//Drupal.CTools.Modal.dismiss();
	//$('#root-admin').data('narrowsearch').refreshGrid(); return false;
	$(".popups-close .close").click();
}


function launchAdminH5PPreview(id,ctype,token)
{
	window.h5ptincantoken = token;
	var iframe = "<iframe id='iframedata' src='"+id+"' style=' width:100%; height:100%; border:none; margin:0; padding:0; overflow:hidden;;' width='100%' height='100%'></iframe>";
	if(ctype == "lrn_cnt_typ_knc")
		$("#root-admin").data("narrowsearch").displayH5PPreview(iframe,"cre_ste_mod_aut", "exp_sp_administration_contentauthor",1000,520);
	else
		$("#root-admin").data("narrowsearch").displayH5PPreview(iframe,"cre_ste_mod_aut", "exp_sp_administration_contentauthor",1000,630);
	
}


//for parent listing page. Preload preview related files so that preview will be faster.
$(document).ready(function(){
	
	preLoadH5PFiles();


});


function videoOnClose()
{
	$(".popups-close .close").attr("refreshRequired","1");
}

(function($) 
{
$.fn.videoOnClose = function()
	{
		videoOnClose();
	}
})(jQuery);


function clonedActionsForVideo()
{
	var esign_status = "";
	$(".addedit-form-cancel-and-save-actions-row").hide();
	$(".clonedactions").remove();
	var clonedActions = $(".addedit-form-cancel-and-save-actions-row").clone().addClass("clonedactions");
	
	var recordType = $("#recordtype").val();
	if(recordType == "new")
		{
		 var ifrm = '<div id="iframe_editor_wrapper_container" style="height:1px;"></div>';
		$("#contentauthor-addedit-form ").after(ifrm);
		}
	
	
	
	$("#iframe_editor_wrapper_container").after(clonedActions);
	$(".clonedactions").show();
	$(".clonedactions").find("#content_save_btn").attr("id","content_save_btn_cloned");
	$("#content_save_btn_cloned").removeClass("form-submit");
	$("#content_save_btn_cloned").removeClass("ajax-processed");
	$("#content_save_btn_cloned").bind("click",function(){
		
	  
		
		var recordType = $("#recordtype").val();
		var data = "";
		
		
		if(recordType == "new")
			{
			esign_status =  create_edit_esign('new');
			}	
		else{
		esign_status =  create_edit_esign('edit');
		}
		if(esign_status == 0)
			return false;
		
		
		if(recordType != "new"){
			data = saveInteractionsH5PWrapper(this);
			if(data == false)
			{
				return false;
			}
			$("#INTERACTIONDATA").val(data);
			
			/*if($(".interactionsexists").size() == 0){
				alert("testabc");
				  $("#INTERACTIONDATA").attr("value","FALSE");
			}*/
		}
		$("#content_save_btn").click();
	});
	 
	//save and publish 
	$(".clonedactions").find("#content_save_pub_btn").attr("id","content_save_pub_btn_cloned");
	$("#content_save_pub_btn_cloned").removeClass("form-submit");
	$("#content_save_pub_btn_cloned").removeClass("ajax-processed");
	$("#content_save_pub_btn_cloned").bind("click",function(){
			var recordType = $("#recordtype").val();
			var data = "";

	
			if(recordType == "new")
		{
		 esign_status =  create_edit_esign('new_publish');
		}	
		else{	
		esign_status =  create_edit_esign('edit_publish');
		}
		if(esign_status == 0)
		 return false;
			
			
			
		if(recordType != "new"){
			data = saveInteractionsH5PWrapper(this);
			if(data == false)
			{
				return false;
			}
			$("#INTERACTIONDATA").val(data);
			
			/*if($(".interactionsexists").size() == 0)
				  $("#INTERACTIONDATA").attr("value","FALSE");*/
		}
		$("#content_save_pub_btn").click();
	}); 

	//UnPublish 
	$(".clonedactions").find("#edit-catalog-course-save-unpublish").attr("id","edit-catalog-course-save-unpublish_cloned");
	$("#edit-catalog-course-save-unpublish_cloned").removeClass("form-submit");
	$("#edit-catalog-course-save-unpublish_cloned").removeClass("ajax-processed");
	$("#edit-catalog-course-save-unpublish_cloned").bind("click",function(){
				
		if(recordType == "new")
		{
		esign_status =  create_edit_esign('new_unpublish');
		}	
	  else{
		esign_status =  create_edit_esign('edit_unpublish');
	  }
		if(esign_status == 0)
		 return false;
		
		
	$("#edit-catalog-course-save-unpublish").click();
	}); 

	}


function displayEditorInEditMode()
{
	EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
	$("#iframe_editor_wrapper").show();
	$("#iframe_editor_wrapper").css("visibility","visible");
		EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("exp-sp-administration-contentauthor-addedit-form");
	$("#iframe_editor_wrapper_container").show();
	EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.destroyLoader("contentauthor-addedit-form-container");
		$("#iframe_editor").show();
	$("#h5pe1loading").hide();
	}
	
	
function h5pCloseErrorMessage()
{
  $("#message-container").hide();
}
	

	
	
function create_edit_esign(type)
{
		/* Adding for Esign support - start*/	
		var actionType =  "";
	//	if(recordType != "new"){
		if(type == "edit"){
			actionType = "contentAuthorAddInteractions_edit";
		}else if(type == "edit_publish"){
			actionType = "contentAuthorAddInteractions_edit_publish";
		}else if(type == "edit_unpublish"){
	       actionType = "contentAuthorAddInteractions_edit_unpublish";
        }else if(type == "new")
			actionType = "contentAuthorAddInteractions_new";
        else if(type == "new_publish")
			actionType = "contentAuthorAddInteractions_new_publish"; 
        else if(type == "new_unpublish")
			actionType = "contentAuthorAddInteractions_new_unpublish"; 
		
	 	  /*if($('#iframe_editor_wrapper').attr("esigndisplayed") == "on")
			{
				//alert(12345678);
				$('#iframe_editor_wrapper').attr("esigndisplayed","off");
			}
			else if($(".h5p-editor-iframe").attr("esigndisplayed") == "on")
			{
				$('.h5p-editor-iframe').attr("esigndisplayed","off");
			}*/
		    if($(".h5p-editor-iframe").attr("esigndisplayed") == "on"){
		    	//alert("not inside the esign conditions");
		    	$('.h5p-editor-iframe').attr("esigndisplayed","off");
		    	return 1;
		    }else if($("#exp-sp-administration-contentauthor-addedit-form").attr("esigndisplayed") == "on"){
		    	$("#exp-sp-administration-contentauthor-addedit-form").attr("esigndisplayed","off");
		    	return 1;
		    }
		    
		    	
			else
			{
				
				if(availableFunctionalities != undefined && availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != "")
				{
					//var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType,'paramForPublish':paramForPublish};
					var esignObj = {'popupDiv':'addedit-edit-newtheme-save-button','esignFor':actionType};
					window.parent.$.fn.getNewEsignPopup(esignObj);
					if(actionType == "contentAuthorAddInteractions_edit" || actionType == "contentAuthorAddInteractions_edit_publish" || actionType == "contentAuthorAddInteractions_edit_unpublish"){
	 				$('.h5p-editor-iframe').attr("esigndisplayed","on");
					}
	 			   else if(actionType == "contentAuthorAddInteractions_new" || actionType == "contentAuthorAddInteractions_new_publish" || actionType == "contentAuthorAddInteractions_new_unpublish")
	 				$('#exp-sp-administration-contentauthor-addedit-form').attr("esigndisplayed","on");	 			
	 			
	 				
					
					return 0;
	         //	return false;
				}
			}  
	  	  
		
		
   /* Adding for Esign support - end*/
}	
