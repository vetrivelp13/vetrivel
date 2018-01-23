(function($) {
$.widget("ui.surveylearner", {
	_init: function() {
	  try {	
	    var self = this;
	    this.currTheme = Drupal.settings.ajaxPageState.theme;
		this.widgetGlobalName="surveylearner";
		this.objectId;
		this.objectTitle;
		this.objectType;
		this.objectStatus;
		this.isPreview;
		this.surveyType='';
		this.surveyId;
		this.seccount=0;
		this.scnt = 1;
		this.cpage=1;
		this.npage=2;
		this.ppage=0;
		this.dialogobj;	
		this.viewOptionSurveyArr=new Array();
		this.viewOptionSurvey;
		this.lnrId;
		this.surveyCount;
		this.uniqueWidgetId;
		this.surveyHeader;
		this.showPreview = 0;
		this.committed = 0;
		this.paramsUpdScore = '';
		this.startTime = '00.00.00';
	  }catch(e){
			// to do
		}
	},	
	callTakeSurveyToClass : function(objId,objTitle,objectType,type, paramsUpdScore,enrollId,preStatus,loaderDiv){
	 try{
		if (loaderDiv != undefined) { // #45963: UI Issue -- Survey in learner site
			this.createLoader(loaderDiv); 
			this.loaderDiv	= loaderDiv; 
		} 
		this.paramsUpdScore = paramsUpdScore;
		this.cpage=1;
		this.npage=2;
		this.ppage=0;
		this.lnrId 	= this.getLearnerId();
		this.uniqueWidgetId = 'take_survey_main';
		this.objectId		= objId;
		this.objectTitle	= unescape(objTitle);
		this.objectType		= objectType;
		this.type 			= type==''?'survey':type;
		this.enrollId = enrollId;
		this.preStatus = (preStatus == 'NULL') ? '' : preStatus;
		if(objectType =='cre_sys_obt_cls'){
			this.objectStatus	= 'lrn_crs_reg_cnf';
		}
		else{
			this.objectStatus	= 'lrn_tpm_ovr_cmp';
		}
		this.isPreview		= 'No';
		this.surveyWId		= '';
		this.paintSurveyDetails();
	 }catch(e){
			// to do
		}
	},
	paintSurveyDetails : function(){
		try{
			$("#"+this.uniqueWidgetId).remove();
			var html = '';
			html	+= '<div id="'+this.uniqueWidgetId+'" class="SurveyContainer" style="padding:0px; backgroun-color: #fff;overflow-x: hidden;">';
			html	+= '</div>';
			if(this.isPreview=="Yes")
			$("body").html("");
			$("body").append(html);
			var closeButt = {};		
			wobj = this;
			
			var submit	= "wobj.surveyCommit()";
			var next 	= "wobj.moveNext('')";
			var prev 	= "wobj.movePrev()";
			var yes		= "wobj.moreSurvey()";
			var preview	= "wobj.loadPreview()";
			
			if(this.isPreview=="No"){
				this.surveyHeader = titleRestrictionFadeoutImage(this.objectTitle,'surveylearnear-js-assessment-comsurvey-title-for-popup');
				//closeButt[Drupal.t('LBL123')]	= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); $("#take_survey_main").remove(); };
				closeButt[' '+Drupal.t('LBL123')+ ' ']	= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); $("#take_survey_main").remove(); };
				closeButt[Drupal.t('LBL123')]	= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); $("#take_survey_main").remove(); };
				
			}else{
				this.surveyHeader = "Survey Preview";
				closeButt[Drupal.t('LBL109')]	= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); window.close(); };
				closeButt[Drupal.t('LBL123')]	= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); window.close(); };
			}
			closeButt[Drupal.t('No')]		= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); $("#take_survey_main").remove(); };
			closeButt['<<'+Drupal.t('LBL692')]	= function(){ eval(prev);};
			closeButt[Drupal.t('Yes')]		= function(){eval(yes);};
			closeButt[Drupal.t('LBL124')]	= function(){ eval(submit);};
			closeButt[Drupal.t('LBL693')+'>>']	= function(){ eval(next);};
			closeButt[Drupal.t('LBL694')]	= function(){ eval(preview);};
			/*
			closeButt[SMARTPORTAL.t('No')]		= function(){ $(this).dialog('destroy');$(this).dialog('close');$('.survey-main-holder').remove(); $("#take_survey_main").remove(); };
			closeButt[SMARTPORTAL.t('<<Previous')]	= function(){ eval(prev);};
			closeButt[SMARTPORTAL.t('Yes')]		= function(){eval(yes);};
			closeButt[SMARTPORTAL.t('Submit')]	= function(){ eval(submit);};
			closeButt[SMARTPORTAL.t('Next>>')]	= function(){ eval(next);};
			closeButt[SMARTPORTAL.t('Preview')]	= function(){ eval(preview);};
			*/
			if(this.viewOptionSurvey=="H"){
				var possi = this.isPreview=="No"?[(getWindowWidth()-800)/2,80]:[2,2];
			}else{
				var possi = this.isPreview=="No"?[(getWindowWidth()-800)/2,80]:[2,2];
			}
			if(this.isPreview=="No"){			
				this.getSurveyDetails();
			}else{		
				this.getSurveyQuestions(this.surveyWId);
			}
			$('#'+this.uniqueWidgetId).show();
			if(this.viewOptionSurvey=="H"){  //-- View ---------
				this.dialogobj=$('#'+this.uniqueWidgetId).dialog({
					autoOpen: false,
					autoResize: true,
					width: 800,
					title: this.type=='survey'? '<span title="'+this.objectTitle+'" class="vtip">'+Drupal.t('LBL717')+ '-' +titleRestrictionFadeoutImage(this.objectTitle,'surveylearnear-js-assessment-comsurvey-title-for-popup')+'</span>': '<span title="'+this.objectTitle+'" class="vtip">'+Drupal.t('LBL718')+ '-' +titleRestrictionFadeoutImage(this.objectTitle,'surveylearnear-js-assessment-comsurvey-title-for-popup')+'</span>',
					modal: true,	
					resizable: false,
					draggable: false,
					position: possi,
					buttons: closeButt,
					overlay: 
					{ 
					   opacity: 0.5, 
					   background: "black"
					},
					close: function(){
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('.survey-main-holder').remove(); 
						$("#take_survey_main").remove();
					}
				});	
			}else{  //-- View ---------
				this.dialogobj=$('#'+this.uniqueWidgetId).dialog({
					autoOpen: false,
					autoResize:true,
					width:800,
					title: '<span title="'+this.objectTitle+'" class="vtip">'+titleRestrictionFadeoutImage(this.objectTitle,'surveylearnear-js-assessment-survey-title-for-popup')+'</span>',
					modal: true,	
					resizable:false,
					draggable:false,
					position:possi,
					buttons:closeButt,
					overlay: 
					{ 
					   opacity: 0.5, 
					   background: "black"
					},
					close: function(){
						$(this).dialog('destroy');
						$(this).dialog('close');
						$('.survey-main-holder').remove(); 
						$("#take_survey_main").remove();
					}
				});				
			} 
			if(this.isPreview=="No"){
				
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("removebutton").end();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();	
			}
			$('#take_survey_main').parent('.ui-dialog').css('border: solid 1px #0630b3');
			$('#take_survey_main').parent('.ui-dialog').wrap("<div class='survey-main-holder'></div>");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
			this.chkdisableBtn();
			if(this.isPreview=="Yes"){
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
			}
		}catch(e){
			// to do
		}
		},
		loadPreview : function(){
		 try{	
			this.seccount=0;
			this.scnt = 1;
			this.cpage=1;
			this.npage=2;
			this.ppage=0;
			this.showPreview = 1;

			// Changing the title
			var currentTitle = $('#ui-dialog-title-take_survey_main').html();
			title = currentTitle.replace('Assessment for','Assessment preview for');
			$('#ui-dialog-title-take_survey_main').html(title);
			this.getSurveyQuestions(this.surveyId);

			// Hide Preview tab
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
			this.chkdisableBtn();
		 }catch(e){
				// to do
			}
		},
		moreSurvey : function(){
			try{
			this.getSurveyDetails();
			this.cpage=1;
			this.npage=2;
			this.ppage=0;
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').hide();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:first').show();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
			this.chkdisableBtn();
			}catch(e){
				// to do
			}
		},
		previewValidateAnswer : function(){
			try{				
			$('.SurveyContent .counteIdClass').each(function(){
				var counte = this.value;
				var right_answer = $('#'+counte+'_ans').val();
				var answer_given = $('#'+counte+'_given_ans').val();
				var answer_status = $('#'+counte+'_given_stat').val();
				var questiontype = $('#'+counte+'_questiontype').val();
				var displayrating = $('#'+counte+'_displayrating').val();
				$('#'+counte+'_result_image').removeClass('wrong-img');
				$('#'+counte+'_result_image').removeClass('correct-img');
				switch(questiontype){
					case 'sry_qtn_typ_rtg':
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html('At least an option should be checked.');
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
						}
						if(displayrating == 'V'){
							$('input[name='+counte+'_RatingVertical]').each(function(){
								if($(this).attr('value') == answer_given)
									$(this).attr('checked','checked');
							});
						} else {
							$('input[name='+counte+'_RatingHorizontal]').each(function(){
								if($(this).attr('value') == answer_given)
									$(this).attr('checked','checked');
							});
						} 
						break;
					case 'sry_qtn_typ_cmt':
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html('Comments should be entered.');
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
							$('#'+counte+'_Text').val(answer_given);
						}
						break;
				    case 'sry_qtn_typ_yno':
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html(Drupal.t("MSG507")+': '+right_answer);
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
						}
						$('input[name='+counte+'_yesno]').each(function(){
							if($(this).attr('value') == answer_given)
								$(this).attr('checked','checked');
						});
						break;
				    case 'sry_qtn_typ_trf':
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html(Drupal.t("MSG507")+': '+right_answer);
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
						}
						$('input[name='+counte+'_truefalse]').each(function(){
							if($(this).attr('value') == answer_given)
								$(this).attr('checked','checked');
						});
				    case 'sry_qtn_typ_dpn':
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html(Drupal.t("MSG507")+': '+right_answer);
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
						}
						$('#'+counte+'_Dropdown option').each(function(){
							if($(this).attr('value') == answer_given)
								$(this).attr('selected','selected');
						});
						break;
				    case 'sry_qtn_typ_mch':				    	
						if(answer_status!='CORRECT'){
							$('#'+counte+'_result_image').addClass('wrong-img');
							$('#'+counte+'_disp_corr_ans').html(Drupal.t("LBL714")+' '+Drupal.t("Answers")+':'+' '+ right_answer.replace(/##/g , ", "));
						} else {
							$('#'+counte+'_result_image').addClass('correct-img');
						}
						var answer_given_arr = answer_given.split(Drupal.settings.custom.EXP_AC_SEPARATOR);
						if(displayrating == 'V'){
							$('input[name='+counte+'_MultiVertical]').each(function(){
								for(var mc=0; mc < answer_given_arr.length; mc++ ){
									if($(this).attr('value') == answer_given_arr[mc])
										$(this).attr('checked','checked');
								}
							});
						} else {							
							$('input[name='+counte+'_MultiHorizontal]').each(function(){
								for(var mc=0; mc < answer_given_arr.length; mc++ ){	
									var thisValue = $(this).attr('value');
									thisValue = thisValue.replace(/&quote/g,"").replace(/'/g,"");									
									answer_given_arr[mc] = answer_given_arr[mc].replace(/&quote/g,"").replace(/'/g,"");									
									if(thisValue == answer_given_arr[mc]){	
										if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
										$(this).parent().removeClass('checkbox-unselected').addClass('checkbox-selected');
										}else{
										$(this).parent().removeClass('checkbox-unselected');	
										}									
										$(this).attr('checked','checked');									
																		
									}
										
								}
							});
						} 
				    	break;
				}

			});
			}catch(e){
				// to do
			}
		},
		moveNext:function(surId){
		 try{
			if(surId!='' && surId!=null && surId!='null'){
				var surveyId = surId;
				this.surveyId = surveyId;
				this.viewOptionSurvey = this.viewOptionSurveyArr[this.surveyId];
				this.getSurveyQuestions(this.surveyId);
			}else{
				var sectioncnt = $('#sectioncnt').val();
				var activeSection;
				if($('#Section_'+this.cpage).attr('style') != 'display: none;') {
					activeSection = "Section_"+this.cpage;
				}
				if(this.mandatoryCheck(activeSection)){
					var totalpage=this.scnt-1;
					this.cpage++;
					this.npage++;
					this.ppage++;			
					$("#Section_"+this.cpage).show();
					$("#Section_"+this.ppage).hide();
					if($('#bread_'+this.cpage).html() != null){
						if(this.viewOptionSurvey == 'H') {
						    $('.SurveyHLeftContent .SurveyModuleList').removeClass('createlnr-activestep');
						    $('.SurveyHLeftContent .SurveyModuleList').addClass('createlnr-prevstep');
						    $('#bread_'+this.cpage+' .SurveyModuleList').removeClass('createlnr-prevstep');
							$('#bread_'+this.cpage+' .SurveyModuleList').addClass('createlnr-activestep');
						}else {
						    $('#leftPanel .SurveyModuleList').removeClass('createlnr-activestep');
						    $('#leftPanel .SurveyModuleList').addClass('createlnr-prevstep');
						    $('#bread_'+this.cpage+' .SurveyModuleList').removeClass('createlnr-prevstep');
						    $('#bread_'+this.cpage+' .SurveyModuleList').addClass('createlnr-activestep');
						}
						
					} 
					if(this.cpage==2){
						$('#fielset').hide();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').show();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').addClass("removebutton");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
						this.chkdisableBtn();
					}
					if(this.cpage==totalpage){
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
						if(this.showPreview==0)
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').show();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').addClass("admin-save-button-middle-bg");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').before('<div class="admin-save-button-left-bg"></div>');
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').after('<div class="admin-save-button-right-bg"></div>');
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
						this.chkdisableBtn();
					}
					if(this.isPreview=='Yes')
					{
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
						$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
						this.chkdisableBtn();
					}
				}
			}
			this.getOverLayHeight();
			this.chkdisableBtn();
		 }catch(e){
				// to do
			}
		},
		movePrev:function(){
		 try{	
			var totalpage=this.scnt-1;
			this.cpage--;
			this.npage--;
			this.ppage--;
			$("#Section_"+this.cpage).show();
			$("#Section_"+this.npage).hide();
			if($('#Sec_list #bread_'+this.npage).html() != null){
				if(this.viewOptionSurvey == 'H') {
					$('.SurveyHLeftContent .SurveyModuleList').removeClass('createlnr-activestep');
					$('.SurveyHLeftContent .SurveyModuleList').addClass('createlnr-prevstep');
					$('#bread_'+this.npage).parents("td").prev("td").find("span").has('.SurveyModuleList').children().removeClass('createlnr-prevstep');
					$('#bread_'+this.npage).parents("td").prev("td").find("span").has('.SurveyModuleList').children().addClass('createlnr-activestep');
				}else{
					$('#leftPanel .SurveyModuleList').removeClass('createlnr-activestep');
					$('#leftPanel .SurveyModuleList').addClass('createlnr-prevstep');
					$('#bread_'+this.npage).parents('tr').prev().find('span').has(".SurveyModuleList").children().removeClass('createlnr-prevstep');
					$('#bread_'+this.npage).parents('tr').prev().find('span').has(".SurveyModuleList").children().addClass('createlnr-activestep');
				}
			}
			if(this.cpage==1){
				$('#fielset').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:button:eq(3)').hide();
			}
			if(this.cpage==totalpage-1){
				if(this.showPreview==0)
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:first').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').addClass("removebutton");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
				}
			this.getOverLayHeight();
			this.chkdisableBtn();
		 }catch(e){
				// to do
			}
		},
		insertLnrSurvey:function(param){
		 try{
			var current	= this;
			this.createLoader('take_survey_main');
			$.ajax({
				type: "POST",
			    url : resource.base_host+"/?q=learning/take-survey/update-survey" + '/' + this.type,
			    data: (param),
			    complete : function(xmlHttpRequest, textStatus) {
				    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
				        return;
				    }
				    var responseText = jQuery.parseJSON(xmlHttpRequest.responseText);
				    current.saveReturn(responseText);
				    $("#block-take-survey").data('surveylearner').destroyLoader('take_survey_main');
			    }
		   });
		 }catch(e){
				// to do
			}
		},
		saveReturn:function(responseText){
		 try{
			this.surveyCount--;
			var outHtml	= '';
			
			if(this.type == 'assessment'){
				var responseTextArr = responseText.split('###');
				var min_mark = responseTextArr[1];
				var max_mark = responseTextArr[2];
				var total_score = responseTextArr[3];
				var min_mark_per = responseTextArr[4];
				var total_score_per = responseTextArr[5];
				var pre_status = responseTextArr[6];
				var survey_id = responseTextArr[7];
				var title = responseTextArr[8];
				var showPreview = responseTextArr[9];
				var sts = ''; 
				if(parseInt(total_score_per) >= parseInt(min_mark_per)){
					sts = 'pass';
					outHtml	= "<div class='survey-completion-status'>"+Drupal.t("MSG396")+" <Br/><Br/>"+ +total_score_per+"%."+"</div>";
				} else {
					sts = 'fail';
					outHtml	= "<div class='survey-completion-status'>"+Drupal.t("MSG397")+' '+title+' '+Drupal.t("Assessment")+" <Br/><Br/>"+Drupal.t("MSG398")+total_score_per+"%."+"</div>";
				}
				outHtml	= outHtml;
			    this.updateScore(min_mark, max_mark, total_score, sts, pre_status,survey_id); // Update the score

				$("#"+this.uniqueWidgetId).html(outHtml);
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();
				if(showPreview != 'no'){ // show The Preview Button To Last attempts #0038540
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').show();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').addClass("removebutton");
				}
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
				
			} else if(this.surveyCount>0){
				outHtml	= "<div id='next-survey-msg' class='survey-completion-status'>"+Drupal.t("LBL1158")+" \""+unescape(this.objectTitle)+"\", "+Drupal.t("LBL1159")+"</div>";
				outHtml	= outHtml;
				$("#"+this.uniqueWidgetId).html(outHtml);
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:first').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();		
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').addClass("admin-save-button-middle-bg");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').hide();
				//$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').addClass("removebutton");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
				$('.survey-main-holder .ui-dialog').css({'width':'500px','left':((getWindowWidth()-800)/2)+132+'px'});
				$('.survey-main-holder .ui-dialog #take_survey_main').css('min-height','63px');
				$('.survey-main-holder .surveylearnear-js-assessment-survey-title-for-popup').addClass('survey-complete-popup-title'); /*Viswanathan added for #0078494*/
				$('.survey-main-holder .ui-dialog #take_survey_main .survey-completion-status').css({'height': 'auto', 'width': 'auto', 'padding-top':'0', 'padding': '20px 16px 14px'});
			}else{
				outHtml	= "<div class='survey-completion-status'>"+Drupal.t("MSG392")+"</div>";
				outHtml	= outHtml;
				$("#"+this.uniqueWidgetId).html(outHtml);
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(7)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
			}
		 }catch(e){
				// to do
			}
		},
		surveyCommit:function(){
		 try{
			var inobj={};
			var timeSpend = this.getElapsedTime();
			var Answers='';
			for(i=0;i < this.seccount;i++){
				var Questions =$('#'+i+'_ques').val(); 
				var QuestionId=$('#'+i+'_QuestionId').val();
				var QuestionType=$('#'+i+'_questiontype').val();
				var DisplayRating=$('#'+i+'_displayrating').val();		
				if(QuestionType=='sry_qtn_typ_rtg') //Rating
				{
					if(DisplayRating=="H"){
						Answers = $("input[name='"+i+"_RatingHorizontal']:checked").val();
					}else if(DisplayRating=="V"){
						Answers = $("input[name='"+i+"_RatingVertical']:checked").val();					
					}else{
						Answers = $("input[name='"+i+"_RatingHorizontal']:checked").val();
					}				
				}			
				if(QuestionType=='sry_qtn_typ_mch')  //Multiple Choice
				{	
					Answers = '';				
					if(DisplayRating=="H"){				
						$(".multiselect_"+i+"_MultiHorizontal:checked").each(function(){
						    if(Answers!=''){
								Answers += Drupal.settings.custom.EXP_AC_SEPARATOR +$(this).val();
							}else{
								 Answers = $(this).val();
							}
						    
						});				
					}else if(DisplayRating=="V"){
						$(".multiselect_"+i+"_MultiVertical:checked").each(function(){
						    if(Answers!=''){
								Answers += Drupal.settings.custom.EXP_AC_SEPARATOR +$(this).val();
							}else{
								 Answers = $(this).val();
							}
						    
						});				
					}else{
						$(".multiselect_"+i+"_MultiHorizontal:checked").each(function(){
						    if(Answers!=''){
								Answers += Drupal.settings.custom.EXP_AC_SEPARATOR+$(this).val();
							}else{
								 Answers = $(this).val();
							}					    
						});
					}					
				}
				
				if(QuestionType=='sry_qtn_typ_cmt') //Comments
				{
					Answers =$('#'+i+'_Text').val();
				}
				/*if(QuestionType=='Label')
				{
					Answers = "";
				}*/
				
				if(QuestionType=='sry_qtn_typ_dpn')  //Dropdown
				{
					Answers = $('#'+i+'_Dropdown').val();
				}			
				
				if(QuestionType=='sry_qtn_typ_yno')   //Yes/No
				{
				     Answers = $("input[name='"+i+"_yesno']:checked").val();
				}
				
				if(QuestionType=='sry_qtn_typ_trf')   //True/False
				{
				     Answers = $("input[name='"+i+"_truefalse']:checked").val();
				}	
				
				var anony=$("#surveyuser").val();
				if(anony=='MyName')	{
					var learnerId=this.lnrId;
				} else {
					var learnerId='0';
				}

				//var y	= 'ObjectId'+this.objectId+'ObjectType'+this.objectType+'LearerId'+learnerId+'SurveyId'+this.surveyId+'QuestionId'+QuestionId+'QuestionType'+QuestionType+'Question'+Questions+'QuestionAns'+Answers;
				var x = {
						'ObjectId':this.objectId,
						'ObjectType':this.objectType,
						'LearerId':learnerId,
						'SurveyId':this.surveyId,
						'QuestionId':QuestionId,
						'QuestionType':QuestionType,
						'Question':Questions,
						'QuestionAns':Answers,
						'enrollId':this.enrollId,
						'enrollId':this.enrollId,
						'PreStatus':this.preStatus
							};
	            inobj[i]=x;			 
			 }
			inobj['totalTime'] = timeSpend;
			var sectioncnt = $('#sectioncnt').val();
			var activeSection = 'Section_'+sectioncnt;
			if($('#Section_'+this.cpage).attr('style') != 'display: none;') {
				activeSection = "Section_"+this.cpage;
			}
			if(this.mandatoryCheck(activeSection)){
				// Disabling the commit functionality to prevent the multiple commits  
				if(this.committed == 0){ 
					this.committed = 1;
				} else { 
					return false; 
				}
				this.insertLnrSurvey(inobj);
		    }		
		 }catch(e){
				// to do
			}
		},
		getSurveyDetails : function() {
		 try{
			this.showPreview = 0;
			var current	= this;
			$.ajax({
				type: "GET",
			    url : resource.base_host+"/?q=learning/take-survey/get-surveys/"  + this.lnrId + '/' + this.objectId + '/' + this.objectType+ '/' + this.type + '/' + this.preStatus + '/' + this.enrollId,
			    complete : function(xmlHttpRequest, textStatus) {
				    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
				        return;
				    }
				    var responseText = jQuery.parseJSON(xmlHttpRequest.responseText);
				    /*$.each(responseText, function(i,tweet){
				    	surveyDetails[j]['surveyId']= tweet.surveyid;
				   });*/
				    current.showSurvey(responseText);
			    }
		   });
		 }catch(e){
				// to do
			}
		},
		showSurvey:function(surveyDetails){
		 try{
			var availableSurvey = 0;
			var attemptCount='';
			var resultStatus;
			var resultMsg='';
			var preCount = '';
			var asessChk = '';
			var preassessCount = '';
			var scoreConflict = '';
			var statusCnt = '';
			//var preTpassess = '';
			if(surveyDetails.length<1)
				resultMsg	= Drupal.t("MSG417");
			else{
			    $.each(surveyDetails, function(i,respData){
			    	//surveyDetails[j]['surveyId']= tweet.surveyid;
					attemptCount = respData.surveycount;
					preCount  = respData.presurveycount;
					preassessCount  = respData.preassCount;
					asessChk  = respData.asschk;
					scoreConflict = respData.scoreConflict;
					statusCnt = respData.statusCount
					//preTpassess= respData.preTpId;
					if(attemptCount!='' && attemptCount!=null && attemptCount==0) {
						availableSurvey++;
					}
		   		});
			}
			
			this.surveyCount	= availableSurvey;
			var cnt 	= surveyDetails.length;
			var outHtml = '';
			var stsHtml = '';
			var msgHtml = '';
			var obj		= this;
			var objStr = '$("#block-take-survey").data("surveylearner")';
			var afterComp	= '';
			var SComp 		= '';
			var SCnt		= 0;
			var showCloseButton	= false;
			/*-- #35887: score conflict error check --*/
			if(scoreConflict == true) {
				resultMsg = Drupal.t('MSG712');
			}
			if(resultMsg!='' && resultMsg!=null){
				outHtml +='<table cellpadding="0" width="750px" cellspacing="0" border="0" class="survey-home-table">';
				outHtml +='<tr>';
				outHtml +='<td>';
				outHtml +='<div class="SurveyStatusText">';
				outHtml += resultMsg;
				outHtml +='</div>';
				outHtml +='</td>';
				outHtml +='</tr>';
				outHtml +='</table>';
				showCloseButton = true;
				
			}else{
				if ( this.objectType == 'cre_sys_obt_trp' || this.objectType == 'cre_sys_obt_trn' || this.objectType == 'cre_sys_obt_crt' || this.objectType == 'cre_sys_obt_cur') {
					outHtml +='<table cellpadding="0" width="750px" cellspacing="0" border="0"  class="survey-home-table">';
					if( this.objectStatus !="lrn_tpm_ovr_cmp" ) {
						for(i=0;i<surveyDetails.length;i++){
							SComp = surveyDetails[i]["aftercompletion"];
							if(SComp=="Y") {
								SCnt++;
							}
						}				
					}
					if(cnt>0){
						if(cnt != SCnt) {
							outHtml +='<tr class="detail-assess-heading">';
							outHtml +='<th class="assess-heading">'+Drupal.t("Assessment")+' '+Drupal.t("LBL107")+'</th>';
							outHtml +='<th class="assess-heading">'+Drupal.t("LBL102")+'</th>';
							outHtml +='<th class="assess-heading">'+Drupal.t("LBL202")+'</th>';
							outHtml +='</tr>';
							
							//outHtml +='<div class="SurveySelectList">';
		
							//outHtml += SMARTPORTAL.t("Survey")+"&nbsp;&nbsp;:&nbsp;&nbsp;</br>";
							for(i=0;i<surveyDetails.length;i++){
								var afterComp = 'X';
								var attemptTaken = surveyDetails[i]["no_of_attempts"] - surveyDetails[i]["attempts"]; // Repeation var attemptTaken is Fixed in Code.
								var sId	= surveyDetails[i]["surveyid"];
								if(obj.objectStatus!="lrn_tpm_ovr_cmp") {
									afterComp = surveyDetails[i]["aftercompletion"];
								}
								outHtml += '<tr>';
							if(surveyDetails[i]['asschk'] == 'post-assessment'){
								if(surveyDetails[i]['preassCount'] > 0) {
								  if(surveyDetails[i]['presurveycount'] > 0){
								if(surveyDetails[i]['surveycount']>0) {
											outHtml += '<td>';
											outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"],'surveylearnear-js-assessment-survey-title')+"</span></div>";
											outHtml += '</td>';
											
											outHtml += '<td>';
											outHtml += '<span class="sur-status-label">'+Drupal.t("Completed")+'</span>';
											outHtml += '</td>';
											
											if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
												outHtml += '<td>';
												outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
												outHtml += '</td>';
										     }
											
										}else{
											outHtml += '<td>';
											outHtml += "<div class='survey-list-active'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"], 'surveylearnear-js-assessment-survey-title')+"</span></a></div>";
											outHtml += '</td>';
											
											if(surveyDetails[i]['statusCount']> 0){
												var sts = (surveyDetails[i]['compstatus'] =="lrn_tpm_ovr_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
												outHtml += '<td>';
												outHtml += '<span class="sur-status-label">'+sts+'</span>';
												outHtml += '</td>';
											}else{
												outHtml += '<td>';
												outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
												outHtml += '</td>';
											}
											if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
												outHtml += '<td>';
												outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
												outHtml += '</td>';
										    }
									   }
								 } else {
									 	msgHtml += '<div class="warningTextmsg">*'+Drupal.t("MSG708")+ ' ' +Drupal.t("MSG719");
									  //  msgHtml += "<span class='yes-link'><a href='javascript:void(0);' class='option-take'  onclick='"+objStr+".takePostAssess(this);'>"+Drupal.t("Yes")+"</a></span>";
									   // msgHtml +=  "<span class='no-link'>"+" / " + "<a href='javascript:void(0);' class='option-take' onclick='"+objStr+".closePopup(this);'>" + Drupal.t("No") + "</a></span>";
								
									 	
									    msgHtml += "<div class='ui-dialog-buttonpane ui-widget-content ui-helper-clearfix'>";
									    msgHtml += "<div class='ui-dialog-buttonset-post-assessment'><div class='white-btn-bg-left'></div>";
									    msgHtml += "<span class='yes-link'><button onclick='"+objStr+".closePopup(this);' class='removebutton white-btn-bg-middle' type='button'>" + Drupal.t("No") + "</button></span>";
									    msgHtml += "<div class='white-btn-bg-right'></div>";
									    msgHtml += "<div class='admin-save-button-left-bg'></div>";
									    msgHtml +=  "<span class='no-link'><button onclick='"+objStr+".takePostAssess(this);' class='admin-save-button-middle-bg' type='button'>"+Drupal.t("Yes")+"</button></span>";
									    msgHtml += "<div class='admin-save-button-right-bg'></div>";
									    msgHtml += "</div></div>";
									    
									    outHtml += '<td>';
									    outHtml += "<div class='post-survey-list-active' style='display:none;'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"], 'surveylearnear-js-assessment-survey-title')+"</span></a></div>";
									    outHtml += '</td>';
									    
									    if(surveyDetails[i]['statusCount']> 0 && surveyDetails[i]["attempts"] >= 1){
									    	var sts = (surveyDetails[i]['compstatus'] =="lrn_tpm_ovr_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
											outHtml += '<td>';
											outHtml += '<span class="post-sur-status-label" style="display:none;">'+sts+'</span>';
											outHtml += '</td>';
										}else{
											outHtml += '<td>';
											outHtml += '<span class="post-sur-status-label" style="display:none;">'+Drupal.t("Not yet taken")+'</span>';
											outHtml += '</td>';
										}
									    
									    if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
									    	outHtml += '<td>';
											outHtml += '<span class="post_no_of_attempts" style="display:none;">'+attemptTaken+'</span>';
											outHtml += '</td>';
								         }
									    }
							  }	else {
								  if(surveyDetails[i]['surveycount'] > 0) {
									    outHtml += '<td>';
										outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"],'surveylearnear-js-assessment-survey-title')+"</span></div>";
										outHtml += '</td>';
										
										outHtml += '<td>';
										outHtml += '<span class="sur-status-label">'+Drupal.t("Completed")+'</span>';
										outHtml += '</td>';
										
										if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
											outHtml += '<td>';
											outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
											outHtml += '</td>';
										}
									}else{
										outHtml += '<td>';
										outHtml += "<div class='survey-list-active'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"], 'surveylearnear-js-assessment-survey-title')+"</span></a></div>";
										outHtml += '</td>';
										
										if(surveyDetails[i]['statusCount']> 0){
											var sts = (surveyDetails[i]['compstatus'] =="lrn_tpm_ovr_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
											outHtml += '<td>';
											outHtml += '<span class="sur-status-label">'+sts+'</span>';
											outHtml += '</td>';
										}else{
											outHtml += '<td>';
											outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
											outHtml += '</td>';
										}
										if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
											outHtml += '<td>';
											outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
											outHtml += '</td>';
										}
									}
							  }  
							}else{
								if(surveyDetails[i]['surveycount'] > 0) {
									outHtml += '<td>';
									outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'>"+titleRestricted(surveyDetails[i]["surveytitle"],'surveylearnear-js-assessment-survey-title')+"</span></div>";
									outHtml += '</td>';
									
									outHtml += '<td>';
									outHtml += '<span class="sur-status-label">'+Drupal.t("Completed")+'</span>';
									outHtml += '</td>';
									
									if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
										outHtml += '<td>';
										outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
										outHtml += '<td>';
									}
								}else{
									outHtml += '<td>';
									outHtml += "<div class='survey-list-active'><span class='vtip' title='"+surveyDetails[i]["surveytitle"].replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(surveyDetails[i]["surveytitle"], 'surveylearnear-js-assessment-survey-title')+"</span></a></div>";
									outHtml += '</td>';
									
									if(surveyDetails[i]['statusCount']> 0 && surveyDetails[i]["attempts"] >= 1){
										var sts = (surveyDetails[i]['compstatus'] =="lrn_tpm_ovr_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
										outHtml += '<td>';
										outHtml += '<span class="sur-status-label">'+sts+'</span>';
										outHtml += '</td>';
									}else{
										outHtml += '<td>';
										outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
										outHtml += '</td>';
									}
									
									if(surveyDetails[i]['surtype'] == 'sry_det_typ_ass'){
										outHtml += '<td>';
										outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
										outHtml += '</td>';
									}
								}						
							}
								obj.viewOptionSurveyArr[sId] = surveyDetails[i]["viewoption"];
                                                                if(this.type == 'survey') {
                                                                    if(surveyDetails[i]['completionstatus'] != "lrn_tpm_ovr_cmp") {
                                                                        stsHtml = '<div class="warningText refer-note">*'+Drupal.t("MSG383")+"</div>";
                                                                    }
                                                                }
                                                }
							outHtml += "</select>";
							//outHtml +='</div>';
							//outHtml +='</ul>';
							//outHtml +='</td>';
							outHtml +='</tr>';
							//outHtml +='<tr><td style="height: 20px;">&nbsp;</td></tr>';
							/*outHtml +='<tr>';
							outHtml +='<td class="warningText">';
							outHtml += SMARTPORTAL.t("*It is recommended to take the survey after completion of the class.");
							outHtml +='</td>';
							outHtml +='</tr>';*/
							/*if(this.type != 'survey'){
								//stsHtml = '<div class="warningText">'+SMARTPORTAL.t("*It is recommended to take the assessment after completion of the class.")+"</div>";
							} else {
								stsHtml = '<div class="warningText refer-note">*'+Drupal.t("MSG383")+"</div>";
							}*/
		
						} else {
							/*outHtml +='<tr>';
							outHtml +='<td>';
							outHtml +='<div class="SurveyStatusText">';
							outHtml += SMARTPORTAL.t("You can take the survey only after completing the training program.");
							outHtml +='</div>';
							outHtml +='</td>';
							outHtml +='</tr>';*/
							if(this.type != 'survey')
								stsHtml = '<div class="SurveyStatusText refer-note">'+Drupal.t("MSG384")+"</div>";
							else
								stsHtml = '<div class="SurveyStatusText refer-note">'+Drupal.t("MSG385")+"</div>";
							showCloseButton = true;
						}
					}else{
						/*outHtml +='<tr>';
						outHtml +='<td>';
						outHtml +='<div class="SurveyStatusText">';
						outHtml += SMARTPORTAL.t("There is no survey for this training program.");
						outHtml +='</div>';
						outHtml +='</td>';
						outHtml +='</tr>';*/
						if(this.type != 'survey')
							stsHtml = '<div class="SurveyStatusText .refer-note">'+Drupal.t("MSG386")+"</div>";
						else
							stsHtml = '<div class="SurveyStatusText .refer-note">'+Drupal.t("MSG387")+"</div>";
						showCloseButton = true;
					}
		
					outHtml +='</table>';
		
				} else if (this.objectType == 'cre_sys_obt_cls') {
					outHtml +='<table cellpadding="0" width="750px" cellspacing="0" border="0" class="survey-home-table">';
					if(this.objectStatus == 'lrn_crs_reg_cnf') {
						if (cnt > 0) {
							
							outHtml +='<tr class="detail-assess-heading">';
							outHtml +='<th class="assess-heading-name">'+Drupal.t("Assessment")+' '+Drupal.t("LBL107")+'</th>';
							outHtml +='<th class="assess-heading-status">'+Drupal.t("LBL102")+'</th>';
							outHtml +='<th class="assess-heading-attempts">'+Drupal.t("LBL202")+'</th>';
							outHtml +='</tr>';
							
						    $.each(surveyDetails, function(i,respData){
								var sId	= respData.surveyid;
								if(respData.attempts){respData.attempts=respData.attempts;}else{respData.attempts=0;}
								outHtml +='<tr class="assess-column">';
								if(respData.asschk == 'post-assessment'){
									if(respData.preassCount >0) {
										if(respData.presurveycount >0){
											if(respData.surveycount>0){	
												outHtml += '<td>';	
												outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></div>";
												outHtml += '</td>';	
												
												outHtml += '<td>';
												outHtml += '<span class="sur-status-label">'+Drupal.t("Completed")+'</span>';
												outHtml += '</td>';	
																					
												if(respData.surtype == 'sry_det_typ_ass'){
													var attemptTaken = respData.no_of_attempts - respData.attempts;
													outHtml += '<td>';
													outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
													outHtml += '</td>';
												}
											}else{
												outHtml += '<td>';	
												outHtml += "<div class='survey-list-active'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></a></div>";									
												outHtml += '</td>';	
												if(respData.statusCount > 0 && respData.attempts >= 1){
													var sts = (respData.compstatus =="lrn_crs_cmp_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
													outHtml += '<td>';
													outHtml += '<span class="sur-status-label">'+sts+'</span>';
													outHtml += '</td>';
												}else{
													outHtml += '<td>';
													outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
													outHtml += '</td>';
												}
												if(respData.surtype == 'sry_det_typ_ass'){	
													var attemptTaken = respData.no_of_attempts - respData.attempts;	
													outHtml += '<td>';
													outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
													outHtml += '</td>';
												}
											}
									   } else{
										    msgHtml += '<div class="warningTextmsg">*'+Drupal.t("MSG708")+ ' ' +Drupal.t("MSG719");
										    
										    msgHtml += "<div class='ui-dialog-buttonpane ui-widget-content ui-helper-clearfix'>";
										    msgHtml += "<div class='ui-dialog-buttonset-post-assessment'><div class='white-btn-bg-left'></div>";
										    msgHtml += "<span class='yes-link'><button onclick='"+objStr+".closePopup(this);' class='removebutton white-btn-bg-middle' type='button'>" + Drupal.t("No") + "</button></span>";
										    msgHtml += "<div class='white-btn-bg-right'></div>";
										    msgHtml += "<div class='admin-save-button-left-bg'></div>";
										    msgHtml +=  "<span class='no-link'><button onclick='"+objStr+".takePostAssess(this);' class='admin-save-button-middle-bg' type='button'>"+Drupal.t("Yes")+"</button></span>";
										    msgHtml += "<div class='admin-save-button-right-bg'></div>";
										    msgHtml += "</div></div>";
										    
										    
						
										    
										    
										    outHtml += "<td>";
										    outHtml += "<div class='post-survey-list-active' style='display:none'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></a></div>";									
										    outHtml += "</td>";
										    
										    if(respData.statusCount > 0 && respData.attempts >= 1){
										    	var sts = (respData.compstatus =="lrn_crs_cmp_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
												outHtml += '<td>';
												outHtml += '<span class="post-sur-status-label" style="display:none">'+sts+'</span>';
												outHtml += '</td>';
											}else{
												outHtml += '<td>';
												outHtml += '<span class="post-sur-status-label" style="display:none">'+Drupal.t("Not yet taken")+'</span>';
												outHtml += '</td>';
											}
										    if(respData.surtype == 'sry_det_typ_ass'){	
												var attemptTaken = respData.no_of_attempts - respData.attempts;	
												outHtml += "<td>";
												outHtml += '<span class="post_no_of_attempts" style="display:none;">'+attemptTaken+'</span>';
												outHtml += "</td>";
											}
											
									   }
									}else {
								if(respData.surveycount>0){
									 		outHtml += '<td>';
											outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></div>";
											outHtml += '</td>';
											
											outHtml += '<td>';
											outHtml += "<span class='sur-status-label'>"+Drupal.t('Completed')+"</span>";	
											outHtml += '</td>';
											
											if(respData.surtype == 'sry_det_typ_ass'){
												var attemptTaken = respData.no_of_attempts - respData.attempts;	
												outHtml += '<td>';
												outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
												outHtml += '</td>';
											}
										}else{
											outHtml += '<td>';
											outHtml += "<div class='survey-list-active'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></a></div>";									
											outHtml += '</td>';
											if(respData.statusCount > 0){
												var sts = (respData.compstatus =="lrn_crs_cmp_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
												outHtml += '<td>';
												outHtml += '<span class="sur-status-label">'+sts+'</span>';
												outHtml += '</td>';
											}else{
												outHtml += '<td>';
												outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
												outHtml += '</td>';
											}
											if(respData.surtype == 'sry_det_typ_ass'){	
												var attemptTaken = respData.no_of_attempts - respData.attempts;	
												outHtml += '<td>';
												outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
												outHtml += '</td>';
											}
											
										}
									}		
									
								} else {
									if(respData.surveycount>0){
										outHtml += '<td>';
										outHtml += "<div class='survey-list-completed'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></div>";
										outHtml += '</td>';
										
										outHtml += '<td>';
										outHtml += '<span class="sur-status-label">'+Drupal.t("Completed")+'</span>';
										outHtml += '</td>';								
									if(respData.surtype == 'sry_det_typ_ass'){
										var attemptTaken = respData.no_of_attempts - respData.attempts;
										    outHtml += '<td>';
											outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
											outHtml += '</td>';
									}
								}else{  
									  outHtml += '<td>';
									  outHtml += "<div class='survey-list-active'><span class='vtip' title='"+respData.surveytitle.replace(/'/g, "&apos;")+"'><a href='javascript:void(0);' class='enrollment-survey-list' onclick='"+objStr+".moveNext("+sId+");'>"+titleRestrictionFadeoutImage(respData.surveytitle,'surveylearnear-js-assessment-survey-title')+"</span></a></div>";
									  outHtml += '</td>';
									  if(respData.statusCount > 0 && respData.attempts >= 1){
										  	var sts = (respData.compstatus =="lrn_crs_cmp_cmp") ? Drupal.t("Completed") : Drupal.t("In progress");
											outHtml += '<td>';
											outHtml += '<span class="sur-status-label">'+sts+'</span>';
											outHtml += '</td>';
										}else{
											outHtml += '<td>';
											outHtml += '<span class="sur-status-label">'+Drupal.t("Not yet taken")+'</span>';
											outHtml += '</td>';
										}
									  if(respData.surtype == 'sry_det_typ_ass'){	
										var attemptTaken = respData.no_of_attempts - respData.attempts;	
										outHtml += '<td>';
										outHtml += '<span class="no_of_attempts">'+attemptTaken+'</span>';
										outHtml += '</td>';
									}
								}
									
								}
								
								obj.viewOptionSurveyArr[sId] = respData.viewoption;
                                                                if(asessChk != 'post-assessment' && asessChk != 'pre-assessment'){
                                                                    if(surveyDetails[i]['completionstatus'] != "lrn_crs_cmp_cmp"){
                                                                        stsHtml = '<div class="warningText">*'+Drupal.t("MSG383")+"</div>";
                                                                    }
                                                                }
					   		});
						 
							outHtml +='</tr>';
							//outHtml +='<tr><td style="height: 20px;">&nbsp;</td></tr>';
							/*outHtml +='<tr>';
							outHtml +='<td class="warningText">';
							outHtml += SMARTPORTAL.t("*It is recommended to take the survey after completion of the class.");
							outHtml +='</td>';
							outHtml +='</tr>';*/
							/*if(this.type != 'survey'){
								//stsHtml = '<div class="warningText">'+SMARTPORTAL.t("*It is recommended to take the assessment after completion of the class.")+"</div>";
							} else {
								stsHtml = '<div class="warningText">*'+Drupal.t("MSG383")+"</div>";
							}*/
						}				
						else {
							/*outHtml +='<tr>';
							outHtml +='<td>';
							outHtml +='<div class="SurveyStatusText">';
							outHtml += SMARTPORTAL.t("Presently, there are no surveys attached.");
							outHtml +='</div>';
							outHtml +='</td>';
							outHtml +='</tr>';*/
							if(this.type != 'survey')
								stsHtml = '<div class="SurveyStatusText">'+Drupal.t("MSG388")+"</div>";
							else
								stsHtml = '<div class="SurveyStatusText">'+Drupal.t("MSG389")+"</div>";
							showCloseButton = true;
						}
					}
					else {
						/*outHtml +='<tr>';
						outHtml +='<td>';
						outHtml +='<div class="SurveyStatusText">';
						outHtml += SMARTPORTAL.t("You can take the survey only after you have registered for the class and your registration is confirmed.");
						outHtml +='</div>';
						outHtml +='</td>';
						outHtml +='</tr>';*/
						if(this.type != 'survey')
							stsHtml = '<div class="SurveyStatusText">'+Drupal.t("MSG390")+"</div>";
						else
							stsHtml = '<div class="SurveyStatusText">'+Drupal.t("MSG391")+"</div>";
						showCloseButton = true;
					}
					outHtml +='</table>';
				}
			}
			
			outHtml = outHtml;
			 this.destroyLoader(this.loaderDiv);
			 $('#'+this.uniqueWidgetId).dialog("open");
			 $.ui.surveylearner.prototype.refreshSession();			 
			 this.currTheme = Drupal.settings.ajaxPageState.theme;
			 if(this.currTheme == "expertusoneV2"){
		  	   changeDialogPopUI();
			 }
			
			
			 $('#'+this.uniqueWidgetId).html(outHtml);

				if((asessChk == 'post-assessment' && (preassessCount > 0) && (preCount >0)) || (asessChk == 'post-assessment' && preassessCount == 0) || (asessChk == 'pre-assessment')){
					$('.detail-assess-heading').show();
					$('.detail-assess-heading').closest(".survey-home-table").addClass('pre-post-assessment');
				}else{
					$('.detail-assess-heading').hide();
					$('.survey-home-table').css('border','none');
					$('.detail-assess-heading').closest(".survey-home-table").removeClass('pre-post-assessment');
				}
			
			//if(preCount == 0) {
				//$('#'+this.uniqueWidgetId).html(outHtml);
				$('#'+this.uniqueWidgetId).append(msgHtml);
				//$('.survey-home-table').html(msgHtml);
			//}
				
			vtip();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
			if(asessChk != 'post-assessment'){
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
			$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();			
			  $('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
			  $('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();			  
			}
			if (showCloseButton == true) {
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
			}
			$('.ui-dialog-buttonpane').after(stsHtml);
			
			$('.survey-main-holder .ui-dialog').css({'width':'800px','left':((getWindowWidth()-800)/2)+'px'});
			$('.survey-main-holder .ui-dialog #take_survey_main').css('min-height','113px');
			$('.survey-main-holder .expertusV2PopupContainer .refer-note').css('width','97.5%');
		 }catch(e){
				// to do
			}
		},
		
		takePostAssess :  function(){
			try{ 
				this.currTheme = Drupal.settings.ajaxPageState.theme;
				$('.warningTextmsg').hide();
				$('.post-survey-list-active').css('display','block');
				$('.post_no_of_attempts').css('display','block');
				$('.detail-assess-heading').show();
				$('.post-sur-status-label').css('display','block');
				$('.survey-home-table').css('border-left','1px solid #e5e5e5');
				$('.survey-home-table').css('border-right','1px solid #e5e5e5');
				$('.survey-home-table').css('border-bottom','1px solid #e5e5e5');
				$('.detail-assess-heading').closest(".survey-home-table").addClass('pre-post-assessment');
				if(this.currTheme != "expertusoneV2"){
				  $('.survey-home-table').css('border','1px solid #bbb');
				}
				//$("#"+this.uniqueWidgetId).html(outHtml);
			    
		           
			}catch(e){
				// to do
			}
		},
		closePopup :  function(){
			try{ 
				$("#"+this.uniqueWidgetId).remove();
		           
			}catch(e){
				// to do
			}
		},
		getSurveyQuestions:function(surveyId){
		 try{
			//var loaderObj	= this.getUniqueWidgetId();
			var userId 	= this.isPreview=='Yes'?"-1":this.lnrId;
			var reqData	= {'SurveyID':surveyId,'UserID':userId,'ObjectId':this.objectId,'ObjectType':this.objectType,'ShowPreview':this.showPreview};
			var param	={'isPreview':this.isPreview};
			var current	= this;
			$.ajax({
				type: "POST",
			    url : resource.base_host+"/?q=learning/take-survey/get-survey-questions",
			    data: (reqData),
			    complete : function(xmlHttpRequest, textStatus) {
				    if((parseInt(xmlHttpRequest.status) != 0) && (parseInt(xmlHttpRequest.status / 100) != 2)) {
				        return;
				    }
				    current.startTime = new Date();
				    var responseText = jQuery.parseJSON(xmlHttpRequest.responseText);
				    current.showQuestions(responseText,param);
			    }
		    });
		 }catch(e){
				// to do
			}
		},
		
		getElapsedTime:function(){
			var currentTime = new Date();
			var elapsed_ms = currentTime - this.startTime;
			var seconds = Math.round(elapsed_ms / 1000);
			var minutes = Math.round(seconds / 60);
			var hours = Math.round(minutes / 60);

			var sec = TrimSecondsMinutes(seconds);
			var min = TrimSecondsMinutes(minutes);
			return hours+"."+min+"."+sec;
		},
		
		updateScore : function(min_mark, max_mark, total_score, sts, pre_status,survey_id){
		 try{	
			this.currTheme = Drupal.settings.ajaxPageState.theme;
		
			// Certificate 		-> cre_sys_obt_crt
			// Curricula 		-> cre_sys_obt_cur
			// Learning plan 	-> cre_sys_obt_trn 
			// Class 			-> cre_sys_obt_cls

			// For Class 
			// Value - 0 => Lesson ID, 1 => Enroll ID, 2 => Course ID, 3 => class ID
			
			// For Certificate, Curricula and Learning plan 
			// Value - 0 => Program Id, 1 => Master Enrollment Id
			
			var paramsUpdScore = this.paramsUpdScore; // Value - 0 => Lesson ID, 1 => Enroll ID, 2 => Course ID, 3 => class ID
			var paramsUpdScoreArray = paramsUpdScore.split("###");
			
	        var sestime					= this.getElapsedTime();
	        var courseComplitionStatus 	= '';
	        var stid 					= this.lnrId;
	        var contentType				= '';
	        var contentVersion			= '';
	        var result					= '';
	        var status 					= 'Completed';
	        var scmax 					= max_mark;
	        var scmin 					= min_mark;
	        var score 					= total_score;
	        var sestimear 				= '';
	        var loc 					= '';
	        var compstatus				= '';
	        var sts						= sts;
	        var objectType				= this.objectType;

	        // Learner failed in classes should be moved to In-Complete state
	        var xstatus					= 'lrn_crs_cmp_cmp';
	        if(sts == 'fail'){
	        	xstatus	= 'lrn_crs_cmp_inc';
	        	status = 'failed';
	        }
	        var crid					= '';
	        var clid					= '';
	        var lesid					= '';
	        var enrollid				= '';
	        var regId;
			if(this.objectType == 'cre_sys_obt_cls'){
		        crid					= paramsUpdScoreArray[2];
		        clid					= paramsUpdScoreArray[3];
		        lesid					= paramsUpdScoreArray[0];
		        enrollid				= paramsUpdScoreArray[1];
		        regId					= paramsUpdScoreArray[1];
		        var versionid				= 0; //paramsUpdScoreArray[4];
			} else {
		        crid					= '';
		        clid					= '';
		        lesid					= paramsUpdScoreArray[0];
		        enrollid				= paramsUpdScoreArray[1];
		        regId					= paramsUpdScoreArray[1];        
			}
			
	        var masterenrollmentid 		= 
	        $('#lanchlinks1_'+clid+'').click();
	        
	        var launchData	=	{
	        		'regid'		: regId,
	                'stid'		: stid,
	                'classid'	: clid,
	                'courseid'	: crid,
	                'lessonid'	: lesid,
	                'versionid' : versionid,
	                'status'	: xstatus,
	                'max'		: scmax,
	                'min'		: scmin,
	                'score'		: score,
	                'sestime'	: sestime,
	                'location'	: loc,
	                'enrollid'	: enrollid,
	                'courseComplitionStatus' : courseComplitionStatus,
	    	        'contentstatus'  : status,
	    	        'objecttype' : objectType,
	    	        'calltype' : 'assessment',
	    	        'prestatus' : pre_status,
	    	        'surveyid' : survey_id
	          };
	        
	        /*
	        var passUrl = 'launch&regid='+regId+'&stid='+stid+'&classid='+clid+'&courseid='+crid+'&lessonid='+lesid+'&status='+xstatus+'&max='+scmax+'&min='+scmin;
	        passUrl = passUrl+'&score='+score+'&grade='+grade+'&sestime='+sestime+'&location='+loc+'&enrollid='+enrollid+'&courseComplitionStatus='+courseComplitionStatus;
	        passUrl = passUrl+'&contentstatus='+status+'&objecttype='+objectType+'&calltype=assessment';
	        */
	        var url='';

        	url = resource.base_url+'/?q='+'ajax/update-launch/launch';
        	
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				data:  launchData,
				success: function(result){
						for(i=0;i<result.length;i++){
							if(result[i].ID == parseInt(lesid)){
								$("#"+lesid+"_"+regId+"attempts_left").html(result[i].contValidateMsg);
								if(result[i].AttemptLeft == 0){		
									if(document.getElementById('learner-enrollment-tab-inner')) {
										// $("#paintEnrollmentResults").trigger("reloadGrid");
										if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
											$("#paintEnrollmentResults").trigger("reloadGrid");
										}
									}
									if(document.getElementById('learningplan-tab-inner')) {
										if (typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
											$("#paintEnrollmentLPResults").trigger("reloadGrid");
										}
									}
								}
							}
						}
						if(document.getElementById('learner-enrollment-tab-inner')) {
							//$("#paintEnrollmentResults").trigger("reloadGrid");
							if($("#learner-maincontent_tab3 ul li.selected a").attr('id') == "EnrollCompleted"){
								$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_cmp","EnrollCompleted");
							}else{
							$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment("lrn_crs_cmp_enr|lrn_crs_cmp_inp","Enrollmentpart");
						   }
						}
						if(document.getElementById('learningplan-tab-inner')) {
							//$("#paintEnrollmentLPResults").trigger("reloadGrid");
							if($("#learningplan-maincontent ul li.selected a").attr('id') == "EnrollLPCompleted"){
								$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_cmp","EnrollLPCompleted");
							}else{
							$("#learningplan-tab-inner").data("learningplan").callLearningPlan("lrn_tpm_ovr_enr|lrn_tpm_ovr_inp","EnrollmentLP");
							}
						}
						if(document.getElementById('instructor-tab-inner')) {
							$("#instructor-tab-inner").data("instructordesk").callInstructor("scheduled");
						}
						if(xstatus == "lrn_crs_cmp_cmp" && document.getElementById("my_transcript_jqgrid")) {
							$("#my_transcript_jqgrid").trigger('reloadGrid');
						}
				}
		    });      
			this.paramsUpdScore = '';
		 }catch(e){
				// to do
			}
		},		
		
		arrayShuffle:function(oldArray) {
		 try{
			var newArray = oldArray.slice();
		 	var len = newArray.length;
			var i = len;
			 while (i--) {
			 	var p = parseInt(Math.random()*len);
				var t = newArray[i];
		  		newArray[i] = newArray[p];
			  	newArray[p] = t;
		 	}
			return newArray; 
		 }catch(e){
				// to do
			}
		},
		
		showQuestions:function(responseText,param){
		 try{	
			var SurveryList =0;
			var ServeyArr = new Array();
			var vhtml='';
			var html='';
			var obj=this;
			var mandatory;
			var sectioncnt 		= 0;
			var alreadyAttempt	= 'No';
			var preStatus;
			var questionCount	= 0;
			var groups;
			var questionPerPage = 0;  
			var qustionPerPage 	= 0;

			if(obj.viewOptionSurvey=="H") {
				var width="80%";
			}else{
				var width="75%";
			}
			
			$(".SurveyStatusText, .warningText").remove();
			$.each(responseText, function(i,respData){
				alreadyAttempt 	= respData.Attempt;
				preStatus 	= respData.PreStatus;				
				questionPerPage = parseInt(respData.QuestionPerPage); 
				qustionPerPage  = questionPerPage;
				sectioncnt 		= respData.Groups.length;
				groups	= respData.Groups;
				$.each(groups, function(i,groupData){
					questionCount	+=groupData.Questions.length;
				});
	    });			
			/*if(alreadyAttempt== 'Yes' && this.showPreview == 0 ){
				if(this.surveyCount>0){
					if(this.type != 'survey')
						vhtml +="<div class='SurveyStatusText survey-status-msg' style='width:750px;'>"+SMARTPORTAL.t("There are more assessments attached to the class \""+unescape(this.objectTitle)+"\", do you want to continue with the next assessment?")+"</div>";
					else
						vhtml +="<div class='SurveyStatusText survey-status-msg' style='width:750px;'>"+Drupal.t("LBL1158")+" \""+unescape(this.objectTitle)+"\", "+Drupal.t("LBL1159")+"</div>";
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').hide();		
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(3)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').show();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(4)').addClass("removebutton");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').show();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(2)').addClass("removebutton");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
					this.chkdisableBtn();
					}else{
					if(this.type != 'survey')
						vhtml +="<div class='SurveyStatusText survey-status-msg' style='width:750px;'>"+Drupal.t("MSG393")+"</div>";
					else 
						vhtml +="<div class='SurveyStatusText survey-status-msg' style='width:750px;'>"+Drupal.t("MSG394")+"</div>";
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
					$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
					this.chkdisableBtn();
				}
			}
			else*/ if(sectioncnt>0 && questionCount>0)  {
				if(obj.viewOptionSurvey == "H") {	 //-- View H --
			     vhtml +='<div class="SurveyContent" style="width:750px; margin: 0px auto;">';
					 vhtml +='<div class="SurveyHLeftContent">';
					 vhtml +='<table id="Sec_list" class="survey-list" border="0" cellspacing="0" cellpadding="0"></table></div>';
				}
				else {  //-- View V --
					if(obj.currTheme == "expertusoneV2"){
					 vhtml +='<div class="SurveyContent" style="width:750px;">';					 
					 vhtml +='<table border="0" cellpadding="0" cellspacing="0" style="width: 100%; table-layout: fixed"><tr>';
					 vhtml +='<td width="23%" valign="top" align="left" id="leftPanel">';
					 vhtml +='<div class="SurveyVLeftContent"><table id="Sec_list" class="survey-list" border="0" cellspacing="0" cellpadding="0"></table></div>';
					 vhtml +='</td><td width="77%" class="SurveyRightBorder" valign="top" align="left" id="rightPanel">';
					}else{
					 vhtml +='<div class="SurveyContent" style="width:750px;">';					 
					 vhtml +='<table border="0" cellpadding="0" cellspacing="0" style="width: 100%; table-layout: fixed"><tr>';
					 vhtml +='<td width="25%" valign="top" align="left" id="leftPanel">';
					 vhtml +='<div class="SurveyVLeftContent"><table id="Sec_list" class="survey-list" border="0" cellspacing="0" cellpadding="0"></table></div>';
					 vhtml +='</td><td width="75%" class="SurveyRightBorder" valign="top" align="left" id="rightPanel">';	
					}
				}
				this.scnt 		= 1;
				this.seccount	= 0; 
				var rcnt=1;
				$.each(groups, function(i,respData){
					var qcnt=1;
					var recid = 1;
					var questionLength = respData.Questions.length;
					//-- View ---------
					questionPerPage = ((qustionPerPage <= 0) || (qustionPerPage == undefined) || (qustionPerPage > questionLength)) ? questionLength : qustionPerPage;

					for(var qt = 0; qt < questionLength; qt += questionPerPage){
						var SectionTitle = respData.Title;
						if(obj.viewOptionSurvey=="H") {  //-- View ---------
							 if(obj.scnt==1) 
							 {
								$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
								if(param.isPreview=="No"){							   
									$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').show();
									$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').addClass("admin-save-button-middle-bg");
									$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
									$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
									$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').before('<div class="admin-save-button-left-bg"></div>');
									$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').after('<div class="admin-save-button-right-bg"></div>');
								}
								html  +='<td align="left" valign="top"><div class="ModuleList"><span id="bread_'+obj.scnt+'" ><span class="SurveyModuleList createlnr-activestep">'+rcnt+'</span></span><span class="SurveyModuleTitle">'+SectionTitle+'</span></div></td>';
								vhtml +='<div id="Section_'+obj.scnt+'" class="SurveyHRightContent">';
								vhtml +='<div class="SurveyHeaderTitle" class="spotlight-item-title">'+SectionTitle+'</div>';
								vhtml +='<div id="Ratingtype"></div>';
								vhtml +='<table class="surveycontent-qatbl" border="0" cellpadding="0" cellspacing="0" width="100%;" style="tbl-survey-question">';
								rcnt++;
							 }else{
								$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').show();
								$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').addClass("removebutton");
								$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
								$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
								$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
								$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
								$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
								$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
								$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
								//this.chkdisableBtn();
													
								if(param.isPreview=="No")	{							   
									$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
								}							  
								if(SectionTitle != SectionTitleTmp){
									html +='<td align="left" valign="top"><div class="ModuleList"><span id="bread_'+obj.scnt+'" ><span class="SurveyModuleList createlnr-prevstep">'+rcnt+'</span></span><span class="SurveyModuleTitle">'+SectionTitle+'</span></div></td>';
									rcnt++;
								}
								vhtml +='<div id="Section_'+obj.scnt+'" class="SurveyRightContent_1 SurveyHRightContent">';  
								vhtml +='<div class="SurveyHeaderTitle" class="spotlight-item-title">'+SectionTitle+'</div>';
								vhtml +='<table class="surveycontent-qatbl" border="0" cellpadding="0" cellspacing="0" width="100%;" style="tbl-survey-question">';
							 }
							 obj.scnt++;
							 						 
					   }else{  //-- View ---------
							 if(obj.scnt==1) 
							 {
								   $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
								   if(param.isPreview=="No"){							   
									   $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').show();
									   $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').addClass("admin-save-button-middle-bg");
										$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
										$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
										$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').before('<div class="admin-save-button-left-bg"></div>');
										$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').after('<div class="admin-save-button-right-bg"></div>');
										$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
										$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
										$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
										$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
										$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
										
								   }
								   html +='<tr><td><div class="ModuleList"><span id="bread_'+obj.scnt+'" ><span class="SurveyModuleList createlnr-activestep">'+rcnt+'</span></span><span class="SurveyModuleTitle">'+SectionTitle+'</span></div></td></tr>';
								   vhtml +='<div id="Section_'+obj.scnt+'" class="SurveyVRightContent">';
								   vhtml +='<div class="SurveyHeaderTitle" class="spotlight-item-title">'+SectionTitle+'</div>';
								   vhtml +='<div id="Ratingtype"></div>';				  
								   vhtml +='<table class="surveycontent-qatbl" border="0" cellpadding="0" cellspacing="0" width="100%">';
								   rcnt++;
							 }
							 else
							 {
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').show();
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').addClass("removebutton");
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
								  $('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
								  
									
									 if(param.isPreview=="No"){
									  $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
								  }						  
								  if(SectionTitle != SectionTitleTmp){
									  html +='<tr><td><div class="ModuleList"><span id="bread_'+obj.scnt+'" ><span class="SurveyModuleList createlnr-prevstep">'+rcnt+'</span></span><span class="SurveyModuleTitle">'+SectionTitle+'</span></div></td></tr>';
									  rcnt++;
								  }
								  vhtml +='<div id="Section_'+obj.scnt+'" class="SurveyRightContent_1 SurveyVRightContent">';
								  vhtml +='<div class="SurveyHeaderTitle" class="spotlight-item-title">'+SectionTitle+'</div>';
								  vhtml +='<table class="surveycontent-qatbl" border="0" cellpadding="0" cellspacing="0" width="100%;"  style="tbl-survey-question">';
							 }	
					      obj.scnt++;
					   	}  
						var SectionTitleTmp = SectionTitle;
						if(respData.surveytype == 'sry_det_typ_ass'){
							vhtml +='<input type="hidden" id="preStatus" value="'+preStatus+'">';
						}						
						for(var qtPg = qt; (qtPg < questionLength && qtPg < qt + questionPerPage); qtPg++){
							this.currTheme = Drupal.settings.ajaxPageState.theme;
							var QuestionType = respData.Questions[qtPg].QuestionType;
							var Question = changeStirngsChars(respData.Questions[qtPg].QuestionTxt);
							var Answers = changeStirngsChars(respData.Questions[qtPg].AnswerTxt);
							var Attr3 = respData.Questions[qtPg].QuestionSeqNo;
							var QuestionIds = respData.Questions[qtPg].ID;					
							var MandatoryOption = respData.Questions[qtPg].IsMandatory;						
							var DisplayRating = respData.Questions[qtPg].DisplayRating;
							var RightAnswer = respData.Questions[qtPg].RightAnswer;
							var AnswerGiven = respData.Questions[qtPg].AnswerGiven;
							var AnswerStatus = respData.Questions[qtPg].AnswerStatus;
							
							if (AnswerGiven != ""){
								RightAnswer = changeStirngsChars(RightAnswer);
								AnswerGiven = changeStirngsChars(AnswerGiven);
							}
							if(MandatoryOption=="Y") {
								mandatory = "mandatory";
							}else{
								mandatory = "";
							}
						
							if(Answers != "") {
								AnsSplit = Answers.split("##");
								if(respData.surveytype == "sry_det_typ_ass" && (respData.random == "sry_det_typ_ass_ran_ans" || respData.random == "sry_det_typ_ass_ran_bot")) {
									AnsSplit = obj.arrayShuffle(AnsSplit);
								}

							}else{
								var AnsSplit="";
							}	
							
							var counte=obj.seccount++;
							var quescnt=qcnt++;

							var maxColspan = 6;						

							
							if(QuestionType=='sry_qtn_typ_rtg'){  // Rating
								
								if(DisplayRating=='H'){
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

									vhtml +='<tr><td class="SurveyQuestionField">';
									
									vhtml +='<table cellpadding="0" cellspacing="0" border="0" class="sq-rating-h">';
									for (i=0;i < AnsSplit.length;i++){
										vhtml +='<tr><td><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
									}		
									/*vhtml +='</tr><tr>';
									
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<td class="surveyQuestionChkBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'"></td>';
									}
									vhtml +='</tr></table>';*/
									vhtml +='</table>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_RatingHorizontal" value="">';

									vhtml +='<div id="'+counte+'_RatingHorizontal_radio"></div>';
									
									vhtml +='</td></tr>';
									}else{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

										vhtml +='<tr><td class="SurveyQuestionField">';
										
										vhtml +='<table cellpadding="0" cellspacing="0" border="0" class="sq-rating-h">';
										for (i=0;i < AnsSplit.length;i++){
											vhtml +='<tr><td><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
										}		
										/*vhtml +='</tr><tr>';
										
										for (i=0;i<AnsSplit.length;i++){
										vhtml +='<td class="surveyQuestionChkBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'"></td>';
										}
										vhtml +='</tr></table>';*/
										vhtml +='</table>';
										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_RatingHorizontal" value="">';

										vhtml +='<div id="'+counte+'_RatingHorizontal_radio"></div>';
										
										vhtml +='</td></tr>';	
									}
								}else if(DisplayRating=='V'){
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-rating-v">';
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<tr valign="top"><td class="surveyQuestChkVBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingVertical" id="'+counte+'_RatingVertical" value="'+AnsSplit[i]+'"></td><td class="SurveyQuesChkField">'+AnsSplit[i]+'</td></tr>';
									}
									vhtml +='</table>';

									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_RatingVertical" value="">';

									vhtml +='<div id="'+counte+'_RatingVertical_radio"></div>';
									
									vhtml +='</td></tr>';
									}else{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

										vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-rating-v">';
										for (i=0;i<AnsSplit.length;i++){
										vhtml +='<tr valign="top"><td class="surveyQuestChkVBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingVertical" id="'+counte+'_RatingVertical" value="'+AnsSplit[i]+'"></td><td class="SurveyQuesChkField">'+AnsSplit[i]+'</td></tr>';
										}
										vhtml +='</table>';

										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_RatingVertical" value="">';

										vhtml +='<div id="'+counte+'_RatingVertical_radio"></div>';
										
										vhtml +='</td></tr>';
									}
								}else{
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-rating-n">';
									for (i=0;i< AnsSplit.length;i++){
										vhtml +='<tr><td><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
									}		
									/*vhtml +='</tr><tr>';
									
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<td class="surveyQuestionChkBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'"></td>';
									}
									vhtml +='</tr></table>';*/
									vhtml +='</table>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_RatingHorizontal" value="">';
									
									vhtml +='</td></tr>';
									}else{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
										
										vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-rating-n">';
										for (i=0;i < AnsSplit.length;i++){
											vhtml +='<tr><td><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
										}		
										/*vhtml +='</tr><tr>';
										
										for (i=0;i<AnsSplit.length;i++){
										vhtml +='<td class="surveyQuestionChkBox"><input class="'+mandatory+'" type="radio" name="'+counte+'_RatingHorizontal" id="'+counte+'_RatingHorizontal" value="'+AnsSplit[i]+'"></td>';
										}
										vhtml +='</tr></table>';*/
										vhtml +='</table>';
										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_RatingHorizontal" value="">';
										
										vhtml +='</td></tr>';
									}
								}
							}						
							
							if(QuestionType=='sry_qtn_typ_mch'){  //Multiple Choice
								
								if(DisplayRating=='H'){
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-h">';
									for (i=0;i < AnsSplit.length;i++){
										vhtml +='<tr><td><div class="checkbox-unselected"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'" onclick="checkboxSelectedUnselectedCommon(this);"></div>'+AnsSplit[i]+'</td></tr>';
									}		
									/*vhtml +='</tr><tr>';
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<td class="surveyQuestionChkBox"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'"></td>';
									}
									vhtml +='</tr></table>';
									*/
									vhtml +='</table>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_MultiHorizontal" value="">';

									vhtml +='<div id="'+counte+'_MultiHorizontal_checkbox"></div>';
									
									vhtml +='</td></tr>';
									}else
									{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
										
										vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-h">';
										for (i=0;i < AnsSplit.length;i++){
											vhtml +='<tr><td><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
										}		
										/*vhtml +='</tr><tr>';
										for (i=0;i<AnsSplit.length;i++){
										vhtml +='<td class="surveyQuestionChkBox"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'"></td>';
										}
										vhtml +='</tr></table>';
										*/
										vhtml +='</table>';
										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_MultiHorizontal" value="">';

										vhtml +='<div id="'+counte+'_MultiHorizontal_checkbox"></div>';
										
										vhtml +='</td></tr>';	
									}
								}else if(DisplayRating=='V'){
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-v">';
									
									for (i=0;i < AnsSplit.length;i++){
									vhtml +='<tr valign="top"><td class="surveyQuestChkVBox"><div class="checkbox-unselected"><input type="checkbox" class="multiselect_'+counte+'_MultiVertical '+mandatory+'" name="'+counte+'_MultiVertical" id="'+counte+'_MultiVertical" value="'+AnsSplit[i]+'" onclick="checkboxSelectedUnselectedCommon(this);"></div></td><td class="SurveyQuesChkField">'+AnsSplit[i]+'</td></tr>';
									}
									vhtml +='</table>';

									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_MultiVertical" value="">';

									vhtml +='<div id="'+counte+'_MultiVertical_checkbox"></div>';
									
									vhtml +='</td></tr>';
									}else{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
										
										vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-v">';
										
										for (i=0;i < AnsSplit.length;i++){
										vhtml +='<tr valign="top"><td class="surveyQuestChkVBox"><input type="checkbox" class="multiselect_'+counte+'_MultiVertical '+mandatory+'" name="'+counte+'_MultiVertical" id="'+counte+'_MultiVertical" value="'+AnsSplit[i]+'"></td><td class="SurveyQuesChkField">'+AnsSplit[i]+'</td></tr>';
										}
										vhtml +='</table>';

										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_MultiVertical" value="">';

										vhtml +='<div id="'+counte+'_MultiVertical_checkbox"></div>';
										
										vhtml +='</td></tr>';
									}
								}else {
									if(obj.currTheme == "expertusoneV2"){ 
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-n">';
									for (i=0;i < AnsSplit.length;i++){
										vhtml +='<tr><td><div class="checkbox-unselected"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'" onclick="checkboxSelectedUnselectedCommon(this);"></div>'+AnsSplit[i]+'</td></tr>';
									}		
									/*vhtml +='</tr><tr>';
									
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<td class="surveyQuestionChkBox"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'"></td>';
									}
									vhtml +='</tr></table>';*/
									vhtml +='</table>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_MultiHorizontal" value="">';

									vhtml +='<div id="'+counte+'_MultiHorizontal_checkbox"></div>';
									
									vhtml +='</td></tr>';
									}else{
										vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
										
										vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';

										vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0" class="sq-multichoice-n">';
										for (i=0;i < AnsSplit.length;i++){
											vhtml +='<tr><td><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</td></tr>';
										}		
										/*vhtml +='</tr><tr>';
										
										for (i=0;i<AnsSplit.length;i++){
										vhtml +='<td class="surveyQuestionChkBox"><input type="checkbox" class="multiselect_'+counte+'_MultiHorizontal '+mandatory+'" name="'+counte+'_MultiHorizontal" id="'+counte+'_MultiHorizontal" value="'+AnsSplit[i]+'"></td>';
										}
										vhtml +='</tr></table>';*/
										vhtml +='</table>';
										vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
										vhtml +='<input type="hidden" id="'+counte+'_MultiHorizontal" value="">';

										vhtml +='<div id="'+counte+'_MultiHorizontal_checkbox"></div>';
										
										vhtml +='</td></tr>';
									}
								}
							}
							

							if(QuestionType=='sry_qtn_typ_cmt'){ //Comments
								if(obj.currTheme == "expertusoneV2"){
								vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
								
								vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
								vhtml +='<tr><td class="SurveyQuestionField"><span><textarea class="'+mandatory+' textareaClass" name="Comments" rows="2" cols="50" id="'+counte+'_Text">'+Answers+'</textarea></span>';	
								vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
								vhtml +='<input type="hidden" id="'+counte+'_Text" value="'+Answers+'">';
								vhtml +='</td></tr>';
								}
								else
								{
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									vhtml +='<tr><td class="SurveyQuestionField"><span><textarea class="'+mandatory+' textareaClass" name="Comments" rows="2" cols="50" id="'+counte+'_Text">'+Answers+'</textarea></span>';	
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_Text" value="'+Answers+'">';
									vhtml +='</td></tr>';
								}
							}
							
							if(QuestionType=='sry_qtn_typ_dpn'){	//Dropdown						
								if(obj.currTheme == "expertusoneV2"){
								vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
								
								vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
								vhtml +='<tr><td align="left" class="SurveyQuestionField"><div class="expertus-dropdown-bg"><div class="expertus-dropdown-icon"><select class="'+mandatory+'" id="'+counte+'_Dropdown" name="an Option">';
								vhtml +='<option value="">'+SMARTPORTAL.t("Select")+'</option>';
								for (i=0;i<AnsSplit.length;i++){
								vhtml +='<option value="'+AnsSplit[i]+'" title="'+AnsSplit[i]+'">'+AnsSplit[i]+'</option>';
								}
								vhtml +='</select></div></div>';
								vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
								vhtml +='<input type="hidden" id="'+counte+'_Dropdown" value="">';
								
								vhtml +='</td></tr>';
								}else
								{
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									vhtml +='<tr><td align="left" class="SurveyQuestionField"><div class="dropdown-select-width"><select class="'+mandatory+'" id="'+counte+'_Dropdown" name="an Option" style="width:100px">';
									vhtml +='<option value="">'+SMARTPORTAL.t("Select")+'</option>';
									for (i=0;i<AnsSplit.length;i++){
									vhtml +='<option value="'+AnsSplit[i]+'">'+AnsSplit[i]+'</option>';
									}
								    vhtml +='</select><div class="dropdown-arrow-iefix" style="display:none;"></div></div>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_Dropdown" value="">';
									
									vhtml +='</td></tr>';	
								}
								
							}
							
							if(QuestionType=='sry_qtn_typ_yno'){  // Yes/No	
									
								if(obj.currTheme == "expertusoneV2"){
								vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
							
								vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
								vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0">';
								
								var string = "Yes,No";
								//var string = Drupal.t('LBL342')+','+Drupal.t('LBL343');
								var string=string.split(",");								
								if(respData.surveytype == "sry_det_typ_ass" && (respData.random == "sry_det_typ_ass_ran_ans" || respData.random == "sry_det_typ_ass_ran_bot")) {
									string = obj.arrayShuffle(string);	
								}								
								for (i=0;i < string.length;i++){
									vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="'+string[i]+'" />&nbsp;'+string[i]+'</td></tr>';
								}
								
								/*
									vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="Yes" />&nbsp;Yes</td></tr>';
									vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="No" />&nbsp;No</td></tr>';
								*/								
							
								vhtml +='</table>';
								vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
								vhtml +='<input type="hidden" id="'+counte+'_yesno" value=" ">';

								vhtml +='<div id="'+counte+'_yesno_radio"></div>';
								
								vhtml +='</td></tr>';
								}else
								{
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									vhtml +='<tr><td class="SurveyQuestionField"><table cellpadding="0" cellspacing="0" border="0">';
									
									var string = "Yes,No";
									//var string = Drupal.t('LBL342')+','+Drupal.t('LBL343');
									var string=string.split(",");								
									if(respData.surveytype == "sry_det_typ_ass" && (respData.random == "sry_det_typ_ass_ran_ans" || respData.random == "sry_det_typ_ass_ran_bot")) {
										string = obj.arrayShuffle(string);	
									}								
									for (i=0;i < string.length;i++){
										vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="'+string[i]+'" />&nbsp;'+string[i]+'</td></tr>';
									}
									
									/*
										vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="Yes" />&nbsp;Yes</td></tr>';
										vhtml +='<tr><td align="left"><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_yesno" id="'+counte+'_yesno" value="No" />&nbsp;No</td></tr>';
									*/								
								
									vhtml +='</table>';
									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_yesno" value=" ">';

									vhtml +='<div id="'+counte+'_yesno_radio"></div>';
									
									vhtml +='</td></tr>';	
								}
								
							}
							
							if(QuestionType=='sry_qtn_typ_trf'){ //True/False
								if(obj.currTheme == "expertusoneV2"){
								vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
								
								vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+'. </span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
								vhtml +='<tr><td class="SurveyQuestionField">';
								
								vhtml +='<table cellpadding="0" cellspacing="0" border="0">';
								
								var string = "True,False";
								//var string = Drupal.t('LBL384')+','+Drupal.t('LBL385');
								var string=string.split(",");								
								if(respData.surveytype == "sry_det_typ_ass" && (respData.random == "sry_det_typ_ass_ran_ans" || respData.random == "sry_det_typ_ass_ran_bot")) {
									string = obj.arrayShuffle(string);	
								}								
								for (i=0;i < string.length;i++){
									vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="'+string[i]+'" />&nbsp;'+string[i]+'</td></tr>';
								}
								
								/*vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="True" />&nbsp;True</td></tr>';
								vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="False" />&nbsp;False</td></tr>';*/
								
								
								vhtml +='</table>';

								vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
								vhtml +='<input type="hidden" id="'+counte+'_truefalse" value=" ">';
								
								vhtml +='<div id="'+counte+'_truefalse_radio"></div>';
								
								vhtml +='</td></tr>';
								}else
								{
									vhtml +='<tr><td class="sq-spacer">&nbsp;</td></tr>';
									
									vhtml +='<tr valign="top"><td class="SurveyQuestionLabel"><span class="SurveyQuestionList">'+quescnt+')</span><span class="SurveyQuestionText"><span class="survey-question-text">'+Question+'</span> <span  id="'+counte+'_result_image"></span></span></td></tr>';
									vhtml +='<tr><td class="SurveyQuestionField">';
									
									vhtml +='<table cellpadding="0" cellspacing="0" border="0">';
									
									var string = "True,False";
									//var string = Drupal.t('LBL384')+','+Drupal.t('LBL385');
									var string=string.split(",");								
									if(respData.surveytype == "sry_det_typ_ass" && (respData.random == "sry_det_typ_ass_ran_ans" || respData.random == "sry_det_typ_ass_ran_bot")) {
										string = obj.arrayShuffle(string);	
									}								
									for (i=0;i < string.length;i++){
										vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="'+string[i]+'" />&nbsp;'+string[i]+'</td></tr>';
									}
									
									/*vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="True" />&nbsp;True</td></tr>';
									vhtml +='<tr><td><input class="'+mandatory+'" type="radio" class="SurveyRemoveBrd" name="'+counte+'_truefalse" id="'+counte+'_truefalse" value="False" />&nbsp;False</td></tr>';*/
									
									
									vhtml +='</table>';

									vhtml +='<input type="hidden" id="'+counte+'_ques" value="'+Question+'">';
									vhtml +='<input type="hidden" id="'+counte+'_truefalse" value=" ">';
									
									vhtml +='<div id="'+counte+'_truefalse_radio"></div>';
									
									vhtml +='</td></tr>';	
								}
								
							}
							
							if(QuestionType=='sry_qtn_typ_img'){  //Image
							}
							vhtml +='<tr><td><table cellpadding="0" cellspacing="0" border="0"><tr><td><div id="'+counte+'_disp_corr_ans" class="survey-correct-answer" ></div></td></tr></table><td></tr>';
							vhtml +='<input type="hidden" id="'+counte+'_questiontype" value="'+QuestionType+'">';
							vhtml +='<input type="hidden" id="'+counte+'_displayrating" value="'+DisplayRating+'">';
							vhtml +='<input type="hidden" id="'+counte+'_QuestionId" value="'+QuestionIds+'">';
							vhtml +='<input type="hidden" id="'+counte+'_ans" value="'+RightAnswer+'">';
							vhtml +='<input type="hidden" id="'+counte+'_given_ans" value="'+AnswerGiven+'">';
							vhtml +='<input type="hidden" id="'+counte+'_given_stat" value="'+AnswerStatus+'">';
							vhtml +='<input type="hidden" id="'+counte+'_val" class="counteIdClass" value="'+counte+'">';
						
						}
						vhtml +='</table></div>';
					}
					
				});	
					 
					 
				 vhtml +='<input type="hidden" id="sectioncnt" value="'+sectioncnt+'"></div>'; 
				 vhtml +='</td></tr></table>';

				 html ='<tr><td>'+html+'</td></tr>';
				 var fieldsetColspan= sectioncnt+1; 
				 html +='<input type="hidden" name="surveyuser" id="surveyuser" value="MyName" >';
			}
			else
			{
				var SectionTitle ="No Survey Questions";
				 vhtml +="<div class='SurveyStatusText'><b>"+SectionTitle+"</b></div>";
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(6)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').show();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").addClass("white-btn-bg-middle");
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").before('<div class="white-btn-bg-left"></div>');
				$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.white-btn-bg-middle").after('<div class="white-btn-bg-right"></div>');
				this.chkdisableBtn();
			}

			vhtml = vhtml;
			this.destroyLoader(this.loaderDiv);
			$('#'+this.uniqueWidgetId).dialog("open");
	        $('#'+this.uniqueWidgetId).html(vhtml);
	    //    $.ui.surveylearner.prototype.refreshSession();
	        $('#Sec_list').html(html);
	        $('#Sec_list').find("tbody").css("border","0px");
	        obj.getOverLayHeight();
	        obj.chkdisableBtn();
		 }catch(e){
				// to do
			}
		},
		chkdisableBtn:function(){
			try{
			$('.ui-dialog-buttonpane .ui-dialog-buttonset').find(" button.removebutton").each(function(){
			if($(this).is(":hidden")){
				$(this).prev(".white-btn-bg-left").remove();
				$(this).next(".white-btn-bg-right").remove();	
			}
			});
			}catch(e){
				// to do
			}
			},
		getOverLayHeight: function(){
		 try{	
			if(this.showPreview==1){
				$('.SurveyContent input[type=radio]').attr('disabled', 'true');
				$('.SurveyContent input[type=checkbox]').attr('disabled', 'true');
				$('.SurveyContent select').attr('disabled', 'true');
				$('.SurveyContent textarea').attr('readonly', 'true');
				this.previewValidateAnswer();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(5)').hide();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-left-bg').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .admin-save-button-right-bg').remove();
				/*$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-left').remove();
				$('.ui-dialog-buttonpane .ui-dialog-buttonset .white-btn-bg-right').remove();*/
			}
			
			// Reset the committed status
			this.committed = 0;
			var dlgHeight =  parseInt($(".survey-main-holder > .ui-dialog").css("height")) + parseInt($(".survey-main-holder > .ui-dialog").css("top")) + 20;
			var olyHeight =  parseInt($(".ui-widget-overlay").eq(-1).css("height"));
			if(dlgHeight > olyHeight) {
				$(".ui-widget-overlay").eq(-1).css("height",(dlgHeight+"px"));
			}
		 }catch(e){
				// to do
			}
		}
		
		 
});

$.extend($.ui.surveylearner.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

/**
 * function used to keep the session alive when the learner taking assessment.
 * @return
 */
 
$.ui.surveylearner.prototype.refreshSession = function(){
	try{
	//console.log('uniqueWidgetId = ', this.uniqueWidgetId, 'take_survey_main - ', $("#take_survey_main"));
		if($("#take_survey_main").length){ //added length as object turns empty after closing the pop-up
			var url = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.constructUrl("aicc/launch/get/0");
			$.ajax({
	    		   type: "POST",
		           url: url,
		           cache: false,
		           success: function(result){}
		        });
			setTimeout("$.ui.surveylearner.prototype.refreshSession()",(1000*60*15));  // request for once in 15 min
		}
	}catch(e){
		// to do
	//	console.log(e, e.stack);
	}	
}

$(function() {	
	try{
	$("#block-take-survey").surveylearner();
	}catch(e){
		// to do
	}
});
$(function() {
//if($.browser.msie && $.browser.version == 7){	
try{	
if (!$.support.leadingWhitespace) { // if IE6/7/8
    $('select.assdrop-downlist')
        .live('focus click', function() {
          $(this).addClass('expand').removeClass('clicked');
          $(this).next('.dropdown-arrow-iefix').hide();
          })
        .live('click', function() {
          $(this).toggleClass('clicked');
          $(this).next('.dropdown-arrow-iefix').show();
           })
        .live('mouseout', function() { 
            if (!$(this).hasClass('clicked')) { 
        	$(this).removeClass('expand');
        	$(this).next('.dropdown-arrow-iefix').hide();
        	}
        })
        .live('blur', function() {
        	$(this).removeClass('expand clicked');
        	$(this).next('.dropdown-arrow-iefix').hide();
        	});
   
}
//}
}catch(e){
	// to do
}
});

// change single and double quotes to html code
function changeStirngsChars(inputStr){
	try{
		if (inputStr != null || inputStr != undefined)
			return inputStr.replace(/'/g, '&#39;').replace(/"/g, '&#34;');
		else 
			return	inputStr;
	} catch(e) {
	}
	
}
/*else {
    alert('Hey, you are not using IE6/7/8 at all!');
    $('select.assdrop-downlist').live('focus mouseover', function() {
    $(this).addClass('expand').removeClass('clicked');
   // alert('mouseover');
    });
    $('select.assdrop-downlist').live('click', function() { $(this).toggleClass('clicked');
    })
    $('select.assdrop-downlist').live('mouseout', function() {
    	if (!$(this).hasClass('clicked'))
    	{ 
    		$(this).removeClass('expand');
    		//alert('mouseout');
    	}
    	});
    $('select.assdrop-downlist').live('click', function() {
    	$(this).removeClass('expand clicked');
    	});
}

});*/
function TrimSecondsMinutes(elapsed) {
    if (elapsed >= 60)
        return TrimSecondsMinutes(elapsed - 60);
    return elapsed;
};
