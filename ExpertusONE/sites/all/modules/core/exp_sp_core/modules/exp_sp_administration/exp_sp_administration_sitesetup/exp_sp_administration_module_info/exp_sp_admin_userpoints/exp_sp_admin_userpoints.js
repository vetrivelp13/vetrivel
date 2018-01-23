(function($) {
	//alert($('#search_users').val());
	try{
	/*
	 * function for load users autocomplete
	 */
	if($('#search_leaderboard-users_txt')) {
		$('#search_leaderboard-users_txt').autocomplete(
				"/?q=administration/sitesetup/moduleinfo/userpoints/user-autocomplete",{
				extraParams : {'exclude_logged_user' : 1,'search_by_username' : 1},
				minChars :3,
				max :50, 
				autoFill :true,
				mustMatch :false,
				matchContains :false
		});
	}
	
	$("#search_leaderboard-users_txt").keyup(function(event){
		try {
	    if(event.keyCode == 13){
	        $("#leaderboard_users").click();
	        $(".ac_results").hide();
	    }
		}catch(e){
			// to do
		}
	});
	
    
$.widget("ui.pointsleaderboard", {
	
	setIntervalId : null, // The id returned by setInterval is saved here.
	
	/*
	 * _init() - The init function of this widget
	 */
	_init: function() {
		try{
		this.renderLeaderBoardUsersList();
		this.logUserId = this.getLearnerId();
		}catch(e){
			// to do
		}
	},

	/*
	 * paintLeaderBoardUsers() - called by jqgrid to render each cell
	 */
	paintLeaderBoardUsers: function(cellvalue, options, rowObject) {
	  try {	
		var drupalUserId = rowObject['id'];
		var personid = rowObject['personid'];
 		var userName = rowObject['screen_name'];
		var userImagePath = rowObject['image_url'];
		var totalPoints = rowObject['totalpoints'];
		if(totalPoints == null) {
			totalPoints = 0;
		}
		var logedUserId = $("#userpoints-leaderboard" ).data('pointsleaderboard').logUserId;
		//var badgeCode = rowObject['badge_code'];
		var css = rowObject['css'];
		var data1="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+personid+"','popupDispId':'profile-qtipid-"+drupalUserId+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_lb_"+drupalUserId+"','dataloaderid':'userpoints-leaderboard','dataIntId':'pointsleaderboard','wBubble':'350','hBubble':'auto'}";
		//onclick="$(\'body\').data(\'learningcore\').getLeanerQtipDiv('+data1+')"
		var html = '';
		if(!document.getElementById("qtipprofileqtip_visible_disp_lb_"+drupalUserId+"_disp")){
			html += '<div id="qtipprofileqtip_visible_disp_lb_'+drupalUserId+'_disp"></div>';
		}
 		html += '<div class="profile-item clearfix ' + css + '" id="leaderboard-qtipid-'+drupalUserId+'">'; // clearfix class is needed to be able to paint a border at the bottom of div
 		//onClick="$(\'body\').data(\'mulitselectdatagrid\').rowEditOptionDisplay(\''+rowEditOptionId+'\', \''+uniqueId+'\');"
 		html += '<div class="leader-board-seperator"><span class="lb-block-seperator">';
 	    html +=   '<a class="profile-image user-list-border-img" >';
 	    html +=     '<img class="vtip" title="' + htmlEntities(userName) + '" src="' + userImagePath + '"/>';
 	    html +=   '</a>';
 	    html +=   '<div class="user-desc">';
 	    html +=     '<span class="user-profile-link">';
 	    if(logedUserId != "" && logedUserId > 0){
 	    	html +=  '<a id="' + drupalUserId + '" class="user-name user-profile" href="javascript:void(0);" onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')">';
 	    }
 	    html +=         '<div class="limit-title-row"><span class="limit-title leaderboard-limit-title lb-user-name vtip" title="'+ userName +'">'+userName+'</span></div>';
 	    if(logedUserId != "" && logedUserId > 0){
 	    	html +=  '</a>';
 	    }
 	    html +=       '<span class="job-title">' + Drupal.t('LBL1064')+': <span class="points-text">'+totalPoints + '</span></span>';
 	    html +=     '</span>';
 	    html +=   '</div>';
 	    //html +=   '<div class="current-badge badge-icon-'+badgeCode.split('_').pop()+'">';
 	    //html +=   '</div>';
 	    html += '</span></div>';
 	    html += '</div>';

 	    return html;
 	   $('.leaderboard-limit-title').trunk8(trunk8.spotleader_title);
	  	}catch(e){
			// to do
		}
	},

	/*
	 * renderCatalogUsersList() - called when this js is loaded to create the jqgrid and fetch and display initial data.
	 */
	renderLeaderBoardUsersList: function() {
	 try{	
		// Use our loader object to signal start of jqGrid AJAX operation
		this.createLoader('leader-board-loader',false);
		// Save reference to this object, as 'this' keyword cannot be used inside jqGrid definition below
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
	  	if(this.currTheme == "expertusoneV2"){
			var cluserWidth = 295;	
		}else{
			if(navigator.userAgent.indexOf("Chrome")>=0){
				var cluserWidth = 292;				
			}
			else {
				var cluserWidth = 287;
			}
				
				
			
	    } 
		// Construct the jqGrid
		$("#leader-board-users-jqgrid").jqGrid({
			url: obj.constructUrl("administration/sitesetup/moduleinfo/userpoints/leaderboard-user/null"),
			datatype: "json",
			mtype: 'GET',
			
			colNames:[''],
			colModel:[{
				name: 'id',
				index: 'id',
				width: cluserWidth,
				title: false,
				sortable: false,
				formatter: obj.paintLeaderBoardUsers
				}],
				
			rowNum: 10,//$('#exp_sp_user_catalog_block_max_list_count').val(),

			viewrecords: true, 
			loadui: false,

			height: 'auto',
			
			//pager: '#expertus-online-datatable-pager',
			//pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			toppager: false,
			
			emptyrecords: "",
			loadtext: "",
			recordtext: "",
			
			//beforeRequest: obj.catalogUsersBeforeRequest,
			//gridComplete: obj.catalogUsersGridComplete,
			loadComplete: obj.catalogUsersLoadComplete
			//onPaging: obj.catalogUsersPaging
			//loadError: obj.catalogUsersLoadError
		});
		
		$('.ui-jqgrid').css('margin','0px');
		$('.leaderboard-limit-title').trunk8(trunk8.spotleader_title);
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
		if ($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId);
			$("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId = null;
		}
		//alert('catalogUsersBeforeRequest end');
		$('.leaderboard-limit-title').trunk8(trunk8.spotleader_title);
		}catch(e){
			// to do
		}
	},

	/*
	 * catalogUsersLoadComplete() - called after server request
	 */	
	catalogUsersLoadComplete : function(data) {
	 try {	
	   //var maxLimit = $('#exp_sp_userpoints_block_max_list_count').val();
		
	   var dataIDs = $('#leader-board-users-jqgrid').jqGrid('getDataIDs');
	   if (dataIDs.length <= 0) {
		   $('#expertus-no-user-with-points').show();
		   $("#usr-leader-board-list").hide();
	   }
	   else {
		   $('#expertus-no-user-with-points').hide();
		   $("#usr-leader-board-list").show();
	   }
	   
	   $("#leader-board-users-jqgrid").jqGrid('setGridWidth', $("#usr-leader-board-list").width());
	   $("#leader-board-users-jqgrid").trigger("reloadGrid");
	   //$("#catalog-users" ).data('lnrcatalogusersonline').setIntervalId =
		   		//setInterval($("#catalog-users" ).data('lnrcatalogusersonline').reloadUsersGrid, 1000 * $('#exp_sp_user_catalog_block_refresh_interval').val());
	   //getUserProfile($("#catalog-users" ).data('lnrcatalogusersonline'), 1000 * $('#exp_sp_user_catalog_block_refresh_interval').val()); // Defined in exp_sp_formatter_user.js which gets included in exp_sp_formatter_module in its hook_init()
  		
  	   //Vtip-Display toolt tip in mouse over
	   vtip();
	   $("#userpoints-leaderboard" ).data('pointsleaderboard').destroyLoader('leader-board-loader');
	   $('.leaderboard-limit-title').trunk8(trunk8.spotleader_title);
	 	}catch(e){
			// to do
		}
	},

	/*
	 * catalogUsersLoadError() - On error, we stop the auto refresh
	 */	
	catalogUsersLoadError : function(xhr, status, error) {
		try{
		$("#userpoints-leaderboard" ).data('pointsleaderboard').destroyLoader('leader-board-loader');
		if ($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId);
			$("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId = null;
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
		try {
		//alert('onlineReloadGrid start');
		if ($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId != null) {
			//alert('catalogUsersBeforeRequest : clearInterval');
			clearInterval($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId);
			$("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId = null;
		}
		
		$("#leader-board-users-jqgrid").setGridParam({page: 1}).trigger('reloadGrid');
		}catch(e){
			// to do
		}
	},

	/*
	 * reloadUsersGrid() - When the user clicks on next/prev page, we want to show the loader. We also
	 *                   - halt the auto refresh, which will again be started on loadComplete.
	 */	
	catalogUsersPaging : function(pgButton) {
		try {
		//alert('	catalogUsersPaging start');
		$("#userpoints-leaderboard" ).data('pointsleaderboard').createLoader('leader-board-loader');
		if ($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId != null) {
			//alert('catalogUsersPaging : clearInterval');
			clearInterval($("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId);
			$("#userpoints-leaderboard" ).data('pointsleaderboard').setIntervalId = null;
		}
		//alert('	catalogUsersPaging end');
		}catch(e){
			// to do
		}
	},
	
	
	callSearchProcess: function(){
		try{
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if($('#lb-users-autocomplete').css('display') == "none"){
			$('#lb-users-autocomplete').css('display','block');	
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
			/*0041355: In Leaderboard search bar if its contains "/" means it keeps loading */
			var uname = encodeURIComponent($('#search_leaderboard-users_txt').val());

			var obj = this;
			// = obj.constructUrl("learning/fetch-catalog-users-list/" + uname);
			this.createLoader('leader-board-loader');
			$('#leader-board-users-jqgrid').setGridParam({url: obj.constructUrl('administration/sitesetup/moduleinfo/userpoints/leaderboard-user/'+ encodeURIComponent(uname))});
			$("#leader-board-users-jqgrid").trigger("reloadGrid",[{page:1}]);	
			if(this.currTheme == "expertusoneV2"){
				$('#lb-users-autocomplete').css('display','none');
				$('#search_leaderboard-users_txt').val(Drupal.t('LBL036')+' '+Drupal.t('LBL107'));
				$('.overallusersearchlist').find('.eol-search-go').removeClass('user-search-rightcorner');
			}
			
				
		}
		$('.leaderboard-limit-title').trunk8(trunk8.spotleader_title);
		}catch(e){
			// to do
		}
	}

	
});

$.extend($.ui.pointsleaderboard.prototype, EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
	}catch(e){
		// to do
	}
})(jQuery);

function addslashes(str) {
	  //  discuss at: http://phpjs.org/functions/addslashes/
	  // original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
	  // improved by: Ates Goral (http://magnetiq.com)
	  // improved by: marrtins
	  // improved by: Nate
	  // improved by: Onno Marsman
	  // improved by: Brett Zamir (http://brett-zamir.me)
	  // improved by: Oskar Larsson Högfeldt (http://oskar-lh.name/)
	  //    input by: Denny Wardhana
	  //   example 1: addslashes("kevin's birthday");
	  //   returns 1: "kevin\\'s birthday"

	  return (str + '')
	    .replace(/[\\"']/g, '\\$&')
	    .replace(/\u0000/g, '\\0');
	}


$(function() {
	try {
	$("#userpoints-leaderboard").pointsleaderboard();	
	$(".user-popout-container .profile-name-inline .limit-title").trunk8(trunk8.profile_title);
	$(".user-popout-container #userprofile-part .limit-desc").trunk8(trunk8.profile_desc);
	}catch(e){
		// to do
	}
});

