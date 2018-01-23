(function($) {
	try{
	var optionprofile = '';
	var gridloadpage  = '';
	var pageroption   = '';
	var hiddenuserid  = '';
$.widget("ui.myprofileskills", {
	_init: function() {
		try{
		var self = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
			var mySkillWidth = 656;	
			var mySkillpopwidth=491;
		}else{
			var mySkillWidth = 670;
			var mySkillpopwidth=470;
	    }
		if(document.getElementById('myprofile-myskillset-screen')){	
		  	optionprofile =  'myprofile-myskillset-screen';
		  	gridloadpage  =  'myprofile-skill-userdetails';
		  	pageroption   =  'userdetailsskill-pager';
		  	hiddenuserid  =  document.getElementById('hidden-other-userid').value;
		  	gridwidth     =  mySkillpopwidth;
			}else{
				optionprofile =  'wizarad-myprofile-skills' ;
				gridloadpage  =  'myprofile-skill-details';
		  	pageroption   =  'skill-pager';
		  	hiddenuserid  =  'emptyvalue';
		  	gridwidth     =  mySkillWidth;
			}
	    
		this.renderMySkillActivity();
		}catch(e){
			// to do
		}
	},
	
	callbackSkillsLoader : function(response, postdata, formid){
		try{
		$("#"+optionprofile).data('myprofileskills').destroyLoader(optionprofile);
		 $('#gbox_'+gridloadpage).show();
			var recs = parseInt($("#"+gridloadpage).getGridParam("records"),15);
	        if (recs == 0) {
	        	  $("#"+pageroption+"_center").css('display','none');
	            var norecordsskillmsg = '<div class="noskillrecords-msg-edit-page" id="hide_'+optionprofile+'">'+Drupal.t('MSG538')+'</div>';
	  					$("#gview_"+gridloadpage+" .ui-jqgrid-hdiv").html(norecordsskillmsg);
	  					 $('.noskillrecords-msg-edit-page').css('padding-top','10px');
	        } else {
	        	$('#hide_'+optionprofile).css('display','none');
	        	$(".no-orderrecords").html("");
	        }
	  
	       
	  			
					$('#gbox_'+gridloadpage+' tr.ui-jqgrid-labels').css("background", "none");
	  			Drupal.attachBehaviors();
	  			//	$("#"+optionprofile).data('myprofileskills').destroyLoader("+optionprofile+");
	        var objnew = $("#"+optionprofile).data("myprofileskills");
	        // Show pagination only when search results span multiple pages
	        var reccount = parseInt($("#"+gridloadpage).getGridParam("reccount"), 5);
	        var hideAllPageControls = true;
	        if (recs > 5) { // 10 is the least view per page option.
	         // hideAllPageControls = false;
	        }	        
	        //console.log('callbackLoader() : reccount = ' + reccount);
	        if (recs <= reccount) {
	          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
	        	//objnew.hideActivityPageControls(hideAllPageControls);
	        }
	        else {
	          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
	        	//objnew.showActivityPageControls();
	        }    
		   $('#'+gridloadpage+' > tbody > tr').last().find('.activity-seperate').css('border','0px');
		   $("#learner-myskill-details .ui-jqgrid-bdiv").css("overflow","hidden");
		   $("#gbox_"+gridloadpage).css("margin","0 auto");
		   vtip();
		   resetFadeOutByClass('#myprofile-skill-details','content-detail-code','line-item-container','profile');
		   $('#learner-myskill-details .limit-title').trunk8(trunk8.profileskill_title);
		}catch(e){
			// to do
		}
	},
  	openCertificateDialog: function(certId,type) {
  	
		try{
	   console.log(certId);
	   console.log(type);
		closeQtip('');	//to close all open qtips. for 042549: Share pop up is not automatically closing...
		//this.createLoader(loaderDiv);
		$("#"+optionprofile).data('myprofileskills').createLoader(optionprofile);
		var url = this.constructUrl('learning/ajax/profile/certificate-details/'+type+'/'+certId+'');
		
		var obj = this;
		$.ajax({
			type: 'POST',
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result) {
		
			//	obj.destroyLoader(loaderDiv);
				$("#"+optionprofile).data('myprofileskills').destroyLoader(optionprofile);
				//$('#add_certificate_container').css('overflow', 'normal');
				obj.paintAddCertificateDialog(result);
					if(result.drupal_settings) {
				   $.extend(true, Drupal.settings, result.drupal_settings);
				}
					Drupal.attachBehaviors();
					
				$('#mycertificate-parent .certificate-name-parent').find('input').attr('autocomplete', 'off');
				$('#mycertificate-parent .form-type-checkbox').append('<i/>');	
	             $('.profile-add-menu').hide();		
					
	(function($) {
	  try{
	 $("#edit-upload-certificate").parent().addClass("uploadiconbgshow");
	 

	 
	 var text1 = '<span class="customlabelfileuploadhelp upload-msg-style">('+Drupal.t('MSG1113')+')</span>';
		if($(".customlabelfileuploadhelp").size() == 0 && $("#CERT_IMG").attr("value") == ""  && $('#edit-upload-certificate').val() == "")
			$("#add_certificate_container").find(".addedit-new-field-value").find(".form-item-files-upload-certificate").find("#edit-upload-certificate").css({"color":"transparent","max-width": "75px"}).after(text1);
		
     if($("#mycertificate-parent").hasClass("certNoDelete")){
       $("#edit-upload-certificate").bind("click.myclick", function(event) {
			  event.preventDefault();
			});
     }
   changeCertificate();	
	}catch(e){
		// to do
	}
  })(jQuery);
	
	if($("#CERT_IMG").attr("value") != "")
    {
   var filename = $("#CERT_IMG").attr("value");
     fileChange(filename);
     
    }
   var  checkboxStatus = $('#edit-verify-required').is(':checked');
   if(type == 'add' || (type == 'edit' && $("#tmpdata").size() == 0 ) )
   {
     $('#edit-verify-required').attr('disabled',true);
     $('#edit-verify-required').css('cursor','default');
   }
 
 
if(checkboxStatus == false || checkboxStatus == "false")
 $("#verify_block").hide();
 else
  $("#verify_block").show(); 
  
  
  if($('#edit-verification-by-externaluser').is(':checked')) { 
    $(".ext_user_fieldset").show();
   }
  else{
    $(".ext_user_fieldset").hide();
  }
  
  if($('#edit-verification-by-user').is(':checked')) { 
   $(".input_fellow_username").css('display','inline-block');
   }
   else
   $(".input_fellow_username").css('display','none');
  
  
    
    				
					
					
			}
	 });	
	    
		}catch(e){
			// to do
		}
	},
	
	openSkillDialog: function(type,certId) {
		try{
	//	alert(11);
	   console.log(certId);
	   console.log(type);
		closeQtip('');	//to close all open qtips. for 042549: Share pop up is not automatically closing...
		//this.createLoader(loaderDiv);
		$("#"+optionprofile).data('myprofileskills').createLoader(optionprofile);
		 
		var url = this.constructUrl('learning/ajax/skill-set/add-skill/'+type+'/'+certId+'');
		
		var obj = this;
		$.ajax({
			type: 'POST',
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result) {
			//	obj.destroyLoader(loaderDiv);
				$("#"+optionprofile).data('myprofileskills').destroyLoader(optionprofile);
				//$('#add_certificate_container').css('overflow', 'normal');
				$('#wizarad-myprofile-skills').data('myprofileskills').paintAddSkillDialog(result);
					if(result.drupal_settings) {
				   $.extend(true, Drupal.settings, result.drupal_settings);
				}
					Drupal.attachBehaviors();
					$('#myskill-parent .form-type-checkbox').append('<i/>');
					$('.profile-add-menu').hide();	
			}
		
	            
	  });
	 
		}catch(e){
			// to do
		}
	},
	
	ShowCertificateDet : function(certName,Company,certNumber,validity,fileName, imgPath, fileFormat) {	
		try{
		
		
		
		
		var obj = this;

			var rhtml = '';
			
			
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("LBL205")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+certName+'">'+titleRestrictionFadeoutImage(certName, 'my-cert-name-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("Company")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+Company+'">'+titleRestrictionFadeoutImage(Company,'my-company-name-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("LBL205")+" "+Drupal.t("LBL161")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+certNumber+'">'+titleRestrictionFadeoutImage(certNumber,'my-cert-number-fadeout-container')+'</span></div>';
			rhtml += '<div  class = "cert_name"><label>'+Drupal.t("LBL604")+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+validity+'">'+titleRestrictionFadeoutImage(validity,'my-validity-date-fadeout-container')+'</span></div>';

			rhtml += '<div  class = "cert_name"><label>'+Drupal.t('LBL610')+' '+Drupal.t("LBL205").toLowerCase()+':</label>';
			rhtml += '<span  class = "cls-crt-lbl-val vtip" title = "'+fileName+'">'+titleRestrictionFadeoutImage(fileName,'my-file-name-fadeout-container')+'</span></div>';
		/*	if(fileName == "no"){
				rhtml += '<div class = "access-tab-icon certificate-icon-disable vtip">';
				rhtml += '<span class = "view_certificate certificate-view-disable">'+Drupal.t('LBL205')+'</span></div>';
		}
			else{
				rhtml += '<div class="access-tab-icon vtip">';
				rhtml += '<span  class = "view_certificate" onclick = $("#wizarad-myprofile-skills").data("myprofileskills").viewCertificateskill(\"'+encodeURIComponent(certName)+'\",\"'+encodeURIComponent(imgPath)+'\",\"'+fileFormat+'\")>'+Drupal.t('LBL205')+'</span></div>';
			//	rhtml += 'onclick = viewCertificate(certName,imgPath,fileFormat)</div>';
*/				
				
		//	}
			//rhtml += '</form>';
			
			//var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';
			var dlgDiv = '<div id="certificate_det_container" class="view_cert-container"></div>';
			$('#certificate_det_container').remove();
			$("#cert_dialouge_holder").remove();
			$('body').append(dlgDiv);
			
			$('#certificate_det_container').html(rhtml);

           var closeButt={};
			closeButt[SMARTPORTAL.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#certificate_det_container').remove();};
			
			$("#certificate_det_container").dialog({
				bgiframe: true,
				width:420,
				resizable:false,
				draggable:false,
				closeOnEscape: false,
				modal: true, 
				title: Drupal.t('LBL205'),
				buttons: closeButt,
				close: function(){
					$("#certificate_det_container").remove();
					$("#cert_dialouge_holder").remove();
				},
				overlay:
				{
				   opacity: 0.4,
				   background: '#000000'
				 }	
			});
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr("tabindex",6);
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr("tabindex",7);
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
			}
			//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog').wrap("<div id='cert_dialouge_holder' class='refer-course-holder'></div>");
			$('#cert_dialouge_holder a.ui-dialog-titlebar-close').html('X');
			//$('.ui-dialog-buttonset').before(noteTxt);
			$('.refer-class-container').css('width','93.2%');
			$('.refer-class-container').css('height','auto');
			vtip();
		
		//return rowObject['details'];
		}catch(e){
			// to do
			//console.log(e);
		}
	},
	
	viewCertificateskill : function(certName, imgPath, fileFormat) {	
		try{
		var obj = this;

			var rhtml = '';
			
			rhtml += '<div  class = "cert_name">';
			if(fileFormat == "pdf" || fileFormat == "PDF"){
			rhtml += '<iframe src="'+imgPath+'" class="vtip" /></div>';
			}
			else if(fileFormat == "png" || fileFormat == "PNG"){
			rhtml += '<img src="'+imgPath+'" class="vtip" /></div>';
			}
			//rhtml += '</form>';
			
			//var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';
			var dlgDiv = '<div id="certificate_det_container" class="view_cert-container pdfWrap" style="padding:0"></div>';
			$('#certificate_det_container').remove();
			$("#cert_dialouge_holder").remove();
			$('body').append(dlgDiv);
			
			$('#certificate_det_container').html(rhtml);

           var closeButt={};
			closeButt[SMARTPORTAL.t('LBL123')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#certificate_det_container').remove();};
			
			if(certName.length >70){
			var titleDisplay = '<div class="line-item-container float-left customSpanstyle"><span title="'+Drupal.t(certName)+'" class="vtip">'+titleRestrictionFadeoutImage( Drupal.t(certName),'mycertificateTitle')+'</span></div>';
			}
			else{
			var titleDisplay = '<div class="line-item-container float-left customSpanstyle"><span title="'+Drupal.t(certName)+'" class="vtip">'+Drupal.t(certName)+'</span></div>';
			}
			$("#certificate_det_container").dialog({
				bgiframe: true,
				width:688,
				resizable:false,
				draggable:false,
				closeOnEscape: false,
				modal: true, 
				title: titleDisplay,
				buttons: closeButt,
				close: function(){
					$("#certificate_det_container").remove();
					$("#cert_dialouge_holder").remove();
				},
				overlay:
				{
				   opacity: 0.4,
				   background: '#000000'
				 }	
			});
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr("tabindex",6);
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr("tabindex",7);
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
			}
			//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
			$('.ui-dialog').wrap("<div id='cert_dialouge_holder' class='refer-course-holder'></div>");
			$('#cert_dialouge_holder a.ui-dialog-titlebar-close').html('X');
			//$('.ui-dialog-buttonset').before(noteTxt);
			$('.refer-class-container').css('width','93.2%');
			$('.refer-class-container').css('height','auto');
		
		
		//return rowObject['details'];
		}catch(e){
			// to do
			//console.log(e);
		}
	},
	paintAddSkillDialog: function(result) {
		try{
		//alert(1);
		
		var noteTxt = '<div class="refer-note">' +
		              '</div>';
		var dlgDiv = '<div id="add_skill_container" class="add-skill-container"></div>';
		//$('#add_skill_container').remove();
		
		$('body').append(dlgDiv);
		var paintHtml = "<div id='paintskillAddcert'><div id='show_expertus_message'></div>"+result.render_content_main+"</div>";
		$('#add_skill_container').html(paintHtml);


		$('#add_skill_container').dialog({
			bgiframe: true,
			width: 365,
			resizable: false,
			draggable: false,
			closeOnEscape: false,
			modal: true, 
			title:  Drupal.t('LBL876'),
			//buttons: closeButt,
			close: function() {
			  closeSkilldialog();
			},
			overlay: {
			   opacity: 0.4,
			   background: '#000000'
			}	
		});
		if (($.browser.msie && this.currTheme == 'expertusoneV2' &&
		      (parseInt($.browser.version, 10) == '9' || parseInt($.browser.version, 10) == '8'))) {
		  //$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
		}
		
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass('admin-save-button-middle-bg');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('removebutton').end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr('tabindex', 6);
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('tabindex', 7);
		
		if (this.currTheme == 'expertusoneV2') {
		 changeDialogPopUI();
		}
		
		$('.ui-dialog').wrap('<div id="add_skill_prof_holder" class="add-skill-holder"></div>');
		$('#add_skill_prof_holder a.add_skill_prof_holder').html('X');
		$('.ui-dialog-buttonset').before(noteTxt);
	//	$('.request-class-container').css('width', '93.9%');
	
	$('.add_skill_prof_holder').click(function(){
		alert(2);
	       // $("#delete-msg-wizard").remove();
	    });
		}catch(e){
			console.log(e);
		}
	},
	
	paintAddCertificateDialog: function(result) {
		try{
		//alert(1);
		
		var noteTxt = '<div class="refer-note">' +
		              '</div>';
		var dlgDiv = '<div id="add_certificate_container" class="add-certi-container"></div>';
		//$('#add_certificate_container').remove();
		
		$('body').append(dlgDiv);
		var paintHtml = "<div id='paintContentAddcert'><div id='show_expertus_message'></div>"+result.render_content_main+"</div>";
		$('#add_certificate_container').html(paintHtml);


		$('#add_certificate_container').dialog({
			bgiframe: true,
			width: 420,
			resizable: false,
			draggable: false,
			closeOnEscape: false,
			modal: true, 
			title: Drupal.t('LBL205'),
			//buttons: closeButt,
			close: function() {
			  closeCertidialog();
			},
			overlay: {
			   opacity: 0.4,
			   background: '#000000'
			}	
		});
		if (($.browser.msie && this.currTheme == 'expertusoneV2' &&
		      (parseInt($.browser.version, 10) == '9' || parseInt($.browser.version, 10) == '8'))) {
		  //$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
		 // $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
		}
		
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass('admin-save-button-middle-bg');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('removebutton').end();
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').attr('tabindex', 6);
		$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').attr('tabindex', 7);
		
		if (this.currTheme == 'expertusoneV2') {
		 changeDialogPopUI();
		}
		
		$('.ui-dialog').wrap('<div id="add_cert_prof_holder" class="add-cert-holder"></div>');
		$('#add_cert_prof_holder a.add_cert_prof_holder').html('X');
		$('.ui-dialog-buttonset').before(noteTxt);
	//	$('.request-class-container').css('width', '93.9%');
	
	$('.add_cert_prof_holder').click(function(){
		alert(2);
	       // $("#delete-msg-wizard").remove();
	    });
		}catch(e){
			console.log(e);
		}
	},
	paintSkillsSearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},
	/*
	 * renderMySkillActivity() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderMySkillActivity: function() {
		try{
		this.createLoader(optionprofile);
		// var obj = $("#"+optionprofile).data("myprofileskills");
	  var objStr = $("#"+optionprofile).data('myprofileskills');
		//alert(objStr.toSource());
		//var obj = this;
		// Construct the jqGrid
		//alert(gridloadpage+'option'+optionprofile);
		 $("#"+gridloadpage).jqGrid({
			  url: this.constructUrl("learning/myprofile/skillset/"+hiddenuserid),
			  datatype: "json",
			  mtype: 'GET',
			  colNames:['Details'],
			  colModel:[{name:'Details',index:'Details', title:false, width:595,'widgetObj':objStr,formatter:objStr.paintSkillsSearchResults}],
 			  rowNum:100,           
  		  rowList:[15,20,30,40],
 				pager: '#'+pageroption,
 				viewrecords:  true,
 				emptyrecords: "",
 				sortorder: "desc",
 				toppager:false,
 				height: 'auto',
 				width: gridwidth,
 				loadtext: "",
 				recordtext: "",
 				pgtext : "{0}"+" "+Drupal.t('MSG1113')+" "+"{1}",
 				loadui:false,
 				hoverrows:false,	
			  loadComplete:objStr.callbackSkillsLoader
		}).navGrid('#'+pageroption,{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}catch(e){
			// to do
		}
	},
	
	renderSkillPage : function(id){
		try{
		//this.createLoader('forum-topic-list-display');
		$('#'+gridloadpage).setGridParam({url: this.constructUrl('learning/myprofile/skillset/emptyvalue')});
		$('#'+gridloadpage).trigger("reloadGrid",[{page:1}]);
		//If Add/Edit action taken in Skill widget Activity widget will be loaded
		$("#myprofile-activity-details").trigger("reloadGrid",[{page:1}]);
		}catch(e){
			// to do
		}
	},
	callSkillDeleteProcess : function(skillId){
		try{
		$('#delete-msg-wizard').remove();
		this.createLoader('wizarad-myprofile-skills'); 
		var obj = this;
			url = obj.constructUrl("learning/myprofile/skills/remove/"+ skillId);
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#'+gridloadpage).setGridParam({url: obj.constructUrl('learning/myprofile/skillset/emptyvalue')});
					$("#"+gridloadpage).trigger("reloadGrid",[{page:1}]);
					//If Add/Edit action taken in Skill widget Activity widget will be loaded
					if(document.getElementById('wizarad-myprofile-skills')){
						$("#myprofile-activity-details").trigger("reloadGrid",[{page:1}]);
					}
				}
		    });
		}catch(e){
			// to do
		}
	},
	exportSearchResults : function() {
		try{
		var obj = this;
		console.log('exportSearchResults called');
		var exportPath = 'learning/myprofile/export';
		console.log(exportPath);
		console.log('soun ----->');
		console.log(obj.constructUrl(exportPath));
		curl = obj.constructUrl(exportPath);
		window.location = curl;
		}catch(e){
			// to do
		}
	},
	displaySkillDeleteWizard : function(title, skillId ){
		try{
		var wSize =  330;
	    $('#delete-msg-wizard').remove();
	    var html = '';	    
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		
	   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
	    html+= '<tr><td color="#333333" style="padding: 30px 15px; text-align: justify;" class="skill-delete-msg commanTitleAll">'+title+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	    
	    	closeButt[Drupal.t('Yes')]=function(){ $("#wizarad-myprofile-skills").data('myprofileskills').callSkillDeleteProcess(skillId); };
	    
	    $("#delete-msg-wizard").dialog({
	        position:[(getWindowWidth()-400)/2,200],
	        bgiframe: true,
	        width:wSize,
	        resizable:false,
	        modal: true,
	        title:Drupal.t("LBL286"),//"title",
	        buttons:closeButt,
	        closeOnEscape: false,
	        draggable:false,
	        overlay:
	         {
	           opacity: 0.9,
	           background: "black"
	         }
	    });
	
	    $('.ui-dialog').wrap("<div id='delete-object-dialog'></div>");

	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
	    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
			if(this.currTheme == "expertusoneV2"){
	       changeDialogPopUI();
	     /*  if ( $.browser.msie && parseInt($.browser.version, 10)=='9' && this.currTheme == "expertusoneV2"|| $.browser.msie && parseInt($.browser.version, 10)=='8'&& this.currTheme == "expertusoneV2"){
	    		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').before('<div class="white-btn-bg-left"></div>');
	    		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass('white-btn-bg-middle');
	    		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').after('<div class="white-btn-bg-right"></div>');
	    		 $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').next('.white-btn-bg-right').css('margin-right','0px');
	    		}*/
			}
	    $("#delete-msg-wizard").show();
	
		$('.ui-dialog-titlebar-close').click(function(){
		//alert(2);
	        $("#delete-msg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	}
});

$.extend($.ui.myprofileskills.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
	}catch(e){
		// to do
	}
})(jQuery);

function closeCertidialog(){
$('#add_certificate_container').remove();
				$('#add_cert_prof_holder').remove();
				$("#wizarad-myprofile-skills").myprofileskills();
		        $("#wizarad-myprofile-skills").data('myprofileskills').renderSkillPage();

}

function closeSkilldialog(){
$('#add_skill_container').remove();
				$('#add_skill_prof_holder').remove();
				$("#wizarad-myprofile-skills").myprofileskills();
		        $("#wizarad-myprofile-skills").data('myprofileskills').renderSkillPage();

}

$(function() {
	try{
  if(document.getElementById('myprofile-myactivity-screen')){	
   	$("#myprofile-myskillset-screen").myprofileskills();
	}
  else{	
		$("#wizarad-myprofile-skills").myprofileskills();
	}
	}catch(e){
		// to do
	}
});
$('body').click(function (event) {
	try{
	
	if ( (!$(event.target).parents(".profile-add-menu").length)) {
	     if($(event.target).hasClass('addskillcerti') != true && $(event.target).hasClass('profile-add-menu') != true ) {

			$('.profile-add-menu').hide();
		   }
		}
	}catch(e){
		// to do
		//console.log(e);
	}
});

(function($) {
	try{
	$.fn.refreshSkillList = function() {
		$(".active-qtip-div").remove();
		$("#wizarad-myprofile-skills").myprofileskills();
		$("#wizarad-myprofile-skills").data('myprofileskills').renderSkillPage();
		
	};
	}catch(e){
		// to do
	}
})(jQuery);

 
  
  Drupal.ajax.prototype.commands.onAfterLoadFormCert = function() {

		var text1 = '<span class="customlabelfileuploadhelp upload-msg-style">('+Drupal.t('MSG1113')+')</span>';
		if($(".customlabelfileuploadhelp").size() == 0 && $("#CERT_IMG").attr("value") == ""  && $('#edit-upload-certificate').val() == ""){
			$("#add_certificate_container").find(".addedit-new-field-value").find(".form-item-files-upload-certificate").find("#edit-upload-certificate").css({"color":"transparent","max-width": "75px"}).after(text1);
		}
	

// For #0088440		
	if($("#tmpdata").size() != 0){
	 var ext = $("#tmpdata").text(); 
	 ext = ext.split('.').pop();
	 ext = ext.trim();
	 if(ext != 'png' && ext != 'PNG' && ext != 'PDF' && ext != 'pdf'){
	  showeditvideo();
	 }	
	 }
	
	 var certImg = $("#tmpdata").size();
	  if($("#tmpdata").size() == 0){
	   var certId = $("#CERT_ID").attr("value");
	  	var url = '/?q=learning/skill-set/delete-img/'+certId+'';
			var obj = this;
			$.ajax({
				type: 'POST',
				url: url,
				success: function(result) {
				}
			
		            
		  });
	  }

	return true;
};



function showeditvideo()
{

//if($('#edit-verify-required').is(':checked'))
//{ 
 $('#edit-verify-required').attr('checked',false);
 $('#edit-verify-required').attr('disabled',true);
  $('#edit-verify-required').css('cursor','default');
//}

	
 $("#tmpdata").remove();
 $("#edit-upload-certificate").show();
 $("#edit-upload-certificate").parent().removeClass("uploadiconbghide");
 $("#edit-upload-certificate").parent().addClass("uploadiconbgshow");
 //$("#edit-upload-certificate").parent().css("width","200px");
$("#edit-upload-certificate").removeAttr("disabled");


 $(".customlabelfileuploadhelp").html('('+Drupal.t('MSG1113')+')');
 $(".customlabelfileuploadhelp").show();
 $("#add_certificate_container").find(".addedit-new-field-value").find(".form-item-files-upload-certificate label").show();
 $(".customlabelfileupload").show();
 //$("#VIDEO_DELETE").val("TRUE");
 $("#videofilenamedisplay").html("");
  $("#CERT_IMG").val("");
  $("#edit-upload-certificate").val("");
  
  
}

function changeCertificate(obtainedFile){
	$("#edit-upload-certificate").change(function(){
			var filename = $("#edit-upload-certificate").val();
			 fileChange(filename);
		});
		
}



function fileChange(filename)
{

	var ext = filename.split('.').pop();
	 ext = ext.trim();
	 if(($("#mycertificate-parent").hasClass("certNoDelete")) || (ext != 'png' && ext != 'PNG' && ext != 'PDF' && ext != 'pdf')){
	  $('#edit-verify-required').attr('disabled',true);
	   $('#edit-verify-required').css('cursor','default');
	 }
	 else
	 {
	   $('#edit-verify-required').attr('disabled',false);
	    $('#edit-verify-required').css('cursor','pointer');
	 }	
	 
	if(filename != "" || filename != undefined)
			{
				filename = filename.replace("\\","/");
				filename = filename.replace("\\","/");
			
				filename = filename.split("/");
				$(".customlabelfileupload").show();
			//	$("#VIDEO_DELETE").val("FALSE");
				
				
				$(".customlabelfileupload").hide();
				$(".customlabelfileupload").next().remove();
				$(".customlabelfileuploadhelp").hide();
				
				$("#add_certificate_container").find(".addedit-new-field-value").find(".form-item-files-upload-certificate label").hide();
				
				var lengthytitle = '';
				if(filename[filename.length-1].length > 30)
					lengthytitle = '<div class="fade-out-title-container  video-file-title-fadeout-container uploadBtnArrow"><span class="title-lengthy-text">'+filename[filename.length-1]+'</span><span class="fade-out-image"></span></div>';
				else
					lengthytitle = filename[filename.length-1];
					
			   var clickEvt = "onclick='showeditvideo();'";
				if($("#mycertificate-parent").hasClass("certNoDelete"))
					clickEvt = "<a>&nbsp;</a>";
				else
				clickEvt = "<a href='javascript:void(0);' "+clickEvt+" >&nbsp;</a>";
				
				if($("#tmpdata").size() == 0){
				$("#edit-upload-certificate").parent().append("<div id='tmpdata' class='uploadBtnArrowVid'>"+lengthytitle+"&nbsp;"+clickEvt+"</div>");
				 $("#mycertificate-parent").addClass("fileNotDeleted");
				
				}
				$("#edit-upload-certificate").css("display","none");
				$("#edit-upload-certificate").parent().addClass("uploadiconbghide");
				$("#edit-upload-certificate").parent().removeClass("uploadiconbgshow");
				var file_name=$("#tmpdata .title-lengthy-text").html();
				$('#tmpdata').addClass('vtip');
				$('#tmpdata').attr('title', file_name);	
				vtip();
				
			}


}


$('body').bind('click', function(event) {
 
try{
	

 var text1 = '<span class="customlabelfileuploadhelp upload-msg-style">('+Drupal.t('MSG1113')+')</span>';
		if($(".customlabelfileuploadhelp").size() == 0 && $("#CERT_IMG").attr("value") == "" && $('#edit-upload-certificate').val() == "")
	    $("#add_certificate_container").find(".addedit-new-field-value").find(".form-item-files-upload-certificate").find("#edit-upload-certificate").css({"color":"transparent","max-width": "75px"}).after(text1);
		


 checkboxStatus = $('#edit-verify-required').is(':checked');
 console.log(checkboxStatus);
if(checkboxStatus == false || checkboxStatus == "false")
{
 $("#verify_block").hide();
 $("#edit-user-text").val("");
 $("#edit-ext-user-name").val("");
 $("#edit-ext-phone-no").val("");
 $("#edit-ext-email").val("");
 
 
 
 }else
  $("#verify_block").show(); 
  
  if($('#edit-verification-by-externaluser').is(':checked')) { 
    $(".ext_user_fieldset").show();
   }
  else{
    $(".ext_user_fieldset").hide();
  }
  
  if($('#edit-verification-by-user').is(':checked')) { 
   $(".input_fellow_username").css('display','inline-block');
   }
   else
   $(".input_fellow_username").css('display','none');
  

	if(event.target.id != 'admin-dropdown-arrow'){
		$('#myteam-cat-delivery-type-list').hide();
	}
	}catch(e){
		// to do
	}
	
});
 
 
 
 
