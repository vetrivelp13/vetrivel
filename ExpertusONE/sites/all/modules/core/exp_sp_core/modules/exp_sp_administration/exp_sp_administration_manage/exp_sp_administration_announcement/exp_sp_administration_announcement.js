(function($) {
	
$.widget("ui.announcement", {
	
	setIntervalId : null, // The id returned by setInterval is saved here.
	
	/*
	 * _init() - The init function of this widget
	 */
	_init: function() {
		try{
		this.loggedUserId = this.getLearnerId();
		this.renderAnnouncementList();
		}catch(e){
			// to do
		}
	},

	/*
	 * paintAnnouncement() - called by jqgrid to render each cell
	 */
	paintAnnouncement: function(cellvalue, options, rowObject) {
		try{
		var drupalUserId = rowObject['id'];
		var personid = rowObject['personid'];
		var userName = rowObject['screen_name'];
		var userImagePath = rowObject['image_url'];
		var noticeId = rowObject['nid'];
		var shortDescription = rowObject['announce_shortdesc'];
		var fulldescription = rowObject['announce_fulldescription'];
		var dateCreated = rowObject['from_date'];
		
		var css = rowObject['css'];
		var data1="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+personid+"','popupDispId':'profile-qtipid-"+drupalUserId+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_"+drupalUserId+"','dataloaderid':'div_announcement','dataIntId':'announcement','wBubble':'350','hBubble':'auto'}";
		var html = '';
		if(!document.getElementById("qtipprofileqtip_visible_disp_"+drupalUserId+"_disp")){
			html += '<div id="qtipprofileqtip_visible_disp_'+drupalUserId+'_disp"></div>';
		}
 		html += '<div class="profile-item clearfix '+ css + '" id="announcement-qtipid-'+drupalUserId+'">'; // clearfix class is needed to be able to paint a border at the bottom of div
 	    html +=   '<a class="profile-image user-list-border-img" >';
 	    html +=     '<img class="vtip" title="' + htmlEntities(userName) + '" src="' + userImagePath + '"/>';
 	    html +=   '</a>';
 	    html +=   '<div class="announcement-profile-desc">';
 	    html +=     '<span class="user-profile-link">';
 	    html +=       '<a id="' + drupalUserId + '" class="limit-title-row user-name user-profile" href="javascript:void(0);" onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')">';
 	    html +=         '<span class="limit-title vtip" title="'+htmlEntities(rowObject['screen_name'])+'">'+userName+'</span>';
 	    html += 		'<a title="'+Drupal.t("LBL286")+'" onclick="$(\'#div_announcement\').data(\'announcement\').callDismissNotice('+noticeId+','+$("#div_announcement" ).data('announcement').loggedUserId+')" class="activity-remove-link vtip">';
 	    html +=         '<span class="vtip" title="'+Drupal.t("LBL286")+'">'+Drupal.t("LBL286")+'</span>';
 	    html +=       '</a>';
 	    html +=       '<span class="created-date">' + dateCreated + '</span>';
 	    //42929: If the Announcement description contains "%" characters, then the learner site it shows "undefined"(removed uridecode funtion)
 	    html +=       '<div class="limit-desc-row"><span class="limit-desc announce-Description vtip">' + fulldescription + '</span></div>';
 	    html +=     '</span>';
 	    html +=   '</div>';
 	    //html += '</span></div>';
 	    html += '</div>';

 	    return html;
 	   //changeDialogPopUI();
		}catch(e){
			// to do
		}
	},
	/*
	 * paintAnnouncement() - called by jqgrid to render each cell
	 */
	paintAnnouncementNewTheme: function(rowObject) {
		try{
		var drupalUserId = rowObject['id'];
		var personid = rowObject['personid'];
		var userName = rowObject['screen_name'];
		var userImagePath = rowObject['image_url'];
		var noticeId = rowObject['nid'];
		var shortDescription = rowObject['announce_shortdesc'];
		var fulldescription = rowObject['announce_fulldescription'];
		var dateCreated = rowObject['from_date'];
//		console.log(rowObject['announce_fulldescription']);
		var css = rowObject['css'];
		var data1="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+personid+"','popupDispId':'profile-qtipid-"+drupalUserId+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_"+drupalUserId+"','dataloaderid':'announcement_new','dataIntId':'announcement','wBubble':'350','hBubble':'auto'}";
		var html = '';
		if(!document.getElementById("qtipprofileqtip_visible_disp_"+drupalUserId+"_disp")) {
			html += '<div id="qtipprofileqtip_visible_disp_'+drupalUserId+'_disp"></div>';
		}
 		html += '<div class="profile-item clearfix '+ css + '" id="announcement-qtipid-'+drupalUserId+'">'; // clearfix class is needed to be able to paint a border at the bottom of div
 	    html +=   '<a class="profile-image user-list-border-img" >';
 	    html +=     '<img class="vtip" title="' + htmlEntities(userName) + '" src="' + userImagePath + '" width="27px" height="27px"/>';
 	    html +=   '</a>';
 	    html +=   '<div class="announcement-profile-desc">';
 	    html +=     '<span class="user-profile-link">';
 	    html +=       '<a id="' + drupalUserId + '" class="limit-title-row user-name user-profile" href="javascript:void(0);" onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')">';
 	    html +=         '<span class="vtip limit-title" title="'+htmlEntities(rowObject['screen_name'])+'">'+userName.toUpperCase()+'</span>';
 	    // html += 		'<a title="'+Drupal.t("LBL286")+'" onclick="$(\'#announcement_new\').data(\'announcement\').callDismissNotice('+noticeId+','+$("#announcement_new" ).data('announcement').loggedUserId+')" class="activity-remove-link vtip">';
 	    // html +=         '<span class="vtip" title="'+Drupal.t("LBL286")+'">'+Drupal.t("LBL286")+'</span>';
 	    html +=       '</a>';
 	    html +=       '<span class="created-date">' + dateCreated + '</span>';
 	    //42929: If the Announcement description contains "%" characters, then the learner site it shows "undefined"(removed uridecode funtion)
 	    html +=       '<div class="announce-shortdescription-list limit-desc-row" data-noticeid="announcement-'+noticeId+'"><span class="limit-desc announce-Description">' + fulldescription + '</span></div>';
 	    html +=       '<div id="announcement-'+noticeId+'" class="announce-fulldescription-list"><span class="announce-Description">' + fulldescription + '</span></div>';
 	    html +=     '</span>';
 	    html +=   '</div>';
 	    //html += '</span></div>';
 	    html += '</div>';
 	  //  html +=       '<div id="announce-fulldescription-list"><span class="announce-Description">' + fulldescription + '</span></div>';

 	    return html;
 	   //changeDialogPopUI();
		}catch(e){
			// to do
			console.log(e.stack);
		}
	},
    
	/*
	 * renderAnnouncementList() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderAnnouncementList: function() {
		try{
		// Use our loader object to signal start of jqGrid AJAX operation
		this.createLoader('announcement_loader');
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var cluserWidth = (navigator.userAgent.indexOf("Chrome")>=0) ? 258 : 253;
			var cluserWidth = (navigator.userAgent.indexOf("Safari")!=-1) ? 258 : 253;
		}else{
			var cluserWidth = 265;
	    } 
		// Construct the jqGrid
		$("#announcement_jqgrid").jqGrid({
			url: this.constructUrl("learning/announcement/"+this.loggedUserId),
			datatype: "json",
			mtype: 'GET',
			
			colNames:[''],
			colModel:[{
				name: 'id',
				index: 'id',
				width: cluserWidth,
				title: false,
				sortable: false,
				formatter: obj.paintAnnouncement
				}],
				
			rowNum: 3,//$('#exp_sp_user_catalog_block_max_list_count').val(),
			viewrecords: true, 
			loadui: false,
			height: 'auto',
			pager: '#announcement_pager',
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			toppager: false,
			emptyrecords: "",
			loadtext: "",
			recordtext: "",
			loadComplete: obj.announcementLoadComplete,
			onPaging: function(){
				obj.createLoader('announcement_loader')
				},
			//loadError: obj.catalogUsersLoadError
		});
		
		$('.ui-jqgrid').css('margin','0px');
		}catch(e){
			// to do
		}
	},

	/*
	 * announcementLoadComplete() - called after server request
	 */	
	announcementLoadComplete : function(data) {
		try{
		
	   if(data.total > 1 ) {
			$('#announcement_pager').css('display','block');
	   }
	   else{
		   $('#announcement_pager').css('display','none');
	   }
		if(data.records == 0) {
			$('#announcement_list').html("<div class='announcement-msg'  style='display: block;'>"+Drupal.t('MSG267')+"</div>");
		}
  	   //Vtip-Display toolt tip in mouse over
	   vtip();
	   $('.ui-pg-table').attr('align','right'); // align pager to right position
	   $("#div_announcement" ).data('announcement').destroyLoader('announcement_loader');
	    $('#announcement_list .limit-title').trunk8(trunk8.ann_title);
	    $('#announcement_list .limit-desc').trunk8(trunk8.ann_desc);
		}catch(e){
			// to do
		}
	},
	
	callDismissNotice : function(noticeId, user_id){
		try{
		//$('#delete-msg-wizard').remove();
		this.createLoader('announcement_loader'); 
		var obj = this;
			url = obj.constructUrl("learning/announcement/dismiss/"+ noticeId +"/"+ user_id);
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#announcement_jqgrid').setGridParam({url: obj.constructUrl('learning/announcement/'+user_id)});
					$("#announcement_jqgrid").trigger("reloadGrid",[{page:1}]);
				}
		    });
		}catch(e){
			// to do
		}
	},	
	
	
});

$.extend($.ui.announcement.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
	$("#div_announcement").announcement();	
	}catch(e){
		// to do
	}
});
function addEditTimeField(selector) {
    // Show default text instead of ''
	try{
    var fieldValue = $(selector).val();
    if (fieldValue == Drupal.t('hh:mm') || fieldValue =='') {
      $(selector).removeClass('narrow-search-filterset-daterange-nonempty');
      $(selector).addClass('narrow-search-filterset-daterange-empty');
      $(selector).val(Drupal.t('hh:mm'));
    }else{
    	$(selector).removeClass('narrow-search-filterset-daterange-empty');
        $(selector).addClass('narrow-search-filterset-daterange-nonempty');
    }
	}catch(e){
		// to do
	}
}
