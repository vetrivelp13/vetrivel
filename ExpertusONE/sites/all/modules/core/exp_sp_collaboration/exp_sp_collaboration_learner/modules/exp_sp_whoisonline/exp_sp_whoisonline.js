(function($) {

$.widget("ui.lnrwhoisonline", {
	
	setIntervalId : null, // The id returned by setInterval is saved here.
	
	/*
	 * _init() - The init function of this widget
	 */
	_init: function() {
		try{
		this.renderOnlineUsersList();
		this.totalRecords = 0;
		this.pageNo = 0;
		}catch(e){
			// to do
		}
	},

	/*
	 * paintOnlineUser() - called by jqgrid to render each cell
	 */
	paintOnlineUser: function(cellvalue, options, rowObject) {
	 try{	
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var drupalUserId = rowObject['id'];
		var userName = rowObject['screen_name'];
		var userImagePath = rowObject['image_url'];
		var jobTitle = rowObject['job_title'];
		if(jobTitle==null){
			jobTitle='';
		}
		var css = rowObject['css'];
		if(this.currTheme == "expertusoneV2"){
		var html = '';
 		html += '<div class="profile-item clearfix '+ css + '">'; // clearfix class is needed to be able to paint a border at the bottom of div
 	    html +=   '<a class="profile-image user-list-border-img">';
 	    html +=     '<img class="vtip" title="' + htmlEntities(userName) + ' ' + SMARTPORTAL.t('picture') + '" width="27" height="27" src="' + userImagePath + '"/>';
 	    html +=   '</a>';
 	    html +=   '<div class="profile-desc">';
 	    html +=     '<span class="user-profile-link limit-title-row">';
 	    html +=       '<a id="' + drupalUserId + '" class="user-name user-profile " href="javascript:void(0);">';
 	   //  html +=userName;
 	    html +=        '<span class="limit-title limit-title-whois-on  vtip" title="'+htmlEntities(rowObject['screen_name'])+'">'+userName+'</span>';
 	    html +=       '</a>';
 	    html +=       '<span class="job-title limit-title limit-title-whois-jobtitle vtip" title="'+htmlEntities(jobTitle)+'" >' + htmlEntities(jobTitle) + '</span>';
 	    html +=     '</span>';
 	    html +=   '</div>';
 	    html += '</div>';

 	    return html;
		}else {
			var html = '';
	 		html += '<div class="profile-item clearfix '+ css + '">'; // clearfix class is needed to be able to paint a border at the bottom of div
	 	    html +=   '<a class="profile-image">';
	 	    html +=     '<img class="vtip" title="' + htmlEntities(userName) + ' ' + SMARTPORTAL.t('picture') + '" width="27" height="27" src="' + userImagePath + '"/>';
	 	    html +=   '</a>';
	 	    html +=   '<div class="profile-desc">';
	 	    html +=     '<span class="user-profile-link limit-title-row">';
	 	    html +=       '<a id="' + drupalUserId + '" class="user-name user-profile" href="javascript:void(0);">';
	 	    //  html +=userName;
	 	    html +=        '<span class="limit-title limit-title-whois-on  vtip" title="'+htmlEntities(rowObject['screen_name'])+'">'+userName+'</span>';
	 	    html +=       '</a>';
	 	     html +=       '<span class="job-title limit-title limit-title-whois-jobtitle vtip" title="'+htmlEntities(jobTitle)+'" >' + htmlEntities(jobTitle) + '</span>';
	 	    html +=     '</span>';
	 	    html +=   '</div>';
	 	    html += '</div>';

	 	    return html;
	
		}
	 }catch(e){
			// to do
		}
	},

	/*
	 * renderOnlineUsersList() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderOnlineUsersList: function() {
     try{
		// Use our loader object to signal start of jqGrid AJAX operation
		//this.createLoader('expertus-online-users-loader');
		
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var whoisWidth = 220;	
		}else{
			var whoisWidth = 269;
	    }
		var obj = this;

		// Construct the jqGrid
		var jqGridOptions = {
		//	url: this.constructUrl("learning/fetch-online-users-list"),
			datatype: "json",
			mtype: 'GET',
			
			colNames:[''],
			colModel:[{
				name: 'id',
				index: 'id',
				width: whoisWidth,
				title: false,
				sortable: false,
				formatter: obj.paintOnlineUser
				}],
				
			rowNum: $('#exp_sp_whoisonline_block_max_list_count').val(),

			viewrecords: true, 
			loadui: false,

			height: 'auto',
			
			pager: '#expertus-online-datatable-pager',
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			toppager: false,
			
			emptyrecords: "",
			loadtext: "",
			recordtext: "",
			
			beforeRequest: obj.onlineBeforeRequest,
			gridComplete: obj.onlineGridComplete,
			loadComplete: obj.onlineLoadComplete,
			onPaging: obj.onlinePaging,
			loadError: obj.onlineLoadError,
			totalRecords: 0,
			pageNo:0
		}
		jqGridOptions.url = this.constructUrl("learning/fetch-online-users-list");
			jqGridOptions.rowNum = 8;
			jqGridOptions.scroll = false;
			jqGridOptions.pager =  '';
		$("#expertus-online-users-jqgrid").jqGrid(jqGridOptions);
		$('.ui-jqgrid').css('margin','0px');
     }catch(e){
			// to do
		}
	},

	/*
	 * onlineBeforeRequest() - called before server request
	 */	
	onlineBeforeRequest : function() {		
	 try{
		if ($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId != null) {
			//alert('onlineBeforeRequest : clearInterval');
			clearInterval($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId);
			$("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId = null;
		}
		//alert('onlineBeforeRequest end');
	 }catch(e){
			// to do
		}
	},

	/*
	 * onlineLoadComplete() - called after server request
	 */	
	onlineLoadComplete : function(data) {
	 try{	
	   var maxLimit = $('#exp_sp_whoisonline_block_max_list_count').val();
	   var dataIDs = $('#expertus-online-users-jqgrid').jqGrid('getDataIDs');
	    $("#expertus-online-users-jqgrid").setGridParam("totalRecords", data['records']);
		$("#expertus-online-users").data('lnrwhoisonline').totalRecords = data['records'];
		$("#expertus-online-users-jqgrid").setGridParam("pageNo", data['page']);
		$("#expertus-online-users").data('lnrwhoisonline').pageNo = data['page'];
	   if( data['records'] < maxLimit ) {
			$('#expertus-online-datatable-pager').css('display','none');
	   }
	   if (dataIDs.length <= 0) {
		   $('#expertus-no-online-users-msg').show();
		   $("#expertus-online-users-list").hide();
	   }
	   else {
		   $('#expertus-no-online-users-msg').hide();
		   $("#expertus-online-users-list").show();
	   }
	   $("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId =
		setInterval($("#expertus-online-users" ).data('lnrwhoisonline').reloadUsersGrid, 1000 * $('#exp_sp_whoisonline_block_refresh_interval').val());
	   //getUserProfile($("#expertus-online-users").data('lnrwhoisonline'), 1000 * 600); // Defined in exp_sp_formatter_user.js which gets included in exp_sp_formatter_module in its hook_init()
  		$('#expertus-online-users .limit-title-whois-on').trunk8(trunk8.whois_username_title);
  		$('#expertus-online-users .limit-title-whois-jobtitle').trunk8(trunk8.whois_jobtitle);
		
  	   //Vtip-Display toolt tip in mouse over
	   vtip();
	   $('#expertus-online-users-jqgrid tr').removeClass('expertus_online_users_last');
	   $('#expertus-online-users-jqgrid tr:last').addClass('expertus_online_users_last');
	   $("#expertus-online-users" ).data('lnrwhoisonline').refreshGrid(data);
	 }catch(e){
			// to do
			//console.log(e, e.stack);
		}
	},

	/*
	 * onlineLoadError() - On error, we stop the auto refresh
	 */	
	onlineLoadError : function(xhr, status, error) {
	 try{	
		$("#expertus-online-users" ).data('lnrwhoisonline').destroyLoader('expertus-online-users-loader');
		if ($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId != null) {
			//alert('onlineBeforeRequest : clearInterval');
			clearInterval($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId);
			$("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId = null;
		}
		//alert('Unable to load the list of currently active users: ' + status);
	 }catch(e){
			// to do
		}
	},
	
	/*
	 * reloadUsersGrid() - Function called by setInterval() to refresh the grid. We
	 *                   - halt the auto refresh, which will again be started on loadComplete.
	 */
	reloadUsersGrid : function() {
	 try{	
		//alert('onlineReloadGrid start');
		if ($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId != null) {
			//alert('onlineBeforeRequest : clearInterval');
			clearInterval($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId);
			$("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId = null;
		}
		
		$("#expertus-online-users-jqgrid").setGridParam({page: 1}).trigger('reloadGrid');
	 }catch(e){
			// to do
		}
	},

	/*
	 * reloadUsersGrid() - When the user clicks on next/prev page, we want to show the loader. We also
	 *                   - halt the auto refresh, which will again be started on loadComplete.
	 */	
	onlinePaging : function(pgButton) {
		try{
		//alert('	onlinePaging start');
		$("#expertus-online-users" ).data('lnrwhoisonline').createLoader('expertus-online-users-loader');
		if ($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId != null) {
			//alert('onlinePaging : clearInterval');
			clearInterval($("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId);
			$("#expertus-online-users" ).data('lnrwhoisonline').setIntervalId = null;
		}
		//alert('	onlinePaging end');
		}catch(e){
			// to do
		}
	},
	//reset the height and initialize jScorllPane if required
	refreshGrid: function (data) {
		try {
			var height = 0;
			var totalRecords = $("#expertus-online-users").data('lnrwhoisonline').totalRecords;
			var MaxRecToShow = 8;
			$('#expertus-online-users-jqgrid tr.ui-widget-content:not(.jqgfirstrow):lt(' + MaxRecToShow + ')').each(function (item) {
				height += $(this).height();
			});
			if (totalRecords > MaxRecToShow) {
				height -= 5;
				$('#whoisonline-block_start').css({
					'height': height + 'px',
					'overflow': 'hidden'
				});
				if ($('#whoisonline-block_start').data('jsp') !== undefined) {
					$('#whoisonline-block_start').data('jsp').reinitialise();
				} else {
					initializeJScrollPane('whoisonline');
				}
			} else {
				$('#whoisonline-block_start').css({
					'height': height + 'px',
					'overflow': 'hidden'
				});
				if ($('#whoisonline-block_start').data('jsp') !== undefined) {
					$('#whoisonline-block_start').data('jsp').destroy();
				}
			}

		} catch (e) {
			//console.log(e, e.stack);
		}
	}
});

$.extend($.ui.lnrwhoisonline.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);

})(jQuery);


$(function() {
	try{
		$( "#expertus-online-users" ).lnrwhoisonline();
	}catch(e){
		// to do
	}
});





