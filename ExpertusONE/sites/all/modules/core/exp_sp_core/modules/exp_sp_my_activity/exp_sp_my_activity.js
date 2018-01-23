(function($) {
	var optionprofile = '';
	var gridloadpage  = '';
	var pageroption   = '';
	var hiddenuserid  = '';
$.widget("ui.myprofileactivity", {
	_init: function() {
		try{
  	    var self = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
			var myActivityWidth = 658;
			var myActivitypopwidth=494;
		}else{
			var myActivityWidth = 670;
			var myActivitypopwidth=460;
	    } 
		if(document.getElementById('myprofile-myactivity-screen')){	
		  	optionprofile =  'myprofile-myactivity-screen';
		  	gridloadpage  =  'myprofile-activity-userdetails';
		  	pageroption   =  'userdetailsactivity-pager';
		  	hiddenuserid  =  document.getElementById('hidden-other-userid').value;
		  	gridwidth     =  myActivitypopwidth;
			}else{
				optionprofile =  'wizarad-myprofile-activity' ;
				gridloadpage  =  'myprofile-activity-details';
		  	pageroption   =  'activity-pager';
		  	hiddenuserid  =  'emptyvalue';
		  	gridwidth     =  myActivityWidth;
			}
		this.renderMyActivity();
		}catch(e){
			// to do
		}
	},
	
	callbackActivityLoader : function(response, postdata, formid){
		try{
		$("#"+optionprofile).data('myprofileactivity').destroyLoader(optionprofile);
		 $('#gbox_'+gridloadpage).show();
			var recs = parseInt($("#"+gridloadpage).getGridParam("records"),10);
	        if (recs == 0) {
	        	  $("#"+pageroption+"_center").css('display','none');
	            var norecordsmsg = '<div class="norecords-msg-edit-page">'+Drupal.t('MSG550')+'</div>';
	  					$("#gview_"+gridloadpage+" .ui-jqgrid-hdiv").after(norecordsmsg);
	  					 $('.norecords-msg-edit-page').css('padding-top','10px');
	        } else {
	        	$('.norecords-msg-edit-page').css('display','none');
	        	$(".no-orderrecords").html("");
	        }
	        $('#first_'+pageroption).click( function(e) {
	        	if(!$('#first_'+pageroption).hasClass('ui-state-disabled')) {
						$('#gbox_'+gridloadpage).show();
						$('#gview_'+gridloadpage).css('min-height','100px');
						//$("#myprofile-myactivity-screen").data("myprofileactivity").createLoader('myprofile-myactivity-screen');
						$("#"+optionprofile).data('myprofileactivity').createLoader(optionprofile);
					}
				});
				
	        $('#next_'+pageroption).click( function(e) {
	        	if(!$('#next_'+pageroption).hasClass('ui-state-disabled')) {
						$('#gbox_'+gridloadpage).show();
						$('#gview_'+gridloadpage).css('min-height','100px');
						//$("#myprofile-myactivity-screen").data("myprofileactivity").createLoader('myprofile-myactivity-screen');
						$("#"+optionprofile).data('myprofileactivity').createLoader(optionprofile);
					}
				});
	  
	      	$('#prev_'+pageroption).click( function(e) {
	      		if(!$('#prev_'+pageroption).hasClass('ui-state-disabled')) {
	  					$('#gbox_'+gridloadpage).show();
	  					$('#gview_'+gridloadpage).css('min-height','100px');
	  					//$("#wizarad-myprofile-activity").data("myprofileactivity").createLoader('wizarad-myprofile-activity');
	  					$("#"+optionprofile).data('myprofileactivity').createLoader(optionprofile);
	  				}
	  			});
	  			
	      	 $('#last_'+pageroption).click( function(e) {
	      		if(!$('#last_'+pageroption).hasClass('ui-state-disabled')) {
							$('#gbox_'+gridloadpage).show();
							$('#gview_'+gridloadpage).css('min-height','100px');
							//$("#myprofile-myactivity-screen").data("myprofileactivity").createLoader('myprofile-myactivity-screen');
							$("#"+optionprofile).data('myprofileactivity').createLoader(optionprofile);
						}
					});
	  			$('.ui-pg-selbox').bind('change',function() {
	  				$('#gbox_'+gridloadpage).show();
	  				$('#gview_'+gridloadpage).css('min-height','100px');
	  				$("#"+optionprofile).data("myprofileactivity").hideActivityPageControls(false);
	  				//$("#wizarad-myprofile-activity").data("myprofileactivity").createLoader('wizarad-myprofile-activity');
	  				$("#"+optionprofile).data('myprofileactivity').createLoader(optionprofile);
	  			});			
	  			$(".ui-pg-input").keyup(function(event){				
	  				if(event.keyCode == 13){
	  					$('#gbox_'+gridloadpage).show();
	  					$('#gview_'+gridloadpage).css('min-height','100px');
	  					//$("#wizarad-myprofile-activity").data("myprofileactivity").createLoader('wizarad-myprofile-activity');
	  					$("#"+optionprofile).data('myprofileactivity').destroyLoader(optionprofile);
	  				}
	  			});
	  			
					$('#gbox_'+gridloadpage+' tr.ui-jqgrid-labels').css("background", "none");
	  			Drupal.attachBehaviors();
	  			//	$("#"+optionprofile).data('myprofileactivity').destroyLoader("+optionprofile+");
	        var obj = $("#"+optionprofile).data("myprofileactivity");
	        // Show pagination only when search results span multiple pages
	       	var reccount = parseInt($("#"+gridloadpage).getGridParam("reccount"),10); 
	        var hideAllPageControls = true;
	        if (recs > 15) { // 15 is the least view per page option.
	          hideAllPageControls = false;
	        }	  
	        //console.log('callbackLoader() : reccount = ' + reccount);
	        if (recs <= reccount) {
	          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
	          obj.hideActivityPageControls(hideAllPageControls);
	        }
	        else {
	          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
	          obj.showActivityPageControls();
	        }    
		   $('#'+gridloadpage+' > tbody > tr').last().find('.activity-seperate').css('border','0px');
		   $("#learner-myactivity-details .ui-jqgrid-bdiv").css("overflow","hidden");
		   $("#gbox_myprofile-activity-details").css("margin","0 auto");
		   // $("#activity-pager").css("height","21px");
		   $("#activity-pager").css("width","660px");
		   vtip();
		   $('#learner-myactivity-details .limit-title').trunk8(trunk8.profileact_title);
		}catch(e){
			// to do
		}
	},
	hideActivityPageControls : function(hideAll) {
	try{	
    var lastDataRow = $('#myaccount-orders-details tr.ui-widget-content').filter(":last");
    
    if (hideAll) {
     if(this.currTheme == "expertusoneV2")
      $('#learner-myactivity-details .block-footer-left').show(); 	
      $('#'+pageroption).hide();
      
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hideOrderPageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      $('#'+pageroption).show();
      if(this.currTheme == "expertusoneV2")
       $('#learner-myactivity-details .block-footer-left').hide();
      $('#'+pageroption+' #'+pageroption+'_center #first_'+pageroption+'').hide();
      $('#'+pageroption+' #'+pageroption+'_center #next_'+pageroption+'').hide();
      $('#'+pageroption+' #'+pageroption+'_center #prev_'+pageroption+'').hide();
      $('#'+pageroption+' #'+pageroption+'_center #last_'+pageroption+'').hide(); 
      $('#'+pageroption+' #'+pageroption+'_center #sp_1_'+pageroption+'').parent().hide();
    }
	}catch(e){
		// to do
	}
  },
  
  showActivityPageControls : function() {
	 try{
    //console.log('showOrderPageControls() : show all control');
    $('#'+pageroption).show();
    if(this.currTheme == "expertusoneV2")
     $('#learner-myactivity-details .block-footer-left').hide();
    $('#'+pageroption+' #'+pageroption+'_center #first_'+pageroption+'').show();
    $('#'+pageroption+' #'+pageroption+'_center #next_'+pageroption+'').show();
    $('#'+pageroption+' #'+pageroption+'_center #prev_'+pageroption+'').show();
    $('#'+pageroption+' #'+pageroption+'_center #last_'+pageroption+'').show(); 
    $('#'+pageroption+' #'+pageroption+'_center #sp_1_'+pageroption+'').parent().show();
	 }catch(e){
			// to do
		}
  },
  
	paintActivitySearchResults : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['details'];
		}catch(e){
			// to do
		}
	},
	
	paintActivityActions : function(cellvalue, options, rowObject) {	
		try{
		return rowObject['actions'];
		}catch(e){
			// to do
		}
	},
	/*
	 * renderMyActivity() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderMyActivity: function() {
		try{
		this.createLoader(optionprofile);
		// var obj = $("#"+optionprofile).data("myprofileactivity");
		var objStr = $("#"+optionprofile).data('myprofileactivity');
		//alert(objStr.toSource());
		var obj = this;
		// Construct the jqGrid
		
		if(hiddenuserid != 'emptyvalue') {
			col_names = ['Details'];
			col_model = [{name:'Details',index:'Details', title:false, width:595,'widgetObj':objStr,formatter:obj.paintActivitySearchResults}];
		}
		else {
			col_names = ['Details','Actions'];
			col_model = [{name:'Details',index:'Details', title:false, width:560,'widgetObj':objStr,formatter:obj.paintActivitySearchResults},
					  			{name:'Actions',index:'Actions', title:false,width:50,'align':'right','widgetObj':objStr,formatter:obj.paintActivityActions,hidden : ((obj.currTheme == "expertusoneV2") ? false : true)}];
		}
		$("#"+gridloadpage).jqGrid({
			  url: this.constructUrl("learning/my-profile/activity/"+hiddenuserid),
			  datatype: "json",
			  mtype: 'GET',
			  colNames:col_names,
			  colModel:col_model,
 			    rowNum:15,           
  				rowList:[15,20,30,40],
 				pager: '#'+pageroption,
 				viewrecords:  true,
 				emptyrecords: "",
 				sortorder: "desc",
 				toppager:false,
 				height: 'auto',
 				autowidth: true,
                shrinkToFit: true,
 				loadtext: "",
 				recordtext: "",
 				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
 				loadui:false,
 				hoverrows:false,	
			  loadComplete:obj.callbackActivityLoader
		}).navGrid('#'+pageroption,{add:false,edit:false,del:false,search:false,refreshtitle:true});
		}catch(e){
			// to do
		}
	},
	
	callDeleteProcess : function(entityId, entity_type, functionality_name,activityid){
		try{
		$('#delete-msg-wizard').remove();
		this.createLoader('wizarad-myprofile-activity'); 
		var obj = this;
			url = obj.constructUrl("learning/my-profile/activity/remove/"+ entityId +"/"+ entity_type +"/"+functionality_name+"/"+activityid);
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#'+gridloadpage).setGridParam({url: obj.constructUrl('learning/my-profile/activity/'+hiddenuserid)});
					$("#"+gridloadpage).trigger("reloadGrid",[{page:1}]);
				}
		    });
		}catch(e){
			// to do
		}
	},	
	
	displayDeleteWizard : function(title, entityId, entity_type, functionality_name ){
		
		try{
			
		var wSize =  300;
	    $('#delete-msg-wizard').remove();
	    var html = '';	    
	    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
	    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
		
	   // html+= '<tr><td height="40px">&nbsp;</td></tr>';
	    html+= '<tr><td color="#333333" style="padding: 30px 15px; text-align: justify;">'+title+''+'?'+'</td></tr>';
	    html+='</table>';
	    html+='</div>';
	    $("body").append(html);
	    var closeButt={};
	    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
	    
	    
	    	closeButt[Drupal.t('Yes')]=function(){ $("#wizarad-myprofile-activity").data('myprofileactivity').callDeleteProcess(entityId,entity_type,functionality_name); };
	    
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
	    
	    $("#delete-msg-wizard").show();
	
		$('.ui-dialog-titlebar-close').click(function(){
	        $("#delete-msg-wizard").remove();
	    });
		}catch(e){
			// to do
		}
	}
});

$.extend($.ui.myprofileactivity.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
  if(document.getElementById('myprofile-myactivity-screen')){	
  	$("#myprofile-myactivity-screen").myprofileactivity();
	}else{
		$("#wizarad-myprofile-activity").myprofileactivity();
	}
	}catch(e){
		// to do
	}
});





