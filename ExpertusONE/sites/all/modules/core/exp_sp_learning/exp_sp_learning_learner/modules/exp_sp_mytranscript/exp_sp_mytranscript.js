(function($) {

$.widget("ui.mytranscript", {

	_init: function() {
		try{
		var self = this;
		this.myTransUserId = this.getLearnerId();
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var getUrlPage =document.location.toString();
		var urlparts = getUrlPage.split('?q=');
		var host = urlparts[1];
		if(host.search('my-profile') > 1){
			this.renderMyTranscript();
		} 	
		this.totalRecords = 0;
		this.pageNo = 0;
		}catch(e){
			// to do
		}
	},
	
	showMore: function(obj) {
		try{
		if($(obj).hasClass('tree')) {
			$(obj).css('visibility','hidden').parents('table').next().toggle().css("width","100%");
		} else {
			$(obj).parents(".mt-action-table").toggle().prev("table").find("span.tree").css("visibility","visible");
		}
		}catch(e){
			// to do
		}
	},
	
	/*
	 * paintMyTranscript() - called by jqgrid to render each cell
	 */
	paintMyTranscript: function(cellvalue, options, rowObject) {
		try{
//		console.log(rowObject);
		var cTitle = rowObject['title'];
		var shortTitle = rowObject['short_title'];
		var object_type_name = rowObject['object_type_name'];
		var enrollID = rowObject['enroll_id'];
		var compDate = rowObject['comp_date'];
		var classID = rowObject['class_id'];
		var cScore = rowObject['score'];
		var cGrade = rowObject['grade'];
		var progType = rowObject['type'];
		var imagePath = rowObject['image_url'];
		var pagePath = rowObject['argument_type'];
		var cstatus =  rowObject['cstatus'];
		var hostName = $("#div_my_transcript").data('mytranscript').constructUrl('learning/class-details/');
		var objStr = "$('#div_my_transcript').data('mytranscript').showMore(this)";		
		var html = '';
		var seperator = "";
		var infoArr = []; 
		var trans_del_type;
		var wopen='';
		
		if(object_type_name) {
			if(object_type_name =='Web-based'){
				trans_del_type = Drupal.t('Web-based');
			}
			else if(object_type_name == 'Virtual Class'){
				trans_del_type = Drupal.t('Virtual Class');
			}
			else if(object_type_name == 'Video'){
				trans_del_type = Drupal.t('Video');
			}
			else if(object_type_name == 'Classroom'){
				trans_del_type = Drupal.t('Classroom');
			}
			else if(object_type_name == 'Certification'){
				trans_del_type = Drupal.t('Certification');
			}
			else if(object_type_name == 'Curricula'){
				trans_del_type = Drupal.t('Curricula');
			}
			else if(object_type_name == 'Learning Plan'){
				trans_del_type = Drupal.t('Learning Plan');
			}}
		
		
		(compDate == "-")? "" : infoArr.push('<span class="class-info vtip" title="'+Drupal.t("LBL027")+'"> '+Drupal.t("LBL027")+': '+compDate+'</span>');
		if(pagePath!='mycertificate'){
		if(cScore != '0'){
		(cScore == "-")? "" : infoArr.push('<span class="class-info vtip" title='+Drupal.t("LBL668")+'> '+Drupal.t("LBL668")+': '+cScore+'</span>');
		}
		}
		//(cGrade == "-")? "" : infoArr.push('<span class="class-info vtip" title="Grade">Grade: '+cGrade+'</span>');
		/*if(pagePath=='mycertificate'){
		addtxt = '<span class="print-certificate-link"><a href="javascript:void(0)" onclick=\'window.open(\"printcertificate.php?enrollid=' + enrollID+ '&classid=' + classID + '&userid=' + $("#div_my_transcript").data('mytranscript').getLearnerId() + '&certifyfrom=' + progType + '\", \"PrintCertificate\", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, scrollbars=yes, copyhistory=yes, resizable=no\")\'>'+ Drupal.t('LBL308')+'</a></span>';
		}else {
			addtxt='';
		}*/
 		html += '<div class="my-transcript-item clearfix">';
 	    html += '<table border="0" cellpadding="0" cellspacing="0" class="mt-content-table"><tr>';
 	    html +=   '<td class="my-transcript-image">';
 	    html +=   	'<div><div title="' +trans_del_type + '" class="'+ imagePath +' vtip"></div></div>';
 	    html +=   '</td>';
 	    html +=   '<td class="my-transcript-desc">';
 	    /*-- #40920 - dblclick issue fix --*/
 	    //html +=     '<div class="profile-my-certificate"><span class="transcript-item-title vtip" data-url="printcertificate.php?enrollid=' + enrollID+ '&classid=' + classID + '&userid=' + $("#div_my_transcript").data('mytranscript').myTransUserId + '&certifyfrom=' + progType + '" )\' title="'+cTitle.replace(/"/g, '&quot;')+'" >'+titleRestricted(shortTitle,(($("#div_my_transcript").data('mytranscript').currTheme == "expertusoneV2") ? 17 : 25))+'</span></div>';
 	  

 	   /*
  	    * Start # 0041917 -  Qa link is shown in salesforce app
  	    * Added By : Ganesh Babu V, Dec 9th 2014 5:00 PM
  	    * Description: Check the print certificate whether it triggers from salesforce or expertusone. callback changed according to trigger 
  	    * Ticket : #0041917 -  Qa link is shown in salesforce app
  	    */    

  	    // 		  html += '<div class="profile-my-certificate"><span class="transcript-item-title vtip" onclick=\'window.open(\"printcertificate.php?enrollid=' + enrollID+ '&classid=' + classID + '&userid=' + $("#div_my_transcript").data('mytranscript').myTransUserId + '&certifyfrom=' + progType + '\", \"PrintCertificate\", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, scrollbars=yes, copyhistory=yes, resizable=yes\")\' title="'+cTitle.replace(/"/g, '&quot;')+'" >'+titleRestricted(shortTitle,(($("#div_my_transcript").data('mytranscript').currTheme == "expertusoneV2") ? 17 : 25))+'</span></div>';
   	    
 	   var title = Drupal.t('LBL205');
	   var message = Drupal.t('MSG755'); 
  	    
 	   if(cstatus=='Y') {
 		  wopen = '\'window.open(\"printcertificate.php?enrollid=' + enrollID+ '&classid=' + classID + '&userid=' + $("#div_my_transcript").data('mytranscript').myTransUserId + '&certifyfrom=' + progType + '\", \"PrintCertificate\", \"width=1050, height=900, toolbar=yes, location=yes, directories=yes, status=yes, menubar=yes, scrollbars=yes, copyhistory=yes, resizable=yes\")\'';
 	   }else {
 		  wopen = '\'$("body").data("learningcore").callMessageWindow(\"'+title+'\",\"'+message+'\")\'';
 	   }
  	    
  	   if($( "#salesforce_my_transcripts" ).length) {
  	      html += '<div class="profile-my-certificate"><span class="transcript-item-title vtip" onclick="$(\'body\').data(\'printcertificate\').getPrintcertificateDetails(\''+enrollID+'\',\''+classID+'\','+$("#div_my_transcript").data('mytranscript').myTransUserId+',\''+progType+'\',\'div_my_transcript\',800,900);" title="'+cTitle.replace(/"/g, '&quot;')+'" >'+shortTitle+'</span></div>';  	    
  	   }else{
  		  html += '<div class="profile-my-certificate  limit-title-row"><span class="limit-title  transcript-item-title vtip" onclick='+wopen+' title="'+cTitle.replace(/"/g, '&quot;')+'" >'+shortTitle+'</span></div>';
  	   }
  	   
  	  /* End # 0041917 -  Qa link is shown in salesforce app */
 	    
 	    html +=   	'<div class="my-class-info clearfix">';
 	    
 	    $.each(infoArr, function(i, val){
 	    	html += seperator+val;
 	    	seperator = '<span class="seperator">|</span>';
 	    });
 	    
 	    html +=   	'</div>';
 	    html +=   '</td>';
 	    html +=   '</tr></table>';
 	    html += '</div>';

 	    return html;
		}catch(e){
			// to do
		}
	},

	/*
	 * renderMyTranscript() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderMyTranscript: function() {
		// Use our loader object to signal start of jqGrid AJAX operation
		
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below	
		try{
			var obj = $("#div_my_transcript" ).data('mytranscript');
			var getUrlPage =document.location.toString();
			var urlparts = getUrlPage.split('?q=');
			var host = urlparts[1];
			if(host.search('my-profile') > 1){
			 	if(this.currTheme == "expertusoneV2"){
					var TranWidth = 260;	
				}else{
					var TranWidth = 268;
			    }
			}else{						
				var TranWidth = $("#div_my_transcript_start").width();	
			}
	 
			
		
		var jqGridOptions = {

//				url: newconsturl,
				datatype: "json",
				mtype: 'GET',
				colNames:[''],
				colModel:[{
					name: 'id',
					index: 'id',
					width: TranWidth,
					title: false,
					sortable: false,
					formatter: obj.paintMyTranscript
					}],
					
				rowNum: parseInt($('#exp_sp_mytranscript_block_max_list_count').val()),
				viewrecords: true, 
				loadui: false,
				height: 'auto',
				header: false,
				pager: '#my_transcript_pager',
				pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
				toppager: false,
				emptyrecords: "",
				loadtext: "",
				recordtext: "",
				beforeRequest: obj.myTranscriptBeforeRequest,
				gridComplete: obj.myTranscriptGridComplete,
				loadComplete: obj.myTranscriptLoadComplete,
				totalRecords: 0,
				pageNo:0
			
		};
		
		if(host.search('my-profile') > 1){
			jqGridOptions.url = this.constructUrl("learning/my-transcript/mycertificate");
		}else {
		  	
			
			jqGridOptions.url = this.constructUrl("learning/my-transcript/mytranscript");
			jqGridOptions.rowNum = 10;
			jqGridOptions.scroll = false;
			jqGridOptions.pager =  '';
		/*	jqGridOptions.gridComplete = function(){
				$('.ui-jqgrid-bdiv').jScrollPane({
					showArrows:true,
					//maintainPosition: false,
					showArrows: true, scrollbarWidth: 17, arrowSize: 17, scrollbarMargin: 0
				});
			};*/
			/*jqGridOptions.loadComplete = function() {
				obj.myTranscriptLoadComplete();
				}*/
			
		}
		// Construct the jqGrid
//		console.log(jqGridOptions);
		$("#my_transcript_jqgrid").jqGrid(jqGridOptions);
		$('.ui-jqgrid').css('margin','0px');
		//$('#gbox_my_transcript_jqgrid').css('width','500px');
		}catch(e){
			// to do
			console.log(e);
		}
	},
	
	myTranscriptBeforeRequest : function() {
		try{
		$("#div_my_transcript" ).data('mytranscript').createLoader('div_my_transcript_start');
		}catch(e){
			// to do
		}
	},
	
	myTranscriptGridComplete : function() {
		try{
		$('#gbox_my_transcript_jqgrid .ui-jqgrid-hdiv').remove();
		$('#gbox_my_transcript_jqgrid').show();
	/*	$('#first_my_transcript_pager').removeClass("ui-state-disabled");
		$('#prev_my_transcript_pager').removeClass("ui-state-disabled");
		$('#next_my_transcript_pager').removeClass("ui-state-disabled");
		$('#last_my_transcript_pager').removeClass("ui-state-disabled");*/
		$('#div_my_transcript').data('mytranscript').destroyLoader('div_my_transcript_start');
		$('#my_transcript_pager_left, #my_transcript_pager_right').hide();
		$('.ui-pg-table').attr('align','right');
		$('#my_transcript_jqgrid *').css("border-right","none 0px");
/*		var scroll_header = $('#gview_my_transcript_jqgrid').find('.ui-jqgrid-hbox').css("position","relative");
        $('#gview_my_transcript_jqgrid').find('.ui-jqgrid-bdiv').bind('jsp-scroll-y', function(event, scrollPositionY, isAtTop, isAtBottom) {
        	     scroll_header.css('bottom', scrollPositionY);
                      }).jScrollPane({
                        showArrows: true, 
                       autoReinitialise: true,
                       maintainPosition: false
                       //horizontalDragMaxWidth: 30,
                       //verticalDragMaxHeight: 30           
                   });
              scrollGrid(this);*/
		}catch(e){
			// to do
		}
	},
	
	/*
	 * myTranscriptLoadComplete() - called after server request
	 */	
	myTranscriptLoadComplete : function(data) {
		try{
		var maxLimit = parseInt($('#exp_sp_mytranscript_block_max_list_count').val());
		$("#my_transcript_jqgrid").setGridParam("totalRecords", data['records']);
		$("#div_my_transcript").data('mytranscript').totalRecords = data['records'];
		$("#my_transcript_jqgrid").setGridParam("pageNo", data['page']);
		$("#div_my_transcript").data('mytranscript').pageNo = data['page'];
		//alert('myTranscriptLoadComplete max limit'+maxLimit);
		if(data['records'] <= maxLimit) {
			$('#my_transcript_pager').hide();
		}
		
/*		if(parseInt(data.records) > 0) {
			$('.my-transcript-inner').css('min-height','auto');
		}*/
		var dataIDs = $('#my_transcript_jqgrid').jqGrid('getDataIDs');
		//alert(dataIDs.length);
		
		//Added by ganeshbabuv to avoid the issue for not showing the enrollment message if there are no enrollments, which comes from salesforce app Ref: SF cookieless option #0054508 on 30th sep 2015 10:40 AM  
		var data_from_sf = $('#salesforce_canvas_widget').attr('data-from-salesforce');  
		if (typeof data_from_sf !== typeof undefined && data_from_sf !== false && data_from_sf=="1"){			 
			setTimeout(function(){ 
				if (dataIDs.length <= 0) {
				   $('#my_transcript_msg').show();
				   $("#my_transcript_list").hide();
				} else {
				   $('#my_transcript_msg').hide();
				   $('#my_transcript_list').show();
				}
			},200);
			
		}else{ 
			if (dataIDs.length <= 0) { 
				   $('#my_transcript_msg').show();
				   $("#my_transcript_list").hide();
				} else {
				   $('#my_transcript_msg').hide();
				   $('#my_transcript_list').show();
				} 
		} 
		
		$('.my-transcript-item').removeClass("my-transcript-last");
		$('.my-transcript-item').eq(-1).addClass("my-transcript-last");

                //#82152 - Certificates are not shown in profile, UI issue occurs in My learning for first time
                var getUrlPage =document.location.toString();
		var urlparts = getUrlPage.split('?q=');
		var host = urlparts[1];
                if(host.search('enrollment-search') > 1){ 
		        var dataIDs = $("#div_my_transcript").data('mytranscript').totalRecords;
			if (dataIDs <= 0) {
				$('#my_transcript_msg').show();
				$("#my_transcript_list").hide();
			} else {
				$('#my_transcript_msg').hide();
				$('#my_transcript_list').show();
			}
			vtip();
			var height = 0;
			var total_pages = Math.ceil(dataIDs / 10);
			var pagenum = 1;
			// console.log(dataIDs);
			$('#my_transcript_jqgrid tr.ui-widget-content:not(.jqgfirstrow):lt(8)').each(function(item) {
				height += $(this).height();
			});
			$('#div_my_transcript_start').css({'height': height + 'px', 'overflow': 'hidden'});
			if (dataIDs > 8) {
				initializeJScrollPane('transcript');
			}
                }
		
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	 	/*if(this.currTheme == "expertusoneV2"){
	 		$('#my_transcript_jqgrid .my-transcript-item:last').css('height','51px');
	 	}*/
		//Vtip-Display toolt tip in mouse over
		vtip();
		$('#my_transcript_list .limit-title').trunk8(trunk8.transcript_title);
		}catch(e){
			// to do
		}
	}
	
});

$.extend($.ui.mytranscript.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
	$("#div_my_transcript").mytranscript();
	}catch(e){
		// to do
	}
});
