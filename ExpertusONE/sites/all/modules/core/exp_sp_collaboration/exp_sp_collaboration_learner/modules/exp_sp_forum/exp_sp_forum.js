(function($) {

$.widget("ui.forumlistdisplay", {
	_init: function() {
	  try{	
		var self = this;		
	
		var curUrl	 = window.location.search.substring(1);
		var splitUrl = curUrl.split('/');
		
		if(splitUrl[1] == "forum-list") {
			this.renderForumResults();
	    }else if(splitUrl[1] == "forum-topic-list") {
			this.renderForumTopicResults(splitUrl[2]);			
			this.id = splitUrl[2];
		}	
	  }catch(e){
		  // to do
	  }
	},
	
	
	searchAction : function(sortbytxt,className,type) {
		try{
		var searchStr = '';		
		
		/*-------Title-------*/
		var title 	  = Drupal.t($('#forum_searchtext').val());
		var getThemeName = Drupal.settings.ajaxPageState.theme
			if(getThemeName == 'expertusoneV2'){
					if(title.toLowerCase() == Drupal.t('LBL304').toLowerCase()) {	title=''; }
				}else{
					if(title.toLowerCase() == Drupal.t('LBL304').toLowerCase()) {	title=''; }
				}
		
					
		/*-------Sort By-------*/
		if (sortbytxt!=null && sortbytxt!=undefined) this.sortbyValue = sortbytxt; 
		var sortby = this.sortbyValue;
		searchStr	= '&title='+encodeURIComponent(title)+'&sortby='+sortby;
		
		
		if(type=="forum") {
	    	this.createLoader('forum-list-display');
			$('#forumListContentResults').setGridParam({url: this.constructUrl('learning/forum-list/search/all/'+searchStr)});
		    $("#forumListContentResults").trigger("reloadGrid",[{page:1}]);
		}else{
			this.createLoader('forum-topic-list-display');
			$('#forumTopicListContentResults').setGridParam({url: this.constructUrl('learning/forum-topic-list/search/'+this.id+'/'+searchStr)});
		    $("#forumTopicListContentResults").trigger("reloadGrid",[{page:1}]);
		}
	    
	    if(className!= null) {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			$('.'+className).addClass('sortype-high-lighter');
	    }
	    else {
			$('.sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
			});
			
			if(this.sortbyValue!=''){
				if(this.sortbyValue == 'ZA')
					$('.type2').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'created')
					$('.type3').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'posttime')
					$('.type4').addClass('sortype-high-lighter');
				else if(this.sortbyValue == 'registration')
					$('.type5').addClass('sortype-high-lighter');
				else
					$('.type1').addClass('sortype-high-lighter');
			} else {
				$('.type1').addClass('sortype-high-lighter');
			}
	    }
		}catch(e){
			  // to do
		  }
	},		

  hidePageControls : function(hideAll) {
   try{
    var lastDataRow = $('#forumListContentResults tr.ui-widget-content').filter(":last");
    
    if (hideAll) {
    	if(this.currTheme == "expertusoneV2")
      	  $('.block-footer-left').show();
          $('#pager').hide();
      
      //remove bottom dotted border from the last row in result if records are present
      if (lastDataRow.length != 0) {
        //console.log('hidePageControls() : hideAll - last data row found');
        lastDataRow.children('td').css('border', '0 none');
      }
    }
    else {
      //console.log('hidePageControls() : hide only next/prev page control');
      $('#pager').show();
      if(this.currTheme == "expertusoneV2")
    	$('.block-footer-left').hide();
      $('#pager #pager_center #first_pager').hide();
      $('#pager #pager_center #last_pager').hide();
      $('#pager #pager_center #next_pager').hide();
      $('#pager #pager_center #prev_pager').hide();              
      $('#pager #pager_center #sp_1_pager').parent().hide();
    }
   }catch(e){
		  // to do
	  }
  },
  
  showPageControls : function() {
   try{	  
    //console.log('showPageControls() : show all control');
    $('#pager').show();
    if(this.currTheme == "expertusoneV2")
    $('.block-footer-left').hide();
    $('#pager #pager_center #first_pager').show();
    $('#pager #pager_center #last_pager').show(); 
    $('#pager #pager_center #next_pager').show();
    $('#pager #pager_center #prev_pager').show();              
    $('#pager #pager_center #sp_1_pager').parent().show();
   }catch(e){
		  // to do
	  }
  },

	renderForumResults : function(){	 
	 try{	
		this.createLoader('forum-list-display');
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		if(this.currTheme == "expertusoneV2"){
			var gridWidth 		= 942;		
		}else{
			var gridWidth 		= 960;
		}	
		var objStr = '$("#forum-list-display").data("forumlistdisplay")';
		$("#forumListContentResults").jqGrid({
			url:obj.constructUrl("learning/forum-list/search/all"),
			datatype: "json",
			mtype: 'GET',
			colNames:['ForumList'],
			colModel:[ {name:'ForumList',index:'ForumList', title:false, width:595,'widgetObj':objStr,formatter:obj.paintFrmDetailsResults}],			           
			rowNum:10,
			rowList:[10,20,30],
			pager: '#pager',
			viewrecords:  true,
			emptyrecords: "",
			sortorder: "desc",
			toppager:true,
			height: 'auto',
			width: gridWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			hoverrows:false,
			loadComplete:obj.frmCallbackLoader
		}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});


		if(this.sortbyValue != '' && this.sortbyValue != undefined){
			if(this.sortbyValue == 'ZA')
				$('.type2').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'created')
				$('.type3').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'posttime')
				$('.type4').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'registration')
				$('.type5').addClass('sortype-high-lighter');
			else
				$('.type1').addClass('sortype-high-lighter');
		} else {
			$('.type3').addClass('sortype-high-lighter');
		}
	 }catch(e){
		  // to do
	  }
  },
	
	frmCallbackLoader : function(response, postdata, formid)
	{
	  try{
		$('#forumListContentResults').show();
		var recs = parseInt($("#forumListContentResults").getGridParam("records"),10);
	  //console.log('callbackLoader() : recs = ' + recs);
        if (recs == 0) {
        	 $('#no-records').css('display','block');
        	 $('.left-section #forum-topic-list-display').css('display','none');
            var html = Drupal.t('MSG530');
            $("#no-records").html(html);            
        } else {
        	$('#no-records').css('display','none');
        	$("#no-records").html("");
        }

        Drupal.attachBehaviors();

        var obj = $("#forum-list-display").data("forumlistdisplay");

        // Show pagination only when search results span multiple pages
        var reccount = parseInt($("#forumListContentResults").getGridParam("reccount"), 10);
        var hideAllPageControls = true;
        if (recs > 10) { // 10 is the least view per page option.
          hideAllPageControls = false;
          //console.log('callbackLoader() : hideAllPageControls set to false');
        }
        
        //console.log('callbackLoader() : reccount = ' + reccount);
        if (recs <= reccount) {
          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
          obj.hidePageControls(hideAllPageControls);
        }
        else {
          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
          obj.showPageControls();
        }       
        
        $("#forum-list-display").data("forumlistdisplay").destroyLoader('forum-list-display');
        $("#forum-list-display").data("forumlistdisplay").destroyLoader('forumListResultsPaint');


	   $('.jqgrow').bind('mouseover',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			if($(ptr).attr("class") !== "subgrid") {
				$(ptr).addClass("ui-state-hover");
			}
			//return false;
		}).bind('mouseout',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			$(ptr).removeClass("ui-state-hover");
			//return false;
		});   

		//Vtip-Display toolt tip in mouse over
		  vtip();
		  $("#gview_forumListContentResults .ui-jqgrid-bdiv").css('width','954px');
		$(".ui-jqgrid-bdiv > div").css('position','');
		 resetFadeOutByClass('#forumListContentResults','content-detail-code','line-item-container','forum');
		 
		$('.limit-title').trunk8(trunk8.forum_title);
		$('.limit-desc').trunk8(trunk8.forum_desc);
	  }catch(e){
		  // to do
	  }
	},	



	paintFrmDetailsResults : function(cellvalue, options, rowObject) {	
	 try{	
		return rowObject['forumlist'];
	 }catch(e){
		  // to do
	  }
	},
	
	
	
	
	renderForumTopicResults : function(param){	 
	 try{	
		var call_frm = '';
		if(isObject(param)){
			call_frm = param.src;
			param = param.id;
		}
		this.createLoader('forum-topic-list-display');
		var obj = this;
		this.currTheme = Drupal.settings.ajaxPageState.theme;
		var gridWidth		= 0;
		var detailWidth 	= 0;
		var actionWidth 	= 0;
		if(this.currTheme == "expertusoneV2"){
			
			gridWidth 		= 920;
			detailWidth 	= 780;
			obj.options.styleWidth=900;
			if(call_frm!=''){
				gridWidth 		= 610;
				detailWidth 	= 440;
				obj.options.styleWidth=610;
			}
			actionWidth 	= 130;
			
		}else{
			gridWidth 		= 960;
			detailWidth 	= 595;
			actionWidth  	= 80;
		}
		var objStr = '$("#forum-topic-list-display").data("forumlistdisplay")';
		$("#forumTopicListContentResults").jqGrid({
			url:obj.constructUrl("learning/forum-topic-list/search/"+param),
			datatype: "json",
			mtype: 'GET',
			colNames:['ForumTopicList','ForumThemeTopic'],
			colModel:[ {name:'ForumTopicList',index:'ForumList', title:false, width:detailWidth,'widgetObj':objStr,formatter:obj.paintFrmTopicDetailsResults},
			           {name:'ForumThemeTopic',index:'ForumTheme', title:false, width:actionWidth,'widgetObj':objStr,formatter:obj.paintFrmTopicThemeResults,hidden:((obj.currTheme == "expertusoneV2") ? false : true)},
			           ],
		    rowNum:10,
			rowList:[10,20,30],
			pager: '#pager',
			viewrecords:  true,
			emptyrecords: "",
			sortorder: "desc",
			toppager:true,
			height: 'auto',
			width: gridWidth,
			loadtext: "",
			recordtext: "",
			pgtext : "{0}"+" "+Drupal.t('LBL981')+" "+"{1}",
			loadui:false,
			hoverrows:false,
			loadComplete:obj.frmTopicCallbackLoader
		}).navGrid('#pager',{add:false,edit:false,del:false,search:false,refreshtitle:true});
		
		
		if(this.sortbyValue != '' && this.sortbyValue != undefined){
			if(this.sortbyValue == 'ZA')
				$('.type2').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'created')
				$('.type3').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'posttime')
				$('.type4').addClass('sortype-high-lighter');
			else if(this.sortbyValue == 'registration')
				$('.type5').addClass('sortype-high-lighter');
			else
				$('.type1').addClass('sortype-high-lighter');
		} else {
			$('.type3').addClass('sortype-high-lighter');
		}
	 }catch(e){
		  // to do
	  }
	},
	
	frmTopicCallbackLoader : function(response, postdata, formid)
	{
	  try{	
		$('#forumTopicListContentResults').show();
		var recs = parseInt($("#forumTopicListContentResults").getGridParam("records"),10);

		if (recs == 0) {
			 $('#frm-topic-list-sort-display').css('display','none');
			 $('.left-section #forum-topic-list-display').css('display','none');
        	 $('#no-records').css('display','block');
            var html = Drupal.t('MSG531');
            $("#no-records").html(html);            
        } else {
        	$('#no-records').css('display','none');
        	$("#no-records").html("");
        }

        Drupal.attachBehaviors();

        var obj = $("#forum-topic-list-display").data("forumlistdisplay");

        // Show pagination only when search results span multiple pages
        var reccount = parseInt($("#forumTopicListContentResults").getGridParam("reccount"), 10);
        var hideAllPageControls = true;
        if (recs > 10) { // 10 is the least view per page option.
          hideAllPageControls = false;
          //console.log('callbackLoader() : hideAllPageControls set to false');
        }
        
        //console.log('callbackLoader() : reccount = ' + reccount);
        if (recs <= reccount) {
          //console.log('callbackLoader() : recs <= reccount : hide pagination controls');
          obj.hidePageControls(hideAllPageControls);
        }
        else {
          //console.log('callbackLoader() : recs <= reccount : show pagination controls');
          obj.showPageControls();
        }       
        
        $("#forum-topic-list-display").data("forumlistdisplay").destroyLoader('forum-topic-list-display');
        $("#forum-topic-list-display").data("forumlistdisplay").destroyLoader('forumTopicListResultsPaint');
        
        if($("#forum-topic-list-display").data("forumlistdisplay").defaults.start) {
        	$("#forum-topic-list-display").data("forumlistdisplay").defaults.start = false;
        }else{
        	$("#topic-accodion-"+obj.nId).click();
        }


	   $('.jqgrow').bind('mouseover',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			if($(ptr).attr("class") !== "subgrid") {
				$(ptr).addClass("ui-state-hover");
			}
			//return false;
		}).bind('mouseout',function(e) {
			ptr = $(e.target).closest("tr.jqgrow");
			$(ptr).removeClass("ui-state-hover");
			//return false;
		});   

		//Vtip-Display toolt tip in mouse over
		  vtip();
		$(".ui-jqgrid-bdiv > div").css('position','');
		$(".ui-jqgrid-bdiv").css('overflow','hidden');
		/*if(Drupal.settings.ajaxPageState.theme == "expertusoneV2"){
		$(".ui-jqgrid-bdiv").css('margin-left','0');
		}else
		{
			$(".ui-jqgrid-bdiv").css('margin-left','0');
		}*/
		//$('#forumTopicListContentResults > tbody > tr > td:first').css('width','800px');
		//$('#course-details-display-content #forumTopicListContentResults > tbody > tr > td:first').css('width','770px');
		$('#forumTopicListContentResults > tbody > tr:visible:last > td').css('border-bottom','0px');
		$('#forumTopicListContentResults > tbody > tr:visible:last > td').css('padding-bottom','0px');
		$('.add-maintopic-list .limit-title').trunk8(trunk8.forumtopic_title);
		$('.learner-forum-topic-description .limit-desc').trunk8(trunk8.forumtopic_desc);
		$('#block-exp-sp-learning-plan-detail-learning-details .add-maintopic-list .limit-title, #block-exp-sp-coursedetail-course-details .add-maintopic-list .limit-title, #block-exp-sp-classdetail-class-details .add-maintopic-list .limit-title').trunk8(trunk8.detailforumtopic_title);
		/*$('.admin-forum-comment-only').each(function(){
			$(this).closest('td').css('text-align','center');
		});*/
		if($('.admin-forum-comment-only').length){
				if($('#block-exp-sp-forum-detail-forum-topic').length){
					$('#forumTopicListContentResults > tbody > tr.jqgfirstrow > td:first-child').width('837px');
					
				}
				else{
					$('#forumTopicListContentResults > tbody > tr.jqgfirstrow > td:first-child').width('535px');
					
				}
		}
	  }catch(e){
		  // to do
	  }
	},	
	
	paintFrmTopicDetailsResults : function(cellvalue, options, rowObject) {		
	 try{	
		var html='';
		var wobj = eval(options.colModel.widgetObj);
		var obj = options.colModel.widgetObj;
		var nid = rowObject['nid'];
		var title = rowObject['title'];
		var tid = rowObject['tid'];
		var created = rowObject['created'];
		var last_comment_timestamp = rowObject['last_comment_timestamp'];
		var comment_count = rowObject['comment_count'];
		var description = rowObject['description'];
		var descriptionFull = rowObject['descriptionFull'];
		var user = rowObject['user'];
		var userid = rowObject['userid'];
		var createdUserId = rowObject['sltUserId'];
		var lastcmtuid = rowObject['lastcmtuid'];
		var forumaccess = rowObject['forumaccess'];
		
		var loggedUserId = wobj.getLearnerId();
		
		var popupentityId = nid;
		var popupEditTopicentityType    = 'forum-topic';
		var popupEditTopicIdInit        	 = popupentityId + '_' + popupEditTopicentityType;
		var popupEditTopicvisibPopupId  = 'qtip_visible_disp_addtopic_' + popupEditTopicIdInit;	
		var anchorId = 'frm-'+popupEditTopicvisibPopupId;
		var popupEditTopic = "data={'entityId':'"+popupentityId+"','entityType':'"+popupEditTopicentityType+"','url':'ajax/forum-add-topic/frm-tp-list/edit/"+tid+"/"+popupentityId+"','popupDispId':'"+popupEditTopicvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupEditTopicIdInit+"','wBubble':475,'hBubble':'auto','tipPosition':'tipfaceTopMiddle','forumPopupType':'Edit'}";
		
		
		var popupAddCommentsentityType    = 'forum-comments';
		var popupAddCommentsIdInit        	 = popupentityId + '_' + popupAddCommentsentityType;
		var popupAddCommentsvisibPopupId  = 'qtip_visible_disp_addcomments_' + popupAddCommentsIdInit;
		var commentsAnchorId = 'comments-'+popupAddCommentsvisibPopupId;
		var popupAddComments = "data={'entityId':'"+popupentityId+"','entityType':'"+popupAddCommentsentityType+"','url':'ajax/forum-comments/add/"+tid+"/"+nid+"/0','popupDispId':'"+popupAddCommentsvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupAddCommentsIdInit+"','wBubble':399,'hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
		
		
		var data1="data={'nid':'"+nid+"','tid':'"+tid+"','title':'"+escape(title)+"'}";		
		
		var data2="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+createdUserId+"','popupDispId':'profile-qtipid-"+userid+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_topic_"+nid+"','dataloaderid':'forum-topic-list-display','dataIntId':'forumlistdisplay','wBubble':'350','hBubble':'auto'}";
		var displayDesc = decodeURIComponent(descriptionFull);
		html += '<div id="qtipprofileqtip_visible_disp_topic_'+nid+'_disp"></div>';
		html += '<div id="topic-detail-container-'+nid+'" class="learner-forum-desc-container">';
		//html += '<div><a id="topic-accodion-'+nid+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+obj+'.accordionToTopicDetails(this.className,"0 0","0 -61px","dt-child-row-En",this,'+obj+',true);\'>&nbsp;&nbsp;</a>';
		html += '<div class="add-maintopic-list limit-title-row"><span id="titlePrgEn_'+nid+'" class="item-title" ><span class="limit-title learner-forum-title vtip" title="'+htmlEntities(decodeURIComponent(title))+'" href="javascript:void(0);">';
		html += decodeURIComponent(title);
		html += '</span></span>';
		var href = "javascript:void(0);",onclick = "";
		if(loggedUserId != "" && loggedUserId > 0){
			href = "/?q=ajax/forum-comments/add/"+tid+"/"+nid+"/0";
			var titleClick = 'onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data2+')"';
		}else{
			onclick = "$(\'body\').data(\'learningcore\').callSigninWidget();";
			var titleClick = 'onclick="$(\'body\').data(\'learningcore\').callSigninWidget();"';
		}
		if(wobj.currTheme != "expertusoneV2"){
		html += '<div class="admin-forum-action-link-container">';
		
		if(loggedUserId == createdUserId || forumaccess == 1 || loggedUserId == 1) {
			html += '<div class="edit_topic_popup"><span id="qtip_visible_disp_addtopic_'+popupEditTopicIdInit+'"></span> <a id='+anchorId+' data='+popupEditTopic+' class="admin-forum-action-links use-ajax " title="'+Drupal.t('LBL063')+'" href="/?q=ajax/forum-add-topic/frm-tp-list/edit/'+tid+'/'+popupentityId+'">'+Drupal.t('LBL063')+'</a>';
			html += '<span class="admin-forum-separater">|</span>';
			/*-- #41381: Confirmation message Fix --*/
  	   		html += '<span><a href="javascript:void(0);" class="admin-forum-action-links " title="'+Drupal.t('LBL286')+'" onClick="$(\'#forum-topic-list-display\').data(\'forumlistdisplay\').displayDeleteWizardForum(\''+Drupal.t('MSG357') + title.toUpperCase()+'\','+tid+',\'\','+nid+',\'\',\'topic\')">'+Drupal.t('LBL286')+'</a></span>';
  	   	    html += '<span class="admin-forum-separater">|</span>';
		}
		if(loggedUserId != "" && loggedUserId > 0){
			//var displayDesc = descController('ADMIN FORUM COURSES',description);
			html += '<span class="admin-forum-floatLeft disp_addcomments_forum-comments" id="qtip_visible_disp_addcomments_'+popupAddCommentsIdInit+'"></span> <a id='+commentsAnchorId+' data='+popupAddComments+' class="admin-forum-action-links use-ajax " title="'+Drupal.t('LBL868')+'"  href = "'+href+'" onClick = "'+onclick+'"><div class="clearBoth">'+Drupal.t('LBL868')+'</div></a><div class="clearBoth"></div>';
		}
		html += '</div>';
		}

		html += '</div></div>';
		if(wobj.currTheme != "expertusoneV2"){
			html += '<div class="learner-forum-list-sub-txt">'+Drupal.t('LBL869')+': '+comment_count+' | '+Drupal.t('MSG532')+' <span class="admin-forum-user-title" style="cursor:pointer;" '+titleClick+'>'+user +'</span> on '+created+' </div>'; // | last comment by <span class="admin-forum-user-title">'+lastcmtuid+'</span>
			html += '<div class="learner-forum-topic-description"><div class="learner-forum-topic-content">'+displayDesc+'</div></div></div>';
		}else{
			html += '<div class="learner-forum-list-sub-txt">'+Drupal.t('LBL869')+': '+comment_count+' | '+ Drupal.t('MSG532').charAt(0).toUpperCase() + Drupal.t('MSG532').slice(1)+' <span class="admin-forum-user-title" style="cursor:pointer;" '+titleClick+'>'+'<a class="vtip" href="#" title="'+user+'">'+titleRestrictionFadeoutImage(user, 'addtopic-user-fadeout-container')+'</a>' +'</span> on '+created+' </div>'; // | last comment by <span class="admin-forum-user-title">'+lastcmtuid+'</span>
			html += '<div class="limit-desc-row"><div class="learner-forum-topic-description"><div class="learner-forum-topic-content forum-second-theme"><span class="limit-desc vtip"><span class="cls-learner-descriptions">'+displayDesc+'</span></span></div></div></div></div>';
		}
		html += '<div class="forum_seemore title_close_dummy" id="topic-accodion-'+nid+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+obj+'.accordionToTopicDetails(this.className,"0 0","0 -61px","dt-child-row-En",'+nid+','+obj+',true);\'>'+ Drupal.t('LBL713') +'</div>';
		html += '<div id="topic-accodion-dublicate-close-'+nid+'" style="display:none;"><div class="forum_close"  id="topic-accodion-close-'+nid+'" href="javascript:void(0);" data="'+data1+'" class="title_close" onclick=\''+obj+'.accordionToTopicDetails(this.className,"0 0","0 -61px","dt-child-row-En",'+nid+','+obj+',true);\'>'+ Drupal.t('LBL3042') +'</div>';
		return html;
	 }catch(e){
		  // to do
	  }
	},
	
	paintFrmTopicThemeResults : function(cellvalue, options, rowObject) {
	 try{
		var html='';
		var wobj = eval(options.colModel.widgetObj);
		var obj = options.colModel.widgetObj;
		var nid = rowObject['nid'];
		var title = rowObject['title'];
		var tid = rowObject['tid'];
		var created = rowObject['created'];
		var last_comment_timestamp = rowObject['last_comment_timestamp'];
		var comment_count = rowObject['comment_count'];
		var description = rowObject['description'];
		var descriptionFull = rowObject['descriptionFull'];
		var user = rowObject['user'];
		var userid = rowObject['userid'];
		var createdUserId = rowObject['sltUserId'];
		var lastcmtuid = rowObject['lastcmtuid'];
		var forumaccess = rowObject['forumaccess'];
		
		var loggedUserId = wobj.getLearnerId();
		
		var popupentityId = nid;
		var popupEditTopicentityType    = 'forum-topic';
		var popupEditTopicIdInit        	 = popupentityId + '_' + popupEditTopicentityType;
		var popupEditTopicvisibPopupId  = 'qtip_visible_disp_addtopic_' + popupEditTopicIdInit;	
		var anchorId = 'frm-'+popupEditTopicvisibPopupId;
		var popupEditTopic = "data={'entityId':'"+popupentityId+"','entityType':'"+popupEditTopicentityType+"','url':'ajax/forum-add-topic/frm-tp-list/edit/"+tid+"/"+popupentityId+"','popupDispId':'"+popupEditTopicvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupEditTopicIdInit+"','wBubble':475,'hBubble':'auto','tipPosition':'tipfaceTopMiddle','forumPopupType':'Edit'}";
		
		
		var popupAddCommentsentityType    = 'forum-comments';
		var popupAddCommentsIdInit        	 = popupentityId + '_' + popupAddCommentsentityType;
		var popupAddCommentsvisibPopupId  = 'qtip_visible_disp_addcomments_' + popupAddCommentsIdInit;
		var commentsAnchorId = 'comments-'+popupAddCommentsvisibPopupId;
		var popupAddComments = "data={'entityId':'"+popupentityId+"','entityType':'"+popupAddCommentsentityType+"','url':'ajax/forum-comments/add/"+tid+"/"+nid+"/0','popupDispId':'"+popupAddCommentsvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupAddCommentsIdInit+"','wBubble':399,'hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
		
		
		var data1="data={'nid':'"+nid+"','tid':'"+tid+"','title':'"+escape(title)+"'}";		
		
		var data2="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+createdUserId+"','popupDispId':'profile-qtipid-"+userid+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_topic_"+nid+"','dataloaderid':'forum-topic-list-display','dataIntId':'forumlistdisplay','wBubble':'350','hBubble':'auto'}";

		html += '<div class="admin-forum-action-link-container"><div class="discuss-image-container">';
		var href = "javascript:void(0);",onclick = "";
		if(loggedUserId != "" && loggedUserId > 0){
			href = "/?q=ajax/forum-comments/add/"+tid+"/"+nid+"/0";
			var titleClick = 'onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data2+')"';
		}else{
			onclick = "$(\'body\').data(\'learningcore\').callSigninWidget();";
			var titleClick = 'onclick="$(\'body\').data(\'learningcore\').callSigninWidget();"';
		}
		if(loggedUserId == createdUserId || forumaccess == 1 || loggedUserId == 1) {
			html += '<span class="admin-forum-floatLeft" id="qtip_visible_disp_addcomments_'+popupAddCommentsIdInit+'"></span> <a id='+commentsAnchorId+' data='+popupAddComments+' class="admin-forum-action-links use-ajax admin-forum-action-addimg learner-forum-addimg" title="'+Drupal.t('LBL868')+'"  href = "'+href+'" onClick = "'+onclick+'"></a>';
			html += '<span id="qtip_visible_disp_addtopic_'+popupEditTopicIdInit+'"></span> <a id='+anchorId+' data='+popupEditTopic+' class="admin-forum-action-links use-ajax admin-forum-action-editimg" title="'+Drupal.t('LBL063')+'" href="/?q=ajax/forum-add-topic/frm-tp-list/edit/'+tid+'/'+popupentityId+'"></a>';
			/*-- #41381: Confirmation message Fix --*/
  	   		html += '<span><a href="javascript:void(0);" class="admin-forum-action-links admin-forum-action-delimg" title="'+Drupal.t('LBL286')+'" onClick="$(\'#forum-topic-list-display\').data(\'forumlistdisplay\').displayDeleteWizardForum(\''+Drupal.t('MSG357') +title.toUpperCase()+'\','+tid+',\'\','+nid+',\'\',\'topic\')"></a></span>';
		}else if(loggedUserId != "" && loggedUserId > 0){
			html +='<div class="learner-forum-action-link-container">';
			/** change by ayyappans
				#0034269: UI issue in Discussions, Reports, and Delete Pop up
				New class has been added to the span tag
			*/
			html += '<span class="admin-forum-floatLeft admin-forum-comment-only" id="qtip_visible_disp_addcomments_'+popupAddCommentsIdInit+'"></span> <a id='+commentsAnchorId+' data='+popupAddComments+' class="admin-forum-action-links use-ajax admin-forum-action-addimg learner-forum-addimg single" title="'+Drupal.t('LBL868')+'"  href = "'+href+'" onClick = "'+onclick+'"></a>';
			html += '</div>';
		}
  	   	html += '</div></div>';   	
		return html;
	 }catch(e){
		  // to do
	  }
	},
	accordionToTopicDetails : function(openCloseClass,openpos,closepos,childClass,obj,parentObj,isRemove) {
		try{
		var className =  openCloseClass;
		this.accordionToTopicView(openCloseClass,openpos,closepos,childClass,obj,parentObj,isRemove);
		if($('.admin-forum-comment-only').length){
					$('.admin-forum-separate-container-line-table').each(function() {
						//alert($(this).width());
						newWidth=parseInt($(this).width())+20;
						//alert(newWidth);
						$(this).width(newWidth);
						
					});
			}			
		}catch(e){
			  // to do
		  }
	},
	accordionToTopicView : function(openCloseClass,openpos,closepos,childClass,obj,parentObj,isRemove) {
	 try{	
		var currTr = obj;
		var data = eval($("#topic-accodion-"+currTr).attr("data"));
		var curObj = this;
		var lesshtml = $('#topic-accodion-dublicate-close-'+currTr).html();
		if(!document.getElementById(currTr+"SubGrid")) {
			$("#"+currTr).after("<tr class='forum-sub-rows' id='"+currTr+"SubGrid'><td colspan='2'><div style='width:"+curObj.options.styleWidth+"px;' id='topic-detail-section-"+data.nid+"'></div>"+lesshtml+"</td></tr>");
			$("#"+currTr+"SubGrid").show();
			$("#topic-accodion-"+currTr).removeClass("title_close_dummy");
			$("#topic-accodion-"+currTr).addClass("title_open_dummy");
			$("#topic-accodion-"+currTr).hide();
			$("#"+currTr).children().css('border','0px');
			$('#topic-accodion-close-'+currTr).attr('id','topic-accodion-close-dummy-'+currTr);
			$('#'+currTr+' .learner-forum-topic-description').find('#arrow-more').click();
			//console.log(1);
		} else {
			var clickStyle = $("#"+currTr+"SubGrid").css("display"); 
			if((clickStyle == "none") || (clickStyle == 'undefined')) {
				$('#'+currTr+' .learner-forum-topic-description').find('#arrow-more').click();
	    	  $("#"+currTr+"SubGrid").show();
	    	  $("#topic-accodion-"+currTr).hide();
	    	  $("#topic-accodion-close-"+currTr).show();
	    	  $("#topic-accodion-"+currTr).removeClass("title_close_dummy");
			  $("#topic-accodion-"+currTr).addClass("title_open_dummy");
		      $("#"+currTr).children().css('border','0px');
		      //console.log(2);
	    	} else {
	    	  $('#'+currTr+' .learner-forum-topic-description').find('#arrow-less').click();		
	    	  $("#"+currTr+"SubGrid").hide();
	    	  $("#topic-accodion-"+currTr).show();
	    	  $("#topic-accodion-close-"+currTr).hide();
	    	  $("#topic-accodion-"+currTr).removeClass("title_open_dummy");
			  $("#topic-accodion-"+currTr).addClass("title_close_dummy");
			  
			  this.currTheme = Drupal.settings.ajaxPageState.theme;
			  if(this.currTheme == "expertusoneV2"){
			  $("#"+currTr).children().css('border-bottom','1px solid #EDEDED');
			  $("#"+currTr).children().css('border-left','0px solid #EDEDED');
			  $("#"+currTr).children().css('border-right','0px solid #EDEDED');
			  $("#"+currTr).children().css('border-top','0px solid #EDEDED');
			  //console.log(3);
			  //console.log('#'+currTr+' .learner-forum-topic-description #arrow-less')
	   		}else{
	   		  $("#"+currTr).children().css('border-bottom','1px solid #cccccc');
			  $("#"+currTr).children().css('border-left','0px solid #cccccc');
			  $("#"+currTr).children().css('border-right','0px solid #cccccc');
			  $("#"+currTr).children().css('border-top','0px solid #cccccc');
			  //console.log(4);
	   		}	
	      }	
		}
		$('#forumTopicListContentResults > tbody > tr:visible:last > td').css('border-bottom','0px');
		$('#forumTopicListContentResults > tbody > tr:visible:last > td').css('padding-bottom','0px');
		
		var classDetSec = this.getTopicCommentsDetail(data);
		//Expertus Arranging Forum topic list function written by 'VJ'
		//this.arrangeForumTopicReply(data);
		Drupal.attachBehaviors();
		
		//REMOVING BORDER FROM LAST REPLY WRITTEN BY 'VJ'
	   //$("[id^='container-']").last().find('.learner-forum-topic-description-border').css('border-bottom','0px');
	 }catch(e){
		  // to do
	  }
	},
	
	
	getTopicCommentsDetail : function(data){		
	 try{	
		var nid  = data.nid;
		var tid = data.tid;

		//if($('#topic-detail-section-'+nid).html() == ''){
		if($('#topic-accodion-'+nid).hasClass('title_open_dummy')){	
			this.createLoader('topic-detail-container-'+nid);
			var url = this.constructUrl("ajax/topic-comments-list/" + nid + "/" + tid);		
			var obj = this;
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				async: false,
				success: function(result){
					obj.paintTopicCommentsDetails(result,data);
					obj.destroyLoader('topic-detail-container-'+nid);
				}
		    });
		}
	    //}
		//Expertus tooltip
		vtip();
	 }catch(e){
		  // to do
	  }
	},
	
	
	paintTopicCommentsDetails : function(result,data){
	 try{	
		//alert(result.toSource())
		var res_length = result.length;
		var nid  = data.nid;
		var tid = data.tid;
		var title = unescape(data.title);
		var obj = this;
		var html = "";
		var c;
		var clsN;
		var picturePath;
		var loggedUserId = this.getLearnerId();
		if(res_length > 0 ) {
			html += '<div class="comments-desc-div">';
			html += '<div class="comments-level">';
			var tmpThread=0;
			var indNumbers=0;
			for(c=0; c < result.length ; c++){
			  
				var cid				= result[c]['cid'];
				var pid				= result[c]['pid'];
				var nid				= result[c]['nid'];
				var uid				= result[c]['uid'];
				var subject			= result[c]['subject'];
				var created			= result[c]['created'];
				var name			= result[c]['name'];
				var description		= result[c]['description'];
				var thread		    = result[c]['thread'];
				var personid		= result[c]['personid'];
				var picture		    = result[c]['picture'];
				var timezone		    = result[c]['timezone'];
				var forumaccess		    = result[c]['access'];
				
			    var n=created.split(" ");
			    created = n[0]+" "+n[1]+" "+n[2]+" "+n[3]+" <span class='time-zone-text'>"+n[4]+"</span>";
				
				var popupentityId = cid;
				var popupEditCommentsentityType    = 'forum-comments-edit';
				var popupEditCommentsIdInit        = "changeclass_"+popupentityId + '_' + popupEditCommentsentityType;
				var popupEditCommentsvisibPopupId  = 'qtip_visible_disp_editcomments_' + popupEditCommentsIdInit;
				var ecommentsAnchorId = 'comments-'+popupEditCommentsvisibPopupId;
				var popupEditComments = "data={'entityId':'"+popupentityId+"','entityType':'"+popupEditCommentsentityType+"','url':'ajax/forum-comments/edit/"+tid+"/"+nid+"/"+cid+"','popupDispId':'"+popupEditCommentsvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupEditCommentsIdInit+"','wBubble':399,'hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
				
				var popupReplyCommentsentityType    = 'forum-comments-reply';
				var popupReplyCommentsIdInit        = "changeclass_"+popupentityId + '_' + popupReplyCommentsentityType;
				var popupReplyCommentsvisibPopupId  = 'qtip_visible_disp_replycomments_' + popupReplyCommentsIdInit;
				var replyAnchorId = 'comments-'+popupReplyCommentsvisibPopupId;
				var popupReplyComments = "data={'entityId':'"+popupentityId+"','entityType':'"+popupReplyCommentsentityType+"','url':'ajax/forum-comments/reply/"+tid+"/"+nid+"/"+cid+"','popupDispId':'"+popupReplyCommentsvisibPopupId+"','catalogVisibleId':'qtipAttachCrsIdqtip_visible_disp_"+popupReplyCommentsIdInit+"','wBubble':399,'hBubble':'auto','tipPosition':'tipfaceTopMiddle'}";
				
				var indentCount = thread.split(".").length - 1;
				if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2') {
					var width = obj.options.styleWidth-(40*indentCount); //22
				} else {
					var width = 940-(40*indentCount); //22					
				}
				var widthStyle = "width:"+width+"px;";
				var widthPx = width+" px";
				var padding = "margin-left : 41px; margin-bottom:0px;margin-top:0px;";				
				if(this.currTheme == "expertusoneV2"){
				if(picture) {
					picturePath ="/sites/default/files/pictures/"+picture;
				}else{
					picturePath =" sites/default/files/pictures/expertusonev2_default_user.png";					
				}
				}else{
					if(picture) {
					picturePath ="/sites/default/files/pictures/"+picture;
				}else{
					picturePath ="/sites/all/themes/core/expertusone/expertusone-internals/images/forum_default_user_img.png";					
				}
				}
				var data1="{'entityId':'1','entityType':'enr_crs_usr','url':'learning/ajax/myprofile/user/"+personid+"','popupDispId':'profile-qtipid-"+uid+"','qtipDisplayPosition':'tipfaceRight','catalogVisibleId':'qtipprofileqtip_visible_disp_"+uid+'-'+c+"','dataloaderid':'forum-topic-list-display','dataIntId':'forumlistdisplay','wBubble':'350','hBubble':'auto'}";

				if(indentCount==0){
					for(j=0;j<=indNumbers;j++){
						html += '</div>';
					}
					indNumbers=0;
					tmpThread=0;
					html += "<div class='top-comment-class learner-forum-desc-container'>";
				}else{
					if(tmpThread!=indentCount){
						if(tmpThread>indentCount){
							for(j=indNumbers;j>=indentCount;j--){
								html += '</div>';
								indNumbers--;
							}
						}
						indNumbers++;
						//var color = '#'+Math.floor((Math.random()*1000000)+1); 
						html += "<div class='top-comment-class learner-forum-desc-container' style='"+padding+""+widthStyle+"'>";
						tmpThread=indentCount;
					}
				}
				
				if(this.currTheme != "expertusoneV2"){
					html += '<div id="qtipprofileqtip_visible_disp_'+uid+'-'+c+'_disp"></div>';
					html += '<div class="admin-forum-separate-container-line" id="container-'+indentCount+'" >';
						html += '<table class="admin-forum-separate-container-line-table" border="0" width="'+widthPx+'" cellpadding="0" cellspacing="0"><tr><td class="admin-forum-picture-container" valign="top"><div style="padding-top:14px;"><img class="admin-forum-user-picture" src="'+picturePath+'" height="50px" width="50px" /></div></td>';
						html += '<td><div class="topic-details-cls">';
						html += '<a id="comments-accodion-'+cid+'" style="display:none;" href="javascript:void(0);" class="title_close" >&nbsp;&nbsp;</a>';
						html += '<span id="titleComtDt_'+cid+'" class="item-title" ><span class="learner-forum-title vtip"  title="'+result[c]['firstname']+' '+result[c]['lastname']+'" href="javascript:void(0);">';
						
						if(loggedUserId != "" && loggedUserId > 0){
							var titleClick = 'onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')"';
						}else{
							var titleClick = 'onclick="$(\'body\').data(\'learningcore\').callSigninWidget();"';
						}
						html += '<span class="admin-forum-user-title-link" style="cursor:pointer;" '+titleClick+'>'+result[c]['firstname']+' '+result[c]['lastname'].charAt(0)+'</span>';//titleRestricted(subject,50);//+"--"+thread;
						html += '</span></span>';
						html += '</div>';
						html += '<div class="learner-forum-list-sub-txt">';
						html += Drupal.t('MSG533')+' '+ timezone;
						html += '</div>';
						html += '<div class="learner-forum-topic-description"><div><div class="learner-forum-topic-content">';
						html += description;
						html += '</div></div>';
						html += '<div class="admin-forum-action-link-container">';			
						if(loggedUserId == personid || forumaccess == 1 || loggedUserId == 1) {
							html += '<span id="qtip_visible_disp_editcomments_'+popupEditCommentsIdInit+'" class="qtip_forum_comments_edit"></span> <a id='+ecommentsAnchorId+' data='+popupEditComments+' class="admin-forum-action-links-first admin-forum-action-links use-ajax" title="'+Drupal.t('LBL063')+'" href="/?q=ajax/forum-comments/edit/'+tid+'/'+nid+'/'+cid+'">'+Drupal.t('LBL063')+'</a>';
							
							html += '<span class="admin-forum-separater">|</span>';
							/*-- #41381: Confirmation message Fix --*/
				  	   		html += '<span><a href="javascript:void(0);" class="admin-forum-action-links" title="'+Drupal.t('LBL286')+'" onClick="$(\'#forum-topic-list-display\').data(\'forumlistdisplay\').displayDeleteWizardForum(\''+Drupal.t('MSG357') + Drupal.t('LBL1140') +'\','+tid+',\'\','+nid+','+cid+',\'comment\')">'+Drupal.t('LBL286')+'</a></span>';
				  	   		
							//if (indentCount < 1) {
								html += '<span class="admin-forum-separater">|</span>';
							//}
						}			  	    
						//if (indentCount < 1) {
						var replyHref = "javascript:void(0);",replyOnclick = "";
						if(loggedUserId != "" && loggedUserId > 0){
							replyHref = "/?q=ajax/forum-comments/reply/"+tid+"/"+nid+"/"+cid;
						}else{
							replyOnclick = "$(\'body\').data(\'learningcore\').callSigninWidget();";
						}
						if(loggedUserId != "" && loggedUserId > 0){
							html += '<span class="admin-forum-floatLeft disp_replycomments" id="qtip_visible_disp_replycomments_'+popupReplyCommentsIdInit+'"></span> <a id='+replyAnchorId+' data='+popupReplyComments+' class="admin-forum-action-links use-ajax" title="'+Drupal.t('LBL867')+'" href="'+replyHref+'" onClick = "'+replyOnclick+'">'+Drupal.t('LBL867')+'</a><div class="clearBoth"></div>';
						}
						html += '</div></div>';					
						html += '</td></tr></table>';
					html += '</div>';
			}    else{
				    html += '<div id="qtipprofileqtip_visible_disp_'+uid+'-'+c+'_disp"></div>';
					html += '<div class="admin-forum-separate-container-line" id="container-'+indentCount+'" >';
					html += '<table class="admin-forum-separate-container-line-table" border="0" width="'+widthPx+'" cellpadding="0" cellspacing="0"><tr><td class="admin-forum-picture-container" valign="top"><div class="avatar-image user-list-border-img"><img class="admin-forum-user-picture" src="'+picturePath+'" height="50px" width="50px" /></div></td>';
					html += '<td><div class="topic-details-cls">';
					html += '<a id="comments-accodion-'+cid+'" style="display:none;" href="javascript:void(0);" class="title_close" >&nbsp;&nbsp;</a>';
					html += '<span id="titleComtDt_'+cid+'" class="item-title" ><span class="learner-forum-title vtip"  title="'+htmlEntities(result[c]['firstname']+' '+result[c]['lastname'])+'" href="javascript:void(0);">';
			
		     if(loggedUserId != "" && loggedUserId > 0){
		    	 var titleClick = 'onclick="$(\'body\').data(\'learningcore\').paintOtherUserProfile('+data1+')"';
		     }else{
				var titleClick = 'onclick="$(\'body\').data(\'learningcore\').callSigninWidget();"';
			}
		     	html += '<div class="limit-title-row"><span class="admin-forum-user-title-link limit-title" style="cursor:pointer;" '+titleClick+'>'+result[c]['firstname']+' '+result[c]['lastname'].charAt(0)+'</span></div>';//titleRestricted(subject,50);//+"--"+thread;
		     	html += '</span></span>';
		     	html += '</div>';
		     	html += '<div class="learner-forum-list-sub-txt">';
		     	var tZ = timezone.split(' ');
		     	timezone = tZ[0]+' '+tZ[1]+', '+tZ[2]+' '+tZ[3]+' '+tZ[4]+' '+tZ[5];
		     	html += Drupal.t('MSG533').charAt(0).toUpperCase() + Drupal.t('MSG533').slice(1)+' '+ timezone;
		     	html += '</div>';
		     	html += '<div class="learner-forum-topic-description"><div><div class="learner-forum-topic-content">';
		     	html += description.replace(/\n/g, "<br />");
		     	html += '</div></div></td>';
				   html += '<td valign="top" style="width:140px"><div class="topic-details-img" >';
				   html += '<div class="admin-forum-action-link-container ">' ;
				   var replyHref = "javascript:void(0);",replyOnclick = "";
					if(loggedUserId != "" && loggedUserId > 0){
						replyHref = "/?q=ajax/forum-comments/reply/"+tid+"/"+nid+"/"+cid;
					}else{
						replyOnclick = "$(\'body\').data(\'learningcore\').callSigninWidget();";
					}
			  if(loggedUserId == personid || forumaccess == 1 || loggedUserId == 1) {
				   html += '<span class="admin-forum-floatLeft" id="qtip_visible_disp_replycomments_'+popupReplyCommentsIdInit+'"></span> <a id='+replyAnchorId+' data='+popupReplyComments+' class="admin-forum-action-links use-ajax admin-forum-action-addimg learner-forum-addimg" title="'+Drupal.t('LBL867')+'" href="'+replyHref+'" onClick = "'+replyOnclick+'"></a>';
				   html += '<span id="qtip_visible_disp_editcomments_'+popupEditCommentsIdInit+'"></span> <a id='+ecommentsAnchorId+' data='+popupEditComments+' class="admin-forum-action-links-first admin-forum-action-links use-ajax admin-forum-action-editimg" title="'+Drupal.t('LBL063')+'" href="/?q=ajax/forum-comments/edit/'+tid+'/'+nid+'/'+cid+'"></a>';
				   /*-- #41381: Confirmation message Fix --*/
				   html += '<span><a href="javascript:void(0);" class="admin-forum-action-links admin-forum-action-delimg" title="'+Drupal.t('LBL286')+'" onClick="$(\'#forum-topic-list-display\').data(\'forumlistdisplay\').displayDeleteWizardForum(\''+Drupal.t('MSG357') + Drupal.t('LBL1140') +'\','+tid+',\'\','+nid+','+cid+',\'comment\')"></a></span>';
	  	   		
				//if (indentCount < 1) {
				//}
			  }	else if(loggedUserId != "" && loggedUserId > 0){
				html +='<div class="learner-forum-action-link-container">';
				html += '<span class="admin-forum-floatLeft  admin-forum-comment-only" id="qtip_visible_disp_replycomments_'+popupReplyCommentsIdInit+'"></span> <a id='+replyAnchorId+' data='+popupReplyComments+' class="admin-forum-action-links use-ajax admin-forum-action-addimg learner-forum-addimg single" title="'+Drupal.t('LBL867')+'" href="'+replyHref+'" onClick = "'+replyOnclick+'"></a>';
				html += '</div>';
			  }		  	    
			//if (indentCount < 1) {
					
					html += '</div></div>';					
					html += '</td></tr></table>';
				    html += '</div>';
			}		
				
		  }
			for(j=0;j<indNumbers;j++){
				html += '</div>';
			}
		  html += '</div>'; //comments-level
		  html += '</div>'; //comments-desc-div
		}
		else{
			html += '<div class="admin-forum-nocomments-found">'+Drupal.t('MSG534')+'</div>';
		}

		$("#topic-detail-section-"+nid).html(html);
		$(".admin-forum-separate-container-line .limit-title").trunk8(trunk8.frmreplyusr_title);
		if(loggedUserId != personid && forumaccess != 1 && loggedUserId != 1) {
			$('.topic-details-img').css('text-align','center');
		}
	 }catch(e){
		  // to do
	  }
	},
	
	arrangeForumTopicReply : function(data){
	 try{	
		var topicId = data.nid;
		var tableLength = $('#'+topicId+'SubGrid').find('.top-comment-class > tbody > tr > td').size();
		for(var i=0; i<tableLength;i++) {
			$('#'+topicId+'SubGrid').find('.top-comment-class > tbody > tr > td').eq(i).css('padding-left',i*25);
		}
	 }catch(e){
		  // to do
	  }
	},
	
	deleteComments : function(tid,nid,cid){		
	 try{	
		var url = this.constructUrl("ajax/delete-comments/"+tid+'/'+nid+'/'+cid);		
		var obj = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#delete-msg-wizard').remove();
					$("#forum-topic-list-display").data("forumlistdisplay").refreshForumPage(tid,nid,'forumtopic');
					/*var data = eval($("#topic-accodion-"+nid).attr("data"));
					$("#forum-topic-list-display").data("forumlistdisplay").getTopicCommentsDetail(data);
					Drupal.attachBehaviors();*/
				}
	    });
	 }catch(e){
		  // to do
	  }
	},
	
	
	deleteTopics : function(tid,nid){		
	 try{	
		var url = this.constructUrl("ajax/delete-topic/"+tid+'/'+nid);		
		var obj = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#delete-msg-wizard').remove();
					$("#forum-topic-list-display").data("forumlistdisplay").refreshForumPage(tid,nid,'forumtopic');
				}
	    });
	 }catch(e){
		  // to do
	  }
	},
	
	deleteForum : function(entityId,entityType){	
	 try{	
		var url = this.constructUrl("ajax/delete-forum/"+entityId+"/"+entityType);		
		var obj = this;
		$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(result){
					$('#delete-msg-wizard').remove();
					$("#forum-list-display").data("forumlistdisplay").refreshForumPage('','','forumlist');
				}
	    });
	 }catch(e){
		  // to do
	  }
	},
	/*-- #41381: Confirmation message Fix --*/
	displayDeleteWizardForum : function(title,objectId,objectType,nid,cid,forumObjType){
		try {
			title = decodeURIComponent(title);
			var uniqueClassPopup = '';
			if(Drupal.settings.ajaxPageState.theme == 'expertusoneV2'){
				if(objectType == "Class" && document.getElementById('catalog-course-basic-addedit-form')){
					uniqueClassPopup = 'unique-delete-class';
				}							
			}
			var objectType = (objectType) ? objectType: '';
			var wSize = (wSize) ? wSize : 300;
			var nodeId = (nid) ? nid : '';
			var commentId = (cid) ? cid : '';
			
		    $('#delete-msg-wizard').remove();
		    var html = '';	    
		    html+='<div id="delete-msg-wizard" style="display:none; padding: 0px;">';
		    html+='<table width="100%" cellpadding="0" cellspacing="0" border="0" valign="center">';
			
		    if(this.currTheme == 'expertusoneV2'){
		   	 html+= '<tr><td style="padding: 30px 24px;" class="commanTitleAll">'+title+''+'?'+'</td></tr>';
		    } else {
		     html+= '<tr><td color="#333333" style="padding: 30px 24px; text-align: justify;">'+title+''+'?'+'</td></tr>';
		    }
		    html+='</table>';
		    html+='</div>';
		    $("body").append(html);
		    
		 
		    var closeButt={};
		    closeButt[Drupal.t('LBL109')]=function(){ $(this).dialog('destroy');$(this).dialog('close');$('#delete-msg-wizard').remove();};
		    
		    if(availableFunctionalities.exp_sp_esignature != undefined && availableFunctionalities.exp_sp_esignature != ""){
		    	var esignObj = {'popupDiv':'delete-object-dialog','esignFor':'displayDeleteWizardForum','objectId':objectId,'objectType':objectType, 'nodeId': nodeId, 'commentId': commentId, 'forumObjType': forumObjType, };
		    	closeButt[Drupal.t('Yes')]=function(){ $.fn.getNewEsignPopup(esignObj);  };
		    }else{
		    	closeButt[Drupal.t('Yes')]=function(){ 
		    		// select action call based on the forum object type
				   switch (forumObjType) {
				    	case 'forum':
				    		$("#forum-list-display").data("forumlistdisplay").deleteForum(objectId,objectType);
				    		break;
				    	case 'topic':
				    		$("#forum-topic-list-display").data("forumlistdisplay").deleteTopics(objectId,nodeId);
					    	break;
				    	case 'comment': 
				    		$("#forum-topic-list-display").data("forumlistdisplay").deleteComments(objectId,nodeId,commentId);
				    		break;	
				    }
		    	};
		    	
		    }
		    
		    $("#delete-msg-wizard").dialog({
		        position:[(getWindowWidth()-400)/2,200],
		        bgiframe: true,
		        width:wSize,
		        resizable:false,
		        modal: true,
		        title:Drupal.t('LBL286'),//"title",
		        buttons:closeButt,
		        closeOnEscape: false,
		        draggable:false,
		        overlay:
		         {
		           opacity: 0.9,
		           background: "black"
		         }
		    });
		    $('.ui-dialog').wrap("<div id='delete-object-dialog' class='"+uniqueClassPopup+"'></div>");

		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(0)').addClass("removebutton").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').before('<div class="admin-save-button-left-bg"></div>');
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').addClass("admin-save-button-middle-bg").end();
		    $('.ui-dialog-buttonpane .ui-dialog-buttonset button:eq(1)').after('<div class="admin-save-button-right-bg"></div>');
		    
		    $("#delete-msg-wizard").show();
		
			$('.ui-dialog-titlebar-close').click(function(){
		        $("#delete-msg-wizard").remove();
		    });
			if(this.currTheme == 'expertusoneV2'){
		    	changeDialogPopUI();
		    }
			/*-- 37211: Unable to delete a class in FF version 32.0 --*/
		    if($('div.qtip-defaults').length > 0) {
		    	var prevZindex = $('.qtip-defaults').css('z-index');
		    	$('#delete-object-dialog .ui-widget-content').css('z-index', prevZindex+2);
		    	$('.ui-widget-overlay').css('z-index', prevZindex+1);
		    }
		} catch(e) {
			//to do
		}

	},
	

	paintAfterReady : function() {		
	 try{	
		if(document.getElementById('forum-list-display')) {
			$('#first_pager').click( function(e) {
				if(!$('#first_pager').hasClass('ui-state-disabled')) {
					$('#forumListContentResults').hide();
					$('#gview_forumListContentResults').css('min-height','100px');
					$("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
				}
			});
			$('#prev_pager').click( function(e) {
				if(!$('#prev_pager').hasClass('ui-state-disabled')) {
					$('#forumListContentResults').hide();
					$('#gview_forumListContentResults').css('min-height','100px');
					$("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
				}
			});
			$('#next_pager').click( function(e) {
				if(!$('#next_pager').hasClass('ui-state-disabled')) {
					$('#forumListContentResults').hide();
					$('#gview_forumListContentResults').css('min-height','100px');
					$("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
				}
			});
			$('#last_pager').click( function(e) {	
				if(!$('#last_pager').hasClass('ui-state-disabled')) {
					$('#forumListContentResults').hide();
					$('#gview_forumListContentResults').css('min-height','100px');
					$("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
				}
			});
			$('.ui-pg-selbox').bind('change',function() {
				$('#forumListContentResults').hide();
				$('#gview_forumListContentResults').css('min-height','100px');
				$("#forum-list-display").data("forumlistdisplay").hidePageControls(false);
				$("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
			});			
			$(".ui-pg-input").keyup(function(event){				
				if(event.keyCode == 13){
					$('#forumListContentResults').hide();
					$('#gview_forumListContentResults').css('min-height','100px');
				  $("#forum-list-display").data("forumlistdisplay").createLoader('forumListResultsPaint');
				}
			});
		} 
	 }catch(e){
		  // to do
	  }
	},

	refreshForumPage : function(tId,nId,page){	
	 try{	
		var pageNo = $(".ui-pg-input").val();
		this.nId = nId;

		if(page == 'forumlist') {
			this.createLoader('forumListResultsPaint');
			$('#forumListContentResults').setGridParam({url: this.constructUrl('learning/forum-list/search/all/')});
		    $("#forumListContentResults").trigger("reloadGrid",[{page:pageNo}]);
		}else{
			this.createLoader('forumTopicListResultsPaint');
			$('#forumTopicListContentResults').setGridParam({url: this.constructUrl('learning/forum-topic-list/search/'+tId)});
		    $("#forumTopicListContentResults").trigger("reloadGrid",[{page:pageNo}]);
		}	
	 }catch(e){
		  // to do
	  }
	}

});

$.extend($.ui.forumlistdisplay.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget,{defaults:{start:true}});

})(jQuery);

$(function() {
 try{
	if(document.getElementById('forum-topic-list-display')) {
		$( "#forum-topic-list-display" ).forumlistdisplay();
	}else if(document.getElementById('forum-list-display')) {
		$( "#forum-list-display" ).forumlistdisplay();
    }
	
	
	$("#forum_searchtext").keyup(function(event){
		  if(event.keyCode == 13){			  
			  if(document.getElementById('forum-topic-list-display')) {
				  $("#forum-topic-list-display").data("forumlistdisplay").searchAction('','','topic');
				}else if(document.getElementById('forum-list-display')) {
				  $("#forum-list-display").data("forumlistdisplay").searchAction('','','forum');
			  }		
			  $('.ac_results').css('display', 'none');
		   }
		});

	$('#forum-search-txt').click(function() {
		if(document.getElementById('forum-topic-list-display')) {
			$("#forum-topic-list-display").data("forumlistdisplay").searchAction('','','topic');
		}else if(document.getElementById('forum-list-display')) {
			$("#forum-list-display").data("forumlistdisplay").searchAction('','','forum');
	    }
	});

	if($('#forum_searchtext')) {
		
		var curUrl	 = window.location.search.substring(1);
		var splitUrl = curUrl.split('/');
		
		if(document.getElementById('forum-topic-list-display')) {
			$('#forum_searchtext').autocomplete(
					"/?q=learning/forum-topic-list-autocomplete/"+splitUrl[2],{
					minChars :3,
					max :50, 
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
			});
		}else{
			$('#forum_searchtext').autocomplete(
					"/?q=learning/forum-list-autocomplete",{
					minChars :3,
					max :50, 
					autoFill :true,
					mustMatch :false,
					matchContains :false,
					inputClass :"ac_input",
					loadingClass :"ac_loading"
			});
		}
	}
	
 }catch(e){
	  // to do
 }
});

(function($) {
	$.fn.refreshList = function(tId,nId) {
	 try{	
		$(".active-qtip-div").remove();
		$("#forum-topic-list-display").data("forumlistdisplay").refreshForumPage(tId,nId,'forumtopic');
	 }catch(e){
		  // to do
	  }
	};
})(jQuery);


(function($) {
	$.fn.refreshListAccordion = function(tId,nId,option) {
		try{
		$(".active-qtip-div").remove();
		if(option == 'reply' || option == 'edit') {
			var data = eval($("#topic-accodion-"+nId).attr("data"));
			$("#forum-topic-list-display").data("forumlistdisplay").getTopicCommentsDetail(data);
			Drupal.attachBehaviors();
		}else{
			$("#topic-accodion-"+nId).click();			
		}
		}catch(e){
			  // to do
		  }
	};
})(jQuery);


(function($) {
	$.fn.redirectPage = function(id) {
		try{
		self.location='?q=learning/forum-topic-list/'+id;
		}catch(e){
			  // to do
		  }
	};
})(jQuery);


(function($) {
  Drupal.ajax.prototype.commands.expQtipDisplay = function(ajax, response, status) {
	  //alert(ajax.element.id)
    try{
    	 var qtipObj = eval($("#"+ajax.element.id).attr("data"));
    		//var obj = 'this';	
    		var popupId 	     = qtipObj.popupDispId;
    		var entityType       = qtipObj.entityType;
    		var wBubble 		 = qtipObj.wBubble;//700; //Bubble Popup Width
    		var hBubble 		 = qtipObj.hBubble;//300; //Bubble Popup Height
    		var setwBubble       = wBubble-5; 
    		var setBtmLeft       = (setwBubble-104)/2;
    		var qtipLeftPos      = (wBubble > 700) ? 400 : (setBtmLeft + 20);
    		var tipPos           = qtipObj.tipPosition;
    		var catalogVisibleId = qtipObj.catalogVisibleId;
    		var mLeft            = 0;
    		var tipPosition 	 = qtipObj.tipPosition;
    		var setTip			 = $('#'+ajax.element.id).position();
    		//var setTip			 = $(obj).position();
    		var topElements,bottomElements;
    		var qtipCls = qtipObj.qtipClass;
    		$(".active-qtip-div").remove();
    		var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message'></div>"+response.html+"</div>"+bottomElements;
    		var contentDiv = qtipObj.catalogVisibleId+"_disp";
    		$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
    		if(qtipCls == 'display-message-positioning'){
    			$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div "+qtipCls+"'></div>");
    		}
    		else{
    			$("#"+popupId).append("<div id='"+contentDiv+"' class='active-qtip-div'></div>");
    		}
    		$("#"+contentDiv).html(paintHtml);
    		$(".bottom-qtip-tip").css('bottom','0px').css('position','absolute').css('z-index','101');
    		
    		var p = $("#"+popupId);
    		var position = p.position();
    		var divHeight = $("#"+contentDiv).height();
    		var parentTopPos = Math.round(position.top) + 10;
    		var parentLeftPos = Math.round(position.left);
    		this.currTheme = Drupal.settings.ajaxPageState.theme;
    		//alert("tipPosition::"+tipPosition);
			if(tipPosition == 'tipfaceMiddleRight') {
				$("#"+popupId).append("<div class='bottom-qtip-tip-right active-qtip-div set-wbubble-left'></div>");
				$(".bottom-qtip-tip-right").css({'top':+parentTopPos-30+'px','position':'absolute','right':'103px'});
				$(".bottom-qtip-tip").remove();
				$("#"+contentDiv).css({'position':'absolute','right':'151px','top':+parentTopPos-125+'px','bottom':'0px','z-index':'100'});
			} else if(tipPosition == 'tipfaceTopMiddle'){
				var windowHeight = ($('#'+popupId).closest('.admin-forum-action-link-container').offset().top) - ($(window).scrollTop()); //retrieve current window height
	            var qtipPlacement;
	            if (windowHeight > 300){
	                qtipPlacement = 'top';
	               } else {
	                qtipPlacement = 'bottom';
	             }
	            //alert('qtipPlacement::'+qtipPlacement);
				if (qtipPlacement == 'top'){
		       		$(".bottom-qtip-tip").remove();
					$("#"+popupId).append("<div class='bottom-qtip-tip active-qtip-div set-wbubble-left'></div>");
					if(qtipObj.forumPopupType == 'Edit') {
						if(this.currTheme == "expertusoneV2"){
							$(".set-wbubble-left").css({'top':+(Math.round(setTip.top)-40)+'px','left':setTip.left-25+'px','position':'absolute'});
							
							if( $.browser.msie && $.browser.version == 9){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-38)+'px','left':setTip.left-25+'px','position':'absolute'});
							}
							if( $.browser.msie && $.browser.version == 8){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-22)+'px','left':setTip.left-15+'px','position':'absolute'});
							}
							if(navigator.userAgent.indexOf("Chrome")>=0){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-34)+'px','left':setTip.left-25+'px','position':'absolute'});
							}	
						}else{
							if( $.browser.msie && $.browser.version == 8){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-45)+'px','left':setTip.left-15+'px','position':'absolute'});
							}else if($.browser.msie && $.browser.version == 10){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-23)+'px','left':setTip.left-15+'px','position':'absolute'}); //Changed the class name, since it is getting conflict with the account settings popup
							}else{
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-22)+'px','left':setTip.left-15+'px','position':'absolute'}); //Changed the class name, since it is getting conflict with the account settings popup
							}
						}
					} 
					else if($.browser.msie && $.browser.version == 7){
						$(".set-wbubble-left").css({'top':+Math.round(setTip.top-15)+'px','left':setTip.left-15+'px','position':'absolute'}); 
					}
					else if( $.browser.msie && $.browser.version == 8 ){
						if((popupId.indexOf('replycomments')>=0) || (popupId.indexOf('editcomments')>=0)){
							if(this.currTheme == "expertusoneV2"){
							  $(".set-wbubble-left").css({'top':+Math.round(setTip.top-43)+'px','left':setTip.left-15+'px','position':'absolute'});
							}else{
							  $(".set-wbubble-left").css({'top':+Math.round(setTip.top-43)+'px','left':setTip.left-15+'px','position':'absolute'});	
							}	
							}else{
							if(this.currTheme == "expertusoneV2"){
							$(".set-wbubble-left").css({'top':+Math.round(setTip.top-44)+'px','left':setTip.left-15+'px','position':'absolute'});
						  }else{
							$(".set-wbubble-left").css({'top':+Math.round(setTip.top-44)+'px','left':setTip.left-15+'px','position':'absolute'});
						}
						}		
					}else {
						
						if(this.currTheme == "expertusoneV2"){
						 $(".set-wbubble-left").css({'top':+Math.round(setTip.top-37)+'px','left':setTip.left-25+'px','position':'absolute'}); //Changed the class name, since it is getting conflict with the account settings popup
						 
						   if( $.browser.msie && $.browser.version == 9 ){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-34)+'px','left':setTip.left-25+'px','position':'absolute'});
							}
						   if( $.browser.msie && $.browser.version >= 10){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-37)+'px','left':setTip.left-25+'px','position':'absolute'});
							}
						   if(navigator.userAgent.indexOf("Chrome")>=0){
								$(".set-wbubble-left").css({'top':+Math.round(setTip.top-36)+'px','left':setTip.left-25+'px','position':'absolute'});
							}
						}else{
						 $(".set-wbubble-left").css({'top':+Math.round(setTip.top-37)+'px','left':setTip.left-15+'px','position':'absolute'}); 
						 
						 if( $.browser.msie && $.browser.version == 10 ){
							 $(".set-wbubble-left").css({'top':+Math.round(setTip.top-37)+'px','left':setTip.left-15+'px','position':'absolute'});
						 }
					  }
					}		
					if(this.currTheme == "expertusoneV2"){
						//alert("after Here come the popup"+Math.round(setTip.left));
						if(Math.round(setTip.left) > 500){
							//alert("HHH"+navigator.userAgent.indexOf("Chrome"));
							if(navigator.userAgent.indexOf("Chrome")>=0) {
								$("#"+contentDiv).css({'position':'absolute','left':setTip.left-441+'px','top':Math.round(setTip.top-263)+'px','z-index':'100'});
								if(qtipObj.forumPopupType == 'Edit') {
									$("#"+contentDiv).css({'position':'absolute','left':setTip.left-441+'px','top':Math.round(setTip.top-298)+'px','z-index':'100'});
								}
		    				}else if($.browser.msie && $.browser.version == 7){
		    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':Math.round(setTip.top-320)+'px','z-index':'100'});
							}
		    				else if($.browser.msie && $.browser.version == 8){
		    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-310+'px','top':Math.round(setTip.top-273)+'px','z-index':'100'});
							}
		    				else if($.browser.msie && $.browser.version == 9 && qtipObj.forumPopupType != 'Edit'){
		    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-440+'px','top':Math.round(setTip.top-276)+'px','z-index':'100'});
		    				}else if($.browser.msie && $.browser.version == 10 && qtipObj.forumPopupType != 'Edit'){
		    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-428+'px','top':Math.round(setTip.top-276)+'px','z-index':'100'});
		    				}else {
		    					if(qtipObj.forumPopupType == 'Edit') {
		    						$("#"+contentDiv).css({'position':'absolute','left':setTip.left-428+'px','top':Math.round(setTip.top-303)+'px','z-index':'100'});
		    					}else{
		    						$("#"+contentDiv).css({'position':'absolute','left':setTip.left-428+'px','top':Math.round(setTip.top-264)+'px','z-index':'100'});
		    					}
		    					
		    				}
	    				}
					}else{
						if($.browser.msie && parseInt($.browser.version, 10)=='7'){
	    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':Math.round(setTip.top-320)+'px','z-index':'100'});
						}
	    				else if($.browser.msie && parseInt($.browser.version, 10)=='8'){
	    					if(Math.round(setTip.left) > 650) {
		    				$("#"+contentDiv).css({'position':'absolute','left':setTip.left-440+'px','top':Math.round(setTip.top-320)+'px','z-index':'100'});
		    				}else{
	    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':Math.round(setTip.top-320)+'px','z-index':'100'});
		    				}
						}
	    				else if(navigator.userAgent.indexOf("Chrome")>=0) {
	    					if(Math.round(setTip.left) > 650) {
			    			$("#"+contentDiv).css({'position':'absolute','left':setTip.left-440+'px','top':Math.round(setTip.top-302
			    					)+'px','z-index':'100'});
			    			}else{
	    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':Math.round(setTip.top-300)+'px','z-index':'100'});
			    			}
	    				}
	    				else {
	    					if(Math.round(setTip.left) > 650) {
			    			$("#"+contentDiv).css({'position':'absolute','left':setTip.left-440+'px','top':Math.round(setTip.top-302)+'px','z-index':'100'});
			    			}else{
	    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':Math.round(setTip.top-300)+'px','z-index':'100'});
			    			}
	    				}
					}
				} else { // top end
    				$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
    				$(".set-wbubble-left").css({'top':+setTip.top+15+'px','left':setTip.left-25+'px','position':'absolute'}); //Changed the class name, since it is getting conflict with the account settings popup
    				$(".bottom-qtip-tip").remove();
    				if(Math.round(setTip.left) > 650) {
    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-440+'px','top':setTip.top+43+'px','z-index':'100'});
    				} else {
    					$("#"+contentDiv).css({'position':'absolute','left':setTip.left-70+'px','top':setTip.top+43+'px','z-index':'100'});
    				}
				}
			} else if(tipPosition == 'tipfaceTopRight'){
				$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
				$(".set-wbubble-left").css({'top':'20px','position':'absolute','right':'30px'}); //Changed the class name, since it is getting conflict with the account settings popup
				$(".bottom-qtip-tip").remove();
				$("#"+contentDiv).css({'position':'absolute','right':'0px','top':'49px','bottom':'0px','z-index':'100'});
			} else {
				$("#"+popupId).append("<div class='bottom-qtip-tip-up active-qtip-div set-wbubble-left'></div>");
				$(".set-wbubble-left").css({'top':'13px','position':'absolute'}); //Changed the class name, since it is getting conflict with the account settings popup
				$(".bottom-qtip-tip").remove();
				$("#"+contentDiv).css({'position':'absolute','left':'-'+qtipLeftPos+'px','top':'42px','bottom':'0px','z-index':'100'});
			}
    			
    		if($("#"+popupId+" .tab-title").width() > 0 ){
    			 var labelWidth = ($("#"+popupId+" .tab-title").width())/2;
    			 var bubbleWidth = 32;
    			 var setbubblePosition = labelWidth - bubbleWidth;
    			 $(".set-wbubble-left").css('left', setbubblePosition +'px');
    	    }

    		//Set buble tool tip arrow position							
    		tipPosition       = (qtipObj.tipPosition) ? qtipObj.tipPosition : 'bottomMiddle';
    		topElements = '<a class="qtip-close-button" onclick="$(\'.active-qtip-div\').remove();"></a><span id="popup_container_'+popupId+'"><table cellspacing="0" cellpadding="0" width="'+setwBubble+'px" height="'+hBubble+'px" id="bubble-face-table"><tbody><tr><td class="bubble-tl"></td><td class="bubble-t"></td><td class="bubble-tr"></td></tr><tr><td class="bubble-cl"></td><td valign="top" class="bubble-c">';
    		bottomElements ='</td><td class="bubble-cr"></td></tr><tr><td class="bubble-bl"></td><td class="bubble-blt"></td><td class="bubble-br"></td></tr></tbody></table></span>';
    		$("#"+popupId).parent().parent().parent().parent().removeClass("qtip-access-control");

    		var paintHtml = topElements+"<div id='paintContent"+popupId+"'><div id='show_expertus_message'></div>"+response.html+"</div>"+bottomElements;
    		var contentDiv = qtipObj.catalogVisibleId+"_disp";
    		$("#"+contentDiv).html(paintHtml);	
    		
    	    Drupal.attachBehaviors();
    	    
    }
    catch(e){
      //Nothing to do
    }
	
  };
})(jQuery);


function remove_forum_error_messages(val){
 try{	
	if(val == "topic") {
		$('#forum-topic .messages').remove();
	}else{
		$('#topic-comments .messages').remove();
	}
 }catch(e){
	  // to do
 }
}

function remove_qtip(val){
 try{	
	$(".active-qtip-div").remove();
 }catch(e){
	  // to do
 }
}