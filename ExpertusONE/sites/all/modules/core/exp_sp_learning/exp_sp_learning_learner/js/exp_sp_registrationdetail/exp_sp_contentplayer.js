;(function ( $, window, document, undefined ) {
	$.widget("ui.contentPlayer", {
		//Options to be used as defaults
        options: {
        	fullScreenControl : true,
        	statusBarControl : true,
        	contentList  : null,
        	defaultContent : 0,
        	isMultiLesson : 0,
        	lastPlayedObject : {},
        	playerMode : 'normal-mode',
        	LaunchFrom : ''
        },
        
        _init: function () {
        	self = this;
        	contentPlayer.closed = false;
        	self.lastPlayedObject = {};
        },
        initiateContentPlayer: function (data) {
        	var obj = this;
        	contentPlayer.closed = false;
        	var windowHeight = $(window).height();
        	var top = 100;
        	var variation = windowHeight - 690;
        	if (variation < 0) {
        		top = 0;
        	} else {
        		top = variation/2;
        	}
        	$('#cp-modal').css('top',top+'px');
        	obj.getLaunchObject(data.LaunchFrom);
        },
        setPlayerMode: function (mode) {
        	try {
        		var obj = this;
        		if (mode != undefined && mode != null)
        			obj.options.playerMode = mode;
        		
        	} catch(e) {
        		// to do
        	}
        },
        _create: function () {
        	//this._generateOverlayContent();
        },
        
        _generateOverlayContent: function() {
        	try {
        		var obj = this;
        		if ($("#cp-overlay").length > 0) {
        			return;
        		}
	        	var modalOverlay = $('<div id="cp-overlay-parent" class="cp-overlay-parent"></div><div id="modalBackdrop" style="z-index: 1000; display: none;"></div><div id="cp-overlay" class="cp-overlay"></div>');
	        	$('body').append(modalOverlay);
	        	obj._generateModalContent();
	        	obj._generatePlayerToolbar();
	        	obj._generatePlayerMenuContent();
//	        	if (this.options.statusBarControl) {
//	        		this._generatePlayerStatusBar();
//	        	}
	        	
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        _generateModalContent: function() {
        	try {
        		var modal = $('<div id="cp-modal" class="cp-modal"><div id="cp-modalcontainer" class="ui-resizable ui-draggable"><div id="cp-modalcontent"></div></div></div>');
        		$('#cp-overlay').append(modal);
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        _generatePlayerToolbar: function() {
        	try {
        		var obj = this;
        		var toolbar = '';
        		toolbar += '<div id="cp-toolbarcontainer" class="cp-toolbarcontainer">';
            	
            	toolbar += 		'<div id="cp-title" class="limit-title-row">';
                toolbar += 			'';
                toolbar +=  	'</div>';
                toolbar +=  	'<div id="cp-tools">';
                if (this.options.fullScreenControl) {
                	toolbar +=  		'<span class="cp-modal-fulscr"></span>';
                	toolbar +=  		'<span class="cp-modal-minscr" style="display:none;"></span>';
                }
                toolbar +=  		'<span class="icon-seperator">|</span> <span class="cp-modal-close">&times; </span>';
                toolbar +=  	'</div>';
                toolbar += '<div id="cp-menu-launcher"><div class="cp-menu-launcher-release" id="cp-menu-launcher-icon"></div></div>';
                toolbar +=  '</div>';
                
                if ($("#cp-toolbarcontainer").length > 0) {
        			$('#cp-toolbarcontainer').replaceWith(toolbar)
        		} else {
        			$('#cp-modalcontent').append(toolbar);
        		}
                $('.cp-modal-close').live('click', function(){ obj.closeModal(); });
                
                //$('.cp-modal-fulscr').live('click', function(){ obj.showFullScreen(''); });
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        _generatePlayerMenuContent: function() {
        	try {
        		var obj = this;
        		var menu = $('<div id="cp-menucontainer" class="cp-menucontainer" alt="show" oncontextmenu="return false;"></div>');
        		
        		if ($("#cp-menucontainer").length > 0) {
        			$('#cp-menucontainer').replaceWith(menu)
        		} else {
        			$('#cp-toolbarcontainer').after(menu);
        		}
        		$('#cp-menu-launcher').live('click', function(){ obj.showMenu(); });
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        _generatePlayerStatusBar: function() {
        	try {
        		var obj = this;        	
        		var status = '';       		
        		status += '<div id="cp-statusbarcontainer">';
        		status += 		'<div class="cp-action-btns">';
        		status += 			'<div id = "cp-done-btn" class="cp-modal-close cp-done-btn">'+Drupal.t('LBL569')+'</div>';
        		status +=  	'</div>';
        		status +=  '</div>';
        		if ($("#cp-statusbarcontainer").length > 0) {
        			$('#cp-statusbarcontainer').replaceWith(status)
        		} else {
        			$('#cp-contentcontainer').append(status);
        		}                
        	} catch(e) {
        		// console.log(e);
        	}
        },
        getPlayList: function(data) {
        	try {
        	var ajaxTime= new Date().getTime();
        		var obj = this;        	
        		var enrollId = data.enrollId        		       		
        		var masterEnrollId = data.masterEnrollId;     
        		var params = {};
        		if (data.programId > 0) {
        			var url = this.constructUrl('ajax/content-player/program-playlist/' + masterEnrollId); 
        		} else {
        			var url = this.constructUrl('ajax/content-player/playlist/' + enrollId);
        		}
        		$.ajax({
        			type: "POST",
					url: url,
					data:  params,
					success: function(result){
					var totalTime = new Date().getTime()-ajaxTime;			
						obj.options.contentList = result.menuList;
						obj.classDetails = result.classDetails;
						if(result.classDetails.cls_comp_status=='lrn_crs_cmp_inc'){
							obj.closeModal();
						}
						if(totalTime>1000){
						 playertimeoutcall(obj,data);
						}
						obj._generatePlayerMenu(data);
						$('#cp-modalcontainer #now-play-contentId-'+data.cType+'-'+data.contentId).show();
						if ($("#cp-content-navigation-container").length >  0) { // update sub content menu only for a multiple lesson content //data.playNext == 1 &&
							obj._generatePlayerContentMenu(data, 1);
		        			$('#cp-modalcontainer #now-play-subcontentId-'+data.LessonId).show();
						}
						 obj.aftercompletetrunk8();
					}
        		});
        	} catch(e) {
        		// console.log(e);
        	}
        },
        aftercompletetrunk8: function(){
        	$('.cp-menulist .limit-title').trunk8(trunk8.contstrip_title);
    		$('.enroll-course-title .limit-title').trunk8(trunk8.myenroll_title);
    		//72790: More and description V should expand all the details instead of only doing what we do today.
    		//$('.limit-desc').trunk8(trunk8.myenroll_desc);
    		//$('.limit-desc-lp').trunk8(trunk8.myprogramLP_desc);
			//$('.lp-class-title-details .limit-desc').trunk8(trunk8.myprogramClass_desc);
    		//$('.limit-desc-lp-mod').trunk8(trunk8.myprogramModule_desc);
    		$('.limit-title-lp').trunk8(trunk8.myprogramLP_title);
    		$('.lp-class-title-details .limit-title').trunk8(trunk8.myprogramClass_title);
			$('.limit-title-lp-mod').trunk8(trunk8.myprogramModule_title);
    		//$('.contplycls-limit-title').trunk8(trunk8.contPlyCls_title);
        },
        _generatePlayerMenu: function(data) {
        	try {
        		var obj = this;
        		obj.initiateContentPlayer(data);
        		var cpObj = "$(\'#"+ obj.options.LaunchFrom +"\').data(\'contentPlayer\')";
        		var totalList = obj.options.contentList;	
        		var player_nav_status, subcontent_id;
        		var listLen = Object.keys(totalList).length;
        		$('#cp-title').html('<span class="ContentPlayerTitle vtip limit-title contplycls-limit-title" title="'+htmlEntities(obj.classDetails.classTitle)+'">'+titleRestrictionFadeoutImage(obj.classDetails.classTitle,'content-player-title')+'</span><span class="ClassCount" > ('+obj.classDetails.completed_content_count+'/'+listLen+' '+ Drupal.t("Completed")+')</span>');
        		var menuHtml = '';
        		if(totalList.length>0){
        			if(data.pagefrom == 'seemore' || data.pagefrom == 'lp_seemore' || data.pagefrom == 'lp_class_seemore' || data.pagefrom == 'class_details' || data.pagefrom == 'course_class_list' || data.pagefrom =='lrnplan_course_class_list') {
        				var containerId = (data.programId > 0) ? 'cp-menulist-seemore-'+ data.containerId : 'cp-menulist-seemore-'+data.enrollId;
        				menuHtml += '<div class="cls-cp-menulist"><ul id="'+containerId+'" class="cp-menulist" oncontextmenu="return false;">';
        			} else {
        				menuHtml += '<div class="cls-cp-menulist"><ul id="cp-menulist-'+data.enrollId+'" class="cp-menulist" oncontextmenu="return false;">';
        			}	
        			$.each(totalList, function (i, list) {	
	        			var contentTitle = (totalList[i]['contentTitle']!=undefined && totalList[i]['contentTitle']!='')?totalList[i]['contentTitle']:totalList[i]['Code'];
	        			contentTitle = htmlEntities(contentTitle);
	        			var defaultContent =  (list.isMultiLesson == 1) ? totalList[i]['defaultContent'] :  i; 
	        			var contentId 	= totalList[i]['contentId'];
	        			var cType 		= totalList[i]['cType'];
	        			var playStatus  = totalList[i]['play_status'];
	                  if((typeof totalList[i]['cls_title'] != 'undefined') || (totalList[i]['contentType'] != "pre-assessment" && totalList[i]['contentType'] != "survey" && totalList[i]['contentType'] != "post-assessment")){    
	        			var prevCompStatus  = obj.classDetails.prev_comp_status;
	        			var prevClassTitle = obj.classDetails.prev_class_title;
	        			var clsStatus = obj.classDetails.prev_class_status;
	        			var classTitle = obj.classDetails.cls_title;
	        			var titleLength = (this.currTheme == 'expertusoneV2' && classTitle.length > 50) ? 50 : classTitle.length;
	        			
						var callDialogObj = '$(\'body\').data(\'learningcore\').callMessageWindow(\' '+ classTitle.replace(/"/g, '&quot;') +' \',\'' + Drupal.t("You need to complete") +' '+ escape(prevClassTitle) +' '+ Drupal.t("Course") + '\',  \''+titleLength+'\');';
                     } 

	        			var launchData = {
	                			masterEnrollId	 		: obj.setValue(data.masterEnrollId, 0),
	                			enrollId 				: obj.setValue(data.enrollId),
	                			programId				: obj.setValue(data.programId, 0),
	                			//classTitle 				: obj.setValue(htmlEntities(data.classTitle)),
	            				isMultiLesson			: obj.setValue(list.isMultiLesson, 0),
	            				contentType 			: totalList[i]['contentType'],
	            				cType 					: totalList[i]['cType'],
	            				//contentTitle			: saniztizeJSData(contentTitle),
	            				contentId 				: totalList[i]['contentId'],            				
	            				LessonId				: obj.setValue(totalList[i]['LessonId'], ''),
	            				VersionId				: obj.setValue(totalList[i]['VersionId'], ''),
	            				url1					: obj.setValue(totalList[i]['LearnerLaunchURL'], ''),		
	            				courseId				: obj.setValue(data.courseId, 0),
	            				classId					: obj.setValue(data.classId, 0),	
	            				url2					: obj.setValue(totalList[i]['PresenterLaunchURL'], ''),
	            				ErrMsg					: '', // always empty
	            				Status					: obj.setValue(totalList[i]['Status'], ''),
	            				LessonLocation			: obj.setValue(totalList[i]['LessonLocation'], ''),
	            				launch_data				: obj.setValue(totalList[i]['LaunchData'], ''),
	            				suspend_data			: obj.setValue(totalList[i]['SuspendData'], ''),
	            				exit					: obj.setValue(totalList[i]['CmiExit'], ''),
	            				AICC_SID				: obj.setValue(totalList[i]['AICC_SID'], ''),
	            				MasteryScore			: obj.setValue(totalList[i]['masteryscore'], ''),
	            				remDays					: totalList[i]['remDays'],
	            				ValidityDays			: totalList[i]['ValidityDays'],
	            				no_of_attempts			: totalList[i]['no_of_attempts'],
	            				AttemptLeft				: totalList[i]['AttemptLeft'],
	            				attempts				: totalList[i]['attempts'],
	            				MaxAttempt				: totalList[i]['MaxAttempt'],
	            				surveycount				: totalList[i]['surveycount'],
	            				is_post_ass_launch		: totalList[i]['is_post_ass_launch'],
	            				playStatus		        : totalList[i]['play_status'],
	            				access_denied_flag      : totalList[i]['access_denied_flag'],
	            				defaultContent			: defaultContent,
	            				pagefrom				: obj.setValue(totalList[i]['pagefrom'],''),
	            				LaunchFrom				: data.LaunchFrom,	            				
	            				containerId				: data.containerId,	            				
	            	    }
	        			launchData = objectToString(launchData);
	        			var cssContentClass = 'icn-' + totalList[i]['contentType'].toLowerCase().replace(/ /g, '-').replace(/\./g, '');
	        			menuHtml += '<li class="cp-list-menu-'+cType+'-'+contentId +'">';
	        			menuHtml += '	<div class="cp-menuitem">';
	        			menuHtml += '		<div class="cp-menu-details"><div class="cp-menu-details-holder">';
	        			var classIconVtip = GetLabelsForContentType(Drupal.t(totalList[i]['contentType']));
	        			var anchormenuHtml = '';
	        			anchormenuHtml += '			<div class="cp-menu-icon">';
	        			anchormenuHtml += '				<span title = "'+classIconVtip+'" class="content-type-icon '+cssContentClass+' vtip" ></span>';
	        			anchormenuHtml += '				<div class="conten-play-status current-status-disable" id="now-play-contentId-'+cType+'-'+contentId+'" style="display:none"><span>'+Drupal.t('Now')+'</span><span> '+Drupal.t('Playing')+'</span></div>';
	        			anchormenuHtml += '			</div>';
	        			anchormenuHtml += '			<div class="cp-menu-action limit-title-row">';
	        			var validityDay ='';
	        			if (totalList[i]['ValidityDays'] !== ''  && totalList[i]['ValidityDays']!=undefined) {
	        		    	var remValidityDays = (totalList[i]['remDays'] !== undefined && totalList[i]['remDays'] != null) ? parseInt(totalList[i]['remDays']) : '';
	        				  if(remValidityDays <= 0){
	        				  var daysLBL = Drupal.t("Expired");//Expired
	        				  remValidityDays = '';// To avoid result as Validity: 0 Expired
	        				  }else if(remValidityDays == 1){
	        				  var daysLBL = Drupal.t("LBL910");//day
	        				  }else if(remValidityDays > 1){
	        				  var daysLBL = Drupal.t("LBL605");//days
	        				  }
	        		    	validityDay = remValidityDays + ' ' + daysLBL;
	        		    }
	        			if(prevCompStatus == 'notallow' && clsStatus != 'lrn_crs_cmp_cmp' && data.pagefrom != 'catalog' && (data.pagefrom == 'lp_seemore' || data.pagefrom =='lrnplan_course_class_list' || data.pagefrom == 'lp_class_seemore')){
	        			//if(prevCompStatus == 'notallow' && clsStatus != 'lrn_crs_cmp_cmp' && data.pagefrom != 'catalog' && (data.pagefrom != 'seemore' || data.pagefrom !='class_details' || data.pagefrom != 'course_class_list')){
	        				if(data.pagefrom == 'seemore' || data.pagefrom == 'lp_seemore' || data.pagefrom == 'lp_class_seemore' ||  data.pagefrom == 'class_details' || data.pagefrom == 'course_class_list' || data.pagefrom =='lrnplan_course_class_list'){
								menuHtml += '<a class="clsMenulistTitle clsInActiveMenu" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="' +callDialogObj+' ">';
							} else {
								menuHtml += '<a class="clsMenulistTitle clsInActiveMenu" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="'+cpObj+'.changePlayerContent('+launchData+');">';
		        			}
	        				menuHtml += anchormenuHtml+'<span title="'+contentTitle+'" class="vtip clsInActiveMenu limit-title">'+ contentTitle +'</span></div></a>';
	        			}else if(playStatus == true){
							if(data.pagefrom == 'seemore' || data.pagefrom == 'lp_seemore' || data.pagefrom == 'lp_class_seemore' || data.pagefrom == 'class_details' || data.pagefrom == 'course_class_list' || data.pagefrom =='lrnplan_course_class_list'){
								menuHtml += '<a class="clsMenulistTitle" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="'+cpObj+'.playContent(' + launchData + ');">';
							} else {
								menuHtml += '<a class="clsMenulistTitle" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="'+cpObj+'.changePlayerContent('+launchData+');">';
		        			}
							menuHtml += anchormenuHtml+'<span title="'+contentTitle+'" class="vtip clsActiveMenu limit-title">'+ contentTitle +'</span></div></a>';
	        			}else{
							//menuHtml += '<a class="clsMenulistTitle" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="">';
	        				if(data.pagefrom == 'seemore' || data.pagefrom == 'lp_seemore' || data.pagefrom == 'lp_class_seemore' ||  data.pagefrom == 'class_details' || data.pagefrom == 'course_class_list' || data.pagefrom =='lrnplan_course_class_list'){
								menuHtml += '<a class="clsMenulistTitle clsInActiveMenu" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="'+cpObj+'.playContent(' + launchData + ');">';
							} else {
								menuHtml += '<a class="clsMenulistTitle clsInActiveMenu" id="selconetentmenulink-'+cType+'-'+contentId +'" href="javascript:void(0);" onclick="'+cpObj+'.changePlayerContent('+launchData+');">';
		        			}
	        				menuHtml += anchormenuHtml+'<span title="'+contentTitle+'" class="vtip clsInActiveMenu limit-title">'+ contentTitle +'</span></div></a>';
	        			}
	        			menuHtml += '			<div class="cp-menu-detail">';
	        			if((totalList[i]['AttemptLeft']!=='' && totalList[i]['AttemptLeft']!=undefined && totalList[i]['AttemptLeft']!='notset') || (totalList[i]['ValidityDays']!=='' && totalList[i]['ValidityDays']!=undefined)  || (totalList[i]['VersionNum']!=='' && totalList[i]['VersionNum']!=undefined && totalList[i]['VersionNum']!=null)){
	        				var clsID = (data.classId==0)? data.programId:data.classId;
	        				var enrID = data.enrollId;
	        				menuHtml += '<span class="clsShowContentValidty" onclick="'+cpObj+'.showContentValidation(\''+cType+'_'+totalList[i]['contentId']+'_'+clsID+'_'+enrID+'\');"></span><span style="display:none;" class ="clsConentValidtiy" id="sel_content_'+cType+'_'+totalList[i]['contentId']+'_'+clsID+'_'+enrID+'">';
		        			var onlyVersionForH5P = true;
		        			if(totalList[i]['AttemptLeft']!=='' && totalList[i]['AttemptLeft']!=undefined && totalList[i]['AttemptLeft']!='notset')
		        			{
		        				onlyVersionForH5P = false;
		        				menuHtml += '<span class="clsAttemptLeft"><div class="clsContentValidtyLBL" >'+Drupal.t('LBL857')+'</div><div>:</div><div class="clsContentValidtyVal">'+totalList[i]['AttemptLeft']+' '+Drupal.t('LBL3072')+'</div></span>';
		        			}
		        			if(totalList[i]['ValidityDays']!=='' && totalList[i]['ValidityDays']!=undefined)
		        			{
		        				onlyVersionForH5P = false;
		        				menuHtml += '<span class="clsValidityDays"><div class="clsContentValidtyLBL">'+Drupal.t('LBL604')+'</div><div>:</div><div class="clsContentValidtyVal">'+validityDay+'</div></span>';
		        			}if(totalList[i]['VersionNum']!=='' && totalList[i]['VersionNum']!=undefined && totalList[i]['VersionNum']!=null)
		        			{
								//h5pcustomize - hide version label for h5p contents 74159
	        					var isNotH5PContent  = true;
	        					if(totalList[i]['LearnerLaunchURL'] != null && (totalList[i]['subtype'] == 'h5p-video-mp4' || totalList[i]['subtype'] == 'h5p-Youtube' || totalList[i]['subtype'] == 'h5p-Vimeo' || totalList[i]['subtype'] == 'h5p-video-webm'||  totalList[i]['subtype'] == "h5p-presentatn" ))
	        					{
	        						isNotH5PContent = false;
	        					}
		        				if(isNotH5PContent)
		        					menuHtml += '<span class="clsValidityDays"><div class="clsContentValidtyLBL">'+Drupal.t('LBL1123')+'</div><div>:</div><div class="clsContentValidtyVal">'+totalList[i]['VersionNum']+'</div></span>';
		        				else if(onlyVersionForH5P){ //display There are no details. Right now, we are not supporting version for h5p content
		        					menuHtml += '<div class="no-records-msg">'+Drupal.t('MSG278')+'</div>';
		        				}	
		        			}
	        				menuHtml += '<span class="clstooltiparrow"></span> </span>';
	        			}
	        			menuHtml += '</div>';	
	        			menuHtml += '			</div></div>';
	        			if(totalList[i]['contentType'] == "pre-assessment" || totalList[i]['contentType'] == "survey" || totalList[i]['contentType'] == "post-assessment"){
	        				var content_id = totalList[i]['contentId'];
	        				var type = 'survey_assessment';
	        			}else{
	        				var content_id = totalList[i]['contentId'];
	        				var type = 'content';
	        			}
	        			
	        			if (data.programId > 0) {
	        				 obj.progressbarLine(data.containerId,totalList[i]['content_completion'], content_id, data.pagefrom, type);
	        				 menuHtml += '			<div id="cp-menu-progress_'+ data.containerId +'_'+content_id+'_'+type+'" class="cp-menu-progress yogaraja">';  
	        			} else {
	        				 obj.progressbarLine(data.enrollId,totalList[i]['content_completion'],content_id,data.pagefrom,type);
	        				 menuHtml += '			<div id="cp-menu-progress_'+data.enrollId+'_'+content_id+'_'+type+'" class="cp-menu-progress">';  
	        			}
	        			
	            		 
	            		
	            		menuHtml += '			</div>';
	            		menuHtml += '	</div>';
	            		menuHtml += '</li>';
	        		});
	        		menuHtml += '</ul></div>';   
        		}else{
        			if(data.pagefrom != 'seemore' && data.pagefrom != 'lp_seemore' && data.pagefrom != 'lp_class_seemore' && data.pagefrom != 'class_details' && data.pagefrom != 'course_class_list' &&  data.pagefrom !='lrnplan_course_class_list') 
        				menuHtml +='<div id="sel-cp-menulist-nomenu-'+data.enrollId+'" style="font-family:openSansRegular, Arial; font-size:12px; color:#aaa; text-align:center; padding:40px 0; border:1px solid #ddd;">'+Drupal.t('LBL635')+'</div>'
            	}
        		
        		if(listLen<2){
	        		//console.log("checked play list count");
	        		$("#cp-menucontainer").addClass("cp-menulist-border");
	        	}else{
	        		$("#cp-menucontainer").removeClass("cp-menulist-border");
	        	}
        		
        		obj.applyCarouselEffect(data, menuHtml); 
        		if(data.pagefrom != 'class_details' && data.pagefrom != 'course_class_list' && data.pagefrom !='lrnplan_course_class_list')
        			$('.cp-menulist .limit-title').trunk8(trunk8.contstrip_title);
        		vtip();
        	} catch(e) {
        		// console.log(e);
        	}
        },
        applyCarouselEffect : function(data, menuHtml) {
        	try {
        		var obj 			= this;
        		var carouselCount 	= 4;
        		var cpObj 			= "$(\'#"+ obj.options.LaunchFrom +"\').data(\'contentPlayer\')";
        		var totalList 		= obj.options.contentList;	
        		var listLen = Object.keys(totalList).length;
        		if(data.pagefrom == 'seemore'){
        			var menuWidth		= 188;
        			var menucontainerWidth = '755px';
        			$('#seemore_'+data.enrollId).css('display', 'none');
        			$('#paindContentResults_'+data.enrollId).css('display', 'block');
        			var seelessHtml = '<div class="cp_seeless" id="seeless_'+data.enrollId+'" onclick="'+cpObj+'.seelesscontent('+data.enrollId+');">'+ Drupal.t('LBL3042') +'</div>';
        			$('#paindContentResults_'+data.enrollId).html(menuHtml);
        			if(document.getElementById("seeless_")+data.enrollId)
        				$('#seeless_'+data.enrollId).remove();
        			setTimeout(function(){$('#'+data.enrollId+'SubGrid').after(seelessHtml);},300);
        			if(totalList.length<=0)
        				$('#paindContentResults_'+data.enrollId).css('display','none');
        			if(listLen > carouselCount){
        				$('#paindLPContentResults_'+data.enrollId + ' .cls-cp-menulist').removeClass('menulist-lt4');
	        			if($(".clsContentFull").length){		        				
	        				$('#paindContentResults_'+data.enrollId).width('755px');
	        			}else{
	        				$('#paindContentResults_'+data.enrollId).width('583px');
	        			}
        			}else{
        				$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
        				menucontainerWidth = ( totalList.length  * menuWidth ) + 'px';
        				$('#paindContentResults_'+data.enrollId + ' .cls-cp-menulist').css('width', menucontainerWidth).addClass('menulist-lt4');
        			}      			
        			obj.destroyLoader('enroll_details_'+data.enrollId);
        		}else if(data.pagefrom == 'class_details'){
        			var menuWidth		= 196;
        			var menucontainerWidth = '591px';
        			carouselCount = 3;
        			$('#paindContentResults_'+data.enrollId).css('display', 'block');
        			$('#paindContentResults_'+data.enrollId).html(menuHtml);
        			if(totalList.length<=0)
        				$('#paindContentResults_'+data.enrollId).css('display','none');

        			if(listLen > carouselCount){
        				$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
	        		}else{
	        			$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
        				menucontainerWidth = ( totalList.length  * menuWidth ) + 'px';
        				$('#paindContentResults_'+data.enrollId + ' .cls-cp-menulist').css('width', menucontainerWidth).addClass('menulist-lt4');
        			}      	
        		}else if(data.pagefrom == 'course_class_list' || data.pagefrom =='lrnplan_course_class_list'){
        			var menuWidth		= 196;
        			var menucontainerWidth = '394px';
        			carouselCount = 2;
        			$('#paindContentResults_'+data.enrollId).css('display', 'block');
        			$('#paindContentResults_'+data.enrollId).html(menuHtml);
        			if(totalList.length<=0)
        				$('#paindContentResults_'+data.enrollId).css('display','none');
        			if(listLen > carouselCount){
        				$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
	        		}else{
	        			$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
        				menucontainerWidth = ( totalList.length  * menuWidth ) + 'px';
        				$('#paindContentResults_'+data.enrollId + ' .cls-cp-menulist').css('width', menucontainerWidth).addClass('menulist-lt4');
        			}      	
        		} else if(data.pagefrom == 'lp_seemore'){
        			var menuWidth		= 188;
        			var menucontainerWidth = '755px';
        			var containerId = data.containerId;			
        			var seelessHtml = '<tr id="lp_seeless_tr_'+containerId+'" data-rowid='+containerId+' class="jqgcrow ui-row-'+containerId+'"><td>&nbsp;</td><td colspan="2"><div class="lp_seeless" id="lp_seeless_'+containerId+'" onclick="'+cpObj+'.seelessLPContent(\''+containerId+'\');">'+ Drupal.t('LBL3042') +'</div></td></tr>';
        			$('#paindLPContentResults_'+containerId).html(menuHtml);
        			
        			if(document.getElementById("lp_seeless_tr_")+containerId)
        				$('#lp_seeless_tr_'+containerId).remove();
        			
        			//var afterElement = ($('#'+containerId+'ModuleGrid').length) ? "#" +containerId+"ModuleGrid" : "#" +containerId+'SubGrid';
        			$("#" +containerId+'SubGrid').after(seelessHtml);
        			
        			if(document.getElementById(containerId+"SubGrid")){
        				$('#lp_seemore_'+containerId).css('display','none');
	        			$('#lp_seeless_tr_'+containerId).css('display','table-row');
        			} else {
        				$('#lp_seemore_'+containerId).css('display', 'none');
        			}
        			
        			if(totalList.length<=0)
        				$('#paindLPContentResults_'+containerId).css('display','none');
        			if(listLen > carouselCount){
        				$('#paindLPContentResults_'+containerId + ' .cls-cp-menulist').removeClass('menulist-lt4');
	        			if($(".clsContentFull").length){		        				
	        				$('#paindLPContentResults_'+containerId).width('755px');
	        			}else{
	        				$('#paindLPContentResults_'+containerId).width('583px');
	        			}
        			}else{
        				$('#paindLPContentResults_'+containerId).width(menucontainerWidth);
        				menucontainerWidth = ( totalList.length  * menuWidth ) + 'px';
        				$('#paindLPContentResults_'+containerId + ' .cls-cp-menulist').css('width', menucontainerWidth).addClass('menulist-lt4');
        			}
        			
        			
        		} else if(data.pagefrom == 'lp_class_seemore'){
        			var menuWidth		= 177;
        			var menucontainerWidth = '707px';
        			$('#lp_class_seemore_'+data.enrollId).css('display', 'none');
        			$('#paindContentResults_'+data.enrollId).css('display', 'block');
        			$('#paindContentResults_'+data.enrollId).html(menuHtml);
        			if(totalList.length<=0)
        				$('#paindContentResults_'+data.enrollId).css('display','none');
        			if(listLen > carouselCount){
        				$('#paindLPContentResults_'+data.enrollId + ' .cls-cp-menulist').removeClass('menulist-lt4');
	        			if($(".clsContentFull").length){		        				
	        				$('#paindContentResults_'+data.enrollId).width('710px');
	        			}else{
	        				$('#paindContentResults_'+data.enrollId).width('583px');
	        			}
        			}else{
        				$('#paindContentResults_'+data.enrollId).width(menucontainerWidth);
        				menucontainerWidth = ( totalList.length  * menuWidth ) + 'px';
        				$('#paindContentResults_'+data.enrollId + ' .cls-cp-menulist').css('width', menucontainerWidth).addClass('menulist-lt4');
        			}    
        		}else{        			
        			$('#cp-menucontainer').html(menuHtml);
        		}
        		if (listLen > carouselCount) { // apply jcarousel more then 4 list element
        			var carouselcontainer = (data.programId > 0) ? data.containerId : data.enrollId;
        			obj.attachMenuCarousel(listLen, data.pagefrom, carouselcontainer);
        		}
        		if(data.pagefrom=='lp_class_seemore' || data.pagefrom=='lp_seemore'){
        			obj.destroyLoader('enroll-lp-result-container');
        		}
        	} catch(e) {
        		// console.log(e);
        	}
        },
        // need to remove
        updatePlayerChangeState : function(status) {
        	try {
        		contentPlayer.changed = (status == 'true') ? true : false;
        	} catch(e) {
        		// to do
        	}
        },
        showContentValidation: function(content_id){
        	if($('#cp-modal').css('display')=='none' || $('#cp-modal').css('display')==undefined){
        		$this = $("#sel_content_"+content_id);
        		$(".clsConentValidtiy").not($this).hide();
        		$("#sel_content_"+content_id).toggle();
        	}else{
        		$this = $("#cp-modalcontainer #sel_content_"+content_id);
        		$("#cp-modalcontainer .clsConentValidtiy").not($this).hide();
        		$("#cp-modalcontainer #sel_content_"+content_id).toggle();
        	}
        },
        changePlayerContent : function (data) {
        	try {
        		var obj = this;
				obj.createLoader("cp-modalcontainer");
        		//obj.initiateContentPlayer(data);
        		var prevObject = obj.lastPlayedObject;
				if ($.isEmptyObject(prevObject)) {
					// log('prev object empty');
					obj.getPlayList(data); 
					obj.getPlayerContent(data);
				} else {
					//make update call here
					data.closed = false;
					obj.updatePlayedStatus(data);
					//obj.getPlayList(data);
     			}
        	} catch(e) {
        		// to do
        		// console.log(e);
        	}
        },
        updatePlayedStatus: function(data) {
        	try {
        			var obj = this;
        			var prevObject = obj.lastPlayedObject;
					var contentType = prevObject.contentType;
					data.launchObject	= obj.options.LaunchFrom;
					if($("#cp-modal").css('display') == 'none' && prevObject.play_status == true){
						if(document.getElementById('lnr-catalog-search') && obj.options.LaunchFrom == 'lnr-catalog-search'){
		                  	$("#lnr-catalog-search").data("lnrcatalogsearch").createLoader('searchRecordsPaint');
		                  	if(document.getElementById("course-details-display-content"))
		                  		$("#course-details-display-content").data("lnrcoursedetails").createLoader('class-list-loader');
                            if(document.getElementById("class_detail_content")){
	                  			$("#class_detail_content").data("lnrclassdetails").createLoader('class_detail_content');
		                  	}
		                	if(document.getElementById('learning-plan-details-display-content'))
                                $("#learning-plan-details-display-content").data("lnrplandetails").createLoader('class-container-loader-'+prevObject.classId);
		                }
						//if(contentType != 'Video on Demand'){	
		        		if(document.getElementById('learner-enrollment-tab-inner') && obj.options.LaunchFrom == 'learner-enrollment-tab-inner'){
		        			$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
		        		}
		        		if(document.getElementById('learningplan-tab-inner')  && obj.options.LaunchFrom == 'learningplan-tab-inner'){
		        			$("#learningplan-tab-inner").data("learningplan").createLoader('enroll-lp-result-container');
		        		}
					//}
					}
					
					// Update Respective Content Types 
					if (prevObject.play_status == true) {
						switch (contentType) {
							case 'SCORM 1.2':
							case 'SCORM 2004':
							case 'Knowledge Content':
								
								var result = {};
		
		 result.completionStatus	= "completed";
		        result.status			= "completed";
		        result.score			= "0";
		        result.location			= "0";
		        result.totalTime		= "0";
		        result.sessionTime		= "";
		    
								var callback_param =  undefined;
							
								if(data.access_denied_flag==1 || prevObject.access_denied_flag==1){
								  obj.getPlayList(data);
	                  		  setTimeout(function(){obj.getPlayerContent(data);},500);
								//alert('entered');
								obj.destroyLoader('launch-wizard');
								obj.destroyLoader("cp-modalcontainer");
							  if(document.getElementById('learner-enrollment-tab-inner') ){
	                          obj.destroyLoader("cp-modalcontainer");
							//	$("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
					        		}
					        		if(document.getElementById('learningplan-tab-inner') ){
								$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
					        		}
								//$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
						if(document.getElementById('launch-wizard'))
							//obj.destroyLoader('launch-wizard');
							$('#class_detail_content #launch-wizard').hide();
							if(document.getElementById("course-details-display-content"))
					    	  $("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
					    	if(document.getElementById('learning-plan-details-display-content'))
            				  $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+clid);
            				if(document.getElementById("class_detail_content"))
	                  		  $("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
	                  		 

							}else{
								
								$('#' + obj.options.LaunchFrom).data('contentLaunch').updateScore(callback_param, data);
							
							
								}
								
								break;
							case 'AICC':
							case 'Tin Can':
								var callback_param =  (contentType == 'AICC' || contentType == 'Tin Can') ? scoobj.callback_param : undefined;
								setTimeout(function(){ $('#' + obj.options.LaunchFrom).data('contentLaunch').updateScore(callback_param, data); }, 500);
								break;
							case 'Video on Demand':
								var launchedFrom 	= prevObject.LaunchFrom;
								var courseId 		= prevObject.courseId;
								var	classId			= prevObject.classId;
								var	lessonId		= prevObject.LessonId;
								var	versionId		= prevObject.VersionId;
								var	enrollId		= prevObject.enrollId;
								var prevContentStatus = '';
								var contentProgress = '';
								$('#' + obj.options.LaunchFrom).data('contentLaunch').updateVODScoreOnCtoolsModalClose(launchedFrom, courseId, classId, lessonId, versionId, enrollId, prevContentStatus, contentProgress, data);
								break;
							case 'pre-assessment':
							case 'post-assessment':
							case 'survey':
								var min_mark		= prevObject.minMark;	
								var max_mark		= prevObject.maxMark;
								var total_score		= prevObject.maxMark;
								var status			= prevObject.status;
								status				= 'suspend';
								var pre_status		= prevObject.preStatus;
								var survey_id 		= prevObject.contentId;
								var surveyCommitted = $("#block-take-survey").data('surveylearner').committed;
								if (surveyCommitted != 1)
									$("#block-take-survey").data('surveylearner').saveCurrentState(min_mark, max_mark, total_score, status, pre_status, survey_id, data); 
								else{
								    if (data.closed != true)
									$('#' + obj.options.LaunchFrom).data('contentPlayer').playNextContent(data);
									if(document.getElementById('lnr-catalog-search') && obj.options.LaunchFrom == 'lnr-catalog-search'){
										if(document.getElementById("lnr-catalog-search"))
					                  	$("#lnr-catalog-search").data("lnrcatalogsearch").destroyLoader('searchRecordsPaint');
					                  	if(document.getElementById("course-details-display-content")){
					                  		$("#course-details-display-content").data("lnrcoursedetails").destroyLoader('class-list-loader');
					                  	}
					                  	else if(document.getElementById("class_detail_content")){
					                  		$('#paindContentclsid_'+prevObject.classId).click();
				                  			$("#class_detail_content").data("lnrclassdetails").destroyLoader('class_detail_content');
					                  	}
					                  	if(document.getElementById('learning-plan-details-display-content'))
			                                $("#learning-plan-details-display-content").data("lnrplandetails").destroyLoader('class-container-loader-'+prevObject.classId);
					                }
								}
							//	if(contentType != 'Video on Demand'){						
									if(document.getElementById('learner-enrollment-tab-inner') && obj.options.LaunchFrom == 'learner-enrollment-tab-inner'){
								$("#learner-enrollment-tab-inner").data("enrollment").destroyLoader('enroll-result-container');
					        		}
					        		if(document.getElementById('learningplan-tab-inner')  && obj.options.LaunchFrom == 'learningplan-tab-inner'){
								$("#learningplan-tab-inner").data("learningplan").destroyLoader('enroll-lp-result-container');
					        		}	
							//	}
							$("#learning-plan-details-display-content").data("lnrplandetails").renderCoursesList(1);
								$("#block-take-survey").surveylearner('destroy');
								break;	
							default:
								break;
						}
					}
					
					if (data.closed != true && prevObject.play_status == false){
						$('#' + obj.options.LaunchFrom).data('contentPlayer').playNextContent(data);
        			} else {
						// close loaders;
        			}	
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        playNextContent: function(data) {
        	try {
        		data.playNext = 1;
        		// Update Main content
        		$("#"+data.launchObject).data('contentPlayer').getPlayList(data);
        		setTimeout(function(){
	        			$("#"+data.launchObject).data('contentPlayer').getPlayerContent(data);
	        		},500);
        		$('#cp-content-navigation').removeAttr("style");
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        getLaunchObject : function(launchFrom) {
        	try {
        		var launchObject = '';
        		var obj = this;
        		if (launchFrom != undefined && launchFrom != null) {
        			switch (launchFrom) {
        				case 'CL': 
        					launchObject = 'lnr-catalog-search';
        					break;
        				case 'ME':
        					launchObject = 'learner-enrollment-tab-inner';
        					break;
        				case 'LP':
        					launchObject = 'learningplan-tab-inner';
        					break;
        				default:
        					launchObject = 'learner-enrollment-tab-inner';
        					break;
        			}
        		}
        		obj.options.LaunchFrom = launchObject;
        		return launchObject;
        	} catch(e) {
        		
        	}
        },
        getPlayerContent: function(data) {
			try {
		    var denied='';
			if(data.access_denied_flag!="undefined" ){
			
			denied = data.access_denied_flag;
			
			}
				var obj = this;
				if (data.defaultContent=='notset') { 
					var contentHtml='';var content = '';
					if(data.lp_have_only_post_ass_cont != undefined && data.lp_have_only_post_ass_cont == true) {
						content = Drupal.t('MSG688');
				    } else if (data.cl_ilt_have_only_post_ass_cont !=undefined && data.cl_ilt_have_only_post_ass_cont==true) {
				    	content = Drupal.t('MSG384');
				    } else {
				    	var contentName = (obj.options.contentList[0]['contentTitle'] != '' && obj.options.contentList[0]['contentTitle'] != 'undefined' && obj.options.contentList[0]['contentTitle'] != undefined) ? obj.options.contentList[0]['contentTitle'] : Drupal.t('Content');
				    	content = contentName + '&nbsp;' + '<b>'+Drupal.t('MSG796')+'</b>';/*Viswanathan added >b> tag for #0078402*/		
				    }	
					//content = 'Curenntly no content available to play !';
					contentHtml += '<div id="cp-contentcontainer" class="cp-contentcontainer cp-full-player" style="height: auto; width: 99%;">';
        			contentHtml += '	<div id="cp-content-main"><span class="msg-content">' + content + '</span></div>';
        			contentHtml += '</div>';
        			if ($("#cp-contentcontainer").length > 0) {
            			$("#cp-contentcontainer").replaceWith(contentHtml)
            		} else {
            			$('#cp-menucontainer').after(contentHtml);
            		}
        			this.windowAdjustment(data);
            		this.resizeInnerContent();
//           			$('#cp-overlay .cp-modal-fulscr').hide();
//           			$('#cp-overlay .cp-modal-minscr').hide();
//           			$("#cp-overlay .cp-modal-minscr").next("span").css("display", "none");
        			obj.destroyLoader("cp-modalcontainer");
        			setTimeout(function(){
	        			$('#cp-modalcontainer .cls-cp-menulist ul li:first-child .conten-play-status').show();
	        		},50);
        		}else{
					obj.setDefaultContent(data.defaultContent);
					// enrollment data
					var enrollId  				= obj.setValue(data.enrollId, 0);
					var masterEnrollId  		= obj.setValue(data.masterEnrollId, 0);
					var defaultContent			= obj.defaultContent;
					var isMultiLesson			= parseInt(obj.isMultiLesson);
					if (isMultiLesson == 1) {
						var contentLaunchData		= obj.options.contentList[obj.defaultContent[0]][obj.defaultContent[1]];
					} else {
						var contentLaunchData		= obj.options.contentList[defaultContent];
					}
					contentLaunchData.enrollId  		= enrollId;
					contentLaunchData.masterEnrollId  	= masterEnrollId;
					contentLaunchData.pagefrom  		= obj.setValue(data.pagefrom,'');
					contentLaunchData.programId   		= obj.setValue(data.programId, 0);
					contentLaunchData.classId    		= obj.setValue(data.classId, 0);
					contentLaunchData.courseId   		= obj.setValue(data.courseId, 0);
					contentLaunchData.classTitle 		= obj.setValue(data.classTitle, '');
					contentLaunchData.userId 			= obj.setValue(data.userId, '');
					contentLaunchData.isMultiLesson 	= isMultiLesson;
					contentLaunchData.LaunchFrom 		= obj.setValue(data.LaunchFrom, '');;
					contentLaunchData.suspend_data 		= obj.setValue(contentLaunchData.SuspendData, '');
					contentLaunchData.playNext 			= obj.setValue(data.playNext, '');
					contentLaunchData.preStatus			= (data.contentType == 'pre-assessment') ? 1 : 0;
					
	        		var params = contentLaunchData;
	        		obj.lastPlayedObject = params;  
	        		
	        		if (isMultiLesson == 1) {
	        			var url = this.constructUrl('ajax/content-player/play-subcontent');
	            		} else {
	        			var url = this.constructUrl('ajax/content-player/playcontent');
					}
	        		$.ajax({
	        			type: "POST",
						url: url,
						data:  params,
						success: function(result){
							obj._generatePlayerContent(result, contentLaunchData);
							obj.playerCompleteCall(contentLaunchData);
	            		}
	        		});
        		}
			
        		//obj.destroyLoader("cp-modalcontainer");
        	} catch(e) {
        		// console.log(e);
        		// console.log(e.stack);
        	}
		},
		playerCompleteCall: function() {
			try {
				var obj = this;
        		var isMultiLesson = obj.isMultiLesson;
        		if (isMultiLesson) {
        			$('.cp-content-navigation-control').css('margin-left', $('#cp-content-navigation').width() - 18);
        		}
        		obj.destroyLoader("cp-modalcontainer");
        	} catch(e) {
				
			}
		},
		
		setDefaultContent: function(content) {
			try {
				var obj = this; 
				if (content !== undefined || content != '' || content !='undefined') {
					var contentParts = content.split('-');
					if (contentParts.length > 1) {
						obj.defaultContent = contentParts;
						obj.isMultiLesson = 1;
					} else {
						obj.defaultContent = contentParts[0];
						obj.isMultiLesson = 0;
					}
				}
			} catch(e) {
        		// console.log(e);
        	}
		},
        
        _generatePlayerContent: function(result, data) {
        	try {
        		var obj = this;
        		var content, contentHtml = ''; 
        		var isMultiLesson = obj.isMultiLesson;
        		var containerClass = (isMultiLesson == 1) ? 'cp-nav-player' : 'cp-full-player';
        		var defaultContent;
        		
        		if (data.play_status == true && data.access_denied_flag!=1) {
       
        			// when multi-lesson launched.
            		if (isMultiLesson == 1) { 
            			var content 		= obj.setValue(result.content, 'No Content found !');
            			if ($("#cp-content-navigation-container").length > 0) {  // call from next lesson call don't refresh menu. // data.playNext == 1 && 
            				contentHtml = '	<div id="cp-content-main">' + content + '</div>';
            			} else { 
    	        			var subMenuHtml = '';
    	        			subMenuHtml = obj._generatePlayerContentMenu(data, 0);
    	            		
    	            		// Build the content
    	        			contentHtml += '<div id="cp-contentcontainer" class="cp-contentcontainer '+ containerClass +'">';
    	        			contentHtml += '<a href="javascript:void(0)" class="cp-content-navigation-control">&lt;</a>';
    	        			contentHtml += '	<div id="cp-content-navigation">';
    	        			contentHtml += '		<div id="cp-content-navigation-container">' + subMenuHtml + '</div>';
    	        			contentHtml += '	</div>';
    	        			contentHtml += '	<div id="cp-content-main">' + content + '</div>';
    	        			contentHtml += '</div>';
    	        			$('.cp-content-navigation-control').live('click', function(){ obj.showContentNavMenu(); });
            			}	
                		
            		} else {
            			if(data.contentType == 'pre-assessment' || data.contentType == 'post-assessment'){
            				var enrollId, paramsUpdScore, resultConainer = '';
            				$("#block-take-survey").surveylearner();
            				var surObj		= $("#block-take-survey").data("surveylearner");
            				contentHtml 	= result.content;
            				if (data.programId > 0) {
            					enrollId			= data.masterEnrollId;
            					paramsUpdScore 		= data.programId+"###"+data.masterEnrollId;
            					resultConainer		= 'enroll-lp-result-container';
            				} else {
            					enrollId			= data.enrollId;
            					paramsUpdScore 		= data.classId+"###"+data.enrollId+"###"+data.courseId+"###"+data.classId;
            					resultConainer		= 'enroll-result-container';
            				}
            				var preStatus 			= (data.contentType == 'pre-assessment') ? 1 : 0;
            				surObj.surveyId = data.contentId;
            				surObj.callTakeSurveyToClass(data.objectid ,escape(data.classTitle), data.objecttype, "assessment", paramsUpdScore, enrollId ,preStatus, resultConainer);
            				$('#take_survey_main').jScrollPane({});
            			}
            			else if(data.contentType == 'survey'){
            				var enrollId, paramsUpdScore, resultConainer = '';
            				$("#block-take-survey").surveylearner();
            				var surObj		= $("#block-take-survey").data("surveylearner");
            				contentHtml 	= result.content;
            				if (data.programId > 0) {
            					enrollId			= data.masterEnrollId;
            					paramsUpdScore 		= data.programId+"###"+data.masterEnrollId;
            					resultConainer		= 'enroll-lp-result-container';
            				} else {
            					enrollId			= data.enrollId;
            					paramsUpdScore 		= data.classId+"###"+data.enrollId+"###"+data.courseId+"###"+data.classId;
            					resultConainer		= 'enroll-result-container';
            				}
    	        			var preStatus 			=  0;
    	        			surObj.surveyId = data.contentId;
            				surObj.callTakeSurveyToClass(data.objectid, escape(data.classTitle), data.objecttype,"survey", paramsUpdScore, enrollId ,preStatus, resultConainer);
            			}
            			else{        				
    	        			content = obj.setValue(result.content, 'No Content found !');
    	        			contentHtml += '<div id="cp-contentcontainer" class="cp-contentcontainer '+ containerClass +'">';
    	        			contentHtml += '	<div id="cp-content-main">' + content + '</div>';
    	        			contentHtml += '</div>';
            			}
                		
            		}
            		if (result.isIframeContent == 1) {
            				if(data.contentType == "Knowledge Content" && data.subtype == "h5p-presentatn") //data.LearnerLaunchURL.indexOf("h5p/embed")>0 )
            				{
            					var MaxAttempt      =  data.MaxAttempt;
            	        		var AttemptLeft     =  data.AttemptLeft;
            					var contentId		= data.contentId;
				        		var contentTitle	= data.contentTitle;
				        		var contentStatus 	= data.Status;
				        		var courseId 		= data.courseId;
				        		var classId 		= data.classId;
				        		var classTitle 		= data.classTitle;
				        		var MaxAttempt      =  data.MaxAttempt;
				        		var AttemptLeft     =  data.AttemptLeft;
				        		$('#cp-menucontainer').attr("MaxAttempt",MaxAttempt);
            					$('#cp-menucontainer').attr("AttemptLeft",AttemptLeft);
            					$('#cp-menucontainer').attr("contentId",contentId);
            					$('#cp-menucontainer').attr("contentTitle",encodeURIComponent(contentTitle));
            					$('#cp-menucontainer').attr("contentStatus",contentStatus);
            					$('#cp-menucontainer').attr("courseId",courseId);
            					$('#cp-menucontainer').attr("classId",classId);
            					$('#cp-menucontainer').attr("classTitle",encodeURIComponent(classTitle));
            					$('#cp-menucontainer').attr("subtype",data.subtype);
            					
        		    			data.LearnerLaunchURL = data.LearnerLaunchURL+"&enrollId="+data.enrollId+"&preview=false";
            				}
            				$("#" + obj.options.LaunchFrom).contentLaunch();
            				$("#" + obj.options.LaunchFrom).data('contentLaunch').launchWBTContent(data);
            				if(obj.options.LaunchFrom =='learner-enrollment-tab-inner')
            					$("#" + obj.options.LaunchFrom).data("enrollment").destroyLoader('enroll-result-container');
            				if(obj.options.LaunchFrom =='learningplan-tab-inner')
            					$("#" + obj.options.LaunchFrom).data("learningplan").destroyLoader('enroll-lp-result-container');
            		}
            		if ($("#cp-contentcontainer").length > 0) {
            			if (isMultiLesson == 1) { 
            				$("#cp-contentcontainer #cp-content-main").replaceWith(contentHtml)
            			} else {
            				$("#cp-contentcontainer").replaceWith(contentHtml)
            			}
            		} else {
            			$('#cp-menucontainer').after(contentHtml);
            		}
            		if(data.contentType == "Video on Demand" && (data.subtype == "h5p-Vimeo"  || data.subtype == "h5p-Youtube" ||  data.subtype == "h5p-video-mp4" ||  data.subtype == "h5p-video-webm")) //LearnerLaunchURL.indexOf("h5p/embed/")> 0)
            		{
            			//var newurl = data.url.split("/");
						//to support tincan change url
  						//data.LearnerLaunchURL = "?q=node/"+newurl[2]+"/view";
  						obj.initiateH5P(data);
            		}
            		else if(data.contentType == 'Video on Demand'){
            			obj.initiateVideoPlayer(result.params,obj);
            		}  
            		
        		}
        		else if(data.play_status == false){
					var contentHtml='';var content = '';
					content = data.contentTitle + '&nbsp;' + '<b>'+Drupal.t('MSG796')+'</b>'; /*Viswanathan added >b> tag for #0078402*/					
					//content = 'Curenntly no content available to play !';
					contentHtml += '<div id="cp-contentcontainer" class="cp-contentcontainer cp-full-player" style="height: auto; width: 99%;">';
        			contentHtml += '	<div id="cp-content-main"><span class="msg-content">' + content + '</span></div>';
        			contentHtml += '</div>';
        			if ($("#cp-contentcontainer").length > 0) {
            			$("#cp-contentcontainer").replaceWith(contentHtml)
            		} else {
            			$('#cp-menucontainer').after(contentHtml);
            		}
           			obj.destroyLoader("cp-modalcontainer");
           		} else { 
           		var launchfromtab = obj.options.LaunchFrom;
					
					var contentHtml='';var content = '';
					content = Drupal.t('MSG796');
					
					params = "{'Id':'"+data.LessonId+"','enrollId':'"+data.enrollId+"','VersionId':'"+data.VersionId+"','url1':'"+data.LearnerLaunchURL+"'," +
			                      "'courseId':'"+data.courseId+"','classId':'"+data.classId+"','url2':'','AICC_SID':'"+data.AICC_SID+"'," +
			                      "'ErrMsg':'','contentType':'"+data.contentType+"','Status':'"+data.Status+"'}";				
					
				  data.LaunchFromTab = launchfromtab;
				    launch_data = objectToString(data);
				   //var external_link = Drupal.t('MSG838',{'@open-new-window' : 'RedirectForLoadDeniedContent("+launch_data+");'});
					contentHtml += '<div id="cp-contentcontainer" class="cp-contentcontainer cp-full-player" style="height: auto; width: 99%;">';
        		        			//	contentHtml += '	<div id="cp-content-main"><span class="msg-content">' + Drupal.t('MSG838', {'@open-new-window' : 'RedirectForLoadDeniedContent("+launch_data+");'}); + '</span></div>';
        		        		//contentHtml += '	<div id="cp-content-main"><span id = "access_denied_flag" class="msg-content">' + content + '<a class="launch_window" onclick="RedirectForLoadDeniedContent('+launch_data+');">'+Drupal.t('clickable link')+'</a></span>';
        		
        		contentHtml += '	<div id="cp-content-main"><span id = "access_denied_flag" class="msg-content">' + Drupal.t("MSG997") + '&nbsp;' +'<a class="launch_window" onclick="RedirectForLoadDeniedContent('+launch_data+');">'+Drupal.t("MSG998")+'</a>';
        		contentHtml += '<span id = "launch_msg">'+Drupal.t("MSG999")+'</span></span>';
        		//	contentHtml +=  '<a class="launch_window" onclick="RedirectForLoadDeniedContent('+launch_data+');">'+Drupal.t('MSG796ssss')+'</a>';
        			contentHtml += '</div></div>';
        			if ($("#cp-contentcontainer").length > 0) {
            			$("#cp-contentcontainer").replaceWith(contentHtml)
            		} else {
            			$('#cp-menucontainer').after(contentHtml);
            		}
        			obj.destroyLoader("cp-modalcontainer");
        		} 
				
        		if (this.options.statusBarControl) {
	        		this._generatePlayerStatusBar();
	        	}
        		      		
        		this.showNowPlay(data);        		
        		this.windowAdjustment(data);
        		this.resizeInnerContent();
        		//obj.destroyLoader("cp-modalcontainer");
            } catch(e) {
        		// console.log(e);
        	}
        },
        showNowPlay : function(data){
        	try {
        		var obj = this;
	    		var totalList = obj.options.contentList;	 
	    		var listLen = Object.keys(totalList).length;
	        	$('.conten-play-status').hide();
	        	$('.sub-content-play-status').hide();
	        	if(document.getElementById('now-play-contentId-'+data.cType+'-'+data.contentId)){
	        		setTimeout(function(){
	        	$('#cp-modalcontainer #now-play-contentId-'+data.cType+'-'+data.contentId).show();
	        		},50);
	        	}
	        	//if(document.getElementById('now-play-subcontentId-'+data.LessonId)){
	        		setTimeout(function(){
	        	$('#cp-modalcontainer #now-play-subcontentId-'+data.LessonId).show();
	        		},200);
	        	//}
	        	//	console.log("showNowPlay for UI test:" +data.cType+'-'+data.contentId);
	        	$('.cp-menulist li').removeClass('active-content');
	        	//$('#cp-modalcontainer .cp-list-menu-'+data.cType+'-'+data.contentId).addClass('active-content');
	        	if(listLen>4){
		        	var currentIndex = 1;
					if($('.cp-list-menu-'+data.cType+'-'+data.contentId).attr('jcarouselindex')>4){
						whole_number = Math.ceil($('.cp-list-menu-'+data.cType+'-'+data.contentId).attr('jcarouselindex')/4);
						currentIndex = (whole_number*4)-3;
					}
					setTimeout(function(){$('#cp-menucontainer').jcarouselcp('scroll', currentIndex, false);},200);
	        	}
	        	setTimeout(function(){$('#cp-modalcontainer .cp-list-menu-'+data.cType+'-'+data.contentId).addClass('active-content');},100);
	        	
	        	if(listLen<2){
	        		//console.log("checked play list count");
	        		$("#cp-menucontainer").addClass("cp-menulist-border");
	        	}else{
	        		$("#cp-menucontainer").removeClass("cp-menulist-border");
	        	}
	        	
        	} catch(e) {
        		// console.log(e);
        	}	
        },
        windowAdjustment : function(data){
        	var cpObj = this;
			var modalWindowContainer = $('body');
			$('#cp-modal #cp-contentcontainer').css({'height': 'auto', 'width': 'auto'});
			var modalWindow = $('#cp-modalcontainer');
			var modalWindowToDrag = $('#cp-modalcontainer');
			var modalWindowPosition = modalWindow.offset();
			var modalWindowBackDropCss = {'width': $('#modalBackdrop').css('width'), 'height': $('#modalBackdrop').css('height')};
			modalWindow.resizable({
				handles: 'all',
				// containment: 'document',
				minHeight: modalWindow.height(),
				minWidth: modalWindow.width(),
				create: function(event, ui) {
					cpObj.resizeInnerContent();
				},
				resize: function(event, ui) {
					if(document.getElementById('cp-frame-container')){
	    				$('#cp-modal #cp-frame-container .iframe-overlay').show();
					}
					cpObj.resizeInnerContent();
				},
				start: function(event, ui) {
					if(document.getElementById('cp-frame-container'))
						$('#cp-modal #cp-frame-container .iframe-overlay').show();
				},
				stop: function(event, ui) {
					$('#cp-modal #cp-frame-container .iframe-overlay').hide();
					// reset the containment when modal window is resized
					var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
					var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
					modalWindowToDrag.draggable('option', 'containment', [5, 5, draggableX2, draggableY2]);
					// resize the height/width to original if its greater than available container (i.e. body)
					var resizePosition = $('#cp-overlay .ui-icon-gripsmall-diagonal-se').position();
					modalWindowPosition = modalWindow.offset();
					if(resizePosition != null && modalWindowPosition != null) {
						if(modalWindowContainer.width() <= (resizePosition.left + modalWindowPosition.left + 5)) {
							modalWindow.css(ui.originalPosition);
							modalWindow.css(ui.originalSize);
						}
						if(modalWindowContainer.height()+$(window).scrollTop() <= (resizePosition.top + modalWindowPosition.top + 5)) {
							modalWindow.css(ui.originalPosition);
							modalWindow.css(ui.originalSize);
						}
						// not to allow modal window to have negative value for top position
						if(modalWindowPosition.top-$(window).scrollTop() < 5) {
							modalWindow.css(ui.originalPosition);
							modalWindow.css(ui.originalSize);
						}
					}
					modalWindowPosition = modalWindow.offset();
					cpObj.showNowPlay(data);
					cpObj.resizeInnerContent();
				}
			});
			
			var enableDrag = function() {
				var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
				var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
				modalWindowToDrag.draggable({
					containment: [5, 5, draggableX2, draggableY2],	//boundaries till which dragging is available
					handle: 'div.cp-toolbarcontainer',
					cursor: 'move',
					create: function(event, ui) {
						$('#cp-overlay .cp-toolbarcontainer').css('cursor', 'move');
					},
					start: function(event, ui) {
						$('#cp-overlay #cp-content-main .iframe-overlay').show();
					},
					stop: function(event, ui) {
						$('#cp-overlay #cp-content-main .iframe-overlay').hide();
						var draggableX2 = $(document).width() - modalWindowToDrag.width() - 5;
						var draggableY2 = $(document).height() - modalWindowToDrag.height() - 5;
						modalWindowToDrag.draggable('option', 'containment', [5, 5, draggableX2, draggableY2]);
						modalWindowPosition = modalWindow.offset();
						var resizePosition = $('#cp-overlay .ui-icon-gripsmall-diagonal-se').position();
						if(resizePosition != null && modalWindowPosition != null) {
							if(modalWindowContainer.width() <= (resizePosition.left + modalWindowPosition.left + 5)) {
								modalWindow.css(ui.originalPosition);
								$('#modalBackdrop').css(modalWindowBackDropCss);
							}
							if(modalWindowContainer.height() + $(window).scrollTop() <= (resizePosition.top + modalWindowPosition.top + 5)) {
								modalWindow.css(ui.originalPosition);
								$('#modalBackdrop').css(modalWindowBackDropCss);
							}console.log(modalWindowPosition);
							// not to allow modal window to have negative value for top position
							if(modalWindowPosition.top-$(window).scrollTop() < 5) {
								modalWindow.css(ui.originalPosition);
								$('#modalBackdrop').css(modalWindowBackDropCss);
							}
						}
					}
				});
			}
			enableDrag();
			var disableDrag = function() {
				modalWindowToDrag.draggable("destroy");
				$('#cp-overlay .cp-toolbarcontainer').css('cursor', 'default');
			}
			// bind callbacks for close button
			
			// Bind a click for maximizing the cp-overlay
			var maximize = function(event) {
				event.preventDefault();
				modalWindow.attr('data-css', JSON.stringify({'width': modalWindow.width(),
											  'height': modalWindow.height(),
											  'position': modalWindow.css('position'),
											  'top': modalWindow.css('top'),
											  'left': modalWindow.css('left')
											  }));
				modalWindow.attr('data-windowtop', $(window).scrollTop());
				modalWindow.addClass('fullscreen-mode')
				.css({
					'width': '100%',
					'top': '0px',
					'left': '0px',
					'bottom': '0px',
					'position': 'fixed',
					'right': '0px',
					'height': 'auto',
					'border': 'none',
					'margin': 0,
					'padding': 0,
					'z-index': 999999
					});
				// Show the minimize button; hide the maximize button when full screen mode is on
				$('#cp-overlay .cp-modal-fulscr').hide();
	
				var height_h5p = modalWindow.height() - 230;
				if($("#h5pplayer").size() > 0)
				{
					h5p_maximize(height_h5p);
				
				}
				
				
				$('#cp-overlay .cp-modal-minscr').show();
				//disable resize option
				$('.ui-resizable-handle').toggle();
				cpObj.setPlayerMode('full-mode');
				//disable drag option
				disableDrag();
				modalWindowPosition = modalWindow.offset();
				cpObj.showNowPlay(data);
				cpObj.resizeInnerContent();
			};
			$('#cp-overlay .cp-modal-fulscr').unbind();
			$('#cp-overlay .cp-modal-fulscr').bind('click', maximize);
			
			var minimize = function(event) {
				event.preventDefault();
				modalWindowPosition = modalWindow.offset();
				modalWindow.css(JSON.parse(modalWindow.attr('data-css')));
				modalWindow.removeClass('fullscreen-mode');
				$(window).scrollTop(modalWindow.attr('data-windowtop'));
				// Show the maximize button; hide the minimize button when full screen is left
				$('#cp-overlay .cp-modal-fulscr').show();
				$('#cp-overlay .cp-modal-minscr').hide();
				//enable resize option
				$('.ui-resizable-handle').toggle();
				cpObj.setPlayerMode('normal-mode');
				//enable drag option
				enableDrag();
				cpObj.showNowPlay(data);
				cpObj.resizeInnerContent();

				//h5pcustomize
				var width_cp = window.parent.$("#cp-content-main").width() ;
				var height_cp = window.parent.$("#cp-content-main").height() - 15;
				var height_cp_wrapper = height_cp - 25;			
				
				if($("#h5pplayer").size() > 0)
				{
				h5p_minimize(width_cp,height_cp,height_cp_wrapper);
				}
				
				
				
			}
			$('#cp-overlay .cp-modal-minscr').unbind();
			$('#cp-overlay .cp-modal-minscr').bind('click', minimize);
			// adjust the position of the modal popup when the browser window is minimized/maximized
			var delay = (function(){
				var timer = 0;
				return function(callback, ms){
					clearTimeout (timer);
					timer = setTimeout(callback, ms);
				};
			})();
			$(window).resize(function() {
				delay(function(){
					modalWindow.offset(modalWindowPosition);
				}, 500);
			});
			// ui-widget-overlay of select class and multiple content launch dialogs have z-index higher than the modal content popup
			if(document.getElementById('cp-overlay') !== undefined && document.getElementById('cp-overlay') != null) {
				$('#modalBackdrop').css('z-index', $('#cp-overlay').zIndex() - 1);
			}
        },
        resizeInnerContent: function(){
        	try {
        	
        	var toolbarWidth 		= parseInt($("#cp-toolbarcontainer").height());
        	var menuWidth 		= parseInt($("#cp-menucontainer").height());
        	var satusBarWidth 	= parseInt($("#cp-statusbarcontainer").height());
        	var reduceWidth;
            if ($("#cp-menucontainer").attr('alt') == 'show') {
            		reduceWidth = 179;
        	} else {
            		reduceWidth = 79;
        	}
           	var modalWindow = $('#cp-modalcontainer');
        	if(document.getElementById('cp-frame-container')) {
        	
				$('#cp-overlay #cp-frame-container').height(modalWindow.height() - reduceWidth);
				  $('#cp-overlay #cp-content-navigation').height(modalWindow.height() - reduceWidth);
				  $('#cp-overlay #cp-content-navigation').css('max-height',modalWindow.height() - reduceWidth);
				  $('#cp-overlay #cp-content-navigation').css('overflow','hidden');
				  $('#cp-overlay #cp-content-navigation').jScrollPane();
				if ($('.cp-content-navigation-control').length > 0) { // Adjust navigation control position
					var leftWidth = (parseInt($('.cp-content-navigation-control').css('margin-left')) == 0) ? 0 : $('#cp-content-navigation-container').width() - 18;
					$('.cp-content-navigation-control').css('margin-left', leftWidth);
				}	
				//for h5p presentation
				var height_cp = window.parent.$("#cp-content-main").height() - 15;
				$("#cp-frame-container").contents().find(".h5p-wrapper").css("height",height_cp);
				$("#cp-frame-container").contents().find(".h5p-wrapper").css("max-height",height_cp);

        	} else if(document.getElementById('selSurveyContainer')){
    				var heightofcontent = modalWindow.height()-reduceWidth;
				$('#selSurveyContainer').css('height', heightofcontent+'px');
    				$('#selSurveyContainer').jScrollPane({autoReinitialise: true});
				$('.SurveyContent .SurveyHLeftContent').addClass('jspScrollable');
				$('.SurveyContent .SurveyHLeftContent').jScrollPane({});
				var widthOfContent = modalWindow.width();
				$('.SurveyHLeftContent').css('width', widthOfContent+'px');
				$('.SurveyHLeftContent').jScrollPane({autoReinitialise: true});
			} else if (document.getElementById('video-container')) {
			
				$('#cp-content-main #video-container').height(modalWindow.height() - reduceWidth);
				$.cookie('vdo_hgt', $('#video-container').height());
				$.cookie('vdo_wdt', $('#video-container').width());
				if (vodPlayer != undefined)
					resizeVideoJS(vodPlayer);
			} else {
			
				$('#cp-overlay #cp-content-main').height(modalWindow.height() - reduceWidth);
				
				if ($("#cp-menucontainer").attr('alt') == 'show') 
				 {
				
				var width_cp = window.parent.$("#cp-content-main").width() ;
				var height_cp = window.parent.$("#cp-content-main").height() - 15;
				var height_cp_wrapper = height_cp - 25;			
				if($("#h5pplayer").size() > 0)
				{
					h5p_minimize(width_cp,height_cp,height_cp_wrapper);
					
				}
				
				}
				else
				{
				
				var height_h5p = "";
				height_h5p = window.parent.$("#cp-content-main").height() - 40;  
				if($("#h5pplayer").size() > 0)
				 {
					h5p_maximize(height_h5p);
					
				 }	
			    }
				

			}
        	$("#cp-menucontainer .cp-menulist li").removeClass('last');
        	} catch(e) {
        		// console.log(e);
        	}
        	
        	
        },
        initiateH5P:function(data)
        {
        	try {
        		
        		if(data.LearnerLaunchURL.indexOf("http") >=0 || data.LearnerLaunchURL.indexOf("https")>=0)
        		{
        			var url = data.LearnerLaunchURL.split("///");
        			data.LearnerLaunchURL = url[1];
        		}
        		var url = data.LearnerLaunchURL+"&enrollId="+data.enrollId+"&preview=false";
        	
        		var width_cp = $("#cp-content-main").width();
        		var height_cp = $("#cp-content-main").height();
        		var contentId		= data.contentId;
        		var contentTitle	= data.contentTitle;
        		var contentStatus 	= data.Status;
        		var courseId 		= data.courseId;
        		var classId 		= data.classId;
        		var classTitle 		= data.classTitle;
        		var MaxAttempt      =  data.MaxAttempt;
        		var AttemptLeft     =  data.AttemptLeft;
        		
        		
        		$("#cp-content-main").html("<iframe id='h5pplayer' subtype='"+data.subtype+"' contentId='"+contentId+"' MaxAttempt='"+MaxAttempt+"' AttemptLeft='"+AttemptLeft+"'  contentTitle='"+encodeURIComponent(contentTitle)+"' contentStatus='"+contentStatus+"' courseId='"+courseId+"' classId='"+classId+"' classTitle='"+encodeURIComponent(classTitle)+"' border='0' frameBorder='0' src='"+url+"' height='100%' style='width:"+width_cp+"px;height:"+height_cp+"px;'> </iframe>");
        		createLoaderNew("cp-content-main");
        	}catch(e){
        	}
        },
        initiateVideoPlayer : function(data,obj) {
        	try {
        		seekForFirstTime = false;
				if(videojs !== undefined && data.video_session_id !== undefined) {
					disposeVideoJSPlayer(data.video_session_id);
					videojs(data.video_session_id, {}, function(){
					  // Player (this) is initialized and ready.
					  vodPlayer = this;
					  
					  if ($('#video-container').length > 0) {
						  vodPlayer.height($('#video-container').height());
						  vodPlayer.width($('#video-container').width());
					  }

					  vodPlayer.on('mouseout', function(){ 
						  this.controls(false);
						});

						vodPlayer.on('mouseover', function(){
						  this.controls(true);
						});
					  if(data.play_from !== undefined) {
					    vodPlayer.currentTime(data.play_from);
					    vodPlayer.play();
						seekForFirstTime = true;
					  }
					  vodPlayer.on('loadedmetadata', function() {
						if(seekForFirstTime && data.play_from > this.currentTime()) {
						  this.currentTime(data.play_from);
						  seekForFirstTime = false;
						}
					  });
					  vodPlayer.on('durationchange', function() {
						this.controlBar.progressControl.seekBar.loadProgressBar.update();
					  });
					  $('#loaderdivmodal-content').remove();
					  //73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
	  				  updateStatusToDB = setInterval(function(){updateVideoProgressToDB(obj);},Drupal.settings.content, vodPlayer);
					  ajaxInterval = vodPlayer.setInterval(function() {
					   updateVideoProgress(vodPlayer);
				  }, 1000, vodPlayer);
			      
				  var updateFreq = (data.update_frequency !== undefined ? (data.update_frequency * 1000) : 10000);
				  saveInterval = setInterval(function() {
				    saveVideoProgress(vodPlayer, false);
				  }, updateFreq, vodPlayer);
			      
				  vodPlayer.on('ended', function(){
				    vodPlayer = this;
					if(vodPlayer.techName_ == 'Youtube') {
						vodPlayer.pause();
						vodPlayer.play();
					}
				    var progress	= 100; // when player cursor moved at the end.
				    videoTrackerProgress = {
				      session_id: data.video_session_id,
				      current_position: this.currentTime(),
				      progress: progress,
				      video_duration: this.duration(),
				      additional_data: {video_session_id: data.video_session_id}
				    };
				    saveVideoProgress(vodPlayer, true);
				    updateVideoProgressToDB(obj);
        			clearInterval(updateStatusToDB);
				    clearInterval(saveInterval);
				    clearInterval(ajaxInterval);
				  });
				});
				}
        	} catch(e) {
        		// console.log(e);
        	}
        },
        _generatePlayerContentMenu :function(data, replaceVal) {
        	try {
        	
        	// Build the submenu
        		var obj = this;
        		var cpObj = "$(\'#"+ obj.options.LaunchFrom +"\').data(\'contentPlayer\')";
        		var subMenuHtml = '';
        		var defaultContent = obj.defaultContent[0];
        		var subMenuList 	= obj.options.contentList[defaultContent];
			subMenuHtml += '<ul id="cp-content-menulist">';
    		$.each(subMenuList, function (i, list) {
				if (typeof list == 'object' && list != null) {
					var contentId 			= subMenuList[i]['LessonId'];
					var playStatus			= subMenuList[i]['play_status'];
					var launchData = {
                			masterEnrollId	 		: obj.setValue(data.master_enroll_id, 0),
                			enrollId 				: obj.setValue(data.enrollId),
                			//classTitle 				: obj.setValue(data.classTitle),
            				contentType 			: subMenuList[i]['contentType'],
            				contentTitle			: subMenuList[i]['contentTitle'],
            				contentId 				: subMenuList[i]['contentId'],
            				cType 					: subMenuList[i]['cType'],
            				LessonId				: subMenuList[i]['LessonId'],
            				//subContentId			: obj.setValue(data.sub_content_id),
            				
            				
            				VersionId				: obj.setValue(subMenuList[i]['VersionId'], ''),
            				url1					: obj.setValue(subMenuList[i]['LearnerLaunchURL'], ''),		
            				courseId				: obj.setValue(data.courseId, 0),
            				classId					: obj.setValue(data.classId, 0),	
            				url2					: obj.setValue(subMenuList[i]['PresenterLaunchURL'], ''),
            				ErrMsg					: '', // always empty
            				Status					: obj.setValue(subMenuList[i]['Status'], ''),
            				LessonLocation			: obj.setValue(subMenuList[i]['LessonLocation'], ''),
            				launch_data				: obj.setValue(subMenuList[i]['LaunchData'], ''),
            				suspend_data			: obj.setValue(subMenuList[i]['SuspendData'], ''),
            				exit					: obj.setValue(subMenuList[i]['CmiExit'], ''),
            				AICC_SID				: obj.setValue(subMenuList[i]['AICC_SID'], ''),
            				MasteryScore			: obj.setValue(subMenuList[i]['masteryscore'], ''),
            				remDays					: subMenuList[i]['remDays'],
            				ValidityDays			: subMenuList[i]['ValidityDays'],
            				no_of_attempts			: subMenuList[i]['no_of_attempts'],
            				AttemptLeft				: subMenuList[i]['AttemptLeft'],
            				attempts				: subMenuList[i]['attempts'],
            				MaxAttempt				: subMenuList[i]['MaxAttempt'],
            				surveycount				: subMenuList[i]['surveycount'],
            				is_post_ass_launch		: subMenuList[i]['is_post_ass_launch'],
            				playStatus				: subMenuList[i]['play_status'],
            				defaultContent			: defaultContent+'-'+i,
            				is_multi_lesson			: 1,
            				LaunchFrom				: data.LaunchFrom,
            				
	    	        	}
					launchData = objectToString(launchData);
	    			subMenuHtml += '<li>';
	    			subMenuHtml += '	<div class="cp-content-menu-details">';
	    			if(playStatus == true){
	    				subMenuHtml += '		<a id="selsubconetentmenulink-'+subMenuList[i]['cType']+'-'+contentId+'" href="javascript:void(0);" onclick="'+cpObj+'.changePlayerContent('+ launchData +');\">'+ decodeURIComponent(unescape(list['contentTitle'])) +'	</a>';
	    			}else{
	    				subMenuHtml += '		<a id="selsubconetentmenulink-'+subMenuList[i]['cType']+'-'+contentId+'" href="javascript:void(0);" onclick="'+cpObj+'.changePlayerContent('+ launchData +');\">'+ decodeURIComponent(unescape(list['contentTitle'])) +'	</a>';
	    			}
	    			subMenuHtml += '		<div class="sub-content-play-status current-status-disable" id="now-play-subcontentId-'+contentId+'" style="display:none"><span>'+Drupal.t('Now')+'</span><span> '+Drupal.t('Playing')+'</span></div>';
	    			subMenuHtml += '	</div>';        		
	    			
	    			
	    			if(subMenuList[i]['contentType'] == "pre-assessment" || subMenuList[i]['contentType'] == "survey" || subMenuList[i]['contentType'] == "post-assessment"){
        				var content_id = subMenuList[i]['surveyid'];
        				var LessonId = subMenuList[i]['LessonId'];
        				var type = 'survey_assessment';
        			}else{
        				var content_id = subMenuList[i]['contentId'];
        				var LessonId = subMenuList[i]['LessonId'];
        				var type = 'content';
        			}
	    			obj.progressbarLine(data.enrollId,subMenuList[i]['content_completion'],content_id,'launch',type,LessonId,1);        				
	    			subMenuHtml += '<div id="cp-menu-progress_'+data.enrollId+'_'+content_id+'_'+LessonId+'_'+type+'" class="cp-menu-progress">';  
	    			subMenuHtml += '</div>';
	    			subMenuHtml += '</li>';
				}
    			
    		});
    		subMenuHtml += '</ul>';
    		if ( replaceVal == 1) {
    			 $("#cp-content-navigation-container").html(subMenuHtml);
    		} else {
    			return subMenuHtml;
    		}
    		
        	} catch(e) {
        		// console.log(e);
        	}
        },
        
        _refreshContentPlayer: function() {
        	try {
        		// only refresh the menu and content
	        	this._generatePlayerMenu();
	        	this._generatePlayerContent();
	        } catch(e) {
        		// console.log(e);
        	}
        },
        
        playContent: function(data) {
        	try {
        		$("div").remove("#cp-overlay");
        		var obj = this;
        		obj._generateOverlayContent();
        		obj.showModal(); 					// show the modal to user end.
        		
        		obj.getPlayList(data); 	
        		
        		
        	
        				// prepare menu list and show the menu
        		setTimeout(function(){
        			if(data.defaultContent==''){
            			data.defaultContent = obj.classDetails['defaultLaunchContent']+'';
        			}
             		data.defaultContent = (data.defaultContent != '' || data.defaultContent != null) ? data.defaultContent : 0; // set default content as first content
             		if(obj.classDetails['lp_have_only_post_ass_cont']!=undefined)
             			data.lp_have_only_post_ass_cont = obj.classDetails['lp_have_only_post_ass_cont'];
             		if(obj.classDetails['cl_ilt_have_only_post_ass_cont']!=undefined)
             			data.cl_ilt_have_only_post_ass_cont = obj.classDetails['cl_ilt_have_only_post_ass_cont'];
        			obj.getPlayerContent(data); 		// prepare default content to play.
        		}, 2000);	
        	} catch(e) {
        		// console.log(e);
        	}
        },
        playContentMylearning: function(data) {
        	try {
        		var obj = this; 
        		if(data.pagefrom=='lp_class_seemore'){
        			//course-detail-section-7
        			this.createLoader('enroll-lp-result-container');
	        		$('#subgrid-class_menu_detail_'+data.enrollId).css('display', 'table-row');
	        		$('#lp_class_seemore_'+data.enrollId).css('display', 'none');
	        		$("#lp-cl-"+data.enrollId+" #classShortDesc_"+data.enrollId).find("#arrow-more").click();
                                $("#lp-cl-"+data.enrollId+" #classShortDesc_"+data.enrollId).find("#arrow-less").css('display','none');
	        		if(document.getElementById("cp-menulist-seemore-"+data.enrollId) || document.getElementById("sel-cp-menulist-nomenu-"+data.enrollId)) {
	        			$("#paindContentResults_"+data.enrollId).css("display","block");
	        			this.destroyLoader('enroll-lp-result-container');
	        		}else{
	        			obj.getPlayList(data); 			// prepate menu list and show the menu 
	        		}
	        		$(".lp_class_seeless" ).each(function() {
	        			var selid = $(this).attr('id').split('_');
	        			if(document.getElementById("cp-menulist-seemore-"+data.enrollId))
	        				$('#lp_class_seemore_'+selid[1]).css('display', 'none');
	        		});
	        		if(document.getElementById(data.enrollId+"SubGrid")){
	        			setTimeout(function(){$("#"+data.enrollId+"SubGrid").show(300);},100);
	        		}
	        		else
	        			setTimeout(function(){$('#lp-class-accodion-'+data.enrollId).click();}, 200);
	        		$('#lp_class_seeless_'+data.enrollId).css('display','block');
	        		$('#lp_class_seemore_'+data.enrollId).css('display','none');
	        		data.defaultContent = (data.defaultContent != '' || data.defaultContent != null) ? data.defaultContent : 0; // set default content as first content
        		} else if (data.pagefrom=='lp_seemore') {
        			var containerId = data.containerId;
        			this.createLoader('enroll-lp-result-container');
        			//$('.clsSeeMorePlaceholderdiv').css('display','none');	
	        		//$('.clsDetailsDiv').css('display','none');
	        		//$('.clsDetailsDivWithSeemore').css('display','none');
	        		
	        		$('#lp_seemore_'+containerId).css('display','none');
        			
        			//$('#lp_seemore_'+containerId).css('display','none');
	        		//console.log("#"+containerId+" #LPShortDesc_"+containerId);
	        		$("#"+containerId+" #LPShortDesc_"+containerId).find("#arrow-more").click();
                                $("#"+containerId+" #LPShortDesc_"+containerId).find("#arrow-less").css('display','none');
	        		if(document.getElementById("cp-menulist-seemore-"+containerId) || document.getElementById("sel-cp-menulist-nomenu-"+containerId)) {
	        			$("#paindLPContentResults_"+containerId).css("display","block");
	        		}else{
	        			obj.getPlayList(data); 			// prepate menu list and show the menu 
	        		}
	        		
	        		$(".lp_cp_seeless" ).each(function() {
	        			var selid = $(this).attr('id').split('_');
	        			if(document.getElementById("cp-menulist-seemore-"+containerId))
	        				$('#lp_seemore_'+selid[1]).css('display', 'none');
	        		});
	        		
	        		if(document.getElementById(containerId+"SubGrid")){
	        			setTimeout(function(){$("#"+containerId+"SubGrid").show(300);},100);
	        			this.destroyLoader('enroll-lp-result-container');
	        		}
	        		else {
	        			
	        			setTimeout(function(){$('#prg-accodion-'+containerId).click();}, 100);
	        		}
	        		$('#lp_seeless_tr_' + containerId).css('display','table-row');
	        		//$('#lp_seemore_' + containerId).css('display','none');
	        		data.defaultContent = (data.defaultContent != '' || data.defaultContent != null) ? data.defaultContent : 0; // set default content as first content
    	        		
        		 }else if(data.pagefrom == 'class_details'){
     	        	$('.class-content-wrapper').show();
     	        	$('.content-details-warpper').show();
     	        	var alt_enroll_id = $('#paindContentclsid_'+data.classId).attr('alt');
     	        	data.enrollId = (data.enrollId=='')? alt_enroll_id : data.enrollId;
     	        	if(alt_enroll_id != data.enrollId)
     	        		data.enrollId = alt_enroll_id;
     	        	obj.getPlayList(data);
     	        }else if(data.pagefrom=='course_class_list' || data.pagefrom =='lrnplan_course_class_list'){
     	        	$('#class_content_moredetail_'+data.classId+' .class-content-wrapper').show();
     	        	$('#class_content_moredetail_'+data.classId+' .content-details-warpper').show();
     	        	var alt_enroll_id = $('#paindContentclsid_'+data.classId).attr('alt');
     	        	data.enrollId = (data.enrollId=='')? alt_enroll_id : data.enrollId;
     	        	if(alt_enroll_id != data.enrollId)
     	        		data.enrollId = alt_enroll_id;
     	        	obj.getPlayList(data);
     	        } else{
    	        	this.createLoader('enroll_details_'+data.enrollId);
	        		//$('.clsSeeMorePlaceholderdiv').css('display','none');	
	        		$('.clsDetailsDiv').css('display','none');
	        		//$('.clsDetailsDivWithSeemore').css('display','none');
	        		$('#seemore_'+data.enrollId).css('display','none');
	        		$("#"+data.enrollId+" #classShortDesc_"+data.enrollId).find("#arrow-more").click();
                                $("#"+data.enrollId+" #classShortDesc_"+data.enrollId).find("#arrow-less").css('display', 'none');
	        		if(document.getElementById("cp-menulist-seemore-"+data.enrollId) || document.getElementById("sel-cp-menulist-nomenu-"+data.enrollId)) {
	        			$("#paindContentResults_"+data.enrollId).css("display","block");
	        			obj.destroyLoader('enroll_details_'+data.enrollId);
	        			obj.aftercompletetrunk8();
	        		}else{
	        			obj.getPlayList(data); 			// prepate menu list and show the menu 
	        		}
//	        		$(".cp_seeless" ).each(function() {
//	        			var selid = $(this).attr('id').split('_');
//	        			if(document.getElementById("cp-menulist-seemore-"+data.enrollId))
//	        				$('#seemore_'+selid[1]).css('display', 'none');
//	        		});
	        		if(document.getElementById(data.enrollId+"SubGrid")){
	        			$("#"+data.enrollId+"SubGrid").show();
	        		}
	        		else
	        			$('#class-accodion-'+data.enrollId).click();
	        		$('#seeless_'+data.enrollId).css('display','block');
	        		$('#seemore_'+data.enrollId).css('display','none');
	        		data.defaultContent = (data.defaultContent != '' || data.defaultContent != null) ? data.defaultContent : 0; // set default content as first content
        		}
//        		setTimeout(function(){
//        			//obj.getPlayerContent(data); 		// prepare default content to play.
//        		}, 500);	
			if(data.pagefrom == 'seemore')
             	resetFadeOutForAttributes('#'+data.enrollId+'SubGrid','main-item-container','line-item-container','container','myenrollment');
          
            } catch(e) {
        		// console.log(e);
        	}
        	vtip();
        },
        destroy: function () {

        },

        closeModal : function(){
        	try {
        		var obj = this;
        		var objData = obj.lastPlayedObject;
        		if(objData.LaunchType=='VOD'){
        			saveInterval = (typeof saveInterval == 'undefined' ? null : saveInterval);
					ajaxInterval = (typeof ajaxInterval == 'undefined' ? null : ajaxInterval);
					updateStatusToDB = (typeof updateStatusToDB == 'undefined' ? null : updateStatusToDB);
        			clearInterval(updateStatusToDB);
        		 	clearInterval(saveInterval);
				 	clearInterval(ajaxInterval);
				}
				if(obj.defaultContent=='notset' || obj.classDetails.cls_comp_status=='lrn_crs_cmp_inc' || objData.access_denied_flag==1){

					if(obj.classDetails.cls_comp_status=='lrn_crs_cmp_inc' || objData.access_denied_flag==1){
						if(document.getElementById('learningplan-tab-inner')) {
							 if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentLPResults') == false) {
								 $("#paintEnrollmentLPResults").trigger("reloadGrid");
							 }
						}
						if(document.getElementById('learner-enrollment-tab-inner')) {
							$("#paintEnrollmentResults").setGridParam({postData: {'enrId': obj.lastPlayedObject.enrollId}});
							if(typeof $("body").data("learningcore") == 'undefined' || $("body").data("learningcore").refreshLastAccessedLearningRow('#paintEnrollmentResults') == false) {
								$("#paintEnrollmentResults").trigger("reloadGrid");
							}
						}
						if(document.getElementById('lnr-catalog-search')){
							if(document.getElementById('paintContentResults')) {
								if(typeof $("#lnr-catalog-search").data("lnrcatalogsearch") == 'undefined' || $("#lnr-catalog-search").data("lnrcatalogsearch").refreshLastAccessedCatalogRow() == false) {
									$("#paintContentResults").trigger("reloadGrid");
								}
							}
							else if(document.getElementById("tbl-paintCatalogContentResults"))
								$("#tbl-paintCatalogContentResults").trigger("reloadGrid");
							else
								location.reload();
					    }
					}
					$("#cp-overlay-parent").css( 'display', 'none');
					$("#cp-overlay").css( 'display', 'none');
					$("#cp-modal").css( 'display', 'none');
				}else{
					$("#cp-overlay-parent").css( 'display', 'none');
					$("#cp-overlay").css( 'display', 'none');
					$("#cp-modal").css( 'display', 'none');
					if (typeof objData == 'object' && objData != null ) {
						contentPlayer.closed = true;
						data = {
							closed : true,
							access_denied_flag : objData.access_denied_flag,
						};
						obj.updatePlayedStatus(data)
					}
					//close content player full screen and resize related work
					if ($('#cp-modal').hasClass('fullscreen-mode')) { // unset full screen, when player in full mode
						$('#cp-modal').removeClass('fullscreen-mode');
						$('#cp-modal').css(JSON.parse($('#cp-modal').attr('data-css')));
						//$('#cp-modalcontainer').css(JSON.parse($('#cp-modal').attr('data-css')));
					}
					$('#cp-overlay .cp-modal-fulscr').show();
	    			$('#cp-overlay .cp-modal-minscr').hide();
	    			$('#cp-modalcontainer').removeAttr("data-windowtop");    			
	    			$('#cp-modalcontainer').removeAttr("data-css");
	    			$('#cp-modalcontainer').removeAttr("style");

	    			$.cookie("vdo_hgt", null, { path: '/' });
	    			$.cookie("vdo_wdt", null, { path: '/' });
	    		
				}
				$('#cp-menu-launcher').die("click");
				$('.cp-modal-close').die("click");
				/*if(document.getElementById('learner-enrollment-tab-inner')) {
					$("#learner-enrollment-tab-inner").contentPlayer("destroy");
				}
				if(document.getElementById('learningplan-tab-inner')) {
					$("#learningplan-tab-inner").contentPlayer("destroy");
				}
				if(document.getElementById('lnr-catalog-search')) {
					$("#lnr-catalog-search").contentPlayer("destroy");
				}*/
				// remove the content from div for #0074507
				$("#selSurveyContainer").html('');
        	} catch(e) {
        		// console.log(e);
        	}
		},
		 launchsurvey : function(data){
			 try{
			var obj = this;        	
			var enrollId = data.enrollId  ;      
			var userId = data.userId  ; 
			var classId = data.classId  ; 
			var courseId = data.courseId  ; 
			var masterEnrollId = data.masterEnrollId;     
			var classTitle = data.classTitle;
			var programId = data.programId ;
			var params = {};
			if (data.masterEnrollId > 0) {
				var url = this.constructUrl('ajax/content-player/program_survey_list/' + masterEnrollId+'/'+userId+'/'+programId); 
			} else {
				var url = this.constructUrl('ajax/content-player/survey_list/' + enrollId +'/'+userId+'/'+classId);
			}
			$.ajax({
				type: "POST",
				url: url,
				data:  params,
				success: function(result){
				var contentdefault = result.defaultcontent.toString();				
				if(data.masterEnrollId > 0)
					$('#learner-enrollment-tab-inner').data('contentPlayer').playContent({'masterEnrollId':data.masterEnrollId,'enrollId':'0','programId':result.programId,'isMultiLesson':'0','contentType':'survey','cType':'survey','contentId':result.contentid,'LessonId':'','VersionId':'','url1':'','courseId':courseId,'classId':classId,'url2':'','ErrMsg':'','Status':'','LessonLocation':'','launch_data':'','suspend_data':'','exit':'','AICC_SID':'','MasteryScore':'','remDays':'undefined','ValidityDays':'undefined','no_of_attempts':'null','AttemptLeft':'undefined','attempts':'null','MaxAttempt':'undefined','surveycount':'0','is_post_ass_launch':'undefined','playStatus':'true','defaultContent':contentdefault,'pagefrom':'','LaunchFrom':'ME',});
				else
				    $('#learner-enrollment-tab-inner').data('contentPlayer').playContent({'masterEnrollId':'0','enrollId':enrollId,'programId':'0','isMultiLesson':'0','contentType':'survey','cType':'survey','contentId':result.contentid,'LessonId':'','VersionId':'','url1':'','courseId':courseId,'classId':classId,'url2':'','ErrMsg':'','Status':'','LessonLocation':'','launch_data':'','suspend_data':'','exit':'','AICC_SID':'','MasteryScore':'','remDays':'undefined','ValidityDays':'undefined','no_of_attempts':'null','AttemptLeft':'undefined','attempts':'null','MaxAttempt':'undefined','surveycount':'0','is_post_ass_launch':'undefined','playStatus':'true','defaultContent':contentdefault,'pagefrom':'','LaunchFrom':'ME',});
		
				}
					
		    	});
			 }catch(e) {
	     		// console.log(e);
	     	}
		},
		showModal : function(){
			try {
				//console.log('show called');
				$("#cp-overlay-parent").css('display', 'block');
				$("#cp-overlay").css('display', 'block');
				$("#cp-modal").css('display', 'block');
				var winWidth = window.innerWidth;
				var modalWidth = $("#cp-modal").width();
				var leftWidth = Math.round(( winWidth - modalWidth ) / 2);
				$("#cp-modal").css({'display':'block', 'left' : leftWidth + 'px'});
				$("#cp-menucontainer").css('display', 'block');
				$("#cp-content-main").html('');
			} catch(e) {
        		// console.log(e);
        	}
		},
		
		showMenu: function() {
			try {
				var obj = this;
				var element = $("#cp-menucontainer");
				var launchElement = $('#cp-menu-launcher');
				if (element.attr('alt') == 'show')
					element.attr('alt','hide');
				else
					element.attr('alt','show');
				if (element.css('display') == 'none') {
					element.toggle("fast","swing",obj.resizeInnerContent());
					launchElement.css({'top':'140px', 'left' : '47%'});
					//.css('display', 'block');
					$("#cp-menu-launcher-icon").removeClass('cp-menu-launcher-pop');
				} else {
					element.toggle("fast","swing",obj.resizeInnerContent());
					launchElement.css({'top':'40px', 'left' : '47%'});
					//.css('display', 'none');
					$("#cp-menu-launcher-icon").addClass('cp-menu-launcher-pop');
				}
				//this.resizeInnerContent();
			} catch(e) {
        		// console.log(e);
        	}
		},
		showContentNavMenu: function() {
			try {
				if ($("#cp-content-navigation").css('display') == 'none') {
					var navWidth = $("#cp-content-navigation").width() - 18;
					$('.cp-content-navigation-control').css('margin-left', navWidth + 'px').html('<').removeClass('exp-arrow');
					$("#cp-content-navigation").show();
					$("#cp-content-main").width('80%')
				} else {
					$('.cp-content-navigation-control').css('margin-left', '0').html('>').addClass('exp-arrow');
					$("#cp-content-navigation").hide();
					$("#cp-content-main").width('100%')	
				}

			} catch(e) {
        		// console.log(e);
        	}
		},
		
		setLastContentSeparator: function(contextId,limit){
			try{
				var leftWidth = Math.abs(parseInt($(contextId).css('left')));
				var divisor = Math.abs(parseInt($(contextId+' li div.cp-menuitem').css('width')));
				$(contextId+" li").removeClass('last');
				var numItem = 0; 
				if(leftWidth == 0){
					numItem =0;
				}
				else{
					numItem = leftWidth/divisor;
				}
				lastItem =  Math.round(numItem+limit);
				$(contextId+" li::nth-child(" + lastItem + ")").addClass('last');
			} catch(e) {
        		// console.log('inside set last content separator::'+e);
        	}
				
		},
		
		attachMenuCarousel: function(total, pagefrom, enrollid) {
			try {
			var obj = this;
			var car_limit = 4;
			var last_new = total;
			if(pagefrom =='class_details')
				var car_limit = 3;
			if(pagefrom == 'course_class_list' || pagefrom =='lrnplan_course_class_list')
				var car_limit = 2;
			if(pagefrom == "seemore" || pagefrom == "lp_seemore" || pagefrom == "lp_class_seemore" || pagefrom =='class_details' || pagefrom == 'course_class_list' || pagefrom =='lrnplan_course_class_list'){
				if (pagefrom == "lp_seemore") {
					var curObj = $('#paindLPContentResults_'+enrollid).jcarouselcp({
						scroll:car_limit,
						itemLoadCallback: function(carousel, state){
							if(parseInt(last_new) < parseInt(car_limit))
								carousel.last = parseInt(last_new);
							obj.setLastContentSeparator('#paindLPContentResults_'+enrollid+' .cp-menulist',car_limit);
					    },
					    
					});
				} else {
					var curObj = $('#paindContentResults_'+enrollid).jcarouselcp({
						scroll:car_limit,
						itemLoadCallback: function(carousel, state){
							if(parseInt(last_new) < parseInt(car_limit))
								carousel.last = parseInt(last_new);
							if (pagefrom == "lp_class_seemore") { 
								obj.setLastContentSeparator('#paindContentResults_'+enrollid +' .cp-menulist',car_limit);
							} else {
								obj.setLastContentSeparator('#paindContentResults_'+enrollid +' .cp-menulist',car_limit);
							}
						},
					    
					});
				}
				if(pagefrom =='class_details' || pagefrom == 'course_class_list' || pagefrom =='lrnplan_course_class_list'){
					$('.paindContentResults .jcarousel-prev-horizontal').show();
					$('.paindContentResults .jcarousel-next-horizontal').show();
				}
				if(pagefrom == "seemore"){
					$('#enroll-result-container .jcarousel-prev-horizontal').show();
					$('#enroll-result-container .jcarousel-next-horizontal').show();
				} else if(pagefrom == "lp_seemore"){
					$('#enroll-lp-result-container .jcarousel-prev-horizontal').show();
					$('#enroll-lp-result-container .jcarousel-next-horizontal').show();
				} else if(pagefrom == "lp_class_seemore"){
					$('#enroll-lp-result-container .jcarousel-prev-horizontal').show();
					$('#enroll-lp-result-container .jcarousel-next-horizontal').show();
				}
				
				
//				$('#paindContentResults_'+enrollid).hover(function(){				
//					$('#enroll-result-container .jcarousel-prev-horizontal').show();
//					$('#enroll-result-container .jcarousel-next-horizontal').show();
//				});
//				$('#paindContentResults_'+enrollid).mouseleave(function(){
//					$('#enroll-result-container .jcarousel-prev-horizontal').hide();
//					$('#enroll-result-container .jcarousel-next-horizontal').hide()
//				});
			}else{
				var curObj =$('#cp-menucontainer').jcarouselcp({
					itemLoadCallback: function(carousel, state){
						if(parseInt(last_new) < parseInt(car_limit))
							carousel.last = parseInt(last_new);
						
						var playerMode = obj.options.playerMode;
						if (playerMode == 'normal-mode') {
							obj.setLastContentSeparator('#cp-menucontainer .cp-menulist',car_limit);
						} else {
							$("#cp-menucontainer .cp-menulist li").removeClass('last');
						}	
				    }
				});
				$('#cp-menucontainer .jcarousel-prev-horizontal').show();
				$('#cp-menucontainer .jcarousel-next-horizontal').show();
//				$('#cp-menucontainer').live("mouseover",function(){
//					$('#cp-menucontainer .jcarousel-prev-horizontal').show();
//					$('#cp-menucontainer .jcarousel-next-horizontal').show();
//				});
//				$('#cp-menucontainer').live("mouseout",function(){
//					$('#cp-menucontainer .jcarousel-prev-horizontal').hide();
//					$('#cp-menucontainer .jcarousel-next-horizontal').hide()
//				});
				}
			} catch(e) {
        		// console.log(e);
        	}
		},
		
		showCarouselNavigation: function() {
			try {
				// console.log('showCarouselNavigation called');
				
			} catch(e) {
        		// console.log(e);
        	}
		},
		
		showFullScreen: function(type) {
			try {
				$("#cp-contentcontainer").fullScreen({
					callback: function(){
						this.removeClass('fullscreen');
					}
				});
				setTimeout(function(){ $('#cp-frame-container').css('position','fixed'); }, 500);
			} catch(e) {
        		// console.log(e);
        	}
		},
		
		setValue: function(value, dafault_value) {
			var dafault_value = (dafault_value != '' || dafault_value != undefined) ?  dafault_value : 0; // default value is always 0
			return (value != undefined && value != '')   ? value : dafault_value;
		},
		seelesscontent :function(enroll_id){
			try{
				$('#paindContentResults_'+enroll_id).hide(300);
				$('#'+enroll_id+'SubGrid').hide(300);
				$('#'+enroll_id+' #classShortDesc_'+enroll_id).find('#arrow-less').click();
				$('#seeless_'+enroll_id).css('display', 'none');
				$('#seemore_'+enroll_id).css('display', 'block');
			}catch(e) {
        		// console.log(e);
        	}
		},
		seelessLPContent :function(enroll_id){
			try{
				$('#paindLPContentResults_'+enroll_id).hide(300);
				$('#'+enroll_id+'SubGrid').hide(300);
				$('#'+enroll_id+' #LPShortDesc_'+enroll_id).find('#arrow-less').click();
				//$('#'+enroll_id+'ModuleGrid').hide(300);
				$('#lp_seeless_tr_'+enroll_id).css('display', 'none');
				$('#lp_seemore_'+enroll_id).css('display', 'block');
			}catch(e) {
        		// console.log(e);
        	}
		},
		lpclassseelesscontent :function(enroll_id){
			try{
				$("#compLpDetails"+enroll_id).hide();
				$('#paindContentResults_'+enroll_id).hide();
				$('#'+enroll_id+'ClassSubGrid').hide();
				$('#lp-cl-'+enroll_id+' #classShortDesc_'+enroll_id).find('#arrow-less').click();
				$('#lp_class_seeless_'+enroll_id).css('display', 'none');
				$('#subgrid-class_menu_detail_'+enroll_id).css('display', 'none');
				$('#lp_class_seemore_'+enroll_id).css('display', 'block');
			}catch(e) {
        		// console.log(e);
        	}
		},
		
		// Respond to any changes the user makes to the
        // option method
        _setOption: function ( key, value ) {
            switch (key) {
            case "color":
                //this.options.someValue = doSomethingWith( value );
                break;
            default:
                this.options[ key ] = value;
                break;
            }

       },
        progressbarLine : function(enrid,value1,contentid,pagefrom,type,LessonId,is_multi_lesson) {
        	try {
        		//var value1 = 0.5;   
        		var top = '-17px';
        		if(pagefrom == 'seemore' || pagefrom == 'class_details' || pagefrom == 'course_class_list' || pagefrom =='lrnplan_course_class_list' || pagefrom == "lp_class_seemore"){
        			var ids = '#paindContentResults_'+enrid;
        		} else if(pagefrom == 'lp_seemore'){
        			var ids = '#paindLPContentResults_'+enrid;
        		} else if(is_multi_lesson == 1 && pagefrom == 'launch'){
        			var ids = '#cp-contentcontainer';
        		}else{
        			var ids = '#cp-menucontainer';
        		}
        		if(is_multi_lesson == 1){
        			var LessonId = LessonId+'_';
        		}else{
        			var LessonId = '';
        		}
        		setTimeout(function(){
        			var elementId = ids+' #cp-menu-progress_'+enrid+'_'+contentid+'_'+LessonId+type;
        			if (!$(elementId).hasClass('cp-prg-done')) {
        			if(typeof this.bar == 'undefined' || typeof this.bar[elementId] == 'undefined') {
        				var bar = new ProgressBar.Line(elementId, {    					  
      					  strokeWidth: 5,
    						  easing: 'easeInOut',
    						  duration: 1400,
    						  color: '#86D752',
    						  trailColor: '#DDDDDD',
    						  trailWidth: 5,
    						  svgStyle: {width: '100%', height: '100%'},
  						  text: {
  						    style: {
  						      // Text color.
  						      // Default: same as stroke color (options.color)
  						      color: '#ffffff',
  						      position: 'relative',
   						      textAlign: 'center',
  						      right: '0',
  						      top: top,
  						      padding: 0,
  						      margin: 0,
  						      transform: null,
  						      width: '100%',
  						      //left: '50%',
  						    },
  						    autoStyleContainer: false,
  						  },
      					  from: {color: '#FFEA82'},
  						  to: {color: '#ED6A5A'},
  						  step: function(state, bar){
  							 var value =  Math.round(value1 * 100);
  							    bar.setText(value+' %');
  							  }
        				});
							if(typeof this.bar == 'undefined') {
								this.bar = [];
							}
            				this.bar[elementId] = bar;
        					bar.text.style.fontFamily = 'Ariel,,"Raleway", Helvetica, sans-serif';
	  					bar.text.style.fontSize = '0.7rem';
        					bar.animate(value1);
	      				//$(elementId).addClass('cp-prg-done');
            			} else {
            				this.bar[elementId].destroy();
        				var bar = new ProgressBar.Line(elementId, {    					  
      					  strokeWidth: 5,
          					  easing: 'easeInOut',
          					  duration: 1400,
    						  color: '#86D752',
    						  trailColor: '#DDDDDD',
    						  trailWidth: 5,
    						  svgStyle: {width: '100%', height: '100%'},
          					  text: {
  						    style: {
  						      // Text color.
  						      // Default: same as stroke color (options.color)
  						      color: '#ffffff',
  						      position: 'relative',
   						      textAlign: 'center',
  						      right: '0',
  						      top: top,
  						      padding: 0,
  						      margin: 0,
  						      transform: null,
  						      width: '100%',
  						      //left: '50%',
  						    },
  						    autoStyleContainer: false,
          					  },
      					  from: {color: '#FFEA82'},
  						  to: {color: '#ED6A5A'},
  						  step: function(state, bar){
          					    var value = Math.round(value1 * 100);
  							    bar.setText(value+' %');
          					  }
          					});
              				this.bar[elementId] = bar;
          					bar.text.style.fontFamily = 'Ariel,,"Raleway", Helvetica, sans-serif';
	  					bar.text.style.fontSize = '0.7rem';
          					bar.animate(value1);
            			}
        			}
        			},100);
        	} catch(e) {
        		// console.log(e);
        	}
        },
        //73059: Content completion % is not getting updated unless we close the window and reopen it or move to the next content
        refreshContentProgressBarLine: function(contentPlayerObj,launched_data){
		 	if(launched_data.contentCode == 'sry_det_typ_ass' || launched_data.contentCode == 'sry_det_typ_sry' || launched_data.contentType == "pre-assessment" ||launched_data.contentType == "survey" || launched_data.contentType == "post-assessment"){
				var type = 'survey_assessment';
			}else{
				var type = 'content';
			}
			if(launched_data.tot_progress > 0){
				if(launched_data.content_id!=undefined && launched_data.RegId!= undefined){ 
		 			contentPlayerObj.progressbarLine(launched_data.RegId,launched_data.tot_progress,launched_data.content_id,'',type);
		 			contentPlayerObj.progressbarLine(launched_data.RegId,launched_data.progress,launched_data.content_id,'launch',type,launched_data.LessonId,1);
		 		}
		 	}else{
		 		if(launched_data.launchFrom == 'TP'){
		 			if(launched_data.content_id!=undefined && launched_data.RegId!= undefined)
		 				contentPlayerObj.progressbarLine('prg_'+launched_data.MasterId,launched_data.progress,launched_data.content_id,'',type);
		 		}
		 		else{
		 			if(launched_data.content_id!=undefined && launched_data.RegId!= undefined)
		 				contentPlayerObj.progressbarLine(launched_data.RegId,launched_data.progress,launched_data.content_id,'',type);
		 		}
		 	}
        },
        getSurveyResult : function(dataId,value1) {
        	try {
        		/*var enrollId = enrollid
        		var params = {};
        		var url = this.constructUrl('ajax/content-player/getsurveyresult/' + enrollId);
        		$.ajax({
        			type: "POST",
        			url: url,
        			data:  params,
        			success: function(result){
        				//obj.options.contentList = result;
        				//obj._generatePlayerMenu(data);
        			}
        		});*/
        	} catch(e) {
        		// console.log(e);
        	}
        }
	// Content player end	
	});
	$.extend($.ui.contentPlayer.prototype,EXPERTUS_SMARTPORTAL_AbstractManager, EXPERTUS_SMARTPORTAL_AbstractDetailsWidget);
})(jQuery, window, document);
var updateStatusToDB;
if (document.addEventListener)
{
    document.addEventListener('webkitfullscreenchange', exitHandler, false);
    document.addEventListener('mozfullscreenchange', exitHandler, false);
    document.addEventListener('fullscreenchange', exitHandler, false);
    document.addEventListener('MSFullscreenChange', exitHandler, false);
}

function exitHandler()
{
	//alert(document.msFullscreenElement);
    if (document.webkitIsFullScreen==false || document.mozFullScreen == false || document.msFullscreenElement == false)
    {
    	$('#cp-frame-container').css('position','relative');
        /* Run code on exit */
    }else{
    	$('#cp-frame-container').css('position','fixed');
    }
}

function resizeVideoJS(myPlayer){
	  //console.log('resizeVideoJS');
	  var id = vodPlayer.id();
	  // Make up an aspect ratio
      var aspectRatio = 16/9; 
        
    //var width = document.getElementById(id).parentElement.offsetWidth;
    //var height = document.getElementById(id).parentElement.offsetHeight;
      
    var width = $('#video-container').width(); //document.getElementById(id).parentElement.offsetWidth;
    var height = $('#video-container').height(); //document.getElementById(id).parentElement.offsetHeight;

    myPlayer.width(width).height(height);
   
}
//Create IE + others compatible event handler
//var eventMethod = window.addEventListener ? "addEventListener" : "attachEvent";
//var eventer = window[eventMethod];
//var messageEvent = eventMethod == "attachEvent" ? "onmessage" : "message";

// Listen to message from child window
//eventer(messageEvent,function(e) {
//	alert(1111);
//	 if (e.keyCode == 27) { // escape key maps to keycode `27`
//	    	alert(1111);
//	    	$('#cp-frame-container').css('position','relative');
//	   }
//},false);


$(function() {
	try{
		if(document.getElementById('learner-enrollment-tab-inner')) {
			$("#learner-enrollment-tab-inner").contentPlayer();
		}
		if(document.getElementById('learningplan-tab-inner')) {
			$("#learningplan-tab-inner").contentPlayer();
		}
		if(document.getElementById('lnr-catalog-search')) {
			$("#lnr-catalog-search").contentPlayer();
		}
		$('body').click(function(e) {
		    if (!$(e.target).closest('.clsConentValidtiy').length && !$(e.target).closest('.clsShowContentValidty').length){
		        $(".clsConentValidtiy").hide();
		    }
		});
		//$("#block-take-survey").surveylearner();
	}catch(e){
		// to do
	}
});

function objectToString( object ) {
	var str = '';
	  str += '{';
	    for (var p in object) {
	        if (object.hasOwnProperty(p)) {
	            str += '\'' + p +  '\'' + ':\'' + object[p] + '\',';
	        }
	    }
	  str += '}';
	    return str;
}
function showLblMsg(selId){
	// console.log(selId);
	$('#cp-contentcontainer #cp-content-main').html('<span class="msg-content"><b>' + Drupal.t('MSG796') + '</b></span>');/*Viswanathan added >b> tag for #0078402*/
}
function keyBindForMaximize(e){
	if (e.keyCode == 27) { // escape key maps to keycode `27`
    	alert(1111);
    	$('#cp-frame-container').css('position','relative');
   }
}
function log(message) {
	console.log(message);
}
function createLoaderNew(id)
{
	//alert("in loader function")
		   var height = 480;
		   var width = 300;
			$("#"+id).prepend("<div id='loader' class='loadercontent' style='z-index:10007;height:480px;width:100%;margin-top:-125px;'></div>");
			$("#loader").html('<table border="0" style="width: 100%; height: 100%;"><tr><td width="40%" height="'+height+'px">&nbsp;</td><td align="center" height="100%" valign="middle"><div class="loaderimg"><span class="ui-accordion-header"></span></div></td><td width="40%" height="'+height+'px">&nbsp;</td></tr></table>');
 }

function h5p_maximize(height_h5p){
	$("#h5pplayer").css("width",$("#cp-content-main").width());
	$("#h5pplayer").css("height",height_h5p);
	
	$("#h5pplayer").css("width","100%");
	$("#h5pplayer").css("max-width","100%");
//	$("#h5pplayer").contents().find(".Fullsc").addClass("fs-maximize");
	$("#h5pplayer").contents().find(".Fullsc").css("width","100%");
	$("#h5pplayer").contents().find(".Fullsc").css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".Fullsc").css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".Fullsc").parent().css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".Fullsc").parent().css("min-width",$("#cp-content-main").width());
	
	$("#h5pplayer").css("height","100%");
	$("#h5pplayer").css("max-height","100%");
	$("#h5pplayer").contents().find(".Fullsc").css("max-height",height_h5p);
	$("#h5pplayer").contents().find(".Fullsc").css("min-height",height_h5p);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("max-height",height_h5p);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("min-height",height_h5p);
		
	$("#h5pplayer").contents().find(".FullscYoutube").css("width","100%");
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("max-width",$("#cp-content-main").width());
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("min-width",$("#cp-content-main").width());
	
	
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-height",height_h5p);
	$("#h5pplayer").contents().find(".FullscYoutube").css("min-height",height_h5p);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("max-height",height_h5p);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("min-height",height_h5p);
	
	//for vimeo
	if($("#h5pplayer").attr("subtype") == "h5p-Vimeo")
		$("#h5pplayer").contents().find("#my_video_2").attr("style","height:180%;width:100%;left:0;position:absolute;top:-40%");

}

function h5p_minimize(width_cp,height_cp,height_cp_wrapper){
	$("#h5pplayer").contents().find(".Fullsc").css("max-width",width_cp);
	$("#h5pplayer").contents().find(".Fullsc").css("max-width",width_cp);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("max-width",width_cp);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("min-width",width_cp);	
	
	$("#h5pplayer").contents().find(".Fullsc").css("max-height",height_cp);
	$("#h5pplayer").contents().find(".Fullsc").css("min-height",height_cp);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("max-height",height_cp_wrapper);
	$("#h5pplayer").contents().find(".Fullsc").parent().css("min-height",height_cp_wrapper);
	
	
	//YOUTUBE
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-width",width_cp);
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-width",width_cp);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("max-width",width_cp);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("min-width",width_cp);
	
	
	$("#h5pplayer").contents().find(".FullscYoutube").css("max-height",height_cp);
	$("#h5pplayer").contents().find(".FullscYoutube").css("min-height",height_cp);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("max-height",height_cp_wrapper);
	$("#h5pplayer").contents().find(".FullscYoutube").parent().css("min-height",height_cp_wrapper);
	
	//for vimeo
	if($("#h5pplayer").attr("subtype") == "h5p-Vimeo")
		$("#h5pplayer").contents().find("#my_video_2").attr("style","height:180%;width:100%;left:0;position:absolute;top:-40%");

	
}
function updateVideoProgressToDB(obj){
	if($('#video-container').length) {
		var prevObject = obj.lastPlayedObject;
  		var launchedFrom 	= prevObject.LaunchFrom;
		var courseId 		= prevObject.courseId;
		var	classId			= prevObject.classId;
		var	lessonId		= prevObject.LessonId;
		var	versionId		= prevObject.VersionId;
		var	enrollId		= prevObject.enrollId;
		var prevContentStatus = '';
		var contentProgress = '';
		var callbackdata = {launchflag:1,data:undefined};
		$('#'+obj.options.LaunchFrom).data('contentLaunch').updateVODScoreOnCtoolsModalClose(launchedFrom, courseId, classId, lessonId, versionId, enrollId, prevContentStatus, contentProgress, callbackdata);
	}
}
function GetLabelsForContentType(contLabel){
	try{
		switch(contLabel) {
		case "Knowledge Content" :
		case "SCORM 1.2" :
		case "SCORM 2004" :
		case "Tin Can" :
		case "AICC" :
			label = Drupal.t(contLabel);
	        break;
	    case "Video on Demand":
	        label = Drupal.t('Video');
	     
	        break;
	    case "survey":
	    	label = Drupal.t('Survey');
	        break;
	    case "pre-assessment":
	    	label = Drupal.t('LBL1253')+" "+Drupal.t('Assessment');
	    	if(Drupal.settings.user.language == 'es') 
	    		label = Drupal.t('LBL1253'); 
	    	break;
	    case "post-assessment":
	    	label = Drupal.t('LBL871')+" "+Drupal.t('Assessment');
	    	if(Drupal.settings.user.language == 'es') 
	    		label = Drupal.t('LBL871');
	    	break;
	    default:
	    	label =  Drupal.t(contLabel);
	    	
		} 
		return label;
	}
	catch(e){
		// to do
	}
} 
  function  RedirectForLoadDeniedContent(data){
  
        	try {
        	
        		$("#" + data.LaunchFromTab).data('contentLaunch').launchWBTContent(data);
        
        	} catch(e) {
        		 console.log(e);
        	}
        }
        function playertimeoutcall(obj,data){

        setTimeout(function(){
        			if(data.defaultContent==''){
            			data.defaultContent = obj.classDetails['defaultLaunchContent']+'';
        			}
             		data.defaultContent = (data.defaultContent != '' || data.defaultContent != null) ? data.defaultContent : 0; // set default content as first content
             		if(obj.classDetails['lp_have_only_post_ass_cont']!=undefined)
             			data.lp_have_only_post_ass_cont = obj.classDetails['lp_have_only_post_ass_cont'];
             		if(obj.classDetails['cl_ilt_have_only_post_ass_cont']!=undefined)
             			data.cl_ilt_have_only_post_ass_cont = obj.classDetails['cl_ilt_have_only_post_ass_cont'];
        			obj.getPlayerContent(data); 		// prepare default content to play.
        		}, 1000);
  }