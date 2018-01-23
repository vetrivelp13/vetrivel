(function($) {

$.widget("ui.refercourse", {
	_init: function() {
		try{
		var self = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		this.referObj = '$("body").data("refercourse")';
		}catch(e){
			// to do
		}
	},
	
	// Menu callback - My enrollment share link
	getReferDetails :function(entityId,entityType,loaderDiv,referFrom){
		try{
		if(!referFrom){
			referFrom = Drupal.t('Class');
		}
		else{
			referFrom = referFrom.toLowerCase();
		}
		this.createLoader(loaderDiv);
		var url = this.constructUrl("ajax/refer-course/" + entityId + "/" +entityType);		
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
			success: function(result){
			refer = result[0]['labelmsg']['msg1'];
				obj.destroyLoader(loaderDiv);
				$('#refer_class_container').css('overflow','normal');
				obj.paintrefercourse(result,referFrom);
			}
	    });
		}catch(e){
			// to do
		}
	},
	
	// Menu callback - Catalog search page share link
	getEmbedReferDetails : function(loaderDiv,referFrom, qtipObj,Gridoption) {
		try{
			Gridoption = (Gridoption == '' || Gridoption == undefined || Gridoption == null) ? '' : Gridoption;
			if(!referFrom){
				referFrom = Drupal.t('Class');
			}
			else{
				referFrom = referFrom.toLowerCase();
			}
			// convert the string to object
			if(typeof qtipObj == 'string'){
				var qtipObj =  unserialize(qtipObj);
			}
			var url = this.constructUrl("ajax/refer-course/" + qtipObj.entityId + "/" + qtipObj.entityType);
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					
					//refer = result[0]['labelmsg']['msg1'];
					obj.destroyLoader(loaderDiv);
						$('#refer_class_container').css('overflow','normal');
						$("body").data("refercourse").paintEmbedReferCourse(result, referFrom, qtipObj, loaderDiv);
				}
		    });
			// #0041880: Condition added Due to the after Share Button clicked the Ajax Loader is showing from Embed Tab To Email Tab.
			if(Gridoption == 'Popup'){
				$('.qtip-popup-visible').html('').hide();
				var beforeShow;
				var afterPosition;
				var afterShow;
				/*Viswanthan added for #0074703*/
				var ua = window.navigator.userAgent;
			    var msie = ua.indexOf("MSIE ");

			    if (msie > 0 || !!navigator.userAgent.match(/Trident.*rv\:11\./))  // If Internet Explorer, return version number
			    {
			    	top_height='40px';
			    
			    }
			    else{
			    	top_height='auto';
			    }
				if(typeof qtipObj.callFrom != 'undefined' && qtipObj.callFrom == 'catalog-multishare') {
					beforeShow = function() {
									$('#multishareoption #visible-popup-0').css({'visibility': 'hidden'});
									$('<div id="multishareoption-mask"></div>').prependTo('#multishareoption')
									.css({'height': '100%', 'width': '100%', 'position': 'absolute', 'opacity': 0});
								};
					afterPosition = function() {
									$('#multishareoption #visible-popup-0').css({'left': 'auto', 'right':'0', 'top': top_height})
									.find('#bubble-face-table').css({'left': 'auto', 'right':'0', 'top': 'auto'})
								}
					afterShow = function() {
									$('#multishareoption #visible-popup-0').css({'visibility': 'visible'});
									$('#multishareoption').addClass('active');
								}
					delete qtipObj.beforeShow;
					delete qtipObj.afterPosition;
					delete qtipObj.afterShow;
					var closeOnClickOutside = function(event) {
						if(($(event.target).parents('#multishareoption').length == 0 && !$(event.target).is('#multishareoption')) 
							|| $(event.target).is('#multishareoption-mask')) {
							closeQtip("multishareoption", 0);
							$('#multishareoption').removeClass('active');
							$('#multishareoption-mask').remove();
						}
					}
					$('body:not(.share-click-binded)').addClass('share-click-binded').bind('click', closeOnClickOutside);
				}
				$('#' + qtipObj.popupDispId).qtipPopup({
					wid : qtipObj.wid,
					heg : qtipObj.heg,
					entityId : qtipObj.entityId,
					popupDispId : qtipObj.popupDispId,
					postype : qtipObj.postype,
					poslwid : (qtipObj.poslwid == '' || qtipObj.poslwid == undefined || qtipObj.poslwid == null) ? '' : qtipObj.poslwid, 
					posrwid : (qtipObj.posrwid == '' || qtipObj.posrwid == undefined || qtipObj.posrwid == null) ? '' : qtipObj.posrwid,
					disp	: (qtipObj.qdis == '' || qtipObj.qdis == undefined || qtipObj.qdis == null) ? ''	: qtipObj.qdis,
					linkid	: qtipObj.linkid,
					onClsFn	: qtipObj.onClsFn,
					dispDown : (qtipObj.dispDown == undefined) ? '': qtipObj.dispDown,
					beforeShow: beforeShow,
					afterPosition: afterPosition,
					afterShow: afterShow
				});
				
				setTimeout(function(){EXPERTUS_SMARTPORTAL_AbstractDetailsWidget.createLoader("paintContent"+ qtipObj.popupDispId)},10);
			}
		} catch(e) {
			// to do
		}
		
	},
	
	callReferSubmit :function(entityId,entityType,referFrom){
		try{
			$('#show_expertus_message').hide();
		if ($('#rc-emailfrom').hasClass('error')) {
		    $('#rc-emailfrom').removeClass('error');
		}
	    if ($('#rc-emailto').hasClass('error')) {
	      $('#rc-emailto').removeClass('error');
	    }
		var obj = this;
		var inc = 0; 
		var eCount = 0;
		var rEmail = new Array();
		var errMsg = new Array();
		errMsg[0] = '';
		if($('#rc-error')){ $('#rc-error').remove(); }
		
		var tbFrom = $('#rc-emailfrom').val();
		var tbTo = $('#rc-emailto').val();
		var tbSubject = $("#rc-subject").val();
		var chkCcopy = $("#rc-ccopy").attr("checked");
		var taComment = $('#edit-description').val(); 
		var multiEmail = tbTo.split(",");
		var Msg = Drupal.t("LBL116");
		var ErrMsg =Drupal.t("LBL117");
		var Errormsg = Drupal.t("ERR152");
		/*this.currTheme = Drupal.settings.ajaxPageState.theme;
		$("#rc-ccopy").live("click",function(){
		  	if(this.currTheme == "expertusoneV2"){
		  		if(chkCcopy==true)
		  		{
		  		alert('call');	
		  		 $(this).parents('div').removeClass('.checkbox-unselected').removeClass('.checkbox-selected');	
		  		}
		  	}
			});
*/		//To Validate List of Email
		$(multiEmail).each(function(){
		  if($.trim(this) != "") {
			  if(obj.validateEmail($.trim(this))) {
				  rEmail[eCount] = $.trim(this);
				  eCount++;
			  }
		  }
		});
		
		if(!obj.validateEmail(tbFrom) || (rEmail.length == 0) || ($(multiEmail).length != $(rEmail).length) || tbFrom.indexOf(',') != '-1') {
			if(jQuery.trim(tbFrom) != "") {
				if(tbFrom.indexOf(',') != '-1'){
					errMsg[inc]= SMARTPORTAL.t("Multiple email ids are not allowed in the <b>From From</b> field.")+"<br/>";
					$('#rc-emailfrom').addClass('error');
					inc++;
				}else if(!obj.validateEmail(tbFrom)){
					errMsg[inc]= Errormsg+" "+Drupal.t("LBL649")+".";
					$('#rc-emailfrom').addClass('error');
					inc++;
				}
			} else {
				errMsg[inc]= Msg+" "+Drupal.t("ERR101");
				$('#rc-emailfrom').addClass('error');
				inc++;
			}
			if(jQuery.trim(tbTo) != "") {
				if(rEmail.length == 0 || ($(multiEmail).length != $(rEmail).length)){
					if(errMsg[0].indexOf(Errormsg)>=0){
						errMsg[0]= Errormsg+" "+Drupal.t("LBL649")+","+Drupal.t("LBL621");
						$('#rc-emailto').addClass('error');
					}
					else{
						errMsg[inc]= Errormsg+" "+Drupal.t("LBL621")+".";
						$('#rc-emailto').addClass('error');
						inc++;
					}
				}
			} else {
				if(errMsg[0].indexOf(Msg)>=0){
					errMsg[inc]= Drupal.t("LBL621")+" "+Drupal.t("ERR101");
					$('#rc-emailto').addClass('error');
					inc++;
				} 
				else{
					errMsg[inc]= ErrMsg+" "+Drupal.t("ERR101");
					$('#rc-emailto').addClass('error');
					inc++;
				}
			}
			var message_call = expertus_error_message(errMsg,'error');
			$('#show_expertus_message').html(message_call);
			$('#show_expertus_message').show();
			
			/*errMsg = '<div id="rc-error" class="rc-error">'+errMsg+'</div>';
			$('#form-refer').prepend(errMsg);
			$('#rc-error').show();*/
			return
			
		}else{
			//$('.ui-dialog-buttonset button').attr("disabled", true);
			if($('#refer_class_container')){
				this.createLoader('refer_class_container');
				$('#refer_class_container').css('overflow','hidden');
			}
			var subcomment = encodeURIComponent(encodeURIComponent(tbSubject))+'@|@'+encodeURIComponent(encodeURIComponent(taComment));
			var url = this.constructUrl("ajax/refer-course-submit/" + entityId + "/" +entityType + "/" + tbFrom + "/" + rEmail + "/" + subcomment + "/" + chkCcopy);
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					if (referFrom == 'multiple') { referFrom = 'catalog'; }
					if (referFrom == 'course'){referFrom = Drupal.t('Course');}
					if (referFrom == 'class'){referFrom = Drupal.t('Class');}
					if (referFrom == 'certification'){referFrom = Drupal.t('Certification');}
					if (referFrom == 'curricula'){referFrom = Drupal.t('Curricula');}
					if (referFrom == 'learning plan'){referFrom = Drupal.t('Learning Plan');}
					if (referFrom == 'catalog'){referFrom = Drupal.t('CATALOG');}
					obj.destroyLoader('refer_class_container');
					$('#form-refer').hide(); // hide e-mail form
					$('.refer-note').remove();
					/*-- #41754: Confirmation message not showing Fix - Check the menu tab exist on the popup to find a popup --*/  
					if ($('#refer_course_holder #learner-maincontent_tab3').length) {
						$('#learner-maincontent_tab3').after('<div id="rc-success-container"></div>');
						$('#rc-success-container').html('<div class="rc-success-message">'+Drupal.t("MSG251")+referFrom.toLowerCase()+'.</div>');
					} else {
						$('#refer_class_container').html('<div class="rc-success-message">'+Drupal.t("MSG251")+referFrom.toLowerCase()+'.</div>');
						//$('.ui-dialog-buttonset button').remove();
						$('.ui-dialog-buttonset').css("margin-right","0px");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("shareclosetext").end();
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').css("display","none");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').prev('.admin-save-button-left-bg').css("display","none");
						$('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').next('.admin-save-button-right-bg').css("display","none");
					}
					
				}
		    });
		}
		}catch(e){
			// to do
		}
	},
	
	validateEmail: function(cValue){
		try{
		//var emailReg = /^([\w-\.]+@([\w-]+\.)+[\w-]{2,4})?$/;
		var emailReg = /^([a-z\d!#$%&'*+\-\/=?^_`{|}~\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]+(\.[a-z\d!#$%&'*+\-\/=?^_`{|}~\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]+)*|"((([ \t]*\r\n)?[ \t]+)?([\x01-\x08\x0b\x0c\x0e-\x1f\x7f\x21\x23-\x5b\x5d-\x7e\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]|\\[\x01-\x09\x0b\x0c\x0d-\x7f\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]))*(([ \t]*\r\n)?[ \t]+)?")@(([a-z\d\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]|[a-z\d\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF][a-z\d\-._~\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]*[a-z\d\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])\.)+([a-z\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]|[a-z\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF][a-z\d\-._~\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]*[a-z\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])\.?$/i;
		if(!emailReg.test(cValue) || cValue == "")
			return false;
		else
			return true;
		}catch(e){
			// to do
		}
	},
	
	paintTabMenu : function(loaderDiv, referFrom, qtipObj) {
		try {
			if(typeof qtipObj == 'object'){
				var callFrom = ( qtipObj.callFrom != undefined ) ? qtipObj.callFrom : '';
				var qtipObj = $.param(qtipObj);
			}
			var menu_access = Drupal.settings.refer_course.admin_access;
			var rhtml = '';
			if (menu_access == true	&& callFrom != 'detail_page'){
				rhtml += '<div id="learner-maincontent_tab3">';
				rhtml += '<ul class="EmbedtabNavigation">';
				rhtml += '<li><a onclick="$(&quot;.EmbedtabNavigation li&quot;).removeClass(&quot;selected&quot;);$(this).parents(&quot;li&quot;).addClass(&quot;selected&quot;);$(&quot;body&quot;).data(&quot;refercourse&quot;).getEmbedReferDetails(&quot;'+loaderDiv+'&quot;,&quot;'+referFrom+'&quot;,&quot;'+qtipObj+'&quot;)" href="javascript:void(0);" id="Sharepart"><span><span>'+Drupal.t("E-MAIL")+'</span></span></a></li>';
					rhtml += '<li><a onclick="$(&quot;.EmbedtabNavigation li&quot;).removeClass(&quot;selected&quot;);$(this).parents(&quot;li&quot;).addClass(&quot;selected&quot;);$(&quot;body&quot;).data(&quot;refercourse&quot;).getEmbedDetails(&quot;'+loaderDiv+'&quot;,&quot;'+referFrom+'&quot;,&quot;'+qtipObj+'&quot;)" href="javascript:void(0);" id="Embedpart"><span><span>'+Drupal.t("EMBED")+'</span></span></a></li>';
				rhtml += '</ul>';
				rhtml += '</div>';
			}
			return rhtml;
		} catch(e) {
			// to do
		}
	},
	
	paintrefercourse: function(result,referFrom){
	 try{	
		 res = result;
			var obj = this;

			var rhtml = '';
			
			rhtml += '<form id="form-refer" method="post"><div id="show_expertus_message" style="display:none;"></div>';
			rhtml += '<table cellpadding="0" cellspacing="0" border="0" class="refer-class-table" width="100%">';
			
			rhtml += '<tr>';
			rhtml += '<td class="refer-class-col1">'+SMARTPORTAL.t(result[0]['labelmsg']['msg2'])+' :<span class="require-text">*</span></td>';
			rhtml += '<td class="refer-class-col2"></td>';
			rhtml += '<td class="refer-class-col3"><input type="text" id="rc-emailfrom" class="input-text" tabindex="1" value="'+res['email']+'"></td>';
			//rhtml += '<td class="refer-class-col4"></td>';
			rhtml += '</tr>';

			rhtml += '<tr>';
			rhtml += '<td class="refer-class-col1">'+SMARTPORTAL.t(result[0]['labelmsg']['msg3'])+' :<span class="require-text">*</span></td>';
			rhtml += '<td class="refer-class-col2"></td>';
			rhtml += '<td class="refer-class-col3"><input type="text" id="rc-emailto" class="input-text" tabindex="2"></td>';
			//rhtml += '<td class="refer-class-col4"></td>';
			rhtml += '</tr>';

			rhtml += '<tr>';
			rhtml += '<td class="refer-class-col1">'+SMARTPORTAL.t(result[0]['labelmsg']['msg4'])+' : </td>';
			rhtml += '<td class="refer-class-col2"></td>';
			rhtml += '<td class="refer-class-col3"><input type="text" id="rc-subject" class="input-text" readonly tabindex="3" value="'+htmlEntities(res['title'])+'"></td>';
			//rhtml += '<td class="refer-class-col4">&nbsp;</td>';
			rhtml += '</tr>';

			rhtml += '<tr>';
			rhtml += '<td class="refer-class-col1">&nbsp;</td>';
			rhtml += '<td class="refer-class-col2">&nbsp;</td>';
			if(this.currTheme == "expertusoneV2"){
				rhtml += '<td class="refer-class-col3"><div class="checkbox-unselected"><input type="checkbox" id="rc-ccopy" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="4"></div>'+SMARTPORTAL.t(result[0]['labelmsg']['msg5'])+'</td>';
			}else{
				rhtml += '<td class="refer-class-col3"><label class="checkbox-unselected" for="rc-ccopy"><input type="checkbox" id="rc-ccopy" tabindex="4" onclick="checkboxSelectedUnselectedCommon(this);"></label><span class="embedsharelabel">'+SMARTPORTAL.t(result[0]['labelmsg']['msg5'])+'</span></td>';
			}
			//rhtml += '<td class="refer-class-col4">&nbsp;</td>';
			rhtml += '</tr>';

			rhtml += '<tr>';
			rhtml += '<td colspan="3" class="refer-class-mtitle"><span>'+SMARTPORTAL.t(result[0]['labelmsg']['msg6'])+'</span> ('+SMARTPORTAL.t(result[0]['labelmsg']['msg7'])+')</td>';
			rhtml += '</tr>';
			
			rhtml += '<tr>';
			rhtml += '<td colspan="3" class="refer-class-mfield"><textarea class="input-textarea" rows="5" cols="40" name="description" id="edit-description" class="input-textarea" tabindex="5"></textarea></td>';
			//rhtml += '<td class="refer-class-col4">&nbsp;</td>';
			rhtml += '</tr>';
			
			rhtml += '</table>';
			rhtml += '</form>';
			
			var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';
			var dlgDiv = '<div id="refer_class_container" class="refer-class-container"></div>';
			$('#refer_class_container').remove();
			$("#refer_course_holder").remove();
			$('body').append(dlgDiv);
			
			$('#refer_class_container').html(rhtml);

			
			var closeButt={};
			closeButt[SMARTPORTAL.t(result[0]['labelmsg']['msg9'])]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#refer_class_container').remove();};
			closeButt[SMARTPORTAL.t(result[0]['labelmsg']['msg10'])]=function(){
				eval('$("body").data("refercourse").callReferSubmit(\''+res["entityId"]+'\',\''+res["entityType"]+'\',\''+referFrom+'\')');
			};
			
			
			if(!referFrom){
				referFrom = 'Class';
			}
			
			$("#refer_class_container").dialog({
				bgiframe: true,
				width:390,
				resizable:false,
				draggable:false,
				closeOnEscape: false,
				modal: true, 
				title: Drupal.t('Share'),
				buttons: closeButt,
				close: function(){
					$("#refer_class_container").remove();
					$("#refer_course_holder").remove();
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
			$('.ui-dialog').wrap("<div id='refer_course_holder' class='refer-course-holder'></div>");
			$('#refer_course_holder a.ui-dialog-titlebar-close').html('X');
			$('.ui-dialog-buttonset').before(noteTxt);
			$('.refer-class-container').css('width','93.2%');
			$('.refer-class-container').css('height','auto');
	 }catch(e){
			// to do
	 }
	},
	
	paintEmbedReferCourse: function(result,referFrom, qtipObj, loaderDiv){
		 try{	
			res = result;
			var rhtml = '';
			var readonly = '';
			if(referFrom != 'multiple'){ // Condtion made for this ticket 
				readonly = 'readonly';
			}
			if(result.sharemodule == '1' && result.shareopt == 'both'){
				rhtml += this.paintTabMenu('refer_class_holder', referFrom, qtipObj);
			}			
			
			// validate json object and check the access
			if (typeof result == 'string') {
				rhtml += '<div align="center" class="access_error">' + Drupal.t('Access denied') + '</div>';
			} else {
				if(result.sharemodule == '1' && (result.shareopt == 'email'  || result.shareopt == 'both')){
				rhtml += '<form id="form-refer" name="form_refer" method="post"><div id="show_expertus_message" style="display:none;"></div>';
				rhtml += '<table cellpadding="0" cellspacing="0" border="0" class="refer-class-table" width="100%">';
				
				rhtml += '<tr>';
				rhtml += '<td class="refer-class-col1">'+Drupal.t(result[0]['labelmsg']['msg2'])+' :<span class="require-text">*</span></td>';
				rhtml += '<td class="refer-class-col2"></td>';
				rhtml += '<td class="refer-class-col3"><input type="text" id="rc-emailfrom" class="input-text" tabindex="1" value="'+res['email']+'"></td>';
				rhtml += '</tr>';
	
				rhtml += '<tr>';
				rhtml += '<td class="refer-class-col1">'+Drupal.t(result[0]['labelmsg']['msg3'])+' :<span class="require-text">*</span></td>';
				rhtml += '<td class="refer-class-col2"></td>';
				rhtml += '<td class="refer-class-col3"><input type="text" id="rc-emailto" class="input-text" tabindex="2"></td>';
				rhtml += '</tr>';
	
				rhtml += '<tr>';
				rhtml += '<td class="refer-class-col1">'+Drupal.t(result[0]['labelmsg']['msg4'])+' : </td>';
				rhtml += '<td class="refer-class-col2"></td>';
				rhtml += '<td class="refer-class-col3"><input type="text" id="rc-subject"'+readonly+' class="input-text" tabindex="3" value="'+htmlEntities(res['title'])+'"></td>';
				rhtml += '</tr>';
	
				rhtml += '<tr>';
				rhtml += '<td class="refer-class-col1">&nbsp;</td>';
				rhtml += '<td class="refer-class-col2">&nbsp;</td>';
				if(this.currTheme == "expertusoneV2"){
					rhtml += '<td class="refer-class-col3"><div class="checkbox-unselected"><input type="checkbox" id="rc-ccopy" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="4"></div><div class="checkbox-label">'+Drupal.t(result[0]['labelmsg']['msg5'])+'</div></td>';
				}else{
					rhtml += '<td class="refer-class-col3"><label class="checkbox-unselected" for="rc-ccopy"><input type="checkbox" id="rc-ccopy" tabindex="4" onclick="checkboxSelectedUnselectedCommon(this);"></label><span class="embedsharelabel">'+Drupal.t(result[0]['labelmsg']['msg5'])+'</span></td>';
				}
				rhtml += '</tr>';
	
				rhtml += '<tr>';
				rhtml += '<td colspan="3" class="refer-class-mtitle"><span>'+Drupal.t(result[0]['labelmsg']['msg6'])+'</span> ('+Drupal.t(result[0]['labelmsg']['msg7'])+')</td>';
				rhtml += '</tr>';
				
				rhtml += '<tr>';
				rhtml += '<td colspan="3" class="refer-class-mfield"><textarea class="input-textarea" rows="5" cols="40" name="description" id="edit-description" class="input-textarea" tabindex="5"></textarea></td>';
				rhtml += '</tr>';
				
				rhtml += '</table>';
				rhtml += '<div class="ui-dialog-buttonpane"><div class="ui-dialog-buttonset">';
				rhtml += '<button type="button" onclick="closeQtip(&quot;'+qtipObj.popupDispId+'&quot;, '+qtipObj.entityId+');" class="removebutton" tabindex="6">'+Drupal.t(result[0]['labelmsg']['msg9'])+'</button>';
				rhtml += '<div class="admin-save-button-left-bg"></div><button type="button" name="submitbtn" class="admin-save-button-middle-bg" tabindex="7">'+Drupal.t(result[0]['labelmsg']['msg10'])+'</button><div class="admin-save-button-right-bg"></div>';
				rhtml += '</div></div>';
				rhtml += '</form>';
			
				var noteTxt ='<div class="refer-note"><span title="This field is required." class="require-text vtip" style="display:inline-block;margin-right:5px;margin-left:7px;">*</span><span class="refer-note-text">'+SMARTPORTAL.t(result[0]['labelmsg']['msg8'])+'</span></div>';

				} else {
					if(typeof(loaderDiv) !== undefined ) {
					    this.getEmbedDetails(loaderDiv,referFrom,qtipObj);	
					}
				}
			}
			
			var dlgDiv = '<div id="refer_course_holder" class="refer-course-holder"><div id="refer_class_container" class="refer-class-container">'+rhtml+'</div></div>';
			
			if(!referFrom){
				referFrom = 'Class';
			}
			$("body").data("refercourse").visibleSharePopup(qtipObj, dlgDiv);
			if (document.form_refer != undefined) {
				document.form_refer.submitbtn.onclick = function() {
					$("body").data("refercourse").callReferSubmit(res['entityId'], res['entityType'], referFrom);
				};
				$('.ui-dialog-buttonset').before(noteTxt);
			}
			
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
				changeDialogPopUI();
			}
			$('.refer-class-container #Sharepart').parents('li').addClass("selected");
			$('.refer-class-container').css('width','100%');
			
		 }catch(e){
				// to do
		 }
		},
	
	getEmbedDetails :function(loaderDiv,referFrom, qtipObj){
		try{
			
		if(!referFrom){
			referFrom = Drupal.t('Class');
		}
		else{
			referFrom = referFrom.toLowerCase();
		}
		// convert the string to object
		if(typeof qtipObj == 'string'){
			var qtipObj =  unserialize(qtipObj);
		}
		this.createLoader(loaderDiv);
		
		var url = this.constructUrl("ajax/embed-widget/" + qtipObj.entityId + "/" +qtipObj.entityType);		
		var obj = this;
		$.ajax({
			type: "POST",
			url: url,
			//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM 
			success: function(result){
				//refer = result[0]['labelmsg']['msg1'];
				obj.destroyLoader(loaderDiv);
				$('#embed_widget_container').css('overflow','normal');
				obj.paintEmbedWidget(result,referFrom, qtipObj);
			}
	    });
		}catch(e){
			// to do
		}
	},
	
	paintEmbedWidget: function(result,referFrom, qtipObj){
		 try{	
			
			res = result;
			var obj = this;
			if(Drupal.settings.convertion.mylearn_version == 1){
				var SourceStr = resource.base_url+'/?q='+'widget/catalog-search' + '/' + res['uniqueId'] + '&img=1&dsc=1&but=1&sln=1&pul=1';
			}else{
				var SourceStr = resource.base_url+'/?q='+'widget/catalog-search' + '/' + res['uniqueId'] + '&img=1&dsc=1&but=1&sln=1';
			}
			var embedlink = '<iframe width="640" height="480" src="'+SourceStr+'" frameborder="0" allowfullscreen></iframe>';
			var rhtml = '';
			if(result.sharemodule == '1' && result.shareopt == 'both'){
			rhtml += this.paintTabMenu('refer_class_holder', referFrom, qtipObj);
			}
			// validate json object and check the access
			if (typeof result == 'string') {
				rhtml += '<div align="center" class="access_error">' + Drupal.t('Access denied') + '</div>';
			} else {
				if(result.sharemodule == '1' && (result.shareopt == 'embed' || result.shareopt == 'both')){
				rhtml += '<form id="form-embed-widget" method="post"><div id="show_expertus_message" style="display:none;"></div>';
				rhtml += '<table cellpadding="0" cellspacing="0" border="0" class="widget-class-table" width="100%">';
				
				rhtml += '<tr>';
				if(result.shareopt == 'embed'){
				rhtml += '<td colspan="2" class="refer-class-mfield"><span class="label-text"><b> Embed URL : </b></span><textarea class="input-textarea" rows="5" cols="40" name="embedCode" id="embedCode" class="input-textarea" tabindex="-1" readonly="readonly" onclick="this.select()">'+embedlink+'</textarea></td>';
				}else{
				rhtml += '<td colspan="2" class="refer-class-mfield"><textarea class="input-textarea" rows="5" cols="40" name="embedCode" id="embedCode" class="input-textarea" tabindex="-1" readonly="readonly" onclick="this.select()">'+embedlink+'</textarea></td>';
				}
				rhtml += '</tr>';
				
				rhtml += '<tr>';
				rhtml += '<td class="dispayparam-sizeselect" colspan="2"><span class="label-text">'+Drupal.t('LBL1275')+':</span><span class="require-text">*</span>';
				rhtml += '<div class="widgetsize-container "><select name="widgetsize" id="widgetsize"  tabindex="1">';
				for(var k in result['sizeOptions']) {
					rhtml += '<option value="'+k+'">'+result['sizeOptions'][k]+'</option>';
				}
				rhtml += '</select></div>';
				rhtml += '</td>';
				rhtml += '</tr>';
	
				rhtml += '<tr>';
				rhtml += '<td class="dispayparam-checkbox"><div class="checkbox-selected"><input type="checkbox" class="display-params" id="showImage" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="2" checked="checked"></div><span class="checkbox-label">'+Drupal.t('LBL1276')+'</span></td>';
				rhtml += '<td class="dispayparam-checkbox dispayparam-rowtwo"><div class="checkbox-selected"><input type="checkbox" class="display-params" id="showDesc" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="3" checked="checked"></div><span class="checkbox-label">'+Drupal.t('LBL1277')+'</span></td>';
				rhtml += '</tr>';
				
				rhtml += '<tr>';
				rhtml += '<td class="dispayparam-checkbox"><div class="checkbox-selected"><input type="checkbox" class="display-params" id="showButton" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="4" checked="checked"></div><span class="checkbox-label">'+Drupal.t('LBL1278')+'</span></td>';
				rhtml += '<td class="dispayparam-checkbox dispayparam-rowtwo"><div class="checkbox-selected"><input type="checkbox" class="display-params" id="showLine" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="5" checked="checked"></div><span class="checkbox-label">'+Drupal.t('LBL1279')+'</span></td>';
				rhtml += '</tr>';
				if(Drupal.settings.convertion.mylearn_version == 1){
					rhtml += '<tr>';
					rhtml += '<td class="dispayparam-checkbox" colspan="2"><div class="checkbox-selected"><input type="checkbox" class="display-params" id="passUrl" tabindex="6" checked="checked"></div><span class="checkbox-label">'+Drupal.t('LBL1280')+'</span></td>';
					rhtml += '</tr>';
				}else{
					rhtml += '<tr>';
					rhtml += '<td class="dispayparam-checkbox" colspan="2"><div class="checkbox-unselected"><input type="checkbox" class="display-params" id="passUrl" onclick="checkboxSelectedUnselectedCommon(this);" tabindex="6"></div><span class="checkbox-label">'+Drupal.t('LBL1280')+'</span></td>';
					rhtml += '</tr>';
				}
				
				rhtml += '<tr><td style="display:none" colspan="4">';
				rhtml += '<input type="hidden" value="'+SourceStr+'" id="embedSource">';
				rhtml += '<input type="hidden" value="'+res["uniqueId"]+'|'+res["entityId"]+'|'+res["entityType"]+'" id="entity">';
				rhtml += '</td></tr>';
				rhtml += '</table>';
				rhtml += '</form>';
			  }else{
				  if(typeof(loaderDiv) !== undefined ) {
					    this.getEmbedReferDetails(loaderDiv,referFrom,qtipObj,Gridoption);	
					}
			  }
			}
			var dlgDiv = '<div class="refer-course-holder" id="refer_course_holder"><div id="embed_widget_container" class="embed-widget-container">'+rhtml+'</div></div>';
			
			//$('#embed_widget_container').remove();
			//$("#refer_course_holder").remove();
			//$('body').append(dlgDiv);
			
			//$('#embed_widget_container').html(rhtml);

			$("body").data("refercourse").visibleSharePopup(qtipObj, dlgDiv);
			
			
			this.currTheme = Drupal.settings.ajaxPageState.theme;
			if(this.currTheme == "expertusoneV2"){
			changeDialogPopUI();
			}
			//$('.ui-dialog-buttonpane').hide();
			$('#embed_widget_container #Embedpart').parents('li').addClass("selected");
			//$('.ui-dialog').wrap("<div id='refer_course_holder' class='refer-course-holder'></div>");
			//$('#refer_course_holder a.ui-dialog-titlebar-close').html('X');
			$('.embed-widget-container').css('width','100%');
		 }catch(e){
				// to do
		 }
	},
	
	customizeCode: function(event) {
		try {
			var SourceStrVal = $('#embedSource').val();
			var widgetsize = $('#widgetsize').val();
			widgetsize = widgetsize.split('x');
			var frameWidth= (widgetsize[0] == '' ? '315' : widgetsize[0]);
			var frameHeight = (widgetsize[1] == '' ? '560' : widgetsize[1]);
			
			if(event.type == 'click') {
				var element = event.target.id;
				if($('#'+element).attr('checked') == true){
					SourceStr = this.addSourceParam(element, SourceStrVal);
					
				} else {
					SourceStr = this.removeSourceParam(element, SourceStrVal);
				}
				;
			} else {
				SourceStr = SourceStrVal;
			}
			
			var embedlink = '<iframe width="'+frameWidth+'" height="'+frameHeight+'" src="'+SourceStr+'" frameborder="0" allowfullscreen></iframe>';
			$('#embedCode').val(embedlink);
			$('#embedSource').val(SourceStr);
		} catch(e) {
			// to do
		}
	},
	
	addSourceParam : function(element, SourceStr) {
		try{
			var parmStr = '';
			switch (element) {
				case 'showImage':
					parmStr = 'img=1';
					break;
				case 'showDesc':
					parmStr = 'dsc=1'; 
					break;
				case 'showButton':
					parmStr = 'but=1';
					break;
				case 'showLine':
					parmStr = 'sln=1';
					break;	
				case 'passUrl':
					parmStr = 'pul=1';
					break;
				default:
					parmStr = '';
					break;
			}
			if(Drupal.settings.convertion.mylearn_version == 1 && element=='passUrl'){
				return SourceStr;
			}else{
				SourceStr += '&'+ parmStr;
			}
			return SourceStr;
		} catch(e) {
			//to do
		}
	},
	
	removeSourceParam : function(element, SourceStr) {
		try{
			var parmStr = '';
			switch (element) {
				case 'showImage':
					parmStr = 'img=1';
					break;
				case 'showDesc':
					parmStr = 'dsc=1';
					break;
				case 'showButton':
					parmStr = 'but=1';
					break;
				case 'showLine':
					parmStr = 'sln=1';
					break;
				case 'passUrl':
					parmStr = 'pul=1';
					break;
				default:
					parmStr = '';
					break;
			}
			var charStart = SourceStr.indexOf(parmStr);
				if(charStart) {
					if(Drupal.settings.convertion.mylearn_version == 1 && element=='passUrl'){
						return SourceStr;
					}else{
						var removeStr = '';
						removeStr = '&' + parmStr;
						SourceStr = SourceStr.replace(removeStr, '');
					}
			}
			return SourceStr;
		} catch(e) {
			//to do
		}
	},
	
	embedSubmitCall : function(widgetMode, combinedParam) {
		try{
			var url = this.constructUrl("ajax/embed-widget-save/" + widgetMode + "/" +combinedParam);		
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				//data:  '', //Commented by ganeshbabuv to avoid the data value as null when invoke the ajax callback for implement salesforce cookieless option #0054508 on 30th Sep 2015 10:40 AM
				success: function(result){
					
				}
		    });
		} catch(e) {
			// to do
		}
	},
	
	visibleSharePopup : function(qtipObj, content) {
		try{
			var popupId 	= qtipObj.popupDispId;
			var entId = qtipObj.entityId;
			
			$('#'+popupId+' #visible-popup-'+entId+' #bubble-face-table .bubble-c #paintContent'+popupId).html(content);
			//$.extend(true, Drupal.settings, data.drupal_settings);
			Drupal.attachBehaviors();
			vtip();
	        
		}catch(e){
		}
		
	}, 

});

$.extend($.ui.refercourse.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
		$("body").refercourse();
		$('#widgetsize').live('change', function(event){
			$("body").data("refercourse").customizeCode(event);
		});
		$('.display-params').live('click', function(event){
			$("body").data("refercourse").customizeCode(event);
		});
		$("#rc-emailto").live('focus',function() {
			$('#show_expertus_message').hide();
		});
		$("#embedCode").live('copy', function(event) {
			var element = event.target.id;
			var entity = $('#entity').val();
			var entityVal = entity.split('|');
			
			var showImage = $('#showImage').attr('checked');
			var showDesc = $('#showDesc').attr('checked');
			var showButton = $('#showButton').attr('checked');
			var showLine = $('#showLine').attr('checked');
			var widgetSize = $('#widgetsize').val();
			if(Drupal.settings.convertion.mylearn_version == 1){
				var combinedParam  = entity+'|'+showImage+'|'+showDesc+'|'+showButton+'|'+showLine+'|'+widgetSize+'|true';
			}else{
				var passUrl = $('#passUrl').attr('checked');
				var combinedParam  = entity+'|'+showImage+'|'+showDesc+'|'+showButton+'|'+showLine+'|'+widgetSize+'|'+passUrl;
			}
			if(entityVal[2] == 'cre_sys_cat_shr') {
				$("body").data("refercourse").embedSubmitCall('multiple', combinedParam);
			} else {
				$("body").data("refercourse").embedSubmitCall('single', combinedParam);
			}
		}); 
	}catch(e){
		// to do
	}
});

/*--- Common library functions ---*/
function unserialize(str) {
	  str = decodeURIComponent(str);
	  var chunks = str.split('&'),
	      obj = {};
	  for(var c=0; c < chunks.length; c++) {
	    var split = chunks[c].split('=', 2);
	    obj[split[0]] = split[1];
	  }
	  return obj;
	}

function closeQtip(popupId,entId,onClsFn){
    try{
            if(typeof(onClsFn) == 'undefined') onClsFn='';
            if(popupId == '')
                    $('.qtip-popup-visible').html('').hide();
            else
                    $('#'+popupId+' #visible-popup-'+entId).html('').hide();
            
            if(typeof(onClsFn) == 'function') {
                    onClsFn();
            }
    }catch(e){
            
    }
}