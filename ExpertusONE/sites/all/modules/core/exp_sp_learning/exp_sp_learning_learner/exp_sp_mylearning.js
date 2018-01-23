function showUserAnnouncementSettings(){
	//initalize announcements
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	var url = obj.constructUrl("learning/announcement/"+obj.getLearnerId());
			$.ajax({
				type: "POST",
				url: url,
				data:  '',
				success: function(data){
				var json = JSON.parse(data);
				if($('.mylearning-announcement-customization').hasClass('active')){
		            $('.mylearning-announcement-customization').removeClass('active');
	           }else{
		             $('.mylearning-announcement-customization').addClass('active');
	           }
	           $('#announcement-notification').hide();
	           $('.mylearning-announcement-tab .limit-title').trunk8(trunk8.ann_title);
	           $('.mylearning-announcement-tab .limit-desc').trunk8(trunk8.ann_desc);
				if(json.records == 0) {
				  	$('#no_announcement_msg').toggle();
				}
				else{
				    $('#announcement_custom').toggle();
					jQuery("#announcement_new").jcarousel({
						        scroll: 3,
						        initCallback: mycarousel_initCallback,
								itemLoadCallback: mycarousel_itemLoadCallback
					});
					if(json.records <= 3){
					   $('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-clip-horizontal').css("height","110px");
						$('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-container-horizontal').css("height","110px");
						$('.jcarousel-control-wrapper').hide();
						$('.jcarousel-control').hide();
						$('.jcarousel-next-horizontal').hide();
					}
					 if(json.records == 1){
					    $('#announcement_loader').css("top","60px");
						$('#announcement_loader').css("left","-210px");
					    $('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-container-horizontal').css("width","275px");
						$('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-clip-horizontal').css("width","275px");
						}
						else if(json.records == 2){
						$('#announcement_loader').css("top","60px");
						$('#announcement_loader').css("left","-66px");
						$('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-container-horizontal').css("width","550px");
						$('#block-exp-sp-administration-announcement-announcement-mylearning .jcarousel-skin-tango .jcarousel-clip-horizontal').css("width","550px");
						}
					}
				}
		});
function mycarousel_initCallback(carousel) {
    jQuery('.jcarousel-control a').bind('click', function() {
        carousel.scroll((jQuery.jcarousel.intval(jQuery(this).text())*3)-2); //  calculation for scroll value to be passed on clicking page number
        return false;
    });
   jQuery('.jcarousel-next').bind('click', function() {
        carousel.next();
        return false;
    });
	jQuery('.jcarousel-prev').bind('click', function() {
        carousel.prev();
        return false;
    });
 };
function mycarousel_itemLoadCallback(carousel, state)
{
   $('#announcement_loader').show();
   $('.mylearning-announcement-tab .limit-title').trunk8(trunk8.ann_title);
   $('.mylearning-announcement-tab .limit-desc').trunk8(trunk8.ann_desc);
	//console.log('mycarousel_itemLoadCallback');
// Check if the requested items already exist
    if (carousel.has(carousel.first, carousel.last)) {
        $('#announcement_loader').hide();
        return;
      }
  jQuery.get(
        obj.constructUrl("learning/announcement/"+obj.getLearnerId()),
        {
            first: carousel.first,
            last: carousel.last
        },
        function(json) {
            mycarousel_itemAddCallback(carousel, carousel.first, carousel.last, json);
        },
        'json'
    );
 };
function mycarousel_itemAddCallback(carousel, first, last, json)
{
    // Set the size of the carousel
	//console.log(json);
	//console.log('mycarousel_itemAddCallback');
	$('#announcement_loader').hide();
    carousel.size(parseInt(json.records));
	var obj = $('#announcement_new').data('announcement');
    jQuery(json.rows).each(function(i, row) {
        carousel.add(first + i, obj.paintAnnouncementNewTheme(row['cell']));
    });
    $('.mylearning-announcement-tab .limit-title').trunk8(trunk8.ann_title);
	$('.mylearning-announcement-tab .limit-desc').trunk8(trunk8.ann_desc);
	$(function() {
        $('.announce-shortdescription-list').mouseenter(function () {
			if($(this).find('span.fade-desc').length > 0 ) {
				$('.announce-fulldescription-list.clone').remove();
				$('#'+$(this).data('noticeid')).clone().insertAfter($('#announcement_new')).show().addClass('clone');
				$('.announce-fulldescription-list.clone').css('width',$(".announce-fulldescription-list.clone").width());
				$(".announce-fulldescription-list.clone")
			           .offset($(this).offset())
			           .mouseleave(function() {
			          $(this).remove();
			           })
			        .jScrollPane();
			      		innerWidthCal =  (parseInt($(window).width()) - parseInt($("#page-container").width())) / 2;
			      		if((parseInt($(window).width()) - innerWidthCal) < ( parseInt($(this).offset().left) + parseInt($(".announce-fulldescription-list.clone").width()))) {
							$('.announce-fulldescription-list.clone').css('left','');
							$('.announce-fulldescription-list.clone').css({
								'right':'0px'											
							});
						}
			}
			}).mouseleave(function(e) {
		    	var target = e.toElement || e.relatedTarget;
		    	if($(target).parents('.announce-fulldescription-list.clone').length == 0) {
		    	 $('.announce-fulldescription-list.clone').remove();
		    	}
		    	$(target).parents('.announce-fulldescription-list.clone').mouseleave(function() {
		    		 $('.announce-fulldescription-list.clone').remove();
		    	});
		    });
		});	
   };
function mycarousel_makeRequest(carousel, first, last, per_page, page)
	{
		//console.log('mycarousel_makeRequestk');
	   // Lock carousel until request has been made
	    carousel.lock();
	    var url = obj.constructUrl("learning/announcement/"+obj.getLearnerId()+'&page='+page+'&rows='+per_page);
	    jQuery.get(
	    		url,
	            {
	                'per_page': per_page, 
	                'page':page
	            },
	            function(result) {
	                mycarousel_itemAddCallback(carousel, first, last, result, page);
	            });
	};
function mycarousel_getItemHTML(row)
	{
	 // console.log('mycarousel_getItemHTML');
	    var drupalUserId = row['cell']['id'];
		var personid = row['cell']['personid'];
		var userName = row['cell']['screen_name'];
		var userImagePath = row['cell']['image_url'];
		var noticeId = row['cell']['nid'];
		var shortDescription = row['cell']['announce_shortdesc'];
		var fulldescription = row['cell']['announce_fulldescription'];
		var dateCreated = row['cell']['from_date'];
		var css = row['cell']['css'];
		var announcement = "<li class = 'announce-list-li'>" +
        "<div class ='announcement_list_name'>"+userName+"</div>" +
		"<div class ='announcement_list_date'>"+dateCreated+"</div>" +
		"<div class ='announcement_list_description'>"+shortDescription+"</div>" +
		"</li>";
		return announcement;
		};
}
function addClass(pageLink){
	$('#announcement_loader').show();
		var activepage = $(pageLink);
		if($('.jcarousel-control a').hasClass('active')){
			$('.jcarousel-control a').removeClass('active');
		}
		if(activepage){
			activepage.addClass('active');
		}
}
function addClassToPrev(){
if(!$('.jcarousel-prev').hasClass('jcarousel-prev-disabled-horizontal')){
	if($(".jcarousel-control a").hasClass("active")){
		var currpage = $(".jcarousel-control > .active").attr("class").match(/\d+/)[0]; //get current page which is active
		$('.page'+currpage).removeClass('active'); 
		$('.page'+(currpage-1)).addClass('active');
	  }
	}
}
function addClassToNext(){
	$('#announcement_loader').show();
	if(!$('.jcarousel-next').hasClass('jcarousel-next-disabled-horizontal')){
	   if($(".jcarousel-control a").hasClass("active")){
			var currpage = $(".jcarousel-control > .active").attr("class").match(/\d+/)[0]; //get current page which is active
			$('.page'+currpage).removeClass('active'); 
			currpage = ++currpage;
			$('.page'+currpage).addClass('active');
		}
	}
}
function showUserMyCalendarSettings(){
	$('#calendar_custom').toggle();
	if($('.mylearning-calendar-customization').hasClass('active')){
		$('.mylearning-calendar-customization').removeClass('active');
	}else{
		$("#catalog-admin-cal").data('mycalendar').launchMyCalendar();
		$('.mylearning-calendar-customization').addClass('active');
	}
	//$( "#catalog-admin-cal" ).mycalendar();
	
}
function showWhoisOnlineSettings() {
	try {
		if ($('.mylearning-whoisonline-customization').hasClass('active')) {
			$('.mylearning-whoisonline-customization').removeClass('active');
			$('#whoisonline-block_start').hide();
		} else {
			$('.mylearning-whoisonline-customization').addClass('active');
			$('#whoisonline-block_start').show();
		}
		var dataIDs = $("#expertus-online-users").data('lnrwhoisonline').totalRecords;
		// console.log('dataIDs length who is online block' + dataIDs);
		if (dataIDs <= 0) {
			$('#expertus-no-online-users-msg').show();
			$("#expertus-online-users-list").hide();
		} else {
			$('#expertus-no-online-users-msg').hide();
			$("#expertus-online-users-list").show();
		}
		$('#expertus-online-users .limit-title-whois-on').trunk8(trunk8.whois_username_title);
        $('#expertus-online-users .limit-title-whois-jobtitle').trunk8(trunk8.whois_jobtitle);
		$("#expertus-online-users").data('lnrwhoisonline').refreshGrid();
	} catch (e) {
		// window.console.log(e, e.stack);
	}
}
function showUserMyTranscriptSettings() {
    try {
        $('#div_my_transcript_start').toggle();
        if ($('.mylearning-mytranscript-customization').hasClass('active')) {
            $('.mylearning-mytranscript-customization').removeClass('active');
        } else {
        	$("#div_my_transcript").data('mytranscript').renderMyTranscript();
            $('.mylearning-mytranscript-customization').addClass('active');
        }
        $('#my_transcript_list .limit-title').trunk8(trunk8.transcript_title);   
	//	var dataIDs = $('#my_transcript_jqgrid').getGridParam("records");
		var dataIDs = $("#div_my_transcript").data('mytranscript').totalRecords;
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
		
        // $("#gview_my_transcript_jqgrid .ui-jqgrid-bdiv .jspScrollable .jspContainer").css('height', '480px');
        // $("#gview_my_transcript_jqgrid .ui-jqgrid-bdiv .jspTrack .jspDrag").css('height', '400px');
    } catch (e) {
        // window.console.log(e, e.stack);
    }
}
function initializeJScrollPane(blockname) {
	try {
	var divId,gridId;
	if(blockname == 'whoisonline') {
		divId = 'whoisonline-block_start';
		gridId = 'expertus-online-users-jqgrid';
	} else if(blockname == 'transcript') {
		divId = 'div_my_transcript_start';
		gridId = 'my_transcript_jqgrid';
	}
    $("#"+divId)
        .jScrollPane({
            showArrows: true,
            maintainPosition: false
        }).bind(
            'jsp-scroll-y',
            function(event, scrollPositionY, isAtTop, isAtBottom) {
				try {
                if (isAtBottom === true) {
                    var option = {
                        afterAppend: function() {
                         if(blockname == 'transcript') {
                         	$('#my_transcript_list .limit-title').trunk8(trunk8.transcript_title);
                         }else if(blockname == 'whoisonline') {
                         	$('#expertus-online-users .limit-title-whois-on').trunk8(trunk8.whois_username_title);
                         	$('#expertus-online-users .limit-title-whois-jobtitle').trunk8(trunk8.whois_jobtitle);
                         }
                            initializeJScrollPane(blockname);
                            vtip();
                        },
                        beforeAppend: function() {
	                        if(blockname == 'transcript') {
	                        	$("#div_my_transcript_start").data('jsp').destroy();
	                        }else if(blockname == 'whoisonline') {
	                            var css = {
										overflow: $("#whoisonline-block_start").css('overflow'),
										padding: $("#whoisonline-block_start").css('padding'),
										display: $("#whoisonline-block_start").css('display'),
										height: $("#whoisonline-block_start").css('height'),
										width: $("#whoisonline-block_start").css('width')
									}
                    			$("#whoisonline-block_start").data('jsp').destroy();
								$("#whoisonline-block_start").css(css);
	                        }
                        }
                    };
                    var postData = $("#"+gridId).jqGrid('getGridParam', "postData");
                    var pagenum = postData.page;
                  	var total_pages;
                    if(blockname == 'whoisonline') {
                    	total_pages = Math.ceil($("#expertus-online-users").data('lnrwhoisonline').totalRecords / 8);
                    } else if(blockname == 'transcript') {
                    	total_pages = Math.ceil($("#div_my_transcript").data('mytranscript').totalRecords / 10);
                    }
                    if (pagenum < total_pages) {
                    	if(blockname == 'transcript') {
                    		$('#div_my_transcript').data('mytranscript').createLoader('div_my_transcript_start');
                        	$('#my_transcript_jqgrid').jqGrid('appendDataToGrid', option);
                    	} else if(blockname == 'whoisonline') {
                    		$('#expertus-online-users').data('lnrwhoisonline').createLoader('whoisonline-block_start');
                        	$('#expertus-online-users-jqgrid').jqGrid('appendDataToGrid', option);
                    	}
                    }
                }
				} catch (e) {
					// window.console.log(e, e.stack);
				}
            });
	} catch (e) {
        // window.console.log(e, e.stack);
    }
}
$(document).ready(function() {
	 // Check my enrollment is exists or not
	  if ($('#tab_my_enrollment_customized').length > 0)  {
		  enrFilters = JSON.parse(Drupal.settings.enrselectedFilters);
		  var enrSearchStr = Drupal.settings.myenrolmentSearchStr;
		  if(enrSearchStr!='' && enrSearchStr!=null && enrSearchStr!=undefined){
			  var sortByIndex = enrSearchStr.indexOf('&sortBy=');
			  if(sortByIndex >0)
	  			var sortbyValue =  enrSearchStr.substring(sortByIndex+8, enrSearchStr.length);
	  		  else if(sortByIndex < 0)
	  			var sortbyValue = 'AZ';
		  }
		  statusFilterSubmit('myenrollment',sortbyValue); 
	  }
  	// Check my programs is exists or not
	  if ($('#tab_my_learningplan_customized').length > 0)  {
		  prgFilters = JSON.parse(Drupal.settings.prgselectedFilters);
		  var prgSearchStr = Drupal.settings.myprogramsSearchStr;
		  if(prgSearchStr!='' && prgSearchStr!=null && prgSearchStr!=undefined){
			  var sortByIndex = prgSearchStr.indexOf('&sortBy=');
			  if(sortByIndex >0)
			  	var sortbyValue =  prgSearchStr.substring(sortByIndex+8, prgSearchStr.length);
			  else
			  	var sortbyValue = 'AZ';
		  }
		  statusFilterSubmit('myprograms',sortbyValue);
	  } 
    // Check my classes is exist or not
     if ($('#tab_instructor_desk_customized').length > 0)  {
  		  clsFilters = JSON.parse(Drupal.settings.clsselectedFilters);
  		  var clsSearchStr = Drupal.settings.myclassesSearchStr;
		  if(clsSearchStr!='' && clsSearchStr!=null && clsSearchStr!=undefined){
			  var sortByIndex = clsSearchStr.indexOf('&sortBy=');
			  if(sortByIndex >0)
			   var sortbyValue =  clsSearchStr.substring(sortByIndex+8, clsSearchStr.length);
			  else
			   var sortbyValue = 'AZ';
		  }
  	      statusFilterSubmit('myclasses',sortbyValue); 	
  	 }
	  //initialize announcement widget
	  if($("#announcement_new").length > 0)
	  	$("#announcement_new").announcement();
});

$('.close-btn').click(function() {
  	$('.mylearning-filter-by').hide();
  	$('.active_block').removeClass('active');
});
// Show/Hide when clicking filters
function attachHandlerToFilterIcon() {
	$('.icon-filter').unbind( "click" );
	$('.icon-filter').click(function(){
	// Hide Calendar
	$('#calendar_custom').hide();
	$('#block-exp-sp-lnrcalendar-mycalendar-mylearning.active').removeClass('active');
	$('#block-exp-sp-lnrcalendar-mycalendar-mylearning.active').height("");
	// Hide Announcements
	$('#announcement_custom').hide();
	$('#no_announcement_msg').hide();
	$('#block-exp-sp-administration-announcement-announcement-mylearning.active').removeClass('active');
	$('#block-exp-sp-administration-announcement-announcement-mylearning.active').height("");
	// Hide Transcripts
	$('#div_my_transcript_start').hide();
	$('#block-exp-sp-mytranscript-mytranscript-mylearning.active').removeClass('active');
	$('#block-exp-sp-mytranscript-mytranscript-mylearning.active').height("");
	customVal = $(this).attr('data1');
	$('.active_block').removeClass('active');
	$('.active_block').height("");
	element = $('#'+customVal);
		if (element.css('display') == 'none') {
		$('.filter-block').hide();
		element.show();
		$(this).parents('.active_block').addClass('active');
		 if ($('#tab_my_enrollment_customized').length > 0)  {
			 tab = 'learner-myenrollment';
			 getTilHeadheig=$("#"+tab).height();
			// $('#'+tab+' .active').height(getTilHeadheig);
			// $('#'+tab).find('#'+customVal).css("top",getTilHeadheig);
		 }
		 if ($('#tab_my_learningplan_customized').length > 0)  {
			 tab = 'learner-myprograms';
			 getTilHeadheig=$("#"+tab).height();
			// $('#'+tab+' .active').height(getTilHeadheig);
			// $('#'+tab).find('#'+customVal).css("top",getTilHeadheig); 
		 }
		 if ($('#tab_instructor_desk_customized').length > 0)  {
			 tab = 'learner-myclasses';
			 getTilHeadheig=$("#"+tab).height();
			// $('#'+tab+' .active').height(getTilHeadheig);
			// $('#'+tab).find('#'+customVal).css("top",getTilHeadheig); 
			 }
		}
		else {
			element.hide();
			$(this).parents('.active_block').removeClass('active');
			$('.active_block').height("");
		}
	return false;
	});
	$('.icon-filter-hover .criteria-refine-icon').mouseenter(function(event) {
		var customVal = $(this).addClass('hover').parent().attr('data1');
		element = $('#'+customVal);
		element.show();
	});
	$('.icon-filter-hover .criteria-refine-icon, .icon-filter-menu').mouseleave(function(event) {
		try {
			if($(this).is('.criteria-refine-icon')) {
				var customVal = $(this).parent().attr('data1');
				var dropdown = $('#'+customVal);
				var refineIcon = $(this);
			} else {
				var dropdown = $(this);
				var refineIcon = $(this).siblings('.icon-filter-hover').find('.criteria-refine-icon');
			}
			var leaveFrom = event.toElement || event.relatedTarget;
			var isVtip = ($(leaveFrom).attr('id') == 'vtip' || $(leaveFrom).attr('id') == 'vtipArrow');
			if(!isVtip && $(leaveFrom).parents('.icon-filter-menu').length == 0 && !$(leaveFrom).is('.icon-filter-menu')) {
				dropdown.hide();
				refineIcon.removeClass('hover')
			}
		} catch(e) {
			// console.log(e, e.stack);
		}
	});
}

$(document).ready(function() {
	attachHandlerToFilterIcon();		
});
// Body click function for filters and menu
$('body').click(function (event) {
	try{
		var target = $(event.target);
		if(!target.hasClass('learner-filter-container active_block active') && target.attr('class') != 'mylearning-filter-by filter-block'
			&& target.parents('.filter-block').length == 0 && !target.is('.filter-block')) {
		 	$('.filter-block').hide();
			$('.active_block').removeClass('active');
			$('.active_block').height("");
		}
	}catch(e){
	//to do	
	}
});
// Location value reset
//commented for 77651
/*$('body').click(function (event) {
	try{
		if(event.target.className != 'ac_input searchtext' && event.target.className != 'ac_results') {
		var input_val =  $("form#myenrollment-filters input[id=myenrollment-location]").val();
	    if(input_val==''){
                     $('form#myenrollment-filters input[id=myenrollment-location]').attr('value',Drupal.t('LBL1321'));    
                       $('form#myenrollment-filters input[id=myenrollment-location]').css("color","#999999");
               }   
		 }
	}catch(e){
	//to do	
	}
});*/
//Body click for Calendar
$('body').click(function (event) {
	try{
        var target = $(event.target);
        if(!target.hasClass('calendaricon') && !target.hasClass('ui-corner-all') && target.attr('id') != 'calendar_new' && target.parents('#calendar_custom').length == 0 && event.target.className!='ui-icon ui-icon-circle-triangle-w' && event.target.className!='ui-icon ui-icon-circle-triangle-e' && event.target.className!='qtip-close-button-visible') {
			$('#calendar_custom').hide();
			$('#block-exp-sp-lnrcalendar-mycalendar-mylearning.active').removeClass('active');
			$('#block-exp-sp-lnrcalendar-mycalendar-mylearning.active').height("");
		}
	}catch(e){
	//to do	
	}
});
// Body click for Announcements
$('body').click(function (event) {
	try{
		var target = $(event.target);
		if(!target.hasClass('announcementicon') && target.attr('id') != 'announcement_new' && target.parents('#announcement_custom').length == 0 && target.parents('#no_announcement_msg').length == 0 && target.attr('id') != 'no_announcement_msg') {
			//alert(event.target.className);
			$('#announcement_custom').hide();
			$('#no_announcement_msg').hide();
			$('#block-exp-sp-administration-announcement-announcement-mylearning.active').removeClass('active');
			$('#block-exp-sp-administration-announcement-announcement-mylearning.active').height("");
		}
		
		//announcement_new
	}catch(e){
	//to do	
	}
});
// Body click for Transcripts
$('body').click(function (event) {
	try{
        var target = $(event.target);
        if(!target.hasClass('transcripticon') && target.attr('id') != 'transcript_new' && target.parents('#div_my_transcript_start').length == 0) {
			$('#div_my_transcript_start').hide();
	        $('#block-exp-sp-mytranscript-mytranscript-mylearning.active').removeClass('active');
	        $('#block-exp-sp-mytranscript-mytranscript-mylearning.active').height("");
		}
	}catch(e){
	//to do	
	}
});
// Body onclick event for Online users block
$('body').click(function (event) {
	try{
        var target = $(event.target);
        if(!target.hasClass('whoisonlineicon') && target.attr('id') != 'whoisonline-block_new' && target.parents('#whoisonline-block_start').length == 0) {
			$('#whoisonline-block_start').hide();
	        $('#block-exp-sp-whoisonline-online-users-mylearning.active').removeClass('active');
	        $('#block-exp-sp-whoisonline-online-users-mylearning.active').height("");
		}
	}catch(e){
	//to do	
	}
});
// Body click for Settings
$('body').click(function (event) {
	try{
		if(event.target.className!='settingicon'){
	        $('#block-exp-sp-learning-tab-user-customization').removeClass('active');
	        $('#block-exp-sp-learning-tab-user-customization').height("");
		}
		if(!$('#block-exp-sp-learning-tab-user-customization').hasClass('active')){
		 $('#block-exp-sp-learning-tab-user-customization').removeClass('active');
	     $('#block-exp-sp-learning-tab-user-customization').height("");
		}
	}catch(e){
	//to do	
	}
});
function selectedMenuFilter(tab,val,menuSel){
	try{
	  	if($('#learning_filter_'+tab+'_'+val).is(":checked") == false) {
			$('#learning_filter_'+tab+'_'+val).parent('label').addClass('checkbox-unselected');
			$('#learning_filter_'+tab+'_'+val).parent('label').removeClass('checkbox-selected');
			$('#learning_filter_'+tab+'_'+val).removeAttr('checked');
		}else {
	    $('#learning_filter_'+tab+'_'+val).parent('label').removeClass('checkbox-unselected');
			$('#learning_filter_'+tab+'_'+val).parent('label').addClass('checkbox-selected');
			$('#learning_filter_'+tab+'_'+val).attr('checked', 'checked');
		}
		if($('#learning_filter_'+tab+'_'+val).is(":checked") == false) {
			$('#learning_filter_'+tab+'_'+val).removeAttr('checked');
		}else {
	   		$('#learning_filter_'+tab+'_'+val).attr('checked', 'checked');
		}
	}catch(e){
		//console.log(e);
		}
}
// Sort by Filter funtionality
function enrollSortForm(sort,className) {
		$('#enroll-result-container .sort-by-links li').each(function() {
				$(this).find('a').removeClass('sortype-high-lighter');
		});		
		$('#enroll-result-container .'+className).addClass('sortype-high-lighter');
    	searchStr			= $.cookie('myenrolment_searchStr')+'&sortBy='+sort
    	var url 			= this.constructUrl('learning/enrollment-search/all/'+searchStr)
		$("#learner-enrollment-tab-inner").data("enrollment").createLoader('enroll-result-container');
    	$('#paintEnrollmentResults').setGridParam({url: url});
        $("#paintEnrollmentResults").trigger("reloadGrid",[{page:1}]);
}
function getSelectedValues(tab,sel,type) {
	try {
		if(tab == 'myenrollment') 
			selectedFilterObject = enrFilters;
		if(tab == 'myprograms')
			selectedFilterObject = prgFilters;
		if(tab == 'myclasses')
			selectedFilterObject = clsFilters;
		if(type == 'ck') {
			e1 = document.getElementById('learning_filter_'+tab+'_'+sel);
			filterVal = document.getElementById('learning_filter_'+tab+'_'+sel).value;
			typeVal = document.getElementById('learning_filter_'+tab+'_'+sel).getAttribute("data1");;
			attrVal = document.getElementById('learning_filter_'+tab+'_'+sel).getAttribute("data");
			nameVal = document.getElementById('learning_filter_'+tab+'_'+sel).getAttribute("name");
			if(e1.checked) {
				selectedFilterObject.push({'attrVal' : attrVal,'filterVal' : filterVal, 'type' : typeVal, 'name': nameVal});
			} 
			else {
				selectedFilterObject = selectedFilterObject.filter(function( obj ) {
				return obj.type !== typeVal;});
			}
		}
		if(type == 'dp') {
			e2 = document.getElementById(tab+'-'+sel);
			filterVal = e2.options[e2.selectedIndex].value;
			nameVal = e2.options[e2.selectedIndex].getAttribute("name")
			typeVal = e2.getAttribute("name");;
			attrVal = e2.options[e2.selectedIndex].getAttribute("data");
			var found = selectedFilterObject.some(function (el) {
	    		return el.type === typeVal;
	  		});
	  		if (found) { 
				selectedFilterObject = selectedFilterObject.filter(function( obj ) {
				return obj.type !== typeVal;});
	  		} 
			selectedFilterObject.push({"filterVal" : filterVal, "attrVal" : attrVal, "type" : typeVal,"name": nameVal});
	  	}
		if(type == 'textparamsearch') {
			
		  	e5 = document.getElementById(tab+'-textsearch');
		  	filterVal = e5.value;
		  	if(filterVal == Drupal.t('Type to search')){
		  		selectedFilterObject = $.grep(selectedFilterObject, function(e){ 
		  		 return e.type != "textsearch"; 
		  		
		  	});
			}
			else {
				typeVal = e5.getAttribute("data");
				var found = selectedFilterObject.some(function (el) {
		    		return el.type === typeVal;
		  		});
		  		if (found) { 
					selectedFilterObject = selectedFilterObject.filter(function( obj ) {
					return obj.type !== typeVal;});
		  		}
				selectedFilterObject.push({"filterVal" : filterVal, "attrVal" : filterVal, "type" : typeVal,"name": typeVal});
			}
		}
	  	if(type == 'other') {
		  	e3 = document.getElementById(tab+'-location');
		  	filterVal = e3.value;
		  	if(filterVal == Drupal.t('LBL1321')){
		  		selectedFilterObject = $.grep(selectedFilterObject, function(e){ 
		  		     return e.type != "location"; 
		  		});
		  		
		  	}
		  	else {
				typeVal = e3.getAttribute("data");
				var found = selectedFilterObject.some(function (el) {
		    		return el.type === typeVal;
		  		});
		  		if (found) { 
					selectedFilterObject = selectedFilterObject.filter(function( obj ) {
					return obj.type !== typeVal;});
		  		}
				selectedFilterObject.push({"filterVal" : filterVal, "attrVal" : filterVal, "type" : typeVal,"name": typeVal});
			}
	  	}
		if(tab == 'myenrollment') 
			enrFilters = selectedFilterObject;
		if(tab == 'myprograms')
			prgFilters = selectedFilterObject;
		if(tab == 'myclasses')
			clsFilters = selectedFilterObject;
	} catch(e) {
	
	}
} 
// Clear all functionality
function clearallFilters(tab,sort) {	
try {
		$('#learner-'+tab).find("input[type=checkbox]").removeAttr('checked');
		$('#learner-'+tab).find('label.checkbox-selected').removeClass('checkbox-selected').addClass('checkbox-unselected');
		$('#learner-'+tab).find("input[type=text], textarea").val("");
		$('#learner-'+tab).find("select").each(function() { this.selectedIndex = 0 }); 
		locationTextsearchFilterSetPlaceholderVal(tab);  
		tempEnrFilters = [];
		if(tab == 'myenrollment')
			enrFilters = tempEnrFilters;
		if(tab == 'myprograms')
			prgFilters = tempEnrFilters;
		if(tab == 'myclasses')
			clsFilters = tempEnrFilters;
		statusFilterSubmit(tab,sort);
		$('#learner-'+tab).find('.cls-filter-clear').hide();
	 } catch(e) {
 }
}
function locationTextsearchFilterSetPlaceholderVal(tab) {
	try {
	
	var input_val =  $(tab+'-location').val();  
	var textsearch_val =  $(tab+'-textsearch').val();  
	if(input_val=='' || input_val== undefined || input_val== null){
 	  if(tab == 'myenrollment'){
 	  	  $('#myenrollment-selectedLocid').val('');
 		  $('form#myenrollment-filters input[id=myenrollment-location]').blur();
 		 }
 	  if(tab == 'myprograms'){
 	  	  $('#myprograms-selectedLocid').val('');
 		  $('form#myprograms_filters input[id=myprograms-location]').blur();
 		  }
 	  if(tab == 'myclasses'){
 	   $('#myclasses-selectedLocid').val('');
 	   $('form#myclasses_filters input[id=myclasses-location]').blur();
 	   }
 	 }  
	    if(textsearch_val=='' || textsearch_val== undefined || textsearch_val== null){
	 	  if(tab == 'myenrollment')
	 		  $('form#myenrollment-filters input[id=myenrollment-textsearch]').blur();
	 	  if(tab == 'myprograms')
	 		  $('form#myprograms_filters input[id=myprograms-textsearch]').blur();
	 	  if(tab == 'myclasses')
	 	   	  $('form#myclasses_filters input[id=myclasses-textsearch]').blur();
}
	} catch(e) {
	} 
}
function capitalizeFirstLetter(string) {
	return string.charAt(0).toUpperCase() + string.substr(1);
}
function getTranslatedString(val) {
	try {
		var strVal = '';
		switch(val) {
			case 'Next 7 days':
				strVal = Drupal.t('Next 7 days');
				break;
			case 'Next 15 days':
				strVal = Drupal.t('Next 15 days');
				break;
			case 'Next 30 days':
				strVal = Drupal.t('Next 30 days');
				break;
			case 'Next 90 days':
				strVal = Drupal.t('Next 90 days');
				break;
			case 'Last 7 days':
				strVal = Drupal.t('Last 7 days');
				break;
			case 'Last 15 days':
				strVal = Drupal.t('Last 15 days');
				break;
			case 'Last 30 days':
				strVal = Drupal.t('Last 30 days');
				break;
			case 'Last 90 days':
				strVal = Drupal.t('Last 90 days');
				break;	
			case 'Priced':
				strVal = Drupal.t('Priced');
				break;
			case 'Free':
				strVal = Drupal.t('Free');
				break;
			case 'System':
				strVal = Drupal.t('LBL432');
				break;
			case 'Web based':
				strVal = Drupal.t('LBL1129');
				break;
			case 'virtual class':
				strVal = Drupal.t('Virtual Class');
				break;
			case 'completed and need reregister':
				strVal = Drupal.t('Completed and need reregister');
				break;
			case 'completed with attempts left':
				strVal = Drupal.t('Completed with attempts left');
				break;
			case 'scheduled':
				strVal = Drupal.t('Scheduled');
				break;
			case 'learning plan':
				strVal = Drupal.t('Learning Plan');
				break;		
			default:
				strVal = '';
				break;			
		}
		return strVal;
	} catch(e) {
	
	}

}
function selectedFilterToHeader(tab,data) {
	try {
		var assignedbyStr = '';
		if(tab == 'myenrollment' || tab == 'myprograms' || tab == 'myclasses') {
			var arr = data;
		}
		arr.forEach(function(e) {
			var label = '';
			if(e.attrVal != null) {
			if(e.type == 'due') {
				label =  Drupal.t("Due date") + " : ";
			}
			if(e.type == 'scheduled') {
				label = Drupal.t('Scheduled') + " : ";
			}
			if(e.type == 'reg') {
				label = Drupal.t('Date registered') + " : ";
			}
			if(e.attrVal == 'Next 7 days' || e.attrVal == 'Next 15 days' || e.attrVal == 'Next 30 days' || e.attrVal == 'Next 90 days'|| e.attrVal == 'Last 7 days'
			|| e.attrVal == 'Last 15 days' || e.attrVal == 'Last 30 days' || e.attrVal == 'Last 90 days' || e.attrVal == 'Priced' || e.attrVal == 'Free'
			|| e.attrVal == 'System' || e.attrVal == 'Web based' || e.attrVal == 'virtual class' || e.attrVal == 'completed and need reregister' || e.attrVal == 'completed with attempts left'
			|| e.attrVal == 'scheduled' || e.attrVal == 'learning plan') {
				transStr = getTranslatedString(e.attrVal);
			}
			else {
				str = capitalizeFirstLetter(e.attrVal);
				transStr = Drupal.t(str);
				// console.log('translate str'+e.attrVal);
			}

			var delAction = '<span class="enable-delete-icon" onclick=\'deleteStatus("'+tab+'","'+e.type+'")\'></span>';
			assignedbyStr = '<span class="cls-filtermenu">'+label+transStr+delAction+'</span>';
			$('#'+tab+'-searchFilter').append("<div class='checkedmenu' id='"+tab+"-"+e.type+"' >"+assignedbyStr+"</div>");
			 }
		});		
	} catch(e) {
	
	}
}
function deleteStatus(tab,status) {
	try {
		if(tab == 'myenrollment') 
			var selectedFilters = enrFilters;
		if(tab == 'myprograms') 
			var selectedFilters = prgFilters;
		if(tab == 'myclasses')
			var selectedFilters = clsFilters;
		selectedFilters = selectedFilters.filter(function(el) {
	    	return el.type !== status;
			});
		if(tab == 'myenrollment')
			enrFilters = selectedFilters;
		if(tab == 'myprograms')
			prgFilters = selectedFilters;
		if(tab == 'myclasses')
			clsFilters = selectedFilters;
		$('#learning_filter_'+tab+'_'+status).parent('label').addClass('checkbox-unselected');
		$('#learning_filter_'+tab+'_'+status).parent('label').removeClass('checkbox-selected');
		$('#learning_filter_'+tab+'_'+status).removeAttr('checked');    
		$('div[id="' + tab + '-'+status+'"]').remove();
		$('#'+tab+'-'+status+' option:eq(0)').attr('selected','selected');
		$('#'+tab+'-'+status).val('');
		locationTextsearchFilterSetPlaceholderVal(tab);
		
		statusFilterSubmit(tab);
	}
	catch(e){
		
	}
}
//Update the selected filters
function filterUpdateCall(tab,data,searchstr){
	try {	
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	var url = obj.constructUrl("filters/update");	
	$.ajax({
		type: "POST",
		url: url,
		dataType: 'json',
		async: false,
		data: {data : data,tab:tab,str:searchstr}, 
		success: function(result){			
			if(tab == 'myprograms') {
				Drupal.settings.prgselectedFilters = data;
				Drupal.settings.myprogramsSearchStr = searchstr;
			}else if(tab == 'myenrollment') {
				Drupal.settings.enrselectedFilters = data;
				Drupal.settings.myenrolmentSearchStr = searchstr;
			}else if(tab == 'myclasses') {
				Drupal.settings.clsselectedFilters = data;
				Drupal.settings.myclassesSearchStr = searchstr;
			}
		}
	});
	} catch (e) {
		// TODO: handle exception
		// console.log(e, e.stack);
	}
}

/*function filterSearchStrUpdateCall(tab,data){
console.log('filterSearchStrUpdateCall call check');
	var obj = EXPERTUS_SMARTPORTAL_AbstractDetailsWidget;
	var url1 = obj.constructUrl("filters/update/searchstr");	
	$.ajax({
		type: "POST",
		url: url1,
		async: false,
		data: {str : data,tab:tab}, 
		success: function(result){}
	});
}*/

function statusFilterSubmit(tab,sort,sortByClassName){
try{
	
	var status='';
  	var menu = '';
  	var checkedMenuName = '';
  	menustr = '';
  	$('#learner-'+tab).find(".sortbyStatusFilter").remove();
 	$('#'+tab+'-searchFilter').empty(); 	
  	if(tab == 'myenrollment'){
  		statusName = 'status_filter_myenrollment';
  	}
  	if(tab == 'myprograms'){
  		statusName = 'status_filter_myprograms';
  	}
  	if(tab == 'myclasses'){
  		statusName = 'status_filter_myclasses';
  	}
  	if($('#learner-'+tab).find("input[name="+statusName+"]:checked").length > 0 ) {
		$('#learner-'+tab).find("input[name="+statusName+"]:checked").each(function() {
		status+=status!=''?'|':'';
		status+=$(this).val();	
		menu+=menu!=''?'|':'';
	});
	}
	if(($('#learner-'+tab).find("input[name="+statusName+"]:checked").length > 1 )||($('#learner-'+tab).find("input[name="+statusName+"]:checked").length == 0) ){
		if(tab == 'myenrollment') {
			$('#learner-'+tab).find(".sort-by-links").append("<li class='sortbyStatusFilter'><a class='type6' onclick=\"statusFilterSubmit('myenrollment','orderbystatus','type6');\" >"+Drupal.t('LBL102')+"</a></li>");
		}
		if(tab == 'myprograms') {
			$('#learner-'+tab).find(".sort-by-links").append("<li class='sortbyStatusFilter'><a class='type6' onclick=\"statusFilterSubmit('myprograms','orderbystatus','type6');\" >"+Drupal.t('LBL102')+"</a></li>");
		}
		if(tab == 'myclasses') {
			$('#learner-'+tab).find(".sort-by-links").append("<li class='sortbyStatusFilter'><a class='type6' onclick=\"statusFilterSubmit('myclasses','orderbystatus','type6');\" >"+Drupal.t('LBL102')+"</a></li>");
		}
	}
	var del_type = '';
	if(tab == 'myenrollment'){
  		delName = 'myenrollment-del_type';
  	}
  	if(tab == 'myprograms'){
  		delName = 'myprograms-del_type';
  	}
  	if(tab == 'myclasses'){
  		delName = 'myclasses-del_type';
  	}
	if($('#learner-'+tab).find("input[name="+delName+"]:checked").length > 0) {
		$('#learner-'+tab).find("input[name="+delName+"]:checked").each(function(){
		  del_type+=del_type!=''?'|':'';
		  del_type+=$(this).val();	
		  checkedMenuName = $(this).attr('data1');
		});
	}
	var tra_type = '';
	if(tab == 'myenrollment'){
  		traName = 'myenrollment-tra_type';
  	}
  	if(tab == 'myprograms'){
  		traName = 'myprograms-tra_type';
  	}
  	if(tab == 'myclasses'){
  		traName = 'myclasses-tra_type';
  	}
	if($('#learner-'+tab).find("input[name="+traName+"]:checked").length > 0) {
		checkedMenuName = 'training type :';
	 	$('#learner-'+tab).find("input[name="+traName+"]:checked").each(function(){
		  tra_type+=tra_type!=''?'|':'';
		  tra_type+=$(this).val();	
		});
	}
	var price = '';
	if(($('#'+tab+'-price :selected').val() != undefined || $('#'+tab+'-price :selected').val() != null) && $('#'+tab+'-price :selected').val() != 0) {
		price = $('#'+tab+'-price :selected').val();
	}
	var location = '';
	if(($('#'+tab+'-location').val() != undefined || $('#'+tab+'-location').val() != null)  && $('#'+tab+'-location').val() != Drupal.t('LBL1321')) {
		location = $('#'+tab+'-location').val(); 
	}
	var scheduled = '';
	if(($('#'+tab+'-scheduled :selected').val() != undefined || $('#'+tab+'-scheduled :selected').val() != null) && $('#'+tab+'-scheduled :selected').val() != 0) {
		scheduled = $('#'+tab+'-scheduled :selected').val();
	}
	var reg = '';
	if(($('#'+tab+'-reg :selected').val() != undefined || $('#'+tab+'-reg :selected').val() != null) && $('#'+tab+'-reg :selected').val() != 0) {
		reg = $('#'+tab+'-reg :selected').val();
	 }
	var due = '';
	if(($('#'+tab+'-due :selected').val() != undefined || $('#'+tab+'-due :selected').val() != null) && $('#'+tab+'-due :selected').val() != 0) {
		due = $('#'+tab+'-due :selected').val();
			
	}
	var assignedby = '';
	if(($('#'+tab+'-assignedby :selected').val() != undefined || $('#'+tab+'-assignedby :selected').val() != null) && $('#'+tab+'-assignedby :selected').val() != 0) {
		assignedby = $('#'+tab+'-assignedby :selected').val();
	}
	var textsearch = '';
	if(($('#'+tab+'-textsearch').val() != undefined || $('#'+tab+'-textsearch').val() != null)  && $('#'+tab+'-textsearch').val() != Drupal.t('Type to search')) {
		textsearch = $('#'+tab+'-textsearch').val();
	}
	/* SORT BY reset issue 71523- Added by maheswari*/
	var searchStr;
	if(tab == 'myenrollment') 
		searchStr = Drupal.settings.myenrolmentSearchStr;
	if(tab == 'myprograms') 
		searchStr = Drupal.settings.myprogramsSearchStr;
	if(tab == 'myclasses')
		searchStr = Drupal.settings.myclassesSearchStr;
	var sortby;
	if (sort!=null && sort!=undefined) {
    	sortby = sort;     	
    }else if(searchStr != '' && searchStr != null && searchStr != undefined) {			
			var sortByIndex = searchStr.indexOf('&sortBy=');  	
			sortbyValue = searchStr.substring(sortByIndex+8, searchStr.length);	
		if((sortbyValue=='orderbystatus')&&($('#learner-'+tab).find("input[name="+statusName+"]:checked").length==1))
		    	sortby = 'AZ'; 
	    else						
		sortby = sortbyValue;	    
		    }else			
		sortby = 'AZ';
    /*Filter reset issue 71523*/		
	if(sortByClassName!= null) {
		$('#learner-'+tab).find('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		$('#learner-'+tab).find('.'+sortByClassName).addClass('sortype-high-lighter');
    }
    else {
		$('#learner-'+tab).find('.sort-by-links li').each(function() {
			$(this).find('a').removeClass('sortype-high-lighter');
		});
		if(sortby!=''){
			if(sortby == 'ZA')
				$('#learner-'+tab).find('.type2').addClass('sortype-high-lighter');
			else if(sortby == 'dateOld')
				$('#learner-'+tab).find('.type5').addClass('sortype-high-lighter');
			else if(sortby == 'dateNew')
				$('#learner-'+tab).find('.type4').addClass('sortype-high-lighter');
			else if(sortby == 'orderbystatus')
				$('#learner-'+tab).find('.type6').addClass('sortype-high-lighter');
			else if(sortby == 'type')
				$('#learner-'+tab).find('.type3').addClass('sortype-high-lighter');
			else if(sortby == 'Mandatory')
				$('#learner-'+tab).find('.type7').addClass('sortype-high-lighter');
			else if(sortby == 'startdate')
				$('#learner-'+tab).find('.type8').addClass('sortype-high-lighter');
			else
				$('#learner-'+tab).find('.type1').addClass('sortype-high-lighter');
		}
	    }
	    selectedLocID = '';
	    locId  = $('#'+tab+'-selectedLocid');
	    if(locId != null || locId != undefined) {
	    	selectedLocID = locId.val();
	    }
	if(tab == 'myenrollment') {
		var enrJsonString = JSON.stringify(enrFilters);
		
		var myenrolmentSearchStr	= '&regstatuschk='+status+'&del_type='+del_type+'&tra_type='+tra_type+'&price='+price+'&scheduled='+scheduled+'&reg='+reg+'&due='+due+'&assignedby='+assignedby+'&location='+location+'&selectedLocID='+selectedLocID+'&searchText='+textsearch+'&sortBy='+sortby;
		selectedFilterToHeader(tab,enrFilters);
		filterUpdateCall(tab,enrJsonString,myenrolmentSearchStr);
		if($("#myenrollment-searchFilter .checkedmenu").length > 0){
			$('#myenrollment-searchFilter').show();
			clearAllStr = Drupal.t('clear all');
			$('#myenrollment-searchFilter').append("<a class=\"cls-filter-clear\" onclick=\"clearallFilters('"+tab+"','"+sortby+"')\">"+clearAllStr+"</a>");
		}
		else 
			$('#myenrollment-searchFilter').hide();
		$("#learner-enrollment-tab-inner").data("enrollment").callEnrollment(myenrolmentSearchStr);		
	}	
	if(tab == 'myprograms') {
		var prgjsonString = JSON.stringify(prgFilters);
		var myprogramssearchStr	= '&regstatuschk='+status+'&del_type='+del_type+'&tra_type='+tra_type+'&price='+price+'&scheduled='+scheduled+'&reg='+reg+'&due='+due+'&assignedby='+assignedby+'&location='+location+'&selectedLocID='+selectedLocID+'&searchText='+textsearch+'&sortBy='+sortby;
		selectedFilterToHeader(tab,prgFilters);
		filterUpdateCall(tab,prgjsonString,myprogramssearchStr);
		if($("#myprograms-searchFilter .checkedmenu").length > 0){
			$('#myprograms-searchFilter').show();
			clearAllStr = Drupal.t('clear all');
			$('#myprograms-searchFilter').append("<a class=\"cls-filter-clear\" onclick=\"clearallFilters('"+tab+"','"+sortby+"')\">"+clearAllStr+"</a>");
		}
		else 
			$('#myprograms-searchFilter').hide();
		$("#learningplan-tab-inner").data("learningplan").callLearningPlan(myprogramssearchStr);
	}
	if(tab == 'myclasses') {
		var clsjsonString = JSON.stringify(clsFilters);
		var myclassesSearchStr	= '&regstatuschk='+status+'&del_type='+del_type+'&tra_type='+tra_type+'&price='+price+'&scheduled='+scheduled+'&reg='+reg+'&due='+due+'&assignedby='+assignedby+'&location='+location+'&selectedLocID='+selectedLocID+'&searchText='+textsearch+'&sortBy='+sortby;
		selectedFilterToHeader(tab,clsFilters);
		filterUpdateCall(tab,clsjsonString,myclassesSearchStr);
		if($("#myclasses-searchFilter .checkedmenu").length > 0){
			$('#myclasses-searchFilter').show();
			clearAllStr = Drupal.t('clear all');
			$('#myclasses-searchFilter').append("<a class=\"cls-filter-clear\" onclick=\"clearallFilters('"+tab+"','"+sortby+"')\">"+clearAllStr+"</a>");
		}
		else 
			$('#myclasses-searchFilter').hide();
		$("#instructor-tab-inner").data("instructordesk").callInstructor(myclassesSearchStr);
	}	
	$('.filter-block').hide();
	$('.active_block').removeClass('active');
	$('.learner-sort').removeClass('active');
	$('.active_block').height("");
} catch(e){
	//console.log(e);
	}
}
function seeMoreMyLearning(dataId,type){
	if(type === 'myenrollmentclass'){
		$("#"+dataId+" #classShortDesc_"+dataId).find("#arrow-more").click();
                $("#"+dataId+" #classShortDesc_"+dataId).find("#arrow-less").css('display', 'none');
		$('#class-accodion-'+dataId).click();
		$('#seemore_'+dataId).css('display', 'none');
		var seelessHtml = '<div class="cp_seeless clsDisabledMode" id="seeless_'+dataId+'" onclick="seeLessMyLearning('+dataId+','+'\''+type+'\');">'+ Drupal.t('LBL3042') +'</div>';
		if(document.getElementById("seeless_")+dataId)
			$('#seeless_'+dataId).remove();
			$('#'+dataId+'SubGrid .enroll-accordian-div').after(seelessHtml);
		resetFadeOutForAttributes('#'+dataId+'SubGrid','main-item-container','line-item-container','container','myenrollment');
		} else if(type === 'myprogramtp'){
		$("#prg_"+dataId+" #LPShortDesc_prg_"+dataId).find("#arrow-more").click();
		$("#prg_"+dataId+" #LPShortDesc_prg_"+dataId).find("#arrow-less").css('display', 'none');
		$('#prg-accodion-prg_'+dataId).click();
		$('#lp_seemore_prg_'+dataId).css('display', 'none');
		var seelessHtml = '<tr id="lp_seeless_tr_prg_'+dataId+'" data-rowid=prg_'+dataId+' class="jqgcrow ui-row-prg_'+dataId+'"><td>&nbsp;</td><td colspan="2"><div class="lp_seeless" id="lp_seeless_prg_'+dataId+'" onclick="seeLessMyLearning('+dataId+','+'\''+type+'\');">'+ Drupal.t('LBL3042') +'</div></td></tr>';
		if(document.getElementById("lp_seeless_prg_"+dataId))
			$('#lp_seeless_prg_'+dataId).remove();
			$('#prg_'+dataId+'SubGrid').after(seelessHtml);
		resetFadeOutForAttributes('#course-detail-section-'+dataId,'main-item-container','line-item-container','container','myprogramtp');
		} else if(type === 'tprecertify'){
		$("#mod_"+dataId+" #LPShortDesc_mod_"+dataId).find("#arrow-more").click();
		$('#prg-accodion-mod_'+dataId).click();
		$('#lp_seemore_prg_'+dataId).css('display', 'none');
		var seelessHtml = '<tr id="lp_seeless_tr_prg_'+dataId+'" data-rowid=prg_'+dataId+' class="jqgcrow ui-row-prg_'+dataId+'"><td>&nbsp;</td><td colspan="2"><div class="lp_seeless" id="lp_seeless_prg_'+dataId+'" onclick="seeLessMyLearning('+dataId+','+'\''+type+'\');">'+ Drupal.t('LBL3042') +'</div></td></tr>';
		if(document.getElementById("lp_seeless_prg_"+dataId))
			$('#lp_seeless_prg_'+dataId).remove();
			$('#mod_'+dataId+'SubGrid').after(seelessHtml);
		} else if(type == 'myprogramclass'){
		$("#lp-cl-"+dataId+" #classShortDesc_"+dataId).find("#arrow-more").click();
		$("#classShortDesc_"+dataId).find("#arrow-less").css('display', 'none');
		$('#lp-class-accodion-'+dataId).click();
		$('#lp_class_seeless_'+dataId).css('display','block');
		$('#lp_class_seemore_'+dataId).css('display','none');
		$("#subgrid-class_menu_detail_"+dataId).css('display', 'none');
		resetFadeOutForAttributes('#'+dataId+'LPDetailsMainDiv','main-item-container','line-item-container','container','myprogramclass');
	}
}
function seeLessMyLearning(dataId,type){

	if(type === 'myenrollmentclass'){
		$('#class-accodion-'+dataId).click();
		$("#"+dataId+" #classShortDesc_"+dataId).find("#arrow-less").click();
		$('#seemore_'+dataId).css('display', 'block');
	} else if(type === 'myprogramtp'){
		//$('#prg-accodion-prg_'+dataId).click();
		$('#prg_'+dataId+'SubGrid').hide(300);
		$("#prg_"+dataId+" #LPShortDesc_prg_"+dataId).find("#arrow-less").click();
		$('#lp_seeless_tr_prg_'+dataId).css('display', 'none');
		$('#lp_seemore_prg_'+dataId).css('display', 'block');
	} else if(type === 'tprecertify'){
		$('#mod_'+dataId+'SubGrid').hide(300);
		$("#mod_"+dataId+" #LPShortDesc_mod_"+dataId).find("#arrow-less").click();
		$('#lp_seeless_tr_prg_'+dataId).css('display', 'none');
		$('#lp_seemore_prg_'+dataId).css('display', 'block');
	} else if(type == 'myprogramclass'){
		$('#lp-class-accodion-'+dataId).click();
		$("#lp-cl-"+dataId+" #classShortDesc_"+dataId).find("#arrow-less").click();
		$('#lp_class_seemore_'+dataId).css('display','block');
		$('#lp_class_seeless_'+dataId).css('display','none');
		$("#subgrid-class_menu_detail_"+dataId).css('display', 'none');
	}
}
// Function to Search text on Enter Key
function searchKeyPress(event,tab){
	try{
		if(event.keyCode == 13){
			var i = tab+'-text search-searchTxtFrm';
		    document.getElementById(i).click();
		    $(".ac_results").hide();
			return false;
		}
		return true;
	}catch(e){
				  //to do
	}
}
function callContentPlayer(obj){
	if(Drupal.settings.mylearning.user.survey_dedicatetpcount > 0) {
		$('#learningplan-tab-inner').data('contentPlayer').playContent({'masterEnrollId':''+Drupal.settings.mylearning.user.survey_masenrollid+'','enrollId':''+Drupal.settings.mylearning.user.survey_enrollid+'','programId':'0','isMultiLesson':'0','contentType':'survey','cType':'survey','contentId':''+Drupal.settings.mylearning.user.survey_id+'','LessonId':'','VersionId':'','url1':'','courseId':''+Drupal.settings.mylearning.user.survey_courseid+'','classId':''+Drupal.settings.mylearning.user.survey_objectid+'','url2':'','ErrMsg':'','Status':'','LessonLocation':'','launch_data':'','suspend_data':'','exit':'','AICC_SID':'','MasteryScore':'','remDays':'undefined','ValidityDays':'undefined','no_of_attempts':'null','AttemptLeft':'undefined','attempts':'null','MaxAttempt':'undefined','surveycount':''+Drupal.settings.mylearning.user.survey_status+'','is_post_ass_launch':'undefined','playStatus':'true','defaultContent':''+Drupal.settings.mylearning.user.survey_defaultcontent+'','pagefrom':'','LaunchFrom':'LP',})
	} else if(Drupal.settings.mylearning.user.survey_recurringcount > 0 || (Drupal.settings.mylearning.user.survey_recertify == 1 && Drupal.settings.mylearning.user.survey_sharetype=='multiple') || Drupal.settings.mylearning.user.survey_multipleTP >= 1) {
		$('#learner-enrollment-tab-inner').data('contentPlayer').playContent(obj);
	} else if(Drupal.settings.mylearning.user.survey_objecttype=='cre_sys_obt_cls' && Drupal.settings.mylearning.user.survey_dedicatetpcount < 1) {
		$('#learner-enrollment-tab-inner').data('contentPlayer').playContent({'masterEnrollId':'0','enrollId':''+Drupal.settings.mylearning.user.survey_enrollid+'','programId':'0','courseId':''+Drupal.settings.mylearning.user.survey_courseid+'','classId':''+Drupal.settings.mylearning.user.survey_objectid+'','userId':''+Drupal.settings.mylearning.user.survey_userid+'','LaunchFrom':'ME','defaultContent':''+Drupal.settings.mylearning.user.survey_defaultcontent+'','pagefrom':'launch',});
	} else {
		$('#learningplan-tab-inner').data('contentPlayer').playContent({'masterEnrollId':''+Drupal.settings.mylearning.user.survey_enrollid+'','enrollId':'0','programId':''+Drupal.settings.mylearning.user.survey_objectid+'','isMultiLesson':'0','contentType':'survey','cType':'survey','contentId':''+Drupal.settings.mylearning.user.survey_id+'','LessonId':'','VersionId':'','url1':'','courseId':'0','classId':'0','url2':'','ErrMsg':'','Status':'','LessonLocation':'','launch_data':'','suspend_data':'','exit':'','AICC_SID':'','MasteryScore':'','remDays':'undefined','ValidityDays':'undefined','no_of_attempts':'null','AttemptLeft':'undefined','attempts':'null','MaxAttempt':'undefined','surveycount':''+Drupal.settings.mylearning.user.survey_status+'','is_post_ass_launch':'undefined','playStatus':'true','defaultContent':''+Drupal.settings.mylearning.user.survey_defaultcontent+'','pagefrom':'','LaunchFrom':'LP',});
	}
}