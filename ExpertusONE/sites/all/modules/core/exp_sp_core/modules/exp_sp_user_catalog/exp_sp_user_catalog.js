(function($) {
	try{
	//alert($('#search_users').val());
	
	/*
	 * function for load users autocomplete
	 */
	if($('#search_users_txt')) {
		$('#search_users_txt').autocomplete(
				"/?q=learning/user-catalog-autocomplete",{
				extraParams : {'exclude_logged_user' : 1,'search_by_username' : 1},
				minChars :3,
				max :50, 
				autoFill :true,
				mustMatch :false,
				matchContains :false
		});
	}
	
	$("#search_users_txt").keyup(function(event){
		try{
	    if(event.keyCode == 13){
	        $("#search_users").click();
	    }
		}catch(e){
			// to do
		}
	});
	
    
$.widget("ui.lnrcatalogusersonline", {
	
	setIntervalId : null, // The id returned by setInterval is saved here.
	
	/*
	 * _init() - The init function of this widget
	 */
	_init: function() {
		try{
		this.renderCatalogUsersList();
		}catch(e){
			// to do
		}
	},

	/*
	 * paintCatalogUsers() - called by jqgrid to render each cell
	 */
	paintCatalogUsers: function(cellvalue, options, rowObject) {
     try{
		var drupalUserId = rowObject['id'];
		var personid = rowObject['personid'];
		var userName = rowObject['screen_name'];
		var tooltipuserName = rowObject['screen_name'];
		var userImagePath = rowObject['image_url'];
		var jobTitle = rowObject['job_title'];
		if(jobTitle==null){
			jobTitle='';
		}
		//learning/user/catalog/"+drupalUserId+"
		var css = rowObject['css'];
		var data1="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+personid+"','popupDispId':'profile-qtipid-"+drupalUserId+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_"+drupalUserId+"','dataloaderid':'catalog-users','dataIntId':'lnrcatalogusersonline','wBubble':'350','hBubble':'auto'}";
		//onclick="$(\'body\').data(\'learningcore\').getLeanerQtipDiv('+data1+')"
		var html = '';
		if(!document.getElementById("qtipprofileqtip_visible_disp_"+drupalUserId+"_disp")){
			html += '<div id="qtipprofileqtip_visible_disp_'+drupalUserId+'_disp"></div>';
		}
 		html += '<div class="profile-item clearfix '+ css + '" id="profile-qtipid-'+drupalUserId+'">'; // clearfix class is needed to be able to paint a border at the bottom of div
 		//onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+rowEditOptionId+'\', \''+uniqueId+'\');"
 	    html +=   '<a class="profile-image user-list-border-img" >';
 	    html +=     '<img class="vtip" title="' + htmlEntities(tooltipuserName) + '" src="' + userImagePath + '"/>';
 	    html +=   '</a>';
 	    html +=   '<div class="profile-desc">';
 	    html +=     '<span class="user-profile-link">';
 	    html +=       '<a id="' + drupalUserId + '" class="limit-title-row user-name user-profile" href="javascript:void(0);" onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')">';
 	    html +=         '<span class="limit-title vtip" title="'+htmlEntities(rowObject['screen_name'])+'">'+userName+'</span>';
 	    html +=       '</a>';
 	    html +=       '<span class="limit-title-row job-title vtip" title="'+htmlEntities(rowObject['job_title'])+'"> <span class="limit-title">' + htmlEntities(jobTitle) + '</span></span>';
 	    html +=     '</span>';
 	    html +=   '</div>';
 	    html += '</div>';

 	    return html;
 	   changeDialogPopUI();
 	  $(".user-profile-link .user-name .limit-title").trunk8(trunk8.usercatalog_title);
 	  $(".user-profile-link .job-title .limit-title").trunk8(trunk8.usrcatjobrole_title); 
	   

     }catch(e){
 		// to do
 	}
	},
    
	/*
	 * renderCatalogUsersList() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderCatalogUsersList: function() {
		try{
		// Use our loader object to signal start of jqGrid AJAX operation
		this.createLoader('catalog-users-loader');
		
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var cluserWidth = (navigator.userAgent.indexOf("Chrome")>=0) ? 258 : 253;	
		}else{
			var cluserWidth = 265;
	    } 
		// Construct the jqGrid
		$("#catalog-users-jqgrid").jqGrid({
			url: this.constructUrl("learning/fetch-catalog-users-list/null"),
			datatype: "json",
			mtype: 'GET',
			
			colNames:[''],
			colModel:[{
				name: 'id',
				index: 'id',
				width: cluserWidth,
				title: false,
				sortable: false,
				formatter: obj.paintCatalogUsers
				}],
				
			rowNum: 5,//$('#exp_sp_user_catalog_block_max_list_count').val(),

			viewrecords: true, 
			loadui: false,

			height: 'auto',
			
			pager: '#expertus-online-datatable-pager',
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			toppager: false,
			
			emptyrecords: "",
			loadtext: "",
			recordtext: "",
			
			beforeRequest: obj.catalogUsersBeforeRequest,
			gridComplete: obj.catalogUsersGridComplete,
			loadComplete: obj.catalogUsersLoadComplete,
			onPaging: obj.catalogUsersPaging,
			loadError: obj.catalogUsersLoadError
		});
		
		$('.ui-jqgrid').css('margin','0px');
		$(".user-profile-link .user-name .limit-title").trunk8(trunk8.usercatalog_title);
	 	$(".user-profile-link .job-title .limit-title").trunk8(trunk8.usrcatjobrole_title); 
		}catch(e){
			// to do
		}
	},

	/*
	 * catalogUsersBeforeRequest() - called before server request
	 */	
	catalogUsersBeforeRequest : function() {	
		try{
		//alert('catalogUsersBeforeRequest start');
		if ($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId);
			$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId = null;
		}
		//alert('catalogUsersBeforeRequest end');
		}catch(e){
			// to do
		}
	},

	/*
	 * catalogUsersLoadComplete() - called after server request
	 */	
	catalogUsersLoadComplete : function(data) {
	  try{	
	   var maxLimit = $('#exp_sp_user_catalog_block_max_list_count').val();
	   
	   if(data.total > 1 ) {
			$('#expertus-online-datatable-pager').css('display','block');
	   }
	   else{
		   $('#expertus-online-datatable-pager').css('display','none');
	   }
		
	   var dataIDs = $('#catalog-users-jqgrid').jqGrid('getDataIDs');
	   if (dataIDs.length <= 0) {
		   $('#expertus-no-online-users-msg').show();
		   $("#catalog-users-list").hide();
	   }
	   else {
		   $('#expertus-no-online-users-msg').hide();
		   $("#catalog-users-list").show();
	   }
	   
	   //$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId =
		   		//setInterval($("#catalog-users" ).data('lnrcatalogusersonline').reloadUsersGrid, 1000 * $('#exp_sp_user_catalog_block_refresh_interval').val());
	   //getUserProfile($("#catalog-users" ).data('lnrcatalogusersonline'), 1000 * $('#exp_sp_user_catalog_block_refresh_interval').val()); // Defined in exp_sp_formatter_user.js which gets included in exp_sp_formatter_module in its hook_init()
  		
  	   //Vtip-Display toolt tip in mouse over
	   vtip();
	   $("#catalog-users" ).data('lnrcatalogusersonline').destroyLoader('catalog-users-loader');
	   $(".user-profile-link .user-name .limit-title").trunk8(trunk8.usercatalog_title);
 	   $(".user-profile-link .job-title .limit-title").trunk8(trunk8.usrcatjobrole_title); 
	  }catch(e){
			// to do
		}
	},

	/*
	 * catalogUsersLoadError() - On error, we stop the auto refresh
	 */	
	catalogUsersLoadError : function(xhr, status, error) {
		try{
		$("#catalog-users" ).data('lnrcatalogusersonline').destroyLoader('catalog-users-loader');
		if ($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId);
			$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId = null;
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
		if ($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId);
			$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId = null;
		}
		
		$("#catalog-users-jqgrid").setGridParam({page: 1}).trigger('reloadGrid');
		}catch(e){
			// to do
		}
	},

	/*
	 * reloadUsersGrid() - When the user clicks on next/prev page, we want to show the loader. We also
	 *                   - halt the auto refresh, which will again be started on loadComplete.
	 */	
	catalogUsersPaging : function(pgButton) {
		try{
		//alert('	catalogUsersPaging start');
		$("#catalog-users" ).data('lnrcatalogusersonline').createLoader('catalog-users-loader');
		if ($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId != null) {
			//alert('catalogUsersPaging : clearInterval');
			clearInterval($("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId);
			$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId = null;
		}
		//alert('	catalogUsersPaging end');
		}catch(e){
			// to do
		}
	},
	
	
	callSearchProcess: function(){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if($('#li-users-autocomplete').css('display') == "none"){
			$('#li-users-autocomplete').css('display','block');	
			if(this.currTheme ==  "expertusoneV2")
			{
			 $('.overallusersearchlist').find('.eol-search-go').removeClass('user-search-rightcorner');	
			 $('.overallusersearchlist').find('.eol-search-go').addClass('user-search-rightcorner');
			}
			else
			{
				$('.overallusersearchlist').find('.eol-search-go').removeClass('user-search-rightcorner');
			}
		}else{
			var uname = $('#search_users_txt').val();
			if(uname == Drupal.t('LBL036')+' '+Drupal.t('LBL107')){ // ie 8 and ie 9 Encode utf-8 issue #0028781
				uname = null;
			}
			var obj = this;
			// = obj.constructUrl("learning/fetch-catalog-users-list/" + uname);
			this.createLoader('catalog-users-loader');
			$('#catalog-users-jqgrid').setGridParam({url: obj.constructUrl('learning/fetch-catalog-users-list/'+ uname)});
			$("#catalog-users-jqgrid").trigger("reloadGrid",[{page:1}]);	
			if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
				$('#li-users-autocomplete').css('display','none');
				$('#search_users_txt').val(Drupal.t('LBL036')+' '+Drupal.t('LBL107'));
				$('.overallusersearchlist').find('.eol-search-go').removeClass('user-search-rightcorner');
			}
		}
		
		 $(".user-profile-link .user-name .limit-title").trunk8(trunk8.usercatalog_title);
	 	 $(".user-profile-link .job-title .limit-title").trunk8(trunk8.usrcatjobrole_title); 
		}catch(e){
			// to do
		}
	}
});

$.extend($.ui.lnrcatalogusersonline.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
	}catch(e){
		// to do
	}
})(jQuery);

$(function() {
	try{
	$("#catalog-users").lnrcatalogusersonline();	
	}catch(e){
		// to do
	}
});
