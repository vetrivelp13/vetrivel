(function($) {

$.widget("ui.learnerannouncement", {
	_init: function() {
		try{
		var self = this;
		this.renderMyLearningAnnouncement();
		}catch(e)
		{
			// to do
		}
	},

	/*
	 * paintMyLearningAnnouncement() - called by jqgrid to render each cell
	 */
	paintMyLearningAnnouncement: function(cellvalue, options, rowObject) {
	 try{	
		var cTitle = rowObject['title'];
		var fullTitle = rowObject['full_title'];
		var shortdesc = rowObject['shortdesc'];
		var html = '';
		
 		html += '<div class="mylearning-announcement-item clearfix">';
 	    html += '<table border="0" cellpadding="0" cellspacing="0" class="mt-content-table"><tr>';
 	    html +=   '<td>';
 	    html +=   	'<div class="spotlight-block">';
 	    html +=   		'<p class="spotlight-block">';
 	    html +=   		'<a class="spotlight-item-title vtip" title="'+htmlEntities(fullTitle)+'">'+cTitle+'</a>';
 	    html +=   		'<span class="spotlight-title-breaker">&nbsp;</span>';
 	    html +=   		shortdesc;
 	    html +=   	'</div>';
 	    html +=   '</td>';
 	    html +=   '</tr></table>';
 	    html += '</div>';

 	    return html;
	 }catch(e)
		{
			// to do
		}
	},

	/*
	 * renderMyLearningAnnouncement() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderMyLearningAnnouncement: function() {
		try{
		// Use our loader object to signal start of jqGrid AJAX operation
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below
		var obj = $("#learner_mylearning_announcement" ).data('learnerannouncement');
		// Construct the jqGrid
		$("#my_learning_announcement_jqgrid").jqGrid({
			url: this.constructUrl("learning/announcement"),
			datatype: "json",
			mtype: 'GET',
			colNames:[''],
			colModel:[{
				name: 'id',
				index: 'id',
				width: 268,
				title: false,
				sortable: false,
				formatter: obj.paintMyLearningAnnouncement
				}],

			rowNum: $('#exp_sp_mylearning_announcement_block_max_list_count').val(),
			viewrecords: true, 
			loadui: false,
			height: 'auto',
			header: false,
			pager: '#my_learning_announcement_pager',
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			toppager: false,
			emptyrecords: "",
			loadtext: "",
			recordtext: "",
			beforeRequest: obj.myLearningAnnouncementBeforeRequest,
			gridComplete: obj.myLearningAnnouncementGridComplete,
			loadComplete: obj.myLearningAnnouncementLoadComplete
		});
		$('.ui-jqgrid').css('margin','0px');
		}catch(e)
		{
			// to do
		}
	},
	
	/*
	 * myLearningAnnouncementBeforeRequest() - Using this function to set loader image
	 */	
	myLearningAnnouncementBeforeRequest : function() {
	 try{	
		$("#learner_mylearning_announcement" ).data('learnerannouncement').createLoader('mylearning_announcement_loader');
	 }catch(e)
		{
			// to do
		}
	},

	/*
	 * myLearningAnnouncementGridComplete() - Using this to find a list of announcement in queue
	 */		
	myLearningAnnouncementGridComplete : function(data) {
		try{
		$('#gbox_my_learning_announcement_jqgrid .ui-jqgrid-hdiv').remove();
		$('#gbox_my_learning_announcement_jqgrid').show();
		$('#prev_my_learning_announcement_pager').removeClass("ui-state-disabled");
		$('#next_my_learning_announcement_pager').removeClass("ui-state-disabled");
		$('#learner_mylearning_announcement').data('learnerannouncement').destroyLoader('mylearning_announcement_loader');
		$('#my_learning_announcement_pager_left, #my_learning_announcement_pager_right').hide();
		$('.ui-pg-table').attr('align','right');
		$('#my_learning_announcement_jqgrid').find('.mylearning-announcement-item').last().css({'border':'none','padding-bottom':'8px'});
		$('#my_learning_announcement_jqgrid *').css("border-right","none 0px");
		}catch(e)
		{
			// to do
		}
	},
	
	/*
	 * myLearningAnnouncementGridComplete() - Using this function to show and hide a pagination
	 */	
	myLearningAnnouncementLoadComplete : function(data) {
      try{
		var maxLimit = parseInt($('#exp_sp_mylearning_announcement_block_max_list_count').val());
		if(data.records <= maxLimit) {
			$('#my_learning_announcement_pager').hide();
		}
		if(parseInt(data.records) > 0) {
			$('.mylearning-announcement-inner').css('min-height','auto');
		}
		var dataIDs = $('#my_learning_announcement_jqgrid').jqGrid('getDataIDs');
		if (dataIDs.length <= 0) {
		   $('#mylearning_announcement_msg').show();
		   $("#mylearning_announcement_list").hide();
		} else {
		   $('#mylearning_announcement_msg').hide();
		   $('#mylearning_announcement_list').show();
		}

		//Vtip-Display toolt tip in mouse over
		 vtip();		
      }catch(e)
		{
			// to do
		}
	}
});

$.extend($.ui.learnerannouncement.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);

$(function() {
	try{
	$("#learner_mylearning_announcement").learnerannouncement();
	}catch(e)
	{
		// to do
	}
});





